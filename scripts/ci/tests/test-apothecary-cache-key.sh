#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../../.." && pwd)"
KEYSH="$ROOT/scripts/ci/apothecary_cache_key.sh"
FIX=$(mktemp)
trap 'rm -f "$FIX"' EXIT

cat > "$FIX" <<'JSON'
{
  "assets": [
    {"name": "openFrameworksLibs_latest_linux_64_gcc10.tar.bz2", "digest": "sha256:aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"},
    {"name": "openFrameworksLibs_latest_linux_64_gcc10.tar.bz2.manifest.json", "digest": "sha256:bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"},
    {"name": "openFrameworksLibs_latest_macos_1.tar.bz2", "digest": "sha256:cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"},
    {"name": "openFrameworksLibs_latest_macos_2.tar.bz2", "digest": "sha256:dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"},
    {"name": "openFrameworksLibs_latest_osx_1.tar.bz2", "digest": "sha256:1111111111111111111111111111111111111111111111111111111111111111"},
    {"name": "openFrameworksLibs_latest_vs_64_1.zip", "digest": "sha256:eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"},
    {"name": "openFrameworksLibs_latest_vs_arm64_1.zip", "digest": "sha256:ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"},
    {"name": "openFrameworksLibs_latest_vs_arm64ec_1.zip", "digest": "sha256:2222222222222222222222222222222222222222222222222222222222222222"},
    {"name": "openFrameworksLibs_latest_android_arm64.tar.bz2", "digest": "sha256:3333333333333333333333333333333333333333333333333333333333333333"},
    {"name": "openFrameworksLibs_latest_android_armv7.tar.bz2", "digest": "sha256:4444444444444444444444444444444444444444444444444444444444444444"},
    {"name": "openFrameworksLibs_latest_emscripten_32.tar.bz2", "digest": "sha256:5555555555555555555555555555555555555555555555555555555555555555"},
    {"name": "openFrameworksLibs_latest_linuxaarch64.tar.bz2", "digest": null}
  ]
}
JSON

export APOTHECARY_RELEASE_JSON="$FIX"

linux=$("$KEYSH" --platform linux --arch 64 --gcc 10 --tag latest)
linux14=$("$KEYSH" --platform linux --arch 64 --gcc 14 --tag latest)
macos=$("$KEYSH" --platform macos --tag latest)
osx=$("$KEYSH" --platform osx --tag latest)
vs=$("$KEYSH" --platform vs --arch 64 --tag latest)
vsx64=$("$KEYSH" --platform vs --arch x64 --tag latest)
vsarm=$("$KEYSH" --platform vs --arch ARM64 --tag latest)
android=$("$KEYSH" --platform android --tag latest)
emsc=$("$KEYSH" --platform emscripten --tag latest)
emsc32=$("$KEYSH" --platform emscripten --arch 32 --tag latest)
empty_arch_osx_missing=$("$KEYSH" --platform catos --tag latest || true)

echo "linux=$linux"
echo "linux14=$linux14"
echo "macos=$macos"
echo "osx=$osx"
echo "vs=$vs"
echo "vsarm=$vsarm"
echo "android=$android"
echo "emsc=$emsc"

[[ "$linux" == apothecary-latest-linux-64-10-* ]] || { echo "bad linux key"; exit 1; }
[[ "$linux" == "$linux14" ]] || { echo "gcc 14 should remap to gcc 10 key"; exit 1; }
[[ "$macos" == apothecary-latest-macos-all-x-* ]] || { echo "bad macos key"; exit 1; }
[[ "$osx" == apothecary-latest-osx-all-x-* ]] || { echo "bad osx key"; exit 1; }
[[ "$vs" == apothecary-latest-vs-64-x-* ]] || { echo "bad vs key"; exit 1; }
[[ "$vs" == "$vsx64" ]] || { echo "x64 alias should match 64"; exit 1; }
[[ "$vsarm" == apothecary-latest-vs-arm64-x-* ]] || { echo "bad vs arm64 key"; exit 1; }
[[ "$vs" != "$vsarm" ]] || { echo "vs 64 and arm64 keys collided"; exit 1; }
[[ "$linux" != "$macos" ]] || { echo "linux/macos keys collided"; exit 1; }
[[ "$android" == apothecary-latest-android-all-x-* ]] || { echo "bad android key"; exit 1; }
[[ "$emsc" == apothecary-latest-emscripten-32-x-* ]] || { echo "bad emscripten key"; exit 1; }
[[ "$emsc" == "$emsc32" ]] || { echo "emscripten default arch should be 32"; exit 1; }
[[ -z "$empty_arch_osx_missing" ]] || { echo "catos should be empty (no digested assets in fixture)"; exit 1; }

listed=$("$KEYSH" --platform linux --arch 64 --gcc 10 --tag latest --list)
[[ "$listed" == "openFrameworksLibs_latest_linux_64_gcc10.tar.bz2" ]] || { echo "bad --list"; exit 1; }

linux2=$("$KEYSH" --platform linux --arch x64 --gcc 10 --tag latest)
[[ "$linux" == "$linux2" ]] || { echo "x64 alias should match 64"; exit 1; }

echo "ok"
