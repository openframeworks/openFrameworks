#!/usr/bin/env python3
"""Keep the English column of scripts/i18n/strings.tsv in step with the code.

    python3 scripts/dev/i18n_extract.py            # update strings.tsv
    python3 scripts/dev/i18n_extract.py --check    # exit 1 if it is out of date

Every user-facing string in scripts/of.sh and scripts/ui.sh is written
    $(t some.key "English text with %s placeholders" arg ...)
strings.tsv is the single dictionary file: a header `key<TAB>en<TAB>de<TAB>...`
and one row per string, one column per language. This tool rewrites the key and
`en` columns from the code and keeps every other language column as it is
(new strings get empty cells, removed strings lose their row). Empty cells fall
back to English at runtime.
"""
import os, re, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
SOURCES = ['scripts/of.sh', 'scripts/ui.sh']
OUT = os.path.join(ROOT, 'scripts', 'i18n', 'strings.tsv')
CALL = re.compile(r'\$\(t ([A-Za-z0-9_.]+) "((?:[^"\\]|\\.)*)"')

def unescape(text):
    # what the shell turns the double-quoted text into
    return re.sub(r'\\([\"$`\\])', r'\1', text)

def extract():
    found = {}
    for rel in SOURCES:
        with open(os.path.join(ROOT, rel), encoding='utf-8') as f:
            src = f.read()
        for m in CALL.finditer(src):
            key, text = m.group(1), unescape(m.group(2))
            if key in found and found[key] != text:
                sys.exit(f'{rel}: key {key!r} used with two different texts:\n  {found[key]!r}\n  {text!r}')
            found[key] = text
    for key, text in found.items():
        if '\t' in text or '\n' in text:
            sys.exit(f'key {key!r}: tabs and newlines are not allowed in catalogue text')
    return found

def read_existing():
    """-> (languages other than en, {key: {lang: text}})"""
    langs, rows = [], {}
    try:
        with open(OUT, encoding='utf-8', newline='') as f:
            lines = f.read().replace('\r\n', '\n').split('\n')
    except FileNotFoundError:
        return langs, rows
    header = lines[0].split('\t') if lines and lines[0] else ['key', 'en']
    langs = [l for l in header[1:] if l != 'en']
    for line in lines[1:]:
        if not line:
            continue
        cells = line.split('\t')
        row = dict(zip(header[1:], cells[1:]))
        rows[cells[0]] = row
    return langs, rows

def render(found, langs, rows):
    out = ['\t'.join(['key', 'en'] + langs)]
    for key in sorted(found):
        old = rows.get(key, {})
        out.append('\t'.join([key, found[key]] + [old.get(l, '') for l in langs]))
    return '\n'.join(out) + '\n'

def main():
    found = extract()
    langs, rows = read_existing()
    body = render(found, langs, rows)
    if '--check' in sys.argv:
        try:
            current = open(OUT, encoding='utf-8', newline='').read().replace('\r\n', '\n')
        except FileNotFoundError:
            current = ''
        if current != body:
            print('scripts/i18n/strings.tsv is out of date: run python3 scripts/dev/i18n_extract.py', file=sys.stderr)
            sys.exit(1)
        print(f'strings.tsv is up to date ({len(found)} strings, languages: en{"".join(", " + l for l in langs)})')
        return
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, 'w', encoding='utf-8', newline='\n') as f:
        f.write(body)
    print(f'wrote {OUT} ({len(found)} strings, languages: en{"".join(", " + l for l in langs)})')

main()
