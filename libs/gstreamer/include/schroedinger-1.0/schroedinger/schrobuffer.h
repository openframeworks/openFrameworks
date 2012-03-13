
#ifndef __SCHRO_BUFFER_H__
#define __SCHRO_BUFFER_H__

#include <schroedinger/schroutils.h>

SCHRO_BEGIN_DECLS

typedef struct _SchroBuffer SchroBuffer;
typedef struct _SchroTag SchroTag;

struct _SchroBuffer
{
  /*< private >*/
  unsigned char *data;
  unsigned int length;

  int ref_count;

  SchroBuffer *parent;

  void (*free) (SchroBuffer *, void *);
  void *priv;

  SchroTag* tag;
};

struct _SchroTag
{
  void (*free) (void *);
  void *value;
};

SchroBuffer *schro_buffer_new (void);
SchroBuffer *schro_buffer_new_and_alloc (int size);
SchroBuffer *schro_buffer_new_with_data (void *data, int size);
SchroBuffer *schro_buffer_new_subbuffer (SchroBuffer * buffer, int offset,
    int length);
SchroBuffer *schro_buffer_dup (SchroBuffer * buffer);
SchroBuffer * schro_buffer_ref (SchroBuffer * buffer);
void schro_buffer_unref (SchroBuffer * buffer);

SchroTag *schro_tag_new (void *value, void (*free_func) (void *));
void schro_tag_free (SchroTag *tag);

SCHRO_END_DECLS

#endif
