#!/bin/bash
set -e

while read f
do
	echo $f
	clang-format -style=file -i $f
done < <(find -type d \( -path ./deps -o -path ./libobjgen/res \) -prune -o \( -name '*.h' -o -name '*.cpp' \) ! -name 'LookupTable*.h' ! -name 'rbtree.h' -print)

if [[ `git status --porcelain` ]]
then
	git status
	git diff
	exit 1
fi
