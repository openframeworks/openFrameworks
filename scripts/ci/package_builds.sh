#!/bin/bash
set -ev

ROOT=${GITHUB_WORKSPACE}

P_SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Check if ROOT is empty
if [[ -z "$ROOT" ]]; then
  # Set ROOT to ../../../ if it's empty
  ROOT="../../"
fi

ROOT=$(realpath "$ROOT")

echo "##[group]apt update"
if [[ "$(uname -s)" == "Linux" ]]; then
    sudo apt-get -y install aptitude
    #for ubuntu 22.04 we need to install wine32
    #sudo dpkg --add-architecture i386
    sudo apt-get update
    sudo aptitude -y install wine64
fi
echo "##[endgroup]"

echo "##[group]submodule update and pull"
git submodule update --init --recursive
git submodule update --recursive --remote
cd apps/projectGenerator
git pull origin master

echo "##[group]ls"
echo "Where is ROOT: $ROOT"
cd $ROOT
ls
OUTPUT_FOLDER=$ROOT/out
mkdir -p $OUTPUT_FOLDER

RELEASE="${RELEASE:-latest}"

lastversion=$(date +%Y%m%d)
# if [ -n "$1" ] && [ "$1" != "nightly" ]; then
	# lastversion=$1
# fi
echo "##[endgroup]"

echo "##[endgroup]"
cd $OUTPUT_FOLDER
pwd
PACKAGES=(
  "linux64 $lastversion master gcc6"
  "linuxarmv6l $lastversion master"
  "linuxaarch64 $lastversion master"
  "msys2 $lastversion master mingw64"
  "msys2 $lastversion master clang64"
  "msys2 $lastversion master ucrt64"
  "vs $lastversion master"
  "vs $lastversion master 64"
  "android $lastversion master windows"
  "android $lastversion master macos"
  "osx $lastversion master"
  "ios $lastversion master"
)

FAILED_PACKAGES=()

echo "##[group]Create packages"
for pkg in "${PACKAGES[@]}"; do
    echo "Creating package with arguments: $pkg"
    # Use "|| true" to prevent immediate exit on error.
    $ROOT/scripts/dev/create_package.sh $pkg || FAILED_PACKAGES+=("$pkg")
done



echo "# Package Build Summary" > package_summary.md
echo "" >> package_summary.md
echo "| Configuration | Status |" >> package_summary.md
echo "|---------------|--------|" >> package_summary.md

for pkg in "${PACKAGES[@]}"; do
    status="SUCCESS"
    for fail in "${FAILED_PACKAGES[@]}"; do
        if [ "$fail" = "$pkg" ]; then
            status="FAILED"
            break
        fi
    done
    echo "| $pkg | $status |" >> package_summary.md
done

if [ "${GITHUB_ACTIONS}" == "true" ]; then
    echo "## Package Build Summary" >> "$GITHUB_STEP_SUMMARY"
    cat package_summary.md >> "$GITHUB_STEP_SUMMARY"
else
    echo "Package Build Summary:"
    cat package_summary.md
fi

ls -la
cd $ROOT

#export a list of files for the next step - matching .zip or .tar anything and including the relative paths
FILES_OUT=$( (ls -t out/*.zip 2> /dev/null || true) && (ls -t out/*.tar* 2> /dev/null || true) )
#remove new lines
FILES_OUT=$(echo $FILES_OUT | tr '\n' ' ')
if [ -n "$GITHUB_OUTPUT" ]; then
	echo "FILES_OUT=$FILES_OUT" >> $GITHUB_OUTPUT
fi
