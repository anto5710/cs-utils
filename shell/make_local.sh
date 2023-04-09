#!/bin/bash

#=========================================================
#                        make_local
#=========================================================
# Purpose: make 40 'C99' executables with
#          pre-downloaded libs
# -------------------------------------------------------
# Author:  Dohyun Ahn
# -------------------------------------------------------
# Usage:   make_local ...
# -------------------------------------------------------
# Note:    All 'make' flags (e.g., -B)
#          are still admissible
#=========================================================


BUILD40="${CS40}/build"
IFLAGS40="-I${BUILD40}/include40/include -I${BUILD40}/cii40/include/cii"
LDFLAGS40="-L${BUILD40}/include40/lib -L${BUILD40}/libhalligan -L${BUILD40}/cii40/lib64"

alias make_local 'make CC+="gcc -no-pie" LDFLAGS+="${LDFLAGS40}" IFLAGS40+="${IFLAGS40}"'
alias makel 'make_local'
alias lmake 'make_local'
