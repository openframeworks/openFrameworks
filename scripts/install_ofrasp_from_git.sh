#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# install_ofrasp_from_git.sh
#
# One-shot setup after cloning the ofRasp openFrameworks repo:
#   1) git submodule update --init --recursive  (Project Generator, apothecary, …)
#   2) scripts/dev/download_libs.sh             (prebuilt libs/ for this platform)
#   3) install_ofrasp_deps.sh                   — MSYS2: pacman; others: OS package hints
#   4) install_ofrasp_spine.sh                  — ofRasp addon forks (ofxFft, ofxLibTess2, …)
#
# Skip core OF steps (only ofRasp layer):  --ofrasp-layer-only
# Fine-grained skips: --no-submodules  --no-download-libs
#
# Usage (openFrameworks repo root):
#   ./scripts/install_ofrasp_from_git.sh
#   ./scripts/install_ofrasp_from_git.sh --ofrasp-layer-only
#   ./scripts/install_ofrasp_from_git.sh --fixups-only
#   ./scripts/install_ofrasp_from_git.sh --no-spine
#   ./scripts/install_ofrasp_from_git.sh --spine-only [--full]
#
# Extra args for download_libs.sh (platform override, etc.):
#   OFRASP_DOWNLOAD_LIBS_ARGS='-p osx' ./scripts/install_ofrasp_from_git.sh
#
# Alias: ./scripts/bootstrap_ofrasp.sh
# -----------------------------------------------------------------------------

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OF_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
cd "${OF_ROOT}"

if [[ "${1:-}" == "-h" || "${1:-}" == "--help" ]]; then
	cat <<'EOF'
Usage: ./scripts/install_ofrasp_from_git.sh [options]

  (default)            Submodules + download_libs + ofRasp deps + spine
  --ofrasp-layer-only  Only steps 3–4 (deps + spine); skip submodules + download_libs
  --no-submodules      Skip git submodule update
  --no-download-libs   Skip scripts/dev/download_libs.sh
  --fixups-only        Runs install_ofrasp_spine only via install_ofrasp_deps (skips duplicate spine step)
  --no-spine           Skip install_ofrasp_spine.sh
  --spine-only         Only install_ofrasp_spine.sh (extra args forwarded, e.g. --full)

download_libs extras: set OFRASP_DOWNLOAD_LIBS_ARGS before running, e.g.
  OFRASP_DOWNLOAD_LIBS_ARGS='-p linux64' ./scripts/install_ofrasp_from_git.sh

Requires: bash, git. download_libs expects curl/wget tools per upstream OF.
See INSTALL_OFRASP.md and INSTALL_FROM_GITHUB.md.

Same as: ./scripts/bootstrap_ofrasp.sh
EOF
	exit 0
fi

if [[ "${1:-}" == "--spine-only" ]]; then
	shift
	exec bash "${SCRIPT_DIR}/install_ofrasp_spine.sh" "$@"
fi

DO_SUBMODULES=1
DO_DOWNLOAD_LIBS=1
DEPS_ARGS=()
DO_SPINE=1

while [[ $# -gt 0 ]]; do
	case "$1" in
		--ofrasp-layer-only)
			DO_SUBMODULES=0
			DO_DOWNLOAD_LIBS=0
			shift
			;;
		--no-submodules)
			DO_SUBMODULES=0
			shift
			;;
		--no-download-libs)
			DO_DOWNLOAD_LIBS=0
			shift
			;;
		--fixups-only | -F)
			DEPS_ARGS+=(--fixups-only)
			DO_SPINE=0
			shift
			;;
		--no-spine)
			DO_SPINE=0
			shift
			;;
		*)
			echo "Unknown option: $1 (try --help)" >&2
			exit 1
			;;
	esac
done

if [[ "${DO_SUBMODULES}" -eq 1 ]]; then
	echo "=== openFrameworks: git submodules (init + update) ==="
	git -C "${OF_ROOT}" submodule update --init --recursive
	echo
fi

if [[ "${DO_DOWNLOAD_LIBS}" -eq 1 ]]; then
	echo "=== openFrameworks: download_libs (prebuilt libs/) ==="
	# shellcheck disable=SC2086
	bash "${OF_ROOT}/scripts/dev/download_libs.sh" ${OFRASP_DOWNLOAD_LIBS_ARGS:-}
	echo
fi

echo "=== ofRasp: install_ofrasp_deps.sh ==="
bash "${SCRIPT_DIR}/install_ofrasp_deps.sh" "${DEPS_ARGS[@]}"

if [[ "${DO_SPINE}" -eq 1 ]]; then
	echo
	echo "=== ofRasp: spine addons ==="
	bash "${SCRIPT_DIR}/install_ofrasp_spine.sh"
fi

echo
echo "=== install_ofrasp_from_git finished ==="
echo "Next: platform packages if any are still missing (see INSTALL_OFRASP.md), then build"
echo "      with Makefile / VS / Xcode, or per-app CMake — docs/CMAKE_OFRASP.md"
