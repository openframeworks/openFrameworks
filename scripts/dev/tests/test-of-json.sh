#!/usr/bin/env bash
# Contract tests for `of ... --json` (alias --api): one JSON document on stdout,
# human text on stderr, no colour, never interactive, stable error codes and
# exit codes. Runs against a throwaway copy of the scripts.
set -u
ROOT=$(cd "$(dirname "$0")/../../.." && pwd)
if ! command -v python3 >/dev/null 2>&1; then echo "skip: python3 not found"; exit 0; fi
TEMP=$(mktemp -d)
trap 'rm -rf "$TEMP"' EXIT
TREE="$TEMP/of"
FAILS=0; CHECKS=0
pass(){ CHECKS=$((CHECKS + 1)); }
fail(){ CHECKS=$((CHECKS + 1)); FAILS=$((FAILS + 1)); printf 'FAIL: %s\n' "$1" >&2; }
expect(){ local d="$1"; shift; if "$@"; then pass; else fail "$d"; fi; }

mkdir -p "$TREE"; cp -R "$ROOT/scripts" "$TREE/scripts"
(
	cd "$TREE" || exit 1
	mkdir -p libs/openFrameworks libs/zlib/lib/macos libs/zlib/lib/linux64 libs/download \
		apps/myApps/p1/src apps/myApps/p1/obj apps/myApps/p1/bin/data
	head -c 100000 /dev/zero > libs/zlib/lib/macos/z.a
	head -c 50000 /dev/zero > libs/download/a.zip
	head -c 40000 /dev/zero > apps/myApps/p1/obj/o.o
)

# run <args…> : stdout -> $TEMP/out.json, stderr -> $TEMP/err.txt, status in RC
run(){
	"$BASH" "$TREE/scripts/of.sh" "$@" </dev/null >"$TEMP/out.json" 2>"$TEMP/err.txt"
	RC=$?
}
# j <python expression over d> : exits 0 when the expression is true
j(){ python3 -c "import json,sys; d=json.load(open(sys.argv[1],encoding='utf-8')); sys.exit(0 if ($2) else 1)" "$TEMP/out.json" ; }
lines(){ wc -l < "$1" | tr -d ' '; }

# --- envelope on every supported command ------------------------------------------
for c in "status" "version" "version pg" "menu" "cleanup usage" "cleanup libs macos --dry-run" "cleanup core all --dry-run"; do
	# shellcheck disable=SC2086
	run $c --json
	expect "[$c] exit 0" test "$RC" -eq 0
	expect "[$c] stdout is exactly one JSON document" test "$(lines "$TEMP/out.json")" -ge 1 -a "$(python3 -c "import json,sys; json.load(open(sys.argv[1],encoding='utf-8')); print('ok')" "$TEMP/out.json" 2>/dev/null)" = ok
	expect "[$c] envelope fields" j x "d['version']==1 and d['ok'] is True and d['error'] is None and isinstance(d['warnings'],list) and isinstance(d['data'],dict) and d['command']"
	expect "[$c] no colour codes on stdout" bash -c '! grep -q "$(printf "\033")" "$1"' _ "$TEMP/out.json"
done

# --api is an alias
run status --api
expect "--api works like --json" j x "d['ok'] is True and d['command']=='status'"
# flag position does not matter
run --json version
expect "--json before the command" j x "d['ok'] is True and d['command']=='version'"
# no arguments at all: the catalogue, never a menu
run --json
expect "bare --json gives the catalogue" j x "d['ok'] and 'commands' in d['data']"

# --- data shapes -----------------------------------------------------------------
run status --json
expect "status data" j x "set(['openframeworks','cli','platform','git','project_generator','libraries','integrity']) <= set(d['data'])"
run version --json
expect "version data has the script version" j x "d['data']['cli'] and 'platform' in d['data']"
run menu --json
expect "catalogue lists cleanup, flags and exit codes" j x "any(c['id']=='cleanup.libs' and c['destructive'] for c in d['data']['commands']) and '--json' in d['data']['flags'] and '3' in d['data']['exit_codes']"
run cleanup usage --json
expect "usage categories" j x "set(d['data']['categories'])=={'projects','caches','libs','core'} and d['data']['unit']=='KiB'"
expect "usage sees the project and the download" j x "any(i['path']=='apps/myApps/p1' for i in d['data']['categories']['projects']['items']) and d['data']['categories']['caches']['total_kb']>0"

# --- cleanup results -------------------------------------------------------------
run cleanup libs macos --dry-run --json
expect "dry run: would_remove item" j x "d['dry_run'] is True and d['data']['items'][0]['action']=='would_remove' and d['data']['totals']['removed']==1 and d['data']['totals']['removed_kb']>0"
expect "dry run: nothing deleted" test -f "$TREE/libs/zlib/lib/macos/z.a"
run cleanup projects all --dry-run --json
expect "projects dry run lists the project" j x "d['data']['items'][0]['path']=='apps/myApps/p1' and d['data']['items'][0]['action']=='would_remove'"
run cleanup libs macos --yes --json
expect "real run exits 0" test "$RC" -eq 0
expect "real run: removed item and totals" j x "d['ok'] and d['data']['items'][0]['action']=='removed' and d['data']['totals']['removed']==1 and d['data']['totals']['failed']==0"
expect "real run deleted it" test ! -e "$TREE/libs/zlib/lib/macos"
run cleanup caches packages --yes --json
expect "caches: per-file item" j x "d['data']['items'][0]['path']=='libs/download/a.zip'"

# --- never interactive; refusals and errors have stable codes ------------------------
run cleanup libs linux64 --json
expect "destructive without --yes: exit 3" test "$RC" -eq 3
expect "destructive without --yes: confirmation_required" j x "d['ok'] is False and d['error']['code']=='confirmation_required'"
expect "refusal deleted nothing" test -d "$TREE/libs/zlib/lib/linux64"
run cleanup libs bogus --yes --json
expect "bad platform: exit 2, invalid_argument, no data" bash -c '[[ "$1" == 2 ]]' _ "$RC"
expect "bad platform envelope" j x "d['error']['code']=='invalid_argument' and d['data'] is None"
run cleanup --json
expect "cleanup without a subcommand: invalid_argument" j x "d['error']['code']=='invalid_argument'"
run build --json
expect "unsupported command: exit 2" bash -c '[[ "$1" == 2 ]]' _ "$RC"
expect "unsupported command: unsupported_command" j x "d['ok'] is False and d['error']['code']=='unsupported_command'"
run version bogus --json
expect "bad version target: invalid_argument" j x "d['error']['code']=='invalid_argument'"

# --- human text stays off stdout ---------------------------------------------------------
run cleanup libs linux64 --dry-run --json
expect "human text goes to stderr" test "$(lines "$TEMP/err.txt")" -gt 0
expect "stdout has no banner" bash -c '! grep -q "openFrameworks  ·" "$1"' _ "$TEMP/out.json"

# --- plain mode is unchanged -----------------------------------------------------------
OUT="$("$BASH" "$TREE/scripts/of.sh" version </dev/null 2>&1)"
expect "without --json the output is human text" bash -c '! grep -q "\"ok\"" <<<"$1"' _ "$OUT"

echo "of --json: $((CHECKS - FAILS))/${CHECKS} checks passed"
[[ $FAILS -eq 0 ]]
