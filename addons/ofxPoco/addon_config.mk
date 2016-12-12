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
	ADDON_NAME = ofxPoco
	ADDON_DESCRIPTION = Addon to maintain legacy projects still using POCO
	ADDON_AUTHOR = OF Team
	ADDON_TAGS = "poco"
	ADDON_URL = 

common:
	# dependencies with other addons, a list of them separated by spaces 
	# or use += in several lines
	# ADDON_DEPENDENCIES =
	
	# include search paths, this will be usually parsed from the file system
	# but if the addon or addon libraries need special search paths they can be
	# specified here separated by spaces or one per line using +=
	ADDON_INCLUDES = libs/poco/include
	ADDON_INCLUDES += src
	
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


    ADDON_SOURCES_EXCLUDE = libs/poco/include/%

linux:
    ADDON_LIBS = libs/poco/lib/linux/libPocoNetSSL.a
    ADDON_LIBS += libs/poco/lib/linux/libPocoNet.a
    ADDON_LIBS += libs/poco/lib/linux/libPocoCrypto.a
    ADDON_LIBS += libs/poco/lib/linux/libPocoUtil.a
    ADDON_LIBS += libs/poco/lib/linux/libPocoJSON.a
    ADDON_LIBS += libs/poco/lib/linux/libPocoXML.a
    ADDON_LIBS += libs/poco/lib/linux/libPocoFoundation.a

linux64:
    ADDON_LIBS = libs/poco/lib/linux64/libPocoNetSSL.a
    ADDON_LIBS += libs/poco/lib/linux64/libPocoNet.a
    ADDON_LIBS += libs/poco/lib/linux64/libPocoCrypto.a
    ADDON_LIBS += libs/poco/lib/linux64/libPocoUtil.a
    ADDON_LIBS += libs/poco/lib/linux64/libPocoJSON.a
    ADDON_LIBS += libs/poco/lib/linux64/libPocoXML.a
    ADDON_LIBS += libs/poco/lib/linux64/libPocoFoundation.a

linuxarmv6l:
    ADDON_LIBS = libs/poco/lib/linuxarmv6l/libPocoNetSSL.a
    ADDON_LIBS += libs/poco/lib/linuxarmv6l/libPocoNet.a
    ADDON_LIBS += libs/poco/lib/linuxarmv6l/libPocoCrypto.a
    ADDON_LIBS += libs/poco/lib/linuxarmv6l/libPocoUtil.a
    ADDON_LIBS += libs/poco/lib/linuxarmv6l/libPocoJSON.a
    ADDON_LIBS += libs/poco/lib/linuxarmv6l/libPocoXML.a
    ADDON_LIBS += libs/poco/lib/linuxarmv6l/libPocoFoundation.a

linuxarmv7l:
    ADDON_LIBS = libs/poco/lib/linuxarmv7l/libPocoNetSSL.a
    ADDON_LIBS += libs/poco/lib/linuxarmv7l/libPocoNet.a
    ADDON_LIBS += libs/poco/lib/linuxarmv7l/libPocoCrypto.a
    ADDON_LIBS += libs/poco/lib/linuxarmv7l/libPocoUtil.a
    ADDON_LIBS += libs/poco/lib/linuxarmv7l/libPocoJSON.a
    ADDON_LIBS += libs/poco/lib/linuxarmv7l/libPocoXML.a
    ADDON_LIBS += libs/poco/lib/linuxarmv7l/libPocoFoundation.a

android/armeabi-v7a:
    ADDON_LIBS = libs/poco/lib/android/armv7/libPocoNetSSL.a
    ADDON_LIBS += libs/poco/lib/android/armv7/libPocoNet.a
    ADDON_LIBS += libs/poco/lib/android/armv7/libPocoCrypto.a
    ADDON_LIBS += libs/poco/lib/android/armv7/libPocoUtil.a
    ADDON_LIBS += libs/poco/lib/android/armv7/libPocoJSON.a
    ADDON_LIBS += libs/poco/lib/android/armv7/libPocoXML.a
    ADDON_LIBS += libs/poco/lib/android/armv7/libPocoFoundation.a

android/x86:
    ADDON_LIBS = libs/poco/lib/android/x86/libPocoNetSSL.a
    ADDON_LIBS += libs/poco/lib/android/x86/libPocoNet.a
    ADDON_LIBS += libs/poco/lib/android/x86/libPocoCrypto.a
    ADDON_LIBS += libs/poco/lib/android/x86/libPocoUtil.a
    ADDON_LIBS += libs/poco/lib/android/x86/libPocoJSON.a
    ADDON_LIBS += libs/poco/lib/android/x86/libPocoXML.a
    ADDON_LIBS += libs/poco/lib/android/x86/libPocoFoundation.a

osx:
    ADDON_INCLUDES += libs/openssl/include
    ADDON_LIBS = libs/poco/lib/osx/PocoNetSSL.a
    ADDON_LIBS += libs/poco/lib/osx/PocoNet.a
    ADDON_LIBS += libs/poco/lib/osx/PocoCrypto.a
    ADDON_LIBS += libs/poco/lib/osx/PocoUtil.a
    ADDON_LIBS += libs/poco/lib/osx/PocoJSON.a
    ADDON_LIBS += libs/poco/lib/osx/PocoXML.a
    ADDON_LIBS += libs/poco/lib/osx/PocoFoundation.a
    ADDON_LIBS += libs/openssl/lib/osx/crypto.a
    ADDON_LIBS += libs/openssl/lib/osx/ssl.a"

ios:
    ADDON_INCLUDES += libs/openssl/include
    ADDON_LIBS = libs/poco/lib/ios/PocoNetSSL.a
    ADDON_LIBS += libs/poco/lib/ios/PocoNet.a
    ADDON_LIBS += libs/poco/lib/ios/PocoCrypto.a
    ADDON_LIBS += libs/poco/lib/ios/PocoUtil.a
    ADDON_LIBS += libs/poco/lib/ios/PocoJSON.a
    ADDON_LIBS += libs/poco/lib/ios/PocoXML.a
    ADDON_LIBS += libs/poco/lib/ios/PocoFoundation.a
    ADDON_LIBS += libs/openssl/lib/ios/crypto.a
    ADDON_LIBS += libs/openssl/lib/ios/ssl.a"

tvos:
    ADDON_INCLUDES += libs/openssl/include
    ADDON_LIBS = libs/poco/lib/tvos/PocoNetSSL.a
    ADDON_LIBS += libs/poco/lib/tvos/PocoNet.a
    ADDON_LIBS += libs/poco/lib/tvos/PocoCrypto.a
    ADDON_LIBS += libs/poco/lib/tvos/PocoUtil.a
    ADDON_LIBS += libs/poco/lib/tvos/PocoJSON.a
    ADDON_LIBS += libs/poco/lib/tvos/PocoXML.a
    ADDON_LIBS += libs/poco/lib/tvos/PocoFoundation.a
    ADDON_LIBS += libs/openssl/lib/tvos/crypto.a
    ADDON_LIBS += libs/openssl/lib/tvos/ssl.a"

msys2:
    ADDON_LDFLAGS = -lPocoNetSSL
    ADDON_LDFLAGS += -lPocoNet
    ADDON_LDFLAGS += -lPocoCrypto
    ADDON_LDFLAGS += -lPocoUtil
    ADDON_LDFLAGS += -lPocoJSON
    ADDON_LDFLAGS += -lPocoXML
    ADDON_LDFLAGS += -lPocoFoundation
    ADDON_CFLAGS = -DPOCO_STATIC
    
vs: 
    ADDON_CFLAGS = -DPOCO_STATIC
