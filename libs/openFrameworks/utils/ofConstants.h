#ifndef OF_CONSTANTS
#define OF_CONSTANTS

//-------------------------------
#define OF_VERSION	6
//-------------------------------


#define OF_LOOP_NONE					0x01
#define OF_LOOP_PALINDROME				0x02
#define OF_LOOP_NORMAL					0x03

//-------------------------------
//  find the system type --------
//-------------------------------

// 		helpful:
// 		http://www.ogre3d.org/docs/api/html/OgrePlatform_8h-source.html

#if defined( __WIN32__ ) || defined( _WIN32 )
	#define TARGET_WIN32
#elif defined( __APPLE_CC__)
	#include <TargetConditionals.h>

	#if (TARGET_OF_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
		#define TARGET_OF_IPHONE
		#define TARGET_OPENGLES
	#else
		#define TARGET_OSX
	#endif
#else
	#define TARGET_LINUX
#endif
//-------------------------------


// then the the platform specific includes:
#ifdef TARGET_WIN32
	//this is for TryEnterCriticalSection
	//http://www.zeroc.com/forums/help-center/351-ice-1-2-tryentercriticalsection-problem.html
	#ifndef _WIN32_WINNT
		#   define _WIN32_WINNT 0x400
	#endif
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include "GLee.h"
   	#include "glu.h"
	#define __WINDOWS_DS__
	#define __WINDOWS_MM__
	#if (_MSC_VER)       // microsoft visual studio
		#pragma warning(disable : 4996)     // disable all deprecation warnings
		#pragma warning(disable : 4068)     // unknown pragmas
		#pragma warning(disable : 4101)     // unreferenced local variable
		#pragma	warning(disable : 4312)		// type cast conversion (in qt vp)
		#pragma warning(disable : 4311)		// type cast pointer truncation (qt vp)
		#pragma warning(disable : 4018)		// signed/unsigned mismatch (since vector.size() is a size_t)
		#pragma warning(disable : 4267)		// conversion from size_t to Size warning... possible loss of data
		#pragma warning(disable : 4800)		// 'Boolean' : forcing value to bool 'true' or 'false'
		#pragma warning(disable : 4099)		// for debug, PDB 'vc80.pdb' was not found with...
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
	#include "GLee.h"
	#include <OpenGL/glu.h>
	#include <ApplicationServices/ApplicationServices.h>

	#if defined(__LITTLE_ENDIAN__)
		#define TARGET_LITTLE_ENDIAN		// intel cpu
	#endif
#endif

#ifdef TARGET_LINUX
        #include <unistd.h>
        #include "GLee.h"
        #include <GL/glu.h>

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


#ifdef TARGET_OF_IPHONE
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>
#endif


#ifndef __MWERKS__
#include <cstdlib>
#define OF_EXIT_APP(val)		std::exit(val);
#else
#define OF_EXIT_APP(val)		std::exit(val);
#endif




//------------------------------------------------ capture
// if are linux

#ifdef TARGET_LINUX


	// firewire cameras doesn't work yet with gstreamer you can change
	// to unicap by uncommenting the following define. note that this can make
	// your app GPL

	// (if you change this, you might need to clean and rebuild, in CB build->rebuild)

	//#define OF_SWITCH_TO_UNICAP_FOR_LINUX_VIDCAP

	#ifdef OF_SWITCH_TO_UNICAP_FOR_LINUX_VIDCAP
		#define OF_VIDEO_CAPTURE_UNICAP
    #else
		#define OF_VIDEO_CAPTURE_GSTREAMER
	#endif


#else

    // non - linux, pc or osx

    // comment out this following line, if you'd like to use the
    // quicktime capture interface on windows
    // if not, we default to videoInput library for
    // direct show capture...

    #define OF_SWITCH_TO_DSHOW_FOR_WIN_VIDCAP

    #ifdef OF_SWITCH_TO_DSHOW_FOR_WIN_VIDCAP
        #ifdef TARGET_OSX
            #define OF_VIDEO_CAPTURE_QUICKTIME
        #else
            #define OF_VIDEO_CAPTURE_DIRECTSHOW
        #endif
    #else
        // all quicktime, all the time
        #define OF_VIDEO_CAPTURE_QUICKTIME
    #endif
#endif


#ifdef TARGET_LINUX
	#define OF_VIDEO_PLAYER_GSTREAMER
#else
	#define OF_VIDEO_PLAYER_QUICKTIME
#endif

// comment out this line to disable all poco related code
#define OF_USING_POCO

//we don't want to break old code that uses ofSimpleApp
//so we forward declare ofBaseApp and make ofSimpleApp mean the same thing
class ofBaseApp;
typedef ofBaseApp ofSimpleApp;

enum ofLogLevel{
	OF_LOG_VERBOSE,
	OF_LOG_NOTICE,
	OF_LOG_WARNING,
	OF_LOG_ERROR,
	OF_LOG_FATAL_ERROR,
	OF_LOG_SILENT	//this one is special and should always be last - set ofSetLogLevel to OF_SILENT to not recieve any messages
};

#define OF_DEFAULT_LOG_LEVEL  OF_LOG_WARNING;

// serial error codes
#define OF_SERIAL_NO_DATA 	-2
#define OF_SERIAL_ERROR		-1

// core: ---------------------------
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>  //for ostringsream
#include <iomanip>  //for setprecision
#include <fstream>
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
	#define CLAMP(val,min,max) (MAX(MIN(val,max),min))
#endif

#ifndef ABS
	#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

#define 	OF_FILLED				0x01
#define 	OF_OUTLINE				0x02
#define 	OF_WINDOW 				0
#define 	OF_FULLSCREEN 			1
#define 	OF_GAME_MODE			2

#define 	OF_RECTMODE_CORNER				0
#define 	OF_RECTMODE_CENTER				1

#define 	OF_IMAGE_GRAYSCALE		0x00
#define 	OF_IMAGE_COLOR			0x01
#define 	OF_IMAGE_COLOR_ALPHA	0x02
#define 	OF_IMAGE_UNDEFINED		0x03

#define		OF_MAX_STYLE_HISTORY	32
#define		OF_MAX_CIRCLE_PTS 1024

// these are straight out of glu, but renamed and included here
// for convenience
//
// we don't mean to wrap the whole glu library (or any other library for that matter)
// but these defines are useful to give people flexability over the polygonizer
//
// some info:
// http://glprogramming.com/red/images/Image128.gif
//
// also: http://glprogramming.com/red/chapter11.html
// (CSG ideas)

#define 	OF_POLY_WINDING_ODD 	          100130
#define 	OF_POLY_WINDING_NONZERO           100131
#define 	OF_POLY_WINDING_POSITIVE          100132
#define 	OF_POLY_WINDING_NEGATIVE          100133
#define		OF_POLY_WINDING_ABS_GEQ_TWO       100134

#define 	OF_CLOSE						  (true)


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

// not sure what to do in the case of non-glut apps....


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

#endif
