/*
     File:       OpenTransportUNIX.h
 
     Contains:   Open Transport client interface file for UNIX compatibility clients.
 
     Version:    Technology: 2.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1993-2001 by Apple Computer, Inc. and Mentat Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OPENTRANSPORTUNIX__
#define __OPENTRANSPORTUNIX__

#include <stddef.h>
#ifndef __OPENTRANSPORTPROTOCOL__
#include <OpenTransportProtocol.h>
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

/* ***** Global Variables ******/

extern int t_errno;

/* ***** XTI Structures ******/

/*
   WARNING:
   These structures will only work if "int"s are the
   same size as "size_t", "long", and "UInt32".  We left
   them this way for maximum UNIX compatibility.
*/


struct netbuf {
    unsigned int                    maxlen;
    unsigned int                    len;
    char *                          buf;
};
typedef struct netbuf                   netbuf;

struct t_info {
    int                             addr;                       /* Maximum size of an address        */
    int                             options;                    /* Maximum size of options          */
    int                             tsdu;                       /* Standard data transmit unit size     */
    int                             etsdu;                      /* Expedited data transmit unit size  */
    int                             connect;                    /* Maximum data size on connect      */
    int                             discon;                     /* Maximum data size on disconnect       */
    unsigned int                    servtype;                   /* service type                */
    unsigned int                    flags;                      /* Flags (see "OpenTransport.h")   */
};
typedef struct t_info                   t_info;

struct t_bind
{
    netbuf              addr;
    unsigned int        qlen;
};
struct t_optmgmt
{
    netbuf          opt;
    long            flags;
};

struct t_discon {
    netbuf                          udata;
    int                             reason;
    int                             sequence;
};
typedef struct t_discon                 t_discon;

struct t_call {
    netbuf                          addr;
    netbuf                          opt;
    netbuf                          udata;
    int                             sequence;
};
typedef struct t_call                   t_call;

struct t_unitdata {
    netbuf                          addr;
    netbuf                          opt;
    netbuf                          udata;
};
typedef struct t_unitdata               t_unitdata;

struct t_uderr {
    netbuf                          addr;
    netbuf                          opt;
    long                            error;
};
typedef struct t_uderr                  t_uderr;
/*    -------------------------------------------------------------------------
    Transaction data structures
    ------------------------------------------------------------------------- */

struct t_request {
    netbuf                          data;
    netbuf                          opt;
    long                            sequence;
};
typedef struct t_request                t_request;

struct t_reply {
    netbuf                          data;
    netbuf                          opt;
    long                            sequence;
};
typedef struct t_reply                  t_reply;

struct t_unitrequest {
    netbuf                          addr;
    netbuf                          opt;
    netbuf                          udata;
    long                            sequence;
};
typedef struct t_unitrequest            t_unitrequest;

struct t_unitreply {
    netbuf                          opt;
    netbuf                          udata;
    long                            sequence;
};
typedef struct t_unitreply              t_unitreply;

struct t_opthdr {
    unsigned long                   len;                        /* total length of option = sizeof(struct t_opthdr) + */
                                                                /*                 length of option value in bytes   */
    unsigned long                   level;                      /* protocol affected */
    unsigned long                   name;                       /* option name */
    unsigned long                   status;                     /* status value */
                                                                /* followed by the option value */
};
typedef struct t_opthdr                 t_opthdr;
/* ***** XTI Interfaces ******/
#if !OTKERNEL
#if CALL_NOT_IN_CARBON
EXTERN_API( int )
t_accept                        (int                    fd,
                                 int                    resfd,
                                 t_call *               call);

EXTERN_API( char *)
t_alloc                         (int                    fd,
                                 int                    struct_type,
                                 int                    fields);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( int ) t_bind(int fd, struct t_bind* req, struct t_bind* ret);
EXTERN_API( int )t_optmgmt(int fd, struct t_optmgmt* req, struct t_optmgmt* ret);
EXTERN_API( int ) t_getprotaddr(int fd, struct t_bind* boundaddr, struct t_bind* peeraddr);
EXTERN_API( int ) t_resolveaddr(int fd, struct t_bind* reqAddr, struct t_bind* retAddr, OTTimeout timeout);
#if CALL_NOT_IN_CARBON
EXTERN_API( int )
t_close                         (int                    fd);

EXTERN_API( int )
t_connect                       (int                    fd,
                                 t_call *               sndcall,
                                 t_call *               rcvcall);

EXTERN_API( int )
t_free                          (char *                 ptr,
                                 int                    struct_type);

EXTERN_API( int )
t_getinfo                       (int                    fd,
                                 t_info *               info);

EXTERN_API( int )
t_getstate                      (int                    fd);

EXTERN_API( int )
t_listen                        (int                    fd,
                                 t_call *               call);

EXTERN_API( int )
t_look                          (int                    fd);

EXTERN_API( int )
t_open                          (char *                 path,
                                 int                    oflag,
                                 t_info *               info);

EXTERN_API( int )
t_blocking                      (int                    fd);

EXTERN_API( int )
t_nonblocking                   (int                    fd);

EXTERN_API( int )
t_rcv                           (int                    fd,
                                 char *                 buf,
                                 size_t                 nbytes,
                                 int *                  flags);

EXTERN_API( int )
t_rcvconnect                    (int                    fd,
                                 t_call *               call);

EXTERN_API( int )
t_rcvdis                        (int                    fd,
                                 t_discon *             discon);

EXTERN_API( int )
t_rcvrel                        (int                    fd);

EXTERN_API( int )
t_rcvudata                      (int                    fd,
                                 t_unitdata *           unitdata,
                                 int *                  flags);

EXTERN_API( int )
t_rcvuderr                      (int                    fd,
                                 t_uderr *              uderr);

EXTERN_API( int )
t_snd                           (int                    fd,
                                 char *                 buf,
                                 size_t                 nbytes,
                                 int                    flags);

EXTERN_API( int )
t_snddis                        (int                    fd,
                                 t_call *               call);

EXTERN_API( int )
t_sndrel                        (int                    fd);

EXTERN_API( int )
t_sndudata                      (int                    fd,
                                 t_unitdata *           unitdata);

EXTERN_API( int )
t_sync                          (int                    fd);

EXTERN_API( int )
t_unbind                        (int                    fd);

EXTERN_API( int )
t_error                         (char *                 errmsg);

/* Apple extensions*/

EXTERN_API( int )
t_isnonblocking                 (int                    fd);

EXTERN_API( int )
t_asynchronous                  (int                    fd);

EXTERN_API( int )
t_synchronous                   (int                    fd);

EXTERN_API( int )
t_issynchronous                 (int                    fd);

EXTERN_API( int )
t_usesyncidleevents             (int                    fd,
                                 int                    useEvents);

/* Not XTI standard functions, but extensions for transaction endpoints */

EXTERN_API( int )
t_sndrequest                    (int                    fd,
                                 t_request *            req,
                                 int                    flags);

EXTERN_API( int )
t_rcvreply                      (int                    fd,
                                 t_reply *              rep,
                                 int *                  flags);

EXTERN_API( int )
t_rcvrequest                    (int                    fd,
                                 t_request *            req,
                                 int *                  flags);

EXTERN_API( int )
t_sndreply                      (int                    fd,
                                 t_reply *              rep,
                                 int                    flags);

EXTERN_API( int )
t_cancelrequest                 (int                    fd,
                                 long                   sequence);

EXTERN_API( int )
t_cancelreply                   (int                    fd,
                                 long                   sequence);

EXTERN_API( int )
t_sndurequest                   (int                    fd,
                                 t_unitrequest *        ureq,
                                 int                    flags);

EXTERN_API( int )
t_rcvureply                     (int                    fd,
                                 t_unitreply *          urep,
                                 int *                  flags);

EXTERN_API( int )
t_rcvurequest                   (int                    fd,
                                 t_unitrequest *        ureq,
                                 int *                  flags);

EXTERN_API( int )
t_sndureply                     (int                    fd,
                                 t_unitreply *          urep,
                                 int                    flags);

EXTERN_API( int )
t_cancelurequest                (int                    fd,
                                 long                   sequence);

EXTERN_API( int )
t_cancelureply                  (int                    fd,
                                 long                   sequence);

EXTERN_API( int )
t_cancelsynchronouscalls        (int                    fd);

EXTERN_API( int )
t_installnotifier               (int                    fd,
                                 OTNotifyProcPtr        proc,
                                 void *                 contextPtr);

EXTERN_API( void )
t_removenotifier                (int                    fd);

/* STREAMS Primitives*/

EXTERN_API( int )
getmsg                          (int                    fd,
                                 strbuf *               ctlbuf,
                                 strbuf *               databuf,
                                 int *                  flagsp);

EXTERN_API( int )
putmsg                          (int                    fd,
                                 const strbuf *         ctlbuf,
                                 const strbuf *         databuf,
                                 int                    flags);

EXTERN_API( int )
getpmsg                         (int                    fd,
                                 strbuf *               ctlbuf,
                                 strbuf *               databuf,
                                 int *                  bandp,
                                 int *                  flagsp);

EXTERN_API( int )
putpmsg                         (int                    fd,
                                 const strbuf *         ctlbuf,
                                 const strbuf *         databuf,
                                 int                    band,
                                 int                    flags);

/* Raw streams operations.*/

EXTERN_API( int )
stream_installnotifier          (int                    fd,
                                 OTNotifyProcPtr        proc,
                                 void *                 contextPtr);

EXTERN_API( int )
stream_blocking                 (int                    fd);

EXTERN_API( int )
stream_nonblocking              (int                    fd);

EXTERN_API( int )
stream_isblocking               (int                    fd);

EXTERN_API( int )
stream_synchronous              (int                    fd);

EXTERN_API( int )
stream_asynchronous             (int                    fd);

EXTERN_API( int )
stream_issynchronous            (int                    fd);

EXTERN_API( int )
stream_open                     (char *                 path,
                                 unsigned long          flags);

EXTERN_API( int )
stream_close                    (int                    fd);

EXTERN_API( int )
stream_read                     (int                    fd,
                                 void *                 buf,
                                 size_t                 len);

EXTERN_API( int )
stream_write                    (int                    fd,
                                 void *                 buf,
                                 size_t                 len);

EXTERN_API_C( int )
stream_ioctl                    (int                    fd,
                                 unsigned long          cmd,
                                 ...);

EXTERN_API( int )
stream_pipe                     (int *                  fds);

#endif  /* CALL_NOT_IN_CARBON */


struct pollfd {
    SInt32                          fd;
    short                           events;
    short                           revents;
    long                            _ifd;                       /* Internal "fd" for the benefit of the kernel */
};
typedef struct pollfd                   pollfd;
#if CALL_NOT_IN_CARBON
EXTERN_API( int )
poll                            (pollfd *               fds,
                                 size_t                 nfds,
                                 unsigned long          timeout);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !OTKERNEL */


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

#endif /* __OPENTRANSPORTUNIX__ */

