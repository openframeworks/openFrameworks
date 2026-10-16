# Windows, Visual Studio / MSVC (mirrors project/vs/openFrameworks{Common,Release,Debug}.props)
#
#   cmake -S . -B build -G "Visual Studio 17 2022" -A x64      (or ARM64 / ARM64EC)
#   cmake --build build --config Release
#
# Ninja from a Developer Command Prompt works too (the arch comes from the compiler).

include_guard(GLOBAL)

set(OF_PLATFORM_SOURCE_EXTENSIONS cpp cc cxx c)

set(OF_PLATFORM_CORE_EXCLUDES
	"/app/ofAppEGLWindow\\.cpp$"
	"/sound/ofOpenALSoundPlayer\\.cpp$"
	"/video/ofGst")

set(OF_PLATFORM_DEFINES
	WIN32 _CONSOLE POCO_STATIC CAIRO_WIN32_STATIC_BUILD DISABLE_SOME_FLOATING_POINT
	URI_STATIC_BUILD CURL_STATICLIB FREEIMAGE_LIB OF_NO_FMOD
	_HAS_STREAM_INSERTION_OPERATORS_DELETED_IN_CXX20
	$<$<CONFIG:Debug>:_DEBUG>)

set(OF_PLATFORM_COMPILE_OPTIONS /Zc:__cplusplus /MP /utf-8)

set(OF_PLATFORM_LINK_OPTIONS
	/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:LIBC.lib
	"$<IF:$<CONFIG:Debug>,/NODEFAULTLIB:msvcrt.lib;/NODEFAULTLIB:libcmt.lib;/NODEFAULTLIB:LIBCMTD.lib;/ignore:4099,/NODEFAULTLIB:LIBCMT.lib>")

set(OF_PLATFORM_LINK_LIBRARIES
	msimg32 OpenGL32 GLu32 kernel32 setupapi Vfw32 comctl32 dsound user32 gdi32 winspool
	comdlg32 advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32 wldap32 crypt32 bcrypt
	Ws2_32 winmm iphlpapi mf mfplat mfuuid mfreadwrite d3d11 xaudio2)

set(OF_PLATFORM_THIRDPARTY_EXCLUDE dawn metalangle)

set(OF_PLATFORM_LIB_EXTENSIONS lib)
set(OF_PLATFORM_LINK_GROUP OFF)

# same as project/vs/openframeworksLib.vcxproj: lib/vs/<arch>/openframeworksLib[_debug].lib
set(OF_CORE_LIB_PREFIX "")
set(OF_CORE_LIB_NAME openframeworksLib)
set(OF_CORE_LIB_NAME_DEBUG openframeworksLib_debug)
set(OF_CORE_LIB_SUFFIX .lib)

function(of_platform_lib_dirs lib_folder out_var)
	set(${out_var} "${lib_folder}/${OF_ARCH}" PARENT_SCOPE)
endfunction()

# libs/<name>/lib/vs/<arch> holds Release and Debug builds side by side
# (rtAudio.lib / rtAudioD.lib, freetype.lib / freetyped.lib), or in
# Release / Debug subfolders; each is linked only in its configuration
function(of_platform_static_libs_in out_var lib_folder)
	set(dir "${lib_folder}/${OF_ARCH}")
	set(result)
	if(IS_DIRECTORY "${dir}/Release" OR IS_DIRECTORY "${dir}/Debug")
		file(GLOB release_libs "${dir}/Release/*.lib")
		file(GLOB debug_libs "${dir}/Debug/*.lib")
		foreach(lib IN LISTS release_libs)
			list(APPEND result "$<$<NOT:$<CONFIG:Debug>>:${lib}>")
		endforeach()
		foreach(lib IN LISTS debug_libs)
			list(APPEND result "$<$<CONFIG:Debug>:${lib}>")
		endforeach()
	endif()
	file(GLOB libs "${dir}/*.lib")
	list(SORT libs)
	foreach(lib IN LISTS libs)
		get_filename_component(name "${lib}" NAME_WE)
		string(REGEX REPLACE "[dD]$" "" base "${name}")
		if(NOT base STREQUAL name AND EXISTS "${dir}/${base}.lib")
			list(APPEND result "$<$<CONFIG:Debug>:${lib}>")
		elseif(EXISTS "${dir}/${name}d.lib" OR EXISTS "${dir}/${name}D.lib")
			list(APPEND result "$<$<NOT:$<CONFIG:Debug>>:${lib}>")
		else()
			list(APPEND result "${lib}")
		endif()
	endforeach()
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

# console app in bin/, like the VS templates (Debug: <name>_debug.exe)
function(of_platform_setup_app target)
	set(icon_rc "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")
	if(EXISTS "${icon_rc}")
		target_sources(${target} PRIVATE "${icon_rc}")
	endif()
	set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT ${target})
endfunction()
