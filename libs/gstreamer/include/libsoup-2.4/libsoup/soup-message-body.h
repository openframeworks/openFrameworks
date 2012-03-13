/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2000-2003, Ximian, Inc.
 */

#ifndef SOUP_MESSAGE_BODY_H
#define SOUP_MESSAGE_BODY_H 1

#include <libsoup/soup-types.h>

G_BEGIN_DECLS

typedef enum {
	SOUP_MEMORY_STATIC,
	SOUP_MEMORY_TAKE,
	SOUP_MEMORY_COPY,
	SOUP_MEMORY_TEMPORARY
} SoupMemoryUse;

typedef struct {
	const char *data;
	gsize       length;
} SoupBuffer;

GType soup_buffer_get_type (void);
#define SOUP_TYPE_BUFFER (soup_buffer_get_type ())

SoupBuffer *soup_buffer_new            (SoupMemoryUse   use,
					gconstpointer   data,
					gsize           length);
SoupBuffer *soup_buffer_new_take       (guchar         *data,
					gsize           length);
SoupBuffer *soup_buffer_new_subbuffer  (SoupBuffer     *parent,
					gsize           offset,
					gsize           length);

SoupBuffer *soup_buffer_new_with_owner (gconstpointer   data,
					gsize           length,
					gpointer        owner,
					GDestroyNotify  owner_dnotify);
gpointer    soup_buffer_get_owner      (SoupBuffer     *buffer);
void        soup_buffer_get_data       (SoupBuffer     *buffer,
					const guint8  **data,
					gsize          *length);
SoupBuffer *soup_buffer_copy           (SoupBuffer     *buffer);
void        soup_buffer_free           (SoupBuffer     *buffer);

typedef struct {
	const char *data;
	goffset     length;
} SoupMessageBody;

GType soup_message_body_get_type (void);
#define SOUP_TYPE_MESSAGE_BODY (soup_message_body_get_type ())

SoupMessageBody *soup_message_body_new           (void);

void             soup_message_body_set_accumulate(SoupMessageBody *body,
						  gboolean         accumulate);
gboolean         soup_message_body_get_accumulate(SoupMessageBody *body);

void             soup_message_body_append        (SoupMessageBody *body,
						  SoupMemoryUse    use,
						  gconstpointer    data,
						  gsize            length);
void             soup_message_body_append_take   (SoupMessageBody *body,
						  guchar          *data,
						  gsize            length);
void             soup_message_body_append_buffer (SoupMessageBody *body,
						  SoupBuffer      *buffer);
void             soup_message_body_truncate      (SoupMessageBody *body);
void             soup_message_body_complete      (SoupMessageBody *body);

SoupBuffer      *soup_message_body_flatten       (SoupMessageBody *body);

SoupBuffer      *soup_message_body_get_chunk     (SoupMessageBody *body,
						  goffset          offset);

void             soup_message_body_got_chunk     (SoupMessageBody *body,
						  SoupBuffer      *chunk);
void             soup_message_body_wrote_chunk   (SoupMessageBody *body,
						  SoupBuffer      *chunk);

void             soup_message_body_free          (SoupMessageBody *body);

G_END_DECLS

#endif /* SOUP_MESSAGE_BODY_H */
