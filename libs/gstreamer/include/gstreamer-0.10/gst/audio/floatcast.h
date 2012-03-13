/* GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
 * Library       <2002> Steve Baker <stevebaker_org@yahoo.co.uk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:gstfloatcast
 * @short_description: Floating point platform independence macros
 *
 * The floatcast.h header file contains a couple of convenience macros for
 * floating point numbers.
 *
 * If you include this header, your application or library must link against
 * libm (for maths.h support).
 *
 * For optimal results, your application&apos;s or library&apos;s build
 * system should check whether the C99 functions lrint and lrintf are supported
 * and define the preprocessor symbols HAVE_LRINT and HAVE_LRINTF if so.  If
 * you are using autoconf, you can do this by using the AC_C99_FUNC_LRINT and
 * AC_C99_FUNC_LRINTF checks in your configure.ac or configure.in file and
 * including your application's config.h header before you include floatcast.h.
 */

#ifndef __FLOATCAST_H__
#define __FLOATCAST_H__

#include <string.h>
#include <glib.h>

#if defined (_MSC_VER) && !defined (inline)
#define inline __inline
#endif

G_BEGIN_DECLS

/* FIXME 0.11: these gst_cast_*() functions are not used anywhere, so we could
 * just as well get rid of them and move the float/double swap macros into
 * gstutils.h in core */

/**
 * gst_cast_float:
 * @x: input value
 *
 * Casts a 32-bit floating point value (float) to an integer without bias.
 */
/**
 * gst_cast_double:
 * @x: input value
 *
 * Casts a 64-bit floating point value (double) to an integer without bias.
 */

/* FIXME: HAVE_LRINT && HAVE_LRINTF are defined by config.h - they should
 * not be used in an installed header. */
#if defined(HAVE_LRINT) && defined(HAVE_LRINTF)

        /*      These defines enable functionality introduced with the 1999 ISO C
        **      standard. They must be defined before the inclusion of math.h to
        **      engage them. If optimisation is enabled, these functions will be 
        **      inlined. With optimisation switched off, you have to link in the
        **      maths library using -lm.
        */

        #define _ISOC9X_SOURCE  1
        #define _ISOC99_SOURCE  1

        #define __USE_ISOC9X    1
        #define __USE_ISOC99    1

        #include        <math.h>

        #define gst_cast_float(x)       ((gint)lrintf(x))
        #define gst_cast_double(x)      ((gint)lrint(x))

#else
        #include <math.h>

        /* use a standard c cast, but do rounding correctly */
        #define gst_cast_float(x)       ((gint)floor((x)+0.5))
        #define gst_cast_double(x)      ((gint)floor((x)+0.5))

#endif

G_END_DECLS

#endif /* __FLOATCAST_H__ */

