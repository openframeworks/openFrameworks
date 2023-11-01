OF_ROOT=$PWD

SCRIPT_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/../../dev/downloader.sh"

${OF_ROOT}/scripts/vs/download_libs.sh -p vs --silent

cd ~/
rm -rf projectGenerator
mkdir -p ~/projectGenerator
cd ~/projectGenerator

echo "Downloading projectGenerator from Github Nightly"
downloader https://github.com/openframeworks/projectGenerator/releases/download/nightly/projectGenerator-vs.zip 2> /dev/null
unzip projectGenerator-vs.zip 2> /dev/null
rm projectGenerator-vs.zip

cd $OF_ROOT
PG_OF_PATH=$OF_ROOT ~/projectGenerator/projectGenerator.exe examples/templates/emptyExample
PG_OF_PATH=$OF_ROOT ~/projectGenerator/projectGenerator.exe examples/templates/allAddonsExample
