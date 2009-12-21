/*
     File:       SpeechRecognition.h
 
     Contains:   Apple Speech Recognition Toolbox Interfaces.
 
     Version:    Technology: PlainTalk 1.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1992-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __SPEECHRECOGNITION__
#define __SPEECHRECOGNITION__

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

/* Error Codes [Speech recognition gets -5100 through -5199] */
enum {
    kSRNotAvailable             = -5100,                        /* the service requested is not avail or applicable */
    kSRInternalError            = -5101,                        /* a system internal or hardware error condition */
    kSRComponentNotFound        = -5102,                        /* a needed system resource was not located */
    kSROutOfMemory              = -5103,                        /* an out of memory error occurred in the toolbox memory space */
    kSRNotASpeechObject         = -5104,                        /* the object specified is no longer or never was valid */
    kSRBadParameter             = -5105,                        /* an invalid parameter was specified */
    kSRParamOutOfRange          = -5106,                        /* when we say 0-100, don't pass in 101. */
    kSRBadSelector              = -5107,                        /* an unrecognized selector was specified */
    kSRBufferTooSmall           = -5108,                        /* returned from attribute access functions */
    kSRNotARecSystem            = -5109,                        /* the object used was not a SRRecognitionSystem */
    kSRFeedbackNotAvail         = -5110,                        /* there is no feedback window associated with SRRecognizer */
    kSRCantSetProperty          = -5111,                        /* a non-settable property was specified */
    kSRCantGetProperty          = -5112,                        /* a non-gettable property was specified */
    kSRCantSetDuringRecognition = -5113,                        /* the property can't be set while recognition is in progress -- do before or between utterances. */
    kSRAlreadyListening         = -5114,                        /* in response to SRStartListening */
    kSRNotListeningState        = -5115,                        /* in response to SRStopListening */
    kSRModelMismatch            = -5116,                        /* no acoustical models are avail to match request */
    kSRNoClientLanguageModel    = -5117,                        /* trying to access a non-specified SRLanguageModel */
    kSRNoPendingUtterances      = -5118,                        /* nothing to continue search on */
    kSRRecognitionCanceled      = -5119,                        /* an abort error occurred during search */
    kSRRecognitionDone          = -5120,                        /* search has finished, but nothing was recognized */
    kSROtherRecAlreadyModal     = -5121,                        /* another recognizer is modal at the moment, so can't set this recognizer's kSRBlockModally property right now */
    kSRHasNoSubItems            = -5122,                        /* SRCountItems or related routine was called on an object without subelements -- e.g. a word -- rather than phrase, path, or LM. */
    kSRSubItemNotFound          = -5123,                        /* returned when accessing a non-existent sub item of a container */
    kSRLanguageModelTooBig      = -5124,                        /* Cant build language models so big */
    kSRAlreadyReleased          = -5125,                        /* this object has already been released before */
    kSRAlreadyFinished          = -5126,                        /* the language model can't be finished twice */
    kSRWordNotFound             = -5127,                        /* the spelling couldn't be found in lookup(s) */
    kSRNotFinishedWithRejection = -5128,                        /* property not found because the LMObj is not finished with rejection */
    kSRExpansionTooDeep         = -5129,                        /* Language model is left recursive or is embedded too many levels */
    kSRTooManyElements          = -5130,                        /* Too many elements added to phrase or path or other langauge model object */
    kSRCantAdd                  = -5131,                        /* Can't add given type of object to the base SRLanguageObject (e.g.in SRAddLanguageObject)   */
    kSRSndInSourceDisconnected  = -5132,                        /* Sound input source is disconnected */
    kSRCantReadLanguageObject   = -5133,                        /* An error while trying to create new Language object from file or pointer -- possibly bad format */
                                                                /* non-release debugging error codes are included here */
    kSRNotImplementedYet        = -5199                         /* you'd better wait for this feature in a future release */
};


/* Type Definitions */
typedef struct OpaqueSRSpeechObject*    SRSpeechObject;
typedef SRSpeechObject                  SRRecognitionSystem;
typedef SRSpeechObject                  SRRecognizer;
typedef SRSpeechObject                  SRSpeechSource;
typedef SRSpeechSource                  SRRecognitionResult;
typedef SRSpeechObject                  SRLanguageObject;
typedef SRLanguageObject                SRLanguageModel;
typedef SRLanguageObject                SRPath;
typedef SRLanguageObject                SRPhrase;
typedef SRLanguageObject                SRWord;
/* between 0 and 100 */
typedef unsigned short                  SRSpeedSetting;
/* between 0 and 100 */
typedef unsigned short                  SRRejectionLevel;
/* When an event occurs, the user supplied proc will be called with a pointer   */
/*  to the param passed in and a flag to indicate conditions such               */
/*  as interrupt time or system background time.                                */

struct SRCallBackStruct {
    long                            what;                       /* one of notification flags */
    long                            message;                    /* contains SRRecognitionResult id */
    SRRecognizer                    instance;                   /* ID of recognizer being notified */
    OSErr                           status;                     /* result status of last search */
    short                           flags;                      /* non-zero if occurs during interrupt */
    long                            refCon;                     /* user defined - set from SRCallBackParam */
};
typedef struct SRCallBackStruct         SRCallBackStruct;
/* Call back procedure definition */
typedef CALLBACK_API( void , SRCallBackProcPtr )(SRCallBackStruct *param);
typedef STACK_UPP_TYPE(SRCallBackProcPtr)                       SRCallBackUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(SRCallBackUPP)
    NewSRCallBackUPP               (SRCallBackProcPtr       userRoutine);

    EXTERN_API(void)
    DisposeSRCallBackUPP           (SRCallBackUPP           userUPP);

    EXTERN_API(void)
    InvokeSRCallBackUPP            (SRCallBackStruct *      param,
                                    SRCallBackUPP           userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppSRCallBackProcInfo = 0x000000C0 };                    /* pascal no_return_value Func(4_bytes) */
    #define NewSRCallBackUPP(userRoutine)                           (SRCallBackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSRCallBackProcInfo, GetCurrentArchitecture())
    #define DisposeSRCallBackUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define InvokeSRCallBackUPP(param, userUPP)                     CALL_ONE_PARAMETER_UPP((userUPP), uppSRCallBackProcInfo, (param))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewSRCallBackProc(userRoutine)                          NewSRCallBackUPP(userRoutine)
#define CallSRCallBackProc(userRoutine, param)                  InvokeSRCallBackUPP(param, userRoutine)

struct SRCallBackParam {
    SRCallBackUPP                   callBack;
    long                            refCon;
};
typedef struct SRCallBackParam          SRCallBackParam;
/* Recognition System Types */
enum {
    kSRDefaultRecognitionSystemID = 0
};

/* Recognition System Properties */
enum {
    kSRFeedbackAndListeningModes = FOUR_CHAR_CODE('fbwn'),      /* short: one of kSRNoFeedbackHasListenModes, kSRHasFeedbackHasListenModes, kSRNoFeedbackNoListenModes */
    kSRRejectedWord             = FOUR_CHAR_CODE('rejq'),       /* the SRWord used to represent a rejection */
    kSRCleanupOnClientExit      = FOUR_CHAR_CODE('clup')        /* Boolean: Default is true. The rec system and everything it owns is disposed when the client application quits */
};

enum {
    kSRNoFeedbackNoListenModes  = 0,                            /* next allocated recognizer has no feedback window and doesn't use listening modes   */
    kSRHasFeedbackHasListenModes = 1,                           /* next allocated recognizer has feedback window and uses listening modes          */
    kSRNoFeedbackHasListenModes = 2                             /* next allocated recognizer has no feedback window but does use listening modes  */
};

/* Speech Source Types */
enum {
    kSRDefaultSpeechSource      = 0,
    kSRLiveDesktopSpeechSource  = FOUR_CHAR_CODE('dklv'),       /* live desktop sound input */
    kSRCanned22kHzSpeechSource  = FOUR_CHAR_CODE('ca22')        /* AIFF file based 16 bit, 22.050 KHz sound input */
};

/* Notification via Apple Event or Callback */
/* Notification Flags */
enum {
    kSRNotifyRecognitionBeginning = 1L << 0,                    /* recognition can begin. client must now call SRContinueRecognition or SRCancelRecognition */
    kSRNotifyRecognitionDone    = 1L << 1                       /* recognition has terminated. result (if any) is available. */
};

/* Apple Event selectors */
/* AppleEvent message class  */
enum {
    kAESpeechSuite              = FOUR_CHAR_CODE('sprc')
};

/* AppleEvent message event ids */
enum {
    kAESpeechDone               = FOUR_CHAR_CODE('srsd'),
    kAESpeechDetected           = FOUR_CHAR_CODE('srbd')
};

/* AppleEvent Parameter ids */
enum {
    keySRRecognizer             = FOUR_CHAR_CODE('krec'),
    keySRSpeechResult           = FOUR_CHAR_CODE('kspr'),
    keySRSpeechStatus           = FOUR_CHAR_CODE('ksst')
};

/* AppleEvent Parameter types */
enum {
    typeSRRecognizer            = FOUR_CHAR_CODE('trec'),
    typeSRSpeechResult          = FOUR_CHAR_CODE('tspr')
};


/* SRRecognizer Properties */
enum {
    kSRNotificationParam        = FOUR_CHAR_CODE('noti'),       /* see notification flags below */
    kSRCallBackParam            = FOUR_CHAR_CODE('call'),       /* type SRCallBackParam */
    kSRSearchStatusParam        = FOUR_CHAR_CODE('stat'),       /* see status flags below */
    kSRAutoFinishingParam       = FOUR_CHAR_CODE('afin'),       /* automatic finishing applied on LM for search */
    kSRForegroundOnly           = FOUR_CHAR_CODE('fgon'),       /* Boolean. Default is true. If true, client recognizer only active when in foreground.   */
    kSRBlockBackground          = FOUR_CHAR_CODE('blbg'),       /* Boolean. Default is false. If true, when client recognizer in foreground, rest of LMs are inactive.    */
    kSRBlockModally             = FOUR_CHAR_CODE('blmd'),       /* Boolean. Default is false. When true, this client's LM is only active LM; all other LMs are inactive. Be nice, don't be modal for long periods! */
    kSRWantsResultTextDrawn     = FOUR_CHAR_CODE('txfb'),       /* Boolean. Default is true. If true, search results are posted to Feedback window */
    kSRWantsAutoFBGestures      = FOUR_CHAR_CODE('dfbr'),       /* Boolean. Default is true. If true, client needn't call SRProcessBegin/End to get default feedback behavior */
    kSRSoundInVolume            = FOUR_CHAR_CODE('volu'),       /* short in [0..100] log scaled sound input power. Can't set this property */
    kSRReadAudioFSSpec          = FOUR_CHAR_CODE('aurd'),       /* *FSSpec. Specify FSSpec where raw audio is to be read (AIFF format) using kSRCanned22kHzSpeechSource. Reads until EOF */
    kSRCancelOnSoundOut         = FOUR_CHAR_CODE('caso'),       /* Boolean: Default is true.  If any sound is played out during utterance, recognition is aborted. */
    kSRSpeedVsAccuracyParam     = FOUR_CHAR_CODE('sped')        /* SRSpeedSetting between 0 and 100 */
};

/* 0 means more accurate but slower. */
/* 100 means (much) less accurate but faster. */
enum {
    kSRUseToggleListen          = 0,                            /* listen key modes */
    kSRUsePushToTalk            = 1
};

enum {
    kSRListenKeyMode            = FOUR_CHAR_CODE('lkmd'),       /* short: either kSRUseToggleListen or kSRUsePushToTalk */
    kSRListenKeyCombo           = FOUR_CHAR_CODE('lkey'),       /* short: Push-To-Talk key combination; high byte is high byte of event->modifiers, the low byte is the keycode from event->message */
    kSRListenKeyName            = FOUR_CHAR_CODE('lnam'),       /* Str63: string representing ListenKeyCombo */
    kSRKeyWord                  = FOUR_CHAR_CODE('kwrd'),       /* Str255: keyword preceding spoken commands in kSRUseToggleListen mode */
    kSRKeyExpected              = FOUR_CHAR_CODE('kexp')        /* Boolean: Must the PTT key be depressed or the key word spoken before recognition can occur? */
};

/* Operational Status Flags */
enum {
    kSRIdleRecognizer           = 1L << 0,                      /* engine is not active */
    kSRSearchInProgress         = 1L << 1,                      /* search is in progress */
    kSRSearchWaitForAllClients  = 1L << 2,                      /* search is suspended waiting on all clients' input */
    kSRMustCancelSearch         = 1L << 3,                      /* something has occurred (sound played, non-speech detected) requiring the search to abort */
    kSRPendingSearch            = 1L << 4                       /* we're about to start searching */
};

/* Recognition Result Properties */
enum {
    kSRTEXTFormat               = FOUR_CHAR_CODE('TEXT'),       /* raw text in user supplied memory */
    kSRPhraseFormat             = FOUR_CHAR_CODE('lmph'),       /* SRPhrase containing result words */
    kSRPathFormat               = FOUR_CHAR_CODE('lmpt'),       /* SRPath containing result phrases or words */
    kSRLanguageModelFormat      = FOUR_CHAR_CODE('lmfm')        /* top level SRLanguageModel for post parse */
};

/* SRLanguageObject Family Properties */
enum {
    kSRSpelling                 = FOUR_CHAR_CODE('spel'),       /* spelling of a SRWord or SRPhrase or SRPath, or name of a SRLanguageModel */
    kSRLMObjType                = FOUR_CHAR_CODE('lmtp'),       /* Returns one of SRLanguageObject Types listed below */
    kSRRefCon                   = FOUR_CHAR_CODE('refc'),       /* 4 bytes of user storage */
    kSROptional                 = FOUR_CHAR_CODE('optl'),       /* Boolean -- true if SRLanguageObject is optional    */
    kSREnabled                  = FOUR_CHAR_CODE('enbl'),       /* Boolean -- true if SRLanguageObject enabled */
    kSRRepeatable               = FOUR_CHAR_CODE('rptb'),       /* Boolean -- true if SRLanguageObject is repeatable */
    kSRRejectable               = FOUR_CHAR_CODE('rjbl'),       /* Boolean -- true if SRLanguageObject is rejectable (Recognition System's kSRRejectedWord */
                                                                /*       object can be returned in place of SRLanguageObject with this property)   */
    kSRRejectionLevel           = FOUR_CHAR_CODE('rjct')        /* SRRejectionLevel between 0 and 100 */
};

/* LM Object Types -- returned as kSRLMObjType property of language model objects */
enum {
    kSRLanguageModelType        = FOUR_CHAR_CODE('lmob'),       /* SRLanguageModel */
    kSRPathType                 = FOUR_CHAR_CODE('path'),       /* SRPath */
    kSRPhraseType               = FOUR_CHAR_CODE('phra'),       /* SRPhrase */
    kSRWordType                 = FOUR_CHAR_CODE('word')        /* SRWord */
};

/* a normal and reasonable rejection level */
enum {
    kSRDefaultRejectionLevel    = 50
};

/********************************************************************************/
/*                      NOTES ON USING THE API                                  */
/*                                                                              */
/*      All operations (with the exception of SRGetRecognitionSystem) are       */
/*      directed toward an object allocated or begot from New, Get and Read     */
/*      type calls.                                                             */
/*                                                                              */
/*      There is a simple rule in dealing with allocation and disposal:         */
/*                                                                              */
/*      *   all toolbox allocations are obtained from a SRRecognitionSystem     */
/*                                                                              */
/*      *   if you obtain an object via New or Get, then you own a reference    */
/*          to that object and it must be released via SRReleaseObject when     */
/*          you no longer need it                                               */
/*                                                                              */
/*      *   when you receive a SRRecognitionResult object via AppleEvent or     */
/*          callback, it has essentially been created on your behalf and so     */
/*          you are responsible for releasing it as above                       */
/*                                                                              */
/*      *   when you close a SRRecognitionSystem, all remaining objects which       */
/*          were allocated with it will be forcefully released and any          */
/*          remaining references to those objects will be invalid.              */
/*                                                                              */
/*      This translates into a very simple guideline:                           */
/*          If you allocate it or have it allocated for you, you must release   */
/*          it.  If you are only peeking at it, then don't release it.          */
/*                                                                              */
/********************************************************************************/
/* Opening and Closing of the SRRecognitionSystem */
EXTERN_API( OSErr )
SROpenRecognitionSystem         (SRRecognitionSystem *  system,
                                 OSType                 systemID)                           THREEWORDINLINE(0x303C, 0x0400, 0xAA56);

EXTERN_API( OSErr )
SRCloseRecognitionSystem        (SRRecognitionSystem    system)                             THREEWORDINLINE(0x303C, 0x0201, 0xAA56);

/* Accessing Properties of any Speech Object */
EXTERN_API( OSErr )
SRSetProperty                   (SRSpeechObject         srObject,
                                 OSType                 selector,
                                 const void *           property,
                                 Size                   propertyLen)                        THREEWORDINLINE(0x303C, 0x0802, 0xAA56);

EXTERN_API( OSErr )
SRGetProperty                   (SRSpeechObject         srObject,
                                 OSType                 selector,
                                 void *                 property,
                                 Size *                 propertyLen)                        THREEWORDINLINE(0x303C, 0x0803, 0xAA56);

/* Any object obtained via New or Get type calls must be released */
EXTERN_API( OSErr )
SRReleaseObject                 (SRSpeechObject         srObject)                           THREEWORDINLINE(0x303C, 0x0204, 0xAA56);

EXTERN_API( OSErr )
SRGetReference                  (SRSpeechObject         srObject,
                                 SRSpeechObject *       newObjectRef)                       THREEWORDINLINE(0x303C, 0x0425, 0xAA56);

/* SRRecognizer Instance Functions */
EXTERN_API( OSErr )
SRNewRecognizer                 (SRRecognitionSystem    system,
                                 SRRecognizer *         recognizer,
                                 OSType                 sourceID)                           THREEWORDINLINE(0x303C, 0x060A, 0xAA56);

EXTERN_API( OSErr )
SRStartListening                (SRRecognizer           recognizer)                         THREEWORDINLINE(0x303C, 0x020C, 0xAA56);

EXTERN_API( OSErr )
SRStopListening                 (SRRecognizer           recognizer)                         THREEWORDINLINE(0x303C, 0x020D, 0xAA56);

EXTERN_API( OSErr )
SRSetLanguageModel              (SRRecognizer           recognizer,
                                 SRLanguageModel        languageModel)                      THREEWORDINLINE(0x303C, 0x040E, 0xAA56);

EXTERN_API( OSErr )
SRGetLanguageModel              (SRRecognizer           recognizer,
                                 SRLanguageModel *      languageModel)                      THREEWORDINLINE(0x303C, 0x040F, 0xAA56);

EXTERN_API( OSErr )
SRContinueRecognition           (SRRecognizer           recognizer)                         THREEWORDINLINE(0x303C, 0x0210, 0xAA56);

EXTERN_API( OSErr )
SRCancelRecognition             (SRRecognizer           recognizer)                         THREEWORDINLINE(0x303C, 0x0211, 0xAA56);

EXTERN_API( OSErr )
SRIdle                          (void)                                                      THREEWORDINLINE(0x303C, 0x0028, 0xAA56);

/* Language Model Building and Manipulation Functions */
EXTERN_API( OSErr )
SRNewLanguageModel              (SRRecognitionSystem    system,
                                 SRLanguageModel *      model,
                                 const void *           name,
                                 Size                   nameLength)                         THREEWORDINLINE(0x303C, 0x0812, 0xAA56);

EXTERN_API( OSErr )
SRNewPath                       (SRRecognitionSystem    system,
                                 SRPath *               path)                               THREEWORDINLINE(0x303C, 0x0413, 0xAA56);

EXTERN_API( OSErr )
SRNewPhrase                     (SRRecognitionSystem    system,
                                 SRPhrase *             phrase,
                                 const void *           text,
                                 Size                   textLength)                         THREEWORDINLINE(0x303C, 0x0814, 0xAA56);

EXTERN_API( OSErr )
SRNewWord                       (SRRecognitionSystem    system,
                                 SRWord *               word,
                                 const void *           text,
                                 Size                   textLength)                         THREEWORDINLINE(0x303C, 0x0815, 0xAA56);

/* Operations on any object of the SRLanguageObject family */
EXTERN_API( OSErr )
SRPutLanguageObjectIntoHandle   (SRLanguageObject       languageObject,
                                 Handle                 lobjHandle)                         THREEWORDINLINE(0x303C, 0x0416, 0xAA56);

EXTERN_API( OSErr )
SRPutLanguageObjectIntoDataFile (SRLanguageObject       languageObject,
                                 short                  fRefNum)                            THREEWORDINLINE(0x303C, 0x0328, 0xAA56);

EXTERN_API( OSErr )
SRNewLanguageObjectFromHandle   (SRRecognitionSystem    system,
                                 SRLanguageObject *     languageObject,
                                 Handle                 lObjHandle)                         THREEWORDINLINE(0x303C, 0x0417, 0xAA56);

EXTERN_API( OSErr )
SRNewLanguageObjectFromDataFile (SRRecognitionSystem    system,
                                 SRLanguageObject *     languageObject,
                                 short                  fRefNum)                            THREEWORDINLINE(0x303C, 0x0427, 0xAA56);

EXTERN_API( OSErr )
SREmptyLanguageObject           (SRLanguageObject       languageObject)                     THREEWORDINLINE(0x303C, 0x0218, 0xAA56);

EXTERN_API( OSErr )
SRChangeLanguageObject          (SRLanguageObject       languageObject,
                                 const void *           text,
                                 Size                   textLength)                         THREEWORDINLINE(0x303C, 0x0619, 0xAA56);

EXTERN_API( OSErr )
SRAddLanguageObject             (SRLanguageObject       base,
                                 SRLanguageObject       addon)                              THREEWORDINLINE(0x303C, 0x041A, 0xAA56);

EXTERN_API( OSErr )
SRAddText                       (SRLanguageObject       base,
                                 const void *           text,
                                 Size                   textLength,
                                 long                   refCon)                             THREEWORDINLINE(0x303C, 0x081B, 0xAA56);

EXTERN_API( OSErr )
SRRemoveLanguageObject          (SRLanguageObject       base,
                                 SRLanguageObject       toRemove)                           THREEWORDINLINE(0x303C, 0x041C, 0xAA56);

/* Traversing SRRecognitionResults or SRLanguageObjects */
EXTERN_API( OSErr )
SRCountItems                    (SRSpeechObject         container,
                                 long *                 count)                              THREEWORDINLINE(0x303C, 0x0405, 0xAA56);

EXTERN_API( OSErr )
SRGetIndexedItem                (SRSpeechObject         container,
                                 SRSpeechObject *       item,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x0606, 0xAA56);

EXTERN_API( OSErr )
SRSetIndexedItem                (SRSpeechObject         container,
                                 SRSpeechObject         item,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x0607, 0xAA56);

EXTERN_API( OSErr )
SRRemoveIndexedItem             (SRSpeechObject         container,
                                 long                   index)                              THREEWORDINLINE(0x303C, 0x0408, 0xAA56);

/* Utilizing the System Feedback Window */
EXTERN_API( OSErr )
SRDrawText                      (SRRecognizer           recognizer,
                                 const void *           dispText,
                                 Size                   dispLength)                         THREEWORDINLINE(0x303C, 0x0621, 0xAA56);

EXTERN_API( OSErr )
SRDrawRecognizedText            (SRRecognizer           recognizer,
                                 const void *           dispText,
                                 Size                   dispLength)                         THREEWORDINLINE(0x303C, 0x0622, 0xAA56);

EXTERN_API( OSErr )
SRSpeakText                     (SRRecognizer           recognizer,
                                 const void *           speakText,
                                 Size                   speakLength)                        THREEWORDINLINE(0x303C, 0x0620, 0xAA56);

EXTERN_API( OSErr )
SRSpeakAndDrawText              (SRRecognizer           recognizer,
                                 const void *           text,
                                 Size                   textLength)                         THREEWORDINLINE(0x303C, 0x061F, 0xAA56);

EXTERN_API( OSErr )
SRStopSpeech                    (SRRecognizer           recognizer)                         THREEWORDINLINE(0x303C, 0x0223, 0xAA56);

EXTERN_API( Boolean )
SRSpeechBusy                    (SRRecognizer           recognizer)                         THREEWORDINLINE(0x303C, 0x0224, 0xAA56);

EXTERN_API( OSErr )
SRProcessBegin                  (SRRecognizer           recognizer,
                                 Boolean                failed)                             THREEWORDINLINE(0x303C, 0x031D, 0xAA56);

EXTERN_API( OSErr )
SRProcessEnd                    (SRRecognizer           recognizer,
                                 Boolean                failed)                             THREEWORDINLINE(0x303C, 0x031E, 0xAA56);




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

#endif /* __SPEECHRECOGNITION__ */

