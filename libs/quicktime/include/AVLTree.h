/*
     File:       AVLTree.h
 
     Contains:   Prototypes for routines which create, destroy, allow for
 
     Version:    Technology: 
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __AVLTREE__
#define __AVLTREE__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif


/* The visit stage for AVLWalk() walkProcs */


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


typedef UInt16 AVLVisitStage;
enum {
    kAVLPreOrder                = 0,
    kAVLInOrder                 = 1,
    kAVLPostOrder               = 2
};

/* The order the tree is walked or disposed of. */

typedef UInt16 AVLOrder;
enum {
    kLeftToRight                = 0,
    kRightToLeft                = 1
};

/* The type of the node being passed to a callback proc. */

typedef UInt16 AVLNodeType;
enum {
    kAVLIsTree                  = 0,
    kAVLIsLeftBranch            = 1,
    kAVLIsRightBranch           = 2,
    kAVLIsLeaf                  = 3,
    kAVLNullNode                = 4
};

enum {
    errItemAlreadyInTree        = -960,
    errNotValidTree             = -961,
    errItemNotFoundInTree       = -962,
    errCanNotInsertWhileWalkProcInProgress = -963,
    errTreeIsLocked             = -964
};

/*  The structure of a tree.  It's opaque; don't assume it's 36 bytes in size.*/

struct AVLTreeStruct {
    OSType                          signature;
    unsigned long                   privateStuff[8];
};
typedef struct AVLTreeStruct            AVLTreeStruct;

typedef AVLTreeStruct *                 AVLTreePtr;
/*
    Every tree must have a function which compares the data for two items and returns < 0, 0, or >0
    for the items - < 0 if the first item is 'before' the second item according to some criteria,
    == 0 if the two items are identical according to the criteria, or > 0 if the first item is
    'after' the second item according to the criteria.  The comparison function is also passed the
    node type, but most of the time this can be ignored.
*/
typedef CALLBACK_API( SInt32 , AVLCompareItemsProcPtr )(AVLTreePtr tree, const void *i1, const void *i2, AVLNodeType nd_typ);
/*
    Every tree must have a itemSizeProc; this routine gets passed a pointer to the item's data and
    returns the size of the data.  If a tree contains records of a fixed size, this function can
    just return sizeof( that-struct ); otherwise it should calculate the size of the item based on
    the data for the item.
*/
typedef CALLBACK_API( UInt32 , AVLItemSizeProcPtr )(AVLTreePtr tree, const void *itemPtr);
/*
    A tree may have an optional disposeItemProc, which gets called whenever an item is removed
    from the tree ( via AVLRemove() or when AVLDispose() deletes all of the items in the tree ).
    This might be useful if the nodes in the tree own 'resources'  ( like, open files ) which
    should be released before the item is removed.
*/
typedef CALLBACK_API( void , AVLDisposeItemProcPtr )(AVLTreePtr tree, const void *dataP);
/*
    The common way to iterate across all of the items in a tree is via AVLWalk(), which takes
    a walkProcPtr.  This function will get called for every item in the tree three times, as
    the tree is being walked across.  First, the walkProc will get called with visitStage ==
    kAVLPreOrder, at which point internally the node of the tree for the given data has just
    been reached.  Later, this function will get called with visitStage == kAVLInOrder, and
    lastly this function will get called with visitStage == kAVLPostOrder.
    The 'minimum' item in the tree will get called with visitStage == kInOrder first, followed
    by the 'next' item in the tree, up until the last item in the tree structure is called.
    In general, you'll only care about calls to this function when visitStage == kAVLInOrder.
*/
typedef CALLBACK_API( OSErr , AVLWalkProcPtr )(AVLTreePtr tree, const void *dataP, AVLVisitStage visitStage, AVLNodeType node, UInt32 level, SInt32 balance, void *refCon);
typedef STACK_UPP_TYPE(AVLCompareItemsProcPtr)                  AVLCompareItemsUPP;
typedef STACK_UPP_TYPE(AVLItemSizeProcPtr)                      AVLItemSizeUPP;
typedef STACK_UPP_TYPE(AVLDisposeItemProcPtr)                   AVLDisposeItemUPP;
typedef STACK_UPP_TYPE(AVLWalkProcPtr)                          AVLWalkUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(AVLCompareItemsUPP)
    NewAVLCompareItemsUPP          (AVLCompareItemsProcPtr  userRoutine);

    EXTERN_API(AVLItemSizeUPP)
    NewAVLItemSizeUPP              (AVLItemSizeProcPtr      userRoutine);

    EXTERN_API(AVLDisposeItemUPP)
    NewAVLDisposeItemUPP           (AVLDisposeItemProcPtr   userRoutine);

    EXTERN_API(AVLWalkUPP)
    NewAVLWalkUPP                  (AVLWalkProcPtr          userRoutine);

    EXTERN_API(void)
    DisposeAVLCompareItemsUPP      (AVLCompareItemsUPP      userUPP);

    EXTERN_API(void)
    DisposeAVLItemSizeUPP          (AVLItemSizeUPP          userUPP);

    EXTERN_API(void)
    DisposeAVLDisposeItemUPP       (AVLDisposeItemUPP       userUPP);

    EXTERN_API(void)
    DisposeAVLWalkUPP              (AVLWalkUPP              userUPP);

    EXTERN_API(SInt32)
    InvokeAVLCompareItemsUPP       (AVLTreePtr              tree,
                                    const void *            i1,
                                    const void *            i2,
                                    AVLNodeType             nd_typ,
                                    AVLCompareItemsUPP      userUPP);

    EXTERN_API(UInt32)
    InvokeAVLItemSizeUPP           (AVLTreePtr              tree,
                                    const void *            itemPtr,
                                    AVLItemSizeUPP          userUPP);

    EXTERN_API(void)
    InvokeAVLDisposeItemUPP        (AVLTreePtr              tree,
                                    const void *            dataP,
                                    AVLDisposeItemUPP       userUPP);

    EXTERN_API(OSErr)
    InvokeAVLWalkUPP               (AVLTreePtr              tree,
                                    const void *            dataP,
                                    AVLVisitStage           visitStage,
                                    AVLNodeType             node,
                                    UInt32                  level,
                                    SInt32                  balance,
                                    void *                  refCon,
                                    AVLWalkUPP              userUPP);

#else
    enum { uppAVLCompareItemsProcInfo = 0x00002FF0 };               /* pascal 4_bytes Func(4_bytes, 4_bytes, 4_bytes, 2_bytes) */
    enum { uppAVLItemSizeProcInfo = 0x000003F0 };                   /* pascal 4_bytes Func(4_bytes, 4_bytes) */
    enum { uppAVLDisposeItemProcInfo = 0x000003C0 };                /* pascal no_return_value Func(4_bytes, 4_bytes) */
    enum { uppAVLWalkProcInfo = 0x000FEBE0 };                       /* pascal 2_bytes Func(4_bytes, 4_bytes, 2_bytes, 2_bytes, 4_bytes, 4_bytes, 4_bytes) */
    #define NewAVLCompareItemsUPP(userRoutine)                      (AVLCompareItemsUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppAVLCompareItemsProcInfo, GetCurrentArchitecture())
    #define NewAVLItemSizeUPP(userRoutine)                          (AVLItemSizeUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppAVLItemSizeProcInfo, GetCurrentArchitecture())
    #define NewAVLDisposeItemUPP(userRoutine)                       (AVLDisposeItemUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppAVLDisposeItemProcInfo, GetCurrentArchitecture())
    #define NewAVLWalkUPP(userRoutine)                              (AVLWalkUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppAVLWalkProcInfo, GetCurrentArchitecture())
    #define DisposeAVLCompareItemsUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #define DisposeAVLItemSizeUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposeAVLDisposeItemUPP(userUPP)                       DisposeRoutineDescriptor(userUPP)
    #define DisposeAVLWalkUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define InvokeAVLCompareItemsUPP(tree, i1, i2, nd_typ, userUPP)  (SInt32)CALL_FOUR_PARAMETER_UPP((userUPP), uppAVLCompareItemsProcInfo, (tree), (i1), (i2), (nd_typ))
    #define InvokeAVLItemSizeUPP(tree, itemPtr, userUPP)            (UInt32)CALL_TWO_PARAMETER_UPP((userUPP), uppAVLItemSizeProcInfo, (tree), (itemPtr))
    #define InvokeAVLDisposeItemUPP(tree, dataP, userUPP)           CALL_TWO_PARAMETER_UPP((userUPP), uppAVLDisposeItemProcInfo, (tree), (dataP))
    #define InvokeAVLWalkUPP(tree, dataP, visitStage, node, level, balance, refCon, userUPP)  (OSErr)CALL_SEVEN_PARAMETER_UPP((userUPP), uppAVLWalkProcInfo, (tree), (dataP), (visitStage), (node), (level), (balance), (refCon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewAVLCompareItemsProc(userRoutine)                     NewAVLCompareItemsUPP(userRoutine)
#define NewAVLItemSizeProc(userRoutine)                         NewAVLItemSizeUPP(userRoutine)
#define NewAVLDisposeItemProc(userRoutine)                      NewAVLDisposeItemUPP(userRoutine)
#define NewAVLWalkProc(userRoutine)                             NewAVLWalkUPP(userRoutine)
#define CallAVLCompareItemsProc(userRoutine, tree, i1, i2, nd_typ) InvokeAVLCompareItemsUPP(tree, i1, i2, nd_typ, userRoutine)
#define CallAVLItemSizeProc(userRoutine, tree, itemPtr)         InvokeAVLItemSizeUPP(tree, itemPtr, userRoutine)
#define CallAVLDisposeItemProc(userRoutine, tree, dataP)        InvokeAVLDisposeItemUPP(tree, dataP, userRoutine)
#define CallAVLWalkProc(userRoutine, tree, dataP, visitStage, node, level, balance, refCon) InvokeAVLWalkUPP(tree, dataP, visitStage, node, level, balance, refCon, userRoutine)
/*
    Create an AVL tree.  The compareItemsProc and the sizeItemProc are required; disposeItemProc is
    optional and can be nil.  The refCon is stored with the list, and is passed back to the
    compareItemsProc, sizeItemProc, and disposeItemsProc calls.  The allocation of the tree ( and all
    nodes later added to the list with AVLInsert ) will be created in what is the current zone at the
    time AVLInit() is called.  Always call AVLDispose() to dispose of a list created with AVLInit().
*/
EXTERN_API( OSErr )
AVLInit                         (UInt32                 flags,
                                 AVLCompareItemsUPP     compareItemsProc,
                                 AVLItemSizeUPP         sizeItemProc,
                                 AVLDisposeItemUPP      disposeItemProc,
                                 void *                 refCon,
                                 AVLTreePtr *           tree)                               THREEWORDINLINE(0x303C, 0x0C01, 0xAA80);

/*
    Dispose of an AVL tree.  This will dispose of each item in the tree in the order specified,
    call the tree's disposeProc proc for each item, and then dispose of the space allocated for
    the tree itself.
*/
EXTERN_API( OSErr )
AVLDispose                      (AVLTreePtr *           tree,
                                 AVLOrder               order)                              THREEWORDINLINE(0x303C, 0x0302, 0xAA80);

/*
    Iterate across all of the items in the tree, in the order specified.  kLeftToRight is
    basically lowest-to-highest order, kRightToLeft is highest-to-lowest order.  For each
    node in the tree, it will call the walkProc with three messages ( at the appropriate 
    time ).  First, with kAVLPreOrder when the walking gets to this node in the tree,
    before handling either the left or right subtree, secondly, with kAVLInOrder after
    handling one subtree but before handling the other, and lastly with kAVLPostOrder after
    handling both subtrees.  If you want to handle items in order, then only do something
    if the visit stage is kAVLInOrder.  You can only call AVLRemove() from inside a walkProc
    if visit stage is kAVLPostOrder ( because if you remove a node during the pre or in order
    stages you will corrupt the list ) OR if you return a non-zero result from the walkProc
    call which called AVLRemove() to immediately terminate the walkProc.  Do not call AVLInsert()
    to insert a node into the tree from inside a walkProc.
    The walkProc function gets called with the AVLTreePtr, a pointer to the data for the
    current node ( which you can change in place as long as you do not affect the order within
    the tree ), the visit stage, the type of the current node ( leaf node, right or left branch,
    or full tree ), the level within the tree ( the root is level 1 ), the balance for the
    current node, and the refCon passed to AVLWalk().  This refCon is different from the one passed
    into AVLInit(); use AVLGetRefCon() to get that refCon if you want it inside a walkProc.
    ( Most walkProcs will not care about the values for node type, level, or balance. )
*/
EXTERN_API( OSErr )
AVLWalk                         (AVLTreePtr             tree,
                                 AVLWalkUPP             walkProc,
                                 AVLOrder               order,
                                 void *                 walkRefCon)                         THREEWORDINLINE(0x303C, 0x0703, 0xAA80);

/*  Return  the number of items in the given tree.*/
EXTERN_API( OSErr )
AVLCount                        (AVLTreePtr             tree,
                                 UInt32 *               count)                              THREEWORDINLINE(0x303C, 0x0804, 0xAA80);

/*
    Return the one-based index-th item from the tree by putting it's data at dataPtr
    if dataPtr is non-nil, and it's size into *itemSize if itemSize is non-nil.
    If index is out of range, return errItemNotFoundInTree.  ( Internally, this does
    an AVLWalk(), so the tree can not be modified while this call is in progress ).
*/
EXTERN_API( OSErr )
AVLGetIndItem                   (AVLTreePtr             tree,
                                 UInt32                 index,
                                 void *                 dataPtr,
                                 UInt32 *               itemSize)                           THREEWORDINLINE(0x303C, 0x0805, 0xAA80);

/*
    Insert the given item into the tree.  This will call the tree's sizeItemProc
    to determine how big the item at data is, and then will make a copy of the
    item and insert it into the tree in the appropriate place.  If an item already
    exists in the tree with the same key ( so that the compareItemsUPP returns 0
    when asked to compare this item to an existing one ), then it will return
    errItemNotFoundInTree.
*/
EXTERN_API( OSErr )
AVLInsert                       (AVLTreePtr             tree,
                                 const void *           data)                               THREEWORDINLINE(0x303C, 0x0406, 0xAA80);

/*
    Remove any item from the tree with the given key.  If dataPtr != nil, then
    copy the item's data to dataPtr before removing it from the tree.  Before
    removing the item, call the tree's disposeItemProc to let it release anything
    used by the data in the tree.  It is not necessary to fill in a complete
    record for key, only that the compareItemsProc return 0 when asked to compare
    the data at key with the node in the tree to be deleted.  If the item cannot
    be found in the tree, this will return errItemNotFoundInTree.
*/
EXTERN_API( OSErr )
AVLRemove                       (AVLTreePtr             tree,
                                 const void *           key,
                                 void *                 dataPtr,
                                 UInt32 *               itemSize)                           THREEWORDINLINE(0x303C, 0x0807, 0xAA80);

/*
    Find the item in the tree with the given key, and return it's data in
    dataPtr ( if dataPtr != nil ), and it's size in *itemSize ( if itemSize
    != nil ).  It is not necessary to fill in a complete record for key,
    only that the compareItemsProc return 0 when asked to compare the data
    at key with the node in the tree to be deleted.  If the item cannot
    be found in the tree, this will return errItemNotFoundInTree.
*/
EXTERN_API( OSErr )
AVLFind                         (AVLTreePtr             tree,
                                 const void *           key,
                                 void *                 dataPtr,
                                 UInt32 *               itemSize)                           THREEWORDINLINE(0x303C, 0x0808, 0xAA80);

/*
    Get the refCon for the given tree ( set in AVLInit ) and return it.
    If the given tree is invalid, then return nil.
*/
EXTERN_API( OSErr )
AVLGetRefcon                    (AVLTreePtr             tree,
                                 void **                refCon)                             THREEWORDINLINE(0x303C, 0x0409, 0xAA80);


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

#endif /* __AVLTREE__ */

