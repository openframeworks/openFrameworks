#!/bin/sh

here="`dirname \"$0\"`"
echo "cd-ing to $here"
cd "$here" || exit 1

./download_libs.sh
