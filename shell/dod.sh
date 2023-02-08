#!/bin/bash

S_='\s+'
BLOCKX='(.|\\\n)*'

TOK='[A-Za-z0-9\_\-]'
NAMEX="${TOK}\+"
NAMEX_LNX="(${TOK}|\\\n)+"

echo "alias${S_}(${NAMEX_LNX})${S_}(\\"${BLOCKX}\\"|\\'${BLOCKX}\\')"

alias