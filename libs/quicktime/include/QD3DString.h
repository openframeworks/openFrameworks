/*
     File:       QD3DString.h
 
     Contains:   Q3CString methods
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DSTRING__
#define __QD3DSTRING__

#ifndef __QD3D__
#include <QD3D.h>
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
    #pragma options align=power
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

#if PRAGMA_ENUM_ALWAYSINT
    #if defined(__fourbyteints__) && !__fourbyteints__ 
        #define __QD3DSTRING__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DSTRING__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                             String Routines                              **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3ObjectType )
Q3String_GetType                (TQ3StringObject        stringObj);


/******************************************************************************
 **                                                                          **
 **                     C String Routines                                    **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3StringObject )
Q3CString_New                   (const char *           str);

EXTERN_API_C( TQ3Status )
Q3CString_GetLength             (TQ3StringObject        stringObj,
                                 unsigned long *        length);

EXTERN_API_C( TQ3Status )
Q3CString_SetString             (TQ3StringObject        stringObj,
                                 const char *           str);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3CString_GetString             (TQ3StringObject        stringObj,
                                 char **                str);

EXTERN_API_C( TQ3Status )
Q3CString_EmptyData             (char **                str);

#endif  /* CALL_NOT_IN_CARBON */



#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DSTRING__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DSTRING__RESTORE_PACKED_ENUMS)
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

#endif /* __QD3DSTRING__ */

