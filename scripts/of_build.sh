#!/usr/bin/env bash
# of_build.sh — interactive + CLI project build helpers for of.sh
# Sourced by scripts/of.sh (expects OF_DIR, UI helpers, findPGBinary, etc.)

# Parallel jobs for make / cmake / xcodebuild
OF_JOBS="${OF_JOBS:-}"
ofBuildJobs(){
	if [[ -n "$OF_JOBS" ]]; then
		printf '%s' "$OF_JOBS"
		return
	fi
	local n
	if command -v nproc >/dev/null 2>&1; then
		n=$(nproc 2>/dev/null)
	elif [[ "$(uname -s)" == "Darwin" ]]; then
		n=$(sysctl -n hw.ncpu 2>/dev/null)
	fi
	[[ -z "$n" || "$n" -lt 1 ]] && n=4
	printf '%s' "$n"
}

# Host PG platform token (osx on Darwin, linux64/linux, msys2/vs)
ofHostPgPlatform(){
	case "$OF_PLATFORM" in
		osx|macos) printf 'osx' ;;
		linux)
			case "$OF_ARCH" in
				x86_64|amd64) printf 'linux64' ;;
				aarch64|arm64) printf 'linuxaarch64' ;;
				armv7l) printf 'linuxarmv7l' ;;
				armv6l) printf 'linuxarmv6l' ;;
				*) printf 'linux64' ;;
			esac
			;;
		msys2) printf 'msys2' ;;
		vs) printf 'vs' ;;
		*) printf '%s' "$OF_PLATFORM" ;;
	esac
}

# PG platform tokens, host-first: this host's own platform, then emscripten,
# then android (the next-most-common cross-compile targets), then the rest.
pgPlatformOrder(){
	local host
	host=$(ofHostPgPlatform)
	local -a full=(osx macos emscripten ios linux64 linux vs msys2 android tvos)
	local -a ordered=()
	local seen=" "
	local p
	for p in "$host" emscripten android "${full[@]}"; do
		[[ " ${full[*]} " == *" ${p} "* ]] || continue
		[[ "$seen" == *" ${p} "* ]] && continue
		ordered+=("$p")
		seen+="${p} "
	done
	printf '%s\n' "${ordered[@]}"
}

# ---------------------------------------------------------------------------
# Tool discovery
# ---------------------------------------------------------------------------
ensureEmscriptenEnv(){
	if command -v emmake >/dev/null 2>&1 && command -v emcc >/dev/null 2>&1; then
		return 0
	fi
	local f
	for f in \
		"${EMSDK}/emsdk_env.sh" \
		"${HOME}/emsdk/emsdk_env.sh" \
		"${OF_DIR}/emsdk/emsdk_env.sh" \
		"/opt/homebrew/share/emscripten/emsdk_env.sh" \
		"/usr/local/share/emscripten/emsdk_env.sh"
	do
		[[ -n "${EMSDK:-}" && "$f" == "${EMSDK}/emsdk_env.sh" && ! -f "$f" ]] && continue
		if [[ -f "$f" ]]; then
			# shellcheck disable=SC1090
			. "$f" >/dev/null 2>&1 || true
			break
		fi
	done
	command -v emmake >/dev/null 2>&1 && command -v emcc >/dev/null 2>&1
}

# Prints browser id for emrun, or empty
findChromeForEmrun(){
	if command -v google-chrome >/dev/null 2>&1 || command -v google-chrome-stable >/dev/null 2>&1; then
		printf 'chrome'
		return 0
	fi
	if command -v chromium >/dev/null 2>&1 || command -v chromium-browser >/dev/null 2>&1; then
		printf 'chromium'
		return 0
	fi
	if [[ -d "/Applications/Google Chrome.app" ]]; then
		printf 'chrome'
		return 0
	fi
	if [[ -d "/Applications/Chromium.app" ]]; then
		printf 'chromium'
		return 0
	fi
	return 1
}

findSafariForEmrun(){
	[[ -d "/Applications/Safari.app" ]] && { printf 'safari'; return 0; }
	return 1
}

# ---------------------------------------------------------------------------
# Project discovery
# ---------------------------------------------------------------------------
isOfProjectDir(){
	local d="$1"
	[[ -d "$d" ]] || return 1
	[[ -d "$d/src" ]] && return 0
	[[ -f "$d/Makefile" || -f "$d/addons.make" || -f "$d/config.make" ]] && return 0
	[[ -f "$d/CMakeLists.txt" ]] && return 0
	# shellcheck disable=SC2086
	compgen -G "$d/*.xcodeproj" >/dev/null 2>&1 && return 0
	compgen -G "$d/*.sln" >/dev/null 2>&1 && return 0
	return 1
}

# List immediate child dirs of $1 that look like OF projects
listOfProjectChildren(){
	local root="$1" d
	[[ -d "$root" ]] || return 0
	for d in "$root"/*/; do
		[[ -d "$d" ]] || continue
		d="${d%/}"
		isOfProjectDir "$d" && printf '%s\n' "$d"
	done | sort
}

# Categories under examples/ (skip platform-only folders when listing generic)
listExampleCategories(){
	local d name
	for d in "${OF_DIR}/examples"/*/; do
		[[ -d "$d" ]] || continue
		name=$(basename "$d")
		[[ "$name" == "README.md" ]] && continue
		printf '%s\n' "$name"
	done | sort
}

listAppGroups(){
	local d name
	for d in "${OF_DIR}/apps"/*/; do
		[[ -d "$d" ]] || continue
		name=$(basename "$d")
		printf '%s\n' "$name"
	done | sort
}

# Every addon dir under addons/ — core ofx* ones and any other/ofxAddons-style
# folder that doesn't follow the ofx naming convention
listAddons(){
	local d name
	for d in "${OF_DIR}/addons"/*/; do
		[[ -d "$d" ]] || continue
		name=$(basename "$d")
		[[ "$name" == .* ]] && continue
		printf '%s\n' "$name"
	done | sort
}

# example* / Example* dirs under an addon (depth 1–2)
listAddonExamples(){
	local addonRoot="$1" d
	[[ -d "$addonRoot" ]] || return 0
	find "$addonRoot" -maxdepth 2 -type d \( \
		-name 'example*' -o -name 'Example*' -o -name 'example-*' \
	\) 2>/dev/null | sort
}

# Resolve relative or absolute project path → absolute under OF or absolute
resolveProjectPath(){
	local p="$1"
	[[ -n "$p" ]] || return 1
	if [[ -d "$p" ]]; then
		realpath "$p"
		return 0
	fi
	if [[ -d "${OF_DIR}/$p" ]]; then
		realpath "${OF_DIR}/$p"
		return 0
	fi
	if [[ -d "${OF_DIR}/examples/$p" ]]; then
		realpath "${OF_DIR}/examples/$p"
		return 0
	fi
	if [[ -d "${OF_DIR}/apps/$p" ]]; then
		realpath "${OF_DIR}/apps/$p"
		return 0
	fi
	return 1
}

# ---------------------------------------------------------------------------
# Interactive pickers
# ---------------------------------------------------------------------------
# Sets UI_MENU_RESULT to absolute project path
menuPickExample(){
	local cat name path
	local -a opts=()
	local c
	while IFS= read -r c; do
		[[ -n "$c" ]] && opts+=("${c}|${c}")
	done < <(listExampleCategories)
	[[ ${#opts[@]} -eq 0 ]] && { echoError "no examples/ categories"; return 1; }
	opts+=("Back|__back__")
	menuPick "Example category" "${opts[@]}" || return 1
	cat="$UI_MENU_RESULT"
	[[ "$cat" == "__back__" ]] && return 1

	opts=()
	while IFS= read -r path; do
		name=$(basename "$path")
		opts+=("${name}|${path}")
	done < <(listOfProjectChildren "${OF_DIR}/examples/${cat}")
	[[ ${#opts[@]} -eq 0 ]] && { echoError "no projects in examples/${cat}"; return 1; }
	opts+=("Back|__back__")
	menuPick "Example · ${cat}" "${opts[@]}" || return 1
	[[ "$UI_MENU_RESULT" == "__back__" ]] && return 1
	return 0
}

menuPickAppProject(){
	local group name path
	local -a opts=()
	local g
	while IFS= read -r g; do
		[[ -n "$g" ]] && opts+=("${g}|${g}")
	done < <(listAppGroups)
	[[ ${#opts[@]} -eq 0 ]] && { echoError "no apps/ groups"; return 1; }
	opts+=("Custom path…|__path__" "Back|__back__")
	menuPick "App group (apps/…)" "${opts[@]}" || return 1
	group="$UI_MENU_RESULT"
	[[ "$group" == "__back__" ]] && return 1
	if [[ "$group" == "__path__" ]]; then
		menuInput "Project path (relative to OF root or absolute)" "apps/myApps/" || return 1
		path=$(resolveProjectPath "$UI_INPUT_RESULT") || {
			echoError "not a directory: $UI_INPUT_RESULT"
			return 1
		}
		UI_MENU_RESULT="$path"
		return 0
	fi

	opts=()
	# prefer projects directly under group; also one level deeper (myApps/foo)
	while IFS= read -r path; do
		name=$(basename "$path")
		opts+=("${name}|${path}")
	done < <(listOfProjectChildren "${OF_DIR}/apps/${group}")

	# nested: group/sub/project
	local sub
	for sub in "${OF_DIR}/apps/${group}"/*/; do
		[[ -d "$sub" ]] || continue
		isOfProjectDir "${sub%/}" && continue
		while IFS= read -r path; do
			name="$(basename "$(dirname "$path")")/$(basename "$path")"
			opts+=("${name}|${path}")
		done < <(listOfProjectChildren "${sub%/}")
	done

	[[ ${#opts[@]} -eq 0 ]] && { echoError "no projects under apps/${group}"; return 1; }
	opts+=("Back|__back__")
	menuPick "Project · apps/${group}" "${opts[@]}" || return 1
	[[ "$UI_MENU_RESULT" == "__back__" ]] && return 1
	return 0
}

menuPickAddonExample(){
	local addon path name
	local -a opts=()
	local a
	while IFS= read -r a; do
		[[ -n "$a" ]] && opts+=("${a}|${a}")
	done < <(listAddons)
	[[ ${#opts[@]} -eq 0 ]] && { echoError "no addons/ofx*"; return 1; }
	opts+=("Back|__back__")
	menuPick "Addon" "${opts[@]}" || return 1
	addon="$UI_MENU_RESULT"
	[[ "$addon" == "__back__" ]] && return 1

	opts=()
	while IFS= read -r path; do
		[[ -d "$path" ]] || continue
		# skip if not project-like when empty
		if isOfProjectDir "$path" || [[ -d "$path/src" ]]; then
			name="${path#${OF_DIR}/addons/${addon}/}"
			opts+=("${name}|${path}")
		fi
	done < <(listAddonExamples "${OF_DIR}/addons/${addon}")

	# also any ofx* folder that has example-style children already covered;
	# if none, list any child with src/
	if [[ ${#opts[@]} -eq 0 ]]; then
		while IFS= read -r path; do
			name=$(basename "$path")
			opts+=("${name}|${path}")
		done < <(listOfProjectChildren "${OF_DIR}/addons/${addon}")
	fi

	[[ ${#opts[@]} -eq 0 ]] && {
		echoError "no examples under addons/${addon}"
		echoNote "core addons often ship without examples — try examples/ or apps/"
		return 1
	}
	opts+=("Back|__back__")
	menuPick "Addon example · ${addon}" "${opts[@]}" || return 1
	[[ "$UI_MENU_RESULT" == "__back__" ]] && return 1
	return 0
}

menuPickConfig(){
	local -a opts=(
		"Debug|Debug"
		"Release|Release"
	)
	menuPick "Configuration" "${opts[@]}" || return 1
}

menuPickBuildSystem(){
	local project="$1"
	local -a opts=()
	local hostPg
	hostPg=$(ofHostPgPlatform)

	local hasVcxproj=0
	compgen -G "${project}/*.vcxproj" >/dev/null 2>&1 && hasVcxproj=1

	if [[ "$OF_PLATFORM" == "vs" || "$hasVcxproj" -eq 1 ]]; then
		opts+=("msbuild (Visual Studio)|msbuild")
	fi
	# "make" needs a Makefile-based host toolchain; on native VS (MSVC) that's not
	# the normal path, but keep it available for msys2/mingw-style builds.
	if [[ "$OF_PLATFORM" != "vs" ]]; then
		opts+=("make (${hostPg} / host)|make")
	fi
	if [[ "$(uname -s)" == "Darwin" ]] && compgen -G "${project}/*.xcodeproj" >/dev/null 2>&1; then
		opts+=("xcodebuild|xcode")
	fi
	opts+=("emscripten (emmake + optional browser)|emscripten")
	if [[ -f "${project}/CMakeLists.txt" ]] || command -v cmake >/dev/null 2>&1; then
		opts+=("cmake|cmake")
	fi
	opts+=("Generate project files only (PG)|generate")
	menuPick "Build system · $(basename "$project")" "${opts[@]}" || return 1
}

# ---------------------------------------------------------------------------
# Visual Studio / MSBuild helpers
# ---------------------------------------------------------------------------
# Common vswhere.exe locations under MSYS2/Git-Bash (C:\ → /c/)
vsWhereBinary(){
	local c
	for c in \
		"/c/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe" \
		"/c/Program Files/Microsoft Visual Studio/Installer/vswhere.exe"
	do
		[[ -f "$c" ]] && { printf '%s' "$c"; return 0; }
	done
	return 1
}

# Locate MSBuild.exe: PATH first (Developer Command Prompt), else vswhere
findMsbuildBinary(){
	if command -v msbuild.exe >/dev/null 2>&1; then
		command -v msbuild.exe
		return 0
	fi
	if command -v MSBuild.exe >/dev/null 2>&1; then
		command -v MSBuild.exe
		return 0
	fi
	local vswhere installPath candidate
	vswhere=$(vsWhereBinary) || return 1
	installPath=$("$vswhere" -latest -prerelease -products '*' \
		-requires Microsoft.Component.MSBuild -property installationPath 2>/dev/null | tr -d '\r')
	[[ -n "$installPath" ]] || return 1
	command -v cygpath >/dev/null 2>&1 && installPath=$(cygpath -u "$installPath")
	candidate="${installPath}/MSBuild/Current/Bin/MSBuild.exe"
	[[ -f "$candidate" ]] && { printf '%s' "$candidate"; return 0; }
	return 1
}

# Host MSBuild Platform token (x64 | ARM64) from detected VS arch (64 | arm64)
vsPlatformFromArch(){
	local arch
	arch=$(command -v detectHostVsArch >/dev/null 2>&1 && detectHostVsArch || printf '64')
	case "$arch" in
		arm64) printf 'ARM64' ;;
		*) printf 'x64' ;;
	esac
}

# PlatformToolset default — v145 (VS2026) unless the newest installed VS is clearly 2022 (17.x)
vsToolsetDefault(){
	local vswhere major
	vswhere=$(vsWhereBinary) || { printf 'v145'; return 0; }
	major=$("$vswhere" -latest -prerelease -products '*' -property installationVersion 2>/dev/null | tr -d '\r' | cut -d. -f1)
	case "$major" in
		17) printf 'v143' ;;
		*) printf 'v145' ;;
	esac
}

menuPickVsToolset(){
	local def
	def=$(vsToolsetDefault)
	menuPick "PlatformToolset  (detected: ${def})" \
		"v145 — Visual Studio 2026|v145" \
		"v143 — Visual Studio 2022|v143" \
		"Custom…|custom" \
		|| return 1
	if [[ "$UI_MENU_RESULT" == "custom" ]]; then
		menuInput "PlatformToolset (e.g. v143, v145, ClangCL)" "$def" || return 1
		UI_MENU_RESULT="$UI_INPUT_RESULT"
	fi
	return 0
}

menuPickVsPlatform(){
	local def
	def=$(vsPlatformFromArch)
	menuPick "Platform  (host: ${def})" \
		"x64|x64" \
		"ARM64|ARM64" \
		"ARM64EC|ARM64EC" \
		|| return 1
	return 0
}

# PG "-t" template: vs2026 (default) vs vs2022 (empty → PG's legacy default)
menuPickPgTemplate(){
	menuPick "Visual Studio project template" \
		"Visual Studio 2026 (default)|vs2026" \
		"Visual Studio 2022|" \
		|| return 1
	return 0
}

findVcxproj(){
	compgen -G "${1}/*.vcxproj" 2>/dev/null | head -1
}

runMsbuildProject(){
	local project="$1"
	local config="${2:-Release}"
	local toolset="${3:-}"
	local platform="${4:-}"
	local vcxproj msbuild

	vcxproj=$(findVcxproj "$project")
	[[ -n "$vcxproj" ]] || {
		echoError "no .vcxproj in $(basename "$project")"
		echoNote "generate one first: of build generate \"$project\""
		return 1
	}

	msbuild=$(findMsbuildBinary) || {
		echoError "msbuild.exe not found"
		echoNote "install the 'Desktop development with C++' workload, or run from a Developer Command Prompt with VS on PATH"
		return 1
	}

	[[ -n "$toolset" ]] || toolset="${OF_VS_TOOLSET:-$(vsToolsetDefault)}"
	[[ -n "$platform" ]] || platform="${OF_VS_PLATFORM:-$(vsPlatformFromArch)}"

	echoInfo "msbuild ${config} · $(basename "$vcxproj") · ${platform} · toolset ${toolset}"
	echoKV "msbuild" "$msbuild"
	# dash-style switches, not /slash-style: under MSYS2/Git-Bash, a leading "/"
	# looks like a POSIX absolute path and gets silently rewritten (e.g. "/m"
	# becomes "M:/", "/nologo" becomes "C:/.../nologo"), which breaks msbuild's
	# argument parsing entirely. MSBuild accepts "-switch" identically.
	"$msbuild" "$vcxproj" \
		"-p:Configuration=${config}" \
		"-p:Platform=${platform}" \
		"-p:PlatformToolset=${toolset}" \
		-nologo -m
}

# ---------------------------------------------------------------------------
# Project file helpers
# ---------------------------------------------------------------------------
projectHasMakefile(){
	[[ -f "${1}/Makefile" ]]
}

ensureProjectMakefile(){
	local project="$1"
	local template=""
	[[ -f "${project}/Makefile" ]] && return 0

	case "$OF_PLATFORM" in
		osx|macos)
			if [[ -f "${OF_DIR}/scripts/templates/macos/Makefile" ]]; then
				template="${OF_DIR}/scripts/templates/macos"
			else
				template="${OF_DIR}/scripts/templates/osx"
			fi
			;;
		linux)
			case "$OF_ARCH" in
				x86_64|amd64) template="${OF_DIR}/scripts/templates/linux64" ;;
				*) template="${OF_DIR}/scripts/templates/linux" ;;
			esac
			;;
		msys2) template="${OF_DIR}/scripts/templates/msys2" ;;
		*) template="${OF_DIR}/scripts/templates/osx" ;;
	esac

	if [[ -f "${template}/Makefile" ]]; then
		echoNote "copying Makefile from $(basename "$template") template"
		cp "${template}/Makefile" "${project}/Makefile"
		[[ ! -f "${project}/config.make" && -f "${template}/config.make" ]] && \
			cp "${template}/config.make" "${project}/config.make"
		return 0
	fi
	return 1
}

runProjectGenerator(){
	local project="$1"
	shift
	local platforms="${1:-}"
	local pgTemplate="${2:-}"
	local bin
	bin=$(findPGBinary) || {
		echoError "Project Generator not found — run: of update pg"
		return 1
	}
	local -a cmd=( "$bin" -o"$OF_DIR" )
	if [[ -n "$platforms" ]]; then
		cmd+=( -p"$platforms" )
	fi
	if [[ -n "$pgTemplate" ]]; then
		cmd+=( -t"$pgTemplate" )
	fi
	cmd+=( "$project" )
	echoNote "${cmd[*]}"
	PG_OF_PATH="$OF_DIR" "${cmd[@]}"
}

ideNameForPlatform(){
	case "$OF_PLATFORM" in
		vs) printf 'Visual Studio' ;;
		osx|macos) printf 'Xcode' ;;
		linux) printf 'Qt Creator / Code::Blocks' ;;
		*) printf 'IDE' ;;
	esac
}

# Best-effort native-IDE launch after generating project files
openGeneratedProject(){
	local project="$1"
	local proj
	case "$OF_PLATFORM" in
		vs)
			proj=$(compgen -G "${project}/*.sln" 2>/dev/null | head -1)
			[[ -z "$proj" ]] && proj=$(findVcxproj "$project")
			[[ -n "$proj" ]] || { echoWarning "no .sln/.vcxproj to open"; return 1; }
			echoInfo "opening $(basename "$proj") in Visual Studio"
			# explorer.exe, not `cmd.exe /c start`: under MSYS2/Git-Bash a leading
			# "/" (like /c) looks like a POSIX path and gets silently rewritten to
			# a drive letter (e.g. "/c" -> "C:/"), so cmd.exe never sees a valid
			# /c switch. explorer.exe takes a plain path — no flag to mangle —
			# and Windows opens .sln with its registered handler (Visual Studio).
			local winProj="$proj"
			command -v cygpath >/dev/null 2>&1 && winProj=$(cygpath -w "$proj")
			explorer.exe "$winProj" >/dev/null 2>&1 &
			disown 2>/dev/null || true
			;;
		osx|macos)
			proj=$(compgen -G "${project}/*.xcodeproj" 2>/dev/null | head -1)
			[[ -n "$proj" ]] || { echoWarning "no .xcodeproj to open"; return 1; }
			echoInfo "opening $(basename "$proj") in Xcode"
			open "$proj"
			;;
		linux)
			proj=$(compgen -G "${project}/*.qbs" 2>/dev/null | head -1)
			if [[ -n "$proj" ]] && command -v qtcreator >/dev/null 2>&1; then
				echoInfo "opening $(basename "$proj") in Qt Creator"
				qtcreator "$proj" >/dev/null 2>&1 &
				disown 2>/dev/null || true
				return 0
			fi
			proj=$(compgen -G "${project}/*.workspace" 2>/dev/null | head -1)
			if [[ -n "$proj" ]] && command -v codeblocks >/dev/null 2>&1; then
				echoInfo "opening $(basename "$proj") in Code::Blocks"
				codeblocks "$proj" >/dev/null 2>&1 &
				disown 2>/dev/null || true
				return 0
			fi
			echoNote "no known IDE found on PATH — project files are in $(basename "$project")"
			return 1
			;;
		*)
			echoNote "no IDE launch wired up for platform '${OF_PLATFORM}'"
			return 0
			;;
	esac
}

# ---------------------------------------------------------------------------
# Build runners
# ---------------------------------------------------------------------------
runMakeInProject(){
	local project="$1"
	local config="${2:-Release}"
	local jobs
	jobs=$(ofBuildJobs)
	ensureProjectMakefile "$project" || {
		echoError "no Makefile and could not install template Makefile"
		echoNote "try: of build generate \"$project\""
		return 1
	}
	echoInfo "make ${config} -j${jobs} · $(basename "$project")"
	echoKV "dir" "$project"
	(
		cd "$project" || exit 1
		make -j"$jobs" "$config"
	)
}

runMakeCore(){
	local config="${1:-Release}"
	local jobs
	jobs=$(ofBuildJobs)
	local coreDir="${OF_DIR}/libs/openFrameworksCompiled/project"
	echoInfo "OF core make ${config} -j${jobs}"
	echoKV "dir" "$coreDir"
	(
		cd "$coreDir" || exit 1
		make -j"$jobs" "$config"
	)
}

runXcodeBuild(){
	local project="$1"
	local config="${2:-Release}"
	local xproj name
	xproj=$(compgen -G "${project}/*.xcodeproj" | head -1)
	[[ -n "$xproj" && -d "$xproj" ]] || {
		echoError "no .xcodeproj in $project"
		echoNote "run PG first: of build generate \"$project\""
		return 1
	}
	name=$(basename "$xproj" .xcodeproj)
	echoInfo "xcodebuild ${config} · ${name}"
	(
		cd "$project" || exit 1
		xcodebuild -configuration "$config" -project "$(basename "$xproj")" -target "$name" \
			-jobs "$(ofBuildJobs)"
	)
}

runEmscriptenBuild(){
	local project="$1"
	local config="${2:-Debug}"
	local jobs debugFlag=0
	jobs=$(ofBuildJobs)

	ensureEmscriptenEnv || {
		echoError "emmake/emcc not found"
		echoNote "install: scripts/emscripten/install_emscripten.sh  or  source ~/emsdk/emsdk_env.sh"
		return 1
	}

	# Ensure Makefile exists (host template is fine; emmake sets CC=emcc → PLATFORM_OS=emscripten)
	ensureProjectMakefile "$project" || {
		echoError "need Makefile in project"
		return 1
	}

	[[ "$config" == "Debug" ]] && debugFlag=1
	echoInfo "emscripten emmake make ${config} -j${jobs}"
	echoKV "emcc" "$(command -v emcc)"
	echoKV "dir" "$project"
	(
		cd "$project" || exit 1
		export EMCC_DEBUG="${EMCC_DEBUG:-$debugFlag}"
		emmake make -j"$jobs" "$config"
	)
}

findEmscriptenIndexHtml(){
	local project="$1"
	local f
	# Preferred layout: bin/em/<name>/index.html
	f=$(find "${project}/bin/em" -name 'index.html' 2>/dev/null | head -1)
	[[ -n "$f" ]] && { printf '%s' "$f"; return 0; }
	f=$(find "${project}/bin" -name 'index.html' 2>/dev/null | head -1)
	[[ -n "$f" ]] && { printf '%s' "$f"; return 0; }
	return 1
}

runEmscriptenOpen(){
	local project="$1"
	local html browser=""
	html=$(findEmscriptenIndexHtml "$project") || {
		echoError "no emscripten index.html under ${project}/bin"
		echoNote "build first with: of build emscripten <path>"
		return 1
	}
	echoKV "html" "$html"

	if ! command -v emrun >/dev/null 2>&1; then
		ensureEmscriptenEnv || true
	fi

	browser=$(findChromeForEmrun || true)
	if command -v emrun >/dev/null 2>&1; then
		local -a args=( --no_emrun_detect )
		if [[ -n "$browser" ]]; then
			args+=( --browser "$browser" )
			echoInfo "emrun → ${browser}"
		else
			echoInfo "emrun (default browser)"
		fi
		# enable-unsafe-webgpu is useful for some OF webgpu experiments; ignore if unsupported
		(
			cd "$(dirname "$html")" || exit 1
			emrun "${args[@]}" --enable-unsafe-webgpu "$(basename "$html")" 2>/dev/null \
				|| emrun "${args[@]}" "$(basename "$html")"
		)
		return $?
	fi

	# Fallback: open file / serve not available — use system open
	if [[ -n "$browser" && "$(uname -s)" == "Darwin" ]]; then
		echoInfo "open in Google Chrome (emrun missing)"
		open -a "Google Chrome" "$html"
		return $?
	fi
	if command -v xdg-open >/dev/null 2>&1; then
		xdg-open "$html"
		return $?
	fi
	if command -v open >/dev/null 2>&1; then
		open "$html"
		return $?
	fi
	echoError "no emrun / open available; open manually: $html"
	return 1
}

runCmakeProject(){
	local project="$1"
	local config="${2:-Release}"
	local buildDir="${project}/build"
	local jobs
	jobs=$(ofBuildJobs)

	command -v cmake >/dev/null 2>&1 || {
		echoError "cmake not found"
		return 1
	}

	if [[ ! -f "${project}/CMakeLists.txt" ]]; then
		echoError "no CMakeLists.txt in $(basename "$project")"
		echoNote "desktop oF projects normally use make/xcode; CMake is first-class for Android"
		echoNote "toolchains live in libs/openFrameworksCompiled/project/CMake/toolchain/"
		return 1
	fi

	echoInfo "cmake · ${config} · $(basename "$project")"
	mkdir -p "$buildDir"
	(
		cmake -S "$project" -B "$buildDir" \
			-DCMAKE_BUILD_TYPE="$config" \
			${OF_CMAKE_ARGS:-} \
			&& cmake --build "$buildDir" --config "$config" -j"$jobs"
	)
}

runCmakeAndroidCore(){
	local androidDir="${OF_DIR}/libs/openFrameworksCompiled/project/android/openframeworksAndroid"
	local config="${1:-Debug}"
	[[ -f "${androidDir}/CMakeLists.txt" ]] || {
		echoError "Android CMakeLists not found at openframeworksAndroid"
		return 1
	}
	echoWarning "Android NDK CMake usually runs via Gradle/Android Studio"
	echoKV "dir" "$androidDir"
	if [[ -f "${androidDir}/gradlew" ]]; then
		confirmYes "Run ./gradlew assemble${config} in openframeworksAndroid?" || return 0
		(
			cd "$androidDir" || exit 1
			./gradlew "assemble${config}"
		)
		return $?
	fi
	runCmakeProject "$androidDir" "$config"
}

runCmakeStatus(){
	printBanner "cmake"
	printf '\n'
	if command -v cmake >/dev/null 2>&1; then
		echoKV "cmake" "$(cmake --version 2>/dev/null | head -1)"
	else
		echoKV "cmake" "not installed"
	fi
	echoKV "toolchains" "${OF_DIR}/libs/openFrameworksCompiled/project/CMake/toolchain"
	local t
	for t in "${OF_DIR}/libs/openFrameworksCompiled/project/CMake/toolchain"/*.cmake; do
		[[ -f "$t" ]] && echoNote "  $(basename "$t")"
	done
	local androidCm="${OF_DIR}/libs/openFrameworksCompiled/project/android/openframeworksAndroid/CMakeLists.txt"
	if [[ -f "$androidCm" ]]; then
		echoKV "android OF" "CMakeLists.txt present (Gradle/NDK)"
	else
		echoKV "android OF" "—"
	fi
	echoKV "desktop OF" "make / Xcode / VS are primary"
	echoNote "per-project cmake works when CMakeLists.txt exists (Android templates, some apps)"
	echoNote "set OF_CMAKE_ARGS for extra configure flags"
	printf '\n'
}

runCleanProject(){
	local project="$1"
	if projectHasMakefile "$project" || ensureProjectMakefile "$project" 2>/dev/null; then
		echoInfo "make clean · $(basename "$project")"
		( cd "$project" && make clean )
		return $?
	fi
	if [[ -d "${project}/build" ]]; then
		echoInfo "rm -rf build/"
		rm -rf "${project}/build"
		return 0
	fi
	echoError "nothing to clean"
	return 1
}

runNativeApp(){
	local project="$1"
	local config="${2:-Release}"
	local name app
	name=$(basename "$project")
	if [[ "$config" == "Debug" ]]; then
		app="${project}/bin/${name}_debug.app"
		[[ -d "$app" ]] || app="${project}/bin/${name}_debug"
	else
		app="${project}/bin/${name}.app"
		[[ -d "$app" ]] || app="${project}/bin/${name}"
	fi
	if [[ -d "$app" && "$(uname -s)" == "Darwin" ]]; then
		echoInfo "open $(basename "$app")"
		open "$app"
		return $?
	fi
	if [[ -x "$app" ]]; then
		echoInfo "run $app"
		"$app" &
		return 0
	fi
	# macOS sometimes uses bin/name.app only for Release
	app=$(find "${project}/bin" -maxdepth 2 -name '*.app' 2>/dev/null | head -1)
	if [[ -n "$app" ]]; then
		open "$app"
		return $?
	fi
	echoWarning "could not locate built app under bin/"
	return 1
}

# ---------------------------------------------------------------------------
# High-level command: build one project with a system
# ---------------------------------------------------------------------------
cmdBuildProject(){
	local project="$1"
	local system="${2:-make}"
	local config="${3:-Release}"
	local openAfter="${4:-0}"
	# system-specific extras: msbuild → toolset/platform; generate → PG template
	local extra1="${5:-}"
	local extra2="${6:-}"

	project=$(resolveProjectPath "$project") || {
		echoError "project not found: $1"
		return 1
	}
	isOfProjectDir "$project" || echoWarning "dir may not be a full OF project: $project"

	printBanner "build"
	echoKV "project" "$project"
	echoKV "system" "$system"
	echoKV "config" "$config"
	printf '\n'

	case "$system" in
		msbuild|vs|vcxproj)
			runMsbuildProject "$project" "$config" "$extra1" "$extra2" || return $?
			[[ "$openAfter" == "1" ]] && runNativeApp "$project" "$config"
			;;
		make|host)
			runMakeInProject "$project" "$config" || return $?
			[[ "$openAfter" == "1" ]] && runNativeApp "$project" "$config"
			;;
		xcode|xcodebuild)
			runXcodeBuild "$project" "$config" || return $?
			[[ "$openAfter" == "1" ]] && runNativeApp "$project" "$config"
			;;
		emscripten|em|wasm)
			runEmscriptenBuild "$project" "$config" || return $?
			if [[ "$openAfter" == "1" ]]; then
				runEmscriptenOpen "$project"
			elif menuCanRun; then
				if confirmYes "Open in browser (Chrome if available)?"; then
					runEmscriptenOpen "$project"
				fi
			fi
			;;
		cmake)
			runCmakeProject "$project" "$config" || return $?
			;;
		generate|pg)
			local plats
			plats=$(ofHostPgPlatform)
			runProjectGenerator "$project" "$plats" "$extra1" || return $?
			;;
		clean)
			runCleanProject "$project" || return $?
			;;
		*)
			echoError "unknown build system: $system"
			echoNote "valid: make msbuild xcode emscripten cmake generate clean"
			return 1
			;;
	esac
	echoSuccess "done · $(basename "$project")"
}

cmdBuildCore(){
	local config="${1:-Release}"
	printBanner "build"
	echoInfo "openFrameworks core library"
	runMakeCore "$config" || return $?
	echoSuccess "core ${config} built"
}

cmdBuildGenerate(){
	local project="$1"
	local platforms="${2:-}"
	local pgTemplate="${3:-}"
	local openAfter="${4:-0}"
	project=$(resolveProjectPath "$project") || {
		echoError "path not found: $1"
		return 1
	}
	[[ -n "$platforms" ]] || platforms=$(ofHostPgPlatform)
	# default the VS project template to 2026 when generating for vs and none was given
	if [[ -z "$pgTemplate" && "$platforms" == *vs* && "$OF_PLATFORM" == "vs" ]]; then
		pgTemplate="${OF_PG_TEMPLATE:-vs2026}"
	fi
	printBanner "generate"
	echoKV "project" "$project"
	echoKV "platforms" "$platforms"
	[[ -n "$pgTemplate" ]] && echoKV "vs template" "$pgTemplate"
	runProjectGenerator "$project" "$platforms" "$pgTemplate" || return $?
	echoSuccess "project files updated"
	[[ "$openAfter" == "1" ]] && openGeneratedProject "$project"
}

# ---------------------------------------------------------------------------
# Interactive Build menu
# ---------------------------------------------------------------------------
menuBuildProjectActions(){
	local project="$1"
	local system config openAfter=0

	echoKV "project" "$project"
	printf '\n'
	menuPickBuildSystem "$project" || return 0
	system="$UI_MENU_RESULT"

	if [[ "$system" == "generate" ]]; then
		local -a popts=()
		local p plats="" pgTemplate="" openAfterGen=0
		while IFS= read -r p; do
			popts+=("${p}|${p}")
		done < <(pgPlatformOrder)
		if menuCanRun; then
			if menuPickMulti "PG platforms (space-separated multi-select)" "${popts[@]}"; then
				# ids are space-separated → comma list for projectGenerator
				plats=$(echo "$UI_MENU_RESULT" | tr -s '[:space:]' ',' | sed 's/^,//;s/,$//')
			fi
		fi
		[[ -z "$plats" ]] && plats=$(ofHostPgPlatform)
		if [[ "$OF_PLATFORM" == "vs" && "$plats" == *vs* ]] && menuCanRun; then
			menuPickPgTemplate && pgTemplate="$UI_MENU_RESULT"
		fi
		if menuCanRun && confirmYes "Open generated project in $(ideNameForPlatform) when done?"; then
			openAfterGen=1
		fi
		cmdBuildGenerate "$project" "$plats" "$pgTemplate" "$openAfterGen"
		return $?
	fi

	if [[ "$system" == "msbuild" ]]; then
		menuPickConfig || return 0
		config="$UI_MENU_RESULT"
		menuPickVsToolset || return 0
		local toolset="$UI_MENU_RESULT"
		menuPickVsPlatform || return 0
		local platform="$UI_MENU_RESULT"
		cmdBuildProject "$project" msbuild "$config" 0 "$toolset" "$platform"
		return $?
	fi

	menuPickConfig || return 0
	config="$UI_MENU_RESULT"

	case "$system" in
		emscripten)
			cmdBuildProject "$project" emscripten "$config" 0
			local st=$?
			[[ $st -ne 0 ]] && return $st
			if confirmYes "Open build in browser (Chrome if installed)?"; then
				runEmscriptenOpen "$project"
			fi
			;;
		make|xcode|cmake)
			cmdBuildProject "$project" "$system" "$config" 0
			local st=$?
			[[ $st -ne 0 ]] && return $st
			if [[ "$system" != "cmake" ]] && confirmNo "Run / open the app?"; then
				runNativeApp "$project" "$config"
			fi
			;;
		*)
			cmdBuildProject "$project" "$system" "$config" 0
			;;
	esac
}

menuCmake(){
	local choice
	printBanner "cmake"
	runCmakeStatus
	menuPick "CMake actions" \
		"Status (again)|status" \
		"Build project with CMakeLists.txt…|project" \
		"Android OF core (Gradle/CMake)…|android" \
		"Back|back" \
		|| return 0
	choice="$UI_MENU_RESULT"
	case "$choice" in
		status) runCmakeStatus ;;
		project)
			local path
			menuPick "Where is the CMake project?" \
				"Browse apps…|apps" \
				"Browse examples…|examples" \
				"Custom path…|path" \
				"Back|back" || return 0
			case "$UI_MENU_RESULT" in
				apps) menuPickAppProject || return 0; path="$UI_MENU_RESULT" ;;
				examples) menuPickExample || return 0; path="$UI_MENU_RESULT" ;;
				path)
					menuInput "Path to project with CMakeLists.txt" || return 0
					path=$(resolveProjectPath "$UI_INPUT_RESULT") || {
						echoError "not found"; return 1
					}
					;;
				*) return 0 ;;
			esac
			menuPickConfig || return 0
			runCmakeProject "$path" "$UI_MENU_RESULT"
			;;
		android)
			menuPickConfig || return 0
			runCmakeAndroidCore "$UI_MENU_RESULT"
			;;
		back) return 0 ;;
	esac
}

menuBuild(){
	local choice project
	if ! menuCanRun; then
		echoWarning "no TTY — use: of build core|project|example|emscripten|generate|cmake"
		printHelpBuild
		return 0
	fi

	while true; do
		printf '\n'
		printBanner "build"
		echoKV "host" "${OF_PLATFORM}${OF_ARCH:+ / ${OF_ARCH}}"
		echoKV "jobs" "$(ofBuildJobs)  (OF_JOBS to override)"
		local em="—"
		if ensureEmscriptenEnv >/dev/null 2>&1; then em="emcc ready"; else em="not in PATH"; fi
		echoKV "emscripten" "$em"
		local pg
		pg=$(findPGBinary 2>/dev/null) || pg=""
		echoKV "project gen" "${pg:-not installed}"
		echoKV "cmake" "$(command -v cmake >/dev/null 2>&1 && cmake --version 2>/dev/null | head -1 || echo 'not installed')"
		printf '\n'

		menuPick "Build…" \
			"OF core library (make)|core" \
			"Project under apps/…|apps" \
			"Example under examples/…|examples" \
			"Addon example…|addons" \
			"Custom project path…|path" \
			"Emscripten — pick project, build, open browser…|emscripten" \
			"Generate project files (PG)…|generate" \
			"CMake…|cmake" \
			"Clean project…|clean" \
			"Apothecary — build libraries…|apothecary" \
			"Upgrade — Projects / Addons…|upgrade" \
			"Help|help" \
			"Back|back" \
			|| return 0
		choice="$UI_MENU_RESULT"
		printf '\n'

		case "$choice" in
			core)
				menuPickConfig || continue
				cmdBuildCore "$UI_MENU_RESULT"
				menuPause
				;;
			apps)
				menuPickAppProject || continue
				menuBuildProjectActions "$UI_MENU_RESULT"
				menuPause
				;;
			examples)
				menuPickExample || continue
				menuBuildProjectActions "$UI_MENU_RESULT"
				menuPause
				;;
			addons)
				menuPickAddonExample || continue
				menuBuildProjectActions "$UI_MENU_RESULT"
				menuPause
				;;
			path)
				menuInput "Project path" "apps/myApps/" || continue
				project=$(resolveProjectPath "$UI_INPUT_RESULT") || {
					echoError "not found: $UI_INPUT_RESULT"
					menuPause
					continue
				}
				menuBuildProjectActions "$project"
				menuPause
				;;
			emscripten)
				menuPick "Emscripten project from…" \
					"apps/…|apps" \
					"examples/…|examples" \
					"addon example…|addons" \
					"custom path…|path" \
					"Back|back" || continue
				case "$UI_MENU_RESULT" in
					apps) menuPickAppProject || continue; project="$UI_MENU_RESULT" ;;
					examples) menuPickExample || continue; project="$UI_MENU_RESULT" ;;
					addons) menuPickAddonExample || continue; project="$UI_MENU_RESULT" ;;
					path)
						menuInput "Project path" || continue
						project=$(resolveProjectPath "$UI_INPUT_RESULT") || {
							echoError "not found"; menuPause; continue
						}
						;;
					*) continue ;;
				esac
				menuPickConfig || continue
				cmdBuildProject "$project" emscripten "$UI_MENU_RESULT" 0
				if confirmYes "Open in browser (Chrome if available)?"; then
					runEmscriptenOpen "$project"
				fi
				menuPause
				;;
			generate)
				menuPick "Generate files for…" \
					"apps/…|apps" \
					"examples/…|examples" \
					"addon example…|addons" \
					"custom path…|path" \
					"recursive examples/|rec-examples" \
					"Back|back" || continue
				case "$UI_MENU_RESULT" in
					apps) menuPickAppProject || continue; project="$UI_MENU_RESULT" ;;
					examples) menuPickExample || continue; project="$UI_MENU_RESULT" ;;
					addons) menuPickAddonExample || continue; project="$UI_MENU_RESULT" ;;
					path)
						menuInput "Project path" || continue
						project=$(resolveProjectPath "$UI_INPUT_RESULT") || {
							echoError "not found"; menuPause; continue
						}
						;;
					rec-examples)
						local bin plats
						bin=$(findPGBinary) || {
							echoError "PG missing — of update pg"
							menuPause
							continue
						}
						plats=$(ofHostPgPlatform)
						confirmYes "Recursively update all examples for ${plats}?" || continue
						echoNote "$bin -r -o${OF_DIR} -p${plats} ${OF_DIR}/examples"
						PG_OF_PATH="$OF_DIR" "$bin" -r -o"$OF_DIR" -p"$plats" "${OF_DIR}/examples"
						menuPause
						continue
						;;
					*) continue ;;
				esac
				local plats pgTemplate="" openAfterGen=0
				plats=$(ofHostPgPlatform)
				if confirmYes "Also add emscripten platform files?"; then
					plats="${plats},emscripten"
				fi
				if [[ "$OF_PLATFORM" == "vs" && "$plats" == *vs* ]]; then
					menuPickPgTemplate && pgTemplate="$UI_MENU_RESULT"
				fi
				if confirmYes "Open generated project in $(ideNameForPlatform) when done?"; then
					openAfterGen=1
				fi
				cmdBuildGenerate "$project" "$plats" "$pgTemplate" "$openAfterGen"
				menuPause
				;;
			cmake)
				menuCmake
				menuPause
				;;
			clean)
				menuPick "Clean…" \
					"apps/…|apps" \
					"examples/…|examples" \
					"custom path…|path" \
					"Back|back" || continue
				case "$UI_MENU_RESULT" in
					apps) menuPickAppProject || continue; project="$UI_MENU_RESULT" ;;
					examples) menuPickExample || continue; project="$UI_MENU_RESULT" ;;
					path)
						menuInput "Project path" || continue
						project=$(resolveProjectPath "$UI_INPUT_RESULT") || {
							echoError "not found"; menuPause; continue
						}
						;;
					*) continue ;;
				esac
				runCleanProject "$project"
				menuPause
				;;
			apothecary)
				menuApothecary
				# menuApothecary returns 2 for "back / cancelled, nothing ran" —
				# skip the pause then instead of stopping on an empty screen
				[[ $? -eq 2 ]] || menuPause
				;;
			upgrade)
				menuUpgrade
				[[ $? -eq 2 ]] || menuPause
				;;
			help)
				printHelpBuild
				menuPause
				;;
			back) return 0 ;;
		esac
	done
}

printHelpBuild(){
	local prog
	prog=$(basename "${0:-of}")
	cat << EOF

  Build commands
    ${prog} build                         Interactive build menu (TTY)
    ${prog} build core [Debug|Release]    Build OF compiled core (make)
    ${prog} build project <path> [sys] [cfg]
    ${prog} build example <path> [sys] [cfg]
    ${prog} build emscripten <path> [Debug|Release]
    ${prog} build generate <path> [platforms]
    ${prog} build cmake <path> [Debug|Release]
    ${prog} build cmake status
    ${prog} build clean <path>
    ${prog} build open-em <path>          Open emscripten index.html (Chrome/emrun)

  Systems (project)
    make (default)  msbuild  xcode  emscripten  cmake  generate

  Env
    OF_JOBS=8              Parallel jobs
    OF_CMAKE_ARGS="..."    Extra cmake -S/-B flags
    EMCC_DEBUG=1           Emscripten debug logging
    OF_VS_TOOLSET=v145     MSBuild PlatformToolset (v145=VS2026, v143=VS2022)
    OF_VS_PLATFORM=x64     MSBuild Platform (x64 | ARM64 | ARM64EC)
    OF_PG_TEMPLATE=vs2026  Project Generator "-t" template when generating for vs

  Examples
    ${prog} build core Debug
    ${prog} build project apps/myApps/mySketch
    ${prog} build project examples/graphics/graphicsExample xcode Debug
    ${prog} build emscripten examples/graphics/graphicsExample Debug
    ${prog} build generate apps/myApps/mySketch osx,emscripten
    OF_VS_TOOLSET=v145 OF_VS_PLATFORM=x64 ${prog} build project apps/myApps/mySketch msbuild Release

EOF
}
