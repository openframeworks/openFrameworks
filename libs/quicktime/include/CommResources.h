/*
     File:       CommResources.h
 
     Contains:   Communications Toolbox Resource Manager Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1988-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __COMMRESOURCES__
#define __COMMRESOURCES__

#ifndef __OSUTILS__
#include <OSUtils.h>
#endif

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
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

#if CALL_NOT_IN_CARBON
enum {
                                                                /*    tool classes (also the tool file types)    */
    classCM                     = FOUR_CHAR_CODE('cbnd'),
    classFT                     = FOUR_CHAR_CODE('fbnd'),
    classTM                     = FOUR_CHAR_CODE('tbnd')
};

enum {
                                                                /*    version of the Comm Resource Manager   */
    curCRMVersion               = 2,                            /* constants general to the use of the Communications Resource Manager */
    crmType                     = 9,                            /* queue type */
    crmRecVersion               = 1,                            /* version of queue structure */
                                                                /*    error codes */
    crmGenericError             = -1,
    crmNoErr                    = 0
};

/* data structures general to the use of the Communications Resource Manager */
typedef OSErr                           CRMErr;

struct CRMRec {
    QElemPtr                        qLink;                      /*reserved*/
    short                           qType;                      /*queue type -- ORD(crmType) = 9*/
    short                           crmVersion;                 /*version of queue element data structure*/
    long                            crmPrivate;                 /*reserved*/
    short                           crmReserved;                /*reserved*/
    long                            crmDeviceType;              /*type of device, assigned by DTS*/
    long                            crmDeviceID;                /*device ID; assigned when CRMInstall is called*/
    long                            crmAttributes;              /*pointer to attribute block*/
    long                            crmStatus;                  /*status variable - device specific*/
    long                            crmRefCon;                  /*for device private use*/
};
typedef struct CRMRec                   CRMRec;
typedef CRMRec *                        CRMRecPtr;
#if CALL_NOT_IN_CARBON
EXTERN_API( CRMErr )
InitCRM                         (void);

EXTERN_API( QHdrPtr )
CRMGetHeader                    (void);

EXTERN_API( void )
CRMInstall                      (CRMRecPtr              crmReqPtr);

EXTERN_API( OSErr )
CRMRemove                       (CRMRecPtr              crmReqPtr);

EXTERN_API( CRMRecPtr )
CRMSearch                       (CRMRecPtr              crmReqPtr);

EXTERN_API( short )
CRMGetCRMVersion                (void);

EXTERN_API( Handle )
CRMGetResource                  (ResType                theType,
                                 short                  theID);

EXTERN_API( Handle )
CRMGet1Resource                 (ResType                theType,
                                 short                  theID);

EXTERN_API( Handle )
CRMGetIndResource               (ResType                theType,
                                 short                  index);

EXTERN_API( Handle )
CRMGet1IndResource              (ResType                theType,
                                 short                  index);

EXTERN_API( Handle )
CRMGetNamedResource             (ResType                theType,
                                 ConstStr255Param       name);

EXTERN_API( Handle )
CRMGet1NamedResource            (ResType                theType,
                                 ConstStr255Param       name);

EXTERN_API( void )
CRMReleaseResource              (Handle                 theHandle);

EXTERN_API( Handle )
CRMGetToolResource              (short                  procID,
                                 ResType                theType,
                                 short                  theID);

EXTERN_API( Handle )
CRMGetToolNamedResource         (short                  procID,
                                 ResType                theType,
                                 ConstStr255Param       name);

EXTERN_API( void )
CRMReleaseToolResource          (short                  procID,
                                 Handle                 theHandle);

EXTERN_API( long )
CRMGetIndex                     (Handle                 theHandle);

EXTERN_API( short )
CRMLocalToRealID                (ResType                bundleType,
                                 short                  toolID,
                                 ResType                theType,
                                 short                  localID);

EXTERN_API( short )
CRMRealToLocalID                (ResType                bundleType,
                                 short                  toolID,
                                 ResType                theType,
                                 short                  realID);

EXTERN_API( OSErr )
CRMGetIndToolName               (OSType                 bundleType,
                                 short                  index,
                                 Str255                 toolName);

EXTERN_API( OSErr )
CRMFindCommunications           (short *                vRefNum,
                                 long *                 dirID);

EXTERN_API( Boolean )
CRMIsDriverOpen                 (ConstStr255Param       driverName);

EXTERN_API( CRMErr )
CRMParseCAPSResource            (Handle                 theHandle,
                                 ResType                selector,
                                 unsigned long *        value);

EXTERN_API( OSErr )
CRMReserveRF                    (short                  refNum);

EXTERN_API( OSErr )
CRMReleaseRF                    (short                  refNum);

#endif  /* CALL_NOT_IN_CARBON */

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

#endif /* __COMMRESOURCES__ */

