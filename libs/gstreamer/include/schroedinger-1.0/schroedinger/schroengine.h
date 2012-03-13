
#ifndef _SCHRO_ENGINE_H_
#define _SCHRO_ENGINE_H_

#include <schroedinger/schroencoder.h>

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

typedef enum {
  SCHRO_QUANTISER_ENGINE_SIMPLE,
  SCHRO_QUANTISER_ENGINE_RDO_BIT_ALLOCATION,
  SCHRO_QUANTISER_ENGINE_CBR,
  SCHRO_QUANTISER_ENGINE_LOSSLESS,
  SCHRO_QUANTISER_ENGINE_LOWDELAY,
  SCHRO_QUANTISER_ENGINE_RDO_LAMBDA,
  SCHRO_QUANTISER_ENGINE_CONSTANT_ERROR
} SchroQuantiserEngineEnum;

int schro_encoder_engine_intra_only (SchroEncoder *encoder);
int schro_encoder_engine_backref (SchroEncoder *encoder);
int schro_encoder_engine_test_intra (SchroEncoder *encoder);
int schro_encoder_engine_lossless (SchroEncoder *encoder);
int schro_encoder_engine_backtest (SchroEncoder *encoder);
int schro_encoder_engine_lowdelay (SchroEncoder *encoder);

int schro_encoder_handle_quants (SchroEncoder *encoder, int i);
void schro_encoder_init_frame (SchroEncoderFrame *frame);

void schro_encoder_handle_gop_backref (SchroEncoder *encoder, int i);
int schro_encoder_setup_frame_backref (SchroEncoderFrame *frame);

void schro_encoder_handle_gop_intra_only (SchroEncoder *encoder, int i);
int schro_encoder_setup_frame_intra_only (SchroEncoderFrame *frame);

void schro_encoder_handle_gop_tworef (SchroEncoder *encoder, int i);
int schro_encoder_setup_frame_tworef (SchroEncoderFrame *frame);

void schro_encoder_handle_gop_lowdelay (SchroEncoder *encoder, int i);
int schro_encoder_setup_frame_lowdelay (SchroEncoderFrame *frame);

void schro_encoder_handle_gop_lossless (SchroEncoder *encoder, int i);
int schro_encoder_setup_frame_lossless (SchroEncoderFrame *frame);

void schro_frame_set_wavelet_params (SchroEncoderFrame* frame);

void init_params (SchroEncoderFrame* frame);

#endif

SCHRO_END_DECLS

#endif

