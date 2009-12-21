/*
     File:       CarbonEvents.h
 
     Contains:   Carbon Event Manager
 
     Version:    Technology: Version 1.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CARBONEVENTS__
#define __CARBONEVENTS__

#ifndef __EVENTS__
#include <Events.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __CONTROLS__
#include <Controls.h>
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

/*======================================================================================*/
/*  EVENT COMMON                                                                        */
/*======================================================================================*/


enum {
    eventAlreadyPostedErr       = -9860,
    eventClassInvalidErr        = -9862,
    eventClassAlreadyExistsErr  = -9863,
    eventClassIncorrectErr      = -9864,
    eventClassInvalidVersionErr = -9865,
    eventHandlerAlreadyInstalledErr = -9866,
    eventCantInstallHandlerErr  = -9867,
    eventInternalErr            = -9868,
    eventKindIncorrectErr       = -9869,
    eventDataTagNotSupportedErr = -9870,
    eventDataSizeMismatchErr    = -9871
};

/*======================================================================================*/
/*  EVENT CORE                                                                          */
/*======================================================================================*/
/*--------------------------------------------------------------------------------------*/
/*  o Event Flags, options                                                              */
/*--------------------------------------------------------------------------------------*/

typedef SInt16 EventPriority;
enum {
    kEventPriorityLow           = 0,
    kEventPriorityStandard      = 1,
    kEventPriorityHigh          = 2
};


typedef UInt32 EventAttributes;
enum {
    kEventAttributeNone         = 0,
    kEventAttributeUserEvent    = (1 << 0),
    kEventAttributeRouteToUserFocus = (1 << 1)
};

enum {
    kEventLeaveInQueue          = false,
    kEventRemoveFromQueue       = true
};

/*--------------------------------------------------------------------------------------*/
/* o Event Times                                                                        */
/*                                                                                      */
/* EventTime is in seconds since boot. Use the constants to make life easy.             */
/*--------------------------------------------------------------------------------------*/
typedef double                          EventTime;
typedef EventTime                       EventTimeout;
typedef EventTime                       EventTimerInterval;

#define kEventDurationSecond            1.0
#define kEventDurationMillisecond       kEventDurationSecond / 1000
#define kEventDurationMicrosecond       kEventDurationSecond / 1000000
#define kEventDurationNanosecond        kEventDurationSecond / 1000000000
#define kEventDurationMinute            kEventDurationSecond * 60
#define kEventDurationHour              kEventDurationMinute * 60
#define kEventDurationDay               kEventDurationHour * 24
#define kEventDurationNoWait            0.0
#define kEventDurationForever           -1.0

/* Helpful doodads to convert from and to ticks from event times*/
#ifdef __cplusplus
    inline EventTime TicksToEventTime( UInt32 t ) { return ( (t) / 60.0 ); }
    inline UInt32 EventTimeToTicks( EventTime t ) { return (UInt32)( (t) * 60 ); }
#else
    #define TicksToEventTime( t )   (EventTime)( (t) / 60.0 )
    #define EventTimeToTicks( t )   (UInt32)( (t) * 60 )
#endif  /* defined(__cplusplus) */


/*--------------------------------------------------------------------------------------*/
/* EventTypeSpec structure                                                              */
/*                                                                                      */
/* This structure is used in many routines to pass a list of event types to a function. */
/* You typically would declare a const array of these types to pass in.                 */
/*--------------------------------------------------------------------------------------*/

struct EventTypeSpec {
    UInt32                          eventClass;
    UInt32                          eventKind;
};
typedef struct EventTypeSpec            EventTypeSpec;

/* MouseTrackingResults returned from TrackMouse*/


typedef UInt16 MouseTrackingResult;
enum {
    kMouseTrackingMousePressed  = 1,
    kMouseTrackingMouseReleased = 2,
    kMouseTrackingMouseExited   = 3,
    kMouseTrackingMouseEntered  = 4,
    kMouseTrackingMouseMoved    = 5
};

typedef struct OpaqueEventRef*          EventRef;

typedef UInt32                          EventClassID;
typedef UInt32                          EventClass;
typedef UInt32                          EventType;
/*--------------------------------------------------------------------------------------*/
/*    o EventLoop                                                 */
/*--------------------------------------------------------------------------------------*/
typedef struct OpaqueEventLoopRef*      EventLoopRef;
EXTERN_API( EventLoopRef )
GetCurrentEventLoop             (void);

EXTERN_API( EventLoopRef )
GetMainEventLoop                (void);

/*--------------------------------------------------------------------------------------*/
/*  o Low-level event blocking                                                          */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( void )
BlockEventLoop                  (EventTimeout           timeout);

EXTERN_API( void )
WakeEventLoop                   (void);

EXTERN_API( Boolean )
BlockUntilNextEvent             (EventTimeout           timeout,
                                 Boolean                pullEvent,
                                 EventRef *             outEvent);

EXTERN_API( Boolean )
BlockUntilNextEventMatchingList (UInt32                 numTypes,
                                 const EventTypeSpec *  list,
                                 EventTimeout           timeout,
                                 Boolean                pullEvent,
                                 EventRef *             outEvent);

/*--------------------------------------------------------------------------------------*/
/*  o Core event lifetime APIs                                                          */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( EventRef )
CopyEvent                       (EventRef               inOther);

EXTERN_API( EventRef )
RetainEvent                     (EventRef               inEvent);

EXTERN_API( UInt32 )
GetEventRetainCount             (EventRef               inEvent);

EXTERN_API( void )
ReleaseEvent                    (EventRef               inEvent);

/*--------------------------------------------------------------------------------------*/
/*  o Getters for 'base-class' event info                                               */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( UInt32 )
GetEventClass                   (EventRef               inEvent);

EXTERN_API( UInt32 )
GetEventKind                    (EventRef               inEvent);

EXTERN_API( EventAttributes )
GetEventAttributes              (EventRef               inEvent);

EXTERN_API( EventTime )
GetEventTime                    (EventRef               inEvent);

EXTERN_API( UInt32 )
GetEventPriority                (EventRef               inEvent);

/*--------------------------------------------------------------------------------------*/
/*  o Setters for 'base-class' event info                                               */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( OSStatus )
SetEventTime                    (EventRef               inEvent,
                                 EventTime              inTime);


/*--------------------------------------------------------------------------------------*/
/*  o Getting/setting common event data via tags                                        */
/*                                                                                      */
/*  NOTE: THIS STUFF IS NOT YET ACTIVE. DO NOT ATTEMPT TO USE IT.                       */
/*--------------------------------------------------------------------------------------*/
enum {
    kEventTagKeyModifiers       = FOUR_CHAR_CODE('mods'),       /* UInt32*/
    kEventTagMouseLocation      = FOUR_CHAR_CODE('mloc')        /* Point*/
};

typedef UInt32                          EventDataTag;
EXTERN_API( OSStatus )
GetEventDataByTag               (EventRef               inEvent,
                                 EventDataTag           inKey,
                                 void *                 buffer,
                                 Size                   bufferSize,
                                 Size *                 actualSize) /* can be NULL */;

EXTERN_API( OSStatus )
SetEventDataByTag               (EventRef               inEvent,
                                 EventDataTag           inKey,
                                 void *                 buffer,
                                 Size                   bufferSize);

/* Common, type-safe wrappers for the above...*/

EXTERN_API( OSStatus )
GetEventKeyModifiers            (EventRef               inEvent,
                                 UInt32 *               outModifiers);

EXTERN_API( OSStatus )
SetEventKeyModifiers            (EventRef               inEvent,
                                 UInt32                 inModifiers);

EXTERN_API( OSStatus )
GetEventMouseLocation           (EventRef               inEvent,
                                 Point *                outMouseLoc);

EXTERN_API( OSStatus )
SetEventMouseLocation           (EventRef               inEvent,
                                 Point                  inMouseLoc);

/*--------------------------------------------------------------------------------------*/
/*  o Event Queue routines (posting, finding, flushing)                                 */
/*--------------------------------------------------------------------------------------*/

typedef CALLBACK_API( Boolean , EventComparatorProcPtr )(EventRef inEvent, void *inCompareData);
typedef STACK_UPP_TYPE(EventComparatorProcPtr)                  EventComparatorUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(EventComparatorUPP)
    NewEventComparatorUPP          (EventComparatorProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeEventComparatorUPP      (EventComparatorUPP      userUPP);

    EXTERN_API(Boolean)
    InvokeEventComparatorUPP       (EventRef                inEvent,
                                    void *                  inCompareData,
                                    EventComparatorUPP      userUPP);

#else
    enum { uppEventComparatorProcInfo = 0x000003D0 };               /* pascal 1_byte Func(4_bytes, 4_bytes) */
    #define NewEventComparatorUPP(userRoutine)                      (EventComparatorUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppEventComparatorProcInfo, GetCurrentArchitecture())
    #define DisposeEventComparatorUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define InvokeEventComparatorUPP(inEvent, inCompareData, userUPP)  (Boolean)CALL_TWO_PARAMETER_UPP((userUPP), uppEventComparatorProcInfo, (inEvent), (inCompareData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewEventComparatorProc(userRoutine)                     NewEventComparatorUPP(userRoutine)
#define CallEventComparatorProc(userRoutine, inEvent, inCompareData) InvokeEventComparatorUPP(inEvent, inCompareData, userRoutine)
/*
   PostEventToQueue retains the given event. You would normally release it after calling
   this function.
*/
EXTERN_API( OSStatus )
PostEventToQueue                (EventRef               inEvent,
                                 EventPriority          inPriority);

EXTERN_API( Boolean )
IsEventInQueue                  (EventRef               inEvent);

EXTERN_API( OSStatus )
FlushEventsMatchingListFromQueue (UInt32                inNumTypes,
                                 const EventTypeSpec *  inList);

EXTERN_API( OSStatus )
FlushSpecificEventsFromQueue    (EventComparatorUPP     comparator,
                                 void *                 compareData);

EXTERN_API( void )
FlushEventQueue                 (void);

/*
   Remove EventFromQueue removes the event from the event queue and releases it. If you
   want to keep the event around, you should make sure to copy or retain it.
*/

EXTERN_API( void )
RemoveEventFromQueue            (EventRef               inEvent);

EXTERN_API( EventRef )
FindSpecificEventInQueue        (EventComparatorUPP     comparator,
                                 void *                 compareData);

EXTERN_API( UInt32 )
GetNumEventsInQueue             (void);

/*--------------------------------------------------------------------------------------*/
/*  o Helpful utilities                                                                 */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( EventTime )
GetCurrentEventTime             (void);

EXTERN_API( Boolean )
IsUserCancelEventRef            (EventRef               event);

EXTERN_API( Boolean )
CheckEventQueueForUserCancel    (void);

EXTERN_API( OSStatus )
TrackMouseLocation              (GrafPtr                port, /* can be NULL */
                                 Point *                outPt,
                                 MouseTrackingResult *  result);

EXTERN_API( OSStatus )
TrackMouseRegion                (GrafPtr                port, /* can be NULL */
                                 RgnHandle              region,
                                 Boolean *              wasInRgn,
                                 MouseTrackingResult *  result);

EXTERN_API( Boolean )
ConvertEventRefToEventRecord    (EventRef               inEvent,
                                 EventRecord *          outEvent);

EXTERN_API( Boolean )
IsEventInMask                   (EventRef               inEvent,
                                 EventMask              inMask);

EXTERN_API( EventTime )
GetLastUserEventTime            (void);

/*--------------------------------------------------------------------------------------*/
/*  o Timers                                                                            */
/*--------------------------------------------------------------------------------------*/
/* Timers, the new null processing mechanism.*/
/* Not just good, but good for ya. */

typedef struct OpaqueEventLoopTimerRef*  EventLoopTimerRef;
typedef CALLBACK_API( void , EventLoopTimerProcPtr )(EventLoopTimerRef inTimer, void *inUserData);
typedef STACK_UPP_TYPE(EventLoopTimerProcPtr)                   EventLoopTimerUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(EventLoopTimerUPP)
    NewEventLoopTimerUPP           (EventLoopTimerProcPtr   userRoutine);

    EXTERN_API(void)
    DisposeEventLoopTimerUPP       (EventLoopTimerUPP       userUPP);

    EXTERN_API(void)
    InvokeEventLoopTimerUPP        (EventLoopTimerRef       inTimer,
                                    void *                  inUserData,
                                    EventLoopTimerUPP       userUPP);

#else
    enum { uppEventLoopTimerProcInfo = 0x000003C0 };                /* pascal no_return_value Func(4_bytes, 4_bytes) */
    #define NewEventLoopTimerUPP(userRoutine)                       (EventLoopTimerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppEventLoopTimerProcInfo, GetCurrentArchitecture())
    #define DisposeEventLoopTimerUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define InvokeEventLoopTimerUPP(inTimer, inUserData, userUPP)   CALL_TWO_PARAMETER_UPP((userUPP), uppEventLoopTimerProcInfo, (inTimer), (inUserData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewEventLoopTimerProc(userRoutine)                      NewEventLoopTimerUPP(userRoutine)
#define CallEventLoopTimerProc(userRoutine, inTimer, inUserData) InvokeEventLoopTimerUPP(inTimer, inUserData, userRoutine)
EXTERN_API( OSStatus )
InstallEventLoopTimer           (EventLoopRef           inEventLoop,
                                 EventTimerInterval     inFirstFire,
                                 EventTimerInterval     inInterval,
                                 EventLoopTimerUPP      inTimerProc,
                                 void *                 inUserData,
                                 EventLoopTimerRef *    outRef);

EXTERN_API( OSStatus )
RemoveEventLoopTimer            (EventLoopTimerRef      inTimer);

EXTERN_API( OSStatus )
SetEventLoopTimerNextFireTime   (EventLoopTimerRef      inTimer,
                                 EventTimerInterval     nextFire);


/*======================================================================================*/
/*  EVENT CLASSES                                                                       */
/*======================================================================================*/
enum {
    kEventClassMouse            = FOUR_CHAR_CODE('mous'),
    kEventClassKeyboard         = FOUR_CHAR_CODE('keyb'),
    kEventClassTextInput        = FOUR_CHAR_CODE('text'),
    kEventClassApplication      = FOUR_CHAR_CODE('appl'),
    kEventClassEPPC             = FOUR_CHAR_CODE('eppc'),
    kEventClassMenu             = FOUR_CHAR_CODE('menu'),
    kEventClassWindow           = FOUR_CHAR_CODE('wind'),
    kEventClassControl          = FOUR_CHAR_CODE('cntl'),
    kEventClassCommand          = FOUR_CHAR_CODE('cmds')
};

/* Mouse Events */
enum {
    kEventLeftMouseDown         = 1,
    kEventLeftMouseUp           = 2,
    kEventRightMouseDown        = 3,
    kEventRightMouseUp          = 4,
    kEventMouseMoved            = 5,
    kEventLeftMouseDragged      = 6,                            /* mouse moved while button down*/
    kEventRightMouseDragged     = 7,
    kEventMouseEntered          = 8,
    kEventMouseExited           = 9,
    kEventMouseWheelMoved       = 10                            /* placeholder. Not yet operational, unlike the new Death Star.*/
};

enum {
    kEventMouseButtonPrimary    = 0,                            /* only button for a one-button mouse (usually left button for multi-button mouse)*/
    kEventMouseButtonSecondary  = 1,                            /* usually right button for a multi-button mouse*/
    kEventMouseButtonTertiary   = 2                             /* usually middle button for a three-button mouse*/
};

typedef UInt16                          EventMouseButton;
enum {
    kEventMouseWheelDirectionUp = 0,
    kEventMouseWheelDirectionDown = 1,
    kEventMouseWheelDirectionLeft = 2,
    kEventMouseWheelDirectionRight = 3
};

typedef UInt16                          EventMouseWheelDirection;
/* Keyboard Events */
enum {
    kEventRawKeyDown            = 1,                            /* hardware-level events*/
    kEventRawKeyRepeat          = 2,
    kEventRawKeyUp              = 3,
    kEventRawKeyModifiersChanged = 4
};

enum {
    kEventTextInput             = 5                             /* high-level keyboard event (not available yet)*/
};

/* Application Events */
enum {
    kEventAppActivated          = 1,                            /* resume, in old parlance*/
    kEventAppDeactivated        = 2,                            /* suspend, in old parlance*/
    kEventAppQuit               = 3,                            /* this app is quitting.*/
    kEventAppLaunchNotification = 4                             /* response to async application launch.*/
};

/* Apple Events */
enum {
    kEventHighLevelEvent        = 1
};


/*
    Window Refresh Events
    
    o kEventWindowUpdate
        low-level update event.  You must call BeginUpdate, call SetPort,
        draw your window content, and then call EndUpdate.
    
    o kEventWindowDrawContent
        sent when the standard window handler sees an update event. 
        BeginUpdate, SetPort, and EndUpdate are called for you; all you do
        is draw.
    
    o kEventWindowDrawStructure
        sent after the window manager has drawn the window frame.  The
        current port is a global port clipped to the window structure
        region, not the window's port.
        
    o kEventWindowEraseContent
        called when the window manager wishes to erase the window
        background (In ShowWindow, for example).  The current port is set
        to the window's port.
        
    o kEventWindowActivate
        window is active now
        
    o kEventWindowDeactivate
        window is inactive now
*/
enum {
    kEventWindowUpdate          = 1,
    kEventWindowDrawContent     = 2,
    kEventWindowDrawStructure   = 3,
    kEventWindowEraseContent    = 4,
    kEventWindowActivate        = 5,
    kEventWindowDeactivate      = 6
};

/*
    Window State Change Events
    
    o kEventWindowZOrderChanged
        Sent after the window has changed position relative
        to other windows in the application window list.
    
    o kEventWindowActivationChanged
        Sent after the window has changed activation state
    
    o kEventWindowOriginChange
      kEventWindowSizeChange
        Sent during ResizeWindow/DragWindow, before the window is
        actually modified.  Alter the current bounds in the event to
        change the intended location of the window.  "Origin"
        indicates that the window's place location onscreen is
        different.  "Size" indicates that the window's size has
        changed (the origin may also be different).
    
    o kEventWindowOriginChanged
      kEventWindowSizeChanged
        Sent after the window has been resized or moved.
*/

enum {
    kEventWindowZOrderChanged   = 16,
    kEventWindowActivationChanged = 18,
    kEventWindowOriginChange    = 20,
    kEventWindowOriginChanged   = 21,
    kEventWindowSizeChange      = 22,
    kEventWindowSizeChanged     = 23
};

/*
    Window Click Events
    
    Low-level events which generate higher-level "action" events.
    Most clients should allow the standard window handler to implement
    these events.
    
    o kEventWindowClickDragRgn
        sent when the mouse is down in the drag region.  Calls DragWindow.
    
    o kEventWindowClickResizeRgn
        sent when the mouse is down in the resize area.  Calls ResizeWindow.
    
    o kEventWindowClickCollapseRgn
        sent when the mouse is down in the collapse widget.  Generates
        kEventWindowExpand or kEventWindowCollapse (whichever is the opposite
        of the window's current collapse state).
    
    o kEventWindowClickCloseRgn
        sent when the mouse is down in the close widget.  Generates
        kEventWindowClose.
    
    o kEventWindowClickZoomRgn
        sent when the mouse is down in the zoom widget.  Generates
        kEventWindowZoom.   
        
    o kEventWindowClickContentRgn
        sent when the mouse is down in the content region.
    
    o kEventWindowClickProxyIconRgn
        sent when the mouse is down in the proxy icon.  Generates proxy
        icon events.
*/
enum {
    kEventWindowClickDragRgn    = 32,
    kEventWindowClickResizeRgn  = 33,
    kEventWindowClickCollapseRgn = 34,
    kEventWindowClickCloseRgn   = 35,
    kEventWindowClickZoomRgn    = 36,
    kEventWindowClickContentRgn = 37,
    kEventWindowClickProxyIconRgn = 38
};

/*
    Window Action Events
    
    o kEventWindowCollapse
        If the window is not collapsed, this event is sent by the standard
        window handler after it has received kEventWindowClickCollapseRgn
        and received true from a call to TrackBox.  Standard window handler
        calls CollapseWindow and then sends kEventWindowCollapsed if no
        error is received from CollapseWindow.
    
    o kEventWindowCollapsed
        Notification that the object has successfully collapsed.
    
    o kEventWindowCollapseAll
        Sent by the standard window handler (when the option key is down)
        after it has received kEventWindowClickCollapseRgn and then
        received true from a call to TrackBox.  The standard window
        handler's response is to send each window of the same class as the
        clicked window a kEventWindowCollapse event.
    
    o kEventWindowExpand
        If the window is collapsed, this event is sent by the standard
        window handler after it has received kEventWindowClickCollapseRgn
        and received true from a call to TrackBox.  The standard window
        handler's response is to call CollapseWindow, then send
        kEventWindowExpanded.
    
    o kEventWindowExpanded
        Sent by the standard window handler (when the option key is down)
        after it has received kEventWindowClickCollapseRgn and then
        received true from a call to TrackBox.  The standard window
        handler's response is to send each window of the same class as the
        clicked window a kEventWindowExpand event.
    
    o kEventWindowExpandAll
        Notification that the window has successfully expanded.
    
    o kEventWindowClose
        Sent by the standard window handler after it has received
        kEventWindowClickCloseRgn and successfully called TrackBox.
    
    o kEventWindowClosed
        Dispatched by DisposeWindow before the object is disposed.
    
    o kEventWindowCloseAll
        Sent by the standard window handler (when the option key is down)
        after it has received kEventWindowClickCloseRgn and received true
        from a call to TrackGoAway.  The standard window handler's response
        is to send each window with the same class as the clicked window a
        kEventWindowClose event.
    
    o kEventWindowZoom
        Sent by the standard window handler upon receiving
        kEventWindowClickZoomRgn and then receiving true from a call to
        TrackBox.  The standard window handler's response is to zoom the
        window using the sample code taken from the 8.5 Window Manager
        documentation. Upon successful zoom, kEventWindowZoomed is sent.
    
    o kEventWindowZoomed
        Notification that object has been successfully zoomed.
    
    o kEventWindowZoomAll
        Sent by the standard window handler (when the option key is down)
        after it has received kEventObjectClickZoomRgn and received true
        from a call to TrackBox.  The standard window handler's response is
        to send each window with the same class as the clicked window a
        kEventObjectZoom event and then to reposition all zoomed windows
        using the kWindowCascadeOnParentWindowScreen positioning method.
    
    o kEventWindowContextualMenuSelect
        Sent when either the right mouse button is pressed, or the control
        key is held down and the left mouse button is pressed, or the left
        mouse button is held down for more than 1/4th of a second (and
        nothing else is handling the generated mouse tracking events).  
        The standard window handler ignores this event.
    
    o kEventWindowPathSelect
        Sent when IsWindowPathSelectClick would return true.  Set the
        MenuRef in the event if you wish to customize the menu passed to
        WindowPathSelect.
    
    o kEventWindowGetIdealSize
        Sent by the standard window handler to determine the standard state
        for zooming.
    o kEventWindowGetMinimumSize
        Sent by the standard window handler to determine the minimum size
        of the window (used during window resizing).
    
    o kEventWindowProxyBeginDrag
        Sent before a proxy icon drag; you can attach data to the DragRef
        in the event.
    
    o kEventWindowProxyEndDrag
        Sent after the proxy icon drag is complete, whether successful or
        not.
*/
enum {
    kEventWindowCollapse        = 66,
    kEventWindowCollapsed       = 67,
    kEventWindowCollapseAll     = 68,
    kEventWindowExpand          = 69,
    kEventWindowExpanded        = 70,
    kEventWindowExpandAll       = 71,
    kEventWindowClose           = 72,
    kEventWindowClosed          = 73,
    kEventWindowCloseAll        = 74,
    kEventWindowZoom            = 75,
    kEventWindowZoomed          = 76,
    kEventWindowZoomAll         = 77,
    kEventWindowContextualMenuSelect = 78,
    kEventWindowPathSelect      = 79,
    kEventWindowGetIdealSize    = 80,
    kEventWindowGetMinimumSize  = 81,
    kEventWindowProxyBeginDrag  = 128,
    kEventWindowProxyEndDrag    = 129
};

/* Menu Events */
enum {
    kEventMenuBeginTracking     = 1,
    kEventMenuEndTracking       = 2,
    kEventMenuChangeTrackingMode = 3,
    kEventMenuOpening           = 4,
    kEventMenuClosed            = 5,
    kEventMenuTargetItem        = 6,
    kEventMenuMatchKey          = 7
};

/* Command-related events */
enum {
    kEventProcessCommand        = 1
};


/* Control Events */
enum {
    kEventControlHit            = 1,
    kEventControlSimulateHit    = 2
};

/* HI Commands */
enum {
    kHICommandCancel            = FOUR_CHAR_CODE('not!'),       /* the most important command: cancel the current operation!  sent when the user presses command-period*/
    kHICommandQuit              = FOUR_CHAR_CODE('quit'),
    kHICommandUndo              = FOUR_CHAR_CODE('undo'),
    kHICommandRedo              = FOUR_CHAR_CODE('redo'),
    kHICommandCut               = FOUR_CHAR_CODE('cut '),
    kHICommandCopy              = FOUR_CHAR_CODE('copy'),
    kHICommandPaste             = FOUR_CHAR_CODE('past'),
    kHICommandClear             = FOUR_CHAR_CODE('clea'),
    kHICommandSelectAll         = FOUR_CHAR_CODE('sall')
};

enum {
    kHICommandFromMenu          = (1L << 0)
};


struct HICommand {
    UInt32                          attributes;
    UInt32                          commandID;
    struct {
        MenuRef                         menuRef;
        UInt16                          menuItemIndex;
    }                                 menu;
};
typedef struct HICommand                HICommand;
/*--------------------------------------------------------------------------------------*/
/* Mouse Events                                                                         */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( OSStatus )
CreateMouseEvent                (UInt32                 kind,
                                 EventTime              when,
                                 Point                  where,
                                 UInt32                 modifiers,
                                 EventRef *             outEvent);

EXTERN_API( OSStatus )
SetMouseEventMouseLocation      (EventRef               inEvent,
                                 Point                  inPoint);

EXTERN_API( OSStatus )
SetMouseEventKeyModifiers       (EventRef               inEvent,
                                 UInt32                 inModifiers);

EXTERN_API( OSStatus )
SetMouseEventClickCount         (EventRef               inEvent,
                                 UInt32                 inCount);

EXTERN_API( OSStatus )
GetMouseEventMouseLocation      (EventRef               inEvent,
                                 Point *                outPoint);

EXTERN_API( OSStatus )
GetMouseEventKeyModifiers       (EventRef               inEvent,
                                 UInt32 *               outModifiers);

EXTERN_API( OSStatus )
GetMouseEventClickCount         (EventRef               inEvent,
                                 UInt32 *               outCount);


/*--------------------------------------------------------------------------------------*/
/* Keyboard Events                                                                      */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( OSStatus )
CreateKeyboardEvent             (UInt32                 kind,
                                 EventTime              when,
                                 UInt32                 inKeyChar,
                                 UInt32                 inKeyCode,
                                 UInt32                 inKeyModifiers,
                                 EventRef *             outEvent);

EXTERN_API( OSStatus )
SetKeyboardEventKeyCode         (EventRef               inEvent,
                                 UInt32                 inKeyCode);

EXTERN_API( OSStatus )
SetKeyboardEventKeyChar         (EventRef               inEvent,
                                 UInt32                 inKeyChar);

EXTERN_API( OSStatus )
SetKeyboardEventKeyModifiers    (EventRef               inEvent,
                                 UInt32                 inModifiers);

EXTERN_API( OSStatus )
GetKeyboardEventKeyCode         (EventRef               inEvent,
                                 UInt32 *               outKeyCode);

EXTERN_API( OSStatus )
GetKeyboardEventKeyChar         (EventRef               inEvent,
                                 UInt32 *               outKeyChar);

EXTERN_API( OSStatus )
GetKeyboardEventKeyModifiers    (EventRef               inEvent,
                                 UInt32 *               outModifiers);

/*--------------------------------------------------------------------------------------*/
/* Window Events                                                                        */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( OSStatus )
CreateWindowEvent               (UInt32                 kind,
                                 EventTime              when,
                                 WindowRef              window,
                                 EventRef *             outEvent);

EXTERN_API( OSStatus )
SetWindowEventWindowRef         (EventRef               inEvent,
                                 WindowRef              inWindow);

EXTERN_API( OSStatus )
SetWindowEventKeyModifiers      (EventRef               inEvent,
                                 UInt32                 inModifiers);

EXTERN_API( OSStatus )
SetWindowEventMouseLocation     (EventRef               inEvent,
                                 Point                  inWhere);

EXTERN_API( OSStatus )
SetWindowEventWindowSize        (EventRef               inEvent,
                                 Point                  inSize);

EXTERN_API( OSStatus )
GetWindowEventWindowRef         (EventRef               inEvent,
                                 WindowRef *            outWindow);

EXTERN_API( OSStatus )
GetWindowEventKeyModifiers      (EventRef               inEvent,
                                 UInt32 *               outModifiers);

EXTERN_API( OSStatus )
GetWindowEventMouseLocation     (EventRef               inEvent,
                                 Point *                outWhere);

EXTERN_API( OSStatus )
GetWindowEventWindowSize        (EventRef               inEvent,
                                 Point *                outSize);

/* window proxy events contain dragrefs*/
EXTERN_API( OSStatus )
GetWindowEventDragRef           (EventRef               eventRef,
                                 DragRef *              outDrag);

EXTERN_API( OSStatus )
SetWindowEventDragRef           (EventRef               eventRef,
                                 DragRef                inDrag);

EXTERN_API( OSStatus )
GetWindowEventMenuRef           (EventRef               eventRef,
                                 MenuRef *              outMenu);

EXTERN_API( OSStatus )
SetWindowEventMenuRef           (EventRef               eventRef,
                                 MenuRef                inMenu);

/* window bounds-changed events*/
EXTERN_API( OSStatus )
GetWindowEventOriginalBounds    (EventRef               eventRef,
                                 Rect *                 outBounds);

EXTERN_API( OSStatus )
SetWindowEventOriginalBounds    (EventRef               eventRef,
                                 const Rect *           inBounds);

EXTERN_API( OSStatus )
GetWindowEventPreviousBounds    (EventRef               eventRef,
                                 Rect *                 outBounds);

EXTERN_API( OSStatus )
SetWindowEventPreviousBounds    (EventRef               eventRef,
                                 const Rect *           inBounds);

EXTERN_API( OSStatus )
GetWindowEventCurrentBounds     (EventRef               eventRef,
                                 Rect *                 outBounds);

EXTERN_API( OSStatus )
SetWindowEventCurrentBounds     (EventRef               eventRef,
                                 const Rect *           inBounds);

EXTERN_API( OSStatus )
GetWindowEventGrafPort          (EventRef               eventRef,
                                 GrafPtr *              outPort);

EXTERN_API( OSStatus )
SetWindowEventGrafPort          (EventRef               eventRef,
                                 GrafPtr                inPort);

/* window click events each contain a reference to the original mouse down event*/
EXTERN_API( OSStatus )
GetWindowEventMouseEvent        (EventRef               eventRef,
                                 EventRef *             outEvent);

EXTERN_API( OSStatus )
SetWindowEventMouseEvent        (EventRef               eventRef,
                                 EventRef               inEvent);


/*--------------------------------------------------------------------------------------*/
/* Control Events                                                                       */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( OSStatus )
CreateControlEvent              (UInt32                 kind,
                                 EventTime              when,
                                 ControlRef             control,
                                 EventRef *             outEvent);

EXTERN_API( OSStatus )
SetControlEventControlRef       (EventRef               inEvent,
                                 ControlRef             inControl);

EXTERN_API( OSStatus )
SetControlEventControlPartCode  (EventRef               inEvent,
                                 ControlPartCode        inPart);

EXTERN_API( OSStatus )
SetControlEventKeyModifiers     (EventRef               inEvent,
                                 UInt32                 inModifiers);

EXTERN_API( OSStatus )
GetControlEventControlRef       (EventRef               inEvent,
                                 ControlRef *           outControl);

EXTERN_API( OSStatus )
GetControlEventControlPartCode  (EventRef               inEvent,
                                 ControlPartCode *      outPart);

EXTERN_API( OSStatus )
GetControlEventKeyModifiers     (EventRef               inEvent,
                                 UInt32 *               outModifiers);

/*--------------------------------------------------------------------------------------*/
/* Menu Events                                                                          */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( OSStatus )
CreateMenuEvent                 (UInt32                 kind,
                                 EventTime              when,
                                 MenuRef                menu,
                                 EventRef *             outEvent);

EXTERN_API( OSStatus )
SetMenuEventMenuRef             (EventRef               inEvent,
                                 MenuRef                inMenu);

EXTERN_API( OSStatus )
SetMenuEventTrackingMode        (EventRef               inEvent,
                                 MenuTrackingMode       inMode,
                                 Boolean                setCurrent);

EXTERN_API( OSStatus )
SetMenuEventFirstTimeOpen       (EventRef               inEvent,
                                 Boolean                inFirstTimeOpen);

EXTERN_API( OSStatus )
SetMenuEventItemIndex           (EventRef               inEvent,
                                 MenuItemIndex          inItemIndex);

EXTERN_API( OSStatus )
SetMenuEventCommandID           (EventRef               inEvent,
                                 MenuCommand            inCommand);

EXTERN_API( OSStatus )
SetMenuEventKeyEvent            (EventRef               inEvent,
                                 EventRef               inKeyEvent);

EXTERN_API( OSStatus )
SetMenuEventHICommand           (EventRef               inEvent,
                                 const HICommand *      inCommand);

EXTERN_API( OSStatus )
GetMenuEventMenuRef             (EventRef               inEvent,
                                 MenuRef *              outMenu);

EXTERN_API( OSStatus )
GetMenuEventTrackingMode        (EventRef               inEvent,
                                 Boolean                getCurrent,
                                 MenuTrackingMode *     outMode);

EXTERN_API( OSStatus )
GetMenuEventFirstTimeOpen       (EventRef               inEvent,
                                 Boolean *              outFirstTimeOpen);

EXTERN_API( OSStatus )
GetMenuEventItemIndex           (EventRef               inEvent,
                                 MenuItemIndex *        outItemIndex);

EXTERN_API( OSStatus )
GetMenuEventCommandID           (EventRef               inEvent,
                                 MenuCommand *          outCommand);

EXTERN_API( OSStatus )
GetMenuEventKeyEvent            (EventRef               inEvent,
                                 EventRef *             outKeyEvent);

EXTERN_API( OSStatus )
GetMenuEventHICommand           (EventRef               inEvent,
                                 HICommand *            outCommand);

/*--------------------------------------------------------------------------------------*/
/* Command Events                                                                       */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( OSStatus )
CreateCommandEvent              (UInt32                 kind,
                                 EventTime              when,
                                 const HICommand *      inCommand,
                                 EventRef *             outEvent);

EXTERN_API( OSStatus )
GetCommandEventCommand          (EventRef               inEvent,
                                 HICommand *            outCommand);

/*--------------------------------------------------------------------------------------*/
/* Miscellany                                                                           */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( OSStatus )
CreateApplicationEvent          (UInt32                 kind,
                                 EventTime              when,
                                 EventRef *             outEvent);


/*
   This routine is used to get information about an async application launch
   It is only valid on Mac OS X.
*/

EXTERN_API( OSStatus )
GetApplicationEventLaunchData   (EventRef               event,
                                 OSStatus *             outLaunchErr,
                                 ProcessSerialNumber *  outProcess,
                                 void **                outLaunchRefCon);

/*======================================================================================*/
/*  EVENT HANDLERS                                                                      */
/*======================================================================================*/

typedef UInt32 EventHandlerResult;
enum {
    kEventHandlerHandled        = 0,
    kEventHandlerNotHandled     = 1,
    kEventHandlerPassUpChain    = 2,
    kEventHandlerStopProcessing = 3
};


enum {
    kEventHandlerStandard       = -1L
};




typedef struct OpaqueEventHandlerRef*   EventHandlerRef;

/*--------------------------------------------------------------------------------------*/
/*  o EventHandler specification                                                        */
/*--------------------------------------------------------------------------------------*/

typedef CALLBACK_API( EventHandlerResult , EventHandlerProcPtr )(EventHandlerRef inHandlerRef, EventRef inEvent, void *userData);
typedef STACK_UPP_TYPE(EventHandlerProcPtr)                     EventHandlerUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(EventHandlerUPP)
    NewEventHandlerUPP             (EventHandlerProcPtr     userRoutine);

    EXTERN_API(void)
    DisposeEventHandlerUPP         (EventHandlerUPP         userUPP);

    EXTERN_API(EventHandlerResult)
    InvokeEventHandlerUPP          (EventHandlerRef         inHandlerRef,
                                    EventRef                inEvent,
                                    void *                  userData,
                                    EventHandlerUPP         userUPP);

#else
    enum { uppEventHandlerProcInfo = 0x00000FF0 };                  /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewEventHandlerUPP(userRoutine)                         (EventHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppEventHandlerProcInfo, GetCurrentArchitecture())
    #define DisposeEventHandlerUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokeEventHandlerUPP(inHandlerRef, inEvent, userData, userUPP)  (EventHandlerResult)CALL_THREE_PARAMETER_UPP((userUPP), uppEventHandlerProcInfo, (inHandlerRef), (inEvent), (userData))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewEventHandlerProc(userRoutine)                        NewEventHandlerUPP(userRoutine)
#define CallEventHandlerProc(userRoutine, inHandlerRef, inEvent, userData) InvokeEventHandlerUPP(inHandlerRef, inEvent, userData, userRoutine)
/*--------------------------------------------------------------------------------------*/
/*  o Installing Event Handlers                                                         */
/*                                                                                      */
/* Use these routines to install event handlers for a specific toolbox object. You may  */
/* pass zero for inNumTypes and NULL for inList if you need to be in a situation where  */
/* you know you will be receiving events, but not exactly which ones at the time you    */
/* are installing the handler. Later, your application can call the Add/Remove routines */
/* listed below this section.                                                           */
/*                                                                                      */
/* You can only install a specific handler once. The combination of inHandler and       */
/* inUserData is considered the 'signature' of a handler. Any attempt to install a new  */
/* handler with the same proc and user data as an already-installed handler will result */
/* in eventHandlerAlreadyInstalledErr. Installing the same proc and user data on a      */
/* different object is legal.                                                           */
/*                                                                                      */
/* Upon successful completion of this routine, you are returned an EventHandlerRef,     */
/* which you can use in various other calls, and is passed to your event handler. You   */
/* use it to extract information about the handler, such as the target (window, etc.)   */
/* if you have the same handler installed for different objects and need to perform     */
/* actions on the current target (say, call a window manager function).                 */
/*--------------------------------------------------------------------------------------*/
EXTERN_API( OSStatus )
InstallApplicationEventHandler  (EventHandlerUPP        inHandler,
                                 UInt32                 inNumTypes,
                                 const EventTypeSpec *  inList,
                                 void *                 inUserData,
                                 EventHandlerRef *      outRef);

EXTERN_API( OSStatus )
InstallWindowEventHandler       (WindowRef              inWindowRef,
                                 EventHandlerUPP        inHandler,
                                 UInt32                 inNumTypes,
                                 const EventTypeSpec *  inList,
                                 void *                 inUserData,
                                 EventHandlerRef *      outRef);

EXTERN_API( OSStatus )
InstallControlEventHandler      (ControlRef             inControlRef,
                                 EventHandlerUPP        inHandler,
                                 UInt32                 inNumTypes,
                                 const EventTypeSpec *  inList,
                                 void *                 inUserData,
                                 EventHandlerRef *      outRef);

EXTERN_API( OSStatus )
InstallMenuEventHandler         (MenuRef                inMenuRef,
                                 EventHandlerUPP        inHandler,
                                 UInt32                 inNumTypes,
                                 const EventTypeSpec *  inList,
                                 void *                 inUserData,
                                 EventHandlerRef *      outRef);

EXTERN_API( OSStatus )
RemoveEventHandler              (EventHandlerRef        inHandlerRef);

/*--------------------------------------------------------------------------------------*/
/*  o Adjusting set of event types after a handler is created                           */
/*                                                                                      */
/* After installing a handler with the routine above, you can adjust the event type     */
/* list telling the toolbox what events to send to that handler by calling the two      */
/* routines below. If you add an event type twice for the same handler, your handler    */
/* will only be called once, but it will take two RemoveEventType calls to stop your    */
/* handler from being called with that event type. In other words, the install count    */
/* for each event type is maintained by the toolbox. This might allow you, for example  */
/* to have subclasses of a window object register for types without caring if the base  */
/* class has already registered for that type. When the subclass removes its types, it  */
/* can succesfully do so without affecting the base class' reception of its events      */
/* types, yielding eternal bliss.                                                       */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( OSStatus )
AddEventTypesToHandler          (EventHandlerRef        inHandlerRef,
                                 UInt32                 inNumTypes,
                                 const EventTypeSpec *  inList);

EXTERN_API( OSStatus )
RemoveEventTypesFromHandler     (EventHandlerRef        inHandlerRef,
                                 UInt32                 inNumTypes,
                                 const EventTypeSpec *  inList);

/*--------------------------------------------------------------------------------------*/
/*  o Explicit Propogation                                                              */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( EventHandlerResult )
CallNextEventHandler            (EventHandlerRef        inThisHandler,
                                 EventRef               inEvent);

/*--------------------------------------------------------------------------------------*/
/*  o Getting the target from a handler ref                                             */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( ControlRef )
GetEventHandlerTargetControl    (EventHandlerRef        inHandler);

EXTERN_API( WindowRef )
GetEventHandlerTargetWindow     (EventHandlerRef        inHandler);

EXTERN_API( MenuRef )
GetEventHandlerTargetMenu       (EventHandlerRef        inHandler);

/*--------------------------------------------------------------------------------------*/
/*  o Sending Events                                                                    */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( EventHandlerResult )
SendEventToApplication          (EventRef               inEvent);

EXTERN_API( EventHandlerResult )
SendEventToWindow               (EventRef               inEvent,
                                 WindowRef              inWindow);

EXTERN_API( EventHandlerResult )
SendEventToControl              (EventRef               inEvent,
                                 ControlRef             inControl);

EXTERN_API( EventHandlerResult )
SendEventToMenu                 (EventRef               inEvent,
                                 MenuRef                inMenu);

EXTERN_API( EventHandlerResult )
SendEventToUserFocus            (EventRef               inEvent);

/*--------------------------------------------------------------------------------------*/
/*  o Command Routines                                                                  */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( EventHandlerResult )
ProcessHICommand                (const HICommand *      inCommand);

/*--------------------------------------------------------------------------------------*/
/*  o Event Loop Routines                                                               */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( void )
RunApplicationEventLoop         (void);

EXTERN_API( void )
QuitApplicationEventLoop        (void);

/*--------------------------------------------------------------------------------------*/
/*  o User Focus                                                                        */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( void )
SetUserFocusWindow              (WindowRef              inWindow);

EXTERN_API( WindowRef )
GetUserFocusWindow              (void);


/*--------------------------------------------------------------------------------------*/
/*  o Default/Cancel buttons                                                            */
/*--------------------------------------------------------------------------------------*/

EXTERN_API( OSStatus )
SetWindowDefaultButton          (WindowRef              inWindow,
                                 ControlRef             inControl) /* can be NULL */;

EXTERN_API( OSStatus )
SetWindowCancelButton           (WindowRef              inWindow,
                                 ControlRef             inControl) /* can be NULL */;

EXTERN_API( OSStatus )
GetWindowDefaultButton          (WindowRef              inWindow,
                                 ControlRef *           outControl);

EXTERN_API( OSStatus )
GetWindowCancelButton           (WindowRef              inWindow,
                                 ControlRef *           outControl);


/*--------------------------------------------------------------------------------------*/
/*  o Mouse Tracking Areas                                                              */
/*                                                                                      */
/*  Mouse tracking areas control the generation of mouse-enter and                      */
/*  mouse-exit events.  When the user moves the mouse into a tracking                   */
/*  area, kEventMouseEnter is sent to the window.  When the user moves                  */
/*  the mouse out of a tracking area, kEventMouseExit is sent.                          */
/*                                                                                      */
/*  Mouse tracking regions are uniquely identified within the scope of                  */
/*  a window by a client signature/MouseTrackingRef pair.  The client                   */
/*  signature is the usual DTS-registered creator OSType.                               */
/*                                                                                      */
/*  Mouse tracking areas can overlap, but are not exclusive.  Mouse                     */
/*  motion events are generated for each of the tracking areas                          */
/*  intersected by the mouse.                                                           */
/*                                                                                      */
/*  Mouse tracking areas are initially enabled.  You can explicitly                     */
/*  disable a mouse tracking area to prevent mouse-enter/exit events                    */
/*  from being generated.                                                               */
/*                                                                                      */
/*  Unlike global mouse-moved events, mouse-enter and mouse-exit are                    */
/*  generated while your app is in the background.  If this is not                      */
/*  considered desirable, disable the tracking areas while the                          */
/*  application is in the background.                                                   */
/*--------------------------------------------------------------------------------------*/
typedef struct OpaqueMouseTrackingRef*  MouseTrackingRef;
/*
    Tracking Options
    If locally clipped, the region is expected in local coordinates and
    mouse movement tracking is clipped to the window's content region.
    If globally clipped, the region is expected in global coordinates and
    the tracking area is clipped to  window's structure region.
*/

typedef OptionBits                      MouseTrackingOptions;
enum {
    kMouseTrackingOptionLocalClip = 0,                          /* specify a region in local coordinates; it will be clipped to the window's content region*/
    kMouseTrackingOptionGlobalClip = 1,                         /* specify a region in global coordinates; it will be clipped to the window's structure region*/
    kMouseTrackingOptionsStandard = kMouseTrackingOptionLocalClip
};

/* Creation*/
EXTERN_API( OSStatus )
CreateMouseTrackingArea         (WindowRef              inWindow,
                                 OSType                 inCreator,
                                 MouseTrackingOptions   inOptions,
                                 RgnHandle              inRegion,
                                 MouseTrackingRef *     outMouseRef);

EXTERN_API( OSStatus )
RetainMouseTrackingArea         (MouseTrackingRef       inMouseRef);

EXTERN_API( OSStatus )
ReleaseMouseTrackingArea        (MouseTrackingRef       inMouseRef);

/* Accessors*/
EXTERN_API( OSStatus )
SetMouseTrackingAreaRefCon      (MouseTrackingRef       inMouseRef,
                                 UInt32                 refCon);

EXTERN_API( OSStatus )
GetMouseTrackingAreaRefCon      (MouseTrackingRef       inMouseRef,
                                 UInt32 *               outRefCon);

/* Geometry*/
EXTERN_API( OSStatus )
MoveMouseTrackingArea           (MouseTrackingRef       inMouseRef,
                                 SInt16                 deltaH,
                                 SInt16                 deltaV);

EXTERN_API( OSStatus )
SetMouseTrackingAreaRegion      (MouseTrackingRef       inMouseRef,
                                 RgnHandle              inRegion);

EXTERN_API( OSStatus )
GetMouseTrackingAreaRegion      (MouseTrackingRef       inMouseRef,
                                 RgnHandle              outRgn);

/* Enable/disable*/
EXTERN_API( OSStatus )
SetMouseTrackingAreaEnabled     (MouseTrackingRef       inMouseRef,
                                 Boolean                enabled);

/* Namespace-scoped operations*/
EXTERN_API( OSStatus )
ReleaseWindowMouseTrackingAreas (WindowRef              inWindow,
                                 OSType                 creator);

EXTERN_API( OSStatus )
MoveWindowMouseTrackingAreas    (WindowRef              inWindow,
                                 OSType                 creator,
                                 SInt16                 deltaH,
                                 SInt16                 deltaV);

EXTERN_API( OSStatus )
SetWindowMouseTrackingAreasEnabled (WindowRef           inWindow,
                                 OSType                 creator,
                                 Boolean                enabled);

/* Mouse event accessor additions*/

/* Mouse-enter and exit events contain the tracking ref for which they are generated.*/
EXTERN_API( OSStatus )
SetMouseEventTrackingArea       (EventRef               inEvent,
                                 MouseTrackingRef       inMouseRef);

EXTERN_API( OSStatus )
GetMouseEventTrackingArea       (EventRef               inEvent,
                                 MouseTrackingRef *     outMouseRef);



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

#endif /* __CARBONEVENTS__ */

