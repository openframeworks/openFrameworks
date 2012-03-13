
#ifndef __SCHRO_ENCODER_H__
#define __SCHRO_ENCODER_H__

#include <schroedinger/schroutils.h>
#include <schroedinger/schropack.h>
#include <schroedinger/schrobuffer.h>
#include <schroedinger/schroparams.h>
#include <schroedinger/schroframe.h>
#include <schroedinger/schroasync.h>
#include <schroedinger/schroqueue.h>
#include <schroedinger/schromotion.h>
#include <schroedinger/schrohistogram.h>
#include <schroedinger/schrolist.h>

SCHRO_BEGIN_DECLS

typedef struct _SchroEncoder SchroEncoder;
typedef struct _SchroEncoderFrame SchroEncoderFrame;
typedef struct _SchroEncoderSetting SchroEncoderSetting;

typedef enum {
  SCHRO_STATE_NEED_FRAME,
  SCHRO_STATE_HAVE_BUFFER,
  SCHRO_STATE_AGAIN,
  SCHRO_STATE_END_OF_STREAM
} SchroStateEnum;

#ifdef SCHRO_ENABLE_UNSTABLE_API
typedef enum {
  SCHRO_ENCODER_FRAME_STAGE_NEW = 0,
  SCHRO_ENCODER_FRAME_STAGE_ANALYSE,
  SCHRO_ENCODER_FRAME_STAGE_SC_DETECT_1,
  SCHRO_ENCODER_FRAME_STAGE_SC_DETECT_2,
  SCHRO_ENCODER_FRAME_STAGE_HAVE_GOP,
  SCHRO_ENCODER_FRAME_STAGE_HAVE_PARAMS,
  SCHRO_ENCODER_FRAME_STAGE_PREDICT_ROUGH,
  SCHRO_ENCODER_FRAME_STAGE_PREDICT_PEL,
  SCHRO_ENCODER_FRAME_STAGE_PREDICT_SUBPEL,
  SCHRO_ENCODER_FRAME_STAGE_MODE_DECISION,
  SCHRO_ENCODER_FRAME_STAGE_HAVE_REFS,
  SCHRO_ENCODER_FRAME_STAGE_HAVE_QUANTS,
  SCHRO_ENCODER_FRAME_STAGE_ENCODING,
  SCHRO_ENCODER_FRAME_STAGE_RECONSTRUCT,
  SCHRO_ENCODER_FRAME_STAGE_POSTANALYSE,
  SCHRO_ENCODER_FRAME_STAGE_DONE,
  SCHRO_ENCODER_FRAME_STAGE_FREE,
  SCHRO_ENCODER_FRAME_STAGE_LAST /* this should be last */
} SchroEncoderFrameStateEnum;
#endif

typedef enum {
  SCHRO_ENCODER_PERCEPTUAL_CONSTANT,
  SCHRO_ENCODER_PERCEPTUAL_CCIR959,
  SCHRO_ENCODER_PERCEPTUAL_MOO,
  SCHRO_ENCODER_PERCEPTUAL_MANOS_SAKRISON
} SchroEncoderPerceptualEnum;

typedef enum {
  SCHRO_ENCODER_RATE_CONTROL_CONSTANT_NOISE_THRESHOLD,
  SCHRO_ENCODER_RATE_CONTROL_CONSTANT_BITRATE,
  SCHRO_ENCODER_RATE_CONTROL_LOW_DELAY,
  SCHRO_ENCODER_RATE_CONTROL_LOSSLESS,
  SCHRO_ENCODER_RATE_CONTROL_CONSTANT_LAMBDA,
  SCHRO_ENCODER_RATE_CONTROL_CONSTANT_ERROR,
  SCHRO_ENCODER_RATE_CONTROL_CONSTANT_QUALITY
} SchroEncoderRateControlEnum;

typedef enum {
  SCHRO_ENCODER_GOP_ADAPTIVE,
  SCHRO_ENCODER_GOP_INTRA_ONLY,
  SCHRO_ENCODER_GOP_BACKREF,
  SCHRO_ENCODER_GOP_CHAINED_BACKREF,
  SCHRO_ENCODER_GOP_BIREF,
  SCHRO_ENCODER_GOP_CHAINED_BIREF,
} SchroEncoderGOPEnum;

typedef enum {
  SCHRO_ENCODER_PROFILE_AUTO,
  SCHRO_ENCODER_PROFILE_VC2_LOW_DELAY,
  SCHRO_ENCODER_PROFILE_VC2_SIMPLE,
  SCHRO_ENCODER_PROFILE_VC2_MAIN,
  SCHRO_ENCODER_PROFILE_MAIN
} SchroEncoderProfile;

#ifdef SCHRO_ENABLE_UNSTABLE_API
typedef int (*SchroEngineIterateFunc) (SchroEncoder *encoder);

/* forward declaration */
struct _SchroMotionEst;
struct _SchroRoughME;

struct _SchroEncoderFrame {
  /*< private >*/
  int refcount;
  //SchroEncoderFrameStateEnum state;
  //SchroEncoderFrameStateEnum needed_state;
  SchroEncoderFrameStateEnum working;
  int busy;

  void *priv;

  unsigned int expired_reference;

  /* Bits telling the engine stages which stuff needs to happen */
  unsigned int need_extension;
  unsigned int need_downsampling;
  unsigned int need_upsampling;
  unsigned int need_filtering;
  unsigned int need_average_luma;
  unsigned int need_mad;

  /* bits indicating that a particular analysis has happened.  Mainly
   * for verification */
  unsigned int have_estimate_tables;
  unsigned int have_histograms;
  unsigned int have_scene_change_score;
  unsigned int have_downsampling;
  unsigned int have_upsampling;
  unsigned int have_average_luma;
  unsigned int have_mad;

  SchroAsyncStage stages[SCHRO_ENCODER_FRAME_STAGE_LAST];

  /* other stuff */

  int start_sequence_header;
  int gop_length;

  SchroPictureNumber frame_number;
  SchroFrame *original_frame;
  SchroFrame *filtered_frame;
  SchroFrame *downsampled_frames[8];
  SchroUpsampledFrame *reconstructed_frame;
  SchroUpsampledFrame *upsampled_original_frame;

  int sc_mad; /* shot change mean absolute difference */
  double sc_threshold; /* shot change threshold */

  SchroBuffer *sequence_header_buffer;
  SchroList *inserted_buffers;
  int output_buffer_size;
  SchroBuffer *output_buffer;
  int presentation_frame;
  int slot;
  int last_frame;

  int is_ref;
  int num_refs;
  SchroPictureNumber picture_number_ref[2];
  SchroPictureNumber retired_picture_number;

  int16_t slice_y_dc_values[100];
  int16_t slice_u_dc_values[100];
  int16_t slice_v_dc_values[100];
  int slice_y_n;
  int slice_uv_n;
  int slice_y_bits;
  int slice_uv_bits;
  int slice_y_trailing_zeros;
  int slice_uv_trailing_zeros;
  SchroFrameData luma_subbands[SCHRO_LIMIT_SUBBANDS];
  SchroFrameData chroma1_subbands[SCHRO_LIMIT_SUBBANDS];
  SchroFrameData chroma2_subbands[SCHRO_LIMIT_SUBBANDS];

  /* from the old SchroEncoderTask */

  int stats_dc;
  int stats_global;
  int stats_motion;

  int subband_size;
  SchroBuffer *subband_buffer;

  int16_t *quant_data;

  int *quant_indices[3][SCHRO_LIMIT_SUBBANDS];

  double est_entropy[3][SCHRO_LIMIT_SUBBANDS][60];
  double actual_subband_bits[3][SCHRO_LIMIT_SUBBANDS];
  double est_error[3][SCHRO_LIMIT_SUBBANDS][60];
  SchroPack *pack;
  SchroParams params;
  SchroEncoder *encoder;
  SchroFrame *iwt_frame;
  SchroFrame *quant_frame;
  SchroFrame *prediction_frame;

  SchroEncoderFrame *previous_frame;
  SchroEncoderFrame *ref_frame[2];

  struct _SchroMotionEst *me;
  struct _SchroRoughME *rme[2];
  struct _SchroPhaseCorr *phasecorr[2];
  struct _SchroHierBm *hier_bm[2];
  struct _SchroMe *deep_me;

  SchroMotion *motion;

  SchroHistogram subband_hists[3][SCHRO_LIMIT_SUBBANDS];
  SchroHistogram hist_test;

  /* statistics */

  double picture_weight;
  double scene_change_score;
  double average_luma;

  int hard_limit_bits;
  int allocated_residual_bits;
  int allocated_mc_bits;
  double frame_lambda;
  double frame_me_lambda;
  int estimated_residual_bits;
  int estimated_mc_bits;

  int actual_residual_bits;
  int actual_mc_bits;
  double mc_error;
  double mean_squared_error_luma;
  double mean_squared_error_chroma;
  double mssim;

  double estimated_arith_context_ratio;

  double badblock_ratio;
  double dcblock_ratio;
  double hist_slope;
};

struct _SchroEncoder {
  /*< private >*/
  SchroAsync *async;

  SchroPictureNumber next_frame_number;

  SchroQueue *frame_queue;

  SchroEncoderFrame *reference_pictures[SCHRO_LIMIT_REFERENCE_FRAMES];
  SchroEncoderFrame *last_frame;

  int assemble_packets;
  int need_rap;

  SchroVideoFormat video_format;
  int version_major;
  int version_minor;

  /* configuration */
  int rate_control;
  int bitrate;
  int max_bitrate;
  int min_bitrate;

  // Buffer model parameters for CBR and (TODO) constrained VBR coding
  int buffer_size;
  int buffer_level;
  double quality;
  double noise_threshold;
  int gop_structure;
  int queue_depth;
  int perceptual_weighting;
  double perceptual_distance;
  int filtering;
  double filter_value;
  SchroEncoderProfile force_profile;
  int profile;
  int level;
  int open_gop;
  int au_distance;
  int max_refs;
  schro_bool enable_psnr;
  schro_bool enable_ssim;
  schro_bool enable_md5;

  int transform_depth;
  int intra_wavelet;
  int inter_wavelet;
  int mv_precision;
  int motion_block_size;
  int motion_block_overlap;
  schro_bool interlaced_coding;
  schro_bool enable_internal_testing;
  schro_bool enable_noarith;
  schro_bool enable_fullscan_estimation;
  schro_bool enable_hierarchical_estimation;
  schro_bool enable_zero_estimation;
  schro_bool enable_phasecorr_estimation;
  schro_bool enable_bigblock_estimation;
  schro_bool enable_multiquant;
  schro_bool enable_dc_multiquant;
  schro_bool enable_global_motion;
  schro_bool enable_scene_change_detection;
  schro_bool enable_deep_estimation;
  schro_bool enable_rdo_cbr;
  schro_bool enable_chroma_me;
  int horiz_slices;
  int vert_slices;
  int codeblock_size;

  double magic_dc_metric_offset;
  double magic_subband0_lambda_scale;
  double magic_chroma_lambda_scale;
  double magic_nonref_lambda_scale;
  double magic_I_lambda_scale;
  double magic_P_lambda_scale;
  double magic_B_lambda_scale;
  double magic_me_lambda_scale;
  double magic_allocation_scale;
  double magic_inter_cpd_scale;
  double magic_keyframe_weight;
  double magic_scene_change_threshold;
  double magic_inter_p_weight;
  double magic_inter_b_weight;
  double magic_me_bailout_limit;
  double magic_bailout_weight;
  double magic_error_power;
  double magic_subgroup_length;
  double magic_badblock_multiplier_nonref;
  double magic_badblock_multiplier_ref;
  double magic_block_search_threshold;
  double magic_scan_distance;
  double magic_diagonal_lambda_scale;

  /* hooks */

  void (*init_frame) (SchroEncoderFrame *frame);
  void (*handle_gop) (SchroEncoder *encoder, int i);
  int (*setup_frame) (SchroEncoderFrame *frame);
  int (*handle_quants) (SchroEncoder *encoder, int i);

  /* other */

  int end_of_stream;
  int end_of_stream_handled;
  int end_of_stream_pulled;
  int completed_eos;
  int prev_offset;
  int force_sequence_header;

  SchroPictureNumber au_frame;
  int next_slot;

  int output_slot;

  SchroList *inserted_buffers;
  int queue_changed;

  int engine_init;
  SchroEngineIterateFunc engine_iterate;
  int quantiser_engine;

  double start_time;
  int downsample_levels;

  /* internal stuff */

  double cycles_per_degree_horiz;
  double cycles_per_degree_vert;

  double intra_subband_weights[SCHRO_N_WAVELETS][SCHRO_LIMIT_TRANSFORM_DEPTH][SCHRO_LIMIT_SUBBANDS];
  double inter_subband_weights[SCHRO_N_WAVELETS][SCHRO_LIMIT_TRANSFORM_DEPTH][SCHRO_LIMIT_SUBBANDS];
  SchroHistogramTable intra_hist_tables[60];


  /* statistics */

  double average_arith_context_ratios_intra[3][SCHRO_LIMIT_SUBBANDS];
  double average_arith_context_ratios_inter[3][SCHRO_LIMIT_SUBBANDS];

  double frame_stats[21];

  /* engine specific stuff */

  int bits_per_picture;
  int subgroup_position;
  int I_complexity;
  int P_complexity;
  int B_complexity;
  int B_complexity_sum;
  long int I_frame_alloc;
  long int P_frame_alloc;
  long int B_frame_alloc;
  long int gop_target;

  // Current qf, from which is derived ...
  double qf;
  // lambda to use for intra pictures in CBR mode
  double intra_cbr_lambda;

  int gop_picture;
  int quant_slot;

  int last_ref;
};
#endif

typedef enum {
  SCHRO_ENCODER_SETTING_TYPE_BOOLEAN,
  SCHRO_ENCODER_SETTING_TYPE_INT,
  SCHRO_ENCODER_SETTING_TYPE_ENUM,
  SCHRO_ENCODER_SETTING_TYPE_DOUBLE,
  SCHRO_ENCODER_SETTING_TYPE_LAST
} SchroEncoderSettingTypeEnum;

struct _SchroEncoderSetting {
  const char *name;
  SchroEncoderSettingTypeEnum type;

  double min;
  double max;
  double default_value;

  const char **enum_list;
};

enum {
  SCHRO_MOTION_FIELD_HIER_REF0,
  SCHRO_MOTION_FIELD_HIER1_REF0,
  SCHRO_MOTION_FIELD_HIER2_REF0,
  SCHRO_MOTION_FIELD_HIER3_REF0,
  SCHRO_MOTION_FIELD_HIER_REF1,
  SCHRO_MOTION_FIELD_HIER1_REF1,
  SCHRO_MOTION_FIELD_HIER2_REF1,
  SCHRO_MOTION_FIELD_HIER3_REF1,
  SCHRO_MOTION_FIELD_PHASECORR_REF0,
  SCHRO_MOTION_FIELD_PHASECORR_REF1,
  SCHRO_MOTION_FIELD_DC,
  SCHRO_MOTION_FIELD_GLOBAL_REF0,
  SCHRO_MOTION_FIELD_GLOBAL_REF1,
  SCHRO_MOTION_FIELD_ZERO_REF0,
  SCHRO_MOTION_FIELD_ZERO_REF1,
  SCHRO_MOTION_FIELD_FULLSCAN_REF0,
  SCHRO_MOTION_FIELD_FULLSCAN_REF1,
  SCHRO_MOTION_FIELD_LAST
};

SchroEncoder * schro_encoder_new (void);
void schro_encoder_free (SchroEncoder *encoder);
SchroVideoFormat * schro_encoder_get_video_format (SchroEncoder *encoder);
void schro_encoder_set_video_format (SchroEncoder *encoder,
    SchroVideoFormat *video_format);
void schro_encoder_set_frame_lambda (SchroEncoderFrame* frame);
void schro_encoder_end_of_stream (SchroEncoder *encoder);
int schro_encoder_push_ready (SchroEncoder *encoder);
void schro_encoder_push_frame (SchroEncoder *encoder, SchroFrame *frame);
void schro_encoder_push_frame_full (SchroEncoder *encoder, SchroFrame *frame, void *priv);
void schro_encoder_force_sequence_header (SchroEncoder *encoder);

SchroBuffer * schro_encoder_encode_auxiliary_data (SchroEncoder *encoder,
    SchroAuxiliaryDataID id, const void *data, int size);
void schro_encoder_encode_parse_info (SchroPack *bits, int parse_code);
void schro_encoder_insert_buffer (SchroEncoder *encoder, SchroBuffer *buffer);
void schro_encoder_frame_insert_buffer (SchroEncoderFrame *frame, SchroBuffer *buffer);
void schro_encoder_start (SchroEncoder *encoder);

void schro_encoder_set_packet_assembly (SchroEncoder *encoder, int value);
SchroStateEnum schro_encoder_wait (SchroEncoder *encoder);
SchroBuffer * schro_encoder_pull (SchroEncoder *encoder,
    int *n_decodable_frames);
SchroBuffer * schro_encoder_pull_full (SchroEncoder *encoder, int *presentation_frame,
    void **priv);
SchroBuffer * schro_encoder_encode_sequence_header (SchroEncoder *encoder);

int schro_encoder_get_n_settings (void);
const SchroEncoderSetting *schro_encoder_get_setting_info (int i);
void schro_encoder_setting_set_double (SchroEncoder *encoder, const char *name,
    double d);
double schro_encoder_setting_get_double (SchroEncoder *encoder, const char *name);

int schro_encoder_get_frame_stats_size (SchroEncoder *encoder);
void schro_encoder_get_frame_stats (SchroEncoder *encoder, double *dest, int n);

#ifdef SCHRO_ENABLE_UNSTABLE_API

void schro_encoder_set_default_subband_weights (SchroEncoder *encoder);
void schro_encoder_calculate_subband_weights (SchroEncoder *encoder,
        double (*perceptual_weight)(double));
double schro_encoder_perceptual_weight_constant (double cpd);
double schro_encoder_perceptual_weight_ccir959 (double cpd);
double schro_encoder_perceptual_weight_moo (double cpd);
double schro_encoder_perceptual_weight_manos_sakrison (double cpd);

void schro_encoder_init_subbands (SchroEncoderFrame *frame);
void schro_encoder_encode_subband (SchroEncoderFrame *frame, int component, int index);
void schro_encoder_encode_subband_noarith (SchroEncoderFrame *frame, int component, int index);

void schro_encoder_analyse_picture (SchroAsyncStage *stage);
void schro_encoder_predict_rough_picture (SchroAsyncStage *stage);
void schro_encoder_predict_pel_picture (SchroAsyncStage *stage);
void schro_encoder_predict_subpel_picture (SchroAsyncStage *stage);

void schro_encoder_fullpel_predict_picture (SchroAsyncStage *stage);
void schro_encoder_mode_decision (SchroAsyncStage *stage);

void schro_encoder_encode_picture (SchroAsyncStage *stage);
void schro_encoder_reconstruct_picture (SchroAsyncStage *stage);
void schro_encoder_postanalyse_picture (SchroAsyncStage *stage);

SchroFrame * schro_encoder_frame_queue_get (SchroEncoder *encoder,
    SchroPictureNumber frame_number);
void schro_encoder_frame_queue_remove (SchroEncoder *encoder,
    SchroPictureNumber frame_number);
SchroEncoderFrame * schro_encoder_reference_get (SchroEncoder *encoder,
    SchroPictureNumber frame_number);
void schro_encoder_encode_picture_header (SchroEncoderFrame *frame);
SchroBuffer * schro_encoder_encode_end_of_stream (SchroEncoder *encoder);
void schro_encoder_clean_up_transform (SchroEncoderFrame *frame);
void schro_encoder_choose_quantisers (SchroEncoderFrame *frame);
void schro_encoder_output_push (SchroEncoder *encoder,
    SchroBuffer *buffer, int slot, int presentation_frame);

SchroEncoderFrame * schro_encoder_frame_new (SchroEncoder *encoder);
void schro_encoder_frame_ref (SchroEncoderFrame *frame);
void schro_encoder_frame_unref (SchroEncoderFrame *frame);
int schro_encoder_frame_is_B_frame (SchroEncoderFrame* frame);
void schro_encoder_encode_lowdelay_transform_data (SchroEncoderFrame *frame);
void schro_encoder_estimate_entropy (SchroEncoderFrame *frame);
void schro_encoder_recalculate_allocations (SchroEncoder *encoder);

void schro_encoder_calculate_test_info (SchroEncoderFrame *frame);

void schro_encoder_init_error_tables (SchroEncoder *encoder);

int schro_encoder_frame_get_quant_index (SchroEncoderFrame *frame, int component,
    int index, int x, int y);
void schro_encoder_frame_set_quant_index (SchroEncoderFrame *frame, int component,
    int index, int x, int y, int quant_index);

#endif

SCHRO_END_DECLS

#endif

