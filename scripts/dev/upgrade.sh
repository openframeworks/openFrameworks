#!/bin/bash
UP_VERSION=0.0.1

local BASE_DIR=$1

OF_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OF_DIR="$(realpath "$OF_DIR/../../")"
OF_CORE_SCRIPT_DIR="$(realpath "$OF_DIR/scripts")"
OF_CORE_CI_SCRIPT_DIR="$(realpath "$OF_DIR/scripts/ci")"
OF_PG_INSTALLED_DIR="$(realpath "$OF_DIR/projectGenerator")"

read -p "Enter the base directory to search for addon_config.mk files: " BASE_DIR
if [[ ! -d "${OF_DIR}/$BASE_DIR" ]]; then
    echo "Error: Directory $BASE_DIR does not exist."
    exit 1
fi

echo "Warning: This script will modify addon_config.mk files in the specified path."
echo "Please backup your projects before proceeding."
read -p "Do you want to continue? (Y/n): " CONFIRM

if [[ ! "$CONFIRM" =~ ^[Yy]$ ]]; then
    echo "Upgrade cancelled. No changes were made."
    exit 0
fi

echo "Searching for addon_config.mk files in $BASE_DIR..."
ADDON_CONFIG_FILES=$(find "$BASE_DIR" -type f -name "addon_config.mk")
if [[ -z "$ADDON_CONFIG_FILES" ]]; then
    echo "No addon_config.mk files found in the specified directory."
    exit 0
fi
echo "Processing addons_config.mk files..."
for FILE in $ADDON_CONFIG_FILES; do
    echo "Updating [$FILE]..."
    sed -i.bak \
        -e 's/linux64:/linux\/64:/g' \
        -e 's/linuxarmv7l:/linux\/armv7l:/g' \
        -e 's/linuxaarch64:/linux\/aarch64:/g' \
        -e 's/linuxarmv6l:/linux\/armv6l:/g' \
        -e 's|/lib/linuxarmv6l/|/lib/linux/armv6l/|g' \
        -e 's|/lib/linuxarmv7l/|/lib/linux/armv7l/|g' \
        -e 's|/lib/linuxaarch64/|/lib/linux/aarch64/|g' \
        -e 's|/lib/linux64/|/lib/linux/64/|g' \
        "$FILE"

    echo "Backup created: [${FILE}.bak]"
done

echo "Upgrade complete. Backup files have been created with a .bak extension."

echo "Remove *.bak backup files?"
read -p "Do you want to continue? (Y/n): " CONFIRM
if [[ ! "$CONFIRM" =~ ^[Yy]$ ]]; then
    echo "Upgrade cancelled. No changes were made."
    exit 0
fi
for FILE in $ADDON_CONFIG_FILES; do
    rm ${FILE}.bak
    echo "Removed created: ${FILE}.bak"
done


