/*
     File:       OpenTransportProtocol.h
 
     Contains:   Definitions likely to be used by low-level protocol stack implementation.
 
     Version:    Technology: 2.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1993-2001 by Apple Computer, Inc. and Mentat Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OPENTRANSPORTPROTOCOL__
#define __OPENTRANSPORTPROTOCOL__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif


#if CALL_NOT_IN_CARBON
#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __CODEFRAGMENTS__
#include <CodeFragments.h>
#endif

#endif  /* CALL_NOT_IN_CARBON */

#ifndef __OPENTRANSPORT__
#include <OpenTransport.h>
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

/* ***** Setup Default Compiler Variables ******/

/*
   OTKERNEL is used to indicate whether the code is being built
   for the kernel environment.  It defaults to 0.  If you include
   "OpenTransportKernel.h" before including this file,
   it will be 1 and you will only be able to see stuff available
   to kernel code.
   As we've included "OpenTransport.h" and it defaults this variable
   to 0 if it's not already been defined, it should always be defined
   by the time we get here.  So we just assert that.  Assertions in
   header files!  Cool (-:
*/

#ifndef OTKERNEL
#error OpenTransportProtocol.h expects OpenTransport.h to set up OTKERNEL.
#endif  /* !defined(OTKERNEL) */

/* ***** Shared Client Memory ******/
#if !OTKERNEL
/*
   These allocators allocate memory in the shared client pool,
   which is shared between all clients and is not disposed when
   a particular client goes away.  See DTS Technote ooo
   "Understanding Open Transport Memory Management" for details.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void *)
OTAllocSharedClientMem          (OTByteCount            size);

EXTERN_API_C( void )
OTFreeSharedClientMem           (void *                 mem);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !OTKERNEL */

/* ***** UNIX Types ******/
#if CALL_NOT_IN_CARBON
/*
   On UNIX, uid_t and gid_t are defined to be big enough
   to hold a user ID and group ID respectively.  As Mac OS
   has no such concepts, we just define them as UInt32 place
   holders.
*/
typedef UInt32                          uid_t;
typedef UInt32                          gid_t;
/* Similarly, dev_t is a UNIX type for denoting a device number.*/
typedef UInt32                          dev_t;
/* ***** From the Mentat "strstat.h" ******/

/* module statistics structure */

struct module_stat {
    long                            ms_pcnt;                    /* count of calls to put proc */
    long                            ms_scnt;                    /* count of calls to service proc */
    long                            ms_ocnt;                    /* count of calls to open proc */
    long                            ms_ccnt;                    /* count of calls to close proc */
    long                            ms_acnt;                    /* count of calls to admin proc */
    char *                          ms_xptr;                    /* pointer to private statistics */
    short                           ms_xsize;                   /* length of private statistics buffer */
};
typedef struct module_stat              module_stat;
/* ***** From the Mentat "cred.h" ******/


struct cred {
    UInt16                          cr_ref;                     /* reference count on processes using cred structures */
    UInt16                          cr_ngroups;                 /* number of groups in cr_groups */
    uid_t                           cr_uid;                     /* effective user id */
    gid_t                           cr_gid;                     /* effective group id */
    uid_t                           cr_ruid;                    /* real user id */
    gid_t                           cr_rgid;                    /* real group id */
    uid_t                           cr_suid;                    /* user id saved by exec */
    gid_t                           cr_sgid;                    /* group id saved by exec */
    gid_t                           cr_groups[1];               /* supplementary groups list */
};
typedef struct cred                     cred;
typedef cred                            cred_t;
/* Free return structure for esballoc */
typedef CALLBACK_API_C( void , FreeFuncType )(char *arg);

struct free_rtn {
    FreeFuncType                    free_func;                  /* Routine to call to free buffer */
    char *                          free_arg;                   /* Parameter to free_func */
};
typedef struct free_rtn                 free_rtn;

typedef free_rtn                        frtn_t;
/* data descriptor */
typedef struct datab                    datab;

union datab_db_f {
    datab *                         freep;
    free_rtn *                      frtnp;
};
typedef union datab_db_f                datab_db_f;

struct datab {
    datab_db_f                      db_f;
    unsigned char *                 db_base;                    /* first byte of buffer */
    unsigned char *                 db_lim;                     /* last byte+1 of buffer */
    unsigned char                   db_ref;                     /* count of messages pointing to block*/
    unsigned char                   db_type;                    /* message type */
    unsigned char                   db_iswhat;                  /* message status */
    unsigned char                   db_filler2;                 /* for spacing */
    UInt32                          db_size;                    /* used internally */
    unsigned char *                 db_msgaddr;                 /* used internally */
    long                            db_filler;
};


typedef datab                           dblk_t;
#define db_freep        db_f.freep
#define db_frtnp        db_f.frtnp

/* message block */

struct msgb {
    struct msgb *                   b_next;                     /* next message on queue */
    struct msgb *                   b_prev;                     /* previous message on queue */
    struct msgb *                   b_cont;                     /* next message block of message */
    unsigned char *                 b_rptr;                     /* first unread data byte in buffer */
    unsigned char *                 b_wptr;                     /* first unwritten data byte */
    datab *                         b_datap;                    /* data block */
    unsigned char                   b_band;                     /* message priority */
    unsigned char                   b_pad1;
    unsigned short                  b_flag;
#ifdef  MSGB_XTRA

    MSGB_XTRA

#endif

};
typedef struct msgb                     msgb;

typedef msgb                            mblk_t;
/* mblk flags */
enum {
    MSGMARK                     = 0x01,                         /* last byte of message is tagged */
    MSGNOLOOP                   = 0x02,                         /* don't pass message to write-side of stream */
    MSGDELIM                    = 0x04,                         /* message is delimited */
    MSGNOGET                    = 0x08
};

/* STREAMS environments are expected to define these constants in a public header file.*/

enum {
    STRCTLSZ                    = 256,                          /* Maximum Control buffer size for messages   */
    STRMSGSZ                    = 8192                          /* Maximum # data bytes for messages   */
};

/* Message types */
enum {
    QNORM                       = 0,
    M_DATA                      = 0,                            /* Ordinary data */
    M_PROTO                     = 1,                            /* Internal control info and data */
    M_BREAK                     = 0x08,                         /* Request a driver to send a break */
    M_PASSFP                    = 0x09,                         /* Used to pass a file pointer */
    M_SIG                       = 0x0B,                         /* Requests a signal to be sent */
    M_DELAY                     = 0x0C,                         /* Request a real-time delay */
    M_CTL                       = 0x0D,                         /* For inter-module communication */
    M_IOCTL                     = 0x0E,                         /* Used internally for I_STR requests */
    M_SETOPTS                   = 0x10,                         /* Alters characteristics of Stream head */
    M_RSE                       = 0x11                          /* Reserved for internal use */
};

/* MPS private type */
enum {
    M_MI                        = 0x40,
    M_MI_READ_RESET             = 1,
    M_MI_READ_SEEK              = 2,
    M_MI_READ_END               = 4
};

/* Priority messages types */
enum {
    QPCTL                       = 0x80,
    M_IOCACK                    = 0x81,                         /* Positive ack of previous M_IOCTL */
    M_IOCNAK                    = 0x82,                         /* Previous M_IOCTL failed */
    M_PCPROTO                   = 0x83,                         /* Same as M_PROTO except for priority */
    M_PCSIG                     = 0x84,                         /* Priority signal */
    M_FLUSH                     = 0x86,                         /* Requests modules to flush queues */
    M_STOP                      = 0x87,                         /* Request drivers to stop output */
    M_START                     = 0x88,                         /* Request drivers to start output */
    M_HANGUP                    = 0x89,                         /* Driver can no longer produce data */
    M_ERROR                     = 0x8A,                         /* Reports downstream error condition */
    M_READ                      = 0x8B,                         /* Reports client read at Stream head */
    M_COPYIN                    = 0x8C,                         /* Requests the Stream to copy data in for a module */
    M_COPYOUT                   = 0x8D,                         /* Requests the Stream to copy data out for a module */
    M_IOCDATA                   = 0x8E,                         /* Status from M_COPYIN/M_COPYOUT message */
    M_PCRSE                     = 0x90,                         /* Reserved for internal use */
    M_STOPI                     = 0x91,                         /* Request drivers to stop input */
    M_STARTI                    = 0x92,                         /* Request drivers to start input */
    M_HPDATA                    = 0x93                          /* MPS-private type; high priority data */
};

/* Defines for flush messages */
enum {
    FLUSHALL                    = 1,
    FLUSHDATA                   = 0
};


enum {
    NOERROR                     = -1                            /* used in M_ERROR messages */
};

typedef struct sth_s sth_s;
typedef struct sqh_s sqh_s;
typedef struct q_xtra q_xtra;
#if OTKERNEL
/*
   module_info is aligned differently on 68K than
   on PowerPC.  Yucky.  I can't defined a conditionalised
   pad field because a) you can't conditionalise specific
   fields in the interface definition language used to
   create Universal Interfaces, and b) lots of code 
   assigns C structured constants to global variables
   of this type, and these assignments break if you
   add an extra field to the type.  Instead, I
   set the alignment appropriately before defining the 
   structure.  The problem with doing that is that
   the interface definition language doesn't allow
   my to set the alignment in the middle of a file,
   so I have to do this via "pass throughs".  This
   works fine for the well known languages (C, Pascal),
   but may cause problems for other languages (Java,
   Asm).
*/
#if TARGET_CPU_PPC
 #pragma options align=power
#endif

struct module_info {
    unsigned short                  mi_idnum;                   /* module ID number */
    char *                          mi_idname;                  /* module name */
    long                            mi_minpsz;                  /* min pkt size, for developer use */
    long                            mi_maxpsz;                  /* max pkt size, for developer use */
    unsigned long                   mi_hiwat;                   /* hi-water mark, for flow control */
    unsigned long                   mi_lowat;                   /* lo-water mark, for flow control */
};
typedef struct module_info              module_info;
typedef module_info *                   module_infoPtr;
#if TARGET_CPU_PPC
 #pragma options align=reset
#endif


typedef struct queue                    queue;
typedef CALLBACK_API_C( OTInt32 , admin_t )(void );
typedef CALLBACK_API_C( void , bufcall_t )(long size);
typedef CALLBACK_API_C( void , bufcallp_t )(long size);
typedef CALLBACK_API_C( OTInt32 , closep_t )(queue *q, OTInt32 foo, cred_t *cred);
typedef CALLBACK_API_C( OTInt32 , old_closep_t )(queue *q);
typedef CALLBACK_API_C( OTInt32 , openp_t )(queue *q, dev_t *dev, OTInt32 foo, OTInt32 bar, cred_t *cred);
typedef CALLBACK_API_C( OTInt32 , openOld_t )(queue *q, dev_t dev, OTInt32 foo, OTInt32 bar);
typedef CALLBACK_API_C( OTInt32 , old_openp_t )(queue *q, dev_t dev, OTInt32 foo, OTInt32 bar);
typedef CALLBACK_API_C( OTInt32 , closeOld_t )(queue *q);
typedef CALLBACK_API_C( OTInt32 , putp_t )(queue *q, msgb *mp);
typedef CALLBACK_API_C( OTInt32 , srvp_t )(queue *q);

struct qinit {
    putp_t                          qi_putp;                    /* put procedure */
    srvp_t                          qi_srvp;                    /* service procedure */
    openp_t                         qi_qopen;                   /* called on each open or a push */
    closep_t                        qi_qclose;                  /* called on last close or a pop */
    admin_t                         qi_qadmin;                  /* reserved for future use */
    module_info *                   qi_minfo;                   /* information structure */
    module_stat *                   qi_mstat;                   /* statistics structure - optional */
};
typedef struct qinit                    qinit;
/* defines module or driver */

struct streamtab {
    qinit *                         st_rdinit;                  /* defines read QUEUE */
    qinit *                         st_wrinit;                  /* defines write QUEUE */
    qinit *                         st_muxrinit;                /* for multiplexing drivers only */
    qinit *                         st_muxwinit;                /* ditto */
};
typedef struct streamtab                streamtab;

struct qband {
    struct qband *                  qb_next;                    /* next band for this queue */
    unsigned long                   qb_count;                   /* weighted count of characters in this band */
    msgb *                          qb_first;                   /* head of message queue */
    msgb *                          qb_last;                    /* tail of message queue */
    unsigned long                   qb_hiwat;                   /* high water mark */
    unsigned long                   qb_lowat;                   /* low water mark */
    unsigned short                  qb_flag;                    /* ooo.state */
    short                           qb_pad1;                    /* ooo reserved */
};
typedef struct qband                    qband;
typedef qband                           qband_t;

union queue_q_u {
    queue *                         q_u_link;                   /* link to scheduling queue */
    sqh_s *                         q_u_sqh_parent;
};
typedef union queue_q_u                 queue_q_u;

struct queue {
    qinit *                         q_qinfo;                    /* procedures and limits for queue */
    msgb *                          q_first;                    /* head of message queue */
    msgb *                          q_last;                     /* tail of message queue */
    struct queue *                  q_next;                     /* next queue in Stream */
    queue_q_u                       q_u;
    char *                          q_ptr;                      /* to private data structure */
    unsigned long                   q_count;                    /* weighted count of characters on q */
    long                            q_minpsz;                   /* min packet size accepted */
    long                            q_maxpsz;                   /* max packet size accepted */
    unsigned long                   q_hiwat;                    /* high water mark, for flow control */
    unsigned long                   q_lowat;                    /* low water mark */
    qband *                         q_bandp;                    /* band information */
    unsigned short                  q_flag;                     /* ooo queue state */
    unsigned char                   q_nband;                    /* ooo number of bands */
    unsigned char                   q_pad1[1];                  /* ooo reserved */
    q_xtra *                        q_osx;                      /* Pointer to OS-dependent extra stuff */
    struct queue *                  q_ffcp;                     /* Forward flow control pointer */
    struct queue *                  q_bfcp;                     /* Backward flow control pointer */
};


typedef queue *                         queuePtr;
typedef queue                           queue_t;
#define q_link          q_u.q_u_link
#define q_sqh_parent    q_u.q_u_sqh_parent

/* queue_t flag defines */
enum {
    QREADR                      = 0x01,                         /* This queue is a read queue */
    QNOENB                      = 0x02,                         /* Don't enable in putq */
    QFULL                       = 0x04,                         /* The queue is full */
    QWANTR                      = 0x08,                         /* The queue should be scheduled in the next putq */
    QWANTW                      = 0x10,                         /* The stream should be back enabled when this queue drains */
    QUSE                        = 0x20,                         /* The queue is allocated and ready for use */
    QENAB                       = 0x40,                         /* The queue is scheduled (on the run queue) */
    QBACK                       = 0x80,                         /* The queue has been back enabled */
    QOLD                        = 0x0100,                       /* Module supports old style opens and closes */
    QHLIST                      = 0x0200,                       /* The Stream head is doing something with this queue (Not supported by MPS) */
    QWELDED                     = 0x0400,                       /* Mentat flag for welded queues */
    QUNWELDING                  = 0x0800,                       /* Queue is scheduled to be unwelded */
    QPROTECTED                  = 0x1000,                       /* Mentat flag for unsafe q access */
    QEXCOPENCLOSE               = 0x2000                        /* Queue wants exclusive open/close calls */
};

/* qband_t flag defines */
enum {
    QB_FULL                     = 0x01,                         /* The band is full */
    QB_WANTW                    = 0x02,                         /* The stream should be back enabled when this band/queue drains */
    QB_BACK                     = 0x04                          /* The queue has been back enabled */
};

#else
/*
   Client code views a queue_t as a simple cookie.
   The real definition lives above and is only available
   to kernel code.
*/

struct queue {
    UInt32                          dummy;
};
typedef struct queue                    queue;
typedef SInt32                          queue_t;
#endif  /* OTKERNEL */

/* structure contained in M_COPYIN/M_COPYOUT messages */

typedef unsigned char *                 caddr_t;

struct copyreq {
    SInt32                          cq_cmd;                     /* ioctl command (from ioc_cmd) */
    cred *                          cq_cr;                      /* pointer to full credentials */
    UInt32                          cq_id;                      /* ioctl id (from ioc_id) */
    caddr_t                         cq_addr;                    /* address to copy data to/from */
    UInt32                          cq_size;                    /* number of bytes to copy */
    SInt32                          cq_flag;                    /* state */
    mblk_t *                        cq_private;                 /* private state information */
    long                            cq_filler[4];
};
typedef struct copyreq                  copyreq;
#define cq_uid  cq_cr->cr_uid
#define cq_gid  cq_cr->cr_gid


/* copyreq defines */
enum {
    STRCANON                    = 0x01,                         /* b_cont data block contains canonical format specifier */
    RECOPY                      = 0x02                          /* perform I_STR copyin again this time using canonical format specifier */
};

/* structure contained in M_IOCDATA message block */

struct copyresp {
    SInt32                          cp_cmd;                     /* ioctl command (from ioc_cmd) */
    cred *                          cp_cr;                      /* pointer to full credentials */
    UInt32                          cp_id;                      /* ioctl id (from ioc_id) */
    caddr_t                         cp_rval;                    /* status of request; 0 for success; error value for failure */
    UInt32                          cp_pad1;
    SInt32                          cp_pad2;
    mblk_t *                        cp_private;                 /* private state information */
    long                            cp_filler[4];
};
typedef struct copyresp                 copyresp;
#define cp_uid  cp_cr->cr_uid
#define cp_gid  cp_cr->cr_gid

/* structure contained in an M_IOCTL message block */

struct iocblk {
    SInt32                          ioc_cmd;                    /* ioctl command type */
    cred *                          ioc_cr;                     /* pointer to full credentials */
    UInt32                          ioc_id;                     /* ioctl id */
    UInt32                          ioc_count;                  /* count of bytes in data field */
    SInt32                          ioc_error;                  /* error code */
    SInt32                          ioc_rval;                   /* return value */
    long                            ioc_filler[4];
};
typedef struct iocblk                   iocblk;
#define ioc_uid ioc_cr->cr_uid
#define ioc_gid ioc_cr->cr_gid



enum {
    TRANSPARENT                 = (unsigned long)0xFFFFFFFF
};

/* Used in M_IOCTL mblks to muxes (ioc_cmd I_LINK) */

struct linkblk {
    queue *                         l_qtop;                     /* lowest level write queue of upper stream */
    queue *                         l_qbot;                     /* highest level write queue of lower stream */
    SInt32                          l_index;                    /* system-unique index for lower stream */
    long                            l_pad[5];
};
typedef struct linkblk                  linkblk;
/* structure contained in an M_PASSFP message block */

struct strpfp {
    unsigned long                   pass_file_cookie;           /* file 'pointer' */
    unsigned short                  pass_uid;                   /* user id of sending stream */
    unsigned short                  pass_gid;
    sth_s *                         pass_sth;                   /* Stream head pointer of passed stream */
};
typedef struct strpfp                   strpfp;
/* structure contained in an M_SETOPTS message block */

struct stroptions {
    unsigned long                   so_flags;                   /* options to set */
    short                           so_readopt;                 /* read option */
    unsigned short                  so_wroff;                   /* write offset */
    long                            so_minpsz;                  /* minimum read packet size */
    long                            so_maxpsz;                  /* maximum read packet size */
    unsigned long                   so_hiwat;                   /* read queue high-water mark */
    unsigned long                   so_lowat;                   /* read queue low-water mark */
    unsigned char                   so_band;                    /* band for water marks */
    unsigned char                   so_filler[3];               /* added for alignment */
    unsigned long                   so_poll_set;                /* poll events to set */
    unsigned long                   so_poll_clr;                /* poll events to clear */
};
typedef struct stroptions               stroptions;
/* definitions for so_flags field */
enum {
    SO_ALL                      = 0x7FFF,                       /* Update all options */
    SO_READOPT                  = 0x0001,                       /* Set the read mode */
    SO_WROFF                    = 0x0002,                       /* Insert an offset in write M_DATA mblks */
    SO_MINPSZ                   = 0x0004,                       /* Change the min packet size on sth rq */
    SO_MAXPSZ                   = 0x0008,                       /* Change the max packet size on sth rq */
    SO_HIWAT                    = 0x0010,                       /* Change the high water mark on sth rq */
    SO_LOWAT                    = 0x0020,                       /* Change the low water mark */
    SO_MREADON                  = 0x0040,                       /* Request M_READ messages */
    SO_MREADOFF                 = 0x0080,                       /* Don't gen M_READ messages */
    SO_NDELON                   = 0x0100,                       /* old TTY semantics for O_NDELAY reads and writes */
    SO_NDELOFF                  = 0x0200,                       /* STREAMS semantics for O_NDELAY reads and writes */
    SO_ISTTY                    = 0x0400,                       /* Become a controlling tty */
    SO_ISNTTY                   = 0x0800,                       /* No longer a controlling tty */
    SO_TOSTOP                   = 0x1000,                       /* Stop on background writes */
    SO_TONSTOP                  = 0x2000,                       /* Don't stop on background writes */
    SO_BAND                     = 0x4000,                       /* Water marks are for a band */
    SO_POLL_SET                 = 0x8000,                       /* Set events to poll */
    SO_POLL_CLR                 = 0x00010000                    /* Clear events to poll */
};

/* Buffer Allocation Priority */
enum {
    BPRI_LO                     = 1,
    BPRI_MED                    = 2,
    BPRI_HI                     = 3
};

enum {
    INFPSZ                      = -1
};


/** Test whether message is a data message */
#define datamsg(type)   ((type) == M_DATA || (type) == M_PROTO || (type) == M_PCPROTO  ||  (type) == M_DELAY)

enum {
    CLONEOPEN                   = 0x02,
    MODOPEN                     = 0x01,
    OPENFAIL                    = -1
};


/* Enumeration values for strqget and strqset */

typedef SInt32 qfields;
enum {
    QHIWAT                      = 0,
    QLOWAT                      = 1,
    QMAXPSZ                     = 2,
    QMINPSZ                     = 3,
    QCOUNT                      = 4,
    QFIRST                      = 5,
    QLAST                       = 6,
    QFLAG                       = 7,
    QBAD                        = 8
};


typedef qfields                         qfields_t;
#endif  /* CALL_NOT_IN_CARBON */

/* ***** From the Mentat "stropts.h" ******/


enum {
    I_NREAD                     = ((MIOC_STREAMIO << 8) | 1),   /* return the number of bytes in 1st msg */
    I_PUSH                      = ((MIOC_STREAMIO << 8) | 2),   /* push module just below stream head */
    I_POP                       = ((MIOC_STREAMIO << 8) | 3),   /* pop module below stream head */
    I_LOOK                      = ((MIOC_STREAMIO << 8) | 4),   /* retrieve name of first stream module */
    I_FLUSH                     = ((MIOC_STREAMIO << 8) | 5),   /* flush all input and/or output queues */
    I_SRDOPT                    = ((MIOC_STREAMIO << 8) | 6),   /* set the read mode */
    I_GRDOPT                    = ((MIOC_STREAMIO << 8) | 7),   /* get the current read mode */
    I_STR                       = ((MIOC_STREAMIO << 8) | 8),   /* create an internal ioctl message      */
    I_SETSIG                    = ((MIOC_STREAMIO << 8) | 9),   /* request SIGPOLL signal on events */
    I_GETSIG                    = ((MIOC_STREAMIO << 8) | 10),  /* query the registered events */
    I_FIND                      = ((MIOC_STREAMIO << 8) | 11),  /* check for module in stream          */
    I_LINK                      = ((MIOC_STREAMIO << 8) | 12),  /* connect stream under mux fd */
    I_UNLINK                    = ((MIOC_STREAMIO << 8) | 13),  /* disconnect two streams */
    I_PEEK                      = ((MIOC_STREAMIO << 8) | 15),  /* peek at data on read queue */
    I_FDINSERT                  = ((MIOC_STREAMIO << 8) | 16),  /* create a message and send downstream */
    I_SENDFD                    = ((MIOC_STREAMIO << 8) | 17),  /* send an fd to a connected pipe stream */
    I_RECVFD                    = ((MIOC_STREAMIO << 8) | 18),  /* retrieve a file descriptor */
    I_FLUSHBAND                 = ((MIOC_STREAMIO << 8) | 19),  /* flush a particular input and/or output band */
    I_SWROPT                    = ((MIOC_STREAMIO << 8) | 20),  /* set the write mode */
    I_GWROPT                    = ((MIOC_STREAMIO << 8) | 21),  /* get the current write mode */
    I_LIST                      = ((MIOC_STREAMIO << 8) | 22),  /* get a list of all modules on a stream  */
    I_ATMARK                    = ((MIOC_STREAMIO << 8) | 23),  /* check to see if the next message is "marked" */
    I_CKBAND                    = ((MIOC_STREAMIO << 8) | 24),  /* check for a message of a particular band */
    I_GETBAND                   = ((MIOC_STREAMIO << 8) | 25),  /* get the band of the next message to be read */
    I_CANPUT                    = ((MIOC_STREAMIO << 8) | 26),  /* check to see if a message may be passed on a stream */
    I_SETCLTIME                 = ((MIOC_STREAMIO << 8) | 27),  /* set the close timeout wait */
    I_GETCLTIME                 = ((MIOC_STREAMIO << 8) | 28),  /* get the current close timeout wait */
    I_PLINK                     = ((MIOC_STREAMIO << 8) | 29),  /* permanently connect a stream under a mux */
    I_PUNLINK                   = ((MIOC_STREAMIO << 8) | 30),  /* disconnect a permanent link */
    I_GETMSG                    = ((MIOC_STREAMIO << 8) | 40),  /* getmsg() system call */
    I_PUTMSG                    = ((MIOC_STREAMIO << 8) | 41),  /* putmsg() system call */
    I_POLL                      = ((MIOC_STREAMIO << 8) | 42),  /* poll() system call */
    I_SETDELAY                  = ((MIOC_STREAMIO << 8) | 43),  /* set blocking status */
    I_GETDELAY                  = ((MIOC_STREAMIO << 8) | 44),  /* get blocking status */
    I_RUN_QUEUES                = ((MIOC_STREAMIO << 8) | 45),  /* sacrifice for the greater good */
    I_GETPMSG                   = ((MIOC_STREAMIO << 8) | 46),  /* getpmsg() system call */
    I_PUTPMSG                   = ((MIOC_STREAMIO << 8) | 47),  /* putpmsg() system call */
    I_AUTOPUSH                  = ((MIOC_STREAMIO << 8) | 48),  /* for systems that cannot do the autopush in open */
    I_PIPE                      = ((MIOC_STREAMIO << 8) | 49),  /* for pipe library call */
    I_HEAP_REPORT               = ((MIOC_STREAMIO << 8) | 50),  /* get heap statistics */
    I_FIFO                      = ((MIOC_STREAMIO << 8) | 51)   /* for fifo library call */
};

/* priority message request on putmsg() or strpeek */
enum {
    RS_HIPRI                    = 0x01
};

/* flags for getpmsg and putpmsg */
enum {
    MSG_HIPRI                   = 0x01,
    MSG_BAND                    = 0x02,                         /* Retrieve a message from a particular band */
    MSG_ANY                     = 0x04                          /* Retrieve a message from any band */
};

/* return values from getmsg(), 0 indicates all ok */
enum {
    MORECTL                     = 0x01,                         /* more control info available */
    MOREDATA                    = 0x02                          /* more data available */
};


enum {
    FMNAMESZ                    = 31                            /* maximum length of a module or device name */
};


/* Infinite poll wait time */
enum {
    INFTIM                      = (unsigned long)0xFFFFFFFF
};

/* flush requests */
enum {
    FLUSHR                      = 0x01,                         /* Flush the read queue */
    FLUSHW                      = 0x02,                         /* Flush the write queue */
    FLUSHRW                     = (FLUSHW | FLUSHR)             /* Flush both */
};

enum {
    FLUSHBAND                   = 0x40                          /* Flush a particular band */
};

/* I_FLUSHBAND */

struct bandinfo {
    unsigned char                   bi_pri;                     /* Band to flush */
    char                            pad1;
    SInt32                          bi_flag;                    /* One of the above flush requests */
};
typedef struct bandinfo                 bandinfo;
/* flags for I_ATMARK */
enum {
    ANYMARK                     = 0x01,                         /* Check if message is marked */
    LASTMARK                    = 0x02                          /* Check if this is the only message marked */
};

/* signal event masks */
enum {
    S_INPUT                     = 0x01,                         /* A non-M_PCPROTO message has arrived */
    S_HIPRI                     = 0x02,                         /* A priority (M_PCPROTO) message is available */
    S_OUTPUT                    = 0x04,                         /* The write queue is no longer full */
    S_MSG                       = 0x08,                         /* A signal message has reached the front of read queue */
    S_RDNORM                    = 0x10,                         /* A non-priority message is available */
    S_RDBAND                    = 0x20,                         /* A banded messsage is available */
    S_WRNORM                    = 0x40,                         /* Same as S_OUTPUT */
    S_WRBAND                    = 0x80,                         /* A priority band exists and is writable */
    S_ERROR                     = 0x0100,                       /* Error message has arrived */
    S_HANGUP                    = 0x0200,                       /* Hangup message has arrived */
    S_BANDURG                   = 0x0400                        /* Use SIGURG instead of SIGPOLL on S_RDBAND signals */
};

/* read mode bits for I_S|GRDOPT; choose one of the following */
enum {
    RNORM                       = 0x01,                         /* byte-stream mode, default */
    RMSGD                       = 0x02,                         /* message-discard mode */
    RMSGN                       = 0x04,                         /* message-nondiscard mode */
    RFILL                       = 0x08                          /* fill read buffer mode (PSE private) */
};

/* More read modes, these are bitwise or'ed with the modes above */
enum {
    RPROTNORM                   = 0x10,                         /* Normal handling of M_PROTO/M_PCPROTO messages, default */
    RPROTDIS                    = 0x20,                         /* Discard M_PROTO/M_PCPROTO message blocks */
    RPROTDAT                    = 0x40                          /* Convert M_PROTO/M_PCPROTO message blocks into M_DATA */
};

/* write modes for I_S|GWROPT */
enum {
    SNDZERO                     = 0x01                          /* Send a zero-length message downstream on a write of zero bytes */
};

enum {
    MUXID_ALL                   = -1                            /* Unlink all lower streams for I_UNLINK and I_PUNLINK */
};

/*
   strbuf is moved to "OpenTransport.h" because that header file
   exports provider routines that take it as a parameter.
*/

/* structure of ioctl data on I_FDINSERT */

struct strfdinsert {
    strbuf                          ctlbuf;
    strbuf                          databuf;
    long                            flags;                      /* type of message, 0 or RS_HIPRI */
    long                            fildes;                     /* fd of other stream (FDCELL) */
    SInt32                          offset;                     /* where to put other stream read qp */
};
typedef struct strfdinsert              strfdinsert;
/* I_LIST structures */

struct str_mlist {
    char                            l_name[32];
};
typedef struct str_mlist                str_mlist;

struct str_list {
    SInt32                          sl_nmods;                   /* number of modules in sl_modlist array */
    str_mlist *                     sl_modlist;
};
typedef struct str_list                 str_list;
/* I_PEEK structure */

struct strpeek {
    strbuf                          ctlbuf;
    strbuf                          databuf;
    long                            flags;                      /* if RS_HIPRI, get priority messages only */
};
typedef struct strpeek                  strpeek;
/* structure for getpmsg and putpmsg */

struct strpmsg {
    strbuf                          ctlbuf;
    strbuf                          databuf;
    SInt32                          band;
    long                            flags;
};
typedef struct strpmsg                  strpmsg;
/* structure of ioctl data on I_RECVFD */

struct strrecvfd {
    long                            fd;                         /* new file descriptor (FDCELL) */
    unsigned short                  uid;                        /* user id of sending stream */
    unsigned short                  gid;
    char                            fill[8];
};
typedef struct strrecvfd                strrecvfd;
/* structure of ioctl data on I_STR */

struct strioctl {
    SInt32                          ic_cmd;                     /* downstream command */
    SInt32                          ic_timout;                  /* ACK/NAK timeout */
    SInt32                          ic_len;                     /* length of data arg */
    char *                          ic_dp;                      /* ptr to data arg */
};
typedef struct strioctl                 strioctl;
/* ***** From the Mentat "strlog.h" ******/


struct log_ctl {
    short                           mid;
    short                           sid;
    char                            level;
    char                            pad1;
    short                           flags;
    long                            ltime;
    long                            ttime;
    SInt32                          seq_no;
};
typedef struct log_ctl                  log_ctl;
enum {
    SL_FATAL                    = 0x01,                         /* Fatal error */
    SL_NOTIFY                   = 0x02,                         /* Notify the system administrator */
    SL_ERROR                    = 0x04,                         /* Pass message to error logger */
    SL_TRACE                    = 0x08,                         /* Pass message to tracer */
    SL_CONSOLE                  = 0x00,                         /* Console messages are disabled */
    SL_WARN                     = 0x20,                         /* Warning */
    SL_NOTE                     = 0x40                          /* Notice this message */
};


struct trace_ids {
    short                           ti_mid;
    short                           ti_sid;
    char                            ti_level;
};
typedef struct trace_ids                trace_ids;
enum {
    I_TRCLOG                    = ((MIOC_STRLOG << 8) | 1),
    I_ERRLOG                    = ((MIOC_STRLOG << 8) | 2)
};

enum {
    LOGMSGSZ                    = 128
};

/* ***** From the Mentat "tihdr.h" ******/

#if CALL_NOT_IN_CARBON

/* TPI Primitives*/



enum {
    T_BIND_REQ = 101,
  T_CONN_REQ = 102,           /* connection request */
   T_CONN_RES = 103,           /* respond to connection indication */
 T_DATA_REQ = 104,
  T_DISCON_REQ = 105,
    T_EXDATA_REQ = 106,
    T_INFO_REQ = 107,
  T_OPTMGMT_REQ = 108,
   T_ORDREL_REQ = 109,
    T_UNBIND_REQ = 110,
    T_UNITDATA_REQ = 111,
  T_ADDR_REQ = 112,           /* Get address request              */
 T_UREQUEST_REQ = 113,       /* UnitRequest (transaction) req    */
 T_REQUEST_REQ = 114,        /* Request (CO transaction) req     */
 T_UREPLY_REQ = 115,         /* UnitRequest (transaction) req    */
 T_REPLY_REQ = 116,          /* REPLY (CO transaction) req       */
 T_CANCELREQUEST_REQ = 117,  /* Cancel outgoing request          */
 T_CANCELREPLY_REQ = 118,    /* Cancel incoming request          */
 T_REGNAME_REQ = 119,        /* Request name registration        */
 T_DELNAME_REQ = 120,        /* Request delete name registration */
 T_LKUPNAME_REQ = 121,       /* Request name lookup              */

    T_BIND_ACK = 122,
  T_CONN_CON = 123,           /* connection confirmation          */
 T_CONN_IND = 124,           /* incoming connection indication   */
 T_DATA_IND = 125,
  T_DISCON_IND = 126,
    T_ERROR_ACK = 127,
 T_EXDATA_IND = 128,
    T_INFO_ACK = 129,
  T_OK_ACK = 130,
    T_OPTMGMT_ACK = 131,
   T_ORDREL_IND = 132,
    T_UNITDATA_IND = 133,
  T_UDERROR_IND = 134,
   T_ADDR_ACK = 135,           /* Get address ack                  */
 T_UREQUEST_IND = 136,       /* UnitRequest (transaction) ind    */
 T_REQUEST_IND = 137,        /* Request (CO transaction) ind     */
 T_UREPLY_IND = 138,         /* Incoming unit reply              */
 T_REPLY_IND = 139,          /* Incoming reply                   */
 T_UREPLY_ACK = 140,         /* outgoing Unit Reply is complete  */
 T_REPLY_ACK = 141,          /* outgoing Reply is complete       */
 T_RESOLVEADDR_REQ = 142,
   T_RESOLVEADDR_ACK = 143,

  T_LKUPNAME_CON = 146,       /* Results of name lookup           */
 T_LKUPNAME_RES = 147,       /* Partial results of name lookup   */
 T_REGNAME_ACK = 148,        /* Request name registration        */
 T_SEQUENCED_ACK = 149,      /* Sequenced version of OK or ERROR ACK */

    T_EVENT_IND = 160           /* Miscellaneous event Indication       */
};

/* State values */
enum {
    TS_UNBND                    = 1,
    TS_WACK_BREQ                = 2,
    TS_WACK_UREQ                = 3,
    TS_IDLE                     = 4,
    TS_WACK_OPTREQ              = 5,
    TS_WACK_CREQ                = 6,
    TS_WCON_CREQ                = 7,
    TS_WRES_CIND                = 8,
    TS_WACK_CRES                = 9,
    TS_DATA_XFER                = 10,
    TS_WIND_ORDREL              = 11,
    TS_WREQ_ORDREL              = 12,
    TS_WACK_DREQ6               = 13,
    TS_WACK_DREQ7               = 14,
    TS_WACK_DREQ9               = 15,
    TS_WACK_DREQ10              = 16,
    TS_WACK_DREQ11              = 17,
    TS_WACK_ORDREL              = 18,
    TS_NOSTATES                 = 19,
    TS_BAD_STATE                = 19
};

/* Transport events */
enum {
    TE_OPENED                   = 1,
    TE_BIND                     = 2,
    TE_OPTMGMT                  = 3,
    TE_UNBIND                   = 4,
    TE_CLOSED                   = 5,
    TE_CONNECT1                 = 6,
    TE_CONNECT2                 = 7,
    TE_ACCEPT1                  = 8,
    TE_ACCEPT2                  = 9,
    TE_ACCEPT3                  = 10,
    TE_SND                      = 11,
    TE_SNDDIS1                  = 12,
    TE_SNDDIS2                  = 13,
    TE_SNDREL                   = 14,
    TE_SNDUDATA                 = 15,
    TE_LISTEN                   = 16,
    TE_RCVCONNECT               = 17,
    TE_RCV                      = 18,
    TE_RCVDIS1                  = 19,
    TE_RCVDIS2                  = 20,
    TE_RCVDIS3                  = 21,
    TE_RCVREL                   = 22,
    TE_RCVUDATA                 = 23,
    TE_RCVUDERR                 = 24,
    TE_PASS_CONN                = 25,
    TE_BAD_EVENT                = 26
};


struct T_addr_ack {
    long                            PRIM_type;                  /* Always T_ADDR_ACK */
    long                            LOCADDR_length;
    long                            LOCADDR_offset;
    long                            REMADDR_length;
    long                            REMADDR_offset;
};
typedef struct T_addr_ack               T_addr_ack;

struct T_addr_req {
    long                            PRIM_type;                  /* Always T_ADDR_REQ */
};
typedef struct T_addr_req               T_addr_req;

struct T_bind_ack {
    long                            PRIM_type;                  /* always T_BIND_ACK */
    long                            ADDR_length;
    long                            ADDR_offset;
    unsigned long                   CONIND_number;
};
typedef struct T_bind_ack               T_bind_ack;

struct T_bind_req {
    long                            PRIM_type;                  /* always T_BIND_REQ */
    long                            ADDR_length;
    long                            ADDR_offset;
    unsigned long                   CONIND_number;
};
typedef struct T_bind_req               T_bind_req;

struct T_conn_con {
    long                            PRIM_type;                  /* always T_CONN_CON */
    long                            RES_length;                 /* responding address length */
    long                            RES_offset;
    long                            OPT_length;
    long                            OPT_offset;
};
typedef struct T_conn_con               T_conn_con;

struct T_conn_ind {
    long                            PRIM_type;                  /* always T_CONN_IND */
    long                            SRC_length;
    long                            SRC_offset;
    long                            OPT_length;
    long                            OPT_offset;
    long                            SEQ_number;
};
typedef struct T_conn_ind               T_conn_ind;

struct T_conn_req {
    long                            PRIM_type;                  /* always T_CONN_REQ */
    long                            DEST_length;
    long                            DEST_offset;
    long                            OPT_length;
    long                            OPT_offset;
};
typedef struct T_conn_req               T_conn_req;

struct T_conn_res {
    long                            PRIM_type;                  /* always T_CONN_RES */
    queue *                         QUEUE_ptr;
    long                            OPT_length;
    long                            OPT_offset;
    long                            SEQ_number;
};
typedef struct T_conn_res               T_conn_res;

struct T_data_ind {
    long                            PRIM_type;                  /* always T_DATA_IND */
    long                            MORE_flag;
};
typedef struct T_data_ind               T_data_ind;

struct T_data_req {
    long                            PRIM_type;                  /* always T_DATA_REQ */
    long                            MORE_flag;
};
typedef struct T_data_req               T_data_req;

struct T_discon_ind {
    long                            PRIM_type;                  /* always T_DISCON_IND */
    long                            DISCON_reason;
    long                            SEQ_number;
};
typedef struct T_discon_ind             T_discon_ind;

struct T_discon_req {
    long                            PRIM_type;                  /* always T_DISCON_REQ */
    long                            SEQ_number;
};
typedef struct T_discon_req             T_discon_req;

struct T_exdata_ind {
    long                            PRIM_type;                  /* always T_EXDATA_IND */
    long                            MORE_flag;
};
typedef struct T_exdata_ind             T_exdata_ind;

struct T_exdata_req {
    long                            PRIM_type;                  /* always T_EXDATA_REQ */
    long                            MORE_flag;
};
typedef struct T_exdata_req             T_exdata_req;

struct T_error_ack {
    long                            PRIM_type;                  /* always T_ERROR_ACK */
    long                            ERROR_prim;                 /* primitive in error */
    long                            TLI_error;
    long                            UNIX_error;
};
typedef struct T_error_ack              T_error_ack;

struct T_info_ack {
    long                            PRIM_type;                  /* always T_INFO_ACK */
    long                            TSDU_size;                  /* max TSDU size */
    long                            ETSDU_size;                 /* max ETSDU size */
    long                            CDATA_size;                 /* connect data size */
    long                            DDATA_size;                 /* disconnect data size */
    long                            ADDR_size;                  /* TSAP size */
    long                            OPT_size;                   /* options size */
    long                            TIDU_size;                  /* TIDU size */
    long                            SERV_type;                  /* service type */
    long                            CURRENT_state;              /* current state */
    long                            PROVIDER_flag;              /* provider flags (see xti.h for defines) */
};
typedef struct T_info_ack               T_info_ack;
/* Provider flags */
enum {
    SENDZERO                    = 0x0001,                       /* supports 0-length TSDU's */
    XPG4_1                      = 0x0002                        /* provider supports recent stuff */
};


struct T_info_req {
    long                            PRIM_type;                  /* always T_INFO_REQ */
};
typedef struct T_info_req               T_info_req;

struct T_ok_ack {
    long                            PRIM_type;                  /* always T_OK_ACK */
    long                            CORRECT_prim;
};
typedef struct T_ok_ack                 T_ok_ack;

struct T_optmgmt_ack {
    long                            PRIM_type;                  /* always T_OPTMGMT_ACK */
    long                            OPT_length;
    long                            OPT_offset;
    long                            MGMT_flags;
};
typedef struct T_optmgmt_ack            T_optmgmt_ack;

struct T_optmgmt_req {
    long                            PRIM_type;                  /* always T_OPTMGMT_REQ */
    long                            OPT_length;
    long                            OPT_offset;
    long                            MGMT_flags;
};
typedef struct T_optmgmt_req            T_optmgmt_req;

struct T_ordrel_ind {
    long                            PRIM_type;                  /* always T_ORDREL_IND */
};
typedef struct T_ordrel_ind             T_ordrel_ind;

struct T_ordrel_req {
    long                            PRIM_type;                  /* always T_ORDREL_REQ */
};
typedef struct T_ordrel_req             T_ordrel_req;

struct T_unbind_req {
    long                            PRIM_type;                  /* always T_UNBIND_REQ */
};
typedef struct T_unbind_req             T_unbind_req;

struct T_uderror_ind {
    long                            PRIM_type;                  /* always T_UDERROR_IND */
    long                            DEST_length;
    long                            DEST_offset;
    long                            OPT_length;
    long                            OPT_offset;
    long                            ERROR_type;
};
typedef struct T_uderror_ind            T_uderror_ind;

struct T_unitdata_ind {
    long                            PRIM_type;                  /* always T_UNITDATA_IND */
    long                            SRC_length;
    long                            SRC_offset;
    long                            OPT_length;
    long                            OPT_offset;
};
typedef struct T_unitdata_ind           T_unitdata_ind;

struct T_unitdata_req {
    long                            PRIM_type;                  /* always T_UNITDATA_REQ */
    long                            DEST_length;
    long                            DEST_offset;
    long                            OPT_length;
    long                            OPT_offset;
};
typedef struct T_unitdata_req           T_unitdata_req;

struct T_resolveaddr_ack {
    long                            PRIM_type;                  /* always T_RESOLVEADDR_ACK */
    long                            SEQ_number;
    long                            ADDR_length;
    long                            ADDR_offset;
    long                            ORIG_client;
    long                            ORIG_data;
    long                            TLI_error;
    long                            UNIX_error;
};
typedef struct T_resolveaddr_ack        T_resolveaddr_ack;

struct T_resolveaddr_req {
    long                            PRIM_type;                  /* always T_RESOLVEADDR_REQ */
    long                            SEQ_number;
    long                            ADDR_length;
    long                            ADDR_offset;
    long                            ORIG_client;
    long                            ORIG_data;
    long                            MAX_milliseconds;
};
typedef struct T_resolveaddr_req        T_resolveaddr_req;

struct T_unitreply_ind {
    long                            PRIM_type;                  /* Always T_UREPLY_IND */
    long                            SEQ_number;
    long                            OPT_length;
    long                            OPT_offset;
    long                            REP_flags;
    long                            TLI_error;
    long                            UNIX_error;
};
typedef struct T_unitreply_ind          T_unitreply_ind;

struct T_unitrequest_ind {
    long                            PRIM_type;                  /* Always T_UREQUEST_IND */
    long                            SEQ_number;
    long                            SRC_length;
    long                            SRC_offset;
    long                            OPT_length;
    long                            OPT_offset;
    long                            REQ_flags;
};
typedef struct T_unitrequest_ind        T_unitrequest_ind;

struct T_unitrequest_req {
    long                            PRIM_type;                  /* Always T_UREQUEST_REQ */
    long                            SEQ_number;
    long                            DEST_length;
    long                            DEST_offset;
    long                            OPT_length;
    long                            OPT_offset;
    long                            REQ_flags;
};
typedef struct T_unitrequest_req        T_unitrequest_req;

struct T_unitreply_req {
    long                            PRIM_type;                  /* Always T_UREPLY_REQ */
    long                            SEQ_number;
    long                            OPT_length;
    long                            OPT_offset;
    long                            REP_flags;
};
typedef struct T_unitreply_req          T_unitreply_req;

struct T_unitreply_ack {
    long                            PRIM_type;                  /* Always T_UREPLY_ACK */
    long                            SEQ_number;
    long                            TLI_error;
    long                            UNIX_error;
};
typedef struct T_unitreply_ack          T_unitreply_ack;

struct T_cancelrequest_req {
    long                            PRIM_type;                  /* Always T_CANCELREQUEST_REQ */
    long                            SEQ_number;
};
typedef struct T_cancelrequest_req      T_cancelrequest_req;

struct T_cancelreply_req {
    long                            PRIM_type;                  /* Always T_CANCELREPLY_REQ */
    long                            SEQ_number;
};
typedef struct T_cancelreply_req        T_cancelreply_req;

struct T_reply_ind {
    long                            PRIM_type;                  /* Always T_REPLY_IND */
    long                            SEQ_number;
    long                            OPT_length;
    long                            OPT_offset;
    long                            REP_flags;
    long                            TLI_error;
    long                            UNIX_error;
};
typedef struct T_reply_ind              T_reply_ind;

struct T_request_ind {
    long                            PRIM_type;                  /* Always T_REQUEST_IND */
    long                            SEQ_number;
    long                            OPT_length;
    long                            OPT_offset;
    long                            REQ_flags;
};
typedef struct T_request_ind            T_request_ind;

struct T_request_req {
    long                            PRIM_type;                  /* Always T_REQUEST_REQ */
    long                            SEQ_number;
    long                            OPT_length;
    long                            OPT_offset;
    long                            REQ_flags;
};
typedef struct T_request_req            T_request_req;

struct T_reply_req {
    long                            PRIM_type;                  /* Always T_REPLY_REQ */
    long                            SEQ_number;
    long                            OPT_length;
    long                            OPT_offset;
    long                            REP_flags;
};
typedef struct T_reply_req              T_reply_req;

struct T_reply_ack {
    long                            PRIM_type;                  /* Always T_REPLY_ACK */
    long                            SEQ_number;
    long                            TLI_error;
    long                            UNIX_error;
};
typedef struct T_reply_ack              T_reply_ack;

struct T_regname_req {
    long                            PRIM_type;                  /* Always T_REGNAME_REQ */
    long                            SEQ_number;                 /* Reply is sequence ack */
    long                            NAME_length;
    long                            NAME_offset;
    long                            ADDR_length;
    long                            ADDR_offset;
    long                            REQ_flags;
};
typedef struct T_regname_req            T_regname_req;

struct T_regname_ack {
    long                            PRIM_type;                  /* always T_REGNAME_ACK     */
    long                            SEQ_number;
    long                            REG_id;
    long                            ADDR_length;
    long                            ADDR_offset;
};
typedef struct T_regname_ack            T_regname_ack;

struct T_delname_req {
    long                            PRIM_type;                  /* Always T_DELNAME_REQ */
    long                            SEQ_number;                 /* Reply is sequence ack */
    long                            NAME_length;
    long                            NAME_offset;
};
typedef struct T_delname_req            T_delname_req;

struct T_lkupname_req {
    long                            PRIM_type;                  /* Always T_LKUPNAME_REQ */
    long                            SEQ_number;                 /* Reply is sequence ack */
    long                            NAME_length;                /* ... or T_LKUPNAME_CON */
    long                            NAME_offset;
    long                            ADDR_length;
    long                            ADDR_offset;
    long                            MAX_number;
    long                            MAX_milliseconds;
    long                            REQ_flags;
};
typedef struct T_lkupname_req           T_lkupname_req;

struct T_lkupname_con {
    long                            PRIM_type;                  /* Either T_LKUPNAME_CON */
    long                            SEQ_number;                 /* Or T_LKUPNAME_RES */
    long                            NAME_length;
    long                            NAME_offset;
    long                            RSP_count;
    long                            RSP_cumcount;
};
typedef struct T_lkupname_con           T_lkupname_con;

struct T_sequence_ack {
    long                            PRIM_type;                  /* always T_SEQUENCED_ACK     */
    long                            ORIG_prim;                  /* original primitive        */
    long                            SEQ_number;
    long                            TLI_error;
    long                            UNIX_error;
};
typedef struct T_sequence_ack           T_sequence_ack;

struct T_event_ind {
    long                            PRIM_type;                  /* always T_EVENT_IND        */
    long                            EVENT_code;
    long                            EVENT_cookie;
};
typedef struct T_event_ind              T_event_ind;

union T_primitives {
    long                        type;

    long                            primType;
    T_addr_ack                      taddrack;
    T_bind_ack                      tbindack;
    T_bind_req                      tbindreq;
    T_conn_con                      tconncon;
    T_conn_ind                      tconnind;
    T_conn_req                      tconnreq;
    T_conn_res                      tconnres;
    T_data_ind                      tdataind;
    T_data_req                      tdatareq;
    T_discon_ind                    tdisconind;
    T_discon_req                    tdisconreq;
    T_exdata_ind                    texdataind;
    T_exdata_req                    texdatareq;
    T_error_ack                     terrorack;
    T_info_ack                      tinfoack;
    T_info_req                      tinforeq;
    T_ok_ack                        tokack;
    T_optmgmt_ack                   toptmgmtack;
    T_optmgmt_req                   toptmgmtreq;
    T_ordrel_ind                    tordrelind;
    T_ordrel_req                    tordrelreq;
    T_unbind_req                    tunbindreq;
    T_uderror_ind                   tuderrorind;
    T_unitdata_ind                  tunitdataind;
    T_unitdata_req                  tunitdatareq;
    T_unitreply_ind                 tunitreplyind;
    T_unitrequest_ind               tunitrequestind;
    T_unitrequest_req               tunitrequestreq;
    T_unitreply_req                 tunitreplyreq;
    T_unitreply_ack                 tunitreplyack;
    T_reply_ind                     treplyind;
    T_request_ind                   trequestind;
    T_request_req                   trequestreq;
    T_reply_req                     treplyreq;
    T_reply_ack                     treplyack;
    T_cancelrequest_req             tcancelreqreq;
    T_resolveaddr_req               tresolvereq;
    T_resolveaddr_ack               tresolveack;
    T_regname_req                   tregnamereq;
    T_regname_ack                   tregnameack;
    T_delname_req                   tdelnamereq;
    T_lkupname_req                  tlkupnamereq;
    T_lkupname_con                  tlkupnamecon;
    T_sequence_ack                  tsequenceack;
    T_event_ind                     teventind;
};
typedef union T_primitives              T_primitives;
/* ***** From the Mentat "dlpi.h" ******/

/*
   This header file has encoded the values so an existing driver
   or user which was written with the Logical Link Interface(LLI)
   can migrate to the DLPI interface in a binary compatible manner.
   Any fields which require a specific format or value are flagged
   with a comment containing the message LLI compatibility.
*/

/* DLPI revision definition history*/

enum {
    DL_CURRENT_VERSION          = 0x02,                         /* current version of dlpi */
    DL_VERSION_2                = 0x02                          /* version of dlpi March 12,1991 */
};


enum {
    DL_INFO_REQ                 = 0x00,                         /* Information Req, LLI compatibility */
    DL_INFO_ACK                 = 0x03,                         /* Information Ack, LLI compatibility */
    DL_ATTACH_REQ               = 0x0B,                         /* Attach a PPA */
    DL_DETACH_REQ               = 0x0C,                         /* Detach a PPA */
    DL_BIND_REQ                 = 0x01,                         /* Bind dlsap address, LLI compatibility */
    DL_BIND_ACK                 = 0x04,                         /* Dlsap address bound, LLI compatibility */
    DL_UNBIND_REQ               = 0x02,                         /* Unbind dlsap address, LLI compatibility */
    DL_OK_ACK                   = 0x06,                         /* Success acknowledgment, LLI compatibility */
    DL_ERROR_ACK                = 0x05,                         /* Error acknowledgment, LLI compatibility */
    DL_SUBS_BIND_REQ            = 0x1B,                         /* Bind Subsequent DLSAP address */
    DL_SUBS_BIND_ACK            = 0x1C,                         /* Subsequent DLSAP address bound */
    DL_SUBS_UNBIND_REQ          = 0x15,                         /* Subsequent unbind */
    DL_ENABMULTI_REQ            = 0x1D,                         /* Enable multicast addresses */
    DL_DISABMULTI_REQ           = 0x1E,                         /* Disable multicast addresses */
    DL_PROMISCON_REQ            = 0x1F,                         /* Turn on promiscuous mode */
    DL_PROMISCOFF_REQ           = 0x20,                         /* Turn off promiscuous mode */
    DL_UNITDATA_REQ             = 0x07,                         /* datagram send request, LLI compatibility */
    DL_UNITDATA_IND             = 0x08,                         /* datagram receive indication, LLI compatibility */
    DL_UDERROR_IND              = 0x09,                         /* datagram error indication, LLI compatibility */
    DL_UDQOS_REQ                = 0x0A,                         /* set QOS for subsequent datagram transmissions */
    DL_CONNECT_REQ              = 0x0D,                         /* Connect request */
    DL_CONNECT_IND              = 0x0E,                         /* Incoming connect indication */
    DL_CONNECT_RES              = 0x0F,                         /* Accept previous connect indication */
    DL_CONNECT_CON              = 0x10,                         /* Connection established */
    DL_TOKEN_REQ                = 0x11,                         /* Passoff token request */
    DL_TOKEN_ACK                = 0x12,                         /* Passoff token ack */
    DL_DISCONNECT_REQ           = 0x13,                         /* Disconnect request */
    DL_DISCONNECT_IND           = 0x14,                         /* Disconnect indication */
    DL_RESET_REQ                = 0x17,                         /* Reset service request */
    DL_RESET_IND                = 0x18,                         /* Incoming reset indication */
    DL_RESET_RES                = 0x19,                         /* Complete reset processing */
    DL_RESET_CON                = 0x1A,                         /* Reset processing complete */
    DL_DATA_ACK_REQ             = 0x21,                         /* data unit transmission request */
    DL_DATA_ACK_IND             = 0x22,                         /* Arrival of a command PDU */
    DL_DATA_ACK_STATUS_IND      = 0x23,                         /* Status indication of DATA_ACK_REQ*/
    DL_REPLY_REQ                = 0x24,                         /* Request a DLSDU from the remote */
    DL_REPLY_IND                = 0x25,                         /* Arrival of a command PDU */
    DL_REPLY_STATUS_IND         = 0x26,                         /* Status indication of REPLY_REQ */
    DL_REPLY_UPDATE_REQ         = 0x27,                         /* Hold a DLSDU for transmission */
    DL_REPLY_UPDATE_STATUS_IND  = 0x28,                         /* Status of REPLY_UPDATE req */
    DL_XID_REQ                  = 0x29,                         /* Request to send an XID PDU */
    DL_XID_IND                  = 0x2A,                         /* Arrival of an XID PDU */
    DL_XID_RES                  = 0x2B,                         /* request to send a response XID PDU*/
    DL_XID_CON                  = 0x2C,                         /* Arrival of a response XID PDU */
    DL_TEST_REQ                 = 0x2D,                         /* TEST command request */
    DL_TEST_IND                 = 0x2E,                         /* TEST response indication */
    DL_TEST_RES                 = 0x2F,                         /* TEST response */
    DL_TEST_CON                 = 0x30,                         /* TEST Confirmation */
    DL_PHYS_ADDR_REQ            = 0x31,                         /* Request to get physical addr */
    DL_PHYS_ADDR_ACK            = 0x32,                         /* Return physical addr */
    DL_SET_PHYS_ADDR_REQ        = 0x33,                         /* set physical addr */
    DL_GET_STATISTICS_REQ       = 0x34,                         /* Request to get statistics */
    DL_GET_STATISTICS_ACK       = 0x35                          /* Return statistics */
};

/* DLPI interface states*/
enum {
    DL_UNATTACHED               = 0x04,                         /* PPA not attached */
    DL_ATTACH_PENDING           = 0x05,                         /* Waiting ack of DL_ATTACH_REQ */
    DL_DETACH_PENDING           = 0x06,                         /* Waiting ack of DL_DETACH_REQ */
    DL_UNBOUND                  = 0x00,                         /* PPA attached, LLI compatibility */
    DL_BIND_PENDING             = 0x01,                         /* Waiting ack of DL_BIND_REQ, LLI compatibility */
    DL_UNBIND_PENDING           = 0x02,                         /* Waiting ack of DL_UNBIND_REQ, LLI compatibility */
    DL_IDLE                     = 0x03,                         /* dlsap bound, awaiting use, LLI compatibility */
    DL_UDQOS_PENDING            = 0x07,                         /* Waiting ack of DL_UDQOS_REQ */
    DL_OUTCON_PENDING           = 0x08,                         /* outgoing connection, awaiting DL_CONN_CON */
    DL_INCON_PENDING            = 0x09,                         /* incoming connection, awaiting DL_CONN_RES */
    DL_CONN_RES_PENDING         = 0x0A,                         /* Waiting ack of DL_CONNECT_RES */
    DL_DATAXFER                 = 0x0B,                         /* connection-oriented data transfer */
    DL_USER_RESET_PENDING       = 0x0C,                         /* user initiated reset, awaiting DL_RESET_CON */
    DL_PROV_RESET_PENDING       = 0x0D,                         /* provider initiated reset, awaiting DL_RESET_RES */
    DL_RESET_RES_PENDING        = 0x0E,                         /* Waiting ack of DL_RESET_RES */
    DL_DISCON8_PENDING          = 0x0F,                         /* Waiting ack of DL_DISC_REQ when in DL_OUTCON_PENDING */
    DL_DISCON9_PENDING          = 0x10,                         /* Waiting ack of DL_DISC_REQ when in DL_INCON_PENDING */
    DL_DISCON11_PENDING         = 0x11,                         /* Waiting ack of DL_DISC_REQ when in DL_DATAXFER */
    DL_DISCON12_PENDING         = 0x12,                         /* Waiting ack of DL_DISC_REQ when in DL_USER_RESET_PENDING */
    DL_DISCON13_PENDING         = 0x13,                         /* Waiting ack of DL_DISC_REQ when in DL_DL_PROV_RESET_PENDING */
    DL_SUBS_BIND_PND            = 0x14,                         /* Waiting ack of DL_SUBS_BIND_REQ */
    DL_SUBS_UNBIND_PND          = 0x15                          /* Waiting ack of DL_SUBS_UNBIND_REQ */
};

/* DL_ERROR_ACK error return values*/

enum {
    DL_ACCESS                   = 0x02,                         /* Improper permissions for request, LLI compatibility */
    DL_BADADDR                  = 0x01,                         /* DLSAP address in improper format or invalid */
    DL_BADCORR                  = 0x05,                         /* Sequence number not from outstanding DL_CONN_IND */
    DL_BADDATA                  = 0x06,                         /* User data exceeded provider limit */
    DL_BADPPA                   = 0x08,                         /* Specified PPA was invalid */
    DL_BADPRIM                  = 0x09,                         /* Primitive received is not known by DLS provider */
    DL_BADQOSPARAM              = 0x0A,                         /* QOS parameters contained invalid values */
    DL_BADQOSTYPE               = 0x0B,                         /* QOS structure type is unknown or unsupported */
    DL_BADSAP                   = 0x00,                         /* Bad LSAP selector, LLI compatibility */
    DL_BADTOKEN                 = 0x0C,                         /* Token used not associated with an active stream */
    DL_BOUND                    = 0x0D,                         /* Attempted second bind with dl_max_conind or    */
                                                                /*    dl_conn_mgmt > 0 on same DLSAP or PPA */
    DL_INITFAILED               = 0x0E,                         /* Physical Link initialization failed */
    DL_NOADDR                   = 0x0F,                         /* Provider couldn't allocate alternate address */
    DL_NOTINIT                  = 0x10,                         /* Physical Link not initialized */
    DL_OUTSTATE                 = 0x03,                         /* Primitive issued in improper state, LLI compatibility */
    DL_SYSERR                   = 0x04,                         /* UNIX system error occurred, LLI compatibility */
    DL_UNSUPPORTED              = 0x07,                         /* Requested service not supplied by provider */
    DL_UNDELIVERABLE            = 0x11,                         /* Previous data unit could not be delivered */
    DL_NOTSUPPORTED             = 0x12,                         /* Primitive is known but not supported by DLS provider */
    DL_TOOMANY                  = 0x13,                         /* limit exceeded */
    DL_NOTENAB                  = 0x14,                         /* Promiscuous mode not enabled */
    DL_BUSY                     = 0x15,                         /* Other streams for a particular PPA in the post-attached state */
    DL_NOAUTO                   = 0x16,                         /* Automatic handling of XID & TEST responses not supported */
    DL_NOXIDAUTO                = 0x17,                         /* Automatic handling of XID not supported */
    DL_NOTESTAUTO               = 0x18,                         /* Automatic handling of TEST not supported */
    DL_XIDAUTO                  = 0x19,                         /* Automatic handling of XID response */
    DL_TESTAUTO                 = 0x1A,                         /* AUtomatic handling of TEST response*/
    DL_PENDING                  = 0x1B                          /* pending outstanding connect indications */
};

/* DLPI media types supported*/

enum {
    DL_CSMACD                   = 0x00,                         /* IEEE 802.3 CSMA/CD network, LLI Compatibility */
    DL_TPB                      = 0x01,                         /* IEEE 802.4 Token Passing Bus, LLI Compatibility */
    DL_TPR                      = 0x02,                         /* IEEE 802.5 Token Passing Ring, LLI Compatibility */
    DL_METRO                    = 0x03,                         /* IEEE 802.6 Metro Net, LLI Compatibility */
    DL_ETHER                    = 0x04,                         /* Ethernet Bus, LLI Compatibility */
    DL_HDLC                     = 0x05,                         /* ISO HDLC protocol support, bit synchronous */
    DL_CHAR                     = 0x06,                         /* Character Synchronous protocol support, eg BISYNC */
    DL_CTCA                     = 0x07,                         /* IBM Channel-to-Channel Adapter */
    DL_FDDI                     = 0x08,                         /* Fiber Distributed data interface */
    DL_OTHER                    = 0x09                          /* Any other medium not listed above */
};

/*
   DLPI provider service supported.
   These must be allowed to be bitwise-OR for dl_service_mode in
   DL_INFO_ACK.
*/
enum {
    DL_CODLS                    = 0x01,                         /* support connection-oriented service */
    DL_CLDLS                    = 0x02,                         /* support connectionless data link service */
    DL_ACLDLS                   = 0x04                          /* support acknowledged connectionless service*/
};

/*
   DLPI provider style.
   The DLPI provider style which determines whether a provider
   requires a DL_ATTACH_REQ to inform the provider which PPA
   user messages should be sent/received on.
*/

enum {
    DL_STYLE1                   = 0x0500,                       /* PPA is implicitly bound by open(2) */
    DL_STYLE2                   = 0x0501                        /* PPA must be explicitly bound via DL_ATTACH_REQ */
};

/* DLPI Originator for Disconnect and Resets*/

enum {
    DL_PROVIDER                 = 0x0700,
    DL_USER                     = 0x0701
};

/* DLPI Disconnect Reasons*/

enum {
    DL_CONREJ_DEST_UNKNOWN      = 0x0800,
    DL_CONREJ_DEST_UNREACH_PERMANENT = 0x0801,
    DL_CONREJ_DEST_UNREACH_TRANSIENT = 0x0802,
    DL_CONREJ_QOS_UNAVAIL_PERMANENT = 0x0803,
    DL_CONREJ_QOS_UNAVAIL_TRANSIENT = 0x0804,
    DL_CONREJ_PERMANENT_COND    = 0x0805,
    DL_CONREJ_TRANSIENT_COND    = 0x0806,
    DL_DISC_ABNORMAL_CONDITION  = 0x0807,
    DL_DISC_NORMAL_CONDITION    = 0x0808,
    DL_DISC_PERMANENT_CONDITION = 0x0809,
    DL_DISC_TRANSIENT_CONDITION = 0x080A,
    DL_DISC_UNSPECIFIED         = 0x080B
};

/* DLPI Reset Reasons*/

enum {
    DL_RESET_FLOW_CONTROL       = 0x0900,
    DL_RESET_LINK_ERROR         = 0x0901,
    DL_RESET_RESYNCH            = 0x0902
};

/* DLPI status values for acknowledged connectionless data transfer*/

enum {
    DL_CMD_MASK                 = 0x0F,                         /* mask for command portion of status */
    DL_CMD_OK                   = 0x00,                         /* Command Accepted */
    DL_CMD_RS                   = 0x01,                         /* Unimplemented or inactivated service */
    DL_CMD_UE                   = 0x05,                         /* Data Link User interface error */
    DL_CMD_PE                   = 0x06,                         /* Protocol error */
    DL_CMD_IP                   = 0x07,                         /* Permanent implementation dependent error*/
    DL_CMD_UN                   = 0x09,                         /* Resources temporarily unavailable */
    DL_CMD_IT                   = 0x0F,                         /* Temporary implementation dependent error */
    DL_RSP_MASK                 = 0xF0,                         /* mask for response portion of status */
    DL_RSP_OK                   = 0x00,                         /* Response DLSDU present */
    DL_RSP_RS                   = 0x10,                         /* Unimplemented or inactivated service */
    DL_RSP_NE                   = 0x30,                         /* Response DLSDU never submitted */
    DL_RSP_NR                   = 0x40,                         /* Response DLSDU not requested */
    DL_RSP_UE                   = 0x50,                         /* Data Link User interface error */
    DL_RSP_IP                   = 0x70,                         /* Permanent implementation dependent error */
    DL_RSP_UN                   = 0x90,                         /* Resources temporarily unavailable */
    DL_RSP_IT                   = 0xF0                          /* Temporary implementation dependent error */
};

/* Service Class values for acknowledged connectionless data transfer*/

enum {
    DL_RQST_RSP                 = 0x01,                         /* Use acknowledge capability in MAC sublayer*/
    DL_RQST_NORSP               = 0x02                          /* No acknowledgement service requested */
};

/* DLPI address type definition*/

enum {
    DL_FACT_PHYS_ADDR           = 0x01,                         /* factory physical address */
    DL_CURR_PHYS_ADDR           = 0x02                          /* current physical address */
};

/* DLPI flag definitions*/

enum {
    DL_POLL_FINAL               = 0x01                          /* if set,indicates poll/final bit set*/
};

/* XID and TEST responses supported by the provider*/

enum {
    DL_AUTO_XID                 = 0x01,                         /* provider will respond to XID */
    DL_AUTO_TEST                = 0x02                          /* provider will respond to TEST */
};

/* Subsequent bind type*/

enum {
    DL_PEER_BIND                = 0x01,                         /* subsequent bind on a peer addr */
    DL_HIERARCHICAL_BIND        = 0x02                          /* subs_bind on a hierarchical addr*/
};

/* DLPI promiscuous mode definitions*/

enum {
    DL_PROMISC_PHYS             = 0x01,                         /* promiscuous mode at phys level */
    DL_PROMISC_SAP              = 0x02,                         /* promiscous mode at sap level */
    DL_PROMISC_MULTI            = 0x03                          /* promiscuous mode for multicast */
};

/*
   DLPI Quality Of Service definition for use in QOS structure definitions.
   The QOS structures are used in connection establishment, DL_INFO_ACK,
   and setting connectionless QOS values.
*/

/*
   Throughput
   
   This parameter is specified for both directions.
*/


struct dl_through_t {
    SInt32                          dl_target_value;            /* desired bits/second desired */
    SInt32                          dl_accept_value;            /* min. acceptable bits/second */
};
typedef struct dl_through_t             dl_through_t;
/*
   transit delay specification
   
   This parameter is specified for both directions.
   expressed in milliseconds assuming a DLSDU size of 128 octets.
   The scaling of the value to the current DLSDU size is provider dependent.
*/

struct dl_transdelay_t {
    SInt32                          dl_target_value;            /* desired value of service */
    SInt32                          dl_accept_value;            /* min. acceptable value of service */
};
typedef struct dl_transdelay_t          dl_transdelay_t;
/*
   priority specification
   priority range is 0-100, with 0 being highest value.
*/


struct dl_priority_t {
    SInt32                          dl_min;
    SInt32                          dl_max;
};
typedef struct dl_priority_t            dl_priority_t;
/* protection specification*/
enum {
    DL_NONE                     = 0x0B01,                       /* no protection supplied */
    DL_MONITOR                  = 0x0B02,                       /* protection against passive monitoring */
    DL_MAXIMUM                  = 0x0B03                        /* protection against modification, replay, addition, or deletion */
};


struct dl_protect_t {
    SInt32                          dl_min;
    SInt32                          dl_max;
};
typedef struct dl_protect_t             dl_protect_t;
/*
   Resilience specification
   probabilities are scaled by a factor of 10,000 with a time interval
   of 10,000 seconds.
*/

struct dl_resilience_t {
    SInt32                          dl_disc_prob;               /* probability of provider init DISC */
    SInt32                          dl_reset_prob;              /* probability of provider init RESET */
};
typedef struct dl_resilience_t          dl_resilience_t;
/*
    QOS type definition to be used for negotiation with the
    remote end of a connection, or a connectionless unitdata request.
    There are two type definitions to handle the negotiation 
    process at connection establishment. The typedef dl_qos_range_t
    is used to present a range for parameters. This is used
    in the DL_CONNECT_REQ and DL_CONNECT_IND messages. The typedef
    dl_qos_sel_t is used to select a specific value for the QOS
    parameters. This is used in the DL_CONNECT_RES, DL_CONNECT_CON,
    and DL_INFO_ACK messages to define the selected QOS parameters
    for a connection.

    NOTE
    A DataLink provider which has unknown values for any of the fields
    will use a value of DL_UNKNOWN for all values in the fields.

    NOTE
    A QOS parameter value of DL_QOS_DONT_CARE informs the DLS
    provider the user requesting this value doesn't care 
    what the QOS parameter is set to. This value becomes the
    least possible value in the range of QOS parameters.
    The order of the QOS parameter range is then:

        DL_QOS_DONT_CARE < 0 < MAXIMUM QOS VALUE
*/
enum {
    DL_UNKNOWN                  = -1,
    DL_QOS_DONT_CARE            = -2
};

/*
    Every QOS structure has the first 4 bytes containing a type
    field, denoting the definition of the rest of the structure.
    This is used in the same manner has the dl_primitive variable
    is in messages.

    The following list is the defined QOS structure type values and structures.
*/
enum {
    DL_QOS_CO_RANGE1            = 0x0101,                       /* QOS range struct. for Connection modeservice */
    DL_QOS_CO_SEL1              = 0x0102,                       /* QOS selection structure */
    DL_QOS_CL_RANGE1            = 0x0103,                       /* QOS range struct. for connectionless*/
    DL_QOS_CL_SEL1              = 0x0104                        /* QOS selection for connectionless mode*/
};


struct dl_qos_co_range1_t {
    UInt32                          dl_qos_type;
    dl_through_t                    dl_rcv_throughput;          /* desired and acceptable*/
    dl_transdelay_t                 dl_rcv_trans_delay;         /* desired and acceptable*/
    dl_through_t                    dl_xmt_throughput;
    dl_transdelay_t                 dl_xmt_trans_delay;
    dl_priority_t                   dl_priority;                /* min and max values */
    dl_protect_t                    dl_protection;              /* min and max values */
    SInt32                          dl_residual_error;
    dl_resilience_t                 dl_resilience;
};
typedef struct dl_qos_co_range1_t       dl_qos_co_range1_t;

struct dl_qos_co_sel1_t {
    UInt32                          dl_qos_type;
    SInt32                          dl_rcv_throughput;
    SInt32                          dl_rcv_trans_delay;
    SInt32                          dl_xmt_throughput;
    SInt32                          dl_xmt_trans_delay;
    SInt32                          dl_priority;
    SInt32                          dl_protection;
    SInt32                          dl_residual_error;
    dl_resilience_t                 dl_resilience;
};
typedef struct dl_qos_co_sel1_t         dl_qos_co_sel1_t;

struct dl_qos_cl_range1_t {
    UInt32                          dl_qos_type;
    dl_transdelay_t                 dl_trans_delay;
    dl_priority_t                   dl_priority;
    dl_protect_t                    dl_protection;
    SInt32                          dl_residual_error;
};
typedef struct dl_qos_cl_range1_t       dl_qos_cl_range1_t;

struct dl_qos_cl_sel1_t {
    UInt32                          dl_qos_type;
    SInt32                          dl_trans_delay;
    SInt32                          dl_priority;
    SInt32                          dl_protection;
    SInt32                          dl_residual_error;
};
typedef struct dl_qos_cl_sel1_t         dl_qos_cl_sel1_t;
/*
    DLPI interface primitive definitions.

    Each primitive is sent as a stream message. It is possible that
    the messages may be viewed as a sequence of bytes that have the
    following form without any padding. The structure definition
    of the following messages may have to change depending on the
    underlying hardware architecture and crossing of a hardware
    boundary with a different hardware architecture.

    Fields in the primitives having a name of the form
    dl_reserved cannot be used and have the value of
    binary zero, no bits turned on.

    Each message has the name defined followed by the
    stream message type (M_PROTO, M_PCPROTO, M_DATA)
 */
/* LOCAL MANAGEMENT SERVICE PRIMITIVES*/

/* DL_INFO_REQ, M_PCPROTO type*/


struct dl_info_req_t {
    UInt32                          dl_primitive;               /* set to DL_INFO_REQ */
};
typedef struct dl_info_req_t            dl_info_req_t;
/* DL_INFO_ACK, M_PCPROTO type*/

struct dl_info_ack_t {
    UInt32                          dl_primitive;               /* set to DL_INFO_ACK */
    UInt32                          dl_max_sdu;                 /* Max bytes in a DLSDU */
    UInt32                          dl_min_sdu;                 /* Min bytes in a DLSDU */
    UInt32                          dl_addr_length;             /* length of DLSAP address */
    UInt32                          dl_mac_type;                /* type of medium supported*/
    UInt32                          dl_reserved;                /* value set to zero */
    UInt32                          dl_current_state;           /* state of DLPI interface */
    SInt32                          dl_sap_length;              /* current length of SAP part of dlsap address */
    UInt32                          dl_service_mode;            /* CO, CL or ACL */
    UInt32                          dl_qos_length;              /* length of qos values */
    UInt32                          dl_qos_offset;              /* offset from beg. of block*/
    UInt32                          dl_qos_range_length;        /* available range of qos */
    UInt32                          dl_qos_range_offset;        /* offset from beg. of block*/
    UInt32                          dl_provider_style;          /* style1 or style2 */
    UInt32                          dl_addr_offset;             /* offset of the dlsap addr */
    UInt32                          dl_version;                 /* version number */
    UInt32                          dl_brdcst_addr_length;      /* length of broadcast addr */
    UInt32                          dl_brdcst_addr_offset;      /* offset from beg. of block*/
    UInt32                          dl_growth;                  /* set to zero */
};
typedef struct dl_info_ack_t            dl_info_ack_t;
/* DL_ATTACH_REQ, M_PROTO type*/

struct dl_attach_req_t {
    UInt32                          dl_primitive;               /* set to DL_ATTACH_REQ*/
    UInt32                          dl_ppa;                     /* id of the PPA */
};
typedef struct dl_attach_req_t          dl_attach_req_t;
/* DL_DETACH_REQ, M_PROTO type*/

struct dl_detach_req_t {
    UInt32                          dl_primitive;               /* set to DL_DETACH_REQ */
};
typedef struct dl_detach_req_t          dl_detach_req_t;
/* DL_BIND_REQ, M_PROTO type*/

struct dl_bind_req_t {
    UInt32                          dl_primitive;               /* set to DL_BIND_REQ */
    UInt32                          dl_sap;                     /* info to identify dlsap addr*/
    UInt32                          dl_max_conind;              /* max # of outstanding con_ind*/
    UInt16                          dl_service_mode;            /* CO, CL or ACL */
    UInt16                          dl_conn_mgmt;               /* if non-zero, is con-mgmt stream*/
    UInt32                          dl_xidtest_flg;             /* if set to 1 indicates automatic initiation of test and xid frames */
};
typedef struct dl_bind_req_t            dl_bind_req_t;
/* DL_BIND_ACK, M_PCPROTO type*/

struct dl_bind_ack_t {
    UInt32                          dl_primitive;               /* DL_BIND_ACK */
    UInt32                          dl_sap;                     /* DLSAP addr info */
    UInt32                          dl_addr_length;             /* length of complete DLSAP addr */
    UInt32                          dl_addr_offset;             /* offset from beginning of M_PCPROTO*/
    UInt32                          dl_max_conind;              /* allowed max. # of con-ind */
    UInt32                          dl_xidtest_flg;             /* responses supported by provider*/
};
typedef struct dl_bind_ack_t            dl_bind_ack_t;
/* DL_SUBS_BIND_REQ, M_PROTO type*/

struct dl_subs_bind_req_t {
    UInt32                          dl_primitive;               /* DL_SUBS_BIND_REQ */
    UInt32                          dl_subs_sap_offset;         /* offset of subs_sap */
    UInt32                          dl_subs_sap_length;         /* length of subs_sap */
    UInt32                          dl_subs_bind_class;         /* peer or hierarchical */
};
typedef struct dl_subs_bind_req_t       dl_subs_bind_req_t;
/* DL_SUBS_BIND_ACK, M_PCPROTO type*/

struct dl_subs_bind_ack_t {
    UInt32                          dl_primitive;               /* DL_SUBS_BIND_ACK */
    UInt32                          dl_subs_sap_offset;         /* offset of subs_sap */
    UInt32                          dl_subs_sap_length;         /* length of subs_sap */
};
typedef struct dl_subs_bind_ack_t       dl_subs_bind_ack_t;
/* DL_UNBIND_REQ, M_PROTO type*/

struct dl_unbind_req_t {
    UInt32                          dl_primitive;               /* DL_UNBIND_REQ */
};
typedef struct dl_unbind_req_t          dl_unbind_req_t;
/* DL_SUBS_UNBIND_REQ, M_PROTO type*/

struct dl_subs_unbind_req_t {
    UInt32                          dl_primitive;               /* DL_SUBS_UNBIND_REQ */
    UInt32                          dl_subs_sap_offset;         /* offset of subs_sap */
    UInt32                          dl_subs_sap_length;         /* length of subs_sap */
};
typedef struct dl_subs_unbind_req_t     dl_subs_unbind_req_t;
/* DL_OK_ACK, M_PCPROTO type*/

struct dl_ok_ack_t {
    UInt32                          dl_primitive;               /* DL_OK_ACK */
    UInt32                          dl_correct_primitive;       /* primitive being acknowledged */
};
typedef struct dl_ok_ack_t              dl_ok_ack_t;
/* DL_ERROR_ACK, M_PCPROTO type*/

struct dl_error_ack_t {
    UInt32                          dl_primitive;               /* DL_ERROR_ACK */
    UInt32                          dl_error_primitive;         /* primitive in error */
    UInt32                          dl_errno;                   /* DLPI error code */
    UInt32                          dl_unix_errno;              /* UNIX system error code */
};
typedef struct dl_error_ack_t           dl_error_ack_t;
/* DL_ENABMULTI_REQ, M_PROTO type*/

struct dl_enabmulti_req_t {
    UInt32                          dl_primitive;               /* DL_ENABMULTI_REQ */
    UInt32                          dl_addr_length;             /* length of multicast address */
    UInt32                          dl_addr_offset;             /* offset from beg. of M_PROTO block*/
};
typedef struct dl_enabmulti_req_t       dl_enabmulti_req_t;
/* DL_DISABMULTI_REQ, M_PROTO type*/

struct dl_disabmulti_req_t {
    UInt32                          dl_primitive;               /* DL_DISABMULTI_REQ */
    UInt32                          dl_addr_length;             /* length of multicast address */
    UInt32                          dl_addr_offset;             /* offset from beg. of M_PROTO block*/
};
typedef struct dl_disabmulti_req_t      dl_disabmulti_req_t;
/* DL_PROMISCON_REQ, M_PROTO type*/

struct dl_promiscon_req_t {
    UInt32                          dl_primitive;               /* DL_PROMISCON_REQ */
    UInt32                          dl_level;                   /* physical,SAP level or ALL multicast*/
};
typedef struct dl_promiscon_req_t       dl_promiscon_req_t;
/* DL_PROMISCOFF_REQ, M_PROTO type*/

struct dl_promiscoff_req_t {
    UInt32                          dl_primitive;               /* DL_PROMISCOFF_REQ */
    UInt32                          dl_level;                   /* Physical,SAP level or ALL multicast*/
};
typedef struct dl_promiscoff_req_t      dl_promiscoff_req_t;
/* Primitives to get and set the Physical address*/
/* DL_PHYS_ADDR_REQ, M_PROTO type*/


struct dl_phys_addr_req_t {
    UInt32                          dl_primitive;               /* DL_PHYS_ADDR_REQ */
    UInt32                          dl_addr_type;               /* factory or current physical addr */
};
typedef struct dl_phys_addr_req_t       dl_phys_addr_req_t;
/* DL_PHYS_ADDR_ACK, M_PCPROTO type*/

struct dl_phys_addr_ack_t {
    UInt32                          dl_primitive;               /* DL_PHYS_ADDR_ACK */
    UInt32                          dl_addr_length;             /* length of the physical addr */
    UInt32                          dl_addr_offset;             /* offset from beg. of block */
};
typedef struct dl_phys_addr_ack_t       dl_phys_addr_ack_t;
/* DL_SET_PHYS_ADDR_REQ, M_PROTO type*/

struct dl_set_phys_addr_req_t {
    UInt32                          dl_primitive;               /* DL_SET_PHYS_ADDR_REQ */
    UInt32                          dl_addr_length;             /* length of physical addr */
    UInt32                          dl_addr_offset;             /* offset from beg. of block */
};
typedef struct dl_set_phys_addr_req_t   dl_set_phys_addr_req_t;
/* Primitives to get statistics*/
/* DL_GET_STATISTICS_REQ, M_PROTO type*/


struct dl_get_statistics_req_t {
    UInt32                          dl_primitive;               /* DL_GET_STATISTICS_REQ */
};
typedef struct dl_get_statistics_req_t  dl_get_statistics_req_t;
/* DL_GET_STATISTICS_ACK, M_PCPROTO type*/

struct dl_get_statistics_ack_t {
    UInt32                          dl_primitive;               /* DL_GET_STATISTICS_ACK */
    UInt32                          dl_stat_length;             /* length of statistics structure*/
    UInt32                          dl_stat_offset;             /* offset from beg. of block */
};
typedef struct dl_get_statistics_ack_t  dl_get_statistics_ack_t;
/* CONNECTION-ORIENTED SERVICE PRIMITIVES*/

/* DL_CONNECT_REQ, M_PROTO type*/


struct dl_connect_req_t {
    UInt32                          dl_primitive;               /* DL_CONNECT_REQ */
    UInt32                          dl_dest_addr_length;        /* len. of dlsap addr*/
    UInt32                          dl_dest_addr_offset;        /* offset */
    UInt32                          dl_qos_length;              /* len. of QOS parm val*/
    UInt32                          dl_qos_offset;              /* offset */
    UInt32                          dl_growth;                  /* set to zero */
};
typedef struct dl_connect_req_t         dl_connect_req_t;
/* DL_CONNECT_IND, M_PROTO type*/

struct dl_connect_ind_t {
    UInt32                          dl_primitive;               /* DL_CONNECT_IND */
    UInt32                          dl_correlation;             /* provider's correlation token*/
    UInt32                          dl_called_addr_length;      /* length of called address */
    UInt32                          dl_called_addr_offset;      /* offset from beginning of block */
    UInt32                          dl_calling_addr_length;     /* length of calling address */
    UInt32                          dl_calling_addr_offset;     /* offset from beginning of block */
    UInt32                          dl_qos_length;              /* length of qos structure */
    UInt32                          dl_qos_offset;              /* offset from beginning of block */
    UInt32                          dl_growth;                  /* set to zero */
};
typedef struct dl_connect_ind_t         dl_connect_ind_t;
/* DL_CONNECT_RES, M_PROTO type*/

struct dl_connect_res_t {
    UInt32                          dl_primitive;               /* DL_CONNECT_RES */
    UInt32                          dl_correlation;             /* provider's correlation token */
    UInt32                          dl_resp_token;              /* token associated with responding stream */
    UInt32                          dl_qos_length;              /* length of qos structure */
    UInt32                          dl_qos_offset;              /* offset from beginning of block */
    UInt32                          dl_growth;                  /* set to zero */
};
typedef struct dl_connect_res_t         dl_connect_res_t;
/* DL_CONNECT_CON, M_PROTO type*/

struct dl_connect_con_t {
    UInt32                          dl_primitive;               /* DL_CONNECT_CON*/
    UInt32                          dl_resp_addr_length;        /* length of responder's address */
    UInt32                          dl_resp_addr_offset;        /* offset from beginning of block*/
    UInt32                          dl_qos_length;              /* length of qos structure */
    UInt32                          dl_qos_offset;              /* offset from beginning of block*/
    UInt32                          dl_growth;                  /* set to zero */
};
typedef struct dl_connect_con_t         dl_connect_con_t;
/* DL_TOKEN_REQ, M_PCPROTO type*/

struct dl_token_req_t {
    UInt32                          dl_primitive;               /* DL_TOKEN_REQ */
};
typedef struct dl_token_req_t           dl_token_req_t;
/* DL_TOKEN_ACK, M_PCPROTO type*/

struct dl_token_ack_t {
    UInt32                          dl_primitive;               /* DL_TOKEN_ACK */
    UInt32                          dl_token;                   /* Connection response token associated with the stream */
};
typedef struct dl_token_ack_t           dl_token_ack_t;
/* DL_DISCONNECT_REQ, M_PROTO type*/

struct dl_disconnect_req_t {
    UInt32                          dl_primitive;               /* DL_DISCONNECT_REQ */
    UInt32                          dl_reason;                  /*normal, abnormal, perm. or transient*/
    UInt32                          dl_correlation;             /* association with connect_ind */
};
typedef struct dl_disconnect_req_t      dl_disconnect_req_t;
/* DL_DISCONNECT_IND, M_PROTO type*/

struct dl_disconnect_ind_t {
    UInt32                          dl_primitive;               /* DL_DISCONNECT_IND */
    UInt32                          dl_originator;              /* USER or PROVIDER */
    UInt32                          dl_reason;                  /* permanent or transient */
    UInt32                          dl_correlation;             /* association with connect_ind */
};
typedef struct dl_disconnect_ind_t      dl_disconnect_ind_t;
/* DL_RESET_REQ, M_PROTO type*/

struct dl_reset_req_t {
    UInt32                          dl_primitive;               /* DL_RESET_REQ */
};
typedef struct dl_reset_req_t           dl_reset_req_t;
/* DL_RESET_IND, M_PROTO type*/

struct dl_reset_ind_t {
    UInt32                          dl_primitive;               /* DL_RESET_IND */
    UInt32                          dl_originator;              /* Provider or User */
    UInt32                          dl_reason;                  /* flow control, link error or resynch*/
};
typedef struct dl_reset_ind_t           dl_reset_ind_t;
/* DL_RESET_RES, M_PROTO type*/

struct dl_reset_res_t {
    UInt32                          dl_primitive;               /* DL_RESET_RES */
};
typedef struct dl_reset_res_t           dl_reset_res_t;
/* DL_RESET_CON, M_PROTO type*/

struct dl_reset_con_t {
    UInt32                          dl_primitive;               /* DL_RESET_CON */
};
typedef struct dl_reset_con_t           dl_reset_con_t;
/* CONNECTIONLESS SERVICE PRIMITIVES*/
/* DL_UNITDATA_REQ, M_PROTO type, with M_DATA block(s)*/


struct dl_unitdata_req_t {
    UInt32                          dl_primitive;               /* DL_UNITDATA_REQ */
    UInt32                          dl_dest_addr_length;        /* DLSAP length of dest. user */
    UInt32                          dl_dest_addr_offset;        /* offset from beg. of block */
    dl_priority_t                   dl_priority;                /* priority value */
};
typedef struct dl_unitdata_req_t        dl_unitdata_req_t;
/* DL_UNITDATA_IND, M_PROTO type, with M_DATA block(s)*/

struct dl_unitdata_ind_t {
    UInt32                          dl_primitive;               /* DL_UNITDATA_IND */
    UInt32                          dl_dest_addr_length;        /* DLSAP length of dest. user */
    UInt32                          dl_dest_addr_offset;        /* offset from beg. of block */
    UInt32                          dl_src_addr_length;         /* DLSAP addr length of sending user*/
    UInt32                          dl_src_addr_offset;         /* offset from beg. of block */
    UInt32                          dl_group_address;           /* set to one if multicast/broadcast*/
};
typedef struct dl_unitdata_ind_t        dl_unitdata_ind_t;
/*
   DL_UDERROR_IND, M_PROTO type
   (or M_PCPROTO type if LLI-based provider)
*/

struct dl_uderror_ind_t {
    UInt32                          dl_primitive;               /* DL_UDERROR_IND */
    UInt32                          dl_dest_addr_length;        /* Destination DLSAP */
    UInt32                          dl_dest_addr_offset;        /* Offset from beg. of block */
    UInt32                          dl_unix_errno;              /* unix system error code*/
    UInt32                          dl_errno;                   /* DLPI error code */
};
typedef struct dl_uderror_ind_t         dl_uderror_ind_t;
/* DL_UDQOS_REQ, M_PROTO type*/

struct dl_udqos_req_t {
    UInt32                          dl_primitive;               /* DL_UDQOS_REQ */
    UInt32                          dl_qos_length;              /* length in bytes of requested qos*/
    UInt32                          dl_qos_offset;              /* offset from beg. of block */
};
typedef struct dl_udqos_req_t           dl_udqos_req_t;
/* Primitives to handle XID and TEST operations*/
/* DL_TEST_REQ, M_PROTO type*/


struct dl_test_req_t {
    UInt32                          dl_primitive;               /* DL_TEST_REQ */
    UInt32                          dl_flag;                    /* poll/final */
    UInt32                          dl_dest_addr_length;        /* DLSAP length of dest. user */
    UInt32                          dl_dest_addr_offset;        /* offset from beg. of block */
};
typedef struct dl_test_req_t            dl_test_req_t;
/* DL_TEST_IND, M_PROTO type*/

struct dl_test_ind_t {
    UInt32                          dl_primitive;               /* DL_TEST_IND */
    UInt32                          dl_flag;                    /* poll/final */
    UInt32                          dl_dest_addr_length;        /* dlsap length of dest. user */
    UInt32                          dl_dest_addr_offset;        /* offset from beg. of block */
    UInt32                          dl_src_addr_length;         /* dlsap length of source user */
    UInt32                          dl_src_addr_offset;         /* offset from beg. of block */
};
typedef struct dl_test_ind_t            dl_test_ind_t;
/* DL_TEST_RES, M_PROTO type*/

struct dl_test_res_t {
    UInt32                          dl_primitive;               /* DL_TEST_RES */
    UInt32                          dl_flag;                    /* poll/final */
    UInt32                          dl_dest_addr_length;        /* DLSAP length of dest. user */
    UInt32                          dl_dest_addr_offset;        /* offset from beg. of block */
};
typedef struct dl_test_res_t            dl_test_res_t;
/* DL_TEST_CON, M_PROTO type*/

struct dl_test_con_t {
    UInt32                          dl_primitive;               /* DL_TEST_CON */
    UInt32                          dl_flag;                    /* poll/final */
    UInt32                          dl_dest_addr_length;        /* dlsap length of dest. user */
    UInt32                          dl_dest_addr_offset;        /* offset from beg. of block */
    UInt32                          dl_src_addr_length;         /* dlsap length of source user */
    UInt32                          dl_src_addr_offset;         /* offset from beg. of block */
};
typedef struct dl_test_con_t            dl_test_con_t;
/* DL_XID_REQ, M_PROTO type*/

struct dl_xid_req_t {
    UInt32                          dl_primitive;               /* DL_XID_REQ */
    UInt32                          dl_flag;                    /* poll/final */
    UInt32                          dl_dest_addr_length;        /* dlsap length of dest. user */
    UInt32                          dl_dest_addr_offset;        /* offset from beg. of block */
};
typedef struct dl_xid_req_t             dl_xid_req_t;
/* DL_XID_IND, M_PROTO type*/

struct dl_xid_ind_t {
    UInt32                          dl_primitive;               /* DL_XID_IND */
    UInt32                          dl_flag;                    /* poll/final */
    UInt32                          dl_dest_addr_length;        /* dlsap length of dest. user */
    UInt32                          dl_dest_addr_offset;        /* offset from beg. of block */
    UInt32                          dl_src_addr_length;         /* dlsap length of source user */
    UInt32                          dl_src_addr_offset;         /* offset from beg. of block */
};
typedef struct dl_xid_ind_t             dl_xid_ind_t;
/* DL_XID_RES, M_PROTO type*/

struct dl_xid_res_t {
    UInt32                          dl_primitive;               /* DL_XID_RES */
    UInt32                          dl_flag;                    /* poll/final */
    UInt32                          dl_dest_addr_length;        /* DLSAP length of dest. user */
    UInt32                          dl_dest_addr_offset;        /* offset from beg. of block */
};
typedef struct dl_xid_res_t             dl_xid_res_t;
/* DL_XID_CON, M_PROTO type*/

struct dl_xid_con_t {
    UInt32                          dl_primitive;               /* DL_XID_CON */
    UInt32                          dl_flag;                    /* poll/final */
    UInt32                          dl_dest_addr_length;        /* dlsap length of dest. user */
    UInt32                          dl_dest_addr_offset;        /* offset from beg. of block */
    UInt32                          dl_src_addr_length;         /* dlsap length of source user */
    UInt32                          dl_src_addr_offset;         /* offset from beg. of block */
};
typedef struct dl_xid_con_t             dl_xid_con_t;
/* ACKNOWLEDGED CONNECTIONLESS SERVICE PRIMITIVES*/

/* DL_DATA_ACK_REQ, M_PROTO type*/


struct dl_data_ack_req_t {
    UInt32                          dl_primitive;               /* DL_DATA_ACK_REQ */
    UInt32                          dl_correlation;             /* User's correlation token */
    UInt32                          dl_dest_addr_length;        /* length of destination addr */
    UInt32                          dl_dest_addr_offset;        /* offset from beginning of block */
    UInt32                          dl_src_addr_length;         /* length of source address */
    UInt32                          dl_src_addr_offset;         /* offset from beginning of block */
    UInt32                          dl_priority;                /* priority */
    UInt32                          dl_service_class;           /* DL_RQST_RSP or DL_RQST_NORSP */
};
typedef struct dl_data_ack_req_t        dl_data_ack_req_t;
/* DL_DATA_ACK_IND, M_PROTO type*/

struct dl_data_ack_ind_t {
    UInt32                          dl_primitive;               /* DL_DATA_ACK_IND */
    UInt32                          dl_dest_addr_length;        /* length of destination addr */
    UInt32                          dl_dest_addr_offset;        /* offset from beginning of block */
    UInt32                          dl_src_addr_length;         /* length of source address */
    UInt32                          dl_src_addr_offset;         /* offset from beginning of block */
    UInt32                          dl_priority;                /* priority for data unit transm. */
    UInt32                          dl_service_class;           /* DL_RQST_RSP or DL_RQST_NORSP */
};
typedef struct dl_data_ack_ind_t        dl_data_ack_ind_t;
/* DL_DATA_ACK_STATUS_IND, M_PROTO type*/

struct dl_data_ack_status_ind_t {
    UInt32                          dl_primitive;               /* DL_DATA_ACK_STATUS_IND */
    UInt32                          dl_correlation;             /* User's correlation token */
    UInt32                          dl_status;                  /* success or failure of previous req*/
};
typedef struct dl_data_ack_status_ind_t dl_data_ack_status_ind_t;
/* DL_REPLY_REQ, M_PROTO type*/

struct dl_reply_req_t {
    UInt32                          dl_primitive;               /* DL_REPLY_REQ */
    UInt32                          dl_correlation;             /* User's correlation token */
    UInt32                          dl_dest_addr_length;        /* length of destination address */
    UInt32                          dl_dest_addr_offset;        /* offset from beginning of block */
    UInt32                          dl_src_addr_length;         /* source address length */
    UInt32                          dl_src_addr_offset;         /* offset from beginning of block */
    UInt32                          dl_priority;                /* priority for data unit transmission*/
    UInt32                          dl_service_class;
};
typedef struct dl_reply_req_t           dl_reply_req_t;
/* DL_REPLY_IND, M_PROTO type*/

struct dl_reply_ind_t {
    UInt32                          dl_primitive;               /* DL_REPLY_IND */
    UInt32                          dl_dest_addr_length;        /* length of destination address */
    UInt32                          dl_dest_addr_offset;        /* offset from beginning of block*/
    UInt32                          dl_src_addr_length;         /* length of source address */
    UInt32                          dl_src_addr_offset;         /* offset from beginning of block */
    UInt32                          dl_priority;                /* priority for data unit transmission*/
    UInt32                          dl_service_class;           /* DL_RQST_RSP or DL_RQST_NORSP */
};
typedef struct dl_reply_ind_t           dl_reply_ind_t;
/* DL_REPLY_STATUS_IND, M_PROTO type*/

struct dl_reply_status_ind_t {
    UInt32                          dl_primitive;               /* DL_REPLY_STATUS_IND */
    UInt32                          dl_correlation;             /* User's correlation token */
    UInt32                          dl_status;                  /* success or failure of previous req*/
};
typedef struct dl_reply_status_ind_t    dl_reply_status_ind_t;
/* DL_REPLY_UPDATE_REQ, M_PROTO type*/

struct dl_reply_update_req_t {
    UInt32                          dl_primitive;               /* DL_REPLY_UPDATE_REQ */
    UInt32                          dl_correlation;             /* user's correlation token */
    UInt32                          dl_src_addr_length;         /* length of source address */
    UInt32                          dl_src_addr_offset;         /* offset from beginning of block */
};
typedef struct dl_reply_update_req_t    dl_reply_update_req_t;
/* DL_REPLY_UPDATE_STATUS_IND, M_PROTO type*/

struct dl_reply_update_status_ind_t {
    UInt32                          dl_primitive;               /* DL_REPLY_UPDATE_STATUS_IND */
    UInt32                          dl_correlation;             /* User's correlation token */
    UInt32                          dl_status;                  /* success or failure of previous req*/
};
typedef struct dl_reply_update_status_ind_t dl_reply_update_status_ind_t;

union DL_primitives {
    UInt32                          dl_primitive;
    dl_info_req_t                   info_req;
    dl_info_ack_t                   info_ack;
    dl_attach_req_t                 attach_req;
    dl_detach_req_t                 detach_req;
    dl_bind_req_t                   bind_req;
    dl_bind_ack_t                   bind_ack;
    dl_unbind_req_t                 unbind_req;
    dl_subs_bind_req_t              subs_bind_req;
    dl_subs_bind_ack_t              subs_bind_ack;
    dl_subs_unbind_req_t            subs_unbind_req;
    dl_ok_ack_t                     ok_ack;
    dl_error_ack_t                  error_ack;
    dl_connect_req_t                connect_req;
    dl_connect_ind_t                connect_ind;
    dl_connect_res_t                connect_res;
    dl_connect_con_t                connect_con;
    dl_token_req_t                  token_req;
    dl_token_ack_t                  token_ack;
    dl_disconnect_req_t             disconnect_req;
    dl_disconnect_ind_t             disconnect_ind;
    dl_reset_req_t                  reset_req;
    dl_reset_ind_t                  reset_ind;
    dl_reset_res_t                  reset_res;
    dl_reset_con_t                  reset_con;
    dl_unitdata_req_t               unitdata_req;
    dl_unitdata_ind_t               unitdata_ind;
    dl_uderror_ind_t                uderror_ind;
    dl_udqos_req_t                  udqos_req;
    dl_enabmulti_req_t              enabmulti_req;
    dl_disabmulti_req_t             disabmulti_req;
    dl_promiscon_req_t              promiscon_req;
    dl_promiscoff_req_t             promiscoff_req;
    dl_phys_addr_req_t              physaddr_req;
    dl_phys_addr_ack_t              physaddr_ack;
    dl_set_phys_addr_req_t          set_physaddr_req;
    dl_get_statistics_req_t         get_statistics_req;
    dl_get_statistics_ack_t         get_statistics_ack;
    dl_test_req_t                   test_req;
    dl_test_ind_t                   test_ind;
    dl_test_res_t                   test_res;
    dl_test_con_t                   test_con;
    dl_xid_req_t                    xid_req;
    dl_xid_ind_t                    xid_ind;
    dl_xid_res_t                    xid_res;
    dl_xid_con_t                    xid_con;
    dl_data_ack_req_t               data_ack_req;
    dl_data_ack_ind_t               data_ack_ind;
    dl_data_ack_status_ind_t        data_ack_status_ind;
    dl_reply_req_t                  reply_req;
    dl_reply_ind_t                  reply_ind;
    dl_reply_status_ind_t           reply_status_ind;
    dl_reply_update_req_t           reply_update_req;
    dl_reply_update_status_ind_t    reply_update_status_ind;
};
typedef union DL_primitives             DL_primitives;
enum {
    DL_INFO_REQ_SIZE            = sizeof(dl_info_req_t),
    DL_INFO_ACK_SIZE            = sizeof(dl_info_ack_t),
    DL_ATTACH_REQ_SIZE          = sizeof(dl_attach_req_t),
    DL_DETACH_REQ_SIZE          = sizeof(dl_detach_req_t),
    DL_BIND_REQ_SIZE            = sizeof(dl_bind_req_t),
    DL_BIND_ACK_SIZE            = sizeof(dl_bind_ack_t),
    DL_UNBIND_REQ_SIZE          = sizeof(dl_unbind_req_t),
    DL_SUBS_BIND_REQ_SIZE       = sizeof(dl_subs_bind_req_t),
    DL_SUBS_BIND_ACK_SIZE       = sizeof(dl_subs_bind_ack_t),
    DL_SUBS_UNBIND_REQ_SIZE     = sizeof(dl_subs_unbind_req_t),
    DL_OK_ACK_SIZE              = sizeof(dl_ok_ack_t),
    DL_ERROR_ACK_SIZE           = sizeof(dl_error_ack_t),
    DL_CONNECT_REQ_SIZE         = sizeof(dl_connect_req_t),
    DL_CONNECT_IND_SIZE         = sizeof(dl_connect_ind_t),
    DL_CONNECT_RES_SIZE         = sizeof(dl_connect_res_t),
    DL_CONNECT_CON_SIZE         = sizeof(dl_connect_con_t),
    DL_TOKEN_REQ_SIZE           = sizeof(dl_token_req_t),
    DL_TOKEN_ACK_SIZE           = sizeof(dl_token_ack_t),
    DL_DISCONNECT_REQ_SIZE      = sizeof(dl_disconnect_req_t),
    DL_DISCONNECT_IND_SIZE      = sizeof(dl_disconnect_ind_t),
    DL_RESET_REQ_SIZE           = sizeof(dl_reset_req_t),
    DL_RESET_IND_SIZE           = sizeof(dl_reset_ind_t),
    DL_RESET_RES_SIZE           = sizeof(dl_reset_res_t),
    DL_RESET_CON_SIZE           = sizeof(dl_reset_con_t),
    DL_UNITDATA_REQ_SIZE        = sizeof(dl_unitdata_req_t),
    DL_UNITDATA_IND_SIZE        = sizeof(dl_unitdata_ind_t),
    DL_UDERROR_IND_SIZE         = sizeof(dl_uderror_ind_t),
    DL_UDQOS_REQ_SIZE           = sizeof(dl_udqos_req_t),
    DL_ENABMULTI_REQ_SIZE       = sizeof(dl_enabmulti_req_t),
    DL_DISABMULTI_REQ_SIZE      = sizeof(dl_disabmulti_req_t),
    DL_PROMISCON_REQ_SIZE       = sizeof(dl_promiscon_req_t),
    DL_PROMISCOFF_REQ_SIZE      = sizeof(dl_promiscoff_req_t),
    DL_PHYS_ADDR_REQ_SIZE       = sizeof(dl_phys_addr_req_t),
    DL_PHYS_ADDR_ACK_SIZE       = sizeof(dl_phys_addr_ack_t),
    DL_SET_PHYS_ADDR_REQ_SIZE   = sizeof(dl_set_phys_addr_req_t),
    DL_GET_STATISTICS_REQ_SIZE  = sizeof(dl_get_statistics_req_t),
    DL_GET_STATISTICS_ACK_SIZE  = sizeof(dl_get_statistics_ack_t),
    DL_XID_REQ_SIZE             = sizeof(dl_xid_req_t),
    DL_XID_IND_SIZE             = sizeof(dl_xid_ind_t),
    DL_XID_RES_SIZE             = sizeof(dl_xid_res_t),
    DL_XID_CON_SIZE             = sizeof(dl_xid_con_t),
    DL_TEST_REQ_SIZE            = sizeof(dl_test_req_t),
    DL_TEST_IND_SIZE            = sizeof(dl_test_ind_t),
    DL_TEST_RES_SIZE            = sizeof(dl_test_res_t),
    DL_TEST_CON_SIZE            = sizeof(dl_test_con_t),
    DL_DATA_ACK_REQ_SIZE        = sizeof(dl_data_ack_req_t),
    DL_DATA_ACK_IND_SIZE        = sizeof(dl_data_ack_ind_t),
    DL_DATA_ACK_STATUS_IND_SIZE = sizeof(dl_data_ack_status_ind_t),
    DL_REPLY_REQ_SIZE           = sizeof(dl_reply_req_t),
    DL_REPLY_IND_SIZE           = sizeof(dl_reply_ind_t),
    DL_REPLY_STATUS_IND_SIZE    = sizeof(dl_reply_status_ind_t),
    DL_REPLY_UPDATE_REQ_SIZE    = sizeof(dl_reply_update_req_t),
    DL_REPLY_UPDATE_STATUS_IND_SIZE = sizeof(dl_reply_update_status_ind_t)
};

enum {
    DL_IOC_HDR_INFO             = ((MIOC_DLPI << 8) | 10)       /* Fast path request */
};

/* ***** From the Mentat "modnames.h" ******/


#define MI_AFU_NAME         "afu"
#define MI_AHARP_NAME       "ahar"
#define MI_AHENET_NAME      "ahen"
#define MI_ARP_NAME         "arp"
#define MI_ARPM_NAME        "arpm"
#define MI_COURMUX_NAME     "courmux"
#define MI_CLONE_NAME       "clone"
#define MI_DLB_NAME         "dlb"
#define MI_DLM_NAME         "dlm"
#define MI_DMODD_NAME       "disdlpi"
#define MI_DMODT_NAME       "distpi"
#define MI_DN_NAME          "dn"
#define MI_DNF_NAME         "dnf"
#define MI_DRVE_NAME        "drve"
#define MI_ECHO_NAME        "echo"
#define MI_ENXR_NAME        "enxr"
#define MI_RAWIP_NAME       "rawip"
#define MI_RAWIPM_NAME      "rawipm"
#define MI_HAVOC_NAME       "havoc"
#define MI_HAVOCM_NAME      "havocm"
#define MI_IP_NAME          "ip"
#define MI_IPM_NAME         "ipm"
#define MI_IPX_NAME         "ipx"
#define MI_LOG_NAME         "log"
#define MI_MODE_NAME        "mode"
#define MI_MUX_NAME         "mux"
#define MI_NECHO_NAME       "necho"
#define MI_NPEP_NAME        "npep"
#define MI_NULS_NAME        "nuls"
#define MI_NULZ_NAME        "nulz"
#define MI_PASS_NAME        "pass"
#define MI_PIPEMOD_NAME     "pipemod"
#define MI_SAD_NAME         "sad"
#define MI_SC_NAME          "sc"
#define MI_SOCKMOD_NAME     "sockmod"
#define MI_SPASS_NAME       "spass"
#define MI_SPX_NAME         "spx"
#define MI_STH_NAME         "mi_sth"
#define MI_TCP_NAME         "tcp"
#define MI_TCPM_NAME        "tcpm"
#define MI_TIMOD_NAME       "timod"
#define MI_TIRDWR_NAME      "tirdwr"
#define MI_TMOD_NAME        "tmod"
#define MI_TMUX_NAME        "tmux"
#define MI_TPIT_NAME        "tpit"
#define MI_TRSR_NAME        "trsr"
#define MI_TRXR_NAME        "trxr"
#define MI_UDP_NAME         "udp"
#define MI_UDPM_NAME        "udpm"
#define MI_WELD_NAME        "mi_weld"
#define MI_XDG_NAME         "xdg"
#define MI_XECHO_NAME       "xecho"
#define MI_XF_NAME          "xf"
#define MI_XFIPX_NAME       "xfipx"
#define MI_XFXNS_NAME       "xfxns"
#define MI_XPE_NAME         "xpe"
#define MI_XS_NAME          "xs"
#define MI_XTINDG_NAME      "xtindg"
#define MI_XTINVC_NAME      "xtinvc"
#define MI_XTM_NAME         "xtm"
#define MI_XTMIP_NAME       "xtmip"

#define MI_AFU_DEVICE       "/dev/afu"
#define MI_ARP_DEVICE       "/dev/arp"
#define MI_COURMUX_DEVICE   "/dev/courmux"
#define MI_CLONE_DEVICE     "/dev/clone"
#define MI_DLB_DEVICE       "/dev/dlb"
#define MI_DN_DEVICE        "/dev/dn"
#define MI_DNF_DEVICE       "/dev/dnf"
#define MI_DRVE_DEVICE      "/dev/drve"
#define MI_ECHO_DEVICE      "/dev/echo"
#define MI_RAWIP_DEVICE     "/dev/rawip"
#define MI_HAVOC_DEVICE     "/dev/havoc"
#define MI_IP_DEVICE        "/dev/ip"
#define MI_IPX_DEVICE       "/dev/ipx"
#define MI_LOG_DEVICE       "/dev/log"
#define MI_MODE_DEVICE      "/dev/mode"
#define MI_MUX_DEVICE       "/dev/mux"
#define MI_NECHO_DEVICE     "/dev/necho"
#define MI_NPEP_DEVICE      "/dev/npep"
#define MI_NULS_DEVICE      "/dev/nuls"
#define MI_NULZ_DEVICE      "/dev/nulz"
#define MI_SAD_DEVICE       "/dev/sad"
#define MI_SPX_DEVICE       "/dev/spx"
#define MI_TCP_DEVICE       "/dev/tcp"
#define MI_TMUX_DEVICE      "/dev/tmux"
#define MI_TMUX0_DEVICE     "/dev/tmux#0"
#define MI_TMUX1_DEVICE     "/dev/tmux#1"
#define MI_TPIT_DEVICE      "/dev/tpit"
#define MI_UDP_DEVICE       "/dev/udp"
#define MI_XDG_DEVICE       "/dev/xdg"
#define MI_XECHO_DEVICE     "/dev/xecho"
#define MI_XF_DEVICE        "/dev/xf"
#define MI_XPE_DEVICE       "/dev/xpe"
#define MI_XS_DEVICE        "/dev/xs"
#define MI_XTINDG_DEVICE    "/dev/xtindg"
#define MI_XTINVC_DEVICE    "/dev/xtinvc"


/* Streamtab entries */
#define MI_AFU_STREAMTAB    afuinfo
#define MI_AHARP_STREAMTAB  aharinfo
#define MI_AHENET_STREAMTAB aheninfo
#define MI_ARP_STREAMTAB    arpinfo
#define MI_ARPM_STREAMTAB   arpminfo
#define MI_COURMUX_STREAMTAB    courmuxinfo
#define MI_CLONE_STREAMTAB  cloneinfo
#define MI_DLB_STREAMTAB    dlbinfo
#define MI_DLM_STREAMTAB    dlminfo
#define MI_DMODD_STREAMTAB  dmoddinfo
#define MI_DMODT_STREAMTAB  dmodtinfo
#define MI_DN_STREAMTAB     dninfo
#define MI_DNF_STREAMTAB    dnfinfo
#define MI_DRVE_STREAMTAB   drveinfo
#define MI_ECHO_STREAMTAB   echoinfo
#define MI_ENXR_STREAMTAB   enxrinfo
#define MI_HAVOC_STREAMTAB  hvcinfo
#define MI_HAVOCM_STREAMTAB hvcminfo
#define MI_IP_STREAMTAB     ipinfo
#define MI_IPM_STREAMTAB    ipminfo
#define MI_IPX_STREAMTAB    ipxinfo
#define MI_LOG_STREAMTAB    loginfo
#define MI_MODE_STREAMTAB   modeinfo
#define MI_MUX_STREAMTAB    muxinfo
#define MI_NECHO_STREAMTAB  nechoinfo
#define MI_NPEP_STREAMTAB   npepinfo
#define MI_NULS_STREAMTAB   nulsinfo
#define MI_NULZ_STREAMTAB   nulzinfo
#define MI_PASS_STREAMTAB   passinfo
#define MI_PIPEMOD_STREAMTAB    pmodinfo
#define MI_RAWIP_STREAMTAB  rawipinfo
#define MI_RAWIPM_STREAMTAB rawipminfo
#define MI_SAD_STREAMTAB    sadinfo
#define MI_SC_STREAMTAB     scinfo
#define MI_SOCKMOD_STREAMTAB    sockmodinfo
#define MI_SPASS_STREAMTAB  spassinfo
#define MI_SPX_STREAMTAB    spxinfo
#define MI_STH_STREAMTAB    mi_sthinfo
#define MI_TCP_STREAMTAB    tcpinfo
#define MI_TCPM_STREAMTAB   tcpminfo
#define MI_TIMOD_STREAMTAB  timodinfo
#define MI_TIRDWR_STREAMTAB tirdwrinfo
#define MI_TMOD_STREAMTAB   tmodinfo
#define MI_TMUX_STREAMTAB   tmuxinfo
#define MI_TPIT_STREAMTAB   tpitinfo
#define MI_TRSR_STREAMTAB   trsrinfo
#define MI_TRXR_STREAMTAB   trxrinfo
#define MI_UDP_STREAMTAB    udpinfo
#define MI_UDPM_STREAMTAB   udpminfo
#define MI_WELD_STREAMTAB   mi_weldinfo
#define MI_XDG_STREAMTAB    xdginfo
#define MI_XECHO_STREAMTAB  xechoinfo
#define MI_XF_STREAMTAB     xfinfo
#define MI_XFIPX_STREAMTAB  xfipxinfo
#define MI_XFXNS_STREAMTAB  xfxnsinfo
#define MI_XPE_STREAMTAB    xpeinfo
#define MI_XS_STREAMTAB     xsinfo
#define MI_XTINDG_STREAMTAB xtindginfo
#define MI_XTINVC_STREAMTAB xtinvcinfo
#define MI_XTM_STREAMTAB    xtminfo
#define MI_XTMIP_STREAMTAB  xtmipinfo


#define MI_AFU_DEVFLAG      afudevflag
#define MI_AHARP_DEVFLAG    ahardevflag
#define MI_AHENET_DEVFLAG   ahendevflag
#define MI_ARP_DEVFLAG      arpdevflag
#define MI_ARPM_DEVFLAG     arpmdevflag
#define MI_COURMUX_DEVFLAG  courmuxdevflag
#define MI_CLONE_DEVFLAG    clonedevflag
#define MI_DLB_DEVFLAG      dlbdevflag
#define MI_DLM_DEVFLAG      dlmdevflag
#define MI_DMODD_DEVFLAG    dmodddevflag
#define MI_DMODT_DEVFLAG    dmodtdevflag
#define MI_DN_DEVFLAG       dndevflag
#define MI_DNF_DEVFLAG      dnfdevflag
#define MI_DRVE_DEVFLAG     drvedevflag
#define MI_ECHO_DEVFLAG     echodevflag
#define MI_ENXR_DEVFLAG     enxrdevflag
#define MI_HAVOC_DEVFLAG    hvcdevflag
#define MI_HAVOCM_DEVFLAG   hvcmdevflag
#define MI_IP_DEVFLAG       ipdevflag
#define MI_IPM_DEVFLAG      ipmdevflag
#define MI_IPX_DEVFLAG      ipxdevflag
#define MI_LOG_DEVFLAG      logdevflag
#define MI_MODE_DEVFLAG     modedevflag
#define MI_MUX_DEVFLAG      muxdevflag
#define MI_NECHO_DEVFLAG    nechodevflag
#define MI_NPEP_DEVFLAG     npepdevflag
#define MI_NULS_DEVFLAG     nulsdevflag
#define MI_NULZ_DEVFLAG     nulzdevflag
#define MI_PASS_DEVFLAG     passdevflag
#define MI_PIPEMOD_DEVFLAG  pipemoddevflag
#define MI_RAWIP_DEVFLAG    rawipdevflag
#define MI_RAWIPM_DEVFLAG   rawipmdevflag

#define MI_SAD_DEVFLAG      saddevflag
#define MI_SC_DEVFLAG       scdevflag
#define MI_SOCKMOD_DEVFLAG  sockmoddevflag
#define MI_SPASS_DEVFLAG    spassdevflag
#define MI_SPX_DEVFLAG      spxdevflag
#define MI_TCP_DEVFLAG      tcpdevflag
#define MI_TCPM_DEVFLAG     tcpmdevflag
#define MI_TIMOD_DEVFLAG    timoddevflag
#define MI_TIRDWR_DEVFLAG   tirdwrdevflag
#define MI_TMOD_DEVFLAG     tmoddevflag
#define MI_TMUX_DEVFLAG     tmuxdevflag
#define MI_TPIT_DEVFLAG     tpitdevflag
#define MI_TRSR_DEVFLAG     trsrdevflag
#define MI_TRXR_DEVFLAG     trxrdevflag
#define MI_UDP_DEVFLAG      udpdevflag
#define MI_UDPM_DEVFLAG     udpmdevflag
#define MI_XDG_DEVFLAG      xdgdevflag
#define MI_XECHO_DEVFLAG    xechodevflag
#define MI_XF_DEVFLAG       xfdevflag
#define MI_XFIPX_DEVFLAG    xfipxdevflag
#define MI_XFXNS_DEVFLAG    xfxnsdevflag
#define MI_XPE_DEVFLAG      xpedevflag
#define MI_XS_DEVFLAG       xsdevflag
#define MI_XTINDG_DEVFLAG   xtindgdevflag
#define MI_XTINVC_DEVFLAG   xtinvcdevflag
#define MI_XTM_DEVFLAG      xtmdevflag
#define MI_XTMIP_DEVFLAG    xtmipdevflag

#define MI_AFU_SQLVL        SQLVL_QUEUEPAIR
#define MI_AHARP_SQLVL      SQLVL_QUEUE
#define MI_AHENET_SQLVL     SQLVL_QUEUE
#define MI_ARP_SQLVL        SQLVL_MODULE
#define MI_ARPM_SQLVL       SQLVL_MODULE
#define MI_COURMUX_SQLVL    SQLVL_MODULE
#define MI_CLONE_SQLVL      SQLVL_MODULE
#define MI_DLB_SQLVL        SQLVL_QUEUE
#define MI_DLM_SQLVL        SQLVL_QUEUE
#define MI_DMODD_SQLVL      SQLVL_QUEUE
#define MI_DMODT_SQLVL      SQLVL_QUEUE
#define MI_DN_SQLVL         SQLVL_QUEUE
#define MI_DNF_SQLVL        SQLVL_QUEUE
#define MI_DRVE_SQLVL       SQLVL_QUEUEPAIR
#define MI_ECHO_SQLVL       SQLVL_QUEUE
#define MI_ENXR_SQLVL       SQLVL_QUEUE
#define MI_RAWIP_SQLVL      SQLVL_QUEUE
#define MI_RAWIPM_SQLVL     SQLVL_QUEUE
#define MI_HAVOC_SQLVL      SQLVL_QUEUE
#define MI_HAVOCM_SQLVL     SQLVL_QUEUE
#define MI_IP_SQLVL         SQLVL_QUEUEPAIR
#define MI_IPM_SQLVL        SQLVL_QUEUEPAIR
#define MI_IPX_SQLVL        SQLVL_QUEUE
#define MI_LOG_SQLVL        SQLVL_MODULE
#define MI_MODE_SQLVL       SQLVL_QUEUEPAIR
#define MI_MUX_SQLVL        SQLVL_MODULE
#define MI_NECHO_SQLVL      SQLVL_QUEUE

#define MI_NPEP_SQLVL       SQLVL_QUEUE
#define MI_NULS_SQLVL       SQLVL_QUEUE
#define MI_NULZ_SQLVL       SQLVL_QUEUE
#define MI_PASS_SQLVL       SQLVL_QUEUE
#define MI_PIPEMOD_SQLVL    SQLVL_QUEUE

#define MI_SAD_SQLVL        SQLVL_MODULE
#define MI_SC_SQLVL         SQLVL_QUEUE
#define MI_SOCKMOD_SQLVL    SQLVL_QUEUEPAIR
#define MI_SPASS_SQLVL      SQLVL_QUEUE
#define MI_SPX_SQLVL        SQLVL_QUEUE
#define MI_TCP_SQLVL        SQLVL_QUEUEPAIR
#define MI_TCPM_SQLVL       SQLVL_QUEUEPAIR
#define MI_TIMOD_SQLVL      SQLVL_QUEUEPAIR
#define MI_TIRDWR_SQLVL     SQLVL_QUEUE
#define MI_TMOD_SQLVL       SQLVL_QUEUEPAIR
#define MI_TMUX_SQLVL       SQLVL_MODULE
#define MI_TPIT_SQLVL       SQLVL_MODULE
#define MI_TRSR_SQLVL       SQLVL_MODULE
#define MI_TRXR_SQLVL       SQLVL_QUEUE
#define MI_UDP_SQLVL        SQLVL_QUEUE
#define MI_UDPM_SQLVL       SQLVL_QUEUE
#define MI_XDG_SQLVL        SQLVL_QUEUE
#define MI_XECHO_SQLVL      SQLVL_QUEUE
#define MI_XF_SQLVL         SQLVL_MODULE
#define MI_XFIPX_SQLVL      SQLVL_MODULE
#define MI_XFXNS_SQLVL      SQLVL_MODULE
#define MI_XPE_SQLVL        SQLVL_QUEUE
#define MI_XS_SQLVL         SQLVL_QUEUEPAIR
#define MI_XTINDG_SQLVL     SQLVL_QUEUEPAIR
#define MI_XTINVC_SQLVL     SQLVL_QUEUEPAIR
#define MI_XTM_SQLVL        SQLVL_QUEUEPAIR
#define MI_XTMIP_SQLVL      SQLVL_QUEUEPAIR
/* ***** Raw Streams ******/


/*
   Flags used in the fType field of OTReadInfo for functions.
   I've removed the terse and confusing comments in this header
   file.  For a full description, read "Open Transport Advanced
   Client Programming".
*/

enum {
    kOTNoMessagesAvailable      = (unsigned long)0xFFFFFFFF,
    kOTAnyMsgType               = (unsigned long)0xFFFFFFFE,
    kOTDataMsgTypes             = (unsigned long)0xFFFFFFFC,
    kOTMProtoMsgTypes           = (unsigned long)0xFFFFFFFB,
    kOTOnlyMProtoMsgTypes       = (unsigned long)0xFFFFFFFA
};

#if !OTKERNEL
/* StreamRef is an opaque reference to a raw stream.*/

typedef struct OpaqueStreamRef*         StreamRef;
#define kOTInvalidStreamRef ((StreamRef)0L)
/* PollRef structure is used with the OTStreamPoll function.*/

struct PollRef {
    SInt32                          filler;                     /* holds a file descriptor an a UNIX system, replaced by ref (at end of structure) under OT*/
    SInt16                          events;
    SInt16                          revents;
    StreamRef                       ref;
};
typedef struct PollRef                  PollRef;
/* OTReadInfo structure is used with the various functions that read and peek at the stream head.*/

struct OTReadInfo {
    UInt32                          fType;
    OTCommand                       fCommand;
    UInt32                          fFiller;                    /* For compatibility with OT 1.0 and 1.1 */
    ByteCount                       fBytes;
    OSStatus                        fError;
};
typedef struct OTReadInfo               OTReadInfo;
/* Opening and closing raw streams*/
#if CALL_NOT_IN_CARBON
EXTERN_API( StreamRef )
OTStreamOpen                    (const char *           name,
                                 OTOpenFlags            oFlags,
                                 OSStatus *             errPtr);

EXTERN_API( OSStatus )
OTAsyncStreamOpen               (const char *           name,
                                 OTOpenFlags            oFlags,
                                 OTNotifyUPP            proc,
                                 void *                 contextPtr);

EXTERN_API( StreamRef )
OTCreateStream                  (OTConfigurationRef     cfig,
                                 OTOpenFlags            oFlags,
                                 OSStatus *             errPtr);

EXTERN_API( OSStatus )
OTAsyncCreateStream             (OTConfigurationRef     cfig,
                                 OTOpenFlags            oFlags,
                                 OTNotifyUPP            proc,
                                 void *                 contextPtr);

EXTERN_API( OSStatus )
OTStreamClose                   (StreamRef              strm);

/* Polling a stream for activity*/

EXTERN_API( OTResult )
OTStreamPoll                    (PollRef *              fds,
                                 UInt32                 nfds,
                                 OTTimeout              timeout);

EXTERN_API( OTResult )
OTAsyncStreamPoll               (PollRef *              fds,
                                 UInt32                 nfds,
                                 OTTimeout              timeout,
                                 OTNotifyUPP            proc,
                                 void *                 contextPtr);

/* Classic UNIX file descriptor operations*/

EXTERN_API( OTResult )
OTStreamRead                    (StreamRef              strm,
                                 void *                 buf,
                                 OTByteCount            len);

EXTERN_API( OTResult )
OTStreamWrite                   (StreamRef              strm,
                                 void *                 buf,
                                 OTByteCount            len);

EXTERN_API( OTResult )
OTStreamIoctl                   (StreamRef              strm,
                                 UInt32                 cmd,
                                 void *                 data);

EXTERN_API( OTResult )
OTStreamPipe                    (StreamRef              streamsToPipe[]);

/* there can be only 2!*/
/* Notifiers and modes of operation*/
EXTERN_API( OSStatus )
OTStreamInstallNotifier         (StreamRef              strm,
                                 OTNotifyUPP            proc,
                                 void *                 contextPtr);

EXTERN_API( void )
OTStreamRemoveNotifier          (StreamRef              strm);

EXTERN_API( OSStatus )
OTStreamUseSyncIdleEvents       (StreamRef              strm,
                                 Boolean                useEvents);

EXTERN_API( void )
OTStreamSetBlocking             (StreamRef              strm);

EXTERN_API( void )
OTStreamSetNonBlocking          (StreamRef              strm);

EXTERN_API( Boolean )
OTStreamIsBlocking              (StreamRef              strm);

EXTERN_API( void )
OTStreamSetSynchronous          (StreamRef              strm);

EXTERN_API( void )
OTStreamSetAsynchronous         (StreamRef              strm);

EXTERN_API( Boolean )
OTStreamIsSynchronous           (StreamRef              strm);

/* STREAMS primitives*/

EXTERN_API( OTResult )
OTStreamGetMessage              (StreamRef              strm,
                                 strbuf *               ctlbuf,
                                 strbuf *               databuf,
                                 OTFlags *              flags);

EXTERN_API( OTResult )
OTStreamGetPriorityMessage      (StreamRef              strm,
                                 strbuf *               ctlbuf,
                                 strbuf *               databuf,
                                 OTBand *               band,
                                 OTFlags *              flags);

EXTERN_API( OSStatus )
OTStreamPutMessage              (StreamRef              strm,
                                 const strbuf *         ctlbuf,
                                 const strbuf *         databuf,
                                 OTFlags                flags);

EXTERN_API( OSStatus )
OTStreamPutPriorityMessage      (StreamRef              strm,
                                 const strbuf *         ctlbuf,
                                 const strbuf *         databuf,
                                 OTBand                 band,
                                 OTFlags                flags);

/* Miscellaneous stuff*/

EXTERN_API( void )
OTStreamSetControlMask          (StreamRef              strm,
                                 UInt32                 mask,
                                 Boolean                setClear);

/*
   Opening endpoints and mappers on a Stream - these calls are synchronous, and may
   only be used at System Task time. Once the stream has been installed into a provider
   or endpoint, you should not continue to use STREAMS APIs on it
*/

EXTERN_API( ProviderRef )
OTOpenProviderOnStream          (StreamRef              strm,
                                 OSStatus *             errPtr);

EXTERN_API( EndpointRef )
OTOpenEndpointOnStream          (StreamRef              strm,
                                 OSStatus *             errPtr);

/*
   To quote an earlier version of this header file:
   
        Some functions that should only be used if
        you really know what you're doing.
*/

EXTERN_API( StreamRef )
OTRemoveStreamFromProvider      (ProviderRef            ref);

EXTERN_API( OSStatus )
OTPeekMessage                   (StreamRef              strm,
                                 OTReadInfo *           readInfo);

EXTERN_API( OTBuffer *)
OTReadMessage                   (StreamRef              strm,
                                 OTReadInfo *           readInfo);

EXTERN_API( void )
OTPutBackBuffer                 (StreamRef              strm,
                                 OTBuffer *             buffer);

EXTERN_API( void )
OTPutBackPartialBuffer          (StreamRef              strm,
                                 OTBufferInfo *         readInfo,
                                 OTBuffer *             buffer);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !OTKERNEL */

#endif  /* CALL_NOT_IN_CARBON */

/* ***** Port Utilities ******/
#if !OTKERNEL
/*
   These types and routines are used during sophisticated
   port management.  High-level clients may get involved
   for things like request a port to be yielding, but typically
   this stuff is used by protocol infrastructure.
*/
/*
   OTPortCloseStruct is used when processing the kOTClosePortRequest
   and kOTYieldPortRequest events.
*/


struct OTPortCloseStruct {
    OTPortRef                       fPortRef;                   /* The port requested to be closed.*/
    ProviderRef                     fTheProvider;               /* The provider using the port.*/
    OSStatus                        fDenyReason;                /* Set to a negative number to deny the request*/
};
typedef struct OTPortCloseStruct        OTPortCloseStruct;
/* OTClientList structure is used with the OTYieldPortRequest function.*/

struct OTClientList {
    ItemCount                       fNumClients;
    UInt8                           fBuffer[4];
};
typedef struct OTClientList             OTClientList;
/*
   Returns a buffer containing all of the clients that refused to yield the port.
   "size" is the total number of bytes @ buffer, including the fNumClients field.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
OTYieldPortRequest              (ProviderRef            ref,
                                 OTPortRef              portRef,
                                 OTClientList *         buffer,
                                 OTByteCount            size);

/* Send a notification to all Open Transport registered clients*/
EXTERN_API_C( void )
OTNotifyAllClients              (OTEventCode            code,
                                 OTResult               result,
                                 void *                 cookie);

/* Determine if "child" is a child port of "parent"*/
EXTERN_API_C( Boolean )
OTIsDependentPort               (OTPortRef              parent,
                                 OTPortRef              child);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !OTKERNEL */

/* ***** Timers ***** */
/*
   STREAMS plug-ins code should not use these timers, instead
   they should use timer messages, ie mi_timer etc.
*/

#if !OTKERNEL

typedef long                            OTTimerTask;
/*
   Under Carbon, OTCreateTimerTask takes a client context pointer.  Applications may pass NULL
   after calling InitOpenTransport(kInitOTForApplicationMask, ...).  Non-applications must always pass a
   valid client context.
*/
EXTERN_API( long )
OTCreateTimerTaskInContext      (OTProcessUPP           upp,
                                 void *                 arg,
                                 OTClientContextPtr     clientContext);

#if CALL_NOT_IN_CARBON
EXTERN_API( OTTimerTask )
OTCreateTimerTask               (OTProcessUPP           proc,
                                 void *                 arg);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( Boolean )
OTCancelTimerTask               (OTTimerTask            timerTask);

EXTERN_API( void )
OTDestroyTimerTask              (OTTimerTask            timerTask);

EXTERN_API( Boolean )
OTScheduleTimerTask             (OTTimerTask            timerTask,
                                 OTTimeout              milliSeconds);

#if OTCARBONAPPLICATION
/* The following macro may be used by applications only.*/
#define OTCreateTimerTask(upp, arg) OTCreateTimerTaskInContext(upp, arg, NULL)
#endif  /* OTCARBONAPPLICATION */

#endif  /* !OTKERNEL */

/* ***** Miscellaneous Helpful Functions ******/

#if !OTKERNEL
/*
   These routines allow you to manipulate OT's buffer structures.
   If you use no-copy receives (described in "OpenTransport.h")
   you will need some of these routines, and may choose to use others.
   See "Open Tranport Advanced Client Programming" for documentation.
*/
EXTERN_API_C( OTByteCount )
OTBufferDataSize                (OTBuffer *             buffer);

EXTERN_API_C( Boolean )
OTReadBuffer                    (OTBufferInfo *         buffer,
                                 void *                 dest,
                                 OTByteCount *          len);

EXTERN_API_C( void )
OTReleaseBuffer                 (OTBuffer *             buffer);


#if CALL_NOT_IN_CARBON
EXTERN_API_C( Boolean )
StoreIntoNetbuf                 (TNetbuf *              netBuf,
                                 void *                 source,
                                 SInt32                 len);

EXTERN_API_C( Boolean )
StoreMsgIntoNetbuf              (TNetbuf *              netBuf,
                                 OTBuffer *             buffer);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !OTKERNEL */

/* ***** OTConfiguration ******/
#if CALL_NOT_IN_CARBON
#if !OTKERNEL
/*
   As promised in "OpenTransport.h", here are the routines
   for advanced operations on configurations.
*/
/* Manipulating a configuration*/

#if CALL_NOT_IN_CARBON
EXTERN_API_C( OTConfigurationRef )
OTCfigNewConfiguration          (const char *           path);

EXTERN_API_C( void )
OTCfigDeleteConfiguration       (OTConfigurationRef     cfig);

EXTERN_API_C( OTConfigurationRef )
OTCfigCloneConfiguration        (OTConfigurationRef     cfig);

EXTERN_API_C( OTConfigurationRef )
OTCfigPushNewSingleChild        (OTConfigurationRef     cfig,
                                 const char *           path,
                                 OSStatus *             errPtr);

EXTERN_API_C( OTConfigurationRef )
OTCfigPushParent                (OTConfigurationRef     cfig,
                                 const char *           path,
                                 OSStatus *             errPtr);

EXTERN_API_C( OTConfigurationRef )
OTCfigPushChild                 (OTConfigurationRef     cfig,
                                 OTItemCount            index,
                                 const char *           path,
                                 OSStatus *             errPtr);

EXTERN_API_C( OSStatus )
OTCfigPopChild                  (OTConfigurationRef     cfig,
                                 OTItemCount            index);

EXTERN_API_C( OTConfigurationRef )
OTCfigGetChild                  (OTConfigurationRef     cfig,
                                 OTItemCount            index);

EXTERN_API_C( OSStatus )
OTCfigSetPath                   (OTConfigurationRef     cfig,
                                 const char *           path);

EXTERN_API_C( OTConfigurationRef )
OTCfigNewChild                  (OTConfigurationRef     cfig,
                                 const char *           path,
                                 OSStatus *             errPtr);

EXTERN_API_C( OSStatus )
OTCfigAddChild                  (OTConfigurationRef     cfig,
                                 OTConfigurationRef     child);

EXTERN_API_C( OTConfigurationRef )
OTCfigRemoveChild               (OTConfigurationRef     cfig,
                                 OTItemCount            index);

EXTERN_API_C( void )
OTCfigSetPortRef                (OTConfigurationRef     cfig,
                                 OTPortRef              portRef);

EXTERN_API_C( void )
OTCfigChangeProviderName        (OTConfigurationRef     cfig,
                                 const char *           name);

/* Query a configuration*/

EXTERN_API_C( UInt16 )
OTCfigNumberOfChildren          (OTConfigurationRef     cfig);

EXTERN_API_C( OTConfigurationRef )
OTCfigGetParent                 (OTConfigurationRef     cfig);

EXTERN_API_C( TNetbuf *)
OTCfigGetOptionNetbuf           (OTConfigurationRef     cfig);

EXTERN_API_C( OTPortRef )
OTCfigGetPortRef                (OTConfigurationRef     cfig);

EXTERN_API_C( UInt32 )
OTCfigGetInstallFlags           (OTConfigurationRef     cfig);

EXTERN_API_C( const char *)
OTCfigGetProviderName           (OTConfigurationRef     cfig);

EXTERN_API_C( Boolean )
OTCfigIsPort                    (OTConfigurationRef     cfig);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !OTKERNEL */

/* ***** Configurators ******/
/*
   The kOTConfiguratorInterfaceID define is what you need to add to your
   export file for the "interfaceID = " clause to export a configurator
   for ASLM.  Similarly, kOTConfiguratorCFMTag is used for CFM-based
   configurators.
*/


#define kOTConfiguratorInterfaceID  kOTClientPrefix "cfigMkr"
#define kOTConfiguratorCFMTag       kOTClientPrefix "cfigMkr"

#if !OTKERNEL
#ifdef __cplusplus
    class TOTConfigurator;
    typedef class TOTConfigurator* TOTConfiguratorRef;
#else
    typedef struct TOTConfigurator TOTConfigurator;
    typedef TOTConfigurator* TOTConfiguratorRef;
#endif
/*
   Typedef for the OTCanConfigure function, and the enum for which pass we're doing.
   The first (kOTSpecificConfigPass) is to give configurators a shot at the configuration
   before we start allowing the generic configurators to get into the act.
*/
enum {
    kOTSpecificConfigPass       = 0,
    kOTGenericConfigPass        = 1
};

typedef CALLBACK_API_C( Boolean , OTCanConfigureProcPtr )(OTConfigurationRef cfig, UInt32 pass);
/* Typedef for the function to create and return a configurator object*/
typedef CALLBACK_API_C( OSStatus , OTCreateConfiguratorProcPtr )(TOTConfiguratorRef *cfigor);
/*
   Typedef for the "OTSetupConfigurator" function that your configurator library must export.
   The enum is for the type of configurator that it is.
*/
#define kOTSetupConfiguratorID "OTSetupConfigurator"
enum {
    kOTDefaultConfigurator      = 0,
    kOTProtocolFamilyConfigurator = 1,
    kOTLinkDriverConfigurator   = 2
};

typedef CALLBACK_API_C( OSStatus , OTSetupConfiguratorProcPtr )(OTCanConfigureProcPtr *canConfigure, OTCreateConfiguratorProcPtr *createConfigurator, UInt8 *configuratorType);
/*
   Procedure pointer definitions for the three key callbacks associated
   with a configurator, as established by OTNewConfigurator.
*/
typedef CALLBACK_API_C( OSStatus , OTCFConfigureProcPtr )(TOTConfiguratorRef cfigor, OTConfigurationRef cfig);
typedef CALLBACK_API_C( OSStatus , OTCFCreateStreamProcPtr )(TOTConfiguratorRef cfigor, OTConfigurationRef cfig, OTOpenFlags oFlags, OTNotifyUPP proc, void *contextPtr);
typedef CALLBACK_API_C( void , OTCFHandleSystemEventProcPtr )(TOTConfiguratorRef cfigor, OTEventCode code, OTResult result, void *cookie);
/*
   Determine if this instance of your configurator is the "master"
   (the one that can create and destroy control streams)
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( Boolean )
OTIsMasterConfigurator          (TOTConfiguratorRef     cfigor);

/* Get back the userData you passed in to OTNewConfigurator*/
EXTERN_API_C( void *)
OTGetConfiguratorUserData       (TOTConfiguratorRef     cfigor);

/* Create a configurator object for use by Open Transport*/
EXTERN_API_C( TOTConfiguratorRef )
OTNewConfigurator               (void *                 userData,
                                 OTCFConfigureProcPtr   configure,
                                 OTCFCreateStreamProcPtr  createStream,
                                 OTCFHandleSystemEventProcPtr  handleEvent);

/* Delete a configurator object created by OTNewConfigurator*/
EXTERN_API_C( void )
OTDeleteConfigurator            (TOTConfiguratorRef     cfigor);

/*
   A utility function to send notifications to the user - it takes care of calls
   from deferred tasks
*/
EXTERN_API_C( OSStatus )
OTNotifyUser                    (FSSpec *               theFile,
                                 SInt32                 rsrcID,
                                 OTItemCount            index,
                                 char *                 parm1,
                                 char *                 parm2);

/* Call when the configurator unloads from memory*/
EXTERN_API_C( void )
OTConfiguratorUnloaded          (TOTConfiguratorRef     cfigor);

/*
   Call to create your control stream if you're not the master
   configurator.  You can also use the state machine function
   OTSMCreateControlStream(OTStateMachine*, OTConfigurationRef, TOTConfiguratorRef cfigor).
*/
EXTERN_API_C( OSStatus )
OTCreateControlStream           (OTConfigurationRef     cfig,
                                 TOTConfiguratorRef     cfigor,
                                 OTNotifyUPP            proc,
                                 void *                 contextPtr);

/*
   A helpful function for the configurators to
   be able to recursively configure the children.
*/
EXTERN_API_C( OSStatus )
OTConfigureChildren             (OTConfigurationRef     cfig);

/* Allocate a bit in the system-wide control mask for streams.*/
EXTERN_API_C( UInt32 )
OTNewControlMask                (void);

/* Warning: These 2 APIs is going away*/
EXTERN_API_C( void )
OTCloseProvidersByUseCount      (SInt32 *               useCount,
                                 OTResult               reason,
                                 OTBooleanParam         doneDeal);

EXTERN_API_C( void )
OTCloseProvidersByPortRef       (OTPortRef              ref,
                                 OTResult               reason,
                                 OTBooleanParam         doneDeal);

/* These are the "real" APIs*/
EXTERN_API_C( void )
OTCloseProviderByStream         (StreamRef              ref,
                                 OTResult               reason,
                                 OTBooleanParam         doneDeal);

EXTERN_API_C( void )
OTCloseMatchingProviders        (UInt32                 mask,
                                 OTPortRef              port,
                                 OTResult               reason,
                                 OTBooleanParam         doneDeal);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !OTKERNEL */

#endif  /* CALL_NOT_IN_CARBON */

/* ***** OTStateMachine ******/
#if CALL_NOT_IN_CARBON
/*
   This utility set allows you to write an asynchronous chain of code that looks 
   somewhat like it is synchronous.  This is primarily used for plumbing 
   streams asynchronously, especially in configurators
*/
#if !OTKERNEL
/* Alas, the state machine is only available to client code.  Sorry.*/

/*
   There are 12 or 8 bytes of reserved space at the front of
   the OTStateMachine structure, depending on whether you're
   building PowerPC or 68K code..  The OTStateMachineDataPad
   type compensates for this.
*/

#if TARGET_CPU_PPC
typedef UInt8                           OTStateMachineDataPad[12];
#else
typedef UInt8                           OTStateMachineDataPad[8];
#endif  /* TARGET_CPU_PPC */

/*
   Forward define OTStateMachine so that OTStateProcPtr has
   access to it.
*/
typedef struct OTStateMachine           OTStateMachine;
/*
   This type is is the required prototype of a state machine
   entry point.
*/
typedef CALLBACK_API( void , OTStateProcPtr )(OTStateMachine *sm);
/*
   This type defines a routine that the state machine will
   call when the top level completes.
*/
typedef CALLBACK_API_C( void , OTSMCompleteProcPtr )(void *contextPtr);
/* And now for the state machine structure itself.*/

struct OTStateMachine {
    OTStateMachineDataPad           fData;
    void *                          fCookie;
    OTEventCode                     fCode;
    OTResult                        fResult;
#ifdef __cplusplus

    // C++ inline methods on this structure.



    void*       GetClientData();

    Boolean     CallStateProc(OTStateProcPtr proc, UInt32 state = 0);

    UInt16      GetState();

    void        SetState(UInt32 state);

    void        Complete();

    void        Complete(OTResult result);

    void        Complete(OTResult result, OTEventCode code, void* contextPtr);

    void        CompleteToClient();

    void        CompleteToClient(OTResult result);

    void        CompleteToClient(OTResult result, OTEventCode code, void* contexPtr);

    void        PopCallback();

    Boolean     CreateStream(OTConfigurationRef cfig, OTOpenFlags flags);

    Boolean     OpenStream(const char* name, OTOpenFlags flags);

    Boolean     SendIoctl(StreamRef ref, UInt32 type, void* data);

    Boolean     SendIoctl(StreamRef ref, UInt32 type, long data);

    Boolean     PutMessage(StreamRef ref, strbuf* ctl, strbuf* data, OTFlags flags);

    Boolean     GetMessage(StreamRef ref, strbuf* ctl, strbuf* data, OTFlags* flagPtr);

    OSStatus    ReturnToCaller();

#endif

};

#define kOTSMBufferSize(callDepth) (80 + (callDepth * 8))
/*
   For structSize, pass the size of your structure that you want associated with
   the state machine.  It can later be obtained by calling OTSMGetClientData.
   For bufSize, use the kOTSMBufferSize macro, plus the size of your structure
   to create a buffer on the stack. For synchronous calls, the stack buffer will
   be used (unless you pass in NULL).  The callDepth is the depth level of nested
   calls using OTSMCallStateProc.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OTStateMachine *)
OTCreateStateMachine            (void *                 buf,
                                 OTByteCount            bufSize,
                                 OTByteCount            structSize,
                                 OTNotifyUPP            proc,
                                 void *                 contextPtr);

EXTERN_API_C( void )
OTDestroyStateMachine           (OTStateMachine *       sm);

/*
   OTSMCallStateProc used to take a parameter of type UInt16_p,
   which was defined to be the same as UInt32.  In an attempt
   to reduce the number of wacky types defined by the OT
   interfaces, we've changed these routines to just take a
   straight UInt32.  You should be warned that the current
   implementation does not support values outside of the
   range 0..32767.  The same applies to OTSMSetState.
*/

EXTERN_API_C( Boolean )
OTSMCallStateProc               (OTStateMachine *       sm,
                                 OTStateProcPtr         proc,
                                 UInt32                 state);

EXTERN_API_C( UInt16 )
OTSMGetState                    (OTStateMachine *       sm);

EXTERN_API_C( void )
OTSMSetState                    (OTStateMachine *       sm,
                                 UInt32                 state);

/* Fill out the fCookie, fCode, and fResult fields before calling!*/
EXTERN_API_C( void )
OTSMComplete                    (OTStateMachine *       sm);

EXTERN_API_C( void )
OTSMPopCallback                 (OTStateMachine *       sm);

EXTERN_API_C( Boolean )
OTSMWaitForComplete             (OTStateMachine *       sm);

EXTERN_API_C( Boolean )
OTSMCreateStream                (OTStateMachine *       sm,
                                 OTConfigurationRef     cfig,
                                 OTOpenFlags            flags);

EXTERN_API_C( Boolean )
OTSMOpenStream                  (OTStateMachine *       sm,
                                 const char *           name,
                                 OTOpenFlags            flags);

EXTERN_API_C( Boolean )
OTSMIoctl                       (OTStateMachine *       sm,
                                 StreamRef              strm,
                                 UInt32                 cmd,
                                 long                   data);

EXTERN_API_C( Boolean )
OTSMPutMessage                  (OTStateMachine *       sm,
                                 StreamRef              strm,
                                 strbuf *               ctlbuf,
                                 strbuf *               databuf,
                                 OTFlags                flags);

EXTERN_API_C( Boolean )
OTSMGetMessage                  (OTStateMachine *       sm,
                                 StreamRef              strm,
                                 strbuf *               ctlbuf,
                                 strbuf *               databuf,
                                 OTFlags *              flagsPtr);

EXTERN_API_C( OSStatus )
OTSMReturnToCaller              (OTStateMachine *       sm);

EXTERN_API_C( void *)
OTSMGetClientData               (OTStateMachine *       sm);

EXTERN_API_C( void )
OTSMInstallCompletionProc       (OTStateMachine *       sm,
                                 OTSMCompleteProcPtr    completeProc,
                                 void *                 contextPtr);

EXTERN_API_C( Boolean )
OTSMCreateControlStream         (OTStateMachine *       sm,
                                 OTConfigurationRef     cfig,
                                 TOTConfiguratorRef     cfigor);


#ifdef __cplusplus
    inline void*        OTStateMachine::GetClientData() { return OTSMGetClientData(this); }
    inline Boolean      OTStateMachine::CallStateProc(OTStateProcPtr proc, UInt32 state)
                                                        { return OTSMCallStateProc(this, proc, state); }
    inline UInt16       OTStateMachine::GetState()      { return OTSMGetState(this); }
    inline void         OTStateMachine::SetState(UInt32 state)
                                                        { OTSMSetState(this, state); }
    inline void         OTStateMachine::PopCallback()   { OTSMPopCallback(this); }
    inline void         OTStateMachine::Complete()      { OTSMComplete(this); }
    inline void         OTStateMachine::Complete(OTResult result, OTEventCode code, void* cookie)
                                                        { fCookie = cookie; fCode = code; fResult = result; Complete(); }
    inline void         OTStateMachine::Complete(OTResult result)
                                                        { fResult = result; Complete(); }
    inline void         OTStateMachine::CompleteToClient()
                                                        { PopCallback(); Complete(); }
    inline void         OTStateMachine::CompleteToClient(OTResult result)
                                                        { fResult   = result; CompleteToClient(); }
    
    inline void         OTStateMachine::CompleteToClient(OTResult result, OTEventCode code, void* cookie)
                                                        { fCookie = cookie; fCode = code; fResult = result; CompleteToClient(); }
    inline Boolean      OTStateMachine::CreateStream(OTConfigurationRef cfig, OTOpenFlags flags)
                                                        { return OTSMCreateStream(this, cfig, flags); }
    inline Boolean      OTStateMachine::OpenStream(const char* name, OTOpenFlags flags)
                                                        { return OTSMOpenStream(this, name, flags); }
    inline Boolean      OTStateMachine::SendIoctl(StreamRef ref, UInt32 type, void* data)
                                                        { return OTSMIoctl(this, ref, type, (long)data); }
    inline Boolean      OTStateMachine::SendIoctl(StreamRef ref, UInt32 type, long data)
                                                        { return OTSMIoctl(this, ref, type, data); }
    inline Boolean      OTStateMachine::PutMessage(StreamRef ref, struct strbuf* ctl, struct strbuf* data, OTFlags flags)
                                                        { return OTSMPutMessage(this, ref, ctl, data, flags); }
    inline Boolean      OTStateMachine::GetMessage(StreamRef ref, struct strbuf* ctl, struct strbuf* data, OTFlags* flagPtr)
                                                        { return OTSMGetMessage(this, ref, ctl, data, flagPtr); }
    inline OSStatus     OTStateMachine::ReturnToCaller()
                                                        { return OTSMReturnToCaller(this); }
#endif

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !OTKERNEL */

/* ***** Autopush Definitions ******/
/*
   The autopush functionality for Open Transport is based on the names of
   devices and modules, rather than on the major number information like 
   SVR4.  This is so that autopush information can be set up for modules
   that are not yet loaded.
*/


/* The name of the STREAMS driver you open and send the ioctls to.*/
#define kSADModuleName  "sad"
/* Autopush ioctls.*/
enum {
    I_SAD_SAP                   = ((MIOC_SAD << 8) | 1),        /* Set autopush information     */
    I_SAD_GAP                   = ((MIOC_SAD << 8) | 2),        /* Get autopush information     */
    I_SAD_VML                   = ((MIOC_SAD << 8) | 3)         /* Validate a list of modules (uses str_list structure) */
};

/* Maximum number of modules autopushed on a driver.*/

enum {
    kOTAutopushMax              = 8
};


/* ioctl structure used for SAD_SAP and SAD_GAP commands.*/


struct OTAutopushInfo {
    UInt32                          sap_cmd;
    char                            sap_device_name[32];
    SInt32                          sap_minor;
    SInt32                          sap_lastminor;
    SInt32                          sap_npush;
    char                            sap_list[8][32];
};
typedef struct OTAutopushInfo           OTAutopushInfo;
/* Command values for sap_cmd field of the above.*/
enum {
    kSAP_ONE                    = 1,                            /* Configure a single minor device         */
    kSAP_RANGE                  = 2,                            /* Configure a range of minor devices     */
    kSAP_ALL                    = 3,                            /* Configure all minor devices          */
    kSAP_CLEAR                  = 4                             /* Clear autopush information          */
};


/* ***** Configuration Helpers ******/

/*
   These definitions are used by device driver and port scanner
   developers to provide a library giving client-side information about
   the registered ports, such as a user-visible name or an icon.
*/

/* Configuration helper library prefix*/

/*
   This prefix is prepended to the string found in the "fResourceInfo"
   field of the OTPortRecord to build the actual library name of the
   configuration helper library.
*/

#define kPortConfigLibPrefix "OTPortCfg$"

/* Get user visible port name entry point.*/

/*
   This entry point returns the user visible name of the port.  If includeSlot
   is true, a slot distinguishing suffix (eg "slot X") should be added.  If
   includePort is true, a port distinguishing suffix (eg " port X") should be added for
   multiport cards.
*/

#define kOTGetUserPortNameID "OTGetUserPortName"
typedef CALLBACK_API_C( void , OTGetPortNameProcPtr )(OTPortRecord *port, OTBooleanParam includeSlot, OTBooleanParam includePort, Str255 userVisibleName);
/* Get icon entry point.*/
/*
   This entry point returns the location of the icon for the port.  Return false if no
   icon is provided.
*/

#define kOTGetPortIconID "OTGetPortIcon"

struct OTResourceLocator {
    FSSpec                          fFile;
    UInt16                          fResID;
};
typedef struct OTResourceLocator        OTResourceLocator;
typedef CALLBACK_API_C( Boolean , OTGetPortIconProcPtr )(OTPortRecord *port, OTResourceLocator *iconLocation);
/* ***** Application Access to Configuration Helpers ******/

#if !OTKERNEL
/*
   These routines are used by clients to get information about ports.
   The canonical user of these routines is the OT control panel(s),
   but applications may want to use them as well (to display the list
   of available Ethernet cards, for example).
*/
/*  Returns a user friendly name for a port.*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
OTGetUserPortNameFromPortRef    (OTPortRef              ref,
                                 Str255                 friendlyName);

/*
    Returns the location for the icon familly representing the port.
    Returns false if the port has no icon.
*/
EXTERN_API_C( Boolean )
OTGetPortIconFromPortRef        (OTPortRef              ref,
                                 OTResourceLocator *    iconLocation);

#endif  /* CALL_NOT_IN_CARBON */

/* Returns true if the port can be used with the specified protocol.*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( Boolean )
OTIsPortCompatibleWith          (const OTPortRecord *   port,
                                 char *                 protocolName);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !OTKERNEL */

#endif  /* CALL_NOT_IN_CARBON */

/* ***** Common Utilities ******/
/*
   The utilities defined in this section are available to both client
   and kernel code.  Cool huh?  These utilities differ from those
   provided in "OpenTransport.h" in that they are only available to native
   architecture clients.
*/

/* Bitmap functions*/

/* These functions atomically deal with a bitmap that is multiple-bytes long*/

/*
   Set the first clear bit in "bitMap", starting with bit "startBit",
   giving up after "numBits".  Returns the bit # that was set, or
   a kOTNotFoundErr if there was no clear bit available
*/
EXTERN_API_C( OTResult )
OTSetFirstClearBit              (UInt8 *                bitMap,
                                 OTByteCount            startBit,
                                 OTByteCount            numBits);

/* Standard clear, set and test bit functions*/
EXTERN_API_C( Boolean )
OTClearBit                      (UInt8 *                bitMap,
                                 OTByteCount            bitNo);

EXTERN_API_C( Boolean )
OTSetBit                        (UInt8 *                bitMap,
                                 OTByteCount            bitNo);

EXTERN_API_C( Boolean )
OTTestBit                       (UInt8 *                bitMap,
                                 OTByteCount            bitNo);

/* OTHashList*/

/*
   This implements a simple, but efficient hash list.  It is not
   thread-safe.
*/

typedef CALLBACK_API_C( UInt32 , OTHashProcPtr )(OTLink *linkToHash);
typedef CALLBACK_API_C( Boolean , OTHashSearchProcPtr )(const void *ref, OTLink *linkToCheck);

struct OTHashList {
    OTHashProcPtr                   fHashProc;
    ByteCount                       fHashTableSize;
    OTLink **                       fHashBuckets;
#ifdef __cplusplus

    // C++ inline methods on this structure.



    void            Add(OTLink* toAdd);

    Boolean         RemoveLink(OTLink* toRemove);

    OTLink*         Remove(OTHashSearchProcPtr proc, const void* refPtr, UInt32 hashValue);

    Boolean         IsInList(OTLink* toFind);

    OTLink*         FindLink(OTHashSearchProcPtr proc, const void* refPtr, UInt32 hash);

#endif

};
typedef struct OTHashList               OTHashList;
/*
   Return the number of bytes of memory needed to create a hash list
   of at least "numEntries" entries.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OTByteCount )
OTCalculateHashListMemoryNeeds  (OTItemCount            numEntries);

/*
   Create an OTHashList from "memory".  Return an error if it
   couldn't be done.
*/
EXTERN_API_C( OTResult )
OTInitHashList                  (void *                 memory,
                                 OTByteCount            numBytes,
                                 OTHashProcPtr          hashProc);

EXTERN_API_C( void )
OTAddToHashList                 (OTHashList *           hashList,
                                 OTLink *               linkToAdd);

EXTERN_API_C( Boolean )
OTRemoveLinkFromHashList        (OTHashList *           hashList,
                                 OTLink *               linkToRemove);

EXTERN_API_C( Boolean )
OTIsInHashList                  (OTHashList *           hashList,
                                 OTLink *               link);

EXTERN_API_C( OTLink *)
OTFindInHashList                (OTHashList *           hashList,
                                 OTHashSearchProcPtr    searchProc,
                                 const void *           refPtr,
                                 UInt32                 hashValue);

EXTERN_API_C( OTLink *)
OTRemoveFromHashList            (OTHashList *           hashList,
                                 OTHashSearchProcPtr    searchProc,
                                 const void *           refPtr,
                                 UInt32                 hashValue);


#ifdef __cplusplus
    // C++ inline methods on this structure.

    inline void         OTHashList::Add(OTLink* toAdd)  { OTAddToHashList(this, toAdd); }

    inline Boolean      OTHashList::RemoveLink(OTLink* toRemove)
                                                        { return OTRemoveLinkFromHashList(this, toRemove); }
    inline OTLink*      OTHashList::Remove(OTHashSearchProcPtr proc, const void* refPtr, UInt32 hashValue)
                                                        { return OTRemoveFromHashList(this, proc, refPtr, hashValue); }
    inline Boolean      OTHashList::IsInList(OTLink* toFind)
                                                        { return OTIsInHashList(this, toFind); }
    inline OTLink*      OTHashList::FindLink(OTHashSearchProcPtr proc, const void* refPtr, UInt32 hash)
                                                        { return OTFindInHashList(this, proc, refPtr, hash); }
#endif

/* Random functions*/

/*
   These implement a very simple random number generator, suitable
   for protocol implementations but not "cryptographically" random.
*/

EXTERN_API_C( UInt32 )
OTGetRandomSeed                 (void);

EXTERN_API_C( UInt32 )
OTGetRandomNumber               (UInt32 *               seed,
                                 UInt32                 lo,
                                 UInt32                 hi);

/* Concurrency Control*/

/*
   OTGate implements a cool concurrency control primitive.
   You're not going to understand it without reading the documentation!
   See "Open Transport Advanced Client Programming" for details.
   WARNING:
   This structure must be on a 4-byte boundary.
*/

#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( Boolean , OTGateProcPtr )(OTLink *thisLink);

struct OTGate {
    OTLIFO                          fLIFO;
    OTList                          fList;
    OTGateProcPtr                   fProc;
    SInt32                          fNumQueued;
    SInt32                          fInside;
};
typedef struct OTGate                   OTGate;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
OTInitGate                      (OTGate *               gate,
                                 OTGateProcPtr          proc);

EXTERN_API_C( Boolean )
OTEnterGate                     (OTGate *               gate,
                                 OTLink *               withLink);

EXTERN_API_C( Boolean )
OTLeaveGate                     (OTGate *               gate);

/* ***** Shared Library Bonus Extras ******/

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
/*
   These routines provide addition shared library support beyond
   that provided by the base shared library mechanism.
*/
/*
   Some flags which can be passed to the "loadFlags" parameter of the
   various CFM routines.  Not all flags can be used with all routines.
   See "Open Transport Advanced Client Programming" for details.
*/

enum {
    kOTGetDataSymbol            = 0,
    kOTGetCodeSymbol            = 1,
    kOTLoadNewCopy              = 2,
    kOTLoadACopy                = 4,
    kOTFindACopy                = 8,
    kOTLibMask                  = kOTLoadNewCopy | kOTLoadACopy | kOTFindACopy,
    kOTLoadLibResident          = 0x20
};

/* Finding all matching CFM libraries.*/

/*
   The routine OTFindCFMLibraries allows you to find all CFM libraries
   that match specific criteria.  The result is placed in a list
   of CFMLibraryInfo structures.  OT allocates those structures using
   a routine of type OTAllocMemProcPtr that you pass to OTFindCFMLibraries.
*/

/*
   A list of CFMLibraryInfo structures is returned by the OTFindCFMLibraries routine.
   The list is created out of the data that is passed to the function.
   
   IMPORTANT:
   Only the first 3 fields are valid when using OT 1.2 and older.
*/


struct CFMLibraryInfo {
    OTLink                          link;                       /* To link them all up on a list            */
    char *                          libName;                    /* "C" String which is fragment name          */
    StringPtr                       intlName;                   /* Pascal String which is internationalized name  */
    FSSpec *                        fileSpec;                   /* location of fragment's file */
    StringPtr                       pstring2;                   /* Secondary string from extended cfrg          */
    StringPtr                       pstring3;                   /* Extra info from extended cfrg            */
};
typedef struct CFMLibraryInfo           CFMLibraryInfo;
/*
   You must pass a routine of type OTAllocMemProcPtr to OTFindCFMLibraries
   which it calls to allocate memory for the CFMLibraryInfo structures.
*/
typedef CALLBACK_API_C( void *, OTAllocMemProcPtr )(OTByteCount size);
/* Find CFM libraries of the specified kind and type*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
OTFindCFMLibraries              (OSType                 libKind,
                                 const char *           libType,
                                 OTList *               theList,
                                 OTAllocMemProcPtr      allocator);

/* Loading libraries and connecting to symbols.*/

/* Load a CFM library by name*/
EXTERN_API_C( OSStatus )
OTLoadCFMLibrary                (const char *           libName,
                                 UInt32 *               connID,
                                 UInt32                 loadFlags);

/* Load a CFM library and get a named pointer from it*/
EXTERN_API_C( void *)
OTGetCFMPointer                 (const char *           libName,
                                 const char *           entryName,
                                 UInt32 *               connID,
                                 UInt32                 loadFlags);

/* Get a named pointer from a CFM library that's already loaded*/
EXTERN_API_C( void *)
OTGetCFMSymbol                  (const char *           entryName,
                                 UInt32                 connID,
                                 UInt32                 loadFlags);

/* Release a connection to a CFM library*/
EXTERN_API_C( void )
OTReleaseCFMConnection          (UInt32 *               connID);

#endif  /* CALL_NOT_IN_CARBON */

#if !TARGET_CPU_68K
/*
   You can call these routines in your CFM initialisation and termination
   routines to hold or unhold your libraries sections.
*/
/*
   Used in a CFM InitProc, will hold the executable code, if applicable.
   This can also be the InitProc of the library
*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
OTHoldThisCFMLibrary            (const CFragInitBlock * initBlock);

/*
   Used in a CFM terminate proc, will unhold the executable code, if applicable.
   This can also be the terminate proc of the library
*/
EXTERN_API_C( void )
OTUnholdThisCFMLibrary          (void);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !TARGET_CPU_68K */

/* ASLM Utilities*/
/* Load an ASLM library*/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
OTLoadASLMLibrary               (const char *           libName);

/* Unload an ASLM library*/
EXTERN_API_C( void )
OTUnloadASLMLibrary             (const char *           libName);

/*
   This is an ASLM utility routine.  You can get it by including
   "LibraryManagerUtilities.h", but since we only use a few ASLM utilities,
   we put the prototype here for convenience.
*/

EXTERN_API_C( void )
UnloadUnusedLibraries           (void);


#endif  /* CALL_NOT_IN_CARBON */

#if !OTKERNEL
/*******************************************************************************
** A few C++ objects for C++ fans
********************************************************************************/
#if CALL_NOT_IN_CARBON

#ifdef __cplusplus
}                   // Terminate C definitions

    class OTConfiguration
    {
        public:
                    OTConfigurationRef  Clone()
                                        { return OTCfigCloneConfiguration(this); }
                //
                // The Path for PushChild and PushParent must be a single module
                //
                    OTConfigurationRef  PushChild(const char* path, OSStatus* errPtr)
                                        { return OTCfigPushNewSingleChild(this, path, errPtr); }
                    OTConfigurationRef  PushParent(const char* path, OSStatus* errPtr)
                                        { return OTCfigPushParent(this, path, errPtr); }
                    OTConfigurationRef  PushNthChild(OTItemCount index, const char* path,
                                                     OSStatus* errPtr)
                                        { return OTCfigPushChild(this, index, path, errPtr); }
                    OSStatus            PopChild(OTItemCount index)
                                        { return OTCfigPopChild(this, index); }

                    OTConfigurationRef  GetChild(OTItemCount index = 0)
                                        { return OTCfigGetChild(this, index); }
                    OTConfigurationRef  GetParent()
                                        { return OTCfigGetParent(this); }
            
                    OSStatus            AddChild(OTConfigurationRef child)
                                        { return OTCfigAddChild(this, child); }
                    
                    OTConfigurationRef  NewChild(const char* path, OSStatus* errPtr)
                                        { return OTCfigNewChild(this, path, errPtr); }
        
                    OSStatus            SetPath(const char* path)
                                        { return OTCfigSetPath(this, path); }
            
                    Boolean             HasOptions()
                                        { return OTCfigGetOptionNetbuf(this)->len != 0; }
    };

/*  -------------------------------------------------------------------------
    Class TOTConfigurator

    This class is subclassed to do configuration for a protocol or protocol stack.
    Of course, you can also use OTNewConfigurator to do it from C.
    
    If you subclass it using C++, you MUST have a UInt32 field as the first
    field of your object that you do not touch or use.
    ------------------------------------------------------------------------- */

#if TARGET_CPU_68K && !defined(__SC__) && !defined(THINK_CPLUS)
    class TOTConfigurator : public SingleObject
#else
    class TOTConfigurator
#endif
    {
    #if defined(__SC__) || defined(THINK_CPLUS) || defined(__MRC__)
        private:
            virtual     void DummyVirtualFunction();
    #endif
                
        public:
                void*           operator new(OTByteCount size)
                                { return OTAllocSharedClientMem(size); }
                void            operator delete(void* mem)
                                { OTFreeSharedClientMem(mem); };
                                
                                _MDECL TOTConfigurator();
            virtual             ~ _MDECL TOTConfigurator();
    
            virtual void        _MDECL HandleSystemEvent(OTEventCode event, OTResult result,
                                                         void* cookie)                      = 0;
            
            virtual OSStatus    _MDECL Configure(OTConfigurationRef)                            = 0;
            virtual OSStatus    _MDECL CreateStream(OTConfigurationRef, OTOpenFlags,
                                                     OTNotifyUPP, void* contextPtr)     = 0;
    };

extern "C" {        // resume C definitions
#endif /*__cplusplus*/

#endif /* CALL_NOT_IN_CARBON */
#endif  /* !OTKERNEL */

#endif  /* CALL_NOT_IN_CARBON */

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

#endif /* __OPENTRANSPORTPROTOCOL__ */

