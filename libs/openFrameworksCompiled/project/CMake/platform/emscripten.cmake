# Emscripten (mirrors libs/openFrameworksCompiled/project/emscripten/config.emscripten.default.mk)
#
#   emcmake cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build
#   emrun bin/em/<app>
#
# Project options (cache variables, same names as the makefiles' config.make options):
#   PROJECT_EMSCRIPTEN_WASM           32 (default) or 64: wasm64 (MEMORY64), libs from
#                                     lib/emscripten/WASM64, core in lib/emscripten/WASM64
#   PROJECT_EMSCRIPTEN_TOTAL_MEMORY   initial memory in bytes (default 128 MB)
#   PROJECT_EMSCRIPTEN_EMBIND_AOT     ON (default) / OFF
#   PROJECT_EMSCRIPTEN_TEMPLATE       HTML shell file (default the one in project/emscripten)
#   OF_WEBGL_VERSION                  2 (default) or 1

include_guard(GLOBAL)

if(EMSCRIPTEN_MEMORY64)
	set(_of_wasm_default 64)
else()
	set(_of_wasm_default 32)
endif()
set(PROJECT_EMSCRIPTEN_WASM ${_of_wasm_default} CACHE STRING "WebAssembly 32 or 64 (MEMORY64)")
set_property(CACHE PROJECT_EMSCRIPTEN_WASM PROPERTY STRINGS 32 64)
if(NOT PROJECT_EMSCRIPTEN_WASM MATCHES "^(32|64)$")
	message(FATAL_ERROR "PROJECT_EMSCRIPTEN_WASM must be 32 or 64, not \"${PROJECT_EMSCRIPTEN_WASM}\"")
endif()
set(PROJECT_EMSCRIPTEN_TOTAL_MEMORY 134217728 CACHE STRING "Initial wasm memory in bytes")
option(PROJECT_EMSCRIPTEN_EMBIND_AOT "Ahead-of-time embind (CSP safe)" ON)
set(PROJECT_EMSCRIPTEN_TEMPLATE "${OF_LIBS_DIR}/openFrameworksCompiled/project/emscripten/template.html"
	CACHE FILEPATH "HTML shell for the app")
set(OF_WEBGL_VERSION 2 CACHE STRING "WebGL version (1 or 2)")

# wasm64: the core goes in lib/emscripten/WASM64, like the makefiles
if(PROJECT_EMSCRIPTEN_WASM STREQUAL "64")
	set(OF_ARCH WASM64)
endif()

set(OF_PLATFORM_SOURCE_EXTENSIONS cpp cc cxx c)

set(OF_PLATFORM_CORE_EXCLUDES
	"/graphics/ofGraphicsCairo\\.cpp$"
	"/graphics/ofCairoRenderer\\.cpp$"
	"/gl/ofGLRenderer\\.cpp$"
	"/video/ofDirectShow"
	"/video/ofMediaFoundation"
	"/sound/ofMediaFoundation"
	"/video/ofGst"
	"/communication/"
	"/sound/ofFmodSoundPlayer\\.cpp$"
	"/sound/ofOpenALSoundPlayer\\.cpp$"
	"/sound/ofRtAudioSoundStream\\.cpp$"
	"/app/ofAppGlutWindow\\.cpp$"
	"/app/ofAppGLFWWindow\\.cpp$"
	"/app/ofAppEGLWindow\\.cpp$")

set(OF_PLATFORM_DEFINES __EMSCRIPTEN__ URI_STATIC_BUILD TARGET_EMSCRIPTEN)
if(OF_WEBGL_VERSION STREQUAL "1")
	list(APPEND OF_PLATFORM_DEFINES OF_WEBGL_VERSION=1)
endif()

set(OF_PLATFORM_COMPILE_OPTIONS
	-fPIC
	-pthread
	-matomics
	-mbulk-memory
	"$<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wno-warn-absolute-paths>")

set(OF_PLATFORM_LINK_OPTIONS
	--emrun
	--bind
	--profiling-funcs
	--use-port=emdawnwebgpu
	-sAUTO_NATIVE_LIBRARIES=1
	-sAUTO_JS_LIBRARIES=1
	-pthread
	-matomics
	-mbulk-memory
	"SHELL:--js-library ${OF_ADDONS_DIR}/ofxEmscripten/libs/html5video/lib/emscripten/library_html5video.js"
	-sMAIN_MODULE=1
	-sEXPORT_ALL=1
	-sNO_DYNAMIC_EXECUTION=1
	-sABORT_ON_WASM_EXCEPTIONS=0
	-sFILESYSTEM=1
	-sMINIFY_HTML=0
	-sUSE_GLFW=3
	-sEXPORTED_RUNTIME_METHODS=ccall,cwrap,FS,UTF8ToString,stringToUTF8,lengthBytesUTF8
	-sERROR_ON_UNDEFINED_SYMBOLS=1
	-sINITIAL_MEMORY=${PROJECT_EMSCRIPTEN_TOTAL_MEMORY}
	-sALLOW_MEMORY_GROWTH=1
	-sWASM=1
	"SHELL:--shell-file ${PROJECT_EMSCRIPTEN_TEMPLATE}"
	"$<$<CONFIG:Debug>:-sVERBOSE=1;-sGL_ASSERTIONS=1;-sASSERTIONS=2;-gsource-map>")
if(OF_WEBGL_VERSION STREQUAL "1")
	list(APPEND OF_PLATFORM_LINK_OPTIONS -sMAX_WEBGL_VERSION=1 -sFULL_ES2)
else()
	list(APPEND OF_PLATFORM_LINK_OPTIONS -sMAX_WEBGL_VERSION=2 -sWEBGL2_BACKWARDS_COMPATIBILITY_EMULATION=1 -sFULL_ES3)
endif()
if(PROJECT_EMSCRIPTEN_EMBIND_AOT)
	list(APPEND OF_PLATFORM_LINK_OPTIONS -sEMBIND_AOT=1 -sDYNAMIC_EXECUTION=0)
else()
	list(APPEND OF_PLATFORM_LINK_OPTIONS -sDYNAMIC_EXECUTION=1)
endif()

if(PROJECT_EMSCRIPTEN_WASM STREQUAL "64")
	list(APPEND OF_PLATFORM_COMPILE_OPTIONS -sMEMORY64=1)
	list(APPEND OF_PLATFORM_LINK_OPTIONS -sMEMORY64=1 -mwasm64)
endif()

set(OF_PLATFORM_LINK_LIBRARIES glfw)

set(OF_PLATFORM_THIRDPARTY_EXCLUDE glew videoInput fmod openssl curl rtAudio cairo pixman metalangle)
if(NOT OF_USE_DAWN)
	list(APPEND OF_PLATFORM_THIRDPARTY_EXCLUDE dawn)
endif()

set(OF_PLATFORM_INCLUDE_DIRS "${OF_ADDONS_DIR}/ofxEmscripten/src")

set(OF_PLATFORM_LIB_EXTENSIONS a)
set(OF_PLATFORM_LINK_GROUP OFF)
set(OF_PLATFORM_REQUIRED_ADDONS ofxEmscripten)

# the makefiles build the core as libopenFrameworks.o; CMake builds a static
# library next to it
set(OF_CORE_LIB_PREFIX lib)
set(OF_CORE_LIB_NAME openFrameworks)
set(OF_CORE_LIB_NAME_DEBUG openFrameworksDebug)
set(OF_CORE_LIB_SUFFIX .a)

# libs/<name>/lib/emscripten/WASM (WASM64 with PROJECT_EMSCRIPTEN_WASM=64)
function(of_platform_lib_dirs lib_folder out_var)
	if(PROJECT_EMSCRIPTEN_WASM STREQUAL "64")
		set(${out_var} "${lib_folder}" "${lib_folder}/WASM64" PARENT_SCOPE)
	else()
		set(${out_var} "${lib_folder}" "${lib_folder}/WASM" PARENT_SCOPE)
	endif()
endfunction()

# bin/em/<name>/index.html (+ .js .wasm .data), with app.js / style.css /
# .htaccess and bin/data preloaded at /data, like the makefiles
function(of_platform_setup_app target)
	set(out_dir "${CMAKE_CURRENT_SOURCE_DIR}/bin/em/${target}")
	set_target_properties(${target} PROPERTIES
		OUTPUT_NAME index
		OUTPUT_NAME_DEBUG index
		SUFFIX ".html"
		RUNTIME_OUTPUT_DIRECTORY "${out_dir}")
	foreach(config Debug Release RelWithDebInfo MinSizeRel)
		string(TOUPPER ${config} upper)
		set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${upper} "${out_dir}")
	endforeach()

	file(GLOB data_files "${CMAKE_CURRENT_SOURCE_DIR}/bin/data/*")
	if(data_files)
		target_link_options(${target} PRIVATE "SHELL:--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/bin/data@data")
	endif()

	set(project_dir "${OF_LIBS_DIR}/openFrameworksCompiled/project/emscripten")
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${project_dir}/app.js" "${project_dir}/style.css" "${project_dir}/.htaccess" "${out_dir}/"
		VERBATIM)
endfunction()
