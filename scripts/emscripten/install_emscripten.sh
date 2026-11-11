#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR" || exit 1

VERSION=7.0.0

CHECKOUT=$1

install_emscripten_source() {
	if [ -z "${EMSDK:-}" ]; then
		echo "Unix Emscripten SDK not yet found"
		echo "Emscripten Download SRC"
		cd "$SCRIPT_DIR/../.." || return
		git clone https://github.com/emscripten-core/emsdk.git || return
		cd emsdk || return
	else
		echo "Emscripten SDK found at $EMSDK"
		cd "$EMSDK" || return
		git pull --ff-only || return
	fi

	# Select the pinned SDK even when an older installation already exists.
	./emsdk install "$VERSION" || return
	./emsdk activate "$VERSION" --permanent || return
	source ./emsdk_env.sh
}

install_emscripten_package() {
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        echo "Detected macOS. Checking for Emscripten..."
        if ! command -v emcc >/dev/null 2>&1; then
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
        if ! command -v emcc >/dev/null 2>&1; then
            echo "Emscripten not found. Installing via apt..."
            sudo apt update
            sudo apt install -y emscripten
        else
            echo "Emscripten is already installed."
        fi

    elif [[ "$OSTYPE" == "cygwin" || "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
        # Windows (assuming Git Bash or similar)
        echo "Detected Windows. Checking for Emscripten..."
        if ! command -v emcc >/dev/null 2>&1; then
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
