AC_PREREQ(2.63)
AC_INIT([openframeworks],[0.6.0],[http://www.openframeworks.cc])
AC_CONFIG_SRCDIR([libs/openFrameworks/app/ofAppRunner.cpp])

AM_INIT_AUTOMAKE(-Wall -Werror -Wno-portability subdir-objects foreign tar-ustar)

LT_INIT
LT_LANG(C++)

PKG_CHECK_MODULES([gstreamer],[gstreamer-0.10])
PKG_CHECK_MODULES([freetype],[freetype2])

AC_DEFINE([SKIP_FMODEX],[1],[Turn off fmodex])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
