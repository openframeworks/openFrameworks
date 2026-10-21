# iOS and tvOS (mirrors project/ios/CoreOF.xcconfig, project/tvOS/CoreOF.xcconfig and
# the scripts/templates/ios / tvOS app templates)
#
#   cmake -S . -B build-ios -G Xcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator
#   cmake --build build-ios --config Release
#
# Use -DCMAKE_SYSTEM_NAME=tvOS (sysroot appletvsimulator / appletvos) for tvOS, and
# -DCMAKE_OSX_SYSROOT=iphoneos -DOF_APPLE_DEVELOPMENT_TEAM=<team id> for a device.
# Needs CMake 3.28+ (links the libs' xcframeworks directly).

include_guard(GLOBAL)

cmake_minimum_required(VERSION 3.28)
include("${CMAKE_CURRENT_LIST_DIR}/apple.cmake")

set(OF_APPLE_DEVELOPMENT_TEAM "" CACHE STRING "Apple development team id for signing device builds")

if(OF_PLATFORM STREQUAL "tvos")
	set(_of_ios_name tvOS)
	if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
		set(CMAKE_OSX_DEPLOYMENT_TARGET 18.0 CACHE STRING "Minimum tvOS version" FORCE)
	endif()
else()
	set(_of_ios_name iOS)
	if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
		set(CMAKE_OSX_DEPLOYMENT_TARGET 15.0 CACHE STRING "Minimum iOS version" FORCE)
	endif()
endif()

# device and simulator builds of the core go in separate folders:
# lib/ios/iphoneos, lib/ios/iphonesimulator, lib/tvos/appletvos, lib/tvos/appletvsimulator
string(TOLOWER "${CMAKE_OSX_SYSROOT}" _of_sysroot)
if(_of_sysroot MATCHES "simulator")
	set(_of_simulator ON)
else()
	set(_of_simulator OFF)
endif()
if(OF_PLATFORM STREQUAL "tvos")
	set(OF_ARCH appletvos)
	if(_of_simulator)
		set(OF_ARCH appletvsimulator)
	endif()
else()
	set(OF_ARCH iphoneos)
	if(_of_simulator)
		set(OF_ARCH iphonesimulator)
	endif()
endif()

set(OF_PLATFORM_SOURCE_EXTENSIONS cpp cc cxx c mm m)

# the iOS core library is libs/openFrameworks + addons/ofxiOS
set(OF_PLATFORM_CORE_EXTRA_SOURCE_DIRS "${OF_ADDONS_DIR}/ofxiOS/src")
of_dir_and_subdirs(OF_PLATFORM_INCLUDE_DIRS "${OF_ADDONS_DIR}/ofxiOS/src")

set(OF_PLATFORM_CORE_EXCLUDES
	"/libs/openFrameworks/app/ofAppGLFWWindow\\.cpp$"
	"/libs/openFrameworks/app/ofAppEGLWindow\\.cpp$"
	"/libs/openFrameworks/app/ofAppGlutWindow\\.cpp$"
	"/libs/openFrameworks/app/ofAppNoWindow\\.cpp$"
	"/libs/openFrameworks/communication/"
	"/libs/openFrameworks/graphics/ofCairoRenderer\\.cpp$"
	"/libs/openFrameworks/graphics/ofGraphicsCairo\\.cpp$"
	"/libs/openFrameworks/sound/ofAVEngineSoundPlayer\\.mm$"
	"/libs/openFrameworks/sound/ofOpenALSoundPlayer\\.cpp$"
	"/libs/openFrameworks/sound/ofRtAudioSoundStream\\.cpp$"
	"/libs/openFrameworks/sound/ofFmodSoundPlayer\\.cpp$"
	"/libs/openFrameworks/sound/ofMediaFoundation"
	"/libs/openFrameworks/video/ofAVFoundation"
	"/libs/openFrameworks/video/ofDirectShow"
	"/libs/openFrameworks/video/ofMediaFoundation"
	"/libs/openFrameworks/video/ofGst")

set(OF_PLATFORM_DEFINES GLES_SILENCE_DEPRECATION=1 $<$<COMPILE_LANGUAGE:CXX,OBJCXX>:__MACOSX_CORE__>)
set(OF_PLATFORM_COMPILE_OPTIONS -fobjc-arc -fobjc-weak -Werror=return-type)
set(OF_PLATFORM_LINK_OPTIONS -ObjC)

set(_of_frameworks
	AudioToolbox Accelerate AVFoundation CoreAudio CoreGraphics CoreLocation CoreMedia CoreVideo
	Foundation GameController GLKit MapKit OpenAL OpenGLES UIKit Security CFNetwork QuartzCore Metal)
if(OF_PLATFORM STREQUAL "tvos")
	list(APPEND _of_frameworks MetalKit)
else()
	list(APPEND _of_frameworks CoreMotion CoreHaptics)
endif()
set(OF_PLATFORM_LINK_LIBRARIES)
foreach(framework IN LISTS _of_frameworks)
	list(APPEND OF_PLATFORM_LINK_LIBRARIES "-framework ${framework}")
endforeach()

set(OF_PLATFORM_THIRDPARTY_EXCLUDE dawn metalangle glew glfw rtAudio cairo pixman)

set(OF_PLATFORM_LIB_EXTENSIONS a)
set(OF_PLATFORM_LINK_GROUP OFF)

set(OF_CORE_LIB_PREFIX "")
set(OF_CORE_LIB_NAME openFrameworks${_of_ios_name})
set(OF_CORE_LIB_NAME_DEBUG openFrameworks${_of_ios_name}Debug)
set(OF_CORE_LIB_SUFFIX .a)

function(of_platform_lib_dirs lib_folder out_var)
	set(${out_var} "${lib_folder}" PARENT_SCOPE)
endfunction()

# whole xcframeworks (CMake / Xcode pick the device or simulator slice) from
# lib/ios (lib/tvos) or, like the Xcode core project, the multi-platform ones in
# lib/macos; only those that have a slice for this platform
function(of_platform_static_libs_in out_var lib_folder)
	get_filename_component(lib_root "${lib_folder}" DIRECTORY)
	if(OF_PLATFORM STREQUAL "tvos")
		set(slice_prefix tvos-)
	else()
		set(slice_prefix ios-)
	endif()
	set(result)
	foreach(folder "${lib_folder}" "${lib_root}/macos")
		of_apple_xcframeworks(xcframeworks "${folder}")
		foreach(xcf IN LISTS xcframeworks)
			get_filename_component(xcf_name "${xcf}" NAME)
			file(GLOB slices LIST_DIRECTORIES true "${xcf}/${slice_prefix}*")
			if(slices AND NOT ";${result};" MATCHES ";[^;]*/${xcf_name};")
				list(APPEND result "${xcf}")
			endif()
		endforeach()
	endforeach()
	file(GLOB libs "${lib_folder}/*.a")
	list(APPEND result ${libs})
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

function(of_platform_setup_core target)
	if(OF_APPLE_DEVELOPMENT_TEAM)
		set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${OF_APPLE_DEVELOPMENT_TEAM}")
	endif()
endfunction()

# bin/<name>.app with bin/data in the bundle (like the template's rsync build
# phase) and, with the Xcode generator, the template's mediaAssets
function(of_platform_setup_app target)
	if(OF_PLATFORM STREQUAL "tvos")
		set(plist "${OF_PROJECT_DIR}/ofxtvOS-Info.plist")
		set(fallback "${OF_ROOT}/scripts/templates/tvOS/ofxtvOS-Info.plist")
		set(assets_dir "${OF_PROJECT_DIR}/mediaAssets")
		set(families 3)
	else()
		set(plist "${OF_PROJECT_DIR}/ofxiOS-Info.plist")
		set(fallback "${OF_ROOT}/scripts/templates/ios/ofxiOS-Info.plist")
		set(assets_dir "${OF_PROJECT_DIR}/mediaAssets")
		set(families "1,2")
	endif()
	of_apple_info_plist(${target} "${plist}" "${fallback}")
	set_target_properties(${target} PROPERTIES
		XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "${families}"
		XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon)
	if(OF_APPLE_DEVELOPMENT_TEAM)
		set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${OF_APPLE_DEVELOPMENT_TEAM}")
	endif()

	if(NOT IS_DIRECTORY "${assets_dir}")
		get_filename_component(template_dir "${fallback}" DIRECTORY)
		set(assets_dir "${template_dir}/mediaAssets")
	endif()
	# storyboards and asset catalogs need Xcode's ibtool / actool
	if(CMAKE_GENERATOR STREQUAL "Xcode" AND IS_DIRECTORY "${assets_dir}")
		file(GLOB assets LIST_DIRECTORIES true "${assets_dir}/*.storyboard" "${assets_dir}/*.xcassets")
		target_sources(${target} PRIVATE ${assets})
		set_source_files_properties(${assets} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
		set_property(TARGET ${target} APPEND PROPERTY RESOURCE ${assets})
	endif()

	if(IS_DIRECTORY "${OF_PROJECT_DIR}/bin/data")
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory "${OF_PROJECT_DIR}/bin/data" "$<TARGET_BUNDLE_CONTENT_DIR:${target}>"
			VERBATIM)
	endif()
endfunction()
