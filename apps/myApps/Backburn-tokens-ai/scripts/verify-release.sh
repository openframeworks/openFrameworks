#!/bin/bash
# verify-release.sh — fail the release if the shipped binary leaks what hardening should hide.
# Usage:
#   scripts/verify-release.sh bin/Backburn-tokens-ai.app            # macOS .app
#   scripts/verify-release.sh bin/Backburn-tokens-ai.app Backburn.dSYM
#   scripts/verify-release.sh path/to/MyApp                          # bare executable (iOS build product)
# Exit 0 = clean. Warnings do not fail; FAIL lines do. Run AFTER Archive, BEFORE upload.
set -u
APP="${1:?usage: verify-release.sh <App.app|executable> [dSYM]}"
DSYM="${2:-}"
FAIL=0
say()  { printf '%s\n' "$*"; }
fail() { say "FAIL: $*"; FAIL=1; }
warn() { say "WARN: $*"; }
ok()   { say "  ok: $*"; }

# Resolve executable
if [ -d "$APP" ]; then
  NAME="$(basename "$APP" .app)"
  BIN="$APP/Contents/MacOS/$NAME"
  [ -f "$BIN" ] || BIN="$(find "$APP/Contents/MacOS" -type f -perm -111 2>/dev/null | head -1)"
  PLIST="$APP/Contents/Info.plist"
else
  BIN="$APP"; PLIST=""
fi
[ -f "$BIN" ] || { say "FAIL: executable not found: $BIN"; exit 1; }
say "== $BIN"
file "$BIN" | sed 's/^/  /'

# 1. Stripped: no local symbol table worth reading
if nm "$BIN" 2>/dev/null | grep -q .; then
  N=$(nm "$BIN" 2>/dev/null | wc -l | tr -d ' ')
  # A stripped Mach-O still shows global/dynamic symbols (T _main, U imports); only local 't' is the leak.
  if nm "$BIN" 2>/dev/null | grep -qE ' t '; then
    fail "local symbols present ($N nm lines) — STRIP_STYLE=all / strip -x not applied"
    nm "$BIN" 2>/dev/null | grep -E ' t ' | head -5 | sed 's/^/    /'
  else ok "no local (t) symbols ($N global/dynamic lines remain, as required by the loader)"; fi
else ok "nm: no symbols"; fi

# 2. No dSYM / debug data inside the bundle
if [ -d "$APP" ]; then
  find "$APP" -name "*.dSYM" -o -name "*.dwarf" 2>/dev/null | grep -q . && fail "dSYM/dwarf inside .app — keep dSYMs outside the bundle" || ok "no dSYM inside bundle"
fi
[ -n "$DSYM" ] && { [ -d "$DSYM" ] && ok "dSYM kept separately: $DSYM" || warn "dSYM path given but missing: $DSYM"; }

# 3. Plaintext leaks: build paths, endpoints, key-shaped strings
STR="$(strings -a "$BIN" 2>/dev/null)"
echo "$STR" | grep -q "/Users/" && { fail "home path leaked (/Users/...) — check -ffile-prefix-map / __FILE__ logging"; echo "$STR" | grep "/Users/" | head -3 | sed 's/^/    /'; } || ok "no /Users/ paths"
echo "$STR" | grep -qE "$HOME|SRCROOT" && warn "SRCROOT/HOME string present — grep the binary manually" || true
SECRET_FAIL=0
for pat in 'sk-ant-[A-Za-z0-9_-]\{20,\}' 'sk-admin-[A-Za-z0-9_-]\{20,\}' 'Bearer [A-Za-z0-9_-]\{20,\}'; do
  if echo "$STR" | grep -qE "$pat"; then fail "secret-shaped string in binary: $pat"; SECRET_FAIL=1; fi
done
[ $SECRET_FAIL -eq 0 ] && ok "no secret-shaped strings (sk-ant-*/sk-admin-*/Bearer token)"
# Endpoints protected with OBFSTR should not appear. UI/help URLs are fine — list, don't fail.
HITS="$(echo "$STR" | grep -E 'https://api\.|https://management-api\.' || true)"
[ -n "$HITS" ] && { warn "API endpoints still plaintext (wrap with OBFSTR in ProviderApi.cpp):"; echo "$HITS" | head -5 | sed 's/^/    /'; } || ok "no plaintext API endpoints"

# 4. Rpaths / linked libs: no build-machine paths, no unexpected dylibs
otool -L "$BIN" 2>/dev/null | grep -q "/Users/" && fail "otool -L shows a /Users/ dylib path" || ok "no /Users/ dylib paths"
otool -l "$BIN" 2>/dev/null | grep -A1 LC_RPATH | grep -q "/Users/" && fail "LC_RPATH leaks a build path" || true

# 5. Signing (macOS bundles only; iOS products are signed by Xcode/Archive)
if [ -d "$APP" ] && [ -n "$PLIST" ]; then
  codesign --verify --deep --strict "$APP" 2>&1 | grep -q . && fail "codesign --verify: $(codesign --verify --deep --strict "$APP" 2>&1 | head -1)" || ok "codesign --verify --deep --strict"
  FLAGS="$(codesign -dv --verbose=4 "$APP" 2>&1)"
  echo "$FLAGS" | grep -q "flags=.*runtime" && ok "hardened runtime (flags contain 'runtime')" || fail "hardened runtime OFF — set ENABLE_HARDENED_RUNTIME=YES"
  say "  entitlements:"; codesign -d --entitlements - "$APP" 2>/dev/null | sed 's/^/    /' | head -20
  spctl -a -t exec -vv "$APP" 2>&1 | sed 's/^/  spctl: /' || true
fi

# 6. Debug artefacts
echo "$STR" | grep -q "__debug" && warn "__debug sections/strings present" || true
[ -n "$PLIST" ] && [ -f "$PLIST" ] && { /usr/libexec/PlistBuddy -c 'Print :CFBundleIdentifier' "$PLIST" 2>/dev/null | sed 's/^/  bundle id: /'; }

say "== $([ $FAIL -eq 0 ] && echo PASS || echo FAIL)"
exit $FAIL
