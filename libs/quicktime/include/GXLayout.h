/*
     File:       GXLayout.h
 
     Contains:   QuickDraw GX layout routine interfaces.
 
     Version:    Technology: Quickdraw GX 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __GXLAYOUT__
#define __GXLAYOUT__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __GXTYPES__
#include <GXTypes.h>
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
EXTERN_API_C( gxShape )
GXNewLayout                     (long                   textRunCount,
                                 const short            textRunLengths[],
                                 const void *           text[],
                                 long                   styleRunCount,
                                 const short            styleRunLengths[],
                                 const gxStyle          styles[],
                                 long                   levelRunCount,
                                 const short            levelRunLengths[],
                                 const short            levels[],
                                 const gxLayoutOptions * layoutOptions,
                                 const gxPoint *        position)                           THREEWORDINLINE(0x303C, 0x0000, 0xA832);

EXTERN_API_C( long )
GXGetLayout                     (gxShape                layout,
                                 void *                 text,
                                 long *                 styleRunCount,
                                 short                  styleRunLengths[],
                                 gxStyle                styles[],
                                 long *                 levelRunCount,
                                 short                  levelRunLengths[],
                                 short                  levels[],
                                 gxLayoutOptions *      layoutOptions,
                                 gxPoint *              position)                           THREEWORDINLINE(0x303C, 0x0001, 0xA832);

EXTERN_API_C( void )
GXSetLayout                     (gxShape                layout,
                                 long                   textRunCount,
                                 const short            textRunLengths[],
                                 const void *           text[],
                                 long                   styleRunCount,
                                 const short            styleRunLengths[],
                                 const gxStyle          styles[],
                                 long                   levelRunCount,
                                 const short            levelRunLengths[],
                                 const short            levels[],
                                 const gxLayoutOptions * layoutOptions,
                                 const gxPoint *        position)                           THREEWORDINLINE(0x303C, 0x0002, 0xA832);

EXTERN_API_C( void )
GXDrawLayout                    (long                   textRunCount,
                                 const short            textRunLengths[],
                                 const void *           text[],
                                 long                   styleRunCount,
                                 const short            styleRunLengths[],
                                 const gxStyle          styles[],
                                 long                   levelRunCount,
                                 const short            levelRunLengths[],
                                 const short            levels[],
                                 const gxLayoutOptions * layoutOptions,
                                 const gxPoint *        position)                           THREEWORDINLINE(0x303C, 0x0003, 0xA832);

EXTERN_API_C( void )
GXSetLayoutParts                (gxShape                layout,
                                 gxByteOffset           oldStartOffset,
                                 gxByteOffset           oldEndOffset,
                                 long                   newTextRunCount,
                                 const short            newTextRunLengths[],
                                 const void *           newText[],
                                 long                   newStyleRunCount,
                                 const short            newStyleRunLengths[],
                                 const gxStyle          newStyles[],
                                 long                   newLevelRunCount,
                                 const short            newLevelRunLengths[],
                                 const short            newLevels[])                          THREEWORDINLINE(0x303C, 0x0004, 0xA832);

EXTERN_API_C( void )
GXSetLayoutShapeParts           (gxShape                layout,
                                 gxByteOffset           startOffset,
                                 gxByteOffset           endOffset,
                                 gxShape                insert)                             THREEWORDINLINE(0x303C, 0x0005, 0xA832);

EXTERN_API_C( long )
GXGetLayoutParts                (gxShape                layout,
                                 gxByteOffset           startOffset,
                                 gxByteOffset           endOffset,
                                 void *                 text,
                                 long *                 styleRunCount,
                                 short                  styleRunLengths[],
                                 gxStyle                styles[],
                                 long *                 levelRunCount,
                                 short                  levelRunLengths[],
                                 short                  levels[])                             THREEWORDINLINE(0x303C, 0x0006, 0xA832);

EXTERN_API_C( gxShape )
GXGetLayoutShapeParts           (gxShape                layout,
                                 gxByteOffset           startOffset,
                                 gxByteOffset           endOffset,
                                 gxShape                dest)                               THREEWORDINLINE(0x303C, 0x0007, 0xA832);

EXTERN_API_C( long )
GXGetLayoutGlyphs               (gxShape                layout,
                                 gxGlyphcode *          glyphs,
                                 gxPoint                positions[],
                                 long                   advance[],
                                 gxPoint                tangents[],
                                 long *                 runCount,
                                 short                  styleRuns[],
                                 gxStyle                glyphStyles[])                        THREEWORDINLINE(0x303C, 0x0008, 0xA832);

EXTERN_API_C( gxByteOffset )
GXHitTestLayout                 (gxShape                layout,
                                 const gxPoint *        hitDown,
                                 gxHighlightType        highlightType,
                                 gxLayoutHitInfo *      hitInfo,
                                 gxShape                hitTrackingArea)                    THREEWORDINLINE(0x303C, 0x0009, 0xA832);

EXTERN_API_C( gxShape )
GXGetLayoutHighlight            (gxShape                layout,
                                 gxByteOffset           startOffset,
                                 gxByteOffset           endOffset,
                                 gxHighlightType        highlightType,
                                 gxShape                highlight)                          THREEWORDINLINE(0x303C, 0x000A, 0xA832);

EXTERN_API_C( gxShape )
GXGetLayoutVisualHighlight      (gxShape                layout,
                                 gxByteOffset           startOffset,
                                 long                   startLeadingEdge,
                                 gxByteOffset           endOffset,
                                 long                   endLeadingEdge,
                                 gxHighlightType        highlightType,
                                 gxShape                highlight)                          THREEWORDINLINE(0x303C, 0x000B, 0xA832);

EXTERN_API_C( gxShape )
GXGetLayoutCaret                (gxShape                layout,
                                 gxByteOffset           offset,
                                 gxHighlightType        highlightType,
                                 gxCaretType            caretType,
                                 gxShape                caret)                              THREEWORDINLINE(0x303C, 0x000C, 0xA832);

EXTERN_API_C( gxByteOffset )
GXGetLayoutBreakOffset          (gxShape                layout,
                                 gxByteOffset           startOffset,
                                 Fixed                  lineWidth,
                                 long                   hyphenationCount,
                                 const gxByteOffset     hyphenationPoints[],
                                 Boolean *              startIsStaked,
                                 gxByteOffset *         priorStake,
                                 gxByteOffset *         nextStake)                          THREEWORDINLINE(0x303C, 0x000D, 0xA832);

EXTERN_API_C( Fixed )
GXGetLayoutRangeWidth           (gxShape                layout,
                                 gxByteOffset           startOffset,
                                 gxByteOffset           endOffset,
                                 gxShape                supplementaryText)                  THREEWORDINLINE(0x303C, 0x000E, 0xA832);

EXTERN_API_C( gxShape )
GXNewLayoutFromRange            (gxShape                layout,
                                 gxByteOffset           startOffset,
                                 gxByteOffset           endOffset,
                                 const gxLayoutOptions * layoutOptions,
                                 gxShape                supplementaryText)                  THREEWORDINLINE(0x303C, 0x000F, 0xA832);

EXTERN_API_C( gxShape )
GXGetCaretAngleArea             (gxShape                layout,
                                 const gxPoint *        hitPoint,
                                 gxHighlightType        highlightType,
                                 gxShape                caretArea,
                                 short *                returnedRise,
                                 short *                returnedRun)                        THREEWORDINLINE(0x303C, 0x0010, 0xA832);

EXTERN_API_C( void )
GXGetStyleBaselineDeltas        (gxStyle                baseStyle,
                                 gxBaselineType         baseType,
                                 gxBaselineDeltas       returnedDeltas)                     THREEWORDINLINE(0x303C, 0x0011, 0xA832);

EXTERN_API_C( gxByteOffset )
GXGetRightVisualOffset          (gxShape                layout,
                                 gxByteOffset           currentOffset)                      THREEWORDINLINE(0x303C, 0x0012, 0xA832);

EXTERN_API_C( gxByteOffset )
GXGetLeftVisualOffset           (gxShape                layout,
                                 gxByteOffset           currentOffset)                      THREEWORDINLINE(0x303C, 0x0013, 0xA832);

EXTERN_API_C( void )
GXGetCompoundCharacterLimits    (gxShape                layout,
                                 gxByteOffset           trial,
                                 gxByteOffset *         minOffset,
                                 gxByteOffset *         maxOffset,
                                 Boolean *              onBoundary)                         THREEWORDINLINE(0x303C, 0x0014, 0xA832);

EXTERN_API_C( void )
GXGetOffsetGlyphs               (gxShape                layout,
                                 gxByteOffset           trial,
                                 long                   leadingEdge,
                                 gxLayoutOffsetState *  offsetState,
                                 unsigned short *       firstGlyph,
                                 unsigned short *       secondGlyph)                        THREEWORDINLINE(0x303C, 0x0015, 0xA832);

EXTERN_API_C( void )
GXGetGlyphOffset                (gxShape                layout,
                                 long                   trial,
                                 long                   onLeftTop,
                                 gxByteOffset *         offset,
                                 Boolean *              leadingEdge,
                                 Boolean *              wasRealCharacter)                   THREEWORDINLINE(0x303C, 0x0016, 0xA832);

EXTERN_API_C( void )
GXGetLayoutSpan                 (gxShape                layout,
                                 Fixed *                lineAscent,
                                 Fixed *                lineDescent)                        THREEWORDINLINE(0x303C, 0x0017, 0xA832);

EXTERN_API_C( void )
GXSetLayoutSpan                 (gxShape                layout,
                                 Fixed                  lineAscent,
                                 Fixed                  lineDescent)                        THREEWORDINLINE(0x303C, 0x0018, 0xA832);

EXTERN_API_C( Fixed )
GXGetLayoutJustificationGap     (gxShape                layout)                             THREEWORDINLINE(0x303C, 0x0279, 0xA832);

EXTERN_API_C( void )
GXGetLayoutJustificationFactors (gxShape                layout,
                                 Fixed                  constrainedFactors[],
                                 Fixed                  unconstrainedFactors[])               THREEWORDINLINE(0x303C, 0x027A, 0xA832);

EXTERN_API_C( void )
GXSetStyleRunControls           (gxStyle                target,
                                 const gxRunControls *  runControls)                        THREEWORDINLINE(0x303C, 0x0019, 0xA832);

EXTERN_API_C( void )
GXSetStyleRunPriorityJustOverride (gxStyle              target,
                                 const gxPriorityJustificationOverride * priorityJustificationOverride) THREEWORDINLINE(0x303C, 0x001A, 0xA832);

EXTERN_API_C( void )
GXSetStyleRunGlyphJustOverrides (gxStyle                target,
                                 long                   count,
                                 const gxGlyphJustificationOverride  glyphJustificationOverrides[]) THREEWORDINLINE(0x303C, 0x001B, 0xA832);

EXTERN_API_C( void )
GXSetStyleRunGlyphSubstitutions (gxStyle                target,
                                 long                   count,
                                 const gxGlyphSubstitution  glyphSubstitutions[])             THREEWORDINLINE(0x303C, 0x001C, 0xA832);

EXTERN_API_C( void )
GXSetStyleRunFeatures           (gxStyle                target,
                                 long                   count,
                                 const gxRunFeature     runFeatures[])                        THREEWORDINLINE(0x303C, 0x001D, 0xA832);

EXTERN_API_C( void )
GXSetStyleRunKerningAdjustments (gxStyle                target,
                                 long                   count,
                                 const gxKerningAdjustment  kerningAdjustments[])             THREEWORDINLINE(0x303C, 0x001E, 0xA832);

EXTERN_API_C( void )
GXSetShapeRunControls           (gxShape                target,
                                 const gxRunControls *  runControls)                        THREEWORDINLINE(0x303C, 0x001F, 0xA832);

EXTERN_API_C( void )
GXSetShapeRunPriorityJustOverride (gxShape              target,
                                 const gxPriorityJustificationOverride * priorityJustificationOverride) THREEWORDINLINE(0x303C, 0x0020, 0xA832);

EXTERN_API_C( void )
GXSetShapeRunGlyphJustOverrides (gxShape                target,
                                 long                   count,
                                 const gxGlyphJustificationOverride  glyphJustificationOverrides[]) THREEWORDINLINE(0x303C, 0x0021, 0xA832);

EXTERN_API_C( void )
GXSetShapeRunGlyphSubstitutions (gxShape                target,
                                 long                   count,
                                 const gxGlyphSubstitution  glyphSubstitutions[])             THREEWORDINLINE(0x303C, 0x0022, 0xA832);

EXTERN_API_C( void )
GXSetShapeRunFeatures           (gxShape                target,
                                 long                   count,
                                 const gxRunFeature     runFeatures[])                        THREEWORDINLINE(0x303C, 0x0023, 0xA832);

EXTERN_API_C( void )
GXSetShapeRunKerningAdjustments (gxShape                target,
                                 long                   count,
                                 const gxKerningAdjustment  kerningAdjustments[])             THREEWORDINLINE(0x303C, 0x0024, 0xA832);

EXTERN_API_C( long )
GXGetStyleRunControls           (gxStyle                source,
                                 gxRunControls *        runControls)                        THREEWORDINLINE(0x303C, 0x0025, 0xA832);

EXTERN_API_C( long )
GXGetStyleRunPriorityJustOverride (gxStyle              source,
                                 gxPriorityJustificationOverride * priorityJustificationOverride) THREEWORDINLINE(0x303C, 0x0026, 0xA832);

EXTERN_API_C( long )
GXGetStyleRunGlyphJustOverrides (gxStyle                source,
                                 gxGlyphJustificationOverride  glyphJustificationOverrides[]) THREEWORDINLINE(0x303C, 0x0027, 0xA832);

EXTERN_API_C( long )
GXGetStyleRunGlyphSubstitutions (gxStyle                source,
                                 gxGlyphSubstitution    glyphSubstitutions[])                 THREEWORDINLINE(0x303C, 0x0028, 0xA832);

EXTERN_API_C( long )
GXGetStyleRunFeatures           (gxStyle                source,
                                 gxRunFeature           runFeatures[])                        THREEWORDINLINE(0x303C, 0x0029, 0xA832);

EXTERN_API_C( long )
GXGetStyleRunKerningAdjustments (gxStyle                source,
                                 gxKerningAdjustment    kerningAdjustments[])                 THREEWORDINLINE(0x303C, 0x002A, 0xA832);

EXTERN_API_C( long )
GXGetShapeRunControls           (gxShape                source,
                                 gxRunControls *        runControls)                        THREEWORDINLINE(0x303C, 0x002B, 0xA832);

EXTERN_API_C( long )
GXGetShapeRunPriorityJustOverride (gxShape              source,
                                 gxPriorityJustificationOverride * priorityJustificationOverride) THREEWORDINLINE(0x303C, 0x002C, 0xA832);

EXTERN_API_C( long )
GXGetShapeRunGlyphJustOverrides (gxShape                source,
                                 gxGlyphJustificationOverride  glyphJustificationOverrides[]) THREEWORDINLINE(0x303C, 0x002D, 0xA832);

EXTERN_API_C( long )
GXGetShapeRunGlyphSubstitutions (gxShape                source,
                                 gxGlyphSubstitution    glyphSubstitutions[])                 THREEWORDINLINE(0x303C, 0x002E, 0xA832);

EXTERN_API_C( long )
GXGetShapeRunFeatures           (gxShape                source,
                                 gxRunFeature           runFeatures[])                        THREEWORDINLINE(0x303C, 0x002F, 0xA832);

EXTERN_API_C( long )
GXGetShapeRunKerningAdjustments (gxShape                source,
                                 gxKerningAdjustment    kerningAdjustments[])                 THREEWORDINLINE(0x303C, 0x0030, 0xA832);


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

#endif /* __GXLAYOUT__ */

