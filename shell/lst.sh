#!/bin/bash
#=========================================================
#                         lst
#=========================================================
# Purpose: ls *.text
#          A wrapper/filter to the command 'ls.'
#          List all text-like files in the current
#          working directory (i.e., ./*).
#          Precisely, all files that yield          
#          a file type name containing the word "text"
#          when inspected with the Linux command 'file'          
# -------------------------------------------------------
# Author:  Dohyun Ahn
# -------------------------------------------------------
# Usage:   - lst
#            Default. Same as lst *
#          - lst [TARGET_1] ... [TARGET_N]
#            Same effect as ls, but
#            with non-text files filtered
# -------------------------------------------------------
# Note:    All 'ls' flags (e.g., -a, -l)
#          are still admissible
#
#=========================================================


# Types to look for: all types that contain substring "text"
FIND_TYPE="text"
FIND_FLAGS="-maxdepth 0 -type f -print0"

#=========================================================
# ls -flags and *targets* separation 
#=========================================================
LS_FLAGS=(   )
LS_TARGETS=( )

#=========================================================
# Separates switchs/target paths
# in given 'ls'-like args
#=========================================================
for ARGI in $@
do
    case $ARGI in
    -*) 
        LS_FLAGS+=( $ARGI );;
     *)
        # Convert dir => dir/*
        if [ -d "${ARGI}" ]
        then
            ARGI="${ARGI%/}/*"
        fi
        
        LS_TARGETS+=( $ARGI );;
    esac
done

#=========================================================
# Default target: pwd/*
#=========================================================
if [[ ! ${LS_TARGETS[@]} ]]
then
    LS_TARGETS=( * )
fi

# Ignore empty dir/* expansion
shopt -s nullglob

#=========================================================
# Save type-matching filenames into string output 
#=========================================================
LST_OUTPUT=\
$(  
    while read -r -d '' file; do 
        if file $file | grep -q ${FIND_TYPE}; then
            echo $file
        fi
    done < <(find "${LS_TARGETS[@]}" ${FIND_FLAGS})
 )

#=========================================================
# Print that output if not empty
#=========================================================
if [ ! -z "${LST_OUTPUT}" ]
then
    ls "${LS_FLAGS[@]}" ${LST_OUTPUT}
fi
