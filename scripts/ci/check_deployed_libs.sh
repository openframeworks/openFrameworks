#!/usr/bin/env bash
# Verify download_libs actually deployed binaries/headers, and that static
# archives are GNU ar (the msys2 "unknown file type" / "treating as linker
# script" failure mode).
set -e

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT"

PLATFORM=""
ARCH=""
ALLOW_INCOMPLETE=0
PASS=0
FAIL=0
WARN=0

printHelp(){
	cat <<EOF
Usage: $0 --platform linux|msys2 --arch <arch> [--allow-incomplete]

  linux   arch: 64, arm64, aarch64, armv6l, armv7l
  msys2   arch: x86_64 (used as lib/msys2/x86_64 nested fallback)
EOF
}

while [[ $# -gt 0 ]]; do
	case "$1" in
		--platform)
			PLATFORM="$2"
			shift 2
			;;
		--arch)
			ARCH="$2"
			shift 2
			;;
		--allow-incomplete)
			ALLOW_INCOMPLETE=1
			shift
			;;
		-h|--help)
			printHelp
			exit 0
			;;
		*)
			echo "Unknown argument: $1"
			printHelp
			exit 1
			;;
	esac
done

if [[ -z "$PLATFORM" || -z "$ARCH" ]]; then
	printHelp
	exit 1
fi

ok() {
	PASS=$((PASS + 1))
	echo "  OK    $*"
}

bad() {
	FAIL=$((FAIL + 1))
	echo "  FAIL  $*"
}

note() {
	WARN=$((WARN + 1))
	echo "  WARN  $*"
}

isGnuAr() {
	local FILE="$1"
	if command -v python3 >/dev/null 2>&1; then
		python3 -c "import sys; sys.exit(0 if open(sys.argv[1],'rb').read(8)==b'!<arch>\n' else 1)" "$FILE"
	elif command -v python >/dev/null 2>&1; then
		python -c "import sys; sys.exit(0 if open(sys.argv[1],'rb').read(8)==b'!<arch>\n' else 1)" "$FILE"
	else
		local MAGIC
		MAGIC=$(dd if="$FILE" bs=7 count=1 2>/dev/null || true)
		[[ "$MAGIC" == '!<arch>' ]]
	fi
}

archiveListsObjects() {
	local FILE="$1"
	local LISTING
	if ! command -v ar >/dev/null 2>&1; then
		return 0
	fi
	LISTING=$(ar t "$FILE" 2>/dev/null || true)
	if [[ -z "$LISTING" ]]; then
		return 1
	fi
	# GNU ar prints foo.o / foo.obj. BSD ar on GNU archives may print /0.
	echo "$LISTING" | grep -v -E '^/{1,2}$' | grep -q .
}

listLibTrees() {
	local PATTERN="$1"
	local ROOTS=()
	[[ -d libs ]] && ROOTS+=(libs)
	[[ -d addons ]] && ROOTS+=(addons)
	if [[ ${#ROOTS[@]} -eq 0 ]]; then
		return 1
	fi
	find "${ROOTS[@]}" -type d -path "$PATTERN" 2>/dev/null | sort
}

requireHeader() {
	local FILE="$1"
	if [[ -f "$FILE" ]]; then
		ok "header $FILE"
	else
		if [[ "$ALLOW_INCOMPLETE" -eq 1 ]]; then
			note "header missing $FILE (incomplete archive allowed)"
		else
			bad "header missing $FILE"
		fi
	fi
}

requireArchive() {
	local FILE="$1"
	if [[ ! -f "$FILE" ]]; then
		if [[ "$ALLOW_INCOMPLETE" -eq 1 ]]; then
			note "archive missing $FILE (incomplete archive allowed)"
		else
			bad "archive missing $FILE"
		fi
		return
	fi
	if [[ ! -s "$FILE" ]]; then
		bad "archive empty $FILE"
		return
	fi
	if isGnuAr "$FILE"; then
		ok "gnu ar magic $FILE ($(wc -c <"$FILE") bytes)"
	else
		bad "not a GNU ar archive $FILE"
		return
	fi
	if archiveListsObjects "$FILE"; then
		ok "ar members $FILE"
	else
		if command -v ar >/dev/null 2>&1; then
			bad "ar could not list object members in $FILE"
			ar t "$FILE" 2>&1 | head -20 || true
		else
			note "no ar on PATH; skipped member listing for $FILE"
		fi
	fi
}

resolveMsys2A() {
	local LIBNAME="$1"
	local ANAME="$2"
	if [[ -f "libs/${LIBNAME}/lib/msys2/${ARCH}/${ANAME}" ]]; then
		echo "libs/${LIBNAME}/lib/msys2/${ARCH}/${ANAME}"
	elif [[ -f "libs/${LIBNAME}/lib/msys2/${ANAME}" ]]; then
		echo "libs/${LIBNAME}/lib/msys2/${ANAME}"
	fi
}

checkMsys2A() {
	local LIBNAME="$1"
	local ANAME="$2"
	local FILE
	FILE=$(resolveMsys2A "$LIBNAME" "$ANAME")
	if [[ -z "$FILE" ]]; then
		bad "missing ${ANAME} for ${LIBNAME} (tried lib/msys2/${ARCH}/ and lib/msys2/)"
		return
	fi
	ok "deployed $FILE"
	if [[ ! -s "$FILE" ]]; then
		bad "archive empty $FILE"
		return
	fi
	if isGnuAr "$FILE"; then
		ok "gnu ar magic $FILE ($(wc -c <"$FILE") bytes)"
	else
		bad "not a GNU ar archive $FILE"
		return
	fi
	if archiveListsObjects "$FILE"; then
		ok "ar members $FILE"
		ar t "$FILE" 2>/dev/null | head -8 || true
	else
		bad "ar could not list object members in $FILE (ld/lld will reject this)"
		ar t "$FILE" 2>&1 | head -20 || true
	fi
}

echo "== deployed libraries  platform=${PLATFORM} arch=${ARCH} =="

if [[ "$PLATFORM" == "linux" ]]; then
	SUBPATH="linux/${ARCH}"
	echo "--- trees under lib/${SUBPATH} ---"
	if listLibTrees "*/lib/${SUBPATH}" | grep -q .; then
		listLibTrees "*/lib/${SUBPATH}"
		ok "found lib/${SUBPATH} folders"
	else
		bad "no lib/${SUBPATH} folders under libs/ or addons/"
	fi

	requireHeader "libs/glm/include/glm/vec2.hpp"
	requireHeader "libs/json/include/nlohmann/json.hpp"
	requireHeader "libs/utf8/include/utf8.h"
	requireArchive "libs/tess2/lib/${SUBPATH}/libtess2.a"
	requireArchive "libs/zlib/lib/${SUBPATH}/zlib.a"
	if [[ -f "libs/kiss/lib/${SUBPATH}/libkiss.a" ]]; then
		requireArchive "libs/kiss/lib/${SUBPATH}/libkiss.a"
	else
		note "kiss not deployed at libs/kiss/lib/${SUBPATH}/libkiss.a"
	fi

elif [[ "$PLATFORM" == "msys2" ]]; then
	echo "--- trees under lib/msys2 ---"
	if listLibTrees '*/lib/msys2' | grep -q .; then
		listLibTrees '*/lib/msys2*'
		ok "found lib/msys2 folders"
	else
		bad "no lib/msys2 folders under libs/ or addons/"
	fi

	checkMsys2A tess2 libtess2.a
	checkMsys2A kiss libkiss.a
	checkMsys2A videoInput videoInput.a
else
	echo "Unsupported platform: $PLATFORM"
	exit 1
fi

echo
echo "Passed: ${PASS}  Failed: ${FAIL}  Warnings: ${WARN}"
if [[ "$FAIL" -ne 0 ]]; then
	exit 1
fi
echo "Deployed library check OK"
