#!/usr/bin/env bash
# Downloads optional library modules (e.g. dawn, metalangle) that are not part
# of the core openFrameworksLibs packages. apothecary publishes them per library
# on its latest-modular release as oF_<lib>_<target>_<arch>[_gcc<N>].tar.bz2
# (.zip for Visual Studio / MSYS2); each one unpacks to libs/<lib>/.
#
# Modules that aren't built for a target yet are skipped with a note.
#
# Usage: download_modules.sh -p <platform> [-a <arch>] [-t <tag>] [--require] <module> [<module>...]
#   download_modules.sh -p macos dawn metalangle
#   OF_LIB_MODULES=dawn,metalangle scripts/macos/download_libs.sh   (via download_libs.sh --modules)

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OF_ROOT="$( cd "$SCRIPT_DIR/../.." && pwd )"
REPO="openframeworks/apothecary"
TAG="latest-modular"
PLATFORM=""
ARCH=""
REQUIRE=0
SILENT_ARGS=""
NO_SSL=""
MODULES=()

. "$SCRIPT_DIR/downloader.sh"
. "$SCRIPT_DIR/sha_verify.sh"

printHelp(){
	sed -n '2,13p' "$0" | sed 's/^# \{0,1\}//'
	cat <<EOF

Options:
  -p, --platform PLATFORM  osx | macos | ios | tvos | vs | msys2 | linux64 | linuxaarch64 | linuxarmv6l | linuxarmv7l | emscripten
  -a, --arch ARCH          vs: 64 | arm64 | arm64ec; emscripten: 32 | 64; msys2: ucrt64 | clang64 | clangarm64
  -t, --tag TAG            apothecary release (default latest-modular)
  --require                fail if a module isn't built for this platform (default: skip it)
  -s, --silent / -k, --no-ssl   passed to the downloader
EOF
}

while [[ $# -gt 0 ]]; do
	case "$1" in
		-p|--platform) PLATFORM="$2"; shift 2 ;;
		-a|--arch) ARCH="$2"; shift 2 ;;
		-t|--tag) TAG="$2"; shift 2 ;;
		--require) REQUIRE=1; shift ;;
		-s|--silent) SILENT_ARGS="-s"; shift ;;
		-k|--no-ssl) NO_SSL="-k"; shift ;;
		-h|--help) printHelp; exit 0 ;;
		-*) echo "Unknown option $1"; printHelp; exit 1 ;;
		*) IFS=', ' read -r -a list <<< "$1"; MODULES+=("${list[@]}"); shift ;;
	esac
done

if [[ ${#MODULES[@]} -eq 0 ]]; then
	echo "No modules given"; printHelp; exit 1
fi
[[ -z "$PLATFORM" ]] && { echo "No platform given (-p)"; exit 1; }

# candidate package suffixes for this platform, most specific first; the first
# one published on the release is used. Apple installs every Apple target that
# is built, like the core macos package covers all of them.
candidateSuffixes(){
	local gcc
	case "$PLATFORM" in
		osx|macos|ios|tvos|catos|xros)
			echo "osx_64.tar.bz2|ios_64.tar.bz2|tvos_64.tar.bz2|catos_64.tar.bz2|xros_64.tar.bz2" ;;
		vs)
			case "$(echo "${ARCH:-64}" | tr '[:upper:]' '[:lower:]')" in
				arm64ec) echo "vs_arm64ec.zip" ;;
				arm64) echo "vs_arm64.zip" ;;
				*) echo "vs_64.zip" ;;
			esac ;;
		msys2)
			echo "msys2_$(echo "${ARCH:-${MSYSTEM:-ucrt64}}" | tr '[:upper:]' '[:lower:]').zip" ;;
		emscripten)
			echo "emscripten_${ARCH:-32}.tar.bz2" ;;
		linux64|linuxaarch64|linuxarmv6l|linuxarmv7l)
			gcc=$(gcc -dumpversion 2>/dev/null | cut -d. -f1)
			local first=gcc14 second=gcc10
			[[ -n "$gcc" && "$gcc" -lt 14 ]] && { first=gcc10; second=gcc14; }
			case "$PLATFORM" in
				linux64) echo "linux_64_${first}.tar.bz2 linux_64_${second}.tar.bz2" ;;
				linuxaarch64) echo "linux_arm64_${first}.tar.bz2 linux_arm64_${second}.tar.bz2 linux_aarch64_gcc10.tar.bz2" ;;
				linuxarmv6l) echo "linux_armv6l_gcc10.tar.bz2" ;;
				linuxarmv7l) echo "linux_armv7l_gcc10.tar.bz2" ;;
			esac ;;
		*) echo "" ;;
	esac
}

MAP=$(mktemp 2>/dev/null || mktemp -t ofmodmap)
HAVE_MAP=0
fetchGithubReleaseDigests "$REPO" "$TAG" "$MAP" && HAVE_MAP=1

isPublished(){
	local asset="$1"
	if [[ $HAVE_MAP -eq 1 ]]; then
		[[ -n "$(lookupDigest "$MAP" "$asset")" ]]
	else
		curl -fsIL -o /dev/null "https://github.com/$REPO/releases/download/$TAG/$asset"
	fi
}

mkdir -p "$OF_ROOT/libs/download"
cd "$OF_ROOT/libs/download" || exit 1

failed=0
installed=()
for module in "${MODULES[@]}"; do
	[[ -z "$module" ]] && continue
	groups=$(candidateSuffixes)
	if [[ -z "$groups" ]]; then
		echo " ! [$module] modules aren't published for platform $PLATFORM"
		[[ $REQUIRE -eq 1 ]] && failed=1
		continue
	fi
	found_any=0
	# "|" separates targets that are all installed (Apple); spaces separate
	# alternatives for one target (first published wins)
	IFS='|' read -r -a targets <<< "$groups"
	for alternatives in "${targets[@]}"; do
		asset=""
		for suffix in $alternatives; do
			if isPublished "oF_${module}_${suffix}"; then asset="oF_${module}_${suffix}"; break; fi
		done
		[[ -z "$asset" ]] && continue
		found_any=1
		echo " [$module] downloading $asset"
		rm -f "$asset"
		downloader "https://github.com/$REPO/releases/download/$TAG/$asset" $SILENT_ARGS $NO_SSL
		if [[ ! -s "$asset" ]]; then
			echo " ! [$module] download failed: $asset"; failed=1; continue
		fi
		if [[ $HAVE_MAP -eq 1 ]] && ! verifyPackagesAgainstMap "$asset" "$MAP" "GitHub apothecary $TAG digests"; then
			echo " ! [$module] SHA-256 mismatch: $asset"; rm -f "$asset"; failed=1; continue
		fi
		# replace the lib/<target> folders this package contains, keep the module's other targets
		case "$asset" in
			*.zip) entries=$(unzip -Z1 "$asset") ;;
			*) entries=$(tar -tjf "$asset") ;;
		esac
		for libdir in $(echo "$entries" | awk -F/ 'NF>=4 && $2=="lib" && $3!="" {print $1"/lib/"$3}' | sort -u); do
			rm -rf "$OF_ROOT/libs/$libdir"
		done
		case "$asset" in
			*.zip) unzip -qo "$asset" -d "$OF_ROOT/libs/" ;;
			*) tar -xjf "$asset" -C "$OF_ROOT/libs/" ;;
		esac || { echo " ! [$module] could not unpack $asset"; failed=1; continue; }
		installed+=("$asset")
	done
	if [[ $found_any -eq 0 ]]; then
		echo " ~ [$module] not built for $PLATFORM yet on $TAG — skipped"
		[[ $REQUIRE -eq 1 ]] && failed=1
	fi
done
rm -f "$MAP"

if [[ ${#installed[@]} -gt 0 ]]; then
	echo " openFrameworks modules installed: ${installed[*]}"
fi
exit $failed
