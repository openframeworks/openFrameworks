# ofRasp

*openFrameworks with Edit mode. Pi-native. Web-native.*

This repo is a distribution of [openFrameworks](https://openframeworks.cc/) aimed at **always-on Edit mode**, **Raspberry Pi**, and the **web**. The toolkit is still openFrameworks: `ofApp`, `ofMain.h`, and the addon ecosystem behave the same way.

**→ [More Info, pillars, non-goals, and status](docs/OF_RASP.md)**

## Quick start

1. Clone the repository. The **default branch is `ofrasp`** (ofRasp development). You can still switch to **stock openFrameworks** in this fork with `git checkout master` (branch name may differ; use `git branch -a`).
2. **Dependencies + ofRasp fixups** (works on **Windows, macOS, Linux / Raspberry Pi**): from the repo root run  
   `bash ./scripts/install_ofrasp_deps.sh`  
   - **MSYS2 UCRT64:** installs `pacman` packages for OF + clones **`addons/ofxFft`** (kissfft for the sound code path) and vendors **`addons/ofxLibTess2/libtess2`**. Add **`ofxLibTess2`** to your app’s `addons.make` so tessellator symbols link.  
   - **macOS / Linux:** prints which upstream `scripts/linux/.../install_dependencies.sh` (or Xcode / OF docs) to use, then runs the same **ofxFft** + **libtess2** fixups. If you already installed OF system packages, use `bash ./scripts/install_ofrasp_deps.sh --fixups-only`.
3. **Spine addons** (ofRasp forks — ImGui, FFT, EnTT):  
   `bash ./scripts/install_spine.sh`  
   Clones or updates `addons/ofxImGui`, `addons/ofxFft`, `addons/ofxEnTT` from `https://github.com/ofrasp/`. Edit `SPINE_REPOS` in that script to add more.
4. Build sketches with the usual [openFrameworks setup](https://openframeworks.cc/download/) flow for your platform. More context: **[docs/OF_RASP.md](docs/OF_RASP.md)**.

## Relationship to upstream

- **Upstream source tree:** [openframeworks/openFrameworks](https://github.com/openframeworks/openFrameworks)  
- **Sync:** add `upstream`, `git fetch upstream`, merge or rebase as you prefer.  
- **Stock OF readme** (badges, folder tour, community links):  
  [github.com/openframeworks/openFrameworks/blob/master/README.md](https://github.com/openframeworks/openFrameworks/blob/master/README.md)

Contributing conventions in this tree still follow **[CONTRIBUTING.md](CONTRIBUTING.md)** where they apply; ofRasp-specific work is discussed on **this fork’s** issues and PRs.

## License

MIT — see **[LICENSE.md](LICENSE.md)** (same as upstream openFrameworks).
