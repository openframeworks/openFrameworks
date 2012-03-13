
#ifndef __SCHRO_DECODER_H__
#define __SCHRO_DECODER_H__

#include <schroedinger/schrobuffer.h>
#include <schroedinger/schroparams.h>
#include <schroedinger/schroframe.h>
#include <schroedinger/schromotion.h>
#include <schroedinger/schrounpack.h>
#include <schroedinger/schrobitstream.h>
#include <schroedinger/schroqueue.h>
#include <schroedinger/schroasync.h>
#include <schroedinger/schroarith.h>
#include <schroedinger/schrobufferlist.h>
#include <schroedinger/schroparse.h>

SCHRO_BEGIN_DECLS

typedef struct _SchroDecoder SchroDecoder;
typedef struct _SchroDecoderInstance SchroDecoderInstance;
typedef struct _SchroPicture SchroPicture;

#ifdef SCHRO_ENABLE_UNSTABLE_API

#ifndef SCHRO_OPENGL_TYPEDEF
#define SCHRO_OPENGL_TYPEDEF
typedef struct _SchroOpenGL SchroOpenGL;
#endif

struct _SchroDecoder {
  /*< private >*/

  SchroMemoryDomain *cpu_domain;
  SchroMemoryDomain *cuda_domain;
  SchroMemoryDomain *opengl_domain;

  SchroAsync *async;

  int use_cuda;
  SchroOpenGL *opengl;
  int use_opengl;

  double skip_value;
  double skip_ratio;
  int earliest_frame;

  /* output pictures in coded order */
  int coded_order;

  int error;
  char *error_message;

  SchroBufferList *input_buflist;
  SchroParseSyncState *sps;

  /* private data that is supplied with the input bitstream and
   * is to be associated with the next picture to occur */
  SchroTag *next_picture_tag;

  SchroDecoderInstance *instance;
};

struct _SchroDecoderInstance {
  /*< private >*/

  /* the decoder this sequence instance belongs to */
  SchroDecoder *decoder;

  /* the next sequence instance to supercede this
   *  - New instances should be created whenever an EOS
   *    is detected at the input to the decoder
   *  - Old instances should be deleted whenever their
   *    state would cause an EOS to be emitted */
  SchroDecoderInstance *next;

  /* the list of reference pictures */
  SchroQueue *reference_queue;

  /* a list of frames provided by the app that we'll decode into */
  /* xxx: maybe this belongs in Decoder and not per instance */
  SchroQueue *output_queue;

  /* the last picture number to be emitted by decoder_pull().
   * used to determine if a stream jumps backwards */
  SchroPictureNumber last_picture_number;
  int last_picture_number_valid;

  int major_version;
  int minor_version;
  int profile;
  int level;
  SchroVideoFormat video_format;
  int compat_quant_offset;

  SchroQueue *reorder_queue;
  int reorder_queue_size;

  int end_of_stream;
  int flushing;

  int first_sequence_header;
  int have_sequence_header;
  SchroBuffer *sequence_header_buffer;
  int have_frame_number;

  int has_md5;
  uint8_t md5_checksum[32];
};

struct _SchroPicture {
  int refcount;

  SchroDecoderInstance *decoder_instance;

  int busy;
  int skip;
  int error;

  SchroBuffer *input_buffer;
  SchroParams params;
  SchroPictureNumber picture_number;
  SchroPictureNumber reference1;
  SchroPictureNumber reference2;
  SchroPictureNumber retired_picture_number;
  SchroPicture *ref0;
  SchroPicture *ref1;
  SchroFrame *planar_output_frame;
  SchroFrame *ref_output_frame;

  SchroAsyncStage stages[9];

  int is_ref;

  int zero_residual;

  SchroFrame *transform_frame;
  SchroFrame *frame;
  SchroFrame *mc_tmp_frame;
  SchroMotion *motion;
  SchroFrame *output_picture;
  SchroUpsampledFrame *upsampled_frame;

  int subband_length[3][SCHRO_LIMIT_SUBBANDS];
  int subband_quant_index[3][SCHRO_LIMIT_SUBBANDS];
  SchroBuffer *subband_buffer[3][SCHRO_LIMIT_SUBBANDS];
  SchroFrameData subband_data[3][SCHRO_LIMIT_SUBBANDS];

  SchroBuffer *motion_buffers[9];

  SchroBuffer *lowdelay_buffer;

  int has_md5;
  uint8_t md5_checksum[32];

  /* private data that is associated with this picture */
  SchroTag *tag;
};

#endif

enum {
  SCHRO_DECODER_OK,
  SCHRO_DECODER_ERROR,
  SCHRO_DECODER_EOS,
  SCHRO_DECODER_FIRST_ACCESS_UNIT,
  SCHRO_DECODER_NEED_BITS,
  SCHRO_DECODER_NEED_FRAME,
  SCHRO_DECODER_WAIT,
  SCHRO_DECODER_STALLED
};

enum {
  SCHRO_DECODER_PICTURE_ORDER_PRESENTATION = 0,
  SCHRO_DECODER_PICTURE_ORDER_CODED
};

SchroDecoder * schro_decoder_new (void);
void schro_decoder_free (SchroDecoder *decoder);
void schro_decoder_reset (SchroDecoder *decoder);
SchroVideoFormat * schro_decoder_get_video_format (SchroDecoder *decoder);
void schro_decoder_add_output_picture (SchroDecoder *decoder, SchroFrame *frame);
int schro_decoder_push_ready (SchroDecoder *decoder);
int schro_decoder_push (SchroDecoder *decoder, SchroBuffer *buffer);
#ifndef SCHRO_DISABLE_DEPRECATED
int schro_decoder_set_flushing (SchroDecoder *decoder, int flushing) SCHRO_DEPRECATED;
#endif
void schro_decoder_set_picture_order (SchroDecoder *decoder, int picture_order);
int schro_decoder_push_end_of_stream (SchroDecoder *decoder);
SchroFrame *schro_decoder_pull (SchroDecoder *decoder);
int schro_decoder_wait (SchroDecoder *decoder);

void schro_decoder_set_earliest_frame (SchroDecoder *decoder, SchroPictureNumber earliest_frame);
void schro_decoder_set_skip_ratio (SchroDecoder *decoder, double ratio);
SchroPictureNumber schro_decoder_get_picture_number (SchroDecoder *decoder);
int schro_decoder_need_output_frame (SchroDecoder *decoder);

int schro_decoder_autoparse_wait (SchroDecoder *decoder);
int schro_decoder_autoparse_push (SchroDecoder *decoder, SchroBuffer *buffer);
int schro_decoder_autoparse_push_end_of_sequence (SchroDecoder *decoder);

SchroTag* schro_decoder_get_picture_tag (SchroDecoder *decoder);

#ifdef SCHRO_ENABLE_UNSTABLE_API

int schro_decoder_begin_sequence (SchroDecoder *decoder);
int schro_decoder_end_sequence (SchroDecoder *decoder);

int schro_decoder_decode_parse_header (SchroUnpack *unpack);
void schro_decoder_parse_sequence_header (SchroDecoderInstance *instance, SchroUnpack *unpack);
int schro_decoder_compare_sequence_header_buffer (SchroBuffer *a, SchroBuffer *b);

void schro_decoder_subband_dc_predict (SchroFrameData *fd);

/* SchroPicture */

SchroPicture * schro_picture_new (SchroDecoderInstance *instance);
SchroPicture * schro_picture_ref (SchroPicture *picture);
void schro_picture_unref (SchroPicture *picture);

int schro_decoder_iterate_picture (SchroDecoderInstance *instance, SchroBuffer *buffer, SchroUnpack *unpack, int parse_code);
void schro_decoder_parse_picture (SchroPicture *picture, SchroUnpack *unpack);
void schro_decoder_parse_picture_header (SchroPicture *picture, SchroUnpack *unpack);
void schro_decoder_parse_picture_prediction_parameters (SchroPicture *picture, SchroUnpack *unpack);
void schro_decoder_parse_block_data (SchroPicture *picture, SchroUnpack *unpack);
void schro_decoder_parse_transform_parameters (SchroPicture *picture, SchroUnpack *unpack);
void schro_decoder_parse_transform_data (SchroPicture *picture, SchroUnpack *unpack);
void schro_decoder_parse_lowdelay_transform_data (SchroPicture *picture, SchroUnpack *unpack);
void schro_decoder_init_subband_frame_data_interleaved (SchroPicture *picture);
void schro_decoder_init_subband_frame_data (SchroPicture *picture);
void schro_decoder_decode_block_data (SchroPicture *picture);
void schro_decoder_decode_transform_data (SchroPicture *picture);
void schro_decoder_decode_lowdelay_transform_data (SchroPicture *picture);
void schro_decoder_decode_macroblock(SchroPicture *picture,
    SchroArith **arith, SchroUnpack *unpack, int i, int j);
void schro_decoder_decode_prediction_unit(SchroPicture *picture,
    SchroArith **arith, SchroUnpack *unpack, SchroMotionVector *motion_vectors,
    int x, int y);


#endif

SCHRO_END_DECLS

#endif

