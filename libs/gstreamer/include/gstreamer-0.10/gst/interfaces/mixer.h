/* GStreamer Mixer
 * Copyright (C) 2003 Ronald Bultje <rbultje@ronald.bitfreak.net>
 *
 * mixer.h: mixer interface design
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

#ifndef __GST_MIXER_H__
#define __GST_MIXER_H__

#include <gst/gst.h>
#include <gst/interfaces/mixeroptions.h>
#include <gst/interfaces/mixertrack.h>
#include <gst/interfaces/interfaces-enumtypes.h>

G_BEGIN_DECLS

#define GST_TYPE_MIXER \
  (gst_mixer_get_type ())
#define GST_MIXER(obj) \
  (GST_IMPLEMENTS_INTERFACE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_MIXER, GstMixer))
#define GST_MIXER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_MIXER, GstMixerClass))
#define GST_IS_MIXER(obj) \
  (GST_IMPLEMENTS_INTERFACE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_MIXER))
#define GST_IS_MIXER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_MIXER))
#define GST_MIXER_GET_CLASS(inst) \
  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GST_TYPE_MIXER, GstMixerClass))

#define GST_MIXER_TYPE(klass) (klass->mixer_type)

typedef struct _GstMixer GstMixer;
typedef struct _GstMixerClass GstMixerClass;

typedef enum
{
  GST_MIXER_HARDWARE,
  GST_MIXER_SOFTWARE
} GstMixerType;

/**
 * GstMixerMessageType:
 * @GST_MIXER_MESSAGE_INVALID: Not a GstMixer message
 * @GST_MIXER_MESSAGE_MUTE_TOGGLED: A mute-toggled GstMixer message
 * @GST_MIXER_MESSAGE_RECORD_TOGGLED: A record-toggled GstMixer message
 * @GST_MIXER_MESSAGE_VOLUME_CHANGED: A volume-changed GstMixer message
 * @GST_MIXER_MESSAGE_OPTION_CHANGED: An option-changed GstMixer message
 * @GST_MIXER_MESSAGE_OPTIONS_LIST_CHANGED: An options-list-changed
 *     GstMixer message, posted when the list of available options for a
 *     GstMixerOptions object has changed (Since: 0.10.18)
 * @GST_MIXER_MESSAGE_MIXER_CHANGED: A mixer-changed GstMixer message, posted
 *     when the list of available mixer tracks has changed. The application
 *     should re-build its interface in this case (Since: 0.10.18)
 *
 * An enumeration for the type of a GstMixer message received on the bus
 *
 * Since: 0.10.14
 */
typedef enum
{
  GST_MIXER_MESSAGE_INVALID,
  GST_MIXER_MESSAGE_MUTE_TOGGLED,
  GST_MIXER_MESSAGE_RECORD_TOGGLED,
  GST_MIXER_MESSAGE_VOLUME_CHANGED,
  GST_MIXER_MESSAGE_OPTION_CHANGED,
  GST_MIXER_MESSAGE_OPTIONS_LIST_CHANGED,
  GST_MIXER_MESSAGE_MIXER_CHANGED
} GstMixerMessageType;

/**
 * GstMixerFlags:
 * @GST_MIXER_FLAG_NONE: No flags
 * @GST_MIXER_FLAG_AUTO_NOTIFICATIONS: The mixer implementation automatically
 *    sends notification messages.
 * @GST_MIXER_FLAG_HAS_WHITELIST: The mixer implementation flags tracks that
 *    should be displayed by default (whitelisted). Since: 0.10.23
 * @GST_MIXER_FLAG_GROUPING: The mixer implementation will leave some controls
 *    marked without either input or output.  Controls marked as input or
 *    output should be grouped with input & output sliders, even if they
 *    are options or bare switches. Since: 0.10.23
 *
 * Flags indicating which optional features are supported by a mixer
 * implementation.
 *
 * Since: 0.10.14
 */
typedef enum
{
  GST_MIXER_FLAG_NONE                = 0,
  GST_MIXER_FLAG_AUTO_NOTIFICATIONS  = (1<<0),
  GST_MIXER_FLAG_HAS_WHITELIST       = (1<<1),
  GST_MIXER_FLAG_GROUPING            = (1<<2),
} GstMixerFlags;

struct _GstMixerClass {
  GTypeInterface klass;

  GstMixerType mixer_type;

  /* virtual functions */
  const GList *  (* list_tracks)   (GstMixer      *mixer);

  void           (* set_volume)    (GstMixer      *mixer,
                                    GstMixerTrack *track,
                                    gint          *volumes);
  void           (* get_volume)    (GstMixer      *mixer,
                                    GstMixerTrack *track,
                                    gint          *volumes);

  void           (* set_mute)      (GstMixer      *mixer,
                                    GstMixerTrack *track,
                                    gboolean       mute);
  void           (* set_record)    (GstMixer      *mixer,
                                    GstMixerTrack *track,
                                    gboolean       record);
#ifndef GST_DISABLE_DEPRECATED
  /* signals (deprecated) */
  void (* mute_toggled)   (GstMixer      *mixer,
                           GstMixerTrack *channel,
                           gboolean       mute);
  void (* record_toggled) (GstMixer      *mixer,
                           GstMixerTrack *channel,
                           gboolean       record);
  void (* volume_changed) (GstMixer      *mixer,
                           GstMixerTrack *channel,
                           gint          *volumes);
#else
  gpointer padding1[3];
#endif /* not GST_DISABLE_DEPRECATED */

  void          (* set_option)     (GstMixer      *mixer,
                                    GstMixerOptions *opts,
                                    gchar         *value);
  const gchar * (* get_option)     (GstMixer      *mixer,
                                    GstMixerOptions *opts);

#ifndef GST_DISABLE_DEPRECATED
  void (* option_changed) (GstMixer      *mixer,
                           GstMixerOptions *opts,
                           gchar   *option);
#else
  gpointer padding2;
#endif /* not GST_DISABLE_DEPRECATED */

  GstMixerFlags (* get_mixer_flags) (GstMixer *mixer);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING-1];
};

GType           gst_mixer_get_type      (void);

/* virtual class function wrappers */
const GList *   gst_mixer_list_tracks    (GstMixer      *mixer);
void            gst_mixer_set_volume     (GstMixer      *mixer,
                                          GstMixerTrack *track,
                                          gint          *volumes);
void            gst_mixer_get_volume     (GstMixer      *mixer,
                                          GstMixerTrack *track,
                                          gint          *volumes);
void            gst_mixer_set_mute       (GstMixer      *mixer,
                                          GstMixerTrack *track,
                                          gboolean       mute);
void            gst_mixer_set_record     (GstMixer      *mixer,
                                          GstMixerTrack *track,
                                          gboolean       record);
void            gst_mixer_set_option     (GstMixer      *mixer,
                                          GstMixerOptions *opts,
                                          gchar         *value);
const gchar *   gst_mixer_get_option     (GstMixer      *mixer,
                                          GstMixerOptions *opts);

/* trigger bus messages */
void            gst_mixer_mute_toggled   (GstMixer      *mixer,
                                          GstMixerTrack *track,
                                          gboolean       mute);
void            gst_mixer_record_toggled (GstMixer      *mixer,
                                          GstMixerTrack *track,
                                          gboolean       record);
void            gst_mixer_volume_changed (GstMixer      *mixer,
                                          GstMixerTrack *track,
                                          gint          *volumes);
void            gst_mixer_option_changed (GstMixer      *mixer,
                                          GstMixerOptions *opts,
                                          const gchar   *value);

void            gst_mixer_mixer_changed   (GstMixer        *mixer);

void            gst_mixer_options_list_changed (GstMixer        *mixer,
                                                GstMixerOptions *opts);

GstMixerType    gst_mixer_get_mixer_type  (GstMixer *mixer);

GstMixerFlags   gst_mixer_get_mixer_flags (GstMixer *mixer);

/* Functions for recognising and parsing GstMixerMessages on the bus */
GstMixerMessageType gst_mixer_message_get_type (GstMessage *message);
void            gst_mixer_message_parse_mute_toggled (GstMessage *message,
                                                      GstMixerTrack **track,
                                                      gboolean *mute);
void            gst_mixer_message_parse_record_toggled (GstMessage *message,
                                                        GstMixerTrack **track,
                                                        gboolean *record);
void            gst_mixer_message_parse_volume_changed (GstMessage *message,
                                                        GstMixerTrack **track,
                                                        gint **volumes,
                                                        gint *num_channels);
void            gst_mixer_message_parse_option_changed (GstMessage *message,
                                                        GstMixerOptions **options,
                                                        const gchar **value);
void            gst_mixer_message_parse_options_list_changed (GstMessage *message,
                                                              GstMixerOptions **options);

G_END_DECLS

#endif /* __GST_MIXER_H__ */
