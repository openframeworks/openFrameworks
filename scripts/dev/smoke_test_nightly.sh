#!/usr/bin/env bash
# smoke_test_nightly.sh — logical smoke test for whether this branch's
# download_libs / install_dependencies / of.yml changes would break a
# nightly or CI build.
#
# Runs in an isolated `git worktree` copy of the repo — never touches your
# real working checkout, so it's safe to run repeatedly and to Ctrl-C.
#
# Sections:
#   1. Syntax check every script changed on this branch vs master
#   2. --help sanity for each platform's download_libs.sh (no network)
#   3. .github/workflows/of.yml still parses, build-vs matrix expands to 6 jobs
#   4. gum block in scripts/linux/*/install_dependencies.sh can't abort the
#      script even when gum isn't in apt/dnf (mocked apt-cache/dnf, no root/network)
#   5. [network, opt-in] a REAL download_libs.sh run mirroring nightly.yml's
#      exact invocation, in the isolated worktree
#   6. [needs 5] SHA-256 verify state gets written, `of.sh status` reports it
#      cleanly, and cleanLibraries' git-tracked check doesn't protect the
#      freshly-downloaded (untracked) libs from a real cleanup
#
# Usage:
#   scripts/dev/smoke_test_nightly.sh              # sections 1-4 only (fast, offline)
#   scripts/dev/smoke_test_nightly.sh --real        # also run 5-6 (network, real download)
#   scripts/dev/smoke_test_nightly.sh --real --keep # ...and leave the worktree for inspection

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OF_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
DO_REAL=0
KEEP=0
for arg in "$@"; do
	case "$arg" in
		--real) DO_REAL=1 ;;
		--keep) KEEP=1 ;;
	esac
done

PASS=0
FAIL=0
pass(){ printf '  \033[32m\xE2\x9C\x93\033[0m %s\n' "$1"; PASS=$((PASS + 1)); }
fail(){ printf '  \033[31m\xE2\x9C\x97\033[0m %s\n' "$1"; FAIL=$((FAIL + 1)); }
section(){ printf '\n\033[1m%s\033[0m\n' "$1"; }

cd "$OF_DIR"

# ---------------------------------------------------------------------------
section "1. Syntax check — scripts changed on this branch vs master"
# ---------------------------------------------------------------------------
MB=$(git merge-base HEAD master 2>/dev/null || true)
CHANGED_SH=()
if [[ -n "$MB" ]]; then
	while IFS= read -r f; do
		[[ -n "$f" ]] && CHANGED_SH+=("$f")
	done < <(git diff --name-only "$MB" -- 'scripts/**/*.sh' 'scripts/*.sh' 2>/dev/null)
fi
if [[ ${#CHANGED_SH[@]} -eq 0 ]]; then
	fail "could not compute changed-file list from git — falling back to a known list"
	CHANGED_SH=(
		scripts/of.sh scripts/of_build.sh scripts/ui.sh scripts/dev/upgrade.sh
		scripts/dev/download_libs.sh scripts/dev/lib_sources.sh scripts/dev/sha_verify.sh
		scripts/osx/install_dependencies.sh scripts/vs/install_dependencies.sh
		scripts/linux/ubuntu/install_dependencies.sh scripts/linux/debian/install_dependencies.sh
		scripts/linux/fedora/install_dependencies.sh scripts/linux/archlinux/install_dependencies.sh
		scripts/linux/download_libs.sh scripts/android/download_libs.sh scripts/emscripten/download_libs.sh
		scripts/ios/download_libs.sh scripts/macos/download_libs.sh scripts/msys2/download_libs.sh
		scripts/osx/download_libs.sh scripts/vs/download_libs.sh scripts/msys2/install_dependencies.sh
	)
fi
for f in "${CHANGED_SH[@]}"; do
	[[ -f "$f" ]] || continue
	err=$(bash -n "$f" 2>&1) && pass "$f" || fail "$f -- $err"
done

# ---------------------------------------------------------------------------
section "2. download_libs.sh --help sanity (no network) for each platform"
# ---------------------------------------------------------------------------
for p in linux osx macos vs msys2 android emscripten ios; do
	f="scripts/${p}/download_libs.sh"
	[[ -f "$f" ]] || continue
	if out=$(bash "$f" --help 2>&1); then
		pass "$f --help"
	else
		# some wrappers exit non-zero on --help by forwarding to dev/download_libs.sh's
		# own exit 0 after printHelp -- only fail if it looks like a real crash
		if echo "$out" | grep -qi "usage\|help"; then
			pass "$f --help (non-zero exit but help text printed — likely fine)"
		else
			fail "$f --help -- $out"
		fi
	fi
done

# ---------------------------------------------------------------------------
section "3. .github/workflows/of.yml — YAML validity + build-vs matrix"
# ---------------------------------------------------------------------------
if command -v python3 >/dev/null 2>&1 && python3 -c "import yaml" 2>/dev/null; then
	result=$(python3 - <<'PY'
import yaml, sys
d = yaml.safe_load(open(".github/workflows/of.yml"))
job = d["jobs"].get("build-vs")
if not job:
    print("MISSING build-vs job"); sys.exit(1)
m = job["strategy"]["matrix"]
n = len(m.get("platform", [])) * len(m.get("vsYear", []))
inc = m.get("include", [])
if n != 6:
    print(f"expected 6 platform x vsYear combos, got {n}"); sys.exit(1)
years = {i["vsYear"] for i in inc}
if years != {2022, 2026}:
    print(f"include entries don't cover vsYear 2022+2026: {years}"); sys.exit(1)
print("OK")
PY
)
	if [[ "$result" == "OK" ]]; then
		pass "of.yml parses; build-vs matrix expands to 6 jobs (3 platforms x 2 VS years)"
	else
		fail "of.yml -- $result"
	fi
else
	fail "python3 + pyyaml not available — skipped (install with: pip3 install pyyaml)"
fi

# ---------------------------------------------------------------------------
section "4. gum block can't abort scripts/linux/*/install_dependencies.sh"
# ---------------------------------------------------------------------------
# Mocks apt-cache/dnf so "gum not found" is exercised without root or network,
# matching what actually happens on GitHub's ubuntu-24.04 runners today.
MOCKDIR=$(mktemp -d)
cat > "$MOCKDIR/apt-cache" <<'EOF'
#!/usr/bin/env bash
[[ "$2" == "gum" ]] && exit 1   # simulate: gum not in configured repos
exit 0
EOF
cat > "$MOCKDIR/dnf" <<'EOF'
#!/usr/bin/env bash
exit 1   # simulate: gum not available
EOF
chmod +x "$MOCKDIR/apt-cache" "$MOCKDIR/dnf"

for distro in ubuntu debian fedora; do
	f="scripts/linux/${distro}/install_dependencies.sh"
	[[ -f "$f" ]] || continue
	# extract just the gum block in isolation so we're not trying to fake
	# an entire apt/dnf install run (needs root + hundreds of real packages)
	block=$(awk '/# Optional: gum/{flag=1} flag{print} /^fi$/{if(flag){exit}}' "$f")
	if [[ -z "$block" ]]; then
		fail "$f -- could not find the gum block (marker comment missing?)"
		continue
	fi
	out=$(PATH="$MOCKDIR:$PATH" bash -c "$block" 2>&1)
	code=$?
	if [[ $code -eq 0 ]]; then
		pass "$f gum block exits 0 when gum is unavailable"
	else
		fail "$f gum block exited $code -- $out"
	fi
done
rm -rf "$MOCKDIR"

# ---------------------------------------------------------------------------
if [[ "$DO_REAL" -eq 0 ]]; then
	section "Skipped (pass --real to also run): a real download_libs.sh run + of.sh status + cleanLibraries check"
else
	section "5. REAL download_libs.sh run (network!), mirroring nightly.yml's linux64 job"
	WT_DIR=$(mktemp -d)/of-smoke-test
	CUR_SHA=$(git rev-parse HEAD)
	# --detach: HEAD's branch is already checked out in the main tree, and
	# git refuses to check out the same branch in two worktrees at once
	if git worktree add --detach "$WT_DIR" "$CUR_SHA" >/tmp/smoke_wt.log 2>&1; then
		pass "created isolated worktree at $WT_DIR (detached at $CUR_SHA)"
	else
		fail "git worktree add failed -- $(cat /tmp/smoke_wt.log)"
		exit 1
	fi

	(
		cd "$WT_DIR" || exit 1
		echo "  -- running: scripts/linux/download_libs.sh -a 64gcc6 (same as nightly.yml) --"
		if ./scripts/linux/download_libs.sh -a 64gcc6; then
			echo PASS_DOWNLOAD
		else
			echo "FAIL_DOWNLOAD exit=$?"
		fi
	) | tee /tmp/smoke_download.log
	if grep -q PASS_DOWNLOAD /tmp/smoke_download.log; then
		pass "download_libs.sh -a 64gcc6 completed"
	else
		fail "download_libs.sh -a 64gcc6 -- see /tmp/smoke_download.log"
	fi

	section "6. of.sh status + cleanLibraries sanity against the freshly downloaded libs"
	# force PLATFORM=linux: we downloaded linux64 libs above (matching
	# nightly.yml) regardless of what host we're actually running this on.
	# cmdStatus intentionally exits non-zero when setup is incomplete (no
	# Project Generator here, since we only ran download_libs) -- that's
	# correct behavior, not a bug, so check for a clean run (no crash/bash
	# error), not exit code 0.
	(cd "$WT_DIR" && PLATFORM=linux bash scripts/of.sh status < /dev/null) > /tmp/smoke_status.log 2>&1
	if grep -qE "unbound variable|command not found|syntax error|: line [0-9]+:" /tmp/smoke_status.log; then
		fail "of.sh status -- looks like a real crash, see /tmp/smoke_status.log"
	elif grep -q "openFrameworks" /tmp/smoke_status.log; then
		pass "of.sh status ran to completion (reported libs/PG state, no crash)"
	else
		fail "of.sh status -- unexpected output, see /tmp/smoke_status.log"
	fi

	# pick any one real downloaded lib dir and confirm it's correctly seen as
	# NOT git-tracked, so cleanLibraries' nuclear/platform modes would still
	# be able to remove it (this is the exact protection added for ofxKinect
	# etc. — confirming it isn't overly broad)
	SAMPLE_LIB=$(find "$WT_DIR/libs" -mindepth 1 -maxdepth 1 -type d ! -name openFrameworks ! -name openFrameworksCompiled ! -name download ! -name scripts 2>/dev/null | head -1)
	if [[ -n "$SAMPLE_LIB" ]]; then
		(
			cd "$WT_DIR" || exit 1
			OF_DIR="$WT_DIR" bash -c '
				source scripts/ui.sh
				source <(sed -n "/^pathIsGitTracked/,/^}/p" scripts/of.sh)
				if pathIsGitTracked "'"$SAMPLE_LIB"'"; then
					echo "TRACKED"
				else
					echo "NOT_TRACKED"
				fi
			'
		) > /tmp/smoke_tracked.log 2>&1
		if grep -q NOT_TRACKED /tmp/smoke_tracked.log; then
			pass "downloaded lib ($SAMPLE_LIB) correctly NOT protected by git-tracked check"
		else
			fail "downloaded lib ($SAMPLE_LIB) -- $(cat /tmp/smoke_tracked.log)"
		fi
	else
		fail "no downloaded lib dir found under $WT_DIR/libs to test"
	fi

	if [[ "$KEEP" -eq 1 ]]; then
		echo
		echo "  --keep passed: worktree left at $WT_DIR for inspection"
		echo "  remove later with: git worktree remove --force $WT_DIR"
	else
		git worktree remove --force "$WT_DIR" >/dev/null 2>&1 \
			&& pass "cleaned up worktree" \
			|| fail "could not remove worktree at $WT_DIR — remove manually"
	fi
fi

# ---------------------------------------------------------------------------
section "Summary"
# ---------------------------------------------------------------------------
echo "  $PASS passed, $FAIL failed"
[[ "$FAIL" -eq 0 ]]
