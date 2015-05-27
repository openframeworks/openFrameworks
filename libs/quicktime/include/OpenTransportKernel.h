/*
     File:       OpenTransportKernel.h
 
     Contains:   Definitions for Open Transport kernel code, such as drivers and protocol modules.
 
     Version:    Technology: 2.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1993-2001 by Apple Computer, Inc. and Mentat Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OPENTRANSPORTKERNEL__
#define __OPENTRANSPORTKERNEL__

#ifdef OTKERNEL
#if !OTKERNEL
#error OpenTransportModule.h can only be used by kernel code.
#endif  /* !OTKERNEL */

#endif  /* defined(OTKERNEL) */

#define OTKERNEL 1
/*
   We include "OpenTransportProtocol.h", which in turn includes
   "OpenTransport.h", thereby picking up all the stuff which
   is shared between client and kernel.
*/
#ifndef __OPENTRANSPORTPROTOCOL__
#include <OpenTransportProtocol.h>
#endif

#ifndef __NAMEREGISTRY__
#include <NameRegistry.h>
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


#if defined(__MWERKS__) && TARGET_CPU_68K
 #pragma push
   #pragma pointers_in_D0
#endif

/* ***** From the Mentat "mistream.h" ******/


/*
   Parts of "mistream.h" that are shared by the client
   and the kernel are in "OpenTransportProtocol.h".
*/



/** Re-allow a queue to be scheduled for service */
#define enableok(q)    ((q)->q_flag &= ~QNOENB)

/** Prevent a queue from being scheduled */
#define noenable(q)   ((q)->q_flag |= QNOENB)

/** Get pointer to the mate queue */
#define OTHERQ(q)     (((q)->q_flag & QREADR) ? WR(q) : RD(q))

/** Get pointer to the read queue, assumes 'q' is a write queue ptr */
#define RD(q)  (&q[-1])

/** Get pointer to the write queue, assumes 'q' is a read queue ptr */
#define WR(q)  (&q[1])

#if CALL_NOT_IN_CARBON
EXTERN_API_C( mblk_t *)
allocb                          (OTByteCount            size,
                                 OTInt32                pri);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( void , allocbiProc )(char *arg);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( mblk_t *)
allocbi                         (OTByteCount            size,
                                 OTInt32                pri,
                                 allocbiProc            pfv,
                                 char *                 arg,
                                 unsigned char *        base);

EXTERN_API_C( queue_t *)
allocq                          (void);

EXTERN_API_C( OTInt32 )
adjmsg                          (mblk_t *               mp,
                                 OTInt32                len_param);

EXTERN_API_C( queue_t *)
backq                           (queue_t *              q);

EXTERN_API_C( OTInt32 )
bcanput                         (queue_t *              q,
                                 OTUInt8Param           pri);

EXTERN_API_C( OTInt32 )
bcanputnext                     (queue_t *              q,
                                 OTUInt8Param           pri);

EXTERN_API_C( OTInt32 )
bufcall                         (OTByteCount            memNeeded,
                                 OTInt32                pri,
                                 bufcallp_t             proc,
                                 long                   context);

EXTERN_API_C( OTInt32 )
canput                          (queue_t *              q);

EXTERN_API_C( OTInt32 )
canputnext                      (queue_t *              q);

EXTERN_API_C( mblk_t *)
copyb                           (mblk_t *               mp);

EXTERN_API_C( mblk_t *)
copymsg                         (mblk_t *               mp);

EXTERN_API_C( mblk_t *)
dupb                            (mblk_t *               mp);

EXTERN_API_C( mblk_t *)
dupmsg                          (mblk_t *               mp);

EXTERN_API_C( mblk_t *)
esballoc                        (unsigned char *        base,
                                 OTByteCount            size,
                                 OTInt32                pri,
                                 frtn_t *               freeInfo);

EXTERN_API_C( mblk_t *)
esballoca                       (unsigned char *        base,
                                 OTByteCount            size,
                                 OTInt32                pri,
                                 frtn_t *               freeInfo);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( void , esbbcallProc )(long arg);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OTInt32 )
esbbcall                        (OTInt32                pri,
                                 esbbcallProc           func,
                                 long                   arg);

EXTERN_API_C( void )
flushband                       (queue_t *              q,
                                 OTUInt8Param           pri,
                                 OTInt32                flag);

EXTERN_API_C( void )
flushq                          (queue_t *              q,
                                 OTInt32                flag);

EXTERN_API_C( void )
freeb                           (mblk_t *               mp);

EXTERN_API_C( void )
freemsg                         (mblk_t *               mp);

EXTERN_API_C( OTInt32 )
freeq                           (queue_t *              q);

EXTERN_API_C( OTInt32 )
freezestr                       (queue_t *              q);

EXTERN_API_C( admin_t )
getadmin                        (OTUInt16Param          mid);

EXTERN_API_C( unsigned short )
getmid                          (char *                 name);

EXTERN_API_C( mblk_t *)
getq                            (queue_t *              q);

EXTERN_API_C( OTInt32 )
insq                            (queue_t *              q,
                                 mblk_t *               emp,
                                 mblk_t *               nmp);

EXTERN_API_C( void )
linkb                           (mblk_t *               mp1,
                                 mblk_t *               mp2);

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( void , mpnotifyProc )(char *arg);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OTInt32 )
mpnotify                        (mblk_t *               mp,
                                 mpnotifyProc           pfv,
                                 char *                 arg);

EXTERN_API_C( OTInt32 )
msgdsize                        (const mblk_t *         mp);

EXTERN_API_C( mblk_t *)
msgpullup                       (mblk_t *               mp,
                                 OTInt32                len);

EXTERN_API_C( OTInt32 )
pullupmsg                       (mblk_t *               mp,
                                 OTInt32                len);

EXTERN_API_C( OTInt32 )
put                             (queue_t *              q,
                                 mblk_t *               mp);

EXTERN_API_C( OTInt32 )
putbq                           (queue_t *              q,
                                 mblk_t *               mp);

EXTERN_API_C( OTInt32 )
putctl                          (queue_t *              q,
                                 OTInt32                mType);

EXTERN_API_C( OTInt32 )
putnextctl                      (queue_t *              q,
                                 OTInt32                mType);

EXTERN_API_C( OTInt32 )
putctl1                         (queue_t *              q,
                                 OTInt32                mType,
                                 OTInt32                c);

EXTERN_API_C( OTInt32 )
putnextctl1                     (queue_t *              q,
                                 OTInt32                mType,
                                 OTInt32                c);

EXTERN_API_C( OTInt32 )
putctl2                         (queue_t *              q,
                                 OTInt32                mType,
                                 OTInt32                c1,
                                 OTInt32                c2);

#endif  /* CALL_NOT_IN_CARBON */

#ifndef puthere
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OTInt32 )
puthere                         (queue_t *              q,
                                 mblk_t *               mp);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !defined(puthere) */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( OTInt32 )
putnext                         (queue_t *              q,
                                 mblk_t *               mp);

EXTERN_API_C( OTInt32 )
putq                            (queue_t *              q,
                                 mblk_t *               mp);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
qenable                         (queue_t *              q);

EXTERN_API_C( void )
qprocson                        (queue_t *              q);

EXTERN_API_C( void )
qprocsoff                       (queue_t *              q);

EXTERN_API_C( OTInt32 )
qreply                          (queue_t *              q,
                                 mblk_t *               mp);

EXTERN_API_C( OTInt32 )
qsize                           (queue_t *              q);

EXTERN_API_C( mblk_t *)
rmvb                            (mblk_t *               mp,
                                 mblk_t *               bp);

EXTERN_API_C( void )
rmvq                            (queue_t *              q,
                                 mblk_t *               mp);

/* prototype for strlog in "strlog.h" section, below */
EXTERN_API_C( OTInt32 )
strqget                         (queue_t *              q,
                                 qfields_t              what,
                                 OTUInt8Param           pri,
                                 long *                 valp);

EXTERN_API_C( OTInt32 )
strqset                         (queue_t *              q,
                                 qfields_t              what,
                                 OTUInt8Param           pri,
                                 long                   val);

EXTERN_API_C( OTInt32 )
testb                           (OTByteCount            size,
                                 OTInt32                pri);

EXTERN_API_C( void )
unbufcall                       (OTInt32                id);

EXTERN_API_C( void )
unfreezestr                     (queue_t *              q,
                                 OTInt32                oldpri);

EXTERN_API_C( mblk_t *)
unlinkb                         (mblk_t *               mp);

/* ***** From the Mentat "strlog.h" ******/


EXTERN_API_C( OTInt32 )
strlog                          (OTInt32                mid,
                                 OTInt32                sid,
                                 OTInt32                level,
                                 OTUInt32               flags,
                                 char *                 fmt,
                                 ...);

/* ***** Printing Functions ******/

#endif  /* CALL_NOT_IN_CARBON */

enum {
    kOTPrintOnly                = 0,
    kOTPrintThenStop            = 1
};

#if CALL_NOT_IN_CARBON
EXTERN_API_C( OTInt32 )
OTKernelPrintf                  (OTInt32                toDo,
                                 char *                 fmt,
                                 ...);

#endif  /* CALL_NOT_IN_CARBON */

enum {
    CE_CONT                     = 0,                            /* Does kOTPrintOnly   */
    CE_NOTE                     = 0,                            /* Just print */
    CE_WARN                     = 1,                            /* Does kOTPrintThenStop  */
    CE_PANIC                    = 2                             /* Does System Error 107  */
};

#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
cmn_err                         (OTInt32                errType,
                                 char *                 fmt,
                                 ...);

EXTERN_API_C( OTInt32 )
mi_sprintf                      (char *                 buf,
                                 char *                 fmt,
                                 ...);

/* Create sprintf and printf functions that will work in STREAM modules.*/


#define sprintf                        mi_sprintf
#define printf                       OTKernelPrintf

/* ***** FIIK ******/

/* ooo useful header comment please ooo*/

/*
   Function to convert the "long" value that comes back in some of the
   netbufs as a result code to the equivalent OSStatus
*/

#endif  /* CALL_NOT_IN_CARBON */

typedef long                            OTError;

#define GetEError(v)           ((OTUnixErr)(((v) >> 16) & 0xffff))
#define GetXTIError(v)          ((OTXTIErr)((v) & 0xffff))
#define MakeTPIEError(e)     ((OTError)(((((UInt16)(e)) << 16) | TSYSERR)))
#define MakeDLPIEError(e)        ((OTError)(((((UInt16)(e)) << 16) | DL_SYSERR)))
#define MakeXTIError(xti)      ((OTError)(xti))
#define MakeOTError(xti, e)        ((OTError)((xti) | ((UInt16)(e)) << 16))

#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
OTErrorToOSStatus               (OTError                err);

/* ***** STREAMS Plug-in Interface *****/

/*
   Synchronization level codes.  These are supplied to modsw_install and
   stored in the appropriate tables.  sth_osr_open and
   sth_ipush use these to set up synch queue subordination for new devices
   and modules.
*/

#endif  /* CALL_NOT_IN_CARBON */

enum {
    SQLVL_QUEUE                 = 1,
    SQLVL_QUEUEPAIR             = 2,
    SQLVL_MODULE                = 3,
    SQLVL_GLOBAL                = 4,
    SQLVL_DEFAULT               = 3
};

/* The install_info structure.*/


struct install_info {
    streamtab *                     install_str;                /* Streamtab pointer.     */
    UInt32                          install_flags;
    UInt32                          install_sqlvl;              /* Synchronization level. */
    char *                          install_buddy;              /* Shared writer list buddy */
    long                            ref_load;                   /* Set to 0             */
    UInt32                          ref_count;                  /* set to 0             */
};
typedef struct install_info             install_info;
/*  Flags used in the install_flags field*/
enum {
    kOTModIsDriver              = 0x00000001,
    kOTModIsModule              = 0x00000002,
    kOTModNoWriter              = 0x00000010,
    kOTModUpperIsTPI            = 0x00001000,
    kOTModUpperIsDLPI           = 0x00002000,
    kOTModLowerIsTPI            = 0x00004000,
    kOTModLowerIsDLPI           = 0x00008000,
    kOTModGlobalContext         = 0x00800000,                   /* This flag says you don't want per-context globals*/
    kOTModUsesInterrupts        = 0x08000000,                   /* This flag is only valid if kOTModIsDriver is set and the driver is a PCI-card driver using the Name Registry*/
    kOTModIsComplexDriver       = 0x20000000,                   /* This flag is only valid if kOTModIsDriver is set.*/
    kOTModIsFilter              = 0x40000000                    /* This flag is only valid if kOTModIsModule is set.*/
};

/* Typedef for the GetOTInstallInfo function*/

/*
   Your module must export this function, and return a pointer to the
   install_info structure for the module.
*/

typedef CALLBACK_API_C( install_info *, GetOTInstallInfoProcPtr )(void );
/* Typedef for the InitStreamModule function*/
/*
   Your module can optionally export this function.  It will be called 
   whenever your module is about to be loaded into a stream for the
   first time, or if it is about to be reloaded after having been 
   unloaded. Return false if your module should NOT be loaded.
   For STREAMS modules, the void* parameter will be NULL.  For drivers, it
   will be the same cookie parameter that was used for registering the module.
   For PCI card drivers, this will be a pointer to the OTPCIInfo structure,
   which can also be interpreted as a RegEntryIDPtr.
*/

typedef CALLBACK_API_C( Boolean , InitStreamModuleProcPtr )(void *portInfo);
/* Typedef for the TerminateStreamModule function*/
/*
   Your module can optionally export this function.  It will be called 
   whenever your module has been closed for the last time (i.e. no other 
   outstanding instances of the module exist).
*/

typedef CALLBACK_API_C( void , TerminateStreamModuleProcPtr )(void *portInfo);
/* Equates for shared library prefixes*/
    
   /*
  * Interface ID for STREAMS Modules for ASLM.
  */
#define kOTModuleInterfaceID     kOTModulePrefix "StrmMod"


/* ***** Majors and Minors ******/

/* Functions for dealing with major and minor device numbers*/


typedef UInt16                          major_t;
typedef UInt16                          minor_t;

/* major part of a device */
#define getmajor(x)       ((major_t)(((unsigned long)(x)>>16)&0xffff))

/* minor part of a device */
#define getminor(x)      ((minor_t)((x)&0xffff))

/* make a device number */
#define makedev(x,y)        ((dev_t)((((dev_t)(x))<<16) | ((y) & 0xffff)))

#define getemajor           getmajor
#define geteminor          getminor
#define makedevice         makedev

#define    etoimajor(majnum)   (majnum)
#define    itoemajor(majnum,j) (majnum)

/*
   This is the first minor number that Apple OT drivers use for CLONEOPENs.
   Minor numbers 0 through 9 are reserved for use by the modules
   for various control streams.  Note that Mentat drivers, which
   use mi_open_comm, start minor numbers from 5.
*/
enum {
    kFirstMinorNumber           = 10
};

/* ***** Logging Macros ******/

#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
OTStrlog                        (queue_t *              q,
                                 OTInt32                lvl,
                                 OTInt32                flags,
                                 const char *           str);

/* These are enums for the level value*/

#endif  /* CALL_NOT_IN_CARBON */

enum {
    kOTLvlFatal                 = 0,
    kOTLvlNonfatal              = 1,
    kOTLvlExtFatal              = 2,
    kOTLvlExtNonfatal           = 3,
    kOTLvlUserErr               = 4,
    kOTLvlInfoErr               = 5,
    kOTLvlInfoOnly              = 6
};



#define STRLOG(q, lvl, flags, str)     OTStrlog(q, lvl, flags, str)

#if OTDEBUG
  #define STRLOG1(q, lvl, flags, str) OTStrlog(q, lvl, flags, str)
#else
 #define STRLOG1(q, lvl, flags, str)
#endif

#if OTDEBUG > 1 || OTDEBUG > 1
 #define STRLOG2(q, lvl, flags, str) OTStrlog(q, lvl, flags, str)
#else
 #define STRLOG2(q, lvl, flags, str)
#endif

/* ***** TPI Additions ******/

/*
   Extra OTCommand codes that may appear on your module queue.
   These are extensions to the TPI specification for Open Transport.
   T_PRIVATE_REQ is the first available TPI message number for private
   use by modules (assuming you don't want to be confused by standard
   TPI messages).
*/



enum {
    T_TIMER_REQ     = 80,   /* Timer event                          */
 T_MIB_REQ       = 81,   /* Request module's MIB                 */
 T_MIB_ACK       = 82,   /* The module's MIB is available        */
 
   T_PRIVATE_REQ   = 90    /* The first private request available  */
};


struct T_MIB_req {
    long                            PRIM_type;                  /* Always T_MIB_REQ */
};
typedef struct T_MIB_req                T_MIB_req;

struct T_MIB_ack {
    long                            PRIM_type;                  /* Always T_MIB_ACK   */
    long                            MIB_length;                 /* MIB length          */
    long                            MIB_offset;                 /* MIB Offset         */
};
typedef struct T_MIB_ack                T_MIB_ack;

struct T_stream_timer {
    long                            PRIM_type;                  /* Always T_TIMER_REQ */
    union {
        long                            USER_long;
        void *                          USER_ptr;
    }                                 USER_data;
};
typedef struct T_stream_timer           T_stream_timer;
/* ***** Kernel Port Stuff ******/
/*
   Kernel port record, which is a direct analogue of
   the OTPortRecord in "OpenTransport.h".  Note that
   when working with TPortRecord's, you're always
   working with OT's one true copy of the record,
   whereas when working with OTPortRecord's, you're
   always working with a copy.
*/


struct TPortRecord {
    OTLink                          fLink;
    char *                          fPortName;
    char *                          fModuleName;
    char *                          fResourceInfo;
    char *                          fSlotID;
    struct TPortRecord *            fAlias;
    ItemCount                       fNumChildren;
    OTPortRef *                     fChildPorts;
    UInt32                          fPortFlags;
    UInt32                          fInfoFlags;
    UInt32                          fCapabilities;
    OTPortRef                       fRef;
    streamtab *                     fStreamtab;
    void *                          fContext;
    void *                          fExtra;
};
typedef struct TPortRecord              TPortRecord;
/* Port utilities*/
/*
   These routines can be used by kernel code to register, find and iterate
   through the various ports on the machine.  Do not confuse these with
   the client-side routines, defined in "OpenTransport.h".
*/

/*
   Register a port. The name the port was registered under is returned in
   the fPortName field.  This routine allocates a TPortRecord and
   copies the supplied OTPortRecord into it.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
OTRegisterPort                  (OTPortRecord *         portInfo,
                                 void *                 ref);

/*
   Unregister the port with the given name (If you re-register the
   port, it may get a different name - use OTChangePortState if
   that is not desireable).  Since a single OTPortRef can be registered
   with several names, the API needs to use the portName rather than
   the OTPortRef to disambiguate.
*/
EXTERN_API_C( OSStatus )
OTUnregisterPort                (const char *           portName,
                                 void **                refPtr);

/* Change the state of the port.*/
EXTERN_API_C( OSStatus )
OTChangePortState               (OTPortRef              portRef,
                                 OTEventCode            theChange,
                                 OTResult               why);

/* Find the TPortRecord for a given Port Name*/
EXTERN_API_C( TPortRecord *)
OTFindPort                      (const char *           portName);

/* Find the "nth" TPortRecord*/
EXTERN_API_C( TPortRecord *)
OTGetIndexedPort                (OTItemCount            index);

/*
   Find another port that is active and conflicts with
   the port described by "ref"
*/
EXTERN_API_C( TPortRecord *)
OTFindPortConflict              (OTPortRef              ref);

/* Other ways of finding the port*/
EXTERN_API_C( TPortRecord *)
OTFindPortByRef                 (OTPortRef              ref);

EXTERN_API_C( TPortRecord *)
OTFindPortByDev                 (dev_t                  dev);

/* ***** Port Scanners ******/

/* Shared library definitions*/

/*
   Prefix for Open Transport port scanners.
   Your port-scanning ASLM function set must use the prefix kOTPortScannerPrefix.
*/

#define kOTPortScannerPrefix        "ot:pScnr$"
/*
   The kOTPortScannerInterfaceID define is what you need to add to your
   export file for the "interfaceID = " clause.  
*/


#define kOTPortScannerInterfaceID          kOTKernelPrefix "pScnr"
#define kOTPseudoPortScannerInterfaceID     kOTKernelPrefix "ppScnr"
#define kOTCompatScannerInterfaceID            kOTKernelPrefix "cpScnr,1.0"

#define kOTPortScannerCFMTag              kOTKernelPrefix "pScnr"
#define kOTPseudoPortScannerCFMTag          kOTKernelPrefix "ppScnr"
#define kOTCompatPortScannerCFMTag         kOTKernelPrefix "cpScnr"


/* OTScanPorts entry point.*/

/* Your port-scanning function must be exported by the name "OTScanPorts".*/

#define kOTScanPortsID "OTScanPorts"
/* Selectors for the scanType parameter to PortScanProcPtr.*/

#endif  /* CALL_NOT_IN_CARBON */

enum {
    kOTInitialScan              = 0,
    kOTScanAfterSleep           = 1
};

/* PortScanProcPtr is the typedef for the scanning function.*/

typedef CALLBACK_API_C( void , PortScanProcPtr )(UInt32 scanType);
/*
   Memory allocation for port persistent data, such as the
   memory referenced by the ref parameter you pass to
   OTRegisterPort.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void *)
OTAllocPortMem                  (OTByteCount            size);

EXTERN_API_C( void )
OTFreePortMem                   (void *                 mem);

/* ***** Interrupt Control Functions *****/

#endif  /* CALL_NOT_IN_CARBON */

#if TARGET_CPU_68K
/*
   MPS_INTR_STATE saves the current interrupt state
   Its definition changes with the definition
   of mps_intr_enable/disable, so this typedef is
   also inside the TARGET_CPU_68K conditional.
*/

typedef UInt8                           MPS_INTR_STATE;
/* Disable interrupts and save the state*/
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter mps_intr_disable(__A0)
                                                                                            #endif
EXTERN_API_C( void )
mps_intr_disable                (MPS_INTR_STATE *       oldState)                           SEVENWORDINLINE(0x40C0, 0xE040, 0x007C, 0x0600, 0x027C, 0xFEFF, 0x1080);

/*
   move sr,d0
   asr.w #8,d0
   ori #$600,sr
   andi #$FEFF,sr
   move.b d0,(a0)
   #if OTDEBUG
       andi.b #$70,d0
       cmpi.b #$70,d0
       bne.s @3
       DebugBreak
   #endif
*/

/* Enable interrupts from the saved state*/

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter mps_intr_enable(__A0)
                                                                                            #endif
EXTERN_API_C( void )
mps_intr_enable                 (MPS_INTR_STATE *       oldState)                           THREEWORDINLINE(0x1010, 0xE140, 0x46C0);

/*
   #if OTDEBUG
       move sr,d0
       andi.w #$700,d0
       cmpi.w #$600,d0
       beq.s @2
       DebugBreak
   #endif
   move.b (a0),d0
   #if OTDEBUG
       andi.b #$70,d0
       cmpi.b #$70,d0
       bne.s @3
       DebugBreak
       move.b (a0),d0
   #endif
   asl.w #8,d0
   move d0,sr
*/

#endif  /* CALL_NOT_IN_CARBON */

#else
typedef UInt8                           MPS_INTR_STATE;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
mps_intr_disable                (MPS_INTR_STATE *       oldState);

EXTERN_API_C( void )
mps_intr_enable                 (MPS_INTR_STATE *       oldState);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_CPU_68K */

/* ***** Timer Functions ******/
enum {
    kOTMinimumTimerValue        = 8                             /* 8 milliseconds is the minimum timeout value*/
};

#if CALL_NOT_IN_CARBON
EXTERN_API_C( mblk_t *)
mi_timer_alloc                  (queue_t *              q,
                                 OTByteCount            size);

EXTERN_API_C( void )
mi_timer_free                   (mblk_t *               mp);

EXTERN_API_C( void )
mi_timer                        (mblk_t *               mp,
                                 unsigned long          milliSeconds);

EXTERN_API_C( Boolean )
mi_timer_cancel                 (mblk_t *               mp);

EXTERN_API_C( Boolean )
mi_timer_valid                  (mblk_t *               mp);

EXTERN_API_C( mblk_t *)
mi_timer_q_switch               (mblk_t *               mp,
                                 queue_t *              q,
                                 mblk_t *               new_mp);

/* ***** Driver Deferred Task Extras ******/

/*
   This routine is used by a driver at interrupt time to schedule
   a deferred task to run their interrupt processing code.
*/
EXTERN_API_C( Boolean )
OTScheduleDriverDeferredTask    (long                   dtCookie);

/* ***** Driver Memory Routines ******/

/*
   These routines are different from the
   similarly named routines in "OpenTransport.h" because they allocate
   memory in the OT kernel pool.  See Technote oooo "Understanding
   Open Transport Memory Management" for details.
*/

/*
   This is the typedef for a function that will be called when a message
   created by OTAllocMsg is destroyed.
*/

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( void , EsbFreeProcPtr )(char *arg);
/*
   This function creates a message which points to "size" bytes of data
   at "buf".  When the message is freed, the EsbFreeProcPtr function "func"
   will be called with the argument "arg".
   NOTE: This function allows users of your buffer to modify the buffer.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( mblk_t *)
OTAllocMsg                      (void *                 buf,
                                 OTByteCount            size,
                                 EsbFreeProcPtr         func,
                                 void *                 arg);

/*
   Routines to allocate and free memory in your modules (these are
   interrupt-time safe!).
*/

EXTERN_API_C( void *)
OTAllocMem                      (OTByteCount            size);

EXTERN_API_C( void )
OTFreeMem                       (void *                 mem);

EXTERN_API_C( void *)
OTReallocMem                    (void *                 ptr,
                                 OTByteCount            newSize);

/*
   Also, make sure that calling traditional "C" allocation routines
   will not compile.
*/


#define calloc                     (DONT_CALL*THIS_FUNCTION)
#define malloc                        (DONT_CALL*THIS_FUNCTION)
#define realloc                       (DONT_CALL*THIS_FUNCTION)
#define free                      (DONT_CALL*THIS_FUNCTION)

/* ***** Kernel Memory Utilities ******/

EXTERN_API_C( OTInt32 )
mi_bcmp                         (const char *           first,
                                 const char *           second,
                                 OTByteCount            nBytes);

/* Standard STREAMS memory utilities.*/

    
#define bcopy(s, d, l)         OTMemcpy(d, s, l)
#define bzero(d, l)               OTMemzero(d, (OTByteCount)(l))
#define bcmp(s, d, l)            mi_bcmp(s, d, l)

/*
   Standard STREAMS bcopy, bzero, & bcmp take char* parameters.
   The BCOPY, BZERO, and BCMP routines take void* so that we do not have
   to cast all the pointers.
*/


#define BCOPY(s, d, l)         bcopy((const char*)(s), (char*)(d), l)
#define BZERO(d, l)              bzero((char*)(d), l)
#define BCMP(s, d, l)          mi_bcmp((const char*)(s), (const char*)(d), l)


/*******************************************************************************
** Some helpful utilites from Mentat
********************************************************************************/
/* Routines to calculate various sizes of STREAM messages*/


#define HEAD_SIZE(mp)  ((mp)->b_rptr - (mp)->b_datap->db_base)
#define TAIL_SIZE(mp)   ((mp)->b_datap->db_lim - (mp)->b_wptr)
#define MBLK_SIZE(mp)    ((mp)->b_wptr - (mp)->b_rptr)
#define DBLK_SIZE(mp) ((mp)->b_datap->db_lim - (mp)->b_datap->db_base)

/* Useful macros for STREAMS copy in and out.*/


#define    MI_COPY_IN      1
#define   MI_COPY_OUT     2
#define   MI_COPY_DIRECTION(mp)                            \
 (((UInt8*)&(mp)->b_cont->b_prev)[0])
#define    MI_COPY_COUNT(mp)                    \
 (((UInt8*)&(mp)->b_cont->b_prev)[1])
#define    MI_COPY_RVAL(mp)    (*(OTInt32 *)&(mp)->b_cont->b_next)
#define MI_COPY_CASE(dir,cnt)   ((OTInt32)(((cnt)<<2)|dir))
#define MI_COPY_STATE(mp)                            \
 ((OTInt32)MI_COPY_CASE(MI_COPY_DIRECTION(mp),MI_COPY_COUNT(mp)))

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( void , OTWriterProcPtr )(queue_t *q, mblk_t *mp);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
mps_become_writer               (queue_t *              q,
                                 mblk_t *               mp,
                                 OTWriterProcPtr        proc);

EXTERN_API_C( OTInt32 )
drv_priv                        (cred *                 credp);

EXTERN_API_C( queue_t *)
mi_allocq                       (streamtab *            st);

EXTERN_API_C( mblk_t *)
mi_tpi_ack_alloc                (mblk_t *               mp,
                                 OTByteCount            size,
                                 long                   primType);

EXTERN_API_C( mblk_t *)
mi_tpi_conn_con                 (mblk_t *               trailer_mp,
                                 char *                 src,
                                 OTByteCount            src_length,
                                 char *                 opt,
                                 OTByteCount            opt_length);

EXTERN_API_C( mblk_t *)
mi_tpi_conn_ind                 (mblk_t *               trailer_mp,
                                 char *                 src,
                                 OTByteCount            src_length,
                                 char *                 opt,
                                 OTByteCount            opt_length,
                                 OTInt32                seqnum);

EXTERN_API_C( mblk_t *)
mi_tpi_conn_req                 (mblk_t *               trailer_mp,
                                 char *                 dest,
                                 OTByteCount            dest_length,
                                 char *                 opt,
                                 OTByteCount            opt_length);

EXTERN_API_C( mblk_t *)
mi_tpi_data_ind                 (mblk_t *               trailer_mp,
                                 OTInt32                flags,
                                 long                   ptype);

EXTERN_API_C( mblk_t *)
mi_tpi_data_req                 (mblk_t *               trailer_mp,
                                 OTInt32                flags,
                                 long                   pttype);

EXTERN_API_C( mblk_t *)
mi_tpi_discon_ind               (mblk_t *               trailer_mp,
                                 OTInt32                reason,
                                 OTInt32                seqnum);

EXTERN_API_C( mblk_t *)
mi_tpi_discon_req               (mblk_t *               trailer_mp,
                                 OTInt32                seqnum);

EXTERN_API_C( mblk_t *)
mi_tpi_err_ack_alloc            (mblk_t *               mp,
                                 OTInt32                tlierr,
                                 OTInt32                unixerr);

EXTERN_API_C( mblk_t *)
mi_tpi_exdata_ind               (mblk_t *               trailer_mp,
                                 OTInt32                flags,
                                 long                   ptype);

EXTERN_API_C( mblk_t *)
mi_tpi_exdata_req               (mblk_t *               trailer_mp,
                                 OTInt32                flags,
                                 long                   ptype);

EXTERN_API_C( mblk_t *)
mi_tpi_info_req                 (void);

EXTERN_API_C( mblk_t *)
mi_tpi_ok_ack_alloc             (mblk_t *               mp);

EXTERN_API_C( mblk_t *)
mi_tpi_ordrel_ind               (void);

EXTERN_API_C( mblk_t *)
mi_tpi_ordrel_req               (void);

EXTERN_API_C( mblk_t *)
mi_tpi_uderror_ind              (char *                 dest,
                                 OTByteCount            dest_length,
                                 char *                 opt,
                                 OTByteCount            opt_length,
                                 OTInt32                error);

EXTERN_API_C( mblk_t *)
mi_tpi_unitdata_ind             (mblk_t *               trailer_mp,
                                 char *                 src,
                                 OTByteCount            src_length,
                                 char *                 opt,
                                 OTByteCount            opt_length);

EXTERN_API_C( mblk_t *)
mi_tpi_unitdata_req             (mblk_t *               trailer_mp,
                                 char *                 dst,
                                 OTByteCount            dst_length,
                                 char *                 opt,
                                 OTByteCount            opt_length);

EXTERN_API_C( mblk_t *)
mi_reuse_proto                  (mblk_t *               toReuse,
                                 OTByteCount            sizeDesired,
                                 OTBooleanParam         keepOnError);

EXTERN_API_C( mblk_t *)
mi_reallocb                     (mblk_t *               old_mp,
                                 OTByteCount            new_size);

EXTERN_API_C( Boolean )
mi_set_sth_hiwat                (queue_t *              q,
                                 OTByteCount            size);

EXTERN_API_C( Boolean )
mi_set_sth_lowat                (queue_t *              q,
                                 OTByteCount            size);

EXTERN_API_C( Boolean )
mi_set_sth_maxblk               (queue_t *              q,
                                 OTByteCount            size);

EXTERN_API_C( Boolean )
mi_set_sth_wroff                (queue_t *              q,
                                 OTByteCount            size);

EXTERN_API_C( UInt8 *)
mi_offset_param                 (mblk_t *               mp,
                                 long                   offset,
                                 long                   len);

EXTERN_API_C( UInt8 *)
mi_offset_paramc                (mblk_t *               mp,
                                 long                   offset,
                                 long                   len);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( char *)
mi_open_detached                (char **                mi_opp_orig,
                                 OTByteCount            size,
                                 dev_t *                devp);

EXTERN_API_C( OTInt32 )
mi_open_comm                    (char **                mi_opp_orig,
                                 OTByteCount            size,
                                 queue_t *              q,
                                 dev_t *                dev,
                                 OTInt32                flag,
                                 OTInt32                sflag,
                                 cred_t *               credp);

EXTERN_API_C( OTInt32 )
mi_close_comm                   (char **                mi_opp_orig,
                                 queue_t *              q);

EXTERN_API_C( void )
mi_bufcall                      (queue_t *              q,
                                 OTByteCount            size,
                                 OTInt32                pri);

EXTERN_API_C( void )
mi_detach                       (queue_t *              q,
                                 char *                 ptr);

EXTERN_API_C( void )
mi_close_detached               (char **                mi_opp_orig,
                                 char *                 ptr);

EXTERN_API_C( char *)
mi_next_ptr                     (char *                 ptr);

EXTERN_API_C( void )
mi_copyin                       (queue_t *              q,
                                 mblk_t *               mp,
                                 char *                 uaddr,
                                 OTByteCount            len);

EXTERN_API_C( void )
mi_copyout                      (queue_t *              q,
                                 mblk_t *               mp);

EXTERN_API_C( mblk_t *)
mi_copyout_alloc                (queue_t *              q,
                                 mblk_t *               mp,
                                 char *                 uaddr,
                                 OTByteCount            len);

EXTERN_API_C( void )
mi_copy_done                    (queue_t *              q,
                                 mblk_t *               mp,
                                 OTInt32                err);

EXTERN_API_C( void )
mi_copy_set_rval                (mblk_t *               mp,
                                 OTInt32                rval);

EXTERN_API_C( OTInt32 )
mi_copy_state                   (queue_t *              q,
                                 mblk_t *               mp,
                                 mblk_t **              mpp);

/* ***** PCI-Specific Stuff*/

/* This is the cookie that is passed to your STREAM Module.*/

#endif  /* CALL_NOT_IN_CARBON */


struct OTPCIInfo {
    RegEntryID                      fTheID;
    void *                          fConfigurationInfo;
    ByteCount                       fConfigurationLength;
};
typedef struct OTPCIInfo                OTPCIInfo;
/* Driver Description*/
/*
   All PCI card drivers should have the following bits set in their
   install_info structure:
   
   o kOTModIsDriver.
   
   They should NEVER have the kOTModPushAlways or the kOTModIsModule
   flags set.
   The kOTModIsLowerMux bits should be set if the driver is a lower
   multiplexor, although Open Transport does nothing with the information
   today.
   The kOTModUpperIsDLPI bit should be set if the driver uses the DLPI message
   specification.  The kOTModUpperIsTPI bit should be set if the driver uses
   the TPI message specification.
*/

/*
   Macro to put together the driverServices.service[x].serviceType field:
   xxxxxddd dddddddd ffffffff xxxxxxTD
   where "d" is the device type for Open Transport,
   the lower two bits are whether the driver is TPI or DLPI,
   and the "f" bits are the framing option flags.
   and all other bits should be 0
*/


#define OTPCIServiceType(devType, framingFlags, isTPI, isDLPI) \
  ((devType << 16) | (((framingFlags) & 0xff) << 8) | (isTPI ? 2 : 0) | (isDLPI ? 1 : 0 ))

/*
   Typedef for the ValidateHardware function.  This function will be
   called only once, at system boot time, before installing your driver
   into the Open Transport module registry.
   The param pointer will is a OTPCIInfo pointer - don't be changing the
   values there!
*/

typedef CALLBACK_API_C( OTResult , ValidateHardwareProcPtr )(OTPCIInfo *param);
/*
   Your driver can return this value if it loaded correctly
   but wants to stay resident, presumably because it's hooked
   itself irrevokably into some other system service.
*/
enum {
    kOTPCINoErrorStayLoaded     = 1
};

/*
   Some descriptors we use - these should eventually show up
   in system header files somewhere.
*/


#define kDescriptorProperty        "driver-descriptor"
#define kDriverProperty         "driver,AAPL,MacOS,PowerPC"
#define kDriverPtrProperty      "driver-ptr"
#define kSlotProperty          "AAPL,slot-name"

/*
   Maximum # of services support by Open Transport.  If your module
   exports more than this # of services, Open Transport will not be
   able to use the module.
*/


enum {
    kMaxServices                = 20
};

/*******************************************************************************
** Everything below here is C++ ONLY
********************************************************************************/

#ifdef __cplusplus
}                   // Terminate C definitions

/*  -------------------------------------------------------------------------
    TStreamQueue class
    
    This class is just a convenient interface to the queue structure
    ------------------------------------------------------------------------- */

    class TStreamQueue : public queue
    {
        public:
            void        EnableQueue();
    };
    
    /*  -------------------------------------------------------------------------
        Inline methods for TStreamQueue
        ------------------------------------------------------------------------- */
    
        inline void TStreamQueue::EnableQueue()
        {
            if ( q_flag & QNOENB )
            {
                enableok(this);
                if ( q_first )
                    qenable(this);
            }
        }

/*  -------------------------------------------------------------------------
    Class TStreamMessage
    
    This class is a C++ interface to the mblk_t structure defined in STREAMS
    ------------------------------------------------------------------------- */
    //
    // The maximum stream buffer size is the largest OTByteCount that
    // doesn't look negative if interpreted as a signed number
    //
    enum
    {
        kMaxStreamBufferSize    = (((OTByteCount)-1L) >> 1)
    };

    class TStreamMessage : public msgb
    {
        private:
                void*       operator new (OTByteCount)  { return NULL; }
                
        public:
                void*       operator new(OTByteCount, OTByteCount size)
                    { return (TStreamMessage*)allocb(size, 0); }
                void*       operator new(OTByteCount, void* buf, OTByteCount size,
                                         EsbFreeProcPtr func, void* arg)
                    { return OTAllocMsg(buf, size, func, arg); }
                void        operator delete(void* ptr)
                    { if ( ptr != NULL ) freemsg((mblk_t*)ptr); }
    
                    void            Reset(OTByteCount = 0);
                    void            ResetWithLeader(OTByteCount);
            
                    void            FreeData();
                    TStreamMessage* RemoveData();
                    void            AppendData(TStreamMessage* data);
            
                    OTByteCount GetSize() const;
                    OTByteCount GetDataSize() const;
                    OTByteCount GetMessageDataSize() const;
                    void                SetDataSize(OTByteCount);
        
                    void            SetType(UInt8);
                    UInt8           GetType() const;
                    TStreamMessage* GetNextBlock() const;
                    TStreamMessage* ReuseMessage(OTByteCount newSize, OTBooleanParam keepOnFailure);
                    
                    void            SetNextBlock(TStreamMessage*);
                    char*           GetDataPointer() const;
                    Boolean         IsReuseable(OTByteCount size) const;
            
                    void            HideBytesAtFront(OTByteCount);
                    void            HideBytesAtEnd(OTByteCount);
            //
            // The following functions ignore non-M_DATA blocks.
            //
                    const TStreamMessage*
                                    _MDECL GetBlockAt(OTByteCount& offset) const;
                    const void*     _MDECL GetPointerTo(OTByteCount offset, OTByteCount* len) const;
                    const void*     _MDECL GetPointerTo(OTByteCount offset, OTByteCount* len, void* bfr) const;
            //
            // This function makes a non-shared copy of any initial non-M_DATA block, and
            // then makes shared copies of the requested data.  It only assumes 1 leading
            // non-M_DATA block, but will skip any other non-M_DATA blocks while 
            // searching for data to copy.
            //
                    TStreamMessage* _MDECL MakeSharedCopy(OTByteCount offset = 0, 
                                                          OTByteCount len = kMaxStreamBufferSize);

                    Boolean         _MDECL HasData() const;
                    Boolean         _MDECL WriteData(void* buf, OTByteCount len);
            //
            // These hide/remove the part of the data already read.  
            //
                    TStreamMessage* _MDECL ReadData(void* buf, OTByteCount* len);
                    OTByteCount     _MDECL ReadControl(void* buf, OTByteCount len);
    };
    
    /*  -------------------------------------------------------------------------
        Inline methods for TStreamMessage
        ------------------------------------------------------------------------- */
    
        inline void TStreamMessage::Reset(OTByteCount size)
        {
            b_rptr = b_datap->db_base;
            b_wptr = b_rptr + size;
        }
        
        inline OTByteCount OTLengthWithLeader(OTByteCount size)
        {
            return (size + 3) & ~3;
        }
        
        inline void TStreamMessage::ResetWithLeader(OTByteCount size)
        {
            b_rptr = b_datap->db_lim - OTLengthWithLeader(size);
            b_wptr = b_rptr + size;
        }
        
        inline void TStreamMessage::FreeData()
        {
            if ( b_cont )
            {
                freemsg(b_cont);
                b_cont = NULL;
            }
        }
        
        inline TStreamMessage* TStreamMessage::RemoveData()
        {
            TStreamMessage* temp = (TStreamMessage*)b_cont;
            b_cont = NULL;
            return temp;
        }
        
        inline void TStreamMessage::AppendData(TStreamMessage* mp)
        {
            linkb(this, mp);
        }
    
        inline OTByteCount TStreamMessage::GetSize() const
        {
            return b_datap->db_lim - b_datap->db_base;
        }
        
        inline OTByteCount TStreamMessage::GetDataSize() const
        {
            return b_wptr - b_rptr;
        }
        
        inline OTByteCount TStreamMessage::GetMessageDataSize() const
        {
            return (b_cont == NULL) ? 
                (b_datap->db_type == M_DATA ? (b_wptr - b_rptr) : 0) : msgdsize(this);
        }
    
        inline void TStreamMessage::SetDataSize(OTByteCount size)
        {
            b_wptr = b_rptr + size;
        }
    
        inline void TStreamMessage::SetType(UInt8 type)
        {
            b_datap->db_type = type;
        }
    
        inline unsigned char TStreamMessage::GetType() const
        {
            return b_datap->db_type;
        }
        
        inline TStreamMessage* TStreamMessage::GetNextBlock() const
        {
            return (TStreamMessage*)b_cont;
        }
        
        inline void TStreamMessage::SetNextBlock(TStreamMessage* mp)
        {
            b_cont = (mblk_t*)mp;
        }
        
        inline char* TStreamMessage::GetDataPointer() const
        {
            return (char*)b_rptr;
        }
        
        inline Boolean TStreamMessage::IsReuseable(OTByteCount size) const
        {
            return (b_datap->db_ref == 1 && GetSize() >= size);
        }
        
        inline void TStreamMessage::HideBytesAtFront(OTByteCount len)
        {
            adjmsg(this, len);
        }
        
        inline void TStreamMessage::HideBytesAtEnd(OTByteCount len)
        {
            adjmsg(this, -len);
        }

        inline TStreamMessage* TStreamMessage::ReuseMessage(OTByteCount newSize, OTBooleanParam keepOnFailure)
        {
            return (TStreamMessage*)mi_reuse_proto(this, newSize, keepOnFailure);
        }
        
/*  -------------------------------------------------------------------------
    TTimerMessage class
    
    This class implements an interface to the STREAM environment timer
    facilities.  A TTimerMessage will be placed on the queue of your choice
    when it's timer expires.
    ------------------------------------------------------------------------- */

#if TARGET_CPU_PPC
    #define TIMER_BUG   1
#else
    #define TIMER_BUG   0
#endif

    class TTimerMessage : public TStreamMessage
    {
    #if TIMER_BUG
        private:
    #else
        public:
    #endif
            inline void* operator new(OTByteCount, queue_t* q)
                {   return mi_timer_alloc(q, sizeof(T_stream_timer)); }
            inline void* operator new(OTByteCount, queue_t* q, OTByteCount extra)
                {   return mi_timer_alloc(q, extra + sizeof(T_stream_timer)); }
    
            inline void operator delete(void* ptr)
                {   if ( ptr != NULL ) mi_timer_free((mblk_t*)ptr); }
    
        public:     
                    Boolean         IsValid();
                    TTimerMessage*  ChangeQueue(TStreamQueue* newQ, TTimerMessage* newMP);
                    Boolean         Cancel();
                    void            Schedule(OTTimeout time);
    
        private:
            inline void* operator new(OTByteCount)
                {   return 0; }
    };
    
    inline TTimerMessage* NewTimerMsg(queue_t* q, OTByteCount extra = 0)
    {
        return (TTimerMessage*)mi_timer_alloc(q, sizeof(T_stream_timer) + extra);
    }
    
    inline void FreeTimerMsg(TTimerMessage* msg)
    {
        mi_timer_free(msg);
    }
    
    /*  -------------------------------------------------------------------------
        Inline methods for TStreamTimer
        ------------------------------------------------------------------------- */
    
        inline Boolean TTimerMessage::IsValid()
        {
            return mi_timer_valid(this);
        }
        
        inline TTimerMessage* TTimerMessage::ChangeQueue(TStreamQueue* q, TTimerMessage* newMP)
        {
            return( (TTimerMessage*) mi_timer_q_switch(this, q, newMP));
        }
    
        inline Boolean TTimerMessage::Cancel()
        {
            return mi_timer_cancel(this);
        }
        
        inline void TTimerMessage::Schedule(OTTimeout time)
        {
            ((T_stream_timer*)b_rptr)->PRIM_type = T_TIMER_REQ;
            mi_timer(this, (unsigned long)time);
        }

extern "C" {        // resume C definitions
#endif  /* __cplusplus  */


#if defined(__MWERKS__) && TARGET_CPU_68K
 #pragma pop
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

#endif /* __OPENTRANSPORTKERNEL__ */

