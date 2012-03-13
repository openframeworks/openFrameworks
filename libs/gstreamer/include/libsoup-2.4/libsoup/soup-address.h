/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2000-2003, Ximian, Inc.
 */

#ifndef SOUP_ADDRESS_H
#define SOUP_ADDRESS_H

#include <sys/types.h>

#include <gio/gio.h>

#include <libsoup/soup-portability.h>
#include <libsoup/soup-types.h>

G_BEGIN_DECLS

#define SOUP_TYPE_ADDRESS            (soup_address_get_type ())
#define SOUP_ADDRESS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOUP_TYPE_ADDRESS, SoupAddress))
#define SOUP_ADDRESS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOUP_TYPE_ADDRESS, SoupAddressClass))
#define SOUP_IS_ADDRESS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOUP_TYPE_ADDRESS))
#define SOUP_IS_ADDRESS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SOUP_TYPE_ADDRESS))
#define SOUP_ADDRESS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOUP_TYPE_ADDRESS, SoupAddressClass))

struct _SoupAddress {
	GObject parent;

};

typedef struct {
	GObjectClass parent_class;

	/* Padding for future expansion */
	void (*_libsoup_reserved1) (void);
	void (*_libsoup_reserved2) (void);
	void (*_libsoup_reserved3) (void);
	void (*_libsoup_reserved4) (void);
} SoupAddressClass;

#define SOUP_ADDRESS_NAME     "name"
#define SOUP_ADDRESS_FAMILY   "family"
#define SOUP_ADDRESS_PORT     "port"
#define SOUP_ADDRESS_PHYSICAL "physical"
#define SOUP_ADDRESS_SOCKADDR "sockaddr"

typedef enum {
	SOUP_ADDRESS_FAMILY_INVALID = -1,

	SOUP_ADDRESS_FAMILY_IPV4 = G_SOCKET_FAMILY_IPV4,
	SOUP_ADDRESS_FAMILY_IPV6 = G_SOCKET_FAMILY_IPV6
} SoupAddressFamily;

#define SOUP_ADDRESS_ANY_PORT 0

typedef void   (*SoupAddressCallback)            (SoupAddress         *addr,
						  guint                status,
						  gpointer             data);

GType soup_address_get_type (void);

SoupAddress     *soup_address_new                (const char          *name,
						  guint                port);
SoupAddress     *soup_address_new_from_sockaddr  (struct sockaddr     *sa,
						  int                  len);
SoupAddress     *soup_address_new_any            (SoupAddressFamily    family,
						  guint                port);

void             soup_address_resolve_async      (SoupAddress         *addr,
						  GMainContext        *async_context,
						  GCancellable        *cancellable,
						  SoupAddressCallback  callback,
						  gpointer             user_data);
guint            soup_address_resolve_sync       (SoupAddress         *addr,
						  GCancellable        *cancellable);

const char      *soup_address_get_name           (SoupAddress         *addr);
const char      *soup_address_get_physical       (SoupAddress         *addr);
guint            soup_address_get_port           (SoupAddress         *addr);
struct sockaddr *soup_address_get_sockaddr       (SoupAddress         *addr,
						  int                 *len);
GSocketAddress  *soup_address_get_gsockaddr      (SoupAddress         *addr);
gboolean         soup_address_is_resolved        (SoupAddress         *addr);

guint            soup_address_hash_by_name       (gconstpointer        addr);
gboolean         soup_address_equal_by_name      (gconstpointer        addr1,
						  gconstpointer        addr2);
guint            soup_address_hash_by_ip         (gconstpointer        addr);
gboolean         soup_address_equal_by_ip        (gconstpointer        addr1,
						  gconstpointer        addr2);


G_END_DECLS

#endif /* SOUP_ADDRESS_H */
