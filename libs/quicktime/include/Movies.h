/*
     File:       Movies.h
 
     Contains:   QuickTime Interfaces.
 
     Version:    Technology: QuickTime 6.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MOVIES__
#define __MOVIES__

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __ALIASES__
#include <Aliases.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __IMAGECOMPRESSION__
#include <ImageCompression.h>
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


/*  "kFix1" is defined in FixMath as "fixed1"  */
/* error codes are in Errors.[haa] */
/* gestalt codes are in Gestalt.[hpa] */
enum {
    MovieFileType               = FOUR_CHAR_CODE('MooV'),
    MovieScrapType              = FOUR_CHAR_CODE('moov')
};

enum {
    MovieResourceType           = FOUR_CHAR_CODE('moov'),
    MovieForwardPointerResourceType = FOUR_CHAR_CODE('fore'),
    MovieBackwardPointerResourceType = FOUR_CHAR_CODE('back')
};

enum {
    MovieResourceAtomType       = FOUR_CHAR_CODE('moov'),
    MovieDataAtomType           = FOUR_CHAR_CODE('mdat'),
    FreeAtomType                = FOUR_CHAR_CODE('free'),
    SkipAtomType                = FOUR_CHAR_CODE('skip'),
    WideAtomPlaceholderType     = FOUR_CHAR_CODE('wide')
};

enum {
    MediaHandlerType            = FOUR_CHAR_CODE('mhlr'),
    DataHandlerType             = FOUR_CHAR_CODE('dhlr')
};

enum {
    VideoMediaType              = FOUR_CHAR_CODE('vide'),
    SoundMediaType              = FOUR_CHAR_CODE('soun'),
    TextMediaType               = FOUR_CHAR_CODE('text'),
    BaseMediaType               = FOUR_CHAR_CODE('gnrc'),
    MPEGMediaType               = FOUR_CHAR_CODE('MPEG'),
    MusicMediaType              = FOUR_CHAR_CODE('musi'),
    TimeCodeMediaType           = FOUR_CHAR_CODE('tmcd'),
    SpriteMediaType             = FOUR_CHAR_CODE('sprt'),
    FlashMediaType              = FOUR_CHAR_CODE('flsh'),
    MovieMediaType              = FOUR_CHAR_CODE('moov'),
    TweenMediaType              = FOUR_CHAR_CODE('twen'),
    ThreeDeeMediaType           = FOUR_CHAR_CODE('qd3d'),
    SkinMediaType               = FOUR_CHAR_CODE('skin'),
    HandleDataHandlerSubType    = FOUR_CHAR_CODE('hndl'),
    PointerDataHandlerSubType   = FOUR_CHAR_CODE('ptr '),
    NullDataHandlerSubType      = FOUR_CHAR_CODE('null'),
    ResourceDataHandlerSubType  = FOUR_CHAR_CODE('rsrc'),
    URLDataHandlerSubType       = FOUR_CHAR_CODE('url '),
    WiredActionHandlerType      = FOUR_CHAR_CODE('wire')
};

enum {
    VisualMediaCharacteristic   = FOUR_CHAR_CODE('eyes'),
    AudioMediaCharacteristic    = FOUR_CHAR_CODE('ears'),
    kCharacteristicCanSendVideo = FOUR_CHAR_CODE('vsnd'),
    kCharacteristicProvidesActions = FOUR_CHAR_CODE('actn'),
    kCharacteristicNonLinear    = FOUR_CHAR_CODE('nonl'),
    kCharacteristicCanStep      = FOUR_CHAR_CODE('step'),
    kCharacteristicHasNoDuration = FOUR_CHAR_CODE('noti'),
    kCharacteristicHasSkinData  = FOUR_CHAR_CODE('skin'),
    kCharacteristicProvidesKeyFocus = FOUR_CHAR_CODE('keyf')
};

enum {
    kUserDataMovieControllerType = FOUR_CHAR_CODE('ctyp'),
    kUserDataName               = FOUR_CHAR_CODE('name'),
    kUserDataTextAlbum          = FOUR_CHAR_CODE(' alb'),
    kUserDataTextArtist         = FOUR_CHAR_CODE(' ART'),
    kUserDataTextAuthor         = FOUR_CHAR_CODE(' aut'),
    kUserDataTextChapter        = FOUR_CHAR_CODE(' chp'),
    kUserDataTextComment        = FOUR_CHAR_CODE(' cmt'),
    kUserDataTextComposer       = FOUR_CHAR_CODE(' com'),
    kUserDataTextCopyright      = FOUR_CHAR_CODE(' cpy'),
    kUserDataTextCreationDate   = FOUR_CHAR_CODE(' day'),
    kUserDataTextDescription    = FOUR_CHAR_CODE(' des'),
    kUserDataTextDirector       = FOUR_CHAR_CODE(' dir'),
    kUserDataTextDisclaimer     = FOUR_CHAR_CODE(' dis'),
    kUserDataTextEncodedBy      = FOUR_CHAR_CODE(' enc'),
    kUserDataTextFullName       = FOUR_CHAR_CODE(' nam'),
    kUserDataTextGenre          = FOUR_CHAR_CODE(' gen'),
    kUserDataTextHostComputer   = FOUR_CHAR_CODE(' hst'),
    kUserDataTextInformation    = FOUR_CHAR_CODE(' inf'),
    kUserDataTextKeywords       = FOUR_CHAR_CODE(' key'),
    kUserDataTextMake           = FOUR_CHAR_CODE(' mak'),
    kUserDataTextModel          = FOUR_CHAR_CODE(' mod'),
    kUserDataTextOriginalArtist = FOUR_CHAR_CODE(' ope'),
    kUserDataTextOriginalFormat = FOUR_CHAR_CODE(' fmt'),
    kUserDataTextOriginalSource = FOUR_CHAR_CODE(' src'),
    kUserDataTextPerformers     = FOUR_CHAR_CODE(' prf'),
    kUserDataTextProducer       = FOUR_CHAR_CODE(' prd'),
    kUserDataTextProduct        = FOUR_CHAR_CODE(' PRD'),
    kUserDataTextSoftware       = FOUR_CHAR_CODE(' swr'),
    kUserDataTextSpecialPlaybackRequirements = FOUR_CHAR_CODE(' req'),
    kUserDataTextTrack          = FOUR_CHAR_CODE(' trk'),
    kUserDataTextWarning        = FOUR_CHAR_CODE(' wrn'),
    kUserDataTextWriter         = FOUR_CHAR_CODE(' wrt'),
    kUserDataTextURLLink        = FOUR_CHAR_CODE(' url'),
    kUserDataTextEditDate1      = FOUR_CHAR_CODE(' ed1')
};

enum {
    kUserDataUnicodeBit         = 1L << 7
};

enum {
    DoTheRightThing             = 0
};



struct MovieRecord {
    long                            data[1];
};
typedef struct MovieRecord              MovieRecord;

typedef MovieRecord *                   Movie;
typedef Movie *                         MoviePtr;

struct TrackRecord {
    long                            data[1];
};
typedef struct TrackRecord              TrackRecord;

typedef TrackRecord *                   Track;

struct MediaRecord {
    long                            data[1];
};
typedef struct MediaRecord              MediaRecord;

typedef MediaRecord *                   Media;

struct UserDataRecord {
    long                            data[1];
};
typedef struct UserDataRecord           UserDataRecord;

typedef UserDataRecord *                UserData;

struct TrackEditStateRecord {
    long                            data[1];
};
typedef struct TrackEditStateRecord     TrackEditStateRecord;

typedef TrackEditStateRecord *          TrackEditState;

struct MovieEditStateRecord {
    long                            data[1];
};
typedef struct MovieEditStateRecord     MovieEditStateRecord;

typedef MovieEditStateRecord *          MovieEditState;

struct QTRestrictionSetRecord {
    long                            data[1];
};
typedef struct QTRestrictionSetRecord   QTRestrictionSetRecord;

typedef QTRestrictionSetRecord *        QTRestrictionSet;

struct SpriteWorldRecord {
    long                            data[1];
};
typedef struct SpriteWorldRecord        SpriteWorldRecord;

typedef SpriteWorldRecord *             SpriteWorld;

struct SpriteRecord {
    long                            data[1];
};
typedef struct SpriteRecord             SpriteRecord;

typedef SpriteRecord *                  Sprite;

struct QTTweenerRecord {
    long                            data[1];
};
typedef struct QTTweenerRecord          QTTweenerRecord;

typedef QTTweenerRecord *               QTTweener;

struct SampleDescription {
    long                            descSize;
    long                            dataFormat;
    long                            resvd1;
    short                           resvd2;
    short                           dataRefIndex;
};
typedef struct SampleDescription        SampleDescription;
typedef SampleDescription *             SampleDescriptionPtr;
typedef SampleDescriptionPtr *          SampleDescriptionHandle;
typedef struct OpaqueQTBandwidthReference*  QTBandwidthReference;
typedef struct OpaqueQTScheduledBandwidthReference*  QTScheduledBandwidthReference;
enum {
    kQTNetworkStatusNoNetwork   = -2,
    kQTNetworkStatusUncertain   = -1,
    kQTNetworkStatusNotConnected = 0,
    kQTNetworkStatusConnected   = 1
};


typedef Handle                          QTAtomContainer;
typedef long                            QTAtom;
typedef long                            QTAtomType;
typedef long                            QTAtomID;
/* QTFloatDouble is the 64-bit IEEE-754 standard*/
typedef Float64                         QTFloatDouble;
/* QTFloatSingle is the 32-bit IEEE-754 standard*/
typedef Float32                         QTFloatSingle;



struct SoundDescription {
    long                            descSize;                   /* total size of SoundDescription including extra data */
    long                            dataFormat;                 /* sound format */
    long                            resvd1;                     /* reserved for apple use. set to zero */
    short                           resvd2;                     /* reserved for apple use. set to zero */
    short                           dataRefIndex;
    short                           version;                    /* which version is this data */
    short                           revlevel;                   /* what version of that codec did this */
    long                            vendor;                     /* whose  codec compressed this data */
    short                           numChannels;                /* number of channels of sound */
    short                           sampleSize;                 /* number of bits per sample */
    short                           compressionID;              /* unused. set to zero. */
    short                           packetSize;                 /* unused. set to zero. */
    UnsignedFixed                   sampleRate;                 /* sample rate sound is captured at */
};
typedef struct SoundDescription         SoundDescription;
typedef SoundDescription *              SoundDescriptionPtr;
typedef SoundDescriptionPtr *           SoundDescriptionHandle;
/* version 1 of the SoundDescription record*/

struct SoundDescriptionV1 {
                                                                /* original fields*/
    SoundDescription                desc;
                                                                /* fixed compression ratio information*/
    unsigned long                   samplesPerPacket;
    unsigned long                   bytesPerPacket;
    unsigned long                   bytesPerFrame;
    unsigned long                   bytesPerSample;
                                                                /* additional atom based fields ([long size, long type, some data], repeat)*/
};
typedef struct SoundDescriptionV1       SoundDescriptionV1;
typedef SoundDescriptionV1 *            SoundDescriptionV1Ptr;
typedef SoundDescriptionV1Ptr *         SoundDescriptionV1Handle;

struct TextDescription {
    long                            descSize;                   /* Total size of TextDescription*/
    long                            dataFormat;                 /* 'text'*/

    long                            resvd1;
    short                           resvd2;
    short                           dataRefIndex;

    long                            displayFlags;               /* see enum below for flag values*/

    long                            textJustification;          /* Can be: teCenter,teFlush -Default,-Right,-Left*/

    RGBColor                        bgColor;                    /* Background color*/

    Rect                            defaultTextBox;             /* Location to place the text within the track bounds*/
    ScrpSTElement                   defaultStyle;               /* Default style (struct defined in TextEdit.h)*/
    char                            defaultFontName[1];         /* Font Name (pascal string - struct extended to fit) */
};
typedef struct TextDescription          TextDescription;
typedef TextDescription *               TextDescriptionPtr;
typedef TextDescriptionPtr *            TextDescriptionHandle;

struct SpriteDescription {
    long                            descSize;                   /* total size of SpriteDescription including extra data */
    long                            dataFormat;                 /*  */
    long                            resvd1;                     /* reserved for apple use */
    short                           resvd2;
    short                           dataRefIndex;
    long                            version;                    /* which version is this data */
    OSType                          decompressorType;           /* which decompressor to use, 0 for no decompression */
    long                            sampleFlags;                /* how to interpret samples */
};
typedef struct SpriteDescription        SpriteDescription;
typedef SpriteDescription *             SpriteDescriptionPtr;
typedef SpriteDescriptionPtr *          SpriteDescriptionHandle;

struct FlashDescription {
    long                            descSize;
    long                            dataFormat;
    long                            resvd1;
    short                           resvd2;
    short                           dataRefIndex;
    long                            version;                    /* which version is this data */
    OSType                          decompressorType;           /* which decompressor to use, 0 for no decompression */
    long                            flags;
};
typedef struct FlashDescription         FlashDescription;
typedef FlashDescription *              FlashDescriptionPtr;
typedef FlashDescriptionPtr *           FlashDescriptionHandle;

struct ThreeDeeDescription {
    long                            descSize;                   /* total size of ThreeDeeDescription including extra data */
    long                            dataFormat;                 /*  */
    long                            resvd1;                     /* reserved for apple use */
    short                           resvd2;
    short                           dataRefIndex;
    long                            version;                    /* which version is this data */
    long                            rendererType;               /* which renderer to use, 0 for default */
    long                            decompressorType;           /* which decompressor to use, 0 for default */
};
typedef struct ThreeDeeDescription      ThreeDeeDescription;
typedef ThreeDeeDescription *           ThreeDeeDescriptionPtr;
typedef ThreeDeeDescriptionPtr *        ThreeDeeDescriptionHandle;

struct DataReferenceRecord {
    OSType                          dataRefType;
    Handle                          dataRef;
};
typedef struct DataReferenceRecord      DataReferenceRecord;

typedef DataReferenceRecord *           DataReferencePtr;
/*--------------------------
  Music Sample Description
--------------------------*/

struct MusicDescription {
    long                            descSize;
    long                            dataFormat;                 /* 'musi' */

    long                            resvd1;
    short                           resvd2;
    short                           dataRefIndex;

    long                            musicFlags;
    unsigned long                   headerData[1];              /* variable size! */

};
typedef struct MusicDescription         MusicDescription;
typedef MusicDescription *              MusicDescriptionPtr;
typedef MusicDescriptionPtr *           MusicDescriptionHandle;
enum {
    kMusicFlagDontPlay2Soft     = 1L << 0,
    kMusicFlagDontSlaveToMovie  = 1L << 1
};


enum {
    dfDontDisplay               = 1 << 0,                       /* Don't display the text*/
    dfDontAutoScale             = 1 << 1,                       /* Don't scale text as track bounds grows or shrinks*/
    dfClipToTextBox             = 1 << 2,                       /* Clip update to the textbox*/
    dfUseMovieBGColor           = 1 << 3,                       /* Set text background to movie's background color*/
    dfShrinkTextBoxToFit        = 1 << 4,                       /* Compute minimum box to fit the sample*/
    dfScrollIn                  = 1 << 5,                       /* Scroll text in until last of text is in view */
    dfScrollOut                 = 1 << 6,                       /* Scroll text out until last of text is gone (if both set, scroll in then out)*/
    dfHorizScroll               = 1 << 7,                       /* Scroll text horizontally (otherwise it's vertical)*/
    dfReverseScroll             = 1 << 8,                       /* vert: scroll down rather than up; horiz: scroll backwards (justfication dependent)*/
    dfContinuousScroll          = 1 << 9,                       /* new samples cause previous samples to scroll out */
    dfFlowHoriz                 = 1 << 10,                      /* horiz scroll text flows in textbox rather than extend to right */
    dfContinuousKaraoke         = 1 << 11,                      /* ignore begin offset, hilite everything up to the end offset(karaoke)*/
    dfDropShadow                = 1 << 12,                      /* display text with a drop shadow */
    dfAntiAlias                 = 1 << 13,                      /* attempt to display text anti aliased*/
    dfKeyedText                 = 1 << 14,                      /* key the text over background*/
    dfInverseHilite             = 1 << 15,                      /* Use inverse hiliting rather than using hilite color*/
    dfTextColorHilite           = 1 << 16                       /* changes text color in place of hiliting. */
};

enum {
    searchTextDontGoToFoundTime = 1L << 16,
    searchTextDontHiliteFoundText = 1L << 17,
    searchTextOneTrackOnly      = 1L << 18,
    searchTextEnabledTracksOnly = 1L << 19
};

/*use these with the text property routines*/
enum {
                                                                /* set property parameter / get property parameter*/
    kTextTextHandle             = 1,                            /* Handle / preallocated Handle*/
    kTextTextPtr                = 2,                            /* Pointer*/
    kTextTEStyle                = 3,                            /* TextStyle * / TextStyle **/
    kTextSelection              = 4,                            /* long [2] / long [2]*/
    kTextBackColor              = 5,                            /* RGBColor * / RGBColor **/
    kTextForeColor              = 6,                            /* RGBColor * / RGBColor **/
    kTextFace                   = 7,                            /* long / long **/
    kTextFont                   = 8,                            /* long / long **/
    kTextSize                   = 9,                            /* long / long **/
    kTextAlignment              = 10,                           /* short * / short **/
    kTextHilite                 = 11,                           /* hiliteRecord * / hiliteRecord **/
    kTextDropShadow             = 12,                           /* dropShadowRecord * / dropShadowRecord **/
    kTextDisplayFlags           = 13,                           /* long / long **/
    kTextScroll                 = 14,                           /* TimeValue * / TimeValue **/
    kTextRelativeScroll         = 15,                           /* Point **/
    kTextHyperTextFace          = 16,                           /* hyperTextSetFace * / hyperTextSetFace **/
    kTextHyperTextColor         = 17,                           /* hyperTextSetColor * / hyperTextSetColor **/
    kTextKeyEntry               = 18,                           /* short*/
    kTextMouseDown              = 19,                           /* Point **/
    kTextTextBox                = 20,                           /* Rect * / Rect **/
    kTextEditState              = 21,                           /* short / short **/
    kTextLength                 = 22                            /*       / long **/
};

enum {
    k3DMediaRendererEntry       = FOUR_CHAR_CODE('rend'),
    k3DMediaRendererName        = FOUR_CHAR_CODE('name'),
    k3DMediaRendererCode        = FOUR_CHAR_CODE('rcod')
};

/* progress messages */
enum {
    movieProgressOpen           = 0,
    movieProgressUpdatePercent  = 1,
    movieProgressClose          = 2
};

/* progress operations */
enum {
    progressOpFlatten           = 1,
    progressOpInsertTrackSegment = 2,
    progressOpInsertMovieSegment = 3,
    progressOpPaste             = 4,
    progressOpAddMovieSelection = 5,
    progressOpCopy              = 6,
    progressOpCut               = 7,
    progressOpLoadMovieIntoRam  = 8,
    progressOpLoadTrackIntoRam  = 9,
    progressOpLoadMediaIntoRam  = 10,
    progressOpImportMovie       = 11,
    progressOpExportMovie       = 12
};

enum {
    mediaQualityDraft           = 0x0000,
    mediaQualityNormal          = 0x0040,
    mediaQualityBetter          = 0x0080,
    mediaQualityBest            = 0x00C0
};

/*****
    Interactive Sprites Support
*****/
/* QTEventRecord flags*/
enum {
    kQTEventPayloadIsQTList     = 1L << 0
};


struct QTEventRecord {
    long                            version;
    OSType                          eventType;
    Point                           where;
    long                            flags;
    long                            payloadRefcon;              /* from here down only present if version >= 2*/
    long                            param1;
    long                            param2;
    long                            param3;
};
typedef struct QTEventRecord            QTEventRecord;
typedef QTEventRecord *                 QTEventRecordPtr;

struct QTAtomSpec {
    QTAtomContainer                 container;
    QTAtom                          atom;
};
typedef struct QTAtomSpec               QTAtomSpec;
typedef QTAtomSpec *                    QTAtomSpecPtr;

struct ResolvedQTEventSpec {
    QTAtomSpec                      actionAtom;
    Track                           targetTrack;
    long                            targetRefCon;
};
typedef struct ResolvedQTEventSpec      ResolvedQTEventSpec;
typedef ResolvedQTEventSpec *           ResolvedQTEventSpecPtr;

/* action constants */
enum {
    kActionMovieSetVolume       = 1024,                         /* (short movieVolume) */
    kActionMovieSetRate         = 1025,                         /* (Fixed rate) */
    kActionMovieSetLoopingFlags = 1026,                         /* (long loopingFlags) */
    kActionMovieGoToTime        = 1027,                         /* (TimeValue time) */
    kActionMovieGoToTimeByName  = 1028,                         /* (Str255 timeName) */
    kActionMovieGoToBeginning   = 1029,                         /* no params */
    kActionMovieGoToEnd         = 1030,                         /* no params */
    kActionMovieStepForward     = 1031,                         /* no params */
    kActionMovieStepBackward    = 1032,                         /* no params */
    kActionMovieSetSelection    = 1033,                         /* (TimeValue startTime, TimeValue endTime) */
    kActionMovieSetSelectionByName = 1034,                      /* (Str255 startTimeName, Str255 endTimeName) */
    kActionMoviePlaySelection   = 1035,                         /* (Boolean selectionOnly) */
    kActionMovieSetLanguage     = 1036,                         /* (long language) */
    kActionMovieChanged         = 1037,                         /* no params */
    kActionMovieRestartAtTime   = 1038,                         /* (TimeValue startTime, Fixed rate) */
    kActionMovieGotoNextChapter = 1039,                         /* no params */
    kActionMovieGotoPreviousChapter = 1040,                     /* no params */
    kActionMovieGotoFirstChapter = 1041,                        /* no params */
    kActionMovieGotoLastChapter = 1042,                         /* no params */
    kActionMovieGotoChapterByIndex = 1043,                      /* ( short index ) */
    kActionMovieSetScale        = 1044,                         /* (Fixed xScale, Fixed yScale) */
    kActionTrackSetVolume       = 2048,                         /* (short volume) */
    kActionTrackSetBalance      = 2049,                         /* (short balance) */
    kActionTrackSetEnabled      = 2050,                         /* (Boolean enabled) */
    kActionTrackSetMatrix       = 2051,                         /* (MatrixRecord matrix) */
    kActionTrackSetLayer        = 2052,                         /* (short layer) */
    kActionTrackSetClip         = 2053,                         /* (RgnHandle clip) */
    kActionTrackSetCursor       = 2054,                         /* (QTATomID cursorID) */
    kActionTrackSetGraphicsMode = 2055,                         /* (ModifierTrackGraphicsModeRecord graphicsMode) */
    kActionTrackSetIdleFrequency = 2056,                        /* (long frequency) */
    kActionTrackSetBassTreble   = 2057,                         /* (short base, short treble) */
    kActionSpriteSetMatrix      = 3072,                         /* (MatrixRecord matrix) */
    kActionSpriteSetImageIndex  = 3073,                         /* (short imageIndex) */
    kActionSpriteSetVisible     = 3074,                         /* (short visible) */
    kActionSpriteSetLayer       = 3075,                         /* (short layer) */
    kActionSpriteSetGraphicsMode = 3076,                        /* (ModifierTrackGraphicsModeRecord graphicsMode) */
    kActionSpritePassMouseToCodec = 3078,                       /* no params */
    kActionSpriteClickOnCodec   = 3079,                         /* Point localLoc */
    kActionSpriteTranslate      = 3080,                         /* (Fixed x, Fixed y, Boolean isAbsolute) */
    kActionSpriteScale          = 3081,                         /* (Fixed xScale, Fixed yScale) */
    kActionSpriteRotate         = 3082,                         /* (Fixed degrees) */
    kActionSpriteStretch        = 3083,                         /* (Fixed p1x, Fixed p1y, Fixed p2x, Fixed p2y, Fixed p3x, Fixed p3y, Fixed p4x, Fixed p4y) */
    kActionSpriteSetCanBeHitTested = 3094,                      /* (short canBeHitTested) */
    kActionQTVRSetPanAngle      = 4096,                         /* (float panAngle) */
    kActionQTVRSetTiltAngle     = 4097,                         /* (float tiltAngle) */
    kActionQTVRSetFieldOfView   = 4098,                         /* (float fieldOfView) */
    kActionQTVRShowDefaultView  = 4099,                         /* no params */
    kActionQTVRGoToNodeID       = 4100,                         /* (UInt32 nodeID) */
    kActionQTVREnableHotSpot    = 4101,                         /* long ID, Boolean enable */
    kActionQTVRShowHotSpots     = 4102,                         /* Boolean show */
    kActionQTVRTranslateObject  = 4103,                         /* float xMove, float yMove */
    kActionQTVRSetViewState     = 4109,                         /* long viewStateType, short state */
    kActionMusicPlayNote        = 5120,                         /* (long sampleDescIndex, long partNumber, long delay, long pitch, long velocity, long duration) */
    kActionMusicSetController   = 5121,                         /* (long sampleDescIndex, long partNumber, long delay, long controller, long value) */
    kActionCase                 = 6144,                         /* [(CaseStatementActionAtoms)] */
    kActionWhile                = 6145,                         /* [(WhileStatementActionAtoms)] */
    kActionGoToURL              = 6146,                         /* (C string urlLink) */
    kActionSendQTEventToSprite  = 6147,                         /* ([(SpriteTargetAtoms)], QTEventRecord theEvent) */
    kActionDebugStr             = 6148,                         /* (Str255 theString) */
    kActionPushCurrentTime      = 6149,                         /* no params */
    kActionPushCurrentTimeWithLabel = 6150,                     /* (Str255 theLabel) */
    kActionPopAndGotoTopTime    = 6151,                         /* no params */
    kActionPopAndGotoLabeledTime = 6152,                        /* (Str255 theLabel) */
    kActionStatusString         = 6153,                         /* (C string theString, long stringTypeFlags) */
    kActionSendQTEventToTrackObject = 6154,                     /* ([(TrackObjectTargetAtoms)], QTEventRecord theEvent) */
    kActionAddChannelSubscription = 6155,                       /* (Str255 channelName, C string channelsURL, C string channelsPictureURL) */
    kActionRemoveChannelSubscription = 6156,                    /* (C string channelsURL) */
    kActionOpenCustomActionHandler = 6157,                      /* (long handlerID, ComponentDescription handlerDesc) */
    kActionDoScript             = 6158,                         /* (long scriptTypeFlags, CString command, CString arguments) */
    kActionDoCompressedActions  = 6159,                         /* (compressed QTAtomContainer prefixed with eight bytes: long compressorType, long decompressedSize) */
    kActionSendAppMessage       = 6160,                         /* (long appMessageID) */
    kActionLoadComponent        = 6161,                         /* (ComponentDescription handlerDesc) */
    kActionSetFocus             = 6162,                         /* [(TargetAtoms theObject)] */
    kActionDontPassKeyEvent     = 6163,                         /* no params */
    kActionSetRandomSeed        = 6164,                         /* long randomSeed */
    kActionSpriteTrackSetVariable = 7168,                       /* (QTAtomID variableID, float value) */
    kActionSpriteTrackNewSprite = 7169,                         /* (QTAtomID spriteID, short imageIndex, MatrixRecord *matrix, short visible, short layer, ModifierTrackGraphicsModeRecord *graphicsMode, QTAtomID actionHandlingSpriteID) */
    kActionSpriteTrackDisposeSprite = 7170,                     /* (QTAtomID spriteID) */
    kActionSpriteTrackSetVariableToString = 7171,               /* (QTAtomID variableID, C string value) */
    kActionSpriteTrackConcatVariables = 7172,                   /* (QTAtomID firstVariableID, QTAtomID secondVariableID, QTAtomID resultVariableID ) */
    kActionSpriteTrackSetVariableToMovieURL = 7173,             /* (QTAtomID variableID, < optional: [(MovieTargetAtoms)] > ) */
    kActionSpriteTrackSetVariableToMovieBaseURL = 7174,         /* (QTAtomID variableID, < optional: [(MovieTargetAtoms)] > ) */
    kActionSpriteTrackSetAllSpritesHitTestingMode = 7181,
    kActionSpriteTrackNewImage  = 7182,                         /* (C string imageURL, QTAtomID desiredID) */
    kActionSpriteTrackDisposeImage = 7183,                      /* (short imageIndex) */
    kActionApplicationNumberAndString = 8192,                   /* (long aNumber, Str255 aString ) */
    kActionQD3DNamedObjectTranslateTo = 9216,                   /* (Fixed x, Fixed y, Fixed z ) */
    kActionQD3DNamedObjectScaleTo = 9217,                       /* (Fixed xScale, Fixed yScale, Fixed zScale ) */
    kActionQD3DNamedObjectRotateTo = 9218,                      /* (Fixed xDegrees, Fixed yDegrees, Fixed zDegrees ) */
    kActionFlashTrackSetPan     = 10240,                        /* (short xPercent, short yPercent ) */
    kActionFlashTrackSetZoom    = 10241,                        /* (short zoomFactor ) */
    kActionFlashTrackSetZoomRect = 10242,                       /* (long left, long top, long right, long bottom ) */
    kActionFlashTrackGotoFrameNumber = 10243,                   /* (long frameNumber ) */
    kActionFlashTrackGotoFrameLabel = 10244,                    /* (C string frameLabel ) */
    kActionFlashTrackSetFlashVariable = 10245,                  /* (C string path, C string name, C string value, Boolean updateFocus) */
    kActionFlashTrackDoButtonActions = 10246,                   /* (C string path, long buttonID, long transition) */
    kActionMovieTrackAddChildMovie = 11264,                     /* (QTAtomID childMovieID, C string childMovieURL) */
    kActionMovieTrackLoadChildMovie = 11265,                    /* (QTAtomID childMovieID) */
    kActionMovieTrackLoadChildMovieWithQTListParams = 11266,    /* (QTAtomID childMovieID, C string qtlistXML) */
    kActionTextTrackPasteText   = 12288,                        /* (C string theText, long startSelection, long endSelection ) */
    kActionTextTrackSetTextBox  = 12291,                        /* (short left, short top, short right, short bottom) */
    kActionTextTrackSetTextStyle = 12292,                       /* (Handle textStyle) */
    kActionTextTrackSetSelection = 12293,                       /* (long startSelection, long endSelection ) */
    kActionTextTrackSetBackgroundColor = 12294,                 /* (ModifierTrackGraphicsModeRecord backgroundColor ) */
    kActionTextTrackSetForegroundColor = 12295,                 /* (ModifierTrackGraphicsModeRecord foregroundColor ) */
    kActionTextTrackSetFace     = 12296,                        /* (long fontFace ) */
    kActionTextTrackSetFont     = 12297,                        /* (long fontID ) */
    kActionTextTrackSetSize     = 12298,                        /* (long fontSize ) */
    kActionTextTrackSetAlignment = 12299,                       /* (short alignment ) */
    kActionTextTrackSetHilite   = 12300,                        /* (long startHighlight, long endHighlight, ModifierTrackGraphicsModeRecord highlightColor ) */
    kActionTextTrackSetDropShadow = 12301,                      /* (Point dropShadow, short transparency ) */
    kActionTextTrackSetDisplayFlags = 12302,                    /* (long flags ) */
    kActionTextTrackSetScroll   = 12303,                        /* (long delay ) */
    kActionTextTrackRelativeScroll = 12304,                     /* (short deltaX, short deltaY ) */
    kActionTextTrackFindText    = 12305,                        /* (long flags, Str255 theText, ModifierTrackGraphicsModeRecord highlightColor ) */
    kActionTextTrackSetHyperTextFace = 12306,                   /* (short index, long fontFace ) */
    kActionTextTrackSetHyperTextColor = 12307,                  /* (short index, ModifierTrackGraphicsModeRecord highlightColor ) */
    kActionTextTrackKeyEntry    = 12308,                        /* (short character ) */
    kActionTextTrackMouseDown   = 12309,                        /* no params */
    kActionTextTrackSetEditable = 12310,                        /* (short editState) */
    kActionListAddElement       = 13312,                        /* (C string parentPath, long atIndex, C string newElementName) */
    kActionListRemoveElements   = 13313,                        /* (C string parentPath, long startIndex, long endIndex) */
    kActionListSetElementValue  = 13314,                        /* (C string elementPath, C string valueString) */
    kActionListPasteFromXML     = 13315,                        /* (C string xml, C string targetParentPath, long startIndex) */
    kActionListSetMatchingFromXML = 13316,                      /* (C string xml, C string targetParentPath) */
    kActionListSetFromURL       = 13317,                        /* (C string url, C string targetParentPath ) */
    kActionListExchangeLists    = 13318,                        /* (C string url, C string parentPath) */
    kActionListServerQuery      = 13319                         /* (C string url, C string keyValuePairs, long flags, C string parentPath) */
};


enum {
    kOperandExpression          = 1,
    kOperandConstant            = 2,
    kOperandSubscribedToChannel = 3,                            /* C string channelsURL */
    kOperandUniqueCustomActionHandlerID = 4,
    kOperandCustomActionHandlerIDIsOpen = 5,                    /* long ID */
    kOperandConnectionSpeed     = 6,
    kOperandGMTDay              = 7,
    kOperandGMTMonth            = 8,
    kOperandGMTYear             = 9,
    kOperandGMTHours            = 10,
    kOperandGMTMinutes          = 11,
    kOperandGMTSeconds          = 12,
    kOperandLocalDay            = 13,
    kOperandLocalMonth          = 14,
    kOperandLocalYear           = 15,
    kOperandLocalHours          = 16,
    kOperandLocalMinutes        = 17,
    kOperandLocalSeconds        = 18,
    kOperandRegisteredForQuickTimePro = 19,
    kOperandPlatformRunningOn   = 20,
    kOperandQuickTimeVersion    = 21,
    kOperandComponentVersion    = 22,                           /* C string type, C string subType, C string manufacturer */
    kOperandOriginalHandlerRefcon = 23,
    kOperandTicks               = 24,
    kOperandMaxLoadedTimeInMovie = 25,
    kOperandEventParameter      = 26,                           /* short index */
    kOperandFreeMemory          = 27,
    kOperandNetworkStatus       = 28,
    kOperandQuickTimeVersionRegistered = 29,                    /* long version */
    kOperandSystemVersion       = 30,
    kOperandMovieVolume         = 1024,
    kOperandMovieRate           = 1025,
    kOperandMovieIsLooping      = 1026,
    kOperandMovieLoopIsPalindrome = 1027,
    kOperandMovieTime           = 1028,
    kOperandMovieDuration       = 1029,
    kOperandMovieTimeScale      = 1030,
    kOperandMovieWidth          = 1031,
    kOperandMovieHeight         = 1032,
    kOperandMovieLoadState      = 1033,
    kOperandMovieTrackCount     = 1034,
    kOperandMovieIsActive       = 1035,
    kOperandMovieName           = 1036,
    kOperandMovieID             = 1037,
    kOperandMovieChapterCount   = 1038,
    kOperandMovieChapterIndex   = 1039,
    kOperandMovieChapterName    = 1040,
    kOperandMovieChapterNameByIndex = 1041,                     /* ( short index ) */
    kOperandMovieChapterIndexByName = 1042,                     /* (c string name)  */
    kOperandMovieAnnotation     = 1043,                         /* (c string requested, long flags) */
    kOperandMovieConnectionFlags = 1044,
    kOperandMovieConnectionString = 1045,
    kOperandTrackVolume         = 2048,
    kOperandTrackBalance        = 2049,
    kOperandTrackEnabled        = 2050,
    kOperandTrackLayer          = 2051,
    kOperandTrackWidth          = 2052,
    kOperandTrackHeight         = 2053,
    kOperandTrackDuration       = 2054,
    kOperandTrackName           = 2055,
    kOperandTrackID             = 2056,
    kOperandTrackIdleFrequency  = 2057,
    kOperandTrackBass           = 2058,
    kOperandTrackTreble         = 2059,
    kOperandSpriteBoundsLeft    = 3072,
    kOperandSpriteBoundsTop     = 3073,
    kOperandSpriteBoundsRight   = 3074,
    kOperandSpriteBoundsBottom  = 3075,
    kOperandSpriteImageIndex    = 3076,
    kOperandSpriteVisible       = 3077,
    kOperandSpriteLayer         = 3078,
    kOperandSpriteTrackVariable = 3079,                         /* [QTAtomID variableID] */
    kOperandSpriteTrackNumSprites = 3080,
    kOperandSpriteTrackNumImages = 3081,
    kOperandSpriteID            = 3082,
    kOperandSpriteIndex         = 3083,
    kOperandSpriteFirstCornerX  = 3084,
    kOperandSpriteFirstCornerY  = 3085,
    kOperandSpriteSecondCornerX = 3086,
    kOperandSpriteSecondCornerY = 3087,
    kOperandSpriteThirdCornerX  = 3088,
    kOperandSpriteThirdCornerY  = 3089,
    kOperandSpriteFourthCornerX = 3090,
    kOperandSpriteFourthCornerY = 3091,
    kOperandSpriteImageRegistrationPointX = 3092,
    kOperandSpriteImageRegistrationPointY = 3093,
    kOperandSpriteTrackSpriteIDAtPoint = 3094,                  /* short x, short y */
    kOperandSpriteName          = 3095,
    kOperandSpriteCanBeHitTested = 3105,                        /* short */
    kOperandSpriteTrackAllSpritesHitTestingMode = 3106,
    kOperandSpriteTrackImageIDByIndex = 3107,                   /* short imageIndex */
    kOperandSpriteTrackImageIndexByID = 3108,                   /* QTAtomID */
    kOperandQTVRPanAngle        = 4096,
    kOperandQTVRTiltAngle       = 4097,
    kOperandQTVRFieldOfView     = 4098,
    kOperandQTVRNodeID          = 4099,
    kOperandQTVRHotSpotsVisible = 4100,
    kOperandQTVRViewCenterH     = 4101,
    kOperandQTVRViewCenterV     = 4102,
    kOperandQTVRViewStateCount  = 4103,
    kOperandQTVRViewState       = 4104,                         /* long viewStateType */
    kOperandMouseLocalHLoc      = 5120,                         /* [TargetAtoms aTrack] */
    kOperandMouseLocalVLoc      = 5121,                         /* [TargetAtoms aTrack] */
    kOperandKeyIsDown           = 5122,                         /* [short modKeys, char asciiValue] */
    kOperandRandom              = 5123,                         /* [short min, short max] */
    kOperandCanHaveFocus        = 5124,                         /* [(TargetAtoms theObject)] */
    kOperandHasFocus            = 5125,                         /* [(TargetAtoms theObject)] */
    kOperandTextTrackEditable   = 6144,
    kOperandTextTrackCopyText   = 6145,                         /* long startSelection, long endSelection */
    kOperandTextTrackStartSelection = 6146,
    kOperandTextTrackEndSelection = 6147,
    kOperandTextTrackTextBoxLeft = 6148,
    kOperandTextTrackTextBoxTop = 6149,
    kOperandTextTrackTextBoxRight = 6150,
    kOperandTextTrackTextBoxBottom = 6151,
    kOperandTextTrackTextLength = 6152,
    kOperandListCountElements   = 7168,                         /* (C string parentPath) */
    kOperandListGetElementPathByIndex = 7169,                   /* (C string parentPath, long index) */
    kOperandListGetElementValue = 7170,                         /* (C string elementPath) */
    kOperandListCopyToXML       = 7171,                         /* (C string parentPath, long startIndex, long endIndex) */
    kOperandSin                 = 8192,                         /* float x    */
    kOperandCos                 = 8193,                         /* float x    */
    kOperandTan                 = 8194,                         /* float x    */
    kOperandATan                = 8195,                         /* float x    */
    kOperandATan2               = 8196,                         /* float y, float x   */
    kOperandDegreesToRadians    = 8197,                         /* float x */
    kOperandRadiansToDegrees    = 8198,                         /* float x */
    kOperandSquareRoot          = 8199,                         /* float x */
    kOperandExponent            = 8200,                         /* float x */
    kOperandLog                 = 8201,                         /* float x */
    kOperandFlashTrackVariable  = 9216,                         /* [CString path, CString name] */
    kOperandStringLength        = 10240,                        /* (C string text) */
    kOperandStringCompare       = 10241,                        /* (C string aText, C string bText, Boolean caseSensitive, Boolan diacSensitive) */
    kOperandStringSubString     = 10242,                        /* (C string text, long offset, long length) */
    kOperandStringConcat        = 10243                         /* (C string aText, C string bText) */
};

enum {
    kFirstMovieAction           = kActionMovieSetVolume,
    kLastMovieAction            = kActionMovieSetScale,
    kFirstTrackAction           = kActionTrackSetVolume,
    kLastTrackAction            = kActionTrackSetBassTreble,
    kFirstSpriteAction          = kActionSpriteSetMatrix,
    kLastSpriteAction           = kActionSpriteSetCanBeHitTested,
    kFirstQTVRAction            = kActionQTVRSetPanAngle,
    kLastQTVRAction             = kActionQTVRSetViewState,
    kFirstMusicAction           = kActionMusicPlayNote,
    kLastMusicAction            = kActionMusicSetController,
    kFirstSystemAction          = kActionCase,
    kLastSystemAction           = kActionSetRandomSeed,
    kFirstSpriteTrackAction     = kActionSpriteTrackSetVariable,
    kLastSpriteTrackAction      = kActionSpriteTrackDisposeImage,
    kFirstApplicationAction     = kActionApplicationNumberAndString,
    kLastApplicationAction      = kActionApplicationNumberAndString,
    kFirstQD3DNamedObjectAction = kActionQD3DNamedObjectTranslateTo,
    kLastQD3DNamedObjectAction  = kActionQD3DNamedObjectRotateTo,
    kFirstFlashTrackAction      = kActionFlashTrackSetPan,
    kLastFlashTrackAction       = kActionFlashTrackDoButtonActions,
    kFirstMovieTrackAction      = kActionMovieTrackAddChildMovie,
    kLastMovieTrackAction       = kActionMovieTrackLoadChildMovieWithQTListParams,
    kFirstTextTrackAction       = kActionTextTrackPasteText,
    kLastTextTrackAction        = kActionTextTrackSetEditable,
    kFirstMultiTargetAction     = kActionListAddElement,
    kLastMultiTargetAction      = kActionListServerQuery,
    kFirstAction                = kFirstMovieAction,
    kLastAction                 = kLastMultiTargetAction
};

/* target atom types*/
enum {
    kTargetMovie                = FOUR_CHAR_CODE('moov'),       /* no data */
    kTargetMovieName            = FOUR_CHAR_CODE('mona'),       /* (PString movieName) */
    kTargetMovieID              = FOUR_CHAR_CODE('moid'),       /* (long movieID) */
    kTargetRootMovie            = FOUR_CHAR_CODE('moro'),       /* no data */
    kTargetParentMovie          = FOUR_CHAR_CODE('mopa'),       /* no data */
    kTargetChildMovieTrackName  = FOUR_CHAR_CODE('motn'),       /* (PString childMovieTrackName) */
    kTargetChildMovieTrackID    = FOUR_CHAR_CODE('moti'),       /* (long childMovieTrackID) */
    kTargetChildMovieTrackIndex = FOUR_CHAR_CODE('motx'),       /* (long childMovieTrackIndex) */
    kTargetChildMovieMovieName  = FOUR_CHAR_CODE('momn'),       /* (PString childMovieName) */
    kTargetChildMovieMovieID    = FOUR_CHAR_CODE('momi'),       /* (long childMovieID) */
    kTargetTrackName            = FOUR_CHAR_CODE('trna'),       /* (PString trackName) */
    kTargetTrackID              = FOUR_CHAR_CODE('trid'),       /* (long trackID) */
    kTargetTrackType            = FOUR_CHAR_CODE('trty'),       /* (OSType trackType) */
    kTargetTrackIndex           = FOUR_CHAR_CODE('trin'),       /* (long trackIndex) */
    kTargetSpriteName           = FOUR_CHAR_CODE('spna'),       /* (PString spriteName) */
    kTargetSpriteID             = FOUR_CHAR_CODE('spid'),       /* (QTAtomID spriteID) */
    kTargetSpriteIndex          = FOUR_CHAR_CODE('spin'),       /* (short spriteIndex) */
    kTargetQD3DNamedObjectName  = FOUR_CHAR_CODE('nana'),       /* (CString objectName) */
    kTargetCurrentQTEventParams = FOUR_CHAR_CODE('evpa')        /* no data */
};

/* action container atom types*/
enum {
    kQTEventType                = FOUR_CHAR_CODE('evnt'),
    kAction                     = FOUR_CHAR_CODE('actn'),
    kWhichAction                = FOUR_CHAR_CODE('whic'),
    kActionParameter            = FOUR_CHAR_CODE('parm'),
    kActionTarget               = FOUR_CHAR_CODE('targ'),
    kActionFlags                = FOUR_CHAR_CODE('flag'),
    kActionParameterMinValue    = FOUR_CHAR_CODE('minv'),
    kActionParameterMaxValue    = FOUR_CHAR_CODE('maxv'),
    kActionListAtomType         = FOUR_CHAR_CODE('list'),
    kExpressionContainerAtomType = FOUR_CHAR_CODE('expr'),
    kConditionalAtomType        = FOUR_CHAR_CODE('test'),
    kOperatorAtomType           = FOUR_CHAR_CODE('oper'),
    kOperandAtomType            = FOUR_CHAR_CODE('oprn'),
    kCommentAtomType            = FOUR_CHAR_CODE('why '),
    kCustomActionHandler        = FOUR_CHAR_CODE('cust'),
    kCustomHandlerID            = FOUR_CHAR_CODE('id  '),
    kCustomHandlerDesc          = FOUR_CHAR_CODE('desc'),
    kQTEventRecordAtomType      = FOUR_CHAR_CODE('erec')
};

/* QTEvent types */
enum {
    kQTEventMouseClick          = FOUR_CHAR_CODE('clik'),
    kQTEventMouseClickEnd       = FOUR_CHAR_CODE('cend'),
    kQTEventMouseClickEndTriggerButton = FOUR_CHAR_CODE('trig'),
    kQTEventMouseEnter          = FOUR_CHAR_CODE('entr'),
    kQTEventMouseExit           = FOUR_CHAR_CODE('exit'),
    kQTEventMouseMoved          = FOUR_CHAR_CODE('move'),
    kQTEventFrameLoaded         = FOUR_CHAR_CODE('fram'),
    kQTEventIdle                = FOUR_CHAR_CODE('idle'),
    kQTEventKey                 = FOUR_CHAR_CODE('key '),       /* qtevent.param1 = key, qtevent.param2 = modifiers, qtEvent.param3 = scanCode */
    kQTEventMovieLoaded         = FOUR_CHAR_CODE('load'),
    kQTEventRequestToModifyMovie = FOUR_CHAR_CODE('reqm'),
    kQTEventListReceived        = FOUR_CHAR_CODE('list'),
    kQTEventKeyUp               = FOUR_CHAR_CODE('keyU')        /* qtevent.param1 = key, qtevent.param2 = modifiers, qtEvent.param3 = scanCode */
};

/* flags for the kActionFlags atom */
enum {
    kActionFlagActionIsDelta    = 1L << 1,
    kActionFlagParameterWrapsAround = 1L << 2,
    kActionFlagActionIsToggle   = 1L << 3
};

/* flags for stringTypeFlags field of the QTStatusStringRecord */
enum {
    kStatusStringIsURLLink      = 1L << 1,
    kStatusStringIsStreamingStatus = 1L << 2,
    kStatusHasCodeNumber        = 1L << 3,                      /* high 16 bits of stringTypeFlags is error code number*/
    kStatusIsError              = 1L << 4
};

/* flags for scriptTypeFlags field of the QTDoScriptRecord*/
enum {
    kScriptIsUnknownType        = 1L << 0,
    kScriptIsJavaScript         = 1L << 1,
    kScriptIsLingoEvent         = 1L << 2,
    kScriptIsVBEvent            = 1L << 3,
    kScriptIsProjectorCommand   = 1L << 4,
    kScriptIsAppleScript        = 1L << 5
};

/* flags for CheckQuickTimeRegistration routine*/
enum {
    kQTRegistrationDialogTimeOutFlag = 1 << 0,
    kQTRegistrationDialogShowDialog = 1 << 1,
    kQTRegistrationDialogForceDialog = 1 << 2
};

/* constants for kOperatorAtomType IDs (operator types)*/
enum {
    kOperatorAdd                = FOUR_CHAR_CODE('add '),
    kOperatorSubtract           = FOUR_CHAR_CODE('sub '),
    kOperatorMultiply           = FOUR_CHAR_CODE('mult'),
    kOperatorDivide             = FOUR_CHAR_CODE('div '),
    kOperatorOr                 = FOUR_CHAR_CODE('or  '),
    kOperatorAnd                = FOUR_CHAR_CODE('and '),
    kOperatorNot                = FOUR_CHAR_CODE('not '),
    kOperatorLessThan           = FOUR_CHAR_CODE('<   '),
    kOperatorLessThanEqualTo    = FOUR_CHAR_CODE('<=  '),
    kOperatorEqualTo            = FOUR_CHAR_CODE('=   '),
    kOperatorNotEqualTo         = FOUR_CHAR_CODE('!=  '),
    kOperatorGreaterThan        = FOUR_CHAR_CODE('>   '),
    kOperatorGreaterThanEqualTo = FOUR_CHAR_CODE('>=  '),
    kOperatorModulo             = FOUR_CHAR_CODE('mod '),
    kOperatorIntegerDivide      = FOUR_CHAR_CODE('idiv'),
    kOperatorAbsoluteValue      = FOUR_CHAR_CODE('abs '),
    kOperatorNegate             = FOUR_CHAR_CODE('neg ')
};

/* constants for kOperandPlatformRunningOn*/
enum {
    kPlatformMacintosh          = 1,
    kPlatformWindows            = 2
};

/* flags for kOperandSystemVersion*/
enum {
    kSystemIsWindows9x          = 0x00010000,
    kSystemIsWindowsNT          = 0x00020000
};

/* constants for MediaPropertiesAtom*/
enum {
    kMediaPropertyNonLinearAtomType = FOUR_CHAR_CODE('nonl'),
    kMediaPropertyHasActions    = 105
};


/* TimeBase equates */

typedef long                            TimeValue;
typedef long                            TimeScale;
typedef wide                            CompTimeValue;
typedef SInt64                          TimeValue64;
enum {
    loopTimeBase                = 1,
    palindromeLoopTimeBase      = 2,
    maintainTimeBaseZero        = 4
};

typedef unsigned long                   TimeBaseFlags;

struct TimeBaseRecord {
    long                            data[1];
};
typedef struct TimeBaseRecord           TimeBaseRecord;

typedef TimeBaseRecord *                TimeBase;

struct CallBackRecord {
    long                            data[1];
};
typedef struct CallBackRecord           CallBackRecord;

typedef CallBackRecord *                QTCallBack;

struct TimeRecord {
    CompTimeValue                   value;                      /* units */
    TimeScale                       scale;                      /* units per second */
    TimeBase                        base;
};
typedef struct TimeRecord               TimeRecord;
/* CallBack equates */
enum {
    triggerTimeFwd              = 0x0001,                       /* when curTime exceeds triggerTime going forward */
    triggerTimeBwd              = 0x0002,                       /* when curTime exceeds triggerTime going backwards */
    triggerTimeEither           = 0x0003,                       /* when curTime exceeds triggerTime going either direction */
    triggerRateLT               = 0x0004,                       /* when rate changes to less than trigger value */
    triggerRateGT               = 0x0008,                       /* when rate changes to greater than trigger value */
    triggerRateEqual            = 0x0010,                       /* when rate changes to equal trigger value */
    triggerRateLTE              = triggerRateLT | triggerRateEqual,
    triggerRateGTE              = triggerRateGT | triggerRateEqual,
    triggerRateNotEqual         = triggerRateGT | triggerRateEqual | triggerRateLT,
    triggerRateChange           = 0,
    triggerAtStart              = 0x0001,
    triggerAtStop               = 0x0002
};


typedef unsigned short                  QTCallBackFlags;
enum {
    timeBaseBeforeStartTime     = 1,
    timeBaseAfterStopTime       = 2
};

typedef unsigned long                   TimeBaseStatus;
enum {
    callBackAtTime              = 1,
    callBackAtRate              = 2,
    callBackAtTimeJump          = 3,
    callBackAtExtremes          = 4,
    callBackAtTimeBaseDisposed  = 5,
    callBackAtInterrupt         = 0x8000,
    callBackAtDeferredTask      = 0x4000
};

typedef unsigned short                  QTCallBackType;
typedef CALLBACK_API( void , QTCallBackProcPtr )(QTCallBack cb, long refCon);
typedef STACK_UPP_TYPE(QTCallBackProcPtr)                       QTCallBackUPP;
enum {
    qtcbNeedsRateChanges        = 1,                            /* wants to know about rate changes */
    qtcbNeedsTimeChanges        = 2,                            /* wants to know about time changes */
    qtcbNeedsStartStopChanges   = 4                             /* wants to know when TimeBase start/stop is changed*/
};


struct QTCallBackHeader {
    long                            callBackFlags;
    long                            reserved1;
    SInt8                           qtPrivate[40];
};
typedef struct QTCallBackHeader         QTCallBackHeader;
typedef CALLBACK_API( void , QTSyncTaskProcPtr )(void *task);
typedef STACK_UPP_TYPE(QTSyncTaskProcPtr)                       QTSyncTaskUPP;

struct QTSyncTaskRecord {
    void *                          qLink;
    QTSyncTaskUPP                   proc;
};
typedef struct QTSyncTaskRecord         QTSyncTaskRecord;

typedef QTSyncTaskRecord *              QTSyncTaskPtr;

typedef CALLBACK_API( OSErr , MovieRgnCoverProcPtr )(Movie theMovie, RgnHandle changedRgn, long refcon);
typedef CALLBACK_API( OSErr , MovieProgressProcPtr )(Movie theMovie, short message, short whatOperation, Fixed percentDone, long refcon);
typedef CALLBACK_API( OSErr , MovieDrawingCompleteProcPtr )(Movie theMovie, long refCon);
typedef CALLBACK_API( OSErr , TrackTransferProcPtr )(Track t, long refCon);
typedef CALLBACK_API( OSErr , GetMovieProcPtr )(long offset, long size, void *dataPtr, void *refCon);
typedef CALLBACK_API( Boolean , MoviePreviewCallOutProcPtr )(long refcon);
typedef CALLBACK_API( OSErr , TextMediaProcPtr )(Handle theText, Movie theMovie, short *displayFlag, long refcon);
typedef CALLBACK_API( OSErr , ActionsProcPtr )(void *refcon, Track targetTrack, long targetRefCon, QTEventRecordPtr theEvent);
typedef CALLBACK_API( OSErr , DoMCActionProcPtr )(void *refcon, short action, void *params, Boolean *handled);
typedef CALLBACK_API( OSErr , MovieExecuteWiredActionsProcPtr )(Movie theMovie, void *refcon, long flags, QTAtomContainer wiredActions);
typedef CALLBACK_API( void , MoviePrePrerollCompleteProcPtr )(Movie theMovie, OSErr prerollErr, void *refcon);
typedef CALLBACK_API( void , QTNextTaskNeededSoonerCallbackProcPtr )(TimeValue duration, unsigned long flags, void *refcon);
typedef CALLBACK_API( void , MoviesErrorProcPtr )(OSErr theErr, long refcon);
typedef STACK_UPP_TYPE(MovieRgnCoverProcPtr)                    MovieRgnCoverUPP;
typedef STACK_UPP_TYPE(MovieProgressProcPtr)                    MovieProgressUPP;
typedef STACK_UPP_TYPE(MovieDrawingCompleteProcPtr)             MovieDrawingCompleteUPP;
typedef STACK_UPP_TYPE(TrackTransferProcPtr)                    TrackTransferUPP;
typedef STACK_UPP_TYPE(GetMovieProcPtr)                         GetMovieUPP;
typedef STACK_UPP_TYPE(MoviePreviewCallOutProcPtr)              MoviePreviewCallOutUPP;
typedef STACK_UPP_TYPE(TextMediaProcPtr)                        TextMediaUPP;
typedef STACK_UPP_TYPE(ActionsProcPtr)                          ActionsUPP;
typedef STACK_UPP_TYPE(DoMCActionProcPtr)                       DoMCActionUPP;
typedef STACK_UPP_TYPE(MovieExecuteWiredActionsProcPtr)         MovieExecuteWiredActionsUPP;
typedef STACK_UPP_TYPE(MoviePrePrerollCompleteProcPtr)          MoviePrePrerollCompleteUPP;
typedef STACK_UPP_TYPE(QTNextTaskNeededSoonerCallbackProcPtr)   QTNextTaskNeededSoonerCallbackUPP;
typedef STACK_UPP_TYPE(MoviesErrorProcPtr)                      MoviesErrorUPP;
typedef ComponentInstance               MediaHandler;
typedef ComponentInstance               DataHandler;
typedef Component                       MediaHandlerComponent;
typedef Component                       DataHandlerComponent;
typedef ComponentResult                 HandlerError;
enum {
    keepInRam                   = 1 << 0,                       /* load and make non-purgable*/
    unkeepInRam                 = 1 << 1,                       /* mark as purgable*/
    flushFromRam                = 1 << 2,                       /* empty those handles*/
    loadForwardTrackEdits       = 1 << 3,                       /*    load track edits into ram for playing forward*/
    loadBackwardTrackEdits      = 1 << 4                        /*    load track edits into ram for playing in reverse*/
};

enum {
    newMovieActive              = 1 << 0,
    newMovieDontResolveDataRefs = 1 << 1,
    newMovieDontAskUnresolvedDataRefs = 1 << 2,
    newMovieDontAutoAlternates  = 1 << 3,
    newMovieDontUpdateForeBackPointers = 1 << 4,
    newMovieDontAutoUpdateClock = 1 << 5,
    newMovieAsyncOK             = 1 << 8,
    newMovieIdleImportOK        = 1 << 10,
    newMovieDontInteractWithUser = 1 << 11
};

/* track usage bits */
enum {
    trackUsageInMovie           = 1 << 1,
    trackUsageInPreview         = 1 << 2,
    trackUsageInPoster          = 1 << 3
};

/* Add/GetMediaSample flags */
enum {
    mediaSampleNotSync          = 1 << 0,                       /* sample is not a sync sample (eg. is frame differenced */
    mediaSampleShadowSync       = 1 << 1                        /* sample is a shadow sync */
};

enum {
    pasteInParallel             = 1 << 0,
    showUserSettingsDialog      = 1 << 1,
    movieToFileOnlyExport       = 1 << 2,
    movieFileSpecValid          = 1 << 3
};

enum {
    nextTimeMediaSample         = 1 << 0,
    nextTimeMediaEdit           = 1 << 1,
    nextTimeTrackEdit           = 1 << 2,
    nextTimeSyncSample          = 1 << 3,
    nextTimeStep                = 1 << 4,
    nextTimeEdgeOK              = 1 << 14,
    nextTimeIgnoreActiveSegment = 1 << 15
};

typedef unsigned short                  nextTimeFlagsEnum;
enum {
    createMovieFileDeleteCurFile = 1L << 31,
    createMovieFileDontCreateMovie = 1L << 30,
    createMovieFileDontOpenFile = 1L << 29,
    createMovieFileDontCreateResFile = 1L << 28
};

typedef unsigned long                   createMovieFileFlagsEnum;
enum {
    flattenAddMovieToDataFork   = 1L << 0,
    flattenActiveTracksOnly     = 1L << 2,
    flattenDontInterleaveFlatten = 1L << 3,
    flattenFSSpecPtrIsDataRefRecordPtr = 1L << 4,
    flattenCompressMovieResource = 1L << 5,
    flattenForceMovieResourceBeforeMovieData = 1L << 6
};

typedef unsigned long                   movieFlattenFlagsEnum;
enum {
    movieInDataForkResID        = -1                            /* magic res ID */
};

enum {
    mcTopLeftMovie              = 1 << 0,                       /* usually centered */
    mcScaleMovieToFit           = 1 << 1,                       /* usually only scales down */
    mcWithBadge                 = 1 << 2,                       /* give me a badge */
    mcNotVisible                = 1 << 3,                       /* don't show controller */
    mcWithFrame                 = 1 << 4                        /* gimme a frame */
};

enum {
    movieScrapDontZeroScrap     = 1 << 0,
    movieScrapOnlyPutMovie      = 1 << 1
};

enum {
    dataRefSelfReference        = 1 << 0,
    dataRefWasNotResolved       = 1 << 1
};

typedef unsigned long                   dataRefAttributesFlags;
enum {
    kMovieAnchorDataRefIsDefault = 1 << 0                       /* data ref returned is movie default data ref */
};

enum {
    hintsScrubMode              = 1 << 0,                       /* mask == && (if flags == scrub on, flags != scrub off) */
    hintsLoop                   = 1 << 1,
    hintsDontPurge              = 1 << 2,
    hintsUseScreenBuffer        = 1 << 5,
    hintsAllowInterlace         = 1 << 6,
    hintsUseSoundInterp         = 1 << 7,
    hintsHighQuality            = 1 << 8,                       /* slooooow */
    hintsPalindrome             = 1 << 9,
    hintsInactive               = 1 << 11,
    hintsOffscreen              = 1 << 12,
    hintsDontDraw               = 1 << 13,
    hintsAllowBlacklining       = 1 << 14,
    hintsDontUseVideoOverlaySurface = 1 << 16,
    hintsIgnoreBandwidthRestrictions = 1 << 17,
    hintsPlayingEveryFrame      = 1 << 18,
    hintsAllowDynamicResize     = 1 << 19,
    hintsSingleField            = 1 << 20,
    hintsNoRenderingTimeOut     = 1 << 21,
    hintsFlushVideoInsteadOfDirtying = 1 << 22,
    hintsEnableSubPixelPositioning = 1L << 23
};

typedef unsigned long                   playHintsEnum;
enum {
    mediaHandlerFlagBaseClient  = 1
};

typedef unsigned long                   mediaHandlerFlagsEnum;
enum {
    movieTrackMediaType         = 1 << 0,
    movieTrackCharacteristic    = 1 << 1,
    movieTrackEnabledOnly       = 1 << 2
};


struct SampleReferenceRecord {
    long                            dataOffset;
    long                            dataSize;
    TimeValue                       durationPerSample;
    long                            numberOfSamples;
    short                           sampleFlags;
};
typedef struct SampleReferenceRecord    SampleReferenceRecord;

typedef SampleReferenceRecord *         SampleReferencePtr;

struct SampleReference64Record {
    wide                            dataOffset;
    unsigned long                   dataSize;
    TimeValue                       durationPerSample;
    unsigned long                   numberOfSamples;
    short                           sampleFlags;
};
typedef struct SampleReference64Record  SampleReference64Record;

typedef SampleReference64Record *       SampleReference64Ptr;

/*************************
* Initialization Routines 
**************************/
EXTERN_API( void )
CheckQuickTimeRegistration      (void *                 registrationKey,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x02DA, 0xAAAA);

EXTERN_API( OSErr )
EnterMovies                     (void)                                                      TWOWORDINLINE(0x7001, 0xAAAA);

EXTERN_API( void )
ExitMovies                      (void)                                                      TWOWORDINLINE(0x7002, 0xAAAA);

/*************************
* Error Routines 
**************************/

EXTERN_API( OSErr )
GetMoviesError                  (void)                                                      TWOWORDINLINE(0x7003, 0xAAAA);

EXTERN_API( void )
ClearMoviesStickyError          (void)                                                      THREEWORDINLINE(0x303C, 0x00DE, 0xAAAA);

EXTERN_API( OSErr )
GetMoviesStickyError            (void)                                                      TWOWORDINLINE(0x7004, 0xAAAA);

EXTERN_API( void )
SetMoviesErrorProc              (MoviesErrorUPP         errProc,
                                 long                   refcon)                             THREEWORDINLINE(0x303C, 0x00EF, 0xAAAA);


/*************************
* Idle Routines 
**************************/
EXTERN_API( void )
MoviesTask                      (Movie                  theMovie,
                                 long                   maxMilliSecToUse)                   TWOWORDINLINE(0x7005, 0xAAAA);

EXTERN_API( OSErr )
PrerollMovie                    (Movie                  theMovie,
                                 TimeValue              time,
                                 Fixed                  Rate)                               TWOWORDINLINE(0x7006, 0xAAAA);

EXTERN_API( OSErr )
PrePrerollMovie                 (Movie                  m,
                                 TimeValue              time,
                                 Fixed                  rate,
                                 MoviePrePrerollCompleteUPP  proc,
                                 void *                 refcon)                             THREEWORDINLINE(0x303C, 0x02F7, 0xAAAA);

EXTERN_API( void )
AbortPrePrerollMovie            (Movie                  m,
                                 OSErr                  err)                                THREEWORDINLINE(0x303C, 0x02F8, 0xAAAA);

EXTERN_API( OSErr )
LoadMovieIntoRam                (Movie                  theMovie,
                                 TimeValue              time,
                                 TimeValue              duration,
                                 long                   flags)                              TWOWORDINLINE(0x7007, 0xAAAA);

EXTERN_API( OSErr )
LoadTrackIntoRam                (Track                  theTrack,
                                 TimeValue              time,
                                 TimeValue              duration,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x016E, 0xAAAA);

EXTERN_API( OSErr )
LoadMediaIntoRam                (Media                  theMedia,
                                 TimeValue              time,
                                 TimeValue              duration,
                                 long                   flags)                              TWOWORDINLINE(0x7008, 0xAAAA);

EXTERN_API( void )
SetMovieActive                  (Movie                  theMovie,
                                 Boolean                active)                             TWOWORDINLINE(0x7009, 0xAAAA);

EXTERN_API( Boolean )
GetMovieActive                  (Movie                  theMovie)                           TWOWORDINLINE(0x700A, 0xAAAA);

EXTERN_API( OSErr )
QTGetWallClockTimeBase          (TimeBase *             wallClockTimeBase)                  THREEWORDINLINE(0x303C, 0x0347, 0xAAAA);


/*************************
* Idle Management
**************************/
typedef struct OpaqueIdleManager*       IdleManager;
EXTERN_API( IdleManager )
QTIdleManagerOpen               (void)                                                      THREEWORDINLINE(0x303C, 0x0348, 0xAAAA);

EXTERN_API( OSErr )
QTIdleManagerClose              (IdleManager            im)                                 THREEWORDINLINE(0x303C, 0x0349, 0xAAAA);

EXTERN_API( OSErr )
QTIdleManagerGetNextIdleTime    (IdleManager            im,
                                 TimeRecord *           nextIdle)                           THREEWORDINLINE(0x303C, 0x034A, 0xAAAA);

EXTERN_API( OSErr )
QTIdleManagerSetNextIdleTime    (IdleManager            im,
                                 TimeRecord *           nextIdle)                           THREEWORDINLINE(0x303C, 0x034B, 0xAAAA);

EXTERN_API( OSErr )
QTIdleManagerSetNextIdleTimeNever (IdleManager          im)                                 THREEWORDINLINE(0x303C, 0x0373, 0xAAAA);

EXTERN_API( OSErr )
QTIdleManagerSetNextIdleTimeNow (IdleManager            im)                                 THREEWORDINLINE(0x303C, 0x0374, 0xAAAA);

EXTERN_API( OSErr )
QTIdleManagerSetNextIdleTimeDelta (IdleManager          im,
                                 TimeValue              duration,
                                 TimeScale              scale)                              THREEWORDINLINE(0x303C, 0x0375, 0xAAAA);

EXTERN_API( OSErr )
QTIdleManagerSetParent          (IdleManager            im,
                                 IdleManager            parent)                             THREEWORDINLINE(0x303C, 0x034C, 0xAAAA);

EXTERN_API( OSErr )
QTIdleManagerNeedsAnIdle        (IdleManager            im,
                                 Boolean *              needsOne)                           THREEWORDINLINE(0x303C, 0x0371, 0xAAAA);


/*************************
* Carbon Movie Control
**************************/
/* Movie Control option bits*/
enum {
    kMovieControlOptionHideController = (1L << 0),
    kMovieControlOptionLocateTopLeft = (1L << 1),
    kMovieControlOptionEnableEditing = (1L << 2),
    kMovieControlOptionHandleEditingHI = (1L << 3),
    kMovieControlOptionSetKeysEnabled = (1L << 4),
    kMovieControlOptionManuallyIdled = (1L << 5)
};

/* Item tags for use in GetControlData() (some with SetControlData()) calls on Movie Controls*/
enum {
    kMovieControlDataMovieController = FOUR_CHAR_CODE('mc  '),
    kMovieControlDataMovie      = FOUR_CHAR_CODE('moov'),
    kMovieControlDataManualIdling = FOUR_CHAR_CODE('manu')
};

/*
** CreateMovieControl() -   This is the public API routine that creates a Movie Control. Given a window and location
**                          plus a movie, it constructs a Movie Control with a Movie Controller in the window.
*/
EXTERN_API( OSErr )
CreateMovieControl              (WindowRef              theWindow,
                                 Rect *                 localRect,
                                 Movie                  theMovie,
                                 UInt32                 options,
                                 ControlRef *           returnedControl)                    THREEWORDINLINE(0x303C, 0x0378, 0xAAAA);


/*************************
* calls for playing movies, previews, posters
**************************/
EXTERN_API( void )
StartMovie                      (Movie                  theMovie)                           TWOWORDINLINE(0x700B, 0xAAAA);

EXTERN_API( void )
StopMovie                       (Movie                  theMovie)                           TWOWORDINLINE(0x700C, 0xAAAA);

EXTERN_API( void )
GoToBeginningOfMovie            (Movie                  theMovie)                           TWOWORDINLINE(0x700D, 0xAAAA);

EXTERN_API( void )
GoToEndOfMovie                  (Movie                  theMovie)                           TWOWORDINLINE(0x700E, 0xAAAA);

EXTERN_API( Boolean )
IsMovieDone                     (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x00DD, 0xAAAA);

EXTERN_API( Boolean )
GetMoviePreviewMode             (Movie                  theMovie)                           TWOWORDINLINE(0x700F, 0xAAAA);

EXTERN_API( void )
SetMoviePreviewMode             (Movie                  theMovie,
                                 Boolean                usePreview)                         TWOWORDINLINE(0x7010, 0xAAAA);

EXTERN_API( void )
ShowMoviePoster                 (Movie                  theMovie)                           TWOWORDINLINE(0x7011, 0xAAAA);

EXTERN_API( void )
PlayMoviePreview                (Movie                  theMovie,
                                 MoviePreviewCallOutUPP  callOutProc,
                                 long                   refcon)                             THREEWORDINLINE(0x303C, 0x00F2, 0xAAAA);

/*************************
* calls for controlling movies & tracks which are playing
**************************/
EXTERN_API( TimeBase )
GetMovieTimeBase                (Movie                  theMovie)                           TWOWORDINLINE(0x7012, 0xAAAA);

EXTERN_API( void )
SetMovieMasterTimeBase          (Movie                  theMovie,
                                 TimeBase               tb,
                                 const TimeRecord *     slaveZero)                          THREEWORDINLINE(0x303C, 0x0167, 0xAAAA);

EXTERN_API( void )
SetMovieMasterClock             (Movie                  theMovie,
                                 Component              clockMeister,
                                 const TimeRecord *     slaveZero)                          THREEWORDINLINE(0x303C, 0x0168, 0xAAAA);

EXTERN_API( void )
ChooseMovieClock                (Movie                  m,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x0269, 0xAAAA);

EXTERN_API( void )
GetMovieGWorld                  (Movie                  theMovie,
                                 CGrafPtr *             port,
                                 GDHandle *             gdh)                                TWOWORDINLINE(0x7015, 0xAAAA);

EXTERN_API( void )
SetMovieGWorld                  (Movie                  theMovie,
                                 CGrafPtr               port,
                                 GDHandle               gdh)                                TWOWORDINLINE(0x7016, 0xAAAA);

enum {
    movieDrawingCallWhenChanged = 0,
    movieDrawingCallAlways      = 1
};

EXTERN_API( void )
SetMovieDrawingCompleteProc     (Movie                  theMovie,
                                 long                   flags,
                                 MovieDrawingCompleteUPP  proc,
                                 long                   refCon)                             THREEWORDINLINE(0x303C, 0x01DE, 0xAAAA);


EXTERN_API( void )
GetMovieNaturalBoundsRect       (Movie                  theMovie,
                                 Rect *                 naturalBounds)                      THREEWORDINLINE(0x303C, 0x022C, 0xAAAA);

EXTERN_API( Track )
GetNextTrackForCompositing      (Movie                  theMovie,
                                 Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x01FA, 0xAAAA);

EXTERN_API( Track )
GetPrevTrackForCompositing      (Movie                  theMovie,
                                 Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x01FB, 0xAAAA);

EXTERN_API( void )
SetTrackGWorld                  (Track                  theTrack,
                                 CGrafPtr               port,
                                 GDHandle               gdh,
                                 TrackTransferUPP       proc,
                                 long                   refCon)                             THREEWORDINLINE(0x303C, 0x009D, 0xAAAA);

EXTERN_API( PicHandle )
GetMoviePict                    (Movie                  theMovie,
                                 TimeValue              time)                               TWOWORDINLINE(0x701D, 0xAAAA);

EXTERN_API( PicHandle )
GetTrackPict                    (Track                  theTrack,
                                 TimeValue              time)                               TWOWORDINLINE(0x701E, 0xAAAA);

EXTERN_API( PicHandle )
GetMoviePosterPict              (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x00F7, 0xAAAA);

/* called between Begin & EndUpdate */
EXTERN_API( OSErr )
UpdateMovie                     (Movie                  theMovie)                           TWOWORDINLINE(0x701F, 0xAAAA);

EXTERN_API( OSErr )
InvalidateMovieRegion           (Movie                  theMovie,
                                 RgnHandle              invalidRgn)                         THREEWORDINLINE(0x303C, 0x022A, 0xAAAA);

/**** spatial movie routines ****/
EXTERN_API( void )
GetMovieBox                     (Movie                  theMovie,
                                 Rect *                 boxRect)                            THREEWORDINLINE(0x303C, 0x00F9, 0xAAAA);

EXTERN_API( void )
SetMovieBox                     (Movie                  theMovie,
                                 const Rect *           boxRect)                            THREEWORDINLINE(0x303C, 0x00FA, 0xAAAA);

/** movie display clip */
EXTERN_API( RgnHandle )
GetMovieDisplayClipRgn          (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x00FC, 0xAAAA);

EXTERN_API( void )
SetMovieDisplayClipRgn          (Movie                  theMovie,
                                 RgnHandle              theClip)                            THREEWORDINLINE(0x303C, 0x00FD, 0xAAAA);

/** movie src clip */
EXTERN_API( RgnHandle )
GetMovieClipRgn                 (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x0100, 0xAAAA);

EXTERN_API( void )
SetMovieClipRgn                 (Movie                  theMovie,
                                 RgnHandle              theClip)                            THREEWORDINLINE(0x303C, 0x0101, 0xAAAA);

/** track src clip */
EXTERN_API( RgnHandle )
GetTrackClipRgn                 (Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x0102, 0xAAAA);

EXTERN_API( void )
SetTrackClipRgn                 (Track                  theTrack,
                                 RgnHandle              theClip)                            THREEWORDINLINE(0x303C, 0x0103, 0xAAAA);

/** bounds in display space (not clipped by display clip) */
EXTERN_API( RgnHandle )
GetMovieDisplayBoundsRgn        (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x00FB, 0xAAAA);

EXTERN_API( RgnHandle )
GetTrackDisplayBoundsRgn        (Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x0112, 0xAAAA);

/** bounds in movie space */
EXTERN_API( RgnHandle )
GetMovieBoundsRgn               (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x00FE, 0xAAAA);

EXTERN_API( RgnHandle )
GetTrackMovieBoundsRgn          (Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x00FF, 0xAAAA);

/** bounds in track space */
EXTERN_API( RgnHandle )
GetTrackBoundsRgn               (Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x0111, 0xAAAA);

/** mattes - always in track space */
EXTERN_API( PixMapHandle )
GetTrackMatte                   (Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x0115, 0xAAAA);

EXTERN_API( void )
SetTrackMatte                   (Track                  theTrack,
                                 PixMapHandle           theMatte)                           THREEWORDINLINE(0x303C, 0x0116, 0xAAAA);

EXTERN_API( void )
DisposeMatte                    (PixMapHandle           theMatte)                           THREEWORDINLINE(0x303C, 0x014A, 0xAAAA);

/** video out */
EXTERN_API( void )
SetMovieVideoOutput             (Movie                  theMovie,
                                 ComponentInstance      vout)                               THREEWORDINLINE(0x303C, 0x0340, 0xAAAA);

/*************************
* calls for getting/saving movies
**************************/
EXTERN_API( Movie )
NewMovie                        (long                   flags)                              THREEWORDINLINE(0x303C, 0x0187, 0xAAAA);

EXTERN_API( OSErr )
PutMovieIntoHandle              (Movie                  theMovie,
                                 Handle                 publicMovie)                        TWOWORDINLINE(0x7022, 0xAAAA);

EXTERN_API( OSErr )
PutMovieIntoDataFork            (Movie                  theMovie,
                                 short                  fRefNum,
                                 long                   offset,
                                 long                   maxSize)                            THREEWORDINLINE(0x303C, 0x01B4, 0xAAAA);

EXTERN_API( OSErr )
PutMovieIntoDataFork64          (Movie                  theMovie,
                                 long                   fRefNum,
                                 const wide *           offset,
                                 unsigned long          maxSize)                            THREEWORDINLINE(0x303C, 0x02EA, 0xAAAA);

EXTERN_API( OSErr )
PutMovieIntoStorage             (Movie                  theMovie,
                                 DataHandler            dh,
                                 const wide *           offset,
                                 unsigned long          maxSize)                            THREEWORDINLINE(0x303C, 0x0391, 0xAAAA);

EXTERN_API( OSErr )
PutMovieForDataRefIntoHandle    (Movie                  theMovie,
                                 Handle                 dataRef,
                                 OSType                 dataRefType,
                                 Handle                 publicMovie)                        THREEWORDINLINE(0x303C, 0x0380, 0xAAAA);

EXTERN_API( void )
DisposeMovie                    (Movie                  theMovie)                           TWOWORDINLINE(0x7023, 0xAAAA);

/*************************
* Movie State Routines
**************************/
EXTERN_API( unsigned long )
GetMovieCreationTime            (Movie                  theMovie)                           TWOWORDINLINE(0x7026, 0xAAAA);

EXTERN_API( unsigned long )
GetMovieModificationTime        (Movie                  theMovie)                           TWOWORDINLINE(0x7027, 0xAAAA);

EXTERN_API( TimeScale )
GetMovieTimeScale               (Movie                  theMovie)                           TWOWORDINLINE(0x7029, 0xAAAA);

EXTERN_API( void )
SetMovieTimeScale               (Movie                  theMovie,
                                 TimeScale              timeScale)                          TWOWORDINLINE(0x702A, 0xAAAA);

EXTERN_API( TimeValue )
GetMovieDuration                (Movie                  theMovie)                           TWOWORDINLINE(0x702B, 0xAAAA);

EXTERN_API( Fixed )
GetMovieRate                    (Movie                  theMovie)                           TWOWORDINLINE(0x702C, 0xAAAA);

EXTERN_API( void )
SetMovieRate                    (Movie                  theMovie,
                                 Fixed                  rate)                               TWOWORDINLINE(0x702D, 0xAAAA);

EXTERN_API( Fixed )
GetMoviePreferredRate           (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x00F3, 0xAAAA);

EXTERN_API( void )
SetMoviePreferredRate           (Movie                  theMovie,
                                 Fixed                  rate)                               THREEWORDINLINE(0x303C, 0x00F4, 0xAAAA);

EXTERN_API( short )
GetMoviePreferredVolume         (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x00F5, 0xAAAA);

EXTERN_API( void )
SetMoviePreferredVolume         (Movie                  theMovie,
                                 short                  volume)                             THREEWORDINLINE(0x303C, 0x00F6, 0xAAAA);

EXTERN_API( short )
GetMovieVolume                  (Movie                  theMovie)                           TWOWORDINLINE(0x702E, 0xAAAA);

EXTERN_API( void )
SetMovieVolume                  (Movie                  theMovie,
                                 short                  volume)                             TWOWORDINLINE(0x702F, 0xAAAA);

EXTERN_API( void )
GetMovieMatrix                  (Movie                  theMovie,
                                 MatrixRecord *         matrix)                             TWOWORDINLINE(0x7031, 0xAAAA);

EXTERN_API( void )
SetMovieMatrix                  (Movie                  theMovie,
                                 const MatrixRecord *   matrix)                             TWOWORDINLINE(0x7032, 0xAAAA);

EXTERN_API( void )
GetMoviePreviewTime             (Movie                  theMovie,
                                 TimeValue *            previewTime,
                                 TimeValue *            previewDuration)                    TWOWORDINLINE(0x7033, 0xAAAA);

EXTERN_API( void )
SetMoviePreviewTime             (Movie                  theMovie,
                                 TimeValue              previewTime,
                                 TimeValue              previewDuration)                    TWOWORDINLINE(0x7034, 0xAAAA);

EXTERN_API( TimeValue )
GetMoviePosterTime              (Movie                  theMovie)                           TWOWORDINLINE(0x7035, 0xAAAA);

EXTERN_API( void )
SetMoviePosterTime              (Movie                  theMovie,
                                 TimeValue              posterTime)                         TWOWORDINLINE(0x7036, 0xAAAA);

EXTERN_API( void )
GetMovieSelection               (Movie                  theMovie,
                                 TimeValue *            selectionTime,
                                 TimeValue *            selectionDuration)                  TWOWORDINLINE(0x7037, 0xAAAA);

EXTERN_API( void )
SetMovieSelection               (Movie                  theMovie,
                                 TimeValue              selectionTime,
                                 TimeValue              selectionDuration)                  TWOWORDINLINE(0x7038, 0xAAAA);

EXTERN_API( void )
SetMovieActiveSegment           (Movie                  theMovie,
                                 TimeValue              startTime,
                                 TimeValue              duration)                           THREEWORDINLINE(0x303C, 0x015C, 0xAAAA);

EXTERN_API( void )
GetMovieActiveSegment           (Movie                  theMovie,
                                 TimeValue *            startTime,
                                 TimeValue *            duration)                           THREEWORDINLINE(0x303C, 0x015D, 0xAAAA);

EXTERN_API( TimeValue )
GetMovieTime                    (Movie                  theMovie,
                                 TimeRecord *           currentTime)                        TWOWORDINLINE(0x7039, 0xAAAA);

EXTERN_API( void )
SetMovieTime                    (Movie                  theMovie,
                                 const TimeRecord *     newtime)                            TWOWORDINLINE(0x703C, 0xAAAA);

EXTERN_API( void )
SetMovieTimeValue               (Movie                  theMovie,
                                 TimeValue              newtime)                            TWOWORDINLINE(0x703D, 0xAAAA);


EXTERN_API( UserData )
GetMovieUserData                (Movie                  theMovie)                           TWOWORDINLINE(0x703E, 0xAAAA);

EXTERN_API( OSErr )
QTGetTimeUntilNextTask          (long *                 duration,
                                 long                   scale)                              THREEWORDINLINE(0x303C, 0x036F, 0xAAAA);

EXTERN_API( OSErr )
QTInstallNextTaskNeededSoonerCallback (QTNextTaskNeededSoonerCallbackUPP  callbackProc,
                                 TimeScale              scale,
                                 unsigned long          flags,
                                 void *                 refcon)                             THREEWORDINLINE(0x303C, 0x0376, 0xAAAA);

EXTERN_API( OSErr )
QTUninstallNextTaskNeededSoonerCallback (QTNextTaskNeededSoonerCallbackUPP  callbackProc,
                                 void *                 refcon)                             THREEWORDINLINE(0x303C, 0x0377, 0xAAAA);

/*************************
* Track/Media finding routines
**************************/
EXTERN_API( long )
GetMovieTrackCount              (Movie                  theMovie)                           TWOWORDINLINE(0x703F, 0xAAAA);

EXTERN_API( Track )
GetMovieTrack                   (Movie                  theMovie,
                                 long                   trackID)                            TWOWORDINLINE(0x7040, 0xAAAA);

EXTERN_API( Track )
GetMovieIndTrack                (Movie                  theMovie,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x0117, 0xAAAA);

EXTERN_API( Track )
GetMovieIndTrackType            (Movie                  theMovie,
                                 long                   index,
                                 OSType                 trackType,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x0208, 0xAAAA);

EXTERN_API( long )
GetTrackID                      (Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x0127, 0xAAAA);

EXTERN_API( Movie )
GetTrackMovie                   (Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x00D0, 0xAAAA);

/*************************
* Track creation routines
**************************/
EXTERN_API( Track )
NewMovieTrack                   (Movie                  theMovie,
                                 Fixed                  width,
                                 Fixed                  height,
                                 short                  trackVolume)                        THREEWORDINLINE(0x303C, 0x0188, 0xAAAA);

EXTERN_API( void )
DisposeMovieTrack               (Track                  theTrack)                           TWOWORDINLINE(0x7042, 0xAAAA);

/*************************
* Track State routines
**************************/
EXTERN_API( unsigned long )
GetTrackCreationTime            (Track                  theTrack)                           TWOWORDINLINE(0x7043, 0xAAAA);

EXTERN_API( unsigned long )
GetTrackModificationTime        (Track                  theTrack)                           TWOWORDINLINE(0x7044, 0xAAAA);


EXTERN_API( Boolean )
GetTrackEnabled                 (Track                  theTrack)                           TWOWORDINLINE(0x7045, 0xAAAA);

EXTERN_API( void )
SetTrackEnabled                 (Track                  theTrack,
                                 Boolean                isEnabled)                          TWOWORDINLINE(0x7046, 0xAAAA);

EXTERN_API( long )
GetTrackUsage                   (Track                  theTrack)                           TWOWORDINLINE(0x7047, 0xAAAA);

EXTERN_API( void )
SetTrackUsage                   (Track                  theTrack,
                                 long                   usage)                              TWOWORDINLINE(0x7048, 0xAAAA);

EXTERN_API( TimeValue )
GetTrackDuration                (Track                  theTrack)                           TWOWORDINLINE(0x704B, 0xAAAA);

EXTERN_API( TimeValue )
GetTrackOffset                  (Track                  theTrack)                           TWOWORDINLINE(0x704C, 0xAAAA);

EXTERN_API( void )
SetTrackOffset                  (Track                  theTrack,
                                 TimeValue              movieOffsetTime)                    TWOWORDINLINE(0x704D, 0xAAAA);

EXTERN_API( short )
GetTrackLayer                   (Track                  theTrack)                           TWOWORDINLINE(0x7050, 0xAAAA);

EXTERN_API( void )
SetTrackLayer                   (Track                  theTrack,
                                 short                  layer)                              TWOWORDINLINE(0x7051, 0xAAAA);

EXTERN_API( Track )
GetTrackAlternate               (Track                  theTrack)                           TWOWORDINLINE(0x7052, 0xAAAA);

EXTERN_API( void )
SetTrackAlternate               (Track                  theTrack,
                                 Track                  alternateT)                         TWOWORDINLINE(0x7053, 0xAAAA);

EXTERN_API( void )
SetAutoTrackAlternatesEnabled   (Movie                  theMovie,
                                 Boolean                enable)                             THREEWORDINLINE(0x303C, 0x015E, 0xAAAA);

EXTERN_API( void )
SelectMovieAlternates           (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x015F, 0xAAAA);

EXTERN_API( short )
GetTrackVolume                  (Track                  theTrack)                           TWOWORDINLINE(0x7054, 0xAAAA);

EXTERN_API( void )
SetTrackVolume                  (Track                  theTrack,
                                 short                  volume)                             TWOWORDINLINE(0x7055, 0xAAAA);

EXTERN_API( void )
GetTrackMatrix                  (Track                  theTrack,
                                 MatrixRecord *         matrix)                             TWOWORDINLINE(0x7056, 0xAAAA);

EXTERN_API( void )
SetTrackMatrix                  (Track                  theTrack,
                                 const MatrixRecord *   matrix)                             TWOWORDINLINE(0x7057, 0xAAAA);

EXTERN_API( void )
GetTrackDimensions              (Track                  theTrack,
                                 Fixed *                width,
                                 Fixed *                height)                             TWOWORDINLINE(0x705D, 0xAAAA);

EXTERN_API( void )
SetTrackDimensions              (Track                  theTrack,
                                 Fixed                  width,
                                 Fixed                  height)                             TWOWORDINLINE(0x705E, 0xAAAA);

EXTERN_API( UserData )
GetTrackUserData                (Track                  theTrack)                           TWOWORDINLINE(0x705F, 0xAAAA);

EXTERN_API( OSErr )
GetTrackDisplayMatrix           (Track                  theTrack,
                                 MatrixRecord *         matrix)                             THREEWORDINLINE(0x303C, 0x0263, 0xAAAA);

EXTERN_API( OSErr )
GetTrackSoundLocalizationSettings (Track                theTrack,
                                 Handle *               settings)                           THREEWORDINLINE(0x303C, 0x0282, 0xAAAA);

EXTERN_API( OSErr )
SetTrackSoundLocalizationSettings (Track                theTrack,
                                 Handle                 settings)                           THREEWORDINLINE(0x303C, 0x0283, 0xAAAA);

/*************************
* get Media routines
**************************/
EXTERN_API( Media )
NewTrackMedia                   (Track                  theTrack,
                                 OSType                 mediaType,
                                 TimeScale              timeScale,
                                 Handle                 dataRef,
                                 OSType                 dataRefType)                        THREEWORDINLINE(0x303C, 0x018E, 0xAAAA);

EXTERN_API( void )
DisposeTrackMedia               (Media                  theMedia)                           TWOWORDINLINE(0x7061, 0xAAAA);

EXTERN_API( Media )
GetTrackMedia                   (Track                  theTrack)                           TWOWORDINLINE(0x7062, 0xAAAA);

EXTERN_API( Track )
GetMediaTrack                   (Media                  theMedia)                           THREEWORDINLINE(0x303C, 0x00C5, 0xAAAA);



/*************************
* Media State routines
**************************/
EXTERN_API( unsigned long )
GetMediaCreationTime            (Media                  theMedia)                           TWOWORDINLINE(0x7066, 0xAAAA);

EXTERN_API( unsigned long )
GetMediaModificationTime        (Media                  theMedia)                           TWOWORDINLINE(0x7067, 0xAAAA);

EXTERN_API( TimeScale )
GetMediaTimeScale               (Media                  theMedia)                           TWOWORDINLINE(0x7068, 0xAAAA);

EXTERN_API( void )
SetMediaTimeScale               (Media                  theMedia,
                                 TimeScale              timeScale)                          TWOWORDINLINE(0x7069, 0xAAAA);

EXTERN_API( TimeValue )
GetMediaDuration                (Media                  theMedia)                           TWOWORDINLINE(0x706A, 0xAAAA);

EXTERN_API( short )
GetMediaLanguage                (Media                  theMedia)                           TWOWORDINLINE(0x706B, 0xAAAA);

EXTERN_API( void )
SetMediaLanguage                (Media                  theMedia,
                                 short                  language)                           TWOWORDINLINE(0x706C, 0xAAAA);

EXTERN_API( short )
GetMediaQuality                 (Media                  theMedia)                           TWOWORDINLINE(0x706D, 0xAAAA);

EXTERN_API( void )
SetMediaQuality                 (Media                  theMedia,
                                 short                  quality)                            TWOWORDINLINE(0x706E, 0xAAAA);

EXTERN_API( void )
GetMediaHandlerDescription      (Media                  theMedia,
                                 OSType *               mediaType,
                                 Str255                 creatorName,
                                 OSType *               creatorManufacturer)                TWOWORDINLINE(0x706F, 0xAAAA);

EXTERN_API( UserData )
GetMediaUserData                (Media                  theMedia)                           TWOWORDINLINE(0x7070, 0xAAAA);

EXTERN_API( OSErr )
GetMediaInputMap                (Media                  theMedia,
                                 QTAtomContainer *      inputMap)                           THREEWORDINLINE(0x303C, 0x0249, 0xAAAA);

EXTERN_API( OSErr )
SetMediaInputMap                (Media                  theMedia,
                                 QTAtomContainer        inputMap)                           THREEWORDINLINE(0x303C, 0x024A, 0xAAAA);

/*************************
* Media Handler routines
**************************/
EXTERN_API( MediaHandler )
GetMediaHandler                 (Media                  theMedia)                           TWOWORDINLINE(0x7071, 0xAAAA);

EXTERN_API( OSErr )
SetMediaHandler                 (Media                  theMedia,
                                 MediaHandlerComponent  mH)                                 THREEWORDINLINE(0x303C, 0x0190, 0xAAAA);


/*************************
* Media's Data routines
**************************/
EXTERN_API( OSErr )
BeginMediaEdits                 (Media                  theMedia)                           TWOWORDINLINE(0x7072, 0xAAAA);

EXTERN_API( OSErr )
EndMediaEdits                   (Media                  theMedia)                           TWOWORDINLINE(0x7073, 0xAAAA);

EXTERN_API( OSErr )
SetMediaDefaultDataRefIndex     (Media                  theMedia,
                                 short                  index)                              THREEWORDINLINE(0x303C, 0x01E0, 0xAAAA);

EXTERN_API( void )
GetMediaDataHandlerDescription  (Media                  theMedia,
                                 short                  index,
                                 OSType *               dhType,
                                 Str255                 creatorName,
                                 OSType *               creatorManufacturer)                THREEWORDINLINE(0x303C, 0x019E, 0xAAAA);

EXTERN_API( DataHandler )
GetMediaDataHandler             (Media                  theMedia,
                                 short                  index)                              THREEWORDINLINE(0x303C, 0x019F, 0xAAAA);

EXTERN_API( OSErr )
SetMediaDataHandler             (Media                  theMedia,
                                 short                  index,
                                 DataHandlerComponent   dataHandler)                        THREEWORDINLINE(0x303C, 0x01A0, 0xAAAA);

EXTERN_API( Component )
GetDataHandler                  (Handle                 dataRef,
                                 OSType                 dataHandlerSubType,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x01ED, 0xAAAA);

EXTERN_API( OSErr )
OpenADataHandler                (Handle                 dataRef,
                                 OSType                 dataHandlerSubType,
                                 Handle                 anchorDataRef,
                                 OSType                 anchorDataRefType,
                                 TimeBase               tb,
                                 long                   flags,
                                 ComponentInstance *    dh)                                 THREEWORDINLINE(0x303C, 0x031C, 0xAAAA);

/*************************
* Media Sample Table Routines
**************************/
EXTERN_API( long )
GetMediaSampleDescriptionCount  (Media                  theMedia)                           TWOWORDINLINE(0x7077, 0xAAAA);

EXTERN_API( void )
GetMediaSampleDescription       (Media                  theMedia,
                                 long                   index,
                                 SampleDescriptionHandle  descH)                            TWOWORDINLINE(0x7078, 0xAAAA);

EXTERN_API( OSErr )
SetMediaSampleDescription       (Media                  theMedia,
                                 long                   index,
                                 SampleDescriptionHandle  descH)                            THREEWORDINLINE(0x303C, 0x01D0, 0xAAAA);

EXTERN_API( long )
GetMediaSampleCount             (Media                  theMedia)                           TWOWORDINLINE(0x7079, 0xAAAA);

EXTERN_API( long )
GetMediaSyncSampleCount         (Media                  theMedia)                           THREEWORDINLINE(0x303C, 0x02B2, 0xAAAA);

EXTERN_API( void )
SampleNumToMediaTime            (Media                  theMedia,
                                 long                   logicalSampleNum,
                                 TimeValue *            sampleTime,
                                 TimeValue *            sampleDuration)                     TWOWORDINLINE(0x707A, 0xAAAA);

EXTERN_API( void )
MediaTimeToSampleNum            (Media                  theMedia,
                                 TimeValue              time,
                                 long *                 sampleNum,
                                 TimeValue *            sampleTime,
                                 TimeValue *            sampleDuration)                     TWOWORDINLINE(0x707B, 0xAAAA);


EXTERN_API( OSErr )
AddMediaSample                  (Media                  theMedia,
                                 Handle                 dataIn,
                                 long                   inOffset,
                                 unsigned long          size,
                                 TimeValue              durationPerSample,
                                 SampleDescriptionHandle  sampleDescriptionH,
                                 long                   numberOfSamples,
                                 short                  sampleFlags,
                                 TimeValue *            sampleTime)                         TWOWORDINLINE(0x707C, 0xAAAA);

EXTERN_API( OSErr )
AddMediaSampleReference         (Media                  theMedia,
                                 long                   dataOffset,
                                 unsigned long          size,
                                 TimeValue              durationPerSample,
                                 SampleDescriptionHandle  sampleDescriptionH,
                                 long                   numberOfSamples,
                                 short                  sampleFlags,
                                 TimeValue *            sampleTime)                         TWOWORDINLINE(0x707D, 0xAAAA);

EXTERN_API( OSErr )
AddMediaSampleReferences        (Media                  theMedia,
                                 SampleDescriptionHandle  sampleDescriptionH,
                                 long                   numberOfSamples,
                                 SampleReferencePtr     sampleRefs,
                                 TimeValue *            sampleTime)                         THREEWORDINLINE(0x303C, 0x01F7, 0xAAAA);

EXTERN_API( OSErr )
AddMediaSampleReferences64      (Media                  theMedia,
                                 SampleDescriptionHandle  sampleDescriptionH,
                                 long                   numberOfSamples,
                                 SampleReference64Ptr   sampleRefs,
                                 TimeValue *            sampleTime)                         THREEWORDINLINE(0x303C, 0x02E8, 0xAAAA);

EXTERN_API( OSErr )
GetMediaSample                  (Media                  theMedia,
                                 Handle                 dataOut,
                                 long                   maxSizeToGrow,
                                 long *                 size,
                                 TimeValue              time,
                                 TimeValue *            sampleTime,
                                 TimeValue *            durationPerSample,
                                 SampleDescriptionHandle  sampleDescriptionH,
                                 long *                 sampleDescriptionIndex,
                                 long                   maxNumberOfSamples,
                                 long *                 numberOfSamples,
                                 short *                sampleFlags)                        TWOWORDINLINE(0x707E, 0xAAAA);

EXTERN_API( OSErr )
GetMediaSampleReference         (Media                  theMedia,
                                 long *                 dataOffset,
                                 long *                 size,
                                 TimeValue              time,
                                 TimeValue *            sampleTime,
                                 TimeValue *            durationPerSample,
                                 SampleDescriptionHandle  sampleDescriptionH,
                                 long *                 sampleDescriptionIndex,
                                 long                   maxNumberOfSamples,
                                 long *                 numberOfSamples,
                                 short *                sampleFlags)                        TWOWORDINLINE(0x707F, 0xAAAA);

EXTERN_API( OSErr )
GetMediaSampleReferences        (Media                  theMedia,
                                 TimeValue              time,
                                 TimeValue *            sampleTime,
                                 SampleDescriptionHandle  sampleDescriptionH,
                                 long *                 sampleDescriptionIndex,
                                 long                   maxNumberOfEntries,
                                 long *                 actualNumberofEntries,
                                 SampleReferencePtr     sampleRefs)                         THREEWORDINLINE(0x303C, 0x0235, 0xAAAA);

EXTERN_API( OSErr )
GetMediaSampleReferences64      (Media                  theMedia,
                                 TimeValue              time,
                                 TimeValue *            sampleTime,
                                 SampleDescriptionHandle  sampleDescriptionH,
                                 long *                 sampleDescriptionIndex,
                                 long                   maxNumberOfEntries,
                                 long *                 actualNumberofEntries,
                                 SampleReference64Ptr   sampleRefs)                         THREEWORDINLINE(0x303C, 0x02E9, 0xAAAA);

EXTERN_API( OSErr )
SetMediaPreferredChunkSize      (Media                  theMedia,
                                 long                   maxChunkSize)                       THREEWORDINLINE(0x303C, 0x01F8, 0xAAAA);

EXTERN_API( OSErr )
GetMediaPreferredChunkSize      (Media                  theMedia,
                                 long *                 maxChunkSize)                       THREEWORDINLINE(0x303C, 0x01F9, 0xAAAA);

EXTERN_API( OSErr )
SetMediaShadowSync              (Media                  theMedia,
                                 long                   frameDiffSampleNum,
                                 long                   syncSampleNum)                      THREEWORDINLINE(0x303C, 0x0121, 0xAAAA);

EXTERN_API( OSErr )
GetMediaShadowSync              (Media                  theMedia,
                                 long                   frameDiffSampleNum,
                                 long *                 syncSampleNum)                      THREEWORDINLINE(0x303C, 0x0122, 0xAAAA);

/*************************
* Editing Routines
**************************/
EXTERN_API( OSErr )
InsertMediaIntoTrack            (Track                  theTrack,
                                 TimeValue              trackStart,
                                 TimeValue              mediaTime,
                                 TimeValue              mediaDuration,
                                 Fixed                  mediaRate)                          THREEWORDINLINE(0x303C, 0x0183, 0xAAAA);

EXTERN_API( OSErr )
InsertTrackSegment              (Track                  srcTrack,
                                 Track                  dstTrack,
                                 TimeValue              srcIn,
                                 TimeValue              srcDuration,
                                 TimeValue              dstIn)                              THREEWORDINLINE(0x303C, 0x0085, 0xAAAA);

EXTERN_API( OSErr )
InsertMovieSegment              (Movie                  srcMovie,
                                 Movie                  dstMovie,
                                 TimeValue              srcIn,
                                 TimeValue              srcDuration,
                                 TimeValue              dstIn)                              THREEWORDINLINE(0x303C, 0x0086, 0xAAAA);

EXTERN_API( OSErr )
InsertEmptyTrackSegment         (Track                  dstTrack,
                                 TimeValue              dstIn,
                                 TimeValue              dstDuration)                        THREEWORDINLINE(0x303C, 0x0087, 0xAAAA);

EXTERN_API( OSErr )
InsertEmptyMovieSegment         (Movie                  dstMovie,
                                 TimeValue              dstIn,
                                 TimeValue              dstDuration)                        THREEWORDINLINE(0x303C, 0x0088, 0xAAAA);

EXTERN_API( OSErr )
DeleteTrackSegment              (Track                  theTrack,
                                 TimeValue              startTime,
                                 TimeValue              duration)                           THREEWORDINLINE(0x303C, 0x0089, 0xAAAA);

EXTERN_API( OSErr )
DeleteMovieSegment              (Movie                  theMovie,
                                 TimeValue              startTime,
                                 TimeValue              duration)                           THREEWORDINLINE(0x303C, 0x008A, 0xAAAA);

EXTERN_API( OSErr )
ScaleTrackSegment               (Track                  theTrack,
                                 TimeValue              startTime,
                                 TimeValue              oldDuration,
                                 TimeValue              newDuration)                        THREEWORDINLINE(0x303C, 0x008B, 0xAAAA);

EXTERN_API( OSErr )
ScaleMovieSegment               (Movie                  theMovie,
                                 TimeValue              startTime,
                                 TimeValue              oldDuration,
                                 TimeValue              newDuration)                        THREEWORDINLINE(0x303C, 0x008C, 0xAAAA);


/*************************
* Hi-level Editing Routines
**************************/
EXTERN_API( Movie )
CutMovieSelection               (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x008D, 0xAAAA);

EXTERN_API( Movie )
CopyMovieSelection              (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x008E, 0xAAAA);

EXTERN_API( void )
PasteMovieSelection             (Movie                  theMovie,
                                 Movie                  src)                                THREEWORDINLINE(0x303C, 0x008F, 0xAAAA);

EXTERN_API( void )
AddMovieSelection               (Movie                  theMovie,
                                 Movie                  src)                                THREEWORDINLINE(0x303C, 0x0152, 0xAAAA);

EXTERN_API( void )
ClearMovieSelection             (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x00E1, 0xAAAA);

EXTERN_API( OSErr )
PasteHandleIntoMovie            (Handle                 h,
                                 OSType                 handleType,
                                 Movie                  theMovie,
                                 long                   flags,
                                 ComponentInstance      userComp)                           THREEWORDINLINE(0x303C, 0x00CB, 0xAAAA);

EXTERN_API( OSErr )
PutMovieIntoTypedHandle         (Movie                  theMovie,
                                 Track                  targetTrack,
                                 OSType                 handleType,
                                 Handle                 publicMovie,
                                 TimeValue              start,
                                 TimeValue              dur,
                                 long                   flags,
                                 ComponentInstance      userComp)                           THREEWORDINLINE(0x303C, 0x01CD, 0xAAAA);

EXTERN_API( Component )
IsScrapMovie                    (Track                  targetTrack)                        THREEWORDINLINE(0x303C, 0x00CC, 0xAAAA);

/*************************
* Middle-level Editing Routines
**************************/
EXTERN_API( OSErr )
CopyTrackSettings               (Track                  srcTrack,
                                 Track                  dstTrack)                           THREEWORDINLINE(0x303C, 0x0153, 0xAAAA);

EXTERN_API( OSErr )
CopyMovieSettings               (Movie                  srcMovie,
                                 Movie                  dstMovie)                           THREEWORDINLINE(0x303C, 0x0154, 0xAAAA);

EXTERN_API( OSErr )
AddEmptyTrackToMovie            (Track                  srcTrack,
                                 Movie                  dstMovie,
                                 Handle                 dataRef,
                                 OSType                 dataRefType,
                                 Track *                dstTrack)                           TWOWORDINLINE(0x7074, 0xAAAA);

enum {
    kQTCloneShareSamples        = 1 << 0,
    kQTCloneDontCopyEdits       = 1 << 1
};

EXTERN_API( OSErr )
AddClonedTrackToMovie           (Track                  srcTrack,
                                 Movie                  dstMovie,
                                 long                   flags,
                                 Track *                dstTrack)                           THREEWORDINLINE(0x303C, 0x0344, 0xAAAA);

/*************************
* movie & track edit state routines
**************************/
EXTERN_API( MovieEditState )
NewMovieEditState               (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x0104, 0xAAAA);

EXTERN_API( OSErr )
UseMovieEditState               (Movie                  theMovie,
                                 MovieEditState         toState)                            THREEWORDINLINE(0x303C, 0x0105, 0xAAAA);

EXTERN_API( OSErr )
DisposeMovieEditState           (MovieEditState         state)                              THREEWORDINLINE(0x303C, 0x0106, 0xAAAA);

EXTERN_API( TrackEditState )
NewTrackEditState               (Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x0107, 0xAAAA);

EXTERN_API( OSErr )
UseTrackEditState               (Track                  theTrack,
                                 TrackEditState         state)                              THREEWORDINLINE(0x303C, 0x0108, 0xAAAA);

EXTERN_API( OSErr )
DisposeTrackEditState           (TrackEditState         state)                              THREEWORDINLINE(0x303C, 0x0109, 0xAAAA);

/*************************
* track reference routines
**************************/
EXTERN_API( OSErr )
AddTrackReference               (Track                  theTrack,
                                 Track                  refTrack,
                                 OSType                 refType,
                                 long *                 addedIndex)                         THREEWORDINLINE(0x303C, 0x01F0, 0xAAAA);

EXTERN_API( OSErr )
DeleteTrackReference            (Track                  theTrack,
                                 OSType                 refType,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x01F1, 0xAAAA);

EXTERN_API( OSErr )
SetTrackReference               (Track                  theTrack,
                                 Track                  refTrack,
                                 OSType                 refType,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x01F2, 0xAAAA);

EXTERN_API( Track )
GetTrackReference               (Track                  theTrack,
                                 OSType                 refType,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x01F3, 0xAAAA);

EXTERN_API( OSType )
GetNextTrackReferenceType       (Track                  theTrack,
                                 OSType                 refType)                            THREEWORDINLINE(0x303C, 0x01F4, 0xAAAA);

EXTERN_API( long )
GetTrackReferenceCount          (Track                  theTrack,
                                 OSType                 refType)                            THREEWORDINLINE(0x303C, 0x01F5, 0xAAAA);


/*************************
* high level file conversion routines
**************************/
EXTERN_API( OSErr )
ConvertFileToMovieFile          (const FSSpec *         inputFile,
                                 const FSSpec *         outputFile,
                                 OSType                 creator,
                                 ScriptCode             scriptTag,
                                 short *                resID,
                                 long                   flags,
                                 ComponentInstance      userComp,
                                 MovieProgressUPP       proc,
                                 long                   refCon)                             THREEWORDINLINE(0x303C, 0x01CB, 0xAAAA);

EXTERN_API( OSErr )
ConvertMovieToFile              (Movie                  theMovie,
                                 Track                  onlyTrack,
                                 FSSpec *               outputFile,
                                 OSType                 fileType,
                                 OSType                 creator,
                                 ScriptCode             scriptTag,
                                 short *                resID,
                                 long                   flags,
                                 ComponentInstance      userComp)                           THREEWORDINLINE(0x303C, 0x01CC, 0xAAAA);

enum {
    kGetMovieImporterValidateToFind = 1L << 0,
    kGetMovieImporterAllowNewFile = 1L << 1,
    kGetMovieImporterDontConsiderGraphicsImporters = 1L << 2,
    kGetMovieImporterDontConsiderFileOnlyImporters = 1L << 6,
    kGetMovieImporterAutoImportOnly = 1L << 10                  /* reject aggressive movie importers which have dontAutoFileMovieImport set*/
};

EXTERN_API( OSErr )
GetMovieImporterForDataRef      (OSType                 dataRefType,
                                 Handle                 dataRef,
                                 long                   flags,
                                 Component *            importer)                           THREEWORDINLINE(0x303C, 0x02C7, 0xAAAA);


enum {
    kQTGetMIMETypeInfoIsQuickTimeMovieType = FOUR_CHAR_CODE('moov'), /* info is a pointer to a Boolean*/
    kQTGetMIMETypeInfoIsUnhelpfulType = FOUR_CHAR_CODE('dumb')  /* info is a pointer to a Boolean*/
};

EXTERN_API( OSErr )
QTGetMIMETypeInfo               (const char *           mimeStringStart,
                                 short                  mimeStringLength,
                                 OSType                 infoSelector,
                                 void *                 infoDataPtr,
                                 long *                 infoDataSize)                       THREEWORDINLINE(0x303C, 0x036A, 0xAAAA);

/*************************
* Movie Timebase Conversion Routines
**************************/
EXTERN_API( TimeValue )
TrackTimeToMediaTime            (TimeValue              value,
                                 Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x0096, 0xAAAA);

EXTERN_API( Fixed )
GetTrackEditRate                (Track                  theTrack,
                                 TimeValue              atTime)                             THREEWORDINLINE(0x303C, 0x0123, 0xAAAA);


/*************************
* Miscellaneous Routines
**************************/

EXTERN_API( long )
GetMovieDataSize                (Movie                  theMovie,
                                 TimeValue              startTime,
                                 TimeValue              duration)                           THREEWORDINLINE(0x303C, 0x0098, 0xAAAA);

EXTERN_API( OSErr )
GetMovieDataSize64              (Movie                  theMovie,
                                 TimeValue              startTime,
                                 TimeValue              duration,
                                 wide *                 dataSize)                           THREEWORDINLINE(0x303C, 0x02EB, 0xAAAA);

EXTERN_API( long )
GetTrackDataSize                (Track                  theTrack,
                                 TimeValue              startTime,
                                 TimeValue              duration)                           THREEWORDINLINE(0x303C, 0x0149, 0xAAAA);

EXTERN_API( OSErr )
GetTrackDataSize64              (Track                  theTrack,
                                 TimeValue              startTime,
                                 TimeValue              duration,
                                 wide *                 dataSize)                           THREEWORDINLINE(0x303C, 0x02EC, 0xAAAA);

EXTERN_API( long )
GetMediaDataSize                (Media                  theMedia,
                                 TimeValue              startTime,
                                 TimeValue              duration)                           THREEWORDINLINE(0x303C, 0x0099, 0xAAAA);

EXTERN_API( OSErr )
GetMediaDataSize64              (Media                  theMedia,
                                 TimeValue              startTime,
                                 TimeValue              duration,
                                 wide *                 dataSize)                           THREEWORDINLINE(0x303C, 0x02ED, 0xAAAA);

EXTERN_API( Boolean )
PtInMovie                       (Movie                  theMovie,
                                 Point                  pt)                                 THREEWORDINLINE(0x303C, 0x009A, 0xAAAA);

EXTERN_API( Boolean )
PtInTrack                       (Track                  theTrack,
                                 Point                  pt)                                 THREEWORDINLINE(0x303C, 0x009B, 0xAAAA);

/*************************
* Group Selection Routines
**************************/

EXTERN_API( void )
SetMovieLanguage                (Movie                  theMovie,
                                 long                   language)                           THREEWORDINLINE(0x303C, 0x009C, 0xAAAA);


/*************************
* User Data
**************************/

EXTERN_API( OSErr )
GetUserData                     (UserData               theUserData,
                                 Handle                 data,
                                 OSType                 udType,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x009E, 0xAAAA);

EXTERN_API( OSErr )
AddUserData                     (UserData               theUserData,
                                 Handle                 data,
                                 OSType                 udType)                             THREEWORDINLINE(0x303C, 0x009F, 0xAAAA);

EXTERN_API( OSErr )
RemoveUserData                  (UserData               theUserData,
                                 OSType                 udType,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x00A0, 0xAAAA);

EXTERN_API( short )
CountUserDataType               (UserData               theUserData,
                                 OSType                 udType)                             THREEWORDINLINE(0x303C, 0x014B, 0xAAAA);

EXTERN_API( long )
GetNextUserDataType             (UserData               theUserData,
                                 OSType                 udType)                             THREEWORDINLINE(0x303C, 0x01A5, 0xAAAA);

EXTERN_API( OSErr )
GetUserDataItem                 (UserData               theUserData,
                                 void *                 data,
                                 long                   size,
                                 OSType                 udType,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x0126, 0xAAAA);

EXTERN_API( OSErr )
SetUserDataItem                 (UserData               theUserData,
                                 void *                 data,
                                 long                   size,
                                 OSType                 udType,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x012E, 0xAAAA);

EXTERN_API( OSErr )
AddUserDataText                 (UserData               theUserData,
                                 Handle                 data,
                                 OSType                 udType,
                                 long                   index,
                                 short                  itlRegionTag)                       THREEWORDINLINE(0x303C, 0x014C, 0xAAAA);

EXTERN_API( OSErr )
GetUserDataText                 (UserData               theUserData,
                                 Handle                 data,
                                 OSType                 udType,
                                 long                   index,
                                 short                  itlRegionTag)                       THREEWORDINLINE(0x303C, 0x014D, 0xAAAA);

EXTERN_API( OSErr )
RemoveUserDataText              (UserData               theUserData,
                                 OSType                 udType,
                                 long                   index,
                                 short                  itlRegionTag)                       THREEWORDINLINE(0x303C, 0x014E, 0xAAAA);

EXTERN_API( OSErr )
NewUserData                     (UserData *             theUserData)                        THREEWORDINLINE(0x303C, 0x012F, 0xAAAA);

EXTERN_API( OSErr )
DisposeUserData                 (UserData               theUserData)                        THREEWORDINLINE(0x303C, 0x0130, 0xAAAA);

EXTERN_API( OSErr )
NewUserDataFromHandle           (Handle                 h,
                                 UserData *             theUserData)                        THREEWORDINLINE(0x303C, 0x0131, 0xAAAA);

EXTERN_API( OSErr )
PutUserDataIntoHandle           (UserData               theUserData,
                                 Handle                 h)                                  THREEWORDINLINE(0x303C, 0x0132, 0xAAAA);


enum {
    kQTCopyUserDataReplace      = FOUR_CHAR_CODE('rplc'),       /* Delete all destination user data items and then add source user data items */
    kQTCopyUserDataMerge        = FOUR_CHAR_CODE('merg')        /* Add source user data items to destination user data */
};

EXTERN_API( OSErr )
CopyMovieUserData               (Movie                  srcMovie,
                                 Movie                  dstMovie,
                                 OSType                 copyRule)                           THREEWORDINLINE(0x303C, 0x0386, 0xAAAA);

EXTERN_API( OSErr )
CopyTrackUserData               (Track                  srcTrack,
                                 Track                  dstTrack,
                                 OSType                 copyRule)                           THREEWORDINLINE(0x303C, 0x0387, 0xAAAA);

EXTERN_API( OSErr )
CopyMediaUserData               (Media                  srcMedia,
                                 Media                  dstMedia,
                                 OSType                 copyRule)                           THREEWORDINLINE(0x303C, 0x0388, 0xAAAA);

EXTERN_API( OSErr )
CopyUserData                    (UserData               srcUserData,
                                 UserData               dstUserData,
                                 OSType                 copyRule)                           THREEWORDINLINE(0x303C, 0x0389, 0xAAAA);


EXTERN_API( OSErr )
SetMoviePropertyAtom            (Movie                  theMovie,
                                 QTAtomContainer        propertyAtom)                       THREEWORDINLINE(0x303C, 0x0284, 0xAAAA);

EXTERN_API( OSErr )
GetMoviePropertyAtom            (Movie                  theMovie,
                                 QTAtomContainer *      propertyAtom)                       THREEWORDINLINE(0x303C, 0x0285, 0xAAAA);


EXTERN_API( void )
GetMediaNextInterestingTime     (Media                  theMedia,
                                 short                  interestingTimeFlags,
                                 TimeValue              time,
                                 Fixed                  rate,
                                 TimeValue *            interestingTime,
                                 TimeValue *            interestingDuration)                THREEWORDINLINE(0x303C, 0x016D, 0xAAAA);

EXTERN_API( void )
GetTrackNextInterestingTime     (Track                  theTrack,
                                 short                  interestingTimeFlags,
                                 TimeValue              time,
                                 Fixed                  rate,
                                 TimeValue *            interestingTime,
                                 TimeValue *            interestingDuration)                THREEWORDINLINE(0x303C, 0x00E2, 0xAAAA);

EXTERN_API( void )
GetMovieNextInterestingTime     (Movie                  theMovie,
                                 short                  interestingTimeFlags,
                                 short                  numMediaTypes,
                                 const OSType *         whichMediaTypes,
                                 TimeValue              time,
                                 Fixed                  rate,
                                 TimeValue *            interestingTime,
                                 TimeValue *            interestingDuration)                THREEWORDINLINE(0x303C, 0x010E, 0xAAAA);


EXTERN_API( OSErr )
CreateMovieFile                 (const FSSpec *         fileSpec,
                                 OSType                 creator,
                                 ScriptCode             scriptTag,
                                 long                   createMovieFileFlags,
                                 short *                resRefNum,
                                 Movie *                newmovie)                           THREEWORDINLINE(0x303C, 0x0191, 0xAAAA);

EXTERN_API( OSErr )
OpenMovieFile                   (const FSSpec *         fileSpec,
                                 short *                resRefNum,
                                 SInt8                  permission)                         THREEWORDINLINE(0x303C, 0x0192, 0xAAAA);

EXTERN_API( OSErr )
CloseMovieFile                  (short                  resRefNum)                          THREEWORDINLINE(0x303C, 0x00D5, 0xAAAA);

EXTERN_API( OSErr )
DeleteMovieFile                 (const FSSpec *         fileSpec)                           THREEWORDINLINE(0x303C, 0x0175, 0xAAAA);

EXTERN_API( OSErr )
NewMovieFromFile                (Movie *                theMovie,
                                 short                  resRefNum,
                                 short *                resId,
                                 StringPtr              resName,
                                 short                  newMovieFlags,
                                 Boolean *              dataRefWasChanged)                  THREEWORDINLINE(0x303C, 0x00F0, 0xAAAA);

EXTERN_API( OSErr )
NewMovieFromHandle              (Movie *                theMovie,
                                 Handle                 h,
                                 short                  newMovieFlags,
                                 Boolean *              dataRefWasChanged)                  THREEWORDINLINE(0x303C, 0x00F1, 0xAAAA);

EXTERN_API( OSErr )
NewMovieFromDataFork            (Movie *                theMovie,
                                 short                  fRefNum,
                                 long                   fileOffset,
                                 short                  newMovieFlags,
                                 Boolean *              dataRefWasChanged)                  THREEWORDINLINE(0x303C, 0x01B3, 0xAAAA);

EXTERN_API( OSErr )
NewMovieFromDataFork64          (Movie *                theMovie,
                                 long                   fRefNum,
                                 const wide *           fileOffset,
                                 short                  newMovieFlags,
                                 Boolean *              dataRefWasChanged)                  THREEWORDINLINE(0x303C, 0x02EE, 0xAAAA);

EXTERN_API( OSErr )
NewMovieFromUserProc            (Movie *                m,
                                 short                  flags,
                                 Boolean *              dataRefWasChanged,
                                 GetMovieUPP            getProc,
                                 void *                 refCon,
                                 Handle                 defaultDataRef,
                                 OSType                 dataRefType)                        THREEWORDINLINE(0x303C, 0x01EC, 0xAAAA);

EXTERN_API( OSErr )
NewMovieFromDataRef             (Movie *                m,
                                 short                  flags,
                                 short *                id,
                                 Handle                 dataRef,
                                 OSType                 dataRefType)                        THREEWORDINLINE(0x303C, 0x0220, 0xAAAA);

EXTERN_API( OSErr )
NewMovieFromStorageOffset       (Movie *                theMovie,
                                 DataHandler            dh,
                                 const wide *           fileOffset,
                                 short                  newMovieFlags,
                                 Boolean *              dataRefWasChanged)                  THREEWORDINLINE(0x303C, 0x03B4, 0xAAAA);

EXTERN_API( OSErr )
NewMovieForDataRefFromHandle    (Movie *                theMovie,
                                 Handle                 h,
                                 short                  newMovieFlags,
                                 Boolean *              dataRefWasChanged,
                                 Handle                 dataRef,
                                 OSType                 dataRefType)                        THREEWORDINLINE(0x303C, 0x0381, 0xAAAA);

EXTERN_API( OSErr )
AddMovieResource                (Movie                  theMovie,
                                 short                  resRefNum,
                                 short *                resId,
                                 ConstStr255Param       resName)                            THREEWORDINLINE(0x303C, 0x00D7, 0xAAAA);

EXTERN_API( OSErr )
UpdateMovieResource             (Movie                  theMovie,
                                 short                  resRefNum,
                                 short                  resId,
                                 ConstStr255Param       resName)                            THREEWORDINLINE(0x303C, 0x00D8, 0xAAAA);

EXTERN_API( OSErr )
RemoveMovieResource             (short                  resRefNum,
                                 short                  resId)                              THREEWORDINLINE(0x303C, 0x0176, 0xAAAA);

EXTERN_API( OSErr )
CreateMovieStorage              (Handle                 dataRef,
                                 OSType                 dataRefType,
                                 OSType                 creator,
                                 ScriptCode             scriptTag,
                                 long                   createMovieFileFlags,
                                 DataHandler *          outDataHandler,
                                 Movie *                newmovie)                           THREEWORDINLINE(0x303C, 0x038A, 0xAAAA);

EXTERN_API( OSErr )
OpenMovieStorage                (Handle                 dataRef,
                                 OSType                 dataRefType,
                                 long                   flags,
                                 DataHandler *          outDataHandler)                     THREEWORDINLINE(0x303C, 0x038B, 0xAAAA);

EXTERN_API( OSErr )
CloseMovieStorage               (DataHandler            dh)                                 THREEWORDINLINE(0x303C, 0x038C, 0xAAAA);

EXTERN_API( OSErr )
DeleteMovieStorage              (Handle                 dataRef,
                                 OSType                 dataRefType)                        THREEWORDINLINE(0x303C, 0x038D, 0xAAAA);

EXTERN_API( OSErr )
AddMovieToStorage               (Movie                  theMovie,
                                 DataHandler            dh)                                 THREEWORDINLINE(0x303C, 0x038F, 0xAAAA);

EXTERN_API( OSErr )
UpdateMovieInStorage            (Movie                  theMovie,
                                 DataHandler            dh)                                 THREEWORDINLINE(0x303C, 0x0390, 0xAAAA);

EXTERN_API( Boolean )
HasMovieChanged                 (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x00D9, 0xAAAA);

EXTERN_API( void )
ClearMovieChanged               (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x0113, 0xAAAA);

EXTERN_API( OSErr )
SetMovieDefaultDataRef          (Movie                  theMovie,
                                 Handle                 dataRef,
                                 OSType                 dataRefType)                        THREEWORDINLINE(0x303C, 0x01C1, 0xAAAA);

EXTERN_API( OSErr )
GetMovieDefaultDataRef          (Movie                  theMovie,
                                 Handle *               dataRef,
                                 OSType *               dataRefType)                        THREEWORDINLINE(0x303C, 0x01D2, 0xAAAA);

EXTERN_API( OSErr )
SetMovieAnchorDataRef           (Movie                  theMovie,
                                 Handle                 dataRef,
                                 OSType                 dataRefType)                        THREEWORDINLINE(0x303C, 0x0315, 0xAAAA);

EXTERN_API( OSErr )
GetMovieAnchorDataRef           (Movie                  theMovie,
                                 Handle *               dataRef,
                                 OSType *               dataRefType,
                                 long *                 outFlags)                           THREEWORDINLINE(0x303C, 0x0316, 0xAAAA);

EXTERN_API( OSErr )
SetMovieColorTable              (Movie                  theMovie,
                                 CTabHandle             ctab)                               THREEWORDINLINE(0x303C, 0x0205, 0xAAAA);

EXTERN_API( OSErr )
GetMovieColorTable              (Movie                  theMovie,
                                 CTabHandle *           ctab)                               THREEWORDINLINE(0x303C, 0x0206, 0xAAAA);

EXTERN_API( void )
FlattenMovie                    (Movie                  theMovie,
                                 long                   movieFlattenFlags,
                                 const FSSpec *         theFile,
                                 OSType                 creator,
                                 ScriptCode             scriptTag,
                                 long                   createMovieFileFlags,
                                 short *                resId,
                                 ConstStr255Param       resName)                            THREEWORDINLINE(0x303C, 0x019B, 0xAAAA);

EXTERN_API( Movie )
FlattenMovieData                (Movie                  theMovie,
                                 long                   movieFlattenFlags,
                                 const FSSpec *         theFile,
                                 OSType                 creator,
                                 ScriptCode             scriptTag,
                                 long                   createMovieFileFlags)               THREEWORDINLINE(0x303C, 0x019C, 0xAAAA);

EXTERN_API( Movie )
FlattenMovieDataToDataRef       (Movie                  theMovie,
                                 long                   movieFlattenFlags,
                                 Handle                 dataRef,
                                 OSType                 dataRefType,
                                 OSType                 creator,
                                 ScriptCode             scriptTag,
                                 long                   createMovieFileFlags)               THREEWORDINLINE(0x303C, 0x0392, 0xAAAA);

EXTERN_API( void )
SetMovieProgressProc            (Movie                  theMovie,
                                 MovieProgressUPP       p,
                                 long                   refcon)                             THREEWORDINLINE(0x303C, 0x019A, 0xAAAA);

EXTERN_API( void )
GetMovieProgressProc            (Movie                  theMovie,
                                 MovieProgressUPP *     p,
                                 long *                 refcon)                             THREEWORDINLINE(0x303C, 0x0300, 0xAAAA);

EXTERN_API( OSErr )
CreateShortcutMovieFile         (const FSSpec *         fileSpec,
                                 OSType                 creator,
                                 ScriptCode             scriptTag,
                                 long                   createMovieFileFlags,
                                 Handle                 targetDataRef,
                                 OSType                 targetDataRefType)                  THREEWORDINLINE(0x303C, 0x02FA, 0xAAAA);

EXTERN_API( OSErr )
MovieSearchText                 (Movie                  theMovie,
                                 Ptr                    text,
                                 long                   size,
                                 long                   searchFlags,
                                 Track *                searchTrack,
                                 TimeValue *            searchTime,
                                 long *                 searchOffset)                       THREEWORDINLINE(0x303C, 0x0207, 0xAAAA);

EXTERN_API( void )
GetPosterBox                    (Movie                  theMovie,
                                 Rect *                 boxRect)                            THREEWORDINLINE(0x303C, 0x016F, 0xAAAA);

EXTERN_API( void )
SetPosterBox                    (Movie                  theMovie,
                                 const Rect *           boxRect)                            THREEWORDINLINE(0x303C, 0x0170, 0xAAAA);

EXTERN_API( RgnHandle )
GetMovieSegmentDisplayBoundsRgn (Movie                  theMovie,
                                 TimeValue              time,
                                 TimeValue              duration)                           THREEWORDINLINE(0x303C, 0x016C, 0xAAAA);

EXTERN_API( RgnHandle )
GetTrackSegmentDisplayBoundsRgn (Track                  theTrack,
                                 TimeValue              time,
                                 TimeValue              duration)                           THREEWORDINLINE(0x303C, 0x016B, 0xAAAA);

EXTERN_API( void )
SetMovieCoverProcs              (Movie                  theMovie,
                                 MovieRgnCoverUPP       uncoverProc,
                                 MovieRgnCoverUPP       coverProc,
                                 long                   refcon)                             THREEWORDINLINE(0x303C, 0x0179, 0xAAAA);

EXTERN_API( OSErr )
GetMovieCoverProcs              (Movie                  theMovie,
                                 MovieRgnCoverUPP *     uncoverProc,
                                 MovieRgnCoverUPP *     coverProc,
                                 long *                 refcon)                             THREEWORDINLINE(0x303C, 0x01DD, 0xAAAA);

EXTERN_API( ComponentResult )
GetTrackStatus                  (Track                  theTrack)                           THREEWORDINLINE(0x303C, 0x0172, 0xAAAA);

EXTERN_API( ComponentResult )
GetMovieStatus                  (Movie                  theMovie,
                                 Track *                firstProblemTrack)                  THREEWORDINLINE(0x303C, 0x0173, 0xAAAA);

enum {
    kMovieLoadStateError        = -1L,
    kMovieLoadStateLoading      = 1000,
    kMovieLoadStateLoaded       = 2000,
    kMovieLoadStatePlayable     = 10000,
    kMovieLoadStatePlaythroughOK = 20000,
    kMovieLoadStateComplete     = 100000L
};

EXTERN_API( long )
GetMovieLoadState               (Movie                  theMovie)                           THREEWORDINLINE(0x303C, 0x0314, 0xAAAA);

/* Input flags for CanQuickTimeOpenFile/DataRef */
enum {
    kQTDontUseDataToFindImporter = 1L << 0,
    kQTDontLookForMovieImporterIfGraphicsImporterFound = 1L << 1,
    kQTAllowOpeningStillImagesAsMovies = 1L << 2,
    kQTAllowImportersThatWouldCreateNewFile = 1L << 3,
    kQTAllowAggressiveImporters = 1L << 4                       /* eg, TEXT and PICT movie importers*/
};

/* Determines whether the file could be opened using a graphics importer or opened in place as a movie. */
EXTERN_API( OSErr )
CanQuickTimeOpenFile            (FSSpecPtr              fileSpec,
                                 OSType                 fileType,
                                 OSType                 fileNameExtension,
                                 Boolean *              outCanOpenWithGraphicsImporter,
                                 Boolean *              outCanOpenAsMovie,
                                 Boolean *              outPreferGraphicsImporter,
                                 UInt32                 inFlags)                            THREEWORDINLINE(0x303C, 0x033E, 0xAAAA);

/* Determines whether the file could be opened using a graphics importer or opened in place as a movie. */
EXTERN_API( OSErr )
CanQuickTimeOpenDataRef         (Handle                 dataRef,
                                 OSType                 dataRefType,
                                 Boolean *              outCanOpenWithGraphicsImporter,
                                 Boolean *              outCanOpenAsMovie,
                                 Boolean *              outPreferGraphicsImporter,
                                 UInt32                 inFlags)                            THREEWORDINLINE(0x303C, 0x033F, 0xAAAA);

/****
    Movie Controller support routines
****/
EXTERN_API( ComponentInstance )
NewMovieController              (Movie                  theMovie,
                                 const Rect *           movieRect,
                                 long                   someFlags)                          THREEWORDINLINE(0x303C, 0x018A, 0xAAAA);

EXTERN_API( void )
DisposeMovieController          (ComponentInstance      mc)                                 THREEWORDINLINE(0x303C, 0x018B, 0xAAAA);

EXTERN_API( void )
ShowMovieInformation            (Movie                  theMovie,
                                 ModalFilterUPP         filterProc,
                                 long                   refCon)                             THREEWORDINLINE(0x303C, 0x0209, 0xAAAA);


/*****
    Scrap routines
*****/
EXTERN_API( OSErr )
PutMovieOnScrap                 (Movie                  theMovie,
                                 long                   movieScrapFlags)                    THREEWORDINLINE(0x303C, 0x018C, 0xAAAA);

EXTERN_API( Movie )
NewMovieFromScrap               (long                   newMovieFlags)                      THREEWORDINLINE(0x303C, 0x018D, 0xAAAA);


/*****
    DataRef routines
*****/

EXTERN_API( OSErr )
GetMediaDataRef                 (Media                  theMedia,
                                 short                  index,
                                 Handle *               dataRef,
                                 OSType *               dataRefType,
                                 long *                 dataRefAttributes)                  THREEWORDINLINE(0x303C, 0x0197, 0xAAAA);

EXTERN_API( OSErr )
SetMediaDataRef                 (Media                  theMedia,
                                 short                  index,
                                 Handle                 dataRef,
                                 OSType                 dataRefType)                        THREEWORDINLINE(0x303C, 0x01C9, 0xAAAA);

EXTERN_API( OSErr )
SetMediaDataRefAttributes       (Media                  theMedia,
                                 short                  index,
                                 long                   dataRefAttributes)                  THREEWORDINLINE(0x303C, 0x01CA, 0xAAAA);

EXTERN_API( OSErr )
AddMediaDataRef                 (Media                  theMedia,
                                 short *                index,
                                 Handle                 dataRef,
                                 OSType                 dataRefType)                        THREEWORDINLINE(0x303C, 0x0198, 0xAAAA);

EXTERN_API( OSErr )
GetMediaDataRefCount            (Media                  theMedia,
                                 short *                count)                              THREEWORDINLINE(0x303C, 0x0199, 0xAAAA);

EXTERN_API( OSErr )
QTNewAlias                      (const FSSpec *         fss,
                                 AliasHandle *          alias,
                                 Boolean                minimal)                            THREEWORDINLINE(0x303C, 0x0201, 0xAAAA);

/*****
    Playback hint routines
*****/
EXTERN_API( void )
SetMoviePlayHints               (Movie                  theMovie,
                                 long                   flags,
                                 long                   flagsMask)                          THREEWORDINLINE(0x303C, 0x01A1, 0xAAAA);

EXTERN_API( void )
SetMediaPlayHints               (Media                  theMedia,
                                 long                   flags,
                                 long                   flagsMask)                          THREEWORDINLINE(0x303C, 0x01A2, 0xAAAA);

EXTERN_API( void )
GetMediaPlayHints               (Media                  theMedia,
                                 long *                 flags)                              THREEWORDINLINE(0x303C, 0x02CE, 0xAAAA);

/*****
    Load time track hints
*****/
enum {
    preloadAlways               = 1L << 0,
    preloadOnlyIfEnabled        = 1L << 1
};

EXTERN_API( void )
SetTrackLoadSettings            (Track                  theTrack,
                                 TimeValue              preloadTime,
                                 TimeValue              preloadDuration,
                                 long                   preloadFlags,
                                 long                   defaultHints)                       THREEWORDINLINE(0x303C, 0x01E3, 0xAAAA);

EXTERN_API( void )
GetTrackLoadSettings            (Track                  theTrack,
                                 TimeValue *            preloadTime,
                                 TimeValue *            preloadDuration,
                                 long *                 preloadFlags,
                                 long *                 defaultHints)                       THREEWORDINLINE(0x303C, 0x01E4, 0xAAAA);

/*****
    Big screen TV
*****/
enum {
    fullScreenHideCursor        = 1L << 0,
    fullScreenAllowEvents       = 1L << 1,
    fullScreenDontChangeMenuBar = 1L << 2,
    fullScreenPreflightSize     = 1L << 3
};

EXTERN_API( OSErr )
BeginFullScreen                 (Ptr *                  restoreState,
                                 GDHandle               whichGD,
                                 short *                desiredWidth,
                                 short *                desiredHeight,
                                 WindowRef *            newWindow,
                                 RGBColor *             eraseColor,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x0233, 0xAAAA);

EXTERN_API( OSErr )
EndFullScreen                   (Ptr                    fullState,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x0234, 0xAAAA);

/*****
    Wired Actions
*****/
/* flags for MovieExecuteWiredActions*/
enum {
    movieExecuteWiredActionDontExecute = 1L << 0
};

EXTERN_API( OSErr )
AddMovieExecuteWiredActionsProc (Movie                  theMovie,
                                 MovieExecuteWiredActionsUPP  proc,
                                 void *                 refCon)                             THREEWORDINLINE(0x303C, 0x0302, 0xAAAA);

EXTERN_API( OSErr )
RemoveMovieExecuteWiredActionsProc (Movie               theMovie,
                                 MovieExecuteWiredActionsUPP  proc,
                                 void *                 refCon)                             THREEWORDINLINE(0x303C, 0x0303, 0xAAAA);

EXTERN_API( OSErr )
MovieExecuteWiredActions        (Movie                  theMovie,
                                 long                   flags,
                                 QTAtomContainer        actions)                            THREEWORDINLINE(0x303C, 0x0304, 0xAAAA);



/*****
    Keyboard Navigation/Editable Text Field Support
*****/
/*
   Navigation Direction Constants
        for MediaNavigateTargetRefCon
*/
enum {
    kRefConNavigationNext       = 0,
    kRefConNavigationPrevious   = 1
};

/*
   Refcon Properties 
        for MediaRefConGetProerty/MediaRefConSetProperty
*/
enum {
    kRefConPropertyCanHaveFocus = 1,                            /* Boolean */
    kRefConPropertyHasFocus     = 2                             /* Boolean */
};

/* media properties*/
enum {
    kTrackFocusCanEditFlag      = FOUR_CHAR_CODE('kedt'),
    kTrackDefaultFocusFlags     = FOUR_CHAR_CODE('kfoc'),
    kTrackFocusDefaultRefcon    = FOUR_CHAR_CODE('kref')
};

/* focus flags*/
enum {
    kTrackFocusOn               = 1,
    kTrackHandlesTabs           = 2                             /* this is reserved for a future release*/
};



/*****
    Flash track properties
*****/
enum {
    kFlashTrackPropertyAcceptAllClicks = FOUR_CHAR_CODE('clik') /* type of media property atom; data is a Boolean */
};

/*****
    Sprite Toolbox
*****/
enum {
    kBackgroundSpriteLayerNum   = 32767
};


/*  Sprite Properties*/
enum {
    kSpritePropertyMatrix       = 1,
    kSpritePropertyImageDescription = 2,
    kSpritePropertyImageDataPtr = 3,
    kSpritePropertyVisible      = 4,
    kSpritePropertyLayer        = 5,
    kSpritePropertyGraphicsMode = 6,
    kSpritePropertyImageDataSize = 7,
    kSpritePropertyActionHandlingSpriteID = 8,
    kSpritePropertyCanBeHitTested = 9,
    kSpritePropertyImageIndex   = 100,
    kSpriteTrackPropertyBackgroundColor = 101,
    kSpriteTrackPropertyOffscreenBitDepth = 102,
    kSpriteTrackPropertySampleFormat = 103,
    kSpriteTrackPropertyScaleSpritesToScaleWorld = 104,
    kSpriteTrackPropertyHasActions = 105,
    kSpriteTrackPropertyVisible = 106,
    kSpriteTrackPropertyQTIdleEventsFrequency = 107,
    kSpriteTrackPropertyAllSpritesHitTestingMode = 108,
    kSpriteTrackPropertyPreferredDepthInterpretationMode = 109,
    kSpriteImagePropertyRegistrationPoint = 1000,
    kSpriteImagePropertyGroupID = 1001
};

/* values for kSpriteTrackPropertyPreferredDepthInterpretationMode*/
enum {
    kSpriteTrackPreferredDepthCompatibilityMode = 0,
    kSpriteTrackPreferredDepthModernMode = 1
};

/* values for kSpriteTrackPropertyAllSpritesHitTestingMode*/
enum {
    kSpriteHitTestUseSpritesOwnPropertiesMode = 0,
    kSpriteHitTestTreatAllSpritesAsHitTestableMode = 1,
    kSpriteHitTestTreatAllSpritesAsNotHitTestableMode = 2
};

/* special value for kSpriteTrackPropertyQTIdleEventsFrequency (the default)*/
enum {
    kNoQTIdleEvents             = -1
};

/* GetSpriteProperties for accessing invalid SpriteWorldRegion*/
enum {
    kGetSpriteWorldInvalidRegionAndLeaveIntact = -1L,
    kGetSpriteWorldInvalidRegionAndThenSetEmpty = -2L
};

/* flagsIn for SpriteWorldIdle*/
enum {
    kOnlyDrawToSpriteWorld      = 1L << 0,
    kSpriteWorldPreflight       = 1L << 1
};

/* flagsOut for SpriteWorldIdle*/
enum {
    kSpriteWorldDidDraw         = 1L << 0,
    kSpriteWorldNeedsToDraw     = 1L << 1
};

/* flags for sprite track sample format*/
enum {
    kKeyFrameAndSingleOverride  = 1L << 1,
    kKeyFrameAndAllOverrides    = 1L << 2
};

/* sprite world flags*/
enum {
    kScaleSpritesToScaleWorld   = 1L << 1,
    kSpriteWorldHighQuality     = 1L << 2,
    kSpriteWorldDontAutoInvalidate = 1L << 3,
    kSpriteWorldInvisible       = 1L << 4,
    kSpriteWorldDirtyInsteadOfFlush = 1L << 5
};

EXTERN_API( OSErr )
NewSpriteWorld                  (SpriteWorld *          newSpriteWorld,
                                 GWorldPtr              destination,
                                 GWorldPtr              spriteLayer,
                                 RGBColor *             backgroundColor,
                                 GWorldPtr              background)                         THREEWORDINLINE(0x303C, 0x0239, 0xAAAA);

EXTERN_API( void )
DisposeSpriteWorld              (SpriteWorld            theSpriteWorld)                     THREEWORDINLINE(0x303C, 0x023A, 0xAAAA);

EXTERN_API( OSErr )
SetSpriteWorldClip              (SpriteWorld            theSpriteWorld,
                                 RgnHandle              clipRgn)                            THREEWORDINLINE(0x303C, 0x023B, 0xAAAA);

EXTERN_API( OSErr )
SetSpriteWorldMatrix            (SpriteWorld            theSpriteWorld,
                                 const MatrixRecord *   matrix)                             THREEWORDINLINE(0x303C, 0x023C, 0xAAAA);

EXTERN_API( OSErr )
SetSpriteWorldGraphicsMode      (SpriteWorld            theSpriteWorld,
                                 long                   mode,
                                 const RGBColor *       opColor)                            THREEWORDINLINE(0x303C, 0x02D9, 0xAAAA);

EXTERN_API( OSErr )
SpriteWorldIdle                 (SpriteWorld            theSpriteWorld,
                                 long                   flagsIn,
                                 long *                 flagsOut)                           THREEWORDINLINE(0x303C, 0x023D, 0xAAAA);

EXTERN_API( OSErr )
InvalidateSpriteWorld           (SpriteWorld            theSpriteWorld,
                                 Rect *                 invalidArea)                        THREEWORDINLINE(0x303C, 0x023E, 0xAAAA);

EXTERN_API( OSErr )
SpriteWorldHitTest              (SpriteWorld            theSpriteWorld,
                                 long                   flags,
                                 Point                  loc,
                                 Sprite *               spriteHit)                          THREEWORDINLINE(0x303C, 0x0246, 0xAAAA);

EXTERN_API( OSErr )
SpriteHitTest                   (Sprite                 theSprite,
                                 long                   flags,
                                 Point                  loc,
                                 Boolean *              wasHit)                             THREEWORDINLINE(0x303C, 0x0247, 0xAAAA);

EXTERN_API( void )
DisposeAllSprites               (SpriteWorld            theSpriteWorld)                     THREEWORDINLINE(0x303C, 0x023F, 0xAAAA);

EXTERN_API( OSErr )
SetSpriteWorldFlags             (SpriteWorld            spriteWorld,
                                 long                   flags,
                                 long                   flagsMask)                          THREEWORDINLINE(0x303C, 0x02C2, 0xAAAA);

EXTERN_API( OSErr )
NewSprite                       (Sprite *               newSprite,
                                 SpriteWorld            itsSpriteWorld,
                                 ImageDescriptionHandle  idh,
                                 Ptr                    imageDataPtr,
                                 MatrixRecord *         matrix,
                                 Boolean                visible,
                                 short                  layer)                              THREEWORDINLINE(0x303C, 0x0240, 0xAAAA);

EXTERN_API( void )
DisposeSprite                   (Sprite                 theSprite)                          THREEWORDINLINE(0x303C, 0x0241, 0xAAAA);

EXTERN_API( void )
InvalidateSprite                (Sprite                 theSprite)                          THREEWORDINLINE(0x303C, 0x0242, 0xAAAA);

EXTERN_API( OSErr )
SetSpriteProperty               (Sprite                 theSprite,
                                 long                   propertyType,
                                 void *                 propertyValue)                      THREEWORDINLINE(0x303C, 0x0243, 0xAAAA);

EXTERN_API( OSErr )
GetSpriteProperty               (Sprite                 theSprite,
                                 long                   propertyType,
                                 void *                 propertyValue)                      THREEWORDINLINE(0x303C, 0x0244, 0xAAAA);

/*****
    QT Atom Data Support
*****/
enum {
    kParentAtomIsContainer      = 0
};

/* create and dispose QTAtomContainer objects*/

EXTERN_API( OSErr )
QTNewAtomContainer              (QTAtomContainer *      atomData)                           THREEWORDINLINE(0x303C, 0x020C, 0xAAAA);

EXTERN_API( OSErr )
QTDisposeAtomContainer          (QTAtomContainer        atomData)                           THREEWORDINLINE(0x303C, 0x020D, 0xAAAA);

/* locating nested atoms within QTAtomContainer container*/

EXTERN_API( QTAtomType )
QTGetNextChildType              (QTAtomContainer        container,
                                 QTAtom                 parentAtom,
                                 QTAtomType             currentChildType)                   THREEWORDINLINE(0x303C, 0x020E, 0xAAAA);

EXTERN_API( short )
QTCountChildrenOfType           (QTAtomContainer        container,
                                 QTAtom                 parentAtom,
                                 QTAtomType             childType)                          THREEWORDINLINE(0x303C, 0x020F, 0xAAAA);

EXTERN_API( QTAtom )
QTFindChildByIndex              (QTAtomContainer        container,
                                 QTAtom                 parentAtom,
                                 QTAtomType             atomType,
                                 short                  index,
                                 QTAtomID *             id)                                 THREEWORDINLINE(0x303C, 0x0210, 0xAAAA);

EXTERN_API( QTAtom )
QTFindChildByID                 (QTAtomContainer        container,
                                 QTAtom                 parentAtom,
                                 QTAtomType             atomType,
                                 QTAtomID               id,
                                 short *                index)                              THREEWORDINLINE(0x303C, 0x021D, 0xAAAA);

EXTERN_API( OSErr )
QTNextChildAnyType              (QTAtomContainer        container,
                                 QTAtom                 parentAtom,
                                 QTAtom                 currentChild,
                                 QTAtom *               nextChild)                          THREEWORDINLINE(0x303C, 0x0200, 0xAAAA);

/* set a leaf atom's data*/
EXTERN_API( OSErr )
QTSetAtomData                   (QTAtomContainer        container,
                                 QTAtom                 atom,
                                 long                   dataSize,
                                 void *                 atomData)                           THREEWORDINLINE(0x303C, 0x0211, 0xAAAA);

/* extracting data*/
EXTERN_API( OSErr )
QTCopyAtomDataToHandle          (QTAtomContainer        container,
                                 QTAtom                 atom,
                                 Handle                 targetHandle)                       THREEWORDINLINE(0x303C, 0x0212, 0xAAAA);

EXTERN_API( OSErr )
QTCopyAtomDataToPtr             (QTAtomContainer        container,
                                 QTAtom                 atom,
                                 Boolean                sizeOrLessOK,
                                 long                   size,
                                 void *                 targetPtr,
                                 long *                 actualSize)                         THREEWORDINLINE(0x303C, 0x0213, 0xAAAA);

EXTERN_API( OSErr )
QTGetAtomTypeAndID              (QTAtomContainer        container,
                                 QTAtom                 atom,
                                 QTAtomType *           atomType,
                                 QTAtomID *             id)                                 THREEWORDINLINE(0x303C, 0x0232, 0xAAAA);

/* extract a copy of an atom and all of it's children, caller disposes*/
EXTERN_API( OSErr )
QTCopyAtom                      (QTAtomContainer        container,
                                 QTAtom                 atom,
                                 QTAtomContainer *      targetContainer)                    THREEWORDINLINE(0x303C, 0x0214, 0xAAAA);

/* obtaining direct reference to atom data*/
EXTERN_API( OSErr )
QTLockContainer                 (QTAtomContainer        container)                          THREEWORDINLINE(0x303C, 0x0215, 0xAAAA);

EXTERN_API( OSErr )
QTGetAtomDataPtr                (QTAtomContainer        container,
                                 QTAtom                 atom,
                                 long *                 dataSize,
                                 Ptr *                  atomData)                           THREEWORDINLINE(0x303C, 0x0216, 0xAAAA);

EXTERN_API( OSErr )
QTUnlockContainer               (QTAtomContainer        container)                          THREEWORDINLINE(0x303C, 0x0217, 0xAAAA);

/*
   building QTAtomContainer trees
   creates and inserts new atom at specified index, existing atoms at or after index are moved toward end of list
   used for Top-Down tree creation
*/
EXTERN_API( OSErr )
QTInsertChild                   (QTAtomContainer        container,
                                 QTAtom                 parentAtom,
                                 QTAtomType             atomType,
                                 QTAtomID               id,
                                 short                  index,
                                 long                   dataSize,
                                 void *                 data,
                                 QTAtom *               newAtom)                            THREEWORDINLINE(0x303C, 0x0218, 0xAAAA);

/* inserts children from childrenContainer as children of parentAtom*/
EXTERN_API( OSErr )
QTInsertChildren                (QTAtomContainer        container,
                                 QTAtom                 parentAtom,
                                 QTAtomContainer        childrenContainer)                  THREEWORDINLINE(0x303C, 0x0219, 0xAAAA);

/* destruction*/
EXTERN_API( OSErr )
QTRemoveAtom                    (QTAtomContainer        container,
                                 QTAtom                 atom)                               THREEWORDINLINE(0x303C, 0x021A, 0xAAAA);

EXTERN_API( OSErr )
QTRemoveChildren                (QTAtomContainer        container,
                                 QTAtom                 atom)                               THREEWORDINLINE(0x303C, 0x021B, 0xAAAA);

/* replacement must be same type as target*/
EXTERN_API( OSErr )
QTReplaceAtom                   (QTAtomContainer        targetContainer,
                                 QTAtom                 targetAtom,
                                 QTAtomContainer        replacementContainer,
                                 QTAtom                 replacementAtom)                    THREEWORDINLINE(0x303C, 0x021C, 0xAAAA);

EXTERN_API( OSErr )
QTSwapAtoms                     (QTAtomContainer        container,
                                 QTAtom                 atom1,
                                 QTAtom                 atom2)                              THREEWORDINLINE(0x303C, 0x01FF, 0xAAAA);

EXTERN_API( OSErr )
QTSetAtomID                     (QTAtomContainer        container,
                                 QTAtom                 atom,
                                 QTAtomID               newID)                              THREEWORDINLINE(0x303C, 0x0231, 0xAAAA);

EXTERN_API( QTAtom )
QTGetAtomParent                 (QTAtomContainer        container,
                                 QTAtom                 childAtom)                          THREEWORDINLINE(0x303C, 0x02EF, 0xAAAA);

EXTERN_API( OSErr )
SetMediaPropertyAtom            (Media                  theMedia,
                                 QTAtomContainer        propertyAtom)                       THREEWORDINLINE(0x303C, 0x022E, 0xAAAA);

EXTERN_API( OSErr )
GetMediaPropertyAtom            (Media                  theMedia,
                                 QTAtomContainer *      propertyAtom)                       THREEWORDINLINE(0x303C, 0x022F, 0xAAAA);


/*****
    Tween Support
*****/
typedef struct TweenRecord              TweenRecord;
typedef CALLBACK_API( ComponentResult , TweenerDataProcPtr )(TweenRecord *tr, void *tweenData, long tweenDataSize, long dataDescriptionSeed, Handle dataDescription, ICMCompletionProcRecordPtr asyncCompletionProc, UniversalProcPtr transferProc, void *refCon);
typedef STACK_UPP_TYPE(TweenerDataProcPtr)                      TweenerDataUPP;

struct TweenRecord {
    long                            version;

    QTAtomContainer                 container;
    QTAtom                          tweenAtom;
    QTAtom                          dataAtom;
    Fixed                           percent;

    TweenerDataUPP                  dataProc;

    void *                          private1;
    void *                          private2;
};


struct TweenV1Record {
    long                            version;

    QTAtomContainer                 container;
    QTAtom                          tweenAtom;
    QTAtom                          dataAtom;
    Fixed                           percent;

    TweenerDataUPP                  dataProc;

    void *                          private1;
    void *                          private2;

    Fract                           fractPercent;
};
typedef struct TweenV1Record            TweenV1Record;
enum {
    kTweenRecordNoFlags         = 0,
    kTweenRecordIsAtInterruptTime = 0x00000001
};


struct TweenV2Record {
    long                            version;

    QTAtomContainer                 container;
    QTAtom                          tweenAtom;
    QTAtom                          dataAtom;
    Fixed                           percent;

    TweenerDataUPP                  dataProc;

    void *                          private1;
    void *                          private2;

    Fract                           fractPercent;

    long                            flags;
};
typedef struct TweenV2Record            TweenV2Record;
EXTERN_API( OSErr )
QTNewTween                      (QTTweener *            tween,
                                 QTAtomContainer        container,
                                 QTAtom                 tweenAtom,
                                 TimeValue              maxTime)                            THREEWORDINLINE(0x303C, 0x029D, 0xAAAA);

EXTERN_API( OSErr )
QTDisposeTween                  (QTTweener              tween)                              THREEWORDINLINE(0x303C, 0x029F, 0xAAAA);

EXTERN_API( OSErr )
QTDoTween                       (QTTweener              tween,
                                 TimeValue              atTime,
                                 Handle                 result,
                                 long *                 resultSize,
                                 TweenerDataUPP         tweenDataProc,
                                 void *                 tweenDataRefCon)                    THREEWORDINLINE(0x303C, 0x029E, 0xAAAA);

/*
    QTDoTweenPtr is an interrupt-safe version of QTDoTween.  It has the following limitations:
     - not all tween types support this call (those which must allocated memory), in which case they return codecUnimpErr.
     - the QTAtomContainer used for the tween must be locked
     - the dataSize must be large enough to contain the result
     - this call is not supported for sequence tweens, use interpolation tweens instead
*/
EXTERN_API( OSErr )
QTDoTweenPtr                    (QTTweener              tween,
                                 TimeValue              atTime,
                                 Ptr                    result,
                                 long                   resultSize)                         THREEWORDINLINE(0x303C, 0x036E, 0xAAAA);

/*****
    Sound Description Manipulations
*****/
EXTERN_API( OSErr )
AddSoundDescriptionExtension    (SoundDescriptionHandle  desc,
                                 Handle                 extension,
                                 OSType                 idType)                             THREEWORDINLINE(0x303C, 0x02CF, 0xAAAA);

EXTERN_API( OSErr )
GetSoundDescriptionExtension    (SoundDescriptionHandle  desc,
                                 Handle *               extension,
                                 OSType                 idType)                             THREEWORDINLINE(0x303C, 0x02D0, 0xAAAA);

EXTERN_API( OSErr )
RemoveSoundDescriptionExtension (SoundDescriptionHandle  desc,
                                 OSType                 idType)                             THREEWORDINLINE(0x303C, 0x02D1, 0xAAAA);

/*****
    Preferences
*****/
EXTERN_API( OSErr )
GetQuickTimePreference          (OSType                 preferenceType,
                                 QTAtomContainer *      preferenceAtom)                     THREEWORDINLINE(0x303C, 0x02D4, 0xAAAA);

EXTERN_API( OSErr )
SetQuickTimePreference          (OSType                 preferenceType,
                                 QTAtomContainer        preferenceAtom)                     THREEWORDINLINE(0x303C, 0x02D5, 0xAAAA);

/*****
    Effects and dialog Support
*****/
/* atom types for entries in the effects list*/
enum {
    kEffectNameAtom             = FOUR_CHAR_CODE('name'),       /* name of effect */
    kEffectTypeAtom             = FOUR_CHAR_CODE('type'),       /* codec sub-type for effect */
    kEffectManufacturerAtom     = FOUR_CHAR_CODE('manu')        /* codec manufacturer for effect */
};


struct QTParamPreviewRecord {
    long                            sourceID;                   /* 1 based source identifier*/
    PicHandle                       sourcePicture;              /* picture for preview, must not dispose until dialog is disposed*/
};
typedef struct QTParamPreviewRecord     QTParamPreviewRecord;

typedef QTParamPreviewRecord *          QTParamPreviewPtr;

struct QTParamDialogEventRecord {
    EventRecord *                   theEvent;                   /* Event received by the dialog */
    DialogRef                       whichDialog;                /* dialog that event was directed towards */
    short                           itemHit;                    /* dialog item which was hit */
};
typedef struct QTParamDialogEventRecord QTParamDialogEventRecord;

typedef QTParamDialogEventRecord *      QTParamDialogEventPtr;

struct QTParamFetchPreviewRecord {
    GWorldPtr                       theWorld;                   /* the world into which to draw the preview */
    Fixed                           percentage;                 /* frame percentage (from 0.0 - 1.0) to be drawn */
};
typedef struct QTParamFetchPreviewRecord QTParamFetchPreviewRecord;

typedef QTParamFetchPreviewRecord *     QTParamFetchPreviewPtr;
enum {
    pdActionConfirmDialog       = 1,                            /* no param*/
    pdActionSetAppleMenu        = 2,                            /* param is MenuRef*/
    pdActionSetEditMenu         = 3,                            /* param is MenuRef*/
    pdActionGetDialogValues     = 4,                            /* param is QTAtomContainer*/
    pdActionSetPreviewUserItem  = 5,                            /* param is long*/
    pdActionSetPreviewPicture   = 6,                            /* param is QTParamPreviewPtr;*/
    pdActionSetColorPickerEventProc = 7,                        /* param is UserEventUPP*/
    pdActionSetDialogTitle      = 8,                            /* param is StringPtr */
    pdActionGetSubPanelMenu     = 9,                            /* param is MenuRef* */
    pdActionActivateSubPanel    = 10,                           /* param is long */
    pdActionConductStopAlert    = 11,                           /* param is StringPtr */
    pdActionModelessCallback    = 12,                           /* param is QTParamDialogEventPtr */
    pdActionFetchPreview        = 13,                           /* param is QTParamFetchPreviewPtr */
    pdActionSetDialogSettings   = 14,                           /* param is QTAtomContainer */
    pdActionGetDialogSettings   = 15,                           /* param is QTAtomContainer */
    pdActionGetNextSample       = 16,                           /* param is QTAtomContainer with effect sample to change - createdDialog may be NIL */
    pdActionGetPreviousSample   = 17,                           /* param is QTAtomContainer with effect sample to change - createdDialog may be NIL */
    pdActionCompactSample       = 18,                           /* param is QTAtomContainer with effect sample to compact, - createdDialog may be NIL */
    pdActionSetEditCallout      = 19,                           /* param is QTParamPreviewCalloutPtr, can be NIL */
    pdActionSetSampleTime       = 20,                           /* param is QTParamSampleTimePtr, can be NIL */
    pdActionDoEditCommand       = 21,                           /* param is long with menu command (ie, mcMenuCut etc) */
    pdActionGetSubPanelMenuValue = 22,                          /* param is long and returns current sub-panel value selected by the effect */
                                                                /* Action codes and typedefs used for custom controls within effects */
    pdActionCustomNewControl    = 23,                           /* param is QTCustomControlNewPtr */
    pdActionCustomDisposeControl = 24,                          /* param is QTCustomControlNewPtr */
    pdActionCustomPositionControl = 25,                         /* param is QTCustomControlPositionControlPtr */
    pdActionCustomShowHideControl = 26,                         /* param is QTCustomControlShowHideControlPtr */
    pdActionCustomHandleEvent   = 27,                           /* param is QTCustomControlHandleEventPtr */
    pdActionCustomSetFocus      = 28,                           /* param is QTCustomControlSetFocusPtr */
    pdActionCustomSetEditMenu   = 29,                           /* param is QTCustomControlSetEditMenuPtr */
    pdActionCustomSetPreviewPicture = 30,                       /* param is QTCustomControlSetPreviewPicturePtr */
    pdActionCustomSetEditCallout = 31,                          /* param is QTCustomControlSetEditCalloutPtr */
    pdActionCustomGetEnableValue = 32,                          /* param is QTCustomControlGetEnableValuePtr */
    pdActionCustomSetSampleTime = 33,                           /* param is QTCustomControlSetSampleTimePtr */
    pdActionCustomGetValue      = 34,                           /* param is QTCustomControlGetValue */
    pdActionCustomDoEditCommand = 35                            /* param is QTCustomControlDoEditCommand */
};

/* Sample Time information */
enum {
    pdSampleTimeDisplayOptionsNone = 0x00000000
};


struct QTParamSampleTimeRecord {
    long                            displayOptions;
    TimeRecord                      sampleStartTime;
    TimeValue                       sampleDuration;
    long                            framesPerSecond;            /* if 0, will cause revert to seconds display*/
};
typedef struct QTParamSampleTimeRecord  QTParamSampleTimeRecord;

typedef QTParamSampleTimeRecord *       QTParamSampleTimePtr;
/* Preview change callout information */

struct QTParamPreviewCalloutRecord {
    MoviePreviewCallOutUPP          calloutProc;                /* called when user makes editing changes to dialog.  May be NIL.  You should return true from your function. */
    long                            refCon;                     /* passed to the callout procedure */
};
typedef struct QTParamPreviewCalloutRecord QTParamPreviewCalloutRecord;

typedef QTParamPreviewCalloutRecord *   QTParamPreviewCalloutPtr;
typedef long                            QTParameterDialogOptions;
/* ------- CUSTOM EFFECT CONTROLS*/
/*
    Effects may choose to implement custom controls to allow the user to more easily edit complex parameters
    that are ill-served by simple sliders or type in boxes.   Effects may allow a custom control for either
    a single parameter, or for a group of parameters.
    
    Parameter(s) for a custom control must still be data types defined by the standard set, or for
    complex records of data, must be defined within a group as individual parameters made up from base
    data types (for example, a point is a group containing two Fixed point numbers).  
    This is to allow applications that do not wish to use the custom control for the effect to set values themselves.
    
    Effects should be aware that these custom controls may be deployed by the application in either a dialog or
    a window, with application defined background colors or patterns, along with application defined font
    characteristics for the window.
    
    It is recommended that effects implement custom controls only when needed, and that custom controls be used
    for specific types of parameters (ie, point, rectangle, polygon, path) rather than the entire user interface
    for the effect.  Effects may choose to implement multiple custom controls which combine with standard controls
    to present the total user interface.  For effects which have very complex user interfaces not well suited for 
    inclusion within a single window, it is recommended to use kParameterImageIsPreset -- which allows the effect to
    have an external editing application for parameters which may then be set within the standard UI via the open file 
    dialog or drag and drop.  The Lens Flare effect's "Flare Type" is an example of such a preset.

    For parameters that use a custom control to control a single parameter value, a new behavior
    flag has been added (kCustomControl), and the behavior for the parameter should be kParameterItemControl.
    
    For parameters that are groups, the same flag (kCustomControl) should be used, and the behavior
    should be kParameterItemGroupDivider.  Groups with the kCustomControl bit set will be implemented
    by calling the custom control for that group -- the parameters within that group will not be processed
    in the normal manner.
    
    In both cases, the new customType and customID fields of the behavior must be filled in.  These are 
    used in order to allow your custom control to determine which parameter is being edited in the case
    where the custom control is used for the editing of multiple parameters.  These values are passed into
    the pdActionCustomNewControl call.  Since the custom control mechanism is also used by QuickTime's
    default effect dialogs, you should be prepared to pass onto the base effect any pdActionCustomNewControl
    calls for type/id pairs that you do not handle yourself.  When  pdActionCustomNewControl is called
    for controls of types handled by QuickTime, customType is kParameterAtomTypeAndID and customID is
    the ID of the parameter atom. 
*/


/*
    pdActionCustomNewControlControl is called by application to create a new custom control or set of controls
    for an effect parameter.  When pdActionCustomNewControl is called, the effect should perform any
    basic allocation it needs for storage and return the result in storage. The options parameter tells
    the control if the application wishes to support interpolated, optionally interpolated, or a single
    value parameter.
    
    Since pdActionCustomNewControlControl may be called upon your effect for other items within the
    dialog, it is recommended that your effect have an easy way to determine which controls it implements:
     a) by having storage be a pointer with an OSType at the begining to mark controls
        implemented by your code.
     - or -
     b) keeping track in your component globals those custom controls which you have created.
    
    When pdActionCustomDisposeControl is called any allocation done by the control should be disposed. In addition, 
    pdActionCustomDisposeControl is the last chance the control has to commit any user changes into the sample.
    Controls which implement type in fields typically need to commit any final user edits at this time.
*/

struct QTCustomControlNewRecord {
    void *                          storage;                    /* storage allocated/disposed by the control*/
    QTParameterDialogOptions        options;                    /* options used to control interpolation/not*/
    QTAtomContainer                 sample;                     /* sample that holds the data to be edited*/
    long                            customType;                 /* custom type and ID specified by effect for creation of this control*/
    long                            customID;
};
typedef struct QTCustomControlNewRecord QTCustomControlNewRecord;

typedef QTCustomControlNewRecord *      QTCustomControlNewPtr;
/*
    pdActionCustomPositionControl is called by the application to position the control within a window or dialog.

    The control should determine if it will fit in the alloted area and position itself there.  It should also
    return the space taken up by the control.   Note you are free to implement controls which are variable in size depending upon
    which parameter you are editing.  You need not scale your control to the requested size.  If the area presented to your
    control is too small, set didFit to false.  You should still return in used the size you would have liked to use for
    the control.   The application will then try again with a new size.  Note that all
    controls must be able to fit within a minimum of 300 by 250 pixels.
    
    Custom controls that draw text should make note of the text font, size, and style at this time in order
    to properly display within application windows.
    
    Note that the default state for the control is hidden.  You will receive a pdActionCustomShowHideControl
    in order to enable your control.  You should not draw your control in response to pdActionCustomPositionControl.
*/

struct QTCustomControlPositionControlRecord {
    void *                          storage;                    /* storage for the control*/
    WindowPtr                       window;                     /* window to be used by the control*/
    Rect                            location;                   /* location within the window the control may use*/
    Rect                            used;                       /* returned by the control to indicate size it actually used*/
    Boolean                         didFit;                     /* did the control fit in the specified area?*/
    Boolean                         pad[3];
};
typedef struct QTCustomControlPositionControlRecord QTCustomControlPositionControlRecord;

typedef QTCustomControlPositionControlRecord * QTCustomControlPositionControlPtr;
/*
    pdActionCustomShowHideControl is called when the application wishes to enable/disable your control, or 
    completely disable drawing of the control
    
    Your control should make note of the new state (if different from the last) and perform an InvalRect()
    on your drawing area, or you may draw your control's initial state in the case of show.  You should not
    attempt to erase your control as the result of a hide -- instead call InvalRect() and allow the application
    to process the resulting event as appropriate.
*/

struct QTCustomControlShowHideControlRecord {
    void *                          storage;                    /* storage for the control*/
    Boolean                         show;                       /* display the control?*/
    Boolean                         enable;                     /* enable the control (ie, black vs gray display)*/
    Boolean                         pad[2];
};
typedef struct QTCustomControlShowHideControlRecord QTCustomControlShowHideControlRecord;

typedef QTCustomControlShowHideControlRecord * QTCustomControlShowHideControlPtr;
/*
    pdActionCustomHandleEvent is called to allow your custom control to process events.
    
    Typical controls handle the following events:
        - activate - to draw your control in normal/gray mode
        - update - to draw your control
        - mouseDown - to handle clicks
        - keyDown - to handle typing when you have focus
        - idle - to perform idle drawing (if applicable)
    If your control handles the entire event, set didProcess to true.  If
    you handled the event, but other controls still need the event, set didProcess to false.
    
    If your control supports the concept of focus for the purposes of typing (such as by having
    a type-in box for the parameter) then you set the tookFocus Boolean as part of your processing
    of the event.  It is assumed that your control will draw the appropriate focus UI as a result, and
    the calling application will disable any focus drawing within the remainder of the UI.

    By default, custom controls are not given idle time.  If you need idle time, set needIdle to true
    in response to the even that causes you to need idle (typically the taking of focus, or the first draw).
    Your control will continue to be given idle events until you set needIdle to false in response to
    a nullEvent.
*/

struct QTCustomControlHandleEventRecord {
    void *                          storage;                    /* storage for the control*/
    EventRecord *                   pEvent;                     /* event to process*/
    Boolean                         didProcess;                 /* did we process entire event?*/
    Boolean                         tookFocus;                  /* did we take focus as a result of this event (typically mouseDowns)*/
    Boolean                         needIdle;                   /* does this control need idle events?*/
    Boolean                         didEdit;                    /* did we edit the samples?*/
};
typedef struct QTCustomControlHandleEventRecord QTCustomControlHandleEventRecord;

typedef QTCustomControlHandleEventRecord * QTCustomControlHandleEventPtr;
/*
    pdActionCustomSetFocus is called in order to set or advance the current focus of the user interface, typically
    because the user has pressed the tab or shift-tab keys, or because the user clicked within the area defined by
    your control.
    
    Your control will be called with pdActionFocusFirst,  pdActionFocusLast, or pdActionFocusOff to set or clear focus on your
    control.  Your control will be called with pdActionFocusForward or pdActionFocusBackward to cycle
    focus within your control (if your control has multiple focus).  If your control does not support focus,
    or the focus request results in focus moving beyond your supported range, return pdActionFocusOff in
    the focus parameter.  Otherwise, return the focus that you set.
    
    Controls which have no focus would always set focus to be pdActionFocusOff.
    
    Controls with a single focus would set pdActionFocusFirst when requsted to set either
    pdActionFocusFirst or pdActionFocusLast, and would set pdActionFocusOff for either
    pdActionFocusForward or pdActionFocusBackward.
*/
enum {
    pdActionFocusOff            = 0,                            /* no focus */
    pdActionFocusFirst          = 1,                            /* focus on first element */
    pdActionFocusLast           = 2,                            /* focus on last element */
    pdActionFocusForward        = 3,                            /* focus on next element */
    pdActionFocusBackward       = 4                             /* focus on previous element */
};


struct QTCustomControlSetFocusRecord {
    void *                          storage;                    /* storage for the control*/
    long                            focus;                      /* focus to set, return resulting focus*/
};
typedef struct QTCustomControlSetFocusRecord QTCustomControlSetFocusRecord;

typedef QTCustomControlSetFocusRecord * QTCustomControlSetFocusPtr;
/* 
    pdActionCustomSetEditMenu will be called to inform your custom control of the location of the edit menu.
    
    If your control has editing boxes, this is useful in order to allow the user to perform cut/copy/paste operations
    when focus is on one of these boxes.
*/

struct QTCustomControlSetEditMenuRecord {
    void *                          storage;                    /* storage for the control*/
    MenuHandle                      editMenu;                   /* edit menu, or NIL*/
};
typedef struct QTCustomControlSetEditMenuRecord QTCustomControlSetEditMenuRecord;

typedef QTCustomControlSetEditMenuRecord * QTCustomControlSetEditMenuPtr;
/*
    pdActionCustomSetPreviewPicture will be called to inform your custom control of preview information that you
    may wish to use in the drawing of your user interface.  
*/

struct QTCustomControlSetPreviewPictureRecord {
    void *                          storage;                    /* storage for the control*/
    QTParamPreviewPtr               preview;                    /* preview to set*/
};
typedef struct QTCustomControlSetPreviewPictureRecord QTCustomControlSetPreviewPictureRecord;

typedef QTCustomControlSetPreviewPictureRecord * QTCustomControlSetPreviewPicturePtr;
/*
    pdActionCustomSetEditCallout tells your control of the need by the application to be informed of
    changes to the parameter values (typically for the purposes of updating previews).
    
    If a callout is available, your custom control should call it whenever a change has been
    made to the parameter(s) that your control is editing (as a result of user actions, most typically).
    If you choose not to implement this, live dragging or updating of values will not work.
*/

struct QTCustomControlSetEditCalloutRecord {
    void *                          storage;                    /* storage for the control*/
    QTParamPreviewCalloutPtr        callout;                    /* requested callout, or NIL to disable*/
};
typedef struct QTCustomControlSetEditCalloutRecord QTCustomControlSetEditCalloutRecord;

typedef QTCustomControlSetEditCalloutRecord * QTCustomControlSetEditCalloutPtr;
/*
    pdActionCustomGetEnableValue allows you to return a value for the purposes of enabling/disabling
    other controls.
    Most custom controls do not need to implement this call.
    
    If your control is able to control the enabling and disabling of other parameter controls (such as is done
    by standard pop up or enumerated type controls), you need to supply a value that can be use for greater than/less than
    types of comparisons.
*/

struct QTCustomControlGetEnableValueRecord {
    void *                          storage;                    /* storage for the control*/
    long                            currentValue;               /* value to compare against for enable/disable purposes*/
};
typedef struct QTCustomControlGetEnableValueRecord QTCustomControlGetEnableValueRecord;

typedef QTCustomControlGetEnableValueRecord * QTCustomControlGetEnableValuePtr;
/*
    pdActionCustomSetSampleTime tells your control information from the application about the duration
    and start time for the sample being edited.
    
    Most controls do not need this information, but some may choose to use it in the interface
    they present the user.  However, this call need not be made by applications, so the custom
    control should be prepared to run when the sample time information is not available.
*/

struct QTCustomControlSetSampleTimeRecord {
    void *                          storage;                    /* storage for the control*/
    QTParamSampleTimePtr            sampleTime;                 /* sample time information or NIL*/
};
typedef struct QTCustomControlSetSampleTimeRecord QTCustomControlSetSampleTimeRecord;

typedef QTCustomControlSetSampleTimeRecord * QTCustomControlSetSampleTimePtr;
/*
    pdActionCustomGetValue tells your control to store any value(s) into the specified atom container.
    
    All custom controls must implement this call
*/

struct QTCustomControlGetValueRecord {
    void *                          storage;                    /* storage for the control*/
    QTAtomContainer                 sample;                     /* sample to store into*/
};
typedef struct QTCustomControlGetValueRecord QTCustomControlGetValueRecord;

typedef QTCustomControlGetValueRecord * QTCustomControlGetValuePtr;
/*
    pdActionCustomDoEditCommand tells your control to handle edit commands if it allow focus and type in boxes.
    
    All custom controls must implement this call if they support edit boxes
*/

struct QTCustomControlDoEditCommandRecord {
    void *                          storage;                    /* storage for the control*/
    long                            command;                    /* command to execute, return 0 here if processed*/
};
typedef struct QTCustomControlDoEditCommandRecord QTCustomControlDoEditCommandRecord;

typedef QTCustomControlDoEditCommandRecord * QTCustomControlDoEditCommandPtr;
typedef long                            QTParameterDialog;
enum {
    elOptionsIncludeNoneInList  = 0x00000001                    /* "None" effect is included in list */
};

typedef long                            QTEffectListOptions;
enum {
    pdOptionsCollectOneValue    = 0x00000001,                   /* should collect a single value only*/
    pdOptionsAllowOptionalInterpolations = 0x00000002,          /* non-novice interpolation options are shown */
    pdOptionsModalDialogBox     = 0x00000004,                   /* dialog box should be modal */
    pdOptionsEditCurrentEffectOnly = 0x00000008,                /* List of effects will not be shown */
    pdOptionsHidePreview        = 0x00000010                    /* Preview item will not be shown */
};

enum {
    effectIsRealtime            = 0                             /* effect can be rendered in real time */
};

/*
    QTGetEffectsListExtended is a call that provides for 
    more advanced filtering of effects to be placed into the
    effect list.  Applications can filter on:
     1) number of input sources
     2) effect major or minor class
     3) custom filtering through a callback
    The callback will be called for each effect which passes
    the other criteria for inclusion.  If the callback
    returns a true result the effect will be included in the list.
    
    Note that your filter proc may receive multiple effects from various
    manufacturers.  If you return true for multiple effects of a given type
    only the one with the higher parameter version number will be included.
    If you wish other filtering (such as effects from a given manufacturer, you
    can do this by return false for the other effects and true for those
    that you prefer.
*/
typedef CALLBACK_API( Boolean , QTEffectListFilterProcPtr )(Component effect, long effectMinSource, long effectMaxSource, OSType majorClass, OSType minorClass, void *refcon);
typedef STACK_UPP_TYPE(QTEffectListFilterProcPtr)               QTEffectListFilterUPP;
EXTERN_API( OSErr )
QTGetEffectsList                (QTAtomContainer *      returnedList,
                                 long                   minSources,
                                 long                   maxSources,
                                 QTEffectListOptions    getOptions)                         THREEWORDINLINE(0x303C, 0x02C9, 0xAAAA);

EXTERN_API( OSErr )
QTGetEffectsListExtended        (QTAtomContainer *      returnedList,
                                 long                   minSources,
                                 long                   maxSources,
                                 QTEffectListOptions    getOptions,
                                 OSType                 majorClass,
                                 OSType                 minorClass,
                                 QTEffectListFilterUPP  filterProc,
                                 void *                 filterRefCon)                       THREEWORDINLINE(0x303C, 0x036D, 0xAAAA);

EXTERN_API( OSErr )
QTCreateStandardParameterDialog (QTAtomContainer        effectList,
                                 QTAtomContainer        parameters,
                                 QTParameterDialogOptions  dialogOptions,
                                 QTParameterDialog *    createdDialog)                      THREEWORDINLINE(0x303C, 0x02CA, 0xAAAA);

EXTERN_API( OSErr )
QTIsStandardParameterDialogEvent (EventRecord *         pEvent,
                                 QTParameterDialog      createdDialog)                      THREEWORDINLINE(0x303C, 0x02CB, 0xAAAA);

EXTERN_API( OSErr )
QTDismissStandardParameterDialog (QTParameterDialog     createdDialog)                      THREEWORDINLINE(0x303C, 0x02CC, 0xAAAA);

EXTERN_API( OSErr )
QTStandardParameterDialogDoAction (QTParameterDialog    createdDialog,
                                 long                   action,
                                 void *                 params)                             THREEWORDINLINE(0x303C, 0x02CD, 0xAAAA);

EXTERN_API( OSErr )
QTGetEffectSpeed                (QTAtomContainer        parameters,
                                 Fixed *                pFPS)                               THREEWORDINLINE(0x303C, 0x02D2, 0xAAAA);

/*****
    Error reporting
*****/

struct QTErrorReplacementRecord {
    long                            numEntries;
    StringPtr                       replacementString[1];       /* array of numEntries StringPtrs (each String is allocated separately).*/
};
typedef struct QTErrorReplacementRecord QTErrorReplacementRecord;

typedef QTErrorReplacementRecord *      QTErrorReplacementPtr;
/*
    QTAddMovieError is used to add orthogonal errors to a list of errors that will
    later be reported (at the end of an import or playback, for example).  Errors are stored
    in 'qter' resources within the component.
    
    QTAddMovieError(Movie       addTo,                          // in: movie to add error to
                    Component   adder,                          // in: component which is adding the error
                    long        errorCode,                      // in: error code being added
                    QTErrorReplacementPtr   stringReplacements);// in: list of strings to subsitute (in order) for "^1", "^2", etc
*/
EXTERN_API( OSErr )
QTAddMovieError                 (Movie                  movieH,
                                 Component              c,
                                 long                   errorCode,
                                 QTErrorReplacementPtr  stringReplacements)                 THREEWORDINLINE(0x303C, 0x037A, 0xAAAA);


/*****
    Access Keys
*****/
enum {
    kAccessKeyAtomType          = FOUR_CHAR_CODE('acky')
};

enum {
    kAccessKeySystemFlag        = 1L << 0
};

EXTERN_API( OSErr )
QTGetAccessKeys                 (Str255                 accessKeyType,
                                 long                   flags,
                                 QTAtomContainer *      keys)                               THREEWORDINLINE(0x303C, 0x02B3, 0xAAAA);

EXTERN_API( OSErr )
QTRegisterAccessKey             (Str255                 accessKeyType,
                                 long                   flags,
                                 Handle                 accessKey)                          THREEWORDINLINE(0x303C, 0x02B4, 0xAAAA);

EXTERN_API( OSErr )
QTUnregisterAccessKey           (Str255                 accessKeyType,
                                 long                   flags,
                                 Handle                 accessKey)                          THREEWORDINLINE(0x303C, 0x02B5, 0xAAAA);

/*****
    Content Restrictions
*****/

EXTERN_API( OSErr )
QTGetMovieRestrictions          (Movie                  theMovie,
                                 QTRestrictionSet *     outRestrictionSet,
                                 UInt32 *               outSeed)                            THREEWORDINLINE(0x303C, 0x03A5, 0xAAAA);


EXTERN_API( OSErr )
QTRestrictionsGetInfo           (QTRestrictionSet       inRestrictionSet,
                                 long *                 outRestrictionClassCount,
                                 long *                 outSeed)                            THREEWORDINLINE(0x303C, 0x03A1, 0xAAAA);

EXTERN_API( OSErr )
QTRestrictionsGetIndClass       (QTRestrictionSet       inRestrictionSet,
                                 long                   inIndex,
                                 OSType *               outClass)                           THREEWORDINLINE(0x303C, 0x03A2, 0xAAAA);

EXTERN_API( OSErr )
QTRestrictionsGetItem           (QTRestrictionSet       inRestrictionSet,
                                 OSType                 inRestrictionClass,
                                 UInt32 *               outRestrictions)                    THREEWORDINLINE(0x303C, 0x03A3, 0xAAAA);


EXTERN_API( OSErr )
QTGetSupportedRestrictions      (OSType                 inRestrictionClass,
                                 UInt32 *               outRestrictionIDs)                  THREEWORDINLINE(0x303C, 0x03B2, 0xAAAA);

EXTERN_API( OSErr )
QTCreateUUID                    (QTUUID *               outUUID,
                                 long                   creationFlags)                      THREEWORDINLINE(0x303C, 0x039B, 0xAAAA);

EXTERN_API( Boolean )
QTEqualUUIDs                    (const QTUUID *         uuid1,
                                 const QTUUID *         uuid2)                              THREEWORDINLINE(0x303C, 0x039C, 0xAAAA);



/*****
    Time table
*****/
EXTERN_API( OSErr )
MakeTrackTimeTable              (Track                  trackH,
                                 long **                offsets,
                                 TimeValue              startTime,
                                 TimeValue              endTime,
                                 TimeValue              timeIncrement,
                                 short                  firstDataRefIndex,
                                 short                  lastDataRefIndex,
                                 long *                 retdataRefSkew)                     THREEWORDINLINE(0x303C, 0x02BE, 0xAAAA);

EXTERN_API( OSErr )
MakeMediaTimeTable              (Media                  theMedia,
                                 long **                offsets,
                                 TimeValue              startTime,
                                 TimeValue              endTime,
                                 TimeValue              timeIncrement,
                                 short                  firstDataRefIndex,
                                 short                  lastDataRefIndex,
                                 long *                 retdataRefSkew)                     THREEWORDINLINE(0x303C, 0x02BF, 0xAAAA);

EXTERN_API( OSErr )
GetMaxLoadedTimeInMovie         (Movie                  theMovie,
                                 TimeValue *            time)                               THREEWORDINLINE(0x303C, 0x02C0, 0xAAAA);

EXTERN_API( OSErr )
QTMovieNeedsTimeTable           (Movie                  theMovie,
                                 Boolean *              needsTimeTable)                     THREEWORDINLINE(0x303C, 0x02C3, 0xAAAA);

EXTERN_API( OSErr )
QTGetDataRefMaxFileOffset       (Movie                  movieH,
                                 OSType                 dataRefType,
                                 Handle                 dataRef,
                                 long *                 offset)                             THREEWORDINLINE(0x303C, 0x02C6, 0xAAAA);


/*****
    Bandwidth management support
*****/
enum {
    ConnectionSpeedPrefsType    = FOUR_CHAR_CODE('cspd'),
    BandwidthManagementPrefsType = FOUR_CHAR_CODE('bwmg')
};



struct ConnectionSpeedPrefsRecord {
    long                            connectionSpeed;
};
typedef struct ConnectionSpeedPrefsRecord ConnectionSpeedPrefsRecord;

typedef ConnectionSpeedPrefsRecord *    ConnectionSpeedPrefsPtr;
typedef ConnectionSpeedPrefsPtr *       ConnectionSpeedPrefsHandle;

struct BandwidthManagementPrefsRecord {
    Boolean                         overrideConnectionSpeedForBandwidth;
};
typedef struct BandwidthManagementPrefsRecord BandwidthManagementPrefsRecord;

typedef BandwidthManagementPrefsRecord * BandwidthManagementPrefsPtr;
typedef BandwidthManagementPrefsPtr *   BandwidthManagementPrefsHandle;
enum {
    kQTIdlePriority             = 10,
    kQTNonRealTimePriority      = 20,
    kQTRealTimeSharedPriority   = 25,
    kQTRealTimePriority         = 30
};

enum {
    kQTBandwidthNotifyNeedToStop = 1L << 0,
    kQTBandwidthNotifyGoodToGo  = 1L << 1,
    kQTBandwidthChangeRequest   = 1L << 2,
    kQTBandwidthQueueRequest    = 1L << 3,
    kQTBandwidthScheduledRequest = 1L << 4,
    kQTBandwidthVoluntaryRelease = 1L << 5
};

typedef CALLBACK_API( OSErr , QTBandwidthNotificationProcPtr )(long flags, void *reserved, void *refcon);

struct QTScheduledBandwidthRecord {
    long                            recordSize;                 /* total number of bytes in QTScheduledBandwidthRecord*/

    long                            priority;
    long                            dataRate;
    CompTimeValue                   startTime;                  /* bandwidth usage start time*/
    CompTimeValue                   duration;                   /* duration of bandwidth usage (0 if unknown)*/
    CompTimeValue                   prerollDuration;            /* time for negotiation before startTime (0 if unknown)*/
    TimeScale                       scale;                      /* timescale of value/duration/prerollDuration fields*/
    TimeBase                        base;                       /* timebase*/
};
typedef struct QTScheduledBandwidthRecord QTScheduledBandwidthRecord;

typedef QTScheduledBandwidthRecord *    QTScheduledBandwidthPtr;
typedef QTScheduledBandwidthPtr *       QTScheduledBandwidthHandle;
typedef STACK_UPP_TYPE(QTBandwidthNotificationProcPtr)          QTBandwidthNotificationUPP;
EXTERN_API( OSErr )
QTBandwidthRequest              (long                   priority,
                                 QTBandwidthNotificationUPP  callback,
                                 const void *           refcon,
                                 QTBandwidthReference * bwRef,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x02F5, 0xAAAA);

EXTERN_API( OSErr )
QTBandwidthRequestForTimeBase   (TimeBase               tb,
                                 long                   priority,
                                 QTBandwidthNotificationUPP  callback,
                                 const void *           refcon,
                                 QTBandwidthReference * bwRef,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x0318, 0xAAAA);

EXTERN_API( OSErr )
QTBandwidthRelease              (QTBandwidthReference   bwRef,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x02F6, 0xAAAA);

EXTERN_API( OSErr )
QTScheduledBandwidthRequest     (QTScheduledBandwidthPtr  scheduleRec,
                                 QTBandwidthNotificationUPP  notificationCallback,
                                 void *                 refcon,
                                 QTScheduledBandwidthReference * sbwRef,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x0310, 0xAAAA);

EXTERN_API( OSErr )
QTScheduledBandwidthRelease     (QTScheduledBandwidthReference  sbwRef,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x0311, 0xAAAA);

#if OPAQUE_UPP_TYPES
    EXTERN_API(QTCallBackUPP)
    NewQTCallBackUPP               (QTCallBackProcPtr       userRoutine);

    EXTERN_API(QTSyncTaskUPP)
    NewQTSyncTaskUPP               (QTSyncTaskProcPtr       userRoutine);

    EXTERN_API(MovieRgnCoverUPP)
    NewMovieRgnCoverUPP            (MovieRgnCoverProcPtr    userRoutine);

    EXTERN_API(MovieProgressUPP)
    NewMovieProgressUPP            (MovieProgressProcPtr    userRoutine);

    EXTERN_API(MovieDrawingCompleteUPP)
    NewMovieDrawingCompleteUPP     (MovieDrawingCompleteProcPtr userRoutine);

    EXTERN_API(TrackTransferUPP)
    NewTrackTransferUPP            (TrackTransferProcPtr    userRoutine);

    EXTERN_API(GetMovieUPP)
    NewGetMovieUPP                 (GetMovieProcPtr         userRoutine);

    EXTERN_API(MoviePreviewCallOutUPP)
    NewMoviePreviewCallOutUPP      (MoviePreviewCallOutProcPtr userRoutine);

    EXTERN_API(TextMediaUPP)
    NewTextMediaUPP                (TextMediaProcPtr        userRoutine);

    EXTERN_API(ActionsUPP)
    NewActionsUPP                  (ActionsProcPtr          userRoutine);

    EXTERN_API(DoMCActionUPP)
    NewDoMCActionUPP               (DoMCActionProcPtr       userRoutine);

    EXTERN_API(MovieExecuteWiredActionsUPP)
    NewMovieExecuteWiredActionsUPP    (MovieExecuteWiredActionsProcPtr userRoutine);

    EXTERN_API(MoviePrePrerollCompleteUPP)
    NewMoviePrePrerollCompleteUPP    (MoviePrePrerollCompleteProcPtr userRoutine);

    EXTERN_API(QTNextTaskNeededSoonerCallbackUPP)
    NewQTNextTaskNeededSoonerCallbackUPP    (QTNextTaskNeededSoonerCallbackProcPtr userRoutine);

    EXTERN_API(MoviesErrorUPP)
    NewMoviesErrorUPP              (MoviesErrorProcPtr      userRoutine);

    EXTERN_API(TweenerDataUPP)
    NewTweenerDataUPP              (TweenerDataProcPtr      userRoutine);

    EXTERN_API(QTEffectListFilterUPP)
    NewQTEffectListFilterUPP       (QTEffectListFilterProcPtr userRoutine);

    EXTERN_API(QTBandwidthNotificationUPP)
    NewQTBandwidthNotificationUPP    (QTBandwidthNotificationProcPtr userRoutine);

    EXTERN_API(void)
    DisposeQTCallBackUPP           (QTCallBackUPP           userUPP);

    EXTERN_API(void)
    DisposeQTSyncTaskUPP           (QTSyncTaskUPP           userUPP);

    EXTERN_API(void)
    DisposeMovieRgnCoverUPP        (MovieRgnCoverUPP        userUPP);

    EXTERN_API(void)
    DisposeMovieProgressUPP        (MovieProgressUPP        userUPP);

    EXTERN_API(void)
    DisposeMovieDrawingCompleteUPP    (MovieDrawingCompleteUPP userUPP);

    EXTERN_API(void)
    DisposeTrackTransferUPP        (TrackTransferUPP        userUPP);

    EXTERN_API(void)
    DisposeGetMovieUPP             (GetMovieUPP             userUPP);

    EXTERN_API(void)
    DisposeMoviePreviewCallOutUPP    (MoviePreviewCallOutUPP userUPP);

    EXTERN_API(void)
    DisposeTextMediaUPP            (TextMediaUPP            userUPP);

    EXTERN_API(void)
    DisposeActionsUPP              (ActionsUPP              userUPP);

    EXTERN_API(void)
    DisposeDoMCActionUPP           (DoMCActionUPP           userUPP);

    EXTERN_API(void)
    DisposeMovieExecuteWiredActionsUPP    (MovieExecuteWiredActionsUPP userUPP);

    EXTERN_API(void)
    DisposeMoviePrePrerollCompleteUPP    (MoviePrePrerollCompleteUPP userUPP);

    EXTERN_API(void)
    DisposeQTNextTaskNeededSoonerCallbackUPP    (QTNextTaskNeededSoonerCallbackUPP userUPP);

    EXTERN_API(void)
    DisposeMoviesErrorUPP          (MoviesErrorUPP          userUPP);

    EXTERN_API(void)
    DisposeTweenerDataUPP          (TweenerDataUPP          userUPP);

    EXTERN_API(void)
    DisposeQTEffectListFilterUPP    (QTEffectListFilterUPP  userUPP);

    EXTERN_API(void)
    DisposeQTBandwidthNotificationUPP    (QTBandwidthNotificationUPP userUPP);

    EXTERN_API(void)
    InvokeQTCallBackUPP            (QTCallBack              cb,
                                    long                    refCon,
                                    QTCallBackUPP           userUPP);

    EXTERN_API(void)
    InvokeQTSyncTaskUPP            (void *                  task,
                                    QTSyncTaskUPP           userUPP);

    EXTERN_API(OSErr)
    InvokeMovieRgnCoverUPP         (Movie                   theMovie,
                                    RgnHandle               changedRgn,
                                    long                    refcon,
                                    MovieRgnCoverUPP        userUPP);

    EXTERN_API(OSErr)
    InvokeMovieProgressUPP         (Movie                   theMovie,
                                    short                   message,
                                    short                   whatOperation,
                                    Fixed                   percentDone,
                                    long                    refcon,
                                    MovieProgressUPP        userUPP);

    EXTERN_API(OSErr)
    InvokeMovieDrawingCompleteUPP    (Movie                 theMovie,
                                    long                    refCon,
                                    MovieDrawingCompleteUPP userUPP);

    EXTERN_API(OSErr)
    InvokeTrackTransferUPP         (Track                   t,
                                    long                    refCon,
                                    TrackTransferUPP        userUPP);

    EXTERN_API(OSErr)
    InvokeGetMovieUPP              (long                    offset,
                                    long                    size,
                                    void *                  dataPtr,
                                    void *                  refCon,
                                    GetMovieUPP             userUPP);

    EXTERN_API(Boolean)
    InvokeMoviePreviewCallOutUPP    (long                   refcon,
                                    MoviePreviewCallOutUPP  userUPP);

    EXTERN_API(OSErr)
    InvokeTextMediaUPP             (Handle                  theText,
                                    Movie                   theMovie,
                                    short *                 displayFlag,
                                    long                    refcon,
                                    TextMediaUPP            userUPP);

    EXTERN_API(OSErr)
    InvokeActionsUPP               (void *                  refcon,
                                    Track                   targetTrack,
                                    long                    targetRefCon,
                                    QTEventRecordPtr        theEvent,
                                    ActionsUPP              userUPP);

    EXTERN_API(OSErr)
    InvokeDoMCActionUPP            (void *                  refcon,
                                    short                   action,
                                    void *                  params,
                                    Boolean *               handled,
                                    DoMCActionUPP           userUPP);

    EXTERN_API(OSErr)
    InvokeMovieExecuteWiredActionsUPP    (Movie             theMovie,
                                    void *                  refcon,
                                    long                    flags,
                                    QTAtomContainer         wiredActions,
                                    MovieExecuteWiredActionsUPP userUPP);

    EXTERN_API(void)
    InvokeMoviePrePrerollCompleteUPP    (Movie              theMovie,
                                    OSErr                   prerollErr,
                                    void *                  refcon,
                                    MoviePrePrerollCompleteUPP userUPP);

    EXTERN_API(void)
    InvokeQTNextTaskNeededSoonerCallbackUPP    (TimeValue   duration,
                                    unsigned long           flags,
                                    void *                  refcon,
                                    QTNextTaskNeededSoonerCallbackUPP userUPP);

    EXTERN_API(void)
    InvokeMoviesErrorUPP           (OSErr                   theErr,
                                    long                    refcon,
                                    MoviesErrorUPP          userUPP);

    EXTERN_API(ComponentResult)
    InvokeTweenerDataUPP           (TweenRecord *           tr,
                                    void *                  tweenData,
                                    long                    tweenDataSize,
                                    long                    dataDescriptionSeed,
                                    Handle                  dataDescription,
                                    ICMCompletionProcRecordPtr asyncCompletionProc,
                                    UniversalProcPtr        transferProc,
                                    void *                  refCon,
                                    TweenerDataUPP          userUPP);

    EXTERN_API(Boolean)
    InvokeQTEffectListFilterUPP    (Component               effect,
                                    long                    effectMinSource,
                                    long                    effectMaxSource,
                                    OSType                  majorClass,
                                    OSType                  minorClass,
                                    void *                  refcon,
                                    QTEffectListFilterUPP   userUPP);

    EXTERN_API(OSErr)
    InvokeQTBandwidthNotificationUPP    (long               flags,
                                    void *                  reserved,
                                    void *                  refcon,
                                    QTBandwidthNotificationUPP userUPP);

#else
    enum { uppQTCallBackProcInfo = 0x000003C0 };                    /* pascal no_return_value Func(4_bytes, 4_bytes) */
    enum { uppQTSyncTaskProcInfo = 0x000000C0 };                    /* pascal no_return_value Func(4_bytes) */
    enum { uppMovieRgnCoverProcInfo = 0x00000FE0 };                 /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppMovieProgressProcInfo = 0x0000FAE0 };                 /* pascal 2_bytes Func(4_bytes, 2_bytes, 2_bytes, 4_bytes, 4_bytes) */
    enum { uppMovieDrawingCompleteProcInfo = 0x000003E0 };          /* pascal 2_bytes Func(4_bytes, 4_bytes) */
    enum { uppTrackTransferProcInfo = 0x000003E0 };                 /* pascal 2_bytes Func(4_bytes, 4_bytes) */
    enum { uppGetMovieProcInfo = 0x00003FE0 };                      /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppMoviePreviewCallOutProcInfo = 0x000000D0 };           /* pascal 1_byte Func(4_bytes) */
    enum { uppTextMediaProcInfo = 0x00003FE0 };                     /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppActionsProcInfo = 0x00003FE0 };                       /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppDoMCActionProcInfo = 0x00003EE0 };                    /* pascal 2_bytes Func(4_bytes, 2_bytes, 4_bytes, 4_bytes) */
    enum { uppMovieExecuteWiredActionsProcInfo = 0x00003FE0 };      /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppMoviePrePrerollCompleteProcInfo = 0x00000EC0 };       /* pascal no_return_value Func(4_bytes, 2_bytes, 4_bytes) */
    enum { uppQTNextTaskNeededSoonerCallbackProcInfo = 0x00000FC0 };  /* pascal no_return_value Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppMoviesErrorProcInfo = 0x00000380 };                   /* pascal no_return_value Func(2_bytes, 4_bytes) */
    enum { uppTweenerDataProcInfo = 0x003FFFF0 };                   /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppQTEffectListFilterProcInfo = 0x0003FFD0 };            /* pascal 1_byte Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    enum { uppQTBandwidthNotificationProcInfo = 0x00000FE0 };       /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewQTCallBackUPP(userRoutine)                           (QTCallBackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppQTCallBackProcInfo, GetCurrentArchitecture())
    #define NewQTSyncTaskUPP(userRoutine)                           (QTSyncTaskUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppQTSyncTaskProcInfo, GetCurrentArchitecture())
    #define NewMovieRgnCoverUPP(userRoutine)                        (MovieRgnCoverUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMovieRgnCoverProcInfo, GetCurrentArchitecture())
    #define NewMovieProgressUPP(userRoutine)                        (MovieProgressUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMovieProgressProcInfo, GetCurrentArchitecture())
    #define NewMovieDrawingCompleteUPP(userRoutine)                 (MovieDrawingCompleteUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMovieDrawingCompleteProcInfo, GetCurrentArchitecture())
    #define NewTrackTransferUPP(userRoutine)                        (TrackTransferUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTrackTransferProcInfo, GetCurrentArchitecture())
    #define NewGetMovieUPP(userRoutine)                             (GetMovieUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGetMovieProcInfo, GetCurrentArchitecture())
    #define NewMoviePreviewCallOutUPP(userRoutine)                  (MoviePreviewCallOutUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMoviePreviewCallOutProcInfo, GetCurrentArchitecture())
    #define NewTextMediaUPP(userRoutine)                            (TextMediaUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTextMediaProcInfo, GetCurrentArchitecture())
    #define NewActionsUPP(userRoutine)                              (ActionsUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppActionsProcInfo, GetCurrentArchitecture())
    #define NewDoMCActionUPP(userRoutine)                           (DoMCActionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDoMCActionProcInfo, GetCurrentArchitecture())
    #define NewMovieExecuteWiredActionsUPP(userRoutine)             (MovieExecuteWiredActionsUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMovieExecuteWiredActionsProcInfo, GetCurrentArchitecture())
    #define NewMoviePrePrerollCompleteUPP(userRoutine)              (MoviePrePrerollCompleteUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMoviePrePrerollCompleteProcInfo, GetCurrentArchitecture())
    #define NewQTNextTaskNeededSoonerCallbackUPP(userRoutine)       (QTNextTaskNeededSoonerCallbackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppQTNextTaskNeededSoonerCallbackProcInfo, GetCurrentArchitecture())
    #define NewMoviesErrorUPP(userRoutine)                          (MoviesErrorUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMoviesErrorProcInfo, GetCurrentArchitecture())
    #define NewTweenerDataUPP(userRoutine)                          (TweenerDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTweenerDataProcInfo, GetCurrentArchitecture())
    #define NewQTEffectListFilterUPP(userRoutine)                   (QTEffectListFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppQTEffectListFilterProcInfo, GetCurrentArchitecture())
    #define NewQTBandwidthNotificationUPP(userRoutine)              (QTBandwidthNotificationUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppQTBandwidthNotificationProcInfo, GetCurrentArchitecture())
    #define DisposeQTCallBackUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposeQTSyncTaskUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposeMovieRgnCoverUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeMovieProgressUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeMovieDrawingCompleteUPP(userUPP)                 DisposeRoutineDescriptor(userUPP)
    #define DisposeTrackTransferUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeGetMovieUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define DisposeMoviePreviewCallOutUPP(userUPP)                  DisposeRoutineDescriptor(userUPP)
    #define DisposeTextMediaUPP(userUPP)                            DisposeRoutineDescriptor(userUPP)
    #define DisposeActionsUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define DisposeDoMCActionUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposeMovieExecuteWiredActionsUPP(userUPP)             DisposeRoutineDescriptor(userUPP)
    #define DisposeMoviePrePrerollCompleteUPP(userUPP)              DisposeRoutineDescriptor(userUPP)
    #define DisposeQTNextTaskNeededSoonerCallbackUPP(userUPP)       DisposeRoutineDescriptor(userUPP)
    #define DisposeMoviesErrorUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposeTweenerDataUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposeQTEffectListFilterUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define DisposeQTBandwidthNotificationUPP(userUPP)              DisposeRoutineDescriptor(userUPP)
    #define InvokeQTCallBackUPP(cb, refCon, userUPP)                CALL_TWO_PARAMETER_UPP((userUPP), uppQTCallBackProcInfo, (cb), (refCon))
    #define InvokeQTSyncTaskUPP(task, userUPP)                      CALL_ONE_PARAMETER_UPP((userUPP), uppQTSyncTaskProcInfo, (task))
    #define InvokeMovieRgnCoverUPP(theMovie, changedRgn, refcon, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppMovieRgnCoverProcInfo, (theMovie), (changedRgn), (refcon))
    #define InvokeMovieProgressUPP(theMovie, message, whatOperation, percentDone, refcon, userUPP)  (OSErr)CALL_FIVE_PARAMETER_UPP((userUPP), uppMovieProgressProcInfo, (theMovie), (message), (whatOperation), (percentDone), (refcon))
    #define InvokeMovieDrawingCompleteUPP(theMovie, refCon, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppMovieDrawingCompleteProcInfo, (theMovie), (refCon))
    #define InvokeTrackTransferUPP(t, refCon, userUPP)              (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppTrackTransferProcInfo, (t), (refCon))
    #define InvokeGetMovieUPP(offset, size, dataPtr, refCon, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppGetMovieProcInfo, (offset), (size), (dataPtr), (refCon))
    #define InvokeMoviePreviewCallOutUPP(refcon, userUPP)           (Boolean)CALL_ONE_PARAMETER_UPP((userUPP), uppMoviePreviewCallOutProcInfo, (refcon))
    #define InvokeTextMediaUPP(theText, theMovie, displayFlag, refcon, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppTextMediaProcInfo, (theText), (theMovie), (displayFlag), (refcon))
    #define InvokeActionsUPP(refcon, targetTrack, targetRefCon, theEvent, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppActionsProcInfo, (refcon), (targetTrack), (targetRefCon), (theEvent))
    #define InvokeDoMCActionUPP(refcon, action, params, handled, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppDoMCActionProcInfo, (refcon), (action), (params), (handled))
    #define InvokeMovieExecuteWiredActionsUPP(theMovie, refcon, flags, wiredActions, userUPP)  (OSErr)CALL_FOUR_PARAMETER_UPP((userUPP), uppMovieExecuteWiredActionsProcInfo, (theMovie), (refcon), (flags), (wiredActions))
    #define InvokeMoviePrePrerollCompleteUPP(theMovie, prerollErr, refcon, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppMoviePrePrerollCompleteProcInfo, (theMovie), (prerollErr), (refcon))
    #define InvokeQTNextTaskNeededSoonerCallbackUPP(duration, flags, refcon, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppQTNextTaskNeededSoonerCallbackProcInfo, (duration), (flags), (refcon))
    #define InvokeMoviesErrorUPP(theErr, refcon, userUPP)           CALL_TWO_PARAMETER_UPP((userUPP), uppMoviesErrorProcInfo, (theErr), (refcon))
    #define InvokeTweenerDataUPP(tr, tweenData, tweenDataSize, dataDescriptionSeed, dataDescription, asyncCompletionProc, transferProc, refCon, userUPP)  (ComponentResult)CALL_EIGHT_PARAMETER_UPP((userUPP), uppTweenerDataProcInfo, (tr), (tweenData), (tweenDataSize), (dataDescriptionSeed), (dataDescription), (asyncCompletionProc), (transferProc), (refCon))
    #define InvokeQTEffectListFilterUPP(effect, effectMinSource, effectMaxSource, majorClass, minorClass, refcon, userUPP)  (Boolean)CALL_SIX_PARAMETER_UPP((userUPP), uppQTEffectListFilterProcInfo, (effect), (effectMinSource), (effectMaxSource), (majorClass), (minorClass), (refcon))
    #define InvokeQTBandwidthNotificationUPP(flags, reserved, refcon, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppQTBandwidthNotificationProcInfo, (flags), (reserved), (refcon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewQTCallBackProc(userRoutine)                          NewQTCallBackUPP(userRoutine)
#define NewQTSyncTaskProc(userRoutine)                          NewQTSyncTaskUPP(userRoutine)
#define NewMovieRgnCoverProc(userRoutine)                       NewMovieRgnCoverUPP(userRoutine)
#define NewMovieProgressProc(userRoutine)                       NewMovieProgressUPP(userRoutine)
#define NewMovieDrawingCompleteProc(userRoutine)                NewMovieDrawingCompleteUPP(userRoutine)
#define NewTrackTransferProc(userRoutine)                       NewTrackTransferUPP(userRoutine)
#define NewGetMovieProc(userRoutine)                            NewGetMovieUPP(userRoutine)
#define NewMoviePreviewCallOutProc(userRoutine)                 NewMoviePreviewCallOutUPP(userRoutine)
#define NewTextMediaProc(userRoutine)                           NewTextMediaUPP(userRoutine)
#define NewActionsProc(userRoutine)                             NewActionsUPP(userRoutine)
#define NewDoMCActionProc(userRoutine)                          NewDoMCActionUPP(userRoutine)
#define NewMovieExecuteWiredActionsProc(userRoutine)            NewMovieExecuteWiredActionsUPP(userRoutine)
#define NewMoviePrePrerollCompleteProc(userRoutine)             NewMoviePrePrerollCompleteUPP(userRoutine)
#define NewQTNextTaskNeededSoonerCallbackProc(userRoutine)      NewQTNextTaskNeededSoonerCallbackUPP(userRoutine)
#define NewMoviesErrorProc(userRoutine)                         NewMoviesErrorUPP(userRoutine)
#define NewTweenerDataProc(userRoutine)                         NewTweenerDataUPP(userRoutine)
#define NewQTEffectListFilterProc(userRoutine)                  NewQTEffectListFilterUPP(userRoutine)
#define NewQTBandwidthNotificationProc(userRoutine)             NewQTBandwidthNotificationUPP(userRoutine)
#define CallQTCallBackProc(userRoutine, cb, refCon)             InvokeQTCallBackUPP(cb, refCon, userRoutine)
#define CallQTSyncTaskProc(userRoutine, task)                   InvokeQTSyncTaskUPP(task, userRoutine)
#define CallMovieRgnCoverProc(userRoutine, theMovie, changedRgn, refcon) InvokeMovieRgnCoverUPP(theMovie, changedRgn, refcon, userRoutine)
#define CallMovieProgressProc(userRoutine, theMovie, message, whatOperation, percentDone, refcon) InvokeMovieProgressUPP(theMovie, message, whatOperation, percentDone, refcon, userRoutine)
#define CallMovieDrawingCompleteProc(userRoutine, theMovie, refCon) InvokeMovieDrawingCompleteUPP(theMovie, refCon, userRoutine)
#define CallTrackTransferProc(userRoutine, t, refCon)           InvokeTrackTransferUPP(t, refCon, userRoutine)
#define CallGetMovieProc(userRoutine, offset, size, dataPtr, refCon) InvokeGetMovieUPP(offset, size, dataPtr, refCon, userRoutine)
#define CallMoviePreviewCallOutProc(userRoutine, refcon)        InvokeMoviePreviewCallOutUPP(refcon, userRoutine)
#define CallTextMediaProc(userRoutine, theText, theMovie, displayFlag, refcon) InvokeTextMediaUPP(theText, theMovie, displayFlag, refcon, userRoutine)
#define CallActionsProc(userRoutine, refcon, targetTrack, targetRefCon, theEvent) InvokeActionsUPP(refcon, targetTrack, targetRefCon, theEvent, userRoutine)
#define CallDoMCActionProc(userRoutine, refcon, action, params, handled) InvokeDoMCActionUPP(refcon, action, params, handled, userRoutine)
#define CallMovieExecuteWiredActionsProc(userRoutine, theMovie, refcon, flags, wiredActions) InvokeMovieExecuteWiredActionsUPP(theMovie, refcon, flags, wiredActions, userRoutine)
#define CallMoviePrePrerollCompleteProc(userRoutine, theMovie, prerollErr, refcon) InvokeMoviePrePrerollCompleteUPP(theMovie, prerollErr, refcon, userRoutine)
#define CallQTNextTaskNeededSoonerCallbackProc(userRoutine, duration, flags, refcon) InvokeQTNextTaskNeededSoonerCallbackUPP(duration, flags, refcon, userRoutine)
#define CallMoviesErrorProc(userRoutine, theErr, refcon)        InvokeMoviesErrorUPP(theErr, refcon, userRoutine)
#define CallTweenerDataProc(userRoutine, tr, tweenData, tweenDataSize, dataDescriptionSeed, dataDescription, asyncCompletionProc, transferProc, refCon) InvokeTweenerDataUPP(tr, tweenData, tweenDataSize, dataDescriptionSeed, dataDescription, asyncCompletionProc, transferProc, refCon, userRoutine)
#define CallQTEffectListFilterProc(userRoutine, effect, effectMinSource, effectMaxSource, majorClass, minorClass, refcon) InvokeQTEffectListFilterUPP(effect, effectMinSource, effectMaxSource, majorClass, minorClass, refcon, userRoutine)
#define CallQTBandwidthNotificationProc(userRoutine, flags, reserved, refcon) InvokeQTBandwidthNotificationUPP(flags, reserved, refcon, userRoutine)
/*****
    QT International Text Atom Support
*****/
enum {
    kITextRemoveEverythingBut   = 0 << 1,
    kITextRemoveLeaveSuggestedAlternate = 1 << 1
};

enum {
    kITextAtomType              = FOUR_CHAR_CODE('itxt'),
    kITextStringAtomType        = FOUR_CHAR_CODE('text')
};

EXTERN_API( OSErr )
ITextAddString                  (QTAtomContainer        container,
                                 QTAtom                 parentAtom,
                                 RegionCode             theRegionCode,
                                 ConstStr255Param       theString)                          THREEWORDINLINE(0x303C, 0x027A, 0xAAAA);

EXTERN_API( OSErr )
ITextRemoveString               (QTAtomContainer        container,
                                 QTAtom                 parentAtom,
                                 RegionCode             theRegionCode,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x027B, 0xAAAA);

EXTERN_API( OSErr )
ITextGetString                  (QTAtomContainer        container,
                                 QTAtom                 parentAtom,
                                 RegionCode             requestedRegion,
                                 RegionCode *           foundRegion,
                                 StringPtr              theString)                          THREEWORDINLINE(0x303C, 0x027C, 0xAAAA);

EXTERN_API( OSErr )
QTTextToNativeText              (Handle                 theText,
                                 long                   encoding,
                                 long                   flags)                              THREEWORDINLINE(0x303C, 0x02DB, 0xAAAA);

/* QTParseTextHREF inContainer atoms*/
enum {
    kQTParseTextHREFText        = FOUR_CHAR_CODE('text'),       /* string*/
    kQTParseTextHREFBaseURL     = FOUR_CHAR_CODE('burl'),       /* string*/
    kQTParseTextHREFClickPoint  = FOUR_CHAR_CODE('clik'),       /* Point; if present, QTParseTextHREF will expand URLs to support server-side image maps*/
    kQTParseTextHREFUseAltDelim = FOUR_CHAR_CODE('altd'),       /* boolean; if no kQTParseTextHREFDelimiter, delim is ':'*/
    kQTParseTextHREFDelimiter   = FOUR_CHAR_CODE('delm'),       /* character*/
    kQTParseTextHREFRecomposeHREF = FOUR_CHAR_CODE('rhrf')      /* Boolean; if true, QTParseTextHREF returns recomposed HREF with URL expanded as appropriate*/
};

/* QTParseTextHREF outContainer atoms*/
enum {
    kQTParseTextHREFURL         = FOUR_CHAR_CODE('url '),       /* string*/
    kQTParseTextHREFTarget      = FOUR_CHAR_CODE('targ'),       /* string*/
    kQTParseTextHREFChapter     = FOUR_CHAR_CODE('chap'),       /* string*/
    kQTParseTextHREFIsAutoHREF  = FOUR_CHAR_CODE('auto'),       /* Boolean*/
    kQTParseTextHREFIsServerMap = FOUR_CHAR_CODE('smap'),       /* Boolean*/
    kQTParseTextHREFHREF        = FOUR_CHAR_CODE('href'),       /* string; recomposed HREF with URL expanded as appropriate, suitable for mcActionLinkToURL*/
    kQTParseTextHREFEMBEDArgs   = FOUR_CHAR_CODE('mbed')        /* string; text between 'E<' and '>' to be used as new movie's embed tags*/
};

EXTERN_API( OSErr )
QTParseTextHREF                 (char *                 href,
                                 SInt32                 hrefLen,
                                 QTAtomContainer        inContainer,
                                 QTAtomContainer *      outContainer)                       THREEWORDINLINE(0x303C, 0x0319, 0xAAAA);

/*************************
* track reference types
**************************/
enum {
    kTrackReferenceChapterList  = FOUR_CHAR_CODE('chap'),
    kTrackReferenceTimeCode     = FOUR_CHAR_CODE('tmcd'),
    kTrackReferenceModifier     = FOUR_CHAR_CODE('ssrc')
};

/*************************
* modifier track types
**************************/
enum {
    kTrackModifierInput         = 0x696E,                       /* is really 'in'*/
    kTrackModifierType          = 0x7479,                       /* is really 'ty'*/
    kTrackModifierReference     = FOUR_CHAR_CODE('ssrc'),
    kTrackModifierObjectID      = FOUR_CHAR_CODE('obid'),
    kTrackModifierInputName     = FOUR_CHAR_CODE('name')
};

enum {
    kInputMapSubInputID         = FOUR_CHAR_CODE('subi')
};

enum {
    kTrackModifierTypeMatrix    = 1,
    kTrackModifierTypeClip      = 2,
    kTrackModifierTypeGraphicsMode = 5,
    kTrackModifierTypeVolume    = 3,
    kTrackModifierTypeBalance   = 4,
    kTrackModifierTypeImage     = FOUR_CHAR_CODE('vide'),       /* was kTrackModifierTypeSpriteImage*/
    kTrackModifierObjectMatrix  = 6,
    kTrackModifierObjectGraphicsMode = 7,
    kTrackModifierType3d4x4Matrix = 8,
    kTrackModifierCameraData    = 9,
    kTrackModifierSoundLocalizationData = 10,
    kTrackModifierObjectImageIndex = 11,
    kTrackModifierObjectLayer   = 12,
    kTrackModifierObjectVisible = 13,
    kTrackModifierAngleAspectCamera = 14,
    kTrackModifierPanAngle      = FOUR_CHAR_CODE('pan '),
    kTrackModifierTiltAngle     = FOUR_CHAR_CODE('tilt'),
    kTrackModifierVerticalFieldOfViewAngle = FOUR_CHAR_CODE('fov '),
    kTrackModifierObjectQTEventSend = FOUR_CHAR_CODE('evnt'),
    kTrackModifierObjectCanBeHitTested = 15
};


struct ModifierTrackGraphicsModeRecord {
    long                            graphicsMode;
    RGBColor                        opColor;
};
typedef struct ModifierTrackGraphicsModeRecord ModifierTrackGraphicsModeRecord;

/*************************
* tween track types
**************************/
enum {
    kTweenTypeShort             = 1,
    kTweenTypeLong              = 2,
    kTweenTypeFixed             = 3,
    kTweenTypePoint             = 4,
    kTweenTypeQDRect            = 5,
    kTweenTypeQDRegion          = 6,
    kTweenTypeMatrix            = 7,
    kTweenTypeRGBColor          = 8,
    kTweenTypeGraphicsModeWithRGBColor = 9,
    kTweenTypeQTFloatSingle     = 10,
    kTweenTypeQTFloatDouble     = 11,
    kTweenTypeFixedPoint        = 12,
    kTweenType3dScale           = FOUR_CHAR_CODE('3sca'),
    kTweenType3dTranslate       = FOUR_CHAR_CODE('3tra'),
    kTweenType3dRotate          = FOUR_CHAR_CODE('3rot'),
    kTweenType3dRotateAboutPoint = FOUR_CHAR_CODE('3rap'),
    kTweenType3dRotateAboutAxis = FOUR_CHAR_CODE('3rax'),
    kTweenType3dRotateAboutVector = FOUR_CHAR_CODE('3rvc'),
    kTweenType3dQuaternion      = FOUR_CHAR_CODE('3qua'),
    kTweenType3dMatrix          = FOUR_CHAR_CODE('3mat'),
    kTweenType3dCameraData      = FOUR_CHAR_CODE('3cam'),
    kTweenType3dAngleAspectCameraData = FOUR_CHAR_CODE('3caa'),
    kTweenType3dSoundLocalizationData = FOUR_CHAR_CODE('3slc'),
    kTweenTypePathToMatrixTranslation = FOUR_CHAR_CODE('gxmt'),
    kTweenTypePathToMatrixRotation = FOUR_CHAR_CODE('gxpr'),
    kTweenTypePathToMatrixTranslationAndRotation = FOUR_CHAR_CODE('gxmr'),
    kTweenTypePathToFixedPoint  = FOUR_CHAR_CODE('gxfp'),
    kTweenTypePathXtoY          = FOUR_CHAR_CODE('gxxy'),
    kTweenTypePathYtoX          = FOUR_CHAR_CODE('gxyx'),
    kTweenTypeAtomList          = FOUR_CHAR_CODE('atom'),
    kTweenTypePolygon           = FOUR_CHAR_CODE('poly'),
    kTweenTypeMultiMatrix       = FOUR_CHAR_CODE('mulm'),
    kTweenTypeSpin              = FOUR_CHAR_CODE('spin'),
    kTweenType3dMatrixNonLinear = FOUR_CHAR_CODE('3nlr'),
    kTweenType3dVRObject        = FOUR_CHAR_CODE('3vro')
};

enum {
    kTweenEntry                 = FOUR_CHAR_CODE('twen'),
    kTweenData                  = FOUR_CHAR_CODE('data'),
    kTweenType                  = FOUR_CHAR_CODE('twnt'),
    kTweenStartOffset           = FOUR_CHAR_CODE('twst'),
    kTweenDuration              = FOUR_CHAR_CODE('twdu'),
    kTweenFlags                 = FOUR_CHAR_CODE('flag'),
    kTweenOutputMin             = FOUR_CHAR_CODE('omin'),
    kTweenOutputMax             = FOUR_CHAR_CODE('omax'),
    kTweenSequenceElement       = FOUR_CHAR_CODE('seqe'),
    kTween3dInitialCondition    = FOUR_CHAR_CODE('icnd'),
    kTweenInterpolationID       = FOUR_CHAR_CODE('intr'),
    kTweenRegionData            = FOUR_CHAR_CODE('qdrg'),
    kTweenPictureData           = FOUR_CHAR_CODE('PICT'),
    kListElementType            = FOUR_CHAR_CODE('type'),
    kListElementDataType        = FOUR_CHAR_CODE('daty'),
    kNameAtom                   = FOUR_CHAR_CODE('name'),
    kInitialRotationAtom        = FOUR_CHAR_CODE('inro'),
    kNonLinearTweenHeader       = FOUR_CHAR_CODE('nlth')
};

/* kTweenFlags*/
enum {
    kTweenReturnDelta           = 1L << 0
};


struct TweenSequenceEntryRecord {
    Fixed                           endPercent;
    QTAtomID                        tweenAtomID;
    QTAtomID                        dataAtomID;
};
typedef struct TweenSequenceEntryRecord TweenSequenceEntryRecord;

#ifdef __QD3D__

struct ThreeDeeVRObjectSample {
  long            rows;  
    long            columns;
   TQ3Matrix4x4    calib1;
    TQ3Matrix4x4    calib2;
    long            reserved1;
 long            reserved2;
};

typedef struct ThreeDeeVRObjectSample ThreeDeeVRObjectSample;

struct ThreeDeeNonLinearSample {
 float           DurFromLastSample;  /* 0 to 1 */
   TQ3Matrix4x4    matrix;
};
typedef struct ThreeDeeNonLinearSample ThreeDeeNonLinearSample;

struct ThreeDeeNonLinearTweenHeaderAtom {
  long    number;
    long    dataSize;
  float   tensionFactor;  /* default is 0 */
 long    reserved1;
 long    reserved2;
};
typedef struct ThreeDeeNonLinearTweenHeaderAtom ThreeDeeNonLinearTweenHeaderAtom;


#endif



#if OLDROUTINENAMES

/*************************
* Video Media routines
**************************/

#define GetVideoMediaGraphicsMode      MediaGetGraphicsMode
#define SetVideoMediaGraphicsMode      MediaSetGraphicsMode

/* use these two routines at your own peril */
#define ResetVideoMediaStatistics      VideoMediaResetStatistics
#define GetVideoMediaStatistics           VideoMediaGetStatistics

/*************************
* Sound Media routines
**************************/

#define GetSoundMediaBalance            MediaGetSoundBalance
#define SetSoundMediaBalance           MediaSetSoundBalance

/*************************
* Text Media routines
**************************/

#define SetTextProc         TextMediaSetTextProc
#define AddTextSample      TextMediaAddTextSample
#define AddTESample          TextMediaAddTESample
#define AddHiliteSample        TextMediaAddHiliteSample
#define FindNextText       TextMediaFindNextText
#define HiliteTextSample  TextMediaHiliteTextSample
#define SetTextSampleData TextMediaSetTextSampleData
#define DrawRaw              TextMediaDrawRaw
#define RawSetup           TextMediaRawSetup
#define RawIdle               TextMediaRawIdle
#define SetTextProperty        TextMediaSetTextProperty

/*************************
* Sprite Media routines
**************************/

#define SetSpriteMediaSpriteProperty  SpriteMediaSetProperty
#define GetSpriteMediaSpriteProperty SpriteMediaGetProperty
#define HitTestSpriteMedia               SpriteMediaHitTestSprites
#define CountSpriteMediaSprites           SpriteMediaCountSprites
#define CountSpriteMediaImages          SpriteMediaCountImages
#define GetSpriteMediaIndImageDescription    SpriteMediaGetIndImageDescription
#define GetDisplayedSampleNumber      SpriteMediaGetDisplayedSampleNumber
#endif /* OLDROUTINENAMES */






/*****
    Content Restrictions
*****/
enum {
    kQTRestrictionClassSave     = FOUR_CHAR_CODE('save'),
    kQTRestrictionSaveDontAddMovieResource = (1L << 0),
    kQTRestrictionSaveDontFlatten = (1L << 1),
    kQTRestrictionSaveDontExport = (1L << 2),
    kQTRestrictionSaveDontExtract = (1L << 3),                  /* don't allow any form of extraction of content*/
    kQTRestrictionClassEdit     = FOUR_CHAR_CODE('edit'),
    kQTRestrictionEditDontCopy  = (1L << 0),                    /* disable copy */
    kQTRestrictionEditDontCut   = (1L << 1),                    /* disable cut */
    kQTRestrictionEditDontPaste = (1L << 2),                    /* disable paste */
    kQTRestrictionEditDontClear = (1L << 3),                    /* disable clear*/
    kQTRestrictionEditDontModify = (1L << 4),                   /* don't allow modification of content*/
    kQTRestrictionEditDontExtract = (1L << 5)                   /* don't allow any form of extraction of content*/
};



/*************************
* Video Media routines
**************************/


enum {
    videoFlagDontLeanAhead      = 1L << 0
};




/* use these five routines at your own peril*/
EXTERN_API( ComponentResult )
VideoMediaResetStatistics       (MediaHandler           mh)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0105, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
VideoMediaGetStatistics         (MediaHandler           mh)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0106, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
VideoMediaGetStallCount         (MediaHandler           mh,
                                 unsigned long *        stalls)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x010E, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
VideoMediaSetCodecParameter     (MediaHandler           mh,
                                 CodecType              cType,
                                 OSType                 parameterID,
                                 long                   parameterChangeSeed,
                                 void *                 dataPtr,
                                 long                   dataSize)                           FIVEWORDINLINE(0x2F3C, 0x0014, 0x010F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
VideoMediaGetCodecParameter     (MediaHandler           mh,
                                 CodecType              cType,
                                 OSType                 parameterID,
                                 Handle                 outParameterData)                   FIVEWORDINLINE(0x2F3C, 0x000C, 0x0110, 0x7000, 0xA82A);




/*************************
* Text Media routines
**************************/



/* Return displayFlags for TextProc */
enum {
    txtProcDefaultDisplay       = 0,                            /*    Use the media's default*/
    txtProcDontDisplay          = 1,                            /*    Don't display the text*/
    txtProcDoDisplay            = 2                             /*    Do display the text*/
};

EXTERN_API( ComponentResult )
TextMediaSetTextProc            (MediaHandler           mh,
                                 TextMediaUPP           TextProc,
                                 long                   refcon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextMediaAddTextSample          (MediaHandler           mh,
                                 Ptr                    text,
                                 unsigned long          size,
                                 short                  fontNumber,
                                 short                  fontSize,
                                 Style                  textFace,
                                 RGBColor *             textColor,
                                 RGBColor *             backColor,
                                 short                  textJustification,
                                 Rect *                 textBox,
                                 long                   displayFlags,
                                 TimeValue              scrollDelay,
                                 short                  hiliteStart,
                                 short                  hiliteEnd,
                                 RGBColor *             rgbHiliteColor,
                                 TimeValue              duration,
                                 TimeValue *            sampleTime)                         FIVEWORDINLINE(0x2F3C, 0x0034, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextMediaAddTESample            (MediaHandler           mh,
                                 TEHandle               hTE,
                                 RGBColor *             backColor,
                                 short                  textJustification,
                                 Rect *                 textBox,
                                 long                   displayFlags,
                                 TimeValue              scrollDelay,
                                 short                  hiliteStart,
                                 short                  hiliteEnd,
                                 RGBColor *             rgbHiliteColor,
                                 TimeValue              duration,
                                 TimeValue *            sampleTime)                         FIVEWORDINLINE(0x2F3C, 0x0026, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextMediaAddHiliteSample        (MediaHandler           mh,
                                 short                  hiliteStart,
                                 short                  hiliteEnd,
                                 RGBColor *             rgbHiliteColor,
                                 TimeValue              duration,
                                 TimeValue *            sampleTime)                         FIVEWORDINLINE(0x2F3C, 0x0010, 0x0104, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextMediaDrawRaw                (MediaHandler           mh,
                                 GWorldPtr              gw,
                                 GDHandle               gd,
                                 void *                 data,
                                 long                   dataSize,
                                 TextDescriptionHandle  tdh)                                FIVEWORDINLINE(0x2F3C, 0x0014, 0x0109, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextMediaSetTextProperty        (MediaHandler           mh,
                                 TimeValue              atMediaTime,
                                 long                   propertyType,
                                 void *                 data,
                                 long                   dataSize)                           FIVEWORDINLINE(0x2F3C, 0x0010, 0x010A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextMediaRawSetup               (MediaHandler           mh,
                                 GWorldPtr              gw,
                                 GDHandle               gd,
                                 void *                 data,
                                 long                   dataSize,
                                 TextDescriptionHandle  tdh,
                                 TimeValue              sampleDuration)                     FIVEWORDINLINE(0x2F3C, 0x0018, 0x010B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextMediaRawIdle                (MediaHandler           mh,
                                 GWorldPtr              gw,
                                 GDHandle               gd,
                                 TimeValue              sampleTime,
                                 long                   flagsIn,
                                 long *                 flagsOut)                           FIVEWORDINLINE(0x2F3C, 0x0014, 0x010C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextMediaGetTextProperty        (MediaHandler           mh,
                                 TimeValue              atMediaTime,
                                 long                   propertyType,
                                 void *                 data,
                                 long                   dataSize)                           FIVEWORDINLINE(0x2F3C, 0x0010, 0x010D, 0x7000, 0xA82A);

enum {
    findTextEdgeOK              = 1 << 0,                       /* Okay to find text at specified sample time*/
    findTextCaseSensitive       = 1 << 1,                       /* Case sensitive search*/
    findTextReverseSearch       = 1 << 2,                       /* Search from sampleTime backwards*/
    findTextWrapAround          = 1 << 3,                       /* Wrap search when beginning or end of movie is hit*/
    findTextUseOffset           = 1 << 4                        /* Begin search at the given character offset into sample rather than edge*/
};

EXTERN_API( ComponentResult )
TextMediaFindNextText           (MediaHandler           mh,
                                 Ptr                    text,
                                 long                   size,
                                 short                  findFlags,
                                 TimeValue              startTime,
                                 TimeValue *            foundTime,
                                 TimeValue *            foundDuration,
                                 long *                 offset)                             FIVEWORDINLINE(0x2F3C, 0x001A, 0x0105, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TextMediaHiliteTextSample       (MediaHandler           mh,
                                 TimeValue              sampleTime,
                                 short                  hiliteStart,
                                 short                  hiliteEnd,
                                 RGBColor *             rgbHiliteColor)                     FIVEWORDINLINE(0x2F3C, 0x000C, 0x0106, 0x7000, 0xA82A);

enum {
    dropShadowOffsetType        = FOUR_CHAR_CODE('drpo'),
    dropShadowTranslucencyType  = FOUR_CHAR_CODE('drpt')
};

EXTERN_API( ComponentResult )
TextMediaSetTextSampleData      (MediaHandler           mh,
                                 void *                 data,
                                 OSType                 dataType)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0107, 0x7000, 0xA82A);





/*************************
* Sprite Media routines
**************************/
/* flags for sprite hit test routines */
enum {
    spriteHitTestBounds         = 1L << 0,                      /*    point must only be within sprite's bounding box*/
    spriteHitTestImage          = 1L << 1,                      /*  point must be within the shape of the sprite's image*/
    spriteHitTestInvisibleSprites = 1L << 2,                    /*  invisible sprites may be hit tested*/
    spriteHitTestIsClick        = 1L << 3,                      /*  for codecs that want mouse events*/
    spriteHitTestLocInDisplayCoordinates = 1L << 4,             /*    set if you want to pass a display coordiate point to SpriteHitTest*/
    spriteHitTestTreatAllSpritesAsHitTestable = 1L << 5         /* set if you want to override each sprites hittestable property as true*/
};

/* atom types for sprite media */
enum {
    kSpriteAtomType             = FOUR_CHAR_CODE('sprt'),
    kSpriteImagesContainerAtomType = FOUR_CHAR_CODE('imct'),
    kSpriteImageAtomType        = FOUR_CHAR_CODE('imag'),
    kSpriteImageDataAtomType    = FOUR_CHAR_CODE('imda'),
    kSpriteImageDataRefAtomType = FOUR_CHAR_CODE('imre'),
    kSpriteImageDataRefTypeAtomType = FOUR_CHAR_CODE('imrt'),
    kSpriteImageGroupIDAtomType = FOUR_CHAR_CODE('imgr'),
    kSpriteImageRegistrationAtomType = FOUR_CHAR_CODE('imrg'),
    kSpriteImageDefaultImageIndexAtomType = FOUR_CHAR_CODE('defi'),
    kSpriteSharedDataAtomType   = FOUR_CHAR_CODE('dflt'),
    kSpriteNameAtomType         = FOUR_CHAR_CODE('name'),
    kSpriteImageNameAtomType    = FOUR_CHAR_CODE('name'),
    kSpriteUsesImageIDsAtomType = FOUR_CHAR_CODE('uses'),       /* leaf data is an array of QTAtomID's, one per image used*/
    kSpriteBehaviorsAtomType    = FOUR_CHAR_CODE('beha'),
    kSpriteImageBehaviorAtomType = FOUR_CHAR_CODE('imag'),
    kSpriteCursorBehaviorAtomType = FOUR_CHAR_CODE('crsr'),
    kSpriteStatusStringsBehaviorAtomType = FOUR_CHAR_CODE('sstr'),
    kSpriteVariablesContainerAtomType = FOUR_CHAR_CODE('vars'),
    kSpriteStringVariableAtomType = FOUR_CHAR_CODE('strv'),
    kSpriteFloatingPointVariableAtomType = FOUR_CHAR_CODE('flov')
};


struct QTRuntimeSpriteDescStruct {
    long                            version;                    /* set to zero*/
    QTAtomID                        spriteID;
    short                           imageIndex;
    MatrixRecord                    matrix;
    short                           visible;
    short                           layer;
    ModifierTrackGraphicsModeRecord  graphicsMode;
    QTAtomID                        actionHandlingSpriteID;
};
typedef struct QTRuntimeSpriteDescStruct QTRuntimeSpriteDescStruct;

typedef QTRuntimeSpriteDescStruct *     QTRuntimeSpriteDescPtr;
/*
   when filling in QTSpriteButtonBehaviorStruct values -1 may be used to indicate that
   the state transition does not change the property
*/

struct QTSpriteButtonBehaviorStruct {
    QTAtomID                        notOverNotPressedStateID;
    QTAtomID                        overNotPressedStateID;
    QTAtomID                        overPressedStateID;
    QTAtomID                        notOverPressedStateID;
};
typedef struct QTSpriteButtonBehaviorStruct QTSpriteButtonBehaviorStruct;

typedef QTSpriteButtonBehaviorStruct *  QTSpriteButtonBehaviorPtr;
EXTERN_API( ComponentResult )
SpriteMediaSetProperty          (MediaHandler           mh,
                                 short                  spriteIndex,
                                 long                   propertyType,
                                 void *                 propertyValue)                      FIVEWORDINLINE(0x2F3C, 0x000A, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaGetProperty          (MediaHandler           mh,
                                 short                  spriteIndex,
                                 long                   propertyType,
                                 void *                 propertyValue)                      FIVEWORDINLINE(0x2F3C, 0x000A, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaHitTestSprites       (MediaHandler           mh,
                                 long                   flags,
                                 Point                  loc,
                                 short *                spriteHitIndex)                     FIVEWORDINLINE(0x2F3C, 0x000C, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaCountSprites         (MediaHandler           mh,
                                 short *                numSprites)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0104, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaCountImages          (MediaHandler           mh,
                                 short *                numImages)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0105, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaGetIndImageDescription (MediaHandler         mh,
                                 short                  imageIndex,
                                 ImageDescriptionHandle  imageDescription)                  FIVEWORDINLINE(0x2F3C, 0x0006, 0x0106, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaGetDisplayedSampleNumber (MediaHandler       mh,
                                 long *                 sampleNum)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0107, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaGetSpriteName        (MediaHandler           mh,
                                 QTAtomID               spriteID,
                                 Str255                 spriteName)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0108, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaGetImageName         (MediaHandler           mh,
                                 short                  imageIndex,
                                 Str255                 imageName)                          FIVEWORDINLINE(0x2F3C, 0x0006, 0x0109, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaSetSpriteProperty    (MediaHandler           mh,
                                 QTAtomID               spriteID,
                                 long                   propertyType,
                                 void *                 propertyValue)                      FIVEWORDINLINE(0x2F3C, 0x000C, 0x010A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaGetSpriteProperty    (MediaHandler           mh,
                                 QTAtomID               spriteID,
                                 long                   propertyType,
                                 void *                 propertyValue)                      FIVEWORDINLINE(0x2F3C, 0x000C, 0x010B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaHitTestAllSprites    (MediaHandler           mh,
                                 long                   flags,
                                 Point                  loc,
                                 QTAtomID *             spriteHitID)                        FIVEWORDINLINE(0x2F3C, 0x000C, 0x010C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaHitTestOneSprite     (MediaHandler           mh,
                                 QTAtomID               spriteID,
                                 long                   flags,
                                 Point                  loc,
                                 Boolean *              wasHit)                             FIVEWORDINLINE(0x2F3C, 0x0010, 0x010D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaSpriteIndexToID      (MediaHandler           mh,
                                 short                  spriteIndex,
                                 QTAtomID *             spriteID)                           FIVEWORDINLINE(0x2F3C, 0x0006, 0x010E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaSpriteIDToIndex      (MediaHandler           mh,
                                 QTAtomID               spriteID,
                                 short *                spriteIndex)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x010F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaGetSpriteActionsForQTEvent (MediaHandler     mh,
                                 QTEventRecordPtr       event,
                                 QTAtomID               spriteID,
                                 QTAtomContainer *      container,
                                 QTAtom *               atom)                               FIVEWORDINLINE(0x2F3C, 0x0010, 0x0110, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaSetActionVariable    (MediaHandler           mh,
                                 QTAtomID               variableID,
                                 const float *          value)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0111, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaGetActionVariable    (MediaHandler           mh,
                                 QTAtomID               variableID,
                                 float *                value)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0112, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaGetIndImageProperty  (MediaHandler           mh,
                                 short                  imageIndex,
                                 long                   imagePropertyType,
                                 void *                 imagePropertyValue)                 FIVEWORDINLINE(0x2F3C, 0x000A, 0x0113, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaNewSprite            (MediaHandler           mh,
                                 QTRuntimeSpriteDescPtr  newSpriteDesc)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x0114, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaDisposeSprite        (MediaHandler           mh,
                                 QTAtomID               spriteID)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0115, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaSetActionVariableToString (MediaHandler      mh,
                                 QTAtomID               variableID,
                                 Ptr                    theCString)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0116, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaGetActionVariableAsString (MediaHandler      mh,
                                 QTAtomID               variableID,
                                 Handle *               theCString)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0117, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
SpriteMediaNewImage             (MediaHandler           mh,
                                 Handle                 dataRef,
                                 OSType                 dataRefType,
                                 QTAtomID               desiredID)                          FIVEWORDINLINE(0x2F3C, 0x000C, 0x011B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaDisposeImage         (MediaHandler           mh,
                                 short                  imageIndex)                         FIVEWORDINLINE(0x2F3C, 0x0002, 0x011C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaImageIndexToID       (MediaHandler           mh,
                                 short                  imageIndex,
                                 QTAtomID *             imageID)                            FIVEWORDINLINE(0x2F3C, 0x0006, 0x011D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
SpriteMediaImageIDToIndex       (MediaHandler           mh,
                                 QTAtomID               imageID,
                                 short *                imageIndex)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x011E, 0x7000, 0xA82A);


/*************************
* Flash Media routines
**************************/

EXTERN_API( ComponentResult )
FlashMediaSetPan                (MediaHandler           mh,
                                 short                  xPercent,
                                 short                  yPercent)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaSetZoom               (MediaHandler           mh,
                                 short                  factor)                             FIVEWORDINLINE(0x2F3C, 0x0002, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaSetZoomRect           (MediaHandler           mh,
                                 long                   left,
                                 long                   top,
                                 long                   right,
                                 long                   bottom)                             FIVEWORDINLINE(0x2F3C, 0x0010, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaGetRefConBounds       (MediaHandler           mh,
                                 long                   refCon,
                                 long *                 left,
                                 long *                 top,
                                 long *                 right,
                                 long *                 bottom)                             FIVEWORDINLINE(0x2F3C, 0x0014, 0x0104, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaGetRefConID           (MediaHandler           mh,
                                 long                   refCon,
                                 long *                 refConID)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0105, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaIDToRefCon            (MediaHandler           mh,
                                 long                   refConID,
                                 long *                 refCon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0106, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaGetDisplayedFrameNumber (MediaHandler         mh,
                                 long *                 flashFrameNumber)                   FIVEWORDINLINE(0x2F3C, 0x0004, 0x0107, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaFrameNumberToMovieTime (MediaHandler          mh,
                                 long                   flashFrameNumber,
                                 TimeValue *            movieTime)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0108, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaFrameLabelToMovieTime (MediaHandler           mh,
                                 Ptr                    theLabel,
                                 TimeValue *            movieTime)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0109, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaGetFlashVariable      (MediaHandler           mh,
                                 char *                 path,
                                 char *                 name,
                                 Handle *               theVariableCStringOut)              FIVEWORDINLINE(0x2F3C, 0x000C, 0x010A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaSetFlashVariable      (MediaHandler           mh,
                                 char *                 path,
                                 char *                 name,
                                 char *                 value,
                                 Boolean                updateFocus)                        FIVEWORDINLINE(0x2F3C, 0x000E, 0x010B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaDoButtonActions       (MediaHandler           mh,
                                 char *                 path,
                                 long                   buttonID,
                                 long                   transition)                         FIVEWORDINLINE(0x2F3C, 0x000C, 0x010C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
FlashMediaGetSupportedSwfVersion (MediaHandler          mh,
                                 unsigned char *        swfVersion)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x010D, 0x7000, 0xA82A);



/* sample format atoms*/
enum {
    kMovieMediaDataReference    = FOUR_CHAR_CODE('mmdr'),       /* data reference*/
    kMovieMediaDefaultDataReferenceID = FOUR_CHAR_CODE('ddri'), /* atom id*/
    kMovieMediaSlaveTime        = FOUR_CHAR_CODE('slti'),       /* boolean*/
    kMovieMediaSlaveAudio       = FOUR_CHAR_CODE('slau'),       /* boolean*/
    kMovieMediaSlaveGraphicsMode = FOUR_CHAR_CODE('slgr'),      /* boolean*/
    kMovieMediaAutoPlay         = FOUR_CHAR_CODE('play'),       /* boolean*/
    kMovieMediaLoop             = FOUR_CHAR_CODE('loop'),       /* UInt8 (0=no loop, 1=loop, 2=palindrome loop)*/
    kMovieMediaUseMIMEType      = FOUR_CHAR_CODE('mime'),       /* string indicating the MIME type to use for the dataref (usually not required)*/
    kMovieMediaTitle            = FOUR_CHAR_CODE('titl'),       /* string of the media's title (tooltips)*/
    kMovieMediaAltText          = FOUR_CHAR_CODE('altt'),       /* string of alternate text if media isn't loaded*/
    kMovieMediaClipBegin        = FOUR_CHAR_CODE('clpb'),       /* MovieMediaTimeRecord of start time of embedded media*/
    kMovieMediaClipDuration     = FOUR_CHAR_CODE('clpd'),       /* MovieMediaTimeRecord of duration of embedded media*/
    kMovieMediaRegionAtom       = FOUR_CHAR_CODE('regi'),       /* contains subatoms that describe layout*/
    kMovieMediaSlaveTrackDuration = FOUR_CHAR_CODE('sltr'),     /* Boolean indicating that media handler should adjust track and media based on actual embedded movie duration*/
    kMovieMediaEnableFrameStepping = FOUR_CHAR_CODE('enfs'),    /* boolean. if true stepping on external movie steps frames within embedded movie.*/
    kMovieMediaBackgroundColor  = FOUR_CHAR_CODE('bkcl'),       /* RGBColor.*/
    kMovieMediaPrerollTime      = FOUR_CHAR_CODE('prer')        /* SInt32 indicating preroll time*/
};

/* fit types*/
enum {
    kMovieMediaFitNone          = 0,
    kMovieMediaFitScroll        = FOUR_CHAR_CODE('scro'),
    kMovieMediaFitClipIfNecessary = FOUR_CHAR_CODE('hidd'),
    kMovieMediaFitFill          = FOUR_CHAR_CODE('fill'),
    kMovieMediaFitMeet          = FOUR_CHAR_CODE('meet'),
    kMovieMediaFitSlice         = FOUR_CHAR_CODE('slic')
};

/* sub atoms for region atom*/
enum {
    kMovieMediaSpatialAdjustment = FOUR_CHAR_CODE('fit '),      /* OSType from kMovieMediaFit**/
    kMovieMediaRectangleAtom    = FOUR_CHAR_CODE('rect'),
    kMovieMediaTop              = FOUR_CHAR_CODE('top '),
    kMovieMediaLeft             = FOUR_CHAR_CODE('left'),
    kMovieMediaWidth            = FOUR_CHAR_CODE('wd  '),
    kMovieMediaHeight           = FOUR_CHAR_CODE('ht  ')
};

/* contained movie properties*/
enum {
    kMoviePropertyDuration      = FOUR_CHAR_CODE('dura'),       /* TimeValue **/
    kMoviePropertyTimeScale     = FOUR_CHAR_CODE('tims'),       /* TimeValue **/
    kMoviePropertyTime          = FOUR_CHAR_CODE('timv'),       /* TimeValue **/
    kMoviePropertyNaturalBounds = FOUR_CHAR_CODE('natb'),       /* Rect **/
    kMoviePropertyMatrix        = FOUR_CHAR_CODE('mtrx'),       /* Matrix **/
    kMoviePropertyTrackList     = FOUR_CHAR_CODE('tlst')        /* long ****/
};


enum {
    kTrackPropertyMediaType     = FOUR_CHAR_CODE('mtyp'),       /* OSType*/
    kTrackPropertyInstantiation = FOUR_CHAR_CODE('inst')        /* MovieMediaInstantiationInfoRecord*/
};


struct MovieMediaTimeRecord {
    wide                            time;
    TimeScale                       scale;
};
typedef struct MovieMediaTimeRecord     MovieMediaTimeRecord;

struct MovieMediaInstantiationInfoRecord {
    Boolean                         immediately;
    Boolean                         pad;
    SInt32                          bitRate;
};
typedef struct MovieMediaInstantiationInfoRecord MovieMediaInstantiationInfoRecord;
/*************************
* Movie Media routines
**************************/


EXTERN_API( ComponentResult )
MovieMediaGetChildDoMCActionCallback (MediaHandler      mh,
                                 DoMCActionUPP *        doMCActionCallbackProc,
                                 long *                 refcon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieMediaGetDoMCActionCallback (MediaHandler           mh,
                                 DoMCActionUPP *        doMCActionCallbackProc,
                                 long *                 refcon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieMediaGetCurrentMovieProperty (MediaHandler         mh,
                                 OSType                 whichProperty,
                                 void *                 value)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0104, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieMediaGetCurrentTrackProperty (MediaHandler         mh,
                                 long                   trackID,
                                 OSType                 whichProperty,
                                 void *                 value)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0105, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieMediaGetChildMovieDataReference (MediaHandler      mh,
                                 QTAtomID               dataRefID,
                                 short                  dataRefIndex,
                                 OSType *               dataRefType,
                                 Handle *               dataRef,
                                 QTAtomID *             dataRefIDOut,
                                 short *                dataRefIndexOut)                    FIVEWORDINLINE(0x2F3C, 0x0016, 0x0106, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieMediaSetChildMovieDataReference (MediaHandler      mh,
                                 QTAtomID               dataRefID,
                                 OSType                 dataRefType,
                                 Handle                 dataRef)                            FIVEWORDINLINE(0x2F3C, 0x000C, 0x0107, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieMediaLoadChildMovieFromDataReference (MediaHandler  mh,
                                 QTAtomID               dataRefID)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0108, 0x7000, 0xA82A);



/*************************
* 3D Media routines
**************************/
EXTERN_API( ComponentResult )
Media3DGetNamedObjectList       (MediaHandler           mh,
                                 QTAtomContainer *      objectList)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
Media3DGetRendererList          (MediaHandler           mh,
                                 QTAtomContainer *      rendererList)                       FIVEWORDINLINE(0x2F3C, 0x0004, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
Media3DGetCurrentGroup          (MediaHandler           mh,
                                 void *                 group)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
Media3DTranslateNamedObjectTo   (MediaHandler           mh,
                                 char *                 objectName,
                                 Fixed                  x,
                                 Fixed                  y,
                                 Fixed                  z)                                  FIVEWORDINLINE(0x2F3C, 0x0010, 0x0104, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
Media3DScaleNamedObjectTo       (MediaHandler           mh,
                                 char *                 objectName,
                                 Fixed                  xScale,
                                 Fixed                  yScale,
                                 Fixed                  zScale)                             FIVEWORDINLINE(0x2F3C, 0x0010, 0x0105, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
Media3DRotateNamedObjectTo      (MediaHandler           mh,
                                 char *                 objectName,
                                 Fixed                  xDegrees,
                                 Fixed                  yDegrees,
                                 Fixed                  zDegrees)                           FIVEWORDINLINE(0x2F3C, 0x0010, 0x0106, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
Media3DSetCameraData            (MediaHandler           mh,
                                 void *                 cameraData)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0107, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
Media3DGetCameraData            (MediaHandler           mh,
                                 void *                 cameraData)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0108, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
Media3DSetCameraAngleAspect     (MediaHandler           mh,
                                 QTFloatSingle          fov,
                                 QTFloatSingle          aspectRatioXToY)                    FIVEWORDINLINE(0x2F3C, 0x0008, 0x0109, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
Media3DGetCameraAngleAspect     (MediaHandler           mh,
                                 QTFloatSingle *        fov,
                                 QTFloatSingle *        aspectRatioXToY)                    FIVEWORDINLINE(0x2F3C, 0x0008, 0x010A, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
Media3DSetCameraRange           (MediaHandler           mh,
                                 void *                 tQ3CameraRange)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x010D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
Media3DGetCameraRange           (MediaHandler           mh,
                                 void *                 tQ3CameraRange)                     FIVEWORDINLINE(0x2F3C, 0x0004, 0x010E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
Media3DGetViewObject            (MediaHandler           mh,
                                 void *                 tq3viewObject)                      FIVEWORDINLINE(0x2F3C, 0x0004, 0x010F, 0x7000, 0xA82A);



/****************************************
*                                       *
*   M O V I E   C O N T R O L L E R     *
*                                       *
****************************************/
enum {
    MovieControllerComponentType = FOUR_CHAR_CODE('play')
};


enum {
    kMovieControllerQTVRFlag    = 1 << 0,
    kMovieControllerDontDisplayToUser = 1 << 1
};


typedef ComponentInstance               MovieController;
typedef MovieController *               MovieControllerPtr;
enum {
    mcActionIdle                = 1,                            /* no param*/
    mcActionDraw                = 2,                            /* param is WindowRef*/
    mcActionActivate            = 3,                            /* no param*/
    mcActionDeactivate          = 4,                            /* no param*/
    mcActionMouseDown           = 5,                            /* param is pointer to EventRecord*/
    mcActionKey                 = 6,                            /* param is pointer to EventRecord*/
    mcActionPlay                = 8,                            /* param is Fixed, play rate*/
    mcActionGoToTime            = 12,                           /* param is TimeRecord*/
    mcActionSetVolume           = 14,                           /* param is a short*/
    mcActionGetVolume           = 15,                           /* param is pointer to a short*/
    mcActionStep                = 18,                           /* param is number of steps (short)*/
    mcActionSetLooping          = 21,                           /* param is Boolean*/
    mcActionGetLooping          = 22,                           /* param is pointer to a Boolean*/
    mcActionSetLoopIsPalindrome = 23,                           /* param is Boolean*/
    mcActionGetLoopIsPalindrome = 24,                           /* param is pointer to a Boolean*/
    mcActionSetGrowBoxBounds    = 25,                           /* param is a Rect*/
    mcActionControllerSizeChanged = 26,                         /* no param*/
    mcActionSetSelectionBegin   = 29,                           /* param is TimeRecord*/
    mcActionSetSelectionDuration = 30,                          /* param is TimeRecord, action only taken on set-duration*/
    mcActionSetKeysEnabled      = 32,                           /* param is Boolean*/
    mcActionGetKeysEnabled      = 33,                           /* param is pointer to Boolean*/
    mcActionSetPlaySelection    = 34,                           /* param is Boolean*/
    mcActionGetPlaySelection    = 35,                           /* param is pointer to Boolean*/
    mcActionSetUseBadge         = 36,                           /* param is Boolean*/
    mcActionGetUseBadge         = 37,                           /* param is pointer to Boolean*/
    mcActionSetFlags            = 38,                           /* param is long of flags*/
    mcActionGetFlags            = 39,                           /* param is pointer to a long of flags*/
    mcActionSetPlayEveryFrame   = 40,                           /* param is Boolean*/
    mcActionGetPlayEveryFrame   = 41,                           /* param is pointer to Boolean*/
    mcActionGetPlayRate         = 42,                           /* param is pointer to Fixed*/
    mcActionShowBalloon         = 43,                           /* param is a pointer to a boolean. set to false to stop balloon*/
    mcActionBadgeClick          = 44,                           /* param is pointer to Boolean. set to false to ignore click*/
    mcActionMovieClick          = 45,                           /* param is pointer to event record. change "what" to nullEvt to kill click*/
    mcActionSuspend             = 46,                           /* no param*/
    mcActionResume              = 47,                           /* no param*/
    mcActionSetControllerKeysEnabled = 48,                      /* param is Boolean*/
    mcActionGetTimeSliderRect   = 49,                           /* param is pointer to rect*/
    mcActionMovieEdited         = 50,                           /* no param*/
    mcActionGetDragEnabled      = 51,                           /* param is pointer to Boolean*/
    mcActionSetDragEnabled      = 52,                           /* param is Boolean*/
    mcActionGetSelectionBegin   = 53,                           /* param is TimeRecord*/
    mcActionGetSelectionDuration = 54,                          /* param is TimeRecord*/
    mcActionPrerollAndPlay      = 55,                           /* param is Fixed, play rate*/
    mcActionGetCursorSettingEnabled = 56,                       /* param is pointer to Boolean*/
    mcActionSetCursorSettingEnabled = 57,                       /* param is Boolean*/
    mcActionSetColorTable       = 58,                           /* param is CTabHandle*/
    mcActionLinkToURL           = 59,                           /* param is Handle to URL*/
    mcActionCustomButtonClick   = 60,                           /* param is pointer to EventRecord*/
    mcActionForceTimeTableUpdate = 61,                          /* no param*/
    mcActionSetControllerTimeLimits = 62,                       /* param is pointer to 2 time values min/max. do no send this message to controller. used internally only.*/
    mcActionExecuteAllActionsForQTEvent = 63,                   /* param is ResolvedQTEventSpecPtr*/
    mcActionExecuteOneActionForQTEvent = 64,                    /* param is ResolvedQTEventSpecPtr*/
    mcActionAdjustCursor        = 65,                           /* param is pointer to EventRecord (WindowRef is in message parameter)*/
    mcActionUseTrackForTimeTable = 66,                          /* param is pointer to {long trackID; Boolean useIt}. do not send this message to controller. */
    mcActionClickAndHoldPoint   = 67,                           /* param is point (local coordinates). return true if point has click & hold action (e.g., VR object movie autorotate spot)*/
    mcActionShowMessageString   = 68,                           /* param is a StringPtr*/
    mcActionShowStatusString    = 69,                           /* param is a QTStatusStringPtr*/
    mcActionGetExternalMovie    = 70,                           /* param is a QTGetExternalMoviePtr*/
    mcActionGetChapterTime      = 71,                           /* param is a QTGetChapterTimePtr*/
    mcActionPerformActionList   = 72,                           /* param is a QTAtomSpecPtr*/
    mcActionEvaluateExpression  = 73,                           /* param is a QTEvaluateExpressionPtr*/
    mcActionFetchParameterAs    = 74,                           /* param is a QTFetchParameterAsPtr*/
    mcActionGetCursorByID       = 75,                           /* param is a QTGetCursorByIDPtr*/
    mcActionGetNextURL          = 76,                           /* param is a Handle to URL*/
    mcActionMovieChanged        = 77,
    mcActionDoScript            = 78,                           /* param is QTDoScriptPtr*/
    mcActionRestartAtTime       = 79,                           /* param is QTResartAtTimePtr*/
    mcActionGetIndChapter       = 80,                           /* param is QTChapterInfoPtr*/
    mcActionLinkToURLExtended   = 81,                           /* param is QTAtomContainer as used by QTParseHREF*/
    mcActionSetVolumeStep       = 82,                           /* param is short containing amount to step volume via arrow keys - default = 64*/
    mcActionAutoPlay            = 83,                           /* param is Fixed, play rate*/
    mcActionPauseToBuffer       = 84,                           /* param is Fixed, play rate on restart*/
    mcActionAppMessageReceived  = 85,                           /* param is a long, application message*/
    mcActionEvaluateExpressionWithType = 89,                    /* param is a QTEvaluateExpressionWithTypePtr*/
    mcActionGetMovieName        = 90,                           /* param is a p String Handle*/
    mcActionGetMovieID          = 91,                           /* param is pointer to long*/
    mcActionGetMovieActive      = 92                            /* param is pointer to Boolean*/
};

typedef short                           mcAction;
enum {
    mcFlagSuppressMovieFrame    = 1 << 0,
    mcFlagSuppressStepButtons   = 1 << 1,
    mcFlagSuppressSpeakerButton = 1 << 2,
    mcFlagsUseWindowPalette     = 1 << 3,
    mcFlagsDontInvalidate       = 1 << 4,
    mcFlagsUseCustomButton      = 1 << 5
};


enum {
    mcPositionDontInvalidate    = 1 << 5
};

typedef unsigned long                   mcFlags;
enum {
    kMCIEEnabledButtonPicture   = 1,
    kMCIEDisabledButtonPicture  = 2,
    kMCIEDepressedButtonPicture = 3,
    kMCIEEnabledSizeBoxPicture  = 4,
    kMCIEDisabledSizeBoxPicture = 5,
    kMCIEEnabledUnavailableButtonPicture = 6,
    kMCIEDisabledUnavailableButtonPicture = 7,
    kMCIESoundSlider            = 128,
    kMCIESoundThumb             = 129,
    kMCIEColorTable             = 256,
    kMCIEIsFlatAppearance       = 257,
    kMCIEDoButtonIconsDropOnDepress = 258
};

typedef unsigned long                   MCInterfaceElement;
typedef CALLBACK_API( Boolean , MCActionFilterProcPtr )(MovieController mc, short *action, void *params);
typedef CALLBACK_API( Boolean , MCActionFilterWithRefConProcPtr )(MovieController mc, short action, void *params, long refCon);
typedef STACK_UPP_TYPE(MCActionFilterProcPtr)                   MCActionFilterUPP;
typedef STACK_UPP_TYPE(MCActionFilterWithRefConProcPtr)         MCActionFilterWithRefConUPP;
/*
    menu related stuff
*/
enum {
    mcInfoUndoAvailable         = 1 << 0,
    mcInfoCutAvailable          = 1 << 1,
    mcInfoCopyAvailable         = 1 << 2,
    mcInfoPasteAvailable        = 1 << 3,
    mcInfoClearAvailable        = 1 << 4,
    mcInfoHasSound              = 1 << 5,
    mcInfoIsPlaying             = 1 << 6,
    mcInfoIsLooping             = 1 << 7,
    mcInfoIsInPalindrome        = 1 << 8,
    mcInfoEditingEnabled        = 1 << 9,
    mcInfoMovieIsInteractive    = 1 << 10
};

/* menu item codes*/
enum {
    mcMenuUndo                  = 1,
    mcMenuCut                   = 3,
    mcMenuCopy                  = 4,
    mcMenuPaste                 = 5,
    mcMenuClear                 = 6
};

/* messages to the application via mcActionAppMessageReceived*/
enum {
    kQTAppMessageSoftwareChanged = 1,                           /* notification to app that installed QuickTime software has been updated*/
    kQTAppMessageWindowCloseRequested = 3,                      /* request for app to close window containing movie controller*/
    kQTAppMessageExitFullScreenRequested = 4,                   /* request for app to turn off full screen mode if active*/
    kQTAppMessageDisplayChannels = 5,                           /* request for app to display the channel UI*/
    kQTAppMessageEnterFullScreenRequested = 6                   /* request for app to turn on full screen mode*/
};

/* structures used as mcActionFilterProc params*/

struct QTStatusStringRecord {
    long                            stringTypeFlags;
    char *                          statusString;
};
typedef struct QTStatusStringRecord     QTStatusStringRecord;

typedef QTStatusStringRecord *          QTStatusStringPtr;

struct QTGetExternalMovieRecord {
    long                            targetType;                 /* set to kTargetMovieName or kTargetMovieID*/
    StringPtr                       movieName;
    long                            movieID;
    MoviePtr                        theMovie;
    MovieControllerPtr              theController;
};
typedef struct QTGetExternalMovieRecord QTGetExternalMovieRecord;

typedef QTGetExternalMovieRecord *      QTGetExternalMoviePtr;

struct QTGetChapterTimeRecord {
    StringPtr                       chapterName;
    TimeRecord                      chapterTime;
};
typedef struct QTGetChapterTimeRecord   QTGetChapterTimeRecord;

typedef QTGetChapterTimeRecord *        QTGetChapterTimePtr;

struct QTChapterInfoRecord {
    long                            index;                      /* first chapter has index of 1*/
    TimeValue                       time;                       /* -1 if no more chapters available*/
    Str255                          name;
};
typedef struct QTChapterInfoRecord      QTChapterInfoRecord;

typedef QTChapterInfoRecord *           QTChapterInfoPtr;

struct QTEvaluateExpressionRecord {
    QTAtomSpec                      expressionSpec;
    float *                         expressionResult;
};
typedef struct QTEvaluateExpressionRecord QTEvaluateExpressionRecord;

typedef QTEvaluateExpressionRecord *    QTEvaluateExpressionPtr;

struct QTEvaluateExpressionWithTypeRecord {
    long                            recordSize;                 /* Size of structure (fill in at allocation) */
    QTAtomSpec                      expressionSpec;
    float *                         expressionResult;
    long                            fetchAsType;
    Handle                          nonNumericResult;
                                                                /* Current size is 24 */
};
typedef struct QTEvaluateExpressionWithTypeRecord QTEvaluateExpressionWithTypeRecord;

typedef QTEvaluateExpressionWithTypeRecord * QTEvaluateExpressionWithTypePtr;

struct QTFetchParameterAsRecord {
    QTAtomSpec                      paramListSpec;
    long                            paramIndex;
    long                            paramType;
    long                            allowedFlags;
    void *                          min;
    void *                          max;
    void *                          currentValue;
    void *                          newValue;
    Boolean                         isUnsignedValue;
};
typedef struct QTFetchParameterAsRecord QTFetchParameterAsRecord;

typedef QTFetchParameterAsRecord *      QTFetchParameterAsPtr;

struct QTGetCursorByIDRecord {
    short                           cursorID;
    Handle                          colorCursorData;
    long                            reserved1;
};
typedef struct QTGetCursorByIDRecord    QTGetCursorByIDRecord;

typedef QTGetCursorByIDRecord *         QTGetCursorByIDPtr;

struct QTDoScriptRecord {
    long                            scriptTypeFlags;
    char *                          command;
    char *                          arguments;
};
typedef struct QTDoScriptRecord         QTDoScriptRecord;

typedef QTDoScriptRecord *              QTDoScriptPtr;

struct QTRestartAtTimeRecord {
    TimeValue                       startTime;                  /* time scale is the movie timescale*/
    Fixed                           rate;                       /* if rate is zero, the movie's current rate is maintained*/
};
typedef struct QTRestartAtTimeRecord    QTRestartAtTimeRecord;

typedef QTRestartAtTimeRecord *         QTRestartAtTimePtr;
/* values for paramType field of QTFetchParameterAsRecord*/
enum {
    kFetchAsBooleanPtr          = 1,
    kFetchAsShortPtr            = 2,
    kFetchAsLongPtr             = 3,
    kFetchAsMatrixRecordPtr     = 4,
    kFetchAsModifierTrackGraphicsModeRecord = 5,
    kFetchAsHandle              = 6,
    kFetchAsStr255              = 7,
    kFetchAsFloatPtr            = 8,
    kFetchAsPointPtr            = 9,
    kFetchAsNewAtomContainer    = 10,
    kFetchAsQTEventRecordPtr    = 11,
    kFetchAsFixedPtr            = 12,
    kFetchAsSetControllerValuePtr = 13,
    kFetchAsRgnHandle           = 14,                           /* flipped to native*/
    kFetchAsComponentDescriptionPtr = 15,
    kFetchAsCString             = 16
};

enum {
    kQTCursorOpenHand           = -19183,
    kQTCursorClosedHand         = -19182,
    kQTCursorPointingHand       = -19181,
    kQTCursorRightArrow         = -19180,
    kQTCursorLeftArrow          = -19179,
    kQTCursorDownArrow          = -19178,
    kQTCursorUpArrow            = -19177,
    kQTCursorIBeam              = -19176
};






/* target management */
EXTERN_API( ComponentResult )
MCSetMovie                      (MovieController        mc,
                                 Movie                  theMovie,
                                 WindowRef              movieWindow,
                                 Point                  where)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0002, 0x7000, 0xA82A);

EXTERN_API( Movie )
MCGetIndMovie                   (MovieController        mc,
                                 short                  index)                              FIVEWORDINLINE(0x2F3C, 0x0002, 0x0005, 0x7000, 0xA82A);


#define MCGetMovie(mc) MCGetIndMovie(mc, 0)
EXTERN_API( ComponentResult )
MCRemoveAllMovies               (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCRemoveAMovie                  (MovieController        mc,
                                 Movie                  m)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCRemoveMovie                   (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0006, 0x7000, 0xA82A);

/* event handling etc. */
EXTERN_API( ComponentResult )
MCIsPlayerEvent                 (MovieController        mc,
                                 const EventRecord *    e)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0007, 0x7000, 0xA82A);

/* obsolete. use MCSetActionFilterWithRefCon instead. */
EXTERN_API( ComponentResult )
MCSetActionFilter               (MovieController        mc,
                                 MCActionFilterUPP      blob)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0008, 0x7000, 0xA82A);

/*
    proc is of the form:
        Boolean userPlayerFilter(MovieController mc, short *action, void *params) =
    proc returns TRUE if it handles the action, FALSE if not
    action is passed as a VAR so that it could be changed by filter
    this is consistent with the current dialog manager stuff
    params is any potential parameters that go with the action
        such as set playback rate to xxx.
*/
EXTERN_API( ComponentResult )
MCDoAction                      (MovieController        mc,
                                 short                  action,
                                 void *                 params)                             FIVEWORDINLINE(0x2F3C, 0x0006, 0x0009, 0x7000, 0xA82A);

/* state type things */
EXTERN_API( ComponentResult )
MCSetControllerAttached         (MovieController        mc,
                                 Boolean                attach)                             FIVEWORDINLINE(0x2F3C, 0x0002, 0x000A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCIsControllerAttached          (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCSetControllerPort             (MovieController        mc,
                                 CGrafPtr               gp)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x000C, 0x7000, 0xA82A);

EXTERN_API( CGrafPtr )
MCGetControllerPort             (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x000D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCSetVisible                    (MovieController        mc,
                                 Boolean                visible)                            FIVEWORDINLINE(0x2F3C, 0x0002, 0x000E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCGetVisible                    (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x000F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCGetControllerBoundsRect       (MovieController        mc,
                                 Rect *                 bounds)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCSetControllerBoundsRect       (MovieController        mc,
                                 const Rect *           bounds)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0011, 0x7000, 0xA82A);

EXTERN_API( RgnHandle )
MCGetControllerBoundsRgn        (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0012, 0x7000, 0xA82A);

EXTERN_API( RgnHandle )
MCGetWindowRgn                  (MovieController        mc,
                                 WindowRef              w)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0013, 0x7000, 0xA82A);


/* other stuff */
EXTERN_API( ComponentResult )
MCMovieChanged                  (MovieController        mc,
                                 Movie                  m)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0014, 0x7000, 0xA82A);


/*
    called when the app has changed thing about the movie (like bounding rect) or rate. So that we
        can update our graphical (and internal) state accordingly.
*/
EXTERN_API( ComponentResult )
MCSetDuration                   (MovieController        mc,
                                 TimeValue              duration)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0015, 0x7000, 0xA82A);

/*
    duration to use for time slider -- will be reset next time MCMovieChanged is called
        or MCSetMovie is called
*/
EXTERN_API( TimeValue )
MCGetCurrentTime                (MovieController        mc,
                                 TimeScale *            scale)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0016, 0x7000, 0xA82A);

/*
    returns the time value and the time scale it is on. if there are no movies, the
        time scale is passed back as 0. scale is an optional parameter

*/
EXTERN_API( ComponentResult )
MCNewAttachedController         (MovieController        mc,
                                 Movie                  theMovie,
                                 WindowRef              w,
                                 Point                  where)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0017, 0x7000, 0xA82A);

/*
    makes theMovie the only movie attached to the controller. makes the controller visible.
    the window and where parameters are passed a long to MCSetMovie and behave as
    described there
*/
EXTERN_API( ComponentResult )
MCDraw                          (MovieController        mc,
                                 WindowRef              w)                                  FIVEWORDINLINE(0x2F3C, 0x0004, 0x0018, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCActivate                      (MovieController        mc,
                                 WindowRef              w,
                                 Boolean                activate)                           FIVEWORDINLINE(0x2F3C, 0x0006, 0x0019, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCIdle                          (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x001A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCKey                           (MovieController        mc,
                                 SInt8                  key,
                                 long                   modifiers)                          FIVEWORDINLINE(0x2F3C, 0x0006, 0x001B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCClick                         (MovieController        mc,
                                 WindowRef              w,
                                 Point                  where,
                                 long                   when,
                                 long                   modifiers)                          FIVEWORDINLINE(0x2F3C, 0x0010, 0x001C, 0x7000, 0xA82A);


/*
    calls for editing
*/
EXTERN_API( ComponentResult )
MCEnableEditing                 (MovieController        mc,
                                 Boolean                enabled)                            FIVEWORDINLINE(0x2F3C, 0x0002, 0x001D, 0x7000, 0xA82A);

EXTERN_API( long )
MCIsEditingEnabled              (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x001E, 0x7000, 0xA82A);

EXTERN_API( Movie )
MCCopy                          (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x001F, 0x7000, 0xA82A);

EXTERN_API( Movie )
MCCut                           (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0020, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCPaste                         (MovieController        mc,
                                 Movie                  srcMovie)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0021, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCClear                         (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0022, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCUndo                          (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0023, 0x7000, 0xA82A);


/*
 *  somewhat special stuff
 */
EXTERN_API( ComponentResult )
MCPositionController            (MovieController        mc,
                                 const Rect *           movieRect,
                                 const Rect *           controllerRect,
                                 long                   someFlags)                          FIVEWORDINLINE(0x2F3C, 0x000C, 0x0024, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
MCGetControllerInfo             (MovieController        mc,
                                 long *                 someFlags)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x0025, 0x7000, 0xA82A);



EXTERN_API( ComponentResult )
MCSetClip                       (MovieController        mc,
                                 RgnHandle              theClip,
                                 RgnHandle              movieClip)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0028, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCGetClip                       (MovieController        mc,
                                 RgnHandle *            theClip,
                                 RgnHandle *            movieClip)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0029, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
MCDrawBadge                     (MovieController        mc,
                                 RgnHandle              movieRgn,
                                 RgnHandle *            badgeRgn)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x002A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCSetUpEditMenu                 (MovieController        mc,
                                 long                   modifiers,
                                 MenuRef                mh)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x002B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCGetMenuString                 (MovieController        mc,
                                 long                   modifiers,
                                 short                  item,
                                 Str255                 aString)                            FIVEWORDINLINE(0x2F3C, 0x000A, 0x002C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCSetActionFilterWithRefCon     (MovieController        mc,
                                 MCActionFilterWithRefConUPP  blob,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x002D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCPtInController                (MovieController        mc,
                                 Point                  thePt,
                                 Boolean *              inController)                       FIVEWORDINLINE(0x2F3C, 0x0008, 0x002E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCInvalidate                    (MovieController        mc,
                                 WindowRef              w,
                                 RgnHandle              invalidRgn)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x002F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCAdjustCursor                  (MovieController        mc,
                                 WindowRef              w,
                                 Point                  where,
                                 long                   modifiers)                          FIVEWORDINLINE(0x2F3C, 0x000C, 0x0030, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCGetInterfaceElement           (MovieController        mc,
                                 MCInterfaceElement     whichElement,
                                 void *                 element)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x0031, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCGetDoActionsProc              (MovieController        mc,
                                 DoMCActionUPP *        doMCActionProc,
                                 long *                 doMCActionRefCon)                   FIVEWORDINLINE(0x2F3C, 0x0008, 0x0032, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCAddMovieSegment               (MovieController        mc,
                                 Movie                  srcMovie,
                                 Boolean                scaled)                             FIVEWORDINLINE(0x2F3C, 0x0006, 0x0033, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCTrimMovieSegment              (MovieController        mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0034, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MCSetIdleManager                (MovieController        mc,
                                 IdleManager            im)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0035, 0x7000, 0xA82A);

/* Called (but not implemented) by controllers that derive from the standard movie controller.
   All controllers except standard movie controller must delegate this call. */
enum {
    kControllerUnderstandsIdleManagers = 1 << 0
};

EXTERN_API( ComponentResult )
MCSetControllerCapabilities     (MovieController        mc,
                                 long                   flags,
                                 long                   flagsMask)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0036, 0x7000, 0xA82A);





/****************************************
*                                       *
*       T  I  M  E  B  A  S  E          *
*                                       *
****************************************/
EXTERN_API( TimeBase )
NewTimeBase                     (void)                                                      THREEWORDINLINE(0x303C, 0x00A5, 0xAAAA);

EXTERN_API( void )
DisposeTimeBase                 (TimeBase               tb)                                 THREEWORDINLINE(0x303C, 0x00B6, 0xAAAA);

EXTERN_API( TimeValue )
GetTimeBaseTime                 (TimeBase               tb,
                                 TimeScale              s,
                                 TimeRecord *           tr)                                 THREEWORDINLINE(0x303C, 0x00A6, 0xAAAA);

EXTERN_API( void )
SetTimeBaseTime                 (TimeBase               tb,
                                 const TimeRecord *     tr)                                 THREEWORDINLINE(0x303C, 0x00A7, 0xAAAA);

EXTERN_API( void )
SetTimeBaseValue                (TimeBase               tb,
                                 TimeValue              t,
                                 TimeScale              s)                                  THREEWORDINLINE(0x303C, 0x00A8, 0xAAAA);

EXTERN_API( Fixed )
GetTimeBaseRate                 (TimeBase               tb)                                 THREEWORDINLINE(0x303C, 0x00A9, 0xAAAA);

EXTERN_API( void )
SetTimeBaseRate                 (TimeBase               tb,
                                 Fixed                  r)                                  THREEWORDINLINE(0x303C, 0x00AA, 0xAAAA);

EXTERN_API( TimeValue )
GetTimeBaseStartTime            (TimeBase               tb,
                                 TimeScale              s,
                                 TimeRecord *           tr)                                 THREEWORDINLINE(0x303C, 0x00AB, 0xAAAA);

EXTERN_API( void )
SetTimeBaseStartTime            (TimeBase               tb,
                                 const TimeRecord *     tr)                                 THREEWORDINLINE(0x303C, 0x00AC, 0xAAAA);

EXTERN_API( TimeValue )
GetTimeBaseStopTime             (TimeBase               tb,
                                 TimeScale              s,
                                 TimeRecord *           tr)                                 THREEWORDINLINE(0x303C, 0x00AD, 0xAAAA);

EXTERN_API( void )
SetTimeBaseStopTime             (TimeBase               tb,
                                 const TimeRecord *     tr)                                 THREEWORDINLINE(0x303C, 0x00AE, 0xAAAA);

EXTERN_API( long )
GetTimeBaseFlags                (TimeBase               tb)                                 THREEWORDINLINE(0x303C, 0x00B1, 0xAAAA);

EXTERN_API( void )
SetTimeBaseFlags                (TimeBase               tb,
                                 long                   timeBaseFlags)                      THREEWORDINLINE(0x303C, 0x00B2, 0xAAAA);

EXTERN_API( void )
SetTimeBaseMasterTimeBase       (TimeBase               slave,
                                 TimeBase               master,
                                 const TimeRecord *     slaveZero)                          THREEWORDINLINE(0x303C, 0x00B4, 0xAAAA);

EXTERN_API( TimeBase )
GetTimeBaseMasterTimeBase       (TimeBase               tb)                                 THREEWORDINLINE(0x303C, 0x00AF, 0xAAAA);

EXTERN_API( void )
SetTimeBaseMasterClock          (TimeBase               slave,
                                 Component              clockMeister,
                                 const TimeRecord *     slaveZero)                          THREEWORDINLINE(0x303C, 0x00B3, 0xAAAA);

EXTERN_API( ComponentInstance )
GetTimeBaseMasterClock          (TimeBase               tb)                                 THREEWORDINLINE(0x303C, 0x00B0, 0xAAAA);

EXTERN_API( void )
ConvertTime                     (TimeRecord *           inout,
                                 TimeBase               newBase)                            THREEWORDINLINE(0x303C, 0x00B5, 0xAAAA);

EXTERN_API( void )
ConvertTimeScale                (TimeRecord *           inout,
                                 TimeScale              newScale)                           THREEWORDINLINE(0x303C, 0x00B7, 0xAAAA);

EXTERN_API( void )
AddTime                         (TimeRecord *           dst,
                                 const TimeRecord *     src)                                THREEWORDINLINE(0x303C, 0x010C, 0xAAAA);

EXTERN_API( void )
SubtractTime                    (TimeRecord *           dst,
                                 const TimeRecord *     src)                                THREEWORDINLINE(0x303C, 0x010D, 0xAAAA);

EXTERN_API( long )
GetTimeBaseStatus               (TimeBase               tb,
                                 TimeRecord *           unpinnedTime)                       THREEWORDINLINE(0x303C, 0x010B, 0xAAAA);

EXTERN_API( void )
SetTimeBaseZero                 (TimeBase               tb,
                                 TimeRecord *           zero)                               THREEWORDINLINE(0x303C, 0x0128, 0xAAAA);

EXTERN_API( Fixed )
GetTimeBaseEffectiveRate        (TimeBase               tb)                                 THREEWORDINLINE(0x303C, 0x0124, 0xAAAA);


/****************************************
*                                       *
*       C  A  L  L  B  A  C  K          *
*                                       *
****************************************/
EXTERN_API( QTCallBack )
NewCallBack                     (TimeBase               tb,
                                 short                  cbType)                             THREEWORDINLINE(0x303C, 0x00EB, 0xAAAA);

EXTERN_API( void )
DisposeCallBack                 (QTCallBack             cb)                                 THREEWORDINLINE(0x303C, 0x00EC, 0xAAAA);

EXTERN_API( short )
GetCallBackType                 (QTCallBack             cb)                                 THREEWORDINLINE(0x303C, 0x00ED, 0xAAAA);

EXTERN_API( TimeBase )
GetCallBackTimeBase             (QTCallBack             cb)                                 THREEWORDINLINE(0x303C, 0x00EE, 0xAAAA);

EXTERN_API( OSErr )
CallMeWhen                      (QTCallBack             cb,
                                 QTCallBackUPP          callBackProc,
                                 long                   refCon,
                                 long                   param1,
                                 long                   param2,
                                 long                   param3)                             THREEWORDINLINE(0x303C, 0x00B8, 0xAAAA);

EXTERN_API( void )
CancelCallBack                  (QTCallBack             cb)                                 THREEWORDINLINE(0x303C, 0x00B9, 0xAAAA);


/****************************************
*                                       *
*       C L O C K   C A L L B A C K     *
*             S U P P O R T             *
*                                       *
****************************************/
EXTERN_API( OSErr )
AddCallBackToTimeBase           (QTCallBack             cb)                                 THREEWORDINLINE(0x303C, 0x0129, 0xAAAA);

EXTERN_API( OSErr )
RemoveCallBackFromTimeBase      (QTCallBack             cb)                                 THREEWORDINLINE(0x303C, 0x012A, 0xAAAA);

EXTERN_API( QTCallBack )
GetFirstCallBack                (TimeBase               tb)                                 THREEWORDINLINE(0x303C, 0x012B, 0xAAAA);

EXTERN_API( QTCallBack )
GetNextCallBack                 (QTCallBack             cb)                                 THREEWORDINLINE(0x303C, 0x012C, 0xAAAA);

EXTERN_API( void )
ExecuteCallBack                 (QTCallBack             cb)                                 THREEWORDINLINE(0x303C, 0x012D, 0xAAAA);






EXTERN_API( ComponentResult )
MusicMediaGetIndexedTunePlayer  (ComponentInstance      ti,
                                 long                   sampleDescIndex,
                                 ComponentInstance *    tp)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0101, 0x7000, 0xA82A);


/* UPP call backs */
#if OPAQUE_UPP_TYPES
    EXTERN_API(MCActionFilterUPP)
    NewMCActionFilterUPP           (MCActionFilterProcPtr   userRoutine);

    EXTERN_API(MCActionFilterWithRefConUPP)
    NewMCActionFilterWithRefConUPP    (MCActionFilterWithRefConProcPtr userRoutine);

    EXTERN_API(void)
    DisposeMCActionFilterUPP       (MCActionFilterUPP       userUPP);

    EXTERN_API(void)
    DisposeMCActionFilterWithRefConUPP    (MCActionFilterWithRefConUPP userUPP);

    EXTERN_API(Boolean)
    InvokeMCActionFilterUPP        (MovieController         mc,
                                    short *                 action,
                                    void *                  params,
                                    MCActionFilterUPP       userUPP);

    EXTERN_API(Boolean)
    InvokeMCActionFilterWithRefConUPP    (MovieController   mc,
                                    short                   action,
                                    void *                  params,
                                    long                    refCon,
                                    MCActionFilterWithRefConUPP userUPP);

#else
    enum { uppMCActionFilterProcInfo = 0x00000FD0 };                /* pascal 1_byte Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppMCActionFilterWithRefConProcInfo = 0x00003ED0 };      /* pascal 1_byte Func(4_bytes, 2_bytes, 4_bytes, 4_bytes) */
    #define NewMCActionFilterUPP(userRoutine)                       (MCActionFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMCActionFilterProcInfo, GetCurrentArchitecture())
    #define NewMCActionFilterWithRefConUPP(userRoutine)             (MCActionFilterWithRefConUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMCActionFilterWithRefConProcInfo, GetCurrentArchitecture())
    #define DisposeMCActionFilterUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define DisposeMCActionFilterWithRefConUPP(userUPP)             DisposeRoutineDescriptor(userUPP)
    #define InvokeMCActionFilterUPP(mc, action, params, userUPP)    (Boolean)CALL_THREE_PARAMETER_UPP((userUPP), uppMCActionFilterProcInfo, (mc), (action), (params))
    #define InvokeMCActionFilterWithRefConUPP(mc, action, params, refCon, userUPP)  (Boolean)CALL_FOUR_PARAMETER_UPP((userUPP), uppMCActionFilterWithRefConProcInfo, (mc), (action), (params), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewMCActionFilterProc(userRoutine)                      NewMCActionFilterUPP(userRoutine)
#define NewMCActionFilterWithRefConProc(userRoutine)            NewMCActionFilterWithRefConUPP(userRoutine)
#define CallMCActionFilterProc(userRoutine, mc, action, params) InvokeMCActionFilterUPP(mc, action, params, userRoutine)
#define CallMCActionFilterWithRefConProc(userRoutine, mc, action, params, refCon) InvokeMCActionFilterWithRefConUPP(mc, action, params, refCon, userRoutine)

/* selectors for component calls */
enum {
    kVideoMediaResetStatisticsSelect           = 0x0105,
    kVideoMediaGetStatisticsSelect             = 0x0106,
    kVideoMediaGetStallCountSelect             = 0x010E,
    kVideoMediaSetCodecParameterSelect         = 0x010F,
    kVideoMediaGetCodecParameterSelect         = 0x0110,
    kTextMediaSetTextProcSelect                = 0x0101,
    kTextMediaAddTextSampleSelect              = 0x0102,
    kTextMediaAddTESampleSelect                = 0x0103,
    kTextMediaAddHiliteSampleSelect            = 0x0104,
    kTextMediaDrawRawSelect                    = 0x0109,
    kTextMediaSetTextPropertySelect            = 0x010A,
    kTextMediaRawSetupSelect                   = 0x010B,
    kTextMediaRawIdleSelect                    = 0x010C,
    kTextMediaGetTextPropertySelect            = 0x010D,
    kTextMediaFindNextTextSelect               = 0x0105,
    kTextMediaHiliteTextSampleSelect           = 0x0106,
    kTextMediaSetTextSampleDataSelect          = 0x0107,
    kSpriteMediaSetPropertySelect              = 0x0101,
    kSpriteMediaGetPropertySelect              = 0x0102,
    kSpriteMediaHitTestSpritesSelect           = 0x0103,
    kSpriteMediaCountSpritesSelect             = 0x0104,
    kSpriteMediaCountImagesSelect              = 0x0105,
    kSpriteMediaGetIndImageDescriptionSelect   = 0x0106,
    kSpriteMediaGetDisplayedSampleNumberSelect = 0x0107,
    kSpriteMediaGetSpriteNameSelect            = 0x0108,
    kSpriteMediaGetImageNameSelect             = 0x0109,
    kSpriteMediaSetSpritePropertySelect        = 0x010A,
    kSpriteMediaGetSpritePropertySelect        = 0x010B,
    kSpriteMediaHitTestAllSpritesSelect        = 0x010C,
    kSpriteMediaHitTestOneSpriteSelect         = 0x010D,
    kSpriteMediaSpriteIndexToIDSelect          = 0x010E,
    kSpriteMediaSpriteIDToIndexSelect          = 0x010F,
    kSpriteMediaGetSpriteActionsForQTEventSelect = 0x0110,
    kSpriteMediaSetActionVariableSelect        = 0x0111,
    kSpriteMediaGetActionVariableSelect        = 0x0112,
    kSpriteMediaGetIndImagePropertySelect      = 0x0113,
    kSpriteMediaNewSpriteSelect                = 0x0114,
    kSpriteMediaDisposeSpriteSelect            = 0x0115,
    kSpriteMediaSetActionVariableToStringSelect = 0x0116,
    kSpriteMediaGetActionVariableAsStringSelect = 0x0117,
    kSpriteMediaNewImageSelect                 = 0x011B,
    kSpriteMediaDisposeImageSelect             = 0x011C,
    kSpriteMediaImageIndexToIDSelect           = 0x011D,
    kSpriteMediaImageIDToIndexSelect           = 0x011E,
    kFlashMediaSetPanSelect                    = 0x0101,
    kFlashMediaSetZoomSelect                   = 0x0102,
    kFlashMediaSetZoomRectSelect               = 0x0103,
    kFlashMediaGetRefConBoundsSelect           = 0x0104,
    kFlashMediaGetRefConIDSelect               = 0x0105,
    kFlashMediaIDToRefConSelect                = 0x0106,
    kFlashMediaGetDisplayedFrameNumberSelect   = 0x0107,
    kFlashMediaFrameNumberToMovieTimeSelect    = 0x0108,
    kFlashMediaFrameLabelToMovieTimeSelect     = 0x0109,
    kFlashMediaGetFlashVariableSelect          = 0x010A,
    kFlashMediaSetFlashVariableSelect          = 0x010B,
    kFlashMediaDoButtonActionsSelect           = 0x010C,
    kFlashMediaGetSupportedSwfVersionSelect    = 0x010D,
    kMovieMediaGetChildDoMCActionCallbackSelect = 0x0102,
    kMovieMediaGetDoMCActionCallbackSelect     = 0x0103,
    kMovieMediaGetCurrentMoviePropertySelect   = 0x0104,
    kMovieMediaGetCurrentTrackPropertySelect   = 0x0105,
    kMovieMediaGetChildMovieDataReferenceSelect = 0x0106,
    kMovieMediaSetChildMovieDataReferenceSelect = 0x0107,
    kMovieMediaLoadChildMovieFromDataReferenceSelect = 0x0108,
    kMedia3DGetNamedObjectListSelect           = 0x0101,
    kMedia3DGetRendererListSelect              = 0x0102,
    kMedia3DGetCurrentGroupSelect              = 0x0103,
    kMedia3DTranslateNamedObjectToSelect       = 0x0104,
    kMedia3DScaleNamedObjectToSelect           = 0x0105,
    kMedia3DRotateNamedObjectToSelect          = 0x0106,
    kMedia3DSetCameraDataSelect                = 0x0107,
    kMedia3DGetCameraDataSelect                = 0x0108,
    kMedia3DSetCameraAngleAspectSelect         = 0x0109,
    kMedia3DGetCameraAngleAspectSelect         = 0x010A,
    kMedia3DSetCameraRangeSelect               = 0x010D,
    kMedia3DGetCameraRangeSelect               = 0x010E,
    kMedia3DGetViewObjectSelect                = 0x010F,
    kMCSetMovieSelect                          = 0x0002,
    kMCGetIndMovieSelect                       = 0x0005,
    kMCRemoveAllMoviesSelect                   = 0x0006,
    kMCRemoveAMovieSelect                      = 0x0003,
    kMCRemoveMovieSelect                       = 0x0006,
    kMCIsPlayerEventSelect                     = 0x0007,
    kMCSetActionFilterSelect                   = 0x0008,
    kMCDoActionSelect                          = 0x0009,
    kMCSetControllerAttachedSelect             = 0x000A,
    kMCIsControllerAttachedSelect              = 0x000B,
    kMCSetControllerPortSelect                 = 0x000C,
    kMCGetControllerPortSelect                 = 0x000D,
    kMCSetVisibleSelect                        = 0x000E,
    kMCGetVisibleSelect                        = 0x000F,
    kMCGetControllerBoundsRectSelect           = 0x0010,
    kMCSetControllerBoundsRectSelect           = 0x0011,
    kMCGetControllerBoundsRgnSelect            = 0x0012,
    kMCGetWindowRgnSelect                      = 0x0013,
    kMCMovieChangedSelect                      = 0x0014,
    kMCSetDurationSelect                       = 0x0015,
    kMCGetCurrentTimeSelect                    = 0x0016,
    kMCNewAttachedControllerSelect             = 0x0017,
    kMCDrawSelect                              = 0x0018,
    kMCActivateSelect                          = 0x0019,
    kMCIdleSelect                              = 0x001A,
    kMCKeySelect                               = 0x001B,
    kMCClickSelect                             = 0x001C,
    kMCEnableEditingSelect                     = 0x001D,
    kMCIsEditingEnabledSelect                  = 0x001E,
    kMCCopySelect                              = 0x001F,
    kMCCutSelect                               = 0x0020,
    kMCPasteSelect                             = 0x0021,
    kMCClearSelect                             = 0x0022,
    kMCUndoSelect                              = 0x0023,
    kMCPositionControllerSelect                = 0x0024,
    kMCGetControllerInfoSelect                 = 0x0025,
    kMCSetClipSelect                           = 0x0028,
    kMCGetClipSelect                           = 0x0029,
    kMCDrawBadgeSelect                         = 0x002A,
    kMCSetUpEditMenuSelect                     = 0x002B,
    kMCGetMenuStringSelect                     = 0x002C,
    kMCSetActionFilterWithRefConSelect         = 0x002D,
    kMCPtInControllerSelect                    = 0x002E,
    kMCInvalidateSelect                        = 0x002F,
    kMCAdjustCursorSelect                      = 0x0030,
    kMCGetInterfaceElementSelect               = 0x0031,
    kMCGetDoActionsProcSelect                  = 0x0032,
    kMCAddMovieSegmentSelect                   = 0x0033,
    kMCTrimMovieSegmentSelect                  = 0x0034,
    kMCSetIdleManagerSelect                    = 0x0035,
    kMCSetControllerCapabilitiesSelect         = 0x0036,
    kMusicMediaGetIndexedTunePlayerSelect      = 0x0101
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

#endif /* __MOVIES__ */

