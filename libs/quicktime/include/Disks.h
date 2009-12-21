/*
     File:       Disks.h
 
     Contains:   Disk Driver Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DISKS__
#define __DISKS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __OSUTILS__
#include <OSUtils.h>
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
    sony                        = 0,
    hard20                      = 1
};

/* Disk Driver Status csCodes */
enum {
    kReturnFormatList           = 6,                            /* .Sony */
    kDriveStatus                = 8,
    kMFMStatus                  = 10
};

/* Disk Driver Control csCodes */
enum {
    kVerify                     = 5,
    kFormat                     = 6,
    kEject                      = 7,
    kSetTagBuffer               = 8,                            /* .Sony */
    kTrackCache                 = 9,                            /* .Sony */
    kDriveIcon                  = 21,
    kMediaIcon                  = 22,
    kDriveInfo                  = 23,
    kRawTrack                   = 18244                         /* .Sony: "diagnostic" raw track dump */
};

/*
    Note:

    qLink is usually the first field in queues, but back in the MacPlus
    days, the DrvSts record needed to be expanded.  In order to do this without
    breaking disk drivers that already added stuff to the end, the fields
    where added to the beginning.  This was originally done in assembly language
    and the record was defined to start at a negative offset, so that the qLink
    field would end up at offset zero.  When the C and pascal interfaces where
    made, they could not support negative record offsets, so qLink was no longer
    the first field.  Universal Interfaces are auto generated and don't support
    negative offsets for any language, so DrvSts in Disks.a has qLinks at a
    none zero offset.  Assembly code which switches to Universal Interfaces will
    need to compensate for that.

*/


struct DrvSts {
    short                           track;                      /* current track */
    char                            writeProt;                  /* bit 7 = 1 if volume is locked */
    char                            diskInPlace;                /* disk in drive */
    char                            installed;                  /* drive installed */
    char                            sides;                      /* -1 for 2-sided, 0 for 1-sided */
    QElemPtr                        qLink;                      /* next queue entry */
    short                           qType;                      /* 1 for HD20 */
    short                           dQDrive;                    /* drive number */
    short                           dQRefNum;                   /* driver reference number */
    short                           dQFSID;                     /* file system ID */
    char                            twoSideFmt;                 /* after 1st rd/wrt: 0=1 side, -1=2 side */
    char                            needsFlush;                 /* -1 for MacPlus drive */
    short                           diskErrs;                   /* soft error count */
};
typedef struct DrvSts                   DrvSts;

struct DrvSts2 {
    short                           track;
    char                            writeProt;
    char                            diskInPlace;
    char                            installed;
    char                            sides;
    QElemPtr                        qLink;
    short                           qType;
    short                           dQDrive;
    short                           dQRefNum;
    short                           dQFSID;
    short                           driveSize;
    short                           driveS1;
    short                           driveType;
    short                           driveManf;
    short                           driveChar;
    char                            driveMisc;
};
typedef struct DrvSts2                  DrvSts2;
enum {
    kdqManualEjectBit           = 5
};

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
DiskEject                       (short                  drvNum);

EXTERN_API( OSErr )
SetTagBuffer                    (void *                 buffPtr);

EXTERN_API( OSErr )
DriveStatus                     (short                  drvNum,
                                 DrvSts *               status);

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

#endif /* __DISKS__ */

