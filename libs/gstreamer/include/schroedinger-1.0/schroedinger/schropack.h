
#ifndef _SCHRO_PACK_H_
#define _SCHRO_PACK_H_

#include <schroedinger/schroutils.h>
#include <schroedinger/schrobuffer.h>

SCHRO_BEGIN_DECLS

typedef struct _SchroPack SchroPack;

struct _SchroPack {
  SchroBuffer *buffer;

  int n;
  int shift;
  int n_pack;

  uint32_t value;

  int error;
};

#ifdef SCHRO_ENABLE_UNSTABLE_API

SchroPack * schro_pack_new (void);
void schro_pack_free (SchroPack *pack);

void schro_pack_encode_init (SchroPack *pack, SchroBuffer *buffer);
void schro_pack_copy (SchroPack *dest, SchroPack *src);

void schro_pack_sync (SchroPack *pack);
void schro_pack_flush (SchroPack *pack);
int schro_pack_get_offset (SchroPack *pack);
int schro_pack_get_bit_offset (SchroPack *pack);

void schro_pack_append (SchroPack *pack, const uint8_t *data, int len);
void schro_pack_append_zero (SchroPack *pack, int len);

void schro_pack_encode_bit (SchroPack *pack, int value);
void schro_pack_encode_bits (SchroPack *pack, int n, unsigned int value);
void schro_pack_encode_uint (SchroPack *pack, int value);
void schro_pack_encode_sint (SchroPack *pack, int value);

int schro_pack_estimate_uint (int value);
int schro_pack_estimate_sint (int value);

#endif

SCHRO_END_DECLS

#endif


