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
echo "Passed: ${PASS}  Failed: ${FAIL}"
if [[ "$FAIL" -ne 0 ]]; then
	exit 1
fi
echo "Linux lib path fallbacks OK"
