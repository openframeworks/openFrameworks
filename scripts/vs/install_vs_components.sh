#!/usr/bin/env bash
# install_vs_components.sh — apply openFrameworks' minimal Visual Studio
# component set (2022.vsconfig / 2026.vsconfig next to this script) through
# the Visual Studio Installer (MSYS2/Git-Bash).
#
# For each installed VS 2022 / VS 2026 the .vsconfig components are checked
# with vswhere; the installer only runs (elevated, --passive) when something
# is missing, the MSVC toolset is older than the one the prebuilt oF libs are
# built with, or --latest / --update is passed. With no Visual Studio installed
# it offers a winget install of Visual Studio Community preloaded with the
# same .vsconfig.
#
# The .vsconfig files are the minimum oF needs. When the installer runs it
# goes to the latest available by default: Visual Studio is updated first (the
# "Latest" MSVC components then resolve to the newest toolset) and the newest
# Windows 11 SDK in the installer catalog is added next to the baseline SDK.
# --minimal applies just the .vsconfig.
#
# Why the toolset floor: the prebuilt libs (FreeImage etc.) come from CI with
# the newest MSVC of each VS line. Linking them with an older toolset fails
# with unresolved __std_* STL helpers (LNK2019 __std_search_1 …), so an
# outdated VS 2022 needs an update, not just the right components.
#
# VS 2019 is the exception: it ends at 16.11 / MSVC 14.29, so no update gives it
# the STL helpers of the standard libs. 2019.vsconfig brings it to that final
# toolset; it must then use v142 libs (download_libs_2019_x64.sh) and a v142
# rebuild of openframeworksLib. It is x64 only and keeps the SDK v142 CI uses.
#
# Usage: install_vs_components.sh [--vs 2019|2022|2026|all] [--arm64] [--latest|--minimal] [--update] [--check] [-y|--noconfirm]
#   --vs       which Visual Studio to configure (default: every installed 2022/2026,
#              or 2019 when it is the only one installed; when none is installed:
#              the latest, 2026 — 2022 with OF_VS_2026=0). all includes 2019.
#   --arm64    also add the ARM64 / ARM64EC build tools (auto on ARM64 hosts
#              and when LIB_ARCH is arm64|arm64ec|all)
#   --latest   run the installer even when the oF minimum is met (update + newest SDK)
#   --minimal  only apply the .vsconfig — no VS update, no newer SDK
#   --update   update Visual Studio itself, even with --minimal
#   --no-update  never update Visual Studio itself (newest SDK still added)
#   --check    report only; exit 0 when the oF minimum is in place, 1 otherwise
#   -y         don't prompt
#
# Env: OF_VS_MIN_MSVC_2019 (default 14.29.30133), OF_VS_MIN_MSVC_2022 (default 14.44),
#      OF_VS_MIN_MSVC_2026 (default 14.50)

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

FORCE_YES=0
CHECK_ONLY=0
DO_UPDATE=0
NO_UPDATE=0
LATEST=1
FORCE_RUN=0
WANT_ARM64=0
VS_SELECT=""

while [[ $# -gt 0 ]]; do
	case "$1" in
		-y|--noconfirm) FORCE_YES=1 ;;
		--check) CHECK_ONLY=1 ;;
		--update) DO_UPDATE=1; FORCE_RUN=1 ;;
		--latest) LATEST=1; FORCE_RUN=1 ;;
		--minimal) LATEST=0 ;;
		--no-update) NO_UPDATE=1 ;;
		--arm64) WANT_ARM64=1 ;;
		--vs) VS_SELECT="$2"; shift ;;
		2019|2022|2026|all) VS_SELECT="$1" ;;
		-h|--help) sed -n '2,43p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'; exit 0 ;;
		*) echo "unknown argument: $1"; exit 2 ;;
	esac
	shift
done

case "$(echo "${PROCESSOR_ARCHITEW6432:-${PROCESSOR_ARCHITECTURE:-}}" | tr '[:lower:]' '[:upper:]')" in
	ARM64) WANT_ARM64=1 ;;
esac
case "${LIB_ARCH:-}" in
	arm64|arm64ec|all) WANT_ARM64=1 ;;
esac

PF86="$(printenv 'ProgramFiles(x86)' 2>/dev/null)"
PF86="${PF86:-C:\\Program Files (x86)}"
INSTALLER_DIR="$(cygpath -u "$PF86" 2>/dev/null || echo "/c/Program Files (x86)")/Microsoft Visual Studio/Installer"
VSWHERE="${INSTALLER_DIR}/vswhere.exe"
VSSETUP="${INSTALLER_DIR}/setup.exe"

echo "openFrameworks · Visual Studio components (.vsconfig)"
echo

vsRange(){
	case "$1" in
		2019) echo "[16.0,17.0)" ;;
		2022) echo "[17.0,18.0)" ;;
		2026) echo "[18.0,19.0)" ;;
	esac
}

minMsvcFor(){
	case "$1" in
		2019) echo "${OF_VS_MIN_MSVC_2019:-14.29.30133}" ;;
		2022) echo "${OF_VS_MIN_MSVC_2022:-14.44}" ;;
		2026) echo "${OF_VS_MIN_MSVC_2026:-14.50}" ;;
	esac
}

wingetIdFor(){
	case "$1" in
		2022) echo "Microsoft.VisualStudio.2022.Community" ;;
		2026) echo "Microsoft.VisualStudio.Community" ;;
	esac
}

# components listed in <year>.vsconfig, plus ARM64 tools when wanted
componentsFor(){
	grep -oE '"Microsoft\.[^"]+"' "${SCRIPT_DIR}/$1.vsconfig" | tr -d '"'
	[[ "$WANT_ARM64" -eq 1 ]] && extraComponentsFor "$1"
}

extraComponentsFor(){
	# oF's VS 2019 packages are x64 only
	[[ "$1" == "2019" ]] && return 0
	echo "Microsoft.VisualStudio.Component.VC.Tools.ARM64"
	echo "Microsoft.VisualStudio.Component.VC.Tools.ARM64EC"
	# oF projects use PlatformToolset v143, which VS 2026 only has side-by-side
	[[ "$1" == "2026" ]] && echo "Microsoft.VisualStudio.Component.VC.14.44.17.14.ARM64"
}

# "installationPath|installationVersion|instanceId" per installed instance of a VS line
instancesFor(){
	"$VSWHERE" -all -prerelease -products '*' -version "$(vsRange "$1")" -format text 2>/dev/null | tr -d '\r' | awk '
		/^instanceId: /          { n = substr($0, 13) }
		/^installationPath: /    { p = substr($0, 19) }
		/^installationVersion: / { print p "|" substr($0, 22) "|" n }'
}

# newest Windows 11 SDK component the instance's installer catalog offers
# (an update refreshes the catalog, so call this after updating)
latestSdkFor(){
	local pd catalog
	pd="$(printenv ProgramData 2>/dev/null)"
	catalog="$(cygpath -u "${pd:-C:\\ProgramData}")/Microsoft/VisualStudio/Packages/_Instances/$1/catalog.json"
	[[ -f "$catalog" ]] || return 1
	grep -oE '"Microsoft\.VisualStudio\.Component\.Windows11SDK\.[0-9]+"' "$catalog" \
		| tr -d '"' | sort -u -t. -k5,5n | tail -n1
}

# "installationPath|packageId" for every package of every instance of a VS line
# (one vswhere call; -requires per component is ~0.5s each)
packagesFor(){
	"$VSWHERE" -all -prerelease -products '*' -version "$(vsRange "$1")" -include packages -format json 2>/dev/null \
		| tr -d '\r' | awk '
			/^    "installationPath": / { p = $0; sub(/^[^:]*: "/, "", p); sub(/",?$/, "", p); gsub(/\\\\/, "\\", p) }
			/^        "id": /           { i = $0; sub(/^[^:]*: "/, "", i); sub(/",?$/, "", i); print p "|" i }'
}

# pure-bash lookup — forks are slow on Windows and this runs per component
hasComponent(){
	[[ $'\n'"$VS_PACKAGES"$'\n' == *$'\n'"$2|$3"$'\n'* ]]
}

# default MSVC version that ships with the instance (e.g. 14.44.35207)
msvcVersionOf(){
	local f
	f="$(cygpath -u "$1")/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt"
	[[ -f "$f" ]] && tr -d '\r\n ' < "$f"
}

verGE(){
	[[ "$(printf '%s\n%s\n' "$2" "$1" | sort -V | head -n1)" == "$2" ]]
}

# vs setup.exe needs admin; MSYS can't spawn a requireAdministrator exe
# directly, so go through PowerShell's RunAs (one UAC prompt per call).
runElevated(){
	local exe a argStr=""
	exe="$(cygpath -w "$1")"; shift
	for a in "$@"; do
		[[ "$a" == *" "* ]] && a="\"$a\""
		argStr+="${argStr:+ }$a"
	done
	MSYS2_ARG_CONV_EXCL='*' powershell.exe -NoProfile -NonInteractive -Command \
		"try { \$p = Start-Process -FilePath '${exe//\'/\'\'}' -ArgumentList '${argStr//\'/\'\'}' -Verb RunAs -Wait -PassThru; exit \$p.ExitCode } catch { Write-Host \$_.Exception.Message; exit 1 }"
}

installerOk(){
	case "$1" in
		0) return 0 ;;
		3010) echo "    (restart Windows to finish)"; return 0 ;;
		*) return 1 ;;
	esac
}

confirmRun(){
	[[ "$FORCE_YES" -eq 1 ]] && return 0
	[[ -t 0 && -t 1 ]] || return 1
	local reply
	read -r -p "$1 [Y/n]: " reply
	[[ -z "$reply" || "$reply" =~ ^[Yy]$ ]]
}

if [[ ! -x "$VSWHERE" ]]; then
	VS_INSTANCES_FOUND=0
else
	VS_INSTANCES_FOUND=1
fi

years=()
case "$VS_SELECT" in
	2019|2022|2026) years=("$VS_SELECT") ;;
	all) years=(2019 2022 2026) ;;
	"")
		years=(2022 2026)
		# only VS 2019 here → configure it, rather than offering a new VS 2026.
		# A leftover 2019 next to a newer VS is left alone (ask with --vs 2019).
		if [[ "$VS_INSTANCES_FOUND" -eq 1 && -z "$(instancesFor 2022)$(instancesFor 2026)" && -n "$(instancesFor 2019)" ]]; then
			years=(2019)
		fi
		;;
	*) echo "--vs must be 2019, 2022, 2026 or all"; exit 2 ;;
esac

allOk=1
anyInstance=0
failed=0

for year in "${years[@]}"; do
	[[ "$VS_INSTANCES_FOUND" -eq 1 ]] || break
	VS_PACKAGES="$(packagesFor "$year")"
	# fd 3, not stdin — the confirm prompt inside needs the terminal
	while IFS='|' read -r -u 3 path version instanceId; do
		[[ -n "$path" ]] || continue
		anyInstance=1
		missing=()
		while read -r id; do
			[[ -n "$id" ]] || continue
			hasComponent "$year" "$path" "$id" || missing+=("$id")
		done < <(componentsFor "$year")

		msvc="$(msvcVersionOf "$path")"
		minMsvc="$(minMsvcFor "$year")"
		outdated=0
		[[ -n "$msvc" ]] && ! verGE "$msvc" "$minMsvc" && outdated=1

		echo "VS ${year}  ${version}  ${path}"
		echo "    MSVC ${msvc:-not installed} (oF libs need >= ${minMsvc})"
		if [[ "$year" == "2019" ]]; then
			echo "    VS 2019 cannot link the standard oF libs (no update adds their STL helpers). Use v142 builds:"
			echo "      scripts/vs/download_libs_2019_x64.sh, then rebuild openframeworksLib with v142"
		fi
		if [[ ${#missing[@]} -eq 0 && "$outdated" -eq 0 ]]; then
			echo "OK  all oF components present"
			if [[ "$FORCE_RUN" -eq 0 || "$CHECK_ONLY" -eq 1 ]]; then
				echo
				continue
			fi
		else
			allOk=0
		fi
		for id in "${missing[@]}"; do echo "    missing: ${id}"; done
		[[ "$outdated" -eq 1 ]] && echo "    toolset older than the prebuilt libs — Visual Studio needs an update"
		[[ "$CHECK_ONLY" -eq 1 ]] && { echo; continue; }

		if ! confirmRun "Run the Visual Studio Installer for VS ${year} (UAC prompt)?"; then
			echo "Skipped. Re-run with -y, or import $(cygpath -w "${SCRIPT_DIR}/${year}.vsconfig") from"
			echo "Visual Studio Installer → More → Import configuration."
			echo
			continue
		fi
		if tasklist.exe //FI "IMAGENAME eq devenv.exe" 2>/dev/null | grep -qi devenv.exe; then
			echo "!   Visual Studio is running — the installer will ask to close it"
		fi

		if [[ "$NO_UPDATE" -eq 0 ]] && [[ "$outdated" -eq 1 || "$DO_UPDATE" -eq 1 || "$LATEST" -eq 1 ]]; then
			echo "Updating VS ${year} …"
			runElevated "$VSSETUP" update --installPath "$path" --passive --norestart
			code=$?
			installerOk "$code" || { echo "FAIL  update exited ${code}"; failed=1; echo; continue; }
		fi

		addArgs=()
		if [[ "$WANT_ARM64" -eq 1 ]]; then
			while read -r id; do addArgs+=(--add "$id"); done < <(extraComponentsFor "$year")
		fi
		# 2019 stays on the SDK its v142 libs are built against
		if [[ "$LATEST" -eq 1 && "$year" != "2019" ]]; then
			sdk="$(latestSdkFor "$instanceId")"
			if [[ -n "$sdk" ]] && ! grep -qF "\"${sdk}\"" "${SCRIPT_DIR}/${year}.vsconfig"; then
				echo "    + ${sdk} (newest SDK available)"
				addArgs+=(--add "$sdk")
			fi
		fi
		echo "Applying ${year}.vsconfig …"
		runElevated "$VSSETUP" modify --installPath "$path" --config "$(cygpath -w "${SCRIPT_DIR}/${year}.vsconfig")" \
			"${addArgs[@]}" --passive --norestart
		code=$?
		if installerOk "$code"; then
			echo "OK  VS ${year} configured"
		else
			echo "FAIL  installer exited ${code}"
			failed=1
		fi
		echo
	done 3< <(instancesFor "$year")
done

if [[ "$anyInstance" -eq 0 ]]; then
	allOk=0
	year="${VS_SELECT}"
	# nothing installed → the latest Visual Studio, unless told otherwise
	[[ "$year" == "2019" || "$year" == "2022" || "$year" == "2026" ]] || { [[ "${OF_VS_2026:-1}" == 0 ]] && year=2022 || year=2026; }
	echo "No Visual Studio ${VS_SELECT:-2022/2026} found."
	[[ "$CHECK_ONLY" -eq 1 ]] && exit 1
	# not on winget, and new installs should prefer a supported VS
	if [[ "$year" == "2019" ]]; then
		echo "Install VS 2019 from https://visualstudio.microsoft.com/vs/older-downloads/ and pick"
		echo "More → Import configuration → $(cygpath -w "${SCRIPT_DIR}/2019.vsconfig")"
		exit 1
	fi
	if command -v winget.exe >/dev/null 2>&1 && confirmRun "Install Visual Studio Community ${year} with the oF components via winget?"; then
		cfg="$(cygpath -w "${SCRIPT_DIR}/${year}.vsconfig")"
		override="--passive --wait --norestart --config \"${cfg}\""
		if [[ "$WANT_ARM64" -eq 1 ]]; then
			while read -r id; do override+=" --add ${id}"; done < <(extraComponentsFor "$year")
		fi
		MSYS2_ARG_CONV_EXCL='*' winget.exe install --id "$(wingetIdFor "$year")" -e --source winget \
			--accept-package-agreements --accept-source-agreements --override "$override"
		code=$?
		if installerOk "$code"; then
			echo "OK  Visual Studio ${year} installed"
			# the fresh install's catalog is only readable now — add the newest
			# SDK it offers (no update needed, winget just installed the latest)
			if [[ "$LATEST" -eq 1 ]]; then
				echo
				"${BASH_SOURCE[0]}" --vs "$year" --latest --no-update -y
			fi
			exit 0
		fi
		echo "FAIL  winget exited ${code}"
		exit 1
	fi
	echo "Install Visual Studio from https://visualstudio.microsoft.com/downloads/ and pick"
	echo "More → Import configuration → $(cygpath -w "${SCRIPT_DIR}/${year}.vsconfig")"
	exit 1
fi

[[ "$failed" -eq 1 ]] && exit 1
[[ "$CHECK_ONLY" -eq 1 && "$allOk" -eq 0 ]] && exit 1
exit 0
