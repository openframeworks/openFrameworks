/*
     File:       NetworkSetup.h
 
     Contains:   Network Setup Interfaces
 
     Version:    Technology: 1.1.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1998-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __NETWORKSETUP__
#define __NETWORKSETUP__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __OPENTRANSPORT__
#include <OpenTransport.h>
#endif

#ifndef __OPENTRANSPORTPROVIDERS__
#include <OpenTransportProviders.h>
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

#if CALL_NOT_IN_CARBON
#ifndef __NETWORKSETUPTYPES__
#define __NETWORKSETUPTYPES__
typedef struct OpaqueCfgDatabaseRef*    CfgDatabaseRef;
typedef UInt32                          CfgAreaID;
typedef OSType                          CfgEntityClass;
typedef OSType                          CfgEntityType;

struct CfgEntityRef {
    CfgAreaID                       fLoc;
    UInt32                          fReserved;
    Str255                          fID;
};
typedef struct CfgEntityRef             CfgEntityRef;

struct CfgResourceLocator {
    FSSpec                          fFile;
    UInt16                          fResID;
};
typedef struct CfgResourceLocator       CfgResourceLocator;

struct CfgEntityInfo {
    CfgEntityClass                  fClass;
    CfgEntityType                   fType;
    Str255                          fName;
    CfgResourceLocator              fIcon;
};
typedef struct CfgEntityInfo            CfgEntityInfo;

typedef void *                          CfgEntityAccessID;

struct CfgPrefsHeader {
    UInt16                          fSize;                      /* size includes this header*/
    UInt16                          fVersion;
    OSType                          fType;
};
typedef struct CfgPrefsHeader           CfgPrefsHeader;
/*    -------------------------------------------------------------------------
    Error codes
    ------------------------------------------------------------------------- */
enum {
    kCfgErrDatabaseChanged      = -3290,                        /* database has changed since last call - close and reopen DB*/
    kCfgErrAreaNotFound         = -3291,                        /* Area doesn't exist*/
    kCfgErrAreaAlreadyExists    = -3292,                        /* Area already exists*/
    kCfgErrAreaNotOpen          = -3293,                        /* Area needs to open first*/
    kCfgErrConfigLocked         = -3294,                        /* Access conflict - retry later*/
    kCfgErrEntityNotFound       = -3295,                        /* An entity with this name doesn't exist*/
    kCfgErrEntityAlreadyExists  = -3296,                        /* An entity with this name already exists*/
    kCfgErrPrefsTypeNotFound    = -3297,                        /* An record with this PrefsType doesn't exist*/
    kCfgErrDataTruncated        = -3298,                        /* Data truncated when read buffer too small*/
    kCfgErrFileCorrupted        = -3299                         /* The database format appears to be corrupted.*/
};

/*  reserve a 'free' tag for free blocks*/
enum {
    kCfgTypefree                = FOUR_CHAR_CODE('free')
};

/*    -------------------------------------------------------------------------
    CfgEntityClass / CfgEntityType

    The database can distinguish between several classes of objects and 
    several types withing each class
    Use of different classes allow to store type of information in the same database

    Other entity classes and types can be defined by developers.
    they should be unique and registered with Developer Tech Support (DTS)
    ------------------------------------------------------------------------- */
enum {
    kCfgClassAnyEntity          = FOUR_CHAR_CODE('****'),
    kCfgClassUnknownEntity      = FOUR_CHAR_CODE('????'),
    kCfgTypeAnyEntity           = FOUR_CHAR_CODE('****'),
    kCfgTypeUnknownEntity       = FOUR_CHAR_CODE('????')
};

/*    -------------------------------------------------------------------------
    For CfgIsSameEntityRef
    ------------------------------------------------------------------------- */
enum {
    kCfgIgnoreArea              = true,
    kCfgDontIgnoreArea          = false
};

#endif  /* __NETWORKSETUPTYPES__ */
/*******************************************************************************
**  Configuration Information Access API 
********************************************************************************/
/*    -------------------------------------------------------------------------
    Database access
    ------------------------------------------------------------------------- */
#if CALL_NOT_IN_CARBON
EXTERN_API( OSStatus )
OTCfgOpenDatabase               (CfgDatabaseRef *       dbRef);

/*
    OTCfgOpenDatabase()

    Inputs:     none
    Outputs:    CfgDatabaseRef* dbRef           Reference to opened database
    Returns:    OSStatus                        *** list errors ***

    Opens the Configuration API for a given client. This call should be made prior to any other call.
*/
EXTERN_API( OSStatus )
OTCfgCloseDatabase              (CfgDatabaseRef *       dbRef);

/*
    OTCfgCloseDatabase()

    Inputs:     CfgDatabaseRef* dbRef           Reference to opened database
    Outputs:    CfgDatabaseRef* dbRef           Reference to opened database is cleared
    Returns:    OSStatus                        *** list errors ***

    Closes the Configuration API for a given client. This call should be made when the client no 
    longer wants to use the Configuration API.  
*/
/*    -------------------------------------------------------------------------
    Area management
    ------------------------------------------------------------------------- */
EXTERN_API( OSStatus )
OTCfgGetAreasCount              (CfgDatabaseRef         dbRef,
                                 ItemCount *            itemCount);

/*
    OTCfgGetAreasCount()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
    Outputs:    ItemCount* itemCount            Number of entities defined
    Returns:    OSStatus                        *** list errors ***

    Returns the number of areas currently defined.
*/
EXTERN_API( OSStatus )
OTCfgGetAreasList               (CfgDatabaseRef         dbRef,
                                 ItemCount *            itemCount,
                                 CfgAreaID              areaID[],
                                 Str255                 areaName[]);

/*
    OTCfgGetAreasList()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                ItemCount* itemCount            Number of entities requested
    Outputs:    ItemCount* itemCount            Number of entities defined
    Returns:    OSStatus                        *** list errors ***

    Returns a list of area IDs and names. On entry, count should be set to whatever OTCfgGetAreasCount 
    returned.  On exit, count contains the actual number of areas found. This can be less than the 
    initial count value if areas were deleted in the meantime.  The id and name parameters are stored 
    in arrays that should each be able to contain count values.
*/
EXTERN_API( OSStatus )
OTCfgGetCurrentArea             (CfgDatabaseRef         dbRef,
                                 CfgAreaID *            areaID);

/*
    OTCfgGetCurrentArea()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
    Outputs:    CfgAreaID* areaID               ID of current area
    Returns:    OSStatus                        *** list errors ***

    Returns the id of the current area.
*/
EXTERN_API( OSStatus )
OTCfgSetCurrentArea             (CfgDatabaseRef         dbRef,
                                 CfgAreaID              areaID);

/*
    OTCfgSetCurrentArea()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID areaID                ID of area to make active
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Sets the current area. If the area doesn't exist kCfgErrAreaNotFound is returned.
*/
EXTERN_API( OSStatus )
OTCfgCreateArea                 (CfgDatabaseRef         dbRef,
                                 ConstStr255Param       areaName,
                                 CfgAreaID *            areaID);

/*
    OTCfgCreateArea()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                ConstStr255Param areaName       Name of area to create
    Outputs:    CfgAreaID* areaID               ID of newly created area
    Returns:    OSStatus                        *** list errors ***

    Creates a new area with the specified name. Then name must be unique or kCfgErrAreaAlreadyExists 
    will be returned.
*/
EXTERN_API( OSStatus )
OTCfgDeleteArea                 (CfgDatabaseRef         dbRef,
                                 CfgAreaID              areaID);

/*
    OTCfgDeleteArea()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID areaID                ID of area to delete
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Deletes the specified area. If the area doesn't exist kCfgErrAreaNotFound is returned.
*/
EXTERN_API( OSStatus )
OTCfgDuplicateArea              (CfgDatabaseRef         dbRef,
                                 CfgAreaID              sourceAreaID,
                                 CfgAreaID              destAreaID);

/*
    OTCfgDuplicateArea()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID sourceAreaID          Area to duplicate
                CfgAreaID destAreaID            Area to contain duplicate
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Duplicates the source area content into the destination area. Both areas should exist prior to 
    making this call. If either area doesn't exist kCfgErrAreaNotFound is returned.
*/
EXTERN_API( OSStatus )
OTCfgSetAreaName                (CfgDatabaseRef         dbRef,
                                 CfgAreaID              areaID,
                                 ConstStr255Param       areaName,
                                 CfgAreaID *            newAreaID);

/*
    OTCfgSetAreaName()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID areaID                ID of area being named
                ConstStr255Param areaName       New name for area
    Outputs:    CfgAreaID* newAreaID            ID of renamed area
    Returns:    OSStatus                        *** list errors ***

    Renames the specified area. A new id is returned: it should be used from now on. If the area 
    doesn't exist kCfgErrAreaNotFound is returned.
*/
EXTERN_API( OSStatus )
OTCfgGetAreaName                (CfgDatabaseRef         dbRef,
                                 CfgAreaID              areaID,
                                 Str255                 areaName);

/*
    OTCfgGetAreaName()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID areaID                ID of area being queried
    Outputs:    Str255 areaName                 Name of area
    Returns:    OSStatus                        *** list errors ***

    Gets the name of the specified area. If the area doesn't exist kCfgErrAreaNotFound is returned.
*/
/*    -------------------------------------------------------------------------
    Configuration Database API
    
    Single Writer ONLY!!!
    ------------------------------------------------------------------------- */
/*    -------------------------------------------------------------------------
    Opening an area for reading
    ------------------------------------------------------------------------- */
EXTERN_API( OSStatus )
OTCfgOpenArea                   (CfgDatabaseRef         dbRef,
                                 CfgAreaID              areaID);

/*
    OTCfgOpenArea()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID areaID                ID of area to open
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Opens the specified area for reading. If the area doesn't exist kCfgErrAreaNotFound is returned.
*/
EXTERN_API( OSStatus )
OTCfgCloseArea                  (CfgDatabaseRef         dbRef,
                                 CfgAreaID              areaID);

/*
    OTCfgCloseArea()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID areaID                ID of area to close
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Closes an area opened for reading. If the area doesn't exist kCfgErrAreaNotFound is returned.  
    Opening an area for writing All modifications to an area should be performed as part of a 
    transaction.
*/
/*
    For write access
*/
EXTERN_API( OSStatus )
OTCfgBeginAreaModifications     (CfgDatabaseRef         dbRef,
                                 CfgAreaID              readAreaID,
                                 CfgAreaID *            writeAreaID);

/*
    OTCfgBeginAreaModifications()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID readAreaID            ID of area opened for reading
    Outputs:    CfgAreaID* writeAreaID          ID of area opened for modification
    Returns:    OSStatus                        *** list errors ***

    Opens the specified area for writing. A new area id is provided.  This area id should be used to 
    enumerate, add, delete, read and write to the modified data. The original id can still be used to 
    access the original unmodified data. If the area doesn't exist kCfgErrAreaNotFound is returned.
*/
EXTERN_API( OSStatus )
OTCfgCommitAreaModifications    (CfgDatabaseRef         dbRef,
                                 CfgAreaID              readAreaID,
                                 CfgAreaID              writeAreaID);

/*
    OTCfgCommitAreaModifications()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID readAreaID            ID of area opened for reading
                CfgAreaID writeAreaID           ID of area opened for modification
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Closes an area opened for writing.  All modifications are committed and readers are informed that 
    the database changed state ( kCfgStateChangedErr ). The areaID should be the id of the original 
    area.  If the area doesn't exist or the wrong id is passed, kCfgErrAreaNotFound is returned.
*/
EXTERN_API( OSStatus )
OTCfgAbortAreaModifications     (CfgDatabaseRef         dbRef,
                                 CfgAreaID              readAreaID);

/*
    OTCfgAbortAreaModifications()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID readAreaID            ID of area opened for reading
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Closes an area opened for writing, discarding any modification. The areaID should be the id of 
    the original area. If the area doesn't exist or the wrong id is passed kCfgErrAreaNotFound is 
    returned.
*/
/*
    Working with entities

    Entities can be manipulated as soon as an area has been opened.  The same calls work both for 
    areas opened for reading or for modification. In the latter case, the calls can be used on the 
    original or new area id to access the original data or the modified data.
*/
/*
    For everybody
    Count receives the actual number of entities
*/
EXTERN_API( OSStatus )
OTCfgGetEntitiesCount           (CfgDatabaseRef         dbRef,
                                 CfgAreaID              areaID,
                                 CfgEntityClass         entityClass,
                                 CfgEntityType          entityType,
                                 ItemCount *            itemCount);

/*
    OTCfgGetEntitiesCount()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID areaID                ID of area to count
                CfgEntityClass entityClass      Class of entities to count
                CfgEntityType entityType        Type of entities to count
    Outputs:    ItemCount* itemCount            Count of matching entities
    Returns:    OSStatus                        *** list errors ***

    Returns the number of entities of the specified class and type in the specified area. To obtain 
    all entities regardless of their class or type pass kCfgClassAnyEntity or kCfgTypeAnyEntity. If 
    the area doesn't exist or the wrong id is passed kCfgErrAreaNotFound is returned.
*/

/*
    Count as input, is the number of entities to read;
    count as output, receives the actual number of entities or the number you specified. 
*/
EXTERN_API( OSStatus )
OTCfgGetEntitiesList            (CfgDatabaseRef         dbRef,
                                 CfgAreaID              areaID,
                                 CfgEntityClass         entityClass,
                                 CfgEntityType          entityType,
                                 ItemCount *            itemCount,
                                 CfgEntityRef           entityRef[],
                                 CfgEntityInfo          entityInfo[]);

/*
    OTCfgGetEntitiesList()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID areaID                ID of area to list
                CfgEntityClass entityClass      Class of entities to list
                CfgEntityType entityType        Type of entities to list
                ItemCount* itemCount            Count of entities requested
    Outputs:    ItemCount* itemCount            Count of entities listed
    Returns:    OSStatus                        *** list errors ***

    Returns the list of entities of the specified class and type in the specified area. To obtain all 
    entities regardless of their class or type pass kCfgClassAnyEntity or kCfgTypeAnyEntity. The 
    count parameter should have the value obtained by CfgGetEntitiesCount.  On exit count may be less 
    if some entities were deleted in the meantime. The id and info parameters should be arrays large 
    enough to hold count entries. If the area doesn't exist or the wrong id is passed 
    kCfgErrAreaNotFound is returned.  The info array contains information about each entity, 
    including its class, type, name and the area of its icon:

    struct CfgEntityInfo
    {
        CfgEntityClass      fClass;
        CfgEntityType       fType;
        ConstStr255Param    fName;
        CfgResourceLocator  fIcon;
    };
*/
EXTERN_API( OSStatus )
OTCfgCreateEntity               (CfgDatabaseRef         dbRef,
                                 CfgAreaID              areaID,
                                 const CfgEntityInfo *  entityInfo,
                                 CfgEntityRef *         entityRef);

/*
    OTCfgCreateEntity()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgAreaID areaID                ID of area to contain entity
                CfgEntityInfo* entityInfo       Information that defines the entity
    Outputs:    CfgEntityRef* entityRef         Reference to entity created
    Returns:    OSStatus                        *** list errors ***

    Creates a new entity with the specified class, type and name and returns an id for it. If the 
    area doesn't exist or the wrong id is passed kCfgErrAreaNotFound is returned. If there is already 
    an entity with the same name kCfgErrEntityAlreadyExists is returned.
*/
EXTERN_API( OSStatus )
OTCfgDeleteEntity               (CfgDatabaseRef         dbRef,
                                 const CfgEntityRef *   entityRef);

/*
    OTCfgDeleteEntity()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgEntityRef* entityRef         Reference to entity to delete
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Deletes the specified entity. If there is no entity with this id kCfgEntityNotfoundErr is returned
*/
EXTERN_API( OSStatus )
OTCfgDuplicateEntity            (CfgDatabaseRef         dbRef,
                                 const CfgEntityRef *   entityRef,
                                 const CfgEntityRef *   newEntityRef);

/*
    OTCfgDuplicateEntity()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgEntityRef* entityRef         Reference to entity to duplicate
    Outputs:    CfgEntityRef* newEntityRef      Reference to duplicate entity
    Returns:    OSStatus                        *** list errors ***

    Duplicates the specified entity. Both entities should exit. If any entity doesn't exist 
    kCfgErrEntityNotFound is returned.
*/
EXTERN_API( OSStatus )
OTCfgSetEntityName              (CfgDatabaseRef         dbRef,
                                 const CfgEntityRef *   entityRef,
                                 ConstStr255Param       entityName,
                                 CfgEntityRef *         newEntityRef);

/*
    OTCfgSetEntityName()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgEntityRef* entityRef         Reference to entity to duplicate
                ConstStr255Param entityName     New name for entity
    Outputs:    CfgEntityRef* newEntityRef      Reference to renamed entity
    Returns:    OSStatus                        *** list errors ***

    Renames the specified entity. If the entity doesn't exist kCfgEntityNotfoundErr is returned. If 
    there is already an entity with that name kCfgErrEntityAlreadyExists is returned.
*/
EXTERN_API( void )
OTCfgGetEntityArea              (const CfgEntityRef *   entityRef,
                                 CfgAreaID *            areaID);

/*
    OTCfgGetEntityArea()

    Inputs:     CfgEntityRef *entityRef         Reference to an entity
    Outputs:    CfgAreaID *areaID               ID of area that contains the entity
    Returns:    none

    Returns the area ID associated with the specified entity reference.
*/
EXTERN_API( void )
OTCfgGetEntityName              (const CfgEntityRef *   entityRef,
                                 Str255                 entityName);

/*
    OTCfgGetEntityName()

    Inputs:     CfgEntityRef *entityRef         Reference to an entity
    Outputs:    Str255 entityName               Name of the entity
    Returns:    none

    Returns the entity name associated with the specified entity reference.
*/
EXTERN_API( void )
OTCfgChangeEntityArea           (CfgEntityRef *         entityRef,
                                 CfgAreaID              newAreaID);

/*
    OTCfgChangeEntityArea()

    Inputs:     CfgEntityRef *entityRef         Reference to an entity
                CfgAreaID newAreaID             ID of area to contain moved entity
    Outputs:    none
    Returns:    none

    Changes the area ID associated with the specified entity reference. This effectively moves the 
    entity to a different area.
*/
/*    -------------------------------------------------------------------------
    These API calls are for the protocol developers to compare the IDs.
    ------------------------------------------------------------------------- */
/*    -------------------------------------------------------------------------
    For OTCfgIsSameEntityRef
    ------------------------------------------------------------------------- */
#endif  /* CALL_NOT_IN_CARBON */

enum {
    kOTCfgIgnoreArea            = kCfgIgnoreArea,
    kOTCfgDontIgnoreArea        = kCfgDontIgnoreArea
};

#if CALL_NOT_IN_CARBON
EXTERN_API( Boolean )
OTCfgIsSameEntityRef            (const CfgEntityRef *   entityRef1,
                                 const CfgEntityRef *   entityRef2,
                                 Boolean                ignoreArea);

/*
    OTCfgIsSameEntityRef()

    Inputs:     CfgEntityRef* entityRef1        Reference to an entity
                CfgEntityRef* entityRef2        Reference to another entity
                Boolean ignoreArea              If true, ignore the area ID
    Outputs:    none
    Returns:    Boolean                         If true, entity references match

    Compare two entity references. If ignoreArea is true, and the two entity names are the same, then return 
    true. If ignoreArea is false, then the area IDs must be the same, as well as the entity names 
    must be the same, then can return true.
*/
EXTERN_API( Boolean )
OTCfgIsSameAreaID               (CfgAreaID              areaID1,
                                 CfgAreaID              areaID2);

/*
    OTCfgIsSameAreaID()

    Inputs:     CfgAreaID areaID1               ID of an area
                CfgAreaID areaID2               ID of another area
    Outputs:    none
    Returns:    Boolean                         If true, area IDs match

    Compare two area IDs. Return true for matching area IDs, and return false for the different area IDs.
*/
/*    -------------------------------------------------------------------------
    Dealing with individual preferences
    ------------------------------------------------------------------------- */
/*    -------------------------------------------------------------------------
    Open Preferences
    if writer = true, GetPrefs and SetPrefs are allowed, else only GetPrefs is allowed.
    ------------------------------------------------------------------------- */
EXTERN_API( OSStatus )
OTCfgOpenPrefs                  (CfgDatabaseRef         dbRef,
                                 const CfgEntityRef *   entityRef,
                                 Boolean                writer,
                                 CfgEntityAccessID *    accessID);

/*
    OTCfgOpenPrefs()

    Inputs:     CfgDatabaseRef dbRef            Reference to opened database
                CfgEntityRef* entityRef         Reference to an entity
                Boolean writer              If true, open for write
    Outputs:    CfgEntityAccessID* accessID     ID for entity access
    Returns:    OSStatus                        *** list errors ***

    Open the specified entity and return the CfgEntityAccessID for the following access of the 
    content of the entity. If writer is true, CfgGetPrefs and CfgSetPrefs are allowed, otherwise only 
    CfgGetPrefs is allowed.
*/
EXTERN_API( OSStatus )
OTCfgClosePrefs                 (CfgEntityAccessID      accessID);

/*
    OTCfgClosePrefs()

    Inputs:     CfgEntityAccessID* accessID     ID for entity to close
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Close the entity with the specified CfgEntityAccessID.
*/
/*    -------------------------------------------------------------------------
    Get/Set Preferences

    Accessing the content of an entity

    These API calls are for the protocol developers. It supports multiple records per entity. Each 
    record is identified by the prefsType and the size of the record. The protocol stack will provide 
    the STRUCT to view the content of each record.
    ------------------------------------------------------------------------- */
EXTERN_API( OSStatus )
OTCfgSetPrefs                   (CfgEntityAccessID      accessID,
                                 OSType                 prefsType,
                                 const void *           data,
                                 ByteCount              length);

/*
    OTCfgSetPrefs()

    Inputs:     CfgEntityAccessID* accessID     ID of entity to access
                OSType prefsType                Record type to set
                void* data                      Address of data
                ByteCount length                Number of bytes of data
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Write the data to the specified record. The record is identified by the prefsType. If the entity 
    is not opened for the writer, an error code is returned.
*/
EXTERN_API( OSStatus )
OTCfgGetPrefs                   (CfgEntityAccessID      accessID,
                                 OSType                 prefsType,
                                 void *                 data,
                                 ByteCount              length);

/*
    OTCfgGetPrefs()

    Inputs:     CfgEntityAccessID* accessID     ID of entity to access
                OSType prefsType                Record type to get
                void* data                      Address for data
                ByteCount length                Number of bytes of data requested
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Read the data from the specified record to the passed buffer. The record is identified by the 
    prefsType. If the passed buffer is too small, kCfgErrDataTruncated is returned, but will copy as 
    many data as possible to the buffer.
*/
EXTERN_API( OSStatus )
OTCfgGetPrefsSize               (CfgEntityAccessID      accessID,
                                 OSType                 prefsType,
                                 ByteCount *            length);

/*
    OTCfgGetPrefsSize()

    Inputs:     CfgEntityAccessID* accessID     ID of entity to access
                OSType prefsType                Record type to get
                ByteCount length                Number of bytes of data available
    Outputs:    none
    Returns:    OSStatus                        *** list errors ***

    Returns the length, in bytes, of the specified record. The record is identified by the prefsType.
*/
/*    -------------------------------------------------------------------------
    Get table of contents for prefs
    ------------------------------------------------------------------------- */
EXTERN_API( OSStatus )
OTCfgGetPrefsTOCCount           (CfgEntityAccessID      accessID,
                                 ItemCount *            itemCount);

/*
    OTCfgGetPrefsTOCCount()

    Inputs:     CfgEntityAccessID* accessID     ID of entity to access
    Outputs:    ItemCount* itemCount            Number of entries available
    Returns:    OSStatus                        *** list errors ***

    Get the count of all the record headers in the entity. Return the number of records in the count. 
*/
EXTERN_API( OSStatus )
OTCfgGetPrefsTOC                (CfgEntityAccessID      accessID,
                                 ItemCount *            itemCount,
                                 CfgPrefsHeader         PrefsTOC[]);

/*
    OTCfgGetPrefsTOC()

    Inputs:     CfgEntityAccessID* accessID     ID of entity to access
                ItemCount* itemCount            Number of entries requested
    Outputs:    ItemCount* itemCount            Number of entries available
                CfgPrefsHeader PrefsTOC[]       Table of entries
    Returns:    OSStatus                        *** list errors ***

    Get the list of all the record headers in the entity. Return the number of records in the count. 
    If the PrefsTOC is specified, it has to be big enough to hold all the record headers. If the 
    PrefsTOC is null, only the count is returned.
*/
EXTERN_API( SInt16 )
OTCfgEncrypt                    (UInt8 *                key,
                                 UInt8 *                data,
                                 SInt16                 dataLen);

/*
    OTCfgEncrypt()

    Inputs:     UInt8 *key                      encryption key ( user name )
                UInt8 *data                     data to encrypt ( password )
                SInt16 dataLen                  length of data to encrypt
    Outputs:    UInt8 *data                     encrypted data
    Returns:    SInt16                          length of encrypted data

    Encrypt the password, using the user name as the encryption key.  Return the encrypted password and its length.  
*/
EXTERN_API( SInt16 )
OTCfgDecrypt                    (UInt8 *                key,
                                 UInt8 *                data,
                                 SInt16                 dataLen);

/*
    OTCfgDecrypt()

    Inputs:     UInt8 *key                      encryption key ( user name )
                UInt8 *data                     data to decrypt ( password )
                SInt16 dataLen                  length of data to decrypt
    Outputs:    UInt8 *data                     decrypted data
    Returns:    SInt16                          length of decrypted data

    Decrypt the password, using the user name as the encryption key.  Return the decrypted password and its length.  
*/
EXTERN_API( Handle )
OTCfgGetDefault                 (ResType                entityType,
                                 ResType                entityClass,
                                 ResType                recordType);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API( OSStatus )
OTCfgInstallNotifier            (CfgDatabaseRef         dbRef,
                                 CfgEntityClass         theClass,
                                 CfgEntityType          theType,
                                 OTNotifyProcPtr        notifier,
                                 void *                 contextPtr);

EXTERN_API( OSStatus )
OTCfgRemoveNotifier             (CfgDatabaseRef         dbRef,
                                 CfgEntityClass         theClass,
                                 CfgEntityType          theType);

#endif  /* CALL_NOT_IN_CARBON */


typedef UInt16                          OTCfgIRPortSetting;
enum {
    kOTCfgTypeStruct            = FOUR_CHAR_CODE('stru'),
    kOTCfgTypeElement           = FOUR_CHAR_CODE('elem'),
    kOTCfgTypeVector            = FOUR_CHAR_CODE('vect')
};

/*    -------------------------------------------------------------------------
    CfgEntityClass / CfgEntityType

    The database can distinguish between several classes of objects and 
    several types withing each class
    Use of different classes allow to store type of information in the same database

    Other entity classes and types can be defined by developers.
    they should be unique and registered with Developer Tech Support (DTS)
    ------------------------------------------------------------------------- */
enum {
    kOTCfgClassNetworkConnection = FOUR_CHAR_CODE('otnc'),
    kOTCfgClassGlobalSettings   = FOUR_CHAR_CODE('otgl'),
    kOTCfgClassServer           = FOUR_CHAR_CODE('otsv'),
    kOTCfgTypeGeneric           = FOUR_CHAR_CODE('otan'),
    kOTCfgTypeAppleTalk         = FOUR_CHAR_CODE('atlk'),
    kOTCfgTypeTCPv4             = FOUR_CHAR_CODE('tcp4'),
    kOTCfgTypeTCPv6             = FOUR_CHAR_CODE('tcp6'),
    kOTCfgTypeRemote            = FOUR_CHAR_CODE('ara '),
    kOTCfgTypeDial              = FOUR_CHAR_CODE('dial'),
    kOTCfgTypeModem             = FOUR_CHAR_CODE('modm'),
    kOTCfgTypeInfrared          = FOUR_CHAR_CODE('infr'),
    kOTCfgClassSetOfSettings    = FOUR_CHAR_CODE('otsc'),
    kOTCfgTypeSetOfSettings     = FOUR_CHAR_CODE('otst'),
    kOTCfgTypeDNS               = FOUR_CHAR_CODE('dns ')
};

/*******************************************************************************
** Preferences Structures
********************************************************************************/
enum {
    kOTCfgIndexSetsActive       = 0,
    kOTCfgIndexSetsEdit         = 1,
    kOTCfgIndexSetsLimit        = 2                             /*    last value, no comma*/
};


struct CfgSetsStruct {
    UInt32                          fFlags;
    UInt32                          fTimes[2];
};
typedef struct CfgSetsStruct            CfgSetsStruct;

struct CfgSetsElement {
    CfgEntityRef                    fEntityRef;
    CfgEntityInfo                   fEntityInfo;
};
typedef struct CfgSetsElement           CfgSetsElement;

struct CfgSetsVector {
    UInt32                          fCount;
    CfgSetsElement                  fElements[1];
};
typedef struct CfgSetsVector            CfgSetsVector;
/*  Common  */
enum {
                                                                /*    connection */
    kOTCfgTypeConfigName        = FOUR_CHAR_CODE('cnam'),
    kOTCfgTypeConfigSelected    = FOUR_CHAR_CODE('ccfg'),       /*    transport options  */
    kOTCfgTypeUserLevel         = FOUR_CHAR_CODE('ulvl'),
    kOTCfgTypeWindowPosition    = FOUR_CHAR_CODE('wpos')
};

/*  AppleTalk   */
enum {
                                                                /*    connection */
    kOTCfgTypeAppleTalkPrefs    = FOUR_CHAR_CODE('atpf'),
    kOTCfgTypeAppleTalkVersion  = FOUR_CHAR_CODE('cvrs'),
    kOTCfgTypeAppleTalkLocks    = FOUR_CHAR_CODE('lcks'),
    kOTCfgTypeAppleTalkPort     = FOUR_CHAR_CODE('port'),
    kOTCfgTypeAppleTalkProtocol = FOUR_CHAR_CODE('prot'),
    kOTCfgTypeAppleTalkPassword = FOUR_CHAR_CODE('pwrd'),
    kOTCfgTypeAppleTalkPortFamily = FOUR_CHAR_CODE('ptfm')      /*    transport options  */
};

enum {
    kOTCfgIndexAppleTalkAARP    = 0,
    kOTCfgIndexAppleTalkDDP     = 1,
    kOTCfgIndexAppleTalkNBP     = 2,
    kOTCfgIndexAppleTalkZIP     = 3,
    kOTCfgIndexAppleTalkATP     = 4,
    kOTCfgIndexAppleTalkADSP    = 5,
    kOTCfgIndexAppleTalkPAP     = 6,
    kOTCfgIndexAppleTalkASP     = 7,
    kOTCfgIndexAppleTalkLast    = 7
};


struct OTCfgAppleTalkPrefs {
    UInt16                          fVersion;
    UInt16                          fNumPrefs;
    OTPortRef                       fPort;
    void *                          fLink;
    void *                          fPrefs[8];
};
typedef struct OTCfgAppleTalkPrefs      OTCfgAppleTalkPrefs;

struct OTCfgAARPPrefs {
    UInt16                          fVersion;
    UInt16                          fSize;
    UInt32                          fAgingCount;
    UInt32                          fAgingInterval;
    OTByteCount                     fProtAddrLen;
    OTByteCount                     fHWAddrLen;
    UInt32                          fMaxEntries;
    OTByteCount                     fProbeInterval;
    OTByteCount                     fProbeRetryCount;
    OTByteCount                     fRequestInterval;
    OTByteCount                     fRequestRetryCount;
};
typedef struct OTCfgAARPPrefs           OTCfgAARPPrefs;

struct OTCfgDDPPrefs {
    UInt16                          fVersion;
    UInt16                          fSize;
    UInt32                          fTSDUSize;
    UInt8                           fLoadType;
    UInt8                           fNode;
    UInt16                          fNetwork;
    UInt16                          fRTMPRequestLimit;
    UInt16                          fRTMPRequestInterval;
    UInt32                          fAddressGenLimit;
    UInt32                          fBRCAgingInterval;
    UInt32                          fRTMPAgingInterval;
    UInt32                          fMaxAddrTries;
    Boolean                         fDefaultChecksum;
    Boolean                         fIsFixedNode;
    UInt8                           fMyZone[33];
};
typedef struct OTCfgDDPPrefs            OTCfgDDPPrefs;

struct OTCfgATPFPrefs {
    OTCfgAppleTalkPrefs             fAT;
    OTCfgAARPPrefs                  fAARP;
    OTCfgDDPPrefs                   fDDP;
    char                            fFill[122];
};
typedef struct OTCfgATPFPrefs           OTCfgATPFPrefs;
/*  Infrared    */
enum {
    kOTCfgTypeInfraredPrefs     = FOUR_CHAR_CODE('atpf'),
    kOTCfgTypeInfraredGlobal    = FOUR_CHAR_CODE('irgo')
};


struct OTCfgIRPrefs {
    CfgPrefsHeader                  fHdr;
    OTPortRef                       fPort;                      /*    OT port id*/
    OTCfgIRPortSetting              fPortSetting;               /*    Ir protocol,  irda or irtalk*/
    Boolean                         fNotifyOnDisconnect;        /*    notify user on irda disconnect?*/
    Boolean                         fDisplayIRControlStrip;     /*    show ir control strip?*/
    Point                           fWindowPosition;            /*    The position of the editor window*/
};
typedef struct OTCfgIRPrefs             OTCfgIRPrefs;

struct OTCfgIRGlobal {
    CfgPrefsHeader                  fHdr;                       /* standard prefererences header*/
    UInt32                          fOptions;                   /* options bitmask*/
    UInt32                          fNotifyMask;                /* Notification options.*/
    UInt32                          fUnloadTimeout;             /* Unload timeout (in milliseconds)*/
};
typedef struct OTCfgIRGlobal            OTCfgIRGlobal;
/*  TCP/IP v4   */
enum {
                                                                /*    connection */
    kOTCfgTypeTCPalis           = FOUR_CHAR_CODE('alis'),
    kOTCfgTypeTCPcvrs           = FOUR_CHAR_CODE('cvrs'),
    kOTCfgTypeTCPdcid           = FOUR_CHAR_CODE('dcid'),
    kOTCfgTypeTCPdclt           = FOUR_CHAR_CODE('dclt'),
    kOTCfgTypeTCPdtyp           = FOUR_CHAR_CODE('dtyp'),
    kOTCfgTypeTCPidns           = FOUR_CHAR_CODE('idns'),
    kOTCfgTypeTCPihst           = FOUR_CHAR_CODE('ihst'),
    kOTCfgTypeTCPiitf           = FOUR_CHAR_CODE('iitf'),
    kOTCfgTypeTCPara            = FOUR_CHAR_CODE('ipcp'),
    kOTCfgTypeTCPirte           = FOUR_CHAR_CODE('irte'),
    kOTCfgTypeTCPisdm           = FOUR_CHAR_CODE('isdm'),
    kOTCfgTypeTCPstng           = FOUR_CHAR_CODE('stng'),
    kOTCfgTypeTCPunld           = FOUR_CHAR_CODE('unld'),
    kOTCfgTypeTCPVersion        = FOUR_CHAR_CODE('cvrs'),       /*    Version */
    kOTCfgTypeTCPDevType        = FOUR_CHAR_CODE('dvty'),
    kOTCfgTypeTCPPrefs          = FOUR_CHAR_CODE('iitf'),
    kOTCfgTypeTCPServersList    = FOUR_CHAR_CODE('idns'),
    kOTCfgTypeTCPSearchList     = FOUR_CHAR_CODE('ihst'),
    kOTCfgTypeTCPRoutersList    = FOUR_CHAR_CODE('irte'),
    kOTCfgTypeTCPDomainsList    = FOUR_CHAR_CODE('isdm'),
    kOTCfgTypeTCPPort           = FOUR_CHAR_CODE('port'),       /*    Ports */
    kOTCfgTypeTCPProtocol       = FOUR_CHAR_CODE('prot'),
    kOTCfgTypeTCPPassword       = FOUR_CHAR_CODE('pwrd'),       /*    Password */
    kOTCfgTypeTCPLocks          = FOUR_CHAR_CODE('stng'),       /*    locks */
    kOTCfgTypeTCPUnloadType     = FOUR_CHAR_CODE('unld')        /*    transport options  */
};



struct OTCfgIDNSPrefs {
    short                           fCount;
    InetHost                        fAddressesList;
};
typedef struct OTCfgIDNSPrefs           OTCfgIDNSPrefs;

struct OTCfgHSTFPrefs {
    char                            fPrimaryInterfaceIndex;     /*    always 1 in OT 1.0 / 1.1*/
                                                                /*    this structure IS packed!*/
    UInt8                           fLocalDomainName[256];
                                                                /*    followed by */
    UInt8                           admindomain[256];
};
typedef struct OTCfgHSTFPrefs           OTCfgHSTFPrefs;
/*  This is your worst case, a fixed size structure, tacked on after a variable length string.*/
/*  Use the macro to help access the movable beast. */

#define kIITFPartP( h ) ( (OTCfgIITFPrefsPart*) &( (**( (OTCfgIITFPrefs**) h )).fAppleTalkZone[ (**( (OTCfgIITFPrefs**) h )).fAppleTalkZone[0] + 1 ] ) )

/*
    This structure also contains an IP address and subnet mask that are not aligned on a four byte boundary.  
    In order to avoid compiler warnings, and the possibility of code that won't work, 
    these fields are defined here as four character arrays.  
    It is suggested that BlockMoveData be used to copy to and from a field of type InetHost.  
*/

struct OTCfgIITFPrefsPart {
    char                            path[36];
    char                            module[32];
    unsigned long                   framing;
};
typedef struct OTCfgIITFPrefsPart       OTCfgIITFPrefsPart;

struct OTCfgIITFPrefs {
    short                           fCount;
    UInt8                           fConfigMethod;
                                                                /*    this structure IS packed!*/
                                                                /*    Followed by:*/
    UInt8                           fIPAddress[4];
    UInt8                           fSubnetMask[4];
    UInt8                           fAppleTalkZone[256];
                                                                /*    this structure IS packed!*/
    UInt8                           fFiller;
    OTCfgIITFPrefsPart              part;
};
typedef struct OTCfgIITFPrefs           OTCfgIITFPrefs;

struct OTCfgIRTEEntry {
    InetHost                        fToHost;                    /*    always 0;*/
    InetHost                        fViaHost;                   /*    router address;*/
    short                           fLocal;                     /*    always 0*/
    short                           fHost;                      /*    always 0*/
};
typedef struct OTCfgIRTEEntry           OTCfgIRTEEntry;

struct OTCfgIRTEPrefs {
    short                           fCount;
    OTCfgIRTEEntry                  fList[1];
};
typedef struct OTCfgIRTEPrefs           OTCfgIRTEPrefs;

struct OTCfgISDMPrefs {
    short                           fCount;
    Str255                          fDomainsList;
};
typedef struct OTCfgISDMPrefs           OTCfgISDMPrefs;

struct OTCfgDHCPRecord {
    InetHost                        ipIPAddr;
    InetHost                        ipConfigServer;
    UInt32                          ipLeaseGrantTime;
    UInt32                          ipLeaseExpirationTime;
};
typedef struct OTCfgDHCPRecord          OTCfgDHCPRecord;
/*  DNS */
enum {
                                                                /*    connection */
    kOTCfgTypeDNSidns           = FOUR_CHAR_CODE('idns'),
    kOTCfgTypeDNSisdm           = FOUR_CHAR_CODE('isdm'),
    kOTCfgTypeDNSihst           = FOUR_CHAR_CODE('ihst'),
    kOTCfgTypeDNSstng           = FOUR_CHAR_CODE('stng'),
    kOTCfgTypeDNSPassword       = FOUR_CHAR_CODE('pwrd')        /*    transport options  */
};

/*  Modem   */
enum {
                                                                /*    connection */
    kOTCfgTypeModemModem        = FOUR_CHAR_CODE('ccl '),       /*    Type for Modem configuration resource*/
    kOTCfgTypeModemLocks        = FOUR_CHAR_CODE('lkmd'),       /*    Types for lock resources*/
    kOTCfgTypeModemAdminPswd    = FOUR_CHAR_CODE('mdpw'),       /*    Password*/
                                                                /*    transport options  */
    kOTCfgTypeModemApp          = FOUR_CHAR_CODE('mapt')
};


struct OTCfgRemoteConfigModem {
    UInt32                          version;
    Boolean                         useModemScript;
    char                            pad00;                      /*    this structure is NOT packed!*/
    FSSpec                          modemScript;
    Boolean                         modemSpeakerOn;
    Boolean                         modemPulseDial;
    UInt32                          modemDialToneMode;
    SInt8                           lowerLayerName[36];
};
typedef struct OTCfgRemoteConfigModem   OTCfgRemoteConfigModem;

struct OTCfgModemLocks {
    UInt32                          version;
    UInt32                          port;
    UInt32                          script;
    UInt32                          speaker;
    UInt32                          dialing;
};
typedef struct OTCfgModemLocks          OTCfgModemLocks;

struct OTCfgModemAppPrefs {
    UInt32                          version;
    Point                           windowPos;
    SInt32                          userMode;
};
typedef struct OTCfgModemAppPrefs       OTCfgModemAppPrefs;
/*  Remote Access   */
enum {
                                                                /*    connection */
    kOTCfgTypeRemoteARAP        = FOUR_CHAR_CODE('arap'),
    kOTCfgTypeRemoteAddress     = FOUR_CHAR_CODE('cadr'),
    kOTCfgTypeRemoteChat        = FOUR_CHAR_CODE('ccha'),
    kOTCfgTypeRemoteDialing     = FOUR_CHAR_CODE('cdia'),
    kOTCfgTypeRemoteExtAddress  = FOUR_CHAR_CODE('cead'),
    kOTCfgTypeRemoteClientLocks = FOUR_CHAR_CODE('clks'),
    kOTCfgTypeRemoteClientMisc  = FOUR_CHAR_CODE('cmsc'),
    kOTCfgTypeRemoteConnect     = FOUR_CHAR_CODE('conn'),
    kOTCfgTypeRemoteUser        = FOUR_CHAR_CODE('cusr'),
    kOTCfgTypeRemoteDialAssist  = FOUR_CHAR_CODE('dass'),
    kOTCfgTypeRemoteIPCP        = FOUR_CHAR_CODE('ipcp'),
    kOTCfgTypeRemoteLCP         = FOUR_CHAR_CODE('lcp '),       /* trailing space is important! */
    kOTCfgTypeRemoteLogOptions  = FOUR_CHAR_CODE('logo'),
    kOTCfgTypeRemotePassword    = FOUR_CHAR_CODE('pass'),
    kOTCfgTypeRemotePort        = FOUR_CHAR_CODE('port'),
    kOTCfgTypeRemoteServerLocks = FOUR_CHAR_CODE('slks'),
    kOTCfgTypeRemoteServer      = FOUR_CHAR_CODE('srvr'),
    kOTCfgTypeRemoteUserMode    = FOUR_CHAR_CODE('usmd'),
    kOTCfgTypeRemoteX25         = FOUR_CHAR_CODE('x25 '),       /* trailing space is important! */
                                                                /*    transport options  */
    kOTCfgTypeRemoteApp         = FOUR_CHAR_CODE('capt')
};

/*******************************************************************************
*   OTCfgRemoteLogOptions
*
*   This structure is appended to OTCfgRemoteConnect records in the 
*   OTCfgRemoteConnect::additional list.
*
*   NOTE
*
*   All OTCfgRemoteConnect::additional structures MUST have the same fields up to
*   the "additional" field.  See OTCfgRemoteX25Info.
********************************************************************************/

struct OTCfgRemoteLogOptions {
    UInt32                          version;
    UInt32                          fType;                      /*    kRAConnectAdditionalLogOptions*/
    void *                          additional;
    UInt32                          logLevel;                   /*    values defined above.*/
    UInt32                          reserved[4];                /*    for later use.*/
};
typedef struct OTCfgRemoteLogOptions    OTCfgRemoteLogOptions;
/*******************************************************************************
*   New structures for dialing mode, phone numbers, and configuration stats.
*   
*   
********************************************************************************/
enum {
    kOTCfgRemoteMaxAddressSize  = (255 + 1)
};


struct OTCfgRemoteAddress {
    struct OTCfgRemoteAddress *     next;
    UInt8                           address[256];
};
typedef struct OTCfgRemoteAddress       OTCfgRemoteAddress;
/*******************************************************************************
*   OTCfgRemoteDialing
*
*   This structure is appended to OTCfgRemoteConnect records in the 
*   OTCfgRemoteConnect::additional list.
*
*   NOTE
*
*   All OTCfgRemoteConnect::additional structures MUST have the same fields up to
*   the "additional" field.  See OTCfgRemoteX25Info.
********************************************************************************/

struct OTCfgRemoteDialing {
    UInt32                          version;
    UInt32                          fType;                      /*    kRAConnectAdditionalDialing*/
    void *                          additional;
    UInt32                          dialMode;                   /*    values defined above.*/
    SInt32                          redialTries;
    UInt32                          redialDelay;                /*    in seconds.*/
    OTCfgRemoteAddress *            addresses;
};
typedef struct OTCfgRemoteDialing       OTCfgRemoteDialing;
/*******************************************************************************
*   OTCfgRemoteScript
*
*   This is appended to OTCfgRemoteConnect records in the "additional" list.
*   It is currently only used for passing in a modem script to override
*   the default script.  Connect scripts have their own field in OTCfgRemoteConnect.
*
*   NOTE
*
*   All OTCfgRemoteConnect::additional structures MUST have the same fields up to
*   the "additional" field.  See OTCfgRemoteX25Info and OTCfgRemoteDialing.
********************************************************************************/

struct OTCfgRemoteScript {
    UInt32                          version;
    UInt32                          fType;                      /*    kRAConnectAdditionalScript*/
    void *                          additional;
    UInt32                          scriptType;
    UInt32                          scriptLength;
    UInt8 *                         scriptData;
};
typedef struct OTCfgRemoteScript        OTCfgRemoteScript;
/*******************************************************************************
*   Miscellaneous limits
*   The size limits for strings include a 1 byte for the string length or
*   a terminating NULL character.
********************************************************************************/
enum {
    kOTCfgRemoteMaxPasswordLength = 255,
    kOTCfgRemoteMaxPasswordSize = (255 + 1),
    kOTCfgRemoteMaxUserNameLength = 255,
    kOTCfgRemoteMaxUserNameSize = (255 + 1),
    kOTCfgRemoteMaxAddressLength = 255,                         /*    kOTCfgRemoteMaxAddressSize           = (255 + 1),*/
    kOTCfgRemoteMaxServerNameLength = 32,
    kOTCfgRemoteMaxServerNameSize = (32 + 1),
    kOTCfgRemoteMaxMessageLength = 255,
    kOTCfgRemoteMaxMessageSize  = (255 + 1),
    kOTCfgRemoteMaxX25ClosedUserGroupLength = 4,
    kOTCfgRemoteInfiniteSeconds = (long)0xFFFFFFFF,
    kOTCfgRemoteMinReminderMinutes = 1,
    kOTCfgRemoteChatScriptFileCreator = FOUR_CHAR_CODE('ttxt'),
    kOTCfgRemoteChatScriptFileType = FOUR_CHAR_CODE('TEXT'),
    kOTCfgRemoteMaxChatScriptLength = 0x8000
};

/*******************************************************************************
*   X.25 connection information, added to OTCfgRemoteConnect's additional info list.
*
*   NOTE
*
*   All OTCfgRemoteConnect::additional structures MUST have the same fields up to
*   the "additional" field.  See OTCfgRemoteScript & OTCfgRemoteDialing.
********************************************************************************/

struct OTCfgRemoteX25Info {
    UInt32                          version;
    UInt32                          fType;                      /*    kRAConnectAdditionalX25*/
    void *                          additional;                 /*    Ptr to additional connect info*/
    FSSpec                          script;                     /*    PAD's CCL script*/
    UInt8                           address[256];               /*    address of server*/
    UInt8                           userName[256];
                                                                /*    network user ID*/
    UInt8                           closedUserGroup[5];
                                                                /*    closed user group*/
    Boolean                         reverseCharge;              /*    request reverse charging*/
};
typedef struct OTCfgRemoteX25Info       OTCfgRemoteX25Info;
/*******************************************************************************
*   OTCfgRemoteDisconnect
*
*   Use this structure to terminate Remote Access connections.
********************************************************************************/

struct OTCfgRemoteDisconnect {
    UInt32                          whenSeconds;                /*    Number of seconds until disconnect*/
    UInt32                          showStatus;                 /*    Show disconnect status window*/
};
typedef struct OTCfgRemoteDisconnect    OTCfgRemoteDisconnect;
/*******************************************************************************
*   OTCfgRemoteIsRemote
*
*   Use this structure to find out if an AppleTalk address is on the 
*   remote side of the current ARA link. The "isRemote" field is set to
*   "true" if the address is remote. 
********************************************************************************/

struct OTCfgRemoteIsRemote {
    UInt32                          net;                        /*    AppleTalk network number*/
    UInt32                          node;                       /*    AppleTalk node number*/
    UInt32                          isRemote;                   /*    returned.*/
};
typedef struct OTCfgRemoteIsRemote      OTCfgRemoteIsRemote;
/*******************************************************************************
*   OTCfgRemoteConnect
*
*   Use this structure to initiate Remote Access connections.
********************************************************************************/

struct OTCfgRemoteConnect {
    UInt32                          version;
    UInt32                          fType;                      /*    RAConnectType defined above.*/
    UInt32                          isGuest;                    /*    (boolean) True for guest login*/
    UInt32                          canInteract;                /*    (boolean) True if dialogs can be displayed*/
    UInt32                          showStatus;                 /*    (boolean) Display (dis)connection status dialogs?*/
    UInt32                          passwordSaved;              /*    (boolean) "Save Password" checked in doc.*/
    UInt32                          flashConnectedIcon;         /*    (boolean) Flash icon in menu bar*/
    UInt32                          issueConnectedReminders;    /*    (boolean) Use Notification Manager reminders*/
    SInt32                          reminderMinutes;            /*    How long between each reminder?*/
    UInt32                          connectManually;            /*    (boolean) True if we are connecting manually*/
    UInt32                          allowModemDataCompression;  /*    (boolean) currently, only for kSerialProtoPPP*/
    UInt32                          chatMode;                   /*    Flags defined above*/
    UInt32                          serialProtocolMode;         /*    Flags defined above*/

    UInt8 *                         password;
    UInt8 *                         userName;
    UInt32                          addressLength;              /*    Length of phone number or other address*/
    UInt8 *                         address;                    /*    Phone number or address data*/
    Str63                           chatScriptName;             /*    Name of imported chat script (informational only)*/
    UInt32                          chatScriptLength;           /*    Length of Chat script*/
    UInt8 *                         chatScript;                 /*    Chat script data*/

    void *                          additional;                 /*    Ptr to additional connect info,*/
                                                                /*    such as OTCfgRemoteX25Info*/

    UInt32                          useSecurityModule;          /*    (boolean) use line-level security module ?*/
    OSType                          securitySignature;          /*    signature of security module file*/
    UInt32                          securityDataLength;         /*    0..kSecurityMaxConfigData*/
    UInt8 *                         securityData;               /*    Ptr to data of size securityDataLength*/
};
typedef struct OTCfgRemoteConnect       OTCfgRemoteConnect;
/*******************************************************************************
*   OTCfgRemoteConnectInfo
*
*   If requestCode = kRAGetConnectInfo, "connectInfo" returns a pointer to a
*   new OTCfgRemoteConnect block that describes the current connection.
*
*   If requestCode = kRADisposeConnectInfo, the memory pointed to by
*   "connectInfo" is released for reuse.  "connectInfo" must point to a valid
*   OTCfgRemoteConnect structure previously returned by kRAGetConnectInfo.
********************************************************************************/

struct OTCfgRemoteConnectInfo {
    OTCfgRemoteConnect *            connectInfo;                /*    Returned or disposed, depending on requestCode*/
};
typedef struct OTCfgRemoteConnectInfo   OTCfgRemoteConnectInfo;
/*******************************************************************************
*   OTCfgRemoteStatus
*
*   Use this structure to get the status of Remote Access connections.
********************************************************************************/
enum {
    kOTCfgRemoteStatusIdle      = 1,
    kOTCfgRemoteStatusConnecting = 2,
    kOTCfgRemoteStatusConnected = 3,
    kOTCfgRemoteStatusDisconnecting = 4
};



struct OTCfgRemoteStatus {
    UInt32                          status;                     /*    values defined above*/
    Boolean                         answerEnabled;
    char                            pad00;                      /*    This structure is NOT packed*/
    UInt32                          secondsConnected;
    UInt32                          secondsRemaining;
    UInt8                           userName[256];              /*    Pascal format*/
    UInt8                           serverName[33];             /*    Pascal format*/
    char                            pad01;                      /*    This structure is NOT packed*/
    UInt32                          messageIndex;
    UInt8                           message[256];               /*    Pascal format*/
    UInt32                          serialProtocolMode;         /*    Flags defined above.*/
    UInt8                           baudMessage[256];           /*    Pascal format*/
    Boolean                         isServer;
    char                            pad02;                      /*    This structure is NOT packed*/
    UInt32                          bytesIn;
    UInt32                          bytesOut;
    UInt32                          linkSpeed;
    UInt32                          localIPAddress;
    UInt32                          remoteIPAddress;
};
typedef struct OTCfgRemoteStatus        OTCfgRemoteStatus;
/*******************************************************************************
*   OTCfgRemoteUserMessage
*
*   Use this structure when converting result codes into user messages.
********************************************************************************/

struct OTCfgRemoteUserMessage {
    UInt32                          version;
    SInt32                          messageID;
    UInt8                           userMessage[256];
    UInt8                           userDiagnostic[256];
};
typedef struct OTCfgRemoteUserMessage   OTCfgRemoteUserMessage;
/*******************************************************************************
*   OTCfgRemoteNotifier
*
*   Use this structure to install a procedure to receive asynchronous 
*   Remote Access notifications.
********************************************************************************/

typedef UInt32                          OTCfgRemoteEventCode;
typedef CALLBACK_API_C( void , RANotifyProcPtr )(void *contextPtr, OTCfgRemoteEventCode code, OSStatus result, void *cookie);

struct OTCfgRemoteNotifier {
    RANotifyProcPtr                 procPtr;
    void *                          contextPtr;
};
typedef struct OTCfgRemoteNotifier      OTCfgRemoteNotifier;
/*******************************************************************************
*   OTCfgRemoteRequest
*
*   All Remote Access API calls must pass a pointer to an OTCfgRemoteRequest structure.
********************************************************************************/

struct OTCfgRemoteRequest {
    SInt8                           reserved1[16];              /*    Do not use. */
    OSErr                           result;                     /*    <--*/
    SInt8                           reserved2[8];               /*    Do not use.*/
    SInt16                          requestCode;                /*     -->*/
    SInt16                          portId;                     /*    <-->*/
    union {
        OTCfgRemoteNotifier             Notifier;
        OTCfgRemoteConnect              Connect;
        OTCfgRemoteDisconnect           Disconnect;
        OTCfgRemoteStatus               Status;
        OTCfgRemoteIsRemote             IsRemote;
        OTCfgRemoteConnectInfo          ConnectInfo;
    }                                 fType;
};
typedef struct OTCfgRemoteRequest       OTCfgRemoteRequest;

struct OTCfgRemoteConfigCAPT {
    UInt32                          fWord1;
    Point                           fWindowPosition;
    UInt32                          fWord3;
    UInt32                          fUserLevel;
    UInt32                          fSetupVisible;
};
typedef struct OTCfgRemoteConfigCAPT    OTCfgRemoteConfigCAPT;
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

#endif /* __NETWORKSETUP__ */

