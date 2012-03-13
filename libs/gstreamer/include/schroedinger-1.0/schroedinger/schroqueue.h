
#ifndef __SCHRO_QUEUE_H__
#define __SCHRO_QUEUE_H__

#include <schroedinger/schroutils.h>

SCHRO_BEGIN_DECLS

typedef struct _SchroQueue SchroQueue;
typedef struct _SchroQueueElement SchroQueueElement;

typedef void (*SchroQueueFreeFunc)(void *data, SchroPictureNumber number);

struct _SchroQueueElement {
  void *data;
  SchroPictureNumber picture_number;
};

struct _SchroQueue {
  int size;
  int n;

  SchroQueueElement *elements;
  SchroQueueFreeFunc free;
};

#ifdef SCHRO_ENABLE_UNSTABLE_API

SchroQueue *schro_queue_new (int size, SchroQueueFreeFunc func);
void schro_queue_free (SchroQueue *queue);

void schro_queue_add (SchroQueue *queue, void *element,
    SchroPictureNumber picture_number);
void *schro_queue_find (SchroQueue *queue, SchroPictureNumber picture_number);
void schro_queue_delete (SchroQueue *queue, SchroPictureNumber picture_number);
void *schro_queue_remove (SchroQueue *queue, SchroPictureNumber picture_number);
void schro_queue_clear (SchroQueue *queue);
void schro_queue_pop (SchroQueue *queue);
void * schro_queue_pull (SchroQueue *queue);
void *schro_queue_peek (SchroQueue *queue);
int schro_queue_is_full (SchroQueue *queue);
int schro_queue_is_empty (SchroQueue *queue);
int schro_queue_slots_available (SchroQueue *queue);

#endif

SCHRO_END_DECLS

#endif

