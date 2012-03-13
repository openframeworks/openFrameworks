/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2001-2003, Ximian, Inc.
 */

#ifndef SOUP_HEADERS_H
#define SOUP_HEADERS_H 1

#include <glib.h>
#include <libsoup/soup-message.h>

G_BEGIN_DECLS

/* HTTP Header Parsing */

gboolean    soup_headers_parse              (const char          *str,
					     int                  len,
					     SoupMessageHeaders  *dest);

guint       soup_headers_parse_request      (const char          *str,
					     int                  len,
					     SoupMessageHeaders  *req_headers,
					     char               **req_method,
					     char               **req_path,
					     SoupHTTPVersion     *ver);

gboolean    soup_headers_parse_status_line  (const char          *status_line,
					     SoupHTTPVersion     *ver,
					     guint               *status_code,
					     char               **reason_phrase);

gboolean    soup_headers_parse_response     (const char          *str,
					     int                  len,
					     SoupMessageHeaders  *headers,
					     SoupHTTPVersion     *ver,
					     guint               *status_code,
					     char               **reason_phrase);

/* Individual header parsing */

GSList     *soup_header_parse_list          (const char       *header);
GSList     *soup_header_parse_quality_list  (const char       *header,
					     GSList          **unacceptable);
void        soup_header_free_list           (GSList           *list);

gboolean    soup_header_contains            (const char       *header,
					     const char       *token);

GHashTable *soup_header_parse_param_list      (const char       *header);
GHashTable *soup_header_parse_semi_param_list (const char       *header);
void        soup_header_free_param_list       (GHashTable       *param_list);

void        soup_header_g_string_append_param (GString          *string,
					       const char       *name,
					       const char       *value);
void        soup_header_g_string_append_param_quoted (GString    *string,
						      const char *name,
						      const char *value);

G_END_DECLS

#endif /*SOUP_HEADERS_H*/
