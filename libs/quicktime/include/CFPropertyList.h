/*
     File:       CFPropertyList.h
 
     Contains:   CoreFoundation PropertyList
 
     Version:    Technology: Mac OS X
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFPROPERTYLIST__
#define __CFPROPERTYLIST__

#ifndef __CFBASE__
#include <CFBase.h>
#endif

#ifndef __CFDATA__
#include <CFData.h>
#endif

#ifndef __CFSTRING__
#include <CFString.h>
#endif



/* Type to mean any instance of a property list type;
   currently, CFString, CFData, CFNumber, CFBoolean, CFDate,
   CFArray, and CFDictionary. 
*/


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

typedef CFTypeRef                       CFPropertyListRef;
enum {
    kCFPropertyListImmutable    = 0,
    kCFPropertyListMutableContainers = 1,
    kCFPropertyListMutableContainersAndLeaves = 2
};

/* 
Creates a property list object (CFString, CFDictionary, CFArray, or CFData) 
from its XML description; xmlData should be the raw bytes of that description, 
possibly the contents of an XML file.  Returns NULL if the data cannot be parsed; 
if the parse fails and errorString is non-NULL, a human-readable description of 
the failure is returned in errorString.  It is the caller's responsibility to 
release either the returned object or the error string, whichever is applicable. 
*/
EXTERN_API_C( CFPropertyListRef )
CFPropertyListCreateFromXMLData (CFAllocatorRef         allocator,
                                 CFDataRef              xmlData,
                                 CFOptionFlags          mutabilityOption,
                                 CFStringRef *          errorString);

/* 
Returns the XML description of the given object; propertyList must be one of
the supported property list types, and (for composite types like CFArray and CFDictionary) 
must not contain any elements that are not themselves of a property list type.  
If a non-property list type is encountered, NULL is returned.  The returned data is
appropriate for writing out to an XML file.  Note that a data, not a string, is 
returned because the bytes contain in them a description of the string encoding used. 
*/
EXTERN_API_C( CFDataRef )
CFPropertyListCreateXMLData     (CFAllocatorRef         allocator,
                                 CFPropertyListRef      propertyList);



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

#endif /* __CFPROPERTYLIST__ */

