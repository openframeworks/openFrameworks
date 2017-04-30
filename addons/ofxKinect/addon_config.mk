# All variables and this file are optional, if they are not present the PG and the
# makefiles will try to parse the correct values from the file system.
#
# Variables that specify exclusions can use % as a wildcard to specify that anything in
# that position will match. A partial path can also be specified to, for example, exclude
# a whole folder from the parsed paths from the file system
#
# Variables can be specified using = or +=
# = will clear the contents of that variable both specified from the file or the ones parsed
# from the file system
# += will add the values to the previous ones in the file or the ones parsed from the file
# system
#
# The PG can be used to detect errors in this file, just create a new project with this addon
# and the PG will write to the console the kind of error and in which line it is

meta:
	ADDON_NAME = ofxKinect
	ADDON_DESCRIPTION = Addon for interfacing with microsoft kinect cameras
	ADDON_AUTHOR = Dan Wilcox, Theo Watson, Kyle McDonald, Arturo Castro
	ADDON_TAGS = "computer vision" "3D sensing" "kinect"
	ADDON_URL = http://github.com/ofTheo/ofxKinect

common:
	# dependencies with other addons, a list of them separated by spaces
	# or use += in several lines
	# ADDON_DEPENDENCIES =

	# include search paths, this will be usually parsed from the file system
	# but if the addon or addon libraries need special search paths they can be
	# specified here separated by spaces or one per line using +=
	# ADDON_INCLUDES =

	# any special flag that should be passed to the compiler when using this
	# addon
	# ADDON_CFLAGS =

	# any special flag that should be passed to the linker when using this
	# addon, also used for system libraries with -lname
	# ADDON_LDFLAGS =

	# linux only, any library that should be included in the project using
	# pkg-config
	# ADDON_PKG_CONFIG_LIBRARIES =

	# osx/iOS only, any framework that should be included in the project
	# ADDON_FRAMEWORKS =

	# source files, these will be usually parsed from the file system looking
	# in the src folders in libs and the root of the addon. if your addon needs
	# to include files in different places or a different set of files per platform
	# they can be specified here
	# ADDON_SOURCES =

	# some addons need resources to be copied to the bin/data folder of the project
	# specify here any files that need to be copied, you can use wildcards like * and ?
	# ADDON_DATA =

	# when parsing the file system looking for libraries exclude this for all or
	# a specific platform
	# ADDON_LIBS_EXCLUDE =

	# when parsing the file system looking for sources exclude this for all or
	# a specific platform
	# ADDON_SOURCES_EXCLUDE =

	# when parsing the file system looking for include paths exclude this for all or
	# a specific platform
	# ADDON_INCLUDES_EXCLUDE =

linux64:
	# linux only, any library that should be included in the project using
	# pkg-config
	ADDON_PKG_CONFIG_LIBRARIES = libusb-1.0

	# when parsing the file system looking for sources exclude this for all or
	# a specific platform
	ADDON_SOURCES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_SOURCES_EXCLUDE += libs/libusb-win32/%

	# when parsing the file system looking for include paths exclude this for all or
	# a specific platform
	ADDON_INCLUDES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_INCLUDES_EXCLUDE += libs/libusb-win32/%
	ADDON_INCLUDES_EXCLUDE += libs/libusb-1.0/%

linux:
	# linux only, any library that should be included in the project using
	# pkg-config
	ADDON_PKG_CONFIG_LIBRARIES = libusb-1.0

	# when parsing the file system looking for sources exclude this for all or
	# a specific platform
	ADDON_SOURCES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_SOURCES_EXCLUDE += libs/libusb-win32/%

	# when parsing the file system looking for include paths exclude this for all or
	# a specific platform
	ADDON_INCLUDES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_INCLUDES_EXCLUDE += libs/libusb-win32/%
	ADDON_INCLUDES_EXCLUDE += libs/libusb-1.0/%

msys2:
	# any library that should be included in the project using pkg-config
	ADDON_PKG_CONFIG_LIBRARIES = libusb-1.0

	# when parsing the file system looking for sources exclude this for all or
	# a specific platform
	ADDON_SOURCES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_SOURCES_EXCLUDE += libs/libusb-win32/%

	# when parsing the file system looking for include paths exclude this for all or
	# a specific platform
	ADDON_INCLUDES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_INCLUDES_EXCLUDE += libs/libusb-win32/%
	ADDON_INCLUDES_EXCLUDE += libs/libusb-1.0/%


vs:
	# source files, these will be usually parsed from the file system looking
	# in the src folders in libs and the root of the addon. if your addon needs
	# to include files in different places or a different set of files per platform
	# they can be specified here
	ADDON_SOURCES += libs/libfreenect/platform/windows/libusb10emu/libusb-1.0/failguard.cpp
	ADDON_SOURCES += libs/libfreenect/platform/windows/libusb10emu/libusb-1.0/libusbemu.cpp

	# include search paths, this will be usually parsed from the file system
	# but if the addon or addon libraries need special search paths they can be
	# specified here separated by spaces or one per line using +=
	ADDON_INCLUDES += libs/libfreenect/platform/windows

	# when parsing the file system looking for include paths exclude this for all or
	# a specific platform
	ADDON_INCLUDES_EXCLUDE += libs/libusb-1.0/%

linuxarmv6l:
	# linux only, any library that should be included in the project using
	# pkg-config
	ADDON_PKG_CONFIG_LIBRARIES = libusb-1.0

	# when parsing the file system looking for sources exclude this for all or
	# a specific platform
	ADDON_SOURCES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_SOURCES_EXCLUDE += libs/libusb-win32/%

	# when parsing the file system looking for include paths exclude this for all or
	# a specific platform
	ADDON_INCLUDES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_INCLUDES_EXCLUDE += libs/libusb-win32/%
	ADDON_INCLUDES_EXCLUDE += libs/libusb-1.0/%


linuxarmv7l:
	# linux only, any library that should be included in the project using
	# pkg-config
	ADDON_PKG_CONFIG_LIBRARIES = libusb-1.0

	# when parsing the file system looking for sources exclude this for all or
	# a specific platform
	ADDON_SOURCES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_SOURCES_EXCLUDE += libs/libusb-win32/%

	# when parsing the file system looking for include paths exclude this for all or
	# a specific platform
	ADDON_INCLUDES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_INCLUDES_EXCLUDE += libs/libusb-win32/%
	ADDON_INCLUDES_EXCLUDE += libs/libusb-1.0/%

android/armeabi:

android/armeabi-v7a:

osx:
	# when parsing the file system looking for sources exclude this for all or
	# a specific platform
	ADDON_SOURCES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_SOURCES_EXCLUDE += libs/libusb-win32/%

	# when parsing the file system looking for include paths exclude this for all or
	# a specific platform
	ADDON_INCLUDES_EXCLUDE = libs/libfreenect/platform/%
	ADDON_INCLUDES_EXCLUDE += libs/libusb-win32/%

ios:


