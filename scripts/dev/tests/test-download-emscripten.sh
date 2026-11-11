#!/usr/bin/env bash
# Exercise package names and installation through the real downloader script.
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/../../.." && pwd)
TEMP=$(mktemp -d)
trap 'rm -rf "$TEMP"' EXIT

for arch in 32 64; do
    platform=WASM
    [ "$arch" = 32 ] || platform=WASM64
    mkdir -p "$TEMP/data$arch/zlib/lib/emscripten/$platform"
    printf '%s\n' "$arch" > "$TEMP/data$arch/zlib/lib/emscripten/$platform/zlib.a"
    tar -cjf "$TEMP/libs$arch.tar.bz2" -C "$TEMP/data$arch" zlib
done

for tag in latest v12.2.0; do
    for arch in default 32 64; do
        of="$TEMP/$tag-$arch"
        mkdir -p "$of/scripts/dev" "$of/libs"
        cp "$ROOT/scripts/dev/download_libs.sh" "$ROOT/scripts/dev/sha_verify.sh" \
            "$ROOT/scripts/dev/apothecary_release_tag.sh" "$of/scripts/dev/"
        cat > "$of/scripts/dev/downloader.sh" <<'DOWNLOAD'
downloader() {
    local url
    for url in "$@"; do
        case "$url" in
            https://*)
                echo "$url" >> "$TEST_URLS"
                [ "$url" = "$TEST_EXPECTED_URL" ] || return 22
                cp "$TEST_ARCHIVE" "${url##*/}"
                ;;
        esac
    done
}
DOWNLOAD
        bits=$arch
        [ "$arch" != default ] || bits=32
        platform=WASM
        [ "$bits" = 32 ] || platform=WASM64
        args=(-p emscripten -b --skip-sha -t "$tag")
        [ "$arch" = default ] || args+=(-a "$arch")
        export TEST_ARCHIVE="$TEMP/libs$bits.tar.bz2" TEST_URLS="$of/urls.log"
        export TEST_EXPECTED_URL="https://github.com/openframeworks/apothecary/releases/download/$tag/openFrameworksLibs_${tag}_emscripten_${bits}.tar.bz2"
        if ! bash "$of/scripts/dev/download_libs.sh" "${args[@]}" > "$of/run.log" 2>&1; then
            cat "$of/run.log"
            echo "Emscripten download failed: tag=$tag arch=$arch" >&2
            exit 1
        fi
        [ "$(cat "$TEST_URLS")" = "$TEST_EXPECTED_URL" ]
        [ "$(cat "$of/libs/zlib/lib/emscripten/$platform/zlib.a")" = "$bits" ]
        echo "ok Emscripten $tag arch=$arch: correct package requested and installed"
    done
done
