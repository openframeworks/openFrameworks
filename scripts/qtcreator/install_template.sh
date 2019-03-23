#!/usr/bin/env bash

# default path for Linux
QTCREATOR_WIZARDS=${HOME}/.config/QtProject/qtcreator/templates/wizards/

# if running MSYS2, templates are located in $APPDATA
OS=`uname`
if [ "${OS:0:5}" == "MINGW" -o "${OS:0:7}" == "MSYS_NT" ]; then
	QTCREATOR_WIZARDS=`cygpath ${APPDATA}/QtProject/qtcreator/templates/wizards/`
fi


mkdir -p ${QTCREATOR_WIZARDS}
cp -r templates/wizards/openFrameworks ${QTCREATOR_WIZARDS}
cp -r templates/wizards/openFrameworksUpdate ${QTCREATOR_WIZARDS}

