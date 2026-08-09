#!/usr/bin/env bash
# ui.sh - terminal UI helpers (banner, tasks, menus)
# Brand with UI_APP_NAME / UI_APP_VERSION before source.

UI_APP_NAME="${UI_APP_NAME:-openFrameworks}"
UI_APP_VERSION="${UI_APP_VERSION:-0.3.0}"
UI_ANIM="${UI_ANIM:-1}"
VERBOSE="${VERBOSE:-0}"

UI_HAS_GUM=0
command -v gum >/dev/null 2>&1 && UI_HAS_GUM=1

UI_COLOR=0
UI_INTERACTIVE=0
if [[ -t 1 && -z "${NO_COLOR:-}" ]]; then
	UI_COLOR=1
	[[ "$UI_ANIM" = 1 ]] && UI_INTERACTIVE=1
fi

UI_SPIN_FRAMES=(⠋ ⠙ ⠹ ⠸ ⠼ ⠴ ⠦ ⠧ ⠇ ⠏)
UI_SPIN_PID=""
UI_CURSOR_HIDDEN=0
UI_TASK_LABELS=()
UI_TASK_STATUS=()
UI_TASK_DETAIL=()
UI_TASK_COUNT=0
UI_TASK_DRAWN=0
UI_TASK_FRAME=0
UI_MENU_RESULT=""

if [[ "$UI_COLOR" -eq 1 ]]; then
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

isInteractive(){ [[ "$UI_INTERACTIVE" -eq 1 ]]; }
menuCanRun(){ [[ -t 0 && -t 1 ]]; }

echoVerbose(){
	[[ "$VERBOSE" = 1 ]] && printf '%s·%s %s\n' "$C_MUTED" "$C_RESET" "$*"
}

hideCursor(){
	if isInteractive && [[ "$UI_CURSOR_HIDDEN" -eq 0 ]]; then
		printf '\033[?25l'
		UI_CURSOR_HIDDEN=1
	fi
}

showCursor(){
	if [[ "$UI_CURSOR_HIDDEN" -eq 1 ]]; then
		printf '\033[?25h'
		UI_CURSOR_HIDDEN=0
	fi
}

spinCancel(){
	if [[ -n "$UI_SPIN_PID" ]]; then
		kill "$UI_SPIN_PID" 2>/dev/null
		wait "$UI_SPIN_PID" 2>/dev/null
		UI_SPIN_PID=""
	fi
}

cleanupUI(){
	spinCancel
	showCursor
}
trap cleanupUI EXIT INT TERM

echoInfo(){    printf '%s›%s %s\n' "$C_ACCENT" "$C_RESET" "$*"; }
echoSuccess(){ printf '%s✓%s %s\n' "$C_OK"     "$C_RESET" "$*"; }
echoWarning(){ printf '%s!%s %s\n' "$C_WARN"   "$C_RESET" "$*"; }
echoError(){   printf '%s✗%s %s\n' "$C_ERR"    "$C_RESET" "$*" >&2; }
echoNote(){    printf '%s│%s %s%s%s\n' "$C_ACCENT" "$C_RESET" "$C_MUTED" "$*" "$C_RESET"; }
echoKV(){      printf '  %s%-14s%s %s\n' "$C_MUTED" "$1" "$C_RESET" "$2"; }

printBanner(){
	local subtitle="${1:-cli}"
	if [[ "$UI_HAS_GUM" -eq 1 ]] && isInteractive; then
		gum style --border rounded --border-foreground 183 --foreground 255 \
			--padding "0 2" --margin "0 0" \
			"${UI_APP_NAME}  ·  ${subtitle}  ·  ${UI_APP_VERSION}"
		return
	fi
	printf '%s\n' "${C_ACCENT}╭──────────────────────────────────────────╮${C_RESET}"
	printf '%s\n' "${C_ACCENT}│${C_RESET}  ${C_BOLD}${C_FG}${UI_APP_NAME}${C_RESET}  ${C_MUTED}·${C_RESET} ${subtitle}  ${C_DIM}${UI_APP_VERSION}${C_RESET}"
	printf '%s\n' "${C_ACCENT}╰──────────────────────────────────────────╯${C_RESET}"
}

confirmYes(){
	local prompt="$1"
	local confirm
	if [[ "$UI_HAS_GUM" -eq 1 && -t 0 && -t 1 ]]; then
		gum confirm --default=yes "$prompt" 2>/dev/null || gum confirm "$prompt"
		return $?
	fi
	read -r -p "$(printf '%s?%s %s [Y/n]: ' "$C_WARN" "$C_RESET" "$prompt")" confirm
	confirm="${confirm%$'\r'}"
	[[ -z "$confirm" || "$confirm" =~ ^[Yy]$ ]]
}

# Default No (Enter = cancel / no)
confirmNo(){
	local prompt="$1"
	local confirm
	if [[ "$UI_HAS_GUM" -eq 1 && -t 0 && -t 1 ]]; then
		if gum confirm --default=no "$prompt" 2>/dev/null; then
			return 0
		fi
		# older gum without --default: fall through to read
		if gum confirm --help 2>&1 | grep -q default; then
			return 1
		fi
	fi
	read -r -p "$(printf '%s?%s %s [y/N]: ' "$C_WARN" "$C_RESET" "$prompt")" confirm
	[[ "$confirm" =~ ^[Yy]$ ]]
}

# Free-text input. Sets UI_INPUT_RESULT. Returns 1 on empty/cancel.
# Usage: menuInput "Prompt" ["default"]
menuInput(){
	local prompt="$1"
	local default="${2:-}"
	UI_INPUT_RESULT=""
	if [[ "$UI_HAS_GUM" -eq 1 ]] && menuCanRun; then
		UI_INPUT_RESULT=$(gum input --prompt "> " --placeholder "$prompt" --value "$default" 2>/dev/null) || return 1
	else
		if [[ -n "$default" ]]; then
			read -r -p "$(printf '%s›%s %s [%s]: ' "$C_ACCENT" "$C_RESET" "$prompt" "$default")" UI_INPUT_RESULT
			[[ -z "$UI_INPUT_RESULT" ]] && UI_INPUT_RESULT="$default"
		else
			read -r -p "$(printf '%s›%s %s: ' "$C_ACCENT" "$C_RESET" "$prompt")" UI_INPUT_RESULT
		fi
	fi
	[[ -n "$UI_INPUT_RESULT" ]]
}

tasksReset(){
	UI_TASK_LABELS=()
	UI_TASK_STATUS=()
	UI_TASK_DETAIL=()
	UI_TASK_COUNT=0
	UI_TASK_DRAWN=0
	UI_TASK_FRAME=0
}

tasksBegin(){
	local title="$1"
	local t
	shift
	tasksReset
	for t in "$@"; do
		UI_TASK_LABELS+=("$t")
		UI_TASK_STATUS+=("pending")
		UI_TASK_DETAIL+=("")
	done
	UI_TASK_COUNT=${#UI_TASK_LABELS[@]}
	printf '\n'
	printf '  %s%s%s\n' "$C_BOLD" "$title" "$C_RESET"
	tasksDraw
}

taskIcon(){
	local status="$1"
	local frame="${2:-0}"
	case "$status" in
		pending) printf '%s○%s' "$C_MUTED" "$C_RESET" ;;
		running)
			if isInteractive; then
				printf '%s%s%s' "$C_RUN" "${UI_SPIN_FRAMES[$((frame % ${#UI_SPIN_FRAMES[@]}))]}" "$C_RESET"
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
	local status="${UI_TASK_STATUS[$idx]}"
	local label="${UI_TASK_LABELS[$idx]}"
	local detail="${UI_TASK_DETAIL[$idx]}"
	local icon
	icon=$(taskIcon "$status" "$UI_TASK_FRAME")
	if [[ -n "$detail" ]]; then
		printf '  %s  %s%s%s  %s%s%s\n' "$icon" "$C_FG" "$label" "$C_RESET" "$C_MUTED" "$detail" "$C_RESET"
	else
		printf '  %s  %s%s%s\n' "$icon" "$C_FG" "$label" "$C_RESET"
	fi
}

tasksDraw(){
	local i
	if ! isInteractive; then
		if [[ "$UI_TASK_DRAWN" -eq 0 ]]; then
			for i in "${!UI_TASK_LABELS[@]}"; do
				taskPrintLine "$i"
			done
			UI_TASK_DRAWN=1
		fi
		return
	fi
	[[ "$UI_TASK_DRAWN" -eq 1 ]] && printf '\033[%dA' "$UI_TASK_COUNT"
	for i in "${!UI_TASK_LABELS[@]}"; do
		printf '\033[2K'
		taskPrintLine "$i"
	done
	UI_TASK_DRAWN=1
}

taskSet(){
	local idx="$1"
	local status="$2"
	local detail="${3:-}"
	UI_TASK_STATUS[$idx]="$status"
	[[ -n "$detail" || $# -ge 3 ]] && UI_TASK_DETAIL[$idx]="$detail"
	if isInteractive; then
		tasksDraw
	elif [[ "$status" != "pending" ]]; then
		taskPrintLine "$idx"
	fi
}

taskRun(){
	local idx="$1"
	local code=0
	local cmd_pid
	local frame=0
	shift
	[[ "$1" == "--" ]] && shift

	if isInteractive; then
		UI_TASK_STATUS[$idx]="running"
		tasksDraw
		hideCursor
		"$@" &
		cmd_pid=$!
		while kill -0 "$cmd_pid" 2>/dev/null; do
			UI_TASK_FRAME=$frame
			tasksDraw
			frame=$((frame + 1))
			sleep 0.08
		done
		wait "$cmd_pid" || code=$?
		showCursor
	else
		UI_TASK_STATUS[$idx]="running"
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
	local icon
	local label
	UI_TASK_STATUS[$idx]="$status"
	UI_TASK_DETAIL[$idx]="$detail"
	UI_TASK_DRAWN=0
	icon=$(taskIcon "$status")
	label="${UI_TASK_LABELS[$idx]}"
	if [[ -n "$detail" ]]; then
		printf '  %s  %s%s%s  %s%s%s\n' "$icon" "$C_FG" "$label" "$C_RESET" "$C_MUTED" "$detail" "$C_RESET"
	else
		printf '  %s  %s%s%s\n' "$icon" "$C_FG" "$label" "$C_RESET"
	fi
}

taskLive(){
	local idx="$1"
	local code=0
	shift
	[[ "$1" == "--" ]] && shift

	UI_TASK_STATUS[$idx]="running"
	UI_TASK_DETAIL[$idx]="running…"
	tasksDraw
	printf '  %s────────────────────────────────────────%s\n' "$C_MUTED" "$C_RESET"
	"$@" || code=$?
	printf '  %s────────────────────────────────────────%s\n' "$C_MUTED" "$C_RESET"
	UI_TASK_DRAWN=0
	if [[ $code -eq 0 ]]; then
		taskTickLine "$idx" done
	else
		taskTickLine "$idx" fail "exit ${code}"
	fi
	return $code
}

tasksSkipRest(){
	local i
	for i in "${!UI_TASK_STATUS[@]}"; do
		if [[ "${UI_TASK_STATUS[$i]}" == "pending" || "${UI_TASK_STATUS[$i]}" == "running" ]]; then
			if isInteractive && [[ "$UI_TASK_DRAWN" -eq 1 ]]; then
				UI_TASK_STATUS[$i]="skip"
			else
				taskTickLine "$i" skip
			fi
		fi
	done
	if isInteractive && [[ "$UI_TASK_DRAWN" -eq 1 ]]; then
		tasksDraw
	fi
}

tasksSummary(){
	local done=0
	local fail=0
	local i
	for i in "${!UI_TASK_STATUS[@]}"; do
		case "${UI_TASK_STATUS[$i]}" in
			done) done=$((done + 1)) ;;
			fail) fail=$((fail + 1)) ;;
		esac
	done
	printf '\n'
	if [[ $fail -gt 0 ]]; then
		echoError "finished with ${fail} failed · ${done} ok"
		return 1
	fi
	echoSuccess "all ${done} tasks complete"
	return 0
}

menuPick(){
	local header="$1"
	local item label id i choice
	local -a items labels ids
	shift
	items=("$@")
	labels=()
	ids=()
	UI_MENU_RESULT=""

	for item in "${items[@]}"; do
		label="${item%%|*}"
		id="${item#*|}"
		[[ "$label" == "$item" ]] && id="$item"
		labels+=("$label")
		ids+=("$id")
	done

	if [[ "$UI_HAS_GUM" -eq 1 ]] && menuCanRun; then
		choice=$(printf '%s\n' "${labels[@]}" | gum choose --header "$header" --cursor "› " --height $(( ${#labels[@]} + 2 ))) || return 1
		for i in "${!labels[@]}"; do
			if [[ "${labels[$i]}" == "$choice" ]]; then
				UI_MENU_RESULT="${ids[$i]}"
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
	[[ -z "$choice" || "$choice" =~ ^[[:space:]]*[Qq] ]] && return 1
	# tolerate stray CR / whitespace / trailing junk (e.g. Windows consoles): use the first number typed
	if [[ "$choice" =~ [0-9]+ ]]; then
		choice="${BASH_REMATCH[0]}"
	else
		echoError "invalid choice: ${choice}"
		return 1
	fi
	if (( choice < 1 || choice > ${#labels[@]} )); then
		echoError "invalid choice: ${choice}"
		return 1
	fi
	UI_MENU_RESULT="${ids[$((choice - 1))]}"
	return 0
}

menuPause(){
	menuCanRun || return 0
	printf '\n'
	printf '  %spress Enter to return to menu…%s ' "$C_MUTED" "$C_RESET"
	read -r _
}

# Multi-select: sets UI_MENU_RESULT to space-separated ids
# Usage: menuPickMulti "Header" "Label|id" ...
menuPickMulti(){
	local header="$1"
	local item label id i choice n
	local -a items labels ids selected
	shift
	items=("$@")
	labels=()
	ids=()
	selected=()
	UI_MENU_RESULT=""

	for item in "${items[@]}"; do
		label="${item%%|*}"
		id="${item#*|}"
		[[ "$label" == "$item" ]] && id="$item"
		labels+=("$label")
		ids+=("$id")
	done

	if [[ "$UI_HAS_GUM" -eq 1 ]] && menuCanRun; then
		local out line
		out=$(printf '%s\n' "${labels[@]}" | gum choose --no-limit --header "$header" --cursor "› " --height $(( ${#labels[@]} + 2 ))) || return 1
		[[ -z "$out" ]] && return 1
		while IFS= read -r line; do
			[[ -z "$line" ]] && continue
			for i in "${!labels[@]}"; do
				if [[ "${labels[$i]}" == "$line" ]]; then
					selected+=("${ids[$i]}")
					break
				fi
			done
		done <<< "$out"
		UI_MENU_RESULT="${selected[*]}"
		[[ -n "$UI_MENU_RESULT" ]]
		return $?
	fi

	printf '\n'
	printf '  %s%s%s\n' "$C_BOLD" "$header" "$C_RESET"
	printf '  %s────────────────────────────────────────%s\n' "$C_MUTED" "$C_RESET"
	for i in "${!labels[@]}"; do
		printf '  %s%2d%s  %s%s%s\n' "$C_ACCENT" "$((i + 1))" "$C_RESET" "$C_FG" "${labels[$i]}" "$C_RESET"
	done
	printf '  %s────────────────────────────────────────%s\n' "$C_MUTED" "$C_RESET"
	printf '  %s›%s multi-select [e.g. 1 3 5, a=all, q=cancel]: ' "$C_ACCENT" "$C_RESET"
	read -r choice
	[[ -z "$choice" || "$choice" =~ ^[[:space:]]*[Qq] ]] && return 1
	if [[ "$choice" =~ ^[[:space:]]*[Aa] ]]; then
		UI_MENU_RESULT="${ids[*]}"
		return 0
	fi
	# tolerate stray CR / commas / trailing junk per token: use the first number in each
	for n in $choice; do
		[[ "$n" =~ [0-9]+ ]] || continue
		n="${BASH_REMATCH[0]}"
		if (( n >= 1 && n <= ${#ids[@]} )); then
			selected+=("${ids[$((n - 1))]}")
		fi
	done
	UI_MENU_RESULT="${selected[*]}"
	[[ -n "$UI_MENU_RESULT" ]]
}
