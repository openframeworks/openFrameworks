/*
     File:       JManager.h
 
     Contains:   Routines that can be used to invoke the Java Virtual Machine in MRJ
 
     Version:    Technology: MRJ 2.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1996-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __JMANAGER__
#define __JMANAGER__

/*
 *  Note:   If you want to use any JManger routines which use JNI or JRI functionality,
 *          you must #include <jni.h> or <jri.h> before this file.
 *
 */
#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __DRAG__
#include <Drag.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __TEXTCOMMON__
#include <TextCommon.h>
#endif

#ifndef __CODEFRAGMENTS__
#include <CodeFragments.h>
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
        #define __JMANAGER__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __JMANAGER__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

enum {
    kJMVersion                  = 0x11800007,                   /* using Sun's 1.1.8 APIs, our rev 7 APIs. */
    kDefaultJMTime              = 0x00000400                    /* how much time to give the JM library on "empty" events, in milliseconds. */
};

enum {
    kJMVersionError             = -60000L,
    kJMExceptionOccurred        = -60001L,
    kJMBadClassPathError        = -60002L
};

/*
 * Special codes for JMFrameKey, JMFrameKeyRelease:
 *
 * When your app notices that a modifiers is pressed (must be done by polling,
 * unless Mac OS changes to support sending modifiers as events)
 * you should notify the runtime using JMFrameKey, JMFrameKeyRelease, using
 * these constants for asciiChar and keyCode.  This will allow the AWT to
 * synthesize the appropriate events
 */
enum {
    kModifierAsciiChar          = 0,
    kModifierKeyCode            = 0
};

/*
 *  Private data structures
 *
 *  JMClientData        - enough bits to reliably store a pointer to arbitrary, client-specific data. 
 *  JMSessionRef        - references the entire java runtime 
 *  JMTextRef           - a Text string, length, and encoding 
 *  JMTextEncoding      - which encoding to use when converting in and out of Java strings.
 *  JMFrameRef          - a java frame 
 *  JMAWTContextRef     - a context for the AWT to request frames, process events 
 *  JMAppletLocatorRef  - a device for locating, fetching, and parsing URLs that may contain applets 
 *  JMAppletViewerRef   - an object that displays applets in a Frame 
 *  JMAppletPageRef     - a way to group JMAWTContextRef's so they share the same class loader
 */
typedef void *                          JMClientData;
typedef struct OpaqueJMSessionRef*      JMSessionRef;
typedef struct OpaqueJMFrameRef*        JMFrameRef;
typedef struct OpaqueJMTextRef*         JMTextRef;
typedef struct OpaqueJMAWTContextRef*   JMAWTContextRef;
typedef struct OpaqueJMAppletLocatorRef*  JMAppletLocatorRef;
typedef struct OpaqueJMAppletViewerRef*  JMAppletViewerRef;
typedef struct OpaqueJMAppletPageRef*   JMAppletPageRef;
typedef TextEncoding                    JMTextEncoding;

/*
 * The runtime requires certain callbacks be used to communicate between
 * session events and the embedding application.
 *
 * In general, you can pass nil as a callback and a "good" default will be used.
 *
 *  JMConsoleProcPtr        - redirect stderr or stdout - the message is delivered in the encoding specified when
 *                              you created the session, or possibly binary data.
 *  JMConsoleReadProcPtr    - take input from the user from a console or file.  The input is expected to 
 *                              be in the encoding specified when you opened the session.
 *  JMExitProcPtr           - called via System.exit(int), return "true" to kill the current thread,
 *                              false, to cause a 'QUIT' AppleEvent to be sent to the current process,
 *                              or just tear down the runtime and exit to shell immediately
 * JMLowMemoryProcPtr       - This callback is available to notify the embedding application that
 *                              a low memory situation has occurred so it can attempt to recover appropriately.
 * JMAuthenicateURLProcPtr  - prompt the user for autentication based on the URL.  If you pass
 *                              nil, JManager will prompt the user.  Return false if the user pressed cancel.
 */
typedef CALLBACK_API_C( void , JMConsoleProcPtr )(JMSessionRef session, const void *message, SInt32 messageLengthInBytes);
typedef CALLBACK_API_C( SInt32 , JMConsoleReadProcPtr )(JMSessionRef session, void *buffer, SInt32 maxBufferLength);
typedef CALLBACK_API_C( Boolean , JMExitProcPtr )(JMSessionRef session, SInt32 status);
typedef CALLBACK_API_C( Boolean , JMAuthenticateURLProcPtr )(JMSessionRef session, const char *url, const char *realm, char userName[255], char password[255])/*  into C string parameters  */;
typedef CALLBACK_API_C( void , JMLowMemoryProcPtr )(JMSessionRef session);

struct JMSessionCallbacks {
    UInt32                          fVersion;                   /* should be set to kJMVersion */
    JMConsoleProcPtr                fStandardOutput;            /* JM will route "stdout" to this function. */
    JMConsoleProcPtr                fStandardError;             /* JM will route "stderr" to this function. */
    JMConsoleReadProcPtr            fStandardIn;                /* read from console - can be nil for default behavior (no console IO) */
    JMExitProcPtr                   fExitProc;                  /* handle System.exit(int) requests */
    JMAuthenticateURLProcPtr        fAuthenticateProc;          /* present basic authentication dialog */
    JMLowMemoryProcPtr              fLowMemProc;                /* Low Memory notification Proc */
};
typedef struct JMSessionCallbacks       JMSessionCallbacks;

enum JMVerifierOptions {
    eDontCheckCode              = 0,
    eCheckRemoteCode            = 1,
    eCheckAllCode               = 2
};
typedef enum JMVerifierOptions JMVerifierOptions;


/*
 * JMRuntimeOptions is a mask that allows you to specify certain attributes
 * for the runtime. Bitwise or the fields together, or use one of the "premade" entries.
 * eJManager2Defaults is the factory default, and best bet to use.
 */

enum JMRuntimeOptions {
    eJManager2Defaults          = 0,
    eUseAppHeapOnly             = (1 << 0),
    eDisableJITC                = (1 << 1),
    eEnableDebugger             = (1 << 2),
    eDisableInternetConfig      = (1 << 3),
    eInhibitClassUnloading      = (1 << 4),
    eEnableProfiling            = (1 << 5),
    eJManager1Compatible        = (eDisableInternetConfig | eInhibitClassUnloading)
};
typedef enum JMRuntimeOptions JMRuntimeOptions;




/*
 * Returns the version of the currently installed JManager library.
 * Compare to kJMVersion.  This is the only call that doesn't
 * require a session, or a reference to something that references
 * a session.
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( unsigned long )
JMGetVersion                    (void);

/*
 * Returns the version number of the MRJ installation.  This is an
 * official version number that will change for each official release
 * of MRJ, whereas for an incremental MRJ release, the version number
 * returned by JMGetVersion may not change.
 */
EXTERN_API_C( UInt32 )
JMGetMRJRuntimeVersion          (void);

/*
 * JMOpenSession creates a new Java Runtime.  Note that JManger 2.0 doesn't set 
 * security options at the time of runtime instantiation.  AppletViewer Objecs have
 * seperate security attributes bound to them, and the verifier is availiable elsewhere
 * as well.  The client data parameter lets a client associate an arbitgrary tagged pointer
 * with the seession.
 * When you create the session, you must specify the desired Text Encoding to use for
 * console IO.  Usually, its OK to use "kTextEncodingMacRoman".  See TextCommon.h for the list.
 */
EXTERN_API_C( OSStatus )
JMOpenSession                   (JMSessionRef *         session,
                                 JMRuntimeOptions       runtimeOptions,
                                 JMVerifierOptions      verifyMode,
                                 const JMSessionCallbacks * callbacks,
                                 JMTextEncoding         desiredEncoding,
                                 JMClientData           data);

EXTERN_API_C( OSStatus )
JMCloseSession                  (JMSessionRef           session);


/*
 * Client data getter/setter functions.
 */
EXTERN_API_C( OSStatus )
JMGetSessionData                (JMSessionRef           session,
                                 JMClientData *         data);

EXTERN_API_C( OSStatus )
JMSetSessionData                (JMSessionRef           session,
                                 JMClientData           data);


/*
 * Prepend the target of the FSSpec to the class path.
 * If a file, .zip or other known archive file - not a .class file
 */
EXTERN_API_C( OSStatus )
JMAddToClassPath                (JMSessionRef           session,
                                 const FSSpec *         spec);


/*
 * Utility returns (client owned) null terminated handle containing "file://xxxx", or nil if fnfErr
 */
EXTERN_API_C( Handle )
JMFSSToURL                      (JMSessionRef           session,
                                 const FSSpec *         spec);


/*
 * Turns "file:///disk/file" into an FSSpec.  other handlers return paramErr
 */
EXTERN_API_C( OSStatus )
JMURLToFSS                      (JMSessionRef           session,
                                 JMTextRef              urlString,
                                 FSSpec *               spec);


/*
 * JMIdle gives time to all Java threads. Giving more time makes Java programs run faster,
 * but can reduce overall system responsiveness. JMIdle will return sooner if low-level (user)
 * events appear in the event queue.
 */
EXTERN_API_C( OSStatus )
JMIdle                          (JMSessionRef           session,
                                 UInt32                 jmTimeMillis);


#endif  /* CALL_NOT_IN_CARBON */

/*
 * JMGetCurrenvEnv gives access to the underlying JRI interface, if available.
 * This gives the client more control over the underlying Java runtime, without
 * exposing the implementation too much.
 * NOTE: JRI is deprecated; please upgrade your code to work with JNI instead.
 */
#ifdef JRI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( JRIRuntimeInstance *)
JMGetJRIRuntimeInstance         (JMSessionRef           session);

EXTERN_API_C( JRIEnv *)
JMGetCurrentJRIEnv              (JMSessionRef           session);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JRI_H) */

/*
 * JNI is to be preferred.
 */
#ifdef JNI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( JNIEnv *)
JMGetCurrentEnv                 (JMSessionRef           session);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JNI_H) */

/*
 * Since JManager calls reutrn jref (JRI Java references)
 * you need to convert between them and JNI references (jobjects) if you're
 * using JNI.  These routines facilitate that conversion.
 */
#if defined(JRI_H) && defined(JNI_H)
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jobject )
JMJRIRefToJNIObject             (JMSessionRef           session,
                                 JNIEnv *               env,
                                 jref                   jriRef);

EXTERN_API_C( jref )
JMJNIObjectToJRIRef             (JMSessionRef           session,
                                 JNIEnv *               env,
                                 jobject                jniObject);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JRI_H) && defined(JNI_H) */

/*
 * Java defines system-wide properties that applets can use to make queries about the
 * host system. Many of these properties correspond to defaults provided by "Internet Config."
 * JMPutSessionProperty can be used by a client program to modify various system-wide properties.
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
JMGetSessionProperty            (JMSessionRef           session,
                                 JMTextRef              propertyName,
                                 JMTextRef *            propertyValue);

EXTERN_API_C( OSStatus )
JMPutSessionProperty            (JMSessionRef           session,
                                 JMTextRef              propertyName,
                                 JMTextRef              propertyValue);


#endif  /* CALL_NOT_IN_CARBON */

/*
 * Returns a com.apple.mrj.JManager.JMSession object
 */
#ifdef JNI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jobject )
JMGetSessionJNIObject           (JMSessionRef           session,
                                 JNIEnv *               env);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JNI_H) */

#ifdef JRI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jref )
JMGetSessionObject              (JMSessionRef           session);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JRI_H) */

/*
 * JMText: opaque object that encapsulates a string, length, and
 * character encoding.  Strings passed between JManager and the
 * embedding application goes through this interface.  Only the most
 * rudimentary conversion routines are supplied - it is expected that
 * the embedding application will most of its work in the System Script.
 *
 * These APIs present some questions about who actually owns the 
 * JMText.  The rule is, if you created a JMTextRef, you are responsible
 * for deleting it after passing it into the runtime.  If the runtime passes
 * one to you, it will be deleted after the callback.
 *
 * If a pointer to an uninitialised JMTextRef is passed in to a routine (eg JMGetSessionProperty),
 * it is assumed to have been created for the caller, and it is the callers responsibility to
 * dispose of it.
 *
 * The encoding types are taken verbatim from the Text Encoding Converter,
 * which handles the ugly backside of script conversion.
 */
/*
 * JMNewTextRef can create from a buffer of data in the specified encoding
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
JMNewTextRef                    (JMSessionRef           session,
                                 JMTextRef *            textRef,
                                 JMTextEncoding         encoding,
                                 const void *           charBuffer,
                                 UInt32                 bufferLengthInBytes);


/*
 * JMCopyTextRef clones a text ref.
 */
EXTERN_API_C( OSStatus )
JMCopyTextRef                   (JMTextRef              textRefSrc,
                                 JMTextRef *            textRefDst);


/*
 * Disposes of a text ref passed back from the runtime, or created explicitly through JMNewTextRef
 */
EXTERN_API_C( OSStatus )
JMDisposeTextRef                (JMTextRef              textRef);


/*
 * Returns the text length, in characters
 */
EXTERN_API_C( OSStatus )
JMGetTextLength                 (JMTextRef              textRef,
                                 UInt32 *               textLengthInCharacters);


/*
 * Returns the text length, in number of bytes taken in the destination encoding
 */
EXTERN_API_C( OSStatus )
JMGetTextLengthInBytes          (JMTextRef              textRef,
                                 JMTextEncoding         dstEncoding,
                                 UInt32 *               textLengthInBytes);


/*
 * Copies the specified number of characters to the destination buffer with the appropriate
 * destination encoding.
 */
EXTERN_API_C( OSStatus )
JMGetTextBytes                  (JMTextRef              textRef,
                                 JMTextEncoding         dstEncoding,
                                 void *                 textBuffer,
                                 UInt32                 textBufferLength,
                                 UInt32 *               numCharsCopied);


#endif  /* CALL_NOT_IN_CARBON */

/*
 * Return the JMText as a reference to a Java String.  Note that
 * this is the only reference to the string - it will be collected if you don't
 * hang on to it.
 */
#ifdef JNI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jstring )
JMTextToJNIJavaString           (JMTextRef              textRef,
                                 JMSessionRef           session,
                                 JNIEnv *               env);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JNI_H) */

#ifdef JRI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jref )
JMTextToJavaString              (JMTextRef              textRef);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JRI_H) */

/*
 * Returns a Handle to a null terminated, "C" string in the System Script.
 * Note that using this routine could result in data loss, if the characters
 * are not availiable in the System Script.
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( Handle )
JMTextToMacOSCStringHandle      (JMTextRef              textRef);




/*
 * Proxy properties in the runtime.
 *
 * These will only be checked if InternetConfig isn't used to specify properties,
 * or if it doesn't have the data for these.
 */
#endif  /* CALL_NOT_IN_CARBON */


struct JMProxyInfo {
    Boolean                         useProxy;
    char                            proxyHost[255];
    UInt16                          proxyPort;
};
typedef struct JMProxyInfo              JMProxyInfo;

enum JMProxyType {
    eHTTPProxy                  = 0,
    eFirewallProxy              = 1,
    eFTPProxy                   = 2
};
typedef enum JMProxyType JMProxyType;

#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
JMGetProxyInfo                  (JMSessionRef           session,
                                 JMProxyType            proxyType,
                                 JMProxyInfo *          proxyInfo);

EXTERN_API_C( OSStatus )
JMSetProxyInfo                  (JMSessionRef           session,
                                 JMProxyType            proxyType,
                                 const JMProxyInfo *    proxyInfo);


/*
 * Security - JManager 2.0 security is handled on a per-applet basis.
 * There are some security settings that are inherited from InternetConfig
 * (Proxy Servers) but the verifier can now be enabled and disabled.
 */
EXTERN_API_C( OSStatus )
JMGetVerifyMode                 (JMSessionRef           session,
                                 JMVerifierOptions *    verifierOptions);

EXTERN_API_C( OSStatus )
JMSetVerifyMode                 (JMSessionRef           session,
                                 JMVerifierOptions      verifierOptions);




/*
 * The basic unit of AWT interaction is the JMFrame.  A JMFrame is bound to top level
 * awt Frame, Window, or Dialog.  When a user event occurs for a MacOS window, the event is passed
 * to the corrosponding frame object.  Similarly, when an AWT event occurs that requires the
 * Mac OS Window to change, a callback is made.  JManager 1.x bound the frame to the window through
 * a callback to set and restore the windows GrafPort.  In JManager 2.0, a GrafPort, Offset, and 
 * ClipRgn are specified up front - changes in visibility and structure require that these be re-set.
 * This enables support for the JavaSoft DrawingSurface API - and also improves graphics performance.
 * You should reset the graphics attributes anytime the visiblity changes, like when scrolling.
 * You should also set it initially when the AWTContext requests the frame.
 * At various times, JM will call back to the client to register a new JMFrame, 
 * indicating the frame type.  The client should take the following steps:
 *
 *  o   Create a new invisible window of the specified type
 *  o   Fill in the callbacks parameter with function pointers
 *  o   Do something to bind the frame to the window (like stuff the WindowPtr in the JMClientData of the frame)
 *  o   Register the visiblity parameters (GrafPtr, etc) with the frame
 */
#endif  /* CALL_NOT_IN_CARBON */


enum ReorderRequest {
    eBringToFront               = 0,                            /* bring the window to front */
    eSendToBack                 = 1,                            /* send the window to back */
    eSendBehindFront            = 2                             /* send the window behind the front window */
};
typedef enum ReorderRequest ReorderRequest;

typedef CALLBACK_API_C( void , JMSetFrameSizeProcPtr )(JMFrameRef frame, const Rect *newBounds);
typedef CALLBACK_API_C( void , JMFrameInvalRectProcPtr )(JMFrameRef frame, const Rect *r);
typedef CALLBACK_API_C( void , JMFrameShowHideProcPtr )(JMFrameRef frame, Boolean showFrameRequested);
typedef CALLBACK_API_C( void , JMSetTitleProcPtr )(JMFrameRef frame, JMTextRef title);
typedef CALLBACK_API_C( void , JMCheckUpdateProcPtr )(JMFrameRef frame);
typedef CALLBACK_API_C( void , JMReorderFrame )(JMFrameRef frame, ReorderRequest theRequest);
typedef CALLBACK_API_C( void , JMSetResizeable )(JMFrameRef frame, Boolean resizeable);
typedef CALLBACK_API_C( void , JMGetFrameInsets )(JMFrameRef frame, Rect *insets);
/*
 * New in JManager 2.1:
 *  If the AWT needs to set focus to a frame (in the case of multiple JMFrames within
 *  a single Mac OS Frame) it will call back to the embedding application using
 *  JMRRequestFocus.  The application should then defocus what it thought did have the
 *  focus, and set the focus to the new frame.
 *  If the user is tabbing within a JMFrame, and the focus reaches the last focusable
 *  component (or the first, if focus is traversing backwards) JMNexetFocus will be called.
 *  The application should defocus the component that requests this, and focus the next application
 *  visible focusable element.  (If none, send focus back to the frame.)
 */
typedef CALLBACK_API_C( void , JMNextFocus )(JMFrameRef frame, Boolean forward);
typedef CALLBACK_API_C( void , JMRequestFocus )(JMFrameRef frame);

struct JMFrameCallbacks {
    UInt32                          fVersion;                   /* should be set to kJMVersion */
    JMSetFrameSizeProcPtr           fSetFrameSize;
    JMFrameInvalRectProcPtr         fInvalRect;
    JMFrameShowHideProcPtr          fShowHide;
    JMSetTitleProcPtr               fSetTitle;
    JMCheckUpdateProcPtr            fCheckUpdate;
    JMReorderFrame                  fReorderFrame;
    JMSetResizeable                 fSetResizeable;
    JMGetFrameInsets                fGetInsets;
    JMNextFocus                     fNextFocus;
    JMRequestFocus                  fRequestFocus;
};
typedef struct JMFrameCallbacks         JMFrameCallbacks;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
JMSetFrameVisibility            (JMFrameRef             frame,
                                 GrafPtr                famePort,
                                 Point                  frameOrigin,
                                 RgnHandle              frameClip);

EXTERN_API_C( OSStatus )
JMGetFrameData                  (JMFrameRef             frame,
                                 JMClientData *         data);

EXTERN_API_C( OSStatus )
JMSetFrameData                  (JMFrameRef             frame,
                                 JMClientData           data);

EXTERN_API_C( OSStatus )
JMGetFrameSize                  (JMFrameRef             frame,
                                 Rect *                 result);

/* note that the top left indicates the "global" position of this frame */
/* use this to update the frame position when it gets moved */
EXTERN_API_C( OSStatus )
JMSetFrameSize                  (JMFrameRef             frame,
                                 const Rect *           newSize);

/*
 * Dispatch a particular event to an embedded frame
 */
EXTERN_API_C( OSStatus )
JMFrameClickWithEventRecord     (JMFrameRef             frame,
                                 Point                  localPos,
                                 const EventRecord *    event);

EXTERN_API_C( OSStatus )
JMFrameKey                      (JMFrameRef             frame,
                                 char                   asciiChar,
                                 char                   keyCode,
                                 short                  modifiers);

EXTERN_API_C( OSStatus )
JMFrameKeyRelease               (JMFrameRef             frame,
                                 char                   asciiChar,
                                 char                   keyCode,
                                 short                  modifiers);

EXTERN_API_C( OSStatus )
JMFrameUpdate                   (JMFrameRef             frame,
                                 RgnHandle              updateRgn);

EXTERN_API_C( OSStatus )
JMFrameActivate                 (JMFrameRef             frame,
                                 Boolean                activate);

EXTERN_API_C( OSStatus )
JMFrameResume                   (JMFrameRef             frame,
                                 Boolean                resume);

EXTERN_API_C( OSStatus )
JMFrameMouseOver                (JMFrameRef             frame,
                                 Point                  localPos,
                                 short                  modifiers);

EXTERN_API_C( OSStatus )
JMFrameShowHide                 (JMFrameRef             frame,
                                 Boolean                showFrame);

EXTERN_API_C( OSStatus )
JMFrameGoAway                   (JMFrameRef             frame);

EXTERN_API_C( JMAWTContextRef )
JMGetFrameContext               (JMFrameRef             frame);

EXTERN_API_C( OSStatus )
JMFrameDragTracking             (JMFrameRef             frame,
                                 DragTrackingMessage    message,
                                 DragReference          theDragRef);

EXTERN_API_C( OSStatus )
JMFrameDragReceive              (JMFrameRef             frame,
                                 DragReference          theDragRef);

/*
 * JMFrameClick is deprecated - please use JMFrameClickWithEventRecord instead.
 */
EXTERN_API_C( OSStatus )
JMFrameClick                    (JMFrameRef             frame,
                                 Point                  localPos,
                                 short                  modifiers);

/*
 * If you may have multiple JMFrames in a single Mac OS Window (for example,
 * in a browser) then use JMFrameFocus to control when you believe the
 * frame should logically contain the focus.  This will allow for the correct
 * appearance of Controls and Text Fields.  If you will only have one
 * JMFrame per Mac OS Window, use JMFrameFocus as well as JMFrameActivate to
 * control hiliting.
 */
EXTERN_API_C( OSStatus )
JMFrameFocus                    (JMFrameRef             frame,
                                 Boolean                gotFocus);

/*
 * Cause a Frame and its contents to be rendered in a GrafPort.
 * This is typically going to be used to cause an applet to print itself
 * into a PrGrafPort.
 * If you pass 'true' as the last parameter, the paint(Graphics) method of the
 * frame is called, rather than print(Graphics).
 */
EXTERN_API_C( OSStatus )
JMDrawFrameInPort               (JMFrameRef             frame,
                                 GrafPtr                framePort,
                                 Point                  frameOrigin,
                                 RgnHandle              clipRgn,
                                 Boolean                callPaintAsOpposedToPrint);

#endif  /* CALL_NOT_IN_CARBON */

/* 
 * returns the java.awt.Frame for this frame 
*/
#ifdef JNI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jobject )
JMGetAWTFrameJNIObject          (JMFrameRef             frame,
                                 JNIEnv *               env);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JNI_H) */

#ifdef JRI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jref )
JMGetAWTFrameObject             (JMFrameRef             frame);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JRI_H) */

/* 
 * returns the com.apple.mrj.JManager.JMFrame for this frame 
 */
#ifdef JNI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jobject )
JMGetJMFrameJNIObject           (JMFrameRef             frame,
                                 JNIEnv *               env);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JNI_H) */

#ifdef JRI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jref )
JMGetJMFrameObject              (JMFrameRef             frame);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JRI_H) */

/*
 * Window types
 */

enum JMFrameKind {
    eBorderlessModelessWindowFrame = 0,
    eModelessWindowFrame        = 1,
    eModalWindowFrame           = 2,
    eModelessDialogFrame        = 3
};
typedef enum JMFrameKind JMFrameKind;


/* JMAppletPageRef -
 * Creating a "page" is optional. 
 * Applets will share the same class loader (and thus static variables) iff
 * they are share the same JMAppletPageRef and have the same codebase.
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
JMNewAppletPage                 (JMAppletPageRef *      page,
                                 JMSessionRef           session);

EXTERN_API_C( OSStatus )
JMDisposeAppletPage             (JMAppletPageRef        page);



/* JMAWTContext -
 * To create a top level frame, you must use a JMAWTContext object.
 * The JMAWTContext provides a context for the AWT to request frames.
 * A AWTContext has a threadgroup associated with it - all events and processing occurs
 * there.  When you create one, it is quiescent, you must call resume before it begins executing.
 */
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( OSStatus , JMRequestFrameProcPtr )(JMAWTContextRef context, JMFrameRef newFrame, JMFrameKind kind, const Rect *initialBounds, Boolean resizeable, JMFrameCallbacks *callbacks);
typedef CALLBACK_API_C( OSStatus , JMReleaseFrameProcPtr )(JMAWTContextRef context, JMFrameRef oldFrame);
typedef CALLBACK_API_C( SInt16 , JMUniqueMenuIDProcPtr )(JMAWTContextRef context, Boolean isSubmenu);
typedef CALLBACK_API_C( void , JMExceptionOccurredProcPtr )(JMAWTContextRef context, JMTextRef exceptionName, JMTextRef exceptionMsg, JMTextRef stackTrace);

struct JMAWTContextCallbacks {
    UInt32                          fVersion;                   /* should be set to kJMVersion */
    JMRequestFrameProcPtr           fRequestFrame;              /* a new frame is being created. */
    JMReleaseFrameProcPtr           fReleaseFrame;              /* an existing frame is being destroyed. */
    JMUniqueMenuIDProcPtr           fUniqueMenuID;              /* a new menu will be created with this id. */
    JMExceptionOccurredProcPtr      fExceptionOccurred;         /* just some notification that some recent operation caused an exception.  You can't do anything really from here. */
};
typedef struct JMAWTContextCallbacks    JMAWTContextCallbacks;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
JMNewAWTContext                 (JMAWTContextRef *      context,
                                 JMSessionRef           session,
                                 const JMAWTContextCallbacks * callbacks,
                                 JMClientData           data);

EXTERN_API_C( OSStatus )
JMNewAWTContextInPage           (JMAWTContextRef *      context,
                                 JMSessionRef           session,
                                 JMAppletPageRef        page,
                                 const JMAWTContextCallbacks * callbacks,
                                 JMClientData           data);

EXTERN_API_C( OSStatus )
JMDisposeAWTContext             (JMAWTContextRef        context);

EXTERN_API_C( OSStatus )
JMGetAWTContextData             (JMAWTContextRef        context,
                                 JMClientData *         data);

EXTERN_API_C( OSStatus )
JMSetAWTContextData             (JMAWTContextRef        context,
                                 JMClientData           data);

EXTERN_API_C( OSStatus )
JMCountAWTContextFrames         (JMAWTContextRef        context,
                                 UInt32 *               frameCount);

EXTERN_API_C( OSStatus )
JMGetAWTContextFrame            (JMAWTContextRef        context,
                                 UInt32                 frameIndex,
                                 JMFrameRef *           frame);

/*
 * Starting in MRJ 2.1, JMMenuSelected is deprecated.  Please use JMMenuSelectedWithModifiers instead.
 */
EXTERN_API_C( OSStatus )
JMMenuSelected                  (JMAWTContextRef        context,
                                 MenuHandle             hMenu,
                                 short                  menuItem);

/*
 * Starting in MRJ 2.1, this call takes an additional 'modifiers' parameter that you can get
 * from your event record.
 */
EXTERN_API_C( OSStatus )
JMMenuSelectedWithModifiers     (JMAWTContextRef        context,
                                 MenuHandle             hMenu,
                                 short                  menuItem,
                                 short                  modifiers);

#endif  /* CALL_NOT_IN_CARBON */

#ifdef JRI_H
/*
 * JRI Access APIs
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
JMExecMethodInContext           (JMAWTContextRef        context,
                                 jref                   objref,
                                 JRIMethodID            methodID,
                                 UInt32                 argCount,
                                 JRIValue               args[]);

EXTERN_API_C( OSStatus )
JMExecStaticMethodInContext     (JMAWTContextRef        context,
                                 JRIClassID             classID,
                                 JRIMethodID            methodID,
                                 UInt32                 argCount,
                                 JRIValue               args[]);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JRI_H) */

#ifdef JNI_H
/*
 * JNI Access APIs
 * Note that you must pass the JNIEnv to these as well.
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
JMExecJNIMethodInContext        (JMAWTContextRef        context,
                                 JNIEnv *               env,
                                 jobject                objref,
                                 jmethodID              methodID,
                                 UInt32                 argCount,
                                 jvalue                 args[]);

EXTERN_API_C( OSStatus )
JMExecJNIStaticMethodInContext  (JMAWTContextRef        context,
                                 JNIEnv *               env,
                                 jclass                 classID,
                                 jmethodID              methodID,
                                 UInt32                 argCount,
                                 jvalue                 args[]);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JNI_H) */

/*
 * return a com.apple.mrj.JManager.JMAWTContext
 */
#ifdef JNI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jobject )
JMGetAwtContextJNIObject        (JMAWTContextRef        context,
                                 JNIEnv *               env);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JNI_H) */

#ifdef JRI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jref )
JMGetAwtContextObject           (JMAWTContextRef        context);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JRI_H) */

#ifdef JNI_H
/*
 * Beginning in 2.1, this function maps a JNIEnv to the owning AWTContext, if one exists
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( JMAWTContextRef )
JMJNIToAWTContext               (JMSessionRef           session,
                                 JNIEnv *               env);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JNI_H) */



/*
 * JMAppletLocator - Since Java applets are always referenced by a Uniform Resource Locator
 * (see RFC 1737, http://www.w3.org/pub/WWW/Addressing/rfc1738.txt), we provide an object
 * that encapsulates the information about a set of applets. A JMAppletLocator is built
 * by providing a base URL, which must point at a valid HTML document containing applet
 * tags. To save a network transaction, the contents of the document may be passed optionally. 
 *
 * You can also use a JMLocatorInfoBlock for a synchronous resolution of the applet,
 * assuming that you already have the info for the tag.
 */

enum JMLocatorErrors {
    eLocatorNoErr               = 0,                            /* the html was retrieved successfully*/
    eHostNotFound               = 1,                            /* the host specified by the url could not be found*/
    eFileNotFound               = 2,                            /* the file could not be found on the host*/
    eLocatorTimeout             = 3,                            /* a timeout occurred retrieving the html text*/
    eLocatorKilled              = 4                             /* in response to a JMDisposeAppletLocator before it has completed*/
};
typedef enum JMLocatorErrors JMLocatorErrors;

typedef CALLBACK_API_C( void , JMFetchCompleted )(JMAppletLocatorRef ref, JMLocatorErrors status);

struct JMAppletLocatorCallbacks {
    UInt32                          fVersion;                   /* should be set to kJMVersion */
    JMFetchCompleted                fCompleted;                 /* called when the html has been completely fetched */
};
typedef struct JMAppletLocatorCallbacks JMAppletLocatorCallbacks;
/*
 * These structures are used to pass pre-parsed parameter
 * tags to the AppletLocator.  Implies synchronous semantics.
 */


struct JMLIBOptionalParams {
    JMTextRef                       fParamName;                 /* could be from a <parameter name=foo value=bar> or "zipbase", etc */
    JMTextRef                       fParamValue;                /* the value of this optional tag */
};
typedef struct JMLIBOptionalParams      JMLIBOptionalParams;

struct JMLocatorInfoBlock {
    UInt32                          fVersion;                   /* should be set to kJMVersion */

                                                                /* These are required to be present and not nil */

    JMTextRef                       fBaseURL;                   /* the URL of this applet's host page */
    JMTextRef                       fAppletCode;                /* code= parameter */
    short                           fWidth;                     /* width= parameter */
    short                           fHeight;                    /* height= parameter */


                                                                /* These are optional parameters */
    SInt32                          fOptionalParameterCount;    /* how many in this array */
    JMLIBOptionalParams *           fParams;                    /* pointer to an array of these (points to first element) */
};
typedef struct JMLocatorInfoBlock       JMLocatorInfoBlock;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
JMNewAppletLocator              (JMAppletLocatorRef *   locatorRef,
                                 JMSessionRef           session,
                                 const JMAppletLocatorCallbacks * callbacks,
                                 JMTextRef              url,
                                 JMTextRef              htmlText,
                                 JMClientData           data);

EXTERN_API_C( OSStatus )
JMNewAppletLocatorFromInfo      (JMAppletLocatorRef *   locatorRef,
                                 JMSessionRef           session,
                                 const JMLocatorInfoBlock * info,
                                 JMClientData           data);

EXTERN_API_C( OSStatus )
JMDisposeAppletLocator          (JMAppletLocatorRef     locatorRef);

EXTERN_API_C( OSStatus )
JMGetAppletLocatorData          (JMAppletLocatorRef     locatorRef,
                                 JMClientData *         data);

EXTERN_API_C( OSStatus )
JMSetAppletLocatorData          (JMAppletLocatorRef     locatorRef,
                                 JMClientData           data);

EXTERN_API_C( OSStatus )
JMCountApplets                  (JMAppletLocatorRef     locatorRef,
                                 UInt32 *               appletCount);

EXTERN_API_C( OSStatus )
JMGetAppletDimensions           (JMAppletLocatorRef     locatorRef,
                                 UInt32                 appletIndex,
                                 UInt32 *               width,
                                 UInt32 *               height);

EXTERN_API_C( OSStatus )
JMGetAppletTag                  (JMAppletLocatorRef     locatorRef,
                                 UInt32                 appletIndex,
                                 JMTextRef *            tagRef);

EXTERN_API_C( OSStatus )
JMGetAppletName                 (JMAppletLocatorRef     locatorRef,
                                 UInt32                 appletIndex,
                                 JMTextRef *            nameRef);


/*
 * JMAppletViewer - Applets are instantiated, one by one, by specifying a JMAppletLocator and
 * a zero-based index (Macintosh API's usually use one-based indexing, the Java language
 * uses zero, however.). The resulting applet is encapsulated in a JMAppletViewer object. 
 * Since applets can have one or more visible areas to draw in, one or more JMFrame objects
 * may be requested while the viewer is being created, or at a later time, thus the client
 * must provide callbacks to satisfy these requests.
 *
 * The window name for the ShowDocument callback is one of:
 *   _self      show in current frame
 *   _parent    show in parent frame
 *   _top       show in top-most frame
 *   _blank     show in new unnamed top-level window
 *   <other>    show in new top-level window named <other> 
 */
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( void , JMShowDocumentProcPtr )(JMAppletViewerRef viewer, JMTextRef urlString, JMTextRef windowName);
typedef CALLBACK_API_C( void , JMSetStatusMsgProcPtr )(JMAppletViewerRef viewer, JMTextRef statusMsg);

struct JMAppletViewerCallbacks {
    UInt32                          fVersion;                   /* should be set to kJMVersion */
    JMShowDocumentProcPtr           fShowDocument;              /* go to a url, optionally in a new window */
    JMSetStatusMsgProcPtr           fSetStatusMsg;              /* applet changed status message */
};
typedef struct JMAppletViewerCallbacks  JMAppletViewerCallbacks;
/*
 * NEW: per-applet security settings
 * Previously, these settings were attached to the session.
 * JManager 2.0 allows them to be attached to each viewer.
 */

enum JMNetworkSecurityOptions {
    eNoNetworkAccess            = 0,
    eAppletHostAccess           = 1,
    eUnrestrictedAccess         = 2
};
typedef enum JMNetworkSecurityOptions JMNetworkSecurityOptions;


enum JMFileSystemOptions {
    eNoFSAccess                 = 0,
    eLocalAppletAccess          = 1,
    eAllFSAccess                = 2
};
typedef enum JMFileSystemOptions JMFileSystemOptions;

/*
 * Lists of packages are comma separated,
 * the default for mrj.security.system.access is
 * "sun,netscape,com.apple".
 */


struct JMAppletSecurity {
    UInt32                          fVersion;                   /* should be set to kJMVersion */
    JMNetworkSecurityOptions        fNetworkSecurity;           /* can this applet access network resources */
    JMFileSystemOptions             fFileSystemSecurity;        /* can this applet access network resources */

    Boolean                         fRestrictSystemAccess;      /* restrict access to system packages (com.apple.*, sun.*, netscape.*) also found in the property "mrj.security.system.access" */
    Boolean                         fRestrictSystemDefine;      /* restrict classes from loading system packages (com.apple.*, sun.*, netscape.*) also found in the property "mrj.security.system.define" */

    Boolean                         fRestrictApplicationAccess; /* restrict access to application packages found in the property "mrj.security.application.access" */
    Boolean                         fRestrictApplicationDefine; /* restrict access to application packages found in the property "mrj.security.application.access" */
};
typedef struct JMAppletSecurity         JMAppletSecurity;
/*
 * AppletViewer methods
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
JMNewAppletViewer               (JMAppletViewerRef *    viewer,
                                 JMAWTContextRef        context,
                                 JMAppletLocatorRef     locatorRef,
                                 UInt32                 appletIndex,
                                 const JMAppletSecurity * security,
                                 const JMAppletViewerCallbacks * callbacks,
                                 JMClientData           data);

EXTERN_API_C( OSStatus )
JMDisposeAppletViewer           (JMAppletViewerRef      viewer);

EXTERN_API_C( OSStatus )
JMGetAppletViewerData           (JMAppletViewerRef      viewer,
                                 JMClientData *         data);

EXTERN_API_C( OSStatus )
JMSetAppletViewerData           (JMAppletViewerRef      viewer,
                                 JMClientData           data);


/*
 * You can change the applet security on the fly
 */
EXTERN_API_C( OSStatus )
JMGetAppletViewerSecurity       (JMAppletViewerRef      viewer,
                                 JMAppletSecurity *     data);

EXTERN_API_C( OSStatus )
JMSetAppletViewerSecurity       (JMAppletViewerRef      viewer,
                                 const JMAppletSecurity * data);


/*
 * JMReloadApplet reloads viewer's applet from the source.
 * JMRestartApplet reinstantiates the applet without reloading.
 */
EXTERN_API_C( OSStatus )
JMReloadApplet                  (JMAppletViewerRef      viewer);

EXTERN_API_C( OSStatus )
JMRestartApplet                 (JMAppletViewerRef      viewer);


/*
 * JMSuspendApplet tells the Java thread scheduler to stop executing the viewer's applet.
 * JMResumeApplet resumes execution of the viewer's applet.
 */
EXTERN_API_C( OSStatus )
JMSuspendApplet                 (JMAppletViewerRef      viewer);

EXTERN_API_C( OSStatus )
JMResumeApplet                  (JMAppletViewerRef      viewer);


/* 
 * To get back to the JMAppletViewerRef instance from whence a frame came,
 * as well as the ultimate frame parent (the one created _for_ the applet viewer)
 */
EXTERN_API_C( OSStatus )
JMGetFrameViewer                (JMFrameRef             frame,
                                 JMAppletViewerRef *    viewer,
                                 JMFrameRef *           parentFrame);

/*
 * To get a ref back to the Frame that was created for this JMAppletViewerRef
 */
EXTERN_API_C( OSStatus )
JMGetViewerFrame                (JMAppletViewerRef      viewer,
                                 JMFrameRef *           frame);

#endif  /* CALL_NOT_IN_CARBON */

/*
 * To get the ref of the com.apple.mrj.JManager.JMAppletViewer java object
 */
#ifdef JNI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jobject )
JMGetAppletViewerJNIObject      (JMAppletViewerRef      viewer,
                                 JNIEnv *               env);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JNI_H) */

#ifdef JRI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jref )
JMGetAppletViewerObject         (JMAppletViewerRef      viewer);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JRI_H) */

/*
 * To get the ref of the java.applet.Applet itself
 */
#ifdef JNI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jobject )
JMGetAppletJNIObject            (JMAppletViewerRef      viewer,
                                 JNIEnv *               env);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JNI_H) */

#ifdef JRI_H
#if CALL_NOT_IN_CARBON
EXTERN_API_C( jref )
JMGetAppletObject               (JMAppletViewerRef      viewer);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* defined(JRI_H) */

/*
 * Tell MRJ to add connID to its list of shared libraries used when searching for
 * JNI (and JRI) native methods. This is useful for overriding or redirecting
 * a java call to System.loadLibrary().  In particular System.loadLibrary()
 * does not reuse CFM connections to libraries already open by someone outside
 * of the java session.  It always forces its own private copy of a shared library 
 * to be opened.  This can result in multiple instances of the data/TOC section
 * of a shared library.  
 * Note: This function has no effect on JDirect based native methods.
 * If connID exports a function named "JNI_OnLoad", it is immediately called.
 * If javaShouldClose is true, MRJ will close the connection when the session is closed.
 * Returns false and does nothing if a library with that name is already registered.
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( Boolean )
JMRegisterLibrary               (JMSessionRef           session,
                                 ConstStr63Param        libraryName,
                                 CFragConnectionID      connID,
                                 Boolean                javaShouldClose);



#endif  /* CALL_NOT_IN_CARBON */


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __JMANAGER__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__JMANAGER__RESTORE_PACKED_ENUMS)
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

#endif /* __JMANAGER__ */

