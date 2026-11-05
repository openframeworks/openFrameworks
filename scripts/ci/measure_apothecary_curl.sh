#!/usr/bin/env bash
# Curl matching apothecary packages into DEST. Prints:
#   elapsed_ms=<n>
#   size_bytes=<n>
#   n_files=<n>
# Used by .github/workflows/measure-apothecary-cache.yml to compare GitHub
# release curl against Actions cache restore.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
DEST=""
ARGS=()
while [[ $# -gt 0 ]]; do
	case "$1" in
		--dest) DEST="$2"; shift 2 ;;
		*) ARGS+=("$1"); shift ;;
	esac
done
[[ -n "$DEST" ]] || { echo "Usage: $0 --dest DIR [--platform ...]" >&2; exit 2; }
mkdir -p "$DEST"
NAMES=()
while IFS= read -r name; do
	[[ -n "$name" ]] && NAMES+=("$name")
done < <(bash "$ROOT/scripts/ci/apothecary_cache_key.sh" --list "${ARGS[@]}")
if [[ ${#NAMES[@]} -eq 0 ]]; then
	echo "elapsed_ms=0"
	echo "size_bytes=0"
	echo "n_files=0"
	echo "no matching digested assets" >&2
	exit 0
fi
TAG=latest
for i in "${!ARGS[@]}"; do
	if [[ "${ARGS[$i]}" == "--tag" ]]; then TAG="${ARGS[$((i+1))]}"; fi
done
PY=python3
command -v python3 >/dev/null 2>&1 || PY=python
START=$("$PY" -c 'import time; print(time.perf_counter())')
bytes=0
n=0
for name in "${NAMES[@]}"; do
	[[ -n "$name" ]] || continue
	url="https://github.com/openframeworks/apothecary/releases/download/${TAG}/${name}"
	echo "curl ${name}"
	curl -fsSL --retry 5 --retry-delay 2 -o "$DEST/$name" "$url"
	bytes=$((bytes + $(wc -c < "$DEST/$name")))
	n=$((n + 1))
done
END=$("$PY" -c 'import time; print(time.perf_counter())')
MS=$("$PY" -c "print(int((${END}-${START})*1000))")
echo "elapsed_ms=${MS}"
echo "size_bytes=${bytes}"
echo "n_files=${n}"
