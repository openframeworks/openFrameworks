/*
     File:       CFData.h
 
     Contains:   CoreFoundation block of bytes
 
     Version:    Technology: Mac OS X
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFDATA__
#define __CFDATA__

#ifndef __CFBASE__
#include <CFBase.h>
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

typedef const struct __CFData * CFDataRef;
typedef struct __CFData * CFMutableDataRef;
EXTERN_API_C( CFTypeID )
CFDataGetTypeID                 (void);


EXTERN_API_C( CFDataRef )
CFDataCreate                    (CFAllocatorRef         allocator,
                                 const UInt8 *          bytes,
                                 CFIndex                length);

EXTERN_API_C( CFDataRef )
CFDataCreateWithBytesNoCopy     (CFAllocatorRef         allocator,
                                 const UInt8 *          bytes,
                                 CFIndex                length,
                                 CFAllocatorRef         bytesDeallocator);

/* Pass kCFAllocatorNull as bytesDeallocator to assure the bytes aren't freed */
EXTERN_API_C( CFDataRef )
CFDataCreateCopy                (CFAllocatorRef         allocator,
                                 CFDataRef              data);

EXTERN_API_C( CFMutableDataRef )
CFDataCreateMutable             (CFAllocatorRef         allocator,
                                 CFIndex                capacity);

EXTERN_API_C( CFMutableDataRef )
CFDataCreateMutableCopy         (CFAllocatorRef         allocator,
                                 CFIndex                capacity,
                                 CFDataRef              data);


EXTERN_API_C( CFIndex )
CFDataGetLength                 (CFDataRef              data);

EXTERN_API_C( const UInt8 *)
CFDataGetBytePtr                (CFDataRef              data);

EXTERN_API_C( UInt8 *)
CFDataGetMutableBytePtr         (CFMutableDataRef       data);

EXTERN_API_C( void )
CFDataGetBytes                  (CFDataRef              data,
                                 CFRange                range,
                                 UInt8 *                buffer);


EXTERN_API_C( void )
CFDataSetLength                 (CFMutableDataRef       data,
                                 CFIndex                length);

EXTERN_API_C( void )
CFDataIncreaseLength            (CFMutableDataRef       data,
                                 CFIndex                extraLength);

EXTERN_API_C( void )
CFDataAppendBytes               (CFMutableDataRef       data,
                                 const UInt8 *          bytes,
                                 CFIndex                length);

EXTERN_API_C( void )
CFDataReplaceBytes              (CFMutableDataRef       data,
                                 CFRange                range,
                                 const UInt8 *          newBytes,
                                 CFIndex                newLength);

EXTERN_API_C( void )
CFDataDeleteBytes               (CFMutableDataRef       data,
                                 CFRange                range);


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

#endif /* __CFDATA__ */

