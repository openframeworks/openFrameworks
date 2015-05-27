/*
     File:       UTCUtils.h
 
     Contains:   Interface for UTC to Local Time conversion and 64 Bit Clock routines
 
     Version:    Technology: Mac OS 9
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __UTCUTILS__
#define __UTCUTILS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MACERRORS__
#include <MacErrors.h>
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

/* Options for Set & Get DateTime Routines */
enum {
    kUTCDefaultOptions          = 0
};

/* 64 Bit Clock Typedefs */

struct UTCDateTime {
    UInt16                          highSeconds;
    UInt32                          lowSeconds;
    UInt16                          fraction;
};
typedef struct UTCDateTime              UTCDateTime;
typedef UTCDateTime *                   UTCDateTimePtr;
typedef UTCDateTimePtr *                UTCDateTimeHandle;

struct LocalDateTime {
    UInt16                          highSeconds;
    UInt32                          lowSeconds;
    UInt16                          fraction;
};
typedef struct LocalDateTime            LocalDateTime;
typedef LocalDateTime *                 LocalDateTimePtr;
typedef LocalDateTimePtr *              LocalDateTimeHandle;
/* Classic 32 bit clock conversion routines */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
ConvertLocalTimeToUTC           (UInt32                 localSeconds,
                                 UInt32 *               utcSeconds);

EXTERN_API_C( OSStatus )
ConvertUTCToLocalTime           (UInt32                 utcSeconds,
                                 UInt32 *               localSeconds);

/* 64 bit clock conversion routines */
EXTERN_API_C( OSStatus )
ConvertUTCToLocalDateTime       (const UTCDateTime *    utcDateTime,
                                 LocalDateTime *        localDateTime);

EXTERN_API_C( OSStatus )
ConvertLocalToUTCDateTime       (const LocalDateTime *  localDateTime,
                                 UTCDateTime *          utcDateTime);

/* Getter and Setter Clock routines using 64 Bit values */
EXTERN_API_C( OSStatus )
GetUTCDateTime                  (UTCDateTime *          utcDateTime,
                                 OptionBits             options);

EXTERN_API_C( OSStatus )
SetUTCDateTime                  (const UTCDateTime *    utcDateTime,
                                 OptionBits             options);

EXTERN_API_C( OSStatus )
GetLocalDateTime                (LocalDateTime *        localDateTime,
                                 OptionBits             options);

EXTERN_API_C( OSStatus )
SetLocalDateTime                (const LocalDateTime *  localDateTime,
                                 OptionBits             options);

#endif  /* CALL_NOT_IN_CARBON */


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

#endif /* __UTCUTILS__ */

