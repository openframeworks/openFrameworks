OF_ROOT=$PWD

SCRIPT_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/../../dev/downloader.sh"

unset BITS

cd ${OF_ROOT}
./scripts/vs/download_latest_libs.sh -p vs --silent

rm -rf projectGenerator
mkdir -p projectGenerator
cd projectGenerator

echo "Downloading projectGenerator from Github Bleeding"
downloader https://github.com/openframeworks/projectGenerator/releases/download/nightly/projectGenerator-vs.zip 2> /dev/null
unzip projectGenerator-vs.zip 2> /dev/null
rm projectGenerator-vs.zip
ls
cd ../

# downloader https://github.com/openframeworks/projectGenerator/releases/download/nightly/projectGenerator-vs-gui.zip 2> /dev/null
# unzip projectGenerator-vs-gui.zip 2> /dev/null
# rm projectGenerator-vs-gui.zip

PG_OF_PATH=$OF_ROOT/projectGenerator/projectGenerator.exe

PROJECTS=(
    "examples/templates/emptyExample"
    "examples/templates/allAddonsExample"
)

for PROJECT in "${PROJECTS[@]}"; do
    "$PG_OF_PATH" -o"$OF_ROOT" -a"$PROJECT"
done