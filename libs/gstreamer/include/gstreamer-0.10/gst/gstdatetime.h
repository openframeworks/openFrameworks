/* GStreamer
 * Copyright (C) 2010 Thiago Santos <thiago.sousa.santos@collabora.co.uk>
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

#ifndef __GST_DATE_TIME_H__
#define __GST_DATE_TIME_H__

#include <time.h>
#include <glib.h>

G_BEGIN_DECLS

/**
 * GstDateTime:
 *
 * Opaque, immutable, refcounted struct that stores date, time and timezone
 * information. It currently supports ranges from 0001-01-01 to
 * 9999-12-31 in the Gregorian proleptic calendar.
 *
 * Use the acessor functions to get the stored values.
 */
typedef struct _GstDateTime GstDateTime;

gint	gst_date_time_get_year 		(const GstDateTime * datetime);
gint	gst_date_time_get_month		(const GstDateTime * datetime);
gint	gst_date_time_get_day		(const GstDateTime * datetime);
gint	gst_date_time_get_hour 		(const GstDateTime * datetime);
gint	gst_date_time_get_minute 	(const GstDateTime * datetime);
gint	gst_date_time_get_second 	(const GstDateTime * datetime);
gint	gst_date_time_get_microsecond 	(const GstDateTime * datetime);
gfloat	gst_date_time_get_time_zone_offset (const GstDateTime * datetime);

GstDateTime 	*gst_date_time_new_from_unix_epoch_local_time (gint64 secs);
GstDateTime 	*gst_date_time_new_from_unix_epoch_utc  (gint64 secs);
GstDateTime 	*gst_date_time_new_local_time		(gint year, gint month,
							 gint day, gint hour,
							 gint minute,
							 gdouble seconds);
GstDateTime 	*gst_date_time_new			(gfloat tzoffset,
							 gint year, gint month,
							 gint day, gint hour,
							 gint minute,
							 gdouble seconds);
GstDateTime 	*gst_date_time_new_now_local_time (void);
GstDateTime 	*gst_date_time_new_now_utc 	(void);

GstDateTime 	*gst_date_time_ref 		(GstDateTime * datetime);
void 		gst_date_time_unref 		(GstDateTime * datetime);

G_END_DECLS
#endif /* __GST_DATE_TIME_H__ */
