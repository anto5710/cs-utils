#!/bin/bash
#=========================================================
#                        latest
#=========================================================
# Author: Chris Ahn (dahn01), Spring 2023, Tufts Univ.
#
THIS="latest"
USAGE=\
"Usage: ${THIS} [-h|-q]
 Args:  -h: Help
        -q: Quiet mode (hides ls -lt)."
#=========================================================
HELP=\
"${THIS}: Bash script to inspect latest modified file(s) in current dir

      Computes file(s) with the latest time (allows tie) stamp using \`ls -lt\`,
      And reports the difference from current system time.

${USAGE}\n"
#=========================================================

ARGC=$#
ARG="$@"
NO_LS_LT=""

usage_abort () {
    printf "${USAGE}\n\a"
    exit 1
}

help_quit () {
    printf "${HELP}"
    exit 0
}

#=========================================================
# Screen command-line args, either: NONE or '-h'
#=========================================================
if [[ ${ARGC} != 0 ]] && [[ ${ARGC} != 1 ]] ; then
    usage_abort
fi

if [[ ${ARGC} == 1 ]]; then
    case "${ARG}" in
       -h | --h | --help  ) help_quit       ;;
       -q | --q | --quiet ) NO_LS_LT="true" ;;
                         *) usage_abort     ;;
    esac
fi

MAX_COUNTDOWN_SECS=3
MAX_PWD_UNIXTIME=0
LATEST_FILES=""
NOW_UNIXTIME=$(date +"%s")

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

#=========================================================
# Find file(s) with max Unix seconds (latest time stamp)
#=========================================================
for file in $(ls); do
    FILE_LAST_UNIXTIME=$(date -r "$file" +"%s") || FILE_LAST_UNIXTIME=0

    # If current file is newer, set (latest) = (curr file).
    if [ "${FILE_LAST_UNIXTIME}" -gt "${MAX_PWD_UNIXTIME}" ]; then
        MAX_PWD_UNIXTIME="${FILE_LAST_UNIXTIME}"
        LATEST_FILES="${file}"

    # Tie (include both)
    elif [ "${FILE_LAST_UNIXTIME}" -eq "${MAX_PWD_UNIXTIME}" ]; then
        LATEST_FILES="${LATEST_FILES}\t${file}"
    fi
done

DIFF_SEC=$(expr ${NOW_UNIXTIME} - ${MAX_PWD_UNIXTIME})
DIFF_MIN=0
DIFF_HOUR=0
DIFF_DAY=0

ST_SEC=""
ST_MIN=""
ST_HOUR=""
ST_DAY=""

style_sec() {
    ST_SEC="${GB}${FW}${B} ${FGG}${DIFF_SEC} sec ${R}"
}

style_min() {
    ST_MIN="${GB}${FW}${B} ${FY}${DIFF_MIN} min ${R}"
}

style_hour() {
    ST_HOUR="${GB}${FW}${B} ${FRR}${DIFF_HOUR} hour ${R}"
}

style_day() {
    ST_DAY="${GB}${FW}${B} ${FP}${DIFF_DAY} day ${R}"
}

#==============================================================
# Style time stamp message (default: in sec)
#==============================================================
style_sec
DIFF_MSG="${ST_SEC}"

#==============================================================
# Reduce sec(s) -> min(s)
#==============================================================
if [ "${DIFF_SEC}" -gt 60 ]; then
    DIFF_MIN=$(expr ${DIFF_SEC} / 60)
    DIFF_SEC=$(expr ${DIFF_SEC} % 60)
    style_sec
    style_min
    DIFF_MSG="${ST_MIN}"
fi

#==============================================================
# Reduce min(s) -> hour(s)
#==============================================================
if [ "${DIFF_MIN}" -gt 60 ]; then
    DIFF_HOUR=$(expr ${DIFF_MIN} / 60)
    DIFF_MIN=$(expr ${DIFF_MIN} % 60)
    style_hour
    style_min
    DIFF_MSG="${ST_HOUR}:${ST_MIN}"
fi

#==============================================================
# Reduce hour(s) -> days(s)
#==============================================================
if [ "${DIFF_HOUR}" -gt 24 ]; then
    DIFF_DAY=$(expr ${DIFF_HOUR} / 24)
    DIFF_HOUR=$(expr ${DIFF_HOUR} % 24)
    style_day
    style_hour
    DIFF_MSG="${ST_DAY}:${ST_HOUR}"
fi

#==============================================================
# Print ls -lt if not turned off
#==============================================================
if [[ ${NO_LS_LT} != "true" ]]; then
    LINE="${W}----------------------------------------------------------${R}"
    echo -e "${LINE}"
    ls -lt
    echo -e "${LINE}"
fi

echo -e -n "${B}${THIS}:${R} last updated: ${U}${I}${LATEST_FILES}${R} ${W}"
echo -e "----${R} ${DIFF_MSG}:ago"