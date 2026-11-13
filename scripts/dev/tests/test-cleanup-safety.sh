#!/usr/bin/env bash
# Safety tests for `of cleanup` (scripts/of.sh): confirmation, path and platform
# validation, symlinks, odd file names (newline, Chinese, spaces), the usage
# report, the cleanup lock and the running-build guard.
#
# Runs against a throwaway copy of the scripts, never the real tree, and only
# ever deletes inside it. Project cleaning is exercised with --dry-run only
# (a real project clean sends files to the Trash / Recycle Bin).
set -u
ROOT=$(cd "$(dirname "$0")/../../.." && pwd)
TEMP=$(mktemp -d)
BG_PIDS=()
cleanup(){
	local p
	for p in ${BG_PIDS[@]+"${BG_PIDS[@]}"}; do kill "$p" 2>/dev/null; done
	rm -rf "$TEMP"
}
trap cleanup EXIT

case "$(uname -s)" in MINGW*|MSYS*|CYGWIN*) IS_WIN=1 ;; *) IS_WIN=0 ;; esac
# Git Bash on a CI runner has no UTF-8 locale unless asked for one
[[ $IS_WIN -eq 1 ]] && export LC_ALL=C.UTF-8
# capabilities of this machine: skip, rather than fail, what it cannot do
CAN_LINK=0
: > "$TEMP/.lt-target"
ln -s .lt-target "$TEMP/.lt-link" 2>/dev/null && [[ -L "$TEMP/.lt-link" ]] && CAN_LINK=1   # Git Bash copies instead of linking
rm -rf "$TEMP/.lt-link" "$TEMP/.lt-target"
CAN_UTF8=0
mkdir "$TEMP/中文" 2>/dev/null && [[ "$(ls "$TEMP" | grep -c '^中文$')" == "1" ]] && CAN_UTF8=1
rmdir "$TEMP/中文" 2>/dev/null
CAN_NEWLINE=0
mkdir "$TEMP/$(printf 'nl\ntest')" 2>/dev/null && CAN_NEWLINE=1   # NTFS refuses newlines in names
rmdir "$TEMP/$(printf 'nl\ntest')" 2>/dev/null
echo "platform: $(uname -s) $(uname -m) · bash ${BASH_VERSION} · symlinks: $CAN_LINK · newline names: $CAN_NEWLINE · utf-8 names: $CAN_UTF8"

TREE="$TEMP/of"
OUTSIDE="$TEMP/outside"
FAILS=0
CHECKS=0

pass(){ CHECKS=$((CHECKS + 1)); }
fail(){ CHECKS=$((CHECKS + 1)); FAILS=$((FAILS + 1)); printf 'FAIL: %s\n' "$1" >&2; }
expect(){ # expect "description" command...
	local d="$1"; shift
	if "$@"; then pass; else fail "$d"; fi
}

# of <args…>: runs the CLI with no TTY, sets OUT (colours stripped) and RC.
of(){
	OUT="$("$BASH" "$TREE/scripts/of.sh" "$@" </dev/null 2>&1)"
	RC=$?
	OUT="$(printf '%s' "$OUT" | sed $'s/\x1b\\[[0-9;?]*[a-zA-Z]//g')"
}

mk_tree(){
	rm -rf "$TREE" "$OUTSIDE"
	mkdir -p "$TREE" "$OUTSIDE/proj/src" "$OUTSIDE/proj/obj"
	cp -R "$ROOT/scripts" "$TREE/scripts"
	(
		cd "$TREE" || exit 1
		mkdir -p libs/openFrameworks libs/zlib/lib/macos libs/zlib/lib/linux64 libs/download \
			libs/openFrameworksCompiled/project/obj addons/ofxA/libs/x/lib/ios \
			apps/myApps/p1/src apps/myApps/p1/obj apps/myApps/p1/bin/data
		head -c 200000 /dev/zero > libs/zlib/lib/macos/z.a
		head -c 100000 /dev/zero > libs/download/a.zip
		head -c 50000 /dev/zero > apps/myApps/p1/obj/o.o
		: > apps/myApps/p1/bin/data/keep
		: > "$OUTSIDE/proj/obj/precious.o"
	)
}

# --- confirmation ---------------------------------------------------------
mk_tree
of cleanup libs all
expect "libs all without --yes refuses (no TTY)" test "$RC" -ne 0
expect "libs all without --yes deletes nothing" test -d "$TREE/libs/zlib"
of cleanup caches all
expect "caches all without --yes refuses (no TTY)" test "$RC" -ne 0
expect "caches all without --yes deletes nothing" test -f "$TREE/libs/download/a.zip"
of cleanup libs all --dry-run
expect "dry run succeeds" test "$RC" -eq 0
expect "dry run deletes nothing" test -d "$TREE/libs/zlib"
expect "dry run reports would-remove" grep -q "would remove" <<<"$OUT"

# --- platform whitelist -----------------------------------------------------
for bad in '*' '../x' 'a/b' '.' '..' 'bogus' 'macos*' 'linux/../x' 'linux/*' 'linux/bogus' 'macos/64' '/linux' 'linux/'; do
	of cleanup libs "$bad" --yes
	expect "platform '$bad' rejected" test "$RC" -ne 0
done
expect "rejected platforms deleted nothing" test -d "$TREE/libs/zlib/lib/linux64"

# --- folders without library binaries are not prebuilt trees ----------------------------
mk_tree
mkdir -p "$TREE/addons/ofxOsc/libs/oscpack/src" "$TREE/addons/ofxGui/libs" "$TREE/libs/glm/include" "$TREE/addons/ofxBin/libs/foo/lib/macos"
: > "$TREE/addons/ofxOsc/libs/oscpack/src/osc.cpp"; : > "$TREE/addons/ofxGui/libs/.gitkeep"; : > "$TREE/libs/glm/include/glm.hpp"
head -c 30000 /dev/zero > "$TREE/addons/ofxBin/libs/foo/lib/macos/foo.a"
of cleanup usage libs --tsv
expect "usage omits an addon libs folder holding only source" bash -c '! grep -q "ofxOsc/libs" <<<"$1"' _ "$OUT"
expect "usage omits an addon libs folder holding only .gitkeep" bash -c '! grep -q "ofxGui/libs" <<<"$1"' _ "$OUT"
expect "usage omits a header-only libs folder" bash -c '! grep -q "libs/glm" <<<"$1"' _ "$OUT"
expect "usage lists a libs folder with a library" grep -q "addons/ofxBin/libs" <<<"$OUT"
expect "usage lists libs/zlib (has z.a)" grep -q "libs/zlib" <<<"$OUT"
of cleanup libs all --yes
expect "libs all keeps addon source folders" test -f "$TREE/addons/ofxOsc/libs/oscpack/src/osc.cpp" -a -f "$TREE/addons/ofxGui/libs/.gitkeep"
expect "libs all keeps a header-only folder" test -f "$TREE/libs/glm/include/glm.hpp"
expect "libs all removes folders that hold libraries" test ! -e "$TREE/libs/zlib" -a ! -e "$TREE/addons/ofxBin/libs"

# --- platform names: new lib/linux/<arch> and lib/msys2/<env> layouts ------------
mk_tree
for spec in linux/64 linux/arm64 linux/aarch64 linux/armv6l linux/armv7l linux/jetson \
	linux64 linux64gcc6 linuxarm64 linuxaarch64 linuxarmv6l linuxarmv7l \
	msys2/ucrt64 msys2/clang64 msys2/clangarm64 msys2/mingw64 msys2/mingw32 tvos xros; do
	mkdir -p "$TREE/libs/zlib/lib/$spec"
	of cleanup libs "$spec" --dry-run
	expect "platform '$spec' is accepted" test "$RC" -eq 0
	expect "platform '$spec' dry run finds its folder" grep -q "lib/$spec" <<<"$OUT"
done
of cleanup libs linux/arm64 --yes
expect "libs linux/arm64 removes only that arch" test ! -e "$TREE/libs/zlib/lib/linux/arm64" -a -d "$TREE/libs/zlib/lib/linux/64"
of cleanup libs msys2/ucrt64 --yes
expect "libs msys2/ucrt64 removes only that environment" test ! -e "$TREE/libs/zlib/lib/msys2/ucrt64" -a -d "$TREE/libs/zlib/lib/msys2/clang64"
of cleanup libs linux --yes
expect "libs linux removes the whole family" test ! -e "$TREE/libs/zlib/lib/linux"
expect "libs linux leaves the older linux64 folder" test -d "$TREE/libs/zlib/lib/linux64"

# --- other-platforms keep-list ------------------------------------------------------
other_tree(){
	mk_tree
	local d
	for d in macos osx ios tvos xros android emscripten vs linux64 linux64gcc6 linuxarm64 linuxaarch64 linuxarmv7l linuxarmv6l \
		linux/64 linux/arm64 linux/aarch64 linux/armv7l linux/armv6l linux/jetson \
		msys2/ucrt64 msys2/clang64 msys2/clangarm64 msys2/mingw64 weird-platform include; do
		mkdir -p "$TREE/libs/zlib/lib/$d"
	done
	: > "$TREE/libs/zlib/lib/weird-platform/keep"
}
has(){ test -e "$TREE/libs/zlib/lib/$1"; }
# PLATFORM / ARCH are forced through the script's own override
runplat(){ OUT="$(PLATFORM="$1" MSYSTEM="${2:-}" "$BASH" "$TREE/scripts/of.sh" cleanup libs other --yes </dev/null 2>&1)"; RC=$?; }
other_tree; runplat osx
expect "osx host: succeeds" test "$RC" -eq 0
expect "osx host: keeps macos/osx/ios/tvos/xros" "$BASH" -c "$(declare -f has); TREE='$TREE'; has macos && has osx && has ios && has tvos && has xros"
expect "osx host: removes linux, msys2, android, emscripten, vs" "$BASH" -c "$(declare -f has); TREE='$TREE'; ! has linux64 && ! has linux && ! has msys2 && ! has android && ! has emscripten && ! has vs"
expect "osx host: keeps an unknown folder and include/" "$BASH" -c "$(declare -f has); TREE='$TREE'; has weird-platform/keep && has include"
other_tree; runplat linux
expect "linux host (arch unknown): keeps every linux layout" "$BASH" -c "$(declare -f has); TREE='$TREE'; has linux64 && has linuxarm64 && has linuxarmv7l && has linux/64 && has linux/arm64 && has linux/jetson"
expect "linux host: removes macos, ios, android, vs, msys2" "$BASH" -c "$(declare -f has); TREE='$TREE'; ! has macos && ! has ios && ! has android && ! has vs && ! has msys2"
expect "linux host: keeps unknown folder" "$BASH" -c "$(declare -f has); TREE='$TREE'; has weird-platform/keep"
other_tree; runplat msys2 UCRT64
expect "msys2 ucrt64 host: keeps msys2/ucrt64 and vs" "$BASH" -c "$(declare -f has); TREE='$TREE'; has msys2/ucrt64 && has vs"
expect "msys2 ucrt64 host: removes the other msys2 environments" "$BASH" -c "$(declare -f has); TREE='$TREE'; ! has msys2/clang64 && ! has msys2/clangarm64 && ! has msys2/mingw64"
expect "msys2 host: removes linux and macos" "$BASH" -c "$(declare -f has); TREE='$TREE'; ! has linux64 && ! has linux && ! has macos"
# keep one chosen platform: of cleanup libs other <keep>
keepplat(){ OUT="$(PLATFORM=osx "$BASH" "$TREE/scripts/of.sh" cleanup libs other "$1" --yes </dev/null 2>&1)"; RC=$?; }
other_tree; keepplat linux/arm64
expect "keep linux/arm64: succeeds" test "$RC" -eq 0
expect "keep linux/arm64: keeps linux/arm64 and the flat linuxarm64" bash -c "$(declare -f has); TREE='$TREE'; has linux/arm64 && has linuxarm64"
expect "keep linux/arm64: removes the host-independent rest" bash -c "$(declare -f has); TREE='$TREE'; ! has macos && ! has ios && ! has vs && ! has linux/64 && ! has linux64 && ! has linux/armv7l && ! has msys2 && ! has android"
expect "keep linux/arm64: keeps an unknown folder" bash -c "$(declare -f has); TREE='$TREE'; has weird-platform/keep"
other_tree; keepplat linux64gcc6
expect "keep linux64gcc6: keeps the whole x86_64 family (new and older names)" bash -c "$(declare -f has); TREE='$TREE'; has linux/64 && has linux64 && has linux64gcc6 && ! has linux/arm64 && ! has linuxarm64 && ! has linuxarmv7l"
other_tree; keepplat linuxarmv7l
expect "keep linuxarmv7l (older name): keeps linux/armv7l too, drops the others" bash -c "$(declare -f has); TREE='$TREE'; has linuxarmv7l && has linux/armv7l && ! has linux64 && ! has linux64gcc6 && ! has linux/64 && ! has linuxarmv6l"
other_tree; keepplat linux
expect "keep linux: keeps every linux layout, drops the rest" bash -c "$(declare -f has); TREE='$TREE'; has linux/64 && has linux64 && has linux64gcc6 && has linuxarm64 && has linuxarmv6l && has linux/jetson && ! has macos && ! has vs"
other_tree; keepplat macos
expect "keep macos: keeps macos and osx" bash -c "$(declare -f has); TREE='$TREE'; has macos && has osx"
expect "keep macos: removes ios, tvos, xros even on a Mac host" bash -c "$(declare -f has); TREE='$TREE'; ! has ios && ! has tvos && ! has xros && ! has linux64 && ! has vs"
other_tree; keepplat msys2/clang64
expect "keep msys2/clang64: keeps only that environment" bash -c "$(declare -f has); TREE='$TREE'; has msys2/clang64 && ! has msys2/ucrt64 && ! has msys2/clangarm64 && ! has macos"
other_tree; keepplat bogus
expect "keep bogus: rejected" test "$RC" -ne 0
expect "keep bogus: deleted nothing" bash -c "$(declare -f has); TREE='$TREE'; has macos && has linux64"

# nothing to remove: say so, do not ask for confirmation
mk_tree
rm -rf "$TREE/libs/zlib/lib/linux64" "$TREE/addons/ofxA/libs/x/lib/ios"
OUT="$(PLATFORM=osx "$BASH" "$TREE/scripts/of.sh" cleanup libs other </dev/null 2>&1)"; RC=$?
expect "no other platforms: exit 0 without --yes (nothing to confirm)" test "$RC" -eq 0
expect "no other platforms: says so" grep -q "no other platforms to remove" <<<"$OUT"
expect "no other platforms: no confirmation refusal" bash -c '! grep -q "refusing" <<<"$1"' _ "$OUT"
OUT="$(PLATFORM=osx "$BASH" "$TREE/scripts/of.sh" cleanup libs linux64 </dev/null 2>&1)"; RC=$?
expect "no lib/linux64 folders: says so, exit 0" bash -c '[[ "$1" == 0 ]] && grep -q "no lib/linux64 folders to remove" <<<"$2"' _ "$RC" "$OUT"
rm -rf "$TREE/libs/zlib"
OUT="$("$BASH" "$TREE/scripts/of.sh" cleanup libs all </dev/null 2>&1)"; RC=$?
expect "no prebuilt libraries: says so, exit 0" bash -c '[[ "$1" == 0 ]] && grep -q "no prebuilt libraries to remove" <<<"$2"' _ "$RC" "$OUT"
OUT="$("$BASH" "$TREE/scripts/of.sh" cleanup core all </dev/null 2>&1)"; RC=$?
expect "no compiled core: says so, exit 0" bash -c '[[ "$1" == 0 ]] && grep -q "no compiled core to remove" <<<"$2"' _ "$RC" "$OUT"

# linux host with a known architecture: keep only that arch (both layouts)
archplat(){ OUT="$(PLATFORM=linux OF_CLEAN_ARCH="$1" "$BASH" "$TREE/scripts/of.sh" cleanup libs other --yes </dev/null 2>&1)"; RC=$?; }
other_tree; archplat x86_64
expect "linux x86_64: keeps linux/64 and linux64" "$BASH" -c "$(declare -f has); TREE='$TREE'; has linux/64 && has linux64"
expect "linux x86_64: removes other arches (both layouts)" "$BASH" -c "$(declare -f has); TREE='$TREE'; ! has linux/arm64 && ! has linux/aarch64 && ! has linux/armv7l && ! has linux/armv6l && ! has linux/jetson && ! has linuxarm64 && ! has linuxaarch64 && ! has linuxarmv7l && ! has linuxarmv6l"
other_tree; archplat aarch64
expect "linux aarch64: keeps linux/arm64, linux/aarch64, linux/jetson and the flat arm64 names" "$BASH" -c "$(declare -f has); TREE='$TREE'; has linux/arm64 && has linux/aarch64 && has linux/jetson && has linuxarm64 && has linuxaarch64"
expect "linux aarch64: removes x86 and 32-bit arm" "$BASH" -c "$(declare -f has); TREE='$TREE'; ! has linux/64 && ! has linux64 && ! has linux/armv7l && ! has linuxarmv7l && ! has linux/armv6l"
other_tree; archplat armv7l
expect "linux armv7l: keeps only armv7l" "$BASH" -c "$(declare -f has); TREE='$TREE'; has linux/armv7l && has linuxarmv7l && ! has linux/armv6l && ! has linux/64 && ! has linux/arm64 && ! has linuxarmv6l"
other_tree; runplat vs
expect "vs host: keeps vs only" "$BASH" -c "$(declare -f has); TREE='$TREE'; has vs && ! has msys2 && ! has macos && ! has linux"

# --- CMake build trees (any directory name, found by CMakeCache.txt) ---------------
mk_tree
CM="$TREE/apps/myApps/cm"
mkdir -p "$CM/src" "$CM/bin/data" "$CM/build-ninja" "$CM/out/macos" "$CM/stale" "$CM/other" "$CM/notmine"
printf 'CMAKE_HOME_DIRECTORY:INTERNAL=%s\n' "$CM" > "$CM/build-ninja/CMakeCache.txt"
printf 'CMAKE_HOME_DIRECTORY:INTERNAL=%s\n' "$CM" > "$CM/out/macos/CMakeCache.txt"
printf 'CMAKE_HOME_DIRECTORY:INTERNAL=%s\n' "$TEMP/gone-project" > "$CM/stale/CMakeCache.txt"
printf 'CMAKE_HOME_DIRECTORY:INTERNAL=%s\n' "$OUTSIDE/proj" > "$CM/notmine/CMakeCache.txt"
printf 'CMAKE_HOME_DIRECTORY:INTERNAL=%s\n' "$CM" > "$CM/CMakeCache.txt"        # in-source build: the project itself
head -c 90000 /dev/zero > "$CM/build-ninja/obj.bin"
: > "$CM/bin/data/keep"
of cleanup usage projects --tsv
expect "usage finds the CMake build tree by CMakeCache.txt" test "$RC" -eq 0
expect "usage lists the project with a CMake tree" grep -q "apps/myApps/cm" <<<"$OUT"
of cleanup projects apps/myApps/cm --dry-run
expect "a project with only CMake build trees is cleaned (dry run)" grep -q "apps/myApps/cm" <<<"$OUT"
sed -n '/^cleanWordIn(){/,/^}/p;/^cleanFindCMakeBuildDirs(){/,/^}/p' "$TREE/scripts/of.sh" > "$TEMP/cmake-fn.sh"
OUT="$("$BASH" -c '. "$1"; cleanFindCMakeBuildDirs "$2" | tr "\0" "\n"' _ "$TEMP/cmake-fn.sh" "$CM" 2>&1)"
expect "finds build-ninja and out/macos" grep -q "build-ninja" <<<"$OUT"
expect "finds nested out/macos" grep -q "out/macos" <<<"$OUT"
expect "finds a stale tree whose project moved" grep -q "/stale" <<<"$OUT"
expect "skips a tree that belongs to another source dir" bash -c '! grep -q "/notmine" <<<"$1"' _ "$OUT"
expect "never returns the project itself (in-source build)" bash -c '! grep -qx "$2" <<<"$1"' _ "$OUT" "$CM"
# CMake-only project (no src/, Makefile or IDE project) is still a project
mkdir -p "$TREE/apps/myApps/cmonly/build"; : > "$TREE/apps/myApps/cmonly/CMakeLists.txt"; head -c 60000 /dev/zero > "$TREE/apps/myApps/cmonly/build/x"
of cleanup projects all --dry-run
expect "a CMakeLists.txt-only project is found" grep -q "apps/myApps/cmonly" <<<"$OUT"
if [[ "$(uname -s)" == "Linux" ]] && ! command -v gio >/dev/null 2>&1 && ! command -v trash-put >/dev/null 2>&1; then
	of cleanup projects apps/myApps/cm --yes
	expect "real clean removes the CMake trees" test ! -e "$CM/build-ninja" -a ! -e "$CM/out/macos" -a ! -e "$CM/stale"
	expect "real clean keeps bin/, src/ and the other project's tree" test -f "$CM/bin/data/keep" -a -d "$CM/src" -a -d "$CM/notmine"
	expect "real clean keeps the project's own in-source CMakeCache.txt" test -f "$CM/CMakeCache.txt"
fi

# --- compiled core: libs/openFrameworksCompiled/lib/<platform> ---------------------------
mk_tree
C="$TREE/libs/openFrameworksCompiled/lib"
for d in osx macos ios linux64 linuxarm64 linux/64 linux/arm64 vs msys2/ucrt64 msys2/clang64 weird; do
	mkdir -p "$C/$d"; head -c 40000 /dev/zero > "$C/$d/libopenFrameworks.a"
done
of cleanup core all
expect "core all without --yes refuses (no TTY)" test "$RC" -ne 0
expect "core all refusal deleted nothing" test -d "$C/macos"
of cleanup core all --dry-run
expect "core all dry run deletes nothing" test -d "$C/macos" -a -d "$C/linux64"
of cleanup core bogus --yes
expect "core rejects an unknown mode" test "$RC" -ne 0
PLATFORM=linux OF_CLEAN_ARCH=x86_64 "$BASH" "$TREE/scripts/of.sh" cleanup core host --yes </dev/null >/dev/null 2>&1
expect "core host (linux x86_64) removes linux/64 and linux64" test ! -e "$C/linux/64" -a ! -e "$C/linux64"
expect "core host keeps other arches and other platforms" test -d "$C/linux/arm64" -a -d "$C/linuxarm64" -a -d "$C/macos" -a -d "$C/vs"
PLATFORM=osx "$BASH" "$TREE/scripts/of.sh" cleanup core host --yes </dev/null >/dev/null 2>&1
expect "core host (osx) removes macos, osx and ios" test ! -e "$C/macos" -a ! -e "$C/osx" -a ! -e "$C/ios"
expect "core host (osx) keeps linux and msys2" test -d "$C/linux/arm64" -a -d "$C/msys2/ucrt64"
of cleanup core all --yes
expect "core all removes every known platform" test ! -e "$C/linux" -a ! -e "$C/msys2" -a ! -e "$C/vs" -a ! -e "$C/linuxarm64"
expect "core all keeps an unknown folder" test -d "$C/weird"
of cleanup usage core --tsv
expect "usage core succeeds" test "$RC" -eq 0

# --- path validation ----------------------------------------------------
for bad in / .. "$OUTSIDE" "$TREE" "$TREE/libs" "$TREE/scripts" "$TREE/.git" libs; do
	of cleanup projects "$bad" --yes
	expect "projects path '$bad' rejected" test "$RC" -ne 0
done
expect "outside project untouched" test -f "$OUTSIDE/proj/obj/precious.o"
if [[ $CAN_LINK -eq 1 ]]; then
	ln -s "$OUTSIDE" "$TREE/apps/myApps/link"
	of cleanup projects apps/myApps/link/proj --yes
	expect "symlinked project path escaping the tree rejected" test "$RC" -ne 0
	expect "symlink target untouched" test -f "$OUTSIDE/proj/obj/precious.o"
fi
of cleanup projects apps/myApps/p1 --dry-run
expect "a single project path is cleaned (dry run)" grep -q "apps/myApps/p1" <<<"$OUT"
expect "dry run leaves obj" test -f "$TREE/apps/myApps/p1/obj/o.o"
rm -f "$TREE/apps/myApps/link"

# --- empty / wrong OF_DIR ---------------------------------------------------
mkdir -p "$TEMP/bare/scripts"
cp -R "$ROOT/scripts/." "$TEMP/bare/scripts/"
OUT="$("$BASH" "$TEMP/bare/scripts/of.sh" cleanup libs all --yes </dev/null 2>&1)"; RC=$?
expect "a tree without libs/openFrameworks is refused" test "$RC" -ne 0
rm -rf "$TEMP/bare"

# --- symlinks inside lib/ ---------------------------------------------------
if [[ $CAN_LINK -eq 1 ]]; then
	mk_tree
	mkdir -p "$OUTSIDE/realmac"; : > "$OUTSIDE/realmac/keep.a"
	rm -rf "$TREE/libs/zlib/lib/macos"
	ln -s "$OUTSIDE/realmac" "$TREE/libs/zlib/lib/macos"
	of cleanup libs macos --yes
	expect "symlinked lib/<platform> is removed" test ! -e "$TREE/libs/zlib/lib/macos" -a ! -L "$TREE/libs/zlib/lib/macos"
	expect "symlink target kept" test -f "$OUTSIDE/realmac/keep.a"
	# a symlink inside a folder that is deleted must not be followed
	mkdir -p "$TREE/libs/zlib/lib/ios/inner"
	ln -s "$OUTSIDE/realmac" "$TREE/libs/zlib/lib/ios/inner/link"
	of cleanup libs ios --yes
	expect "folder with an inner symlink removed" test ! -e "$TREE/libs/zlib/lib/ios"
	expect "inner symlink target kept" test -f "$OUTSIDE/realmac/keep.a"
fi

# --- odd file names: newline, Chinese, spaces --------------------------------
mk_tree
NL_DIR="$TREE/libs/nl/lib/macos"
mkdir -p "$NL_DIR"
[[ $CAN_NEWLINE -eq 1 ]] && : > "$NL_DIR/$(printf 'a\nb.a')"
mkdir -p "$TREE/libs/中文库/lib/linux64" "$TREE/libs/with space/lib/linux64"
: > "$TREE/libs/中文库/lib/linux64/库.a"
mkdir -p "$TREE/apps/myApps/项目/src" "$TREE/apps/myApps/项目/obj"
head -c 80000 /dev/zero > "$TREE/apps/myApps/项目/obj/对象.o"
mkdir -p "$TREE/apps/myApps/my app/src" "$TREE/apps/myApps/my app/obj"
: > "$TREE/apps/myApps/my app/obj/x.o"
if [[ $CAN_NEWLINE -eq 1 ]]; then
	mkdir -p "$TREE/apps/myApps/nl$(printf '\n')proj/src" "$TREE/apps/myApps/nl$(printf '\n')proj/obj"
	: > "$TREE/apps/myApps/nl$(printf '\n')proj/obj/x.o"
fi
of cleanup projects all --dry-run
[[ $CAN_UTF8 -eq 0 ]] || expect "Chinese project folder listed" grep -q "项目" <<<"$OUT"
expect "project with spaces listed" grep -q "my app" <<<"$OUT"
expect "project with a newline in its name does not break the run" test "$RC" -eq 0
of cleanup usage --tsv
[[ $CAN_UTF8 -eq 0 ]] || expect "usage tsv: Chinese lib listed" grep -q "libs/中文库" <<<"$OUT"
[[ $CAN_UTF8 -eq 0 ]] || expect "usage tsv: Chinese project listed" grep -q "apps/myApps/项目" <<<"$OUT"
expect "usage tsv: every line has 9 fields" test -z "$(printf '%s\n' "$OUT" | awk -F'\t' 'NF!=9' | head -1)"
if command -v python3 >/dev/null 2>&1; then
	"$BASH" "$TREE/scripts/of.sh" cleanup usage --json </dev/null >"$TEMP/usage.json" 2>"$TEMP/usage.err"
	expect "usage json is valid and stdout is clean" python3 -c "
import json,sys
d=json.load(open(sys.argv[1],encoding='utf-8'))
assert d['version']==1 and d['ok'] and d['command']=='cleanup usage'
d=d['data']
assert set(d['categories'])=={'projects','caches','libs','core'}
paths=[i['path'] for c in d['categories'].values() for i in c['items']]
if len(sys.argv)>2 and sys.argv[2]=='utf8':
    assert 'libs/中文库' in paths, paths
    assert any(p.startswith('apps/myApps/项目') for p in paths), paths
tot=d['categories']['libs']['total_kb']
assert tot>0
" "$TEMP/usage.json" "$([[ $CAN_UTF8 -eq 1 ]] && echo utf8 || echo ascii)"
fi
of cleanup libs linux64 --yes
[[ $CAN_UTF8 -eq 0 ]] || expect "Chinese lib/<platform> removed" test ! -e "$TREE/libs/中文库/lib/linux64"
expect "lib/<platform> with a space removed" test ! -e "$TREE/libs/with space/lib/linux64"
of cleanup libs macos --yes
expect "lib folder holding a newline-named file removed" test ! -e "$NL_DIR"

# --- usage args -------------------------------------------------------------
of cleanup usage bogus
expect "usage rejects an unknown category" test "$RC" -ne 0

# --- lock ---------------------------------------------------------------------
mk_tree
sleep 60 & LIVE=$!; BG_PIDS+=("$LIVE")
mkdir -p "$TREE/libs/.of-clean.lock"; printf '%s\n' "$LIVE" > "$TREE/libs/.of-clean.lock/pid"
of cleanup caches all --yes
expect "second cleanup refused while another holds the lock" test "$RC" -ne 0
expect "lock holder's lock left in place" test -d "$TREE/libs/.of-clean.lock"
expect "refused cleanup deleted nothing" test -f "$TREE/libs/download/a.zip"
kill "$LIVE" 2>/dev/null; wait "$LIVE" 2>/dev/null
of cleanup caches all --yes
expect "stale lock (owner gone) is reclaimed" test "$RC" -eq 0
expect "lock released afterwards" test ! -e "$TREE/libs/.of-clean.lock"
expect "reclaimed run cleaned" test ! -f "$TREE/libs/download/a.zip"

# --- running build -------------------------------------------------------------
mk_tree
BUILD=""
if [[ $CAN_LINK -eq 1 ]]; then
	ln -s "$(command -v sleep)" "$TREE/make"
	"$TREE/make" 60 & BUILD=$!; BG_PIDS+=("$BUILD")
	sleep 1
fi
if [[ -n "$BUILD" ]] && ps -axo pid=,command= 2>/dev/null | grep -q "$TREE/make"; then
	of cleanup caches all --yes
	expect "cleanup refused while a build is running" test "$RC" -ne 0
	expect "build guard deleted nothing" test -f "$TREE/libs/download/a.zip"
	of cleanup caches all --yes --force
	expect "--force cleans anyway" test "$RC" -eq 0
	of cleanup libs all --dry-run
	expect "dry run ignores the build guard" test "$RC" -eq 0
else
	echo "skip: ps does not show the build stand-in on this system"
fi
[[ -n "$BUILD" ]] && kill "$BUILD" 2>/dev/null

# --- failures are reported, not counted as freed ------------------------------
# (skipped as root, where permissions do not stop rm, and on Windows)
if [[ "$(id -u)" != "0" && $IS_WIN -eq 0 ]]; then
	mk_tree
	chmod 555 "$TREE/libs/zlib/lib"
	of cleanup libs macos --yes
	chmod 755 "$TREE/libs/zlib/lib"
	expect "a failed delete returns non-zero" test "$RC" -ne 0
	expect "a failed delete is shown" grep -q "could not remove" <<<"$OUT"
	expect "the failed folder is still there" test -d "$TREE/libs/zlib/lib/macos"
fi

# --- real project clean (Linux without a trash tool: permanent delete path) --------
if [[ "$(uname -s)" == "Linux" ]] && ! command -v gio >/dev/null 2>&1 \
	&& ! command -v trash-put >/dev/null 2>&1 && ! command -v trash >/dev/null 2>&1; then
	mk_tree
	# a Makefile whose clean target wipes bin/ must never be run by the cleanup
	printf 'clean:\n\trm -rf bin/data\n' > "$TREE/apps/myApps/p1/Makefile"
	of cleanup projects apps/myApps/p1 --yes
	expect "project clean succeeds" test "$RC" -eq 0
	expect "project obj removed" test ! -e "$TREE/apps/myApps/p1/obj"
	expect "bin/data kept" test -f "$TREE/apps/myApps/p1/bin/data/keep"
	expect "project clean reports freed space" grep -q "freed" <<<"$OUT"
	if [[ "$(id -u)" != "0" ]]; then
		mk_tree
		chmod 555 "$TREE/apps/myApps/p1/obj"
		of cleanup projects apps/myApps/p1 --yes
		chmod 755 "$TREE/apps/myApps/p1/obj"
		expect "project clean with an undeletable path fails" test "$RC" -ne 0
		expect "project clean marks the failure" grep -q "could not be removed" <<<"$OUT"
	fi
fi

echo "cleanup safety: $((CHECKS - FAILS))/${CHECKS} checks passed"
[[ $FAILS -eq 0 ]]
