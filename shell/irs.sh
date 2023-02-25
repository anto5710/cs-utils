#!/bin/bash

HOST="eecs.cs.tufts.edu"

CUR_HOSTNAME="$(hostname)"
HOSTNAME_HINT="${CUR_HOSTNAME}"
HOSTNAME_X='.*[^0-9]([0-9]+).*'

THIS="irs.sh"
BELL="\a"

SERIAL_HOSTNAME=""

usage_abort() {
        echo -e "Usage: ./${THIS} {<UUID|NAME>s...}${BELL}"
        exit 1
}

ARGC=$#
ARGS=$@

if [[ ${ARGC} < 1 ]]; then
        usage_abort
fi

# echo -e "${THIS}: adding SSH private key (Ctrl + C to quit)"
# eval $(ssh-agent -s)
# ssh-add -t 1m

if [[ "${HOSTNAME_HINT}" =~ ${HOSTNAME_X} ]]; then
        echo "${BASH_REMATCH[1]}"
        SERIAL_HOSTNAME="true"
fi

if [[ "${SERIAL_HOSTNAME}"="true" ]]; then


fi



