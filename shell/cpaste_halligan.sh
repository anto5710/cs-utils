#!/bin/bash
#=========================================================
#                    cpaste_halligan
#=========================================================
# Author: Chris Ahn (dahn01), Spring 2023, Tufts Univ.
#
BOLD="\e[1m"
R="\e[m" # reset
THIS="${BOLD}cpaste_halligan${R}"
#=========================================================
USAGE=\
"Usage: ${THIS} <FILE>
       ${THIS} -h"
#=========================================================
HELP=\
"${THIS}: Bash script to reset & overwrite the given <FILE>,
                 with automatic backup.

${USAGE}
Procedure:
 1. If any, back-ups current <FILE>.xyz as <FILE>_old.xyz
                (or to avoid duplicate, as <FILE>_old_#.xyz)

 2. <FILE> is set empty
 3. Opens \`nano\` editor for <FILE>
    (User can just copy & paste the code here)

 4. User can Save & Close by pressing ${BOLD}Ctrl+X${R} and then ${BOLD}Y${R}
"
#=========================================================

help_quit () {
    echo -e "${HELP}"
    exit 0
}

fail_abort () {
    echo -e "${THIS}: failed\n"
}

usage_abort () {
    echo -e "${USAGE}\a"
    exit 1
}

filefail_abort() {
    echo -e "${THIS}: cannot open file: \"${FILE}\" (same named dir exists?)\a"
    exit 1
}

#=========================================================
# Screen command-line args format, either: <FILE> or '-h'
#=========================================================
ARGC=$#
ARG="$@"
FILE=""
FILE_ALREADY_EXITS=""

if [[ $ARGC != 1 ]]; then
    usage_abort
fi

case "${ARG}" in
  -h | --h | --help ) help_quit     ;;
                   *) FILE="${ARG}" ;;
esac

if [[ -f "${FILE}" ]]; then
    FILE_ALREADY_EXITS="true"
fi

if [[ -z "${FILE}" ]] || [[ -d "${FILE}" ]]; then
    filefail_abort
fi

#=========================================================
# Determine name of backup file:
# either <FILE>.xyz -> <FILE>_old.xyz
# or <FILE>_old_NUM.xyz, if duplicate exists
#=========================================================
FILE_WITHOUT_EXT="${FILE%.*}"
FILE_EXT=".${FILE##*.}"

# If <FILE> doesn't have extension, (backup's extension) = ""
if [[ ! "${FILE}" =~ .*\..* ]]; then
    FILE_EXT=""
fi

# If backup name is already taken, append number to evade
SAVE_BACKUP_TO="${FILE_WITHOUT_EXT}_old${FILE_EXT}"
NUM="1"
while [[ -e "${SAVE_BACKUP_TO}" ]]; do
    SAVE_BACKUP_TO="${FILE_WITHOUT_EXT}_old_${NUM}${FILE_EXT}"
    NUM=$(( NUM + 1 ))
done

#=========================================================
# Back up <FILE> to ${SAVE_BACKUP_TO} (or abort on failure)
#=========================================================
cp "${FILE}" "${SAVE_BACKUP_TO}" || fail_abort
echo -e "${THIS}: ${BOLD}saved backup to \"${SAVE_BACKUP_TO}\"${R}"

#=========================================================
# Reset <FILE> empty (or abort on failure)
#=========================================================
echo "" > "${FILE}"              || fail_abort
echo -e "${THIS}: overwriting: \"${FILE}\""

# Open editor
nano "${FILE}"

# Report & verify with `ls -lt`
echo -e "--------------------------------------------------------------"
echo -e " -> now updated: "

ls -lt "${FILE}"

echo -e "--------------------------------------------------------------"

