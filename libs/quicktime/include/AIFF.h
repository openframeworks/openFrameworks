/*
     File:       AIFF.h
 
     Contains:   Definition of AIFF file format components.
 
     Version:    Technology: System 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1989-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __AIFF__
#define __AIFF__

#ifndef __MACTYPES__
#include <MacTypes.h>
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
    AIFFID                      = FOUR_CHAR_CODE('AIFF'),
    AIFCID                      = FOUR_CHAR_CODE('AIFC'),
    FormatVersionID             = FOUR_CHAR_CODE('FVER'),
    CommonID                    = FOUR_CHAR_CODE('COMM'),
    FORMID                      = FOUR_CHAR_CODE('FORM'),
    SoundDataID                 = FOUR_CHAR_CODE('SSND'),
    MarkerID                    = FOUR_CHAR_CODE('MARK'),
    InstrumentID                = FOUR_CHAR_CODE('INST'),
    MIDIDataID                  = FOUR_CHAR_CODE('MIDI'),
    AudioRecordingID            = FOUR_CHAR_CODE('AESD'),
    ApplicationSpecificID       = FOUR_CHAR_CODE('APPL'),
    CommentID                   = FOUR_CHAR_CODE('COMT'),
    NameID                      = FOUR_CHAR_CODE('NAME'),
    AuthorID                    = FOUR_CHAR_CODE('AUTH'),
    CopyrightID                 = FOUR_CHAR_CODE('(c) '),
    AnnotationID                = FOUR_CHAR_CODE('ANNO')
};

enum {
    NoLooping                   = 0,
    ForwardLooping              = 1,
    ForwardBackwardLooping      = 2
};

enum {
                                                                /* AIFF-C Versions */
    AIFCVersion1                = (long)0xA2805140
};



/* Compression Names */
#define NoneName "\pnot compressed"
#define ACE2to1Name "\pACE 2-to-1"
#define ACE8to3Name "\pACE 8-to-3"
#define MACE3to1Name "\pMACE 3-to-1"
#define MACE6to1Name "\pMACE 6-to-1"

enum {
                                                                /* Compression Types */
    NoneType                    = FOUR_CHAR_CODE('NONE'),
    ACE2Type                    = FOUR_CHAR_CODE('ACE2'),
    ACE8Type                    = FOUR_CHAR_CODE('ACE8'),
    MACE3Type                   = FOUR_CHAR_CODE('MAC3'),
    MACE6Type                   = FOUR_CHAR_CODE('MAC6')
};

typedef unsigned long                   ID;
typedef short                           MarkerIdType;

struct ChunkHeader {
    ID                              ckID;
    long                            ckSize;
};
typedef struct ChunkHeader              ChunkHeader;

struct ContainerChunk {
    ID                              ckID;
    long                            ckSize;
    ID                              formType;
};
typedef struct ContainerChunk           ContainerChunk;

struct FormatVersionChunk {
    ID                              ckID;
    long                            ckSize;
    unsigned long                   timestamp;
};
typedef struct FormatVersionChunk       FormatVersionChunk;
typedef FormatVersionChunk *            FormatVersionChunkPtr;

struct CommonChunk {
    ID                              ckID;
    long                            ckSize;
    short                           numChannels;
    unsigned long                   numSampleFrames;
    short                           sampleSize;
    extended80                      sampleRate;
};
typedef struct CommonChunk              CommonChunk;
typedef CommonChunk *                   CommonChunkPtr;

struct ExtCommonChunk {
    ID                              ckID;
    long                            ckSize;
    short                           numChannels;
    unsigned long                   numSampleFrames;
    short                           sampleSize;
    extended80                      sampleRate;
    ID                              compressionType;
    char                            compressionName[1];         /* variable length array, Pascal string */
};
typedef struct ExtCommonChunk           ExtCommonChunk;
typedef ExtCommonChunk *                ExtCommonChunkPtr;

struct SoundDataChunk {
    ID                              ckID;
    long                            ckSize;
    unsigned long                   offset;
    unsigned long                   blockSize;
};
typedef struct SoundDataChunk           SoundDataChunk;
typedef SoundDataChunk *                SoundDataChunkPtr;

struct Marker {
    MarkerIdType                    id;
    unsigned long                   position;
    Str255                          markerName;
};
typedef struct Marker                   Marker;

struct MarkerChunk {
    ID                              ckID;
    long                            ckSize;
    unsigned short                  numMarkers;
    Marker                          Markers[1];                 /* variable length array */
};
typedef struct MarkerChunk              MarkerChunk;
typedef MarkerChunk *                   MarkerChunkPtr;

struct AIFFLoop {
    short                           playMode;
    MarkerIdType                    beginLoop;
    MarkerIdType                    endLoop;
};
typedef struct AIFFLoop                 AIFFLoop;

struct InstrumentChunk {
    ID                              ckID;
    long                            ckSize;
    UInt8                           baseFrequency;
    UInt8                           detune;
    UInt8                           lowFrequency;
    UInt8                           highFrequency;
    UInt8                           lowVelocity;
    UInt8                           highVelocity;
    short                           gain;
    AIFFLoop                        sustainLoop;
    AIFFLoop                        releaseLoop;
};
typedef struct InstrumentChunk          InstrumentChunk;
typedef InstrumentChunk *               InstrumentChunkPtr;

struct MIDIDataChunk {
    ID                              ckID;
    long                            ckSize;
    UInt8                           MIDIdata[1];                /* variable length array */
};
typedef struct MIDIDataChunk            MIDIDataChunk;
typedef MIDIDataChunk *                 MIDIDataChunkPtr;

struct AudioRecordingChunk {
    ID                              ckID;
    long                            ckSize;
    UInt8                           AESChannelStatus[24];
};
typedef struct AudioRecordingChunk      AudioRecordingChunk;
typedef AudioRecordingChunk *           AudioRecordingChunkPtr;

struct ApplicationSpecificChunk {
    ID                              ckID;
    long                            ckSize;
    OSType                          applicationSignature;
    UInt8                           data[1];                    /* variable length array */
};
typedef struct ApplicationSpecificChunk ApplicationSpecificChunk;
typedef ApplicationSpecificChunk *      ApplicationSpecificChunkPtr;

struct Comment {
    unsigned long                   timeStamp;
    MarkerIdType                    marker;
    unsigned short                  count;
    char                            text[1];                    /* variable length array, Pascal string */
};
typedef struct Comment                  Comment;

struct CommentsChunk {
    ID                              ckID;
    long                            ckSize;
    unsigned short                  numComments;
    Comment                         comments[1];                /* variable length array */
};
typedef struct CommentsChunk            CommentsChunk;
typedef CommentsChunk *                 CommentsChunkPtr;

struct TextChunk {
    ID                              ckID;
    long                            ckSize;
    char                            text[1];                    /* variable length array, Pascal string */
};
typedef struct TextChunk                TextChunk;
typedef TextChunk *                     TextChunkPtr;

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

#endif /* __AIFF__ */

