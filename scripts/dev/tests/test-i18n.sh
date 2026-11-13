#!/usr/bin/env bash
# Tests for the string dictionary: scripts/i18n/strings.tsv (one file, a column
# per language) is in sync with the code, --lang / OF_LANG pick a language,
# placeholders are filled, missing languages and empty cells fall back to
# English, and `of i18n` exports / imports / checks. Runs on a throwaway copy.
set -u
ROOT=$(cd "$(dirname "$0")/../../.." && pwd)
TEMP=$(mktemp -d)
trap 'rm -rf "$TEMP"' EXIT
case "$(uname -s)" in MINGW*|MSYS*|CYGWIN*) [[ -n "${LC_ALL:-}" ]] || export LC_ALL=C.UTF-8 ;; esac
TREE="$TEMP/of"
FAILS=0; CHECKS=0
pass(){ CHECKS=$((CHECKS + 1)); }
fail(){ CHECKS=$((CHECKS + 1)); FAILS=$((FAILS + 1)); printf 'FAIL: %s\n' "$1" >&2; }
expect(){ local d="$1"; shift; if "$@"; then pass; else fail "$d"; fi; }

mkdir -p "$TREE/libs/openFrameworks" "$TREE/libs/download"
cp -R "$ROOT/scripts" "$TREE/scripts"
CAT="$TREE/scripts/i18n/strings.tsv"
[[ -f "$CAT" ]] || { echo "FAIL: scripts/i18n/strings.tsv is missing" >&2; exit 1; }
cp "$CAT" "$TEMP/strings.orig.tsv"

run(){ OUT="$("$BASH" "$TREE/scripts/of.sh" "$@" </dev/null 2>&1)"; RC=$?; OUT="$(printf '%s' "$OUT" | sed $'s/\x1b\\[[0-9;?]*[a-zA-Z]//g')"; }
key_for(){ awk -F'\t' -v t="$1" '{ sub(/\r$/, "", $2) } $2==t { print $1; exit }' "$CAT"; }
# cell <lang> <key> : that language's text for a key
cell(){ awk -F'\t' -v l="$1" -v k="$2" 'NR==1 { for (i = 2; i <= NF; i++) if ($i == l) c = i; next } $1==k && c { sub(/\r$/, "", $c); print $c; exit }' "$CAT"; }
# addlang <lang> <key> <text> [<key> <text> ...] : import through `of i18n import`
addlang(){
	local lang="$1"; shift
	: > "$TEMP/in.tsv"
	while [[ $# -ge 2 ]]; do printf '%s\t%s\n' "$1" "$2" >> "$TEMP/in.tsv"; shift 2; done
	run i18n import "$lang" "$TEMP/in.tsv"
}

# --- catalogue ------------------------------------------------------------------
if command -v python3 >/dev/null 2>&1; then
	expect "strings.tsv is in sync with the code (python3 scripts/dev/i18n_extract.py)" python3 "$ROOT/scripts/dev/i18n_extract.py" --check
fi
expect "header is key<TAB>en" test "$(head -1 "$CAT" | tr -d '\r')" = "$(printf 'key\ten')"
expect "every row has the same number of columns as the header" test -z "$(awk -F'\t' 'NR==1{n=NF} NF!=n || $1=="" || $2==""' "$CAT" | head -1)"
expect "catalogue keys are unique" test -z "$(tail -n +2 "$CAT" | cut -f1 | sort | uniq -d | head -1)"
expect "catalogue has a few hundred strings" test "$(wc -l < "$CAT" | tr -d ' ')" -gt 200

K_NONE="$(key_for 'no other platforms to remove')"
K_PLAT="$(key_for 'no lib/%s folders to remove')"
K_CANCEL="$(key_for 'cancelled — nothing deleted')"
expect "known keys exist" test -n "$K_NONE" -a -n "$K_PLAT" -a -n "$K_CANCEL"

mkdir -p "$TREE/libs/zlib/lib/macos"; : > "$TREE/libs/zlib/lib/macos/z.a"

# --- one file for every language ---------------------------------------------------
addlang xx "$K_NONE" "XX: nothing else to remove" "$K_PLAT" "XX: no lib/%s here"
expect "import xx succeeds" test "$RC" -eq 0
addlang yy "$K_PLAT" "%s :YY"
addlang zh "$K_NONE" "没有其他平台可删除"
expect "three languages are columns of the same file" test "$(head -1 "$CAT" | tr -d '\r')" = "$(printf 'key\ten\txx\tyy\tzh')"
expect "still exactly one .tsv file" test "$(ls "$TREE/scripts/i18n"/*.tsv | wc -l | tr -d ' ')" = 1
expect "rows keep the column count after imports" test -z "$(awk -F'\t' 'NR==1{n=NF} NF!=n' "$CAT" | head -1)"
expect "English column untouched by imports" test "$(cut -f1,2 "$CAT")" = "$(cut -f1,2 "$TEMP/strings.orig.tsv")"
expect "xx cell holds the translation" test "$(cell xx "$K_NONE")" = "XX: nothing else to remove"

# --- selecting a language ------------------------------------------------------------
PLATFORM=osx run cleanup libs other
expect "English by default" grep -q "no other platforms to remove" <<<"$OUT"
PLATFORM=osx run cleanup libs other --lang xx
expect "--lang xx translates a message" grep -q "XX: nothing else to remove" <<<"$OUT"
expect "--lang xx: the English text is gone" bash -c '! grep -q "no other platforms to remove" <<<"$1"' _ "$OUT"
OUT="$(PLATFORM=osx OF_LANG=xx "$BASH" "$TREE/scripts/of.sh" cleanup libs other </dev/null 2>&1)"
expect "OF_LANG=xx works too" grep -q "XX: nothing else to remove" <<<"$OUT"
PLATFORM=osx run cleanup libs other --lang=xx
expect "--lang=xx works too" grep -q "XX: nothing else to remove" <<<"$OUT"
PLATFORM=osx run --lang xx cleanup libs other
expect "--lang before the command" grep -q "XX: nothing else to remove" <<<"$OUT"
run cleanup libs linux64 --lang xx
expect "placeholders are filled" grep -q "XX: no lib/linux64 here" <<<"$OUT"
expect "--lang is not passed on as a command argument" test "$RC" -eq 0
run cleanup libs linux64 --lang yy
expect "placeholder position can move" grep -q "linux64 :YY" <<<"$OUT"
PLATFORM=osx run cleanup libs other --lang yy
expect "an empty cell falls back to English" grep -q "no other platforms to remove" <<<"$OUT"
PLATFORM=osx run cleanup libs other --lang zh
expect "Chinese translation is shown" grep -q "没有其他平台可删除" <<<"$OUT"
PLATFORM=osx run cleanup libs other --lang nosuchlang
expect "unknown language: warns" grep -q "nosuchlang" <<<"$OUT"
expect "unknown language: falls back to English" grep -q "no other platforms to remove" <<<"$OUT"
PLATFORM=osx run cleanup libs other --lang 'key'
expect "the key column is not a language" grep -q "no other platforms to remove" <<<"$OUT"
run cleanup libs other --lang '../../etc/passwd'
expect "a path-like language name is not loaded" bash -c '! grep -q "root:" <<<"$1"' _ "$OUT"
run cleanup libs bogus --yes --json --lang xx
expect "--json and --lang together: no crash" bash -c 'echo "$1" >/dev/null' _ "$OUT"

# --- of i18n ---------------------------------------------------------------------------
run i18n list
expect "i18n list shows the catalogue size and every language" bash -c 'grep -q "strings" <<<"$1" && grep -q "xx" <<<"$1" && grep -q "yy" <<<"$1" && grep -q "zh" <<<"$1"' _ "$OUT"
"$BASH" "$TREE/scripts/of.sh" i18n export </dev/null 2>/dev/null > "$TEMP/export.tsv"
expect "i18n export (English): key and English columns of the catalogue" cmp -s "$TEMP/export.tsv" <(cut -f1,2 "$CAT" | tail -n +2 | tr -d '\r')
"$BASH" "$TREE/scripts/of.sh" i18n export xx </dev/null 2>/dev/null > "$TEMP/export-xx.tsv"
expect "i18n export xx: three columns with the translation" test -n "$(awk -F'\t' -v k="$K_NONE" '$1==k && NF==3 && $3=="XX: nothing else to remove"' "$TEMP/export-xx.tsv")"
expect "i18n export xx: an untranslated row has an empty third column" test -n "$(awk -F'\t' -v k="$K_CANCEL" '$1==k && NF==3 && $3==""' "$TEMP/export-xx.tsv")"

if command -v python3 >/dev/null 2>&1; then
	"$BASH" "$TREE/scripts/of.sh" i18n export xx --json </dev/null 2>/dev/null > "$TEMP/export.json"
	expect "i18n export --json: envelope with strings and the translation" python3 -c "
import json,sys
d=json.load(open(sys.argv[1],encoding='utf-8'))
assert d['ok'] and d['data']['source_lang']=='en' and d['data']['lang']=='xx'
s={i['key']:i for i in d['data']['strings']}
assert d['data']['count']==len(s)>200
assert s[sys.argv[2]]['translation']=='XX: nothing else to remove', s[sys.argv[2]]
assert s[sys.argv[3]]['placeholders']==1
assert s[sys.argv[4]]['translation'] is None
" "$TEMP/export.json" "$K_NONE" "$K_PLAT" "$K_CANCEL"
fi

# --- import: validate, merge, keep the other languages ---------------------------------------
addlang de "$K_NONE" "DE: nichts zu entfernen" "$K_PLAT" "DE: kein lib/%s" "bogus.key" "x" "$K_CANCEL" "no placeholder needed"
expect "import succeeds with the valid lines" test "$RC" -eq 0
expect "import reports the unknown key" grep -q "bogus.key: unknown key" <<<"$OUT"
addlang de "$K_PLAT" "DE: zu viele %s und %s"
expect "import rejects a wrong placeholder count" grep -q "placeholder" <<<"$OUT"
expect "a rejected line keeps the previous translation" test "$(cell de "$K_PLAT")" = "DE: kein lib/%s"
expect "bogus keys never reach the file" bash -c '! grep -q "bogus.key" "$1"' _ "$CAT"
PLATFORM=osx run cleanup libs other --lang de
expect "the imported language works" grep -q "DE: nichts zu entfernen" <<<"$OUT"
addlang de "$K_CANCEL" "DE: abgebrochen"
expect "importing more lines keeps the earlier ones (merge)" test "$(cell de "$K_NONE")" = "DE: nichts zu entfernen" -a "$(cell de "$K_CANCEL")" = "DE: abgebrochen"
addlang de "$K_NONE" "DE: nichts mehr"
expect "re-importing a key replaces it" test "$(cell de "$K_NONE")" = "DE: nichts mehr"
expect "other languages untouched by a later import" test "$(cell xx "$K_NONE")" = "XX: nothing else to remove" -a "$(cell zh "$K_NONE")" = "没有其他平台可删除"
expect "each language appears once in the header" test "$(head -1 "$CAT" | tr '\t' '\n' | sort | uniq -d | head -1)" = ""
expect "English column untouched" test "$(cut -f1,2 "$CAT")" = "$(cut -f1,2 "$TEMP/strings.orig.tsv")"
printf 'bogus.key\tx\n' > "$TEMP/bad.tsv"
run i18n import fr "$TEMP/bad.tsv"
expect "importing only invalid lines fails" test "$RC" -ne 0
expect "no column is added for it" bash -c '! head -1 "$1" | grep -q "fr"' _ "$CAT"
run i18n import en "$TEMP/in.tsv"
expect "en cannot be overwritten" test "$RC" -ne 0
run i18n import "../x" "$TEMP/in.tsv"
expect "a path-like language name is refused" test "$RC" -ne 0
run i18n import de "$TEMP/does-not-exist.tsv"
expect "a missing file is refused" test "$RC" -ne 0
printf '%s\t%s\n' "$K_NONE" "ES: nada" | "$BASH" "$TREE/scripts/of.sh" i18n import es - >/dev/null 2>&1
expect "import from stdin adds a column" test "$(cell es "$K_NONE")" = "ES: nada"
expect "the file keeps LF line endings" bash -c '! grep -q "$(printf "\r")" "$1"' _ "$CAT"

if command -v python3 >/dev/null 2>&1; then
	cp "$CAT" "$TEMP/before-extract.tsv"
	expect "the extractor runs on a file with several languages" python3 "$TREE/scripts/dev/i18n_extract.py"
	expect "the extractor kept every language column" cmp -s "$CAT" "$TEMP/before-extract.tsv"
	expect "strings.tsv with imported languages is still in sync" python3 "$TREE/scripts/dev/i18n_extract.py" --check
fi

# --- check ---------------------------------------------------------------------------------
run i18n check de
expect "check: a valid language passes" test "$RC" -eq 0
expect "check: reports the missing strings" grep -q "missing" <<<"$OUT"
# put a bad placeholder straight into the file
awk -F'\t' -v OFS='\t' -v k="$K_PLAT" 'NR==1 { for (i = 2; i <= NF; i++) if ($i == "yy") c = i } NR>1 && $1==k { $c = "%d" } { print }' "$CAT" > "$TEMP/hand.tsv" && cp "$TEMP/hand.tsv" "$CAT"
run i18n check yy
expect "check: a bad placeholder fails" test "$RC" -ne 0
run i18n check nosuchlang
expect "check: a language with no column has nothing valid" grep -q "0 valid" <<<"$OUT"

# --- a Windows checkout may give the file CRLF line endings -------------------------------
sed 's/$/\r/' "$TEMP/strings.orig.tsv" > "$CAT"
addlang xx "$K_NONE" "XX: crlf works"
expect "import into a CRLF file" test "$(cell xx "$K_NONE")" = "XX: crlf works"
sed 's/$/\r/' "$CAT" > "$TEMP/crlf.tsv" && cp "$TEMP/crlf.tsv" "$CAT"
PLATFORM=osx run cleanup libs other --lang xx
expect "a CRLF catalogue still loads" grep -q "XX: crlf works" <<<"$OUT"
"$BASH" "$TREE/scripts/of.sh" i18n export </dev/null 2>/dev/null > "$TEMP/export-crlf.tsv"
expect "export from a CRLF catalogue has no CR" bash -c '! grep -q "$(printf "\r")" "$1"' _ "$TEMP/export-crlf.tsv"

echo "i18n: $((CHECKS - FAILS))/${CHECKS} checks passed"
[[ $FAILS -eq 0 ]]
