/*
     File:       OCETemplates.h
 
     Contains:   Apple Open Collaboration Environment Templates Interfaces.
 
     Version:    Technology: AOCE Toolbox 1.02
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OCETEMPLATES__
#define __OCETEMPLATES__

#ifndef REZ
#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
#endif


#endif /* REZ */
#ifndef __OCE__
#include <OCE.h>
#endif


#ifndef REZ
#ifndef __OCESTANDARDMAIL__
#include <OCEStandardMail.h>
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

#endif /* REZ */


/************************************************************************************************
 ********************************* Template Resource Constants: *********************************
 ************************************************************************************************/


/* Current versions of all the different template types: */

#define kDETAspectVersion        -976
#define kDETInfoPageVersion        -976
#define kDETKillerVersion      -976
#define kDETForwarderVersion   -976
#define kDETFileTypeVersion        -976


/*   'Normal' separation for template IDs within the file (this is just a suggestion; you can use whatever
  separation you like, so long as two separate templates don't have overlapping resources): */
#define kDETIDSep 250

/* A few predefined base IDs (again, just suggestions): */
#define kDETFirstID      (1000)
#define kDETSecondID (1000+kDETIDSep)
#define kDETThirdID        (1000+2*kDETIDSep)
#define kDETFourthID (1000+3*kDETIDSep)
#define kDETFifthID      (1000+4*kDETIDSep)



/* Templates consist of a set of associated resources, at constant offsets from a 'base ID' set by the
    signature resource of the template. In the case of aspect templates, most of the resources in the
  template are accessible from the template as property default values. The property number is the same
  as the offset from the base ID of the resource. In describing the resources which make up templates,
   we give the type, the offset, and a description. For aspect templates, the offset is also the property #. 
 
   All templates include the following resource fork resources:

   Type   Offset                      Description
     ----   ------                      -----------
    'rstr'  kDETTemplateName            Contains the name of the template

*/

#define kDETTemplateName 0

/*   Aspects, info-pages, and forwarders include the following as well:

     Type   Offset                      Description
     ----   ------                      -----------
    'rstr'  kDETRecordType              Contains the type of record this applies to
    'rstr'  kDETAttributeType           Contains the type of attribute this applies to
 'detn'  kDETAttributeValueTag       Contains the tag of the attribute values this applies to

*/

#define kDETRecordType                1
#define kDETAttributeType         2
#define kDETAttributeValueTag     3



/* ************************************ Aspects */

/*    In the case of aspects, property numbers and resource id offsets are the same. Therefore, some of the following
    defines are used for resource offsets, some are used for dynamically generated properties, and some are used for
   both (i.e., properties which may be dynamically generated, but if they aren't, then they're taken from the
 resource). Resource types are given in all cases below; even if there is no actual resource (for example in
    the case of kDETAspectName), it indicates the type for the dynamically generated property. A resource type of
  'rstr' corresponds to a property type of kDETPrTypeString; type 'detn' corresponds to kDETPrTypeNumber; and
    'detb' corresponds to kDETPrTypeBinary.



    Type   Offset                      Description
     ----   ------                      -----------
    'deta'  0                           Identifies the type of resource
    'detc'  kDETCode                    Is the code resource, if any is used
   'ICN#'  kDETAspectMainBitmap
   'icl8'  kDETAspectMainBitmap
   'icl4'  kDETAspectMainBitmap
   'ics#'  kDETAspectMainBitmap
   'ics8'  kDETAspectMainBitmap
   'ics4'  kDETAspectMainBitmap
   'sicn'  kDETAspectMainBitmap        Is the icon suite to display for this item type (main aspect only)
 'rstr'  kDETAspectName              Contains the name of the item (attribute main aspect only)
 'rst#'  kDETAspectCategory          Contains the internal categories for the record type (main aspect only)
    'rst#'  kDETAspectExternalCategory  Contains the external (user-visible) names which correspond to the categories in
                                       kDETAspectCategory; if this resource is not present, external names are taken from
                                     another template; if no other template provides an external name for a given internal
                                      category, the internal name is used as the external name (main aspect only)
    'rstr'  kDETAspectKind              Is the item kind to display (main aspect only)
 'detn'  kDETAspectGender            Is the gender of this kind of object for internationalization (main aspect only)
   'rstr'  kDETAspectWhatIs            Is the string for balloon help when item is in sublist (main aspect only)
  'rstr'  kDETAspectAliasKind         Is the item kind to display for aliases to this type of item (main aspect only)
    'detn'  kDETAspectAliasGender       Is the gender of an alias to this kind of object for internationalization (main aspect only)
   'rstr'  kDETAspectAliasWhatIs       Is the string for balloon help when an alias to the item is in sublist (main aspect only)
  'rst#'  kDETAspectBalloons          Is a list of strings for balloon help; for each item in an info-page, it's
                                     property # times 2 is used as an index into this array; if the item is not editable,
                                       then the property # times 2 plus 1 is used


   'rstr'  kDETAspectNewMenuName       Is the string to be used for the user to select new item creation; for records, the
                                        string is used as a menu entry in the Catalogs menu; for attributes, the string
                                        is used in a selection dialog invoked by an 'Add...' button
    'rstr'  kDETAspectNewEntryName      Is the name to be used for new records (with a digit appended if not unique)
   'detb'  kDETAspectNewValue          Is the value to use when creating a new attribute value; the first four bytes
                                      is the tag; the rest is the attribute value contents
   'detn'  kDETAspectSublistOpenOnNew  If true (non-zero), automatically open newly created entries (property can be set
                                      from a code resource, or via a default value in a resource)
    'dett'  kDETAspectLookup            Is the attribute-to-property translation table
 'rstr'  kDETAspectDragInString      Is a string describing the action of dragging into this aspect (if any)
    'rstr'  kDETAspectDragInVerb        Is a single, short word that's the verb of the action (like 'add' or 'drop'; if
                                        there's any doubt, use 'OK')
   'rstr'  kDETAspectDragInSummary     Is a short phrase that describes the action, suitable to be included in a selection list
   'rst#'  kDETAspectRecordDragIn      Is a list of type pairs; in each pair, the first is the type of a record which
                                     can be dragged into this aspect, and the second is the attribute type to store
                                     the reference in
   'rst#'  kDETAspectRecordCatDragIn   Is a list of category/attribute type pairs; in each pair, the first is the
                                     category of records which can be dragged in, and the second is the type of
                                     attribute to place the alias in
    'rst#'  kDETAspectAttrDragIn        Is a list of type triples; in each triple, the first is the record type which can be
                                       dragged from (or '' for any), the second is the attribute type which can be dragged
                                        in, and the third is the attribute type to store the new attribute in


   'rst#'  kDETAspectDragOut           Is a list of attribute types which can be dragged out of this aspect (an 'rst#'
                                        resource with no entries means nothing can be dragged out; no 'rst#' resource means
                                        everything can be dragged out)
 'detm'  kDETAspectViewMenu          Is a table to fill in the view menu from
   'detp'  kDETAspectReverseSort       Is a table listing which properties to sort in reverse order
   'detw'  kDETAspectInfoPageCustomWindow  Is a specification of a custom window size/placement & whether to use the
                                          page-selector (main aspect only)
   'detv'  kDETAspectInfoPageCustomWindow  Is a view list which is common to all info-pages (main aspect only)
*/


#define kDETAspectCode                  4
#define kDETAspectMainBitmap          5
#define kDETAspectName                    6
#define kDETAspectCategory                7
#define kDETAspectExternalCategory        8
#define kDETAspectKind                    9
#define kDETAspectGender             10
#define kDETAspectWhatIs             11
#define kDETAspectAliasKind              12
#define kDETAspectAliasGender            13
#define kDETAspectAliasWhatIs            14
#define kDETAspectBalloons               15
#define kDETAspectNewMenuName            16
#define kDETAspectNewEntryName           17
#define kDETAspectNewValue               18
#define kDETAspectSublistOpenOnNew       19
#define kDETAspectLookup             20
#define kDETAspectDragInString           21
#define kDETAspectDragInVerb         22
#define kDETAspectDragInSummary          23
#define kDETAspectRecordDragIn           24
#define kDETAspectRecordCatDragIn        25
#define kDETAspectAttrDragIn         26
#define kDETAspectAttrDragOut            27
#define kDETAspectViewMenu               28
#define kDETAspectReverseSort            29
#define kDETAspectInfoPageCustomWindow   30



/* Properties: */

#define kDETNoProperty             -1

/* Each aspect has 250 attribute properties in this range: */
#define kDETFirstLocalProperty        0
#define kDETLastLocalProperty         (kDETFirstLocalProperty+249)

/* Developers should use property numbers starting at this point: */
#define kDETFirstDevProperty     40

/* The following range provides constant numeric properties for use in patterns and comparisons (constant n is
   given by kDETFirstConstantProperty+n): */
#define kDETFirstConstantProperty   250
#define kDETLastConstantProperty    (kDETFirstConstantProperty+249)

/* To convert a number into a constant property, add this: */
#define kDETConstantProperty     kDETFirstConstantProperty

#define kDETZeroProperty         (kDETConstantProperty+0)
#define kDETOneProperty                (kDETConstantProperty+1)
#define kDETFalseProperty          (kDETConstantProperty+0)
#define kDETTrueProperty           (kDETConstantProperty+1)

/* The following apply to records, attributes, or aliases; they are the name and kind, as they appear in icon lists: */
#define kDETPrName                    3050
#define kDETPrKind                     3051

/* Access mask properties: */
#define kDETDNodeAccessMask         25825   /* The DNode access mask */
#define kDETRecordAccessMask        25826   /* The record access mask */
#define kDETAttributeAccessMask        25827   /* The attribute access mask */
#define kDETPrimaryMaskByBit        25828   /* A set of sixteen properties to access all bits of the primary mask */



/* See AOCE documentation for details definitions of each of these bits: */
#define kDETPrimarySeeMask         kDETPrimaryMaskByBit
#define kDETPrimaryAddMask         (kDETPrimaryMaskByBit+1)
#define kDETPrimaryDeleteMask      (kDETPrimaryMaskByBit+2)
#define kDETPrimaryChangeMask      (kDETPrimaryMaskByBit+3)
#define kDETPrimaryRenameMask      (kDETPrimaryMaskByBit+4)
#define kDETPrimaryChangePrivsMask (kDETPrimaryMaskByBit+5)
#define kDETPrimaryTopMaskBit      (kDETPrimaryMaskByBit+15)

/* The following property is zero until we've completed the first catalog lookup; from then on it's 1 */
#define kDETPastFirstLookup         26550

/* The following property is the page number; issuing a property command with this property will flip info-pages */
#define kDETInfoPageNumber           27050

/* The value of the following properties contains the template number of the targeted aspect's template, and the
   currently open info-page (if any). These values can be used with kDETAspectTemplate and kDETInfoPageTemplate
   target selectors. */
#define kDETAspectTemplateNumber    26551
#define kDETInfoPageTemplateNumber    26552

/* Properties for property commands to deal with sublist items: */
#define kDETOpenSelectedItems     26553   /* Open selected sublist items */
#define kDETAddNewItem                26554   /* Add new sublist item */
#define kDETRemoveSelectedItems      26555   /* Remove selected sublist items */

/* Property types are used to specify types of properties and conversions between types (zero and negative numbers
   are reserved for Apple; developer code resources can use positive numbers): */
#define kDETPrTypeNumber          -1  /* A number */
#define kDETPrTypeString         -2  /* A string */
#define kDETPrTypeBinary         -3  /* A binary block */

/* Rez-compatible tag definitions (These are the same as in OCE.h, but they're enums there and can't be rezed...) */
#ifdef REZ
#define typeRString           'rstr'
#define typePackedDSSpec 'dspc'
#define typeBinary           'bnry'
#endif



/* ************************************ Info-pages */

/* Info-pages include the following as well:

  Type   Offset                      Description
     ----   ------                      -----------
    'deti'  0                           Identifies the type of resource; see below for details on the contents
 'rstr'  kDETInfoPageName            Is the name of the view to use in the page selection pop-up
    'rstr'  kDETInfoPageMainViewAspect  Is the name of the aspect to use with the main page view
   'rstr'  kDETInfoPageMenuName        Is the name of the catalogs menu ('Catalogs' if not present)
   'detm'  kDETInfoPageMenuEntries     Are menu entries to go in the Catalogs menu

*/

#define kDETInfoPageName           4
#define kDETInfoPageMainViewAspect    5
#define kDETInfoPageMenuName      6
#define kDETInfoPageMenuEntries       7




/* ************************************ Views 

   Flags: */

#define kDETNoFlags                      0
#define kDETEnabled                       (1 << 0)    /* Main view (non-sublist) field enabled */

/* The following flags make sense for items in a sublist only */
#define kDETHilightIfSelected         (1 << 0)    /* Hilight view when entry is selected */

/* The following flags make sense for text views only */
#define kDETNumericOnly                 (1 << 3)    /* Only allow the user to enter digits */
#define kDETMultiLine                 (1 << 4)    /* Allow multiple lines in view */
#define kDETDynamicSize                  (1 << 9)    /* Don't draw box around text until user clicks in it, then auto-size it */
#define kDETAllowNoColons               (1 << 10)   /* Don't allow the user to enter colons (convert ':'s to '-'s) */

/* The following flags are used for pop-up menus only */
#define kDETPopupDynamicSize            (1 << 8)    /* Automatically resize pop-up based on contents */

/* The following flags are used for EditPicture views only */
#define kDETScaleToView                  (1 << 8)    /* Scale picture to view bounds rather than cropping */

/* Sizes for icons */
#define kDETLargeIcon        0
#define kDETSmallIcon     1
#define kDETMiniIcon      2

/* Stolen from TextEdit.h */
#define kDETLeft         0
#define kDETCenter        1
#define kDETRight        -1
#define kDETForceLeft    -2



/* Flags for use within Box view type attributes - these are distinct from the flags above */

#define kDETUnused                  0
#define kDETBoxTakesContentClicks (1 << 0)
#define kDETBoxIsRounded           (1 << 1)
#define kDETBoxIsGrayed                (1 << 2)
#define kDETBoxIsInvisible         (1 << 3)

/* The common font info */

#define kDETApplicationFont                1
#define kDETApplicationFontSize       9
#define kDETAppFontLineHeight            12

#define kDETSystemFont                   0
#define kDETSystemFontSize               12
#define kDETSystemFontLineHeight     16

#define kDETDefaultFont                  1
#define kDETDefaultFontSize               9
#define kDETDefaultFontLineHeight        12

/*  These were taken from QuickDraw.h (where they're enums and therefore unusable in resource definitions): */
#define kDETNormal                       0
#define kDETBold                      1
#define kDETItalic                        2
#define kDETUnderline                 4
#define kDETOutline                       8
#define kDETShadow                     0x10
#define kDETCondense               0x20
#define kDETExtend                     0x40

#define kDETIconStyle                   -3   /* Normal text style for regular sublist entries, italic text style for aliases */

/* View menu: */

#define   kDETChangeViewCommand       'view'  /* Change the view; used especially in StaticCommandTextFromView sublist headers */


/* Info-page window sizes: */

/* Default record info-pages: */
#define kDETRecordInfoWindHeight      228
#define kDETRecordInfoWindWidth         400

/* Default attribute info-pages: */
#define kDETAttributeInfoWindHeight        250
#define kDETAttributeInfoWindWidth      230

/* Page identifying icon (for default info-page layout): */
#define kDETSubpageIconTop     8
#define kDETSubpageIconLeft       8
#define kDETSubpageIconBottom (kDETSubpageIconTop+32)
#define kDETSubpageIconRight    (kDETSubpageIconLeft+32)
#define kDETSubpageIconRect        {kDETSubpageIconTop, kDETSubpageIconLeft, kDETSubpageIconBottom, kDETSubpageIconRight}

/* The following rectangle can be used in a 'deti' with no sublist: */
#define kDETNoSublistRect        {0, 0, 0, 0}




/* ************************************ Killers 

   Killers include the following as well:

     Type   Offset                      Description
     ----   ------                      -----------
    'detk'  0                           Identifies the type of resource; see below for details on the contents
 'rst#'  kDETKillerName              Contains a list of template names to be killed

*/

#define kDETKillerName 1


/* ************************************ Forwarders 

   Forwarders include the following as well:

   Type   Offset                      Description
     ----   ------                      -----------
    'detf'  0                           Identifies the type of resource; see below for details on the contents
 'rst#'  kDETForwarderTemplateNames  Contains a list of names of templates to forward to

*/

#define kDETForwarderTemplateNames 4



/*********************************************************************************** 
 ********************************* Categories: ************************************* 
 *********************************************************************************** 

   Internal category names: */

#define kDETCategoryAllItems        "aoce All Items"        /* Everything in or out of a category */
#define kDETCategoryAddressItems   "aoce Address Items"    /* Anything that can be used to address something */
#define kDETCategoryMisc           "aoce Miscellaneous"    /* Things that don't deserve their own category */



/***********************************************************************************/
/********************************* Code Resources: *********************************/
/***********************************************************************************/
#ifndef REZ
/* Target specification: */
enum {
    kDETSelf                    = 0,                            /* The "current" item */
    kDETSelfOtherAspect         = 1,                            /* Another aspect of the current item */
    kDETParent                  = 2,                            /* The parent (i.e., the aspect we're in the sublist of, if any) of the current item */
    kDETSublistItem             = 3,                            /* The itemNumberth item in the sublist */
    kDETSelectedSublistItem     = 4,                            /* The itemNumberth selected item in the sublist */
    kDETDSSpec                  = 5,                            /* The item specified by the packed DSSpec */
    kDETAspectTemplate          = 6,                            /* A specific aspect template (number itemNumber) */
    kDETInfoPageTemplate        = 7,                            /* A specific info-page template (number itemNumber) * Force type to be short */
    kDETHighSelector            = 0xF000
};

typedef long                            DETTargetSelector;

struct DETTargetSpecification {
    DETTargetSelector               selector;                   /* Target selection method (see above) */
    RStringPtr                      aspectName;                 /* The name of the aspect (kDETSelfOtherAspect, kDETSublistItem,*/
                                                                /*   kDETSelectedSublistItem, kDETDSSpec); nil for main aspect or none; */
                                                                /*  always filled in for calls if there is an aspect, even if it's the main aspect */
    long                            itemNumber;                 /* Sublist index (kDETSublistItem & kDETSelectedSublistItem & kDETAspectTemplate);*/
                                                                /*   1-based indexing */
    PackedDSSpecPtr                 dsSpec;                     /* DSSpec (kDETDSSpec only) */
};
typedef struct DETTargetSpecification   DETTargetSpecification;
/* Code resource calls and call-backs both return an OSType:
        kDETDidNotHandle (1)    = used by template to say "I didn't handle it" (for calls only)
        noErr                   = function completed successfully
        any error               = function failed, and here's why
*/
#define kDETDidNotHandle 1
/* Call-back functions:

        reqFunction                         Action
        -----------                         ------
        kDETcmdBeep                         Call SysBeep; useful for testing that a code resource's calls/call-backs are working at all

        kDETcmdBusy                         Put up watch cursor and switch processes; user events elicit a beep

        kDETcmdChangeCallFors               Change call-fors mask

        kDETcmdGetCommandSelectionCount     Get the command selection count (for calls which have a command selection list)
        kDETcmdGetCommandItemN              Get command selection item n (for calls which have a command selection list)

        kDETcmdOpenDSSpec                   PackedDSSpec open (can also be done via AppleEvents -- this is a short-cut)

        kDETcmdAboutToTalk                  About to talk to user: bring us to front, disable watch cursor, etc.

        kDETcmdUnloadTemplates              Flush templates

        kDETcmdTemplateCounts               Return number of aspect and info-page templates in system

        kDETcmdGetDSSpec                    Get the PackedDSSpec for this object

        kDETcmdSublistCount                 Return the count of the sublist items
        kDETcmdSelectedSublistCount         Return the count of the selected sublist items

        kDETcmdRequestSync                  Request a sync-up of the aspect with the catalog

        kDETcmdBreakAttribute               Break an attribute -- apply all applicable patterns to an attribute to generate properties

        kDETcmdGetTemplateFSSpec            Get the FSSpec of the file containing the template

        kDETcmdGetOpenEdit                  Return the property of the view being edited (or kDETNoProperty if none)
        kDETcmdCloseEdit                    Close the current edit

        kDETcmdGetPropertyType              Get a property type

        kDETcmdGetPropertyNumber            Get a property, number format
        kDETcmdGetPropertyRString           Get a property, RString format
        kDETcmdGetPropertyBinarySize        Get a property, binary, return size
        kDETcmdGetPropertyBinary            Get a property, binary format

        kDETcmdGetPropertyChanged           Get a property changed flag
        kDETcmdGetPropertyEditable          Get a property editable flag

        kDETcmdSetPropertyType              Set a property type

        kDETcmdSetPropertyNumber            Set a property, number format
        kDETcmdSetPropertyRString           Set a property, RString format
        kDETcmdSetPropertyBinary            Set a property, binary data & size

        kDETcmdSetPropertyChanged           Set a property changed flag
        kDETcmdSetPropertyEditable          Set a property editable flag

        kDETcmdDirtyProperty                Dirty a property (notify other code resources of change)

        kDETcmdDoPropertyCommand            Issue a property command

        kDETcmdAddMenu                      Add to the end of a dynamic menu
        kDETcmdRemoveMenu                   Remove a dynamic menu item
        kDETcmdMenuItemRString              Get a dynamic menu item RString

        kDETcmdSaveProperty                 Force a save of a property -- apply all applicable patterns to write out the property

        kDETcmdGetCustomViewUserReference   Get custom view user reference (as given in .r file)
        kDETcmdGetCustomViewBounds          Get custom view current bounds

        kDETcmdGetResource                  Get a resource from a template
*/

enum {
    kDETcmdSimpleCallback       = 0,
    kDETcmdBeep                 = 1,
    kDETcmdBusy                 = 2,
    kDETcmdChangeCallFors       = 3,
    kDETcmdGetCommandSelectionCount = 4,
    kDETcmdGetCommandItemN      = 5,
    kDETcmdOpenDSSpec           = 6,
    kDETcmdAboutToTalk          = 7,
    kDETcmdUnloadTemplates      = 8,
    kDETcmdTemplateCounts       = 9,
    kDETcmdTargetedCallback     = 1000,
    kDETcmdGetDSSpec            = 1001,
    kDETcmdSublistCount         = 1002,
    kDETcmdSelectedSublistCount = 1003,
    kDETcmdRequestSync          = 1004,
    kDETcmdBreakAttribute       = 1005,
    kDETcmdGetTemplateFSSpec    = 1006,
    kDETcmdGetOpenEdit          = 1007,
    kDETcmdCloseEdit            = 1008,
    kDETcmdPropertyCallback     = 2000,
    kDETcmdGetPropertyType      = 2001,
    kDETcmdGetPropertyNumber    = 2002,
    kDETcmdGetPropertyRString   = 2003,
    kDETcmdGetPropertyBinarySize = 2004,
    kDETcmdGetPropertyBinary    = 2005,
    kDETcmdGetPropertyChanged   = 2006,
    kDETcmdGetPropertyEditable  = 2007,
    kDETcmdSetPropertyType      = 2008,
    kDETcmdSetPropertyNumber    = 2009,
    kDETcmdSetPropertyRString   = 2010,
    kDETcmdSetPropertyBinary    = 2011,
    kDETcmdSetPropertyChanged   = 2012,
    kDETcmdSetPropertyEditable  = 2013,
    kDETcmdDirtyProperty        = 2014,
    kDETcmdDoPropertyCommand    = 2015,
    kDETcmdAddMenu              = 2016,
    kDETcmdRemoveMenu           = 2017,
    kDETcmdMenuItemRString      = 2018,
    kDETcmdSaveProperty         = 2019,
    kDETcmdGetCustomViewUserReference = 2020,
    kDETcmdGetCustomViewBounds  = 2021,
    kDETcmdGetResource          = 2022,                         /* Force type to be long */
    kDETcmdHighCallback         = (long)0xF0000000
};


typedef unsigned long                   DETCallBackFunctions;
/* Call functions:

        reqFunction                     Action
        -----------                     ------
        kDETcmdInit                     Called once when template is first loaded (good time to allocate private data); returns call-for list
        kDETcmdExit                     Called once when template is freed (good time to free private data)

        kDETcmdAttributeCreation        New sublist attribute creation about to occur; this gives the template a chance to modify
                                        the value that's about to be created; sent to the template that will be used for
                                        the main aspect of the new entry

        kDETcmdDynamicForwarders        Return a list of dynamically created forwarders

        kDETcmdInstanceInit             Called once when instance of template is started (good time to allocate private instance data)
        kDETcmdInstanceExit             Called once when instance is ended (good time to free private instance data)

        kDETcmdIdle                     Called periodically during idle times

        kDETcmdViewListChanged          Called when the info-page view-list (list of enabled views) has changed

        kDETcmdValidateSave             Validate save: about to save info-page, return noErr (or kDETDidNotHandle) if it's OK to do so

        kDETcmdDropQuery                Drop query: return the appropriate operation for this drag; ask destination
        kDETcmdDropMeQuery              Drop query: return the appropriate operation for this drag; ask dropee

        kDETcmdAttributeNew             Attribute value new (return kDETDidNotHandle to let normal new processing occur)
        kDETcmdAttributeChange          Attribute value change (return kDETDidNotHandle to let normal change processing occur)
        kDETcmdAttributeDelete          Attribute value delete (return kDETDidNotHandle to let normal deletion occur); sent to the
                                        main aspect of the attribute that's about to be deleted
        kDETcmdItemNew                  Target item (record or attribute) has just been created

        kDETcmdOpenSelf                 Self open (return noErr to prevent opening; return kDETDidNotHandle to allow it)

        kDETcmdDynamicResource          Return a dynamically created resource

        kDETcmdShouldSync               Check if the code resource wants to force a sync (update data from catalog)
        kDETcmdDoSync                   Give code resource a chance to sync (read in and break all attributes)

        kDETcmdPropertyCommand          Command received in the property number range (usually means a button's been pushed)

        kDETcmdMaximumTextLength        Return maximum size for text form of property

        kDETcmdPropertyDirtied          Property dirtied, need to redraw

        kDETcmdPatternIn                Custom pattern element encountered on reading in an attribute
        kDETcmdPatternOut               Custom pattern element encountered on writing out an attribute

        kDETcmdConvertToNumber          Convert from template-defined property type to number
        kDETcmdConvertToRString         Convert from template-defined property type to RString
        kDETcmdConvertFromNumber        Convert from number to template-defined property type
        kDETcmdConvertFromRString       Convert from RString to template-defined property type

        kDETcmdCustomViewDraw           Custom view draw
        kDETcmdCustomViewMouseDown      Custom view mouse down

        kDETcmdKeyPress                 Key press (used primarily to filter entry into EditText views)
        kDETcmdPaste                    Paste (used primarily to filter entry into EditText views)

        kDETcmdCustomMenuSelected       Custom Catalogs menu selected
        kDETcmdCustomMenuEnabled        Return whether custom Catalogs menu entry should be enabled
*/

enum {
    kDETcmdSimpleCall           = 0,
    kDETcmdInit                 = 1,
    kDETcmdExit                 = 2,
    kDETcmdAttributeCreation    = 3,
    kDETcmdDynamicForwarders    = 4,
    kDETcmdTargetedCall         = 1000,
    kDETcmdInstanceInit         = 1001,
    kDETcmdInstanceExit         = 1002,
    kDETcmdIdle                 = 1003,
    kDETcmdViewListChanged      = 1004,
    kDETcmdValidateSave         = 1005,
    kDETcmdDropQuery            = 1006,
    kDETcmdDropMeQuery          = 1007,
    kDETcmdAttributeNew         = 1008,
    kDETcmdAttributeChange      = 1009,
    kDETcmdAttributeDelete      = 1010,
    kDETcmdItemNew              = 1011,
    kDETcmdOpenSelf             = 1012,
    kDETcmdDynamicResource      = 1013,
    kDETcmdShouldSync           = 1014,
    kDETcmdDoSync               = 1015,
    kDETcmdPropertyCall         = 2000,
    kDETcmdPropertyCommand      = 2001,
    kDETcmdMaximumTextLength    = 2002,
    kDETcmdPropertyDirtied      = 2003,
    kDETcmdPatternIn            = 2004,
    kDETcmdPatternOut           = 2005,
    kDETcmdConvertToNumber      = 2006,
    kDETcmdConvertToRString     = 2007,
    kDETcmdConvertFromNumber    = 2008,
    kDETcmdConvertFromRString   = 2009,
    kDETcmdCustomViewDraw       = 2010,
    kDETcmdCustomViewMouseDown  = 2011,
    kDETcmdKeyPress             = 2012,
    kDETcmdPaste                = 2013,
    kDETcmdCustomMenuSelected   = 2014,
    kDETcmdCustomMenuEnabled    = 2015,
    kDETcmdHighCall             = (long)0xF0000000              /* Force the type to be long */
};

typedef unsigned long                   DETCallFunctions;
/* Valid commandIDs for DETDropQueryBlock and DETDropMeQueryBlock (in addition to property numbers): */
enum {
    kDETDoNothing               = FOUR_CHAR_CODE('xxx0'),
    kDETMove                    = FOUR_CHAR_CODE('move'),
    kDETDrag                    = FOUR_CHAR_CODE('drag'),
    kDETAlias                   = FOUR_CHAR_CODE('alis')
};



struct DETProtoCallBackBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
};
typedef struct DETProtoCallBackBlock    DETProtoCallBackBlock;

struct DETBeepBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
};
typedef struct DETBeepBlock             DETBeepBlock;

struct DETBusyBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
};
typedef struct DETBusyBlock             DETBusyBlock;

struct DETChangeCallForsBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    long                            newCallFors;                /*  -> New call-for mask */
};
typedef struct DETChangeCallForsBlock   DETChangeCallForsBlock;

struct DETGetCommandSelectionCountBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    long                            count;                      /* <-  The number of items in the command selection list */
};
typedef struct DETGetCommandSelectionCountBlock DETGetCommandSelectionCountBlock;
enum {
    kDETHFSType                 = 0,                            /* HFS item type */
    kDETDSType                  = 1,                            /* Catalog Service item type */
    kDETMailType                = 2,                            /* Mail (letter) item type */
    kDETMoverType               = 3,                            /* Sounds, fonts, etc., from inside a suitcase or system file */
    kDETLastItemType            = (long)0xF0000000              /* Force it to be a long (C & C++ seem to disagree about the definition of 0xF000) */
};


typedef unsigned long                   DETItemType;
/* FSSpec plus possibly interesting additional info */

struct DETFSInfo {
    OSType                          fileType;                   /* File type */
    OSType                          fileCreator;                /* File creator */
    unsigned short                  fdFlags;                    /* Finder flags */
    FSSpec                          fsSpec;                     /* FSSpec */
};
typedef struct DETFSInfo                DETFSInfo;

struct DSRec {
    PackedDSSpecPtr *               dsSpec;                     /* <-  DSSpec for item (caller must DisposHandle() when done) */
    short                           refNum;                     /* <-  Refnum for returned address */
    AuthIdentity                    identity;                   /* <-  Identity for returned address */
};
typedef struct DSRec                    DSRec;

union ItemRec {
    DETFSInfo **                    fsInfo;                     /* <-  FSSpec & info for item (caller must DisposHandle() when done) */
    DSRec                           ds;
    PackedDSSpecPtr *               dsSpec;                     /* <-  DSSpec for item (caller must DisposHandle() when done) */
    LetterSpec **                   ltrSpec;                    /* <-  Letter spec for item (caller must DisposHandle() when done) */
};
typedef union ItemRec                   ItemRec;

struct DETGetCommandItemNBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    long                            itemNumber;                 /*  -> Item number to retrieve (1-based) */
    DETItemType                     itemType;                   /*  -> Type of item to be returned (if we can interpret it as such) */
    ItemRec                         item;
};
typedef struct DETGetCommandItemNBlock  DETGetCommandItemNBlock;

struct DETGetDSSpecBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    PackedDSSpecPtr *               dsSpec;                     /* <-  Handle with result (caller must DisposHandle() when done) */
    short                           refNum;                     /* <-  Refnum for address if PD */
    AuthIdentity                    identity;                   /* <-  Identity for address */
    Boolean                         isAlias;                    /* <-  True if this entry is an alias */
    Boolean                         isRecordRef;                /* <-  True if this entry is a record reference (reserved) */
};
typedef struct DETGetDSSpecBlock        DETGetDSSpecBlock;

struct DETGetTemplateFSSpecBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    FSSpec                          fsSpec;                     /* <-  FSSpec of template file */
    short                           baseID;                     /* <-  Base ID of this template */
    long                            aspectTemplateNumber;       /* <-  The template number for this aspect template */
};
typedef struct DETGetTemplateFSSpecBlock DETGetTemplateFSSpecBlock;

struct DETGetOpenEditBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           viewProperty;               /* <-  The property of the view being edited (or kNoProperty if none) */
};
typedef struct DETGetOpenEditBlock      DETGetOpenEditBlock;

struct DETCloseEditBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
};
typedef struct DETCloseEditBlock        DETCloseEditBlock;

struct DETGetPropertyTypeBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    short                           propertyType;               /* <-  The type of the property */
};
typedef struct DETGetPropertyTypeBlock  DETGetPropertyTypeBlock;

struct DETGetPropertyNumberBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    unsigned long                   propertyValue;              /* <-  The value of the property */
};
typedef struct DETGetPropertyNumberBlock DETGetPropertyNumberBlock;

struct DETGetPropertyRStringBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    RStringHandle                   propertyValue;              /* <-  A handle containing the property (as an RString) (caller must DisposHandle() when done) */
};
typedef struct DETGetPropertyRStringBlock DETGetPropertyRStringBlock;

struct DETGetPropertyBinarySizeBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    long                            propertyBinarySize;         /* <-  The size of the property as a binary block */
};
typedef struct DETGetPropertyBinarySizeBlock DETGetPropertyBinarySizeBlock;

struct DETGetPropertyBinaryBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    Handle                          propertyValue;              /* <-  Handle with the value of the property (caller must DisposHandle() when done) */
};
typedef struct DETGetPropertyBinaryBlock DETGetPropertyBinaryBlock;

struct DETGetPropertyChangedBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    Boolean                         propertyChanged;            /* <-  True if the property is marked as changed */
    Boolean                         filler1;
};
typedef struct DETGetPropertyChangedBlock DETGetPropertyChangedBlock;

struct DETGetPropertyEditableBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    Boolean                         propertyEditable;           /* <-  True if the property can be edited by the user (if false, view will appear disabled) */
    Boolean                         filler1;
};
typedef struct DETGetPropertyEditableBlock DETGetPropertyEditableBlock;

struct DETSetPropertyTypeBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    short                           newType;                    /*  -> New type for property (just sets type, does not convert contents) */
};
typedef struct DETSetPropertyTypeBlock  DETSetPropertyTypeBlock;

struct DETSetPropertyNumberBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    unsigned long                   newValue;                   /*  -> New value to set property to (and set type to number) */
};
typedef struct DETSetPropertyNumberBlock DETSetPropertyNumberBlock;

struct DETSetPropertyRStringBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    RStringPtr                      newValue;                   /*  -> New value to set property to (and set type to RString) */
};
typedef struct DETSetPropertyRStringBlock DETSetPropertyRStringBlock;

struct DETSetPropertyBinaryBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    void *                          newValue;                   /*  -> New value to set property to (and set type to binary) */
    long                            newValueSize;               /*  -> Size of new value */
};
typedef struct DETSetPropertyBinaryBlock DETSetPropertyBinaryBlock;

struct DETSetPropertyChangedBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    Boolean                         propertyChanged;            /*  -> Value to set changed flag on property to */
    Boolean                         filler1;
};
typedef struct DETSetPropertyChangedBlock DETSetPropertyChangedBlock;

struct DETSetPropertyEditableBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    Boolean                         propertyEditable;           /*  -> Value to set editable flag on property to */
    Boolean                         filler1;
};
typedef struct DETSetPropertyEditableBlock DETSetPropertyEditableBlock;

struct DETDirtyPropertyBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
};
typedef struct DETDirtyPropertyBlock    DETDirtyPropertyBlock;

struct DETDoPropertyCommandBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    long                            parameter;                  /* ->  Parameter of command */
};
typedef struct DETDoPropertyCommandBlock DETDoPropertyCommandBlock;

struct DETSublistCountBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    long                            count;                      /* <-  The number of items in the current item's sublist */
};
typedef struct DETSublistCountBlock     DETSublistCountBlock;

struct DETSelectedSublistCountBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    long                            count;                      /* <-  The number of selected items in the current item's sublist */
};
typedef struct DETSelectedSublistCountBlock DETSelectedSublistCountBlock;

struct DETRequestSyncBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
};
typedef struct DETRequestSyncBlock      DETRequestSyncBlock;

struct DETAddMenuBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    RString *                       name;                       /*  -> Name of new menu item */
    long                            parameter;                  /*  -> Parameter to return when this item is selected */
    long                            addAfter;                   /*  -> Parameter of entry to add after, or -1 for add at end */
};
typedef struct DETAddMenuBlock          DETAddMenuBlock;

struct DETRemoveMenuBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    long                            itemToRemove;               /*  -> Parameter of menu item to remove */
};
typedef struct DETRemoveMenuBlock       DETRemoveMenuBlock;

struct DETMenuItemRStringBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    long                            itemParameter;              /*  -> Parameter of menu item to return string for */
    RStringHandle                   rString;                    /* <-  Handle with the RString (caller must DisposHandle() when done) */
};
typedef struct DETMenuItemRStringBlock  DETMenuItemRStringBlock;

struct DETOpenDSSpecBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    PackedDSSpecPtr                 dsSpec;                     /*  -> DSSpec of object to be opened */
};
typedef struct DETOpenDSSpecBlock       DETOpenDSSpecBlock;

struct DETAboutToTalkBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
};
typedef struct DETAboutToTalkBlock      DETAboutToTalkBlock;

struct DETBreakAttributeBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    AttributePtr                    breakAttribute;             /*  -> Attribute to parse */
    Boolean                         isChangeable;               /*  -> True if the value can be changed by the user */
    Boolean                         filler1;
};
typedef struct DETBreakAttributeBlock   DETBreakAttributeBlock;

struct DETSavePropertyBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
};
typedef struct DETSavePropertyBlock     DETSavePropertyBlock;

struct DETGetCustomViewUserReferenceBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    short                           userReference;              /* <-  User reference value, as specified in the .r file */
};
typedef struct DETGetCustomViewUserReferenceBlock DETGetCustomViewUserReferenceBlock;

struct DETGetCustomViewBoundsBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    Rect                            bounds;                     /* <-  Bounds of the view */
};
typedef struct DETGetCustomViewBoundsBlock DETGetCustomViewBoundsBlock;

struct DETGetResourceBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    DETTargetSpecification          target;                     /* The target for the request */
    short                           property;                   /* The property to apply the request to */
    ResType                         resourceType;               /*  -> Resource type */
    Handle                          theResource;                /* <-  The resource handle (caller must dispose when done) */
};
typedef struct DETGetResourceBlock      DETGetResourceBlock;

struct DETTemplateCounts {
    DETCallBackFunctions            reqFunction;                /* Requested function */
    long                            aspectTemplateCount;        /* <-  Number of aspect templates in the system */
    long                            infoPageTemplateCount;      /* <-  Number of info-page templates in the system */
};
typedef struct DETTemplateCounts        DETTemplateCounts;

struct DETUnloadTemplatesBlock {
    DETCallBackFunctions            reqFunction;                /* Requested function */
};
typedef struct DETUnloadTemplatesBlock  DETUnloadTemplatesBlock;


union DETCallBackBlock {
    DETProtoCallBackBlock           protoCallBack;
    DETBeepBlock                    beep;
    DETBusyBlock                    busy;
    DETChangeCallForsBlock          changeCallFors;
    DETGetCommandSelectionCountBlock  getCommandSelectionCount;
    DETGetCommandItemNBlock         getCommandItemN;
    DETGetDSSpecBlock               getDSSpec;
    DETGetTemplateFSSpecBlock       getTemplateFSSpec;
    DETGetOpenEditBlock             getOpenEdit;
    DETCloseEditBlock               closeEdit;
    DETGetPropertyTypeBlock         getPropertyType;
    DETGetPropertyNumberBlock       getPropertyNumber;
    DETGetPropertyRStringBlock      getPropertyRString;
    DETGetPropertyBinarySizeBlock   getPropertyBinarySize;
    DETGetPropertyBinaryBlock       getPropertyBinary;
    DETGetPropertyChangedBlock      getPropertyChanged;
    DETGetPropertyEditableBlock     getPropertyEditable;
    DETSetPropertyTypeBlock         setPropertyType;
    DETSetPropertyNumberBlock       setPropertyNumber;
    DETSetPropertyRStringBlock      setPropertyRString;
    DETSetPropertyBinaryBlock       setPropertyBinary;
    DETSetPropertyChangedBlock      setPropertyChanged;
    DETSetPropertyEditableBlock     setPropertyEditable;
    DETDirtyPropertyBlock           dirtyProperty;
    DETDoPropertyCommandBlock       doPropertyCommand;
    DETSublistCountBlock            sublistCount;
    DETSelectedSublistCountBlock    selectedSublistCount;
    DETRequestSyncBlock             requestSync;
    DETAddMenuBlock                 addMenu;
    DETRemoveMenuBlock              removeMenu;
    DETMenuItemRStringBlock         menuItemRString;
    DETOpenDSSpecBlock              openDSSpec;
    DETAboutToTalkBlock             aboutToTalk;
    DETBreakAttributeBlock          breakAttribute;
    DETSavePropertyBlock            saveProperty;
    DETGetCustomViewUserReferenceBlock  getCustomViewUserReference;
    DETGetCustomViewBoundsBlock     getCustomViewBounds;
    DETGetResourceBlock             getResource;
    DETTemplateCounts               templateCounts;
    DETUnloadTemplatesBlock         unloadTemplates;
};
typedef union DETCallBackBlock          DETCallBackBlock;
typedef DETCallBackBlock *              DETCallBackBlockPtr;
typedef union DETCallBlock              DETCallBlock;

typedef DETCallBlock *                  DETCallBlockPtr;
typedef CALLBACK_API( OSErr , DETCallBackProcPtr )(DETCallBlock *callBlockPtr, DETCallBackBlockPtr callBackBlockPtr);
typedef STACK_UPP_TYPE(DETCallBackProcPtr)                      DETCallBackUPP;
typedef DETCallBackUPP                  DETCallBack;

struct DETProtoCallBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
};
typedef struct DETProtoCallBlock        DETProtoCallBlock;

struct DETInitBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, common to all invocations of code resource) */
    long                            newCallFors;                /* <-  New call-for mask */
};
typedef struct DETInitBlock             DETInitBlock;

struct DETExitBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, common to all invocations of code resource) */
};
typedef struct DETExitBlock             DETExitBlock;

struct DETInstanceInitBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
};
typedef struct DETInstanceInitBlock     DETInstanceInitBlock;

struct DETInstanceExitBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
};
typedef struct DETInstanceExitBlock     DETInstanceExitBlock;

struct DETInstanceIdleBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
};
typedef struct DETInstanceIdleBlock     DETInstanceIdleBlock;

struct DETPropertyCommandBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
    long                            parameter;                  /*  -> Parameter of command */
};
typedef struct DETPropertyCommandBlock  DETPropertyCommandBlock;

struct DETMaximumTextLengthBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
    long                            maxSize;                    /* <-  Return the maximum number of characters the user can entry when property is edited in an EditText */
};
typedef struct DETMaximumTextLengthBlock DETMaximumTextLengthBlock;

struct DETViewListChangedBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
};
typedef struct DETViewListChangedBlock  DETViewListChangedBlock;

struct DETPropertyDirtiedBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
};
typedef struct DETPropertyDirtiedBlock  DETPropertyDirtiedBlock;

struct DETValidateSaveBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    RStringHandle                   errorString;                /* <-  Handle with error string if validation fails (callee must allocate handle, DE will DisposHandle() it) */
};
typedef struct DETValidateSaveBlock     DETValidateSaveBlock;

struct DETDropQueryBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           modifiers;                  /*  -> Modifiers at drop time (option/control/command/shift keys) */
    long                            commandID;                  /* <-> Command ID (kDETDoNothing, kDETMove, kDETDrag (copy), kDETAlias, or a property number) */
    AttributeType                   destinationType;            /* <-> Type to convert attribute to */
    Boolean                         copyToHFS;                  /* <-  If true, object should be copied to HFS before being operated on, and deleted after */
    Boolean                         filler2;
};
typedef struct DETDropQueryBlock        DETDropQueryBlock;

struct DETDropMeQueryBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           modifiers;                  /*  -> Modifiers at drop time (option/control/command/shift keys) */
    long                            commandID;                  /* <-> Command ID (kDETDoNothing, kDETMove, kDETDrag (copy), kDETAlias, or a property number) */
    AttributeType                   destinationType;            /* <-> Type to convert attribute to */
    Boolean                         copyToHFS;                  /* <-  If true, object should be copied to HFS before being operated on, and deleted after */
    Boolean                         filler2;
};
typedef struct DETDropMeQueryBlock      DETDropMeQueryBlock;

struct DETAttributeCreationBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, common to all invocations of code resource) */
    PackedDSSpecPtr                 parent;                     /*  -> The object within which the creation will occur */
    short                           refNum;                     /*  -> Refnum for returned address (DSSpecs in PDs only) */
    AuthIdentity                    identity;                   /*  -> The identity we're browsing as in the parent object */
    AttributeType                   attrType;                   /* <-> The type of the attribute being created */
    AttributeTag                    attrTag;                    /* <-> The tag of the attribute being created */
    Handle                          value;                      /* <-> The value to write (pre-allocated, resize as needed) */
};
typedef struct DETAttributeCreationBlock DETAttributeCreationBlock;

struct DETAttributeNewBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    PackedDSSpecPtr                 parent;                     /*  -> The object within which the creation will occur */
    short                           refNum;                     /*  -> Refnum for returned address (DSSpecs in PDs only) */
    AuthIdentity                    identity;                   /*  -> The identity we're browsing as in the parent object */
    AttributeType                   attrType;                   /* <-> The type of the attribute being created */
    AttributeTag                    attrTag;                    /* <-> The tag of the attribute being created */
    Handle                          value;                      /* <-> The value to write (pre-allocated, resize as needed) */
};
typedef struct DETAttributeNewBlock     DETAttributeNewBlock;

struct DETAttributeChangeBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    PackedDSSpecPtr                 parent;                     /*  -> The object within which the creation will occur */
    short                           refNum;                     /*  -> Refnum for returned address (DSSpecs in PDs only) */
    AuthIdentity                    identity;                   /*  -> The identity we're browsing as in the parent object */
    AttributeType                   attrType;                   /* <-> The type of the attribute being changed */
    AttributeTag                    attrTag;                    /* <-> The tag of the attribute being changed */
    AttributeCreationID             attrCID;                    /* <-> The CID of the attribute being changed */
    Handle                          value;                      /* <-> The value to write (pre-allocated, resize as needed) */
};
typedef struct DETAttributeChangeBlock  DETAttributeChangeBlock;

struct DETAttributeDeleteBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    PackedDSSpecPtr                 dsSpec;                     /*  -> The object which will be deleted */
    short                           refNum;                     /*  -> Refnum for returned address (DSSpecs in PDs only) */
    AuthIdentity                    identity;                   /*  -> The identity we're browsing as */
};
typedef struct DETAttributeDeleteBlock  DETAttributeDeleteBlock;

struct DETItemNewBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
};
typedef struct DETItemNewBlock          DETItemNewBlock;

struct DETShouldSyncBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    Boolean                         shouldSync;                 /* <-  True if we should now sync with catalog */
    Boolean                         filler2;
};
typedef struct DETShouldSyncBlock       DETShouldSyncBlock;

struct DETDoSyncBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
};
typedef struct DETDoSyncBlock           DETDoSyncBlock;

struct DETPatternInBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
    long                            elementType;                /*  -> Element type from pattern */
    long                            extra;                      /*  -> Extra field from pattern */
    AttributePtr                    attribute;                  /*  -> The complete attribute */
    long                            dataOffset;                 /* <-> Offset to current (next) byte */
    short                           bitOffset;                  /* <-> Bit offset (next bit is *fData >> fBitOffset++) */
};
typedef struct DETPatternInBlock        DETPatternInBlock;

struct DETPatternOutBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
    long                            elementType;                /*  -> Element type from pattern */
    long                            extra;                      /*  -> Extra field from pattern */
    AttributePtr                    attribute;                  /*  -> The attribute (minus the data portion) */
    Handle                          data;                       /*  -> Data to be written (pre-allocated, resize and add at end) */
    long                            dataOffset;                 /* <-> Offset to next byte to write */
    short                           bitOffset;                  /* <-> Bit offset (if zero, handle will need to be resized to one more byte before write) */
};
typedef struct DETPatternOutBlock       DETPatternOutBlock;

struct DETOpenSelfBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           modifiers;                  /*  -> Modifiers at open time (option/control/command/shift keys) */
};
typedef struct DETOpenSelfBlock         DETOpenSelfBlock;

struct DETConvertToNumberBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
    long                            theValue;                   /* <-  The converted value to return */
};
typedef struct DETConvertToNumberBlock  DETConvertToNumberBlock;

struct DETConvertToRStringBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
    RStringHandle                   theValue;                   /* <-  A handle with the converted value (callee must allocate handle, DE will DisposHandle() it) */
};
typedef struct DETConvertToRStringBlock DETConvertToRStringBlock;

struct DETConvertFromNumberBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
    long                            theValue;                   /*  -> The value to convert (result should be written direct to the property) */
};
typedef struct DETConvertFromNumberBlock DETConvertFromNumberBlock;

struct DETConvertFromRStringBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
    RStringPtr                      theValue;                   /*  -> The value to convert (result should be written direct to the property) */
};
typedef struct DETConvertFromRStringBlock DETConvertFromRStringBlock;

struct DETCustomViewDrawBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
};
typedef struct DETCustomViewDrawBlock   DETCustomViewDrawBlock;

struct DETCustomViewMouseDownBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
    EventRecord *                   theEvent;                   /*  -> The original event record of the mouse-down */
};
typedef struct DETCustomViewMouseDownBlock DETCustomViewMouseDownBlock;

struct DETKeyPressBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
    EventRecord *                   theEvent;                   /*  -> The original event record of the key-press */
};
typedef struct DETKeyPressBlock         DETKeyPressBlock;

struct DETPasteBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           property;                   /* The property number the call refers to */
    short                           modifiers;                  /*  -> Modifiers at paste time (option/control/command/shift keys) */
};
typedef struct DETPasteBlock            DETPasteBlock;

struct DETCustomMenuSelectedBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           menuTableParameter;         /*  -> The "property" field from the custom menu table */
};
typedef struct DETCustomMenuSelectedBlock DETCustomMenuSelectedBlock;

struct DETCustomMenuEnabledBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    short                           menuTableParameter;         /*  -> The "property" field from the custom menu table */
    Boolean                         enable;                     /* <-  Whether to enable the menu item */
    Boolean                         filler2;
};
typedef struct DETCustomMenuEnabledBlock DETCustomMenuEnabledBlock;

struct DETForwarderListItem {
    struct DETForwarderListItem **  next;                       /* Pointer to next item, or nil */
    AttributeTag                    attributeValueTag;          /* Tag of new templates (0 for none) */
    PackedPathName                  rstrs;                      /* Record type (empty if none), attrbute type (empty if none),list of template names to forward to */
};
typedef struct DETForwarderListItem     DETForwarderListItem;

typedef DETForwarderListItem *          DETForwarderListPtr;
typedef DETForwarderListPtr *           DETForwarderListHandle;

struct DETDynamicForwardersBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, common to all invocations of code resource) */
    DETForwarderListHandle          forwarders;                 /* <-  List of forwaders */
};
typedef struct DETDynamicForwardersBlock DETDynamicForwardersBlock;

struct DETDynamicResourceBlock {
    DETCallFunctions                reqFunction;                /* Requested function */
    DETCallBack                     callBack;                   /* Pointer to call-back routine */
    long                            callBackPrivate;            /* Private data for the call-back routine */
    long                            templatePrivate;            /* Private storage for use by code resource (stays for life of code resource, */
                                                                /*    common to all invocations of code resource) */
    long                            instancePrivate;            /* Private storage for use by code resource (separate for each item using the */
                                                                /*    code resource) */
    DETTargetSpecification          target;                     /* The target (originator) of the call, for targeted and property calls */
    Boolean                         targetIsMainAspect;         /* True if the target is the main aspect (even though it has a non-nil name) */
    Boolean                         filler1;
    ResType                         resourceType;               /*  -> The resource type being requested */
    short                           propertyNumber;             /*  -> The property number of the resource being requested */
    short                           resourceID;                 /*  -> The resource ID (base ID + property number) of the resource */
    Handle                          theResource;                /* <-  The requested resource */
};
typedef struct DETDynamicResourceBlock  DETDynamicResourceBlock;


union DETCallBlock {
    DETProtoCallBlock               protoCall;
    DETInitBlock                    init;
    DETExitBlock                    exit;
    DETInstanceInitBlock            instanceInit;
    DETInstanceExitBlock            instanceExit;
    DETInstanceIdleBlock            instanceIdle;
    DETPropertyCommandBlock         propertyCommand;
    DETMaximumTextLengthBlock       maximumTextLength;
    DETViewListChangedBlock         viewListChanged;
    DETPropertyDirtiedBlock         propertyDirtied;
    DETValidateSaveBlock            validateSave;
    DETDropQueryBlock               dropQuery;
    DETDropMeQueryBlock             dropMeQuery;
    DETAttributeCreationBlock       attributeCreationBlock;
    DETAttributeNewBlock            attributeNew;
    DETAttributeChangeBlock         attributeChange;
    DETAttributeDeleteBlock         attributeDelete;
    DETItemNewBlock                 itemNew;
    DETPatternInBlock               patternIn;
    DETPatternOutBlock              patternOut;
    DETShouldSyncBlock              shouldSync;
    DETDoSyncBlock                  doSync;
    DETOpenSelfBlock                openSelf;
    DETConvertToNumberBlock         convertToNumber;
    DETConvertToRStringBlock        convertToRString;
    DETConvertFromNumberBlock       convertFromNumber;
    DETConvertFromRStringBlock      convertFromRString;
    DETCustomViewDrawBlock          customViewDraw;
    DETCustomViewMouseDownBlock     customViewMouseDown;
    DETKeyPressBlock                keyPress;
    DETPasteBlock                   paste;
    DETCustomMenuSelectedBlock      customMenuSelected;
    DETCustomMenuEnabledBlock       customMenuEnabled;
    DETDynamicForwardersBlock       dynamicForwarders;
    DETDynamicResourceBlock         dynamicResource;
};

#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(DETCallBackUPP)
    NewDETCallBackUPP              (DETCallBackProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeDETCallBackUPP          (DETCallBackUPP          userUPP);

    EXTERN_API(OSErr)
    InvokeDETCallBackUPP           (DETCallBlock *          callBlockPtr,
                                    DETCallBackBlockPtr     callBackBlockPtr,
                                    DETCallBackUPP          userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppDETCallBackProcInfo = 0x000003E0 };                   /* pascal 2_bytes Func(4_bytes, 4_bytes) */
    #define NewDETCallBackUPP(userRoutine)                          (DETCallBackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDETCallBackProcInfo, GetCurrentArchitecture())
    #define DisposeDETCallBackUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeDETCallBackUPP(callBlockPtr, callBackBlockPtr, userUPP)  (OSErr)CALL_TWO_PARAMETER_UPP((userUPP), uppDETCallBackProcInfo, (callBlockPtr), (callBackBlockPtr))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewDETCallBackProc(userRoutine)                         NewDETCallBackUPP(userRoutine)
#define CallDETCallBackProc(userRoutine, callBlockPtr, callBackBlockPtr) InvokeDETCallBackUPP(callBlockPtr, callBackBlockPtr, userRoutine)
/* Call-for list: */
enum {
    kDETCallForOther            = 1,                            /* Call for things not listed below (also auto-enabled by DE if any of the below are enabled) */
    kDETCallForIdle             = 2,                            /* kDETcmdIdle */
    kDETCallForCommands         = 4,                            /* kDETcmdPropertyCommand, kDETcmdSelfOpen */
    kDETCallForViewChanges      = 8,                            /* kDETcmdViewListChanged, kDETcmdPropertyDirtied, kDETcmdMaximumTextLength */
    kDETCallForDrops            = 0x10,                         /* kDETcmdDropQuery, kDETcmdDropMeQuery */
    kDETCallForAttributes       = 0x20,                         /* kDETcmdAttributeCreation, kDETcmdAttributeNew, kDETcmdAttributeChange, kDETcmdAttributeDelete */
    kDETCallForValidation       = 0x40,                         /* kDETcmdValidateSave */
    kDETCallForKeyPresses       = 0x80,                         /* kDETcmdKeyPress and kDETcmdPaste */
    kDETCallForResources        = 0x0100,                       /* kDETcmdDynamicResource */
    kDETCallForSyncing          = 0x0200,                       /* kDETcmdShouldSync, kDETcmdDoSync */
    kDETCallForEscalation       = 0x8000,                       /* All calls escalated from the next lower level */
    kDETCallForNothing          = 0,                            /* None of the above */
    kDETCallForEverything       = (long)0xFFFFFFFF              /* All of the above */
};

typedef CALLBACK_API( OSErr , DETCallProcPtr )(DETCallBlockPtr callBlockPtr);
typedef STACK_UPP_TYPE(DETCallProcPtr)                          DETCallUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(DETCallUPP)
    NewDETCallUPP                  (DETCallProcPtr          userRoutine);

    EXTERN_API(void)
    DisposeDETCallUPP              (DETCallUPP              userUPP);

    EXTERN_API(OSErr)
    InvokeDETCallUPP               (DETCallBlockPtr         callBlockPtr,
                                    DETCallUPP              userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppDETCallProcInfo = 0x000000E0 };                       /* pascal 2_bytes Func(4_bytes) */
    #define NewDETCallUPP(userRoutine)                              (DETCallUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppDETCallProcInfo, GetCurrentArchitecture())
    #define DisposeDETCallUPP(userUPP)                              DisposeRoutineDescriptor(userUPP)
    #define InvokeDETCallUPP(callBlockPtr, userUPP)                 (OSErr)CALL_ONE_PARAMETER_UPP((userUPP), uppDETCallProcInfo, (callBlockPtr))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewDETCallProc(userRoutine)                             NewDETCallUPP(userRoutine)
#define CallDETCallProc(userRoutine, callBlockPtr)              InvokeDETCallUPP(callBlockPtr, userRoutine)

typedef DETCallUPP                      DETCall;
/* This following macro saves you from having to dig out the call-back pointer from the call block: */
#define CallBackDET(callBlockPtr, callBackBlockPtr) CallDETCallBackProc(callBlockPtr->protoCall.callBack,callBlockPtr,callBackBlockPtr)
#endif /* REZ */

/************************************************************************************/
/********************************* SAM Definitions: *********************************/
/************************************************************************************/

/* SAM Developers should use property numbers starting at this point: */
#define kSAMFirstDevProperty                kDETFirstDevProperty + 10


/*
 SAM templates have additional resources/properties that are required
   for interaction with the AOCE Key Chain.
   
    Type   Offset                      Description
     ----   ------                      -----------
    'rstr'  kSAMAspectUserName          The user name
  'rstr'  kSAMAspectKind              The kind of SAM
    'detn'  kSAMAspectCannotDelete      If 0, then the slot cannot be deleted
  'sami'  kSAMAspectSlotCreationInfo  The info required to create a slot record
*/

#define kSAMAspectUserName                        kDETFirstDevProperty + 1
#define kSAMAspectKind                         kDETFirstDevProperty + 2
#define kSAMAspectCannotDelete                 kDETFirstDevProperty + 3
#define kSAMAspectSlotCreationInfo             kDETFirstDevProperty + 4

/**************************************************************************************
 ********************************* Admin Definitions: *********************************
 **************************************************************************************/

#define kDETAdminVersion        -978




#ifndef REZ

#endif /* REZ */

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

#endif /* __OCETEMPLATES__ */

