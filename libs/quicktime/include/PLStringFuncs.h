/*
     File:       PLStringFuncs.h
 
     Contains:   xxx put contents here xxx
 
     Version:    Technology: xxx put version here xxx
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __PLSTRINGFUNCS__
#define __PLSTRINGFUNCS__

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

EXTERN_API( short )
PLstrcmp                        (ConstStr255Param       str1,
                                 ConstStr255Param       str2);

EXTERN_API( short )
PLstrncmp                       (ConstStr255Param       str1,
                                 ConstStr255Param       str2,
                                 short                  num);

EXTERN_API( StringPtr )
PLstrcpy                        (StringPtr              str1,
                                 ConstStr255Param       str2);

EXTERN_API( StringPtr )
PLstrncpy                       (StringPtr              str1,
                                 ConstStr255Param       str2,
                                 short                  num);

EXTERN_API( StringPtr )
PLstrcat                        (StringPtr              str1,
                                 ConstStr255Param       str2);

EXTERN_API( StringPtr )
PLstrncat                       (StringPtr              str1,
                                 ConstStr255Param       str2,
                                 short                  num);

EXTERN_API( Ptr )
PLstrchr                        (ConstStr255Param       str1,
                                 short                  ch1);

EXTERN_API( Ptr )
PLstrrchr                       (ConstStr255Param       str1,
                                 short                  ch1);

EXTERN_API( Ptr )
PLstrpbrk                       (ConstStr255Param       str1,
                                 ConstStr255Param       str2);

EXTERN_API( short )
PLstrspn                        (ConstStr255Param       str1,
                                 ConstStr255Param       str2);

EXTERN_API( Ptr )
PLstrstr                        (ConstStr255Param       str1,
                                 ConstStr255Param       str2);

EXTERN_API( short )
PLstrlen                        (ConstStr255Param       str);

EXTERN_API( short )
PLpos                           (ConstStr255Param       str1,
                                 ConstStr255Param       str2);



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

#endif /* __PLSTRINGFUNCS__ */

