# Install ofRasp from GitHub

Short guide to clone this fork and get a **buildable** tree. For stock openFrameworks see **[INSTALL_FROM_GITHUB.md](INSTALL_FROM_GITHUB.md)**.

---

## Before you start

- Use a path **without spaces** (many scripts assume that).
- You need **Git** and **Bash** (Git Bash on Windows, or MSYS2 / WSL / Terminal on Unix).

---

## 1. Clone

Default branch is **`ofrasp`**.

**HTTPS** (no SSH key):

```bash
git clone https://github.com/ofrasp/openFrameworks.git
cd openFrameworks
```

**SSH**:

```bash
git clone git@github.com:ofrasp/openFrameworks.git
cd openFrameworks
```

Confirm branch:

```bash
git branch --show-current   # expect: ofrasp
```

To work from **development** checkout:

```bash
git checkout ofrasp-dev
```

---

## 2. One-shot install (openFrameworks + ofRasp)

From the **openFrameworks repo root**, in **Bash** (Git Bash, MSYS2, Linux, macOS):

```bash
bash ./scripts/install_ofrasp_from_git.sh
```

---

## 3. Build something

- Follow your platform’s usual OF workflow: **Makefile**, **Visual Studio**, or **Xcode**, as in **[download / setup](https://openframeworks.cc/download/)**.
- Examples that ship **`build.bat`** / **`build.sh`** can be built from their folder (see comments in those scripts).
- **CMake / CLion:** if you use **ofxCMake**, see **[docs/CMAKE_OFRASP.md](docs/CMAKE_OFRASP.md)** — set **`OF_ROOT`** to this ofRasp repo, run the same deps/spine steps, then configure **per app**.

ofRasp aims to rely on **package managers** and small scripts instead of apothecary / Project Generator for day-to-day use; PG is still valid if you prefer it.

---

## 6. Stay in sync with upstream openFrameworks

```bash
git remote add upstream https://github.com/openframeworks/openFrameworks.git   # once
git fetch upstream
git merge upstream/master    # or rebase, per your workflow
```

Resolve conflicts, then re-run **`install_ofrasp_from_git.sh`** (or at least **`install_ofrasp_spine.sh`**) if addon scripts or forks changed.

---
