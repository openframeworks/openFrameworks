/*
     File:       MoviesFormat.h
 
     Contains:   QuickTime Interfaces.
 
     Version:    Technology: QuickTime 6.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MOVIESFORMAT__
#define __MOVIESFORMAT__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MOVIES__
#include <Movies.h>
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
    kMovieVersion               = 0                             /* version number of the format here described */
};

/****************************************
*
*   General Types -
*       These types are used in more than one of the
*       directory types.
*
****************************************/
/* MoviesUserData is the type used for user data in movie and track directories */

struct MoviesUserData {
    long                            size;                       /* size of this user data */
    long                            udType;                     /* type of user data */
    char                            data[1];                    /* the user data */
};
typedef struct MoviesUserData           MoviesUserData;

struct UserDataAtom {
    long                            size;
    long                            atomType;
    MoviesUserData                  userData[1];
};
typedef struct UserDataAtom             UserDataAtom;
/* MoviesDataDescription tells us where the data for the movie or track lives.
   The data can follow the directory, be in the datafork of the same file as the directory resource,
   be in the resource fork of the same file as the directory resource, be in another file in the
   data fork or resource fork, or require a specific bottleneck to fetch the data. */
/****************************************
*
*   MediaDirectory information -
*       The MediaDirectory is tightly coupled to the data.
*
****************************************/


struct SampleDescriptionAtom {
    long                            size;
    long                            atomType;                   /* = 'stsd' */
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */
    long                            numEntries;
    SampleDescription               sampleDescTable[1];
};
typedef struct SampleDescriptionAtom    SampleDescriptionAtom;
/* TimeToSampleNum maps physical sample time to physical sample number. */

struct TimeToSampleNum {
    long                            sampleCount;
    TimeValue                       sampleDuration;
};
typedef struct TimeToSampleNum          TimeToSampleNum;

struct TimeToSampleNumAtom {
    long                            size;
    long                            atomType;                   /* = 'stts' */
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */
    long                            numEntries;
    TimeToSampleNum                 timeToSampleNumTable[1];
};
typedef struct TimeToSampleNumAtom      TimeToSampleNumAtom;
/* SyncSamples is a list of the physical samples which are self contained. */

struct SyncSampleAtom {
    long                            size;
    long                            atomType;                   /* = 'stss' */
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */
    long                            numEntries;
    long                            syncSampleTable[1];
};
typedef struct SyncSampleAtom           SyncSampleAtom;
/* SampleToChunk maps physical sample number to chunk number. */
/* same as SampleToChunk, but redundant first sample is removed */

struct SampleToChunk {
    long                            firstChunk;
    long                            samplesPerChunk;
    long                            sampleDescriptionID;
};
typedef struct SampleToChunk            SampleToChunk;

struct SampleToChunkAtom {
    long                            size;
    long                            atomType;                   /* = 'stsc' */
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */
    long                            numEntries;
    SampleToChunk                   sampleToChunkTable[1];
};
typedef struct SampleToChunkAtom        SampleToChunkAtom;

struct ChunkOffsetAtom {
    long                            size;
    long                            atomType;                   /* = 'stco' */
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */
    long                            numEntries;
    long                            chunkOffsetTable[1];
};
typedef struct ChunkOffsetAtom          ChunkOffsetAtom;

struct SampleSizeAtom {
    long                            size;
    long                            atomType;                   /* = 'stsz' */
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */
    long                            sampleSize;
    long                            numEntries;
    long                            sampleSizeTable[1];
};
typedef struct SampleSizeAtom           SampleSizeAtom;

struct ShadowSync {
    long                            fdSampleNum;
    long                            syncSampleNum;
};
typedef struct ShadowSync               ShadowSync;

struct ShadowSyncAtom {
    long                            size;
    long                            atomType;                   /* = 'stsz' */
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */
    long                            numEntries;
    ShadowSync                      shadowSyncTable[1];
};
typedef struct ShadowSyncAtom           ShadowSyncAtom;

struct SampleTableAtom {
    long                            size;
    long                            atomType;                   /* = 'stbl' */

    SampleDescriptionAtom           sampleDescription;
    TimeToSampleNumAtom             timeToSampleNum;
    SampleToChunkAtom               sampleToChunk;
    SyncSampleAtom                  syncSample;
    SampleSizeAtom                  sampleSize;
    ChunkOffsetAtom                 chunkOffset;
    ShadowSyncAtom                  shadowSync;
};
typedef struct SampleTableAtom          SampleTableAtom;

struct PublicHandlerInfo {
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */

    long                            componentType;
    long                            componentSubType;
    long                            componentManufacturer;
    long                            componentFlags;
    long                            componentFlagsMask;
    char                            componentName[1];
};
typedef struct PublicHandlerInfo        PublicHandlerInfo;

struct HandlerAtom {
    long                            size;
    long                            atomType;                   /* = 'hdlr' */

    PublicHandlerInfo               hInfo;
};
typedef struct HandlerAtom              HandlerAtom;
/* a data reference is a private structure */

typedef long                            DataRefAtom;

struct DataInfoAtom {
    long                            size;
    long                            atomType;                   /* = 'dinf' */

    DataRefAtom                     dataRef;
};
typedef struct DataInfoAtom             DataInfoAtom;

struct RgnAtom {
    long                            size;
    long                            atomType;

    short                           rgnSize;
    Rect                            rgnBBox;
    char                            data[1];
};
typedef struct RgnAtom                  RgnAtom;

struct MatteCompressedAtom {
    long                            size;
    long                            atomType;

    long                            flags;                      /* 1 byte of version / 3 bytes of flags */

    ImageDescription                matteImageDescription;

    char                            matteData[1];
};
typedef struct MatteCompressedAtom      MatteCompressedAtom;

struct MatteAtom {
    long                            size;
    long                            atomType;

    MatteCompressedAtom             aCompressedMatte;
};
typedef struct MatteAtom                MatteAtom;

struct ClippingAtom {
    long                            size;
    long                            atomType;

    RgnAtom                         aRgnClip;
};
typedef struct ClippingAtom             ClippingAtom;
/***********************
* Media Info Example Structures
***********************/


struct VideoMediaInfoHeader {
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */

    short                           graphicsMode;               /* for QD - transfer mode */
    short                           opColorRed;                 /* opcolor for transfer mode */
    short                           opColorGreen;
    short                           opColorBlue;
};
typedef struct VideoMediaInfoHeader     VideoMediaInfoHeader;

struct VideoMediaInfoHeaderAtom {
    long                            size;                       /* size of Media info */
    long                            atomType;                   /* = 'vmhd' */
    VideoMediaInfoHeader            vmiHeader;
};
typedef struct VideoMediaInfoHeaderAtom VideoMediaInfoHeaderAtom;

struct VideoMediaInfo {
    long                            size;                       /* size of Media info */
    long                            atomType;                   /* = 'minf' */

    VideoMediaInfoHeaderAtom        header;

    HandlerAtom                     dataHandler;

    DataInfoAtom                    dataInfo;

    SampleTableAtom                 sampleTable;
};
typedef struct VideoMediaInfo           VideoMediaInfo;

struct SoundMediaInfoHeader {
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */

    short                           balance;
    short                           rsrvd;
};
typedef struct SoundMediaInfoHeader     SoundMediaInfoHeader;

struct SoundMediaInfoHeaderAtom {
    long                            size;                       /* size of Media info */
    long                            atomType;                   /* = 'vmhd' */

    SoundMediaInfoHeader            smiHeader;
};
typedef struct SoundMediaInfoHeaderAtom SoundMediaInfoHeaderAtom;

struct SoundMediaInfo {
    long                            size;                       /* size of Media info */
    long                            atomType;                   /* = 'minf' */

    SoundMediaInfoHeaderAtom        header;

    HandlerAtom                     dataHandler;

    DataRefAtom                     dataReference;

    SampleTableAtom                 sampleTable;
};
typedef struct SoundMediaInfo           SoundMediaInfo;
/* whatever data the media handler needs goes after the atomType */

struct MediaInfo {
    long                            size;
    long                            atomType;
};
typedef struct MediaInfo                MediaInfo;
/***********************
* Media Directory Structures
***********************/

struct MediaHeader {
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */

    long                            creationTime;               /* seconds since Jan 1904 when directory was created */
    long                            modificationTime;           /* seconds since Jan 1904 when directory was appended */

    TimeValue                       timeScale;                  /* start time for Media (Media time) */
    TimeValue                       duration;                   /* length of Media (Media time) */

    short                           language;
    short                           quality;
};
typedef struct MediaHeader              MediaHeader;

struct MediaHeaderAtom {
    long                            size;
    long                            atomType;

    MediaHeader                     header;
};
typedef struct MediaHeaderAtom          MediaHeaderAtom;

struct MediaDirectory {
    long                            size;
    long                            atomType;                   /* = 'mdia' */

    MediaHeaderAtom                 mediaHeader;                /* standard Media information */

    HandlerAtom                     mediaHandler;

    MediaInfo                       mediaInfo;
};
typedef struct MediaDirectory           MediaDirectory;
/***********************
* Track Structures
***********************/
enum {
    TrackEnable                 = 1 << 0,
    TrackInMovie                = 1 << 1,
    TrackInPreview              = 1 << 2,
    TrackInPoster               = 1 << 3
};


struct TrackHeader {
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */

    long                            creationTime;               /* seconds since Jan 1904 when directory was created */
    long                            modificationTime;           /* seconds since Jan 1904 when directory was appended */

    long                            trackID;

    long                            reserved1;

    TimeValue                       duration;                   /* length of track (track time) */

    long                            reserved2;
    long                            reserved3;

    short                           layer;
    short                           alternateGroup;

    short                           volume;
    short                           reserved4;

    MatrixRecord                    matrix;
    Fixed                           trackWidth;
    Fixed                           trackHeight;
};
typedef struct TrackHeader              TrackHeader;

struct TrackHeaderAtom {
    long                            size;                       /* size of track header */
    long                            atomType;                   /* = 'tkhd' */

    TrackHeader                     header;
};
typedef struct TrackHeaderAtom          TrackHeaderAtom;

struct EditListType {
    TimeValue                       trackDuration;
    TimeValue                       mediaTime;
    Fixed                           mediaRate;
};
typedef struct EditListType             EditListType;

struct EditListAtom {
    long                            size;
    long                            atomType;                   /* = elst */

    long                            flags;                      /* 1 byte of version / 3 bytes of flags */

    long                            numEntries;
    EditListType                    editListTable[1];
};
typedef struct EditListAtom             EditListAtom;

struct EditsAtom {
    long                            size;
    long                            atomType;                   /* = edts */

    EditListAtom                    editList;
};
typedef struct EditsAtom                EditsAtom;

struct TrackLoadSettings {
    TimeValue                       preloadStartTime;
    TimeValue                       preloadDuration;
    long                            preloadFlags;
    long                            defaultHints;
};
typedef struct TrackLoadSettings        TrackLoadSettings;

struct TrackLoadSettingsAtom {
    long                            size;
    long                            atomType;                   /* = load */

    TrackLoadSettings               settings;
};
typedef struct TrackLoadSettingsAtom    TrackLoadSettingsAtom;

struct TrackDirectory {
    long                            size;
    long                            atomType;                   /* = 'trak' */

    TrackHeaderAtom                 trackHeader;                /* standard track information */

    ClippingAtom                    trackClip;

    EditsAtom                       edits;

    MediaDirectory                  media;

    UserDataAtom                    userData;                   /* space for extending with new data types */
};
typedef struct TrackDirectory           TrackDirectory;
/****************************************
*
*   MovieDirectory -
*       The MovieDirectory is the top level structure which
*       holds the TrackInstance describing where the
*       TrackDirectories are.
*
****************************************/

struct MovieHeader {
    long                            flags;                      /* 1 byte of version / 3 bytes of flags */

    long                            creationTime;               /* seconds since Jan 1904 when directory was created */
    long                            modificationTime;           /* seconds since Jan 1904 when directory was appended */

    TimeValue                       timeScale;                  /* Time specifications */
    TimeValue                       duration;
    Fixed                           preferredRate;              /* rate at which to play this movie */

    short                           preferredVolume;            /* volume to play movie at */
    short                           reserved1;

    long                            preferredLong1;
    long                            preferredLong2;

    MatrixRecord                    matrix;

    TimeValue                       previewTime;                /* time in track the proxy begins (track time) */
    TimeValue                       previewDuration;            /* how long the proxy lasts (track time) */

    TimeValue                       posterTime;                 /* time in track the proxy begins (track time) */

    TimeValue                       selectionTime;              /* time in track the proxy begins (track time) */
    TimeValue                       selectionDuration;          /* time in track the proxy begins (track time) */
    TimeValue                       currentTime;                /* time in track the proxy begins (track time) */

    long                            nextTrackID;                /* next value to use for a TrackID */
};
typedef struct MovieHeader              MovieHeader;

struct MovieHeaderAtom {
    long                            size;
    long                            atomType;                   /* = 'mvhd' */

    MovieHeader                     header;
};
typedef struct MovieHeaderAtom          MovieHeaderAtom;

struct TrackDirectoryEntry {
    TrackDirectory                  trackDirectory;             /* Track directory information */
};
typedef struct TrackDirectoryEntry      TrackDirectoryEntry;

struct MovieDirectory {
    long                            size;
    long                            atomType;                   /* = 'moov' */

    MovieHeaderAtom                 header;

    ClippingAtom                    movieClip;

                                                                /* Track Directories */
    TrackDirectoryEntry             track[1];                   /* Track directory information */

                                                                /* User data for Movie */
    UserDataAtom                    userData;                   /* space for user extensions */
};
typedef struct MovieDirectory           MovieDirectory;
/****************************************
****************************************/

/* Movie formats and tags */
enum {
                                                                /* some system defined format IDs */
    MOVIE_TYPE                  = FOUR_CHAR_CODE('moov'),
    TRACK_TYPE                  = FOUR_CHAR_CODE('trak'),
    MEDIA_TYPE                  = FOUR_CHAR_CODE('mdia'),
    VIDEO_TYPE                  = FOUR_CHAR_CODE('vide'),
    SOUND_TYPE                  = FOUR_CHAR_CODE('soun')
};

/* atom id's */
enum {
    MovieAID                    = FOUR_CHAR_CODE('moov'),
    MovieHeaderAID              = FOUR_CHAR_CODE('mvhd'),
    ClipAID                     = FOUR_CHAR_CODE('clip'),
    RgnClipAID                  = FOUR_CHAR_CODE('crgn'),
    MatteAID                    = FOUR_CHAR_CODE('matt'),
    MatteCompAID                = FOUR_CHAR_CODE('kmat'),
    TrackAID                    = FOUR_CHAR_CODE('trak'),
    UserDataAID                 = FOUR_CHAR_CODE('udta'),
    TrackHeaderAID              = FOUR_CHAR_CODE('tkhd'),
    EditsAID                    = FOUR_CHAR_CODE('edts'),
    EditListAID                 = FOUR_CHAR_CODE('elst'),
    MediaAID                    = FOUR_CHAR_CODE('mdia'),
    MediaHeaderAID              = FOUR_CHAR_CODE('mdhd'),
    MediaInfoAID                = FOUR_CHAR_CODE('minf'),
    VideoMediaInfoHeaderAID     = FOUR_CHAR_CODE('vmhd'),
    SoundMediaInfoHeaderAID     = FOUR_CHAR_CODE('smhd'),
    GenericMediaInfoHeaderAID   = FOUR_CHAR_CODE('gmhd'),
    GenericMediaInfoAID         = FOUR_CHAR_CODE('gmin'),
    DataInfoAID                 = FOUR_CHAR_CODE('dinf'),
    DataRefAID                  = FOUR_CHAR_CODE('dref'),
    SampleTableAID              = FOUR_CHAR_CODE('stbl'),
    STSampleDescAID             = FOUR_CHAR_CODE('stsd'),
    STTimeToSampAID             = FOUR_CHAR_CODE('stts'),
    STSyncSampleAID             = FOUR_CHAR_CODE('stss'),
    STSampleToChunkAID          = FOUR_CHAR_CODE('stsc'),
    STShadowSyncAID             = FOUR_CHAR_CODE('stsh'),
    HandlerAID                  = FOUR_CHAR_CODE('hdlr'),
    STSampleSizeAID             = FOUR_CHAR_CODE('stsz'),
    STChunkOffsetAID            = FOUR_CHAR_CODE('stco'),
    STChunkOffset64AID          = FOUR_CHAR_CODE('co64'),
    STSampleIDAID               = FOUR_CHAR_CODE('stid'),
    DataRefContainerAID         = FOUR_CHAR_CODE('drfc'),
    TrackReferenceAID           = FOUR_CHAR_CODE('tref'),
    ColorTableAID               = FOUR_CHAR_CODE('ctab'),
    LoadSettingsAID             = FOUR_CHAR_CODE('load'),
    PropertyAtomAID             = FOUR_CHAR_CODE('code'),
    InputMapAID                 = FOUR_CHAR_CODE('imap'),
    MovieBufferHintsAID         = FOUR_CHAR_CODE('mbfh'),
    MovieDataRefAliasAID        = FOUR_CHAR_CODE('mdra'),
    SoundLocalizationAID        = FOUR_CHAR_CODE('sloc'),
    CompressedMovieAID          = FOUR_CHAR_CODE('cmov'),
    CompressedMovieDataAID      = FOUR_CHAR_CODE('cmvd'),
    DataCompressionAtomAID      = FOUR_CHAR_CODE('dcom'),
    ReferenceMovieRecordAID     = FOUR_CHAR_CODE('rmra'),
    ReferenceMovieDescriptorAID = FOUR_CHAR_CODE('rmda'),
    ReferenceMovieDataRefAID    = FOUR_CHAR_CODE('rdrf'),
    ReferenceMovieVersionCheckAID = FOUR_CHAR_CODE('rmvc'),
    ReferenceMovieDataRateAID   = FOUR_CHAR_CODE('rmdr'),
    ReferenceMovieComponentCheckAID = FOUR_CHAR_CODE('rmcd'),
    ReferenceMovieQualityAID    = FOUR_CHAR_CODE('rmqu'),
    ReferenceMovieLanguageAID   = FOUR_CHAR_CODE('rmla'),
    ReferenceMovieCPURatingAID  = FOUR_CHAR_CODE('rmcs'),
    ReferenceMovieAlternateGroupAID = FOUR_CHAR_CODE('rmag'),
    ReferenceMovieNetworkStatusAID = FOUR_CHAR_CODE('rnet'),
    CloneMediaAID               = FOUR_CHAR_CODE('clon'),
    FileTypeAID                 = FOUR_CHAR_CODE('ftyp'),
    SecureContentInfoAID        = FOUR_CHAR_CODE('sinf'),
    SecureContentSchemeTypeAID  = FOUR_CHAR_CODE('schm'),
    SecureContentSchemeInfoAID  = FOUR_CHAR_CODE('schi')
};

/* Text ATOM definitions*/


struct TextBoxAtom {
    long                            size;
    long                            atomType;                   /* = 'tbox' */
    Rect                            textBox;                    /* New text box (overrides defaultTextBox)*/
};
typedef struct TextBoxAtom              TextBoxAtom;

struct HiliteAtom {
    long                            size;
    long                            atomType;                   /* = 'hlit' */
    long                            selStart;                   /* hilite selection start character*/
    long                            selEnd;                     /* hilite selection end character*/
};
typedef struct HiliteAtom               HiliteAtom;

struct KaraokeRec {
    TimeValue                       timeVal;
    short                           beginHilite;
    short                           endHilite;
};
typedef struct KaraokeRec               KaraokeRec;

struct KaraokeAtom {
    long                            numEntries;
    KaraokeRec                      karaokeEntries[1];
};
typedef struct KaraokeAtom              KaraokeAtom;
/* for ReferenceMovieDataRefRecord.flags*/
enum {
    kDataRefIsSelfContained     = (1 << 0)
};


struct ReferenceMovieDataRefRecord {
    long                            flags;
    OSType                          dataRefType;
    long                            dataRefSize;
    char                            dataRef[1];
};
typedef struct ReferenceMovieDataRefRecord ReferenceMovieDataRefRecord;
/* for VersionCheckRecord.checkType*/
enum {
    kVersionCheckMin            = 0,                            /* val1 is the min. version required*/
    kVersionCheckMask           = 1                             /* (gestalt return value & val2) must == val1*/
};


struct QTAltVersionCheckRecord {
    long                            flags;                      /* currently always 0*/
    OSType                          gestaltTag;
    UInt32                          val1;
    UInt32                          val2;
    short                           checkType;
};
typedef struct QTAltVersionCheckRecord  QTAltVersionCheckRecord;
/* some helpful constants for DataRateRecord.dataRate */
enum {
    kDataRate144ModemRate       = 1400L,
    kDataRate288ModemRate       = 2800L,
    kDataRateISDNRate           = 5600L,
    kDataRateDualISDNRate       = 11200L,
    kDataRate256kbpsRate        = 25600L,
    kDataRate384kbpsRate        = 38400L,
    kDataRate512kbpsRate        = 51200L,
    kDataRate768kbpsRate        = 76800L,
    kDataRate1MbpsRate          = 100000L,
    kDataRateT1Rate             = 150000L,
    kDataRateInfiniteRate       = 0x7FFFFFFF,
    kDataRateDefaultIfNotSet    = kDataRateISDNRate
};


struct QTAltDataRateRecord {
    long                            flags;                      /* currently always 0*/
    long                            dataRate;
};
typedef struct QTAltDataRateRecord      QTAltDataRateRecord;

struct QTAltComponentCheckRecord {
    long                            flags;                      /* currently always 0 */
    ComponentDescription            cd;
    unsigned long                   minVersion;
};
typedef struct QTAltComponentCheckRecord QTAltComponentCheckRecord;

struct QTAltLanguageRecord {
    long                            flags;                      /* currently always 0*/
    short                           language;
};
typedef struct QTAltLanguageRecord      QTAltLanguageRecord;

enum {
    kQTCPUSpeed1Rating          = 100,                          /* slowest*/
    kQTCPUSpeed2Rating          = 200,
    kQTCPUSpeed3Rating          = 300,
    kQTCPUSpeed4Rating          = 400,
    kQTCPUSpeed5Rating          = 500                           /* fastest*/
};


struct QTAltCPURatingRecord {
    UInt32                          flags;                      /* currently always 0*/
    UInt16                          speed;
};
typedef struct QTAltCPURatingRecord     QTAltCPURatingRecord;

struct ReferenceMovieNetworkStatusRecord {
    UInt32                          flags;                      /* currently always 0*/
    UInt32                          valueCount;                 /* how many status values are in array*/
    long                            netStatusValues[1];         /* a value from kQTNetworkStatus... constants*/
};
typedef struct ReferenceMovieNetworkStatusRecord ReferenceMovieNetworkStatusRecord;

struct CloneRecord {
    long                            flags;
    long                            masterTrackID;              /* track ID of the track we're cloning */
};
typedef struct CloneRecord              CloneRecord;

struct CloneAtom {
    long                            size;
    long                            atomType;                   /* = clon */

    CloneRecord                     cloneInfo;
};
typedef struct CloneAtom                CloneAtom;

struct FileTypeAtom {
    long                            size;
    long                            atomType;                   /* = 'ftyp' */
    long                            majorBrand;                 /* best use brand */
    long                            minorVersion;
    long                            compatibleBrands[4];        /* 1 or greater */
};
typedef struct FileTypeAtom             FileTypeAtom;
enum {
    kQTFileTypeBrandQuickTimeMovie = FOUR_CHAR_CODE('qt  '),    /* QuickTime movie files*/
    kQTFileTypeBrandISOFile     = FOUR_CHAR_CODE('isom'),       /* ISO Base Media files*/
    kQTFileTypeBrandMPEG4v1     = FOUR_CHAR_CODE('mp41'),       /* MPEG-4 (ISO/IEC 14496-1) version 1 files*/
    kQTFileTypeBrandMPEG4v2     = FOUR_CHAR_CODE('mp42')        /* MPEG-4 (ISO/IEC 14496-1) version 2 files*/
};


struct SecureContentInfoAtom {
    long                            size;
    long                            atomType;                   /* = 'sinf' */
};
typedef struct SecureContentInfoAtom    SecureContentInfoAtom;

struct SecureContentSchemeTypeAtom {
    long                            size;
    long                            atomType;                   /* = 'schm' */

    long                            flags;                      /* 1 byte of version / 3 bytes of flags */

    long                            schemeType;
    UInt16                          schemeVersion;
                                                                /* if flags & 1, C string holding URL for security component server*/
};
typedef struct SecureContentSchemeTypeAtom SecureContentSchemeTypeAtom;

struct SecureContentSchemeInfoAtom {
    long                            size;
    long                            atomType;                   /* = 'schi' */
};
typedef struct SecureContentSchemeInfoAtom SecureContentSchemeInfoAtom;

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

#endif /* __MOVIESFORMAT__ */

