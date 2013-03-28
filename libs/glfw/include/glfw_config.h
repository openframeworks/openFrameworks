//========================================================================
// GLFW - An OpenGL library
// Platform:    Any
// API version: 3.0
// WWW:         http://www.glfw.org/
//------------------------------------------------------------------------
// Copyright (c) 2010 Camilla Berglund <elmindreda@elmindreda.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// As config.h.in, this file is used by CMake to produce the config.h shared
// configuration header file.  If you are adding a feature requiring
// conditional compilation, this is the proper place to add the macros.
//========================================================================
// As config.h, this file defines compile-time build options and macros for
// all platforms supported by GLFW.  As this is a generated file, don't modify
// it.  Instead, you should modify the config.h.in file.
//========================================================================

// Define this to 1 if building GLFW for X11
/* #undef _GLFW_X11 */
// Define this to 1 if building GLFW for Win32
/* #undef _GLFW_WIN32 */
// Define this to 1 if building GLFW for Cocoa
#if defined( __APPLE_CC__)
	#define _GLFW_COCOA
#else 
	#define _GLFW_X11
#endif
// Define this to 1 if building GLFW for EGL
/* #undef _GLFW_EGL */
// Define this to 1 if building GLFW for GLX
/* #undef _GLFW_GLX */
// Define this to 1 if building GLFW for WGL
/* #undef _GLFW_WGL */
// Define this to 1 if building GLFW for NSGL
#if defined( __APPLE_CC__)
	#define _GLFW_NSGL
#else 
	#define _GLFW_GLX
#endif
// Define this to 1 if building as a shared library / dynamic library / DLL
/* #undef _GLFW_BUILD_DLL */

// Define this to 1 to disable dynamic loading of winmm
/* #undef _GLFW_NO_DLOAD_WINMM */

// Define this to 1 if glXGetProcAddress is available
/* #undef _GLFW_HAS_GLXGETPROCADDRESS */
// Define this to 1 if glXGetProcAddressARB is available
/* #undef _GLFW_HAS_GLXGETPROCADDRESSARB */
// Define this to 1 if glXGetProcAddressEXT is available
/* #undef _GLFW_HAS_GLXGETPROCADDRESSEXT */

// Define this to 1 if glfwInit should change the current directory
#define _GLFW_USE_CHDIR
// Define this to 1 if glfwCreateWindow should populate the menu bar
#define _GLFW_USE_MENUBAR

// Define this to 1 if using OpenGL as the client library
#define _GLFW_USE_OPENGL
// Define this to 1 if using OpenGL ES 1.1 as the client library
/* #undef _GLFW_USE_GLESV1 */
// Define this to 1 if using OpenGL ES 2.0 as the client library
/* #undef _GLFW_USE_GLESV2 */

// The GLFW version as used by glfwGetVersionString
#define _GLFW_VERSION_FULL "3.0.0"

