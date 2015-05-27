/*
     File:       GXGraphics.h
 
     Contains:   QuickDraw GX graphic routine interfaces.
 
     Version:    Technology: Quickdraw GX 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __GXGRAPHICS__
#define __GXGRAPHICS__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FONTS__
#include <Fonts.h>
#endif

#ifndef __GXERRORS__
#include <GXErrors.h>
#endif

#ifndef __GXTYPES__
#include <GXTypes.h>
#endif




#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if PRAGMA_IMPORT
#pragma import on
#endif

#if PRAGMA_STRUCT_ALIGN
    #pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

#if defined(__MWERKS__) && TARGET_CPU_68K
    #pragma push
    #pragma pointers_in_D0
#endif
 
#if CALL_NOT_IN_CARBON
EXTERN_API_C( gxGraphicsClient )
GXNewGraphicsClient             (void *                 memoryStart,
                                 long                   memoryLength,
                                 gxClientAttribute      attributes)                         THREEWORDINLINE(0x303C, 0x0059, 0xA832);

EXTERN_API_C( gxGraphicsClient )
GXGetGraphicsClient             (void)                                                      THREEWORDINLINE(0x303C, 0x005A, 0xA832);

EXTERN_API_C( void )
GXSetGraphicsClient             (gxGraphicsClient       client)                             THREEWORDINLINE(0x303C, 0x005B, 0xA832);

EXTERN_API_C( void )
GXDisposeGraphicsClient         (gxGraphicsClient       client)                             THREEWORDINLINE(0x303C, 0x005C, 0xA832);

#endif  /* CALL_NOT_IN_CARBON */

/*returns the count */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( long )
GXGetGraphicsClients            (long                   index,
                                 long                   count,
                                 gxGraphicsClient       clients[])                            THREEWORDINLINE(0x303C, 0x005E, 0xA832);

EXTERN_API_C( void )
GXEnterGraphics                 (void)                                                      THREEWORDINLINE(0x303C, 0x005F, 0xA832);

EXTERN_API_C( void )
GXExitGraphics                  (void)                                                      THREEWORDINLINE(0x303C, 0x0060, 0xA832);

EXTERN_API_C( gxGraphicsError )
GXGetGraphicsError              (gxGraphicsError *      stickyError)                        THREEWORDINLINE(0x303C, 0x0061, 0xA832);

EXTERN_API_C( gxGraphicsNotice )
GXGetGraphicsNotice             (gxGraphicsNotice *     stickyNotice)                       THREEWORDINLINE(0x303C, 0x0062, 0xA832);

EXTERN_API_C( gxGraphicsWarning )
GXGetGraphicsWarning            (gxGraphicsWarning *    stickyWarning)                      THREEWORDINLINE(0x303C, 0x0063, 0xA832);

EXTERN_API_C( void )
GXPostGraphicsError             (gxGraphicsError        error)                              THREEWORDINLINE(0x303C, 0x0064, 0xA832);

EXTERN_API_C( void )
GXPostGraphicsWarning           (gxGraphicsWarning      warning)                            THREEWORDINLINE(0x303C, 0x0066, 0xA832);

EXTERN_API_C( gxUserErrorUPP )
GXGetUserGraphicsError          (long *                 reference)                          THREEWORDINLINE(0x303C, 0x0067, 0xA832);

EXTERN_API_C( gxUserNoticeUPP )
GXGetUserGraphicsNotice         (long *                 reference)                          THREEWORDINLINE(0x303C, 0x0068, 0xA832);

EXTERN_API_C( gxUserWarningUPP )
GXGetUserGraphicsWarning        (long *                 reference)                          THREEWORDINLINE(0x303C, 0x0069, 0xA832);

EXTERN_API_C( void )
GXSetUserGraphicsError          (gxUserErrorUPP         userFunction,
                                 long                   reference)                          THREEWORDINLINE(0x303C, 0x006A, 0xA832);

EXTERN_API_C( void )
GXSetUserGraphicsNotice         (gxUserNoticeUPP        userFunction,
                                 long                   reference)                          THREEWORDINLINE(0x303C, 0x006B, 0xA832);

EXTERN_API_C( void )
GXSetUserGraphicsWarning        (gxUserWarningUPP       userFunction,
                                 long                   reference)                          THREEWORDINLINE(0x303C, 0x006C, 0xA832);

EXTERN_API_C( void )
GXIgnoreGraphicsWarning         (gxGraphicsWarning      warning)                            THREEWORDINLINE(0x303C, 0x006F, 0xA832);

EXTERN_API_C( void )
GXPopGraphicsWarning            (void)                                                      THREEWORDINLINE(0x303C, 0x0070, 0xA832);

EXTERN_API_C( gxShape )
GXNewShapeVector                (gxShapeType            aType,
                                 const Fixed            vector[])                             THREEWORDINLINE(0x303C, 0x0071, 0xA832);

EXTERN_API_C( void )
GXSetShapeVector                (gxShape                target,
                                 const Fixed            vector[])                             THREEWORDINLINE(0x303C, 0x0072, 0xA832);

EXTERN_API_C( gxShape )
GXNewBitmap                     (const gxBitmap *       data,
                                 const gxPoint *        position)                           THREEWORDINLINE(0x303C, 0x0073, 0xA832);

EXTERN_API_C( gxShape )
GXNewCurve                      (const gxCurve *        data)                               THREEWORDINLINE(0x303C, 0x0074, 0xA832);

EXTERN_API_C( gxShape )
GXNewGlyphs                     (long                   charCount,
                                 const unsigned char    text[],
                                 const gxPoint          positions[],
                                 const long             advance[],
                                 const gxPoint          tangents[],
                                 const short            styleRuns[],
                                 const gxStyle          glyphStyles[])                        THREEWORDINLINE(0x303C, 0x0075, 0xA832);

EXTERN_API_C( gxShape )
GXNewLine                       (const gxLine *         data)                               THREEWORDINLINE(0x303C, 0x0076, 0xA832);

EXTERN_API_C( gxShape )
GXNewPaths                      (const gxPaths *        data)                               THREEWORDINLINE(0x303C, 0x0077, 0xA832);

EXTERN_API_C( gxShape )
GXNewPicture                    (long                   count,
                                 const gxShape          shapes[],
                                 const gxStyle          styles[],
                                 const gxInk            inks[],
                                 const gxTransform      transforms[])                         THREEWORDINLINE(0x303C, 0x0078, 0xA832);

EXTERN_API_C( gxShape )
GXNewPoint                      (const gxPoint *        data)                               THREEWORDINLINE(0x303C, 0x0079, 0xA832);

EXTERN_API_C( gxShape )
GXNewPolygons                   (const gxPolygons *     data)                               THREEWORDINLINE(0x303C, 0x007A, 0xA832);

EXTERN_API_C( gxShape )
GXNewRectangle                  (const gxRectangle *    data)                               THREEWORDINLINE(0x303C, 0x007B, 0xA832);

EXTERN_API_C( gxShape )
GXNewText                       (long                   charCount,
                                 const unsigned char    text[],
                                 const gxPoint *        position)                           THREEWORDINLINE(0x303C, 0x007C, 0xA832);

EXTERN_API_C( gxBitmap *)
GXGetBitmap                     (gxShape                source,
                                 gxBitmap *             data,
                                 gxPoint *              position)                           THREEWORDINLINE(0x303C, 0x007D, 0xA832);

EXTERN_API_C( gxCurve *)
GXGetCurve                      (gxShape                source,
                                 gxCurve *              data)                               THREEWORDINLINE(0x303C, 0x007E, 0xA832);

/* returns byte length of glyphs */
EXTERN_API_C( long )
GXGetGlyphs                     (gxShape                source,
                                 long *                 charCount,
                                 unsigned char          text[],
                                 gxPoint                positions[],
                                 long                   advance[],
                                 gxPoint                tangents[],
                                 long *                 runCount,
                                 short                  styleRuns[],
                                 gxStyle                glyphStyles[])                        THREEWORDINLINE(0x303C, 0x007F, 0xA832);

EXTERN_API_C( gxLine *)
GXGetLine                       (gxShape                source,
                                 gxLine *               data)                               THREEWORDINLINE(0x303C, 0x0080, 0xA832);

/* returns byte length */
EXTERN_API_C( long )
GXGetPaths                      (gxShape                source,
                                 gxPaths *              data)                               THREEWORDINLINE(0x303C, 0x0081, 0xA832);

/* returns count */
EXTERN_API_C( long )
GXGetPicture                    (gxShape                source,
                                 gxShape                shapes[],
                                 gxStyle                styles[],
                                 gxInk                  inks[],
                                 gxTransform            transforms[])                         THREEWORDINLINE(0x303C, 0x0082, 0xA832);

EXTERN_API_C( gxPoint *)
GXGetPoint                      (gxShape                source,
                                 gxPoint *              data)                               THREEWORDINLINE(0x303C, 0x0083, 0xA832);

/* returns byte length */
EXTERN_API_C( long )
GXGetPolygons                   (gxShape                source,
                                 gxPolygons *           data)                               THREEWORDINLINE(0x303C, 0x0084, 0xA832);

EXTERN_API_C( gxRectangle *)
GXGetRectangle                  (gxShape                source,
                                 gxRectangle *          data)                               THREEWORDINLINE(0x303C, 0x0085, 0xA832);

/* returns byte length */
EXTERN_API_C( long )
GXGetText                       (gxShape                source,
                                 long *                 charCount,
                                 unsigned char          text[],
                                 gxPoint *              position)                           THREEWORDINLINE(0x303C, 0x0086, 0xA832);

EXTERN_API_C( void )
GXSetBitmap                     (gxShape                target,
                                 const gxBitmap *       data,
                                 const gxPoint *        position)                           THREEWORDINLINE(0x303C, 0x0087, 0xA832);

EXTERN_API_C( void )
GXSetCurve                      (gxShape                target,
                                 const gxCurve *        data)                               THREEWORDINLINE(0x303C, 0x0088, 0xA832);

EXTERN_API_C( void )
GXSetGlyphs                     (gxShape                target,
                                 long                   charCount,
                                 const unsigned char    text[],
                                 const gxPoint          positions[],
                                 const long             advance[],
                                 const gxPoint          tangents[],
                                 const short            styleRuns[],
                                 const gxStyle          glyphStyles[])                        THREEWORDINLINE(0x303C, 0x0089, 0xA832);

EXTERN_API_C( void )
GXSetLine                       (gxShape                target,
                                 const gxLine *         data)                               THREEWORDINLINE(0x303C, 0x008A, 0xA832);

EXTERN_API_C( void )
GXSetPaths                      (gxShape                target,
                                 const gxPaths *        data)                               THREEWORDINLINE(0x303C, 0x008B, 0xA832);

EXTERN_API_C( void )
GXSetPicture                    (gxShape                target,
                                 long                   count,
                                 const gxShape          shapes[],
                                 const gxStyle          styles[],
                                 const gxInk            inks[],
                                 const gxTransform      transforms[])                         THREEWORDINLINE(0x303C, 0x008C, 0xA832);

EXTERN_API_C( void )
GXSetPoint                      (gxShape                target,
                                 const gxPoint *        data)                               THREEWORDINLINE(0x303C, 0x008D, 0xA832);

EXTERN_API_C( void )
GXSetPolygons                   (gxShape                target,
                                 const gxPolygons *     data)                               THREEWORDINLINE(0x303C, 0x008E, 0xA832);

EXTERN_API_C( void )
GXSetRectangle                  (gxShape                target,
                                 const gxRectangle *    data)                               THREEWORDINLINE(0x303C, 0x008F, 0xA832);

EXTERN_API_C( void )
GXSetText                       (gxShape                target,
                                 long                   charCount,
                                 const unsigned char    text[],
                                 const gxPoint *        position)                           THREEWORDINLINE(0x303C, 0x0090, 0xA832);

EXTERN_API_C( void )
GXDrawBitmap                    (const gxBitmap *       data,
                                 const gxPoint *        position)                           THREEWORDINLINE(0x303C, 0x0091, 0xA832);

EXTERN_API_C( void )
GXDrawCurve                     (const gxCurve *        data)                               THREEWORDINLINE(0x303C, 0x0092, 0xA832);

EXTERN_API_C( void )
GXDrawGlyphs                    (long                   charCount,
                                 const unsigned char    text[],
                                 const gxPoint          positions[],
                                 const long             advance[],
                                 const gxPoint          tangents[],
                                 const short            styleRuns[],
                                 const gxStyle          glyphStyles[])                        THREEWORDINLINE(0x303C, 0x0093, 0xA832);

EXTERN_API_C( void )
GXDrawLine                      (const gxLine *         data)                               THREEWORDINLINE(0x303C, 0x0094, 0xA832);

EXTERN_API_C( void )
GXDrawPaths                     (const gxPaths *        data,
                                 gxShapeFill            fill)                               THREEWORDINLINE(0x303C, 0x0095, 0xA832);

EXTERN_API_C( void )
GXDrawPicture                   (long                   count,
                                 const gxShape          shapes[],
                                 const gxStyle          styles[],
                                 const gxInk            inks[],
                                 const gxTransform      transforms[])                         THREEWORDINLINE(0x303C, 0x0096, 0xA832);

EXTERN_API_C( void )
GXDrawPoint                     (const gxPoint *        data)                               THREEWORDINLINE(0x303C, 0x0097, 0xA832);

EXTERN_API_C( void )
GXDrawPolygons                  (const gxPolygons *     data,
                                 gxShapeFill            fill)                               THREEWORDINLINE(0x303C, 0x0098, 0xA832);

EXTERN_API_C( void )
GXDrawRectangle                 (const gxRectangle *    data,
                                 gxShapeFill            fill)                               THREEWORDINLINE(0x303C, 0x0099, 0xA832);

EXTERN_API_C( void )
GXDrawText                      (long                   charCount,
                                 const unsigned char    text[],
                                 const gxPoint *        position)                           THREEWORDINLINE(0x303C, 0x009A, 0xA832);

EXTERN_API_C( gxColorProfile )
GXNewColorProfile               (long                   size,
                                 void *                 colorProfileData)                   THREEWORDINLINE(0x303C, 0x009B, 0xA832);

EXTERN_API_C( gxColorSet )
GXNewColorSet                   (gxColorSpace           space,
                                 long                   count,
                                 const gxSetColor       colors[])                             THREEWORDINLINE(0x303C, 0x009C, 0xA832);

EXTERN_API_C( gxInk )
GXNewInk                        (void)                                                      THREEWORDINLINE(0x303C, 0x009D, 0xA832);

EXTERN_API_C( gxShape )
GXNewShape                      (gxShapeType            aType)                              THREEWORDINLINE(0x303C, 0x009E, 0xA832);

EXTERN_API_C( gxStyle )
GXNewStyle                      (void)                                                      THREEWORDINLINE(0x303C, 0x009F, 0xA832);

EXTERN_API_C( gxTag )
GXNewTag                        (long                   tagType,
                                 long                   length,
                                 const void *           data)                               THREEWORDINLINE(0x303C, 0x00A0, 0xA832);

EXTERN_API_C( gxTransform )
GXNewTransform                  (void)                                                      THREEWORDINLINE(0x303C, 0x00A1, 0xA832);

EXTERN_API_C( gxViewDevice )
GXNewViewDevice                 (gxViewGroup            group,
                                 gxShape                bitmapShape)                        THREEWORDINLINE(0x303C, 0x00A2, 0xA832);

EXTERN_API_C( gxViewGroup )
GXNewViewGroup                  (void)                                                      THREEWORDINLINE(0x303C, 0x00A3, 0xA832);

EXTERN_API_C( gxViewPort )
GXNewViewPort                   (gxViewGroup            group)                              THREEWORDINLINE(0x303C, 0x00A4, 0xA832);

EXTERN_API_C( void )
GXDisposeColorProfile           (gxColorProfile         target)                             THREEWORDINLINE(0x303C, 0x00A5, 0xA832);

EXTERN_API_C( void )
GXDisposeColorSet               (gxColorSet             target)                             THREEWORDINLINE(0x303C, 0x00A6, 0xA832);

EXTERN_API_C( void )
GXDisposeInk                    (gxInk                  target)                             THREEWORDINLINE(0x303C, 0x00A7, 0xA832);

EXTERN_API_C( void )
GXDisposeShape                  (gxShape                target)                             THREEWORDINLINE(0x303C, 0x00A8, 0xA832);

EXTERN_API_C( void )
GXDisposeStyle                  (gxStyle                target)                             THREEWORDINLINE(0x303C, 0x00A9, 0xA832);

EXTERN_API_C( void )
GXDisposeTag                    (gxTag                  target)                             THREEWORDINLINE(0x303C, 0x00AA, 0xA832);

EXTERN_API_C( void )
GXDisposeTransform              (gxTransform            target)                             THREEWORDINLINE(0x303C, 0x00AB, 0xA832);

EXTERN_API_C( void )
GXDisposeViewDevice             (gxViewDevice           target)                             THREEWORDINLINE(0x303C, 0x00AC, 0xA832);

EXTERN_API_C( void )
GXDisposeViewGroup              (gxViewGroup            target)                             THREEWORDINLINE(0x303C, 0x00AD, 0xA832);

EXTERN_API_C( void )
GXDisposeViewPort               (gxViewPort             target)                             THREEWORDINLINE(0x303C, 0x00AE, 0xA832);

EXTERN_API_C( gxColorProfile )
GXCloneColorProfile             (gxColorProfile         source)                             THREEWORDINLINE(0x303C, 0x00AF, 0xA832);

EXTERN_API_C( gxColorSet )
GXCloneColorSet                 (gxColorSet             source)                             THREEWORDINLINE(0x303C, 0x00B0, 0xA832);

EXTERN_API_C( gxInk )
GXCloneInk                      (gxInk                  source)                             THREEWORDINLINE(0x303C, 0x00B1, 0xA832);

EXTERN_API_C( gxShape )
GXCloneShape                    (gxShape                source)                             THREEWORDINLINE(0x303C, 0x00B2, 0xA832);

EXTERN_API_C( gxStyle )
GXCloneStyle                    (gxStyle                source)                             THREEWORDINLINE(0x303C, 0x00B3, 0xA832);

EXTERN_API_C( gxTag )
GXCloneTag                      (gxTag                  source)                             THREEWORDINLINE(0x303C, 0x00B4, 0xA832);

EXTERN_API_C( gxTransform )
GXCloneTransform                (gxTransform            source)                             THREEWORDINLINE(0x303C, 0x00B5, 0xA832);

EXTERN_API_C( gxColorProfile )
GXCopyToColorProfile            (gxColorProfile         target,
                                 gxColorProfile         source)                             THREEWORDINLINE(0x303C, 0x00B6, 0xA832);

EXTERN_API_C( gxColorSet )
GXCopyToColorSet                (gxColorSet             target,
                                 gxColorSet             source)                             THREEWORDINLINE(0x303C, 0x00B7, 0xA832);

EXTERN_API_C( gxInk )
GXCopyToInk                     (gxInk                  target,
                                 gxInk                  source)                             THREEWORDINLINE(0x303C, 0x00B8, 0xA832);

EXTERN_API_C( gxShape )
GXCopyToShape                   (gxShape                target,
                                 gxShape                source)                             THREEWORDINLINE(0x303C, 0x00B9, 0xA832);

EXTERN_API_C( gxStyle )
GXCopyToStyle                   (gxStyle                target,
                                 gxStyle                source)                             THREEWORDINLINE(0x303C, 0x00BA, 0xA832);

EXTERN_API_C( gxTag )
GXCopyToTag                     (gxTag                  target,
                                 gxTag                  source)                             THREEWORDINLINE(0x303C, 0x00BB, 0xA832);

EXTERN_API_C( gxTransform )
GXCopyToTransform               (gxTransform            target,
                                 gxTransform            source)                             THREEWORDINLINE(0x303C, 0x00BC, 0xA832);

EXTERN_API_C( gxViewDevice )
GXCopyToViewDevice              (gxViewDevice           target,
                                 gxViewDevice           source)                             THREEWORDINLINE(0x303C, 0x00BD, 0xA832);

EXTERN_API_C( gxViewPort )
GXCopyToViewPort                (gxViewPort             target,
                                 gxViewPort             source)                             THREEWORDINLINE(0x303C, 0x00BE, 0xA832);

EXTERN_API_C( Boolean )
GXEqualColorProfile             (gxColorProfile         one,
                                 gxColorProfile         two)                                THREEWORDINLINE(0x303C, 0x00BF, 0xA832);

EXTERN_API_C( Boolean )
GXEqualColorSet                 (gxColorSet             one,
                                 gxColorSet             two)                                THREEWORDINLINE(0x303C, 0x00C0, 0xA832);

EXTERN_API_C( Boolean )
GXEqualInk                      (gxInk                  one,
                                 gxInk                  two)                                THREEWORDINLINE(0x303C, 0x00C1, 0xA832);

EXTERN_API_C( Boolean )
GXEqualShape                    (gxShape                one,
                                 gxShape                two)                                THREEWORDINLINE(0x303C, 0x00C2, 0xA832);

EXTERN_API_C( Boolean )
GXEqualStyle                    (gxStyle                one,
                                 gxStyle                two)                                THREEWORDINLINE(0x303C, 0x00C3, 0xA832);

EXTERN_API_C( Boolean )
GXEqualTag                      (gxTag                  one,
                                 gxTag                  two)                                THREEWORDINLINE(0x303C, 0x00C4, 0xA832);

EXTERN_API_C( Boolean )
GXEqualTransform                (gxTransform            one,
                                 gxTransform            two)                                THREEWORDINLINE(0x303C, 0x00C5, 0xA832);

EXTERN_API_C( Boolean )
GXEqualViewDevice               (gxViewDevice           one,
                                 gxViewDevice           two)                                THREEWORDINLINE(0x303C, 0x00C6, 0xA832);

EXTERN_API_C( Boolean )
GXEqualViewPort                 (gxViewPort             one,
                                 gxViewPort             two)                                THREEWORDINLINE(0x303C, 0x00C7, 0xA832);

EXTERN_API_C( void )
GXResetInk                      (gxInk                  target)                             THREEWORDINLINE(0x303C, 0x00C8, 0xA832);

EXTERN_API_C( void )
GXResetShape                    (gxShape                target)                             THREEWORDINLINE(0x303C, 0x00C9, 0xA832);

EXTERN_API_C( void )
GXResetStyle                    (gxStyle                target)                             THREEWORDINLINE(0x303C, 0x00CA, 0xA832);

EXTERN_API_C( void )
GXResetTransform                (gxTransform            target)                             THREEWORDINLINE(0x303C, 0x00CB, 0xA832);

EXTERN_API_C( void )
GXLoadColorProfile              (gxColorProfile         target)                             THREEWORDINLINE(0x303C, 0x00CC, 0xA832);

EXTERN_API_C( void )
GXLoadColorSet                  (gxColorSet             target)                             THREEWORDINLINE(0x303C, 0x00CD, 0xA832);

EXTERN_API_C( void )
GXLoadInk                       (gxInk                  target)                             THREEWORDINLINE(0x303C, 0x00CE, 0xA832);

EXTERN_API_C( void )
GXLoadShape                     (gxShape                target)                             THREEWORDINLINE(0x303C, 0x00CF, 0xA832);

EXTERN_API_C( void )
GXLoadStyle                     (gxStyle                target)                             THREEWORDINLINE(0x303C, 0x00D0, 0xA832);

EXTERN_API_C( void )
GXLoadTag                       (gxTag                  target)                             THREEWORDINLINE(0x303C, 0x00D1, 0xA832);

EXTERN_API_C( void )
GXLoadTransform                 (gxTransform            target)                             THREEWORDINLINE(0x303C, 0x00D2, 0xA832);

EXTERN_API_C( void )
GXUnloadColorProfile            (gxColorProfile         target)                             THREEWORDINLINE(0x303C, 0x00D3, 0xA832);

EXTERN_API_C( void )
GXUnloadColorSet                (gxColorSet             target)                             THREEWORDINLINE(0x303C, 0x00D4, 0xA832);

EXTERN_API_C( void )
GXUnloadInk                     (gxInk                  target)                             THREEWORDINLINE(0x303C, 0x00D5, 0xA832);

EXTERN_API_C( void )
GXUnloadShape                   (gxShape                target)                             THREEWORDINLINE(0x303C, 0x00D6, 0xA832);

EXTERN_API_C( void )
GXUnloadStyle                   (gxStyle                target)                             THREEWORDINLINE(0x303C, 0x00D7, 0xA832);

EXTERN_API_C( void )
GXUnloadTag                     (gxTag                  target)                             THREEWORDINLINE(0x303C, 0x00D8, 0xA832);

EXTERN_API_C( void )
GXUnloadTransform               (gxTransform            target)                             THREEWORDINLINE(0x303C, 0x00D9, 0xA832);

EXTERN_API_C( void )
GXCacheShape                    (gxShape                source)                             THREEWORDINLINE(0x303C, 0x00DA, 0xA832);

EXTERN_API_C( gxShape )
GXCopyDeepToShape               (gxShape                target,
                                 gxShape                source)                             THREEWORDINLINE(0x303C, 0x00DB, 0xA832);

EXTERN_API_C( void )
GXDrawShape                     (gxShape                source)                             THREEWORDINLINE(0x303C, 0x00DC, 0xA832);

EXTERN_API_C( void )
GXDisposeShapeCache             (gxShape                target)                             THREEWORDINLINE(0x303C, 0x00DD, 0xA832);

EXTERN_API_C( gxColorProfile )
GXGetDefaultColorProfile        (void)                                                      THREEWORDINLINE(0x303C, 0x00DE, 0xA832);

EXTERN_API_C( gxShape )
GXGetDefaultShape               (gxShapeType            aType)                              THREEWORDINLINE(0x303C, 0x00DF, 0xA832);

EXTERN_API_C( gxColorSet )
GXGetDefaultColorSet            (long                   pixelDepth)                         THREEWORDINLINE(0x303C, 0x00E0, 0xA832);


EXTERN_API_C( void )
GXSetDefaultShape               (gxShape                target)                             THREEWORDINLINE(0x303C, 0x00E1, 0xA832);

EXTERN_API_C( void )
GXSetDefaultColorSet            (gxColorSet             target,
                                 long                   pixelDepth)                         THREEWORDINLINE(0x303C, 0x00E2, 0xA832);

EXTERN_API_C( long )
GXGetTag                        (gxTag                  source,
                                 long *                 tagType,
                                 void *                 data)                               THREEWORDINLINE(0x303C, 0x00E3, 0xA832);

EXTERN_API_C( void )
GXSetTag                        (gxTag                  target,
                                 long                   tagType,
                                 long                   length,
                                 const void *           data)                               THREEWORDINLINE(0x303C, 0x00E4, 0xA832);

EXTERN_API_C( gxRectangle *)
GXGetShapeBounds                (gxShape                source,
                                 long                   index,
                                 gxRectangle *          bounds)                             THREEWORDINLINE(0x303C, 0x00E5, 0xA832);

EXTERN_API_C( gxShapeFill )
GXGetShapeFill                  (gxShape                source)                             THREEWORDINLINE(0x303C, 0x00E6, 0xA832);

EXTERN_API_C( gxInk )
GXGetShapeInk                   (gxShape                source)                             THREEWORDINLINE(0x303C, 0x00E7, 0xA832);

EXTERN_API_C( long )
GXGetShapePixel                 (gxShape                source,
                                 long                   x,
                                 long                   y,
                                 gxColor *              data,
                                 long *                 index)                              THREEWORDINLINE(0x303C, 0x00E8, 0xA832);

EXTERN_API_C( gxStyle )
GXGetShapeStyle                 (gxShape                source)                             THREEWORDINLINE(0x303C, 0x00E9, 0xA832);

EXTERN_API_C( gxTransform )
GXGetShapeTransform             (gxShape                source)                             THREEWORDINLINE(0x303C, 0x00EA, 0xA832);

EXTERN_API_C( gxShapeType )
GXGetShapeType                  (gxShape                source)                             THREEWORDINLINE(0x303C, 0x00EB, 0xA832);

EXTERN_API_C( gxRectangle *)
GXGetShapeTypographicBounds     (gxShape                source,
                                 gxRectangle *          bounds)                             THREEWORDINLINE(0x303C, 0x00EC, 0xA832);

EXTERN_API_C( gxShape )
GXGetBitmapParts                (gxShape                source,
                                 const gxLongRectangle * bounds)                            THREEWORDINLINE(0x303C, 0x00ED, 0xA832);

EXTERN_API_C( void )
GXGetStyleFontMetrics           (gxStyle                sourceStyle,
                                 gxPoint *              before,
                                 gxPoint *              after,
                                 gxPoint *              caretAngle,
                                 gxPoint *              caretOffset)                        THREEWORDINLINE(0x303C, 0x00EE, 0xA832);

EXTERN_API_C( void )
GXGetShapeFontMetrics           (gxShape                source,
                                 gxPoint *              before,
                                 gxPoint *              after,
                                 gxPoint *              caretAngle,
                                 gxPoint *              caretOffset)                        THREEWORDINLINE(0x303C, 0x00EF, 0xA832);

EXTERN_API_C( void )
GXSetShapeBounds                (gxShape                target,
                                 const gxRectangle *    newBounds)                          THREEWORDINLINE(0x303C, 0x00F0, 0xA832);

EXTERN_API_C( void )
GXSetShapeFill                  (gxShape                target,
                                 gxShapeFill            newFill)                            THREEWORDINLINE(0x303C, 0x00F1, 0xA832);

EXTERN_API_C( void )
GXSetShapeInk                   (gxShape                target,
                                 gxInk                  newInk)                             THREEWORDINLINE(0x303C, 0x00F2, 0xA832);

EXTERN_API_C( void )
GXSetShapePixel                 (gxShape                target,
                                 long                   x,
                                 long                   y,
                                 const gxColor *        newColor,
                                 long                   newIndex)                           THREEWORDINLINE(0x303C, 0x00F3, 0xA832);

EXTERN_API_C( void )
GXSetShapeStyle                 (gxShape                target,
                                 gxStyle                newStyle)                           THREEWORDINLINE(0x303C, 0x00F4, 0xA832);

EXTERN_API_C( void )
GXSetShapeTransform             (gxShape                target,
                                 gxTransform            newTransform)                       THREEWORDINLINE(0x303C, 0x00F5, 0xA832);

EXTERN_API_C( void )
GXSetShapeType                  (gxShape                target,
                                 gxShapeType            newType)                            THREEWORDINLINE(0x303C, 0x00F6, 0xA832);

EXTERN_API_C( void )
GXSetBitmapParts                (gxShape                target,
                                 const gxLongRectangle * bounds,
                                 gxShape                bitmapShape)                        THREEWORDINLINE(0x303C, 0x00F7, 0xA832);

EXTERN_API_C( void )
GXSetShapeGeometry              (gxShape                target,
                                 gxShape                geometry)                           THREEWORDINLINE(0x303C, 0x00F8, 0xA832);

EXTERN_API_C( Fixed )
GXGetShapeCurveError            (gxShape                source)                             THREEWORDINLINE(0x303C, 0x00F9, 0xA832);

EXTERN_API_C( gxDashRecord *)
GXGetShapeDash                  (gxShape                source,
                                 gxDashRecord *         dash)                               THREEWORDINLINE(0x303C, 0x00FA, 0xA832);

EXTERN_API_C( gxCapRecord *)
GXGetShapeCap                   (gxShape                source,
                                 gxCapRecord *          cap)                                THREEWORDINLINE(0x303C, 0x00FB, 0xA832);

/* returns the number of layers */
EXTERN_API_C( long )
GXGetShapeFace                  (gxShape                source,
                                 gxTextFace *           face)                               THREEWORDINLINE(0x303C, 0x00FC, 0xA832);

EXTERN_API_C( gxFont )
GXGetShapeFont                  (gxShape                source)                             THREEWORDINLINE(0x303C, 0x00FD, 0xA832);

EXTERN_API_C( gxJoinRecord *)
GXGetShapeJoin                  (gxShape                source,
                                 gxJoinRecord *         join)                               THREEWORDINLINE(0x303C, 0x00FE, 0xA832);

EXTERN_API_C( Fract )
GXGetShapeJustification         (gxShape                source)                             THREEWORDINLINE(0x303C, 0x00FF, 0xA832);

EXTERN_API_C( gxPatternRecord *)
GXGetShapePattern               (gxShape                source,
                                 gxPatternRecord *      pattern)                            THREEWORDINLINE(0x303C, 0x0100, 0xA832);

EXTERN_API_C( Fixed )
GXGetShapePen                   (gxShape                source)                             THREEWORDINLINE(0x303C, 0x0101, 0xA832);

EXTERN_API_C( gxFontPlatform )
GXGetShapeEncoding              (gxShape                source,
                                 gxFontScript *         script,
                                 gxFontLanguage *       language)                           THREEWORDINLINE(0x303C, 0x0102, 0xA832);

EXTERN_API_C( Fixed )
GXGetShapeTextSize              (gxShape                source)                             THREEWORDINLINE(0x303C, 0x0103, 0xA832);

EXTERN_API_C( long )
GXGetShapeFontVariations        (gxShape                source,
                                 gxFontVariation        variations[])                         THREEWORDINLINE(0x303C, 0x0104, 0xA832);

EXTERN_API_C( long )
GXGetShapeFontVariationSuite    (gxShape                source,
                                 gxFontVariation        variations[])                         THREEWORDINLINE(0x303C, 0x0105, 0xA832);

EXTERN_API_C( Fixed )
GXGetStyleCurveError            (gxStyle                source)                             THREEWORDINLINE(0x303C, 0x0106, 0xA832);

EXTERN_API_C( gxDashRecord *)
GXGetStyleDash                  (gxStyle                source,
                                 gxDashRecord *         dash)                               THREEWORDINLINE(0x303C, 0x0107, 0xA832);

EXTERN_API_C( gxCapRecord *)
GXGetStyleCap                   (gxStyle                source,
                                 gxCapRecord *          cap)                                THREEWORDINLINE(0x303C, 0x0108, 0xA832);

/* returns the number of layers */
EXTERN_API_C( long )
GXGetStyleFace                  (gxStyle                source,
                                 gxTextFace *           face)                               THREEWORDINLINE(0x303C, 0x0109, 0xA832);

EXTERN_API_C( gxFont )
GXGetStyleFont                  (gxStyle                source)                             THREEWORDINLINE(0x303C, 0x010A, 0xA832);

EXTERN_API_C( gxJoinRecord *)
GXGetStyleJoin                  (gxStyle                source,
                                 gxJoinRecord *         join)                               THREEWORDINLINE(0x303C, 0x010B, 0xA832);

EXTERN_API_C( Fract )
GXGetStyleJustification         (gxStyle                source)                             THREEWORDINLINE(0x303C, 0x010C, 0xA832);

EXTERN_API_C( gxPatternRecord *)
GXGetStylePattern               (gxStyle                source,
                                 gxPatternRecord *      pattern)                            THREEWORDINLINE(0x303C, 0x010D, 0xA832);

EXTERN_API_C( Fixed )
GXGetStylePen                   (gxStyle                source)                             THREEWORDINLINE(0x303C, 0x010E, 0xA832);

EXTERN_API_C( gxFontPlatform )
GXGetStyleEncoding              (gxStyle                source,
                                 gxFontScript *         script,
                                 gxFontLanguage *       language)                           THREEWORDINLINE(0x303C, 0x010F, 0xA832);

EXTERN_API_C( Fixed )
GXGetStyleTextSize              (gxStyle                source)                             THREEWORDINLINE(0x303C, 0x0110, 0xA832);

EXTERN_API_C( long )
GXGetStyleFontVariations        (gxStyle                source,
                                 gxFontVariation        variations[])                         THREEWORDINLINE(0x303C, 0x0111, 0xA832);

EXTERN_API_C( long )
GXGetStyleFontVariationSuite    (gxStyle                source,
                                 gxFontVariation        variations[])                         THREEWORDINLINE(0x303C, 0x0112, 0xA832);

EXTERN_API_C( void )
GXSetShapeCurveError            (gxShape                target,
                                 Fixed                  error)                              THREEWORDINLINE(0x303C, 0x0113, 0xA832);

EXTERN_API_C( void )
GXSetShapeDash                  (gxShape                target,
                                 const gxDashRecord *   dash)                               THREEWORDINLINE(0x303C, 0x0114, 0xA832);

EXTERN_API_C( void )
GXSetShapeCap                   (gxShape                target,
                                 const gxCapRecord *    cap)                                THREEWORDINLINE(0x303C, 0x0115, 0xA832);

EXTERN_API_C( void )
GXSetShapeFace                  (gxShape                target,
                                 const gxTextFace *     face)                               THREEWORDINLINE(0x303C, 0x0116, 0xA832);

EXTERN_API_C( void )
GXSetShapeFont                  (gxShape                target,
                                 gxFont                 aFont)                              THREEWORDINLINE(0x303C, 0x0117, 0xA832);

EXTERN_API_C( void )
GXSetShapeJoin                  (gxShape                target,
                                 const gxJoinRecord *   join)                               THREEWORDINLINE(0x303C, 0x0118, 0xA832);

EXTERN_API_C( void )
GXSetShapeJustification         (gxShape                target,
                                 Fract                  justify)                            THREEWORDINLINE(0x303C, 0x0119, 0xA832);

EXTERN_API_C( void )
GXSetShapePattern               (gxShape                target,
                                 const gxPatternRecord * pattern)                           THREEWORDINLINE(0x303C, 0x011A, 0xA832);

EXTERN_API_C( void )
GXSetShapePen                   (gxShape                target,
                                 Fixed                  pen)                                THREEWORDINLINE(0x303C, 0x011B, 0xA832);

EXTERN_API_C( void )
GXSetShapeEncoding              (gxShape                target,
                                 gxFontPlatform         platform,
                                 gxFontScript           script,
                                 gxFontLanguage         language)                           THREEWORDINLINE(0x303C, 0x011C, 0xA832);

EXTERN_API_C( void )
GXSetShapeTextSize              (gxShape                target,
                                 Fixed                  size)                               THREEWORDINLINE(0x303C, 0x011D, 0xA832);

EXTERN_API_C( void )
GXSetShapeFontVariations        (gxShape                target,
                                 long                   count,
                                 const gxFontVariation  variations[])                         THREEWORDINLINE(0x303C, 0x011E, 0xA832);

EXTERN_API_C( void )
GXSetStyleCurveError            (gxStyle                target,
                                 Fixed                  error)                              THREEWORDINLINE(0x303C, 0x011F, 0xA832);

EXTERN_API_C( void )
GXSetStyleDash                  (gxStyle                target,
                                 const gxDashRecord *   dash)                               THREEWORDINLINE(0x303C, 0x0120, 0xA832);

EXTERN_API_C( void )
GXSetStyleCap                   (gxStyle                target,
                                 const gxCapRecord *    cap)                                THREEWORDINLINE(0x303C, 0x0121, 0xA832);

EXTERN_API_C( void )
GXSetStyleFace                  (gxStyle                target,
                                 const gxTextFace *     face)                               THREEWORDINLINE(0x303C, 0x0122, 0xA832);

EXTERN_API_C( void )
GXSetStyleFont                  (gxStyle                target,
                                 gxFont                 aFont)                              THREEWORDINLINE(0x303C, 0x0123, 0xA832);

EXTERN_API_C( void )
GXSetStyleJoin                  (gxStyle                target,
                                 const gxJoinRecord *   join)                               THREEWORDINLINE(0x303C, 0x0124, 0xA832);

EXTERN_API_C( void )
GXSetStyleJustification         (gxStyle                target,
                                 Fract                  justify)                            THREEWORDINLINE(0x303C, 0x0125, 0xA832);

EXTERN_API_C( void )
GXSetStylePattern               (gxStyle                target,
                                 const gxPatternRecord * pattern)                           THREEWORDINLINE(0x303C, 0x0126, 0xA832);

EXTERN_API_C( void )
GXSetStylePen                   (gxStyle                target,
                                 Fixed                  pen)                                THREEWORDINLINE(0x303C, 0x0127, 0xA832);

EXTERN_API_C( void )
GXSetStyleEncoding              (gxStyle                target,
                                 gxFontPlatform         platform,
                                 gxFontScript           script,
                                 gxFontLanguage         language)                           THREEWORDINLINE(0x303C, 0x0128, 0xA832);

EXTERN_API_C( void )
GXSetStyleTextSize              (gxStyle                target,
                                 Fixed                  size)                               THREEWORDINLINE(0x303C, 0x0129, 0xA832);

EXTERN_API_C( void )
GXSetStyleFontVariations        (gxStyle                target,
                                 long                   count,
                                 const gxFontVariation  variations[])                         THREEWORDINLINE(0x303C, 0x012A, 0xA832);

EXTERN_API_C( gxColor *)
GXGetShapeColor                 (gxShape                source,
                                 gxColor *              data)                               THREEWORDINLINE(0x303C, 0x012B, 0xA832);

EXTERN_API_C( gxTransferMode *)
GXGetShapeTransfer              (gxShape                source,
                                 gxTransferMode *       data)                               THREEWORDINLINE(0x303C, 0x012C, 0xA832);

EXTERN_API_C( gxColor *)
GXGetInkColor                   (gxInk                  source,
                                 gxColor *              data)                               THREEWORDINLINE(0x303C, 0x012D, 0xA832);

EXTERN_API_C( gxTransferMode *)
GXGetInkTransfer                (gxInk                  source,
                                 gxTransferMode *       data)                               THREEWORDINLINE(0x303C, 0x012E, 0xA832);

EXTERN_API_C( void )
GXSetShapeColor                 (gxShape                target,
                                 const gxColor *        data)                               THREEWORDINLINE(0x303C, 0x012F, 0xA832);

EXTERN_API_C( void )
GXSetShapeTransfer              (gxShape                target,
                                 const gxTransferMode * data)                               THREEWORDINLINE(0x303C, 0x0130, 0xA832);

EXTERN_API_C( void )
GXSetInkColor                   (gxInk                  target,
                                 const gxColor *        data)                               THREEWORDINLINE(0x303C, 0x0131, 0xA832);

EXTERN_API_C( void )
GXSetInkTransfer                (gxInk                  target,
                                 const gxTransferMode * data)                               THREEWORDINLINE(0x303C, 0x0132, 0xA832);

EXTERN_API_C( gxShape )
GXGetShapeClip                  (gxShape                source)                             THREEWORDINLINE(0x303C, 0x0133, 0xA832);

EXTERN_API_C( gxShapeType )
GXGetShapeClipType              (gxShape                source)                             THREEWORDINLINE(0x303C, 0x0276, 0xA832);

EXTERN_API_C( gxMapping *)
GXGetShapeMapping               (gxShape                source,
                                 gxMapping *            map)                                THREEWORDINLINE(0x303C, 0x0134, 0xA832);

EXTERN_API_C( gxShapePart )
GXGetShapeHitTest               (gxShape                source,
                                 Fixed *                tolerance)                          THREEWORDINLINE(0x303C, 0x0135, 0xA832);

EXTERN_API_C( long )
GXGetShapeViewPorts             (gxShape                source,
                                 gxViewPort             list[])                               THREEWORDINLINE(0x303C, 0x0136, 0xA832);

EXTERN_API_C( gxShape )
GXGetTransformClip              (gxTransform            source)                             THREEWORDINLINE(0x303C, 0x0137, 0xA832);

EXTERN_API_C( gxShapeType )
GXGetTransformClipType          (gxTransform            source)                             THREEWORDINLINE(0x303C, 0x0277, 0xA832);

EXTERN_API_C( gxMapping *)
GXGetTransformMapping           (gxTransform            source,
                                 gxMapping *            map)                                THREEWORDINLINE(0x303C, 0x0138, 0xA832);

EXTERN_API_C( gxShapePart )
GXGetTransformHitTest           (gxTransform            source,
                                 Fixed *                tolerance)                          THREEWORDINLINE(0x303C, 0x0139, 0xA832);

EXTERN_API_C( long )
GXGetTransformViewPorts         (gxTransform            source,
                                 gxViewPort             list[])                               THREEWORDINLINE(0x303C, 0x013A, 0xA832);

EXTERN_API_C( void )
GXSetShapeClip                  (gxShape                target,
                                 gxShape                clip)                               THREEWORDINLINE(0x303C, 0x013B, 0xA832);

EXTERN_API_C( void )
GXSetShapeMapping               (gxShape                target,
                                 const gxMapping *      map)                                THREEWORDINLINE(0x303C, 0x013C, 0xA832);

EXTERN_API_C( void )
GXSetShapeHitTest               (gxShape                target,
                                 gxShapePart            mask,
                                 Fixed                  tolerance)                          THREEWORDINLINE(0x303C, 0x013D, 0xA832);

EXTERN_API_C( void )
GXSetShapeViewPorts             (gxShape                target,
                                 long                   count,
                                 const gxViewPort       list[])                               THREEWORDINLINE(0x303C, 0x013E, 0xA832);

EXTERN_API_C( void )
GXSetTransformClip              (gxTransform            target,
                                 gxShape                clip)                               THREEWORDINLINE(0x303C, 0x013F, 0xA832);

EXTERN_API_C( void )
GXSetTransformMapping           (gxTransform            target,
                                 const gxMapping *      map)                                THREEWORDINLINE(0x303C, 0x0140, 0xA832);

EXTERN_API_C( void )
GXSetTransformHitTest           (gxTransform            target,
                                 gxShapePart            mask,
                                 Fixed                  tolerance)                          THREEWORDINLINE(0x303C, 0x0141, 0xA832);

EXTERN_API_C( void )
GXSetTransformViewPorts         (gxTransform            target,
                                 long                   count,
                                 const gxViewPort       list[])                               THREEWORDINLINE(0x303C, 0x0142, 0xA832);

EXTERN_API_C( long )
GXGetColorSet                   (gxColorSet             source,
                                 gxColorSpace *         space,
                                 gxSetColor             colors[])                             THREEWORDINLINE(0x303C, 0x0143, 0xA832);

EXTERN_API_C( long )
GXGetColorProfile               (gxColorProfile         source,
                                 void *                 colorProfileData)                   THREEWORDINLINE(0x303C, 0x0144, 0xA832);

EXTERN_API_C( void )
GXSetColorSet                   (gxColorSet             target,
                                 gxColorSpace           space,
                                 long                   count,
                                 const gxSetColor       colors[])                             THREEWORDINLINE(0x303C, 0x0145, 0xA832);

EXTERN_API_C( void )
GXSetColorProfile               (gxColorProfile         target,
                                 long                   size,
                                 void *                 colorProfileData)                   THREEWORDINLINE(0x303C, 0x0146, 0xA832);

EXTERN_API_C( gxShape )
GXGetViewDeviceBitmap           (gxViewDevice           source)                             THREEWORDINLINE(0x303C, 0x0147, 0xA832);

EXTERN_API_C( gxShape )
GXGetViewDeviceClip             (gxViewDevice           source)                             THREEWORDINLINE(0x303C, 0x0148, 0xA832);

EXTERN_API_C( gxMapping *)
GXGetViewDeviceMapping          (gxViewDevice           source,
                                 gxMapping *            map)                                THREEWORDINLINE(0x303C, 0x0149, 0xA832);

EXTERN_API_C( gxViewGroup )
GXGetViewDeviceViewGroup        (gxViewDevice           source)                             THREEWORDINLINE(0x303C, 0x014A, 0xA832);

EXTERN_API_C( void )
GXSetViewDeviceBitmap           (gxViewDevice           target,
                                 gxShape                bitmapShape)                        THREEWORDINLINE(0x303C, 0x014B, 0xA832);

EXTERN_API_C( void )
GXSetViewDeviceClip             (gxViewDevice           target,
                                 gxShape                clip)                               THREEWORDINLINE(0x303C, 0x014C, 0xA832);

EXTERN_API_C( void )
GXSetViewDeviceMapping          (gxViewDevice           target,
                                 const gxMapping *      map)                                THREEWORDINLINE(0x303C, 0x014D, 0xA832);

EXTERN_API_C( void )
GXSetViewDeviceViewGroup        (gxViewDevice           target,
                                 gxViewGroup            group)                              THREEWORDINLINE(0x303C, 0x014E, 0xA832);

EXTERN_API_C( long )
GXGetViewPortChildren           (gxViewPort             source,
                                 gxViewPort             list[])                               THREEWORDINLINE(0x303C, 0x014F, 0xA832);

EXTERN_API_C( gxShape )
GXGetViewPortClip               (gxViewPort             source)                             THREEWORDINLINE(0x303C, 0x0150, 0xA832);

EXTERN_API_C( long )
GXGetViewPortDither             (gxViewPort             source)                             THREEWORDINLINE(0x303C, 0x0151, 0xA832);

EXTERN_API_C( Boolean )
GXGetViewPortHalftone           (gxViewPort             source,
                                 gxHalftone *           data)                               THREEWORDINLINE(0x303C, 0x0152, 0xA832);

EXTERN_API_C( gxMapping *)
GXGetViewPortMapping            (gxViewPort             source,
                                 gxMapping *            map)                                THREEWORDINLINE(0x303C, 0x0153, 0xA832);

EXTERN_API_C( gxViewPort )
GXGetViewPortParent             (gxViewPort             source)                             THREEWORDINLINE(0x303C, 0x0154, 0xA832);

EXTERN_API_C( gxViewGroup )
GXGetViewPortViewGroup          (gxViewPort             source)                             THREEWORDINLINE(0x303C, 0x0155, 0xA832);

EXTERN_API_C( long )
GXGetViewPortHalftoneMatrix     (gxViewPort             source,
                                 gxViewDevice           sourceDevice,
                                 gxHalftoneMatrix *     theMatrix)                          THREEWORDINLINE(0x303C, 0x0273, 0xA832);

EXTERN_API_C( void )
GXSetViewPortChildren           (gxViewPort             target,
                                 long                   count,
                                 const gxViewPort       list[])                               THREEWORDINLINE(0x303C, 0x0156, 0xA832);

EXTERN_API_C( void )
GXSetViewPortClip               (gxViewPort             target,
                                 gxShape                clip)                               THREEWORDINLINE(0x303C, 0x0157, 0xA832);

EXTERN_API_C( void )
GXSetViewPortDither             (gxViewPort             target,
                                 long                   level)                              THREEWORDINLINE(0x303C, 0x0158, 0xA832);

EXTERN_API_C( void )
GXSetViewPortHalftone           (gxViewPort             target,
                                 const gxHalftone *     data)                               THREEWORDINLINE(0x303C, 0x0159, 0xA832);

EXTERN_API_C( void )
GXSetViewPortMapping            (gxViewPort             target,
                                 const gxMapping *      map)                                THREEWORDINLINE(0x303C, 0x015A, 0xA832);

EXTERN_API_C( void )
GXSetViewPortParent             (gxViewPort             target,
                                 gxViewPort             parent)                             THREEWORDINLINE(0x303C, 0x015B, 0xA832);

EXTERN_API_C( void )
GXSetViewPortViewGroup          (gxViewPort             target,
                                 gxViewGroup            group)                              THREEWORDINLINE(0x303C, 0x015C, 0xA832);

EXTERN_API_C( long )
GXGetColorProfileTags           (gxColorProfile         source,
                                 long                   tagType,
                                 long                   index,
                                 long                   count,
                                 gxTag                  items[])                              THREEWORDINLINE(0x303C, 0x015D, 0xA832);

EXTERN_API_C( long )
GXGetColorSetTags               (gxColorSet             source,
                                 long                   tagType,
                                 long                   index,
                                 long                   count,
                                 gxTag                  items[])                              THREEWORDINLINE(0x303C, 0x015E, 0xA832);

EXTERN_API_C( long )
GXGetInkTags                    (gxInk                  source,
                                 long                   tagType,
                                 long                   index,
                                 long                   count,
                                 gxTag                  items[])                              THREEWORDINLINE(0x303C, 0x015F, 0xA832);

EXTERN_API_C( long )
GXGetShapeTags                  (gxShape                source,
                                 long                   tagType,
                                 long                   index,
                                 long                   count,
                                 gxTag                  items[])                              THREEWORDINLINE(0x303C, 0x0160, 0xA832);

EXTERN_API_C( long )
GXGetStyleTags                  (gxStyle                source,
                                 long                   tagType,
                                 long                   index,
                                 long                   count,
                                 gxTag                  items[])                              THREEWORDINLINE(0x303C, 0x0161, 0xA832);

EXTERN_API_C( long )
GXGetTransformTags              (gxTransform            source,
                                 long                   tagType,
                                 long                   index,
                                 long                   count,
                                 gxTag                  items[])                              THREEWORDINLINE(0x303C, 0x0162, 0xA832);

EXTERN_API_C( long )
GXGetViewDeviceTags             (gxViewDevice           source,
                                 long                   tagType,
                                 long                   index,
                                 long                   count,
                                 gxTag                  items[])                              THREEWORDINLINE(0x303C, 0x0163, 0xA832);

EXTERN_API_C( long )
GXGetViewPortTags               (gxViewPort             source,
                                 long                   tagType,
                                 long                   index,
                                 long                   count,
                                 gxTag                  items[])                              THREEWORDINLINE(0x303C, 0x0164, 0xA832);

EXTERN_API_C( void )
GXSetColorProfileTags           (gxColorProfile         target,
                                 long                   tagType,
                                 long                   index,
                                 long                   oldCount,
                                 long                   newCount,
                                 const gxTag            items[])                              THREEWORDINLINE(0x303C, 0x0165, 0xA832);

EXTERN_API_C( void )
GXSetColorSetTags               (gxColorSet             target,
                                 long                   tagType,
                                 long                   index,
                                 long                   oldCount,
                                 long                   newCount,
                                 const gxTag            items[])                              THREEWORDINLINE(0x303C, 0x0166, 0xA832);

EXTERN_API_C( void )
GXSetInkTags                    (gxInk                  target,
                                 long                   tagType,
                                 long                   index,
                                 long                   oldCount,
                                 long                   newCount,
                                 const gxTag            items[])                              THREEWORDINLINE(0x303C, 0x0167, 0xA832);

EXTERN_API_C( void )
GXSetShapeTags                  (gxShape                target,
                                 long                   tagType,
                                 long                   index,
                                 long                   oldCount,
                                 long                   newCount,
                                 const gxTag            items[])                              THREEWORDINLINE(0x303C, 0x0168, 0xA832);

EXTERN_API_C( void )
GXSetStyleTags                  (gxStyle                target,
                                 long                   tagType,
                                 long                   index,
                                 long                   oldCount,
                                 long                   newCount,
                                 const gxTag            items[])                              THREEWORDINLINE(0x303C, 0x0169, 0xA832);

EXTERN_API_C( void )
GXSetTransformTags              (gxTransform            target,
                                 long                   tagType,
                                 long                   index,
                                 long                   oldCount,
                                 long                   newCount,
                                 const gxTag            items[])                              THREEWORDINLINE(0x303C, 0x016A, 0xA832);

EXTERN_API_C( void )
GXSetViewDeviceTags             (gxViewDevice           target,
                                 long                   tagType,
                                 long                   index,
                                 long                   oldCount,
                                 long                   newCount,
                                 const gxTag            items[])                              THREEWORDINLINE(0x303C, 0x016B, 0xA832);

EXTERN_API_C( void )
GXSetViewPortTags               (gxViewPort             target,
                                 long                   tagType,
                                 long                   index,
                                 long                   oldCount,
                                 long                   newCount,
                                 const gxTag            items[])                              THREEWORDINLINE(0x303C, 0x016C, 0xA832);

EXTERN_API_C( gxInkAttribute )
GXGetInkAttributes              (gxInk                  source)                             THREEWORDINLINE(0x303C, 0x016D, 0xA832);

EXTERN_API_C( gxShapeAttribute )
GXGetShapeAttributes            (gxShape                source)                             THREEWORDINLINE(0x303C, 0x016E, 0xA832);

EXTERN_API_C( gxInkAttribute )
GXGetShapeInkAttributes         (gxShape                source)                             THREEWORDINLINE(0x303C, 0x016F, 0xA832);

EXTERN_API_C( gxStyleAttribute )
GXGetShapeStyleAttributes       (gxShape                source)                             THREEWORDINLINE(0x303C, 0x0170, 0xA832);

EXTERN_API_C( gxStyleAttribute )
GXGetStyleAttributes            (gxStyle                source)                             THREEWORDINLINE(0x303C, 0x0171, 0xA832);

EXTERN_API_C( gxTextAttribute )
GXGetShapeTextAttributes        (gxShape                source)                             THREEWORDINLINE(0x303C, 0x0172, 0xA832);

EXTERN_API_C( gxTextAttribute )
GXGetStyleTextAttributes        (gxStyle                source)                             THREEWORDINLINE(0x303C, 0x0173, 0xA832);

EXTERN_API_C( gxDeviceAttribute )
GXGetViewDeviceAttributes       (gxViewDevice           source)                             THREEWORDINLINE(0x303C, 0x0174, 0xA832);

EXTERN_API_C( gxPortAttribute )
GXGetViewPortAttributes         (gxViewPort             source)                             THREEWORDINLINE(0x303C, 0x0175, 0xA832);

EXTERN_API_C( void )
GXSetInkAttributes              (gxInk                  target,
                                 gxInkAttribute         attributes)                         THREEWORDINLINE(0x303C, 0x0176, 0xA832);

EXTERN_API_C( void )
GXSetShapeAttributes            (gxShape                target,
                                 gxShapeAttribute       attributes)                         THREEWORDINLINE(0x303C, 0x0177, 0xA832);

EXTERN_API_C( void )
GXSetShapeInkAttributes         (gxShape                target,
                                 gxInkAttribute         attributes)                         THREEWORDINLINE(0x303C, 0x0178, 0xA832);

EXTERN_API_C( void )
GXSetShapeStyleAttributes       (gxShape                target,
                                 gxStyleAttribute       attributes)                         THREEWORDINLINE(0x303C, 0x0179, 0xA832);

EXTERN_API_C( void )
GXSetStyleAttributes            (gxStyle                target,
                                 gxStyleAttribute       attributes)                         THREEWORDINLINE(0x303C, 0x017A, 0xA832);

EXTERN_API_C( void )
GXSetShapeTextAttributes        (gxShape                target,
                                 gxTextAttribute        attributes)                         THREEWORDINLINE(0x303C, 0x017B, 0xA832);

EXTERN_API_C( void )
GXSetStyleTextAttributes        (gxStyle                target,
                                 gxTextAttribute        attributes)                         THREEWORDINLINE(0x303C, 0x017C, 0xA832);

EXTERN_API_C( void )
GXSetViewDeviceAttributes       (gxViewDevice           target,
                                 gxDeviceAttribute      attributes)                         THREEWORDINLINE(0x303C, 0x017D, 0xA832);

EXTERN_API_C( void )
GXSetViewPortAttributes         (gxViewPort             target,
                                 gxPortAttribute        attributes)                         THREEWORDINLINE(0x303C, 0x017E, 0xA832);

EXTERN_API_C( long )
GXGetColorProfileOwners         (gxColorProfile         source)                             THREEWORDINLINE(0x303C, 0x017F, 0xA832);

EXTERN_API_C( long )
GXGetColorSetOwners             (gxColorSet             source)                             THREEWORDINLINE(0x303C, 0x0180, 0xA832);

EXTERN_API_C( long )
GXGetInkOwners                  (gxInk                  source)                             THREEWORDINLINE(0x303C, 0x0181, 0xA832);

EXTERN_API_C( long )
GXGetShapeOwners                (gxShape                source)                             THREEWORDINLINE(0x303C, 0x0182, 0xA832);

EXTERN_API_C( long )
GXGetStyleOwners                (gxStyle                source)                             THREEWORDINLINE(0x303C, 0x0183, 0xA832);

EXTERN_API_C( long )
GXGetTagOwners                  (gxTag                  source)                             THREEWORDINLINE(0x303C, 0x0184, 0xA832);

EXTERN_API_C( long )
GXGetTransformOwners            (gxTransform            source)                             THREEWORDINLINE(0x303C, 0x0185, 0xA832);

EXTERN_API_C( void )
GXLockShape                     (gxShape                target)                             THREEWORDINLINE(0x303C, 0x0186, 0xA832);

EXTERN_API_C( void )
GXLockTag                       (gxTag                  target)                             THREEWORDINLINE(0x303C, 0x0187, 0xA832);

EXTERN_API_C( void )
GXUnlockShape                   (gxShape                target)                             THREEWORDINLINE(0x303C, 0x0188, 0xA832);

EXTERN_API_C( void )
GXUnlockTag                     (gxTag                  target)                             THREEWORDINLINE(0x303C, 0x0189, 0xA832);

EXTERN_API_C( void *)
GXGetShapeStructure             (gxShape                source,
                                 long *                 length)                             THREEWORDINLINE(0x303C, 0x018A, 0xA832);

EXTERN_API_C( void *)
GXGetTagStructure               (gxTag                  source,
                                 long *                 length)                             THREEWORDINLINE(0x303C, 0x018B, 0xA832);

EXTERN_API_C( Fixed )
GXGetColorDistance              (const gxColor *        target,
                                 const gxColor *        source)                             THREEWORDINLINE(0x303C, 0x018C, 0xA832);

EXTERN_API_C( gxPoint *)
GXShapeLengthToPoint            (gxShape                target,
                                 long                   index,
                                 Fixed                  length,
                                 gxPoint *              location,
                                 gxPoint *              tangent)                            THREEWORDINLINE(0x303C, 0x018D, 0xA832);

EXTERN_API_C( wide *)
GXGetShapeArea                  (gxShape                source,
                                 long                   index,
                                 wide *                 area)                               THREEWORDINLINE(0x303C, 0x018E, 0xA832);

EXTERN_API_C( long )
GXGetShapeCacheSize             (gxShape                source)                             THREEWORDINLINE(0x303C, 0x018F, 0xA832);

EXTERN_API_C( gxPoint *)
GXGetShapeCenter                (gxShape                source,
                                 long                   index,
                                 gxPoint *              center)                             THREEWORDINLINE(0x303C, 0x0190, 0xA832);

EXTERN_API_C( gxContourDirection )
GXGetShapeDirection             (gxShape                source,
                                 long                   contour)                            THREEWORDINLINE(0x303C, 0x0191, 0xA832);

EXTERN_API_C( long )
GXGetShapeIndex                 (gxShape                source,
                                 long                   contour,
                                 long                   vector)                             THREEWORDINLINE(0x303C, 0x0192, 0xA832);

EXTERN_API_C( wide *)
GXGetShapeLength                (gxShape                source,
                                 long                   index,
                                 wide *                 length)                             THREEWORDINLINE(0x303C, 0x0193, 0xA832);

EXTERN_API_C( long )
GXGetShapeSize                  (gxShape                source)                             THREEWORDINLINE(0x303C, 0x0194, 0xA832);

EXTERN_API_C( long )
GXCountShapeContours            (gxShape                source)                             THREEWORDINLINE(0x303C, 0x0195, 0xA832);

EXTERN_API_C( long )
GXCountShapePoints              (gxShape                source,
                                 long                   contour)                            THREEWORDINLINE(0x303C, 0x0196, 0xA832);

/* returns the number of positions */
EXTERN_API_C( long )
GXGetShapeDashPositions         (gxShape                source,
                                 gxMapping              dashMappings[])                       THREEWORDINLINE(0x303C, 0x0197, 0xA832);

EXTERN_API_C( long )
GXGetShapeDeviceArea            (gxShape                source,
                                 gxViewPort             port,
                                 gxViewDevice           device)                             THREEWORDINLINE(0x303C, 0x0198, 0xA832);

EXTERN_API_C( Boolean )
GXGetShapeDeviceBounds          (gxShape                source,
                                 gxViewPort             port,
                                 gxViewDevice           device,
                                 gxRectangle *          bounds)                             THREEWORDINLINE(0x303C, 0x0199, 0xA832);

EXTERN_API_C( gxColorSet )
GXGetShapeDeviceColors          (gxShape                source,
                                 gxViewPort             port,
                                 gxViewDevice           device,
                                 long *                 width)                              THREEWORDINLINE(0x303C, 0x019A, 0xA832);

EXTERN_API_C( Boolean )
GXGetShapeGlobalBounds          (gxShape                source,
                                 gxViewPort             port,
                                 gxViewGroup            group,
                                 gxRectangle *          bounds)                             THREEWORDINLINE(0x303C, 0x019B, 0xA832);

EXTERN_API_C( long )
GXGetShapeGlobalViewDevices     (gxShape                source,
                                 gxViewPort             port,
                                 gxViewDevice           list[])                               THREEWORDINLINE(0x303C, 0x019C, 0xA832);

EXTERN_API_C( long )
GXGetShapeGlobalViewPorts       (gxShape                source,
                                 gxViewPort             list[])                               THREEWORDINLINE(0x303C, 0x019D, 0xA832);

EXTERN_API_C( gxRectangle *)
GXGetShapeLocalBounds           (gxShape                source,
                                 gxRectangle *          bounds)                             THREEWORDINLINE(0x303C, 0x019E, 0xA832);

/* returns the number of positions */
EXTERN_API_C( long )
GXGetShapePatternPositions      (gxShape                source,
                                 gxPoint                positions[])                          THREEWORDINLINE(0x303C, 0x019F, 0xA832);

EXTERN_API_C( void )
GXGetShapeLocalFontMetrics      (gxShape                sourceShape,
                                 gxPoint *              before,
                                 gxPoint *              after,
                                 gxPoint *              caretAngle,
                                 gxPoint *              caretOffset)                        THREEWORDINLINE(0x303C, 0x01A0, 0xA832);

EXTERN_API_C( void )
GXGetShapeDeviceFontMetrics     (gxShape                sourceShape,
                                 gxViewPort             port,
                                 gxViewDevice           device,
                                 gxPoint *              before,
                                 gxPoint *              after,
                                 gxPoint *              caretAngle,
                                 gxPoint *              caretOffset)                        THREEWORDINLINE(0x303C, 0x01A1, 0xA832);

EXTERN_API_C( long )
GXGetViewGroupViewDevices       (gxViewGroup            source,
                                 gxViewDevice           list[])                               THREEWORDINLINE(0x303C, 0x01A2, 0xA832);

EXTERN_API_C( long )
GXGetViewGroupViewPorts         (gxViewGroup            source,
                                 gxViewPort             list[])                               THREEWORDINLINE(0x303C, 0x01A3, 0xA832);

EXTERN_API_C( gxMapping *)
GXGetViewPortGlobalMapping      (gxViewPort             source,
                                 gxMapping *            map)                                THREEWORDINLINE(0x303C, 0x01A4, 0xA832);

EXTERN_API_C( long )
GXGetViewPortViewDevices        (gxViewPort             source,
                                 gxViewDevice           list[])                               THREEWORDINLINE(0x303C, 0x01A5, 0xA832);

EXTERN_API_C( gxShape )
GXHitTestPicture                (gxShape                target,
                                 const gxPoint *        test,
                                 gxHitTestInfo *        result,
                                 long                   level,
                                 long                   depth)                              THREEWORDINLINE(0x303C, 0x01A6, 0xA832);

EXTERN_API_C( Boolean )
GXIntersectRectangle            (gxRectangle *          target,
                                 const gxRectangle *    source,
                                 const gxRectangle *    operand)                            THREEWORDINLINE(0x303C, 0x01A7, 0xA832);

EXTERN_API_C( gxRectangle *)
GXUnionRectangle                (gxRectangle *          target,
                                 const gxRectangle *    source,
                                 const gxRectangle *    operand)                            THREEWORDINLINE(0x303C, 0x01A8, 0xA832);

EXTERN_API_C( Boolean )
GXTouchesRectanglePoint         (const gxRectangle *    target,
                                 const gxPoint *        test)                               THREEWORDINLINE(0x303C, 0x01A9, 0xA832);

EXTERN_API_C( Boolean )
GXTouchesShape                  (gxShape                target,
                                 gxShape                test)                               THREEWORDINLINE(0x303C, 0x01AA, 0xA832);

EXTERN_API_C( Boolean )
GXTouchesBoundsShape            (const gxRectangle *    target,
                                 gxShape                test)                               THREEWORDINLINE(0x303C, 0x01AB, 0xA832);

EXTERN_API_C( Boolean )
GXContainsRectangle             (const gxRectangle *    container,
                                 const gxRectangle *    test)                               THREEWORDINLINE(0x303C, 0x01AC, 0xA832);

EXTERN_API_C( Boolean )
GXContainsShape                 (gxShape                container,
                                 gxShape                test)                               THREEWORDINLINE(0x303C, 0x01AD, 0xA832);

EXTERN_API_C( Boolean )
GXContainsBoundsShape           (const gxRectangle *    container,
                                 gxShape                test,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x01AE, 0xA832);

EXTERN_API_C( gxColor *)
GXConvertColor                  (gxColor *              target,
                                 gxColorSpace           space,
                                 gxColorSet             aSet,
                                 gxColorProfile         profile)                            THREEWORDINLINE(0x303C, 0x01AF, 0xA832);

EXTERN_API_C( gxColor *)
GXCombineColor                  (gxColor *              target,
                                 gxInk                  operand)                            THREEWORDINLINE(0x303C, 0x01B0, 0xA832);

EXTERN_API_C( Boolean )
GXCheckColor                    (const gxColor *        source,
                                 gxColorSpace           space,
                                 gxColorSet             aSet,
                                 gxColorProfile         profile)                            THREEWORDINLINE(0x303C, 0x01B1, 0xA832);

EXTERN_API_C( gxShape )
GXCheckBitmapColor              (gxShape                source,
                                 const gxLongRectangle * area,
                                 gxColorSpace           space,
                                 gxColorSet             aSet,
                                 gxColorProfile         profile)                            THREEWORDINLINE(0x303C, 0x01B2, 0xA832);

EXTERN_API_C( Fixed )
GXGetHalftoneDeviceAngle        (gxViewDevice           source,
                                 const gxHalftone *     data)                               THREEWORDINLINE(0x303C, 0x01B3, 0xA832);

EXTERN_API_C( long )
GXGetColorSetParts              (gxColorSet             source,
                                 long                   index,
                                 long                   count,
                                 gxColorSpace *         space,
                                 gxSetColor             data[])                               THREEWORDINLINE(0x303C, 0x01B4, 0xA832);

/* returns the glyph count */
EXTERN_API_C( long )
GXGetGlyphParts                 (gxShape                source,
                                 long                   index,
                                 long                   charCount,
                                 long *                 byteLength,
                                 unsigned char          text[],
                                 gxPoint                positions[],
                                 long                   advanceBits[],
                                 gxPoint                tangents[],
                                 long *                 runCount,
                                 short                  styleRuns[],
                                 gxStyle                styles[])                             THREEWORDINLINE(0x303C, 0x01B5, 0xA832);

EXTERN_API_C( long )
GXGetPathParts                  (gxShape                source,
                                 long                   index,
                                 long                   count,
                                 gxPaths *              data)                               THREEWORDINLINE(0x303C, 0x01B6, 0xA832);

EXTERN_API_C( long )
GXGetPictureParts               (gxShape                source,
                                 long                   index,
                                 long                   count,
                                 gxShape                shapes[],
                                 gxStyle                styles[],
                                 gxInk                  inks[],
                                 gxTransform            transforms[])                         THREEWORDINLINE(0x303C, 0x01B7, 0xA832);

EXTERN_API_C( long )
GXGetPolygonParts               (gxShape                source,
                                 long                   index,
                                 long                   count,
                                 gxPolygons *           data)                               THREEWORDINLINE(0x303C, 0x01B8, 0xA832);

EXTERN_API_C( gxShape )
GXGetShapeParts                 (gxShape                source,
                                 long                   index,
                                 long                   count,
                                 gxShape                destination)                        THREEWORDINLINE(0x303C, 0x01B9, 0xA832);

EXTERN_API_C( long )
GXGetTextParts                  (gxShape                source,
                                 long                   index,
                                 long                   charCount,
                                 unsigned char          text[])                               THREEWORDINLINE(0x303C, 0x01BA, 0xA832);

EXTERN_API_C( void )
GXSetColorSetParts              (gxColorSet             target,
                                 long                   index,
                                 long                   oldCount,
                                 long                   newCount,
                                 const gxSetColor       data[])                               THREEWORDINLINE(0x303C, 0x01BB, 0xA832);

EXTERN_API_C( void )
GXSetGlyphParts                 (gxShape                source,
                                 long                   index,
                                 long                   oldCharCount,
                                 long                   newCharCount,
                                 const unsigned char    text[],
                                 const gxPoint          positions[],
                                 const long             advanceBits[],
                                 const gxPoint          tangents[],
                                 const short            styleRuns[],
                                 const gxStyle          styles[])                             THREEWORDINLINE(0x303C, 0x01BC, 0xA832);

EXTERN_API_C( void )
GXSetPathParts                  (gxShape                target,
                                 long                   index,
                                 long                   count,
                                 const gxPaths *        data,
                                 gxEditShapeFlag        flags)                              THREEWORDINLINE(0x303C, 0x01BD, 0xA832);

EXTERN_API_C( void )
GXSetPictureParts               (gxShape                target,
                                 long                   index,
                                 long                   oldCount,
                                 long                   newCount,
                                 const gxShape          shapes[],
                                 const gxStyle          styles[],
                                 const gxInk            inks[],
                                 const gxTransform      transforms[])                         THREEWORDINLINE(0x303C, 0x01BE, 0xA832);

EXTERN_API_C( void )
GXSetPolygonParts               (gxShape                target,
                                 long                   index,
                                 long                   count,
                                 const gxPolygons *     data,
                                 gxEditShapeFlag        flags)                              THREEWORDINLINE(0x303C, 0x01BF, 0xA832);

EXTERN_API_C( void )
GXSetShapeParts                 (gxShape                target,
                                 long                   index,
                                 long                   count,
                                 gxShape                insert,
                                 gxEditShapeFlag        flags)                              THREEWORDINLINE(0x303C, 0x01C0, 0xA832);

EXTERN_API_C( void )
GXSetTextParts                  (gxShape                target,
                                 long                   index,
                                 long                   oldCharCount,
                                 long                   newCharCount,
                                 const unsigned char    text[])                               THREEWORDINLINE(0x303C, 0x01C1, 0xA832);

EXTERN_API_C( long )
GXGetShapePoints                (gxShape                source,
                                 long                   index,
                                 long                   count,
                                 gxPoint                data[])                               THREEWORDINLINE(0x303C, 0x01C2, 0xA832);

EXTERN_API_C( void )
GXSetShapePoints                (gxShape                target,
                                 long                   index,
                                 long                   count,
                                 const gxPoint          data[])                               THREEWORDINLINE(0x303C, 0x01C3, 0xA832);

EXTERN_API_C( long )
GXGetGlyphPositions             (gxShape                source,
                                 long                   index,
                                 long                   charCount,
                                 long                   advance[],
                                 gxPoint                positions[])                          THREEWORDINLINE(0x303C, 0x01C4, 0xA832);

EXTERN_API_C( long )
GXGetGlyphTangents              (gxShape                source,
                                 long                   index,
                                 long                   charCount,
                                 gxPoint                tangents[])                           THREEWORDINLINE(0x303C, 0x01C5, 0xA832);

EXTERN_API_C( void )
GXSetGlyphPositions             (gxShape                target,
                                 long                   index,
                                 long                   charCount,
                                 const long             advance[],
                                 const gxPoint          positions[])                          THREEWORDINLINE(0x303C, 0x01C6, 0xA832);

EXTERN_API_C( void )
GXSetGlyphTangents              (gxShape                target,
                                 long                   index,
                                 long                   charCount,
                                 const gxPoint          tangents[])                           THREEWORDINLINE(0x303C, 0x01C7, 0xA832);

EXTERN_API_C( long )
GXGetGlyphMetrics               (gxShape                source,
                                 gxPoint                glyphOrigins[],
                                 gxRectangle            boundingBoxes[],
                                 gxPoint                sideBearings[])                       THREEWORDINLINE(0x303C, 0x01C8, 0xA832);

EXTERN_API_C( void )
GXDifferenceShape               (gxShape                target,
                                 gxShape                operand)                            THREEWORDINLINE(0x303C, 0x01C9, 0xA832);

EXTERN_API_C( void )
GXExcludeShape                  (gxShape                target,
                                 gxShape                operand)                            THREEWORDINLINE(0x303C, 0x01CA, 0xA832);

EXTERN_API_C( void )
GXIntersectShape                (gxShape                target,
                                 gxShape                operand)                            THREEWORDINLINE(0x303C, 0x01CB, 0xA832);

EXTERN_API_C( void )
GXMapShape                      (gxShape                target,
                                 const gxMapping *      map)                                THREEWORDINLINE(0x303C, 0x01CC, 0xA832);

EXTERN_API_C( void )
GXMoveShape                     (gxShape                target,
                                 Fixed                  deltaX,
                                 Fixed                  deltaY)                             THREEWORDINLINE(0x303C, 0x01CD, 0xA832);

EXTERN_API_C( void )
GXMoveShapeTo                   (gxShape                target,
                                 Fixed                  x,
                                 Fixed                  y)                                  THREEWORDINLINE(0x303C, 0x01CE, 0xA832);

EXTERN_API_C( void )
GXReverseDifferenceShape        (gxShape                target,
                                 gxShape                operand)                            THREEWORDINLINE(0x303C, 0x01CF, 0xA832);

EXTERN_API_C( void )
GXRotateShape                   (gxShape                target,
                                 Fixed                  degrees,
                                 Fixed                  xOffset,
                                 Fixed                  yOffset)                            THREEWORDINLINE(0x303C, 0x01D0, 0xA832);

EXTERN_API_C( void )
GXScaleShape                    (gxShape                target,
                                 Fixed                  hScale,
                                 Fixed                  vScale,
                                 Fixed                  xOffset,
                                 Fixed                  yOffset)                            THREEWORDINLINE(0x303C, 0x01D1, 0xA832);

EXTERN_API_C( void )
GXSkewShape                     (gxShape                target,
                                 Fixed                  xSkew,
                                 Fixed                  ySkew,
                                 Fixed                  xOffset,
                                 Fixed                  yOffset)                            THREEWORDINLINE(0x303C, 0x01D2, 0xA832);

EXTERN_API_C( void )
GXUnionShape                    (gxShape                target,
                                 gxShape                operand)                            THREEWORDINLINE(0x303C, 0x01D3, 0xA832);

EXTERN_API_C( void )
GXDifferenceTransform           (gxTransform            target,
                                 gxShape                operand)                            THREEWORDINLINE(0x303C, 0x01D4, 0xA832);

EXTERN_API_C( void )
GXExcludeTransform              (gxTransform            target,
                                 gxShape                operand)                            THREEWORDINLINE(0x303C, 0x01D5, 0xA832);

EXTERN_API_C( void )
GXIntersectTransform            (gxTransform            target,
                                 gxShape                operand)                            THREEWORDINLINE(0x303C, 0x01D6, 0xA832);

EXTERN_API_C( void )
GXMapTransform                  (gxTransform            target,
                                 const gxMapping *      map)                                THREEWORDINLINE(0x303C, 0x01D7, 0xA832);

EXTERN_API_C( void )
GXMoveTransform                 (gxTransform            target,
                                 Fixed                  deltaX,
                                 Fixed                  deltaY)                             THREEWORDINLINE(0x303C, 0x01D8, 0xA832);

EXTERN_API_C( void )
GXMoveTransformTo               (gxTransform            target,
                                 Fixed                  x,
                                 Fixed                  y)                                  THREEWORDINLINE(0x303C, 0x01D9, 0xA832);

EXTERN_API_C( void )
GXReverseDifferenceTransform    (gxTransform            target,
                                 gxShape                operand)                            THREEWORDINLINE(0x303C, 0x01DA, 0xA832);

EXTERN_API_C( void )
GXRotateTransform               (gxTransform            target,
                                 Fixed                  degrees,
                                 Fixed                  xOffset,
                                 Fixed                  yOffset)                            THREEWORDINLINE(0x303C, 0x01DB, 0xA832);

EXTERN_API_C( void )
GXScaleTransform                (gxTransform            target,
                                 Fixed                  hScale,
                                 Fixed                  vScale,
                                 Fixed                  xOffset,
                                 Fixed                  yOffset)                            THREEWORDINLINE(0x303C, 0x01DC, 0xA832);

EXTERN_API_C( void )
GXSkewTransform                 (gxTransform            target,
                                 Fixed                  xSkew,
                                 Fixed                  ySkew,
                                 Fixed                  xOffset,
                                 Fixed                  yOffset)                            THREEWORDINLINE(0x303C, 0x01DD, 0xA832);

EXTERN_API_C( void )
GXUnionTransform                (gxTransform            target,
                                 gxShape                operand)                            THREEWORDINLINE(0x303C, 0x01DE, 0xA832);

EXTERN_API_C( void )
GXBreakShape                    (gxShape                target,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x01DF, 0xA832);

EXTERN_API_C( void )
GXChangedShape                  (gxShape                target)                             THREEWORDINLINE(0x303C, 0x01E0, 0xA832);

EXTERN_API_C( gxShapePart )
GXHitTestShape                  (gxShape                target,
                                 const gxPoint *        test,
                                 gxHitTestInfo *        result)                             THREEWORDINLINE(0x303C, 0x01E1, 0xA832);

EXTERN_API_C( gxShape )
GXHitTestDevice                 (gxShape                target,
                                 gxViewPort             port,
                                 gxViewDevice           device,
                                 const gxPoint *        test,
                                 const gxPoint *        tolerance)                          THREEWORDINLINE(0x303C, 0x01E2, 0xA832);

EXTERN_API_C( void )
GXInsetShape                    (gxShape                target,
                                 Fixed                  inset)                              THREEWORDINLINE(0x303C, 0x01E3, 0xA832);

EXTERN_API_C( void )
GXInvertShape                   (gxShape                target)                             THREEWORDINLINE(0x303C, 0x01E4, 0xA832);

EXTERN_API_C( void )
GXPrimitiveShape                (gxShape                target)                             THREEWORDINLINE(0x303C, 0x01E5, 0xA832);

EXTERN_API_C( void )
GXReduceShape                   (gxShape                target,
                                 long                   contour)                            THREEWORDINLINE(0x303C, 0x01E6, 0xA832);

EXTERN_API_C( void )
GXReverseShape                  (gxShape                target,
                                 long                   contour)                            THREEWORDINLINE(0x303C, 0x01E7, 0xA832);

EXTERN_API_C( void )
GXSimplifyShape                 (gxShape                target)                             THREEWORDINLINE(0x303C, 0x01E8, 0xA832);

EXTERN_API_C( void )
GXLockColorProfile              (gxColorProfile         source)                             THREEWORDINLINE(0x303C, 0x01E9, 0xA832);

EXTERN_API_C( void )
GXUnlockColorProfile            (gxColorProfile         source)                             THREEWORDINLINE(0x303C, 0x01EA, 0xA832);

EXTERN_API_C( void *)
GXGetColorProfileStructure      (gxColorProfile         source,
                                 long *                 length)                             THREEWORDINLINE(0x303C, 0x01EB, 0xA832);

EXTERN_API_C( void )
GXFlattenShape                  (gxShape                source,
                                 gxFlattenFlag          flags,
                                 gxSpoolBlock *         block)                              THREEWORDINLINE(0x303C, 0x01EC, 0xA832);

EXTERN_API_C( gxShape )
GXUnflattenShape                (gxSpoolBlock *         block,
                                 long                   count,
                                 const gxViewPort       portList[])                           THREEWORDINLINE(0x303C, 0x01ED, 0xA832);

EXTERN_API_C( void )
GXPostGraphicsNotice            (gxGraphicsNotice       notice)                             THREEWORDINLINE(0x303C, 0x0065, 0xA832);

EXTERN_API_C( void )
GXIgnoreGraphicsNotice          (gxGraphicsNotice       notice)                             THREEWORDINLINE(0x303C, 0x006D, 0xA832);

EXTERN_API_C( void )
GXPopGraphicsNotice             (void)                                                      THREEWORDINLINE(0x303C, 0x006E, 0xA832);



#endif  /* CALL_NOT_IN_CARBON */

 
#if defined(__MWERKS__) && TARGET_CPU_68K
    #pragma pop
#endif

#if PRAGMA_STRUCT_ALIGN
    #pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef PRAGMA_IMPORT_OFF
#pragma import off
#elif PRAGMA_IMPORT
#pragma import reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* __GXGRAPHICS__ */

