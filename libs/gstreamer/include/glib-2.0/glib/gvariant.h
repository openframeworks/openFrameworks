/*
 * Copyright © 2007, 2008 Ryan Lortie
 * Copyright © 2009, 2010 Codethink Limited
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the licence, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Ryan Lortie <desrt@desrt.ca>
 */

#if defined(G_DISABLE_SINGLE_INCLUDES) && !defined (__GLIB_H_INSIDE__) && !defined (GLIB_COMPILATION)
#error "Only <glib.h> can be included directly."
#endif

#ifndef __G_VARIANT_H__
#define __G_VARIANT_H__

#include <glib/gvarianttype.h>
#include <glib/gstring.h>

G_BEGIN_DECLS

typedef struct _GVariant        GVariant;

typedef enum
{
  G_VARIANT_CLASS_BOOLEAN       = 'b',
  G_VARIANT_CLASS_BYTE          = 'y',
  G_VARIANT_CLASS_INT16         = 'n',
  G_VARIANT_CLASS_UINT16        = 'q',
  G_VARIANT_CLASS_INT32         = 'i',
  G_VARIANT_CLASS_UINT32        = 'u',
  G_VARIANT_CLASS_INT64         = 'x',
  G_VARIANT_CLASS_UINT64        = 't',
  G_VARIANT_CLASS_HANDLE        = 'h',
  G_VARIANT_CLASS_DOUBLE        = 'd',
  G_VARIANT_CLASS_STRING        = 's',
  G_VARIANT_CLASS_OBJECT_PATH   = 'o',
  G_VARIANT_CLASS_SIGNATURE     = 'g',
  G_VARIANT_CLASS_VARIANT       = 'v',
  G_VARIANT_CLASS_MAYBE         = 'm',
  G_VARIANT_CLASS_ARRAY         = 'a',
  G_VARIANT_CLASS_TUPLE         = '(',
  G_VARIANT_CLASS_DICT_ENTRY    = '{'
} GVariantClass;

void                            g_variant_unref                         (GVariant             *value);
GVariant *                      g_variant_ref                           (GVariant             *value);
GVariant *                      g_variant_ref_sink                      (GVariant             *value);
gboolean                        g_variant_is_floating                   (GVariant             *value);

const GVariantType *            g_variant_get_type                      (GVariant             *value);
const gchar *                   g_variant_get_type_string               (GVariant             *value);
gboolean                        g_variant_is_of_type                    (GVariant             *value,
                                                                         const GVariantType   *type);
gboolean                        g_variant_is_container                  (GVariant             *value);
GVariantClass                   g_variant_classify                      (GVariant             *value);
GVariant *                      g_variant_new_boolean                   (gboolean              boolean);
GVariant *                      g_variant_new_byte                      (guchar                byte);
GVariant *                      g_variant_new_int16                     (gint16                int16);
GVariant *                      g_variant_new_uint16                    (guint16               uint16);
GVariant *                      g_variant_new_int32                     (gint32                int32);
GVariant *                      g_variant_new_uint32                    (guint32               uint32);
GVariant *                      g_variant_new_int64                     (gint64                int64);
GVariant *                      g_variant_new_uint64                    (guint64               uint64);
GVariant *                      g_variant_new_handle                    (gint32                handle);
GVariant *                      g_variant_new_double                    (gdouble               floating);
GVariant *                      g_variant_new_string                    (const gchar          *string);
GVariant *                      g_variant_new_object_path               (const gchar          *object_path);
gboolean                        g_variant_is_object_path                (const gchar          *string);
GVariant *                      g_variant_new_signature                 (const gchar          *signature);
gboolean                        g_variant_is_signature                  (const gchar          *string);
GVariant *                      g_variant_new_variant                   (GVariant             *value);
GVariant *                      g_variant_new_strv                      (const gchar * const  *strv,
                                                                         gssize                length);
GVariant *                      g_variant_new_bytestring                (const gchar          *string);
GVariant *                      g_variant_new_bytestring_array          (const gchar * const  *strv,
                                                                         gssize                length);

gboolean                        g_variant_get_boolean                   (GVariant             *value);
guchar                          g_variant_get_byte                      (GVariant             *value);
gint16                          g_variant_get_int16                     (GVariant             *value);
guint16                         g_variant_get_uint16                    (GVariant             *value);
gint32                          g_variant_get_int32                     (GVariant             *value);
guint32                         g_variant_get_uint32                    (GVariant             *value);
gint64                          g_variant_get_int64                     (GVariant             *value);
guint64                         g_variant_get_uint64                    (GVariant             *value);
gint32                          g_variant_get_handle                    (GVariant             *value);
gdouble                         g_variant_get_double                    (GVariant             *value);
GVariant *                      g_variant_get_variant                   (GVariant             *value);
const gchar *                   g_variant_get_string                    (GVariant             *value,
                                                                         gsize                *length);
gchar *                         g_variant_dup_string                    (GVariant             *value,
                                                                         gsize                *length);
const gchar **                  g_variant_get_strv                      (GVariant             *value,
                                                                         gsize                *length);
gchar **                        g_variant_dup_strv                      (GVariant             *value,
                                                                         gsize                *length);
const gchar *                   g_variant_get_bytestring                (GVariant             *value);
gchar *                         g_variant_dup_bytestring                (GVariant             *value,
                                                                         gsize                *length);
const gchar **                  g_variant_get_bytestring_array          (GVariant             *value,
                                                                         gsize                *length);
gchar **                        g_variant_dup_bytestring_array          (GVariant             *value,
                                                                         gsize                *length);

GVariant *                      g_variant_new_maybe                     (const GVariantType   *child_type,
                                                                         GVariant             *child);
GVariant *                      g_variant_new_array                     (const GVariantType   *child_type,
                                                                         GVariant * const     *children,
                                                                         gsize                 n_children);
GVariant *                      g_variant_new_tuple                     (GVariant * const     *children,
                                                                         gsize                 n_children);
GVariant *                      g_variant_new_dict_entry                (GVariant             *key,
                                                                         GVariant             *value);

GVariant *                      g_variant_get_maybe                     (GVariant             *value);
gsize                           g_variant_n_children                    (GVariant             *value);
void                            g_variant_get_child                     (GVariant             *value,
                                                                         gsize                 index_,
                                                                         const gchar          *format_string,
                                                                         ...);
GVariant *                      g_variant_get_child_value               (GVariant             *value,
                                                                         gsize                 index_);
gconstpointer                   g_variant_get_fixed_array               (GVariant             *value,
                                                                         gsize                *n_elements,
                                                                         gsize                 element_size);

gsize                           g_variant_get_size                      (GVariant             *value);
gconstpointer                   g_variant_get_data                      (GVariant             *value);
void                            g_variant_store                         (GVariant             *value,
                                                                         gpointer              data);

gchar *                         g_variant_print                         (GVariant             *value,
                                                                         gboolean              type_annotate);
GString *                       g_variant_print_string                  (GVariant             *value,
                                                                         GString              *string,
                                                                         gboolean              type_annotate);

guint                           g_variant_hash                          (gconstpointer         value);
gboolean                        g_variant_equal                         (gconstpointer         one,
                                                                         gconstpointer         two);

GVariant *                      g_variant_get_normal_form               (GVariant             *value);
gboolean                        g_variant_is_normal_form                (GVariant             *value);
GVariant *                      g_variant_byteswap                      (GVariant             *value);
GVariant *                      g_variant_new_from_data                 (const GVariantType   *type,
                                                                         gconstpointer         data,
                                                                         gsize                 size,
                                                                         gboolean              trusted,
                                                                         GDestroyNotify        notify,
                                                                         gpointer              user_data);

typedef struct _GVariantIter GVariantIter;
struct _GVariantIter {
  /*< private >*/
  gsize x[16];
};

GVariantIter *                  g_variant_iter_new                      (GVariant             *value);
gsize                           g_variant_iter_init                     (GVariantIter         *iter,
                                                                         GVariant             *value);
GVariantIter *                  g_variant_iter_copy                     (GVariantIter         *iter);
gsize                           g_variant_iter_n_children               (GVariantIter         *iter);
void                            g_variant_iter_free                     (GVariantIter         *iter);
GVariant *                      g_variant_iter_next_value               (GVariantIter         *iter);
gboolean                        g_variant_iter_next                     (GVariantIter         *iter,
                                                                         const gchar          *format_string,
                                                                         ...);
gboolean                        g_variant_iter_loop                     (GVariantIter         *iter,
                                                                         const gchar          *format_string,
                                                                         ...);


typedef struct _GVariantBuilder GVariantBuilder;
struct _GVariantBuilder {
  /*< private >*/
  gsize x[16];
};

typedef enum
{
  G_VARIANT_PARSE_ERROR_FAILED
} GVariantParseError;
#define G_VARIANT_PARSE_ERROR (g_variant_parser_get_error_quark ())

GQuark                          g_variant_parser_get_error_quark        (void);

GVariantBuilder *               g_variant_builder_new                   (const GVariantType   *type);
void                            g_variant_builder_unref                 (GVariantBuilder      *builder);
GVariantBuilder *               g_variant_builder_ref                   (GVariantBuilder      *builder);
void                            g_variant_builder_init                  (GVariantBuilder      *builder,
                                                                         const GVariantType   *type);
GVariant *                      g_variant_builder_end                   (GVariantBuilder      *builder);
void                            g_variant_builder_clear                 (GVariantBuilder      *builder);
void                            g_variant_builder_open                  (GVariantBuilder      *builder,
                                                                         const GVariantType   *type);
void                            g_variant_builder_close                 (GVariantBuilder      *builder);
void                            g_variant_builder_add_value             (GVariantBuilder      *builder,
                                                                         GVariant             *value);
void                            g_variant_builder_add                   (GVariantBuilder      *builder,
                                                                         const gchar          *format_string,
                                                                         ...);
void                            g_variant_builder_add_parsed            (GVariantBuilder      *builder,
                                                                         const gchar          *format,
                                                                         ...);

GVariant *                      g_variant_new                           (const gchar          *format_string,
                                                                         ...);
void                            g_variant_get                           (GVariant             *value,
                                                                         const gchar          *format_string,
                                                                         ...);
GVariant *                      g_variant_new_va                        (const gchar          *format_string,
                                                                         const gchar         **endptr,
                                                                         va_list              *app);
void                            g_variant_get_va                        (GVariant             *value,
                                                                         const gchar          *format_string,
                                                                         const gchar         **endptr,
                                                                         va_list              *app);


GVariant *                      g_variant_parse                         (const GVariantType   *type,
                                                                         const gchar          *text,
                                                                         const gchar          *limit,
                                                                         const gchar         **endptr,
                                                                         GError              **error);
GVariant *                      g_variant_new_parsed                    (const gchar          *format,
                                                                         ...);
GVariant *                      g_variant_new_parsed_va                 (const gchar          *format,
                                                                         va_list              *app);

gint                            g_variant_compare                       (gconstpointer one,
                                                                         gconstpointer two);
G_END_DECLS

#endif /* __G_VARIANT_H__ */
