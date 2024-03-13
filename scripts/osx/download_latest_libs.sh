#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR
# cd $SCRIPT_DIR
if [ ! -z ${BITS+x} ]; then
    ../dev/download_libs.sh -p osx -b -a $BITS $@
else
    # ../dev/download_libs.sh -p osx -b -a x86_64 $@
    ../dev/download_libs.sh -p osx -b -a arm64 -n $@
fi


# URL_LIST=('https://github.com/openframeworks/apothecary/releases/download/bleeding/openFrameworksLibs_bleeding_osx_1.tar.bz2' 'https://github.com/openframeworks/apothecary/releases/download/bleeding/openFrameworksLibs_bleeding_osx_2.tar.bz2' 'https://github.com/openframeworks/apothecary/releases/download/bleeding/openFrameworksLibs_bleeding_osx_3.tar.bz2')

# echo $URL_LIST
# echo $URL_LIST | xargs -n 1 -P 8 curl -LO -q

# wget -b $URL_LIST[0]
# wget -b $URL_LIST[1]
# wget -b $URL_LIST[2]

# wget2 'https://github.com/openframeworks/apothecary/releases/download/bleeding/openFrameworksLibs_bleeding_osx_1.tar.bz2' 'https://github.com/openframeworks/apothecary/releases/download/bleeding/openFrameworksLibs_bleeding_osx_2.tar.bz2' 'https://github.com/openframeworks/apothecary/releases/download/bleeding/openFrameworksLibs_bleeding_osx_3.tar.bz2'