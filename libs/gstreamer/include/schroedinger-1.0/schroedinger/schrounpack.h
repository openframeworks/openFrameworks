
#ifndef _SCHRO_UNPACK_H_
#define _SCHRO_UNPACK_H_

#include <schroedinger/schroutils.h>

SCHRO_BEGIN_DECLS

typedef struct _SchroUnpack SchroUnpack;

struct _SchroUnpack {
  /* pointer to data that haven't been shifted into the shift register */
  uint8_t *data;

  /* number of bits remaining that haven't been shifted into the shift
   * register */
  int n_bits_left;

  /* number of bits read by reader */
  int n_bits_read;

  /* chunk of bits we're currently frobbing, aligned to (1<<31) */
  uint32_t shift_register;
  int n_bits_in_shift_register;

  /* bit pattern used after an error */
  unsigned int guard_bit;

  /* unpacking has overrun the end of the buffer */
  int overrun;
};

#ifdef SCHRO_ENABLE_UNSTABLE_API

void schro_unpack_init_with_data (SchroUnpack *unpack, uint8_t *data,
    int n_bytes, unsigned int guard_bit);
void schro_unpack_copy (SchroUnpack *dest, SchroUnpack *src);

int schro_unpack_get_bits_read (SchroUnpack *unpack);
int schro_unpack_get_bits_remaining (SchroUnpack *unpack);

void schro_unpack_limit_bits_remaining (SchroUnpack *unpack, int n_bits);

void schro_unpack_byte_sync (SchroUnpack *unpack);
void schro_unpack_skip_bits (SchroUnpack *unpack, int n_bits);

unsigned int schro_unpack_decode_bit (SchroUnpack *unpack);
unsigned int schro_unpack_decode_bits (SchroUnpack *unpack, int n);
unsigned int schro_unpack_decode_uint (SchroUnpack *unpack);
int schro_unpack_decode_sint (SchroUnpack *unpack);
int schro_unpack_decode_sint_slow (SchroUnpack *unpack);
void schro_unpack_decode_sint_s16 (int16_t *dest, SchroUnpack *unpack, int n);

#endif

SCHRO_END_DECLS

#endif


