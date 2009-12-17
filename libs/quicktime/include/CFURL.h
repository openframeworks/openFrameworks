/*
     File:       CFURL.h
 
     Contains:   CoreFoundation urls
 
     Version:    Technology: Mac OS X
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFURL__
#define __CFURL__

#ifndef __CFBASE__
#include <CFBase.h>
#endif

#ifndef __CFDATA__
#include <CFData.h>
#endif

#ifndef __CFSTRING__
#include <CFString.h>
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

#if PRAGMA_ENUM_ALWAYSINT
    #if defined(__fourbyteints__) && !__fourbyteints__ 
        #define __CFURL__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __CFURL__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

typedef UInt32                          CFURLPathStyle;
enum {
    kCFURLPOSIXPathStyle        = 0,
    kCFURLHFSPathStyle          = 1,
    kCFURLWindowsPathStyle      = 2
};

typedef struct __CFURL * CFURLRef;
EXTERN_API_C( CFTypeID )
CFURLGetTypeID                  (void);

/* encoding will be used both to interpret the bytes of URLBytes, and to */
/* interpret any percent-escapes within the bytes. */
EXTERN_API_C( CFURLRef )
CFURLCreateWithBytes            (CFAllocatorRef         allocator,
                                 const UInt8 *          URLBytes,
                                 CFIndex                length,
                                 CFStringEncoding       encoding,
                                 CFURLRef               baseURL);

/* Escapes any character that is not 7-bit ASCII with the byte-code */
/* for the given encoding.  If escapeWhitespace is true, whitespace */
/* characters (' ', '\t', '\r', '\n') will be escaped also (desirable */
/* if embedding the URL into a larger text stream like HTML) */
EXTERN_API_C( CFDataRef )
CFURLCreateData                 (CFAllocatorRef         allocator,
                                 CFURLRef               url,
                                 CFStringEncoding       encoding,
                                 Boolean                escapeWhitespace);

/* Any escape sequences in URLString will be interpreted via UTF-8. */
EXTERN_API_C( CFURLRef )
CFURLCreateWithString           (CFAllocatorRef         allocator,
                                 CFStringRef            URLString,
                                 CFURLRef               baseURL);

/* filePath should be the URL's path expressed as a path of the type */
/* fsType.  If filePath is not absolute, the resulting URL will be */
/* considered relative to the current working directory (evaluated */
/* at creation time).  isDirectory determines whether filePath is */
/* treated as a directory path when resolving against relative path */
/* components */
EXTERN_API_C( CFURLRef )
CFURLCreateWithFileSystemPath   (CFAllocatorRef         allocator,
                                 CFStringRef            filePath,
                                 CFURLPathStyle         pathStyle,
                                 Boolean                isDirectory);

/* Returns the URL's path in a form suitable for file system use. */
/* The path has been fully processed for the file system type requested */
/* by fsType.  If resolveAgainstBase is TRUE, then the URL's path is first */
/* resolved against that of its base URL (if a base URL exists) before being returned. */
EXTERN_API_C( CFStringRef )
CFURLCreateStringWithFileSystemPath (CFAllocatorRef     allocator,
                                 CFURLRef               anURL,
                                 CFURLPathStyle         pathStyle,
                                 Boolean                resolveAgainstBase);

/* Creates a new URL by resolving the relative portion of relativeURL against its base. */
EXTERN_API_C( CFURLRef )
CFURLCopyAbsoluteURL            (CFURLRef               relativeURL);

/* Returns the URL's string. */
EXTERN_API_C( CFStringRef )
CFURLGetString                  (CFURLRef               anURL);

/* Returns the base URL if it exists */
EXTERN_API_C( CFURLRef )
CFURLGetBaseURL                 (CFURLRef               anURL);

/* All URLs can be broken into two pieces - the scheme (preceding the first colon) and the resource */
/* specifier (following the first colon).  Most URLs are also "standard" URLs conforming to RFC 1808 */
/* (available from www.w3c.org).  This category includes URLs of the file, http, https, and ftp */
/* schemes, to name a few.  Standard URLs start the resource specifier with two slashes ("//"), and */
/* can be broken into 4 distinct pieces - the scheme, the net location, the path, and further */
/* resource specifiers (typically an optional parameter, query, and/or fragment).  The net location */
/* appears immediately following the two slashes and goes up to the next slash; it's format is */
/* scheme-specific, but is usually composed of some or all of a username, password, host name, and */
/* port.  The path is a series of path components separated by slashes; if the net location is */
/* present, the path always begins with a slash.  Standard URLs can be relative to another URL, in */
/* which case at least the scheme and possibly other pieces as well come from the base URL (see RFC */
/* 1808 for precise details when resolving a relative URL against its base).  The full URL is therefore */
/* <scheme> "://" <net location> <path, always starting with a slash> <additional resource specifiers> */
/* If a given CFURL can be decomposed (that is, conforms to RFC 1808), you can ask for each of the four */
/* basic pieces (scheme, net location, path, and resource specifer) separately, as well as for its base */
/* URL.  The basic pieces are returned with any percent escape sequences still in place (although note */
/* that the scheme may not legally include any percent escapes); this is to allow the caller to distinguish */
/* between percent sequences that may have syntactic meaning if replaced by the character being escaped */
/* (for instance, a '/' in a path component).  Since only the individual schemes know which characters */
/* are syntactically significant, CFURL cannot safely replace any percent escape sequences.  However, */
/* you can use CFStringCreateReplacingPercentEscapes() to create a new string with the percent escapes */
/* removed.  The first argument is the original string containing percent escapes; the second argument */
/* is a string containing any characters that, if escaped in the original string, should NOT be replaced. */
/* Note that this function may return the original string (retained) if the original string contains no */
/* escape sequences to be processed. */
/* If a given CFURL can not be decomposed, you can ask for its scheme and its resource specifier; asking */
/* it for its net location or path will return NULL. */
/* All of the methods discussed above do not require an autorelease pool to be in place. */
/* To get more refined information about the components of a decomposable CFURL, you may ask for more */
/* specific pieces of the URL, expressed with the percent escapes removed.  These functions require an */
/* autorelease pool to be in place, as they will return an autoreleased CFString.  They are */
/* CFURLHostName(), CFURLPortNumber() (actually returns an Int32; does not use the autorelease pool), */
/* CFURLUserName(), CFURLPassword(), CFURLQuery(), CFURLParameters(), and CFURLFragment().  Because the */
/* parameters, query, and fragment of an URL may contain scheme-specific syntaxes, these methods take a */
/* second argument, giving a list of characters which should NOT be replaced if percent escaped.  For */
/* instance, the ftp parameter syntax gives simple key-value pairs as "<key>=<value>;"  Clearly if a key */
/* or value includes either '=' or ';', it must be escaped to avoid corrupting the meaning of the */
/* parameters, so the caller may request the parameter string as */
/* CFStringRef myParams = CFURLParameters(ftpURL, CFSTR("=;%")); */
/* requesting that all percent escape sequences be replaced by the represented characters, except for escaped */
/* '=', '%' or ';' characters.  If you are not interested in any escape sequences being left in, pass NULL for the */
/* second argument. */
/* Returns TRUE if anURL conforms to RFC 1808 */
EXTERN_API_C( Boolean )
CFURLCanBeDecomposed            (CFURLRef               anURL);

/* The next several methods leave any percent escape sequences intact */
EXTERN_API_C( CFStringRef )
CFURLCopyScheme                 (CFURLRef               anURL);

/* NULL if CFURLCanBeDecomposed(anURL) is FALSE */
EXTERN_API_C( CFStringRef )
CFURLCopyNetLocation            (CFURLRef               anURL);

/* NULL if CFURLCanBeDecomposed(anURL) is FALSE; also does not resolve */
/* the URL against its base.  See also CFCreateAbsoluteURL() and */
/* CFStringCreateFileSystemPathFromURL() */
EXTERN_API_C( CFStringRef )
CFURLCopyPath                   (CFURLRef               anURL);

/* Returns whether anURL's path represents a directory */
/* (TRUE returned) or a simple file (FALSE returned) */
EXTERN_API_C( Boolean )
CFURLHasDirectoryPath           (CFURLRef               anURL);

/* Any additional resource specifiers after the path.  For URLs */
/* that cannot be decomposed, this is everything except the scheme itself. */
EXTERN_API_C( CFStringRef )
CFURLCopyResourceSpecifier      (CFURLRef               anURL);

EXTERN_API_C( CFStringRef )
CFURLCopyHostName               (CFURLRef               anURL);

EXTERN_API_C( SInt32 )
CFURLGetPortNumber              (CFURLRef               anURL);

/* Returns -1 if no port number is specified */
EXTERN_API_C( CFStringRef )
CFURLCopyUserName               (CFURLRef               anURL);

EXTERN_API_C( CFStringRef )
CFURLCopyPassword               (CFURLRef               anURL);

/* These remove all percent escape sequences except those for */
/* characters in charactersToLeaveEscaped.  If charactersToLeaveEscaped */
/* is empty (""), all percent escape sequences are replaced by their */
/* corresponding characters.  If charactersToLeaveEscaped is NULL, */
/* then no escape sequences are removed at all */
EXTERN_API_C( CFStringRef )
CFURLCopyParameterString        (CFURLRef               anURL,
                                 CFStringRef            charactersToLeaveEscaped);

EXTERN_API_C( CFStringRef )
CFURLCopyQueryString            (CFURLRef               anURL,
                                 CFStringRef            charactersToLeaveEscaped);

EXTERN_API_C( CFStringRef )
CFURLCopyFragment               (CFURLRef               anURL,
                                 CFStringRef            charactersToLeaveEscaped);

/* Returns a string with any percent escape sequences that do NOT */
/* correspond to characters in charactersToLeaveEscaped with their */
/* equivalent.  Returns NULL on failure (if an invalid percent sequence */
/* is encountered), or the original string itself if no characters need to be replaced. */
EXTERN_API_C( CFStringRef )
CFURLCreateStringByReplacingPercentEscapes (CFAllocatorRef  allocator,
                                 CFStringRef            originalString,
                                 CFStringRef            charactersToLeaveEscaped);


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __CFURL__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__CFURL__RESTORE_PACKED_ENUMS)
    #pragma options(pack_enums)
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

#endif /* __CFURL__ */

