#!/bin/bash

HEADER="${B}omake:${R}"

#========================================================
#                 ANSI STYLING TABLE
#========================================================
B="\e[1m"
U="\e[4m"
I="\e[3m"
R="\e[m"
W="\e[2m"
NW="\e[22m"
F_G="\e[32m"

#========================================================
#                   COMPILER SETUP
#========================================================
CXX="clang++"
CXXFLAGS="-g3 -std=c++11 -Wall -Wextra -Wpedantic -Wshadow"
LDFLAGS="-g3 -pthread"

#========================================================
# INT_MAINX: a regex for space-free prefix: int main() {
#========================================================
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
ARGC=$#

PRIMAIN=""
PRIMAIN_OUT=""

# We only add these as sources in compile arg list
SOURCE_TYPEX='.*\.cpp|.*\.c'

# Default 'main cpp' candidates: cmd args
PRIMAIN_CANDIDATES=("$@")
PWD=$(pwd)

#========================================================
# Fill the PRIMAIN_CANDIDATES in time (recentness) order.
#========================================================
sortfill_candidates_pwd() {
    while read -r -d '' _ file; do
        PRIMAIN_CANDIDATES+=("${file##"./"}")
    done < <(find -maxdepth 1 -type f \
        \( -iname '*.cpp' -o          \
           -iname '*.c'   -o          \
           -iname '*.h'   -o          \
           -iname '*.inl' \)          \
        -printf "%T@ %p\0" | sort -z -k1,1gr)
}

#========================================================
# Find the blue: the latest int-main()-bearing file
#========================================================
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
    #----------------------------------------------------
    # If candidates given as args, just use thme.
    #----------------------------------------------------
    find_blue
else
    #----------------------------------------------------
    # Else, obtain candidates from pwd, then find blue.
    #----------------------------------------------------
    PRIMAIN_CANDIDATES=()
    sortfill_candidates_pwd
    find_blue
fi

#========================================================
# If not found blue (the main cpp), abort
#========================================================
on_found_blue() {
    echo -e -n "${HEADER} Located primain (.cpp): "
    echo -e "${U}${W}${PWD}${NW}/${PRIMAIN}${R}"

    PRIMAIN_OUT="${PRIMAIN%%.*}"

    echo -e -n "${HEADER} Compile executable ---> "
    echo -e "${U}${W}${PWD}${NW}/${F_G}${I}${PRIMAIN_OUT}${R}"
}

on_no_blues_abort() {
    echo -e "${HEADER} Unable to locate a primain (.cpp)."
    exit 1
}

if [[ -z ${PRIMAIN} ]]; then
    on_no_blues_abort
else
    on_found_blue
fi

#========================================================
# Declare (Hash)Sets:
# - DEP_BASE_SET:  Source file[basename]s that have been appended to args
# - VISITED_SET:   Visited files
# Then a list (queue):
# - TO_VISIT_Q:    Queue of files-to-visit
# Then actual args to be compiled
# - DEP_LIST:      String-appending of source args to compile
# - DEP_LIST_FANCY Fancied version for preview
#========================================================
declare -A DEP_BASE_SET
declare -A VISITED_SET
TO_VISIT_Q=("${PRIMAIN}")

DEP_LIST="${PRIMAIN}"
DEP_LIST_FANCY="${B}${I}${U}${PRIMAIN}${R}"

#========================================================
# Enqueue files to later visit, in-order
#========================================================
enqueue_includes() {
    for cur_source in “${H_ASSOCIATES[@]}“; do
        if [[ -e "${cur_source}" ]]; then
            TO_VISIT_Q+=("${cur_source}")
        fi
    done
}

#========================================================
# Iterate over given base-name's associates: {.cpp, .c, .h, ...}
# and collect the latest modified time
# (if greater than initial $TOP_LAST_UNIXTIME) &
# and collect the latest modified 'source' type file {.cpp, .c}.
#========================================================
search_latest_source() {
    for cur_source in "${H_ASSOCIATES[@]}"; do
        if [[ -e ${cur_source} ]]; then
            CUR_LAST_UNIXTIME=$(date -r "${cur_source}" +"%s") \
         || CUR_LAST_UNIXTIME=0

            #---------------------------------------------------
            # Update if newer than preivous top modified time
            #---------------------------------------------------
            if [[ ${CUR_LAST_UNIXTIME} -gt ${TOP_LAST_UNIXTIME} ]]; then
                TOP_LAST_UNIXTIME="${CUR_LAST_UNIXTIME}"

                #-----------------------------------------------
                # Set new source file if newer & is of source type
                #-----------------------------------------------
                if [[ "${cur_source}" =~ ${SOURCE_TYPEX} ]]; then
                    TOP_LAST_SOURCE="${cur_source}"
                fi
            fi
        fi
    done
}

#========================================================
# Check the top source select -> compile -c if not .o yet
# then finally append .o to the args.
#========================================================
compile_append_source() {
    if [[ ! -z "${TOP_LAST_SOURCE}" ]]  \
    && [[   -e "${TOP_LAST_SOURCE}" ]]; then
        #------------------------------------------------
        # Compile -c if non .o file
        #------------------------------------------------
        if ! [[ "${TOP_LAST_SOURCE}" =~ .*\.o ]]; then
            ${CXX} ${CXXFLAGS} -c ${TOP_LAST_SOURCE} -o "${H_BASENAME}.o"
            TOP_LAST_SOURCE="${H_BASENAME}.o"
        fi

        #------------------------------------------------
        # Add the top source select to dep lists & mark it appended
        #------------------------------------------------
        DEP_BASE_SET["${H_BASENAME}"]=1
        DEP_LIST+=" ${TOP_LAST_SOURCE}"
        DEP_LIST_FANCY+=" ${U}${TOP_LAST_SOURCE}${R}"
    fi
}

#========================================================
# Given a start-point (an included file),
# Explore all its same-base-name associates,
# Find the latest source (compile -c if not .o),
# and append to the dep args.
#========================================================
stash_associateds() {
    H_BASENAME="${H_HEADER%%.h}"
    H_ASSOCIATES=(
        "${H_HEADER}"       \
        "${H_BASENAME}.cpp" \
        "${H_BASENAME}.c"   \
        "${H_BASENAME}.inl" \
        "${H_BASENAME}.h"   )

    TOP_LAST_UNIXTIME=0
    TOP_LAST_SOURCE=""
    #------------------------------------------------
    # If found any pre-compiled .o,
    # Set it the default go-to option,
    # if no other sources are found/newer than it
    #------------------------------------------------
    if [[ -e "${H_BASENAME}.o" ]]; then
        TOP_LAST_UNIXTIME=$(date -r "${H_BASENAME}.o" +"%s")
        TOP_LAST_SOURCE="${H_BASENAME}.o"
    fi

    enqueue_includes
    #------------------------------------------------
    # If not appended this source[basename] yet, select latest & append it.
    #------------------------------------------------
    if [[ ! "${DEP_BASE_SET[${H_BASENAME}]}" ]]; then
        search_latest_source
        compile_append_source
    fi
}

#========================================================
# Starting from the premain (main cpp),
# Recursively stash all its depedency tree
# to the compile source args (DEP_LIST),
# compiling -c parts if needed.
#========================================================
stash_all_from_premain() {
    V_I=0

    #----------------------------------------------------
    # Visit all files in queue, stashing its #included files,
    # (possibly adding more files to visit amid)
    #----------------------------------------------------
    while [[ V_I -lt ${#TO_VISIT_Q[@]} ]]; do
        file="${TO_VISIT_Q[${V_I}]}"
        ((V_I += 1))

        #------------------------------------------------
        # Handle iff it's a text file & not visited yet
        #------------------------------------------------
        if [[ ! "$(file ${file})" =~ .*text ]] \
        || [[   "${VISITED_SET[${file}]}"   ]]; then
            continue
        fi

        #------------------------------------------------
        # Extract all '#includes' and stash their content
        #------------------------------------------------
        while IFS= read -r H_HEADER; do
            stash_associateds
        done < <(grep -oP "[^\s"\""\n]+\.(h|inl)" "${file}")

        # Set VISITED true
        VISITED_SET["${file}"]=1
    done
}

final_compile() {
    echo -e "${CXX} ${W}${CXXFLAGS}${NW} \\"
    echo -e "   -o ${F_G}${I}${PRIMAIN_OUT}${R} ${DEP_LIST_FANCY}"
    echo -e "----------------------------------------------------------------------"
    ${CXX} ${CXXFLAGS} -o ${PRIMAIN_OUT} ${DEP_LIST}
}


#========================================================
# COMPILE & EXECUTE
#========================================================
final_compile
./${PRIMAIN_OUT}

exit 0
