#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

VERSION=4.0.4

CHECKOUT=$1

install_emscripten_source() {
	if [ -z "${EMSDK+x}" ]; then
		echo "Unix Emscripten SDK not yet found"
		echo "Emscripten Download SRC"
		cd ../../
		git clone https://github.com/emscripten-core/emsdk.git
		cd emsdk
		git pull

		echo "if any issues with python - make sure to add python paths to bash environment Variables:"
	  python -m pip install --upgrade pip setuptools virtualenv
	  ./emsdk install latest
	  ./emsdk activate latest --permanent
	   ./emsdk_env.sh
	  # ./emsdk install ${VERSION}

	else
	  echo "Emscripten SDK found at $EMSDK"
	  source "$EMSDK/emsdk_env"
	fi
}

install_emscripten_package() {
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        echo "Detected macOS. Checking for Emscripten..."
        if ! command_exists emcc; then
            echo "Emscripten not found. Installing via Homebrew..."
            brew install emscripten
        else
            echo "Emscripten is already installed."
        fi
        # Set Emscripten environment variables
        echo "Setting up Emscripten environment..."
        source $(brew --prefix)/share/emscripten/emsdk_env.sh

    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        # Linux
        echo "Detected Linux. Checking for Emscripten..."
        if ! command_exists emcc; then
            echo "Emscripten not found. Installing via apt..."
            sudo apt update
            sudo apt install -y emscripten
        else
            echo "Emscripten is already installed."
        fi

    elif [[ "$OSTYPE" == "cygwin" || "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
        # Windows (assuming Git Bash or similar)
        echo "Detected Windows. Checking for Emscripten..."
        if ! command_exists emcc; then
            echo "Emscripten not found. Installing via winget..."
            winget install -e --id emscripten.emsdk
        else
            echo "Emscripten is already installed."
        fi
    else
        echo "Unsupported operating system: $OSTYPE"
        exit 1
    fi
}

if [ -z "${CHECKOUT}" ]; then
	echo "Installing Emscripten with Package Manager"
	install_emscripten_package
else
    echo "Installing Emscripten from Source"
    install_emscripten_source
fi


