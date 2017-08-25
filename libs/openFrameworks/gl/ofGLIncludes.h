#pragma once

#include "ofConstants.h"

// then the the platform specific includes:
#ifdef TARGET_WIN32
#	define GLEW_STATIC
#	define GLEW_NO_GLU
#	include "GL/glew.h"
#	include "GL/wglew.h"
// some gl.h files, like dev-c++, are old - this is pretty universal
#	ifndef GL_BGR_EXT
#		define GL_BGR_EXT 0x80E0
#	endif
#endif // TARGET_WIN32


#ifdef TARGET_OSX
#	include "GL/glew.h"
#	include <OpenGL/gl.h>
#endif // TARGET_OSX

#ifdef TARGET_LINUX

#	include <unistd.h>

#	ifdef TARGET_LINUX_ARM
#		ifdef TARGET_RASPBERRY_PI
#			include "bcm_host.h"
#		endif
#		include "GLES/gl.h"
#		include "GLES/glext.h"
#		include "GLES2/gl2.h"
#		include "GLES2/gl2ext.h"
#		define EGL_EGLEXT_PROTOTYPES
#		include "EGL/egl.h"
#		include "EGL/eglext.h"
#	else // normal linux
#		define GL_GLEXT_PROTOTYPES
#		include <GL/glew.h>
#		include <GL/gl.h>
#		include <GL/glext.h>
#	endif

// for some reason, this isn't defined at compile time,
// so this hack let's us work
// for 99% of the linux folks that are on intel
// everyone one else will have RGB / BGR issues.
//#if defined(__LITTLE_ENDIAN__)
#	define TARGET_LITTLE_ENDIAN		// intel cpu

// some things for serial compilation:
#	define B14400	14400
#	define B28800	28800

#endif // TARGET_LINUX


#ifdef TARGET_OF_IOS
#	import <OpenGLES/ES1/gl.h>
#	import <OpenGLES/ES1/glext.h>
#	import <OpenGLES/ES2/gl.h>
#	import <OpenGLES/ES2/glext.h>
#endif // TARGET_OF_IOS

#ifdef TARGET_ANDROID
#	include <GLES/gl.h>
#	define GL_GLEXT_PROTOTYPES
#	include <GLES/glext.h>
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#endif // TARGET_ANDROID

#ifdef TARGET_EMSCRIPTEN
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#	include "EGL/egl.h"
#	include "EGL/eglext.h"
#endif // TARGET_EMSCRIPTEN

