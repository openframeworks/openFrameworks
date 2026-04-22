#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# install_ofrasp_deps.sh
#
# Platform-aware ofRasp dependency helper (openFrameworks repo root).
#
#   Windows (MSYS2 UCRT64): full pacman install + shared fixups (ofxFft, ofxLibTess2/libtess2)
#   macOS: prints OF setup pointers + runs shared fixups only
#   Linux (incl. Raspberry Pi): prints distro install pointers + runs fixups only
#
# System packages on macOS / Linux are not fully automated here — use upstream
# OF scripts for your OS (same as stock openFrameworks), then run this script.
#
# Usage (always from openFrameworks repo root):
#     ./scripts/install_ofrasp_deps.sh
#     ./scripts/install_ofrasp_deps.sh --fixups-only
#
# --fixups-only : only clone ofxFft / vendor libtess2 under addons/ofxLibTess2 (no pacman)
#                 beyond needing bash + git). Safe on any OS after OF deps exist.
# -----------------------------------------------------------------------------

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OF_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
export OF_ROOT

FIXUPS_ONLY=0
if [[ "${1:-}" == "--fixups-only" ]] || [[ "${1:-}" == "-F" ]]; then
	FIXUPS_ONLY=1
fi

# shellcheck source=scripts/ofrasp/deps_common.sh
source "${SCRIPT_DIR}/ofrasp/deps_common.sh"

if [[ "${FIXUPS_ONLY}" -eq 1 ]]; then
	ofrasp_deps_run_fixups
	echo
	echo "=== Fixups done. Next: ./scripts/install_spine.sh ==="
	exit 0
fi

uname_s="$(uname -s 2>/dev/null || echo unknown)"

# MSYS2 / MinGW: full installer lives next to this file
case "${uname_s}" in
	MINGW* | MSYS* | CYGWIN*)
		if [[ -n "${MINGW_PACKAGE_PREFIX:-}" ]] && [[ -n "${MINGW_PREFIX:-}" ]]; then
			exec "${SCRIPT_DIR}/msys2/install_ofrasp_deps.sh"
		fi
		echo "NOTE: MSYS2 UCRT64 shell not detected (MINGW_* unset)." >&2
		echo "      For full Windows deps, open 'MSYS2 UCRT64' and run:" >&2
		echo "        ${SCRIPT_DIR}/msys2/install_ofrasp_deps.sh" >&2
		echo "      Running ofRasp fixups only (ofxFft, tess2)..." >&2
		ofrasp_deps_run_fixups
		echo
		echo "=== Next: open MSYS2 UCRT64 for pacman deps, then ./scripts/install_spine.sh ==="
		exit 0
		;;
esac

case "${uname_s}" in
	Darwin)
		echo "=== macOS ==="
		echo "Install Xcode Command Line Tools (xcode-select --install) and follow:"
		echo "  https://openframeworks.cc/download/"
		echo "Use OF’s macOS project templates / download_libs.sh as documented upstream."
		echo
		ofrasp_deps_run_fixups
		echo
		echo "=== Next: ./scripts/install_spine.sh ==="
		;;
	Linux)
		echo "=== Linux (includes Raspberry Pi OS) ==="
		echo "Install openFrameworks system packages for your distro (same as upstream OF), e.g.:"
		if command -v apt-get >/dev/null 2>&1; then
			echo "  sudo ${SCRIPT_DIR}/linux/debian/install_dependencies.sh"
			echo "  # or: sudo ${SCRIPT_DIR}/linux/ubuntu/install_dependencies.sh"
		elif command -v dnf >/dev/null 2>&1; then
			echo "  sudo ${SCRIPT_DIR}/linux/fedora/install_dependencies.sh"
		elif command -v pacman >/dev/null 2>&1 && [[ -f /etc/arch-release ]]; then
			echo "  ${SCRIPT_DIR}/linux/archlinux/install_dependencies.sh  (see script for sudo)"
		else
			echo "  See scripts/linux/<your-distro>/ in this repo."
		fi
		echo
		ofrasp_deps_run_fixups
		echo
		echo "=== Next: ./scripts/install_spine.sh ==="
		;;
	*)
		echo "=== Unknown OS: ${uname_s} ===" >&2
		echo "Running fixups only (ofxFft, tess2). Install OF system deps manually." >&2
		ofrasp_deps_run_fixups
		echo
		echo "=== Next: ./scripts/install_spine.sh ==="
		;;
esac
