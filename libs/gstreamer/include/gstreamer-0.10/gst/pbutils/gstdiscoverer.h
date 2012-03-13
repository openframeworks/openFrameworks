/* GStreamer
 * Copyright (C) 2009 Edward Hervey <edward.hervey@collabora.co.uk>
 *               2009 Nokia Corporation
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

#ifndef _GST_DISCOVERER_H_
#define _GST_DISCOVERER_H_

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_DISCOVERER_STREAM_INFO \
  (gst_discoverer_stream_info_get_type ())
#define GST_DISCOVERER_STREAM_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DISCOVERER_STREAM_INFO, GstDiscovererStreamInfo))
#define GST_IS_DISCOVERER_STREAM_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DISCOVERER_STREAM_INFO))
typedef struct _GstDiscovererStreamInfo GstDiscovererStreamInfo;
typedef GstMiniObjectClass GstDiscovererStreamInfoClass;
GType gst_discoverer_stream_info_get_type (void);

/**
 * GstDiscovererStreamInfo:
 *
 * Base structure for informations concerning a media stream. Depending on the @streamtype,
 * One can find more media-specific information in #GstDiscovererAudioInfo,
 * #GstDiscovererVideoInfo, #GstDiscovererContainerInfo.
 *
 * Since: 0.10.31
 */
#define gst_discoverer_stream_info_ref(info) ((GstDiscovererStreamInfo*) gst_mini_object_ref((GstMiniObject*) info))
#define gst_discoverer_stream_info_unref(info) (gst_mini_object_unref((GstMiniObject*) info))

GstDiscovererStreamInfo* gst_discoverer_stream_info_get_previous(GstDiscovererStreamInfo* info);
GstDiscovererStreamInfo* gst_discoverer_stream_info_get_next(GstDiscovererStreamInfo* info);
GstCaps*                 gst_discoverer_stream_info_get_caps(GstDiscovererStreamInfo* info);
const GstTagList*        gst_discoverer_stream_info_get_tags(GstDiscovererStreamInfo* info);
const GstStructure*      gst_discoverer_stream_info_get_misc(GstDiscovererStreamInfo* info);
const gchar *            gst_discoverer_stream_info_get_stream_type_nick(GstDiscovererStreamInfo* info);

/**
 * GstDiscovererContainerInfo:
 *
 * #GstDiscovererStreamInfo specific to container streams.
 *
 * Since: 0.10.31
 */
#define GST_TYPE_DISCOVERER_CONTAINER_INFO \
  (gst_discoverer_container_info_get_type ())
#define GST_DISCOVERER_CONTAINER_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DISCOVERER_CONTAINER_INFO, GstDiscovererContainerInfo))
#define GST_IS_DISCOVERER_CONTAINER_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DISCOVERER_CONTAINER_INFO))
typedef struct _GstDiscovererContainerInfo GstDiscovererContainerInfo;
typedef GstMiniObjectClass GstDiscovererContainerInfoClass;

GType gst_discoverer_container_info_get_type (void);

GList *gst_discoverer_container_info_get_streams(GstDiscovererContainerInfo *info);


/**
 * GstDiscovererAudioInfo:
 *
 * #GstDiscovererStreamInfo specific to audio streams.
 *
 * Since: 0.10.31
 */
#define GST_TYPE_DISCOVERER_AUDIO_INFO \
  (gst_discoverer_audio_info_get_type ())
#define GST_DISCOVERER_AUDIO_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DISCOVERER_AUDIO_INFO, GstDiscovererAudioInfo))
#define GST_IS_DISCOVERER_AUDIO_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DISCOVERER_AUDIO_INFO))
typedef struct _GstDiscovererAudioInfo GstDiscovererAudioInfo;
typedef GstMiniObjectClass GstDiscovererAudioInfoClass;

GType gst_discoverer_audio_info_get_type (void);

guint gst_discoverer_audio_info_get_channels(const GstDiscovererAudioInfo* info);
guint gst_discoverer_audio_info_get_sample_rate(const GstDiscovererAudioInfo* info);
guint gst_discoverer_audio_info_get_depth(const GstDiscovererAudioInfo* info);
guint gst_discoverer_audio_info_get_bitrate(const GstDiscovererAudioInfo* info);
guint gst_discoverer_audio_info_get_max_bitrate(const GstDiscovererAudioInfo* info);

/**
 * GstDiscovererVideoInfo:
 *
 * #GstDiscovererStreamInfo specific to video streams.
 *
 * Since: 0.10.31
 */
#define GST_TYPE_DISCOVERER_VIDEO_INFO \
  (gst_discoverer_video_info_get_type ())
#define GST_DISCOVERER_VIDEO_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DISCOVERER_VIDEO_INFO, GstDiscovererVideoInfo))
#define GST_IS_DISCOVERER_VIDEO_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DISCOVERER_VIDEO_INFO))
typedef struct _GstDiscovererVideoInfo GstDiscovererVideoInfo;
typedef GstMiniObjectClass GstDiscovererVideoInfoClass;
GType gst_discoverer_video_info_get_type (void);

guint           gst_discoverer_video_info_get_width(const GstDiscovererVideoInfo* info);
guint           gst_discoverer_video_info_get_height(const GstDiscovererVideoInfo* info);
guint           gst_discoverer_video_info_get_depth(const GstDiscovererVideoInfo* info);
guint           gst_discoverer_video_info_get_framerate_num(const GstDiscovererVideoInfo* info);
guint           gst_discoverer_video_info_get_framerate_denom(const GstDiscovererVideoInfo* info);
guint           gst_discoverer_video_info_get_par_num(const GstDiscovererVideoInfo* info);
guint           gst_discoverer_video_info_get_par_denom(const GstDiscovererVideoInfo* info);
gboolean        gst_discoverer_video_info_is_interlaced(const GstDiscovererVideoInfo* info);
guint           gst_discoverer_video_info_get_bitrate(const GstDiscovererVideoInfo* info);
guint           gst_discoverer_video_info_get_max_bitrate(const GstDiscovererVideoInfo* info);
gboolean        gst_discoverer_video_info_is_image(const GstDiscovererVideoInfo* info);

/**
 * GstDiscovererResult:
 * @GST_DISCOVERER_OK: The discovery was successful
 * @GST_DISCOVERER_URI_INVALID: the URI is invalid
 * @GST_DISCOVERER_ERROR: an error happened and the GError is set
 * @GST_DISCOVERER_TIMEOUT: the discovery timed-out
 * @GST_DISCOVERER_BUSY: the discoverer was already discovering a file
 * @GST_DISCOVERER_MISSING_PLUGINS: Some plugins are missing for full discovery
 *
 * Result values for the discovery process.
 *
 * Since: 0.10.31
 */
typedef enum {
  GST_DISCOVERER_OK               = 0,
  GST_DISCOVERER_URI_INVALID      = 1,
  GST_DISCOVERER_ERROR            = 2,
  GST_DISCOVERER_TIMEOUT          = 3,
  GST_DISCOVERER_BUSY             = 4,
  GST_DISCOVERER_MISSING_PLUGINS  = 5
} GstDiscovererResult;


/**
 * GstDiscovererInfo:
 *
 * Structure containing the information of a URI analyzed by #GstDiscoverer.
 *
 * Since: 0.10.31
 */
typedef struct _GstDiscovererInfo GstDiscovererInfo;

#define GST_TYPE_DISCOVERER_INFO \
  (gst_discoverer_info_get_type ())
#define GST_DISCOVERER_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DISCOVERER_INFO, GstDiscovererInfo))
#define GST_IS_DISCOVERER_INFO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DISCOVERER_INFO))
typedef GstMiniObjectClass GstDiscovererInfoClass;
GType gst_discoverer_info_get_type (void);

#define gst_discoverer_info_unref(info) (gst_mini_object_unref((GstMiniObject*)info))
#define gst_discoverer_info_ref(info) (gst_mini_object_ref((GstMiniObject*)info))

GstDiscovererInfo*        gst_discoverer_info_copy (GstDiscovererInfo * ptr);

const gchar*              gst_discoverer_info_get_uri(const GstDiscovererInfo* info);
GstDiscovererResult       gst_discoverer_info_get_result(const GstDiscovererInfo* info);
GstDiscovererStreamInfo*  gst_discoverer_info_get_stream_info(GstDiscovererInfo* info);
GList*                    gst_discoverer_info_get_stream_list(GstDiscovererInfo* info);
GstClockTime              gst_discoverer_info_get_duration(const GstDiscovererInfo* info);
const GstStructure*       gst_discoverer_info_get_misc(const GstDiscovererInfo* info);
const GstTagList*         gst_discoverer_info_get_tags(const GstDiscovererInfo* info);

GList *                   gst_discoverer_info_get_streams (GstDiscovererInfo *info,
							   GType streamtype);
GList *                   gst_discoverer_info_get_audio_streams (GstDiscovererInfo *info);
GList *                   gst_discoverer_info_get_video_streams (GstDiscovererInfo *info);
GList *                   gst_discoverer_info_get_container_streams (GstDiscovererInfo *info);

void                      gst_discoverer_stream_info_list_free (GList *infos);

#define GST_TYPE_DISCOVERER \
  (gst_discoverer_get_type())
#define GST_DISCOVERER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DISCOVERER,GstDiscoverer))
#define GST_DISCOVERER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_DISCOVERER,GstDiscovererClass))
#define GST_IS_DISCOVERER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DISCOVERER))
#define GST_IS_DISCOVERER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_DISCOVERER))

typedef struct _GstDiscoverer GstDiscoverer;
typedef struct _GstDiscovererClass GstDiscovererClass;
typedef struct _GstDiscovererPrivate GstDiscovererPrivate;

/**
 * GstDiscoverer:
 *
 * The #GstDiscoverer structure.
 *
 * Since: 0.10.31
 **/
struct _GstDiscoverer {
  GObject parent;

  GstDiscovererPrivate *priv;

  gpointer _reserved[GST_PADDING];
};

struct _GstDiscovererClass {
  GObjectClass parentclass;

  /*< signals >*/
  void        (*finished)        (GstDiscoverer *discoverer);
  void        (*starting)        (GstDiscoverer *discoverer);
  void        (*discovered)      (GstDiscoverer *discoverer,
                                  GstDiscovererInfo *info,
				  const GError *err);

  gpointer _reserved[GST_PADDING];
};

GType          gst_discoverer_get_type (void);
GstDiscoverer *gst_discoverer_new (GstClockTime timeout, GError **err);

/* Asynchronous API */
void           gst_discoverer_start (GstDiscoverer *discoverer);
void           gst_discoverer_stop (GstDiscoverer *discoverer);
gboolean       gst_discoverer_discover_uri_async (GstDiscoverer *discoverer,
						  const gchar *uri);

/* Synchronous API */
GstDiscovererInfo *
gst_discoverer_discover_uri (GstDiscoverer * discoverer,
			     const gchar * uri,
			     GError ** err);

G_END_DECLS

#endif /* _GST_DISCOVERER_H */
