cmake_minimum_required(VERSION 3.24)
project(of_vs_addon_library_selection LANGUAGES CXX)

get_filename_component(source_root "${CMAKE_CURRENT_LIST_DIR}/../../../../.." ABSOLUTE)
set(OF_ROOT "${CMAKE_CURRENT_BINARY_DIR}/fixture")
set(OF_ADDONS_DIR "${OF_ROOT}/addons")
set(OF_PLATFORM vs)
set(OF_ARCH x64)
include("${source_root}/libs/openFrameworksCompiled/project/CMake/OFCore.cmake")
include("${source_root}/libs/openFrameworksCompiled/project/CMake/platform/vs.cmake")
include("${source_root}/libs/openFrameworksCompiled/project/CMake/OFAddons.cmake")

# Match Apothecary's split-configuration and flat paired/shared library layouts.
# The files are discovery fixtures; the CI examples perform the native link.
foreach(addon ofxAssimpModelLoader ofxOpenCv ofxFlat)
	file(MAKE_DIRECTORY "${OF_ADDONS_DIR}/${addon}/src")
endforeach()
foreach(config Release Debug)
	file(WRITE "${OF_ADDONS_DIR}/ofxAssimpModelLoader/libs/assimp/lib/vs/x64/${config}/assimp.lib" "")
	file(WRITE "${OF_ADDONS_DIR}/ofxOpenCv/libs/opencv/lib/vs/x64/${config}/opencv_world4140.lib" "")
endforeach()
foreach(name rtAudio rtAudioD shared)
	file(WRITE "${OF_ADDONS_DIR}/ofxFlat/libs/audio/lib/vs/x64/${name}.lib" "")
endforeach()
# A different architecture must never leak into the selected libraries.
file(WRITE "${OF_ADDONS_DIR}/ofxFlat/libs/audio/lib/vs/ARM64/foreign.lib" "")
file(WRITE "${OF_ROOT}/main.cpp" "int main() { return 0; }\n")
add_executable(addon_fixture EXCLUDE_FROM_ALL "${OF_ROOT}/main.cpp")
of_target_addons(addon_fixture ofxAssimpModelLoader ofxOpenCv ofxFlat)
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/selected-$<CONFIG>.txt"
	CONTENT "$<JOIN:$<TARGET_GENEX_EVAL:addon_fixture,$<TARGET_PROPERTY:addon_fixture,LINK_LIBRARIES>>,\n>\n")
add_custom_target(verify_selection ALL
	COMMAND "${CMAKE_COMMAND}" "-DCONFIG=$<CONFIG>" "-DFIXTURE_ROOT=${OF_ROOT}"
		"-DSELECTED_FILE=${CMAKE_CURRENT_BINARY_DIR}/selected-$<CONFIG>.txt"
		-P "${CMAKE_CURRENT_LIST_DIR}/verify.cmake"
	VERBATIM)
