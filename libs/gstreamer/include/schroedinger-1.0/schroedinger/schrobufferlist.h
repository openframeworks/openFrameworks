#ifndef __SCHRO_BUFFERLIST_H__
#define __SCHRO_BUFFERLIST_H__

#include <schroedinger/schroutils.h>
#include <schroedinger/schrobuffer.h>
#include <schroedinger/schrolist.h>

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

typedef struct _SchroBufferList SchroBufferList;

struct _SchroBufferList
{
  /* private */
  SchroList *list;
  unsigned offset;

  /* stashed tag for use in next extraction */
  SchroTag *tag;
};

SchroBufferList *schro_buflist_new (void);
void schro_buflist_free (SchroBufferList *buflist);
void schro_buflist_append (SchroBufferList *buflist, SchroBuffer *buf);
int schro_buflist_peekbytes (uint8_t *dst, unsigned len, SchroBufferList *buflist, unsigned offset);
int schro_buflist_findbytes (SchroBufferList *buflist, unsigned *start, const uint8_t *needle, unsigned needle_len);
void schro_buflist_flush (SchroBufferList *buflist, unsigned amount);
SchroBuffer * schro_buflist_extract (SchroBufferList *buflist, unsigned start, unsigned len);

#endif

SCHRO_END_DECLS

#endif
