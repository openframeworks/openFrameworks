/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2009 Gustavo Noronha Silva.
 */

#ifndef SOUP_CONTENT_SNIFFER_H
#define SOUP_CONTENT_SNIFFER_H 1

#include <libsoup/soup-types.h>
#include <libsoup/soup-message-body.h>

G_BEGIN_DECLS

#define SOUP_TYPE_CONTENT_SNIFFER            (soup_content_sniffer_get_type ())
#define SOUP_CONTENT_SNIFFER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_CONTENT_SNIFFER, SoupContentSniffer))
#define SOUP_CONTENT_SNIFFER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_CONTENT_SNIFFER, SoupContentSnifferClass))
#define SOUP_IS_CONTENT_SNIFFER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_CONTENT_SNIFFER))
#define SOUP_IS_CONTENT_SNIFFER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_CONTENT_SNIFFER))
#define SOUP_CONTENT_SNIFFER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_CONTENT_SNIFFER, SoupContentSnifferClass))

typedef struct _SoupContentSnifferPrivate SoupContentSnifferPrivate;

typedef struct {
	GObject parent;

	SoupContentSnifferPrivate *priv;
} SoupContentSniffer;

typedef struct {
	GObjectClass parent_class;

	char* (*sniff)              (SoupContentSniffer *sniffer,
				     SoupMessage *msg,
				     SoupBuffer *buffer,
				     GHashTable **params);
	gsize (*get_buffer_size)    (SoupContentSniffer *sniffer);

	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
	void (*_libsoup_reserved5) (void);
} SoupContentSnifferClass;

GType               soup_content_sniffer_get_type (void);

SoupContentSniffer *soup_content_sniffer_new      (void);

char               *soup_content_sniffer_sniff    (SoupContentSniffer *sniffer,
						   SoupMessage *msg,
						   SoupBuffer *buffer,
						   GHashTable **params);

G_END_DECLS

#endif /* SOUP_CONTENT_SNIFFER_H */
