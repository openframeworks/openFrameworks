#!/usr/bin/env bash
# Verify download_libs actually deployed binaries/headers, and that static
# archives are GNU ar (the msys2 "unknown file type" / "treating as linker
# script" failure mode).
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT"

PLATFORM=""
ARCH=""
ALLOW_INCOMPLETE=0

usage() {
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
		-h | --help)
			usage
			exit 0
			;;
		*)
			echo "Unknown argument: $1"
			usage
			exit 1
			;;
	esac
done

if [[ -z "$PLATFORM" || -z "$ARCH" ]]; then
	usage
	exit 1
fi

pass=0
fail=0
warn=0

ok() {
	pass=$((pass + 1))
	echo "  OK    $*"
}

bad() {
	fail=$((fail + 1))
	echo "  FAIL  $*"
}

note() {
	warn=$((warn + 1))
	echo "  WARN  $*"
}

is_gnu_ar() {
	local f="$1"
	if command -v python3 >/dev/null 2>&1; then
		python3 -c "import sys; sys.exit(0 if open(sys.argv[1],'rb').read(8)==b'!<arch>\n' else 1)" "$f"
	elif command -v python >/dev/null 2>&1; then
		python -c "import sys; sys.exit(0 if open(sys.argv[1],'rb').read(8)==b'!<arch>\n' else 1)" "$f"
	else
		# bash / dd fallback: first 7 bytes must be !<arch>
		local magic
		magic=$(dd if="$f" bs=7 count=1 2>/dev/null || true)
		[[ "$magic" == '!<arch>' ]]
	fi
}

archive_lists_objects() {
	local f="$1"
	local listing
	if ! command -v ar >/dev/null 2>&1; then
		return 0
	fi
	listing=$(ar t "$f" 2>/dev/null || true)
	if [[ -z "$listing" ]]; then
		return 1
	fi
	# GNU ar prints foo.o / foo.obj. BSD ar on GNU archives may print /0.
	# Ignore symbol-table members named / or //.
	echo "$listing" | grep -v -E '^/{1,2}$' | grep -q .
}

list_lib_trees() {
	local pattern="$1"
	local roots=()
	[[ -d libs ]] && roots+=(libs)
	[[ -d addons ]] && roots+=(addons)
	if [[ ${#roots[@]} -eq 0 ]]; then
		return 1
	fi
	find "${roots[@]}" -type d -path "$pattern" 2>/dev/null | sort
}

echo "== deployed libraries  platform=${PLATFORM} arch=${ARCH} =="

if [[ "$PLATFORM" == "linux" ]]; then
	SUBPATH="linux/${ARCH}"
	echo "--- trees under lib/${SUBPATH} ---"
	if list_lib_trees "*/lib/${SUBPATH}" | grep -q .; then
		list_lib_trees "*/lib/${SUBPATH}"
		ok "found lib/${SUBPATH} folders"
	else
		bad "no lib/${SUBPATH} folders under libs/ or addons/"
	fi

	require_header() {
		local p="$1"
		if [[ -f "$p" ]]; then
			ok "header $p"
		else
			if [[ "$ALLOW_INCOMPLETE" -eq 1 ]]; then
				note "header missing $p (incomplete archive allowed)"
			else
				bad "header missing $p"
			fi
		fi
	}

	require_archive() {
		local p="$1"
		if [[ ! -f "$p" ]]; then
			if [[ "$ALLOW_INCOMPLETE" -eq 1 ]]; then
				note "archive missing $p (incomplete archive allowed)"
			else
				bad "archive missing $p"
			fi
			return
		fi
		if [[ ! -s "$p" ]]; then
			bad "archive empty $p"
			return
		fi
		if is_gnu_ar "$p"; then
			ok "gnu ar magic $p ($(wc -c <"$p") bytes)"
		else
			bad "not a GNU ar archive $p"
			return
		fi
		if archive_lists_objects "$p"; then
			ok "ar members $p"
		else
			if command -v ar >/dev/null 2>&1; then
				bad "ar could not list object members in $p"
				ar t "$p" 2>&1 | head -20 || true
			else
				note "no ar on PATH; skipped member listing for $p"
			fi
		fi
	}

	require_header "libs/glm/include/glm/vec2.hpp"
	require_header "libs/json/include/nlohmann/json.hpp"
	require_header "libs/utf8/include/utf8.h"
	require_archive "libs/tess2/lib/${SUBPATH}/libtess2.a"
	require_archive "libs/zlib/lib/${SUBPATH}/zlib.a"
	# kiss is named libkiss.a on linux; optional if formula set omitted it
	if [[ -f "libs/kiss/lib/${SUBPATH}/libkiss.a" ]]; then
		require_archive "libs/kiss/lib/${SUBPATH}/libkiss.a"
	else
		note "kiss not deployed at libs/kiss/lib/${SUBPATH}/libkiss.a"
	fi

elif [[ "$PLATFORM" == "msys2" ]]; then
	echo "--- trees under lib/msys2 ---"
	if list_lib_trees '*/lib/msys2' | grep -q .; then
		list_lib_trees '*/lib/msys2*'
		ok "found lib/msys2 folders"
	else
		bad "no lib/msys2 folders under libs/ or addons/"
	fi

	# Prefer nested arch folder, then flat lib/msys2 (same as the makefile).
	resolve_msys2_a() {
		local libname="$1"
		local aname="$2"
		if [[ -f "libs/${libname}/lib/msys2/${ARCH}/${aname}" ]]; then
			echo "libs/${libname}/lib/msys2/${ARCH}/${aname}"
		elif [[ -f "libs/${libname}/lib/msys2/${aname}" ]]; then
			echo "libs/${libname}/lib/msys2/${aname}"
		fi
	}

	check_msys2_a() {
		local libname="$1"
		local aname="$2"
		local p
		p=$(resolve_msys2_a "$libname" "$aname")
		if [[ -z "$p" ]]; then
			bad "missing ${aname} for ${libname} (tried lib/msys2/${ARCH}/ and lib/msys2/)"
			return
		fi
		ok "deployed $p"
		if [[ ! -s "$p" ]]; then
			bad "archive empty $p"
			return
		fi
		if is_gnu_ar "$p"; then
			ok "gnu ar magic $p ($(wc -c <"$p") bytes)"
		else
			bad "not a GNU ar archive $p"
			return
		fi
		if archive_lists_objects "$p"; then
			ok "ar members $p"
			ar t "$p" 2>/dev/null | head -8 || true
		else
			bad "ar could not list object members in $p (ld/lld will reject this)"
			ar t "$p" 2>&1 | head -20 || true
		fi
	}

	check_msys2_a tess2 libtess2.a
	check_msys2_a kiss libkiss.a
	check_msys2_a videoInput videoInput.a
else
	echo "Unsupported platform: $PLATFORM"
	exit 1
fi

echo
echo "Passed: ${pass}  Failed: ${fail}  Warnings: ${warn}"
if [[ "$fail" -ne 0 ]]; then
	exit 1
fi
echo "Deployed library check OK"
