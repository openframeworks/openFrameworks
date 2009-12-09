/*
     File:       GXFonts.h
 
     Contains:   QuickDraw GX font routine interfaces.
 
     Version:    Technology: Quickdraw GX 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __GXFONTS__
#define __GXFONTS__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __GXMATH__
#include <GXMath.h>
#endif

#ifndef __GXTYPES__
#include <GXTypes.h>
#endif

#ifndef __SCALERSTREAMTYPES__
#include <ScalerStreamTypes.h>
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

#if defined(__MWERKS__) && TARGET_CPU_68K
    #pragma push
    #pragma pointers_in_D0
#endif
 
#if CALL_NOT_IN_CARBON
EXTERN_API_C( gxFont )
GXNewFont                       (gxFontStorageTag       storage,
                                 gxFontStorageReference  reference,
                                 gxFontAttribute        attributes)                         THREEWORDINLINE(0x303C, 0x0201, 0xA832);

EXTERN_API_C( gxFontStorageTag )
GXGetFont                       (gxFont                 fontID,
                                 gxFontStorageReference * reference,
                                 gxFontAttribute *      attributes)                         THREEWORDINLINE(0x303C, 0x0202, 0xA832);

EXTERN_API_C( gxFont )
GXFindFont                      (gxFontStorageTag       storage,
                                 gxFontStorageReference  reference,
                                 gxFontAttribute *      attributes)                         THREEWORDINLINE(0x303C, 0x0203, 0xA832);

EXTERN_API_C( void )
GXSetFont                       (gxFont                 fontID,
                                 gxFontStorageTag       storage,
                                 gxFontStorageReference  reference,
                                 gxFontAttribute        attributes)                         THREEWORDINLINE(0x303C, 0x0204, 0xA832);

EXTERN_API_C( void )
GXDisposeFont                   (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x0205, 0xA832);

EXTERN_API_C( void )
GXChangedFont                   (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x0206, 0xA832);

EXTERN_API_C( gxFontFormatTag )
GXGetFontFormat                 (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x0207, 0xA832);

EXTERN_API_C( gxFont )
GXGetDefaultFont                (void)                                                      THREEWORDINLINE(0x303C, 0x0208, 0xA832);

EXTERN_API_C( gxFont )
GXSetDefaultFont                (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x0209, 0xA832);

EXTERN_API_C( long )
GXFindFonts                     (gxFont                 familyID,
                                 gxFontName             name,
                                 gxFontPlatform         platform,
                                 gxFontScript           script,
                                 gxFontLanguage         language,
                                 long                   length,
                                 const unsigned char    text[],
                                 long                   index,
                                 long                   count,
                                 gxFont                 fonts[])                              THREEWORDINLINE(0x303C, 0x020A, 0xA832);

EXTERN_API_C( long )
GXCountFontGlyphs               (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x020B, 0xA832);

EXTERN_API_C( long )
GXCountFontTables               (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x020C, 0xA832);

EXTERN_API_C( long )
GXGetFontTable                  (gxFont                 fontID,
                                 long                   index,
                                 void *                 tableData,
                                 gxFontTableTag *       tableTag)                           THREEWORDINLINE(0x303C, 0x020D, 0xA832);

EXTERN_API_C( long )
GXFindFontTable                 (gxFont                 fontID,
                                 gxFontTableTag         tableTag,
                                 void *                 tableData,
                                 long *                 index)                              THREEWORDINLINE(0x303C, 0x020E, 0xA832);

EXTERN_API_C( long )
GXGetFontTableParts             (gxFont                 fontID,
                                 long                   index,
                                 long                   offset,
                                 long                   length,
                                 void *                 tableData,
                                 gxFontTableTag *       tableTag)                           THREEWORDINLINE(0x303C, 0x020F, 0xA832);

EXTERN_API_C( long )
GXFindFontTableParts            (gxFont                 fontID,
                                 gxFontTableTag         tableTag,
                                 long                   offset,
                                 long                   length,
                                 void *                 tableData,
                                 long *                 index)                              THREEWORDINLINE(0x303C, 0x0210, 0xA832);

EXTERN_API_C( long )
GXSetFontTable                  (gxFont                 fontID,
                                 long                   index,
                                 gxFontTableTag         tableTag,
                                 long                   length,
                                 const void *           tableData)                          THREEWORDINLINE(0x303C, 0x0211, 0xA832);

EXTERN_API_C( long )
GXSetFontTableParts             (gxFont                 fontID,
                                 long                   index,
                                 gxFontTableTag         tableTag,
                                 long                   offset,
                                 long                   oldLength,
                                 long                   newLength,
                                 const void *           tableData)                          THREEWORDINLINE(0x303C, 0x0212, 0xA832);

EXTERN_API_C( long )
GXDeleteFontTable               (gxFont                 fontID,
                                 long                   index,
                                 gxFontTableTag         tableTag)                           THREEWORDINLINE(0x303C, 0x0213, 0xA832);

EXTERN_API_C( long )
GXCountFontNames                (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x0214, 0xA832);

EXTERN_API_C( long )
GXGetFontName                   (gxFont                 fontID,
                                 long                   index,
                                 gxFontName *           name,
                                 gxFontPlatform *       platform,
                                 gxFontScript *         script,
                                 gxFontLanguage *       language,
                                 unsigned char          text[])                               THREEWORDINLINE(0x303C, 0x0215, 0xA832);

EXTERN_API_C( long )
GXFindFontName                  (gxFont                 fontID,
                                 gxFontName             name,
                                 gxFontPlatform         platform,
                                 gxFontScript           script,
                                 gxFontLanguage         language,
                                 unsigned char          text[],
                                 long *                 index)                              THREEWORDINLINE(0x303C, 0x0216, 0xA832);

EXTERN_API_C( long )
GXSetFontName                   (gxFont                 fontID,
                                 gxFontName             name,
                                 gxFontPlatform         platform,
                                 gxFontScript           script,
                                 gxFontLanguage         language,
                                 long                   length,
                                 const unsigned char    text[])                               THREEWORDINLINE(0x303C, 0x0217, 0xA832);

EXTERN_API_C( long )
GXDeleteFontName                (gxFont                 fontID,
                                 long                   index,
                                 gxFontName             name,
                                 gxFontPlatform         platform,
                                 gxFontScript           script,
                                 gxFontLanguage         language)                           THREEWORDINLINE(0x303C, 0x0218, 0xA832);

EXTERN_API_C( gxFontName )
GXNewFontNameID                 (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x0219, 0xA832);

EXTERN_API_C( long )
GXCountFontEncodings            (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x021A, 0xA832);

EXTERN_API_C( gxFontPlatform )
GXGetFontEncoding               (gxFont                 fontID,
                                 long                   index,
                                 gxFontScript *         script,
                                 gxFontLanguage *       language)                           THREEWORDINLINE(0x303C, 0x021B, 0xA832);

EXTERN_API_C( long )
GXFindFontEncoding              (gxFont                 fontID,
                                 gxFontPlatform         platform,
                                 gxFontScript           script,
                                 gxFontLanguage         language)                           THREEWORDINLINE(0x303C, 0x021C, 0xA832);

EXTERN_API_C( long )
GXApplyFontEncoding             (gxFont                 fontID,
                                 long                   index,
                                 long *                 length,
                                 const unsigned char    text[],
                                 long                   count,
                                 unsigned short         glyphs[],
                                 char                   was16Bit[])                           THREEWORDINLINE(0x303C, 0x021D, 0xA832);

EXTERN_API_C( long )
GXCountFontVariations           (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x021E, 0xA832);

EXTERN_API_C( long )
GXFindFontVariation             (gxFont                 fontID,
                                 gxFontVariationTag     variationTag,
                                 Fixed *                minValue,
                                 Fixed *                defaultValue,
                                 Fixed *                maxValue,
                                 gxFontName *           name)                               THREEWORDINLINE(0x303C, 0x021F, 0xA832);

EXTERN_API_C( gxFontVariationTag )
GXGetFontVariation              (gxFont                 fontID,
                                 long                   index,
                                 Fixed *                minValue,
                                 Fixed *                defaultValue,
                                 Fixed *                maxValue,
                                 gxFontName *           name)                               THREEWORDINLINE(0x303C, 0x0220, 0xA832);

EXTERN_API_C( long )
GXCountFontInstances            (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x0221, 0xA832);

EXTERN_API_C( gxFontName )
GXGetFontInstance               (gxFont                 fontID,
                                 long                   index,
                                 gxFontVariation        variation[])                          THREEWORDINLINE(0x303C, 0x0222, 0xA832);

EXTERN_API_C( long )
GXSetFontInstance               (gxFont                 fontID,
                                 long                   index,
                                 gxFontName             name,
                                 const gxFontVariation  variation[])                          THREEWORDINLINE(0x303C, 0x0223, 0xA832);

EXTERN_API_C( long )
GXDeleteFontInstance            (gxFont                 fontID,
                                 long                   index,
                                 gxFontName             name)                               THREEWORDINLINE(0x303C, 0x0224, 0xA832);

EXTERN_API_C( long )
GXCountFontDescriptors          (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x0225, 0xA832);

EXTERN_API_C( gxFontDescriptorTag )
GXGetFontDescriptor             (gxFont                 fontID,
                                 long                   index,
                                 Fixed *                descriptorValue)                    THREEWORDINLINE(0x303C, 0x0226, 0xA832);

EXTERN_API_C( long )
GXFindFontDescriptor            (gxFont                 fontID,
                                 gxFontDescriptorTag    descriptorTag,
                                 Fixed *                descriptorValue)                    THREEWORDINLINE(0x303C, 0x0227, 0xA832);

EXTERN_API_C( long )
GXSetFontDescriptor             (gxFont                 fontID,
                                 long                   index,
                                 gxFontDescriptorTag    descriptorTag,
                                 Fixed                  descriptorValue)                    THREEWORDINLINE(0x303C, 0x0228, 0xA832);

EXTERN_API_C( long )
GXDeleteFontDescriptor          (gxFont                 fontID,
                                 long                   index,
                                 gxFontDescriptorTag    descriptorTag)                      THREEWORDINLINE(0x303C, 0x0229, 0xA832);

EXTERN_API_C( long )
GXCountFontFeatures             (gxFont                 fontID)                             THREEWORDINLINE(0x303C, 0x022A, 0xA832);

EXTERN_API_C( gxFontName )
GXGetFontFeature                (gxFont                 fontID,
                                 long                   index,
                                 gxFontFeatureFlag *    flags,
                                 long *                 settingCount,
                                 gxFontFeatureSetting   settings[],
                                 gxFontFeature *        feature)                            THREEWORDINLINE(0x303C, 0x022B, 0xA832);

EXTERN_API_C( gxFontName )
GXFindFontFeature               (gxFont                 fontID,
                                 gxFontFeature          feature,
                                 gxFontFeatureFlag *    flags,
                                 long *                 settingCount,
                                 gxFontFeatureSetting   settings[],
                                 long *                 index)                              THREEWORDINLINE(0x303C, 0x022C, 0xA832);

EXTERN_API_C( long )
GXGetFontDefaultFeatures        (gxFont                 fontID,
                                 gxRunFeature           features[])                           THREEWORDINLINE(0x303C, 0x0274, 0xA832);

EXTERN_API_C( void )
GXFlattenFont                   (gxFont                 source,
                                 scalerStream *         stream,
                                 gxSpoolBlock *         block)                              THREEWORDINLINE(0x303C, 0x022D, 0xA832);


#endif  /* CALL_NOT_IN_CARBON */

 
#if defined(__MWERKS__) && TARGET_CPU_68K
    #pragma pop
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

#endif /* __GXFONTS__ */

