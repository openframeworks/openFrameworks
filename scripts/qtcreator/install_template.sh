#!/usr/bin/env bash

# Set the default path for Linux
QTCREATOR_WIZARDS=${HOME}/.config/QtProject/qtcreator/templates/wizards/

# If running MSYS2, templates are located in $APPDATA
OS=$(uname)
if [[ "${OS:0:5}" == "MINGW" || "${OS:0:7}" == "MSYS_NT" ]]; then
	QTCREATOR_WIZARDS=$(cygpath "${APPDATA}/QtProject/qtcreator/templates/wizards/")
fi

# Ensure the destination directory exists
mkdir -p "${QTCREATOR_WIZARDS}"

# Copy OpenFrameworks templates to the Qt Creator wizards directory
cp -r templates/wizards/openFrameworks "${QTCREATOR_WIZARDS}"
cp -r templates/wizards/openFrameworksUpdate "${QTCREATOR_WIZARDS}"
