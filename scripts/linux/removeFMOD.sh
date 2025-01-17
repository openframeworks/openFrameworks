#!/bin/bash
OFDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OFDIR="$(realpath "$OF_DIR/../..")"

if [ -d "${OFDIR}/libs/fmod" ]; then
    rm -r "${OFDIR}/libs/fmod"
    echo "Deleted ${OFDIR}/libs/fmod"
fi

if [ -d "${OFDIR}/export/linux/libs" ]; then
	if ls ${OFDIR}/export/linux/libs/libfmod* 1> /dev/null 2>&1; then
	    rm ${OFDIR}/export/linux/libs/libfmod*
	    echo "Deleted files in ${OFDIR}/export/linux/libs/"
	fi
fi

if [ -d "${OFDIR}/export/linux64/libs" ]; then
	if ls ${OFDIR}/export/linux64/libs/libfmod* 1> /dev/null 2>&1; then
	    rm ${OFDIR}/export/linux64/libs/libfmod*
	    echo "Deleted files in ${OFDIR}/export/linux64/libs/"
	fi
fi
