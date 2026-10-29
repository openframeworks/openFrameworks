#!/usr/bin/env bash
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/../../.." && pwd)
TEMP=$(mktemp -d)
trap 'rm -rf "$TEMP"' EXIT
mkdir -p "$TEMP/data" "$TEMP/bin"
printf contents > "$TEMP/data/object.a"
tar -cjf "$TEMP/good.tar.bz2" -C "$TEMP/data" object.a
(cd "$TEMP/data" && zip -q "$TEMP/good.zip" object.a)
printf '<html>download failed</html>' > "$TEMP/bad.tar.bz2"
head -c 20 "$TEMP/good.tar.bz2" > "$TEMP/truncated.tar.bz2"
: > "$TEMP/empty.tar.bz2"
source "$ROOT/scripts/dev/sha_verify.sh"
verifyPackageArchives "$TEMP/good.tar.bz2 $TEMP/good.zip"
for archive in bad.tar.bz2 truncated.tar.bz2 empty.tar.bz2 absent.tar.bz2; do
    if verifyPackageArchives "$TEMP/$archive" > "$TEMP/validation.log" 2>&1; then
        echo "Accepted invalid archive: $archive" >&2; exit 1
    fi
done
# A package whose SHA-256 matched is skipped (not decompressed again); one
# without a matching digest is still opened.
SHA_VERIFIED_PKGS=" $TEMP/bad.tar.bz2 "
verifyPackageArchives "$TEMP/bad.tar.bz2"
SHA_VERIFIED_PKGS=" $TEMP/bad.tar.bz2.old "
if verifyPackageArchives "$TEMP/bad.tar.bz2" > "$TEMP/validation.log" 2>&1; then
    echo "Skipped an archive whose digest didn't match by name" >&2; exit 1
fi
SHA_VERIFIED_PKGS=" "
if verifyPackageArchives "$TEMP/bad.tar.bz2" > "$TEMP/validation.log" 2>&1; then
    echo "Accepted invalid archive without a verified digest" >&2; exit 1
fi
unset SHA_VERIFIED_PKGS
# The real downloader must preserve failures instead of returning the status
# of an optional connection-cleanup request.
cat > "$TEMP/bin/curl" <<'CURL'
#!/usr/bin/env bash
case "$1" in -V|--version) echo 'curl 8.0.1'; exit 0 ;; esac
case " $* " in
    *' -Z '*)
        for option in --fail --fail-early --show-error; do
            case " $* " in *" $option "*) : ;; *) exit 99 ;; esac
        done
        exit 22
        ;;
    *' --write-out '*) printf 'https://example.invalid/fixture.tar.bz2'; exit 0 ;;
    *) exit 22 ;;
esac
CURL
cat > "$TEMP/bin/wget" <<'WGET'
#!/usr/bin/env bash
exit 4
WGET
chmod +x "$TEMP/bin/curl" "$TEMP/bin/wget"
export PATH="$TEMP/bin:$PATH"
source "$ROOT/scripts/dev/downloader.sh"
cd "$TEMP"
for mode in normal silent; do
    result=0
    if [ "$mode" = silent ]; then
        downloader --curl --silent --no-compression https://example.invalid/missing.tar.bz2 > "$TEMP/curl.log" 2>&1 || result=$?
    else
        downloader --curl --no-compression https://example.invalid/missing.tar.bz2 > "$TEMP/curl.log" 2>&1 || result=$?
    fi
    [ "$result" -eq 22 ] || { cat "$TEMP/curl.log"; echo "Masked $mode HTTP failure: $result" >&2; exit 1; }
done
result=0
downloader --wget --no-compression https://example.invalid/missing.tar.bz2 > "$TEMP/wget.log" 2>&1 || result=$?
[ "$result" -eq 4 ] || { cat "$TEMP/wget.log"; echo "Masked wget failure: $result" >&2; exit 1; }
# Exercise the actual installer with unavailable remote digests and an invalid
# third bundle. It must reject the download before deleting installed libraries.
mkdir -p "$TEMP/of/scripts/dev" "$TEMP/of/libs/zlib/lib/osx"
cp "$ROOT/scripts/dev/download_libs.sh" "$ROOT/scripts/dev/sha_verify.sh" "$ROOT/scripts/dev/apothecary_release_tag.sh" "$TEMP/of/scripts/dev/"
printf installed > "$TEMP/of/libs/zlib/lib/osx/sentinel"
cat > "$TEMP/of/scripts/dev/downloader.sh" <<'DOWNLOAD'
downloader() {
    local url
    for url in "$@"; do
        case "$url" in
            *osx_3.tar.bz2) printf '<html>failed</html>' > "${url##*/}" ;;
            https://*) cp "$GOOD_ARCHIVE" "${url##*/}" ;;
        esac
    done
}
DOWNLOAD
export GOOD_ARCHIVE="$TEMP/good.tar.bz2"
if bash "$TEMP/of/scripts/dev/download_libs.sh" -p osx -b > "$TEMP/install.log" 2>&1; then
    cat "$TEMP/install.log"
    echo 'Installer accepted invalid third bundle' >&2; exit 1
fi
grep -q 'Invalid library archive: openFrameworksLibs_latest_osx_3.tar.bz2' "$TEMP/install.log"
test -f "$TEMP/of/libs/zlib/lib/osx/sentinel"
echo 'Download validation passed: HTTP/wget failures propagate, corrupt archives rejected, installed libraries preserved'
