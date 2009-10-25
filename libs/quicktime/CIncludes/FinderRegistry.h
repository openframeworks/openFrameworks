/*
     File:       FinderRegistry.h
 
     Contains:   Data types for Finder AppleEvents
 
     Version:    Technology: Mac OS 8
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FINDERREGISTRY__
#define __FINDERREGISTRY__

#ifndef __AEREGISTRY__
#include <AERegistry.h>
#endif

#ifndef __OSA__
#include <OSA.h>
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
  //////////////////////////////////////
   Finder Suite
  //////////////////////////////////////
*/

/*
   The old Finder Event suite was 'FNDR'
   The new suite is 'fndr'
*/
enum {
    kAEFinderSuite              = FOUR_CHAR_CODE('fndr')
};

/*
  //////////////////////////////////////
   Finder Events
  //////////////////////////////////////
*/
enum {
    kAECleanUp                  = FOUR_CHAR_CODE('fclu'),
    kAEEject                    = FOUR_CHAR_CODE('ejct'),
    kAEEmpty                    = FOUR_CHAR_CODE('empt'),
    kAEErase                    = FOUR_CHAR_CODE('fera'),
    kAEGestalt                  = FOUR_CHAR_CODE('gstl'),
    kAEPutAway                  = FOUR_CHAR_CODE('ptwy'),
    kAERebuildDesktopDB         = FOUR_CHAR_CODE('rddb'),
    kAESync                     = FOUR_CHAR_CODE('fupd'),
    kAEInterceptOpen            = FOUR_CHAR_CODE('fopn')
};

/* "Sort" from the database suite:*/
enum {
    kAEDatabaseSuite            = FOUR_CHAR_CODE('DATA'),
    kAESort                     = FOUR_CHAR_CODE('SORT')
};

/*
  ////////////////////////////////////////////////////////////////////////
   Classes
   Note: all classes are defined up front so that the property definitions
   can reference classes.
  ////////////////////////////////////////////////////////////////////////
*/

enum {
    cInternalFinderObject       = FOUR_CHAR_CODE('obj ')        /* cReference - used to distinguish objects used inside the Finder only*/
};

/*
   Main Finder class definitions
   Indentation implies object model hierarchy
*/
enum {
                                                                /* We do not use class cItem from AERegistry.r. Instead our class Item is a cObject*/
                                                                /*         cItem                        = 'citm',   // defined in AERegistry.r*/
                                                                /*          cFile                    = 'file',  // defined in AERegistry.r*/
    cAliasFile                  = FOUR_CHAR_CODE('alia'),
    cApplicationFile            = FOUR_CHAR_CODE('appf'),
    cControlPanelFile           = FOUR_CHAR_CODE('ccdv'),
    cDeskAccessoryFile          = FOUR_CHAR_CODE('dafi'),
    cDocumentFile               = FOUR_CHAR_CODE('docf'),
    cFontFile                   = FOUR_CHAR_CODE('fntf'),
    cSoundFile                  = FOUR_CHAR_CODE('sndf'),
    cClippingFile               = FOUR_CHAR_CODE('clpf'),
    cContainer                  = FOUR_CHAR_CODE('ctnr'),
    cDesktop                    = FOUR_CHAR_CODE('cdsk'),
    cSharableContainer          = FOUR_CHAR_CODE('sctr'),
    cDisk                       = FOUR_CHAR_CODE('cdis'),
    cFolder                     = FOUR_CHAR_CODE('cfol'),
    cSuitcase                   = FOUR_CHAR_CODE('stcs'),
    cAccessorySuitcase          = FOUR_CHAR_CODE('dsut'),
    cFontSuitcase               = FOUR_CHAR_CODE('fsut'),
    cTrash                      = FOUR_CHAR_CODE('ctrs'),
    cDesktopPrinter             = FOUR_CHAR_CODE('dskp'),
    cPackage                    = FOUR_CHAR_CODE('pack'),
    cContentSpace               = FOUR_CHAR_CODE('dwnd'),       /*          cWindow                    = 'cwin',       // defined in AERegistry.r*/
    cContainerWindow            = FOUR_CHAR_CODE('cwnd'),
    cInfoWindow                 = FOUR_CHAR_CODE('iwnd'),
    cSharingWindow              = FOUR_CHAR_CODE('swnd'),
    cStatusWindow               = FOUR_CHAR_CODE('qwnd'),
    cClippingWindow             = FOUR_CHAR_CODE('lwnd'),
    cPreferencesWindow          = FOUR_CHAR_CODE('pwnd'),
    cDTPWindow                  = FOUR_CHAR_CODE('dtpw'),
    cProcess                    = FOUR_CHAR_CODE('prcs'),
    cAccessoryProcess           = FOUR_CHAR_CODE('pcda'),
    cApplicationProcess         = FOUR_CHAR_CODE('pcap'),
    cGroup                      = FOUR_CHAR_CODE('sgrp'),
    cUser                       = FOUR_CHAR_CODE('cuse'),       /*         cApplication                  = 'capp',     // defined in AERegistry.r*/
    cSharingPrivileges          = FOUR_CHAR_CODE('priv'),
    cPreferences                = FOUR_CHAR_CODE('cprf'),
    cLabel                      = FOUR_CHAR_CODE('clbl'),
    cSound                      = FOUR_CHAR_CODE('snd '),
    cAliasList                  = FOUR_CHAR_CODE('alst'),
    cSpecialFolders             = FOUR_CHAR_CODE('spfl'),       /* For use by viewer search engines:*/
    cOnlineDisk                 = FOUR_CHAR_CODE('cods'),
    cOnlineLocalDisk            = FOUR_CHAR_CODE('clds'),
    cOnlineRemoteDisk           = FOUR_CHAR_CODE('crds'),       /* Miscellaneous class definitions*/
    cEntireContents             = FOUR_CHAR_CODE('ects'),
    cIconFamily                 = FOUR_CHAR_CODE('ifam')
};


/*
  //////////////////////////////////////
   Properties
  //////////////////////////////////////
*/

/* Properties of class cItem (really cObject)*/
enum {
                                                                /*    pBounds                        = 'pbnd',       // defined in AERegistry.r*/
    pComment                    = FOUR_CHAR_CODE('comt'),
    pContainer                  = cContainer,
    pContentSpace               = cContentSpace,
    pCreationDateOld            = FOUR_CHAR_CODE('crtd'),       /* to support pre-Finder 8 scripts*/
    pCreationDate               = FOUR_CHAR_CODE('ascd'),       /* from File Commands OSAX*/
    pDescription                = FOUR_CHAR_CODE('dscr'),
    pDisk                       = cDisk,
    pFolderOld                  = cFolder,                      /* to support pre-Finder 8 scripts*/
    pFolder                     = FOUR_CHAR_CODE('asdr'),       /* from File Commands OSAX*/
    pIconBitmap                 = FOUR_CHAR_CODE('iimg'),       /*    pID                           = 'ID  ',        // defined in AERegistry.r*/
    pInfoWindow                 = cInfoWindow,
    pKind                       = FOUR_CHAR_CODE('kind'),
    pLabelIndex                 = FOUR_CHAR_CODE('labi'),
    pModificationDateOld        = FOUR_CHAR_CODE('modd'),       /* to support pre-Finder 8 scripts*/
    pModificationDate           = FOUR_CHAR_CODE('asmo'),       /* from File Commands OSAX*/
                                                                /*    pName                      = 'pnam',         // defined in AERegistry.r*/
    pPhysicalSize               = FOUR_CHAR_CODE('phys'),
    pPosition                   = FOUR_CHAR_CODE('posn'),
    pIsSelected                 = FOUR_CHAR_CODE('issl'),
    pSize                       = pPointSize,                   /* pPointSize defined in AERegistry.r*/
    pWindow                     = cWindow,
    pPreferencesWindow          = cPreferencesWindow
};


/* Properties of class cFile (subclass of cItem)*/
enum {
    pFileCreator                = FOUR_CHAR_CODE('fcrt'),
    pFileType                   = FOUR_CHAR_CODE('asty'),       /* from File Commands OSAX*/
    pFileTypeOld                = FOUR_CHAR_CODE('fitp'),       /* to support pre-Finder 8 scripts*/
    pIsLocked                   = FOUR_CHAR_CODE('aslk'),       /* from File Commands OSAX*/
    pIsLockedOld                = FOUR_CHAR_CODE('islk'),       /* to support pre-Finder 8 scripts*/
                                                                /*    pIsStationeryPad               = 'pspd',         // defined in AERegistry.r                */
                                                                /*    pVersion                    = 'vers',       // defined in AERegistry.r*/
    pProductVersion             = FOUR_CHAR_CODE('ver2')
};


/* Properties of class cAliasFile (subclass of cFile)*/
enum {
    pOriginalItem               = FOUR_CHAR_CODE('orig')
};

/* Properties of class cApplicationFile (subclass of cFile)*/
enum {
    pMinAppPartition            = FOUR_CHAR_CODE('mprt'),
    pAppPartition               = FOUR_CHAR_CODE('appt'),
    pSuggestedAppPartition      = FOUR_CHAR_CODE('sprt'),
    pIsScriptable               = FOUR_CHAR_CODE('isab')
};

/* Properties of class cURLFile (subclass of cFile)*/
enum {
    pInternetLocation           = FOUR_CHAR_CODE('iloc')
};

/* Properties of class cSoundFile (subclass of cFile)*/
enum {
    pSound                      = FOUR_CHAR_CODE('snd ')
};


/*
   Properties of class cControlPanel (Views CP only) (subclass of cFile)
   Note: the other view-like preference settings are not available in the Views
   control panel. These properties are only offered here for backward compatability.
   To set the full range of Finder Preferences, use the Preferences object.
*/
enum {
    pShowFolderSize             = FOUR_CHAR_CODE('sfsz'),       /* Moved to a per-folder basis in Finder 8.0 HIS*/
    pShowComment                = FOUR_CHAR_CODE('scom'),       /* Moved to a per-folder basis in Finder 8.0 HIS*/
    pShowDate                   = FOUR_CHAR_CODE('sdat'),       /* Moved to a per-folder basis in Finder 8.0 HIS*/
    pShowCreationDate           = FOUR_CHAR_CODE('scda'),       /* Moved to a per-folder basis in Finder 8.0 HIS*/
    pShowKind                   = FOUR_CHAR_CODE('sknd'),       /* Moved to a per-folder basis in Finder 8.0 HIS*/
    pShowLabel                  = FOUR_CHAR_CODE('slbl'),       /* Moved to a per-folder basis in Finder 8.0 HIS*/
    pShowSize                   = FOUR_CHAR_CODE('ssiz'),       /* Moved to a per-folder basis in Finder 8.0 HIS*/
    pShowVersion                = FOUR_CHAR_CODE('svrs'),       /* Moved to a per-folder basis in Finder 8.0 HIS*/
    pSortDirection              = FOUR_CHAR_CODE('sord'),
    pShowDiskInfo               = FOUR_CHAR_CODE('sdin'),       /* Always on in Finder 8.0 HIS*/
    pListViewIconSize           = FOUR_CHAR_CODE('lvis'),       /* Moved to a per-folder basis in Finder 8.0 HIS*/
    pGridIcons                  = FOUR_CHAR_CODE('fgrd'),       /* Moved to a per-folder basis in Finder 8.0 HIS*/
    pStaggerIcons               = FOUR_CHAR_CODE('fstg'),       /* No longer part of the Finder 8.0 HIS*/
    pViewFont                   = FOUR_CHAR_CODE('vfnt'),
    pViewFontSize               = FOUR_CHAR_CODE('vfsz')
};

/* Properties of class cContainer (subclass of cItem)*/
enum {
    pCompletelyExpanded         = FOUR_CHAR_CODE('pexc'),
    pContainerWindow            = cContainerWindow,
    pEntireContents             = cEntireContents,
    pExpandable                 = FOUR_CHAR_CODE('pexa'),
    pExpanded                   = FOUR_CHAR_CODE('pexp'),
    pPreviousView               = FOUR_CHAR_CODE('svew'),       /*    pSelection                    = 'sele',       // defined in AERegistry.r*/
    pView                       = FOUR_CHAR_CODE('pvew'),
    pIconSize                   = pListViewIconSize,            /* defined above*/
    pKeepArranged               = FOUR_CHAR_CODE('arrg'),       /* OBSOLETE in Finder 9 or later*/
    pKeepArrangedBy             = FOUR_CHAR_CODE('arby')        /* OBSOLETE in Finder 9 or later*/
};

/* Properties of class cDesktop (subclass of cContainer)*/
enum {
    pStartupDisk                = FOUR_CHAR_CODE('sdsk'),
    pTrash                      = FOUR_CHAR_CODE('trsh')
};

/* Properties of class cSharableContainer (subclass of cContainer)*/
enum {
    pOwner                      = FOUR_CHAR_CODE('sown'),
    pOwnerPrivileges            = FOUR_CHAR_CODE('ownr'),
    pGroup                      = cGroup,
    pGroupPrivileges            = FOUR_CHAR_CODE('gppr'),
    pGuestPrivileges            = FOUR_CHAR_CODE('gstp'),
    pArePrivilegesInherited     = FOUR_CHAR_CODE('iprv'),
    pExported                   = FOUR_CHAR_CODE('sexp'),
    pMounted                    = FOUR_CHAR_CODE('smou'),
    pSharingProtection          = FOUR_CHAR_CODE('spro'),
    pSharing                    = FOUR_CHAR_CODE('shar'),
    pSharingWindow              = cSharingWindow
};

/* Properties of class cDisk (subclass of cSharableContainer)*/
enum {
    pCapacity                   = FOUR_CHAR_CODE('capa'),
    pEjectable                  = FOUR_CHAR_CODE('isej'),
    pFreeSpace                  = FOUR_CHAR_CODE('frsp'),
    pLocal                      = FOUR_CHAR_CODE('isrv'),
    pIsStartup                  = FOUR_CHAR_CODE('istd')
};

/* Properties of class cTrash (subclass of cSharableContainer)*/
enum {
    pWarnOnEmpty                = FOUR_CHAR_CODE('warn')
};

/* Properties of class cWindow (subclass of cContentSpace)*/
enum {
                                                                /*    pBounds                        = 'pbnd',   // defined in AERegistry.r*/
                                                                /*    pHasCloseBox                = 'hclb',     // defined in AERegistry.r*/
                                                                /*    pIsFloating                    = 'isfl',     // defined in AERegistry.r*/
                                                                /*    pIndex                     = 'pidx',     // defined in AERegistry.r*/
                                                                /*    pIsModal                    = 'pmod',   // defined in AERegistry.r*/
                                                                /*    pPosition                    = 'posn',     // defined above*/
                                                                /*    pIsResizable                = 'prsz',     // defined in AERegistry.r*/
                                                                /*    pHasTitleBar                = 'ptit',     // defined in AERegistry.r*/
                                                                /*    pVisible                    = 'pvis',   // defined in AERegistry.r*/
                                                                /*    pIsZoomable                    = 'iszm',     // defined in AERegistry.r*/
                                                                /*    pIsZoomed                    = 'pzum',     // defined in AERegistry.r*/
    pIsZoomedFull               = FOUR_CHAR_CODE('zumf'),
    pIsPopup                    = FOUR_CHAR_CODE('drwr'),
    pIsPulledOpen               = FOUR_CHAR_CODE('pull'),       /* only applies to popup windows*/
    pIsCollapsed                = FOUR_CHAR_CODE('wshd')        /* only applies to normal windows*/
};

/* Properties of class cContainerWindow (subclass of cWindow)*/
enum {
    pObject                     = cObject
};

/* Properties of class cSharingWindow (subclass of cWindow)*/
enum {
    pSharableContainer          = cSharableContainer
};

/* Properties of class cInfoWindow (subclass of cWindow)*/
enum {
    pInfoPanel                  = FOUR_CHAR_CODE('panl')
};


/* Properties of networking support*/
enum {
    pFileShareOn                = FOUR_CHAR_CODE('fshr'),
    pFileShareStartingUp        = FOUR_CHAR_CODE('fsup'),
    pProgramLinkingOn           = FOUR_CHAR_CODE('iac ')
};

/* Properties of class cPreferencesWindow (subclass of cWindow)*/
enum {
                                                                /*    pShowFolderSize                   = 'sfsz',         // defined above for Views CP*/
                                                                /*    pShowComment                = 'scom',      // defined above for Views CP*/
    pShowModificationDate       = pShowDate,                    /* pShowDate defined above for Views CP*/
                                                                /*    pShowKind                    = 'sknd',        // defined above for Views CP*/
                                                                /*    pShowLabel                    = 'slbl',         // defined above for Views CP*/
                                                                /*    pShowSize                    = 'ssiz',        // defined above for Views CP*/
                                                                /*    pShowVersion                = 'svrs',      // defined above for Views CP*/
                                                                /*    pShowCreationDate             = 'scda',      // Removed from Finder 8.0 HIS*/
                                                                /*    pShowFileType                 = 'sfty',       // Removed from Finder 8.0 HIS*/
                                                                /*    pShowFileCreator               = 'sfcr',         // Removed from Finder 8.0 HIS*/
                                                                /*    pListViewIconSize             = 'lvis',      // defined above for Views CP*/
                                                                /*    pGridIcons                    = 'fgrd',         // defined above for Views CP*/
                                                                /*    pStaggerIcons                 = 'fstg',       // defined above for Views CP*/
                                                                /*    pViewFont                    = 'vfnt',        // defined above for Views CP*/
                                                                /*    pViewFontSize                 = 'vfsz',       // defined above for Views CP*/
    pUseRelativeDate            = FOUR_CHAR_CODE('urdt'),       /* Moved to a per-folder basis in Finder 8.0 HIS*/
    pDelayBeforeSpringing       = FOUR_CHAR_CODE('dela'),
    pSpringOpenFolders          = FOUR_CHAR_CODE('sprg'),
    pUseShortMenus              = FOUR_CHAR_CODE('usme'),
    pUseWideGrid                = FOUR_CHAR_CODE('uswg'),
    pLabel1                     = FOUR_CHAR_CODE('lbl1'),
    pLabel2                     = FOUR_CHAR_CODE('lbl2'),
    pLabel3                     = FOUR_CHAR_CODE('lbl3'),
    pLabel4                     = FOUR_CHAR_CODE('lbl4'),
    pLabel5                     = FOUR_CHAR_CODE('lbl5'),
    pLabel6                     = FOUR_CHAR_CODE('lbl6'),
    pLabel7                     = FOUR_CHAR_CODE('lbl7'),
    pDefaultIconViewIconSize    = FOUR_CHAR_CODE('iisz'),
    pDefaultButtonViewIconSize  = FOUR_CHAR_CODE('bisz'),
    pDefaultListViewIconSize    = FOUR_CHAR_CODE('lisz'),       /* old use of this name is now pIconSize*/
    pIconViewArrangement        = FOUR_CHAR_CODE('iarr'),
    pButtonViewArrangement      = FOUR_CHAR_CODE('barr')
};

/*
   The next bunch are the various arrangements that make up
   enumArrangement
*/
enum {
    pNoArrangement              = FOUR_CHAR_CODE('narr'),
    pSnapToGridArrangement      = FOUR_CHAR_CODE('grda'),
    pByNameArrangement          = FOUR_CHAR_CODE('nama'),
    pByModificationDateArrangement = FOUR_CHAR_CODE('mdta'),
    pByCreationDateArrangement  = FOUR_CHAR_CODE('cdta'),
    pBySizeArrangement          = FOUR_CHAR_CODE('siza'),
    pByKindArrangement          = FOUR_CHAR_CODE('kina'),
    pByLabelArrangement         = FOUR_CHAR_CODE('laba')
};

/*  #define pObject                                 cObject         // defined above*/

/* Properties of class cProcess (subclass of cObject)*/
enum {
                                                                /*    pName                      = 'pnam',         // defined in AERegistry.r*/
    pFile                       = cFile,                        /*    pCreatorType                = 'fcrt',      // defined above*/
                                                                /*    pFileType                    = 'asty',        // defined above*/
                                                                /*    pIsFrontProcess                   = 'pisf',         // defined in AERegistry.r*/
                                                                /*    pAppPartition                 = 'appt',       // defined above*/
    pPartitionSpaceUsed         = FOUR_CHAR_CODE('pusd'),       /*    pIsScriptable                 = 'isab',       // defined in AERegistry.r*/
                                                                /*    pVisible                    = 'pvis'      // defined in AERegistry.r*/
    pLocalAndRemoteEvents       = FOUR_CHAR_CODE('revt'),
    pHasScriptingTerminology    = FOUR_CHAR_CODE('hscr')
};

/* Properties of class cAccessoryProcess (subclass of cProcess)*/
enum {
    pDeskAccessoryFile          = cDeskAccessoryFile
};

/* Properties of class cApplicationProcess (subclass of cProcess)*/
enum {
    pApplicationFile            = cApplicationFile
};

/*
   Properties of class cGroup (subclass of cObject)
  enum {
    pBounds
    pIconBitmap
    pLabelIndex
    pName
    pPosition
    pWindow                                 = cWindow           // defined above
  };
*/

/* Properties of class cUser (subclass of cObject)*/
enum {
                                                                /*    pBounds*/
                                                                /*    pIconBitmap*/
                                                                /*    pLabelIndex*/
                                                                /*    pName*/
                                                                /*    pPosition*/
                                                                /*    pWindow                        = cWindow,        // defined above*/
    pCanConnect                 = FOUR_CHAR_CODE('ccon'),
    pCanChangePassword          = FOUR_CHAR_CODE('ccpw'),
    pCanDoProgramLinking        = FOUR_CHAR_CODE('ciac'),
    pIsOwner                    = FOUR_CHAR_CODE('isow'),
    pARADialIn                  = FOUR_CHAR_CODE('arad'),
    pShouldCallBack             = FOUR_CHAR_CODE('calb'),
    pCallBackNumber             = FOUR_CHAR_CODE('cbnm')
};

/*
   Properties of class cApplication (subclass of cObject)
   NOTE: properties for the special folders must match their respective kXXXFolderType constants
*/
enum {
    pAboutMacintosh             = FOUR_CHAR_CODE('abbx'),
    pAppleMenuItemsFolder       = FOUR_CHAR_CODE('amnu'),       /* kAppleMenuFolderType*/
                                                                /*    pClipboard                    = 'pcli',         // defined in AERegistry.r*/
    pControlPanelsFolder        = FOUR_CHAR_CODE('ctrl'),       /* kControlPanelFolderType*/
    pDesktop                    = FOUR_CHAR_CODE('desk'),       /* kDesktopFolderType*/
    pExtensionsFolder           = FOUR_CHAR_CODE('extn'),       /* kExtensionFolderType*/
                                                                /*    pFileShareOn                = 'fshr',      // defined above*/
    pFinderPreferences          = FOUR_CHAR_CODE('pfrp'),
    pFontsFolder                = FOUR_CHAR_CODE('font'),
    pFontsFolderPreAllegro      = FOUR_CHAR_CODE('ffnt'),       /* DO NOT USE THIS - FOR BACKWARDS COMPAT ONLY*/
                                                                /*    pIsFrontProcess                   = 'pisf',         // defined in AERegistry.r*/
                                                                /*    pInsertionLoc                 = 'pins',       // defined in AERegistry.r*/
    pLargestFreeBlock           = FOUR_CHAR_CODE('mfre'),
    pPreferencesFolder          = FOUR_CHAR_CODE('pref'),       /* kPreferencesFolderType*/
                                                                /*    pProductVersion                   = 'ver2',         // defined above*/
                                                                /*    pUserSelection                  = 'pusl',        // defined in AERegistry.r*/
                                                                /*    pFileShareStartingUp             = 'fsup',        // defined above*/
    pShortCuts                  = FOUR_CHAR_CODE('scut'),
    pShutdownFolder             = FOUR_CHAR_CODE('shdf'),
    pStartupItemsFolder         = FOUR_CHAR_CODE('strt'),       /* kStartupFolderType*/
    pSystemFolder               = FOUR_CHAR_CODE('macs'),       /* kSystemFolderType*/
    pTemporaryFolder            = FOUR_CHAR_CODE('temp'),       /* kTemporaryFolderType*/
                                                                /*    pVersion                    = 'vers',       // defined in AERegistry.r*/
    pViewPreferences            = FOUR_CHAR_CODE('pvwp'),       /*    pVisible                    = 'pvis',       // defined in AERegistry.r*/
    pStartingUp                 = FOUR_CHAR_CODE('awak')        /* private property to tell whether the Finder is fully up and running*/
};

/* Properties of class cSharingPrivileges (subclass of cObject)*/
enum {
    pSeeFiles                   = FOUR_CHAR_CODE('prvr'),
    pSeeFolders                 = FOUR_CHAR_CODE('prvs'),
    pMakeChanges                = FOUR_CHAR_CODE('prvw')
};

/*
   Properties of class cPreferences (subclass of cObject)
  enum {
    pShowFolderSize                         = 'sfsz',           // defined above for Views CP
    pShowComment                            = 'scom',           // defined above for Views CP
    pShowModificationDate                   = pShowDate,            // pShowDate defined above for Views CP
    pShowKind                               = 'sknd',           // defined above for Views CP
    pShowLabel                              = 'slbl',           // defined above for Views CP
    pShowSize                               = 'ssiz',           // defined above for Views CP
    pShowVersion                            = 'svrs',           // defined above for Views CP
    pShowCreationDate                       = 'scda',           // defined in cPreferencesWindow
    pShowFileType                           = 'sfty',           // defined in cPreferencesWindow
    pShowFileCreator                        = 'sfcr',           // defined in cPreferencesWindow
    pListViewIconSize                       = 'lvis',           // defined above for Views CP
    pGridIcons                              = 'fgrd',           // defined above for Views CP
    pStaggerIcons                           = 'fstg',           // defined above for Views CP
    pViewFont                               = 'vfnt',           // defined above for Views CP
    pViewFontSize                           = 'vfsz',           // defined above for Views CP
    pUseRelativeDate                        = 'urdt',           // defined in cPreferencesWindow
    pDelayBeforeSpringing                   = 'dela',           // defined in cPreferencesWindow
    pShowMacOSFolder                        = 'sosf',           // defined in cPreferencesWindow
    pUseShortMenus                          = 'usme',           // defined in cPreferencesWindow
    pUseCustomNewMenu                       = 'ucnm',           // defined in cPreferencesWindow
    pShowDesktopInBackground                = 'sdtb',           // defined in cPreferencesWindow
    pActivateDesktopOnClick                 = 'adtc',           // defined in cPreferencesWindow
    pLabel1                                 = 'lbl1',           // defined in cPreferencesWindow
    pLabel2                                 = 'lbl2',           // defined in cPreferencesWindow
    pLabel3                                 = 'lbl3',           // defined in cPreferencesWindow
    pLabel4                                 = 'lbl4',           // defined in cPreferencesWindow
    pLabel5                                 = 'lbl5',           // defined in cPreferencesWindow
    pLabel6                                 = 'lbl6',           // defined in cPreferencesWindow
    pLabel7                                 = 'lbl7',           // defined in cPreferencesWindow
    pWindow                                 = cWindow           // defined above
  };
*/

/*
   Properties of class cLabel (subclass of cObject)
  enum {
    pName                                   = 'pnam',           // defined in AERegistry.r
    pColor                                  = 'colr',           // defined in AERegistry.r
  };
*/

/* Misc Properties*/
enum {
    pSmallIcon                  = FOUR_CHAR_CODE('smic'),
    pSmallButton                = FOUR_CHAR_CODE('smbu'),
    pLargeButton                = FOUR_CHAR_CODE('lgbu'),
    pGrid                       = FOUR_CHAR_CODE('grid')
};

/*
  //////////////////////////////////////
   Enumerations defined by the Finder
  //////////////////////////////////////
*/

enum {
    enumViewBy                  = FOUR_CHAR_CODE('vwby'),
    enumGestalt                 = FOUR_CHAR_CODE('gsen'),
    enumConflicts               = FOUR_CHAR_CODE('cflc'),
    enumExistingItems           = FOUR_CHAR_CODE('exsi'),
    enumOlderItems              = FOUR_CHAR_CODE('oldr')
};

enum {
    enumDate                    = FOUR_CHAR_CODE('enda'),
    enumAnyDate                 = FOUR_CHAR_CODE('anyd'),
    enumToday                   = FOUR_CHAR_CODE('tday'),
    enumYesterday               = FOUR_CHAR_CODE('yday'),
    enumThisWeek                = FOUR_CHAR_CODE('twek'),
    enumLastWeek                = FOUR_CHAR_CODE('lwek'),
    enumThisMonth               = FOUR_CHAR_CODE('tmon'),
    enumLastMonth               = FOUR_CHAR_CODE('lmon'),
    enumThisYear                = FOUR_CHAR_CODE('tyer'),
    enumLastYear                = FOUR_CHAR_CODE('lyer'),
    enumBeforeDate              = FOUR_CHAR_CODE('bfdt'),
    enumAfterDate               = FOUR_CHAR_CODE('afdt'),
    enumBetweenDate             = FOUR_CHAR_CODE('btdt'),
    enumOnDate                  = FOUR_CHAR_CODE('ondt')
};

enum {
    enumAllDocuments            = FOUR_CHAR_CODE('alld'),
    enumFolders                 = FOUR_CHAR_CODE('fold'),
    enumAliases                 = FOUR_CHAR_CODE('alia'),
    enumStationery              = FOUR_CHAR_CODE('stat')
};

enum {
    enumWhere                   = FOUR_CHAR_CODE('wher'),
    enumAllLocalDisks           = FOUR_CHAR_CODE('aldk'),
    enumAllRemoteDisks          = FOUR_CHAR_CODE('ardk'),
    enumAllDisks                = FOUR_CHAR_CODE('alld'),
    enumAllOpenFolders          = FOUR_CHAR_CODE('aofo')
};


enum {
    enumIconSize                = FOUR_CHAR_CODE('isiz'),
    enumSmallIconSize           = pSmallIcon,
    enumMiniIconSize            = FOUR_CHAR_CODE('miic'),
    enumLargeIconSize           = FOUR_CHAR_CODE('lgic')
};

enum {
    enumSortDirection           = FOUR_CHAR_CODE('sodr'),
    enumSortDirectionNormal     = FOUR_CHAR_CODE('snrm'),
    enumSortDirectionReverse    = FOUR_CHAR_CODE('srvs')
};

enum {
    enumArrangement             = FOUR_CHAR_CODE('earr')
};

/* Get Info Window panel enumeration*/
enum {
    enumInfoWindowPanel         = FOUR_CHAR_CODE('ipnl'),
    enumGeneralPanel            = FOUR_CHAR_CODE('gpnl'),
    enumSharingPanel            = FOUR_CHAR_CODE('spnl'),
    enumStatusNConfigPanel      = FOUR_CHAR_CODE('scnl'),
    enumFontsPanel              = FOUR_CHAR_CODE('fpnl'),
    enumMemoryPanel             = FOUR_CHAR_CODE('mpnl')
};


/* Preferences panel enumeration*/
enum {
    enumPrefsWindowPanel        = FOUR_CHAR_CODE('pple'),
    enumPrefsGeneralPanel       = FOUR_CHAR_CODE('pgnp'),
    enumPrefsLabelPanel         = FOUR_CHAR_CODE('plbp'),
    enumPrefsIconViewPanel      = FOUR_CHAR_CODE('pivp'),
    enumPrefsButtonViewPanel    = FOUR_CHAR_CODE('pbvp'),
    enumPrefsListViewPanel      = FOUR_CHAR_CODE('plvp')
};

/*
  //////////////////////////////////////
   Types defined by the Finder
  //////////////////////////////////////
*/

enum {
    typeIconFamily              = cIconFamily,                  /* An AEList of typeIconAndMask, type8BitIcon, & c.*/
    typeIconAndMask             = FOUR_CHAR_CODE('ICN#'),
    type8BitMask                = FOUR_CHAR_CODE('l8mk'),
    type32BitIcon               = FOUR_CHAR_CODE('il32'),
    type8BitIcon                = FOUR_CHAR_CODE('icl8'),
    type4BitIcon                = FOUR_CHAR_CODE('icl4'),
    typeSmallIconAndMask        = FOUR_CHAR_CODE('ics#'),
    typeSmall8BitMask           = FOUR_CHAR_CODE('s8mk'),
    typeSmall32BitIcon          = FOUR_CHAR_CODE('is32'),
    typeSmall8BitIcon           = FOUR_CHAR_CODE('ics8'),
    typeSmall4BitIcon           = FOUR_CHAR_CODE('ics4'),
    typeRelativeTime            = FOUR_CHAR_CODE('rtim'),
    typeConceptualTime          = FOUR_CHAR_CODE('timc')
};

/*
  //////////////////////////////////////
   Keywords defined by the Finder
  //////////////////////////////////////
*/

enum {
    keyIconAndMask              = FOUR_CHAR_CODE('ICN#'),
    key32BitIcon                = FOUR_CHAR_CODE('il32'),
    key8BitIcon                 = FOUR_CHAR_CODE('icl8'),
    key4BitIcon                 = FOUR_CHAR_CODE('icl4'),
    key8BitMask                 = FOUR_CHAR_CODE('l8mk'),
    keySmallIconAndMask         = FOUR_CHAR_CODE('ics#'),
    keySmall8BitIcon            = FOUR_CHAR_CODE('ics8'),
    keySmall4BitIcon            = FOUR_CHAR_CODE('ics4'),
    keySmall32BitIcon           = FOUR_CHAR_CODE('is32'),
    keySmall8BitMask            = FOUR_CHAR_CODE('s8mk'),
    keyMini1BitMask             = FOUR_CHAR_CODE('icm#'),
    keyMini4BitIcon             = FOUR_CHAR_CODE('icm4'),
    keyMini8BitIcon             = FOUR_CHAR_CODE('icm8'),
    keyAEUsing                  = FOUR_CHAR_CODE('usin'),
    keyAEReplacing              = FOUR_CHAR_CODE('alrp'),
    keyAENoAutoRouting          = FOUR_CHAR_CODE('rout'),
    keyLocalPositionList        = FOUR_CHAR_CODE('mvpl'),
    keyGlobalPositionList       = FOUR_CHAR_CODE('mvpg'),
    keyRedirectedDocumentList   = FOUR_CHAR_CODE('fpdl')
};

/*
  //////////////////////////////////////
   New prepositions used by the Finder
  //////////////////////////////////////
*/

enum {
    keyASPrepositionHas         = FOUR_CHAR_CODE('has '),
    keyAll                      = FOUR_CHAR_CODE('kyal'),
    keyOldFinderItems           = FOUR_CHAR_CODE('fsel')
};

/*
  //////////////////////////////////////
   New key forms used by the Finder
  //////////////////////////////////////
*/

enum {
    formAlias                   = typeAlias,
    formCreator                 = pFileCreator
};


/*
  //////////////////////////////////////
   Finder error codes
  //////////////////////////////////////
*/

enum {
    errFinderIsBusy             = -15260,
    errFinderWindowNotOpen      = -15261,
    errFinderCannotPutAway      = -15262,
    errFinderWindowMustBeIconView = -15263,                     /* RequireWindowInIconView*/
    errFinderWindowMustBeListView = -15264,                     /* RequireWindowInListView*/
    errFinderCantMoveToDestination = -15265,
    errFinderCantMoveSource     = -15266,
    errFinderCantOverwrite      = -15267,
    errFinderIncestuousMove     = -15268,                       /* Could just use errFinderCantMoveSource*/
    errFinderCantMoveToAncestor = -15269,                       /* Could also use errFinderCantMoveSource*/
    errFinderCantUseTrashedItems = -15270,
    errFinderItemAlreadyInDest  = -15271,                       /* Move from folder A to folder A*/
    errFinderUnknownUser        = -15272,                       /* Includes unknown group*/
    errFinderSharePointsCantInherit = -15273,
    errFinderWindowWrongType    = -15274,
    errFinderPropertyNowWindowBased = -15275,
    errFinderAppFolderProtected = -15276,                       /* used by General controls when folder protection is on*/
    errFinderSysFolderProtected = -15277,                       /* used by General controls when folder protection is on*/
    errFinderBoundsWrong        = -15278,
    errAEValueOutOfRange        = -15279,
    errFinderPropertyDoesNotApply = -15280,
    errFinderFileSharingMustBeOn = -15281,
    errFinderMustBeActive       = -15282,
    errFinderVolumeNotFound     = -15283,                       /* more descriptive than what we get with nsvErr*/
    errFinderLockedItemsInTrash = -15284,                       /* there are some locked items in the trash*/
    errFinderOnlyLockedItemsInTrash = -15285,                   /* all the items (except folders) in the trash are locked*/
    errFinderProgramLinkingMustBeOn = -15286,
    errFinderWindowMustBeButtonView = -15287,
    errFinderBadPackageContents = -15288,                       /* something is wrong within the package   */
    errFinderUnsupportedInsidePackages = -15289,                /* operation cannot be used on items within a package     */
    errFinderCorruptOpenFolderList = -15290,                    /* was -15276 in Finder 8.6 and earlier, but that conflicted with General Controls*/
    errFinderNoInvisibleFiles   = -15291,                       /* was -15277 in Finder 8.6 and earlier, but that conflicted with General Controls*/
    errFinderCantDeleteImmediately = -15292,                    /* cannot delete immediately via scripting*/
    errFinderLastReserved       = -15379
};


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

#endif /* __FINDERREGISTRY__ */

