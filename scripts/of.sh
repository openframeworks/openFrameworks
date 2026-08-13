#!/usr/bin/env bash
# of.sh - openFrameworks CLI  |  Dan Rosser 2025
OF_SCRIPT_VERSION=0.4.3

OF_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OF_DIR="$(realpath "$OF_DIR/../")"
OF_CORE_SCRIPT_DIR="$(realpath "$OF_DIR/scripts")"
OF_CORE_CI_SCRIPT_DIR="$(realpath "$OF_DIR/scripts/ci")"
if [[ -d "$OF_DIR/projectGenerator" ]]; then
	OF_PG_INSTALLED_DIR="$(realpath "$OF_DIR/projectGenerator")"
else
	OF_PG_INSTALLED_DIR="$OF_DIR/projectGenerator"
fi

UI_APP_NAME="openFrameworks"
UI_APP_VERSION="$OF_SCRIPT_VERSION"
UI_ANIM="${OF_ANIM:-1}"
. "$OF_CORE_SCRIPT_DIR/ui.sh"
. "$OF_CORE_SCRIPT_DIR/dev/lib_sources.sh"
. "$OF_CORE_SCRIPT_DIR/of_build.sh"

OF_LIB_PLATFORMS=(osx macos ios android linux vs emscripten msys2)
OF_LINUX_DISTRO="${OF_LINUX_DISTRO:-}"
LIB_SOURCE="${LIB_SOURCE:-apothecary}"
LIB_TAG="${LIB_TAG:-latest}"
LIB_LIBS="${LIB_LIBS:-core}"

# Core prebuilt packages (download_libs.sh) + common headers
OF_CORE_LIBS=(
	brotli cairo curl FreeImage fmod fmt freetype glew glfw glm json
	libpng openssl pixman poco rtAudio tess2 uriparser utf8 videoInput zlib
	pugixml kiss
)

# Core addon lib folders → ofx* (scanned under addons/)
# format: libname:addon
OF_CORE_ADDON_MAP=(
	assimp:ofxAssimpModelLoader
	opencv:ofxOpenCv
	ippicv:ofxOpenCv
	libusb:ofxKinect
	libfreenect:ofxKinect
	libxml2:ofxSvg
	svgtiny:ofxSvg
	poco:ofxPoco
	oscpack:ofxOsc
)

# All addons that ship with vanilla openFrameworks — broader than
# OF_CORE_ADDON_MAP (which only covers addons bundling a 3rd-party lib);
# most core addons (ofxGui, ofxNetwork, ofxXmlSettings, …) are pure C++ /
# OS-framework wrappers with nothing to download, so they'd otherwise never
# show up anywhere in `of status`.
OF_CORE_ADDON_NAMES=(
	ofxAccelerometer ofxAndroid ofxAssimp ofxAssimpModelLoader ofxEmscripten
	ofxGPS ofxGui ofxiOS ofxKinect ofxNetwork ofxOpenCv ofxOsc ofxPoco ofxSvg
	ofxThreadedImageLoader ofxUnitTests ofxVectorGraphics ofxXmlSettings
)

isCoreAddonName(){
	local n="$1" x
	for x in "${OF_CORE_ADDON_NAMES[@]}"; do
		[[ "$x" == "$n" ]] && return 0
	done
	return 1
}

APO_HOME=""
APO_ENGINE=""
APO_CLI=""
APO_BUILD_DIR=""
APO_BUILD_TYPES=(osx macos ios tvos xros watchos catos android linux vs msys2 emscripten)

# -----------------------------------------------------------------------------
resolveApothecary(){
	local c
	APO_HOME=""
	APO_ENGINE=""
	APO_CLI=""
	APO_BUILD_DIR=""

	if [[ -f "${OF_CORE_SCRIPT_DIR}/apothecary/apothecary/apothecary" ]]; then
		APO_HOME="${OF_CORE_SCRIPT_DIR}/apothecary"
		APO_ENGINE="${APO_HOME}/apothecary/apothecary"
		APO_BUILD_DIR="${APO_HOME}/build"
		if [[ -f "${APO_HOME}/scripts/apo.sh" ]]; then
			APO_CLI="${APO_HOME}/scripts/apo.sh"
		elif [[ -f "${APO_HOME}/apo" ]]; then
			APO_CLI="${APO_HOME}/apo"
		fi
	fi

	if [[ -z "$APO_CLI" ]]; then
		local -a roots=()
		[[ -n "${APOTHECARY_HOME:-}" ]] && roots+=("$APOTHECARY_HOME")
		roots+=(
			"${OF_DIR}/../OpenSource/apothecary2"
			"${OF_DIR}/../apothecary2"
			"${HOME}/SOURCE/OpenSource/apothecary2"
		)
		for c in "${roots[@]}"; do
			[[ -d "$c" ]] || continue
			if [[ -f "${c}/scripts/apo.sh" ]]; then
				APO_CLI="${c}/scripts/apo.sh"
			elif [[ -f "${c}/apo" ]]; then
				APO_CLI="${c}/apo"
			else
				continue
			fi
			if [[ -z "$APO_ENGINE" && -f "${c}/apothecary/apothecary" ]]; then
				APO_HOME="$c"
				APO_ENGINE="${c}/apothecary/apothecary"
				APO_BUILD_DIR="${c}/build"
			fi
			break
		done
	fi

	if [[ -n "$APO_HOME" ]]; then
		APO_BUILD_DIR="${APO_BUILD_DIR:-${APO_HOME}/build}"
		mkdir -p "$APO_BUILD_DIR" 2>/dev/null || true
	fi
}
resolveApothecary

# Host VS package arch: 64 | arm64 (for download_libs / status display)
detectHostVsArch(){
	local m pa
	pa="${PROCESSOR_ARCHITECTURE:-}"
	[[ -n "${PROCESSOR_ARCHITEW6432:-}" ]] && pa="${PROCESSOR_ARCHITEW6432}"
	case "${MSYSTEM:-}" in
		CLANGARM64|clangarm64) echo "arm64"; return 0 ;;
	esac
	case "$(echo "$pa" | tr '[:lower:]' '[:upper:]')" in
		ARM64) echo "arm64"; return 0 ;;
		AMD64|X86) echo "64"; return 0 ;;
	esac
	m=$(uname -m 2>/dev/null || echo "")
	case "$m" in
		aarch64|arm64|ARM64) echo "arm64" ;;
		*) echo "64" ;;
	esac
}

autoDetectOS(){
	if [[ -z "$PLATFORM" ]]; then
		export OF_OS OF_PLATFORM OF_ARCH
		OF_OS=$(uname -s | tr '[:upper:]' '[:lower:]')
		case "$OF_OS" in
			darwin|dawin) OF_PLATFORM="osx"; OF_ARCH=$(uname -m) ;;
			linux) OF_PLATFORM="linux"; OF_ARCH=$(uname -m) ;;
			mingw*|cygwin*|msys*)
				# Prefer VS toolchain path on Windows shells; arch is CPU not MSYSTEM name
				OF_PLATFORM="vs"
				OF_ARCH=$(detectHostVsArch)
				export OF_MSYSTEM="${MSYSTEM:-}"
				;;
			*) echoError "Unsupported platform: $OF_OS"; exit 1 ;;
		esac
	else
		export OF_OS OF_PLATFORM OF_ARCH
		OF_OS=$(echo "${PLATFORM}" | tr '[:upper:]' '[:lower:]')
		OF_PLATFORM="$PLATFORM"
		OF_ARCH=""
		if [[ "$OF_PLATFORM" == "vs" ]]; then
			OF_ARCH=$(detectHostVsArch)
		fi
	fi
}

detectLinuxDistro(){
	[[ "$OF_PLATFORM" == "linux" ]] || return 0
	[[ -n "$OF_LINUX_DISTRO" ]] && return 0
	local id="" id_like="" like distro=""
	if [[ -r /etc/os-release ]]; then
		. /etc/os-release
		id=$(echo "${ID:-}" | tr '[:upper:]' '[:lower:]')
		id_like=$(echo "${ID_LIKE:-}" | tr '[:upper:]' '[:lower:]')
	elif [[ -r /etc/lsb-release ]]; then
		. /etc/lsb-release
		id=$(echo "${DISTRIB_ID:-}" | tr '[:upper:]' '[:lower:]')
	fi
	case "$id" in
		ubuntu|pop|linuxmint|elementary|zorin|neon|pop-os) distro="ubuntu" ;;
		debian|raspbian|kali|parrot) distro="debian" ;;
		fedora) distro="fedora" ;;
		rhel|centos|rocky|almalinux|ol|scientific) distro="fedora" ;;
		arch|manjaro|endeavouros|garuda|artix|cachyos)
			if [[ "$OF_ARCH" == "armv7l" && -d "${OF_CORE_SCRIPT_DIR}/linux/archlinux_armv7" ]]; then
				distro="archlinux_armv7"
			else
				distro="archlinux"
			fi
			;;
		*)
			for like in $id_like; do
				case "$like" in
					ubuntu) distro="ubuntu"; break ;;
					debian) distro="debian"; break ;;
					fedora|rhel|centos) distro="fedora"; break ;;
					arch) distro="archlinux"; break ;;
				esac
			done
			;;
	esac
	if [[ -n "$distro" && -d "${OF_CORE_SCRIPT_DIR}/linux/${distro}" ]]; then
		OF_LINUX_DISTRO="$distro"
	else
		OF_LINUX_DISTRO=""
	fi
	export OF_LINUX_DISTRO
}

linuxDistroScriptDir(){
	[[ -n "$OF_LINUX_DISTRO" ]] && printf '%s' "${OF_CORE_SCRIPT_DIR}/linux/${OF_LINUX_DISTRO}"
}

listLinuxDistros(){
	local d
	for d in "${OF_CORE_SCRIPT_DIR}/linux"/*/; do
		[[ -d "$d" && -f "${d}install_dependencies.sh" ]] || continue
		basename "$d"
	done
}

pickLinuxDistro(){
	local -a opts=()
	local d
	while IFS= read -r d; do
		[[ -n "$d" ]] && opts+=("${d}|${d}")
	done < <(listLinuxDistros)
	[[ ${#opts[@]} -eq 0 ]] && { echoError "no linux distro install scripts"; return 1; }
	if menuCanRun; then
		menuPick "Select Linux distro install scripts" "${opts[@]}" || return 1
		OF_LINUX_DISTRO="$UI_MENU_RESULT"
	else
		echoError "set OF_LINUX_DISTRO=ubuntu|debian|fedora|archlinux"
		return 1
	fi
	export OF_LINUX_DISTRO
}

runAsRoot(){
	if [[ "$(id -u)" -eq 0 ]]; then
		"$@"
	elif command -v sudo >/dev/null 2>&1; then
		sudo "$@"
	else
		echoError "root or sudo required"
		return 1
	fi
}

runDistroInstallScript(){
	local script="$1"
	shift
	[[ -f "$script" ]] || { echoError "missing $script"; return 1; }
	[[ -x "$script" ]] || chmod +x "$script" 2>/dev/null || true
	if [[ "$(id -u)" -eq 0 ]]; then
		"$script" -y "$@"
	else
		echoNote "sudo $(basename "$script")"
		runAsRoot "$script" -y "$@"
	fi
}

coreScriptPath(){
	case "$OF_PLATFORM" in
		linux)
			case "$OF_ARCH" in
				arm64|jetson|armv7l|armv8l|aarch64)
					export OF_SCRIPT_PATH="${OF_CORE_SCRIPT_DIR}/${OF_PLATFORM}/${OF_ARCH}/"
					;;
				*) export OF_SCRIPT_PATH="${OF_CORE_SCRIPT_DIR}/${OF_PLATFORM}" ;;
			esac
			;;
		macos|osx|vs|emscripten|msys2|android)
			export OF_SCRIPT_PATH="${OF_CORE_SCRIPT_DIR}/${OF_PLATFORM}"
			;;
		*) echoError "Unsupported platform: $OF_PLATFORM"; exit 1 ;;
	esac
}

autoDetectOS
detectLinuxDistro
coreScriptPath
echoVerbose "platform:[${OF_PLATFORM}] arch:[${OF_ARCH}] distro:[${OF_LINUX_DISTRO}]"

# -----------------------------------------------------------------------------
isCoreLibName(){
	local n="$1" x
	for x in "${OF_CORE_LIBS[@]}"; do
		[[ "$x" == "$n" ]] && return 0
	done
	return 1
}

coreAddonForLib(){
	local n="$1" pair
	for pair in "${OF_CORE_ADDON_MAP[@]}"; do
		if [[ "${pair%%:*}" == "$n" ]]; then
			printf '%s' "${pair#*:}"
			return 0
		fi
	done
	return 1
}

isCoreAddonLibName(){
	coreAddonForLib "$1" >/dev/null
}

ensureScript(){
	local script="$1"
	[[ -e "$script" ]] || { echoError "Not found: $script"; return 1; }
	[[ -x "$script" ]] || chmod +x "$script" 2>/dev/null || true
	[[ -x "$script" ]] || { echoError "Not executable: $script"; return 1; }
}

resolvePGScript(){
	local platformDir="$1"
	local script="${OF_CORE_SCRIPT_DIR}/${platformDir}/download_projectGenerator.sh"
	[[ -f "$script" ]] || script="${OF_CORE_SCRIPT_DIR}/osx/download_projectGenerator.sh"
	[[ -f "$script" ]] || script="${OF_CORE_SCRIPT_DIR}/vs/download_projectGenerator.sh"
	printf '%s' "$script"
}

readOfVersion(){
	local constants="${OF_DIR}/libs/openFrameworks/utils/ofConstants.h"
	local major minor patch pre
	[[ -f "$constants" ]] || return 1
	major=$(grep "#define OF_VERSION_MAJOR" "$constants" | awk '{print $3}')
	minor=$(grep "#define OF_VERSION_MINOR" "$constants" | awk '{print $3}')
	patch=$(grep "#define OF_VERSION_PATCH" "$constants" | awk '{print $3}')
	pre=$(grep "#define OF_VERSION_PRE_RELEASE" "$constants" | awk '{print $3}' | tr -d '"')
	if [[ -n "$pre" && "$pre" != "0" ]]; then
		printf '%s.%s.%s-%s' "$major" "$minor" "$patch" "$pre"
	else
		printf '%s.%s.%s' "$major" "$minor" "$patch"
	fi
}

# True CLI projectGenerator only — never the Electron GUI host.
# Windows GUI package layout (download_pg.sh):
#   projectGenerator/projectGenerator.exe          → Electron GUI (opens UI; EPIPE on --version)
#   projectGenerator/projectGeneratorCmd.exe       → CLI copy (preferred)
#   projectGenerator/resources/app/app/projectGenerator.exe → CLI
# macOS:
#   projectGenerator/projectGenerator              → CLI copy from app resources
#   …/Contents/Resources/app/app/projectGenerator  → CLI inside .app
#   …/Contents/MacOS/projectGenerator              → Electron host (skip)
findPGBinary(){
	local root p
	local -a roots=()
	[[ -n "${OF_DIR:-}" ]] && roots+=("${OF_DIR}/projectGenerator")
	[[ -n "${OF_PG_INSTALLED_DIR:-}" && "${OF_PG_INSTALLED_DIR}" != "${OF_DIR}/projectGenerator" ]] && \
		roots+=("${OF_PG_INSTALLED_DIR}")
	# de-dupe while preserving order
	local -a seen=() uniq=()
	for root in "${roots[@]}"; do
		[[ -d "$root" ]] || continue
		local r
		r=$(cd "$root" 2>/dev/null && pwd -P) || r="$root"
		local s
		for s in "${seen[@]+"${seen[@]}"}"; do [[ "$s" == "$r" ]] && continue 2; done
		seen+=("$r")
		uniq+=("$root")
	done

	for root in "${uniq[@]+"${uniq[@]}"}"; do
		for p in \
			"${root}/projectGeneratorCmd.exe" \
			"${root}/projectGeneratorCmd" \
			"${root}/resources/app/app/projectGenerator.exe" \
			"${root}/resources/app/app/projectGenerator" \
			"${root}/projectGenerator.app/Contents/Resources/app/app/projectGenerator" \
			"${root}/projectGenerator"
		do
			# Prefer real files. On Windows, bare "projectGenerator" must NOT
			# resolve via PATHEXT to projectGenerator.exe (Electron host).
			if [[ -f "$p" ]]; then
				# Reject Electron hosts if we accidentally hit them
				case "$p" in
					*/Contents/MacOS/projectGenerator) continue ;;
					*/projectGenerator.exe)
						# root-level .exe is the GUI; only accept under resources/app/app/
						[[ "$p" == */resources/app/app/projectGenerator.exe ]] || continue
						;;
				esac
				printf '%s' "$p"
				return 0
			fi
		done
	done
	return 1
}

# Optional: path to GUI (for messages only — do not exec for status/version)
findPGGui(){
	local root p
	for root in "${OF_DIR}/projectGenerator" "${OF_PG_INSTALLED_DIR}"; do
		[[ -d "$root" ]] || continue
		for p in \
			"${root}/projectGenerator.exe" \
			"${root}/projectGenerator.app" \
			"${root}/projectGenerator.app/Contents/MacOS/projectGenerator"
		do
			[[ -e "$p" ]] && { printf '%s' "$p"; return 0; }
		done
	done
	return 1
}

readPGVersion(){
	local bin ver
	bin=$(findPGBinary) || return 1
	# Bound runtime so a mis-resolved GUI cannot hang status (EPIPE / Electron)
	if command -v timeout >/dev/null 2>&1; then
		ver=$(timeout 5s "$bin" --version 2>/dev/null | head -1 | tr -d '\r')
		[[ -z "$ver" ]] && ver=$(timeout 5s "$bin" -v 2>/dev/null | head -1 | tr -d '\r')
	elif command -v gtimeout >/dev/null 2>&1; then
		ver=$(gtimeout 5s "$bin" --version 2>/dev/null | head -1 | tr -d '\r')
		[[ -z "$ver" ]] && ver=$(gtimeout 5s "$bin" -v 2>/dev/null | head -1 | tr -d '\r')
	else
		ver=$("$bin" --version 2>/dev/null | head -1 | tr -d '\r')
		[[ -z "$ver" ]] && ver=$("$bin" -v 2>/dev/null | head -1 | tr -d '\r')
	fi
	if [[ "$ver" == *projectGenerator* ]]; then
		ver=$(printf '%s' "$ver" | sed -E 's/.*"openFrameworks projectGenerator": *"([^"]+)".*/\1/')
	fi
	# Electron sometimes prints garbage / empty when invoked as CLI
	if [[ -z "$ver" || "$ver" == *"Electron"* || "$ver" == *"chrome-error"* ]]; then
		printf '%s' "installed (CLI: $(basename "$bin"))"
		return 0
	fi
	printf '%s' "${ver:-installed}"
}

readLibsLastUpdate(){
	local newest="" f ts
	local statm
	if [[ "$(uname)" == "Darwin" ]]; then statm='stat -f %m'; else statm='stat -c %Y'; fi

	while IFS= read -r f; do
		[[ -f "$f" ]] || continue
		ts=$($statm "$f" 2>/dev/null) || continue
		[[ -z "$newest" || "$ts" -gt "$newest" ]] && newest=$ts
	done < <(find "${OF_DIR}/libs/download" -type f \( -name "*.tar.bz2" -o -name "*.zip" -o -name "*.tar.gz" \) 2>/dev/null | head -50)

	while IFS= read -r f; do
		[[ -f "$f" ]] || continue
		ts=$($statm "$f" 2>/dev/null) || continue
		[[ -z "$newest" || "$ts" -gt "$newest" ]] && newest=$ts
	done < <(find "${OF_DIR}/libs" "${OF_DIR}/addons" -type f -name "*.pkl" 2>/dev/null | head -120)

	if [[ -z "$newest" && -d "${OF_DIR}/libs" ]]; then
		newest=$($statm "${OF_DIR}/libs" 2>/dev/null)
	fi
	if [[ -n "$newest" ]]; then
		if [[ "$(uname)" == "Darwin" ]]; then
			date -r "$newest" "+%Y-%m-%d %H:%M" 2>/dev/null
		else
			date -d "@${newest}" "+%Y-%m-%d %H:%M" 2>/dev/null
		fi
	else
		printf 'unknown'
	fi
}

# Sets LIB_META_VER LIB_META_TYPE LIB_META_BUILT LIB_META_SRC
readLibVersionMeta(){
	local libDir="$1"
	local pkl pc cv d md f maj min pat
	LIB_META_VER=""; LIB_META_TYPE=""; LIB_META_BUILT=""; LIB_META_SRC=""
	[[ -d "$libDir" ]] || return 1

	pkl=$(find "$libDir" -type f -name "*.pkl" 2>/dev/null | head -1)
	if [[ -n "$pkl" ]]; then
		LIB_META_VER=$(awk -F'=' '/^version[[:space:]]*=/ {gsub(/[" ]/,"",$2); print $2; exit}' "$pkl")
		LIB_META_TYPE=$(awk -F'=' '/^type[[:space:]]*=/ {gsub(/[" ]/,"",$2); print $2; exit}' "$pkl")
		LIB_META_BUILT=$(awk -F'=' '/^buildTime[[:space:]]*=/ {gsub(/^[" ]+|[" ]+$/,"",$2); print $2; exit}' "$pkl")
		if [[ -n "$LIB_META_VER" ]]; then LIB_META_SRC="pkl"; return 0; fi
	fi

	pc=$(find "$libDir" -type f -path '*/pkgconfig/*.pc' 2>/dev/null | head -1)
	[[ -z "$pc" ]] && pc=$(find "$libDir" -type f -name "*.pc" 2>/dev/null | head -1)
	if [[ -n "$pc" ]]; then
		LIB_META_VER=$(grep -iE '^Version:' "$pc" 2>/dev/null | head -1 | sed -E 's/^[Vv]ersion:[[:space:]]*//' | tr -d ' \r')
		if [[ -z "$LIB_META_VER" ]] && command -v pkg-config >/dev/null 2>&1; then
			LIB_META_VER=$(PKG_CONFIG_PATH="$(dirname "$pc")" pkg-config --modversion "$(basename "$pc" .pc)" 2>/dev/null | tr -d ' \r')
		fi
		if [[ -n "$LIB_META_VER" ]]; then
			LIB_META_SRC="pkgconfig"
			LIB_META_TYPE="${LIB_META_TYPE:-oflibs}"
			return 0
		fi
	fi

	cv=$(find "$libDir" -type f \( -name '*-config-version.cmake' -o -name '*ConfigVersion.cmake' \) 2>/dev/null | head -1)
	if [[ -n "$cv" ]]; then
		LIB_META_VER=$(grep -E 'set[[:space:]]*\([[:space:]]*PACKAGE_VERSION' "$cv" 2>/dev/null | head -1 | sed -E 's/.*"([^"]+)".*/\1/')
		[[ -z "$LIB_META_VER" ]] && LIB_META_VER=$(grep -E 'PACKAGE_VERSION[[:space:]]+' "$cv" 2>/dev/null | head -1 | sed -E 's/.*"([^"]+)".*/\1/')
		if [[ -n "$LIB_META_VER" ]]; then
			LIB_META_SRC="cmake"
			LIB_META_TYPE="${LIB_META_TYPE:-oflibs}"
			return 0
		fi
	fi

	d=$(find "$libDir" -type d -path '*/cmake/*' 2>/dev/null | grep -E '/cmake/[^/]+-[0-9]' | head -1)
	if [[ -n "$d" ]]; then
		LIB_META_VER=$(basename "$d" | sed -E 's/.*-([0-9]+(\.[0-9]+)+.*)/\1/')
		if [[ -n "$LIB_META_VER" && "$LIB_META_VER" != "$(basename "$d")" ]]; then
			LIB_META_SRC="cmake-dir"
			LIB_META_TYPE="${LIB_META_TYPE:-oflibs}"
			return 0
		fi
	fi

	for md in "${libDir}Build.md" "${libDir}VERSION" "${libDir}version.txt" "${libDir}README.md"; do
		[[ -f "$md" ]] || continue
		LIB_META_VER=$(grep -iE 'version[[:space:]]*[:=]' "$md" 2>/dev/null | head -1 | sed -E 's/.*[Vv]ersion[[:space:]]*[:=][[:space:]]*//')
		LIB_META_VER=$(printf '%s' "$LIB_META_VER" | tr -d " \`\"'" | head -c 32)
		if [[ -n "$LIB_META_VER" ]]; then
			LIB_META_SRC=$(basename "$md")
			return 0
		fi
	done

	while IFS= read -r f; do
		[[ -f "$f" ]] || continue
		LIB_META_VER=$(grep -E '#define[[:space:]]+[A-Z0-9_]*VERSION_STRING' "$f" 2>/dev/null | head -1 | sed -E 's/.*"([^"]+)".*/\1/')
		[[ -n "$LIB_META_VER" ]] && { LIB_META_SRC="header"; return 0; }
		maj=$(grep -E '#define[[:space:]]+[A-Z0-9_]*VERSION_MAJOR[[:space:]]+' "$f" 2>/dev/null | head -1 | awk '{print $NF}')
		min=$(grep -E '#define[[:space:]]+[A-Z0-9_]*VERSION_MINOR[[:space:]]+' "$f" 2>/dev/null | head -1 | awk '{print $NF}')
		pat=$(grep -E '#define[[:space:]]+[A-Z0-9_]*VERSION_PATCH[[:space:]]+' "$f" 2>/dev/null | head -1 | awk '{print $NF}')
		if [[ -n "$maj" && -n "$min" ]]; then
			LIB_META_VER="${maj}.${min}${pat:+.${pat}}"
			LIB_META_SRC="header"
			return 0
		fi
	done < <(find "${libDir}/include" -type f \( -name '*version*.h' -o -name '*version*.hpp' \) 2>/dev/null | head -5)

	return 1
}

# Streams "name(version · note), name(version · note), …" onto one growing
# line as each library is read, instead of resolving the whole section
# silently before printing anything. Version lookup does several `find`
# passes per lib (pkl/pkgconfig/cmake/header fallbacks) which can be slow on
# Windows/MSYS2 filesystems — this keeps the status command visibly moving
# (each entry appears the moment it's read) instead of appearing to hang.
printLibInlineSection(){
	local title="$1"
	shift
	# nothing to show — skip the whole section instead of a header + "—"
	[[ $# -eq 0 ]] && return 0

	printf '\n'
	printf '  %s%s%s  %s────────────────%s\n' "$C_ACCENT" "$title" "$C_RESET" "$C_MUTED" "$C_RESET"

	local -a items=("$@")
	local path name note entry n=0

	# group entries by their addon note first (ofxEmscripten(html5audio 1.0.0,
	# html5video 1.0.0)) instead of repeating the addon name per lib; entries
	# with no note (plain core libs) print individually same as before
	local -a noteOrder=()
	local seenNotes=" "
	for path in "${items[@]}"; do
		note=""
		[[ "$path" == *"|"* ]] && note="${path#*|}"
		[[ -n "$note" && "$seenNotes" != *" ${note} "* ]] && { noteOrder+=("$note"); seenNotes+="${note} "; }
	done

	printf '  '
	local grp gi
	for grp in "${noteOrder[@]}"; do
		[[ "$n" -gt 0 ]] && printf '%s,%s ' "$C_MUTED" "$C_RESET"
		printf '%s%s%s(' "$C_FG" "$grp" "$C_RESET"
		gi=0
		for path in "${items[@]}"; do
			note=""
			if [[ "$path" == *"|"* ]]; then
				note="${path#*|}"
				path="${path%%|*}"
			fi
			[[ "$note" == "$grp" ]] || continue
			[[ -d "$path" ]] || continue
			name=$(basename "$path")
			if readLibVersionMeta "$path"; then
				entry="${name} ${LIB_META_VER}"
			else
				entry="${name} ?"
			fi
			[[ "$gi" -gt 0 ]] && printf '%s,%s ' "$C_MUTED" "$C_RESET"
			printf '%s%s%s' "$C_FG" "$entry" "$C_RESET"
			gi=$((gi + 1))
		done
		printf '%s)%s' "$C_FG" "$C_RESET"
		n=$((n + 1))
	done

	for path in "${items[@]}"; do
		note=""
		if [[ "$path" == *"|"* ]]; then
			note="${path#*|}"
			path="${path%%|*}"
		fi
		[[ -n "$note" ]] && continue
		[[ -d "$path" ]] || continue
		name=$(basename "$path")
		if readLibVersionMeta "$path"; then
			entry="${name}(${LIB_META_VER})"
		else
			entry="${name}(?)"
		fi
		[[ "$n" -gt 0 ]] && printf '%s,%s ' "$C_MUTED" "$C_RESET"
		printf '%s%s%s' "$C_FG" "$entry" "$C_RESET"
		n=$((n + 1))
	done

	[[ "$n" -eq 0 ]] && printf '%s—%s' "$C_MUTED" "$C_RESET"
	printf '\n'
}

# Every addon name in $names shown as "name(lib version, …)" when it has
# bundled libs among $items (path|note entries, same shape as SEC_CORE /
# SEC_ADDONS), or plain "name" when it doesn't — most core addons (ofxGui,
# ofxNetwork, …) have no bundled 3rd-party lib at all. Args: title, names...,
# "--", items...
printAddonGroupSection(){
	local title="$1"
	shift

	local -a names=() items=()
	local sawSep=0 a
	for a in "$@"; do
		if [[ "$sawSep" -eq 0 && "$a" == "--" ]]; then
			sawSep=1
			continue
		fi
		if [[ "$sawSep" -eq 0 ]]; then
			names+=("$a")
		else
			items+=("$a")
		fi
	done

	# nothing to show — skip the whole section instead of a header + "—"
	[[ ${#names[@]} -eq 0 ]] && return 0

	printf '\n'
	printf '  %s%s%s  %s────────────────%s\n' "$C_ACCENT" "$title" "$C_RESET" "$C_MUTED" "$C_RESET"
	printf '  '
	local nm path note libname n=0 gi
	local -a libEntries
	for nm in "${names[@]}"; do
		[[ "$n" -gt 0 ]] && printf '%s,%s ' "$C_MUTED" "$C_RESET"
		libEntries=()
		for path in "${items[@]}"; do
			note=""
			if [[ "$path" == *"|"* ]]; then
				note="${path#*|}"
				path="${path%%|*}"
			fi
			[[ "$note" == "$nm" ]] || continue
			[[ -d "$path" ]] || continue
			libname=$(basename "$path")
			if readLibVersionMeta "$path"; then
				libEntries+=("${libname} ${LIB_META_VER}")
			else
				libEntries+=("${libname} ?")
			fi
		done
		if [[ ${#libEntries[@]} -gt 0 ]]; then
			printf '%s%s%s(' "$C_FG" "$nm" "$C_RESET"
			for ((gi = 0; gi < ${#libEntries[@]}; gi++)); do
				[[ "$gi" -gt 0 ]] && printf '%s,%s ' "$C_MUTED" "$C_RESET"
				printf '%s%s%s' "$C_FG" "${libEntries[$gi]}" "$C_RESET"
			done
			printf '%s)%s' "$C_FG" "$C_RESET"
		else
			printf '%s%s%s' "$C_FG" "$nm" "$C_RESET"
		fi
		n=$((n + 1))
	done
	printf '\n'
}

# bash-3.2 safe: track seen keys as newline-separated string
_seenHas(){
	[[ $'\n'"${_SEEN_KEYS}"$'\n' == *$'\n'"$1"$'\n'* ]]
}
_seenAdd(){
	_SEEN_KEYS="${_SEEN_KEYS}"$'\n'"$1"
}

# Sets arrays: SEC_CORE SEC_ADDONS SEC_OTHER (paths, optional |note)
collectLibSections(){
	SEC_CORE=()
	SEC_ADDONS=()
	SEC_OTHER=()
	_SEEN_KEYS=""
	local name path addon pair libname key

	# libs/* — single pass classifying core / core-addon / other (was two
	# passes over the same directory; halves the stat cost here, which
	# matters on slower filesystems like Windows/MSYS2)
	if [[ -d "${OF_DIR}/libs" ]]; then
		for path in "${OF_DIR}/libs"/*/; do
			[[ -d "$path" ]] || continue
			name=$(basename "$path")
			case "$name" in
				download|openFrameworks|openFrameworksCompiled|scripts) continue ;;
			esac
			[[ -d "${path}include" || -d "${path}lib" ]] || continue
			if isCoreLibName "$name"; then
				SEC_CORE+=("$path")
			elif isCoreAddonLibName "$name"; then
				addon=$(coreAddonForLib "$name" 2>/dev/null || echo "addon")
				SEC_CORE+=("${path}|${addon}")
			else
				SEC_OTHER+=("$path")
			fi
			_seenAdd "libs/$name"
		done
	fi

	# core addon libs under addons/ofx*/libs/* (core pass)
	if [[ -d "${OF_DIR}/addons" ]]; then
		for pair in "${OF_CORE_ADDON_MAP[@]}"; do
			libname="${pair%%:*}"
			addon="${pair#*:}"
			path="${OF_DIR}/addons/${addon}/libs/${libname}"
			if [[ -d "$path" ]]; then
				key="addons/${addon}/libs/${libname}"
				if ! _seenHas "$key"; then
					SEC_CORE+=("${path}|${addon}")
					_seenAdd "$key"
				fi
			fi
		done
		for addon in ofxOpenCv ofxAssimpModelLoader ofxKinect ofxSvg ofxPoco ofxOsc ofxAssimp; do
			[[ -d "${OF_DIR}/addons/${addon}/libs" ]] || continue
			for path in "${OF_DIR}/addons/${addon}/libs"/*/; do
				[[ -d "$path" ]] || continue
				name=$(basename "$path")
				[[ -d "${path}include" || -d "${path}lib" ]] || continue
				key="addons/${addon}/libs/${name}"
				_seenHas "$key" && continue
				if isCoreAddonLibName "$name" || isCoreLibName "$name"; then
					SEC_CORE+=("${path}|${addon}")
				else
					SEC_ADDONS+=("${path}|${addon}")
				fi
				_seenAdd "$key"
			done
		done
	fi

	# remaining addon libs
	if [[ -d "${OF_DIR}/addons" ]]; then
		for path in "${OF_DIR}/addons"/*/libs/*/; do
			[[ -d "$path" ]] || continue
			addon=$(basename "$(dirname "$(dirname "$path")")")
			name=$(basename "$path")
			key="addons/${addon}/libs/${name}"
			_seenHas "$key" && continue
			[[ -d "${path}include" || -d "${path}lib" ]] || continue
			SEC_ADDONS+=("${path}|${addon}")
			_seenAdd "$key"
		done
	fi
}

cmdStatus(){
	local ofVer pgBin pgVer lastUp hostLine
	local issues=0
	local -a issueNotes=()

	printBanner "status"
	printf '\n'

	tasksBegin "Status" \
		"Host" \
		"openFrameworks" \
		"Project Generator" \
		"Libraries" \
		"Last update"

	taskSet 0 running
	hostLine="${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
	[[ -n "$OF_LINUX_DISTRO" ]] && hostLine+=" · ${OF_LINUX_DISTRO}"
	sleep 0.05
	taskSet 0 done "$hostLine"

	taskSet 1 running
	ofVer=$(readOfVersion) || ofVer=""
	if [[ -n "$ofVer" ]]; then
		taskSet 1 done "v${ofVer}"
	else
		taskSet 1 fail "missing"
		issues=1
		issueNotes+=("openFrameworks version missing")
	fi

	taskSet 2 running
	pgBin=$(findPGBinary) || pgBin=""
	if [[ -n "$pgBin" ]]; then
		pgVer=$(readPGVersion) || pgVer="installed"
		taskSet 2 done "$pgVer"
	else
		taskSet 2 fail "not installed"
		issues=1
		issueNotes+=("Project Generator not installed")
	fi

	taskSet 3 running
	# assessLibsState alone is a handful of stat() calls (essentials only) —
	# fast even on slow filesystems. The full libs/addons enumeration
	# (collectLibSections) is much heavier (every libs/* and addons/*/libs/*
	# dir) and runs later, right before it's printed, so it doesn't sit
	# behind this spinner making status feel like it's hung.
	assessLibsState "$OF_PLATFORM"
	if [[ "$LIBS_STATE" == "missing" ]]; then
		taskSet 3 fail "${LIBS_STATE_DETAIL:-none found}"
		issues=1
		issueNotes+=("libraries missing")
	else
		taskSet 3 done "${LIBS_STATE_DETAIL}"
	fi

	taskSet 4 running
	lastUp=$(readLibsLastUpdate)
	taskSet 4 done "$lastUp"

	tasksSummary

	printf '\n'
	printf '  %s%s%s  %s%s%s\n' "$C_BOLD" "openFrameworks" "$C_RESET" "$C_ACCENT" "v${ofVer:-?}" "$C_RESET"
	printf '  %s────────────────────────────────────────%s\n' "$C_MUTED" "$C_RESET"
	echoKV "platform" "$hostLine"
	if [[ -n "$pgBin" ]]; then
		echoKV "project gen" "$pgVer"
	else
		echoKV "project gen" "not installed"
	fi
	echoKV "last update" "$lastUp"
	echoKV "cli" "$OF_SCRIPT_VERSION"
	local stSha stShaN
	stSha=$(readLibStateField sha_status) || stSha=$(readVerifyField status) || stSha=""
	stShaN=$(readLibStateField sha_verified) || stShaN=$(readVerifyField verified) || stShaN=""
	case "$stSha" in
		verified)
			if [[ -n "$stShaN" && "$stShaN" != "0" ]]; then
				echoKV "integrity" "SHA-256 verified (${stShaN} package(s)) · secure"
			else
				echoKV "integrity" "SHA-256 verified · secure"
			fi
			;;
		failed) echoKV "integrity" "SHA-256 failed" ;;
		no-digest|no-remote) echoKV "integrity" "SHA not available for last download" ;;
		no-tools) echoKV "integrity" "no local sha256 tool" ;;
		skipped) echoKV "integrity" "SHA check skipped" ;;
	esac

	# heavier enumeration deferred to here (see task-3 comment above) — every
	# libs/* and addons/*/libs/* dir gets stat'd, which is the slow part on
	# Windows/MSYS2, so it happens while printing rather than behind a spinner
	collectLibSections

	# SEC_CORE mixes two things: plain core libs under libs/ (no note), and
	# libs bundled specifically for one of OF's own core addons (note = addon
	# name, e.g. assimp for ofxAssimpModelLoader). Split them into their own
	# sections instead of showing them side by side under one "Core" header.
	local -a coreLibsOnly=()
	local p
	for p in "${SEC_CORE[@]}"; do
		[[ "$p" == *"|"* ]] || coreLibsOnly+=("$p")
	done

	# every installed addon, split into core vs other by name (not by whether
	# it happens to bundle a 3rd-party lib — most core addons don't)
	local -a installedAddons=() coreAddonNames=() otherAddonNames=()
	local a
	while IFS= read -r a; do
		[[ -n "$a" ]] && installedAddons+=("$a")
	done < <(listAddons)
	for a in "${installedAddons[@]}"; do
		if isCoreAddonName "$a"; then
			coreAddonNames+=("$a")
		else
			otherAddonNames+=("$a")
		fi
	done

	printLibInlineSection "Core (Libraries)" "${coreLibsOnly[@]}"
	# search both pools regardless of which one collectLibSections happened to
	# put a given addon's libs into (its own addon-name list doesn't
	# necessarily match OF_CORE_ADDON_NAMES, e.g. ofxEmscripten)
	printAddonGroupSection "Core (Addons)" "${coreAddonNames[@]}" -- "${SEC_CORE[@]}" "${SEC_ADDONS[@]}"
	printAddonGroupSection "Other Addons — every other addon under addons/" "${otherAddonNames[@]}" -- "${SEC_CORE[@]}" "${SEC_ADDONS[@]}"
	printLibInlineSection "Other Libraries — libs/ not part of OF's core set" "${SEC_OTHER[@]}"

	printf '\n'
	if [[ "$issues" -eq 0 ]]; then
		echoSuccess "ready"
		printf '\n'
		return 0
	fi

	echoWarning "setup needed"
	local n
	for n in "${issueNotes[@]}"; do
		echoNote "$n"
	done
	printf '\n'

	if menuCanRun; then
		if confirmYes "Would you like to run setup?"; then
			printf '\n'
			cmdSetup "$OF_PLATFORM"
			return $?
		fi
		echoInfo "ok — run of setup when ready"
	else
		echoNote "run: of setup"
	fi
	printf '\n'
	return 1
}

cmdInstalledLibs(){
	cmdStatus
}

# -----------------------------------------------------------------------------
printHelp(){
	local prog
	prog=$(basename "${0:-of}")
	printBanner "cli"
	cat << EOF

  Usage
    ${prog}                        Interactive menu (TTY)
    ${prog} <command> [args]       Run a command directly

  Commands
    menu                      Interactive menu
    status                    System checker + full library list
    setup                     Install deps/libs/PG if missing or outdated
    update    libs | pg | all Prompt source; refresh libs/PG as needed
    build     …               Build core / projects / emscripten / cmake (see of build help)
    cleanup   projects|caches|libs   Free disk (artifacts / downloads / prebuilts)
    version   of  | pg        Version info
    upgrade   addons | apps   Upgrade tree
    test      [group]         Run tests/ smoke tests (build + run), or menu for bash smoke scripts too
    installed                 Alias for status
    apothecary                Build libraries via apothecary submodule

  Env
    LIB_SOURCE=apothecary|oflibs|archive
    LIB_TAG=latest|v12.1.0|v0.11.2
    LIB_LIBS=core|all|"glfw glm"
    LIB_CLEAN_MODE=platform|merge|full   # default platform: only lib/<plat>
    OF_LINUX_DISTRO=ubuntu    Force linux distro scripts
    OF_APO_VS_VER=18          apothecary VS toolchain (18=VS2026, 17=VS2022, 16=VS2019)
    OF_JOBS=8                 Parallel build jobs
    VERBOSE=1  NO_COLOR=1  OF_ANIM=0

  Examples
    ${prog}
    ${prog} status
    ${prog} update libs
    LIB_SOURCE=oflibs LIB_LIBS=core ${prog} update libs
    ${prog} build core Debug
    ${prog} build project apps/myApps/mySketch
    ${prog} build emscripten examples/graphics/graphicsExample
    ${prog} cleanup projects
    ${prog} cleanup caches
    ${prog} cleanup libs other
    ${prog} apothecary build

EOF
	echoKV "platform" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
	echoKV "libs source" "${LIB_SOURCE} / ${LIB_TAG}"
	echoNote "build details: ${prog} build help"
	printf '\n'
}

cmdVersion(){
	local ofVer
	printBanner "version"
	ofVer=$(readOfVersion) || ofVer=""
	printf '\n'
	echoKV "openFrameworks" "${ofVer:-—}"
	echoKV "platform" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
	echoKV "cli" "$OF_SCRIPT_VERSION"
	printf '\n'
}

cmdVersionPG(){
	local bin ver
	printBanner "version"
	bin=$(findPGBinary) || bin=""
	printf '\n'
	if [[ -n "$bin" ]]; then
		ver=$(readPGVersion)
		echoKV "projectGenerator" "$ver"
		echoKV "path" "$bin"
	else
		echoKV "projectGenerator" "not installed"
		echoNote "try: of update pg"
	fi
	printf '\n'
}

# Combined openFrameworks + Project Generator version screen (menu entry point)
menuVersion(){
	local ofVer bin ver
	printBanner "version"
	ofVer=$(readOfVersion) || ofVer=""
	bin=$(findPGBinary) || bin=""
	printf '\n'
	echoKV "openFrameworks" "${ofVer:-—}"
	echoKV "platform" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
	echoKV "cli" "$OF_SCRIPT_VERSION"
	printf '\n'
	if [[ -n "$bin" ]]; then
		ver=$(readPGVersion)
		echoKV "projectGenerator" "$ver"
		echoKV "pg path" "$bin"
	else
		echoKV "projectGenerator" "not installed"
		echoNote "try: of update pg"
	fi
	printf '\n'
}

# test() — runs either the tests/*/* smoke test projects (build + run,
# reusing cmdTest from of_build.sh) or one of the standalone smoke_test_*.sh
# bash scripts under scripts/dev/ (e.g. smoke_test_nightly.sh).
menuTest(){
	local choice
	if ! menuCanRun; then
		echoWarning "no TTY — running all tests/ smoke tests non-interactively"
		cmdTest
		return $?
	fi
	printBanner "test"

	local -a opts=("All tests/ smoke tests (build + run)|tests-all")
	local g gName
	if [[ -d "${OF_DIR}/tests" ]]; then
		for g in "${OF_DIR}/tests"/*/; do
			[[ -d "$g" ]] || continue
			gName=$(basename "${g%/}")
			opts+=("tests/${gName} only|tests-${gName}")
		done
	fi
	local -a smokeScripts=()
	local s
	for s in "${OF_CORE_SCRIPT_DIR}/dev"/smoke_test_*.sh; do
		[[ -f "$s" ]] && smokeScripts+=("$(basename "$s")")
	done
	for s in "${smokeScripts[@]}"; do
		opts+=("Smoke script — ${s}|script:${s}")
	done
	opts+=("Back|back")

	menuPick "Run which tests?" "${opts[@]}" || return 2
	choice="$UI_MENU_RESULT"
	case "$choice" in
		tests-all) cmdTest ;;
		tests-*)   cmdTest "${choice#tests-}" ;;
		script:*)
			local scriptName="${choice#script:}"
			local scriptPath="${OF_CORE_SCRIPT_DIR}/dev/${scriptName}"
			local -a args=()
			if menuCanRun && confirmYes "Include the real network/build checks (--real)?"; then
				args+=(--real)
			fi
			ensureScript "$scriptPath" 2>/dev/null
			bash "$scriptPath" "${args[@]}"
			;;
		back) return 2 ;;
	esac
}

OF_LIB_STATE_FILE="${OF_DIR}/libs/.of-cli-state"

# Essentials used to decide "installed"
OF_LIBS_ESSENTIAL=(freetype glew glfw zlib tess2 uriparser utf8 json)

# Merge last SHA verify result from download_libs (libs/download/.last-verify)
readVerifyField(){
	local key="$1"
	local f="${OF_DIR}/libs/download/.last-verify"
	[[ -f "$f" ]] || return 1
	grep -E "^${key}=" "$f" 2>/dev/null | head -1 | cut -d= -f2-
}

writeLibState(){
	local source="${1:-$LIB_SOURCE}"
	local tag="${2:-$LIB_TAG}"
	local platformDir="${3:-$OF_PLATFORM}"
	local shaStatus shaVerified shaUpdated
	shaStatus=$(readVerifyField status) || shaStatus=""
	shaVerified=$(readVerifyField verified) || shaVerified=""
	shaUpdated=$(readVerifyField updated) || shaUpdated=""
	mkdir -p "${OF_DIR}/libs"
	cat > "$OF_LIB_STATE_FILE" << EOF
source=${source}
tag=${tag}
platform=${platformDir}
arch=${OF_ARCH:-}
updated=$(date -u +%Y-%m-%dT%H:%M:%SZ)
of_cli=${OF_SCRIPT_VERSION}
sha_status=${shaStatus}
sha_verified=${shaVerified}
sha_updated=${shaUpdated}
EOF
}

readLibStateField(){
	local key="$1"
	[[ -f "$OF_LIB_STATE_FILE" ]] || return 1
	grep -E "^${key}=" "$OF_LIB_STATE_FILE" 2>/dev/null | head -1 | cut -d= -f2-
}

# Sets LIBS_STATE=ok|missing  LIBS_STATE_DETAIL=
# If libs are detected on disk → ok (no .of-cli-state means assume latest).
# Only missing essentials count as a problem.
assessLibsState(){
	local name missing=0 present=0
	local stSource stTag stUpdated

	LIBS_STATE="ok"
	LIBS_STATE_DETAIL=""

	for name in "${OF_LIBS_ESSENTIAL[@]}"; do
		if [[ -d "${OF_DIR}/libs/${name}/include" || -d "${OF_DIR}/libs/${name}/lib" ]]; then
			present=$((present + 1))
		else
			missing=$((missing + 1))
		fi
	done

	if [[ $present -eq 0 ]]; then
		LIBS_STATE="missing"
		LIBS_STATE_DETAIL="no core libraries found under libs/"
		return 0
	fi
	if [[ $missing -gt 0 ]]; then
		# still treat as present if most essentials exist
		if [[ $present -ge 4 ]]; then
			LIBS_STATE="ok"
			LIBS_STATE_DETAIL="detected (${present} essentials)"
		else
			LIBS_STATE="missing"
			LIBS_STATE_DETAIL="${missing}/${#OF_LIBS_ESSENTIAL[@]} essential libs missing"
			return 0
		fi
	fi

	# optional marker for bookkeeping only — absence means assume latest
	stSource=$(readLibStateField source) || stSource="apothecary"
	stTag=$(readLibStateField tag) || stTag="latest"
	stUpdated=$(readLibStateField updated) || stUpdated=""
	local stSha stShaN
	stSha=$(readLibStateField sha_status) || stSha=$(readVerifyField status) || stSha=""
	stShaN=$(readLibStateField sha_verified) || stShaN=$(readVerifyField verified) || stShaN=""
	LIBS_STATE="ok"
	if [[ -n "$stUpdated" ]]; then
		LIBS_STATE_DETAIL="${stSource}@${stTag} · ${stUpdated}"
	else
		LIBS_STATE_DETAIL="detected · assume latest"
	fi
	case "$stSha" in
		verified)
			if [[ -n "$stShaN" && "$stShaN" != "0" ]]; then
				LIBS_STATE_DETAIL+=" · SHA-256 secure (${stShaN} pkg)"
			else
				LIBS_STATE_DETAIL+=" · SHA-256 secure"
			fi
			;;
		failed) LIBS_STATE_DETAIL+=" · SHA failed" ;;
		no-digest|no-remote|no-tools|skipped|unchecked|"") ;;
		*) LIBS_STATE_DETAIL+=" · SHA ${stSha}" ;;
	esac
}

# Sets PG_STATE=ok|missing  PG_STATE_DETAIL=
assessPGState(){
	local bin ver gui
	PG_STATE="ok"
	PG_STATE_DETAIL=""
	bin=$(findPGBinary) || bin=""
	if [[ -z "$bin" ]]; then
		PG_STATE="missing"
		gui=$(findPGGui) || gui=""
		if [[ -n "$gui" ]]; then
			# Windows/mac GUI package present but CLI path missing (old layout / incomplete extract)
			PG_STATE_DETAIL="GUI found, CLI missing — re-run: of update pg  (need projectGeneratorCmd.exe)"
		else
			PG_STATE_DETAIL="Project Generator not installed"
		fi
		return 0
	fi
	ver=$(readPGVersion) || ver="installed"
	PG_STATE="ok"
	PG_STATE_DETAIL="$ver"
}

# Download libs using current LIB_SOURCE / LIB_TAG (no banner)
runLibsDownload(){
	local platformDir="${1:-$OF_PLATFORM}"
	local source="${LIB_SOURCE:-apothecary}"
	local tag="${LIB_TAG:-latest}"
	local libsSel="${LIB_LIBS:-core}"
	local -a oflibsArgs=()

	case "$source" in
		apothecary|"")
			export LIB_TAG="$tag"
			downloadApothecaryLibs "$platformDir" || return 1
			;;
		oflibs)
			export LIB_TAG="$tag"
			if [[ "$tag" == "latest" ]]; then
				tag=$(listOfLibsReleases | head -1)
				[[ -z "$tag" ]] && tag="v1.0"
				LIB_TAG="$tag"
			fi
			libsSel="${libsSel//,/ }"
			# shellcheck disable=SC2206
			oflibsArgs=( $libsSel )
			downloadOfLibs "$OF_DIR" "$tag" "$platformDir" "${oflibsArgs[@]}" || return 1
			;;
		archive)
			[[ -n "$tag" && "$tag" != "latest" ]] || {
				echoError "archive needs LIB_TAG=v0.x.y"
				return 1
			}
			downloadArchiveRelease "$OF_DIR" "$tag" "$platformDir" "$OF_ARCH" libs || return 1
			;;
		*)
			echoError "unknown LIB_SOURCE: $source"
			return 1
			;;
	esac
	writeLibState "$source" "$LIB_TAG" "$platformDir"
}

runPGDownload(){
	local platformDir="${1:-$OF_PLATFORM}"
	local script
	script=$(resolvePGScript "$platformDir")
	ensureScript "$script" || return 1
	"$script"
}

cmdUpdateLibs(){
	local platformDir="${1:-$OF_PLATFORM}"
	local source="${LIB_SOURCE:-apothecary}"
	local tag="${LIB_TAG:-latest}"
	local force="${2:-0}"

	printBanner "update"
	echoInfo "download libraries · ${source}"
	echoKV "platform" "${platformDir}${OF_ARCH:+ / ${OF_ARCH}}"
	echoKV "source" "$source"
	echoKV "tag" "$tag"
	printf '\n'

	assessLibsState "$platformDir" "$source" "$tag"
	echoKV "libs state" "${LIBS_STATE} — ${LIBS_STATE_DETAIL}"

	if [[ "$LIBS_STATE" == "ok" && "$force" != "1" ]]; then
		if menuCanRun; then
			confirmNo "Libraries look current. Redownload anyway?" || {
				echoInfo "skipped — libs already ${LIBS_STATE_DETAIL}"
				return 0
			}
		else
			echoInfo "libs ok — skip redownload (set force or use menu)"
			return 0
		fi
	fi

	tasksBegin "Tasks" \
		"Detect platform" \
		"Resolve source (${source})" \
		"Download / install" \
		"Finish"

	taskSet 0 done "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}} → ${platformDir}"
	taskSet 1 done "${source} @ ${tag}"

	if ! taskLive 2 -- runLibsDownload "$platformDir"; then
		taskTickLine 3 skip
		tasksSummary
		return 1
	fi
	taskTickLine 3 done
	tasksSummary
	echoSuccess "libraries ready · ${source}@${LIB_TAG}"
	printf '\n'
}

cmdUpdatePG(){
	local platformDir="${1:-$OF_PLATFORM}"
	local force="${2:-0}"

	printBanner "update"
	echoInfo "Project Generator"
	assessPGState
	echoKV "pg state" "${PG_STATE} — ${PG_STATE_DETAIL}"

	if [[ "$PG_STATE" == "ok" && "$force" != "1" ]]; then
		if menuCanRun; then
			confirmNo "PG installed (${PG_STATE_DETAIL}). Download latest anyway?" || {
				echoInfo "skipped PG — ${PG_STATE_DETAIL}"
				return 0
			}
		else
			echoInfo "PG ok — skip (already ${PG_STATE_DETAIL})"
			return 0
		fi
	fi

	tasksBegin "Tasks" "Resolve script" "Download PG" "Finish"
	local script
	script=$(resolvePGScript "$platformDir")
	if ! ensureScript "$script"; then
		taskSet 0 fail
		tasksSkipRest
		tasksSummary
		return 1
	fi
	taskSet 0 done "$(basename "$script")"
	if ! taskLive 1 -- runPGDownload "$platformDir"; then
		taskTickLine 2 skip
		tasksSummary
		return 1
	fi
	taskTickLine 2 done
	tasksSummary
	assessPGState
	echoSuccess "PG ready · ${PG_STATE_DETAIL}"
	printf '\n'
}

cmdUpdate(){
	local subcmd="$1"
	local platformDir="${2:-$OF_PLATFORM}"
	case "$subcmd" in
		""|libs)
			# interactive: always prompt source; then state-aware download
			if menuCanRun; then
				menuDownloadLibs "$platformDir"
			else
				cmdUpdateLibs "$platformDir"
			fi
			return $?
			;;
		pg|projectgenerator)
			cmdUpdatePG "$platformDir"
			return $?
			;;
		all)
			if menuCanRun; then
				menuDownloadLibs "$platformDir" || return $?
			else
				cmdUpdateLibs "$platformDir" || return $?
			fi
			cmdUpdatePG "$platformDir"
			return $?
			;;
		*) echoError "valid: libs | pg | all"; return 1 ;;
	esac
}

menuPickLibSource(){
	menuPick "Library source" \
		"Apothecary  — openframeworks/apothecary (recommended)|apothecary" \
		"ofLibs      — ofWorks/ofLibs (experimental)|oflibs" \
		"Archive     — libs.danoli3.com historical|archive" \
		|| return 1
	LIB_SOURCE="$UI_MENU_RESULT"
	export LIB_SOURCE
}

menuPickLibPlatform(){
	local source="${1:-$LIB_SOURCE}"
	local current="${2:-$OF_PLATFORM}"
	local -a opts=()
	case "$source" in
		apothecary)
			opts=(
				"osx         — desktop host libraries|osx"
				"macos       — multi-target XCFrameworks (iOS, macOS, tvOS)|macos"
				"ios         — iPhone / iPad|ios"
				"android|android"
				"linux|linux"
				"emscripten|emscripten"
				"msys2|msys2"
				"vs          — Visual Studio|vs"
			)
			;;
		oflibs)
			opts=(
				"osx / macos|osx"
				"linux|linux"
				"linux aarch64|linuxaarch64"
				"emscripten|emscripten"
				"Visual Studio|vs"
			)
			;;
		archive)
			opts=(
				"osx|osx"
				"ios|ios"
				"android|android"
				"linux|linux"
				"msys2|msys2"
				"Visual Studio|vs"
			)
			;;
	esac
	[[ ${#opts[@]} -gt 0 ]] || return 1
	echoNote "detected platform: ${current}${OF_ARCH:+ / ${OF_ARCH}}"
	menuPick "Libraries for which platform?" "${opts[@]}" || return 1
	LIB_PLATFORM="$UI_MENU_RESULT"
	export LIB_PLATFORM
}

menuPickLibTag(){
	local source="${1:-$LIB_SOURCE}"
	local -a opts=()
	local t seen=""
	case "$source" in
		apothecary)
			opts+=("latest (bleeding edge)|latest" "nightly|nightly")
			echoNote "fetching apothecary tags…"
			while IFS= read -r t; do
				[[ -z "$t" || "$t" == "latest" || "$t" == "nightly" ]] && continue
				[[ " $seen " == *" $t "* ]] && continue
				seen+=" $t"
				opts+=("${t}|${t}")
			done < <(listApothecaryReleases | head -20)
			;;
		oflibs)
			echoNote "fetching ofLibs tags…"
			while IFS= read -r t; do
				[[ -n "$t" ]] && opts+=("${t}|${t}")
			done < <(listOfLibsReleases | head -12)
			[[ ${#opts[@]} -eq 0 ]] && opts+=("v1.0|v1.0")
			;;
		archive)
			echoNote "fetching archive versions…"
			while IFS= read -r t; do
				[[ -n "$t" ]] && opts+=("${t}|${t}")
			done < <(listArchiveVersions | sort -Vr | head -24)
			;;
	esac
	[[ ${#opts[@]} -eq 0 ]] && { LIB_TAG="latest"; return 0; }
	menuPick "Release / version · ${source}" "${opts[@]}" || return 1
	LIB_TAG="$UI_MENU_RESULT"
	export LIB_TAG
}

menuPickOfLibs(){
	local tag="${1:-$LIB_TAG}"
	local platformDir="${2:-$OF_PLATFORM}"
	local mode name
	local -a opts=()
	menuPick "ofLibs selection · ${tag} / ${platformDir}" \
		"Core set|core" \
		"All packages for platform|all" \
		"Pick libraries…|pick" \
		|| return 1
	mode="$UI_MENU_RESULT"
	case "$mode" in
		core|all) LIB_LIBS="$mode"; export LIB_LIBS; echoSuccess "libs → $LIB_LIBS"; return 0 ;;
		pick)
			echoNote "listing packages…"
			while IFS= read -r name; do
				[[ -n "$name" ]] && opts+=("${name}|${name}")
			done < <(listOfLibsNames "$tag" "$platformDir")
			[[ ${#opts[@]} -eq 0 ]] && { echoError "no packages"; return 1; }
			menuPickMulti "Select ofLibs" "${opts[@]}" || return 1
			LIB_LIBS="$UI_MENU_RESULT"
			export LIB_LIBS
			echoSuccess "libs → $LIB_LIBS"
			;;
	esac
}

menuDownloadLibs(){
	local platformDir="${1:-$OF_PLATFORM}"
	printBanner "libs"
	echoInfo "download libraries · choose source"
	printf '\n'

	menuPickLibSource || return 2
	echoSuccess "source → $LIB_SOURCE"
	menuPickLibPlatform "$LIB_SOURCE" "$platformDir" || return 2
	platformDir="$LIB_PLATFORM"
	echoSuccess "platform → $platformDir"
	menuPickLibTag "$LIB_SOURCE" || return 2
	echoSuccess "tag → $LIB_TAG"

	# source + tag is enough to start — no extra gates after this. Cross-
	# platform downloads and non-default clean modes are still available via
	# `of update libs <platform>` / LIB_CLEAN_MODE for the cases that need them.
	if [[ "$LIB_SOURCE" == "apothecary" ]]; then
		case "$platformDir" in
			osx)
				echoNote "osx package = desktop host libs"
				echoNote "tip: choose macos for Apple multi-target xcframeworks"
				;;
			macos)
				echoNote "macos packages → lib/macos/*.xcframework (osx · ios · tvos · …)"
				;;
		esac
		export LIB_CLEAN_MODE="${LIB_CLEAN_MODE:-platform}"
		echoKV "clean" "$LIB_CLEAN_MODE"
	fi
	[[ "$LIB_SOURCE" == "oflibs" ]] && { menuPickOfLibs "$LIB_TAG" "$platformDir" || return 2; }

	# force download after explicit update path (user already chose source)
	cmdUpdateLibs "$platformDir" 1
}

archesForApoType(){
	case "$1" in
		osx|macos) echo "arm64 x86_64" ;;
		ios|tvos|watchos|xros) echo "arm64 SIM_arm64 x86_64" ;;
		catos) echo "arm64 x86_64" ;;
		android) echo "arm64 armv7 x86 x86_64" ;;
		emscripten) echo "64 32" ;;
		linux) echo "64 aarch64 armv7l armv6l" ;;
		vs) echo "64 arm64 arm64ec all" ;;
		msys2) echo "64" ;;
		*) echo "${OF_ARCH:-64}" ;;
	esac
}

# Apothecary's own VS_VER env var picks the toolchain (17=VS2022, 18=VS2026,
# 16=VS2019) — if unset it silently defaults to 17 inside apothecary itself,
# regardless of what's actually installed. Default here to 18 (VS2026).
menuPickApoVsVer(){
	menuPick "Visual Studio version for apothecary build" \
		"Visual Studio 2026 (18)|18" \
		"Visual Studio 2022 (17)|17" \
		"Visual Studio 2019 (16)|16" \
		|| return 1
	return 0
}

runApothecaryEngine(){
	local type="$1" arch="$2"
	shift 2
	local -a cmd=()
	resolveApothecary
	[[ -f "$APO_ENGINE" ]] || { echoError "apothecary engine missing — git submodule update --init scripts/apothecary"; return 1; }
	[[ -x "$APO_ENGINE" ]] || chmod +x "$APO_ENGINE" 2>/dev/null || true
	mkdir -p "${OF_DIR}/libs" "$APO_BUILD_DIR"
	cmd=( "$APO_ENGINE" -t "$type" )
	[[ -n "$arch" ]] && cmd+=( -a "$arch" )
	cmd+=( -b "$APO_BUILD_DIR" -d "${OF_DIR}/libs" )
	[[ "$VERBOSE" = 1 ]] && cmd+=( -v )
	cmd+=( "$@" )
	echoNote "${cmd[*]}"
	if [[ "$type" == "vs" ]]; then
		local vsVer="${OF_APO_VS_VER:-18}"
		echoKV "VS_VER" "${vsVer} ($([[ "$vsVer" == 18 ]] && echo 2026 || { [[ "$vsVer" == 17 ]] && echo 2022 || echo 2019; }))"
		( cd "${APO_HOME}/apothecary" 2>/dev/null || cd "$APO_HOME" || exit 1; VS_VER="$vsVer" "${cmd[@]}" )
	else
		( cd "${APO_HOME}/apothecary" 2>/dev/null || cd "$APO_HOME" || exit 1; "${cmd[@]}" )
	fi
}

launchApoMenu(){
	resolveApothecary
	[[ -f "$APO_CLI" ]] || {
		echoError "apo CLI not found in scripts/apothecary"
		echoNote "export APOTHECARY_HOME=/path/to/apothecary with scripts/apo.sh"
		return 1
	}
	echoInfo "apothecary menu · output → ${OF_DIR}/libs"
	mkdir -p "${OF_DIR}/libs" "${APO_BUILD_DIR:-${APO_HOME}/build}"
	(
		export OUTPUT_FOLDER="${OF_DIR}/libs"
		export BUILD_DIR="${APO_BUILD_DIR:-${APO_HOME}/build}"
		export TYPE="${TYPE:-$OF_PLATFORM}"
		export ARCH="${ARCH:-$OF_ARCH}"
		export TARGET="$TYPE"
		bash "$APO_CLI" menu
	)
}

cmdApothecaryBuildAll(){
	local type="${1:-$OF_PLATFORM}" arch="${2:-$OF_ARCH}"
	printBanner "apothecary"
	echoInfo "build all core · ${type}/${arch}"
	echoKV "out" "${OF_DIR}/libs"
	[[ "$type" == "osx" || "$type" == "macos" ]] && \
		echoNote "macos = multi-target xcframeworks; osx = desktop host"
	confirmYes "Run apothecary update core for ${type}/${arch}?" || { echoInfo "cancelled"; return 0; }
	tasksBegin "Tasks" "Resolve apothecary" "Platform" "update core" "Finish"
	resolveApothecary
	[[ -n "$APO_ENGINE" ]] || { taskSet 0 fail; tasksSkipRest; tasksSummary; return 1; }
	taskSet 0 done "$APO_HOME"
	taskSet 1 done "${type}/${arch}"
	if ! taskLive 2 -- runApothecaryEngine "$type" "$arch" update core; then
		taskTickLine 3 skip; tasksSummary; return 1
	fi
	taskTickLine 3 done
	tasksSummary
	echoSuccess "core built → ${OF_DIR}/libs"
}

menuApothecary(){
	local choice type arch name a
	local -a opts aopts arches
	resolveApothecary
	printBanner "apothecary"
	echoKV "submodule" "${APO_HOME:-missing}"
	echoKV "engine" "${APO_ENGINE:-missing}"
	echoKV "apo menu" "${APO_CLI:-not found}"
	echoKV "output" "${OF_DIR}/libs"
	printf '\n'
	[[ -n "$APO_ENGINE" ]] || { echoError "init scripts/apothecary submodule"; return 1; }

	opts=(
		"Build all core for this machine (${OF_PLATFORM}/${OF_ARCH})|build-host"
		"Build all core for platform…|build-type"
		"Build single library…|build-one"
	)
	[[ -n "$APO_CLI" ]] && opts+=("Open Apothecary interactive menu|apo-menu")
	opts+=("Status|status" "Back|back")
	# return 2 (not 0) for every "nothing happened, back/cancelled" exit below
	# so the caller can skip the "press Enter to return" pause and go straight
	# back up a level instead of pausing on an empty/unchanged screen.
	menuPick "Apothecary builds" "${opts[@]}" || return 2
	choice="$UI_MENU_RESULT"

	case "$choice" in
		build-host)
			if [[ "$OF_PLATFORM" == "vs" ]]; then
				menuPickApoVsVer || return 2
				export OF_APO_VS_VER="$UI_MENU_RESULT"
			fi
			cmdApothecaryBuildAll "$OF_PLATFORM" "$OF_ARCH"
			;;
		build-type)
			opts=()
			for type in "${APO_BUILD_TYPES[@]}"; do
				if [[ "$type" == "macos" ]]; then
					opts+=("macos  — Apple multi-target xcframeworks|macos")
				elif [[ "$type" == "osx" ]]; then
					opts+=("osx  — desktop host|osx")
				else
					opts+=("${type}|${type}")
				fi
			done
			[[ "$OF_PLATFORM" == "osx" ]] && echoNote "macos covers osx · ios · tvos · xros · watchos · catos"
			menuPick "Build platform" "${opts[@]}" || return 2
			type="$UI_MENU_RESULT"
			if [[ "$type" == "vs" ]]; then
				menuPickApoVsVer || return 2
				export OF_APO_VS_VER="$UI_MENU_RESULT"
			fi
			aopts=()
			read -r -a arches <<< "$(archesForApoType "$type")"
			for a in "${arches[@]}"; do
				aopts+=("${a}|${a}")
			done
			menuPick "Architecture · ${type}" "${aopts[@]}" || return 2
			cmdApothecaryBuildAll "$type" "$UI_MENU_RESULT"
			;;
		build-one)
			opts=()
			for type in "${APO_BUILD_TYPES[@]}"; do
				opts+=("${type}|${type}")
			done
			menuPick "Platform" "${opts[@]}" || return 2
			type="$UI_MENU_RESULT"
			if [[ "$type" == "vs" ]]; then
				menuPickApoVsVer || return 2
				export OF_APO_VS_VER="$UI_MENU_RESULT"
			fi
			aopts=()
			read -r -a arches <<< "$(archesForApoType "$type")"
			for a in "${arches[@]}"; do aopts+=("${a}|${a}"); done
			menuPick "Architecture · ${type}" "${aopts[@]}" || return 2
			arch="$UI_MENU_RESULT"
			opts=()
			local f
			for f in "${APO_HOME}/apothecary/formulas"/*; do
				name=$(basename "$f")
				[[ "$name" == _* ]] && continue
				if [[ -d "$f" ]]; then opts+=("${name}|${name}")
				elif [[ "$name" == *.sh ]]; then opts+=("${name%.sh}|${name%.sh}"); fi
			done
			menuPick "Formula · ${type}/${arch}" "${opts[@]}" || return 2
			name="$UI_MENU_RESULT"
			confirmYes "Build ${name} for ${type}/${arch}?" || return 2
			tasksBegin "Tasks" "Resolve" "Build ${name}" "Finish"
			taskSet 0 done "$APO_HOME"
			if ! taskLive 1 -- runApothecaryEngine "$type" "$arch" update "$name"; then
				taskTickLine 2 skip; tasksSummary; return 1
			fi
			taskTickLine 2 done
			tasksSummary
			;;
		apo-menu) launchApoMenu ;;
		status)
			resolveApothecary
			printBanner "apothecary"
			echoKV "home" "${APO_HOME:-—}"
			echoKV "engine" "${APO_ENGINE:-—}"
			echoKV "cli" "${APO_CLI:-—}"
			echoKV "libs out" "${OF_DIR}/libs"
			printf '\n'
			;;
		back) return 2 ;;
	esac
}

cmdUpgrade(){
	local subcmd="$1"
	local script="${OF_CORE_SCRIPT_DIR}/dev/upgrade.sh"
	case "$subcmd" in
		addons|apps) ;;
		*) echoError "valid: addons | apps"; return 1 ;;
	esac
	printBanner "upgrade"
	tasksBegin "Tasks" "Confirm" "Locate script" "Upgrade ${subcmd}" "Finish"
	echoWarning "modifies ${subcmd}/ — backup first"
	confirmYes "Continue upgrade ${subcmd}?" || { taskSet 0 skip; tasksSkipRest; echoInfo "cancelled"; return 0; }
	taskSet 0 done "confirmed"
	if ! ensureScript "$script"; then taskSet 1 fail; tasksSkipRest; tasksSummary; return 1; fi
	taskSet 1 done "dev/upgrade.sh"
	if ! taskLive 2 -- "$script" "$subcmd"; then taskTickLine 3 skip; tasksSummary; return 1; fi
	taskTickLine 3 done
	tasksSummary
}

# Single menu entry point for "Upgrade" — old-layout projects/addons
# (pre-0.12 linux64 → linux/64 path style) up to the current SDK version.
menuUpgrade(){
	local ofVer choice
	ofVer=$(readOfVersion) || ofVer=""
	menuPick "Upgrade  — oF 7.0–11.0 → ${ofVer:+v${ofVer} }(Latest SDK)" \
		"Addons only|addons" \
		"Projects (apps/)|apps" \
		"Both|both" \
		"Back|back" \
		|| return 2
	choice="$UI_MENU_RESULT"
	case "$choice" in
		addons) cmdUpgrade addons; return $? ;;
		apps)   cmdUpgrade apps; return $? ;;
		both)   cmdUpgrade addons; cmdUpgrade apps ;;
		back)   return 2 ;;
	esac
}

cmdSetup(){
	local platformDir="${1:-$OF_PLATFORM}"
	local distroDir depsScript codecsScript
	local doCodecs=0 taskN=0 needLibs=0 needPG=0
	local -a taskNames=()

	# Setup always uses stock apothecary latest for libs
	export LIB_SOURCE=apothecary
	export LIB_TAG=latest

	printBanner "setup"
	echoInfo "setup · ${platformDir}  (libs source: apothecary @ latest)"
	printf '\n'

	assessLibsState "$platformDir" "apothecary" "latest"
	echoKV "libs" "${LIBS_STATE} — ${LIBS_STATE_DETAIL}"
	[[ "$LIBS_STATE" != "ok" ]] && needLibs=1

	assessPGState
	echoKV "pg" "${PG_STATE} — ${PG_STATE_DETAIL}"
	[[ "$PG_STATE" != "ok" ]] && needPG=1

	if [[ "$OF_PLATFORM" == "linux" ]]; then
		detectLinuxDistro
		[[ -n "$OF_LINUX_DISTRO" ]] || pickLinuxDistro || return 1
		distroDir=$(linuxDistroScriptDir)
		depsScript="${distroDir}/install_dependencies.sh"
		codecsScript="${distroDir}/install_codecs.sh"
		echoKV "distro" "$OF_LINUX_DISTRO"
		if [[ -f "$codecsScript" ]]; then
			if [[ "${OF_SETUP_CODECS:-}" = 1 ]]; then doCodecs=1
			elif [[ "${OF_SETUP_CODECS:-}" = 0 ]]; then doCodecs=0
			elif menuCanRun && confirmYes "Install media codecs too?"; then doCodecs=1
			fi
		fi
	elif [[ "$OF_PLATFORM" == "osx" ]]; then
		depsScript="${OF_CORE_SCRIPT_DIR}/osx/install_dependencies.sh"
	elif [[ "$OF_PLATFORM" == "vs" || "$OF_PLATFORM" == "msys2" ]]; then
		depsScript="${OF_CORE_SCRIPT_DIR}/vs/install_dependencies.sh"
	fi

	if [[ "$needLibs" -eq 0 && "$needPG" -eq 0 && "$OF_PLATFORM" != "linux" && "$OF_PLATFORM" != "osx" \
		&& "$OF_PLATFORM" != "vs" && "$OF_PLATFORM" != "msys2" ]]; then
		echoSuccess "already set up — libs + PG look current"
		echoNote "use Update to redownload from a chosen source"
		printf '\n'
		return 0
	fi

	taskNames+=("Detect platform")
	[[ "$OF_PLATFORM" == "linux" ]] && taskNames+=("Install dependencies (${OF_LINUX_DISTRO})")
	[[ "$doCodecs" -eq 1 ]] && taskNames+=("Install codecs (${OF_LINUX_DISTRO})")
	[[ "$OF_PLATFORM" == "osx" ]] && taskNames+=("Xcode CLT / Homebrew (cmake, gum)")
	[[ "$OF_PLATFORM" == "vs" || "$OF_PLATFORM" == "msys2" ]] && taskNames+=("gum via winget/scoop (optional)")
	if [[ "$needLibs" -eq 1 ]]; then
		taskNames+=("Download libraries (apothecary @ latest)")
	else
		taskNames+=("Libraries (already current)")
	fi
	if [[ "$needPG" -eq 1 ]]; then
		taskNames+=("Download Project Generator")
	else
		taskNames+=("Project Generator (already installed)")
	fi
	taskNames+=("Finish")

	tasksBegin "Tasks" "${taskNames[@]}"
	taskSet 0 done "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}${OF_LINUX_DISTRO:+ · ${OF_LINUX_DISTRO}}"
	taskN=1

	if [[ "$OF_PLATFORM" == "linux" ]]; then
		[[ -f "$depsScript" ]] || { taskSet "$taskN" fail; tasksSkipRest; tasksSummary; return 1; }
		if ! taskLive "$taskN" -- runDistroInstallScript "$depsScript"; then
			tasksSkipRest; tasksSummary; return 1
		fi
		taskN=$((taskN + 1))
		if [[ "$doCodecs" -eq 1 ]]; then
			if [[ -f "$codecsScript" ]]; then
				if ! taskLive "$taskN" -- runDistroInstallScript "$codecsScript"; then
					tasksSkipRest; tasksSummary; return 1
				fi
			else
				taskTickLine "$taskN" skip
			fi
			taskN=$((taskN + 1))
		fi
	elif [[ "$OF_PLATFORM" == "osx" ]]; then
		if [[ -f "$depsScript" ]]; then
			ensureScript "$depsScript" 2>/dev/null
			# best-effort: CLT/Homebrew are checked, not required to proceed — libs/PG
			# come from openFrameworks' own apothecary download either way.
			taskLive "$taskN" -- "$depsScript" -y
		else
			taskTickLine "$taskN" skip
		fi
		taskN=$((taskN + 1))
	elif [[ "$OF_PLATFORM" == "vs" || "$OF_PLATFORM" == "msys2" ]]; then
		if [[ -f "$depsScript" ]]; then
			ensureScript "$depsScript" 2>/dev/null
			# best-effort: gum is a nicety, not required — libs/PG come from
			# openFrameworks' own apothecary download either way.
			taskLive "$taskN" -- "$depsScript" -y
		else
			taskTickLine "$taskN" skip
		fi
		taskN=$((taskN + 1))
	fi

	# libraries — only if missing/outdated
	if [[ "$needLibs" -eq 1 ]]; then
		echoNote "downloading libraries · apothecary @ latest"
		if ! taskLive "$taskN" -- runLibsDownload "$platformDir"; then
			tasksSkipRest; tasksSummary; return 1
		fi
	else
		taskSet "$taskN" done "${LIBS_STATE_DETAIL}"
	fi
	taskN=$((taskN + 1))

	# PG — only if missing (or force latest when missing path already covered)
	if [[ "$needPG" -eq 1 ]]; then
		if ! taskLive "$taskN" -- runPGDownload "$platformDir"; then
			tasksSkipRest; tasksSummary; return 1
		fi
	else
		taskSet "$taskN" done "${PG_STATE_DETAIL}"
	fi
	taskN=$((taskN + 1))
	taskTickLine "$taskN" done
	tasksSummary
	echoSuccess "setup complete"
	[[ "$OF_PLATFORM" == "linux" ]] && echoNote "next: scripts/linux/compileOF.sh"
	printf '\n'
}

# ---------------------------------------------------------------------------
# Cleanup — projects / caches / libraries
# ---------------------------------------------------------------------------

# KB-on-disk for existing paths (0 for none) — one batched `du`, not per-path
pathsSizeKB(){
	local -a existing=()
	local p
	for p in "$@"; do
		[[ -e "$p" ]] && existing+=("$p")
	done
	[[ ${#existing[@]} -eq 0 ]] && { printf '0'; return 0; }
	du -ck "${existing[@]}" 2>/dev/null | tail -1 | awk '{print $1}'
}

formatKB(){
	awk -v kb="${1:-0}" 'BEGIN{printf "%.1f MB", kb/1024}'
}

# True if $1 (or anything under it) is tracked by git — used to keep the
# nuclear/platform lib-cleanup commands from deleting committed, vendored
# addon source (e.g. addons/ofxKinect/libs/libfreenect, ofxEmscripten's
# html5audio/html5video/cors) that happens to live under a libs/ or
# lib/<platform>/ path alongside real downloaded prebuilt binaries.
# Best-effort: if this isn't a git checkout (e.g. a release zip) or git
# isn't available, returns 1 (not tracked) so behavior is unchanged there.
pathIsGitTracked(){
	local p="$1"
	command -v git >/dev/null 2>&1 || return 1
	git -C "$OF_DIR" rev-parse --is-inside-work-tree >/dev/null 2>&1 || return 1
	[[ -n "$(git -C "$OF_DIR" ls-files -- "$p" 2>/dev/null | head -1)" ]]
}

# Best-effort "send to trash/recycle bin" for a batch of paths instead of a
# permanent rm -rf — one external call for the whole batch (trashing dozens
# of build artifacts one at a time would reintroduce the per-item slowness
# cleanup was just fixed for). Falls back to permanent delete if no trash
# mechanism is available. Sets TRASH_METHOD to "trash" or "rm" so callers can
# report "moved to Trash" vs "freed" accurately — Trash/Recycle Bin doesn't
# actually reclaim disk space until it's emptied.
trashPaths(){
	local -a paths=()
	local p
	for p in "$@"; do
		[[ -e "$p" ]] && paths+=("$p")
	done
	TRASH_METHOD="rm"
	[[ ${#paths[@]} -eq 0 ]] && return 0

	case "$OF_PLATFORM" in
		vs|msys2)
			if command -v powershell.exe >/dev/null 2>&1; then
				local wp psList=""
				for p in "${paths[@]}"; do
					wp=$(command -v cygpath >/dev/null 2>&1 && cygpath -w "$p" 2>/dev/null || printf '%s' "$p")
					wp="${wp//\'/\'\'}"
					psList+="'${wp}',"
				done
				psList="${psList%,}"
				local psCmd="Add-Type -AssemblyName Microsoft.VisualBasic; foreach (\$p in @(${psList})) { if (Test-Path -LiteralPath \$p -PathType Container) { [Microsoft.VisualBasic.FileIO.FileSystem]::DeleteDirectory(\$p, 'OnlyErrorDialogs', 'SendToRecycleBin') } elseif (Test-Path -LiteralPath \$p) { [Microsoft.VisualBasic.FileIO.FileSystem]::DeleteFile(\$p, 'OnlyErrorDialogs', 'SendToRecycleBin') } }"
				powershell.exe -NoProfile -NonInteractive -Command "$psCmd" >/dev/null 2>&1 && TRASH_METHOD="trash"
			fi
			;;
		osx)
			if command -v trash >/dev/null 2>&1 && trash -F "${paths[@]}" >/dev/null 2>&1; then
				TRASH_METHOD="trash"
			else
				local asList="" esc
				for p in "${paths[@]}"; do
					esc="${p//\"/\\\"}"
					asList+="POSIX file \"${esc}\", "
				done
				asList="${asList%, }"
				osascript -e "tell application \"Finder\" to delete {${asList}}" >/dev/null 2>&1 && TRASH_METHOD="trash"
			fi
			;;
		linux)
			if command -v gio >/dev/null 2>&1 && gio trash "${paths[@]}" >/dev/null 2>&1; then
				TRASH_METHOD="trash"
			elif command -v trash-put >/dev/null 2>&1 && trash-put "${paths[@]}" >/dev/null 2>&1; then
				TRASH_METHOD="trash"
			elif command -v trash >/dev/null 2>&1 && trash "${paths[@]}" >/dev/null 2>&1; then
				TRASH_METHOD="trash"
			fi
			;;
	esac

	[[ "$TRASH_METHOD" == "trash" ]] || rm -rf "${paths[@]}" 2>/dev/null || true
}

# Fast, stat-only check — no `find`, no `make clean` invocation. Lets callers
# skip the expensive deep clean entirely for projects with nothing built
# (this is what made "clean all examples" slow, especially over MSYS2/Windows
# filesystems where every extra process/find is costly).
projectHasBuildArtifacts(){
	local project="$1"
	[[ -d "${project}/obj" ]] && return 0
	[[ -d "${project}/build" ]] && return 0
	[[ -d "${project}/.vs" ]] && return 0
	[[ -d "${project}/cmake-build-debug" ]] && return 0
	[[ -d "${project}/cmake-build-release" ]] && return 0
	[[ -d "${project}/DerivedData" ]] && return 0
	compgen -G "${project}"/*.xcodeproj/xcuserdata >/dev/null 2>&1 && return 0
	compgen -G "${project}"/*.xcodeproj/project.xcworkspace >/dev/null 2>&1 && return 0
	return 1
}

# Cleans one project dir. Sets CLEAN_TREE_DID_CLEAN (0/1), CLEAN_TREE_FREED_KB,
# and CLEAN_TREE_METHOD ("trash"|"rm"). Never touches <project>/bin — that's
# where oF projects keep bin/data (assets) alongside the built binary, so it's
# left alone entirely. `make clean` (the project's own Makefile) may still
# remove its own built binary from bin/ — that's the Makefile's business, not
# ours; we don't add any rm/find of our own under bin/.
cleanProjectTree(){
	local project="$1"
	CLEAN_TREE_DID_CLEAN=0
	CLEAN_TREE_FREED_KB=0
	CLEAN_TREE_METHOD="rm"
	[[ -d "$project" ]] || return 1

	# nothing built here — skip make/find entirely
	projectHasBuildArtifacts "$project" || return 0

	local -a targets=(
		"${project}/obj"
		"${project}/build"
		"${project}/.vs"
		"${project}/cmake-build-debug"
		"${project}/cmake-build-release"
		"${project}/DerivedData"
	)

	# exclude bin/ and anything already covered by $targets, so the deep finds
	# below don't re-match (and double-count/double-trash) files inside them
	local -a prune=(-not -path "${project}/bin/*")
	local t
	for t in "${targets[@]}"; do
		prune+=(-not -path "${t}/*")
	done

	local -a deepTargets=()
	local g
	while IFS= read -r -d '' g; do
		deepTargets+=("$g")
	done < <(find "$project" -maxdepth 3 "${prune[@]}" \( \
		-name 'xcuserdata' -o -name 'project.xcworkspace' -o \
		-name '*.o' -o -name '*.d' -o -name '*.depend' -o -name '*.layout' \
		\) -print0 2>/dev/null)
	while IFS= read -r -d '' g; do
		deepTargets+=("$g")
	done < <(find "$project" -maxdepth 3 "${prune[@]}" \( \
		-name 'x64' -o -name 'ARM64' -o -name 'Win32' -o \
		-name 'Debug' -o -name 'Release' \
		\) -type d -path '*/obj/*' -print0 2>/dev/null)

	local freedKB
	freedKB=$(pathsSizeKB "${targets[@]}" "${deepTargets[@]}")

	if [[ -f "${project}/Makefile" ]] || [[ -f "${project}/makefile" ]]; then
		( cd "$project" && make clean >/dev/null 2>&1 ) || true
	fi

	trashPaths "${targets[@]}" "${deepTargets[@]}"
	CLEAN_TREE_METHOD="$TRASH_METHOD"

	CLEAN_TREE_DID_CLEAN=1
	CLEAN_TREE_FREED_KB=$freedKB
	return 0
}

# Clean one project dir and print its ✓/– result line
cleanProjectsScopeOne(){
	local proj="$1" rel="$2"
	cleanProjectTree "$proj"
	if [[ "$CLEAN_TREE_DID_CLEAN" == "1" ]]; then
		local verb="freed"
		[[ "$CLEAN_TREE_METHOD" == "trash" ]] && verb="moved to Trash"
		printf '  %s✓%s %s%s%s  %s(%s %s)%s\n' \
			"$C_OK" "$C_RESET" "$C_FG" "$rel" "$C_RESET" "$C_MUTED" "$(formatKB "$CLEAN_TREE_FREED_KB")" "$verb" "$C_RESET"
	else
		printf '  %s–%s %s%s%s  %s(nothing to clean)%s\n' \
			"$C_MUTED" "$C_RESET" "$C_FG" "$rel" "$C_RESET" "$C_MUTED" "$C_RESET"
	fi
}

# Walk apps/ or examples/ (or a custom root) and clean each project-like folder
cleanProjectsScope(){
	local scope="$1"   # apps|examples|all|path
	local path="${2:-}"
	local root cleaned=0 skipped=0 totalFreedKB=0
	local -a roots=()

	case "$scope" in
		apps) roots+=("${OF_DIR}/apps") ;;
		examples) roots+=("${OF_DIR}/examples") ;;
		all) roots+=("${OF_DIR}/apps" "${OF_DIR}/examples") ;;
		path)
			[[ -n "$path" && -d "$path" ]] || { echoError "path required"; return 1; }
			roots+=("$path")
			;;
		*) echoError "scope: apps|examples|all|path"; return 1 ;;
	esac

	printBanner "clean"
	echoInfo "cleanup projects · ${scope}${path:+ · $path}"
	echoNote "bin/ (incl. bin/data) is never touched — only obj/build/.vs/etc."
	printf '\n'

	local cat ex proj rel
	for root in "${roots[@]}"; do
		[[ -d "$root" ]] || continue
		# category / project  (apps/myApps/foo, examples/graphics/bar)
		for cat in "$root"/*/; do
			[[ -d "$cat" ]] || continue
			# direct project under category
			if [[ -d "${cat}src" || -f "${cat}Makefile" ]]; then
				proj="${cat%/}"
				rel="${proj#"$OF_DIR"/}"
				cleanProjectsScopeOne "$proj" "$rel"
				if [[ "$CLEAN_TREE_DID_CLEAN" == "1" ]]; then
					cleaned=$((cleaned + 1))
					totalFreedKB=$((totalFreedKB + CLEAN_TREE_FREED_KB))
				else
					skipped=$((skipped + 1))
				fi
				continue
			fi
			for ex in "$cat"*/; do
				[[ -d "$ex" ]] || continue
				if [[ -d "${ex}src" || -f "${ex}Makefile" || -f "${ex}makefile" ]] \
					|| compgen -G "${ex}*.xcodeproj" >/dev/null 2>&1 \
					|| compgen -G "${ex}*.vcxproj" >/dev/null 2>&1; then
					proj="${ex%/}"
					rel="${proj#"$OF_DIR"/}"
					cleanProjectsScopeOne "$proj" "$rel"
					if [[ "$CLEAN_TREE_DID_CLEAN" == "1" ]]; then
						cleaned=$((cleaned + 1))
						totalFreedKB=$((totalFreedKB + CLEAN_TREE_FREED_KB))
					else
						skipped=$((skipped + 1))
					fi
				fi
			done
		done
	done
	printf '\n'
	echoSuccess "cleaned ${cleaned} project(s) · skipped ${skipped} (nothing to clean) · freed $(formatKB "$totalFreedKB")"
}

cleanDownloadCaches(){
	local mode="${1:-packages}" # packages|all
	local dl="${OF_DIR}/libs/download"
	local freedKB=0
	printBanner "clean"
	echoInfo "clean caches · ${mode}"
	printf '\n'
	if [[ ! -d "$dl" ]]; then
		echoNote "no libs/download cache"
		return 0
	fi
	case "$mode" in
		packages)
			# package archives + sidecars; keep folder
			local -a files=()
			local f n=0
			while IFS= read -r f; do
				[[ -f "$f" ]] && files+=("$f")
			done < <(find "$dl" -type f \( \
				-name '*.tar.bz2' -o -name '*.tar.gz' -o -name '*.zip' -o \
				-name '*.sha256' -o -name 'SHA256SUMS' -o -name '.last-verify' \
				\) 2>/dev/null)
			freedKB=$(pathsSizeKB "${files[@]}")
			for f in "${files[@]}"; do
				rm -f "$f"
				n=$((n + 1))
			done
			echoSuccess "removed ${n} cached package file(s) under libs/download · freed $(formatKB "$freedKB")"
			;;
		all)
			echoWarning "removing entire libs/download/"
			freedKB=$(pathsSizeKB "$dl")
			rm -rf "$dl"
			mkdir -p "$dl"
			echoSuccess "libs/download cleared · freed $(formatKB "$freedKB")"
			;;
		*) echoError "mode: packages|all"; return 1 ;;
	esac
	# optional compile junk under openFrameworksCompiled
	if [[ "$mode" == "all" ]]; then
		local -a junk=()
		while IFS= read -r f; do
			[[ -d "$f" ]] && junk+=("$f")
		done < <(find "${OF_DIR}/libs/openFrameworksCompiled" -type d \( -name 'obj' -o -name 'intermediates' \) 2>/dev/null)
		if [[ ${#junk[@]} -gt 0 ]]; then
			local junkKB
			junkKB=$(pathsSizeKB "${junk[@]}")
			rm -rf "${junk[@]}" 2>/dev/null || true
			echoNote "also cleared openFrameworksCompiled obj/intermediates · freed $(formatKB "$junkKB")"
		fi
	fi
}

# Remove prebuilt library binaries (minimise disk) — never deletes openFrameworks source
cleanLibraries(){
	local mode="$1" # other-platforms|platform|all-prebuilt|list
	local plat="${2:-}"
	local libDir d name p count=0 skipped=0 totalFreedKB=0 dKB

	printBanner "clean"
	echoInfo "libraries · ${mode}${plat:+ · $plat}"
	printf '\n'

	case "$mode" in
		list)
			echoNote "installed lib/<platform> folders:"
			find "${OF_DIR}/libs" "${OF_DIR}/addons" -type d -path '*/lib/*' -mindepth 3 -maxdepth 4 2>/dev/null \
				| sed "s|^${OF_DIR}/||" | sort -u | head -80
			return 0
			;;
		other-platforms)
			# keep host-related platform dirs; remove the rest
			local keep=""
			case "$OF_PLATFORM" in
				vs) keep="vs" ;;
				osx|macos) keep="macos|osx" ;;
				ios) keep="macos|ios" ;;
				android) keep="android" ;;
				emscripten) keep="emscripten" ;;
				msys2) keep="msys2|vs" ;;
				linux) keep="linux|linux64|linuxaarch64" ;;
				*) keep="$OF_PLATFORM" ;;
			esac
			echoKV "keep matching" "$keep"
			while IFS= read -r d; do
				name=$(basename "$d")
				if echo "$name" | grep -Eq "^(${keep})$"; then
					continue
				fi
				# skip non-platform utility dirs
				case "$name" in
					pkgconfig|cmake|cmake-build*|include) continue ;;
				esac
				if pathIsGitTracked "$d"; then
					skipped=$((skipped + 1))
					printf '  %s–%s %s  %s(git-tracked source — skipped)%s\n' "$C_MUTED" "$C_RESET" "${d#"$OF_DIR"/}" "$C_MUTED" "$C_RESET"
					continue
				fi
				dKB=$(pathsSizeKB "$d")
				rm -rf "$d"
				totalFreedKB=$((totalFreedKB + dKB))
				count=$((count + 1))
				printf '  %s✓%s %s  %s(%s freed)%s\n' "$C_OK" "$C_RESET" "${d#"$OF_DIR"/}" "$C_MUTED" "$(formatKB "$dKB")" "$C_RESET"
			done < <(find "${OF_DIR}/libs" "${OF_DIR}/addons" -type d -path '*/lib/*' -mindepth 3 -maxdepth 4 2>/dev/null)
			printf '\n'
			echoSuccess "removed ${count} non-host platform lib folder(s) · skipped ${skipped} (git-tracked) · freed $(formatKB "$totalFreedKB")"
			;;
		platform)
			[[ -n "$plat" ]] || { echoError "platform name required"; return 1; }
			while IFS= read -r d; do
				if pathIsGitTracked "$d"; then
					skipped=$((skipped + 1))
					printf '  %s–%s %s  %s(git-tracked source — skipped)%s\n' "$C_MUTED" "$C_RESET" "${d#"$OF_DIR"/}" "$C_MUTED" "$C_RESET"
					continue
				fi
				dKB=$(pathsSizeKB "$d")
				rm -rf "$d"
				totalFreedKB=$((totalFreedKB + dKB))
				count=$((count + 1))
				printf '  %s✓%s %s  %s(%s freed)%s\n' "$C_OK" "$C_RESET" "${d#"$OF_DIR"/}" "$C_MUTED" "$(formatKB "$dKB")" "$C_RESET"
			done < <(find "${OF_DIR}/libs" "${OF_DIR}/addons" -type d -path "*/lib/${plat}" 2>/dev/null)
			printf '\n'
			echoSuccess "removed ${count} path(s) for platform ${plat} · skipped ${skipped} (git-tracked) · freed $(formatKB "$totalFreedKB")"
			;;
		all-prebuilt)
			echoWarning "This deletes downloaded prebuilts under libs/* and addon libs binaries."
			echoWarning "Keeps: libs/openFrameworks, libs/openFrameworksCompiled (source/project)."
			echoWarning "Also keeps anything git-tracked (vendored addon source, e.g. ofxKinect/libs/libfreenect)."
			for d in "${OF_DIR}/libs"/*; do
				[[ -d "$d" ]] || continue
				name=$(basename "$d")
				case "$name" in
					openFrameworks|openFrameworksCompiled|download|scripts) continue ;;
				esac
				if pathIsGitTracked "$d"; then
					skipped=$((skipped + 1))
					printf '  %s–%s libs/%s  %s(git-tracked source — skipped)%s\n' "$C_MUTED" "$C_RESET" "$name" "$C_MUTED" "$C_RESET"
					continue
				fi
				dKB=$(pathsSizeKB "$d")
				rm -rf "$d"
				totalFreedKB=$((totalFreedKB + dKB))
				count=$((count + 1))
				printf '  %s✓%s libs/%s  %s(%s freed)%s\n' "$C_OK" "$C_RESET" "$name" "$C_MUTED" "$(formatKB "$dKB")" "$C_RESET"
			done
			# addon binary folders only
			for d in "${OF_DIR}/addons"/*/libs; do
				[[ -d "$d" ]] || continue
				if pathIsGitTracked "$d"; then
					skipped=$((skipped + 1))
					printf '  %s–%s %s  %s(git-tracked source — skipped)%s\n' "$C_MUTED" "$C_RESET" "${d#"$OF_DIR"/}" "$C_MUTED" "$C_RESET"
					continue
				fi
				dKB=$(pathsSizeKB "$d")
				rm -rf "$d"
				totalFreedKB=$((totalFreedKB + dKB))
				count=$((count + 1))
				printf '  %s✓%s %s  %s(%s freed)%s\n' "$C_OK" "$C_RESET" "${d#"$OF_DIR"/}" "$C_MUTED" "$(formatKB "$dKB")" "$C_RESET"
			done
			rm -f "${OF_DIR}/libs/.of-cli-state" 2>/dev/null || true
			printf '\n'
			echoSuccess "removed ${count} prebuilt tree(s) · skipped ${skipped} (git-tracked) · freed $(formatKB "$totalFreedKB") — run Update libs to restore"
			;;
		*) echoError "mode: list|other-platforms|platform|all-prebuilt"; return 1 ;;
	esac
}

menuCleanup(){
	local choice scope plat
	printBanner "clean"
	echoInfo "cleanup projects · caches · libraries"
	printf '\n'

	while true; do
		menuPick "Cleanup" \
			"Projects — obj/build/.vs artifacts (bin/ untouched)|projects" \
			"Caches — libs/download packages|caches" \
			"Libraries — remove / minimise prebuilts|libs" \
			"Back|back" \
			|| return 0
		choice="$UI_MENU_RESULT"
		case "$choice" in
			projects)
				menuPick "Project cleanup scope" \
					"All apps + examples|all" \
					"apps/ only|apps" \
					"examples/ only|examples" \
					"Single path…|path" \
					"Back|back" || continue
				scope="$UI_MENU_RESULT"
				[[ "$scope" == "back" ]] && continue
				if [[ "$scope" == "path" ]]; then
					menuInput "Project or folder path" || continue
					cleanProjectsScope path "$UI_INPUT_RESULT"
				else
					cleanProjectsScope "$scope"
				fi
				menuPause
				;;
			caches)
				menuPick "Cache cleanup" \
					"Package downloads only (Recommended)|packages" \
					"Wipe entire libs/download + compiled intermediates|all" \
					"Back|back" || continue
				[[ "$UI_MENU_RESULT" == "back" ]] && continue
				cleanDownloadCaches "$UI_MENU_RESULT"
				menuPause
				;;
			libs)
				menuPick "Library cleanup" \
					"Remove ALL prebuilt libs (nuclear)|all" \
					"Remove other platforms (keep this host) — Recommended|other" \
					"Remove one platform…|one" \
					"List installed lib/<platform> folders|list" \
					"Back|back" || continue
				case "$UI_MENU_RESULT" in
					list) cleanLibraries list; menuPause ;;
					other) cleanLibraries other-platforms; menuPause ;;
					one)
						menuPick "Platform folder to remove" \
							"vs|vs" "macos|macos" "osx|osx" "android|android" \
							"emscripten|emscripten" "msys2|msys2" "linux|linux" \
							"ios|ios" "Custom…|custom" || continue
						plat="$UI_MENU_RESULT"
						if [[ "$plat" == "custom" ]]; then
							menuInput "lib/<name> folder" || continue
							plat="$UI_INPUT_RESULT"
						fi
						cleanLibraries platform "$plat"
						menuPause
						;;
					all) cleanLibraries all-prebuilt; menuPause ;;
				esac
				;;
			back) return 0 ;;
		esac
	done
}

cmdMenu(){
	# labelled like the ofBaseApp lifecycle (setup/update/draw/exit) since
	# this is openFrameworks' own menu — fn name shown dim on the right
	local choice
	local setupDesc="libs + Project Generator (apothecary @ latest)"
	if ! menuCanRun; then
		echoWarning "no TTY — showing status"
		cmdStatus
		return 0
	fi
	[[ "$OF_PLATFORM" == "linux" ]] && setupDesc="distro deps + libs + Project Generator"

	while true; do
		printf '\n'
		printBanner "menu"
		printf '\n'
		echoKV "host" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
		echoKV "of dir" "$OF_DIR"
		echoKV "libs" "${LIB_SOURCE} @ ${LIB_TAG}"
		[[ -n "$OF_LINUX_DISTRO" ]] && echoKV "distro" "$OF_LINUX_DISTRO"
		printf '\n'

		# short labels on purpose — long lines wrap/truncate differently across
		# terminals and can desync gum's highlighted row from the real selection
		if ! menuPick "What do you want to do?" \
			"status()   — checker  (cmdStatus)|status" \
			"setup()    — install  (cmdSetup)|setup" \
			"update()   — refresh  (menuUpdate)|update" \
			"draw()     — build  (menuBuild)|build" \
			"cleanup()  — free space  (menuCleanup)|cleanup" \
			"version()  — info  (menuVersion)|version" \
			"test()     — smoke tests  (menuTest)|test" \
			"exit()|exit"
		then
			echoInfo "bye"
			return 0
		fi
		choice="$UI_MENU_RESULT"
		printf '\n'
		case "$choice" in
			status)     cmdStatus; menuPause ;;
			setup)      cmdSetup "$OF_PLATFORM"; menuPause ;;
			update)     menuUpdate; [[ $? -eq 2 ]] || menuPause ;;
			build)      menuBuild ;;
			cleanup|clean) menuCleanup ;;
			version)    menuVersion; menuPause ;;
			test)       menuTest; [[ $? -eq 2 ]] || menuPause ;;
			exit|quit)  echoSuccess "bye"; return 0 ;;
			*)          echoError "unknown: $choice"; menuPause ;;
		esac
	done
}

# Update menu: pick what to refresh, always prompt libs source when downloading libs
menuUpdate(){
	local what
	printBanner "update"
	echoInfo "refresh libraries / Project Generator"
	printf '\n'
	assessLibsState "$OF_PLATFORM" "${LIB_SOURCE:-apothecary}" "${LIB_TAG:-latest}"
	echoKV "libs" "${LIBS_STATE} — ${LIBS_STATE_DETAIL}"
	assessPGState
	echoKV "pg" "${PG_STATE} — ${PG_STATE_DETAIL}"
	printf '\n'

	# return 2 for "nothing happened, back/cancelled" so the caller can skip
	# the "press Enter to return" pause, same convention as menuApothecary/menuUpgrade
	menuPick "What to update?" \
		"Libraries (choose source)…|libs" \
		"Project Generator|pg" \
		"Libraries + Project Generator|all" \
		"Back|back" \
		|| return 2
	what="$UI_MENU_RESULT"
	case "$what" in
		libs) menuDownloadLibs "$OF_PLATFORM"; return $? ;;
		pg)   cmdUpdatePG "$OF_PLATFORM" 1 ;;
		all)
			menuDownloadLibs "$OF_PLATFORM" || return $?
			# after libs, refresh PG to latest
			cmdUpdatePG "$OF_PLATFORM" 1
			;;
		back) return 2 ;;
	esac
}

# of build <subcmd> …
cmdBuild(){
	local sub="${1:-}"
	shift || true
	case "$sub" in
		""|menu) menuBuild ;;
		help|-h|--help) printHelpBuild ;;
		core|lib|library)
			cmdBuildCore "${1:-Release}"
			;;
		project|app|proj)
			local path="${1:-}" system="make" config="Release"
			[[ -n "$path" ]] || { echoError "usage: of build project <path> [system] [Debug|Release]"; return 1; }
			shift || true
			# optional system then config, or config alone
			if [[ "${1:-}" =~ ^(make|msbuild|xcode|xcodebuild|emscripten|em|wasm|cmake|generate|pg|host)$ ]]; then
				system="$1"; shift || true
			fi
			[[ -n "${1:-}" ]] && config="$1"
			cmdBuildProject "$path" "$system" "$config" 0
			;;
		example|examples)
			local path="${1:-}" system="make" config="Release"
			[[ -n "$path" ]] || { echoError "usage: of build example <path> [system] [cfg]"; return 1; }
			shift || true
			if [[ "${1:-}" =~ ^(make|msbuild|xcode|xcodebuild|emscripten|em|wasm|cmake|generate|pg|host)$ ]]; then
				system="$1"; shift || true
			fi
			[[ -n "${1:-}" ]] && config="$1"
			# allow short example paths
			if [[ ! -d "$path" && -d "${OF_DIR}/examples/$path" ]]; then
				path="${OF_DIR}/examples/$path"
			fi
			cmdBuildProject "$path" "$system" "$config" 0
			;;
		emscripten|em|wasm)
			local path="${1:-}" config="${2:-Debug}"
			[[ -n "$path" ]] || { echoError "usage: of build emscripten <path> [Debug|Release]"; return 1; }
			cmdBuildProject "$path" emscripten "$config" 0
			;;
		open-em|emrun|open-emscripten)
			local path="${1:-}"
			[[ -n "$path" ]] || { echoError "usage: of build open-em <path>"; return 1; }
			path=$(resolveProjectPath "$path") || { echoError "not found: $1"; return 1; }
			ensureEmscriptenEnv || true
			runEmscriptenOpen "$path"
			;;
		generate|pg)
			local path="${1:-}" platforms="${2:-}"
			[[ -n "$path" ]] || { echoError "usage: of build generate <path> [platforms]"; return 1; }
			cmdBuildGenerate "$path" "$platforms"
			;;
		cmake)
			case "${1:-}" in
				""|status|info) runCmakeStatus ;;
				android) runCmakeAndroidCore "${2:-Debug}" ;;
				*)
					local path="$1" config="${2:-Release}"
					path=$(resolveProjectPath "$path") || { echoError "not found: $1"; return 1; }
					runCmakeProject "$path" "$config"
					;;
			esac
			;;
		clean)
			local path="${1:-}"
			[[ -n "$path" ]] || { echoError "usage: of build clean <path>"; return 1; }
			path=$(resolveProjectPath "$path") || { echoError "not found: $1"; return 1; }
			runCleanProject "$path"
			;;
		*)
			# bare path → treat as project with make
			if [[ -n "$sub" ]] && { [[ -d "$sub" ]] || [[ -d "${OF_DIR}/$sub" ]] || [[ -d "${OF_DIR}/examples/$sub" ]] || [[ -d "${OF_DIR}/apps/$sub" ]]; }; then
				cmdBuildProject "$sub" "${1:-make}" "${2:-Release}" 0
				return $?
			fi
			echoError "unknown build subcommand: ${sub:-}"
			printHelpBuild
			return 1
			;;
	esac
}

runCommand(){
	local cmd=$1 subcmd=$2 subcmd2=$3 subcmd3=$4
	case "$cmd" in
		help|-h|--help) printHelp ;;
		menu) cmdMenu ;;
		status|check|doctor|installed|libs-status) cmdStatus ;;
		setup|install) cmdSetup "${subcmd:-$OF_PLATFORM}" ;;
		update) cmdUpdate "$subcmd" "$subcmd2" ;;
		build)
			shift
			cmdBuild "$@"
			;;
		version)
			case "$subcmd" in
				""|of) cmdVersion ;;
				pg|projectgenerator) cmdVersionPG ;;
				*) echoError "valid: of | pg"; return 1 ;;
			esac
			;;
		upgrade) cmdUpgrade "$subcmd" ;;
		test)
			case "${subcmd:-}" in
				""|menu) menuTest ;;
				*) cmdTest "$subcmd" ;;
			esac
			;;
		cleanup|clean)
			case "${subcmd:-}" in
				""|menu) menuCleanup ;;
				projects|project)
					case "${subcmd2:-all}" in
						apps|examples|all) cleanProjectsScope "${subcmd2:-all}" ;;
						*) cleanProjectsScope path "${subcmd2}" ;;
					esac
					;;
				caches|cache)
					cleanDownloadCaches "${subcmd2:-packages}"
					;;
				libs|libraries)
					case "${subcmd2:-other}" in
						list) cleanLibraries list ;;
						other|other-platforms|host) cleanLibraries other-platforms ;;
						all|nuclear) cleanLibraries all-prebuilt ;;
						*) cleanLibraries platform "${subcmd2}" ;;
					esac
					;;
				*)
					echoError "usage: of cleanup [projects|caches|libs] …"
					echoNote "  of cleanup projects [all|apps|examples|<path>]"
					echoNote "  of cleanup caches [packages|all]"
					echoNote "  of cleanup libs [other|list|all|vs|macos|…]"
					return 1
					;;
			esac
			;;
		apothecary|apo)
			case "$subcmd" in
				""|menu) menuApothecary ;;
				build|core) cmdApothecaryBuildAll "${subcmd2:-$OF_PLATFORM}" "${subcmd3:-$OF_ARCH}" ;;
				open) launchApoMenu ;;
				*) [[ -n "$subcmd" ]] && runApothecaryEngine "${subcmd2:-$OF_PLATFORM}" "${subcmd3:-$OF_ARCH}" update "$subcmd" || menuApothecary ;;
			esac
			;;
		*)
			echoError "Unknown command: $cmd"
			echoNote "valid: menu status setup update build cleanup version upgrade apothecary help"
			printHelp
			return 1
			;;
	esac
}

if [[ $# -eq 0 ]]; then
	if menuCanRun; then
		cmdMenu
		exit $?
	fi
	cmdStatus
	exit 0
fi

runCommand "$@"
exit $?
