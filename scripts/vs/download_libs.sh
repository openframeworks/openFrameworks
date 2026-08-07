#!/usr/bin/env bash
# Platform-clean removes only lib/vs; shared include/bin need --full-clean.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

detectHostVsArch(){
	local m pa
	pa="${PROCESSOR_ARCHITECTURE:-}"
	[[ -n "${PROCESSOR_ARCHITEW6432:-}" ]] && pa="${PROCESSOR_ARCHITEW6432}"
	case "${MSYSTEM:-}" in
		CLANGARM64|clangarm64) echo "arm64"; return 0 ;;
	esac
	case "$(echo "$pa" | tr '[:lower:]' '[:upper:]')" in
		ARM64) echo "arm64"; return 0 ;;
		AMD64|X86) echo "64"; return 0 ;;
	esac
	m=$(uname -m 2>/dev/null || echo "")
	case "$m" in
		aarch64|arm64|ARM64) echo "arm64"; return 0 ;;
		*) echo "64"; return 0 ;;
	esac
}

if [ ! -z ${BITS+x} ]; then
	../dev/download_libs.sh -p vs -b -a "$BITS" "$@"
else
	# Pass through -a if user already provided it
	has_a=0
	for arg in "$@"; do
		case "$arg" in -a|--arch) has_a=1; break ;; esac
	done
	if [[ $has_a -eq 1 ]]; then
		../dev/download_libs.sh -p vs -b "$@"
	else
		HOST_VS=$(detectHostVsArch)
		echo " VS download · host arch → ${HOST_VS}"
		../dev/download_libs.sh -p vs -b -a "$HOST_VS" "$@"
	fi
fi
