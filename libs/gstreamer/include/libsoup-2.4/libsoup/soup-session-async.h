/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2000-2003, Ximian, Inc.
 */

#ifndef SOUP_SESSION_ASYNC_H
#define SOUP_SESSION_ASYNC_H 1

#include <libsoup/soup-types.h>
#include <libsoup/soup-session.h>

G_BEGIN_DECLS

#define SOUP_TYPE_SESSION_ASYNC            (soup_session_async_get_type ())
#define SOUP_SESSION_ASYNC(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_SESSION_ASYNC, SoupSessionAsync))
#define SOUP_SESSION_ASYNC_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_SESSION_ASYNC, SoupSessionAsyncClass))
#define SOUP_IS_SESSION_ASYNC(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_SESSION_ASYNC))
#define SOUP_IS_SESSION_ASYNC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_SESSION_ASYNC))
#define SOUP_SESSION_ASYNC_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_SESSION_ASYNC, SoupSessionAsyncClass))

struct _SoupSessionAsync {
	SoupSession parent;

};

typedef struct {
	SoupSessionClass parent_class;

	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupSessionAsyncClass;

GType soup_session_async_get_type (void);

SoupSession *soup_session_async_new              (void);
SoupSession *soup_session_async_new_with_options (const char *optname1,
						  ...) G_GNUC_NULL_TERMINATED;

G_END_DECLS

#endif /* SOUP_SESSION_ASYNC_H */
