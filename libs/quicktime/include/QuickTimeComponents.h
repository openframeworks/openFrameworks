/*
     File:       QuickTimeComponents.h
 
     Contains:   QuickTime Interfaces.
 
     Version:    Technology: QuickTime 6.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QUICKTIMECOMPONENTS__
#define __QUICKTIMECOMPONENTS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __IMAGECOMPRESSION__
#include <ImageCompression.h>
#endif

#ifndef __MOVIES__
#include <Movies.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __VIDEO__
#include <Video.h>
#endif

#ifndef __SOUND__
#include <Sound.h>
#endif

#ifndef __QUICKTIMEMUSIC__
#include <QuickTimeMusic.h>
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
    clockComponentType          = FOUR_CHAR_CODE('clok'),
    systemTickClock             = FOUR_CHAR_CODE('tick'),       /* subtype: 60ths since boot   */
    systemSecondClock           = FOUR_CHAR_CODE('seco'),       /* subtype: seconds since 1904       */
    systemMillisecondClock      = FOUR_CHAR_CODE('mill'),       /* subtype: 1000ths since boot       */
    systemMicrosecondClock      = FOUR_CHAR_CODE('micr')        /* subtype: 1000000ths since boot */
};

enum {
    kClockRateIsLinear          = 1,
    kClockImplementsCallBacks   = 2,
    kClockCanHandleIntermittentSound = 4                        /* sound clocks only */
};

#if OLDROUTINENAMES
#define GetClockTime(aClock, out) ClockGetTime(aClock, out)
#endif
/** These are Clock procedures **/
EXTERN_API( ComponentResult )
ClockGetTime                    (ComponentInstance      aClock,
                                 TimeRecord *           out)                                FIVEWORDINLINE(0x2F3C, 0x0004, 0x0001, 0x7000, 0xA82A);


EXTERN_API( QTCallBack )
ClockNewCallBack                (ComponentInstance      aClock,
                                 TimeBase               tb,
                                 short                  callBackType)                       FIVEWORDINLINE(0x2F3C, 0x0006, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
ClockDisposeCallBack            (ComponentInstance      aClock,
                                 QTCallBack             cb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
ClockCallMeWhen                 (ComponentInstance      aClock,
                                 QTCallBack             cb,
                                 long                   param1,
                                 long                   param2,
                                 long                   param3)                             FIVEWORDINLINE(0x2F3C, 0x0010, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
ClockCancelCallBack             (ComponentInstance      aClock,
                                 QTCallBack             cb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
ClockRateChanged                (ComponentInstance      aClock,
                                 QTCallBack             cb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
ClockTimeChanged                (ComponentInstance      aClock,
                                 QTCallBack             cb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
ClockSetTimeBase                (ComponentInstance      aClock,
                                 TimeBase               tb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0008, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
ClockStartStopChanged           (ComponentInstance      aClock,
                                 QTCallBack             cb,
                                 Boolean                startChanged,
                                 Boolean                stopChanged)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x0009, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
ClockGetRate                    (ComponentInstance      aClock,
                                 Fixed *                rate)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x000A, 0x7000, 0xA82A);




enum {
    StandardCompressionType     = FOUR_CHAR_CODE('scdi'),
    StandardCompressionSubType  = FOUR_CHAR_CODE('imag'),
    StandardCompressionSubTypeSound = FOUR_CHAR_CODE('soun')
};


typedef CALLBACK_API( Boolean , SCModalFilterProcPtr )(DialogRef theDialog, EventRecord *theEvent, short *itemHit, long refcon);
typedef CALLBACK_API( short , SCModalHookProcPtr )(DialogRef theDialog, short itemHit, void *params, long refcon);
typedef STACK_UPP_TYPE(SCModalFilterProcPtr)                    SCModalFilterUPP;
typedef STACK_UPP_TYPE(SCModalHookProcPtr)                      SCModalHookUPP;
/*  Preference flags.*/
enum {
    scListEveryCodec            = 1L << 1,
    scAllowZeroFrameRate        = 1L << 2,
    scAllowZeroKeyFrameRate     = 1L << 3,
    scShowBestDepth             = 1L << 4,
    scUseMovableModal           = 1L << 5,
    scDisableFrameRateItem      = 1L << 6,
    scShowDataRateAsKilobits    = 1L << 7
};


/*  Possible test flags for setting test image.*/
enum {
    scPreferCropping            = 1 << 0,
    scPreferScaling             = 1 << 1,
    scPreferScalingAndCropping  = scPreferScaling | scPreferCropping,
    scDontDetermineSettingsFromTestImage = 1 << 2
};


/*  Dimensions of the image preview box.*/
enum {
    scTestImageWidth            = 80,
    scTestImageHeight           = 80
};

/*  Possible items returned by hookProc.*/
enum {
    scOKItem                    = 1,
    scCancelItem                = 2,
    scCustomItem                = 3
};

/*  Result returned when user cancelled.*/
enum {
    scUserCancelled             = 1
};



/* Component selectors*/
enum {
    scPositionRect              = 2,
    scPositionDialog            = 3,
    scSetTestImagePictHandle    = 4,
    scSetTestImagePictFile      = 5,
    scSetTestImagePixMap        = 6,
    scGetBestDeviceRect         = 7,
    scRequestImageSettings      = 10,
    scCompressImage             = 11,
    scCompressPicture           = 12,
    scCompressPictureFile       = 13,
    scRequestSequenceSettings   = 14,
    scCompressSequenceBegin     = 15,
    scCompressSequenceFrame     = 16,
    scCompressSequenceEnd       = 17,
    scDefaultPictHandleSettings = 18,
    scDefaultPictFileSettings   = 19,
    scDefaultPixMapSettings     = 20,
    scGetInfo                   = 21,
    scSetInfo                   = 22,
    scNewGWorld                 = 23
};

/*  Get/SetInfo structures.*/


struct SCSpatialSettings {
    CodecType                       codecType;
    CodecComponent                  codec;
    short                           depth;
    CodecQ                          spatialQuality;
};
typedef struct SCSpatialSettings        SCSpatialSettings;

struct SCTemporalSettings {
    CodecQ                          temporalQuality;
    Fixed                           frameRate;
    long                            keyFrameRate;
};
typedef struct SCTemporalSettings       SCTemporalSettings;

struct SCDataRateSettings {
    long                            dataRate;
    long                            frameDuration;
    CodecQ                          minSpatialQuality;
    CodecQ                          minTemporalQuality;
};
typedef struct SCDataRateSettings       SCDataRateSettings;

struct SCExtendedProcs {
    SCModalFilterUPP                filterProc;
    SCModalHookUPP                  hookProc;
    long                            refcon;
    Str31                           customName;
};
typedef struct SCExtendedProcs          SCExtendedProcs;
/*  Get/SetInfo selectors*/
enum {
    scSpatialSettingsType       = FOUR_CHAR_CODE('sptl'),       /* pointer to SCSpatialSettings struct*/
    scTemporalSettingsType      = FOUR_CHAR_CODE('tprl'),       /* pointer to SCTemporalSettings struct*/
    scDataRateSettingsType      = FOUR_CHAR_CODE('drat'),       /* pointer to SCDataRateSettings struct*/
    scColorTableType            = FOUR_CHAR_CODE('clut'),       /* pointer to CTabHandle*/
    scProgressProcType          = FOUR_CHAR_CODE('prog'),       /* pointer to ProgressRecord struct*/
    scExtendedProcsType         = FOUR_CHAR_CODE('xprc'),       /* pointer to SCExtendedProcs struct*/
    scPreferenceFlagsType       = FOUR_CHAR_CODE('pref'),       /* pointer to long*/
    scSettingsStateType         = FOUR_CHAR_CODE('ssta'),       /* pointer to Handle*/
    scSequenceIDType            = FOUR_CHAR_CODE('sequ'),       /* pointer to ImageSequence*/
    scWindowPositionType        = FOUR_CHAR_CODE('wndw'),       /* pointer to Point*/
    scCodecFlagsType            = FOUR_CHAR_CODE('cflg'),       /* pointer to CodecFlags*/
    scCodecSettingsType         = FOUR_CHAR_CODE('cdec'),       /* pointer to Handle*/
    scForceKeyValueType         = FOUR_CHAR_CODE('ksim'),       /* pointer to long*/
    scSoundSampleRateType       = FOUR_CHAR_CODE('ssrt'),       /* pointer to UnsignedFixed*/
    scSoundSampleSizeType       = FOUR_CHAR_CODE('ssss'),       /* pointer to short*/
    scSoundChannelCountType     = FOUR_CHAR_CODE('sscc'),       /* pointer to short*/
    scSoundCompressionType      = FOUR_CHAR_CODE('ssct'),       /* pointer to OSType*/
    scCompressionListType       = FOUR_CHAR_CODE('ctyl'),       /* pointer to OSType Handle*/
    scCodecManufacturerType     = FOUR_CHAR_CODE('cmfr'),       /* pointer to OSType*/
    scSoundVBRCompressionOK     = FOUR_CHAR_CODE('cvbr'),       /* pointer to Boolean*/
    scSoundInputSampleRateType  = FOUR_CHAR_CODE('ssir'),       /* pointer to UnsignedFixed*/
    scSoundSampleRateChangeOK   = FOUR_CHAR_CODE('rcok'),       /* pointer to Boolean*/
    scAvailableCompressionListType = FOUR_CHAR_CODE('avai')     /* pointer to OSType Handle*/
};

/*  scTypeNotFoundErr returned by Get/SetInfo when type cannot be found.*/



struct SCParams {
    long                            flags;
    CodecType                       theCodecType;
    CodecComponent                  theCodec;
    CodecQ                          spatialQuality;
    CodecQ                          temporalQuality;
    short                           depth;
    Fixed                           frameRate;
    long                            keyFrameRate;
    long                            reserved1;
    long                            reserved2;
};
typedef struct SCParams                 SCParams;
enum {
    scGetCompression            = 1,
    scShowMotionSettings        = 1L << 0,
    scSettingsChangedItem       = -1
};

enum {
    scCompressFlagIgnoreIdenticalFrames = 1
};

/* QTAtomTypes for atoms found in settings atom containers*/
enum {
    kQTSettingsVideo            = FOUR_CHAR_CODE('vide'),       /* Container for video/image compression related atoms (Get/SetInfo selectors)*/
    kQTSettingsSound            = FOUR_CHAR_CODE('soun'),       /* Container for sound compression related atoms (Get/SetInfo selectors)*/
    kQTSettingsComponentVersion = FOUR_CHAR_CODE('vers')        /* . Version of component that wrote settings (QTSettingsVersionAtomRecord)*/
};

/* Format of 'vers' atom found in settings atom containers*/

struct QTSettingsVersionAtomRecord {
    long                            componentVersion;           /* standard compression component version*/
    short                           flags;                      /* low bit is 1 if little endian platform, 0 if big endian platform*/
    short                           reserved;                   /* should be 0*/
};
typedef struct QTSettingsVersionAtomRecord QTSettingsVersionAtomRecord;
#define SCGetCompression(ci, params, where) SCGetCompressionExtended(ci,params,where,0,0,0,0)
/** These are Progress procedures **/
EXTERN_API( ComponentResult )
SCGetCompressionExtended        (ComponentInstance      ci,
                                 SCParams *             params,
                                 Point                  where,
                                 SCModalFilterUPP       filterProc,
                                 SCModalHookUPP         hookProc,
                                 long                   refcon,
                                 StringPtr              customName)                         FIVEWORDINLINE(0x2F3C, 0x0018, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCPositionRect                  (ComponentInstance      ci,
                                 Rect *                 rp,
                                 Point *                where)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCPositionDialog                (ComponentInstance      ci,
                                 short                  id,
                                 Point *                where)                              FIVEWORDINLINE(0x2F3C, 0x0006, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCSetTestImagePictHandle        (ComponentInstance      ci,
                                 PicHandle              testPict,
                                 Rect *                 testRect,
                                 short                  testFlags)                          FIVEWORDINLINE(0x2F3C, 0x000A, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCSetTestImagePictFile          (ComponentInstance      ci,
                                 short                  testFileRef,
                                 Rect *                 testRect,
                                 short                  testFlags)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCSetTestImagePixMap            (ComponentInstance      ci,
                                 PixMapHandle           testPixMap,
                                 Rect *                 testRect,
                                 short                  testFlags)                          FIVEWORDINLINE(0x2F3C, 0x000A, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCGetBestDeviceRect             (ComponentInstance      ci,
                                 Rect *                 r)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0007, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
SCRequestImageSettings          (ComponentInstance      ci)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x000A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCCompressImage                 (ComponentInstance      ci,
                                 PixMapHandle           src,
                                 const Rect *           srcRect,
                                 ImageDescriptionHandle * desc,
                                 Handle *               data)                               FIVEWORDINLINE(0x2F3C, 0x0010, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCCompressPicture               (ComponentInstance      ci,
                                 PicHandle              srcPicture,
                                 PicHandle              dstPicture)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x000C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCCompressPictureFile           (ComponentInstance      ci,
                                 short                  srcRefNum,
                                 short                  dstRefNum)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x000D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCRequestSequenceSettings       (ComponentInstance      ci)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x000E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCCompressSequenceBegin         (ComponentInstance      ci,
                                 PixMapHandle           src,
                                 const Rect *           srcRect,
                                 ImageDescriptionHandle * desc)                             FIVEWORDINLINE(0x2F3C, 0x000C, 0x000F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCCompressSequenceFrame         (ComponentInstance      ci,
                                 PixMapHandle           src,
                                 const Rect *           srcRect,
                                 Handle *               data,
                                 long *                 dataSize,
                                 short *                notSyncFlag)                        FIVEWORDINLINE(0x2F3C, 0x0014, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCCompressSequenceEnd           (ComponentInstance      ci)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0011, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCDefaultPictHandleSettings     (ComponentInstance      ci,
                                 PicHandle              srcPicture,
                                 short                  motion)                             FIVEWORDINLINE(0x2F3C, 0x0006, 0x0012, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCDefaultPictFileSettings       (ComponentInstance      ci,
                                 short                  srcRef,
                                 short                  motion)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0013, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCDefaultPixMapSettings         (ComponentInstance      ci,
                                 PixMapHandle           src,
                                 short                  motion)                             FIVEWORDINLINE(0x2F3C, 0x0006, 0x0014, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCGetInfo                       (ComponentInstance      ci,
                                 OSType                 infoType,
                                 void *                 info)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x0015, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCSetInfo                       (ComponentInstance      ci,
                                 OSType                 infoType,
                                 void *                 info)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x0016, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCNewGWorld                     (ComponentInstance      ci,
                                 GWorldPtr *            gwp,
                                 Rect *                 rp,
                                 GWorldFlags            flags)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0017, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCSetCompressFlags              (ComponentInstance      ci,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0018, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCGetCompressFlags              (ComponentInstance      ci,
                                 long *                 flags)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0019, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCGetSettingsAsText             (ComponentInstance      ci,
                                 Handle *               text)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x001A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCGetSettingsAsAtomContainer    (ComponentInstance      ci,
                                 QTAtomContainer *      settings)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x001B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCSetSettingsFromAtomContainer  (ComponentInstance      ci,
                                 QTAtomContainer        settings)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x001C, 0x7000, 0xA82A);

/* Note: if you're using SCCompressSequenceFrameAsync with a scForceKeyValue setting, you must call SCAsyncIdle occasionally at main task time. */
EXTERN_API( ComponentResult )
SCCompressSequenceFrameAsync    (ComponentInstance      ci,
                                 PixMapHandle           src,
                                 const Rect *           srcRect,
                                 Handle *               data,
                                 long *                 dataSize,
                                 short *                notSyncFlag,
                                 ICMCompletionProcRecordPtr  asyncCompletionProc)           FIVEWORDINLINE(0x2F3C, 0x0018, 0x001D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SCAsyncIdle                     (ComponentInstance      ci)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x001E, 0x7000, 0xA82A);




enum {
    TweenComponentType          = FOUR_CHAR_CODE('twen')
};


typedef ComponentInstance               TweenerComponent;
EXTERN_API( ComponentResult )
TweenerInitialize               (TweenerComponent       tc,
                                 QTAtomContainer        container,
                                 QTAtom                 tweenAtom,
                                 QTAtom                 dataAtom)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TweenerDoTween                  (TweenerComponent       tc,
                                 TweenRecord *          tr)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TweenerReset                    (TweenerComponent       tc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0003, 0x7000, 0xA82A);



enum {
    TCSourceRefNameType         = FOUR_CHAR_CODE('name')
};

enum {
    tcDropFrame                 = 1 << 0,
    tc24HourMax                 = 1 << 1,
    tcNegTimesOK                = 1 << 2,
    tcCounter                   = 1 << 3
};


struct TimeCodeDef {
    long                            flags;                      /* drop-frame, etc.*/
    TimeScale                       fTimeScale;                 /* time scale of frameDuration (eg. 2997)*/
    TimeValue                       frameDuration;              /* duration of each frame (eg. 100)*/
    UInt8                           numFrames;                  /* frames/sec for timecode (eg. 30) OR frames/tick for counter mode*/
    UInt8                           padding;                    /* unused padding byte*/
};
typedef struct TimeCodeDef              TimeCodeDef;
enum {
    tctNegFlag                  = 0x80                          /* negative bit is in minutes*/
};


struct TimeCodeTime {
    UInt8                           hours;
    UInt8                           minutes;
    UInt8                           seconds;
    UInt8                           frames;
};
typedef struct TimeCodeTime             TimeCodeTime;

struct TimeCodeCounter {
    long                            counter;
};
typedef struct TimeCodeCounter          TimeCodeCounter;

union TimeCodeRecord {
    TimeCodeTime                    t;
    TimeCodeCounter                 c;
};
typedef union TimeCodeRecord            TimeCodeRecord;

struct TimeCodeDescription {
    long                            descSize;                   /* standard sample description header*/
    long                            dataFormat;
    long                            resvd1;
    short                           resvd2;
    short                           dataRefIndex;
    long                            flags;                      /* timecode specific stuff*/
    TimeCodeDef                     timeCodeDef;
    long                            srcRef[1];
};
typedef struct TimeCodeDescription      TimeCodeDescription;
typedef TimeCodeDescription *           TimeCodeDescriptionPtr;
typedef TimeCodeDescriptionPtr *        TimeCodeDescriptionHandle;
enum {
    tcdfShowTimeCode            = 1 << 0
};



struct TCTextOptions {
    short                           txFont;
    short                           txFace;
    short                           txSize;
    short                           pad;                        /* let's make it longword aligned - thanks.. */
    RGBColor                        foreColor;
    RGBColor                        backColor;
};
typedef struct TCTextOptions            TCTextOptions;
typedef TCTextOptions *                 TCTextOptionsPtr;
EXTERN_API( HandlerError )
TCGetCurrentTimeCode            (MediaHandler           mh,
                                 long *                 frameNum,
                                 TimeCodeDef *          tcdef,
                                 TimeCodeRecord *       tcrec,
                                 UserData *             srcRefH)                            FIVEWORDINLINE(0x2F3C, 0x0010, 0x0101, 0x7000, 0xA82A);

EXTERN_API( HandlerError )
TCGetTimeCodeAtTime             (MediaHandler           mh,
                                 TimeValue              mediaTime,
                                 long *                 frameNum,
                                 TimeCodeDef *          tcdef,
                                 TimeCodeRecord *       tcdata,
                                 UserData *             srcRefH)                            FIVEWORDINLINE(0x2F3C, 0x0014, 0x0102, 0x7000, 0xA82A);

EXTERN_API( HandlerError )
TCTimeCodeToString              (MediaHandler           mh,
                                 TimeCodeDef *          tcdef,
                                 TimeCodeRecord *       tcrec,
                                 StringPtr              tcStr)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0103, 0x7000, 0xA82A);

EXTERN_API( HandlerError )
TCTimeCodeToFrameNumber         (MediaHandler           mh,
                                 TimeCodeDef *          tcdef,
                                 TimeCodeRecord *       tcrec,
                                 long *                 frameNumber)                        FIVEWORDINLINE(0x2F3C, 0x000C, 0x0104, 0x7000, 0xA82A);

EXTERN_API( HandlerError )
TCFrameNumberToTimeCode         (MediaHandler           mh,
                                 long                   frameNumber,
                                 TimeCodeDef *          tcdef,
                                 TimeCodeRecord *       tcrec)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0105, 0x7000, 0xA82A);

EXTERN_API( HandlerError )
TCGetSourceRef                  (MediaHandler           mh,
                                 TimeCodeDescriptionHandle  tcdH,
                                 UserData *             srefH)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0106, 0x7000, 0xA82A);

EXTERN_API( HandlerError )
TCSetSourceRef                  (MediaHandler           mh,
                                 TimeCodeDescriptionHandle  tcdH,
                                 UserData               srefH)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0107, 0x7000, 0xA82A);

EXTERN_API( HandlerError )
TCSetTimeCodeFlags              (MediaHandler           mh,
                                 long                   flags,
                                 long                   flagsMask)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0108, 0x7000, 0xA82A);

EXTERN_API( HandlerError )
TCGetTimeCodeFlags              (MediaHandler           mh,
                                 long *                 flags)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0109, 0x7000, 0xA82A);

EXTERN_API( HandlerError )
TCSetDisplayOptions             (MediaHandler           mh,
                                 TCTextOptionsPtr       textOptions)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x010A, 0x7000, 0xA82A);

EXTERN_API( HandlerError )
TCGetDisplayOptions             (MediaHandler           mh,
                                 TCTextOptionsPtr       textOptions)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x010B, 0x7000, 0xA82A);




typedef ComponentInstance               MovieImportComponent;
typedef ComponentInstance               MovieExportComponent;
enum {
    MovieImportType             = FOUR_CHAR_CODE('eat '),
    MovieExportType             = FOUR_CHAR_CODE('spit')
};

enum {
    canMovieImportHandles       = 1 << 0,
    canMovieImportFiles         = 1 << 1,
    hasMovieImportUserInterface = 1 << 2,
    canMovieExportHandles       = 1 << 3,
    canMovieExportFiles         = 1 << 4,
    hasMovieExportUserInterface = 1 << 5,
    movieImporterIsXMLBased     = 1 << 5,
    dontAutoFileMovieImport     = 1 << 6,
    canMovieExportAuxDataHandle = 1 << 7,
    canMovieImportValidateHandles = 1 << 8,
    canMovieImportValidateFile  = 1 << 9,
    dontRegisterWithEasyOpen    = 1 << 10,
    canMovieImportInPlace       = 1 << 11,
    movieImportSubTypeIsFileExtension = 1 << 12,
    canMovieImportPartial       = 1 << 13,
    hasMovieImportMIMEList      = 1 << 14,
    canMovieImportAvoidBlocking = 1 << 15,
    canMovieExportFromProcedures = 1 << 15,
    canMovieExportValidateMovie = 1L << 16,
    movieImportMustGetDestinationMediaType = 1L << 16,
    movieExportNeedsResourceFork = 1L << 17,
    canMovieImportDataReferences = 1L << 18,
    movieExportMustGetSourceMediaType = 1L << 19,
    canMovieImportWithIdle      = 1L << 20,
    canMovieImportValidateDataReferences = 1L << 21,
    reservedForUseByGraphicsImporters = 1L << 23
};

enum {
    movieImportCreateTrack      = 1,
    movieImportInParallel       = 2,
    movieImportMustUseTrack     = 4,
    movieImportWithIdle         = 16
};

enum {
    movieImportResultUsedMultipleTracks = 8,
    movieImportResultNeedIdles  = 32,
    movieImportResultComplete   = 64
};

enum {
    kMovieExportTextOnly        = 0,
    kMovieExportAbsoluteTime    = 1,
    kMovieExportRelativeTime    = 2
};

enum {
    kMIDIImportSilenceBefore    = 1 << 0,
    kMIDIImportSilenceAfter     = 1 << 1,
    kMIDIImport20Playable       = 1 << 2,
    kMIDIImportWantLyrics       = 1 << 3
};


enum {
    kQTMediaConfigResourceType  = FOUR_CHAR_CODE('mcfg'),
    kQTMediaConfigResourceVersion = 2,
    kQTMediaGroupResourceType   = FOUR_CHAR_CODE('mgrp'),
    kQTMediaGroupResourceVersion = 1,
    kQTBrowserInfoResourceType  = FOUR_CHAR_CODE('brws'),
    kQTBrowserInfoResourceVersion = 1
};


enum {
    kQTMediaMIMEInfoHasChanged  = (1L << 1),                    /* the MIME type(s) is(are) new or has changed since the last time */
                                                                /*  someone asked about it */
    kQTMediaFileInfoHasChanged  = (1L << 2),                    /* the file extension(s) is(are) new or has changed since the last time */
                                                                /*  anyone asked about it */
    kQTMediaConfigCanUseApp     = (1L << 18),                   /* this MIME type can be configured to use app */
    kQTMediaConfigCanUsePlugin  = (1L << 19),                   /* this MIME type can be configured to use plug-in */
    kQTMediaConfigUNUSED        = (1L << 20),                   /* currently unused */
    kQTMediaConfigBinaryFile    = (1L << 23),                   /* file should be transfered in binary mode */
    kQTMediaConfigTextFile      = 0,                            /* not a bit, defined for clarity */
    kQTMediaConfigMacintoshFile = (1L << 24),                   /* file's resource fork is significant */
    kQTMediaConfigAssociateByDefault = (1L << 27),              /* take this file association by default */
    kQTMediaConfigUseAppByDefault = (1L << 28),                 /* use the app by default for this MIME type */
    kQTMediaConfigUsePluginByDefault = (1L << 29),              /* use the plug-in by default for this MIME type */
    kQTMediaConfigDefaultsMask  = (kQTMediaConfigUseAppByDefault | kQTMediaConfigUsePluginByDefault),
    kQTMediaConfigDefaultsShift = 12,                           /* ((flags & kQTMediaConfigDefaultsMask) >> kQTMediaConfigDefaultsShift) to get default setting */
    kQTMediaConfigHasFileHasQTAtoms = (1L << 30)                /* the file has a "QuickTime like" file format */
};


/* mime type group constants for groupID field of 'mcfg' resource*/
enum {
    kQTMediaConfigStreamGroupID = FOUR_CHAR_CODE('strm'),
    kQTMediaConfigInteractiveGroupID = FOUR_CHAR_CODE('intr'),
    kQTMediaConfigVideoGroupID  = FOUR_CHAR_CODE('eyes'),
    kQTMediaConfigAudioGroupID  = FOUR_CHAR_CODE('ears'),
    kQTMediaConfigMPEGGroupID   = FOUR_CHAR_CODE('mpeg'),
    kQTMediaConfigMP3GroupID    = FOUR_CHAR_CODE('mp3 '),
    kQTMediaConfigImageGroupID  = FOUR_CHAR_CODE('ogle'),
    kQTMediaConfigMiscGroupID   = FOUR_CHAR_CODE('misc')
};

/* file type group constants for groupID field of 'mcfg' resource*/
enum {
    kQTMediaInfoNetGroup        = FOUR_CHAR_CODE('net '),
    kQTMediaInfoWinGroup        = FOUR_CHAR_CODE('win '),
    kQTMediaInfoMacGroup        = FOUR_CHAR_CODE('mac '),
    kQTMediaInfoMiscGroup       = 0x3F3F3F3F                    /* '????'*/
};



enum {
    kMimeInfoMimeTypeTag        = FOUR_CHAR_CODE('mime'),
    kMimeInfoFileExtensionTag   = FOUR_CHAR_CODE('ext '),
    kMimeInfoDescriptionTag     = FOUR_CHAR_CODE('desc'),
    kMimeInfoGroupTag           = FOUR_CHAR_CODE('grop'),
    kMimeInfoDoNotOverrideExistingFileTypeAssociation = FOUR_CHAR_CODE('nofa')
};

enum {
    kQTFileTypeAIFF             = FOUR_CHAR_CODE('AIFF'),
    kQTFileTypeAIFC             = FOUR_CHAR_CODE('AIFC'),
    kQTFileTypeDVC              = FOUR_CHAR_CODE('dvc!'),
    kQTFileTypeMIDI             = FOUR_CHAR_CODE('Midi'),
    kQTFileTypePicture          = FOUR_CHAR_CODE('PICT'),
    kQTFileTypeMovie            = FOUR_CHAR_CODE('MooV'),
    kQTFileTypeText             = FOUR_CHAR_CODE('TEXT'),
    kQTFileTypeWave             = FOUR_CHAR_CODE('WAVE'),
    kQTFileTypeSystemSevenSound = FOUR_CHAR_CODE('sfil'),
    kQTFileTypeMuLaw            = FOUR_CHAR_CODE('ULAW'),
    kQTFileTypeAVI              = FOUR_CHAR_CODE('VfW '),
    kQTFileTypeSoundDesignerII  = FOUR_CHAR_CODE('Sd2f'),
    kQTFileTypeAudioCDTrack     = FOUR_CHAR_CODE('trak'),
    kQTFileTypePICS             = FOUR_CHAR_CODE('PICS'),
    kQTFileTypeGIF              = FOUR_CHAR_CODE('GIFf'),
    kQTFileTypePNG              = FOUR_CHAR_CODE('PNGf'),
    kQTFileTypeTIFF             = FOUR_CHAR_CODE('TIFF'),
    kQTFileTypePhotoShop        = FOUR_CHAR_CODE('8BPS'),
    kQTFileTypeSGIImage         = FOUR_CHAR_CODE('.SGI'),
    kQTFileTypeBMP              = FOUR_CHAR_CODE('BMPf'),
    kQTFileTypeJPEG             = FOUR_CHAR_CODE('JPEG'),
    kQTFileTypeJFIF             = FOUR_CHAR_CODE('JPEG'),
    kQTFileTypeMacPaint         = FOUR_CHAR_CODE('PNTG'),
    kQTFileTypeTargaImage       = FOUR_CHAR_CODE('TPIC'),
    kQTFileTypeQuickDrawGXPicture = FOUR_CHAR_CODE('qdgx'),
    kQTFileTypeQuickTimeImage   = FOUR_CHAR_CODE('qtif'),
    kQTFileType3DMF             = FOUR_CHAR_CODE('3DMF'),
    kQTFileTypeFLC              = FOUR_CHAR_CODE('FLC '),
    kQTFileTypeFlash            = FOUR_CHAR_CODE('SWFL'),
    kQTFileTypeFlashPix         = FOUR_CHAR_CODE('FPix'),
    kQTFileTypeMP4              = FOUR_CHAR_CODE('mpg4')
};

/* QTAtomTypes for atoms in import/export settings containers*/
enum {
    kQTSettingsDVExportNTSC     = FOUR_CHAR_CODE('dvcv'),       /* True is export as NTSC, false is export as PAL. (Boolean)*/
    kQTSettingsDVExportLockedAudio = FOUR_CHAR_CODE('lock'),    /* True if audio locked to video. (Boolean)*/
    kQTSettingsEffect           = FOUR_CHAR_CODE('effe'),       /* Parent atom whose contents are atoms of an effects description*/
    kQTSettingsGraphicsFileImportSequence = FOUR_CHAR_CODE('sequ'), /* Parent atom of graphic file movie import component*/
    kQTSettingsGraphicsFileImportSequenceEnabled = FOUR_CHAR_CODE('enab'), /* . If true, import numbered image sequence (Boolean)*/
    kQTSettingsMovieExportEnableVideo = FOUR_CHAR_CODE('envi'), /* Enable exporting of video track (Boolean)*/
    kQTSettingsMovieExportEnableSound = FOUR_CHAR_CODE('enso'), /* Enable exporting of sound track (Boolean)*/
    kQTSettingsMovieExportSaveOptions = FOUR_CHAR_CODE('save'), /* Parent atom of save options*/
    kQTSettingsMovieExportSaveForInternet = FOUR_CHAR_CODE('fast'), /* . Save for Internet*/
    kQTSettingsMovieExportSaveCompressedMovie = FOUR_CHAR_CODE('cmpm'), /* . Save compressed movie resource*/
    kQTSettingsMIDI             = FOUR_CHAR_CODE('MIDI'),       /* MIDI import related container*/
    kQTSettingsMIDISettingFlags = FOUR_CHAR_CODE('sttg'),       /* . MIDI import settings (UInt32)*/
    kQTSettingsText             = FOUR_CHAR_CODE('text'),       /* Text related container*/
    kQTSettingsTextDescription  = FOUR_CHAR_CODE('desc'),       /* . Text import settings (TextDescription record)*/
    kQTSettingsTextSize         = FOUR_CHAR_CODE('size'),       /* . Width/height to create during import (FixedPoint)*/
    kQTSettingsTextSettingFlags = FOUR_CHAR_CODE('sttg'),       /* . Text export settings (UInt32)*/
    kQTSettingsTextTimeFraction = FOUR_CHAR_CODE('timf'),       /* . Movie time fraction for export (UInt32)*/
    kQTSettingsTime             = FOUR_CHAR_CODE('time'),       /* Time related container*/
    kQTSettingsTimeDuration     = FOUR_CHAR_CODE('dura'),       /* . Time related container*/
    kQTSettingsAudioCDTrack     = FOUR_CHAR_CODE('trak'),       /* Audio CD track related container*/
    kQTSettingsAudioCDTrackRateShift = FOUR_CHAR_CODE('rshf'),  /* . Rate shift to be performed (SInt16)*/
    kQTSettingsDVExportDVFormat = FOUR_CHAR_CODE('dvcf')        /* Exported DV Format, DV('dv  ') or DVCPRO('dvp '). (OSType)*/
};





struct MovieExportGetDataParams {
    long                            recordSize;

    long                            trackID;

    TimeScale                       sourceTimeScale;
    TimeValue                       requestedTime;
    TimeValue                       actualTime;

    Ptr                             dataPtr;
    long                            dataSize;

    SampleDescriptionHandle         desc;
    OSType                          descType;
    long                            descSeed;

    long                            requestedSampleCount;
    long                            actualSampleCount;
    TimeValue                       durationPerSample;
    long                            sampleFlags;
};
typedef struct MovieExportGetDataParams MovieExportGetDataParams;
typedef CALLBACK_API( OSErr , MovieExportGetDataProcPtr )(void *refCon, MovieExportGetDataParams *params);
typedef CALLBACK_API( OSErr , MovieExportGetPropertyProcPtr )(void *refcon, long trackID, OSType propertyType, void *propertyValue);
enum {
    kQTPresetsListResourceType  = FOUR_CHAR_CODE('stg#'),
    kQTPresetsPlatformListResourceType = FOUR_CHAR_CODE('stgp')
};

enum {
    kQTPresetInfoIsDivider      = 1
};


struct QTPresetInfo {
    OSType                          presetKey;                  /* unique key for this preset in presetsArray */
    UInt32                          presetFlags;                /* flags about this preset */
    OSType                          settingsResourceType;       /* resource type of settings resource */
    SInt16                          settingsResourceID;         /* resource id of settings resource */
    SInt16                          padding1;
    SInt16                          nameStringListID;           /* name string list resource id */
    SInt16                          nameStringIndex;            /* name string index */
    SInt16                          infoStringListID;           /* info string list resource id */
    SInt16                          infoStringIndex;            /* info string index */
};
typedef struct QTPresetInfo             QTPresetInfo;

struct QTPresetListRecord {
    UInt32                          flags;                      /* flags for whole list */
    UInt32                          count;                      /* number of elements in presetsArray */
    UInt32                          reserved;
    QTPresetInfo                    presetsArray[1];            /* info about each preset */
};
typedef struct QTPresetListRecord       QTPresetListRecord;
enum {
    kQTMovieExportSourceInfoResourceType = FOUR_CHAR_CODE('src#'),
    kQTMovieExportSourceInfoIsMediaType = 1L << 0,
    kQTMovieExportSourceInfoIsMediaCharacteristic = 1L << 1,
    kQTMovieExportSourceInfoIsSourceType = 1L << 2
};


struct QTMovieExportSourceInfo {
    OSType                          mediaType;                  /* Media type of source */
    UInt16                          minCount;                   /* min number of sources of this kind required, zero if none required */
    UInt16                          maxCount;                   /* max number of sources of this kind allowed, -1 if unlimited allowed */
    long                            flags;                      /* reserved for flags */
};
typedef struct QTMovieExportSourceInfo  QTMovieExportSourceInfo;

struct QTMovieExportSourceRecord {
    long                            count;
    long                            reserved;
    QTMovieExportSourceInfo         sourceArray[1];
};
typedef struct QTMovieExportSourceRecord QTMovieExportSourceRecord;
typedef STACK_UPP_TYPE(MovieExportGetDataProcPtr)               MovieExportGetDataUPP;
typedef STACK_UPP_TYPE(MovieExportGetPropertyProcPtr)           MovieExportGetPropertyUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(SCModalFilterUPP)
    NewSCModalFilterUPP            (SCModalFilterProcPtr    userRoutine);

    EXTERN_API(SCModalHookUPP)
    NewSCModalHookUPP              (SCModalHookProcPtr      userRoutine);

    EXTERN_API(MovieExportGetDataUPP)
    NewMovieExportGetDataUPP       (MovieExportGetDataProcPtr userRoutine);

    EXTERN_API(MovieExportGetPropertyUPP)
    NewMovieExportGetPropertyUPP    (MovieExportGetPropertyProcPtr userRoutine);

    EXTERN_API(void)
    DisposeSCModalFilterUPP        (SCModalFilterUPP        userUPP);

    EXTERN_API(void)
    DisposeSCModalHookUPP          (SCModalHookUPP          userUPP);

    EXTERN_API(void)
    DisposeMovieExportGetDataUPP    (MovieExportGetDataUPP  userUPP);

    EXTERN_API(void)
    DisposeMovieExportGetPropertyUPP    (MovieExportGetPropertyUPP userUPP);

    EXTERN_API(Boolean)
    InvokeSCModalFilterUPP         (DialogRef               theDialog,
                                    EventRecord *           theEvent,
                                    short *                 itemHit,
                                    long                    refcon,
                                    SCModalFilterUPP        userUPP);

    EXTERN_API(short)
    InvokeSCModalHookUPP           (DialogRef               theDialog,
                                    short                   itemHit,
                                    void *                  params,
                                    long                    refcon,
                                    SCModalHookUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeMovieExportGetDataUPP    (void *                  refCon,
                                    MovieExportGetDataParams * params,
                                    MovieExportGetDataUPP   userUPP);

    EXTERN_API(OSErr)
    InvokeMovieExportGetPropertyUPP    (void *              refcon,
                                    long                    trackID,
                                    OSType                  propertyType,
                                    void *                  propertyValue,
                                    MovieExportGetPropertyUPP userUPP);

#else
    enum { uppSCModalFilterProcInfo = 0x00003FD0 };                 /* pascal 1_byte Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppSCModalHookProcInfo = 0x00003EE0 };                   /* pascal 2_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes) */
    enum { uppMovieExportGetDataProcInfo = 0x000003E0 };            /* pascal 2_bytes Func(4_bytes, 4_bytes) */
    enum { uppMovieExportGetPropertyProcInfo = 0x00003FE0 };        /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewSCModalFilterUPP(userRoutine)                        (SCModalFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSCModalFilterProcInfo, GetCurrentArchitecture())
    #define NewSCModalHookUPP(userRoutine)                          (SCModalHookUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSCModalHookProcInfo, GetCurrentArchitecture())
    #define NewMovieExportGetDataUPP(userRoutine)                   (MovieExportGetDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMovieExportGetDataProcInfo, GetCurrentArchitecture())
    #define NewMovieExportGetPropertyUPP(userRoutine)               (MovieExportGetPropertyUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMovieExportGetPropertyProcInfo, GetCurrentArchitecture())
    #define DisposeSCModalFilterUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeSCModalHookUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposeMovieExportGetDataUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define DisposeMovieExportGetPropertyUPP(userUPP)               DisposeRoutineDescriptor(userUPP)
    #define InvokeSCModalFilterUPP(theDialog, theEvent, itemHit, refcon, userUPP)  (Boolean)CALL_FOUR_PARAMETER_UPP((userUPP), uppSCModalFilterProcInfo, (theDialog), (theEvent), (itemHit), (refcon))
    #define InvokeSCModalHookUPP(theDialog, itemHit, params, refcon, userUPP)  (short)CALL_FOUR_PARAMETER_UPP((userUPP), uppSCModalHookProcInfo, (theDialog), (itemHit), (params), (refcon))
    #define InvokeMovieExportGetDataUPP(refCon, params, userUPP)    (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppMovieExportGetDataProcInfo, (refCon), (params))
    #define InvokeMovieExportGetPropertyUPP(refcon, trackID, propertyType, propertyValue, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppMovieExportGetPropertyProcInfo, (refcon), (trackID), (propertyType), (propertyValue))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewSCModalFilterProc(userRoutine)                       NewSCModalFilterUPP(userRoutine)
#define NewSCModalHookProc(userRoutine)                         NewSCModalHookUPP(userRoutine)
#define NewMovieExportGetDataProc(userRoutine)                  NewMovieExportGetDataUPP(userRoutine)
#define NewMovieExportGetPropertyProc(userRoutine)              NewMovieExportGetPropertyUPP(userRoutine)
#define CallSCModalFilterProc(userRoutine, theDialog, theEvent, itemHit, refcon) InvokeSCModalFilterUPP(theDialog, theEvent, itemHit, refcon, userRoutine)
#define CallSCModalHookProc(userRoutine, theDialog, itemHit, params, refcon) InvokeSCModalHookUPP(theDialog, itemHit, params, refcon, userRoutine)
#define CallMovieExportGetDataProc(userRoutine, refCon, params) InvokeMovieExportGetDataUPP(refCon, params, userRoutine)
#define CallMovieExportGetPropertyProc(userRoutine, refcon, trackID, propertyType, propertyValue) InvokeMovieExportGetPropertyUPP(refcon, trackID, propertyType, propertyValue, userRoutine)
EXTERN_API( ComponentResult )
MovieImportHandle               (MovieImportComponent   ci,
                                 Handle                 dataH,
                                 Movie                  theMovie,
                                 Track                  targetTrack,
                                 Track *                usedTrack,
                                 TimeValue              atTime,
                                 TimeValue *            addedDuration,
                                 long                   inFlags,
                                 long *                 outFlags)                           FIVEWORDINLINE(0x2F3C, 0x0020, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportFile                 (MovieImportComponent   ci,
                                 const FSSpec *         theFile,
                                 Movie                  theMovie,
                                 Track                  targetTrack,
                                 Track *                usedTrack,
                                 TimeValue              atTime,
                                 TimeValue *            addedDuration,
                                 long                   inFlags,
                                 long *                 outFlags)                           FIVEWORDINLINE(0x2F3C, 0x0020, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetSampleDuration    (MovieImportComponent   ci,
                                 TimeValue              duration,
                                 TimeScale              scale)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetSampleDescription (MovieImportComponent   ci,
                                 SampleDescriptionHandle  desc,
                                 OSType                 mediaType)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetMediaFile         (MovieImportComponent   ci,
                                 AliasHandle            alias)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetDimensions        (MovieImportComponent   ci,
                                 Fixed                  width,
                                 Fixed                  height)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetChunkSize         (MovieImportComponent   ci,
                                 long                   chunkSize)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetProgressProc      (MovieImportComponent   ci,
                                 MovieProgressUPP       proc,
                                 long                   refcon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0008, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetAuxiliaryData     (MovieImportComponent   ci,
                                 Handle                 data,
                                 OSType                 handleType)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0009, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetFromScrap         (MovieImportComponent   ci,
                                 Boolean                fromScrap)                          FIVEWORDINLINE(0x2F3C, 0x0002, 0x000A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportDoUserDialog         (MovieImportComponent   ci,
                                 const FSSpec *         theFile,
                                 Handle                 theData,
                                 Boolean *              canceled)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetDuration          (MovieImportComponent   ci,
                                 TimeValue              duration)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x000C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportGetAuxiliaryDataType (MovieImportComponent   ci,
                                 OSType *               auxType)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x000D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportValidate             (MovieImportComponent   ci,
                                 const FSSpec *         theFile,
                                 Handle                 theData,
                                 Boolean *              valid)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x000E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportGetFileType          (MovieImportComponent   ci,
                                 OSType *               fileType)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x000F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportDataRef              (MovieImportComponent   ci,
                                 Handle                 dataRef,
                                 OSType                 dataRefType,
                                 Movie                  theMovie,
                                 Track                  targetTrack,
                                 Track *                usedTrack,
                                 TimeValue              atTime,
                                 TimeValue *            addedDuration,
                                 long                   inFlags,
                                 long *                 outFlags)                           FIVEWORDINLINE(0x2F3C, 0x0024, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportGetSampleDescription (MovieImportComponent   ci,
                                 SampleDescriptionHandle * desc,
                                 OSType *               mediaType)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0011, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportGetMIMETypeList      (MovieImportComponent   ci,
                                 QTAtomContainer *      mimeInfo)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0012, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetOffsetAndLimit    (MovieImportComponent   ci,
                                 unsigned long          offset,
                                 unsigned long          limit)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0013, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportGetSettingsAsAtomContainer (MovieImportComponent  ci,
                                 QTAtomContainer *      settings)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0014, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetSettingsFromAtomContainer (MovieImportComponent  ci,
                                 QTAtomContainer        settings)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0015, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetOffsetAndLimit64  (MovieImportComponent   ci,
                                 const wide *           offset,
                                 const wide *           limit)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0016, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportIdle                 (MovieImportComponent   ci,
                                 long                   inFlags,
                                 long *                 outFlags)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0017, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportValidateDataRef      (MovieImportComponent   ci,
                                 Handle                 dataRef,
                                 OSType                 dataRefType,
                                 UInt8 *                valid)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0018, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportGetLoadState         (MovieImportComponent   ci,
                                 long *                 importerLoadState)                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0019, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportGetMaxLoadedTime     (MovieImportComponent   ci,
                                 TimeValue *            time)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x001A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportEstimateCompletionTime (MovieImportComponent  ci,
                                 TimeRecord *           time)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x001B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetDontBlock         (MovieImportComponent   ci,
                                 Boolean                dontBlock)                          FIVEWORDINLINE(0x2F3C, 0x0002, 0x001C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportGetDontBlock         (MovieImportComponent   ci,
                                 Boolean *              willBlock)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x001D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetIdleManager       (MovieImportComponent   ci,
                                 IdleManager            im)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x001E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportSetNewMovieFlags     (MovieImportComponent   ci,
                                 long                   newMovieFlags)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x001F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieImportGetDestinationMediaType (MovieImportComponent  ci,
                                 OSType *               mediaType)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0020, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportToHandle             (MovieExportComponent   ci,
                                 Handle                 dataH,
                                 Movie                  theMovie,
                                 Track                  onlyThisTrack,
                                 TimeValue              startTime,
                                 TimeValue              duration)                           FIVEWORDINLINE(0x2F3C, 0x0014, 0x0080, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportToFile               (MovieExportComponent   ci,
                                 const FSSpec *         theFile,
                                 Movie                  theMovie,
                                 Track                  onlyThisTrack,
                                 TimeValue              startTime,
                                 TimeValue              duration)                           FIVEWORDINLINE(0x2F3C, 0x0014, 0x0081, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportGetAuxiliaryData     (MovieExportComponent   ci,
                                 Handle                 dataH,
                                 OSType *               handleType)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0083, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportSetProgressProc      (MovieExportComponent   ci,
                                 MovieProgressUPP       proc,
                                 long                   refcon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0084, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportSetSampleDescription (MovieExportComponent   ci,
                                 SampleDescriptionHandle  desc,
                                 OSType                 mediaType)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0085, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportDoUserDialog         (MovieExportComponent   ci,
                                 Movie                  theMovie,
                                 Track                  onlyThisTrack,
                                 TimeValue              startTime,
                                 TimeValue              duration,
                                 Boolean *              canceled)                           FIVEWORDINLINE(0x2F3C, 0x0014, 0x0086, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportGetCreatorType       (MovieExportComponent   ci,
                                 OSType *               creator)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0087, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportToDataRef            (MovieExportComponent   ci,
                                 Handle                 dataRef,
                                 OSType                 dataRefType,
                                 Movie                  theMovie,
                                 Track                  onlyThisTrack,
                                 TimeValue              startTime,
                                 TimeValue              duration)                           FIVEWORDINLINE(0x2F3C, 0x0018, 0x0088, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportFromProceduresToDataRef (MovieExportComponent  ci,
                                 Handle                 dataRef,
                                 OSType                 dataRefType)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x0089, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportAddDataSource        (MovieExportComponent   ci,
                                 OSType                 trackType,
                                 TimeScale              scale,
                                 long *                 trackID,
                                 MovieExportGetPropertyUPP  getPropertyProc,
                                 MovieExportGetDataUPP  getDataProc,
                                 void *                 refCon)                             FIVEWORDINLINE(0x2F3C, 0x0018, 0x008A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportValidate             (MovieExportComponent   ci,
                                 Movie                  theMovie,
                                 Track                  onlyThisTrack,
                                 Boolean *              valid)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x008B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportGetSettingsAsAtomContainer (MovieExportComponent  ci,
                                 QTAtomContainer *      settings)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x008C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportSetSettingsFromAtomContainer (MovieExportComponent  ci,
                                 QTAtomContainer        settings)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x008D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportGetFileNameExtension (MovieExportComponent   ci,
                                 OSType *               extension)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x008E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportGetShortFileTypeString (MovieExportComponent  ci,
                                 Str255                 typeString)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x008F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportGetSourceMediaType   (MovieExportComponent   ci,
                                 OSType *               mediaType)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0090, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportSetGetMoviePropertyProc (MovieExportComponent  ci,
                                 MovieExportGetPropertyUPP  getPropertyProc,
                                 void *                 refCon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0091, 0x7000, 0xA82A);

/* Text Export Display Info data structure*/

struct TextDisplayData {
    long                            displayFlags;
    long                            textJustification;
    RGBColor                        bgColor;
    Rect                            textBox;

    short                           beginHilite;
    short                           endHilite;
    RGBColor                        hiliteColor;
    Boolean                         doHiliteColor;
    SInt8                           filler;
    TimeValue                       scrollDelayDur;
    Point                           dropShadowOffset;
    short                           dropShadowTransparency;
};
typedef struct TextDisplayData          TextDisplayData;

typedef ComponentInstance               TextExportComponent;
typedef ComponentInstance               GraphicImageMovieImportComponent;
EXTERN_API( ComponentResult )
TextExportGetDisplayData        (TextExportComponent    ci,
                                 TextDisplayData *      textDisplay)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x0100, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextExportGetTimeFraction       (TextExportComponent    ci,
                                 long *                 movieTimeFraction)                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextExportSetTimeFraction       (TextExportComponent    ci,
                                 long                   movieTimeFraction)                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextExportGetSettings           (TextExportComponent    ci,
                                 long *                 setting)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextExportSetSettings           (TextExportComponent    ci,
                                 long                   setting)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0104, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
MIDIImportGetSettings           (TextExportComponent    ci,
                                 long *                 setting)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0100, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MIDIImportSetSettings           (TextExportComponent    ci,
                                 long                   setting)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportNewGetDataAndPropertiesProcs (MovieExportComponent  ci,
                                 OSType                 trackType,
                                 TimeScale *            scale,
                                 Movie                  theMovie,
                                 Track                  theTrack,
                                 TimeValue              startTime,
                                 TimeValue              duration,
                                 MovieExportGetPropertyUPP * getPropertyProc,
                                 MovieExportGetDataUPP * getDataProc,
                                 void **                refCon)                             FIVEWORDINLINE(0x2F3C, 0x0024, 0x0100, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieExportDisposeGetDataAndPropertiesProcs (MovieExportComponent  ci,
                                 MovieExportGetPropertyUPP  getPropertyProc,
                                 MovieExportGetDataUPP  getDataProc,
                                 void *                 refCon)                             FIVEWORDINLINE(0x2F3C, 0x000C, 0x0101, 0x7000, 0xA82A);

enum {
    movieExportUseConfiguredSettings = FOUR_CHAR_CODE('ucfg'),  /* pointer to Boolean*/
    movieExportWidth            = FOUR_CHAR_CODE('wdth'),       /* pointer to Fixed*/
    movieExportHeight           = FOUR_CHAR_CODE('hegt'),       /* pointer to Fixed*/
    movieExportDuration         = FOUR_CHAR_CODE('dura'),       /* pointer to TimeRecord*/
    movieExportVideoFilter      = FOUR_CHAR_CODE('iflt'),       /* pointer to QTAtomContainer*/
    movieExportTimeScale        = FOUR_CHAR_CODE('tmsc')        /* pointer to TimeScale*/
};

EXTERN_API( ComponentResult )
GraphicsImageImportSetSequenceEnabled (GraphicImageMovieImportComponent  ci,
                                 Boolean                enable)                             FIVEWORDINLINE(0x2F3C, 0x0002, 0x0100, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
GraphicsImageImportGetSequenceEnabled (GraphicImageMovieImportComponent  ci,
                                 Boolean *              enable)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0101, 0x7000, 0xA82A);






/*----------------------------'brws' o browser prefs configuration info ------------------------*/
enum {
    kQTBrowserInfoCanUseSystemFolderPlugin = (1L << 0)          /* Mac browser can use plug-in from System "Internet Plug-ins" folder */
};




enum {
    kQTPreFlightOpenComponent   = (1L << 1)                     /* Open component as preflight check*/
};


struct ComponentPreflightFlags {
    long                            flags;
};
typedef struct ComponentPreflightFlags  ComponentPreflightFlags;





/***************

    File Preview Components

***************/

typedef ComponentInstance               pnotComponent;
enum {
    pnotComponentWantsEvents    = 1,
    pnotComponentNeedsNoCache   = 2
};

enum {
    ShowFilePreviewComponentType = FOUR_CHAR_CODE('pnot'),
    CreateFilePreviewComponentType = FOUR_CHAR_CODE('pmak')
};

EXTERN_API( ComponentResult )
PreviewShowData                 (pnotComponent          p,
                                 OSType                 dataType,
                                 Handle                 data,
                                 const Rect *           inHere)                             FIVEWORDINLINE(0x2F3C, 0x000C, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
PreviewMakePreview              (pnotComponent          p,
                                 OSType *               previewType,
                                 Handle *               previewResult,
                                 const FSSpec *         sourceFile,
                                 ICMProgressProcRecordPtr  progress)                        FIVEWORDINLINE(0x2F3C, 0x0010, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
PreviewMakePreviewReference     (pnotComponent          p,
                                 OSType *               previewType,
                                 short *                resID,
                                 const FSSpec *         sourceFile)                         FIVEWORDINLINE(0x2F3C, 0x000C, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
PreviewEvent                    (pnotComponent          p,
                                 EventRecord *          e,
                                 Boolean *              handledEvent)                       FIVEWORDINLINE(0x2F3C, 0x0008, 0x0004, 0x7000, 0xA82A);



typedef ComponentInstance               DataCompressorComponent;
typedef ComponentInstance               DataDecompressorComponent;
typedef ComponentInstance               DataCodecComponent;
enum {
    DataCompressorComponentType = FOUR_CHAR_CODE('dcom'),
    DataDecompressorComponentType = FOUR_CHAR_CODE('ddec'),
    AppleDataCompressorSubType  = FOUR_CHAR_CODE('adec'),
    zlibDataCompressorSubType   = FOUR_CHAR_CODE('zlib')
};


/** These are DataCodec procedures **/
EXTERN_API( ComponentResult )
DataCodecDecompress             (DataCodecComponent     dc,
                                 void *                 srcData,
                                 UInt32                 srcSize,
                                 void *                 dstData,
                                 UInt32                 dstBufferSize)                      FIVEWORDINLINE(0x2F3C, 0x0010, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataCodecGetCompressBufferSize  (DataCodecComponent     dc,
                                 UInt32                 srcSize,
                                 UInt32 *               dstSize)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataCodecCompress               (DataCodecComponent     dc,
                                 void *                 srcData,
                                 UInt32                 srcSize,
                                 void *                 dstData,
                                 UInt32                 dstBufferSize,
                                 UInt32 *               actualDstSize,
                                 UInt32 *               decompressSlop)                     FIVEWORDINLINE(0x2F3C, 0x0018, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataCodecBeginInterruptSafe     (DataCodecComponent     dc,
                                 unsigned long          maxSrcSize)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataCodecEndInterruptSafe       (DataCodecComponent     dc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataCodecDecompressPartial      (DataCodecComponent     dc,
                                 void **                next_in,
                                 unsigned long *        avail_in,
                                 unsigned long *        total_in,
                                 void **                next_out,
                                 unsigned long *        avail_out,
                                 unsigned long *        total_out,
                                 Boolean *              didFinish)                          FIVEWORDINLINE(0x2F3C, 0x001C, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataCodecCompressPartial        (DataCodecComponent     dc,
                                 void **                next_in,
                                 unsigned long *        avail_in,
                                 unsigned long *        total_in,
                                 void **                next_out,
                                 unsigned long *        avail_out,
                                 unsigned long *        total_out,
                                 Boolean                tryToFinish,
                                 Boolean *              didFinish)                          FIVEWORDINLINE(0x2F3C, 0x001E, 0x0007, 0x7000, 0xA82A);




typedef CALLBACK_API( void , DataHCompletionProcPtr )(Ptr request, long refcon, OSErr err);
typedef STACK_UPP_TYPE(DataHCompletionProcPtr)                  DataHCompletionUPP;

enum {
    kDataHCanRead               = 1L << 0,
    kDataHSpecialRead           = 1L << 1,
    kDataHSpecialReadFile       = 1L << 2,
    kDataHCanWrite              = 1L << 3,
    kDataHSpecialWrite          = 1 << 4,
    kDataHSpecialWriteFile      = 1 << 5,
    kDataHCanStreamingWrite     = 1 << 6,
    kDataHMustCheckDataRef      = 1 << 7
};

/* Data reference records for specific data ref types*/

struct HandleDataRefRecord {
    Handle                          dataHndl;
};
typedef struct HandleDataRefRecord      HandleDataRefRecord;

typedef HandleDataRefRecord *           HandleDataRefPtr;
typedef HandleDataRefPtr *              HandleDataRef;

struct PointerDataRefRecord {
    void *                          data;
    Size                            dataLength;
};
typedef struct PointerDataRefRecord     PointerDataRefRecord;

typedef PointerDataRefRecord *          PointerDataRefPtr;
typedef PointerDataRefPtr *             PointerDataRef;
/* Data reference extensions*/
enum {
    kDataRefExtensionChokeSpeed = FOUR_CHAR_CODE('chok'),
    kDataRefExtensionFileName   = FOUR_CHAR_CODE('fnam'),
    kDataRefExtensionMIMEType   = FOUR_CHAR_CODE('mime'),
    kDataRefExtensionMacOSFileType = FOUR_CHAR_CODE('ftyp'),
    kDataRefExtensionInitializationData = FOUR_CHAR_CODE('data'),
    kDataRefExtensionQuickTimeMediaType = FOUR_CHAR_CODE('mtyp')
};

enum {
    kDataHChokeToMovieDataRate  = 1 << 0,                       /* param is 0*/
    kDataHChokeToParam          = 1 << 1                        /* param is bytes per second*/
};


struct DataHChokeAtomRecord {
    long                            flags;                      /* one of kDataHChokeTo constants*/
    long                            param;
};
typedef struct DataHChokeAtomRecord     DataHChokeAtomRecord;


struct DataHVolumeListRecord {
    short                           vRefNum;
    long                            flags;
};
typedef struct DataHVolumeListRecord    DataHVolumeListRecord;

typedef DataHVolumeListRecord *         DataHVolumeListPtr;
typedef DataHVolumeListPtr *            DataHVolumeList;
enum {
    kDataHExtendedSchedule      = FOUR_CHAR_CODE('xtnd')
};


struct DataHScheduleRecord {
    TimeRecord                      timeNeededBy;
    long                            extendedID;                 /* always is kDataHExtendedSchedule*/
    long                            extendedVers;               /* always set to 0*/
    Fixed                           priority;                   /* 100.0 or more means must have. lower numbers...*/
};
typedef struct DataHScheduleRecord      DataHScheduleRecord;

typedef DataHScheduleRecord *           DataHSchedulePtr;
/* Flags for DataHGetInfoFlags*/
enum {
    kDataHInfoFlagNeverStreams  = 1 << 0,                       /* set if this data handler doesn't stream*/
    kDataHInfoFlagCanUpdateDataRefs = 1 << 1,                   /* set if this data handler might update data reference*/
    kDataHInfoFlagNeedsNetworkBandwidth = 1 << 2                /* set if this data handler may need to occupy the network*/
};


/* Types for DataHGetFileTypeOrdering*/
enum {
    kDataHFileTypeMacOSFileType = FOUR_CHAR_CODE('ftyp'),
    kDataHFileTypeExtension     = FOUR_CHAR_CODE('fext'),
    kDataHFileTypeMIME          = FOUR_CHAR_CODE('mime')
};

typedef OSType *                        DataHFileTypeOrderingPtr;
typedef DataHFileTypeOrderingPtr *      DataHFileTypeOrderingHandle;

EXTERN_API( ComponentResult )
DataHGetData                    (DataHandler            dh,
                                 Handle                 h,
                                 long                   hOffset,
                                 long                   offset,
                                 long                   size)                               FIVEWORDINLINE(0x2F3C, 0x0010, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHPutData                    (DataHandler            dh,
                                 Handle                 h,
                                 long                   hOffset,
                                 long *                 offset,
                                 long                   size)                               FIVEWORDINLINE(0x2F3C, 0x0010, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHFlushData                  (DataHandler            dh)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHOpenForWrite               (DataHandler            dh)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHCloseForWrite              (DataHandler            dh)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0006, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
DataHOpenForRead                (DataHandler            dh)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0008, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHCloseForRead               (DataHandler            dh)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0009, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHSetDataRef                 (DataHandler            dh,
                                 Handle                 dataRef)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x000A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetDataRef                 (DataHandler            dh,
                                 Handle *               dataRef)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHCompareDataRef             (DataHandler            dh,
                                 Handle                 dataRef,
                                 Boolean *              equal)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x000C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHTask                       (DataHandler            dh)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x000D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHScheduleData               (DataHandler            dh,
                                 Ptr                    PlaceToPutDataPtr,
                                 long                   FileOffset,
                                 long                   DataSize,
                                 long                   RefCon,
                                 DataHSchedulePtr       scheduleRec,
                                 DataHCompletionUPP     CompletionRtn)                      FIVEWORDINLINE(0x2F3C, 0x0018, 0x000E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHFinishData                 (DataHandler            dh,
                                 Ptr                    PlaceToPutDataPtr,
                                 Boolean                Cancel)                             FIVEWORDINLINE(0x2F3C, 0x0006, 0x000F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHFlushCache                 (DataHandler            dh)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHResolveDataRef             (DataHandler            dh,
                                 Handle                 theDataRef,
                                 Boolean *              wasChanged,
                                 Boolean                userInterfaceAllowed)               FIVEWORDINLINE(0x2F3C, 0x000A, 0x0011, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetFileSize                (DataHandler            dh,
                                 long *                 fileSize)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0012, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHCanUseDataRef              (DataHandler            dh,
                                 Handle                 dataRef,
                                 long *                 useFlags)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0013, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetVolumeList              (DataHandler            dh,
                                 DataHVolumeList *      volumeList)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0014, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHWrite                      (DataHandler            dh,
                                 Ptr                    data,
                                 long                   offset,
                                 long                   size,
                                 DataHCompletionUPP     completion,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x0014, 0x0015, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHPreextend                  (DataHandler            dh,
                                 unsigned long          maxToAdd,
                                 unsigned long *        spaceAdded)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0016, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHSetFileSize                (DataHandler            dh,
                                 long                   fileSize)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0017, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetFreeSpace               (DataHandler            dh,
                                 unsigned long *        freeSize)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0018, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHCreateFile                 (DataHandler            dh,
                                 OSType                 creator,
                                 Boolean                deleteExisting)                     FIVEWORDINLINE(0x2F3C, 0x0006, 0x0019, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetPreferredBlockSize      (DataHandler            dh,
                                 long *                 blockSize)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x001A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetDeviceIndex             (DataHandler            dh,
                                 long *                 deviceIndex)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x001B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHIsStreamingDataHandler     (DataHandler            dh,
                                 Boolean *              yes)                                FIVEWORDINLINE(0x2F3C, 0x0004, 0x001C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetDataInBuffer            (DataHandler            dh,
                                 long                   startOffset,
                                 long *                 size)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x001D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetScheduleAheadTime       (DataHandler            dh,
                                 long *                 millisecs)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x001E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHSetCacheSizeLimit          (DataHandler            dh,
                                 Size                   cacheSizeLimit)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x001F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetCacheSizeLimit          (DataHandler            dh,
                                 Size *                 cacheSizeLimit)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0020, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetMovie                   (DataHandler            dh,
                                 Movie *                theMovie,
                                 short *                id)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0021, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHAddMovie                   (DataHandler            dh,
                                 Movie                  theMovie,
                                 short *                id)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0022, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHUpdateMovie                (DataHandler            dh,
                                 Movie                  theMovie,
                                 short                  id)                                 FIVEWORDINLINE(0x2F3C, 0x0006, 0x0023, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHDoesBuffer                 (DataHandler            dh,
                                 Boolean *              buffersReads,
                                 Boolean *              buffersWrites)                      FIVEWORDINLINE(0x2F3C, 0x0008, 0x0024, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetFileName                (DataHandler            dh,
                                 Str255                 str)                                FIVEWORDINLINE(0x2F3C, 0x0004, 0x0025, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetAvailableFileSize       (DataHandler            dh,
                                 long *                 fileSize)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0026, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetMacOSFileType           (DataHandler            dh,
                                 OSType *               fileType)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0027, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetMIMEType                (DataHandler            dh,
                                 Str255                 mimeType)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0028, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHSetDataRefWithAnchor       (DataHandler            dh,
                                 Handle                 anchorDataRef,
                                 OSType                 dataRefType,
                                 Handle                 dataRef)                            FIVEWORDINLINE(0x2F3C, 0x000C, 0x0029, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetDataRefWithAnchor       (DataHandler            dh,
                                 Handle                 anchorDataRef,
                                 OSType                 dataRefType,
                                 Handle *               dataRef)                            FIVEWORDINLINE(0x2F3C, 0x000C, 0x002A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHSetMacOSFileType           (DataHandler            dh,
                                 OSType                 fileType)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x002B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHSetTimeBase                (DataHandler            dh,
                                 TimeBase               tb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x002C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetInfoFlags               (DataHandler            dh,
                                 UInt32 *               flags)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x002D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHScheduleData64             (DataHandler            dh,
                                 Ptr                    PlaceToPutDataPtr,
                                 const wide *           FileOffset,
                                 long                   DataSize,
                                 long                   RefCon,
                                 DataHSchedulePtr       scheduleRec,
                                 DataHCompletionUPP     CompletionRtn)                      FIVEWORDINLINE(0x2F3C, 0x0018, 0x002E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHWrite64                    (DataHandler            dh,
                                 Ptr                    data,
                                 const wide *           offset,
                                 long                   size,
                                 DataHCompletionUPP     completion,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x0014, 0x002F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetFileSize64              (DataHandler            dh,
                                 wide *                 fileSize)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0030, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHPreextend64                (DataHandler            dh,
                                 const wide *           maxToAdd,
                                 wide *                 spaceAdded)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0031, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHSetFileSize64              (DataHandler            dh,
                                 const wide *           fileSize)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0032, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetFreeSpace64             (DataHandler            dh,
                                 wide *                 freeSize)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0033, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHAppend64                   (DataHandler            dh,
                                 void *                 data,
                                 wide *                 fileOffset,
                                 unsigned long          size)                               FIVEWORDINLINE(0x2F3C, 0x000C, 0x0034, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHReadAsync                  (DataHandler            dh,
                                 void *                 dataPtr,
                                 UInt32                 dataSize,
                                 const wide *           dataOffset,
                                 DataHCompletionUPP     completion,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x0014, 0x0035, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHPollRead                   (DataHandler            dh,
                                 void *                 dataPtr,
                                 UInt32 *               dataSizeSoFar)                      FIVEWORDINLINE(0x2F3C, 0x0008, 0x0036, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetDataAvailability        (DataHandler            dh,
                                 long                   offset,
                                 long                   len,
                                 long *                 missing_offset,
                                 long *                 missing_len)                        FIVEWORDINLINE(0x2F3C, 0x0010, 0x0037, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetFileSizeAsync           (DataHandler            dh,
                                 wide *                 fileSize,
                                 DataHCompletionUPP     completionRtn,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x000C, 0x003A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetDataRefAsType           (DataHandler            dh,
                                 OSType                 requestedType,
                                 Handle *               dataRef)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x003B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHSetDataRefExtension        (DataHandler            dh,
                                 Handle                 extension,
                                 OSType                 idType)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x003C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetDataRefExtension        (DataHandler            dh,
                                 Handle *               extension,
                                 OSType                 idType)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x003D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetMovieWithFlags          (DataHandler            dh,
                                 Movie *                theMovie,
                                 short *                id,
                                 short                  flags)                              FIVEWORDINLINE(0x2F3C, 0x000A, 0x003E, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
DataHGetFileTypeOrdering        (DataHandler            dh,
                                 DataHFileTypeOrderingHandle * orderingListHandle)          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0040, 0x7000, 0xA82A);

/* flags for DataHCreateFileWithFlags*/
enum {
    kDataHCreateFileButDontCreateResFile = (1L << 0)
};

EXTERN_API( ComponentResult )
DataHCreateFileWithFlags        (DataHandler            dh,
                                 OSType                 creator,
                                 Boolean                deleteExisting,
                                 UInt32                 flags)                              FIVEWORDINLINE(0x2F3C, 0x000A, 0x0041, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetMIMETypeAsync           (DataHandler            dh,
                                 Str255                 mimeType,
                                 DataHCompletionUPP     completionRtn,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x000C, 0x0042, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetInfo                    (DataHandler            dh,
                                 OSType                 what,
                                 void *                 info)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x0043, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHSetIdleManager             (DataHandler            dh,
                                 IdleManager            im)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0044, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHDeleteFile                 (DataHandler            dh)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0045, 0x7000, 0xA82A);

enum {
    kDataHMovieUsageDoAppendMDAT = 1L << 0                      /* if set, datahandler should append wide and mdat atoms in append call*/
};

EXTERN_API( ComponentResult )
DataHSetMovieUsageFlags         (DataHandler            dh,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0046, 0x7000, 0xA82A);


enum {
    kDataHTempUseSameDirectory  = 1L << 0,                      /* temp data ref should be in same directory as current data ref (vs. in temporary directory)*/
    kDataHTempUseSameVolume     = 1L << 1,                      /* temp data ref should be on same volume as current data ref (vs. find "best" volume)*/
    kDataHTempCreateFile        = 1L << 2,                      /* create the file*/
    kDataHTempOpenFile          = 1L << 3                       /* open temporary file for write (kDataHTempCreateFile must be passed, too)*/
};

EXTERN_API( ComponentResult )
DataHUseTemporaryDataRef        (DataHandler            dh,
                                 long                   inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0047, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHGetTemporaryDataRefCapabilities (DataHandler       dh,
                                 long *                 outUnderstoodFlags)                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0048, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHRenameFile                 (DataHandler            dh,
                                 Handle                 newDataRef)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0049, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
DataHPlaybackHints              (DataHandler            dh,
                                 long                   flags,
                                 unsigned long          minFileOffset,
                                 unsigned long          maxFileOffset,
                                 long                   bytesPerSecond)                     FIVEWORDINLINE(0x2F3C, 0x0010, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
DataHPlaybackHints64            (DataHandler            dh,
                                 long                   flags,
                                 const wide *           minFileOffset,
                                 const wide *           maxFileOffset,
                                 long                   bytesPerSecond)                     FIVEWORDINLINE(0x2F3C, 0x0010, 0x010E, 0x7000, 0xA82A);

/* Symbolic constants for DataHGetDataRate*/
enum {
    kDataHGetDataRateInfiniteRate = 0x7FFFFFFF                  /* all the data arrived instantaneously*/
};

EXTERN_API( ComponentResult )
DataHGetDataRate                (DataHandler            dh,
                                 long                   flags,
                                 long *                 bytesPerSecond)                     FIVEWORDINLINE(0x2F3C, 0x0008, 0x0110, 0x7000, 0xA82A);

/* Flags for DataHSetTimeHints*/
enum {
    kDataHSetTimeHintsSkipBandwidthRequest = 1 << 0             /* set if this data handler should use the network without requesting bandwidth*/
};

EXTERN_API( ComponentResult )
DataHSetTimeHints               (DataHandler            dh,
                                 long                   flags,
                                 long                   bandwidthPriority,
                                 TimeScale              scale,
                                 TimeValue              minTime,
                                 TimeValue              maxTime)                            FIVEWORDINLINE(0x2F3C, 0x0014, 0x0111, 0x7000, 0xA82A);





/* Standard type for video digitizers */
enum {
    videoDigitizerComponentType = FOUR_CHAR_CODE('vdig'),
    vdigInterfaceRev            = 2
};

/* Input Format Standards */
enum {
    ntscIn                      = 0,                            /* current input format */
    currentIn                   = 0,                            /* ntsc input format */
    palIn                       = 1,                            /* pal input format */
    secamIn                     = 2,                            /* secam input format */
    ntscReallyIn                = 3                             /* ntsc input format */
};

/* Input Formats */
enum {
    compositeIn                 = 0,                            /* input is composite format */
    sVideoIn                    = 1,                            /* input is sVideo format */
    rgbComponentIn              = 2,                            /* input is rgb component format */
    rgbComponentSyncIn          = 3,                            /* input is rgb component format (sync on green?)*/
    yuvComponentIn              = 4,                            /* input is yuv component format */
    yuvComponentSyncIn          = 5,                            /* input is yuv component format (sync on green?) */
    tvTunerIn                   = 6,
    sdiIn                       = 7
};


/* Video Digitizer PlayThru States */
enum {
    vdPlayThruOff               = 0,
    vdPlayThruOn                = 1
};

/* Input Color Space Modes */
enum {
    vdDigitizerBW               = 0,                            /* black and white */
    vdDigitizerRGB              = 1                             /* rgb color */
};

/* Phase Lock Loop Modes */
enum {
    vdBroadcastMode             = 0,                            /* Broadcast / Laser Disk video mode */
    vdVTRMode                   = 1                             /* VCR / Magnetic media mode */
};

/* Field Select Options */
enum {
    vdUseAnyField               = 0,                            /* Digitizers choice on field use */
    vdUseOddField               = 1,                            /* Use odd field for half size vert and smaller */
    vdUseEvenField              = 2                             /* Use even field for half size vert and smaller */
};

/* vdig types */
enum {
    vdTypeBasic                 = 0,                            /* basic, no clipping */
    vdTypeAlpha                 = 1,                            /* supports clipping with alpha channel */
    vdTypeMask                  = 2,                            /* supports clipping with mask plane */
    vdTypeKey                   = 3                             /* supports clipping with key color(s) */
};



/* Digitizer Input Capability/Current Flags */
enum {
    digiInDoesNTSC              = 1L << 0,                      /* digitizer supports NTSC input format */
    digiInDoesPAL               = 1L << 1,                      /* digitizer supports PAL input format */
    digiInDoesSECAM             = 1L << 2,                      /* digitizer supports SECAM input format */
    digiInDoesGenLock           = 1L << 7,                      /* digitizer does genlock */
    digiInDoesComposite         = 1L << 8,                      /* digitizer supports composite input type */
    digiInDoesSVideo            = 1L << 9,                      /* digitizer supports S-Video input type */
    digiInDoesComponent         = 1L << 10,                     /* digitizer supports component = rgb, input type */
    digiInVTR_Broadcast         = 1L << 11,                     /* digitizer can differentiate between the two */
    digiInDoesColor             = 1L << 12,                     /* digitizer supports color */
    digiInDoesBW                = 1L << 13,                     /* digitizer supports black & white */
                                                                /* Digitizer Input Current Flags = these are valid only during active operating conditions,   */
    digiInSignalLock            = 1L << 31                      /* digitizer detects input signal is locked, this bit = horiz lock || vertical lock */
};


/* Digitizer Output Capability/Current Flags */
enum {
    digiOutDoes1                = 1L << 0,                      /* digitizer supports 1 bit pixels */
    digiOutDoes2                = 1L << 1,                      /* digitizer supports 2 bit pixels */
    digiOutDoes4                = 1L << 2,                      /* digitizer supports 4 bit pixels */
    digiOutDoes8                = 1L << 3,                      /* digitizer supports 8 bit pixels */
    digiOutDoes16               = 1L << 4,                      /* digitizer supports 16 bit pixels */
    digiOutDoes32               = 1L << 5,                      /* digitizer supports 32 bit pixels */
    digiOutDoesDither           = 1L << 6,                      /* digitizer dithers in indexed modes */
    digiOutDoesStretch          = 1L << 7,                      /* digitizer can arbitrarily stretch */
    digiOutDoesShrink           = 1L << 8,                      /* digitizer can arbitrarily shrink */
    digiOutDoesMask             = 1L << 9,                      /* digitizer can mask to clipping regions */
    digiOutDoesDouble           = 1L << 11,                     /* digitizer can stretch to exactly double size */
    digiOutDoesQuad             = 1L << 12,                     /* digitizer can stretch exactly quadruple size */
    digiOutDoesQuarter          = 1L << 13,                     /* digitizer can shrink to exactly quarter size */
    digiOutDoesSixteenth        = 1L << 14,                     /* digitizer can shrink to exactly sixteenth size */
    digiOutDoesRotate           = 1L << 15,                     /* digitizer supports rotate transformations */
    digiOutDoesHorizFlip        = 1L << 16,                     /* digitizer supports horizontal flips Sx < 0 */
    digiOutDoesVertFlip         = 1L << 17,                     /* digitizer supports vertical flips Sy < 0 */
    digiOutDoesSkew             = 1L << 18,                     /* digitizer supports skew = shear,twist, */
    digiOutDoesBlend            = 1L << 19,
    digiOutDoesWarp             = 1L << 20,
    digiOutDoesHW_DMA           = 1L << 21,                     /* digitizer not constrained to local device */
    digiOutDoesHWPlayThru       = 1L << 22,                     /* digitizer doesn't need time to play thru */
    digiOutDoesILUT             = 1L << 23,                     /* digitizer does inverse LUT for index modes */
    digiOutDoesKeyColor         = 1L << 24,                     /* digitizer does key color functions too */
    digiOutDoesAsyncGrabs       = 1L << 25,                     /* digitizer supports async grabs */
    digiOutDoesUnreadableScreenBits = 1L << 26,                 /* playthru doesn't generate readable bits on screen*/
    digiOutDoesCompress         = 1L << 27,                     /* supports alternate output data types */
    digiOutDoesCompressOnly     = 1L << 28,                     /* can't provide raw frames anywhere */
    digiOutDoesPlayThruDuringCompress = 1L << 29,               /* digi can do playthru while providing compressed data */
    digiOutDoesCompressPartiallyVisible = 1L << 30,             /* digi doesn't need all bits visible on screen to do hardware compress */
    digiOutDoesNotNeedCopyOfCompressData = 1L << 31             /* digi doesn't need any bufferization when providing compressed data */
};

/* Types */
typedef ComponentInstance               VideoDigitizerComponent;
typedef ComponentResult                 VideoDigitizerError;

struct DigitizerInfo {
    short                           vdigType;
    long                            inputCapabilityFlags;
    long                            outputCapabilityFlags;
    long                            inputCurrentFlags;
    long                            outputCurrentFlags;
    short                           slot;                       /* temporary for connection purposes */
    GDHandle                        gdh;                        /* temporary for digitizers that have preferred screen */
    GDHandle                        maskgdh;                    /* temporary for digitizers that have mask planes */
    short                           minDestHeight;              /* Smallest resizable height */
    short                           minDestWidth;               /* Smallest resizable width */
    short                           maxDestHeight;              /* Largest resizable height */
    short                           maxDestWidth;               /* Largest resizable width */
    short                           blendLevels;                /* Number of blend levels supported (2 if 1 bit mask) */
    long                            reserved;                   /* reserved */
};
typedef struct DigitizerInfo            DigitizerInfo;

struct VdigType {
    long                            digType;
    long                            reserved;
};
typedef struct VdigType                 VdigType;

struct VdigTypeList {
    short                           count;
    VdigType                        list[1];
};
typedef struct VdigTypeList             VdigTypeList;

struct VdigBufferRec {
    PixMapHandle                    dest;
    Point                           location;
    long                            reserved;
};
typedef struct VdigBufferRec            VdigBufferRec;

struct VdigBufferRecList {
    short                           count;
    MatrixRecordPtr                 matrix;
    RgnHandle                       mask;
    VdigBufferRec                   list[1];
};
typedef struct VdigBufferRecList        VdigBufferRecList;
typedef VdigBufferRecList *             VdigBufferRecListPtr;
typedef VdigBufferRecListPtr *          VdigBufferRecListHandle;
typedef CALLBACK_API( void , VdigIntProcPtr )(long flags, long refcon);
typedef STACK_UPP_TYPE(VdigIntProcPtr)                          VdigIntUPP;

struct VDCompressionList {
    CodecComponent                  codec;
    CodecType                       cType;
    Str63                           typeName;
    Str63                           name;
    long                            formatFlags;
    long                            compressFlags;
    long                            reserved;
};
typedef struct VDCompressionList        VDCompressionList;
typedef VDCompressionList *             VDCompressionListPtr;
typedef VDCompressionListPtr *          VDCompressionListHandle;
enum {
    dmaDepth1                   = 1,
    dmaDepth2                   = 2,
    dmaDepth4                   = 4,
    dmaDepth8                   = 8,
    dmaDepth16                  = 16,
    dmaDepth32                  = 32,
    dmaDepth2Gray               = 64,
    dmaDepth4Gray               = 128,
    dmaDepth8Gray               = 256
};

enum {
    kVDIGControlledFrameRate    = -1
};


EXTERN_API( VideoDigitizerError )
VDGetMaxSrcRect                 (VideoDigitizerComponent  ci,
                                 short                  inputStd,
                                 Rect *                 maxSrcRect)                         FIVEWORDINLINE(0x2F3C, 0x0006, 0x0001, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetActiveSrcRect              (VideoDigitizerComponent  ci,
                                 short                  inputStd,
                                 Rect *                 activeSrcRect)                      FIVEWORDINLINE(0x2F3C, 0x0006, 0x0002, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetDigitizerRect              (VideoDigitizerComponent  ci,
                                 Rect *                 digitizerRect)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x0003, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetDigitizerRect              (VideoDigitizerComponent  ci,
                                 Rect *                 digitizerRect)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x0004, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetVBlankRect                 (VideoDigitizerComponent  ci,
                                 short                  inputStd,
                                 Rect *                 vBlankRect)                         FIVEWORDINLINE(0x2F3C, 0x0006, 0x0005, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetMaskPixMap                 (VideoDigitizerComponent  ci,
                                 PixMapHandle           maskPixMap)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0006, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetPlayThruDestination        (VideoDigitizerComponent  ci,
                                 PixMapHandle *         dest,
                                 Rect *                 destRect,
                                 MatrixRecord *         m,
                                 RgnHandle *            mask)                               FIVEWORDINLINE(0x2F3C, 0x0010, 0x0008, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDUseThisCLUT                   (VideoDigitizerComponent  ci,
                                 CTabHandle             colorTableHandle)                   FIVEWORDINLINE(0x2F3C, 0x0004, 0x0009, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetInputGammaValue            (VideoDigitizerComponent  ci,
                                 Fixed                  channel1,
                                 Fixed                  channel2,
                                 Fixed                  channel3)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x000A, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetInputGammaValue            (VideoDigitizerComponent  ci,
                                 Fixed *                channel1,
                                 Fixed *                channel2,
                                 Fixed *                channel3)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x000B, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetBrightness                 (VideoDigitizerComponent  ci,
                                 unsigned short *       brightness)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x000C, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetBrightness                 (VideoDigitizerComponent  ci,
                                 unsigned short *       brightness)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x000D, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetContrast                   (VideoDigitizerComponent  ci,
                                 unsigned short *       contrast)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x000E, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetHue                        (VideoDigitizerComponent  ci,
                                 unsigned short *       hue)                                FIVEWORDINLINE(0x2F3C, 0x0004, 0x000F, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetSharpness                  (VideoDigitizerComponent  ci,
                                 unsigned short *       sharpness)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0010, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetSaturation                 (VideoDigitizerComponent  ci,
                                 unsigned short *       saturation)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0011, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetContrast                   (VideoDigitizerComponent  ci,
                                 unsigned short *       contrast)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0012, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetHue                        (VideoDigitizerComponent  ci,
                                 unsigned short *       hue)                                FIVEWORDINLINE(0x2F3C, 0x0004, 0x0013, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetSharpness                  (VideoDigitizerComponent  ci,
                                 unsigned short *       sharpness)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0014, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetSaturation                 (VideoDigitizerComponent  ci,
                                 unsigned short *       saturation)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0015, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGrabOneFrame                  (VideoDigitizerComponent  ci)                               FIVEWORDINLINE(0x2F3C, 0x0000, 0x0016, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetMaxAuxBuffer               (VideoDigitizerComponent  ci,
                                 PixMapHandle *         pm,
                                 Rect *                 r)                                  FIVEWORDINLINE(0x2F3C, 0x0008, 0x0017, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetDigitizerInfo              (VideoDigitizerComponent  ci,
                                 DigitizerInfo *        info)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0019, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetCurrentFlags               (VideoDigitizerComponent  ci,
                                 long *                 inputCurrentFlag,
                                 long *                 outputCurrentFlag)                  FIVEWORDINLINE(0x2F3C, 0x0008, 0x001A, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetKeyColor                   (VideoDigitizerComponent  ci,
                                 long                   index)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x001B, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetKeyColor                   (VideoDigitizerComponent  ci,
                                 long *                 index)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x001C, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDAddKeyColor                   (VideoDigitizerComponent  ci,
                                 long *                 index)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x001D, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetNextKeyColor               (VideoDigitizerComponent  ci,
                                 long                   index)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x001E, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetKeyColorRange              (VideoDigitizerComponent  ci,
                                 RGBColor *             minRGB,
                                 RGBColor *             maxRGB)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x001F, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetKeyColorRange              (VideoDigitizerComponent  ci,
                                 RGBColor *             minRGB,
                                 RGBColor *             maxRGB)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0020, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetDigitizerUserInterrupt     (VideoDigitizerComponent  ci,
                                 long                   flags,
                                 VdigIntUPP             userInterruptProc,
                                 long                   refcon)                             FIVEWORDINLINE(0x2F3C, 0x000C, 0x0021, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetInputColorSpaceMode        (VideoDigitizerComponent  ci,
                                 short                  colorSpaceMode)                     FIVEWORDINLINE(0x2F3C, 0x0002, 0x0022, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetInputColorSpaceMode        (VideoDigitizerComponent  ci,
                                 short *                colorSpaceMode)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0023, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetClipState                  (VideoDigitizerComponent  ci,
                                 short                  clipEnable)                         FIVEWORDINLINE(0x2F3C, 0x0002, 0x0024, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetClipState                  (VideoDigitizerComponent  ci,
                                 short *                clipEnable)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0025, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetClipRgn                    (VideoDigitizerComponent  ci,
                                 RgnHandle              clipRegion)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0026, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDClearClipRgn                  (VideoDigitizerComponent  ci,
                                 RgnHandle              clipRegion)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0027, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetCLUTInUse                  (VideoDigitizerComponent  ci,
                                 CTabHandle *           colorTableHandle)                   FIVEWORDINLINE(0x2F3C, 0x0004, 0x0028, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetPLLFilterType              (VideoDigitizerComponent  ci,
                                 short                  pllType)                            FIVEWORDINLINE(0x2F3C, 0x0002, 0x0029, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetPLLFilterType              (VideoDigitizerComponent  ci,
                                 short *                pllType)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x002A, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetMaskandValue               (VideoDigitizerComponent  ci,
                                 unsigned short         blendLevel,
                                 long *                 mask,
                                 long *                 value)                              FIVEWORDINLINE(0x2F3C, 0x000A, 0x002B, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetMasterBlendLevel           (VideoDigitizerComponent  ci,
                                 unsigned short *       blendLevel)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x002C, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetPlayThruDestination        (VideoDigitizerComponent  ci,
                                 PixMapHandle           dest,
                                 RectPtr                destRect,
                                 MatrixRecordPtr        m,
                                 RgnHandle              mask)                               FIVEWORDINLINE(0x2F3C, 0x0010, 0x002D, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetPlayThruOnOff              (VideoDigitizerComponent  ci,
                                 short                  state)                              FIVEWORDINLINE(0x2F3C, 0x0002, 0x002E, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetFieldPreference            (VideoDigitizerComponent  ci,
                                 short                  fieldFlag)                          FIVEWORDINLINE(0x2F3C, 0x0002, 0x002F, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetFieldPreference            (VideoDigitizerComponent  ci,
                                 short *                fieldFlag)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0030, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDPreflightDestination          (VideoDigitizerComponent  ci,
                                 Rect *                 digitizerRect,
                                 PixMap **              dest,
                                 RectPtr                destRect,
                                 MatrixRecordPtr        m)                                  FIVEWORDINLINE(0x2F3C, 0x0010, 0x0032, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDPreflightGlobalRect           (VideoDigitizerComponent  ci,
                                 GrafPtr                theWindow,
                                 Rect *                 globalRect)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0033, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetPlayThruGlobalRect         (VideoDigitizerComponent  ci,
                                 GrafPtr                theWindow,
                                 Rect *                 globalRect)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0034, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetInputGammaRecord           (VideoDigitizerComponent  ci,
                                 VDGamRecPtr            inputGammaPtr)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x0035, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetInputGammaRecord           (VideoDigitizerComponent  ci,
                                 VDGamRecPtr *          inputGammaPtr)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x0036, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetBlackLevelValue            (VideoDigitizerComponent  ci,
                                 unsigned short *       blackLevel)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0037, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetBlackLevelValue            (VideoDigitizerComponent  ci,
                                 unsigned short *       blackLevel)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0038, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetWhiteLevelValue            (VideoDigitizerComponent  ci,
                                 unsigned short *       whiteLevel)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0039, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetWhiteLevelValue            (VideoDigitizerComponent  ci,
                                 unsigned short *       whiteLevel)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x003A, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetVideoDefaults              (VideoDigitizerComponent  ci,
                                 unsigned short *       blackLevel,
                                 unsigned short *       whiteLevel,
                                 unsigned short *       brightness,
                                 unsigned short *       hue,
                                 unsigned short *       saturation,
                                 unsigned short *       contrast,
                                 unsigned short *       sharpness)                          FIVEWORDINLINE(0x2F3C, 0x001C, 0x003B, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetNumberOfInputs             (VideoDigitizerComponent  ci,
                                 short *                inputs)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x003C, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetInputFormat                (VideoDigitizerComponent  ci,
                                 short                  input,
                                 short *                format)                             FIVEWORDINLINE(0x2F3C, 0x0006, 0x003D, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetInput                      (VideoDigitizerComponent  ci,
                                 short                  input)                              FIVEWORDINLINE(0x2F3C, 0x0002, 0x003E, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetInput                      (VideoDigitizerComponent  ci,
                                 short *                input)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x003F, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetInputStandard              (VideoDigitizerComponent  ci,
                                 short                  inputStandard)                      FIVEWORDINLINE(0x2F3C, 0x0002, 0x0040, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetupBuffers                  (VideoDigitizerComponent  ci,
                                 VdigBufferRecListHandle  bufferList)                       FIVEWORDINLINE(0x2F3C, 0x0004, 0x0041, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGrabOneFrameAsync             (VideoDigitizerComponent  ci,
                                 short                  buffer)                             FIVEWORDINLINE(0x2F3C, 0x0002, 0x0042, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDDone                          (VideoDigitizerComponent  ci,
                                 short                  buffer)                             FIVEWORDINLINE(0x2F3C, 0x0002, 0x0043, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetCompression                (VideoDigitizerComponent  ci,
                                 OSType                 compressType,
                                 short                  depth,
                                 Rect *                 bounds,
                                 CodecQ                 spatialQuality,
                                 CodecQ                 temporalQuality,
                                 long                   keyFrameRate)                       FIVEWORDINLINE(0x2F3C, 0x0016, 0x0044, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDCompressOneFrameAsync         (VideoDigitizerComponent  ci)                               FIVEWORDINLINE(0x2F3C, 0x0000, 0x0045, 0x7000, 0xA82A);

/*    Note that UInt8* queuedFrameCount replaces Boolean* done. 0(==false) still means no frames, and 1(==true) one, 
    but if more than one are available the number should be returned here. The value 2 previously meant more than one frame,
    so some VDIGs may return 2 even if more than 2 are available, and some will still return 1 as they are using the original definition */
EXTERN_API( VideoDigitizerError )
VDCompressDone                  (VideoDigitizerComponent  ci,
                                 UInt8 *                queuedFrameCount,
                                 Ptr *                  theData,
                                 long *                 dataSize,
                                 UInt8 *                similarity,
                                 TimeRecord *           t)                                  FIVEWORDINLINE(0x2F3C, 0x0014, 0x0046, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDReleaseCompressBuffer         (VideoDigitizerComponent  ci,
                                 Ptr                    bufferAddr)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0047, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetImageDescription           (VideoDigitizerComponent  ci,
                                 ImageDescriptionHandle  desc)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0048, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDResetCompressSequence         (VideoDigitizerComponent  ci)                               FIVEWORDINLINE(0x2F3C, 0x0000, 0x0049, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetCompressionOnOff           (VideoDigitizerComponent  ci,
                                 Boolean                state)                              FIVEWORDINLINE(0x2F3C, 0x0002, 0x004A, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetCompressionTypes           (VideoDigitizerComponent  ci,
                                 VDCompressionListHandle  h)                                FIVEWORDINLINE(0x2F3C, 0x0004, 0x004B, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetTimeBase                   (VideoDigitizerComponent  ci,
                                 TimeBase               t)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x004C, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetFrameRate                  (VideoDigitizerComponent  ci,
                                 Fixed                  framesPerSecond)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x004D, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetDataRate                   (VideoDigitizerComponent  ci,
                                 long *                 milliSecPerFrame,
                                 Fixed *                framesPerSecond,
                                 long *                 bytesPerSecond)                     FIVEWORDINLINE(0x2F3C, 0x000C, 0x004E, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetSoundInputDriver           (VideoDigitizerComponent  ci,
                                 Str255                 soundDriverName)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x004F, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetDMADepths                  (VideoDigitizerComponent  ci,
                                 long *                 depthArray,
                                 long *                 preferredDepth)                     FIVEWORDINLINE(0x2F3C, 0x0008, 0x0050, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetPreferredTimeScale         (VideoDigitizerComponent  ci,
                                 TimeScale *            preferred)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0051, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDReleaseAsyncBuffers           (VideoDigitizerComponent  ci)                               FIVEWORDINLINE(0x2F3C, 0x0000, 0x0052, 0x7000, 0xA82A);

/* 83 is reserved for compatibility reasons */
EXTERN_API( VideoDigitizerError )
VDSetDataRate                   (VideoDigitizerComponent  ci,
                                 long                   bytesPerSecond)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0054, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetTimeCode                   (VideoDigitizerComponent  ci,
                                 TimeRecord *           atTime,
                                 void *                 timeCodeFormat,
                                 void *                 timeCodeTime)                       FIVEWORDINLINE(0x2F3C, 0x000C, 0x0055, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDUseSafeBuffers                (VideoDigitizerComponent  ci,
                                 Boolean                useSafeBuffers)                     FIVEWORDINLINE(0x2F3C, 0x0002, 0x0056, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetSoundInputSource           (VideoDigitizerComponent  ci,
                                 long                   videoInput,
                                 long *                 soundInput)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0057, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetCompressionTime            (VideoDigitizerComponent  ci,
                                 OSType                 compressionType,
                                 short                  depth,
                                 Rect *                 srcRect,
                                 CodecQ *               spatialQuality,
                                 CodecQ *               temporalQuality,
                                 unsigned long *        compressTime)                       FIVEWORDINLINE(0x2F3C, 0x0016, 0x0058, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetPreferredPacketSize        (VideoDigitizerComponent  ci,
                                 long                   preferredPacketSizeInBytes)         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0059, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetPreferredImageDimensions   (VideoDigitizerComponent  ci,
                                 long                   width,
                                 long                   height)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x005A, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetPreferredImageDimensions   (VideoDigitizerComponent  ci,
                                 long *                 width,
                                 long *                 height)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x005B, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDGetInputName                  (VideoDigitizerComponent  ci,
                                 long                   videoInput,
                                 Str255                 name)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x005C, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDSetDestinationPort            (VideoDigitizerComponent  ci,
                                 CGrafPtr               destPort)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x005D, 0x7000, 0xA82A);

/*
   The following call is designed to give the VDIG a little more control over how it is presented to the User, to clarify the 
   distinction between Devices and Inputs. Historically, the assumption has been that there is one component registered per device
   and the component name is displayed. This change lets a component choose its name after registration.
   vdDeviceFlagShowInputsAsDevices is meant for components that register once and support multiple devices 
   The UI is clearer if these are presented as device rather than inputs, 
   and this allows a VDIG to present itself this way without huge restructuring
   vdDeviceFlagHideDevice is for the kind of VDIG that registers itself, and then can register a further VDIG for each device. 
   If no hardware is available, returning this flag will omit it from the list. 
   This call being made is also a good time to check for hardware and register further VDIG components if needed, 
   allowing for lazy initialization when the Application needs to find a VDIG rather than on every launch or replug.
*/

enum {
    vdDeviceFlagShowInputsAsDevices = (1 << 0),                 /* Tell the Panel to promote Inputs to Devices*/
    vdDeviceFlagHideDevice      = (1 << 1)                      /* Omit this Device entirely from the list*/
};

EXTERN_API( VideoDigitizerError )
VDGetDeviceNameAndFlags         (VideoDigitizerComponent  ci,
                                 Str255                 outName,
                                 UInt32 *               outNameFlags)                       FIVEWORDINLINE(0x2F3C, 0x0008, 0x005E, 0x7000, 0xA82A);

enum {
    vdFlagCaptureStarting       = (1 << 0),                     /* Capture is about to start; allocate bandwidth */
    vdFlagCaptureStopping       = (1 << 1),                     /* Capture is about to stop; stop queuing frames*/
    vdFlagCaptureIsForPreview   = (1 << 2),                     /* Capture is just to screen for preview purposes*/
    vdFlagCaptureIsForRecord    = (1 << 3),                     /* Capture is going to be recorded*/
    vdFlagCaptureLowLatency     = (1 << 4),                     /* Fresh frames are more important than delivering every frame - don't queue too much*/
    vdFlagCaptureAlwaysUseTimeBase = (1 << 5),                  /* Use the timebase for every frame; don't worry about making durations uniform*/
    vdFlagCaptureSetSettingsBegin = (1 << 6),                   /* A series of calls are about to be made to restore settings.*/
    vdFlagCaptureSetSettingsEnd = (1 << 7)                      /* Finished restoring settings; any set calls after this are from the app or UI*/
};

EXTERN_API( VideoDigitizerError )
VDCaptureStateChanging          (VideoDigitizerComponent  ci,
                                 UInt32                 inStateFlags)                       FIVEWORDINLINE(0x2F3C, 0x0004, 0x005F, 0x7000, 0xA82A);

/*
   These UniqueID calls are so that the VDIG can give the SG information enabling it to restore a particular
   configuration - choose a particular device and input from those available.
   For example, restoring the specific camera for a set of several hot-plugged FireWire cameras 
   the caller can pass nil if it is not interested in one of the IDs
   returning 0 in an ID means you don't have one
*/

EXTERN_API( VideoDigitizerError )
VDGetUniqueIDs                  (VideoDigitizerComponent  ci,
                                 UInt64 *               outDeviceID,
                                 UInt64 *               outInputID)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0060, 0x7000, 0xA82A);

/*
   Note this is a 'Select' not a 'Set' - the assumption is that the Unique ID is a function of the hardware
   and not modifiable by the calling application. Either a nil pointer or 0 an the ID means don't care.
   return vdDontHaveThatUniqueIDErr if your device doesn't have a match.
*/

EXTERN_API( VideoDigitizerError )
VDSelectUniqueIDs               (VideoDigitizerComponent  ci,
                                 const UInt64 *         inDeviceID,
                                 const UInt64 *         inInputID)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0061, 0x7000, 0xA82A);


/*
   IIDC (Instrumentation & Industrial Digital Camera) Video Digitizers
   Video Digitizers of subtype vdSubtypeIIDC support FireWire cameras which conform to the
   "IIDC 1394-based Digital Camera Specification." 
*/

enum {
    vdSubtypeIIDC               = FOUR_CHAR_CODE('iidc')        /* Subtype for IIDC 1394-Digital Camera video digitizer*/
};

/*
   vdIIDCAtomTypeFeature
   Parent node for the QTAtoms which describe a given feature.  
*/
enum {
    vdIIDCAtomTypeFeature       = FOUR_CHAR_CODE('feat')
};

/*
   vdIIDCAtomTypeFeatureAtomTypeAndID
   This atom describes the feature's OSType/group/name and QTAtomType & QTAtomID needed to retrieve its settings.
   The contents of this atom is a VDIIDCFeatureAtomTypeAndID structure.  
*/
enum {
    vdIIDCAtomTypeFeatureAtomTypeAndID = FOUR_CHAR_CODE('t&id'),
    vdIIDCAtomIDFeatureAtomTypeAndID = 1
};


struct VDIIDCFeatureAtomTypeAndID {
    OSType                          feature;                    /* OSType of feature*/
    OSType                          group;                      /* OSType of group that feature is categorized into*/
    Str255                          name;                       /* Name of this feature*/
    QTAtomType                      atomType;                   /* Atom type which contains feature's settings*/
    QTAtomID                        atomID;                     /* Atom ID which contains feature's settings*/
};
typedef struct VDIIDCFeatureAtomTypeAndID VDIIDCFeatureAtomTypeAndID;
/* IIDC Feature OSTypes*/
enum {
    vdIIDCFeatureHue            = FOUR_CHAR_CODE('hue '),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureSaturation     = FOUR_CHAR_CODE('satu'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureContrast       = FOUR_CHAR_CODE('ctst'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureSharpness      = FOUR_CHAR_CODE('shrp'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureBlackLevel     = FOUR_CHAR_CODE('blvl'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureGain           = FOUR_CHAR_CODE('gain'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureIris           = FOUR_CHAR_CODE('iris'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureShutter        = FOUR_CHAR_CODE('shtr'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureExposure       = FOUR_CHAR_CODE('xpsr'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureWhiteBalanceU  = FOUR_CHAR_CODE('whbu'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureWhiteBalanceV  = FOUR_CHAR_CODE('whbv'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureGamma          = FOUR_CHAR_CODE('gmma'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureTemperature    = FOUR_CHAR_CODE('temp'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureZoom           = FOUR_CHAR_CODE('zoom'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureFocus          = FOUR_CHAR_CODE('fcus'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeaturePan            = FOUR_CHAR_CODE('pan '),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureTilt           = FOUR_CHAR_CODE('tilt'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureOpticalFilter  = FOUR_CHAR_CODE('opft'),       /* Feature's settings handled by VDIIDCFeatureSettings*/
    vdIIDCFeatureTrigger        = FOUR_CHAR_CODE('trgr'),       /* Trigger's setttings handled by VDIIDCTriggerSettings*/
    vdIIDCFeatureCaptureSize    = FOUR_CHAR_CODE('cpsz'),       /* Feature's settings is not defined*/
    vdIIDCFeatureCaptureQuality = FOUR_CHAR_CODE('cpql')        /* Feature's settings is not defined*/
};

/* IIDC Group OSTypes that features are categorized into*/
enum {
    vdIIDCGroupImage            = FOUR_CHAR_CODE('imag'),       /* Feature related to camera's image*/
    vdIIDCGroupColor            = FOUR_CHAR_CODE('colr'),       /* Feature related to camera's color control*/
    vdIIDCGroupMechanics        = FOUR_CHAR_CODE('mech'),       /* Feature related to camera's mechanics*/
    vdIIDCGroupTrigger          = FOUR_CHAR_CODE('trig')        /* Feature related to camera's trigger*/
};

/*
   vdIIDCAtomTypeFeatureSettings
   This atom describes the settings for the majority of features.
   The contents of this atom is a VDIIDCFeatureSettings structure.
*/
enum {
    vdIIDCAtomTypeFeatureSettings = FOUR_CHAR_CODE('fstg'),
    vdIIDCAtomIDFeatureSettings = 1
};


struct VDIIDCFeatureCapabilities {
    UInt32                          flags;
    UInt16                          rawMinimum;
    UInt16                          rawMaximum;
    QTFloatSingle                   absoluteMinimum;
    QTFloatSingle                   absoluteMaximum;
};
typedef struct VDIIDCFeatureCapabilities VDIIDCFeatureCapabilities;

struct VDIIDCFeatureState {
    UInt32                          flags;
    QTFloatSingle                   value;
};
typedef struct VDIIDCFeatureState       VDIIDCFeatureState;

struct VDIIDCFeatureSettings {
    VDIIDCFeatureCapabilities       capabilities;
    VDIIDCFeatureState              state;
};
typedef struct VDIIDCFeatureSettings    VDIIDCFeatureSettings;
/*
   Flags for use in VDIIDCFeatureCapabilities.flags & VDIIDCFeatureState.flags
   When indicating capabilities, the flag being set indicates that the feature can be put into the given state.
   When indicating/setting state, the flag represents the current/desired state.
   Note that certain combinations of flags are valid for cababilities (i.e. vdIIDCFeatureFlagOn | vdIIDCFeatureFlagOff)
   but are mutally exclusive for state.
*/
enum {
    vdIIDCFeatureFlagOn         = (1 << 0),
    vdIIDCFeatureFlagOff        = (1 << 1),
    vdIIDCFeatureFlagManual     = (1 << 2),
    vdIIDCFeatureFlagAuto       = (1 << 3),
    vdIIDCFeatureFlagTune       = (1 << 4),
    vdIIDCFeatureFlagRawControl = (1 << 5),
    vdIIDCFeatureFlagAbsoluteControl = (1 << 6)
};

/*
   vdIIDCAtomTypeTriggerSettings
   This atom describes the settings for the trigger feature.
   The contents of this atom is a VDIIDCTriggerSettings structure.
*/
enum {
    vdIIDCAtomTypeTriggerSettings = FOUR_CHAR_CODE('tstg'),
    vdIIDCAtomIDTriggerSettings = 1
};


struct VDIIDCTriggerCapabilities {
    UInt32                          flags;
    QTFloatSingle                   absoluteMinimum;
    QTFloatSingle                   absoluteMaximum;
};
typedef struct VDIIDCTriggerCapabilities VDIIDCTriggerCapabilities;

struct VDIIDCTriggerState {
    UInt32                          flags;
    UInt16                          mode2TransitionCount;
    UInt16                          mode3FrameRateMultiplier;
    QTFloatSingle                   absoluteValue;
};
typedef struct VDIIDCTriggerState       VDIIDCTriggerState;

struct VDIIDCTriggerSettings {
    VDIIDCTriggerCapabilities       capabilities;
    VDIIDCTriggerState              state;
};
typedef struct VDIIDCTriggerSettings    VDIIDCTriggerSettings;
/*
   Flags for use in VDIIDCTriggerCapabilities.flags & VDIIDCTriggerState.flags
   When indicating capabilities, the flag being set indicates that the trigger can be put into the given state.
   When indicating/setting state, the flag represents the current/desired state.
   Note that certain combinations of flags are valid for cababilities (i.e. vdIIDCTriggerFlagOn | vdIIDCTriggerFlagOff)
   but are mutally exclusive for state.
*/
enum {
    vdIIDCTriggerFlagOn         = (1 << 0),
    vdIIDCTriggerFlagOff        = (1 << 1),
    vdIIDCTriggerFlagActiveHigh = (1 << 2),
    vdIIDCTriggerFlagActiveLow  = (1 << 3),
    vdIIDCTriggerFlagMode0      = (1 << 4),
    vdIIDCTriggerFlagMode1      = (1 << 5),
    vdIIDCTriggerFlagMode2      = (1 << 6),
    vdIIDCTriggerFlagMode3      = (1 << 7),
    vdIIDCTriggerFlagRawControl = (1 << 8),
    vdIIDCTriggerFlagAbsoluteControl = (1 << 9)
};

/*
   VDIIDC calls are additional calls for IIDC digitizers (vdSubtypeIIDC)
   These calls are only valid for video digitizers of subtype vdSubtypeIIDC.
*/
EXTERN_API( VideoDigitizerError )
VDIIDCGetFeatures               (VideoDigitizerComponent  ci,
                                 QTAtomContainer *      container)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0200, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDIIDCSetFeatures               (VideoDigitizerComponent  ci,
                                 QTAtomContainer        container)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0201, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDIIDCGetDefaultFeatures        (VideoDigitizerComponent  ci,
                                 QTAtomContainer *      container)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0202, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDIIDCGetCSRData                (VideoDigitizerComponent  ci,
                                 Boolean                offsetFromRegisterBase,
                                 UInt32                 offset,
                                 UInt32 *               data)                               FIVEWORDINLINE(0x2F3C, 0x000A, 0x0203, 0x7000, 0xA82A);

EXTERN_API( VideoDigitizerError )
VDIIDCSetCSRData                (VideoDigitizerComponent  ci,
                                 Boolean                offsetFromRegisterBase,
                                 UInt32                 offset,
                                 UInt32                 data)                               FIVEWORDINLINE(0x2F3C, 0x000A, 0x0204, 0x7000, 0xA82A);



enum {
    xmlParseComponentType       = FOUR_CHAR_CODE('pars'),
    xmlParseComponentSubType    = FOUR_CHAR_CODE('xml ')
};

enum {
    xmlIdentifierInvalid        = 0,
    xmlIdentifierUnrecognized   = (long)0xFFFFFFFF,
    xmlContentTypeInvalid       = 0,
    xmlContentTypeElement       = 1,
    xmlContentTypeCharData      = 2
};

enum {
    elementFlagAlwaysSelfContained = 1L << 0,                   /*    Element doesn't have contents or closing tag even if it doesn't end with />, as in the HTML <img> tag*/
    elementFlagPreserveWhiteSpace = 1L << 1,                    /*  Preserve whitespace in content, default is to remove it */
    xmlParseFlagAllowUppercase  = 1L << 0,                      /*    Entities and attributes do not have to be lowercase (strict XML), but can be upper or mixed case as in HTML*/
    xmlParseFlagAllowUnquotedAttributeValues = 1L << 1,         /*    Attributes values do not have to be enclosed in quotes (strict XML), but can be left unquoted if they contain no spaces*/
    xmlParseFlagEventParseOnly  = 1L << 2                       /*    Do event parsing only*/
};

enum {
    attributeValueKindCharString = 0,
    attributeValueKindInteger   = 1L << 0,                      /*    Number*/
    attributeValueKindPercent   = 1L << 1,                      /*    Number or percent*/
    attributeValueKindBoolean   = 1L << 2,                      /*    "true" or "false"*/
    attributeValueKindOnOff     = 1L << 3,                      /*    "on" or "off"*/
    attributeValueKindColor     = 1L << 4,                      /*    Either "#rrggbb" or a color name*/
    attributeValueKindEnum      = 1L << 5,                      /*    one of a number of strings; the enum strings are passed as a zero-separated, double-zero-terminated C string in the attributeKindValueInfo param*/
    attributeValueKindCaseSensEnum = 1L << 6,                   /*    one of a number of strings; the enum strings are passed as for attributeValueKindEnum, but the values are case-sensitive*/
    MAX_ATTRIBUTE_VALUE_KIND    = attributeValueKindCaseSensEnum
};

enum {
    nameSpaceIDNone             = 0
};

/*  A Parsed XML attribute value, one of number/percent, boolean/on-off, color, or enumerated type*/

union XMLAttributeValue {
    SInt32                          number;                     /*    The value when valueKind is attributeValueKindInteger or attributeValueKindPercent*/
    Boolean                         boolean;                    /*    The value when valueKind is attributeValueKindBoolean or attributeValueKindOnOff*/
    RGBColor                        color;                      /*    The value when valueKind is attributeValueKindColor*/
    UInt32                          enumType;                   /*    The value when valueKind is attributeValueKindEnum*/
};
typedef union XMLAttributeValue         XMLAttributeValue;
/*  An XML attribute-value pair*/

struct XMLAttribute {
    UInt32                          identifier;                 /*    Tokenized identifier, if the attribute name was recognized by the parser*/
    char *                          name;                       /*    Attribute name, Only present if identifier == xmlIdentifierUnrecognized*/
    long                            valueKind;                  /*    Type of parsed value, if the value was recognized and parsed; otherwise, attributeValueKindCharString*/
    XMLAttributeValue               value;                      /*    Parsed attribute value*/
    char *                          valueStr;                   /*    Always present*/
};
typedef struct XMLAttribute             XMLAttribute;
typedef XMLAttribute *                  XMLAttributePtr;
/*  Forward struct declarations for recursively-defined tree structure*/
typedef struct XMLContent               XMLContent;

typedef XMLContent *                    XMLContentPtr;
/*
    An XML Element, i.e.
        <element attr="value" attr="value" ...> [contents] </element>
    or
        <element attr="value" attr="value" .../>
*/

struct XMLElement {
    UInt32                          identifier;                 /*    Tokenized identifier, if the element name was recognized by the parser*/
    char *                          name;                       /*    Element name, only present if identifier == xmlIdentifierUnrecognized*/
    XMLAttributePtr                 attributes;                 /*    Array of attributes, terminated with an attribute with identifier == xmlIdentifierInvalid*/
    XMLContentPtr                   contents;                   /*    Array of contents, terminated with a content with kind == xmlIdentifierInvalid*/
};
typedef struct XMLElement               XMLElement;
typedef XMLElement *                    XMLElementPtr;
/*
    The content of an XML element is a series of parts, each of which may be either another element
    or simply character data.
*/

union XMLElementContent {
    XMLElement                      element;                    /*    The contents when the content kind is xmlContentTypeElement*/
    char *                          charData;                   /*    The contents when the content kind is xmlContentTypeCharData*/
};
typedef union XMLElementContent         XMLElementContent;

struct XMLContent {
    UInt32                          kind;
    XMLElementContent               actualContent;
};


struct XMLDocRecord {
    void *                          xmlDataStorage;             /*    opaque storage*/
    XMLElement                      rootElement;
};
typedef struct XMLDocRecord             XMLDocRecord;

typedef XMLDocRecord *                  XMLDoc;
/*callback routines for event parsing*/
typedef CALLBACK_API( ComponentResult , StartDocumentHandler )(long refcon);
typedef CALLBACK_API( ComponentResult , EndDocumentHandler )(long refcon);
typedef CALLBACK_API( ComponentResult , StartElementHandler )(const char *name, const char *const atts[], long refcon);
typedef CALLBACK_API( ComponentResult , EndElementHandler )(const char *name, long refcon);
typedef CALLBACK_API( ComponentResult , CharDataHandler )(const char *charData, long refcon);
typedef CALLBACK_API( ComponentResult , PreprocessInstructionHandler )(const char *name, const char *const atts[], long refcon);
typedef CALLBACK_API( ComponentResult , CommentHandler )(const char *comment, long refcon);
typedef CALLBACK_API( ComponentResult , CDataHandler )(const char *cdata, long refcon);
typedef STACK_UPP_TYPE(StartDocumentHandler)                    StartDocumentHandlerUPP;
typedef STACK_UPP_TYPE(EndDocumentHandler)                      EndDocumentHandlerUPP;
typedef STACK_UPP_TYPE(StartElementHandler)                     StartElementHandlerUPP;
typedef STACK_UPP_TYPE(EndElementHandler)                       EndElementHandlerUPP;
typedef STACK_UPP_TYPE(CharDataHandler)                         CharDataHandlerUPP;
typedef STACK_UPP_TYPE(PreprocessInstructionHandler)            PreprocessInstructionHandlerUPP;
typedef STACK_UPP_TYPE(CommentHandler)                          CommentHandlerUPP;
typedef STACK_UPP_TYPE(CDataHandler)                            CDataHandlerUPP;
/*  Parses the XML file pointed to by dataRef, returning a XMLDoc parse tree*/
EXTERN_API( ComponentResult )
XMLParseDataRef                 (ComponentInstance      aParser,
                                 Handle                 dataRef,
                                 OSType                 dataRefType,
                                 long                   parseFlags,
                                 XMLDoc *               document)                           FIVEWORDINLINE(0x2F3C, 0x0010, 0x0001, 0x7000, 0xA82A);

/*  Parses the XML file pointed to by fileSpec, returning a XMLDoc parse tree*/
EXTERN_API( ComponentResult )
XMLParseFile                    (ComponentInstance      aParser,
                                 ConstFSSpecPtr         fileSpec,
                                 long                   parseFlags,
                                 XMLDoc *               document)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x0002, 0x7000, 0xA82A);

/*  Disposes of a XMLDoc parse tree*/
EXTERN_API( ComponentResult )
XMLParseDisposeXMLDoc           (ComponentInstance      aParser,
                                 XMLDoc                 document)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0003, 0x7000, 0xA82A);

/*
    Returns a more detailed description of the error and the line in which it occurred, if a
    file failed to parse properly.
*/
EXTERN_API( ComponentResult )
XMLParseGetDetailedParseError   (ComponentInstance      aParser,
                                 long *                 errorLine,
                                 StringPtr              errDesc)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x0004, 0x7000, 0xA82A);

/*
    Tell the parser of an element to be recognized. The tokenized element unique identifier is
    passed in *elementID, unless *elementID is zero, whereupon a unique ID is generated and returned.
    Thus, a valid element identifier can never be zero.
*/
EXTERN_API( ComponentResult )
XMLParseAddElement              (ComponentInstance      aParser,
                                 char *                 elementName,
                                 UInt32                 nameSpaceID,
                                 UInt32 *               elementID,
                                 long                   elementFlags)                       FIVEWORDINLINE(0x2F3C, 0x0010, 0x0005, 0x7000, 0xA82A);

/*
    Tells the parser of an attribute for the specified element. The tokenized attribute unique
    ID is passed in *attributeID, unless *attributeID is zero, whereupon a unique ID is generated and
    returned. Thus, a valid attribute identifier can never be zero.
*/
EXTERN_API( ComponentResult )
XMLParseAddAttribute            (ComponentInstance      aParser,
                                 UInt32                 elementID,
                                 UInt32                 nameSpaceID,
                                 char *                 attributeName,
                                 UInt32 *               attributeID)                        FIVEWORDINLINE(0x2F3C, 0x0010, 0x0006, 0x7000, 0xA82A);

/*
    Tells the parser of several attributes for the specified element. The attributes are passed
    as a zero-delimited, double-zero-terminated C string in attributeNames, and the attribute
    IDs are passed in on attributeIDs as an array; if any attributeIDs are zero, unique IDs
    are generated for those and returned
*/
EXTERN_API( ComponentResult )
XMLParseAddMultipleAttributes   (ComponentInstance      aParser,
                                 UInt32                 elementID,
                                 UInt32 *               nameSpaceIDs,
                                 char *                 attributeNames,
                                 UInt32 *               attributeIDs)                       FIVEWORDINLINE(0x2F3C, 0x0010, 0x0007, 0x7000, 0xA82A);

/*
    Tells the parser of an attribute, which may have a particular type of value, for the
    specified element. Params are as in XMLParseAddAttribute, plus all the kinds of values
    the attribute may have are passed in attributeValueKind, and optional additional information
    required to tokenize the particular kind of attribute is passed in attributeValueKindInfo
*/
EXTERN_API( ComponentResult )
XMLParseAddAttributeAndValue    (ComponentInstance      aParser,
                                 UInt32                 elementID,
                                 UInt32                 nameSpaceID,
                                 char *                 attributeName,
                                 UInt32 *               attributeID,
                                 UInt32                 attributeValueKind,
                                 void *                 attributeValueKindInfo)             FIVEWORDINLINE(0x2F3C, 0x0018, 0x0008, 0x7000, 0xA82A);

/*
    Tells the parser of several attributes, which may have a particular type of value, for the
    specified element. Params are as in XMLParseAddMultipleAttributes, plus all the kinds of values
    the attributes may have are passed in attributeValueKinds, and optional additional information
    required to tokenize the particular kind of attributes is passed in attributeValueKindInfos
*/
EXTERN_API( ComponentResult )
XMLParseAddMultipleAttributesAndValues (ComponentInstance  aParser,
                                 UInt32                 elementID,
                                 UInt32 *               nameSpaceIDs,
                                 char *                 attributeNames,
                                 UInt32 *               attributeIDs,
                                 UInt32 *               attributeValueKinds,
                                 void **                attributeValueKindInfos)            FIVEWORDINLINE(0x2F3C, 0x0018, 0x0009, 0x7000, 0xA82A);

/*
    Tells the parser that the particular attribute may have an additional kind of
    value, as specified by attributeValueKind and attributeValueKindInfo
*/
EXTERN_API( ComponentResult )
XMLParseAddAttributeValueKind   (ComponentInstance      aParser,
                                 UInt32                 elementID,
                                 UInt32                 attributeID,
                                 UInt32                 attributeValueKind,
                                 void *                 attributeValueKindInfo)             FIVEWORDINLINE(0x2F3C, 0x0010, 0x000A, 0x7000, 0xA82A);

/*
    Tell the parser of a namespace to be recognized. The tokenized namespace unique identifier is
    passed in *nameSpaceID, unless *nameSpaceID is zero, whereupon a unique ID is generated and returned.
    Thus, a valid nameSpaceID identifier can never be zero.
*/
EXTERN_API( ComponentResult )
XMLParseAddNameSpace            (ComponentInstance      aParser,
                                 char *                 nameSpaceURL,
                                 UInt32 *               nameSpaceID)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x000B, 0x7000, 0xA82A);

/*  Specifies the offset and limit for reading from the dataref to be used when parsing*/
EXTERN_API( ComponentResult )
XMLParseSetOffsetAndLimit       (ComponentInstance      aParser,
                                 UInt32                 offset,
                                 UInt32                 limit)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x000C, 0x7000, 0xA82A);

/*  Set the event parse refcon*/
EXTERN_API( ComponentResult )
XMLParseSetEventParseRefCon     (ComponentInstance      aParser,
                                 long                   refcon)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x000D, 0x7000, 0xA82A);

/*  Set the start document handler UPP for event parsing*/
EXTERN_API( ComponentResult )
XMLParseSetStartDocumentHandler (ComponentInstance      aParser,
                                 StartDocumentHandlerUPP  startDocument)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x000E, 0x7000, 0xA82A);

/*  Set the end document handler UPP for event parsing*/
EXTERN_API( ComponentResult )
XMLParseSetEndDocumentHandler   (ComponentInstance      aParser,
                                 EndDocumentHandlerUPP  endDocument)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x000F, 0x7000, 0xA82A);

/*  Set the start element handler UPP for event parsing*/
EXTERN_API( ComponentResult )
XMLParseSetStartElementHandler  (ComponentInstance      aParser,
                                 StartElementHandlerUPP  startElement)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x0010, 0x7000, 0xA82A);

/*  Set the end element handler UPP for event parsing*/
EXTERN_API( ComponentResult )
XMLParseSetEndElementHandler    (ComponentInstance      aParser,
                                 EndElementHandlerUPP   endElement)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0011, 0x7000, 0xA82A);

/*  Set the character data handler UPP for event parsing*/
EXTERN_API( ComponentResult )
XMLParseSetCharDataHandler      (ComponentInstance      aParser,
                                 CharDataHandlerUPP     charData)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0012, 0x7000, 0xA82A);

/*  Set the preprocess instruction handler UPP for event parsing*/
EXTERN_API( ComponentResult )
XMLParseSetPreprocessInstructionHandler (ComponentInstance  aParser,
                                 PreprocessInstructionHandlerUPP  preprocessInstruction)    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0013, 0x7000, 0xA82A);

/*  Set the comment handler UPP for event parsing*/
EXTERN_API( ComponentResult )
XMLParseSetCommentHandler       (ComponentInstance      aParser,
                                 CommentHandlerUPP      comment)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0014, 0x7000, 0xA82A);

/*  Set the cdata handler UPP for event parsing*/
EXTERN_API( ComponentResult )
XMLParseSetCDataHandler         (ComponentInstance      aParser,
                                 CDataHandlerUPP        cdata)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0015, 0x7000, 0xA82A);

/*
    Helper Macros
    
        These macros allow you to easily add entities and attributes to the parser
        in an error free manner when the identifiers are defined in a particular manner.
        For these to work, you must define the identifiers as follows:
        
        For entities, they must be defined as element_elementName, as in:
        
            enum
            {
                element_xml =   1,      //  "xml"
                element_head,           //  "head"
                element_body            //  "body"
            };
            
        If the element name has characters that are illegal in an identifier,
        some of the macros support that, but the identifier must not contain
        the illegal characters:
        
            enum
            {
                element_rootlayout      //  "root-layout"
            }
            
        For attribute names, similar rules apply except that they must be defined
        as attr_attributeName, as in:
            
            enum
            {
                attr_src    =   1,      //  "src"
                attr_href,
                attr_width,
                attr_height
            }
            
        Finally, the existence of local variables elementID and attributeID is required.
*/
/*
    Adds the specified element to the parser, i.e. XML_ADD_ELEMENT(head) adds the element "head" with
    a unique identifier of element_head
*/

#define XML_ADD_ELEMENT_NS(elementName,nameSpaceID)        elementID   =   GLUE2(element_,elementName);    XMLParseAddElement(xmlParser, #elementName, nameSpaceID, &elementID, 0)
#define XML_ADD_ELEMENT(elementName)                    XML_ADD_ELEMENT_NS(elementName,nameSpaceIDNone)

/*
    Adds the specified element to the parser, not using the same string to generate the identifier and
    the element name. Use for element names that contain characters which are illegal in identifiers,
    i.e XML_ADD_COMPLEX_ELEMENT("root-layout",rootlayout) adds the element "root-layout" with a unique
    identifier of element_rootlayout
*/

#define XML_ADD_COMPLEX_ELEMENT_NS(elementName,elemID,nameSpaceID)     elementID   =   GLUE2(element_,elemID);     XMLParseAddElement(xmlParser, #elementName, nameSpaceID, &elementID, 0)
#define XML_ADD_COMPLEX_ELEMENT(elementName,elemID)                     XML_ADD_COMPLEX_ELEMENT_NS(elementName,elemID,nameSpaceIDNone)

/*
    Adds the specified attribute to the current element in the parser, i.e. XML_ADD_ATTRIBUTE(src)
    adds the attribute "src" to the current element, and identifies it by attr_src
*/

#define XML_ADD_ATTRIBUTE_NS(attrName,nameSpaceID)     attributeID =   GLUE2(attr_,attrName);      XMLParseAddAttribute(xmlParser, elementID, nameSpaceID, #attrName, &attributeID);
#define XML_ADD_ATTRIBUTE(attrName)                       XML_ADD_ATTRIBUTE_NS(attrName,nameSpaceIDNone)

/*
    Adds the specified attribute to the current element in the parser, i.e. XML_ADD_ATTRIBUTE(element_img, src)
    adds the attribute "src" to the element_img element, and identifies it by attr_src
    Adds the specified attribute to the current element in the parser, not using the same string to
    generate the identifier and the element name. Use for attribute names that contain characters which
    are illegal in identifiers, i.e XML_ADD_COMPLEX_ATTRIBUTE("http-equiv",httpequiv) adds the element
    "http-equiv" with a unique identifier of attr_httpequiv
*/

#define XML_ADD_COMPLEX_ATTRIBUTE_NS(attrName,attrID,nameSpaceID)  attributeID =   GLUE2(attr_,attrID);        XMLParseAddAttribute(xmlParser, elementID, nameSpaceID, #attrName, &attributeID);
#define XML_ADD_COMPLEX_ATTRIBUTE(attrName,attrID)                    XML_ADD_COMPLEX_ATTRIBUTE_NS(attrName,attrID,nameSpaceIDNone)


#define XML_ADD_ATTRIBUTE_AND_VALUE_NS(attrName,valueKind,valueKindInfo,nameSpaceID)   attributeID =   GLUE2(attr_,attrName);      XMLParseAddAttributeAndValue(xmlParser, elementID, nameSpaceID, #attrName, &attributeID, valueKind, valueKindInfo);
#define XML_ADD_ATTRIBUTE_AND_VALUE(attrName,valueKind,valueKindInfo)                   XML_ADD_ATTRIBUTE_AND_VALUE_NS(attrName,valueKind,valueKindInfo,nameSpaceIDNone)


#define XML_ADD_COMPLEX_ATTRIBUTE_AND_VALUE_NS(attrName,attrID,valueKind,valueKindInfo,nameSpaceID)        attributeID =   GLUE2(attr_,attrID);        XMLParseAddAttributeAndValue(xmlParser, elementID, nameSpaceID, #attrName, &attributeID, valueKind, valueKindInfo);
#define XML_ADD_COMPLEX_ATTRIBUTE_AND_VALUE(attrName,attrID,valueKind,valueKindInfo)                    XML_ADD_COMPLEX_ATTRIBUTE_AND_VALUE_NS(attrName,attrID,valueKind,valueKindInfo,nameSpaceIDNone)






/*
    General Sequence Grab stuff
*/
typedef ComponentInstance               SeqGrabComponent;
typedef ComponentInstance               SGChannel;
enum {
    SeqGrabComponentType        = FOUR_CHAR_CODE('barg'),
    SeqGrabChannelType          = FOUR_CHAR_CODE('sgch'),
    SeqGrabPanelType            = FOUR_CHAR_CODE('sgpn'),
    SeqGrabCompressionPanelType = FOUR_CHAR_CODE('cmpr'),
    SeqGrabSourcePanelType      = FOUR_CHAR_CODE('sour')
};

enum {
    seqGrabToDisk               = 1,
    seqGrabToMemory             = 2,
    seqGrabDontUseTempMemory    = 4,
    seqGrabAppendToFile         = 8,
    seqGrabDontAddMovieResource = 16,
    seqGrabDontMakeMovie        = 32,
    seqGrabPreExtendFile        = 64,
    seqGrabDataProcIsInterruptSafe = 128,
    seqGrabDataProcDoesOverlappingReads = 256
};

typedef unsigned long                   SeqGrabDataOutputEnum;
enum {
    seqGrabRecord               = 1,
    seqGrabPreview              = 2,
    seqGrabPlayDuringRecord     = 4,
    seqGrabLowLatencyCapture    = 8,                            /* return the freshest frame possible, for live work (videoconferencing, live broadcast, live image processing) */
    seqGrabAlwaysUseTimeBase    = 16                            /* Tell VDIGs to use TimebaseTime always, rather than creating uniform frame durations, for more accurate live sync with audio */
};

typedef unsigned long                   SeqGrabUsageEnum;
enum {
    seqGrabHasBounds            = 1,
    seqGrabHasVolume            = 2,
    seqGrabHasDiscreteSamples   = 4,
    seqGrabDoNotBufferizeData   = 8,
    seqGrabCanMoveWindowWhileRecording = 16
};

typedef unsigned long                   SeqGrabChannelInfoEnum;

struct SGOutputRecord {
    long                            data[1];
};
typedef struct SGOutputRecord           SGOutputRecord;

typedef SGOutputRecord *                SGOutput;

struct SeqGrabFrameInfo {
    long                            frameOffset;
    long                            frameTime;
    long                            frameSize;
    SGChannel                       frameChannel;
    long                            frameRefCon;
};
typedef struct SeqGrabFrameInfo         SeqGrabFrameInfo;
typedef SeqGrabFrameInfo *              SeqGrabFrameInfoPtr;

struct SeqGrabExtendedFrameInfo {
    wide                            frameOffset;
    long                            frameTime;
    long                            frameSize;
    SGChannel                       frameChannel;
    long                            frameRefCon;
    SGOutput                        frameOutput;
};
typedef struct SeqGrabExtendedFrameInfo SeqGrabExtendedFrameInfo;
typedef SeqGrabExtendedFrameInfo *      SeqGrabExtendedFrameInfoPtr;
enum {
    grabPictOffScreen           = 1,
    grabPictIgnoreClip          = 2,
    grabPictCurrentImage        = 4
};

enum {
    sgFlagControlledGrab        = (1 << 0),
    sgFlagAllowNonRGBPixMaps    = (1 << 1)
};

typedef CALLBACK_API( OSErr , SGDataProcPtr )(SGChannel c, Ptr p, long len, long *offset, long chRefCon, TimeValue time, short writeType, long refCon);
typedef STACK_UPP_TYPE(SGDataProcPtr)                           SGDataUPP;

struct SGDeviceInputName {
    Str63                           name;
    Handle                          icon;
    long                            flags;
    long                            reserved;                   /* zero*/
};
typedef struct SGDeviceInputName        SGDeviceInputName;
enum {
    sgDeviceInputNameFlagInputUnavailable = (1 << 0)
};


struct SGDeviceInputListRecord {
    short                           count;
    short                           selectedIndex;
    long                            reserved;                   /* zero*/
    SGDeviceInputName               entry[1];
};
typedef struct SGDeviceInputListRecord  SGDeviceInputListRecord;

typedef SGDeviceInputListRecord *       SGDeviceInputListPtr;
typedef SGDeviceInputListPtr *          SGDeviceInputList;

struct SGDeviceName {
    Str63                           name;
    Handle                          icon;
    long                            flags;
    long                            refCon;
    SGDeviceInputList               inputs;                     /* list of inputs; formerly reserved to 0*/
};
typedef struct SGDeviceName             SGDeviceName;
enum {
    sgDeviceNameFlagDeviceUnavailable = (1 << 0),
    sgDeviceNameFlagShowInputsAsDevices = (1 << 1)
};


struct SGDeviceListRecord {
    short                           count;
    short                           selectedIndex;
    long                            reserved;                   /* zero*/
    SGDeviceName                    entry[1];
};
typedef struct SGDeviceListRecord       SGDeviceListRecord;

typedef SGDeviceListRecord *            SGDeviceListPtr;
typedef SGDeviceListPtr *               SGDeviceList;
enum {
    sgDeviceListWithIcons       = (1 << 0),
    sgDeviceListDontCheckAvailability = (1 << 1),
    sgDeviceListIncludeInputs   = (1 << 2)
};

enum {
    seqGrabWriteAppend          = 0,
    seqGrabWriteReserve         = 1,
    seqGrabWriteFill            = 2
};

enum {
    seqGrabUnpause              = 0,
    seqGrabPause                = 1,
    seqGrabPauseForMenu         = 3
};

enum {
    channelFlagDontOpenResFile  = 2,
    channelFlagHasDependency    = 4
};

typedef CALLBACK_API( Boolean , SGModalFilterProcPtr )(DialogRef theDialog, const EventRecord *theEvent, short *itemHit, long refCon);
typedef STACK_UPP_TYPE(SGModalFilterProcPtr)                    SGModalFilterUPP;
enum {
    sgPanelFlagForPanel         = 1
};

enum {
    seqGrabSettingsPreviewOnly  = 1
};

enum {
    channelPlayNormal           = 0,
    channelPlayFast             = 1,
    channelPlayHighQuality      = 2,
    channelPlayAllData          = 4
};


EXTERN_API( ComponentResult )
SGInitialize                    (SeqGrabComponent       s)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetDataOutput                 (SeqGrabComponent       s,
                                 const FSSpec *         movieFile,
                                 long                   whereFlags)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetDataOutput                 (SeqGrabComponent       s,
                                 FSSpec *               movieFile,
                                 long *                 whereFlags)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetGWorld                     (SeqGrabComponent       s,
                                 CGrafPtr               gp,
                                 GDHandle               gd)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetGWorld                     (SeqGrabComponent       s,
                                 CGrafPtr *             gp,
                                 GDHandle *             gd)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGNewChannel                    (SeqGrabComponent       s,
                                 OSType                 channelType,
                                 SGChannel *            ref)                                FIVEWORDINLINE(0x2F3C, 0x0008, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGDisposeChannel                (SeqGrabComponent       s,
                                 SGChannel              c)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGStartPreview                  (SeqGrabComponent       s)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGStartRecord                   (SeqGrabComponent       s)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0011, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGIdle                          (SeqGrabComponent       s)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0012, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGStop                          (SeqGrabComponent       s)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0013, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPause                         (SeqGrabComponent       s,
                                 Byte                   pause)                              FIVEWORDINLINE(0x2F3C, 0x0002, 0x0014, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPrepare                       (SeqGrabComponent       s,
                                 Boolean                prepareForPreview,
                                 Boolean                prepareForRecord)                   FIVEWORDINLINE(0x2F3C, 0x0004, 0x0015, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGRelease                       (SeqGrabComponent       s)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0016, 0x7000, 0xA82A);

EXTERN_API( Movie )
SGGetMovie                      (SeqGrabComponent       s)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0017, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetMaximumRecordTime          (SeqGrabComponent       s,
                                 unsigned long          ticks)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0018, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetMaximumRecordTime          (SeqGrabComponent       s,
                                 unsigned long *        ticks)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0019, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetStorageSpaceRemaining      (SeqGrabComponent       s,
                                 unsigned long *        bytes)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x001A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetTimeRemaining              (SeqGrabComponent       s,
                                 long *                 ticksLeft)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x001B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGrabPict                      (SeqGrabComponent       s,
                                 PicHandle *            p,
                                 const Rect *           bounds,
                                 short                  offscreenDepth,
                                 long                   grabPictFlags)                      FIVEWORDINLINE(0x2F3C, 0x000E, 0x001C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetLastMovieResID             (SeqGrabComponent       s,
                                 short *                resID)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x001D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetFlags                      (SeqGrabComponent       s,
                                 long                   sgFlags)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x001E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetFlags                      (SeqGrabComponent       s,
                                 long *                 sgFlags)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x001F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetDataProc                   (SeqGrabComponent       s,
                                 SGDataUPP              proc,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0020, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGNewChannelFromComponent       (SeqGrabComponent       s,
                                 SGChannel *            newChannel,
                                 Component              sgChannelComponent)                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0021, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGDisposeDeviceList             (SeqGrabComponent       s,
                                 SGDeviceList           list)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0022, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGAppendDeviceListToMenu        (SeqGrabComponent       s,
                                 SGDeviceList           list,
                                 MenuRef                mh)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0023, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetSettings                   (SeqGrabComponent       s,
                                 UserData               ud,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0024, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetSettings                   (SeqGrabComponent       s,
                                 UserData *             ud,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0025, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetIndChannel                 (SeqGrabComponent       s,
                                 short                  index,
                                 SGChannel *            ref,
                                 OSType *               chanType)                           FIVEWORDINLINE(0x2F3C, 0x000A, 0x0026, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGUpdate                        (SeqGrabComponent       s,
                                 RgnHandle              updateRgn)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0027, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetPause                      (SeqGrabComponent       s,
                                 Byte *                 paused)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0028, 0x7000, 0xA82A);

typedef const Component *               ConstComponentListPtr;
EXTERN_API( ComponentResult )
SGSettingsDialog                (SeqGrabComponent       s,
                                 SGChannel              c,
                                 short                  numPanels,
                                 ConstComponentListPtr  panelList,
                                 long                   flags,
                                 SGModalFilterUPP       proc,
                                 long                   procRefNum)                         FIVEWORDINLINE(0x2F3C, 0x0016, 0x0029, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetAlignmentProc              (SeqGrabComponent       s,
                                 ICMAlignmentProcRecordPtr  alignmentProc)                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x002A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetChannelSettings            (SeqGrabComponent       s,
                                 SGChannel              c,
                                 UserData               ud,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x002B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelSettings            (SeqGrabComponent       s,
                                 SGChannel              c,
                                 UserData *             ud,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x002C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetMode                       (SeqGrabComponent       s,
                                 Boolean *              previewMode,
                                 Boolean *              recordMode)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x002D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetDataRef                    (SeqGrabComponent       s,
                                 Handle                 dataRef,
                                 OSType                 dataRefType,
                                 long                   whereFlags)                         FIVEWORDINLINE(0x2F3C, 0x000C, 0x002E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetDataRef                    (SeqGrabComponent       s,
                                 Handle *               dataRef,
                                 OSType *               dataRefType,
                                 long *                 whereFlags)                         FIVEWORDINLINE(0x2F3C, 0x000C, 0x002F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGNewOutput                     (SeqGrabComponent       s,
                                 Handle                 dataRef,
                                 OSType                 dataRefType,
                                 long                   whereFlags,
                                 SGOutput *             sgOut)                              FIVEWORDINLINE(0x2F3C, 0x0010, 0x0030, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGDisposeOutput                 (SeqGrabComponent       s,
                                 SGOutput               sgOut)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0031, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetOutputFlags                (SeqGrabComponent       s,
                                 SGOutput               sgOut,
                                 long                   whereFlags)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0032, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetChannelOutput              (SeqGrabComponent       s,
                                 SGChannel              c,
                                 SGOutput               sgOut)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0033, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetDataOutputStorageSpaceRemaining (SeqGrabComponent  s,
                                 SGOutput               sgOut,
                                 unsigned long *        space)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0034, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGHandleUpdateEvent             (SeqGrabComponent       s,
                                 const EventRecord *    event,
                                 Boolean *              handled)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x0035, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetOutputNextOutput           (SeqGrabComponent       s,
                                 SGOutput               sgOut,
                                 SGOutput               nextOut)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x0036, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetOutputNextOutput           (SeqGrabComponent       s,
                                 SGOutput               sgOut,
                                 SGOutput *             nextOut)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x0037, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetOutputMaximumOffset        (SeqGrabComponent       s,
                                 SGOutput               sgOut,
                                 const wide *           maxOffset)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0038, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetOutputMaximumOffset        (SeqGrabComponent       s,
                                 SGOutput               sgOut,
                                 wide *                 maxOffset)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0039, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetOutputDataReference        (SeqGrabComponent       s,
                                 SGOutput               sgOut,
                                 Handle *               dataRef,
                                 OSType *               dataRefType)                        FIVEWORDINLINE(0x2F3C, 0x000C, 0x003A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGWriteExtendedMovieData        (SeqGrabComponent       s,
                                 SGChannel              c,
                                 Ptr                    p,
                                 long                   len,
                                 wide *                 offset,
                                 SGOutput *             sgOut)                              FIVEWORDINLINE(0x2F3C, 0x0014, 0x003B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetStorageSpaceRemaining64    (SeqGrabComponent       s,
                                 wide *                 bytes)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x003C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetDataOutputStorageSpaceRemaining64 (SeqGrabComponent  s,
                                 SGOutput               sgOut,
                                 wide *                 space)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x003D, 0x7000, 0xA82A);

/*
    calls from Channel to seqGrab
*/
EXTERN_API( ComponentResult )
SGWriteMovieData                (SeqGrabComponent       s,
                                 SGChannel              c,
                                 Ptr                    p,
                                 long                   len,
                                 long *                 offset)                             FIVEWORDINLINE(0x2F3C, 0x0010, 0x0100, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGAddFrameReference             (SeqGrabComponent       s,
                                 SeqGrabFrameInfoPtr    frameInfo)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetNextFrameReference         (SeqGrabComponent       s,
                                 SeqGrabFrameInfoPtr    frameInfo,
                                 TimeValue *            frameDuration,
                                 long *                 frameNumber)                        FIVEWORDINLINE(0x2F3C, 0x000C, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetTimeBase                   (SeqGrabComponent       s,
                                 TimeBase *             tb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSortDeviceList                (SeqGrabComponent       s,
                                 SGDeviceList           list)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0104, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGAddMovieData                  (SeqGrabComponent       s,
                                 SGChannel              c,
                                 Ptr                    p,
                                 long                   len,
                                 long *                 offset,
                                 long                   chRefCon,
                                 TimeValue              time,
                                 short                  writeType)                          FIVEWORDINLINE(0x2F3C, 0x001A, 0x0105, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGChangedSource                 (SeqGrabComponent       s,
                                 SGChannel              c)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0106, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGAddExtendedFrameReference     (SeqGrabComponent       s,
                                 SeqGrabExtendedFrameInfoPtr  frameInfo)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0107, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetNextExtendedFrameReference (SeqGrabComponent       s,
                                 SeqGrabExtendedFrameInfoPtr  frameInfo,
                                 TimeValue *            frameDuration,
                                 long *                 frameNumber)                        FIVEWORDINLINE(0x2F3C, 0x000C, 0x0108, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGAddExtendedMovieData          (SeqGrabComponent       s,
                                 SGChannel              c,
                                 Ptr                    p,
                                 long                   len,
                                 wide *                 offset,
                                 long                   chRefCon,
                                 TimeValue              time,
                                 short                  writeType,
                                 SGOutput *             whichOutput)                        FIVEWORDINLINE(0x2F3C, 0x001E, 0x0109, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGAddOutputDataRefToMedia       (SeqGrabComponent       s,
                                 SGOutput               sgOut,
                                 Media                  theMedia,
                                 SampleDescriptionHandle  desc)                             FIVEWORDINLINE(0x2F3C, 0x000C, 0x010A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetSettingsSummary            (SeqGrabComponent       s,
                                 Handle                 summaryText)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x010B, 0x7000, 0xA82A);



/*** Sequence Grab CHANNEL Component Stuff ***/

EXTERN_API( ComponentResult )
SGSetChannelUsage               (SGChannel              c,
                                 long                   usage)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0080, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelUsage               (SGChannel              c,
                                 long *                 usage)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0081, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetChannelBounds              (SGChannel              c,
                                 const Rect *           bounds)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0082, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelBounds              (SGChannel              c,
                                 Rect *                 bounds)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0083, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetChannelVolume              (SGChannel              c,
                                 short                  volume)                             FIVEWORDINLINE(0x2F3C, 0x0002, 0x0084, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelVolume              (SGChannel              c,
                                 short *                volume)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0085, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelInfo                (SGChannel              c,
                                 long *                 channelInfo)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x0086, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetChannelPlayFlags           (SGChannel              c,
                                 long                   playFlags)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0087, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelPlayFlags           (SGChannel              c,
                                 long *                 playFlags)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0088, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetChannelMaxFrames           (SGChannel              c,
                                 long                   frameCount)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0089, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelMaxFrames           (SGChannel              c,
                                 long *                 frameCount)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x008A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetChannelRefCon              (SGChannel              c,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x008B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetChannelClip                (SGChannel              c,
                                 RgnHandle              theClip)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x008C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelClip                (SGChannel              c,
                                 RgnHandle *            theClip)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x008D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelSampleDescription   (SGChannel              c,
                                 Handle                 sampleDesc)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x008E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelDeviceList          (SGChannel              c,
                                 long                   selectionFlags,
                                 SGDeviceList *         list)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x008F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetChannelDevice              (SGChannel              c,
                                 StringPtr              name)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0090, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetChannelMatrix              (SGChannel              c,
                                 const MatrixRecord *   m)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0091, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelMatrix              (SGChannel              c,
                                 MatrixRecord *         m)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0092, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelTimeScale           (SGChannel              c,
                                 TimeScale *            scale)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0093, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGChannelPutPicture             (SGChannel              c)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0094, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGChannelSetRequestedDataRate   (SGChannel              c,
                                 long                   bytesPerSecond)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0095, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGChannelGetRequestedDataRate   (SGChannel              c,
                                 long *                 bytesPerSecond)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0096, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGChannelSetDataSourceName      (SGChannel              c,
                                 ConstStr255Param       name,
                                 ScriptCode             scriptTag)                          FIVEWORDINLINE(0x2F3C, 0x0006, 0x0097, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGChannelGetDataSourceName      (SGChannel              c,
                                 Str255                 name,
                                 ScriptCode *           scriptTag)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0098, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGChannelSetCodecSettings       (SGChannel              c,
                                 Handle                 settings)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0099, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGChannelGetCodecSettings       (SGChannel              c,
                                 Handle *               settings)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x009A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelTimeBase            (SGChannel              c,
                                 TimeBase *             tb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x009B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetChannelRefCon              (SGChannel              c,
                                 long *                 refCon)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x009C, 0x7000, 0xA82A);

/* A utility call to find out the current device and input names, instead of having to call GetDeviceList and walk it yourself */
EXTERN_API( ComponentResult )
SGGetChannelDeviceAndInputNames (SGChannel              c,
                                 Str255                 outDeviceName,
                                 Str255                 outInputName,
                                 short *                outInputNumber)                     FIVEWORDINLINE(0x2F3C, 0x000C, 0x009D, 0x7000, 0xA82A);

/* A media format independent call for this. Inputs start at 0 here (Sound starts at 1, VDIGs at 0 in direct calls) */
EXTERN_API( ComponentResult )
SGSetChannelDeviceInput         (SGChannel              c,
                                 short                  inInputNumber)                      FIVEWORDINLINE(0x2F3C, 0x0002, 0x009E, 0x7000, 0xA82A);

/* A call to bracket SetSettings related calls, to give downstream components an opportunity to deal with the entire 
    settings change in one go */
enum {
    sgSetSettingsBegin          = (1 << 0),                     /* SGSetSettings related set calls about to start*/
    sgSetSettingsEnd            = (1 << 1)                      /* Finished SGSetSettings calls. Get ready to use the new settings*/
};

EXTERN_API( ComponentResult )
SGSetChannelSettingsStateChanging (SGChannel            c,
                                 UInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x009F, 0x7000, 0xA82A);

/*
    calls from seqGrab to Channel
*/
EXTERN_API( ComponentResult )
SGInitChannel                   (SGChannel              c,
                                 SeqGrabComponent       owner)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0180, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGWriteSamples                  (SGChannel              c,
                                 Movie                  m,
                                 AliasHandle            theFile)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x0181, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetDataRate                   (SGChannel              c,
                                 long *                 bytesPerSecond)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0182, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGAlignChannelRect              (SGChannel              c,
                                 Rect *                 r)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0183, 0x7000, 0xA82A);

/*
    Dorky dialog panel calls
*/
EXTERN_API( ComponentResult )
SGPanelGetDitl                  (SeqGrabComponent       s,
                                 Handle *               ditl)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0200, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelGetTitle                 (SeqGrabComponent       s,
                                 Str255                 title)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0201, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelCanRun                   (SeqGrabComponent       s,
                                 SGChannel              c)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0202, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelInstall                  (SeqGrabComponent       s,
                                 SGChannel              c,
                                 DialogRef              d,
                                 short                  itemOffset)                         FIVEWORDINLINE(0x2F3C, 0x000A, 0x0203, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelEvent                    (SeqGrabComponent       s,
                                 SGChannel              c,
                                 DialogRef              d,
                                 short                  itemOffset,
                                 const EventRecord *    theEvent,
                                 short *                itemHit,
                                 Boolean *              handled)                            FIVEWORDINLINE(0x2F3C, 0x0016, 0x0204, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelItem                     (SeqGrabComponent       s,
                                 SGChannel              c,
                                 DialogRef              d,
                                 short                  itemOffset,
                                 short                  itemNum)                            FIVEWORDINLINE(0x2F3C, 0x000C, 0x0205, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelRemove                   (SeqGrabComponent       s,
                                 SGChannel              c,
                                 DialogRef              d,
                                 short                  itemOffset)                         FIVEWORDINLINE(0x2F3C, 0x000A, 0x0206, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelSetGrabber               (SeqGrabComponent       s,
                                 SeqGrabComponent       sg)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0207, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelSetResFile               (SeqGrabComponent       s,
                                 short                  resRef)                             FIVEWORDINLINE(0x2F3C, 0x0002, 0x0208, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelGetSettings              (SeqGrabComponent       s,
                                 SGChannel              c,
                                 UserData *             ud,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0209, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelSetSettings              (SeqGrabComponent       s,
                                 SGChannel              c,
                                 UserData               ud,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x020A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelValidateInput            (SeqGrabComponent       s,
                                 Boolean *              ok)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x020B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGPanelSetEventFilter           (SeqGrabComponent       s,
                                 SGModalFilterUPP       proc,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x020C, 0x7000, 0xA82A);

/*
    SGPanelGetDITLForSize is used to retrieve user interface elements that fit within a specified size
    panel.  The component should return badComponentSelector for sizes it does not support.  The component
    is required to support kSGSmallestDITLSize, and it is recommended to support kSGLargestDITLSize.
    
    If SGPanelGetDITLForSize is unimplemented entirely, the panel is assumed to not have resizable UI elements.
*/
enum {
    kSGSmallestDITLSize         = -1,                           /* requestedSize h and v set to this to retrieve small size*/
    kSGLargestDITLSize          = -2                            /* requestedSize h and v set to this to retrieve large size*/
};

EXTERN_API( ComponentResult )
SGPanelGetDITLForSize           (SeqGrabComponent       s,
                                 Handle *               ditl,
                                 Point *                requestedSize)                      FIVEWORDINLINE(0x2F3C, 0x0008, 0x020D, 0x7000, 0xA82A);


/*** Sequence Grab VIDEO CHANNEL Component Stuff ***/
/*
    Video stuff
*/

struct SGCompressInfo {
    Ptr                             buffer;
    unsigned long                   bufferSize;
    UInt8                           similarity;
    UInt8                           reserved;
};
typedef struct SGCompressInfo           SGCompressInfo;
typedef CALLBACK_API( ComponentResult , SGGrabBottleProcPtr )(SGChannel c, short bufferNum, long refCon);
typedef CALLBACK_API( ComponentResult , SGGrabCompleteBottleProcPtr )(SGChannel c, short bufferNum, Boolean *done, long refCon);
typedef CALLBACK_API( ComponentResult , SGDisplayBottleProcPtr )(SGChannel c, short bufferNum, MatrixRecord *mp, RgnHandle clipRgn, long refCon);
typedef CALLBACK_API( ComponentResult , SGCompressBottleProcPtr )(SGChannel c, short bufferNum, long refCon);
typedef CALLBACK_API( ComponentResult , SGCompressCompleteBottleProcPtr )(SGChannel c, short bufferNum, Boolean *done, SGCompressInfo *ci, long refCon);
typedef CALLBACK_API( ComponentResult , SGAddFrameBottleProcPtr )(SGChannel c, short bufferNum, TimeValue atTime, TimeScale scale, const SGCompressInfo *ci, long refCon);
typedef CALLBACK_API( ComponentResult , SGTransferFrameBottleProcPtr )(SGChannel c, short bufferNum, MatrixRecord *mp, RgnHandle clipRgn, long refCon);
/*    Note that UInt8 *queuedFrameCount replaces Boolean *done. 0(==false) still means no frames, and 1(==true) one, 
    but if more than one are available the number should be returned here. The value 2 previously meant more than one frame,
    so some VDIGs may return 2 even if more than 2 are available, and some will still return 1 as they are using the original definition. */
typedef CALLBACK_API( ComponentResult , SGGrabCompressCompleteBottleProcPtr )(SGChannel c, UInt8 *queuedFrameCount, SGCompressInfo *ci, TimeRecord *t, long refCon);
typedef CALLBACK_API( ComponentResult , SGDisplayCompressBottleProcPtr )(SGChannel c, Ptr dataPtr, ImageDescriptionHandle desc, MatrixRecord *mp, RgnHandle clipRgn, long refCon);
typedef STACK_UPP_TYPE(SGGrabBottleProcPtr)                     SGGrabBottleUPP;
typedef STACK_UPP_TYPE(SGGrabCompleteBottleProcPtr)             SGGrabCompleteBottleUPP;
typedef STACK_UPP_TYPE(SGDisplayBottleProcPtr)                  SGDisplayBottleUPP;
typedef STACK_UPP_TYPE(SGCompressBottleProcPtr)                 SGCompressBottleUPP;
typedef STACK_UPP_TYPE(SGCompressCompleteBottleProcPtr)         SGCompressCompleteBottleUPP;
typedef STACK_UPP_TYPE(SGAddFrameBottleProcPtr)                 SGAddFrameBottleUPP;
typedef STACK_UPP_TYPE(SGTransferFrameBottleProcPtr)            SGTransferFrameBottleUPP;
typedef STACK_UPP_TYPE(SGGrabCompressCompleteBottleProcPtr)     SGGrabCompressCompleteBottleUPP;
typedef STACK_UPP_TYPE(SGDisplayCompressBottleProcPtr)          SGDisplayCompressBottleUPP;

struct VideoBottles {
    short                           procCount;
    SGGrabBottleUPP                 grabProc;
    SGGrabCompleteBottleUPP         grabCompleteProc;
    SGDisplayBottleUPP              displayProc;
    SGCompressBottleUPP             compressProc;
    SGCompressCompleteBottleUPP     compressCompleteProc;
    SGAddFrameBottleUPP             addFrameProc;
    SGTransferFrameBottleUPP        transferFrameProc;
    SGGrabCompressCompleteBottleUPP  grabCompressCompleteProc;
    SGDisplayCompressBottleUPP      displayCompressProc;
};
typedef struct VideoBottles             VideoBottles;
EXTERN_API( ComponentResult )
SGGetSrcVideoBounds             (SGChannel              c,
                                 Rect *                 r)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0100, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetVideoRect                  (SGChannel              c,
                                 const Rect *           r)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetVideoRect                  (SGChannel              c,
                                 Rect *                 r)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetVideoCompressorType        (SGChannel              c,
                                 OSType *               compressorType)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetVideoCompressorType        (SGChannel              c,
                                 OSType                 compressorType)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0104, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetVideoCompressor            (SGChannel              c,
                                 short                  depth,
                                 CompressorComponent    compressor,
                                 CodecQ                 spatialQuality,
                                 CodecQ                 temporalQuality,
                                 long                   keyFrameRate)                       FIVEWORDINLINE(0x2F3C, 0x0012, 0x0105, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetVideoCompressor            (SGChannel              c,
                                 short *                depth,
                                 CompressorComponent *  compressor,
                                 CodecQ *               spatialQuality,
                                 CodecQ *               temporalQuality,
                                 long *                 keyFrameRate)                       FIVEWORDINLINE(0x2F3C, 0x0014, 0x0106, 0x7000, 0xA82A);

EXTERN_API( ComponentInstance )
SGGetVideoDigitizerComponent    (SGChannel              c)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0107, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetVideoDigitizerComponent    (SGChannel              c,
                                 ComponentInstance      vdig)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0108, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGVideoDigitizerChanged         (SGChannel              c)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0109, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetVideoBottlenecks           (SGChannel              c,
                                 VideoBottles *         vb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x010A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetVideoBottlenecks           (SGChannel              c,
                                 VideoBottles *         vb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x010B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGrabFrame                     (SGChannel              c,
                                 short                  bufferNum)                          FIVEWORDINLINE(0x2F3C, 0x0002, 0x010C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGrabFrameComplete             (SGChannel              c,
                                 short                  bufferNum,
                                 Boolean *              done)                               FIVEWORDINLINE(0x2F3C, 0x0006, 0x010D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGDisplayFrame                  (SGChannel              c,
                                 short                  bufferNum,
                                 const MatrixRecord *   mp,
                                 RgnHandle              clipRgn)                            FIVEWORDINLINE(0x2F3C, 0x000A, 0x010E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGCompressFrame                 (SGChannel              c,
                                 short                  bufferNum)                          FIVEWORDINLINE(0x2F3C, 0x0002, 0x010F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGCompressFrameComplete         (SGChannel              c,
                                 short                  bufferNum,
                                 Boolean *              done,
                                 SGCompressInfo *       ci)                                 FIVEWORDINLINE(0x2F3C, 0x000A, 0x0110, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGAddFrame                      (SGChannel              c,
                                 short                  bufferNum,
                                 TimeValue              atTime,
                                 TimeScale              scale,
                                 const SGCompressInfo * ci)                                 FIVEWORDINLINE(0x2F3C, 0x000E, 0x0111, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGTransferFrameForCompress      (SGChannel              c,
                                 short                  bufferNum,
                                 const MatrixRecord *   mp,
                                 RgnHandle              clipRgn)                            FIVEWORDINLINE(0x2F3C, 0x000A, 0x0112, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetCompressBuffer             (SGChannel              c,
                                 short                  depth,
                                 const Rect *           compressSize)                       FIVEWORDINLINE(0x2F3C, 0x0006, 0x0113, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetCompressBuffer             (SGChannel              c,
                                 short *                depth,
                                 Rect *                 compressSize)                       FIVEWORDINLINE(0x2F3C, 0x0008, 0x0114, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetBufferInfo                 (SGChannel              c,
                                 short                  bufferNum,
                                 PixMapHandle *         bufferPM,
                                 Rect *                 bufferRect,
                                 GWorldPtr *            compressBuffer,
                                 Rect *                 compressBufferRect)                 FIVEWORDINLINE(0x2F3C, 0x0012, 0x0115, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetUseScreenBuffer            (SGChannel              c,
                                 Boolean                useScreenBuffer)                    FIVEWORDINLINE(0x2F3C, 0x0002, 0x0116, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetUseScreenBuffer            (SGChannel              c,
                                 Boolean *              useScreenBuffer)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0117, 0x7000, 0xA82A);

/*    Note that UInt8 *queuedFrameCount replaces Boolean *done. 0(==false) still means no frames, and 1(==true) one, 
    but if more than one are available the number should be returned here. The value 2 previously meant more than one frame,
    so some VDIGs may return 2 even if more than 2 are available, and some will still return 1 as they are using the original definition. */
EXTERN_API( ComponentResult )
SGGrabCompressComplete          (SGChannel              c,
                                 UInt8 *                queuedFrameCount,
                                 SGCompressInfo *       ci,
                                 TimeRecord *           tr)                                 FIVEWORDINLINE(0x2F3C, 0x000C, 0x0118, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGDisplayCompress               (SGChannel              c,
                                 Ptr                    dataPtr,
                                 ImageDescriptionHandle  desc,
                                 MatrixRecord *         mp,
                                 RgnHandle              clipRgn)                            FIVEWORDINLINE(0x2F3C, 0x0010, 0x0119, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetFrameRate                  (SGChannel              c,
                                 Fixed                  frameRate)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x011A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetFrameRate                  (SGChannel              c,
                                 Fixed *                frameRate)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x011B, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
SGSetPreferredPacketSize        (SGChannel              c,
                                 long                   preferredPacketSizeInBytes)         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0121, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetPreferredPacketSize        (SGChannel              c,
                                 long *                 preferredPacketSizeInBytes)         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0122, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetUserVideoCompressorList    (SGChannel              c,
                                 Handle                 compressorTypes)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0123, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetUserVideoCompressorList    (SGChannel              c,
                                 Handle *               compressorTypes)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0124, 0x7000, 0xA82A);

/*** Sequence Grab SOUND CHANNEL Component Stuff ***/

/*
    Sound stuff
*/
EXTERN_API( ComponentResult )
SGSetSoundInputDriver           (SGChannel              c,
                                 ConstStr255Param       driverName)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0100, 0x7000, 0xA82A);

EXTERN_API( long )
SGGetSoundInputDriver           (SGChannel              c)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSoundInputDriverChanged       (SGChannel              c)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetSoundRecordChunkSize       (SGChannel              c,
                                 long                   seconds)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0103, 0x7000, 0xA82A);

EXTERN_API( long )
SGGetSoundRecordChunkSize       (SGChannel              c)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0104, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetSoundInputRate             (SGChannel              c,
                                 Fixed                  rate)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0105, 0x7000, 0xA82A);

EXTERN_API( Fixed )
SGGetSoundInputRate             (SGChannel              c)                                  FIVEWORDINLINE(0x2F3C, 0x0000, 0x0106, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetSoundInputParameters       (SGChannel              c,
                                 short                  sampleSize,
                                 short                  numChannels,
                                 OSType                 compressionType)                    FIVEWORDINLINE(0x2F3C, 0x0008, 0x0107, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetSoundInputParameters       (SGChannel              c,
                                 short *                sampleSize,
                                 short *                numChannels,
                                 OSType *               compressionType)                    FIVEWORDINLINE(0x2F3C, 0x000C, 0x0108, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetAdditionalSoundRates       (SGChannel              c,
                                 Handle                 rates)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0109, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetAdditionalSoundRates       (SGChannel              c,
                                 Handle *               rates)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x010A, 0x7000, 0xA82A);

/*
    Text stuff
*/
EXTERN_API( ComponentResult )
SGSetFontName                   (SGChannel              c,
                                 StringPtr              pstr)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0100, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetFontSize                   (SGChannel              c,
                                 short                  fontSize)                           FIVEWORDINLINE(0x2F3C, 0x0002, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetTextForeColor              (SGChannel              c,
                                 RGBColor *             theColor)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetTextBackColor              (SGChannel              c,
                                 RGBColor *             theColor)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetJustification              (SGChannel              c,
                                 short                  just)                               FIVEWORDINLINE(0x2F3C, 0x0002, 0x0104, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGGetTextReturnToSpaceValue     (SGChannel              c,
                                 short *                rettospace)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0105, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetTextReturnToSpaceValue     (SGChannel              c,
                                 short                  rettospace)                         FIVEWORDINLINE(0x2F3C, 0x0002, 0x0106, 0x7000, 0xA82A);

/*
    Music stuff
*/
EXTERN_API( ComponentResult )
SGGetInstrument                 (SGChannel              c,
                                 ToneDescription *      td)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0100, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SGSetInstrument                 (SGChannel              c,
                                 ToneDescription *      td)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0101, 0x7000, 0xA82A);


enum {
    sgChannelAtom               = FOUR_CHAR_CODE('chan'),
    sgChannelSettingsAtom       = FOUR_CHAR_CODE('ctom'),
    sgChannelDescription        = FOUR_CHAR_CODE('cdsc'),
    sgChannelSettings           = FOUR_CHAR_CODE('cset')
};

enum {
    sgDeviceNameType            = FOUR_CHAR_CODE('name'),
    sgDeviceDisplayNameType     = FOUR_CHAR_CODE('dnam'),
    sgDeviceUIDType             = FOUR_CHAR_CODE('duid'),
    sgInputUIDType              = FOUR_CHAR_CODE('iuid'),
    sgUsageType                 = FOUR_CHAR_CODE('use '),
    sgPlayFlagsType             = FOUR_CHAR_CODE('plyf'),
    sgClipType                  = FOUR_CHAR_CODE('clip'),
    sgMatrixType                = FOUR_CHAR_CODE('mtrx'),
    sgVolumeType                = FOUR_CHAR_CODE('volu')
};

enum {
    sgPanelSettingsAtom         = FOUR_CHAR_CODE('ptom'),
    sgPanelDescription          = FOUR_CHAR_CODE('pdsc'),
    sgPanelSettings             = FOUR_CHAR_CODE('pset')
};

enum {
    sgcSoundCompressionType     = FOUR_CHAR_CODE('scmp'),
    sgcSoundCodecSettingsType   = FOUR_CHAR_CODE('cdec'),
    sgcSoundSampleRateType      = FOUR_CHAR_CODE('srat'),
    sgcSoundChannelCountType    = FOUR_CHAR_CODE('schn'),
    sgcSoundSampleSizeType      = FOUR_CHAR_CODE('ssiz'),
    sgcSoundInputType           = FOUR_CHAR_CODE('sinp'),
    sgcSoundGainType            = FOUR_CHAR_CODE('gain')
};

enum {
    sgcVideoHueType             = FOUR_CHAR_CODE('hue '),
    sgcVideoSaturationType      = FOUR_CHAR_CODE('satr'),
    sgcVideoContrastType        = FOUR_CHAR_CODE('trst'),
    sgcVideoSharpnessType       = FOUR_CHAR_CODE('shrp'),
    sgcVideoBrigtnessType       = FOUR_CHAR_CODE('brit'),
    sgcVideoBlackLevelType      = FOUR_CHAR_CODE('blkl'),
    sgcVideoWhiteLevelType      = FOUR_CHAR_CODE('whtl'),
    sgcVideoInputType           = FOUR_CHAR_CODE('vinp'),
    sgcVideoFormatType          = FOUR_CHAR_CODE('vstd'),
    sgcVideoFilterType          = FOUR_CHAR_CODE('vflt'),
    sgcVideoRectType            = FOUR_CHAR_CODE('vrct'),
    sgcVideoDigitizerType       = FOUR_CHAR_CODE('vdig')
};





typedef ComponentInstance               QTVideoOutputComponent;
/* Component type and subtype enumerations*/
enum {
    QTVideoOutputComponentType  = FOUR_CHAR_CODE('vout'),
    QTVideoOutputComponentBaseSubType = FOUR_CHAR_CODE('base')
};


/* QTVideoOutput Component flags*/

enum {
    kQTVideoOutputDontDisplayToUser = 1L << 0
};

/* Display mode atom types*/

enum {
    kQTVODisplayModeItem        = FOUR_CHAR_CODE('qdmi'),
    kQTVODimensions             = FOUR_CHAR_CODE('dimn'),       /* atom contains two longs - pixel count - width, height*/
    kQTVOResolution             = FOUR_CHAR_CODE('resl'),       /* atom contains two Fixed - hRes, vRes in dpi*/
    kQTVORefreshRate            = FOUR_CHAR_CODE('refr'),       /* atom contains one Fixed - refresh rate in Hz*/
    kQTVOPixelType              = FOUR_CHAR_CODE('pixl'),       /* atom contains one OSType - pixel format of mode*/
    kQTVOName                   = FOUR_CHAR_CODE('name'),       /* atom contains string (no length byte) - name of mode for display to user*/
    kQTVODecompressors          = FOUR_CHAR_CODE('deco'),       /* atom contains other atoms indicating supported decompressors*/
                                                                /* kQTVODecompressors sub-atoms*/
    kQTVODecompressorType       = FOUR_CHAR_CODE('dety'),       /* atom contains one OSType - decompressor type code*/
    kQTVODecompressorContinuous = FOUR_CHAR_CODE('cont'),       /* atom contains one Boolean - true if this type is displayed continuously*/
    kQTVODecompressorComponent  = FOUR_CHAR_CODE('cmpt')        /* atom contains one Component - component id of decompressor*/
};

/** These are QTVideoOutput procedures **/
EXTERN_API( ComponentResult )
QTVideoOutputGetDisplayModeList (QTVideoOutputComponent  vo,
                                 QTAtomContainer *      outputs)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputGetCurrentClientName (QTVideoOutputComponent  vo,
                                 Str255                 str)                                FIVEWORDINLINE(0x2F3C, 0x0004, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputSetClientName      (QTVideoOutputComponent  vo,
                                 ConstStr255Param       str)                                FIVEWORDINLINE(0x2F3C, 0x0004, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputGetClientName      (QTVideoOutputComponent  vo,
                                 Str255                 str)                                FIVEWORDINLINE(0x2F3C, 0x0004, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputBegin              (QTVideoOutputComponent  vo)                                FIVEWORDINLINE(0x2F3C, 0x0000, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputEnd                (QTVideoOutputComponent  vo)                                FIVEWORDINLINE(0x2F3C, 0x0000, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputSetDisplayMode     (QTVideoOutputComponent  vo,
                                 long                   displayModeID)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputGetDisplayMode     (QTVideoOutputComponent  vo,
                                 long *                 displayModeID)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x0008, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputCustomConfigureDisplay (QTVideoOutputComponent  vo,
                                 ModalFilterUPP         filter)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0009, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputSaveState          (QTVideoOutputComponent  vo,
                                 QTAtomContainer *      state)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x000A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputRestoreState       (QTVideoOutputComponent  vo,
                                 QTAtomContainer        state)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputGetGWorld          (QTVideoOutputComponent  vo,
                                 GWorldPtr *            gw)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x000C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputGetGWorldParameters (QTVideoOutputComponent  vo,
                                 Ptr *                  baseAddr,
                                 long *                 rowBytes,
                                 CTabHandle *           colorTable)                         FIVEWORDINLINE(0x2F3C, 0x000C, 0x000D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputGetIndSoundOutput  (QTVideoOutputComponent  vo,
                                 long                   index,
                                 Component *            outputComponent)                    FIVEWORDINLINE(0x2F3C, 0x0008, 0x000E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputGetClock           (QTVideoOutputComponent  vo,
                                 ComponentInstance *    clock)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x000F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputSetEchoPort        (QTVideoOutputComponent  vo,
                                 CGrafPtr               echoPort)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputGetIndImageDecompressor (QTVideoOutputComponent  vo,
                                 long                   index,
                                 Component *            codec)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0011, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTVideoOutputBaseSetEchoPort    (QTVideoOutputComponent  vo,
                                 CGrafPtr               echoPort)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0012, 0x7000, 0xA82A);


/* UPP call backs */
#if OPAQUE_UPP_TYPES
    EXTERN_API(DataHCompletionUPP)
    NewDataHCompletionUPP          (DataHCompletionProcPtr  userRoutine);

    EXTERN_API(VdigIntUPP)
    NewVdigIntUPP                  (VdigIntProcPtr          userRoutine);

    EXTERN_API(StartDocumentHandlerUPP)
    NewStartDocumentHandlerUPP     (StartDocumentHandler    userRoutine);

    EXTERN_API(EndDocumentHandlerUPP)
    NewEndDocumentHandlerUPP       (EndDocumentHandler      userRoutine);

    EXTERN_API(StartElementHandlerUPP)
    NewStartElementHandlerUPP      (StartElementHandler     userRoutine);

    EXTERN_API(EndElementHandlerUPP)
    NewEndElementHandlerUPP        (EndElementHandler       userRoutine);

    EXTERN_API(CharDataHandlerUPP)
    NewCharDataHandlerUPP          (CharDataHandler         userRoutine);

    EXTERN_API(PreprocessInstructionHandlerUPP)
    NewPreprocessInstructionHandlerUPP    (PreprocessInstructionHandler userRoutine);

    EXTERN_API(CommentHandlerUPP)
    NewCommentHandlerUPP           (CommentHandler          userRoutine);

    EXTERN_API(CDataHandlerUPP)
    NewCDataHandlerUPP             (CDataHandler            userRoutine);

    EXTERN_API(SGDataUPP)
    NewSGDataUPP                   (SGDataProcPtr           userRoutine);

    EXTERN_API(SGModalFilterUPP)
    NewSGModalFilterUPP            (SGModalFilterProcPtr    userRoutine);

    EXTERN_API(SGGrabBottleUPP)
    NewSGGrabBottleUPP             (SGGrabBottleProcPtr     userRoutine);

    EXTERN_API(SGGrabCompleteBottleUPP)
    NewSGGrabCompleteBottleUPP     (SGGrabCompleteBottleProcPtr userRoutine);

    EXTERN_API(SGDisplayBottleUPP)
    NewSGDisplayBottleUPP          (SGDisplayBottleProcPtr  userRoutine);

    EXTERN_API(SGCompressBottleUPP)
    NewSGCompressBottleUPP         (SGCompressBottleProcPtr userRoutine);

    EXTERN_API(SGCompressCompleteBottleUPP)
    NewSGCompressCompleteBottleUPP    (SGCompressCompleteBottleProcPtr userRoutine);

    EXTERN_API(SGAddFrameBottleUPP)
    NewSGAddFrameBottleUPP         (SGAddFrameBottleProcPtr userRoutine);

    EXTERN_API(SGTransferFrameBottleUPP)
    NewSGTransferFrameBottleUPP    (SGTransferFrameBottleProcPtr userRoutine);

    EXTERN_API(SGGrabCompressCompleteBottleUPP)
    NewSGGrabCompressCompleteBottleUPP    (SGGrabCompressCompleteBottleProcPtr userRoutine);

    EXTERN_API(SGDisplayCompressBottleUPP)
    NewSGDisplayCompressBottleUPP    (SGDisplayCompressBottleProcPtr userRoutine);

    EXTERN_API(void)
    DisposeDataHCompletionUPP      (DataHCompletionUPP      userUPP);

    EXTERN_API(void)
    DisposeVdigIntUPP              (VdigIntUPP              userUPP);

    EXTERN_API(void)
    DisposeStartDocumentHandlerUPP    (StartDocumentHandlerUPP userUPP);

    EXTERN_API(void)
    DisposeEndDocumentHandlerUPP    (EndDocumentHandlerUPP  userUPP);

    EXTERN_API(void)
    DisposeStartElementHandlerUPP    (StartElementHandlerUPP userUPP);

    EXTERN_API(void)
    DisposeEndElementHandlerUPP    (EndElementHandlerUPP    userUPP);

    EXTERN_API(void)
    DisposeCharDataHandlerUPP      (CharDataHandlerUPP      userUPP);

    EXTERN_API(void)
    DisposePreprocessInstructionHandlerUPP    (PreprocessInstructionHandlerUPP userUPP);

    EXTERN_API(void)
    DisposeCommentHandlerUPP       (CommentHandlerUPP       userUPP);

    EXTERN_API(void)
    DisposeCDataHandlerUPP         (CDataHandlerUPP         userUPP);

    EXTERN_API(void)
    DisposeSGDataUPP               (SGDataUPP               userUPP);

    EXTERN_API(void)
    DisposeSGModalFilterUPP        (SGModalFilterUPP        userUPP);

    EXTERN_API(void)
    DisposeSGGrabBottleUPP         (SGGrabBottleUPP         userUPP);

    EXTERN_API(void)
    DisposeSGGrabCompleteBottleUPP    (SGGrabCompleteBottleUPP userUPP);

    EXTERN_API(void)
    DisposeSGDisplayBottleUPP      (SGDisplayBottleUPP      userUPP);

    EXTERN_API(void)
    DisposeSGCompressBottleUPP     (SGCompressBottleUPP     userUPP);

    EXTERN_API(void)
    DisposeSGCompressCompleteBottleUPP    (SGCompressCompleteBottleUPP userUPP);

    EXTERN_API(void)
    DisposeSGAddFrameBottleUPP     (SGAddFrameBottleUPP     userUPP);

    EXTERN_API(void)
    DisposeSGTransferFrameBottleUPP    (SGTransferFrameBottleUPP userUPP);

    EXTERN_API(void)
    DisposeSGGrabCompressCompleteBottleUPP    (SGGrabCompressCompleteBottleUPP userUPP);

    EXTERN_API(void)
    DisposeSGDisplayCompressBottleUPP    (SGDisplayCompressBottleUPP userUPP);

    EXTERN_API(void)
    InvokeDataHCompletionUPP       (Ptr                     request,
                                    long                    refcon,
                                    OSErr                   err,
                                    DataHCompletionUPP      userUPP);

    EXTERN_API(void)
    InvokeVdigIntUPP               (long                    flags,
                                    long                    refcon,
                                    VdigIntUPP              userUPP);

    EXTERN_API(ComponentResult)
    InvokeStartDocumentHandlerUPP    (long                  refcon,
                                    StartDocumentHandlerUPP userUPP);

    EXTERN_API(ComponentResult)
    InvokeEndDocumentHandlerUPP    (long                    refcon,
                                    EndDocumentHandlerUPP   userUPP);

    EXTERN_API(ComponentResult)
    InvokeStartElementHandlerUPP    (const char *           name,
                                    const char *const       atts[],
                                    long                    refcon,
                                    StartElementHandlerUPP  userUPP);

    EXTERN_API(ComponentResult)
    InvokeEndElementHandlerUPP     (const char *            name,
                                    long                    refcon,
                                    EndElementHandlerUPP    userUPP);

    EXTERN_API(ComponentResult)
    InvokeCharDataHandlerUPP       (const char *            charData,
                                    long                    refcon,
                                    CharDataHandlerUPP      userUPP);

    EXTERN_API(ComponentResult)
    InvokePreprocessInstructionHandlerUPP    (const char *  name,
                                    const char *const       atts[],
                                    long                    refcon,
                                    PreprocessInstructionHandlerUPP userUPP);

    EXTERN_API(ComponentResult)
    InvokeCommentHandlerUPP        (const char *            comment,
                                    long                    refcon,
                                    CommentHandlerUPP       userUPP);

    EXTERN_API(ComponentResult)
    InvokeCDataHandlerUPP          (const char *            cdata,
                                    long                    refcon,
                                    CDataHandlerUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeSGDataUPP                (SGChannel               c,
                                    Ptr                     p,
                                    long                    len,
                                    long *                  offset,
                                    long                    chRefCon,
                                    TimeValue               time,
                                    short                   writeType,
                                    long                    refCon,
                                    SGDataUPP               userUPP);

    EXTERN_API(Boolean)
    InvokeSGModalFilterUPP         (DialogRef               theDialog,
                                    const EventRecord *     theEvent,
                                    short *                 itemHit,
                                    long                    refCon,
                                    SGModalFilterUPP        userUPP);

    EXTERN_API(ComponentResult)
    InvokeSGGrabBottleUPP          (SGChannel               c,
                                    short                   bufferNum,
                                    long                    refCon,
                                    SGGrabBottleUPP         userUPP);

    EXTERN_API(ComponentResult)
    InvokeSGGrabCompleteBottleUPP    (SGChannel             c,
                                    short                   bufferNum,
                                    Boolean *               done,
                                    long                    refCon,
                                    SGGrabCompleteBottleUPP userUPP);

    EXTERN_API(ComponentResult)
    InvokeSGDisplayBottleUPP       (SGChannel               c,
                                    short                   bufferNum,
                                    MatrixRecord *          mp,
                                    RgnHandle               clipRgn,
                                    long                    refCon,
                                    SGDisplayBottleUPP      userUPP);

    EXTERN_API(ComponentResult)
    InvokeSGCompressBottleUPP      (SGChannel               c,
                                    short                   bufferNum,
                                    long                    refCon,
                                    SGCompressBottleUPP     userUPP);

    EXTERN_API(ComponentResult)
    InvokeSGCompressCompleteBottleUPP    (SGChannel         c,
                                    short                   bufferNum,
                                    Boolean *               done,
                                    SGCompressInfo *        ci,
                                    long                    refCon,
                                    SGCompressCompleteBottleUPP userUPP);

    EXTERN_API(ComponentResult)
    InvokeSGAddFrameBottleUPP      (SGChannel               c,
                                    short                   bufferNum,
                                    TimeValue               atTime,
                                    TimeScale               scale,
                                    const SGCompressInfo *  ci,
                                    long                    refCon,
                                    SGAddFrameBottleUPP     userUPP);

    EXTERN_API(ComponentResult)
    InvokeSGTransferFrameBottleUPP    (SGChannel            c,
                                    short                   bufferNum,
                                    MatrixRecord *          mp,
                                    RgnHandle               clipRgn,
                                    long                    refCon,
                                    SGTransferFrameBottleUPP userUPP);

    EXTERN_API(ComponentResult)
    InvokeSGGrabCompressCompleteBottleUPP    (SGChannel     c,
                                    UInt8 *                 queuedFrameCount,
                                    SGCompressInfo *        ci,
                                    TimeRecord *            t,
                                    long                    refCon,
                                    SGGrabCompressCompleteBottleUPP userUPP);

    EXTERN_API(ComponentResult)
    InvokeSGDisplayCompressBottleUPP    (SGChannel          c,
                                    Ptr                     dataPtr,
                                    ImageDescriptionHandle  desc,
                                    MatrixRecord *          mp,
                                    RgnHandle               clipRgn,
                                    long                    refCon,
                                    SGDisplayCompressBottleUPP userUPP);

#else
    enum { uppDataHCompletionProcInfo = 0x00000BC0 };               /* pascal no_return_value Func(4_bytes, 4_bytes, 2_bytes) */
    enum { uppVdigIntProcInfo = 0x000003C0 };                       /* pascal no_return_value Func(4_bytes, 4_bytes) */
    enum { uppStartDocumentHandlerProcInfo = 0x000000F0 };          /* pascal 4_bytes Func(4_bytes) */
    enum { uppEndDocumentHandlerProcInfo = 0x000000F0 };            /* pascal 4_bytes Func(4_bytes) */
    enum { uppStartElementHandlerProcInfo = 0x00000FF0 };           /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppEndElementHandlerProcInfo = 0x000003F0 };             /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppCharDataHandlerProcInfo = 0x000003F0 };               /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppPreprocessInstructionHandlerProcInfo = 0x00000FF0 };  /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppCommentHandlerProcInfo = 0x000003F0 };                /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppCDataHandlerProcInfo = 0x000003F0 };                  /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppSGDataProcInfo = 0x003BFFE0 };                        /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 2_bytes, 4_bytes) */
    enum { uppSGModalFilterProcInfo = 0x00003FD0 };                 /* pascal 1_byte Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppSGGrabBottleProcInfo = 0x00000EF0 };                  /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes) */
    enum { uppSGGrabCompleteBottleProcInfo = 0x00003EF0 };          /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes) */
    enum { uppSGDisplayBottleProcInfo = 0x0000FEF0 };               /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppSGCompressBottleProcInfo = 0x00000EF0 };              /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes) */
    enum { uppSGCompressCompleteBottleProcInfo = 0x0000FEF0 };      /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppSGAddFrameBottleProcInfo = 0x0003FEF0 };              /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppSGTransferFrameBottleProcInfo = 0x0000FEF0 };         /* pascal 4_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppSGGrabCompressCompleteBottleProcInfo = 0x0000FFF0 };  /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppSGDisplayCompressBottleProcInfo = 0x0003FFF0 };       /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewDataHCompletionUPP(userRoutine)                      (DataHCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDataHCompletionProcInfo, GetCurrentArchitecture())
    #define NewVdigIntUPP(userRoutine)                              (VdigIntUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppVdigIntProcInfo, GetCurrentArchitecture())
    #define NewStartDocumentHandlerUPP(userRoutine)                 (StartDocumentHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppStartDocumentHandlerProcInfo, GetCurrentArchitecture())
    #define NewEndDocumentHandlerUPP(userRoutine)                   (EndDocumentHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppEndDocumentHandlerProcInfo, GetCurrentArchitecture())
    #define NewStartElementHandlerUPP(userRoutine)                  (StartElementHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppStartElementHandlerProcInfo, GetCurrentArchitecture())
    #define NewEndElementHandlerUPP(userRoutine)                    (EndElementHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppEndElementHandlerProcInfo, GetCurrentArchitecture())
    #define NewCharDataHandlerUPP(userRoutine)                      (CharDataHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCharDataHandlerProcInfo, GetCurrentArchitecture())
    #define NewPreprocessInstructionHandlerUPP(userRoutine)         (PreprocessInstructionHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppPreprocessInstructionHandlerProcInfo, GetCurrentArchitecture())
    #define NewCommentHandlerUPP(userRoutine)                       (CommentHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCommentHandlerProcInfo, GetCurrentArchitecture())
    #define NewCDataHandlerUPP(userRoutine)                         (CDataHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppCDataHandlerProcInfo, GetCurrentArchitecture())
    #define NewSGDataUPP(userRoutine)                               (SGDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSGDataProcInfo, GetCurrentArchitecture())
    #define NewSGModalFilterUPP(userRoutine)                        (SGModalFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSGModalFilterProcInfo, GetCurrentArchitecture())
    #define NewSGGrabBottleUPP(userRoutine)                         (SGGrabBottleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSGGrabBottleProcInfo, GetCurrentArchitecture())
    #define NewSGGrabCompleteBottleUPP(userRoutine)                 (SGGrabCompleteBottleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSGGrabCompleteBottleProcInfo, GetCurrentArchitecture())
    #define NewSGDisplayBottleUPP(userRoutine)                      (SGDisplayBottleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSGDisplayBottleProcInfo, GetCurrentArchitecture())
    #define NewSGCompressBottleUPP(userRoutine)                     (SGCompressBottleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSGCompressBottleProcInfo, GetCurrentArchitecture())
    #define NewSGCompressCompleteBottleUPP(userRoutine)             (SGCompressCompleteBottleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSGCompressCompleteBottleProcInfo, GetCurrentArchitecture())
    #define NewSGAddFrameBottleUPP(userRoutine)                     (SGAddFrameBottleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSGAddFrameBottleProcInfo, GetCurrentArchitecture())
    #define NewSGTransferFrameBottleUPP(userRoutine)                (SGTransferFrameBottleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSGTransferFrameBottleProcInfo, GetCurrentArchitecture())
    #define NewSGGrabCompressCompleteBottleUPP(userRoutine)         (SGGrabCompressCompleteBottleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSGGrabCompressCompleteBottleProcInfo, GetCurrentArchitecture())
    #define NewSGDisplayCompressBottleUPP(userRoutine)              (SGDisplayCompressBottleUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSGDisplayCompressBottleProcInfo, GetCurrentArchitecture())
    #define DisposeDataHCompletionUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeVdigIntUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define DisposeStartDocumentHandlerUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define DisposeEndDocumentHandlerUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define DisposeStartElementHandlerUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define DisposeEndElementHandlerUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposeCharDataHandlerUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposePreprocessInstructionHandlerUPP(userUPP)         DisposeRoutineDescriptor(userUPP)
    #define DisposeCommentHandlerUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define DisposeCDataHandlerUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define DisposeSGDataUPP(userUPP)                               DisposeRoutineDescriptor(userUPP)
    #define DisposeSGModalFilterUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeSGGrabBottleUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define DisposeSGGrabCompleteBottleUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define DisposeSGDisplayBottleUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeSGCompressBottleUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposeSGCompressCompleteBottleUPP(userUPP)             DisposeRoutineDescriptor(userUPP)
    #define DisposeSGAddFrameBottleUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposeSGTransferFrameBottleUPP(userUPP)                DisposeRoutineDescriptor(userUPP)
    #define DisposeSGGrabCompressCompleteBottleUPP(userUPP)         DisposeRoutineDescriptor(userUPP)
    #define DisposeSGDisplayCompressBottleUPP(userUPP)              DisposeRoutineDescriptor(userUPP)
    #define InvokeDataHCompletionUPP(request, refcon, err, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppDataHCompletionProcInfo, (request), (refcon), (err))
    #define InvokeVdigIntUPP(flags, refcon, userUPP)                CALL_TWO_PARAMETER_UPP((userUPP), uppVdigIntProcInfo, (flags), (refcon))
    #define InvokeStartDocumentHandlerUPP(refcon, userUPP)          (ComponentResult)CALL_ONE_PARAMETER_UPP((userUPP), uppStartDocumentHandlerProcInfo, (refcon))
    #define InvokeEndDocumentHandlerUPP(refcon, userUPP)            (ComponentResult)CALL_ONE_PARAMETER_UPP((userUPP), uppEndDocumentHandlerProcInfo, (refcon))
    #define InvokeStartElementHandlerUPP(name, atts, refcon, userUPP)  (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppStartElementHandlerProcInfo, (name), (atts), (refcon))
    #define InvokeEndElementHandlerUPP(name, refcon, userUPP)       (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppEndElementHandlerProcInfo, (name), (refcon))
    #define InvokeCharDataHandlerUPP(charData, refcon, userUPP)     (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppCharDataHandlerProcInfo, (charData), (refcon))
    #define InvokePreprocessInstructionHandlerUPP(name, atts, refcon, userUPP)  (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppPreprocessInstructionHandlerProcInfo, (name), (atts), (refcon))
    #define InvokeCommentHandlerUPP(comment, refcon, userUPP)       (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppCommentHandlerProcInfo, (comment), (refcon))
    #define InvokeCDataHandlerUPP(cdata, refcon, userUPP)           (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppCDataHandlerProcInfo, (cdata), (refcon))
    #define InvokeSGDataUPP(c, p, len, offset, chRefCon, time, writeType, refCon, userUPP)  (OSErr)CALL_EIGHT_PARAMETER_UPP((userUPP), uppSGDataProcInfo, (c), (p), (len), (offset), (chRefCon), (time), (writeType), (refCon))
    #define InvokeSGModalFilterUPP(theDialog, theEvent, itemHit, refCon, userUPP)  (Boolean)CALL_FOUR_PARAMETER_UPP((userUPP), uppSGModalFilterProcInfo, (theDialog), (theEvent), (itemHit), (refCon))
    #define InvokeSGGrabBottleUPP(c, bufferNum, refCon, userUPP)    (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppSGGrabBottleProcInfo, (c), (bufferNum), (refCon))
    #define InvokeSGGrabCompleteBottleUPP(c, bufferNum, done, refCon, userUPP)  (ComponentResult)CALL_FOUR_PARAMETER_UPP((userUPP), uppSGGrabCompleteBottleProcInfo, (c), (bufferNum), (done), (refCon))
    #define InvokeSGDisplayBottleUPP(c, bufferNum, mp, clipRgn, refCon, userUPP)  (ComponentResult)CALL_FIVE_PARAMETER_UPP((userUPP), uppSGDisplayBottleProcInfo, (c), (bufferNum), (mp), (clipRgn), (refCon))
    #define InvokeSGCompressBottleUPP(c, bufferNum, refCon, userUPP)  (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppSGCompressBottleProcInfo, (c), (bufferNum), (refCon))
    #define InvokeSGCompressCompleteBottleUPP(c, bufferNum, done, ci, refCon, userUPP)  (ComponentResult)CALL_FIVE_PARAMETER_UPP((userUPP), uppSGCompressCompleteBottleProcInfo, (c), (bufferNum), (done), (ci), (refCon))
    #define InvokeSGAddFrameBottleUPP(c, bufferNum, atTime, scale, ci, refCon, userUPP)  (ComponentResult)CALL_SIX_PARAMETER_UPP((userUPP), uppSGAddFrameBottleProcInfo, (c), (bufferNum), (atTime), (scale), (ci), (refCon))
    #define InvokeSGTransferFrameBottleUPP(c, bufferNum, mp, clipRgn, refCon, userUPP)  (ComponentResult)CALL_FIVE_PARAMETER_UPP((userUPP), uppSGTransferFrameBottleProcInfo, (c), (bufferNum), (mp), (clipRgn), (refCon))
    #define InvokeSGGrabCompressCompleteBottleUPP(c, queuedFrameCount, ci, t, refCon, userUPP)  (ComponentResult)CALL_FIVE_PARAMETER_UPP((userUPP), uppSGGrabCompressCompleteBottleProcInfo, (c), (queuedFrameCount), (ci), (t), (refCon))
    #define InvokeSGDisplayCompressBottleUPP(c, dataPtr, desc, mp, clipRgn, refCon, userUPP)  (ComponentResult)CALL_SIX_PARAMETER_UPP((userUPP), uppSGDisplayCompressBottleProcInfo, (c), (dataPtr), (desc), (mp), (clipRgn), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewDataHCompletionProc(userRoutine)                     NewDataHCompletionUPP(userRoutine)
#define NewVdigIntProc(userRoutine)                             NewVdigIntUPP(userRoutine)
#define NewStartDocumentHandlerProc(userRoutine)                NewStartDocumentHandlerUPP(userRoutine)
#define NewEndDocumentHandlerProc(userRoutine)                  NewEndDocumentHandlerUPP(userRoutine)
#define NewStartElementHandlerProc(userRoutine)                 NewStartElementHandlerUPP(userRoutine)
#define NewEndElementHandlerProc(userRoutine)                   NewEndElementHandlerUPP(userRoutine)
#define NewCharDataHandlerProc(userRoutine)                     NewCharDataHandlerUPP(userRoutine)
#define NewPreprocessInstructionHandlerProc(userRoutine)        NewPreprocessInstructionHandlerUPP(userRoutine)
#define NewCommentHandlerProc(userRoutine)                      NewCommentHandlerUPP(userRoutine)
#define NewCDataHandlerProc(userRoutine)                        NewCDataHandlerUPP(userRoutine)
#define NewSGDataProc(userRoutine)                              NewSGDataUPP(userRoutine)
#define NewSGModalFilterProc(userRoutine)                       NewSGModalFilterUPP(userRoutine)
#define NewSGGrabBottleProc(userRoutine)                        NewSGGrabBottleUPP(userRoutine)
#define NewSGGrabCompleteBottleProc(userRoutine)                NewSGGrabCompleteBottleUPP(userRoutine)
#define NewSGDisplayBottleProc(userRoutine)                     NewSGDisplayBottleUPP(userRoutine)
#define NewSGCompressBottleProc(userRoutine)                    NewSGCompressBottleUPP(userRoutine)
#define NewSGCompressCompleteBottleProc(userRoutine)            NewSGCompressCompleteBottleUPP(userRoutine)
#define NewSGAddFrameBottleProc(userRoutine)                    NewSGAddFrameBottleUPP(userRoutine)
#define NewSGTransferFrameBottleProc(userRoutine)               NewSGTransferFrameBottleUPP(userRoutine)
#define NewSGGrabCompressCompleteBottleProc(userRoutine)        NewSGGrabCompressCompleteBottleUPP(userRoutine)
#define NewSGDisplayCompressBottleProc(userRoutine)             NewSGDisplayCompressBottleUPP(userRoutine)
#define CallDataHCompletionProc(userRoutine, request, refcon, err) InvokeDataHCompletionUPP(request, refcon, err, userRoutine)
#define CallVdigIntProc(userRoutine, flags, refcon)             InvokeVdigIntUPP(flags, refcon, userRoutine)
#define CallStartDocumentHandlerProc(userRoutine, refcon)       InvokeStartDocumentHandlerUPP(refcon, userRoutine)
#define CallEndDocumentHandlerProc(userRoutine, refcon)         InvokeEndDocumentHandlerUPP(refcon, userRoutine)
#define CallStartElementHandlerProc(userRoutine, name, atts, refcon) InvokeStartElementHandlerUPP(name, atts, refcon, userRoutine)
#define CallEndElementHandlerProc(userRoutine, name, refcon)    InvokeEndElementHandlerUPP(name, refcon, userRoutine)
#define CallCharDataHandlerProc(userRoutine, charData, refcon)  InvokeCharDataHandlerUPP(charData, refcon, userRoutine)
#define CallPreprocessInstructionHandlerProc(userRoutine, name, atts, refcon) InvokePreprocessInstructionHandlerUPP(name, atts, refcon, userRoutine)
#define CallCommentHandlerProc(userRoutine, comment, refcon)    InvokeCommentHandlerUPP(comment, refcon, userRoutine)
#define CallCDataHandlerProc(userRoutine, cdata, refcon)        InvokeCDataHandlerUPP(cdata, refcon, userRoutine)
#define CallSGDataProc(userRoutine, c, p, len, offset, chRefCon, time, writeType, refCon) InvokeSGDataUPP(c, p, len, offset, chRefCon, time, writeType, refCon, userRoutine)
#define CallSGModalFilterProc(userRoutine, theDialog, theEvent, itemHit, refCon) InvokeSGModalFilterUPP(theDialog, theEvent, itemHit, refCon, userRoutine)
#define CallSGGrabBottleProc(userRoutine, c, bufferNum, refCon) InvokeSGGrabBottleUPP(c, bufferNum, refCon, userRoutine)
#define CallSGGrabCompleteBottleProc(userRoutine, c, bufferNum, done, refCon) InvokeSGGrabCompleteBottleUPP(c, bufferNum, done, refCon, userRoutine)
#define CallSGDisplayBottleProc(userRoutine, c, bufferNum, mp, clipRgn, refCon) InvokeSGDisplayBottleUPP(c, bufferNum, mp, clipRgn, refCon, userRoutine)
#define CallSGCompressBottleProc(userRoutine, c, bufferNum, refCon) InvokeSGCompressBottleUPP(c, bufferNum, refCon, userRoutine)
#define CallSGCompressCompleteBottleProc(userRoutine, c, bufferNum, done, ci, refCon) InvokeSGCompressCompleteBottleUPP(c, bufferNum, done, ci, refCon, userRoutine)
#define CallSGAddFrameBottleProc(userRoutine, c, bufferNum, atTime, scale, ci, refCon) InvokeSGAddFrameBottleUPP(c, bufferNum, atTime, scale, ci, refCon, userRoutine)
#define CallSGTransferFrameBottleProc(userRoutine, c, bufferNum, mp, clipRgn, refCon) InvokeSGTransferFrameBottleUPP(c, bufferNum, mp, clipRgn, refCon, userRoutine)
#define CallSGGrabCompressCompleteBottleProc(userRoutine, c, queuedFrameCount, ci, t, refCon) InvokeSGGrabCompressCompleteBottleUPP(c, queuedFrameCount, ci, t, refCon, userRoutine)
#define CallSGDisplayCompressBottleProc(userRoutine, c, dataPtr, desc, mp, clipRgn, refCon) InvokeSGDisplayCompressBottleUPP(c, dataPtr, desc, mp, clipRgn, refCon, userRoutine)

/* selectors for component calls */
enum {
    kClockGetTimeSelect                        = 0x0001,
    kClockNewCallBackSelect                    = 0x0002,
    kClockDisposeCallBackSelect                = 0x0003,
    kClockCallMeWhenSelect                     = 0x0004,
    kClockCancelCallBackSelect                 = 0x0005,
    kClockRateChangedSelect                    = 0x0006,
    kClockTimeChangedSelect                    = 0x0007,
    kClockSetTimeBaseSelect                    = 0x0008,
    kClockStartStopChangedSelect               = 0x0009,
    kClockGetRateSelect                        = 0x000A,
    kSCGetCompressionExtendedSelect            = 0x0001,
    kSCPositionRectSelect                      = 0x0002,
    kSCPositionDialogSelect                    = 0x0003,
    kSCSetTestImagePictHandleSelect            = 0x0004,
    kSCSetTestImagePictFileSelect              = 0x0005,
    kSCSetTestImagePixMapSelect                = 0x0006,
    kSCGetBestDeviceRectSelect                 = 0x0007,
    kSCRequestImageSettingsSelect              = 0x000A,
    kSCCompressImageSelect                     = 0x000B,
    kSCCompressPictureSelect                   = 0x000C,
    kSCCompressPictureFileSelect               = 0x000D,
    kSCRequestSequenceSettingsSelect           = 0x000E,
    kSCCompressSequenceBeginSelect             = 0x000F,
    kSCCompressSequenceFrameSelect             = 0x0010,
    kSCCompressSequenceEndSelect               = 0x0011,
    kSCDefaultPictHandleSettingsSelect         = 0x0012,
    kSCDefaultPictFileSettingsSelect           = 0x0013,
    kSCDefaultPixMapSettingsSelect             = 0x0014,
    kSCGetInfoSelect                           = 0x0015,
    kSCSetInfoSelect                           = 0x0016,
    kSCNewGWorldSelect                         = 0x0017,
    kSCSetCompressFlagsSelect                  = 0x0018,
    kSCGetCompressFlagsSelect                  = 0x0019,
    kSCGetSettingsAsTextSelect                 = 0x001A,
    kSCGetSettingsAsAtomContainerSelect        = 0x001B,
    kSCSetSettingsFromAtomContainerSelect      = 0x001C,
    kSCCompressSequenceFrameAsyncSelect        = 0x001D,
    kSCAsyncIdleSelect                         = 0x001E,
    kTweenerInitializeSelect                   = 0x0001,
    kTweenerDoTweenSelect                      = 0x0002,
    kTweenerResetSelect                        = 0x0003,
    kTCGetCurrentTimeCodeSelect                = 0x0101,
    kTCGetTimeCodeAtTimeSelect                 = 0x0102,
    kTCTimeCodeToStringSelect                  = 0x0103,
    kTCTimeCodeToFrameNumberSelect             = 0x0104,
    kTCFrameNumberToTimeCodeSelect             = 0x0105,
    kTCGetSourceRefSelect                      = 0x0106,
    kTCSetSourceRefSelect                      = 0x0107,
    kTCSetTimeCodeFlagsSelect                  = 0x0108,
    kTCGetTimeCodeFlagsSelect                  = 0x0109,
    kTCSetDisplayOptionsSelect                 = 0x010A,
    kTCGetDisplayOptionsSelect                 = 0x010B,
    kMovieImportHandleSelect                   = 0x0001,
    kMovieImportFileSelect                     = 0x0002,
    kMovieImportSetSampleDurationSelect        = 0x0003,
    kMovieImportSetSampleDescriptionSelect     = 0x0004,
    kMovieImportSetMediaFileSelect             = 0x0005,
    kMovieImportSetDimensionsSelect            = 0x0006,
    kMovieImportSetChunkSizeSelect             = 0x0007,
    kMovieImportSetProgressProcSelect          = 0x0008,
    kMovieImportSetAuxiliaryDataSelect         = 0x0009,
    kMovieImportSetFromScrapSelect             = 0x000A,
    kMovieImportDoUserDialogSelect             = 0x000B,
    kMovieImportSetDurationSelect              = 0x000C,
    kMovieImportGetAuxiliaryDataTypeSelect     = 0x000D,
    kMovieImportValidateSelect                 = 0x000E,
    kMovieImportGetFileTypeSelect              = 0x000F,
    kMovieImportDataRefSelect                  = 0x0010,
    kMovieImportGetSampleDescriptionSelect     = 0x0011,
    kMovieImportGetMIMETypeListSelect          = 0x0012,
    kMovieImportSetOffsetAndLimitSelect        = 0x0013,
    kMovieImportGetSettingsAsAtomContainerSelect = 0x0014,
    kMovieImportSetSettingsFromAtomContainerSelect = 0x0015,
    kMovieImportSetOffsetAndLimit64Select      = 0x0016,
    kMovieImportIdleSelect                     = 0x0017,
    kMovieImportValidateDataRefSelect          = 0x0018,
    kMovieImportGetLoadStateSelect             = 0x0019,
    kMovieImportGetMaxLoadedTimeSelect         = 0x001A,
    kMovieImportEstimateCompletionTimeSelect   = 0x001B,
    kMovieImportSetDontBlockSelect             = 0x001C,
    kMovieImportGetDontBlockSelect             = 0x001D,
    kMovieImportSetIdleManagerSelect           = 0x001E,
    kMovieImportSetNewMovieFlagsSelect         = 0x001F,
    kMovieImportGetDestinationMediaTypeSelect  = 0x0020,
    kMovieExportToHandleSelect                 = 0x0080,
    kMovieExportToFileSelect                   = 0x0081,
    kMovieExportGetAuxiliaryDataSelect         = 0x0083,
    kMovieExportSetProgressProcSelect          = 0x0084,
    kMovieExportSetSampleDescriptionSelect     = 0x0085,
    kMovieExportDoUserDialogSelect             = 0x0086,
    kMovieExportGetCreatorTypeSelect           = 0x0087,
    kMovieExportToDataRefSelect                = 0x0088,
    kMovieExportFromProceduresToDataRefSelect  = 0x0089,
    kMovieExportAddDataSourceSelect            = 0x008A,
    kMovieExportValidateSelect                 = 0x008B,
    kMovieExportGetSettingsAsAtomContainerSelect = 0x008C,
    kMovieExportSetSettingsFromAtomContainerSelect = 0x008D,
    kMovieExportGetFileNameExtensionSelect     = 0x008E,
    kMovieExportGetShortFileTypeStringSelect   = 0x008F,
    kMovieExportGetSourceMediaTypeSelect       = 0x0090,
    kMovieExportSetGetMoviePropertyProcSelect  = 0x0091,
    kTextExportGetDisplayDataSelect            = 0x0100,
    kTextExportGetTimeFractionSelect           = 0x0101,
    kTextExportSetTimeFractionSelect           = 0x0102,
    kTextExportGetSettingsSelect               = 0x0103,
    kTextExportSetSettingsSelect               = 0x0104,
    kMIDIImportGetSettingsSelect               = 0x0100,
    kMIDIImportSetSettingsSelect               = 0x0101,
    kMovieExportNewGetDataAndPropertiesProcsSelect = 0x0100,
    kMovieExportDisposeGetDataAndPropertiesProcsSelect = 0x0101,
    kGraphicsImageImportSetSequenceEnabledSelect = 0x0100,
    kGraphicsImageImportGetSequenceEnabledSelect = 0x0101,
    kPreviewShowDataSelect                     = 0x0001,
    kPreviewMakePreviewSelect                  = 0x0002,
    kPreviewMakePreviewReferenceSelect         = 0x0003,
    kPreviewEventSelect                        = 0x0004,
    kDataCodecDecompressSelect                 = 0x0001,
    kDataCodecGetCompressBufferSizeSelect      = 0x0002,
    kDataCodecCompressSelect                   = 0x0003,
    kDataCodecBeginInterruptSafeSelect         = 0x0004,
    kDataCodecEndInterruptSafeSelect           = 0x0005,
    kDataCodecDecompressPartialSelect          = 0x0006,
    kDataCodecCompressPartialSelect            = 0x0007,
    kDataHGetDataSelect                        = 0x0002,
    kDataHPutDataSelect                        = 0x0003,
    kDataHFlushDataSelect                      = 0x0004,
    kDataHOpenForWriteSelect                   = 0x0005,
    kDataHCloseForWriteSelect                  = 0x0006,
    kDataHOpenForReadSelect                    = 0x0008,
    kDataHCloseForReadSelect                   = 0x0009,
    kDataHSetDataRefSelect                     = 0x000A,
    kDataHGetDataRefSelect                     = 0x000B,
    kDataHCompareDataRefSelect                 = 0x000C,
    kDataHTaskSelect                           = 0x000D,
    kDataHScheduleDataSelect                   = 0x000E,
    kDataHFinishDataSelect                     = 0x000F,
    kDataHFlushCacheSelect                     = 0x0010,
    kDataHResolveDataRefSelect                 = 0x0011,
    kDataHGetFileSizeSelect                    = 0x0012,
    kDataHCanUseDataRefSelect                  = 0x0013,
    kDataHGetVolumeListSelect                  = 0x0014,
    kDataHWriteSelect                          = 0x0015,
    kDataHPreextendSelect                      = 0x0016,
    kDataHSetFileSizeSelect                    = 0x0017,
    kDataHGetFreeSpaceSelect                   = 0x0018,
    kDataHCreateFileSelect                     = 0x0019,
    kDataHGetPreferredBlockSizeSelect          = 0x001A,
    kDataHGetDeviceIndexSelect                 = 0x001B,
    kDataHIsStreamingDataHandlerSelect         = 0x001C,
    kDataHGetDataInBufferSelect                = 0x001D,
    kDataHGetScheduleAheadTimeSelect           = 0x001E,
    kDataHSetCacheSizeLimitSelect              = 0x001F,
    kDataHGetCacheSizeLimitSelect              = 0x0020,
    kDataHGetMovieSelect                       = 0x0021,
    kDataHAddMovieSelect                       = 0x0022,
    kDataHUpdateMovieSelect                    = 0x0023,
    kDataHDoesBufferSelect                     = 0x0024,
    kDataHGetFileNameSelect                    = 0x0025,
    kDataHGetAvailableFileSizeSelect           = 0x0026,
    kDataHGetMacOSFileTypeSelect               = 0x0027,
    kDataHGetMIMETypeSelect                    = 0x0028,
    kDataHSetDataRefWithAnchorSelect           = 0x0029,
    kDataHGetDataRefWithAnchorSelect           = 0x002A,
    kDataHSetMacOSFileTypeSelect               = 0x002B,
    kDataHSetTimeBaseSelect                    = 0x002C,
    kDataHGetInfoFlagsSelect                   = 0x002D,
    kDataHScheduleData64Select                 = 0x002E,
    kDataHWrite64Select                        = 0x002F,
    kDataHGetFileSize64Select                  = 0x0030,
    kDataHPreextend64Select                    = 0x0031,
    kDataHSetFileSize64Select                  = 0x0032,
    kDataHGetFreeSpace64Select                 = 0x0033,
    kDataHAppend64Select                       = 0x0034,
    kDataHReadAsyncSelect                      = 0x0035,
    kDataHPollReadSelect                       = 0x0036,
    kDataHGetDataAvailabilitySelect            = 0x0037,
    kDataHGetFileSizeAsyncSelect               = 0x003A,
    kDataHGetDataRefAsTypeSelect               = 0x003B,
    kDataHSetDataRefExtensionSelect            = 0x003C,
    kDataHGetDataRefExtensionSelect            = 0x003D,
    kDataHGetMovieWithFlagsSelect              = 0x003E,
    kDataHGetFileTypeOrderingSelect            = 0x0040,
    kDataHCreateFileWithFlagsSelect            = 0x0041,
    kDataHGetMIMETypeAsyncSelect               = 0x0042,
    kDataHGetInfoSelect                        = 0x0043,
    kDataHSetIdleManagerSelect                 = 0x0044,
    kDataHDeleteFileSelect                     = 0x0045,
    kDataHSetMovieUsageFlagsSelect             = 0x0046,
    kDataHUseTemporaryDataRefSelect            = 0x0047,
    kDataHGetTemporaryDataRefCapabilitiesSelect = 0x0048,
    kDataHRenameFileSelect                     = 0x0049,
    kDataHPlaybackHintsSelect                  = 0x0103,
    kDataHPlaybackHints64Select                = 0x010E,
    kDataHGetDataRateSelect                    = 0x0110,
    kDataHSetTimeHintsSelect                   = 0x0111,
    kVDGetMaxSrcRectSelect                     = 0x0001,
    kVDGetActiveSrcRectSelect                  = 0x0002,
    kVDSetDigitizerRectSelect                  = 0x0003,
    kVDGetDigitizerRectSelect                  = 0x0004,
    kVDGetVBlankRectSelect                     = 0x0005,
    kVDGetMaskPixMapSelect                     = 0x0006,
    kVDGetPlayThruDestinationSelect            = 0x0008,
    kVDUseThisCLUTSelect                       = 0x0009,
    kVDSetInputGammaValueSelect                = 0x000A,
    kVDGetInputGammaValueSelect                = 0x000B,
    kVDSetBrightnessSelect                     = 0x000C,
    kVDGetBrightnessSelect                     = 0x000D,
    kVDSetContrastSelect                       = 0x000E,
    kVDSetHueSelect                            = 0x000F,
    kVDSetSharpnessSelect                      = 0x0010,
    kVDSetSaturationSelect                     = 0x0011,
    kVDGetContrastSelect                       = 0x0012,
    kVDGetHueSelect                            = 0x0013,
    kVDGetSharpnessSelect                      = 0x0014,
    kVDGetSaturationSelect                     = 0x0015,
    kVDGrabOneFrameSelect                      = 0x0016,
    kVDGetMaxAuxBufferSelect                   = 0x0017,
    kVDGetDigitizerInfoSelect                  = 0x0019,
    kVDGetCurrentFlagsSelect                   = 0x001A,
    kVDSetKeyColorSelect                       = 0x001B,
    kVDGetKeyColorSelect                       = 0x001C,
    kVDAddKeyColorSelect                       = 0x001D,
    kVDGetNextKeyColorSelect                   = 0x001E,
    kVDSetKeyColorRangeSelect                  = 0x001F,
    kVDGetKeyColorRangeSelect                  = 0x0020,
    kVDSetDigitizerUserInterruptSelect         = 0x0021,
    kVDSetInputColorSpaceModeSelect            = 0x0022,
    kVDGetInputColorSpaceModeSelect            = 0x0023,
    kVDSetClipStateSelect                      = 0x0024,
    kVDGetClipStateSelect                      = 0x0025,
    kVDSetClipRgnSelect                        = 0x0026,
    kVDClearClipRgnSelect                      = 0x0027,
    kVDGetCLUTInUseSelect                      = 0x0028,
    kVDSetPLLFilterTypeSelect                  = 0x0029,
    kVDGetPLLFilterTypeSelect                  = 0x002A,
    kVDGetMaskandValueSelect                   = 0x002B,
    kVDSetMasterBlendLevelSelect               = 0x002C,
    kVDSetPlayThruDestinationSelect            = 0x002D,
    kVDSetPlayThruOnOffSelect                  = 0x002E,
    kVDSetFieldPreferenceSelect                = 0x002F,
    kVDGetFieldPreferenceSelect                = 0x0030,
    kVDPreflightDestinationSelect              = 0x0032,
    kVDPreflightGlobalRectSelect               = 0x0033,
    kVDSetPlayThruGlobalRectSelect             = 0x0034,
    kVDSetInputGammaRecordSelect               = 0x0035,
    kVDGetInputGammaRecordSelect               = 0x0036,
    kVDSetBlackLevelValueSelect                = 0x0037,
    kVDGetBlackLevelValueSelect                = 0x0038,
    kVDSetWhiteLevelValueSelect                = 0x0039,
    kVDGetWhiteLevelValueSelect                = 0x003A,
    kVDGetVideoDefaultsSelect                  = 0x003B,
    kVDGetNumberOfInputsSelect                 = 0x003C,
    kVDGetInputFormatSelect                    = 0x003D,
    kVDSetInputSelect                          = 0x003E,
    kVDGetInputSelect                          = 0x003F,
    kVDSetInputStandardSelect                  = 0x0040,
    kVDSetupBuffersSelect                      = 0x0041,
    kVDGrabOneFrameAsyncSelect                 = 0x0042,
    kVDDoneSelect                              = 0x0043,
    kVDSetCompressionSelect                    = 0x0044,
    kVDCompressOneFrameAsyncSelect             = 0x0045,
    kVDCompressDoneSelect                      = 0x0046,
    kVDReleaseCompressBufferSelect             = 0x0047,
    kVDGetImageDescriptionSelect               = 0x0048,
    kVDResetCompressSequenceSelect             = 0x0049,
    kVDSetCompressionOnOffSelect               = 0x004A,
    kVDGetCompressionTypesSelect               = 0x004B,
    kVDSetTimeBaseSelect                       = 0x004C,
    kVDSetFrameRateSelect                      = 0x004D,
    kVDGetDataRateSelect                       = 0x004E,
    kVDGetSoundInputDriverSelect               = 0x004F,
    kVDGetDMADepthsSelect                      = 0x0050,
    kVDGetPreferredTimeScaleSelect             = 0x0051,
    kVDReleaseAsyncBuffersSelect               = 0x0052,
    kVDSetDataRateSelect                       = 0x0054,
    kVDGetTimeCodeSelect                       = 0x0055,
    kVDUseSafeBuffersSelect                    = 0x0056,
    kVDGetSoundInputSourceSelect               = 0x0057,
    kVDGetCompressionTimeSelect                = 0x0058,
    kVDSetPreferredPacketSizeSelect            = 0x0059,
    kVDSetPreferredImageDimensionsSelect       = 0x005A,
    kVDGetPreferredImageDimensionsSelect       = 0x005B,
    kVDGetInputNameSelect                      = 0x005C,
    kVDSetDestinationPortSelect                = 0x005D,
    kVDGetDeviceNameAndFlagsSelect             = 0x005E,
    kVDCaptureStateChangingSelect              = 0x005F,
    kVDGetUniqueIDsSelect                      = 0x0060,
    kVDSelectUniqueIDsSelect                   = 0x0061,
    kVDIIDCGetFeaturesSelect                   = 0x0200,
    kVDIIDCSetFeaturesSelect                   = 0x0201,
    kVDIIDCGetDefaultFeaturesSelect            = 0x0202,
    kVDIIDCGetCSRDataSelect                    = 0x0203,
    kVDIIDCSetCSRDataSelect                    = 0x0204,
    kXMLParseDataRefSelect                     = 0x0001,
    kXMLParseFileSelect                        = 0x0002,
    kXMLParseDisposeXMLDocSelect               = 0x0003,
    kXMLParseGetDetailedParseErrorSelect       = 0x0004,
    kXMLParseAddElementSelect                  = 0x0005,
    kXMLParseAddAttributeSelect                = 0x0006,
    kXMLParseAddMultipleAttributesSelect       = 0x0007,
    kXMLParseAddAttributeAndValueSelect        = 0x0008,
    kXMLParseAddMultipleAttributesAndValuesSelect = 0x0009,
    kXMLParseAddAttributeValueKindSelect       = 0x000A,
    kXMLParseAddNameSpaceSelect                = 0x000B,
    kXMLParseSetOffsetAndLimitSelect           = 0x000C,
    kXMLParseSetEventParseRefConSelect         = 0x000D,
    kXMLParseSetStartDocumentHandlerSelect     = 0x000E,
    kXMLParseSetEndDocumentHandlerSelect       = 0x000F,
    kXMLParseSetStartElementHandlerSelect      = 0x0010,
    kXMLParseSetEndElementHandlerSelect        = 0x0011,
    kXMLParseSetCharDataHandlerSelect          = 0x0012,
    kXMLParseSetPreprocessInstructionHandlerSelect = 0x0013,
    kXMLParseSetCommentHandlerSelect           = 0x0014,
    kXMLParseSetCDataHandlerSelect             = 0x0015,
    kSGInitializeSelect                        = 0x0001,
    kSGSetDataOutputSelect                     = 0x0002,
    kSGGetDataOutputSelect                     = 0x0003,
    kSGSetGWorldSelect                         = 0x0004,
    kSGGetGWorldSelect                         = 0x0005,
    kSGNewChannelSelect                        = 0x0006,
    kSGDisposeChannelSelect                    = 0x0007,
    kSGStartPreviewSelect                      = 0x0010,
    kSGStartRecordSelect                       = 0x0011,
    kSGIdleSelect                              = 0x0012,
    kSGStopSelect                              = 0x0013,
    kSGPauseSelect                             = 0x0014,
    kSGPrepareSelect                           = 0x0015,
    kSGReleaseSelect                           = 0x0016,
    kSGGetMovieSelect                          = 0x0017,
    kSGSetMaximumRecordTimeSelect              = 0x0018,
    kSGGetMaximumRecordTimeSelect              = 0x0019,
    kSGGetStorageSpaceRemainingSelect          = 0x001A,
    kSGGetTimeRemainingSelect                  = 0x001B,
    kSGGrabPictSelect                          = 0x001C,
    kSGGetLastMovieResIDSelect                 = 0x001D,
    kSGSetFlagsSelect                          = 0x001E,
    kSGGetFlagsSelect                          = 0x001F,
    kSGSetDataProcSelect                       = 0x0020,
    kSGNewChannelFromComponentSelect           = 0x0021,
    kSGDisposeDeviceListSelect                 = 0x0022,
    kSGAppendDeviceListToMenuSelect            = 0x0023,
    kSGSetSettingsSelect                       = 0x0024,
    kSGGetSettingsSelect                       = 0x0025,
    kSGGetIndChannelSelect                     = 0x0026,
    kSGUpdateSelect                            = 0x0027,
    kSGGetPauseSelect                          = 0x0028,
    kSGSettingsDialogSelect                    = 0x0029,
    kSGGetAlignmentProcSelect                  = 0x002A,
    kSGSetChannelSettingsSelect                = 0x002B,
    kSGGetChannelSettingsSelect                = 0x002C,
    kSGGetModeSelect                           = 0x002D,
    kSGSetDataRefSelect                        = 0x002E,
    kSGGetDataRefSelect                        = 0x002F,
    kSGNewOutputSelect                         = 0x0030,
    kSGDisposeOutputSelect                     = 0x0031,
    kSGSetOutputFlagsSelect                    = 0x0032,
    kSGSetChannelOutputSelect                  = 0x0033,
    kSGGetDataOutputStorageSpaceRemainingSelect = 0x0034,
    kSGHandleUpdateEventSelect                 = 0x0035,
    kSGSetOutputNextOutputSelect               = 0x0036,
    kSGGetOutputNextOutputSelect               = 0x0037,
    kSGSetOutputMaximumOffsetSelect            = 0x0038,
    kSGGetOutputMaximumOffsetSelect            = 0x0039,
    kSGGetOutputDataReferenceSelect            = 0x003A,
    kSGWriteExtendedMovieDataSelect            = 0x003B,
    kSGGetStorageSpaceRemaining64Select        = 0x003C,
    kSGGetDataOutputStorageSpaceRemaining64Select = 0x003D,
    kSGWriteMovieDataSelect                    = 0x0100,
    kSGAddFrameReferenceSelect                 = 0x0101,
    kSGGetNextFrameReferenceSelect             = 0x0102,
    kSGGetTimeBaseSelect                       = 0x0103,
    kSGSortDeviceListSelect                    = 0x0104,
    kSGAddMovieDataSelect                      = 0x0105,
    kSGChangedSourceSelect                     = 0x0106,
    kSGAddExtendedFrameReferenceSelect         = 0x0107,
    kSGGetNextExtendedFrameReferenceSelect     = 0x0108,
    kSGAddExtendedMovieDataSelect              = 0x0109,
    kSGAddOutputDataRefToMediaSelect           = 0x010A,
    kSGSetSettingsSummarySelect                = 0x010B,
    kSGSetChannelUsageSelect                   = 0x0080,
    kSGGetChannelUsageSelect                   = 0x0081,
    kSGSetChannelBoundsSelect                  = 0x0082,
    kSGGetChannelBoundsSelect                  = 0x0083,
    kSGSetChannelVolumeSelect                  = 0x0084,
    kSGGetChannelVolumeSelect                  = 0x0085,
    kSGGetChannelInfoSelect                    = 0x0086,
    kSGSetChannelPlayFlagsSelect               = 0x0087,
    kSGGetChannelPlayFlagsSelect               = 0x0088,
    kSGSetChannelMaxFramesSelect               = 0x0089,
    kSGGetChannelMaxFramesSelect               = 0x008A,
    kSGSetChannelRefConSelect                  = 0x008B,
    kSGSetChannelClipSelect                    = 0x008C,
    kSGGetChannelClipSelect                    = 0x008D,
    kSGGetChannelSampleDescriptionSelect       = 0x008E,
    kSGGetChannelDeviceListSelect              = 0x008F,
    kSGSetChannelDeviceSelect                  = 0x0090,
    kSGSetChannelMatrixSelect                  = 0x0091,
    kSGGetChannelMatrixSelect                  = 0x0092,
    kSGGetChannelTimeScaleSelect               = 0x0093,
    kSGChannelPutPictureSelect                 = 0x0094,
    kSGChannelSetRequestedDataRateSelect       = 0x0095,
    kSGChannelGetRequestedDataRateSelect       = 0x0096,
    kSGChannelSetDataSourceNameSelect          = 0x0097,
    kSGChannelGetDataSourceNameSelect          = 0x0098,
    kSGChannelSetCodecSettingsSelect           = 0x0099,
    kSGChannelGetCodecSettingsSelect           = 0x009A,
    kSGGetChannelTimeBaseSelect                = 0x009B,
    kSGGetChannelRefConSelect                  = 0x009C,
    kSGGetChannelDeviceAndInputNamesSelect     = 0x009D,
    kSGSetChannelDeviceInputSelect             = 0x009E,
    kSGSetChannelSettingsStateChangingSelect   = 0x009F,
    kSGInitChannelSelect                       = 0x0180,
    kSGWriteSamplesSelect                      = 0x0181,
    kSGGetDataRateSelect                       = 0x0182,
    kSGAlignChannelRectSelect                  = 0x0183,
    kSGPanelGetDitlSelect                      = 0x0200,
    kSGPanelGetTitleSelect                     = 0x0201,
    kSGPanelCanRunSelect                       = 0x0202,
    kSGPanelInstallSelect                      = 0x0203,
    kSGPanelEventSelect                        = 0x0204,
    kSGPanelItemSelect                         = 0x0205,
    kSGPanelRemoveSelect                       = 0x0206,
    kSGPanelSetGrabberSelect                   = 0x0207,
    kSGPanelSetResFileSelect                   = 0x0208,
    kSGPanelGetSettingsSelect                  = 0x0209,
    kSGPanelSetSettingsSelect                  = 0x020A,
    kSGPanelValidateInputSelect                = 0x020B,
    kSGPanelSetEventFilterSelect               = 0x020C,
    kSGPanelGetDITLForSizeSelect               = 0x020D,
    kSGGetSrcVideoBoundsSelect                 = 0x0100,
    kSGSetVideoRectSelect                      = 0x0101,
    kSGGetVideoRectSelect                      = 0x0102,
    kSGGetVideoCompressorTypeSelect            = 0x0103,
    kSGSetVideoCompressorTypeSelect            = 0x0104,
    kSGSetVideoCompressorSelect                = 0x0105,
    kSGGetVideoCompressorSelect                = 0x0106,
    kSGGetVideoDigitizerComponentSelect        = 0x0107,
    kSGSetVideoDigitizerComponentSelect        = 0x0108,
    kSGVideoDigitizerChangedSelect             = 0x0109,
    kSGSetVideoBottlenecksSelect               = 0x010A,
    kSGGetVideoBottlenecksSelect               = 0x010B,
    kSGGrabFrameSelect                         = 0x010C,
    kSGGrabFrameCompleteSelect                 = 0x010D,
    kSGDisplayFrameSelect                      = 0x010E,
    kSGCompressFrameSelect                     = 0x010F,
    kSGCompressFrameCompleteSelect             = 0x0110,
    kSGAddFrameSelect                          = 0x0111,
    kSGTransferFrameForCompressSelect          = 0x0112,
    kSGSetCompressBufferSelect                 = 0x0113,
    kSGGetCompressBufferSelect                 = 0x0114,
    kSGGetBufferInfoSelect                     = 0x0115,
    kSGSetUseScreenBufferSelect                = 0x0116,
    kSGGetUseScreenBufferSelect                = 0x0117,
    kSGGrabCompressCompleteSelect              = 0x0118,
    kSGDisplayCompressSelect                   = 0x0119,
    kSGSetFrameRateSelect                      = 0x011A,
    kSGGetFrameRateSelect                      = 0x011B,
    kSGSetPreferredPacketSizeSelect            = 0x0121,
    kSGGetPreferredPacketSizeSelect            = 0x0122,
    kSGSetUserVideoCompressorListSelect        = 0x0123,
    kSGGetUserVideoCompressorListSelect        = 0x0124,
    kSGSetSoundInputDriverSelect               = 0x0100,
    kSGGetSoundInputDriverSelect               = 0x0101,
    kSGSoundInputDriverChangedSelect           = 0x0102,
    kSGSetSoundRecordChunkSizeSelect           = 0x0103,
    kSGGetSoundRecordChunkSizeSelect           = 0x0104,
    kSGSetSoundInputRateSelect                 = 0x0105,
    kSGGetSoundInputRateSelect                 = 0x0106,
    kSGSetSoundInputParametersSelect           = 0x0107,
    kSGGetSoundInputParametersSelect           = 0x0108,
    kSGSetAdditionalSoundRatesSelect           = 0x0109,
    kSGGetAdditionalSoundRatesSelect           = 0x010A,
    kSGSetFontNameSelect                       = 0x0100,
    kSGSetFontSizeSelect                       = 0x0101,
    kSGSetTextForeColorSelect                  = 0x0102,
    kSGSetTextBackColorSelect                  = 0x0103,
    kSGSetJustificationSelect                  = 0x0104,
    kSGGetTextReturnToSpaceValueSelect         = 0x0105,
    kSGSetTextReturnToSpaceValueSelect         = 0x0106,
    kSGGetInstrumentSelect                     = 0x0100,
    kSGSetInstrumentSelect                     = 0x0101,
    kQTVideoOutputGetDisplayModeListSelect     = 0x0001,
    kQTVideoOutputGetCurrentClientNameSelect   = 0x0002,
    kQTVideoOutputSetClientNameSelect          = 0x0003,
    kQTVideoOutputGetClientNameSelect          = 0x0004,
    kQTVideoOutputBeginSelect                  = 0x0005,
    kQTVideoOutputEndSelect                    = 0x0006,
    kQTVideoOutputSetDisplayModeSelect         = 0x0007,
    kQTVideoOutputGetDisplayModeSelect         = 0x0008,
    kQTVideoOutputCustomConfigureDisplaySelect = 0x0009,
    kQTVideoOutputSaveStateSelect              = 0x000A,
    kQTVideoOutputRestoreStateSelect           = 0x000B,
    kQTVideoOutputGetGWorldSelect              = 0x000C,
    kQTVideoOutputGetGWorldParametersSelect    = 0x000D,
    kQTVideoOutputGetIndSoundOutputSelect      = 0x000E,
    kQTVideoOutputGetClockSelect               = 0x000F,
    kQTVideoOutputSetEchoPortSelect            = 0x0010,
    kQTVideoOutputGetIndImageDecompressorSelect = 0x0011,
    kQTVideoOutputBaseSetEchoPortSelect        = 0x0012
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

#endif /* __QUICKTIMECOMPONENTS__ */

