#!/usr/bin/env bash

OF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OF_DIR="$(realpath "$OF_DIR/../..")"

GCC_VERSION=$(gcc -dumpversion 2>/dev/null || echo "0")
GCC_REQUIRED_VERSION="14"
GCC_URL="https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz"
GCC_FILE="gcc-14.2.0"

if [[ "$GCC_VERSION" =~ ^"$GCC_REQUIRED_VERSION" ]]; then
    echo "GCC $GCC_VERSION is already installed."
    exit 0
fi

ID=""
UBUNTU_VERSION=""
if [[ -f /etc/os-release ]]; then
    . /etc/os-release
    ID="$ID"
    UBUNTU_VERSION="$VERSION_ID"
fi

INSTALL_FROM_SOURCE=0
echoVerbose() { echo "$1"; }
if [ $INSTALL_FROM_SOURCE = 0 ]; then
    case "$ID" in
        "ubuntu")
            if [[ "$(echo "$UBUNTU_VERSION < 24.04" | bc)" -eq 1 ]]; then
                echo "Ubuntu $UBUNTU_VERSION detected. GCC 14 not in default repos. Compiling from source..."
                INSTALL_FROM_SOURCE=1
            else
                echoVerbose "Installing GCC 14 via apt on Ubuntu $UBUNTU_VERSION..."
                sudo apt update
                sudo apt install -y gcc-14 g++-14 && {
                    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100 \
                        --slave /usr/bin/g++ g++ /usr/bin/g++-14
                    sudo update-alternatives --set gcc /usr/bin/gcc-14
                    exit 0
                } || {
                    echo "Apt install failed. Falling back to source..."
                    INSTALL_FROM_SOURCE=1
                }
            fi
            ;;
        "debian")
            echo "Debian detected. Checking for GCC 14 in repos..."
            sudo apt update
            sudo apt install -y gcc-14 g++-14 && {
                sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100 \
                    --slave /usr/bin/g++ g++ /usr/bin/g++-14
                sudo update-alternatives --set gcc /usr/bin/gcc-14
                exit 0
            } || {
                echo "GCC 14 not available in Debian repos. Compiling from source..."
                INSTALL_FROM_SOURCE=1
            }
            ;;
        "centos" | "rhel" | "fedora")
            echoVerbose "Detected $ID. Trying DNF..."
            sudo dnf install -y gcc gcc-c++ && {
                if [[ "$(gcc -dumpversion)" =~ ^"$GCC_REQUIRED_VERSION" ]]; then
                    exit 0
                else
                    echo "DNF didn’t provide GCC 14. Compiling from source..."
                    INSTALL_FROM_SOURCE=1
                fi
            } || INSTALL_FROM_SOURCE=1
            ;;
        "arch")
            echoVerbose "Arch Linux detected. Using pacman..."
            sudo pacman -Syu gcc || {
                echo "GCC 14 not available via pacman yet. Compiling from source..."
                INSTALL_FROM_SOURCE=1
            }
            ;;
        "opensuse" | "suse")
            echoVerbose "openSUSE detected. Trying zypper..."
            sudo zypper install -y gcc14 gcc14-c++ || {
                echo "GCC 14 not available via zypper. Compiling from source..."
                INSTALL_FROM_SOURCE=1
            }
            ;;
        *)
            echo "Unsupported distro: $ID. Compiling GCC 14 from source..."
            INSTALL_FROM_SOURCE=1
            ;;
    esac
fi

if [[ "$INSTALL_FROM_SOURCE" -eq 1 ]]; then
    echo "Compiling GCC 14 from source..."
    if [[ -f /etc/debian_version ]]; then
        sudo apt update
        sudo apt install -y build-essential flex bison libgmp-dev libmpfr-dev libisl-dev libmpc-dev texinfo wget
    elif [[ "$ID" == "centos" || "$ID" == "rhel" || "$ID" == "fedora" ]]; then
        sudo dnf groupinstall -y "Development Tools"
        sudo dnf install -y gmp-devel mpfr-devel libmpc-devel isl-devel flex bison texinfo wget
    elif [[ "$ID" == "arch" ]]; then
        sudo pacman -Syu base-devel gmp mpfr libmpc isl flex bison texinfo wget
    elif [[ "$ID" == "opensuse" || "$ID" == "suse" ]]; then
        sudo zypper install -y patterns-devel-base-devel_basis gmp-devel mpfr-devel libmpc3-devel isl-devel flex bison texinfo wget
    else
        echo "Warning: Unknown distro. Attempting source build without dependency check..."
    fi

    "$OF_DIR/scripts/dev/downloader.sh" "$GCC_URL" "$GCC_FILE.tar.gz" || { echo "Download failed"; exit 1; }
    tar -xzf "$GCC_FILE.tar.gz" || { echo "Extraction failed"; exit 1; }
    cd "$GCC_FILE" || exit 1

    # GCC requires its own prerequisites (e.g., GMP, MPFR) in-tree
    ./contrib/download_prerequisites || { echo "Failed to download prerequisites"; exit 1; }

    # Configure and build
    mkdir -p build && cd build || exit 1
    ../configure --prefix="/usr/local/gcc-14" \
                 --enable-languages=c,c++ \
                 --disable-multilib \
                 --disable-bootstrap \
                 --with-system-zlib || { echo "Configure failed"; exit 1; }
    make -j"$(nproc)" || { echo "Make failed"; exit 1; }
    sudo make install || { echo "Install failed"; exit 1; }

    # Set up paths based on architecture
    ARCH=$(uname -m)
    case "$ARCH" in
        "x86_64")
            GCC_PATH="/usr/local/gcc-14/bin"
            GCC_LIB_PATH="/usr/local/gcc-14/lib64"
            ;;
        "aarch64" | "arm64")
            GCC_PATH="/usr/local/gcc-14/bin"
            GCC_LIB_PATH="/usr/local/gcc-14/lib"
            ;;
        *)
            echo "Unsupported architecture: $ARCH"
            exit 1
            ;;
    esac

    # Symlink binaries
    if [[ -f "$GCC_PATH/gcc" ]]; then
        sudo ln -sf "$GCC_PATH/gcc" /usr/bin/gcc
        sudo ln -sf "$GCC_PATH/g++" /usr/bin/g++
    else
        echo "GCC 14 binary not found after install!"
        exit 1
    fi

    # Update shell profile
    USER_SHELL=$(basename "$SHELL")
    case "$USER_SHELL" in
        "bash") PROFILE_FILE="$HOME/.bashrc" ;;
        "zsh") PROFILE_FILE="$HOME/.zshrc" ;;
        "fish") PROFILE_FILE="$HOME/.config/fish/config.fish" ;;
        *) PROFILE_FILE="$HOME/.profile" ;;  # Fallback
    esac

    if [[ "$USER_SHELL" == "fish" ]]; then
        echo "set -x PATH $GCC_PATH \$PATH" >> "$PROFILE_FILE"
        echo "set -x LD_LIBRARY_PATH $GCC_LIB_PATH \$LD_LIBRARY_PATH" >> "$PROFILE_FILE"
    else
        echo "export PATH=$GCC_PATH:\$PATH" >> "$PROFILE_FILE"
        echo "export LD_LIBRARY_PATH=$GCC_LIB_PATH:\$LD_LIBRARY_PATH" >> "$PROFILE_FILE"
        source "$PROFILE_FILE" 2>/dev/null || echo "Run 'source $PROFILE_FILE' manually."
    fi
fi

# Verify installation
echo "Installed GCC version:"
gcc --version || { echo "GCC not found after install!"; exit 1; }
g++ --version || { echo "G++ not found after install!"; exit 1; }
