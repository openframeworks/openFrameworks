
#ifndef __SCHRO_SCHRO_TABLES_H__
#define __SCHRO_SCHRO_TABLES_H__

#include <schroedinger/schroutils.h>
#include <schroedinger/schrobitstream.h>

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

extern const uint32_t schro_table_offset_3_8[61];
extern const uint32_t schro_table_offset_1_2[61];
extern const uint32_t schro_table_quant[61];
extern const uint16_t schro_table_inverse_quant[61];
extern const uint16_t schro_table_division_factor[257];
extern const double schro_table_error_hist_shift3_1_2[60][104];
extern const double schro_table_onebits_hist_shift3_1_2[60][104];
extern const double schro_table_zerobits_hist_shift3_1_2[60][104];

extern const float schro_tables_wavelet_noise_curve[SCHRO_N_WAVELETS][8][128];
extern const double schro_tables_wavelet_gain[SCHRO_N_WAVELETS][2];

#define SCHRO_UNPACK_TABLE_SHIFT 10
extern const int16_t schro_table_unpack_sint[(1<<SCHRO_UNPACK_TABLE_SHIFT)][SCHRO_UNPACK_TABLE_SHIFT];

void schro_tables_init (void);
int16_t *schro_tables_get_quantise_table (int quant_index, schro_bool is_intra);
int16_t *schro_tables_get_dequantise_table (int quant_index, schro_bool is_intra);

#endif

SCHRO_END_DECLS

#endif

