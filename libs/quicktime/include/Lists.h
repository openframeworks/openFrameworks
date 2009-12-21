/*
     File:       Lists.h
 
     Contains:   List Manager Interfaces.
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __LISTS__
#define __LISTS__

#ifndef __MACTYPES__
#include <MacTypes.h>
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

typedef Point                           Cell;
typedef Rect                            ListBounds;
typedef char                            DataArray[32001];
typedef char *                          DataPtr;
typedef DataPtr *                       DataHandle;
typedef CALLBACK_API( short , ListSearchProcPtr )(Ptr aPtr, Ptr bPtr, short aLen, short bLen);
typedef CALLBACK_API( Boolean , ListClickLoopProcPtr )(void );
/*
    WARNING: ListClickLoopProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef STACK_UPP_TYPE(ListSearchProcPtr)                       ListSearchUPP;
typedef REGISTER_UPP_TYPE(ListClickLoopProcPtr)                 ListClickLoopUPP;
#if !TARGET_OS_MAC
/* QuickTime 3.0 */
typedef long                            ListNotification;
enum {
    listNotifyNothing           = FOUR_CHAR_CODE('nada'),       /* No (null) notification*/
    listNotifyClick             = FOUR_CHAR_CODE('clik'),       /* Control was clicked*/
    listNotifyDoubleClick       = FOUR_CHAR_CODE('dblc'),       /* Control was double-clicked*/
    listNotifyPreClick          = FOUR_CHAR_CODE('pclk')        /* Control about to be clicked*/
};

#endif  /* !TARGET_OS_MAC */


struct ListRec {
    Rect                            rView;
    GrafPtr                         port;
    Point                           indent;
    Point                           cellSize;
    ListBounds                      visible;
    ControlHandle                   vScroll;
    ControlHandle                   hScroll;
    SInt8                           selFlags;
    Boolean                         lActive;
    SInt8                           lReserved;
    SInt8                           listFlags;
    long                            clikTime;
    Point                           clikLoc;
    Point                           mouseLoc;
    ListClickLoopUPP                lClickLoop;
    Cell                            lastClick;
    long                            refCon;
    Handle                          listDefProc;
    Handle                          userHandle;
    ListBounds                      dataBounds;
    DataHandle                      cells;
    short                           maxIndex;
    short                           cellArray[1];
};
typedef struct ListRec                  ListRec;

typedef ListRec *                       ListPtr;
typedef ListPtr *                       ListHandle;
/* ListRef is obsolete.  Use ListHandle. */
typedef ListHandle                      ListRef;


enum {
                                                                /* ListRec.listFlags bits*/
    lDoVAutoscrollBit           = 1,
    lDoHAutoscrollBit           = 0
};

enum {
                                                                /* ListRec.listFlags masks*/
    lDoVAutoscroll              = 2,
    lDoHAutoscroll              = 1
};


enum {
                                                                /* ListRec.selFlags bits*/
    lOnlyOneBit                 = 7,
    lExtendDragBit              = 6,
    lNoDisjointBit              = 5,
    lNoExtendBit                = 4,
    lNoRectBit                  = 3,
    lUseSenseBit                = 2,
    lNoNilHiliteBit             = 1
};


enum {
                                                                /* ListRec.selFlags masks*/
    lOnlyOne                    = -128,
    lExtendDrag                 = 64,
    lNoDisjoint                 = 32,
    lNoExtend                   = 16,
    lNoRect                     = 8,
    lUseSense                   = 4,
    lNoNilHilite                = 2
};


enum {
                                                                /* LDEF messages*/
    lInitMsg                    = 0,
    lDrawMsg                    = 1,
    lHiliteMsg                  = 2,
    lCloseMsg                   = 3
};




typedef CALLBACK_API( void , ListDefProcPtr )(short lMessage, Boolean lSelect, Rect *lRect, Cell lCell, short lDataOffset, short lDataLen, ListHandle lHandle);
typedef STACK_UPP_TYPE(ListDefProcPtr)                          ListDefUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(ListSearchUPP)
    NewListSearchUPP               (ListSearchProcPtr       userRoutine);

    EXTERN_API(ListClickLoopUPP)
    NewListClickLoopUPP            (ListClickLoopProcPtr    userRoutine);

    EXTERN_API(ListDefUPP)
    NewListDefUPP                  (ListDefProcPtr          userRoutine);

    EXTERN_API(void)
    DisposeListSearchUPP           (ListSearchUPP           userUPP);

    EXTERN_API(void)
    DisposeListClickLoopUPP        (ListClickLoopUPP        userUPP);

    EXTERN_API(void)
    DisposeListDefUPP              (ListDefUPP              userUPP);

    EXTERN_API(short)
    InvokeListSearchUPP            (Ptr                     aPtr,
                                    Ptr                     bPtr,
                                    short                   aLen,
                                    short                   bLen,
                                    ListSearchUPP           userUPP);

    EXTERN_API(Boolean)
    InvokeListClickLoopUPP         (ListClickLoopUPP        userUPP);

    EXTERN_API(void)
    InvokeListDefUPP               (short                   lMessage,
                                    Boolean                 lSelect,
                                    Rect *                  lRect,
                                    Cell                    lCell,
                                    short                   lDataOffset,
                                    short                   lDataLen,
                                    ListHandle              lHandle,
                                    ListDefUPP              userUPP);

#else
    enum { uppListSearchProcInfo = 0x00002BE0 };                    /* pascal 2_bytes Func(4_bytes, 4_bytes, 2_bytes, 2_bytes) */
    enum { uppListClickLoopProcInfo = 0x00000012 };                 /* register 1_byte:D0 Func() */
    enum { uppListDefProcInfo = 0x000EBD80 };                       /* pascal no_return_value Func(2_bytes, 1_byte, 4_bytes, 4_bytes, 2_bytes, 2_bytes, 4_bytes) */
    #define NewListSearchUPP(userRoutine)                           (ListSearchUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppListSearchProcInfo, GetCurrentArchitecture())
    #define NewListClickLoopUPP(userRoutine)                        (ListClickLoopUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppListClickLoopProcInfo, GetCurrentArchitecture())
    #define NewListDefUPP(userRoutine)                              (ListDefUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppListDefProcInfo, GetCurrentArchitecture())
    #define DisposeListSearchUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposeListClickLoopUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposeListDefUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define InvokeListSearchUPP(aPtr, bPtr, aLen, bLen, userUPP)    (short)CALL_FOUR_PARAMETER_UPP((userUPP), uppListSearchProcInfo, (aPtr), (bPtr), (aLen), (bLen))
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter __D0 InvokeListClickLoopUPP(__A0)
    Boolean InvokeListClickLoopUPP(ListClickLoopUPP userUPP) = 0x4E90;
    #else
        #define InvokeListClickLoopUPP(userUPP)                         (Boolean)CALL_ZERO_PARAMETER_UPP((userUPP), uppListClickLoopProcInfo)
    #endif
    #define InvokeListDefUPP(lMessage, lSelect, lRect, lCell, lDataOffset, lDataLen, lHandle, userUPP)  CALL_SEVEN_PARAMETER_UPP((userUPP), uppListDefProcInfo, (lMessage), (lSelect), (lRect), (lCell), (lDataOffset), (lDataLen), (lHandle))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewListSearchProc(userRoutine)                          NewListSearchUPP(userRoutine)
#define NewListClickLoopProc(userRoutine)                       NewListClickLoopUPP(userRoutine)
#define NewListDefProc(userRoutine)                             NewListDefUPP(userRoutine)
#define CallListSearchProc(userRoutine, aPtr, bPtr, aLen, bLen) InvokeListSearchUPP(aPtr, bPtr, aLen, bLen, userRoutine)
#define CallListClickLoopProc(userRoutine)                      InvokeListClickLoopUPP(userRoutine)
#define CallListDefProc(userRoutine, lMessage, lSelect, lRect, lCell, lDataOffset, lDataLen, lHandle) InvokeListDefUPP(lMessage, lSelect, lRect, lCell, lDataOffset, lDataLen, lHandle, userRoutine)
enum {
    kListDefUserProcType        = 0,
    kListDefStandardTextType    = 1,
    kListDefStandardIconType    = 2
};

typedef UInt32                          ListDefType;

struct ListDefSpec {
    ListDefType                     defType;
    union {
        ListDefUPP                      userProc;
    }                                 u;
};
typedef struct ListDefSpec              ListDefSpec;
EXTERN_API( OSStatus )
CreateCustomList                (const Rect *           rView,
                                 const ListBounds *     dataBounds,
                                 Point                  cellSize,
                                 const ListDefSpec *    theSpec,
                                 WindowPtr              theWindow,
                                 Boolean                drawIt,
                                 Boolean                hasGrow,
                                 Boolean                scrollHoriz,
                                 Boolean                scrollVert,
                                 ListHandle *           outList);


#if !TARGET_OS_MAC
/* QuickTime 3.0 */
typedef CALLBACK_API( void , ListNotificationProcPtr )(ListHandle theList, ListNotification notification, long param);

typedef ListNotificationProcPtr         ListNotificationUPP;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
LSetNotificationCallback        (ListNotificationProcPtr  callBack,
                                 ListHandle             lHandle);

EXTERN_API_C( void )
GetListVisibleBounds            (ListHandle             theList,
                                 Rect *                 visibleBounds);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !TARGET_OS_MAC */

EXTERN_API( ListHandle )
LNew                            (const Rect *           rView,
                                 const ListBounds *     dataBounds,
                                 Point                  cSize,
                                 short                  theProc,
                                 WindowPtr              theWindow,
                                 Boolean                drawIt,
                                 Boolean                hasGrow,
                                 Boolean                scrollHoriz,
                                 Boolean                scrollVert)                         THREEWORDINLINE(0x3F3C, 0x0044, 0xA9E7);

EXTERN_API( void )
LDispose                        (ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0028, 0xA9E7);

EXTERN_API( short )
LAddColumn                      (short                  count,
                                 short                  colNum,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0004, 0xA9E7);

EXTERN_API( short )
LAddRow                         (short                  count,
                                 short                  rowNum,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0008, 0xA9E7);

EXTERN_API( void )
LDelColumn                      (short                  count,
                                 short                  colNum,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0020, 0xA9E7);

EXTERN_API( void )
LDelRow                         (short                  count,
                                 short                  rowNum,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0024, 0xA9E7);

EXTERN_API( Boolean )
LGetSelect                      (Boolean                next,
                                 Cell *                 theCell,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x003C, 0xA9E7);

EXTERN_API( Cell )
LLastClick                      (ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0040, 0xA9E7);

EXTERN_API( Boolean )
LNextCell                       (Boolean                hNext,
                                 Boolean                vNext,
                                 Cell *                 theCell,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0048, 0xA9E7);

EXTERN_API( Boolean )
LSearch                         (const void *           dataPtr,
                                 short                  dataLen,
                                 ListSearchUPP          searchProc,
                                 Cell *                 theCell,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0054, 0xA9E7);

EXTERN_API( void )
LSize                           (short                  listWidth,
                                 short                  listHeight,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0060, 0xA9E7);

EXTERN_API( void )
LSetDrawingMode                 (Boolean                drawIt,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x002C, 0xA9E7);

EXTERN_API( void )
LScroll                         (short                  dCols,
                                 short                  dRows,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0050, 0xA9E7);

EXTERN_API( void )
LAutoScroll                     (ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0010, 0xA9E7);

EXTERN_API( void )
LUpdate                         (RgnHandle              theRgn,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0064, 0xA9E7);

EXTERN_API( void )
LActivate                       (Boolean                act,
                                 ListHandle             lHandle)                            TWOWORDINLINE(0x4267, 0xA9E7);

EXTERN_API( void )
LCellSize                       (Point                  cSize,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0014, 0xA9E7);

EXTERN_API( Boolean )
LClick                          (Point                  pt,
                                 short                  modifiers,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0018, 0xA9E7);

EXTERN_API( void )
LAddToCell                      (const void *           dataPtr,
                                 short                  dataLen,
                                 Cell                   theCell,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x000C, 0xA9E7);

EXTERN_API( void )
LClrCell                        (Cell                   theCell,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x001C, 0xA9E7);

EXTERN_API( void )
LGetCell                        (void *                 dataPtr,
                                 short *                dataLen,
                                 Cell                   theCell,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0038, 0xA9E7);

EXTERN_API( void )
LRect                           (Rect *                 cellRect,
                                 Cell                   theCell,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x004C, 0xA9E7);

EXTERN_API( void )
LSetCell                        (const void *           dataPtr,
                                 short                  dataLen,
                                 Cell                   theCell,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0058, 0xA9E7);

EXTERN_API( void )
LSetSelect                      (Boolean                setIt,
                                 Cell                   theCell,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x005C, 0xA9E7);

EXTERN_API( void )
LDraw                           (Cell                   theCell,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0030, 0xA9E7);

EXTERN_API( void )
LGetCellDataLocation            (short *                offset,
                                 short *                len,
                                 Cell                   theCell,
                                 ListHandle             lHandle)                            THREEWORDINLINE(0x3F3C, 0x0034, 0xA9E7);

#if CALL_NOT_IN_CARBON
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
SetListDefinitionProc           (SInt16                 resID,
                                 ListDefUPP             defProc);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* CALL_NOT_IN_CARBON */

#if !TARGET_OS_MAC
/* QuickTime 3.0 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
LSetLDEF                        (ListDefProcPtr         proc,
                                 ListRef                lHandle);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* !TARGET_OS_MAC */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
laddtocell                      (const void *           dataPtr,
                                 short                  dataLen,
                                 const Cell *           theCell,
                                 ListHandle             lHandle);

EXTERN_API_C( void )
lclrcell                        (const Cell *           theCell,
                                 ListHandle             lHandle);

EXTERN_API_C( void )
lgetcelldatalocation            (short *                offset,
                                 short *                len,
                                 const Cell *           theCell,
                                 ListHandle             lHandle);

EXTERN_API_C( void )
lgetcell                        (void *                 dataPtr,
                                 short *                dataLen,
                                 const Cell *           theCell,
                                 ListHandle             lHandle);

EXTERN_API_C( ListHandle )
lnew                            (const Rect *           rView,
                                 const ListBounds *     dataBounds,
                                 Point *                cSize,
                                 short                  theProc,
                                 WindowPtr              theWindow,
                                 Boolean                drawIt,
                                 Boolean                hasGrow,
                                 Boolean                scrollHoriz,
                                 Boolean                scrollVert);

EXTERN_API_C( void )
lrect                           (Rect *                 cellRect,
                                 const Cell *           theCell,
                                 ListHandle             lHandle);

EXTERN_API_C( void )
lsetcell                        (const void *           dataPtr,
                                 short                  dataLen,
                                 const Cell *           theCell,
                                 ListHandle             lHandle);

EXTERN_API_C( void )
lsetselect                      (Boolean                setIt,
                                 const Cell *           theCell,
                                 ListHandle             lHandle);

EXTERN_API_C( void )
ldraw                           (const Cell *           theCell,
                                 ListHandle             lHandle);

EXTERN_API_C( Boolean )
lclick                          (Point *                pt,
                                 short                  modifiers,
                                 ListHandle             lHandle);

EXTERN_API_C( void )
lcellsize                       (Point *                cSize,
                                 ListHandle             lHandle);

#endif  /* CALL_NOT_IN_CARBON */

#if OLDROUTINENAMES
#define LDoDraw(drawIt, lHandle) LSetDrawingMode(drawIt, lHandle)
#define LFind(offset, len, theCell, lHandle) LGetCellDataLocation(offset, len, theCell, lHandle)
#if CGLUESUPPORTED
#define lfind(offset, len, theCell, lHandle) lgetcelldatalocation(offset, len, theCell, lHandle)
#endif
#endif  /* OLDROUTINENAMES */

#if ACCESSOR_CALLS_ARE_FUNCTIONS
/* Getters */
EXTERN_API( Rect *)
GetListViewBounds               (ListRef                list,
                                 Rect *                 view);

EXTERN_API( CGrafPtr )
GetListPort                     (ListRef                list);

EXTERN_API( Point *)
GetListCellIndent               (ListRef                list,
                                 Point *                indent);

EXTERN_API( Point *)
GetListCellSize                 (ListRef                list,
                                 Point *                size);

EXTERN_API( ListBounds *)
GetListVisibleCells             (ListRef                list,
                                 ListBounds *           visible);

EXTERN_API( ControlHandle )
GetListVerticalScrollBar        (ListRef                list);

EXTERN_API( ControlHandle )
GetListHorizontalScrollBar      (ListRef                list);

EXTERN_API( Boolean )
GetListActive                   (ListRef                list);

EXTERN_API( SInt32 )
GetListClickTime                (ListRef                list);

EXTERN_API( Point *)
GetListClickLocation            (ListRef                list,
                                 Point *                click);

EXTERN_API( Point *)
GetListMouseLocation            (ListRef                list,
                                 Point *                mouse);

EXTERN_API( ListClickLoopUPP )
GetListClickLoop                (ListRef                list);

EXTERN_API( SInt32 )
GetListRefCon                   (ListRef                list);

EXTERN_API( Handle )
GetListDefinition               (ListRef                list);

EXTERN_API( Handle )
GetListUserHandle               (ListRef                list);

EXTERN_API( ListBounds *)
GetListDataBounds               (ListRef                list,
                                 ListBounds *           bounds);

EXTERN_API( DataHandle )
GetListDataHandle               (ListRef                list);

EXTERN_API( OptionBits )
GetListFlags                    (ListRef                list);

EXTERN_API( OptionBits )
GetListSelectionFlags           (ListRef                list);

/* Setters */
EXTERN_API( void )
SetListViewBounds               (ListRef                list,
                                 const Rect *           view);

EXTERN_API( void )
SetListPort                     (ListRef                list,
                                 CGrafPtr               port);

EXTERN_API( void )
SetListCellIndent               (ListRef                list,
                                 Point *                indent);

EXTERN_API( void )
SetListClickTime                (ListRef                list,
                                 SInt32                 time);

EXTERN_API( void )
SetListClickLoop                (ListRef                list,
                                 ListClickLoopUPP       clickLoop);

EXTERN_API( void )
SetListLastClick                (ListRef                list,
                                 Cell *                 lastClick);

EXTERN_API( void )
SetListRefCon                   (ListRef                list,
                                 SInt32                 refCon);

EXTERN_API( void )
SetListUserHandle               (ListRef                list,
                                 Handle                 userHandle);

EXTERN_API( void )
SetListFlags                    (ListRef                list,
                                 OptionBits             listFlags);

EXTERN_API( void )
SetListSelectionFlags           (ListRef                list,
                                 OptionBits             selectionFlags);

/* WARNING: These may go away in a future build.  Beware! */
#if CALL_NOT_IN_CARBON
EXTERN_API( void )
SetListDefinition               (ListRef                list,
                                 Handle                 listDefProc);

EXTERN_API( void )
SetListCellSize                 (ListRef                list,
                                 Point *                size);

EXTERN_API( void )
SetListHorizontalScrollBar      (ListRef                list,
                                 ControlHandle          hScroll);

EXTERN_API( void )
SetListVerticalScrollBar        (ListRef                list,
                                 ControlHandle          vScroll);

EXTERN_API( void )
SetListVisibleCells             (ListRef                list,
                                 ListBounds *           visible);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* ACCESSOR_CALLS_ARE_FUNCTIONS */


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

#endif /* __LISTS__ */

