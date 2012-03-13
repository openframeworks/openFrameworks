
#ifndef __SCHRO_SCHRO_HISTOGRAM_H__
#define __SCHRO_SCHRO_HISTOGRAM_H__

#include <schroedinger/schroutils.h>
#include <schroedinger/schroframe.h>

SCHRO_BEGIN_DECLS

#define SCHRO_HISTOGRAM_SHIFT 3
#define SCHRO_HISTOGRAM_SIZE ((16-SCHRO_HISTOGRAM_SHIFT)*(1<<SCHRO_HISTOGRAM_SHIFT))

typedef struct _SchroHistogram SchroHistogram;
typedef struct _SchroHistogramTable SchroHistogramTable;

struct _SchroHistogram {
  /*< private >*/
  int n;
  double bins[SCHRO_HISTOGRAM_SIZE];
};

struct _SchroHistogramTable {
  /*< private >*/
  double weights[SCHRO_HISTOGRAM_SIZE];
};

#ifdef SCHRO_ENABLE_UNSTABLE_API

double schro_histogram_get_range (SchroHistogram *hist, int start, int end);
void schro_histogram_init (SchroHistogram *hist);
void schro_histogram_add (SchroHistogram *hist, int value);
void schro_histogram_add_array_s16 (SchroHistogram *hist, int16_t *src, int n);
void schro_histogram_scale (SchroHistogram *hist, double scale);

void schro_histogram_table_generate (SchroHistogramTable *table,
    double (*func)(int value, void *priv), void *priv);
double schro_histogram_apply_table (SchroHistogram *hist,
    SchroHistogramTable *table);
double schro_histogram_apply_table_range (SchroHistogram *hist,
    SchroHistogramTable *table, int start, int end);
double schro_histogram_estimate_noise_level (SchroHistogram *hist);
double schro_histogram_estimate_slope (SchroHistogram *hist);

double schro_histogram_estimate_entropy (SchroHistogram *hist, int quant_index,
    int noarith);
void schro_frame_data_generate_histogram (SchroFrameData *fd,
    SchroHistogram *hist, int skip);
void schro_frame_data_generate_histogram_dc_predict (SchroFrameData *fd,
    SchroHistogram *hist, int skip, int x, int y);

#endif

SCHRO_END_DECLS

#endif

