#!/usr/bin/env bash
set -e

FILES=$(git ls-files '*.cpp' '*.h' '*.mm')

for file in $FILES; do
    clang-format -i "$file"
done
