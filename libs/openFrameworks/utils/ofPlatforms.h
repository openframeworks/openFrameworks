#pragma once

//-------------------------------

/// \brief This enumerates the targeted operating systems or platforms.
enum ofTargetPlatform{
	/// \brief 32- and 64-bit x86 architecture on Mac OSX.
	OF_TARGET_OSX,
	/// \brief 32- and 64-bit x86 architecture using MinGW on Windows OS.
    OF_TARGET_MINGW,
	/// \brief 32- and 64-bit x86 architecture using Visual Studio on Windows OS.
	OF_TARGET_WINVS,
	/// \brief 32- and 64-bit armv7, arm64, x86 (simulator) architecture Mac iOS.
	OF_TARGET_IOS,
	/// \brief 32- and 64-bit armeabi-v7a and x86 Android OS.
	OF_TARGET_ANDROID,
	/// \brief 32-bit x86 architecture on Linux OS.
	OF_TARGET_LINUX,
	/// \brief 64-bit x86 architecture on Linux OS.
	OF_TARGET_LINUX64,
	/// \brief 32-bit armv6 little endian architecture on Linux OS.
	OF_TARGET_LINUXARMV6L,
	/// \brief 32-bit armv7 little endian architecture on Linux OS.
	OF_TARGET_LINUXARMV7L,
	/// \brief Compiled to javascript using Emscripten.
	/// \sa https://github.com/kripken/emscripten
	OF_TARGET_EMSCRIPTEN
};


#ifndef OF_TARGET_IPHONE
    #define OF_TARGET_IPHONE OF_TARGET_IOS
#endif


//-------------------------------
//  find the system type --------
//-------------------------------

// 		helpful:
// 		http://www.ogre3d.org/docs/api/html/OgrePlatform_8h-source.html

#if defined( __WIN32__ ) || defined( _WIN32 )
	#define TARGET_WIN32
	#if defined(_MSC_VER)
		#define TARGET_WINVS
	#endif
	#if defined(__MINGW32__) || defined(__MINGW64__)
		#define TARGET_MINGW
	#endif
#elif defined( __APPLE_CC__)
    #define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0
    #include <TargetConditionals.h>
	#if (TARGET_OS_IPHONE || TARGET_OS_IOS || TARGET_OS_SIMULATOR || TARGET_IPHONE_SIMULATOR) && !TARGET_OS_TV && !TARGET_OS_WATCH
        #define TARGET_OF_IPHONE
        #define TARGET_OF_IOS
        #define TARGET_OPENGLES
        #include <unistd.h>
    #elif TARGET_OS_TV
        #define TARGET_OF_IOS
        #define TARGET_OF_TVOS
        #define TARGET_OPENGLES
        #include <unistd.h>
    #elif TARGET_OS_WATCH
        #define TARGET_OF_IOS
        #define TARGET_OF_WATCHOS
        #define TARGET_OPENGLES
        #include <unistd.h>
	#else
		#define TARGET_OSX
	#endif
#elif defined (__ANDROID__)
	#define TARGET_ANDROID
	#define TARGET_OPENGLES
#elif defined(__ARMEL__)
	#define TARGET_LINUX
	#define TARGET_OPENGLES
	#define TARGET_LINUX_ARM
#elif defined(__EMSCRIPTEN__)
	#define TARGET_EMSCRIPTEN
	#define TARGET_OPENGLES
	#define TARGET_PROGRAMMABLE_GL
	#define TARGET_IMPLEMENTS_URL_LOADER
#else
	#define TARGET_LINUX
#endif
//-------------------------------
