#!/bin/bash
# testmake

CXX=clang++
CXXFLAGS="-c -g3 -std=c++11 -Wall -Wextra -Wpedantic \
          -Wshadow -Wno-deprecated -Wno-zero-length-array"
LDFLAGS="-g3 -pthread"
HEADER="\e[1mtestmake: \e[m"

dest_name () {
    FILE_DEST="${FILE}.testc"
}

test_compile () {
    compile
    clean
}

compile () {
    dest_name
    COMPILE_OUTPUT="$(${CXX} ${CXXFLAGS} ${LDFLAGS}\
                      -o ${FILE_DEST} -c ${FILE} 2>&1)"
    print_compile_result
}

print_compile_result() {
    if [ -z "$COMPILE_OUTPUT" ]
    then
        if [ ! $SILENT == "true" ]
        then 
            echo -e -n "${HEADER}successfully compiled \
\e[4;1;3m${FILE}\e[m -> \e[32;4;1;3m${FILE_DEST}\e[m\t"
            
        fi
        PASS_FILES="$(($PASS_FILES + 1))"
    else
        ${CXX} ${CXXFLAGS} ${LDFLAGS} -o ${FILE_DEST} -c ${FILE}
        FAIL_FILES="$(($FAIL_FILES + 1))"
    fi
}

clean () {
    if [ -f $FILE_DEST ]
    then
        if [ ! $SILENT == "true" ]; then 
            echo -e "\e[2m ... (cleaning) \e[m"; 
        fi
        rm ${FILE_DEST}
    else
        if [ ! $SILENT == "true" ]; then echo; fi
    fi
}

double_clean () {
    rm -f ./*.testc.tmp
}

print_summary() {
    if [ $((PASS_FILES + FAIL_FILES)) -gt 0 ]
    then 
        echo -e "${HEADER}\
Compiled: \e[32;1m${PASS_FILES}\e[m / \
Erred: \e[31;1m${FAIL_FILES}\e[m files"
    fi
}

PASS_FILES=0
FAIL_FILES=0
SILENT="false"

ARGC="$#"
DEFAULT=""*.cpp" "*.h""

if [ "$ARGC" -gt 0 ] && [ "$1" == "-s" ]
then
    SILENT="true"
    ARGC=$(( $ARGC - 1 ))
fi

TARGETS="${DEFAULT}"
if [ ${ARGC} -gt 0 ]
then
    TARGETS="$@"
fi

TARGETS_LIST=($TARGETS)

NON_ZERO_TARGET="false"
for arg in $TARGETS; do
    if [ -f $arg ]; then NON_ZERO_TARGET="true"; fi
done

if [ ${NON_ZERO_TARGET} == "true" ]; then
    echo -e "${HEADER}compiling ${TARGETS_LIST[@]}"
fi

for arg in $TARGETS
do
    if [ -f $arg ]; then
        FILE="$arg"
        test_compile
    fi
done

double_clean
print_summary 

