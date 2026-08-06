#!/usr/bin/env bash
set -euo pipefail

PACKAGE="${1:-cc.openframeworks.androidLifecycleStressTest}"
ACTIVITY="${2:-cc.openframeworks.android.OFActivity}"
CYCLES="${3:-20}"

if ! command -v adb >/dev/null 2>&1; then
	echo "adb was not found in PATH" >&2
	exit 2
fi

adb get-state >/dev/null
adb logcat -c

restore_rotation() {
	adb shell settings put system accelerometer_rotation 1 >/dev/null 2>&1 || true
}
trap restore_rotation EXIT

launch_app() {
	adb shell am start -W -n "${PACKAGE}/${ACTIVITY}" >/dev/null
}

launch_app
for ((cycle = 1; cycle <= CYCLES; ++cycle)); do
	echo "hardware lifecycle cycle ${cycle}/${CYCLES}"
	adb shell input keyevent KEYCODE_HOME
	sleep 1
	launch_app
	sleep 1

	adb shell settings put system accelerometer_rotation 0
	adb shell settings put system user_rotation $((cycle % 4))
	sleep 1

	if ((cycle % 5 == 0)); then
		adb shell am kill "${PACKAGE}"
		sleep 1
		launch_app
	fi
done

restore_rotation
sleep 2

LOG_OUTPUT="$(adb logcat -d -v threadtime)"
echo "${LOG_OUTPUT}" | grep -E "androidLifecycleStressTest|FATAL EXCEPTION|Fatal signal|ANR in ${PACKAGE}" || true

if echo "${LOG_OUTPUT}" | grep -Eq "FATAL EXCEPTION|Fatal signal|ANR in ${PACKAGE}|androidLifecycleStressTest.*glGetError="; then
	echo "Android hardware lifecycle stress test failed" >&2
	exit 1
fi

echo "Android hardware lifecycle stress test passed ${CYCLES} cycles"
