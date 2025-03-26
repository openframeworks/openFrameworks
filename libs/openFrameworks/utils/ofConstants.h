#pragma once

// version: ------------------------
#define OF_VERSION_MAJOR 0
#define OF_VERSION_MINOR 12
#define OF_VERSION_PATCH 0
#define OF_VERSION_PRE_RELEASE "master"

// core: ---------------------------
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>

// Set to 1 for compatibility with old projects using ofVec instead of glm
#ifndef OF_USE_LEGACY_VECTOR_MATH
	#define OF_USE_LEGACY_VECTOR_MATH 0
#endif

// This enables glm's old behavior of initializing with non garbage values
#if !defined(GLM_FORCE_CTOR_INIT)
	#define GLM_FORCE_CTOR_INIT
#endif

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
	OF_TARGET_EMSCRIPTEN,
	OF_TARGET_LINUXAARCH64,
    OF_TARGET_MACOS,
};


#ifndef OF_TARGET_IPHONE
    #define OF_TARGET_IPHONE OF_TARGET_IOS
#endif


// FIXME: not used anymore in OF Core. Only kept for addons compatibility - 20231206
// Cross-platform deprecation warning
#ifdef __GNUC__
	// clang also has this defined. deprecated(message) is only for gcc>=4.5
	#if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || __GNUC__ > 4
        #define OF_DEPRECATED_MSG(message, ...) __VA_ARGS__ __attribute__ ((deprecated(message)))
    #else
        #define OF_DEPRECATED_MSG(message, ...) __VA_ARGS__ __attribute__ ((deprecated))
    #endif
	#define OF_DEPRECATED(...) __VA_ARGS__ __attribute__ ((deprecated))
	#define OF_INTERNAL_DEPRECATED(...) __VA_ARGS__ __attribute__ ((deprecated("OF core deprecated")))
#elif defined(_MSC_VER)
	#define OF_DEPRECATED_MSG(message, ...) __declspec(deprecated(message)) __VA_ARGS__
	#define OF_DEPRECATED(...) __declspec(deprecated) __VA_ARGS__
	#define OF_INTERNAL_DEPRECATED(...) __declspec(deprecated("OF core deprecated")) __VA_ARGS__
#else
	#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
	#define OF_DEPRECATED_MSG(message, ...) __VA_ARGS__
	#define OF_DEPRECATED(...) __VA_ARGS__
#endif

//-------------------------------
//  find the system type --------
//-------------------------------

// 		helpful:
// 		http://www.ogre3d.org/docs/api/html/OgrePlatform_8h-source.html

#if defined( __WIN32__ ) || defined( _WIN32 )
	#define OF_OS_WINDOWS
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
	#if (TARGET_OS_IPHONE || TARGET_OS_IOS || TARGET_OS_SIMULATOR || TARGET_IPHONE_SIMULATOR) && !TARGET_OS_TV && !TARGET_OS_WATCH && !TARGET_OS_MACCATALYST && !TARGET_OS_VISION
        #define TARGET_OF_IPHONE
        #define TARGET_OF_IOS
        #define TARGET_OPENGLES
    #elif TARGET_OS_TV
        #define TARGET_OF_IOS
        #define TARGET_OF_TVOS
        #define TARGET_OPENGLES
        #define TARGET_IMPLEMENTS_URL_LOADER
    #elif TARGET_OS_WATCH
        #define TARGET_OF_IOS
        #define TARGET_OF_WATCHOS
        #define TARGET_OPENGLES
        #define TARGET_IMPLEMENTS_URL_LOADER
    #elif TARGET_OS_VISION
        #define TARGET_OF_IOS
        #define TARGET_OF_XROS
    #elif TARGET_OS_MACCATALYST
        #define TARGET_OF_IOS
        #define TARGET_OF_MACCATALYST
	#else
		#define TARGET_OSX
        #define TARGET_MAC
        #define TARGET_OF_MAC
	#endif
#elif defined (__ANDROID__)
	#define TARGET_ANDROID
	#define TARGET_OPENGLES
//#elif defined(__ARMEL__)
#elif defined(__ARM__)
	#define TARGET_LINUX
	#define TARGET_OPENGLES
	#define TARGET_LINUX_ARM
#elif defined(__EMSCRIPTEN__)
#ifndef TARGET_EMSCRIPTEN
	#define TARGET_EMSCRIPTEN
#endif
	#define TARGET_OPENGLES
	#define TARGET_PROGRAMMABLE_GL
	#define TARGET_IMPLEMENTS_URL_LOADER
#else
	#define TARGET_LINUX
#endif
//-------------------------------


// then the the platform specific includes:
#ifdef TARGET_WIN32
	#define GLEW_STATIC
	#define GLEW_NO_GLU
    #define TARGET_GLFW_WINDOW
    #define OF_CAIRO
	#include "GL/glew.h"
	#include "GL/wglew.h"
	#define OF_RTAUDIO
	#define __WINDOWS_DS__
	#define __WINDOWS_WASAPI__
	#define __WINDOWS_ASIO__
	#define __WINDOWS_MM__ // rtMidi?
	#if (_MSC_VER)       // microsoft visual studio
		//TODO: Fix this in the code instead of disabling the warnings
		#define _CRT_SECURE_NO_WARNINGS
		#define _WINSOCK_DEPRECATED_NO_WARNINGS

		#include <stdint.h>
		#include <functional>
		#pragma warning(disable : 4068)		// unknown pragmas
		#pragma warning(disable : 4756)		// overflow in constant arithmetic
		#pragma warning(disable : 4800)		// 'Boolean' : forcing value to bool 'true' or 'false'

		// make microsoft visual studio complain less about double / float conversion and
		// truncation
		#pragma warning(disable : 4244)
		#pragma warning(disable : 4305)
		// warnings: http://msdn.microsoft.com/library/2c8f766e.aspx

		// NOMINMAX doesn't seem to work anymore in vs2015 so let's just remove them
		#undef min
		#undef max
	#endif

	#define TARGET_LITTLE_ENDIAN			// intel cpu

	// some gl.h files, like dev-c++, are old - this is pretty universal
	#ifndef GL_BGR_EXT
	#define GL_BGR_EXT 0x80E0
	#endif

	#define WIN32_HIGH_RES_TIMING

	// note: this is experimental!
	// uncomment to turn this on (only for windows machines)
	// if you want to try setting the timer to be high resolution
	// this could make camera grabbing and other low level
	// operations quicker, but you must quit the app normally,
	// ie, using "esc", rather than killing the process or closing
	// the console window in order to set the timer resolution back
	// to normal (since the high res timer might give the OS
	// problems)
	// info: http://www.geisswerks.com/ryan/FAQS/timing.html

#endif

#if defined(TARGET_OS_OSX) && !defined(TARGET_OF_IOS)
    #define TARGET_GLFW_WINDOW
    #define OF_CAIRO
    #define OF_RTAUDIO
	#ifndef __MACOSX_CORE__
		#define __MACOSX_CORE__ // rtAudio
	#endif
	#ifndef OF_NO_FMOD
		#define OF_NO_FMOD
	#endif
	#include "GL/glew.h"
    #include "OpenGL/OpenGL.h"

	#if defined(__LITTLE_ENDIAN__)
		#define TARGET_LITTLE_ENDIAN		// intel cpu
	#endif

	#if defined(__OBJC__) && !__has_feature(objc_arc)
		#warning "Please enable ARC (Automatic Reference Counting) at the project level"
	#endif
#endif

#ifdef TARGET_LINUX

	#ifdef TARGET_LINUX_ARM
		#ifdef TARGET_RASPBERRY_PI
			#include <bcm_host.h>
			// rpi firmware headers define countof
			// which messes up other libraries like glm
			#undef countof
		#endif

		#include <GLES/gl.h>
		#include <GLES/glext.h>
		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>

		#define EGL_EGLEXT_PROTOTYPES
		#include <EGL/egl.h>
		#include <EGL/eglext.h>
	#else // desktop linux
        #define TARGET_GLFW_WINDOW
        #define OF_RTAUDIO
		#ifndef __LINUX_PULSE__
			#define __LINUX_PULSE__
		#endif
		#ifndef __LINUX_ALSA__
			#define __LINUX_ALSA__
		#endif
		#define __LINUX_OSS__
		#include <GL/glew.h>
	#endif

	// for some reason, this isn't defined at compile time,
	// so this hack let's us work
	// for 99% of the linux folks that are on intel
	// everyone one else will have RGB / BGR issues.
	//#if defined(__LITTLE_ENDIAN__)
	#define TARGET_LITTLE_ENDIAN		// intel cpu
	//#endif
	// some things for serial compilation:
	#define B14400	14400
	#define B28800	28800

#endif


#ifdef TARGET_OF_IOS
    #import <OpenGLES/ES1/gl.h>
    #import <OpenGLES/ES1/glext.h>
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
	#import <OpenGLES/ES3/gl.h>
	#import <OpenGLES/ES3/glext.h>
    #define TARGET_LITTLE_ENDIAN    // arm cpu
    #if defined(__OBJC__) && !__has_feature(objc_arc)
        #warning "ARC (Automatic Reference Counting) is not enabled."
        #warning "Enable ARC at the project level, or if using Objective-C/C++ with manual memory management,"
        #warning "add '-fno-objc-arc' in Build Phases -> Compile Sources -> Compiler Flags."
    #endif
#endif

#ifdef TARGET_ANDROID
	#include <typeinfo>
	#include <GLES/gl.h>
	#define GL_GLEXT_PROTOTYPES
	#include <GLES/glext.h>

	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>

	#define TARGET_LITTLE_ENDIAN
#endif

#ifdef TARGET_EMSCRIPTEN
	#define GL_GLEXT_PROTOTYPES
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <GLES3/gl3.h>
	#include <GL/glew.h>

	#include "EGL/egl.h"
	#include "EGL/eglext.h"

	#define TARGET_LITTLE_ENDIAN
#endif

#include <tesselator.h>
typedef TESSindex ofIndexType;



#define OF_EXIT_APP(val)		std::exit(val);



#if (defined(_M_ARM64) || defined(_M_ARM64EC)) && defined(TARGET_WIN32)
	#undef USE_FMOD // No FMOD lib for ARM64 yet
	#ifndef OF_NO_FMOD
		#define OF_NO_FMOD
	#endif
	#include <arm64_neon.h> // intrinsics SIMD on https://learn.microsoft.com/en-us/cpp/intrinsics/arm64-intrinsics?view=msvc-170
#endif

//------------------------------------------------ soundplayer
//MAC_OS and IOS uncomment to enable AVEnginePlayer
#ifdef OF_NO_FMOD
    #undef USE_FMOD
    #if defined(TARGET_OF_IOS) || defined(TARGET_OSX)
        #define OF_SOUND_PLAYER_AV_ENGINE
    #elif defined(TARGET_WIN32)
		#define OF_SOUND_PLAYER_MEDIA_FOUNDATION
	#endif
#endif

// check if any soundplayer api is defined from the compiler

#if !defined(TARGET_NO_SOUND)
#if !defined(OF_SOUND_PLAYER_QUICKTIME) && !defined(OF_SOUND_PLAYER_FMOD) && !defined(OF_SOUND_PLAYER_OPENAL) && !defined(OF_SOUND_PLAYER_EMSCRIPTEN) && !defined(OF_SOUND_PLAYER_AV_ENGINE) && !defined(OF_SOUND_PLAYER_MEDIA_FOUNDATION)
  #ifdef TARGET_OF_IOS
  	#define OF_SOUND_PLAYER_IPHONE
  #elif defined(TARGET_LINUX) || defined(TARGET_MINGW)
  	#define OF_SOUND_PLAYER_OPENAL
  #elif defined(TARGET_EMSCRIPTEN)
	#define OF_SOUND_PLAYER_EMSCRIPTEN
  #elif !defined(TARGET_ANDROID) && (!defined(USE_FMOD) || USE_FMOD)
  	#define OF_SOUND_PLAYER_FMOD
  #endif
#endif

#endif

//------------------------------------------------ thread local storage
// clang has a bug where it won't support tls on some versions even
// on c++11, this is a workaround that bug
#ifndef HAS_TLS
	#if defined(__clang__) && __clang__
		#if __has_feature(cxx_thread_local) && !defined(__MINGW64__) && !defined(__MINGW32__) && !defined(__ANDROID__) && !defined(TARGET_OF_IOS)
			#define HAS_TLS 1
		#endif
    #elif !defined(TARGET_WIN32) || _MSC_VER
		#define HAS_TLS 1
	#endif
#endif



// If you are building with c++17 or newer std filesystem will be enabled by default
#if __cplusplus >= 201500
// #pragma message ( "__cplusplus >= 201500 " )
    #define OF_HAS_CPP17
    #if __cplusplus < 201703L
		// #pragma message ( "__cplusplus < 201703L" )
        #define OF_USE_EXPERIMENTAL_FS
    #endif
#else
    #undef OF_HAS_CPP17
#endif


#ifndef OF_USING_STD_FS
	#if defined(OF_HAS_CPP17)
		#define OF_USING_STD_FS
	#else
		#undef OF_USING_STD_FS
	#endif
#endif

// Some projects will specify OF_USING_STD_FS even if the compiler isn't newer than 201703L
// This may be okay but we need to test for the way C++17 is including the filesystem

#if defined(OF_USING_STD_FS) && !defined(OF_USE_EXPERIMENTAL_FS)
    #if defined(__cpp_lib_filesystem)
		// #pragma message ( "ok __cpp_lib_filesystem" )
        #undef OF_USE_EXPERIMENTAL_FS
    #elif defined(__cpp_lib_experimental_filesystem)
		// #pragma message ( "ok __cpp_lib_experimental_filesystem" )
        #define OF_USE_EXPERIMENTAL_FS
    #elif !defined(__has_include)
		// #pragma message ( "not __has_include so we add OF_USE_EXPERIMENTAL_FS? seems wrong" )
        #define OF_USE_EXPERIMENTAL_FS
    #elif __has_include(<filesystem>)
        // If we're compiling on Visual Studio and are not compiling with C++17, we need to use experimental
        #ifdef _MSC_VER

            // Check and include header that defines "_HAS_CXX17"
            #if __has_include(<yvals_core.h>)
                #include <yvals_core.h>

                // Check for enabled C++17 support
                #if defined(_HAS_CXX17) && _HAS_CXX17
                // We're using C++17, so let's use the normal version
                    #undef OF_USE_EXPERIMENTAL_FS
                #endif
            #endif

            // If the macro isn't defined yet, that means any of the other VS specific checks failed, so we need to use experimental
            #ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
                #define OF_USE_EXPERIMENTAL_FS
            #endif

        // Not on Visual Studio. Let's use the normal version
        #else // #ifdef _MSC_VER
            #undef OF_USE_EXPERIMENTAL_FS
        #endif
    #else
        #undef OF_USE_EXPERIMENTAL_FS
    #endif
#endif


#if defined(OF_USE_EXPERIMENTAL_FS)
	// C++17 experimental fs support
	#include <experimental/filesystem>
	namespace std {
		namespace experimental{
			namespace filesystem {
				using path = v1::path;
			}
		}
	}

	namespace of {
		namespace filesystem = std::experimental::filesystem;
	}
#else
	#include <filesystem>
	namespace of {
		namespace filesystem = std::filesystem;
	}
#endif
