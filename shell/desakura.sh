#!/bin/bash -i
# necessary since we have to pick up commands from user's shell,
# not this new container

INSPECT_TO=3
FINAL_SPARKS="$(history ${INSPECT_TO})"

echo "$FINAL_SPARKS" | tac | while read line
do
        echo "$line"
done