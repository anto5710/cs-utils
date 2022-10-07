#!/bin/bash
#

# $PKG
CONFIRM="install ok installed"
assert_pkg() {
    dpkg-query -W --showformat='${Status}\n' $PKG

    # echo "\"${PKGQ}\""
    # if [ ! -z "${PKGQ}" ]; then
    #     echo "DOWNLOAD MF!"
    # fi
}

PKG="$@"
assert_pkg
