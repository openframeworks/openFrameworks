/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gst.h: Main header for GStreamer, apps should include this
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


#ifndef __GST_H__
#define __GST_H__

#include <glib.h>

#include <gst/glib-compat.h>

#include <gst/gstenumtypes.h>
#include <gst/gstversion.h>

#include <gst/gstbin.h>
#include <gst/gstbuffer.h>
#include <gst/gstbufferlist.h>
#include <gst/gstcaps.h>
#include <gst/gstchildproxy.h>
#include <gst/gstclock.h>
#include <gst/gstdatetime.h>
#include <gst/gstdebugutils.h>
#include <gst/gstelement.h>
#include <gst/gsterror.h>
#include <gst/gstevent.h>
#include <gst/gstghostpad.h>
#include <gst/gstindex.h>
#include <gst/gstindexfactory.h>
#include <gst/gstinfo.h>
#include <gst/gstinterface.h>
#include <gst/gstiterator.h>
#include <gst/gstmarshal.h>
#include <gst/gstmessage.h>
#include <gst/gstminiobject.h>
#include <gst/gstobject.h>
#include <gst/gstpad.h>
#include <gst/gstparamspecs.h>
#include <gst/gstpipeline.h>
#include <gst/gstplugin.h>
#include <gst/gstpoll.h>
#include <gst/gstpreset.h>
#include <gst/gstquery.h>
#include <gst/gstregistry.h>
#include <gst/gstsegment.h>
#include <gst/gststructure.h>
#include <gst/gstsystemclock.h>
#include <gst/gsttaglist.h>
#include <gst/gsttagsetter.h>
#include <gst/gsttask.h>
#include <gst/gsttaskpool.h>
#include <gst/gsttrace.h>
#include <gst/gsttypefind.h>
#include <gst/gsttypefindfactory.h>
#include <gst/gsturi.h>
#include <gst/gstutils.h>
#include <gst/gstvalue.h>
#include <gst/gstxml.h>

#include <gst/gstparse.h>

/* API compatibility stuff */
#include <gst/gstcompat.h>

G_BEGIN_DECLS

void		gst_init			(int *argc, char **argv[]);
gboolean	gst_init_check			(int *argc, char **argv[],
						 GError ** err);
gboolean        gst_is_initialized              (void);
GOptionGroup *	gst_init_get_option_group	(void);
void		gst_deinit			(void);

void		gst_version			(guint *major, guint *minor,
						 guint *micro, guint *nano);
gchar *		gst_version_string		(void);

gboolean        gst_segtrap_is_enabled          (void);
void            gst_segtrap_set_enabled         (gboolean enabled);

gboolean        gst_registry_fork_is_enabled    (void);
void            gst_registry_fork_set_enabled   (gboolean enabled);

gboolean        gst_update_registry (void);

G_END_DECLS

#endif /* __GST_H__ */
