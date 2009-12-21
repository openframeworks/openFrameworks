/*
     File:       TranslationExtensions.h
 
     Contains:   Macintosh Easy Open Translation Extension Interfaces.
 
     Version:    Technology: Macintosh Easy Open 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1989-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TRANSLATIONEXTENSIONS__
#define __TRANSLATIONEXTENSIONS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __COMPONENTS__
#include <Components.h>
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
    kSupportsFileTranslation    = 1,
    kSupportsScrapTranslation   = 2,
    kTranslatorCanGenerateFilename = 4
};


/* better names for 4-char codes*/
typedef OSType                          FileType;
typedef ResType                         ScrapType;
typedef unsigned long                   TranslationAttributes;
enum {
    taDstDocNeedsResourceFork   = 1,
    taDstIsAppTranslation       = 2
};



struct FileTypeSpec {
    FileType                        format;
    long                            hint;
    TranslationAttributes           flags;                      /* taDstDocNeedsResourceFork, taDstIsAppTranslation*/
    OSType                          catInfoType;
    OSType                          catInfoCreator;
};
typedef struct FileTypeSpec             FileTypeSpec;

struct FileTranslationList {
    unsigned long                   modDate;
    unsigned long                   groupCount;
                                                                /*    unsigned long group1SrcCount;*/
                                                                /*    unsigned long group1SrcEntrySize = sizeof(FileTypeSpec);*/
                                                                /*  FileTypeSpec  group1SrcTypes[group1SrcCount]*/
                                                                /*  unsigned long group1DstCount;*/
                                                                /*  unsigned long group1DstEntrySize = sizeof(FileTypeSpec);*/
                                                                /*  FileTypeSpec  group1DstTypes[group1DstCount]*/
};
typedef struct FileTranslationList      FileTranslationList;
typedef FileTranslationList *           FileTranslationListPtr;
typedef FileTranslationListPtr *        FileTranslationListHandle;


struct ScrapTypeSpec {
    ScrapType                       format;
    long                            hint;
};
typedef struct ScrapTypeSpec            ScrapTypeSpec;

struct ScrapTranslationList {
    unsigned long                   modDate;
    unsigned long                   groupCount;
                                                                /*    unsigned long     group1SrcCount;*/
                                                                /*    unsigned long     group1SrcEntrySize = sizeof(ScrapTypeSpec);*/
                                                                /*  ScrapTypeSpec     group1SrcTypes[group1SrcCount]*/
                                                                /*  unsigned long     group1DstCount;*/
                                                                /*    unsigned long     group1DstEntrySize = sizeof(ScrapTypeSpec);*/
                                                                /*  ScrapTypeSpec     group1DstTypes[group1DstCount]*/
};
typedef struct ScrapTranslationList     ScrapTranslationList;
typedef ScrapTranslationList *          ScrapTranslationListPtr;
typedef ScrapTranslationListPtr *       ScrapTranslationListHandle;
/* definition of callbacks to update progress dialog*/

typedef long                            TranslationRefNum;
/*****************************************************************************************
*
* This routine sets the advertisement in the top half of the progress dialog.
* It is called once at the beginning of your DoTranslateFile routine.
*
* Enter:    refNum          Translation reference supplied to DoTranslateFile.
*           advertisement   A handle to the picture to display.  This must be non-purgable.
*                           Before returning from DoTranslateFile, you should dispose
*                           of the memory.  (Normally, it is in the temp translation heap
*                           so it is cleaned up for you.)
*
* Exit: returns         noErr, paramErr, or memFullErr
*/
EXTERN_API( OSErr )
SetTranslationAdvertisement     (TranslationRefNum      refNum,
                                 PicHandle              advertisement)                      TWOWORDINLINE(0x7002, 0xABFC);


/*****************************************************************************************
*
* This routine updates the progress bar in the progress dialog.
* It is called repeatedly from within your DoTranslateFile routine.
* It should be called often, so that the user will get feedback if he tries to cancel.
*
* Enter:    refNum      translation reference supplied to DoTranslateFile.
*           progress    percent complete (0-100)
*
* Exit:     canceled    TRUE if the user clicked the Cancel button, FALSE otherwise
*           returns     noErr, paramErr, or memFullErr
*/
EXTERN_API( OSErr )
UpdateTranslationProgress       (TranslationRefNum      refNum,
                                 short                  percentDone,
                                 Boolean *              canceled)                           TWOWORDINLINE(0x7001, 0xABFC);



/* ComponentMgr selectors for routines*/
enum {
    kTranslateGetFileTranslationList = 0,                       /* component selectors*/
    kTranslateIdentifyFile      = 1,
    kTranslateTranslateFile     = 2,
    kTranslateGetTranslatedFilename = 3,
    kTranslateGetScrapTranslationList = 10,                     /* skip to scrap routines*/
    kTranslateIdentifyScrap     = 11,
    kTranslateTranslateScrap    = 12,
    kTranslateGetScrapTranslationListConsideringData = 13
};


/* Routines to implment in a file translation extension*/


typedef CALLBACK_API( ComponentResult , DoGetFileTranslationListProcPtr )(ComponentInstance self, FileTranslationListHandle translationList);
typedef CALLBACK_API( ComponentResult , DoIdentifyFileProcPtr )(ComponentInstance self, const FSSpec *theDocument, FileType *docType);
typedef CALLBACK_API( ComponentResult , DoTranslateFileProcPtr )(ComponentInstance self, TranslationRefNum refNum, const FSSpec *sourceDocument, FileType srcType, long srcTypeHint, const FSSpec *dstDoc, FileType dstType, long dstTypeHint);
typedef CALLBACK_API( ComponentResult , DoGetTranslatedFilenameProcPtr )(ComponentInstance self, FileType dstType, long dstTypeHint, FSSpec *theDocument);

/* Routine to implement in a scrap translation extension*/

typedef CALLBACK_API( ComponentResult , DoGetScrapTranslationListProcPtr )(ComponentInstance self, ScrapTranslationListHandle list);
typedef CALLBACK_API( ComponentResult , DoIdentifyScrapProcPtr )(ComponentInstance self, const void *dataPtr, Size dataLength, ScrapType *dataFormat);
typedef CALLBACK_API( ComponentResult , DoTranslateScrapProcPtr )(ComponentInstance self, TranslationRefNum refNum, const void *srcDataPtr, Size srcDataLength, ScrapType srcType, long srcTypeHint, Handle dstData, ScrapType dstType, long dstTypeHint);




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

#endif /* __TRANSLATIONEXTENSIONS__ */

