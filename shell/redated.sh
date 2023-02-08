#!/bin/bash

CLOCK_HOST="pool.npt.org"
TOLERANCE_UNIX_SEC=100

HOST_STR_TIME=$(wget --method=HEAD -qSO- --max-redirect=0 "${CLOCK_HOST}" 2>&1 \
  | grep "Date:" | cut -d' ' -f4-10)

HOST_UNIX_SEC=$(date -d "$d" +"%s")
SYS_UNIX_SEC=$(date +"%s")

DIFF_UNIX_SEC=$(( SYS_UNIX_SEC - HOST_UNIX_SEC ))

echo $DIFF_UNIX_SEC