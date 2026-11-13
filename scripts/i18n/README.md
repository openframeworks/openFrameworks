# of menu translations

Every user-facing string in `scripts/of.sh` and `scripts/ui.sh` (menu items, titles,
headings, prompts, messages) is written as

```bash
$(t some.key "English text with %s placeholders" arg ...)
```

The English text lives in the code, so the CLI still works if this folder is missing.
`en.tsv` is generated from it:

```sh
python3 scripts/dev/i18n_extract.py          # rewrite en.tsv after changing a string
python3 scripts/dev/i18n_extract.py --check  # CI: fails when en.tsv is out of date
```

## Add a language

A language is `scripts/i18n/<lang>.tsv`: one `key<TAB>translated text` per line.
Keep the same number of `%s` placeholders as the English text (their order in the sentence
may change). Keys you leave out fall back to English.

```sh
./of i18n export                 # key<TAB>English, to translate by hand or send to a translation API
./of i18n export --json          # the same, structured: key, text, placeholders
./of i18n import de de.tsv       # validate (unknown keys, placeholder counts) and install de.tsv
./of i18n check de               # what is missing or invalid
./of i18n list                   # languages and how much of the catalogue each covers
```

Use it with `./of --lang de ...` or `OF_LANG=de ./of ...`.

Notes
- `--json` output keeps its field names and error `code`s in English; only human text and
  the error `message` follow `--lang`.
- Tabs and newlines are not allowed in catalogue text. `%%` is a literal percent sign.
