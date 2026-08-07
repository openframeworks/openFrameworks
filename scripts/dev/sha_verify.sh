#!/usr/bin/env bash
# sha_verify.sh — shared SHA-256 helpers for download_libs / ofLibs / archive
# Source from other scripts:  . "$(dirname "$0")/sha_verify.sh"
#
# Env:
#   VERIFY_SHA=1|0     default 1
#   REQUIRE_SHA=1|0    default 0 — fail when digest missing / no tools

VERIFY_SHA="${VERIFY_SHA:-1}"
REQUIRE_SHA="${REQUIRE_SHA:-0}"
SHA_VERIFIED="${SHA_VERIFIED:-0}"
SHA_SKIPPED="${SHA_SKIPPED:-0}"
SHA_FAILED="${SHA_FAILED:-0}"
SHA_STATUS="${SHA_STATUS:-unchecked}"

# Cross-platform SHA-256 of a file → lowercase hex (no filename)
sha256_file(){
	local f="$1"
	if command -v sha256sum >/dev/null 2>&1; then
		sha256sum "$f" 2>/dev/null | awk '{print tolower($1)}'
	elif command -v shasum >/dev/null 2>&1; then
		shasum -a 256 "$f" 2>/dev/null | awk '{print tolower($1)}'
	elif command -v openssl >/dev/null 2>&1; then
		openssl dgst -sha256 "$f" 2>/dev/null | awk '{print tolower($NF)}'
	else
		return 1
	fi
}

sha_has_tools(){
	command -v sha256sum >/dev/null 2>&1 \
		|| command -v shasum >/dev/null 2>&1 \
		|| command -v openssl >/dev/null 2>&1
}

# Parse GitHub Releases JSON → name<TAB>hex  (uses asset "digest": "sha256:…")
# Usage: githubReleaseDigestsToMap <api.json> <out.map>
githubReleaseDigestsToMap(){
	local api="$1"
	local out="$2"
	: > "$out"
	[[ -f "$api" ]] || return 1
	if command -v python3 >/dev/null 2>&1; then
		python3 - "$api" "$out" <<'PY'
import json, sys
src, dst = sys.argv[1], sys.argv[2]
with open(src, encoding="utf-8") as f:
    data = json.load(f)
with open(dst, "w", encoding="utf-8") as out:
    for a in data.get("assets") or []:
        name = a.get("name") or ""
        digest = a.get("digest") or ""
        if not name or not digest:
            continue
        if ":" in digest:
            algo, hx = digest.split(":", 1)
        else:
            algo, hx = "sha256", digest
        if algo.lower() != "sha256":
            continue
        out.write(f"{name}\t{hx.lower()}\n")
PY
	else
		awk '
			/"name":/ {
				if (match($0, /"name":[[:space:]]*"[^"]+"/)) {
					n = substr($0, RSTART, RLENGTH)
					gsub(/"name":[[:space:]]*"/, "", n)
					gsub(/"/, "", n)
					name = n
				}
			}
			/"digest":/ {
				if (name != "" && match($0, /sha256:[a-fA-F0-9]+/)) {
					d = substr($0, RSTART, RLENGTH)
					sub(/^sha256:/, "", d)
					print name "\t" tolower(d)
					name = ""
				}
			}
		' "$api" > "$out"
	fi
	[[ -s "$out" ]]
}

# Fetch digests for owner/repo @ tag (or "latest") into map file
# Usage: fetchGithubReleaseDigests owner/repo tag out.map
fetchGithubReleaseDigests(){
	local repo="$1"
	local tag="$2"
	local out="$3"
	local url api
	: > "$out"
	if [[ "$tag" == "latest" ]]; then
		url="https://api.github.com/repos/${repo}/releases/latest"
	else
		url="https://api.github.com/repos/${repo}/releases/tags/${tag}"
	fi
	api=$(mktemp 2>/dev/null || mktemp -t ofdigest)
	if ! curl -fsSL -H "Accept: application/vnd.github+json" \
		-H "User-Agent: openFrameworks-sha-verify" \
		"$url" -o "$api" 2>/dev/null; then
		rm -f "$api"
		return 1
	fi
	githubReleaseDigestsToMap "$api" "$out"
	local rc=$?
	rm -f "$api"
	return $rc
}

lookupDigest(){
	local map="$1"
	local name="$2"
	[[ -f "$map" ]] || return 1
	awk -F'\t' -v n="$name" '$1==n {print $2; exit}' "$map"
}

# Parse common checksum file formats → hex only for basename
# Accepts:
#   deadbeef...
#   deadbeef  filename
#   deadbeef *filename
#   sha256:deadbeef
parseChecksumText(){
	local text="$1"
	local want="$2" # optional basename to match
	local line hx file
	# Store regex in vars so bash 3.2 does not mangle {64} via brace expansion
	local re_sha='^sha256:([a-fA-F0-9]{64})$'
	local re_hex='^([a-fA-F0-9]{64})$'
	local re_sum='^([a-fA-F0-9]{64})[[:space:]]+\*?(.+)$'
	while IFS= read -r line || [[ -n "$line" ]]; do
		line=$(printf '%s' "$line" | tr -d '\r')
		[[ -z "$line" || "$line" == \#* ]] && continue
		# sha256:HEX
		if [[ "$line" =~ $re_sha ]]; then
			printf '%s' "$(printf '%s' "${BASH_REMATCH[1]}" | tr 'A-F' 'a-f')"
			return 0
		fi
		# HEX only
		if [[ "$line" =~ $re_hex ]]; then
			printf '%s' "$(printf '%s' "${BASH_REMATCH[1]}" | tr 'A-F' 'a-f')"
			return 0
		fi
		# HEX  file  or  HEX *file
		if [[ "$line" =~ $re_sum ]]; then
			hx=$(printf '%s' "${BASH_REMATCH[1]}" | tr 'A-F' 'a-f')
			file=$(basename "${BASH_REMATCH[2]}")
			if [[ -z "$want" || "$file" == "$want" ]]; then
				printf '%s' "$hx"
				return 0
			fi
		fi
	done <<< "$text"
	return 1
}

# Fetch expected digest for a remote package URL (sidecar / SHA256SUMS)
# Tries:  URL.sha256  URL.sha256sum  dir/SHA256SUMS  dir/checksums.sha256  dir/sha256sums.txt
fetchUrlSidecarDigest(){
	local url="$1"
	local basename
	basename=$(basename "$url")
	local dir text
	dir=$(printf '%s' "$url" | sed 's|/[^/]*$||')
	local candidates=(
		"${url}.sha256"
		"${url}.sha256sum"
		"${dir}/${basename}.sha256"
		"${dir}/SHA256SUMS"
		"${dir}/sha256sums.txt"
		"${dir}/checksums.sha256"
		"${dir}/checksums.txt"
	)
	local c
	for c in "${candidates[@]}"; do
		text=$(curl -fsSL --max-time 15 "$c" 2>/dev/null) || continue
		[[ -z "$text" ]] && continue
		if parseChecksumText "$text" "$basename"; then
			return 0
		fi
	done
	return 1
}

# Verify one local file against expected hex. Writes sidecar .sha256 on success.
# Sets: returns 0 match, 1 mismatch, 2 no expected / skip
verifyFileSha256(){
	local file="$1"
	local expected="$2"
	local actual
	[[ -f "$file" ]] || return 1
	if [[ -z "$expected" ]]; then
		return 2
	fi
	expected=$(printf '%s' "$expected" | tr 'A-F' 'a-f' | tr -d ' \r\n')
	actual=$(sha256_file "$file") || actual=""
	if [[ -z "$actual" ]]; then
		return 1
	fi
	if [[ "$actual" == "$expected" ]]; then
		printf '%s\n' "$actual" > "${file}.sha256"
		return 0
	fi
	return 1
}

writeVerifyState(){
	local state_dir="$1"
	local release="${2:-}"
	local platform="${3:-}"
	local arch="${4:-}"
	mkdir -p "$state_dir"
	cat > "${state_dir}/.last-verify" << EOF
status=${SHA_STATUS}
algo=sha256
release=${release}
platform=${platform}
arch=${arch}
verified=${SHA_VERIFIED}
skipped=${SHA_SKIPPED}
failed=${SHA_FAILED}
updated=$(date -u +%Y-%m-%dT%H:%M:%SZ)
EOF
}

# Verify list of basenames in cwd against a digest map file (name<TAB>hex)
# Usage: verifyPackagesAgainstMap "pkg1 pkg2" mapfile
verifyPackagesAgainstMap(){
	local pkgs="$1"
	local map="$2"
	local label="${3:-GitHub release digests}"
	local expected actual pkg any_expected=0

	SHA_VERIFIED=0
	SHA_SKIPPED=0
	SHA_FAILED=0
	SHA_STATUS="unchecked"

	if [[ "${VERIFY_SHA}" == "0" ]]; then
		SHA_STATUS="skipped"
		echo " SHA-256 verify disabled (VERIFY_SHA=0 / --skip-sha)"
		return 0
	fi

	echo " "
	echo " ------ "
	echo " Verifying package integrity (SHA-256 via ${label})…"

	if ! sha_has_tools; then
		SHA_STATUS="no-tools"
		echo "  ! no local sha256 tool (sha256sum/shasum/openssl) — cannot verify"
		[[ "${REQUIRE_SHA}" == "1" ]] && return 1
		return 0
	fi

	if [[ ! -s "$map" ]]; then
		SHA_STATUS="no-remote"
		echo "  ! no digest map available — integrity not verified"
		[[ "${REQUIRE_SHA}" == "1" ]] && return 1
		return 0
	fi

	for pkg in $pkgs; do
		[[ -f "$pkg" ]] || { echo "  ! missing package file: $pkg"; SHA_FAILED=$((SHA_FAILED+1)); continue; }
		expected=$(lookupDigest "$map" "$pkg")
		if [[ -z "$expected" ]]; then
			SHA_SKIPPED=$((SHA_SKIPPED+1))
			echo "  ~ $pkg  (no digest — skip)"
			continue
		fi
		any_expected=1
		if verifyFileSha256 "$pkg" "$expected"; then
			SHA_VERIFIED=$((SHA_VERIFIED+1))
			actual=$(cat "${pkg}.sha256" 2>/dev/null || true)
			echo "  ✓ $pkg"
			echo "      sha256:${actual}"
		else
			SHA_FAILED=$((SHA_FAILED+1))
			actual=$(sha256_file "$pkg" 2>/dev/null || echo "?")
			echo "  ✗ $pkg  SHA-256 MISMATCH"
			echo "      expected: $expected"
			echo "      actual:   $actual"
			echo "      removing corrupt download"
			rm -f "$pkg" "${pkg}.sha256"
		fi
	done

	if [[ $SHA_FAILED -gt 0 ]]; then
		SHA_STATUS="failed"
		echo " "
		echo " ERROR: $SHA_FAILED package(s) failed SHA-256 verification."
		return 1
	fi
	if [[ $SHA_VERIFIED -gt 0 ]]; then
		SHA_STATUS="verified"
		echo " "
		echo " ✓ Secure download: $SHA_VERIFIED package(s) verified SHA-256"
		[[ $SHA_SKIPPED -gt 0 ]] && echo "   ($SHA_SKIPPED without published digest — not checked)"
	elif [[ $any_expected -eq 0 ]]; then
		SHA_STATUS="no-digest"
		echo "  ~ no SHA digests for these packages — integrity not verified"
		[[ "${REQUIRE_SHA}" == "1" ]] && return 1
	else
		SHA_STATUS="skipped"
	fi
	return 0
}
