/*
     File:       Translation.h
 
     Contains:   Translation Manager (Macintosh Easy Open) Interfaces.
 
     Version:    Technology: Macintosh Easy Open 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1991-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TRANSLATION__
#define __TRANSLATION__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __TRANSLATIONEXTENSIONS__
#include <TranslationExtensions.h>
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

/* enumerated types on how a document can be opened*/
typedef short                           DocOpenMethod;
enum {
    domCannot                   = 0,
    domNative                   = 1,
    domTranslateFirst           = 2,
    domWildcard                 = 3
};

/* 0L terminated array of OSTypes, or FileTypes*/
typedef OSType                          TypesBlock[64];
typedef OSType *                        TypesBlockPtr;
/* Progress dialog resource ID*/
enum {
    kTranslationScrapProgressDialogID = -16555
};

/* block of data that describes how to translate*/

struct FileTranslationSpec {
    OSType                          componentSignature;
    const void *                    translationSystemInfo;
    FileTypeSpec                    src;
    FileTypeSpec                    dst;
};
typedef struct FileTranslationSpec      FileTranslationSpec;
typedef FileTranslationSpec *           FileTranslationSpecArrayPtr;
typedef FileTranslationSpecArrayPtr *   FileTranslationSpecArrayHandle;

/*****************************************************************************************
* 
*   GetFileTypesThatAppCanNativelyOpen
* 
*  This routine returns a list of all FileTypes that an application can open by itself
* 
*  Enter:   appVRefNumHint      volume where application resides (can be wrong, and if is, will be used as a starting point)
*           appSignature        signature (creator) of application
*           nativeTypes         pointer to a buffer to be filled with up to 64 FileTypes
* 
*  Exit:    nativeTypes         zero terminated array of FileTypes that can be opened by app
*/
EXTERN_API( OSErr )
GetFileTypesThatAppCanNativelyOpen (short               appVRefNumHint,
                                 OSType                 appSignature,
                                 FileType *             nativeTypes)                        TWOWORDINLINE(0x701C, 0xABFC);

/*****************************************************************************************
* 
*  ExtendFileTypeList
* 
*  This routine makes a new list of file types that can be translated into a type in the given list
*  Used by StandardFile
* 
*  Enter:   originalTypeList        pointer to list of file types that can be opened
*           numberOriginalTypes     number of file types in orgTypeList
*           extendedTypeList        pointer to a buffer to be filled with file types
*           numberExtendedTypes     max number of file types that can be put in extendedTypeList
* 
*  Exit:    extendedTypeList        buffer filled in with file types that can be translated
*           numberExtendedTypes     number of file types put in extendedTypeList
*/
EXTERN_API( OSErr )
ExtendFileTypeList              (const FileType *       originalTypeList,
                                 short                  numberOriginalTypes,
                                 FileType *             extendedTypeList,
                                 short *                numberExtendedTypes)                TWOWORDINLINE(0x7009, 0xABFC);


/*****************************************************************************************
* 
* 
*  This routine checks if a file can be opened by a particular application.
*  If so, it returns if it needs to be translated first, and if so then how.
*  The FileTypes that the app can open are specified by nativelyOpenableTypes,
*  or if it is NULL, GetFileTypesThatAppCanNativelyOpen is called.
* 
*  Enter:   targetDocument      document to check if it can be opened
*           appVRefNumHint      vRefNum of application to open doc ( can be wrong, and if is, will be used as a starting point)
*           appSignature        signature (creator) of application to open doc
*           nativeTypes         zero terminated list of FileTypes app can open natively, or NULL to use default list
*           onlyNative          whether to consider if document can be translated before opening
*           howToOpen           pointer to buffer in which to put how the document can be opened
*           howToTranslate      pointer to buffer in which to put a FileTranslationSpec record
* 
*  Exit:    howToOpen           whether file needs to be translated to be read
*           howToTranslate      if file can be translated, buffer filled in with how to translate
*           returns             noErr, noPrefAppErr
*/
EXTERN_API( OSErr )
CanDocBeOpened                  (const FSSpec *         targetDocument,
                                 short                  appVRefNumHint,
                                 OSType                 appSignature,
                                 const FileType *       nativeTypes,
                                 Boolean                onlyNative,
                                 DocOpenMethod *        howToOpen,
                                 FileTranslationSpec *  howToTranslate)                     TWOWORDINLINE(0x701E, 0xABFC);


/*****************************************************************************************
* 
*  GetFileTranslationPaths
* 
*  This routine returns a list of all ways a translation can occure to or from a FileType.
*  The app is checked to exist.  The hint for each app is the VRefNum and DTRefNum
* 
*  Enter:   srcDoc          source file or NULL for all matches
*           dstDoc          destination FileType or 0 for all matches
*           maxResultCount
*           resultBuffer
*  Exit:    number of paths
*/
EXTERN_API( short )
GetFileTranslationPaths         (FSSpec *               srcDocument,
                                 FileType               dstDocType,
                                 unsigned short         maxResultCount,
                                 FileTranslationSpecArrayPtr  resultBuffer)                 TWOWORDINLINE(0x7038, 0xABFC);

/*****************************************************************************************
* 
*  GetPathFromTranslationDialog
* 
*  This routine, with a given document, application, and a passed typelist will display the
*  Macintosh Easy Open translation dialog allowing the user to make a choice.  The choice
*  made will be written as a preference (so the next call to CanDocBeOpened() will work).
*  The routine returns the translation path information.
* 
*  Enter:   theDocument         FSSpec to document to open
*           theApplication      FSSpec to application to open document
*           typeList            Nil terminated list of FileType's (e.g. SFTypeList-like) of types
*                               you would like the documented translated to.  Order most perferred
*                               to least.
* 
*  Exit:    howToOpen           Translation method needed to open document
*           howToTranslate      Translation specification
*           returns             Any errors that might occur.
*/
EXTERN_API( OSErr )
GetPathFromTranslationDialog    (const FSSpec *         theDocument,
                                 const FSSpec *         theApplication,
                                 TypesBlockPtr          typeList,
                                 DocOpenMethod *        howToOpen,
                                 FileTranslationSpec *  howToTranslate)                     TWOWORDINLINE(0x7037, 0xABFC);


/*****************************************************************************************
* 
*   TranslateFile
* 
*  This routine reads a file of one format and writes it to another file in another format. 
*  The information on how to translated is generated by the routine CanDocBeOpened.
*  TranslateFile calls through to the TranslateFile Extension's DoTranslateFile routine.  
*  The destination file must not exist.  It is created by this routine.  
* 
*  Enter:   sourceDocument          input file to translate
*           destinationDocument     output file of translation
*           howToTranslate          pointer to info on how to translate
*  Exit:    returns                 noErr, badTranslationSpecErr 
*/
EXTERN_API( OSErr )
TranslateFile                   (const FSSpec *         sourceDocument,
                                 const FSSpec *         destinationDocument,
                                 const FileTranslationSpec * howToTranslate)                TWOWORDINLINE(0x700C, 0xABFC);

/*****************************************************************************************
* 
*   GetDocumentKindString
* 
*  This routine returns the string the Finder should show for the "kind" of a document
*  in the GetInfo window and in the kind column of a list view.  
* 
*  Enter:   docVRefNum      The volume containing the document
*           docType         The catInfo.fdType of the document
*           docCreator      The catInfo.fdCreator of the document
*           kindString      pointer to where to return the string
* 
*  Exit:    kindString      pascal string.  Ex: "\pSurfCalc spreadsheet"
*           returns         noErr, or afpItemNoFound if kind could not be determined
*/
EXTERN_API( OSErr )
GetDocumentKindString           (short                  docVRefNum,
                                 OSType                 docType,
                                 OSType                 docCreator,
                                 Str63                  kindString)                         TWOWORDINLINE(0x7016, 0xABFC);

/*****************************************************************************************
* 
*  GetTranslationExtensionName
* 
*  This routine returns the translation system name from a specified TranslationSpec
* 
*  Enter:   translationMethod   The translation path to get the translation name from
* 
*  Exit:    extensionName       The name of the translation system
*           returns             Any errors that might occur
*/
EXTERN_API( OSErr )
GetTranslationExtensionName     (const FileTranslationSpec * translationMethod,
                                 Str31                  extensionName)                      TWOWORDINLINE(0x7036, 0xABFC);


/*****************************************************************************************
* 
*  GetScrapDataProcPtr
* 
*  This is a prototype for the function you must supply to TranslateScrap. It is called to 
*  get the data to be translated.  The first call TranslateScrap will make to this is to
*  ask for the 'fmts' data.  That is a special.   You should resize and fill in the handle
*  with a list all the formats that you have available to be translated, and the length of each.
*  (See I.M. VI 4-23 for details of 'fmts').  It will then be called again asking for one of  
*  the formats that 'fmts' list said was available.
* 
*  Enter:   requestedFormat         Format of data that TranslateScrap needs.
*           dataH                   Handle in which to put the requested data
*           srcDataGetterRefCon     Extra parameter for you passed to TranslateScrap
*           
*  Exit:    dataH                   Handle is resized and filled with data in requested format
*/
typedef CALLBACK_API( OSErr , GetScrapDataProcPtr )(ScrapType requestedFormat, Handle dataH, void *srcDataGetterRefCon);
typedef STACK_UPP_TYPE(GetScrapDataProcPtr)                     GetScrapDataUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(GetScrapDataUPP)
    NewGetScrapDataUPP             (GetScrapDataProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeGetScrapDataUPP         (GetScrapDataUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeGetScrapDataUPP          (ScrapType               requestedFormat,
                                    Handle                  dataH,
                                    void *                  srcDataGetterRefCon,
                                    GetScrapDataUPP         userUPP);

#else
    enum { uppGetScrapDataProcInfo = 0x00000FE0 };                  /* pascal 2_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewGetScrapDataUPP(userRoutine)                         (GetScrapDataUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppGetScrapDataProcInfo, GetCurrentArchitecture())
    #define DisposeGetScrapDataUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeGetScrapDataUPP(requestedFormat, dataH, srcDataGetterRefCon, userUPP)  (OSErr)CALL_THREE_PARAMETER_UPP((userUPP), uppGetScrapDataProcInfo, (requestedFormat), (dataH), (srcDataGetterRefCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewGetScrapDataProc(userRoutine)                        NewGetScrapDataUPP(userRoutine)
#define CallGetScrapDataProc(userRoutine, requestedFormat, dataH, srcDataGetterRefCon) InvokeGetScrapDataUPP(requestedFormat, dataH, srcDataGetterRefCon, userRoutine)

typedef GetScrapDataUPP                 GetScrapData;
/*****************************************************************************************
* 
*  TranslateScrap
* 
*  This routine resizes the destination handle and fills it with data of the requested format.
*  The data is generated by translated one or more source formats of data supplied by
*  the procedure srcDataGetter.  
*  This routine is automatically called by GetScrap and ReadEdition.  You only need to call
*  this if you need to translated scrap style data, but are not using the ScrapMgr or EditionMgr.
* 
*  Enter:   sourceDataGetter            Pointer to routine that can get src data
*           sourceDataGetterRefCon      Extra parameter for dataGetter
*           destinationFormat           Format of data desired
*           destinationData             Handle in which to store translated data
*           
*  Exit:    dstData                     Handle is resized and filled with data in requested format
*/
EXTERN_API( OSErr )
TranslateScrap                  (GetScrapDataUPP        sourceDataGetter,
                                 void *                 sourceDataGetterRefCon,
                                 ScrapType              destinationFormat,
                                 Handle                 destinationData,
                                 short                  progressDialogID)                   TWOWORDINLINE(0x700E, 0xABFC);




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

#endif /* __TRANSLATION__ */

