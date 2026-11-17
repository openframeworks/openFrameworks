#!/usr/bin/env bash
# Exercises the real download_libs.sh with a stubbed downloader and fixture zips:
# - failures and signals keep every cached package (and don't trip the trap)
# - --vs-switch only clears what the new packages restore, and only after they verify
# - a failed download never removes installed libraries
# Pass SCRIPT=/path/to/download_libs.sh to run against another copy.
set -uo pipefail
ROOT=$(cd "$(dirname "$0")/../../.." && pwd)
SCRIPT="${SCRIPT:-$ROOT/scripts/dev/download_libs.sh}"
TEMP=$(mktemp -d)
trap 'rm -rf "$TEMP"' EXIT
FAILED=0
ok(){ echo "ok   $1"; }
bad(){ echo "FAIL $1"; FAILED=1; }
check(){ if eval "$2"; then ok "$1"; else bad "$1"; fi; }

PY=$(command -v python3 || command -v python || command -v py)
[ -n "$PY" ] || { echo 'needs python to build fixtures' >&2; exit 2; }
command -v unzip >/dev/null || { echo 'needs unzip' >&2; exit 2; }

# <zip> <path=content>...   written by python so `zip` isn't required
mkzip(){
    local out="$1"; shift
    "$PY" -c 'import sys,zipfile
z=zipfile.ZipFile(sys.argv[1],"w")
for a in sys.argv[2:]:
    p,c=a.split("=",1); z.writestr(p,c)
z.close()' "$out" "$@"
}
PKG1="$TEMP/pkg1.zip"; PKG2="$TEMP/pkg2.zip"
mkzip "$PKG1" FreeImage/include/FreeImage.h=new FreeImage/lib/vs/x64/FreeImage.lib=new pugixml/lib/vs/x64/pugixml.lib=new
mkzip "$PKG2" curl/include/curl.h=new curl/bin/libcurl.dll=new curl/lib/vs/x64/libcurl.lib=new \
    poco/include/Poco/Poco.h=new poco/lib/vs/x64/PocoFoundation.lib=new

# fresh tree holding "old VS" libs; MODE picks the stub downloader's behaviour
fresh(){
    OF="$TEMP/of$1"; rm -rf "$OF"
    mkdir -p "$OF/scripts/dev" "$OF/libs/download"
    cp "$SCRIPT" "$OF/scripts/dev/download_libs.sh"
    cp "$ROOT/scripts/dev/sha_verify.sh" "$ROOT/scripts/dev/apothecary_release_tag.sh" "$OF/scripts/dev/"
    cat > "$OF/scripts/dev/downloader.sh" <<'DOWNLOAD'
downloader() {
    local url
    case "$STUB_MODE" in
        fail) return 22 ;;
        term) kill -TERM $$; sleep 1 ;;
    esac
    for url in "$@"; do
        case "$url" in
            https://*)
                echo "$url" >> "$STUB_LOG"
                case "${url##*/}" in
                    *_2.zip) cp "$STUB_PKG2" "${url##*/}" ;;
                    *) if [ "$STUB_MODE" = corrupt ]; then printf '<html>' > "${url##*/}"; else cp "$STUB_PKG1" "${url##*/}"; fi ;;
                esac
                ;;
        esac
    done
}
DOWNLOAD
    local d
    for d in FreeImage/include FreeImage/lib/vs/x64 curl/include curl/bin curl/lib/vs/x64 pugixml/lib/vs/x64 \
        kiss/include kiss/lib/vs/x64 FreeImage/lib/msys2 \
        openFrameworksCompiled/lib/vs/x64 openFrameworksCompiled/lib/msys2 \
        openFrameworksCompiled/project/vs/obj openFrameworksCompiled/project/vs2019/obj openFrameworksCompiled/project/vs2026/obj; do
        mkdir -p "$OF/libs/$d"; printf old > "$OF/libs/$d/stale"
    done
    printf '' > "$OF/libs/openFrameworksCompiled/lib/vs/.gitkeep"
    # another arch's cache next to the packages this run wants
    printf cached > "$OF/libs/download/openFrameworksLibs_latest_vs_arm64_1.zip"
    printf cached > "$OF/libs/download/openFrameworksLibs_latest_vs_arm64_2.zip"
    printf cached > "$OF/libs/download/openFrameworksLibs_latest_vs_64_1.zip"
    printf cached > "$OF/libs/download/openFrameworksLibs_latest_vs_64_2.zip"
}
run(){
    local mode="$1"; shift
    LOG="$TEMP/run.log"; : > "$TEMP/urls.log"
    STUB_MODE="$mode" STUB_LOG="$TEMP/urls.log" STUB_PKG1="$PKG1" STUB_PKG2="$PKG2" \
        bash "$OF/scripts/dev/download_libs.sh" -p vs -b -a 64 --skip-sha "$@" > "$LOG" 2>&1
    RC=$?
}
L(){ echo "$OF/libs/$1"; }
cached(){ ls "$OF/libs/download" | grep -c '^openFrameworksLibs_'; }

echo '--- failed download (several cached packages)'
fresh a; run fail
check 'exits non-zero' '[ "$RC" -ne 0 ]'
check 'no shell error from the trap' '! grep -q "too many arguments" "$LOG"'
check 'cached packages kept' '[ "$(cached)" -eq 4 ]'
check 'installed libs kept' '[ -f "$(L FreeImage/lib/vs/x64/stale)" ]'

echo '--- interrupted download'
fresh b; run term
check 'exits with the signal status' '[ "$RC" -eq 143 ]'
check 'says it was interrupted' 'grep -q "Interrupted (SIGTERM)" "$LOG"'
check 'no shell error from the trap' '! grep -q "too many arguments" "$LOG"'
check 'cached packages kept' '[ "$(cached)" -eq 4 ]'
check 'installed libs kept' '[ -f "$(L FreeImage/lib/vs/x64/stale)" ]'

echo '--- --vs-switch, corrupt package'
fresh c; run corrupt --vs-switch
check 'rejected' '[ "$RC" -ne 0 ]'
check 'installed libs kept' '[ -f "$(L FreeImage/lib/vs/x64/stale)" ] && [ -f "$(L FreeImage/include/stale)" ]'
check 'core build kept' '[ -f "$(L openFrameworksCompiled/lib/vs/x64/stale)" ]'

echo '--- --vs-switch, good packages'
fresh d; run ok --vs-switch
[ "$RC" -eq 0 ] || sed 's/^/    /' "$LOG"
check 'succeeds' '[ "$RC" -eq 0 ]'
check 'new files installed' '[ -f "$(L FreeImage/lib/vs/x64/FreeImage.lib)" ] && [ -f "$(L curl/bin/libcurl.dll)" ] && [ -f "$(L pugixml/lib/vs/x64/pugixml.lib)" ]'
for f in FreeImage/lib/vs/x64 FreeImage/include curl/bin curl/include pugixml/lib/vs/x64 \
    openFrameworksCompiled/lib/vs/x64 openFrameworksCompiled/project/vs/obj \
    openFrameworksCompiled/project/vs2019/obj openFrameworksCompiled/project/vs2026/obj; do
    check "stale gone: $f" '[ ! -e "$(L $f/stale)" ]'
done
check 'Poco deployed to ofxPoco' '[ -f "$OF/addons/ofxPoco/libs/poco/lib/vs/x64/PocoFoundation.lib" ]'
check 'Poco not deployed to ofxSvg' '[ ! -e "$OF/addons/ofxSvg/libs/poco" ]'
check 'tracked .gitkeep kept' '[ -f "$(L openFrameworksCompiled/lib/vs/.gitkeep)" ]'
check 'lib missing from the packages keeps headers' '[ -f "$(L kiss/include/stale)" ]'
check 'other platform untouched' '[ -f "$(L FreeImage/lib/msys2/stale)" ] && [ -f "$(L openFrameworksCompiled/lib/msys2/stale)" ]'

echo '--- plain update (no --vs-switch)'
fresh e; run ok
check 'succeeds' '[ "$RC" -eq 0 ]'
check 'shared include kept' '[ -f "$(L FreeImage/include/stale)" ]'
check 'core build kept' '[ -f "$(L openFrameworksCompiled/lib/vs/x64/stale)" ] && [ -f "$(L openFrameworksCompiled/project/vs/obj/stale)" ]'

echo '--- package names'
fresh f; run ok -l
check 'VS 2019 packages requested' 'grep -q "/latest/openFrameworksLibs_latest_vs_2019_64_1.zip" "$TEMP/urls.log"'
fresh g; run ok -vs
check 'VS 2026 packages requested' 'grep -q "/latest/openFrameworksLibs_latest_vs_2026_64_1.zip" "$TEMP/urls.log"'

[ "$FAILED" -eq 0 ] && echo 'VS switch / trap checks passed' || { echo 'VS switch / trap checks FAILED'; exit 1; }
