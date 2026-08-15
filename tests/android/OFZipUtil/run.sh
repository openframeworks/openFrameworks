#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(git rev-parse --show-toplevel)"
TEST_CLASSES="$(mktemp -d)"

javac -d "$TEST_CLASSES" \
  "$REPO_ROOT/tests/android/OFZipUtil/stubs/android/util/Log.java" \
  "$REPO_ROOT/tests/android/OFZipUtil/stubs/androidx/annotation/Keep.java" \
  "$REPO_ROOT/addons/ofxAndroid/Java/cc/openframeworks/OFZipUtil.java" \
  "$REPO_ROOT/tests/android/OFZipUtil/src/OFZipUtilTest.java"

java -cp "$TEST_CLASSES" OFZipUtilTest
