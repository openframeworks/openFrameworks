#!/bin/sh
cd "$(dirname "$0")"

checkLib() {
	for lib in "$@"
	do
		if ! command -v $lib 2>&1 >/dev/null
		then
		    echo "<$lib> not found, installing via brew"
		    executa brew install $lib
	    else
			echo "$lib ok"
		fi
	done
}

checkLib jq

plutil -convert json ./emptyExample.xcodeproj/project.pbxproj
cd ./emptyExample.xcodeproj
mv project.pbxproj p.pbxproj
jq --tab . p.pbxproj > project.pbxproj
rm p.pbxproj

echo "OF XCode template conversion to JSON complete"
