/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/* 
 * Copyright 2007, 2008 Red Hat, Inc.
 */

#ifndef SOUP_COOKIE_H
#define SOUP_COOKIE_H 1

#include <libsoup/soup-types.h>

G_BEGIN_DECLS

struct _SoupCookie {
	char     *name;
	char     *value;
	char     *domain;
	char     *path;
	SoupDate *expires;
	gboolean  secure;
	gboolean  http_only;
};

GType soup_cookie_get_type (void);
#define SOUP_TYPE_COOKIE (soup_cookie_get_type())

#define SOUP_COOKIE_MAX_AGE_ONE_HOUR (60 * 60)
#define SOUP_COOKIE_MAX_AGE_ONE_DAY  (SOUP_COOKIE_MAX_AGE_ONE_HOUR * 24)
#define SOUP_COOKIE_MAX_AGE_ONE_WEEK (SOUP_COOKIE_MAX_AGE_ONE_DAY * 7)
#define SOUP_COOKIE_MAX_AGE_ONE_YEAR (SOUP_COOKIE_MAX_AGE_ONE_DAY * 365.2422)

SoupCookie *soup_cookie_new                     (const char  *name,
						 const char  *value,
						 const char  *domain,
						 const char  *path,
						 int          max_age);
SoupCookie *soup_cookie_parse                   (const char  *header,
						 SoupURI     *origin);
SoupCookie *soup_cookie_copy                    (SoupCookie  *cookie);

const char *soup_cookie_get_name                (SoupCookie  *cookie);
void        soup_cookie_set_name                (SoupCookie  *cookie,
						 const char  *name);
const char *soup_cookie_get_value               (SoupCookie  *cookie);
void        soup_cookie_set_value               (SoupCookie  *cookie,
						 const char  *value);
const char *soup_cookie_get_domain              (SoupCookie  *cookie);
void        soup_cookie_set_domain              (SoupCookie  *cookie,
						 const char  *domain);
const char *soup_cookie_get_path                (SoupCookie  *cookie);
void        soup_cookie_set_path                (SoupCookie  *cookie,
						 const char  *path);
void        soup_cookie_set_max_age             (SoupCookie  *cookie,
						 int          max_age);
SoupDate   *soup_cookie_get_expires             (SoupCookie  *cookie);
void        soup_cookie_set_expires             (SoupCookie  *cookie,
						 SoupDate    *expires);
gboolean    soup_cookie_get_secure              (SoupCookie  *cookie);
void        soup_cookie_set_secure              (SoupCookie  *cookie,
						 gboolean     secure);
gboolean    soup_cookie_get_http_only           (SoupCookie  *cookie);
void        soup_cookie_set_http_only           (SoupCookie  *cookie,
						 gboolean     http_only);

char       *soup_cookie_to_set_cookie_header    (SoupCookie  *cookie);
char       *soup_cookie_to_cookie_header        (SoupCookie  *cookie);

gboolean    soup_cookie_applies_to_uri          (SoupCookie  *cookie,
						 SoupURI     *uri);
gboolean    soup_cookie_equal                   (SoupCookie  *cookie1,
						 SoupCookie  *cookie2);

void        soup_cookie_free                    (SoupCookie  *cookie);

GSList     *soup_cookies_from_response          (SoupMessage *msg);
GSList     *soup_cookies_from_request           (SoupMessage *msg);

void        soup_cookies_to_response            (GSList      *cookies,
						 SoupMessage *msg);
void        soup_cookies_to_request             (GSList      *cookies,
						 SoupMessage *msg);

void        soup_cookies_free                   (GSList      *cookies);

char       *soup_cookies_to_cookie_header       (GSList      *cookies);

gboolean    soup_cookie_domain_matches          (SoupCookie  *cookie,
						 const char  *host);

G_END_DECLS

#endif /* SOUP_COOKIE_H */
