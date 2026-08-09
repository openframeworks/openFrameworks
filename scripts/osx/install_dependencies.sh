#!/usr/bin/env bash
# install_dependencies.sh — macOS setup helper for openFrameworks
#
# macOS has no long native dependency list the way Linux distros do — the
# real build requirement is Xcode's Command Line Tools, and openFrameworks
# ships its own prebuilt libs via apothecary. This checks for the CLT, and —
# only if Homebrew is already installed — offers to install the small set of
# optional CLI tools this tooling can use (cmake, gum). It never installs
# Homebrew itself; if it's missing, those installs are just skipped.
#
# Usage: install_dependencies.sh [-y|--noconfirm]

FORCE_YES=0
for arg in "$@"; do
	case "$arg" in
		-y|--noconfirm) FORCE_YES=1 ;;
	esac
done

BREW_PACKAGES=(cmake gum)

echo "openFrameworks · macOS dependency check"
echo

# --- Xcode Command Line Tools ------------------------------------------
if xcode-select -p >/dev/null 2>&1; then
	echo "OK  Xcode Command Line Tools: $(xcode-select -p)"
else
	echo "!!  Xcode Command Line Tools not found — required to build openFrameworks."
	echo "    Launching the installer (a system dialog will appear)…"
	xcode-select --install
	echo "    Re-run setup once that install finishes."
fi
echo

# --- Homebrew (optional: cmake, gum) ------------------------------------
if ! command -v brew >/dev/null 2>&1; then
	echo "Homebrew not found on PATH — skipping optional package installs (${BREW_PACKAGES[*]})."
	echo "Not installing Homebrew automatically. Get it from https://brew.sh if you want these, then re-run setup."
	exit 0
fi
echo "OK  Homebrew: $(brew --version | head -1)"

MISSING=()
for pkg in "${BREW_PACKAGES[@]}"; do
	if command -v "$pkg" >/dev/null 2>&1 || brew list --formula "$pkg" >/dev/null 2>&1; then
		echo "OK  ${pkg} already available"
	else
		MISSING+=("$pkg")
	fi
done

if [[ ${#MISSING[@]} -eq 0 ]]; then
	echo "All optional packages already installed."
	exit 0
fi

echo
echo "Installing via Homebrew: ${MISSING[*]}"
if [[ "$FORCE_YES" -ne 1 ]]; then
	read -r -p "Continue? [Y/n]: " CONFIRM
	[[ -z "$CONFIRM" || "$CONFIRM" =~ ^[Yy]$ ]] || { echo "Skipped."; exit 0; }
fi

brew install "${MISSING[@]}"
