
#ifndef __SCHRO_FRAME_H__
#define __SCHRO_FRAME_H__

#include <schroedinger/schroutils.h>
#include <schroedinger/schrodomain.h>

SCHRO_BEGIN_DECLS

typedef struct _SchroFrame SchroFrame;
typedef struct _SchroFrameData SchroFrameData;
typedef struct _SchroUpsampledFrame SchroUpsampledFrame;

typedef void (*SchroFrameFreeFunc)(SchroFrame *frame, void *priv);
typedef void (*SchroFrameRenderFunc)(SchroFrame *frame, void *dest, int component, int i);

/* bit pattern:
 *  0x100 - 0: normal, 1: indirect (packed)
 *  0x001 - horizontal chroma subsampling: 0: 1, 1: 2
 *  0x002 - vertical chroma subsampling: 0: 1, 1: 2
 *  0x00c - depth: 0: u8, 1: s16, 2: s32
 *  */
typedef enum _SchroFrameFormat {
  SCHRO_FRAME_FORMAT_U8_444 = 0x00,
  SCHRO_FRAME_FORMAT_U8_422 = 0x01,
  SCHRO_FRAME_FORMAT_U8_420 = 0x03,

  SCHRO_FRAME_FORMAT_S16_444 = 0x04,
  SCHRO_FRAME_FORMAT_S16_422 = 0x05,
  SCHRO_FRAME_FORMAT_S16_420 = 0x07,

  SCHRO_FRAME_FORMAT_S32_444 = 0x08,
  SCHRO_FRAME_FORMAT_S32_422 = 0x09,
  SCHRO_FRAME_FORMAT_S32_420 = 0x0b,

  /* indirectly supported */
  SCHRO_FRAME_FORMAT_YUYV = 0x100, /* YUYV order */
  SCHRO_FRAME_FORMAT_UYVY = 0x101, /* UYVY order */
  SCHRO_FRAME_FORMAT_AYUV = 0x102,
  SCHRO_FRAME_FORMAT_ARGB = 0x103,
  SCHRO_FRAME_FORMAT_RGB = 0x104,
  SCHRO_FRAME_FORMAT_v216 = 0x105,
  SCHRO_FRAME_FORMAT_v210 = 0x106
} SchroFrameFormat;

#define SCHRO_FRAME_FORMAT_DEPTH(format) ((format) & 0xc)
#define SCHRO_FRAME_FORMAT_DEPTH_U8 0x00
#define SCHRO_FRAME_FORMAT_DEPTH_S16 0x04
#define SCHRO_FRAME_FORMAT_DEPTH_S32 0x08

#define SCHRO_FRAME_FORMAT_H_SHIFT(format) ((format) & 0x1)
#define SCHRO_FRAME_FORMAT_V_SHIFT(format) (((format)>>1) & 0x1)

#define SCHRO_FRAME_IS_PACKED(format) (((format)>>8) & 0x1)

#define SCHRO_FRAME_CACHE_SIZE 32

struct _SchroFrameData {
  SchroFrameFormat format;
  void *data;
  int stride;
  int width;
  int height;
  int length;
  int h_shift;
  int v_shift;
};

struct _SchroFrame {
  int refcount;
  SchroFrameFreeFunc free;
  SchroMemoryDomain *domain;
  void *regions[3];
  void *priv;

  SchroFrameFormat format;
  int width;
  int height;

  SchroFrameData components[3];

  int is_virtual;
  int cached_lines[3][SCHRO_FRAME_CACHE_SIZE];
  SchroFrame *virt_frame1;
  SchroFrame *virt_frame2;
  void (*render_line) (SchroFrame *frame, void *dest, int component, int i);
  void *virt_priv;
  void *virt_priv2;

  int extension;
  int cache_offset[3];
};

struct _SchroUpsampledFrame {
  SchroFrame *frames[4];
  void *components[3];
};

#define SCHRO_FRAME_DATA_GET_LINE(fd,i) (SCHRO_OFFSET((fd)->data,(fd)->stride*(i)))
#define SCHRO_FRAME_DATA_GET_PIXEL_U8(fd,i,j) ((uint8_t *)SCHRO_OFFSET((fd)->data,(fd)->stride*(j)+(i)))
#define SCHRO_FRAME_DATA_GET_PIXEL_S16(fd,i,j) ((int16_t *)SCHRO_OFFSET((fd)->data,(fd)->stride*(j)+(i)*sizeof(int16_t)))

SchroFrame * schro_frame_new (void);
SchroFrame * schro_frame_new_and_alloc (SchroMemoryDomain *domain,
    SchroFrameFormat format, int width, int height);
SchroFrame * schro_frame_new_from_data_I420 (void *data, int width, int height);
SchroFrame * schro_frame_new_from_data_Y42B (void *data, int width, int height);
SchroFrame * schro_frame_new_from_data_Y444 (void *data, int width, int height);
SchroFrame * schro_frame_new_from_data_YV12 (void *data, int width, int height);
SchroFrame * schro_frame_new_from_data_YUY2 (void *data, int width, int height);
SchroFrame * schro_frame_new_from_data_UYVY (void *data, int width, int height);
SchroFrame * schro_frame_new_from_data_UYVY_full (void *data, int width, int height, int stride);
SchroFrame * schro_frame_new_from_data_AYUV (void *data, int width, int height);
SchroFrame * schro_frame_new_from_data_v216 (void *data, int width, int height);
SchroFrame * schro_frame_new_from_data_v210 (void *data, int width, int height);
void schro_frame_set_free_callback (SchroFrame *frame,
    SchroFrameFreeFunc free_func, void *priv);
void schro_frame_unref (SchroFrame *frame);
SchroFrame *schro_frame_ref (SchroFrame *frame);
SchroFrame *schro_frame_dup (SchroFrame *frame);
SchroFrame *schro_frame_clone (SchroMemoryDomain *domain, SchroFrame *frame);

void schro_frame_convert (SchroFrame *dest, SchroFrame *src);
void schro_frame_add (SchroFrame *dest, SchroFrame *src);
void schro_frame_subtract (SchroFrame *dest, SchroFrame *src);
void schro_frame_shift_left (SchroFrame *frame, int shift);
void schro_frame_shift_right (SchroFrame *frame, int shift);
void schro_frame_clear (SchroFrame *frame);

void schro_frame_downsample (SchroFrame *dest, SchroFrame *src);
void schro_frame_upsample_horiz (SchroFrame *dest, SchroFrame *src);
void schro_frame_upsample_vert (SchroFrame *dest, SchroFrame *src);
double schro_frame_calculate_average_luma (SchroFrame *frame);

SchroFrame * schro_frame_convert_to_444 (SchroFrame *frame);
void schro_frame_md5 (SchroFrame *frame, uint32_t *state);

#ifdef SCHRO_ENABLE_UNSTABLE_API

SchroFrame * schro_frame_new_and_alloc_extended (SchroMemoryDomain *domain,
    SchroFrameFormat format, int width, int height, int extension);
SchroFrame *schro_frame_dup_extended (SchroFrame *frame, int extension);
void schro_frame_edge_extend (SchroFrame *frame, int width, int height);
void schro_frame_zero_extend (SchroFrame *frame, int width, int height);
void schro_frame_mark (SchroFrame *frame, int value);
void schro_frame_mc_edgeextend (SchroFrame *frame);

void schro_frame_data_get_codeblock (SchroFrameData *dest, SchroFrameData *src,
        int x, int y, int horiz_codeblocks, int vert_codeblocks);

SchroUpsampledFrame * schro_upsampled_frame_new (SchroFrame *frame);
void schro_upsampled_frame_free (SchroUpsampledFrame *df);
void schro_upsampled_frame_upsample (SchroUpsampledFrame *df);
#ifdef ENABLE_MOTION_REF
int schro_upsampled_frame_get_pixel_prec0 (SchroUpsampledFrame *upframe, int k,
    int x, int y);
int schro_upsampled_frame_get_pixel_prec1 (SchroUpsampledFrame *upframe, int k,
    int x, int y);
int schro_upsampled_frame_get_pixel_prec3 (SchroUpsampledFrame *upframe, int k,
    int x, int y);
int schro_upsampled_frame_get_pixel_precN (SchroUpsampledFrame *upframe, int k,
    int x, int y, int mv_precision);
#endif
void schro_upsampled_frame_get_block_precN (SchroUpsampledFrame *upframe, int k,
    int x, int y, int prec, SchroFrameData *dest);
void schro_upsampled_frame_get_block_fast_precN (SchroUpsampledFrame *upframe, int k,
    int x, int y, int prec, SchroFrameData *dest, SchroFrameData *fd);
void schro_upsampled_frame_get_subdata_prec0 (SchroUpsampledFrame *upframe,
    int k, int x, int y, SchroFrameData *fd);
void schro_upsampled_frame_get_subdata_prec1 (SchroUpsampledFrame *upframe,
    int k, int x, int y, SchroFrameData *fd);

/* it extracts a block of data from a frame, if possible */
int schro_frame_get_data (SchroFrame* frame, SchroFrameData* fd, int comp
    , int x, int y);

void schro_frame_get_subdata (SchroFrame *frame, SchroFrameData *fd,
        int comp, int x, int y);
void schro_frame_get_reference_subdata (SchroFrame* frame, SchroFrameData* fd
    , int comp, int x, int y);

void schro_frame_split_fields (SchroFrame *dest1, SchroFrame *dest2, SchroFrame *src);

#endif

SCHRO_END_DECLS

#endif

