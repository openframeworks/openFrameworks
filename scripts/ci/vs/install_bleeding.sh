OF_ROOT=$PWD

SCRIPT_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/../../dev/downloader.sh"

unset BITS
cd "${OF_ROOT}"
./scripts/vs/download_libs.sh -p vs --silent

rm -rf projectGenerator
mkdir -p projectGenerator
cd projectGenerator

echo "Downloading projectGenerator from Github Bleeding"
downloader https://github.com/openframeworks/projectGenerator/releases/download/nightly/projectGenerator-vs.zip 2> /dev/null
unzip -o projectGenerator-vs.zip 2> /dev/null
#rm projectGenerator-vs.zip

#downloader https://github.com/openframeworks/projectGenerator/releases/download/nightly/projectGenerator-vs-gui.zip 2> /dev/null
#unzip projectGenerator-vs-gui.zip 2> /dev/null
#rm projectGenerator-vs-gui.zip

cd "${OF_ROOT}"

OFW_ROOT=$(cygpath -w "$PWD")
PG_OFX_PATH="${OF_ROOT}\\projectGenerator\\projectGenerator.exe"
PG_OF_PATH="${OF_ROOT}/projectGenerator/projectGenerator.exe"

PGW_OFX_PATH=$(cygpath -w "$PG_OF_PATH")

PROJECTS=(
    "examples\\templates\\emptyExample"
    "examples\\templates\\allAddonsExample"
)

ADDONS=(
    ""
    "ofxAssimpModelLoader,ofxGui,ofxKinect,ofxNetwork,ofxOpenCv,ofxOsc,ofxSvg,ofxThreadedImageLoader,ofxXmlSettings"
)

echo "Updating projects with PG at:${PG_OF_PATH}"

for i in "${!PROJECTS[@]}"; do
    PROJECT=${PROJECTS[i]}
    ADDON=${ADDONS[i]}
    OPTIONS="-o\"${OF_ROOT}\" -v -a\"$ADDON\" -p\"vs\"  -t\"\" \"${OF_ROOT}\\${PROJECT}\""
    # Run the project generator executable with the combined options
    echo "Updating: ${PROJECT} with:${PG_OF_PATH}"
    #cmd.exe /c "${PG_OF_PATH} ${OPTIONS}"
    eval "${PG_OF_PATH} ${OPTIONS}"
done