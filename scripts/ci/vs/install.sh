OF_ROOT=$PWD

SCRIPT_DIR="${BASH_SOURCE%/*}"

RELEASE="${RELEASE:-nightly}"

if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/../../dev/downloader.sh"

unset BITS
cd "${OF_ROOT}"
./scripts/vs/download_libs.sh -p vs --silent -t $RELEASE

rm -rf projectGenerator
mkdir -p projectGenerator
cd projectGenerator

if [[ "$RELEASE" == "nightly" ]] || [[ "$RELEASE" == "latest" ]]; then
  echo "Downloading nightly projectGenerator from Github Latest"
  URL="https://github.com/openframeworks/projectGenerator/releases/download/nightly/projectGenerator-vs.zip"
else
  echo "Downloading projectGenerator for release $RELEASE from Github"
  URL="https://github.com/openframeworks/projectGenerator/releases/download/$RELEASE/projectGenerator-vs.zip"
fi

echo "Downloading projectGenerator from Github Nightly"
downloader $URL 2> /dev/null
unzip projectGenerator-vs.zip 2> /dev/null
rm projectGenerator-vs.zip

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