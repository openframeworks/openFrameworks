/*
     File:       QuickTimeMusic.h
 
     Contains:   QuickTime Interfaces.
 
     Version:    Technology: QuickTime 6.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QUICKTIMEMUSIC__
#define __QUICKTIMEMUSIC__

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

#ifndef __MACMEMORY__
#include <MacMemory.h>
#endif

#ifndef __SOUND__
#include <Sound.h>
#endif

#ifndef __ENDIAN__
#include <Endian.h>
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
    kaiToneDescType             = FOUR_CHAR_CODE('tone'),
    kaiNoteRequestInfoType      = FOUR_CHAR_CODE('ntrq'),
    kaiKnobListType             = FOUR_CHAR_CODE('knbl'),
    kaiKeyRangeInfoType         = FOUR_CHAR_CODE('sinf'),
    kaiSampleDescType           = FOUR_CHAR_CODE('sdsc'),
    kaiSampleInfoType           = FOUR_CHAR_CODE('smin'),
    kaiSampleDataType           = FOUR_CHAR_CODE('sdat'),
    kaiSampleDataQUIDType       = FOUR_CHAR_CODE('quid'),
    kaiInstInfoType             = FOUR_CHAR_CODE('iinf'),
    kaiPictType                 = FOUR_CHAR_CODE('pict'),
    kaiWriterType               = FOUR_CHAR_CODE(' wrt'),
    kaiCopyrightType            = FOUR_CHAR_CODE(' cpy'),
    kaiOtherStrType             = FOUR_CHAR_CODE('str '),
    kaiInstrumentRefType        = FOUR_CHAR_CODE('iref'),
    kaiInstGMQualityType        = FOUR_CHAR_CODE('qual'),
    kaiLibraryInfoType          = FOUR_CHAR_CODE('linf'),
    kaiLibraryDescType          = FOUR_CHAR_CODE('ldsc')
};


struct InstLibDescRec {
    Str31                           libIDName;
};
typedef struct InstLibDescRec           InstLibDescRec;

struct InstKnobRec {
    BigEndianLong                   number;
    BigEndianLong                   value;
};
typedef struct InstKnobRec              InstKnobRec;
enum {
    kInstKnobMissingUnknown     = 0,
    kInstKnobMissingDefault     = (1 << 0)
};


struct InstKnobList {
    BigEndianLong                   knobCount;
    BigEndianLong                   knobFlags;
    InstKnobRec                     knob[1];
};
typedef struct InstKnobList             InstKnobList;
enum {
    kMusicLoopTypeNormal        = 0,
    kMusicLoopTypePalindrome    = 1                             /* back & forth*/
};

enum {
    instSamplePreProcessFlag    = 1 << 0
};


struct InstSampleDescRec {
    BigEndianOSType                 dataFormat;
    BigEndianShort                  numChannels;
    BigEndianShort                  sampleSize;
    BigEndianUnsignedFixed          sampleRate;
    BigEndianShort                  sampleDataID;
    BigEndianLong                   offset;                     /* offset within SampleData - this could be just for internal use*/
    BigEndianLong                   numSamples;                 /* this could also just be for internal use, we'll see*/

    BigEndianLong                   loopType;
    BigEndianLong                   loopStart;
    BigEndianLong                   loopEnd;

    BigEndianLong                   pitchNormal;
    BigEndianLong                   pitchLow;
    BigEndianLong                   pitchHigh;
};
typedef struct InstSampleDescRec        InstSampleDescRec;

typedef Handle                          AtomicInstrument;
typedef Ptr                             AtomicInstrumentPtr;
enum {
    kQTMIDIComponentType        = FOUR_CHAR_CODE('midi')
};

enum {
    kOMSComponentSubType        = FOUR_CHAR_CODE('OMS '),
    kFMSComponentSubType        = FOUR_CHAR_CODE('FMS '),
    kMIDIManagerComponentSubType = FOUR_CHAR_CODE('mmgr'),
    kOSXMIDIComponentSubType    = FOUR_CHAR_CODE('osxm')
};

typedef ComponentInstance               QTMIDIComponent;
enum {
    kMusicPacketPortLost        = 1,                            /* received when application loses the default input port */
    kMusicPacketPortFound       = 2,                            /* received when application gets it back out from under someone else's claim */
    kMusicPacketTimeGap         = 3                             /* data[0] = number of milliseconds to keep the MIDI line silent */
};

enum {
    kAppleSysexID               = 0x11,                         /* apple sysex is followed by 2-byte command. 0001 is the command for samplesize */
    kAppleSysexCmdSampleSize    = 0x0001,                       /* 21 bit number in 3 midi bytes follows sysex ID and 2 cmd bytes */
    kAppleSysexCmdSampleBreak   = 0x0002,                       /* specifies that the sample should break right here */
    kAppleSysexCmdAtomicInstrument = 0x0010,                    /* contents of atomic instrument handle */
    kAppleSysexCmdDeveloper     = 0x7F00                        /* F0 11 7F 00 ww xx yy zz ... F7 is available for non-Apple developers, where wxyz is unique app signature with 8th bit cleared, unique to developer, and 00 and 7f are reserved */
};


struct MusicMIDIPacket {
    unsigned short                  length;
    unsigned long                   reserved;                   /* if length zero, then reserved = above enum */
    UInt8                           data[249];
};
typedef struct MusicMIDIPacket          MusicMIDIPacket;
typedef CALLBACK_API( ComponentResult , MusicMIDISendProcPtr )(ComponentInstance self, long refCon, MusicMIDIPacket *mmp);
typedef CALLBACK_API( ComponentResult , MusicMIDIReadHookProcPtr )(MusicMIDIPacket *mp, long myRefCon);
typedef STACK_UPP_TYPE(MusicMIDISendProcPtr)                    MusicMIDISendUPP;
typedef STACK_UPP_TYPE(MusicMIDIReadHookProcPtr)                MusicMIDIReadHookUPP;
enum {
    kSynthesizerConnectionFMS   = 1,                            /* this connection imported from FMS */
    kSynthesizerConnectionMMgr  = 2,                            /* this connection imported from the MIDI Mgr */
    kSynthesizerConnectionOMS   = 4,                            /* this connection imported from OMS */
    kSynthesizerConnectionQT    = 8,                            /* this connection is a QuickTime-only port */
    kSynthesizerConnectionOSXMIDI = 16,                         /* this connection is an OS X CoreMIDI port */
                                                                /* lowest five bits are mutually exclusive; combinations reserved for future use.*/
    kSynthesizerConnectionUnavailable = 256                     /* port exists, but cannot be used just now */
};

/*
    The sampleBankFile field of this structure can be used to pass in a pointer to an FSSpec
    that represents a SoundFont 2 or DLS file (otherwise set it to NULL ).
    
    You then pass in a structure with this field set (all other fields should be zero) to
    NARegisterMusicDevice:
        - with synthType as kSoftSynthComponentSubType 
        - with name being used to return to the application the "name" of the synth 
        that should be used in the synthesiserName field of the ToneDescription structure
        and is also used to retrieve a particular MusicComponent with the
        NAGetRegisteredMusicDevice call
    
    This call will create a MusicComponent of kSoftSynthComponentSubType, with the specified
    sound bank as the sample data source.
*/

struct SynthesizerConnections {
    OSType                          clientID;
    OSType                          inputPortID;                /* terminology death: this port is used to SEND to the midi synth */
    OSType                          outputPortID;               /* terminology death: this port receives from a keyboard or other control device */
    long                            midiChannel;                /* The system channel; others are configurable (or the nubus slot number) */
    long                            flags;
    long                            unique;                     /* unique id may be used instead of index, to getinfo and unregister calls */
    FSSpecPtr                       sampleBankFile;             /*  see notes above */
    long                            reserved2;                  /* should be zero */
};
typedef struct SynthesizerConnections   SynthesizerConnections;

struct QTMIDIPort {
    SynthesizerConnections          portConnections;
    Str63                           portName;
};
typedef struct QTMIDIPort               QTMIDIPort;

struct QTMIDIPortList {
    short                           portCount;
    QTMIDIPort                      port[1];
};
typedef struct QTMIDIPortList           QTMIDIPortList;
typedef QTMIDIPortList *                QTMIDIPortListPtr;
typedef QTMIDIPortListPtr *             QTMIDIPortListHandle;
EXTERN_API( ComponentResult )
QTMIDIGetMIDIPorts              (QTMIDIComponent        ci,
                                 QTMIDIPortListHandle * inputPorts,
                                 QTMIDIPortListHandle * outputPorts)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTMIDIUseSendPort               (QTMIDIComponent        ci,
                                 long                   portIndex,
                                 long                   inUse)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTMIDISendMIDI                  (QTMIDIComponent        ci,
                                 long                   portIndex,
                                 MusicMIDIPacket *      mp)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
QTMIDIUseReceivePort            (QTMIDIComponent        ci,
                                 long                   portIndex,
                                 MusicMIDIReadHookUPP   readHook,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x000C, 0x0004, 0x7000, 0xA82A);



enum {
    kMusicComponentType         = FOUR_CHAR_CODE('musi'),
    kInstrumentComponentType    = FOUR_CHAR_CODE('inst')
};

enum {
    kSoftSynthComponentSubType  = FOUR_CHAR_CODE('ss  '),
    kGMSynthComponentSubType    = FOUR_CHAR_CODE('gm  ')
};



typedef ComponentInstance               MusicComponent;
/* MusicSynthesizerFlags*/
enum {
    kSynthesizerDynamicVoice    = 1 << 0,                       /* can assign voices on the fly (else, polyphony is very important */
    kSynthesizerUsesMIDIPort    = 1 << 1,                       /* must be patched through MIDI Manager */
    kSynthesizerMicrotone       = 1 << 2,                       /* can play microtonal scales */
    kSynthesizerHasSamples      = 1 << 3,                       /* synthesizer has some use for sampled data */
    kSynthesizerMixedDrums      = 1 << 4,                       /* any part can play drum parts, total = instrument parts */
    kSynthesizerSoftware        = 1 << 5,                       /* implemented in main CPU software == uses cpu cycles */
    kSynthesizerHardware        = 1 << 6,                       /* is a hardware device (such as nubus, or maybe DSP?) */
    kSynthesizerDynamicChannel  = 1 << 7,                       /* can move any part to any channel or disable each part. (else we assume it lives on all channels in masks) */
    kSynthesizerHogsSystemChannel = 1 << 8,                     /* can be channelwise dynamic, but always responds on its system channel */
    kSynthesizerHasSystemChannel = 1 << 9,                      /* has some "system channel" notion to distinguish it from multiple instances of the same device (GM devices dont) */
    kSynthesizerSlowSetPart     = 1 << 10,                      /* SetPart() and SetPartInstrumentNumber() calls do not have rapid response, may glitch notes */
    kSynthesizerOffline         = 1 << 12,                      /* can enter an offline synthesis mode */
    kSynthesizerGM              = 1 << 14,                      /* synth is a GM device */
    kSynthesizerDLS             = 1 << 15,                      /* synth supports DLS level 1 */
    kSynthesizerSoundLocalization = 1 << 16                     /* synth supports extremely baroque, nonstandard, and proprietary "apple game sprockets" localization parameter set */
};

/*
 * Note that these controller numbers
 * are _not_ identical to the MIDI controller numbers.
 * These are _signed_ 8.8 values, and the LSB's are
 * always sent to a MIDI device. Controllers 32-63 are
 * reserved (for MIDI, they are LSB's for 0-31, but we
 * always send both).
 *
 * The full range, therefore, is -128.00 to 127.7f.
 *
 * _Excepting_ _volume_, all controls default to zero.
 *
 * Pitch bend is specified in fractional semitones! No
 * more "pitch bend range" nonsense. You can bend as far
 * as you want, any time you want.
 */
typedef SInt32                          MusicController;
enum {
    kControllerModulationWheel  = 1,
    kControllerBreath           = 2,
    kControllerFoot             = 4,
    kControllerPortamentoTime   = 5,                            /* time in 8.8 seconds, portamento on/off is omitted, 0 time = 'off' */
    kControllerVolume           = 7,                            /* main volume control */
    kControllerBalance          = 8,
    kControllerPan              = 10,                           /* 0 - "default", 1 - n: positioned in output 1-n (incl fractions) */
    kControllerExpression       = 11,                           /* secondary volume control */
    kControllerLever1           = 16,                           /* general purpose controllers */
    kControllerLever2           = 17,                           /* general purpose controllers */
    kControllerLever3           = 18,                           /* general purpose controllers */
    kControllerLever4           = 19,                           /* general purpose controllers */
    kControllerLever5           = 80,                           /* general purpose controllers */
    kControllerLever6           = 81,                           /* general purpose controllers */
    kControllerLever7           = 82,                           /* general purpose controllers */
    kControllerLever8           = 83,                           /* general purpose controllers */
    kControllerPitchBend        = 32,                           /* positive & negative semitones, with 8 bits fraction, same units as transpose controllers*/
    kControllerAfterTouch       = 33,                           /* aka channel pressure */
    kControllerPartTranspose    = 40,                           /* identical to pitchbend, for overall part xpose */
    kControllerTuneTranspose    = 41,                           /* another pitchbend, for "song global" pitch offset */
    kControllerPartVolume       = 42,                           /* another volume control, passed right down from note allocator part volume */
    kControllerTuneVolume       = 43,                           /* another volume control, used for "song global" volume - since we share one synthesizer across multiple tuneplayers*/
    kControllerSustain          = 64,                           /* boolean - positive for on, 0 or negative off */
    kControllerPortamento       = 65,                           /* boolean*/
    kControllerSostenuto        = 66,                           /* boolean */
    kControllerSoftPedal        = 67,                           /* boolean */
    kControllerReverb           = 91,
    kControllerTremolo          = 92,
    kControllerChorus           = 93,
    kControllerCeleste          = 94,
    kControllerPhaser           = 95,
    kControllerEditPart         = 113,                          /* last 16 controllers 113-128 and above are global controllers which respond on part zero */
    kControllerMasterTune       = 114,
    kControllerMasterTranspose  = 114,                          /* preferred*/
    kControllerMasterVolume     = 115,
    kControllerMasterCPULoad    = 116,
    kControllerMasterPolyphony  = 117,
    kControllerMasterFeatures   = 118
};


/* ID's of knobs supported by the QuickTime Music Synthesizer built into QuickTime*/

enum {
    kQTMSKnobStartID            = 0x02000000,
    kQTMSKnobVolumeAttackTimeID = 0x02000001,
    kQTMSKnobVolumeDecayTimeID  = 0x02000002,
    kQTMSKnobVolumeSustainLevelID = 0x02000003,
    kQTMSKnobVolumeRelease1RateID = 0x02000004,
    kQTMSKnobVolumeDecayKeyScalingID = 0x02000005,
    kQTMSKnobVolumeReleaseTimeID = 0x02000006,
    kQTMSKnobVolumeLFODelayID   = 0x02000007,
    kQTMSKnobVolumeLFORampTimeID = 0x02000008,
    kQTMSKnobVolumeLFOPeriodID  = 0x02000009,
    kQTMSKnobVolumeLFOShapeID   = 0x0200000A,
    kQTMSKnobVolumeLFODepthID   = 0x0200000B,
    kQTMSKnobVolumeOverallID    = 0x0200000C,
    kQTMSKnobVolumeVelocity127ID = 0x0200000D,
    kQTMSKnobVolumeVelocity96ID = 0x0200000E,
    kQTMSKnobVolumeVelocity64ID = 0x0200000F,
    kQTMSKnobVolumeVelocity32ID = 0x02000010,
    kQTMSKnobVolumeVelocity16ID = 0x02000011,                   /* Pitch related knobs*/
    kQTMSKnobPitchTransposeID   = 0x02000012,
    kQTMSKnobPitchLFODelayID    = 0x02000013,
    kQTMSKnobPitchLFORampTimeID = 0x02000014,
    kQTMSKnobPitchLFOPeriodID   = 0x02000015,
    kQTMSKnobPitchLFOShapeID    = 0x02000016,
    kQTMSKnobPitchLFODepthID    = 0x02000017,
    kQTMSKnobPitchLFOQuantizeID = 0x02000018,                   /* Stereo related knobs*/
    kQTMSKnobStereoDefaultPanID = 0x02000019,
    kQTMSKnobStereoPositionKeyScalingID = 0x0200001A,
    kQTMSKnobPitchLFOOffsetID   = 0x0200001B,
    kQTMSKnobExclusionGroupID   = 0x0200001C,                   /* Misc knobs, late additions*/
    kQTMSKnobSustainTimeID      = 0x0200001D,
    kQTMSKnobSustainInfiniteID  = 0x0200001E,
    kQTMSKnobVolumeLFOStereoID  = 0x0200001F,
    kQTMSKnobVelocityLowID      = 0x02000020,
    kQTMSKnobVelocityHighID     = 0x02000021,
    kQTMSKnobVelocitySensitivityID = 0x02000022,
    kQTMSKnobPitchSensitivityID = 0x02000023,
    kQTMSKnobVolumeLFODepthFromWheelID = 0x02000024,
    kQTMSKnobPitchLFODepthFromWheelID = 0x02000025,             /* Volume Env again*/
    kQTMSKnobVolumeExpOptionsID = 0x02000026,                   /* Env1*/
    kQTMSKnobEnv1AttackTimeID   = 0x02000027,
    kQTMSKnobEnv1DecayTimeID    = 0x02000028,
    kQTMSKnobEnv1SustainLevelID = 0x02000029,
    kQTMSKnobEnv1SustainTimeID  = 0x0200002A,
    kQTMSKnobEnv1SustainInfiniteID = 0x0200002B,
    kQTMSKnobEnv1ReleaseTimeID  = 0x0200002C,
    kQTMSKnobEnv1ExpOptionsID   = 0x0200002D,                   /* Env2*/
    kQTMSKnobEnv2AttackTimeID   = 0x0200002E,
    kQTMSKnobEnv2DecayTimeID    = 0x0200002F,
    kQTMSKnobEnv2SustainLevelID = 0x02000030,
    kQTMSKnobEnv2SustainTimeID  = 0x02000031,
    kQTMSKnobEnv2SustainInfiniteID = 0x02000032,
    kQTMSKnobEnv2ReleaseTimeID  = 0x02000033,
    kQTMSKnobEnv2ExpOptionsID   = 0x02000034,                   /* Pitch Env*/
    kQTMSKnobPitchEnvelopeID    = 0x02000035,
    kQTMSKnobPitchEnvelopeDepthID = 0x02000036,                 /* Filter*/
    kQTMSKnobFilterKeyFollowID  = 0x02000037,
    kQTMSKnobFilterTransposeID  = 0x02000038,
    kQTMSKnobFilterQID          = 0x02000039,
    kQTMSKnobFilterFrequencyEnvelopeID = 0x0200003A,
    kQTMSKnobFilterFrequencyEnvelopeDepthID = 0x0200003B,
    kQTMSKnobFilterQEnvelopeID  = 0x0200003C,
    kQTMSKnobFilterQEnvelopeDepthID = 0x0200003D,               /* Reverb Threshhold*/
    kQTMSKnobReverbThresholdID  = 0x0200003E,
    kQTMSKnobVolumeAttackVelScalingID = 0x0200003F,
    kQTMSKnobLastIDPlus1        = 0x02000040
};




enum {
    kControllerMaximum          = 0x00007FFF,                   /* +01111111.11111111 */
    kControllerMinimum          = (long)0xFFFF8000              /* -10000000.00000000 */
};


struct SynthesizerDescription {
    OSType                          synthesizerType;            /* synthesizer type (must be same as component subtype) */
    Str31                           name;                       /* text name of synthesizer type */
    unsigned long                   flags;                      /* from the above enum */
    unsigned long                   voiceCount;                 /* maximum polyphony */

    unsigned long                   partCount;                  /* maximum multi-timbrality (and midi channels) */
    unsigned long                   instrumentCount;            /* non gm, built in (rom) instruments only */
    unsigned long                   modifiableInstrumentCount;  /* plus n-more are user modifiable */
    unsigned long                   channelMask;                /* (midi device only) which channels device always uses */

    unsigned long                   drumPartCount;              /* maximum multi-timbrality of drum parts */
    unsigned long                   drumCount;                  /* non gm, built in (rom) drumkits only */
    unsigned long                   modifiableDrumCount;        /* plus n-more are user modifiable */
    unsigned long                   drumChannelMask;            /* (midi device only) which channels device always uses */

    unsigned long                   outputCount;                /* number of audio outputs (usually two) */
    unsigned long                   latency;                    /* response time in .Sec */

    unsigned long                   controllers[4];             /* array of 128 bits */
    unsigned long                   gmInstruments[4];           /* array of 128 bits */
    unsigned long                   gmDrums[4];                 /* array of 128 bits */
};
typedef struct SynthesizerDescription   SynthesizerDescription;
enum {
    kVoiceCountDynamic          = -1                            /* constant to use to specify dynamic voicing */
};



struct ToneDescription {
    BigEndianOSType                 synthesizerType;            /* synthesizer type */
    Str31                           synthesizerName;            /* name of instantiation of synth */
    Str31                           instrumentName;             /* preferred name for human use */
    BigEndianLong                   instrumentNumber;           /* inst-number used if synth-name matches */
    BigEndianLong                   gmNumber;                   /* Best matching general MIDI number */
};
typedef struct ToneDescription          ToneDescription;
enum {
    kFirstGMInstrument          = 0x00000001,
    kLastGMInstrument           = 0x00000080,
    kFirstGSInstrument          = 0x00000081,
    kLastGSInstrument           = 0x00003FFF,
    kFirstDrumkit               = 0x00004000,                   /* (first value is "no drum". instrument numbers from 16384->16384+128 are drumkits, and for GM they are _defined_ drumkits! */
    kLastDrumkit                = 0x00004080,
    kFirstROMInstrument         = 0x00008000,
    kLastROMInstrument          = 0x0000FFFF,
    kFirstUserInstrument        = 0x00010000,
    kLastUserInstrument         = 0x0001FFFF
};

/* InstrumentMatch*/
enum {
    kInstrumentMatchSynthesizerType = 1,
    kInstrumentMatchSynthesizerName = 2,
    kInstrumentMatchName        = 4,
    kInstrumentMatchNumber      = 8,
    kInstrumentMatchGMNumber    = 16,
    kInstrumentMatchGSNumber    = 32
};

/* KnobFlags*/
enum {
    kKnobBasic                  = 8,                            /* knob shows up in certain simplified lists of knobs */
    kKnobReadOnly               = 16,                           /* knob value cannot be changed by user or with a SetKnob call */
    kKnobInterruptUnsafe        = 32,                           /* only alter this knob from foreground task time (may access toolbox) */
    kKnobKeyrangeOverride       = 64,                           /* knob can be overridden within a single keyrange (software synth only) */
    kKnobGroupStart             = 128,                          /* knob is first in some logical group of knobs */
    kKnobFixedPoint8            = 1024,
    kKnobFixedPoint16           = 2048,                         /* One of these may be used at a time. */
    kKnobTypeNumber             = 0 << 12,
    kKnobTypeGroupName          = 1 << 12,                      /* "knob" is really a group name for display purposes */
    kKnobTypeBoolean            = 2 << 12,                      /* if range is greater than 1, its a multi-checkbox field */
    kKnobTypeNote               = 3 << 12,                      /* knob range is equivalent to MIDI keys */
    kKnobTypePan                = 4 << 12,                      /* range goes left/right (lose this? ) */
    kKnobTypeInstrument         = 5 << 12,                      /* knob value = reference to another instrument number */
    kKnobTypeSetting            = 6 << 12,                      /* knob value is 1 of n different things (eg, fm algorithms) popup menu */
    kKnobTypeMilliseconds       = 7 << 12,                      /* knob is a millisecond time range */
    kKnobTypePercentage         = 8 << 12,                      /* knob range is displayed as a Percentage */
    kKnobTypeHertz              = 9 << 12,                      /* knob represents frequency */
    kKnobTypeButton             = 10 << 12                      /* momentary trigger push button */
};


enum {
    kUnknownKnobValue           = 0x7FFFFFFF,                   /* a knob with this value means, we don't know it. */
    kDefaultKnobValue           = 0x7FFFFFFE                    /* used to SET a knob to its default value. */
};


struct KnobDescription {
    Str63                           name;
    long                            lowValue;
    long                            highValue;
    long                            defaultValue;               /* a default instrument is made of all default values */
    long                            flags;
    long                            knobID;
};
typedef struct KnobDescription          KnobDescription;

struct GCInstrumentData {
    ToneDescription                 tone;
    long                            knobCount;
    long                            knob[1];
};
typedef struct GCInstrumentData         GCInstrumentData;
typedef GCInstrumentData *              GCInstrumentDataPtr;
typedef GCInstrumentDataPtr *           GCInstrumentDataHandle;

struct InstrumentAboutInfo {
    PicHandle                       p;
    Str255                          author;
    Str255                          copyright;
    Str255                          other;
};
typedef struct InstrumentAboutInfo      InstrumentAboutInfo;

enum {
    notImplementedMusicErr      = (0x80000000 | (0xFFFF & (notImplementedMusicOSErr))),
    cantSendToSynthesizerErr    = (0x80000000 | (0xFFFF & (cantSendToSynthesizerOSErr))),
    cantReceiveFromSynthesizerErr = (0x80000000 | (0xFFFF & (cantReceiveFromSynthesizerOSErr))),
    illegalVoiceAllocationErr   = (0x80000000 | (0xFFFF & (illegalVoiceAllocationOSErr))),
    illegalPartErr              = (0x80000000 | (0xFFFF & (illegalPartOSErr))),
    illegalChannelErr           = (0x80000000 | (0xFFFF & (illegalChannelOSErr))),
    illegalKnobErr              = (0x80000000 | (0xFFFF & (illegalKnobOSErr))),
    illegalKnobValueErr         = (0x80000000 | (0xFFFF & (illegalKnobValueOSErr))),
    illegalInstrumentErr        = (0x80000000 | (0xFFFF & (illegalInstrumentOSErr))),
    illegalControllerErr        = (0x80000000 | (0xFFFF & (illegalControllerOSErr))),
    midiManagerAbsentErr        = (0x80000000 | (0xFFFF & (midiManagerAbsentOSErr))),
    synthesizerNotRespondingErr = (0x80000000 | (0xFFFF & (synthesizerNotRespondingOSErr))),
    synthesizerErr              = (0x80000000 | (0xFFFF & (synthesizerOSErr))),
    illegalNoteChannelErr       = (0x80000000 | (0xFFFF & (illegalNoteChannelOSErr))),
    noteChannelNotAllocatedErr  = (0x80000000 | (0xFFFF & (noteChannelNotAllocatedOSErr))),
    tunePlayerFullErr           = (0x80000000 | (0xFFFF & (tunePlayerFullOSErr))),
    tuneParseErr                = (0x80000000 | (0xFFFF & (tuneParseOSErr)))
};

enum {
    kGetAtomicInstNoExpandedSamples = 1 << 0,
    kGetAtomicInstNoOriginalSamples = 1 << 1,
    kGetAtomicInstNoSamples     = kGetAtomicInstNoExpandedSamples | kGetAtomicInstNoOriginalSamples,
    kGetAtomicInstNoKnobList    = 1 << 2,
    kGetAtomicInstNoInstrumentInfo = 1 << 3,
    kGetAtomicInstOriginalKnobList = 1 << 4,
    kGetAtomicInstAllKnobs      = 1 << 5                        /* return even those that are set to default*/
};

/*
   For non-gm instruments, instrument number of tone description == 0
   If you want to speed up while running, slam the inst num with what Get instrument number returns
   All missing knobs are slammed to the default value
*/
enum {
    kSetAtomicInstKeepOriginalInstrument = 1 << 0,
    kSetAtomicInstShareAcrossParts = 1 << 1,                    /* inst disappears when app goes away*/
    kSetAtomicInstCallerTosses  = 1 << 2,                       /* the caller isn't keeping a copy around (for NASetAtomicInstrument)*/
    kSetAtomicInstCallerGuarantees = 1 << 3,                    /* the caller guarantees a copy is around*/
    kSetAtomicInstInterruptSafe = 1 << 4,                       /* dont move memory at this time (but process at next task time)*/
    kSetAtomicInstDontPreprocess = 1 << 7                       /* perform no further preprocessing because either 1)you know the instrument is digitally clean, or 2) you got it from a GetPartAtomic*/
};

enum {
    kInstrumentNamesModifiable  = 1,
    kInstrumentNamesBoth        = 2
};

/*
 * Structures specific to the GenericMusicComponent
 */

enum {
    kGenericMusicComponentSubtype = FOUR_CHAR_CODE('gene')
};


struct GenericKnobDescription {
    KnobDescription                 kd;
    long                            hw1;                        /* driver defined */
    long                            hw2;                        /* driver defined */
    long                            hw3;                        /* driver defined */
    long                            settingsID;                 /* resource ID list for boolean and popup names */
};
typedef struct GenericKnobDescription   GenericKnobDescription;

struct GenericKnobDescriptionList {
    long                            knobCount;
    GenericKnobDescription          knob[1];
};
typedef struct GenericKnobDescriptionList GenericKnobDescriptionList;
typedef GenericKnobDescriptionList *    GenericKnobDescriptionListPtr;
typedef GenericKnobDescriptionListPtr * GenericKnobDescriptionListHandle;
/* knobTypes for MusicDerivedSetKnob */
enum {
    kGenericMusicKnob           = 1,
    kGenericMusicInstrumentKnob = 2,
    kGenericMusicDrumKnob       = 3,
    kGenericMusicGlobalController = 4
};



enum {
    kGenericMusicResFirst       = 0,
    kGenericMusicResMiscStringList = 1,                         /* STR# 1: synth name, 2:about author,3:aboutcopyright,4:aboutother */
    kGenericMusicResMiscLongList = 2,                           /* Long various params, see list below */
    kGenericMusicResInstrumentList = 3,                         /* NmLs of names and shorts, categories prefixed by 'oo' */
    kGenericMusicResDrumList    = 4,                            /* NmLs of names and shorts */
    kGenericMusicResInstrumentKnobDescriptionList = 5,          /* Knob */
    kGenericMusicResDrumKnobDescriptionList = 6,                /* Knob */
    kGenericMusicResKnobDescriptionList = 7,                    /* Knob */
    kGenericMusicResBitsLongList = 8,                           /* Long back to back bitmaps of controllers, gminstruments, and drums */
    kGenericMusicResModifiableInstrumentHW = 9,                 /* Shrt same as the hw shorts trailing the instrument names, a shortlist */
    kGenericMusicResGMTranslation = 10,                         /* Long 128 long entries, 1 for each gm inst, of local instrument numbers 1-n (not hw numbers) */
    kGenericMusicResROMInstrumentData = 11,                     /* knob lists for ROM instruments, so the knob values may be known */
    kGenericMusicResAboutPICT   = 12,                           /* picture for aboutlist. must be present for GetAbout call to work */
    kGenericMusicResLast        = 13
};

/* elements of the misc long list */
enum {
    kGenericMusicMiscLongFirst  = 0,
    kGenericMusicMiscLongVoiceCount = 1,
    kGenericMusicMiscLongPartCount = 2,
    kGenericMusicMiscLongModifiableInstrumentCount = 3,
    kGenericMusicMiscLongChannelMask = 4,
    kGenericMusicMiscLongDrumPartCount = 5,
    kGenericMusicMiscLongModifiableDrumCount = 6,
    kGenericMusicMiscLongDrumChannelMask = 7,
    kGenericMusicMiscLongOutputCount = 8,
    kGenericMusicMiscLongLatency = 9,
    kGenericMusicMiscLongFlags  = 10,
    kGenericMusicMiscLongFirstGMHW = 11,                        /* number to add to locate GM main instruments */
    kGenericMusicMiscLongFirstGMDrumHW = 12,                    /* number to add to locate GM drumkits */
    kGenericMusicMiscLongFirstUserHW = 13,                      /* First hw number of user instruments (presumed sequential) */
    kGenericMusicMiscLongLast   = 14
};


struct GCPart {
    long                            hwInstrumentNumber;         /* internal number of recalled instrument */
    short                           controller[128];            /* current values for all controllers */
    long                            volume;                     /* ctrl 7 is special case */
    long                            polyphony;
    long                            midiChannel;                /* 1-16 if in use */
    GCInstrumentData                id;                         /* ToneDescription & knoblist, uncertain length */
};
typedef struct GCPart                   GCPart;
/*
 * Calls specific to the GenericMusicComponent
 */
enum {
    kMusicGenericRange          = 0x0100,
    kMusicDerivedRange          = 0x0200
};

/*
 * Flags in GenericMusicConfigure call
 */
enum {
    kGenericMusicDoMIDI         = 1 << 0,                       /* implement normal MIDI messages for note, controllers, and program changes 0-127 */
    kGenericMusicBank0          = 1 << 1,                       /* implement instrument bank changes on controller 0 */
    kGenericMusicBank32         = 1 << 2,                       /* implement instrument bank changes on controller 32 */
    kGenericMusicErsatzMIDI     = 1 << 3,                       /* construct MIDI packets, but send them to the derived component */
    kGenericMusicCallKnobs      = 1 << 4,                       /* call the derived component with special knob format call */
    kGenericMusicCallParts      = 1 << 5,                       /* call the derived component with special part format call */
    kGenericMusicCallInstrument = 1 << 6,                       /* call MusicDerivedSetInstrument for MusicSetInstrument calls */
    kGenericMusicCallNumber     = 1 << 7,                       /* call MusicDerivedSetPartInstrumentNumber for MusicSetPartInstrumentNumber calls, & don't send any C0 or bank stuff */
    kGenericMusicCallROMInstrument = 1 << 8,                    /* call MusicSetInstrument for MusicSetPartInstrumentNumber for "ROM" instruments, passing params from the ROMi resource */
    kGenericMusicAllDefaults    = 1 << 9                        /* indicates that when a new instrument is recalled, all knobs are reset to DEFAULT settings. True for GS modules */
};





typedef CALLBACK_API( ComponentResult , MusicOfflineDataProcPtr )(Ptr SoundData, long numBytes, long myRefCon);
typedef STACK_UPP_TYPE(MusicOfflineDataProcPtr)                 MusicOfflineDataUPP;

struct OfflineSampleType {
    unsigned long                   numChannels;                /*number of channels,  ie mono = 1*/
    UnsignedFixed                   sampleRate;                 /*sample rate in Apples Fixed point representation*/
    unsigned short                  sampleSize;                 /*number of bits in sample*/
};
typedef struct OfflineSampleType        OfflineSampleType;

struct InstrumentInfoRecord {
    long                            instrumentNumber;           /* instrument number (if 0, name is a catagory)*/
    long                            flags;                      /* show in picker, etc.*/
    long                            toneNameIndex;              /* index in toneNames (1 based)*/
    long                            itxtNameAtomID;             /* index in itxtNames (itxt/name by index)*/
};
typedef struct InstrumentInfoRecord     InstrumentInfoRecord;

struct InstrumentInfoList {
    long                            recordCount;
    Handle                          toneNames;                  /* name from tone description*/
    QTAtomContainer                 itxtNames;                  /* itxt/name atoms for instruments*/
    InstrumentInfoRecord            info[1];
};
typedef struct InstrumentInfoList       InstrumentInfoList;
typedef InstrumentInfoList *            InstrumentInfoListPtr;
typedef InstrumentInfoListPtr *         InstrumentInfoListHandle;
EXTERN_API( ComponentResult )
MusicGetDescription             (MusicComponent         mc,
                                 SynthesizerDescription * sd)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetPart                    (MusicComponent         mc,
                                 long                   part,
                                 long *                 midiChannel,
                                 long *                 polyphony)                          FIVEWORDINLINE(0x2F3C, 0x000C, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetPart                    (MusicComponent         mc,
                                 long                   part,
                                 long                   midiChannel,
                                 long                   polyphony)                          FIVEWORDINLINE(0x2F3C, 0x000C, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetPartInstrumentNumber    (MusicComponent         mc,
                                 long                   part,
                                 long                   instrumentNumber)                   FIVEWORDINLINE(0x2F3C, 0x0008, 0x0004, 0x7000, 0xA82A);

#if OLDROUTINENAMES
#define MusicSetInstrumentNumber(ci,part,instrumentNumber) MusicSetPartInstrumentNumber(ci, part,instrumentNumber)
#endif

EXTERN_API( ComponentResult )
MusicGetPartInstrumentNumber    (MusicComponent         mc,
                                 long                   part)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicStorePartInstrument        (MusicComponent         mc,
                                 long                   part,
                                 long                   instrumentNumber)                   FIVEWORDINLINE(0x2F3C, 0x0008, 0x0006, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
MusicGetPartAtomicInstrument    (MusicComponent         mc,
                                 long                   part,
                                 AtomicInstrument *     ai,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0009, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetPartAtomicInstrument    (MusicComponent         mc,
                                 long                   part,
                                 AtomicInstrumentPtr    aiP,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x000A, 0x7000, 0xA82A);


/* Obsolete calls*/
EXTERN_API( ComponentResult )
MusicGetInstrumentKnobDescriptionObsolete (MusicComponent  mc,
                                 long                   knobIndex,
                                 void *                 mkd)                                FIVEWORDINLINE(0x2F3C, 0x0008, 0x000D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetDrumKnobDescriptionObsolete (MusicComponent     mc,
                                 long                   knobIndex,
                                 void *                 mkd)                                FIVEWORDINLINE(0x2F3C, 0x0008, 0x000E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetKnobDescriptionObsolete (MusicComponent         mc,
                                 long                   knobIndex,
                                 void *                 mkd)                                FIVEWORDINLINE(0x2F3C, 0x0008, 0x000F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetPartKnob                (MusicComponent         mc,
                                 long                   part,
                                 long                   knobID)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetPartKnob                (MusicComponent         mc,
                                 long                   part,
                                 long                   knobID,
                                 long                   knobValue)                          FIVEWORDINLINE(0x2F3C, 0x000C, 0x0011, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetKnob                    (MusicComponent         mc,
                                 long                   knobID)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0012, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetKnob                    (MusicComponent         mc,
                                 long                   knobID,
                                 long                   knobValue)                          FIVEWORDINLINE(0x2F3C, 0x0008, 0x0013, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetPartName                (MusicComponent         mc,
                                 long                   part,
                                 StringPtr              name)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x0014, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetPartName                (MusicComponent         mc,
                                 long                   part,
                                 StringPtr              name)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x0015, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicFindTone                   (MusicComponent         mc,
                                 ToneDescription *      td,
                                 long *                 libraryIndexOut,
                                 unsigned long *        fit)                                FIVEWORDINLINE(0x2F3C, 0x000C, 0x0016, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicPlayNote                   (MusicComponent         mc,
                                 long                   part,
                                 long                   pitch,
                                 long                   velocity)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x0017, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicResetPart                  (MusicComponent         mc,
                                 long                   part)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0018, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetPartController          (MusicComponent         mc,
                                 long                   part,
                                 MusicController        controllerNumber,
                                 long                   controllerValue)                    FIVEWORDINLINE(0x2F3C, 0x000C, 0x0019, 0x7000, 0xA82A);

#if OLDROUTINENAMES
#define MusicSetController(ci,part,controllerNumber,controllerValue) MusicSetPartController(ci, part,controllerNumber,controllerValue)
#endif

EXTERN_API( ComponentResult )
MusicGetPartController          (MusicComponent         mc,
                                 long                   part,
                                 MusicController        controllerNumber)                   FIVEWORDINLINE(0x2F3C, 0x0008, 0x001A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetMIDIProc                (MusicComponent         mc,
                                 MusicMIDISendUPP *     midiSendProc,
                                 long *                 refCon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x001B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetMIDIProc                (MusicComponent         mc,
                                 MusicMIDISendUPP       midiSendProc,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x001C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetInstrumentNames         (MusicComponent         mc,
                                 long                   modifiableInstruments,
                                 Handle *               instrumentNames,
                                 Handle *               instrumentCategoryLasts,
                                 Handle *               instrumentCategoryNames)            FIVEWORDINLINE(0x2F3C, 0x0010, 0x001D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetDrumNames               (MusicComponent         mc,
                                 long                   modifiableInstruments,
                                 Handle *               instrumentNumbers,
                                 Handle *               instrumentNames)                    FIVEWORDINLINE(0x2F3C, 0x000C, 0x001E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetMasterTune              (MusicComponent         mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x001F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetMasterTune              (MusicComponent         mc,
                                 long                   masterTune)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0020, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
MusicGetInstrumentAboutInfo     (MusicComponent         mc,
                                 long                   part,
                                 InstrumentAboutInfo *  iai)                                FIVEWORDINLINE(0x2F3C, 0x0008, 0x0022, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetDeviceConnection        (MusicComponent         mc,
                                 long                   index,
                                 long *                 id1,
                                 long *                 id2)                                FIVEWORDINLINE(0x2F3C, 0x000C, 0x0023, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicUseDeviceConnection        (MusicComponent         mc,
                                 long                   id1,
                                 long                   id2)                                FIVEWORDINLINE(0x2F3C, 0x0008, 0x0024, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetKnobSettingStrings      (MusicComponent         mc,
                                 long                   knobIndex,
                                 long                   isGlobal,
                                 Handle *               settingsNames,
                                 Handle *               settingsCategoryLasts,
                                 Handle *               settingsCategoryNames)              FIVEWORDINLINE(0x2F3C, 0x0014, 0x0025, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetMIDIPorts               (MusicComponent         mc,
                                 long *                 inputPortCount,
                                 long *                 outputPortCount)                    FIVEWORDINLINE(0x2F3C, 0x0008, 0x0026, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSendMIDI                   (MusicComponent         mc,
                                 long                   portIndex,
                                 MusicMIDIPacket *      mp)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0027, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicReceiveMIDI                (MusicComponent         mc,
                                 MusicMIDIReadHookUPP   readHook,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x0028, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicStartOffline               (MusicComponent         mc,
                                 unsigned long *        numChannels,
                                 UnsignedFixed *        sampleRate,
                                 unsigned short *       sampleSize,
                                 MusicOfflineDataUPP    dataProc,
                                 long                   dataProcRefCon)                     FIVEWORDINLINE(0x2F3C, 0x0014, 0x0029, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetOfflineTimeTo           (MusicComponent         mc,
                                 long                   newTimeStamp)                       FIVEWORDINLINE(0x2F3C, 0x0004, 0x002A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetInstrumentKnobDescription (MusicComponent       mc,
                                 long                   knobIndex,
                                 KnobDescription *      mkd)                                FIVEWORDINLINE(0x2F3C, 0x0008, 0x002B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetDrumKnobDescription     (MusicComponent         mc,
                                 long                   knobIndex,
                                 KnobDescription *      mkd)                                FIVEWORDINLINE(0x2F3C, 0x0008, 0x002C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetKnobDescription         (MusicComponent         mc,
                                 long                   knobIndex,
                                 KnobDescription *      mkd)                                FIVEWORDINLINE(0x2F3C, 0x0008, 0x002D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGetInfoText                (MusicComponent         mc,
                                 long                   selector,
                                 Handle *               textH,
                                 Handle *               styleH)                             FIVEWORDINLINE(0x2F3C, 0x000C, 0x002E, 0x7000, 0xA82A);

enum {
    kGetInstrumentInfoNoBuiltIn = 1 << 0,
    kGetInstrumentInfoMidiUserInst = 1 << 1,
    kGetInstrumentInfoNoIText   = 1 << 2
};

EXTERN_API( ComponentResult )
MusicGetInstrumentInfo          (MusicComponent         mc,
                                 long                   getInstrumentInfoFlags,
                                 InstrumentInfoListHandle * infoListH)                      FIVEWORDINLINE(0x2F3C, 0x0008, 0x002F, 0x7000, 0xA82A);




EXTERN_API( ComponentResult )
MusicTask                       (MusicComponent         mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0031, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetPartInstrumentNumberInterruptSafe (MusicComponent  mc,
                                 long                   part,
                                 long                   instrumentNumber)                   FIVEWORDINLINE(0x2F3C, 0x0008, 0x0032, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicSetPartSoundLocalization   (MusicComponent         mc,
                                 long                   part,
                                 Handle                 data)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x0033, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGenericConfigure           (MusicComponent         mc,
                                 long                   mode,
                                 long                   flags,
                                 long                   baseResID)                          FIVEWORDINLINE(0x2F3C, 0x000C, 0x0100, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGenericGetPart             (MusicComponent         mc,
                                 long                   partNumber,
                                 GCPart **              part)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x0101, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGenericGetKnobList         (MusicComponent         mc,
                                 long                   knobType,
                                 GenericKnobDescriptionListHandle * gkdlH)                  FIVEWORDINLINE(0x2F3C, 0x0008, 0x0102, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicGenericSetResourceNumbers  (MusicComponent         mc,
                                 Handle                 resourceIDH)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x0103, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicDerivedMIDISend            (MusicComponent         mc,
                                 MusicMIDIPacket *      packet)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0200, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicDerivedSetKnob             (MusicComponent         mc,
                                 long                   knobType,
                                 long                   knobNumber,
                                 long                   knobValue,
                                 long                   partNumber,
                                 GCPart *               p,
                                 GenericKnobDescription * gkd)                              FIVEWORDINLINE(0x2F3C, 0x0018, 0x0201, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicDerivedSetPart             (MusicComponent         mc,
                                 long                   partNumber,
                                 GCPart *               p)                                  FIVEWORDINLINE(0x2F3C, 0x0008, 0x0202, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicDerivedSetInstrument       (MusicComponent         mc,
                                 long                   partNumber,
                                 GCPart *               p)                                  FIVEWORDINLINE(0x2F3C, 0x0008, 0x0203, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicDerivedSetPartInstrumentNumber (MusicComponent     mc,
                                 long                   partNumber,
                                 GCPart *               p)                                  FIVEWORDINLINE(0x2F3C, 0x0008, 0x0204, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicDerivedSetMIDI             (MusicComponent         mc,
                                 MusicMIDISendUPP       midiProc,
                                 long                   refcon,
                                 long                   midiChannel)                        FIVEWORDINLINE(0x2F3C, 0x000C, 0x0205, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicDerivedStorePartInstrument (MusicComponent         mc,
                                 long                   partNumber,
                                 GCPart *               p,
                                 long                   instrumentNumber)                   FIVEWORDINLINE(0x2F3C, 0x000C, 0x0206, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicDerivedOpenResFile         (MusicComponent         mc)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0207, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
MusicDerivedCloseResFile        (MusicComponent         mc,
                                 short                  resRefNum)                          FIVEWORDINLINE(0x2F3C, 0x0002, 0x0208, 0x7000, 0xA82A);






/* Mask bit for returned value by InstrumentFind.*/
enum {
    kInstrumentExactMatch       = 0x00020000,
    kInstrumentRecommendedSubstitute = 0x00010000,
    kInstrumentQualityField     = (long)0xFF000000,
    kInstrumentRoland8BitQuality = 0x05000000
};


typedef InstrumentAboutInfo *           InstrumentAboutInfoPtr;
typedef InstrumentAboutInfoPtr *        InstrumentAboutInfoHandle;

struct GMInstrumentInfo {
    long                            cmpInstID;
    long                            gmInstNum;
    long                            instMatch;
};
typedef struct GMInstrumentInfo         GMInstrumentInfo;
typedef GMInstrumentInfo *              GMInstrumentInfoPtr;
typedef GMInstrumentInfoPtr *           GMInstrumentInfoHandle;

struct nonGMInstrumentInfoRecord {
    long                            cmpInstID;                  /* if 0, category name*/
    long                            flags;                      /* match, show in picker*/
    long                            toneNameIndex;              /* index in toneNames (1 based)*/
    long                            itxtNameAtomID;             /* index in itxtNames (itxt/name by index)*/
};
typedef struct nonGMInstrumentInfoRecord nonGMInstrumentInfoRecord;

struct nonGMInstrumentInfo {
    long                            recordCount;
    Handle                          toneNames;                  /* name from tone description*/
    QTAtomContainer                 itxtNames;                  /* itext/name atoms for instruments*/
    nonGMInstrumentInfoRecord       instInfo[1];
};
typedef struct nonGMInstrumentInfo      nonGMInstrumentInfo;
typedef nonGMInstrumentInfo *           nonGMInstrumentInfoPtr;
typedef nonGMInstrumentInfoPtr *        nonGMInstrumentInfoHandle;

struct InstCompInfo {
    long                            infoSize;                   /* size of this record*/
    Str31                           InstrumentLibraryName;
    QTAtomContainer                 InstrumentLibraryITxt;      /* itext/name atoms for instruments*/
    long                            GMinstrumentCount;
    GMInstrumentInfoHandle          GMinstrumentInfo;
    long                            GMdrumCount;
    GMInstrumentInfoHandle          GMdrumInfo;
    long                            nonGMinstrumentCount;
    nonGMInstrumentInfoHandle       nonGMinstrumentInfo;
};
typedef struct InstCompInfo             InstCompInfo;
typedef InstCompInfo *                  InstCompInfoPtr;
typedef InstCompInfoPtr *               InstCompInfoHandle;
EXTERN_API( ComponentResult )
InstrumentGetInst               (ComponentInstance      ci,
                                 long                   instID,
                                 AtomicInstrument *     atomicInst,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
InstrumentGetInfo               (ComponentInstance      ci,
                                 long                   getInstrumentInfoFlags,
                                 InstCompInfoHandle *   instInfo)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
InstrumentInitialize            (ComponentInstance      ci,
                                 long                   initFormat,
                                 void *                 initParams)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
InstrumentOpenComponentResFile  (ComponentInstance      ci,
                                 short *                resFile)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
InstrumentCloseComponentResFile (ComponentInstance      ci,
                                 short                  resFile)                            FIVEWORDINLINE(0x2F3C, 0x0002, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
InstrumentGetComponentRefCon    (ComponentInstance      ci,
                                 void **                refCon)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
InstrumentSetComponentRefCon    (ComponentInstance      ci,
                                 void *                 refCon)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
InstrumentGetSynthesizerType    (ComponentInstance      ci,
                                 OSType *               synthesizerType)                    FIVEWORDINLINE(0x2F3C, 0x0004, 0x0008, 0x7000, 0xA82A);



/*--------------------------
    Types
--------------------------*/
enum {
    kNoteRequestNoGM            = 1,                            /* dont degrade to a GM synth */
    kNoteRequestNoSynthType     = 2,                            /* dont degrade to another synth of same type but different name */
    kNoteRequestSynthMustMatch  = 4                             /* synthType must be a match, including kGMSynthComponentSubType */
};


enum {
    kNoteRequestSpecifyMIDIChannel = 0x80
};


typedef ComponentInstance               NoteAllocator;
/*
    The midiChannelAssignment field of this structure is used to assign a MIDI channel 
    when a NoteChannel is created from a NoteRequest.
    A value of 0 indicates a MIDI channel has *not* been assigned
    A value of (kNoteRequestSpecifyMIDIChannel | 1->16) is a MIDI channel assignment
*/
typedef UInt8                           NoteRequestMIDIChannel;

struct NoteRequestInfo {
    UInt8                           flags;                      /* 1: dont accept GM match, 2: dont accept same-synth-type match */
    NoteRequestMIDIChannel          midiChannelAssignment;      /* (kNoteRequestSpecifyMIDIChannel | 1->16) as MIDI Channel assignement or zero - see notes above  */
    BigEndianShort                  polyphony;                  /* Maximum number of voices */
    BigEndianFixed                  typicalPolyphony;           /* Hint for level mixing */
};
typedef struct NoteRequestInfo          NoteRequestInfo;

struct NoteRequest {
    NoteRequestInfo                 info;
    ToneDescription                 tone;
};
typedef struct NoteRequest              NoteRequest;
typedef struct OpaqueNoteChannel*       NoteChannel;


enum {
    kPickDontMix                = 1,                            /* dont mix instruments with drum sounds */
    kPickSameSynth              = 2,                            /* only allow the same device that went in, to come out */
    kPickUserInsts              = 4,                            /* show user insts in addition to ROM voices */
    kPickEditAllowEdit          = 8,                            /* lets user switch over to edit mode */
    kPickEditAllowPick          = 16,                           /* lets the user switch over to pick mode */
    kPickEditSynthGlobal        = 32,                           /* edit the global knobs of the synth */
    kPickEditControllers        = 64                            /* edit the controllers of the notechannel */
};


enum {
    kNoteAllocatorComponentType = FOUR_CHAR_CODE('nota')
};


/*--------------------------------
    Note Allocator Prototypes
--------------------------------*/
EXTERN_API( ComponentResult )
NARegisterMusicDevice           (NoteAllocator          na,
                                 OSType                 synthType,
                                 Str31                  name,
                                 SynthesizerConnections * connections)                      FIVEWORDINLINE(0x2F3C, 0x000C, 0x0000, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAUnregisterMusicDevice         (NoteAllocator          na,
                                 long                   index)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0001, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAGetRegisteredMusicDevice      (NoteAllocator          na,
                                 long                   index,
                                 OSType *               synthType,
                                 Str31                  name,
                                 SynthesizerConnections * connections,
                                 MusicComponent *       mc)                                 FIVEWORDINLINE(0x2F3C, 0x0014, 0x0002, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NASaveMusicConfiguration        (NoteAllocator          na)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0003, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NANewNoteChannel                (NoteAllocator          na,
                                 NoteRequest *          noteRequest,
                                 NoteChannel *          outChannel)                         FIVEWORDINLINE(0x2F3C, 0x0008, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NADisposeNoteChannel            (NoteAllocator          na,
                                 NoteChannel            noteChannel)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAGetNoteChannelInfo            (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 long *                 index,
                                 long *                 part)                               FIVEWORDINLINE(0x2F3C, 0x000C, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAPrerollNoteChannel            (NoteAllocator          na,
                                 NoteChannel            noteChannel)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAUnrollNoteChannel             (NoteAllocator          na,
                                 NoteChannel            noteChannel)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x0008, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
NASetNoteChannelVolume          (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 Fixed                  volume)                             FIVEWORDINLINE(0x2F3C, 0x0008, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAResetNoteChannel              (NoteAllocator          na,
                                 NoteChannel            noteChannel)                        FIVEWORDINLINE(0x2F3C, 0x0004, 0x000C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAPlayNote                      (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 long                   pitch,
                                 long                   velocity)                           FIVEWORDINLINE(0x2F3C, 0x000C, 0x000D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NASetController                 (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 long                   controllerNumber,
                                 long                   controllerValue)                    FIVEWORDINLINE(0x2F3C, 0x000C, 0x000E, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NASetKnob                       (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 long                   knobNumber,
                                 long                   knobValue)                          FIVEWORDINLINE(0x2F3C, 0x000C, 0x000F, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAFindNoteChannelTone           (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 ToneDescription *      td,
                                 long *                 instrumentNumber)                   FIVEWORDINLINE(0x2F3C, 0x000C, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NASetInstrumentNumber           (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 long                   instrumentNumber)                   FIVEWORDINLINE(0x2F3C, 0x0008, 0x0011, 0x7000, 0xA82A);



#if OLDROUTINENAMES
#define NASetNoteChannelInstrument(ci, noteChannel,instrumentNumber ) NASetInstrumentNumber(ci, noteChannel,instrumentNumber)
#define NASetInstrument(ci, noteChannel,instrumentNumber ) NASetInstrumentNumber(ci, noteChannel,instrumentNumber)
#endif
EXTERN_API( ComponentResult )
NAPickInstrument                (NoteAllocator          na,
                                 ModalFilterUPP         filterProc,
                                 StringPtr              prompt,
                                 ToneDescription *      sd,
                                 unsigned long          flags,
                                 long                   refCon,
                                 long                   reserved1,
                                 long                   reserved2)                          FIVEWORDINLINE(0x2F3C, 0x001C, 0x0012, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAPickArrangement               (NoteAllocator          na,
                                 ModalFilterUPP         filterProc,
                                 StringPtr              prompt,
                                 long                   zero1,
                                 long                   zero2,
                                 Track                  t,
                                 StringPtr              songName)                           FIVEWORDINLINE(0x2F3C, 0x0018, 0x0013, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
NASetDefaultMIDIInput           (NoteAllocator          na,
                                 SynthesizerConnections * sc)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0015, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAGetDefaultMIDIInput           (NoteAllocator          na,
                                 SynthesizerConnections * sc)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x0016, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
NAUseDefaultMIDIInput           (NoteAllocator          na,
                                 MusicMIDIReadHookUPP   readHook,
                                 long                   refCon,
                                 unsigned long          flags)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0019, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NALoseDefaultMIDIInput          (NoteAllocator          na)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x001A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAStuffToneDescription          (NoteAllocator          na,
                                 long                   gmNumber,
                                 ToneDescription *      td)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x001B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NACopyrightDialog               (NoteAllocator          na,
                                 PicHandle              p,
                                 StringPtr              author,
                                 StringPtr              copyright,
                                 StringPtr              other,
                                 StringPtr              title,
                                 ModalFilterUPP         filterProc,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x001C, 0x001C, 0x7000, 0xA82A);


/*
    kNADummyOneSelect = 29
    kNADummyTwoSelect = 30
*/

EXTERN_API( ComponentResult )
NAGetIndNoteChannel             (NoteAllocator          na,
                                 long                   index,
                                 NoteChannel *          nc,
                                 long *                 seed)                               FIVEWORDINLINE(0x2F3C, 0x000C, 0x001F, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
NAGetMIDIPorts                  (NoteAllocator          na,
                                 QTMIDIPortListHandle * inputPorts,
                                 QTMIDIPortListHandle * outputPorts)                        FIVEWORDINLINE(0x2F3C, 0x0008, 0x0021, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAGetNoteRequest                (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 NoteRequest *          nrOut)                              FIVEWORDINLINE(0x2F3C, 0x0008, 0x0022, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NASendMIDI                      (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 MusicMIDIPacket *      mp)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0023, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAPickEditInstrument            (NoteAllocator          na,
                                 ModalFilterUPP         filterProc,
                                 StringPtr              prompt,
                                 long                   refCon,
                                 NoteChannel            nc,
                                 AtomicInstrument       ai,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x0018, 0x0024, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NANewNoteChannelFromAtomicInstrument (NoteAllocator     na,
                                 AtomicInstrumentPtr    instrument,
                                 long                   flags,
                                 NoteChannel *          outChannel)                         FIVEWORDINLINE(0x2F3C, 0x000C, 0x0025, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NASetAtomicInstrument           (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 AtomicInstrumentPtr    instrument,
                                 long                   flags)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0026, 0x7000, 0xA82A);



EXTERN_API( ComponentResult )
NAGetKnob                       (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 long                   knobNumber,
                                 long *                 knobValue)                          FIVEWORDINLINE(0x2F3C, 0x000C, 0x0028, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NATask                          (NoteAllocator          na)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0029, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NASetNoteChannelBalance         (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 long                   balance)                            FIVEWORDINLINE(0x2F3C, 0x0008, 0x002A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NASetInstrumentNumberInterruptSafe (NoteAllocator       na,
                                 NoteChannel            noteChannel,
                                 long                   instrumentNumber)                   FIVEWORDINLINE(0x2F3C, 0x0008, 0x002B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NASetNoteChannelSoundLocalization (NoteAllocator        na,
                                 NoteChannel            noteChannel,
                                 Handle                 data)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x002C, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
NAGetController                 (NoteAllocator          na,
                                 NoteChannel            noteChannel,
                                 long                   controllerNumber,
                                 long *                 controllerValue)                    FIVEWORDINLINE(0x2F3C, 0x000C, 0x002D, 0x7000, 0xA82A);





enum {
    kTuneQueueDepth             = 8                             /* Deepest you can queue tune segments */
};



struct TuneStatus {
    unsigned long *                 tune;                       /* currently playing tune */
    unsigned long *                 tunePtr;                    /* position within currently playing piece */
    TimeValue                       time;                       /* current tune time */
    short                           queueCount;                 /* how many pieces queued up? */
    short                           queueSpots;                 /* How many more tunepieces can be queued */
    TimeValue                       queueTime;                  /* How much time is queued up? (can be very inaccurate) */
    long                            reserved[3];
};
typedef struct TuneStatus               TuneStatus;
typedef CALLBACK_API( void , TuneCallBackProcPtr )(const TuneStatus *status, long refCon);
typedef CALLBACK_API( void , TunePlayCallBackProcPtr )(unsigned long *event, long seed, long refCon);
typedef STACK_UPP_TYPE(TuneCallBackProcPtr)                     TuneCallBackUPP;
typedef STACK_UPP_TYPE(TunePlayCallBackProcPtr)                 TunePlayCallBackUPP;

typedef ComponentInstance               TunePlayer;
enum {
    kTunePlayerComponentType    = FOUR_CHAR_CODE('tune')
};


EXTERN_API( ComponentResult )
TuneSetHeader                   (TunePlayer             tp,
                                 unsigned long *        header)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0004, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneGetTimeBase                 (TunePlayer             tp,
                                 TimeBase *             tb)                                 FIVEWORDINLINE(0x2F3C, 0x0004, 0x0005, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneSetTimeScale                (TunePlayer             tp,
                                 TimeScale              scale)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0006, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneGetTimeScale                (TunePlayer             tp,
                                 TimeScale *            scale)                              FIVEWORDINLINE(0x2F3C, 0x0004, 0x0007, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneGetIndexedNoteChannel       (TunePlayer             tp,
                                 long                   i,
                                 NoteChannel *          nc)                                 FIVEWORDINLINE(0x2F3C, 0x0008, 0x0008, 0x7000, 0xA82A);


/* Values for when to start. */
enum {
    kTuneStartNow               = 1,                            /* start after buffer is implied */
    kTuneDontClipNotes          = 2,                            /* allow notes to finish their durations outside sample */
    kTuneExcludeEdgeNotes       = 4,                            /* dont play notes that start at end of tune */
    kTuneQuickStart             = 8,                            /* Leave all the controllers where they are, ignore start time */
    kTuneLoopUntil              = 16,                           /* loop a queued tune if there's nothing else in the queue*/
    kTunePlayDifference         = 32,                           /* by default, the tune difference is skipped*/
    kTunePlayConcurrent         = 64,                           /* dont block the next tune sequence with this one*/
    kTuneStartNewMaster         = 16384
};

EXTERN_API( ComponentResult )
TuneQueue                       (TunePlayer             tp,
                                 unsigned long *        tune,
                                 Fixed                  tuneRate,
                                 unsigned long          tuneStartPosition,
                                 unsigned long          tuneStopPosition,
                                 unsigned long          queueFlags,
                                 TuneCallBackUPP        callBackProc,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x001C, 0x000A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneInstant                     (TunePlayer             tp,
                                 unsigned long *        tune,
                                 unsigned long          tunePosition)                       FIVEWORDINLINE(0x2F3C, 0x0008, 0x000B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneGetStatus                   (TunePlayer             tp,
                                 TuneStatus *           status)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x000C, 0x7000, 0xA82A);

/* Values for stopping. */
enum {
    kTuneStopFade               = 1,                            /* do a quick, synchronous fadeout */
    kTuneStopSustain            = 2,                            /* don't silece notes */
    kTuneStopInstant            = 4,                            /* silence notes fast (else, decay them) */
    kTuneStopReleaseChannels    = 8                             /* afterwards, let the channels go */
};

EXTERN_API( ComponentResult )
TuneStop                        (TunePlayer             tp,
                                 long                   stopFlags)                          FIVEWORDINLINE(0x2F3C, 0x0004, 0x000D, 0x7000, 0xA82A);


EXTERN_API( ComponentResult )
TuneSetVolume                   (TunePlayer             tp,
                                 Fixed                  volume)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0010, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneGetVolume                   (TunePlayer             tp)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0011, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TunePreroll                     (TunePlayer             tp)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0012, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneUnroll                      (TunePlayer             tp)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0013, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneSetNoteChannels             (TunePlayer             tp,
                                 unsigned long          count,
                                 NoteChannel *          noteChannelList,
                                 TunePlayCallBackUPP    playCallBackProc,
                                 long                   refCon)                             FIVEWORDINLINE(0x2F3C, 0x0010, 0x0014, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneSetPartTranspose            (TunePlayer             tp,
                                 unsigned long          part,
                                 long                   transpose,
                                 long                   velocityShift)                      FIVEWORDINLINE(0x2F3C, 0x000C, 0x0015, 0x7000, 0xA82A);


EXTERN_API( NoteAllocator )
TuneGetNoteAllocator            (TunePlayer             tp)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0017, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneSetSofter                   (TunePlayer             tp,
                                 long                   softer)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0018, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneTask                        (TunePlayer             tp)                                 FIVEWORDINLINE(0x2F3C, 0x0000, 0x0019, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneSetBalance                  (TunePlayer             tp,
                                 long                   balance)                            FIVEWORDINLINE(0x2F3C, 0x0004, 0x001A, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneSetSoundLocalization        (TunePlayer             tp,
                                 Handle                 data)                               FIVEWORDINLINE(0x2F3C, 0x0004, 0x001B, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneSetHeaderWithSize           (TunePlayer             tp,
                                 unsigned long *        header,
                                 unsigned long          size)                               FIVEWORDINLINE(0x2F3C, 0x0008, 0x001C, 0x7000, 0xA82A);

/* flags for part mix. */
enum {
    kTuneMixMute                = 1,                            /* disable a part */
    kTuneMixSolo                = 2                             /* if any parts soloed, play only soloed parts */
};


EXTERN_API( ComponentResult )
TuneSetPartMix                  (TunePlayer             tp,
                                 unsigned long          partNumber,
                                 long                   volume,
                                 long                   balance,
                                 long                   mixFlags)                           FIVEWORDINLINE(0x2F3C, 0x0010, 0x001D, 0x7000, 0xA82A);

EXTERN_API( ComponentResult )
TuneGetPartMix                  (TunePlayer             tp,
                                 unsigned long          partNumber,
                                 long *                 volumeOut,
                                 long *                 balanceOut,
                                 long *                 mixFlagsOut)                        FIVEWORDINLINE(0x2F3C, 0x0010, 0x001E, 0x7000, 0xA82A);





typedef unsigned long                   MusicOpWord;
typedef MusicOpWord *                   MusicOpWordPtr;
/*    QuickTime Music Track Event Formats:

    At this time, QuickTime music tracks support 5 different event types -- REST events,
    short NOTE events, short CONTROL events, short GENERAL events, Long NOTE events, 
    long CONTROL events, and variable GENERAL events.
 
        o REST Event (4 bytes/event):
    
            (0 0 0) (5-bit UNUSED) (24-bit Rest Duration)
        
        o.Short NOTE Events (4 bytes/event):
    
            (0 0 1) (5-bit Part) (6-bit Pitch) (7-bit Volume) (11-bit Duration)
        
            where:  Pitch is offset by 32 (Actual pitch = pitch field + 32)

        o.Short CONTROL Events (4 bytes/event):
    
            (0 1 0) (5-bit Part) (8-bit Controller) (1-bit UNUSED) (1-bit Sign) (7-bit MSB) (7-bit LSB)
                                                                         ( or 15-bit Signed Value)
        o Short GENERAL Event (4 bytes/event):
    
            (0 1 1) (1-bit UNUSED) (12-bit Sub-Type) (16-bit Value)
    
        o Long NOTE Events (8 bytes/event):
    
            (1 0 0 1) (12-bit Part) (1-bit UNUSED) (7-bit Pitch) (1-bit UNUSED) (7-bit Volume)
            (1 0) (8-bit UNUSED) (22-bit Duration)
        
        o.Long CONTROL Event (8 bytes/event):
        
            (1 0 1 0) (12-bit Part) (16-bit Value MSB) 
            (1 0) (14-bit Controller) (16-bit Value LSB)
    
        o.Long KNOB Event (8 bytes/event):
    
            (1 0 1 1) (12-bit Sub-Type) (16-bit Value MSB)
            (1 0) (14-bit KNOB) (16-bit Value LSB)
    
        o.Variable GENERAL Length Events (N bytes/event):
    
            (1 1 1 1) (12-bit Sub-Type) (16-bit Length)
                :
            (32-bit Data values)
                :
            (1 1) (14-bit UNUSED) (16-bit Length)
    
            where:  Length field is the number of LONG words in the record.
                    Lengths include the first and last long words (Minimum length = 2)
                
    The following event type values have not been used yet and are reserved for 
    future expansion:
        
        o (1 0 0 0)     (8 bytes/event)
        o (1 1 0 0)     (N bytes/event)
        o (1 1 0 1)     (N bytes/event)
        o (1 1 1 0)     (N bytes/event)
        
    For all events, the following generalizations apply:
    
        -   All duration values are specified in Millisecond units.
        -   Pitch values are intended to map directly to the MIDI key numbers.
        -   Controllers from 0 to 127 correspond to the standard MIDI controllers.
            Controllers greater than 127 correspond to other controls (i.e., Pitch Bend, 
            Key Pressure, and Channel Pressure).    
*/

/* Defines for the implemented music event data fields*/
enum {
    kRestEventType              = 0x00000000,                   /* lower 3-bits */
    kNoteEventType              = 0x00000001,                   /* lower 3-bits */
    kControlEventType           = 0x00000002,                   /* lower 3-bits */
    kMarkerEventType            = 0x00000003,                   /* lower 3-bits */
    kUndefined1EventType        = 0x00000008,                   /* 4-bits */
    kXNoteEventType             = 0x00000009,                   /* 4-bits */
    kXControlEventType          = 0x0000000A,                   /* 4-bits */
    kKnobEventType              = 0x0000000B,                   /* 4-bits */
    kUndefined2EventType        = 0x0000000C,                   /* 4-bits */
    kUndefined3EventType        = 0x0000000D,                   /* 4-bits */
    kUndefined4EventType        = 0x0000000E,                   /* 4-bits */
    kGeneralEventType           = 0x0000000F,                   /* 4-bits */
    kXEventLengthBits           = 0x00000002,                   /* 2 bits: indicates 8-byte event record */
    kGeneralEventLengthBits     = 0x00000003,                   /* 2 bits: indicates variable length event record */
    kEventLen                   = 1L,                           /* length of events in long words */
    kXEventLen                  = 2L,
    kRestEventLen               = kEventLen,                    /* length of events in long words */
    kNoteEventLen               = kEventLen,
    kControlEventLen            = kEventLen,
    kMarkerEventLen             = kEventLen,
    kXNoteEventLen              = kXEventLen,
    kXControlEventLen           = kXEventLen,
    kGeneralEventLen            = kXEventLen,                   /* 2 or more, however */
                                                                /* Universal Event Defines*/
    kEventLengthFieldPos        = 30,                           /* by looking at these two bits of the 1st or last word         */
    kEventLengthFieldWidth      = 2,                            /* of an event you can determine the event length                */
                                                                /* length field: 0 & 1 => 1 long; 2 => 2 longs; 3 => variable length */
    kEventTypeFieldPos          = 29,                           /* event type field for short events */
    kEventTypeFieldWidth        = 3,                            /* short type is 3 bits */
    kXEventTypeFieldPos         = 28,                           /* event type field for extended events */
    kXEventTypeFieldWidth       = 4,                            /* extended type is 4 bits */
    kEventPartFieldPos          = 24,
    kEventPartFieldWidth        = 5,
    kXEventPartFieldPos         = 16,                           /* in the 1st long word */
    kXEventPartFieldWidth       = 12,                           /* Rest Events*/
    kRestEventDurationFieldPos  = 0,
    kRestEventDurationFieldWidth = 24,
    kRestEventDurationMax       = ((1L << kRestEventDurationFieldWidth) - 1), /* Note Events*/
    kNoteEventPitchFieldPos     = 18,
    kNoteEventPitchFieldWidth   = 6,
    kNoteEventPitchOffset       = 32,                           /* add to value in pitch field to get actual pitch */
    kNoteEventVolumeFieldPos    = 11,
    kNoteEventVolumeFieldWidth  = 7,
    kNoteEventVolumeOffset      = 0,                            /* add to value in volume field to get actual volume */
    kNoteEventDurationFieldPos  = 0,
    kNoteEventDurationFieldWidth = 11,
    kNoteEventDurationMax       = ((1L << kNoteEventDurationFieldWidth) - 1),
    kXNoteEventPitchFieldPos    = 0,                            /* in the 1st long word */
    kXNoteEventPitchFieldWidth  = 16,
    kXNoteEventDurationFieldPos = 0,                            /* in the 2nd long word */
    kXNoteEventDurationFieldWidth = 22,
    kXNoteEventDurationMax      = ((1L << kXNoteEventDurationFieldWidth) - 1),
    kXNoteEventVolumeFieldPos   = 22,                           /* in the 2nd long word */
    kXNoteEventVolumeFieldWidth = 7,                            /* Control Events*/
    kControlEventControllerFieldPos = 16,
    kControlEventControllerFieldWidth = 8,
    kControlEventValueFieldPos  = 0,
    kControlEventValueFieldWidth = 16,
    kXControlEventControllerFieldPos = 0,                       /* in the 2nd long word */
    kXControlEventControllerFieldWidth = 16,
    kXControlEventValueFieldPos = 0,                            /* in the 1st long word */
    kXControlEventValueFieldWidth = 16,                         /* Knob Events*/
    kKnobEventValueHighFieldPos = 0,                            /* 1st long word */
    kKnobEventValueHighFieldWidth = 16,
    kKnobEventKnobFieldPos      = 16,                           /* 2nd long word */
    kKnobEventKnobFieldWidth    = 14,
    kKnobEventValueLowFieldPos  = 0,                            /* 2nd long word */
    kKnobEventValueLowFieldWidth = 16,                          /* Marker Events*/
    kMarkerEventSubtypeFieldPos = 16,
    kMarkerEventSubtypeFieldWidth = 8,
    kMarkerEventValueFieldPos   = 0,
    kMarkerEventValueFieldWidth = 16,                           /* General Events*/
    kGeneralEventSubtypeFieldPos = 16,                          /* in the last long word */
    kGeneralEventSubtypeFieldWidth = 14,
    kGeneralEventLengthFieldPos = 0,                            /* in the 1st & last long words */
    kGeneralEventLengthFieldWidth = 16
};

#if TARGET_RT_LITTLE_ENDIAN
enum {
    kEndMarkerValue             = 0x00000060
};

#else
enum {
    kEndMarkerValue             = 0x60000000
};

#endif  /* TARGET_RT_LITTLE_ENDIAN */

/* macros for extracting various fields from the QuickTime event records*/

#define qtma_MASK(bitWidth)             ((1L << (bitWidth)) - 1)
#define qtma_EXT(val, pos, width)       ((EndianU32_BtoN(val) >> (pos)) & qtma_MASK(width))
#define qtma_EventLengthForward(xP,ulen)        \
    {                                           \
        unsigned long _ext;                         \
        unsigned long *lP = (unsigned long *)(xP);  \
        _ext = qtma_EXT(*lP, kEventLengthFieldPos, kEventLengthFieldWidth); \
        if (_ext != 3) {                        \
            ulen = (_ext < 2) ? 1 : 2;          \
        } else {                                \
            ulen = (unsigned short)qtma_EXT(*lP, kGeneralEventLengthFieldPos, kGeneralEventLengthFieldWidth); \
            if (ulen < 2) {                     \
                ulen = lP[1];                   \
            }                                   \
        }                                       \
    }
#define qtma_EventLengthBackward(xP,ulen)       \
    {                                           \
        unsigned long _ext;                     \
        unsigned long *lP = (unsigned long *)(xP); \
        _ext = qtma_EXT(*lP, kEventLengthFieldPos, kEventLengthFieldWidth);     \
        if (_ext != 3) {                        \
            ulen = (_ext < 2) ? 1 : 2;          \
        } else {                                \
            ulen = (unsigned short)qtma_EXT(*lP, kGeneralEventLengthFieldPos, kGeneralEventLengthFieldWidth);       \
            if (ulen < 2) {                     \
                ulen = lP[-1];                  \
            }                                   \
        }                                       \
    }
#define qtma_EventType(x)               ((qtma_EXT(x, kEventTypeFieldPos, kEventTypeFieldWidth) > 3) ? qtma_EXT(x, kXEventTypeFieldPos, kXEventTypeFieldWidth) : qtma_EXT(x, kEventTypeFieldPos, kEventTypeFieldWidth))
#define qtma_RestDuration(x)            (qtma_EXT(x, kRestEventDurationFieldPos, kRestEventDurationFieldWidth))
#define qtma_Part(x)                    (qtma_EXT(x, kEventPartFieldPos, kEventPartFieldWidth))
#define qtma_XPart(m, l)                (qtma_EXT(m, kXEventPartFieldPos, kXEventPartFieldWidth))
#define qtma_NotePitch(x)               (qtma_EXT(x, kNoteEventPitchFieldPos, kNoteEventPitchFieldWidth) + kNoteEventPitchOffset)
#define qtma_NoteVolume(x)              (qtma_EXT(x, kNoteEventVolumeFieldPos, kNoteEventVolumeFieldWidth) + kNoteEventVolumeOffset)
#define qtma_NoteDuration(x)            (qtma_EXT(x, kNoteEventDurationFieldPos, kNoteEventDurationFieldWidth))
#define qtma_NoteVelocity qtma_NoteVolume
#define qtma_XNotePitch(m, l)           (qtma_EXT(m, kXNoteEventPitchFieldPos, kXNoteEventPitchFieldWidth))
#define qtma_XNoteVolume(m, l)          (qtma_EXT(l, kXNoteEventVolumeFieldPos, kXNoteEventVolumeFieldWidth))
#define qtma_XNoteDuration(m, l)        (qtma_EXT(l, kXNoteEventDurationFieldPos, kXNoteEventDurationFieldWidth))
#define qtma_XNoteVelocity qtma_XNoteVolume
#define qtma_ControlController(x)       (qtma_EXT(x, kControlEventControllerFieldPos, kControlEventControllerFieldWidth))
#define qtma_ControlValue(x)            (qtma_EXT(x, kControlEventValueFieldPos, kControlEventValueFieldWidth))
#define qtma_XControlController(m, l)   (qtma_EXT(l, kXControlEventControllerFieldPos, kXControlEventControllerFieldWidth))
#define qtma_XControlValue(m, l)        (qtma_EXT(m, kXControlEventValueFieldPos, kXControlEventValueFieldWidth))
#define qtma_MarkerSubtype(x)           (qtma_EXT(x,kMarkerEventSubtypeFieldPos,kMarkerEventSubtypeFieldWidth))
#define qtma_MarkerValue(x)             (qtma_EXT(x, kMarkerEventValueFieldPos, kMarkerEventValueFieldWidth))

#define qtma_KnobValue(m,l)                ((qtma_EXT(m,kKnobEventValueHighFieldPos,kKnobEventValueHighFieldWidth) << 16)  \
                                      | (qtma_EXT(l,kKnobEventValueLowFieldPos,kKnobEventValueLowFieldWidth)))

#define qtma_KnobKnob(m,l)              (qtma_EXT(l,kKnobEventKnobFieldPos,kKnobEventKnobFieldWidth))
#define qtma_GeneralSubtype(m,l)        (qtma_EXT(l,kGeneralEventSubtypeFieldPos,kGeneralEventSubtypeFieldWidth))
#define qtma_GeneralLength(m,l)           (qtma_EXT(m,kGeneralEventLengthFieldPos,kGeneralEventLengthFieldWidth))
#define qtma_StuffRestEvent(x, duration) ( \
    x =    (kRestEventType << kEventTypeFieldPos)   \
        |  ((long)(duration) << kRestEventDurationFieldPos),    \
    x = EndianU32_NtoB(x) )
#define qtma_StuffNoteEvent(x, part, pitch, volume, duration) ( \
    x =     (kNoteEventType << kEventTypeFieldPos)  \
        |   ((long)(part) << kEventPartFieldPos)    \
        |   (((long)(pitch) - kNoteEventPitchOffset) << kNoteEventPitchFieldPos)    \
        |   (((long)(volume) - kNoteEventVolumeOffset) << kNoteEventVolumeFieldPos) \
        |   ((long)(duration) << kNoteEventDurationFieldPos), \
    x = EndianU32_NtoB(x) )
#define qtma_StuffControlEvent(x, part, control, value) ( \
    x =     (kControlEventType << kEventTypeFieldPos)           \
        |   ((long)(part) << kEventPartFieldPos)                \
        |   ((long)(control) << kControlEventControllerFieldPos)\
        |   ((long)((value) & qtma_MASK(kControlEventValueFieldWidth)) << kControlEventValueFieldPos), \
    x = EndianU32_NtoB(x) )
#define qtma_StuffMarkerEvent(x, markerType, markerValue) ( \
    x =     (kMarkerEventType << kEventTypeFieldPos)    \
        |   ((long)(markerType) << kMarkerEventSubtypeFieldPos) \
        |   ((long)(markerValue) << kMarkerEventValueFieldPos), \
    x = EndianU32_NtoB(x) )
#define qtma_StuffXNoteEvent(w1, w2, part, pitch, volume, duration) ( \
    w1 =    (kXNoteEventType << kXEventTypeFieldPos)            \
        |   ((long)(part) << kXEventPartFieldPos)               \
        |   ((long)(pitch) << kXNoteEventPitchFieldPos),        \
    w1 = EndianU32_NtoB(w1),                                    \
    w2 =     (kXEventLengthBits << kEventLengthFieldPos)        \
        |   ((long)(duration) << kXNoteEventDurationFieldPos)   \
        |   ((long)(volume) << kXNoteEventVolumeFieldPos),      \
    w2 = EndianU32_NtoB(w2) )
#define qtma_StuffXControlEvent(w1, w2, part, control, value) (     \
    w1 =    (kXControlEventType << kXEventTypeFieldPos)             \
        |   ((long)(part) << kXEventPartFieldPos)                   \
        |   ((long)((value) & qtma_MASK(kXControlEventValueFieldWidth)) << kXControlEventValueFieldPos), \
    w1 = EndianU32_NtoB(w1),                                        \
    w2 =    (kXEventLengthBits << kEventLengthFieldPos)             \
        |   ((long)(control) << kXControlEventControllerFieldPos),  \
    w2 = EndianU32_NtoB(w2) )
#define qtma_StuffKnobEvent(w1, w2, part, knob, value) ( \
    w1 =    (kKnobEventType << kXEventTypeFieldPos)                 \
        |   ((long)(part) << kXEventPartFieldPos)                   \
        |   ((long)(value >> 16) << kKnobEventValueLowFieldPos),    \
    w1 = EndianU32_NtoB(w1),                                        \
    w2 =    (kXEventLengthBits << kEventLengthFieldPos)             \
        |   ((long)(knob) << kKnobEventKnobFieldPos)                \
        |   ((long)(value & 0xFFFF) << kKnobEventValueLowFieldPos), \
    w2 = EndianU32_NtoB(w2) )
#define qtma_StuffGeneralEvent(w1,w2,part,subType,length) ( \
    w1 =    (kGeneralEventType << kXEventTypeFieldPos)              \
        |   ((long)(part) << kXEventPartFieldPos)                   \
        |   ((long)(length) << kGeneralEventLengthFieldPos),        \
    w1 = EndianU32_NtoB(w1),                                        \
    w2 = (kGeneralEventLengthBits << kEventLengthFieldPos)          \
        |   ((long)(subType) << kGeneralEventSubtypeFieldPos)       \
        |   ((long)(length) << kGeneralEventLengthFieldPos),        \
    w2 = EndianU32_NtoB(w2) )
#define qtma_NeedXGeneralEvent(length)   (((unsigned long)(length)) > (unsigned long)0xffff)

/* General Event Defined Types*/
enum {
    kGeneralEventNoteRequest    = 1,                            /* Encapsulates NoteRequest data structure */
    kGeneralEventPartKey        = 4,
    kGeneralEventTuneDifference = 5,                            /* Contains a standard sequence, with end marker, for the tune difference of a sequence piece (halts QuickTime 2.0 Music) */
    kGeneralEventAtomicInstrument = 6,                          /* Encapsulates AtomicInstrument record */
    kGeneralEventKnob           = 7,                            /* knobID/knobValue pairs; smallest event is 4 longs */
    kGeneralEventMIDIChannel    = 8,                            /* used in tune header, one longword identifies the midi channel it originally came from */
    kGeneralEventPartChange     = 9,                            /* used in tune sequence, one longword identifies the tune part which can now take over this part's note channel (similar to program change) (halts QuickTime 2.0 Music)*/
    kGeneralEventNoOp           = 10,                           /* guaranteed to do nothing and be ignored. (halts QuickTime 2.0 Music) */
    kGeneralEventUsedNotes      = 11,                           /* four longwords specifying which midi notes are actually used, 0..127 msb to lsb */
    kGeneralEventPartMix        = 12                            /* three longwords: Fixed volume, long balance, long flags */
};

/* Marker Event Defined Types       // marker is 60 ee vv vv in hex, where e = event type, and v = value*/
enum {
    kMarkerEventEnd             = 0,                            /* marker type 0 means: value 0 - stop, value != 0 - ignore*/
    kMarkerEventBeat            = 1,                            /* value 0 = single beat; anything else is 65536ths-of-a-beat (quarter note)*/
    kMarkerEventTempo           = 2                             /* value same as beat marker, but indicates that a tempo event should be computed (based on where the next beat or tempo marker is) and emitted upon export*/
};

enum {
    kCurrentlyNativeEndian      = 1,
    kCurrentlyNotNativeEndian   = 2
};

/* UPP call backs */
#if OPAQUE_UPP_TYPES
    EXTERN_API(MusicMIDISendUPP)
    NewMusicMIDISendUPP            (MusicMIDISendProcPtr    userRoutine);

    EXTERN_API(MusicMIDIReadHookUPP)
    NewMusicMIDIReadHookUPP        (MusicMIDIReadHookProcPtr userRoutine);

    EXTERN_API(MusicOfflineDataUPP)
    NewMusicOfflineDataUPP         (MusicOfflineDataProcPtr userRoutine);

    EXTERN_API(TuneCallBackUPP)
    NewTuneCallBackUPP             (TuneCallBackProcPtr     userRoutine);

    EXTERN_API(TunePlayCallBackUPP)
    NewTunePlayCallBackUPP         (TunePlayCallBackProcPtr userRoutine);

    EXTERN_API(void)
    DisposeMusicMIDISendUPP        (MusicMIDISendUPP        userUPP);

    EXTERN_API(void)
    DisposeMusicMIDIReadHookUPP    (MusicMIDIReadHookUPP    userUPP);

    EXTERN_API(void)
    DisposeMusicOfflineDataUPP     (MusicOfflineDataUPP     userUPP);

    EXTERN_API(void)
    DisposeTuneCallBackUPP         (TuneCallBackUPP         userUPP);

    EXTERN_API(void)
    DisposeTunePlayCallBackUPP     (TunePlayCallBackUPP     userUPP);

    EXTERN_API(ComponentResult)
    InvokeMusicMIDISendUPP         (ComponentInstance       self,
                                    long                    refCon,
                                    MusicMIDIPacket *       mmp,
                                    MusicMIDISendUPP        userUPP);

    EXTERN_API(ComponentResult)
    InvokeMusicMIDIReadHookUPP     (MusicMIDIPacket *       mp,
                                    long                    myRefCon,
                                    MusicMIDIReadHookUPP    userUPP);

    EXTERN_API(ComponentResult)
    InvokeMusicOfflineDataUPP      (Ptr                     SoundData,
                                    long                    numBytes,
                                    long                    myRefCon,
                                    MusicOfflineDataUPP     userUPP);

    EXTERN_API(void)
    InvokeTuneCallBackUPP          (const TuneStatus *      status,
                                    long                    refCon,
                                    TuneCallBackUPP         userUPP);

    EXTERN_API(void)
    InvokeTunePlayCallBackUPP      (unsigned long *         event,
                                    long                    seed,
                                    long                    refCon,
                                    TunePlayCallBackUPP     userUPP);

#else
    enum { uppMusicMIDISendProcInfo = 0x00000FF0 };                 /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppMusicMIDIReadHookProcInfo = 0x000003F0 };             /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppMusicOfflineDataProcInfo = 0x00000FF0 };              /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppTuneCallBackProcInfo = 0x000003C0 };                  /* pascal no_return_value Func(4_bytes, 4_bytes) */
    enum { uppTunePlayCallBackProcInfo = 0x00000FC0 };              /* pascal no_return_value Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewMusicMIDISendUPP(userRoutine)                        (MusicMIDISendUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMusicMIDISendProcInfo, GetCurrentArchitecture())
    #define NewMusicMIDIReadHookUPP(userRoutine)                    (MusicMIDIReadHookUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMusicMIDIReadHookProcInfo, GetCurrentArchitecture())
    #define NewMusicOfflineDataUPP(userRoutine)                     (MusicOfflineDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppMusicOfflineDataProcInfo, GetCurrentArchitecture())
    #define NewTuneCallBackUPP(userRoutine)                         (TuneCallBackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTuneCallBackProcInfo, GetCurrentArchitecture())
    #define NewTunePlayCallBackUPP(userRoutine)                     (TunePlayCallBackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppTunePlayCallBackProcInfo, GetCurrentArchitecture())
    #define DisposeMusicMIDISendUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeMusicMIDIReadHookUPP(userUPP)                    DisposeRoutineDescriptor(userUPP)
    #define DisposeMusicOfflineDataUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define DisposeTuneCallBackUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define DisposeTunePlayCallBackUPP(userUPP)                     DisposeRoutineDescriptor(userUPP)
    #define InvokeMusicMIDISendUPP(self, refCon, mmp, userUPP)      (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppMusicMIDISendProcInfo, (self), (refCon), (mmp))
    #define InvokeMusicMIDIReadHookUPP(mp, myRefCon, userUPP)       (ComponentResult)CALL_TWO_PARAMETER_UPP((userUPP), uppMusicMIDIReadHookProcInfo, (mp), (myRefCon))
    #define InvokeMusicOfflineDataUPP(SoundData, numBytes, myRefCon, userUPP)  (ComponentResult)CALL_THREE_PARAMETER_UPP((userUPP), uppMusicOfflineDataProcInfo, (SoundData), (numBytes), (myRefCon))
    #define InvokeTuneCallBackUPP(status, refCon, userUPP)          CALL_TWO_PARAMETER_UPP((userUPP), uppTuneCallBackProcInfo, (status), (refCon))
    #define InvokeTunePlayCallBackUPP(event, seed, refCon, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppTunePlayCallBackProcInfo, (event), (seed), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewMusicMIDISendProc(userRoutine)                       NewMusicMIDISendUPP(userRoutine)
#define NewMusicMIDIReadHookProc(userRoutine)                   NewMusicMIDIReadHookUPP(userRoutine)
#define NewMusicOfflineDataProc(userRoutine)                    NewMusicOfflineDataUPP(userRoutine)
#define NewTuneCallBackProc(userRoutine)                        NewTuneCallBackUPP(userRoutine)
#define NewTunePlayCallBackProc(userRoutine)                    NewTunePlayCallBackUPP(userRoutine)
#define CallMusicMIDISendProc(userRoutine, self, refCon, mmp)   InvokeMusicMIDISendUPP(self, refCon, mmp, userRoutine)
#define CallMusicMIDIReadHookProc(userRoutine, mp, myRefCon)    InvokeMusicMIDIReadHookUPP(mp, myRefCon, userRoutine)
#define CallMusicOfflineDataProc(userRoutine, SoundData, numBytes, myRefCon) InvokeMusicOfflineDataUPP(SoundData, numBytes, myRefCon, userRoutine)
#define CallTuneCallBackProc(userRoutine, status, refCon)       InvokeTuneCallBackUPP(status, refCon, userRoutine)
#define CallTunePlayCallBackProc(userRoutine, event, seed, refCon) InvokeTunePlayCallBackUPP(event, seed, refCon, userRoutine)

/* selectors for component calls */
enum {
    kQTMIDIGetMIDIPortsSelect                  = 0x0001,
    kQTMIDIUseSendPortSelect                   = 0x0002,
    kQTMIDISendMIDISelect                      = 0x0003,
    kQTMIDIUseReceivePortSelect                = 0x0004,
    kMusicGetDescriptionSelect                 = 0x0001,
    kMusicGetPartSelect                        = 0x0002,
    kMusicSetPartSelect                        = 0x0003,
    kMusicSetPartInstrumentNumberSelect        = 0x0004,
    kMusicGetPartInstrumentNumberSelect        = 0x0005,
    kMusicStorePartInstrumentSelect            = 0x0006,
    kMusicGetPartAtomicInstrumentSelect        = 0x0009,
    kMusicSetPartAtomicInstrumentSelect        = 0x000A,
    kMusicGetInstrumentKnobDescriptionObsoleteSelect = 0x000D,
    kMusicGetDrumKnobDescriptionObsoleteSelect = 0x000E,
    kMusicGetKnobDescriptionObsoleteSelect     = 0x000F,
    kMusicGetPartKnobSelect                    = 0x0010,
    kMusicSetPartKnobSelect                    = 0x0011,
    kMusicGetKnobSelect                        = 0x0012,
    kMusicSetKnobSelect                        = 0x0013,
    kMusicGetPartNameSelect                    = 0x0014,
    kMusicSetPartNameSelect                    = 0x0015,
    kMusicFindToneSelect                       = 0x0016,
    kMusicPlayNoteSelect                       = 0x0017,
    kMusicResetPartSelect                      = 0x0018,
    kMusicSetPartControllerSelect              = 0x0019,
    kMusicGetPartControllerSelect              = 0x001A,
    kMusicGetMIDIProcSelect                    = 0x001B,
    kMusicSetMIDIProcSelect                    = 0x001C,
    kMusicGetInstrumentNamesSelect             = 0x001D,
    kMusicGetDrumNamesSelect                   = 0x001E,
    kMusicGetMasterTuneSelect                  = 0x001F,
    kMusicSetMasterTuneSelect                  = 0x0020,
    kMusicGetInstrumentAboutInfoSelect         = 0x0022,
    kMusicGetDeviceConnectionSelect            = 0x0023,
    kMusicUseDeviceConnectionSelect            = 0x0024,
    kMusicGetKnobSettingStringsSelect          = 0x0025,
    kMusicGetMIDIPortsSelect                   = 0x0026,
    kMusicSendMIDISelect                       = 0x0027,
    kMusicReceiveMIDISelect                    = 0x0028,
    kMusicStartOfflineSelect                   = 0x0029,
    kMusicSetOfflineTimeToSelect               = 0x002A,
    kMusicGetInstrumentKnobDescriptionSelect   = 0x002B,
    kMusicGetDrumKnobDescriptionSelect         = 0x002C,
    kMusicGetKnobDescriptionSelect             = 0x002D,
    kMusicGetInfoTextSelect                    = 0x002E,
    kMusicGetInstrumentInfoSelect              = 0x002F,
    kMusicTaskSelect                           = 0x0031,
    kMusicSetPartInstrumentNumberInterruptSafeSelect = 0x0032,
    kMusicSetPartSoundLocalizationSelect       = 0x0033,
    kMusicGenericConfigureSelect               = 0x0100,
    kMusicGenericGetPartSelect                 = 0x0101,
    kMusicGenericGetKnobListSelect             = 0x0102,
    kMusicGenericSetResourceNumbersSelect      = 0x0103,
    kMusicDerivedMIDISendSelect                = 0x0200,
    kMusicDerivedSetKnobSelect                 = 0x0201,
    kMusicDerivedSetPartSelect                 = 0x0202,
    kMusicDerivedSetInstrumentSelect           = 0x0203,
    kMusicDerivedSetPartInstrumentNumberSelect = 0x0204,
    kMusicDerivedSetMIDISelect                 = 0x0205,
    kMusicDerivedStorePartInstrumentSelect     = 0x0206,
    kMusicDerivedOpenResFileSelect             = 0x0207,
    kMusicDerivedCloseResFileSelect            = 0x0208,
    kInstrumentGetInstSelect                   = 0x0001,
    kInstrumentGetInfoSelect                   = 0x0002,
    kInstrumentInitializeSelect                = 0x0003,
    kInstrumentOpenComponentResFileSelect      = 0x0004,
    kInstrumentCloseComponentResFileSelect     = 0x0005,
    kInstrumentGetComponentRefConSelect        = 0x0006,
    kInstrumentSetComponentRefConSelect        = 0x0007,
    kInstrumentGetSynthesizerTypeSelect        = 0x0008,
    kNARegisterMusicDeviceSelect               = 0x0000,
    kNAUnregisterMusicDeviceSelect             = 0x0001,
    kNAGetRegisteredMusicDeviceSelect          = 0x0002,
    kNASaveMusicConfigurationSelect            = 0x0003,
    kNANewNoteChannelSelect                    = 0x0004,
    kNADisposeNoteChannelSelect                = 0x0005,
    kNAGetNoteChannelInfoSelect                = 0x0006,
    kNAPrerollNoteChannelSelect                = 0x0007,
    kNAUnrollNoteChannelSelect                 = 0x0008,
    kNASetNoteChannelVolumeSelect              = 0x000B,
    kNAResetNoteChannelSelect                  = 0x000C,
    kNAPlayNoteSelect                          = 0x000D,
    kNASetControllerSelect                     = 0x000E,
    kNASetKnobSelect                           = 0x000F,
    kNAFindNoteChannelToneSelect               = 0x0010,
    kNASetInstrumentNumberSelect               = 0x0011,
    kNAPickInstrumentSelect                    = 0x0012,
    kNAPickArrangementSelect                   = 0x0013,
    kNASetDefaultMIDIInputSelect               = 0x0015,
    kNAGetDefaultMIDIInputSelect               = 0x0016,
    kNAUseDefaultMIDIInputSelect               = 0x0019,
    kNALoseDefaultMIDIInputSelect              = 0x001A,
    kNAStuffToneDescriptionSelect              = 0x001B,
    kNACopyrightDialogSelect                   = 0x001C,
    kNAGetIndNoteChannelSelect                 = 0x001F,
    kNAGetMIDIPortsSelect                      = 0x0021,
    kNAGetNoteRequestSelect                    = 0x0022,
    kNASendMIDISelect                          = 0x0023,
    kNAPickEditInstrumentSelect                = 0x0024,
    kNANewNoteChannelFromAtomicInstrumentSelect = 0x0025,
    kNASetAtomicInstrumentSelect               = 0x0026,
    kNAGetKnobSelect                           = 0x0028,
    kNATaskSelect                              = 0x0029,
    kNASetNoteChannelBalanceSelect             = 0x002A,
    kNASetInstrumentNumberInterruptSafeSelect  = 0x002B,
    kNASetNoteChannelSoundLocalizationSelect   = 0x002C,
    kNAGetControllerSelect                     = 0x002D,
    kTuneSetHeaderSelect                       = 0x0004,
    kTuneGetTimeBaseSelect                     = 0x0005,
    kTuneSetTimeScaleSelect                    = 0x0006,
    kTuneGetTimeScaleSelect                    = 0x0007,
    kTuneGetIndexedNoteChannelSelect           = 0x0008,
    kTuneQueueSelect                           = 0x000A,
    kTuneInstantSelect                         = 0x000B,
    kTuneGetStatusSelect                       = 0x000C,
    kTuneStopSelect                            = 0x000D,
    kTuneSetVolumeSelect                       = 0x0010,
    kTuneGetVolumeSelect                       = 0x0011,
    kTunePrerollSelect                         = 0x0012,
    kTuneUnrollSelect                          = 0x0013,
    kTuneSetNoteChannelsSelect                 = 0x0014,
    kTuneSetPartTransposeSelect                = 0x0015,
    kTuneGetNoteAllocatorSelect                = 0x0017,
    kTuneSetSofterSelect                       = 0x0018,
    kTuneTaskSelect                            = 0x0019,
    kTuneSetBalanceSelect                      = 0x001A,
    kTuneSetSoundLocalizationSelect            = 0x001B,
    kTuneSetHeaderWithSizeSelect               = 0x001C,
    kTuneSetPartMixSelect                      = 0x001D,
    kTuneGetPartMixSelect                      = 0x001E
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

#endif /* __QUICKTIMEMUSIC__ */

