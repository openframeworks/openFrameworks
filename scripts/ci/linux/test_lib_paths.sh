#!/usr/bin/env bash
# Prove makefile lib discovery accepts both:
#   new  libs/<lib>/lib/linux/<arch>
#   old  libs/<lib>/lib/linux64  (and the other pre-0.13 flat names)
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../../.." && pwd)"
SHARED="$ROOT/libs/openFrameworksCompiled/project/makefileCommon/config.shared.mk"
PROJECT="$ROOT/libs/openFrameworksCompiled/project/makefileCommon/config.project.mk"
ADDONS="$ROOT/libs/openFrameworksCompiled/project/makefileCommon/config.addons.mk"

fail=0
pass=0

ok() {
	pass=$((pass + 1))
	echo "  OK  $*"
}

bad() {
	fail=$((fail + 1))
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

WORKDIR="$(mktemp -d)"
trap 'rm -rf "$WORKDIR"' EXIT

DEFINE="$(awk '/^define find_platform_lib_path/,/^endef/' "$SHARED")"
if [[ -z "$DEFINE" ]]; then
	echo "Could not extract find_platform_lib_path from $SHARED"
	exit 1
fi

# canonical -> legacy aliases (must match PLATFORM_LEGACY_LIB_SUBPATHS)
pairs=(
	"linux/64|linux64"
	"linux/arm64|linuxarm64"
	"linux/aarch64|linuxaarch64"
	"linux/armv7l|linuxarmv7l"
	"linux/armv6l|linuxarmv6l"
)

resolve() {
	local candidates="$1"
	local libroot="$2"
	local mk="$WORKDIR/resolve.mk"
	cat >"$mk" <<EOF
ABI_LIB_SUBPATHS = ${candidates}
${DEFINE}
all:
	@printf '%s' '\$(call find_platform_lib_path,${libroot})'
EOF
	make -sf "$mk"
}

echo
echo "== find_platform_lib_path (new wins, old is fallback) =="
for pair in "${pairs[@]}"; do
	canonical="${pair%%|*}"
	legacy="${pair##*|}"
	base="$WORKDIR/case-${canonical//\//-}"
	mkdir -p "$base/both/lib/${canonical}" "$base/both/lib/${legacy}"
	mkdir -p "$base/new/lib/${canonical}"
	mkdir -p "$base/old/lib/${legacy}"
	mkdir -p "$base/empty/lib"
	touch "$base/both/lib/${canonical}/libtess2.a" "$base/both/lib/${legacy}/libtess2.a"
	touch "$base/new/lib/${canonical}/libtess2.a"
	touch "$base/old/lib/${legacy}/libtess2.a"

	candidates="${canonical} ${legacy}"
	got_both="$(resolve "$candidates" "$base/both")"
	got_new="$(resolve "$candidates" "$base/new")"
	got_old="$(resolve "$candidates" "$base/old")"
	got_empty="$(resolve "$candidates" "$base/empty")"

	if [[ "$got_both" == "$base/both/lib/${canonical}" ]]; then
		ok "${canonical}: both present -> new path"
	else
		bad "${canonical}: both present expected .../lib/${canonical} got '${got_both}'"
	fi
	if [[ "$got_new" == "$base/new/lib/${canonical}" ]]; then
		ok "${canonical}: only new -> new path"
	else
		bad "${canonical}: only new expected .../lib/${canonical} got '${got_new}'"
	fi
	if [[ "$got_old" == "$base/old/lib/${legacy}" ]]; then
		ok "${canonical}: only old ${legacy} -> old path"
	else
		bad "${canonical}: only old expected .../lib/${legacy} got '${got_old}'"
	fi
	if [[ -z "$got_empty" ]]; then
		ok "${canonical}: neither -> empty"
	else
		bad "${canonical}: neither expected empty got '${got_empty}'"
	fi
done

echo
echo "== PLATFORM_LEGACY_LIB_SUBPATHS aliases in config.shared.mk =="
for pair in "${pairs[@]}"; do
	canonical="${pair%%|*}"
	legacy="${pair##*|}"
	# arm64 also lists linuxaarch64 as a second alias
	if grep -q "PLATFORM_LIB_SUBPATH),${canonical})" "$SHARED" && grep -q "${legacy}" "$SHARED"; then
		ok "makefile still maps ${canonical} <-> ${legacy}"
	else
		bad "makefile missing ${canonical} / ${legacy} alias"
	fi
done

echo
echo "Passed: ${pass}  Failed: ${fail}"
if [[ "$fail" -ne 0 ]]; then
	exit 1
fi
echo "Linux lib path fallbacks OK"
