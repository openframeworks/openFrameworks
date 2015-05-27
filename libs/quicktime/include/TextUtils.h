/*
     File:       TextUtils.h
 
     Contains:   Text Utilities Interfaces.
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TEXTUTILS__
#define __TEXTUTILS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __NUMBERFORMATTING__
#include <NumberFormatting.h>
#endif

#ifndef __STRINGCOMPARE__
#include <StringCompare.h>
#endif

#ifndef __DATETIMEUTILS__
#include <DateTimeUtils.h>
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

/*

    Here are the current System 7 routine names and the translations to the older forms.
    Please use the newer forms in all new code and migrate the older names out of existing
    code as maintainance permits.
    
    NEW NAME                    OLD NAMEs                   OBSOLETE FORM (no script code)

    FindScriptRun
    FindWordBreaks                                          NFindWord, FindWord
    GetIndString            
    GetString
    Munger
    NewString               
    SetString               
    StyledLineBreak
    TruncString
    TruncText

    UpperString ($A054)         UprString, UprText
    UppercaseText               SCUpperText (a only)        UpperText ($A456)
    LowercaseText                                           LwrString, LowerText, LwrText ($A056)
    StripDiacritics                                         StripText ($A256)
    UppercaseStripDiacritics                                StripUpperText ($A656)


*/

/* Type for truncWhere parameter in TruncString, TruncText */
typedef short                           TruncCode;
enum {
                                                                /* Constants for truncWhere argument in TruncString and TruncText */
    truncEnd                    = 0,                            /* Truncate at end */
    truncMiddle                 = 0x4000,                       /* Truncate in middle */
    smTruncEnd                  = 0,                            /* Truncate at end - obsolete */
    smTruncMiddle               = 0x4000                        /* Truncate in middle - obsolete */
};

enum {
                                                                /* Constants for TruncString and TruncText results */
    notTruncated                = 0,                            /* No truncation was necessary */
    truncated                   = 1,                            /* Truncation performed */
    truncErr                    = -1,                           /* General error */
    smNotTruncated              = 0,                            /* No truncation was necessary - obsolete */
    smTruncated                 = 1,                            /* Truncation performed   - obsolete */
    smTruncErr                  = -1                            /* General error - obsolete */
};

typedef SInt8                           StyledLineBreakCode;
enum {
    smBreakWord                 = 0,
    smBreakChar                 = 1,
    smBreakOverflow             = 2
};


struct ScriptRunStatus {
    SInt8                           script;
    SInt8                           runVariant;
};
typedef struct ScriptRunStatus          ScriptRunStatus;

struct BreakTable {
    char                            charTypes[256];
    short                           tripleLength;
    short                           triples[1];
};
typedef struct BreakTable               BreakTable;
typedef BreakTable *                    BreakTablePtr;

struct NBreakTable {
    SInt8                           flags1;
    SInt8                           flags2;
    short                           version;
    short                           classTableOff;
    short                           auxCTableOff;
    short                           backwdTableOff;
    short                           forwdTableOff;
    short                           doBackup;
    short                           length;                     /* length of NBreakTable */
    char                            charTypes[256];
    short                           tables[1];
};
typedef struct NBreakTable              NBreakTable;
typedef NBreakTable *                   NBreakTablePtr;
/* The following functions are new names that work on 68k and PowerPC*/
EXTERN_API( long )
Munger                          (Handle                 h,
                                 long                   offset,
                                 const void *           ptr1,
                                 long                   len1,
                                 const void *           ptr2,
                                 long                   len2)                               ONEWORDINLINE(0xA9E0);

EXTERN_API( StringHandle )
NewString                       (ConstStr255Param       theString)                          ONEWORDINLINE(0xA906);

EXTERN_API( void )
SetString                       (StringHandle           theString,
                                 ConstStr255Param       strNew)                             ONEWORDINLINE(0xA907);

EXTERN_API( StringHandle )
GetString                       (short                  stringID)                           ONEWORDINLINE(0xA9BA);

EXTERN_API( void )
GetIndString                    (Str255                 theString,
                                 short                  strListID,
                                 short                  index);

#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
setstring                       (StringHandle           theString,
                                 const char *           strNew);

EXTERN_API_C( StringHandle )
newstring                       (const char *           theString);

EXTERN_API_C( void )
getindstring                    (char *                 theString,
                                 short                  strListID,
                                 short                  index);

#endif  /* CALL_NOT_IN_CARBON */

EXTERN_API( StyledLineBreakCode )
StyledLineBreak                 (Ptr                    textPtr,
                                 long                   textLen,
                                 long                   textStart,
                                 long                   textEnd,
                                 long                   flags,
                                 Fixed *                textWidth,
                                 long *                 textOffset)                         FOURWORDINLINE(0x2F3C, 0x821C, 0xFFFE, 0xA8B5);

EXTERN_API( short )
TruncString                     (short                  width,
                                 Str255                 theString,
                                 TruncCode              truncWhere)                         FOURWORDINLINE(0x2F3C, 0x8208, 0xFFE0, 0xA8B5);

EXTERN_API( short )
TruncText                       (short                  width,
                                 Ptr                    textPtr,
                                 short *                length,
                                 TruncCode              truncWhere)                         FOURWORDINLINE(0x2F3C, 0x820C, 0xFFDE, 0xA8B5);

EXTERN_API( void )
FindWordBreaks                  (Ptr                    textPtr,
                                 short                  textLength,
                                 short                  offset,
                                 Boolean                leadingEdge,
                                 BreakTablePtr          breaks,
                                 OffsetTable            offsets,
                                 ScriptCode             script)                             FOURWORDINLINE(0x2F3C, 0xC012, 0x001A, 0xA8B5);

EXTERN_API( void )
LowercaseText                   (Ptr                    textPtr,
                                 short                  len,
                                 ScriptCode             script)                             SIXWORDINLINE(0x3F3C, 0x0000, 0x2F3C, 0x800A, 0xFFB6, 0xA8B5);

EXTERN_API( void )
UppercaseText                   (Ptr                    textPtr,
                                 short                  len,
                                 ScriptCode             script)                             SIXWORDINLINE(0x3F3C, 0x0400, 0x2F3C, 0x800A, 0xFFB6, 0xA8B5);

EXTERN_API( void )
StripDiacritics                 (Ptr                    textPtr,
                                 short                  len,
                                 ScriptCode             script)                             SIXWORDINLINE(0x3F3C, 0x0200, 0x2F3C, 0x800A, 0xFFB6, 0xA8B5);

EXTERN_API( void )
UppercaseStripDiacritics        (Ptr                    textPtr,
                                 short                  len,
                                 ScriptCode             script)                             SIXWORDINLINE(0x3F3C, 0x0600, 0x2F3C, 0x800A, 0xFFB6, 0xA8B5);

EXTERN_API( ScriptRunStatus )
FindScriptRun                   (Ptr                    textPtr,
                                 long                   textLen,
                                 long *                 lenUsed)                            FOURWORDINLINE(0x2F3C, 0x820C, 0x0026, 0xA8B5);

#if CALL_NOT_IN_CARBON
/*
    The following functions are old names, but are required for PowerPC builds
    because InterfaceLib exports these names, instead of the new ones.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
FindWord                        (Ptr                    textPtr,
                                 short                  textLength,
                                 short                  offset,
                                 Boolean                leadingEdge,
                                 BreakTablePtr          breaks,
                                 OffsetTable            offsets)                            FOURWORDINLINE(0x2F3C, 0x8012, 0x001A, 0xA8B5);

EXTERN_API( void )
NFindWord                       (Ptr                    textPtr,
                                 short                  textLength,
                                 short                  offset,
                                 Boolean                leadingEdge,
                                 NBreakTablePtr         nbreaks,
                                 OffsetTable            offsets)                            FOURWORDINLINE(0x2F3C, 0x8012, 0xFFE2, 0xA8B5);

/*
   On 68K machines, LwrText, LowerText, StripText, UpperText and StripUpperText
   return an error code in register D0, but System 7 PowerMacs do not emulate
   this properly, so checking D0 is unreliable.
*/

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter LwrText(__A0, __D0)
                                                                                            #endif
EXTERN_API( void )
LwrText                         (Ptr                    textPtr,
                                 short                  len)                                ONEWORDINLINE(0xA056);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter LowerText(__A0, __D0)
                                                                                            #endif
EXTERN_API( void )
LowerText                       (Ptr                    textPtr,
                                 short                  len)                                ONEWORDINLINE(0xA056);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter StripText(__A0, __D0)
                                                                                            #endif
EXTERN_API( void )
StripText                       (Ptr                    textPtr,
                                 short                  len)                                ONEWORDINLINE(0xA256);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter UpperText(__A0, __D0)
                                                                                            #endif
EXTERN_API( void )
UpperText                       (Ptr                    textPtr,
                                 short                  len)                                ONEWORDINLINE(0xA456);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter StripUpperText(__A0, __D0)
                                                                                            #endif
EXTERN_API( void )
StripUpperText                  (Ptr                    textPtr,
                                 short                  len)                                ONEWORDINLINE(0xA656);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

/* The following are new names which are exported by InterfaceLib*/
EXTERN_API( void )
UpperString                     (Str255                 theString,
                                 Boolean                diacSensitive);

EXTERN_API_C( void )
upperstring                     (char *                 theString,
                                 Boolean                diacSensitive);

/* The following are macros which map old names to the names exported by InterfaceLib*/
#if OLDROUTINENAMES
#define UprString(theString, diacSensitive)  \
         UpperString(theString, diacSensitive)
#endif  /* OLDROUTINENAMES */

/* Old routine name but no new names are mapped to it:*/
#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter UprText(__A0, __D0)
                                                                                            #endif
EXTERN_API( void )
UprText                         (Ptr                    textPtr,
                                 short                  len)                                ONEWORDINLINE(0xA054);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

/*
    Functions for converting between C and Pascal Strings
    (Previously in Strings.h)
    
    Note: CopyPascalStringToC, CopyCStringToPascal, c2pstrcpy, and p2cstrcpy
          are written to allow inplace conversion.  That is, the src and dst
          parameters can point to the memory location.  
          
    Note: c2pstr, C2PStr, p2cstr, and P2CStr are all deprecated.  These functions
          only do inplace conversion and often require casts to call them.  This can
          cause bugs because you can easily cast away a const and change the 
          contents of a read-only buffer.  Do you know what c2pstr("Hello") will do?
    
*/
EXTERN_API_C( void )
c2pstrcpy                       (Str255                 dst,
                                 const char *           src);

EXTERN_API_C( void )
p2cstrcpy                       (char *                 dst,
                                 ConstStr255Param       src);

EXTERN_API_C( void )
CopyPascalStringToC             (ConstStr255Param       src,
                                 char *                 dst);

EXTERN_API_C( void )
CopyCStringToPascal             (const char *           src,
                                 Str255                 dst);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API_C( StringPtr )
c2pstr                          (char *                 aStr);

EXTERN_API( StringPtr )
C2PStr                          (Ptr                    cString);

EXTERN_API_C( char *)
p2cstr                          (StringPtr              aStr);

EXTERN_API( Ptr )
P2CStr                          (StringPtr              pString);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if !TARGET_OS_MAC
    /* Added for QuickTime 3.0 */
    #define C2PStr(a)  (StringPtr)c2pstr((Ptr)(a))
    #define P2CStr(a)  (Ptr)p2cstr(a)

    #define CopyPascalStringToC(src,dst) p2cstrcpy(dst,src)
    #define CopyCStringToPascal(src,dst) c2pstrcpy(dst,src)
#endif
/*
   Added macros to convert p2cstr and c2pstr calls into p2cstrcpy and c2pstrcpy calls,
   and map C2PStr and P2CStr to c2pstr and p2cstr for Carbon.
   The use of the often unappreciated comma operator to coerce the proper return values,
   since the p2cstrcpy and c2pstrcpy routines have no return value.
*/

#if TARGET_OS_MAC && TARGET_API_MAC_CARBON && OLDP2C
    #define p2cstr(aStr) (p2cstrcpy((char *) aStr, aStr) , (char *) aStr)
    #define c2pstr(aStr) (c2pstrcpy(aStr, (char *) aStr) , (StringPtr) aStr)

    #define C2PStr(a)  (StringPtr)c2pstr((Ptr)(a))
    #define P2CStr(a)  (Ptr)p2cstr(a)
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

#endif /* __TEXTUTILS__ */

