#!/bin/sh
#=========================================================
#                        omake
#=========================================================
# 
# 
#=========================================================
HEADER="\e[1momake:\e[22m"

#========================================================
# Compiler setup
#========================================================
CXX="clang++"
CXXFLAGS="-g3 -std=c++11 -Wall -Wextra -Wpedantic -Wshadow"
LDFLAGS="-g3 -pthread"

#========================================================
# Select target files, first of which = main.cpp
#========================================================

DEFAULT_TARGETS=`ls *.cpp`

ARGC=$#
if [ ${ARGC} -le 0 ]; then
    #----------------------------------------------------
    # Default target: pwd/*.cpp
    #----------------------------------------------------
    TARGETS=${DEFAULT_TARGETS}
    MAIN_TARGET=`echo ${TARGETS} | head -n1 | awk '{print $1;}'`
else
    MAIN_TARGET="$1"
    TARGETS="$@"
fi

for O in *.o
do
    TARGETS=`echo "${TARGETS}" | sed "s/${O%.o}.cpp//g"`
done

TARGETS=`echo "${TARGETS}" \
|sed "s/\s\+/\n/g" | sort -u | tr "\n" " "`

#========================================================
# If no *.cpp at all, abort
#========================================================
if [ -z "${MAIN_TARGET}" ]; then
    printf "${HEADER} missing operand"
    echo
    exit
fi

#========================================================
# Set -o MAIN <= given "MAIN.cpp" (remove *.cpp suffix)
# If such executable exists, update with 'make'
#========================================================
PURIFIED="${MAIN_TARGET%.cpp}"

if [ -f "${PURIFIED}" ]; then
    make ${PURIFIED}
else 
    #----------------------------------------------------
    # Preview and run clang++ -o ... *.cpp 
    #----------------------------------------------------
    UNDERLINED_TARGETS=$(echo ${TARGETS} \
    | sed -e 's/\(\S*\)/\\e[4m\1\\e[24m/g')

    printf "${HEADER} ${CXX} -o \e[1;3m${PURIFIED}\e[22;23m "
    printf "${UNDERLINED_TARGETS} \e[2m${CXXFLAGS} ${LDFLAGS}\e[22m"
    echo

    ${CXX} ${CXXFLAGS} ${LDFLAGS} -o ${PURIFIED} ${TARGETS}
fi

#========================================================
# Run resulting executable in following
#========================================================
printf "${HEADER} ./${PURIFIED}"
echo

./${PURIFIED}

