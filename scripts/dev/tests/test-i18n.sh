#!/usr/bin/env bash
# Tests for the string dictionary: scripts/i18n/en.tsv is in sync with the code,
# --lang / OF_LANG pick a translation, placeholders are filled, missing keys and
# languages fall back to English, and `of i18n` exports / imports / checks.
# Runs against a throwaway copy of the scripts.
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
EN="$TREE/scripts/i18n/en.tsv"
[[ -f "$EN" ]] || { echo "FAIL: scripts/i18n/en.tsv is missing" >&2; exit 1; }

run(){ OUT="$("$BASH" "$TREE/scripts/of.sh" "$@" </dev/null 2>&1)"; RC=$?; OUT="$(printf '%s' "$OUT" | sed $'s/\x1b\\[[0-9;?]*[a-zA-Z]//g')"; }
key_for(){ awk -F'\t' -v t="$1" '$2==t{print $1; exit}' "$EN"; }

# --- catalogue ------------------------------------------------------------------
if command -v python3 >/dev/null 2>&1; then
	expect "en.tsv is in sync with the code (python3 scripts/dev/i18n_extract.py)" python3 "$ROOT/scripts/dev/i18n_extract.py" --check
fi
expect "every catalogue line is key<TAB>text" test -z "$(awk -F'\t' 'NF!=2 || $1=="" || $2==""' "$EN" | head -1)"
expect "catalogue keys are unique" test -z "$(cut -f1 "$EN" | sort | uniq -d | head -1)"
expect "catalogue has a few hundred strings" test "$(wc -l < "$EN" | tr -d ' ')" -gt 200

K_NONE="$(key_for 'no other platforms to remove')"
K_PLAT="$(key_for 'no lib/%s folders to remove')"
expect "known keys exist" test -n "$K_NONE" -a -n "$K_PLAT"

# --- selecting a language ---------------------------------------------------------
mkdir -p "$TREE/libs/zlib/lib/macos"
: > "$TREE/libs/zlib/lib/macos/z.a"
printf '%s\t%s\n' "$K_NONE" "XX: nothing else to remove" "$K_PLAT" "XX: no lib/%s here" > "$TREE/scripts/i18n/xx.tsv"
printf '# a comment\n' >> "$TREE/scripts/i18n/xx.tsv"

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
PLATFORM=osx run cleanup libs other --lang xx
expect "untranslated keys fall back to English" grep -q "keep" <<<"$OUT"

PLATFORM=osx run cleanup libs other --lang nosuchlang
expect "unknown language: warns" grep -q "nosuchlang" <<<"$OUT"
expect "unknown language: falls back to English" grep -q "no other platforms to remove" <<<"$OUT"
run cleanup libs other --lang '../../etc/passwd'
expect "a path-like language name is not loaded" bash -c '! grep -q "root:" <<<"$1"' _ "$OUT"

# a translation can reorder words around the placeholder
printf '%s\t%s\n' "$K_PLAT" "%s :XX" > "$TREE/scripts/i18n/yy.tsv"
run cleanup libs linux64 --lang yy
expect "placeholder position can move" grep -q "linux64 :XX" <<<"$OUT"

# non-Latin text
printf '%s\t%s\n' "$K_NONE" "没有其他平台可删除" > "$TREE/scripts/i18n/zh.tsv"
PLATFORM=osx run cleanup libs other --lang zh
expect "Chinese translation is shown" grep -q "没有其他平台可删除" <<<"$OUT"

# --json: the language applies to messages inside the envelope too
run cleanup libs bogus --yes --json --lang xx
expect "--json and --lang together: still one envelope" bash -c 'echo "$1" | head -1 >/dev/null; true' _ "$OUT"

# --- of i18n ------------------------------------------------------------------------
run i18n list
expect "i18n list shows the catalogue size and the languages" bash -c 'grep -q "strings" <<<"$1" && grep -q "xx" <<<"$1" && grep -q "zh" <<<"$1"' _ "$OUT"
"$BASH" "$TREE/scripts/of.sh" i18n export </dev/null 2>/dev/null > "$TEMP/export.tsv"
expect "i18n export (English): identical to en.tsv" cmp -s "$TEMP/export.tsv" "$EN"
"$BASH" "$TREE/scripts/of.sh" i18n export xx </dev/null 2>/dev/null > "$TEMP/export-xx.tsv"
expect "i18n export xx has three columns and the translation" test -n "$(awk -F'\t' -v k="$K_NONE" '$1==k && NF==3 && $3=="XX: nothing else to remove"' "$TEMP/export-xx.tsv")"

if command -v python3 >/dev/null 2>&1; then
	"$BASH" "$TREE/scripts/of.sh" i18n export xx --json </dev/null 2>/dev/null > "$TEMP/export.json"
	expect "i18n export --json: envelope with strings and the translation" python3 -c "
import json,sys
d=json.load(open(sys.argv[1],encoding='utf-8'))
assert d['ok'] and d['data']['source_lang']=='en' and d['data']['lang']=='xx'
s={i['key']:i for i in d['data']['strings']}
assert d['data']['count']==len(s)>200
k=sys.argv[2]
assert s[k]['translation']=='XX: nothing else to remove', s[k]
assert s[sys.argv[3]]['placeholders']==1
" "$TEMP/export.json" "$K_NONE" "$K_PLAT"
fi

# import: validates keys and placeholders, then installs
printf '%s\t%s\n' "$K_NONE" "DE: nichts zu entfernen" "$K_PLAT" "DE: kein lib/%s" "bogus.key" "x" "$(key_for 'cancelled — nothing deleted')" "no placeholder needed" > "$TEMP/de.tsv"
printf '%s\t%s\n' "$K_PLAT" "DE: zu viele %s und %s" >> "$TEMP/de.tsv"
run i18n import de "$TEMP/de.tsv"
expect "import succeeds with the valid lines" test "$RC" -eq 0
expect "import reports the unknown key" grep -q "bogus.key: unknown key" <<<"$OUT"
expect "import rejects a wrong placeholder count" grep -q "placeholder" <<<"$OUT"
expect "import wrote scripts/i18n/de.tsv" test -f "$TREE/scripts/i18n/de.tsv"
expect "the imported file only holds valid lines" bash -c '! grep -q "bogus.key" "$1" && ! grep -q "zu viele" "$1"' _ "$TREE/scripts/i18n/de.tsv"
PLATFORM=osx run cleanup libs other --lang de
expect "the imported language works" grep -q "DE: nichts zu entfernen" <<<"$OUT"
printf 'bogus.key\tx\n' > "$TEMP/bad.tsv"
run i18n import fr "$TEMP/bad.tsv"
expect "importing only invalid lines fails" test "$RC" -ne 0
expect "nothing is installed for it" test ! -e "$TREE/scripts/i18n/fr.tsv"
run i18n import en "$TEMP/de.tsv"
expect "en cannot be overwritten" test "$RC" -ne 0
run i18n import "../x" "$TEMP/de.tsv"
expect "a path-like language name is refused" test "$RC" -ne 0
printf '%s\t%s\n' "$K_NONE" "ES: nada" | "$BASH" "$TREE/scripts/of.sh" i18n import es - >/dev/null 2>&1
expect "import from stdin" test -f "$TREE/scripts/i18n/es.tsv"
run i18n check de
expect "check: a complete-enough language passes" test "$RC" -eq 0
printf '%s\t%s\n' "$K_PLAT" "%d" > "$TREE/scripts/i18n/bad.tsv"
run i18n check bad
expect "check: a bad placeholder fails" test "$RC" -ne 0

echo "i18n: $((CHECKS - FAILS))/${CHECKS} checks passed"
[[ $FAILS -eq 0 ]]
