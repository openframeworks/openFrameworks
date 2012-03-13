/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/* 
 * Copyright 1999-2002 Ximian, Inc.
 */


#ifndef  SOUP_URI_H
#define  SOUP_URI_H 1

#include <libsoup/soup-types.h>
#include <libsoup/soup-misc.h>

G_BEGIN_DECLS

struct _SoupURI {
	const char *scheme;

	char       *user;
	char       *password;

	char       *host;
	guint       port;

	char       *path;
	char       *query;

	char       *fragment;
};

GType       soup_uri_get_type              (void);
#define SOUP_TYPE_URI (soup_uri_get_type ())

#define SOUP_URI_SCHEME_HTTP  _SOUP_ATOMIC_INTERN_STRING (_SOUP_URI_SCHEME_HTTP, "http")
#define SOUP_URI_SCHEME_HTTPS _SOUP_ATOMIC_INTERN_STRING (_SOUP_URI_SCHEME_HTTPS, "https")
#define SOUP_URI_SCHEME_FTP   _SOUP_ATOMIC_INTERN_STRING (_SOUP_URI_SCHEME_FTP, "ftp")
#define SOUP_URI_SCHEME_FILE  _SOUP_ATOMIC_INTERN_STRING (_SOUP_URI_SCHEME_FILE, "file")
#define SOUP_URI_SCHEME_DATA  _SOUP_ATOMIC_INTERN_STRING (_SOUP_URI_SCHEME_DATA, "data")
extern gpointer _SOUP_URI_SCHEME_HTTP, _SOUP_URI_SCHEME_HTTPS;
extern gpointer _SOUP_URI_SCHEME_FTP;
extern gpointer _SOUP_URI_SCHEME_FILE, _SOUP_URI_SCHEME_DATA;

SoupURI	   *soup_uri_new_with_base         (SoupURI    *base,
					    const char *uri_string);
SoupURI	   *soup_uri_new                   (const char *uri_string);

char   	   *soup_uri_to_string             (SoupURI    *uri,
					    gboolean    just_path_and_query);

SoupURI	   *soup_uri_copy                  (SoupURI    *uri);

gboolean    soup_uri_equal                 (SoupURI    *uri1,
					    SoupURI    *uri2);

void  	    soup_uri_free                  (SoupURI    *uri);

char  	   *soup_uri_encode                (const char *part,
					    const char *escape_extra);
char  	   *soup_uri_decode                (const char *part);
char  	   *soup_uri_normalize             (const char *part,
					    const char *unescape_extra);

gboolean    soup_uri_uses_default_port     (SoupURI    *uri);

const char *soup_uri_get_scheme            (SoupURI    *uri);
void        soup_uri_set_scheme            (SoupURI    *uri,
					    const char *scheme);
const char *soup_uri_get_user              (SoupURI    *uri);
void        soup_uri_set_user              (SoupURI    *uri,
					    const char *user);
const char *soup_uri_get_password          (SoupURI    *uri);
void        soup_uri_set_password          (SoupURI    *uri,
					    const char *password);
const char *soup_uri_get_host              (SoupURI    *uri);
void        soup_uri_set_host              (SoupURI    *uri,
					    const char *host);
guint       soup_uri_get_port              (SoupURI    *uri);
void        soup_uri_set_port              (SoupURI    *uri,
					    guint       port);
const char *soup_uri_get_path              (SoupURI    *uri);
void        soup_uri_set_path              (SoupURI    *uri,
					    const char *path);
const char *soup_uri_get_query             (SoupURI    *uri);
void        soup_uri_set_query             (SoupURI    *uri,
					    const char *query);
void        soup_uri_set_query_from_form   (SoupURI    *uri,
					    GHashTable *form);
void        soup_uri_set_query_from_fields (SoupURI    *uri,
					    const char *first_field,
					    ...) G_GNUC_NULL_TERMINATED;
const char *soup_uri_get_fragment          (SoupURI    *uri);
void        soup_uri_set_fragment          (SoupURI    *uri,
					    const char *fragment);

SoupURI    *soup_uri_copy_host             (SoupURI    *uri);
guint       soup_uri_host_hash             (gconstpointer key);
gboolean    soup_uri_host_equal            (gconstpointer v1,
					    gconstpointer v2);

#define   SOUP_URI_VALID_FOR_HTTP(uri) ((uri) && ((uri)->scheme == SOUP_URI_SCHEME_HTTP || (uri)->scheme == SOUP_URI_SCHEME_HTTPS) && (uri)->host && (uri)->path)

G_END_DECLS

#endif /*SOUP_URI_H*/
