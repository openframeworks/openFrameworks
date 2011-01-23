dnl -*- bash -*-
dnl Process this file with autoconf to produce a configure script.
dnl
dnl Copyright (C) 2011 Monty Taylor
dnl
dnl Permission is hereby granted, free of charge, to any person
dnl obtaining a copy of this software and associated documentation
dnl files (the "Software"), to deal in the Software without
dnl restriction, including without limitation the rights to use,
dnl copy, modify, merge, publish, distribute, sublicense, and/or sell
dnl copies of the Software, and to permit persons to whom the
dnl Software is furnished to do so, subject to the following
dnl conditions:
dnl
dnl The above copyright notice and this permission notice shall be
dnl included in all copies or substantial portions of the Software.
dnl
dnl THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
dnl EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
dnl OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
dnl NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
dnl HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
dnl WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
dnl FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
dnl OTHER DEALINGS IN THE SOFTWARE.

AC_PREREQ(2.61)
AC_INIT([openframeworks],[0.6.0],[http://www.openframeworks.cc])
AC_CONFIG_SRCDIR([libs/openFrameworks/app/ofAppRunner.cpp])

AM_INIT_AUTOMAKE(-Wall -Werror -Wno-portability subdir-objects foreign tar-ustar)

OFX_LIB_VERSION=0:6:0
AC_SUBST([OFX_LIB_VERSION])

LT_INIT
LT_LANG(C++)

PKG_CHECK_MODULES([gstreamer],[gstreamer-0.10])
PKG_CHECK_MODULES([freetype],[freetype2])
PKG_CHECK_MODULES([opencv],[opencv])

AM_CPPFLAGS="${AM_CPPFLAGS} -I\${top_srcdir}/libs/openFrameworks "
AM_CPPFLAGS="${AM_CPPFLAGS} -I\${top_srcdir}/libs/openFrameworks/3d "
AM_CPPFLAGS="${AM_CPPFLAGS} -I\${top_srcdir}/libs/openFrameworks/app "
AM_CPPFLAGS="${AM_CPPFLAGS} -I\${top_srcdir}/libs/openFrameworks/communication "
AM_CPPFLAGS="${AM_CPPFLAGS} -I\${top_srcdir}/libs/openFrameworks/events "
AM_CPPFLAGS="${AM_CPPFLAGS} -I\${top_srcdir}/libs/openFrameworks/graphics "
AM_CPPFLAGS="${AM_CPPFLAGS} -I\${top_srcdir}/libs/openFrameworks/math "
AM_CPPFLAGS="${AM_CPPFLAGS} -I\${top_srcdir}/libs/openFrameworks/sound "
AM_CPPFLAGS="${AM_CPPFLAGS} -I\${top_srcdir}/libs/openFrameworks/types "
AM_CPPFLAGS="${AM_CPPFLAGS} -I\${top_srcdir}/libs/openFrameworks/utils "
AM_CPPFLAGS="${AM_CPPFLAGS} -I\${top_srcdir}/libs/openFrameworks/video "
AM_CPPFLAGS="${AM_CPPFLAGS} ${gstreamer_CFLAGS} ${freetype_CFLAGS}"


AC_SUBST([AM_CPPFLAGS])


AC_DEFINE([SKIP_FMODEX],[1],[Turn off fmodex])

AC_CONFIG_FILES([Makefile libs/Makefile
  addons/ofx3DModelLoader/src/Makefile
  addons/ofxAccelerometer/src/Makefile
  addons/ofxDirList/src/Makefile
  addons/ofxNetwork/src/Makefile
  addons/ofxOpenCv/src/Makefile
  addons/ofxOsc/src/Makefile
  addons/ofxVectorGraphics/Makefile
  addons/ofxXmlSettings/Makefile
])
AC_OUTPUT
