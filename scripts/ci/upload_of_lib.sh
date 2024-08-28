#!/usr/bin/env bash
OF_ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
if [[ "$GITHUB_ACTIONS" = true ]]; then
    OF_ROOT=$GITHUB_WORKSPACE
fi

CI_ROOT=$OF_ROOT/scripts/ci

DO_UPLOAD="false"

if [[ "${TRAVIS_REPO_SLUG}/${TRAVIS_BRANCH}" = "openframeworks/openFrameworks/master" && "${TRAVIS_PULL_REQUEST}" = "false" ]]; then
    echo "Travis on master branch going to upload libs"
    openssl aes-256-cbc -K $encrypted_418b42d90fab_key -iv $encrypted_418b42d90fab_iv -in $CI_ROOT/id_rsa.enc -out $CI_ROOT/id_rsa -d
    DO_UPLOAD="true";
fi

if [[ "$GITHUB_ACTIONS" = true && "${GITHUB_REF##*/}" == "master" && -z "${GITHUB_HEAD_REF}" ]]; then
    echo "Github Actions on master branch going to upload libs"
    openssl aes-256-cbc -salt -md md5 -a -d -in $CI_ROOT/githubactions-id_rsa.enc -out $CI_ROOT/id_rsa -pass env:GA_CI_SECRET
    mkdir -p ~/.ssh
    DO_UPLOAD="true";
fi

if [ "$DO_UPLOAD" = "true" ]; then
    cp $CI_ROOT/ssh_config ~/.ssh/config
    chmod 600 $CI_ROOT/id_rsa
    if [ "$TARGET" = "android" ]; then
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/project/android/build/intermediates/binaries/debug/arm7/obj/armeabi-v7a/libopenFrameworksLib.a tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/armv7/libopenFrameworksDebug.a_new
        # scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/x86/libopenFrameworksDebug.a tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/x86/libopenFrameworksDebug.a_new
        ssh -i $CI_ROOT/id_rsa tests@ci.openframeworks.cc "mv openFrameworks_libs/$TARGET/armv7/libopenFrameworksDebug.a_new openFrameworks_libs/$TARGET/armv7/libopenFrameworksDebug.a"
        # ssh -i $CI_ROOT/id_rsa tests@ci.openframeworks.cc "mv openFrameworks_libs/$TARGET/x86/libopenFrameworksDebug.a_new openFrameworks_libs/$TARGET/x86/libopenFrameworksDebug.a"
    elif [ "$TARGET" = "emscripten" ]; then
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/libopenFrameworksDebug.bc tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/libopenFrameworksDebug.bc_new
        ssh -i $CI_ROOT/id_rsa tests@ci.openframeworks.cc "mv openFrameworks_libs/$TARGET/libopenFrameworksDebug.bc_new openFrameworks_libs/$TARGET/libopenFrameworksDebug.bc"
    elif [ "$TARGET" = "ios" ]; then
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/libofxiOS_iphonesimulator_Debug.a tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/libofxiOS_iphonesimulator_Debug.a_new
        ssh -i $CI_ROOT/id_rsa tests@ci.openframeworks.cc "mv openFrameworks_libs/$TARGET/libofxiOS_iphonesimulator_Debug.a_new openFrameworks_libs/$TARGET/libofxiOS_iphonesimulator_Debug.a"
    elif [ "$TARGET" = "tvos" ]; then
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/libtvOS+OFLib_Release.a tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/libtvOS+OFLib_Release.a_new
        ssh -i $CI_ROOT/id_rsa tests@ci.openframeworks.cc "mv openFrameworks_libs/$TARGET/libtvOS+OFLib_Release.a_new openFrameworks_libs/$TARGET/libtvOS+OFLib_Release.a"
    elif [ "$TARGET" = "osx" ]; then
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/openFrameworks.a tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/libopenFrameworksDebug.a_new
        ssh -i $CI_ROOT/id_rsa tests@ci.openframeworks.cc "mv openFrameworks_libs/$TARGET/libopenFrameworksDebug.a_new openFrameworks_libs/$TARGET/libopenFrameworksDebug.a"
    else
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/libopenFrameworksDebug.a tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/libopenFrameworksDebug.a_new
        ssh -i $CI_ROOT/id_rsa tests@ci.openframeworks.cc "mv openFrameworks_libs/$TARGET/libopenFrameworksDebug.a_new openFrameworks_libs/$TARGET/libopenFrameworksDebug.a"
    fi
fi
rm -rf $CI_ROOT/id_rsa
