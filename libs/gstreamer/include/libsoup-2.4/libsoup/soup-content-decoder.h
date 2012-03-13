/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2009 Red Hat, Inc.
 */

#ifndef SOUP_CONTENT_DECODER_H
#define SOUP_CONTENT_DECODER_H 1

#include <libsoup/soup-types.h>
#include <libsoup/soup-message-body.h>

G_BEGIN_DECLS

#define SOUP_TYPE_CONTENT_DECODER            (soup_content_decoder_get_type ())
#define SOUP_CONTENT_DECODER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_CONTENT_DECODER, SoupContentDecoder))
#define SOUP_CONTENT_DECODER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_CONTENT_DECODER, SoupContentDecoderClass))
#define SOUP_IS_CONTENT_DECODER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_CONTENT_DECODER))
#define SOUP_IS_CONTENT_DECODER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_CONTENT_DECODER))
#define SOUP_CONTENT_DECODER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_CONTENT_DECODER, SoupContentDecoderClass))

typedef struct _SoupContentDecoderPrivate SoupContentDecoderPrivate;

typedef struct {
	GObject parent;

	SoupContentDecoderPrivate *priv;
} SoupContentDecoder;

typedef struct {
	GObjectClass parent_class;

	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
	void (*_libsoup_reserved5) (void);
} SoupContentDecoderClass;

GType               soup_content_decoder_get_type (void);

G_END_DECLS

#endif /* SOUP_CONTENT_DECODER_H */
