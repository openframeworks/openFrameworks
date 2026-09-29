#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

function usage {
    echo usage:
    echo ./install_dependencies.sh [--help] [--noconfirm]
    echo --help:
    echo display this message
    echo
    echo --noconfirm:
    echo install packages without user confirmation
}

#Analyse script arguments
FORCE_YES=0
while [[ $# > 0 ]] ; do
	arg=$1
	shift
	if [ "$arg" == "--noconfirm" ]; then
		FORCE_YES=1
		continue
	fi
	if [ "$arg" == "--confirm" ]; then
		FORCE_YES=0
		continue
	fi
	if [ "$arg" == "--help" ]; then
		usage
		exit 1
	fi
	if [[ "$arg" == "--msystem="* ]]; then
		msystem="${arg#*=}"
		continue
	fi
	echo Invalid argument : $arg
	usage
	exit 1
done

resolveMsystem(){
	if [[ -z "${msystem:-}" ]]; then
		msystem=$(echo "${MSYSTEM:-ucrt64}" | tr '[:upper:]' '[:lower:]')
	fi
	# MSYS shell has no MinGW prefix (pacman would see -assimp)
	if [[ "$msystem" == "msys" ]]; then
		echo "MSYS shell detected — use Start Menu → MSYS2 UCRT64 to compile. https://www.msys2.org/"
		echo "Installing ucrt64 packages anyway."
		msystem="ucrt64"
		FROM_MSYS_SHELL=1
	fi
	case "$msystem" in
		clang64)    MINGW_PACKAGE_PREFIX="mingw-w64-clang-x86_64" ;;
		clangarm64) MINGW_PACKAGE_PREFIX="mingw-w64-clang-aarch64" ;;
		ucrt64)     MINGW_PACKAGE_PREFIX="mingw-w64-ucrt-x86_64" ;;
		mingw64)    MINGW_PACKAGE_PREFIX="mingw-w64-x86_64" ;;
		mingw32)    MINGW_PACKAGE_PREFIX="mingw-w64-i686" ;;
		*)
			msystem="ucrt64"
			MINGW_PACKAGE_PREFIX="mingw-w64-ucrt-x86_64"
			;;
	esac
	export MINGW_PACKAGE_PREFIX
}

# Git Bash often has no execute bit on .exe; -f is the reliable exists check.
msys2PacmanAt(){
	local root="$1" p
	[[ -n "$root" ]] || return 1
	for p in "$root/usr/bin/pacman.exe" "$root/usr/bin/pacman"; do
		[[ -f "$p" ]] && { printf '%s' "$p"; return 0; }
	done
	return 1
}

findPacman(){
	local p root
	if command -v pacman >/dev/null 2>&1; then
		command -v pacman
		return 0
	fi
	if command -v pacman.exe >/dev/null 2>&1; then
		command -v pacman.exe
		return 0
	fi
	# MSYS2 installed, but this is Git Bash / cmd so pacman is not on PATH
	for root in \
		"${MSYS2_ROOT:-}" \
		"${MSYS2_PATH:-}" \
		/c/msys64 \
		/d/msys64 \
		/e/msys64 \
		/c/msys32 \
		/c/tools/msys64 \
		"/c/Program Files/MSYS2" \
		"/c/Program Files/msys64" \
		"$HOME/msys64" \
		"$HOME/scoop/apps/msys2/current" \
		"${LOCALAPPDATA:-}/Programs/MSYS2" \
		"${LOCALAPPDATA:-}/msys64"
	do
		p=$(msys2PacmanAt "$root") || continue
		printf '%s' "$p"
		return 0
	done
	return 1
}

isWindowsHost(){
	case "$(uname -s 2>/dev/null)" in
		MINGW*|MSYS*|CYGWIN*) return 0 ;;
	esac
	[[ "${OS:-}" == "Windows_NT" ]]
}

resolveMsystem

PACMAN=$(findPacman) || PACMAN=""
if [[ -z "$PACMAN" ]]; then
	echo "pacman is not on PATH in this shell (Git Bash / cmd / PowerShell)."
	echo "MSYS2 may already be installed — open a UCRT64 / MINGW64 / CLANG64 terminal"
	echo "from the Start menu and re-run:"
	echo "  scripts/msys2/install_dependencies.sh"
	echo
	echo "If MSYS2 is not installed yet: https://www.msys2.org/"
	echo
	echo "Visual Studio / Git Bash does not need pacman:"
	echo "  scripts/vs/install_dependencies.sh"
	echo "  scripts/vs/download_libs.sh"
	echo
	if isWindowsHost && [[ -f "$SCRIPT_DIR/../vs/install_dependencies.sh" ]]; then
		echo "Falling back to the Windows VS/Git-Bash helper (optional gum via winget/scoop)."
		VS_ARGS=()
		[[ "$FORCE_YES" == "1" ]] && VS_ARGS+=(-y)
		exec bash "$SCRIPT_DIR/../vs/install_dependencies.sh" "${VS_ARGS[@]}"
	fi
	exit 1
fi
# Off-PATH pacman.exe still needs usr/bin for msys-2.0.dll
PACMAN_DIR=$(dirname "$PACMAN")
case ":$PATH:" in
	*":${PACMAN_DIR}:"*) ;;
	*) export PATH="${PACMAN_DIR}:$PATH" ;;
esac
echo "using pacman: $PACMAN"

# List of MSYS packages to be installed
# MSYS_PACKAGES="make rsync zip unzip wget"
MSYS_PACKAGES="unzip make"

# List of MINGW packages to be installed (without prefix)
# NB: no opencv here — ofxOpenCv uses the Apothecary-built static libraries
# bundled under addons/ofxOpenCv/libs/opencv (see below)
MINGW_PACKAGES="assimp cairo curl freeimage \
	glew glfw glm fmt zlib brotli libpng \
	harfbuzz libsndfile libusb libxml2 mpg123 \
	nlohmann-json openal \
	pkgconf pugixml rtaudio \
	uriparser utf8cpp "
if [[ "$msystem" == "clang64" ]] || [[ "$msystem" == "clangarm64" ]] ; then
    MINGW_PACKAGES="clang $MINGW_PACKAGES"
elif [[ "$msystem" == "ucrt64" ]]; then
    MINGW_PACKAGES="gcc $MINGW_PACKAGES"
elif [[ "$msystem" == "mingw64" ]]; then
    MINGW_PACKAGES="gcc $MINGW_PACKAGES"
fi
# gdb zlib ntldd-git
# boost poco tools
# Build the full list of packages adding prefix to MINGW packages
PACKAGES=${MSYS_PACKAGES}
for pkg in ${MINGW_PACKAGES}; do
	PACKAGES="$PACKAGES ${MINGW_PACKAGE_PREFIX}-${pkg}"
done
if [[ "$PACKAGES" == *" -"* ]] || [[ -z "$MINGW_PACKAGE_PREFIX" ]]; then
	echo "MINGW_PACKAGE_PREFIX is empty — refusing to pass '-package' names to pacman."
	echo "Open an MSYS2 UCRT64 terminal: https://www.msys2.org/"
	exit 1
fi

# pacman has --noconfirm, not --confirm. -S installs; do not -Su (full upgrade).
# -Sy first so we don't request deleted files (stale DB → fmt-10.2.1-1 404s).
SYNC_FLAGS=(-Sy)
[[ "$FORCE_YES" == "1" ]] && SYNC_FLAGS+=(--noconfirm)
echo "Refreshing package databases…"
"$PACMAN" "${SYNC_FLAGS[@]}"
exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error refreshing pacman databases (mirrors may be unreachable)"
	echo "try: pacman -Syu   then re-run this script"
	exit $exit_code
fi

PACMAN_FLAGS=(-S --needed)
[[ "$FORCE_YES" == "1" ]] && PACMAN_FLAGS+=(--noconfirm)

"$PACMAN" "${PACMAN_FLAGS[@]}" ${PACKAGES}


# this would install gstreamer which can be used in mingw too
# "$PACMAN" -Su ${MINGW_PACKAGE_PREFIX}-gst-libav ${MINGW_PACKAGE_PREFIX}-gst-plugins-bad ${MINGW_PACKAGE_PREFIX}-gst-plugins-base ${MINGW_PACKAGE_PREFIX}-gst-plugins-good ${MINGW_PACKAGE_PREFIX}-gst-plugins-ugly ${MINGW_PACKAGE_PREFIX}-gstreamer

exit_code=$?
if [ $exit_code != 0 ]; then
	echo "error installing packages, there could be an error with your internet connection"
	exit $exit_code
fi


# OpenCV is intentionally NOT installed from pacman here.
# ofxOpenCv links the Apothecary-built OpenCV 4 static libraries bundled under
# addons/ofxOpenCv/libs/opencv (see the msys2 section of addon_config.mk),
# so no system opencv (4.x, 5.x, pinned or otherwise) is required.

if [[ "${FROM_MSYS_SHELL:-0}" == "1" ]]; then
	echo "done — open MSYS2 UCRT64 to compile"
fi
