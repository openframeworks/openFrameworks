/*
     File:       QTContentRestrictions.h
 
     Contains:   QuickTime Interfaces.
 
     Version:    Technology: QuickTime 6.0
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1990-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
/*****
    Content Restrictions
*****/
#ifndef __QTCONTENTRESTRICTIONS__
#define __QTCONTENTRESTRICTIONS__



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

enum {
    kQTRestrictionClassSave     = FOUR_CHAR_CODE('save'),
    kQTRestrictionSaveDontAddMovieResource = (1L << 0),
    kQTRestrictionSaveDontFlatten = (1L << 1),
    kQTRestrictionSaveDontExport = (1L << 2),
    kQTRestrictionSaveDontExtract = (1L << 3),                  /* don't allow any form of extraction of content*/
    kQTRestrictionClassEdit     = FOUR_CHAR_CODE('edit'),
    kQTRestrictionEditDontCopy  = (1L << 0),                    /* disable copy */
    kQTRestrictionEditDontCut   = (1L << 1),                    /* disable cut */
    kQTRestrictionEditDontPaste = (1L << 2),                    /* disable paste */
    kQTRestrictionEditDontClear = (1L << 3),                    /* disable clear*/
    kQTRestrictionEditDontModify = (1L << 4),                   /* don't allow modification of content*/
    kQTRestrictionEditDontExtract = (1L << 5)                   /* don't allow any form of extraction of content*/
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

#endif /* __QTCONTENTRESTRICTIONS__ */

