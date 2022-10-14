#!/bin/bash

ARGC=$#
APDC=0
ARGS=$@
CMD=""
B="\e[1m"
I="\e[3m"
W="\e[2m"
R="\e[m"
F_R="\e[31m"
F_G="\e[32m"

HEADER="${B}spn${R}"
HADDER="   "

IS_ONSET="true"

PAD_N() {
    START=0
    PAD="$(for ((i = $START; i < $N; i++)); do echo -n " "; done)"
}

X_AUTOMATIC=""
SSH_MODE=""

abort_usage() {
    echo -e "${HEADER}: Usage: ${I}spn${R} [FLAGS...] [CMD...]"
    echo -e "${HADDER}  [FLAG]:="

    echo -e -n "${HADDER}   -a  | --a | --automatic"
    echo -e "\tSmart mode. Resots to remote iff [CMD] query fails on local."

    echo -e -n "${HADDER}   -t  | -l  | --l | --live"
    echo -e "\tssh -t. Enables live-text I/O with the remote SSH."

    echo -e -n "${HADDER}   -tt | --llive"
    echo -e\
    "\t\tssh -t. Enables full ANSI-styled live-text I/O with the remote SSH."
    exit
}

apply_switch() {
    case ${ARGI} in
    -t | -l | --l | --live)
        SSH_MODE="-t"
        ;;
    -tt | --llive)
        SSH_MODE="-tt"
        ;;
    -a | --a | --automatic)
        X_AUTOMATIC="true"
        ;;
    *) ;;
    esac
}

append_cmd() {
    IS_ONSET="false"
    CMD="${CMD} ${ARGI}"
    $((APDC++))
}

#=========================================================
# Separates switchs/target paths
#=========================================================
for ARGI in ${ARGS}; do
    case ${ARGI} in
    \\-*)
        append_cmd
        ;;
    -*)
        if [[ "${IS_ONSET}" == "true" ]]; then
            apply_switch
        else
            append_cmd
        fi
        ;;
    *)
        append_cmd
        ;;
    esac
done

if [[ ${APDC} -le 0 ]]; then
    abort_usage
fi

while [[ $CMD == ' '* ]]; do
    CMD="${CMD## }"
done

ON=""
LOCAL="on:local"
REMOTE="on:remote"

REMOTE_HOST_Q=".*tufts.edu"
REMOTE_HOST="dahn01@homework.cs.tufts.edu"

LOCAL_ROOT="/home/anto5710/cs/"
LOCAL_PWD="$(pwd)"
LOCAL_RELATIVE=""

REMOTE_ROOT="/h/dahn01/"
REMOTE_PWD=""

SSH_FLAGS=""

LOCAL_QUERY_SUCCESS=""
LOCAL_TO_REMOTE_OFFSETER=""
#
# DIR="${DEFAULT_DIR}"

offset_file_local_to_remote() {
    file="${REMOTE_ROOT}${file}"
}

state_on_where() {
    FULL_HOST="$(hostname -f)"

    if [[ "${FULL_HOST}" =~ ${REMOTE_HOST_Q} ]]; then
        ON="${REMOTE}"
    else
        ON="${LOCAL}"
    fi
}

print_offseter() {
    echo -e "${HEADER}: remoting...: ${F_R}\"${LOCAL_PWD_R_ALIGNED%/}/\"${R}"
    echo -e "             ···> ${F_G}\"${REMOTE_PWD_R_ALIGNED%/}/\"${R}"
    echo -e "-------------------------------------------------------------"
}

right_align_paths() {
    LOCAL_PWD_LENGTH="${#LOCAL_PWD}"
    REMOTE_PWD_LENGTH="${#REMOTE_PWD}"

    if [[ ${LOCAL_PWD_LENGTH} -gt ${REMOTE_PWD_LENGTH} ]]; then
        DIFF_PWD_LENGTH="$((LOCAL_PWD_LENGTH - REMOTE_PWD_LENGTH))"
    else
        DIFF_PWD_LENGTH="$((REMOTE_PWD_LENGTH - LOCAL_PWD_LENGTH))"
    fi

    N=${DIFF_PWD_LENGTH}
    PAD_N
    DIFF_PADDER="${PAD}"

    if [[ ${LOCAL_PWD_LENGTH} -gt ${REMOTE_PWD_LENGTH} ]]; then
        LOCAL_PWD_R_ALIGNED="${LOCAL_PWD}"
        REMOTE_PWD_R_ALIGNED="${DIFF_PADDER}${REMOTE_PWD}"
    else
        LOCAL_PWD_R_ALIGNED="${DIFF_PADDER}${LOCAL_PWD}"
        REMOTE_PWD_R_ALIGNED="${REMOTE_PWD}"
    fi
}

locate_remote_pwd() {
    LOCAL_RELATIVE="${LOCAL_PWD#"${LOCAL_ROOT}"}"
    REMOTE_PWD="${REMOTE_ROOT}${LOCAL_RELATIVE}"
}

query_cmd_on_local() {
    CMD_HEAD="${CMD% *}"
    echo -e "${HEADER}: Querying command: ${F_G}${CMD_HEAD}${R} on local...${W}"

    if [[ "${LOCAL_QUERY_SUCCESS}" != "true" ]]; then
        ${CMD_HEAD} --version && LOCAL_QUERY_SUCCESS="true" &>/dev/null
    fi

    if [[ "${LOCAL_QUERY_SUCCESS}" != "true" ]]; then
        ${CMD_HEAD} --help && LOCAL_QUERY_SUCCESS="true" &>/dev/null
    fi

    echo -e -n "${R}"
    if [[ "${LOCAL_QUERY_SUCCESS}" == "true" ]]; then
        echo -e "${HEADER}: ${F_G}Success: running cmd on local${R}"
    else
        echo -e "${HEADER}: ${F_R}Failed: running cmd on remote${R}"
    fi
}

state_on_where

if [[ "${X_LIVE_TEXT}" == true ]]; then
    SSH_FLAGS="-t"
fi

if [[ "${X_AUTOMATIC}" == true ]]; then
    query_cmd_on_local
    if [[ "${LOCAL_QUERY_SUCCESS}" == "true" ]]; then
        ON="${REMOTE}"
    fi
fi

if [[ "${ON}" == "${REMOTE}" ]]; then
    eval ${CMD}
else # on remote:
    locate_remote_pwd
    right_align_paths
    print_offseter

    ssh ${SSH_MODE} "${REMOTE_HOST}" "cd ${REMOTE_PWD}; eval '${CMD}'"
fi
