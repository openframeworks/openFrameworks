/*
     File:       AEPackObject.h
 
     Contains:   AppleEvents object packing Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1991-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __AEPACKOBJECT__
#define __AEPACKOBJECT__

#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
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

/* These are the object packing routines.  */
EXTERN_API( OSErr )
CreateOffsetDescriptor          (long                   theOffset,
                                 AEDesc *               theDescriptor);

EXTERN_API( OSErr )
CreateCompDescriptor            (DescType               comparisonOperator,
                                 AEDesc *               operand1,
                                 AEDesc *               operand2,
                                 Boolean                disposeInputs,
                                 AEDesc *               theDescriptor);

EXTERN_API( OSErr )
CreateLogicalDescriptor         (AEDescList *           theLogicalTerms,
                                 DescType               theLogicOperator,
                                 Boolean                disposeInputs,
                                 AEDesc *               theDescriptor);


EXTERN_API( OSErr )
CreateObjSpecifier              (DescType               desiredClass,
                                 AEDesc *               theContainer,
                                 DescType               keyForm,
                                 AEDesc *               keyData,
                                 Boolean                disposeInputs,
                                 AEDesc *               objSpecifier);

EXTERN_API( OSErr )
CreateRangeDescriptor           (AEDesc *               rangeStart,
                                 AEDesc *               rangeStop,
                                 Boolean                disposeInputs,
                                 AEDesc *               theDescriptor);


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

#endif /* __AEPACKOBJECT__ */

