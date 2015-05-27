/*
     File:       AGP.h
 
     Contains:   API for Accelerated Graphics Port (AGP)
 
     Version:    Technology: 1.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __AGP__
#define __AGP__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

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
    #pragma options align=power
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

/* AGP related property names*/
#define kAGPMasterProperty "AGP_Master"
#define kAGPTargetProperty "AGP_Target"
#define kAGPAddressRange "AGP_Address_Range"
#define kAGPAddressBlock "AGP_Address_Block"
#define kAGPAlignment "AGP_Alignment"
#define kAGPAllowOverlap "AGP_AllowOverlap"
#define kAGPMajorRevision "AGP_MajorRevision"
#define kAGPMinorRevision "AGP_MinorRevision"
#define kAGPDepth "AGP_Depth"
#define kAGPSBASupport "AGP_SBA_Support"
#define AGPTransferType "AGP_Transfer_Type"
#define kAGPFourGig "AGP_4Gig_Enabled"
#define kAGPFastWrite "AGP_FastWrite_Enabled"
#define kAGPEnabled "AGP_Enabled"


/* AGP Capabilities Register definitions*/
enum {
                                                                /* AGP Register Offsets*/
    kAGPRevisionOffset          = 0x02,
    kAGPStatusOffset            = 0x04,
    kAGPCommandOffset           = 0x08,                         /* Major/Minor Rev Bits*/
    kAGPMinorRevMask            = 0x0F,
    kAGPMajorRevMask            = 0xF0,
    kAGPMinorRevBit             = 0x00,
    kAGPMajorRevBit             = 0x04,                         /* AGP Status Register definitions*/
    kAGPDataRateMask            = 0x00000003,
    kAGPFastWriteMask           = 0x00000010,
    kAGPFourGigAddrMask         = 0x00000020,
    kAGPSideBandAddrMask        = 0x00000200,
    kAGPRequestDepthMask        = (long)0xFF000000,
    kAGPDataRateBit             = 0x00,
    kAGPFastWriteBit            = 0x04,
    kAGPFourGigAddrBit          = 0x05,
    kAGPSideBandAddrBit         = 0x09,
    kAGPRequestDepthBit         = 0x18,                         /* AGP Command Register definitions*/
    kAGPEnableBit               = 0x08,
    kAGPEnableMask              = 0x00000100
};

/* AGP Speed definitions*/
enum {
    kAGPOneX                    = 1,
    kAGPTwoX                    = 2,
    kAGPFourX                   = 4
};

enum {
    kAGPQueryBaseAddress        = 1,
    kAGPQueryEnabled            = 2,
    kAGPQueryMaxAGPMemory       = 3,
    kAGPQueryFreeAGPMemory      = 4,
    kAGPEnabledStatus           = 1,
    kAGPDisabledStatus          = 0
};


struct AGPAddressPair {
    LogicalAddress                  systemLogicalAddress;
    LogicalAddress                  agpLogicalAddress;
    UInt32                          internalInfo[4];
};
typedef struct AGPAddressPair           AGPAddressPair;

typedef UInt32                          AGPQuery;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
AGPNewMemory                    (AGPAddressPair *       agpAddressPair,
                                 ByteCount              byteSize,
                                 Boolean                clear);

EXTERN_API_C( OSStatus )
AGPReserveMemory                (AGPAddressPair *       agpAddressPair,
                                 ByteCount              byteSize);

EXTERN_API_C( OSStatus )
AGPDisposeMemory                (AGPAddressPair *       agpAddress);

EXTERN_API_C( OSStatus )
AGPReleaseMemory                (AGPAddressPair *       agpAddress);

EXTERN_API_C( OSStatus )
AGPCommitMemory                 (AGPAddressPair *       agpAddress,
                                 Boolean                agpClientManagesCaches);

EXTERN_API_C( OSStatus )
AGPGetAGPLogicalAddress         (AGPAddressPair *       agpAddressPair);

EXTERN_API_C( OSStatus )
AGPGetSystemLogicalAddress      (AGPAddressPair *       agpAddressPair);

EXTERN_API_C( OSStatus )
AGPGetStatus                    (AGPQuery               query,
                                 void *                 result);

EXTERN_API_C( OSStatus )
AGPDecommitMemory               (AGPAddressPair *       agpAddress);

EXTERN_API_C( OSStatus )
AGPRestoreCacheState            (AGPAddressPair *       agpAddress);

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

#endif /* __AGP__ */

