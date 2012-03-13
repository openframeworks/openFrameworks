/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


/*!\defgroup vp8 VP8
 * \ingroup codecs
 * VP8 is vpx's newest video compression algorithm that uses motion
 * compensated prediction, Discrete Cosine Transform (DCT) coding of the
 * prediction error signal and context dependent entropy coding techniques
 * based on arithmatic principles. It features:
 *  - YUV 4:2:0 image format
 *  - Macro-block based coding (16x16 luma plus two 8x8 chroma)
 *  - 1/4 (1/8) pixel accuracy motion compensated prediction
 *  - 4x4 DCT transform
 *  - 128 level linear quantizer
 *  - In loop deblocking filter
 *  - Context-based entropy coding
 *
 * @{
 */
/*!\file vp8.h
 * \brief Provides controls common to both the VP8 encoder and decoder.
 */
#ifndef VP8_H
#define VP8_H
#include "vpx/vpx_codec_impl_top.h"

/*!\brief Control functions
 *
 * The set of macros define the control functions of VP8 interface
 */
enum vp8_dec_control_id
{
    VP8_SET_REFERENCE       = 1,    /**< pass in an external frame into decoder to be used as reference frame */
    VP8_COPY_REFERENCE      = 2,    /**< get a copy of reference frame from the decoder */
    VP8_SET_POSTPROC        = 3,    /**< set decoder's the post processing settings  */
    VP8_COMMON_CTRL_ID_MAX
};

/*!\brief post process flags
 *
 * The set of macros define VP8 decoder post processing flags
 */
enum vp8_postproc_level
{
    VP8_NOFILTERING    = 0,
    VP8_DEBLOCK        = 1,
    VP8_DEMACROBLOCK   = 2,
    VP8_ADDNOISE       = 4
};

/*!\brief post process flags
 *
 * This define a structure that describe the post processing settings. For
 * the best objective measure (using thet PSNR metric) set post_proc_flag
 * to VP8_DEBLOCK and deblocking_level to 1.
 */

typedef struct vp8_postproc_cfg
{
    int post_proc_flag;           /**< the types of post processing to be done, should be combination of "vp8_postproc_level" */
    int deblocking_level;        /**< the strength of deblocking, valid range [0, 16] */
    int noise_level;             /**< the strength of additive noise, valid range [0, 16] */
} vp8_postproc_cfg_t;

/*!\brief reference frame type
 *
 * The set of macros define the type of VP8 reference frames
 */
typedef enum vpx_ref_frame_type
{
    VP8_LAST_FRAME = 1,
    VP8_GOLD_FRAME = 2,
    VP8_ALTR_FRAME = 4
} vpx_ref_frame_type_t;

/*!\brief reference frame data struct
 *
 * define the data struct to access vp8 reference frames
 */

typedef struct vpx_ref_frame
{
    vpx_ref_frame_type_t  frame_type;   /**< which reference frame */
    vpx_image_t           img;          /**< reference frame data in image format */
} vpx_ref_frame_t;


/*!\brief vp8 decoder control funciton parameter type
 *
 * defines the data type for each of VP8 decoder control funciton requires
 */

VPX_CTRL_USE_TYPE(VP8_SET_REFERENCE,           vpx_ref_frame_t *)
VPX_CTRL_USE_TYPE(VP8_COPY_REFERENCE,          vpx_ref_frame_t *)
VPX_CTRL_USE_TYPE(VP8_SET_POSTPROC,            vp8_postproc_cfg_t *)


/*! @} - end defgroup vp8 */

#if !defined(VPX_CODEC_DISABLE_COMPAT) || !VPX_CODEC_DISABLE_COMPAT
/* The following definitions are provided for backward compatibility with
 * the VP8 1.0.x SDK. USE IN PRODUCTION CODE IS NOT RECOMMENDED.
 */

DECLSPEC_DEPRECATED extern vpx_codec_iface_t vpx_codec_vp8_algo DEPRECATED;
#endif

#include "vpx/vpx_codec_impl_bottom.h"
#endif
