
#ifndef SCHRO_METRIC_H
#define SCHRO_METRIC_H

#include <schroedinger/schroutils.h>
#include <schroedinger/schroframe.h>
#include <limits.h>

SCHRO_BEGIN_DECLS

typedef struct _SchroMetricScan SchroMetricScan;
typedef struct _SchroMetricInfo SchroMetricInfo;

#ifdef SCHRO_ENABLE_UNSTABLE_API

#define SCHRO_LIMIT_METRIC_SCAN 42

struct _SchroMetricInfo {
  SchroFrame *frame;
  SchroFrame *ref_frame;

  int block_width[3];
  int block_height[3];
  int h_shift[3];
  int v_shift[3];

  int (*metric) (SchroMetricInfo *info, int ref_x, int ref_y,
      int dx, int dy);
  int (*metric_right) (SchroMetricInfo *info, int ref_x, int ref_y,
      int dx, int dy);
  int (*metric_bottom) (SchroMetricInfo *info, int ref_x, int ref_y,
      int dx, int dy);
  int (*metric_corner) (SchroMetricInfo *info, int ref_x, int ref_y,
      int dx, int dy);

};

struct _SchroMetricScan {
  SchroFrame *frame;
  SchroFrame *ref_frame;
  int block_width;
  int block_height;
  int x, y;
  int ref_x, ref_y;
  int scan_width;
  int scan_height;
  int gravity_scale;
  int gravity_x, gravity_y;
  int use_chroma;
  /* output */
  uint32_t metrics[SCHRO_LIMIT_METRIC_SCAN*SCHRO_LIMIT_METRIC_SCAN];
  uint32_t chroma_metrics[SCHRO_LIMIT_METRIC_SCAN*SCHRO_LIMIT_METRIC_SCAN];
};

#define SCHRO_METRIC_INVALID INT_MAX

int schro_metric_absdiff_u8 (uint8_t *a, int a_stride, uint8_t *b,
    int b_stride, int width, int height);
int schro_metric_haar (uint8_t *src1, int stride1, uint8_t *src2, int stride2,
    int width, int height);
int schro_metric_haar_const (uint8_t *data, int stride, int dc_value,
    int width, int height);
int schro_metric_abssum_s16 (int16_t *data, int stride, int width, int height);
int schro_metric_sum_u8 (uint8_t *data, int stride, int width, int height);

void schro_metric_scan_do_scan (SchroMetricScan *scan);
int schro_metric_scan_get_min (SchroMetricScan *scan, int *dx, int *dy
    , uint32_t* chroma_metric);
void schro_metric_scan_setup (SchroMetricScan *scan, int dx, int dy, int dist
    , int use_chroma);

int schro_metric_get (SchroFrameData *src1, SchroFrameData *src2, int width, int height);
int schro_metric_get_biref (SchroFrameData *fd, SchroFrameData *src1,
    int weight1, SchroFrameData *src2, int weight2, int shift, int width,
    int height);
int schro_metric_get_dc (SchroFrameData *src, int value, int width, int height);

void schro_metric_info_init (SchroMetricInfo *info, SchroFrame *frame,
    SchroFrame *ref_frame, int block_width, int block_height);

int schro_metric_fast_block (SchroMetricInfo *info, int x, int y,
    int dx, int dy);
int schro_metric_fast_subsuperblock (SchroMetricInfo *info, int x, int y,
    int dx, int dy);
int schro_metric_fast_superblock (SchroMetricInfo *info, int x, int y,
    int dx, int dy);


#endif

SCHRO_END_DECLS

#endif

