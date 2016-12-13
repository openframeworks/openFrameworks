#!/usr/bin/env bash
OF_ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
CI_ROOT=$OF_ROOT/scripts/ci
if [ "${TRAVIS_REPO_SLUG}/${TRAVIS_BRANCH}" = "openframeworks/openFrameworks/master" ] && [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
    openssl aes-256-cbc -K $encrypted_cd38768cbb9d_key -iv $encrypted_cd38768cbb9d_iv -in $CI_ROOT/id_rsa.enc -out $CI_ROOT/id_rsa -d
    cp ssh_config ~/.ssh/config
    chmod 600 $CI_ROOT/id_rsa
    if [ "$TARGET"=="android" ]; then
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/armv7/libopenFrameworksDebug.a tests@ci.openframeworks.cc:openFrameworks_libs/armv7/$TARGET
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/x86/libopenFrameworksDebug.a tests@ci.openframeworks.cc:openFrameworks_libs/x86/$TARGET
    elif [ "$TARGET"=="emscripten" ]; then
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/*/libopenFrameworksDebug.bc tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/
    else
        scp -i $CI_ROOT/id_rsa $OF_ROOT/libs/openFrameworksCompiled/lib/$TARGET/*/libopenFrameworksDebug.a tests@ci.openframeworks.cc:openFrameworks_libs/$TARGET/
    fi
fi
rm -rf scripts/id_rsa
