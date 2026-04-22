# ofRasp

*openFrameworks with Edit mode. Pi-native. Web-native.*

> A long-running branch of [openFrameworks](http://openframeworks.cc) that prioritizes always-on editing.

## What is this?

ofRasp **is openFrameworks.** Your sketches compile unchanged — `ofApp`, `ofMain.h`, every `ofx`* addon still works the same way.

## Bootstrap (vendored addons in `addons/`)

To keep clean with upstream ...

Run **`./scripts/install_ofrasp_deps.sh`** from the repo root. Besides OS-specific package hints, it clones **`addons/ofxFft`** (kissfft sources used by the core sound path) and vendors **`addons/ofxLibTess2/libtess2`**. The **`ofxLibTess2`** folder in this repo holds `addon_config.mk`; add **`ofxLibTess2`** to your project’s **`addons.make`** so Makefile builds link libtess2 with `ofTessellator`. Then run **`./scripts/install_spine.sh`** for ofRasp-pinned forks (ImGui, EnTT, ofxFft etc.).

## Edit mode

Every sketch ships with always-on editor. While your sketch is running, hit `Ctrl-E` to see:

- **Nodes** panel listing every entity in the scene
- **Properties** panel to scrub any `ofParameter`, position, color, radius, or other reflected field
- **Viewport** gizmo for direct manipulation
- **Toolbar** with your app name and menu hooks

Hit `Ctrl-E` again and the editor disappears — your sketch is a clean render again, frame-perfect, zero overhead.
It's what Unity calls Play/Edit, what Max/MSP calls locked/unlocked.

## The three pillars

### 1. Edit mode

The reflection plumbing that powers ofRasp is an EnTT. The intent is that *every* future OF-style sketch shipped on ofRasp is editable by its author (or viewer) without writing a line of GUI code.

### 2. Pi-native

Upstream OF's Raspberry Pi support has drifted. ofRasp tries to keep Pi as a first-class target: GLES3 renderer maintained, headless-friendly build paths, reproducible one-command setup on a fresh Raspberry Pi OS install. Pi is the smallest full-stack Edit-mode machine we can ship.

### 3. Web-native

The long arc: ofrasp.com itself runs as an ofRasp sketch under Emscripten.

## No-goals (things we will deliberately remove)

- **No projectGenerator.** Every example ships a single project-agnostic `build.sh` that derives the project name from its own folder.

- **No apothecary.** We rely on platform package managers — MSYS2 `pacman`, `apt`, Homebrew, `vcpkg` — for dependencies. `install_dependencies.sh` wires this up for you.

## Relationship to upstream openFrameworks

We send fixes that make sense upstream back as PRs. ofRasp-specific changes (Edit mode core, build system, ImGui-in-core) stay on this branch. However most of it will be in seperate addons.

If you want stock openFrameworks: `git checkout master`. If you want ofRasp: `git checkout ofrasp`

## Status

Early. Interface is unstable — in particular, the exact API for how Edit mode plugs into a sketch is still in flux. The compatibility promise for upstream OF code is stable.

## License

MIT, same as upstream openFrameworks. See [LICENSE.md](../LICENSE.md).

## Contact

- **Website:** [ofrasp.com](https://ofrasp.com)
- **Upstream:** [openframeworks.cc](http://openframeworks.cc) — everything OF's forum/Slack/docs say is still true here.
- **Issues / discussion:** on this fork's GitHub repo.

---

*ofRasp is an unofficial fork. Not affiliated with or endorsed by the openFrameworks project. openFrameworks is the work of many people over many years; ofRasp stands on their shoulders.*
