#!/usr/bin/env bash
# scripts/vs/download_libs.ps1 must behave like scripts/vs/download_libs.sh.
# Both are run against a stub of ../dev/download_libs.sh that records what it was
# given, and the recorded arguments and printed output are compared.
# Needs PowerShell (pwsh, or powershell on Windows); skips when there is none.
set -u
ROOT=$(cd "$(dirname "$0")/../../.." && pwd)
PS=""
for c in pwsh powershell; do command -v "$c" >/dev/null 2>&1 && { PS="$c"; break; }; done
[[ -n "$PS" ]] || { echo "skip: no PowerShell found"; exit 0; }

TEMP=$(mktemp -d)
trap 'rm -rf "$TEMP"' EXIT
FAILS=0; CHECKS=0
pass(){ CHECKS=$((CHECKS + 1)); }
fail(){ CHECKS=$((CHECKS + 1)); FAILS=$((FAILS + 1)); printf 'FAIL: %s\n' "$1" >&2; }

T="$TEMP/of"
mkdir -p "$T/scripts/vs" "$T/scripts/dev"
cp "$ROOT/scripts/vs/download_libs.sh" "$ROOT/scripts/vs/download_libs.ps1" "$T/scripts/vs/"
cat > "$T/scripts/dev/download_libs.sh" <<'STUB'
#!/usr/bin/env bash
# stand-in for the real downloader: record how it was called
printf 'cwd=%s\n' "$(basename "$(pwd)")"
n=0; for a in "$@"; do n=$((n + 1)); printf 'arg%d=[%s]\n' "$n" "$a"; done
printf 'argc=%d\n' "$n"
exit "${STUB_EXIT:-0}"
STUB
chmod +x "$T/scripts/vs/download_libs.sh" "$T/scripts/dev/download_libs.sh"

PS_FILE="$T/scripts/vs/download_libs.ps1"
command -v cygpath >/dev/null 2>&1 && PS_FILE="$(cygpath -w "$PS_FILE")"

run_sh(){ ( cd "$T/scripts/vs" && "$@" 2>&1 ); }
run_ps(){ ( "$PS" -NoProfile -ExecutionPolicy Bypass -File "$PS_FILE" "$@" 2>&1 ); }
# compare <label> <args...> : same output from both entry points
compare(){
	local label="$1"; shift
	local a b
	a="$(cd "$T/scripts/vs" && ./download_libs.sh "$@" 2>&1; echo "exit=$?")"
	b="$(run_ps "$@"; echo "exit=$?")"
	a="$(printf '%s' "$a" | tr -d '\r')"; b="$(printf '%s' "$b" | tr -d '\r')"
	if [[ "$a" == "$b" ]]; then pass; else fail "$label"; printf -- '--- sh ---\n%s\n--- ps1 ---\n%s\n' "$a" "$b" >&2; fi
}

echo "PowerShell: $("$PS" -NoProfile -Command '$PSVersionTable.PSVersion.ToString()' 2>&1 | tr -d '\r') ($PS) · bash ${BASH_VERSION}"

compare "no arguments (host architecture default)"
compare "-a arm64 is passed through"            -a arm64
compare "--arch 64 is passed through"           --arch 64
compare "-a all"                                -a all
compare "other options are passed on"           -t 1.2.3 --no-overwrite
compare "--help"                                --help
compare "an argument with spaces"               -t "two words"
compare "an empty argument"                     "" -a 64
compare "several options after -a"              -a arm64 -n --full-clean

# BITS makes the .sh pick the architecture; the ps1 inherits the environment
a="$(cd "$T/scripts/vs" && BITS=arm64ec ./download_libs.sh -t x 2>&1 | tr -d '\r')"
b="$(BITS=arm64ec run_ps -t x | tr -d '\r')"
if [[ "$a" == "$b" ]]; then pass; else fail "BITS is honoured"; printf -- '--- sh ---\n%s\n--- ps1 ---\n%s\n' "$a" "$b" >&2; fi

# the exit code of the downloader comes back
( cd "$T/scripts/vs" && STUB_EXIT=3 ./download_libs.sh >/dev/null 2>&1 ); a=$?
STUB_EXIT=3 run_ps >/dev/null 2>&1; b=$?
[[ "$a" == "3" && "$b" == "3" ]] && pass || fail "exit code is returned (sh=$a ps1=$b)"

# the script can be started from any directory
a="$(cd "$TEMP" && "$PS" -NoProfile -ExecutionPolicy Bypass -File "$PS_FILE" -a 64 2>&1 | tr -d '\r' | grep -c 'cwd=vs')"
[[ "$a" == "1" ]] && pass || fail "runs the downloader from scripts/vs whatever the current directory"

echo "vs download_libs ps1 parity: $((CHECKS - FAILS))/${CHECKS} checks passed"
[[ $FAILS -eq 0 ]]
