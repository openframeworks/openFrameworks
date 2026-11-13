#!/usr/bin/env bash
# of.sh - openFrameworks CLI  |  Dan Rosser 2025
OF_SCRIPT_VERSION=0.7.0

# --json / --api: machine-readable mode. Exactly one JSON document goes to the
# original stdout (fd 8); the usual human text goes to stderr; colours, spinners
# and prompts are off. Only some commands support it (see ofJsonDispatch).
OF_JSON=0
OF_LANG="${OF_LANG:-}"
_ofArgs=()
_ofNextLang=0
for _a in "$@"; do
	if [[ $_ofNextLang -eq 1 ]]; then OF_LANG="$_a"; _ofNextLang=0; continue; fi
	case "$_a" in
		--json|--api) OF_JSON=1 ;;
		--lang) _ofNextLang=1 ;;
		--lang=*) OF_LANG="${_a#--lang=}" ;;
		*) _ofArgs+=("$_a") ;;
	esac
done
if [[ "$OF_JSON" == "1" || "${#_ofArgs[@]}" -ne "$#" ]]; then
	set -- ${_ofArgs[@]+"${_ofArgs[@]}"}
fi
if [[ "$OF_JSON" == "1" ]]; then
	export NO_COLOR=1
	OF_ANIM=0
	OF_NONINTERACTIVE=1
	exec 8>&1 1>&2
fi
unset _a _ofArgs _ofNextLang

OF_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OF_DIR="$(realpath "$OF_DIR/../")"
OF_CORE_SCRIPT_DIR="$(realpath "$OF_DIR/scripts")"
if [[ -d "$OF_DIR/scripts/ci" ]]; then
	OF_CORE_CI_SCRIPT_DIR="$(realpath "$OF_DIR/scripts/ci")"
else
	OF_CORE_CI_SCRIPT_DIR="${OF_CORE_SCRIPT_DIR}/ci"
fi
if [[ -d "$OF_DIR/projectGenerator" ]]; then
	OF_PG_INSTALLED_DIR="$(realpath "$OF_DIR/projectGenerator")"
else
	OF_PG_INSTALLED_DIR="$OF_DIR/projectGenerator"
fi

UI_APP_NAME="openFrameworks"
UI_APP_VERSION="$OF_SCRIPT_VERSION"
UI_ANIM="${OF_ANIM:-1}"
OF_GIT_REMOTE="${OF_GIT_REMOTE:-https://github.com/openframeworks/openFrameworks.git}"
OF_GIT_BRANCH="${OF_GIT_BRANCH:-master}"
. "$OF_CORE_SCRIPT_DIR/ui.sh"
OF_I18N_DIR="${OF_I18N_DIR:-$OF_CORE_SCRIPT_DIR/i18n}"
if [[ -n "$OF_LANG" && "$OF_LANG" != "en" ]] && ! i18nLoad "$OF_LANG"; then
	echoWarning "$(t msg.language_not_found_using_english "language '%s' not found in %s — using English" "$OF_LANG" "$OF_I18N_DIR")"
	OF_LANG=""
fi
if [[ -f "$OF_CORE_SCRIPT_DIR/dev/lib_sources.sh" ]]; then
	. "$OF_CORE_SCRIPT_DIR/dev/lib_sources.sh"
else
	echoWarning "$(t msg.missing_scripts_dev_lib_sources_sh "missing scripts/dev/lib_sources.sh — of update libs needs it (of clone restores a full checkout)")"
fi
if [[ -f "$OF_CORE_SCRIPT_DIR/of_build.sh" ]]; then
	. "$OF_CORE_SCRIPT_DIR/of_build.sh"
fi

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
	if hostIsArm64; then echo "arm64"; else echo "64"; fi
}

# Native Windows CPU, also from emulated x64 shells: Git Bash and MSYS2's own
# bash are x64 on Windows on ARM and report PROCESSOR_ARCHITECTURE=AMD64 /
# uname -m x86_64, so the registry (not emulated) is checked first
hostIsArm64(){
	local reg="/proc/registry/HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Control/Session Manager/Environment/PROCESSOR_ARCHITECTURE"
	local pa
	[[ -r "$reg" && "$(tr -d '\0\r\n ' < "$reg")" == "ARM64" ]] && return 0
	case "${MSYSTEM:-}" in
		CLANGARM64|clangarm64) return 0 ;;
	esac
	pa="${PROCESSOR_ARCHITEW6432:-${PROCESSOR_ARCHITECTURE:-}}"
	[[ "$(echo "$pa" | tr '[:lower:]' '[:upper:]')" == "ARM64" ]] && return 0
	case "$(uname -s 2>/dev/null)" in
		*ARM64*) return 0 ;;
	esac
	case "$(uname -m 2>/dev/null)" in
		aarch64|arm64|ARM64) return 0 ;;
	esac
	return 1
}

# MSYS2 toolchain environments that have openFrameworks lib packages
OF_MSYS2_ENVS=(ucrt64 clang64 clangarm64 mingw64)

# Git for Windows ships git-bash.exe at its root; MSYS2 doesn't (#8640).
# Git Bash also sets MSYSTEM=MINGW64, so MSYSTEM alone can't tell them apart.
isGitBash(){
	[[ -f /git-bash.exe || -f /git-cmd.exe ]]
}

isMsys2Shell(){
	isGitBash && return 1
	[[ -x /usr/bin/pacman ]] && return 0
	case "$(echo "${MSYSTEM:-}" | tr '[:upper:]' '[:lower:]')" in
		ucrt64|clang64|clangarm64|mingw64) return 0 ;;
	esac
	return 1
}

# MSYS2 env for libs: the shell's own MSYSTEM when it is one of OF_MSYS2_ENVS
# (and this is really MSYS2), else by CPU — clangarm64 on ARM64, ucrt64 on x64
detectMsys2Env(){
	local env
	env=$(echo "${MSYSTEM:-}" | tr '[:upper:]' '[:lower:]')
	if ! isGitBash; then
		case "$env" in
			ucrt64|clang64|clangarm64|mingw64) echo "$env"; return 0 ;;
		esac
	fi
	if hostIsArm64; then
		echo "clangarm64"
	else
		echo "ucrt64"
	fi
}

autoDetectOS(){
	if [[ -z "$PLATFORM" ]]; then
		export OF_OS OF_PLATFORM OF_ARCH
		OF_OS=$(uname -s | tr '[:upper:]' '[:lower:]')
		case "$OF_OS" in
			darwin|dawin) OF_PLATFORM="osx"; OF_ARCH=$(uname -m) ;;
			linux) OF_PLATFORM="linux"; OF_ARCH=$(uname -m) ;;
			mingw*|cygwin*|msys*)
				export OF_MSYSTEM="${MSYSTEM:-}"
				if isMsys2Shell; then
					# MSYS2 shell (UCRT64 / CLANG64 / CLANGARM64 / MINGW64) → msys2 libs
					OF_PLATFORM="msys2"
					OF_ARCH=$(detectMsys2Env)
				else
					# Git Bash / other Windows shells → Visual Studio; arch is CPU
					OF_PLATFORM="vs"
					OF_ARCH=$(detectHostVsArch)
				fi
				;;
			*) echoError "$(t msg.unsupported_platform "Unsupported platform: %s" "$OF_OS")"; exit 1 ;;
		esac
	else
		export OF_OS OF_PLATFORM OF_ARCH
		OF_OS=$(echo "${PLATFORM}" | tr '[:upper:]' '[:lower:]')
		OF_PLATFORM="$PLATFORM"
		OF_ARCH=""
		if [[ "$OF_PLATFORM" == "vs" ]]; then
			OF_ARCH=$(detectHostVsArch)
		elif [[ "$OF_PLATFORM" == "msys2" ]]; then
			OF_ARCH=$(detectMsys2Env)
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
	[[ ${#opts[@]} -eq 0 ]] && { echoError "$(t msg.no_linux_distro_install_scripts "no linux distro install scripts")"; return 1; }
	if menuCanRun; then
		menuPick "$(t menu.select_linux_distro_install_scripts "Select Linux distro install scripts")" "${opts[@]}" || return 1
		OF_LINUX_DISTRO="$UI_MENU_RESULT"
	else
		echoError "$(t msg.set_linux_distro_ubuntu_debian_fedora "set OF_LINUX_DISTRO=ubuntu|debian|fedora|archlinux")"
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
		echoError "$(t msg.root_sudo_required "root or sudo required")"
		return 1
	fi
}

runDistroInstallScript(){
	local script="$1"
	shift
	[[ -f "$script" ]] || { echoError "$(t msg.missing "missing %s" "$script")"; return 1; }
	[[ -x "$script" ]] || chmod +x "$script" 2>/dev/null || true
	if [[ "$(id -u)" -eq 0 ]]; then
		"$script" -y "$@"
	else
		echoNote "$(t msg.sudo "sudo %s" "$(basename "$script")")"
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
		*) echoError "$(t msg.unsupported_platform "Unsupported platform: %s" "$OF_PLATFORM")"; exit 1 ;;
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
	[[ -e "$script" ]] || { echoError "$(t msg.not_found "Not found: %s" "$script")"; return 1; }
	[[ -x "$script" ]] || chmod +x "$script" 2>/dev/null || true
	[[ -x "$script" ]] || { echoError "$(t msg.not_executable "Not executable: %s" "$script")"; return 1; }
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
	echoKV "$(t msg.platform "platform")" "$hostLine"
	if [[ -n "$pgBin" ]]; then
		echoKV "$(t msg.project_gen "project gen")" "$pgVer"
	else
		echoKV "$(t msg.project_gen "project gen")" "not installed"
	fi
	echoKV "$(t msg.last_update "last update")" "$lastUp"
	echoKV "$(t msg.cli "cli")" "$OF_SCRIPT_VERSION"
	if ofHasGit; then
		echoKV "$(t msg.git "git")" "$(ofGitSummary)"
	else
		echoKV "$(t msg.git "git")" "$(ofNoGitLabel)"
		echoNote "$(t msg.clone_attach_openframeworks_git "of clone  — attach openFrameworks.git")"
	fi
	local stSha stShaN
	stSha=$(readLibStateField sha_status) || stSha=$(readVerifyField status) || stSha=""
	stShaN=$(readLibStateField sha_verified) || stShaN=$(readVerifyField verified) || stShaN=""
	case "$stSha" in
		verified)
			if [[ -n "$stShaN" && "$stShaN" != "0" ]]; then
				echoKV "$(t msg.integrity "integrity")" "SHA-256 verified (${stShaN} package(s)) · secure"
			else
				echoKV "$(t msg.integrity "integrity")" "SHA-256 verified · secure"
			fi
			;;
		failed) echoKV "$(t msg.integrity "integrity")" "SHA-256 failed" ;;
		no-digest|no-remote) echoKV "$(t msg.integrity "integrity")" "SHA not available for last download" ;;
		no-tools) echoKV "$(t msg.integrity "integrity")" "no local sha256 tool" ;;
		skipped) echoKV "$(t msg.integrity "integrity")" "SHA check skipped" ;;
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
		echoSuccess "$(t msg.ready "ready")"
		printf '\n'
		return 0
	fi

	echoWarning "$(t msg.setup_needed "setup needed")"
	local n
	for n in "${issueNotes[@]}"; do
		echoNote "$n"
	done
	printf '\n'

	if menuCanRun; then
		if confirmYes "$(t msg.would_you_like_run_setup "Would you like to run setup?")"; then
			printf '\n'
			cmdSetup "$OF_PLATFORM"
			return $?
		fi
		echoInfo "$(t msg.ok_run_setup_when_ready "ok — run of setup when ready")"
	else
		echoNote "$(t msg.run_setup "run: of setup")"
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
    vsconfig  [check|latest|minimal|no-update] [2019|2022|2026]  VS components (scripts/vs/<year>.vsconfig); installs a missing VS
    update    libs | pg | all Prompt source; refresh libs/PG as needed
    build     …               Build core / projects / emscripten / cmake (see of build help)
    cleanup   projects|caches|libs|core   Free disk (artifacts / downloads / prebuilts)  [--dry-run] [--yes]
    i18n      list|export|check|import   The string dictionary (translations): scripts/i18n
    --lang xx (or OF_LANG=xx) Use scripts/i18n/xx.tsv for menu text and messages
    --json  (or --api)        Machine-readable output: one JSON document on stdout, human text on
                              stderr, never prompts. For: status, version, menu, cleanup …
    version   of  | pg        Version info
    upgrade   addons | apps   Upgrade tree
    clone                     Nightly zip → git clone of openFrameworks.git (when no .git)
    test      [group]         Run tests/ smoke tests (build + run), or menu for bash smoke scripts too
    installed                 Alias for status
    apothecary                Build libraries via apothecary submodule

  Env
    LIB_SOURCE=apothecary|oflibs|archive
    LIB_TAG=latest|v12.1.0|v0.11.2
    LIB_LIBS=core|all|"glfw glm"
    LIB_ARCH=64|arm64|arm64ec|all   passed as -a to download_libs (vs/linux)
    LIB_ARCH=ucrt64|clang64|clangarm64|mingw64   msys2 env (default: detected MSYSTEM)
    LIB_CLEAN_MODE=platform|merge|full   # default platform: only lib/<plat>
    DOWNLOAD_LIBS_ARGS="-vs --full-clean"  extra flags to download_libs.sh
    OF_VS_LIBS=2026|2022|2019 Visual Studio to download libs for (remembered; replaces another VS's libs)
    OF_VS_2026=1              VS 2026 package names (_vs_2026_)
    OF_SETUP_VS=1|0           setup: run / skip the VS Installer (.vsconfig) without asking
    OF_LINUX_DISTRO=ubuntu    Force linux distro scripts
    OF_APO_VS_VER=18          apothecary VS toolchain (18=VS2026, 17=VS2022, 16=VS2019)
    OF_JOBS=8                 Parallel build jobs
    VERBOSE=1  NO_COLOR=1  OF_ANIM=0

  Examples
    ${prog}
    ${prog} status
    ${prog} update libs
    ${prog} update libs vs arm64
    LIB_ARCH=arm64 ${prog} update libs vs
    LIB_SOURCE=oflibs LIB_LIBS=core ${prog} update libs
    ${prog} build core Debug
    ${prog} build project apps/myApps/mySketch
    ${prog} build emscripten examples/graphics/graphicsExample
    ${prog} cleanup projects
    ${prog} cleanup caches
    ${prog} cleanup libs other
    ${prog} clone
    ${prog} apothecary build

EOF
	echoKV "$(t msg.platform "platform")" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
	echoKV "$(t msg.libs_source "libs source")" "${LIB_SOURCE} / ${LIB_TAG}"
	echoNote "$(t msg.build_details_build_help "build details: %s build help" "${prog}")"
	printf '\n'
}

cmdVersion(){
	local ofVer
	printBanner "version"
	ofVer=$(readOfVersion) || ofVer=""
	printf '\n'
	echoKV "$(t msg.openframeworks "openFrameworks")" "${ofVer:-—}"
	echoKV "$(t msg.platform "platform")" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
	echoKV "$(t msg.cli "cli")" "$OF_SCRIPT_VERSION"
	if ofHasGit; then
		echoKV "$(t msg.git "git")" "$(ofGitSummary)"
	else
		echoKV "$(t msg.git "git")" "$(ofNoGitLabel)"
	fi
	printf '\n'
}

cmdVersionPG(){
	local bin ver
	printBanner "version"
	bin=$(findPGBinary) || bin=""
	printf '\n'
	if [[ -n "$bin" ]]; then
		ver=$(readPGVersion)
		echoKV "$(t msg.projectgenerator "projectGenerator")" "$ver"
		echoKV "$(t msg.path "path")" "$bin"
	else
		echoKV "$(t msg.projectgenerator "projectGenerator")" "not installed"
		echoNote "$(t msg.try_update_pg "try: of update pg")"
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
	echoKV "$(t msg.openframeworks "openFrameworks")" "${ofVer:-—}"
	echoKV "$(t msg.platform "platform")" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
	echoKV "$(t msg.cli "cli")" "$OF_SCRIPT_VERSION"
	if ofHasGit; then
		echoKV "$(t msg.git "git")" "$(ofGitSummary)"
	else
		echoKV "$(t msg.git "git")" "$(ofNoGitLabel)"
	fi
	printf '\n'
	if [[ -n "$bin" ]]; then
		ver=$(readPGVersion)
		echoKV "$(t msg.projectgenerator "projectGenerator")" "$ver"
		echoKV "$(t msg.pg_path "pg path")" "$bin"
	else
		echoKV "$(t msg.projectgenerator "projectGenerator")" "not installed"
		echoNote "$(t msg.try_update_pg "try: of update pg")"
	fi
	printf '\n'
}

# test() — runs either the tests/*/* smoke test projects (build + run,
# reusing cmdTest from of_build.sh) or one of the standalone smoke_test_*.sh
# bash scripts under scripts/dev/ (e.g. smoke_test_nightly.sh).
menuTest(){
	local choice
	if ! menuCanRun; then
		echoWarning "$(t msg.no_tty_running_all_tests_smoke "no TTY — running all tests/ smoke tests non-interactively")"
		cmdTest
		return $?
	fi
	printBanner "test"

	local -a opts=("$(t menu.all_tests_smoke_tests_build_run "All tests/ smoke tests (build + run)")|tests-all")
	local -a testOpts=()
	local -a groupOpts=()
	local g gName test testName
	if [[ -d "${OF_DIR}/tests" ]]; then
		for g in "${OF_DIR}/tests"/*/; do
			[[ -d "$g" ]] || continue
			gName=$(basename "${g%/}")
			groupOpts+=("All tests/${gName}|tests-${gName}")
			for test in "$g"*/; do
				[[ -d "${test}src" ]] || continue
				testName=$(basename "${test%/}")
				testOpts+=("${gName}/${testName}|test:${gName}/${testName}")
			done
		done
	fi
	# Keep every directly runnable project together at the top of the menu;
	# broader group runs follow them.
	opts+=("${testOpts[@]}")
	opts+=("${groupOpts[@]}")
	local -a smokeScripts=()
	local s
	for s in "${OF_CORE_SCRIPT_DIR}/dev"/smoke_test_*.sh; do
		[[ -f "$s" ]] && smokeScripts+=("$(basename "$s")")
	done
	for s in "${smokeScripts[@]}"; do
		opts+=("Smoke script — ${s}|script:${s}")
	done
	opts+=("$(t menu.back "Back")|back")

	menuPick "$(t menu.run_which_tests "Run which tests?")" "${opts[@]}" || return 2
	choice="$UI_MENU_RESULT"
	case "$choice" in
		tests-all) cmdTest ;;
		tests-*)   cmdTest "${choice#tests-}" ;;
		test:*)    cmdTest "${choice#test:}" ;;
		script:*)
			local scriptName="${choice#script:}"
			local scriptPath="${OF_CORE_SCRIPT_DIR}/dev/${scriptName}"
			local -a args=()
			if menuCanRun && confirmYes "$(t msg.include_real_network_build_checks_real "Include the real network/build checks (--real)?")"; then
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
	# set by runLibsDownload for vs; other platforms must not forget what lib/vs holds
	local vsLibs="${OF_VS_LIBS_INSTALLED-$(readLibStateField vs_libs)}"
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
vs_libs=${vsLibs}
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
	local stVs
	stVs=$(readLibStateField vs_libs) || stVs=""
	[[ -n "$stVs" ]] && LIBS_STATE_DETAIL+=" · VS ${stVs}"
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

# Which Visual Studio to download libs for → VS_LIBS_RESOLVED=2026|2022|2019, and
# VS_LIBS_SWITCH=1 when that replaces libs for another VS. The choice is remembered,
# so `of setup` / `of update` don't put another VS's libs back. An unrecorded install
# only counts as a switch when a VS was asked for; plain updates keep old behaviour.
resolveVsLibs(){
	local prior wanted="${OF_VS_LIBS:-}"
	prior=$(readLibStateField vs_libs) || prior=""
	[[ -z "$wanted" && "${OF_VS_2026:-}" == "1" ]] && wanted=2026
	case "$wanted" in
		""|2019|2022|2026) ;;
		*) echoError "$(t msg.vs_libs_must_be_2019_2022 "OF_VS_LIBS must be 2019, 2022 or 2026 (got: %s)" "${wanted}")"; return 1 ;;
	esac
	VS_LIBS_RESOLVED="${wanted:-${prior:-2022}}"
	VS_LIBS_SWITCH=0
	if [[ "$VS_LIBS_RESOLVED" != "$prior" && ( -n "$prior" || -n "$wanted" ) ]] \
		&& [[ -e "${OF_DIR}/libs/FreeImage/lib/vs" || -e "${OF_DIR}/libs/openFrameworksCompiled/lib/vs" ]]; then
		VS_LIBS_SWITCH=1
		echoNote "$(t msg.installed_libs_vs_replacing_with_vs "installed libs: VS %s → replacing with VS %s" "${prior:-unrecorded}" "${VS_LIBS_RESOLVED}")"
	fi
	if [[ "$VS_LIBS_RESOLVED" == "2019" ]] && hostIsArm64; then
		echoNote "$(t msg.vs_2019_libs_x64_only "VS 2019 libs are x64 only")"
	fi
}

# Download libs using current LIB_SOURCE / LIB_TAG (no banner)
runLibsDownload(){
	local platformDir="${1:-$OF_PLATFORM}"
	# msys2 package = toolchain env; pass it explicitly — from Git Bash the
	# wrapper would otherwise read MSYSTEM=MINGW64. Function-local export so
	# it never leaks into a later vs/linux download in the same menu session.
	if [[ "$platformDir" == "msys2" ]]; then
		local msysEnv="${LIB_ARCH:-}"
		case "$msysEnv" in
			ucrt64|clang64|clangarm64|mingw64) ;;
			*)
				if [[ -n "${OF_MSYS2_LIB_ENV:-}" ]]; then
					msysEnv="$OF_MSYS2_LIB_ENV"
				elif [[ "$OF_PLATFORM" == "msys2" && -n "$OF_ARCH" ]]; then
					msysEnv="$OF_ARCH"
				else
					msysEnv=$(detectMsys2Env)
				fi
				;;
		esac
		unset OF_MSYS2_LIB_ENV
		local -x LIB_ARCH="$msysEnv"
	fi
	local source="${LIB_SOURCE:-apothecary}"
	local tag="${LIB_TAG:-latest}"
	local libsSel="${LIB_LIBS:-core}"
	local -a oflibsArgs=()
	# read by writeLibState; stays unset for other platforms so lib/vs is remembered
	local OF_VS_LIBS_INSTALLED
	[[ "$platformDir" == "vs" ]] && OF_VS_LIBS_INSTALLED=""

	case "$source" in
		apothecary|"")
			export LIB_TAG="$tag"
			if ! type downloadApothecaryLibs >/dev/null 2>&1; then
				echoError "$(t msg.missing_scripts_dev_lib_sources_sh_2 "missing scripts/dev/lib_sources.sh")"
				echoNote "$(t msg.run_clone_re_download_nightly_that "run: of clone   (or re-download a nightly that ships of Menu scripts)")"
				return 1
			fi
			if [[ "$platformDir" == "vs" ]]; then
				resolveVsLibs || return 1
				local -x OF_VS_LIBS="$VS_LIBS_RESOLVED"
				local -x OF_VS_SWITCH="$VS_LIBS_SWITCH"
				OF_VS_LIBS_INSTALLED="$VS_LIBS_RESOLVED"
			fi
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
				echoError "$(t msg.archive_needs_lib_tag_v0_x "archive needs LIB_TAG=v0.x.y")"
				return 1
			}
			downloadArchiveRelease "$OF_DIR" "$tag" "$platformDir" "$OF_ARCH" libs || return 1
			;;
		*)
			echoError "$(t msg.unknown_lib_source "unknown LIB_SOURCE: %s" "$source")"
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
	echoInfo "$(t msg.download_libraries "download libraries · %s" "${source}")"
	echoKV "$(t msg.platform "platform")" "${platformDir}${OF_ARCH:+ / ${OF_ARCH}}"
	echoKV "$(t msg.source "source")" "$source"
	echoKV "$(t msg.tag "tag")" "$tag"
	printf '\n'

	assessLibsState "$platformDir" "$source" "$tag"
	echoKV "$(t msg.libs_state "libs state")" "${LIBS_STATE} — ${LIBS_STATE_DETAIL}"

	if [[ "$LIBS_STATE" == "ok" && "$force" != "1" ]]; then
		if menuCanRun; then
			confirmNo "$(t msg.libraries_look_current_redownload_anyway "Libraries look current. Redownload anyway?")" || {
				echoInfo "$(t msg.skipped_libs_already "skipped — libs already %s" "${LIBS_STATE_DETAIL}")"
				return 0
			}
		else
			echoInfo "$(t msg.libs_ok_skip_redownload_set_force "libs ok — skip redownload (set force or use menu)")"
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
	echoSuccess "$(t msg.libraries_ready "libraries ready · %s@%s" "${source}" "${LIB_TAG}")"
	printf '\n'
}

cmdUpdatePG(){
	local platformDir="${1:-$OF_PLATFORM}"
	local force="${2:-0}"

	printBanner "update"
	echoInfo "$(t msg.project_generator "Project Generator")"
	assessPGState
	echoKV "$(t msg.pg_state "pg state")" "${PG_STATE} — ${PG_STATE_DETAIL}"

	if [[ "$PG_STATE" == "ok" && "$force" != "1" ]]; then
		if menuCanRun; then
			confirmNo "$(t msg.pg_installed_download_latest_anyway "PG installed (%s). Download latest anyway?" "${PG_STATE_DETAIL}")" || {
				echoInfo "$(t msg.skipped_pg "skipped PG — %s" "${PG_STATE_DETAIL}")"
				return 0
			}
		else
			echoInfo "$(t msg.pg_ok_skip_already "PG ok — skip (already %s)" "${PG_STATE_DETAIL}")"
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
	echoSuccess "$(t msg.pg_ready "PG ready · %s" "${PG_STATE_DETAIL}")"
	printf '\n'
}

cmdUpdate(){
	local subcmd="$1"
	local platformDir="${2:-$OF_PLATFORM}"
	local arch="${3:-}"
	if [[ -n "$arch" ]]; then
		export LIB_ARCH="$arch"
		export OF_ARCH="$arch"
	fi
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
		*) echoError "$(t msg.valid_libs_pg_all "valid: libs | pg | all")"; return 1 ;;
	esac
}

menuPickLibSource(){
	menuPick "$(t menu.library_source "Library source")" \
		"$(t menu.apothecary_openframeworks_apothecary_recommended "Apothecary  — openframeworks/apothecary (recommended)")|apothecary" \
		"$(t menu.oflibs_ofworks_oflibs_experimental "ofLibs      — ofWorks/ofLibs (experimental)")|oflibs" \
		"$(t menu.archive_libs_danoli3_com_historical "Archive     — libs.danoli3.com historical")|archive" \
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
				"$(t menu.osx_desktop_host_libraries "osx         — desktop host libraries")|osx"
				"$(t menu.macos_multi_target_xcframeworks_ios_macos "macos       — multi-target XCFrameworks (iOS, macOS, tvOS)")|macos"
				"$(t menu.ios_iphone_ipad "ios         — iPhone / iPad")|ios"
				"android|android"
				"linux|linux"
				"emscripten|emscripten"
				"msys2|msys2"
				"$(t menu.vs_visual_studio "vs          — Visual Studio")|vs"
			)
			;;
		oflibs)
			opts=(
				"$(t menu.osx_macos "osx / macos")|osx"
				"linux|linux"
				"$(t menu.linux_aarch64 "linux aarch64")|linuxaarch64"
				"emscripten|emscripten"
				"$(t menu.visual_studio "Visual Studio")|vs"
			)
			;;
		archive)
			opts=(
				"osx|osx"
				"ios|ios"
				"android|android"
				"linux|linux"
				"msys2|msys2"
				"$(t menu.visual_studio "Visual Studio")|vs"
			)
			;;
	esac
	[[ ${#opts[@]} -gt 0 ]] || return 1
	echoNote "$(t msg.detected_platform "detected platform: %s%s" "${current}" "${OF_ARCH:+ / ${OF_ARCH}}")"
	menuPick "$(t menu.libraries_for_which_platform "Libraries for which platform?")" "${opts[@]}" || return 1
	LIB_PLATFORM="$UI_MENU_RESULT"
	export LIB_PLATFORM
	if [[ "$LIB_PLATFORM" == "msys2" ]]; then
		menuPickMsys2Env || return 1
	fi
}

# MSYS2 environment → LIB_ARCH; the detected env is listed first (default)
menuPickMsys2Env(){
	local detected env label tag="(detected)"
	local -a opts=()
	if [[ "$OF_PLATFORM" == "msys2" && -n "$OF_ARCH" ]]; then
		detected="$OF_ARCH"
	else
		# not in an MSYS2 shell (e.g. Git Bash) — CPU default, not detected
		detected=$(detectMsys2Env)
		tag="(default)"
	fi
	for env in "$detected" clangarm64 ucrt64 clang64; do
		[[ " ${opts[*]} " == *"|${env} "* ]] && continue
		case "$env" in
			ucrt64)     label="ucrt64      — GCC, UCRT (x64)" ;;
			clang64)    label="clang64     — Clang, UCRT (x64)" ;;
			clangarm64) label="clangarm64  — Clang, UCRT (ARM64)" ;;
			mingw64)    label="mingw64     — GCC, MSVCRT (x64)" ;;
			*) continue ;;
		esac
		[[ "$env" == "$detected" ]] && label+="  ${tag}"
		opts+=("${label}|${env}")
	done
	menuPick "$(t menu.msys2_environment "MSYS2 environment?")" "${opts[@]}" || return 1
	# consumed (and cleared) by runLibsDownload
	OF_MSYS2_LIB_ENV="$UI_MENU_RESULT"
	echoSuccess "$(t msg.msys2_env "msys2 env → %s" "$OF_MSYS2_LIB_ENV")"
}

menuPickLibTag(){
	local source="${1:-$LIB_SOURCE}"
	local -a opts=()
	local t seen=""
	case "$source" in
		apothecary)
			opts+=("$(t menu.latest_bleeding_edge "latest (bleeding edge)")|latest" "nightly|nightly")
			echoNote "$(t msg.fetching_apothecary_tags "fetching apothecary tags…")"
			while IFS= read -r t; do
				[[ -z "$t" || "$t" == "latest" || "$t" == "nightly" ]] && continue
				[[ " $seen " == *" $t "* ]] && continue
				seen+=" $t"
				opts+=("${t}|${t}")
			done < <(listApothecaryReleases | head -20)
			;;
		oflibs)
			echoNote "$(t msg.fetching_oflibs_tags "fetching ofLibs tags…")"
			while IFS= read -r t; do
				[[ -n "$t" ]] && opts+=("${t}|${t}")
			done < <(listOfLibsReleases | head -12)
			[[ ${#opts[@]} -eq 0 ]] && opts+=("v1.0|v1.0")
			;;
		archive)
			echoNote "$(t msg.fetching_archive_versions "fetching archive versions…")"
			while IFS= read -r t; do
				[[ -n "$t" ]] && opts+=("${t}|${t}")
			done < <(listArchiveVersions | sort -Vr | head -24)
			;;
	esac
	[[ ${#opts[@]} -eq 0 ]] && { LIB_TAG="latest"; return 0; }
	menuPick "$(t menu.release_version "Release / version · %s" "${source}")" "${opts[@]}" || return 1
	LIB_TAG="$UI_MENU_RESULT"
	export LIB_TAG
}

# 0 when this VS line has an instance installed
vsIsInstalled(){
	local range pf86 vswhere
	case "$1" in
		2019) range="[16.0,17.0)" ;;
		2022) range="[17.0,18.0)" ;;
		2026) range="[18.0,19.0)" ;;
		*) return 1 ;;
	esac
	pf86="$(printenv 'ProgramFiles(x86)' 2>/dev/null)"
	vswhere="$(cygpath -u "${pf86:-C:\\Program Files (x86)}" 2>/dev/null)/Microsoft Visual Studio/Installer/vswhere.exe"
	[[ -x "$vswhere" ]] || return 1
	[[ -n "$("$vswhere" -all -prerelease -products '*' -version "$range" -property installationPath 2>/dev/null)" ]]
}

# 0 when the release has this VS flavour; older tags only have the plain vs package
vsLibsOnRelease(){
	local year="$1" tag="$2" flavour="vs" repo="$2" ver="$2"
	[[ "$tag" == "nightly" ]] && { repo=latest; ver=latest; }
	[[ "$year" == "2022" ]] || flavour="vs_${year}"
	curl -fsIL --max-time 15 -o /dev/null \
		"https://github.com/openframeworks/apothecary/releases/download/${repo}/openFrameworksLibs_${ver}_${flavour}_64_1.zip" 2>/dev/null
}

# Which Visual Studio the libraries are for → OF_VS_LIBS
menuPickVsLibs(){
	local tag="${1:-$LIB_TAG}" year label current
	local -a opts=()
	current=$(readLibStateField vs_libs) || current=""
	echoNote "$(t msg.checking_vs_packages_on "checking VS packages on %s…" "${tag}")"
	for year in 2026 2022 2019; do
		vsLibsOnRelease "$year" "$tag" || continue
		label="VS ${year}"
		vsIsInstalled "$year" && label+="  installed"
		[[ "$year" == "$current" ]] && label+="  current libs"
		opts+=("${label}|${year}")
	done
	if [[ ${#opts[@]} -eq 0 ]]; then
		# offline or rate limited — let the download report what's wrong
		opts=("$(t menu.vs_2026 "VS 2026")|2026" "$(t menu.vs_2022 "VS 2022")|2022" "$(t menu.vs_2019 "VS 2019")|2019")
	fi
	menuPick "$(t menu.libraries_for_which_visual_studio "Libraries for which Visual Studio?")" "${opts[@]}" || return 1
	OF_VS_LIBS="$UI_MENU_RESULT"
	export OF_VS_LIBS
	echoSuccess "$(t msg.visual_studio "visual studio → %s" "$OF_VS_LIBS")"
	if [[ "$OF_VS_LIBS" == "2019" ]]; then
		echoNote "$(t msg.vs_2019_stops_at_msvc_14 "VS 2019 stops at MSVC 14.29, so it can't use the 2022/2026 libs")"
	fi
}

menuPickOfLibs(){
	local tag="${1:-$LIB_TAG}"
	local platformDir="${2:-$OF_PLATFORM}"
	local mode name
	local -a opts=()
	menuPick "$(t menu.oflibs_selection "ofLibs selection · %s / %s" "${tag}" "${platformDir}")" \
		"$(t menu.core_set "Core set")|core" \
		"$(t menu.all_packages_for_platform "All packages for platform")|all" \
		"$(t menu.pick_libraries "Pick libraries…")|pick" \
		|| return 1
	mode="$UI_MENU_RESULT"
	case "$mode" in
		core|all) LIB_LIBS="$mode"; export LIB_LIBS; echoSuccess "$(t msg.libs "libs → %s" "$LIB_LIBS")"; return 0 ;;
		pick)
			echoNote "$(t msg.listing_packages "listing packages…")"
			while IFS= read -r name; do
				[[ -n "$name" ]] && opts+=("${name}|${name}")
			done < <(listOfLibsNames "$tag" "$platformDir")
			[[ ${#opts[@]} -eq 0 ]] && { echoError "$(t msg.no_packages "no packages")"; return 1; }
			menuPickMulti "$(t menu.select_oflibs "Select ofLibs")" "${opts[@]}" || return 1
			LIB_LIBS="$UI_MENU_RESULT"
			export LIB_LIBS
			echoSuccess "$(t msg.libs "libs → %s" "$LIB_LIBS")"
			;;
	esac
}

menuDownloadLibs(){
	local platformDir="${1:-$OF_PLATFORM}"
	printBanner "libs"
	echoInfo "$(t msg.download_libraries_choose_source "download libraries · choose source")"
	printf '\n'

	menuPickLibSource || return 2
	echoSuccess "$(t msg.source_2 "source → %s" "$LIB_SOURCE")"
	menuPickLibPlatform "$LIB_SOURCE" "$platformDir" || return 2
	platformDir="$LIB_PLATFORM"
	echoSuccess "$(t msg.platform_2 "platform → %s" "$platformDir")"
	menuPickLibTag "$LIB_SOURCE" || return 2
	echoSuccess "$(t msg.tag_2 "tag → %s" "$LIB_TAG")"
	if [[ "$LIB_SOURCE" == "apothecary" && "$platformDir" == "vs" ]]; then
		menuPickVsLibs "$LIB_TAG" || return 2
	fi

	# source + tag is enough to start — no extra gates after this. Cross-
	# platform downloads and non-default clean modes are still available via
	# `of update libs <platform>` / LIB_CLEAN_MODE for the cases that need them.
	if [[ "$LIB_SOURCE" == "apothecary" ]]; then
		case "$platformDir" in
			osx)
				echoNote "$(t msg.osx_package_desktop_host_libs "osx package = desktop host libs")"
				echoNote "$(t msg.tip_choose_macos_for_apple_multi "tip: choose macos for Apple multi-target xcframeworks")"
				;;
			macos)
				echoNote "$(t msg.macos_packages_lib_macos_xcframework_osx "macos packages → lib/macos/*.xcframework (osx · ios · tvos · …)")"
				;;
		esac
		export LIB_CLEAN_MODE="${LIB_CLEAN_MODE:-platform}"
		echoKV "$(t msg.clean "clean")" "$LIB_CLEAN_MODE"
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
	menuPick "$(t menu.visual_studio_version_for_apothecary_build "Visual Studio version for apothecary build")" \
		"$(t menu.visual_studio_2026_18 "Visual Studio 2026 (18)")|18" \
		"$(t menu.visual_studio_2022_17 "Visual Studio 2022 (17)")|17" \
		"$(t menu.visual_studio_2019_16 "Visual Studio 2019 (16)")|16" \
		|| return 1
	return 0
}

menuPickApoArch(){
	local type="$1" a
	local -a choices=() arches=()
	if [[ "$type" == "vs" ]]; then
		choices=(
			"All (x64 + ARM64 + ARM64EC)|all"
			"x64 (64)|64"
			"ARM64|arm64"
			"ARM64EC|arm64ec"
			"ARM64 + ARM64EC|arm64+arm64ec"
		)
	else
		read -r -a arches <<< "$(archesForApoType "$type")"
		for a in "${arches[@]}"; do choices+=("${a}|${a}"); done
	fi
	menuPick "$(t menu.target_architecture "Target architecture · %s" "${type}")" "${choices[@]}"
}

# Keep the native platform first in Apothecary's platform menus.  In
# particular, Windows users should see Visual Studio before Apple targets.
apoBuildTypesOrdered(){
	local type
	if [[ "$OF_PLATFORM" == "vs" ]]; then
		printf '%s\n' vs
	fi
	for type in "${APO_BUILD_TYPES[@]}"; do
		[[ "$OF_PLATFORM" == "vs" && "$type" == "vs" ]] && continue
		printf '%s\n' "$type"
	done
}

runApothecaryEngine(){
	local type="$1" arch="$2"
	shift 2
	# Apothecary accepts one VS architecture per invocation.  Let the oF menu
	# expose useful groups while keeping the underlying calls conventional.
	if [[ "$type" == "vs" && ( "$arch" == "all" || "$arch" == *","* || "$arch" == *"+"* ) ]]; then
		local expanded="$arch" vsArch
		[[ "$expanded" == "all" ]] && expanded="64,arm64,arm64ec"
		expanded="${expanded//+/,}"
		for vsArch in ${expanded//,/ }; do
			echoInfo "$(t msg.visual_studio_libraries "Visual Studio libraries · %s" "${vsArch}")"
			runApothecaryEngine "$type" "$vsArch" "$@" || return $?
		done
		return 0
	fi
	local -a cmd=()
	resolveApothecary
	[[ -f "$APO_ENGINE" ]] || { echoError "$(t msg.apothecary_engine_missing_git_submodule_update "apothecary engine missing — git submodule update --init scripts/apothecary")"; return 1; }
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
		echoKV "$(t msg.vs_ver "VS_VER")" "${vsVer} ($([[ "$vsVer" == 18 ]] && echo 2026 || { [[ "$vsVer" == 17 ]] && echo 2022 || echo 2019; }))"
		( cd "${APO_HOME}/apothecary" 2>/dev/null || cd "$APO_HOME" || exit 1; VS_VER="$vsVer" "${cmd[@]}" )
	else
		( cd "${APO_HOME}/apothecary" 2>/dev/null || cd "$APO_HOME" || exit 1; "${cmd[@]}" )
	fi
}

launchApoMenu(){
	resolveApothecary
	[[ -f "$APO_CLI" ]] || {
		echoError "$(t msg.apo_cli_not_found_scripts_apothecary "apo CLI not found in scripts/apothecary")"
		echoNote "$(t msg.export_apothecary_home_path_apothecary_with "export APOTHECARY_HOME=/path/to/apothecary with scripts/apo.sh")"
		return 1
	}
	echoInfo "$(t msg.apothecary_menu_output_libs "apothecary menu · output → %s/libs" "${OF_DIR}")"
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
	echoInfo "$(t msg.build_all_core "build all core · %s/%s" "${type}" "${arch}")"
	echoKV "$(t msg.out "out")" "${OF_DIR}/libs"
	[[ "$type" == "osx" || "$type" == "macos" ]] && \
		echoNote "$(t msg.macos_multi_target_xcframeworks_osx_desktop "macos = multi-target xcframeworks; osx = desktop host")"
	confirmYes "$(t msg.run_apothecary_update_core_for "Run apothecary update core for %s/%s?" "${type}" "${arch}")" || { echoInfo "$(t msg.cancelled "cancelled")"; return 0; }
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
	echoSuccess "$(t msg.core_built_libs "core built → %s/libs" "${OF_DIR}")"
}

menuApothecary(){
	local choice type arch name a
	local -a opts aopts arches
	resolveApothecary
	printBanner "apothecary"
	echoKV "$(t msg.submodule "submodule")" "${APO_HOME:-missing}"
	echoKV "$(t msg.engine "engine")" "${APO_ENGINE:-missing}"
	echoKV "$(t msg.apo_menu "apo menu")" "${APO_CLI:-not found}"
	echoKV "$(t msg.output "output")" "${OF_DIR}/libs"
	printf '\n'
	[[ -n "$APO_ENGINE" ]] || { echoError "$(t msg.init_scripts_apothecary_submodule "init scripts/apothecary submodule")"; return 1; }

	opts=(
		"$(t menu.build_all_core_for_this_machine "Build all core for this machine (%s/%s)" "${OF_PLATFORM}" "${OF_ARCH}")|build-host"
		"$(t menu.build_all_core_for_platform "Build all core for platform…")|build-type"
		"$(t menu.build_single_library "Build single library…")|build-one"
	)
	[[ -n "$APO_CLI" ]] && opts+=("$(t menu.open_apothecary_interactive_menu "Open Apothecary interactive menu")|apo-menu")
	opts+=("$(t menu.status "Status")|status" "$(t menu.back "Back")|back")
	# return 2 (not 0) for every "nothing happened, back/cancelled" exit below
	# so the caller can skip the "press Enter to return" pause and go straight
	# back up a level instead of pausing on an empty/unchanged screen.
	menuPick "$(t menu.apothecary_builds "Apothecary builds")" "${opts[@]}" || return 2
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
			while IFS= read -r type; do
				if [[ "$type" == "macos" ]]; then
					opts+=("$(t menu.macos_apple_multi_target_xcframeworks "macos  — Apple multi-target xcframeworks")|macos")
				elif [[ "$type" == "osx" ]]; then
					opts+=("$(t menu.osx_desktop_host "osx  — desktop host")|osx")
				else
					opts+=("${type}|${type}")
				fi
			done < <(apoBuildTypesOrdered)
			[[ "$OF_PLATFORM" == "osx" ]] && echoNote "$(t msg.macos_covers_osx_ios_tvos_xros "macos covers osx · ios · tvos · xros · watchos · catos")"
			menuPick "$(t menu.build_platform "Build platform")" "${opts[@]}" || return 2
			type="$UI_MENU_RESULT"
			if [[ "$type" == "vs" ]]; then
				menuPickApoVsVer || return 2
				export OF_APO_VS_VER="$UI_MENU_RESULT"
			fi
			menuPickApoArch "$type" || return 2
			cmdApothecaryBuildAll "$type" "$UI_MENU_RESULT"
			;;
		build-one)
			opts=()
			while IFS= read -r type; do
				opts+=("${type}|${type}")
			done < <(apoBuildTypesOrdered)
			menuPick "$(t menu.platform "Platform")" "${opts[@]}" || return 2
			type="$UI_MENU_RESULT"
			if [[ "$type" == "vs" ]]; then
				menuPickApoVsVer || return 2
				export OF_APO_VS_VER="$UI_MENU_RESULT"
			fi
			menuPickApoArch "$type" || return 2
			arch="$UI_MENU_RESULT"
			opts=()
			local f
			for f in "${APO_HOME}/apothecary/formulas"/*; do
				name=$(basename "$f")
				[[ "$name" == _* ]] && continue
				if [[ -d "$f" ]]; then opts+=("${name}|${name}")
				elif [[ "$name" == *.sh ]]; then opts+=("${name%.sh}|${name%.sh}"); fi
			done
			menuPick "$(t menu.formula "Formula · %s/%s" "${type}" "${arch}")" "${opts[@]}" || return 2
			name="$UI_MENU_RESULT"
			confirmYes "$(t msg.build_for "Build %s for %s/%s?" "${name}" "${type}" "${arch}")" || return 2
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
			echoKV "$(t msg.home "home")" "${APO_HOME:-—}"
			echoKV "$(t msg.engine "engine")" "${APO_ENGINE:-—}"
			echoKV "$(t msg.cli "cli")" "${APO_CLI:-—}"
			echoKV "$(t msg.libs_out "libs out")" "${OF_DIR}/libs"
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
		*) echoError "$(t msg.valid_addons_apps "valid: addons | apps")"; return 1 ;;
	esac
	printBanner "upgrade"
	tasksBegin "Tasks" "Confirm" "Locate script" "Upgrade ${subcmd}" "Finish"
	echoWarning "$(t msg.modifies_backup_first "modifies %s/ — backup first" "${subcmd}")"
	confirmYes "$(t msg.continue_upgrade "Continue upgrade %s?" "${subcmd}")" || { taskSet 0 skip; tasksSkipRest; echoInfo "$(t msg.cancelled "cancelled")"; return 0; }
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
	menuPick "$(t menu.upgrade_7_0_11_0_latest "Upgrade  — oF 7.0–11.0 → %s(Latest SDK)" "${ofVer:+v${ofVer} }")" \
		"$(t menu.addons_only "Addons only")|addons" \
		"$(t menu.projects_apps "Projects (apps/)")|apps" \
		"$(t menu.both "Both")|both" \
		"$(t menu.back "Back")|back" \
		|| return 2
	choice="$UI_MENU_RESULT"
	case "$choice" in
		addons) cmdUpgrade addons; return $? ;;
		apps)   cmdUpgrade apps; return $? ;;
		both)   cmdUpgrade addons; cmdUpgrade apps ;;
		back)   return 2 ;;
	esac
}

# Visual Studio components — scripts/vs/install_vs_components.sh applies
# scripts/vs/2019.vsconfig / 2022.vsconfig / 2026.vsconfig through the VS Installer
cmdVsConfig(){
	local vsScript="${OF_CORE_SCRIPT_DIR}/vs/install_vs_components.sh"
	local -a args=()
	local a
	if [[ "$OF_PLATFORM" != "vs" && "$OF_PLATFORM" != "msys2" ]]; then
		echoError "$(t msg.vsconfig_windows_only_platform "vsconfig is Windows only (platform: %s)" "${OF_PLATFORM}")"
		return 1
	fi
	[[ -f "$vsScript" ]] || { echoError "$(t msg.missing_2 "missing: %s" "$vsScript")"; return 1; }
	for a in "$@"; do
		case "$a" in
			"") ;;
			check|--check) args+=(--check) ;;
			update|--update) args+=(--update) ;;
			no-update|--no-update) args+=(--no-update) ;;
			latest|--latest) args+=(--latest) ;;
			minimal|--minimal) args+=(--minimal) ;;
			2019|2022|2026|all) args+=(--vs "$a") ;;
			arm64|--arm64) args+=(--arm64) ;;
			*) echoError "$(t msg.valid_check_latest_minimal_update_no "valid: check | latest | minimal | update | no-update | 2019 | 2022 | 2026 | all | arm64")"; return 1 ;;
		esac
	done
	printBanner "vsconfig"
	ensureScript "$vsScript" 2>/dev/null
	"$vsScript" "${args[@]}"
}

# Menu entry for vsconfig — Auto covers normal machines; VS 2019 is only ever
# picked automatically when it is the only VS installed, so offer it by name.
menuVsConfig(){
	local choice year label
	local -a opts=("$(t menu.auto_installed_vs "Auto — installed VS")|auto" "$(t menu.check_only "Check only")|check")
	# a VS that isn't installed is offered as an install (signed MS bootstrapper)
	for year in 2026 2022 2019; do
		label="VS ${year}"
		[[ "$year" == 2019 ]] && label+=" — final 16.11 (v142)"
		if vsIsInstalled "$year"; then label+="  installed"; else label+="  — install"; fi
		opts+=("${label}|${year}")
	done
	opts+=("$(t menu.back "Back")|back")
	menuPick "$(t menu.visual_studio_components_vsconfig "Visual Studio components (.vsconfig)")" "${opts[@]}" || return 2
	choice="$UI_MENU_RESULT"
	case "$choice" in
		auto) cmdVsConfig ;;
		back) return 2 ;;
		*)    cmdVsConfig "$choice" ;;
	esac
}

cmdSetup(){
	local platformDir="${1:-$OF_PLATFORM}"
	local distroDir depsScript codecsScript
	local doCodecs=0 taskN=0 needLibs=0 needPG=0
	local doVs=0 vsOk=0 vsScript="${OF_CORE_SCRIPT_DIR}/vs/install_vs_components.sh"
	local -a taskNames=()

	# Setup always uses stock apothecary latest for libs
	export LIB_SOURCE=apothecary
	export LIB_TAG=latest

	printBanner "setup"
	echoInfo "$(t msg.setup_libs_source_apothecary_latest "setup · %s  (libs source: apothecary @ latest)" "${platformDir}")"
	printf '\n'

	assessLibsState "$platformDir" "apothecary" "latest"
	echoKV "$(t msg.libs_2 "libs")" "${LIBS_STATE} — ${LIBS_STATE_DETAIL}"
	[[ "$LIBS_STATE" != "ok" ]] && needLibs=1

	assessPGState
	echoKV "$(t msg.pg "pg")" "${PG_STATE} — ${PG_STATE_DETAIL}"
	[[ "$PG_STATE" != "ok" ]] && needPG=1

	if [[ "$OF_PLATFORM" == "linux" ]]; then
		detectLinuxDistro
		[[ -n "$OF_LINUX_DISTRO" ]] || pickLinuxDistro || return 1
		distroDir=$(linuxDistroScriptDir)
		depsScript="${distroDir}/install_dependencies.sh"
		codecsScript="${distroDir}/install_codecs.sh"
		echoKV "$(t msg.distro "distro")" "$OF_LINUX_DISTRO"
		if [[ -f "$codecsScript" ]]; then
			if [[ "${OF_SETUP_CODECS:-}" = 1 ]]; then doCodecs=1
			elif [[ "${OF_SETUP_CODECS:-}" = 0 ]]; then doCodecs=0
			elif menuCanRun && confirmYes "$(t msg.install_media_codecs_too "Install media codecs too?")"; then doCodecs=1
			fi
		fi
	elif [[ "$OF_PLATFORM" == "osx" ]]; then
		depsScript="${OF_CORE_SCRIPT_DIR}/osx/install_dependencies.sh"
	elif [[ "$OF_PLATFORM" == "vs" || "$OF_PLATFORM" == "msys2" ]]; then
		depsScript="${OF_CORE_SCRIPT_DIR}/vs/install_dependencies.sh"
	fi
	# Visual Studio components from scripts/vs/<year>.vsconfig — opt-in, it
	# launches the VS Installer (UAC). OF_SETUP_VS=1|0 answers without asking.
	if [[ "$OF_PLATFORM" == "vs" && -f "$vsScript" ]]; then
		if bash "$vsScript" --check >/dev/null 2>&1; then vsOk=1
		elif [[ "${OF_SETUP_VS:-}" = 1 ]]; then doVs=1
		elif [[ "${OF_SETUP_VS:-}" = 0 ]]; then doVs=0
		elif menuCanRun && confirmYes "$(t msg.visual_studio_missing_components_run_vs "Visual Studio is missing oF components — run the VS Installer (.vsconfig)?")"; then doVs=1
		fi
	fi

	if [[ "$needLibs" -eq 0 && "$needPG" -eq 0 && "$OF_PLATFORM" != "linux" && "$OF_PLATFORM" != "osx" \
		&& "$OF_PLATFORM" != "vs" && "$OF_PLATFORM" != "msys2" ]]; then
		echoSuccess "$(t msg.already_set_up_libs_pg_look "already set up — libs + PG look current")"
		echoNote "$(t msg.use_update_redownload_from_chosen_source "use Update to redownload from a chosen source")"
		printf '\n'
		return 0
	fi

	taskNames+=("Detect platform")
	[[ "$OF_PLATFORM" == "linux" ]] && taskNames+=("Install dependencies (${OF_LINUX_DISTRO})")
	[[ "$doCodecs" -eq 1 ]] && taskNames+=("Install codecs (${OF_LINUX_DISTRO})")
	[[ "$OF_PLATFORM" == "osx" ]] && taskNames+=("Xcode CLT / Homebrew (cmake, gum)")
	[[ "$OF_PLATFORM" == "vs" || "$OF_PLATFORM" == "msys2" ]] && taskNames+=("gum via winget/scoop (optional)")
	[[ "$OF_PLATFORM" == "vs" ]] && taskNames+=("Visual Studio components (.vsconfig)")
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
		if [[ "$OF_PLATFORM" == "vs" ]]; then
			if [[ "$vsOk" -eq 1 ]]; then
				taskSet "$taskN" done "already installed"
			elif [[ "$doVs" -eq 1 ]]; then
				# best-effort: report a failed install but still fetch libs/PG
				ensureScript "$vsScript" 2>/dev/null
				taskLive "$taskN" -- "$vsScript" -y
			else
				taskTickLine "$taskN" skip "run: of vsconfig"
			fi
			taskN=$((taskN + 1))
		fi
	fi

	# libraries — only if missing/outdated
	if [[ "$needLibs" -eq 1 ]]; then
		echoNote "$(t msg.downloading_libraries_apothecary_latest "downloading libraries · apothecary @ latest")"
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
	echoSuccess "$(t msg.setup_complete "setup complete")"
	[[ "$OF_PLATFORM" == "linux" ]] && echoNote "$(t msg.next_scripts_linux_compileof_sh "next: scripts/linux/compileOF.sh")"
	printf '\n'
}

# ---------------------------------------------------------------------------
# Cleanup — projects / caches / libraries
# ---------------------------------------------------------------------------

# Record one cleanup outcome for the --json envelope (no-op otherwise).
#   cleanRecord <removed|would_remove|skipped|failed> <path> <kb> [reason]
OF_JSON_ITEMS=()
OF_J_REMOVED=0; OF_J_KB=0; OF_J_SKIPPED=0; OF_J_FAILED=0
cleanRecord(){
	[[ "$OF_JSON" == "1" ]] || return 0
	local action="$1" path="${2#"$OF_DIR"/}" kb="${3:-0}" reason="${4:-}"
	case "$action" in
		removed|would_remove) OF_J_REMOVED=$((OF_J_REMOVED + 1)); OF_J_KB=$((OF_J_KB + kb)) ;;
		skipped) OF_J_SKIPPED=$((OF_J_SKIPPED + 1)) ;;
		failed) OF_J_FAILED=$((OF_J_FAILED + 1)) ;;
	esac
	OF_JSON_ITEMS+=("{\"action\": \"${action}\", \"path\": \"$(jsonEscape "$path")\", \"kb\": ${kb}$([[ -n "$reason" ]] && printf ', "reason": "%s"' "$(jsonEscape "$reason")")}")
}

# echoError with a stable machine code for the --json envelope
OF_ERR_CODE=""
echoErrorCode(){ OF_ERR_CODE="$1"; shift; echoError "$@"; }

# --- cleanup safety --------------------------------------------------------
# Every delete in the cleanup commands goes through these helpers so nothing
# can be removed outside the openFrameworks tree, and nothing is removed
# without confirmation.
#   OF_CLEAN_YES=1      skip confirmation prompts   (of cleanup ... --yes)
#   OF_CLEAN_DRY_RUN=1  list what would go, delete nothing (--dry-run)
OF_CLEAN_YES="${OF_CLEAN_YES:-0}"
OF_CLEAN_DRY_RUN="${OF_CLEAN_DRY_RUN:-0}"
CLEAN_ABORT_RC=0

# Platform folder names accepted by `of cleanup libs <name>`
# Older Linux layouts are flat folders next to the current lib/linux/<arch>:
# linux64, linux64gcc6, linuxarm64, linuxaarch64, linuxarmv6l, linuxarmv7l.
# Known lib/<platform> folders. Anything not listed is never touched by
# `other-platforms` (unknown folders are kept, not deleted).
#   families:   a folder directly under lib/
#   sub-arches: lib/linux/<arch> (current layout), lib/msys2/<environment>
OF_CLEAN_FAMILIES="vs macos osx ios tvos xros android emscripten msys2 linux linux64 linux64gcc6 linuxarm64 linuxaarch64 linuxarmv6l linuxarmv7l"
OF_CLEAN_SUBS_linux="64 arm64 aarch64 armv6l armv7l jetson"
OF_CLEAN_SUBS_msys2="mingw64 mingw32 ucrt64 clang64 clangarm64"
# What `of cleanup libs <name>` accepts: a family, or family/sub-arch
OF_CLEAN_PLATFORMS="$OF_CLEAN_FAMILIES"
for _s in $OF_CLEAN_SUBS_linux; do OF_CLEAN_PLATFORMS+=" linux/$_s"; done
for _s in $OF_CLEAN_SUBS_msys2; do OF_CLEAN_PLATFORMS+=" msys2/$_s"; done
unset _s

cleanWordIn(){ # cleanWordIn word "list of words"
	local w="$1" x
	for x in $2; do [[ "$x" == "$w" ]] && return 0; done
	return 1
}
# True when a tree holds real library binaries (static/shared libraries,
# frameworks, xcframeworks, jars). A libs/ folder that only has .gitkeep,
# headers or vendored source is not a prebuilt library: `usage` leaves it out
# and `libs all` leaves it alone.
cleanHasLibBinaries(){
	[[ -d "$1" ]] || return 1
	find "$1" -maxdepth 8 \( \
		\( -type f \( -name '*.a' -o -name '*.lib' -o -name '*.so' -o -name '*.so.*' -o -name '*.dylib' \
			-o -name '*.dll' -o -name '*.bc' -o -name '*.jar' -o -name '*.aar' \) \) \
		-o \( -type d \( -name '*.xcframework' -o -name '*.framework' \) \) \
		\) -print 2>/dev/null | head -1 | grep -q .
}

# Sub-arch folders known for a family (empty when it has none)
cleanSubsOf(){
	case "$1" in
		linux) printf '%s' "$OF_CLEAN_SUBS_linux" ;;
		msys2) printf '%s' "$OF_CLEAN_SUBS_msys2" ;;
	esac
}
# Host keep-set for `other-platforms`, from OF_PLATFORM / OF_ARCH.
# Sets CLEAN_KEEP_FAMS and CLEAN_KEEP_SUBS_linux / CLEAN_KEEP_SUBS_msys2.
# An empty sub list means "keep every sub-arch" (host arch unknown).
cleanHostKeepSet(){
	CLEAN_KEEP_FAMS=""
	CLEAN_KEEP_SUBS_linux=""
	CLEAN_KEEP_SUBS_msys2=""
	local arch="${OF_CLEAN_ARCH:-${OF_ARCH:-}}"   # OF_CLEAN_ARCH: override, mainly for tests
	case "$OF_PLATFORM" in
		vs) CLEAN_KEEP_FAMS="vs" ;;
		# Apple platforms share the core and frameworks: keep them together so an
		# iOS / tvOS / visionOS build does not lose its libs on a Mac
		osx|macos|ios|tvos|xros) CLEAN_KEEP_FAMS="macos osx ios tvos xros" ;;
		android|emscripten) CLEAN_KEEP_FAMS="$OF_PLATFORM" ;;
		msys2)
			CLEAN_KEEP_FAMS="msys2 vs"
			cleanWordIn "$arch" "$OF_CLEAN_SUBS_msys2" && CLEAN_KEEP_SUBS_msys2="$arch"
			;;
		linux)
			CLEAN_KEEP_FAMS="linux"
			case "$arch" in
				x86_64|amd64|64)   CLEAN_KEEP_FAMS+=" linux64 linux64gcc6";          CLEAN_KEEP_SUBS_linux="64" ;;
				aarch64|arm64)     CLEAN_KEEP_FAMS+=" linuxaarch64 linuxarm64";    CLEAN_KEEP_SUBS_linux="arm64 aarch64 jetson" ;;
				armv7l)            CLEAN_KEEP_FAMS+=" linuxarmv7l";                CLEAN_KEEP_SUBS_linux="armv7l" ;;
				armv6l)            CLEAN_KEEP_FAMS+=" linuxarmv6l";                CLEAN_KEEP_SUBS_linux="armv6l" ;;
				*) CLEAN_KEEP_FAMS+=" linux64 linux64gcc6 linuxarm64 linuxaarch64 linuxarmv6l linuxarmv7l" ;;  # unknown arch: keep all
			esac
			;;
		*) CLEAN_KEEP_FAMS="$OF_PLATFORM" ;;
	esac
}

# Keep set for `libs other <platform>`: keep exactly the platform the user
# picked (a family, or family/arch, in either linux layout) and remove the rest.
cleanKeepSetFromSpec(){
	local spec="$1" sub
	CLEAN_KEEP_FAMS=""; CLEAN_KEEP_SUBS_linux=""; CLEAN_KEEP_SUBS_msys2=""
	case "$spec" in
		macos|osx) CLEAN_KEEP_FAMS="macos osx" ;;
		ios|tvos|xros|android|emscripten|vs) CLEAN_KEEP_FAMS="$spec" ;;
		msys2) CLEAN_KEEP_FAMS="msys2" ;;
		msys2/*) CLEAN_KEEP_FAMS="msys2"; CLEAN_KEEP_SUBS_msys2="${spec#msys2/}" ;;
		linux) CLEAN_KEEP_FAMS="linux linux64 linux64gcc6 linuxarm64 linuxaarch64 linuxarmv6l linuxarmv7l" ;;
		linux/*|linux64|linux64gcc6|linuxarm64|linuxaarch64|linuxarmv6l|linuxarmv7l)
			# one architecture, in the current layout and the older flat one
			case "$spec" in
				linux/*)      sub="${spec#linux/}" ;;
				linux64gcc6)  sub="64" ;;
				*)            sub="${spec#linux}" ;;
			esac
			CLEAN_KEEP_SUBS_linux="$sub"
			CLEAN_KEEP_FAMS="linux"
			if [[ "$sub" != jetson ]]; then CLEAN_KEEP_FAMS+=" linux${sub}"; fi
			[[ "$sub" == 64 ]] && CLEAN_KEEP_FAMS+=" linux64gcc6"
			;;
	esac
}

# Should `other-platforms` remove $1 given the keep set (CLEAN_KEEP_*)?
cleanOtherShouldRemove(){
	local d="$1" name fam sub keepSubs
	name=$(basename -- "$d")
	if cleanWordIn "$name" "$OF_CLEAN_FAMILIES"; then
		fam="$name"; sub=""
	else
		fam=$(basename -- "$(dirname -- "$d")"); sub="$name"
	fi
	if [[ -z "$sub" ]]; then
		cleanWordIn "$fam" "$CLEAN_KEEP_FAMS" && return 1      # a family that is kept
		return 0
	fi
	# a sub-arch folder: only inside a kept family (a dropped family goes whole)
	cleanWordIn "$fam" "$CLEAN_KEEP_FAMS" || return 1
	case "$fam" in linux) keepSubs="$CLEAN_KEEP_SUBS_linux" ;; msys2) keepSubs="$CLEAN_KEEP_SUBS_msys2" ;; *) keepSubs="" ;; esac
	[[ -z "$keepSubs" ]] && return 1
	cleanWordIn "$sub" "$keepSubs" && return 1
	return 0
}

# Remove the paths in CLEAN_CAND (git-tracked ones are skipped). Works on the
# caller's count / skipped / failed / totalFreedKB.
cleanLibRemoveList(){
	local d
	for d in ${CLEAN_CAND[@]+"${CLEAN_CAND[@]}"}; do
		[[ -e "$d" || -L "$d" ]] || continue
		if pathIsGitTracked "$d"; then
			skipped=$((skipped + 1))
			cleanRecord skipped "$d" 0 git_tracked
			printf '  %s–%s %s  %s(git-tracked source — skipped)%s\n' "$C_MUTED" "$C_RESET" "$(cleanShow "${d#"$OF_DIR"/}")" "$C_MUTED" "$C_RESET"
			continue
		fi
		cleanLibRemoveOne "$d" "${d#"$OF_DIR"/}"
	done
}
# How many of CLEAN_CAND would really be removed (not git-tracked)
cleanCandRemovable(){
	local d n=0
	for d in ${CLEAN_CAND[@]+"${CLEAN_CAND[@]}"}; do
		pathIsGitTracked "$d" || n=$((n + 1))
	done
	printf '%s' "$n"
}

# Print (NUL-terminated) every folder or symlink inside a lib/ directory that
# is a known platform: families, and for linux / msys2 their sub-arch folders.
# Used by `other-platforms` and `usage`.
cleanEnumPlatformDirs(){   # [lib-dir]  (default: every lib/ under libs/ and addons/)
	local libdir e name sub subs only="${1:-}"
	while IFS= read -r -d '' libdir; do
		for e in "$libdir"/*; do
			[[ -d "$e" || -L "$e" ]] || continue
			name="$(basename -- "$e")"
			cleanWordIn "$name" "$OF_CLEAN_FAMILIES" || continue
			printf '%s\0' "$e"
			subs="$(cleanSubsOf "$name")"
			[[ -n "$subs" && -d "$e" && ! -L "$e" ]] || continue
			for sub in "$e"/*; do
				[[ -d "$sub" || -L "$sub" ]] || continue
				cleanWordIn "$(basename -- "$sub")" "$subs" && printf '%s\0' "$sub"
			done
		done
	done < <(if [[ -n "$only" ]]; then [[ -d "$only" ]] && printf '%s\0' "$only"; else find "${OF_DIR}/libs" "${OF_DIR}/addons" -maxdepth 6 -type d -name lib -print0 2>/dev/null; fi)
}

# True when OF_DIR really is an openFrameworks tree (guards "${OF_DIR}/libs"
# collapsing to /libs when OF_DIR is empty, "/" or the wrong folder).
ofRootIsSane(){
	[[ -n "${OF_DIR:-}" && "$OF_DIR" != "/" ]] || return 1
	[[ -d "$OF_DIR/libs/openFrameworks" && -d "$OF_DIR/scripts" ]]
}

# Canonical dirname/basename of $1. A final symlink is NOT followed, so a
# symlinked folder is judged (and removed) as the link, never its target.
cleanResolve(){
	local p="$1" dir base
	[[ -n "$p" ]] || return 1
	dir="$(dirname -- "$p")"
	base="$(basename -- "$p")"
	case "$base" in ""|.|..|/) return 1 ;; esac
	dir="$(cd -- "$dir" 2>/dev/null && pwd -P)" || return 1
	printf '%s/%s' "${dir%/}" "$base"
}

# True if $1 resolves strictly inside OF_DIR and is not one of the trees the
# cleanup commands must never delete (the tree root, .git, source, scripts).
cleanPathIsSafe(){
	local full root rel
	ofRootIsSane || return 1
	full="$(cleanResolve "$1")" || return 1
	root="$(cd -- "$OF_DIR" && pwd -P)" || return 1
	[[ "$full" == "$root"/* ]] || return 1
	rel="${full#"$root"/}"
	case "$rel" in
		""|.git|.git/*) return 1 ;;
		libs|addons|apps|examples|scripts|scripts/*|projectGenerator) return 1 ;;
		apps/myApps|libs/openFrameworks|libs/openFrameworks/*|libs/openFrameworksCompiled) return 1 ;;
	esac
	return 0
}

# Is $1 a folder inside OF_DIR that `of cleanup projects <path>` may walk?
# (Looser than cleanPathIsSafe: apps/, examples/ and addons/ themselves are
# fine as scan roots, but not the tree root, .git, libs or scripts.)
cleanScanRootIsSafe(){
	local full root rel
	ofRootIsSane || return 1
	[[ -d "$1" ]] || return 1
	full="$(cd -- "$1" 2>/dev/null && pwd -P)" || return 1
	root="$(cd -- "$OF_DIR" && pwd -P)" || return 1
	[[ "$full" == "$root"/* ]] || return 1
	rel="${full#"$root"/}"
	case "$rel" in
		.git|.git/*|libs|libs/*|scripts|scripts/*|projectGenerator|projectGenerator/*) return 1 ;;
	esac
	return 0
}

# Is $1 one of the accepted `libs <platform>` names?
cleanPlatformIsValid(){
	local n="$1" ok
	[[ -n "$n" ]] || return 1
	for ok in $OF_CLEAN_PLATFORMS; do
		[[ "$n" == "$ok" ]] && return 0
	done
	return 1
}

# Ask before a destructive step. Honours --yes / --dry-run; with no TTY and
# no --yes it refuses. Returns 0 to proceed; otherwise 1 with CLEAN_ABORT_RC
# set to the code the caller should return (0 = user said no, 1 = refused).
confirmDestructive(){
	local prompt="$1"
	CLEAN_ABORT_RC=0
	[[ "$OF_CLEAN_DRY_RUN" == "1" ]] && return 0
	[[ "$OF_CLEAN_YES" == "1" ]] && return 0
	if menuCanRun; then
		confirmNo "$prompt" && return 0
		echoInfo "$(t msg.cancelled_nothing_deleted "cancelled — nothing deleted")"
		return 1
	fi
	echoErrorCode confirmation_required "$(t msg.refusing_delete_without_confirmation_no_tty "refusing to delete without confirmation (no TTY) — re-run with --yes")"
	CLEAN_ABORT_RC=1
	return 1
}

# The one permanent-delete helper. Validates the path, honours --dry-run,
# unlinks symlinks instead of following them, and returns rm's real status
# (so callers only count what was actually removed).
safeRemovePath(){
	local p="$1" full
	[[ -e "$p" || -L "$p" ]] || return 0
	if ! cleanPathIsSafe "$p"; then
		echoWarning "$(t msg.refusing_delete_outside_openframeworks_tree "refusing to delete outside the openFrameworks tree: %s" "$(cleanShow "$p")")"
		return 1
	fi
	[[ "$OF_CLEAN_DRY_RUN" == "1" ]] && return 0
	full="$(cleanResolve "$p")" || return 1
	if [[ -L "$full" ]]; then
		rm -f -- "$full"
	else
		rm -rf -- "$full"
	fi
}

# KB-on-disk for existing paths (0 for none) — one batched `du`, not per-path
pathsSizeKB(){
	local -a existing=()
	local p
	for p in "$@"; do
		[[ -e "$p" || -L "$p" ]] && existing+=("$p")
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

# MSYS2 doesn't ship git (pacman -S git). When git isn't on PATH but Git for
# Windows is installed, use its git.exe — MSYS2 converts the /c/… path args.
if ! command -v git >/dev/null 2>&1; then
	for _ofGitExe in \
		"$(cygpath -u "$(printenv ProgramW6432 2>/dev/null || printenv ProgramFiles 2>/dev/null)" 2>/dev/null)/Git/cmd/git.exe" \
		"/c/Program Files/Git/cmd/git.exe" \
		"$(cygpath -u "$(printenv LOCALAPPDATA 2>/dev/null)" 2>/dev/null)/Programs/Git/cmd/git.exe"; do
		if [[ -x "$_ofGitExe" ]]; then
			OF_GIT_EXE="$_ofGitExe"
			git(){ "$OF_GIT_EXE" "$@"; }
			break
		fi
	done
	unset _ofGitExe
fi

ofHasGit(){
	command -v git >/dev/null 2>&1 || return 1
	[[ -e "$OF_DIR/.git" ]] || return 1
	git -C "$OF_DIR" rev-parse --verify HEAD >/dev/null 2>&1
}

# git line when ofHasGit fails: a real nightly, or a checkout without git
ofNoGitLabel(){
	if [[ -e "$OF_DIR/.git" ]]; then
		printf 'git checkout (git not found — pacman -S git)'
	else
		printf 'nightly package (no .git)'
	fi
}

ofGitSummary(){
	local branch sha
	ofHasGit || { ofNoGitLabel; return 1; }
	branch=$(git -C "$OF_DIR" rev-parse --abbrev-ref HEAD 2>/dev/null || echo "?")
	sha=$(git -C "$OF_DIR" rev-parse --short HEAD 2>/dev/null || echo "?")
	printf '%s @ %s' "$branch" "$sha"
}

# Nightly zip → git init + fetch + checkout; untracked libs/PG/apps stay.
cmdClone(){
	local remote="${OF_GIT_REMOTE:-https://github.com/openframeworks/openFrameworks.git}"
	local branch="${OF_GIT_BRANCH:-master}"
	local sha
	printBanner "clone"
	if ofHasGit; then
		echoSuccess "$(t msg.already_git_checkout "already a git checkout")"
		echoKV "$(t msg.git "git")" "$(ofGitSummary)"
		echoKV "$(t msg.remote "remote")" "$(git -C "$OF_DIR" remote get-url origin 2>/dev/null || echo none)"
		return 0
	fi
	if [[ -e "$OF_DIR/.git" ]]; then
		echoWarning "$(t msg.already_git_checkout_but_git_can "already a git checkout, but git can't read it")"
		echoKV "$(t msg.git "git")" "$(ofNoGitLabel)"
		return 1
	fi
	if ! command -v git >/dev/null 2>&1; then
		echoError "$(t msg.git_not_found "git not found")"
		echoNote "$(t msg.install_git_then_re_run_clone "install git, then re-run: of clone")"
		return 1
	fi
	echoInfo "$(t msg.attach_this_folder "attach %s (%s) to this folder" "${remote}" "${branch}")"
	echoNote "$(t msg.keeps_downloaded_libs_projectgenerator_apps_myapps "keeps downloaded libs, projectGenerator, and apps/myApps")"
	echoNote "$(t msg.restores_files_nightly_package_stripped "restores files the nightly package stripped")"
	echoKV "$(t msg.dir "dir")" "$OF_DIR"
	echoKV "$(t msg.remote "remote")" "$remote"
	echoKV "$(t msg.branch "branch")" "$branch"
	printf '\n'
	if menuCanRun; then
		confirmYes "$(t msg.turn_this_nightly_into_git_clone "Turn this nightly into a git clone of openFrameworks.git?")" || { echoInfo "$(t msg.cancelled "cancelled")"; return 0; }
	fi

	tasksBegin "Clone" "git init" "remote" "fetch ${branch}" "checkout" "Finish"
	taskSet 0 running
	if [[ ! -e "$OF_DIR/.git" ]]; then
		if ! git -C "$OF_DIR" -c init.defaultBranch="$branch" init -q; then
			taskSet 0 fail; tasksSkipRest; tasksSummary; return 1
		fi
	fi
	taskSet 0 done
	taskSet 1 running
	if git -C "$OF_DIR" remote get-url origin >/dev/null 2>&1; then
		git -C "$OF_DIR" remote set-url origin "$remote" || { taskSet 1 fail; tasksSkipRest; tasksSummary; return 1; }
	else
		git -C "$OF_DIR" remote add origin "$remote" || { taskSet 1 fail; tasksSkipRest; tasksSummary; return 1; }
	fi
	taskSet 1 done "$remote"
	taskSet 2 running
	if ! git -C "$OF_DIR" fetch --depth=1 origin "$branch"; then
		taskSet 2 fail "fetch failed"
		tasksSkipRest
		tasksSummary
		echoNote "$(t msg.check_network_git_remote "check network / OF_GIT_REMOTE")"
		return 1
	fi
	taskSet 2 done
	taskSet 3 running
	if ! git -C "$OF_DIR" checkout -f -B "$branch" "origin/${branch}"; then
		taskSet 3 fail "checkout failed"
		tasksSkipRest
		tasksSummary
		return 1
	fi
	sha=$(git -C "$OF_DIR" rev-parse --short HEAD 2>/dev/null || echo "?")
	taskSet 3 done "$sha"
	taskTickLine 4 done
	tasksSummary
	echoSuccess "$(t msg.this_folder_clone "this folder is a clone of %s (%s @ %s)" "${remote}" "${branch}" "${sha}")"
	echoNote "$(t msg.later_git_pull_deepen_history_git "later: git pull   ·   deepen history: git fetch --unshallow")"
}

# Run a command, killing it after $1 seconds. Returns its status (143 on
# timeout). Used for the Trash helpers, which can block on a dialog or an
# unresponsive Finder / shell. No `timeout` binary needed (macOS has none).
cleanRunTimeout(){
	local secs="$1" pid watcher rc
	shift
	"$@" &
	pid=$!
	( sleep "$secs"; kill "$pid" 2>/dev/null ) >/dev/null 2>&1 &
	watcher=$!
	wait "$pid" 2>/dev/null
	rc=$?
	kill "$watcher" 2>/dev/null
	wait "$watcher" 2>/dev/null
	return $rc
}
OF_CLEAN_TRASH_TIMEOUT="${OF_CLEAN_TRASH_TIMEOUT:-30}"

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
		[[ -e "$p" || -L "$p" ]] || continue
		if ! cleanPathIsSafe "$p"; then
			echoWarning "$(t msg.refusing_delete_outside_openframeworks_tree "refusing to delete outside the openFrameworks tree: %s" "$(cleanShow "$p")")"
			continue
		fi
		paths+=("$p")
	done
	TRASH_METHOD="rm"
	[[ ${#paths[@]} -eq 0 ]] && return 0
	[[ "$OF_CLEAN_DRY_RUN" == "1" ]] && { TRASH_METHOD="dry-run"; return 0; }

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
				local psEnc=""
				if command -v iconv >/dev/null 2>&1 && command -v base64 >/dev/null 2>&1; then
					# -EncodedCommand takes UTF-16LE, so names outside the console code page are kept intact
					psEnc=$(printf '%s' "$psCmd" | iconv -f UTF-8 -t UTF-16LE 2>/dev/null | base64 | tr -d '\n\r')
				fi
				if [[ -n "$psEnc" ]]; then
					cleanRunTimeout "$OF_CLEAN_TRASH_TIMEOUT" powershell.exe -NoProfile -NonInteractive -EncodedCommand "$psEnc" >/dev/null 2>&1 && TRASH_METHOD="trash"
				else
					cleanRunTimeout "$OF_CLEAN_TRASH_TIMEOUT" powershell.exe -NoProfile -NonInteractive -Command "$psCmd" >/dev/null 2>&1 && TRASH_METHOD="trash"
				fi
			fi
			;;
		osx)
			if command -v trash >/dev/null 2>&1 && cleanRunTimeout "$OF_CLEAN_TRASH_TIMEOUT" trash -F "${paths[@]}" >/dev/null 2>&1; then
				TRASH_METHOD="trash"
			elif [[ -z "${SSH_CONNECTION:-}" && "$(launchctl managername 2>/dev/null)" == "Aqua" ]]; then
				# Finder needs a logged-in GUI session; over ssh it would hang or prompt
				local asList="" esc
				for p in "${paths[@]}"; do
					esc="${p//\\/\\\\}"
					esc="${esc//\"/\\\"}"
					asList+="POSIX file \"${esc}\", "
				done
				asList="${asList%, }"
				cleanRunTimeout "$OF_CLEAN_TRASH_TIMEOUT" osascript -e "tell application \"Finder\" to delete {${asList}}" >/dev/null 2>&1 && TRASH_METHOD="trash"
			fi
			;;
		linux)
			if command -v gio >/dev/null 2>&1 && cleanRunTimeout "$OF_CLEAN_TRASH_TIMEOUT" gio trash "${paths[@]}" >/dev/null 2>&1; then
				TRASH_METHOD="trash"
			elif command -v trash-put >/dev/null 2>&1 && cleanRunTimeout "$OF_CLEAN_TRASH_TIMEOUT" trash-put "${paths[@]}" >/dev/null 2>&1; then
				TRASH_METHOD="trash"
			elif command -v trash >/dev/null 2>&1 && cleanRunTimeout "$OF_CLEAN_TRASH_TIMEOUT" trash "${paths[@]}" >/dev/null 2>&1; then
				TRASH_METHOD="trash"
			fi
			;;
	esac

	if [[ "$TRASH_METHOD" != "trash" ]]; then
		for p in "${paths[@]}"; do safeRemovePath "$p" || true; done
	fi
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
	# a CMake build tree under any name: <project>/<dir>[/<dir>]/CMakeCache.txt
	compgen -G "${project}/*/CMakeCache.txt" >/dev/null 2>&1 && return 0
	compgen -G "${project}/*/*/CMakeCache.txt" >/dev/null 2>&1 && return 0
	compgen -G "${project}"/*.xcodeproj/xcuserdata >/dev/null 2>&1 && return 0
	compgen -G "${project}"/*.xcodeproj/project.xcworkspace >/dev/null 2>&1 && return 0
	return 1
}

# Print (NUL-terminated) the CMake build trees inside a project: any directory
# up to two levels down that holds a CMakeCache.txt created for this project
# (CMAKE_HOME_DIRECTORY is the project). Never the project itself (an in-source
# build), never bin/ or src/, and never a tree that belongs to another source dir.
cleanFindCMakeBuildDirs(){
	local project="${1%/}" cache dir home realProject realHome
	realProject="$(cd -- "$project" 2>/dev/null && pwd -P)" || return 0
	while IFS= read -r -d '' cache; do
		dir="$(dirname -- "$cache")"
		[[ "$dir" != "$project" ]] || continue
		case "${dir#"$project"/}" in bin|bin/*|src|src/*|addons|addons/*|.git|.git/*) continue ;; esac
		home="$(sed -n 's/^CMAKE_HOME_DIRECTORY:INTERNAL=//p' "$cache" 2>/dev/null | head -1)"
		[[ -n "$home" ]] || continue
		# a home that no longer exists is a stale tree (project moved): still ours
		if [[ -d "$home" ]]; then
			realHome="$(cd -- "$home" 2>/dev/null && pwd -P)" || continue
			[[ "$realHome" == "$realProject" ]] || continue
		fi
		printf '%s\0' "$dir"
	done < <(find "$project" -maxdepth 3 -type f -name CMakeCache.txt -not -path "${project}/bin/*" -print0 2>/dev/null)
}

# Which paths a project clean would remove. Sets CLEAN_TARGETS (known build
# folders, only the ones that exist) and CLEAN_DEEP (stray per-IDE files).
# bin/ is excluded. Shared by cleanProjectTree and `of cleanup usage`.
cleanProjectTargets(){
	local project="$1" t g cdir
	CLEAN_TARGETS=()
	CLEAN_DEEP=()
	local -a candidates=(
		"${project}/obj"
		"${project}/build"
		"${project}/.vs"
		"${project}/cmake-build-debug"
		"${project}/cmake-build-release"
		"${project}/DerivedData"
	)
	# exclude bin/ and anything already covered by the folders above, so the
	# deep finds below don't re-match (and double-count) files inside them
	local -a prune=(-not -path "${project}/bin/*")
	for t in "${candidates[@]}"; do
		prune+=(-not -path "${t}/*")
		[[ -e "$t" ]] && CLEAN_TARGETS+=("$t")
	done
	# CMake build trees under any name (-B build-ninja, out/macos, ...)
	while IFS= read -r -d '' cdir; do
		[[ -n "$cdir" ]] || continue
		local dup=0
		for t in "${candidates[@]}"; do [[ "$cdir" == "$t" ]] && dup=1; done
		[[ $dup -eq 1 ]] && continue
		CLEAN_TARGETS+=("$cdir")
		prune+=(-not -path "${cdir}/*")
	done < <(cleanFindCMakeBuildDirs "$project")
	while IFS= read -r -d '' g; do
		CLEAN_DEEP+=("$g")
	done < <(find "$project" -maxdepth 3 "${prune[@]}" \( \
		-name 'xcuserdata' -o -name 'project.xcworkspace' -o \
		-name '*.o' -o -name '*.d' -o -name '*.depend' -o -name '*.layout' \
		\) -print0 2>/dev/null)
	while IFS= read -r -d '' g; do
		CLEAN_DEEP+=("$g")
	done < <(find "$project" -maxdepth 3 "${prune[@]}" \( \
		-name 'x64' -o -name 'ARM64' -o -name 'Win32' -o \
		-name 'Debug' -o -name 'Release' \
		\) -a -type d -a -path '*/obj/*' -print0 2>/dev/null)
}

# Is $1 an openFrameworks project folder (src/, Makefile, CMakeLists.txt, .xcodeproj, .vcxproj)?
cleanIsProjectDir(){
	local d="${1%/}"
	[[ -d "${d}/src" || -f "${d}/Makefile" || -f "${d}/makefile" || -f "${d}/CMakeLists.txt" ]] \
		|| compgen -G "${d}/*.xcodeproj" >/dev/null 2>&1 \
		|| compgen -G "${d}/*.vcxproj" >/dev/null 2>&1
}
# Print every project folder under $1 (category/project, two levels), or $1
# itself when $2 is "self" and it is a project.
cleanFindProjects(){  # NUL-terminated output
	local root="${1%/}" mode="${2:-}" cat ex
	[[ -d "$root" ]] || return 0
	if [[ "$mode" == "self" ]] && cleanIsProjectDir "$root"; then
		printf '%s\0' "$root"
		return 0
	fi
	for cat in "$root"/*/; do
		[[ -d "$cat" ]] || continue
		if cleanIsProjectDir "$cat"; then
			printf '%s\0' "${cat%/}"
			continue
		fi
		for ex in "$cat"*/; do
			[[ -d "$ex" ]] || continue
			cleanIsProjectDir "$ex" && printf '%s\0' "${ex%/}"
		done
	done
}

# Cleans one project dir. Sets CLEAN_TREE_DID_CLEAN (0/1), CLEAN_TREE_FREED_KB,
# and CLEAN_TREE_METHOD ("trash"|"rm"). Never touches <project>/bin — that's
# where oF projects keep bin/data (assets) alongside the built binary, so it's
# left alone entirely. The project's Makefile is never run (its clean target
# could delete bin/), so nothing under bin/ is removed by anything here.
cleanProjectTree(){
	local project="$1"
	CLEAN_TREE_DID_CLEAN=0
	CLEAN_TREE_FREED_KB=0
	CLEAN_TREE_FAILED=0
	CLEAN_TREE_METHOD="rm"
	[[ -d "$project" ]] || return 1

	# nothing built here — skip make/find entirely
	projectHasBuildArtifacts "$project" || return 0

	cleanProjectTargets "$project"
	local -a targets=("${CLEAN_TARGETS[@]}")
	local -a deepTargets=()
	[[ ${#CLEAN_DEEP[@]} -gt 0 ]] && deepTargets=("${CLEAN_DEEP[@]}")

	# size each path first, so afterwards only what is really gone counts as freed
	local -a allPaths=("${targets[@]}")
	[[ ${#deepTargets[@]} -gt 0 ]] && allPaths+=("${deepTargets[@]}")
	local -a sizes=()
	local p freedKB=0 failed=0 i
	for p in "${allPaths[@]}"; do
		sizes+=("$(pathsSizeKB "$p")")
	done

	# Deliberately no `make clean`: a project Makefile's clean target is arbitrary
	# code and may delete bin/ (bin/data holds the app's assets). Removing obj/
	# and the other build folders below is what `make clean` would do anyway.

	trashPaths "${allPaths[@]}"
	CLEAN_TREE_METHOD="$TRASH_METHOD"

	for i in "${!allPaths[@]}"; do
		p="${allPaths[$i]}"
		if [[ "$TRASH_METHOD" == "dry-run" ]]; then
			freedKB=$((freedKB + sizes[i]))
		elif [[ -e "$p" || -L "$p" ]]; then
			failed=$((failed + 1))     # still there: trash/rm failed or was refused
		else
			freedKB=$((freedKB + sizes[i]))
		fi
	done

	CLEAN_TREE_DID_CLEAN=1
	CLEAN_TREE_FREED_KB=$freedKB
	CLEAN_TREE_FAILED=$failed
	return 0
}

# Clean one project dir and print its ✓/– result line
cleanProjectsScopeOne(){
	local proj="$1" rel="$2"
	cleanProjectTree "$proj"
	if [[ "$CLEAN_TREE_DID_CLEAN" == "1" ]]; then
		local verb="freed"
		[[ "$CLEAN_TREE_METHOD" == "trash" ]] && verb="moved to Trash"
		[[ "$CLEAN_TREE_METHOD" == "dry-run" ]] && verb="would free"
		local mark="${C_OK}✓" extra=""
		if [[ "${CLEAN_TREE_FAILED:-0}" -gt 0 ]]; then
			mark="${C_ERR}✗"
			extra=" · ${CLEAN_TREE_FAILED} path(s) could not be removed"
			cleanRecord failed "$proj" "$CLEAN_TREE_FREED_KB" "${CLEAN_TREE_FAILED} path(s) could not be removed"
		elif [[ "$CLEAN_TREE_METHOD" == "dry-run" ]]; then
			cleanRecord would_remove "$proj" "$CLEAN_TREE_FREED_KB"
		else
			cleanRecord removed "$proj" "$CLEAN_TREE_FREED_KB" "$CLEAN_TREE_METHOD"
		fi
		printf '  %s%s %s%s%s  %s(%s %s%s)%s\n' \
			"$mark" "$C_RESET" "$C_FG" "$(cleanShow "$rel")" "$C_RESET" "$C_MUTED" "$(formatKB "$CLEAN_TREE_FREED_KB")" "$verb" "$extra" "$C_RESET"
	else
		cleanRecord skipped "$proj" 0 nothing_to_clean
		printf '  %s–%s %s%s%s  %s(nothing to clean)%s\n' \
			"$C_MUTED" "$C_RESET" "$C_FG" "$(cleanShow "$rel")" "$C_RESET" "$C_MUTED" "$C_RESET"
	fi
}

# Walk apps/ or examples/ (or a custom root) and clean each project-like folder
cleanProjectsScopeRun(){
	local scope="$1"   # apps|examples|all|path
	local path="${2:-}"
	local root cleaned=0 skipped=0 totalFreedKB=0 failedPaths=0 sawTrash=0
	local -a roots=()

	ofRootIsSane || { echoErrorCode invalid_tree "$(t msg.dir_not_openframeworks_tree_refusing_clean "OF_DIR is not an openFrameworks tree (%s) — refusing to clean" "${OF_DIR:-unset}")"; return 1; }

	case "$scope" in
		apps) roots+=("${OF_DIR}/apps") ;;
		examples) roots+=("${OF_DIR}/examples") ;;
		all) roots+=("${OF_DIR}/apps" "${OF_DIR}/examples") ;;
		path)
			[[ -n "$path" ]] || { echoErrorCode invalid_argument "$(t msg.path_required_folder_inside "path required (a folder inside %s)" "${OF_DIR}")"; return 1; }
			# a relative path is taken relative to the openFrameworks folder
			[[ "$path" == /* || "$path" == ?:* ]] || path="${OF_DIR}/${path}"
			[[ -d "$path" ]] || { echoErrorCode invalid_argument "$(t msg.not_folder "not a folder: %s" "$path")"; return 1; }
			if ! cleanScanRootIsSafe "$path"; then
				echoErrorCode invalid_argument "$(t msg.path_must_be_folder_inside_not "path must be a folder inside %s (not the root, .git, libs or scripts): %s" "${OF_DIR}" "$path")"
				return 1
			fi
			path="$(cd -- "$path" && pwd -P)"
			roots+=("$path")
			;;
		*) echoErrorCode invalid_argument "$(t msg.scope_apps_examples_all_path "scope: apps|examples|all|path")"; return 1 ;;
	esac

	printBanner "clean"
	echoInfo "$(t msg.cleanup_projects "cleanup projects · %s%s" "${scope}" "${path:+ · $path}")"
	echoNote "$(t msg.bin_incl_bin_data_never_touched "bin/ (incl. bin/data) is never touched — only obj/build/.vs/etc.")"
	printf '\n'

	local proj rel mode=""
	[[ "$scope" == "path" ]] && mode="self"
	for root in "${roots[@]}"; do
		while IFS= read -r -d '' proj; do
			[[ -n "$proj" ]] || continue
			rel="${proj#"$OF_DIR"/}"
			cleanProjectsScopeOne "$proj" "$rel"
			if [[ "$CLEAN_TREE_DID_CLEAN" == "1" ]]; then
				cleaned=$((cleaned + 1))
				totalFreedKB=$((totalFreedKB + CLEAN_TREE_FREED_KB))
				failedPaths=$((failedPaths + CLEAN_TREE_FAILED))
				[[ "$CLEAN_TREE_METHOD" == "trash" ]] && sawTrash=1
			else
				skipped=$((skipped + 1))
			fi
		done < <(cleanFindProjects "$root" "$mode")
	done
	printf '\n'
	if [[ "$OF_CLEAN_DRY_RUN" == "1" ]]; then
		echoSuccess "$(t msg.dry_run_would_clean_project_s "dry run: would clean %s project(s) · skipped %s (nothing to clean) · would free %s" "${cleaned}" "${skipped}" "$(formatKB "$totalFreedKB")")"
	elif [[ $sawTrash -eq 1 ]]; then
		echoSuccess "$(t msg.cleaned_project_s_skipped_nothing_clean "cleaned %s project(s) · skipped %s (nothing to clean) · moved %s to the Trash (disk space returns when it is emptied)" "${cleaned}" "${skipped}" "$(formatKB "$totalFreedKB")")"
	else
		echoSuccess "$(t msg.cleaned_project_s_skipped_nothing_clean_2 "cleaned %s project(s) · skipped %s (nothing to clean) · freed %s" "${cleaned}" "${skipped}" "$(formatKB "$totalFreedKB")")"
	fi
	if [[ $failedPaths -gt 0 ]]; then
		echoErrorCode partial_failure "$(t msg.path_s_could_not_be_removed "%s path(s) could not be removed (permissions, or in use) — see the lines marked ✗" "${failedPaths}")"
		return 1
	fi
}

cleanDownloadCachesRun(){
	local mode="${1:-packages}" # packages|all
	local dl="${OF_DIR}/libs/download"
	local freedKB=0
	ofRootIsSane || { echoErrorCode invalid_tree "$(t msg.dir_not_openframeworks_tree_refusing_clean "OF_DIR is not an openFrameworks tree (%s) — refusing to clean" "${OF_DIR:-unset}")"; return 1; }
	printBanner "clean"
	echoInfo "$(t msg.clean_caches "clean caches · %s" "${mode}")"
	[[ "$OF_CLEAN_DRY_RUN" == "1" ]] && echoNote "$(t msg.dry_run_nothing_will_be_deleted "dry run — nothing will be deleted")"
	printf '\n'
	if [[ ! -d "$dl" ]]; then
		echoNote "$(t msg.no_libs_download_cache "no libs/download cache")"
		return 0
	fi
	case "$mode" in
		packages)
			# package archives + sidecars; keep folder
			local -a files=()
			local f n=0
			while IFS= read -r -d '' f; do
				[[ -f "$f" || -L "$f" ]] && files+=("$f")
			done < <(find "$dl" \( -type f -o -type l \) \( \
				-name '*.tar.bz2' -o -name '*.tar.gz' -o -name '*.zip' -o \
				-name '*.sha256' -o -name 'SHA256SUMS' -o -name '.last-verify' \
				\) -print0 2>/dev/null)
			freedKB=0
			local fKB failed=0
			for f in "${files[@]}"; do
				fKB=$(pathsSizeKB "$f")
				if safeRemovePath "$f"; then
					cleanRecord "$([[ "$OF_CLEAN_DRY_RUN" == "1" ]] && echo would_remove || echo removed)" "$f" "$fKB"
					n=$((n + 1))
					freedKB=$((freedKB + fKB))
				else
					failed=$((failed + 1))
					cleanRecord failed "$f" "$fKB" "could not remove"
					echoWarning "$(t msg.could_not_remove "could not remove %s" "${f#"$OF_DIR"/}")"
				fi
			done
			if [[ "$OF_CLEAN_DRY_RUN" == "1" ]]; then
				echoSuccess "$(t msg.dry_run_would_remove_cached_package "dry run: would remove %s cached package file(s) under libs/download · would free %s" "${n}" "$(formatKB "$freedKB")")"
			else
				echoSuccess "$(t msg.removed_cached_package_file_s_under "removed %s cached package file(s) under libs/download · freed %s%s" "${n}" "$(formatKB "$freedKB")" "$([[ $failed -gt 0 ]] && printf ' · %s failed' "$failed")")"
			fi
			[[ $failed -eq 0 ]] || return 1
			;;
		all)
			echoWarning "$(t msg.removing_entire_libs_download_compiled_intermediates "removing entire libs/download/ and compiled intermediates")"
			confirmDestructive "$(t msg.delete_everything_libs_download_openframeworkscompiled_obj "Delete everything in libs/download and the openFrameworksCompiled obj/intermediates?")" || return $CLEAN_ABORT_RC
			freedKB=$(pathsSizeKB "$dl")
			if safeRemovePath "$dl"; then
				cleanRecord "$([[ "$OF_CLEAN_DRY_RUN" == "1" ]] && echo would_remove || echo removed)" "$dl" "$freedKB"
				[[ "$OF_CLEAN_DRY_RUN" == "1" ]] || mkdir -p "$dl"
				if [[ "$OF_CLEAN_DRY_RUN" == "1" ]]; then
					echoSuccess "$(t msg.dry_run_would_clear_libs_download "dry run: would clear libs/download · would free %s" "$(formatKB "$freedKB")")"
				else
					echoSuccess "$(t msg.libs_download_cleared_freed "libs/download cleared · freed %s" "$(formatKB "$freedKB")")"
				fi
			else
				echoErrorCode partial_failure "$(t msg.could_not_clear_libs_download "could not clear libs/download")"
				return 1
			fi
			;;
		*) echoErrorCode invalid_argument "$(t msg.mode_packages_all "mode: packages|all")"; return 1 ;;
	esac
	# optional compile junk under openFrameworksCompiled
	if [[ "$mode" == "all" ]]; then
		local -a junk=()
		while IFS= read -r -d '' f; do
			[[ -d "$f" ]] && junk+=("$f")
		done < <(find "${OF_DIR}/libs/openFrameworksCompiled" -type d \( -name 'obj' -o -name 'intermediates' \) -print0 2>/dev/null)
		if [[ ${#junk[@]} -gt 0 ]]; then
			local junkKB
			junkKB=$(pathsSizeKB "${junk[@]}")
			local j
			local jKB
			for j in "${junk[@]}"; do
				jKB=$(pathsSizeKB "$j")
				if safeRemovePath "$j"; then
					cleanRecord "$([[ "$OF_CLEAN_DRY_RUN" == "1" ]] && echo would_remove || echo removed)" "$j" "$jKB"
				else
					cleanRecord failed "$j" "$jKB" "could not remove"
					echoWarning "$(t msg.could_not_remove "could not remove %s" "${j#"$OF_DIR"/}")"
				fi
			done
			echoNote "$(t msg.also_cleared_openframeworkscompiled_obj_intermediates "also cleared openFrameworksCompiled obj/intermediates · %s" "$(formatKB "$junkKB")")"
		fi
	fi
}

# Delete one lib folder through safeRemovePath and print its result line.
# Updates the caller's count / failed / totalFreedKB (dynamic scope).
cleanLibRemoveOne(){
	local d="$1" label dKB verb="freed"
	label="$(cleanShow "$2")"
	dKB=$(pathsSizeKB "$d")
	[[ "$OF_CLEAN_DRY_RUN" == "1" ]] && verb="would free"
	if safeRemovePath "$d"; then
		cleanRecord "$([[ "$OF_CLEAN_DRY_RUN" == "1" ]] && echo would_remove || echo removed)" "$d" "$dKB"
		totalFreedKB=$((totalFreedKB + dKB))
		count=$((count + 1))
		printf '  %s✓%s %s  %s(%s %s)%s\n' "$C_OK" "$C_RESET" "$label" "$C_MUTED" "$(formatKB "$dKB")" "$verb" "$C_RESET"
	else
		failed=$((failed + 1))
		cleanRecord failed "$d" "$dKB" "could not remove"
		printf '  %s✗%s %s  %s(could not remove)%s\n' "$C_ERR" "$C_RESET" "$label" "$C_MUTED" "$C_RESET"
	fi
}

# Remove prebuilt library binaries (minimise disk) — never deletes openFrameworks source
cleanLibrariesRun(){
	local mode="$1" # other-platforms|platform|all-prebuilt|list
	local plat="${2:-}"
	local libDir d name p count=0 skipped=0 failed=0 totalFreedKB=0 dKB
	local doneWord="removed" totalWord="freed"
	[[ "$OF_CLEAN_DRY_RUN" == "1" ]] && { doneWord="would remove"; totalWord="would free"; }

	ofRootIsSane || { echoErrorCode invalid_tree "$(t msg.dir_not_openframeworks_tree_refusing_clean "OF_DIR is not an openFrameworks tree (%s) — refusing to clean" "${OF_DIR:-unset}")"; return 1; }
	if [[ "$mode" == "platform" || ( "$mode" == "other-platforms" && -n "$plat" ) ]]; then
		if ! cleanPlatformIsValid "$plat"; then
			echoErrorCode invalid_argument "$(t msg.unknown_platform "unknown platform '%s'" "${plat}")"
			echoNote "$(t msg.valid "valid: %s" "${OF_CLEAN_PLATFORMS}")"
			return 1
		fi
	fi

	printBanner "clean"
	echoInfo "$(t msg.libraries "libraries · %s%s" "${mode}" "${plat:+ · $plat}")"
	[[ "$OF_CLEAN_DRY_RUN" == "1" ]] && echoNote "$(t msg.dry_run_nothing_will_be_deleted "dry run — nothing will be deleted")"
	printf '\n'

	case "$mode" in
		list)
			echoNote "$(t msg.installed_lib_platform_folders "installed lib/<platform> folders:")"
			find "${OF_DIR}/libs" "${OF_DIR}/addons" -mindepth 3 -maxdepth 4 -type d -path '*/lib/*' 2>/dev/null \
				| sed "s|^${OF_DIR}/||" | sort -u | head -80
			return 0
			;;
		other-platforms)
			# Keep one platform (the host's unless one is given) and remove the *known*
			# others. Folders this script does not recognise are left alone.
			if [[ -n "$plat" ]]; then cleanKeepSetFromSpec "$plat"; else cleanHostKeepSet; fi
			echoKV "$(t msg.keep "keep")" "${plat:-$CLEAN_KEEP_FAMS}"
			[[ -z "$plat" && -n "$CLEAN_KEEP_SUBS_linux" ]] && echoKV "$(t msg.keep_linux "keep linux/")" "${CLEAN_KEEP_SUBS_linux}"
			[[ -z "$plat" && -n "$CLEAN_KEEP_SUBS_msys2" ]] && echoKV "$(t msg.keep_msys2 "keep msys2/")" "${CLEAN_KEEP_SUBS_msys2}"
			CLEAN_CAND=()
			while IFS= read -r -d '' d; do
				cleanOtherShouldRemove "$d" && CLEAN_CAND+=("$d")
			done < <(cleanEnumPlatformDirs)
			if [[ "$(cleanCandRemovable)" == "0" ]]; then
				echoSuccess "$(t msg.no_other_platforms_remove "no other platforms to remove")"
				[[ ${#CLEAN_CAND[@]} -gt 0 ]] && echoNote "$(t msg.git_tracked_folder_s_kept "%s git-tracked folder(s) are kept" "${#CLEAN_CAND[@]}")"
				return 0
			fi
			confirmDestructive "$(t msg.delete_every_prebuilt_lib_platform_folder "Delete every prebuilt lib/<platform> folder except: %s ?" "${plat:-${CLEAN_KEEP_FAMS// /, }}")" || return $CLEAN_ABORT_RC
			cleanLibRemoveList
			printf '\n'
			echoSuccess "$(t msg.non_host_platform_lib_folder_s "%s %s non-host platform lib folder(s) · skipped %s (git-tracked) · %s failed · %s %s" "${doneWord}" "${count}" "${skipped}" "${failed}" "${totalWord}" "$(formatKB "$totalFreedKB")")"
			;;
		platform)
			# exact folder name (validated above), directly under a lib/ folder
			CLEAN_CAND=()
			while IFS= read -r -d '' d; do CLEAN_CAND+=("$d"); done \
				< <(find "${OF_DIR}/libs" "${OF_DIR}/addons" \( -type d -o -type l \) -path "*/lib/${plat}" -print0 2>/dev/null)
			if [[ "$(cleanCandRemovable)" == "0" ]]; then
				echoSuccess "$(t msg.no_lib_folders_remove "no lib/%s folders to remove" "${plat}")"
				return 0
			fi
			confirmDestructive "$(t msg.delete_every_lib_folder_under_libs "Delete every lib/%s folder under libs/ and addons/?" "${plat}")" || return $CLEAN_ABORT_RC
			cleanLibRemoveList
			printf '\n'
			echoSuccess "$(t msg.path_s_for_platform_skipped_git "%s %s path(s) for platform %s · skipped %s (git-tracked) · %s failed · %s %s" "${doneWord}" "${count}" "${plat}" "${skipped}" "${failed}" "${totalWord}" "$(formatKB "$totalFreedKB")")"
			;;
		all-prebuilt)
			echoWarning "$(t msg.this_deletes_downloaded_prebuilts_under_libs "This deletes downloaded prebuilts under libs/* and addon libs binaries.")"
			echoNote "$(t msg.folders_with_no_library_binaries_only "folders with no library binaries (only .gitkeep, headers or source) are left alone")"
			echoWarning "$(t msg.keeps_libs_openframeworks_libs_openframeworkscompiled_source "Keeps: libs/openFrameworks, libs/openFrameworksCompiled (source/project).")"
			echoWarning "$(t msg.also_keeps_anything_git_tracked_vendored "Also keeps anything git-tracked (vendored addon source, e.g. ofxKinect/libs/libfreenect).")"
			CLEAN_CAND=()
			for d in "${OF_DIR}/libs"/*; do
				[[ -d "$d" ]] || continue
				name=$(basename "$d")
				case "$name" in openFrameworks|openFrameworksCompiled|download|scripts) continue ;; esac
				cleanHasLibBinaries "$d" && CLEAN_CAND+=("$d")     # not .gitkeep / headers / source only
			done
			for d in "${OF_DIR}/addons"/*/libs; do
				[[ -d "$d" ]] || continue
				cleanHasLibBinaries "$d" && CLEAN_CAND+=("$d")
			done
			if [[ "$(cleanCandRemovable)" == "0" ]]; then
				echoSuccess "$(t msg.no_prebuilt_libraries_remove "no prebuilt libraries to remove")"
				return 0
			fi
			confirmDestructive "$(t msg.delete_all_prebuilt_libraries_libs_addons "Delete ALL prebuilt libraries (libs/* and addons/*/libs)? You will need to re-download them.")" || return $CLEAN_ABORT_RC
			cleanLibRemoveList
			[[ "$OF_CLEAN_DRY_RUN" == "1" ]] || rm -f "${OF_DIR}/libs/.of-cli-state" 2>/dev/null || true
			printf '\n'
			echoSuccess "$(t msg.prebuilt_tree_s_skipped_git_tracked "%s %s prebuilt tree(s) · skipped %s (git-tracked) · %s failed · %s %s — run Update libs to restore" "${doneWord}" "${count}" "${skipped}" "${failed}" "${totalWord}" "$(formatKB "$totalFreedKB")")"
			;;
		*) echoErrorCode invalid_argument "$(t msg.mode_list_other_platforms_platform_all "mode: list|other-platforms|platform|all-prebuilt")"; return 1 ;;
	esac
	[[ $failed -eq 0 ]]
}

# --- core build outputs ------------------------------------------------------
# libs/openFrameworksCompiled/lib/<platform>[/<arch>] holds the compiled core
# (make, Xcode and the CMake build all write it; a CMake app build reuses it).
# Removing it only costs a rebuild. `of cleanup core` removes the host's;
# `of cleanup core all` removes every known platform's.
OF_CORE_LIB_DIR_REL="libs/openFrameworksCompiled/lib"

cleanCoreBuildsRun(){
	local mode="${1:-host}" d name fam sub keepSubs
	local count=0 skipped=0 failed=0 totalFreedKB=0
	local doneWord="removed" totalWord="freed"
	[[ "$OF_CLEAN_DRY_RUN" == "1" ]] && { doneWord="would remove"; totalWord="would free"; }
	case "$mode" in host|all) ;; *) echoErrorCode invalid_argument "$(t msg.usage_cleanup_core_host_all "usage: of cleanup core [host|all]")"; return 1 ;; esac
	ofRootIsSane || { echoErrorCode invalid_tree "$(t msg.dir_not_openframeworks_tree_refusing_clean "OF_DIR is not an openFrameworks tree (%s) — refusing to clean" "${OF_DIR:-unset}")"; return 1; }

	printBanner "clean"
	echoInfo "$(t msg.core_build "core build · %s" "${mode}")"
	[[ "$OF_CLEAN_DRY_RUN" == "1" ]] && echoNote "$(t msg.dry_run_nothing_will_be_deleted "dry run — nothing will be deleted")"
	echoNote "$(t msg.platform_arch_next_build_recompiles_core "%s/<platform>[/<arch>] — the next build recompiles the core" "${OF_CORE_LIB_DIR_REL}")"
	printf '\n'
	cleanHostKeepSet
	[[ "$mode" == "host" ]] && echoKV "$(t msg.host "host")" "${CLEAN_KEEP_FAMS}"
	# decide first, so there is nothing to confirm when nothing would go
	CLEAN_CAND=()
	while IFS= read -r -d '' d; do
		name=$(basename -- "$d")
		if cleanWordIn "$name" "$OF_CLEAN_FAMILIES"; then
			fam="$name"; sub=""
		else
			fam=$(basename -- "$(dirname -- "$d")"); sub="$name"
		fi
		if [[ "$mode" == "host" ]]; then
			cleanWordIn "$fam" "$CLEAN_KEEP_FAMS" || continue
			case "$fam" in linux) keepSubs="$CLEAN_KEEP_SUBS_linux" ;; msys2) keepSubs="$CLEAN_KEEP_SUBS_msys2" ;; *) keepSubs="" ;; esac
			if [[ -z "$sub" ]]; then
				# a family that has per-arch folders is trimmed to the host arch
				[[ -n "$keepSubs" ]] && continue
			else
				cleanWordIn "$sub" "$keepSubs" || continue
			fi
		else
			[[ -z "$sub" ]] || continue      # `all` removes whole families
		fi
		CLEAN_CAND+=("$d")
	done < <(cleanEnumPlatformDirs "${OF_DIR}/${OF_CORE_LIB_DIR_REL}")
	if [[ "$(cleanCandRemovable)" == "0" ]]; then
		echoSuccess "$(t msg.no_compiled_core_remove "no compiled core to remove")"
		return 0
	fi
	if [[ "$mode" == "host" ]]; then
		confirmDestructive "$(t msg.delete_compiled_core_for_this_host "Delete the compiled core for this host (%s%s)? It is rebuilt by the next build." "${OF_PLATFORM}" "${OF_ARCH:+/$OF_ARCH}")" || return $CLEAN_ABORT_RC
	else
		confirmDestructive "$(t msg.delete_compiled_core_for_every_platform "Delete the compiled core for EVERY platform under %s?" "${OF_CORE_LIB_DIR_REL}")" || return $CLEAN_ABORT_RC
	fi
	cleanLibRemoveList
	printf '\n'
	echoSuccess "$(t msg.core_build_folder_s_skipped_git "%s %s core build folder(s) · skipped %s (git-tracked) · %s failed · %s %s" "${doneWord}" "${count}" "${skipped}" "${failed}" "${totalWord}" "$(formatKB "$totalFreedKB")")"
	[[ $failed -eq 0 ]]
}

# --- concurrency -----------------------------------------------------------
# One cleanup at a time (mkdir is atomic), and none while a build in this tree
# is running. A lock whose owner is gone (killed, Ctrl-C) is reclaimed.
OF_CLEAN_FORCE="${OF_CLEAN_FORCE:-0}"       # --force: clean even if a build looks active
OF_CLEAN_LOCK_DIR=""

# Print command lines of build tools that mention this tree. Heuristic: needs
# a POSIX `ps`; returns 1 (nothing found) where that is unavailable.
cleanBuildProcesses(){
	command -v ps >/dev/null 2>&1 || return 1
	local line rest pid cmd exe found=1
	while IFS= read -r line; do
		line="${line#"${line%%[![:space:]]*}"}"
		pid="${line%%[[:space:]]*}"
		cmd="${line#"$pid"}"; cmd="${cmd#"${cmd%%[![:space:]]*}"}"
		[[ "$pid" =~ ^[0-9]+$ ]] || continue
		[[ "$pid" == "$$" || "$pid" == "$PPID" ]] && continue
		# OF_DIR is a real path; a build started through a symlinked path
		# (macOS /var -> /private/var, /tmp) shows the other spelling
		[[ "$cmd" == *"$OF_DIR"* || "$cmd" == *"${OF_DIR#/private}"* ]] || continue
		exe="${cmd%%[[:space:]]*}"; exe="${exe##*[/\\]}"
		case "$exe" in
			make|gmake|mingw32-make|xcodebuild|cmake|ninja|msbuild|MSBuild.exe|devenv.exe|clang|clang++|gcc|g++|cc1plus|ld|ar|ranlib|swiftc|swift-frontend)
				printf '%s\n' "${pid} ${cmd}"
				found=0
				;;
		esac
	done < <(ps -axo pid=,command= 2>/dev/null)
	return $found
}

cleanBuildGuard(){
	local procs
	procs="$(cleanBuildProcesses)" || return 0
	echoWarning "$(t msg.build_looks_active_this_tree "a build looks active in this tree:")"
	printf '%s\n' "$procs" | head -3 | while IFS= read -r l; do printf '    %s\n' "$(cleanShow "${l:0:140}")"; done
	[[ "$OF_CLEAN_FORCE" == "1" ]] && { echoNote "$(t msg.force_given_continuing "--force given — continuing")"; return 0; }
	if menuCanRun && [[ "$OF_CLEAN_YES" != "1" ]]; then
		confirmNo "$(t msg.clean_anyway_deleting_objects_libs_under "Clean anyway? Deleting objects or libs under a running build can break it.")" && return 0
		echoInfo "$(t msg.cancelled_nothing_deleted "cancelled — nothing deleted")"
		CLEAN_ABORT_RC=0
		return 1
	fi
	echoErrorCode build_active "$(t msg.refusing_clean_while_build_running_wait "refusing to clean while a build is running — wait for it, or re-run with --force")"
	CLEAN_ABORT_RC=1
	return 1
}

cleanLockAcquire(){
	local dir="${OF_DIR}/libs/.of-clean.lock" pid
	if mkdir "$dir" 2>/dev/null; then
		printf '%s\n' "$$" > "$dir/pid" 2>/dev/null
		OF_CLEAN_LOCK_DIR="$dir"
		return 0
	fi
	pid="$(cat "$dir/pid" 2>/dev/null)"
	if [[ "$pid" =~ ^[0-9]+$ ]] && kill -0 "$pid" 2>/dev/null; then
		echoErrorCode busy "$(t msg.another_cleanup_already_running_pid_try "another cleanup is already running (pid %s) — try again when it finishes" "${pid}")"
		return 1
	fi
	# owner is gone: reclaim, but only if the lock is still the one we looked at
	if [[ "$(cat "$dir/pid" 2>/dev/null)" == "$pid" ]]; then
		rm -rf -- "$dir"
	fi
	if mkdir "$dir" 2>/dev/null; then
		printf '%s\n' "$$" > "$dir/pid" 2>/dev/null
		OF_CLEAN_LOCK_DIR="$dir"
		return 0
	fi
	echoErrorCode busy "$(t msg.could_not_take_cleanup_lock "could not take the cleanup lock (%s)" "${dir}")"
	return 1
}

cleanLockRelease(){
	[[ -n "$OF_CLEAN_LOCK_DIR" ]] && rm -rf -- "$OF_CLEAN_LOCK_DIR"
	OF_CLEAN_LOCK_DIR=""
}

# cleanLocked fn args… — run a deleting command under the lock and build guard
# (a dry run changes nothing, so it needs neither).
cleanLocked(){
	local fn="$1" rc
	shift
	if [[ "$OF_CLEAN_DRY_RUN" != "1" ]]; then
		ofRootIsSane || { echoErrorCode invalid_tree "$(t msg.dir_not_openframeworks_tree_refusing_clean "OF_DIR is not an openFrameworks tree (%s) — refusing to clean" "${OF_DIR:-unset}")"; return 1; }
		cleanBuildGuard || return $CLEAN_ABORT_RC
		cleanLockAcquire || return 1
	fi
	"$fn" "$@"
	rc=$?
	cleanLockRelease
	return $rc
}

cleanProjectsScope(){ cleanLocked cleanProjectsScopeRun "$@"; }
cleanDownloadCaches(){ cleanLocked cleanDownloadCachesRun "$@"; }
cleanCoreBuilds(){ cleanLocked cleanCoreBuildsRun "$@"; }
cleanLibraries(){
	if [[ "${1:-}" == "list" ]]; then cleanLibrariesRun "$@"; else cleanLocked cleanLibrariesRun "$@"; fi
}

# ---------------------------------------------------------------------------
# Disk usage report:  of cleanup usage [projects|caches|libs|all] [--json|--tsv]
# Reports how much each cleanable path holds, without deleting anything.
# Sizes are disk usage in KiB (du -k). Rows nested inside another row's path
# ("nested": true) are not added to that category's total.
# ---------------------------------------------------------------------------
OF_CLEAN_FORMAT="${OF_CLEAN_FORMAT:-human}"   # human|json|tsv
CLEAN_US=$'\037'                               # row field separator
CLEAN_USAGE_ROWS=()

# Printable form of a path for terminal output: control characters (a newline
# in a file name, escape sequences) become "?". UTF-8 names (Chinese etc.) are
# left alone.
cleanShow(){
	if [[ "$1" == *[[:cntrl:]]* ]]; then
		printf '%s' "$1" | LC_ALL=C tr '\000-\037\177' '?'
	else
		printf '%s' "$1"
	fi
}

# Split one usage row (fields joined by $CLEAN_US) into the caller's
# c kind path kb removable tracked nested plat cmd. Plain expansions, not
# `read`, so a newline inside a path cannot truncate the row.
cleanRowParse(){
	local r="$1"
	c="${r%%"$CLEAN_US"*}";         r="${r#*"$CLEAN_US"}"
	kind="${r%%"$CLEAN_US"*}";      r="${r#*"$CLEAN_US"}"
	path="${r%%"$CLEAN_US"*}";      r="${r#*"$CLEAN_US"}"
	kb="${r%%"$CLEAN_US"*}";        r="${r#*"$CLEAN_US"}"
	removable="${r%%"$CLEAN_US"*}"; r="${r#*"$CLEAN_US"}"
	tracked="${r%%"$CLEAN_US"*}";   r="${r#*"$CLEAN_US"}"
	nested="${r%%"$CLEAN_US"*}";    r="${r#*"$CLEAN_US"}"
	plat="${r%%"$CLEAN_US"*}";      r="${r#*"$CLEAN_US"}"
	cmd="$r"
}

jsonEscape(){
	local s="$1"
	s="${s//\\/\\\\}"
	s="${s//\"/\\\"}"
	s="${s//$'\t'/\\t}"
	s="${s//$'\n'/\\n}"
	s="${s//$'\r'/\\r}"
	# any other control character would make the JSON invalid
	[[ "$s" == *[[:cntrl:]]* ]] && s="$(printf '%s' "$s" | LC_ALL=C tr '\000-\037\177' '?')"
	printf '%s' "$s"
}

# cleanUsageAdd category kind path kb removable tracked nested platform command
cleanUsageAdd(){
	local rel="${3#"$OF_DIR"/}"
	CLEAN_USAGE_ROWS+=("${1}${CLEAN_US}${2}${CLEAN_US}${rel}${CLEAN_US}${4}${CLEAN_US}${5}${CLEAN_US}${6}${CLEAN_US}${7}${CLEAN_US}${8}${CLEAN_US}${9}")
}

cleanUsageCollect(){
	local what="${1:-all}" d name rel kb tracked removable plat n
	CLEAN_USAGE_ROWS=()

	if [[ "$what" == "all" || "$what" == "projects" ]]; then
		local root proj
		for root in "${OF_DIR}/apps" "${OF_DIR}/examples"; do
			while IFS= read -r -d '' proj; do
				[[ -n "$proj" ]] || continue
				projectHasBuildArtifacts "$proj" || continue
				cleanProjectTargets "$proj"
				local -a all=("${CLEAN_TARGETS[@]}")
				[[ ${#CLEAN_DEEP[@]} -gt 0 ]] && all+=("${CLEAN_DEEP[@]}")
				[[ ${#all[@]} -gt 0 ]] || continue
				kb=$(pathsSizeKB "${all[@]}")
				removable=false
				cleanPathIsSafe "${all[0]}" && removable=true
				cleanUsageAdd projects project "$proj" "$kb" "$removable" false false "" "of cleanup projects ${proj#"$OF_DIR"/}"
			done < <(cleanFindProjects "$root")
		done
	fi

	if [[ "$what" == "all" || "$what" == "caches" ]]; then
		local dl="${OF_DIR}/libs/download" f
		if [[ -d "$dl" ]]; then
			local -a files=()
			while IFS= read -r -d '' f; do
				[[ -f "$f" || -L "$f" ]] && files+=("$f")
			done < <(find "$dl" \( -type f -o -type l \) \( -name '*.tar.bz2' -o -name '*.tar.gz' -o -name '*.zip' \
				-o -name '*.sha256' -o -name 'SHA256SUMS' -o -name '.last-verify' \) -print0 2>/dev/null)
			kb=0
			[[ ${#files[@]} -gt 0 ]] && kb=$(pathsSizeKB "${files[@]}")
			cleanUsageAdd caches packages "$dl" "$kb" true false true "" "of cleanup caches packages"
			cleanUsageAdd caches download "$dl" "$(pathsSizeKB "$dl")" true false false "" "of cleanup caches all"
		fi
		while IFS= read -r -d '' d; do
			[[ -d "$d" ]] || continue
			cleanUsageAdd caches intermediates "$d" "$(pathsSizeKB "$d")" true false false "" "of cleanup caches all"
		done < <(find "${OF_DIR}/libs/openFrameworksCompiled" -type d \( -name 'obj' -o -name 'intermediates' \) -print0 2>/dev/null)
	fi

	if [[ "$what" == "all" || "$what" == "libs" ]]; then
		# prebuilt trees (what `libs all` deletes)
		for d in "${OF_DIR}/libs"/* "${OF_DIR}/addons"/*/libs; do
			[[ -d "$d" ]] || continue
			name=$(basename "$d")
			case "$d" in
				"${OF_DIR}/libs/"*)
					case "$name" in openFrameworks|openFrameworksCompiled|download|scripts) continue ;; esac ;;
			esac
			cleanHasLibBinaries "$d" || continue     # .gitkeep / source only: not a prebuilt tree
			tracked=false; pathIsGitTracked "$d" && tracked=true
			removable=false
			[[ "$tracked" == false ]] && cleanPathIsSafe "$d" && removable=true
			cleanUsageAdd libs prebuilt "$d" "$(pathsSizeKB "$d")" "$removable" "$tracked" false "" "of cleanup libs all"
		done
		# lib/<platform> folders inside them (what `libs <platform>` deletes);
		# the compiled core has its own category
		while IFS= read -r -d '' d; do
			[[ "$d" == "${OF_DIR}/${OF_CORE_LIB_DIR_REL}/"* ]] && continue
			name=$(basename "$d")
			plat="$name"
			cleanWordIn "$name" "$OF_CLEAN_FAMILIES" || plat="$(basename -- "$(dirname -- "$d")")/$name"
			cleanPlatformIsValid "$plat" || plat=""
			tracked=false; pathIsGitTracked "$d" && tracked=true
			removable=false
			[[ "$tracked" == false && -n "$plat" ]] && cleanPathIsSafe "$d" && removable=true
			cleanUsageAdd libs platform "$d" "$(pathsSizeKB "$d")" "$removable" "$tracked" true "$plat" "${plat:+of cleanup libs $plat}"
		done < <(cleanEnumPlatformDirs)
	fi

	if [[ "$what" == "all" || "$what" == "core" ]]; then
		cleanHostKeepSet
		local host fam sub keepSubs
		while IFS= read -r -d '' d; do
			name=$(basename "$d")
			if cleanWordIn "$name" "$OF_CLEAN_FAMILIES"; then fam="$name"; sub=""; else fam=$(basename -- "$(dirname -- "$d")"); sub="$name"; fi
			tracked=false; pathIsGitTracked "$d" && tracked=true
			plat="$name"; [[ -n "$sub" ]] && plat="$fam/$sub"
			removable=false; [[ "$tracked" == false ]] && cleanPathIsSafe "$d" && removable=true
			# "host" marks what `of cleanup core` (without `all`) removes
			host=all
			if cleanWordIn "$fam" "$CLEAN_KEEP_FAMS"; then
				case "$fam" in linux) keepSubs="$CLEAN_KEEP_SUBS_linux" ;; msys2) keepSubs="$CLEAN_KEEP_SUBS_msys2" ;; *) keepSubs="" ;; esac
				if [[ -z "$sub" ]]; then [[ -z "$keepSubs" ]] && host=host
				else cleanWordIn "$sub" "$keepSubs" && host=host; fi
			fi
			cleanUsageAdd core "$host" "$d" "$(pathsSizeKB "$d")" "$removable" "$tracked" "$([[ -n "$sub" ]] && echo true || echo false)" "$plat" "of cleanup core $([[ "$host" == host ]] && echo host || echo all)"
		done < <(cleanEnumPlatformDirs "${OF_DIR}/${OF_CORE_LIB_DIR_REL}")
	fi
}

cleanUsageReport(){
	local what="${1:-all}" fmt="${OF_CLEAN_FORMAT:-human}" cat
	case "$what" in all|projects|caches|libs|core) ;; *) echoErrorCode invalid_argument "$(t msg.usage_cleanup_usage_projects_caches_libs "usage: of cleanup usage [projects|caches|libs|core|all] [--json|--tsv]")"; return 1 ;; esac
	ofRootIsSane || { echoErrorCode invalid_tree "$(t msg.dir_not_openframeworks_tree_refusing "OF_DIR is not an openFrameworks tree (%s) — refusing" "${OF_DIR:-unset}")"; return 1; }

	cleanUsageCollect "$what"
	local -a sorted=()
	if [[ ${#CLEAN_USAGE_ROWS[@]} -gt 0 ]]; then
		while IFS= read -r -d '' row; do sorted+=("$row"); done < <(printf '%s\0' "${CLEAN_USAGE_ROWS[@]}" \
			| LC_ALL=C sort -z -t "$CLEAN_US" -k1,1 -k4,4nr)
	fi

	local -a cats=()
	[[ "$what" == "all" ]] && cats=(projects caches libs core) || cats=("$what")

	local row c kind path kb removable tracked nested plat cmd
	case "$fmt" in
	tsv)
		printf 'category\tkind\tpath\tkb\tremovable\tgit_tracked\tnested\tplatform\tcommand\n'
		for row in ${sorted[@]+"${sorted[@]}"}; do
			cleanRowParse "$row"
			path="$(cleanShow "${path//$'\t'/ }")"
			printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$c" "$kind" "$path" "$kb" "$removable" "$tracked" "$nested" "$plat" "$cmd"
		done
		;;
	json)
		printf '{\n  "of_dir": "%s",\n  "generated": "%s",\n  "unit": "KiB",\n  "categories": {' \
			"$(jsonEscape "$OF_DIR")" "$(date -u +%Y-%m-%dT%H:%M:%SZ)"
		local firstCat=1 firstItem total removableKB count
		for cat in "${cats[@]}"; do
			total=0; removableKB=0; count=0
			for row in ${sorted[@]+"${sorted[@]}"}; do
				cleanRowParse "$row"
				[[ "$c" == "$cat" ]] || continue
				count=$((count + 1))
				[[ "$nested" == "true" ]] && continue
				total=$((total + kb))
				[[ "$removable" == "true" ]] && removableKB=$((removableKB + kb))
			done
			[[ $firstCat -eq 1 ]] || printf ','
			firstCat=0
			printf '\n    "%s": {\n      "total_kb": %s,\n      "removable_kb": %s,\n      "count": %s,\n      "items": [' "$cat" "$total" "$removableKB" "$count"
			firstItem=1
			for row in ${sorted[@]+"${sorted[@]}"}; do
				cleanRowParse "$row"
				[[ "$c" == "$cat" ]] || continue
				[[ $firstItem -eq 1 ]] || printf ','
				firstItem=0
				printf '\n        {"kind": "%s", "path": "%s", "kb": %s, "removable": %s, "git_tracked": %s, "nested": %s, "platform": %s, "command": %s}' \
					"$kind" "$(jsonEscape "$path")" "$kb" "$removable" "$tracked" "$nested" \
					"$([[ -n "$plat" ]] && printf '"%s"' "$(jsonEscape "$plat")" || printf null)" \
					"$([[ -n "$cmd" ]] && printf '"%s"' "$(jsonEscape "$cmd")" || printf null)"
			done
			[[ $firstItem -eq 1 ]] && printf ']' || printf '\n      ]'
			printf '\n    }'
		done
		printf '\n  }\n}\n'
		;;
	*)
		printBanner "clean"
		echoInfo "$(t msg.disk_usage "disk usage · %s" "${what}")"
		printf '\n'
		local grand=0
		for cat in "${cats[@]}"; do
			total=0
			printf '%s%s%s\n' "$C_BOLD" "$cat" "$C_RESET"
			for row in ${sorted[@]+"${sorted[@]}"}; do
				cleanRowParse "$row"
				[[ "$c" == "$cat" ]] || continue
				local note=""
				[[ "$tracked" == "true" ]] && note=" git-tracked — kept"
				[[ "$nested" == "true" ]] && note="${note} (inside another entry)"
				printf '  %10s  %s%s%s%s%s%s\n' "$(formatKB "$kb")" "$C_FG" "$(cleanShow "$path")" "$C_RESET" "$C_MUTED" "$note" "$C_RESET"
				[[ "$nested" == "true" ]] || total=$((total + kb))
			done
			printf '  %s%10s  total%s\n\n' "$C_ACCENT" "$(formatKB "$total")" "$C_RESET"
			grand=$((grand + total))
		done
		echoNote "$(t msg.sizes_disk_usage_nested_entries_not "sizes are disk usage; nested entries are not double counted")"
		;;
	esac
}

# Pick a lib/<platform> name. Sets MENU_PLAT; returns 1 when cancelled.
# $1 title, $2 optional first entry "label|value" (e.g. the host).
menuPickLibPlatformSpec(){
	local -a opts=()
	[[ -n "${2:-}" ]] && opts+=("$2")
	opts+=("$(t menu.macos_osx "macos (+ osx)")|macos" "ios|ios" "tvos|tvos" "xros|xros"
		"android|android" "emscripten|emscripten" "vs|vs"
		"$(t menu.linux_all_architectures "linux (all architectures)")|linux" "linux/64|linux/64" "linux/arm64|linux/arm64"
		"linux/aarch64|linux/aarch64" "linux/armv6l|linux/armv6l" "linux/armv7l|linux/armv7l"
		"$(t menu.linux64_older_layout "linux64 (older layout)")|linux64" "$(t menu.linux64gcc6_older_layout "linux64gcc6 (older layout)")|linux64gcc6"
		"$(t menu.linuxarm64_older_layout "linuxarm64 (older layout)")|linuxarm64" "$(t menu.linuxaarch64_older_layout "linuxaarch64 (older layout)")|linuxaarch64"
		"$(t menu.linuxarmv6l_older_layout "linuxarmv6l (older layout)")|linuxarmv6l" "$(t menu.linuxarmv7l_older_layout "linuxarmv7l (older layout)")|linuxarmv7l"
		"$(t menu.msys2_all_environments "msys2 (all environments)")|msys2" "msys2/ucrt64|msys2/ucrt64" "msys2/clang64|msys2/clang64"
		"msys2/clangarm64|msys2/clangarm64" "msys2/mingw64|msys2/mingw64"
		"$(t menu.custom "Custom…")|custom")
	menuPick "$1" "${opts[@]}" || return 1
	MENU_PLAT="$UI_MENU_RESULT"
	if [[ "$MENU_PLAT" == "custom" ]]; then
		menuInput "$(t msg.lib_name_folder_e_g_linux "lib/<name> folder (e.g. linux/arm64, msys2/ucrt64)")" || return 1
		MENU_PLAT="$UI_INPUT_RESULT"
	fi
	return 0
}

menuCleanup(){
	local choice scope plat
	printBanner "clean"
	echoInfo "$(t msg.cleanup_projects_caches_libraries "cleanup projects · caches · libraries")"
	printf '\n'

	while true; do
		menuPick "$(t menu.cleanup "Cleanup")" \
			"$(t menu.disk_usage_how_much_each_path "Disk usage — how much each path holds")|usage" \
			"$(t menu.projects_obj_build_vs_artifacts_bin "Projects — obj/build/.vs artifacts (bin/ untouched)")|projects" \
			"$(t menu.caches_libs_download_packages "Caches — libs/download packages")|caches" \
			"$(t menu.libraries_remove_minimise_prebuilts "Libraries — remove / minimise prebuilts")|libs" \
			"$(t menu.core_build_compiled_openframeworks_core "Core build — compiled openFrameworks core")|core" \
			"$(t menu.back "Back")|back" \
			|| return 0
		choice="$UI_MENU_RESULT"
		case "$choice" in
			projects)
				menuPick "$(t menu.project_cleanup_scope "Project cleanup scope")" \
					"$(t menu.all_apps_examples "All apps + examples")|all" \
					"$(t menu.apps_only "apps/ only")|apps" \
					"$(t menu.examples_only "examples/ only")|examples" \
					"$(t menu.single_path "Single path…")|path" \
					"$(t menu.back "Back")|back" || continue
				scope="$UI_MENU_RESULT"
				[[ "$scope" == "back" ]] && continue
				if [[ "$scope" == "path" ]]; then
					menuInput "$(t msg.project_folder_path "Project or folder path")" || continue
					cleanProjectsScope path "$UI_INPUT_RESULT"
				else
					cleanProjectsScope "$scope"
				fi
				menuPause
				;;
			caches)
				menuPick "$(t menu.cache_cleanup "Cache cleanup")" \
					"$(t menu.package_downloads_only_recommended "Package downloads only (Recommended)")|packages" \
					"$(t menu.wipe_entire_libs_download_compiled_intermediates "Wipe entire libs/download + compiled intermediates")|all" \
					"$(t menu.back "Back")|back" || continue
				[[ "$UI_MENU_RESULT" == "back" ]] && continue
				cleanDownloadCaches "$UI_MENU_RESULT"
				menuPause
				;;
			libs)
				menuPick "$(t menu.library_cleanup "Library cleanup")" \
					"$(t menu.remove_all_prebuilt_libs_nuclear "Remove ALL prebuilt libs (nuclear)")|all" \
					"$(t menu.remove_other_platforms_keep_one_you "Remove other platforms — keep the one you choose")|other" \
					"$(t menu.remove_one_platform "Remove one platform…")|one" \
					"$(t menu.list_installed_lib_platform_folders "List installed lib/<platform> folders")|list" \
					"$(t menu.back "Back")|back" || continue
				case "$UI_MENU_RESULT" in
					list) cleanLibraries list; menuPause ;;
					other)
						menuPickLibPlatformSpec "Keep which platform? Everything else is removed" "This host (${OF_PLATFORM}${OF_ARCH:+ / $OF_ARCH}) — automatic|host" || continue
						plat="$MENU_PLAT"
						[[ "$plat" == "host" ]] && plat=""
						cleanLibraries other-platforms "$plat"
						menuPause
						;;
					one)
						menuPickLibPlatformSpec "Platform folder to remove" || continue
						cleanLibraries platform "$MENU_PLAT"
						menuPause
						;;
					all) cleanLibraries all-prebuilt; menuPause ;;
				esac
				;;
			core)
				menuPick "$(t menu.compiled_core_libs_openframeworkscompiled_lib "Compiled core (libs/openFrameworksCompiled/lib)")" \
					"$(t menu.this_host_only_rebuilt_by_next "This host only — rebuilt by the next build")|host" \
					"$(t menu.every_platform "Every platform")|all" \
					"$(t menu.back "Back")|back" || continue
				[[ "$UI_MENU_RESULT" == "back" ]] && continue
				cleanCoreBuilds "$UI_MENU_RESULT"
				menuPause
				;;
			usage) cleanUsageReport all; menuPause ;;
			back) return 0 ;;
		esac
	done
}

cmdMenu(){
	# labelled like the ofBaseApp lifecycle (setup/update/draw/exit) since
	# this is openFrameworks' own menu — fn name shown dim on the right
	local choice
	local setupDesc="libs + Project Generator (apothecary @ latest)"
	local -a opts
	if ! menuCanRun; then
		echoWarning "$(t msg.no_tty_showing_status "no TTY — showing status")"
		cmdStatus
		return 0
	fi
	[[ "$OF_PLATFORM" == "linux" ]] && setupDesc="distro deps + libs + Project Generator"

	while true; do
		printf '\n'
		printBanner "menu"
		printf '\n'
		echoKV "$(t msg.host "host")" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
		echoKV "$(t msg.dir_2 "of dir")" "$OF_DIR"
		echoKV "$(t msg.libs_2 "libs")" "${LIB_SOURCE} @ ${LIB_TAG}"
		[[ -n "$OF_LINUX_DISTRO" ]] && echoKV "$(t msg.distro "distro")" "$OF_LINUX_DISTRO"
		if ofHasGit; then
			echoKV "$(t msg.git "git")" "$(ofGitSummary)"
		else
			echoKV "$(t msg.git "git")" "$(ofNoGitLabel)"
		fi
		printf '\n'

		# short labels on purpose — long lines wrap/truncate differently across
		# terminals and can desync gum's highlighted row from the real selection
		opts=(
			"$(t menu.status_checker_cmdstatus "status()   — checker  (cmdStatus)")|status"
			"$(t menu.setup_install_cmdsetup "setup()    — install  (cmdSetup)")|setup"
		)
		if [[ "$OF_PLATFORM" == "vs" ]]; then
			opts+=("$(t menu.vsconfig_vs_components_cmdvsconfig "vsconfig() — VS components  (cmdVsConfig)")|vsconfig")
		fi
		opts+=(
			"$(t menu.update_refresh_menuupdate "update()   — refresh  (menuUpdate)")|update"
			"$(t menu.draw_build_menubuild "draw()     — build  (menuBuild)")|build"
			"$(t menu.cleanup_free_space_menucleanup "cleanup()  — free space  (menuCleanup)")|cleanup"
			"$(t menu.version_info_menuversion "version()  — info  (menuVersion)")|version"
			"$(t menu.test_smoke_tests_menutest "test()     — smoke tests  (menuTest)")|test"
		)
		if [[ ! -e "$OF_DIR/.git" ]]; then
			opts+=("$(t menu.clone_nightly_git_clone_cmdclone "clone()    — nightly → git clone  (cmdClone)")|clone")
		fi
		opts+=("$(t menu.exit "exit()")|exit")
		if ! menuPick "$(t menu.what_do_you_want_do "What do you want to do?")" "${opts[@]}"
		then
			echoInfo "$(t msg.bye "bye")"
			return 0
		fi
		choice="$UI_MENU_RESULT"
		printf '\n'
		case "$choice" in
			status)     cmdStatus; menuPause ;;
			setup)      cmdSetup "$OF_PLATFORM"; menuPause ;;
			vsconfig)   menuVsConfig; [[ $? -eq 2 ]] || menuPause ;;
			update)     menuUpdate; [[ $? -eq 2 ]] || menuPause ;;
			build)      menuBuild ;;
			cleanup|clean) menuCleanup ;;
			clone)      cmdClone; menuPause ;;
			version)    menuVersion; menuPause ;;
			test)       menuTest; [[ $? -eq 2 ]] || menuPause ;;
			exit|quit)  echoSuccess "$(t msg.bye "bye")"; return 0 ;;
			*)          echoError "$(t msg.unknown "unknown: %s" "$choice")"; menuPause ;;
		esac
	done
}

# Update menu: pick what to refresh, always prompt libs source when downloading libs
menuUpdate(){
	local what
	printBanner "update"
	echoInfo "$(t msg.refresh_libraries_project_generator "refresh libraries / Project Generator")"
	printf '\n'
	assessLibsState "$OF_PLATFORM" "${LIB_SOURCE:-apothecary}" "${LIB_TAG:-latest}"
	echoKV "$(t msg.libs_2 "libs")" "${LIBS_STATE} — ${LIBS_STATE_DETAIL}"
	assessPGState
	echoKV "$(t msg.pg "pg")" "${PG_STATE} — ${PG_STATE_DETAIL}"
	printf '\n'

	# return 2 for "nothing happened, back/cancelled" so the caller can skip
	# the "press Enter to return" pause, same convention as menuApothecary/menuUpgrade
	menuPick "$(t menu.what_update "What to update?")" \
		"$(t menu.libraries_choose_source "Libraries (choose source)…")|libs" \
		"$(t msg.project_generator "Project Generator")|pg" \
		"$(t menu.libraries_project_generator "Libraries + Project Generator")|all" \
		"$(t menu.back "Back")|back" \
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
			[[ -n "$path" ]] || { echoError "$(t msg.usage_build_project_path_system_debug "usage: of build project <path> [system] [Debug|Release]")"; return 1; }
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
			[[ -n "$path" ]] || { echoError "$(t msg.usage_build_example_path_system_cfg "usage: of build example <path> [system] [cfg]")"; return 1; }
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
			[[ -n "$path" ]] || { echoError "$(t msg.usage_build_emscripten_path_debug_release "usage: of build emscripten <path> [Debug|Release]")"; return 1; }
			cmdBuildProject "$path" emscripten "$config" 0
			;;
		open-em|emrun|open-emscripten)
			local path="${1:-}"
			[[ -n "$path" ]] || { echoError "$(t msg.usage_build_open_em_path "usage: of build open-em <path>")"; return 1; }
			path=$(resolveProjectPath "$path") || { echoError "$(t msg.not_found_2 "not found: %s" "$1")"; return 1; }
			ensureEmscriptenEnv || true
			runEmscriptenOpen "$path"
			;;
		generate|pg)
			local path="${1:-}" platforms="${2:-}"
			[[ -n "$path" ]] || { echoError "$(t msg.usage_build_generate_path_platforms "usage: of build generate <path> [platforms]")"; return 1; }
			cmdBuildGenerate "$path" "$platforms"
			;;
		cmake)
			case "${1:-}" in
				""|status|info) runCmakeStatus ;;
				android) runCmakeAndroidCore "${2:-Debug}" ;;
				*)
					local path="$1" config="${2:-Release}"
					path=$(resolveProjectPath "$path") || { echoError "$(t msg.not_found_2 "not found: %s" "$1")"; return 1; }
					runCmakeProject "$path" "$config"
					;;
			esac
			;;
		tests|test)
			if [[ "$OF_PLATFORM" == "vs" ]]; then
				runHostTests "${1:-Debug}" "${2:-${OF_VS_PLATFORM:-$(vsPlatformFromArch)}}" "${3:-${OF_VS_TOOLSET:-$(vsToolsetDefault)}}"
			else
				runHostTests "$@"
			fi
			;;
		clean)
			local path="${1:-}"
			[[ -n "$path" ]] || { echoError "$(t msg.usage_build_clean_path "usage: of build clean <path>")"; return 1; }
			path=$(resolveProjectPath "$path") || { echoError "$(t msg.not_found_2 "not found: %s" "$1")"; return 1; }
			runCleanProject "$path"
			;;
		*)
			# bare path → treat as project with make
			if [[ -n "$sub" ]] && { [[ -d "$sub" ]] || [[ -d "${OF_DIR}/$sub" ]] || [[ -d "${OF_DIR}/examples/$sub" ]] || [[ -d "${OF_DIR}/apps/$sub" ]]; }; then
				cmdBuildProject "$sub" "${1:-make}" "${2:-Release}" 0
				return $?
			fi
			echoError "$(t msg.unknown_build_subcommand "unknown build subcommand: %s" "${sub:-}")"
			printHelpBuild
			return 1
			;;
	esac
}

# ---------------------------------------------------------------------------
# of i18n — the string dictionary (see scripts/i18n and scripts/dev/i18n_extract.py)
#   of i18n list                      languages, with how much of the catalogue each covers
#   of i18n export [<lang>]           key<TAB>English[<TAB>translation] on stdout (--json: structured)
#   of i18n check <lang>              keys that are missing or invalid in <lang>
#   of i18n import <lang> <file|->    install a translation (key<TAB>text lines), validated
# ---------------------------------------------------------------------------
i18nCatalog(){ printf '%s/en.tsv' "$OF_I18N_DIR"; }

# number of %s placeholders in a text; prints -1 when it holds any other % spec
i18nPlaceholders(){
	local t="${1//%%/}" rest
	rest="${t//%s/}"
	if [[ "$rest" == *%* ]]; then printf -- '-1'; return; fi
	printf '%s' "$(( (${#t} - ${#rest}) / 2 ))"
}

# i18nReadTsv <file> — fills I18N_KEYS / I18N_TEXTS (parallel arrays)
i18nReadTsv(){
	local file="$1" k v
	I18N_KEYS=(); I18N_TEXTS=()
	[[ -f "$file" ]] || return 1
	while IFS=$'\t' read -r k v || [[ -n "$k" ]]; do
		v="${v%$'\r'}"
		[[ -z "$k" || "$k" == \#* ]] && continue
		I18N_KEYS+=("$k"); I18N_TEXTS+=("$v")
	done < "$file"
}

# Validate a translation file against the catalogue.
# Sets I18N_OK (valid pairs, as lines "key<TAB>text"), I18N_BAD (problems), I18N_MISSING (count)
i18nValidate(){
	local file="$1" i key text want got var total
	local -a trKeys=() trTexts=()
	I18N_OK=(); I18N_BAD=(); I18N_MISSING=0
	i18nReadTsv "$(i18nCatalog)" || return 2
	total=${#I18N_KEYS[@]}
	for ((i = 0; i < total; i++)); do
		printf -v "OF_E_$(printf '%s' "${I18N_KEYS[$i]}" | LC_ALL=C tr -c 'A-Za-z0-9_
' '_')" '%s' "${I18N_TEXTS[$i]}"
	done
	i18nReadTsv "$file" || return 3
	trKeys=("${I18N_KEYS[@]}"); trTexts=("${I18N_TEXTS[@]}")
	for ((i = 0; i < ${#trKeys[@]}; i++)); do
		key="${trKeys[$i]}"; text="${trTexts[$i]}"
		var="OF_E_${key//[^A-Za-z0-9_]/_}"
		if [[ -z "${!var:-}" ]]; then I18N_BAD+=("${key}: unknown key"); continue; fi
		if [[ -z "$text" ]]; then I18N_BAD+=("${key}: empty text"); continue; fi
		want="$(i18nPlaceholders "${!var}")"
		got="$(i18nPlaceholders "$text")"
		if [[ "$got" != "$want" ]]; then
			I18N_BAD+=("${key}: needs ${want} %s placeholder(s), has ${got}")
			continue
		fi
		I18N_OK+=("${key}"$'\t'"${text}")
	done
	I18N_MISSING=$(( total - ${#I18N_OK[@]} ))
	[[ $I18N_MISSING -ge 0 ]] || I18N_MISSING=0
	return 0
}

i18nLangFiles(){
	local f
	for f in "$OF_I18N_DIR"/*.tsv; do
		[[ -f "$f" ]] || continue
		[[ "$(basename "$f" .tsv)" == "en" ]] && continue
		printf '%s\n' "$f"
	done
}

cmdI18n(){
	local sub="${1:-list}" lang="${2:-}" file="${3:-}" f i total=0 n tmpIn=""
	[[ "$sub" == "export" ]] || printBanner "i18n"     # export prints data only
	case "$sub" in
		list)
			i18nReadTsv "$(i18nCatalog)" && total=${#I18N_KEYS[@]}
			echoKV "$(t msg.catalogue "catalogue")" "$(t msg.strings_english "%s strings (English) in %s" "$total" "$(i18nCatalog)")"
			echoKV "$(t msg.current "current")" "${OF_LANG:-en}"
			local any=0
			while IFS= read -r f; do
				[[ -n "$f" ]] || continue
				any=1
				i18nValidate "$f" && n=${#I18N_OK[@]}
				echoKV "$(basename "$f" .tsv)" "$(t msg.strings_covered "%s of %s strings · %s problem(s)" "${n:-0}" "$total" "${#I18N_BAD[@]}")"
			done < <(i18nLangFiles)
			[[ $any -eq 1 ]] || echoNote "$(t msg.no_translations_yet "no translations yet — of i18n export > en.tsv, translate it, then: of i18n import <lang> <file>")"
			;;
		export)
			i18nReadTsv "$(i18nCatalog)" || { echoErrorCode failed "$(t msg.catalogue_missing "catalogue not found: %s (run python3 scripts/dev/i18n_extract.py)" "$(i18nCatalog)")"; return 1; }
			local -a enKeys=("${I18N_KEYS[@]}") enTexts=("${I18N_TEXTS[@]}")
			local hasTr=0
			if [[ -n "$lang" && "$lang" != "en" ]]; then
				[[ "$lang" =~ ^[A-Za-z][A-Za-z0-9_-]*$ ]] || { echoErrorCode invalid_argument "$(t msg.invalid_language_name "invalid language name: %s" "$lang")"; return 2; }
				OF_LANG="$lang"
				i18nLoad "$lang" && hasTr=1
			fi
			local k tr var
			if [[ "$OF_JSON" == "1" ]]; then
				local items="" first=1
				for ((i = 0; i < ${#enKeys[@]}; i++)); do
					k="${enKeys[$i]}"; tr=""
					if [[ $hasTr -eq 1 ]]; then var="$(i18nVarName "$k")"; tr="${!var:-}"; fi
					[[ $first -eq 1 ]] || items+=","
					first=0
					items+=$'\n    '"{\"key\": \"$(jsonEscape "$k")\", \"text\": \"$(jsonEscape "${enTexts[$i]}")\", \"placeholders\": $(i18nPlaceholders "${enTexts[$i]}"), \"translation\": $(jsonStrOrNull "$tr")}"
				done
				OF_JSON_DATA="$(printf '{"lang": "%s", "source_lang": "en", "count": %s, "strings": [%s\n  ]}' "$(jsonEscape "${lang:-en}")" "${#enKeys[@]}" "$items")"
			else
				for ((i = 0; i < ${#enKeys[@]}; i++)); do
					k="${enKeys[$i]}"; tr=""
					if [[ $hasTr -eq 1 ]]; then var="$(i18nVarName "$k")"; tr="${!var:-}"; fi
					if [[ -n "$lang" && "$lang" != "en" ]]; then printf '%s\t%s\t%s\n' "$k" "${enTexts[$i]}" "$tr"
					else printf '%s\t%s\n' "$k" "${enTexts[$i]}"; fi
				done
			fi
			;;
		check|import)
			[[ "$lang" =~ ^[A-Za-z][A-Za-z0-9_-]*$ && "$lang" != "en" ]] || { echoErrorCode invalid_argument "$(t msg.usage_i18n "usage: of i18n check <lang>  |  of i18n import <lang> <file|->")"; return 2; }
			if [[ "$sub" == "check" ]]; then file="$OF_I18N_DIR/${lang}.tsv"
			elif [[ -z "$file" ]]; then echoErrorCode invalid_argument "$(t msg.usage_i18n "usage: of i18n check <lang>  |  of i18n import <lang> <file|->")"; return 2
			elif [[ "$file" == "-" ]]; then
				file="$(mktemp)"; cat > "$file"; tmpIn="$file"
			fi
			i18nValidate "$file"; local vrc=$?
			[[ -n "$tmpIn" ]] && rm -f "$tmpIn"
			case $vrc in
				2) echoErrorCode failed "$(t msg.catalogue_missing "catalogue not found: %s (run python3 scripts/dev/i18n_extract.py)" "$(i18nCatalog)")"; return 1 ;;
				3) echoErrorCode invalid_argument "$(t msg.file_not_found "file not found: %s" "$file")"; return 2 ;;
			esac
			for f in ${I18N_BAD[@]+"${I18N_BAD[@]}"}; do echoWarning "$f"; done
			if [[ "$sub" == "import" ]]; then
				if [[ ${#I18N_OK[@]} -eq 0 ]]; then
					echoErrorCode invalid_argument "$(t msg.nothing_valid_to_import "nothing valid to import")"
					return 2
				fi
				mkdir -p "$OF_I18N_DIR" && printf '%s\n' "${I18N_OK[@]}" | LC_ALL=C sort -t $'\t' -k1,1 > "$OF_I18N_DIR/${lang}.tsv" || { echoErrorCode failed "$(t msg.could_not_write "could not write %s" "$OF_I18N_DIR/${lang}.tsv")"; return 1; }
				echoSuccess "$(t msg.imported "imported %s string(s) for %s · %s rejected · %s missing" "${#I18N_OK[@]}" "$lang" "${#I18N_BAD[@]}" "$I18N_MISSING")"
			else
				echoSuccess "$(t msg.check_summary "%s valid · %s problem(s) · %s missing" "${#I18N_OK[@]}" "${#I18N_BAD[@]}" "$I18N_MISSING")"
			fi
			if [[ "$OF_JSON" == "1" ]]; then
				local bad="" b first=1
				for b in ${I18N_BAD[@]+"${I18N_BAD[@]}"}; do [[ $first -eq 1 ]] || bad+=", "; first=0; bad+="\"$(jsonEscape "$b")\""; done
				OF_JSON_DATA="$(printf '{"lang": "%s", "valid": %s, "problems": [%s], "missing": %s}' "$(jsonEscape "$lang")" "${#I18N_OK[@]}" "$bad" "$I18N_MISSING")"
			fi
			[[ "$sub" == "import" || ${#I18N_BAD[@]} -eq 0 ]] || return 1
			;;
		*)
			echoErrorCode invalid_argument "$(t msg.usage_i18n_all "usage: of i18n [list|export [lang]|check <lang>|import <lang> <file|->]")"
			return 2 ;;
	esac
	return 0
}

# ---------------------------------------------------------------------------
# --json mode
#   stdout  one document:  {"version":1,"command":"…","ok":true,"dry_run":false,
#                           "data":{…}|null,"warnings":[…],"error":null|{"code","message"}}
#   stderr  the usual human text (drop it with 2>/dev/null)
#   exit    0 ok · 1 failed (incl. partial) · 2 bad arguments / unsupported · 3 refused
#           (confirmation_required, busy, build_active)
# Never prompts: a destructive command without --yes answers confirmation_required.
# Supported: status, version [pg], menu (catalogue), cleanup …
# ---------------------------------------------------------------------------
OF_JSON_VERSION=1
OF_JSON_DATA=""

# "text" or null
jsonStrOrNull(){ if [[ -n "${1:-}" ]]; then printf '"%s"' "$(jsonEscape "$1")"; else printf 'null'; fi; }

ofJsonGitObject(){
	if ofHasGit; then
		printf '{"checkout": true, "summary": %s}' "$(jsonStrOrNull "$(ofGitSummary)")"
	else
		printf '{"checkout": false, "summary": null}'
	fi
}

ofJsonStatus(){
	local ofVer pgBin pgVer lastUp stSha stShaN
	ofVer=$(readOfVersion) || ofVer=""
	pgBin=$(findPGBinary) || pgBin=""
	[[ -n "$pgBin" ]] && { pgVer=$(readPGVersion) || pgVer=""; }
	assessLibsState "$OF_PLATFORM"
	lastUp=$(readLibsLastUpdate)
	stSha=$(readLibStateField sha_status) || stSha=$(readVerifyField status) || stSha=""
	stShaN=$(readLibStateField sha_verified) || stShaN=$(readVerifyField verified) || stShaN=""
	OF_JSON_DATA="$(printf '{"openframeworks": %s, "cli": "%s", "platform": %s, "arch": %s, "distro": %s, "of_dir": "%s", "git": %s, "project_generator": {"installed": %s, "version": %s, "path": %s}, "libraries": {"state": %s, "detail": %s, "source": %s, "tag": %s, "last_update": %s}, "integrity": {"status": %s, "verified_packages": %s}}' \
		"$(jsonStrOrNull "$ofVer")" "$OF_SCRIPT_VERSION" "$(jsonStrOrNull "$OF_PLATFORM")" "$(jsonStrOrNull "${OF_ARCH:-}")" "$(jsonStrOrNull "${OF_LINUX_DISTRO:-}")" \
		"$(jsonEscape "$OF_DIR")" "$(ofJsonGitObject)" \
		"$([[ -n "$pgBin" ]] && echo true || echo false)" "$(jsonStrOrNull "${pgVer:-}")" "$(jsonStrOrNull "$pgBin")" \
		"$(jsonStrOrNull "${LIBS_STATE:-}")" "$(jsonStrOrNull "${LIBS_STATE_DETAIL:-}")" "$(jsonStrOrNull "${LIB_SOURCE:-}")" "$(jsonStrOrNull "${LIB_TAG:-}")" "$(jsonStrOrNull "$lastUp")" \
		"$(jsonStrOrNull "$stSha")" "$([[ "$stShaN" =~ ^[0-9]+$ ]] && echo "$stShaN" || echo null)")"
	[[ -n "$ofVer" ]] || OF_WARNINGS+=("openFrameworks version not found")
	return 0
}

ofJsonVersion(){
	local ofVer
	ofVer=$(readOfVersion) || ofVer=""
	OF_JSON_DATA="$(printf '{"openframeworks": %s, "cli": "%s", "platform": %s, "arch": %s, "git": %s}' \
		"$(jsonStrOrNull "$ofVer")" "$OF_SCRIPT_VERSION" "$(jsonStrOrNull "$OF_PLATFORM")" "$(jsonStrOrNull "${OF_ARCH:-}")" "$(ofJsonGitObject)")"
}

ofJsonVersionPG(){
	local bin ver=""
	bin=$(findPGBinary) || bin=""
	[[ -n "$bin" ]] && { ver=$(readPGVersion) || ver=""; }
	OF_JSON_DATA="$(printf '{"installed": %s, "version": %s, "path": %s}' \
		"$([[ -n "$bin" ]] && echo true || echo false)" "$(jsonStrOrNull "$ver")" "$(jsonStrOrNull "$bin")")"
}

# What the menu offers, as data: id, the command to run, whether it deletes
# anything, whether it has --json output yet.
ofJsonCatalog(){
	local d
	d='{"commands": ['
	d+='{"id": "status",  "command": "of status",  "description": "host, openFrameworks, Project Generator, libraries", "destructive": false, "json": true},'
	d+='{"id": "version", "command": "of version [of|pg]", "description": "version info", "destructive": false, "json": true},'
	d+='{"id": "setup",   "command": "of setup [platform]", "description": "install dependencies, libraries and Project Generator", "destructive": false, "json": false},'
	d+='{"id": "update",  "command": "of update [libs|pg|all]", "description": "refresh libraries / Project Generator", "destructive": false, "json": false},'
	d+='{"id": "build",   "command": "of build [project]", "description": "build a project", "destructive": false, "json": false},'
	d+='{"id": "test",    "command": "of test [group]", "description": "run the smoke tests", "destructive": false, "json": false},'
	d+='{"id": "clone",   "command": "of clone", "description": "turn a nightly package into a git checkout", "destructive": false, "json": false},'
	d+='{"id": "cleanup.usage", "command": "of cleanup usage [projects|caches|libs|core|all]", "description": "disk usage of every cleanable path", "destructive": false, "json": true},'
	d+='{"id": "cleanup.projects", "command": "of cleanup projects [all|apps|examples|<path>]", "description": "remove obj/build/CMake build trees (bin/ is never touched)", "destructive": true, "json": true},'
	d+='{"id": "cleanup.caches", "command": "of cleanup caches [packages|all]", "description": "libs/download packages, compiled intermediates", "destructive": true, "json": true},'
	d+='{"id": "cleanup.libs", "command": "of cleanup libs [other|all|<platform>|list]", "description": "prebuilt libraries", "destructive": true, "json": true},'
	d+='{"id": "cleanup.core", "command": "of cleanup core [host|all]", "description": "compiled core in libs/openFrameworksCompiled/lib", "destructive": true, "json": true}'
	d+='], "flags": {"--json": "machine-readable output (alias --api)", "--yes": "skip confirmation prompts (cleanup)", "--dry-run": "report only, delete nothing (cleanup)", "--force": "clean even if a build looks active (cleanup)"},'
	d+=' "exit_codes": {"0": "ok", "1": "failed or partly failed", "2": "bad arguments or unsupported command", "3": "refused: confirmation_required, busy or build_active"}}'
	OF_JSON_DATA="$d"
}

# cleanup under --json: usage returns its report as data; deleting commands run
# normally (human text on stderr) and return what happened, item by item.
ofJsonCleanup(){
	local a sub="" what="" rc=0
	local -a pos=()
	for a in "$@"; do
		case "$a" in --yes|-y|--dry-run|-n|--force|--tsv) ;; *) pos+=("$a") ;; esac
	done
	sub="${pos[0]:-}"
	what="${pos[1]:-}${pos[2]:+ ${pos[2]}}"
	case "$sub" in
		"")
			OF_ERR_CODE=invalid_argument
			echoError "$(t msg.usage_cleanup_projects_caches_libs_core "usage: of cleanup [projects|caches|libs|core|usage] …")"
			return 2 ;;
		usage|size|sizes|du)
			OF_CLEAN_FORMAT=json
			OF_JSON_DATA="$(cleanUsageReport "${what:-all}")"
			rc=$?
			[[ $rc -eq 0 ]] || OF_JSON_DATA=""
			return $rc ;;
	esac
	runCommand cleanup "$@"
	rc=$?
	# items hold JSON with spaces, so join them without word splitting
	local items="" idx
	for ((idx = 0; idx < ${#OF_JSON_ITEMS[@]}; idx++)); do
		[[ $idx -gt 0 ]] && items+=","
		items+=$'\n    '"${OF_JSON_ITEMS[$idx]}"
	done
	[[ -n "$items" ]] && items+=$'\n  '
	OF_JSON_DATA="$(printf '{"command": "%s", "dry_run": %s, "totals": {"removed": %s, "removed_kb": %s, "skipped": %s, "failed": %s}, "items": [%s]}' \
		"$(jsonEscape "$sub${what:+ $what}")" "$([[ "$OF_CLEAN_DRY_RUN" == "1" ]] && echo true || echo false)" \
		"$OF_J_REMOVED" "$OF_J_KB" "$OF_J_SKIPPED" "$OF_J_FAILED" "$items")"
	return $rc
}

ofJsonDispatch(){
	local cmd="${1:-}"
	[[ $# -gt 0 ]] && shift
	case "$cmd" in
		""|menu|commands|help|-h|--help) ofJsonCatalog ;;
		status|check|doctor|installed|libs-status) ofJsonStatus ;;
		version)
			case "${1:-}" in
				""|of) ofJsonVersion ;;
				pg|projectgenerator) ofJsonVersionPG ;;
				*) OF_ERR_CODE=invalid_argument; echoError "$(t msg.valid_pg "valid: of | pg")"; return 2 ;;
			esac ;;
		cleanup|clean) ofJsonCleanup "$@" ;;
		i18n|lang) cmdI18n "$@" ;;
		*)
			OF_ERR_CODE=unsupported_command
			echoError "$(t msg.has_no_json_output_yet_supported "'%s' has no --json output yet (supported: status, version, menu, cleanup)" "${cmd}")"
			return 2 ;;
	esac
}

# Run a command in --json mode and print the envelope to the original stdout.
ofJsonRun(){
	local rc code msg ok=true errObj=null warns="" first=1
	ofJsonDispatch "$@"
	rc=$?
	if [[ $rc -ne 0 ]]; then
		ok=false
		code="${OF_ERR_CODE:-failed}"
		[[ "$code" == "failed" && $OF_J_FAILED -gt 0 ]] && code=partial_failure
		msg="${OF_LAST_ERROR:-command failed}"
		case "$code" in
			invalid_argument|unsupported_command|invalid_tree) rc=2 ;;
			confirmation_required|busy|build_active) rc=3 ;;
			*) rc=1 ;;
		esac
		errObj="$(printf '{"code": "%s", "message": "%s"}' "$code" "$(jsonEscape "$(cleanShow "$msg")")")"
		# a request that never ran has no result to report
		case "$code" in invalid_argument|unsupported_command|invalid_tree) OF_JSON_DATA="" ;; esac
	fi
	local i
	for ((i = 0; i < ${#OF_WARNINGS[@]}; i++)); do
		[[ $first -eq 1 ]] || warns+=", "
		first=0
		warns+="\"$(jsonEscape "$(cleanShow "${OF_WARNINGS[$i]}")")\""
	done
	printf '{"version": %s, "command": "%s", "ok": %s, "dry_run": %s, "data": %s, "warnings": [%s], "error": %s}\n' \
		"$OF_JSON_VERSION" "$(jsonEscape "$*")" "$ok" "$([[ "$OF_CLEAN_DRY_RUN" == "1" ]] && echo true || echo false)" \
		"${OF_JSON_DATA:-null}" "$warns" "$errObj" >&8
	return $rc
}

runCommand(){
	local cmd=$1 subcmd=$2 subcmd2=$3 subcmd3=$4
	case "$cmd" in
		help|-h|--help) printHelp ;;
		menu) cmdMenu ;;
		status|check|doctor|installed|libs-status) cmdStatus ;;
		setup|install) cmdSetup "${subcmd:-$OF_PLATFORM}" ;;
		vsconfig|vs-components) cmdVsConfig "$subcmd" "$subcmd2" "$subcmd3" ;;
		update) cmdUpdate "$subcmd" "$subcmd2" "$subcmd3" ;;
		build)
			shift
			cmdBuild "$@"
			;;
		version)
			case "$subcmd" in
				""|of) cmdVersion ;;
				pg|projectgenerator) cmdVersionPG ;;
				*) echoError "$(t msg.valid_pg "valid: of | pg")"; return 1 ;;
			esac
			;;
		upgrade) cmdUpgrade "$subcmd" ;;
		clone|git)
			cmdClone
			;;
		test)
			case "${subcmd:-}" in
				""|menu) menuTest ;;
				*) cmdTest "$subcmd" ;;
			esac
			;;
		i18n|lang) shift; cmdI18n "$@" ;;
		cleanup|clean)
			# --yes/-y and --dry-run/-n may appear anywhere after the command
			local -a cleanArgs=()
			local cleanArg
			for cleanArg in "${@:2}"; do
				case "$cleanArg" in
					--yes|-y) OF_CLEAN_YES=1 ;;
					--dry-run|-n) OF_CLEAN_DRY_RUN=1 ;;
					--force) OF_CLEAN_FORCE=1 ;;
					--json) OF_CLEAN_FORMAT=json ;;
					--tsv) OF_CLEAN_FORMAT=tsv ;;
					*) cleanArgs+=("$cleanArg") ;;
				esac
			done
			subcmd="${cleanArgs[0]:-}"
			subcmd2="${cleanArgs[1]:-}"
			subcmd3="${cleanArgs[2]:-}"
			case "${subcmd:-}" in
				""|menu) menuCleanup ;;
				usage|size|sizes|du) cleanUsageReport "${subcmd2:-all}" ;;
				core) cleanCoreBuilds "${subcmd2:-host}" ;;
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
						other|other-platforms|host) cleanLibraries other-platforms "${subcmd3:-}" ;;
						all|nuclear) cleanLibraries all-prebuilt ;;
						*) cleanLibraries platform "${subcmd2}" ;;
					esac
					;;
				*)
					echoErrorCode invalid_argument "$(t msg.usage_cleanup_projects_caches_libs_core "usage: of cleanup [projects|caches|libs|core|usage] …")"
					echoNote "$(t msg.cleanup_projects_all_apps_examples_path "  of cleanup projects [all|apps|examples|<path>]")"
					echoNote "$(t msg.cleanup_caches_packages_all "  of cleanup caches [packages|all]")"
					echoNote "$(t msg.cleanup_libs_other_keep_list_all "  of cleanup libs [other [<keep>]|list|all|<platform>]   e.g. vs  macos  linux64  linux/arm64  msys2/ucrt64")"
					echoNote "$(t msg.cleanup_libs_other_keep_removes_every "  of cleanup libs other <keep>    removes every other platform and keeps <keep>; without it, keeps this host")"
					echoNote "$(t msg.cleanup_core_host_all_compiled_core "  of cleanup core [host|all]        compiled core in libs/openFrameworksCompiled/lib")"
					echoNote "$(t msg.cleanup_usage_projects_caches_libs_core "  of cleanup usage [projects|caches|libs|core|all] [--json|--tsv]")"
					echoNote "$(t msg.add_dry_run_preview_yes_skip "  add --dry-run to preview, --yes to skip the confirmation prompt,")"
					echoNote "$(t msg.force_clean_while_build_looks_active "  --force to clean while a build looks active")"
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
			echoError "$(t msg.unknown_command "Unknown command: %s" "$cmd")"
			echoNote "$(t msg.valid_menu_status_setup_update_build "valid: menu status setup update build cleanup version upgrade clone apothecary help")"
			printHelp
			return 1
			;;
	esac
}

if [[ "$OF_JSON" == "1" ]]; then
	ofJsonRun "$@"
	exit $?
fi

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
