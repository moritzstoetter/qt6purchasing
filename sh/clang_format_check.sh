#!/usr/bin/env bash

FILES=$(git ls-files '*.cpp' '*.h' '*.mm')
ISSUES_FILE="/tmp/clang_format.check"

> "${ISSUES_FILE}"

for file in $FILES; do
    DIFF=$(clang-format -style=file "$file" | diff -u "$file" - )
    if [ $? -ne 0 ]; then
        echo "File: $file"
        echo "$DIFF"
        echo
    fi
done >> "${ISSUES_FILE}"

if [ -s "${ISSUES_FILE}" ]; then
    cat "${ISSUES_FILE}"
    exit 1
fi
