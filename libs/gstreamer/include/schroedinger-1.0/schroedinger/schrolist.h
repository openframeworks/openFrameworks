
#ifndef _SCHRO_LIST_H_
#define _SCHRO_LIST_H_

#include <schroedinger/schroutils.h>

SCHRO_BEGIN_DECLS

#ifdef SCHRO_ENABLE_UNSTABLE_API

typedef struct _SchroList SchroList;

typedef void (*SchroListFreeFunc)(void *member, void *priv);

struct _SchroList {
  void **members;
  int n;
  int n_alloc;

  SchroListFreeFunc free;
  void *priv;
};

SchroList *schro_list_new (void);
SchroList *schro_list_new_full (SchroListFreeFunc freefunc, void *priv);
void schro_list_free (SchroList *list);

void *schro_list_get (SchroList *list, int i);
int schro_list_get_size (SchroList *list);

void schro_list_append (SchroList *list, void *value);
void schro_list_insert (SchroList *list, int i, void *value);
void *schro_list_remove (SchroList *list, int i);
void schro_list_delete (SchroList *list, int i);
void *schro_list_replace (SchroList *list, int i, void *value);
void schro_list_prepend (SchroList *list, void *value);

#endif

SCHRO_END_DECLS

#endif


