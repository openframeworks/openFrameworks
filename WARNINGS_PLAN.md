# Fix MSVC Compiler Warnings — openFrameworks (Windows/VS build)

**Branch:** `danzeeeman/warnings`

## Context

We stood up a from-scratch Windows/MSVC build of openFrameworks: downloaded the VS libs (`scripts/vs/download_libs.sh`), generated VS project files for all 221 examples via `projectGeneratorCmd.exe -p vs`, and built every desktop-targetable example (160 of 221 — the rest are `android`/`ios` examples, out of scope for a Windows build) in `Release|x64`. 154/160 build successfully; the other 6 fail for reasons unrelated to compiler warnings (2 tvOS examples, 1 EGL-only example, 1 real missing `ofxGui` template instantiation, and 2 OpenCV examples missing the vendored `highgui` module) — **those 6 are out of scope for this warnings pass.**

The goal is a clean, warning-free Windows build: fix the warnings currently emitted, without changing runtime behavior, and without enabling `/WX` (warnings-as-errors) afterward — this is a cleanup pass, not a policy change.

**Scope:** all MSVC warnings in first-party code — core engine (`libs/openFrameworks*`), addons used by the examples (`addons/ofx*`), and example sample code (`examples/**/src`). **Vendored third-party sources bundled inside addons** (e.g. `addons/ofxKinect/libs/libfreenect`, `addons/ofxOsc/libs/oscpack`) **are out of scope** — those aren't ours to modify and any warning fix there would need to go upstream instead.

**Organization:** work directory/module by directory/module (core lib first, then each addon, then examples), fixing whatever warning codes appear in that module as encountered.

**No `/WX`** — leave warnings-as-errors off; this is a cleanup pass, not a policy change.

## Complete warning inventory (from the full 154-example build)

Warnings compiled once in the core lib and once per addon (not duplicated per-example, since examples were built with `-p:BuildProjectReferences=false` against the already-built core lib and addon libs). Header-only code (`.inl`/`.h`) still triggers its warning in every example TU that includes it — those are marked "recurs".

| Code | Count* | Location | Root cause |
|---|---|---|---|
| C4267 | ~530 (recurs) | `libs/openFrameworks/3d/ofMesh.inl` (many sites, e.g. line 1757, 1759, 1761, 1667–1673) | `size_t` → `ofIndexType`/`int` narrowing in mesh vertex/face indexing |
| C4267 | recurs | `libs/openFrameworks/graphics/ofPolyline.inl:1124` (`return`) | `size_t` → `int` narrowing |
| C4267 | recurs | `addons/ofxGui/src/ofxSliderGroup.cpp:25` | `size_t` → `glm::vec<N,...>::length_type` narrowing |
| C4267 | per-addon | `addons/ofxOpenCv/src/ofxCvHaarFinder.cpp:114,119,302`, `ofxCvImage.cpp:855,860`, `ofxCvContourFinder.cpp:203,207` | `size_t` → `int` narrowing, mostly `.size()`/`.length()` passed to OpenCV `int` params |
| C4267 | 1 (example) | `examples/3d/3DPrimitivesExample/src/ofApp.cpp:131` | `size_t` → `ofIndexType` narrowing |
| C4244 | recurs | `libs/openFrameworks/types/ofColor.h:33, 600, 603, 751` | `float`/`int` → `PixelType` (`unsigned char`/`unsigned short`/`float`) narrowing, no explicit cast, across templated `ofColor_<T>` methods |
| C4834 | 1 | `libs/openFrameworks/communication/ofArduino.cpp:941` | `[[nodiscard]]` return of `*it++;` discarded |
| C4551 | 1 (per-addon) | `addons/ofxAssimpModelLoader/src/ofxAssimpTexture.cpp:31` | `aiTextureTypeToString(...)` call MSVC misparses as missing arg list |
| C4996 | 2 (per-addon) | `addons/ofxAssimpModelLoader/src/ofxAssimpModelLoader.cpp:49,54` | addon's own deprecated `load()` overload called from within itself |
| C4996 | 1 (example) | `examples/3d/modelNoiseExample/src/ofApp.cpp:31` | example calls deprecated `ofxAssimpModelLoader::loadModel()` |
| C4996 | 1 (example) | `examples/gl/glInfoExample/src/ofApp.cpp:254` | `freopen` — use `freopen_s` or scope-disable |
| C4996 | 2 (example) | `examples/gl/gpuParticleSystemExample/src/ofApp.cpp:107,129` | deprecated `ofClear(float)` — use `ofClear(brightness, alpha)` |
| C4996 | 2 (example) | `examples/input_output/xmlSettingsExample/src/ofApp.cpp:17,154` | deprecated `ofxXmlSettings::loadFile`/`saveFile` — use `load()`/`save()` |
| C4018 | per-addon | `addons/ofxAssimp/src/Source/ofxAssimpSrcScene.cpp:219` | signed/unsigned `<` comparison |
| C4804 | per-addon | `addons/ofxSvg/src/ofxSvgElements.cpp:533` | unsafe `bool` used in `>` comparison |
| C4200 | **out of scope** | `addons/ofxKinect/libs/libfreenect/src/freenect_internal.h:204` | vendored third-party (libfreenect) — do not modify |
| C4996 (`strcpy`/`gethostbyname`/`ctime`) | **out of scope** | `addons/ofxOsc/libs/oscpack/**` | vendored third-party (oscpack) — do not modify |

\* Counts are approximate raw occurrences across all 154 example logs; the same header-driven warning appears in every example that includes it, so the number of *distinct fix sites* is much smaller than the raw count (see file list below).

**Key finding — prioritize the core lib:** `libs/openFrameworksCompiled/project/vs/openframeworksLib.vcxproj` sets `WarningLevel=Level1`, but `ofMesh.inl`, `ofPolyline.inl`, and `ofColor.h` are header-only and get recompiled at `Level3` inside every example's TU (`scripts/templates/vs/emptyExample.vcxproj`). Fixing those 3 headers once each eliminates the large majority of all warning volume across all 154 examples. `ofArduino.cpp` is a real `.cpp`, so its one warning only appears once (in the core lib build) — already effectively fixed by fixing it once.

No `/WX` and no existing `/wd`/`#pragma warning(disable` suppressions exist anywhere in the project/props files today — warnings are fully visible, unsuppressed. No CI script gates on compiler warnings.

## Fix order (by module, core lib first)

1. **Core lib headers (highest leverage — fixes recur across all 154 examples):**
   - `libs/openFrameworks/3d/ofMesh.inl` — cast `size_t` sizes/indices to `ofIndexType` at the narrowing point (or the smallest correct local fix per call site)
   - `libs/openFrameworks/graphics/ofPolyline.inl:1124` — cast return value
   - `libs/openFrameworks/types/ofColor.h:33,600,603,751` — explicit cast in each `PixelType` assignment
   - `libs/openFrameworks/communication/ofArduino.cpp:941` — use or explicitly discard (`(void)`) the `*it++` result based on what `ofArduino.cpp`'s loop actually needs
2. **Addons (fix per-addon; each recurs across every example using that addon):**
   - `addons/ofxGui/src/ofxSliderGroup.cpp:25`
   - `addons/ofxOpenCv/src/{ofxCvHaarFinder.cpp, ofxCvImage.cpp, ofxCvContourFinder.cpp}`
   - `addons/ofxAssimpModelLoader/src/ofxAssimpTexture.cpp:31` (C4551) and `ofxAssimpModelLoader.cpp:49,54` (C4996 — call the non-deprecated overload internally)
   - `addons/ofxAssimp/src/Source/ofxAssimpSrcScene.cpp:219` (C4018)
   - `addons/ofxSvg/src/ofxSvgElements.cpp:533` (C4804)
   - Skip vendored third-party code: `addons/ofxKinect/libs/libfreenect/**`, `addons/ofxOsc/libs/oscpack/**`
3. **Examples (smallest diffs, most numerous, fix last):**
   - `examples/3d/3DPrimitivesExample/src/ofApp.cpp:131`
   - `examples/3d/modelNoiseExample/src/ofApp.cpp:31` (switch to non-deprecated `load()`)
   - `examples/gl/glInfoExample/src/ofApp.cpp:254`
   - `examples/gl/gpuParticleSystemExample/src/ofApp.cpp:107,129`
   - `examples/input_output/xmlSettingsExample/src/ofApp.cpp:17,154`

Within each file, fix with the smallest behavior-preserving change:
- **Narrowing (C4267/C4244/C4018):** explicit `static_cast<T>(...)` at the narrowing point, or widen/narrow the variable's declared type when that's the more natural fix.
- **`[[nodiscard]]` ignored (C4834):** use the return value, or explicitly discard with `(void)` if genuinely unused — decide per call site based on what the value means.
- **Deprecated API self-calls (C4996 in `ofxAssimpModelLoader`):** call the non-deprecated overload internally instead of the deprecated one.
- **Deprecated API calls in examples (C4996):** switch the call site to the suggested replacement (e.g. `ofClear(brightness, alpha)`, `ofxXmlSettings::load()/save()`, `freopen_s`).
- **C4551/C4804:** fix per the compiler's specific complaint at that exact call site (add the missing call parens / fix the `bool` comparison logic) — no blanket suppression.
- **Never use `#pragma warning(disable:...)` or `/wd` flags as the fix.**

## Verification

- After each module's fixes, rebuild that module's `.vcxproj` directly (not the full `.sln`, to avoid rebuilding shared project references unnecessarily): `MSBuild.exe <path>.vcxproj -p:Configuration=Release -p:Platform=x64 -p:BuildProjectReferences=false -m -nologo -v:minimal`, and confirm no `warning C####` remains for files in that module.
- Core lib: rebuild `libs/openFrameworksCompiled/project/vs/openframeworksLib.vcxproj` directly and confirm zero warnings.
- Final verification: full rebuild of all 154 example `.vcxproj` (reuse `scripts/vs/build_remaining_parallel.sh`, which builds all examples in parallel against the pre-built core lib/addons) and confirm zero `warning C` lines across all logs in `scripts/vs/logs/`, with the same 154/160 success count (no new build failures introduced).
- Spot-check a couple of example `.exe`s actually run (e.g. `3DPrimitivesExample.exe`, an `ofxOpenCv`-based example) to confirm the narrowing-conversion fixes didn't change rendered output/behavior.
