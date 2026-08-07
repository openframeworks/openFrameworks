#!/usr/bin/env bash
# lib_sources.sh - multi-source library download helpers for openFrameworks
# Sources:
#   apothecary  - github.com/openframeworks/apothecary  (default, stock oF layout)
#   oflibs      - github.com/ofWorks/ofLibs             (per-lib zips; ofWorks-oriented)
#   archive     - libs.danoli3.com/versions             (historical full OF releases)
#
# Keep apothecary as the core path for day-to-day master/nightly builds.

LIB_SOURCE="${LIB_SOURCE:-apothecary}"
LIB_TAG="${LIB_TAG:-}"
OFLIBS_REPO="${OFLIBS_REPO:-ofWorks/ofLibs}"
ARCHIVE_BASE="${ARCHIVE_BASE:-https://libs.danoli3.com}"
APOTHECARY_RELEASES="${APOTHECARY_RELEASES:-https://github.com/openframeworks/apothecary/releases/download}"

# Shared SHA-256 helpers (GitHub digests + URL sidecars)
_OF_SHA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=sha_verify.sh
. "${_OF_SHA_DIR}/sha_verify.sh"

# ofLibs platform token for a host OF platform
ofLibsPlatform(){
	local p="${1:-osx}"
	case "$p" in
		osx|macos) echo "macos" ;;
		linux|linux64) echo "linux64" ;;
		linuxaarch64|linuxarm64|jetson) echo "rpi-aarch64" ;;
		vs|msys2) echo "vs" ;;
		emscripten) echo "emscripten" ;;
		*) echo "" ;;
	esac
}

# Map ofLibs package name → stock oF libs/ folder name
ofLibsToOfName(){
	case "$1" in
		utfcpp) echo "utf8" ;;
		zlib-ng) echo "zlib" ;;
		kissfft) echo "kiss" ;;
		*) echo "$1" ;;
	esac
}

# Core ofLibs packages that roughly map to stock oF core/addons
OFLIBS_CORE=(
	freetype glew glfw glm json pugixml rtAudio tess2 uriparser utfcpp zlib-ng
	fmt brotli assimp cairo opencv libusb
)

listOfLibsReleases(){
	curl -fsSL "https://api.github.com/repos/${OFLIBS_REPO}/releases?per_page=30" 2>/dev/null \
		| grep -o '"tag_name": *"[^"]*"' \
		| sed 's/.*"tag_name": *"\([^"]*\)".*/\1/'
}

listApothecaryReleases(){
	# GitHub release tags for openframeworks/apothecary (plus virtual latest/nightly)
	curl -fsSL "https://api.github.com/repos/openframeworks/apothecary/releases?per_page=40" 2>/dev/null \
		| grep -o '"tag_name": *"[^"]*"' \
		| sed 's/.*"tag_name": *"\([^"]*\)".*/\1/'
}

listOfLibsAssets(){
	local tag="${1:-v1.0}"
	local plat="$2"
	curl -fsSL "https://api.github.com/repos/${OFLIBS_REPO}/releases/tags/${tag}" 2>/dev/null \
		| grep -o '"name": *"ofLibs_[^"]*"' \
		| sed 's/.*"name": *"\([^"]*\)".*/\1/' \
		| if [[ -n "$plat" ]]; then grep "_${plat}\.zip$"; else cat; fi
}

# Library names available for a given ofLibs tag + platform (e.g. glfw, glm, assimp)
listOfLibsNames(){
	local tag="${1:-v1.0}"
	local ofPlat="${2:-osx}"
	local plat
	plat=$(ofLibsPlatform "$ofPlat")
	[[ -z "$plat" ]] && return 1
	listOfLibsAssets "$tag" "$plat" \
		| sed -E "s/^ofLibs_(.+)_${plat}\.zip$/\1/" \
		| sort -u
}

listArchiveVersions(){
	curl -fsSL "${ARCHIVE_BASE}/?dir=versions" 2>/dev/null \
		| grep -oE 'versions/v[0-9]+\.[0-9]+(\.[0-9]+)?' \
		| sed 's|versions/||' \
		| sort -uV
}

# Map OF platform → danoli3 package token inside of_vX_Y_Z_<token>_release.*
archivePlatformToken(){
	local p="${1:-osx}"
	local arch="${2:-}"
	case "$p" in
		osx|macos) echo "osx" ;;
		ios) echo "ios" ;;
		android) echo "android" ;;
		vs) echo "vs2017" ;;
		msys2)
			if [[ "$arch" == *"32"* ]]; then echo "msys2_mingw32"; else echo "msys2_mingw64"; fi
			;;
		linux)
			case "$arch" in
				armv6l) echo "linuxarmv6l" ;;
				armv7l) echo "linuxarmv7l" ;;
				aarch64|arm64) echo "linuxarmv7l" ;; # best-effort; older archives vary
				*) echo "linux64gcc6" ;;
			esac
			;;
		*) echo "$p" ;;
	esac
}

listArchivePackages(){
	local ver="$1"
	curl -fsSL "${ARCHIVE_BASE}/?dir=versions/${ver}" 2>/dev/null \
		| grep -oE "versions/${ver}/of_[^\"]+" \
		| sed "s|versions/${ver}/||" \
		| sort -u
}

# ---------- downloads ----------

downloadApothecaryLibs(){
	local platformDir="$1"
	local scriptsRoot="${OF_CORE_SCRIPT_DIR:-$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)}"
	local script="${scriptsRoot}/${platformDir}/download_libs.sh"
	if [[ ! -f "$script" ]]; then
		echo "download script missing: $script" >&2
		return 1
	fi
	[[ -x "$script" ]] || chmod +x "$script" 2>/dev/null || true
	local -a args=()
	# -b bleeding-edge → "latest" packages; -t TAG for a specific release tag
	if [[ -n "$LIB_TAG" && "$LIB_TAG" != "latest" && "$LIB_TAG" != "nightly" ]]; then
		args+=( -t "$LIB_TAG" )
	else
		args+=( -b )
	fi
	# LIB_CLEAN_MODE: platform (default) | merge (-n) | full (--full-clean)
	case "${LIB_CLEAN_MODE:-platform}" in
		merge|no-overwrite|n) args+=( -n ) ;;
		full|full-clean)      args+=( --full-clean ) ;;
		platform|*)           ;; # default platform-scoped clean in download_libs.sh
	esac
	"$script" "${args[@]}"
}

# Install ofLibs zips into OF_DIR/libs/<name>/{include,lib}
# libs args: empty | "core" | "all" | name name ...
downloadOfLibs(){
	local ofDir="$1"
	local tag="${2:-v1.0}"
	local ofPlat="${3:-osx}"
	shift 3 || true
	local -a libs=("$@")
	local plat pkg url tmp dest name ofName ok=0 skip=0 fail=0
	local dlDir="${ofDir}/libs/download/oflibs"
	local digestMap expected

	plat=$(ofLibsPlatform "$ofPlat")
	if [[ -z "$plat" ]]; then
		echo "ofLibs has no package map for platform '${ofPlat}' (supported: osx/macos, linux, vs, emscripten, linuxaarch64)" >&2
		return 1
	fi

	if [[ ${#libs[@]} -eq 0 || "${libs[0]}" == "core" ]]; then
		libs=("${OFLIBS_CORE[@]}")
	elif [[ "${libs[0]}" == "all" ]]; then
		local names
		names=$(listOfLibsNames "$tag" "$ofPlat")
		if [[ -z "$names" ]]; then
			echo "could not list ofLibs packages for tag=${tag} plat=${plat}" >&2
			return 1
		fi
		# shellcheck disable=SC2206
		libs=( $names )
	fi

	mkdir -p "$dlDir"
	tmp=$(mktemp -d "${TMPDIR:-/tmp}/oflibs.XXXXXX")
	# shellcheck disable=SC2064
	trap "rm -rf '$tmp'" RETURN

	# Prefetch GitHub release digests once (ofLibs publishes digest on every asset)
	digestMap=$(mktemp 2>/dev/null || mktemp -t oflibsdigest)
	if [[ "${VERIFY_SHA:-1}" != "0" ]]; then
		echo " ofLibs  fetching SHA-256 digests for ${OFLIBS_REPO}@${tag}…"
		if ! fetchGithubReleaseDigests "$OFLIBS_REPO" "$tag" "$digestMap"; then
			echo "    ! could not fetch ofLibs digests — packages will install without verify" >&2
			: > "$digestMap"
		fi
	fi

	SHA_VERIFIED=0
	SHA_SKIPPED=0
	SHA_FAILED=0
	SHA_STATUS="unchecked"

	echo " ofLibs  tag=${tag}  platform=${plat}  libs=${#libs[@]}  →  ${ofDir}/libs"
	for name in "${libs[@]}"; do
		[[ -n "$name" ]] || continue
		pkg="ofLibs_${name}_${plat}.zip"
		url="https://github.com/${OFLIBS_REPO}/releases/download/${tag}/${pkg}"
		ofName=$(ofLibsToOfName "$name")
		dest="${ofDir}/libs/${ofName}"

		echo "  › ${name} → libs/${ofName}"
		if ! curl -fsSL -o "${dlDir}/${pkg}" "$url"; then
			echo "    ! skip (not available): ${pkg}" >&2
			skip=$((skip + 1))
			continue
		fi

		# SHA-256 against GitHub release asset digest when available
		if [[ "${VERIFY_SHA:-1}" != "0" ]]; then
			if ! sha_has_tools; then
				SHA_STATUS="no-tools"
				[[ "${REQUIRE_SHA:-0}" == "1" ]] && { rm -f "$digestMap"; return 1; }
			else
				expected=$(lookupDigest "$digestMap" "$pkg")
				if [[ -z "$expected" ]]; then
					SHA_SKIPPED=$((SHA_SKIPPED+1))
					echo "    ~ no digest for ${pkg} — skip verify"
				elif verifyFileSha256 "${dlDir}/${pkg}" "$expected"; then
					SHA_VERIFIED=$((SHA_VERIFIED+1))
					echo "    ✓ sha256 verified (secure)"
				else
					SHA_FAILED=$((SHA_FAILED+1))
					fail=$((fail + 1))
					echo "    ✗ SHA-256 MISMATCH — removing ${pkg}" >&2
					rm -f "${dlDir}/${pkg}" "${dlDir}/${pkg}.sha256"
					continue
				fi
			fi
		fi

		rm -rf "${tmp}/extract"
		mkdir -p "${tmp}/extract" "$dest"
		unzip -qo "${dlDir}/${pkg}" -d "${tmp}/extract"
		# Merge into libs/<name> so other platforms under lib/<platform>/ are kept.
		# Headers are shared and replaced; only this package's lib/* children are replaced.
		if [[ -d "${tmp}/extract/include" ]]; then
			mkdir -p "${dest}/include"
			if command -v rsync &>/dev/null; then
				rsync -a --delete "${tmp}/extract/include/" "${dest}/include/"
			else
				rm -rf "${dest}/include"
				mv "${tmp}/extract/include" "${dest}/include"
			fi
		fi
		if [[ -d "${tmp}/extract/lib" ]]; then
			mkdir -p "${dest}/lib"
			# Replace only platform subdirs present in the package (e.g. lib/macos, lib/android)
			local child
			for child in "${tmp}/extract/lib"/*; do
				[[ -e "$child" ]] || continue
				local base
				base=$(basename "$child")
				rm -rf "${dest}/lib/${base}"
				mv "$child" "${dest}/lib/${base}"
			done
		fi
		[[ -f "${tmp}/extract/copying.txt" ]] && cp "${tmp}/extract/copying.txt" "${dest}/" 2>/dev/null || true
		ok=$((ok + 1))
	done

	if [[ $SHA_FAILED -gt 0 ]]; then
		SHA_STATUS="failed"
	elif [[ $SHA_VERIFIED -gt 0 ]]; then
		SHA_STATUS="verified"
	elif [[ $SHA_SKIPPED -gt 0 ]]; then
		SHA_STATUS="no-digest"
	elif [[ "${VERIFY_SHA:-1}" == "0" ]]; then
		SHA_STATUS="skipped"
	fi
	writeVerifyState "${ofDir}/libs/download" "oflibs:${tag}" "$ofPlat" ""
	rm -f "$digestMap"

	echo " ofLibs install complete  ok=${ok}  skipped=${skip}  sha_fail=${fail}  (libs/<lib>/{include,lib/<platform>})"
	if [[ $SHA_VERIFIED -gt 0 ]]; then
		echo " ✓ Secure download: ${SHA_VERIFIED} ofLibs package(s) verified SHA-256 against GitHub digests"
	fi
	echo " note: ofLibs is ofWorks-oriented; stock oF may need path tweaks for some libs."
	[[ $fail -eq 0 && $ok -gt 0 ]]
}

# Download a historical full OF release package from danoli3 archive
downloadArchiveRelease(){
	local ofDir="$1"
	local ver="$2"
	local ofPlat="${3:-osx}"
	local arch="${4:-}"
	local token pkg url dlDir dest extractMode="${5:-libs}" # libs | full
	local tmp

	token=$(archivePlatformToken "$ofPlat" "$arch")
	# resolve exact filename from listing
	pkg=$(listArchivePackages "$ver" | grep -E "_${token}_release\.(zip|tar\.gz)$" | head -1)
	if [[ -z "$pkg" ]]; then
		# try looser match
		pkg=$(listArchivePackages "$ver" | grep -E "${token}" | head -1)
	fi
	if [[ -z "$pkg" ]]; then
		echo "no archive package for version=${ver} platform token=${token}" >&2
		echo "available:" >&2
		listArchivePackages "$ver" >&2
		return 1
	fi

	url="${ARCHIVE_BASE}/versions/${ver}/${pkg}"
	dlDir="${ofDir}/libs/download/archive"
	mkdir -p "$dlDir"
	echo " archive  ${ver}  ${pkg}"
	echo " url  ${url}"
	if ! curl -fL --progress-bar -o "${dlDir}/${pkg}" "$url"; then
		echo "download failed: $url" >&2
		return 1
	fi

	# SHA when published on libs.danoli3.com (sidecar .sha256 or SHA256SUMS — see generate_sha256.php)
	SHA_VERIFIED=0
	SHA_SKIPPED=0
	SHA_FAILED=0
	SHA_STATUS="unchecked"
	if [[ "${VERIFY_SHA:-1}" != "0" ]]; then
		echo " archive  checking SHA-256 sidecars…"
		if ! sha_has_tools; then
			SHA_STATUS="no-tools"
			echo "  ! no local sha256 tool — skip verify"
			[[ "${REQUIRE_SHA:-0}" == "1" ]] && return 1
		else
			local expected
			expected=$(fetchUrlSidecarDigest "$url") || expected=""
			if [[ -z "$expected" ]]; then
				SHA_SKIPPED=1
				SHA_STATUS="no-digest"
				echo "  ~ no SHA published for ${pkg} yet (add ${pkg}.sha256 or SHA256SUMS on server)"
				[[ "${REQUIRE_SHA:-0}" == "1" ]] && return 1
			elif verifyFileSha256 "${dlDir}/${pkg}" "$expected"; then
				SHA_VERIFIED=1
				SHA_STATUS="verified"
				echo "  ✓ sha256 verified (secure)"
				echo "      sha256:${expected}"
			else
				SHA_FAILED=1
				SHA_STATUS="failed"
				echo "  ✗ SHA-256 MISMATCH — removing corrupt download" >&2
				rm -f "${dlDir}/${pkg}" "${dlDir}/${pkg}.sha256"
				writeVerifyState "${ofDir}/libs/download" "archive:${ver}" "$ofPlat" "$arch"
				return 1
			fi
		fi
	else
		SHA_STATUS="skipped"
	fi
	writeVerifyState "${ofDir}/libs/download" "archive:${ver}" "$ofPlat" "$arch"

	tmp=$(mktemp -d "${TMPDIR:-/tmp}/ofarchive.XXXXXX")
	# shellcheck disable=SC2064
	trap "rm -rf '$tmp'" RETURN

	case "$pkg" in
		*.zip) unzip -qo "${dlDir}/${pkg}" -d "$tmp" ;;
		*.tar.gz) tar -xzf "${dlDir}/${pkg}" -C "$tmp" ;;
		*.tar.bz2) tar -xjf "${dlDir}/${pkg}" -C "$tmp" ;;
		*) echo "unknown archive type: $pkg" >&2; return 1 ;;
	esac

	# find extracted root (usually of_vX.Y.Z_*)
	local root
	root=$(find "$tmp" -mindepth 1 -maxdepth 2 -type d -name 'of_*' | head -1)
	[[ -z "$root" ]] && root=$(find "$tmp" -mindepth 1 -maxdepth 1 -type d | head -1)
	if [[ -z "$root" ]]; then
		echo "could not locate extracted OF root in archive" >&2
		return 1
	fi

	if [[ "$extractMode" == "full" ]]; then
		dest="${ofDir}/../of_releases/${ver}_${token}"
		mkdir -p "$(dirname "$dest")"
		rm -rf "$dest"
		mv "$root" "$dest"
		echo " full release extracted → ${dest}"
	else
		# merge libs/ only into current tree
		if [[ ! -d "${root}/libs" ]]; then
			echo "archive has no libs/ folder at ${root}" >&2
			return 1
		fi
		echo " merging libs/ from ${ver} into ${ofDir}/libs"
		mkdir -p "${ofDir}/libs"
		# copy library folders, skip download cache
		local d
		for d in "${root}/libs"/*; do
			[[ -e "$d" ]] || continue
			[[ "$(basename "$d")" == "download" ]] && continue
			rm -rf "${ofDir}/libs/$(basename "$d")"
			cp -R "$d" "${ofDir}/libs/"
		done
		echo " archive libs merge complete"
	fi
}
