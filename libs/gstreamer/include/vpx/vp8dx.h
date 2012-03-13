/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


#include "vp8.h"

/*!\defgroup vp8_decoder WebM VP8 Decoder
 * \ingroup vp8
 *
 * @{
 */
/*!\file vp8dx.h
 * \brief Provides definitions for using the VP8 algorithm within the vpx Decoder
 *        interface.
 */
#ifndef VP8DX_H
#define VP8DX_H
#include "vpx/vpx_codec_impl_top.h"

/*!\brief Algorithm interface for VP8
 *
 * This interface provides the capability to decode raw VP8 streams, as would
 * be found in AVI files and other non-Flash uses.
 */
extern vpx_codec_iface_t  vpx_codec_vp8_dx_algo;
extern vpx_codec_iface_t* vpx_codec_vp8_dx(void);

/* Include controls common to both the encoder and decoder */
#include "vp8.h"


/*! @} - end defgroup vp8_decoder */


#include "vpx/vpx_codec_impl_bottom.h"
#endif
