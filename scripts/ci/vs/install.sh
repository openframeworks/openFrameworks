OF_ROOT=$PWD

SCRIPT_DIR="${BASH_SOURCE%/*}"

RELEASE="${RELEASE:-nightly}"
VS_TARGET=""

if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/../../dev/downloader.sh"

ARCH=""

# Parse flags
while [[ $# -gt 0 ]]; do
  case $1 in
    --vs2026) VS_TARGET="--vs2026"; shift ;;
    -a|--arch) ARCH="$2"; shift 2 ;;
    *) shift ;;
  esac
done

if [[ -n "$ARCH" ]]; then
  ARCH=$(echo "$ARCH" | tr '[:upper:]' '[:lower:]')
  echo "Downloading libs for architecture: $ARCH (target folder: $VS_TARGET)"
  if [[ "$ARCH" == "x64" ]]; then
    ARCH="64"
  fi
fi

unset BITS
cd "${OF_ROOT}"
if [[ -n "$ARCH" ]]; then
  echo "Downloading libs for architecture: $ARCH (target folder: $VS_TARGET)"
  ./scripts/vs/download_libs.sh -p vs -a ${ARCH} --silent -t $RELEASE $VS_TARGET
else
  ./scripts/vs/download_libs.sh -p vs --silent -t $RELEASE $VS_TARGET
fi

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
if [[ "$VS_TARGET" == "vs2026" ]]; then
    for i in "${!PROJECTS[@]}"; do
      PROJECT=${PROJECTS[i]}
      ADDON=${ADDONS[i]}
      OPTIONS="-o\"${OF_ROOT}\" -v -a\"$ADDON\" -p\"vs\" -t\"vs2026\" \"${OF_ROOT}\\${PROJECT}\""
      echo "Updating: ${PROJECT}"
      eval "${PG_OF_PATH} ${OPTIONS}"
    done
else
    for i in "${!PROJECTS[@]}"; do
        PROJECT=${PROJECTS[i]}
        ADDON=${ADDONS[i]}
        OPTIONS="-o\"${OF_ROOT}\" -v -a\"$ADDON\" -p\"vs\"  -t\"\" \"${OF_ROOT}\\${PROJECT}\""
        # Run the project generator executable with the combined options
        echo "Updating: ${PROJECT} with:${PG_OF_PATH}"
        #cmd.exe /c "${PG_OF_PATH} ${OPTIONS}"
        eval "${PG_OF_PATH} ${OPTIONS}"
    done
fi