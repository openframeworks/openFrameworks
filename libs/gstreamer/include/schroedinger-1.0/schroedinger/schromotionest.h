
#ifndef __SCHRO_MOTIONEST_H__
#define __SCHRO_MOTIONEST_H__

#include <schroedinger/schroencoder.h>

SCHRO_BEGIN_DECLS

typedef struct _SchroMotionEst SchroMotionEst;
typedef struct _SchroRoughME SchroRoughME;
typedef struct _SchroBlock SchroBlock;

/* supports hierarchical block-matching motion estimation */
typedef struct _SchroHierBm SchroHierBm;
/* supports motion estimation */
typedef struct _SchroMe SchroMe;

#ifdef SCHRO_ENABLE_UNSTABLE_API

#define SCHRO_MAX_HIER_LEVELS 8

struct _SchroHierBm {
  int                ref_count;
  int                ref;
  int                hierarchy_levels;
  SchroParams*       params;
  SchroFrame**       downsampled_src;
  SchroFrame**       downsampled_ref;
  SchroMotionField** downsampled_mf;
  schro_bool         use_chroma;
};

struct _SchroMotionEst {
  SchroEncoderFrame *encoder_frame;
  SchroParams *params;

  double lambda;

  SchroFrame *downsampled_src0[SCHRO_MAX_HIER_LEVELS];
  SchroFrame *downsampled_src1[SCHRO_MAX_HIER_LEVELS];

  SchroMotion *motion;

  int scan_distance;

  int badblocks;
  double hier_score;
};

struct _SchroRoughME {
  SchroEncoderFrame *encoder_frame;
  SchroEncoderFrame *ref_frame;

  SchroMotionField *motion_fields[SCHRO_MAX_HIER_LEVELS];
};

struct _SchroBlock {
  int valid;
  int error;
  int entropy;

  double score;

  SchroMotionVector mv[4][4];
};

SchroMotionEst *schro_motionest_new (SchroEncoderFrame *frame);
void schro_motionest_free (SchroMotionEst *me);

SchroRoughME * schro_rough_me_new (SchroEncoderFrame *frame, SchroEncoderFrame *ref);
void schro_rough_me_free (SchroRoughME *rme);
void schro_rough_me_heirarchical_scan (SchroRoughME *rme);
void schro_rough_me_heirarchical_scan_nohint (SchroRoughME *rme, int shift,
    int distance);
void schro_rough_me_heirarchical_scan_hint (SchroRoughME *rme, int shift,
    int distance);

void schro_encoder_hierarchical_block_matching (SchroEncoderFrame* frame);

void schro_encoder_motion_predict_rough (SchroEncoderFrame *frame);
void schro_encoder_motion_predict_pel (SchroEncoderFrame *frame);
void schro_encoder_motion_predict_subpel (SchroEncoderFrame *frame);
void schro_encoder_motion_predict_subpel_deep (SchroMe *me);

void schro_encoder_global_estimation (SchroEncoderFrame *frame);

SchroMotionField * schro_motion_field_new (int x_num_blocks, int y_num_blocks);
void schro_motion_field_free (SchroMotionField *field);
void schro_motion_field_scan (SchroMotionField *field, SchroParams *params, SchroFrame *frame, SchroFrame *ref, int dist);
void schro_motion_field_inherit (SchroMotionField *field, SchroMotionField *parent);
void schro_motion_field_copy (SchroMotionField *field, SchroMotionField *parent);
void schro_motion_field_set (SchroMotionField *field, int split, int pred_mode);

int schro_frame_get_metric (SchroFrame *frame1, int x1, int y1,
    SchroFrame *frame2, int x2, int y2);
void schro_motion_field_lshift (SchroMotionField *mf, int n);

int schro_motion_estimate_entropy (SchroMotion *motion);
int schro_motion_block_estimate_entropy (SchroMotion *motion, int i, int j);
int schro_motion_superblock_estimate_entropy (SchroMotion *motion, int i, int j);
int schro_motion_superblock_try_estimate_entropy (SchroMotion *motion, int i,
    int j, SchroBlock *block);
int schro_motionest_superblock_get_metric (SchroMotionEst *me,
    SchroBlock *block, int i, int j);
void schro_motion_copy_from (SchroMotion *motion, int i, int j, SchroBlock *block);
void schro_motion_copy_to (SchroMotion *motion, int i, int j, SchroBlock *block);

void schro_block_fixup (SchroBlock *block);
int schro_block_check (SchroBlock *block);

/* SchroHierBm *interface */
SchroHierBm *schro_hbm_new ( SchroEncoderFrame* frame, int ref_number );
SchroHierBm *schro_hbm_ref ( SchroHierBm *schro_hbm );
void schro_hbm_unref ( SchroHierBm* schro_hbm );

void schro_hbm_scan ( SchroHierBm *schro_hbm );
void schro_hierarchical_bm_scan_hint ( SchroHierBm *schro_hbm, int shift, int h_range );

SchroMotionField* schro_hbm_best_mf ( SchroHierBm *schro_hbm );
SchroMotionField* schro_hbm_motion_field ( SchroHierBm *schro_hbm, int level);

/* SchroMe interface */
SchroMe *schro_me_new (SchroEncoderFrame* framer );
void schro_me_free (SchroMe* me );

SchroFrame* schro_me_src (SchroMe *me );
SchroUpsampledFrame* schro_me_ref (SchroMe *me, int ref_number );

SchroMotionField* schro_me_subpel_mf ( SchroMe *me, int ref_number );
void schro_me_set_subpel_mf ( SchroMe *me, SchroMotionField* mf, int ref_number );
SchroMotionField* schro_me_split2_mf ( SchroMe *me, int ref_number );
void schro_me_set_split2_mf ( SchroMe *me, SchroMotionField* mf, int ref_number );
SchroMotionField* schro_me_split1_mf ( SchroMe *me, int ref_number );
void schro_me_set_split1_mf ( SchroMe *me, SchroMotionField* mf, int ref_number );
SchroMotionField* schro_me_split0_mf ( SchroMe *me, int ref_number );
void schro_me_set_split0_mf ( SchroMe *me, SchroMotionField* mf, int ref_number );

SchroHierBm *schro_me_hbm (SchroMe *me, int ref_number);

void schro_me_set_lambda ( SchroMe *me, double lambda );
double schro_me_lambda ( SchroMe *me );
SchroParams* schro_me_params ( SchroMe *me );
SchroMotion* schro_me_motion ( SchroMe *me );
void schro_me_set_motion ( SchroMe *me, SchroMotion* motion );
void schro_me_set_mc_error ( SchroMe *me, double mc_err );
double schro_me_mc_error ( SchroMe *me );
void schro_me_set_badblock_ratio ( SchroMe *me, double badblocks_ratio );
double schro_me_badblocks_ratio ( SchroMe *me );
void schro_me_set_dcblock_ratio ( SchroMe *me, double dcblock_ratio );
double schro_me_dcblock_ratio ( SchroMe *me );
void schro_mode_decision (SchroMe *me);

void schro_motion_calculate_stats (SchroMotion *motion, SchroEncoderFrame *frame);

void schro_motionest_superblock_phasecorr1 (SchroMotionEst *me, int ref,
    SchroBlock *block, int i, int j);
void schro_motionest_superblock_global (SchroMotionEst *me, int ref,
    SchroBlock *block, int i, int j);

#endif

SCHRO_END_DECLS

#endif

