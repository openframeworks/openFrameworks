#pragma once
#include <stdint.h>

//-------------------------------
#define OF_VERSION_MAJOR 0
#define OF_VERSION_MINOR 10
#define OF_VERSION_PATCH 0
#define OF_VERSION_PRE_RELEASE "master"

//-------------------------------

/// \brief Used to represent the available video looping modes.
/// 
/// \sa ofVideoPlayer::setLoopState()
/// \sa ofVideoPlayer::getLoopState()
enum ofLoopType{
	/// \brief Plays the video once without looping.
	OF_LOOP_NONE=0x01,
	/// \brief Plays the video forwards then backwards repeatedly.
	OF_LOOP_PALINDROME=0x02,
	/// \brief Repeats the video over and over.
	OF_LOOP_NORMAL=0x03
};

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

// Cross-platform deprecation warning
#ifdef __GNUC__
	// clang also has this defined. deprecated(message) is only for gcc>=4.5
	#if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || __GNUC__ > 4
        #define OF_DEPRECATED_MSG(message, func) func __attribute__ ((deprecated(message)))
    #else
        #define OF_DEPRECATED_MSG(message, func) func __attribute__ ((deprecated))
    #endif
	#define OF_DEPRECATED(func) func __attribute__ ((deprecated))
	#define OF_INTERNAL_DEPRECATED(func) func __attribute__ ((deprecated("OF core deprecated")))
#elif defined(_MSC_VER)
	#define OF_DEPRECATED_MSG(message, func) __declspec(deprecated(message)) func
	#define OF_DEPRECATED(func) __declspec(deprecated) func
	#define OF_INTERNAL_DEPRECATED(func) __declspec(deprecated("OF core deprecated")) func
#else
	#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
	#define OF_DEPRECATED_MSG(message, func) func
	#define OF_DEPRECATED(func) func
#endif

//-------------------------------
//  find the system type --------
//-------------------------------

// 		helpful:
// 		http://www.ogre3d.org/docs/api/html/OgrePlatform_8h-source.html

#if defined( __WIN32__ ) || defined( _WIN32 )
	#define TARGET_WIN32
#elif defined( __APPLE_CC__)
    #define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0
    #include <TargetConditionals.h>
	#if (TARGET_OS_IPHONE || TARGET_OS_IOS || TARGET_OS_SIMULATOR || TARGET_OS_IPHONE_SIMULATOR) && !TARGET_OS_TV && !TARGET_OS_WATCH
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
	#define TARGET_NO_THREADS
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
	#include "GL/glew.h"
	#include "GL/wglew.h"
   	#include "glu.h"
	#define __WINDOWS_DS__
	#define __WINDOWS_MM__
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

#ifdef TARGET_OSX
	#ifndef __MACOSX_CORE__
		#define __MACOSX_CORE__
	#endif
	#include <unistd.h>
	#include "GL/glew.h"
	#include <OpenGL/gl.h>
	#include <ApplicationServices/ApplicationServices.h>

	#if defined(__LITTLE_ENDIAN__)
		#define TARGET_LITTLE_ENDIAN		// intel cpu
	#endif
#endif

#ifdef TARGET_LINUX

	#include <unistd.h>

	#ifdef TARGET_LINUX_ARM
		#ifdef TARGET_RASPBERRY_PI
			#include "bcm_host.h"
		#endif

		#include "GLES/gl.h"
		#include "GLES/glext.h"
		#include "GLES2/gl2.h"
		#include "GLES2/gl2ext.h"

		#define EGL_EGLEXT_PROTOTYPES
		#include "EGL/egl.h"
		#include "EGL/eglext.h"
	#else // normal linux
		#define GL_GLEXT_PROTOTYPES
		#include <GL/glew.h>
		#include <GL/gl.h>
		#include <GL/glext.h>
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

	
	#define TARGET_LITTLE_ENDIAN		// arm cpu	
#endif

#ifdef TARGET_ANDROID
	#include <typeinfo>
	#include <unistd.h>
	#include <GLES/gl.h>
	#define GL_GLEXT_PROTOTYPES
	#include <GLES/glext.h>

	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>

	#define TARGET_LITTLE_ENDIAN
#endif

#ifdef TARGET_EMSCRIPTEN
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include "EGL/egl.h"
	#include "EGL/eglext.h"

	#define TARGET_LITTLE_ENDIAN
#endif

#include "tesselator.h"
typedef TESSindex ofIndexType;


#ifndef __MWERKS__
#include <cstdlib>
#endif

#define OF_EXIT_APP(val)		std::exit(val);




//------------------------------------------------ capture
// check if any video capture system is already defined from the compiler
#if !defined(OF_VIDEO_CAPTURE_GSTREAMER) && !defined(OF_VIDEO_CAPTURE_QUICKTIME) && !defined(OF_VIDEO_CAPTURE_DIRECTSHOW) && !defined(OF_VIDEO_CAPTURE_ANDROID) && !defined(OF_VIDEO_CAPTURE_IOS)
	#ifdef TARGET_LINUX

		#define OF_VIDEO_CAPTURE_GSTREAMER

	#elif defined(TARGET_OSX)
		//on 10.6 and below we can use the old grabber
		#ifndef MAC_OS_X_VERSION_10_7
			#define OF_VIDEO_CAPTURE_QUICKTIME
		#else
			#define OF_VIDEO_CAPTURE_QTKIT
        #endif

	#elif defined (TARGET_WIN32)

		// comment out this following line, if you'd like to use the
		// quicktime capture interface on windows
		// if not, we default to videoInput library for
		// direct show capture...

		#define OF_SWITCH_TO_DSHOW_FOR_WIN_VIDCAP

		#ifdef OF_SWITCH_TO_DSHOW_FOR_WIN_VIDCAP
			#define OF_VIDEO_CAPTURE_DIRECTSHOW
		#else
			#define OF_VIDEO_CAPTURE_QUICKTIME
		#endif

	#elif defined(TARGET_ANDROID)

		#define OF_VIDEO_CAPTURE_ANDROID

	#elif defined(TARGET_EMSCRIPTEN)

		#define OF_VIDEO_CAPTURE_EMSCRIPTEN

	#elif defined(TARGET_OF_IOS)

		#define OF_VIDEO_CAPTURE_IOS

	#endif
#endif

//------------------------------------------------  video player
// check if any video player system is already defined from the compiler
#if !defined(OF_VIDEO_PLAYER_GSTREAMER) && !defined(OF_VIDEO_PLAYER_IOS) && !defined(OF_VIDEO_PLAYER_DIRECTSHOW) && !defined(OF_VIDEO_PLAYER_QUICKTIME) && !defined(OF_VIDEO_PLAYER_AVFOUNDATION) && !defined(OF_VIDEO_PLAYER_EMSCRIPTEN)
    #ifdef TARGET_LINUX
        #define OF_VIDEO_PLAYER_GSTREAMER
    #elif defined(TARGET_ANDROID)
        #define OF_VIDEO_PLAYER_ANDROID
    #elif defined(TARGET_OF_IOS)
        #define OF_VIDEO_PLAYER_IOS
	#elif defined(TARGET_WIN32)
        #define OF_VIDEO_PLAYER_DIRECTSHOW
    #elif defined(TARGET_OSX)
        //for 10.8 and 10.9 users we use AVFoundation, for 10.7 we use QTKit, for 10.6 users we use QuickTime
        #ifndef MAC_OS_X_VERSION_10_7
            #define OF_VIDEO_PLAYER_QUICKTIME
        #elif !defined(MAC_OS_X_VERSION_10_8)
            #define OF_VIDEO_PLAYER_QTKIT
        #else
            #define OF_VIDEO_PLAYER_AVFOUNDATION
        #endif
    #elif defined(TARGET_EMSCRIPTEN)
        #define OF_VIDEO_PLAYER_EMSCRIPTEN
    #else
        #define OF_VIDEO_PLAYER_QUICKTIME
    #endif
#endif

//------------------------------------------------ soundstream
// check if any soundstream api is defined from the compiler
#if !defined(OF_SOUNDSTREAM_RTAUDIO) && !defined(OF_SOUNDSTREAM_ANDROID) && !defined(OF_SOUNDSTREAM_IOS) && !defined(OF_SOUNDSTREAM_EMSCRIPTEN)
	#if defined(TARGET_LINUX) || defined(TARGET_WIN32) || defined(TARGET_OSX)
		#define OF_SOUNDSTREAM_RTAUDIO
	#elif defined(TARGET_ANDROID)
		#define OF_SOUNDSTREAM_ANDROID
	#elif defined(TARGET_OF_IOS)
		#define OF_SOUNDSTREAM_IOS
	#elif defined(TARGET_EMSCRIPTEN)
		#define OF_SOUNDSTREAM_EMSCRIPTEN
	#endif
#endif

//------------------------------------------------ soundplayer
// check if any soundplayer api is defined from the compiler
#if !defined(OF_SOUND_PLAYER_QUICKTIME) && !defined(OF_SOUND_PLAYER_FMOD) && !defined(OF_SOUND_PLAYER_OPENAL) && !defined(OF_SOUND_PLAYER_EMSCRIPTEN)
  #ifdef TARGET_OF_IOS
  	#define OF_SOUND_PLAYER_IPHONE
  #elif defined(TARGET_LINUX)
  	#define OF_SOUND_PLAYER_OPENAL
  #elif defined(TARGET_EMSCRIPTEN)
	#define OF_SOUND_PLAYER_EMSCRIPTEN
  #elif !defined(TARGET_ANDROID) && (!defined(USE_FMOD) || USE_FMOD)
  	#define OF_SOUND_PLAYER_FMOD
  #endif
#endif

//------------------------------------------------ c++11
// check if the compiler supports c++11. vs hasn't updated the value
// of __cplusplus so we need to check for vs >= 2012 (1700)
#if __cplusplus>=201103 || _MSC_VER >= 1700
#define HAS_CPP11 1
#endif

//------------------------------------------------ thread local storage
// clang has a bug where it won't support tls on some versions even
// on c++11, this is a workaround that bug
#ifndef HAS_TLS
	#if defined(__clang__) && __clang__
		#if __has_feature(cxx_thread_local) && !defined(__MINGW64__) && !defined(__MINGW32__) && !defined(__ANDROID__)
			#define HAS_TLS 1
		#endif
    #elif !defined(TARGET_WIN32) || _MSC_VER
		#define HAS_TLS 1
	#endif
#endif

//we don't want to break old code that uses ofSimpleApp
//so we forward declare ofBaseApp and make ofSimpleApp mean the same thing
class ofBaseApp;
typedef ofBaseApp ofSimpleApp;

// serial error codes
#define OF_SERIAL_NO_DATA 	-2
#define OF_SERIAL_ERROR		-1

// core: ---------------------------
#include <cstdio>
#include <cstdarg>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>  //for ostringsream
#include <iomanip>  //for setprecision
#include <fstream>
#include <algorithm>
#include <cfloat>
#include <map>
#include <stack>
#include <unordered_map>
#include <memory>

#include "json.hpp"

// for convenience
using ofJson = nlohmann::json;

using namespace std;

#ifndef PI
	#define PI       3.14159265358979323846
#endif

#ifndef TWO_PI
	#define TWO_PI   6.28318530717958647693
#endif

#ifndef M_TWO_PI
	#define M_TWO_PI   6.28318530717958647693
#endif

#ifndef FOUR_PI
	#define FOUR_PI 12.56637061435917295385
#endif

#ifndef HALF_PI
	#define HALF_PI  1.57079632679489661923
#endif

#ifndef DEG_TO_RAD
	#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef RAD_TO_DEG
	#define RAD_TO_DEG (180.0/PI)
#endif

#ifndef MIN
	#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
	#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef CLAMP
	#define CLAMP(val,min,max) ((val) < (min) ? (min) : ((val > max) ? (max) : (val)))
#endif

#ifndef ABS
	#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

/// \brief Used to represent the available fill modes.
/// 
/// \sa ofBaseRenderer
enum ofFillFlag{
	/// \brief Draw shapes as outlines, unfilled.
	OF_OUTLINE=	0,
	/// \brief Draw shapes filled with the current draw color.
	OF_FILLED = 1,
};

/// \brief Used to represent the available windowing modes for the application.
enum ofWindowMode{
	/// \brief A floating application window.
	OF_WINDOW 		= 0,
	/// \brief A fullscreen application window.
	OF_FULLSCREEN 	= 1,
	/// \brief A fullscreen application window with a custom width and height.
 	OF_GAME_MODE	= 2
};

/// \brief Used to represent the available rectangle aspect ratio scaling modes.
/// 
/// \sa ofRectangle
enum ofAspectRatioMode {
	/// \brief Set the rectangle's width and height to match the target.
    OF_ASPECT_RATIO_IGNORE            = 0,
    /// \brief Resizes the rectangle to completely fit within the target.
    OF_ASPECT_RATIO_KEEP              = 1,
    /// \brief Resizes the rectangle to completely enclose the target.
    OF_ASPECT_RATIO_KEEP_BY_EXPANDING = 2,
};

/// \brief Used to represent the available vertical rectangle alignment modes.
/// 
/// \sa ofRectangle
enum ofAlignVert {
	/// \brief Do not perform any vertical alignment.
    OF_ALIGN_VERT_IGNORE   = 0x0000,
	/// \brief Use the upper edge of the rectangle to vertically anchor the alignment.
    OF_ALIGN_VERT_TOP      = 0x0010,
	/// \brief Use the bottom edge of the rectangle to vertically anchor the alignment.
    OF_ALIGN_VERT_BOTTOM   = 0x0020,
	/// \brief Use the center of the rectangle to vertically anchor the alignment.
    OF_ALIGN_VERT_CENTER   = 0x0040,
};


/// \brief Used to represent the available horizontal rectangle alignment modes.
/// 
/// \sa ofRectangle
enum ofAlignHorz {
    /// \brief Do not perform any horizontal alignment.
    OF_ALIGN_HORZ_IGNORE   = 0x0000,
    /// \brief Use the left edge of the rectangle to horizontally anchor the alignment.
    OF_ALIGN_HORZ_LEFT     = 0x0001,
    /// \brief Use the right edge of the rectangle to horizontally anchor the alignment.
    OF_ALIGN_HORZ_RIGHT    = 0x0002,
    /// \brief Use the center of the rectangle to horizontally anchor the alignment.
    OF_ALIGN_HORZ_CENTER   = 0x0004,
};

/// \brief Used to represent the available rectangle drawing modes.
///
/// \sa ofRectangle
/// \sa ofTexture
/// \sa ofImage
enum ofRectMode{
	/// \brief Represents the mode where rectangles draw from the top left.
	OF_RECTMODE_CORNER=0,
	/// \brief Represents the mode where rectangles draw from the center.
 	OF_RECTMODE_CENTER=1
};

/// \brief Used to represent the available rectangle scaling modes.
///
/// ofScaleMode can usually be interpreted as a concise combination of
/// an ::ofAspectRatioMode, an ::ofAlignVert and an ::ofAlignHorz.
enum ofScaleMode{

    /// \brief Center and scale the rectangle to fit inside the target.
	/// 
	/// This centers the subject rectangle within the target rectangle and
	/// resizes the subject rectangle to completely fit within the target
	/// rectangle.
    OF_SCALEMODE_FIT     = 0,

    /// \brief Move and scale the rectangle to completely enclose the target.
    ///
    /// This centers the subject rectangle within the target rectangle and
    /// resizes the subject rectangle to completely encompass the target
    /// rectangle.
    OF_SCALEMODE_FILL    = 1,

    /// \brief Move the rectangle to be centered on the target.
    /// 
    /// This centers the subject rectangle within the target rectangle and
    /// does not modify the Subject's size or aspect ratio.
    OF_SCALEMODE_CENTER  = 2, // centers the subject

 	/// \brief Match the target rectangle's position and dimensions.
 	OF_SCALEMODE_STRETCH_TO_FILL = 3
};

/// \brief Used to represent the available channel types in ofImage.
/// 
/// These represent an abstraction of both CPU pixels (ofPixels) and GPU pixels
/// (ofTexture). In most cases, developers should prefer ::ofPixelFormat over
/// ::ofImageType for a more precise description of channel types.
/// 
/// \sa ofImage
enum ofImageType{
	/// \brief A single channel (or monochrome) image.
	/// 
	/// \sa OF_PIXELS_GRAY
	OF_IMAGE_GRAYSCALE		= 0x00,
	/// \brief A three channel (or RGB) image.
	/// 
	/// \sa OF_PIXELS_RGB
 	OF_IMAGE_COLOR			= 0x01,
	/// \brief A four channel (or RGBA) image.
	/// 
	/// \sa OF_PIXELS_RGBA
 	OF_IMAGE_COLOR_ALPHA	= 0x02,
 	/// \brief An unknown and unsupported image type.
	/// 
	/// \sa OF_PIXELS_UNKNOWN
 	OF_IMAGE_UNDEFINED		= 0x03
};

#define		OF_MAX_STYLE_HISTORY	32

/// \deprecated Not currently used in the OF codebase.
#define		OF_MAX_VIEWPORT_HISTORY	32

/// \deprecated Not currently used in the OF codebase.
#define		OF_MAX_CIRCLE_PTS 1024

/// \brief Used to represent the available blending modes for drawing.
enum ofBlendMode{
	/// \brief Blend mode is disabled.
	OF_BLENDMODE_DISABLED = 0,
	/// \brief Blend mode used for alpha blending. 
	OF_BLENDMODE_ALPHA 	  = 1,
	/// \brief Blend mode used for additive blending. 
	OF_BLENDMODE_ADD 	  = 2,
	/// \brief Blend mode used for subtractive blending. 
	OF_BLENDMODE_SUBTRACT = 3,
	/// \brief Blend mode used for multiplicative blending. 
	OF_BLENDMODE_MULTIPLY = 4,
	/// \brief Blend mode used for screen blending. 
	OF_BLENDMODE_SCREEN   = 5
};

/// \brief Used to represent the available screen orientations.
///
/// These don't use "landscape" or "portrait", because phones typically default 
/// to portrait while desktop screens are typically landscape by default.
/// 
/// \sa ::ofSetOrientation
/// \sa ::ofGetOrientation
enum ofOrientation{
	/// \brief Represents the default screen orientation.
	OF_ORIENTATION_DEFAULT = 1,	
	/// \brief Represents a screen rotated 180 degrees, also known as upside-down.	
	OF_ORIENTATION_180 = 2,
	/// \brief Represents a screen rotated 90 degrees clockwise.	
    OF_ORIENTATION_90_LEFT = 3,
	/// \brief Represents a screen rotated 90 degrees counter-clockwise.	
	OF_ORIENTATION_90_RIGHT = 4,
	/// \brief Represents an unknown orientation.	
	OF_ORIENTATION_UNKNOWN = 5
};

/// \brief Represents the gradient types available to ofBackgroundGradient(). 
enum ofGradientMode {
	/// \brief Represents a top-to-bottom linear gradient.
	OF_GRADIENT_LINEAR = 0,
	/// \brief Represents a circular gradient beginning at the screen's center.
	OF_GRADIENT_CIRCULAR,
	/// \brief Represents a horizontal bar gradient.
	/// 
	///  This is a horizontal gradient starting across the screen's center,
	///  and extending both to the top and bottom of the screen.
	OF_GRADIENT_BAR
};

/// \brief represents the available polygon winding modes.
/// 
/// These are straight out of glu, but renamed and included here
/// for convenience.  We don't mean to wrap the whole glu library
/// (or any other library for that matter), but these defines are useful
/// to give people flexibility over the polygonizer.
/// 
/// \sa ofPath::tessellate()
/// \sa ofTessellator::performTessellation()
/// \sa http://glprogramming.com/red/images/Image128.gif
/// \sa http://glprogramming.com/red/chapter11.html
enum ofPolyWindingMode{
	/// \brief Fill odd winding numbers.
	OF_POLY_WINDING_ODD 	        ,
	/// \brief Fill all non-zero winding numbers.
	OF_POLY_WINDING_NONZERO         ,
	/// \brief Fill all winding numbers greater than zero.
	OF_POLY_WINDING_POSITIVE        ,
	/// \brief Fill all winding numbers less than zero.
	OF_POLY_WINDING_NEGATIVE        ,
	/// \brief Fill all winding numbers greater than 1 or less than -1.
	/// 
	/// This stands for "Fill ABSolute values Greater than EQual to TWO".
	OF_POLY_WINDING_ABS_GEQ_TWO
};

/// \deprecated Not currently used in the OF codebase.
#define 	OF_CLOSE						  (true)

/// \brief represents the available matrix coordinate system handednesses.
///
/// \sa ofMatrixStack
/// \sa http://seanmiddleditch.com/matrices-handedness-pre-and-post-multiplication-row-vs-column-major-and-notations/ 
enum ofHandednessType {OF_LEFT_HANDED, OF_RIGHT_HANDED};

/// \brief represents the available matrix types used internally in ::ofMatrixStack.
enum ofMatrixMode {OF_MATRIX_MODELVIEW=0, OF_MATRIX_PROJECTION, OF_MATRIX_TEXTURE};

//--------------------------------------------
//
// 	Keyboard definitions
//
// 	ok -- why this?
// 	glut key commands have some annoying features,
// 	in that some normal keys have the same value as special keys,
// 	but we want ONE key routine, so we need to redefine several,
// 	so that we get some normalacy across keys routines
//
// 	(everything that comes through "glutSpecialKeyFunc" will get 256 added to it,
// 	to avoid conflicts, before, values like "left, right up down" (ie, 104, 105, 106) were conflicting with
// 	letters.. now they will be 256 + 104, 256 + 105....)


	#define OF_KEY_MODIFIER 	0x0100
	#define OF_KEY_RETURN		13
	#define OF_KEY_ESC			27
    #define OF_KEY_TAB          9
    #define OF_KEY_COMMAND      OF_KEY_SUPER
    
	// http://www.openframeworks.cc/forum/viewtopic.php?t=494
	// some issues with keys across platforms:

	#ifdef TARGET_OSX
		#define OF_KEY_BACKSPACE	127
		#define OF_KEY_DEL			8
	#else
		#define OF_KEY_BACKSPACE	8
		#define OF_KEY_DEL			127
	#endif

	// zach - there are more of these keys, we can add them here...
	// these are keys that are not coming through "special keys"
	// via glut, but just other keys on your keyboard like

	#define OF_KEY_F1			(1 | OF_KEY_MODIFIER)
	#define OF_KEY_F2			(2 | OF_KEY_MODIFIER)
	#define OF_KEY_F3			(3 | OF_KEY_MODIFIER)
	#define OF_KEY_F4			(4 | OF_KEY_MODIFIER)
	#define OF_KEY_F5			(5 | OF_KEY_MODIFIER)
	#define OF_KEY_F6			(6 | OF_KEY_MODIFIER)
	#define OF_KEY_F7			(7 | OF_KEY_MODIFIER)
	#define OF_KEY_F8			(8 | OF_KEY_MODIFIER)
	#define OF_KEY_F9			(9 | OF_KEY_MODIFIER)
	#define OF_KEY_F10			(10 | OF_KEY_MODIFIER)
	#define OF_KEY_F11			(11 | OF_KEY_MODIFIER)
	#define OF_KEY_F12			(12 | OF_KEY_MODIFIER)
	#define OF_KEY_LEFT			(100 | OF_KEY_MODIFIER)
	#define OF_KEY_UP			(101 | OF_KEY_MODIFIER)
	#define OF_KEY_RIGHT		(102 | OF_KEY_MODIFIER)
	#define OF_KEY_DOWN			(103 | OF_KEY_MODIFIER)
	#define OF_KEY_PAGE_UP		(104 | OF_KEY_MODIFIER)
	#define OF_KEY_PAGE_DOWN	(105 | OF_KEY_MODIFIER)
	#define OF_KEY_HOME			(106 | OF_KEY_MODIFIER)
	#define OF_KEY_END			(107 | OF_KEY_MODIFIER)
	#define OF_KEY_INSERT		(108 | OF_KEY_MODIFIER)
	#define OF_KEY_CONTROL		(0x200 | OF_KEY_MODIFIER)
	#define OF_KEY_ALT			(0x400 | OF_KEY_MODIFIER)
	#define OF_KEY_SHIFT		(0x800 | OF_KEY_MODIFIER)
	#define OF_KEY_SUPER		(0x1000 | OF_KEY_MODIFIER)
	#define OF_KEY_LEFT_SHIFT	(0x1 | OF_KEY_SHIFT)
	#define OF_KEY_RIGHT_SHIFT	(0x2 | OF_KEY_SHIFT)
	#define OF_KEY_LEFT_CONTROL	(0x1 | OF_KEY_CONTROL)
	#define OF_KEY_RIGHT_CONTROL (0x2 | OF_KEY_CONTROL)
	#define OF_KEY_LEFT_ALT		(0x1 | OF_KEY_ALT)
	#define OF_KEY_RIGHT_ALT	(0x2 | OF_KEY_ALT)
	#define OF_KEY_LEFT_SUPER	(0x1 | OF_KEY_SUPER)
	#define OF_KEY_RIGHT_SUPER	(0x2 | OF_KEY_SUPER)
	#define OF_KEY_LEFT_COMMAND OF_KEY_LEFT_SUPER
	#define OF_KEY_RIGHT_COMMAND OF_KEY_RIGHT_SUPER
// not sure what to do in the case of non-glut apps....

    #define OF_MOUSE_BUTTON_1      0
    #define OF_MOUSE_BUTTON_2      1
    #define OF_MOUSE_BUTTON_3      2
    #define OF_MOUSE_BUTTON_4      3
    #define OF_MOUSE_BUTTON_5      4
    #define OF_MOUSE_BUTTON_6      5
    #define OF_MOUSE_BUTTON_7      6
    #define OF_MOUSE_BUTTON_8      7
    #define OF_MOUSE_BUTTON_LAST   OF_MOUSE_BUTTON_8
    #define OF_MOUSE_BUTTON_LEFT   OF_MOUSE_BUTTON_1
    #define OF_MOUSE_BUTTON_MIDDLE OF_MOUSE_BUTTON_2
    #define OF_MOUSE_BUTTON_RIGHT  OF_MOUSE_BUTTON_3

//--------------------------------------------
//console colors for our logger - shame this doesn't work with the xcode console

#ifdef TARGET_WIN32

	#define OF_CONSOLE_COLOR_RESTORE (0 | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) )
	#define OF_CONSOLE_COLOR_BLACK (0)
	#define OF_CONSOLE_COLOR_RED (FOREGROUND_RED)
	#define OF_CONSOLE_COLOR_GREEN (FOREGROUND_GREEN)
	#define OF_CONSOLE_COLOR_YELLOW (FOREGROUND_RED|FOREGROUND_GREEN)
	#define OF_CONSOLE_COLOR_BLUE (FOREGROUND_BLUE)
	#define OF_CONSOLE_COLOR_PURPLE (FOREGROUND_RED | FOREGROUND_BLUE )
	#define OF_CONSOLE_COLOR_CYAN (FOREGROUND_GREEN | FOREGROUND_BLUE)
	#define OF_CONSOLE_COLOR_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

#else

	#define OF_CONSOLE_COLOR_RESTORE (0)
	#define OF_CONSOLE_COLOR_BLACK (30)
	#define OF_CONSOLE_COLOR_RED (31)
	#define OF_CONSOLE_COLOR_GREEN (32)
	#define OF_CONSOLE_COLOR_YELLOW (33)
	#define OF_CONSOLE_COLOR_BLUE (34)
	#define OF_CONSOLE_COLOR_PURPLE (35)
	#define OF_CONSOLE_COLOR_CYAN (36)
	#define OF_CONSOLE_COLOR_WHITE (37)

#endif

/// \brief Used to represent the available pixel formats.
///
/// \sa ofPixels
enum ofPixelFormat{
	/// \brief A single-channel pixel, typically used for greyscale images.
	///
	/// This has 1 channel and a type-dependent number of bits per-pixel.
	OF_PIXELS_GRAY = 0,
	/// \brief A single-channel pixel with an alpha channel.
	///
	/// This has 2 channels and a type-dependent number of bits per-pixel.
	OF_PIXELS_GRAY_ALPHA = 1,

	/// \brief An RGB pixel with no alpha channel.
	///
	/// This has 3 channels and a type-dependent number of bits per-pixel.
	///
	/// \sa http://www.fourcc.org/rgb.php#BI_RGB
	OF_PIXELS_RGB=2,
	/// \brief A pixel used for color data with a blue/green/red channel order.
	///
	/// This has 3 channels and a type-dependent number of bits per-pixel.
	OF_PIXELS_BGR=3,
	/// \brief An RGBA pixel. This is typically used for color with transparency.
	///
	/// This has 4 channels and a type-dependent number of bits per-pixel.
	///
	/// \sa http://www.fourcc.org/rgb.php#RGBA
	OF_PIXELS_RGBA=4,
	/// \brief A pixel used for color/transparency with a blue/green/red/alpha channel order.
	///
	/// This has 4 channels and a type-dependent number of bits per-pixel.
	OF_PIXELS_BGRA=5,

	// \brief A 16-bit color pixel with 5-bit red and blue channels and a 6-bit green channel.
	OF_PIXELS_RGB565=6,

	/// \brief A 12-bit YUV 4:2:0 pixel with an interleaved U/V plane.
	///
	/// YUV 4:2:0 image with a plane of 8-bit Y samples followed by an
	/// interleaved U/V plane containing 8-bit 2x2 subsampled color difference
	/// samples.
	///
	/// \sa http://www.fourcc.org/yuv.php#NV12
	OF_PIXELS_NV12=7,
	/// \brief A 12-bit YUV 4:2:0 pixel with an interleaved V/U plane.
	/// 
	/// YUV 4:2:0 image with a plane of 8-bit Y samples followed by an
	/// interleaved V/U plane containing 8-bit 2x2 subsampled chroma samples.
	/// The same as NV12 except the interleave order of U and V is reversed.
	///
	/// \sa http://www.fourcc.org/yuv.php#NV21
	OF_PIXELS_NV21=8,
	/// \brief A 12-bit YUV NxM Y plane followed by (N/2)x(M/2) V and U planes.
	///
	/// \sa http://www.fourcc.org/yuv.php#YV12
	OF_PIXELS_YV12=9,
	/// \brief A 12-bit YUV format similar to ::OF_PIXELS_YV12, but with U & V reversed.
	/// 
	/// Note that IYUV and I420 appear to be identical.
	///
	/// \sa http://www.fourcc.org/yuv.php#IYUV
	OF_PIXELS_I420=10,
	/// \brief A 16-bit YUV 4:2:2 format.
	///
	/// \sa http://www.fourcc.org/yuv.php#YUY2
	OF_PIXELS_YUY2=11,
	/// \brief A 16-bit YUV 4:2:2 format.
	///
	/// \sa http://www.fourcc.org/yuv.php#UYVY
	OF_PIXELS_UYVY=12,

	/// \brief A single channel pixel, typically used for the luma component of YUV.
	OF_PIXELS_Y,
	/// \brief A single channel pixel, typically used (with V) for the chroma component of YUV.
	OF_PIXELS_U,
	/// \brief A single channel pixel, typically used (with U) for the chroma component of YUV.
	OF_PIXELS_V,
	/// \brief A two channel pixel, with U first, representing both chroma components of YUV.
	OF_PIXELS_UV,
	/// \brief A two channel pixel, with V first, representing both chroma components of YUV.
	OF_PIXELS_VU,

	/// \brief This is a placeholder to indicate the last valid enum.
	OF_PIXELS_NUM_FORMATS,

	/// \brief This indicates an unknown pixel type.
	OF_PIXELS_UNKNOWN=-1,
	/// \brief This indicates an unknown, native pixel type.
	OF_PIXELS_NATIVE=-2
};

#define OF_PIXELS_MONO OF_PIXELS_GRAY
#define OF_PIXELS_R OF_PIXELS_GRAY
#define OF_PIXELS_RG OF_PIXELS_GRAY_ALPHA


/// \brief Sets the bitmap drawing mode for text.
/// \sa ofSetDrawBitmapMode()
enum ofDrawBitmapMode{
	OF_BITMAPMODE_SIMPLE = 0,
	OF_BITMAPMODE_SCREEN,
	OF_BITMAPMODE_VIEWPORT,
	OF_BITMAPMODE_MODEL,
	OF_BITMAPMODE_MODEL_BILLBOARD
};

/// \brief Sets the text encoding mode.
/// 
/// This is not currently used in the codebase, but the
/// assumption is that will once again begin using this as we
/// continue to work on our UTF8 implementation.
enum ofTextEncoding{
	OF_ENCODING_UTF8,
	OF_ENCODING_ISO_8859_15
};
