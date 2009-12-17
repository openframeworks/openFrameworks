/*
     File:       QD3DStorage.h
 
     Contains:   Abstraction to deal with various types of stream-based storage devices
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DSTORAGE__
#define __QD3DSTORAGE__

#ifndef __QD3D__
#include <QD3D.h>
#endif


#if TARGET_OS_MAC
#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#endif  /* TARGET_OS_MAC */


#if TARGET_OS_WIN32
   #include <windows.h>
#endif /* TARGET_OS_WIN32 */

#include <stdio.h>


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
    #pragma options align=power
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

#if PRAGMA_ENUM_ALWAYSINT
    #if defined(__fourbyteints__) && !__fourbyteints__ 
        #define __QD3DSTORAGE__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DSTORAGE__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                             Storage Routines                             **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3ObjectType )
Q3Storage_GetType               (TQ3StorageObject       storage);

EXTERN_API_C( TQ3Status )
Q3Storage_GetSize               (TQ3StorageObject       storage,
                                 unsigned long *        size);

/* 
 *  Reads "dataSize" bytes starting at offset in storage, copying into data. 
 *  sizeRead returns the number of bytes filled in. 
 *  
 *  You may assume if *sizeRead < dataSize, then EOF is at offset + *sizeRead
 */
EXTERN_API_C( TQ3Status )
Q3Storage_GetData               (TQ3StorageObject       storage,
                                 unsigned long          offset,
                                 unsigned long          dataSize,
                                 unsigned char *        data,
                                 unsigned long *        sizeRead);

/* 
 *  Write "dataSize" bytes starting at offset in storage, copying from data. 
 *  sizeWritten returns the number of bytes filled in. 
 *  
 *  You may assume if *sizeRead < dataSize, then EOF is at offset + *sizeWritten
 */
EXTERN_API_C( TQ3Status )
Q3Storage_SetData               (TQ3StorageObject       storage,
                                 unsigned long          offset,
                                 unsigned long          dataSize,
                                 const unsigned char *  data,
                                 unsigned long *        sizeWritten);

/******************************************************************************
 **                                                                          **
 **                          Memory Storage Prototypes                       **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3ObjectType )
Q3MemoryStorage_GetType         (TQ3StorageObject       storage);

/*
 * These calls COPY the buffer into QD3D space
 */
EXTERN_API_C( TQ3StorageObject )
Q3MemoryStorage_New             (const unsigned char *  buffer,
                                 unsigned long          validSize);

EXTERN_API_C( TQ3Status )
Q3MemoryStorage_Set             (TQ3StorageObject       storage,
                                 const unsigned char *  buffer,
                                 unsigned long          validSize);

/*
 * These calls use the pointer given - you must dispose it when you're through
 */
#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StorageObject )
Q3MemoryStorage_NewBuffer       (unsigned char *        buffer,
                                 unsigned long          validSize,
                                 unsigned long          bufferSize);

EXTERN_API_C( TQ3Status )
Q3MemoryStorage_SetBuffer       (TQ3StorageObject       storage,
                                 unsigned char *        buffer,
                                 unsigned long          validSize,
                                 unsigned long          bufferSize);

EXTERN_API_C( TQ3Status )
Q3MemoryStorage_GetBuffer       (TQ3StorageObject       storage,
                                 unsigned char **       buffer,
                                 unsigned long *        validSize,
                                 unsigned long *        bufferSize);

#endif  /* CALL_NOT_IN_CARBON */

#if TARGET_OS_MAC
/******************************************************************************
 **                                                                          **
 **                             Macintosh Handles Prototypes                 **
 **                                                                          **
 *****************************************************************************/
/* Handle Storage is a subclass of Memory Storage */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StorageObject )
Q3HandleStorage_New             (Handle                 handle,
                                 unsigned long          validSize);

EXTERN_API_C( TQ3Status )
Q3HandleStorage_Set             (TQ3StorageObject       storage,
                                 Handle                 handle,
                                 unsigned long          validSize);

EXTERN_API_C( TQ3Status )
Q3HandleStorage_Get             (TQ3StorageObject       storage,
                                 Handle *               handle,
                                 unsigned long *        validSize);

/******************************************************************************
 **                                                                          **
 **                             Macintosh Storage Prototypes                 **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3StorageObject )
Q3MacintoshStorage_New          (short                  fsRefNum);

/* Note: This storage is assumed open */
EXTERN_API_C( TQ3Status )
Q3MacintoshStorage_Set          (TQ3StorageObject       storage,
                                 short                  fsRefNum);

EXTERN_API_C( TQ3Status )
Q3MacintoshStorage_Get          (TQ3StorageObject       storage,
                                 short *                fsRefNum);

EXTERN_API_C( TQ3ObjectType )
Q3MacintoshStorage_GetType      (TQ3StorageObject       storage);


/******************************************************************************
 **                                                                          **
 **                         Macintosh FSSpec Storage Prototypes              **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3StorageObject )
Q3FSSpecStorage_New             (const FSSpec *         fs);

EXTERN_API_C( TQ3Status )
Q3FSSpecStorage_Set             (TQ3StorageObject       storage,
                                 const FSSpec *         fs);

EXTERN_API_C( TQ3Status )
Q3FSSpecStorage_Get             (TQ3StorageObject       storage,
                                 FSSpec *               fs);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_OS_MAC */

#if TARGET_OS_WIN32
/******************************************************************************
 **                                                                          **
 **                         Win32 HANDLE Storage Prototypes                  **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StorageObject )
Q3Win32Storage_New              (HANDLE                 hFile);

EXTERN_API_C( TQ3Status )
Q3Win32Storage_Set              (TQ3StorageObject       storage,
                                 HANDLE                 hFile);

EXTERN_API_C( TQ3Status )
Q3Win32Storage_Get              (TQ3StorageObject       storage,
                                 HANDLE *               hFile);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_OS_WIN32 */


/******************************************************************************
 **                                                                          **
 **                                 Unix Prototypes                          **
 **             The Unix Storage prototypes have been obsoleted.             **
 **                                                                          **
 *****************************************************************************/


/******************************************************************************
 **                                                                          **
 **                             Unix Path Prototypes                         **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3StorageObject )
Q3UnixPathStorage_New           (const char *           pathName);

/* C string */
EXTERN_API_C( TQ3Status )
Q3UnixPathStorage_Set           (TQ3StorageObject       storage,
                                 const char *           pathName);

/* C string */
EXTERN_API_C( TQ3Status )
Q3UnixPathStorage_Get           (TQ3StorageObject       storage,
                                 char *                 pathName);

/* pathName is a buffer */



#endif  /* CALL_NOT_IN_CARBON */


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DSTORAGE__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DSTORAGE__RESTORE_PACKED_ENUMS)
    #pragma options(pack_enums)
#endif

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

#endif /* __QD3DSTORAGE__ */

