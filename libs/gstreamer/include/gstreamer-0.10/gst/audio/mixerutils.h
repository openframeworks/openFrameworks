/* GStreamer
 * Copyright (C) 2005-2006 Tim-Philipp Müller <tim centricular net>
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

#ifndef __GST_AUDIO_MIXERUTILS_H__
#define __GST_AUDIO_MIXERUTILS_H__

#include <gst/gst.h>
#include <gst/interfaces/mixer.h>

G_BEGIN_DECLS

/**
 * GstAudioMixerFilterFunc:
 * @mixer: a #GstElement implementing the #GstMixer interface
 * @user_data: user data
 *
 * Function that will be called by gst_audio_default_registry_mixer_filter()
 * so the caller can decide which mixer elements should be kept and returned.
 * When the mixer element is passed to the callback function, it is opened
 * and in READY state. If you decide to keep the element, you need to set it
 * back to NULL state yourself (unless you want to keep it opened of course).
 *
 * Returns: TRUE if the element should be kept, FALSE otherwise.
 */
typedef gboolean (*GstAudioMixerFilterFunc) (GstMixer * mixer, gpointer user_data);


GList * gst_audio_default_registry_mixer_filter (GstAudioMixerFilterFunc  filter_func,
                                                 gboolean                 first,
                                                 gpointer                 user_data);

G_END_DECLS

#endif /* __GST_AUDIO_MIXERUTILS_H__ */
