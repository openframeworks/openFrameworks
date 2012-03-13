/* GIO - GLib Input, Output and Streaming Library
 *
 * Copyright (C) 2006-2007 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Alexander Larsson <alexl@redhat.com>
 */

#if !defined (__GIO_GIO_H_INSIDE__) && !defined (GIO_COMPILATION)
#error "Only <gio/gio.h> can be included directly."
#endif

#ifndef __G_MOUNT_OPERATION_H__
#define __G_MOUNT_OPERATION_H__

#include <gio/giotypes.h>

G_BEGIN_DECLS

#define G_TYPE_MOUNT_OPERATION         (g_mount_operation_get_type ())
#define G_MOUNT_OPERATION(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), G_TYPE_MOUNT_OPERATION, GMountOperation))
#define G_MOUNT_OPERATION_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), G_TYPE_MOUNT_OPERATION, GMountOperationClass))
#define G_IS_MOUNT_OPERATION(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), G_TYPE_MOUNT_OPERATION))
#define G_IS_MOUNT_OPERATION_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), G_TYPE_MOUNT_OPERATION))
#define G_MOUNT_OPERATION_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), G_TYPE_MOUNT_OPERATION, GMountOperationClass))

/**
 * GMountOperation:
 *
 * Class for providing authentication methods for mounting operations,
 * such as mounting a file locally, or authenticating with a server.
 **/
typedef struct _GMountOperationClass   GMountOperationClass;
typedef struct _GMountOperationPrivate GMountOperationPrivate;

struct _GMountOperation
{
  GObject parent_instance;

  GMountOperationPrivate *priv;
};

struct _GMountOperationClass
{
  GObjectClass parent_class;

  /* signals: */

  void (* ask_password) (GMountOperation       *op,
			 const char            *message,
			 const char            *default_user,
			 const char            *default_domain,
			 GAskPasswordFlags      flags);

  void (* ask_question) (GMountOperation       *op,
			 const char            *message,
			 const char            *choices[]);

  void (* reply)        (GMountOperation       *op,
			 GMountOperationResult  result);

  void (* aborted)      (GMountOperation       *op);

  void (* show_processes) (GMountOperation      *op,
                           const gchar          *message,
                           GArray               *processes,
                           const gchar          *choices[]);

  /*< private >*/
  /* Padding for future expansion */
  void (*_g_reserved1) (void);
  void (*_g_reserved2) (void);
  void (*_g_reserved3) (void);
  void (*_g_reserved4) (void);
  void (*_g_reserved5) (void);
  void (*_g_reserved6) (void);
  void (*_g_reserved7) (void);
  void (*_g_reserved8) (void);
  void (*_g_reserved9) (void);
  void (*_g_reserved10) (void);
};

GType             g_mount_operation_get_type      (void) G_GNUC_CONST;
GMountOperation * g_mount_operation_new           (void);

const char *  g_mount_operation_get_username      (GMountOperation *op);
void          g_mount_operation_set_username      (GMountOperation *op,
						   const char      *username);
const char *  g_mount_operation_get_password      (GMountOperation *op);
void          g_mount_operation_set_password      (GMountOperation *op,
						   const char      *password);
gboolean      g_mount_operation_get_anonymous     (GMountOperation *op);
void          g_mount_operation_set_anonymous     (GMountOperation *op,
						   gboolean         anonymous);
const char *  g_mount_operation_get_domain        (GMountOperation *op);
void          g_mount_operation_set_domain        (GMountOperation *op,
						   const char      *domain);
GPasswordSave g_mount_operation_get_password_save (GMountOperation *op);
void          g_mount_operation_set_password_save (GMountOperation *op,
						   GPasswordSave    save);
int           g_mount_operation_get_choice        (GMountOperation *op);
void          g_mount_operation_set_choice        (GMountOperation *op,
						   int              choice);
void          g_mount_operation_reply             (GMountOperation *op,
						   GMountOperationResult result);

G_END_DECLS

#endif /* __G_MOUNT_OPERATION_H__ */
