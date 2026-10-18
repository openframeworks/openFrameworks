#!/usr/bin/env bash
# Builds the core and a few examples with CMake (libs/openFrameworksCompiled/project/CMake).
# Each example gets its CMakeLists.txt from projectGenerator (-t cmake) when --pg is
# given, otherwise from scripts/templates/cmake/CMakeLists.txt (the file PG copies).
#
# Usage: scripts/ci/cmake/build.sh [--pg <projectGenerator>] [--platform <pg platform>] [-- <cmake configure args>]
#   scripts/ci/cmake/build.sh --pg ~/pg/projectGenerator --platform linux64 -- -G Ninja
#   scripts/ci/cmake/build.sh -- -G "Visual Studio 17 2022" -A x64
ROOT="$( cd "$(dirname "$0")/../../.." ; pwd -P )"
PG=""
PG_PLATFORM=""
CONFIGURE_ARGS=()
while [[ $# -gt 0 ]]; do
	case "$1" in
		--pg) PG="$2"; shift 2 ;;
		--platform) PG_PLATFORM="$2"; shift 2 ;;
		--) shift; CONFIGURE_ARGS=("$@"); break ;;
		*) echo "Unknown argument $1"; exit 1 ;;
	esac
done

# example path:configurations
EXAMPLES=(
	"templates/emptyExample:Release Debug"
	"graphics/graphicsExample:Release"
	"gui/guiExample:Release"
	"templates/allAddonsExample:Release"
)

if command -v ccache >/dev/null 2>&1; then
	CONFIGURE_ARGS+=(-DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache)
fi

fail(){
	echo "::error title=CMake build failed::$*"
	exit 1
}

# single-config generators take the build type at configure time, multi-config ones at build time
configure_and_build(){
	local src="$1" dir="$2" config="$3"
	cmake -S "$src" -B "$dir" "${CONFIGURE_ARGS[@]}" -DCMAKE_BUILD_TYPE="$config" || return 1
	cmake --build "$dir" --config "$config" --parallel || return 1
}

# Exercise MSVC addon layouts on every host; no Windows binaries are linked here.
echo "##[group]MSVC addon library selection"
for config in Release Debug; do
	configure_and_build "$ROOT/scripts/ci/cmake/tests/vs-addon-libraries" "$ROOT/build/cmake-vs-addon-libraries-$config" "$config" \
		|| fail "MSVC addon library selection $config"
done
echo "##[endgroup]"

echo "##[group]core"
for config in Release Debug; do
	configure_and_build "$ROOT/libs/openFrameworksCompiled/project/CMake" "$ROOT/build/cmake-core-$config" "$config" \
		|| fail "core $config"
done
echo "##[endgroup]"

for entry in "${EXAMPLES[@]}"; do
	example="${entry%%:*}"
	configs="${entry#*:}"
	dir="$ROOT/examples/$example"
	app="$(basename "$example")"
	echo "##[group]$example"
	rm -f "$dir/CMakeLists.txt"
	if [[ -n "$PG" ]]; then
		pg_args=(-o"$ROOT" -t"cmake")
		[[ -n "$PG_PLATFORM" ]] && pg_args+=(-p"$PG_PLATFORM")
		"$PG" "${pg_args[@]}" "$dir" || fail "projectGenerator -t cmake $example"
		[[ -f "$dir/CMakeLists.txt" ]] || fail "projectGenerator wrote no CMakeLists.txt for $example"
	else
		cp "$ROOT/scripts/templates/cmake/CMakeLists.txt" "$dir/" || fail "copy template to $example"
	fi
	for config in $configs; do
		configure_and_build "$dir" "$ROOT/build/cmake-$app-$config" "$config" || fail "$example $config"
		name="$app"; [[ "$config" == "Debug" ]] && name="${app}_debug"
		found=""
		for out in "$dir/bin/$name" "$dir/bin/$name.exe" "$dir/bin/$name.app"; do
			[[ -e "$out" ]] && found="$out"
		done
		[[ -n "$found" ]] || fail "$example $config: no bin/$name"
		echo "built ${found#$ROOT/}"
	done
	echo "##[endgroup]"
done
