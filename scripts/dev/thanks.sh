if [ $# -ne 2 ]; then
	echo Error: wrong number of arguments
	echo Usage: ./thanks.sh tag1 tag2
fi
git log $1...$2 --no-merges "$@" | grep ^Author: | sed 's/ <.*//; s/^Author: //' | sort | uniq -c | sed 's/\s*[0-9][0-9]*\(.*\)/-\1/g'
