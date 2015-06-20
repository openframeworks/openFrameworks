/* Cairo - a vector graphics library with display and print output
 *
 * Copyright © 2007 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335, USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 * The Original Code is the cairo graphics library.
 *
 * The Initial Developer of the Original Code is Red Hat, Inc.
 */

#ifndef CAIRO_XLIB_XRENDER_PRIVATE_H
#define CAIRO_XLIB_XRENDER_PRIVATE_H

#include "cairo-features.h"
#include "cairo-compiler-private.h"

#include <X11/Xlib.h>

/* These prototypes are used when defining interfaces missing from the
 * render headers.  As it happens, it is the case that all libxrender
 * functions take a pointer as first argument. */

__attribute__((__unused__)) static void   _void_consume        (void *p, ...)   { }
__attribute__((__unused__)) static void * _voidp_consume       (void *p, ...)   { return (void *)0; }
__attribute__((__unused__)) static int    _int_consume         (void *p, ...)   { return 0; }
__attribute__((__unused__)) static void   _void_consume_free   (Display *p, XID n) { }


#if CAIRO_HAS_XLIB_XRENDER_SURFACE

#include "cairo-xlib-xrender.h"

#include <X11/extensions/Xrender.h>
#include <X11/extensions/renderproto.h>

/* We require Render >= 0.6.  The following defines were only added in
 * 0.10.  Make sure they are defined.
 */

/* Filters included in 0.10 */
#ifndef FilterConvolution
#define FilterConvolution		    "convolution"
#endif

/* Extended repeat attributes included in 0.10 */
#ifndef RepeatNone
#define RepeatNone                          0
#define RepeatNormal                        1
#define RepeatPad                           2
#define RepeatReflect                       3
#endif


#ifndef PictOptBlendMinimum
/*
 * Operators only available in version 0.11
 */
#define PictOpBlendMinimum			    0x30
#define PictOpMultiply				    0x30
#define PictOpScreen				    0x31
#define PictOpOverlay				    0x32
#define PictOpDarken				    0x33
#define PictOpLighten				    0x34
#define PictOpColorDodge			    0x35
#define PictOpColorBurn				    0x36
#define PictOpHardLight				    0x37
#define PictOpSoftLight				    0x38
#define PictOpDifference			    0x39
#define PictOpExclusion				    0x3a
#define PictOpHSLHue				    0x3b
#define PictOpHSLSaturation			    0x3c
#define PictOpHSLColor				    0x3d
#define PictOpHSLLuminosity			    0x3e
#define PictOpBlendMaximum			    0x3e
#endif

#if !HAVE_XRENDERCREATELINEARGRADIENT
#define XRenderCreateLinearGradient			_int_consume

typedef struct _XLinearGradient {
    XPointFixed p1;
    XPointFixed p2;
} XLinearGradient;
#endif

#if !HAVE_XRENDERCREATERADIALGRADIENT
#define XRenderCreateRadialGradient			_int_consume

typedef struct _XCircle {
    XFixed x;
    XFixed y;
    XFixed radius;
} XCircle;
typedef struct _XRadialGradient {
    XCircle inner;
    XCircle outer;
} XRadialGradient;
#endif

#if !HAVE_XRENDERCREATECONICALGRADIENT
#define XRenderCreateConicalGradient			_int_consume

typedef struct _XConicalGradient {
    XPointFixed center;
    XFixed angle; /* in degrees */
} XConicalGradient;
#endif


#else /* !CAIRO_HAS_XLIB_XRENDER_SURFACE */

/* Provide dummy symbols and macros to get it compile and take the fallback
 * route, just like as if Xrender is not available in the server at run-time. */


/* Functions */

#define XRenderQueryExtension				_int_consume
#define XRenderQueryVersion				_int_consume
#define XRenderQueryFormats				_int_consume
#define XRenderQuerySubpixelOrder			_int_consume
#define XRenderSetSubpixelOrder				_int_consume
#define XRenderFindVisualFormat				_voidp_consume
#define XRenderFindFormat				_voidp_consume
#define XRenderFindStandardFormat			_voidp_consume
#define XRenderQueryPictIndexValues			_voidp_consume
#define XRenderCreatePicture				_int_consume
#define XRenderChangePicture				_void_consume
#define XRenderSetPictureClipRectangles			_void_consume
#define XRenderSetPictureClipRegion			_void_consume
#define XRenderSetPictureTransform			_void_consume
#define XRenderFreePicture				_void_consume_free
#define XRenderComposite				_void_consume
#define XRenderCreateGlyphSet				_int_consume
#define XRenderReferenceGlyphSet			_int_consume
#define XRenderFreeGlyphSet				_void_consume_free
#define XRenderAddGlyphs				_void_consume
#define XRenderFreeGlyphs				_void_consume
#define XRenderCompositeString8				_void_consume
#define XRenderCompositeString16			_void_consume
#define XRenderCompositeString32			_void_consume
#define XRenderCompositeText8				(cairo_xrender_composite_text_func_t) _void_consume
#define XRenderCompositeText16				_void_consume
#define XRenderCompositeText32				_void_consume
#define XRenderFillRectangle				_void_consume
#define XRenderFillRectangles				_void_consume
#define XRenderCompositeTrapezoids			_void_consume
#define XRenderCompositeTriangles			_void_consume
#define XRenderCompositeTriStrip			_void_consume
#define XRenderCompositeTriFan				_void_consume
#define XRenderCompositeDoublePoly			_void_consume
#define XRenderParseColor				_int_consume
#define XRenderCreateCursor				_int_consume
#define XRenderQueryFilters				_voidp_consume
#define XRenderSetPictureFilter				_void_consume
#define XRenderCreateAnimCursor				_int_consume
#define XRenderAddTraps					_void_consume
#define XRenderCreateSolidFill				_int_consume
#define XRenderCreateLinearGradient			_int_consume
#define XRenderCreateRadialGradient			_int_consume
#define XRenderCreateConicalGradient			_int_consume

#define cairo_xlib_surface_create_with_xrender_format	_voidp_consume



/* The rest of this file is copied from various Xrender header files, with
 * the following copyright/license information:
 *
 * Copyright © 2000 SuSE, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of SuSE not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  SuSE makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * SuSE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL SuSE
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, SuSE, Inc.
 */


/* Copied from X11/extensions/render.h */

typedef unsigned long	Glyph;
typedef unsigned long	GlyphSet;
typedef unsigned long	Picture;
typedef unsigned long	PictFormat;

#define BadPictFormat			    0
#define BadPicture			    1
#define BadPictOp			    2
#define BadGlyphSet			    3
#define BadGlyph			    4
#define RenderNumberErrors		    (BadGlyph+1)

#define PictTypeIndexed			    0
#define PictTypeDirect			    1

#define PictOpMinimum			    0
#define PictOpClear			    0
#define PictOpSrc			    1
#define PictOpDst			    2
#define PictOpOver			    3
#define PictOpOverReverse		    4
#define PictOpIn			    5
#define PictOpInReverse			    6
#define PictOpOut			    7
#define PictOpOutReverse		    8
#define PictOpAtop			    9
#define PictOpAtopReverse		    10
#define PictOpXor			    11
#define PictOpAdd			    12
#define PictOpSaturate			    13
#define PictOpMaximum			    13

/*
 * Operators only available in version 0.2
 */
#define PictOpDisjointMinimum			    0x10
#define PictOpDisjointClear			    0x10
#define PictOpDisjointSrc			    0x11
#define PictOpDisjointDst			    0x12
#define PictOpDisjointOver			    0x13
#define PictOpDisjointOverReverse		    0x14
#define PictOpDisjointIn			    0x15
#define PictOpDisjointInReverse			    0x16
#define PictOpDisjointOut			    0x17
#define PictOpDisjointOutReverse		    0x18
#define PictOpDisjointAtop			    0x19
#define PictOpDisjointAtopReverse		    0x1a
#define PictOpDisjointXor			    0x1b
#define PictOpDisjointMaximum			    0x1b

#define PictOpConjointMinimum			    0x20
#define PictOpConjointClear			    0x20
#define PictOpConjointSrc			    0x21
#define PictOpConjointDst			    0x22
#define PictOpConjointOver			    0x23
#define PictOpConjointOverReverse		    0x24
#define PictOpConjointIn			    0x25
#define PictOpConjointInReverse			    0x26
#define PictOpConjointOut			    0x27
#define PictOpConjointOutReverse		    0x28
#define PictOpConjointAtop			    0x29
#define PictOpConjointAtopReverse		    0x2a
#define PictOpConjointXor			    0x2b
#define PictOpConjointMaximum			    0x2b

/*
 * Operators only available in version 0.11
 */
#define PictOpBlendMinimum			    0x30
#define PictOpMultiply				    0x30
#define PictOpScreen				    0x31
#define PictOpOverlay				    0x32
#define PictOpDarken				    0x33
#define PictOpLighten				    0x34
#define PictOpColorDodge			    0x35
#define PictOpColorBurn				    0x36
#define PictOpHardLight				    0x37
#define PictOpSoftLight				    0x38
#define PictOpDifference			    0x39
#define PictOpExclusion				    0x3a
#define PictOpHSLHue				    0x3b
#define PictOpHSLSaturation			    0x3c
#define PictOpHSLColor				    0x3d
#define PictOpHSLLuminosity			    0x3e
#define PictOpBlendMaximum			    0x3e

#define PolyEdgeSharp			    0
#define PolyEdgeSmooth			    1

#define PolyModePrecise			    0
#define PolyModeImprecise		    1

#define CPRepeat			    (1 << 0)
#define CPAlphaMap			    (1 << 1)
#define CPAlphaXOrigin			    (1 << 2)
#define CPAlphaYOrigin			    (1 << 3)
#define CPClipXOrigin			    (1 << 4)
#define CPClipYOrigin			    (1 << 5)
#define CPClipMask			    (1 << 6)
#define CPGraphicsExposure		    (1 << 7)
#define CPSubwindowMode			    (1 << 8)
#define CPPolyEdge			    (1 << 9)
#define CPPolyMode			    (1 << 10)
#define CPDither			    (1 << 11)
#define CPComponentAlpha		    (1 << 12)
#define CPLastBit			    12

/* Filters included in 0.6 */
#define FilterNearest			    "nearest"
#define FilterBilinear			    "bilinear"
/* Filters included in 0.10 */
#define FilterConvolution		    "convolution"

#define FilterFast			    "fast"
#define FilterGood			    "good"
#define FilterBest			    "best"

#define FilterAliasNone			    -1

/* Subpixel orders included in 0.6 */
#define SubPixelUnknown			    0
#define SubPixelHorizontalRGB		    1
#define SubPixelHorizontalBGR		    2
#define SubPixelVerticalRGB		    3
#define SubPixelVerticalBGR		    4
#define SubPixelNone			    5

/* Extended repeat attributes included in 0.10 */
#define RepeatNone                          0
#define RepeatNormal                        1
#define RepeatPad                           2
#define RepeatReflect                       3



/* Copied from X11/extensions/Xrender.h */

typedef struct {
    short   red;
    short   redMask;
    short   green;
    short   greenMask;
    short   blue;
    short   blueMask;
    short   alpha;
    short   alphaMask;
} XRenderDirectFormat;

typedef struct {
    PictFormat		id;
    int			type;
    int			depth;
    XRenderDirectFormat	direct;
    Colormap		colormap;
} XRenderPictFormat;

#define PictFormatID	    (1 << 0)
#define PictFormatType	    (1 << 1)
#define PictFormatDepth	    (1 << 2)
#define PictFormatRed	    (1 << 3)
#define PictFormatRedMask   (1 << 4)
#define PictFormatGreen	    (1 << 5)
#define PictFormatGreenMask (1 << 6)
#define PictFormatBlue	    (1 << 7)
#define PictFormatBlueMask  (1 << 8)
#define PictFormatAlpha	    (1 << 9)
#define PictFormatAlphaMask (1 << 10)
#define PictFormatColormap  (1 << 11)

typedef struct _XRenderPictureAttributes {
    int 		repeat;
    Picture		alpha_map;
    int			alpha_x_origin;
    int			alpha_y_origin;
    int			clip_x_origin;
    int			clip_y_origin;
    Pixmap		clip_mask;
    Bool		graphics_exposures;
    int			subwindow_mode;
    int			poly_edge;
    int			poly_mode;
    Atom		dither;
    Bool		component_alpha;
} XRenderPictureAttributes;

typedef struct {
    unsigned short   red;
    unsigned short   green;
    unsigned short   blue;
    unsigned short   alpha;
} XRenderColor;

typedef struct _XGlyphInfo {
    unsigned short  width;
    unsigned short  height;
    short	    x;
    short	    y;
    short	    xOff;
    short	    yOff;
} XGlyphInfo;

typedef struct _XGlyphElt8 {
    GlyphSet		    glyphset;
    _Xconst char	    *chars;
    int			    nchars;
    int			    xOff;
    int			    yOff;
} XGlyphElt8;

typedef struct _XGlyphElt16 {
    GlyphSet		    glyphset;
    _Xconst unsigned short  *chars;
    int			    nchars;
    int			    xOff;
    int			    yOff;
} XGlyphElt16;

typedef struct _XGlyphElt32 {
    GlyphSet		    glyphset;
    _Xconst unsigned int    *chars;
    int			    nchars;
    int			    xOff;
    int			    yOff;
} XGlyphElt32;

typedef double	XDouble;

typedef struct _XPointDouble {
    XDouble  x, y;
} XPointDouble;

#define XDoubleToFixed(f)    ((XFixed) ((f) * 65536))
#define XFixedToDouble(f)    (((XDouble) (f)) / 65536)

typedef int XFixed;

typedef struct _XPointFixed {
    XFixed  x, y;
} XPointFixed;

typedef struct _XLineFixed {
    XPointFixed	p1, p2;
} XLineFixed;

typedef struct _XTriangle {
    XPointFixed	p1, p2, p3;
} XTriangle;

typedef struct _XCircle {
    XFixed x;
    XFixed y;
    XFixed radius;
} XCircle;

typedef struct _XTrapezoid {
    XFixed  top, bottom;
    XLineFixed	left, right;
} XTrapezoid;

typedef struct _XTransform {
    XFixed  matrix[3][3];
} XTransform;

typedef struct _XFilters {
    int	    nfilter;
    char    **filter;
    int	    nalias;
    short   *alias;
} XFilters;

typedef struct _XIndexValue {
    unsigned long    pixel;
    unsigned short   red, green, blue, alpha;
} XIndexValue;

typedef struct _XAnimCursor {
    Cursor	    cursor;
    unsigned long   delay;
} XAnimCursor;

typedef struct _XSpanFix {
    XFixed	    left, right, y;
} XSpanFix;

typedef struct _XTrap {
    XSpanFix	    top, bottom;
} XTrap;

typedef struct _XLinearGradient {
    XPointFixed p1;
    XPointFixed p2;
} XLinearGradient;

typedef struct _XRadialGradient {
    XCircle inner;
    XCircle outer;
} XRadialGradient;

typedef struct _XConicalGradient {
    XPointFixed center;
    XFixed angle; /* in degrees */
} XConicalGradient;

#define PictStandardARGB32  0
#define PictStandardRGB24   1
#define PictStandardA8	    2
#define PictStandardA4	    3
#define PictStandardA1	    4
#define PictStandardNUM	    5



/* Copied from X11/extensions/renderproto.h */

#include <X11/Xmd.h>

#define Window CARD32
#define Drawable CARD32
#define Font CARD32
#define Pixmap CARD32
#define Cursor CARD32
#define Colormap CARD32
#define GContext CARD32
#define Atom CARD32
#define VisualID CARD32
#define Time CARD32
#define KeyCode CARD8
#define KeySym CARD32

#define Picture	    CARD32
#define PictFormat  CARD32
#define Fixed	    INT32
#define Glyphset    CARD32
#define Glyph	    CARD32

/*
 * data structures
 */

typedef struct {
    CARD16  red B16;
    CARD16  redMask B16;
    CARD16  green B16;
    CARD16  greenMask B16;
    CARD16  blue B16;
    CARD16  blueMask B16;
    CARD16  alpha B16;
    CARD16  alphaMask B16;
} xDirectFormat;

#define sz_xDirectFormat    16

typedef struct {
    PictFormat	id B32;
    CARD8	type;
    CARD8	depth;
    CARD16	pad1 B16;
    xDirectFormat   direct;
    Colormap	colormap;
} xPictFormInfo;

#define sz_xPictFormInfo    28

typedef struct {
    VisualID	visual;
    PictFormat	format;
} xPictVisual;

#define sz_xPictVisual	    8

typedef struct {
    CARD8	depth;
    CARD8	pad1;
    CARD16	nPictVisuals B16;
    CARD32	pad2 B32;
} xPictDepth;

#define sz_xPictDepth	8

typedef struct {
    CARD32	nDepth B32;
    PictFormat	fallback B32;
} xPictScreen;

#define sz_xPictScreen	8

typedef struct {
    CARD32	pixel B32;
    CARD16	red B16;
    CARD16	green B16;
    CARD16	blue B16;
    CARD16	alpha B16;
} xIndexValue;

#define sz_xIndexValue	12

typedef struct {
    CARD16	red B16;
    CARD16	green B16;
    CARD16	blue B16;
    CARD16	alpha B16;
} xRenderColor;

#define sz_xRenderColor	8

typedef struct {
    Fixed	x B32;
    Fixed	y B32;
} xPointFixed;

#define sz_xPointFixed	8

typedef struct {
    xPointFixed	p1;
    xPointFixed p2;
} xLineFixed;

#define sz_xLineFixed	16

typedef struct {
    xPointFixed	p1, p2, p3;
} xTriangle;

#define sz_xTriangle	24

typedef struct {
    Fixed	top B32;
    Fixed	bottom B32;
    xLineFixed	left;
    xLineFixed	right;
} xTrapezoid;

#define sz_xTrapezoid	40

typedef struct {
    CARD16  width B16;
    CARD16  height B16;
    INT16   x B16;
    INT16   y B16;
    INT16   xOff B16;
    INT16   yOff B16;
} xGlyphInfo;

#define sz_xGlyphInfo	12

typedef struct {
    CARD8   len;
    CARD8   pad1;
    CARD16  pad2;
    INT16   deltax;
    INT16   deltay;
} xGlyphElt;

#define sz_xGlyphElt	8

typedef struct {
    Fixed   l, r, y;
} xSpanFix;

#define sz_xSpanFix	12

typedef struct {
    xSpanFix	top, bot;
} xTrap;

#define sz_xTrap	24

/*
 * requests and replies
 */
typedef struct {
    CARD8   reqType;
    CARD8   renderReqType;
    CARD16  length B16;
    CARD32  majorVersion B32;
    CARD32  minorVersion B32;
} xRenderQueryVersionReq;

#define sz_xRenderQueryVersionReq   12

typedef struct {
    BYTE    type;   /* X_Reply */
    BYTE    pad1;
    CARD16  sequenceNumber B16;
    CARD32  length B32;
    CARD32  majorVersion B32;
    CARD32  minorVersion B32;
    CARD32  pad2 B32;
    CARD32  pad3 B32;
    CARD32  pad4 B32;
    CARD32  pad5 B32;
} xRenderQueryVersionReply;

#define sz_xRenderQueryVersionReply	32

typedef struct {
    CARD8   reqType;
    CARD8   renderReqType;
    CARD16  length B16;
} xRenderQueryPictFormatsReq;

#define sz_xRenderQueryPictFormatsReq	4

typedef struct {
    BYTE    type;   /* X_Reply */
    BYTE    pad1;
    CARD16  sequenceNumber B16;
    CARD32  length B32;
    CARD32  numFormats B32;
    CARD32  numScreens B32;
    CARD32  numDepths B32;
    CARD32  numVisuals B32;
    CARD32  numSubpixel B32;	    /* Version 0.6 */
    CARD32  pad5 B32;
} xRenderQueryPictFormatsReply;

#define sz_xRenderQueryPictFormatsReply	32

typedef struct {
    CARD8   reqType;
    CARD8   renderReqType;
    CARD16  length B16;
    PictFormat	format B32;
} xRenderQueryPictIndexValuesReq;

#define sz_xRenderQueryPictIndexValuesReq   8

typedef struct {
    BYTE    type;   /* X_Reply */
    BYTE    pad1;
    CARD16  sequenceNumber B16;
    CARD32  length B32;
    CARD32  numIndexValues;
    CARD32  pad2 B32;
    CARD32  pad3 B32;
    CARD32  pad4 B32;
    CARD32  pad5 B32;
    CARD32  pad6 B32;
} xRenderQueryPictIndexValuesReply;

#define sz_xRenderQueryPictIndexValuesReply 32

typedef struct {
    CARD8	reqType;
    CARD8	renderReqType;
    CARD16	length B16;
    Picture	pid B32;
    Drawable	drawable B32;
    PictFormat	format B32;
    CARD32	mask B32;
} xRenderCreatePictureReq;

#define sz_xRenderCreatePictureReq	    20

typedef struct {
    CARD8	reqType;
    CARD8	renderReqType;
    CARD16	length B16;
    Picture	picture B32;
    CARD32	mask B32;
} xRenderChangePictureReq;

#define sz_xRenderChangePictureReq	    12

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    Picture     picture B32;
    INT16	xOrigin B16;
    INT16	yOrigin B16;
} xRenderSetPictureClipRectanglesReq;

#define sz_xRenderSetPictureClipRectanglesReq	    12

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    Picture     picture B32;
} xRenderFreePictureReq;

#define sz_xRenderFreePictureReq	    8

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    CARD8	op;
    CARD8	pad1;
    CARD16	pad2 B16;
    Picture	src B32;
    Picture	mask B32;
    Picture	dst B32;
    INT16	xSrc B16;
    INT16	ySrc B16;
    INT16	xMask B16;
    INT16	yMask B16;
    INT16	xDst B16;
    INT16	yDst B16;
    CARD16	width B16;
    CARD16	height B16;
} xRenderCompositeReq;

#define sz_xRenderCompositeReq		    36

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    Picture	src B32;
    Picture	dst B32;
    CARD32	colorScale B32;
    CARD32	alphaScale B32;
    INT16	xSrc B16;
    INT16	ySrc B16;
    INT16	xDst B16;
    INT16	yDst B16;
    CARD16	width B16;
    CARD16	height B16;
} xRenderScaleReq;

#define sz_xRenderScaleReq			    32

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    CARD8	op;
    CARD8	pad1;
    CARD16	pad2 B16;
    Picture	src B32;
    Picture	dst B32;
    PictFormat	maskFormat B32;
    INT16	xSrc B16;
    INT16	ySrc B16;
} xRenderTrapezoidsReq;

#define sz_xRenderTrapezoidsReq			    24

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    CARD8	op;
    CARD8	pad1;
    CARD16	pad2 B16;
    Picture	src B32;
    Picture	dst B32;
    PictFormat	maskFormat B32;
    INT16	xSrc B16;
    INT16	ySrc B16;
} xRenderTrianglesReq;

#define sz_xRenderTrianglesReq			    24

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    CARD8	op;
    CARD8	pad1;
    CARD16	pad2 B16;
    Picture	src B32;
    Picture	dst B32;
    PictFormat	maskFormat B32;
    INT16	xSrc B16;
    INT16	ySrc B16;
} xRenderTriStripReq;

#define sz_xRenderTriStripReq			    24

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    CARD8	op;
    CARD8	pad1;
    CARD16	pad2 B16;
    Picture	src B32;
    Picture	dst B32;
    PictFormat	maskFormat B32;
    INT16	xSrc B16;
    INT16	ySrc B16;
} xRenderTriFanReq;

#define sz_xRenderTriFanReq			    24

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    Glyphset	gsid B32;
    PictFormat	format B32;
} xRenderCreateGlyphSetReq;

#define sz_xRenderCreateGlyphSetReq		    12

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    Glyphset    gsid B32;
    Glyphset    existing B32;
} xRenderReferenceGlyphSetReq;

#define sz_xRenderReferenceGlyphSetReq		    24

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    Glyphset    glyphset B32;
} xRenderFreeGlyphSetReq;

#define sz_xRenderFreeGlyphSetReq		    8

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    Glyphset    glyphset B32;
    CARD32	nglyphs;
} xRenderAddGlyphsReq;

#define sz_xRenderAddGlyphsReq			    12

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    Glyphset    glyphset B32;
} xRenderFreeGlyphsReq;

#define sz_xRenderFreeGlyphsReq			    8

typedef struct {
    CARD8       reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    CARD8	op;
    CARD8	pad1;
    CARD16	pad2 B16;
    Picture	src B32;
    Picture	dst B32;
    PictFormat	maskFormat B32;
    Glyphset    glyphset B32;
    INT16	xSrc B16;
    INT16	ySrc B16;
} xRenderCompositeGlyphsReq, xRenderCompositeGlyphs8Req,
xRenderCompositeGlyphs16Req, xRenderCompositeGlyphs32Req;

#define sz_xRenderCompositeGlyphs8Req		    28
#define sz_xRenderCompositeGlyphs16Req		    28
#define sz_xRenderCompositeGlyphs32Req		    28

/* 0.1 and higher */

typedef struct {
    CARD8	reqType;
    CARD8       renderReqType;
    CARD16      length B16;
    CARD8	op;
    CARD8	pad1;
    CARD16	pad2 B16;
    Picture	dst B32;
    xRenderColor    color;
} xRenderFillRectanglesReq;

#define sz_xRenderFillRectanglesReq		    20

/* 0.5 and higher */

typedef struct {
    CARD8	reqType;
    CARD8	renderReqType;
    CARD16	length B16;
    Cursor	cid B32;
    Picture	src B32;
    CARD16	x B16;
    CARD16	y B16;
} xRenderCreateCursorReq;

#define sz_xRenderCreateCursorReq		    16

/* 0.6 and higher */

/*
 * This can't use an array because 32-bit values may be in bitfields
 */
typedef struct {
    Fixed	matrix11 B32;
    Fixed	matrix12 B32;
    Fixed	matrix13 B32;
    Fixed	matrix21 B32;
    Fixed	matrix22 B32;
    Fixed	matrix23 B32;
    Fixed	matrix31 B32;
    Fixed	matrix32 B32;
    Fixed	matrix33 B32;
} xRenderTransform;

#define sz_xRenderTransform 36

typedef struct {
    CARD8		reqType;
    CARD8		renderReqType;
    CARD16		length B16;
    Picture		picture B32;
    xRenderTransform	transform;
} xRenderSetPictureTransformReq;

#define sz_xRenderSetPictureTransformReq	    44

typedef struct {
    CARD8		reqType;
    CARD8		renderReqType;
    CARD16		length B16;
    Drawable		drawable B32;
} xRenderQueryFiltersReq;

#define sz_xRenderQueryFiltersReq		    8

typedef struct {
    BYTE    type;   /* X_Reply */
    BYTE    pad1;
    CARD16  sequenceNumber B16;
    CARD32  length B32;
    CARD32  numAliases B32;	/* LISTofCARD16 */
    CARD32  numFilters B32;	/* LISTofSTRING8 */
    CARD32  pad2 B32;
    CARD32  pad3 B32;
    CARD32  pad4 B32;
    CARD32  pad5 B32;
} xRenderQueryFiltersReply;

#define sz_xRenderQueryFiltersReply		    32

typedef struct {
    CARD8		reqType;
    CARD8		renderReqType;
    CARD16		length B16;
    Picture		picture B32;
    CARD16		nbytes B16; /* number of bytes in name */
    CARD16		pad B16;
} xRenderSetPictureFilterReq;

#define sz_xRenderSetPictureFilterReq		    12

/* 0.8 and higher */

typedef struct {
    Cursor		cursor B32;
    CARD32		delay B32;
} xAnimCursorElt;

#define sz_xAnimCursorElt			    8

typedef struct {
    CARD8		reqType;
    CARD8		renderReqType;
    CARD16		length B16;
    Cursor		cid B32;
} xRenderCreateAnimCursorReq;

#define sz_xRenderCreateAnimCursorReq		    8

/* 0.9 and higher */

typedef struct {
    CARD8		reqType;
    CARD8		renderReqType;
    CARD16		length B16;
    Picture		picture;
    INT16		xOff B16;
    INT16		yOff B16;
} xRenderAddTrapsReq;

#define sz_xRenderAddTrapsReq			    12

/* 0.10 and higher */

typedef struct {
    CARD8	reqType;
    CARD8	renderReqType;
    CARD16	length B16;
    Picture	pid B32;
    xRenderColor color;
} xRenderCreateSolidFillReq;

#define sz_xRenderCreateSolidFillReq                 16

typedef struct {
    CARD8	reqType;
    CARD8	renderReqType;
    CARD16	length B16;
    Picture	pid B32;
    xPointFixed p1;
    xPointFixed p2;
    CARD32      nStops;
} xRenderCreateLinearGradientReq;

#define sz_xRenderCreateLinearGradientReq                 28

typedef struct {
    CARD8	reqType;
    CARD8	renderReqType;
    CARD16	length B16;
    Picture	pid B32;
    xPointFixed inner;
    xPointFixed outer;
    Fixed       inner_radius;
    Fixed       outer_radius;
    CARD32      nStops;
} xRenderCreateRadialGradientReq;

#define sz_xRenderCreateRadialGradientReq                 36

typedef struct {
    CARD8	reqType;
    CARD8	renderReqType;
    CARD16	length B16;
    Picture	pid B32;
    xPointFixed center;
    Fixed       angle; /* in degrees */
    CARD32      nStops;
} xRenderCreateConicalGradientReq;

#define sz_xRenderCreateConicalGradientReq                 24

#undef Window
#undef Drawable
#undef Font
#undef Pixmap
#undef Cursor
#undef Colormap
#undef GContext
#undef Atom
#undef VisualID
#undef Time
#undef KeyCode
#undef KeySym

#undef Picture
#undef PictFormat
#undef Fixed
#undef Glyphset
#undef Glyph


#endif /* CAIRO_HAS_XLIB_XRENDER_SURFACE */

#endif /* CAIRO_XLIB_XRENDER_PRIVATE_H */
