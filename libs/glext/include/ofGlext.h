	/*
	Copyright:  (c) 1999-2008 Apple Inc. All rights reserved.
*/

#ifndef __glext_h_
#define __glext_h_

#ifdef __cplusplus
extern "C" {
#endif
	
	
	//------------------------------------------------
	// ofadditions
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS			GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT
	//------------------------------------------------
	

/*
** License Applicability. Except to the extent portions of this file are
** made subject to an alternative license as permitted in the SGI Free
** Software License B, Version 1.1 (the "License"), the contents of this
** file are subject only to the provisions of the License. You may not use
** this file except in compliance with the License. You may obtain a copy
** of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
** Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
**
** http://oss.sgi.com/projects/FreeB
**
** Note that, as provided in the License, the Software is distributed on an
** "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
** DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
** CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
** PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
**
** Original Code. The Original Code is: OpenGL Sample Implementation,
** Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
** Inc. The Original Code is Copyright (c) 1991-2000 Silicon Graphics, Inc.
** Copyright in any portions created by third parties is as indicated
** elsewhere herein. All Rights Reserved.
**
** Additional Notice Provisions: This software was created using the
** OpenGL(R) version 1.2.1 Sample Implementation published by SGI, but has
** not been independently verified as being compliant with the OpenGL(R)
** version 1.2.1 Specification.
*/

/* switches to providing function pointers */
/* #define GL_GLEXT_FUNCTION_POINTERS 1 */

#define GL_ARB_imaging                      1
#define GL_ARB_transpose_matrix             1
#define GL_ARB_multitexture                 1
#define GL_ARB_texture_env_add              1
#define GL_ARB_texture_env_combine          1
#define GL_ARB_texture_env_dot3             1
#define GL_ARB_texture_env_crossbar         1
#define GL_ARB_texture_cube_map             1
#define GL_ARB_texture_compression          1
#define GL_ARB_multisample                  1
#define GL_ARB_texture_border_clamp         1
#define GL_ARB_point_parameters             1
#define GL_ARB_vertex_program               1
#define GL_ARB_fragment_program             1
#define GL_ARB_fragment_program_shadow      1
#define GL_ARB_texture_mirrored_repeat      1
#define GL_ARB_depth_texture                1
#define GL_ARB_shadow                       1
#define GL_ARB_shadow_ambient               1
#define GL_ARB_vertex_blend                 1
#define GL_ARB_window_pos                   1
#define GL_ARB_occlusion_query              1
#define GL_ARB_shader_objects               1
#define GL_ARB_vertex_shader                1
#define GL_ARB_fragment_shader              1
#define GL_ARB_shading_language_100         1
#define GL_ARB_vertex_buffer_object         1
#define GL_ARB_point_sprite                 1
#define GL_ARB_texture_non_power_of_two     1
#define GL_ARB_texture_rectangle            1
#define GL_ARB_draw_buffers                 1
#define GL_ARB_pixel_buffer_object          1
#define GL_ARB_shader_texture_lod           1
#define GL_ARB_texture_float                1
#define GL_ARB_half_float_pixel             1
#define GL_ARB_color_buffer_float           1
#define GL_ARB_half_float_vertex            1
#define GL_ARB_texture_compression_rgtc     1
#define GL_ARB_texture_rg                   1
#define GL_ARB_uniform_buffer_object        1
#define GL_ARB_framebuffer_object           1
#define GL_ARB_instanced_arrays             1
#define GL_ARB_depth_buffer_float           1
#define GL_EXT_draw_instanced               0
#define GL_EXT_clip_volume_hint             1
#define GL_EXT_rescale_normal               1
#define GL_EXT_blend_color                  1
#define GL_EXT_blend_minmax                 1
#define GL_EXT_blend_subtract               1
#define GL_EXT_compiled_vertex_array        1
#define GL_EXT_texture_array                1
#define GL_EXT_texture_lod_bias             1
#define GL_EXT_texture_env_add              1
#define GL_EXT_abgr                         1
#define GL_EXT_bgra                         1
#define GL_EXT_texture_filter_anisotropic   1
#define GL_EXT_secondary_color              1
#define GL_EXT_separate_specular_color      1
#define GL_EXT_texture_compression_s3tc     1
#define GL_EXT_texture_rectangle            1
#define GL_EXT_fog_coord                    1
#define GL_EXT_draw_range_elements          1
#define GL_EXT_stencil_wrap                 1
#define GL_EXT_blend_func_separate          1
#define GL_EXT_multi_draw_arrays            1
#define GL_EXT_shadow_funcs                 1
#define GL_EXT_stencil_two_side             1
#define GL_EXT_depth_bounds_test            1
#define GL_EXT_blend_equation_separate      1
#define GL_EXT_texture_mirror_clamp         1
#define GL_EXT_texture_compression_dxt1     1
#define GL_EXT_texture_sRGB                 1
#define GL_EXT_framebuffer_object           1
#define GL_EXT_framebuffer_blit             1
#define GL_EXT_framebuffer_multisample      1
#define GL_EXT_packed_depth_stencil         1
#define GL_EXT_gpu_program_parameters       1
#define GL_EXT_geometry_shader4             1
#define GL_EXT_transform_feedback           1
#define GL_EXT_bindable_uniform             1
#define GL_EXT_texture_integer              1
#define GL_EXT_gpu_shader4                  1
#define GL_EXT_draw_buffers2                1
#define GL_EXT_framebuffer_sRGB             1
#define GL_EXT_packed_float                 1
#define GL_EXT_texture_shared_exponent      1
#define GL_EXT_provoking_vertex             1
#define GL_EXT_vertex_array_bgra            1
#define GL_APPLE_flush_buffer_range         1
#define GL_APPLE_specular_vector            1
#define GL_APPLE_transform_hint             1
#define GL_APPLE_packed_pixels              1
#define GL_APPLE_client_storage             1
#define GL_APPLE_ycbcr_422                  1
#define GL_APPLE_texture_range              1
#define GL_APPLE_fence                      1
#define GL_APPLE_vertex_array_range         1
#define GL_APPLE_vertex_array_object        1
#define GL_APPLE_element_array              1
#define GL_APPLE_vertex_program_evaluators  1
#define GL_APPLE_float_pixels               1
#define GL_APPLE_flush_render               1
#define GL_APPLE_pixel_buffer               1
#define GL_APPLE_aux_depth_stencil          1
#define GL_APPLE_row_bytes                  1
#define GL_APPLE_object_purgeable           1
#define GL_APPLE_rgb_422                    1
#define GL_ATI_point_cull_mode              1
#define GL_ATI_texture_mirror_once          1
#define GL_ATI_pn_triangles                 1
#define GL_ATI_blend_equation_separate      1
#define GL_ATI_blend_weighted_minmax        1
#define GL_ATI_texture_env_combine3         1
#define GL_ATI_separate_stencil             1
#define GL_ATI_texture_compression_3dc      1
#define GL_ATI_texture_float                1
#define GL_ATIX_pn_triangles                1
#define GL_IBM_rasterpos_clip               1
#define GL_NV_point_sprite                  1
#define GL_NV_blend_square                  1
#define GL_NV_fog_distance                  1
#define GL_NV_multisample_filter_hint       1
#define GL_NV_texgen_reflection             1
#define GL_NV_depth_clamp                   1
#define GL_NV_light_max_exponent            1
#define GL_NV_fragment_program_option       1
#define GL_NV_fragment_program2             1
#define GL_NV_vertex_program2_option        1
#define GL_NV_vertex_program3               1
#define GL_NV_conditional_render            1
#define GL_SGI_color_matrix                 1
#define GL_SGIS_texture_edge_clamp          1
#define GL_SGIS_generate_mipmap             1
#define GL_SGIS_texture_lod                 1

#ifdef GL_GLEXT_WUNDEF_SUPPORT
#define GL_EXT_422_pixels                   0
#define GL_EXT_cmyka                        0
#define GL_EXT_convolution                  0
#define GL_EXT_color_subtable               0
#define GL_EXT_coordinate_frame             0
#define GL_EXT_copy_texture                 0
#define GL_EXT_cull_vertex                  0
#define GL_EXT_fragment_shader              0
#define GL_EXT_histogram                    0
#define GL_EXT_index_array_formats          0
#define GL_EXT_index_func                   0
#define GL_EXT_index_material               0
#define GL_EXT_light_texture                0
#define GL_EXT_multisample                  0
#define GL_EXT_packed_pixels                0
#define GL_EXT_pixel_transform              0
#define GL_EXT_polygon_offset               0
#define GL_EXT_shared_texture_palette       0
#define GL_EXT_subtexture                   0
#define GL_EXT_texture                      0
#define GL_EXT_texture3D                    0
#define GL_EXT_texture_cube_map             0
#define GL_EXT_texture_env_combine          0
#define GL_EXT_texture_object               0
#define GL_EXT_texture_perturb_normal       0
#define GL_EXT_timer_query                  0
#define GL_EXT_vertex_array                 0
#define GL_EXT_vertex_shader                0
#define GL_EXT_vertex_weighting             0
#define GL_EXT_paletted_texture             0
#define GL_APPLE_vertex_point_size          0
#define GL_ATI_text_fragment_shader         0
#define GL_ATI_array_rev_comps_in_4_bytes   0
#define GL_HP_convolution_border_modes      0
#define GL_HP_image_transform               0
#define GL_HP_occlusion_test                0
#define GL_HP_texture_lighting              0
#define GL_IBM_cull_vertex                  0
#define GL_IBM_multimode_draw_arrays        0
#define GL_IBM_vertex_array_lists           0
#define GL_INGR_color_clamp                 0
#define GL_INGR_interlace_read              0
#define GL_INTEL_parallel_arrays            0
#define GL_MESA_resize_buffers              0
#define GL_NV_register_combiners            0
#define GL_NV_register_combiners2           0
#define GL_NV_texture_shader                0
#define GL_NV_texture_shader2               0
#define GL_NV_texture_shader3               0
#define GL_NV_texgen_emboss                 0
#define GL_NV_texture_env_combine4          0
#define GL_NV_vertex_array_range            0
#define GL_NV_vertex_array_range2           0
#define GL_NV_vertex_program                0
#define GL_PGI_misc_hints                   0
#define GL_PGI_vertex_hints                 0
#define GL_REND_screen_coordinates          0
#define GL_SGI_color_table                  0
#define GL_SGI_depth_pass_instrument        0
#define GL_SGI_texture_color_table          0
#define GL_SGIS_detail_texture              0
#define GL_SGIS_fog_function                0
#define GL_SGIS_multisample                 0
#define GL_SGIS_pixel_texture               0
#define GL_SGIS_point_line_texgen           0
#define GL_SGIS_point_parameters            0
#define GL_SGIS_sharpen_texture             0
#define GL_SGIS_texture4D                   0
#define GL_SGIS_texture_border_clamp        0
#define GL_SGIS_texture_color_mask          0
#define GL_SGIS_texture_filter4             0
#define GL_SGIS_texture_select              0
#define GL_SGIX_async                       0
#define GL_SGIX_async_histogram             0
#define GL_SGIX_async_pixel                 0
#define GL_SGIX_blend_alpha_minmax          0
#define GL_SGIX_calligraphic_fragment       0
#define GL_SGIX_clipmap                     0
#define GL_SGIX_convolution_accuracy        0
#define GL_SGIX_depth_texture               0
#define GL_SGIX_flush_raster                0
#define GL_SGIX_fog_offset                  0
#define GL_SGIX_fog_scale                   0
#define GL_SGIX_fragment_lighting           0
#define GL_SGIX_framezoom                   0
#define GL_SGIX_igloo_interface             0
#define GL_SGIX_instruments                 0
#define GL_SGIX_interlace                   0
#define GL_SGIX_ir_instrument1              0
#define GL_SGIX_list_priority               0
#define GL_SGIX_pixel_texture               0
#define GL_SGIX_pixel_tiles                 0
#define GL_SGIX_polynomial_ffd              0
#define GL_SGIX_reference_plane             0
#define GL_SGIX_resample                    0
#define GL_SGIX_shadow                      0
#define GL_SGIX_sprite                      0
#define GL_SGIX_subsample                   0
#define GL_SGIX_tag_sample_buffer           0
#define GL_SGIX_texture_add_env             0
#define GL_SGIX_texture_lod_bias            0
#define GL_SGIX_texture_multi_buffer        0
#define GL_SGIX_texture_scale_bias          0
#define GL_SGIX_vertex_preclip              0
#define GL_SGIX_ycrcb                       0
#define GL_SGIX_ycrcba                      0
#define GL_SUN_convolution_border_modes     0
#define GL_SUN_global_alpha                 0
#define GL_SUN_triangle_list                0
#define GL_SUN_vertex                       0
#define GL_SUNX_constant_data               0
#define GL_WIN_phong_shading                0
#define GL_WIN_specular_fog                 0
#define GL_3DFX_multisample                 0
#define GL_3DFX_tbuffer                     0
#define GL_3DFX_texture_compression_FXT1    0
#endif

/*************************************************************/
#if GL_ARB_shader_objects
typedef char GLcharARB;
typedef void *GLhandleARB;
#endif

#if GL_ARB_vertex_buffer_object
typedef long GLintptrARB;
typedef long GLsizeiptrARB;
#endif

#if GL_ARB_half_float_pixel
typedef unsigned short GLhalfARB;
#endif

#if GL_ARB_half_float_vertex
typedef unsigned short GLhalf;
#endif

/*************************************************************/
#define GL_GLEXT_VERSION 7

#if GL_ARB_multitexture
#define GL_TEXTURE0_ARB                   0x84C0
#define GL_TEXTURE1_ARB                   0x84C1
#define GL_TEXTURE2_ARB                   0x84C2
#define GL_TEXTURE3_ARB                   0x84C3
#define GL_TEXTURE4_ARB                   0x84C4
#define GL_TEXTURE5_ARB                   0x84C5
#define GL_TEXTURE6_ARB                   0x84C6
#define GL_TEXTURE7_ARB                   0x84C7
#define GL_TEXTURE8_ARB                   0x84C8
#define GL_TEXTURE9_ARB                   0x84C9
#define GL_TEXTURE10_ARB                  0x84CA
#define GL_TEXTURE11_ARB                  0x84CB
#define GL_TEXTURE12_ARB                  0x84CC
#define GL_TEXTURE13_ARB                  0x84CD
#define GL_TEXTURE14_ARB                  0x84CE
#define GL_TEXTURE15_ARB                  0x84CF
#define GL_TEXTURE16_ARB                  0x84D0
#define GL_TEXTURE17_ARB                  0x84D1
#define GL_TEXTURE18_ARB                  0x84D2
#define GL_TEXTURE19_ARB                  0x84D3
#define GL_TEXTURE20_ARB                  0x84D4
#define GL_TEXTURE21_ARB                  0x84D5
#define GL_TEXTURE22_ARB                  0x84D6
#define GL_TEXTURE23_ARB                  0x84D7
#define GL_TEXTURE24_ARB                  0x84D8
#define GL_TEXTURE25_ARB                  0x84D9
#define GL_TEXTURE26_ARB                  0x84DA
#define GL_TEXTURE27_ARB                  0x84DB
#define GL_TEXTURE28_ARB                  0x84DC
#define GL_TEXTURE29_ARB                  0x84DD
#define GL_TEXTURE30_ARB                  0x84DE
#define GL_TEXTURE31_ARB                  0x84DF
#define GL_ACTIVE_TEXTURE_ARB             0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB      0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB          0x84E2
#endif

#if GL_ARB_transpose_matrix
#define GL_TRANSPOSE_MODELVIEW_MATRIX_ARB 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX_ARB 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX_ARB   0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX_ARB     0x84E6
#endif

#if GL_ARB_multisample
#define GL_MULTISAMPLE_ARB                0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB   0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_ARB        0x809F
#define GL_SAMPLE_COVERAGE_ARB            0x80A0
#define GL_SAMPLE_BUFFERS_ARB             0x80A8
#define GL_SAMPLES_ARB                    0x80A9
#define GL_SAMPLE_COVERAGE_VALUE_ARB      0x80AA
#define GL_SAMPLE_COVERAGE_INVERT_ARB     0x80AB
#define GL_MULTISAMPLE_BIT_ARB            0x20000000
#endif

#if GL_ARB_texture_cube_map
#define GL_NORMAL_MAP_ARB                 0x8511
#define GL_REFLECTION_MAP_ARB             0x8512
#define GL_TEXTURE_CUBE_MAP_ARB           0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB   0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB     0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB  0x851C
#endif

#if GL_ARB_texture_compression
#define GL_COMPRESSED_ALPHA_ARB           0x84E9
#define GL_COMPRESSED_LUMINANCE_ARB       0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB 0x84EB
#define GL_COMPRESSED_INTENSITY_ARB       0x84EC
#define GL_COMPRESSED_RGB_ARB             0x84ED
#define GL_COMPRESSED_RGBA_ARB            0x84EE
#define GL_TEXTURE_COMPRESSION_HINT_ARB   0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB 0x86A0
#define GL_TEXTURE_COMPRESSED_ARB         0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A3
#endif

#if GL_ARB_vertex_blend
#define GL_MAX_VERTEX_UNITS_ARB           0x86A4
#define GL_ACTIVE_VERTEX_UNITS_ARB        0x86A5
#define GL_WEIGHT_SUM_UNITY_ARB           0x86A6
#define GL_VERTEX_BLEND_ARB               0x86A7
#define GL_CURRENT_WEIGHT_ARB             0x86A8
#define GL_WEIGHT_ARRAY_TYPE_ARB          0x86A9
#define GL_WEIGHT_ARRAY_STRIDE_ARB        0x86AA
#define GL_WEIGHT_ARRAY_SIZE_ARB          0x86AB
#define GL_WEIGHT_ARRAY_POINTER_ARB       0x86AC
#define GL_WEIGHT_ARRAY_ARB               0x86AD
#define GL_MODELVIEW0_ARB                 0x1700
#define GL_MODELVIEW1_ARB                 0x850A
#define GL_MODELVIEW2_ARB                 0x8722
#define GL_MODELVIEW3_ARB                 0x8723
#define GL_MODELVIEW4_ARB                 0x8724
#define GL_MODELVIEW5_ARB                 0x8725
#define GL_MODELVIEW6_ARB                 0x8726
#define GL_MODELVIEW7_ARB                 0x8727
#define GL_MODELVIEW8_ARB                 0x8728
#define GL_MODELVIEW9_ARB                 0x8729
#define GL_MODELVIEW10_ARB                0x872A
#define GL_MODELVIEW11_ARB                0x872B
#define GL_MODELVIEW12_ARB                0x872C
#define GL_MODELVIEW13_ARB                0x872D
#define GL_MODELVIEW14_ARB                0x872E
#define GL_MODELVIEW15_ARB                0x872F
#define GL_MODELVIEW16_ARB                0x8730
#define GL_MODELVIEW17_ARB                0x8731
#define GL_MODELVIEW18_ARB                0x8732
#define GL_MODELVIEW19_ARB                0x8733
#define GL_MODELVIEW20_ARB                0x8734
#define GL_MODELVIEW21_ARB                0x8735
#define GL_MODELVIEW22_ARB                0x8736
#define GL_MODELVIEW23_ARB                0x8737
#define GL_MODELVIEW24_ARB                0x8738
#define GL_MODELVIEW25_ARB                0x8739
#define GL_MODELVIEW26_ARB                0x873A
#define GL_MODELVIEW27_ARB                0x873B
#define GL_MODELVIEW28_ARB                0x873C
#define GL_MODELVIEW29_ARB                0x873D
#define GL_MODELVIEW30_ARB                0x873E
#define GL_MODELVIEW31_ARB                0x873F
#endif

#if GL_ARB_occlusion_query
#define GL_SAMPLES_PASSED_ARB             0x8914
#define GL_QUERY_COUNTER_BITS_ARB         0x8864
#define GL_CURRENT_QUERY_ARB              0x8865
#define GL_QUERY_RESULT_ARB               0x8866
#define GL_QUERY_RESULT_AVAILABLE_ARB     0x8867
#endif

#if GL_ARB_texture_border_clamp
#define GL_CLAMP_TO_BORDER_ARB           0x812D
#endif

#if GL_ARB_depth_texture
#define GL_DEPTH_COMPONENT16_ARB          0x81A5
#define GL_DEPTH_COMPONENT24_ARB          0x81A6
#define GL_DEPTH_COMPONENT32_ARB          0x81A7
#define GL_TEXTURE_DEPTH_SIZE_ARB         0x884A
#define GL_DEPTH_TEXTURE_MODE_ARB         0x884B
#endif

#if GL_ARB_shadow
#define GL_TEXTURE_COMPARE_MODE_ARB       0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB       0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB       0x884E
#endif

#if GL_ARB_shadow_ambient
#define GL_TEXTURE_COMPARE_FAIL_VALUE_ARB 0x80BF
#endif

#if GL_ARB_texture_env_combine
#define GL_COMBINE_ARB                    0x8570
#define GL_COMBINE_RGB_ARB                0x8571
#define GL_COMBINE_ALPHA_ARB              0x8572
#define GL_RGB_SCALE_ARB                  0x8573
#define GL_ADD_SIGNED_ARB                 0x8574
#define GL_INTERPOLATE_ARB                0x8575
#define GL_CONSTANT_ARB                   0x8576
#define GL_PRIMARY_COLOR_ARB              0x8577
#define GL_PREVIOUS_ARB                   0x8578
#define GL_SUBTRACT_ARB                   0x84E7
#define GL_SOURCE0_RGB_ARB                0x8580
#define GL_SOURCE1_RGB_ARB                0x8581
#define GL_SOURCE2_RGB_ARB                0x8582
#define GL_SOURCE3_RGB_ARB                0x8583
#define GL_SOURCE4_RGB_ARB                0x8584
#define GL_SOURCE5_RGB_ARB                0x8585
#define GL_SOURCE6_RGB_ARB                0x8586
#define GL_SOURCE7_RGB_ARB                0x8587
#define GL_SOURCE0_ALPHA_ARB              0x8588
#define GL_SOURCE1_ALPHA_ARB              0x8589
#define GL_SOURCE2_ALPHA_ARB              0x858A
#define GL_SOURCE3_ALPHA_ARB              0x858B
#define GL_SOURCE4_ALPHA_ARB              0x858C
#define GL_SOURCE5_ALPHA_ARB              0x858D
#define GL_SOURCE6_ALPHA_ARB              0x858E
#define GL_SOURCE7_ALPHA_ARB              0x858F
#define GL_OPERAND0_RGB_ARB               0x8590
#define GL_OPERAND1_RGB_ARB               0x8591
#define GL_OPERAND2_RGB_ARB               0x8592
#define GL_OPERAND3_RGB_ARB               0x8593
#define GL_OPERAND4_RGB_ARB               0x8594
#define GL_OPERAND5_RGB_ARB               0x8595
#define GL_OPERAND6_RGB_ARB               0x8596
#define GL_OPERAND7_RGB_ARB               0x8597
#define GL_OPERAND0_ALPHA_ARB             0x8598
#define GL_OPERAND1_ALPHA_ARB             0x8599
#define GL_OPERAND2_ALPHA_ARB             0x859A
#define GL_OPERAND3_ALPHA_ARB             0x859B
#define GL_OPERAND4_ALPHA_ARB             0x859C
#define GL_OPERAND5_ALPHA_ARB             0x859D
#define GL_OPERAND6_ALPHA_ARB             0x859E
#define GL_OPERAND7_ALPHA_ARB             0x859F
#endif

#if GL_ARB_texture_mirrored_repeat
#define GL_MIRRORED_REPEAT_ARB            0x8370
#endif

#if GL_ARB_texture_env_dot3
#define GL_DOT3_RGB_ARB                   0x86AE
#define GL_DOT3_RGBA_ARB                  0x86AF
#endif

#if GL_ARB_point_parameters
#define GL_POINT_SIZE_MIN_ARB                            0x8126
#define GL_POINT_SIZE_MAX_ARB                            0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB                 0x8128
#define GL_POINT_DISTANCE_ATTENUATION_ARB                0x8129
#endif

#if GL_ARB_fragment_program
#define GL_FRAGMENT_PROGRAM_ARB                         0x8804
#define GL_PROGRAM_ALU_INSTRUCTIONS_ARB                 0x8805
#define GL_PROGRAM_TEX_INSTRUCTIONS_ARB                 0x8806
#define GL_PROGRAM_TEX_INDIRECTIONS_ARB                 0x8807
#define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB          0x8808
#define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB          0x8809
#define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB          0x880A
#define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB             0x880B
#define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB             0x880C
#define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB             0x880D
#define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB      0x880E
#define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB      0x880F
#define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB      0x8810
#define GL_MAX_TEXTURE_COORDS_ARB                       0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB                  0x8872
#endif

#if GL_ARB_vertex_program
#define GL_VERTEX_PROGRAM_ARB                            0x8620
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB                 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB                   0x8643
#define GL_PROGRAM_FORMAT_ASCII_ARB                      0x8875
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB               0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB                  0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB                0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB                  0x8625
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB            0x886A
#define GL_CURRENT_VERTEX_ATTRIB_ARB                     0x8626
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB               0x8645
#define GL_PROGRAM_LENGTH_ARB                            0x8627
#define GL_PROGRAM_FORMAT_ARB                            0x8876
#define GL_PROGRAM_NAME_ARB                              0x8677
#define GL_PROGRAM_BINDING_ARB                           0x8677
#define GL_PROGRAM_INSTRUCTIONS_ARB                      0x88A0
#define GL_MAX_PROGRAM_INSTRUCTIONS_ARB                  0x88A1
#define GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB               0x88A2
#define GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB           0x88A3
#define GL_PROGRAM_TEMPORARIES_ARB                       0x88A4
#define GL_MAX_PROGRAM_TEMPORARIES_ARB                   0x88A5
#define GL_PROGRAM_NATIVE_TEMPORARIES_ARB                0x88A6
#define GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB            0x88A7
#define GL_PROGRAM_PARAMETERS_ARB                        0x88A8
#define GL_MAX_PROGRAM_PARAMETERS_ARB                    0x88A9
#define GL_PROGRAM_NATIVE_PARAMETERS_ARB                 0x88AA
#define GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB             0x88AB
#define GL_PROGRAM_ATTRIBS_ARB                           0x88AC
#define GL_MAX_PROGRAM_ATTRIBS_ARB                       0x88AD
#define GL_PROGRAM_NATIVE_ATTRIBS_ARB                    0x88AE
#define GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB                0x88AF
#define GL_PROGRAM_ADDRESS_REGISTERS_ARB                 0x88B0
#define GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB             0x88B1
#define GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB          0x88B2
#define GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB      0x88B3
#define GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB              0x88B4
#define GL_MAX_PROGRAM_ENV_PARAMETERS_ARB                0x88B5
#define GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB               0x88B6
#define GL_PROGRAM_STRING_ARB                            0x8628
#define GL_PROGRAM_ERROR_POSITION_ARB                    0x864B
#define GL_CURRENT_MATRIX_ARB                            0x8641
#define GL_TRANSPOSE_CURRENT_MATRIX_ARB                  0x88B7
#define GL_CURRENT_MATRIX_STACK_DEPTH_ARB                0x8640
#define GL_MAX_VERTEX_ATTRIBS_ARB                        0x8869
#define GL_MAX_PROGRAM_MATRICES_ARB                      0x862F
#define GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB            0x862E
#define GL_PROGRAM_ERROR_STRING_ARB                      0x8874

#define GL_MATRIX0_ARB                                   0x88C0
#define GL_MATRIX1_ARB                                   0x88C1
#define GL_MATRIX2_ARB                                   0x88C2
#define GL_MATRIX3_ARB                                   0x88C3
#define GL_MATRIX4_ARB                                   0x88C4
#define GL_MATRIX5_ARB                                   0x88C5
#define GL_MATRIX6_ARB                                   0x88C6
#define GL_MATRIX7_ARB                                   0x88C7
#define GL_MATRIX8_ARB                                   0x88C8
#define GL_MATRIX9_ARB                                   0x88C9
#define GL_MATRIX10_ARB                                  0x88CA
#define GL_MATRIX11_ARB                                  0x88CB
#define GL_MATRIX12_ARB                                  0x88CC
#define GL_MATRIX13_ARB                                  0x88CD
#define GL_MATRIX14_ARB                                  0x88CE
#define GL_MATRIX15_ARB                                  0x88CF
#define GL_MATRIX16_ARB                                  0x88D0
#define GL_MATRIX17_ARB                                  0x88D1
#define GL_MATRIX18_ARB                                  0x88D2
#define GL_MATRIX19_ARB                                  0x88D3
#define GL_MATRIX20_ARB                                  0x88D4
#define GL_MATRIX21_ARB                                  0x88D5
#define GL_MATRIX22_ARB                                  0x88D6
#define GL_MATRIX23_ARB                                  0x88D7
#define GL_MATRIX24_ARB                                  0x88D8
#define GL_MATRIX25_ARB                                  0x88D9
#define GL_MATRIX26_ARB                                  0x88DA
#define GL_MATRIX27_ARB                                  0x88DB
#define GL_MATRIX28_ARB                                  0x88DC
#define GL_MATRIX29_ARB                                  0x88DD
#define GL_MATRIX30_ARB                                  0x88DE
#define GL_MATRIX31_ARB                                  0x88DF

#define GL_COLOR_SUM_ARB                                 0x8458
#endif

#if GL_ARB_shader_objects
#define GL_PROGRAM_OBJECT_ARB                              0x8B40
#define GL_OBJECT_TYPE_ARB                                 0x8B4E
#define GL_OBJECT_SUBTYPE_ARB                              0x8B4F
#define GL_OBJECT_DELETE_STATUS_ARB                        0x8B80
#define GL_OBJECT_COMPILE_STATUS_ARB                       0x8B81
#define GL_OBJECT_LINK_STATUS_ARB                          0x8B82
#define GL_OBJECT_VALIDATE_STATUS_ARB                      0x8B83
#define GL_OBJECT_INFO_LOG_LENGTH_ARB                      0x8B84
#define GL_OBJECT_ATTACHED_OBJECTS_ARB                     0x8B85
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB                      0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB            0x8B87
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB                 0x8B88
#define GL_SHADER_OBJECT_ARB                               0x8B48
#define GL_FLOAT_VEC2_ARB                                  0x8B50
#define GL_FLOAT_VEC3_ARB                                  0x8B51
#define GL_FLOAT_VEC4_ARB                                  0x8B52
#define GL_INT_VEC2_ARB                                    0x8B53
#define GL_INT_VEC3_ARB                                    0x8B54
#define GL_INT_VEC4_ARB                                    0x8B55
#define GL_BOOL_ARB                                        0x8B56
#define GL_BOOL_VEC2_ARB                                   0x8B57
#define GL_BOOL_VEC3_ARB                                   0x8B58
#define GL_BOOL_VEC4_ARB                                   0x8B59
#define GL_FLOAT_MAT2_ARB                                  0x8B5A
#define GL_FLOAT_MAT3_ARB                                  0x8B5B
#define GL_FLOAT_MAT4_ARB                                  0x8B5C
#define GL_SAMPLER_1D_ARB                                  0x8B5D
#define GL_SAMPLER_2D_ARB                                  0x8B5E
#define GL_SAMPLER_3D_ARB                                  0x8B5F
#define GL_SAMPLER_CUBE_ARB                                0x8B60
#define GL_SAMPLER_1D_SHADOW_ARB                           0x8B61
#define GL_SAMPLER_2D_SHADOW_ARB                           0x8B62
#define GL_SAMPLER_2D_RECT_ARB                             0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB                      0x8B64
#endif

#if GL_ARB_vertex_shader || GL_NV_vertex_program3
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB              0x8B4C
#endif

#if GL_ARB_vertex_shader
#define GL_VERTEX_SHADER_ARB                               0x8B31
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB               0x8B4A
#define GL_MAX_TEXTURE_COORDS_ARB                          0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB                     0x8872
#define GL_MAX_VARYING_FLOATS_ARB                          0x8B4B
#define GL_MAX_VERTEX_ATTRIBS_ARB                          0x8869
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB            0x8B4D
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB                   0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB                     0x8643
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB                    0x8B89
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB          0x8B8A
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB                 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB                    0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB                  0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB                    0x8625
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB              0x886A
#define GL_CURRENT_VERTEX_ATTRIB_ARB                       0x8626
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB                 0x8645
#endif

#if GL_ARB_fragment_shader
#define GL_FRAGMENT_SHADER_ARB                             0x8B30
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB             0x8B49
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB             0x8B8B
#define GL_MAX_TEXTURE_COORDS_ARB                          0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB                     0x8872
#endif

#if GL_ARB_shading_language_100
#define GL_SHADING_LANGUAGE_VERSION_ARB                    0x8B8C
#endif

#if GL_ARB_vertex_buffer_object
#define GL_ARRAY_BUFFER_ARB                                0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB                        0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB                        0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB                0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB                 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB                 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB                  0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB                  0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB          0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB              0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB        0x889C
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING_ARB              0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB                 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB          0x889F
#define GL_STREAM_DRAW_ARB                                 0x88E0
#define GL_STREAM_READ_ARB                                 0x88E1
#define GL_STREAM_COPY_ARB                                 0x88E2
#define GL_STATIC_DRAW_ARB                                 0x88E4
#define GL_STATIC_READ_ARB                                 0x88E5
#define GL_STATIC_COPY_ARB                                 0x88E6
#define GL_DYNAMIC_DRAW_ARB                                0x88E8
#define GL_DYNAMIC_READ_ARB                                0x88E9
#define GL_DYNAMIC_COPY_ARB                                0x88EA
#define GL_READ_ONLY_ARB                                   0x88B8
#define GL_WRITE_ONLY_ARB                                  0x88B9
#define GL_READ_WRITE_ARB                                  0x88BA
#define GL_BUFFER_SIZE_ARB                                 0x8764
#define GL_BUFFER_USAGE_ARB                                0x8765
#define GL_BUFFER_ACCESS_ARB                               0x88BB
#define GL_BUFFER_MAPPED_ARB                               0x88BC
#define GL_BUFFER_MAP_POINTER_ARB                          0x88BD
/* Obsolete */
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB         0x889D
#endif

#if GL_ARB_point_sprite
#define GL_POINT_SPRITE_ARB                                0x8861
#define GL_COORD_REPLACE_ARB                               0x8862
#endif

#if GL_ARB_texture_rectangle
#define GL_TEXTURE_RECTANGLE_ARB          0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_ARB  0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_ARB    0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB 0x84F8
#endif

#if GL_ARB_draw_buffers
#define GL_MAX_DRAW_BUFFERS_ARB           0x8824
#define GL_DRAW_BUFFER0_ARB               0x8825
#define GL_DRAW_BUFFER1_ARB               0x8826
#define GL_DRAW_BUFFER2_ARB               0x8827
#define GL_DRAW_BUFFER3_ARB               0x8828
#define GL_DRAW_BUFFER4_ARB               0x8829
#define GL_DRAW_BUFFER5_ARB               0x882A
#define GL_DRAW_BUFFER6_ARB               0x882B
#define GL_DRAW_BUFFER7_ARB               0x882C
#define GL_DRAW_BUFFER8_ARB               0x882D
#define GL_DRAW_BUFFER9_ARB               0x882E
#define GL_DRAW_BUFFER10_ARB              0x882F
#define GL_DRAW_BUFFER11_ARB              0x8830
#define GL_DRAW_BUFFER12_ARB              0x8831
#define GL_DRAW_BUFFER13_ARB              0x8832
#define GL_DRAW_BUFFER14_ARB              0x8833
#define GL_DRAW_BUFFER15_ARB              0x8834
#endif

#if GL_ARB_pixel_buffer_object
#define GL_PIXEL_PACK_BUFFER_ARB                        0x88EB
#define GL_PIXEL_UNPACK_BUFFER_ARB                      0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_ARB                0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_ARB              0x88EF
#endif

#if GL_ARB_texture_float
#define GL_TEXTURE_RED_TYPE_ARB             0x8C10
#define GL_TEXTURE_GREEN_TYPE_ARB           0x8C11
#define GL_TEXTURE_BLUE_TYPE_ARB            0x8C12
#define GL_TEXTURE_ALPHA_TYPE_ARB           0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE_ARB       0x8C14
#define GL_TEXTURE_INTENSITY_TYPE_ARB       0x8C15
#define GL_TEXTURE_DEPTH_TYPE_ARB           0x8C16
#define GL_UNSIGNED_NORMALIZED_ARB          0x8C17
#define GL_RGBA32F_ARB                      0x8814
#define GL_RGB32F_ARB                       0x8815
#define GL_ALPHA32F_ARB                     0x8816
#define GL_INTENSITY32F_ARB                 0x8817
#define GL_LUMINANCE32F_ARB                 0x8818
#define GL_LUMINANCE_ALPHA32F_ARB           0x8819
#define GL_RGBA16F_ARB                      0x881A
#define GL_RGB16F_ARB                       0x881B
#define GL_ALPHA16F_ARB                     0x881C
#define GL_INTENSITY16F_ARB                 0x881D
#define GL_LUMINANCE16F_ARB                 0x881E
#define GL_LUMINANCE_ALPHA16F_ARB           0x881F
#endif

#if GL_ARB_half_float_pixel
#define GL_HALF_FLOAT_ARB                 0x140B
#endif

#if GL_ARB_color_buffer_float
#define GL_RGBA_FLOAT_MODE_ARB            0x8820
#define GL_CLAMP_VERTEX_COLOR_ARB         0x891A
#define GL_CLAMP_FRAGMENT_COLOR_ARB       0x891B
#define GL_CLAMP_READ_COLOR_ARB           0x891C
#define GL_FIXED_ONLY_ARB                 0x891D
#endif

#if GL_ARB_half_float_vertex
#define GL_HALF_FLOAT                     0x140B
#endif

#if GL_ARB_texture_rg
#define GL_COMPRESSED_RED                 0x8225
#define GL_COMPRESSED_RG                  0x8226
#define GL_RG                             0x8227
#define GL_RG_INTEGER                     0x8228
#define GL_R8                             0x8229
#define GL_R16                            0x822A
#define GL_RG8                            0x822B
#define GL_RG16                           0x822C
#define GL_R16F                           0x822D
#define GL_R32F                           0x822E
#define GL_RG16F                          0x822F
#define GL_RG32F                          0x8230
#define GL_R8I                            0x8231
#define GL_R8UI                           0x8232
#define GL_R16I                           0x8233
#define GL_R16UI                          0x8234
#define GL_R32I                           0x8235
#define GL_R32UI                          0x8236
#define GL_RG8I                           0x8237
#define GL_RG8UI                          0x8238
#define GL_RG16I                          0x8239
#define GL_RG16UI                         0x823A
#define GL_RG32I                          0x823B
#define GL_RG32UI                         0x823C
#endif

#if GL_ARB_instanced_arrays
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ARB                 0x88FE
#endif

#if GL_ARB_uniform_buffer_object
#define GL_UNIFORM_BUFFER                                  0x8A11
#define GL_UNIFORM_BUFFER_BINDING                          0x8A28
#define GL_UNIFORM_BUFFER_START                            0x8A29
#define GL_UNIFORM_BUFFER_SIZE                             0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS                       0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS                     0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS                     0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS                     0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS                     0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE                          0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS          0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS        0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS        0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT                 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH            0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS                           0x8A36
#define GL_UNIFORM_TYPE                                    0x8A37
#define GL_UNIFORM_SIZE                                    0x8A38
#define GL_UNIFORM_NAME_LENGTH                             0x8A39
#define GL_UNIFORM_BLOCK_INDEX                             0x8A3A
#define GL_UNIFORM_OFFSET                                  0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE                            0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE                           0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR                            0x8A3E
#define GL_UNIFORM_BLOCK_BINDING                           0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE                         0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH                       0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS                   0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES            0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER       0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER     0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER     0x8A46
#define GL_INVALID_INDEX                                   0xFFFFFFFF
#endif

#if GL_ARB_depth_buffer_float
#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_DEPTH32F_STENCIL8              0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#endif

#if GL_EXT_abgr
#define GL_ABGR_EXT                       0x8000
#endif

#if GL_EXT_blend_color
#define GL_CONSTANT_COLOR_EXT             0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR_EXT   0x8002
#define GL_CONSTANT_ALPHA_EXT             0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA_EXT   0x8004
#define GL_BLEND_COLOR_EXT                0x8005
#endif

#if GL_EXT_polygon_offset
#define GL_POLYGON_OFFSET_EXT             0x8037
#define GL_POLYGON_OFFSET_FACTOR_EXT      0x8038
#define GL_POLYGON_OFFSET_BIAS_EXT        0x8039
#endif

#if GL_EXT_texture
#define GL_ALPHA4_EXT                     0x803B
#define GL_ALPHA8_EXT                     0x803C
#define GL_ALPHA12_EXT                    0x803D
#define GL_ALPHA16_EXT                    0x803E
#define GL_LUMINANCE4_EXT                 0x803F
#define GL_LUMINANCE8_EXT                 0x8040
#define GL_LUMINANCE12_EXT                0x8041
#define GL_LUMINANCE16_EXT                0x8042
#define GL_LUMINANCE4_ALPHA4_EXT          0x8043
#define GL_LUMINANCE6_ALPHA2_EXT          0x8044
#define GL_LUMINANCE8_ALPHA8_EXT          0x8045
#define GL_LUMINANCE12_ALPHA4_EXT         0x8046
#define GL_LUMINANCE12_ALPHA12_EXT        0x8047
#define GL_LUMINANCE16_ALPHA16_EXT        0x8048
#define GL_INTENSITY_EXT                  0x8049
#define GL_INTENSITY4_EXT                 0x804A
#define GL_INTENSITY8_EXT                 0x804B
#define GL_INTENSITY12_EXT                0x804C
#define GL_INTENSITY16_EXT                0x804D
#define GL_RGB2_EXT                       0x804E
#define GL_RGB4_EXT                       0x804F
#define GL_RGB5_EXT                       0x8050
#define GL_RGB8_EXT                       0x8051
#define GL_RGB10_EXT                      0x8052
#define GL_RGB12_EXT                      0x8053
#define GL_RGB16_EXT                      0x8054
#define GL_RGBA2_EXT                      0x8055
#define GL_RGBA4_EXT                      0x8056
#define GL_RGB5_A1_EXT                    0x8057
#define GL_RGBA8_EXT                      0x8058
#define GL_RGB10_A2_EXT                   0x8059
#define GL_RGBA12_EXT                     0x805A
#define GL_RGBA16_EXT                     0x805B
#define GL_TEXTURE_RED_SIZE_EXT           0x805C
#define GL_TEXTURE_GREEN_SIZE_EXT         0x805D
#define GL_TEXTURE_BLUE_SIZE_EXT          0x805E
#define GL_TEXTURE_ALPHA_SIZE_EXT         0x805F
#define GL_TEXTURE_LUMINANCE_SIZE_EXT     0x8060
#define GL_TEXTURE_INTENSITY_SIZE_EXT     0x8061
#define GL_REPLACE_EXT                    0x8062
#define GL_PROXY_TEXTURE_1D_EXT           0x8063
#define GL_PROXY_TEXTURE_2D_EXT           0x8064
#define GL_TEXTURE_TOO_LARGE_EXT          0x8065
#endif

#if GL_EXT_texture3D
#define GL_PACK_SKIP_IMAGES_EXT           0x806B
#define GL_PACK_IMAGE_HEIGHT_EXT          0x806C
#define GL_UNPACK_SKIP_IMAGES_EXT         0x806D
#define GL_UNPACK_IMAGE_HEIGHT_EXT        0x806E
#define GL_TEXTURE_3D_EXT                 0x806F
#define GL_PROXY_TEXTURE_3D_EXT           0x8070
#define GL_TEXTURE_DEPTH_EXT              0x8071
#define GL_TEXTURE_WRAP_R_EXT             0x8072
#define GL_MAX_3D_TEXTURE_SIZE_EXT        0x8073
#endif

#if GL_EXT_histogram
#define GL_HISTOGRAM_EXT                  0x8024
#define GL_PROXY_HISTOGRAM_EXT            0x8025
#define GL_HISTOGRAM_WIDTH_EXT            0x8026
#define GL_HISTOGRAM_FORMAT_EXT           0x8027
#define GL_HISTOGRAM_RED_SIZE_EXT         0x8028
#define GL_HISTOGRAM_GREEN_SIZE_EXT       0x8029
#define GL_HISTOGRAM_BLUE_SIZE_EXT        0x802A
#define GL_HISTOGRAM_ALPHA_SIZE_EXT       0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE_EXT   0x802C
#define GL_HISTOGRAM_SINK_EXT             0x802D
#define GL_MINMAX_EXT                     0x802E
#define GL_MINMAX_FORMAT_EXT              0x802F
#define GL_MINMAX_SINK_EXT                0x8030
#define GL_TABLE_TOO_LARGE_EXT            0x8031
#endif

#if GL_EXT_convolution
#define GL_CONVOLUTION_1D_EXT             0x8010
#define GL_CONVOLUTION_2D_EXT             0x8011
#define GL_SEPARABLE_2D_EXT               0x8012
#define GL_CONVOLUTION_BORDER_MODE_EXT    0x8013
#define GL_CONVOLUTION_FILTER_SCALE_EXT   0x8014
#define GL_CONVOLUTION_FILTER_BIAS_EXT    0x8015
#define GL_REDUCE_EXT                     0x8016
#define GL_CONVOLUTION_FORMAT_EXT         0x8017
#define GL_CONVOLUTION_WIDTH_EXT          0x8018
#define GL_CONVOLUTION_HEIGHT_EXT         0x8019
#define GL_MAX_CONVOLUTION_WIDTH_EXT      0x801A
#define GL_MAX_CONVOLUTION_HEIGHT_EXT     0x801B
#define GL_POST_CONVOLUTION_RED_SCALE_EXT 0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE_EXT 0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE_EXT 0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE_EXT 0x801F
#define GL_POST_CONVOLUTION_RED_BIAS_EXT  0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS_EXT 0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS_EXT 0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS_EXT 0x8023
#endif

#if GL_EXT_cmyka
#define GL_CMYK_EXT                       0x800C
#define GL_CMYKA_EXT                      0x800D
#define GL_PACK_CMYK_HINT_EXT             0x800E
#define GL_UNPACK_CMYK_HINT_EXT           0x800F
#endif

#if GL_EXT_texture_object
#define GL_TEXTURE_PRIORITY_EXT           0x8066
#define GL_TEXTURE_RESIDENT_EXT           0x8067
#define GL_TEXTURE_1D_BINDING_EXT         0x8068
#define GL_TEXTURE_2D_BINDING_EXT         0x8069
#define GL_TEXTURE_3D_BINDING_EXT         0x806A
#endif

#if GL_EXT_packed_pixels
#define GL_UNSIGNED_BYTE_3_3_2_EXT        0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT     0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT     0x8034
#define GL_UNSIGNED_INT_8_8_8_8_EXT       0x8035
#define GL_UNSIGNED_INT_10_10_10_2_EXT    0x8036
#endif

#if GL_EXT_rescale_normal
#define GL_RESCALE_NORMAL_EXT             0x803A
#endif

#if GL_EXT_vertex_array
#define GL_VERTEX_ARRAY_EXT               0x8074
#define GL_NORMAL_ARRAY_EXT               0x8075
#define GL_COLOR_ARRAY_EXT                0x8076
#define GL_INDEX_ARRAY_EXT                0x8077
#define GL_TEXTURE_COORD_ARRAY_EXT        0x8078
#define GL_EDGE_FLAG_ARRAY_EXT            0x8079
#define GL_VERTEX_ARRAY_SIZE_EXT          0x807A
#define GL_VERTEX_ARRAY_TYPE_EXT          0x807B
#define GL_VERTEX_ARRAY_STRIDE_EXT        0x807C
#define GL_VERTEX_ARRAY_COUNT_EXT         0x807D
#define GL_NORMAL_ARRAY_TYPE_EXT          0x807E
#define GL_NORMAL_ARRAY_STRIDE_EXT        0x807F
#define GL_NORMAL_ARRAY_COUNT_EXT         0x8080
#define GL_COLOR_ARRAY_SIZE_EXT           0x8081
#define GL_COLOR_ARRAY_TYPE_EXT           0x8082
#define GL_COLOR_ARRAY_STRIDE_EXT         0x8083
#define GL_COLOR_ARRAY_COUNT_EXT          0x8084
#define GL_INDEX_ARRAY_TYPE_EXT           0x8085
#define GL_INDEX_ARRAY_STRIDE_EXT         0x8086
#define GL_INDEX_ARRAY_COUNT_EXT          0x8087
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT   0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT   0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT 0x808A
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT  0x808B
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT     0x808C
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT      0x808D
#define GL_VERTEX_ARRAY_POINTER_EXT       0x808E
#define GL_NORMAL_ARRAY_POINTER_EXT       0x808F
#define GL_COLOR_ARRAY_POINTER_EXT        0x8090
#define GL_INDEX_ARRAY_POINTER_EXT        0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT    0x8093
#endif

#if GL_EXT_blend_minmax
#define GL_FUNC_ADD_EXT                   0x8006
#define GL_MIN_EXT                        0x8007
#define GL_MAX_EXT                        0x8008
#define GL_BLEND_EQUATION_EXT             0x8009
#endif

#if GL_EXT_blend_subtract
#define GL_FUNC_SUBTRACT_EXT              0x800A
#define GL_FUNC_REVERSE_SUBTRACT_EXT      0x800B
#endif

#if GL_EXT_paletted_texture
#define GL_COLOR_INDEX1_EXT               0x80E2
#define GL_COLOR_INDEX2_EXT               0x80E3
#define GL_COLOR_INDEX4_EXT               0x80E4
#define GL_COLOR_INDEX8_EXT               0x80E5
#define GL_COLOR_INDEX12_EXT              0x80E6
#define GL_COLOR_INDEX16_EXT              0x80E7
#define GL_TEXTURE_INDEX_SIZE_EXT         0x80ED
#endif

#if GL_EXT_clip_volume_hint
#define GL_CLIP_VOLUME_CLIPPING_HINT_EXT  0x80F0
#endif

#if GL_EXT_index_material
#define GL_INDEX_MATERIAL_EXT             0x81B8
#define GL_INDEX_MATERIAL_PARAMETER_EXT   0x81B9
#define GL_INDEX_MATERIAL_FACE_EXT        0x81BA
#endif

#if GL_EXT_index_func
#define GL_INDEX_TEST_EXT                 0x81B5
#define GL_INDEX_TEST_FUNC_EXT            0x81B6
#define GL_INDEX_TEST_REF_EXT             0x81B7
#endif

#if GL_EXT_index_array_formats
#define GL_IUI_V2F_EXT                    0x81AD
#define GL_IUI_V3F_EXT                    0x81AE
#define GL_IUI_N3F_V2F_EXT                0x81AF
#define GL_IUI_N3F_V3F_EXT                0x81B0
#define GL_T2F_IUI_V2F_EXT                0x81B1
#define GL_T2F_IUI_V3F_EXT                0x81B2
#define GL_T2F_IUI_N3F_V2F_EXT            0x81B3
#define GL_T2F_IUI_N3F_V3F_EXT            0x81B4
#endif

#if GL_EXT_compiled_vertex_array
#define GL_ARRAY_ELEMENT_LOCK_FIRST_EXT   0x81A8
#define GL_ARRAY_ELEMENT_LOCK_COUNT_EXT   0x81A9
#endif

#if GL_EXT_cull_vertex
#define GL_CULL_VERTEX_EXT                0x81AA
#define GL_CULL_VERTEX_EYE_POSITION_EXT   0x81AB
#define GL_CULL_VERTEX_OBJECT_POSITION_EXT 0x81AC
#endif

#if GL_EXT_draw_range_elements
#define GL_MAX_ELEMENTS_VERTICES_EXT      0x80E8
#define GL_MAX_ELEMENTS_INDICES_EXT       0x80E9
#endif

#if GL_EXT_light_texture
#define GL_FRAGMENT_MATERIAL_EXT          0x8349
#define GL_FRAGMENT_NORMAL_EXT            0x834A
#define GL_FRAGMENT_COLOR_EXT             0x834C
#define GL_ATTENUATION_EXT                0x834D
#define GL_SHADOW_ATTENUATION_EXT         0x834E
#define GL_TEXTURE_APPLICATION_MODE_EXT   0x834F
#define GL_TEXTURE_LIGHT_EXT              0x8350
#define GL_TEXTURE_MATERIAL_FACE_EXT      0x8351
#define GL_TEXTURE_MATERIAL_PARAMETER_EXT 0x8352
/* reuse GL_FRAGMENT_DEPTH_EXT */
#endif

#if GL_EXT_bgra
#define GL_BGR_EXT                        0x80E0
#define GL_BGRA_EXT                       0x80E1
#endif

#if GL_EXT_pixel_transform
#define GL_PIXEL_TRANSFORM_2D_EXT         0x8330
#define GL_PIXEL_MAG_FILTER_EXT           0x8331
#define GL_PIXEL_MIN_FILTER_EXT           0x8332
#define GL_PIXEL_CUBIC_WEIGHT_EXT         0x8333
#define GL_CUBIC_EXT                      0x8334
#define GL_AVERAGE_EXT                    0x8335
#define GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8336
#define GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8337
#define GL_PIXEL_TRANSFORM_2D_MATRIX_EXT  0x8338
#endif

#if GL_EXT_shared_texture_palette
#define GL_SHARED_TEXTURE_PALETTE_EXT     0x81FB
#endif

#if GL_EXT_separate_specular_color
#define GL_LIGHT_MODEL_COLOR_CONTROL_EXT  0x81F8
#define GL_SINGLE_COLOR_EXT               0x81F9
#define GL_SEPARATE_SPECULAR_COLOR_EXT    0x81FA
#endif

#if GL_EXT_secondary_color
#define GL_COLOR_SUM_EXT                  0x8458
#define GL_CURRENT_SECONDARY_COLOR_EXT    0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE_EXT 0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE_EXT 0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT 0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER_EXT 0x845D
#define GL_SECONDARY_COLOR_ARRAY_EXT      0x845E
#endif

#if GL_EXT_texture_perturb_normal
#define GL_PERTURB_EXT                    0x85AE
#define GL_TEXTURE_NORMAL_EXT             0x85AF
#endif

#if GL_EXT_fog_coord
#define GL_FOG_COORDINATE_SOURCE_EXT      0x8450
#define GL_FOG_COORDINATE_EXT             0x8451
#define GL_FRAGMENT_DEPTH_EXT             0x8452
#define GL_CURRENT_FOG_COORDINATE_EXT     0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE_EXT  0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE_EXT 0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER_EXT 0x8456
#define GL_FOG_COORDINATE_ARRAY_EXT       0x8457
#endif

#if GL_EXT_coordinate_frame
#define GL_TANGENT_ARRAY_EXT              0x8439
#define GL_BINORMAL_ARRAY_EXT             0x843A
#define GL_CURRENT_TANGENT_EXT            0x843B
#define GL_CURRENT_BINORMAL_EXT           0x843C
#define GL_TANGENT_ARRAY_TYPE_EXT         0x843E
#define GL_TANGENT_ARRAY_STRIDE_EXT       0x843F
#define GL_BINORMAL_ARRAY_TYPE_EXT        0x8440
#define GL_BINORMAL_ARRAY_STRIDE_EXT      0x8441
#define GL_TANGENT_ARRAY_POINTER_EXT      0x8442
#define GL_BINORMAL_ARRAY_POINTER_EXT     0x8443
#define GL_MAP1_TANGENT_EXT               0x8444
#define GL_MAP2_TANGENT_EXT               0x8445
#define GL_MAP1_BINORMAL_EXT              0x8446
#define GL_MAP2_BINORMAL_EXT              0x8447
#endif

#if GL_EXT_texture_env_combine
#define GL_COMBINE_EXT                    0x8570
#define GL_COMBINE_RGB_EXT                0x8571
#define GL_COMBINE_ALPHA_EXT              0x8572
#define GL_RGB_SCALE_EXT                  0x8573
#define GL_ADD_SIGNED_EXT                 0x8574
#define GL_INTERPOLATE_EXT                0x8575
#define GL_CONSTANT_EXT                   0x8576
#define GL_PRIMARY_COLOR_EXT              0x8577
#define GL_PREVIOUS_EXT                   0x8578
#define GL_SOURCE0_RGB_EXT                0x8580
#define GL_SOURCE1_RGB_EXT                0x8581
#define GL_SOURCE2_RGB_EXT                0x8582
#define GL_SOURCE3_RGB_EXT                0x8583
#define GL_SOURCE4_RGB_EXT                0x8584
#define GL_SOURCE5_RGB_EXT                0x8585
#define GL_SOURCE6_RGB_EXT                0x8586
#define GL_SOURCE7_RGB_EXT                0x8587
#define GL_SOURCE0_ALPHA_EXT              0x8588
#define GL_SOURCE1_ALPHA_EXT              0x8589
#define GL_SOURCE2_ALPHA_EXT              0x858A
#define GL_SOURCE3_ALPHA_EXT              0x858B
#define GL_SOURCE4_ALPHA_EXT              0x858C
#define GL_SOURCE5_ALPHA_EXT              0x858D
#define GL_SOURCE6_ALPHA_EXT              0x858E
#define GL_SOURCE7_ALPHA_EXT              0x858F
#define GL_OPERAND0_RGB_EXT               0x8590
#define GL_OPERAND1_RGB_EXT               0x8591
#define GL_OPERAND2_RGB_EXT               0x8592
#define GL_OPERAND3_RGB_EXT               0x8593
#define GL_OPERAND4_RGB_EXT               0x8594
#define GL_OPERAND5_RGB_EXT               0x8595
#define GL_OPERAND6_RGB_EXT               0x8596
#define GL_OPERAND7_RGB_EXT               0x8597
#define GL_OPERAND0_ALPHA_EXT             0x8598
#define GL_OPERAND1_ALPHA_EXT             0x8599
#define GL_OPERAND2_ALPHA_EXT             0x859A
#define GL_OPERAND3_ALPHA_EXT             0x859B
#define GL_OPERAND4_ALPHA_EXT             0x859C
#define GL_OPERAND5_ALPHA_EXT             0x859D
#define GL_OPERAND6_ALPHA_EXT             0x859E
#define GL_OPERAND7_ALPHA_EXT             0x859F
#endif

#if GL_EXT_blend_func_separate
#define GL_BLEND_DST_RGB_EXT              0x80C8
#define GL_BLEND_SRC_RGB_EXT              0x80C9
#define GL_BLEND_DST_ALPHA_EXT            0x80CA
#define GL_BLEND_SRC_ALPHA_EXT            0x80CB
#endif

#if GL_EXT_stencil_wrap
#define GL_INCR_WRAP_EXT                  0x8507
#define GL_DECR_WRAP_EXT                  0x8508
#endif

#if GL_EXT_422_pixels
#define GL_422_EXT                        0x80CC
#define GL_422_REV_EXT                    0x80CD
#define GL_422_AVERAGE_EXT                0x80CE
#define GL_422_REV_AVERAGE_EXT            0x80CF
#endif

#if GL_EXT_texture_cube_map
#define GL_NORMAL_MAP_EXT                 0x8511
#define GL_REFLECTION_MAP_EXT             0x8512
#define GL_TEXTURE_CUBE_MAP_EXT           0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_EXT   0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_EXT     0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT  0x851C
#endif

#if GL_EXT_texture_lod_bias
#define GL_MAX_TEXTURE_LOD_BIAS_EXT       0x84FD
#define GL_TEXTURE_FILTER_CONTROL_EXT     0x8500
#define GL_TEXTURE_LOD_BIAS_EXT           0x8501
#endif

#if GL_EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif

#if GL_EXT_vertex_weighting
#define GL_MODELVIEW0_STACK_DEPTH_EXT     GL_MODELVIEW_STACK_DEPTH
#define GL_MODELVIEW1_STACK_DEPTH_EXT     0x8502
#define GL_MODELVIEW0_MATRIX_EXT          GL_MODELVIEW_MATRIX
#define GL_MODELVIEW_MATRIX1_EXT          0x8506
#define GL_VERTEX_WEIGHTING_EXT           0x8509
#define GL_MODELVIEW0_EXT                 GL_MODELVIEW
#define GL_MODELVIEW1_EXT                 0x850A
#define GL_CURRENT_VERTEX_WEIGHT_EXT      0x850B
#define GL_VERTEX_WEIGHT_ARRAY_EXT        0x850C
#define GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT   0x850D
#define GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT   0x850E
#define GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT 0x850F
#define GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT 0x8510
#endif

#if GL_EXT_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
#endif

#if GL_EXT_texture_rectangle
#define GL_TEXTURE_RECTANGLE_EXT          0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_EXT  0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_EXT    0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT 0x84F8
#endif

#if GL_EXT_texture_sRGB
#define GL_SRGB_EXT                            0x8C40
#define GL_SRGB8_EXT                           0x8C41
#define GL_SRGB_ALPHA_EXT                      0x8C42
#define GL_SRGB8_ALPHA8_EXT                    0x8C43
#define GL_SLUMINANCE_ALPHA_EXT                0x8C44
#define GL_SLUMINANCE8_ALPHA8_EXT              0x8C45
#define GL_SLUMINANCE_EXT                      0x8C46
#define GL_SLUMINANCE8_EXT                     0x8C47
#define GL_COMPRESSED_SRGB_EXT                 0x8C48
#define GL_COMPRESSED_SRGB_ALPHA_EXT           0x8C49
#define GL_COMPRESSED_SLUMINANCE_EXT           0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA_EXT     0x8C4B
#define GL_COMPRESSED_SRGB_S3TC_DXT1_EXT       0x8C4C
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT 0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT 0x8C4E
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT 0x8C4F
#endif

#if GL_EXT_vertex_shader
#define GL_VERTEX_SHADER_EXT              0x8780
#define GL_VARIANT_VALUE_EXT              0x87E4
#define GL_VARIANT_DATATYPE_EXT           0x87E5
#define GL_VARIANT_ARRAY_STRIDE_EXT       0x87E6
#define GL_VARIANT_ARRAY_TYPE_EXT         0x87E7
#define GL_VARIANT_ARRAY_EXT              0x87E8
#define GL_VARIANT_ARRAY_POINTER_EXT      0x87E9
#define GL_INVARIANT_VALUE_EXT            0x87EA
#define GL_INVARIANT_DATATYPE_EXT         0x87EB
#define GL_LOCAL_CONSTANT_VALUE_EXT       0x87EC
#define GL_LOCAL_CONSTANT_DATATYPE_EXT    0x87Ed
#define GL_OP_INDEX_EXT                   0x8782
#define GL_OP_NEGATE_EXT                  0x8783
#define GL_OP_DOT3_EXT                    0x8784
#define GL_OP_DOT4_EXT                    0x8785
#define GL_OP_MUL_EXT                     0x8786
#define GL_OP_ADD_EXT                     0x8787
#define GL_OP_MADD_EXT                    0x8788
#define GL_OP_FRAC_EXT                    0x8789
#define GL_OP_MAX_EXT                     0x878A
#define GL_OP_MIN_EXT                     0x878B
#define GL_OP_SET_GE_EXT                  0x878C
#define GL_OP_SET_LT_EXT                  0x878D
#define GL_OP_CLAMP_EXT                   0x878E
#define GL_OP_FLOOR_EXT                   0x878F
#define GL_OP_ROUND_EXT                   0x8790
#define GL_OP_EXP_BASE_2_EXT              0x8791
#define GL_OP_LOG_BASE_2_EXT              0x8792
#define GL_OP_POWER_EXT                   0x8793
#define GL_OP_RECIP_EXT                   0x8794
#define GL_OP_RECIP_SQRT_EXT              0x8795
#define GL_OP_SUB_EXT                     0x8796
#define GL_OP_CROSS_PRODUCT_EXT           0x8797
#define GL_OP_MULTIPLY_MATRIX_EXT         0x8798
#define GL_OP_MOV_EXT                     0x8799
#define GL_OUTPUT_VERTEX_EXT              0x879A
#define GL_OUTPUT_COLOR0_EXT              0x879B
#define GL_OUTPUT_COLOR1_EXT              0x879C
#define GL_OUTPUT_TEXTURE_COORD0_EXT      0x879D
#define GL_OUTPUT_TEXTURE_COORD1_EXT      0x879E
#define GL_OUTPUT_TEXTURE_COORD2_EXT      0x879F
#define GL_OUTPUT_TEXTURE_COORD3_EXT      0x87A0
#define GL_OUTPUT_TEXTURE_COORD4_EXT      0x87A1
#define GL_OUTPUT_TEXTURE_COORD5_EXT      0x87A2
#define GL_OUTPUT_TEXTURE_COORD6_EXT      0x87A3
#define GL_OUTPUT_TEXTURE_COORD7_EXT      0x87A4
#define GL_OUTPUT_TEXTURE_COORD8_EXT      0x87A5
#define GL_OUTPUT_TEXTURE_COORD9_EXT      0x87A6
#define GL_OUTPUT_TEXTURE_COORD10_EXT     0x87A7
#define GL_OUTPUT_TEXTURE_COORD11_EXT     0x87A8
#define GL_OUTPUT_TEXTURE_COORD12_EXT     0x87A9
#define GL_OUTPUT_TEXTURE_COORD13_EXT     0x87AA
#define GL_OUTPUT_TEXTURE_COORD14_EXT     0x87AB
#define GL_OUTPUT_TEXTURE_COORD15_EXT     0x87AC
#define GL_OUTPUT_TEXTURE_COORD16_EXT     0x87AD
#define GL_OUTPUT_TEXTURE_COORD17_EXT     0x87AE
#define GL_OUTPUT_TEXTURE_COORD18_EXT     0x87AF
#define GL_OUTPUT_TEXTURE_COORD19_EXT     0x87B0
#define GL_OUTPUT_TEXTURE_COORD20_EXT     0x87B1
#define GL_OUTPUT_TEXTURE_COORD21_EXT     0x87B2
#define GL_OUTPUT_TEXTURE_COORD22_EXT     0x87B3
#define GL_OUTPUT_TEXTURE_COORD23_EXT     0x87B4
#define GL_OUTPUT_TEXTURE_COORD24_EXT     0x87B5
#define GL_OUTPUT_TEXTURE_COORD25_EXT     0x87B6
#define GL_OUTPUT_TEXTURE_COORD26_EXT     0x87B7
#define GL_OUTPUT_TEXTURE_COORD27_EXT     0x87B8
#define GL_OUTPUT_TEXTURE_COORD28_EXT     0x87B9
#define GL_OUTPUT_TEXTURE_COORD29_EXT     0x87BA
#define GL_OUTPUT_TEXTURE_COORD30_EXT     0x87BB
#define GL_OUTPUT_TEXTURE_COORD31_EXT     0x87BC
#define GL_OUTPUT_FOG_EXT                 0x87BD
#define GL_SCALAR_EXT                     0x87BE
#define GL_VECTOR_EXT                     0x87BF
#define GL_MATRIX_EXT                     0x87C0
#define GL_VARIANT_EXT                    0x87C1
#define GL_INVARIANT_EXT                  0x87C2
#define GL_LOCAL_CONSTANT_EXT             0x87C3
#define GL_LOCAL_EXT                      0x87C4
#define GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT              0x87C5
#define GL_MAX_VERTEX_SHADER_VARIANTS_EXT                  0x87C6
#define GL_MAX_VERTEX_SHADER_INVARIANTS_EXT                0x87C7
#define GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT           0x87C8
#define GL_MAX_VERTEX_SHADER_LOCALS_EXT                    0x87C9
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT    0x87CA
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT        0x87CB
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87CC
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT      0x87CD
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT          0x87CE
#define GL_VERTEX_SHADER_INSTRUCTIONS_EXT                  0x87CF
#define GL_VERTEX_SHADER_VARIANTS_EXT                      0x87D0
#define GL_VERTEX_SHADER_INVARIANTS_EXT                    0x87D1
#define GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT               0x87D2
#define GL_VERTEX_SHADER_LOCALS_EXT                        0x87D3
#define GL_VERTEX_SHADER_BINDING_EXT                       0x8781
#define GL_VERTEX_SHADER_OPTIMIZED_EXT                     0x87D4
#define GL_X_EXT                          0x87D5
#define GL_Y_EXT                          0x87D6
#define GL_Z_EXT                          0x87D7
#define GL_W_EXT                          0x87D8
#define GL_NEGATIVE_X_EXT                 0x87D9
#define GL_NEGATIVE_Y_EXT                 0x87DA
#define GL_NEGATIVE_Z_EXT                 0x87DB
#define GL_NEGATIVE_W_EXT                 0x87DC
#define GL_NEGATIVE_ONE_EXT               0x87DF
#define GL_NORMALIZED_RANGE_EXT           0x87E0
#define GL_FULL_RANGE_EXT                 0x87E1
#define GL_CURRENT_VERTEX_EXT             0x87E2
#define GL_MVP_MATRIX_EXT                 0x87E3
#endif

#if GL_EXT_fragment_shader
#define GL_FRAGMENT_SHADER_EXT            0x8920
#define GL_REG_0_EXT                      0x8921
#define GL_REG_1_EXT                      0x8922
#define GL_REG_2_EXT                      0x8923
#define GL_REG_3_EXT                      0x8924
#define GL_REG_4_EXT                      0x8925
#define GL_REG_5_EXT                      0x8926
#define GL_REG_6_EXT                      0x8927
#define GL_REG_7_EXT                      0x8928
#define GL_REG_8_EXT                      0x8929
#define GL_REG_9_EXT                      0x892A
#define GL_REG_10_EXT                     0x892B
#define GL_REG_11_EXT                     0x892C
#define GL_REG_12_EXT                     0x892D
#define GL_REG_13_EXT                     0x892E
#define GL_REG_14_EXT                     0x892F
#define GL_REG_15_EXT                     0x8930
#define GL_REG_16_EXT                     0x8931
#define GL_REG_17_EXT                     0x8932
#define GL_REG_18_EXT                     0x8933
#define GL_REG_19_EXT                     0x8934
#define GL_REG_20_EXT                     0x8935
#define GL_REG_21_EXT                     0x8936
#define GL_REG_22_EXT                     0x8937
#define GL_REG_23_EXT                     0x8938
#define GL_REG_24_EXT                     0x8939
#define GL_REG_25_EXT                     0x893A
#define GL_REG_26_EXT                     0x893B
#define GL_REG_27_EXT                     0x893C
#define GL_REG_28_EXT                     0x893D
#define GL_REG_29_EXT                     0x893E
#define GL_REG_30_EXT                     0x893F
#define GL_REG_31_EXT                     0x8940
#define GL_CON_0_EXT                      0x8941
#define GL_CON_1_EXT                      0x8942
#define GL_CON_2_EXT                      0x8943
#define GL_CON_3_EXT                      0x8944
#define GL_CON_4_EXT                      0x8945
#define GL_CON_5_EXT                      0x8946
#define GL_CON_6_EXT                      0x8947
#define GL_CON_7_EXT                      0x8948
#define GL_CON_8_EXT                      0x8949
#define GL_CON_9_EXT                      0x894A
#define GL_CON_10_EXT                     0x894B
#define GL_CON_11_EXT                     0x894C
#define GL_CON_12_EXT                     0x894D
#define GL_CON_13_EXT                     0x894E
#define GL_CON_14_EXT                     0x894F
#define GL_CON_15_EXT                     0x8950
#define GL_CON_16_EXT                     0x8951
#define GL_CON_17_EXT                     0x8952
#define GL_CON_18_EXT                     0x8953
#define GL_CON_19_EXT                     0x8954
#define GL_CON_20_EXT                     0x8955
#define GL_CON_21_EXT                     0x8956
#define GL_CON_22_EXT                     0x8957
#define GL_CON_23_EXT                     0x8958
#define GL_CON_24_EXT                     0x8959
#define GL_CON_25_EXT                     0x895A
#define GL_CON_26_EXT                     0x895B
#define GL_CON_27_EXT                     0x895C
#define GL_CON_28_EXT                     0x895D
#define GL_CON_29_EXT                     0x895E
#define GL_CON_30_EXT                     0x895F
#define GL_CON_31_EXT                     0x8960
#define GL_MOV_EXT                        0x8961
#define GL_ADD_EXT                        0x8963
#define GL_MUL_EXT                        0x8964
#define GL_SUB_EXT                        0x8965
#define GL_DOT3_EXT                       0x8966
#define GL_DOT4_EXT                       0x8967
#define GL_MAD_EXT                        0x8968
#define GL_LERP_EXT                       0x8969
#define GL_CND_EXT                        0x896A
#define GL_CND0_EXT                       0x896B
#define GL_DOT2_ADD_EXT                   0x896C
#define GL_SECONDARY_INTERPOLATOR_EXT     0x896D
#define GL_NUM_FRAGMENT_REGISTERS_EXT     0x896E
#define GL_NUM_FRAGMENT_CONSTANTS_EXT     0x896F
#define GL_NUM_PASSES_EXT                 0x8970
#define GL_NUM_INSTRUCTIONS_PER_PASS_EXT  0x8971
#define GL_NUM_INSTRUCTIONS_TOTAL_EXT     0x8972
#define GL_NUM_INPUT_INTERPOLATOR_COMPONENTS_EXT 0x8973
#define GL_NUM_LOOPBACK_COMPONENTS_EXT    0x8974
#define GL_COLOR_ALPHA_PAIRING_EXT        0x8975
#define GL_SWIZZLE_STR_EXT                0x8976
#define GL_SWIZZLE_STQ_EXT                0x8977
#define GL_SWIZZLE_STR_DR_EXT             0x8978
#define GL_SWIZZLE_STQ_DQ_EXT             0x8979
#define GL_SWIZZLE_STRQ_EXT               0x897A
#define GL_SWIZZLE_STRQ_DQ_EXT            0x897B
#define GL_RED_BIT_EXT                    0x00000001
#define GL_GREEN_BIT_EXT                  0x00000002
#define GL_BLUE_BIT_EXT                   0x00000004
#define GL_2X_BIT_EXT                     0x00000001
#define GL_4X_BIT_EXT                     0x00000002
#define GL_8X_BIT_EXT                     0x00000004
#define GL_HALF_BIT_EXT                   0x00000008
#define GL_QUARTER_BIT_EXT                0x00000010
#define GL_EIGHTH_BIT_EXT                 0x00000020
#define GL_SATURATE_BIT_EXT               0x00000040
#define GL_COMP_BIT_EXT                   0x00000002
#define GL_NEGATE_BIT_EXT                 0x00000004
#define GL_BIAS_BIT_EXT                   0x00000008
#endif

#if GL_EXT_multisample
#define GL_MULTISAMPLE_EXT                0x809D
#define GL_SAMPLE_ALPHA_TO_MASK_EXT       0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_EXT        0x809F
#define GL_SAMPLE_MASK_EXT                0x80A0
#define GL_1PASS_EXT                      0x80A1
#define GL_2PASS_0_EXT                    0x80A2
#define GL_2PASS_1_EXT                    0x80A3
#define GL_4PASS_0_EXT                    0x80A4
#define GL_4PASS_1_EXT                    0x80A5
#define GL_4PASS_2_EXT                    0x80A6
#define GL_4PASS_3_EXT                    0x80A7
#define GL_SAMPLE_BUFFERS_EXT             0x80A8
#define GL_SAMPLES_EXT                    0x80A9
#define GL_SAMPLE_MASK_VALUE_EXT          0x80AA
#define GL_SAMPLE_MASK_INVERT_EXT         0x80AB
#define GL_SAMPLE_PATTERN_EXT             0x80AC
#endif

#if GL_EXT_stencil_two_side
#define GL_STENCIL_TEST_TWO_SIDE_EXT      0x8910
#define GL_ACTIVE_STENCIL_FACE_EXT        0x8911
#endif

#if GL_EXT_depth_bounds_test
#define GL_DEPTH_BOUNDS_TEST_EXT          0x8890
#define GL_DEPTH_BOUNDS_EXT               0x8891
#endif

#if GL_EXT_blend_equation_separate
#define GL_BLEND_EQUATION_RGB_EXT         0x8009
#define GL_BLEND_EQUATION_ALPHA_EXT       0x883D
#endif

#if GL_EXT_texture_mirror_clamp
#define GL_MIRROR_CLAMP_EXT               0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_EXT       0x8743
#define GL_MIRROR_CLAMP_TO_BORDER_EXT     0x8912
#endif

#if GL_EXT_framebuffer_object
#define GL_FRAMEBUFFER_EXT                 0x8D40
#define GL_RENDERBUFFER_EXT                0x8D41
#define GL_STENCIL_INDEX1_EXT              0x8D46
#define GL_STENCIL_INDEX4_EXT              0x8D47
#define GL_STENCIL_INDEX8_EXT              0x8D48
#define GL_STENCIL_INDEX16_EXT             0x8D49
#define GL_RENDERBUFFER_WIDTH_EXT           0x8D42
#define GL_RENDERBUFFER_HEIGHT_EXT          0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT 0x8D44
#define GL_RENDERBUFFER_RED_SIZE_EXT        0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE_EXT      0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE_EXT       0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE_EXT      0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE_EXT      0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE_EXT    0x8D55
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT           0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT           0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT         0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT    0x8CD4
#define GL_COLOR_ATTACHMENT0_EXT           0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT           0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT           0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT           0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT           0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT           0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT           0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT           0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT           0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT           0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT          0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT          0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT          0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT          0x8CED
#define GL_COLOR_ATTACHMENT14_EXT          0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT          0x8CEF
#define GL_DEPTH_ATTACHMENT_EXT            0x8D00
#define GL_STENCIL_ATTACHMENT_EXT          0x8D20
#define GL_FRAMEBUFFER_COMPLETE_EXT                        0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT           0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT   0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT           0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT              0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT          0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT          0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT                     0x8CDD
#define GL_FRAMEBUFFER_BINDING_EXT         0x8CA6
#define GL_RENDERBUFFER_BINDING_EXT        0x8CA7
#define GL_MAX_COLOR_ATTACHMENTS_EXT       0x8CDF
#define GL_MAX_RENDERBUFFER_SIZE_EXT       0x84E8
#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT 0x0506
#endif

#if GL_EXT_framebuffer_blit
#define GL_READ_FRAMEBUFFER_EXT                 0x8CA8
#define GL_DRAW_FRAMEBUFFER_EXT                 0x8CA9
#define GL_DRAW_FRAMEBUFFER_BINDING_EXT         0x8CA6
#define GL_READ_FRAMEBUFFER_BINDING_EXT         0x8CAA
#endif

#if GL_EXT_framebuffer_multisample
#define GL_RENDERBUFFER_SAMPLES_EXT               0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT 0x8D56
#define GL_MAX_SAMPLES_EXT                        0x8D57
#endif

#if GL_ARB_framebuffer_object
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT            0x8218
#define GL_FRAMEBUFFER_UNDEFINED          0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_MAX_RENDERBUFFER_SIZE          0x84E8
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_TEXTURE_STENCIL_SIZE           0x88F1
#define GL_TEXTURE_RED_TYPE               0x8C10
#define GL_TEXTURE_GREEN_TYPE             0x8C11
#define GL_TEXTURE_BLUE_TYPE              0x8C12
#define GL_TEXTURE_ALPHA_TYPE             0x8C13
#define GL_TEXTURE_DEPTH_TYPE             0x8C16
#define GL_UNSIGNED_NORMALIZED            0x8C17
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING       GL_FRAMEBUFFER_BINDING
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING       0x8CAA
#define GL_RENDERBUFFER_SAMPLES           0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS          0x8CDF
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
#define GL_COLOR_ATTACHMENT4              0x8CE4
#define GL_COLOR_ATTACHMENT5              0x8CE5
#define GL_COLOR_ATTACHMENT6              0x8CE6
#define GL_COLOR_ATTACHMENT7              0x8CE7
#define GL_COLOR_ATTACHMENT8              0x8CE8
#define GL_COLOR_ATTACHMENT9              0x8CE9
#define GL_COLOR_ATTACHMENT10             0x8CEA
#define GL_COLOR_ATTACHMENT11             0x8CEB
#define GL_COLOR_ATTACHMENT12             0x8CEC
#define GL_COLOR_ATTACHMENT13             0x8CED
#define GL_COLOR_ATTACHMENT14             0x8CEE
#define GL_COLOR_ATTACHMENT15             0x8CEF
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_RENDERBUFFER_WIDTH             0x8D42
#define GL_RENDERBUFFER_HEIGHT            0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   0x8D44
#define GL_STENCIL_INDEX1                 0x8D46
#define GL_STENCIL_INDEX4                 0x8D47
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_STENCIL_INDEX16                0x8D49
#define GL_RENDERBUFFER_RED_SIZE          0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE        0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE         0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE        0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE        0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE      0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES                    0x8D57
#endif
    
    
#if GL_EXT_packed_depth_stencil
#define GL_DEPTH_STENCIL_EXT                    0x84F9
#define GL_UNSIGNED_INT_24_8_EXT                0x84FA
#define GL_DEPTH24_STENCIL8_EXT                 0x88F0
#define GL_TEXTURE_STENCIL_SIZE_EXT             0x88F1
#endif

#if GL_EXT_geometry_shader4
#define GL_GEOMETRY_SHADER_EXT                      0x8DD9
#define GL_GEOMETRY_VERTICES_OUT_EXT                0x8DDA
#define GL_GEOMETRY_INPUT_TYPE_EXT                  0x8DDB
#define GL_GEOMETRY_OUTPUT_TYPE_EXT                 0x8DDC
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT     0x8C29
#define GL_MAX_GEOMETRY_VARYING_COMPONENTS_EXT      0x8DDD
#define GL_MAX_VERTEX_VARYING_COMPONENTS_EXT        0x8DDE
#define GL_MAX_VARYING_COMPONENTS_EXT               0x8B4B
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT      0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT         0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT 0x8DE1
#define GL_LINES_ADJACENCY_EXT                      0xA
#define GL_LINE_STRIP_ADJACENCY_EXT                 0xB
#define GL_TRIANGLES_ADJACENCY_EXT                  0xC
#define GL_TRIANGLE_STRIP_ADJACENCY_EXT             0xD
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT 0x8DA8
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_EXT   0x8DA9
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT       0x8DA7
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT 0x8CD4
#define GL_PROGRAM_POINT_SIZE_EXT                   0x8642
#endif

#if GL_EXT_transform_feedback
#define GL_TRANSFORM_FEEDBACK_BUFFER_EXT                      0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_START_EXT                0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_EXT                 0x8C85
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_EXT              0x8C8F
#define GL_INTERLEAVED_ATTRIBS_EXT                            0x8C8C
#define GL_SEPARATE_ATTRIBS_EXT                               0x8C8D
#define GL_PRIMITIVES_GENERATED_EXT                           0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_EXT          0x8C88
#define GL_RASTERIZER_DISCARD_EXT                             0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_EXT  0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_EXT        0x8C8B
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_EXT     0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS_EXT                    0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE_EXT                 0x8C7F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH_EXT          0x8C76
#endif

#if GL_EXT_bindable_uniform
#define GL_MAX_VERTEX_BINDABLE_UNIFORMS_EXT                0x8DE2
#define GL_MAX_FRAGMENT_BINDABLE_UNIFORMS_EXT              0x8DE3
#define GL_MAX_GEOMETRY_BINDABLE_UNIFORMS_EXT              0x8DE4
#define GL_MAX_BINDABLE_UNIFORM_SIZE_EXT                   0x8DED
#define GL_UNIFORM_BUFFER_BINDING_EXT                      0x8DEF
#define GL_UNIFORM_BUFFER_EXT                              0x8DEE
#endif

#if GL_EXT_texture_integer
#define GL_RGBA_INTEGER_MODE_EXT                           0x8D9E
#define GL_RGBA32UI_EXT                                    0x8D70
#define GL_RGB32UI_EXT                                     0x8D71
#define GL_ALPHA32UI_EXT                                   0x8D72
#define GL_INTENSITY32UI_EXT                               0x8D73
#define GL_LUMINANCE32UI_EXT                               0x8D74
#define GL_LUMINANCE_ALPHA32UI_EXT                         0x8D75
#define GL_RGBA16UI_EXT                                    0x8D76
#define GL_RGB16UI_EXT                                     0x8D77
#define GL_ALPHA16UI_EXT                                   0x8D78
#define GL_INTENSITY16UI_EXT                               0x8D79
#define GL_LUMINANCE16UI_EXT                               0x8D7A
#define GL_LUMINANCE_ALPHA16UI_EXT                         0x8D7B
#define GL_RGBA8UI_EXT                                     0x8D7C
#define GL_RGB8UI_EXT                                      0x8D7D
#define GL_ALPHA8UI_EXT                                    0x8D7E
#define GL_INTENSITY8UI_EXT                                0x8D7F
#define GL_LUMINANCE8UI_EXT                                0x8D80
#define GL_LUMINANCE_ALPHA8UI_EXT                          0x8D81
#define GL_RGBA32I_EXT                                     0x8D82
#define GL_RGB32I_EXT                                      0x8D83
#define GL_ALPHA32I_EXT                                    0x8D84
#define GL_INTENSITY32I_EXT                                0x8D85
#define GL_LUMINANCE32I_EXT                                0x8D86
#define GL_LUMINANCE_ALPHA32I_EXT                          0x8D87
#define GL_RGBA16I_EXT                                     0x8D88
#define GL_RGB16I_EXT                                      0x8D89
#define GL_ALPHA16I_EXT                                    0x8D8A
#define GL_INTENSITY16I_EXT                                0x8D8B
#define GL_LUMINANCE16I_EXT                                0x8D8C
#define GL_LUMINANCE_ALPHA16I_EXT                          0x8D8D
#define GL_RGBA8I_EXT                                      0x8D8E
#define GL_RGB8I_EXT                                       0x8D8F
#define GL_ALPHA8I_EXT                                     0x8D90
#define GL_INTENSITY8I_EXT                                 0x8D91
#define GL_LUMINANCE8I_EXT                                 0x8D92
#define GL_LUMINANCE_ALPHA8I_EXT                           0x8D93
#define GL_RED_INTEGER_EXT                                 0x8D94
#define GL_GREEN_INTEGER_EXT                               0x8D95
#define GL_BLUE_INTEGER_EXT                                0x8D96
#define GL_ALPHA_INTEGER_EXT                               0x8D97
#define GL_RGB_INTEGER_EXT                                 0x8D98
#define GL_RGBA_INTEGER_EXT                                0x8D99
#define GL_BGR_INTEGER_EXT                                 0x8D9A
#define GL_BGRA_INTEGER_EXT                                0x8D9B
#define GL_LUMINANCE_INTEGER_EXT                           0x8D9C
#define GL_LUMINANCE_ALPHA_INTEGER_EXT                     0x8D9D
#endif

#if GL_EXT_texture_array
#define GL_TEXTURE_1D_ARRAY_EXT                            0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY_EXT                      0x8C19
#define GL_TEXTURE_2D_ARRAY_EXT                            0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY_EXT                      0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY_EXT                    0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY_EXT                    0x8C1D
#define GL_MAX_ARRAY_TEXTURE_LAYERS_EXT                    0x88FF
#define GL_COMPARE_REF_DEPTH_TO_TEXTURE_EXT                0x884E
/* reuse GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT */
/* reuse GL_SAMPLER_1D_ARRAY_EXT */
/* reuse GL_SAMPLER_2D_ARRAY_EXT */
/* reuse GL_SAMPLER_1D_ARRAY_SHADOW_EXT */
/* reuse GL_SAMPLER_2D_ARRAY_SHADOW_EXT */
#endif

#if GL_EXT_gpu_shader4
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER_EXT                    0x88FD
#define GL_SAMPLER_1D_ARRAY_EXT                               0x8DC0
#define GL_SAMPLER_2D_ARRAY_EXT                               0x8DC1
#define GL_SAMPLER_BUFFER_EXT                                 0x8DC2
#define GL_SAMPLER_1D_ARRAY_SHADOW_EXT                        0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW_EXT                        0x8DC4
#define GL_SAMPLER_CUBE_SHADOW_EXT                            0x8DC5
#define GL_UNSIGNED_INT_VEC2_EXT                              0x8DC6
#define GL_UNSIGNED_INT_VEC3_EXT                              0x8DC7
#define GL_UNSIGNED_INT_VEC4_EXT                              0x8DC8
#define GL_INT_SAMPLER_1D_EXT                                 0x8DC9
#define GL_INT_SAMPLER_2D_EXT                                 0x8DCA
#define GL_INT_SAMPLER_3D_EXT                                 0x8DCB
#define GL_INT_SAMPLER_CUBE_EXT                               0x8DCC
#define GL_INT_SAMPLER_2D_RECT_EXT                            0x8DCD
#define GL_INT_SAMPLER_1D_ARRAY_EXT                           0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY_EXT                           0x8DCF
#define GL_INT_SAMPLER_BUFFER_EXT                             0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_1D_EXT                        0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D_EXT                        0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D_EXT                        0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE_EXT                      0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT_EXT                   0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY_EXT                  0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY_EXT                  0x8DD7
#define GL_UNSIGNED_INT_SAMPLER_BUFFER_EXT                    0x8DD8
#define GL_MIN_PROGRAM_TEXEL_OFFSET_EXT                       0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET_EXT                       0x8905
#endif
	
#if GL_EXT_provoking_vertex
#define GL_FIRST_VERTEX_CONVENTION_EXT                   0x8E4D
#define GL_LAST_VERTEX_CONVENTION_EXT                    0x8E4E
#define GL_PROVOKING_VERTEX_EXT                          0x8E4F
#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION_EXT  0x8E4C
#endif
	
#if GL_EXT_vertex_array_bgra
#define GL_BGRA                                 0x80E1
#endif

#if GL_EXT_framebuffer_sRGB
#define GL_FRAMEBUFFER_SRGB_EXT                 0x8DB9
#define GL_FRAMEBUFFER_SRGB_CAPABLE_EXT         0x8DBA
#endif
	
#if GL_EXT_packed_float
#define GL_R11F_G11F_B10F_EXT                      0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV_EXT        0x8C3B
#define GL_RGBA_SIGNED_COMPONENTS_EXT              0x8C3C
#endif
	
#if GL_EXT_texture_shared_exponent
#define GL_RGB9_E5_EXT                          0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV_EXT         0x8C3E
#define GL_TEXTURE_SHARED_SIZE_EXT              0x8C3F
#endif

#if GL_APPLE_vertex_array_range
#define GL_VERTEX_ARRAY_RANGE_APPLE             0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE      0x851E
#define GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_APPLE 0x8520
#define GL_VERTEX_ARRAY_RANGE_POINTER_APPLE     0x8521
#define GL_VERTEX_ARRAY_STORAGE_HINT_APPLE      0x851F
#define GL_STORAGE_CLIENT_APPLE                 0x85B4
#define GL_STORAGE_PRIVATE_APPLE                0x85BD
#define GL_STORAGE_CACHED_APPLE                 0x85BE
#define GL_STORAGE_SHARED_APPLE                 0x85BF
#endif

#if GL_APPLE_vertex_array_object
#define GL_VERTEX_ARRAY_BINDING_APPLE      0x85B5
#endif

#if GL_APPLE_element_array
#define GL_ELEMENT_ARRAY_APPLE             0x8A0C
#define GL_ELEMENT_ARRAY_TYPE_APPLE        0x8A0D
#define GL_ELEMENT_ARRAY_POINTER_APPLE     0x8A0E
#endif

#if GL_APPLE_fence
#define GL_DRAW_PIXELS_APPLE              0x8A0A
#define GL_FENCE_APPLE                    0x8A0B
#define GL_BUFFER_OBJECT_APPLE            0x85B3
#endif

#if GL_APPLE_specular_vector
#define GL_LIGHT_MODEL_SPECULAR_VECTOR_APPLE 0x85B0
#endif

#if GL_APPLE_transform_hint
#define GL_TRANSFORM_HINT_APPLE           0x85B1
#endif

#if GL_APPLE_client_storage
#define GL_UNPACK_CLIENT_STORAGE_APPLE    0x85B2
#endif

#if GL_APPLE_ycbcr_422
#define GL_YCBCR_422_APPLE                 0x85B9
#endif

#if GL_APPLE_rgb_422
#define GL_RGB_422_APPLE                   0x8A1F
#endif
	
#if GL_APPLE_ycbcr_422 || GL_APPLE_rgb_422
#define GL_UNSIGNED_SHORT_8_8_APPLE        0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_APPLE    0x85BB
#endif

#if GL_APPLE_texture_range
#define GL_TEXTURE_RANGE_LENGTH_APPLE      0x85B7
#define GL_TEXTURE_RANGE_POINTER_APPLE     0x85B8
#define GL_TEXTURE_STORAGE_HINT_APPLE      0x85BC
#define GL_TEXTURE_MINIMIZE_STORAGE_APPLE  0x85B6
#define GL_STORAGE_PRIVATE_APPLE           0x85BD
#define GL_STORAGE_CACHED_APPLE            0x85BE
#define GL_STORAGE_SHARED_APPLE            0x85BF
#endif

#if GL_APPLE_float_pixels
#define GL_HALF_APPLE                      0x140B
#define GL_COLOR_FLOAT_APPLE               0x8A0F
#define GL_RGBA_FLOAT32_APPLE              0x8814
#define GL_RGB_FLOAT32_APPLE               0x8815
#define GL_ALPHA_FLOAT32_APPLE             0x8816
#define GL_INTENSITY_FLOAT32_APPLE         0x8817
#define GL_LUMINANCE_FLOAT32_APPLE         0x8818
#define GL_LUMINANCE_ALPHA_FLOAT32_APPLE   0x8819
#define GL_RGBA_FLOAT16_APPLE              0x881A
#define GL_RGB_FLOAT16_APPLE               0x881B
#define GL_ALPHA_FLOAT16_APPLE             0x881C
#define GL_INTENSITY_FLOAT16_APPLE         0x881D
#define GL_LUMINANCE_FLOAT16_APPLE         0x881E
#define GL_LUMINANCE_ALPHA_FLOAT16_APPLE   0x881F
#endif

#if GL_APPLE_pixel_buffer
#define GL_MIN_PBUFFER_VIEWPORT_DIMS_APPLE 0x8A10
#endif

#if GL_APPLE_vertex_program_evaluators
#define GL_VERTEX_ATTRIB_MAP1_APPLE                      0x8A00
#define GL_VERTEX_ATTRIB_MAP2_APPLE                      0x8A01
#define GL_VERTEX_ATTRIB_MAP1_SIZE_APPLE                 0x8A02
#define GL_VERTEX_ATTRIB_MAP1_COEFF_APPLE                0x8A03
#define GL_VERTEX_ATTRIB_MAP1_ORDER_APPLE                0x8A04
#define GL_VERTEX_ATTRIB_MAP1_DOMAIN_APPLE               0x8A05
#define GL_VERTEX_ATTRIB_MAP2_SIZE_APPLE                 0x8A06
#define GL_VERTEX_ATTRIB_MAP2_COEFF_APPLE                0x8A07
#define GL_VERTEX_ATTRIB_MAP2_ORDER_APPLE                0x8A08
#define GL_VERTEX_ATTRIB_MAP2_DOMAIN_APPLE               0x8A09
#endif

#if GL_APPLE_flush_buffer_range
#define GL_BUFFER_SERIALIZED_MODIFY_APPLE 0x8A12
#define GL_BUFFER_FLUSHING_UNMAP_APPLE    0x8A13
#endif

#if GL_APPLE_aux_depth_stencil
#define GL_AUX_DEPTH_STENCIL_APPLE        0x8A14
#endif

#if GL_APPLE_row_bytes
#define GL_PACK_ROW_BYTES_APPLE           0x8A15
#define GL_UNPACK_ROW_BYTES_APPLE         0x8A16
#define GL_PACK_IMAGE_BYTES_APPLE         0x8A17
#define GL_UNPACK_IMAGE_BYTES_APPLE       0x8A18
#endif

#if GL_APPLE_object_purgeable
#define GL_RELEASED_APPLE                 0x8A19
#define GL_VOLATILE_APPLE                 0x8A1A
#define GL_RETAINED_APPLE                 0x8A1B
#define GL_UNDEFINED_APPLE                0x8A1C
#define GL_PURGEABLE_APPLE                0x8A1D
#endif

#if GL_APPLE_vertex_point_size
#define GL_VERTEX_POINT_SIZE_APPLE               0x8A26
#define GL_CURRENT_POINT_SIZE_APPLE              0x8A27
#define GL_POINT_SIZE_ARRAY_APPLE                0x8B9C
#define GL_POINT_SIZE_ARRAY_TYPE_APPLE           0x898A
#define GL_POINT_SIZE_ARRAY_STRIDE_APPLE         0x898B
#define GL_POINT_SIZE_ARRAY_POINTER_APPLE        0x898C
#define GL_POINT_SIZE_ARRAY_BUFFER_BINDING_APPLE 0x8B9F
#endif
	
#if GL_ATI_blend_weighted_minmax
#define GL_MIN_WEIGHTED_ATI               0x877D
#define GL_MAX_WEIGHTED_ATI               0x877E
#endif

#if GL_ATI_texture_env_combine3
#define GL_MODULATE_ADD_ATI               0x8744
#define GL_MODULATE_SIGNED_ADD_ATI        0x8745
#define GL_MODULATE_SUBTRACT_ATI          0x8746
#endif

#if GL_ATI_separate_stencil
#define GL_STENCIL_BACK_FUNC_ATI            0x8800
#define GL_STENCIL_BACK_FAIL_ATI            0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI 0x8803
#endif

#if GL_ATI_array_rev_comps_in_4_bytes
#define GL_ARRAY_REV_COMPS_IN_4_BYTES_ATI 0x897C
#endif

#if GL_ATI_texture_mirror_once
#define GL_MIRROR_CLAMP_ATI                             0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_ATI                     0x8743
#endif

#if GL_ATI_pn_triangles
#define GL_PN_TRIANGLES_ATI                             0x6090
#define GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATI       0x6091
#define GL_PN_TRIANGLES_POINT_MODE_ATI                  0x6092
#define GL_PN_TRIANGLES_NORMAL_MODE_ATI                 0x6093
#define GL_PN_TRIANGLES_TESSELATION_LEVEL_ATI           0x6094
#define GL_PN_TRIANGLES_POINT_MODE_LINEAR_ATI           0x6095
#define GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATI            0x6096
#define GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATI          0x6097
#define GL_PN_TRIANGLES_NORMAL_MODE_QUADRATIC_ATI       0x6098
#endif

#if GL_ATI_text_fragment_shader
#define GL_TEXT_FRAGMENT_SHADER_ATI                     0x8200
#endif

#if GL_ATI_blend_equation_separate
#define GL_ALPHA_BLEND_EQUATION_ATI                     0x883D
#endif

#if GL_ATI_point_cull_mode
#define GL_POINT_CULL_MODE_ATI                           0x60B3
#define GL_POINT_CULL_CENTER_ATI                         0x60B4
#define GL_POINT_CULL_CLIP_ATI                           0x60B5
#endif

#if GL_ATIX_pn_triangles
#define GL_PN_TRIANGLES_ATIX                            0x6090
#define GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATIX      0x6091
#define GL_PN_TRIANGLES_POINT_MODE_ATIX                 0x6092
#define GL_PN_TRIANGLES_NORMAL_MODE_ATIX                0x6093
#define GL_PN_TRIANGLES_TESSELATION_LEVEL_ATIX          0x6094
#define GL_PN_TRIANGLES_POINT_MODE_LINEAR_ATIX          0x6095
#define GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATIX           0x6096
#define GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATIX         0x6097
#define GL_PN_TRIANGLES_NORMAL_MODE_QUADRATIC_ATIX      0x6098
#endif

#if GL_ATI_texture_compression_3dc
#define GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI           0x8837
#endif

#if GL_ARB_texture_compression_rgtc
#define GL_COMPRESSED_RED_RGTC1                       0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1                0x8DBC
#define GL_COMPRESSED_RG_RGTC2                        0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2                 0x8DBE
#endif

#if GL_ATI_texture_float
#define GL_RGBA_FLOAT32_ATI                             0x8814
#define GL_RGB_FLOAT32_ATI                              0x8815
#define GL_ALPHA_FLOAT32_ATI                            0x8816
#define GL_INTENSITY_FLOAT32_ATI                        0x8817
#define GL_LUMINANCE_FLOAT32_ATI                        0x8818
#define GL_LUMINANCE_ALPHA_FLOAT32_ATI                  0x8819
#define GL_RGBA_FLOAT16_ATI                             0x881A
#define GL_RGB_FLOAT16_ATI                              0x881B
#define GL_ALPHA_FLOAT16_ATI                            0x881C
#define GL_INTENSITY_FLOAT16_ATI                        0x881D
#define GL_LUMINANCE_FLOAT16_ATI                        0x881E
#define GL_LUMINANCE_ALPHA_FLOAT16_ATI                  0x881F
#endif


#if GL_HP_image_transform
#define GL_IMAGE_SCALE_X_HP               0x8155
#define GL_IMAGE_SCALE_Y_HP               0x8156
#define GL_IMAGE_TRANSLATE_X_HP           0x8157
#define GL_IMAGE_TRANSLATE_Y_HP           0x8158
#define GL_IMAGE_ROTATE_ANGLE_HP          0x8159
#define GL_IMAGE_ROTATE_ORIGIN_X_HP       0x815A
#define GL_IMAGE_ROTATE_ORIGIN_Y_HP       0x815B
#define GL_IMAGE_MAG_FILTER_HP            0x815C
#define GL_IMAGE_MIN_FILTER_HP            0x815D
#define GL_IMAGE_CUBIC_WEIGHT_HP          0x815E
#define GL_CUBIC_HP                       0x815F
#define GL_AVERAGE_HP                     0x8160
#define GL_IMAGE_TRANSFORM_2D_HP          0x8161
#define GL_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP 0x8162
#define GL_PROXY_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP 0x8163
#endif

#if GL_HP_convolution_border_modes
#define GL_IGNORE_BORDER_HP               0x8150
#define GL_CONSTANT_BORDER_HP             0x8151
#define GL_REPLICATE_BORDER_HP            0x8153
#define GL_CONVOLUTION_BORDER_COLOR_HP    0x8154
#endif

#if GL_HP_texture_lighting
#define GL_TEXTURE_LIGHTING_MODE_HP       0x8167
#define GL_TEXTURE_POST_SPECULAR_HP       0x8168
#define GL_TEXTURE_PRE_SPECULAR_HP        0x8169
#endif

#if GL_HP_occlusion_test
#define GL_OCCLUSION_TEST_HP              0x8165
#define GL_OCCLUSION_TEST_RESULT_HP       0x8166
#endif


#if GL_IBM_rasterpos_clip
#define GL_RASTER_POSITION_UNCLIPPED_IBM  0x19262
#endif

#if GL_IBM_cull_vertex
#define GL_CULL_VERTEX_IBM                103050
#endif

#if GL_IBM_vertex_array_lists
#define GL_VERTEX_ARRAY_LIST_IBM          103070
#define GL_NORMAL_ARRAY_LIST_IBM          103071
#define GL_COLOR_ARRAY_LIST_IBM           103072
#define GL_INDEX_ARRAY_LIST_IBM           103073
#define GL_TEXTURE_COORD_ARRAY_LIST_IBM   103074
#define GL_EDGE_FLAG_ARRAY_LIST_IBM       103075
#define GL_FOG_COORDINATE_ARRAY_LIST_IBM  103076
#define GL_SECONDARY_COLOR_ARRAY_LIST_IBM 103077
#define GL_VERTEX_ARRAY_LIST_STRIDE_IBM   103080
#define GL_NORMAL_ARRAY_LIST_STRIDE_IBM   103081
#define GL_COLOR_ARRAY_LIST_STRIDE_IBM    103082
#define GL_INDEX_ARRAY_LIST_STRIDE_IBM    103083
#define GL_TEXTURE_COORD_ARRAY_LIST_STRIDE_IBM 103084
#define GL_EDGE_FLAG_ARRAY_LIST_STRIDE_IBM 103085
#define GL_FOG_COORDINATE_ARRAY_LIST_STRIDE_IBM 103086
#define GL_SECONDARY_COLOR_ARRAY_LIST_STRIDE_IBM 103087
#endif


#if GL_INGR_color_clamp
#define GL_RED_MIN_CLAMP_INGR             0x8560
#define GL_GREEN_MIN_CLAMP_INGR           0x8561
#define GL_BLUE_MIN_CLAMP_INGR            0x8562
#define GL_ALPHA_MIN_CLAMP_INGR           0x8563
#define GL_RED_MAX_CLAMP_INGR             0x8564
#define GL_GREEN_MAX_CLAMP_INGR           0x8565
#define GL_BLUE_MAX_CLAMP_INGR            0x8566
#define GL_ALPHA_MAX_CLAMP_INGR           0x8567
#endif

#if GL_INGR_interlace_read
#define GL_INTERLACE_READ_INGR            0x8568
#endif


#if GL_INTEL_parallel_arrays
#define GL_PARALLEL_ARRAYS_INTEL          0x83F4
#define GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL 0x83F5
#define GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL 0x83F6
#define GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL 0x83F7
#define GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL 0x83F8
#endif


#if GL_NV_texgen_reflection
#define GL_NORMAL_MAP_NV                  0x8511
#define GL_REFLECTION_MAP_NV              0x8512
#endif

#if GL_NV_light_max_exponent
#define GL_MAX_SHININESS_NV               0x8504
#define GL_MAX_SPOT_EXPONENT_NV           0x8505
#endif

#if GL_NV_vertex_array_range
#define GL_VERTEX_ARRAY_RANGE_NV          0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_NV   0x851E
#define GL_VERTEX_ARRAY_RANGE_VALID_NV    0x851F
#define GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV 0x8520
#define GL_VERTEX_ARRAY_RANGE_POINTER_NV  0x8521
#endif

#if GL_NV_vertex_array_range2
#define GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV 0x8533
#endif

#if GL_NV_register_combiners
#define GL_REGISTER_COMBINERS_NV          0x8522
#define GL_VARIABLE_A_NV                  0x8523
#define GL_VARIABLE_B_NV                  0x8524
#define GL_VARIABLE_C_NV                  0x8525
#define GL_VARIABLE_D_NV                  0x8526
#define GL_VARIABLE_E_NV                  0x8527
#define GL_VARIABLE_F_NV                  0x8528
#define GL_VARIABLE_G_NV                  0x8529
#define GL_CONSTANT_COLOR0_NV             0x852A
#define GL_CONSTANT_COLOR1_NV             0x852B
#define GL_PRIMARY_COLOR_NV               0x852C
#define GL_SECONDARY_COLOR_NV             0x852D
#define GL_SPARE0_NV                      0x852E
#define GL_SPARE1_NV                      0x852F
#define GL_DISCARD_NV                     0x8530
#define GL_E_TIMES_F_NV                   0x8531
#define GL_SPARE0_PLUS_SECONDARY_COLOR_NV 0x8532
#define GL_UNSIGNED_IDENTITY_NV           0x8536
#define GL_UNSIGNED_INVERT_NV             0x8537
#define GL_EXPAND_NORMAL_NV               0x8538
#define GL_EXPAND_NEGATE_NV               0x8539
#define GL_HALF_BIAS_NORMAL_NV            0x853A
#define GL_HALF_BIAS_NEGATE_NV            0x853B
#define GL_SIGNED_IDENTITY_NV             0x853C
#define GL_SIGNED_NEGATE_NV               0x853D
#define GL_SCALE_BY_TWO_NV                0x853E
#define GL_SCALE_BY_FOUR_NV               0x853F
#define GL_SCALE_BY_ONE_HALF_NV           0x8540
#define GL_BIAS_BY_NEGATIVE_ONE_HALF_NV   0x8541
#define GL_COMBINER_INPUT_NV              0x8542
#define GL_COMBINER_MAPPING_NV            0x8543
#define GL_COMBINER_COMPONENT_USAGE_NV    0x8544
#define GL_COMBINER_AB_DOT_PRODUCT_NV     0x8545
#define GL_COMBINER_CD_DOT_PRODUCT_NV     0x8546
#define GL_COMBINER_MUX_SUM_NV            0x8547
#define GL_COMBINER_SCALE_NV              0x8548
#define GL_COMBINER_BIAS_NV               0x8549
#define GL_COMBINER_AB_OUTPUT_NV          0x854A
#define GL_COMBINER_CD_OUTPUT_NV          0x854B
#define GL_COMBINER_SUM_OUTPUT_NV         0x854C
#define GL_MAX_GENERAL_COMBINERS_NV       0x854D
#define GL_NUM_GENERAL_COMBINERS_NV       0x854E
#define GL_COLOR_SUM_CLAMP_NV             0x854F
#define GL_COMBINER0_NV                   0x8550
#define GL_COMBINER1_NV                   0x8551
#define GL_COMBINER2_NV                   0x8552
#define GL_COMBINER3_NV                   0x8553
#define GL_COMBINER4_NV                   0x8554
#define GL_COMBINER5_NV                   0x8555
#define GL_COMBINER6_NV                   0x8556
#define GL_COMBINER7_NV                   0x8557
/* reuse GL_TEXTURE0_ARB */
/* reuse GL_TEXTURE1_ARB */
/* reuse GL_ZERO */
/* reuse GL_NONE */
/* reuse GL_FOG */
#endif

#if GL_NV_register_combiners2
#define GL_PER_STAGE_CONSTANTS_NV         0x8535
#endif

#if GL_NV_fog_distance
#define GL_FOG_DISTANCE_MODE_NV           0x855A
#define GL_EYE_RADIAL_NV                  0x855B
#define GL_EYE_PLANE_ABSOLUTE_NV          0x855C
/* reuse GL_EYE_PLANE */
#endif

#if GL_NV_texgen_emboss
#define GL_EMBOSS_LIGHT_NV                0x855D
#define GL_EMBOSS_CONSTANT_NV             0x855E
#define GL_EMBOSS_MAP_NV                  0x855F
#endif

#if GL_NV_vertex_program
#define GL_VERTEX_PROGRAM_NV              0x8620
#define GL_VERTEX_STATE_PROGRAM_NV        0x8621
#define GL_ATTRIB_ARRAY_SIZE_NV           0x8623
#define GL_ATTRIB_ARRAY_STRIDE_NV         0x8624
#define GL_ATTRIB_ARRAY_TYPE_NV           0x8625
#define GL_CURRENT_ATTRIB_NV              0x8626
#define GL_PROGRAM_LENGTH_NV              0x8627
#define GL_PROGRAM_STRING_NV              0x8628
#define GL_MODELVIEW_PROJECTION_NV        0x8629
#define GL_IDENTITY_NV                    0x862A
#define GL_INVERSE_NV                     0x862B
#define GL_TRANSPOSE_NV                   0x862C
#define GL_INVERSE_TRANSPOSE_NV           0x862D
#define GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV 0x862E
#define GL_MAX_TRACK_MATRICES_NV          0x862F
#define GL_MATRIX0_NV                     0x8630
#define GL_MATRIX1_NV                     0x8631
#define GL_MATRIX2_NV                     0x8632
#define GL_MATRIX3_NV                     0x8633
#define GL_MATRIX4_NV                     0x8634
#define GL_MATRIX5_NV                     0x8635
#define GL_MATRIX6_NV                     0x8636
#define GL_MATRIX7_NV                     0x8637
#define GL_CURRENT_MATRIX_STACK_DEPTH_NV  0x8640
#define GL_CURRENT_MATRIX_NV              0x8641
#define GL_VERTEX_PROGRAM_POINT_SIZE_NV   0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_NV     0x8643
#define GL_PROGRAM_PARAMETER_NV           0x8644
#define GL_ATTRIB_ARRAY_POINTER_NV        0x8645
#define GL_PROGRAM_TARGET_NV              0x8646
#define GL_PROGRAM_RESIDENT_NV            0x8647
#define GL_TRACK_MATRIX_NV                0x8648
#define GL_TRACK_MATRIX_TRANSFORM_NV      0x8649
#define GL_VERTEX_PROGRAM_BINDING_NV      0x864A
#define GL_PROGRAM_ERROR_POSITION_NV      0x864B
#define GL_VERTEX_ATTRIB_ARRAY0_NV        0x8650
#define GL_VERTEX_ATTRIB_ARRAY1_NV        0x8651
#define GL_VERTEX_ATTRIB_ARRAY2_NV        0x8652
#define GL_VERTEX_ATTRIB_ARRAY3_NV        0x8653
#define GL_VERTEX_ATTRIB_ARRAY4_NV        0x8654
#define GL_VERTEX_ATTRIB_ARRAY5_NV        0x8655
#define GL_VERTEX_ATTRIB_ARRAY6_NV        0x8656
#define GL_VERTEX_ATTRIB_ARRAY7_NV        0x8657
#define GL_VERTEX_ATTRIB_ARRAY8_NV        0x8658
#define GL_VERTEX_ATTRIB_ARRAY9_NV        0x8659
#define GL_VERTEX_ATTRIB_ARRAY10_NV       0x865A
#define GL_VERTEX_ATTRIB_ARRAY11_NV       0x865B
#define GL_VERTEX_ATTRIB_ARRAY12_NV       0x865C
#define GL_VERTEX_ATTRIB_ARRAY13_NV       0x865D
#define GL_VERTEX_ATTRIB_ARRAY14_NV       0x865E
#define GL_VERTEX_ATTRIB_ARRAY15_NV       0x865F
#define GL_MAP1_VERTEX_ATTRIB0_4_NV       0x8660
#define GL_MAP1_VERTEX_ATTRIB1_4_NV       0x8661
#define GL_MAP1_VERTEX_ATTRIB2_4_NV       0x8662
#define GL_MAP1_VERTEX_ATTRIB3_4_NV       0x8663
#define GL_MAP1_VERTEX_ATTRIB4_4_NV       0x8664
#define GL_MAP1_VERTEX_ATTRIB5_4_NV       0x8665
#define GL_MAP1_VERTEX_ATTRIB6_4_NV       0x8666
#define GL_MAP1_VERTEX_ATTRIB7_4_NV       0x8667
#define GL_MAP1_VERTEX_ATTRIB8_4_NV       0x8668
#define GL_MAP1_VERTEX_ATTRIB9_4_NV       0x8669
#define GL_MAP1_VERTEX_ATTRIB10_4_NV      0x866A
#define GL_MAP1_VERTEX_ATTRIB11_4_NV      0x866B
#define GL_MAP1_VERTEX_ATTRIB12_4_NV      0x866C
#define GL_MAP1_VERTEX_ATTRIB13_4_NV      0x866D
#define GL_MAP1_VERTEX_ATTRIB14_4_NV      0x866E
#define GL_MAP1_VERTEX_ATTRIB15_4_NV      0x866F
#define GL_MAP2_VERTEX_ATTRIB0_4_NV       0x8670
#define GL_MAP2_VERTEX_ATTRIB1_4_NV       0x8671
#define GL_MAP2_VERTEX_ATTRIB2_4_NV       0x8672
#define GL_MAP2_VERTEX_ATTRIB3_4_NV       0x8673
#define GL_MAP2_VERTEX_ATTRIB4_4_NV       0x8674
#define GL_MAP2_VERTEX_ATTRIB5_4_NV       0x8675
#define GL_MAP2_VERTEX_ATTRIB6_4_NV       0x8676
#define GL_MAP2_VERTEX_ATTRIB7_4_NV       0x8677
#define GL_MAP2_VERTEX_ATTRIB8_4_NV       0x8678
#define GL_MAP2_VERTEX_ATTRIB9_4_NV       0x8679
#define GL_MAP2_VERTEX_ATTRIB10_4_NV      0x867A
#define GL_MAP2_VERTEX_ATTRIB11_4_NV      0x867B
#define GL_MAP2_VERTEX_ATTRIB12_4_NV      0x867C
#define GL_MAP2_VERTEX_ATTRIB13_4_NV      0x867D
#define GL_MAP2_VERTEX_ATTRIB14_4_NV      0x867E
#define GL_MAP2_VERTEX_ATTRIB15_4_NV      0x867F
#endif

#if GL_NV_texture_shader
#define GL_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV          0x86D9
#define GL_UNSIGNED_INT_S8_S8_8_8_NV                     0x86DA
#define GL_UNSIGNED_INT_8_8_S8_S8_REV_NV                 0x86DB
#define GL_DSDT_MAG_INTENSITY_NV                         0x86DC
#define GL_TEXTURE_SHADER_NV                             0x86DE
#define GL_SHADER_OPERATION_NV                           0x86DF

#define GL_CULL_MODES_NV                                 0x86E0
#define GL_OFFSET_TEXTURE_MATRIX_NV                      0x86E1
#define GL_OFFSET_TEXTURE_SCALE_NV                       0x86E2
#define GL_OFFSET_TEXTURE_BIAS_NV                        0x86E3
#define GL_OFFSET_TEXTURE_2D_MATRIX_NV                   GL_OFFSET_TEXTURE_MATRIX_NV
#define GL_OFFSET_TEXTURE_2D_SCALE_NV                    GL_OFFSET_TEXTURE_SCALE_NV
#define GL_OFFSET_TEXTURE_2D_BIAS_NV                     GL_OFFSET_TEXTURE_BIAS_NV
#define GL_PREVIOUS_TEXTURE_INPUT_NV                     0x86E4
#define GL_CONST_EYE_NV                                  0x86E5
#define GL_SHADER_CONSISTENT_NV                          0x86DD
#define GL_PASS_THROUGH_NV                               0x86E6
#define GL_CULL_FRAGMENT_NV                              0x86E7
#define GL_OFFSET_TEXTURE_2D_NV                          0x86E8
#define GL_OFFSET_TEXTURE_RECTANGLE_NV                   0x864C
#define GL_OFFSET_TEXTURE_RECTANGLE_SCALE_NV             0x864D
#define GL_DEPENDENT_AR_TEXTURE_2D_NV                    0x86E9
#define GL_DEPENDENT_GB_TEXTURE_2D_NV                    0x86EA
#define GL_DOT_PRODUCT_NV                                0x86EC
#define GL_DOT_PRODUCT_DEPTH_REPLACE_NV                  0x86ED
#define GL_DOT_PRODUCT_TEXTURE_2D_NV                     0x86EE
#define GL_DOT_PRODUCT_TEXTURE_RECTANGLE_NV              0x864E
#define GL_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV               0x86F0
#define GL_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV               0x86F1
#define GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV               0x86F2
#define GL_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV     0x86F3
#define GL_HILO_NV                                       0x86F4
#define GL_DSDT_NV                                       0x86F5
#define GL_DSDT_MAG_NV                                   0x86F6
#define GL_DSDT_MAG_VIB_NV                               0x86F7
#define GL_HILO16_NV                                     0x86F8
#define GL_SIGNED_HILO_NV                                0x86F9
#define GL_SIGNED_HILO16_NV                              0x86FA
#define GL_SIGNED_RGBA_NV                                0x86FB
#define GL_SIGNED_RGBA8_NV                               0x86FC
#define GL_SIGNED_RGB_NV                                 0x86FE
#define GL_SIGNED_RGB8_NV                                0x86FF
#define GL_SIGNED_LUMINANCE_NV                           0x8701
#define GL_SIGNED_LUMINANCE8_NV                          0x8702
#define GL_SIGNED_LUMINANCE_ALPHA_NV                     0x8703
#define GL_SIGNED_LUMINANCE8_ALPHA8_NV                   0x8704
#define GL_SIGNED_ALPHA_NV                               0x8705
#define GL_SIGNED_ALPHA8_NV                              0x8706
#define GL_SIGNED_INTENSITY_NV                           0x8707
#define GL_SIGNED_INTENSITY8_NV                          0x8708
#define GL_DSDT8_NV                                      0x8709
#define GL_DSDT8_MAG8_NV                                 0x870A
#define GL_DSDT8_MAG8_INTENSITY8_NV                      0x870B
#define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV                  0x870C
#define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV                0x870D
#define GL_HI_SCALE_NV                                   0x870E
#define GL_LO_SCALE_NV                                   0x870F
#define GL_DS_SCALE_NV                                   0x8710
#define GL_DT_SCALE_NV                                   0x8711
#define GL_MAGNITUDE_SCALE_NV                            0x8712
#define GL_VIBRANCE_SCALE_NV                             0x8713
#define GL_HI_BIAS_NV                                    0x8714
#define GL_LO_BIAS_NV                                    0x8715
#define GL_DS_BIAS_NV                                    0x8716
#define GL_DT_BIAS_NV                                    0x8717
#define GL_MAGNITUDE_BIAS_NV                             0x8718
#define GL_VIBRANCE_BIAS_NV                              0x8719
#define GL_TEXTURE_BORDER_VALUES_NV                      0x871A
#define GL_TEXTURE_HI_SIZE_NV                            0x871B
#define GL_TEXTURE_LO_SIZE_NV                            0x871C
#define GL_TEXTURE_DS_SIZE_NV                            0x871D
#define GL_TEXTURE_DT_SIZE_NV                            0x871E
#define GL_TEXTURE_MAG_SIZE_NV                           0x871F
#endif

#if GL_NV_texture_shader2
#define GL_DOT_PRODUCT_TEXTURE_3D_NV                     0x86EF
#endif

#if GL_NV_texture_shader3
#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_NV               0x8850
#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_SCALE_NV         0x8851
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_NV        0x8852
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_SCALE_NV  0x8853
#define GL_OFFSET_HILO_TEXTURE_2D_NV                     0x8854
#define GL_OFFSET_HILO_TEXTURE_RECTANGLE_NV              0x8855
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_2D_NV          0x8856
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_RECTANGLE_NV   0x8857
#define GL_DEPENDENT_HILO_TEXTURE_2D_NV                  0x8858
#define GL_DEPENDENT_RGB_TEXTURE_3D_NV                   0x8859
#define GL_DEPENDENT_RGB_TEXTURE_CUBE_MAP_NV             0x885A
#define GL_DOT_PRODUCT_PASS_THROUGH_NV                   0x885B
#define GL_DOT_PRODUCT_TEXTURE_1D_NV                     0x885C
#define GL_DOT_PRODUCT_AFFINE_DEPTH_REPLACE_NV           0x885D
#define GL_HILO8_NV                                      0x885E
#define GL_SIGNED_HILO8_NV                               0x885F
#define GL_FORCE_BLUE_TO_ONE_NV                          0x8860
#endif

#if GL_NV_point_sprite
#define GL_POINT_SPRITE_NV                               0x8861
#define GL_COORD_REPLACE_NV                              0x8862
#define GL_POINT_SPRITE_R_MODE_NV                        0x8863
#endif

#if GL_NV_depth_clamp
#define GL_DEPTH_CLAMP_NV                                0x864F
#endif

#if GL_NV_multisample_filter_hint
#define GL_MULTISAMPLE_FILTER_HINT_NV                    0x8534
#endif

#if GL_NV_light_max_exponent
#define GL_MAX_SHININESS_NV                              0x8504
#define GL_MAX_SPOT_EXPONENT_NV                          0x8505
#endif

#if GL_NV_fragment_program2 || GL_NV_vertex_program2_option
#define GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV              0x88F4
#define GL_MAX_PROGRAM_CALL_DEPTH_NV                     0x88F5
#endif

#if GL_NV_fragment_program2
#define GL_MAX_PROGRAM_IF_DEPTH_NV                       0x88F6
#define GL_MAX_PROGRAM_LOOP_DEPTH_NV                     0x88F7
#define GL_MAX_PROGRAM_LOOP_COUNT_NV                     0x88F8
#endif

#if GL_NV_conditional_render
#define GL_QUERY_WAIT_NV                                 0x8E13
#define GL_QUERY_NO_WAIT_NV                              0x8E14
#define GL_QUERY_BY_REGION_WAIT_NV                       0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT_NV                    0x8E16
#endif

#if GL_PGI_vertex_hints
#define GL_VERTEX_DATA_HINT_PGI           0x1A22A
#define GL_VERTEX_CONSISTENT_HINT_PGI     0x1A22B
#define GL_MATERIAL_SIDE_HINT_PGI         0x1A22C
#define GL_MAX_VERTEX_HINT_PGI            0x1A22D
#define GL_COLOR3_BIT_PGI                 0x00010000
#define GL_COLOR4_BIT_PGI                 0x00020000
#define GL_EDGEFLAG_BIT_PGI               0x00040000
#define GL_INDEX_BIT_PGI                  0x00080000
#define GL_MAT_AMBIENT_BIT_PGI            0x00100000
#define GL_MAT_AMBIENT_AND_DIFFUSE_BIT_PGI 0x00200000
#define GL_MAT_DIFFUSE_BIT_PGI            0x00400000
#define GL_MAT_EMISSION_BIT_PGI           0x00800000
#define GL_MAT_COLOR_INDEXES_BIT_PGI      0x01000000
#define GL_MAT_SHININESS_BIT_PGI          0x02000000
#define GL_MAT_SPECULAR_BIT_PGI           0x04000000
#define GL_NORMAL_BIT_PGI                 0x08000000
#define GL_TEXCOORD1_BIT_PGI              0x10000000
#define GL_TEXCOORD2_BIT_PGI              0x20000000
#define GL_TEXCOORD3_BIT_PGI              0x40000000
#define GL_TEXCOORD4_BIT_PGI              0x80000000
#define GL_VERTEX23_BIT_PGI               0x00000004
#define GL_VERTEX4_BIT_PGI                0x00000008
#endif

#if GL_PGI_misc_hints
#define GL_PREFER_DOUBLEBUFFER_HINT_PGI   0x1A1F8
#define GL_CONSERVE_MEMORY_HINT_PGI       0x1A1FD
#define GL_RECLAIM_MEMORY_HINT_PGI        0x1A1FE
#define GL_NATIVE_GRAPHICS_HANDLE_PGI     0x1A202
#define GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI 0x1A203
#define GL_NATIVE_GRAPHICS_END_HINT_PGI   0x1A204
#define GL_ALWAYS_FAST_HINT_PGI           0x1A20C
#define GL_ALWAYS_SOFT_HINT_PGI           0x1A20D
#define GL_ALLOW_DRAW_OBJ_HINT_PGI        0x1A20E
#define GL_ALLOW_DRAW_WIN_HINT_PGI        0x1A20F
#define GL_ALLOW_DRAW_FRG_HINT_PGI        0x1A210
#define GL_ALLOW_DRAW_MEM_HINT_PGI        0x1A211
#define GL_STRICT_DEPTHFUNC_HINT_PGI      0x1A216
#define GL_STRICT_LIGHTING_HINT_PGI       0x1A217
#define GL_STRICT_SCISSOR_HINT_PGI        0x1A218
#define GL_FULL_STIPPLE_HINT_PGI          0x1A219
#define GL_CLIP_NEAR_HINT_PGI             0x1A220
#define GL_CLIP_FAR_HINT_PGI              0x1A221
#define GL_WIDE_LINE_HINT_PGI             0x1A222
#define GL_BACK_NORMALS_HINT_PGI          0x1A223
#endif


#if GL_REND_screen_coordinates
#define GL_SCREEN_COORDINATES_REND        0x8490
#define GL_INVERTED_SCREEN_W_REND         0x8491
#endif


#if GL_SGI_color_matrix
#define GL_COLOR_MATRIX_SGI               0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH_SGI   0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI 0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE_SGI 0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI 0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI 0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI 0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS_SGI 0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI 0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI 0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI 0x80BB
#endif

#if GL_SGI_color_table
#define GL_COLOR_TABLE_SGI                0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE_SGI 0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI 0x80D2
#define GL_PROXY_COLOR_TABLE_SGI          0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI 0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI 0x80D5
#define GL_COLOR_TABLE_SCALE_SGI          0x80D6
#define GL_COLOR_TABLE_BIAS_SGI           0x80D7
#define GL_COLOR_TABLE_FORMAT_SGI         0x80D8
#define GL_COLOR_TABLE_WIDTH_SGI          0x80D9
#define GL_COLOR_TABLE_RED_SIZE_SGI       0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_SGI     0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_SGI      0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_SGI     0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_SGI 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_SGI 0x80DF
#endif

#if GL_SGI_texture_color_table
#define GL_TEXTURE_COLOR_TABLE_SGI        0x80BC
#define GL_PROXY_TEXTURE_COLOR_TABLE_SGI  0x80BD
#endif

#if GL_SGI_depth_pass_instrument
#define GL_DEPTH_PASS_INSTRUMENT_SGIX     0x8310
#define GL_DEPTH_PASS_INSTRUMENT_COUNTERS_SGIX 0x8311
#define GL_DEPTH_PASS_INSTRUMENT_MAX_SGIX 0x8312
#endif

#if GL_SGIS_texture_filter4
#define GL_FILTER4_SGIS                   0x8146
#define GL_TEXTURE_FILTER4_SIZE_SGIS      0x8147
#endif

#if GL_SGIS_pixel_texture
#define GL_PIXEL_TEXTURE_SGIS             0x8353
#define GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS 0x8354
#define GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS 0x8355
#define GL_PIXEL_GROUP_COLOR_SGIS         0x8356
#endif

#if GL_SGIS_texture4D
#define GL_PACK_SKIP_VOLUMES_SGIS         0x8130
#define GL_PACK_IMAGE_DEPTH_SGIS          0x8131
#define GL_UNPACK_SKIP_VOLUMES_SGIS       0x8132
#define GL_UNPACK_IMAGE_DEPTH_SGIS        0x8133
#define GL_TEXTURE_4D_SGIS                0x8134
#define GL_PROXY_TEXTURE_4D_SGIS          0x8135
#define GL_TEXTURE_4DSIZE_SGIS            0x8136
#define GL_TEXTURE_WRAP_Q_SGIS            0x8137
#define GL_MAX_4D_TEXTURE_SIZE_SGIS       0x8138
#define GL_TEXTURE_4D_BINDING_SGIS        0x814F
#endif

#if GL_SGIS_detail_texture
#define GL_DETAIL_TEXTURE_2D_SGIS         0x8095
#define GL_DETAIL_TEXTURE_2D_BINDING_SGIS 0x8096
#define GL_LINEAR_DETAIL_SGIS             0x8097
#define GL_LINEAR_DETAIL_ALPHA_SGIS       0x8098
#define GL_LINEAR_DETAIL_COLOR_SGIS       0x8099
#define GL_DETAIL_TEXTURE_LEVEL_SGIS      0x809A
#define GL_DETAIL_TEXTURE_MODE_SGIS       0x809B
#define GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS 0x809C
#endif

#if GL_SGIS_sharpen_texture
#define GL_LINEAR_SHARPEN_SGIS            0x80AD
#define GL_LINEAR_SHARPEN_ALPHA_SGIS      0x80AE
#define GL_LINEAR_SHARPEN_COLOR_SGIS      0x80AF
#define GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS 0x80B0
#endif

#if GL_SGIS_texture_lod
#define GL_TEXTURE_MIN_LOD_SGIS           0x813A
#define GL_TEXTURE_MAX_LOD_SGIS           0x813B
#define GL_TEXTURE_BASE_LEVEL_SGIS        0x813C
#define GL_TEXTURE_MAX_LEVEL_SGIS         0x813D
#endif

#if GL_SGIS_multisample
#define GL_MULTISAMPLE_SGIS               0x809D
#define GL_SAMPLE_ALPHA_TO_MASK_SGIS      0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_SGIS       0x809F
#define GL_SAMPLE_MASK_SGIS               0x80A0
#define GL_1PASS_SGIS                     0x80A1
#define GL_2PASS_0_SGIS                   0x80A2
#define GL_2PASS_1_SGIS                   0x80A3
#define GL_4PASS_0_SGIS                   0x80A4
#define GL_4PASS_1_SGIS                   0x80A5
#define GL_4PASS_2_SGIS                   0x80A6
#define GL_4PASS_3_SGIS                   0x80A7
#define GL_SAMPLE_BUFFERS_SGIS            0x80A8
#define GL_SAMPLES_SGIS                   0x80A9
#define GL_SAMPLE_MASK_VALUE_SGIS         0x80AA
#define GL_SAMPLE_MASK_INVERT_SGIS        0x80AB
#define GL_SAMPLE_PATTERN_SGIS            0x80AC
#endif

#if GL_SGIS_generate_mipmap
#define GL_GENERATE_MIPMAP_SGIS           0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS      0x8192
#endif

#if GL_SGIS_texture_edge_clamp
#define GL_CLAMP_TO_EDGE_SGIS             0x812F
#endif

#if GL_SGIS_texture_border_clamp
#define GL_CLAMP_TO_BORDER_SGIS           0x812D
#endif

#if GL_SGIS_texture_select
#define GL_DUAL_ALPHA4_SGIS               0x8110
#define GL_DUAL_ALPHA8_SGIS               0x8111
#define GL_DUAL_ALPHA12_SGIS              0x8112
#define GL_DUAL_ALPHA16_SGIS              0x8113
#define GL_DUAL_LUMINANCE4_SGIS           0x8114
#define GL_DUAL_LUMINANCE8_SGIS           0x8115
#define GL_DUAL_LUMINANCE12_SGIS          0x8116
#define GL_DUAL_LUMINANCE16_SGIS          0x8117
#define GL_DUAL_INTENSITY4_SGIS           0x8118
#define GL_DUAL_INTENSITY8_SGIS           0x8119
#define GL_DUAL_INTENSITY12_SGIS          0x811A
#define GL_DUAL_INTENSITY16_SGIS          0x811B
#define GL_DUAL_LUMINANCE_ALPHA4_SGIS     0x811C
#define GL_DUAL_LUMINANCE_ALPHA8_SGIS     0x811D
#define GL_QUAD_ALPHA4_SGIS               0x811E
#define GL_QUAD_ALPHA8_SGIS               0x811F
#define GL_QUAD_LUMINANCE4_SGIS           0x8120
#define GL_QUAD_LUMINANCE8_SGIS           0x8121
#define GL_QUAD_INTENSITY4_SGIS           0x8122
#define GL_QUAD_INTENSITY8_SGIS           0x8123
#define GL_DUAL_TEXTURE_SELECT_SGIS       0x8124
#define GL_QUAD_TEXTURE_SELECT_SGIS       0x8125
#endif

#if GL_SGIS_point_parameters
#define GL_POINT_SIZE_MIN_EXT             0x8126
#define GL_POINT_SIZE_MIN_SGIS            0x8126
#define GL_POINT_SIZE_MAX_EXT             0x8127
#define GL_POINT_SIZE_MAX_SGIS            0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT  0x8128
#define GL_POINT_FADE_THRESHOLD_SIZE_SGIS 0x8128
#define GL_DISTANCE_ATTENUATION_EXT       0x8129
#define GL_DISTANCE_ATTENUATION_SGIS      0x8129
#endif

#if GL_SGIS_fog_function
#define GL_FOG_FUNC_SGIS                  0x812A
#define GL_FOG_FUNC_POINTS_SGIS           0x812B
#define GL_MAX_FOG_FUNC_POINTS_SGIS       0x812C
#endif

#if GL_SGIS_point_line_texgen
#define GL_EYE_DISTANCE_TO_POINT_SGIS     0x81F0
#define GL_OBJECT_DISTANCE_TO_POINT_SGIS  0x81F1
#define GL_EYE_DISTANCE_TO_LINE_SGIS      0x81F2
#define GL_OBJECT_DISTANCE_TO_LINE_SGIS   0x81F3
#define GL_EYE_POINT_SGIS                 0x81F4
#define GL_OBJECT_POINT_SGIS              0x81F5
#define GL_EYE_LINE_SGIS                  0x81F6
#define GL_OBJECT_LINE_SGIS               0x81F7
#endif

#if GL_SGIS_texture_color_mask
#define GL_TEXTURE_COLOR_WRITEMASK_SGIS   0x81EF
#endif

#if GL_SGIX_pixel_texture
#define GL_PIXEL_TEX_GEN_SGIX             0x8139
#define GL_PIXEL_TEX_GEN_MODE_SGIX        0x832B
#endif

#if GL_SGIX_clipmap
#define GL_LINEAR_CLIPMAP_LINEAR_SGIX     0x8170
#define GL_TEXTURE_CLIPMAP_CENTER_SGIX    0x8171
#define GL_TEXTURE_CLIPMAP_FRAME_SGIX     0x8172
#define GL_TEXTURE_CLIPMAP_OFFSET_SGIX    0x8173
#define GL_TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX 0x8174
#define GL_TEXTURE_CLIPMAP_LOD_OFFSET_SGIX 0x8175
#define GL_TEXTURE_CLIPMAP_DEPTH_SGIX     0x8176
#define GL_MAX_CLIPMAP_DEPTH_SGIX         0x8177
#define GL_MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX 0x8178
#define GL_NEAREST_CLIPMAP_NEAREST_SGIX   0x844D
#define GL_NEAREST_CLIPMAP_LINEAR_SGIX    0x844E
#define GL_LINEAR_CLIPMAP_NEAREST_SGIX    0x844F
#endif

#if GL_SGIX_shadow
#define GL_TEXTURE_COMPARE_SGIX           0x819A
#define GL_TEXTURE_COMPARE_OPERATOR_SGIX  0x819B
#define GL_TEXTURE_LEQUAL_R_SGIX          0x819C
#define GL_TEXTURE_GEQUAL_R_SGIX          0x819D
#endif

#if GL_SGIX_interlace
#define GL_INTERLACE_SGIX                 0x8094
#endif

#if GL_SGIX_pixel_tiles
#define GL_PIXEL_TILE_BEST_ALIGNMENT_SGIX 0x813E
#define GL_PIXEL_TILE_CACHE_INCREMENT_SGIX 0x813F
#define GL_PIXEL_TILE_WIDTH_SGIX          0x8140
#define GL_PIXEL_TILE_HEIGHT_SGIX         0x8141
#define GL_PIXEL_TILE_GRID_WIDTH_SGIX     0x8142
#define GL_PIXEL_TILE_GRID_HEIGHT_SGIX    0x8143
#define GL_PIXEL_TILE_GRID_DEPTH_SGIX     0x8144
#define GL_PIXEL_TILE_CACHE_SIZE_SGIX     0x8145
#endif

#if GL_SGIX_sprite
#define GL_SPRITE_SGIX                    0x8148
#define GL_SPRITE_MODE_SGIX               0x8149
#define GL_SPRITE_AXIS_SGIX               0x814A
#define GL_SPRITE_TRANSLATION_SGIX        0x814B
#define GL_SPRITE_AXIAL_SGIX              0x814C
#define GL_SPRITE_OBJECT_ALIGNED_SGIX     0x814D
#define GL_SPRITE_EYE_ALIGNED_SGIX        0x814E
#endif

#if GL_SGIX_texture_multi_buffer
#define GL_TEXTURE_MULTI_BUFFER_HINT_SGIX 0x812E
#endif

#if GL_SGIX_instruments
#define GL_INSTRUMENT_BUFFER_POINTER_SGIX 0x8180
#define GL_INSTRUMENT_MEASUREMENTS_SGIX   0x8181
#endif

#if GL_SGIX_texture_scale_bias
#define GL_POST_TEXTURE_FILTER_BIAS_SGIX  0x8179
#define GL_POST_TEXTURE_FILTER_SCALE_SGIX 0x817A
#define GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX 0x817B
#define GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX 0x817C
#endif

#if GL_SGIX_framezoom
#define GL_FRAMEZOOM_SGIX                 0x818B
#define GL_FRAMEZOOM_FACTOR_SGIX          0x818C
#define GL_MAX_FRAMEZOOM_FACTOR_SGIX      0x818D
#endif

#if GL_SGIX_polynomial_ffd
#define GL_GEOMETRY_DEFORMATION_SGIX      0x8194
#define GL_TEXTURE_DEFORMATION_SGIX       0x8195
#define GL_DEFORMATIONS_MASK_SGIX         0x8196
#define GL_MAX_DEFORMATION_ORDER_SGIX     0x8197
#endif

#if GL_SGIX_reference_plane
#define GL_REFERENCE_PLANE_SGIX           0x817D
#define GL_REFERENCE_PLANE_EQUATION_SGIX  0x817E
#endif

#if GL_SGIX_depth_texture
#define GL_DEPTH_COMPONENT16_SGIX         0x81A5
#define GL_DEPTH_COMPONENT24_SGIX         0x81A6
#define GL_DEPTH_COMPONENT32_SGIX         0x81A7
#endif

#if GL_SGIX_fog_offset
#define GL_FOG_OFFSET_SGIX                0x8198
#define GL_FOG_OFFSET_VALUE_SGIX          0x8199
#endif

#if GL_SGIX_texture_add_env
#define GL_TEXTURE_ENV_BIAS_SGIX          0x80BE
#endif

#if GL_SGIX_list_priority
#define GL_LIST_PRIORITY_SGIX             0x8182
#endif

#if GL_SGIX_ir_instrument1
#define GL_IR_INSTRUMENT1_SGIX            0x817F
#endif

#if GL_SGIX_calligraphic_fragment
#define GL_CALLIGRAPHIC_FRAGMENT_SGIX     0x8183
#endif

#if GL_SGIX_texture_lod_bias
#define GL_TEXTURE_LOD_BIAS_S_SGIX        0x818E
#define GL_TEXTURE_LOD_BIAS_T_SGIX        0x818F
#define GL_TEXTURE_LOD_BIAS_R_SGIX        0x8190
#endif

#if GL_SGIX_ycrcb
#define GL_YCRCB_422_SGIX                 0x81BB
#define GL_YCRCB_444_SGIX                 0x81BC
#endif

#if GL_SGIX_fragment_lighting
#define GL_FRAGMENT_LIGHTING_SGIX         0x8400
#define GL_FRAGMENT_COLOR_MATERIAL_SGIX   0x8401
#define GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX 0x8402
#define GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX 0x8403
#define GL_MAX_FRAGMENT_LIGHTS_SGIX       0x8404
#define GL_MAX_ACTIVE_LIGHTS_SGIX         0x8405
#define GL_CURRENT_RASTER_NORMAL_SGIX     0x8406
#define GL_LIGHT_ENV_MODE_SGIX            0x8407
#define GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX 0x8408
#define GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX 0x8409
#define GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX 0x840A
#define GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX 0x840B
#define GL_FRAGMENT_LIGHT0_SGIX           0x840C
#define GL_FRAGMENT_LIGHT1_SGIX           0x840D
#define GL_FRAGMENT_LIGHT2_SGIX           0x840E
#define GL_FRAGMENT_LIGHT3_SGIX           0x840F
#define GL_FRAGMENT_LIGHT4_SGIX           0x8410
#define GL_FRAGMENT_LIGHT5_SGIX           0x8411
#define GL_FRAGMENT_LIGHT6_SGIX           0x8412
#define GL_FRAGMENT_LIGHT7_SGIX           0x8413
#endif

#if GL_SGIX_blend_alpha_minmax
#define GL_ALPHA_MIN_SGIX                 0x8320
#define GL_ALPHA_MAX_SGIX                 0x8321
#endif

#if GL_SGIX_async
#define GL_ASYNC_MARKER_SGIX              0x8329
#endif

#if GL_SGIX_async_pixel
#define GL_ASYNC_TEX_IMAGE_SGIX           0x835C
#define GL_ASYNC_DRAW_PIXELS_SGIX         0x835D
#define GL_ASYNC_READ_PIXELS_SGIX         0x835E
#define GL_MAX_ASYNC_TEX_IMAGE_SGIX       0x835F
#define GL_MAX_ASYNC_DRAW_PIXELS_SGIX     0x8360
#define GL_MAX_ASYNC_READ_PIXELS_SGIX     0x8361
#endif

#if GL_SGIX_async_histogram
#define GL_ASYNC_HISTOGRAM_SGIX           0x832C
#define GL_MAX_ASYNC_HISTOGRAM_SGIX       0x832D
#endif

#if GL_SGIX_fog_scale
#define GL_FOG_SCALE_SGIX                 0x81FC
#define GL_FOG_SCALE_VALUE_SGIX           0x81FD
#endif

#if GL_SGIX_subsample
#define GL_PACK_SUBSAMPLE_RATE_SGIX       0x85A0
#define GL_UNPACK_SUBSAMPLE_RATE_SGIX     0x85A1
#define GL_PIXEL_SUBSAMPLE_4444_SGIX      0x85A2
#define GL_PIXEL_SUBSAMPLE_2424_SGIX      0x85A3
#define GL_PIXEL_SUBSAMPLE_4242_SGIX      0x85A4
#endif

#if GL_SGIX_ycrcba
#define GL_YCRCB_SGIX                     0x8318
#define GL_YCRCBA_SGIX                    0x8319
#endif

#if GL_SGIX_vertex_preclip
#define GL_VERTEX_PRECLIP_SGIX            0x83EE
#define GL_VERTEX_PRECLIP_HINT_SGIX       0x83EF
#endif

#if GL_SGIX_convolution_accuracy
#define GL_CONVOLUTION_HINT_SGIX          0x8316
#endif

#if GL_SGIX_resample
#define GL_PACK_RESAMPLE_SGIX             0x842C
#define GL_UNPACK_RESAMPLE_SGIX           0x842D
#define GL_RESAMPLE_REPLICATE_SGIX        0x842E
#define GL_RESAMPLE_ZERO_FILL_SGIX        0x842F
#define GL_RESAMPLE_DECIMATE_SGIX         0x8430
#endif


#if GL_SUN_global_alpha
#define GL_GLOBAL_ALPHA_SUN               0x81D9
#define GL_GLOBAL_ALPHA_FACTOR_SUN        0x81DA
#endif

#if GL_SUN_triangle_list
#define GL_RESTART_SUN                    0x01
#define GL_REPLACE_MIDDLE_SUN             0x02
#define GL_REPLACE_OLDEST_SUN             0x03
#define GL_TRIANGLE_LIST_SUN              0x81D7
#define GL_REPLACEMENT_CODE_SUN           0x81D8
#define GL_REPLACEMENT_CODE_ARRAY_SUN     0x85C0
#define GL_REPLACEMENT_CODE_ARRAY_TYPE_SUN 0x85C1
#define GL_REPLACEMENT_CODE_ARRAY_STRIDE_SUN 0x85C2
#define GL_REPLACEMENT_CODE_ARRAY_POINTER_SUN 0x85C3
#define GL_R1UI_V3F_SUN                   0x85C4
#define GL_R1UI_C4UB_V3F_SUN              0x85C5
#define GL_R1UI_C3F_V3F_SUN               0x85C6
#define GL_R1UI_N3F_V3F_SUN               0x85C7
#define GL_R1UI_C4F_N3F_V3F_SUN           0x85C8
#define GL_R1UI_T2F_V3F_SUN               0x85C9
#define GL_R1UI_T2F_N3F_V3F_SUN           0x85CA
#define GL_R1UI_T2F_C4F_N3F_V3F_SUN       0x85CB
#endif

#if GL_SUN_convolution_border_modes
#define GL_WRAP_BORDER_SUN                0x81D4
#endif

#if GL_SUNX_constant_data
#define GL_UNPACK_CONSTANT_DATA_SUNX      0x81D5
#define GL_TEXTURE_CONSTANT_DATA_SUNX     0x81D6
#endif


#if GL_WIN_phong_shading
#define GL_PHONG_WIN                      0x80EA
#define GL_PHONG_HINT_WIN                 0x80EB
#endif

#if GL_WIN_specular_fog
#define GL_FOG_SPECULAR_TEXTURE_WIN       0x80EC
#endif


#if GL_3DFX_texture_compression_FXT1
#define GL_COMPRESSED_RGB_FXT1_3DFX       0x86B0
#define GL_COMPRESSED_RGBA_FXT1_3DFX      0x86B1
#endif

#if GL_3DFX_multisample
#define GL_MULTISAMPLE_3DFX               0x86B2
#define GL_SAMPLE_BUFFERS_3DFX            0x86B3
#define GL_SAMPLES_3DFX                   0x86B4
#define GL_MULTISAMPLE_BIT_3DFX           0x20000000
#endif


/*************************************************************/

#if GL_ARB_multitexture
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glActiveTextureARBProcPtr) (GLenum);
typedef void (* glClientActiveTextureARBProcPtr) (GLenum);
typedef void (* glMultiTexCoord1dARBProcPtr) (GLenum, GLdouble);
typedef void (* glMultiTexCoord1dvARBProcPtr) (GLenum, const GLdouble *);
typedef void (* glMultiTexCoord1fARBProcPtr) (GLenum, GLfloat);
typedef void (* glMultiTexCoord1fvARBProcPtr) (GLenum, const GLfloat *);
typedef void (* glMultiTexCoord1iARBProcPtr) (GLenum, GLint);
typedef void (* glMultiTexCoord1ivARBProcPtr) (GLenum, const GLint *);
typedef void (* glMultiTexCoord1sARBProcPtr) (GLenum, GLshort);
typedef void (* glMultiTexCoord1svARBProcPtr) (GLenum, const GLshort *);
typedef void (* glMultiTexCoord2dARBProcPtr) (GLenum, GLdouble, GLdouble);
typedef void (* glMultiTexCoord2dvARBProcPtr) (GLenum, const GLdouble *);
typedef void (* glMultiTexCoord2fARBProcPtr) (GLenum, GLfloat, GLfloat);
typedef void (* glMultiTexCoord2fvARBProcPtr) (GLenum, const GLfloat *);
typedef void (* glMultiTexCoord2iARBProcPtr) (GLenum, GLint, GLint);
typedef void (* glMultiTexCoord2ivARBProcPtr) (GLenum, const GLint *);
typedef void (* glMultiTexCoord2sARBProcPtr) (GLenum, GLshort, GLshort);
typedef void (* glMultiTexCoord2svARBProcPtr) (GLenum, const GLshort *);
typedef void (* glMultiTexCoord3dARBProcPtr) (GLenum, GLdouble, GLdouble, GLdouble);
typedef void (* glMultiTexCoord3dvARBProcPtr) (GLenum, const GLdouble *);
typedef void (* glMultiTexCoord3fARBProcPtr) (GLenum, GLfloat, GLfloat, GLfloat);
typedef void (* glMultiTexCoord3fvARBProcPtr) (GLenum, const GLfloat *);
typedef void (* glMultiTexCoord3iARBProcPtr) (GLenum, GLint, GLint, GLint);
typedef void (* glMultiTexCoord3ivARBProcPtr) (GLenum, const GLint *);
typedef void (* glMultiTexCoord3sARBProcPtr) (GLenum, GLshort, GLshort, GLshort);
typedef void (* glMultiTexCoord3svARBProcPtr) (GLenum, const GLshort *);
typedef void (* glMultiTexCoord4dARBProcPtr) (GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
typedef void (* glMultiTexCoord4dvARBProcPtr) (GLenum, const GLdouble *);
typedef void (* glMultiTexCoord4fARBProcPtr) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glMultiTexCoord4fvARBProcPtr) (GLenum, const GLfloat *);
typedef void (* glMultiTexCoord4iARBProcPtr) (GLenum, GLint, GLint, GLint, GLint);
typedef void (* glMultiTexCoord4ivARBProcPtr) (GLenum, const GLint *);
typedef void (* glMultiTexCoord4sARBProcPtr) (GLenum, GLshort, GLshort, GLshort, GLshort);
typedef void (* glMultiTexCoord4svARBProcPtr) (GLenum, const GLshort *);
#else
extern void glActiveTextureARB (GLenum);
extern void glClientActiveTextureARB (GLenum);
extern void glMultiTexCoord1dARB (GLenum, GLdouble);
extern void glMultiTexCoord1dvARB (GLenum, const GLdouble *);
extern void glMultiTexCoord1fARB (GLenum, GLfloat);
extern void glMultiTexCoord1fvARB (GLenum, const GLfloat *);
extern void glMultiTexCoord1iARB (GLenum, GLint);
extern void glMultiTexCoord1ivARB (GLenum, const GLint *);
extern void glMultiTexCoord1sARB (GLenum, GLshort);
extern void glMultiTexCoord1svARB (GLenum, const GLshort *);
extern void glMultiTexCoord2dARB (GLenum, GLdouble, GLdouble);
extern void glMultiTexCoord2dvARB (GLenum, const GLdouble *);
extern void glMultiTexCoord2fARB (GLenum, GLfloat, GLfloat);
extern void glMultiTexCoord2fvARB (GLenum, const GLfloat *);
extern void glMultiTexCoord2iARB (GLenum, GLint, GLint);
extern void glMultiTexCoord2ivARB (GLenum, const GLint *);
extern void glMultiTexCoord2sARB (GLenum, GLshort, GLshort);
extern void glMultiTexCoord2svARB (GLenum, const GLshort *);
extern void glMultiTexCoord3dARB (GLenum, GLdouble, GLdouble, GLdouble);
extern void glMultiTexCoord3dvARB (GLenum, const GLdouble *);
extern void glMultiTexCoord3fARB (GLenum, GLfloat, GLfloat, GLfloat);
extern void glMultiTexCoord3fvARB (GLenum, const GLfloat *);
extern void glMultiTexCoord3iARB (GLenum, GLint, GLint, GLint);
extern void glMultiTexCoord3ivARB (GLenum, const GLint *);
extern void glMultiTexCoord3sARB (GLenum, GLshort, GLshort, GLshort);
extern void glMultiTexCoord3svARB (GLenum, const GLshort *);
extern void glMultiTexCoord4dARB (GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
extern void glMultiTexCoord4dvARB (GLenum, const GLdouble *);
extern void glMultiTexCoord4fARB (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glMultiTexCoord4fvARB (GLenum, const GLfloat *);
extern void glMultiTexCoord4iARB (GLenum, GLint, GLint, GLint, GLint);
extern void glMultiTexCoord4ivARB (GLenum, const GLint *);
extern void glMultiTexCoord4sARB (GLenum, GLshort, GLshort, GLshort, GLshort);
extern void glMultiTexCoord4svARB (GLenum, const GLshort *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ARB_transpose_matrix
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glLoadTransposeMatrixfARBProcPtr) (const GLfloat *);
typedef void (* glLoadTransposeMatrixdARBProcPtr) (const GLdouble *);
typedef void (* glMultTransposeMatrixfARBProcPtr) (const GLfloat *);
typedef void (* glMultTransposeMatrixdARBProcPtr) (const GLdouble *);
#else
extern void glLoadTransposeMatrixfARB (const GLfloat *);
extern void glLoadTransposeMatrixdARB (const GLdouble *);
extern void glMultTransposeMatrixfARB (const GLfloat *);
extern void glMultTransposeMatrixdARB (const GLdouble *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ARB_multisample
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glSampleCoverageARBProcPtr) (GLclampf, GLboolean);
typedef void (* glSamplePassARBProcPtr) (GLenum);
#else
extern void glSampleCoverageARB (GLclampf, GLboolean);
extern void glSamplePassARB (GLenum);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ARB_texture_compression
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glCompressedTexImage3DARBProcPtr) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
typedef void (* glCompressedTexImage2DARBProcPtr) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
typedef void (* glCompressedTexImage1DARBProcPtr) (GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid *);
typedef void (* glCompressedTexSubImage3DARBProcPtr) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
typedef void (* glCompressedTexSubImage2DARBProcPtr) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
typedef void (* glCompressedTexSubImage1DARBProcPtr) (GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid *);
typedef void (* glGetCompressedTexImageARBProcPtr) (GLenum, GLint, GLvoid *);
#else
extern void glCompressedTexImage3DARB (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
extern void glCompressedTexImage2DARB (GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *);
extern void glCompressedTexImage1DARB (GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid *);
extern void glCompressedTexSubImage3DARB (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
extern void glCompressedTexSubImage2DARB (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *);
extern void glCompressedTexSubImage1DARB (GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid *);
extern void glGetCompressedTexImageARB (GLenum, GLint, GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ARB_vertex_blend
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glWeightbvARBProcPtr) (GLint, const GLbyte *);
typedef void (* glWeightsvARBProcPtr) (GLint, const GLshort *);
typedef void (* glWeightivARBProcPtr) (GLint, const GLint *);
typedef void (* glWeightfvARBProcPtr) (GLint, const GLfloat *);
typedef void (* glWeightdvARBProcPtr) (GLint, const GLdouble *);
typedef void (* glWeightubvARBProcPtr) (GLint, const GLubyte *);
typedef void (* glWeightusvARBProcPtr) (GLint, const GLushort *);
typedef void (* glWeightuivARBProcPtr) (GLint, const GLuint *);
typedef void (* glWeightPointerARBProcPtr) (GLint, GLenum, GLsizei, const GLvoid *);
typedef void (* glVertexBlendARBProcPtr) (GLint);
#else
extern void glWeightbvARB(GLint, const GLbyte *);
extern void glWeightsvARB(GLint, const GLshort *);
extern void glWeightivARB(GLint, const GLint *);
extern void glWeightfvARB(GLint, const GLfloat *);
extern void glWeightdvARB(GLint, const GLdouble *);
extern void glWeightubvARB(GLint, const GLubyte *);
extern void glWeightusvARB(GLint, const GLushort *);
extern void glWeightuivARB(GLint, const GLuint *);
extern void glWeightPointerARB(GLint, GLenum, GLsizei, const GLvoid *);
extern void glVertexBlendARB(GLint);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ARB_window_pos
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glWindowPos2dARBProcPtr) (GLdouble, GLdouble);
typedef void (* glWindowPos2dvARBProcPtr) (const GLdouble *);
typedef void (* glWindowPos2fARBProcPtr) (GLfloat, GLfloat);
typedef void (* glWindowPos2fvARBProcPtr) (const GLfloat *);
typedef void (* glWindowPos2iARBProcPtr) (GLint, GLint);
typedef void (* glWindowPos2ivARBProcPtr) (const GLint *);
typedef void (* glWindowPos2sARBProcPtr) (GLshort, GLshort);
typedef void (* glWindowPos2svARBProcPtr) (const GLshort *);
typedef void (* glWindowPos3dARBProcPtr) (GLdouble, GLdouble, GLdouble);
typedef void (* glWindowPos3dvARBProcPtr) (const GLdouble *);
typedef void (* glWindowPos3fARBProcPtr) (GLfloat, GLfloat, GLfloat);
typedef void (* glWindowPos3fvARBProcPtr) (const GLfloat *);
typedef void (* glWindowPos3iARBProcPtr) (GLint, GLint, GLint);
typedef void (* glWindowPos3ivARBProcPtr) (const GLint *);
typedef void (* glWindowPos3sARBProcPtr) (GLshort, GLshort, GLshort);
typedef void (* glWindowPos3svARBProcPtr) (const GLshort *);
#else
extern void glWindowPos2dARB (GLdouble, GLdouble);
extern void glWindowPos2dvARB (const GLdouble *);
extern void glWindowPos2fARB (GLfloat, GLfloat);
extern void glWindowPos2fvARB (const GLfloat *);
extern void glWindowPos2iARB (GLint, GLint);
extern void glWindowPos2ivARB (const GLint *);
extern void glWindowPos2sARB (GLshort, GLshort);
extern void glWindowPos2svARB (const GLshort *);
extern void glWindowPos3dARB (GLdouble, GLdouble, GLdouble);
extern void glWindowPos3dvARB (const GLdouble *);
extern void glWindowPos3fARB (GLfloat, GLfloat, GLfloat);
extern void glWindowPos3fvARB (const GLfloat *);
extern void glWindowPos3iARB (GLint, GLint, GLint);
extern void glWindowPos3ivARB (const GLint *);
extern void glWindowPos3sARB (GLshort, GLshort, GLshort);
extern void glWindowPos3svARB (const GLshort *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ARB_occlusion_query
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glGenQueriesARBProcPtr) (GLsizei n, GLuint *ids);
typedef void (* glDeleteQueriesARBProcPtr) (GLsizei n, const GLuint *ids);
typedef GLboolean (* glIsQueryARBProcPtr) (GLuint id);
typedef void (* glBeginQueryARBProcPtr) (GLenum target, GLuint id);
typedef void (* glEndQueryARBProcPtr) (GLenum target);
typedef void (* glGetQueryivARBProcPtr) (GLenum target, GLenum pname, GLint *params);
typedef void (* glGetQueryObjectivARBProcPtr) (GLuint id, GLenum pname, GLint *params);
typedef void (* glGetQueryObjectuivARBProcPtr) (GLuint id, GLenum pname, GLuint *params);
#else
extern void glGenQueriesARB(GLsizei n, GLuint *ids);
extern void glDeleteQueriesARB(GLsizei n, const GLuint *ids);
extern GLboolean glIsQueryARB(GLuint id);
extern void glBeginQueryARB(GLenum target, GLuint id);
extern void glEndQueryARB(GLenum target);
extern void glGetQueryivARB(GLenum target, GLenum pname, GLint *params);
extern void glGetQueryObjectivARB(GLuint id, GLenum pname, GLint *params);
extern void glGetQueryObjectuivARB(GLuint id, GLenum pname, GLuint *params);
#endif
#endif

#if GL_ARB_point_parameters
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glPointParameterfARBProcPtr) (GLenum pname, GLfloat param);
typedef void (* glPointParameterfvARBProcPtr) (GLenum pname, const GLfloat *params);
#else
extern void glPointParameterfARB(GLenum pname, GLfloat param);
extern void glPointParameterfvARB(GLenum pname, const GLfloat *params);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ARB_vertex_program
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBindProgramARBProcPtr) (GLenum target, GLuint program);
typedef void (* glDeleteProgramsARBProcPtr) (GLsizei n, const GLuint *programs);
typedef void (* glGenProgramsARBProcPtr) (GLsizei n, GLuint *programs);
typedef GLboolean (* glIsProgramARBProcPtr) (GLuint program);

typedef void (* glProgramEnvParameter4dARBProcPtr) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (* glProgramEnvParameter4dvARBProcPtr) (GLenum target, GLuint index, const GLdouble *params);
typedef void (* glProgramEnvParameter4fARBProcPtr) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (* glProgramEnvParameter4fvARBProcPtr) (GLenum target, GLuint index, const GLfloat *params);
typedef void (* glProgramLocalParameter4dARBProcPtr) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (* glProgramLocalParameter4dvARBProcPtr) (GLenum target, GLuint index, const GLdouble *params);
typedef void (* glProgramLocalParameter4fARBProcPtr) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (* glProgramLocalParameter4fvARBProcPtr) (GLenum target, GLuint index, const GLfloat *params);

#if GL_EXT_gpu_program_parameters
typedef void (* glProgramEnvParameters4fvEXTProcPtr) (GLenum target, GLuint index, GLsizei count, const GLfloat *params);
typedef void (* glProgramLocalParameters4fvEXTProcPtr) (GLenum target, GLuint index, GLsizei count, const GLfloat *params);
#endif

typedef void (* glGetProgramEnvParameterdvARBProcPtr) (GLenum target, GLuint index, GLdouble *params);
typedef void (* glGetProgramEnvParameterfvARBProcPtr) (GLenum target, GLuint index, GLfloat *params);
typedef void (* glGetProgramLocalParameterdvARBProcPtr) (GLenum target, GLuint index, GLdouble *params);
typedef void (* glGetProgramLocalParameterfvARBProcPtr) (GLenum target, GLuint index, GLfloat *params);

typedef void (* glProgramStringARBProcPtr) (GLenum target, GLenum format, GLsizei len, const GLvoid *string);
typedef void (* glGetProgramStringARBProcPtr) (GLenum target, GLenum pname, GLvoid *string);

typedef void (* glGetProgramivARBProcPtr) (GLenum target, GLenum pname, GLint *params);
#else
extern void glBindProgramARB(GLenum target, GLuint program);
extern void glDeleteProgramsARB(GLsizei n, const GLuint *programs);
extern void glGenProgramsARB(GLsizei n, GLuint *programs);
extern GLboolean glIsProgramARB(GLuint program);

extern void glProgramEnvParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glProgramEnvParameter4dvARB(GLenum target, GLuint index, const GLdouble *params);
extern void glProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glProgramEnvParameter4fvARB(GLenum target, GLuint index, const GLfloat *params);
extern void glProgramLocalParameter4dARB(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glProgramLocalParameter4dvARB(GLenum target, GLuint index, const GLdouble *params);
extern void glProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glProgramLocalParameter4fvARB(GLenum target, GLuint index, const GLfloat *params);

extern void glGetProgramEnvParameterdvARB(GLenum target, GLuint index, GLdouble *params);
extern void glGetProgramEnvParameterfvARB(GLenum target, GLuint index, GLfloat *params);
#if GL_EXT_gpu_program_parameters
extern void glProgramEnvParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat *params);
extern void glProgramLocalParameters4fvEXT(GLenum target, GLuint index, GLsizei count, const GLfloat *params);
#endif

extern void glGetProgramLocalParameterdvARB(GLenum target, GLuint index, GLdouble *params);
extern void glGetProgramLocalParameterfvARB(GLenum target, GLuint index, GLfloat *params);

extern void glProgramStringARB(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
extern void glGetProgramStringARB(GLenum target, GLenum pname, GLvoid *string);

extern void glGetProgramivARB(GLenum target, GLenum pname, GLint *params);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif


#if GL_ARB_vertex_shader || GL_ARB_vertex_program
#ifdef GL_GLEXT_FUNCTION_POINTERS

typedef void (* glVertexAttrib1dARBProcPtr) (GLuint index, GLdouble x);
typedef void (* glVertexAttrib1dvARBProcPtr) (GLuint index, const GLdouble *v);
typedef void (* glVertexAttrib1fARBProcPtr) (GLuint index, GLfloat x);
typedef void (* glVertexAttrib1fvARBProcPtr) (GLuint index, const GLfloat *v);
typedef void (* glVertexAttrib1sARBProcPtr) (GLuint index, GLshort x);
typedef void (* glVertexAttrib1svARBProcPtr) (GLuint index, const GLshort *v);
typedef void (* glVertexAttrib2dARBProcPtr) (GLuint index, GLdouble x, GLdouble y);
typedef void (* glVertexAttrib2dvARBProcPtr) (GLuint index, const GLdouble *v);
typedef void (* glVertexAttrib2fARBProcPtr) (GLuint index, GLfloat x, GLfloat y);
typedef void (* glVertexAttrib2fvARBProcPtr) (GLuint index, const GLfloat *v);
typedef void (* glVertexAttrib2sARBProcPtr) (GLuint index, GLshort x, GLshort y);
typedef void (* glVertexAttrib2svARBProcPtr) (GLuint index, const GLshort *v);
typedef void (* glVertexAttrib3dARBProcPtr) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (* glVertexAttrib3dvARBProcPtr) (GLuint index, const GLdouble *v);
typedef void (* glVertexAttrib3fARBProcPtr) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (* glVertexAttrib3fvARBProcPtr) (GLuint index, const GLfloat *v);
typedef void (* glVertexAttrib3sARBProcPtr) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (* glVertexAttrib3svARBProcPtr) (GLuint index, const GLshort *v);
typedef void (* glVertexAttrib4NbvARBProcPtr) (GLuint index, const GLbyte *v);
typedef void (* glVertexAttrib4NivARBProcPtr) (GLuint index, const GLint *v);
typedef void (* glVertexAttrib4NsvARBProcPtr) (GLuint index, const GLshort *v);
typedef void (* glVertexAttrib4NubARBProcPtr) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (* glVertexAttrib4NubvARBProcPtr) (GLuint index, const GLubyte *v);
typedef void (* glVertexAttrib4NuivARBProcPtr) (GLuint index, const GLuint *v);
typedef void (* glVertexAttrib4NusvARBProcPtr) (GLuint index, const GLushort *v);
typedef void (* glVertexAttrib4bvARBProcPtr) (GLuint index, const GLbyte *v);
typedef void (* glVertexAttrib4dARBProcPtr) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (* glVertexAttrib4dvARBProcPtr) (GLuint index, const GLdouble *v);
typedef void (* glVertexAttrib4fARBProcPtr) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (* glVertexAttrib4fvARBProcPtr) (GLuint index, const GLfloat *v);
typedef void (* glVertexAttrib4ivARBProcPtr) (GLuint index, const GLint *v);
typedef void (* glVertexAttrib4sARBProcPtr) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (* glVertexAttrib4svARBProcPtr) (GLuint index, const GLshort *v);
typedef void (* glVertexAttrib4ubvARBProcPtr) (GLuint index, const GLubyte *v);
typedef void (* glVertexAttrib4uivARBProcPtr) (GLuint index, const GLuint *v);
typedef void (* glVertexAttrib4usvARBProcPtr) (GLuint index, const GLushort *v);
typedef void (* glVertexAttribPointerARBProcPtr) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

typedef void (* glDisableVertexAttribArrayARBProcPtr) (GLuint index);
typedef void (* glEnableVertexAttribArrayARBProcPtr) (GLuint index);

typedef void (* glGetVertexAttribPointervARBProcPtr) (GLuint index, GLenum pname, GLvoid **pointer);
typedef void (* glGetVertexAttribdvARBProcPtr) (GLuint index, GLenum pname, GLdouble *params);
typedef void (* glGetVertexAttribfvARBProcPtr) (GLuint index, GLenum pname, GLfloat *params);
typedef void (* glGetVertexAttribivARBProcPtr) (GLuint index, GLenum pname, GLint *params);

#else

extern void glVertexAttrib1dARB(GLuint index, GLdouble x);
extern void glVertexAttrib1dvARB(GLuint index, const GLdouble *v);
extern void glVertexAttrib1fARB(GLuint index, GLfloat x);
extern void glVertexAttrib1fvARB(GLuint index, const GLfloat *v);
extern void glVertexAttrib1sARB(GLuint index, GLshort x);
extern void glVertexAttrib1svARB(GLuint index, const GLshort *v);
extern void glVertexAttrib2dARB(GLuint index, GLdouble x, GLdouble y);
extern void glVertexAttrib2dvARB(GLuint index, const GLdouble *v);
extern void glVertexAttrib2fARB(GLuint index, GLfloat x, GLfloat y);
extern void glVertexAttrib2fvARB(GLuint index, const GLfloat *v);
extern void glVertexAttrib2sARB(GLuint index, GLshort x, GLshort y);
extern void glVertexAttrib2svARB(GLuint index, const GLshort *v);
extern void glVertexAttrib3dARB(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void glVertexAttrib3dvARB(GLuint index, const GLdouble *v);
extern void glVertexAttrib3fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern void glVertexAttrib3fvARB(GLuint index, const GLfloat *v);
extern void glVertexAttrib3sARB(GLuint index, GLshort x, GLshort y, GLshort z);
extern void glVertexAttrib3svARB(GLuint index, const GLshort *v);
extern void glVertexAttrib4NbvARB(GLuint index, const GLbyte *v);
extern void glVertexAttrib4NivARB(GLuint index, const GLint *v);
extern void glVertexAttrib4NsvARB(GLuint index, const GLshort *v);
extern void glVertexAttrib4NubARB(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern void glVertexAttrib4NubvARB(GLuint index, const GLubyte *v);
extern void glVertexAttrib4NuivARB(GLuint index, const GLuint *v);
extern void glVertexAttrib4NusvARB(GLuint index, const GLushort *v);
extern void glVertexAttrib4bvARB(GLuint index, const GLbyte *v);
extern void glVertexAttrib4dARB(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glVertexAttrib4dvARB(GLuint index, const GLdouble *v);
extern void glVertexAttrib4fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glVertexAttrib4fvARB(GLuint index, const GLfloat *v);
extern void glVertexAttrib4ivARB(GLuint index, const GLint *v);
extern void glVertexAttrib4sARB(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern void glVertexAttrib4svARB(GLuint index, const GLshort *v);
extern void glVertexAttrib4ubvARB(GLuint index, const GLubyte *v);
extern void glVertexAttrib4uivARB(GLuint index, const GLuint *v);
extern void glVertexAttrib4usvARB(GLuint index, const GLushort *v);
extern void glVertexAttribPointerARB(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

extern void glDisableVertexAttribArrayARB(GLuint index);
extern void glEnableVertexAttribArrayARB(GLuint index);

extern void glGetVertexAttribPointervARB(GLuint index, GLenum pname, GLvoid **pointer);
extern void glGetVertexAttribdvARB(GLuint index, GLenum pname, GLdouble *params);
extern void glGetVertexAttribfvARB(GLuint index, GLenum pname, GLfloat *params);
extern void glGetVertexAttribivARB(GLuint index, GLenum pname, GLint *params);

#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ARB_shader_objects
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glDeleteObjectARBProcPtr) (GLhandleARB obj);
typedef GLhandleARB (* glGetHandleARBProcPtr) (GLenum pname);
typedef void (* glDetachObjectARBProcPtr) (GLhandleARB containerObj, GLhandleARB attachedObj);
typedef GLhandleARB (* glCreateShaderObjectARBProcPtr) (GLenum shaderType);
typedef void (* glShaderSourceARBProcPtr) (GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length);
typedef void (* glCompileShaderARBProcPtr) (GLhandleARB shaderObj);
typedef GLhandleARB (* glCreateProgramObjectARBProcPtr) (void);
typedef void (* glAttachObjectARBProcPtr) (GLhandleARB containerObj, GLhandleARB obj);
typedef void (* glLinkProgramARBProcPtr) (GLhandleARB programObj);
typedef void (* glUseProgramObjectARBProcPtr) (GLhandleARB programObj);
typedef void (* glValidateProgramARBProcPtr) (GLhandleARB programObj);
typedef void (* glUniform1fARBProcPtr) (GLint location, GLfloat v0);
typedef void (* glUniform2fARBProcPtr) (GLint location, GLfloat v0, GLfloat v1);
typedef void (* glUniform3fARBProcPtr) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (* glUniform4fARBProcPtr) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (* glUniform1iARBProcPtr) (GLint location, GLint v0);
typedef void (* glUniform2iARBProcPtr) (GLint location, GLint v0, GLint v1);
typedef void (* glUniform3iARBProcPtr) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (* glUniform4iARBProcPtr) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (* glUniform1fvARBProcPtr) (GLint location, GLsizei count, const GLfloat *value);
typedef void (* glUniform2fvARBProcPtr) (GLint location, GLsizei count, const GLfloat *value);
typedef void (* glUniform3fvARBProcPtr) (GLint location, GLsizei count, const GLfloat *value);
typedef void (* glUniform4fvARBProcPtr) (GLint location, GLsizei count, const GLfloat *value);
typedef void (* glUniform1ivARBProcPtr) (GLint location, GLsizei count, const GLint *value);
typedef void (* glUniform2ivARBProcPtr) (GLint location, GLsizei count, const GLint *value);
typedef void (* glUniform3ivARBProcPtr) (GLint location, GLsizei count, const GLint *value);
typedef void (* glUniform4ivARBProcPtr) (GLint location, GLsizei count, const GLint *value);
typedef void (* glUniformMatrix2fvARBProcPtr) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (* glUniformMatrix3fvARBProcPtr) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (* glUniformMatrix4fvARBProcPtr) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (* glGetObjectParameterfvARBProcPtr) (GLhandleARB obj, GLenum pname, GLfloat *params);
typedef void (* glGetObjectParameterivARBProcPtr) (GLhandleARB obj, GLenum pname, GLint *params);
typedef void (* glGetInfoLogARBProcPtr) (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
typedef void (* glGetAttachedObjectsARBProcPtr) (GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj);
typedef GLint (* glGetUniformLocationARBProcPtr) (GLhandleARB programObj, const GLcharARB *name);
typedef void (* glGetActiveUniformARBProcPtr) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
typedef void (* glGetUniformfvARBProcPtr) (GLhandleARB programObj, GLint location, GLfloat *params);
typedef void (* glGetUniformivARBProcPtr) (GLhandleARB programObj, GLint location, GLint *params);
typedef void (* glGetShaderSourceARBProcPtr) (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source);
#else
extern void glDeleteObjectARB(GLhandleARB obj);
extern GLhandleARB glGetHandleARB(GLenum pname);
extern void glDetachObjectARB(GLhandleARB containerObj, GLhandleARB attachedObj);
extern GLhandleARB glCreateShaderObjectARB(GLenum shaderType);
extern void glShaderSourceARB(GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length);
extern void glCompileShaderARB(GLhandleARB shaderObj);
extern GLhandleARB glCreateProgramObjectARB(void);
extern void glAttachObjectARB(GLhandleARB containerObj, GLhandleARB obj);
extern void glLinkProgramARB(GLhandleARB programObj);
extern void glUseProgramObjectARB(GLhandleARB programObj);
extern void glValidateProgramARB(GLhandleARB programObj);
extern void glUniform1fARB(GLint location, GLfloat v0);
extern void glUniform2fARB(GLint location, GLfloat v0, GLfloat v1);
extern void glUniform3fARB(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void glUniform4fARB(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void glUniform1iARB(GLint location, GLint v0);
extern void glUniform2iARB(GLint location, GLint v0, GLint v1);
extern void glUniform3iARB(GLint location, GLint v0, GLint v1, GLint v2);
extern void glUniform4iARB(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void glUniform1fvARB(GLint location, GLsizei count, const GLfloat *value);
extern void glUniform2fvARB(GLint location, GLsizei count, const GLfloat *value);
extern void glUniform3fvARB(GLint location, GLsizei count, const GLfloat *value);
extern void glUniform4fvARB(GLint location, GLsizei count, const GLfloat *value);
extern void glUniform1ivARB(GLint location, GLsizei count, const GLint *value);
extern void glUniform2ivARB(GLint location, GLsizei count, const GLint *value);
extern void glUniform3ivARB(GLint location, GLsizei count, const GLint *value);
extern void glUniform4ivARB(GLint location, GLsizei count, const GLint *value);
extern void glUniformMatrix2fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void glUniformMatrix3fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void glUniformMatrix4fvARB(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void glGetObjectParameterfvARB(GLhandleARB obj, GLenum pname, GLfloat *params);
extern void glGetObjectParameterivARB(GLhandleARB obj, GLenum pname, GLint *params);
extern void glGetInfoLogARB(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
extern void glGetAttachedObjectsARB(GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj);
extern GLint glGetUniformLocationARB(GLhandleARB programObj, const GLcharARB *name);
extern void glGetActiveUniformARB(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
extern void glGetUniformfvARB(GLhandleARB programObj, GLint location, GLfloat *params);
extern void glGetUniformivARB(GLhandleARB programObj, GLint location, GLint *params);
extern void glGetShaderSourceARB(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ARB_vertex_shader
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBindAttribLocationARBProcPtr) (GLhandleARB programObj, GLuint index, const GLcharARB *name);
typedef void (* glGetActiveAttribARBProcPtr) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
typedef GLint (* glGetAttribLocationARBProcPtr) (GLhandleARB programObj, const GLcharARB *name);
#else
extern void glBindAttribLocationARB(GLhandleARB programObj, GLuint index, const GLcharARB *name);
extern void glGetActiveAttribARB(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
extern GLint glGetAttribLocationARB(GLhandleARB programObj, const GLcharARB *name);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ARB_vertex_buffer_object
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBindBufferARBProcPtr) (GLenum target, GLuint buffer);
typedef void (* glDeleteBuffersARBProcPtr) (GLsizei n, const GLuint *buffers);
typedef void (* glGenBuffersARBProcPtr) (GLsizei n, GLuint *buffers);
typedef GLboolean (* glIsBufferARBProcPtr) (GLuint buffer);
typedef void (* glBufferDataARBProcPtr) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
typedef void (* glBufferSubDataARBProcPtr) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
typedef void (* glGetBufferSubDataARBProcPtr) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
typedef GLvoid *(* glMapBufferARBProcPtr) (GLenum target, GLenum access);
typedef GLboolean (* glUnmapBufferARBProcPtr) (GLenum target);
typedef void (* glGetBufferParameterivARBProcPtr) (GLenum target, GLenum pname, GLint *params);
typedef void (* glGetBufferPointervARBProcPtr) (GLenum target, GLenum pname, GLvoid **params);
#else
extern void glBindBufferARB(GLenum target, GLuint buffer);
extern void glDeleteBuffersARB(GLsizei n, const GLuint *buffers);
extern void glGenBuffersARB(GLsizei n, GLuint *buffers);
extern GLboolean glIsBufferARB(GLuint buffer);
extern void glBufferDataARB(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
extern void glBufferSubDataARB(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
extern void glGetBufferSubDataARB(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
extern GLvoid *glMapBufferARB(GLenum target, GLenum access);
extern GLboolean glUnmapBufferARB(GLenum target);
extern void glGetBufferParameterivARB(GLenum target, GLenum pname, GLint *params);
extern void glGetBufferPointervARB(GLenum target, GLenum pname, GLvoid **params);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ARB_draw_buffers
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glDrawBuffersARBProcPtr) (GLsizei n, const GLenum *bufs);
#else
#endif /* GL_GLEXT_FUNCTION_POINTERS */
extern void glDrawBuffersARB(GLsizei n, const GLenum *bufs);
#endif

#if GL_ARB_color_buffer_float
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glClampColorARBProcPtr) (GLenum target, GLenum clamp);
#else
#endif /* GL_GLEXT_FUNCTION_POINTERS */
extern void glClampColorARB(GLenum target, GLenum clamp);
#endif

#if GL_ARB_instanced_arrays /*|| GL_ARB_draw_instanced*/
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glDrawArraysInstancedARBProcPtr) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);
typedef void (* glDrawElementsInstancedARBProcPtr) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);
#else /* GL_GLEXT_FUNCTION_POINTERS */
#endif
extern void glDrawArraysInstancedARB(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
extern void glDrawElementsInstancedARB(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);
#endif

#if GL_ARB_instanced_arrays
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glVertexAttribDivisorARBProcPtr) (GLuint index, GLuint divisor);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
extern void glVertexAttribDivisorARB(GLuint index, GLuint divisor);
#endif

#if GL_ARB_uniform_buffer_object
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef  void (* glGetUniformIndicesProcPtr) (GLuint program, GLsizei uniformCount, const GLchar** uniformNames, GLuint* uniformIndices);
typedef  void (* glGetActiveUniformsivProcPtr) (GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
typedef  void (* glGetActiveUniformNameProcPtr) (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);
typedef  GLuint (* glGetUniformBlockIndexProcPtr) (GLuint program, const GLchar* uniformBlockName);
typedef  void (* glGetActiveUniformBlockivProcPtr) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
typedef  void (* glGetActiveUniformBlockNameProcPtr) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
typedef  void (* glBindBufferRangeProcPtr) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef  void (* glBindBufferBaseProcPtr) (GLenum target, GLuint index, GLuint buffer);
typedef  void (* glGetIntegeri_vProcPtr) (GLenum pname, GLuint index, GLint* data);
typedef  void (* glUniformBlockBindingProcPtr) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
#else
extern void glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar** uniformNames, GLuint* uniformIndices);
extern void glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
extern void glGetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);
extern GLuint glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName);
extern void glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
extern void glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
extern void glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
extern void glGetIntegeri_v(GLenum pname, GLuint index, GLint* data);
extern void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_blend_color
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBlendColorEXTProcPtr) (GLclampf, GLclampf, GLclampf, GLclampf);
#else
extern void glBlendColorEXT (GLclampf, GLclampf, GLclampf, GLclampf);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_polygon_offset
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glPolygonOffsetEXTProcPtr) (GLfloat, GLfloat);
#else
extern void glPolygonOffsetEXT (GLfloat, GLfloat);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_texture3D
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glTexImage3DEXTProcPtr) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
typedef void (* glTexSubImage3DEXTProcPtr) (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
#else
extern void glTexImage3DEXT (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
extern void glTexSubImage3DEXT (GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_subtexture
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glTexSubImage1DEXTProcPtr) (GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *);
typedef void (* glTexSubImage2DEXTProcPtr) (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
#else
extern void glTexSubImage1DEXT (GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *);
extern void glTexSubImage2DEXT (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_copy_texture
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glCopyTexImage1DEXTProcPtr) (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint);
typedef void (* glCopyTexImage2DEXTProcPtr) (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
typedef void (* glCopyTexSubImage1DEXTProcPtr) (GLenum, GLint, GLint, GLint, GLint, GLsizei);
typedef void (* glCopyTexSubImage2DEXTProcPtr) (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
typedef void (* glCopyTexSubImage3DEXTProcPtr) (GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
#else
extern void glCopyTexImage1DEXT (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint);
extern void glCopyTexImage2DEXT (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
extern void glCopyTexSubImage1DEXT (GLenum, GLint, GLint, GLint, GLint, GLsizei);
extern void glCopyTexSubImage2DEXT (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
extern void glCopyTexSubImage3DEXT (GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_histogram
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glGetHistogramEXTProcPtr) (GLenum, GLboolean, GLenum, GLenum, GLvoid *);
typedef void (* glGetHistogramParameterfvEXTProcPtr) (GLenum, GLenum, GLfloat *);
typedef void (* glGetHistogramParameterivEXTProcPtr) (GLenum, GLenum, GLint *);
typedef void (* glGetMinmaxEXTProcPtr) (GLenum, GLboolean, GLenum, GLenum, GLvoid *);
typedef void (* glGetMinmaxParameterfvEXTProcPtr) (GLenum, GLenum, GLfloat *);
typedef void (* glGetMinmaxParameterivEXTProcPtr) (GLenum, GLenum, GLint *);
typedef void (* glHistogramEXTProcPtr) (GLenum, GLsizei, GLenum, GLboolean);
typedef void (* glMinmaxEXTProcPtr) (GLenum, GLenum, GLboolean);
typedef void (* glResetHistogramEXTProcPtr) (GLenum);
typedef void (* glResetMinmaxEXTProcPtr) (GLenum);
#else
extern void glGetHistogramEXT (GLenum, GLboolean, GLenum, GLenum, GLvoid *);
extern void glGetHistogramParameterfvEXT (GLenum, GLenum, GLfloat *);
extern void glGetHistogramParameterivEXT (GLenum, GLenum, GLint *);
extern void glGetMinmaxEXT (GLenum, GLboolean, GLenum, GLenum, GLvoid *);
extern void glGetMinmaxParameterfvEXT (GLenum, GLenum, GLfloat *);
extern void glGetMinmaxParameterivEXT (GLenum, GLenum, GLint *);
extern void glHistogramEXT (GLenum, GLsizei, GLenum, GLboolean);
extern void glMinmaxEXT (GLenum, GLenum, GLboolean);
extern void glResetHistogramEXT (GLenum);
extern void glResetMinmaxEXT (GLenum);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_convolution
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glConvolutionFilter1DEXTProcPtr) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
typedef void (* glConvolutionFilter2DEXTProcPtr) (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
typedef void (* glConvolutionParameterfEXTProcPtr) (GLenum, GLenum, GLfloat);
typedef void (* glConvolutionParameterfvEXTProcPtr) (GLenum, GLenum, const GLfloat *);
typedef void (* glConvolutionParameteriEXTProcPtr) (GLenum, GLenum, GLint);
typedef void (* glConvolutionParameterivEXTProcPtr) (GLenum, GLenum, const GLint *);
typedef void (* glCopyConvolutionFilter1DEXTProcPtr) (GLenum, GLenum, GLint, GLint, GLsizei);
typedef void (* glCopyConvolutionFilter2DEXTProcPtr) (GLenum, GLenum, GLint, GLint, GLsizei, GLsizei);
typedef void (* glGetConvolutionFilterEXTProcPtr) (GLenum, GLenum, GLenum, GLvoid *);
typedef void (* glGetConvolutionParameterfvEXTProcPtr) (GLenum, GLenum, GLfloat *);
typedef void (* glGetConvolutionParameterivEXTProcPtr) (GLenum, GLenum, GLint *);
typedef void (* glGetSeparableFilterEXTProcPtr) (GLenum, GLenum, GLenum, GLvoid *, GLvoid *, GLvoid *);
typedef void (* glSeparableFilter2DEXTProcPtr) (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *, const GLvoid *);
#else
extern void glConvolutionFilter1DEXT (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
extern void glConvolutionFilter2DEXT (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
extern void glConvolutionParameterfEXT (GLenum, GLenum, GLfloat);
extern void glConvolutionParameterfvEXT (GLenum, GLenum, const GLfloat *);
extern void glConvolutionParameteriEXT (GLenum, GLenum, GLint);
extern void glConvolutionParameterivEXT (GLenum, GLenum, const GLint *);
extern void glCopyConvolutionFilter1DEXT (GLenum, GLenum, GLint, GLint, GLsizei);
extern void glCopyConvolutionFilter2DEXT (GLenum, GLenum, GLint, GLint, GLsizei, GLsizei);
extern void glGetConvolutionFilterEXT (GLenum, GLenum, GLenum, GLvoid *);
extern void glGetConvolutionParameterfvEXT (GLenum, GLenum, GLfloat *);
extern void glGetConvolutionParameterivEXT (GLenum, GLenum, GLint *);
extern void glGetSeparableFilterEXT (GLenum, GLenum, GLenum, GLvoid *, GLvoid *, GLvoid *);
extern void glSeparableFilter2DEXT (GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_texture_object
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef GLboolean (* glAreTexturesResidentEXTProcPtr) (GLsizei, const GLuint *, GLboolean *);
typedef void (* glBindTextureEXTProcPtr) (GLenum, GLuint);
typedef void (* glDeleteTexturesEXTProcPtr) (GLsizei, const GLuint *);
typedef void (* glGenTexturesEXTProcPtr) (GLsizei, GLuint *);
typedef GLboolean (* glIsTextureEXTProcPtr) (GLuint);
typedef void (* glPrioritizeTexturesEXTProcPtr) (GLsizei, const GLuint *, const GLclampf *);
#else
extern GLboolean glAreTexturesResidentEXT (GLsizei, const GLuint *, GLboolean *);
extern void glBindTextureEXT (GLenum, GLuint);
extern void glDeleteTexturesEXT (GLsizei, const GLuint *);
extern void glGenTexturesEXT (GLsizei, GLuint *);
extern GLboolean glIsTextureEXT (GLuint);
extern void glPrioritizeTexturesEXT (GLsizei, const GLuint *, const GLclampf *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_vertex_array
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glArrayElementEXTProcPtr) (GLint);
typedef void (* glColorPointerEXTProcPtr) (GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
typedef void (* glDrawArraysEXTProcPtr) (GLenum, GLint, GLsizei);
typedef void (* glEdgeFlagPointerEXTProcPtr) (GLsizei, GLsizei, const GLvoid *);
typedef void (* glGetPointervEXTProcPtr) (GLenum, GLvoid* *);
typedef void (* glIndexPointerEXTProcPtr) (GLenum, GLsizei, GLsizei, const GLvoid *);
typedef void (* glNormalPointerEXTProcPtr) (GLenum, GLsizei, GLsizei, const GLvoid *);
typedef void (* glTexCoordPointerEXTProcPtr) (GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
typedef void (* glVertexPointerEXTProcPtr) (GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
#else
extern void glArrayElementEXT (GLint);
extern void glColorPointerEXT (GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
extern void glDrawArraysEXT (GLenum, GLint, GLsizei);
extern void glEdgeFlagPointerEXT (GLsizei, GLsizei, const GLvoid *);
extern void glGetPointervEXT (GLenum, GLvoid* *);
extern void glIndexPointerEXT (GLenum, GLsizei, GLsizei, const GLvoid *);
extern void glNormalPointerEXT (GLenum, GLsizei, GLsizei, const GLvoid *);
extern void glTexCoordPointerEXT (GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
extern void glVertexPointerEXT (GLint, GLenum, GLsizei, GLsizei, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_blend_minmax
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBlendEquationEXTProcPtr) (GLenum);
#else
extern void glBlendEquationEXT (GLenum);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_color_subtable
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glColorSubTableEXTProcPtr) (GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
typedef void (* glCopyColorSubTableEXTProcPtr) (GLenum, GLsizei, GLint, GLint, GLsizei);
#else
extern void glColorSubTableEXT (GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
extern void glCopyColorSubTableEXT (GLenum, GLsizei, GLint, GLint, GLsizei);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_paletted_texture
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glColorTableEXTProcPtr) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
typedef void (* glColorSubTableEXTProcPtr) (GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
typedef void (* glGetColorTableEXTProcPtr) (GLenum, GLenum, GLenum, GLvoid *);
typedef void (* glGetColorTableParameterivEXTProcPtr) (GLenum, GLenum, GLint *);
typedef void (* glGetColorTableParameterfvEXTProcPtr) (GLenum, GLenum, GLfloat *);
#else
extern void glColorTableEXT (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
extern void glColorSubTableEXT (GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
extern void glGetColorTableEXT (GLenum, GLenum, GLenum, GLvoid *);
extern void glGetColorTableParameterivEXT (GLenum, GLenum, GLint *);
extern void glGetColorTableParameterfvEXT (GLenum, GLenum, GLfloat *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_index_material
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glIndexMaterialEXTProcPtr) (GLenum, GLenum);
#else
extern void glIndexMaterialEXT (GLenum, GLenum);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_index_func
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glIndexFuncEXTProcPtr) (GLenum, GLclampf);
#else
extern void glIndexFuncEXT (GLenum, GLclampf);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_compiled_vertex_array
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glLockArraysEXTProcPtr) (GLint, GLsizei);
typedef void (* glUnlockArraysEXTProcPtr) (void);
#else
extern void glLockArraysEXT (GLint, GLsizei);
extern void glUnlockArraysEXT (void);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_cull_vertex
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glCullParameterdvEXTProcPtr) (GLenum, GLdouble *);
typedef void (* glCullParameterfvEXTProcPtr) (GLenum, GLfloat *);
#else
extern void glCullParameterdvEXT (GLenum, GLdouble *);
extern void glCullParameterfvEXT (GLenum, GLfloat *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_draw_range_elements
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glDrawRangeElementsEXTProcPtr) (GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *);
#else
extern void glDrawRangeElementsEXT (GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_light_texture
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glApplyTextureEXTProcPtr) (GLenum);
typedef void (* glTextureLightEXTProcPtr) (GLenum);
typedef void (* glTextureMaterialEXTProcPtr) (GLenum, GLenum);
#else
extern void glApplyTextureEXT (GLenum);
extern void glTextureLightEXT (GLenum);
extern void glTextureMaterialEXT (GLenum, GLenum);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_pixel_transform
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glPixelTransformParameteriEXTProcPtr) (GLenum, GLenum, GLint);
typedef void (* glPixelTransformParameterfEXTProcPtr) (GLenum, GLenum, GLfloat);
typedef void (* glPixelTransformParameterivEXTProcPtr) (GLenum, GLenum, const GLint *);
typedef void (* glPixelTransformParameterfvEXTProcPtr) (GLenum, GLenum, const GLfloat *);
#else
extern void glPixelTransformParameteriEXT (GLenum, GLenum, GLint);
extern void glPixelTransformParameterfEXT (GLenum, GLenum, GLfloat);
extern void glPixelTransformParameterivEXT (GLenum, GLenum, const GLint *);
extern void glPixelTransformParameterfvEXT (GLenum, GLenum, const GLfloat *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_secondary_color
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glSecondaryColor3bEXTProcPtr) (GLbyte, GLbyte, GLbyte);
typedef void (* glSecondaryColor3bvEXTProcPtr) (const GLbyte *);
typedef void (* glSecondaryColor3dEXTProcPtr) (GLdouble, GLdouble, GLdouble);
typedef void (* glSecondaryColor3dvEXTProcPtr) (const GLdouble *);
typedef void (* glSecondaryColor3fEXTProcPtr) (GLfloat, GLfloat, GLfloat);
typedef void (* glSecondaryColor3fvEXTProcPtr) (const GLfloat *);
typedef void (* glSecondaryColor3iEXTProcPtr) (GLint, GLint, GLint);
typedef void (* glSecondaryColor3ivEXTProcPtr) (const GLint *);
typedef void (* glSecondaryColor3sEXTProcPtr) (GLshort, GLshort, GLshort);
typedef void (* glSecondaryColor3svEXTProcPtr) (const GLshort *);
typedef void (* glSecondaryColor3ubEXTProcPtr) (GLubyte, GLubyte, GLubyte);
typedef void (* glSecondaryColor3ubvEXTProcPtr) (const GLubyte *);
typedef void (* glSecondaryColor3uiEXTProcPtr) (GLuint, GLuint, GLuint);
typedef void (* glSecondaryColor3uivEXTProcPtr) (const GLuint *);
typedef void (* glSecondaryColor3usEXTProcPtr) (GLushort, GLushort, GLushort);
typedef void (* glSecondaryColor3usvEXTProcPtr) (const GLushort *);
typedef void (* glSecondaryColorPointerEXTProcPtr) (GLint, GLenum, GLsizei, const GLvoid *);
#else
extern void glSecondaryColor3bEXT (GLbyte, GLbyte, GLbyte);
extern void glSecondaryColor3bvEXT (const GLbyte *);
extern void glSecondaryColor3dEXT (GLdouble, GLdouble, GLdouble);
extern void glSecondaryColor3dvEXT (const GLdouble *);
extern void glSecondaryColor3fEXT (GLfloat, GLfloat, GLfloat);
extern void glSecondaryColor3fvEXT (const GLfloat *);
extern void glSecondaryColor3iEXT (GLint, GLint, GLint);
extern void glSecondaryColor3ivEXT (const GLint *);
extern void glSecondaryColor3sEXT (GLshort, GLshort, GLshort);
extern void glSecondaryColor3svEXT (const GLshort *);
extern void glSecondaryColor3ubEXT (GLubyte, GLubyte, GLubyte);
extern void glSecondaryColor3ubvEXT (const GLubyte *);
extern void glSecondaryColor3uiEXT (GLuint, GLuint, GLuint);
extern void glSecondaryColor3uivEXT (const GLuint *);
extern void glSecondaryColor3usEXT (GLushort, GLushort, GLushort);
extern void glSecondaryColor3usvEXT (const GLushort *);
extern void glSecondaryColorPointerEXT (GLint, GLenum, GLsizei, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_texture_perturb_normal
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glTextureNormalEXTProcPtr) (GLenum);
#else
extern void glTextureNormalEXT (GLenum);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_multi_draw_arrays
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glMultiDrawArraysEXTProcPtr) (GLenum, const GLint *, const GLsizei *, GLsizei);
typedef void (* glMultiDrawElementsEXTProcPtr) (GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei);
#else
extern void glMultiDrawArraysEXT (GLenum, const GLint *, const GLsizei *, GLsizei);
extern void glMultiDrawElementsEXT (GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_fog_coord
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glFogCoordfEXTProcPtr) (GLfloat);
typedef void (* glFogCoordfvEXTProcPtr) (const GLfloat *);
typedef void (* glFogCoorddEXTProcPtr) (GLdouble);
typedef void (* glFogCoorddvEXTProcPtr) (const GLdouble *);
typedef void (* glFogCoordPointerEXTProcPtr) (GLenum, GLsizei, const GLvoid *);
#else
extern void glFogCoordfEXT (GLfloat);
extern void glFogCoordfvEXT (const GLfloat *);
extern void glFogCoorddEXT (GLdouble);
extern void glFogCoorddvEXT (const GLdouble *);
extern void glFogCoordPointerEXT (GLenum, GLsizei, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_coordinate_frame
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glTangent3bEXTProcPtr) (GLbyte, GLbyte, GLbyte);
typedef void (* glTangent3bvEXTProcPtr) (const GLbyte *);
typedef void (* glTangent3dEXTProcPtr) (GLdouble, GLdouble, GLdouble);
typedef void (* glTangent3dvEXTProcPtr) (const GLdouble *);
typedef void (* glTangent3fEXTProcPtr) (GLfloat, GLfloat, GLfloat);
typedef void (* glTangent3fvEXTProcPtr) (const GLfloat *);
typedef void (* glTangent3iEXTProcPtr) (GLint, GLint, GLint);
typedef void (* glTangent3ivEXTProcPtr) (const GLint *);
typedef void (* glTangent3sEXTProcPtr) (GLshort, GLshort, GLshort);
typedef void (* glTangent3svEXTProcPtr) (const GLshort *);
typedef void (* glBinormal3bEXTProcPtr) (GLbyte, GLbyte, GLbyte);
typedef void (* glBinormal3bvEXTProcPtr) (const GLbyte *);
typedef void (* glBinormal3dEXTProcPtr) (GLdouble, GLdouble, GLdouble);
typedef void (* glBinormal3dvEXTProcPtr) (const GLdouble *);
typedef void (* glBinormal3fEXTProcPtr) (GLfloat, GLfloat, GLfloat);
typedef void (* glBinormal3fvEXTProcPtr) (const GLfloat *);
typedef void (* glBinormal3iEXTProcPtr) (GLint, GLint, GLint);
typedef void (* glBinormal3ivEXTProcPtr) (const GLint *);
typedef void (* glBinormal3sEXTProcPtr) (GLshort, GLshort, GLshort);
typedef void (* glBinormal3svEXTProcPtr) (const GLshort *);
typedef void (* glTangentPointerEXTProcPtr) (GLenum, GLsizei, const GLvoid *);
typedef void (* glBinormalPointerEXTProcPtr) (GLenum, GLsizei, const GLvoid *);
#else
extern void glTangent3bEXT (GLbyte, GLbyte, GLbyte);
extern void glTangent3bvEXT (const GLbyte *);
extern void glTangent3dEXT (GLdouble, GLdouble, GLdouble);
extern void glTangent3dvEXT (const GLdouble *);
extern void glTangent3fEXT (GLfloat, GLfloat, GLfloat);
extern void glTangent3fvEXT (const GLfloat *);
extern void glTangent3iEXT (GLint, GLint, GLint);
extern void glTangent3ivEXT (const GLint *);
extern void glTangent3sEXT (GLshort, GLshort, GLshort);
extern void glTangent3svEXT (const GLshort *);
extern void glBinormal3bEXT (GLbyte, GLbyte, GLbyte);
extern void glBinormal3bvEXT (const GLbyte *);
extern void glBinormal3dEXT (GLdouble, GLdouble, GLdouble);
extern void glBinormal3dvEXT (const GLdouble *);
extern void glBinormal3fEXT (GLfloat, GLfloat, GLfloat);
extern void glBinormal3fvEXT (const GLfloat *);
extern void glBinormal3iEXT (GLint, GLint, GLint);
extern void glBinormal3ivEXT (const GLint *);
extern void glBinormal3sEXT (GLshort, GLshort, GLshort);
extern void glBinormal3svEXT (const GLshort *);
extern void glTangentPointerEXT (GLenum, GLsizei, const GLvoid *);
extern void glBinormalPointerEXT (GLenum, GLsizei, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_blend_func_separate
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBlendFuncSeparateEXTProcPtr) (GLenum, GLenum, GLenum, GLenum);
#else
extern void glBlendFuncSeparateEXT (GLenum, GLenum, GLenum, GLenum);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_vertex_weighting
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glVertexWeightfEXTProcPtr) (GLfloat);
typedef void (* glVertexWeightfvEXTProcPtr) (const GLfloat *);
typedef void (* glVertexWeightPointerEXTProcPtr) (GLsizei, GLenum, GLsizei, const GLvoid *);
#else
extern void glVertexWeightfEXT (GLfloat);
extern void glVertexWeightfvEXT (const GLfloat *);
extern void glVertexWeightPointerEXT (GLsizei, GLenum, GLsizei, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_vertex_shader
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBeginVertexShaderEXTProcPtr) (void);
typedef void (* glEndVertexShaderEXTProcPtr) (void);
typedef void (* glBindVertexShaderEXTProcPtr) (GLuint id);
typedef GLuint (* glGenVertexShadersEXTProcPtr) (GLuint range);
typedef void (* glDeleteVertexShaderEXTProcPtr) (GLuint id);
typedef void (* glShaderOp1EXTProcPtr) (GLenum op, GLuint res, GLuint arg1);
typedef void (* glShaderOp2EXTProcPtr) (GLenum op, GLuint res, GLuint arg1, GLuint arg2);
typedef void (* glShaderOp3EXTProcPtr) (GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
typedef void (* glSwizzleEXTProcPtr) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
typedef void (* glWriteMaskEXTProcPtr) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
typedef void (* glInsertComponentEXTProcPtr) (GLuint res, GLuint src, GLuint num);
typedef void (* glExtractComponentEXTProcPtr) (GLuint res, GLuint src, GLuint num);
typedef GLuint (* glGenSymbolsEXTProcPtr) (GLenum datatype, GLenum storagetype, GLenum range, GLuint components);
typedef void (* glSetInvariantEXTProcPtr) (GLuint id, GLenum type, GLvoid *addr);
typedef void (* glSetLocalConstantEXTProcPtr) (GLuint id, GLenum type, GLvoid *addr);
typedef void (* glVariantbvEXTProcPtr) (GLuint id, GLbyte *addr);
typedef void (* glVariantdvEXTProcPtr) (GLuint id, GLdouble *addr);
typedef void (* glVariantfvEXTProcPtr) (GLuint id, GLfloat *addr);
typedef void (* glVariantivEXTProcPtr) (GLuint id, GLint *addr);
typedef void (* glVariantsvEXTProcPtr) (GLuint id, GLshort *addr);
typedef void (* glVariantubvEXTProcPtr) (GLuint id, GLubyte *addr);
typedef void (* glVariantuivEXTProcPtr) (GLuint id, GLuint *addr);
typedef void (* glVariantusvEXTProcPtr) (GLuint id, GLushort *addr);
typedef void (* glVariantPointerEXTProcPtr) (GLuint id, GLenum type, GLuint stride, GLvoid *addr);
typedef void (* glEnableVariantClientStateEXTProcPtr) (GLuint id);
typedef void (* glDisableVariantClientStateEXTProcPtr) (GLuint id);
typedef GLuint (* glBindLightParameterEXTProcPtr) (GLenum light, GLenum value);
typedef GLuint (* glBindMaterialParameterEXTProcPtr) (GLenum face, GLenum value);
typedef GLuint (* glBindTexGenParameterEXTProcPtr) (GLenum unit, GLenum coord, GLenum value);
typedef GLuint (* glBindTextureUnitParameterEXTProcPtr) (GLenum unit, GLenum value);
typedef GLuint (* glBindParameterEXTProcPtr) (GLenum value);
typedef GLboolean (* glIsVariantEnabledEXTProcPtr) (GLuint id, GLenum cap);
typedef void (* glGetVariantBooleanvEXTProcPtr) (GLuint id, GLenum value, GLboolean *data);
typedef void (* glGetVariantIntegervEXTProcPtr) (GLuint id, GLenum value, GLint *data);
typedef void (* glGetVariantFloatvEXTProcPtr) (GLuint id, GLenum value, GLfloat *data);
typedef void (* glGetVariantPointervEXTProcPtr) (GLuint id, GLenum value, GLvoid **data);
typedef void (* glGetInvariantBooleanvEXTProcPtr) (GLuint id, GLenum value, GLboolean *data);
typedef void (* glGetInvariantIntegervEXTProcPtr) (GLuint id, GLenum value, GLint *data);
typedef void (* glGetInvariantFloatvEXTProcPtr) (GLuint id, GLenum value, GLfloat *data);
typedef void (* glGetLocalConstantBooleanvEXTProcPtr) (GLuint id, GLenum value, GLboolean *data);
typedef void (* glGetLocalConstantIntegervEXTProcPtr) (GLuint id, GLenum value, GLint *data);
typedef void (* glGetLocalConstantFloatvEXTProcPtr) (GLuint id, GLenum value, GLfloat *data);
#else
extern void glBeginVertexShaderEXT (void);
extern void glEndVertexShaderEXT (void);
extern void glBindVertexShaderEXT (GLuint id);
extern GLuint glGenVertexShadersEXT (GLuint range);
extern void glDeleteVertexShaderEXT (GLuint id);
extern void glShaderOp1EXT (GLenum op, GLuint res, GLuint arg1);
extern void glShaderOp2EXT (GLenum op, GLuint res, GLuint arg1, GLuint arg2);
extern void glShaderOp3EXT (GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
extern void glSwizzleEXT (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
extern void glWriteMaskEXT (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
extern void glInsertComponentEXT (GLuint res, GLuint src, GLuint num);
extern void glExtractComponentEXT (GLuint res, GLuint src, GLuint num);
extern GLuint glGenSymbolsEXT (GLenum datatype, GLenum storagetype, GLenum range, GLuint components);
extern void glSetInvariantEXT (GLuint id, GLenum type, GLvoid *addr);
extern void glSetLocalConstantEXT (GLuint id, GLenum type, GLvoid *addr);
extern void glVariantbvEXT (GLuint id, GLbyte *addr);
extern void glVariantdvEXT (GLuint id, GLdouble *addr);
extern void glVariantfvEXT (GLuint id, GLfloat *addr);
extern void glVariantivEXT (GLuint id, GLint *addr);
extern void glVariantsvEXT (GLuint id, GLshort *addr);
extern void glVariantubvEXT (GLuint id, GLubyte *addr);
extern void glVariantuivEXT (GLuint id, GLuint *addr);
extern void glVariantusvEXT (GLuint id, GLushort *addr);
extern void glVariantPointerEXT (GLuint id, GLenum type, GLuint stride, GLvoid *addr);
extern void glEnableVariantClientStateEXT (GLuint id);
extern void glDisableVariantClientStateEXT (GLuint id);
extern GLuint glBindLightParameterEXT (GLenum light, GLenum value);
extern GLuint glBindMaterialParameterEXT (GLenum face, GLenum value);
extern GLuint glBindTexGenParameterEXT (GLenum unit, GLenum coord, GLenum value);
extern GLuint glBindTextureUnitParameterEXT (GLenum unit, GLenum value);
extern GLuint glBindParameterEXT (GLenum value);
extern GLboolean glIsVariantEnabledEXT (GLuint id, GLenum cap);
extern void glGetVariantBooleanvEXT (GLuint id, GLenum value, GLboolean *data);
extern void glGetVariantIntegervEXT (GLuint id, GLenum value, GLint *data);
extern void glGetVariantFloatvEXT (GLuint id, GLenum value, GLfloat *data);
extern void glGetVariantPointervEXT (GLuint id, GLenum value, GLvoid **data);
extern void glGetInvariantBooleanvEXT (GLuint id, GLenum value, GLboolean *data);
extern void glGetInvariantIntegervEXT (GLuint id, GLenum value, GLint *data);
extern void glGetInvariantFloatvEXT (GLuint id, GLenum value, GLfloat *data);
extern void glGetLocalConstantBooleanvEXT (GLuint id, GLenum value, GLboolean *data);
extern void glGetLocalConstantIntegervEXT (GLuint id, GLenum value, GLint *data);
extern void glGetLocalConstantFloatvEXT (GLuint id, GLenum value, GLfloat *data);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_fragment_shader
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef GLuint (* glGenFragmentShadersEXTProcPtr) (GLuint range);
typedef void (* glBindFragmentShaderEXTProcPtr) (GLuint id);
typedef void (* glDeleteFragmentShaderEXTProcPtr) (GLuint id);
typedef void (* glBeginFragmentShaderEXTProcPtr) (void);
typedef void (* glEndFragmentShaderEXTProcPtr) (void);
typedef void (* glPassTexCoordEXTProcPtr) (GLuint dst, GLuint coord, GLenum swizzle);
typedef void (* glSampleMapEXTProcPtr) (GLuint dst, GLuint interp, GLenum swizzle);
typedef void (* glColorFragmentOp1EXTProcPtr) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
typedef void (* glColorFragmentOp2EXTProcPtr) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
typedef void (* glColorFragmentOp3EXTProcPtr) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
typedef void (* glAlphaFragmentOp1EXTProcPtr) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
typedef void (* glAlphaFragmentOp2EXTProcPtr) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
typedef void (* glAlphaFragmentOp3EXTProcPtr) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
typedef void (* glSetFragmentShaderConstantEXTProcPtr) (GLuint dst, const GLfloat *value);
#else
extern GLuint glGenFragmentShadersEXT (GLuint range);
extern void glBindFragmentShaderEXT (GLuint id);
extern void glDeleteFragmentShaderEXT (GLuint id);
extern void glBeginFragmentShaderEXT (void);
extern void glEndFragmentShaderEXT (void);
extern void glPassTexCoordEXT (GLuint dst, GLuint coord, GLenum swizzle);
extern void glSampleMapEXT (GLuint dst, GLuint interp, GLenum swizzle);
extern void glColorFragmentOp1EXT (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
extern void glColorFragmentOp2EXT (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
extern void glColorFragmentOp3EXT (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
extern void glAlphaFragmentOp1EXT (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
extern void glAlphaFragmentOp2EXT (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
extern void glAlphaFragmentOp3EXT (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
extern void glSetFragmentShaderConstantEXT (GLuint dst, const GLfloat *value);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_multisample
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glSampleMaskEXTProcPtr) (GLclampf, GLboolean);
typedef void (* glSamplePatternEXTProcPtr) (GLenum);
#else
extern void glSampleMaskEXT (GLclampf, GLboolean);
extern void glSamplePatternEXT (GLenum);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_stencil_two_side
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glActiveStencilFaceEXTProcPtr) (GLenum face);
#else
extern void glActiveStencilFaceEXT(GLenum face);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_depth_bounds_test
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glDepthBoundsEXTProcPtr) (GLclampd zmin, GLclampd zmax);
#else
extern void glDepthBoundsEXT(GLclampd zmin, GLclampd zmax);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_blend_equation_separate
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBlendEquationSeparateEXTProcPtr) (GLenum modeRGB, GLenum modeAlpha);
#else
extern void glBlendEquationSeparateEXT(GLenum modeRGB, GLenum modeAlpha);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_framebuffer_object
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef GLboolean (* glIsRenderbufferEXTProcPtr) (GLuint renderbuffer);
typedef void (* glBindRenderbufferEXTProcPtr) (GLenum target, GLuint renderbuffer);
typedef void (* glDeleteRenderbuffersEXTProcPtr) (GLsizei n, const GLuint *renderbuffers);
typedef void (* glGenRenderbuffersEXTProcPtr) (GLsizei n, GLuint *renderbuffers);
typedef void (* glRenderbufferStorageEXTProcPtr) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (* glGetRenderbufferParameterivEXTProcPtr) (GLenum target, GLenum pname, GLint *params);
typedef GLboolean (* glIsFramebufferEXTProcPtr) (GLuint framebuffer);
typedef void (* glBindFramebufferEXTProcPtr) (GLenum target, GLuint framebuffer);
typedef void (* glDeleteFramebuffersEXTProcPtr) (GLsizei n, const GLuint *framebuffers);
typedef void (* glGenFramebuffersEXTProcPtr) (GLsizei n, GLuint *framebuffers);
typedef GLenum (* glCheckFramebufferStatusEXTProcPtr) (GLenum target);
typedef void (* glFramebufferTexture1DEXTProcPtr) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (* glFramebufferTexture2DEXTProcPtr) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (* glFramebufferTexture3DEXTProcPtr) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (* glFramebufferRenderbufferEXTProcPtr) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (* glGetFramebufferAttachmentParameterivEXTProcPtr) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
typedef void (* glGenerateMipmapEXTProcPtr) (GLenum target);
#else
extern GLboolean glIsRenderbufferEXT(GLuint renderbuffer);
extern void glBindRenderbufferEXT(GLenum target, GLuint renderbuffer);
extern void glDeleteRenderbuffersEXT(GLsizei n, const GLuint *renderbuffers);
extern void glGenRenderbuffersEXT(GLsizei n, GLuint *renderbuffers);
extern void glRenderbufferStorageEXT(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern void glGetRenderbufferParameterivEXT(GLenum target, GLenum pname, GLint *params);
extern GLboolean glIsFramebufferEXT(GLuint framebuffer);
extern void glBindFramebufferEXT(GLenum target, GLuint framebuffer);
extern void glDeleteFramebuffersEXT(GLsizei n, const GLuint *framebuffers);
extern void glGenFramebuffersEXT(GLsizei n, GLuint *framebuffers);
extern GLenum glCheckFramebufferStatusEXT(GLenum target);
extern void glFramebufferTexture1DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void glFramebufferTexture2DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void glFramebufferTexture3DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
extern void glFramebufferRenderbufferEXT(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void glGetFramebufferAttachmentParameterivEXT(GLenum target, GLenum attachment, GLenum pname, GLint *params);
extern void glGenerateMipmapEXT(GLenum target);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_framebuffer_blit
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBlitFramebufferEXTProcPtr) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
#else
extern void glBlitFramebufferEXT(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_framebuffer_multisample
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glRenderbufferStorageMultisampleEXTProcPtr) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
#else
extern void glRenderbufferStorageMultisampleEXT(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_geometry_shader4
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glProgramParameteriEXTProcPtr) (GLuint program, GLenum pname, GLint value);
typedef void (* glFramebufferTextureEXTProcPtr) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (* glFramebufferTextureFaceEXTProcPtr) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
#else
extern void glProgramParameteriEXT(GLuint program, GLenum pname, GLint value);
extern void glFramebufferTextureEXT(GLenum target, GLenum attachment, GLuint texture, GLint level);
extern void glFramebufferTextureFaceEXT(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_geometry_shader4 || GL_EXT_texture_array
#ifdef GL_GLEXT_FUNCTION_POINTER
typedef void (* glFramebufferTextureLayerEXTProcPtr) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
#else
extern void glFramebufferTextureLayerEXT(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
#endif
#endif

#if GL_ARB_framebuffer_object
#ifdef GL_GLEXT_FUNCTION_POINTERS
extern GLboolean (* glIsRenderbufferProcPtr) (GLuint);
extern void (* glBindRenderbufferProcPtr) (GLenum, GLuint);
extern void (* glDeleteRenderbuffersProcPtr) (GLsizei, const GLuint *);
extern void (* glGenRenderbuffersProcPtr) (GLsizei, GLuint *);
extern void (* glRenderbufferStorageProcPtr) (GLenum, GLenum, GLsizei, GLsizei);
extern void (* glGetRenderbufferParameterivProcPtr) (GLenum, GLenum, GLint *);
extern GLboolean (* glIsFramebufferProcPtr) (GLuint);
extern void (* glBindFramebufferProcPtr) (GLenum, GLuint);
extern void (* glDeleteFramebuffersProcPtr) (GLsizei, const GLuint *);
extern void (* glGenFramebuffersProcPtr) (GLsizei, GLuint *);
extern GLenum (* glCheckFramebufferStatusProcPtr) (GLenum);
extern void (* glFramebufferTexture1DProcPtr) (GLenum, GLenum, GLenum, GLuint, GLint);
extern void (* glFramebufferTexture2DProcPtr) (GLenum, GLenum, GLenum, GLuint, GLint);
extern void (* glFramebufferTexture3DProcPtr) (GLenum, GLenum, GLenum, GLuint, GLint, GLint);
extern void (* glFramebufferRenderbufferProcPtr) (GLenum, GLenum, GLenum, GLuint);
extern void (* glGetFramebufferAttachmentParameterivProcPtr) (GLenum, GLenum, GLenum, GLint *);
extern void (* glGenerateMipmapProcPtr) (GLenum);
extern void (* glBlitFramebufferProcPtr) (GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
extern void (* glRenderbufferStorageMultisampleProcPtr) (GLenum, GLsizei, GLenum, GLsizei, GLsizei);
extern void (* glFramebufferTextureLayerProcPtr) (GLenum, GLenum, GLuint, GLint, GLint);
#else
extern GLboolean glIsRenderbuffer (GLuint);
extern void glBindRenderbuffer (GLenum, GLuint);
extern void glDeleteRenderbuffers (GLsizei, const GLuint *);
extern void glGenRenderbuffers (GLsizei, GLuint *);
extern void glRenderbufferStorage (GLenum, GLenum, GLsizei, GLsizei);
extern void glGetRenderbufferParameteriv (GLenum, GLenum, GLint *);
extern GLboolean glIsFramebuffer (GLuint);
extern void glBindFramebuffer (GLenum, GLuint);
extern void glDeleteFramebuffers (GLsizei, const GLuint *);
extern void glGenFramebuffers (GLsizei, GLuint *);
extern GLenum glCheckFramebufferStatus (GLenum);
extern void glFramebufferTexture1D (GLenum, GLenum, GLenum, GLuint, GLint);
extern void glFramebufferTexture2D (GLenum, GLenum, GLenum, GLuint, GLint);
extern void glFramebufferTexture3D (GLenum, GLenum, GLenum, GLuint, GLint, GLint);
extern void glFramebufferRenderbuffer (GLenum, GLenum, GLenum, GLuint);
extern void glGetFramebufferAttachmentParameteriv (GLenum, GLenum, GLenum, GLint *);
extern void glGenerateMipmap (GLenum);
extern void glBlitFramebuffer (GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
extern void glRenderbufferStorageMultisample (GLenum, GLsizei, GLenum, GLsizei, GLsizei);
extern void glFramebufferTextureLayer (GLenum, GLenum, GLuint, GLint, GLint);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif
    
#if GL_EXT_transform_feedback
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBindBufferRangeEXTProcPtr) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (* glBindBufferOffsetEXTProcPtr) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
typedef void (* glBindBufferBaseEXTProcPtr) (GLenum target, GLuint index, GLuint buffer);
typedef void (* glBeginTransformFeedbackEXTProcPtr) (GLenum primitiveMode);
typedef void (* glEndTransformFeedbackEXTProcPtr) (void);
typedef void (* glTransformFeedbackVaryingsEXTProcPtr) (GLuint program, GLsizei count, const GLchar **varyings, GLenum bufferMode);
typedef void (* glGetTransformFeedbackVaryingEXTProcPtr) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
#else
extern void glBindBufferRangeEXT(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern void glBindBufferOffsetEXT(GLenum target, GLuint index, GLuint buffer, GLintptr offset);
extern void glBindBufferBaseEXT(GLenum target, GLuint index, GLuint buffer);
extern void glBeginTransformFeedbackEXT(GLenum primitiveMode);
extern void glEndTransformFeedbackEXT(void);
extern void glTransformFeedbackVaryingsEXT(GLuint program, GLsizei count, const GLchar **varyings, GLenum bufferMode);
extern void glGetTransformFeedbackVaryingEXT(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
#endif
#endif /* GL_EXT_transform_feedback */

#if GL_EXT_transform_feedback || GL_EXT_draw_buffers2
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glGetIntegerIndexedvEXTProcPtr) (GLenum param, GLint index, GLint *values);
typedef void (* glGetBooleanIndexedvEXTProcPtr) (GLenum param, GLint index, GLboolean *values);
#else
extern void glGetIntegerIndexedvEXT(GLenum param, GLuint index, GLint *values);
extern void glGetBooleanIndexedvEXT(GLenum param, GLuint index, GLboolean *values);
#endif
#endif /* GL_EXT_transform_feedback || GL_EXT_draw_buffers2 */

#if GL_EXT_bindable_uniform
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (*glUniformBufferEXTProcPtr) (GLuint program, GLint location, GLuint buffer);
typedef GLint (*glGetUniformBufferSizeEXTProcPtr) (GLuint program, GLint location);
typedef GLintptr (*glGetUniformOffsetEXTProcPtr) (GLuint program, GLint location);
#else
extern void glUniformBufferEXT(GLuint program, GLint location, GLuint buffer);
extern GLint glGetUniformBufferSizeEXT(GLuint program, GLint location);
extern GLintptr glGetUniformOffsetEXT(GLuint program, GLint location);
#endif
#endif /* GL_EXT_bindable_uniform */

#if GL_EXT_texture_integer
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (*glClearColorIiEXTProcPtr) ( GLint r, GLint g, GLint b, GLint a );
typedef void (*glClearColorIuiEXTProcPtr) ( GLuint r, GLuint g, GLuint b, GLuint a );
typedef void (*glTexParameterIivEXTProcPtr)( GLenum target, GLenum pname, GLint *params );
typedef void (*glTexParameterIuivEXTProcPtr)( GLenum target, GLenum pname, GLuint *params );
typedef void (*glGetTexParameterIivEXTProcPtr) ( GLenum target, GLenum pname, GLint *params);
typedef void (*glGetTexParameterIuivEXTProcPtr) ( GLenum target, GLenum pname, GLuint *params);
#else
extern void glClearColorIiEXT( GLint r, GLint g, GLint b, GLint a );
extern void glClearColorIuiEXT( GLuint r, GLuint g, GLuint b, GLuint a );
extern void glTexParameterIivEXT( GLenum target, GLenum pname, GLint *params );
extern void glTexParameterIuivEXT( GLenum target, GLenum pname, GLuint *params );
extern void glGetTexParameterIivEXT( GLenum target, GLenum pname, GLint *params);
extern void glGetTexParameterIuivEXT( GLenum target, GLenum pname, GLuint *params);
#endif
#endif /* GL_EXT_texture_integer */

#if GL_EXT_gpu_shader4
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (*glVertexAttribI1iEXTProcPtr) (GLuint index, GLint x);
typedef void (*glVertexAttribI2iEXTProcPtr) (GLuint index, GLint x, GLint y);
typedef void (*glVertexAttribI3iEXTProcPtr) (GLuint index, GLint x, GLint y, GLint z);
typedef void (*glVertexAttribI4iEXTProcPtr) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (*glVertexAttribI1uiEXTProcPtr) (GLuint index, GLuint x);
typedef void (*glVertexAttribI2uiEXTProcPtr) (GLuint index, GLuint x, GLuint y);
typedef void (*glVertexAttribI3uiEXTProcPtr) (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void (*glVertexAttribI4uiEXTProcPtr) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (*glVertexAttribI1ivEXTProcPtr) (GLuint index, const GLint *v);
typedef void (*glVertexAttribI2ivEXTProcPtr) (GLuint index, const GLint *v);
typedef void (*glVertexAttribI3ivEXTProcPtr) (GLuint index, const GLint *v);
typedef void (*glVertexAttribI4ivEXTProcPtr) (GLuint index, const GLint *v);
typedef void (*glVertexAttribI1uivEXTProcPtr) (GLuint index, const GLuint *v);
typedef void (*glVertexAttribI2uivEXTProcPtr) (GLuint index, const GLuint *v);
typedef void (*glVertexAttribI3uivEXTProcPtr) (GLuint index, const GLuint *v);
typedef void (*glVertexAttribI4uivEXTProcPtr) (GLuint index, const GLuint *v);
typedef void (*glVertexAttribI4bvEXTProcPtr) (GLuint index, const GLbyte *v);
typedef void (*glVertexAttribI4svEXTProcPtr) (GLuint index, const GLshort *v);
typedef void (*glVertexAttribI4ubvEXTProcPtr) (GLuint index, const GLubyte *v);
typedef void (*glVertexAttribI4usvEXTProcPtr) (GLuint index, const GLushort *v);
typedef void (*glVertexAttribIPointerEXTProcPtr) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (*glGetVertexAttribIivEXTProcPtr) (GLuint index, GLenum pname, GLint *params);
typedef void (*glGetVertexAttribIuivEXTProcPtr) (GLuint index, GLenum pname, GLuint *params);
typedef void (*glUniform1uiEXTProcPtr) (GLint location, GLuint v0);
typedef void (*glUniform2uiEXTProcPtr) (GLint location, GLuint v0, GLuint v1);
typedef void (*glUniform3uiEXTProcPtr) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (*glUniform4uiEXTProcPtr) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (*glUniform1uivEXTProcPtr) (GLint location, GLsizei count, const GLuint *value);
typedef void (*glUniform2uivEXTProcPtr) (GLint location, GLsizei count, const GLuint *value);
typedef void (*glUniform3uivEXTProcPtr) (GLint location, GLsizei count, const GLuint *value);
typedef void (*glUniform4uivEXTProcPtr) (GLint location, GLsizei count, const GLuint *value);
typedef void (*glGetUniformuivEXTProcPtr) (GLuint program, GLint location, GLuint *params);
typedef void (*glBindFragDataLocationEXTProcPtr) (GLuint program, GLuint colorNumber, const GLchar *name);
typedef GLint (*glGetFragDataLocationEXTProcPtr) (GLuint program, const GLchar *name);
#else
extern void glVertexAttribI1iEXT(GLuint index, GLint x);
extern void glVertexAttribI2iEXT(GLuint index, GLint x, GLint y);
extern void glVertexAttribI3iEXT(GLuint index, GLint x, GLint y, GLint z);
extern void glVertexAttribI4iEXT(GLuint index, GLint x, GLint y, GLint z, GLint w);
extern void glVertexAttribI1uiEXT(GLuint index, GLuint x);
extern void glVertexAttribI2uiEXT(GLuint index, GLuint x, GLuint y);
extern void glVertexAttribI3uiEXT(GLuint index, GLuint x, GLuint y, GLuint z);
extern void glVertexAttribI4uiEXT(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern void glVertexAttribI1ivEXT(GLuint index, const GLint *v);
extern void glVertexAttribI2ivEXT(GLuint index, const GLint *v);
extern void glVertexAttribI3ivEXT(GLuint index, const GLint *v);
extern void glVertexAttribI4ivEXT(GLuint index, const GLint *v);
extern void glVertexAttribI1uivEXT(GLuint index, const GLuint *v);
extern void glVertexAttribI2uivEXT(GLuint index, const GLuint *v);
extern void glVertexAttribI3uivEXT(GLuint index, const GLuint *v);
extern void glVertexAttribI4uivEXT(GLuint index, const GLuint *v);
extern void glVertexAttribI4bvEXT(GLuint index, const GLbyte *v);
extern void glVertexAttribI4svEXT(GLuint index, const GLshort *v);
extern void glVertexAttribI4ubvEXT(GLuint index, const GLubyte *v);
extern void glVertexAttribI4usvEXT(GLuint index, const GLushort *v);
extern void glVertexAttribIPointerEXT(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glGetVertexAttribIivEXT(GLuint index, GLenum pname, GLint *params);
extern void glGetVertexAttribIuivEXT(GLuint index, GLenum pname, GLuint *params);
extern void glUniform1uiEXT(GLint location, GLuint v0);
extern void glUniform2uiEXT(GLint location, GLuint v0, GLuint v1);
extern void glUniform3uiEXT(GLint location, GLuint v0, GLuint v1, GLuint v2);
extern void glUniform4uiEXT(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
extern void glUniform1uivEXT(GLint location, GLsizei count, const GLuint *value);
extern void glUniform2uivEXT(GLint location, GLsizei count, const GLuint *value);
extern void glUniform3uivEXT(GLint location, GLsizei count, const GLuint *value);
extern void glUniform4uivEXT(GLint location, GLsizei count, const GLuint *value);
extern void glGetUniformuivEXT(GLuint program, GLint location, GLuint *params);
extern void glBindFragDataLocationEXT(GLuint program, GLuint colorNumber, const GLchar *name);
extern GLint glGetFragDataLocationEXT(GLuint program, const GLchar *name);
#endif
#endif /* GL_EXT_gpu_shader4 */

#if GL_EXT_draw_buffers2
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glColorMaskIndexedEXTProcPtr) (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void (* glEnableIndexedEXTProcPtr) (GLenum target, GLuint index);
typedef void (* glDisableIndexedEXTProcPtr) (GLenum target, GLuint index);
typedef GLboolean (* glIsEnabledIndexedEXTProcPtr) (GLenum target, GLuint index);
#else
extern void glColorMaskIndexedEXT(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
extern void glEnableIndexedEXT(GLenum target, GLuint index);
extern void glDisableIndexedEXT(GLenum target, GLuint index);
extern GLboolean glIsEnabledIndexedEXT(GLenum target, GLuint index);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_EXT_provoking_vertex
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glProvokingVertexEXTProcPtr) (GLenum mode);
#else
extern void glProvokingVertexEXT(GLenum mode);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_APPLE_texture_range
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glTextureRangeAPPLEProcPtr) (GLenum target, GLsizei length, const GLvoid *pointer);
typedef void (* glGetTexParameterPointervAPPLEProcPtr) (GLenum target, GLenum pname, GLvoid **params);
#else
extern void glTextureRangeAPPLE(GLenum target, GLsizei length, const GLvoid *pointer);
extern void glGetTexParameterPointervAPPLE(GLenum target, GLenum pname, GLvoid **params);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_APPLE_vertex_array_range
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glVertexArrayRangeAPPLEProcPtr) (GLsizei length, const GLvoid *pointer);
typedef void (* glFlushVertexArrayRangeAPPLEProcPtr) (GLsizei length, const GLvoid *pointer);
typedef void (* glVertexArrayParameteriAPPLEProcPtr) (GLenum pname, GLint param);
#else
extern void glVertexArrayRangeAPPLE(GLsizei length, const GLvoid *pointer);
extern void glFlushVertexArrayRangeAPPLE(GLsizei length, const GLvoid *pointer);
extern void glVertexArrayParameteriAPPLE(GLenum pname, GLint param);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_APPLE_vertex_array_object
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBindVertexArrayAPPLEProcPtr) (GLuint id);
typedef void (* glDeleteVertexArraysAPPLEProcPtr) (GLsizei n, const GLuint *ids);
typedef void (* glGenVertexArraysAPPLEProcPtr) (GLsizei n, GLuint *ids);
typedef GLboolean (* glIsVertexArrayAPPLEProcPtr) (GLuint id);
#else
extern void glBindVertexArrayAPPLE(GLuint id);
extern void glDeleteVertexArraysAPPLE(GLsizei n, const GLuint *ids);
extern void glGenVertexArraysAPPLE(GLsizei n, GLuint *ids);
extern GLboolean glIsVertexArrayAPPLE(GLuint id);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_APPLE_fence
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glGenFencesAPPLEProcPtr) (GLsizei n, GLuint *fences);
typedef void (* glDeleteFencesAPPLEProcPtr) (GLsizei n, const GLuint *fences);
typedef void (* glSetFenceAPPLEProcPtr) (GLuint fence);
typedef GLboolean (* glIsFenceAPPLEProcPtr) (GLuint fence);
typedef GLboolean (* glTestFenceAPPLEProcPtr) (GLuint fence);
typedef void (* glFinishFenceAPPLEProcPtr) (GLuint fence);
typedef GLboolean (* glTestObjectAPPLEProcPtr) (GLenum object, GLuint name);
typedef void (* glFinishObjectAPPLEProcPtr) (GLenum object, GLuint name);
#else
extern void glGenFencesAPPLE(GLsizei n, GLuint *fences);
extern void glDeleteFencesAPPLE(GLsizei n, const GLuint *fences);
extern void glSetFenceAPPLE(GLuint fence);
extern GLboolean glIsFenceAPPLE(GLuint fence);
extern GLboolean glTestFenceAPPLE(GLuint fence);
extern void glFinishFenceAPPLE(GLuint fence);
extern GLboolean glTestObjectAPPLE(GLenum object, GLuint name);
extern void glFinishObjectAPPLE(GLenum object, GLuint name);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_APPLE_element_array
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glElementPointerAPPLEProcPtr) (GLenum type, const GLvoid *pointer);
typedef void (* glDrawElementArrayAPPLEProcPtr) (GLenum mode, GLint first, GLsizei count);
typedef void (* glDrawRangeElementArrayAPPLEProcPtr) (GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
typedef void (* glMultiDrawElementArrayAPPLEProcPtr) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
typedef void (* glMultiDrawRangeElementArrayAPPLEProcPtr) (GLenum mode, GLuint start, GLuint end, const GLint *first, const GLsizei *count, GLsizei primcount);
#else
extern void glElementPointerAPPLE(GLenum type, const GLvoid *pointer);
extern void glDrawElementArrayAPPLE(GLenum mode, GLint first, GLsizei count);
extern void glDrawRangeElementArrayAPPLE(GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
extern void glMultiDrawElementArrayAPPLE(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
extern void glMultiDrawRangeElementArrayAPPLE(GLenum mode, GLuint start, GLuint end, const GLint *first, const GLsizei *count, GLsizei primcount);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_APPLE_flush_render
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glFlushRenderAPPLEProcPtr) (void);
typedef void (* glFinishRenderAPPLEProcPtr) (void);
typedef void (* glSwapAPPLEProcPtr) (void);
#else
extern void glFlushRenderAPPLE(void);
extern void glFinishRenderAPPLE(void);
extern void glSwapAPPLE(void);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_APPLE_vertex_program_evaluators
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glEnableVertexAttribAPPLEProcPtr) (GLuint index, GLenum pname);
typedef void (* glDisableVertexAttribAPPLEProcPtr) (GLuint index, GLenum pname);
typedef GLboolean (* glIsVertexAttribEnabledAPPLEProcPtr) (GLuint index, GLenum pname);
typedef void (* glMapVertexAttrib1dAPPLEProcPtr) (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
typedef void (* glMapVertexAttrib1fAPPLEProcPtr) (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
typedef void (* glMapVertexAttrib2dAPPLEProcPtr) (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
typedef void (* glMapVertexAttrib2fAPPLEProcPtr) (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
#else
extern void glEnableVertexAttribAPPLE(GLuint index, GLenum pname);
extern void glDisableVertexAttribAPPLE(GLuint index, GLenum pname);
extern GLboolean glIsVertexAttribEnabledAPPLE(GLuint index, GLenum pname);
extern void glMapVertexAttrib1dAPPLE(GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
extern void glMapVertexAttrib1fAPPLE(GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
extern void glMapVertexAttrib2dAPPLE(GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
extern void glMapVertexAttrib2fAPPLE(GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_APPLE_flush_buffer_range
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBufferParameteriAPPLEProcPtr) (GLenum target, GLenum pname, GLint param);
typedef void (* glFlushMappedBufferRangeAPPLEProcPtr) (GLenum target, GLintptr offset, GLsizeiptr size);
#else
extern void glBufferParameteriAPPLE(GLenum target, GLenum pname, GLint param);
extern void glFlushMappedBufferRangeAPPLE(GLenum target, GLintptr offset, GLsizeiptr size);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_APPLE_object_purgeable
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef GLenum (* glObjectPurgeableAPPLEProcPtr) (GLenum objectType, GLuint name, GLenum option);
typedef GLenum (* glObjectUnpurgeableAPPLEProcPtr) (GLenum objectType, GLuint name, GLenum option);
typedef void (* glGetObjectParameterivAPPLEProcPtr) (GLenum objectType, GLuint name, GLenum pname, GLint* params);
#else
extern GLenum glObjectPurgeableAPPLE(GLenum objectType, GLuint name, GLenum option);
extern GLenum glObjectUnpurgeableAPPLE(GLenum objectType, GLuint name, GLenum option);
extern void   glGetObjectParameterivAPPLE(GLenum objectType, GLuint name, GLenum pname, GLint* params);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_APPLE_vertex_point_size
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glPointSizePointerAPPLEProcPtr) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (* glVertexPointSizefAPPLEProcPtr) (GLfloat size);
#else
extern void glPointSizePointerAPPLE(GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glVertexPointSizefAPPLE(GLfloat size);
#endif
#endif

#if GL_ATI_pn_triangles
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glPNTrianglesiATIProcPtr) (GLenum pname, GLint param);
typedef void (* glPNTrianglesfATIProcPtr) (GLenum pname, GLfloat param);
#else
extern void glPNTrianglesiATI(GLenum pname, GLint param);
extern void glPNTrianglesfATI(GLenum pname, GLfloat param);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ATI_blend_equation_separate
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBlendEquationSeparateATIProcPtr) (GLenum equationRGB, GLenum equationAlpha);
#else
extern void glBlendEquationSeparateATI(GLenum equationRGB, GLenum equationAlpha);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ATI_separate_stencil
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glStencilOpSeparateATIProcPtr) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (* glStencilFuncSeparateATIProcPtr) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
#else
extern void glStencilOpSeparateATI(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
extern void glStencilFuncSeparateATI(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_ATIX_pn_triangles
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glPNTrianglesiATIXProcPtr) (GLenum pname, GLint param);
typedef void (* glPNTrianglesfATIXProcPtr) (GLenum pname, GLfloat param);
#else
extern void glPNTrianglesiATIX(GLenum pname, GLint param);
extern void glPNTrianglesfATIX(GLenum pname, GLfloat param);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif


#if GL_HP_image_transform
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glImageTransformParameteriHPProcPtr) (GLenum, GLenum, GLint);
typedef void (* glImageTransformParameterfHPProcPtr) (GLenum, GLenum, GLfloat);
typedef void (* glImageTransformParameterivHPProcPtr) (GLenum, GLenum, const GLint *);
typedef void (* glImageTransformParameterfvHPProcPtr) (GLenum, GLenum, const GLfloat *);
typedef void (* glGetImageTransformParameterivHPProcPtr) (GLenum, GLenum, GLint *);
typedef void (* glGetImageTransformParameterfvHPProcPtr) (GLenum, GLenum, GLfloat *);
#else
extern void glImageTransformParameteriHP (GLenum, GLenum, GLint);
extern void glImageTransformParameterfHP (GLenum, GLenum, GLfloat);
extern void glImageTransformParameterivHP (GLenum, GLenum, const GLint *);
extern void glImageTransformParameterfvHP (GLenum, GLenum, const GLfloat *);
extern void glGetImageTransformParameterivHP (GLenum, GLenum, GLint *);
extern void glGetImageTransformParameterfvHP (GLenum, GLenum, GLfloat *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif


#if GL_IBM_multimode_draw_arrays
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glMultiModeDrawArraysIBMProcPtr) (GLenum, const GLint *, const GLsizei *, GLsizei, GLint);
typedef void (* glMultiModeDrawElementsIBMProcPtr) (const GLenum *, const GLsizei *, GLenum, const GLvoid* *, GLsizei, GLint);
#else
extern void glMultiModeDrawArraysIBM (GLenum, const GLint *, const GLsizei *, GLsizei, GLint);
extern void glMultiModeDrawElementsIBM (const GLenum *, const GLsizei *, GLenum, const GLvoid* *, GLsizei, GLint);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_IBM_vertex_array_lists
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glColorPointerListIBMProcPtr) (GLint, GLenum, GLint, const GLvoid* *, GLint);
typedef void (* glSecondaryColorPointerListIBMProcPtr) (GLint, GLenum, GLint, const GLvoid* *, GLint);
typedef void (* glEdgeFlagPointerListIBMProcPtr) (GLint, const GLboolean* *, GLint);
typedef void (* glFogCoordPointerListIBMProcPtr) (GLenum, GLint, const GLvoid* *, GLint);
typedef void (* glIndexPointerListIBMProcPtr) (GLenum, GLint, const GLvoid* *, GLint);
typedef void (* glNormalPointerListIBMProcPtr) (GLenum, GLint, const GLvoid* *, GLint);
typedef void (* glTexCoordPointerListIBMProcPtr) (GLint, GLenum, GLint, const GLvoid* *, GLint);
typedef void (* glVertexPointerListIBMProcPtr) (GLint, GLenum, GLint, const GLvoid* *, GLint);
#else
extern void glColorPointerListIBM (GLint, GLenum, GLint, const GLvoid* *, GLint);
extern void glSecondaryColorPointerListIBM (GLint, GLenum, GLint, const GLvoid* *, GLint);
extern void glEdgeFlagPointerListIBM (GLint, const GLboolean* *, GLint);
extern void glFogCoordPointerListIBM (GLenum, GLint, const GLvoid* *, GLint);
extern void glIndexPointerListIBM (GLenum, GLint, const GLvoid* *, GLint);
extern void glNormalPointerListIBM (GLenum, GLint, const GLvoid* *, GLint);
extern void glTexCoordPointerListIBM (GLint, GLenum, GLint, const GLvoid* *, GLint);
extern void glVertexPointerListIBM (GLint, GLenum, GLint, const GLvoid* *, GLint);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif


#if GL_INTEL_parallel_arrays
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glVertexPointervINTELProcPtr) (GLint, GLenum, const GLvoid* *);
typedef void (* glNormalPointervINTELProcPtr) (GLenum, const GLvoid* *);
typedef void (* glColorPointervINTELProcPtr) (GLint, GLenum, const GLvoid* *);
typedef void (* glTexCoordPointervINTELProcPtr) (GLint, GLenum, const GLvoid* *);
#else
extern void glVertexPointervINTEL (GLint, GLenum, const GLvoid* *);
extern void glNormalPointervINTEL (GLenum, const GLvoid* *);
extern void glColorPointervINTEL (GLint, GLenum, const GLvoid* *);
extern void glTexCoordPointervINTEL (GLint, GLenum, const GLvoid* *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif


#if GL_MESA_resize_buffers
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glResizeBuffersMESAProcPtr) (void);
#else
extern void glResizeBuffersMESA (void);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif


#if GL_NV_vertex_array_range
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glFlushVertexArrayRangeNVProcPtr) (void);
typedef void (* glVertexArrayRangeNVProcPtr) (GLsizei, const GLvoid *);
#else
extern void glFlushVertexArrayRangeNV (void);
extern void glVertexArrayRangeNV (GLsizei, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_NV_register_combiners
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glCombinerParameterfvNVProcPtr) (GLenum, const GLfloat *);
typedef void (* glCombinerParameterfNVProcPtr) (GLenum, GLfloat);
typedef void (* glCombinerParameterivNVProcPtr) (GLenum, const GLint *);
typedef void (* glCombinerParameteriNVProcPtr) (GLenum, GLint);
typedef void (* glCombinerInputNVProcPtr) (GLenum, GLenum, GLenum, GLenum, GLenum, GLenum);
typedef void (* glCombinerOutputNVProcPtr) (GLenum, GLenum, GLenum, GLenum, GLenum, GLenum, GLenum, GLboolean, GLboolean, GLboolean);
typedef void (* glFinalCombinerInputNVProcPtr) (GLenum, GLenum, GLenum, GLenum);
typedef void (* glGetCombinerInputParameterfvNVProcPtr) (GLenum, GLenum, GLenum, GLenum, GLfloat *);
typedef void (* glGetCombinerInputParameterivNVProcPtr) (GLenum, GLenum, GLenum, GLenum, GLint *);
typedef void (* glGetCombinerOutputParameterfvNVProcPtr) (GLenum, GLenum, GLenum, GLfloat *);
typedef void (* glGetCombinerOutputParameterivNVProcPtr) (GLenum, GLenum, GLenum, GLint *);
typedef void (* glGetFinalCombinerInputParameterfvNVProcPtr) (GLenum, GLenum, GLfloat *);
typedef void (* glGetFinalCombinerInputParameterivNVProcPtr) (GLenum, GLenum, GLint *);
#else
extern void glCombinerParameterfvNV (GLenum, const GLfloat *);
extern void glCombinerParameterfNV (GLenum, GLfloat);
extern void glCombinerParameterivNV (GLenum, const GLint *);
extern void glCombinerParameteriNV (GLenum, GLint);
extern void glCombinerInputNV (GLenum, GLenum, GLenum, GLenum, GLenum, GLenum);
extern void glCombinerOutputNV (GLenum, GLenum, GLenum, GLenum, GLenum, GLenum, GLenum, GLboolean, GLboolean, GLboolean);
extern void glFinalCombinerInputNV (GLenum, GLenum, GLenum, GLenum);
extern void glGetCombinerInputParameterfvNV (GLenum, GLenum, GLenum, GLenum, GLfloat *);
extern void glGetCombinerInputParameterivNV (GLenum, GLenum, GLenum, GLenum, GLint *);
extern void glGetCombinerOutputParameterfvNV (GLenum, GLenum, GLenum, GLfloat *);
extern void glGetCombinerOutputParameterivNV (GLenum, GLenum, GLenum, GLint *);
extern void glGetFinalCombinerInputParameterfvNV (GLenum, GLenum, GLfloat *);
extern void glGetFinalCombinerInputParameterivNV (GLenum, GLenum, GLint *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_NV_register_combiners2
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glCombinerStageParameterfvNVProcPtr) (GLenum, GLenum, const GLfloat *);
typedef void (* glGetCombinerStageParameterfvNVProcPtr) (GLenum, GLenum, GLfloat *);
#else
extern void glCombinerStageParameterfvNV (GLenum, GLenum, const GLfloat *);
extern void glGetCombinerStageParameterfvNV (GLenum, GLenum, GLfloat *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_NV_vertex_program
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBindProgramNVProcPtr) (GLenum target, GLuint id);
typedef void (* glDeleteProgramsNVProcPtr) (GLsizei n, const GLuint *ids);
typedef void (* glExecuteProgramNVProcPtr) (GLenum target, GLuint id, const GLfloat *params);
typedef void (* glGenProgramsNVProcPtr) (GLsizei n, GLuint *ids);
typedef GLboolean (* glAreProgramsResidentNVProcPtr) (GLsizei n, const GLuint *ids, GLboolean *residences);
typedef void (* glRequestResidentProgramsNVProcPtr) (GLsizei n, GLuint *ids);
typedef void (* glGetProgramParameterfvNVProcPtr) (GLenum target, GLuint index, GLenum pname, GLfloat *params);
typedef void (* glGetProgramParameterdvNVProcPtr) (GLenum target, GLuint index, GLenum pname, GLdouble *params);
typedef void (* glGetProgramivNVProcPtr) (GLuint id, GLenum pname, GLint *params);
typedef void (* glGetProgramStringNVProcPtr) (GLuint id, GLenum pname, GLubyte *program);
typedef void (* glGetTrackMatrixivNVProcPtr) (GLenum target, GLuint address, GLenum pname, GLint *params);
typedef void (* glGetVertexAttribdvNVProcPtr) (GLuint index, GLenum pname, GLdouble *params);
typedef void (* glGetVertexAttribfvNVProcPtr) (GLuint index, GLenum pname, GLfloat *params);
typedef void (* glGetVertexAttribivNVProcPtr) (GLuint index, GLenum pname, GLint *params);
typedef void (* glGetVertexAttribPointervNVProcPtr) (GLuint index, GLenum pname, GLvoid **pointer);
typedef GLboolean (* glIsProgramNVProcPtr) (GLuint id);
typedef void (* glLoadProgramNVProcPtr) (GLenum target, GLuint id, GLsizei len, const GLubyte *program);
typedef void (* glProgramParameter4fNVProcPtr) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (* glProgramParameter4dNVProcPtr) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (* glProgramParameter4dvNVProcPtr) (GLenum target, GLuint index, const GLdouble *params);
typedef void (* glProgramParameter4fvNVProcPtr) (GLenum target, GLuint index, const GLfloat *params);
typedef void (* glProgramParameters4dvNVProcPtr) (GLenum target, GLuint index, GLuint num, const GLdouble *params);
typedef void (* glProgramParameters4fvNVProcPtr) (GLenum target, GLuint index, GLuint num, const GLfloat *params);
typedef void (* glTrackMatrixNVProcPtr) (GLenum target, GLuint address, GLenum matrix, GLenum transform);
typedef void (* glVertexAttribPointerNVProcPtr) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (* glVertexAttrib1sNVProcPtr) (GLuint index, GLshort x);
typedef void (* glVertexAttrib1fNVProcPtr) (GLuint index, GLfloat x);
typedef void (* glVertexAttrib1dNVProcPtr) (GLuint index, GLdouble x);
typedef void (* glVertexAttrib2sNVProcPtr) (GLuint index, GLshort x, GLshort y);
typedef void (* glVertexAttrib2fNVProcPtr) (GLuint index, GLfloat x, GLfloat y);
typedef void (* glVertexAttrib2dNVProcPtr) (GLuint index, GLdouble x, GLdouble y);
typedef void (* glVertexAttrib3sNVProcPtr) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (* glVertexAttrib3fNVProcPtr) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (* glVertexAttrib3dNVProcPtr) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (* glVertexAttrib4sNVProcPtr) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (* glVertexAttrib4fNVProcPtr) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (* glVertexAttrib4dNVProcPtr) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (* glVertexAttrib4ubNVProcPtr) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (* glVertexAttrib1svNVProcPtr) (GLuint index, GLshort *v);
typedef void (* glVertexAttrib1fvNVProcPtr) (GLuint index, GLfloat *v);
typedef void (* glVertexAttrib1dvNVProcPtr) (GLuint index, GLdouble *v);
typedef void (* glVertexAttrib2svNVProcPtr) (GLuint index, GLshort *v);
typedef void (* glVertexAttrib2fvNVProcPtr) (GLuint index, GLfloat *v);
typedef void (* glVertexAttrib2dvNVProcPtr) (GLuint index, GLdouble *v);
typedef void (* glVertexAttrib3svNVProcPtr) (GLuint index, GLshort *v);
typedef void (* glVertexAttrib3fvNVProcPtr) (GLuint index, GLfloat *v);
typedef void (* glVertexAttrib3dvNVProcPtr) (GLuint index, GLdouble *v);
typedef void (* glVertexAttrib4svNVProcPtr) (GLuint index, GLshort *v);
typedef void (* glVertexAttrib4fvNVProcPtr) (GLuint index, GLfloat *v);
typedef void (* glVertexAttrib4dvNVProcPtr) (GLuint index, GLdouble *v);
typedef void (* glVertexAttrib4ubvNVProcPtr) (GLuint index, GLubyte *v);
typedef void (* glVertexAttribs1svNVProcPtr) (GLuint index, GLsizei n, GLshort *v);
typedef void (* glVertexAttribs1fvNVProcPtr) (GLuint index, GLsizei n, GLfloat *v);
typedef void (* glVertexAttribs1dvNVProcPtr) (GLuint index, GLsizei n, GLdouble *v);
typedef void (* glVertexAttribs2svNVProcPtr) (GLuint index, GLsizei n, GLshort *v);
typedef void (* glVertexAttribs2fvNVProcPtr) (GLuint index, GLsizei n, GLfloat *v);
typedef void (* glVertexAttribs2dvNVProcPtr) (GLuint index, GLsizei n, GLdouble *v);
typedef void (* glVertexAttribs3svNVProcPtr) (GLuint index, GLsizei n, GLshort *v);
typedef void (* glVertexAttribs3fvNVProcPtr) (GLuint index, GLsizei n, GLfloat *v);
typedef void (* glVertexAttribs3dvNVProcPtr) (GLuint index, GLsizei n, GLdouble *v);
typedef void (* glVertexAttribs4svNVProcPtr) (GLuint index, GLsizei n, GLshort *v);
typedef void (* glVertexAttribs4fvNVProcPtr) (GLuint index, GLsizei n, GLfloat *v);
typedef void (* glVertexAttribs4dvNVProcPtr) (GLuint index, GLsizei n, GLdouble *v);
typedef void (* glVertexAttribs4ubvNVProcPtr) (GLuint index, GLsizei n, GLubyte *v);
#else
extern void glBindProgramNV(GLenum target, GLuint id);
extern void glDeleteProgramsNV(GLsizei n, const GLuint *ids);
extern void glExecuteProgramNV(GLenum target, GLuint id, const GLfloat *params);
extern void glGenProgramsNV(GLsizei n, GLuint *ids);
extern GLboolean glAreProgramsResidentNV(GLsizei n, const GLuint *ids, GLboolean *residences);
extern void glRequestResidentProgramsNV(GLsizei n, GLuint *ids);
extern void glGetProgramParameterfvNV(GLenum target, GLuint index, GLenum pname, GLfloat *params);
extern void glGetProgramParameterdvNV(GLenum target, GLuint index, GLenum pname, GLdouble *params);
extern void glGetProgramivNV(GLuint id, GLenum pname, GLint *params);
extern void glGetProgramStringNV(GLuint id, GLenum pname, GLubyte *program);
extern void glGetTrackMatrixivNV(GLenum target, GLuint address, GLenum pname, GLint *params);
extern void glGetVertexAttribdvNV(GLuint index, GLenum pname, GLdouble *params);
extern void glGetVertexAttribfvNV(GLuint index, GLenum pname, GLfloat *params);
extern void glGetVertexAttribivNV(GLuint index, GLenum pname, GLint *params);
extern void glGetVertexAttribPointervNV(GLuint index, GLenum pname, GLvoid **pointer);
extern GLboolean glIsProgramNV(GLuint id);
extern void glLoadProgramNV(GLenum target, GLuint id, GLsizei len, const GLubyte *program);
extern void glProgramParameter4fNV(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glProgramParameter4dNV(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glProgramParameter4dvNV(GLenum target, GLuint index, const GLdouble *params);
extern void glProgramParameter4fvNV(GLenum target, GLuint index, const GLfloat *params);
extern void glProgramParameters4dvNV(GLenum target, GLuint index, GLuint num, const GLdouble *params);
extern void glProgramParameters4fvNV(GLenum target, GLuint index, GLuint num, const GLfloat *params);
extern void glTrackMatrixNV(GLenum target, GLuint address, GLenum matrix, GLenum transform);
extern void glVertexAttribPointerNV(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glVertexAttrib1sNV(GLuint index, GLshort x);
extern void glVertexAttrib1fNV(GLuint index, GLfloat x);
extern void glVertexAttrib1dNV(GLuint index, GLdouble x);
extern void glVertexAttrib2sNV(GLuint index, GLshort x, GLshort y);
extern void glVertexAttrib2fNV(GLuint index, GLfloat x, GLfloat y);
extern void glVertexAttrib2dNV(GLuint index, GLdouble x, GLdouble y);
extern void glVertexAttrib3sNV(GLuint index, GLshort x, GLshort y, GLshort z);
extern void glVertexAttrib3fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern void glVertexAttrib3dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void glVertexAttrib4sNV(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern void glVertexAttrib4fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glVertexAttrib4dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glVertexAttrib4ubNV(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern void glVertexAttrib1svNV(GLuint index, GLshort *v);
extern void glVertexAttrib1fvNV(GLuint index, GLfloat *v);
extern void glVertexAttrib1dvNV(GLuint index, GLdouble *v);
extern void glVertexAttrib2svNV(GLuint index, GLshort *v);
extern void glVertexAttrib2fvNV(GLuint index, GLfloat *v);
extern void glVertexAttrib2dvNV(GLuint index, GLdouble *v);
extern void glVertexAttrib3svNV(GLuint index, GLshort *v);
extern void glVertexAttrib3fvNV(GLuint index, GLfloat *v);
extern void glVertexAttrib3dvNV(GLuint index, GLdouble *v);
extern void glVertexAttrib4svNV(GLuint index, GLshort *v);
extern void glVertexAttrib4fvNV(GLuint index, GLfloat *v);
extern void glVertexAttrib4dvNV(GLuint index, GLdouble *v);
extern void glVertexAttrib4ubvNV(GLuint index, GLubyte *v);
extern void glVertexAttribs1svNV(GLuint index, GLsizei n, GLshort *v);
extern void glVertexAttribs1fvNV(GLuint index, GLsizei n, GLfloat *v);
extern void glVertexAttribs1dvNV(GLuint index, GLsizei n, GLdouble *v);
extern void glVertexAttribs2svNV(GLuint index, GLsizei n, GLshort *v);
extern void glVertexAttribs2fvNV(GLuint index, GLsizei n, GLfloat *v);
extern void glVertexAttribs2dvNV(GLuint index, GLsizei n, GLdouble *v);
extern void glVertexAttribs3svNV(GLuint index, GLsizei n, GLshort *v);
extern void glVertexAttribs3fvNV(GLuint index, GLsizei n, GLfloat *v);
extern void glVertexAttribs3dvNV(GLuint index, GLsizei n, GLdouble *v);
extern void glVertexAttribs4svNV(GLuint index, GLsizei n, GLshort *v);
extern void glVertexAttribs4fvNV(GLuint index, GLsizei n, GLfloat *v);
extern void glVertexAttribs4dvNV(GLuint index, GLsizei n, GLdouble *v);
extern void glVertexAttribs4ubvNV(GLuint index, GLsizei n, GLubyte *v);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_NV_point_sprite
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glPointParameteriNVProcPtr) (GLenum pname, GLint param);
typedef void (* glPointParameterivNVProcPtr) (GLenum pname, const GLint *params);
#else
extern void glPointParameteriNV(GLenum pname, GLint param);
extern void glPointParameterivNV(GLenum pname, const GLint *params);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif


#if GL_PGI_misc_hints
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glHintPGIProcPtr) (GLenum, GLint);
#else
extern void glHintPGI (GLenum, GLint);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif


#if GL_SGI_color_table
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glColorTableSGIProcPtr) (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
typedef void (* glColorTableParameterfvSGIProcPtr) (GLenum, GLenum, const GLfloat *);
typedef void (* glColorTableParameterivSGIProcPtr) (GLenum, GLenum, const GLint *);
typedef void (* glCopyColorTableSGIProcPtr) (GLenum, GLenum, GLint, GLint, GLsizei);
typedef void (* glGetColorTableSGIProcPtr) (GLenum, GLenum, GLenum, GLvoid *);
typedef void (* glGetColorTableParameterfvSGIProcPtr) (GLenum, GLenum, GLfloat *);
typedef void (* glGetColorTableParameterivSGIProcPtr) (GLenum, GLenum, GLint *);
#else
extern void glColorTableSGI (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
extern void glColorTableParameterfvSGI (GLenum, GLenum, const GLfloat *);
extern void glColorTableParameterivSGI (GLenum, GLenum, const GLint *);
extern void glCopyColorTableSGI (GLenum, GLenum, GLint, GLint, GLsizei);
extern void glGetColorTableSGI (GLenum, GLenum, GLenum, GLvoid *);
extern void glGetColorTableParameterfvSGI (GLenum, GLenum, GLfloat *);
extern void glGetColorTableParameterivSGI (GLenum, GLenum, GLint *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIS_texture_filter4
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glGetTexFilterFuncSGISProcPtr) (GLenum, GLenum, GLfloat *);
typedef void (* glTexFilterFuncSGISProcPtr) (GLenum, GLenum, GLsizei, const GLfloat *);
#else
extern void glGetTexFilterFuncSGIS (GLenum, GLenum, GLfloat *);
extern void glTexFilterFuncSGIS (GLenum, GLenum, GLsizei, const GLfloat *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIS_pixel_texture
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glPixelTexGenParameteriSGISProcPtr) (GLenum, GLint);
typedef void (* glPixelTexGenParameterivSGISProcPtr) (GLenum, const GLint *);
typedef void (* glPixelTexGenParameterfSGISProcPtr) (GLenum, GLfloat);
typedef void (* glPixelTexGenParameterfvSGISProcPtr) (GLenum, const GLfloat *);
typedef void (* glGetPixelTexGenParameterivSGISProcPtr) (GLenum, GLint *);
typedef void (* glGetPixelTexGenParameterfvSGISProcPtr) (GLenum, GLfloat *);
#else
extern void glPixelTexGenParameteriSGIS (GLenum, GLint);
extern void glPixelTexGenParameterivSGIS (GLenum, const GLint *);
extern void glPixelTexGenParameterfSGIS (GLenum, GLfloat);
extern void glPixelTexGenParameterfvSGIS (GLenum, const GLfloat *);
extern void glGetPixelTexGenParameterivSGIS (GLenum, GLint *);
extern void glGetPixelTexGenParameterfvSGIS (GLenum, GLfloat *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIS_texture4D
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glTexImage4DSGISProcPtr) (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
typedef void (* glTexSubImage4DSGISProcPtr) (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
#else
extern void glTexImage4DSGIS (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
extern void glTexSubImage4DSGIS (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIS_detail_texture
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glDetailTexFuncSGISProcPtr) (GLenum, GLsizei, const GLfloat *);
typedef void (* glGetDetailTexFuncSGISProcPtr) (GLenum, GLfloat *);
#else
extern void glDetailTexFuncSGIS (GLenum, GLsizei, const GLfloat *);
extern void glGetDetailTexFuncSGIS (GLenum, GLfloat *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIS_sharpen_texture
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glSharpenTexFuncSGISProcPtr) (GLenum, GLsizei, const GLfloat *);
typedef void (* glGetSharpenTexFuncSGISProcPtr) (GLenum, GLfloat *);
#else
extern void glSharpenTexFuncSGIS (GLenum, GLsizei, const GLfloat *);
extern void glGetSharpenTexFuncSGIS (GLenum, GLfloat *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIS_multisample
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glSampleMaskSGISProcPtr) (GLclampf, GLboolean);
typedef void (* glSamplePatternSGISProcPtr) (GLenum);
#else
extern void glSampleMaskSGIS (GLclampf, GLboolean);
extern void glSamplePatternSGIS (GLenum);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIS_fog_function
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glFogFuncSGISProcPtr) (GLsizei, const GLfloat *);
typedef void (* glGetFogFuncSGISProcPtr) (const GLfloat *);
#else
extern void glFogFuncSGIS (GLsizei, const GLfloat *);
extern void glGetFogFuncSGIS (const GLfloat *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIS_texture_color_mask
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glTextureColorMaskSGISProcPtr) (GLboolean, GLboolean, GLboolean, GLboolean);
#else
extern void glTextureColorMaskSGIS (GLboolean, GLboolean, GLboolean, GLboolean);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_pixel_texture
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glPixelTexGenSGIXProcPtr) (GLenum);
#else
extern void glPixelTexGenSGIX (GLenum);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_sprite
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glSpriteParameterfSGIXProcPtr) (GLenum, GLfloat);
typedef void (* glSpriteParameterfvSGIXProcPtr) (GLenum, const GLfloat *);
typedef void (* glSpriteParameteriSGIXProcPtr) (GLenum, GLint);
typedef void (* glSpriteParameterivSGIXProcPtr) (GLenum, const GLint *);
#else
extern void glSpriteParameterfSGIX (GLenum, GLfloat);
extern void glSpriteParameterfvSGIX (GLenum, const GLfloat *);
extern void glSpriteParameteriSGIX (GLenum, GLint);
extern void glSpriteParameterivSGIX (GLenum, const GLint *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_instruments
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef GLint (* glGetInstrumentsSGIXProcPtr) (void);
typedef void (* glInstrumentsBufferSGIXProcPtr) (GLsizei, GLint *);
typedef GLint (* glPollInstrumentsSGIXProcPtr) (GLint *);
typedef void (* glReadInstrumentsSGIXProcPtr) (GLint);
typedef void (* glStartInstrumentsSGIXProcPtr) (void);
typedef void (* glStopInstrumentsSGIXProcPtr) (GLint);
#else
extern GLint glGetInstrumentsSGIX (void);
extern void glInstrumentsBufferSGIX (GLsizei, GLint *);
extern GLint glPollInstrumentsSGIX (GLint *);
extern void glReadInstrumentsSGIX (GLint);
extern void glStartInstrumentsSGIX (void);
extern void glStopInstrumentsSGIX (GLint);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_framezoom
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glFrameZoomSGIXProcPtr) (GLint);
#else
extern void glFrameZoomSGIX (GLint);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_tag_sample_buffer
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glTagSampleBufferSGIXProcPtr) (void);
#else
extern void glTagSampleBufferSGIX (void);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_polynomial_ffd
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glDeformationMap3dSGIXProcPtr) (GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble *);
typedef void (* glDeformationMap3fSGIXProcPtr) (GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat *);
typedef void (* glDeformSGIXProcPtr) (GLbitfield);
typedef void (* glLoadIdentityDeformationMapSGIXProcPtr) (GLbitfield);
#else
extern void glDeformationMap3dSGIX (GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble *);
extern void glDeformationMap3fSGIX (GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat *);
extern void glDeformSGIX (GLbitfield);
extern void glLoadIdentityDeformationMapSGIX (GLbitfield);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_reference_plane
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glReferencePlaneSGIXProcPtr) (const GLdouble *);
#else
extern void glReferencePlaneSGIX (const GLdouble *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_flush_raster
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glFlushRasterSGIXProcPtr) (void);
#else
extern void glFlushRasterSGIX (void);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_list_priority
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glGetListParameterfvSGIXProcPtr) (GLuint, GLenum, GLfloat *);
typedef void (* glGetListParameterivSGIXProcPtr) (GLuint, GLenum, GLint *);
typedef void (* glListParameterfSGIXProcPtr) (GLuint, GLenum, GLfloat);
typedef void (* glListParameterfvSGIXProcPtr) (GLuint, GLenum, const GLfloat *);
typedef void (* glListParameteriSGIXProcPtr) (GLuint, GLenum, GLint);
typedef void (* glListParameterivSGIXProcPtr) (GLuint, GLenum, const GLint *);
#else
extern void glGetListParameterfvSGIX (GLuint, GLenum, GLfloat *);
extern void glGetListParameterivSGIX (GLuint, GLenum, GLint *);
extern void glListParameterfSGIX (GLuint, GLenum, GLfloat);
extern void glListParameterfvSGIX (GLuint, GLenum, const GLfloat *);
extern void glListParameteriSGIX (GLuint, GLenum, GLint);
extern void glListParameterivSGIX (GLuint, GLenum, const GLint *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_fragment_lighting
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glFragmentColorMaterialSGIXProcPtr) (GLenum, GLenum);
typedef void (* glFragmentLightfSGIXProcPtr) (GLenum, GLenum, GLfloat);
typedef void (* glFragmentLightfvSGIXProcPtr) (GLenum, GLenum, const GLfloat *);
typedef void (* glFragmentLightiSGIXProcPtr) (GLenum, GLenum, GLint);
typedef void (* glFragmentLightivSGIXProcPtr) (GLenum, GLenum, const GLint *);
typedef void (* glFragmentLightModelfSGIXProcPtr) (GLenum, GLfloat);
typedef void (* glFragmentLightModelfvSGIXProcPtr) (GLenum, const GLfloat *);
typedef void (* glFragmentLightModeliSGIXProcPtr) (GLenum, GLint);
typedef void (* glFragmentLightModelivSGIXProcPtr) (GLenum, const GLint *);
typedef void (* glFragmentMaterialfSGIXProcPtr) (GLenum, GLenum, GLfloat);
typedef void (* glFragmentMaterialfvSGIXProcPtr) (GLenum, GLenum, const GLfloat *);
typedef void (* glFragmentMaterialiSGIXProcPtr) (GLenum, GLenum, GLint);
typedef void (* glFragmentMaterialivSGIXProcPtr) (GLenum, GLenum, const GLint *);
typedef void (* glGetFragmentLightfvSGIXProcPtr) (GLenum, GLenum, GLfloat *);
typedef void (* glGetFragmentLightivSGIXProcPtr) (GLenum, GLenum, GLint *);
typedef void (* glGetFragmentMaterialfvSGIXProcPtr) (GLenum, GLenum, GLfloat *);
typedef void (* glGetFragmentMaterialivSGIXProcPtr) (GLenum, GLenum, GLint *);
typedef void (* glLightEnviSGIXProcPtr) (GLenum, GLint);
#else
extern void glFragmentColorMaterialSGIX (GLenum, GLenum);
extern void glFragmentLightfSGIX (GLenum, GLenum, GLfloat);
extern void glFragmentLightfvSGIX (GLenum, GLenum, const GLfloat *);
extern void glFragmentLightiSGIX (GLenum, GLenum, GLint);
extern void glFragmentLightivSGIX (GLenum, GLenum, const GLint *);
extern void glFragmentLightModelfSGIX (GLenum, GLfloat);
extern void glFragmentLightModelfvSGIX (GLenum, const GLfloat *);
extern void glFragmentLightModeliSGIX (GLenum, GLint);
extern void glFragmentLightModelivSGIX (GLenum, const GLint *);
extern void glFragmentMaterialfSGIX (GLenum, GLenum, GLfloat);
extern void glFragmentMaterialfvSGIX (GLenum, GLenum, const GLfloat *);
extern void glFragmentMaterialiSGIX (GLenum, GLenum, GLint);
extern void glFragmentMaterialivSGIX (GLenum, GLenum, const GLint *);
extern void glGetFragmentLightfvSGIX (GLenum, GLenum, GLfloat *);
extern void glGetFragmentLightivSGIX (GLenum, GLenum, GLint *);
extern void glGetFragmentMaterialfvSGIX (GLenum, GLenum, GLfloat *);
extern void glGetFragmentMaterialivSGIX (GLenum, GLenum, GLint *);
extern void glLightEnviSGIX (GLenum, GLint);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_async
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glAsyncMarkerSGIXProcPtr) (GLuint);
typedef GLint (* glFinishAsyncSGIXProcPtr) (GLuint *);
typedef GLint (* glPollAsyncSGIXProcPtr) (GLuint *);
typedef GLuint (* glGenAsyncMarkersSGIXProcPtr) (GLsizei);
typedef void (* glDeleteAsyncMarkersSGIXProcPtr) (GLuint, GLsizei);
typedef GLboolean (* glIsAsyncMarkerSGIXProcPtr) (GLuint);
#else
extern void glAsyncMarkerSGIX (GLuint);
extern GLint glFinishAsyncSGIX (GLuint *);
extern GLint glPollAsyncSGIX (GLuint *);
extern GLuint glGenAsyncMarkersSGIX (GLsizei);
extern void glDeleteAsyncMarkersSGIX (GLuint, GLsizei);
extern GLboolean glIsAsyncMarkerSGIX (GLuint);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SGIX_igloo_interface
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glIglooInterfaceSGIXProcPtr) (GLenum, const GLvoid *);
#else
extern void glIglooInterfaceSGIX (GLenum, const GLvoid *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif


#if GL_SUN_global_alpha
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glGlobalAlphaFactorbSUNProcPtr) (GLbyte);
typedef void (* glGlobalAlphaFactorsSUNProcPtr) (GLshort);
typedef void (* glGlobalAlphaFactoriSUNProcPtr) (GLint);
typedef void (* glGlobalAlphaFactorfSUNProcPtr) (GLfloat);
typedef void (* glGlobalAlphaFactordSUNProcPtr) (GLdouble);
typedef void (* glGlobalAlphaFactorubSUNProcPtr) (GLubyte);
typedef void (* glGlobalAlphaFactorusSUNProcPtr) (GLushort);
typedef void (* glGlobalAlphaFactoruiSUNProcPtr) (GLuint);
#else
extern void glGlobalAlphaFactorbSUN (GLbyte);
extern void glGlobalAlphaFactorsSUN (GLshort);
extern void glGlobalAlphaFactoriSUN (GLint);
extern void glGlobalAlphaFactorfSUN (GLfloat);
extern void glGlobalAlphaFactordSUN (GLdouble);
extern void glGlobalAlphaFactorubSUN (GLubyte);
extern void glGlobalAlphaFactorusSUN (GLushort);
extern void glGlobalAlphaFactoruiSUN (GLuint);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SUN_triangle_list
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glReplacementCodeuiSUNProcPtr) (GLuint);
typedef void (* glReplacementCodeusSUNProcPtr) (GLushort);
typedef void (* glReplacementCodeubSUNProcPtr) (GLubyte);
typedef void (* glReplacementCodeuivSUNProcPtr) (const GLuint *);
typedef void (* glReplacementCodeusvSUNProcPtr) (const GLushort *);
typedef void (* glReplacementCodeubvSUNProcPtr) (const GLubyte *);
typedef void (* glReplacementCodePointerSUNProcPtr) (GLenum, GLsizei, const GLvoid* *);
#else
extern void glReplacementCodeuiSUN (GLuint);
extern void glReplacementCodeusSUN (GLushort);
extern void glReplacementCodeubSUN (GLubyte);
extern void glReplacementCodeuivSUN (const GLuint *);
extern void glReplacementCodeusvSUN (const GLushort *);
extern void glReplacementCodeubvSUN (const GLubyte *);
extern void glReplacementCodePointerSUN (GLenum, GLsizei, const GLvoid* *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SUN_vertex
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glColor4ubVertex2fSUNProcPtr) (GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat);
typedef void (* glColor4ubVertex2fvSUNProcPtr) (const GLubyte *, const GLfloat *);
typedef void (* glColor4ubVertex3fSUNProcPtr) (GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
typedef void (* glColor4ubVertex3fvSUNProcPtr) (const GLubyte *, const GLfloat *);
typedef void (* glColor3fVertex3fSUNProcPtr) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glColor3fVertex3fvSUNProcPtr) (const GLfloat *, const GLfloat *);
typedef void (* glNormal3fVertex3fSUNProcPtr) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glNormal3fVertex3fvSUNProcPtr) (const GLfloat *, const GLfloat *);
typedef void (* glColor4fNormal3fVertex3fSUNProcPtr) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glColor4fNormal3fVertex3fvSUNProcPtr) (const GLfloat *, const GLfloat *, const GLfloat *);
typedef void (* glTexCoord2fVertex3fSUNProcPtr) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glTexCoord2fVertex3fvSUNProcPtr) (const GLfloat *, const GLfloat *);
typedef void (* glTexCoord4fVertex4fSUNProcPtr) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glTexCoord4fVertex4fvSUNProcPtr) (const GLfloat *, const GLfloat *);
typedef void (* glTexCoord2fColor4ubVertex3fSUNProcPtr) (GLfloat, GLfloat, GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
typedef void (* glTexCoord2fColor4ubVertex3fvSUNProcPtr) (const GLfloat *, const GLubyte *, const GLfloat *);
typedef void (* glTexCoord2fColor3fVertex3fSUNProcPtr) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glTexCoord2fColor3fVertex3fvSUNProcPtr) (const GLfloat *, const GLfloat *, const GLfloat *);
typedef void (* glTexCoord2fNormal3fVertex3fSUNProcPtr) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glTexCoord2fNormal3fVertex3fvSUNProcPtr) (const GLfloat *, const GLfloat *, const GLfloat *);
typedef void (* glTexCoord2fColor4fNormal3fVertex3fSUNProcPtr) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glTexCoord2fColor4fNormal3fVertex3fvSUNProcPtr) (const GLfloat *, const GLfloat *, const GLfloat *, const GLfloat *);
typedef void (* glTexCoord4fColor4fNormal3fVertex4fSUNProcPtr) (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glTexCoord4fColor4fNormal3fVertex4fvSUNProcPtr) (const GLfloat *, const GLfloat *, const GLfloat *, const GLfloat *);
typedef void (* glReplacementCodeuiVertex3fSUNProcPtr) (GLenum, GLfloat, GLfloat, GLfloat);
typedef void (* glReplacementCodeuiVertex3fvSUNProcPtr) (const GLenum *, const GLfloat *);
typedef void (* glReplacementCodeuiColor4ubVertex3fSUNProcPtr) (GLenum, GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
typedef void (* glReplacementCodeuiColor4ubVertex3fvSUNProcPtr) (const GLenum *, const GLubyte *, const GLfloat *);
typedef void (* glReplacementCodeuiColor3fVertex3fSUNProcPtr) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glReplacementCodeuiColor3fVertex3fvSUNProcPtr) (const GLenum *, const GLfloat *, const GLfloat *);
typedef void (* glReplacementCodeuiNormal3fVertex3fSUNProcPtr) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glReplacementCodeuiNormal3fVertex3fvSUNProcPtr) (const GLenum *, const GLfloat *, const GLfloat *);
typedef void (* glReplacementCodeuiColor4fNormal3fVertex3fSUNProcPtr) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glReplacementCodeuiColor4fNormal3fVertex3fvSUNProcPtr) (const GLenum *, const GLfloat *, const GLfloat *, const GLfloat *);
typedef void (* glReplacementCodeuiTexCoord2fVertex3fSUNProcPtr) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glReplacementCodeuiTexCoord2fVertex3fvSUNProcPtr) (const GLenum *, const GLfloat *, const GLfloat *);
typedef void (* glReplacementCodeuiTexCoord2fNormal3fVertex3fSUNProcPtr) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUNProcPtr) (const GLenum *, const GLfloat *, const GLfloat *, const GLfloat *);
typedef void (* glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUNProcPtr) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (* glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUNProcPtr) (const GLenum *, const GLfloat *, const GLfloat *, const GLfloat *, const GLfloat *);
#else
extern void glColor4ubVertex2fSUN (GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat);
extern void glColor4ubVertex2fvSUN (const GLubyte *, const GLfloat *);
extern void glColor4ubVertex3fSUN (GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
extern void glColor4ubVertex3fvSUN (const GLubyte *, const GLfloat *);
extern void glColor3fVertex3fSUN (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glColor3fVertex3fvSUN (const GLfloat *, const GLfloat *);
extern void glNormal3fVertex3fSUN (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glNormal3fVertex3fvSUN (const GLfloat *, const GLfloat *);
extern void glColor4fNormal3fVertex3fSUN (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glColor4fNormal3fVertex3fvSUN (const GLfloat *, const GLfloat *, const GLfloat *);
extern void glTexCoord2fVertex3fSUN (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glTexCoord2fVertex3fvSUN (const GLfloat *, const GLfloat *);
extern void glTexCoord4fVertex4fSUN (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glTexCoord4fVertex4fvSUN (const GLfloat *, const GLfloat *);
extern void glTexCoord2fColor4ubVertex3fSUN (GLfloat, GLfloat, GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
extern void glTexCoord2fColor4ubVertex3fvSUN (const GLfloat *, const GLubyte *, const GLfloat *);
extern void glTexCoord2fColor3fVertex3fSUN (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glTexCoord2fColor3fVertex3fvSUN (const GLfloat *, const GLfloat *, const GLfloat *);
extern void glTexCoord2fNormal3fVertex3fSUN (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glTexCoord2fNormal3fVertex3fvSUN (const GLfloat *, const GLfloat *, const GLfloat *);
extern void glTexCoord2fColor4fNormal3fVertex3fSUN (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glTexCoord2fColor4fNormal3fVertex3fvSUN (const GLfloat *, const GLfloat *, const GLfloat *, const GLfloat *);
extern void glTexCoord4fColor4fNormal3fVertex4fSUN (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glTexCoord4fColor4fNormal3fVertex4fvSUN (const GLfloat *, const GLfloat *, const GLfloat *, const GLfloat *);
extern void glReplacementCodeuiVertex3fSUN (GLenum, GLfloat, GLfloat, GLfloat);
extern void glReplacementCodeuiVertex3fvSUN (const GLenum *, const GLfloat *);
extern void glReplacementCodeuiColor4ubVertex3fSUN (GLenum, GLubyte, GLubyte, GLubyte, GLubyte, GLfloat, GLfloat, GLfloat);
extern void glReplacementCodeuiColor4ubVertex3fvSUN (const GLenum *, const GLubyte *, const GLfloat *);
extern void glReplacementCodeuiColor3fVertex3fSUN (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glReplacementCodeuiColor3fVertex3fvSUN (const GLenum *, const GLfloat *, const GLfloat *);
extern void glReplacementCodeuiNormal3fVertex3fSUN (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glReplacementCodeuiNormal3fVertex3fvSUN (const GLenum *, const GLfloat *, const GLfloat *);
extern void glReplacementCodeuiColor4fNormal3fVertex3fSUN (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glReplacementCodeuiColor4fNormal3fVertex3fvSUN (const GLenum *, const GLfloat *, const GLfloat *, const GLfloat *);
extern void glReplacementCodeuiTexCoord2fVertex3fSUN (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glReplacementCodeuiTexCoord2fVertex3fvSUN (const GLenum *, const GLfloat *, const GLfloat *);
extern void glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN (const GLenum *, const GLfloat *, const GLfloat *, const GLfloat *);
extern void glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN (GLenum, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
extern void glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN (const GLenum *, const GLfloat *, const GLfloat *, const GLfloat *, const GLfloat *);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif

#if GL_SUNX_constant_data
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glFinishTextureSUNXProcPtr) (void);
#else
extern void glFinishTextureSUNX (void);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif


#if GL_3DFX_tbuffer
#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glTbufferMask3DFXProcPtr) (GLuint);
#else
extern void glTbufferMask3DFX (GLuint);
#endif /* GL_GLEXT_FUNCTION_POINTERS */
#endif


#ifdef GL_GLEXT_FUNCTION_POINTERS
typedef void (* glBeginConditionalRenderNVProcPtr) (GLuint id, GLenum mode);
typedef void (* glEndConditionalRenderNVProcPtr) ();
#else
extern void glBeginConditionalRenderNV (GLuint id, GLenum mode);
extern void glEndConditionalRenderNV (void);
#endif
	
#ifdef __cplusplus
}
#endif

#endif

