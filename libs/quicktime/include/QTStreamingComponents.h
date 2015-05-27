/*
     File:       QTStreamingComponents.h
 
     Contains:   QuickTime Interfaces.
 
     Version:    Technology: QuickTime 6.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QTSTREAMINGCOMPONENTS__
#define __QTSTREAMINGCOMPONENTS__

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __MACERRORS__
#include <MacErrors.h>
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

/*============================================================================
        Stream Sourcer
============================================================================*/
enum {
    kQTSSourcerType             = FOUR_CHAR_CODE('srcr')
};

typedef ComponentInstance               QTSSourcer;
enum {
    kQTSSGChannelSourcerType    = FOUR_CHAR_CODE('sgch'),
    kQTSMovieTrackSourcerType   = FOUR_CHAR_CODE('trak'),
    kQTSPushDataSourcerType     = FOUR_CHAR_CODE('push')
};

/* flags for sourcer data */
enum {
    kQTSSourcerDataFlag_SyncSample = 0x00000001,
    kQTSPushDataSourcerFlag_SampleTimeIsValid = (long)0x80000000
};


enum {
    kQTSSourcerInitParamsVersion1 = 1
};


struct QTSSourcerInitParams {
    SInt32                          version;
    SInt32                          flags;
    OSType                          dataType;
    void *                          data;
    UInt32                          dataLength;
};
typedef struct QTSSourcerInitParams     QTSSourcerInitParams;
EXTERN_API_C( OSErr )
QTSNewSourcer                   (void *                 params,
                                 const QTSSourcerInitParams * inInitParams,
                                 SInt32                 inFlags,
                                 ComponentInstance *    outSourcer);

/* info selectors for sourcers - get and set */
enum {
    kQTSInfo_Track              = FOUR_CHAR_CODE('trak'),       /* QTSTrackParams* */
    kQTSInfo_Loop               = FOUR_CHAR_CODE('loop'),       /* QTSLoopParams* */
    kQTSInfo_SourcerTiming      = FOUR_CHAR_CODE('stim'),       /* QTSSourcerTimingParams* */
    kQTSInfo_TargetFrameRate    = FOUR_CHAR_CODE('tfps'),       /* Fixed * in frames per second */
    kQTSInfo_PushData           = FOUR_CHAR_CODE('push'),       /* QTSPushDataParams* */
    kQTSInfo_SourcerCallbackProc = FOUR_CHAR_CODE('scbp'),      /* QTSSourcerCallbackProcParams* */
    kQTSInfo_TargetDataRate     = FOUR_CHAR_CODE('tdrt'),       /* UInt32 * in bytes per second */
    kQTSInfo_AudioAutoGainOnOff = FOUR_CHAR_CODE('agc '),       /* Boolean*  - error if unavailable*/
    kQTSInfo_AudioGain          = FOUR_CHAR_CODE('gain'),       /* Fixed* kFixed1 is unity gain */
    kQTSInfo_CroppedInputRect   = FOUR_CHAR_CODE('crpr'),       /* Rect* - defined relative to kQTSInfo_FullInputRect below */
    kQTSInfo_SpatialSettings    = FOUR_CHAR_CODE('sptl'),       /* pointer to SCSpatialSettings struct*/
    kQTSInfo_TemporalSettings   = FOUR_CHAR_CODE('tprl'),       /* pointer to SCTemporalSettings struct*/
    kQTSInfo_DataRateSettings   = FOUR_CHAR_CODE('drat'),       /* pointer to SCDataRateSettings struct*/
    kQTSInfo_CodecFlags         = FOUR_CHAR_CODE('cflg'),       /* pointer to CodecFlags*/
    kQTSInfo_CodecSettings      = FOUR_CHAR_CODE('cdec'),       /* pointer to Handle*/
    kQTSInfo_ForceKeyValue      = FOUR_CHAR_CODE('ksim'),       /* pointer to long*/
    kQTSInfo_SoundSampleRate    = FOUR_CHAR_CODE('ssrt'),       /* pointer to UnsignedFixed*/
    kQTSInfo_SoundSampleSize    = FOUR_CHAR_CODE('ssss'),       /* pointer to short*/
    kQTSInfo_SoundChannelCount  = FOUR_CHAR_CODE('sscc'),       /* pointer to short*/
    kQTSInfo_SoundCompression   = FOUR_CHAR_CODE('ssct'),       /* pointer to OSType*/
    kQTSInfo_CompressionList    = FOUR_CHAR_CODE('ctyl'),       /* pointer to OSType Handle*/
    kQTSInfo_VideoHue           = FOUR_CHAR_CODE('hue '),       /* UInt16* */
    kQTSInfo_VideoSaturation    = FOUR_CHAR_CODE('satr'),       /* UInt16* */
    kQTSInfo_VideoContrast      = FOUR_CHAR_CODE('trst'),       /* UInt16* */
    kQTSInfo_VideoBrightness    = FOUR_CHAR_CODE('brit'),       /* UInt16* */
    kQTSInfo_VideoSharpness     = FOUR_CHAR_CODE('shrp'),       /* UInt16* */
    kQTSInfo_TimeScale          = FOUR_CHAR_CODE('scal'),       /* UInt32* */
    kQTSInfo_SGChannelDeviceName = FOUR_CHAR_CODE('innm'),      /* Handle* */
    kQTSInfo_SGChannelDeviceList = FOUR_CHAR_CODE('srdl'),      /* SGDeviceList* */
    kQTSInfo_SGChannelDeviceInput = FOUR_CHAR_CODE('sdii'),     /* short* */
    kQTSInfo_SGChannelSettings  = FOUR_CHAR_CODE('sesg'),       /* QTSSGChannelSettingsParams */
    kQTSInfo_PreviewWhileRecordingMode = FOUR_CHAR_CODE('srpr'), /* Boolean* */
    kQTSInfo_CompressionParams  = FOUR_CHAR_CODE('sccp')        /* QTAtomContainer* */
};

/* info selectors for sourcers - get only*/
enum {
    kQTSInfo_SGChannel          = FOUR_CHAR_CODE('sgch'),       /* SGChannel* */
    kQTSInfo_SGChannelInputName = FOUR_CHAR_CODE('srnm'),       /* Handle* */
    kQTSInfo_FullInputRect      = FOUR_CHAR_CODE('fulr')        /* Rect* */
};

/* loop flags */
enum {
    kQTSLoopFlag_Loop           = 0x00000001
};

enum {
    kQTSLoopParamsVersion1      = 1
};


struct QTSLoopParams {
    SInt32                          version;
    SInt32                          flags;
    SInt32                          loopFlags;
    SInt32                          flagsMask;
    SInt32                          numLoops;
};
typedef struct QTSLoopParams            QTSLoopParams;
enum {
    kQTSTrackParamsVersion1     = 1
};


struct QTSTrackParams {
    SInt32                          version;
    SInt32                          flags;
    Track                           track;
    TimeValue64                     trackStartOffset;           /* to start other than at the beginning otherwise set to 0*/
    TimeValue64                     duration;                   /* to limit the duration otherwise set to 0*/
    QTSLoopParams *                 loopParams;                 /* set to NULL if not using; default is no looping */
};
typedef struct QTSTrackParams           QTSTrackParams;
enum {
    kQTSSourcerTimingParamsVersion1 = 1
};


struct QTSSourcerTimingParams {
    SInt32                          version;
    SInt32                          flags;
    TimeScale                       timeScale;
    TimeValue64                     presentationStartTime;
    TimeValue64                     presentationEndTime;
    TimeValue64                     presentationCurrentTime;
    TimeValue64                     localStartTime;
    TimeValue64                     localEndTime;
    TimeValue64                     localCurrentTime;
};
typedef struct QTSSourcerTimingParams   QTSSourcerTimingParams;
enum {
    kQTSPushDataParamsVersion1  = 1
};

enum {
    kQTSPushDataFlag_SampleTimeIsValid = 0x00000001,
    kQTSPushDataFlag_DurationIsValid = 0x00000002
};


struct QTSPushDataParams {
    SInt32                          version;
    SInt32                          flags;
    SampleDescriptionHandle         sampleDescription;          /* caller owns the handle */
    UInt32                          sampleDescSeed;
    TimeValue64                     sampleTime;                 /* also set flag if you set this */
    TimeValue64                     duration;                   /* also set flag if you set this */
    UInt32                          dataLength;
    void *                          dataPtr;                    /* this does not have to be a real macintosh Ptr */
};
typedef struct QTSPushDataParams        QTSPushDataParams;
enum {
    kQTSSourcerCallbackProcParamsVersion1 = 1
};



struct QTSSourcerCallbackProcParams {
    SInt32                          version;
    SInt32                          flags;
    QTSNotificationUPP              proc;
    void *                          refCon;
};
typedef struct QTSSourcerCallbackProcParams QTSSourcerCallbackProcParams;
/* track sourcer callback selectors*/
enum {
    kQTSSourcerCallback_Done    = FOUR_CHAR_CODE('done')        /* QTSSourcerDoneParams* */
};


/* push data sourcer callback selectors*/
enum {
    kQTSPushDataSourcerCallback_HasCharacteristic = 0x050D,     /* QTSPushDataHasCharacteristicParams* */
    kQTSPushDataSourcerCallback_SetInfo = 0x0507,               /* QTSPushDataInfoParams* */
    kQTSPushDataSourcerCallback_GetInfo = 0x0508                /* QTSPushDataInfoParams* */
};


struct QTSPushDataHasCharacteristicParams {
    SInt32                          version;
    SInt32                          flags;
    OSType                          characteristic;
    Boolean                         returnedHasIt;
    char                            reserved1;
    char                            reserved2;
    char                            reserved3;
};
typedef struct QTSPushDataHasCharacteristicParams QTSPushDataHasCharacteristicParams;

struct QTSPushDataInfoParams {
    SInt32                          version;
    SInt32                          flags;
    OSType                          selector;
    void *                          ioParams;
};
typedef struct QTSPushDataInfoParams    QTSPushDataInfoParams;
enum {
    kQTSSourcerDoneParamsVersion1 = 1
};


struct QTSSourcerDoneParams {
    SInt32                          version;
    SInt32                          flags;
    ComponentInstance               sourcer;
};
typedef struct QTSSourcerDoneParams     QTSSourcerDoneParams;

struct QTSSGChannelSettingsParams {
    UserData                        settings;
    SInt32                          flags;
};
typedef struct QTSSGChannelSettingsParams QTSSGChannelSettingsParams;




/*-----------------------------------------
    Stream Sourcer Selectors
-----------------------------------------*/
enum {
    kQTSSourcerInitializeSelect = 0x0500,
    kQTSSourcerSetEnableSelect  = 0x0503,
    kQTSSourcerGetEnableSelect  = 0x0504,
    kQTSSourcerSetInfoSelect    = 0x0507,
    kQTSSourcerGetInfoSelect    = 0x0508,
    kQTSSourcerSetTimeScaleSelect = 0x050E,
    kQTSSourcerGetTimeScaleSelect = 0x050F,
    kQTSSourcerIdleSelect       = 0x0516
};

/*-----------------------------------------
    Stream Sourcer Prototypes
-----------------------------------------*/
EXTERN_API( ComponentResult )
QTSSourcerInitialize            (QTSSourcer             inSourcer,
                                 const QTSSourcerInitParams * inInitParams)                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0500, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTSSourcerIdle                  (QTSSourcer             inSourcer,
                                 const TimeValue64 *    inTime,
                                 SInt32                 inFlags,
                                 SInt32 *               outFlags)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x0516, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTSSourcerSetEnable             (QTSSourcer             inSourcer,
                                 Boolean                inEnableMode,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0006, 0x0503, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTSSourcerGetEnable             (QTSSourcer             inSourcer,
                                 Boolean *              outEnableMode,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x0504, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTSSourcerSetTimeScale          (QTSSourcer             inSourcer,
                                 TimeScale              inTimeScale)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x050E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTSSourcerGetTimeScale          (QTSSourcer             inSourcer,
                                 TimeScale *            outTimeScale)                       FIVEWORDINLINE(0x2F3C, 0x0004, 0x050F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTSSourcerSetInfo               (QTSSourcer             inSourcer,
                                 OSType                 inSelector,
                                 void *                 ioParams)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0507, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTSSourcerGetInfo               (QTSSourcer             inSourcer,
                                 OSType                 inSelector,
                                 void *                 ioParams)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0508, 0x7000, 0xA82A);


enum {
    kQTSInfo_InputDeviceName    = FOUR_CHAR_CODE('innm'),       /* Handle* */
    kQTSInfo_InputSourceName    = FOUR_CHAR_CODE('srnm')        /* Handle* */
};




/*============================================================================
        Stream Handler
============================================================================*/

/*
    Server edits are only valid for the current chunk
*/

struct SHServerEditParameters {
    UInt32                          version;
    Fixed                           editRate;
    TimeValue64                     dataStartTime_mediaAxis;
    TimeValue64                     dataEndTime_mediaAxis;
};
typedef struct SHServerEditParameters   SHServerEditParameters;
enum {
    kSHNoChunkDispatchFlags     = 0,
    kSHChunkFlagSyncSample      = 1 << 2,
    kSHChunkFlagDataLoss        = 1 << 4,
    kSHChunkFlagExtended        = 1 << 5
};


struct SHChunkRecord {
    UInt32                          version;
    long                            reserved1;
    SInt32                          flags;
    UInt32                          dataSize;
    UInt8 *                         dataPtr;
    long                            reserved2;
    long                            reserved3;
    TimeValue64                     presentationTime;

    long                            reserved4;
    long                            reserved5;
    const SHServerEditParameters *  serverEditParameters;
    long                            reserved6;
    long                            reserved7;
};
typedef struct SHChunkRecord            SHChunkRecord;
enum {
    kSHNumExtendedDataLongs     = 10
};

enum {
    kSHExtendedChunkFlag_HasSampleCount = 1 << 0,
    kSHExtendedChunkFlag_HasFrameLengths = 1 << 1
};


struct SHExtendedChunkRecord {
    SHChunkRecord                   chunk;
    SInt32                          extendedFlags;
    SInt32                          extendedData[10];
};
typedef struct SHExtendedChunkRecord    SHExtendedChunkRecord;

/*============================================================================
        RTP Components
============================================================================*/

typedef UInt32                          RTPSSRC;
enum {
    kRTPInvalidSSRC             = 0
};


/* RTP standard content encodings for audio */
enum {
    kRTPPayload_PCMU            = 0,                            /* 8kHz PCM mu-law mono */
    kRTPPayload_1016            = 1,                            /* 8kHz CELP (Fed Std 1016) mono */
    kRTPPayload_G721            = 2,                            /* 8kHz G.721 ADPCM mono */
    kRTPPayload_GSM             = 3,                            /* 8kHz GSM mono */
    kRTPPayload_G723            = 4,                            /* 8kHz G.723 ADPCM mono */
    kRTPPayload_DVI_8           = 5,                            /* 8kHz Intel DVI ADPCM mono */
    kRTPPayload_DVI_16          = 6,                            /* 16kHz Intel DVI ADPCM mono */
    kRTPPayload_LPC             = 7,                            /* 8kHz LPC */
    kRTPPayload_PCMA            = 8,                            /* 8kHz PCM a-law mono */
    kRTPPayload_L16_44_2        = 10,                           /* 44.1kHz 16-bit linear stereo */
    kRTPPayload_L16_44_1        = 11,                           /* 44.1kHz 16-bit linear mono */
    kRTPPayload_PureVoice       = 12,                           /* 8kHz PureVoice mono (QCELP) */
    kRTPPayload_MPEGAUDIO       = 14,                           /* MPEG I and II audio */
    kRTPPayload_DVI_11          = 16,                           /* 11kHz Intel DVI ADPCM mono */
    kRTPPayload_DVI_22          = 17                            /* 22kHz Intel DVI ADPCM mono */
};

/* RTP standard content encodings for video */
enum {
    kRTPPayload_CELLB           = 25,                           /* Sun CellB */
    kRTPPayload_JPEG            = 26,                           /* JPEG */
    kRTPPayload_CUSEEME         = 27,                           /* Cornell CU-SeeMe */
    kRTPPayload_NV              = 28,                           /* Xerox PARC nv */
    kRTPPayload_PICWIN          = 29,                           /* BBN Picture Window */
    kRTPPayload_CPV             = 30,                           /* Bolter CPV */
    kRTPPayload_H261            = 31,                           /* CCITT H.261 */
    kRTPPayload_MPEGVIDEO       = 32,                           /* MPEG I and II video */
    kRTPPayload_H263            = 34                            /* CCITT H.263 */
};

/* Other RTP standard content encodings */
enum {
    kRTPPayload_MPEG2T          = 33                            /* MPEG 2 Transport */
};

/* Dynamic encodings */
enum {
    kRTPPayload_FirstDynamic    = 96,
    kRTPPayload_LastDynamic     = 127,
    kRTPPayload_Unknown         = 0xFF
};



/*
-----------------------------------------
    RTP Info selectors
-----------------------------------------
*/
/* ----- these are get and set ----- */
enum {
    kRTPInfo_SSRC               = FOUR_CHAR_CODE('ssrc'),       /* UInt32* */
    kRTPInfo_NextSeqNum         = FOUR_CHAR_CODE('rnsn')        /* UInt16* */
};

/*-----------------------------------------
    RTP Statistics
-----------------------------------------*/
enum {
    kRTPTotalReceivedPktsStat   = FOUR_CHAR_CODE('trcp'),
    kRTPTotalLostPktsStat       = FOUR_CHAR_CODE('tlsp'),
    kRTPTotalProcessedPktsStat  = FOUR_CHAR_CODE('tprp'),
    kRTPTotalDroppedPktsStat    = FOUR_CHAR_CODE('tdrp'),
    kRTPBadHeaderDroppedPktsStat = FOUR_CHAR_CODE('bhdp'),
    kRTPOurHeaderDroppedPktsStat = FOUR_CHAR_CODE('ohdp'),
    kRTPNotReceivingSenderDroppedPktsStat = FOUR_CHAR_CODE('nsdp'),
    kRTPNotProcessingDroppedPktsStat = FOUR_CHAR_CODE('npdp'),
    kRTPBadSeqDroppedPktsStat   = FOUR_CHAR_CODE('bsdp'),
    kRTPArriveTooLatePktsStat   = FOUR_CHAR_CODE('artl'),
    kRTPWaitForSeqDroppedPktsStat = FOUR_CHAR_CODE('wsdp'),
    kRTPBadStateDroppedPktsStat = FOUR_CHAR_CODE('stdp'),
    kRTPBadPayloadDroppedPktsStat = FOUR_CHAR_CODE('bpdp'),
    kRTPNoTimeScaleDroppedPktsStat = FOUR_CHAR_CODE('ntdp'),
    kRTPDupSeqNumDroppedPktsStat = FOUR_CHAR_CODE('dsdp'),
    kRTPLostPktsPercentStat     = FOUR_CHAR_CODE('lspp'),
    kRTPDroppedPktsPercentStat  = FOUR_CHAR_CODE('dppp'),
    kRTPTotalUnprocessedPktsPercentStat = FOUR_CHAR_CODE('tupp'),
    kRTPRTCPDataRateStat        = FOUR_CHAR_CODE('rrcd'),
    kRTPPayloadIDStat           = FOUR_CHAR_CODE('rpid'),
    kRTPPayloadNameStat         = FOUR_CHAR_CODE('rpnm'),
    kRTPNumPktsInQueueStat      = FOUR_CHAR_CODE('rnpq'),
    kRTPTotalPktsInQueueStat    = FOUR_CHAR_CODE('rtpq'),
    kRTPTotalOutOfOrderPktsStat = FOUR_CHAR_CODE('rtoo'),
    kRTPRetransmissionStat      = FOUR_CHAR_CODE('rrtx')
};


/*-----------------------------------------
    Payload Info
-----------------------------------------*/
enum {
    kRTPPayloadSpeedTag         = FOUR_CHAR_CODE('sped'),       /* 0-255, 255 is fastest*/
    kRTPPayloadLossRecoveryTag  = FOUR_CHAR_CODE('loss'),       /* 0-255, 0 can't handle any loss, 128 can handle 50% packet loss*/
    kRTPPayloadConformanceTag   = FOUR_CHAR_CODE('conf')        /* more than one of these can be present*/
};



struct RTPPayloadCharacteristic {
    OSType                          tag;
    long                            value;
};
typedef struct RTPPayloadCharacteristic RTPPayloadCharacteristic;
/*
    pass RTPPayloadSortRequest to QTSFindMediaPacketizer or QTSFindMediaPacketizerForTrack.
    define the characteristics to sort by. tag is key to sort on. value is positive for ascending
    sort (low value first), negative for descending sort (high value first).
*/

struct RTPPayloadSortRequest {
    long                            characteristicCount;
    RTPPayloadCharacteristic        characteristic[1];          /* tag is key to sort on, value is + for ascending, - for descending*/
};
typedef struct RTPPayloadSortRequest    RTPPayloadSortRequest;
typedef RTPPayloadSortRequest *         RTPPayloadSortRequestPtr;
/* flags for RTPPayloadInfo */
enum {
    kRTPPayloadTypeStaticFlag   = 0x00000001,
    kRTPPayloadTypeDynamicFlag  = 0x00000002
};


struct RTPPayloadInfo {
    long                            payloadFlags;
    UInt8                           payloadID;
    char                            reserved1;
    char                            reserved2;
    char                            reserved3;
    char                            payloadName[1];
};
typedef struct RTPPayloadInfo           RTPPayloadInfo;
typedef RTPPayloadInfo *                RTPPayloadInfoPtr;
typedef RTPPayloadInfoPtr *             RTPPayloadInfoHandle;
/*============================================================================
        RTP Reassembler
============================================================================*/

typedef ComponentInstance               RTPReassembler;
enum {
    kRTPReassemblerType         = FOUR_CHAR_CODE('rtpr')
};

enum {
    kRTPBaseReassemblerType     = FOUR_CHAR_CODE('gnrc'),
    kRTP261ReassemblerType      = FOUR_CHAR_CODE('h261'),
    kRTP263ReassemblerType      = FOUR_CHAR_CODE('h263'),
    kRTP263PlusReassemblerType  = FOUR_CHAR_CODE('263+'),
    kRTPAudioReassemblerType    = FOUR_CHAR_CODE('soun'),
    kRTPQTReassemblerType       = FOUR_CHAR_CODE('qtim'),
    kRTPPureVoiceReassemblerType = FOUR_CHAR_CODE('Qclp'),
    kRTPJPEGReassemblerType     = FOUR_CHAR_CODE('jpeg'),
    kRTPQDesign2ReassemblerType = FOUR_CHAR_CODE('QDM2'),
    kRTPSorensonReassemblerType = FOUR_CHAR_CODE('SVQ1'),
    kRTPMP3ReassemblerType      = FOUR_CHAR_CODE('mp3 '),
    kRTPMPEG4AudioReassemblerType = FOUR_CHAR_CODE('mp4a'),
    kRTPMPEG4VideoReassemblerType = FOUR_CHAR_CODE('mp4v')
};


struct RTPRssmInitParams {
    RTPSSRC                         ssrc;
    UInt8                           payloadType;
    UInt8                           reserved1;
    UInt8                           reserved2;
    UInt8                           reserved3;
    TimeBase                        timeBase;
    TimeScale                       timeScale;
};
typedef struct RTPRssmInitParams        RTPRssmInitParams;

struct RTPDescParams {
    QTAtomContainer                 container;
    QTAtom                          presentationParentAtom;
    QTAtom                          streamParentAtom;
};
typedef struct RTPDescParams            RTPDescParams;

struct RTPRssmMoreInitParams {
    RTPRssmInitParams               initParams;
    SInt32                          version;
    RTPDescParams                   desc;
};
typedef struct RTPRssmMoreInitParams    RTPRssmMoreInitParams;
enum {
    kRTPRssmMoreInitParamsVersion1 = 1
};


/* get/set info selectors*/
enum {
    kRTPRssmInfo_MoreInitParams = FOUR_CHAR_CODE('rrmi')
};



struct RTPRssmPacket {
    struct RTPRssmPacket *          next;
    struct RTPRssmPacket *          prev;
    QTSStreamBuffer *               streamBuffer;
    Boolean                         paramsFilledIn;
    UInt8                           reserved;
    UInt16                          sequenceNum;
    UInt32                          transportHeaderLength;      /* filled in by base*/
    UInt32                          payloadHeaderLength;        /* derived adjusts this */
    UInt32                          dataLength;
    SHServerEditParameters          serverEditParams;
    TimeValue64                     timeStamp;                  /* lower 32 bits is original rtp timestamp*/
    SInt32                          chunkFlags;                 /* these are or'd together*/
    SInt32                          flags;

};
typedef struct RTPRssmPacket            RTPRssmPacket;
/* flags for RTPRssmPacket struct*/
enum {
    kRTPRssmPacketHasMarkerBitSet = 0x00000001,
    kRTPRssmPacketHasServerEditFlag = 0x00010000
};

/* flags for RTPRssmSendStreamBufferRange*/
enum {
    kRTPRssmCanRefStreamBuffer  = 0x00000001
};

/* flags for RTPRssmSendPacketList*/
enum {
    kRTPRssmLostSomePackets     = 0x00000001
};

/* flags for RTPRssmSetFlags*/
enum {
    kRTPRssmEveryPacketAChunkFlag = 0x00000001,
    kRTPRssmQueueAndUseMarkerBitFlag = 0x00000002,
    kRTPRssmTrackLostPacketsFlag = 0x00010000,
    kRTPRssmNoReorderingRequiredFlag = 0x00020000
};



struct RTPSendStreamBufferRangeParams {
    QTSStreamBuffer *               streamBuffer;
    TimeValue64                     presentationTime;
    UInt32                          chunkStartPosition;
    UInt32                          numDataBytes;
    SInt32                          chunkFlags;
    SInt32                          flags;
    const SHServerEditParameters *  serverEditParams;           /* NULL if no edit*/
};
typedef struct RTPSendStreamBufferRangeParams RTPSendStreamBufferRangeParams;
/* characteristics*/
enum {
    kRTPCharacteristic_RequiresOrderedPackets = FOUR_CHAR_CODE('rrop'),
    kRTPCharacteristic_TimeStampsNotMonoIncreasing = FOUR_CHAR_CODE('tsmi')
};


enum {
    kRTPReassemblerInfoResType  = FOUR_CHAR_CODE('rsmi')
};


struct RTPReassemblerInfo {
    long                            characteristicCount;
    RTPPayloadCharacteristic        characteristic[1];

                                                                /* after the last characteristic, the payload name (defined by the MediaPacketizerPayloadInfo*/
                                                                /* structure) is present. */
};
typedef struct RTPReassemblerInfo       RTPReassemblerInfo;
typedef RTPReassemblerInfo *            RTPReassemblerInfoPtr;
typedef RTPReassemblerInfoPtr *         RTPReassemblerInfoHandle;
#define RTPReassemblerInfoToPayloadInfo(_rsmi) ((RTPPayloadInfoPtr)(&((_rsmi)->characteristic[(_rsmi)->characteristicCount])))
/* RTPReassemblerInfoElement structs are padded to 32 bits */
enum {
    kRTPReassemblerInfoPadUpToBytes = 4
};


EXTERN_API_C( OSErr )
QTSFindReassemblerForPayloadID  (UInt8                  inPayloadID,
                                 RTPPayloadSortRequest * inSortInfo,
                                 QTAtomContainer *      outReassemblerList);

EXTERN_API_C( OSErr )
QTSFindReassemblerForPayloadName (const char *          inPayloadName,
                                 RTPPayloadSortRequest * inSortInfo,
                                 QTAtomContainer *      outReassemblerList);

/*-----------------------------------------
    RTP Reassembler Selectors
-----------------------------------------*/
enum {
    kRTPRssmSetCapabilitiesSelect = 0x0100,
    kRTPRssmGetCapabilitiesSelect = 0x0101,
    kRTPRssmSetPayloadHeaderLengthSelect = 0x0102,
    kRTPRssmGetPayloadHeaderLengthSelect = 0x0103,
    kRTPRssmSetTimeScaleSelect  = 0x0104,
    kRTPRssmGetTimeScaleSelect  = 0x0105,
    kRTPRssmNewStreamHandlerSelect = 0x0106,
    kRTPRssmSetStreamHandlerSelect = 0x0107,
    kRTPRssmGetStreamHandlerSelect = 0x0108,
    kRTPRssmSendStreamHandlerChangedSelect = 0x0109,
    kRTPRssmSetSampleDescriptionSelect = 0x010A,
    kRTPRssmGetChunkAndIncrRefCountSelect = 0x010D,
    kRTPRssmSendChunkAndDecrRefCountSelect = 0x010E,
    kRTPRssmSendLostChunkSelect = 0x010F,
    kRTPRssmSendStreamBufferRangeSelect = 0x0110,
    kRTPRssmClearCachedPackets  = 0x0111,
    kRTPRssmFillPacketListParamsSelect = 0x0113,
    kRTPRssmReleasePacketListSelect = 0x0114,
    kRTPRssmIncrChunkRefCountSelect = 0x0115,
    kRTPRssmDecrChunkRefCountSelect = 0x0116,
    kRTPRssmGetExtChunkAndIncrRefCountSelect = 0x0117,
    kRTPRssmInitializeSelect    = 0x0500,
    kRTPRssmHandleNewPacketSelect = 0x0501,
    kRTPRssmComputeChunkSizeSelect = 0x0502,
    kRTPRssmAdjustPacketParamsSelect = 0x0503,
    kRTPRssmCopyDataToChunkSelect = 0x0504,
    kRTPRssmSendPacketListSelect = 0x0505,
    kRTPRssmGetTimeScaleFromPacketSelect = 0x0506,
    kRTPRssmSetInfoSelect       = 0x0509,
    kRTPRssmGetInfoSelect       = 0x050A,
    kRTPRssmHasCharacteristicSelect = 0x050B,
    kRTPRssmResetSelect         = 0x050C
};

/*-----------------------------------------
    RTP Reassembler functions - base to derived
-----------------------------------------*/

EXTERN_API( ComponentResult )
RTPRssmInitialize               (RTPReassembler         rtpr,
                                 RTPRssmInitParams *    inInitParams)                       FIVEWORDINLINE(0x2F3C, 0x0004, 0x0500, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmHandleNewPacket          (RTPReassembler         rtpr,
                                 QTSStreamBuffer *      inStreamBuffer,
                                 SInt32                 inNumWraparounds)                   FIVEWORDINLINE(0x2F3C, 0x0008, 0x0501, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmComputeChunkSize         (RTPReassembler         rtpr,
                                 RTPRssmPacket *        inPacketListHead,
                                 SInt32                 inFlags,
                                 UInt32 *               outChunkDataSize)                   FIVEWORDINLINE(0x2F3C, 0x000C, 0x0502, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmAdjustPacketParams       (RTPReassembler         rtpr,
                                 RTPRssmPacket *        inPacket,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x0503, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmCopyDataToChunk          (RTPReassembler         rtpr,
                                 RTPRssmPacket *        inPacketListHead,
                                 UInt32                 inMaxChunkDataSize,
                                 SHChunkRecord *        inChunk,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0010, 0x0504, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmSendPacketList           (RTPReassembler         rtpr,
                                 RTPRssmPacket *        inPacketListHead,
                                 const TimeValue64 *    inLastChunkPresentationTime,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x000C, 0x0505, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmGetTimeScaleFromPacket   (RTPReassembler         rtpr,
                                 QTSStreamBuffer *      inStreamBuffer,
                                 TimeScale *            outTimeScale)                       FIVEWORDINLINE(0x2F3C, 0x0008, 0x0506, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmSetInfo                  (RTPReassembler         rtpr,
                                 OSType                 inSelector,
                                 void *                 ioParams)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0509, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmGetInfo                  (RTPReassembler         rtpr,
                                 OSType                 inSelector,
                                 void *                 ioParams)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x050A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmHasCharacteristic        (RTPReassembler         rtpr,
                                 OSType                 inCharacteristic,
                                 Boolean *              outHasIt)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x050B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmReset                    (RTPReassembler         rtpr,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x050C, 0x7000, 0xA82A);

/*-----------------------------------------
    RTP Reassembler functions - derived to base
-----------------------------------------*/
/* ----- setup*/
EXTERN_API( ComponentResult )
RTPRssmSetCapabilities          (RTPReassembler         rtpr,
                                 SInt32                 inFlags,
                                 SInt32                 inFlagsMask)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x0100, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmGetCapabilities          (RTPReassembler         rtpr,
                                 SInt32 *               outFlags)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmSetPayloadHeaderLength   (RTPReassembler         rtpr,
                                 UInt32                 inPayloadHeaderLength)              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmGetPayloadHeaderLength   (RTPReassembler         rtpr,
                                 UInt32 *               outPayloadHeaderLength)             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmSetTimeScale             (RTPReassembler         rtpr,
                                 TimeScale              inSHTimeScale)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x0104, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmGetTimeScale             (RTPReassembler         rtpr,
                                 TimeScale *            outSHTimeScale)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0105, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmNewStreamHandler         (RTPReassembler         rtpr,
                                 OSType                 inSHType,
                                 SampleDescriptionHandle  inSampleDescription,
                                 TimeScale              inSHTimeScale,
                                 ComponentInstance *    outHandler)                         FIVEWORDINLINE(0x2F3C, 0x0010, 0x0106, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmSetStreamHandler         (RTPReassembler         rtpr,
                                 ComponentInstance      inStreamHandler)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0107, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmGetStreamHandler         (RTPReassembler         rtpr,
                                 ComponentInstance *    outStreamHandler)                   FIVEWORDINLINE(0x2F3C, 0x0004, 0x0108, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
RTPRssmSendStreamHandlerChanged (RTPReassembler         rtpr)                               FIVEWORDINLINE(0x2F3C, 0x0000, 0x0109, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmSetSampleDescription     (RTPReassembler         rtpr,
                                 SampleDescriptionHandle  inSampleDescription)              FIVEWORDINLINE(0x2F3C, 0x0004, 0x010A, 0x7000, 0xA82A);

/* ----- manually sending chunks*/
EXTERN_API( ComponentResult )
RTPRssmGetChunkAndIncrRefCount  (RTPReassembler         rtpr,
                                 UInt32                 inChunkDataSize,
                                 const TimeValue64 *    inChunkPresentationTime,
                                 SHChunkRecord **       outChunk)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x010D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmGetExtChunkAndIncrRefCount (RTPReassembler       rtpr,
                                 UInt32                 inChunkDataSize,
                                 const TimeValue64 *    inChunkPresentationTime,
                                 SInt32                 inFlags,
                                 SHExtendedChunkRecord ** outChunk)                         FIVEWORDINLINE(0x2F3C, 0x0010, 0x0117, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmSendChunkAndDecrRefCount (RTPReassembler         rtpr,
                                 SHChunkRecord *        inChunk,
                                 const SHServerEditParameters * inServerEdit)               FIVEWORDINLINE(0x2F3C, 0x0008, 0x010E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmSendLostChunk            (RTPReassembler         rtpr,
                                 const TimeValue64 *    inChunkPresentationTime)            FIVEWORDINLINE(0x2F3C, 0x0004, 0x010F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmSendStreamBufferRange    (RTPReassembler         rtpr,
                                 RTPSendStreamBufferRangeParams * inParams)                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0110, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmClearCachedPackets       (RTPReassembler         rtpr,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0111, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmFillPacketListParams     (RTPReassembler         rtpr,
                                 RTPRssmPacket *        inPacketListHead,
                                 SInt32                 inNumWraparounds,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x000C, 0x0113, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmReleasePacketList        (RTPReassembler         rtpr,
                                 RTPRssmPacket *        inPacketListHead)                   FIVEWORDINLINE(0x2F3C, 0x0004, 0x0114, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmIncrChunkRefCount        (RTPReassembler         rtpr,
                                 SHChunkRecord *        inChunk)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0115, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPRssmDecrChunkRefCount        (RTPReassembler         rtpr,
                                 SHChunkRecord *        inChunk)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0116, 0x7000, 0xA82A);

/*============================================================================
        RTP Media Packetizer
============================================================================*/
enum {
    kRTPMediaPacketizerType     = FOUR_CHAR_CODE('rtpm')
};


typedef ComponentInstance               RTPMediaPacketizer;
enum {
    kRTPBaseMediaPacketizerType = FOUR_CHAR_CODE('gnrc'),
    kRTP261MediaPacketizerType  = FOUR_CHAR_CODE('h261'),
    kRTP263PlusMediaPacketizerType = FOUR_CHAR_CODE('263+'),
    kRTPAudioMediaPacketizerType = FOUR_CHAR_CODE('soun'),
    kRTPQTMediaPacketizerType   = FOUR_CHAR_CODE('qtim'),
    kRTPPureVoiceMediaPacketizerType = FOUR_CHAR_CODE('Qclp'),
    kRTPJPEGMediaPacketizerType = FOUR_CHAR_CODE('jpeg'),
    kRTPQDesign2MediaPacketizerType = FOUR_CHAR_CODE('QDM2'),
    kRTPSorensonMediaPacketizerType = FOUR_CHAR_CODE('SVQ1'),
    kRTPMP3MediaPacketizerType  = FOUR_CHAR_CODE('mp3 '),
    kRTPMPEG4AudioMediaPacketizerType = FOUR_CHAR_CODE('mp4a'),
    kRTPMPEG4VideoMediaPacketizerType = FOUR_CHAR_CODE('mp4v')
};

typedef UInt32                          RTPMPSampleRef;
typedef CALLBACK_API( void , RTPMPDataReleaseProcPtr )(UInt8 *inData, void *inRefCon);
typedef STACK_UPP_TYPE(RTPMPDataReleaseProcPtr)                 RTPMPDataReleaseUPP;
enum {
    kMediaPacketizerCanPackEditRate = 1 << 0,
    kMediaPacketizerCanPackLayer = 1 << 1,
    kMediaPacketizerCanPackVolume = 1 << 2,
    kMediaPacketizerCanPackBalance = 1 << 3,
    kMediaPacketizerCanPackGraphicsMode = 1 << 4,
    kMediaPacketizerCanPackEmptyEdit = 1 << 5
};



struct MediaPacketizerRequirements {
    OSType                          mediaType;                  /* media type supported (0 for all)*/
    OSType                          dataFormat;                 /* data format (e.g., compression) supported (0 for all)*/
    UInt32                          capabilityFlags;            /* ability to handle non-standard track characteristics*/
    UInt8                           canPackMatrixType;          /* can pack any matrix type up to this (identityMatrixType for identity only)*/
    UInt8                           reserved1;
    UInt8                           reserved2;
    UInt8                           reserved3;
};
typedef struct MediaPacketizerRequirements MediaPacketizerRequirements;
typedef MediaPacketizerRequirements *   MediaPacketizerRequirementsPtr;

struct MediaPacketizerInfo {
    OSType                          mediaType;                  /* media type supported (0 for all)*/
    OSType                          dataFormat;                 /* data format (e.g., compression) supported (0 for all)*/
    OSType                          vendor;                     /* manufacturer of this packetizer (e.g., 'appl' for Apple)*/
    UInt32                          capabilityFlags;            /* ability to handle non-standard track characteristics*/
    UInt8                           canPackMatrixType;          /* can pack any matrix type up to this (identityMatrixType for identity only)*/
    UInt8                           reserved1;
    UInt8                           reserved2;
    UInt8                           reserved3;
    long                            characteristicCount;
    RTPPayloadCharacteristic        characteristic[1];

                                                                /* after the last characteristic, the payload name (defined by the RTPPayloadInfo*/
                                                                /* structure) is present. */
};
typedef struct MediaPacketizerInfo      MediaPacketizerInfo;
typedef MediaPacketizerInfo *           MediaPacketizerInfoPtr;
typedef MediaPacketizerInfoPtr *        MediaPacketizerInfoHandle;
#define MediaPacketizerInfoToPayloadInfo(_mpi) ((RTPPayloadInfoPtr)(&((_mpi)->characteristic[(_mpi)->characteristicCount])))
/* MediaPacketizerInfo structs are padded to 32 bits */
enum {
    kMediaPacketizerInfoPadUpToBytes = 4
};

enum {
    kRTPMediaPacketizerInfoRezType = FOUR_CHAR_CODE('pcki')
};


EXTERN_API_C( OSErr )
QTSFindMediaPacketizer          (MediaPacketizerRequirementsPtr  inPacketizerinfo,
                                 SampleDescriptionHandle  inSampleDescription,
                                 RTPPayloadSortRequestPtr  inSortInfo,
                                 QTAtomContainer *      outPacketizerList);

EXTERN_API_C( OSErr )
QTSFindMediaPacketizerForTrack  (Track                  inTrack,
                                 long                   inSampleDescriptionIndex,
                                 RTPPayloadSortRequestPtr  inSortInfo,
                                 QTAtomContainer *      outPacketizerList);

EXTERN_API_C( OSErr )
QTSFindMediaPacketizerForPayloadID (long                payloadID,
                                 RTPPayloadSortRequestPtr  inSortInfo,
                                 QTAtomContainer *      outPacketizerList);

EXTERN_API_C( OSErr )
QTSFindMediaPacketizerForPayloadName (const char *      payloadName,
                                 RTPPayloadSortRequestPtr  inSortInfo,
                                 QTAtomContainer *      outPacketizerList);

/* flags for RTPMPInitialize*/
enum {
    kRTPMPRealtimeModeFlag      = 0x00000001
};

/* flags for RTPMPSampleDataParams*/
enum {
    kRTPMPSyncSampleFlag        = 0x00000001,
    kRTPMPRespectDurationFlag   = 0x00000002
};


struct RTPMPSampleDataParams {
    UInt32                          version;
    UInt32                          timeStamp;
    UInt32                          duration;                   /* 0 = unknown duration*/
    UInt32                          playOffset;
    Fixed                           playRate;
    SInt32                          flags;
    UInt32                          sampleDescSeed;
    Handle                          sampleDescription;
    RTPMPSampleRef                  sampleRef;
    UInt32                          dataLength;
    const UInt8 *                   data;
    RTPMPDataReleaseUPP             releaseProc;
    void *                          refCon;
};
typedef struct RTPMPSampleDataParams    RTPMPSampleDataParams;
/* out flags for idle, RTPMPSetSampleData, and RTPMPFlush*/
enum {
    kRTPMPStillProcessingData   = 0x00000001                    /* not done with data you've got*/
};


struct RTPMPPayloadTypeParams {
    UInt32                          flags;
    UInt32                          payloadNumber;
    short                           nameLength;                 /* in: size of payloadName buffer (counting null terminator) -- this will be reset to needed length and paramErr returned if too small */
    char *                          payloadName;                /* caller must provide buffer */
};
typedef struct RTPMPPayloadTypeParams   RTPMPPayloadTypeParams;
/*-----------------------------------------
    RTP Media Packetizer Info selectors
-----------------------------------------*/
/* info selectors - get only */
enum {
    kRTPMPPayloadTypeInfo       = FOUR_CHAR_CODE('rtpp'),       /* RTPMPPayloadTypeParams* */
    kRTPMPRTPTimeScaleInfo      = FOUR_CHAR_CODE('rtpt'),       /* TimeScale* */
    kRTPMPRequiredSampleDescriptionInfo = FOUR_CHAR_CODE('sdsc'), /* SampleDescriptionHandle* */
    kRTPMPMinPayloadSize        = FOUR_CHAR_CODE('mins'),       /* UInt32* in bytes, does not include rtp header; default is 0 */
    kRTPMPMinPacketDuration     = FOUR_CHAR_CODE('mind'),       /* UInt3* in milliseconds; default is no min required */
    kRTPMPSuggestedRepeatPktCountInfo = FOUR_CHAR_CODE('srpc'), /* UInt32* */
    kRTPMPSuggestedRepeatPktSpacingInfo = FOUR_CHAR_CODE('srps'), /* UInt32* in milliseconds */
    kRTPMPMaxPartialSampleSizeInfo = FOUR_CHAR_CODE('mpss'),    /* UInt32* in bytes */
    kRTPMPPreferredBufferDelayInfo = FOUR_CHAR_CODE('prbd'),    /* UInt32* in milliseconds */
    kRTPMPPayloadNameInfo       = FOUR_CHAR_CODE('name'),       /* StringPtr */
    kRTPInfo_FormatString       = FOUR_CHAR_CODE('fmtp')        /* char **, caller allocates ptr, callee disposes */
};

/*-----------------------------------------
    RTP Media Packetizer Characteristics
-----------------------------------------*/
/* also supports relevant ones in Movies.h and QTSToolbox.h */
enum {
    kRTPMPNoSampleDataRequiredCharacteristic = FOUR_CHAR_CODE('nsdr'),
    kRTPMPHasUserSettingsDialogCharacteristic = FOUR_CHAR_CODE('sdlg'),
    kRTPMPPrefersReliableTransportCharacteristic = FOUR_CHAR_CODE('rely'),
    kRTPMPRequiresOutOfBandDimensionsCharacteristic = FOUR_CHAR_CODE('robd'),
    kRTPMPReadsPartialSamplesCharacteristic = FOUR_CHAR_CODE('rpsp')
};

/*-----------------------------------------
    RTP Media Packetizer selectors
-----------------------------------------*/
enum {
    kRTPMPInitializeSelect      = 0x0500,
    kRTPMPPreflightMediaSelect  = 0x0501,
    kRTPMPIdleSelect            = 0x0502,
    kRTPMPSetSampleDataSelect   = 0x0503,
    kRTPMPFlushSelect           = 0x0504,
    kRTPMPResetSelect           = 0x0505,
    kRTPMPSetInfoSelect         = 0x0506,
    kRTPMPGetInfoSelect         = 0x0507,
    kRTPMPSetTimeScaleSelect    = 0x0508,
    kRTPMPGetTimeScaleSelect    = 0x0509,
    kRTPMPSetTimeBaseSelect     = 0x050A,
    kRTPMPGetTimeBaseSelect     = 0x050B,
    kRTPMPHasCharacteristicSelect = 0x050C,
    kRTPMPSetPacketBuilderSelect = 0x050E,
    kRTPMPGetPacketBuilderSelect = 0x050F,
    kRTPMPSetMediaTypeSelect    = 0x0510,
    kRTPMPGetMediaTypeSelect    = 0x0511,
    kRTPMPSetMaxPacketSizeSelect = 0x0512,
    kRTPMPGetMaxPacketSizeSelect = 0x0513,
    kRTPMPSetMaxPacketDurationSelect = 0x0514,
    kRTPMPGetMaxPacketDurationSelect = 0x0515,                  /* for export component and apps who want to*/
                                                                /* access dialogs for Media-specific settings*/
                                                                /* (such as Pure Voice interleave factor)*/
    kRTPMPDoUserDialogSelect    = 0x0516,
    kRTPMPSetSettingsFromAtomContainerAtAtomSelect = 0x0517,
    kRTPMPGetSettingsIntoAtomContainerAtAtomSelect = 0x0518,
    kRTPMPGetSettingsAsTextSelect = 0x0519,
    kRTPMPGetSettingsSelect     = 0x051A,
    kRTPMPSetSettingsSelect     = 0x051B
};

/*-----------------------------------------
    RTP Media Packetizer functions
-----------------------------------------*/

EXTERN_API( ComponentResult )
RTPMPInitialize                 (RTPMediaPacketizer     rtpm,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0500, 0x7000, 0xA82A);

/* return noErr if you can handle this media */
EXTERN_API( ComponentResult )
RTPMPPreflightMedia             (RTPMediaPacketizer     rtpm,
                                 OSType                 inMediaType,
                                 SampleDescriptionHandle  inSampleDescription)              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0501, 0x7000, 0xA82A);

/*
   do work here if you need to - give up time periodically
   if you're doing time consuming operations
*/
EXTERN_API( ComponentResult )
RTPMPIdle                       (RTPMediaPacketizer     rtpm,
                                 SInt32                 inFlags,
                                 SInt32 *               outFlags)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0502, 0x7000, 0xA82A);

/*
   caller owns the RTPMPSampleDataParams struct
   media Packetizer must copy any fields of the struct it wants to keep
   media Packetizer must call release proc when done with the data
   you can do the processing work here if it does not take up too
   much cpu time - otherwise do it in idle
*/
EXTERN_API( ComponentResult )
RTPMPSetSampleData              (RTPMediaPacketizer     rtpm,
                                 const RTPMPSampleDataParams * inSampleData,
                                 SInt32 *               outFlags)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0503, 0x7000, 0xA82A);

/*
   send everything you have buffered - you will get idles while
   you set the kRTPMPStillProcessingData flag here and in idle
*/
EXTERN_API( ComponentResult )
RTPMPFlush                      (RTPMediaPacketizer     rtpm,
                                 SInt32                 inFlags,
                                 SInt32 *               outFlags)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0504, 0x7000, 0xA82A);

/*
   dispose of anything buffered and get rid of state
   do not send the buffered data (because presumably
   there is no connection for you to send on)
   state should be the same as if you were just initialized
*/
EXTERN_API( ComponentResult )
RTPMPReset                      (RTPMediaPacketizer     rtpm,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0505, 0x7000, 0xA82A);

/*-----------------------------------------
    RTP Media Packetizer get / set functions
-----------------------------------------*/
EXTERN_API( ComponentResult )
RTPMPSetInfo                    (RTPMediaPacketizer     rtpm,
                                 OSType                 inSelector,
                                 const void *           ioParams)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0506, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPGetInfo                    (RTPMediaPacketizer     rtpm,
                                 OSType                 inSelector,
                                 void *                 ioParams)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0507, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPSetTimeScale               (RTPMediaPacketizer     rtpm,
                                 TimeScale              inTimeScale)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x0508, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPGetTimeScale               (RTPMediaPacketizer     rtpm,
                                 TimeScale *            outTimeScale)                       FIVEWORDINLINE(0x2F3C, 0x0004, 0x0509, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPSetTimeBase                (RTPMediaPacketizer     rtpm,
                                 TimeBase               inTimeBase)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x050A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPGetTimeBase                (RTPMediaPacketizer     rtpm,
                                 TimeBase *             outTimeBase)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x050B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPHasCharacteristic          (RTPMediaPacketizer     rtpm,
                                 OSType                 inSelector,
                                 Boolean *              outHasIt)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x050C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPSetPacketBuilder           (RTPMediaPacketizer     rtpm,
                                 ComponentInstance      inPacketBuilder)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x050E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPGetPacketBuilder           (RTPMediaPacketizer     rtpm,
                                 ComponentInstance *    outPacketBuilder)                   FIVEWORDINLINE(0x2F3C, 0x0004, 0x050F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPSetMediaType               (RTPMediaPacketizer     rtpm,
                                 OSType                 inMediaType)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x0510, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPGetMediaType               (RTPMediaPacketizer     rtpm,
                                 OSType *               outMediaType)                       FIVEWORDINLINE(0x2F3C, 0x0004, 0x0511, 0x7000, 0xA82A);

/* size is in bytes*/
EXTERN_API( ComponentResult )
RTPMPSetMaxPacketSize           (RTPMediaPacketizer     rtpm,
                                 UInt32                 inMaxPacketSize)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0512, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPGetMaxPacketSize           (RTPMediaPacketizer     rtpm,
                                 UInt32 *               outMaxPacketSize)                   FIVEWORDINLINE(0x2F3C, 0x0004, 0x0513, 0x7000, 0xA82A);

/* duration is in milliseconds*/
EXTERN_API( ComponentResult )
RTPMPSetMaxPacketDuration       (RTPMediaPacketizer     rtpm,
                                 UInt32                 inMaxPacketDuration)                FIVEWORDINLINE(0x2F3C, 0x0004, 0x0514, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPGetMaxPacketDuration       (RTPMediaPacketizer     rtpm,
                                 UInt32 *               outMaxPacketDuration)               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0515, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPDoUserDialog               (RTPMediaPacketizer     rtpm,
                                 ModalFilterUPP         inFilterUPP,
                                 Boolean *              canceled)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0516, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPSetSettingsFromAtomContainerAtAtom (RTPMediaPacketizer  rtpm,
                                 QTAtomContainer        inContainer,
                                 QTAtom                 inParentAtom)                       FIVEWORDINLINE(0x2F3C, 0x0008, 0x0517, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPGetSettingsIntoAtomContainerAtAtom (RTPMediaPacketizer  rtpm,
                                 QTAtomContainer        inOutContainer,
                                 QTAtom                 inParentAtom)                       FIVEWORDINLINE(0x2F3C, 0x0008, 0x0518, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPGetSettingsAsText          (RTPMediaPacketizer     rtpm,
                                 Handle *               text)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0519, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
RTPMPGetSettings                (RTPMediaPacketizer     rtpm,
                                 QTAtomContainer *      outSettings,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x051A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPMPSetSettings                (RTPMediaPacketizer     rtpm,
                                 QTAtomSpecPtr          inSettings,
                                 SInt32                 inFlags)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x051B, 0x7000, 0xA82A);

/*============================================================================
        RTP Packet Builder
============================================================================*/
enum {
    kRTPPacketBuilderType       = FOUR_CHAR_CODE('rtpb')
};


typedef ComponentInstance               RTPPacketBuilder;
typedef struct OpaqueRTPPacketGroupRef*  RTPPacketGroupRef;
typedef struct OpaqueRTPPacketRef*      RTPPacketRef;
typedef struct OpaqueRTPPacketRepeatedDataRef*  RTPPacketRepeatedDataRef;
/* flags for RTPPBBegin/EndPacket, RTPPBBegin/EndPacketGroup*/
enum {
    kRTPPBSetMarkerFlag         = 0x00000001,
    kRTPPBRepeatPacketFlag      = 0x00000002,
    kRTPPBSyncSampleFlag        = 0x00010000,
    kRTPPBBFrameFlag            = 0x00020000,
    kRTPPBDontSendFlag          = 0x10000000                    /* when set in EndPacketGroup, will not add group*/
};

enum {
    kRTPPBUnknownPacketMediaDataLength = 0
};

/* flags for RTPPBGetSampleData*/
enum {
    kRTPPBEndOfDataFlag         = 0x00000001
};


typedef CALLBACK_API( void , RTPPBCallbackProcPtr )(OSType inSelector, void *ioParams, void *inRefCon);
typedef STACK_UPP_TYPE(RTPPBCallbackProcPtr)                    RTPPBCallbackUPP;
/*-----------------------------------------
    RTP Packet Builder selectors
-----------------------------------------*/
enum {
    kRTPPBBeginPacketGroupSelect = 0x0500,
    kRTPPBEndPacketGroupSelect  = 0x0501,
    kRTPPBBeginPacketSelect     = 0x0502,
    kRTPPBEndPacketSelect       = 0x0503,
    kRTPPBAddPacketLiteralDataSelect = 0x0504,
    kRTPPBAddPacketSampleDataSelect = 0x0505,
    kRTPPBAddPacketRepeatedDataSelect = 0x0506,
    kRTPPBReleaseRepeatedDataSelect = 0x0507,
    kRTPPBSetPacketSequenceNumberSelect = 0x0508,
    kRTPPBGetPacketSequenceNumberSelect = 0x0509,
    kRTPPBSetCallbackSelect     = 0x050A,
    kRTPPBGetCallbackSelect     = 0x050B,
    kRTPPBSetInfoSelect         = 0x050C,
    kRTPPBGetInfoSelect         = 0x050D,
    kRTPPBSetPacketTimeStampOffsetSelect = 0x050E,
    kRTPPBGetPacketTimeStampOffsetSelect = 0x050F,
    kRTPPBAddPacketSampleData64Select = 0x0510,
    kRTPPBGetSampleDataSelect   = 0x0511,
    kRTPPBAddRepeatPacketSelect = 0x0512
};

/*-----------------------------------------
    RTP Packet Builder functions
-----------------------------------------*/
EXTERN_API( ComponentResult )
RTPPBBeginPacketGroup           (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 UInt32                 inTimeStamp,
                                 RTPPacketGroupRef *    outPacketGroup)                     FIVEWORDINLINE(0x2F3C, 0x000C, 0x0500, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPPBEndPacketGroup             (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup)                      FIVEWORDINLINE(0x2F3C, 0x0008, 0x0501, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPPBBeginPacket                (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup,
                                 UInt32                 inPacketMediaDataLength,
                                 RTPPacketRef *         outPacket)                          FIVEWORDINLINE(0x2F3C, 0x0010, 0x0502, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPPBEndPacket                  (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup,
                                 RTPPacketRef           inPacket,
                                 UInt32                 inTransmissionTimeOffset,
                                 UInt32                 inDuration)                         FIVEWORDINLINE(0x2F3C, 0x0014, 0x0503, 0x7000, 0xA82A);

/*
   non-NULL RTPPacketRepeatedDataRef means this data will be repeated later
   pb must return a repeated data ref
*/
EXTERN_API( ComponentResult )
RTPPBAddPacketLiteralData       (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup,
                                 RTPPacketRef           inPacket,
                                 UInt8 *                inData,
                                 UInt32                 inDataLength,
                                 RTPPacketRepeatedDataRef * outDataRef)                     FIVEWORDINLINE(0x2F3C, 0x0018, 0x0504, 0x7000, 0xA82A);

/*
   non-NULL RTPPacketRepeatedDataRef means this data will be repeated later
   pb must return a repeated data ref
*/
EXTERN_API( ComponentResult )
RTPPBAddPacketSampleData        (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup,
                                 RTPPacketRef           inPacket,
                                 RTPMPSampleDataParams * inSampleDataParams,
                                 UInt32                 inSampleOffset,
                                 UInt32                 inSampleDataLength,
                                 RTPPacketRepeatedDataRef * outDataRef)                     FIVEWORDINLINE(0x2F3C, 0x001C, 0x0505, 0x7000, 0xA82A);

/*
   non-NULL RTPPacketRepeatedDataRef means this data will be repeated later
   pb must return a repeated data ref
*/
EXTERN_API( ComponentResult )
RTPPBAddPacketSampleData64      (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup,
                                 RTPPacketRef           inPacket,
                                 RTPMPSampleDataParams * inSampleDataParams,
                                 const UInt64 *         inSampleOffset,
                                 UInt32                 inSampleDataLength,
                                 RTPPacketRepeatedDataRef * outDataRef)                     FIVEWORDINLINE(0x2F3C, 0x001C, 0x0510, 0x7000, 0xA82A);

/*
   call to add the repeated data using the ref you got from
   RTPPBAddPacketLiteralData or RTPPBAddPacketSampleData
*/
EXTERN_API( ComponentResult )
RTPPBAddPacketRepeatedData      (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup,
                                 RTPPacketRef           inPacket,
                                 RTPPacketRepeatedDataRef  inDataRef)                       FIVEWORDINLINE(0x2F3C, 0x0010, 0x0506, 0x7000, 0xA82A);

/* call when done with repeated data*/
EXTERN_API( ComponentResult )
RTPPBReleaseRepeatedData        (RTPPacketBuilder       rtpb,
                                 RTPPacketRepeatedDataRef  inDataRef)                       FIVEWORDINLINE(0x2F3C, 0x0004, 0x0507, 0x7000, 0xA82A);

/*
   seq number is just relative seq number
   don't call if you don't care when seq # is used
*/
EXTERN_API( ComponentResult )
RTPPBSetPacketSequenceNumber    (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup,
                                 RTPPacketRef           inPacket,
                                 UInt32                 inSequenceNumber)                   FIVEWORDINLINE(0x2F3C, 0x0010, 0x0508, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPPBGetPacketSequenceNumber    (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup,
                                 RTPPacketRef           inPacket,
                                 UInt32 *               outSequenceNumber)                  FIVEWORDINLINE(0x2F3C, 0x0010, 0x0509, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPPBSetPacketTimeStampOffset   (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup,
                                 RTPPacketRef           inPacket,
                                 SInt32                 inTimeStampOffset)                  FIVEWORDINLINE(0x2F3C, 0x0010, 0x050E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPPBGetPacketTimeStampOffset   (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup,
                                 RTPPacketRef           inPacket,
                                 SInt32 *               outTimeStampOffset)                 FIVEWORDINLINE(0x2F3C, 0x0010, 0x050F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPPBAddRepeatPacket            (RTPPacketBuilder       rtpb,
                                 SInt32                 inFlags,
                                 RTPPacketGroupRef      inPacketGroup,
                                 RTPPacketRef           inPacket,
                                 TimeValue              inTransmissionOffset,
                                 UInt32                 inSequenceNumber)                   FIVEWORDINLINE(0x2F3C, 0x0014, 0x0512, 0x7000, 0xA82A);

/*
   used for communicating with the caller of the media packetizers if needed
   NOT used for communicating with the media packetizers themselves
*/
EXTERN_API( ComponentResult )
RTPPBSetCallback                (RTPPacketBuilder       rtpb,
                                 RTPPBCallbackUPP       inCallback,
                                 void *                 inRefCon)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x050A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPPBGetCallback                (RTPPacketBuilder       rtpb,
                                 RTPPBCallbackUPP *     outCallback,
                                 void **                outRefCon)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x050B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPPBSetInfo                    (RTPPacketBuilder       rtpb,
                                 OSType                 inSelector,
                                 void *                 ioParams)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x050C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPPBGetInfo                    (RTPPacketBuilder       rtpb,
                                 OSType                 inSelector,
                                 void *                 ioParams)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x050D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
RTPPBGetSampleData              (RTPPacketBuilder       rtpb,
                                 RTPMPSampleDataParams * inParams,
                                 const UInt64 *         inStartOffset,
                                 UInt8 *                outDataBuffer,
                                 UInt32                 inBytesToRead,
                                 UInt32 *               outBytesRead,
                                 SInt32 *               outFlags)                           FIVEWORDINLINE(0x2F3C, 0x0018, 0x0511, 0x7000, 0xA82A);


/* UPP call backs */
#if OPAQUE_UPP_TYPES
    EXTERN_API(RTPMPDataReleaseUPP)
    NewRTPMPDataReleaseUPP         (RTPMPDataReleaseProcPtr userRoutine);

    EXTERN_API(RTPPBCallbackUPP)
    NewRTPPBCallbackUPP            (RTPPBCallbackProcPtr    userRoutine);

    EXTERN_API(void)
    DisposeRTPMPDataReleaseUPP     (RTPMPDataReleaseUPP     userUPP);

    EXTERN_API(void)
    DisposeRTPPBCallbackUPP        (RTPPBCallbackUPP        userUPP);

    EXTERN_API(void)
    InvokeRTPMPDataReleaseUPP      (UInt8 *                 inData,
                                    void *                  inRefCon,
                                    RTPMPDataReleaseUPP     userUPP);

    EXTERN_API(void)
    InvokeRTPPBCallbackUPP         (OSType                  inSelector,
                                    void *                  ioParams,
                                    void *                  inRefCon,
                                    RTPPBCallbackUPP        userUPP);

#else
    enum { uppRTPMPDataReleaseProcInfo = 0x000003C0 };              /* pascal no_return_value Func(4_bytes, 4_bytes) */
    enum { uppRTPPBCallbackProcInfo = 0x00000FC0 };                 /* pascal no_return_value Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewRTPMPDataReleaseUPP(userRoutine)                     (RTPMPDataReleaseUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppRTPMPDataReleaseProcInfo, GetCurrentArchitecture())
    #define NewRTPPBCallbackUPP(userRoutine)                        (RTPPBCallbackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppRTPPBCallbackProcInfo, GetCurrentArchitecture())
    #define DisposeRTPMPDataReleaseUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposeRTPPBCallbackUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define InvokeRTPMPDataReleaseUPP(inData, inRefCon, userUPP)    CALL_TWO_PARAMETER_UPP((userUPP), uppRTPMPDataReleaseProcInfo, (inData), (inRefCon))
    #define InvokeRTPPBCallbackUPP(inSelector, ioParams, inRefCon, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppRTPPBCallbackProcInfo, (inSelector), (ioParams), (inRefCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewRTPMPDataReleaseProc(userRoutine)                    NewRTPMPDataReleaseUPP(userRoutine)
#define NewRTPPBCallbackProc(userRoutine)                       NewRTPPBCallbackUPP(userRoutine)
#define CallRTPMPDataReleaseProc(userRoutine, inData, inRefCon) InvokeRTPMPDataReleaseUPP(inData, inRefCon, userRoutine)
#define CallRTPPBCallbackProc(userRoutine, inSelector, ioParams, inRefCon) InvokeRTPPBCallbackUPP(inSelector, ioParams, inRefCon, userRoutine)

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

#endif /* __QTSTREAMINGCOMPONENTS__ */

