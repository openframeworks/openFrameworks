/*
     File:       CMPRComponent.h
 
     Contains:   ColorSync ProfileResponder Component API
 
     Version:    Technology: ColorSync 1.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1993-2001 by Apple Computer, Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CMPRCOMPONENT__
#define __CMPRCOMPONENT__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __CMAPPLICATION__
#include <CMApplication.h>
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
    CMPRInterfaceVersion        = 0
};

/* Component function selectors */
enum {
    kCMPRGetProfile             = 0,
    kCMPRSetProfile             = 1,
    kCMPRSetProfileDescription  = 2,
    kCMPRGetIndexedProfile      = 3,
    kCMPRDeleteDeviceProfile    = 4
};


#if CALL_NOT_IN_CARBON
EXTERN_API( CMError )
CMGetProfile                    (ComponentInstance      pr,
                                 CMProfileHandle        aProfile,
                                 CMProfileHandle *      returnedProfile)                    FIVEWORDINLINE(0x2F3C, 0x0008, 0x0000, 0x7000, 0xA82A);

EXTERN_API( CMError )
CMSetProfile                    (ComponentInstance      pr,
                                 CMProfileHandle        newProfile)                         FIVEWORDINLINE(0x2F3C, 0x0004, 0x0001, 0x7000, 0xA82A);

EXTERN_API( CMError )
CMSetProfileDescription         (ComponentInstance      pr,
                                 long                   DeviceData,
                                 CMProfileHandle        hProfile)                           FIVEWORDINLINE(0x2F3C, 0x0008, 0x0002, 0x7000, 0xA82A);

EXTERN_API( CMError )
CMGetIndexedProfile             (ComponentInstance      pr,
                                 CMProfileSearchRecordHandle  search,
                                 CMProfileHandle *      returnProfile,
                                 long *                 index)                              FIVEWORDINLINE(0x2F3C, 0x000C, 0x0003, 0x7000, 0xA82A);

EXTERN_API( CMError )
CMDeleteDeviceProfile           (ComponentInstance      pr,
                                 CMProfileHandle        deleteMe)                           FIVEWORDINLINE(0x2F3C, 0x0004, 0x0004, 0x7000, 0xA82A);


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

#endif /* __CMPRCOMPONENT__ */

