# Windows, MSYS2 (clang64 / ucrt64 / mingw64 / clangarm64)
# (mirrors libs/openFrameworksCompiled/project/msys2/config.msys2.default.mk)
#
# From an MSYS2 shell, with the dependencies from scripts/msys2/install_dependencies.sh:
#   cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build build

include_guard(GLOBAL)

find_package(PkgConfig REQUIRED)
enable_language(RC)  # windres, for the app icon

set(OF_PLATFORM_SOURCE_EXTENSIONS cpp cc cxx c)

set(OF_PLATFORM_CORE_EXCLUDES
	"/sound/ofMediaFoundationSoundPlayer\\.cpp$"
	"/video/ofMediaFoundationPlayer\\.cpp$"
	"/video/ofGst"
	"/app/ofAppEGLWindow\\.cpp$")

set(OF_PLATFORM_DEFINES OF_VIDEO_PLAYER_DIRECTSHOW UNICODE _UNICODE)
set(OF_PLATFORM_COMPILE_OPTIONS -Wall -fexceptions)
set(OF_PLATFORM_LINK_OPTIONS)
if(OF_MSYSTEM STREQUAL "mingw64")
	list(APPEND OF_PLATFORM_LINK_OPTIONS "-Wl,--disable-dynamicbase,--disable-high-entropy-va,--default-image-base-low")
endif()

set(OF_PLATFORM_LINK_LIBRARIES)
foreach(pkg cairo zlib openssl freetype2 glew glfw3 libcurl liburiparser nlohmann_json openal pugixml rtaudio)
	string(MAKE_C_IDENTIFIER "OF_PC_${pkg}" id)
	pkg_check_modules(${id} REQUIRED IMPORTED_TARGET GLOBAL ${pkg})
	list(APPEND OF_PLATFORM_LINK_LIBRARIES PkgConfig::${id})
endforeach()
pkg_check_modules(OF_PC_mpg123 QUIET IMPORTED_TARGET GLOBAL libmpg123)
if(OF_PC_mpg123_FOUND)
	pkg_check_modules(OF_PC_sndfile REQUIRED IMPORTED_TARGET GLOBAL sndfile)
	list(APPEND OF_PLATFORM_LINK_LIBRARIES PkgConfig::OF_PC_mpg123 PkgConfig::OF_PC_sndfile)
	list(APPEND OF_PLATFORM_DEFINES OF_USING_MPG123)
endif()
list(APPEND OF_PLATFORM_LINK_LIBRARIES
	ksuser opengl32 gdi32 msimg32 glu32 dsound winmm strmiids
	uuid ole32 oleaut32 setupapi wsock32 ws2_32 Iphlpapi Comdlg32
	freeimage mf mfplat mfuuid mfreadwrite pthread)

# these come from MSYS2 packages, not libs/
set(OF_PLATFORM_THIRDPARTY_EXCLUDE
	glew cairo freetype FreeImage glm json openssl glfw curl pugixml rtAudio uriparser utf8
	pixman dawn metalangle)

set(OF_PLATFORM_LIB_EXTENSIONS a)
set(OF_PLATFORM_LINK_GROUP ON)

set(OF_CORE_LIB_PREFIX lib)
set(OF_CORE_LIB_NAME openFrameworks)
set(OF_CORE_LIB_NAME_DEBUG openFrameworksDebug)
set(OF_CORE_LIB_SUFFIX .a)

# libs/<name>/lib/msys2, and lib/msys2/<environment> where a library has one
function(of_platform_lib_dirs lib_folder out_var)
	set(dirs "${lib_folder}")
	if(OF_MSYSTEM)
		list(APPEND dirs "${lib_folder}/${OF_MSYSTEM}")
	endif()
	set(${out_var} ${dirs} PARENT_SCOPE)
endfunction()

# bin/<name>.exe with the openFrameworks icon (Debug: <name>_debug.exe)
function(of_platform_setup_app target)
	set(icon_rc "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")
	if(EXISTS "${icon_rc}")
		target_sources(${target} PRIVATE "${icon_rc}")
	endif()
endfunction()
