# Shared by macos.cmake and ios.cmake

include_guard(GLOBAL)

enable_language(OBJC)
enable_language(OBJCXX)
set(CMAKE_OBJC_STANDARD 11)
set(CMAKE_OBJCXX_STANDARD 23)

# libs/<name>/lib/<platform>/*.xcframework, except the combined openssl.xcframework
# (libssl / libcrypto have their own)
function(of_apple_xcframeworks out_var lib_folder)
	file(GLOB xcframeworks LIST_DIRECTORIES true "${lib_folder}/*.xcframework")
	list(FILTER xcframeworks EXCLUDE REGEX "/openssl\\.xcframework$")
	list(SORT xcframeworks)
	set(${out_var} ${xcframeworks} PARENT_SCOPE)
endfunction()

# Writes the app's Info.plist from the project's plist (or <fallback> if the
# project has none), filling in the Xcode-style $(VAR) / ${VAR} placeholders
# the way the makefiles do, and sets it on <target>'s bundle.
function(of_apple_info_plist target project_plist fallback_plist)
	get_target_property(name ${target} OUTPUT_NAME)
	if(NOT name)
		set(name ${target})
	endif()
	set(plist_text "")
	if(EXISTS "${project_plist}")
		file(READ "${project_plist}" plist_text)
	endif()
	if(NOT plist_text MATCHES "CFBundleExecutable")
		file(READ "${fallback_plist}" plist_text)
	endif()
	string(MAKE_C_IDENTIFIER "${name}" bundle_name)
	string(REPLACE "_" "-" bundle_name "${bundle_name}")
	set(identifier "cc.openFrameworks.${bundle_name}")
	foreach(pair
			"DEVELOPMENT_LANGUAGE=English"
			"ICON_NAME=of.icns"
			"PRODUCT_BUNDLE_IDENTIFIER=${identifier}"
			"TARGET_NAME=${name}"
			"PRODUCT_NAME=${name}"
			"MARKETING_VERSION=1.0"
			"CURRENT_PROJECT_VERSION=1"
			"HIGH_RESOLUTION_CAPABLE=true"
			"MACOSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}")
		string(REGEX REPLACE "=.*" "" key "${pair}")
		string(REGEX REPLACE "^[^=]*=" "" value "${pair}")
		string(REPLACE "\${${key}}" "${value}" plist_text "${plist_text}")
		string(REPLACE "$(${key})" "${value}" plist_text "${plist_text}")
	endforeach()
	# the executable name differs per configuration (<name>_debug), so CMake fills it in
	string(REPLACE "\${EXECUTABLE_NAME}" "\${MACOSX_BUNDLE_EXECUTABLE_NAME}" plist_text "${plist_text}")
	string(REPLACE "$(EXECUTABLE_NAME)" "\${MACOSX_BUNDLE_EXECUTABLE_NAME}" plist_text "${plist_text}")
	set(generated "${CMAKE_CURRENT_BINARY_DIR}/${target}-Info.plist.in")
	file(WRITE "${generated}" "${plist_text}")
	set_target_properties(${target} PROPERTIES
		MACOSX_BUNDLE ON
		MACOSX_BUNDLE_INFO_PLIST "${generated}"
		MACOSX_BUNDLE_GUI_IDENTIFIER "${identifier}"
		XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${identifier}")
endfunction()
