#!/usr/bin/env bash
# install_dependencies.sh — Windows setup helper for openFrameworks (MSYS2/Git-Bash)
#
# Windows has no single native dependency list the way Linux distros do — the
# real build requirement is Visual Studio (MSVC + Windows SDK), which this
# can't silently install. This only offers to install the small optional CLI
# tool this tooling can use (gum, for the nicer of.sh interactive menu) via a
# package manager already on the machine (winget, then scoop). It never
# installs a package manager itself; if neither is found, the install is
# just skipped.
#
# Usage: install_dependencies.sh [-y|--noconfirm]

FORCE_YES=0
for arg in "$@"; do
	case "$arg" in
		-y|--noconfirm) FORCE_YES=1 ;;
	esac
done

echo "openFrameworks · Windows dependency check"
echo

installViaWinget(){
	command -v winget.exe >/dev/null 2>&1 || return 1
	echo "Trying: winget install gum"
	winget.exe install --id Charmbracelet.gum -e --source winget \
		--accept-package-agreements --accept-source-agreements >/dev/null 2>&1
}

installViaScoop(){
	local scoopBin=""
	command -v scoop >/dev/null 2>&1 && scoopBin="scoop"
	[[ -z "$scoopBin" ]] && command -v scoop.exe >/dev/null 2>&1 && scoopBin="scoop.exe"
	[[ -n "$scoopBin" ]] || return 1
	echo "Trying: scoop install gum (extras bucket)"
	"$scoopBin" bucket add extras >/dev/null 2>&1
	"$scoopBin" install gum >/dev/null 2>&1
}

if command -v gum >/dev/null 2>&1 || command -v gum.exe >/dev/null 2>&1; then
	echo "OK  gum already available"
	exit 0
fi

if [[ "$FORCE_YES" -ne 1 ]]; then
	read -r -p "Install gum via winget/scoop if available? [Y/n]: " CONFIRM
	[[ -z "$CONFIRM" || "$CONFIRM" =~ ^[Yy]$ ]] || { echo "Skipped."; exit 0; }
fi

if installViaWinget; then
	echo "OK  gum installed via winget"
	exit 0
fi

if installViaScoop; then
	echo "OK  gum installed via scoop"
	exit 0
fi

echo "gum not installed — no winget/scoop package succeeded (or neither is available)."
echo "Not installing a package manager automatically."
echo "Install winget (usually preinstalled as 'App Installer') or scoop (https://scoop.sh) yourself, then re-run setup."
exit 0
