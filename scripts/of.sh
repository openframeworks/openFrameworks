#!/usr/bin/env bash
# of.sh - openFrameworks CLI (Dan Rosser 2025)
OF_SCRIPT_VERSION=0.2.5

OF_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OF_DIR="$(realpath "$OF_DIR/../")"
OF_CORE_SCRIPT_DIR="$(realpath "$OF_DIR/scripts")"
OF_CORE_CI_SCRIPT_DIR="$(realpath "$OF_DIR/scripts/ci")"
if [[ -d "$OF_DIR/projectGenerator" ]]; then
	OF_PG_INSTALLED_DIR="$(realpath "$OF_DIR/projectGenerator")"
else
	OF_PG_INSTALLED_DIR="$OF_DIR/projectGenerator"
fi

VERBOSE=${VERBOSE:-0}
OF_ANIM=${OF_ANIM:-1}
OF_LIB_PLATFORMS=(osx macos ios android linux vs emscripten msys2)

OF_HAS_GUM=0
command -v gum >/dev/null 2>&1 && OF_HAS_GUM=1

# Cache TTY state once — [[ -t 1 ]] fails inside $(...)
OF_COLOR=0
OF_INTERACTIVE=0
if [[ -t 1 && -z "${NO_COLOR:-}" ]]; then
	OF_COLOR=1
	[[ "${OF_ANIM}" = 1 ]] && OF_INTERACTIVE=1
fi

OF_SPIN_FRAMES=(⠋ ⠙ ⠹ ⠸ ⠼ ⠴ ⠦ ⠧ ⠇ ⠏)
OF_SPIN_PID=""
OF_CURSOR_HIDDEN=0
OF_TASK_LABELS=()
OF_TASK_STATUS=()
OF_TASK_DETAIL=()
OF_TASK_COUNT=0
OF_TASK_DRAWN=0
OF_TASK_FRAME=0
OF_MENU_RESULT=""

if [[ "$OF_COLOR" -eq 1 ]]; then
	C_RESET=$'\033[0m'
	C_DIM=$'\033[2m'
	C_BOLD=$'\033[1m'
	C_ACCENT=$'\033[38;2;192;132;252m'
	C_OK=$'\033[38;2;52;211;153m'
	C_WARN=$'\033[38;2;251;191;36m'
	C_ERR=$'\033[38;2;248;113;113m'
	C_MUTED=$'\033[38;2;148;163;184m'
	C_FG=$'\033[38;2;226;232;240m'
	C_RUN=$'\033[38;2;125;211;252m'
else
	C_RESET= C_DIM= C_BOLD= C_ACCENT= C_OK= C_WARN= C_ERR= C_MUTED= C_FG= C_RUN=
fi

isInteractive(){ [[ "$OF_INTERACTIVE" -eq 1 ]]; }

echoVerbose(){
	[[ "$VERBOSE" = 1 ]] && printf '%s·%s %s\n' "$C_MUTED" "$C_RESET" "$*"
}

hideCursor(){
	if isInteractive && [[ "$OF_CURSOR_HIDDEN" -eq 0 ]]; then
		printf '\033[?25l'
		OF_CURSOR_HIDDEN=1
	fi
}

showCursor(){
	if [[ "$OF_CURSOR_HIDDEN" -eq 1 ]]; then
		printf '\033[?25h'
		OF_CURSOR_HIDDEN=0
	fi
}

spinCancel(){
	if [[ -n "$OF_SPIN_PID" ]]; then
		kill "$OF_SPIN_PID" 2>/dev/null
		wait "$OF_SPIN_PID" 2>/dev/null
		OF_SPIN_PID=""
	fi
}

cleanupUI(){
	spinCancel
	showCursor
}
trap cleanupUI EXIT INT TERM

ofInfo(){  printf '%s›%s %s\n' "$C_ACCENT" "$C_RESET" "$*"; }
ofOk(){    printf '%s✓%s %s\n' "$C_OK"     "$C_RESET" "$*"; }
ofWarn(){  printf '%s!%s %s\n' "$C_WARN"   "$C_RESET" "$*"; }
ofErr(){   printf '%s✗%s %s\n' "$C_ERR"    "$C_RESET" "$*" >&2; }
ofNote(){  printf '%s│%s %s%s%s\n' "$C_ACCENT" "$C_RESET" "$C_MUTED" "$*" "$C_RESET"; }
ofKV(){    printf '  %s%-14s%s %s\n' "$C_MUTED" "$1" "$C_RESET" "$2"; }

ofBanner(){
	local subtitle="${1:-cli}"
	if [[ "$OF_HAS_GUM" -eq 1 ]] && isInteractive; then
		gum style --border rounded --border-foreground 183 --foreground 255 \
			--padding "0 2" --margin "0 0" \
			"openFrameworks  ·  ${subtitle}  ·  ${OF_SCRIPT_VERSION}"
		return
	fi
	printf '%s\n' "${C_ACCENT}╭──────────────────────────────────────────╮${C_RESET}"
	printf '%s\n' "${C_ACCENT}│${C_RESET}  ${C_BOLD}${C_FG}openFrameworks${C_RESET}  ${C_MUTED}·${C_RESET} ${subtitle}  ${C_DIM}${OF_SCRIPT_VERSION}${C_RESET}"
	printf '%s\n' "${C_ACCENT}╰──────────────────────────────────────────╯${C_RESET}"
}

ofConfirm(){
	local prompt="$1" confirm
	if [[ "$OF_HAS_GUM" -eq 1 && -t 0 && -t 1 ]]; then
		gum confirm "$prompt"
		return $?
	fi
	read -r -p "$(printf '%s?%s %s [Y/n]: ' "$C_WARN" "$C_RESET" "$prompt")" confirm
	[[ -z "$confirm" || "$confirm" =~ ^[Yy]$ ]]
}

tasksReset(){
	OF_TASK_LABELS=()
	OF_TASK_STATUS=()
	OF_TASK_DETAIL=()
	OF_TASK_COUNT=0
	OF_TASK_DRAWN=0
	OF_TASK_FRAME=0
}

tasksBegin(){
	local title="$1" t
	shift
	tasksReset
	for t in "$@"; do
		OF_TASK_LABELS+=("$t")
		OF_TASK_STATUS+=("pending")
		OF_TASK_DETAIL+=("")
	done
	OF_TASK_COUNT=${#OF_TASK_LABELS[@]}
	printf '\n'
	printf '  %s%s%s\n' "$C_BOLD" "$title" "$C_RESET"
	tasksDraw
}

taskIcon(){
	local status="$1" frame="${2:-0}"
	case "$status" in
		pending) printf '%s○%s' "$C_MUTED" "$C_RESET" ;;
		running)
			if isInteractive; then
				printf '%s%s%s' "$C_RUN" "${OF_SPIN_FRAMES[$((frame % ${#OF_SPIN_FRAMES[@]}))]}" "$C_RESET"
			else
				printf '%s●%s' "$C_RUN" "$C_RESET"
			fi
			;;
		done) printf '%s✓%s' "$C_OK" "$C_RESET" ;;
		fail) printf '%s✗%s' "$C_ERR" "$C_RESET" ;;
		skip) printf '%s–%s' "$C_MUTED" "$C_RESET" ;;
		*)    printf '%s·%s' "$C_MUTED" "$C_RESET" ;;
	esac
}

taskPrintLine(){
	local idx="$1"
	local status="${OF_TASK_STATUS[$idx]}"
	local label="${OF_TASK_LABELS[$idx]}"
	local detail="${OF_TASK_DETAIL[$idx]}"
	local icon
	icon=$(taskIcon "$status" "$OF_TASK_FRAME")
	if [[ -n "$detail" ]]; then
		printf '  %s  %s%s%s  %s%s%s\n' "$icon" "$C_FG" "$label" "$C_RESET" "$C_MUTED" "$detail" "$C_RESET"
	else
		printf '  %s  %s%s%s\n' "$icon" "$C_FG" "$label" "$C_RESET"
	fi
}

tasksDraw(){
	local i
	if ! isInteractive; then
		if [[ "$OF_TASK_DRAWN" -eq 0 ]]; then
			for i in "${!OF_TASK_LABELS[@]}"; do taskPrintLine "$i"; done
			OF_TASK_DRAWN=1
		fi
		return
	fi
	[[ "$OF_TASK_DRAWN" -eq 1 ]] && printf '\033[%dA' "$OF_TASK_COUNT"
	for i in "${!OF_TASK_LABELS[@]}"; do
		printf '\033[2K'
		taskPrintLine "$i"
	done
	OF_TASK_DRAWN=1
}

taskSet(){
	local idx="$1" status="$2" detail="${3:-}"
	OF_TASK_STATUS[$idx]="$status"
	[[ -n "$detail" || $# -ge 3 ]] && OF_TASK_DETAIL[$idx]="$detail"
	if isInteractive; then
		tasksDraw
	elif [[ "$status" != "pending" ]]; then
		taskPrintLine "$idx"
	fi
}

taskRun(){
	local idx="$1" code=0 cmd_pid frame=0
	shift
	[[ "$1" == "--" ]] && shift

	if isInteractive; then
		OF_TASK_STATUS[$idx]="running"
		tasksDraw
		hideCursor
		"$@" &
		cmd_pid=$!
		while kill -0 "$cmd_pid" 2>/dev/null; do
			OF_TASK_FRAME=$frame
			tasksDraw
			frame=$((frame + 1))
			sleep 0.08
		done
		wait "$cmd_pid" || code=$?
		showCursor
	else
		OF_TASK_STATUS[$idx]="running"
		taskPrintLine "$idx"
		"$@" || code=$?
	fi

	if [[ $code -eq 0 ]]; then
		taskSet "$idx" done
	else
		taskSet "$idx" fail "exit ${code}"
	fi
	return $code
}

taskTickLine(){
	local idx="$1"
	local status="${2:-done}"
	local detail="${3:-}"
	local icon label
	OF_TASK_STATUS[$idx]="$status"
	OF_TASK_DETAIL[$idx]="$detail"
	OF_TASK_DRAWN=0
	icon=$(taskIcon "$status")
	label="${OF_TASK_LABELS[$idx]}"
	if [[ -n "$detail" ]]; then
		printf '  %s  %s%s%s  %s%s%s\n' "$icon" "$C_FG" "$label" "$C_RESET" "$C_MUTED" "$detail" "$C_RESET"
	else
		printf '  %s  %s%s%s\n' "$icon" "$C_FG" "$label" "$C_RESET"
	fi
}

taskLive(){
	local idx="$1" code=0
	shift
	[[ "$1" == "--" ]] && shift

	OF_TASK_STATUS[$idx]="running"
	OF_TASK_DETAIL[$idx]="running…"
	tasksDraw
	printf '  %s────────────────────────────────────────%s\n' "$C_MUTED" "$C_RESET"
	"$@" || code=$?
	printf '  %s────────────────────────────────────────%s\n' "$C_MUTED" "$C_RESET"
	OF_TASK_DRAWN=0
	if [[ $code -eq 0 ]]; then
		taskTickLine "$idx" done
	else
		taskTickLine "$idx" fail "exit ${code}"
	fi
	return $code
}

tasksSkipRest(){
	local i
	for i in "${!OF_TASK_STATUS[@]}"; do
		if [[ "${OF_TASK_STATUS[$i]}" == "pending" || "${OF_TASK_STATUS[$i]}" == "running" ]]; then
			if isInteractive && [[ "$OF_TASK_DRAWN" -eq 1 ]]; then
				OF_TASK_STATUS[$i]="skip"
			else
				taskTickLine "$i" skip
			fi
		fi
	done
	if isInteractive && [[ "$OF_TASK_DRAWN" -eq 1 ]]; then
		tasksDraw
	fi
}

tasksSummary(){
	local done=0 fail=0 i
	for i in "${!OF_TASK_STATUS[@]}"; do
		case "${OF_TASK_STATUS[$i]}" in
			done) done=$((done + 1)) ;;
			fail) fail=$((fail + 1)) ;;
		esac
	done
	printf '\n'
	if [[ $fail -gt 0 ]]; then
		ofErr "finished with ${fail} failed · ${done} ok"
		return 1
	fi
	ofOk "all ${done} tasks complete"
	return 0
}

autoDetectOS(){
	if [[ -z "$PLATFORM" ]]; then
		export OF_OS
		OF_OS=$(uname -s | tr '[:upper:]' '[:lower:]')
		case "$OF_OS" in
			darwin|dawin)
				export OF_PLATFORM="osx"
				export OF_ARCH
				OF_ARCH=$(uname -m)
				;;
			linux)
				export OF_PLATFORM="linux"
				export OF_ARCH
				OF_ARCH=$(uname -m)
				;;
			mingw*|cygwin*|msys*)
				export OF_PLATFORM="vs"
				export OF_ARCH
				OF_ARCH=${MSYSTEM,,}
				;;
			*)
				ofErr "Unsupported platform: $OF_OS"
				exit 1
				;;
		esac
	else
		export OF_OS
		OF_OS=$(echo "${PLATFORM}" | tr '[:upper:]' '[:lower:]')
		export OF_PLATFORM="$PLATFORM"
		export OF_ARCH=""
	fi
}

coreScriptPath(){
	case "$OF_PLATFORM" in
		linux)
			case "$OF_ARCH" in
				arm64|jetson|armv7l|armv8l|aarch64)
					export OF_SCRIPT_PATH="${OF_CORE_SCRIPT_DIR}/${OF_PLATFORM}/${OF_ARCH}/"
					;;
				*)
					export OF_SCRIPT_PATH="${OF_CORE_SCRIPT_DIR}/${OF_PLATFORM}"
					;;
			esac
			;;
		macos|osx|vs|emscripten|msys2|android)
			export OF_SCRIPT_PATH="${OF_CORE_SCRIPT_DIR}/${OF_PLATFORM}"
			;;
		*)
			ofErr "Unsupported platform: $OF_PLATFORM"
			exit 1
			;;
	esac
}

autoDetectOS
coreScriptPath
echoVerbose "platform:[${OF_PLATFORM}] arch:[${OF_ARCH}] path:[${OF_SCRIPT_PATH}] gum:[${OF_HAS_GUM}]"

printHelp(){
	local prog
	prog=$(basename "${0:-of}")
	ofBanner "cli"
	printf '\n'
	printf '  %sUsage%s\n' "$C_BOLD" "$C_RESET"
	printf '    %s%s%s                        Interactive menu (TTY)\n' "$C_ACCENT" "$prog" "$C_RESET"
	printf '    %s%s%s <command> [args]      Run a command directly\n\n' "$C_ACCENT" "$prog" "$C_RESET"
	printf '  %sCommands%s\n' "$C_BOLD" "$C_RESET"
	printf '    %smenu%s                      Open interactive menu\n' "$C_ACCENT" "$C_RESET"
	printf '    %ssetup%s                     First-time: libs + Project Generator\n' "$C_ACCENT" "$C_RESET"
	printf '    %supdate%s    libs | pg          Download libs or Project Generator\n' "$C_ACCENT" "$C_RESET"
	printf '    %sversion%s   of  | pg          Show version information\n' "$C_ACCENT" "$C_RESET"
	printf '    %supgrade%s   addons | apps     Upgrade tree (backup first)\n' "$C_ACCENT" "$C_RESET"
	printf '    %sdemo%s                        Preview task list + animations\n\n' "$C_ACCENT" "$C_RESET"
	printf '  %sOptions%s\n' "$C_BOLD" "$C_RESET"
	printf '    %s-h, --help%s               Show this help\n' "$C_MUTED" "$C_RESET"
	printf '    %sVERBOSE=1%s                Debug / path details\n' "$C_MUTED" "$C_RESET"
	printf '    %sNO_COLOR=1%s               Disable color\n' "$C_MUTED" "$C_RESET"
	printf '    %sOF_ANIM=0%s                Disable spinners / list animation\n\n' "$C_MUTED" "$C_RESET"
	printf '  %sExamples%s\n' "$C_BOLD" "$C_RESET"
	printf '    %s%s%s                       # interactive menu\n' "$C_DIM" "$prog" "$C_RESET"
	printf '    %s%s setup%s                  # libs + project generator\n' "$C_DIM" "$prog" "$C_RESET"
	printf '    %s%s update%s                 # download libraries\n' "$C_DIM" "$prog" "$C_RESET"
	printf '    %s%s update pg%s              # download Project Generator\n' "$C_DIM" "$prog" "$C_RESET"
	printf '    %s%s update libs ios%s        # libs for another platform\n' "$C_DIM" "$prog" "$C_RESET"
	printf '    %s%s version%s                # openFrameworks version\n\n' "$C_DIM" "$prog" "$C_RESET"
	if [[ "$OF_HAS_GUM" -eq 1 ]]; then
		ofNote "gum detected — menus, confirms, banner"
	else
		ofNote "tip: brew install gum  → arrow-key menus"
	fi
	printf '\n'
	ofKV "platform" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
	printf '\n'
}

ensureScript(){
	local script="$1"
	if [[ ! -e "$script" ]]; then
		ofErr "Not found: ${script}"
		return 1
	fi
	if [[ ! -x "$script" ]]; then
		ofErr "Not executable: ${script}"
		return 1
	fi
	return 0
}

resolvePGScript(){
	local platformDir="$1" script
	script="${OF_CORE_SCRIPT_DIR}/${platformDir}/download_projectGenerator.sh"
	if [[ ! -f "$script" && -f "${OF_CORE_SCRIPT_DIR}/osx/download_projectGenerator.sh" ]]; then
		script="${OF_CORE_SCRIPT_DIR}/osx/download_projectGenerator.sh"
	fi
	if [[ ! -f "$script" && -f "${OF_CORE_SCRIPT_DIR}/vs/download_projectGenerator.sh" ]]; then
		script="${OF_CORE_SCRIPT_DIR}/vs/download_projectGenerator.sh"
	fi
	printf '%s' "$script"
}

cmdDemo(){
	ofBanner "demo"
	tasksBegin "Tasks" \
		"Detect platform" \
		"Resolve script paths" \
		"Simulate download" \
		"Verify layout" \
		"Write summary"
	taskSet 0 running
	sleep 0.15
	taskSet 0 done "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
	taskRun 1 -- sleep 0.9
	taskRun 2 -- sleep 1.4
	taskSet 2 done "cached · ok"
	taskRun 3 -- sleep 0.7
	taskSet 4 running
	sleep 0.2
	taskSet 4 done
	tasksSummary
}

cmdVersion(){
	local constants="${OF_DIR}/libs/openFrameworks/utils/ofConstants.h"
	local major minor patch pre version f

	ofBanner "version"
	tasksBegin "Tasks" \
		"Locate ofConstants.h" \
		"Parse version defines" \
		"Report"

	if [[ ! -f "$constants" ]]; then
		taskSet 0 fail "missing"
		tasksSkipRest
		tasksSummary
		ofErr "ofConstants.h not found at ${constants}"
		return 1
	fi
	taskSet 0 done "utils/ofConstants.h"

	taskSet 1 running
	major=$(grep "#define OF_VERSION_MAJOR" "$constants" | awk '{print $3}')
	minor=$(grep "#define OF_VERSION_MINOR" "$constants" | awk '{print $3}')
	patch=$(grep "#define OF_VERSION_PATCH" "$constants" | awk '{print $3}')
	pre=$(grep "#define OF_VERSION_PRE_RELEASE" "$constants" | awk '{print $3}' | tr -d '"')
	version="${major}.${minor}.${patch}"
	[[ -n "$pre" && "$pre" != "0" ]] && version+="-${pre}"

	if isInteractive; then
		for f in 0 1 2 3 4 5; do
			OF_TASK_FRAME=$f
			tasksDraw
			sleep 0.05
		done
	fi
	taskSet 1 done "$version"
	taskSet 2 done "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
	tasksSummary

	printf '\n'
	ofKV "openFrameworks" "$version"
	ofKV "platform" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
	ofKV "cli" "$OF_SCRIPT_VERSION"
	printf '\n'
}

cmdVersionPG(){
	local script="${OF_DIR}/projectGenerator/projectGenerator"
	local ver_out code=0 f

	ofBanner "version"
	tasksBegin "Tasks" \
		"Locate Project Generator" \
		"Query --version" \
		"Report"

	if [[ ! -e "$script" ]]; then
		taskSet 0 fail "not installed"
		tasksSkipRest
		tasksSummary
		ofNote "try: of update pg"
		return 1
	fi
	taskSet 0 done "projectGenerator"

	OF_TASK_STATUS[1]="running"
	if isInteractive; then
		tasksDraw
		hideCursor
		ver_out=$("$script" --version 2>&1) || code=$?
		for f in 0 1 2 3 4 5 6 7; do
			OF_TASK_FRAME=$f
			tasksDraw
			sleep 0.05
		done
		showCursor
	else
		taskPrintLine 1
		ver_out=$("$script" --version 2>&1) || code=$?
	fi

	if [[ $code -ne 0 ]]; then
		taskSet 1 fail
		tasksSkipRest
		tasksSummary
		return "$code"
	fi
	ver_out=$(printf '%s' "$ver_out" | tr '\n' ' ' | sed 's/[[:space:]]*$//')
	taskSet 1 done
	taskSet 2 done "$ver_out"
	tasksSummary
	printf '\n'
	ofKV "projectGenerator" "$ver_out"
	printf '\n'
}

cmdUpdate(){
	local subcmd="$1" platformDir="${2:-$OF_PLATFORM}" script title

	case "$subcmd" in
		""|libs)
			script="${OF_CORE_SCRIPT_DIR}/${platformDir}/download_libs.sh"
			title="Download libraries"
			;;
		pg|projectgenerator)
			script="${OF_CORE_SCRIPT_DIR}/${platformDir}/download_projectGenerator.sh"
			title="Download Project Generator"
			;;
		*)
			ofErr "Unknown update target: ${subcmd}"
			ofNote "valid: libs (default), pg | projectgenerator"
			return 1
			;;
	esac

	ofBanner "update"
	tasksBegin "Tasks" \
		"Detect platform" \
		"Resolve download script" \
		"$title" \
		"Finish"

	taskSet 0 done "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}} → ${platformDir}"

	if ! ensureScript "$script"; then
		taskSet 1 fail
		tasksSkipRest
		tasksSummary
		return 1
	fi
	taskSet 1 done "$(basename "$script")"

	echoVerbose "run: ${script}"
	if ! taskLive 2 -- "$script"; then
		taskTickLine 3 skip
		tasksSummary
		return 1
	fi
	taskTickLine 3 done
	tasksSummary
}

cmdUpgrade(){
	local subcmd="$1" script="${OF_CORE_SCRIPT_DIR}/dev/upgrade.sh"

	case "$subcmd" in
		addons|apps) ;;
		*)
			ofErr "Unknown upgrade target: ${subcmd:-<none>}"
			ofNote "valid: addons, apps"
			return 1
			;;
	esac

	ofBanner "upgrade"
	tasksBegin "Tasks" \
		"Confirm destructive upgrade" \
		"Locate upgrade script" \
		"Upgrade ${subcmd}" \
		"Finish"

	ofWarn "This will modify files in the ${subcmd} folder."
	ofNote "back up first · commit local repos before continuing"
	printf '\n'
	if ! ofConfirm "Continue with upgrade ${subcmd}?"; then
		taskSet 0 skip "cancelled"
		tasksSkipRest
		ofInfo "upgrade cancelled — no changes made"
		return 0
	fi
	taskSet 0 done "confirmed"

	if ! ensureScript "$script"; then
		taskSet 1 fail
		tasksSkipRest
		tasksSummary
		return 1
	fi
	taskSet 1 done "dev/upgrade.sh"

	if ! taskLive 2 -- "$script" "$subcmd"; then
		taskTickLine 3 skip
		tasksSummary
		return 1
	fi
	taskTickLine 3 done
	tasksSummary
}

cmdSetup(){
	local platformDir="${1:-$OF_PLATFORM}"
	local libsScript="${OF_CORE_SCRIPT_DIR}/${platformDir}/download_libs.sh"
	local pgScript
	pgScript=$(resolvePGScript "$platformDir")

	ofBanner "setup"
	ofInfo "first-time install  ·  platform ${platformDir}"
	tasksBegin "Tasks" \
		"Detect platform" \
		"Download libraries" \
		"Download Project Generator" \
		"Finish"

	taskSet 0 done "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}} → ${platformDir}"

	if ! ensureScript "$libsScript"; then
		taskSet 1 fail "no download_libs.sh for ${platformDir}"
		tasksSkipRest
		tasksSummary
		return 1
	fi

	echoVerbose "run: ${libsScript}"
	if ! taskLive 1 -- "$libsScript"; then
		taskTickLine 2 skip
		taskTickLine 3 skip
		tasksSummary
		return 1
	fi

	if [[ ! -x "$pgScript" ]]; then
		taskTickLine 2 skip "no PG script for ${platformDir}"
		taskTickLine 3 done "libs only"
		tasksSummary
		ofWarn "Project Generator script missing — try: of update pg"
		return 0
	fi

	echoVerbose "run: ${pgScript}"
	if ! taskLive 2 -- "$pgScript"; then
		taskTickLine 3 skip
		tasksSummary
		return 1
	fi
	taskTickLine 3 done
	tasksSummary
	printf '\n'
	ofOk "setup complete — open Project Generator or start coding"
	ofNote "hint: of version   ·   of update libs   ·   of menu"
	printf '\n'
}

menuCanRun(){ [[ -t 0 && -t 1 ]]; }

menuPick(){
	local header="$1" item label id i choice
	local -a items labels ids
	shift
	items=("$@")
	labels=()
	ids=()
	OF_MENU_RESULT=""

	for item in "${items[@]}"; do
		label="${item%%|*}"
		id="${item#*|}"
		[[ "$label" == "$item" ]] && id="$item"
		labels+=("$label")
		ids+=("$id")
	done

	if [[ "$OF_HAS_GUM" -eq 1 ]] && menuCanRun; then
		choice=$(printf '%s\n' "${labels[@]}" | gum choose --header "$header" --cursor "› " --height $(( ${#labels[@]} + 2 ))) || return 1
		for i in "${!labels[@]}"; do
			if [[ "${labels[$i]}" == "$choice" ]]; then
				OF_MENU_RESULT="${ids[$i]}"
				return 0
			fi
		done
		return 1
	fi

	printf '\n'
	printf '  %s%s%s\n' "$C_BOLD" "$header" "$C_RESET"
	printf '  %s────────────────────────────────────────%s\n' "$C_MUTED" "$C_RESET"
	for i in "${!labels[@]}"; do
		printf '  %s%2d%s  %s%s%s\n' "$C_ACCENT" "$((i + 1))" "$C_RESET" "$C_FG" "${labels[$i]}" "$C_RESET"
	done
	printf '  %s────────────────────────────────────────%s\n' "$C_MUTED" "$C_RESET"
	printf '  %s›%s choose [1-%d, q]: ' "$C_ACCENT" "$C_RESET" "${#labels[@]}"
	read -r choice
	[[ -z "$choice" || "$choice" =~ ^[Qq]$ ]] && return 1
	if [[ ! "$choice" =~ ^[0-9]+$ ]] || (( choice < 1 || choice > ${#labels[@]} )); then
		ofErr "invalid choice: ${choice}"
		return 1
	fi
	OF_MENU_RESULT="${ids[$((choice - 1))]}"
	return 0
}

menuPause(){
	menuCanRun || return 0
	printf '\n'
	printf '  %spress Enter to return to menu…%s ' "$C_MUTED" "$C_RESET"
	read -r _
}

menuPickPlatform(){
	local -a opts=()
	local p script
	for p in "${OF_LIB_PLATFORMS[@]}"; do
		script="${OF_CORE_SCRIPT_DIR}/${p}/download_libs.sh"
		[[ -f "$script" ]] || continue
		if [[ "$p" == "$OF_PLATFORM" ]]; then
			opts+=("${p}  (this machine)|${p}")
		else
			opts+=("${p}|${p}")
		fi
	done
	if [[ ${#opts[@]} -eq 0 ]]; then
		ofErr "no platform download scripts found under scripts/"
		return 1
	fi
	menuPick "Download libraries — platform" "${opts[@]}" || return 1
	cmdUpdate libs "$OF_MENU_RESULT"
}

cmdMenu(){
	local choice
	if ! menuCanRun; then
		ofWarn "no interactive TTY — showing help instead"
		printHelp
		return 1
	fi

	while true; do
		printf '\n'
		ofBanner "menu"
		printf '\n'
		ofKV "host" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
		ofKV "of dir" "$OF_DIR"
		if [[ -x "${OF_PG_INSTALLED_DIR}/projectGenerator" || -x "${OF_DIR}/projectGenerator/projectGenerator" ]]; then
			ofKV "pg" "installed"
		else
			ofKV "pg" "not installed  ·  setup or update pg"
		fi
		printf '\n'

		if ! menuPick "What do you want to do?" \
			"Setup (download libs + Project Generator)|setup" \
			"Download libraries (this platform)|libs" \
			"Download libraries (choose platform)|libs-platform" \
			"Download Project Generator|pg" \
			"Show openFrameworks version|version" \
			"Show Project Generator version|version-pg" \
			"Upgrade addons|upgrade-addons" \
			"Upgrade apps|upgrade-apps" \
			"Preview UI demo|demo" \
			"Help|help" \
			"Quit|quit"
		then
			ofInfo "bye"
			return 0
		fi
		choice="$OF_MENU_RESULT"
		printf '\n'

		case "$choice" in
			setup)          cmdSetup "$OF_PLATFORM"; menuPause ;;
			libs)           cmdUpdate libs "$OF_PLATFORM"; menuPause ;;
			libs-platform)  menuPickPlatform; menuPause ;;
			pg)             cmdUpdate pg "$OF_PLATFORM"; menuPause ;;
			version)        cmdVersion; menuPause ;;
			version-pg)     cmdVersionPG; menuPause ;;
			upgrade-addons) cmdUpgrade addons; menuPause ;;
			upgrade-apps)   cmdUpgrade apps; menuPause ;;
			demo)           cmdDemo; menuPause ;;
			help)           printHelp; menuPause ;;
			quit)           ofOk "bye"; return 0 ;;
			*)              ofErr "unknown menu id: ${choice}"; menuPause ;;
		esac
	done
}

runCommand(){
	local cmd=$1 subcmd=$2 subcmd2=$3
	case "$cmd" in
		help|-h|--help)
			printHelp
			;;
		menu)
			cmdMenu
			;;
		setup|install)
			cmdSetup "${subcmd:-$OF_PLATFORM}"
			;;
		demo)
			cmdDemo
			;;
		update)
			cmdUpdate "$subcmd" "$subcmd2"
			;;
		version)
			case "$subcmd" in
				""|of) cmdVersion ;;
				pg|projectgenerator) cmdVersionPG ;;
				*)
					ofErr "Unknown version target: ${subcmd}"
					ofNote "valid: of (default), pg | projectgenerator"
					return 1
					;;
			esac
			;;
		upgrade)
			cmdUpgrade "$subcmd"
			;;
		*)
			ofErr "Unknown command: ${cmd}"
			ofNote "valid: menu, setup, update, version, upgrade, demo, help"
			printf '\n'
			printHelp
			return 1
			;;
	esac
}

if [[ $# -eq 0 ]]; then
	if menuCanRun; then
		cmdMenu
		exit $?
	fi
	printHelp
	exit 1
fi

runCommand "$@"
exit $?
