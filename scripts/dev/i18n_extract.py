#!/usr/bin/env python3
"""Build scripts/i18n/en.tsv, the English catalogue, from the t calls in the scripts.

    python3 scripts/dev/i18n_extract.py            # rewrite scripts/i18n/en.tsv
    python3 scripts/dev/i18n_extract.py --check    # exit 1 if en.tsv is out of date

Every user-facing string in scripts/of.sh and scripts/ui.sh is written
    $(t some.key "English text with %s placeholders" arg ...)
so a translator (a person, or a translation API) only needs en.tsv:
one `key<TAB>text` per line. A translation is the same file with the text
replaced, saved as scripts/i18n/<lang>.tsv (see `of i18n import`).
"""
import os, re, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
SOURCES = ['scripts/of.sh', 'scripts/ui.sh']
OUT = os.path.join(ROOT, 'scripts', 'i18n', 'en.tsv')
CALL = re.compile(r'\$\(t ([A-Za-z0-9_.]+) "((?:[^"\\]|\\.)*)"')

def unescape(text):
    # what the shell turns the double-quoted text into
    return re.sub(r'\\([\"$`\\])', r'\1', text)

def extract():
    found = {}
    for rel in SOURCES:
        path = os.path.join(ROOT, rel)
        with open(path, encoding='utf-8') as f:
            src = f.read()
        for m in CALL.finditer(src):
            key, text = m.group(1), unescape(m.group(2))
            if key in found and found[key] != text:
                sys.exit(f'{rel}: key {key!r} used with two different texts:\n  {found[key]!r}\n  {text!r}')
            found[key] = text
    return found

def render(found):
    lines = []
    for key in sorted(found):
        text = found[key]
        if '\t' in text or '\n' in text:
            sys.exit(f'key {key!r}: tabs and newlines are not allowed in catalogue text')
        lines.append(f'{key}\t{text}')
    return '\n'.join(lines) + '\n'

def main():
    body = render(extract())
    if '--check' in sys.argv:
        try:
            current = open(OUT, encoding='utf-8').read()
        except FileNotFoundError:
            current = ''
        if current != body:
            print('scripts/i18n/en.tsv is out of date: run python3 scripts/dev/i18n_extract.py', file=sys.stderr)
            sys.exit(1)
        print(f'en.tsv is up to date ({body.count(chr(10))} strings)')
        return
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, 'w', encoding='utf-8', newline='\n') as f:
        f.write(body)
    print(f'wrote {OUT} ({body.count(chr(10))} strings)')

main()
