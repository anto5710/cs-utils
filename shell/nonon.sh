#!/bin/bash

shopt -s nullglob
FILES=(*)

NO=$@

if [[ ${NO} -lt 0 ]]; then
    NO=$(( ${#FILES[@]} + ${NO} ))
fi

echo ${FILES[${NO}]}
