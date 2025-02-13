#!/bin/bash -e

if [ "$#" -ne 1 ]; then
	echo "Usage: $0 <file>"; exit 1
fi

if ! command -v "xxd" &> /dev/null; then
	echo "Error: xxd not found"; exit 1
fi

if [ ! -f "$1" ]; then
	echo "Error: File not found"; exit 1
fi

var_name="$(basename -- "$1" | sed 's/[^a-zA-Z0-9_]/_/g')"

echo "#pragma once"
echo "unsigned char ${var_name}[] = {"
xxd -p "$1" | sed 's/\(..\)/0x\1,/g'
echo "}; unsigned int ${var_name}_len = sizeof(${var_name});"
