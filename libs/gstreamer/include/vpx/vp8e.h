/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


/* This file contains backwards compatibility stubs for applications using
 * the VP8 version 1.0 API.
 */
#ifndef VP8E_H
#define VP8E_H
#include "vpx/vpx_codec_impl_top.h"

#if defined(VPX_CODEC_DISABLE_COMPAT) && VPX_CODEC_DISABLE_COMPAT
#error "Backwards compatibility disabled: don't include vp8e.h"
#endif

#include "vp8cx.h"
DECLSPEC_DEPRECATED extern vpx_codec_iface_t vpx_enc_vp8_algo DEPRECATED;


enum
{
    VP8E_SET_REFERENCE     = VP8_SET_REFERENCE,
    VP8E_COPY_REFERENCE    = VP8_COPY_REFERENCE,
    VP8E_SET_PREVIEWPP     = VP8_SET_POSTPROC,
    VP8E_SET_FLUSHFLAG     = 4,
    VP8E_SET_FRAMETYPE     = 10,
    VP8E_SET_ENCODING_MODE = 12
};

#define NORMAL_FRAME   (0)
#define KEY_FRAME      (1)

/* Change VP8E to VP8 to get the undeprecated version of these (defined in
 * vp8.h)
 */
VPX_CTRL_USE_TYPE_DEPRECATED(VP8E_SET_REFERENCE,   vpx_ref_frame_t *)
VPX_CTRL_USE_TYPE_DEPRECATED(VP8E_COPY_REFERENCE,  vpx_ref_frame_t *)
VPX_CTRL_USE_TYPE_DEPRECATED(VP8E_SET_PREVIEWPP,   vp8_postproc_cfg_t *)


/* Flush is done by calling vpx_codec_encode with a NULL input image. */
VPX_CTRL_USE_TYPE_DEPRECATED(VP8E_SET_FLUSHFLAG,          int)


/* Frame type is set with a flag to vpx_codec_control. See VPX_EFLAG_FORCE_KF
 */
VPX_CTRL_USE_TYPE_DEPRECATED(VP8E_SET_FRAMETYPE,          int)


/* This control has been deprecated in favor of the duration parameter to
 * vpx_codec_encode(). Use the #VPX_DL_REALTIME, #VPX_DL_GOOD_QUALITY,
 * #VPX_DL_BEST_QUALITY constants to that parameter instead.
 */
VPX_CTRL_USE_TYPE_DEPRECATED(VP8E_SET_ENCODING_MODE, vp8e_encoding_mode)
#include "vpx/vpx_codec_impl_bottom.h"
#endif
