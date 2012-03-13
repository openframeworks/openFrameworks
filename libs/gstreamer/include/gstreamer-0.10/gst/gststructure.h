/* GStreamer
 * Copyright (C) 2003 David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GST_STRUCTURE_H__
#define __GST_STRUCTURE_H__

#include <gst/gstconfig.h>
#include <glib-object.h>
#include <gst/gstclock.h>
#include <gst/gstdatetime.h>
#include <gst/glib-compat.h>

G_BEGIN_DECLS

#define GST_TYPE_STRUCTURE             (gst_structure_get_type ())
#define GST_STRUCTURE(object)          ((GstStructure *)(object))
#define GST_IS_STRUCTURE(object)       ((object) && (GST_STRUCTURE(object)->type == GST_TYPE_STRUCTURE))

typedef struct _GstStructure GstStructure;

/**
 * GstStructureForeachFunc:
 * @field_id: the #GQuark of the field name
 * @value: the #GValue of the field
 * @user_data: user data
 *
 * A function that will be called in gst_structure_foreach(). The function may
 * not modify @value.
 *
 * Returns: TRUE if the foreach operation should continue, FALSE if
 * the foreach operation should stop with FALSE.
 */
typedef gboolean (*GstStructureForeachFunc) (GQuark   field_id,
					     const GValue * value,
					     gpointer user_data);

/**
 * GstStructureMapFunc:
 * @field_id: the #GQuark of the field name
 * @value: the #GValue of the field
 * @user_data: user data
 *
 * A function that will be called in gst_structure_map_in_place(). The function
 * may modify @value.
 *
 * Returns: TRUE if the map operation should continue, FALSE if
 * the map operation should stop with FALSE.
 */
typedef gboolean (*GstStructureMapFunc)     (GQuark   field_id,
					     GValue * value,
					     gpointer user_data);

/**
 * GstStructure:
 * @type: the GType of a structure
 *
 * The GstStructure object. Most fields are private.
 */
struct _GstStructure {
  GType type;

  /*< private >*/
  GQuark name;

  /* owned by parent structure, NULL if no parent */
  gint *parent_refcount;

  GArray *fields;

  gpointer _gst_reserved;
};

GType                   gst_structure_get_type             (void);

GstStructure *          gst_structure_empty_new            (const gchar *            name);
GstStructure *          gst_structure_id_empty_new         (GQuark                   quark);
GstStructure *          gst_structure_new                  (const gchar *            name,
					                    const gchar *            firstfield,
							    ...);
GstStructure *          gst_structure_new_valist           (const gchar *            name,
						            const gchar *            firstfield,
							    va_list                  varargs);
GstStructure *          gst_structure_id_new               (GQuark                   name_quark,
                                                            GQuark                   field_quark,
                                                            ...);
GstStructure *          gst_structure_copy                 (const GstStructure      *structure);
void			gst_structure_set_parent_refcount  (GstStructure            *structure,
                                                            gint            *refcount);
void                    gst_structure_free                 (GstStructure            *structure);

G_CONST_RETURN gchar *  gst_structure_get_name             (const GstStructure      *structure);
GQuark			gst_structure_get_name_id          (const GstStructure      *structure);
gboolean                gst_structure_has_name             (const GstStructure      *structure,
							    const gchar             *name);
void                    gst_structure_set_name             (GstStructure            *structure,
							    const gchar             *name);

void                    gst_structure_id_set_value         (GstStructure            *structure,
							    GQuark                   field,
							    const GValue            *value);
void                    gst_structure_set_value            (GstStructure            *structure,
							    const gchar             *fieldname,
							    const GValue            *value);
void                    gst_structure_id_take_value         (GstStructure            *structure,
							    GQuark                   field,
							    GValue                  *value);
void                    gst_structure_take_value            (GstStructure            *structure,
							    const gchar             *fieldname,
							    GValue                  *value);
void                    gst_structure_set                  (GstStructure            *structure,
							    const gchar             *fieldname,
							    ...) G_GNUC_NULL_TERMINATED;

void                    gst_structure_set_valist           (GstStructure            *structure,
							    const gchar             *fieldname,
							    va_list varargs);

void                    gst_structure_id_set                (GstStructure            *structure,
							    GQuark                   fieldname,
							    ...) G_GNUC_NULL_TERMINATED;

void                    gst_structure_id_set_valist         (GstStructure            *structure,
							    GQuark                   fieldname,
							    va_list varargs);

gboolean                gst_structure_get_valist           (const GstStructure      *structure,
                                                            const char              *first_fieldname,
                                                            va_list                  args);

gboolean                gst_structure_get                  (const GstStructure      *structure,
                                                            const char              *first_fieldname,
                                                            ...) G_GNUC_NULL_TERMINATED;

gboolean                gst_structure_id_get_valist        (const GstStructure      *structure,
                                                            GQuark                   first_field_id,
                                                            va_list                  args);

gboolean                gst_structure_id_get               (const GstStructure      *structure,
                                                            GQuark                   first_field_id,
                                                            ...) G_GNUC_NULL_TERMINATED;

G_CONST_RETURN GValue * gst_structure_id_get_value         (const GstStructure      *structure,
							    GQuark                   field);
G_CONST_RETURN GValue * gst_structure_get_value            (const GstStructure      *structure,
							    const gchar             *fieldname);
void                    gst_structure_remove_field         (GstStructure            *structure,
							    const gchar             *fieldname);
void                    gst_structure_remove_fields        (GstStructure            *structure,
							     const gchar            *fieldname,
							    ...) G_GNUC_NULL_TERMINATED;
void                    gst_structure_remove_fields_valist (GstStructure             *structure,
							    const gchar             *fieldname,
							    va_list                  varargs);
void                    gst_structure_remove_all_fields    (GstStructure            *structure);

GType                   gst_structure_get_field_type       (const GstStructure      *structure,
							    const gchar             *fieldname);
gboolean                gst_structure_foreach              (const GstStructure      *structure,
							    GstStructureForeachFunc  func,
							    gpointer                 user_data);
gboolean                gst_structure_map_in_place         (GstStructure            *structure,
							    GstStructureMapFunc      func,
							    gpointer                 user_data);
gint                    gst_structure_n_fields             (const GstStructure      *structure);
const gchar *           gst_structure_nth_field_name       (const GstStructure      *structure, guint index);
gboolean                gst_structure_id_has_field         (const GstStructure      *structure,
							    GQuark                   field);
gboolean                gst_structure_id_has_field_typed   (const GstStructure      *structure,
							    GQuark                   field,
							    GType                    type);
gboolean                gst_structure_has_field            (const GstStructure      *structure,
							    const gchar             *fieldname);
gboolean                gst_structure_has_field_typed      (const GstStructure      *structure,
							    const gchar             *fieldname,
							    GType                    type);

/* utility functions */
gboolean                gst_structure_get_boolean          (const GstStructure      *structure,
							    const gchar             *fieldname,
							    gboolean                *value);
gboolean                gst_structure_get_int              (const GstStructure      *structure,
							    const gchar             *fieldname,
							    gint                    *value);
gboolean                gst_structure_get_uint             (const GstStructure      *structure,
							    const gchar             *fieldname,
							    guint                   *value);
gboolean                gst_structure_get_fourcc           (const GstStructure      *structure,
							    const gchar             *fieldname,
							    guint32                 *value);
gboolean                gst_structure_get_double           (const GstStructure      *structure,
							    const gchar             *fieldname,
							    gdouble                 *value);
gboolean                gst_structure_get_date             (const GstStructure      *structure,
							    const gchar             *fieldname,
                                                            GDate                  **value);
gboolean                gst_structure_get_date_time        (const GstStructure      *structure,
							    const gchar             *fieldname,
                                                            GstDateTime              **value);
gboolean                gst_structure_get_clock_time       (const GstStructure      *structure,
							    const gchar             *fieldname,
							    GstClockTime            *value);
G_CONST_RETURN gchar *  gst_structure_get_string           (const GstStructure      *structure,
							    const gchar             *fieldname);
gboolean                gst_structure_get_enum             (const GstStructure      *structure,
							    const gchar             *fieldname,
							    GType                    enumtype,
							    gint                    *value);
gboolean                gst_structure_get_fraction         (const GstStructure      *structure,
							    const gchar             *fieldname,
							    gint *value_numerator,
							    gint *value_denominator);

gchar *                 gst_structure_to_string            (const GstStructure      *structure);
GstStructure *          gst_structure_from_string          (const gchar             *string,
							    gchar                  **end);

gboolean                 gst_structure_fixate_field_nearest_int    (GstStructure *structure,
									 const char   *field_name,
									 int           target);
gboolean                 gst_structure_fixate_field_nearest_double (GstStructure *structure,
									 const char   *field_name,
									 double        target);

gboolean                 gst_structure_fixate_field_boolean (GstStructure *structure,
									 const char   *field_name,
									 gboolean        target);
gboolean                 gst_structure_fixate_field_string (GstStructure *structure,
									 const char   *field_name,
									 const gchar  *target);
gboolean                 gst_structure_fixate_field_nearest_fraction (GstStructure *structure,
									 const char   *field_name,
									 const gint target_numerator,
									 const gint target_denominator);

G_END_DECLS

#endif

