#!/bin/bash
#=========================================================
#                         nower
#=========================================================

MAX_COUNTDOWN_SECS=3
MAX_PWD_UNIXTIME=0
LATEST_FILES=""
NOW_UNIXTIME=$(date +"%s")

B="\e[1m"
I="\e[3m"
R="\e[m"
U="\e[4m"
GB="\e[40m"
FW="\e[37m"
FG="\e[32m"
FGG="\e[92m"
FY="\e[93m"
FRR="\e[91m"
FR="\e[31m"
FP="\e[35m"
BG="\e[100m"

for file in $(ls); do
    FILE_LAST_UNIXTIME=$(date -r "$file" +"%s") || FILE_LAST_UNIXTIME=0
    if [ "${FILE_LAST_UNIXTIME}" -gt "${MAX_PWD_UNIXTIME}" ]; then
        MAX_PWD_UNIXTIME="${FILE_LAST_UNIXTIME}"
        LATEST_FILES="${file}"

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

echo -e -n "${B}now.sh:${R} ${U}${I}${LATEST_FILES}${R} ${W}"
echo -e "----${R} ${DIFF_MSG}:ago"
