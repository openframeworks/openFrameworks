# Linux: linux/64 / linux/aarch64 / linux/armv7l / linux/armv6l
# (mirrors makefileCommon/config.linux.common.mk and <platform>/config.<platform>.default.mk)
#
# Most dependencies come from the system through pkg-config; install them with
# scripts/linux/<distro>/install_dependencies.sh.

include_guard(GLOBAL)

find_package(PkgConfig REQUIRED)

set(_of_linux_arm OFF)
if(OF_PLATFORM MATCHES "^linux/armv[67]l$")
	set(_of_linux_arm ON)
endif()

set(OF_PLATFORM_SOURCE_EXTENSIONS cpp cc cxx c)

set(OF_PLATFORM_CORE_EXCLUDES
	"/video/ofDirectShow"
	"/video/ofMediaFoundation"
	"/sound/ofMediaFoundation"
	"/sound/ofFmodSoundPlayer\\.cpp$")
if(_of_linux_arm)
	list(APPEND OF_PLATFORM_CORE_EXCLUDES "/app/ofAppGlutWindow\\.cpp$")
else()
	list(APPEND OF_PLATFORM_CORE_EXCLUDES "/app/ofAppEGLWindow\\.cpp$")
endif()

set(OF_PLATFORM_DEFINES)
set(OF_PLATFORM_COMPILE_OPTIONS -Wall -Werror=return-type)
set(OF_PLATFORM_LINK_OPTIONS "-Wl,-rpath=./libs:./bin/libs:./" -Wl,--as-needed -Wl,--gc-sections)

if(OF_PLATFORM STREQUAL "linux/aarch64")
	list(APPEND OF_PLATFORM_COMPILE_OPTIONS
		-march=armv8-a -mcpu=cortex-a72 -mtune=cortex-a72 -fPIC -ftree-vectorize -Wno-psabi -pipe)
	list(APPEND OF_PLATFORM_LINK_OPTIONS -no-pie)
elseif(OF_PLATFORM STREQUAL "linux/armv7l")
	list(APPEND OF_PLATFORM_COMPILE_OPTIONS
		-march=armv7 -mtune=cortex-a8 -mfpu=neon -mfloat-abi=hard -fPIC -ftree-vectorize -Wno-psabi -pipe)
endif()

set(OF_PLATFORM_LINK_LIBRARIES)

# required pkg-config packages
set(_of_pkgs cairo zlib gstreamer-app-1.0 gstreamer-1.0 gstreamer-video-1.0 gstreamer-base-1.0
	libudev freetype2 fontconfig sndfile openal)
if(NOT _of_linux_arm)
	list(APPEND _of_pkgs libpulse-simple alsa gl glu glew)
endif()
if(OF_PLATFORM MATCHES "^linux/(aarch64|armv7l|armv6l)$")
	list(APPEND _of_pkgs glesv1_cm glesv2 egl)
endif()
foreach(pkg IN LISTS _of_pkgs)
	string(MAKE_C_IDENTIFIER "OF_PC_${pkg}" id)
	pkg_check_modules(${id} REQUIRED IMPORTED_TARGET GLOBAL ${pkg})
	list(APPEND OF_PLATFORM_LINK_LIBRARIES PkgConfig::${id})
endforeach()

# optional pkg-config packages. Not glfw3: the makefile leaves that commented
# out and links libs/glfw. Ubuntu's libglfw3-dev is 3.3.
foreach(pkg openssl libcurl libcurl4 rtaudio)
	string(MAKE_C_IDENTIFIER "OF_PC_${pkg}" id)
	pkg_check_modules(${id} QUIET IMPORTED_TARGET GLOBAL ${pkg})
	if(${id}_FOUND)
		list(APPEND OF_PLATFORM_LINK_LIBRARIES PkgConfig::${id})
	endif()
endforeach()
if(NOT OF_PC_rtaudio_FOUND)
	list(APPEND OF_PLATFORM_LINK_LIBRARIES rtaudio)
endif()

pkg_check_modules(OF_PC_gtk3 QUIET IMPORTED_TARGET GLOBAL gtk+-3.0)
if(OF_PC_gtk3_FOUND)
	list(APPEND OF_PLATFORM_LINK_LIBRARIES PkgConfig::OF_PC_gtk3)
	list(APPEND OF_PLATFORM_DEFINES OF_USING_GTK)
else()
	pkg_check_modules(OF_PC_gtk2 QUIET IMPORTED_TARGET GLOBAL gtk+-2.0)
	if(OF_PC_gtk2_FOUND)
		list(APPEND OF_PLATFORM_LINK_LIBRARIES PkgConfig::OF_PC_gtk2)
		list(APPEND OF_PLATFORM_DEFINES OF_USING_GTK)
	endif()
endif()
pkg_check_modules(OF_PC_mpg123 QUIET IMPORTED_TARGET GLOBAL libmpg123)
if(OF_PC_mpg123_FOUND)
	list(APPEND OF_PLATFORM_LINK_LIBRARIES PkgConfig::OF_PC_mpg123)
	list(APPEND OF_PLATFORM_DEFINES OF_USING_MPG123)
endif()

# plain system libraries
if(NOT _of_linux_arm)
	list(APPEND OF_PLATFORM_LINK_LIBRARIES glut)
endif()
if(NOT OF_PLATFORM STREQUAL "linux/armv6l")
	list(APPEND OF_PLATFORM_LINK_LIBRARIES X11 Xrandr Xxf86vm Xi Xcursor dl pthread)
endif()
list(APPEND OF_PLATFORM_LINK_LIBRARIES freeimage pugixml uriparser)

# These come from the system on Linux, not libs/.
# glfw stays in libs/. Ubuntu's libglfw3-dev is 3.3, and ofAppGLFWWindow
# calls glfwGetPlatform, which the bundled GLFW 3.4 headers declare.
set(OF_PLATFORM_THIRDPARTY_EXCLUDE
	glew cairo videoInput freetype FreeImage assimp rtAudio openssl curl uriparser fmod
	pixman dawn metalangle)

set(OF_PLATFORM_LIB_EXTENSIONS a)
set(OF_PLATFORM_LINK_GROUP ON)

set(OF_CORE_LIB_PREFIX lib)
set(OF_CORE_LIB_NAME openFrameworks)
set(OF_CORE_LIB_NAME_DEBUG openFrameworksDebug)
set(OF_CORE_LIB_SUFFIX .a)

function(of_platform_lib_dirs lib_folder out_var)
	set(${out_var} "${lib_folder}" PARENT_SCOPE)
endfunction()

# bin/<name> (bin/<name>_debug for Debug); nothing else to do
function(of_platform_setup_app target)
endfunction()
