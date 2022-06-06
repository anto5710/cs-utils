
ARGC=$#

shopt -s extglob

DIFF_FLAGS=( )
DIFF_STARTN=0

HEADER="\e[1mdiffbi:\e[22m"

usage() {
    echo -e "${HEADER} Usage:"
    echo -e "        \e[3mdiffbi\e[23m <MY_EXEC> <REF_EXEC> \e[2m... (ARGS)\e[m"
}

run_diff() {
    # echo -e -n "${HEADER} "
    MY_EXEC="./${MY_EXEC##./}"
    REF_EXEC="./${REF_EXEC##./}"

    MY_STDOUT="${MY_EXEC}.stdout"
    MY_STDERR="${MY_EXEC}.stderr"

    REF_STDOUT="${REF_EXEC}.stdout"
    REF_STDERR="${REF_EXEC}.stderr"

    echo -e "\e[1m${MY_EXEC}\e[22m ${RUN_ARGS}${STDINER}"\
            "> \e[4;2m${MY_STDOUT}\e[m 2> \e[4;2m${MY_STDERR}\e[m"


    echo -e "\e[1m${REF_EXEC}\e[22m ${RUN_ARGS}${STDINER}"\
            "> \e[4;2m${REF_STDOUT}\e[m 2> \e[4;2m${REF_STDERR}\e[m"

    if [ ! -t 0 ]; then
        echo $STDIN | ${MY_EXEC} ${RUN_ARGS} > ${MY_STDOUT} 2> ${MY_STDERR}
    else
        ${MY_EXEC} ${RUN_ARGS} > ${MY_STDOUT} 2> ${MY_STDERR}
    fi
}

for ARG in $@; do
    case ${ARG} in
        # -@(s|S)* ) echo "SILENCE!"       ; (( DIFF_STARTN++ ));;
        -*       ) DIFF_FLAGS+=( ${ARG} ); (( DIFF_STARTN++ ));;
         *       ) break;;
    esac
done

DIFF_ARGC=$(( $ARGC - $DIFF_STARTN ))

if [ ${DIFF_ARGC} -lt 2 ]; then
    usage
    exit 
fi

DIFF_STARTI=$(( DIFF_STARTN + 1 ))
MY_EXEC=${!DIFF_STARTI}

DIFF_NEXTI=$(( DIFF_STARTN + 2 ))
REF_EXEC=${!DIFF_NEXTI}




if [ ! -e "${MY_EXEC}" ] && [ ! -L "${MY_EXEC}" ]; then
    echo -e "${HEADER} executable \e[4m${MY_EXEC}\e[24m is not found"
    set DDD
fi

if [ ! -e "${REF_EXEC}" ] && [ ! -L "${REF_EXEC}" ]; then
    echo -e "${HEADER} executable \e[4m${REF_EXEC}\e[24m is not found"
    set DDD
fi

RUN_ARGS_STARTI=$(( DIFF_STARTN + 3 ))
RUN_ARGS=${@:$RUN_ARGS_STARTI}

if [ ! -t 0 ]; then
    IFS=''
    STDIN="`cat`"
    STDINER=" < \e[2;1mstd::in\e[m"
    STDIN_ECHOER="echo ${STDIN} | "
fi

run_diff

