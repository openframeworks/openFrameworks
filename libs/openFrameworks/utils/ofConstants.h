#pragma once
		#include <stdint.h>

//-------------------------------
#define OF_VERSION_MAJOR 0
#define OF_VERSION_MINOR 8
#define OF_VERSION_PATCH 1

//-------------------------------

enum ofLoopType{
	OF_LOOP_NONE=0x01,
	OF_LOOP_PALINDROME=0x02,
	OF_LOOP_NORMAL=0x03
};

enum ofTargetPlatform{
	OF_TARGET_OSX,
	OF_TARGET_WINGCC,
	OF_TARGET_WINVS,
	OF_TARGET_IOS,
	OF_TARGET_ANDROID,
	OF_TARGET_LINUX,
	OF_TARGET_LINUX64,
	OF_TARGET_LINUXARMV6L, // arm v6 little endian
	OF_TARGET_LINUXARMV7L, // arm v7 little endian
};

#ifndef OF_TARGET_IPHONE
    #define OF_TARGET_IPHONE OF_TARGET_IOS
#endif 

// Cross-platform deprecation warning
#ifdef __GNUC__
	// clang also has this defined. deprecated(message) is only for gcc>=4.5
	#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 5)
        #define OF_DEPRECATED_MSG(message, func) func __attribute__ ((deprecated(message)))
    #else
        #define OF_DEPRECATED_MSG(message, func) func __attribute__ ((deprecated))
    #endif
	#define OF_DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
	#define OF_DEPRECATED_MSG(message, func) __declspec(deprecated(message)) func
	#define OF_DEPRECATED(func) __declspec(deprecated) func
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
	#include <TargetConditionals.h>

	#if (TARGET_OS_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
		#define TARGET_OF_IPHONE
        #define TARGET_OF_IOS
		#define TARGET_OPENGLES
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
#else
	#define TARGET_LINUX
#endif
//-------------------------------


// then the the platform specific includes:
#ifdef TARGET_WIN32
	//this is for TryEnterCriticalSection
	//http://www.zeroc.com/forums/help-center/351-ice-1-2-tryentercriticalsection-problem.html
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x500
	#endif
	#define WIN32_LEAN_AND_MEAN

	#if (_MSC_VER)
		#define NOMINMAX		
		//http://stackoverflow.com/questions/1904635/warning-c4003-and-errors-c2589-and-c2059-on-x-stdnumeric-limitsintmax
	#endif

	#include <windows.h>
	#define GLEW_STATIC
	#include "GL\glew.h"
	#include "GL\wglew.h"
   	#include "glu.h"
	#define __WINDOWS_DS__
	#define __WINDOWS_MM__
	#if (_MSC_VER)       // microsoft visual studio
		#include <stdint.h>
		#include <functional>
		#pragma warning(disable : 4018)		// signed/unsigned mismatch (since vector.size() is a size_t)
		#pragma warning(disable : 4068)		// unknown pragmas
		#pragma warning(disable : 4101)		// unreferenced local variable
		#pragma warning(disable : 4267)		// conversion from size_t to Size warning... possible loss of data
		#pragma warning(disable : 4311)		// type cast pointer truncation (qt vp)
		#pragma warning(disable : 4312)		// type cast conversion (in qt vp)
		#pragma warning(disable : 4800)		// 'Boolean' : forcing value to bool 'true' or 'false'

		// make microsoft visual studio complain less about double / float conversion and
		// truncation
		#pragma warning(disable : 4244)
		#pragma warning(disable : 4305)

		// warnings: http://msdn.microsoft.com/library/2c8f766e.aspx
	#endif

	#define TARGET_LITTLE_ENDIAN			// intel cpu

	// some gl.h files, like dev-c++, are old - this is pretty universal
	#ifndef GL_BGR_EXT
	#define GL_BGR_EXT 0x80E0
	#endif

	// #define WIN32_HIGH_RES_TIMING

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

		#define GL_GLEXT_PROTOTYPES
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
        #include <GL/glew.h>
        #include <GL/gl.h>
        #include <GL/glx.h>
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

#ifdef TARGET_OPENGLES
//	#include "glu.h"
	//typedef GLushort ofIndexType ;
#else
	//typedef GLuint ofIndexType;
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

	#elif defined(TARGET_OF_IOS)

		#define OF_VIDEO_CAPTURE_IOS

	#endif
#endif

//------------------------------------------------  video player
// check if any video player system is already defined from the compiler
#if !defined(OF_VIDEO_PLAYER_GSTREAMER) && !defined(OF_VIDEO_PLAYER_IOS) && !defined(OF_VIDEO_PLAYER_QUICKTIME)
	#ifdef TARGET_LINUX
		#define OF_VIDEO_PLAYER_GSTREAMER
	#elif defined(TARGET_ANDROID)
		#define OF_VIDEO_PLAYER_ANDROID
	#elif defined(TARGET_OF_IOS)
		#define OF_VIDEO_PLAYER_IOS
	#elif defined(TARGET_OSX)
		//for 10.7 and 10.8 users we use QTKit for 10.6 users we use QuickTime
		#ifndef MAC_OS_X_VERSION_10_7
			#define OF_VIDEO_PLAYER_QUICKTIME
		#else
			#define OF_VIDEO_PLAYER_QTKIT
		#endif
	#else
		#define OF_VIDEO_PLAYER_QUICKTIME
	#endif
#endif

//------------------------------------------------ soundstream
// check if any soundstream api is defined from the compiler
#if !defined(OF_SOUNDSTREAM_PORTAUDIO) && !defined(OF_SOUNDSTREAM_RTAUDIO) && !defined(OF_SOUNDSTREAM_ANDROID)
	#if defined(TARGET_LINUX) || defined(TARGET_WIN32) || defined(TARGET_OSX)
		#define OF_SOUNDSTREAM_RTAUDIO
	#elif defined(TARGET_ANDROID)
		#define OF_SOUNDSTREAM_ANDROID
	#elif defined(TARGET_OF_IOS)
		#define OF_SOUNDSTREAM_IOS
	#endif
#endif

//------------------------------------------------ soundplayer
// check if any soundplayer api is defined from the compiler
#if !defined(OF_SOUND_PLAYER_QUICKTIME) && !defined(OF_SOUND_PLAYER_FMOD) && !defined(OF_SOUND_PLAYER_OPENAL)
  #ifdef TARGET_OF_IOS
  	#define OF_SOUND_PLAYER_IPHONE
  #elif defined TARGET_LINUX
  	#define OF_SOUND_PLAYER_OPENAL
  #elif !defined(TARGET_ANDROID)
  	#define OF_SOUND_PLAYER_FMOD
  #endif
#endif

// comment out this line to disable all poco related code
#define OF_USING_POCO


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

enum ofFillFlag{
	OF_OUTLINE=	0,
	OF_FILLED = 1,
};

enum ofWindowMode{
	OF_WINDOW 		= 0,
	OF_FULLSCREEN 	= 1,
 	OF_GAME_MODE	= 2
};

enum ofAspectRatioMode {
    OF_ASPECT_RATIO_IGNORE            = 0,
    OF_ASPECT_RATIO_KEEP              = 1,
    OF_ASPECT_RATIO_KEEP_BY_EXPANDING = 2,
};

enum ofAlignVert {
    OF_ALIGN_VERT_IGNORE   = 0x0000,
    OF_ALIGN_VERT_TOP      = 0x0010,
    OF_ALIGN_VERT_BOTTOM   = 0x0020,
    OF_ALIGN_VERT_CENTER   = 0x0040,
};

enum ofAlignHorz {
    OF_ALIGN_HORZ_IGNORE   = 0x0000,
    OF_ALIGN_HORZ_LEFT     = 0x0001,
    OF_ALIGN_HORZ_RIGHT    = 0x0002,
    OF_ALIGN_HORZ_CENTER   = 0x0004,
};

enum ofRectMode{
	OF_RECTMODE_CORNER=0,
 	OF_RECTMODE_CENTER=1
};

enum ofScaleMode{
    // ofScaleMode can usually be interpreted as a concise combination of
    // an ofAspectRatioMode, an ofAlignVert and an ofAlignHorz.
    
    // fits the SUBJECT rect INSIDE the TARGET rect.
    // Preserves SUBJECTS's aspect ratio.
    // Final Subject's Area <= Target's Area.
    // Subject's Center == Target's Center
    OF_SCALEMODE_FIT     = 0,
    // FILLS the TARGET rect with the SUBJECT rect.
    // Preserves the SUBJECT's aspect ratio.
    // Subject's Area >= Target's Area.
    // Subject's Center == Target's Center
    OF_SCALEMODE_FILL    = 1,
    // Preserves the SUBJECT's aspect ratio.
    // Subject's Area is Unchanged
    // Subject's Center == Target's Center
    OF_SCALEMODE_CENTER  = 2, // centers the subject
    // Can CHANGE the SUBJECT's aspect ratio.
    // Subject's Area == Target's Area
    // Subject's Center == Target's Center
 	OF_SCALEMODE_STRETCH_TO_FILL = 3, // simply matches the target dims
};

enum ofImageType{
	OF_IMAGE_GRAYSCALE		= 0x00,
 	OF_IMAGE_COLOR			= 0x01,
 	OF_IMAGE_COLOR_ALPHA	= 0x02,
 	OF_IMAGE_UNDEFINED		= 0x03
};

enum ofPixelFormat{
	OF_PIXELS_MONO = 0, 
	OF_PIXELS_RGB,
	OF_PIXELS_RGBA,
	OF_PIXELS_BGRA,
	OF_PIXELS_RGB565,
	OF_PIXELS_UNKNOWN
};

#define		OF_MAX_STYLE_HISTORY	32
#define		OF_MAX_VIEWPORT_HISTORY	32
#define		OF_MAX_CIRCLE_PTS 1024

// Blend Modes
enum ofBlendMode{
	OF_BLENDMODE_DISABLED = 0,
	OF_BLENDMODE_ALPHA 	  = 1,
	OF_BLENDMODE_ADD 	  = 2,
	OF_BLENDMODE_SUBTRACT = 3,
	OF_BLENDMODE_MULTIPLY = 4,
	OF_BLENDMODE_SCREEN   = 5
};

//this is done to match the iPhone defaults 
//we don't say landscape, portrait etc because iPhone apps default to portrait while desktop apps are typically landscape
enum ofOrientation{
	OF_ORIENTATION_DEFAULT = 1,	
	OF_ORIENTATION_180 = 2,
    OF_ORIENTATION_90_LEFT = 3,
	OF_ORIENTATION_90_RIGHT = 4,
    OF_ORIENTATION_UNKNOWN = 5
};

// gradient modes when using ofBackgroundGradient
enum ofGradientMode {
	OF_GRADIENT_LINEAR = 0,
	OF_GRADIENT_CIRCULAR,
	OF_GRADIENT_BAR
};

// these are straight out of glu, but renamed and included here
// for convenience
//
// we don't mean to wrap the whole glu library (or any other library for that matter)
// but these defines are useful to give people flexibility over the polygonizer
//
// some info:
// http://glprogramming.com/red/images/Image128.gif
//
// also: http://glprogramming.com/red/chapter11.html
// (CSG ideas)

enum ofPolyWindingMode{
	OF_POLY_WINDING_ODD 	        ,
	OF_POLY_WINDING_NONZERO         ,
	OF_POLY_WINDING_POSITIVE        ,
	OF_POLY_WINDING_NEGATIVE        ,
	OF_POLY_WINDING_ABS_GEQ_TWO
};

#define 	OF_CLOSE						  (true)


enum ofHandednessType {OF_LEFT_HANDED, OF_RIGHT_HANDED};

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


//--------------------------------------------
//ofBitmap draw mode
enum ofDrawBitmapMode{
	OF_BITMAPMODE_SIMPLE = 0,
	OF_BITMAPMODE_SCREEN,
	OF_BITMAPMODE_VIEWPORT,
	OF_BITMAPMODE_MODEL,
	OF_BITMAPMODE_MODEL_BILLBOARD
};

enum ofTextEncoding{
	OF_ENCODING_UTF8,
	OF_ENCODING_ISO_8859_15
};
