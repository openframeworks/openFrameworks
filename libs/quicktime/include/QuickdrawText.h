/*
     File:       QuickdrawText.h
 
     Contains:   Quickdraw Text Interfaces.
 
     Version:    Technology: Mac OS 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1983-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QUICKDRAWTEXT__
#define __QUICKDRAWTEXT__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
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

/* new CGrafPort bottleneck ("newProc2") function, used in Unicode Text drawing */
EXTERN_API_C( OSStatus )
StandardGlyphs                  (void *                 dataStream,
                                 ByteCount              size);


enum {
                                                                /* CharToPixel directions */
    leftCaret                   = 0,                            /*Place caret for left block*/
    rightCaret                  = -1,                           /*Place caret for right block*/
    kHilite                     = 1                             /*Direction is SysDirection*/
};

enum {
    smLeftCaret                 = 0,                            /*Place caret for left block - obsolete */
    smRightCaret                = -1,                           /*Place caret for right block - obsolete */
    smHilite                    = 1                             /*Direction is TESysJust - obsolete */
};

enum {
                                                                /*Constants for styleRunPosition argument in PortionLine, DrawJustified, MeasureJustified, CharToPixel, and PixelToChar.*/
    onlyStyleRun                = 0,                            /* This is the only style run on the line */
    leftStyleRun                = 1,                            /* This is leftmost of multiple style runs on the line */
    rightStyleRun               = 2,                            /* This is rightmost of multiple style runs on the line */
    middleStyleRun              = 3,                            /* There are multiple style runs on the line and this is neither the leftmost nor the rightmost. */
    smOnlyStyleRun              = 0,                            /* obsolete */
    smLeftStyleRun              = 1,                            /* obsolete */
    smRightStyleRun             = 2,                            /* obsolete */
    smMiddleStyleRun            = 3                             /* obsolete */
};

/* type for styleRunPosition parameter in PixelToChar etc. */
typedef short                           JustStyleCode;
/*QuickTime3.0*/
enum {
                                                                /* Constants for txFlags (which used to be the pad field after txFace) */
    tfAntiAlias                 = 1 << 0,
    tfUnicode                   = 1 << 1
};



struct FontInfo {
    short                           ascent;
    short                           descent;
    short                           widMax;
    short                           leading;
};
typedef struct FontInfo                 FontInfo;

typedef short                           FormatOrder[1];
typedef FormatOrder *                   FormatOrderPtr;
/* FormatStatus was moved to TextUtils.i */

struct OffPair {
    short                           offFirst;
    short                           offSecond;
};
typedef struct OffPair                  OffPair;

typedef OffPair                         OffsetTable[3];
typedef CALLBACK_API( Boolean , StyleRunDirectionProcPtr )(short styleRunIndex, void *dirParam);
typedef STACK_UPP_TYPE(StyleRunDirectionProcPtr)                StyleRunDirectionUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(StyleRunDirectionUPP)
    NewStyleRunDirectionUPP        (StyleRunDirectionProcPtr userRoutine);

    EXTERN_API(void)
    DisposeStyleRunDirectionUPP    (StyleRunDirectionUPP    userUPP);

    EXTERN_API(Boolean)
    InvokeStyleRunDirectionUPP     (short                   styleRunIndex,
                                    void *                  dirParam,
                                    StyleRunDirectionUPP    userUPP);

#else
    enum { uppStyleRunDirectionProcInfo = 0x00000390 };             /* pascal 1_byte Func(2_bytes, 4_bytes) */
    #define NewStyleRunDirectionUPP(userRoutine)                    (StyleRunDirectionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppStyleRunDirectionProcInfo, GetCurrentArchitecture())
    #define DisposeStyleRunDirectionUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define InvokeStyleRunDirectionUPP(styleRunIndex, dirParam, userUPP)  (Boolean)CALL_TWO_PARAMETER_UPP((userUPP), uppStyleRunDirectionProcInfo, (styleRunIndex), (dirParam))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewStyleRunDirectionProc(userRoutine)                   NewStyleRunDirectionUPP(userRoutine)
#define CallStyleRunDirectionProc(userRoutine, styleRunIndex, dirParam) InvokeStyleRunDirectionUPP(styleRunIndex, dirParam, userRoutine)
#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( short )
Pixel2Char                      (Ptr                    textBuf,
                                 short                  textLen,
                                 short                  slop,
                                 short                  pixelWidth,
                                 Boolean *              leadingEdge)                        FOURWORDINLINE(0x2F3C, 0x820E, 0x0014, 0xA8B5);

EXTERN_API( short )
Char2Pixel                      (Ptr                    textBuf,
                                 short                  textLen,
                                 short                  slop,
                                 short                  offset,
                                 short                  direction)                          FOURWORDINLINE(0x2F3C, 0x820C, 0x0016, 0xA8B5);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( short )
PixelToChar                     (Ptr                    textBuf,
                                 long                   textLength,
                                 Fixed                  slop,
                                 Fixed                  pixelWidth,
                                 Boolean *              leadingEdge,
                                 Fixed *                widthRemaining,
                                 JustStyleCode          styleRunPosition,
                                 Point                  numer,
                                 Point                  denom)                              FOURWORDINLINE(0x2F3C, 0x8222, 0x002E, 0xA8B5);

EXTERN_API( short )
CharToPixel                     (Ptr                    textBuf,
                                 long                   textLength,
                                 Fixed                  slop,
                                 long                   offset,
                                 short                  direction,
                                 JustStyleCode          styleRunPosition,
                                 Point                  numer,
                                 Point                  denom)                              FOURWORDINLINE(0x2F3C, 0x821C, 0x0030, 0xA8B5);

EXTERN_API( void )
DrawJustified                   (Ptr                    textPtr,
                                 long                   textLength,
                                 Fixed                  slop,
                                 JustStyleCode          styleRunPosition,
                                 Point                  numer,
                                 Point                  denom)                              FOURWORDINLINE(0x2F3C, 0x8016, 0x0032, 0xA8B5);

EXTERN_API( void )
MeasureJustified                (Ptr                    textPtr,
                                 long                   textLength,
                                 Fixed                  slop,
                                 Ptr                    charLocs,
                                 JustStyleCode          styleRunPosition,
                                 Point                  numer,
                                 Point                  denom)                              FOURWORDINLINE(0x2F3C, 0x801A, 0x0034, 0xA8B5);

EXTERN_API( Fixed )
PortionLine                     (Ptr                    textPtr,
                                 long                   textLen,
                                 JustStyleCode          styleRunPosition,
                                 Point                  numer,
                                 Point                  denom)                              FOURWORDINLINE(0x2F3C, 0x8412, 0x0036, 0xA8B5);

EXTERN_API( void )
HiliteText                      (Ptr                    textPtr,
                                 short                  textLength,
                                 short                  firstOffset,
                                 short                  secondOffset,
                                 OffsetTable            offsets)                            FOURWORDINLINE(0x2F3C, 0x800E, 0x001C, 0xA8B5);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
DrawJust                        (Ptr                    textPtr,
                                 short                  textLength,
                                 short                  slop)                               FOURWORDINLINE(0x2F3C, 0x8008, 0x001E, 0xA8B5);

EXTERN_API( void )
MeasureJust                     (Ptr                    textPtr,
                                 short                  textLength,
                                 short                  slop,
                                 Ptr                    charLocs)                           FOURWORDINLINE(0x2F3C, 0x800C, 0x0020, 0xA8B5);

EXTERN_API( Fixed )
PortionText                     (Ptr                    textPtr,
                                 long                   textLength)                         FOURWORDINLINE(0x2F3C, 0x8408, 0x0024, 0xA8B5);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( long )
VisibleLength                   (Ptr                    textPtr,
                                 long                   textLength)                         FOURWORDINLINE(0x2F3C, 0x8408, 0x0028, 0xA8B5);

EXTERN_API( void )
GetFormatOrder                  (FormatOrderPtr         ordering,
                                 short                  firstFormat,
                                 short                  lastFormat,
                                 Boolean                lineRight,
                                 StyleRunDirectionUPP   rlDirProc,
                                 Ptr                    dirParam)                           FOURWORDINLINE(0x2F3C, 0x8012, 0xFFFC, 0xA8B5);

EXTERN_API( void )
TextFont                        (short                  font)                               ONEWORDINLINE(0xA887);

EXTERN_API( void )
TextFace                        (StyleParameter         face)                               ONEWORDINLINE(0xA888);

EXTERN_API( void )
TextMode                        (short                  mode)                               ONEWORDINLINE(0xA889);

EXTERN_API( void )
TextSize                        (short                  size)                               ONEWORDINLINE(0xA88A);

EXTERN_API( void )
SpaceExtra                      (Fixed                  extra)                              ONEWORDINLINE(0xA88E);

EXTERN_API( void )
DrawChar                        (CharParameter          ch)                                 ONEWORDINLINE(0xA883);

EXTERN_API( void )
DrawString                      (ConstStr255Param       s)                                  ONEWORDINLINE(0xA884);

#if TARGET_OS_MAC
    #define MacDrawText DrawText
#endif
EXTERN_API( void )
MacDrawText                     (const void *           textBuf,
                                 short                  firstByte,
                                 short                  byteCount)                          ONEWORDINLINE(0xA885);

EXTERN_API( short )
CharWidth                       (CharParameter          ch)                                 ONEWORDINLINE(0xA88D);

EXTERN_API( short )
StringWidth                     (ConstStr255Param       s)                                  ONEWORDINLINE(0xA88C);

EXTERN_API( short )
TextWidth                       (const void *           textBuf,
                                 short                  firstByte,
                                 short                  byteCount)                          ONEWORDINLINE(0xA886);

EXTERN_API( void )
MeasureText                     (short                  count,
                                 const void *           textAddr,
                                 void *                 charLocs)                           ONEWORDINLINE(0xA837);

EXTERN_API( void )
GetFontInfo                     (FontInfo *             info)                               ONEWORDINLINE(0xA88B);

EXTERN_API( void )
CharExtra                       (Fixed                  extra)                              ONEWORDINLINE(0xAA23);

EXTERN_API( void )
StdText                         (short                  count,
                                 const void *           textAddr,
                                 Point                  numer,
                                 Point                  denom)                              ONEWORDINLINE(0xA882);

EXTERN_API( short )
StdTxMeas                       (short                  byteCount,
                                 const void *           textAddr,
                                 Point *                numer,
                                 Point *                denom,
                                 FontInfo *             info)                               ONEWORDINLINE(0xA8ED);


#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
drawstring                      (const char *           s);

EXTERN_API_C( short )
stringwidth                     (const char *           s);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API_C( void )
stdtext                         (short                  count,
                                 const void *           textAddr,
                                 const Point *          numer,
                                 const Point *          denom);

#if OLDROUTINENAMES
#define NPixel2Char(textBuf, textLen, slop, pixelWidth, leadingEdge, widthRemaining, styleRunPosition, numer, denom) \
    PixelToChar(textBuf, textLen, slop, pixelWidth, leadingEdge, widthRemaining, styleRunPosition, numer, denom) 
#define NChar2Pixel(textBuf, textLen, slop, offset, direction, styleRunPosition, numer, denom) \
    CharToPixel(textBuf, textLen, slop, offset, direction, styleRunPosition, numer, denom)
#define NDrawJust(textPtr, textLength, slop, styleRunPosition, numer, denom) \
    DrawJustified(textPtr, textLength, slop, styleRunPosition, numer, denom)
#define NMeasureJust(textPtr, textLength, slop, charLocs, styleRunPosition, numer, denom) \
    MeasureJustified(textPtr, textLength, slop, charLocs, styleRunPosition, numer, denom)
#define NPortionText(textPtr, textLen, styleRunPosition, numer, denom)\
    PortionLine(textPtr, textLen, styleRunPosition, numer, denom)
#endif  /* OLDROUTINENAMES */


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

#endif /* __QUICKDRAWTEXT__ */

