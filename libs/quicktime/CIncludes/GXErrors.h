/*
     File:       GXErrors.h
 
     Contains:   QuickDraw GX error constants and debugging routines
 
     Version:    Technology: Quickdraw GX 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1994-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __GXERRORS__
#define __GXERRORS__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
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
 
enum {
    gxFirstSystemError          = -27999,                       /* 0xffff92a1 */
    gxFirstFatalError           = -27999,
    gxLastFatalError            = -27951,
    gxFirstNonfatalError        = -27950,
    gxFirstFontScalerError      = -27900,
    gxLastFontScalerError       = -27851,
    gxFirstParameterError       = -27850,
    gxFirstImplementationLimitError = -27800,
    gxFirstSystemDebuggingError = -27700,
    gxLastSystemError           = -27000,                       /* 0xffff9688 */
    gxFirstLibraryError         = 1048576L,                     /* 0x00100000 */
    gxLastLibraryError          = 2097151L,                     /* 0x001fffff */
    gxFirstAppError             = 2097152L,                     /* 0x00200000 */
    gxLastAppError              = 4194303L,                     /* 0x003fffff */
    gxFirstSystemWarning        = -26999,                       /* 0xffff9689 */
    gxFirstResultOutOfRangeWarning = -26950,
    gxFirstParameterOutOfRangeWarning = -26900,
    gxFirstFontScalerWarning    = -26850,
    gxFirstSystemDebuggingWarning = -26700,
    gxLastSystemWarning         = -26000,                       /* 0xffff9a70 */
    gxFirstLibraryWarning       = 4194304L,                     /* 0x00400000 */
    gxLastLibraryWarning        = 5242879L,                     /* 0x004fffff */
    gxFirstAppWarning           = 5242880L,                     /* 0x00500000 */
    gxLastAppWarning            = 7340031L,                     /* 0x006fffff */
    gxFirstSystemNotice         = -25999,                       /* 0xffff9a71 */
    gxLastSystemNotice          = -25500,                       /* 0xffff9c64 */
    gxFirstLibraryNotice        = 7340032L,                     /* 0x00700000 */
    gxLastLibraryNotice         = 7602175L,                     /* 0x0073ffff */
    gxFirstAppNotice            = 7602176L,                     /* 0x00740000 */
    gxLastAppNotice             = 8388607L                      /* 0x007fffff */
};

enum {
                                                                /* truly fatal errors */
    out_of_memory               = gxFirstFatalError,
    internal_fatal_error        = gxFirstFatalError + 1,
    no_outline_font_found       = gxFirstFatalError + 2,
    not_enough_memory_for_graphics_client_heap = gxFirstFatalError + 3,
    could_not_create_backing_store = gxFirstFatalError + 4,     /* internal errors */
    internal_error              = gxFirstNonfatalError,
    internal_font_error         = gxFirstNonfatalError + 1,
    internal_layout_error       = gxFirstNonfatalError + 2,     /* recoverable errors */
    could_not_dispose_backing_store = internal_layout_error + 2,
    unflattening_interrupted_by_client = internal_layout_error + 3, /* font manager errors */
    font_cannot_be_changed      = internal_layout_error + 4,
    illegal_font_parameter      = internal_layout_error + 5,    /* gxFont scaler errors */
    null_font_scaler_context    = gxFirstFontScalerError,
    null_font_scaler_input      = gxFirstFontScalerError + 1,
    invalid_font_scaler_context = gxFirstFontScalerError + 2,
    invalid_font_scaler_input   = gxFirstFontScalerError + 3,
    invalid_font_scaler_font_data = gxFirstFontScalerError + 4,
    font_scaler_newblock_failed = gxFirstFontScalerError + 5,
    font_scaler_getfonttable_failed = gxFirstFontScalerError + 6,
    font_scaler_bitmap_allocation_failed = gxFirstFontScalerError + 7,
    font_scaler_outline_allocation_failed = gxFirstFontScalerError + 8,
    required_font_scaler_table_missing = gxFirstFontScalerError + 9,
    unsupported_font_scaler_outline_format = gxFirstFontScalerError + 10,
    unsupported_font_scaler_stream_format = gxFirstFontScalerError + 11,
    unsupported_font_scaler_font_format = gxFirstFontScalerError + 12,
    font_scaler_hinting_error   = gxFirstFontScalerError + 13,
    font_scaler_rasterizer_error = gxFirstFontScalerError + 14,
    font_scaler_internal_error  = gxFirstFontScalerError + 15,
    font_scaler_invalid_matrix  = gxFirstFontScalerError + 16,
    font_scaler_fixed_overflow  = gxFirstFontScalerError + 17,
    font_scaler_api_version_mismatch = gxFirstFontScalerError + 18,
    font_scaler_streaming_aborted = gxFirstFontScalerError + 19,
    unknown_font_scaler_error   = gxFirstFontScalerError + 20,  /* bad parameters */
    parameter_is_nil            = gxFirstParameterError,
    shape_is_nil                = gxFirstParameterError + 1,
    style_is_nil                = gxFirstParameterError + 2,
    transform_is_nil            = gxFirstParameterError + 3,
    ink_is_nil                  = gxFirstParameterError + 4,
    transferMode_is_nil         = gxFirstParameterError + 5,
    color_is_nil                = gxFirstParameterError + 6,
    colorProfile_is_nil         = gxFirstParameterError + 7,
    colorSet_is_nil             = gxFirstParameterError + 8,
    spoolProcedure_is_nil       = gxFirstParameterError + 9,
    tag_is_nil                  = gxFirstParameterError + 10,
    type_is_nil                 = gxFirstParameterError + 11,
    mapping_is_nil              = gxFirstParameterError + 12,
    invalid_viewDevice_reference = gxFirstParameterError + 13,
    invalid_viewGroup_reference = gxFirstParameterError + 14,
    invalid_viewPort_reference  = gxFirstParameterError + 15,   /* implementation limits, these should be right before the debugging errors */
    number_of_contours_exceeds_implementation_limit = gxFirstImplementationLimitError,
    number_of_points_exceeds_implementation_limit = gxFirstImplementationLimitError + 1,
    size_of_polygon_exceeds_implementation_limit = gxFirstImplementationLimitError + 2,
    size_of_path_exceeds_implementation_limit = gxFirstImplementationLimitError + 3,
    size_of_text_exceeds_implementation_limit = gxFirstImplementationLimitError + 4,
    size_of_bitmap_exceeds_implementation_limit = gxFirstImplementationLimitError + 5,
    number_of_colors_exceeds_implementation_limit = gxFirstImplementationLimitError + 6,
    procedure_not_reentrant     = gxFirstImplementationLimitError + 7
};



enum {
                                                                /* internal debugging errors */
    functionality_unimplemented = gxFirstSystemDebuggingError,
    clip_to_frame_shape_unimplemented = gxFirstSystemDebuggingError + 1, /* font parameter debugging errors */
    illegal_font_storage_type   = gxFirstSystemDebuggingError + 2,
    illegal_font_storage_reference = gxFirstSystemDebuggingError + 3,
    illegal_font_attributes     = gxFirstSystemDebuggingError + 4, /* parameter debugging errors */
    parameter_out_of_range      = gxFirstSystemDebuggingError + 5,
    inconsistent_parameters     = gxFirstSystemDebuggingError + 6,
    index_is_less_than_zero     = gxFirstSystemDebuggingError + 7,
    index_is_less_than_one      = gxFirstSystemDebuggingError + 8,
    count_is_less_than_zero     = gxFirstSystemDebuggingError + 9,
    count_is_less_than_one      = gxFirstSystemDebuggingError + 10,
    contour_is_less_than_zero   = gxFirstSystemDebuggingError + 11,
    length_is_less_than_zero    = gxFirstSystemDebuggingError + 12,
    invalid_client_reference    = gxFirstSystemDebuggingError + 13,
    invalid_graphics_heap_start_pointer = gxFirstSystemDebuggingError + 14,
    invalid_nongraphic_globals_pointer = gxFirstSystemDebuggingError + 15,
    colorSpace_out_of_range     = gxFirstSystemDebuggingError + 16,
    pattern_lattice_out_of_range = gxFirstSystemDebuggingError + 17,
    frequency_parameter_out_of_range = gxFirstSystemDebuggingError + 18,
    tinting_parameter_out_of_range = gxFirstSystemDebuggingError + 19,
    method_parameter_out_of_range = gxFirstSystemDebuggingError + 20,
    space_may_not_be_indexed    = gxFirstSystemDebuggingError + 21,
    glyph_index_too_small       = gxFirstSystemDebuggingError + 22,
    no_glyphs_added_to_font     = gxFirstSystemDebuggingError + 23,
    glyph_not_added_to_font     = gxFirstSystemDebuggingError + 24,
    point_does_not_intersect_bitmap = gxFirstSystemDebuggingError + 25,
    required_font_table_not_present = gxFirstSystemDebuggingError + 26,
    unknown_font_table_format   = gxFirstSystemDebuggingError + 27, /* the styles encoding is not present in the font */
    shapeFill_not_allowed       = gxFirstSystemDebuggingError + 28,
    inverseFill_face_must_set_clipLayer_flag = gxFirstSystemDebuggingError + 29,
    invalid_transferMode_colorSpace = gxFirstSystemDebuggingError + 30,
    colorProfile_must_be_nil    = gxFirstSystemDebuggingError + 31,
    bitmap_pixel_size_must_be_1 = gxFirstSystemDebuggingError + 32,
    empty_shape_not_allowed     = gxFirstSystemDebuggingError + 33,
    ignorePlatformShape_not_allowed = gxFirstSystemDebuggingError + 34,
    nil_style_in_glyph_not_allowed = gxFirstSystemDebuggingError + 35,
    complex_glyph_style_not_allowed = gxFirstSystemDebuggingError + 36,
    invalid_mapping             = gxFirstSystemDebuggingError + 37,
    cannot_set_item_shapes_to_nil = gxFirstSystemDebuggingError + 38,
    cannot_use_original_item_shapes_when_growing_picture = gxFirstSystemDebuggingError + 39,
    cannot_add_unspecified_new_glyphs = gxFirstSystemDebuggingError + 40,
    cannot_dispose_locked_tag   = gxFirstSystemDebuggingError + 41,
    cannot_dispose_locked_shape = gxFirstSystemDebuggingError + 42, /* restricted access */
    shape_access_not_allowed    = gxFirstSystemDebuggingError + 43,
    colorSet_access_restricted  = gxFirstSystemDebuggingError + 44,
    colorProfile_access_restricted = gxFirstSystemDebuggingError + 45,
    tag_access_restricted       = gxFirstSystemDebuggingError + 46,
    viewDevice_access_restricted = gxFirstSystemDebuggingError + 47,
    graphic_type_does_not_have_a_structure = gxFirstSystemDebuggingError + 48,
    style_run_array_does_not_match_number_of_characters = gxFirstSystemDebuggingError + 49,
    rectangles_cannot_be_inserted_into = gxFirstSystemDebuggingError + 50,
    unknown_graphics_heap       = gxFirstSystemDebuggingError + 51,
    graphics_routine_selector_is_obsolete = gxFirstSystemDebuggingError + 52,
    cannot_set_graphics_client_memory_without_setting_size = gxFirstSystemDebuggingError + 53,
    graphics_client_memory_too_small = gxFirstSystemDebuggingError + 54,
    graphics_client_memory_is_already_allocated = gxFirstSystemDebuggingError + 55,
    viewPort_is_a_window        = gxFirstSystemDebuggingError + 56, /* wrong type/bad reference */
    illegal_type_for_shape      = gxFirstSystemDebuggingError + 57,
    shape_does_not_contain_a_bitmap = gxFirstSystemDebuggingError + 58,
    shape_does_not_contain_text = gxFirstSystemDebuggingError + 59,
    picture_expected            = gxFirstSystemDebuggingError + 60,
    bitmap_is_not_resizable     = gxFirstSystemDebuggingError + 61,
    shape_may_not_be_a_bitmap   = gxFirstSystemDebuggingError + 62,
    shape_may_not_be_a_picture  = gxFirstSystemDebuggingError + 63,
    graphic_type_does_not_contain_points = gxFirstSystemDebuggingError + 64,
    graphic_type_does_not_have_multiple_contours = gxFirstSystemDebuggingError + 65,
    graphic_type_cannot_be_mapped = gxFirstSystemDebuggingError + 66,
    graphic_type_cannot_be_moved = gxFirstSystemDebuggingError + 67,
    graphic_type_cannot_be_scaled = gxFirstSystemDebuggingError + 68,
    graphic_type_cannot_be_rotated = gxFirstSystemDebuggingError + 69,
    graphic_type_cannot_be_skewed = gxFirstSystemDebuggingError + 70,
    graphic_type_cannot_be_reset = gxFirstSystemDebuggingError + 71,
    graphic_type_cannot_be_dashed = gxFirstSystemDebuggingError + 72,
    graphic_type_cannot_be_reduced = gxFirstSystemDebuggingError + 73,
    graphic_type_cannot_be_inset = gxFirstSystemDebuggingError + 74,
    shape_cannot_be_inverted    = gxFirstSystemDebuggingError + 75,
    shape_does_not_have_area    = gxFirstSystemDebuggingError + 76,
    shape_does_not_have_length  = gxFirstSystemDebuggingError + 77,
    first_glyph_advance_must_be_absolute = gxFirstSystemDebuggingError + 78,
    picture_cannot_contain_itself = gxFirstSystemDebuggingError + 79,
    viewPort_cannot_contain_itself = gxFirstSystemDebuggingError + 80,
    cannot_set_unique_items_attribute_when_picture_contains_items = gxFirstSystemDebuggingError + 81,
    layer_style_cannot_contain_a_face = gxFirstSystemDebuggingError + 82,
    layer_glyph_shape_cannot_contain_nil_styles = gxFirstSystemDebuggingError + 83, /* validation errors */
    object_wrong_type           = gxFirstSystemDebuggingError + 84,
    shape_wrong_type            = gxFirstSystemDebuggingError + 85,
    style_wrong_type            = gxFirstSystemDebuggingError + 86,
    ink_wrong_type              = gxFirstSystemDebuggingError + 87,
    transform_wrong_type        = gxFirstSystemDebuggingError + 88,
    device_wrong_type           = gxFirstSystemDebuggingError + 89,
    port_wrong_type             = gxFirstSystemDebuggingError + 90, /* validation cache errors */
    shape_cache_wrong_type      = gxFirstSystemDebuggingError + 91,
    style_cache_wrong_type      = gxFirstSystemDebuggingError + 92,
    ink_cache_wrong_type        = gxFirstSystemDebuggingError + 93,
    transform_cache_wrong_type  = gxFirstSystemDebuggingError + 94,
    port_cache_wrong_type       = gxFirstSystemDebuggingError + 95,
    shape_cache_parent_mismatch = gxFirstSystemDebuggingError + 96,
    style_cache_parent_mismatch = gxFirstSystemDebuggingError + 97,
    ink_cache_parent_mismatch   = gxFirstSystemDebuggingError + 98,
    transform_cache_parent_mismatch = gxFirstSystemDebuggingError + 99,
    port_cache_parent_mismatch  = gxFirstSystemDebuggingError + 100,
    invalid_shape_cache_port    = gxFirstSystemDebuggingError + 101,
    invalid_shape_cache_device  = gxFirstSystemDebuggingError + 102,
    invalid_ink_cache_port      = gxFirstSystemDebuggingError + 103,
    invalid_ink_cache_device    = gxFirstSystemDebuggingError + 104,
    invalid_style_cache_port    = gxFirstSystemDebuggingError + 105,
    invalid_style_cache_device  = gxFirstSystemDebuggingError + 106,
    invalid_transform_cache_port = gxFirstSystemDebuggingError + 107,
    invalid_transform_cache_device = gxFirstSystemDebuggingError + 108,
    recursive_caches            = gxFirstSystemDebuggingError + 109, /* validation shape cache errors */
    invalid_fillShape_ownerCount = gxFirstSystemDebuggingError + 110,
    recursive_fillShapes        = gxFirstSystemDebuggingError + 111, /* validation memory block errors */
    indirect_memory_block_too_small = gxFirstSystemDebuggingError + 112,
    indirect_memory_block_too_large = gxFirstSystemDebuggingError + 113,
    unexpected_nil_pointer      = gxFirstSystemDebuggingError + 114,
    bad_address                 = gxFirstSystemDebuggingError + 115, /* validation object errors */
    no_owners                   = gxFirstSystemDebuggingError + 116,
    invalid_pointer             = gxFirstSystemDebuggingError + 117,
    invalid_seed                = gxFirstSystemDebuggingError + 118,
    invalid_frame_seed          = gxFirstSystemDebuggingError + 119,
    invalid_text_seed           = gxFirstSystemDebuggingError + 120,
    invalid_draw_seed           = gxFirstSystemDebuggingError + 121,
    bad_private_flags           = gxFirstSystemDebuggingError + 122, /* validation path and polygon errors */
    invalid_vector_count        = gxFirstSystemDebuggingError + 123,
    invalid_contour_count       = gxFirstSystemDebuggingError + 124, /* validation bitmap errors */
    bitmap_ptr_too_small        = gxFirstSystemDebuggingError + 125,
    bitmap_ptr_not_aligned      = gxFirstSystemDebuggingError + 126,
    bitmap_rowBytes_negative    = gxFirstSystemDebuggingError + 127,
    bitmap_width_negative       = gxFirstSystemDebuggingError + 128,
    bitmap_height_negative      = gxFirstSystemDebuggingError + 129,
    invalid_pixelSize           = gxFirstSystemDebuggingError + 130,
    bitmap_rowBytes_too_small   = gxFirstSystemDebuggingError + 131,
    bitmap_rowBytes_not_aligned = gxFirstSystemDebuggingError + 132,
    bitmap_rowBytes_must_be_specified_for_user_image_buffer = gxFirstSystemDebuggingError + 133, /* validation bitmap image errors */
    invalid_bitImage_fileOffset = gxFirstSystemDebuggingError + 134,
    invalid_bitImage_owners     = gxFirstSystemDebuggingError + 135,
    invalid_bitImage_rowBytes   = gxFirstSystemDebuggingError + 136,
    invalid_bitImage_internal_flag = gxFirstSystemDebuggingError + 137, /* validation text errors */
    text_bounds_cache_wrong_size = gxFirstSystemDebuggingError + 138,
    text_metrics_cache_wrong_size = gxFirstSystemDebuggingError + 139,
    text_index_cache_wrong_size = gxFirstSystemDebuggingError + 140, /* validation glyph errors */
    glyph_run_count_negative    = gxFirstSystemDebuggingError + 141,
    glyph_run_count_zero        = gxFirstSystemDebuggingError + 142,
    glyph_run_counts_do_not_sum_to_character_count = gxFirstSystemDebuggingError + 143,
    glyph_first_advance_bit_set_not_allowed = gxFirstSystemDebuggingError + 144,
    glyph_tangent_vectors_both_zero = gxFirstSystemDebuggingError + 145, /* validation layout errors */
    layout_run_length_negative  = gxFirstSystemDebuggingError + 146,
    layout_run_length_zero      = gxFirstSystemDebuggingError + 147,
    layout_run_level_negative   = gxFirstSystemDebuggingError + 148,
    layout_run_lengths_do_not_sum_to_text_length = gxFirstSystemDebuggingError + 149, /* validation picture errors */
    bad_shape_in_picture        = gxFirstSystemDebuggingError + 150,
    bad_style_in_picture        = gxFirstSystemDebuggingError + 151,
    bad_ink_in_picture          = gxFirstSystemDebuggingError + 152,
    bad_transform_in_picture    = gxFirstSystemDebuggingError + 153,
    bad_shape_cache_in_picture  = gxFirstSystemDebuggingError + 154,
    bad_seed_in_picture         = gxFirstSystemDebuggingError + 155,
    invalid_picture_count       = gxFirstSystemDebuggingError + 156, /* validation text face errors */
    bad_textLayer_count         = gxFirstSystemDebuggingError + 157,
    bad_fillType_in_textFace    = gxFirstSystemDebuggingError + 158,
    bad_style_in_textFace       = gxFirstSystemDebuggingError + 159,
    bad_transform_in_textFace   = gxFirstSystemDebuggingError + 160, /* validation transform errors */
    invalid_matrix_flag         = gxFirstSystemDebuggingError + 161,
    transform_clip_missing      = gxFirstSystemDebuggingError + 162, /* validation font cache errors */
    metrics_wrong_type          = gxFirstSystemDebuggingError + 163,
    metrics_point_size_probably_bad = gxFirstSystemDebuggingError + 164,
    scalar_block_wrong_type     = gxFirstSystemDebuggingError + 165,
    scalar_block_parent_mismatch = gxFirstSystemDebuggingError + 166,
    scalar_block_too_small      = gxFirstSystemDebuggingError + 167,
    scalar_block_too_large      = gxFirstSystemDebuggingError + 168,
    invalid_metrics_range       = gxFirstSystemDebuggingError + 169,
    invalid_metrics_flags       = gxFirstSystemDebuggingError + 170,
    metrics_maxWidth_probably_bad = gxFirstSystemDebuggingError + 171,
    font_wrong_type             = gxFirstSystemDebuggingError + 172,
    font_wrong_size             = gxFirstSystemDebuggingError + 173,
    invalid_font_platform       = gxFirstSystemDebuggingError + 174,
    invalid_lookup_range        = gxFirstSystemDebuggingError + 175,
    invalid_lookup_platform     = gxFirstSystemDebuggingError + 176,
    font_not_in_font_list       = gxFirstSystemDebuggingError + 177,
    metrics_not_in_metrics_list = gxFirstSystemDebuggingError + 178, /* validation view device errors */
    bad_device_private_flags    = gxFirstSystemDebuggingError + 179,
    bad_device_attributes       = gxFirstSystemDebuggingError + 180,
    invalid_device_number       = gxFirstSystemDebuggingError + 181,
    invalid_device_viewGroup    = gxFirstSystemDebuggingError + 182,
    invalid_device_bounds       = gxFirstSystemDebuggingError + 183,
    invalid_bitmap_in_device    = gxFirstSystemDebuggingError + 184, /* validation color set errors */
    colorSet_wrong_type         = gxFirstSystemDebuggingError + 185,
    invalid_colorSet_viewDevice_owners = gxFirstSystemDebuggingError + 186,
    invalid_colorSet_colorSpace = gxFirstSystemDebuggingError + 187,
    invalid_colorSet_count      = gxFirstSystemDebuggingError + 188, /* validation color profile errors */
    colorProfile_wrong_type     = gxFirstSystemDebuggingError + 189,
    invalid_colorProfile_flags  = gxFirstSystemDebuggingError + 190,
    invalid_colorProfile_response_count = gxFirstSystemDebuggingError + 191, /* validation internal backing store errors */
    backing_free_parent_mismatch = gxFirstSystemDebuggingError + 192,
    backing_store_parent_mismatch = gxFirstSystemDebuggingError + 193
};


enum {
                                                                /* warnings about warnings */
    warning_stack_underflow     = gxFirstSystemWarning,
    warning_stack_overflow      = gxFirstSystemWarning + 1,
    notice_stack_underflow      = gxFirstSystemWarning + 2,
    notice_stack_overflow       = gxFirstSystemWarning + 3,
    about_to_grow_heap          = gxFirstSystemWarning + 4,
    about_to_unload_objects     = gxFirstSystemWarning + 5,     /* result went out of range */
    map_shape_out_of_range      = gxFirstResultOutOfRangeWarning,
    move_shape_out_of_range     = gxFirstResultOutOfRangeWarning + 1,
    scale_shape_out_of_range    = gxFirstResultOutOfRangeWarning + 2,
    rotate_shape_out_of_range   = gxFirstResultOutOfRangeWarning + 3,
    skew_shape_out_of_range     = gxFirstResultOutOfRangeWarning + 4,
    map_transform_out_of_range  = gxFirstResultOutOfRangeWarning + 5,
    move_transform_out_of_range = gxFirstResultOutOfRangeWarning + 6,
    scale_transform_out_of_range = gxFirstResultOutOfRangeWarning + 7,
    rotate_transform_out_of_range = gxFirstResultOutOfRangeWarning + 8,
    skew_transform_out_of_range = gxFirstResultOutOfRangeWarning + 9,
    map_points_out_of_range     = gxFirstResultOutOfRangeWarning + 10, /* gave a parameter out of range */
    contour_out_of_range        = gxFirstParameterOutOfRangeWarning,
    index_out_of_range_in_contour = gxFirstParameterOutOfRangeWarning + 1,
    picture_index_out_of_range  = gxFirstParameterOutOfRangeWarning + 2,
    color_index_requested_not_found = gxFirstParameterOutOfRangeWarning + 3,
    colorSet_index_out_of_range = gxFirstParameterOutOfRangeWarning + 4,
    index_out_of_range          = gxFirstParameterOutOfRangeWarning + 5,
    count_out_of_range          = gxFirstParameterOutOfRangeWarning + 6,
    length_out_of_range         = gxFirstParameterOutOfRangeWarning + 7,
    font_table_index_out_of_range = gxFirstParameterOutOfRangeWarning + 8,
    font_glyph_index_out_of_range = gxFirstParameterOutOfRangeWarning + 9,
    point_out_of_range          = gxFirstParameterOutOfRangeWarning + 10,
    profile_response_out_of_range = gxFirstParameterOutOfRangeWarning + 11, /* gxFont scaler warnings */
    font_scaler_no_output       = gxFirstFontScalerWarning,
    font_scaler_fake_metrics    = gxFirstFontScalerWarning + 1,
    font_scaler_fake_linespacing = gxFirstFontScalerWarning + 2,
    font_scaler_glyph_substitution = gxFirstFontScalerWarning + 3,
    font_scaler_no_kerning_applied = gxFirstFontScalerWarning + 4, /* might not be what you expected */
    character_substitution_took_place = gxFirstFontScalerWarning + 5,
    unable_to_get_bounds_on_multiple_devices = gxFirstFontScalerWarning + 6,
    font_language_not_found     = gxFirstFontScalerWarning + 7,
    font_not_found_during_unflattening = gxFirstFontScalerWarning + 8, /*storage */
    unrecognized_stream_version = gxFirstFontScalerWarning + 9,
    bad_data_in_stream          = gxFirstFontScalerWarning + 10
};


enum {
                                                                /* nonsense data */
    new_shape_contains_invalid_data = gxFirstSystemDebuggingWarning,
    new_tag_contains_invalid_data = gxFirstSystemDebuggingWarning + 1,
    extra_data_passed_was_ignored = gxFirstSystemDebuggingWarning + 2,
    font_table_not_found        = gxFirstSystemDebuggingWarning + 3,
    font_name_not_found         = gxFirstSystemDebuggingWarning + 4, /* doesn't make sense to do */
    unable_to_traverse_open_contour_that_starts_or_ends_off_the_curve = gxFirstSystemDebuggingWarning + 5,
    unable_to_draw_open_contour_that_starts_or_ends_off_the_curve = gxFirstSystemDebuggingWarning + 6,
    cannot_dispose_default_shape = gxFirstSystemDebuggingWarning + 7,
    cannot_dispose_default_style = gxFirstSystemDebuggingWarning + 8,
    cannot_dispose_default_ink  = gxFirstSystemDebuggingWarning + 9,
    cannot_dispose_default_transform = gxFirstSystemDebuggingWarning + 10,
    cannot_dispose_default_colorProfile = gxFirstSystemDebuggingWarning + 11,
    cannot_dispose_default_colorSet = gxFirstSystemDebuggingWarning + 12,
    shape_direct_attribute_not_set = gxFirstSystemDebuggingWarning + 13, /* couldn't find what you were looking for */
    point_does_not_intersect_port = gxFirstSystemDebuggingWarning + 14,
    cannot_dispose_non_font     = gxFirstSystemDebuggingWarning + 15,
    face_override_style_font_must_match_style = gxFirstSystemDebuggingWarning + 16,
    union_of_area_and_length_returns_area_only = gxFirstSystemDebuggingWarning + 17,
    insufficient_coordinate_space_for_new_device = gxFirstSystemDebuggingWarning + 18, /* other */
    shape_passed_has_no_bounds  = gxFirstSystemDebuggingWarning + 19,
    tags_of_type_flst_removed   = gxFirstSystemDebuggingWarning + 20,
    translator_not_installed_on_this_grafport = gxFirstSystemDebuggingWarning + 21
};

enum {
    parameters_have_no_effect   = gxFirstSystemNotice,
    attributes_already_set      = gxFirstSystemNotice + 1,
    caps_already_set            = gxFirstSystemNotice + 2,
    clip_already_set            = gxFirstSystemNotice + 3,
    color_already_set           = gxFirstSystemNotice + 4,
    curve_error_already_set     = gxFirstSystemNotice + 5,
    dash_already_set            = gxFirstSystemNotice + 6,
    default_colorProfile_already_set = gxFirstSystemNotice + 7,
    default_ink_already_set     = gxFirstSystemNotice + 8,
    default_transform_already_set = gxFirstSystemNotice + 9,
    default_shape_already_set   = gxFirstSystemNotice + 10,
    default_style_already_set   = gxFirstSystemNotice + 11,
    dither_already_set          = gxFirstSystemNotice + 12,
    encoding_already_set        = gxFirstSystemNotice + 13,
    face_already_set            = gxFirstSystemNotice + 14,
    fill_already_set            = gxFirstSystemNotice + 15,
    font_already_set            = gxFirstSystemNotice + 16,
    font_variations_already_set = gxFirstSystemNotice + 17,
    glyph_positions_are_already_set = gxFirstSystemNotice + 18,
    glyph_tangents_are_already_set = gxFirstSystemNotice + 19,
    halftone_already_set        = gxFirstSystemNotice + 20,
    hit_test_already_set        = gxFirstSystemNotice + 21,
    ink_already_set             = gxFirstSystemNotice + 22,
    join_already_set            = gxFirstSystemNotice + 23,
    justification_already_set   = gxFirstSystemNotice + 24,
    mapping_already_set         = gxFirstSystemNotice + 25,
    pattern_already_set         = gxFirstSystemNotice + 26,
    pen_already_set             = gxFirstSystemNotice + 27,
    style_already_set           = gxFirstSystemNotice + 28,
    tag_already_set             = gxFirstSystemNotice + 29,
    text_attributes_already_set = gxFirstSystemNotice + 30,
    text_size_already_set       = gxFirstSystemNotice + 31,
    transfer_already_set        = gxFirstSystemNotice + 32,
    translator_already_installed_on_this_grafport = gxFirstSystemNotice + 33,
    transform_already_set       = gxFirstSystemNotice + 34,
    type_already_set            = gxFirstSystemNotice + 35,
    validation_level_already_set = gxFirstSystemNotice + 36,
    viewPorts_already_set       = gxFirstSystemNotice + 37,
    viewPort_already_in_viewGroup = gxFirstSystemNotice + 38,
    viewDevice_already_in_viewGroup = gxFirstSystemNotice + 39,
    geometry_unaffected         = gxFirstSystemNotice + 40,
    mapping_unaffected          = gxFirstSystemNotice + 41,
    tags_in_shape_ignored       = gxFirstSystemNotice + 42,
    shape_already_in_primitive_form = gxFirstSystemNotice + 43,
    shape_already_in_simple_form = gxFirstSystemNotice + 44,
    shape_already_broken        = gxFirstSystemNotice + 45,
    shape_already_joined        = gxFirstSystemNotice + 46,
    cache_already_cleared       = gxFirstSystemNotice + 47,
    shape_not_disposed          = gxFirstSystemNotice + 48,
    style_not_disposed          = gxFirstSystemNotice + 49,
    ink_not_disposed            = gxFirstSystemNotice + 50,
    transform_not_disposed      = gxFirstSystemNotice + 51,
    colorSet_not_disposed       = gxFirstSystemNotice + 52,
    colorProfile_not_disposed   = gxFirstSystemNotice + 53,
    font_not_disposed           = gxFirstSystemNotice + 54,
    glyph_tangents_have_no_effect = gxFirstSystemNotice + 55,
    glyph_positions_determined_by_advance = gxFirstSystemNotice + 56,
    transform_viewPorts_already_set = gxFirstSystemNotice + 57,
    directShape_attribute_set_as_side_effect = gxFirstSystemNotice + 58,
    lockShape_called_as_side_effect = gxFirstSystemNotice + 59,
    lockTag_called_as_side_effect = gxFirstSystemNotice + 60,
    shapes_unlocked_as_side_effect = gxFirstSystemNotice + 61,
    shape_not_locked            = gxFirstSystemNotice + 62,
    tag_not_locked              = gxFirstSystemNotice + 63,
    profile_not_locked          = tag_not_locked,
    lockProfile_called_as_side_effect = lockTag_called_as_side_effect,
    disposed_dead_caches        = gxFirstSystemNotice + 64,
    disposed_live_caches        = gxFirstSystemNotice + 65,
    low_on_memory               = gxFirstSystemNotice + 66,
    very_low_on_memory          = gxFirstSystemNotice + 67,
    transform_references_disposed_viewPort = gxFirstSystemNotice + 68
};


typedef long                            gxGraphicsError;
typedef long                            gxGraphicsWarning;
typedef long                            gxGraphicsNotice;
typedef CALLBACK_API_C( void , gxUserErrorProcPtr )(gxGraphicsError status, long refcon);
typedef CALLBACK_API_C( void , gxUserWarningProcPtr )(gxGraphicsWarning status, long refcon);
typedef CALLBACK_API_C( void , gxUserNoticeProcPtr )(gxGraphicsNotice status, long refcon);
typedef STACK_UPP_TYPE(gxUserErrorProcPtr)                      gxUserErrorUPP;
typedef STACK_UPP_TYPE(gxUserWarningProcPtr)                    gxUserWarningUPP;
typedef STACK_UPP_TYPE(gxUserNoticeProcPtr)                     gxUserNoticeUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(gxUserErrorUPP)
    NewgxUserErrorUPP              (gxUserErrorProcPtr      userRoutine);

    EXTERN_API(gxUserWarningUPP)
    NewgxUserWarningUPP            (gxUserWarningProcPtr    userRoutine);

    EXTERN_API(gxUserNoticeUPP)
    NewgxUserNoticeUPP             (gxUserNoticeProcPtr     userRoutine);

    EXTERN_API(void)
    DisposegxUserErrorUPP          (gxUserErrorUPP          userUPP);

    EXTERN_API(void)
    DisposegxUserWarningUPP        (gxUserWarningUPP        userUPP);

    EXTERN_API(void)
    DisposegxUserNoticeUPP         (gxUserNoticeUPP         userUPP);

    EXTERN_API(void)
    InvokegxUserErrorUPP           (gxGraphicsError         status,
                                    long                    refcon,
                                    gxUserErrorUPP          userUPP);

    EXTERN_API(void)
    InvokegxUserWarningUPP         (gxGraphicsWarning       status,
                                    long                    refcon,
                                    gxUserWarningUPP        userUPP);

    EXTERN_API(void)
    InvokegxUserNoticeUPP          (gxGraphicsNotice        status,
                                    long                    refcon,
                                    gxUserNoticeUPP         userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppgxUserErrorProcInfo = 0x000003C1 };                   /* no_return_value Func(4_bytes, 4_bytes) */
    enum { uppgxUserWarningProcInfo = 0x000003C1 };                 /* no_return_value Func(4_bytes, 4_bytes) */
    enum { uppgxUserNoticeProcInfo = 0x000003C1 };                  /* no_return_value Func(4_bytes, 4_bytes) */
    #define NewgxUserErrorUPP(userRoutine)                          (gxUserErrorUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppgxUserErrorProcInfo, GetCurrentArchitecture())
    #define NewgxUserWarningUPP(userRoutine)                        (gxUserWarningUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppgxUserWarningProcInfo, GetCurrentArchitecture())
    #define NewgxUserNoticeUPP(userRoutine)                         (gxUserNoticeUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppgxUserNoticeProcInfo, GetCurrentArchitecture())
    #define DisposegxUserErrorUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define DisposegxUserWarningUPP(userUPP)                        DisposeRoutineDescriptor(userUPP)
    #define DisposegxUserNoticeUPP(userUPP)                         DisposeRoutineDescriptor(userUPP)
    #define InvokegxUserErrorUPP(status, refcon, userUPP)           CALL_TWO_PARAMETER_UPP((userUPP), uppgxUserErrorProcInfo, (status), (refcon))
    #define InvokegxUserWarningUPP(status, refcon, userUPP)         CALL_TWO_PARAMETER_UPP((userUPP), uppgxUserWarningProcInfo, (status), (refcon))
    #define InvokegxUserNoticeUPP(status, refcon, userUPP)          CALL_TWO_PARAMETER_UPP((userUPP), uppgxUserNoticeProcInfo, (status), (refcon))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewgxUserErrorProc(userRoutine)                         NewgxUserErrorUPP(userRoutine)
#define NewgxUserWarningProc(userRoutine)                       NewgxUserWarningUPP(userRoutine)
#define NewgxUserNoticeProc(userRoutine)                        NewgxUserNoticeUPP(userRoutine)
#define CallgxUserErrorProc(userRoutine, status, refcon)        InvokegxUserErrorUPP(status, refcon, userRoutine)
#define CallgxUserWarningProc(userRoutine, status, refcon)      InvokegxUserWarningUPP(status, refcon, userRoutine)
#define CallgxUserNoticeProc(userRoutine, status, refcon)       InvokegxUserNoticeUPP(status, refcon, userRoutine)
typedef gxUserErrorProcPtr              gxUserErrorFunction;
typedef gxUserWarningProcPtr            gxUserWarningFunction;
typedef gxUserNoticeProcPtr             gxUserNoticeFunction;

enum {
    common_colors_not_initialized = gxFirstLibraryError,
    no_open_picture             = gxFirstLibraryError + 1,
    picture_already_open        = gxFirstLibraryError + 2,
    no_open_poly                = gxFirstLibraryError + 3,
    poly_already_open           = gxFirstLibraryError + 4,
    no_open_region              = gxFirstLibraryError + 5,
    region_already_open         = gxFirstLibraryError + 6,
    no_active_picture           = gxFirstLibraryError + 7
};

enum {
    no_picture_drawn            = gxFirstLibraryWarning,
    polygons_have_different_size_contours = gxFirstLibraryWarning + 1,
    graphic_type_cannot_be_specifed_by_four_values = gxFirstLibraryWarning + 2,
    graphic_type_cannot_be_specifed_by_six_values = gxFirstLibraryWarning + 3,
    point_expected              = gxFirstLibraryWarning + 4,
    line_or_rectangle_expected  = gxFirstLibraryWarning + 5,
    curve_expected              = gxFirstLibraryWarning + 6,
    graphic_type_does_not_contain_control_bits = gxFirstLibraryWarning + 7,
    request_exceeds_available_data = gxFirstLibraryWarning + 8,
    extra_data_unread           = gxFirstLibraryWarning + 9,
    no_variable_length_user_data_saved = gxFirstLibraryWarning + 10
};

enum {
    zero_length_string_passed   = gxFirstLibraryNotice
};

enum {
                                                                /* These levels tell how to validate routines.  Choose one. */
    gxNoValidation              = 0x00,                         /* no validation */
    gxPublicValidation          = 0x01,                         /* check parameters to public routines */
    gxInternalValidation        = 0x02,                         /* check parameters to internal routines */
                                                                /* These levels tell how to validate types.  Choose one. */
    gxTypeValidation            = 0x00,                         /* check types of objects */
    gxStructureValidation       = 0x10,                         /* check fields of private structures */
    gxAllObjectValidation       = 0x20,                         /* check every object over every call */
                                                                /* These levels tell how to validate memory manager blocks.  Choose any combination. */
    gxNoMemoryManagerValidation = 0x0000,
    gxApBlockValidation         = 0x0100,                       /* check the relevant block structures after each memory mgr. call */
    gxFontBlockValidation       = 0x0200,                       /* check the system gxHeap as well */
    gxApHeapValidation          = 0x0400,                       /* check the memory manager's gxHeap after every mem. call */
    gxFontHeapValidation        = 0x0800,                       /* check the system gxHeap as well */
    gxCheckApHeapValidation     = 0x1000,                       /* check the memory manager's gxHeap if checking routine parameters */
    gxCheckFontHeapValidation   = 0x2000                        /* check the system gxHeap as well */
};

typedef long                            gxValidationLevel;
enum {
    no_draw_error               = 0,                            /* gxShape type errors */
    shape_emptyType             = 1,
    shape_inverse_fullType      = 2,
    rectangle_zero_width        = 3,
    rectangle_zero_height       = 4,
    polygon_empty               = 5,
    path_empty                  = 6,
    bitmap_zero_width           = 7,
    bitmap_zero_height          = 8,
    text_empty                  = 9,
    glyph_empty                 = 10,
    layout_empty                = 11,
    picture_empty               = 12,                           /* general gxShape errors */
    shape_no_fill               = 13,
    shape_no_enclosed_area      = 14,
    shape_no_enclosed_pixels    = 15,
    shape_very_small            = 16,
    shape_very_large            = 17,
    shape_contours_cancel       = 18,                           /* gxStyle errors */
    pen_too_small               = 19,
    text_size_too_small         = 20,
    dash_empty                  = 21,
    start_cap_empty             = 22,
    pattern_empty               = 23,
    textFace_empty              = 24,
    shape_primitive_empty       = 25,
    shape_primitive_very_small  = 26,                           /* gxInk errors */
    transfer_equals_noMode      = 27,
    transfer_matrix_ignores_source = 28,
    transfer_matrix_ignores_device = 29,
    transfer_source_reject      = 30,
    transfer_mode_ineffective   = 31,
    colorSet_no_entries         = 32,
    bitmap_colorSet_one_entry   = 33,                           /* gxTransform errors */
    transform_scale_too_small   = 34,
    transform_map_too_large     = 35,
    transform_move_too_large    = 36,
    transform_scale_too_large   = 37,
    transform_rotate_too_large  = 38,
    transform_perspective_too_large = 39,
    transform_skew_too_large    = 40,
    transform_clip_no_intersection = 41,
    transform_clip_empty        = 42,
    transform_no_viewPorts      = 43,                           /* gxViewPort errors */
    viewPort_disposed           = 44,
    viewPort_clip_empty         = 45,
    viewPort_clip_no_intersection = 46,
    viewPort_scale_too_small    = 47,
    viewPort_map_too_large      = 48,
    viewPort_move_too_large     = 49,
    viewPort_scale_too_large    = 50,
    viewPort_rotate_too_large   = 51,
    viewPort_perspective_too_large = 52,
    viewPort_skew_too_large     = 53,
    viewPort_viewGroup_offscreen = 54,                          /* gxViewDevice errors */
    viewDevice_clip_no_intersection = 55,
    viewDevice_scale_too_small  = 56,
    viewDevice_map_too_large    = 57,
    viewDevice_move_too_large   = 58,
    viewDevice_scale_too_large  = 59,
    viewDevice_rotate_too_large = 60,
    viewDevice_perspective_too_large = 61,
    viewDevice_skew_too_large   = 62
};

typedef long                            gxDrawError;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( gxDrawError )
GXGetShapeDrawError             (gxShape                source)                             THREEWORDINLINE(0x303C, 0x01EE, 0xA832);

EXTERN_API_C( void )
GXValidateAll                   (void)                                                      THREEWORDINLINE(0x303C, 0x01EF, 0xA832);

EXTERN_API_C( void )
GXValidateColorSet              (gxColorSet             target)                             THREEWORDINLINE(0x303C, 0x01F0, 0xA832);

EXTERN_API_C( void )
GXValidateColorProfile          (gxColorProfile         target)                             THREEWORDINLINE(0x303C, 0x01F1, 0xA832);

EXTERN_API_C( void )
GXValidateGraphicsClient        (gxGraphicsClient       target)                             THREEWORDINLINE(0x303C, 0x01F2, 0xA832);

EXTERN_API_C( void )
GXValidateInk                   (gxInk                  target)                             THREEWORDINLINE(0x303C, 0x01F3, 0xA832);

EXTERN_API_C( void )
GXValidateShape                 (gxShape                target)                             THREEWORDINLINE(0x303C, 0x01F4, 0xA832);

EXTERN_API_C( void )
GXValidateStyle                 (gxStyle                target)                             THREEWORDINLINE(0x303C, 0x01F5, 0xA832);

EXTERN_API_C( void )
GXValidateTag                   (gxTag                  target)                             THREEWORDINLINE(0x303C, 0x01F6, 0xA832);

EXTERN_API_C( void )
GXValidateTransform             (gxTransform            target)                             THREEWORDINLINE(0x303C, 0x01F7, 0xA832);

EXTERN_API_C( void )
GXValidateViewDevice            (gxViewDevice           target)                             THREEWORDINLINE(0x303C, 0x01F8, 0xA832);

EXTERN_API_C( void )
GXValidateViewPort              (gxViewPort             target)                             THREEWORDINLINE(0x303C, 0x01F9, 0xA832);

EXTERN_API_C( void )
GXValidateViewGroup             (gxViewGroup            target)                             THREEWORDINLINE(0x303C, 0x01FA, 0xA832);

EXTERN_API_C( gxValidationLevel )
GXGetValidation                 (void)                                                      THREEWORDINLINE(0x303C, 0x01FB, 0xA832);

EXTERN_API_C( void )
GXSetValidation                 (gxValidationLevel      level)                              THREEWORDINLINE(0x303C, 0x01FC, 0xA832);

EXTERN_API_C( long )
GXGetValidationError            (char *                 procedureName,
                                 void **                argument,
                                 long *                 argumentNumber)                     THREEWORDINLINE(0x303C, 0x01FD, 0xA832);


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

#endif /* __GXERRORS__ */

