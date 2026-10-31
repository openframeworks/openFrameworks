# Protecting an openFrameworks app from decompiling — Release strategy

**Reality check first.** A native C++/ObjC app cannot be made un-decompilable.
Anyone with the `.app` can run `strings`, `nm`, `otool`, Hopper/Ghidra/IDA and
recover logic, URLs and assets. What you *can* do — and what this setup does —
is remove the free wins (symbol table, plaintext endpoints, build paths, debug
info) so recovery takes real effort, and keep secrets out of the binary entirely.
That is the App-Store-safe ceiling. Anything stronger (LLVM obfuscators,
packers, anti-debug traps) risks App Review rejection, notarization failure,
crashes under the hardened runtime, and is usually reversed anyway.

Your current shipped binary (audited 2026-10-31) fails on every free win:
76,950 `nm` lines, `/Users/runner/...` Poco build paths, and plaintext
`https://api.anthropic.com/...`, `https://api.openai.com/...`,
`https://management-api.x.ai/...`. Run `scripts/verify-release.sh` on it to see.

---

## 1. What was built for you

| File | What it does |
| --- | --- |
| [Obfuscate.h](include/Obfuscate.h) | Header-only C++ obfuscation: compile-time XOR-encrypted strings (`OBFSTR`/`OBF_GUARD`), consteval hashes (`OBF_HASH`), hidden ints (`OBF_INT`), opaque predicates (`OBF_TRUE`/`OBF_FALSE`), secure wipe. No-ops to plain literals in Debug. |
| [Hardened.xcconfig](Hardened.xcconfig) | Release-only Xcode settings: `-Os` + full LTO, hidden visibility, strip-all, dead-strip, `-ffile-prefix-map` (kills `/Users/...` leaks), `NDEBUG` + `BACKBURN_HARDENED=1`. |
| [macos/AppStore.xcconfig](macos/AppStore.xcconfig) | Mac App Store: hardened + sandbox entitlements + hardened runtime. |
| [macos/DeveloperID.xcconfig](macos/DeveloperID.xcconfig) | Developer ID: hardened + hardened runtime, no sandbox (for notarized direct download). |
| [ios/AppStore.xcconfig](ios/AppStore.xcconfig) | iOS/tvOS App Store viewer: hardened, deployment 15.0. |
| [../scripts/verify-release.sh](../scripts/verify-release.sh) | Fails the release if symbols, paths, endpoints or secrets leak, or hardened runtime is off. |
| [../scripts/post-build-strip.sh](../scripts/post-build-strip.sh) | Optional Run Script phase: `strip -x -S` + re-sign for plain Release builds. |
| `core/ProviderApi.cpp` (patched) | The three provider API endpoints + key pages now go through `OBFSTR(...)`. Signature change: `keyPage()` returns `std::string`. |

Already correct in this repo — keep it: API keys live in the Keychain
(`core/SecretsMac.mm`), never in `settings.json` or the binary. That single
decision matters more than all obfuscation below.

## 2. Xcode project settings (the 80%)

Set the **Release** base configuration to the store wrapper, not Debug:

- Xcode: Project → Info → Configurations → Release → pick
  `release/macos/AppStore.xcconfig` (or DeveloperID / iOS wrapper).
- Command line (as in `release/README.md`):
  `xcodebuild -project Backburn-tokens-ai.xcodeproj -target Backburn-tokens-aimacOS -configuration Release -xcconfig release/macos/AppStore.xcconfig archive`

Key settings (all in `Hardened.xcconfig`, Release-only so Debug is untouched):

| Setting | Value | Why |
| --- | --- | --- |
| `GCC_OPTIMIZATION_LEVEL` | `s` (`-Os`) | Inlining + size; boundaries blur vs `-O0` |
| `LLVM_LTO` | `YES` (full) | Cross-file inlining, internalises helpers. `YES_THIN` if link is too slow |
| `STRIP_INSTALLED_PRODUCT` + `STRIP_STYLE` | `YES` / `all` | Ships no symbol table |
| `DEBUG_INFORMATION_FORMAT` | `dwarf-with-dsym` | You keep a dSYM (outside the `.app`) for crash logs |
| `DEAD_CODE_STRIPPING` + `-Wl,-dead_strip -Wl,-x` | `YES` | Unused OF/addon code never ships (linker flags via `-Wl,` — a bare `-x` breaks the clang driver) |
| `GCC_SYMBOLS_PRIVATE_EXTERN` + `-fvisibility=hidden` | `YES` | Only `main`/exported symbols visible |
| `-ffile-prefix-map=$(SRCROOT)=.` (+ `OF_PATH`) | on | Erases `/Users/you/...` from `__FILE__`/assert strings |
| `ENABLE_HARDENED_RUNTIME` (macOS wrappers) | `YES` | Required for notarization; blocks injection |
| `VALIDATE_PRODUCT` | `YES` | Xcode checks bundle/signing before upload |

**Rebuild openFrameworksLib with the same Release config.** Hardening only the
app target leaves the OF static lib's symbols and strings (Poco paths above)
linked straight back in. Your "Compile openFrameworks" script phase already
forwards `${CONFIGURATION}` and defines — build the Archive and it follows.

openFrameworks specifics:

- `bin/data/` ships **in the clear** inside the bundle. Anything sensitive
  there (shaders with proprietary logic, JSON price tables, prompts) is one
  `unzip` away. Encrypt assets that matter (AES-GCM, key split/obfuscated —
  §3) and decrypt to memory; never write the plaintext back to disk.
- GLSL shaders compile to readable source in the binary/bundle. If a shader is
  the product, ship it encrypted with `OBF_GUARD` and pass the decrypted source
  to `ofShader::setupShaderFromSource` at runtime.
- `ofLog`/asserts leak file paths and logic in Release: `NDEBUG` + prefix-map
  handles most; audit `ofLogNotice` calls that print URLs/tokens.

## 3. C++ obfuscation in code (`Obfuscate.h`)

```cpp
#include "Obfuscate.h"   // HEADER_SEARCH_PATHS already includes release/include

// 1. Endpoints / paths / header values — plaintext never reaches the binary
std::string url = OBFSTR("https://api.example.com/v1/") + id;
auto g = OBF_GUARD("/prepaid/balance");  callApi(g.c_str());  // wiped on scope exit

// 2. String dispatch without literals to grep for
switch (obf::hash(provider)) {
  case OBF_HASH("claude"): /* ... */ break;
  case OBF_HASH("codex"):  /* ... */ break;
}

// 3. Constants a scanner would look for
int port = OBF_INT(8790);

// 4. Guard the sensitive branch so -Os/LTO cannot prune it to a straight line
if (OBF_TRUE && keyIsValid(k)) { grantProFeatures(); }
obf::wipe(keyCopy);  // erase plaintext after use
```

Rules:

- Protect **secrets-adjacent** strings only: API endpoints, internal URLs,
  file paths, license/feature strings, shader source. Obfuscating every UI
  label slows the build and helps nothing — UI text is visible on screen anyway.
- Never put a real API key, signing secret or license private key in code in
  any form, obfuscated or not. User keys → Keychain (done). Your keys → there
  are none that are safe client-side; if a service needs *your* key, proxy it
  through your own server. `OBFSTR` hides strings from `strings(1)`, not from
  a debugger watching the decrypt.
- Mark internal helpers `OBF_HIDDEN` (or rely on `-fvisibility=hidden`) and
  prefer anonymous namespaces — fewer symbols survive even before stripping.

## 4. Strategies ranked (do → don't)

**Do:** strip + dSYM separation; LTO + hidden visibility; encrypted strings for
endpoints; Keychain for keys; asset encryption for valuable `bin/data`;
hardened runtime + notarization; server-side checks for anything paid
(license validation, feature flags — a patched binary defeats any client-only
check, so the server must be the authority); App Store receipt validation if
you sell features.

**Don't (App Store / notarization risk, low payoff):**

- **OLLVM / Hikari / `-mllvm -fla -bcf -sub`** — control-flow flattening passes
  trip App Review's static analysis, break with LTO + hardened runtime, and
  IDA deobfuscators undo them. Not used here, deliberately.
- **Anti-debug (`PT_DENY_ATTACH`, `ptrace` traps), jailbreak checks as a gate,
  self-modifying code, packers/encrypt-then-execute loaders** — Guideline 2.3.1
  (hidden/obfuscated functionality) and 5.2.3 risk, plus crash reports you
  cannot symbolicate. If you add tamper detection, make it *report-only*
  (phone home, degrade a feature), never crash or refuse to launch.
- **Bitcode-era advice** (`ENABLE_BITCODE`) — deprecated since Xcode 14; leave it out.

## 5. Release checklist

1. Archive with the store xcconfig (same config for OF lib + app).
2. `scripts/verify-release.sh path/to/Your.app path/to/Your.dSYM` — must print `PASS`.
   It checks: no local symbols, no `/Users/` paths, no plaintext API endpoints,
   no secret-shaped strings, no dSYM in the bundle, hardened runtime on,
   `codesign --verify --deep --strict` clean.
3. Spot-check yourself: `strings Your.app/Contents/MacOS/YourApp | grep -E 'api\.|/Users/'`
   and `nm Your.app/Contents/MacOS/YourApp | head` (expect almost nothing).
4. Keep the Archive's dSYM somewhere versioned — without it, crash logs from
   users are unreadable. Never ship it in the bundle.
5. Developer ID only: `xcrun notarytool submit --wait` + `xcrun stapler staple`,
   then `spctl -a -vv` (see `release/README.md`).

## 6. Limits, honestly

After all of this, a skilled reverse engineer with Ghidra can still recover
your algorithms — obfuscation buys time and stops casual copying/`strings`
archaeology, nothing more. Budget accordingly: spend on the server-side
authority and on shipping, not on stronger client obfuscation. If a specific
algorithm *is* the business, run it server-side; that is the only real fix.
