#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# install_ofrasp_deps.sh (MSYS2 / MinGW entry)
#
# Full pacman install for Windows MSYS2 UCRT64. Normally invoked via:
#     ./scripts/install_ofrasp_deps.sh
# which detects MSYS2 and execs this script.
#
# Direct usage (MSYS2 UCRT64 shell only):
#     ./scripts/msys2/install_ofrasp_deps.sh
#
# See also: scripts/ofrasp/deps_common.sh (shared ofxFft + ofxLibTess2/libtess2)
# -----------------------------------------------------------------------------

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OF_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
export OF_ROOT

# shellcheck source=scripts/ofrasp/deps_common.sh
source "${SCRIPT_DIR}/../ofrasp/deps_common.sh"

# --- sanity checks -----------------------------------------------------------

if [[ -z "${MINGW_PACKAGE_PREFIX:-}" ]]; then
	echo "ERROR: \$MINGW_PACKAGE_PREFIX is not set." >&2
	echo "       Open the 'MSYS2 UCRT64' shell and re-run (not plain MSYS)." >&2
	exit 1
fi

if [[ -z "${MINGW_PREFIX:-}" ]]; then
	echo "ERROR: \$MINGW_PREFIX is not set." >&2
	exit 1
fi

echo "=== Environment ==="
echo "  MSYSTEM              = ${MSYSTEM:-?}"
echo "  MINGW_PACKAGE_PREFIX = ${MINGW_PACKAGE_PREFIX}"
echo "  MINGW_PREFIX         = ${MINGW_PREFIX}"
echo

# --- packages ----------------------------------------------------------------

msys_pkgs=(
	make
	unzip
	git
)

mingw_pkgs=(
	gcc
	pkgconf
	assimp cairo curl freeimage
	glew glfw glm fmt zlib brotli libpng
	harfbuzz libsndfile libusb libxml2 mpg123
	nlohmann-json openal opencv
	pugixml rtaudio uriparser utf8cpp
	ntldd-git
)

full_pkgs=("${msys_pkgs[@]}")
for p in "${mingw_pkgs[@]}"; do
	full_pkgs+=("${MINGW_PACKAGE_PREFIX}-${p}")
done

echo "=== Syncing pacman database ==="
pacman -Sy --noconfirm

echo
echo "=== Installing ${#full_pkgs[@]} packages (idempotent) ==="
printf '  %s\n' "${full_pkgs[@]}"
echo
pacman -S --noconfirm --needed "${full_pkgs[@]}"

ofrasp_deps_run_fixups

echo
echo "=== Done. Next: ./scripts/install_spine.sh (from openFrameworks repo root). ==="
