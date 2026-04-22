#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# deps_common.sh — sourced by install_ofrasp_deps.sh (do not run directly).
#
# Shared post-install steps for ofRasp on every OS:
#   - addons/ofxFft (kissfft sources for ofOpenALSoundPlayer)
#   - addons/ofxLibTess2/libtess2 (memononen/libtess2; list ofxLibTess2 in addons.make)
#
# Requires: OF_ROOT absolute path set by caller; git on PATH.
# -----------------------------------------------------------------------------

if [[ -z "${OF_ROOT:-}" ]]; then
	echo "ERROR: deps_common.sh must be sourced after OF_ROOT is set." >&2
	return 1 2>/dev/null || exit 1
fi

ofrasp_deps_clone_ofxfft() {
	local ofxfft_dir="${OF_ROOT}/addons/ofxFft"
	if [[ ! -d "${ofxfft_dir}/libs/kiss" ]]; then
		echo
		echo "=== Cloning ofxFft into ${ofxfft_dir} ==="
		rm -rf "${ofxfft_dir}"
		git clone --depth 1 https://github.com/kylemcdonald/ofxFft.git "${ofxfft_dir}"
	else
		echo
		echo "=== ofxFft already present at ${ofxfft_dir} (skipping) ==="
	fi
}

ofrasp_deps_vendor_tess2() {
	local tess2_dir="${OF_ROOT}/addons/ofxLibTess2/libtess2"
	if [[ ! -d "${tess2_dir}/Source" ]]; then
		echo
		echo "=== Vendoring libtess2 into ${tess2_dir} ==="
		mkdir -p "$(dirname "${tess2_dir}")"
		rm -rf "${tess2_dir}"
		git clone --depth 1 https://github.com/memononen/libtess2.git "${tess2_dir}"
	else
		echo
		echo "=== libtess2 already vendored at ${tess2_dir} (skipping) ==="
	fi
}

ofrasp_deps_run_fixups() {
	ofrasp_deps_clone_ofxfft
	ofrasp_deps_vendor_tess2
}
