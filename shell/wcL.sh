#!/bin/bash
#==============================================================
#                             wcL
#==============================================================
# Author:  Dohyun Ahn
#--------------------------------------------------------------
# Purpose: Asserts that no *text* file in target
#          exceeds set column limit (80 chars).
#
#          I.e) Asserts if
#               1) `file $TARGET` contains "text"
#               2) `wc -L $TARGET` > 80
#--------------------------------------------------------------
# Usuage:  wcL [-s] [-MAX_COLS]
#          - Scans pwd/* by default
#
#          wcL [-s] [-MAX_COLS] [TARGET1] [TARGET2] ...
#          - Scans all files (in) targets
#--------------------------------------------------------------
# Flags:   [-s]:
#               Remains silent if not erred
#
#          [-MAX_COLS]:
#               Asserts for given upper limit (default: 80)
#==============================================================

#==============================================================
MAX_COLS=80
#==============================================================
#    Config:: Set to check if any file > 80 characters/line
#==============================================================

#==============================================================
# Select targets in which to find
#==============================================================
ARGC=$#
FIND_TARGETS=""
SILENT_MODE=false

#==============================================================
# Search for '-s' switch
#==============================================================
for ARGI in $@; do
    case $ARGI in
         -s) SILENT_MODE=true       ;;
    -[0-9]*) MAX_COLS=${ARGI#-}     ;;
          *) FIND_TARGETS+=" $ARGI" ;;
    esac
done

# Default target = pwd/*
if [ -z "$FIND_TARGETS" ]; then
    FIND_TARGETS="*"
fi

#==============================================================
# Set up necessary 'find' flags
#==============================================================
FIND_FLAGS="-maxdepth 0 -type f -print0"
FIND_TYPE="text"

WARN_STYLE="\e[31m"
SAFE_STYLE="\e[32m"

# STYLE_GRAD=( 40 34 70 106 142 178 209 202 196 )
STYLE_GRAD=(40 34 70 106 142 178 209)

BEEP="\007"
PARTITIONER="\t"

TOTAL_COLS=0
POST_PRINTED=false

#==============================================================
# Update maximum total columns
#==============================================================
update_max_cols() {
    if ((${CUR_COLS} > ${TOTAL_COLS})); then
        TOTAL_COLS=${CUR_COLS}
    fi
}

#==============================================================
# Whether erred or non-silent, print wc -L output, styled
#==============================================================
print_file_wcl() {
    if ((${VIOLATED_COLS})); then
        CUR_STYLE=${WARN_STYLE}
    else
        VAC=$((${#STYLE_GRAD[@]} - 1 - \
        (((${MAX_COLS} - ${CUR_COLS}) / 2) % ${#STYLE_GRAD[@]})))

        CUR_STYLE="\e[38;5;${STYLE_GRAD[$VAC]}m"
        # CUR_STYLE=${SAFE_STYLE}
    fi

    echo -e -n "${CUR_STYLE}\e[1m${CUR_COLS}\e[22m${PARTITIONER}"
    echo -e "\e[4m${file}\e[m"
}

#==============================================================
# Determine wc -L of current file and whether it is > 80
#==============================================================
update_file_wcl() {
    CUR_COLS=$(wc -L <${file})
    VIOLATED_COLS=$((${CUR_COLS} > ${MAX_COLS}))
}

#==============================================================
# Iterate through each file in TARGETS
# to assert <= 80 cols limit.
#==============================================================
while read -r -d '' file; do
    #----------------------------------------------------------
    # If  1) file.type contains "text"
    # AND 2) wc L {} exceeds 80 cols
    #----------------------------------------------------------
    if file $file | grep -q ${FIND_TYPE}; then
        update_file_wcl
        update_max_cols

        if ! ${SILENT_MODE} || ((${VIOLATED_COLS})); then
            print_file_wcl
            POST_PRINTED=true
        fi
    fi
done < <(find ${FIND_TARGETS} ${FIND_FLAGS})

#==============================================================
# Total (maximum) columns. Beep if violative
#==============================================================
if ! ${POST_PRINTED}; then
    PARTITIONER=" "
fi

echo -e -n "\e[1m${TOTAL_COLS}\e[22m"
echo -e     "${PARTITIONER}total\e[m"

if (($TOTAL_COLS > $MAX_COLS)); then
    echo -e -n ${BEEP} >/dev/tty
fi
