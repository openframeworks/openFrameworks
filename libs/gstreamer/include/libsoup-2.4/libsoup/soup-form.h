/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/* 
 * Copyright 2008 Red Hat, Inc.
 */

#ifndef  SOUP_FORM_H
#define  SOUP_FORM_H 1

#include <libsoup/soup-types.h>
#include <libsoup/soup-multipart.h>

G_BEGIN_DECLS

#define SOUP_FORM_MIME_TYPE_URLENCODED "application/x-www-form-urlencoded"
#define SOUP_FORM_MIME_TYPE_MULTIPART  "multipart/form-data"

GHashTable  *soup_form_decode           (const char   *encoded_form);
GHashTable  *soup_form_decode_multipart (SoupMessage  *msg,
					 const char   *file_control_name,
					 char        **filename,
					 char        **content_type,
					 SoupBuffer  **file);

char        *soup_form_encode           (const char   *first_field,
					 ...) G_GNUC_NULL_TERMINATED;
char        *soup_form_encode_hash      (GHashTable   *form_data_set);
char        *soup_form_encode_datalist  (GData       **form_data_set);
char        *soup_form_encode_valist    (const char   *first_field,
					 va_list       args);

#ifndef LIBSOUP_DISABLE_DEPRECATED
/* Compatibility with libsoup 2.3.0 */
#define soup_form_decode_urlencoded      soup_form_decode
#define soup_form_encode_urlencoded      soup_form_encode_hash
#define soup_form_encode_urlencoded_list soup_form_encode_datalist
#endif

SoupMessage *soup_form_request_new                (const char     *method,
						   const char     *uri,
						   const char     *first_field,
						   ...) G_GNUC_NULL_TERMINATED;
SoupMessage *soup_form_request_new_from_hash      (const char     *method,
						   const char     *uri,
						   GHashTable     *form_data_set);
SoupMessage *soup_form_request_new_from_datalist  (const char     *method,
						   const char     *uri,
						   GData         **form_data_set);
SoupMessage *soup_form_request_new_from_multipart (const char     *uri,
						   SoupMultipart  *multipart);

G_END_DECLS

#endif /* SOUP_FORM_H */
