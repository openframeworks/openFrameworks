/*
     File:       DatabaseAccess.h
 
     Contains:   Database Access Manager Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1989-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DATABASEACCESS__
#define __DATABASEACCESS__

#ifndef __RESOURCES__
#include <Resources.h>
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

/* data type codes */
enum {
    typeNone                    = FOUR_CHAR_CODE('none'),
    typeDate                    = FOUR_CHAR_CODE('date'),
    typeTime                    = FOUR_CHAR_CODE('time'),
    typeTimeStamp               = FOUR_CHAR_CODE('tims'),
    typeDecimal                 = FOUR_CHAR_CODE('deci'),
    typeMoney                   = FOUR_CHAR_CODE('mone'),
    typeVChar                   = FOUR_CHAR_CODE('vcha'),
    typeVBin                    = FOUR_CHAR_CODE('vbin'),
    typeLChar                   = FOUR_CHAR_CODE('lcha'),
    typeLBin                    = FOUR_CHAR_CODE('lbin'),
    typeDiscard                 = FOUR_CHAR_CODE('disc'),       /* "dummy" types for DBResultsToText */
    typeUnknown                 = FOUR_CHAR_CODE('unkn'),
    typeColBreak                = FOUR_CHAR_CODE('colb'),
    typeRowBreak                = FOUR_CHAR_CODE('rowb'),       /* pass this in to DBGetItem for any data type */
    typeAnyType                 = 0
};

/* infinite timeout value for DBGetItem */
enum {
                                                                /* messages for status functions for DBStartQuery */
    kDBUpdateWind               = 0,
    kDBAboutToInit              = 1,
    kDBInitComplete             = 2,
    kDBSendComplete             = 3,
    kDBExecComplete             = 4,
    kDBStartQueryComplete       = 5
};

enum {
                                                                /* messages for status functions for DBGetQueryResults */
    kDBGetItemComplete          = 6,
    kDBGetQueryResultsComplete  = 7,
    kDBWaitForever              = -1
};

enum {
                                                                /*  flags for DBGetItem  */
    kDBLastColFlag              = 0x0001,
    kDBNullFlag                 = 0x0004
};

typedef OSType                          DBType;
typedef struct DBAsyncParamBlockRec     DBAsyncParamBlockRec;
typedef DBAsyncParamBlockRec *          DBAsyncParmBlkPtr;
typedef CALLBACK_API( void , DBCompletionProcPtr )(DBAsyncParmBlkPtr pb);
/*
    WARNING: DBCompletionProcPtr uses register based parameters under classic 68k
             and cannot be written in a high-level language without 
             the help of mixed mode or assembly glue.
*/
typedef REGISTER_UPP_TYPE(DBCompletionProcPtr)                  DBCompletionUPP;
/* structure for asynchronous parameter block */

struct DBAsyncParamBlockRec {
    DBCompletionUPP                 completionProc;             /* pointer to completion routine */
    OSErr                           result;                     /* result of call */
    long                            userRef;                    /* for application's use */
    long                            ddevRef;                    /* for ddev's use */
    long                            reserved;                   /* for internal use */
};

/* structure for resource list in QueryRecord */

struct ResListElem {
    ResType                         theType;                    /* resource type */
    short                           id;                         /* resource id */
};
typedef struct ResListElem              ResListElem;

typedef ResListElem *                   ResListPtr;
typedef ResListPtr *                    ResListHandle;
/* structure for query list in QueryRecord */
typedef Handle                          QueryArray[256];
typedef Handle *                        QueryListPtr;
typedef QueryListPtr *                  QueryListHandle;

struct QueryRecord {
    short                           version;                    /* version */
    short                           id;                         /* id of 'qrsc' this came from */
    Handle                          queryProc;                  /* handle to query def proc */
    Str63                           ddevName;                   /* ddev name */
    Str255                          host;                       /* host name */
    Str255                          user;                       /* user name */
    Str255                          password;                   /* password */
    Str255                          connStr;                    /* connection string */
    short                           currQuery;                  /* index of current query */
    short                           numQueries;                 /* number of queries in list */
    QueryListHandle                 queryList;                  /* handle to array of handles to text */
    short                           numRes;                     /* number of resources in list */
    ResListHandle                   resList;                    /* handle to array of resource list elements */
    Handle                          dataHandle;                 /* for use by query def proc */
    long                            refCon;                     /* for use by application */
};
typedef struct QueryRecord              QueryRecord;

typedef QueryRecord *                   QueryPtr;
typedef QueryPtr *                      QueryHandle;
/* structure of column types array in ResultsRecord */
typedef DBType                          ColTypesArray[256];
typedef Handle                          ColTypesHandle;
/* structure for column info in ResultsRecord */

struct DBColInfoRecord {
    short                           len;
    short                           places;
    short                           flags;
};
typedef struct DBColInfoRecord          DBColInfoRecord;

typedef DBColInfoRecord                 ColInfoArray[256];
typedef Handle                          ColInfoHandle;
/* structure of results returned by DBGetResults */

struct ResultsRecord {
    short                           numRows;                    /* number of rows in result */
    short                           numCols;                    /* number of columns per row */
    ColTypesHandle                  colTypes;                   /* data type array */
    Handle                          colData;                    /* actual results */
    ColInfoHandle                   colInfo;                    /* DBColInfoRecord array */
};
typedef struct ResultsRecord            ResultsRecord;
enum {
                                                                /* messages sent to a 'ddev'*/
    kDBInit                     = 0,
    kDBEnd                      = 1,
    kDBGetConnInfo              = 2,
    kDBGetSessionNum            = 3,
    kDBSend                     = 4,
    kDBSendItem                 = 5,
    kDBExec                     = 6,
    kDBState                    = 7,
    kDBGetErr                   = 8,
    kDBBreak                    = 9,
    kDBGetItem                  = 10,
    kDBUngetItem                = 11,
    kDBKill                     = 12,
    kDBOpen                     = 100,
    kDBClose                    = 101,
    kDBIdle                     = 102
};

typedef CALLBACK_API( OSErr , DBQueryDefProcPtr )(long *sessID, QueryHandle query);
typedef CALLBACK_API( Boolean , DBStatusProcPtr )(short message, OSErr result, short dataLen, short dataPlaces, short dataFlags, DBType dataType, Ptr dataPtr);
typedef CALLBACK_API( OSErr , DBResultHandlerProcPtr )(DBType dataType, short theLen, short thePlaces, short theFlags, Ptr theData, Handle theText);
typedef STACK_UPP_TYPE(DBQueryDefProcPtr)                       DBQueryDefUPP;
typedef STACK_UPP_TYPE(DBStatusProcPtr)                         DBStatusUPP;
typedef STACK_UPP_TYPE(DBResultHandlerProcPtr)                  DBResultHandlerUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(DBCompletionUPP)
    NewDBCompletionUPP             (DBCompletionProcPtr     userRoutine);

    EXTERN_API(DBQueryDefUPP)
    NewDBQueryDefUPP               (DBQueryDefProcPtr       userRoutine);

    EXTERN_API(DBStatusUPP)
    NewDBStatusUPP                 (DBStatusProcPtr         userRoutine);

    EXTERN_API(DBResultHandlerUPP)
    NewDBResultHandlerUPP          (DBResultHandlerProcPtr  userRoutine);

    EXTERN_API(void)
    DisposeDBCompletionUPP         (DBCompletionUPP         userUPP);

    EXTERN_API(void)
    DisposeDBQueryDefUPP           (DBQueryDefUPP           userUPP);

    EXTERN_API(void)
    DisposeDBStatusUPP             (DBStatusUPP             userUPP);

    EXTERN_API(void)
    DisposeDBResultHandlerUPP      (DBResultHandlerUPP      userUPP);

    EXTERN_API(void)
    InvokeDBCompletionUPP          (DBAsyncParmBlkPtr       pb,
                                    DBCompletionUPP         userUPP);

    EXTERN_API(OSErr)
    InvokeDBQueryDefUPP            (long *                  sessID,
                                    QueryHandle             query,
                                    DBQueryDefUPP           userUPP);

    EXTERN_API(Boolean)
    InvokeDBStatusUPP              (short                   message,
                                    OSErr                   result,
                                    short                   dataLen,
                                    short                   dataPlaces,
                                    short                   dataFlags,
                                    DBType                  dataType,
                                    Ptr                     dataPtr,
                                    DBStatusUPP             userUPP);

    EXTERN_API(OSErr)
    InvokeDBResultHandlerUPP       (DBType                  dataType,
                                    short                   theLen,
                                    short                   thePlaces,
                                    short                   theFlags,
                                    Ptr                     theData,
                                    Handle                  theText,
                                    DBResultHandlerUPP      userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppDBCompletionProcInfo = 0x0000B802 };                  /* register no_return_value Func(4_bytes:A1) */
    enum { uppDBQueryDefProcInfo = 0x000003E0 };                    /* pascal 2_bytes Func(4_bytes, 4_bytes) */
    enum { uppDBStatusProcInfo = 0x000FAA90 };                      /* pascal 1_byte Func(2_bytes, 2_bytes, 2_bytes, 2_bytes, 2_bytes, 4_bytes, 4_bytes) */
    enum { uppDBResultHandlerProcInfo = 0x0003EAE0 };               /* pascal 2_bytes Func(4_bytes, 2_bytes, 2_bytes, 2_bytes, 4_bytes, 4_bytes) */
    #define NewDBCompletionUPP(userRoutine)                         (DBCompletionUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDBCompletionProcInfo, GetCurrentArchitecture())
    #define NewDBQueryDefUPP(userRoutine)                           (DBQueryDefUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDBQueryDefProcInfo, GetCurrentArchitecture())
    #define NewDBStatusUPP(userRoutine)                             (DBStatusUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDBStatusProcInfo, GetCurrentArchitecture())
    #define NewDBResultHandlerUPP(userRoutine)                      (DBResultHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDBResultHandlerProcInfo, GetCurrentArchitecture())
    #define DisposeDBCompletionUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define DisposeDBQueryDefUPP(userUPP)                           DisposeRoutineDescriptor(userUPP)
    #define DisposeDBStatusUPP(userUPP)                             DisposeRoutineDescriptor(userUPP)
    #define DisposeDBResultHandlerUPP(userUPP)                      DisposeRoutineDescriptor(userUPP)
    #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
    #pragma parameter InvokeDBCompletionUPP(__A1, __A0)
    void InvokeDBCompletionUPP(DBAsyncParmBlkPtr pb, DBCompletionUPP userUPP) = 0x4E90;
    #else
        #define InvokeDBCompletionUPP(pb, userUPP)                      CALL_ONE_PARAMETER_UPP((userUPP), uppDBCompletionProcInfo, (pb))
    #endif
    #define InvokeDBQueryDefUPP(sessID, query, userUPP)             (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppDBQueryDefProcInfo, (sessID), (query))
    #define InvokeDBStatusUPP(message, result, dataLen, dataPlaces, dataFlags, dataType, dataPtr, userUPP)  (Boolean)CALL_SEVEN_PARAMETER_UPP((userUPP), uppDBStatusProcInfo, (message), (result), (dataLen), (dataPlaces), (dataFlags), (dataType), (dataPtr))
    #define InvokeDBResultHandlerUPP(dataType, theLen, thePlaces, theFlags, theData, theText, userUPP)  (OSErr)CALL_SIX_PARAMETER_UPP((userUPP), uppDBResultHandlerProcInfo, (dataType), (theLen), (thePlaces), (theFlags), (theData), (theText))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewDBCompletionProc(userRoutine)                        NewDBCompletionUPP(userRoutine)
#define NewDBQueryDefProc(userRoutine)                          NewDBQueryDefUPP(userRoutine)
#define NewDBStatusProc(userRoutine)                            NewDBStatusUPP(userRoutine)
#define NewDBResultHandlerProc(userRoutine)                     NewDBResultHandlerUPP(userRoutine)
#define CallDBCompletionProc(userRoutine, pb)                   InvokeDBCompletionUPP(pb, userRoutine)
#define CallDBQueryDefProc(userRoutine, sessID, query)          InvokeDBQueryDefUPP(sessID, query, userRoutine)
#define CallDBStatusProc(userRoutine, message, result, dataLen, dataPlaces, dataFlags, dataType, dataPtr) InvokeDBStatusUPP(message, result, dataLen, dataPlaces, dataFlags, dataType, dataPtr, userRoutine)
#define CallDBResultHandlerProc(userRoutine, dataType, theLen, thePlaces, theFlags, theData, theText) InvokeDBResultHandlerUPP(dataType, theLen, thePlaces, theFlags, theData, theText, userRoutine)
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
InitDBPack                      (void)                                                      FIVEWORDINLINE(0x3F3C, 0x0004, 0x303C, 0x0100, 0xA82F);

EXTERN_API( OSErr )
DBInit                          (long *                 sessID,
                                 ConstStr63Param        ddevName,
                                 ConstStr255Param       host,
                                 ConstStr255Param       user,
                                 ConstStr255Param       passwd,
                                 ConstStr255Param       connStr,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x0E02, 0xA82F);

EXTERN_API( OSErr )
DBEnd                           (long                   sessID,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x0403, 0xA82F);

EXTERN_API( OSErr )
DBGetConnInfo                   (long                   sessID,
                                 short                  sessNum,
                                 long *                 returnedID,
                                 long *                 version,
                                 Str63                  ddevName,
                                 Str255                 host,
                                 Str255                 user,
                                 Str255                 network,
                                 Str255                 connStr,
                                 long *                 start,
                                 OSErr *                state,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x1704, 0xA82F);

EXTERN_API( OSErr )
DBGetSessionNum                 (long                   sessID,
                                 short *                sessNum,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x0605, 0xA82F);

EXTERN_API( OSErr )
DBSend                          (long                   sessID,
                                 Ptr                    text,
                                 short                  len,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x0706, 0xA82F);

EXTERN_API( OSErr )
DBSendItem                      (long                   sessID,
                                 DBType                 dataType,
                                 short                  len,
                                 short                  places,
                                 short                  flags,
                                 void *                 buffer,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x0B07, 0xA82F);

EXTERN_API( OSErr )
DBExec                          (long                   sessID,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x0408, 0xA82F);

EXTERN_API( OSErr )
DBState                         (long                   sessID,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x0409, 0xA82F);

EXTERN_API( OSErr )
DBGetErr                        (long                   sessID,
                                 long *                 err1,
                                 long *                 err2,
                                 Str255                 item1,
                                 Str255                 item2,
                                 Str255                 errorMsg,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x0E0A, 0xA82F);

EXTERN_API( OSErr )
DBBreak                         (long                   sessID,
                                 Boolean                abort,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x050B, 0xA82F);

EXTERN_API( OSErr )
DBGetItem                       (long                   sessID,
                                 long                   timeout,
                                 DBType *               dataType,
                                 short *                len,
                                 short *                places,
                                 short *                flags,
                                 void *                 buffer,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x100C, 0xA82F);

EXTERN_API( OSErr )
DBUnGetItem                     (long                   sessID,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x040D, 0xA82F);

EXTERN_API( OSErr )
DBKill                          (DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x020E, 0xA82F);

EXTERN_API( OSErr )
DBGetNewQuery                   (short                  queryID,
                                 QueryHandle *          query)                              THREEWORDINLINE(0x303C, 0x030F, 0xA82F);

EXTERN_API( OSErr )
DBDisposeQuery                  (QueryHandle            query)                              THREEWORDINLINE(0x303C, 0x0210, 0xA82F);

EXTERN_API( OSErr )
DBStartQuery                    (long *                 sessID,
                                 QueryHandle            query,
                                 DBStatusUPP            statusProc,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x0811, 0xA82F);

EXTERN_API( OSErr )
DBGetQueryResults               (long                   sessID,
                                 ResultsRecord *        results,
                                 long                   timeout,
                                 DBStatusUPP            statusProc,
                                 DBAsyncParmBlkPtr      asyncPB)                            THREEWORDINLINE(0x303C, 0x0A12, 0xA82F);

EXTERN_API( OSErr )
DBResultsToText                 (ResultsRecord *        results,
                                 Handle *               theText)                            THREEWORDINLINE(0x303C, 0x0413, 0xA82F);

EXTERN_API( OSErr )
DBInstallResultHandler          (DBType                 dataType,
                                 DBResultHandlerUPP     theHandler,
                                 Boolean                isSysHandler)                       THREEWORDINLINE(0x303C, 0x0514, 0xA82F);

EXTERN_API( OSErr )
DBRemoveResultHandler           (DBType                 dataType)                           THREEWORDINLINE(0x303C, 0x0215, 0xA82F);

EXTERN_API( OSErr )
DBGetResultHandler              (DBType                 dataType,
                                 DBResultHandlerUPP *   theHandler,
                                 Boolean                getSysHandler)                      THREEWORDINLINE(0x303C, 0x0516, 0xA82F);

EXTERN_API( OSErr )
DBIdle                          (void)                                                      THREEWORDINLINE(0x303C, 0x00FF, 0xA82F);



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

#endif /* __DATABASEACCESS__ */

