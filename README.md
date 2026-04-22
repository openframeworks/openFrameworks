# ofRasp

*openFrameworks with Edit mode. Pi-native. Web-native.*

This repo is a distribution of [openFrameworks](https://openframeworks.cc/) aimed at **always-on Edit mode**, **Raspberry Pi**, and the **web**. The toolkit is still openFrameworks: `ofApp`, `ofMain.h`, and the addon ecosystem behave the same way.

**→ [More Info, pillars, non-goals, and status](docs/OF_RASP.md)**  
**→ [Install from GitHub](INSTALL_OFRASP.md)** (clone, deps, spine, build)  
**→ [CMake + ofxCMake](docs/CMAKE_OFRASP.md)** (optional)

## Quick start

1. Clone the repository. The **default branch is `ofrasp`** use ofrasp-dev for development. You can still switch to **stock openFrameworks** in this fork with `git checkout master` (branch name may differ; use `git branch -a`).

2. **One-shot from git:** from the repo root run  
   `bash ./scripts/install_ofrasp_from_git.sh`  

   **OR** run those pieces yourself (e.g. **[INSTALL_FROM_GITHUB.md](INSTALL_FROM_GITHUB.md)** for **`download_libs`**, then **`install_ofrasp_deps.sh`**, then **`install_ofrasp_spine.sh`**).  

3. Build sketches as the usual. More context: **[docs/OF_RASP.md](docs/OF_RASP.md)**.

## Contributing

Contributing conventions in this tree still follow **[CONTRIBUTING.md](CONTRIBUTING.md)** where they apply; ofRasp-specific work is discussed on **this fork’s** issues and PRs.

## License

MIT — see **[LICENSE.md](LICENSE.md)** (same as upstream openFrameworks).
