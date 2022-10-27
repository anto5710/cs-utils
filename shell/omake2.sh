#!/bin/bash

B="\e[1m"
U="\e[4m"
I="\e[3m"
R="\e[m"
W="\e[2m"
NW="\e[22m"
F_G="\e[32m"

HEADER="${B}omake:${R}"

#========================================================
# Compiler setup
#========================================================
CXX="clang++"
CXXFLAGS="-g3 -std=c++11 -Wall -Wextra -Wpedantic -Wshadow"
LDFLAGS="-g3 -pthread"

# S_="[\s\n]"
# NAMEX="[\_a-zA-Z][\w\_]*"
# CHARP_ARR_PARAMX="char${S_}*\*${S_}*${NAMEX}${S_}*\[${S_}*\]"
# CHARPP_PARAMX="char${S_}*\*${S_}*\*${S_}*${NAMEX}${S_}*"
# ARGC_PARAMX="int${S_}+${NAMEX}"
# ARGV_PARAMX="(${CHARP_ARR_PARAMX}|${CHARPP_PARAMX})"
# INT_MAIN_PARAMX="${ARGC_PARAMX}${S_}*,${S_}*${ARGV_PARAMX}"
# INT_MAINX="int${S_}*main${S_}*\(${S_}*(${INT_MAIN_PARAMX})?${S_}*\)${S_}*"

INT_MAINX="int[\s\n]*main[\s\n]*\([\s\n]*(int[\s\n]+[\_a-zA-Z][\w\_]*[\s\n]*\,\
[\s\n]*(char[\s\n]*\*[\s\n]*[\_a-zA-Z][\w\_]*[\s\n]*\[[\s\n]*\]|\
char[\s\n]*\*[\s\n]*\*[\s\n]*[\_a-zA-Z][\w\_]*[\s\n]*))?[\s\n]*\)[\s\n]*"

FILES=(*)

PRIMAIN=""
PRIMAIN_OUT=""
ARGC=$#
ARGS=$@

PRIMAIN_CANDIDATES=()
PWD=$(pwd)

time_sorted_ls() {
    # echo "${FIND_FLAGS[@]}"
    find -maxdepth 1 -type f -iname '*.cpp'

    while read -r -d '' _ file; do
        PRIMAIN_CANDIDATES+=("${file##"./"}")
    done < <(find -maxdepth 1 -type f -iname '*.cpp' \
        -printf "%T@ %p\0" | sort -z -k1,1gr)
}

find_blue() {
    for file in "${PRIMAIN_CANDIDATES[@]}"; do
        if [[ ! -z $(grep -oP "${INT_MAINX}" "${file}") ]]; then
            PRIMAIN="${file}"
            break
        fi
    done
}

#========================================================
# Select target files, first of which = main.cpp
#========================================================

if [ ${ARGC} -gt 0 ]; then
    PRIMAIN="$1"
else
    time_sorted_ls
    find_blue
fi

if [[ -z ${PRIMAIN} ]]; then
    echo -e "${HEADER} Unable to locate a primain (.cpp)."
    exit 1
else
    echo -e "${HEADER} Located primain (.cpp): ${U}${W}${PWD}${NW}/${PRIMAIN}${R}"

    PRIMAIN_OUT="${PRIMAIN%%.*}"

    while [[ -e "${PRIMAIN_OUT}" ]]; do
        PRIMAIN_OUT+=".out"
    done

    echo -e "${HEADER} Compile executable ---> ${U}${W}${PWD}${NW}/${F_G}${I}${PRIMAIN_OUT}${R}"
fi

declare -A VISITEDS
TO_VISITS=("${PRIMAIN}")
declare -A DEPS

DEPS_L="${PRIMAIN}"
DEPS_FL="${B}${I}${U}${PRIMAIN}${R}"

stash_source() {
    H_BASENAME="${header%%.h}"

    if [[ -e "${H_BASENAME}.o" ]]; then
        if [[ ! "${DEPS[${H_BASENAME}.o]}" ]] && [[ ! "${DEPS[${H_BASENAME}.cpp]}" ]]; then
            DEPS["${H_BASENAME}.o"]=1
            DEPS_L+=" ${H_BASENAME}.o"
            DEPS_FL+=" ${U}${H_BASENAME}.o${R}"
        fi
    fi

    if [[ -e "${H_BASENAME}.cpp" ]]; then
        if [[ ! "${DEPS[${H_BASENAME}.o]}" ]] && [[ ! "${DEPS[${H_BASENAME}.cpp]}" ]]; then
            DEPS["${H_BASENAME}.cpp"]=1
            DEPS_L+=" ${H_BASENAME}.cpp"
            DEPS_FL+=" ${U}${H_BASENAME}.cpp${R}"
        fi
    fi

    if [[ -e "${H_BASENAME}.h" ]]; then
        TO_VISITS+=("${H_BASENAME}.h")
    fi

    if [[ -e "${H_BASENAME}.cpp" ]]; then
        TO_VISITS+=("${H_BASENAME}.cpp")
    fi
}

V_I=0
while [[ V_I -lt ${#TO_VISITS[@]} ]]; do
    file="${TO_VISITS[${V_I}]}"
    ((V_I += 1))

    if [[ ! "$(file ${file})" =~ .*text ]] ||
        [[ "${VISITEDS[${file}]}" ]]; then
        continue
    fi
    echo visti "${file}"

    while IFS= read -r header; do
        stash_source
    done < <(grep -oP "[^\s"\""\n]+\.h" "${file}")

    VISITEDS["${file}"]=1
done

echo -e "${HEADER} Collected: ${DEPS_FL}"

${CXX} ${CXXFLAGS} -o ${PRIMAIN_OUT} ${DEPS_L}
