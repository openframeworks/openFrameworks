/*
     File:       DeviceControl.h
 
     Contains:   Component API for doing AVC transactions.
 
     Version:    Technology: xxx put version here xxx
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DEVICECONTROL__
#define __DEVICECONTROL__

#ifndef __MACTYPES__
#include <MacTypes.h>
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

typedef CALLBACK_API_C( UInt32 , DCResponseHandler )(UInt32 fwCommandObjectID, Ptr responseBuffer, UInt32 responseLength);

struct DVCTransactionParams {
    Ptr                             commandBufferPtr;
    UInt32                          commandLength;
    Ptr                             responseBufferPtr;
    UInt32                          responseBufferSize;
    DCResponseHandler *             responseHandler;
};
typedef struct DVCTransactionParams     DVCTransactionParams;
#if CALL_NOT_IN_CARBON
EXTERN_API( ComponentResult )
DeviceControlDoAVCTransaction   (ComponentInstance      instance,
                                 DVCTransactionParams * params)                             FIVEWORDINLINE(0x2F3C, 0x0004, 0x0001, 0x7000, 0xA82A);



/* selectors for component calls */
enum {
    kDeviceControlDoAVCTransactionSelect       = 0x0001
};
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

#endif /* __DEVICECONTROL__ */

