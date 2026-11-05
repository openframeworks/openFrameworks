#!/usr/bin/env bash
# Print an Actions cache key for apothecary packages: latest + SHA of matching
# openFrameworksLibs_* asset digests. Empty output means "do not cache".
#
# One key per platform/arch slice. Exact match only (no restore-keys prefix).
# Official Linux archives are GCC 10; other -g values remap like download_libs.sh.
# VS ARM64 hosts also pull x64 + arm64ec (see scripts/dev/download_libs.sh).
set -euo pipefail

TAG=latest
PLATFORM=""
ARCH=""
GCC=""
VS2026=0
REPO="openframeworks/apothecary"
LIST=0

usage() {
	echo "Usage: $0 --platform osx|macos|linux|vs|msys2|emscripten|android [--tag latest] [--arch 64] [--gcc 10] [--vs2026] [--list]" >&2
	exit 2
}

while [[ $# -gt 0 ]]; do
	case "$1" in
		--tag) TAG="$2"; shift 2 ;;
		--platform) PLATFORM="$2"; shift 2 ;;
		--arch) ARCH="$2"; shift 2 ;;
		--gcc) GCC="$2"; shift 2 ;;
		--vs2026) VS2026=1; shift ;;
		--repo) REPO="$2"; shift 2 ;;
		--list) LIST=1; shift ;;
		-h|--help) usage ;;
		*) usage ;;
	esac
done

[[ -n "$PLATFORM" ]] || usage

# CI VS jobs pass x64/ARM64; linux keeps aarch64 vs arm64 as distinct archives.
case "$PLATFORM" in
	vs|vs_2026|msys2)
		case "$ARCH" in
			x64|x86_64|amd64) ARCH=64 ;;
			ARM64) ARCH=arm64 ;;
			ARM64EC) ARCH=arm64ec ;;
		esac
		;;
	linux)
		case "$ARCH" in
			x64|x86_64) ARCH=64 ;;
		esac
		;;
esac

PY=python3
command -v python3 >/dev/null 2>&1 || PY=python

"$PY" - "$TAG" "$PLATFORM" "$ARCH" "$GCC" "$VS2026" "$REPO" "$LIST" <<'PY'
import hashlib, json, os, sys, urllib.request

tag, platform, arch, gcc, vs2026, repo, list_mode = sys.argv[1:8]
vs2026 = vs2026 == "1"
list_mode = list_mode == "1"
# Official apothecary Linux archives are GCC 10 (download_libs.sh OFFICIAL_LINUX_GCC).
if platform == "linux":
    gcc = "10"
    arch = arch or "64"
elif platform in ("vs", "vs_2026") or vs2026:
    arch = arch or "64"
elif platform == "msys2":
    arch = arch or "ucrt64"
elif platform == "emscripten":
    arch = (arch or "32").lstrip("_")
else:
    gcc = gcc or "10"

def prefixes():
    base = f"openFrameworksLibs_{tag}_"
    if platform == "linux":
        return [f"{base}linux_{arch}_gcc{gcc}"]
    if platform in ("vs", "vs_2026") or vs2026:
        plat = "vs_2026" if (vs2026 or platform == "vs_2026") else "vs"
        # download_libs.sh on ARM64 hosts also installs x64 + arm64ec.
        if arch == "arm64":
            return [f"{base}{plat}_64_", f"{base}{plat}_arm64_", f"{base}{plat}_arm64ec_"]
        return [f"{base}{plat}_{arch}_"]
    if platform == "msys2":
        return [f"{base}msys2_{arch}"]
    if platform == "emscripten":
        return [f"{base}emscripten_{arch}", f"{base}emscripten{arch}"]
    if platform == "android":
        if arch:
            return [f"{base}android_{arch}"]
        return [f"{base}android_"]
    return [f"{base}{platform}_", f"{base}{platform}"]

prefs = prefixes()
fixture = os.environ.get("APOTHECARY_RELEASE_JSON")
if fixture:
    with open(fixture, encoding="utf-8") as f:
        data = json.load(f)
else:
    url = f"https://api.github.com/repos/{repo}/releases/tags/{tag}"
    req = urllib.request.Request(url, headers={
        "Accept": "application/vnd.github+json",
        "User-Agent": "openFrameworks-apothecary-cache",
    })
    token = os.environ.get("GITHUB_TOKEN") or os.environ.get("GH_TOKEN") or ""
    if token:
        req.add_header("Authorization", f"Bearer {token}")
    try:
        with urllib.request.urlopen(req, timeout=30) as resp:
            data = json.load(resp)
    except Exception as exc:
        sys.stderr.write(f"apothecary digest lookup failed: {exc}\n")
        sys.exit(0)

rows = []
names = []
for asset in data.get("assets") or []:
    name = asset.get("name") or ""
    digest = asset.get("digest") or ""
    if not name.endswith((".tar.bz2", ".zip")):
        continue
    if not any(name.startswith(p) for p in prefs):
        continue
    if ":" in digest:
        algo, hx = digest.split(":", 1)
        if algo.lower() != "sha256" or len(hx) < 32:
            continue
        digest = hx.lower()
    else:
        continue
    rows.append(f"{name}={digest}")
    names.append(name)

if not rows:
    sys.stderr.write(f"no digested assets for {prefs}\n")
    sys.exit(0)

if list_mode:
    for name in sorted(names):
        print(name)
    sys.exit(0)

blob = "\n".join(sorted(rows)) + "\n"
hx = hashlib.sha256(blob.encode()).hexdigest()[:16]
arch_part = arch or "all"
gcc_part = gcc if platform == "linux" else "x"
safe = lambda s: "".join(c if c.isalnum() or c in "-_" else "-" for c in s)
print(f"apothecary-{safe(tag)}-{safe(platform)}-{safe(arch_part)}-{safe(gcc_part)}-{hx}")
PY
