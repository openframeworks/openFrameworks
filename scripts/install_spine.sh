#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# install_spine.sh
#
# Clone or update the ofRasp "spine" addons into <OF_ROOT>/addons.
# Default remote: https://github.com/ofrasp/<repo>.git
#
# Requires: git, bash
# Works from: MSYS2 UCRT64, Linux, macOS, WSL (anywhere git works).
#
# Usage (from openFrameworks repo root):
#     ./scripts/install_spine.sh
#     ./scripts/install_spine.sh --full          # full clone (no --depth)
#
# Override org or URLs:
#     OFRASP_ORG=myfork ./scripts/install_spine.sh
#     OFRASP_GIT_BASE=https://github.com/ofrasp ./scripts/install_spine.sh
#
# Add or remove repos by editing SPINE_REPOS below.
# -----------------------------------------------------------------------------

set -euo pipefail

OF_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ADDONS_DIR="${OF_ROOT}/addons"
ORG="${OFRASP_ORG:-ofrasp}"
GIT_BASE="${OFRASP_GIT_BASE:-https://github.com/${ORG}}"
DEPTH_ARG=(--depth 1)
DO_PULL=1

usage() {
	cat <<'EOF'
Usage: ./scripts/install_spine.sh [--full] [--no-pull]

  --full     Full clone (omit --depth 1)
  --no-pull  Skip git pull on existing clones

Environment:
  OFRASP_ORG       GitHub org or user (default: ofrasp)
  OFRASP_GIT_BASE  e.g. https://github.com/ofrasp (overrides org-based URL)

Edit SPINE_REPOS in this script to add spine addons.
EOF
}

while [[ $# -gt 0 ]]; do
	case "$1" in
		-h | --help)
			usage
			exit 0
			;;
		--full)
			DEPTH_ARG=()
			shift
			;;
		--no-pull)
			DO_PULL=0
			shift
			;;
		*)
			echo "Unknown option: $1 (try --help)" >&2
			exit 1
			;;
	esac
done

# GitHub repo name = folder name under addons/ (OF convention).
SPINE_REPOS=(
	ofxImGui
	ofxFft
	ofxEnTT
)

if [[ ${#DEPTH_ARG[@]} -eq 0 ]]; then
	depth_note="full clone (no --depth)"
else
	depth_note="${DEPTH_ARG[*]}"
fi

echo "=== ofRasp spine addons ==="
echo "  OF_ROOT     = ${OF_ROOT}"
echo "  ADDONS_DIR  = ${ADDONS_DIR}"
echo "  GIT_BASE    = ${GIT_BASE}"
echo "  depth       = ${depth_note}"
echo

mkdir -p "${ADDONS_DIR}"

for repo in "${SPINE_REPOS[@]}"; do
	dest="${ADDONS_DIR}/${repo}"
	url="${GIT_BASE}/${repo}.git"

	if [[ -d "${dest}/.git" ]]; then
		if [[ "${DO_PULL}" -eq 1 ]]; then
			echo ">>> ${repo}: existing clone — updating (ff-only)"
			git -C "${dest}" fetch --tags --prune origin
			head_branch="$(git -C "${dest}" rev-parse --abbrev-ref HEAD)"
			git -C "${dest}" pull --ff-only origin "${head_branch}" || {
				echo "    WARN: ff-only pull failed for ${repo}; resolve manually in ${dest}" >&2
			}
		else
			echo ">>> ${repo}: existing clone — skipped (--no-pull)"
		fi
		continue
	fi

	if [[ -e "${dest}" ]]; then
		echo "ERROR: ${dest} exists but is not a git repo; move it aside or remove it." >&2
		exit 1
	fi

	echo ">>> ${repo}: cloning ${url}"
	git clone "${DEPTH_ARG[@]}" "${url}" "${dest}"
done

echo
echo "=== Done. Spine addons are under: ${ADDONS_DIR} ==="
