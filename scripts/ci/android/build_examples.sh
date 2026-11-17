#!/usr/bin/env bash
# Build examples/android with the Android template and the libraries already
# in libs/. The core .so for $ABI must already exist. Each example is copied
# to apps/myApps, which is gitignored.
set -euo pipefail

ABI="${1:-arm64-v8a}"
shift || true

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
TEMPLATE="$ROOT/scripts/templates/android"
LIB="$ROOT/libs/openFrameworksCompiled/lib/android/${ABI}/libopenFrameworksAndroid.so"

if [[ ! -f "$LIB" ]]; then
	echo "Missing $LIB. Build the core for ${ABI} before the examples." >&2
	exit 1
fi

if [[ -z "${ANDROID_HOME:-}" && -d "${HOME}/Library/Android/sdk" ]]; then
	export ANDROID_HOME="${HOME}/Library/Android/sdk"
fi
export ANDROID_SDK_ROOT="${ANDROID_SDK_ROOT:-${ANDROID_HOME:-}}"

GRADLE_BIN="${GRADLE:-gradle}"
if ! command -v "$GRADLE_BIN" >/dev/null 2>&1; then
	echo "gradle is not on PATH. Set GRADLE to a Gradle 8.14 binary." >&2
	exit 1
fi

# The template jar is the historical Gradle 2.10 wrapper. `gradle wrapper`
# replaces it with the 8.14.3 jar. Do that once, then copy gradlew and the jar
# into every example. Repeating the wrapper task rechecks services.gradle.org.
WRAPPER_GRADLEW="$ROOT/libs/openFrameworksCompiled/project/android/gradlew"
WRAPPER_JAR="$ROOT/libs/openFrameworksCompiled/project/android/gradle/wrapper/gradle-wrapper.jar"
if [[ ! -x "$WRAPPER_GRADLEW" || ! -f "$WRAPPER_JAR" ]]; then
	cache="$ROOT/apps/myApps/.gradle-wrapper"
	WRAPPER_GRADLEW="$cache/gradlew"
	WRAPPER_JAR="$cache/gradle-wrapper.jar"
fi

failed=()
names=("$@")
if [[ ${#names[@]} -eq 0 ]]; then
	for dir in "$ROOT"/examples/android/android*; do
		[[ -d "$dir/src" ]] || continue
		names+=("$(basename "$dir")")
	done
fi

for name in "${names[@]}"; do
	example="$ROOT/examples/android/$name"
	if [[ ! -d "$example/src" ]]; then
		echo "No example source at $example" >&2
		exit 1
	fi
	dest="$ROOT/apps/myApps/$name"
	rm -rf "$dest"
	mkdir -p "$dest"
	cp "$TEMPLATE/build.gradle" "$TEMPLATE/settings.gradle" "$TEMPLATE/gradle.properties" "$TEMPLATE/proguard.cfg" "$dest/"
	cp -R "$TEMPLATE/gradle" "$TEMPLATE/ofApp" "$dest/"
	cp -R "$example/src" "$dest/src"
	if [[ -f "$example/addons.make" ]]; then
		cp "$example/addons.make" "$dest/addons.make"
	fi
	id="$(printf '%s' "$name" | tr '[:upper:]' '[:lower:]')"
	perl -i.bak -pe "s/applicationId\\s+\"[^\"]*\"/applicationId \"cc.openframeworks.${id}\"/" "$dest/ofApp/build.gradle"
	rm -f "$dest/ofApp/build.gradle.bak"

	echo "-----------------------------------------------------------------"
	echo "Building $name for $ABI"
	# A subshell on the left of || ignores set -e, so run the build as its own
	# bash and keep the status.
	set +e
	bash -c '
		set -euo pipefail
		cd "$1"
		if [[ ! -x "$2" || ! -f "$5" ]]; then
			"$6" wrapper --gradle-version 8.14.3
			mkdir -p "$(dirname "$2")" "$(dirname "$5")"
			cp ./gradlew "$2"
			cp ./gradle/wrapper/gradle-wrapper.jar "$5"
			chmod +x "$2"
		fi
		cp "$2" ./gradlew
		cp "$5" ./gradle/wrapper/gradle-wrapper.jar
		chmod +x ./gradlew
		./gradlew assembleDebug -Pandroid.injected.build.abi="$3" --stacktrace
		apk=""
		while IFS= read -r candidate; do
			apk="$candidate"
			break
		done < <(find ofApp/build -name "*.apk" -type f)
		listing="$(unzip -l "$apk" 2>/dev/null || true)"
		if [[ -z "$apk" ]] || ! grep -q "lib/${3}/libofApp.so" <<<"$listing"; then
			echo "No ${3} APK produced for $4" >&2
			exit 1
		fi
		echo "APK: $1/$apk"
	' bash "$dest" "$WRAPPER_GRADLEW" "$ABI" "$name" "$WRAPPER_JAR" "$GRADLE_BIN"
	rc=$?
	set -e
	if [[ $rc -ne 0 ]]; then
		failed+=("$name")
	fi
done

if [[ ${#failed[@]} -gt 0 ]]; then
	echo "Failed examples: ${failed[*]}" >&2
	exit 1
fi
