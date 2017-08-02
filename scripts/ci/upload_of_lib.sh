#!/usr/bin/env bash
OF_ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
CI_ROOT=$OF_ROOT/scripts/ci
if [ "${TRAVIS_REPO_SLUG}/${TRAVIS_BRANCH}" = "openframeworks/openFrameworks/master" ] && [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
    openssl aes-256-cbc -K $encrypted_418b42d90fab_key -iv $encrypted_418b42d90fab_iv -in $CI_ROOT/id_rsa.enc -out $CI_ROOT/id_rsa -d
    cp $CI_ROOT/ssh_config ~/.ssh/config
    chmod 600 $CI_ROOT/id_rsa
    if [ "$TARGET" = "android" ]; then
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/armv7/libopenFrameworksDebug.a tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/armv7/libopenFrameworksDebug.a_new
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/x86/libopenFrameworksDebug.a tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/x86/libopenFrameworksDebug.a_new
        ssh -i $CI_ROOT/id_rsa tests@ci.openframeworks.cc "mv openFrameworks_libs/$TARGET/armv7/libopenFrameworksDebug.a_new openFrameworks_libs/$TARGET/armv7/libopenFrameworksDebug.a"
        ssh -i $CI_ROOT/id_rsa tests@ci.openframeworks.cc "mv openFrameworks_libs/$TARGET/x86/libopenFrameworksDebug.a_new openFrameworks_libs/$TARGET/x86/libopenFrameworksDebug.a"
    elif [ "$TARGET" = "emscripten" ]; then
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/libopenFrameworksDebug.bc tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/libopenFrameworksDebug.bc_new
        ssh -i $CI_ROOT/id_rsa tests@ci.openframeworks.cc "mv openFrameworks_libs/$TARGET/libopenFrameworksDebug.bc_new openFrameworks_libs/$TARGET/libopenFrameworksDebug.bc"
    else
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/libopenFrameworksDebug.a tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/libopenFrameworksDebug.a_new
        ssh -i $CI_ROOT/id_rsa tests@ci.openframeworks.cc "mv openFrameworks_libs/$TARGET/libopenFrameworksDebug.a_new openFrameworks_libs/$TARGET/libopenFrameworksDebug.a"
    fi
fi
rm -rf $CI_ROOT/id_rsa
