
#ifndef __SCHRO_UTILS_H__
#define __SCHRO_UTILS_H__

#if defined(_MSC_VER)
#ifndef SCHRO_NO_STDINT_TYPEDEFS
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
#endif
#else
#include <stdint.h>
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef uint32_t SchroPictureNumber;
typedef unsigned int schro_bool;

/* This must match the CUDA stream type, as we don't want to include the CUDA
   includes in any of the Schroedinger include files.
 */
typedef int SchroCUDAStream;

#ifdef SCHRO_ENABLE_UNSTABLE_API

#define SCHRO_PICTURE_NUMBER_INVALID (-1)

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define DIVIDE_ROUND_UP(a,b) (((a) + (b) - 1)/(b))
#ifndef MIN
#define MIN(a,b) ((a)<(b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a)>(b) ? (a) : (b))
#endif
#ifndef CLAMP
#define CLAMP(x,a,b) ((x)<(a) ? (a) : ((x)>(b) ? (b) : (x)))
#endif
#define NEED_CLAMP(x,y,a,b) ((x) < (a) || (y) > (b))
#define ROUND_UP_SHIFT(x,y) (((x) + (1<<(y)) - 1)>>(y))
#define ROUND_UP_POW2(x,y) (((x) + (1<<(y)) - 1)&((~0)<<(y)))
#define ROUND_UP_2(x) ROUND_UP_POW2(x,1)
#define ROUND_UP_4(x) ROUND_UP_POW2(x,2)
#define ROUND_UP_8(x) ROUND_UP_POW2(x,3)
#define ROUND_UP_16(x) ROUND_UP_POW2(x,4)
#define ROUND_UP_64(x) ROUND_UP_POW2(x,6)
#define OFFSET(ptr,offset) ((void *)(((uint8_t *)(ptr)) + (offset)))
#define ROUND_SHIFT(x,y) (((x) + (1<<((y)-1)))>>(y))

#define schro_divide(a,b) (((a)<0)?(((a) - (b) + 1)/(b)):((a)/(b)))
#define schro_divide3(a) (((a)*21845 + 10922)>>16)

#endif

#define SCHRO_OFFSET(ptr,offset) ((void *)(((uint8_t *)(ptr)) + (offset)))
#define SCHRO_GET(ptr, offset, type) (*(type *)((uint8_t *)(ptr) + (offset)) )

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#define SCHRO_GNUC_PREREQ(maj, min) \
  ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
#define SCHRO_GNUC_PREREQ(maj, min) 0
#endif

#if SCHRO_GNUC_PREREQ(3,3) && defined(__ELF__)
#define SCHRO_INTERNAL __attribute__ ((visibility ("internal")))
#else
#define SCHRO_INTERNAL
#endif

#if SCHRO_GNUC_PREREQ(3,3)
#define SCHRO_DEPRECATED __attribute__ ((deprecated))
#else
#define SCHRO_DEPRECATED
#endif

#ifdef __cplusplus
#define SCHRO_BEGIN_DECLS extern "C" {
#define SCHRO_END_DECLS }
#else
#define SCHRO_BEGIN_DECLS
#define SCHRO_END_DECLS
#endif

#if SCHRO_GNUC_PREREQ(3,0) && defined(__OPTIMIZE__)
#define SCHRO_LIKELY(expr) (__builtin_expect ((expr), 1))
#define SCHRO_UNLIKELY(expr) (__builtin_expect ((expr), 0))
#else
#define SCHRO_LIKELY(expr) (expr)
#define SCHRO_UNLIKELY(expr) (expr)
#endif

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

void * schro_malloc (int size);
void * schro_malloc0 (int size);
void * schro_realloc (void *ptr, int size);
void schro_free (void *ptr);

int muldiv64 (int a, int b, int c);
int schro_utils_multiplier_to_quant_index (double x);
int schro_dequantise (int q, int quant_factor, int quant_offset);
int schro_quantise (int value, int quant_factor, int quant_offset);
void schro_quantise_s16 (int16_t *dest, int16_t *src, int quant_factor,
    int quant_offset, int n);
void schro_quantise_s16_table (int16_t *dest, int16_t *src, int quant_index,
    schro_bool is_intra, int n);
void schro_dequantise_s16 (int16_t *dest, int16_t *src, int quant_factor,
    int quant_offset, int n);
void schro_dequantise_s16_table (int16_t *dest, int16_t *src, int quant_index,
    schro_bool is_intra, int n);
double schro_utils_probability_to_entropy (double x);
double schro_utils_entropy (double a, double total);
void schro_utils_reduce_fraction (int *n, int *d);
double schro_utils_get_time (void);

#endif

SCHRO_END_DECLS

#endif

