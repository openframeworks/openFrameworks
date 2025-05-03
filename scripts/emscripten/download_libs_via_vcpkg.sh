#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

export VCPKG_ROOT="$HOME/vcpkg"
echo 'export VCPKG_ROOT=~/vcpkg' >> ~/.bashrc
source ~/.bashrc
mkdir -p ${VCPKG_ROOT}

# Check if vcpkg is installed
# if ! command_exists vcpkg; then
    echo "vcpkg not found. Installing via Homebrew..."
    # brew install vcpkg
    if [ ! -d "${VCPKG_ROOT}/.git" ]; then
	    echo "Cloning vcpkg into ${VCPKG_ROOT}..."
	    git clone https://github.com/microsoft/vcpkg.git "$VCPKG_ROOT"
	    cd "$VCPKG_ROOT"
	    ./bootstrap-vcpkg.sh
	else
	    echo "vcpkg is already cloned at ${VCPKG_ROOT}."
	    cd "$VCPKG_ROOT"
	    git pull
	    ./bootstrap-vcpkg.sh
	fi
# else
#     echo "vcpkg is already installed."
# fi

install_libraries() {
    echo "Installing libraries using vcpkg for wasm32-emscripten target..."
    cd $HOME/vcpkg

    # Set the Emscripten triplet
    # export VCPKG_DEFAULT_TRIPLET=wasm32-emscripten

    vcpkg install zlib --triplet=wasm32-emscripten

    # List of libraries to install
    libraries=(
        "assimp"
        "fmt"
        "freeimage"
        "freetype"
        "glm"
        "nlohmann-json"
        "libpng"
        "libxml2"
        "pugixml"
        "svgtiny"
        "tess2"
        "uriparser"
        "utf8cpp"
        "zlib"
    )

    for library in "${libraries[@]}"; do
        echo "Installing $library..."
        ./vcpkg install $library --triplet=wasm32-emscripten
    done

    echo "All libraries installed successfully."
}

# Main script execution
if ! command_exists vcpkg; then
    install_vcpkg
fi

install_libraries


echo "All tasks completed."
