/*
     File:       MovieManager.h
 
     Contains:   Interface for MovieManager component
 
     Version:    Technology: QuickTime 4.2
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 2000-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MOVIEMANAGER__
#define __MOVIEMANAGER__

#ifndef __MOVIES__
#include <Movies.h>
#endif

#ifndef __QUICKTIMEVR__
#include <QuickTimeVR.h>
#endif

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MACWINDOWS__
#include <MacWindows.h>
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

typedef ComponentInstance               MovieManagerComponent;
typedef void *                          ObjectRef;
typedef void *                          ObjectSpec;
typedef OSType                          MessageClass;
typedef UInt32                          PropertyID;
typedef UInt32                          MessageID;
typedef OSType                          ValueFormat;
typedef void *                          MessageData;
/* == message class constants ==*/
enum {
    kMMgrGetPropertyClass       = FOUR_CHAR_CODE('getp'),
    kMMgrSetPropertyClass       = FOUR_CHAR_CODE('setp'),
    kMMgrSendCommandClass       = FOUR_CHAR_CODE('cmd ')
};


struct CharBuff {
    UInt32                          length;
    char *                          buff;
};
typedef struct CharBuff                 CharBuff;

struct PropArg {
    ValueFormat                     valueFormat;
    void *                          value;                      /* data if <= 32 bits, otherwise pointer to the actual data */
};
typedef struct PropArg                  PropArg;

struct PropertyParams {
    UInt16                          paramCount;
    UInt16                          flags;
    PropArg                         args[8];
};
typedef struct PropertyParams           PropertyParams;
/*
   
   struct with the same layout as a PropArg but with a union for the value field for ease of use 
*/

struct PropArgU {
    ValueFormat                     valueFormat;
    union {
        Boolean                         booleanVal;
        SInt8                           byteVal;
        SInt32                          longVal;
        SInt16                          shortVal;
        Fixed                           fixedVal;
        Point                           pointVal;
        QTFloatSingle                   float32Val;
        QTFloatDouble *                 float64PtrVal;
        void *                          voidVal;
        StringPtr                       str255Val;
        char *                          cStrVal;
        CharBuff *                      charBuffVal;
        RGBColor *                      rbgPtr;
        Ptr                             ptrVal;
        Handle                          textHandle;
    }                                 value;
};
typedef struct PropArgU                 PropArgU;
/*
   
   struct with the same layout as a PropertyParams but with an array of PropArgUs
*/

struct PropertyParamsU {
    UInt16                          paramCount;
    UInt16                          flags;
    PropArgU                        args[8];
};
typedef struct PropertyParamsU          PropertyParamsU;

struct FindOffset {
    TimeValue                       time;                       /* the movie time at which the text was found */
    SInt32                          trackNum;                   /* number of the track in which the text was found */
    SInt32                          offset;                     /* offset (in chars) of the text with the track's sample */
};
typedef struct FindOffset               FindOffset;
/* == enums ==*/

/* Movie Commands */
enum {
    kMMgrCmdAdd                 = FOUR_CHAR_CODE('add '),
    kMMgrCmdAddScaled           = FOUR_CHAR_CODE('adsc'),
    kMMgrCmdClear               = FOUR_CHAR_CODE('cler'),
    kMMgrCmdCopy                = FOUR_CHAR_CODE('copy'),
    kMMgrCmdCopyFrame           = FOUR_CHAR_CODE('cfrm'),
    kMMgrCmdCut                 = FOUR_CHAR_CODE('cut '),
    kMMgrCmdDelete              = FOUR_CHAR_CODE('delt'),
    kMMgrCmdExportMovie         = FOUR_CHAR_CODE('expt'),
    kMMgrCmdFindText            = FOUR_CHAR_CODE('find'),
    kMMgrCmdFrameLoaded         = FOUR_CHAR_CODE('fram'),
    kMMgrCmdGoBackAction        = FOUR_CHAR_CODE('gobk'),
    kMMgrCmdIdle                = FOUR_CHAR_CODE('idle'),
    kMMgrCmdLoadExporterSettings = FOUR_CHAR_CODE('lexp'),
    kMMgrCmdMouseDown           = FOUR_CHAR_CODE('clik'),
    kMMgrCmdMouseEnter          = FOUR_CHAR_CODE('entr'),
    kMMgrCmdMouseExit           = FOUR_CHAR_CODE('exit'),
    kMMgrCmdMouseTrigger        = FOUR_CHAR_CODE('trig'),
    kMMgrCmdMouseUp             = FOUR_CHAR_CODE('cend'),
    kMMgrCmdMovieLoaded         = FOUR_CHAR_CODE('load'),
    kMMgrCmdNudge               = FOUR_CHAR_CODE('nudg'),
    kMMgrCmdPaste               = FOUR_CHAR_CODE('pste'),
    kMMgrCmdPause               = FOUR_CHAR_CODE('paus'),
    kMMgrCmdPlay                = FOUR_CHAR_CODE('play'),
    kMMgrCmdPreroll             = FOUR_CHAR_CODE('prol'),
    kMMgrCmdReplace             = FOUR_CHAR_CODE('rplc'),
    kMMgrCmdReverse             = FOUR_CHAR_CODE('rvrs'),
    kMMgrCmdRewind              = FOUR_CHAR_CODE('rwnd'),
    kMMgrCmdRotate              = FOUR_CHAR_CODE('rota'),
    kMMgrCmdSave                = FOUR_CHAR_CODE('sve '),
    kMMgrCmdSaveAs              = FOUR_CHAR_CODE('sas '),
    kMMgrCmdSaveAsSelfContained = FOUR_CHAR_CODE('sass'),
    kMMgrCmdSaveCopy            = FOUR_CHAR_CODE('scp '),
    kMMgrCmdSaveCopySelfContained = FOUR_CHAR_CODE('scps'),
    kMMgrCmdSaveExporterSettings = FOUR_CHAR_CODE('sexp'),
    kMMgrCmdSaveSelfContained   = FOUR_CHAR_CODE('sves'),
    kMMgrCmdShowDefaultView     = FOUR_CHAR_CODE('sdfl'),
    kMMgrCmdShowExportDialog    = FOUR_CHAR_CODE('shoe'),
    kMMgrCmdShowPoster          = FOUR_CHAR_CODE('spos'),
    kMMgrCmdSkew                = FOUR_CHAR_CODE('skew'),
    kMMgrCmdStep                = FOUR_CHAR_CODE('step'),
    kMMgrCmdStepFwd             = FOUR_CHAR_CODE('stpf'),
    kMMgrCmdStepRev             = FOUR_CHAR_CODE('stpr'),
    kMMgrCmdStop                = FOUR_CHAR_CODE('stop'),
    kMMgrCmdTrim                = FOUR_CHAR_CODE('trim'),
    kMMgrCmdUndo                = FOUR_CHAR_CODE('undo')
};


/* Properties */
enum {
    kMMgrPropActionsEnabled     = FOUR_CHAR_CODE('a ok'),
    kMMgrPropActiveTransport    = FOUR_CHAR_CODE('atrn'),
    kMMgrPropAlbum              = FOUR_CHAR_CODE(' alb'),
    kMMgrPropAllowBalloonHelp   = FOUR_CHAR_CODE('balo'),
    kMMgrPropAllowEditing       = FOUR_CHAR_CODE('edit'),
    kMMgrPropAllowMultipleStreams = FOUR_CHAR_CODE('amss'),
    kMMgrPropArtist             = FOUR_CHAR_CODE('@ART'),
    kMMgrPropAttached           = FOUR_CHAR_CODE('atch'),
    kMMgrPropAutoPlay           = FOUR_CHAR_CODE('play'),
    kMMgrPropBackColor          = FOUR_CHAR_CODE('bclr'),
    kMMgrPropBadge              = FOUR_CHAR_CODE('badg'),
    kMMgrPropBitRate            = FOUR_CHAR_CODE('brte'),
    kMMgrPropBottom             = FOUR_CHAR_CODE('botm'),
    kMMgrPropBottomRight        = FOUR_CHAR_CODE('btrt'),
    kMMgrPropChapterTrackRef    = FOUR_CHAR_CODE('chap'),
    kMMgrPropChapterMode        = FOUR_CHAR_CODE('chmd'),
    kMMgrPropChapterName        = FOUR_CHAR_CODE('@chp'),
    kMMgrPropCharacteristics    = FOUR_CHAR_CODE('char'),
    kMMgrPropCodecSequence      = FOUR_CHAR_CODE('cseq'),
    kMMgrPropCommands           = FOUR_CHAR_CODE('cmds'),
    kMMgrPropComments           = FOUR_CHAR_CODE('cmnt'),
    kMMgrPropComponentVersion   = FOUR_CHAR_CODE('cvrs'),
    kMMgrPropCompressMovieHeader = FOUR_CHAR_CODE('chdr'),
    kMMgrPropConnectionSpeed    = FOUR_CHAR_CODE('cspd'),
    kMMgrPropConverted          = FOUR_CHAR_CODE('cnvt'),
    kMMgrPropCopyright          = FOUR_CHAR_CODE(' cpy'),
    kMMgrPropCursorID           = FOUR_CHAR_CODE('crid'),
    kMMgrPropCurrNodeID         = FOUR_CHAR_CODE('cnod'),
    kMMgrPropDataRefs           = FOUR_CHAR_CODE('dtrf'),
    kMMgrPropDefaultNodeID      = FOUR_CHAR_CODE('dnod'),
    kMMgrPropDescription        = FOUR_CHAR_CODE(' des'),
    kMMgrPropDefaultRect        = FOUR_CHAR_CODE('drct'),
    kMMgrPropDragEnabled        = FOUR_CHAR_CODE('drag'),
    kMMgrPropDuration           = FOUR_CHAR_CODE('dura'),
    kMMgrPropEnabled            = FOUR_CHAR_CODE('enab'),
    kMMgrPropEndSelection       = FOUR_CHAR_CODE('ends'),
    kMMgrPropEndTime            = FOUR_CHAR_CODE('endt'),
    kMMgrPropExporter           = FOUR_CHAR_CODE('exp '),
    kMMgrPropExporters          = FOUR_CHAR_CODE('exps'),
    kMMgrPropFieldOfView        = FOUR_CHAR_CODE('fov '),
    kMMgrPropFieldOfViewRange   = FOUR_CHAR_CODE('fovr'),
    kMMgrPropFindTextCaseSensitive = FOUR_CHAR_CODE('fcse'),
    kMMgrPropFindTextReverse    = FOUR_CHAR_CODE('frev'),
    kMMgrPropFindTextStart      = FOUR_CHAR_CODE('fstt'),
    kMMgrPropFindTextWrapAround = FOUR_CHAR_CODE('fwrp'),
    kMMgrPropFlattenCrossplatform = FOUR_CHAR_CODE('flat'),
    kMMgrPropForeColor          = FOUR_CHAR_CODE('fclr'),
    kMMgrPropFormat             = FOUR_CHAR_CODE('frmt'),
    kMMgrPropFoundTextOffset    = FOUR_CHAR_CODE('foff'),
    kMMgrPropFrameObjects       = FOUR_CHAR_CODE('frob'),
    kMMgrPropFrameRate          = FOUR_CHAR_CODE('frte'),
    kMMgrPropFullCommandList    = FOUR_CHAR_CODE('fcmd'),
    kMMgrPropFullName           = FOUR_CHAR_CODE(' nam'),
    kMMgrPropFullPropertyList   = FOUR_CHAR_CODE('fprp'),
    kMMgrPropGraphicsMode       = FOUR_CHAR_CODE('mode'),
    kMMgrPropHasChanged         = FOUR_CHAR_CODE('drty'),
    kMMgrPropHeight             = FOUR_CHAR_CODE('hght'),
    kMMgrPropHighQualityMode    = FOUR_CHAR_CODE('hqty'),
    kMMgrPropHREF               = FOUR_CHAR_CODE('href'),
    kMMgrPropHTTPProxy          = FOUR_CHAR_CODE('http'),
    kMMgrPropHTTPTransport      = FOUR_CHAR_CODE('httt'),
    kMMgrPropID                 = FOUR_CHAR_CODE('id  '),
    kMMgrPropIdleFrequency      = FOUR_CHAR_CODE('idle'),
    kMMgrPropImageCorrection    = FOUR_CHAR_CODE('icor'),
    kMMgrPropImageName          = FOUR_CHAR_CODE('imnm'),
    kMMgrPropImageNdx           = FOUR_CHAR_CODE('imnd'),
    kMMgrPropImageQuality       = FOUR_CHAR_CODE('iqty'),
    kMMgrPropInformation        = FOUR_CHAR_CODE(' inf'),
    kMMgrPropIsRegistered       = FOUR_CHAR_CODE('regi'),
    kMMgrPropIsVRMovie          = FOUR_CHAR_CODE('isvr'),
    kMMgrPropLanguage           = FOUR_CHAR_CODE('lang'),
    kMMgrPropLayer              = FOUR_CHAR_CODE('layr'),
    kMMgrPropLeft               = FOUR_CHAR_CODE('left'),
    kMMgrPropLinkDestination    = FOUR_CHAR_CODE('dest'),
    kMMgrPropLoadState          = FOUR_CHAR_CODE('load'),
    kMMgrPropLocation           = FOUR_CHAR_CODE('loc '),
    kMMgrPropLockMessages       = FOUR_CHAR_CODE('lokm'),
    kMMgrPropLockScreen         = FOUR_CHAR_CODE('loks'),
    kMMgrPropLoop               = FOUR_CHAR_CODE('loop'),
    kMMgrPropLoopIsPalindrome   = FOUR_CHAR_CODE('paln'),
    kMMgrPropMask               = FOUR_CHAR_CODE('mask'),
    kMMgrPropMatrix             = FOUR_CHAR_CODE('mtrx'),
    kMMgrPropMaxTimeLoaded      = FOUR_CHAR_CODE('mtme'),
    kMMgrPropMemCheckLevel      = FOUR_CHAR_CODE('dbmc'),
    kMMgrPropMessages           = FOUR_CHAR_CODE('msg '),
    kMMgrPropMIMEType           = FOUR_CHAR_CODE('mime'),
    kMMgrPropMissingComponents  = FOUR_CHAR_CODE('miss'),
    kMMgrPropMovie              = FOUR_CHAR_CODE('moov'),
    kMMgrPropMute               = FOUR_CHAR_CODE('mute'),
    kMMgrPropName               = FOUR_CHAR_CODE('name'),
    kMMgrPropNextMovie          = FOUR_CHAR_CODE('next'),
    kMMgrPropNoProxyHosts       = FOUR_CHAR_CODE('nprx'),
    kMMgrPropNumber             = FOUR_CHAR_CODE('numb'),
    kMMgrPropOriginalFormat     = FOUR_CHAR_CODE(' fmt'),
    kMMgrPropOriginalSource     = FOUR_CHAR_CODE(' src'),
    kMMgrPropOwner              = FOUR_CHAR_CODE('ownr'),
    kMMgrPropPacketLoss         = FOUR_CHAR_CODE('loss'),
    kMMgrPropPanAngle           = FOUR_CHAR_CODE('pan '),
    kMMgrPropPanAngleRange      = FOUR_CHAR_CODE('panr'),
    kMMgrPropPlaySelectionOnly  = FOUR_CHAR_CODE('psel'),
    kMMgrPropPort               = FOUR_CHAR_CODE('port'),
    kMMgrPropPreferredLoc       = FOUR_CHAR_CODE('ploc'),
    kMMgrPropPreferredRate      = FOUR_CHAR_CODE('prte'),
    kMMgrPropPreferredSoundVolume = FOUR_CHAR_CODE('pvol'),
    kMMgrPropProperties         = FOUR_CHAR_CODE('prop'),
    kMMgrPropRate               = FOUR_CHAR_CODE('rate'),
    kMMgrPropReadOnly           = FOUR_CHAR_CODE('rwno'),
    kMMgrPropRectangle          = FOUR_CHAR_CODE('rect'),
    kMMgrPropRight              = FOUR_CHAR_CODE('rght'),
    kMMgrPropRTSPProxy          = FOUR_CHAR_CODE('rtsp'),
    kMMgrPropSaveDisabled       = FOUR_CHAR_CODE('save'),
    kMMgrPropScale              = FOUR_CHAR_CODE('scal'),
    kMMgrPropScaleMedia         = FOUR_CHAR_CODE('spsc'),
    kMMgrPropScript             = FOUR_CHAR_CODE('scrp'),
    kMMgrPropSeeAllFrames       = FOUR_CHAR_CODE('sall'),
    kMMgrPropShowHotSpots       = FOUR_CHAR_CODE('show'),
    kMMgrPropShowStreamStatsNames = FOUR_CHAR_CODE('snam'),
    kMMgrPropSize               = FOUR_CHAR_CODE('size'),
    kMMgrPropSOCKSProxy         = FOUR_CHAR_CODE('sock'),
    kMMgrPropSoftware           = FOUR_CHAR_CODE(' swr'),
    kMMgrPropSoundBalance       = FOUR_CHAR_CODE('sbal'),
    kMMgrPropSoundBass          = FOUR_CHAR_CODE('sbas'),
    kMMgrPropSoundOutputComponent = FOUR_CHAR_CODE('sout'),
    kMMgrPropSoundTreble        = FOUR_CHAR_CODE('strb'),
    kMMgrPropSoundVolume        = FOUR_CHAR_CODE('svol'),
    kMMgrPropStartSelection     = FOUR_CHAR_CODE('strs'),
    kMMgrPropStartTime          = FOUR_CHAR_CODE('strt'),
    kMMgrPropStatus             = FOUR_CHAR_CODE('stts'),
    kMMgrPropStreamStats        = FOUR_CHAR_CODE('stat'),
    kMMgrPropSuppressHotSpotNames = FOUR_CHAR_CODE('hnam'),
    kMMgrPropTargetMovieID      = FOUR_CHAR_CODE('tid '),
    kMMgrPropTargetMovieName    = FOUR_CHAR_CODE('tnam'),
    kMMgrPropText               = FOUR_CHAR_CODE('text'),
    kMMgrPropTiltAngle          = FOUR_CHAR_CODE('tlt '),
    kMMgrPropTiltAngleRange     = FOUR_CHAR_CODE('tltr'),
    kMMgrPropTime               = FOUR_CHAR_CODE('time'),
    kMMgrPropTimeScale          = FOUR_CHAR_CODE('tscl'),
    kMMgrPropTop                = FOUR_CHAR_CODE('top '),
    kMMgrPropTopLeft            = FOUR_CHAR_CODE('topl'),
    kMMgrPropType               = FOUR_CHAR_CODE('type'),
    kMMgrPropUDPTransport       = FOUR_CHAR_CODE('udpt'),
    kMMgrPropURL                = FOUR_CHAR_CODE('url '),
    kMMgrPropUseCustomCLUT      = FOUR_CHAR_CODE('clut'),
    kMMgrPropUserData           = FOUR_CHAR_CODE('udat'),
    kMMgrPropVariable           = FOUR_CHAR_CODE('var '),
    kMMgrPropVersion            = FOUR_CHAR_CODE('vers'),
    kMMgrPropVisible            = FOUR_CHAR_CODE('visi'),
    kMMgrPropWidth              = FOUR_CHAR_CODE('widt')
};


/*
   **
   == object specification ==
*/
enum {
                                                                /* ***/
                                                                /* obj->objClass*/
    kObjClassQuickTime          = FOUR_CHAR_CODE('cQtm'),
    kObjClassMovie              = FOUR_CHAR_CODE('cMoV'),
    kObjClassWindow             = FOUR_CHAR_CODE('cwnd'),
    kObjClassTrack              = FOUR_CHAR_CODE('ctrk'),
    kObjClassSprite             = FOUR_CHAR_CODE('cspr'),
    kObjClassHotSpot            = FOUR_CHAR_CODE('chsp'),
    kObjClassNode               = FOUR_CHAR_CODE('cnde'),
    kObjClassQTiObj             = FOUR_CHAR_CODE('cqti'),
    kObjClassController         = FOUR_CHAR_CODE('cctr'),
    kObjClassImage              = FOUR_CHAR_CODE('cimg'),
    kObjClassScene              = FOUR_CHAR_CODE('cscn'),
    kObjClassDataRefs           = FOUR_CHAR_CODE('data'),
    kObjUserData                = FOUR_CHAR_CODE('udta'),
    kObjClassEvery              = FOUR_CHAR_CODE('****'),       /* ** */
                                                                /* obj->objRefType*/
    kByIndex                    = FOUR_CHAR_CODE('indx'),       /* formAbsolutePosition */
    kByID                       = FOUR_CHAR_CODE('ID  '),       /* formUniqueID */
    kByName                     = FOUR_CHAR_CODE('name'),       /* formName */
    kByPosition                 = FOUR_CHAR_CODE('rele'),       /* formRelativePosition */
    kByType                     = FOUR_CHAR_CODE('type'),       /* ** */
                                                                /* special values when obj->objRefVal == kByIndex */
    kIndexFirst                 = 1,
    kIndexAll                   = -1,
    kIndexLast                  = -2,
    kIndexMiddle                = -3,
    kIndexAny                   = -4,                           /* ** */
                                                                /* values when obj->objRefVal == kByPosition */
    kIndexThis                  = FOUR_CHAR_CODE('this'),
    kIndexPrev                  = FOUR_CHAR_CODE('prev'),       /* kAEPrevious */
    kIndexNext                  = FOUR_CHAR_CODE('next'),       /* kAENext */
                                                                /* ** */
                                                                /* obj->objModifier "special" track types (non-media types) */
    kTrackTypeAny               = FOUR_CHAR_CODE('*trk'),
    kTrackTypeQTVR              = FOUR_CHAR_CODE('vrtr'),
    KTrackTypeMPEGVideo         = FOUR_CHAR_CODE('m1a '),
    KTrackTypeMPEGAudeo         = FOUR_CHAR_CODE('m1v '),
    kTrackTypeChapter           = FOUR_CHAR_CODE('chap'),       /* kTrackReferenceChapterList */
    kTrackTypeAudio             = FOUR_CHAR_CODE('ears'),       /* AudioMediaCharacteristic */
    kTrackTypeVisual            = FOUR_CHAR_CODE('eyes'),       /* VisualMediaCharacteristic */
    kTrackTypeStreaming         = FOUR_CHAR_CODE('strm')        /* kQTSStreamMediaType*/
};



/* == movie manager object spec structures ==*/

/*
   **
   == movie object spec ==
*/

struct ExternalObjectSpec {
    OSType                          objClass;                   /* type of object (movie, track, etc) */
    OSType                          objModifier;                /* object modifier (track type, etc) */
    OSType                          objRefType;                 /* how to interpret objRefVal (by name, by ID, etc) */
    SInt32                          objRefVal;                  /* object identifier (ID, index, offset to name, etc) */
};
typedef struct ExternalObjectSpec       ExternalObjectSpec;
typedef ExternalObjectSpec *            ExternalObjectSpecPtr;

struct MovieObjectSpec {
    SInt32                          count;
    ExternalObjectSpec              objRef[1];
};
typedef struct MovieObjectSpec          MovieObjectSpec;
typedef MovieObjectSpec *               MovieObjectSpecPtr;

struct MovieObjectRecord {
    SInt32                          data[1];
};
typedef struct MovieObjectRecord        MovieObjectRecord;

typedef MovieObjectRecord *             MovieObjectPtr;
/* == movie manager object reference structures ==*/

/*
   **
   == forward struct declarations for recursively-defined structures
*/
typedef struct MovieRef                 MovieRef;
typedef MovieRef *                      MovieRefPtr;
typedef struct TrackRef                 TrackRef;
typedef TrackRef *                      TrackRefPtr;
typedef struct SceneRef                 SceneRef;
typedef SceneRef *                      SceneRefPtr;
typedef struct SpriteRef                SpriteRef;
typedef SpriteRef *                     SpriteRefPtr;
typedef struct NodeRef                  NodeRef;
typedef NodeRef *                       NodeRefPtr;
typedef struct HotSpotRef               HotSpotRef;
typedef HotSpotRef *                    HotSpotRefPtr;
typedef struct GenericRef               GenericRef;
typedef GenericRef *                    GenericRefPtr;
/* header common to all movie object reference structures*/

struct GenericRef {
    GenericRefPtr                   parent;
    GenericRefPtr                   child;
    OSType                          objType;
};


struct MovieObjectRef {
    void *                          reserved;
    OSType                          targetType;
    SInt32                          objectCount;
    union {
        GenericRefPtr                   generic;
        MovieRefPtr                     movie;
        TrackRefPtr                     track;
        SceneRefPtr                     scene;
        SpriteRefPtr                    sprite;
        NodeRefPtr                      node;
        HotSpotRefPtr                   hotSpot;
    }                                 obj;
};
typedef struct MovieObjectRef           MovieObjectRef;
typedef MovieObjectRef *                MovieObjectRefPtr;

#define    MOVIE_OBJECT_REF_TYPE(ref)  ((ref) ? (ref)->obj.generic->objType : 0)


struct MovieRef {
    TrackRefPtr                     parent;                     /* track or NULL if is root movie*/
    TrackRefPtr                     child;                      /* child track */
    OSType                          objType;
    Movie                           movie;
    MovieController                 controller;
    DoMCActionUPP                   mcActionFilterUPP;          /* UPP of MovieMedia movie controller's filter proc*/
    void *                          mcActionRefcon;             /* refcon for same*/
};


struct TrackRef {
    MovieRefPtr                     parent;                     /* parent movie */
    GenericRefPtr                   child;                      /* child scene or node */
    OSType                          objType;
    UInt32                          flags;
    OSType                          trackType;
    Track                           track;
    SInt32                          id;
    SInt32                          index;
    Media                           media;

                                                                /* VR only data*/
    QTVRInstance                    vrInstance;
    QTAtomContainer                 vrWorld;
};


struct SceneRef {
    TrackRefPtr                     parent;                     /* parent track */
    SpriteRefPtr                    child;                      /* child sprite */
    OSType                          objType;
    SInt32                          index;                      /* ID is the same as index*/
    TimeValue                       startTime;
    TimeValue                       duration;
    SInt32                          currentSceneIndex;
};


struct SpriteRef {
    SceneRefPtr                     parent;
    GenericRefPtr                   child;                      /* NULL, sprites have no children */
    OSType                          objType;
    MediaHandler                    handler;
    QTAtomID                        id;
    SInt16                          index;
};


struct NodeRef {
    TrackRefPtr                     parent;                     /* parent track */
    HotSpotRefPtr                   child;                      /* NULL, */
    OSType                          objType;
    QTAtomContainer                 info;
    QTAtomID                        id;
    SInt16                          index;
    UInt16                          flags;
};


struct HotSpotRef {
    NodeRefPtr                      parent;                     /* parent node */
    GenericRefPtr                   child;                      /* NULL, hotspots have no children */
    OSType                          objType;
    QTAtomID                        id;
    SInt16                          index;
};

enum {
    kReserved1                  = (1L << 0),
    kReserved2                  = (1L << 1),
    kHasAudioCharacteristics    = (1L << 2),
    kHasVisualCharacteristics   = (1L << 3),
    kHasTextCharacteristics     = (1L << 4),
    kCanSendVideoCharacteristics = (1L << 5),
    kProvidesActionsCharacteristics = (1L << 6),
    kHasNonLinearCharacteristics = (1L << 7),
    kHasCanStepCharacteristics  = (1L << 8),
    kHasNoDurationCharacteristics = (1L << 9),                  /* not really "characteristics", but hey...*/
    kHasSpriteCharacteristics   = (1L << 10),
    kHasVRCharacteristics       = (1L << 11),
    kHasStreamingCharacteristics = (1L << 12),
    kHasMovieCharacteristics    = (1L << 13)
};



/* == data types ==*/

enum {
    kPtrToDataType              = (1L << 31),
    dataType_Boolean            = FOUR_CHAR_CODE('bool'),
    dataType_BooleanPtr         = (dataType_Boolean | kPtrToDataType),
    dataType_UInt8              = FOUR_CHAR_CODE('ui8 '),
    dataType_UInt8Ptr           = (dataType_Boolean | kPtrToDataType),
    dataType_Int8               = FOUR_CHAR_CODE('i8  '),
    dataType_Int8Ptr            = (dataType_Int8 | kPtrToDataType),
    dataType_UInt16             = FOUR_CHAR_CODE('ui16'),
    dataType_UInt16Ptr          = (dataType_UInt16 | kPtrToDataType),
    dataType_Int16              = FOUR_CHAR_CODE('i16 '),
    dataType_Int16Ptr           = (dataType_Int16 | kPtrToDataType),
    dataType_UInt32             = FOUR_CHAR_CODE('ui32'),
    dataType_UInt32Ptr          = (dataType_UInt32 | kPtrToDataType),
    dataType_Int32              = FOUR_CHAR_CODE('i32 '),
    dataType_Int32Ptr           = (dataType_Int32 | kPtrToDataType),
    dataType_FourCharCode       = FOUR_CHAR_CODE('code'),
    dataType_FourCharCodePtr    = (dataType_FourCharCode | kPtrToDataType),
    dataType_RGBColor           = FOUR_CHAR_CODE('rgba'),
    dataType_RGBColorPtr        = (dataType_RGBColor | kPtrToDataType),
    dataType_Fixed              = FOUR_CHAR_CODE('fix '),
    dataType_FixedPtr           = (dataType_Fixed | kPtrToDataType),
    dataType_FloatSingle        = FOUR_CHAR_CODE('fsin'),
    dataType_FloatSinglePtr     = (dataType_FloatSingle | kPtrToDataType),
    dataType_FloatDouble        = FOUR_CHAR_CODE('fdbl'),
    dataType_FloatDoublePtr     = (dataType_FloatDouble | kPtrToDataType),
    dataType_QDPoint            = FOUR_CHAR_CODE('poin'),
    dataType_QDPointPtr         = (dataType_QDPoint | kPtrToDataType),
    dataType_FixedPoint         = FOUR_CHAR_CODE('fixp'),
    dataType_FixedPointPtr      = (dataType_FixedPoint | kPtrToDataType),
    dataType_QDRect             = FOUR_CHAR_CODE('rect'),
    dataType_QDRectPtr          = (dataType_QDRect | kPtrToDataType),
    dataType_QDRegion           = FOUR_CHAR_CODE('rgn '),
    dataType_QDRegionPtr        = (dataType_QDRegion | kPtrToDataType),
    dataType_PString            = FOUR_CHAR_CODE('pstr'),
    dataType_PStringPtr         = (dataType_PString | kPtrToDataType),
    dataType_CString            = FOUR_CHAR_CODE('cstr'),
    dataType_CStringPtr         = (dataType_CString | kPtrToDataType),
    dataType_CharBuff           = FOUR_CHAR_CODE('cbuf'),
    dataType_CharBuffPtr        = (dataType_CharBuff | kPtrToDataType),
    dataType_TextHandle         = FOUR_CHAR_CODE('text'),
    dataType_TextHandlePtr      = (dataType_TextHandle | kPtrToDataType),
    dataType_Matrix             = FOUR_CHAR_CODE('matr'),
    dataType_MatrixPtr          = (dataType_Matrix | kPtrToDataType),
    dataType_Alias              = FOUR_CHAR_CODE('alis'),
    dataType_AliasPtr           = (dataType_Alias | kPtrToDataType),
    dataType_MacDate            = FOUR_CHAR_CODE('date'),
    dataType_MacDatePtr         = (dataType_MacDate | kPtrToDataType),
    dataType_TimeValue          = FOUR_CHAR_CODE('time'),
    dataType_TimeValuePtr       = (dataType_TimeValue | kPtrToDataType),
    dataType_BCDVersion         = FOUR_CHAR_CODE('vers'),
    dataType_BCDVersionPtr      = (dataType_BCDVersion | kPtrToDataType),
    dataType_Opaque             = FOUR_CHAR_CODE('void'),
    dataType_OpaquePtr          = (dataType_Opaque | kPtrToDataType),
    dataType_GraphicsMode       = FOUR_CHAR_CODE('gmod'),
    dataType_GraphicsModePtr    = (dataType_GraphicsMode | kPtrToDataType),
    dataType_GraphicsModeRecord = FOUR_CHAR_CODE('gmrc'),
    dataType_GraphicsModeRecordPtr = (dataType_GraphicsModeRecord | kPtrToDataType),
    dataType_QTSProxyPref       = FOUR_CHAR_CODE('prxy'),
    dataType_QTSProxyPrefPtr    = (dataType_QTSProxyPref | kPtrToDataType),
    dataType_QTSTransportPref   = FOUR_CHAR_CODE('trns'),
    dataType_QTSTransportPrefPtr = (dataType_QTSTransportPref | kPtrToDataType),
    dataType_QTSNoProxyPref     = FOUR_CHAR_CODE('nprx'),
    dataType_QTSNoProxyPrefPtr  = (dataType_QTSNoProxyPref | kPtrToDataType),
    dataType_FindOffset         = FOUR_CHAR_CODE('find'),
    dataType_FindOffsetPtr      = (dataType_FindOffset | kPtrToDataType)
};


/* == movie manager notification flags == */
enum {
    kNotifyBeforeMessageSend    = (1L << 0),                    /* notify before a message is sent to an object*/
    kNotifyAfterMessageSend     = (1L << 1),                    /* notify after a message was successfully sent to an object*/
    kNotifyOnHighLevelEvent     = (1L << 2),                    /* notify before high level events*/
    kNotifyOnMouseEvent         = (1L << 3),                    /* notify before mouse up/down/enter/exit/trigger events*/
    kNotifyOnMCActionEvent      = (1L << 4)                     /* notify before looking at mcAction events*/
};


/* == movie manager notification types ==*/
enum {
                                                                /* ***/
                                                                /* notification types*/
    kNotifyTypeHighLevelEvent   = FOUR_CHAR_CODE('high'),       /* hight level event (HighLevelEventRecord *)*/
    kNotifyTypeMCAction         = FOUR_CHAR_CODE('mcac'),       /* mcAction notification (MMgrControllerEventRecord *)*/
    kNotifyMouseEvent           = FOUR_CHAR_CODE('mous'),       /* mouse event*/
    kNotifyTypePreMessageSend   = FOUR_CHAR_CODE('prms'),       /* message send (MovieObjectRef *)*/
    kNotifyTypePostMessageSend  = FOUR_CHAR_CODE('poms')        /* */
};

/* == movie manager notification IDs ==*/
enum {
                                                                /* ***/
                                                                /* "high level" events*/
    kNotifyOpenMovie            = FOUR_CHAR_CODE('open'),
    kNotifyCloseMovie           = FOUR_CHAR_CODE('clos'),
    kNotifyEnteringNode         = FOUR_CHAR_CODE('vren'),       /* entering node, data = new node ID (SInt32)*/
    kNotifyLeavingNode          = FOUR_CHAR_CODE('vrlv'),       /* leaving VR node, data = leave node data (LeaveNodeRecord *)*/
                                                                /* ?? */
                                                                /* ????? does it make sense to send these notifications? User could use the notification to set the*/
                                                                /* ?????  movieID/movieName property, which would then be returned...*/
                                                                /*@@@@@   kGetExternalMovieID,*/
                                                                /*@@@@@   kGetExternalMovieName,*/
                                                                /* ?????*/
    kNotifyEdited               = FOUR_CHAR_CODE('edit'),       /* movie has been edited*/
    kNotifyLinkToURL            = FOUR_CHAR_CODE('lurl'),       /* the movie wants to link to a url, data = URL (handle of C string)*/
    kNotifyRateChanged          = FOUR_CHAR_CODE('rate'),       /* the movie's rate has changed, data = rate (Fixed)*/
    kNotifyTimeChanged          = FOUR_CHAR_CODE('ctim'),       /* the movie's time has changed, data = new time (SInt32)*/
    kNotifyVolumeChanged        = FOUR_CHAR_CODE('cvol'),       /* the movie's volume has changed, data = new volume (SInt32)*/
    kNotifyShowStatusString     = FOUR_CHAR_CODE('stts'),       /* a movie status message, data = status string (handle of C string)*/
    kNotifyShowMessageString    = FOUR_CHAR_CODE('smsg'),       /* a sprite message string (mcActionShowMessageString), data = string (StringPtr)*/
    kNotifySpriteCustomAction   = FOUR_CHAR_CODE('scst'),       /* a sprite custom app wired event (kActionApplicationNumberAndString), data = params (SpriteDebugMessage *)*/
    kNotifySpriteDebugStr       = FOUR_CHAR_CODE('sdbg'),       /* a sprite debugstr wired event (kActionDebugStr), param = string (stringPtr)*/
                                                                /* ***/
                                                                /* mouse event notification*/
    kNotifyMouseEnter           = FOUR_CHAR_CODE('entr'),       /* mouse has entered a movie object, data = object ID (SInt32)*/
    kNotifyMouseWithin          = FOUR_CHAR_CODE('mwin'),       /* mouse is still within a movie object, data = object ID (SInt32)*/
    kNotifyMouseExit            = FOUR_CHAR_CODE('exit'),       /* mouse has left the building, data = object ID (SInt32)*/
    kNotifyMouseDown            = FOUR_CHAR_CODE('clik'),       /* mouse clicked within a movie object, data = object ID (SInt32)*/
    kNotifyMouseStillDown       = FOUR_CHAR_CODE('stld'),       /* mouse is still down within a movie object, data = object ID (SInt32)*/
    kNotifyMouseTrigger         = FOUR_CHAR_CODE('trig'),       /* mouse released within the movie object it went down in, data = object ID (SInt32)*/
    kNotifyMouseUp              = FOUR_CHAR_CODE('cend')        /* mouse released outside of the movie object it went down in, data = object ID (SInt32)*/
};


/*
   **
   == movie manager event record == 
*/

struct MMgrEventRecord {
    OSType                          objClass;                   /* class of event target object*/
    SInt32                          when;                       /* ticks at time of event*/
    Point                           where;                      /* local mouse loc at time of event*/
    UInt32                          flags;
};
typedef struct MMgrEventRecord          MMgrEventRecord;
typedef MMgrEventRecord *               MMgrEventRecordPtr;
/*
   **
   == structure passed to notification proc for a controller filter event notification ==
*/

struct MMgrControllerEventRecord {
    void *                          params;                     /* param normally passed to controller filter for this action*/
    SInt16                          action;                     /* action*/
};
typedef struct MMgrControllerEventRecord MMgrControllerEventRecord;
typedef MMgrControllerEventRecord *     MMgrControllerEventRecordPtr;
/*
   **
   == structure passed to notification proc for pre/post message send ==
*/

struct MMgrMessageNotificationRecord {
    MovieObjectRef *                targetObject;               /* resolved movie object target*/
    MessageClass                    msgClass;                   /* message class*/
    MessageID                       msgID;                      /* message ID*/
    PropertyParams *                params;                     /* message params*/
};
typedef struct MMgrMessageNotificationRecord MMgrMessageNotificationRecord;
typedef MMgrMessageNotificationRecord * MMgrMessageNotificationRecordPtr;
/*
   **
   == movie manager hight level event notification record == 
*/

struct HighLevelEventRecord {
    OSType                          eventID;
    void *                          data;                       /* event specific params*/
};
typedef struct HighLevelEventRecord     HighLevelEventRecord;

/*
   **
   == structure passed to notification proc for a sprite kNotifySpriteCustomAction (kActionApplicationNumberAndString) notification ==
*/

struct SpriteDebugMessage {
    SInt32                          number;                     /* sprite action param 1, a long*/
    Handle                          stringHandle;               /* sprite action param 2, a Pascal string*/
};
typedef struct SpriteDebugMessage       SpriteDebugMessage;
typedef SpriteDebugMessage *            SpriteDebugMessagePtr;
/*
   **
   == structure passed to notification proc upon leaving a VR node ==
*/

struct LeaveNodeRecord {
    UInt32                          fromNodeID;                 /* the id of the node we're about to leave*/
    UInt32                          toNodeID;                   /* the id of the node we're about to enter*/
};
typedef struct LeaveNodeRecord          LeaveNodeRecord;
typedef LeaveNodeRecord *               LeaveNodeRecordPtr;
/* == movie manager errors == */
enum {
    errInvalidTrackType         = -2300,
    errCurrentSceneOnly         = -2301,
    errNoSuchScene              = -2302,
    errCantCoerceValue          = -2303,
    errPropNotSupported         = -2304,
    errCmdNotSupported          = -2305,
    errParamCountOutOfBounds    = -2306,
    errNotImplementedYet        = -2307,
    errBadLanguageForThisMovie  = -2308,
    errReadOnly                 = -2319,
    errInvalidTimeForThisMovie  = -2310,
    errNoExporterSet            = -2311,
    errMovieNotEditable         = -2312
};

/*
   **
   == messaging callback ==
*/

typedef CALLBACK_API( OSErr , MovieManagerFilterProcPtr )(Movie movie, MovieController mc, OSType eventType, MMgrEventRecord *evt, void *evtData, void *refCon);
typedef STACK_UPP_TYPE(MovieManagerFilterProcPtr)               MovieManagerFilterUPP;
/* == selector ranges == */
enum {
    selectorRange_MooVMgr       = 0x0000
};


enum {
    kMovieManagerCompType       = FOUR_CHAR_CODE('mngr'),
    kMovieManagerSubType        = FOUR_CHAR_CODE('moov')
};

enum {
    kMooVMgrSetMovieSelect      = selectorRange_MooVMgr + 1,
    kMooVMgrGetMovieSelect      = 0x0002,
    kMooVMgrSetMovieControllerSelect = 0x0003,
    kMooVMgrGetMovieControllerSelect = 0x0004,
    kMooVMgrObjectExistsSelect  = 0x0005,
    kMooVMgrCountObjectsSelect  = 0x0006,
    kMooVMgrSendMessageToObjectSelect = 0x0007,
    kMooVMgrMapNameToIDSelect   = 0x0008,
    kMooVMgrCoerceValueToTypeSelect = 0x0009,
    kMooVMgrSetNotificationFlagsSelect = 0x000A,
    kMooVMgrGetNotificationFlagsSelect = 0x000B,
    kMooVMgrSetNotificationProcSelect = 0x000C,
    kMooVMgrGetObjectProperty   = 0x000D,
    kMooVMgrSetObjectProperty   = 0x000E
};



/* = standard component stuff = */

#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
MovieManagerOpen                (MovieManagerComponent  moovMgr,
                                 ComponentInstance      self)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0xFFFF, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerClose               (MovieManagerComponent  moovMgr,
                                 ComponentInstance      self)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0xFFFE, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerCanDo               (MovieManagerComponent  moovMgr,
                                 SInt16                 ftnNumber)                          FIVEWORDINLINE(0x2F3C, 0x0002, 0xFFFD, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerVersion             (MovieManagerComponent  moovMgr)                            FIVEWORDINLINE(0x2F3C, 0x0000, 0xFFFC, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerTarget              (MovieManagerComponent  moovMgr,
                                 ComponentInstance      target)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0xFFFA, 0x7000, 0xA82A);


/* = initialization, hierarchy, editing = */

EXTERN_API( ComponentResult )
MovieManagerSetMovie            (MovieManagerComponent  moovMgr,
                                 Movie                  movie)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerGetMovie            (MovieManagerComponent  moovMgr,
                                 Movie *                movie)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerSetMovieController  (MovieManagerComponent  moovMgr,
                                 MovieController        controller)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerGetMovieController  (MovieManagerComponent  moovMgr,
                                 MovieController *      controller)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0004, 0x7000, 0xA82A);


/* = movie objects = */

EXTERN_API( ComponentResult )
MovieManagerObjectExists        (MovieManagerComponent  moovMgr,
                                 MovieObjectSpec *      objectSpec,
                                 Boolean *              exists)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerCountObjects        (MovieManagerComponent  moovMgr,
                                 OSType                 countClass,
                                 MovieObjectSpec *      objectSpec,
                                 SInt32 *               count)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0006, 0x7000, 0xA82A);


/* = property/message = */

EXTERN_API( ComponentResult )
MovieManagerSendMessageToObject (MovieManagerComponent  moovMgr,
                                 MovieObjectSpec *      objectSpec,
                                 MessageClass           msgClass,
                                 MessageID              msgID,
                                 PropertyParams *       params)                             FIVEWORDINLINE(0x2F3C, 0x0010, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerMapNameToID         (MovieManagerComponent  moovMgr,
                                 StringPtr              name,
                                 OSType *               id)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0008, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerCoerceValueToType   (MovieManagerComponent  moovMgr,
                                 PropArgU *             dst,
                                 ValueFormat            dstType,
                                 PropArgU *             src)                                FIVEWORDINLINE(0x2F3C, 0x000C, 0x0009, 0x7000, 0xA82A);

/* = notification */
EXTERN_API( ComponentResult )
MovieManagerSetNotificationFlags (MovieManagerComponent  moovMgr,
                                 UInt32                 flags,
                                 UInt32                 flagsMask)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x000A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerGetNotificationFlags (MovieManagerComponent  moovMgr,
                                 UInt32 *               flags)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerSetNotificationProc (MovieManagerComponent  moovMgr,
                                 MovieManagerFilterUPP  callback,
                                 const void *           refCon,
                                 UInt32                 flags)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x000C, 0x7000, 0xA82A);


/* = obsolete, to be removed after OS X beta*/
EXTERN_API( ComponentResult )
MovieManagerGetObjectProperty   (MovieManagerComponent  moovMgr,
                                 MovieObjectSpec *      objectSpec,
                                 PropertyID             propID,
                                 PropertyParams *       params)                             FIVEWORDINLINE(0x2F3C, 0x000C, 0x000D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MovieManagerSetObjectProperty   (MovieManagerComponent  moovMgr,
                                 MovieObjectSpec *      objectSpec,
                                 PropertyID             propID,
                                 PropertyParams *       params)                             FIVEWORDINLINE(0x2F3C, 0x000C, 0x000E, 0x7000, 0xA82A);




/* selectors for component calls */
enum {
    kMovieManagerOpenSelect                    = -1,
    kMovieManagerCloseSelect                   = -2,
    kMovieManagerCanDoSelect                   = -3,
    kMovieManagerVersionSelect                 = -4,
    kMovieManagerTargetSelect                  = -6,
    kMovieManagerSetMovieSelect                = 0x0001,
    kMovieManagerGetMovieSelect                = 0x0002,
    kMovieManagerSetMovieControllerSelect      = 0x0003,
    kMovieManagerGetMovieControllerSelect      = 0x0004,
    kMovieManagerObjectExistsSelect            = 0x0005,
    kMovieManagerCountObjectsSelect            = 0x0006,
    kMovieManagerSendMessageToObjectSelect     = 0x0007,
    kMovieManagerMapNameToIDSelect             = 0x0008,
    kMovieManagerCoerceValueToTypeSelect       = 0x0009,
    kMovieManagerSetNotificationFlagsSelect    = 0x000A,
    kMovieManagerGetNotificationFlagsSelect    = 0x000B,
    kMovieManagerSetNotificationProcSelect     = 0x000C,
    kMovieManagerGetObjectPropertySelect       = 0x000D,
    kMovieManagerSetObjectPropertySelect       = 0x000E
};
#endif  /* CALL_NOT_IN_CARBON */

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(MovieManagerFilterUPP)
    NewMovieManagerFilterUPP       (MovieManagerFilterProcPtr userRoutine);

    EXTERN_API(void)
    DisposeMovieManagerFilterUPP    (MovieManagerFilterUPP  userUPP);

    EXTERN_API(OSErr)
    InvokeMovieManagerFilterUPP    (Movie                   movie,
                                    MovieController         mc,
                                    OSType                  eventType,
                                    MMgrEventRecord *       evt,
                                    void *                  evtData,
                                    void *                  refCon,
                                    MovieManagerFilterUPP   userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppMovieManagerFilterProcInfo = 0x0003FFE0 };            /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewMovieManagerFilterUPP(userRoutine)                   (MovieManagerFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMovieManagerFilterProcInfo, GetCurrentArchitecture())
    #define DisposeMovieManagerFilterUPP(userUPP)                   DisposeRoutineDescriptor(userUPP)
    #define InvokeMovieManagerFilterUPP(movie, mc, eventType, evt, evtData, refCon, userUPP)  (OSErr)CALL_SIX_PARAMETER_UPP((userUPP), uppMovieManagerFilterProcInfo, (movie), (mc), (eventType), (evt), (evtData), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewMovieManagerFilterProc(userRoutine)                  NewMovieManagerFilterUPP(userRoutine)
#define CallMovieManagerFilterProc(userRoutine, movie, mc, eventType, evt, evtData, refCon) InvokeMovieManagerFilterUPP(movie, mc, eventType, evt, evtData, refCon, userRoutine)

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

#endif /* __MOVIEMANAGER__ */

