# macOS (mirrors libs/openFrameworksCompiled/project/macos/config.macos.default.mk)

include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/apple.cmake")

if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
	set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0 CACHE STRING "Minimum macOS version" FORCE)
endif()

set(OF_PLATFORM_SOURCE_EXTENSIONS cpp cc cxx c mm m)

set(OF_PLATFORM_CORE_EXCLUDES
	"/app/ofAppGlutWindow\\.cpp$"
	"/app/ofAppEGLWindow\\.cpp$"
	"/video/ofDirectShow"
	"/video/ofMediaFoundation"
	"/sound/ofMediaFoundation"
	"/video/ofGst"
	"/sound/ofFmodSoundPlayer\\.cpp$")

set(OF_PLATFORM_DEFINES __MACOSX_CORE__ USE_FMOD=0)

set(OF_PLATFORM_COMPILE_OPTIONS
	-Wall
	-Werror=return-type
	-fexceptions
	-fobjc-arc
	# like the makefiles, C++ is compiled as Objective-C++ so it can use Cocoa
	"$<$<COMPILE_LANGUAGE:CXX>:-xobjective-c++>")

set(OF_PLATFORM_LINK_OPTIONS)

set(OF_PLATFORM_LINK_LIBRARIES objc)
foreach(framework
		Accelerate AppKit ApplicationServices AVFoundation AudioToolbox Cocoa
		CoreVideo CoreAudio CoreMedia CoreFoundation CoreServices Metal Foundation
		IOKit OpenGL QuartzCore Security CFNetwork SystemConfiguration)
	list(APPEND OF_PLATFORM_LINK_LIBRARIES "-framework ${framework}")
endforeach()

# Dawn / MetalANGLE are opt-in renderers, not core dependencies
set(OF_PLATFORM_THIRDPARTY_EXCLUDE)
if(NOT OF_USE_DAWN)
	list(APPEND OF_PLATFORM_THIRDPARTY_EXCLUDE dawn)
endif()
if(NOT OF_USE_ANGLE)
	list(APPEND OF_PLATFORM_THIRDPARTY_EXCLUDE metalangle)
endif()

set(OF_PLATFORM_LIB_EXTENSIONS a)
set(OF_PLATFORM_LINK_GROUP OFF)

set(OF_CORE_LIB_PREFIX "")  # same as the Xcode core project: lib/macos/openFrameworks.a
set(OF_CORE_LIB_NAME openFrameworks)
set(OF_CORE_LIB_NAME_DEBUG openFrameworksDebug)
set(OF_CORE_LIB_SUFFIX .a)

# libs/<name>/lib/macos holds .a files and/or xcframeworks; use each
# xcframework's macOS slice
function(of_platform_lib_dirs lib_folder out_var)
	set(dirs "${lib_folder}")
	of_apple_xcframeworks(xcframeworks "${lib_folder}")
	foreach(xcf IN LISTS xcframeworks)
		file(GLOB slices LIST_DIRECTORIES true "${xcf}/macos-*")
		list(APPEND dirs ${slices})
	endforeach()
	set(${out_var} ${dirs} PARENT_SCOPE)
endfunction()

# turns the app into bin/<name>.app (bin/<name>_debug.app for Debug), with the
# Info.plist and icon the makefiles use
function(of_platform_setup_app target)
	of_apple_info_plist(${target}
		"${CMAKE_CURRENT_SOURCE_DIR}/openFrameworks-Info.plist"
		"${OF_ROOT}/scripts/templates/osx/openFrameworks-Info.plist")

	set(icon "${CMAKE_CURRENT_SOURCE_DIR}/of.icns")
	if(NOT EXISTS "${icon}")
		set(icon "${OF_LIBS_DIR}/openFrameworksCompiled/project/osx/of.icns")
	endif()
	set_source_files_properties("${icon}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	target_sources(${target} PRIVATE "${icon}")
endfunction()
