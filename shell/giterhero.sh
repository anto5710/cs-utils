#!/bin/bash
#==============================================================================
#                                 githero.sh
#==============================================================================
# automization of git commands, for dummies.
#
# Usage:
#         ./githero.sh [OPTIONS] (MSG)
#
# (MSG) is commit message, and defaults to the template:
#         "[YYYY.MO.DD - HH:MI] += {CHANGED_FILES}"
# Options:
#         -h* | --h*    display usage
#         -d* | --d*    lists git diff of the current branch against remote
#         -c* | --c*    commit -all 's with commit message as (MSG)
#         -p* | --p*    commits --all and pushes to upstream with (MSG)
#         -m* | --m* [DEST_BRANCH]
#                       checkouts to given [DEST_BRANCH] (defaults to maind,
#                       merges from current branch, and pushes changes
#==============================================================================
# Examples:
#         ./githero.sh -m master
#                       Overwrite master by merging contents from cur branch
#         ./githero.sh -p
#                       Git add, commit, push everything with default msg.
#==============================================================================
THIS="giterhero"
THIS_FILE="$(
        cd -- "$(dirname "$0")" >/dev/null 2>&1
        pwd -P
)/$(basename "$0")"

# Usage prints what's above without preceding #
usage_exit() {
        sed -n '2,26p' "${THIS_FILE}" | sed 's/^#//g'
}
#==============================================================================
# Stylers
RESET="\e[m"
FG_RESET="\e[22;24m"
FG_WEAK="\e[2m"
FG_RED="\e[31m"
FG_BOLD="\e[1m"
FG_GREEN="\e[32m"
FG_YELLOW="\e[33m"
FG_MAGENTA="\e[35m"
FG_BLUE="\e[34m"

font_weaken() {
        echo -e -n "${FG_WEAK}"
}

font_reset() {
        echo -e -n "${FG_RESET}"
}
#==============================================================================
HEADER="${FG_BOLD}${THIS}:${FG_RESET}"
RETURN_FAIL=1
RETURN_SUCCESS=0

#==============================================================================
# Miscellaneous git abbreviators
git_current_branch() {
        CURRENT_BRANCH="$(git rev-parse --abbrev-ref HEAD)"
}

git_diff_list() {
        GIT_DIFFG_LIST=($(git diff --name-only))
}

#==============================================================================
# Commit helpers
git_default_commit_msg() {
        git_diff_list
        MSG="[$(date +"%Y.%m.%d - %k:%m")] += {${GIT_DIFFG_LIST[@]}}"
}

git_last_commit_msg() {
        LAST_MSG="$(git log -1 --pretty=%B)"
}

git_commit_all() {
        if [[ -z "${MSG}" ]]; then
                git_default_commit_msg
        fi

        font_weaken
        git add --all && git commit -m "${MSG}"
        COMMIT_RES="$?"
        font_reset

        if [[ "${COMMIT_RES}" -eq 0 ]]; then
                echo -e "${HEADER}" \
                        "${FG_GREEN}commiting --all with msg:\n" \
                        "> \"${MSG}\"${RESET}"
        else
                echo -e "${HEADER}${FG_YELLOW} commit interrupted${RESET}"
        fi

        return ${COMMIT_RES}
}

#==============================================================================
git_push() {
        font_weaken #==========================================================
        git_current_branch
        TO_PUSH_LIST="$(git diff ..origin/${CURRENT_BRANCH})"
        git push
        PUSH_RES="$?"
        font_reset #===========================================================

        if [[ "${PUSH_RES}" -eq 0 ]] && [[ ! -z "${TO_PUSH_LIST}" ]]; then
                echo -e "${HEADER}${FG_BLUE} pushed commit: \"${MSG}\"${RESET}"
        else
                echo -e "${HEADER}${FG_YELLOW} push interrupted${RESET}"
        fi

        return ${PUSH_RES}
}

git_commit_and_push() {
        git_commit_all
        git_push
}

#==============================================================================
# Formats DEST_BRANCH, LAST_BRANCH into string:
# FLOW_STYLED == "LAST_BRANCH -> DEST_BRANCH", with LAST_BRANCH faded
git_style_flow() {
        DEST_BRANCH_STYLED="${FG_BOLD}${DEST_BRANCH}${FG_RESET}"
        LAST_BRANCH_STYLED="${FG_WEAK}${LAST_BRANCH}${FG_RESET}"
        FLOW_STYLED="${LAST_BRANCH_STYLED} -> ${DEST_BRANCH_STYLED}${RESET}"
}

#==============================================================================
# git check-outs (switches) into given DEST_BRANCH, if different branch
# Saves previous branch as LAST_BRANCH
git_checkout() {
        git_current_branch
        LAST_BRANCH="${CURRENT_BRANCH}"

        git_style_flow
        if [[ "${CURRENT_BRANCH}" != "${DEST_BRANCH}" ]]; then

                font_weaken #==================================================
                git checkout "${DEST_BRANCH}"
                CHECKOUT_RES="$?"
                font_reset #===================================================

                if [[ "${CHECKOUT_RES}" -eq 0 ]]; then
                        echo -e "${HEADER}${FG_MAGENTA}" \
                                "checked into branch ${FLOW_STYLED}"
                else
                        echo -e "${HEADER}${FG_RED}" \
                                "failed to check into branch ${FLOW_STYLED}"
                fi
        fi
}

#==============================================================================
# Given a different DEST_BRANCH, go to the branch, merge from previous branch,
# push everything, then return back to the previous one.
git_merge_and_return() {
        # save current branh
        git_current_branch
        if [[ "${CURRENT_BRANCH}" == "${DEST_BRANCH}" ]]; then
                echo -e "${HEADER}" \
                        "${FG_RED}cannot merge from same branch${RESET}"
                return ${RETURN_FAIL}
        fi

        # Determine merge msg
        git_last_commit_msg
        MSG="merged <- ${CURRENT_BRANCH}: ${LAST_MSG}"

        # Check out to DEST_BRANCH (by default: main)
        git_checkout || return ${RETURN_FAIL}

        # Preview list of changes to merge (empty means merge will likely idle)
        TO_MERGE_LIST="$(git diff ..${LAST_BRANCH})"
        font_weaken #==========================================================
        git pull
        git merge "${CURRENT_BRANCH}" -m "${MSG}"
        MERGE_RES="$?"
        font_reset #===========================================================

        git_style_flow
        if [[ "${MERGE_RES}" -eq 0 ]] && [[ ! -z "${TO_MERGE_LIST}" ]]; then
                echo -e "${HEADER}${FG_BLUE} merged ${FLOW_STYLED}"
        else
                echo -e "${HEADER}" \
                        "${FG_YELLOW}merge interrupted: ${FLOW_STYLED}"
        fi

        # Push in the dest branch, return back to previous branch
        git_push

        DEST_BRANCH="${LAST_BRANCH}"
        git_checkout || return ${RETURN_FAIL}
}

#==============================================================================
# NR style sub-argument parser
READ_NEXT_ARG() {
        ((I++))
        OPT="${!I}"
        if [[ -z "${OPT}" ]]; then
                OPT="${OPT_DEFAULT}"
        fi
}

CMD=""
#==============================================================================
# main loop. Parse and evaluate each args.
for ((I = 1; I <= "$#"; I++)); do
        arg="${!I}"

        case "${arg}" in
        -m* | --m*)
                OPT_DEFAULT="main"
                READ_NEXT_ARG
                DEST_BRANCH="${OPT}"
                CMD="${arg}"
                ;;
        -*)
                CMD="${arg}"
                ;;
        *)
                MSG="${arg}"
                ;;
        esac
done

case "${CMD}" in
-h | --h)
        usage_exit
        ;;
-d* | --d*)
        git_diff_list
        ;;
-c* | --c*)
        git_commit_all
        ;;
-b* | --b*)
        git_current_branch
        ;;
-p* | --p*)
        git_commit_and_push
        ;;
-m* | --m*)
        git_merge_and_return
        ;;
esac
