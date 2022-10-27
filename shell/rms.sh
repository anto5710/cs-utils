#!/bin/bash
#==============================================================
# Total (maximum) columns. Beep if violative
#==============================================================
ARGC=$#
APDC=0
ARGS=("$@")
FLAGS=()
CMDS=()

CMD=""
B="\e[1m"
I="\e[3m"
W="\e[2m"
R="\e[m"
U="\e[4m"
F_R="\e[31m"
F_G="\e[32m"

HEADER="${B}rms${R}"
HADDER="   "

EXIT_SUCCESS=0
EXIT_FAILURE=1

IS_ONSET="true"
SPN_EXEC_PATH=~/cs/cs-utils/shell/spn.sh

PAD_N() {
    START=0
    PAD="$(for ((i = $START; i < $N; i++)); do echo -n " "; done)"
}

abort_usage() {
    echo -e "${HEADER}: Usage: ${I}rms${R} [-rm FLAGS]... [TARGET]..."
    echo -e "${HADDER}         [-rm FLAGS]\tUse ${I}rm${R} --help for detail"
    echo -e "${HADDER}         [TARGET]   \tTarget(s) to delete co-local-remotely."
    exit ${EXIT_FAILURE}
}

abort_missing_spn_dep() {
    echo -e "${HEADER}: Missing ${U}spn.sh${R}, required for coordinated ssh remotion."
}

append_switch() {
    FLAGS+=(${ARGI})
}

append_cmd() {
    IS_ONSET="false"
    CMDS+=(${ARGI})
}

#=========================================================
# Separates switchs/target paths
#=========================================================
for ARGI in "${ARGS[@]}"; do
    case ${ARGI} in
    \\-*)
        append_cmd
        ;;
    -*)
        if [[ "${IS_ONSET}" == "true" ]]; then
            append_switch
        else
            append_cmd
        fi
        ;;
    *)
        append_cmd
        ;;
    esac
done

CMDC="${#CMDS[@]}"

if [[ ${CMDC} -le 0 ]]; then
    abort_usage
fi

REFERRER=""
if [[ ${CMDC} -le 1 ]]; then
    REFERRER="${B}1 file${R}"
else
    REFERRER="${B}${CMDC} files${R}"
fi

echo -e "${HEADER}: Deleting ${REFERRER} on ${F_G}local${R}  ...${W}"
rm ${FLAGS} ${CMDS}

echo -e "${R}${HEADER}: Deleting ${REFERRER} on ${F_R}remote${R} ..."
bash ${SPN_EXEC_PATH} -tt "rm ${FLAGS} ${CMDS}" || abort_missing_spn_dep
