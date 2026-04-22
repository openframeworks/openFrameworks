# ofRasp

*openFrameworks with Edit mode. Pi-native. Web-native.*

> A long-running branch of [openFrameworks](http://openframeworks.cc) that prioritizes always-on editing.

## What is this?

ofRasp **is openFrameworks.** Your sketches compile unchanged — `ofApp`, `ofMain.h`, every `ofx`* addon still works the same way.

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

The reflection plumbing that powers ofRasp is an ECS called EnTT. The intent is that *every* future OF-style sketch shipped on ofRasp is editable by its author (or viewer) without writing a line of GUI code. Human Altering.

ofRasp's reflection layer is built on EnTT, an ECS framework. The intent is that *every* sketch run through ofRasp should be editable by its author—or its viewer—without writing a single line of GUI code. We call this Human Editable.

### 2. Pi-native

Upstream OF's Raspberry Pi support has drifted. ofRasp tries to keep Pi as a first-class target: GLES3 renderer maintained, headless-friendly build paths, reproducible one-command setup on a fresh Raspberry Pi OS install. Pi is the smallest full-stack Edit-mode machine we can ship, and embedded systems with limited memory tend to run better on 32-bit than 64-bit.

### 3. Web-native

Export to web should be easy. ofrasp.com itself runs as an ofRasp sketch, compiled to the browser via Emscripten.

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
