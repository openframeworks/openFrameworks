/*
     File:       Serial.h
 
     Contains:   Asynchronous Serial Driver (.AIn/.AOut/.BIn/.BOut) Interfaces
 
     Version:    Technology: System 7.6+
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __SERIAL__
#define __SERIAL__

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
    #pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

enum {
    baud150                     = 763,
    baud300                     = 380,
    baud600                     = 189,
    baud1200                    = 94,
    baud1800                    = 62,
    baud2400                    = 46,
    baud3600                    = 30,
    baud4800                    = 22,
    baud7200                    = 14,
    baud9600                    = 10,
    baud14400                   = 6,
    baud19200                   = 4,
    baud28800                   = 2,
    baud38400                   = 1,
    baud57600                   = 0
};

enum {
    stop10                      = 16384,
    stop15                      = -32768L,
    stop20                      = -16384
};

enum {
    noParity                    = 0,
    oddParity                   = 4096,
    evenParity                  = 12288
};

enum {
    data5                       = 0,
    data6                       = 2048,
    data7                       = 1024,
    data8                       = 3072
};

enum {
    aData                       = 6,                            /* channel A data in or out (historical) */
    aCtl                        = 2,                            /* channel A control (historical) */
    bData                       = 4,                            /* channel B data in or out (historical) */
    bCtl                        = 0                             /* channel B control (historical) */
};

enum {
    dsrEvent                    = 2,                            /* flag for SerShk.evts */
    riEvent                     = 4,                            /* flag for SerShk.evts */
    dcdEvent                    = 8,                            /* flag for SerShk.evts */
    ctsEvent                    = 32,                           /* flag for SerShk.evts */
    breakEvent                  = 128                           /* flag for SerShk.evts */
};

enum {
    xOffWasSent                 = 128,                          /* flag for SerStaRec.xOffSent */
    dtrNegated                  = 64,                           /* flag for SerStaRec.xOffSent */
    rtsNegated                  = 32                            /* flag for SerStaRec.xOffSent */
};

enum {
    ainRefNum                   = -6,                           /* serial port A input */
    aoutRefNum                  = -7,                           /* serial port A output */
    binRefNum                   = -8,                           /* serial port B input */
    boutRefNum                  = -9                            /* serial port B output */
};

enum {
    swOverrunErr                = 1,                            /* serial driver error masks */
    breakErr                    = 8,                            /* serial driver error masks */
    parityErr                   = 16,                           /* serial driver error masks */
    hwOverrunErr                = 32,                           /* serial driver error masks */
    framingErr                  = 64                            /* serial driver error masks */
};

enum {
    kOptionPreserveDTR          = 128,                          /* option bit used with Control code 16 */
    kOptionClockX1CTS           = 64                            /* option bit used with Control code 16 */
};

enum {
    kUseCTSOutputFlowControl    = 128,                          /* flag for SerShk.fCTS */
    kUseDSROutputFlowControl    = 64,                           /* flag for SerShk.fCTS */
    kUseRTSInputFlowControl     = 128,                          /* flag for SerShk.fDTR */
    kUseDTRInputFlowControl     = 64                            /* flag for SerShk.fDTR */
};

enum {
    sPortA                      = 0,                            /* Macintosh modem port */
    sPortB                      = 1,                            /* Macintosh printer port */
    sCOM1                       = 2,                            /* RS-232 port COM1 */
    sCOM2                       = 3                             /* RS-232 port COM2 */
};

typedef SInt8                           SPortSel;
/* csCodes for serial driver Control routines */
enum {
    kSERDConfiguration          = 8,                            /* program port speed, bits/char, parity, and stop bits */
    kSERDInputBuffer            = 9,                            /* set buffer for chars received with no read pending */
    kSERDSerHShake              = 10,                           /* equivalent to SerHShake, largely obsolete */
    kSERDClearBreak             = 11,                           /* assert break signal on output */
    kSERDSetBreak               = 12,                           /* negate break state on output */
    kSERDBaudRate               = 13,                           /* set explicit baud rate, other settings unchanged */
    kSERDHandshake              = 14,                           /* superset of 10, honors setting of fDTR */
    kSERDClockMIDI              = 15,                           /* clock externally on CTS with specified multiplier */
    kSERDMiscOptions            = 16,                           /* select clock source and DTR behavior on close */
    kSERDAssertDTR              = 17,                           /* assert DTR output */
    kSERDNegateDTR              = 18,                           /* negate DTR output */
    kSERDSetPEChar              = 19,                           /* select char to replace chars with invalid parity */
    kSERDSetPEAltChar           = 20,                           /* select char to replace char that replaces chars with invalid parity */
    kSERDSetXOffFlag            = 21,                           /* set XOff output flow control (same as receiving XOff) */
    kSERDClearXOffFlag          = 22,                           /* clear XOff output flow control (same as receiving XOn) */
    kSERDSendXOn                = 23,                           /* send XOn if input flow control state is XOff */
    kSERDSendXOnOut             = 24,                           /* send XOn regardless of input flow control state */
    kSERDSendXOff               = 25,                           /* send XOff if input flow control state is XOn */
    kSERDSendXOffOut            = 26,                           /* send XOff regardless of input flow control state */
    kSERDResetChannel           = 27,                           /* reset serial I/O channel hardware */
    kSERDHandshakeRS232         = 28,                           /* extension of 14, allows full RS-232 hardware handshaking */
    kSERDStickParity            = 29,                           /* use mark/space parity */
    kSERDAssertRTS              = 30,                           /* assert RTS output */
    kSERDNegateRTS              = 31,                           /* negate RTS output */
    kSERD115KBaud               = 115,                          /* set 115.2K baud data rate */
    kSERD230KBaud               = 230                           /* set 230.4K baud data rate */
};


/* csCodes for serial driver Status routines */
enum {
    kSERDInputCount             = 2,                            /* return characters available (SerGetBuf) */
    kSERDStatus                 = 8,                            /* return characters available (SerStatus) */
    kSERDVersion                = 9,                            /* return version number in first byte of csParam */
    kSERDGetDCD                 = 256                           /* get instantaneous state of DCD (GPi) */
};



struct SerShk {
    Byte                            fXOn;                       /* XOn/XOff output flow control flag */
    Byte                            fCTS;                       /* hardware output flow control flags */
    unsigned char                   xOn;                        /* XOn character */
    unsigned char                   xOff;                       /* XOff character */
    Byte                            errs;                       /* errors mask bits */
    Byte                            evts;                       /* event enable mask bits */
    Byte                            fInX;                       /* XOn/XOff input flow control flag */
    Byte                            fDTR;                       /* hardware input flow control flags */
};
typedef struct SerShk                   SerShk;

struct SerStaRec {
    Byte                            cumErrs;                    /* errors accumulated since last SerStatus() call */
    Byte                            xOffSent;                   /* input (requested to be) held off by xOffWasSent or dtrNegated or rtsNegated */
    Byte                            rdPend;                     /* incomplete read pending in I/O queue */
    Byte                            wrPend;                     /* incomplete write pending in I/O queue */
    Byte                            ctsHold;                    /* transmit disabled by hardware handshaking */
    Byte                            xOffHold;                   /* transmit disabled by XOn/XOff handshaking */
    Byte                            dsrHold;                    /* transmit disabled: external device not ready */
    Byte                            modemStatus;                /* reports modem status according to SerShk.evts */
};
typedef struct SerStaRec                SerStaRec;
#if OLDROUTINENAMES
/* ********************************************************************************************* */
/* The following constant names have been retired in favor of standard, more descriptive names.  */
/* You can still compile old code by defining OLDROUTINENAMES. There were several constants that */
/* were formerly available that have been removed, as they are now regarded as either private or */
/* unsupported. We advise that you stop using any constants that are not defined in this file.   */
/* ********************************************************************************************* */
enum {
    serdOptionClockExternal     = kOptionClockX1CTS,            /* option bit used with Control code 16 */
    serdOptionPreserveDTR       = kOptionPreserveDTR            /* option bit used with Control code 16 */
};

/* csCodes for serial driver Control routines */
enum {
    serdReset                   = kSERDConfiguration,
    serdSetBuf                  = kSERDInputBuffer,
    serdHShake                  = kSERDSerHShake,
    serdClrBrk                  = kSERDClearBreak,
    serdSetBrk                  = kSERDSetBreak,
    serdSetBaud                 = kSERDBaudRate,
    serdHShakeDTR               = kSERDHandshake,
    serdSetMIDI                 = kSERDClockMIDI,
    serdSetMisc                 = kSERDMiscOptions,
    serdSetDTR                  = kSERDAssertDTR,
    serdClrDTR                  = kSERDNegateDTR,
    serdSetPEChar               = kSERDSetPEChar,
    serdSetPECharAlternate      = kSERDSetPEAltChar,
    serdSetXOff                 = kSERDSetXOffFlag,
    serdClrXOff                 = kSERDClearXOffFlag,
    serdSendXOnConditional      = kSERDSendXOn,
    serdSendXOn                 = kSERDSendXOnOut,
    serdSendXOffConditional     = kSERDSendXOff,
    serdSendXOff                = kSERDSendXOffOut,
    serdChannelReset            = kSERDResetChannel,
    serdSet230KBaud             = kSERD230KBaud                 /* set 230K baud data rate */
};


/* csCodes for serial driver Status routines */
enum {
    serdGetBuf                  = kSERDInputCount,
    serdStatus                  = kSERDStatus,
    serdGetVers                 = kSERDVersion
};

#endif  /* OLDROUTINENAMES */

/*
    The following interfaces are for the legacy high-level serial driver glue in
    the interface libraries of your development system. They merely substitue for
    the corresponding synchronous calls to PBControl and PBStatus.

    They have not been updated as the serial driver API has evolved. Take note:

        SerHShake does not support hardware input flow control. Use csCode 14.
        SerStatus returns only the first six bytes of SerStaRec (through xOffHold).
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
SerReset                        (short                  refNum,
                                 short                  serConfig);

EXTERN_API( OSErr )
SerSetBuf                       (short                  refNum,
                                 Ptr                    serBPtr,
                                 short                  serBLen);

EXTERN_API( OSErr )
SerHShake                       (short                  refNum,
                                 const SerShk *         flags);

EXTERN_API( OSErr )
SerSetBrk                       (short                  refNum);

EXTERN_API( OSErr )
SerClrBrk                       (short                  refNum);

EXTERN_API( OSErr )
SerGetBuf                       (short                  refNum,
                                 long *                 count);

EXTERN_API( OSErr )
SerStatus                       (short                  refNum,
                                 SerStaRec *            serSta);


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

#endif /* __SERIAL__ */

