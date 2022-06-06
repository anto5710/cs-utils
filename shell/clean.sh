#!/bin/sh
#========================================================
#                         clean
#========================================================
# Author: Dohyun Ahn
# 
# Formalization of make clean
# Likewise, defaults to a.out *.o, *.gch, *~, a.out
# 
# Wraps rm: passes down rm flags
#========================================================

#========================================================
# Splice switches from target/file args
#========================================================
ARGS="$@"
FLAGS="-I"
TARGETS=""
HEADER="\e[1mclean:\e[22m"

for ARG in $ARGS; do
    case $ARG in
        -*) FLAGS="${FLAGS} ${ARG}";;
         *) TARGETS="${TARGETS} ${ARG}";;
    esac
done

#========================================================
# If no target, default to *.o *.gch ....
#========================================================
if [ -z "$TARGETS" ]; then
    TARGETS=`find * -maxdepth    0\
            -name "*.o"         -o\
            -name "*.gch"       -o\
            -name "a.out"       -o\
            -name "*.stdout"    -o\
            -name "*.stderr"    -o\
            -name "*~"     -type f`
fi

#========================================================
# If some target, notify and proceed deletion
#========================================================
if [ ! -z "$TARGETS" ]; then

    printf "${HEADER} removing ... "
    #----------------------------------------------------
    # Print targets underlined in single line
    #----------------------------------------------------
    for TARGET in $TARGETS; do
        printf "\e[4m${TARGET}\e[24m "
    done
    echo

    #----------------------------------------------------
    # Deletion
    #----------------------------------------------------
    rm ${FLAGS} ${TARGETS}
else
    printf "${HEADER} missing operand"; echo
fi
