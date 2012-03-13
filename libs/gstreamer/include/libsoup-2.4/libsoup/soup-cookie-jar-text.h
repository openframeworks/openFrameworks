/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2008 Red Hat, Inc.
 */

#ifndef SOUP_COOKIE_JAR_TEXT_H
#define SOUP_COOKIE_JAR_TEXT_H 1

#include <libsoup/soup-cookie-jar.h>

G_BEGIN_DECLS

#define SOUP_TYPE_COOKIE_JAR_TEXT            (soup_cookie_jar_text_get_type ())
#define SOUP_COOKIE_JAR_TEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_COOKIE_JAR_TEXT, SoupCookieJarText))
#define SOUP_COOKIE_JAR_TEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_COOKIE_JAR_TEXT, SoupCookieJarTextClass))
#define SOUP_IS_COOKIE_JAR_TEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_COOKIE_JAR_TEXT))
#define SOUP_IS_COOKIE_JAR_TEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_COOKIE_JAR_TEXT))
#define SOUP_COOKIE_JAR_TEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_COOKIE_JAR_TEXT, SoupCookieJarTextClass))

typedef struct {
	SoupCookieJar parent;

} SoupCookieJarText;

typedef struct {
	SoupCookieJarClass parent_class;

	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupCookieJarTextClass;

#define SOUP_COOKIE_JAR_TEXT_FILENAME  "filename"

GType soup_cookie_jar_text_get_type (void);

SoupCookieJar *soup_cookie_jar_text_new (const char *filename,
					 gboolean    read_only);

G_END_DECLS

#endif /* SOUP_COOKIE_JAR_TEXT_H */
