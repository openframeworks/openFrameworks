/*
     File:       OpenTransportProviders.h
 
     Contains:   This file contains provider-specific definitions for various built-in providers.
 
     Version:    Technology: 2.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1993-2001 by Apple Computer, Inc. and Mentat Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OPENTRANSPORTPROVIDERS__
#define __OPENTRANSPORTPROVIDERS__

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


/* ***** TCP/IP ******/

/* Basic types*/

typedef UInt16                          InetPort;
typedef UInt32                          InetHost;
/*  Enums used as address type designations.*/
#ifndef AF_INET /* might conflict with BSD headers */ 
enum {
    AF_INET                     = 2,                            /* Traditonal*/
    AF_DNS                      = 42                            /* Obviously, the answer to...*/
};

#else
enum {
    AF_DNS              = 42    // Obviously, the answer to...
};
#endif
/*
    Enum which can be used to bind to all IP interfaces
    rather than a specific one.
*/
enum {
    kOTAnyInetAddress           = 0                             /* Wildcard*/
};

/*
   Define the InetSvcRef type.  This type needs special
   processing because in C++ it's a subclass of TProvider.
   See the definition of TEndpointRef in "OpenTransport.h"
   for the logic behind this definition.
*/

#ifdef __cplusplus
 typedef class TInternetServices*   InetSvcRef;
#else
 typedef void*                      InetSvcRef;
#endif
#define kDefaultInternetServicesPath        ((OTConfigurationRef)-3L)
/* Shared library prefixes*/


#define kInetVersion   "3.1.1"
#define kInetPrefix     "ot:inet$"

/* Module Names*/


#define kDNRName       "dnr"
#define kTCPName      "tcp"
#define kUDPName      "udp"
#define kRawIPName        "rawip"

/* XTI Options*/

/* Protocol levels*/

enum {
    INET_IP                     = 0x00,
    INET_TCP                    = 0x06,
    INET_UDP                    = 0x11
};

/* TCP Level Options*/

enum {
    TCP_NODELAY                 = 0x01,
    TCP_MAXSEG                  = 0x02,
    TCP_NOTIFY_THRESHOLD        = 0x10,                         /** not a real XTI option */
    TCP_ABORT_THRESHOLD         = 0x11,                         /** not a real XTI option */
    TCP_CONN_NOTIFY_THRESHOLD   = 0x12,                         /** not a real XTI option */
    TCP_CONN_ABORT_THRESHOLD    = 0x13,                         /** not a real XTI option */
    TCP_OOBINLINE               = 0x14,                         /** not a real XTI option */
    TCP_URGENT_PTR_TYPE         = 0x15,                         /** not a real XTI option */
    TCP_KEEPALIVE               = 0x0008                        /* keepalive defined in OpenTransport.h */
};

enum {
    T_GARBAGE                   = 2
};

/* UDP Level Options*/

enum {
    UDP_CHECKSUM                = 0x0600,
    UDP_RX_ICMP                 = 0x02
};

/* IP Level Options*/
enum {
    kIP_OPTIONS                 = 0x01,
    kIP_TOS                     = 0x02,
    kIP_TTL                     = 0x03,
    kIP_REUSEADDR               = 0x04,
    kIP_DONTROUTE               = 0x10,
    kIP_BROADCAST               = 0x20,
    kIP_REUSEPORT               = 0x0200,
    kIP_HDRINCL                 = 0x1002,
    kIP_RCVOPTS                 = 0x1005,
    kIP_RCVDSTADDR              = 0x1007,
    kIP_MULTICAST_IF            = 0x1010,                       /* set/get IP multicast interface */
    kIP_MULTICAST_TTL           = 0x1011,                       /* set/get IP multicast timetolive    */
    kIP_MULTICAST_LOOP          = 0x1012,                       /* set/get IP multicast loopback  */
    kIP_ADD_MEMBERSHIP          = 0x1013,                       /* add an IP group membership     */
    kIP_DROP_MEMBERSHIP         = 0x1014,                       /* drop an IP group membership       */
    kIP_BROADCAST_IFNAME        = 0x1015,                       /* Set interface for broadcasts   */
    kIP_RCVIFADDR               = 0x1016                        /* Set interface for broadcasts   */
};

/*
 * The value of some of the following constants may conflict with 
 * the values found in the BSD header netinet/in.h.
 * Use instead the constant defined in the enum, for example use 
 * kIP_OPTIONS instead of IP_OPTIONS.
 */
#ifndef IP_OPTIONS
enum {
    IP_OPTIONS                  = kIP_OPTIONS,
    IP_TOS                      = kIP_TOS,
    IP_TTL                      = kIP_TTL,
    IP_REUSEADDR                = kIP_REUSEADDR,
    IP_DONTROUTE                = kIP_DONTROUTE,
    IP_BROADCAST                = kIP_BROADCAST,
    IP_REUSEPORT                = kIP_REUSEPORT,
    IP_HDRINCL                  = kIP_HDRINCL,
    IP_RCVOPTS                  = kIP_RCVOPTS,
    IP_RCVDSTADDR               = kIP_RCVDSTADDR,
    IP_MULTICAST_IF             = kIP_MULTICAST_IF,             /* set/get IP multicast interface */
    IP_MULTICAST_TTL            = kIP_MULTICAST_TTL,            /* set/get IP multicast timetolive    */
    IP_MULTICAST_LOOP           = kIP_MULTICAST_LOOP,           /* set/get IP multicast loopback  */
    IP_ADD_MEMBERSHIP           = kIP_ADD_MEMBERSHIP,           /* add an IP group membership     */
    IP_DROP_MEMBERSHIP          = kIP_DROP_MEMBERSHIP,          /* drop an IP group membership       */
    IP_BROADCAST_IFNAME         = kIP_BROADCAST_IFNAME,         /* Set interface for broadcasts   */
    IP_RCVIFADDR                = kIP_RCVIFADDR                 /* Set interface for broadcasts   */
};

#endif

enum {
    DVMRP_INIT                  = 100,                          /* DVMRP-specific setsockopt commands, from ip_mroute.h*/
    DVMRP_DONE                  = 101,
    DVMRP_ADD_VIF               = 102,
    DVMRP_DEL_VIF               = 103,
    DVMRP_ADD_LGRP              = 104,
    DVMRP_DEL_LGRP              = 105,
    DVMRP_ADD_MRT               = 106,
    DVMRP_DEL_MRT               = 107
};


/* IP_TOS precdence levels*/

enum {
    T_ROUTINE                   = 0,
    T_PRIORITY                  = 1,
    T_IMMEDIATE                 = 2,
    T_FLASH                     = 3,
    T_OVERRIDEFLASH             = 4,
    T_CRITIC_ECP                = 5,
    T_INETCONTROL               = 6,
    T_NETCONTROL                = 7
};

/*  IP_TOS type of service*/

enum {
    T_NOTOS                     = 0x00,
    T_LDELAY                    = (1 << 4),
    T_HITHRPT                   = (1 << 3),
    T_HIREL                     = (1 << 2)
};

#define SET_TOS(prec,tos)   (((0x7 & (prec)) << 5) | (0x1c & (tos)))
/* IP Multicast option structures*/


struct TIPAddMulticast {
    InetHost                        multicastGroupAddress;
    InetHost                        interfaceAddress;
};
typedef struct TIPAddMulticast          TIPAddMulticast;
/* Protocol-specific events*/
enum {
    T_DNRSTRINGTOADDRCOMPLETE   = kPRIVATEEVENT + 1,
    T_DNRADDRTONAMECOMPLETE     = kPRIVATEEVENT + 2,
    T_DNRSYSINFOCOMPLETE        = kPRIVATEEVENT + 3,
    T_DNRMAILEXCHANGECOMPLETE   = kPRIVATEEVENT + 4,
    T_DNRQUERYCOMPLETE          = kPRIVATEEVENT + 5
};

/* InetAddress*/


struct InetAddress {
    OTAddressType                   fAddressType;               /* always AF_INET*/
    InetPort                        fPort;                      /* Port number */
    InetHost                        fHost;                      /* Host address in net byte order*/
    UInt8                           fUnused[8];                 /* Traditional unused bytes*/
};
typedef struct InetAddress              InetAddress;
/* Domain Name Resolver (DNR) */
enum {
    kMaxHostAddrs               = 10,
    kMaxSysStringLen            = 32,
    kMaxHostNameLen             = 255
};


typedef char                            InetDomainName[256];

struct InetHostInfo {
    InetDomainName                  name;
    InetHost                        addrs[10];
};
typedef struct InetHostInfo             InetHostInfo;

struct InetSysInfo {
    char                            cpuType[32];
    char                            osType[32];
};
typedef struct InetSysInfo              InetSysInfo;

struct InetMailExchange {
    UInt16                          preference;
    InetDomainName                  exchange;
};
typedef struct InetMailExchange         InetMailExchange;

struct DNSQueryInfo {
    UInt16                          qType;
    UInt16                          qClass;
    UInt32                          ttl;
    InetDomainName                  name;
    UInt16                          responseType;               /* answer, authority, or additional*/
    UInt16                          resourceLen;                /* actual length of array which follows*/
    char                            resourceData[4];            /* size varies*/
};
typedef struct DNSQueryInfo             DNSQueryInfo;
/* DNSAddress*/
/*
   The DNSAddress format is optional and may be used in connects,
   datagram sends, and resolve address calls.   The name takes the 
   format "somewhere.com" or "somewhere.com:portnumber" where
   the ":portnumber" is optional.   The length of this structure
   is arbitrarily limited to the overall max length of a domain
   name (255 chars), although a longer one can be use successfully
   if you use this as a template for doing so.   However, the domain name 
   is still limited to 255 characters.
*/


struct DNSAddress {
    OTAddressType                   fAddressType;               /* always AF_DNS*/
    InetDomainName                  fName;
};
typedef struct DNSAddress               DNSAddress;
/* InetInterfaceInfo*/
enum {
    kDefaultInetInterface       = -1
};

enum {
    kInetInterfaceInfoVersion   = 3
};


struct InetInterfaceInfo {
    InetHost                        fAddress;
    InetHost                        fNetmask;
    InetHost                        fBroadcastAddr;
    InetHost                        fDefaultGatewayAddr;
    InetHost                        fDNSAddr;
    UInt16                          fVersion;
    UInt16                          fHWAddrLen;
    UInt8 *                         fHWAddr;
    UInt32                          fIfMTU;
    UInt8 *                         fReservedPtrs[2];
    InetDomainName                  fDomainName;
    UInt32                          fIPSecondaryCount;
    UInt8                           fReserved[252];
};
typedef struct InetInterfaceInfo        InetInterfaceInfo;
/* InetDHCPOption*/
enum {
    kAllDHCPOptions             = -1,
    kDHCPLongOption             = 126,
    kDHCPLongOptionReq          = 127
};


struct InetDHCPOption {
    UInt8                           fOptionTag;
    UInt8                           fOptionLen;
    UInt8                           fOptionValue;
};
typedef struct InetDHCPOption           InetDHCPOption;
/* TCP/IP Utility Routines*/

EXTERN_API( void )
OTInitInetAddress               (InetAddress *          addr,
                                 InetPort               port,
                                 InetHost               host);

EXTERN_API( OTByteCount )
OTInitDNSAddress                (DNSAddress *           addr,
                                 char *                 str);

EXTERN_API( OSStatus )
OTInetStringToHost              (char *                 str,
                                 InetHost *             host);

EXTERN_API( void )
OTInetHostToString              (InetHost               host,
                                 char *                 str);

EXTERN_API( OSStatus )
OTInetGetInterfaceInfo          (InetInterfaceInfo *    info,
                                 SInt32                 val);

EXTERN_API( OSStatus )
OTInetGetSecondaryAddresses     (InetHost *             addr,
                                 UInt32 *               count,
                                 SInt32                 val);

#if CALL_NOT_IN_CARBON
EXTERN_API( OSStatus )
OTInetGetDHCPConfigInfo         (InetDHCPOption *       buf,
                                 UInt32                 bufSize,
                                 SInt32                 index,
                                 SInt32                 opt);

/* InetServices & DNR Calls*/

#endif  /* CALL_NOT_IN_CARBON */

#if !OTKERNEL
/*
   Under Carbon, OTOpenInternetServices routines take a client context pointer.  Applications may pass NULL
   after calling InitOpenTransport(kInitOTForApplicationMask, ...).  Non-applications must always pass a
   valid client context.
*/
EXTERN_API( InetSvcRef )
OTOpenInternetServicesInContext (OTConfigurationRef     cfig,
                                 OTOpenFlags            oflag,
                                 OSStatus *             err,
                                 OTClientContextPtr     clientContext);

EXTERN_API( OSStatus )
OTAsyncOpenInternetServicesInContext (OTConfigurationRef  cfig,
                                 OTOpenFlags            oflag,
                                 OTNotifyUPP            upp,
                                 void *                 contextPtr,
                                 OTClientContextPtr     clientContext);

#if CALL_NOT_IN_CARBON
EXTERN_API( InetSvcRef )
OTOpenInternetServices          (OTConfigurationRef     cfig,
                                 OTOpenFlags            oflag,
                                 OSStatus *             err);

EXTERN_API( OSStatus )
OTAsyncOpenInternetServices     (OTConfigurationRef     cfig,
                                 OTOpenFlags            oflag,
                                 OTNotifyProcPtr        proc,
                                 void *                 contextPtr);

#endif  /* CALL_NOT_IN_CARBON */

#if TARGET_API_MAC_CARBON
/* The following macro may be used by applications only.*/
#define OTOpenInternetServices(cfig, oflags, err) OTOpenInternetServicesInContext(cfig, oflags, err, NULL)
#define OTAsyncOpenInternetServices(cfig, oflags, proc, contextPtr)  OTAsyncOpenInternetServicesInContext(cfig, oflags, proc, contextPtr, NULL)
#endif  /* TARGET_API_MAC_CARBON */

EXTERN_API( OSStatus )
OTInetStringToAddress           (InetSvcRef             ref,
                                 char *                 name,
                                 InetHostInfo *         hinfo);

EXTERN_API( OSStatus )
OTInetAddressToName             (InetSvcRef             ref,
                                 InetHost               addr,
                                 InetDomainName         name);

EXTERN_API( OSStatus )
OTInetSysInfo                   (InetSvcRef             ref,
                                 char *                 name,
                                 InetSysInfo *          sysinfo);

EXTERN_API( OSStatus )
OTInetMailExchange              (InetSvcRef             ref,
                                 char *                 name,
                                 UInt16 *               num,
                                 InetMailExchange *     mx);

EXTERN_API( OSStatus )
OTInetQuery                     (InetSvcRef             ref,
                                 char *                 name,
                                 UInt16                 qClass,
                                 UInt16                 qType,
                                 char *                 buf,
                                 OTByteCount            buflen,
                                 void **                argv,
                                 OTByteCount            argvlen,
                                 OTFlags                flags);

#ifdef __cplusplus
}                   // Terminate C definitions

class TInternetServices : public TProvider
{
    public:
            OSStatus    StringToAddress(char* name, InetHostInfo* hinfo)
                        { return OTInetStringToAddress(this, name, hinfo); }
                    
            OSStatus    AddressToName(InetHost addr, InetDomainName name)
                        { return OTInetAddressToName(this, addr, name); }
                    
            OSStatus    SysInfo(char* name, InetSysInfo* sysinfo )
                        { return OTInetSysInfo(this, name, sysinfo); }
                    
            OSStatus    MailExchange(char* name, UInt16* num, InetMailExchange* mx)
                        { return OTInetMailExchange(this, name, num, mx); }
                        
            OSStatus    Query(char* name, UInt16 qClass, UInt16 qType, 
                              char* buf, OTByteCount buflen, 
                              void** argv, OTByteCount argvlen,
                              OTFlags flags)
                        { return OTInetQuery(this, name, qClass, qType, buf, buflen, argv, argvlen, flags); }
};

extern "C" {        /* resume C definitions */
#endif
#endif  /* !OTKERNEL */

/* ***** AppleTalk ******/
/* Shared library prefixes*/


#define kATalkVersion  "1.1"
#define kATalkPrefix  "ot:atlk$"
#define kATBinderID      "ot:atbd$"

/*******************************************************************************
** Module definitions
********************************************************************************/
/* XTI Levels*/

enum {
    ATK_DDP                     = FOUR_CHAR_CODE('DDP '),
    ATK_AARP                    = FOUR_CHAR_CODE('AARP'),
    ATK_ATP                     = FOUR_CHAR_CODE('ATP '),
    ATK_ADSP                    = FOUR_CHAR_CODE('ADSP'),
    ATK_ASP                     = FOUR_CHAR_CODE('ASP '),
    ATK_PAP                     = FOUR_CHAR_CODE('PAP '),
    ATK_NBP                     = FOUR_CHAR_CODE('NBP '),
    ATK_ZIP                     = FOUR_CHAR_CODE('ZIP ')
};

/* Module Names*/


#define kDDPName       "ddp"
#define   kATPName        "atp"
#define kADSPName     "adsp"
#define  kASPName        "asp"
#define kPAPName      "pap"
#define kNBPName      "nbp"
#define kZIPName      "zip"
#define kLTalkName        "ltlk"
#define kLTalkAName      "ltlkA"
#define kLTalkBName     "ltlkB"

/*
   Protocol-specific Options
   NOTE:
   All Protocols support OPT_CHECKSUM (Value is (unsigned long)T_YES/T_NO)
   ATP supports OPT_RETRYCNT (# Retries, 0 = try once) and
                OPT_INTERVAL (# Milliseconds to wait)
*/

enum {
    DDP_OPT_CHECKSUM            = 0x0600,
    DDP_OPT_SRCADDR             = 0x2101,                       /* DDP UnitDataReq Only - set src address, Value is DDPAddress */
    ATP_OPT_REPLYCNT            = 0x2110,                       /* AppleTalk - ATP Resp Pkt Ct Type, Value is (unsigned long)  pkt count */
    ATP_OPT_DATALEN             = 0x2111,                       /* AppleTalk - ATP Pkt Data Len Type, Value is (unsigned long) length */
    ATP_OPT_RELTIMER            = 0x2112,                       /* AppleTalk - ATP Release Timer Type, Value is (unsigned long) timer, (See Inside AppleTalk, second edition */
    ATP_OPT_TRANID              = 0x2113,                       /* Value is (unsigned long) Boolean, Used to request Transaction ID, Returned with Transaction ID on requests */
    PAP_OPT_OPENRETRY           = 0x2120                        /* AppleTalk - PAP OpenConn Retry count, Value is (unsigned long) T_YES/T_NO */
};

/* Protocol-Specific Events*/

/*
   If you send the IOCTL: OTIoctl(I_OTGetMiscellaneousEvents, 1),
   you will receive the T_ATALKxxx events on your endpoint.
   NOTE: The endpoint does not need to be bound.
*/

enum {
    kAppleTalkEvent             = kPROTOCOLEVENT | 0x00010000,
    T_GETMYZONECOMPLETE         = kAppleTalkEvent + 1,
    T_GETLOCALZONESCOMPLETE     = kAppleTalkEvent + 2,
    T_GETZONELISTCOMPLETE       = kAppleTalkEvent + 3,
    T_GETATALKINFOCOMPLETE      = kAppleTalkEvent + 4,
    T_ATALKROUTERDOWNEVENT      = kAppleTalkEvent + 51,         /* No routers have been seen for a while.  If the cookie is NULL, all routers are gone.  Otherwise, there is still an ARA router hanging around being used, and only the local cable has been  timed out.*/
    T_ATALKROUTERUPEVENT        = kAppleTalkEvent + 52,         /* We didn't have a router, but now one has come up. Cookie is NULL for a normal router coming up, non-NULL for an ARA router coming on-line*/
    T_ATALKZONENAMECHANGEDEVENT = kAppleTalkEvent + 53,         /* A Zone name change was issued from the router, so our AppleTalk Zone has changed.*/
    T_ATALKCONNECTIVITYCHANGEDEVENT = kAppleTalkEvent + 54,     /* An ARA connection was established (cookie != NULL), or was disconnected (cookie == NULL)*/
    T_ATALKINTERNETAVAILABLEEVENT = kAppleTalkEvent + 55,       /* A router has appeared, and our address is in the startup range.  Cookie is hi/lo of new cable range.*/
    T_ATALKCABLERANGECHANGEDEVENT = kAppleTalkEvent + 56        /* A router has appeared, and it's incompatible with our current address.  Cookie is hi/lo of new cable range.*/
};

enum {
    kAllATalkRoutersDown        = 0,                            /* This indicates that all routers are offline*/
    kLocalATalkRoutersDown      = -1L,                          /* This indicates that all local routers went offline, but an ARA router is still active*/
    kARARouterDisconnected      = -2L                           /* This indicates that ARA was disconnected, do it's router went offline, and we have no local routers to fall back onto.*/
};

enum {
    kARARouterOnline            = -1L,                          /* We had no local routers, but an ARA router is now online.*/
    kATalkRouterOnline          = 0,                            /* We had no routers, but a local router is now online*/
    kLocalATalkRouterOnline     = -2L                           /* We have an ARA router, but now we've seen a local router as well*/
};

#define IsAppleTalkEvent(x)         ((x) & 0xffff0000) == kAppleTalkEvent)
/* Protocol-specific IOCTLs*/

enum {
    ATALK_IOC_FULLSELFSEND      = ((MIOC_ATALK << 8) | 47),     /* Turn on/off full self-send (it's automatic for non-backward-compatible links)*/
    ADSP_IOC_FORWARDRESET       = ((MIOC_ATALK << 8) | 60)      /* ADSP Forward Reset*/
};

/* Protocol-specific constants*/

/* ECHO*/

enum {
    kECHO_TSDU                  = 585                           /* Max. # of data bytes.*/
};

/* NBP*/

enum {
    kNBPMaxNameLength           = 32,
    kNBPMaxTypeLength           = 32,
    kNBPMaxZoneLength           = 32,
    kNBPSlushLength             = 9,                            /* Extra space for @, : and a few escape chars*/
    kNBPMaxEntityLength         = (kNBPMaxNameLength + kNBPMaxTypeLength + kNBPMaxZoneLength + 3),
    kNBPEntityBufferSize        = (kNBPMaxNameLength + kNBPMaxTypeLength + kNBPMaxZoneLength + kNBPSlushLength),
    kNBPWildCard                = 0x3D,                         /* NBP name and type match anything '='*/
    kNBPImbeddedWildCard        = 0xC5,                         /* NBP name and type match some '*'*/
    kNBPDefaultZone             = 0x2A                          /* NBP default zone '*'*/
};

/* ZIP*/

enum {
    kZIPMaxZoneLength           = kNBPMaxZoneLength
};

enum {
    kDDPAddressLength           = 8,                            /* value to use in netbuf.len field, Maximum length of AppleTalk address*/
    kNBPAddressLength           = kNBPEntityBufferSize,
    kAppleTalkAddressLength     = kDDPAddressLength + kNBPEntityBufferSize
};


#define OTCopyDDPAddress(addr, dest)               \
  {                                               \
      ((UInt32*)(dest))[0] = ((UInt32*)(addr))[0];    \
      ((UInt32*)(dest))[1] = ((UInt32*)(addr))[1];    \
  }

/*******************************************************************************
** CLASS TAppleTalkServices
********************************************************************************/
#if !OTKERNEL
/*
   Define the ATSvcRef type.  This type needs special
   processing because in C++ it's a subclass of TProvider.
   See the definition of TEndpointRef in "OpenTransport.h"
   for the logic behind this definition.
*/
#ifdef __cplusplus
    typedef class TAppleTalkServices*   ATSvcRef;
#else
    typedef void*   ATSvcRef;
#endif
#define kDefaultAppleTalkServicesPath       ((OTConfigurationRef)-3L)
/*
   Under Carbon, OpenAppleTalkServices routines take a client context pointer.  Applications may pass NULL
   after calling InitOpenTransport(kInitOTForApplicationMask, ...).  Non-applications must always pass a
   valid client context.
*/
EXTERN_API( OSStatus )
OTAsyncOpenAppleTalkServicesInContext (OTConfigurationRef  cfig,
                                 OTOpenFlags            flags,
                                 OTNotifyUPP            proc,
                                 void *                 contextPtr,
                                 OTClientContextPtr     clientContext);

EXTERN_API( ATSvcRef )
OTOpenAppleTalkServicesInContext (OTConfigurationRef    cfig,
                                 OTOpenFlags            flags,
                                 OSStatus *             err,
                                 OTClientContextPtr     clientContext);

#if CALL_NOT_IN_CARBON
EXTERN_API( OSStatus )
OTAsyncOpenAppleTalkServices    (OTConfigurationRef     cfig,
                                 OTOpenFlags            flags,
                                 OTNotifyProcPtr        proc,
                                 void *                 contextPtr);

EXTERN_API( ATSvcRef )
OTOpenAppleTalkServices         (OTConfigurationRef     cfig,
                                 OTOpenFlags            flags,
                                 OSStatus *             err);

#endif  /* CALL_NOT_IN_CARBON */

#if TARGET_API_MAC_CARBON
/* The following macro may be used by applications only.*/
#define OTOpenAppleTalkServices(cfig, oflags, err) OTOpenAppleTalkServicesInContext(cfig, oflags, err, NULL)
#define OTAsyncOpenAppleTalkServices(cfig, oflags, proc, contextPtr) OTAsyncOpenAppleTalkServicesInContext(cfig, oflags, proc, contextPtr, NULL)
#endif  /* TARGET_API_MAC_CARBON */

/* Get the zone associated with the ATSvcRef*/
EXTERN_API( OSStatus )
OTATalkGetMyZone                (ATSvcRef               ref,
                                 TNetbuf *              zone);

/*
   Get the list of available zones associated with the local cable
   of the ATSvcRef
*/
EXTERN_API( OSStatus )
OTATalkGetLocalZones            (ATSvcRef               ref,
                                 TNetbuf *              zones);

/* Get the list of all zones on the internet specified by the ATSvcRef*/
EXTERN_API( OSStatus )
OTATalkGetZoneList              (ATSvcRef               ref,
                                 TNetbuf *              zones);

/* Stores an AppleTalkInfo structure into the TNetbuf (see later in this file)*/
EXTERN_API( OSStatus )
OTATalkGetInfo                  (ATSvcRef               ref,
                                 TNetbuf *              info);

#ifdef __cplusplus
}                   // Terminate C definitions

class TAppleTalkServices : public TProvider
{
    public:
            OSStatus    GetMyZone(TNetbuf* zone)        { return OTATalkGetMyZone(this, zone); }
            OSStatus    GetLocalZones(TNetbuf* zones)   { return OTATalkGetLocalZones(this, zones); }
            OSStatus    GetZoneList(TNetbuf* zones)     { return OTATalkGetZoneList(this, zones); }
            OSStatus    GetInfo(TNetbuf* info)          { return OTATalkGetInfo(this, info); }
};

extern "C" {        /* resume C definitions */
#endif  /* _cplus */
#endif  /* !OTKERNEL */

/* AppleTalk Addressing*/
/*
   The NBPEntity structure is used to manipulate NBP names without regard
   to issues of what kind of "special" characters are in the name.
   
   When stored as an address in an NBPAddress or DDPNBPAddress, they are 
   stored as a character string, which is currently just ASCII, but in the
   future may be UniChar, or some other internationalizable scripting set.
   The string following an NBPAddress or DDPNBPAddress is intended to be
   suitable for showing to users, whereas NBPEntity is not.
   WARNING: NBPAddress and DDPNBPAddress structures do not "know" the length
   of the address.  That must have been obtained as part of a Lookup or
   ResolveAddress call.
*/

enum {
    AF_ATALK_FAMILY             = 0x0100,
    AF_ATALK_DDP                = 0x0100,
    AF_ATALK_DDPNBP             = AF_ATALK_FAMILY + 1,
    AF_ATALK_NBP                = AF_ATALK_FAMILY + 2,
    AF_ATALK_MNODE              = AF_ATALK_FAMILY + 3
};


struct NBPEntity {
    UInt8                           fEntity[99];
};
typedef struct NBPEntity                NBPEntity;

struct DDPAddress {
    OTAddressType                   fAddressType;               /* One of the enums above*/
    UInt16                          fNetwork;
    UInt8                           fNodeID;
    UInt8                           fSocket;
    UInt8                           fDDPType;
    UInt8                           fPad;

#ifdef __cplusplus
    // C++ inline methods on this structure.

  void            Init(const DDPAddress&);
   void            Init(UInt16 net, UInt8 node, UInt8 socket);
    void            Init(UInt16 net, UInt8 node, UInt8 socket, UInt8 type);

   void            SetSocket(UInt8);
  void            SetType(UInt8);
    void            SetNode(UInt8);
    void            SetNetwork(UInt16);

   OTByteCount     GetAddressLength() const;
  OTAddressType   GetAddressType() const;
    UInt8           GetSocket() const;
 UInt8           GetType() const;
   UInt8           GetNode() const;
   UInt16          GetNetwork() const;

   Boolean         operator==(const DDPAddress&) const;
   Boolean         operator!=(const DDPAddress&) const;
   void            operator=(const DDPAddress&);
#endif


};
typedef struct DDPAddress               DDPAddress;

struct NBPAddress {
    OTAddressType                   fAddressType;               /* One of the enums above*/
    UInt8                           fNBPNameBuffer[105];

#ifdef __cplusplus
    // C++ inline methods on this structure.

  OTByteCount     Init();
    OTByteCount     Init(const NBPEntity&);
    OTByteCount     Init(const char*);
 OTByteCount     Init(const char*, OTByteCount len);
    Boolean         ExtractEntity(NBPEntity&, OTByteCount len);

   OTAddressType   GetAddressType() const;
#endif


};
typedef struct NBPAddress               NBPAddress;

struct DDPNBPAddress {
    OTAddressType                   fAddressType;               /* One of the enums above*/
    UInt16                          fNetwork;
    UInt8                           fNodeID;
    UInt8                           fSocket;
    UInt8                           fDDPType;
    UInt8                           fPad;
    UInt8                           fNBPNameBuffer[105];

#ifdef __cplusplus
    // C++ inline methods on this structure.

  void            Init(const DDPAddress&);
   void            Init(UInt16 net, UInt8 node, UInt8 socket);
    void            Init(UInt16 net, UInt8 node, UInt8 socket, UInt8 type);

   void            SetSocket(UInt8);
  void            SetType(UInt8);
    void            SetNode(UInt8);
    void            SetNetwork(UInt16);
    
   OTAddressType   GetAddressType() const;
    UInt8           GetSocket() const;
 UInt8           GetType() const;
   UInt8           GetNode() const;
   UInt16          GetNetwork() const;
    
   Boolean         ExtractEntity(NBPEntity&, OTByteCount len);
    OTByteCount     SetNBPEntity(const NBPEntity&);
    OTByteCount     SetNBPEntity(const char*);
 OTByteCount     SetNBPEntity(const char*, OTByteCount len);
    
   Boolean         operator==(const DDPAddress&) const;
#endif


};
typedef struct DDPNBPAddress            DDPNBPAddress;
/* These are some utility routines for dealing with NBP and DDP addresses. */

/* Functions to initialize the various AppleTalk Address types*/
EXTERN_API( void )
OTInitDDPAddress                (DDPAddress *           addr,
                                 UInt16                 net,
                                 UInt8                  node,
                                 UInt8                  socket,
                                 UInt8                  ddpType);

EXTERN_API( OTByteCount )
OTInitNBPAddress                (NBPAddress *           addr,
                                 const char *           name);

EXTERN_API( OTByteCount )
OTInitDDPNBPAddress             (DDPNBPAddress *        addr,
                                 const char *           name,
                                 UInt16                 net,
                                 UInt8                  node,
                                 UInt8                  socket,
                                 UInt8                  ddpType);

/* Compare 2 DDP addresses for equality*/
EXTERN_API( Boolean )
OTCompareDDPAddresses           (const DDPAddress *     addr1,
                                 const DDPAddress *     addr2);

/* Init an NBPEntity to a NULL name*/
EXTERN_API( void )
OTInitNBPEntity                 (NBPEntity *            entity);

/* Get the length an NBPEntity would have when stored as an address*/
EXTERN_API( OTByteCount )
OTGetNBPEntityLengthAsAddress   (const NBPEntity *      entity);

/* Store an NBPEntity into an address buffer*/
EXTERN_API( OTByteCount )
OTSetAddressFromNBPEntity       (UInt8 *                nameBuf,
                                 const NBPEntity *      entity);

/* Create an address buffer from a string (use -1 for len to use strlen)*/
EXTERN_API( OTByteCount )
OTSetAddressFromNBPString       (UInt8 *                addrBuf,
                                 const char *           name,
                                 SInt32                 len);

/*
   Create an NBPEntity from an address buffer. False is returned if
     the address was truncated.
*/
EXTERN_API( Boolean )
OTSetNBPEntityFromAddress       (NBPEntity *            entity,
                                 const UInt8 *          addrBuf,
                                 OTByteCount            len);

/* Routines to set a piece of an NBP entity from a character string*/
EXTERN_API( Boolean )
OTSetNBPName                    (NBPEntity *            entity,
                                 const char *           name);

EXTERN_API( Boolean )
OTSetNBPType                    (NBPEntity *            entity,
                                 const char *           typeVal);

EXTERN_API( Boolean )
OTSetNBPZone                    (NBPEntity *            entity,
                                 const char *           zone);

/* Routines to extract pieces of an NBP entity*/
EXTERN_API( void )
OTExtractNBPName                (const NBPEntity *      entity,
                                 char *                 name);

EXTERN_API( void )
OTExtractNBPType                (const NBPEntity *      entity,
                                 char *                 typeVal);

EXTERN_API( void )
OTExtractNBPZone                (const NBPEntity *      entity,
                                 char *                 zone);

#ifdef __cplusplus

// Inline methods for DDPAddress

inline void DDPAddress::operator=(const DDPAddress& addr)
{
    *(UInt32*)&fAddressType = *(UInt32*)&addr.fAddressType;
    *(UInt32*)&fNodeID = *(UInt32*)&addr.fNodeID;
}

inline Boolean DDPAddress::operator==(const DDPAddress& addr) const
{
    return OTCompareDDPAddresses(&addr, this);
}

inline Boolean DDPAddress::operator!=(const DDPAddress& addr) const
{
    return !OTCompareDDPAddresses(&addr, this);
}

inline void DDPAddress::SetSocket(UInt8 socket)
{
    fSocket = socket;
}

inline void DDPAddress::SetNode(UInt8 node)
{
    fNodeID = node;
}

inline void DDPAddress::SetType(UInt8 type)
{
    fDDPType = type;
}

inline void DDPAddress::SetNetwork(UInt16 net)
{
    fNetwork = net;
}

inline OTByteCount DDPAddress::GetAddressLength() const
{
    return kDDPAddressLength;
}

inline OTAddressType DDPAddress::GetAddressType() const
{
    return fAddressType;
}

inline UInt8 DDPAddress::GetSocket() const
{
    return fSocket;
}

inline UInt8 DDPAddress::GetNode() const
{
    return fNodeID;
}

inline UInt8 DDPAddress::GetType() const
{
    return fDDPType;
}

inline UInt16 DDPAddress::GetNetwork() const
{
    return fNetwork;
}

inline void  DDPAddress::Init(UInt16 net, UInt8 node,
                              UInt8 socket)
{
    fAddressType = AF_ATALK_DDP;
    SetNetwork(net);
    SetNode(node);
    SetSocket(socket);
    SetType(0);
}

inline void  DDPAddress::Init(UInt16 net, UInt8 node,
                              UInt8 socket, UInt8 type)
{
    fAddressType = AF_ATALK_DDP;
    SetNetwork(net);
    SetNode(node);
    SetSocket(socket);
    SetType(type);
}

inline void DDPAddress::Init(const DDPAddress& addr)
{
    *(UInt32*)&fAddressType = *(UInt32*)&addr.fAddressType;
    *(UInt32*)&fNodeID = *(UInt32*)&addr.fNodeID;
}

// Inline methods for NBPAddress

inline OTByteCount NBPAddress::Init()
{
    fAddressType = AF_ATALK_NBP;
    return sizeof(OTAddressType);
}

inline OTByteCount NBPAddress::Init(const NBPEntity& addr)
{
    fAddressType = AF_ATALK_NBP;
    return sizeof(OTAddressType) + OTSetAddressFromNBPEntity(fNBPNameBuffer, &addr);
}

inline OTByteCount NBPAddress::Init(const char* name)
{
    fAddressType = AF_ATALK_NBP;
    return sizeof(OTAddressType) + OTSetAddressFromNBPString(fNBPNameBuffer, name, -1);
}

inline OTByteCount NBPAddress::Init(const char* name, OTByteCount len)
{
    fAddressType = AF_ATALK_NBP;
    return sizeof(OTAddressType) + OTSetAddressFromNBPString(fNBPNameBuffer, name, (SInt32)len);
}

inline Boolean NBPAddress::ExtractEntity(NBPEntity& entity, OTByteCount len)
{
    return OTSetNBPEntityFromAddress(&entity, fNBPNameBuffer, len);
}
    
inline OTAddressType NBPAddress::GetAddressType() const
{
    return fAddressType;
}
    
// Inline methods for DDPNBPAddress

inline Boolean DDPNBPAddress::operator==(const DDPAddress& addr) const
{
    return OTCompareDDPAddresses((const DDPAddress*)this, &addr);
}

inline void DDPNBPAddress::SetSocket(UInt8 socket)
{
    fSocket = socket;
}

inline void DDPNBPAddress::SetNode(UInt8 node)
{
    fNodeID = node;
}

inline void DDPNBPAddress::SetType(UInt8 type)
{
    fDDPType = type;
}

inline void DDPNBPAddress::SetNetwork(UInt16 net)
{
    fNetwork = net;
}

inline OTAddressType DDPNBPAddress::GetAddressType() const
{
    return fAddressType;
}

inline UInt8 DDPNBPAddress::GetSocket() const
{
    return fSocket;
}

inline UInt8 DDPNBPAddress::GetNode() const
{
    return fNodeID;
}

inline UInt8 DDPNBPAddress::GetType() const
{
    return fDDPType;
}

inline UInt16 DDPNBPAddress::GetNetwork() const
{
    return fNetwork;
}

inline void DDPNBPAddress::Init(UInt16 net, UInt8 node,
                                UInt8 socket)
{
    fAddressType = AF_ATALK_DDPNBP;
    SetNetwork(net);
    SetNode(node);
    SetSocket(socket);
    SetType(0);
}

inline void DDPNBPAddress::Init(UInt16 net, UInt8 node,
                                UInt8 socket, UInt8 type)
{
    fAddressType = AF_ATALK_DDPNBP;
    SetNetwork(net);
    SetNode(node);
    SetSocket(socket);
    SetType(type);
}

inline void DDPNBPAddress::Init(const DDPAddress& addr)
{
    fAddressType = AF_ATALK_DDPNBP;
    SetNetwork(addr.GetNetwork());
    SetNode(addr.GetNode());
    SetSocket(addr.GetSocket());
    SetType(addr.GetType());
    fNBPNameBuffer[0] = 0;
}

inline OTByteCount DDPNBPAddress::SetNBPEntity(const NBPEntity& entity)
{
    return OTSetAddressFromNBPEntity(fNBPNameBuffer, &entity) + kDDPAddressLength;
}

inline OTByteCount DDPNBPAddress::SetNBPEntity(const char* name)
{
    return OTSetAddressFromNBPString(fNBPNameBuffer, name, -1) + kDDPAddressLength;
}

inline OTByteCount DDPNBPAddress::SetNBPEntity(const char* name, OTByteCount len)
{
    return OTSetAddressFromNBPString(fNBPNameBuffer, name, (SInt32)len) + kDDPAddressLength;
}

inline Boolean DDPNBPAddress::ExtractEntity(NBPEntity& entity, OTByteCount len)
{
    return OTSetNBPEntityFromAddress(&entity, fNBPNameBuffer, len);
}

#endif  /* __cplusplus */
/* AppleTalkInfo as used by the OTGetATalkInfo function*/


struct AppleTalkInfo {
    DDPAddress                      fOurAddress;                /* Our DDP address (network # & node)*/
    DDPAddress                      fRouterAddress;             /* The address of a router on our cable*/
    UInt16                          fCableRange[2];             /* The current cable range*/
    UInt16                          fFlags;                     /* See below*/
};
typedef struct AppleTalkInfo            AppleTalkInfo;
/* For the fFlags field in AppleTalkInfo*/
enum {
    kATalkInfoIsExtended        = 0x0001,                       /* This is an extended (phase 2) network*/
    kATalkInfoHasRouter         = 0x0002,                       /* This cable has a router*/
    kATalkInfoOneZone           = 0x0004                        /* This cable has only one zone*/
};

/* ***** Ethernet ******/

/* Interface option flags*/

/* Ethernet framing options*/

enum {
    kOTFramingEthernet          = 0x01,
    kOTFramingEthernetIPX       = 0x02,
    kOTFraming8023              = 0x04,
    kOTFraming8022              = 0x08
};

/*
   These are obsolete and will be going away in OT 1.5.
   Hmmm, OT 1.5 got cancelled.  The status of these options
   is uncertain.
*/

/* RawMode options*/

enum {
    kOTRawRcvOn                 = 0,
    kOTRawRcvOff                = 1,
    kOTRawRcvOnWithTimeStamp    = 2
};

enum {
    DL_PROMISC_OFF              = 0                             /* OPT_SETPROMISCUOUS value*/
};

/* Module definitions*/

/* Module IDs*/

enum {
    kT8022ModuleID              = 7100,
    kEnetModuleID               = 7101,
    kTokenRingModuleID          = 7102,
    kFDDIModuleID               = 7103
};

/* Module Names*/


#define kEnet8022Name      "enet8022x"
#define kEnetName           "enet"
#define kFastEnetName        "fenet"
#define kTokenRingName      "tokn"
#define kFDDIName            "fddi"
#define kIRTalkName          "irtlk"
#define kSMDSName           "smds"
#define kATMName         "atm"
#define kT8022Name            "tpi8022x"
#define kATMSNAPName     "atmsnap"
#define kFireWireName     "firewire"
#define kFibreChannelName    "fibre"

/* Address Family*/

enum {
    AF_8022                     = 8200                          /* Our 802.2 generic address family*/
};

/* XTI Levels*/

enum {
    LNK_ENET                    = FOUR_CHAR_CODE('ENET'),
    LNK_TOKN                    = FOUR_CHAR_CODE('TOKN'),
    LNK_FDDI                    = FOUR_CHAR_CODE('FDDI'),
    LNK_TPI                     = FOUR_CHAR_CODE('LTPI')
};

/* Options*/

enum {
    OPT_ADDMCAST                = 0x1000,
    OPT_DELMCAST                = 0x1001,
    OPT_RCVPACKETTYPE           = 0x1002,
    OPT_RCVDESTADDR             = 0x1003,
    OPT_SETRAWMODE              = 0x1004,
    OPT_SETPROMISCUOUS          = 0x1005
};


typedef UInt32 OTPacketType;
enum {
    kETypeStandard              = 0,
    kETypeMulticast             = 1,
    kETypeBroadcast             = 2,
    kETRawPacketBit             = (unsigned long)0x80000000,
    kETTimeStampBit             = 0x40000000
};

/* Link related constants*/

enum {
    kMulticastLength            = 6,                            /* length of an ENET hardware addressaddress*/
    k48BitAddrLength            = 6,
    k8022DLSAPLength            = 2,                            /* The protocol type is our DLSAP*/
    k8022SNAPLength             = 5,
    kEnetAddressLength          = k48BitAddrLength + k8022DLSAPLength, /* length of an address field used by the ENET enpoint*/
                                                                /*    = k48BitAddrLength + sizeof(protocol type)*/
    kSNAPSAP                    = 0x00AA,                       /* Special DLSAPS for ENET*/
    kIPXSAP                     = 0x00FF,
    kMax8022SAP                 = 0x00FE,
    k8022GlobalSAP              = 0x00FF,
    kMinDIXSAP                  = 1501,
    kMaxDIXSAP                  = 0xFFFF
};

/* Generic Address Structure*/


struct T8022Address {
    OTAddressType                   fAddrFamily;
    UInt8                           fHWAddr[6];
    UInt16                          fSAP;
    UInt8                           fSNAP[5];
};
typedef struct T8022Address             T8022Address;
enum {
    k8022BasicAddressLength     = sizeof(OTAddressType) + k48BitAddrLength + sizeof(UInt16),
    k8022SNAPAddressLength      = sizeof(OTAddressType) + k48BitAddrLength + sizeof(UInt16) + k8022SNAPLength
};

/* Some helpful stuff for dealing with 48 bit addresses*/


#define OTCompare48BitAddresses(p1, p2)                                                        \
  (*(const UInt32*)((const UInt8*)(p1)) == *(const UInt32*)((const UInt8*)(p2)) &&        \
   *(const UInt16*)(((const UInt8*)(p1))+4) == *(const UInt16*)(((const UInt8*)(p2))+4) )

#define OTCopy48BitAddress(p1, p2)                                             \
  (*(UInt32*)((UInt8*)(p2)) = *(const UInt32*)((const UInt8*)(p1)),           \
   *(UInt16*)(((UInt8*)(p2))+4) = *(const UInt16*)(((const UInt8*)(p1))+4) )

#define OTClear48BitAddress(p1)                                                 \
  (*(UInt32*)((UInt8*)(p1)) = 0,                                              \
   *(UInt16*)(((UInt8*)(p1))+4) = 0 )

#define OTCompare8022SNAP(p1, p2)                                                      \
  (*(const UInt32*)((const UInt8*)(p1)) == *(const UInt32*)((const UInt8*)(p2)) &&    \
   *(((const UInt8*)(p1))+4) == *(((const UInt8*)(p2))+4) )

#define OTCopy8022SNAP(p1, p2)                                               \
  (*(UInt32*)((UInt8*)(p2)) = *(const UInt32*)((const UInt8*)(p1)),       \
   *(((UInt8*)(p2))+4) = *(((const UInt8*)(p1))+4) )

#define OTIs48BitBroadcastAddress(p1)                   \
  (*(UInt32*)((UInt8*)(p1)) == 0xffffffff &&          \
   *(UInt16*)(((UInt8*)(p1))+4) == 0xffff )

#define OTSet48BitBroadcastAddress(p1)                   \
  (*(UInt32*)((UInt8*)(p1)) = 0xffffffff,             \
   *(UInt16*)(((UInt8*)(p1))+4) = 0xffff )

#define OTIs48BitZeroAddress(p1)              \
  (*(UInt32*)((UInt8*)(p1)) == 0 &&           \
   *(UInt16*)(((UInt8*)(p1))+4) == 0 )

/* Link related constants*/

enum {
    kEnetPacketHeaderLength     = (2 * k48BitAddrLength) + k8022DLSAPLength,
    kEnetTSDU                   = 1514,                         /* The TSDU for ethernet.*/
    kTokenRingTSDU              = 4458,                         /* The TSDU for TokenRing.*/
    kFDDITSDU                   = 4458,                         /* The TSDU for FDDI. */
    k8022SAPLength              = 1,
    k8022BasicHeaderLength      = 3,                            /* define the length of the header portion of an 802.2 packet.*/
                                                                /* = SSAP+DSAP+ControlByte*/
    k8022SNAPHeaderLength       = k8022SNAPLength + k8022BasicHeaderLength
};

/*******************************************************************************
** Address Types recognized by the Enet DLPI
********************************************************************************/

typedef UInt32 EAddrType;
enum {
    keaStandardAddress          = 0,
    keaMulticast                = 1,
    keaBroadcast                = 2,
    keaBadAddress               = 3,
    keaRawPacketBit             = (unsigned long)0x80000000,
    keaTimeStampBit             = 0x40000000
};

/* Packet Header Structures*/


struct EnetPacketHeader {
    UInt8                           fDestAddr[6];
    UInt8                           fSourceAddr[6];
    UInt16                          fProto;
};
typedef struct EnetPacketHeader         EnetPacketHeader;

struct T8022Header {
    UInt8                           fDSAP;
    UInt8                           fSSAP;
    UInt8                           fCtrl;
};
typedef struct T8022Header              T8022Header;

struct T8022SNAPHeader {
    UInt8                           fDSAP;
    UInt8                           fSSAP;
    UInt8                           fCtrl;
    UInt8                           fSNAP[5];
};
typedef struct T8022SNAPHeader          T8022SNAPHeader;

struct T8022FullPacketHeader {
    EnetPacketHeader                fEnetPart;
    T8022SNAPHeader                 f8022Part;
};
typedef struct T8022FullPacketHeader    T8022FullPacketHeader;
/* Define the lengths of the structures above*/
enum {
    kT8022HeaderLength          = 3,
    kT8022SNAPHeaderLength      = 3 + k8022SNAPLength,
    kT8022FullPacketHeaderLength = kEnetPacketHeaderLength + kT8022SNAPHeaderLength
};

/* ***** Serial ******/

/* Module Definitions*/

/* XTI Level*/

enum {
    COM_SERIAL                  = FOUR_CHAR_CODE('SERL')
};

/* Version Number*/


#define kSerialABVersion   "1.1.1"

/* Module Names*/


#define kSerialABName      "serialAB"
#define kSerialName          "serial"
#define kSerialPortAName   "serialA"
#define kSerialPortBName  "serialB"

enum {
    kSerialABModuleID           = 7200
};

enum {
    kOTSerialFramingAsync       = 0x01,                         /* Support Async serial mode         */
    kOTSerialFramingHDLC        = 0x02,                         /* Support HDLC synchronous serial mode   */
    kOTSerialFramingSDLC        = 0x04,                         /* Support SDLC synchronous serial mode   */
    kOTSerialFramingAsyncPackets = 0x08                         /* Support Async "packet" serial mode */
};

/* IOCTL Calls for Serial Drivers*/

enum {
    I_SetSerialDTR              = ((MIOC_SRL << 8) | 0),        /* Set DTR (0 = off, 1 = on)*/
    kOTSerialSetDTROff          = 0,
    kOTSerialSetDTROn           = 1,
    I_SetSerialBreak            = ((MIOC_SRL << 8) | 1),        /* Send a break on the line - kOTSerialSetBreakOff = off, kOTSerialSetBreakOn = on,*/
                                                                /* Any other number is the number of milliseconds to leave break on, then*/
                                                                /* auto shutoff*/
    kOTSerialSetBreakOn         = (unsigned long)0xFFFFFFFF,
    kOTSerialSetBreakOff        = 0,
    I_SetSerialXOffState        = ((MIOC_SRL << 8) | 2),        /* Force XOFF state - 0 = Unconditionally clear XOFF state, 1 = unconditionally set it*/
    kOTSerialForceXOffTrue      = 1,
    kOTSerialForceXOffFalse     = 0,
    I_SetSerialXOn              = ((MIOC_SRL << 8) | 3),        /* Send an XON character, 0 = send only if in XOFF state, 1 = send always*/
    kOTSerialSendXOnAlways      = 1,
    kOTSerialSendXOnIfXOffTrue  = 0,
    I_SetSerialXOff             = ((MIOC_SRL << 8) | 4),        /* Send an XOFF character, 0 = send only if in XON state, 1 = send always*/
    kOTSerialSendXOffAlways     = 1,
    kOTSerialSendXOffIfXOnTrue  = 0
};

/* Option Management for Serial Drivers*/

/*
   These options are all 4-byte values.
   BaudRate is the baud rate.
   DataBits is the number of data bits.
   StopBits is the number of stop bits times 10.
   Parity is an enum
*/

enum {
    SERIAL_OPT_BAUDRATE         = 0x0100,                       /* UInt32 */
    SERIAL_OPT_DATABITS         = 0x0101,                       /* UInt32 */
    SERIAL_OPT_STOPBITS         = 0x0102,                       /* UInt32 10, 15 or 20 for 1, 1.5 or 2    */
    SERIAL_OPT_PARITY           = 0x0103,                       /* UInt32 */
    SERIAL_OPT_STATUS           = 0x0104,                       /* UInt32 */
                                                                /* The "Status" option is a 4-byte value option that is ReadOnly*/
                                                                /* It returns a bitmap of the current serial status*/
    SERIAL_OPT_HANDSHAKE        = 0x0105,                       /* UInt32 */
                                                                /* The "Handshake" option defines what kind of handshaking the serial port*/
                                                                /* will do for line flow control.  The value is a 32-bit value defined by*/
                                                                /* the function or macro SerialHandshakeData below.*/
                                                                /* For no handshake, or CTS handshake, the onChar and offChar parameters*/
                                                                /* are ignored.*/
    SERIAL_OPT_RCVTIMEOUT       = 0x0106,                       /* The "RcvTimeout" option defines how long the receiver should wait before delivering*/
                                                                /* less than the RcvLoWat number of characters.  If RcvLoWat is 0, then the RcvTimeout*/
                                                                /* is how long a gap to wait for before delivering characters.  This parameter is advisory,*/
                                                                /* and serial drivers are free to deliver data whenever they deem it convenient.  For instance,*/
                                                                /* many serial drivers will deliver data whenever 64 bytes have been received, since 64 bytes*/
                                                                /* is the smallest STREAMS buffer size. Keep in mind that timeouts are quantized, so be sure to*/
                                                                /* look at the return value of the option to determine what it was negotiated to.*/
    SERIAL_OPT_ERRORCHARACTER   = 0x0107,                       /* This option defines how characters with parity errors are handled.*/
                                                                /* A 0 value will disable all replacement.  A single character value in the low*/
                                                                /* byte designates the replacement character.  When characters are received with a */
                                                                /* parity error, they are replaced by this specified character.  If a valid incoming*/
                                                                /* character matches the replacement character, then the received character's msb is*/
                                                                /* cleared. For this situation, the alternate character is used, if specified in bits*/
                                                                /* 8 through 15 of the option long, with 0xff being place in bits 16 through 23.*/
                                                                /* Whenever a valid character is received that matches the first replacement character,*/
                                                                /* it is replaced with this alternate character.*/
    SERIAL_OPT_EXTCLOCK         = 0x0108,                       /* The "ExtClock" requests an external clock.  A 0-value turns off external clocking.*/
                                                                /* Any other value is a requested divisor for the external clock.  Be aware that*/
                                                                /* not all serial implementations support an external clock, and that not all*/
                                                                /* requested divisors will be supported if it does support an external clock.*/
    SERIAL_OPT_BURSTMODE        = 0x0109,                       /* The "BurstMode" option informs the serial driver that it should continue looping,*/
                                                                /* reading incoming characters, rather than waiting for an interrupt for each character.*/
                                                                /* This option may not be supported by all Serial driver*/
    SERIAL_OPT_DUMMY            = 0x010A                        /* placeholder*/
};


typedef UInt32 ParityOptionValues;
enum {
    kOTSerialNoParity           = 0,
    kOTSerialOddParity          = 1,
    kOTSerialEvenParity         = 2
};

enum {
    kOTSerialSwOverRunErr       = 0x01,
    kOTSerialBreakOn            = 0x08,
    kOTSerialParityErr          = 0x10,
    kOTSerialOverrunErr         = 0x20,
    kOTSerialFramingErr         = 0x40,
    kOTSerialXOffSent           = 0x00010000,
    kOTSerialDTRNegated         = 0x00020000,
    kOTSerialCTLHold            = 0x00040000,
    kOTSerialXOffHold           = 0x00080000,
    kOTSerialOutputBreakOn      = 0x01000000
};

enum {
    kOTSerialXOnOffInputHandshake = 1,                          /* Want XOn/XOff handshake for incoming characters    */
    kOTSerialXOnOffOutputHandshake = 2,                         /* Want XOn/XOff handshake for outgoing characters    */
    kOTSerialCTSInputHandshake  = 4,                            /* Want CTS handshake for incoming characters     */
    kOTSerialDTROutputHandshake = 8                             /* Want DTR handshake for outoing characters   */
};


#ifdef __cplusplus

   inline UInt32 OTSerialHandshakeData(UInt16 type, UInt8 onChar, UInt8 offChar)
  {
      return (((UInt32)type) << 16) | (((UInt32)onChar) << 8) | offChar;
 }

#else

  #define OTSerialHandshakeData(type, onChar, offChar)    \
      ((((UInt32)type) << 16) | (((UInt32)onChar) << 8) | offChar)
   
#endif


#ifdef __cplusplus

inline UInt32 OTSerialSetErrorCharacter(UInt8 rep)
{
  return (UInt32)rep & 0x000000ff;
}

inline UInt32 OTSerialSetErrorCharacterWithAlternate(UInt8 rep, UInt8 alternate)
{
 return (((rep & 0xff) | ((alternate & 0xff) << 8)) | 0x80000000L);
}

#else

#define OTSerialSetErrorCharacter(rep) \
  ((rep) & 0xff)

#define OTSerialSetErrorCharacterWithAlternate(rep, alternate)  \
  ((((rep) & 0xff) | (((alternate) & 0xff) << 8)) | 0x80000000L)

#endif

/* Default attributes for the serial ports*/

enum {
    kOTSerialDefaultBaudRate    = 19200,
    kOTSerialDefaultDataBits    = 8,
    kOTSerialDefaultStopBits    = 10,
    kOTSerialDefaultParity      = kOTSerialNoParity,
    kOTSerialDefaultHandshake   = 0,
    kOTSerialDefaultOnChar      = ('Q' & 0xFFFFFFBF),
    kOTSerialDefaultOffChar     = ('S' & 0xFFFFFFBF),
    kOTSerialDefaultSndBufSize  = 1024,
    kOTSerialDefaultRcvBufSize  = 1024,
    kOTSerialDefaultSndLoWat    = 96,
    kOTSerialDefaultRcvLoWat    = 1,
    kOTSerialDefaultRcvTimeout  = 10
};

/* ***** ISDN ******/

/* Module Definitions*/

/* XTI Level*/

enum {
    COM_ISDN                    = FOUR_CHAR_CODE('ISDN')
};

/* Module Names*/

#define kISDNName       "isdn"
enum {
    kISDNModuleID               = 7300
};


/* ISDN framing methods, set using the I_OTSetFramingType IOCTL*/

enum {
    kOTISDNFramingTransparentSupported = 0x0010,                /* Support Transparent mode    */
    kOTISDNFramingHDLCSupported = 0x0020,                       /* Support HDLC Synchronous mode  */
    kOTISDNFramingV110Supported = 0x0040,                       /* Support V.110 Asynchronous mode    */
    kOTISDNFramingV14ESupported = 0x0080                        /* Support V.14 Asynchronous mode     */
};

/* Miscellaneous equates*/

/* Disconnect reason codes (from Q.931)*/

enum {
    kOTISDNUnallocatedNumber    = 1,
    kOTISDNNoRouteToSpecifiedTransitNetwork = 2,
    kOTISDNNoRouteToDestination = 3,
    kOTISDNChannelUnacceptable  = 6,
    kOTISDNNormal               = 16,
    kOTISDNUserBusy             = 17,
    kOTISDNNoUserResponding     = 18,
    kOTISDNNoAnswerFromUser     = 19,
    kOTISDNCallRejected         = 21,
    kOTISDNNumberChanged        = 22,
    kOTISDNNonSelectedUserClearing = 26,
    kOTISDNDestinationOutOfOrder = 27,
    kOTISDNInvalidNumberFormat  = 28,
    kOTISDNFacilityRejected     = 29,
    kOTISDNNormalUnspecified    = 31,
    kOTISDNNoCircuitChannelAvailable = 34,
    kOTISDNNetworkOutOfOrder    = 41,
    kOTISDNSwitchingEquipmentCongestion = 42,
    kOTISDNAccessInformationDiscarded = 43,
    kOTISDNRequestedCircuitChannelNotAvailable = 44,
    kOTISDNResourceUnavailableUnspecified = 45,
    kOTISDNQualityOfServiceUnvailable = 49,
    kOTISDNRequestedFacilityNotSubscribed = 50,
    kOTISDNBearerCapabilityNotAuthorized = 57,
    kOTISDNBearerCapabilityNotPresentlyAvailable = 58,
    kOTISDNCallRestricted       = 59,
    kOTISDNServiceOrOptionNotAvilableUnspecified = 63,
    kOTISDNBearerCapabilityNotImplemented = 65,
    kOTISDNRequestedFacilityNotImplemented = 69,
    kOTISDNOnlyRestrictedDigitalBearer = 70,
    kOTISDNServiceOrOptionNotImplementedUnspecified = 79,
    kOTISDNCallIdentityNotUsed  = 83,
    kOTISDNCallIdentityInUse    = 84,
    kOTISDNNoCallSuspended      = 85,
    kOTISDNCallIdentityCleared  = 86,
    kOTISDNIncompatibleDestination = 88,
    kOTISDNInvalidTransitNetworkSelection = 91,
    kOTISDNInvalidMessageUnspecified = 95,
    kOTISDNMandatoryInformationElementIsMissing = 96,
    kOTISDNMessageTypeNonExistentOrNotImplemented = 97,
    kOTISDNInterworkingUnspecified = 127
};

/* OTISDNAddress*/

/*
   The OTISDNAddress has the following format:
   "xxxxxx*yy"
   where 'x' is the phone number and 'y' is the sub address (if available
   in the network. The characters are coded in ASCII (IA5), and valid
   characters are: '0'-'9','#','*'.
   The max length of the each phone number is 21 characters (?) and the max
   subaddress length is network dependent.
   When using bonded channels the phone numbers are separated by '&'.
   The X.25 user data is preceded by '@'.
*/

#ifndef AF_ISDN /* might conflict with BSD headers */ 
enum {
    AF_ISDN                     = 0x2000
};

#endif
enum {
    kOTISDNMaxPhoneSize         = 32,
    kOTISDNMaxSubSize           = 4
};


struct OTISDNAddress {
    OTAddressType                   fAddressType;
    UInt16                          fPhoneLength;
    char                            fPhoneNumber[37];
};
typedef struct OTISDNAddress            OTISDNAddress;
/* IOCTL Calls for ISDN*/
/* ISDN shares the same ioctl space as serial.*/

enum {
    MIOC_ISDN                   = 'U'                           /* ISDN ioctl() cmds */
};

enum {
    I_OTISDNAlerting            = ((MIOC_ISDN << 8) | 100),     /* Send or receive an ALERTING message*/
    I_OTISDNSuspend             = ((MIOC_ISDN << 8) | 101),     /* Send a SUSPEND message*/
                                                                /* The parameter is the Call Identity (Maximum 8 octets)*/
    I_OTISDNSuspendAcknowledge  = ((MIOC_ISDN << 8) | 102),     /* Receive a SUSPEND ACKNOWLEDGE message*/
    I_OTISDNSuspendReject       = ((MIOC_ISDN << 8) | 103),     /* Receive a SUSPEND REJECT message*/
    I_OTISDNResume              = ((MIOC_ISDN << 8) | 104),     /* Send a RESUME message*/
                                                                /* The parameter is the Call Identity (Maximum 8 octets)*/
    I_OTISDNResumeAcknowledge   = ((MIOC_ISDN << 8) | 105),     /* Receive a RESUME ACKNOWLEDGE message*/
    I_OTISDNResumeReject        = ((MIOC_ISDN << 8) | 106),     /* Receive a RESUME REJECT message*/
    I_OTISDNFaciltity           = ((MIOC_ISDN << 8) | 107)      /* Send or receive a FACILITY message*/
};

/* Connect user data size*/

enum {
    kOTISDNMaxUserDataSize      = 32
};

/* Option management calls for ISDN*/

enum {
    ISDN_OPT_COMMTYPE           = 0x0200,
    ISDN_OPT_FRAMINGTYPE        = 0x0201,
    ISDN_OPT_56KADAPTATION      = 0x0202
};

/* For ISDN_OPT_COMMTYPE...*/

enum {
    kOTISDNTelephoneALaw        = 1,                            /* G.711 A-law                */
    kOTISDNTelephoneMuLaw       = 26,                           /* G.711 .-law                */
    kOTISDNDigital64k           = 13,                           /* unrestricted digital (default)     */
    kOTISDNDigital56k           = 37,                           /* user rate 56Kb/s           */
    kOTISDNVideo64k             = 41,                           /* video terminal at 64Kb/s    */
    kOTISDNVideo56k             = 42                            /* video terminal at 56Kb/s    */
};

/* For ISDN_OPT_FRAMINGTYPE...*/

enum {
    kOTISDNFramingTransparent   = 0x0010,                       /* Transparent mode           */
    kOTISDNFramingHDLC          = 0x0020,                       /* HDLC synchronous mode (default)    */
    kOTISDNFramingV110          = 0x0040,                       /* V.110 asynchronous mode       */
    kOTISDNFramingV14E          = 0x0080                        /* V.14E asynchronous mode         */
};

/* For ISDN_OPT_56KADAPTATION...*/

enum {
    kOTISDNNot56KAdaptation     = false,                        /* not 56K Adaptation (default)     */
    kOTISDN56KAdaptation        = true                          /* 56K Adaptation           */
};

/* Default options, you do not need to set these*/

enum {
    kOTISDNDefaultCommType      = kOTISDNDigital64k,
    kOTISDNDefaultFramingType   = kOTISDNFramingHDLC,
    kOTISDNDefault56KAdaptation = kOTISDNNot56KAdaptation
};


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

#endif /* __OPENTRANSPORTPROVIDERS__ */

