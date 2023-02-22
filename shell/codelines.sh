#!/bin/bash

ARGC=$#

#=========================================================
# ANSI Color & Styling codes (print $R to reset style)
#=========================================================
R="\e[m"     # RESET
B="\e[1m"    # Bold
I="\e[3m"    # Italic
U="\e[4m"    # Underline
GB="\e[40m"  # background: Green
FW="\e[37m"  # font: White
FG="\e[32m"  # font: Green
FGG="\e[92m" # font: dark green
FY="\e[93m"  # font: yellow
FRR="\e[91m" # font: dark red
FR="\e[31m"  # font: red
FP="\e[35m"  # font: purple
W="\e[2m"    # Dim

THIS="${B}codelines${R}"
NON_WS_COMMENTS='^\s*$|^\s*//\.*|^\s*/[*]\.*|^\s*[*]/\.*'
ARGS="$@"

if [[ ${ARGC} -eq 0 ]]; then
    ARGS="$(find * -maxdepth 0 -type f | xargs)"
fi

echo -e \
"${THIS}: grep --color=always. -cvE ${FG}'$NON_WS_COMMENTS'${R} ${ARGS}"
grep --color=always -cvE "\'${NON_WS_COMMENTS}\'" ${ARGS}
