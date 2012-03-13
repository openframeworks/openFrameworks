
#ifndef __SCHRO_PARSE_H__
#define __SCHRO_PARSE_H__

#include <schroedinger/schrovideoformat.h>
#include <schroedinger/schrobufferlist.h>

SCHRO_BEGIN_DECLS

int schro_parse_decode_sequence_header (uint8_t *data, int length,
    SchroVideoFormat *video_format);

#ifdef SCHRO_ENABLE_UNSTABLE_API

typedef struct _SchroParseSyncState SchroParseSyncState;

SchroParseSyncState *schro_parse_sync_new (void);
void schro_parse_sync_free (SchroParseSyncState *);
SchroBuffer *schro_parse_sync (SchroParseSyncState *sps, SchroBufferList *input_bufer);

#endif

SCHRO_END_DECLS

#endif

