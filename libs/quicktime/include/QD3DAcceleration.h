/*
     File:       QD3DAcceleration.h
 
     Contains:   Header file for low-level 3D driver API
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DACCELERATION__
#define __QD3DACCELERATION__

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
        #define __QD3DACCELERATION__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DACCELERATION__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                         Vendor ID definitions                            **
 **                                                                          **
 *****************************************************************************/
/*
 * If kQAVendor_BestChoice is used, the system chooses the "best" drawing engine
 * available for the target device. This should be used for the default.
 */
enum {
    kQAVendor_BestChoice        = -1
};

/*
 * The other definitions (kQAVendor_Apple, etc.) identify specific vendors
 * of drawing engines. When a vendor ID is used in conjunction with a
 * vendor-defined engine ID, a specific drawing engine can be selected.
 */
enum {
    kQAVendor_Apple             = 0,
    kQAVendor_ATI               = 1,
    kQAVendor_Radius            = 2,
    kQAVendor_DesignMark        = 3,
    kQAVendor_Matrox            = 4,
    kQAVendor_Yarc              = 5,
    kQAVendor_DiamondMM         = 6,
    kQAVendor_3DLabs            = 7,
    kQAVendor_D3DAdaptor        = 8,
    kQAVendor_IXMicro           = 9,
    kQAVendor_NumberNine        = 10,
    kQAVendor_MicroConversions  = 11,
    kQAVendor_PurpleShark       = 12,
    kQAVendor_VillageTronic     = 14
};

/******************************************************************************
 **                                                                          **
 **                      Apple's engine ID definitions                       **
 **                                                                          **
 *****************************************************************************/
enum {
    kQAEngine_AppleSW           = 0,                            /* Default software rasterizer*/
    kQAEngine_AppleHW           = -1,                           /* Apple accelerator*/
    kQAEngine_AppleHW2          = 1,                            /* Another Apple accelerator*/
    kQAEngine_AppleHW3          = 2                             /* Another Apple accelerator*/
};




#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DACCELERATION__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DACCELERATION__RESTORE_PACKED_ENUMS)
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

#endif /* __QD3DACCELERATION__ */

