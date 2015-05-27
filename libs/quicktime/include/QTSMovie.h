/*
     File:       QTSMovie.h
 
     Contains:   QuickTime Interfaces.
 
     Version:    Technology: QuickTime 6.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QTSMOVIE__
#define __QTSMOVIE__

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __MOVIES__
#include <Movies.h>
#endif

#ifndef __QUICKTIMESTREAMING__
#include <QuickTimeStreaming.h>
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

enum {
    kQTSStreamMediaType         = FOUR_CHAR_CODE('strm')
};


struct QTSSampleDescription {
    long                            descSize;
    long                            dataFormat;
    long                            resvd1;                     /* set to 0*/
    short                           resvd2;                     /* set to 0*/
    short                           dataRefIndex;
    UInt32                          version;
    UInt32                          resvd3;                     /* set to 0*/
    SInt32                          flags;
                                                                /* qt atoms follow:*/
                                                                /*      long size, long type, some data*/
                                                                /*      repeat as necessary*/
};
typedef struct QTSSampleDescription     QTSSampleDescription;
typedef QTSSampleDescription *          QTSSampleDescriptionPtr;
typedef QTSSampleDescriptionPtr *       QTSSampleDescriptionHandle;
enum {
    kQTSSampleDescriptionVersion1 = 1L
};

enum {
    kQTSDefaultMediaTimeScale   = 600L
};

/* sample description flags*/
enum {
    kQTSSampleDescPassSampleDataAsHandleFlag = 0x00000001
};


/*============================================================================
        Stream Media Handler
============================================================================*/
/*-----------------------------------------
    Info Selectors
-----------------------------------------*/
/* all indexes start at 1 */

enum {
    kQTSMediaPresentationInfo   = FOUR_CHAR_CODE('pres'),       /* QTSMediaPresentationParams* */
    kQTSMediaNotificationInfo   = FOUR_CHAR_CODE('noti'),       /* QTSMediaNotificationParams* */
    kQTSMediaTotalDataRateInfo  = FOUR_CHAR_CODE('dtrt'),       /* UInt32*, bits/sec */
    kQTSMediaLostPercentInfo    = FOUR_CHAR_CODE('lspc'),       /* Fixed* */
    kQTSMediaNumStreamsInfo     = FOUR_CHAR_CODE('nstr'),       /* UInt32* */
    kQTSMediaIndSampleDescriptionInfo = FOUR_CHAR_CODE('isdc')  /* QTSMediaIndSampleDescriptionParams* */
};



struct QTSMediaPresentationParams {
    QTSPresentation                 presentationID;
};
typedef struct QTSMediaPresentationParams QTSMediaPresentationParams;

struct QTSMediaNotificationParams {
    QTSNotificationUPP              notificationProc;
    void *                          notificationRefCon;
    SInt32                          flags;
};
typedef struct QTSMediaNotificationParams QTSMediaNotificationParams;

struct QTSMediaIndSampleDescriptionParams {
    SInt32                          index;
    OSType                          returnedMediaType;
    SampleDescriptionHandle         returnedSampleDescription;
};
typedef struct QTSMediaIndSampleDescriptionParams QTSMediaIndSampleDescriptionParams;
/*-----------------------------------------
    QTS Media Handler Selectors
-----------------------------------------*/
enum {
    kQTSMediaSetInfoSelect      = 0x0100,
    kQTSMediaGetInfoSelect      = 0x0101,
    kQTSMediaSetIndStreamInfoSelect = 0x0102,
    kQTSMediaGetIndStreamInfoSelect = 0x0103
};

/*-----------------------------------------
    QTS Media Handler functions
-----------------------------------------*/
EXTERN_API( ComponentResult )
QTSMediaSetInfo                 (MediaHandler           mh,
                                 OSType                 inSelector,
                                 void *                 ioParams)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0100, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTSMediaGetInfo                 (MediaHandler           mh,
                                 OSType                 inSelector,
                                 void *                 ioParams)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTSMediaSetIndStreamInfo        (MediaHandler           mh,
                                 SInt32                 inIndex,
                                 OSType                 inSelector,
                                 void *                 ioParams)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTSMediaGetIndStreamInfo        (MediaHandler           mh,
                                 SInt32                 inIndex,
                                 OSType                 inSelector,
                                 void *                 ioParams)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x0103, 0x7000, 0xA82A);


/*============================================================================
        Hint Media Handler
============================================================================*/
enum {
    kQTSHintMediaType           = FOUR_CHAR_CODE('hint')
};

enum {
    kQTSHintTrackReference      = FOUR_CHAR_CODE('hint')
};





/* MixedMode ProcInfo constants for component calls */
enum {
    uppQTSMediaSetInfoProcInfo                 = 0x00000FF0,
    uppQTSMediaGetInfoProcInfo                 = 0x00000FF0,
    uppQTSMediaSetIndStreamInfoProcInfo        = 0x00003FF0,
    uppQTSMediaGetIndStreamInfoProcInfo        = 0x00003FF0
};

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

#endif /* __QTSMOVIE__ */

