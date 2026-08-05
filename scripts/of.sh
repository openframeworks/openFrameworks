#!/usr/bin/env bash
# of.sh - openFrameworks CLI  |  Dan Rosser 2025
OF_SCRIPT_VERSION=0.4.0

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

autoDetectOS(){
	if [[ -z "$PLATFORM" ]]; then
		export OF_OS OF_PLATFORM OF_ARCH
		OF_OS=$(uname -s | tr '[:upper:]' '[:lower:]')
		case "$OF_OS" in
			darwin|dawin) OF_PLATFORM="osx"; OF_ARCH=$(uname -m) ;;
			linux) OF_PLATFORM="linux"; OF_ARCH=$(uname -m) ;;
			mingw*|cygwin*|msys*) OF_PLATFORM="vs"; OF_ARCH=${MSYSTEM,,} ;;
			*) echoError "Unsupported platform: $OF_OS"; exit 1 ;;
		esac
	else
		export OF_OS OF_PLATFORM OF_ARCH
		OF_OS=$(echo "${PLATFORM}" | tr '[:upper:]' '[:lower:]')
		OF_PLATFORM="$PLATFORM"
		OF_ARCH=""
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

findPGBinary(){
	local p
	for p in \
		"${OF_DIR}/projectGenerator/projectGenerator" \
		"${OF_PG_INSTALLED_DIR}/projectGenerator" \
		"${OF_DIR}/projectGenerator/projectGenerator.app/Contents/MacOS/projectGenerator"
	do
		[[ -x "$p" || -f "$p" ]] && { printf '%s' "$p"; return 0; }
	done
	return 1
}

readPGVersion(){
	local bin ver
	bin=$(findPGBinary) || return 1
	ver=$("$bin" --version 2>/dev/null | head -1 | tr -d '\r')
	[[ -z "$ver" ]] && ver=$("$bin" -v 2>/dev/null | head -1 | tr -d '\r')
	if [[ "$ver" == *projectGenerator* ]]; then
		ver=$(printf '%s' "$ver" | sed -E 's/.*"openFrameworks projectGenerator": *"([^"]+)".*/\1/')
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

# Inline list: libpng (1.6.58), glfw (3.4), assimp (5.4.3 · ofxAssimpModelLoader), …
# Sets FORMATTED_LIB_LINE and FORMATTED_LIB_COUNT
formatLibInlineList(){
	local -a items=("$@")
	local path name note entry line="" n=0

	FORMATTED_LIB_LINE=""
	FORMATTED_LIB_COUNT=0

	for path in "${items[@]}"; do
		note=""
		if [[ "$path" == *"|"* ]]; then
			note="${path#*|}"
			path="${path%%|*}"
		fi
		[[ -d "$path" ]] || continue
		name=$(basename "$path")
		if readLibVersionMeta "$path"; then
			if [[ -n "$note" ]]; then
				entry="${name} (${LIB_META_VER} · ${note})"
			else
				entry="${name} (${LIB_META_VER})"
			fi
		else
			if [[ -n "$note" ]]; then
				entry="${name} (? · ${note})"
			else
				entry="${name} (?)"
			fi
		fi
		[[ -n "$line" ]] && line+=", "
		line+="$entry"
		n=$((n + 1))
	done

	FORMATTED_LIB_LINE="$line"
	FORMATTED_LIB_COUNT=$n
}

printLibInlineSection(){
	local title="$1"
	shift
	printf '\n'
	printf '  %s%s%s  %s────────────────%s\n' "$C_ACCENT" "$title" "$C_RESET" "$C_MUTED" "$C_RESET"
	if [[ $# -eq 0 ]]; then
		printf '  %s—%s\n' "$C_MUTED" "$C_RESET"
		return 0
	fi
	formatLibInlineList "$@"
	if [[ "$FORMATTED_LIB_COUNT" -eq 0 ]]; then
		printf '  %s—%s\n' "$C_MUTED" "$C_RESET"
		return 0
	fi
	printf '  %s\n' "$FORMATTED_LIB_LINE"
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

	# core libs under libs/
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
				_seenAdd "libs/$name"
			fi
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

	# others under libs/
	if [[ -d "${OF_DIR}/libs" ]]; then
		for path in "${OF_DIR}/libs"/*/; do
			[[ -d "$path" ]] || continue
			name=$(basename "$path")
			case "$name" in
				download|openFrameworks|openFrameworksCompiled|scripts) continue ;;
			esac
			[[ -d "${path}include" || -d "${path}lib" ]] || continue
			_seenHas "libs/$name" && continue
			if isCoreLibName "$name"; then
				continue
			fi
			if isCoreAddonLibName "$name"; then
				addon=$(coreAddonForLib "$name" 2>/dev/null || echo "addon")
				SEC_CORE+=("${path}|${addon}")
			else
				SEC_OTHER+=("$path")
			fi
			_seenAdd "libs/$name"
		done
	fi
}

cmdStatus(){
	local ofVer pgBin pgVer lastUp hostLine
	local issues=0
	local -a issueNotes=()
	local total=0

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
	collectLibSections
	total=$(( ${#SEC_CORE[@]} + ${#SEC_ADDONS[@]} + ${#SEC_OTHER[@]} ))
	assessLibsState "$OF_PLATFORM"
	# only flag libs if none detected
	if [[ "$LIBS_STATE" == "missing" || "$total" -eq 0 ]]; then
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

	printLibInlineSection "Core" "${SEC_CORE[@]}"
	printLibInlineSection "Addons" "${SEC_ADDONS[@]}"
	printLibInlineSection "Other" "${SEC_OTHER[@]}"

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
    version   of  | pg        Version info
    upgrade   addons | apps   Upgrade tree
    installed                 Alias for status
    apothecary                Build libraries via apothecary submodule

  Env
    LIB_SOURCE=apothecary|oflibs|archive
    LIB_TAG=latest|v12.1.0|v0.11.2
    LIB_LIBS=core|all|"glfw glm"
    OF_LINUX_DISTRO=ubuntu    Force linux distro scripts
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

OF_LIB_STATE_FILE="${OF_DIR}/libs/.of-cli-state"

# Essentials used to decide "installed"
OF_LIBS_ESSENTIAL=(freetype glew glfw zlib tess2 uriparser utf8 json)

writeLibState(){
	local source="${1:-$LIB_SOURCE}"
	local tag="${2:-$LIB_TAG}"
	local platformDir="${3:-$OF_PLATFORM}"
	mkdir -p "${OF_DIR}/libs"
	cat > "$OF_LIB_STATE_FILE" << EOF
source=${source}
tag=${tag}
platform=${platformDir}
arch=${OF_ARCH:-}
updated=$(date -u +%Y-%m-%dT%H:%M:%SZ)
of_cli=${OF_SCRIPT_VERSION}
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
	LIBS_STATE="ok"
	if [[ -n "$stUpdated" ]]; then
		LIBS_STATE_DETAIL="${stSource}@${stTag} · ${stUpdated}"
	else
		LIBS_STATE_DETAIL="detected · assume latest"
	fi
}

# Sets PG_STATE=ok|missing  PG_STATE_DETAIL=
assessPGState(){
	local bin ver
	PG_STATE="ok"
	PG_STATE_DETAIL=""
	bin=$(findPGBinary) || bin=""
	if [[ -z "$bin" ]]; then
		PG_STATE="missing"
		PG_STATE_DETAIL="Project Generator not installed"
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

menuPickPlatformForLibs(){
	local -a opts=()
	local p script label
	for p in "${OF_LIB_PLATFORMS[@]}"; do
		script="${OF_CORE_SCRIPT_DIR}/${p}/download_libs.sh"
		if [[ "$LIB_SOURCE" != "apothecary" ]] || [[ -f "$script" ]]; then
			label="$p"
			if [[ "$LIB_SOURCE" == "apothecary" ]]; then
				case "$p" in
					macos) label="macos  — Apple multi-target (osx · ios · tvos · xros · watchos · catos)" ;;
					osx)
						label="osx  — desktop host package"
						[[ "$p" == "$OF_PLATFORM" ]] && label+="  (this machine)"
						;;
					*) [[ "$p" == "$OF_PLATFORM" ]] && label="${p}  (this machine)" ;;
				esac
			else
				[[ "$p" == "$OF_PLATFORM" ]] && label="${p}  (this machine)"
			fi
			opts+=("${label}|${p}")
		fi
	done
	[[ "$LIB_SOURCE" == "apothecary" && "$OF_PLATFORM" == "osx" ]] && \
		echoNote "macos packages install lib/macos/*.xcframework covering all Apple targets"
	menuPick "Target platform" "${opts[@]}" || return 1
}

menuDownloadLibs(){
	local platformDir="${1:-$OF_PLATFORM}"
	printBanner "libs"
	echoInfo "download libraries · choose source"
	printf '\n'

	menuPickLibSource || return 1
	echoSuccess "source → $LIB_SOURCE"
	menuPickLibTag "$LIB_SOURCE" || return 1
	echoSuccess "tag → $LIB_TAG"

	if [[ "$platformDir" == "$OF_PLATFORM" ]]; then
		if menuCanRun && confirmNo "Download for another platform instead of ${platformDir}?"; then
			menuPickPlatformForLibs || return 1
			platformDir="$UI_MENU_RESULT"
		fi
	fi

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
	fi
	[[ "$LIB_SOURCE" == "oflibs" ]] && { menuPickOfLibs "$LIB_TAG" "$platformDir" || return 1; }

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
		vs) echo "64 arm64" ;;
		msys2) echo "64" ;;
		*) echo "${OF_ARCH:-64}" ;;
	esac
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
	( cd "${APO_HOME}/apothecary" 2>/dev/null || cd "$APO_HOME" || exit 1; "${cmd[@]}" )
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
	menuPick "Apothecary builds" "${opts[@]}" || return 0
	choice="$UI_MENU_RESULT"

	case "$choice" in
		build-host) cmdApothecaryBuildAll "$OF_PLATFORM" "$OF_ARCH" ;;
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
			menuPick "Build platform" "${opts[@]}" || return 0
			type="$UI_MENU_RESULT"
			aopts=()
			read -r -a arches <<< "$(archesForApoType "$type")"
			for a in "${arches[@]}"; do
				aopts+=("${a}|${a}")
			done
			menuPick "Architecture · ${type}" "${aopts[@]}" || return 0
			cmdApothecaryBuildAll "$type" "$UI_MENU_RESULT"
			;;
		build-one)
			opts=()
			for type in "${APO_BUILD_TYPES[@]}"; do
				opts+=("${type}|${type}")
			done
			menuPick "Platform" "${opts[@]}" || return 0
			type="$UI_MENU_RESULT"
			aopts=()
			read -r -a arches <<< "$(archesForApoType "$type")"
			for a in "${arches[@]}"; do aopts+=("${a}|${a}"); done
			menuPick "Architecture · ${type}" "${aopts[@]}" || return 0
			arch="$UI_MENU_RESULT"
			opts=()
			local f
			for f in "${APO_HOME}/apothecary/formulas"/*; do
				name=$(basename "$f")
				[[ "$name" == _* ]] && continue
				if [[ -d "$f" ]]; then opts+=("${name}|${name}")
				elif [[ "$name" == *.sh ]]; then opts+=("${name%.sh}|${name%.sh}"); fi
			done
			menuPick "Formula · ${type}/${arch}" "${opts[@]}" || return 0
			name="$UI_MENU_RESULT"
			confirmYes "Build ${name} for ${type}/${arch}?" || return 0
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
		back) return 0 ;;
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
	fi

	if [[ "$needLibs" -eq 0 && "$needPG" -eq 0 && "$OF_PLATFORM" != "linux" ]]; then
		echoSuccess "already set up — libs + PG look current"
		echoNote "use Update to redownload from a chosen source"
		printf '\n'
		return 0
	fi

	taskNames+=("Detect platform")
	[[ "$OF_PLATFORM" == "linux" ]] && taskNames+=("Install dependencies (${OF_LINUX_DISTRO})")
	[[ "$doCodecs" -eq 1 ]] && taskNames+=("Install codecs (${OF_LINUX_DISTRO})")
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

cmdMenu(){
	local choice setupLabel="Setup"
	if ! menuCanRun; then
		echoWarning "no TTY — showing status"
		cmdStatus
		return 0
	fi
	[[ "$OF_PLATFORM" == "linux" ]] && setupLabel="Setup (distro deps + libs + PG)"

	while true; do
		printf '\n'
		printBanner "menu"
		printf '\n'
		echoKV "host" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
		echoKV "of dir" "$OF_DIR"
		echoKV "libs" "${LIB_SOURCE} @ ${LIB_TAG}"
		[[ -n "$OF_LINUX_DISTRO" ]] && echoKV "distro" "$OF_LINUX_DISTRO"
		printf '\n'

		if ! menuPick "What do you want to do?" \
			"Status  — system checker|status" \
			"${setupLabel}|setup" \
			"Update  — libs / PG (choose source)|update" \
			"Build…  — core / projects / examples / emscripten / cmake|build" \
			"Build libraries (Apothecary)…|apothecary" \
			"Show openFrameworks version|version" \
			"Show Project Generator version|version-pg" \
			"Upgrade addons|upgrade-addons" \
			"Upgrade apps|upgrade-apps" \
			"Help|help" \
			"Quit|quit"
		then
			echoInfo "bye"
			return 0
		fi
		choice="$UI_MENU_RESULT"
		printf '\n'
		case "$choice" in
			status)         cmdStatus; menuPause ;;
			setup)          cmdSetup "$OF_PLATFORM"; menuPause ;;
			update)         menuUpdate; menuPause ;;
			build)          menuBuild ;;
			apothecary)     menuApothecary; menuPause ;;
			version)        cmdVersion; menuPause ;;
			version-pg)     cmdVersionPG; menuPause ;;
			upgrade-addons) cmdUpgrade addons; menuPause ;;
			upgrade-apps)   cmdUpgrade apps; menuPause ;;
			help)           printHelp; menuPause ;;
			quit)           echoSuccess "bye"; return 0 ;;
			*)              echoError "unknown: $choice"; menuPause ;;
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

	menuPick "What to update?" \
		"Libraries (choose source)…|libs" \
		"Project Generator|pg" \
		"Libraries + Project Generator|all" \
		"Back|back" \
		|| return 0
	what="$UI_MENU_RESULT"
	case "$what" in
		libs) menuDownloadLibs "$OF_PLATFORM" ;;
		pg)   cmdUpdatePG "$OF_PLATFORM" 1 ;;
		all)
			menuDownloadLibs "$OF_PLATFORM" || return $?
			# after libs, refresh PG to latest
			cmdUpdatePG "$OF_PLATFORM" 1
			;;
		back) return 0 ;;
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
			if [[ "${1:-}" =~ ^(make|xcode|xcodebuild|emscripten|em|wasm|cmake|generate|pg|host)$ ]]; then
				system="$1"; shift || true
			fi
			[[ -n "${1:-}" ]] && config="$1"
			cmdBuildProject "$path" "$system" "$config" 0
			;;
		example|examples)
			local path="${1:-}" system="make" config="Release"
			[[ -n "$path" ]] || { echoError "usage: of build example <path> [system] [cfg]"; return 1; }
			shift || true
			if [[ "${1:-}" =~ ^(make|xcode|xcodebuild|emscripten|em|wasm|cmake|generate|pg|host)$ ]]; then
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
			echoNote "valid: menu status setup update build version upgrade apothecary help"
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
