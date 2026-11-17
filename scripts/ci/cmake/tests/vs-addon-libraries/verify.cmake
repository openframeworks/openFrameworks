file(STRINGS "${SELECTED_FILE}" actual)
if(CONFIG STREQUAL "Debug")
	set(split Debug)
	set(audio rtAudioD)
else()
	set(split Release)
	set(audio rtAudio)
endif()
set(expected
	"${FIXTURE_ROOT}/addons/ofxAssimpModelLoader/libs/assimp/lib/vs/x64/${split}/assimp.lib"
	"${FIXTURE_ROOT}/addons/ofxOpenCv/libs/opencv/lib/vs/x64/${split}/opencv_world4140.lib"
	"${FIXTURE_ROOT}/addons/ofxFlat/libs/audio/lib/vs/x64/${audio}.lib"
	"${FIXTURE_ROOT}/addons/ofxFlat/libs/audio/lib/vs/x64/shared.lib")
list(FILTER actual EXCLUDE REGEX "^$")
list(SORT actual)
list(SORT expected)
if(NOT actual STREQUAL expected)
	message(FATAL_ERROR "${CONFIG} addon libraries differ:\n  actual: ${actual}\n  expected: ${expected}")
endif()
message(STATUS "${CONFIG}: split, paired and shared MSVC addon libraries selected correctly")
