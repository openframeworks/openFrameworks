#!/usr/bin/env bash
# Prove makefile lib discovery accepts both:
#   new  libs/<lib>/lib/linux/<arch>
#   old  libs/<lib>/lib/linux64  (and the other pre-0.13 flat names)
set -e

ROOT="$(cd "$(dirname "$0")/../../.." && pwd)"
SHARED="$ROOT/libs/openFrameworksCompiled/project/makefileCommon/config.shared.mk"
PROJECT="$ROOT/libs/openFrameworksCompiled/project/makefileCommon/config.project.mk"
ADDONS="$ROOT/libs/openFrameworksCompiled/project/makefileCommon/config.addons.mk"
PASS=0
FAIL=0
WORKDIR="$(mktemp -d)"
trap 'rm -rf "$WORKDIR"' EXIT

ok() {
	PASS=$((PASS + 1))
	echo "  OK  $*"
}

bad() {
	FAIL=$((FAIL + 1))
	echo "  FAIL  $*"
}

echo "== source contracts =="
if grep -q '^define find_platform_lib_path' "$SHARED"; then
	ok "config.shared.mk defines find_platform_lib_path"
else
	bad "config.shared.mk is missing find_platform_lib_path"
fi
if grep -q 'call find_platform_lib_path' "$PROJECT"; then
	ok "config.project.mk uses find_platform_lib_path for core libs"
else
	bad "config.project.mk does not call find_platform_lib_path"
fi
if grep -q 'call find_platform_lib_path' "$ADDONS"; then
	ok "config.addons.mk uses find_platform_lib_path for addon libs"
else
	bad "config.addons.mk does not call find_platform_lib_path"
fi

echo
echo "== MSYS2 ABI library paths expand without recursion =="
MSYS_MK="$WORKDIR/msys-paths.mk"
MSYS_PATH_BLOCK="$(awk '
	/^# Canonical path first/ { capture=1 }
	/^ifeq \(\$\(PLATFORM_OS\),Linux\)/ { capture=0 }
	capture
' "$SHARED")"
cat >"$MSYS_MK" <<EOF
PLATFORM_LIB_SUBPATH := msys2
ABI_LIB_SUBPATH := msys2
ABI_LEGACY_LIB_SUBPATHS :=
$MSYS_PATH_BLOCK
all:
	@printf '%s' '\$(ABI_LIB_SUBPATHS)'
EOF
GOT_MSYS_X64="$(make -sf "$MSYS_MK" HOST_ARCH=x86_64)"
GOT_MSYS_ARM64="$(make -sf "$MSYS_MK" HOST_ARCH=aarch64)"
if [[ "$GOT_MSYS_X64" == "msys2/x86_64 msys2" ]]; then
	ok "MSYS2 x86_64 paths expand"
else
	bad "MSYS2 x86_64 paths expected 'msys2/x86_64 msys2' got '$GOT_MSYS_X64'"
fi
if [[ "$GOT_MSYS_ARM64" == "msys2/aarch64 msys2/arm64 msys2" ]]; then
	ok "MSYS2 arm64 paths expand"
else
	bad "MSYS2 arm64 paths expected 'msys2/aarch64 msys2/arm64 msys2' got '$GOT_MSYS_ARM64'"
fi

DEFINE="$(awk '/^define find_platform_lib_path/,/^endef/' "$SHARED")"
if [[ -z "$DEFINE" ]]; then
	echo "Could not extract find_platform_lib_path from $SHARED"
	exit 1
fi

# canonical -> legacy aliases (must match PLATFORM_LEGACY_LIB_SUBPATHS)
PAIRS=(
	"linux/64|linux64"
	"linux/arm64|linuxarm64"
	"linux/aarch64|linuxaarch64"
	"linux/armv7l|linuxarmv7l"
	"linux/armv6l|linuxarmv6l"
)

resolve() {
	local CANDIDATES="$1"
	local LIBROOT="$2"
	local MK="$WORKDIR/resolve.mk"
	cat >"$MK" <<EOF
ABI_LIB_SUBPATHS = ${CANDIDATES}
${DEFINE}
all:
	@printf '%s' '\$(call find_platform_lib_path,${LIBROOT})'
EOF
	make -sf "$MK"
}

echo
echo "== find_platform_lib_path (new wins, old is fallback) =="
for PAIR in "${PAIRS[@]}"; do
	CANONICAL="${PAIR%%|*}"
	LEGACY="${PAIR##*|}"
	BASE="$WORKDIR/case-${CANONICAL//\//-}"
	mkdir -p "$BASE/both/lib/${CANONICAL}" "$BASE/both/lib/${LEGACY}"
	mkdir -p "$BASE/new/lib/${CANONICAL}"
	mkdir -p "$BASE/old/lib/${LEGACY}"
	mkdir -p "$BASE/empty/lib"
	touch "$BASE/both/lib/${CANONICAL}/libtess2.a" "$BASE/both/lib/${LEGACY}/libtess2.a"
	touch "$BASE/new/lib/${CANONICAL}/libtess2.a"
	touch "$BASE/old/lib/${LEGACY}/libtess2.a"

	CANDIDATES="${CANONICAL} ${LEGACY}"
	GOT_BOTH="$(resolve "$CANDIDATES" "$BASE/both")"
	GOT_NEW="$(resolve "$CANDIDATES" "$BASE/new")"
	GOT_OLD="$(resolve "$CANDIDATES" "$BASE/old")"
	GOT_EMPTY="$(resolve "$CANDIDATES" "$BASE/empty")"

	if [[ "$GOT_BOTH" == "$BASE/both/lib/${CANONICAL}" ]]; then
		ok "${CANONICAL}: both present -> new path"
	else
		bad "${CANONICAL}: both present expected .../lib/${CANONICAL} got '${GOT_BOTH}'"
	fi
	if [[ "$GOT_NEW" == "$BASE/new/lib/${CANONICAL}" ]]; then
		ok "${CANONICAL}: only new -> new path"
	else
		bad "${CANONICAL}: only new expected .../lib/${CANONICAL} got '${GOT_NEW}'"
	fi
	if [[ "$GOT_OLD" == "$BASE/old/lib/${LEGACY}" ]]; then
		ok "${CANONICAL}: only old ${LEGACY} -> old path"
	else
		bad "${CANONICAL}: only old expected .../lib/${LEGACY} got '${GOT_OLD}'"
	fi
	if [[ -z "$GOT_EMPTY" ]]; then
		ok "${CANONICAL}: neither -> empty"
	else
		bad "${CANONICAL}: neither expected empty got '${GOT_EMPTY}'"
	fi
done

echo
echo "== PLATFORM_LEGACY_LIB_SUBPATHS aliases in config.shared.mk =="
for PAIR in "${PAIRS[@]}"; do
	CANONICAL="${PAIR%%|*}"
	LEGACY="${PAIR##*|}"
	if grep -q "PLATFORM_LIB_SUBPATH),${CANONICAL})" "$SHARED" && grep -q "${LEGACY}" "$SHARED"; then
		ok "makefile still maps ${CANONICAL} <-> ${LEGACY}"
	else
		bad "makefile missing ${CANONICAL} / ${LEGACY} alias"
	fi
done

echo
echo "== Raspberry Pi detection (downloader + makefile agree) =="
if grep -q 'etc/rpi-issue' "$SHARED" && grep -q 'raspberrypi' "$SHARED"; then
	ok "config.shared.mk RPI_DETECTED accepts RPi OS userland markers"
else
	bad "config.shared.mk RPI_DETECTED is missing RPi OS userland markers"
fi
DL="$ROOT/scripts/dev/download_libs.sh"
FN="$WORKDIR/is_raspberry_pi.sh"
sed -n '/^is_raspberry_pi(){/,/^}/p' "$DL" >"$FN"
if [[ -s "$FN" ]]; then
	ok "download_libs.sh defines is_raspberry_pi"
else
	bad "could not extract is_raspberry_pi from download_libs.sh"
fi
rpi_fixture() {
	local dir="$WORKDIR/rpi-$1"
	mkdir -p "$dir/etc/apt/sources.list.d"
	printf 'ID=debian\nPRETTY_NAME="Debian GNU/Linux 13 (trixie)"\n' >"$dir/etc/os-release"
	case "$1" in
		trixie) printf 'deb http://archive.raspberrypi.org/debian/ trixie main\n' >"$dir/etc/apt/sources.list.d/raspi.list" ;;
		devicetree)
			mkdir -p "$dir/proc/device-tree"
			printf 'Raspberry Pi 5 Model B\0' >"$dir/proc/device-tree/model" ;;
	esac
	(
		source "$FN"
		OF_RPI_PROBE_ROOT="$dir" is_raspberry_pi
	)
}
if rpi_fixture trixie; then
	ok "RPi OS trixie userland (raspi apt repo, ID=debian) detected"
else
	bad "RPi OS trixie userland not detected"
fi
if rpi_fixture devicetree; then
	ok "device-tree Raspberry Pi model detected"
else
	bad "device-tree Raspberry Pi model not detected"
fi
if rpi_fixture plain; then
	bad "plain Debian fixture wrongly detected as Raspberry Pi"
else
	ok "plain Debian fixture not detected as Raspberry Pi"
fi

echo
echo "== Linux uses the apothecary FreeImage (not the system one) =="
LINUX_COMMON="$ROOT/libs/openFrameworksCompiled/project/makefileCommon/config.linux.common.mk"
if grep -q 'PLATFORM_CORE_EXCLUSIONS += $(OF_LIBS_PATH)/FreeImage/%' "$LINUX_COMMON"; then
	bad "config.linux.common.mk still excludes the bundled FreeImage"
else
	ok "config.linux.common.mk does not exclude the bundled FreeImage"
fi
if grep -qE '^PLATFORM_LIBRARIES \+= freeimage' "$LINUX_COMMON"; then
	bad "config.linux.common.mk still links system -lfreeimage"
else
	ok "config.linux.common.mk does not link system -lfreeimage"
fi

echo
echo "== Jetson detection can be forced (containers have no device-tree) =="
if grep -q '^JETSON_DETECTED ?=' "$SHARED"; then
	ok "config.shared.mk allows JETSON_DETECTED override"
else
	bad "config.shared.mk does not allow JETSON_DETECTED override"
fi

echo
echo "== Jetson uses the Ubuntu arm64 (linux/arm64) libs =="
if grep -q 'Jetson.*Ubuntu arm64 libs' "$DL"; then
	ok "download_libs.sh maps Jetson to the Ubuntu arm64 libs"
else
	bad "download_libs.sh does not map Jetson to the Ubuntu arm64 libs"
fi
if grep -A2 'JETSON_DETECTED),yes' "$SHARED" | grep -q 'PLATFORM_LIB_SUBPATH=linux/arm64'; then
	ok "makefile Jetson path uses linux/arm64 libs"
else
	bad "makefile Jetson path does not use linux/arm64 libs"
fi

echo
echo "Passed: ${PASS}  Failed: ${FAIL}"
if [[ "$FAIL" -ne 0 ]]; then
	exit 1
fi
echo "Linux lib path fallbacks OK"
