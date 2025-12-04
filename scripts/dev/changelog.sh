if [ $# -ne 2 ]; then
	echo Error: wrong number of arguments
	echo Usage: ./changelog.sh tag1 tag2
fi
git log --ancestry-path $1..$2 --no-merges --pretty=format:'- %s [commit](https://github.com/openframeworks/openFrameworks/commit/%H)' | sed 's/^\s*[0-9][0-9]*\s\(.*\)$/- \1/g'
