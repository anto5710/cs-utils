#!/bin/bash

B="\e[1m"
I="\e[3m"
R="\e[m"
W="\e[2m"
F_G="\e[32m"
F_Y="\e[33m"
HEADER="${B}deduplicate${R}"

ARGC=$#
ARGS=("$@")

# Default dir = pwd
DIRS=""
DEFAULT_DIR="$(pwd)"

FIND_OPTIONS="-type f"
FIND_HIDDEN=""

#==============================================================
# Wrong usage. Beep if violative
#==============================================================
EXIT_FAILURE=1
abort_usage() {
    echo "Usage: ./deduplicate.sh [OPTIONS] (DIR)"
    exit ${EXIT_FAILURE}
}

IS_ONSET="true"

apply_switch() {
    case ${ARGI} in
    -a | --a | -all)
        FIND_HIDDEN="true"
        ;;
    -*) FIND_OPTIONS+=" ${ARGI}" ;;
    *) ;;
    esac
}

append_dir() {
    IS_ONSET="false"
    DIRS+=" ${ARGI}"
}

#=========================================================
# Separates switchs/target paths
#=========================================================
for ARGI in "${ARGS[@]}"; do
    case ${ARGI} in
    \\-*)
        ARGI="${ARGI#\\}" ## sanitize the escaper (backslash)
        append_dir
        ;;
    -*)
        if [[ "${IS_ONSET}" == "true" ]]; then
            apply_switch
        else
            append_dir
        fi
        ;;
    *)
        append_dir
        ;;
    esac
done

#=========================================================
# Set current dir as target if missing
#=========================================================
if [[ -z ${DIRS} ]]; then
    DIRS="${DEFAULT_DIR}"
fi

#=========================================================
# Opt whether to search hidden (*/.*) files
#=========================================================
if [[ -z "${FIND_HIDDEN}" ]]; then
    FIND_OPTIONS+=" -not -path '*/.*'"
    echo -e "${HEADER}: ${W}Ignorning hidden files '*/.*' by default...${R}"
else
    echo -e "${HEADER}: Searching hidden files '*/.*' inclusive"
fi

COL_SEP="\t"
RES="$(find "${DIRS[@]}" ${FIND_OPTIONS} -printf "%s${COL_SEP}%p\n")"

FILES=()
readarray -t FILES < <(echo "$RES" | sort -k 1 -n)
FILE_I=0
FILES_SIZE=${#FILES[@]}

RES_PRINT=""

while [ ${FILE_I} -lt ${FILES_SIZE} ]; do
    FILE_J=$((FILE_I + 1))
    FILE_A_LINE="${FILES[${FILE_I}]}"
    FILE_A="$(echo "${FILE_A_LINE}" | head -n1 | awk '{print $2;}')"
    FILE_A_BYTES=$(echo "${FILE_A_LINE}" | head -n1 | awk '{print $1;}')

    STILL_EQUALS="true"
    FOUND_DUPE=""

    while [ ${FILE_J} -lt ${FILES_SIZE} ]; do
        FILE_B_LINE="${FILES[${FILE_J}]}"
        FILE_B="$(echo "${FILE_B_LINE}" | head -n1 | awk '{print $2;}')"
        FILE_B_BYTES=$(echo "${FILE_B_LINE}" | head -n1 | awk '{print $1;}')

        if [[ ${FILE_A_BYTES} -eq ${FILE_B_BYTES} ]]; then

            if [[ -z ${FOUND_DUPE} ]]; then
                printf "============= duplicates =============\n"
                printf "${FILE_A}\n"
                # echo -e -n "${W}${FILE_A%/*}${R}/${F_G}${FILE_A##*/}${R}\n"
                FOUND_DUPE="true"
            fi
            printf "${FILE_B}\n"
            # echo -e -n "${W}${FILE_B%/*}${R}/${F_Y}${FILE_B##*/}${R}\n"
        else
            break
        fi
        FILE_J=$((FILE_J + 1))
    done

    FILE_I=$((FILE_I + 1))
done

# echo -e -n "${RES_PRINT}"
