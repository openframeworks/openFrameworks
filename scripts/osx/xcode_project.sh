#!/bin/zsh

divider () {
    echo "---------------------------------------------------------------"
}

msg() {
    # echo "\033[32;1;4m2$1\033[0m"
    divider
    echo "💾 $1"
}

# MARK: COPY RESOURCES ----------------------------------------------------
copy_resources() {
    msg "Copying Resources - Icon"

    # echo mkdir -p "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"
    mkdir -p "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"
    # Copy default icon file into App/Resources

    if [[ $ICON_FILE != "" ]] ; then
        # echo rsync -aved --delete --exclude='.DS_Store' "$ICON_FILE" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"
        rsync -aved --delete --exclude='.DS_Store' "$ICON_FILE" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources/"
    fi
    # Copy libfmod and change install directory for fmod to run
    if [ -z "$OF_NO_FMOD" ] ; then
        msg "Copying Resources - Fmod"
        echo rsync -aved --delete --ignore-existing "$OF_PATH/libs/fmod/lib/macos/libfmod.dylib" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Frameworks/";
        rsync -aved --delete --ignore-existing "$OF_PATH/libs/fmod/lib/macos/libfmod.dylib" "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Frameworks/";
    fi
    # Not needed as we now call install_name_tool -id @loader_path/../Frameworks/libfmod.dylib libfmod.dylib on the dylib directly which prevents the need for calling every post build - keeping here for reference and possible legacy usage
    # install_name_tool -change @rpath/libfmod.dylib @executable_path/../Frameworks/libfmod.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
}


# MARK: - BUNDLE DATA FOLDER ------------------------------------------
bundle_data_folder() {
    ARCHIVING=0
    if echo "$TARGET_BUILD_DIR" | grep -q "ArchiveIntermediates"; then
        echo "Archive action detected in TARGET_BUILD_DIR. Skipping copy step."
        if [ -z "$OF_BUNDLE_DATA_FOLDER" ] ; then
            msg 'WARNING: Archive action detected however OF_BUNDLE_DATA_FOLDER is false!'
        fi
        ARCHIVING=1
    fi

    if [ -z "$OF_BUNDLE_DATA_FOLDER" ]; then
        msg 'Bundle data folder disabled \ncan be enabled with OF_BUNDLE_DATA_FOLDER=1 in Project.xcconfig ';
#        if [ "$ARCHIVING" = 0 ]; then
#            if [ -d "${SRCROOT}/bin/data/" ]; then
#                rsync -avz --delete --exclude='.DS_Store' "${SRCROOT}/bin/data/" "${TARGET_BUILD_DIR}/data/"
#            else
#                echo "Source directory ${SRCROOT}/bin/data/ does not exist."
#            fi
#        fi
    else
        # Copy bin/data into App/Resources
        msg 'Bundle data folder enabled - will copy bin/data to App Package'
        if [ -d "${SRCROOT}/bin/data/" ]; then
             rsync -avz --delete --exclude='.DS_Store' "${SRCROOT}/bin/data/" "${TARGET_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/data/"
        else
            echo "Source directory ${SRCROOT}/bin/data/ does not exist."
        fi
    fi
}


# MARK: CODE SIGN  ----------------------------------------------------
code_sign() {
    if [ -z "$OF_CODESIGN" ] ; then
        msg "Codesign Disabled";
    #echo "Note: Not copying bin/data to App Package or doing App Code signing. Use AppStore target for AppStore distribution";
    else
        msg "Codesign Enabled";

        # ---- Code Sign App Package ----
        # WARNING: You may have to run Clean in Xcode after changing CODE_SIGN_IDENTITY!

        # Verify that $CODE_SIGN_IDENTITY is set
        if [ -z "${CODE_SIGN_IDENTITY}" ] ; then
            echo "CODE_SIGN_IDENTITY needs to be set for framework code-signing"
            exit 0
        fi

        if [ -z "${CODE_SIGN_ENTITLEMENTS}" ] ; then
            echo "CODE_SIGN_ENTITLEMENTS needs to be set for framework code-signing!"

            if [ "${CONFIGURATION}" = "Release" ] ; then
                exit 1
            else
                # Code-signing is optional for non-release builds.
                exit 0
            fi
        fi

        ITEMS=""

        FRAMEWORKS_DIR="${TARGET_BUILD_DIR}/${FRAMEWORKS_FOLDER_PATH}"
        echo "$FRAMEWORKS_DIR"
        if [ -d "$FRAMEWORKS_DIR" ] ; then
            FRAMEWORKS=$(find "${FRAMEWORKS_DIR}" -depth -type d -name "*.framework" -or -name "*.xcframework" -or -name "*.dylib" -or -name "*.bundle" | sed -e "s/\(.*framework\)/\1\/Versions\/A\//")
            RESULT=$?
            if [[ $RESULT != 0 ]] ; then
                exit 1
            fi

            ITEMS="${FRAMEWORKS}"
        fi

        LOGINITEMS_DIR="${TARGET_BUILD_DIR}/${CONTENTS_FOLDER_PATH}/Library/LoginItems/"
        if [ -d "$LOGINITEMS_DIR" ] ; then
            LOGINITEMS=$(find "${LOGINITEMS_DIR}" -depth -type d -name "*.app")
            RESULT=$?
            if [[ $RESULT != 0 ]] ; then
                exit 1
            fi

            ITEMS="${ITEMS}"$'\n'"${LOGINITEMS}"
        fi

        # Prefer the expanded name, if available.
        CODE_SIGN_IDENTITY_FOR_ITEMS="${EXPANDED_CODE_SIGN_IDENTITY_NAME}"
        if [ "${CODE_SIGN_IDENTITY_FOR_ITEMS}" = "" ] ; then
            # Fall back to old behavior.
            CODE_SIGN_IDENTITY_FOR_ITEMS="${CODE_SIGN_IDENTITY}"
        fi

        echo "Identity: ${CODE_SIGN_IDENTITY_FOR_ITEMS}"

        echo "Entitlements: ${CODE_SIGN_ENTITLEMENTS}"

        echo "Found: ${ITEMS}"

        # Change the Internal Field Separator (IFS) so that spaces in paths will not cause problems below.
        SAVED_IFS=$IFS
        IFS=$(echo "\n\b")

        # Loop through all items.
        for ITEM in ${ITEMS}
        do
            if lipo -archs "${ITEM}" | grep -q 'i386'; then
                echo "Stripping invalid archs '${ITEM}'"
                lipo -remove i386 "${ITEM}" -o "${ITEM}"
            else
                echo "No need to strip invalid archs '{$ITEM}'"
            fi

            echo "Signing '${ITEM}'"
            codesign --force --verbose --sign "${CODE_SIGN_IDENTITY_FOR_ITEMS}" --entitlements "${CODE_SIGN_ENTITLEMENTS}" "${ITEM}"
            RESULT=$?
            if [[ $RESULT != 0 ]]; then
                echo "Failed to sign '${ITEM}'."
                IFS=$SAVED_IFS
                exit 1
            fi
        done

        # Restore $IFS.
        IFS=$SAVED_IFS

    fi
}



# MARK: Bundle DYLIBs

# make sure shell above is set to /bin/zsh
#
# requires `brew install dylibbundler` (v1.0.5 as of 20230121)
#
# activate via flag in Project.xcconfig
#
# note: dylib subdependencies (dylibs that load dylibs) will be found
#       recursively and bundled within the .app but might need doctoring
#       to work in both bundled or freestanding states.
#
bundle_dylibs() {
    if [ -z "$OF_BUNDLE_DYLIBS" ]; then
        msg "Bundling dylibs disabled \ncan be enabled with OF_BUNDLE_DYLIBS=1 in Project.xcconfig ";
    else
        msg "Bundling dylibs";

            flags=(${(@s: :)OTHER_LDFLAGS})
            declare -A paths
            sargs=""

        # use the keys of an associative array to accumulate unique directories enclosing .dylibs based on OTHER_LDFLAGS
        for ITEM in $flags; do
            if [[ $ITEM == *'.dylib' ]]; then
                d=$(dirname "$ITEM")
                paths[$d]=$d
            fi
        done

        # construct a list of -s args for dylibbuilder to find the dylibs
        for key val in "${(@kv)paths}"; do
            sargs+=-s\ $key\
        done

        # do the thing
        /opt/homebrew/bin/dylibbundler -cd -b -x "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME" -d "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/libs" ${(s[ ])sargs}

    fi
}


copy_binary() {
    msg "Copy Binary to bin/ ";
    APP_NAME="${PRODUCT_NAME}.app"
    APP_SOURCE="${CONFIGURATION_BUILD_DIR}/${APP_NAME}"
    APP_DEST="${SRCROOT}/bin/"
    echo "------------------------------------------"
    echo "🔍 Debugging copy_binary function"

    echo "App Source: ${APP_SOURCE}"
    echo "App Destination: ${APP_DEST}"
    echo "------------------------------------------"
    if [ ! -d "${CONFIGURATION_BUILD_DIR}" ]; then
        echo "ERROR: CONFIGURATION_BUILD_DIR does not exist: ${CONFIGURATION_BUILD_DIR}"
        exit 1
    fi



    # Check if the .app file exists
    if [ ! -d "${APP_SOURCE}" ]; then
        echo "ERROR: Application bundle not found: ${APP_SOURCE}"
        ls -l "${CONFIGURATION_BUILD_DIR}"  # List files in the build directory for debugging
        exit 1
    fi
    mkdir -p "${SRCROOT}/bin/"
    if [ "${ACTION}" = "archive" ]; then
        echo "Skipping copy to bin/ during Archive action."
        return 0
    fi

    if [ -L "${APP_SOURCE}" ] || file "${APP_SOURCE}" | grep -q "alias"; then
        echo "WARNING: App source ${APP_SOURCE} is an alias or symlink. Skipping copy to prevent corruption."
        return 0
    fi


    rsync -avz --delete "$APP_SOURCE" "$APP_DEST"
    if [ $? -eq 0 ]; then
        echo "App successfully $APP_DEST"
    else
        echo "ERROR: Failed to copy the app"
        exit 1
    fi

    if [ -z "${CODE_SIGN_IDENTITY}" ]; then
        echo "No CODE_SIGN_IDENTITY set. Unable to sign the app."
        exit 0
    fi
    echo "Current CODE_SIGN_IDENTITY: '${CODE_SIGN_IDENTITY}'"
    codesign --force --sign "${CODE_SIGN_IDENTITY}" "${APP_DEST}/${APP_NAME}"
}

echo ''
divider
echo "Running $0"

if [ "$ACTION" = "archive" ]; then
    echo "Archive action detected. Skipping copy step."
    exit 0
fi

if [ -n "$ARCHIVE_PATH" ]; then
    echo "Archive path detected: $ARCHIVE_PATH. Skipping copy step."
    exit 0
fi

echo "TARGET Build Dir: ${TARGET_BUILD_DIR}"
echo "Configuration Build Dir: ${CONFIGURATION_BUILD_DIR}"
echo "Product Name: ${PRODUCT_NAME}"

copy_resources
bundle_data_folder
code_sign
bundle_dylibs
#copy_binary

divider

