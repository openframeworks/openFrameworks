# of menu translations

Every user-facing string in `scripts/of.sh` and `scripts/ui.sh` (menu items, titles,
headings, prompts, messages) is written as

```bash
$(t some.key "English text with %s placeholders" arg ...)
```

The English text lives in the code, so the CLI still works if this folder is missing.

## One file, one column per language

All languages live in **`strings.tsv`**: a header row and one row per string.

```
key	en	de	zh
menu.back	Back	Zurück	返回
msg.no_other_platforms_to_remove	no other platforms to remove	…	…
```

- Column 1 is the key, column 2 is the English text (generated from the code), every
  further column is a language. Tab-separated; no tabs or newlines inside a text.
- An empty cell falls back to English, so a language can be translated gradually.
- Keep the same number of `%s` placeholders as the English text (their position in
  the sentence may move). `%%` is a literal percent sign.

After changing a string in the code, update the English column (CI runs `--check`):

```sh
python3 scripts/dev/i18n_extract.py          # rewrite key + en; every language column is kept
python3 scripts/dev/i18n_extract.py --check
```

## Add or update a language

```sh
./of i18n export                 # key<TAB>English, to translate by hand or send to a translation API
./of i18n export --json          # the same, structured: key, text, placeholders
./of i18n import de de.tsv       # key<TAB>text lines -> adds / updates the "de" column
./of i18n check de               # what is missing or invalid
./of i18n list                   # languages and how much of the catalogue each covers
```

`import` validates every line (known key, same placeholders), merges into the file
(keys not in the import keep their translation), and leaves the other languages alone.

Use a language with `./of --lang de ...` or `OF_LANG=de ./of ...`.

`--json` output keeps its field names and error `code`s in English; only human text and
the error `message` follow `--lang`.
