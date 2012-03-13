/* GStreamer
 * Copyright (C) 2003 Benjamin Otte <in7y118@public.uni-hamburg.de>
 *
 * gsttaglist.h: Header for tag support
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


#ifndef __GST_TAGLIST_H__
#define __GST_TAGLIST_H__

#include <gst/gstdatetime.h>
#include <gst/gstbuffer.h>
#include <gst/gststructure.h>
#include <gst/glib-compat.h>

G_BEGIN_DECLS

/**
 * GstTagMergeMode:
 * @GST_TAG_MERGE_UNDEFINED: undefined merge mode
 * @GST_TAG_MERGE_REPLACE_ALL: replace all tags (clear list and append)
 * @GST_TAG_MERGE_REPLACE: replace tags
 * @GST_TAG_MERGE_APPEND: append tags
 * @GST_TAG_MERGE_PREPEND: prepend tags
 * @GST_TAG_MERGE_KEEP: keep existing tags
 * @GST_TAG_MERGE_KEEP_ALL: keep all existing tags
 * @GST_TAG_MERGE_COUNT: the number of merge modes
 *
 * The different tag merging modes are basically replace, overwrite and append,
 * but they can be seen from two directions. Given two taglists: (A) the tags
 * already in the element and (B) the ones that are supplied to the element (
 * e.g. via gst_tag_setter_merge_tags() / gst_tag_setter_add_tags() or a
 * %GST_EVENT_TAG), how are these tags merged?
 * In the table below this is shown for the cases that a tag exists in the list
 * (A) or does not exists (!A) and combinations thereof.
 *
 * <table frame="all" colsep="1" rowsep="1">
 *   <title>merge mode</title>
 *   <tgroup cols='5' align='left'>
 *     <thead>
 *       <row>
 *         <entry>merge mode</entry>
 *         <entry>A + B</entry>
 *         <entry>A + !B</entry>
 *         <entry>!A + B</entry>
 *         <entry>!A + !B</entry>
 *       </row>
 *     </thead>
 *     <tbody>
 *       <row>
 *         <entry>REPLACE_ALL</entry>
 *         <entry>B</entry>
 *         <entry>-</entry>
 *         <entry>B</entry>
 *         <entry>-</entry>
 *       </row>
 *       <row>
 *         <entry>REPLACE</entry>
 *         <entry>B</entry>
 *         <entry>A</entry>
 *         <entry>B</entry>
 *         <entry>-</entry>
 *       </row>
 *       <row>
 *         <entry>APPEND</entry>
 *         <entry>A, B</entry>
 *         <entry>A</entry>
 *         <entry>B</entry>
 *         <entry>-</entry>
 *       </row>
 *       <row>
 *         <entry>PREPEND</entry>
 *         <entry>B, A</entry>
 *         <entry>A</entry>
 *         <entry>B</entry>
 *         <entry>-</entry>
 *       </row>
 *       <row>
 *         <entry>KEEP</entry>
 *         <entry>A</entry>
 *         <entry>A</entry>
 *         <entry>B</entry>
 *         <entry>-</entry>
 *       </row>
 *       <row>
 *         <entry>KEEP_ALL</entry>
 *         <entry>A</entry>
 *         <entry>A</entry>
 *         <entry>-</entry>
 *         <entry>-</entry>
 *       </row>
 *     </tbody>
 *   </tgroup>
 * </table>
 */
typedef enum {
  GST_TAG_MERGE_UNDEFINED,
  GST_TAG_MERGE_REPLACE_ALL,
  GST_TAG_MERGE_REPLACE,
  GST_TAG_MERGE_APPEND,
  GST_TAG_MERGE_PREPEND,
  GST_TAG_MERGE_KEEP,
  GST_TAG_MERGE_KEEP_ALL,
  /* add more */
  GST_TAG_MERGE_COUNT
} GstTagMergeMode;

#define GST_TAG_MODE_IS_VALID(mode)     (((mode) > GST_TAG_MERGE_UNDEFINED) && ((mode) < GST_TAG_MERGE_COUNT))

/**
 * GstTagFlag:
 * @GST_TAG_FLAG_UNDEFINED: undefined flag
 * @GST_TAG_FLAG_META: tag is meta data
 * @GST_TAG_FLAG_ENCODED: tag is encoded
 * @GST_TAG_FLAG_DECODED: tag is decoded
 * @GST_TAG_FLAG_COUNT: number of tag flags
 *
 * Extra tag flags used when registering tags.
 */
typedef enum {
  GST_TAG_FLAG_UNDEFINED,
  GST_TAG_FLAG_META,
  GST_TAG_FLAG_ENCODED,
  GST_TAG_FLAG_DECODED,
  GST_TAG_FLAG_COUNT
} GstTagFlag;

#define GST_TAG_FLAG_IS_VALID(flag)     (((flag) > GST_TAG_FLAG_UNDEFINED) && ((flag) < GST_TAG_FLAG_COUNT))

/* FIXME 0.11: Don't typedef GstTagList to be a GstStructure, they're
 *             internally the same but not from an API point of view.
 *             See bug #518934.
 */
/**
 * GstTagList:
 *
 * Opaque #GstTagList data structure.
 */
#ifdef _FOOL_GTK_DOC_
typedef struct _GstTagList GstTagList;
#else
#ifdef IN_GOBJECT_INTROSPECTION
typedef struct _GstTagList GstTagList;
#else
typedef GstStructure GstTagList;
#endif
#endif

#define GST_TAG_LIST(x)       ((GstTagList *) (x))
#define GST_IS_TAG_LIST(x)    ((x) != NULL && gst_is_tag_list (GST_TAG_LIST (x)))
#define GST_TYPE_TAG_LIST     (gst_tag_list_get_type ())

/**
 * GstTagForeachFunc:
 * @list: the #GstTagList
 * @tag: a name of a tag in @list
 * @user_data: user data
 *
 * A function that will be called in gst_tag_list_foreach(). The function may
 * not modify the tag list.
 */
typedef void (*GstTagForeachFunc) (const GstTagList * list,
                                   const gchar      * tag,
                                   gpointer           user_data);

/**
 * GstTagMergeFunc:
 * @dest: the destination #GValue
 * @src: the source #GValue
 *
 * A function for merging multiple values of a tag used when registering
 * tags.
 */
typedef void (* GstTagMergeFunc) (GValue *dest, const GValue *src);

GType        gst_tag_list_get_type (void);

/* tag registration */
void         gst_tag_register      (const gchar     * name,
                                    GstTagFlag        flag,
                                    GType             type,
                                    const gchar     * nick,
                                    const gchar     * blurb,
                                    GstTagMergeFunc   func);

/* some default merging functions */
void      gst_tag_merge_use_first          (GValue * dest, const GValue * src);
void      gst_tag_merge_strings_with_comma (GValue * dest, const GValue * src);

/* basic tag support */
gboolean               gst_tag_exists          (const gchar * tag);
GType                  gst_tag_get_type        (const gchar * tag);
G_CONST_RETURN gchar * gst_tag_get_nick        (const gchar * tag);
G_CONST_RETURN gchar * gst_tag_get_description (const gchar * tag);
GstTagFlag             gst_tag_get_flag        (const gchar * tag);
gboolean               gst_tag_is_fixed        (const gchar * tag);

/* tag lists */
GstTagList * gst_tag_list_new               (void);
GstTagList * gst_tag_list_new_full          (const gchar * tag, ...);
GstTagList * gst_tag_list_new_full_valist   (va_list var_args);

gboolean     gst_is_tag_list                (gconstpointer p);
GstTagList * gst_tag_list_copy              (const GstTagList * list);
gboolean     gst_tag_list_is_empty          (const GstTagList * list);
void         gst_tag_list_insert            (GstTagList       * into,
                                             const GstTagList * from,
                                             GstTagMergeMode    mode);
GstTagList * gst_tag_list_merge             (const GstTagList * list1,
                                             const GstTagList * list2,
                                             GstTagMergeMode    mode);
void         gst_tag_list_free              (GstTagList       * list);
guint        gst_tag_list_get_tag_size      (const GstTagList * list,
                                             const gchar      * tag);
void         gst_tag_list_add               (GstTagList       * list,
                                             GstTagMergeMode    mode,
                                             const gchar      * tag,
                                             ...) G_GNUC_NULL_TERMINATED;
void         gst_tag_list_add_values        (GstTagList       * list,
                                             GstTagMergeMode    mode,
                                             const gchar      * tag,
                                             ...) G_GNUC_NULL_TERMINATED;
void         gst_tag_list_add_valist        (GstTagList       * list,
                                             GstTagMergeMode    mode,
                                             const gchar      * tag,
                                             va_list        var_args);
void         gst_tag_list_add_valist_values (GstTagList       * list,
                                             GstTagMergeMode    mode,
                                             const gchar      * tag,
                                             va_list            var_args);
void         gst_tag_list_add_value         (GstTagList       * list,
                                             GstTagMergeMode    mode,
                                             const gchar      * tag,
                                             const GValue     * value);
void         gst_tag_list_remove_tag        (GstTagList       * list,
                                             const gchar      * tag);
void         gst_tag_list_foreach           (const GstTagList * list,
                                             GstTagForeachFunc  func,
                                             gpointer           user_data);

G_CONST_RETURN GValue *
             gst_tag_list_get_value_index   (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index);
gboolean     gst_tag_list_copy_value        (GValue           * dest,
                                             const GstTagList * list,
                                             const gchar      * tag);

/* simplifications (FIXME: do we want them?) */
gboolean     gst_tag_list_get_char          (const GstTagList * list,
                                             const gchar      * tag,
                                             gchar            * value);
gboolean     gst_tag_list_get_char_index    (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             gchar            * value);
gboolean     gst_tag_list_get_uchar         (const GstTagList * list,
                                             const gchar      * tag,
                                             guchar           * value);
gboolean     gst_tag_list_get_uchar_index   (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             guchar           * value);
gboolean     gst_tag_list_get_boolean       (const GstTagList * list,
                                             const gchar      * tag,
                                             gboolean         * value);
gboolean     gst_tag_list_get_boolean_index (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             gboolean         * value);
gboolean     gst_tag_list_get_int           (const GstTagList * list,
                                             const gchar      * tag,
                                             gint             * value);
gboolean     gst_tag_list_get_int_index     (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             gint             * value);
gboolean     gst_tag_list_get_uint          (const GstTagList * list,
                                             const gchar      * tag,
                                             guint            * value);
gboolean     gst_tag_list_get_uint_index    (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             guint            * value);
gboolean     gst_tag_list_get_long          (const GstTagList * list,
                                             const gchar      * tag,
                                             glong            * value);
gboolean     gst_tag_list_get_long_index    (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             glong            * value);
gboolean     gst_tag_list_get_ulong         (const GstTagList * list,
                                             const gchar      * tag,
                                             gulong           * value);
gboolean     gst_tag_list_get_ulong_index   (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             gulong           * value);
gboolean     gst_tag_list_get_int64         (const GstTagList * list,
                                             const gchar      * tag,
                                             gint64           * value);
gboolean     gst_tag_list_get_int64_index   (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             gint64           * value);
gboolean     gst_tag_list_get_uint64        (const GstTagList * list,
                                             const gchar      * tag,
                                             guint64          * value);
gboolean     gst_tag_list_get_uint64_index  (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             guint64          * value);
gboolean     gst_tag_list_get_float         (const GstTagList * list,
                                             const gchar      * tag,
                                             gfloat           * value);
gboolean     gst_tag_list_get_float_index   (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             gfloat           * value);
gboolean     gst_tag_list_get_double        (const GstTagList * list,
                                             const gchar      * tag,
                                             gdouble          * value);
gboolean     gst_tag_list_get_double_index  (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             gdouble          * value);
gboolean     gst_tag_list_get_string        (const GstTagList * list,
                                             const gchar      * tag,
                                             gchar           ** value);
gboolean     gst_tag_list_get_string_index  (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             gchar           ** value);
gboolean     gst_tag_list_peek_string_index (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             const gchar     ** value);
gboolean     gst_tag_list_get_pointer       (const GstTagList * list,
                                             const gchar      * tag,
                                             gpointer         * value);
gboolean     gst_tag_list_get_pointer_index (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             gpointer         * value);
gboolean     gst_tag_list_get_date          (const GstTagList * list,
                                             const gchar      * tag,
                                             GDate           ** value);
gboolean     gst_tag_list_get_date_index    (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             GDate           ** value);
gboolean     gst_tag_list_get_date_time     (const GstTagList * list,
                                             const gchar      * tag,
                                             GstDateTime     ** value);
gboolean     gst_tag_list_get_date_time_index (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             GstDateTime     ** value);
gboolean     gst_tag_list_get_buffer        (const GstTagList * list,
                                             const gchar      * tag,
                                             GstBuffer       ** value);
gboolean     gst_tag_list_get_buffer_index  (const GstTagList * list,
                                             const gchar      * tag,
                                             guint              index,
                                             GstBuffer       ** value);

/* GStreamer core tags */
/**
 * GST_TAG_TITLE:
 *
 * commonly used title (string)
 *
 * The title as it should be displayed, e.g. 'The Doll House'
 */
#define GST_TAG_TITLE                  "title"
/**
 * GST_TAG_TITLE_SORTNAME:
 *
 * commonly used title, as used for sorting (string)
 *
 * The title as it should be sorted, e.g. 'Doll House, The'
 *
 * Since: 0.10.15
 */
#define GST_TAG_TITLE_SORTNAME         "title-sortname"
/**
 * GST_TAG_ARTIST:
 *
 * person(s) responsible for the recording (string)
 *
 * The artist name as it should be displayed, e.g. 'Jimi Hendrix' or
 * 'The Guitar Heroes'
 */
#define GST_TAG_ARTIST                 "artist"
/**
 * GST_TAG_ARTIST_SORTNAME:
 *
 * person(s) responsible for the recording, as used for sorting (string)
 *
 * The artist name as it should be sorted, e.g. 'Hendrix, Jimi' or
 * 'Guitar Heroes, The'
 *
 * Since: 0.10.15
 */
/* FIXME 0.11: change to "artist-sortname" */
#define GST_TAG_ARTIST_SORTNAME        "musicbrainz-sortname"
/**
 * GST_TAG_ALBUM:
 *
 * album containing this data (string)
 *
 * The album name as it should be displayed, e.g. 'The Jazz Guitar'
 */
#define GST_TAG_ALBUM                  "album"
/**
 * GST_TAG_ALBUM_SORTNAME:
 *
 * album containing this data, as used for sorting (string)
 *
 * The album name as it should be sorted, e.g. 'Jazz Guitar, The'
 *
 * Since: 0.10.15
 */
#define GST_TAG_ALBUM_SORTNAME         "album-sortname"
/**
 * GST_TAG_ALBUM_ARTIST:
 *
 * The artist of the entire album, as it should be displayed.
 *
 * Since: 0.10.25
 */
#define GST_TAG_ALBUM_ARTIST           "album-artist"
/**
 * GST_TAG_ALBUM_ARTIST_SORTNAME:
 *
 * The artist of the entire album, as it should be sorted.
 *
 * Since: 0.10.25
 */
#define GST_TAG_ALBUM_ARTIST_SORTNAME  "album-artist-sortname"
/**
 * GST_TAG_COMPOSER:
 *
 * person(s) who composed the recording (string)
 *
 * Since: 0.10.15
 */
#define GST_TAG_COMPOSER               "composer"
/**
 * GST_TAG_DATE:
 *
 * date the data was created (#GDate structure)
 */
#define GST_TAG_DATE                   "date"
/**
 * GST_TAG_DATE_TIME:
 *
 * date and time the data was created (#GstDateTime structure)
 *
 * Since: 0.10.31
 */
#define GST_TAG_DATE_TIME              "datetime"
/**
 * GST_TAG_GENRE:
 *
 * genre this data belongs to (string)
 */
#define GST_TAG_GENRE                  "genre"
/**
 * GST_TAG_COMMENT:
 *
 * free text commenting the data (string)
 */
#define GST_TAG_COMMENT                "comment"
/**
 * GST_TAG_EXTENDED_COMMENT:
 *
 * key/value text commenting the data (string)
 *
 * Must be in the form of 'key=comment' or
 * 'key[lc]=comment' where 'lc' is an ISO-639
 * language code.
 *
 * This tag is used for unknown Vorbis comment tags,
 * unknown APE tags and certain ID3v2 comment fields.
 *
 * Since: 0.10.10
 */
#define GST_TAG_EXTENDED_COMMENT       "extended-comment"
/**
 * GST_TAG_TRACK_NUMBER:
 *
 * track number inside a collection (unsigned integer)
 */
#define GST_TAG_TRACK_NUMBER           "track-number"
/**
 * GST_TAG_TRACK_COUNT:
 *
 * count of tracks inside collection this track belongs to (unsigned integer)
 */
#define GST_TAG_TRACK_COUNT            "track-count"
/**
 * GST_TAG_ALBUM_VOLUME_NUMBER:
 *
 * disc number inside a collection (unsigned integer)
 */
#define GST_TAG_ALBUM_VOLUME_NUMBER    "album-disc-number"
/**
 * GST_TAG_ALBUM_VOLUME_COUNT:
 *
 * count of discs inside collection this disc belongs to (unsigned integer)
 */
#define GST_TAG_ALBUM_VOLUME_COUNT    "album-disc-count"
/**
 * GST_TAG_LOCATION:
 *
 * Origin of media as a URI (location, where the original of the file or stream
 * is hosted) (string)
 */
#define GST_TAG_LOCATION               "location"
/**
 * GST_TAG_HOMEPAGE:
 *
 * Homepage for this media (i.e. artist or movie homepage) (string)
 *
 * Since: 0.10.23
 */
#define GST_TAG_HOMEPAGE               "homepage"
/**
 * GST_TAG_DESCRIPTION:
 *
 * short text describing the content of the data (string)
 */
#define GST_TAG_DESCRIPTION            "description"
/**
 * GST_TAG_VERSION:
 *
 * version of this data (string)
 */
#define GST_TAG_VERSION                "version"
/**
 * GST_TAG_ISRC:
 *
 * International Standard Recording Code - see http://www.ifpi.org/isrc/ (string)
 */
#define GST_TAG_ISRC                   "isrc"
/**
 * GST_TAG_ORGANIZATION:
 *
 * organization (string)
 */
#define GST_TAG_ORGANIZATION           "organization"
/**
 * GST_TAG_COPYRIGHT:
 *
 * copyright notice of the data (string)
 */
#define GST_TAG_COPYRIGHT              "copyright"
/**
 * GST_TAG_COPYRIGHT_URI:
 *
 * URI to location where copyright details can be found (string)
 *
 * Since: 0.10.14
 */
#define GST_TAG_COPYRIGHT_URI          "copyright-uri"
/**
 * GST_TAG_CONTACT:
 *
 * contact information (string)
 */
#define GST_TAG_CONTACT                "contact"
/**
 * GST_TAG_LICENSE:
 *
 * license of data (string)
 */
#define GST_TAG_LICENSE                "license"
/**
 * GST_TAG_LICENSE_URI:
 *
 * URI to location where license details can be found (string)
 *
 * Since: 0.10.14
 */
#define GST_TAG_LICENSE_URI            "license-uri"
/**
 * GST_TAG_PERFORMER:
 *
 * person(s) performing (string)
 */
#define GST_TAG_PERFORMER              "performer"
/**
 * GST_TAG_DURATION:
 *
 * length in GStreamer time units (nanoseconds) (unsigned 64-bit integer)
 */
#define GST_TAG_DURATION               "duration"
/**
 * GST_TAG_CODEC:
 *
 * codec the data is stored in (string)
 */
#define GST_TAG_CODEC                  "codec"
/**
 * GST_TAG_VIDEO_CODEC:
 *
 * codec the video data is stored in (string)
 */
#define GST_TAG_VIDEO_CODEC            "video-codec"
/**
 * GST_TAG_AUDIO_CODEC:
 *
 * codec the audio data is stored in (string)
 */
#define GST_TAG_AUDIO_CODEC            "audio-codec"
/**
 * GST_TAG_SUBTITLE_CODEC:
 *
 * codec/format the subtitle data is stored in (string)
 *
 * Since: 0.10.23
 */
#define GST_TAG_SUBTITLE_CODEC         "subtitle-codec"
/**
 * GST_TAG_CONTAINER_FORMAT:
 *
 * container format the data is stored in (string)
 *
 * Since: 0.10.24
 */
#define GST_TAG_CONTAINER_FORMAT       "container-format"
/**
 * GST_TAG_BITRATE:
 *
 * exact or average bitrate in bits/s (unsigned integer)
 */
#define GST_TAG_BITRATE                "bitrate"
/**
 * GST_TAG_NOMINAL_BITRATE:
 *
 * nominal bitrate in bits/s (unsigned integer)
 */
#define GST_TAG_NOMINAL_BITRATE        "nominal-bitrate"
/**
 * GST_TAG_MINIMUM_BITRATE:
 *
 * minimum bitrate in bits/s (unsigned integer)
 */
#define GST_TAG_MINIMUM_BITRATE        "minimum-bitrate"
/**
 * GST_TAG_MAXIMUM_BITRATE:
 *
 * maximum bitrate in bits/s (unsigned integer)
 */
#define GST_TAG_MAXIMUM_BITRATE        "maximum-bitrate"
/**
 * GST_TAG_SERIAL:
 *
 * serial number of track (unsigned integer)
 */
#define GST_TAG_SERIAL                 "serial"
/**
 * GST_TAG_ENCODER:
 *
 * encoder used to encode this stream (string)
 */
#define GST_TAG_ENCODER                "encoder"
/**
 * GST_TAG_ENCODER_VERSION:
 *
 * version of the encoder used to encode this stream (unsigned integer)
 */
#define GST_TAG_ENCODER_VERSION        "encoder-version"
/**
 * GST_TAG_TRACK_GAIN:
 *
 * track gain in db (double)
 */
#define GST_TAG_TRACK_GAIN             "replaygain-track-gain"
/**
 * GST_TAG_TRACK_PEAK:
 *
 * peak of the track (double)
 */
#define GST_TAG_TRACK_PEAK             "replaygain-track-peak"
/**
 * GST_TAG_ALBUM_GAIN:
 *
 * album gain in db (double)
 */
#define GST_TAG_ALBUM_GAIN             "replaygain-album-gain"
/**
 * GST_TAG_ALBUM_PEAK:
 *
 * peak of the album (double)
 */
#define GST_TAG_ALBUM_PEAK             "replaygain-album-peak"
/**
 * GST_TAG_REFERENCE_LEVEL:
 *
 * reference level of track and album gain values (double)
 *
 * Since: 0.10.12
 */
#define GST_TAG_REFERENCE_LEVEL        "replaygain-reference-level"
/**
 * GST_TAG_LANGUAGE_CODE:
 *
 * Language code (ISO-639-1) (string) of the content
 */
#define GST_TAG_LANGUAGE_CODE          "language-code"
/**
 * GST_TAG_IMAGE:
 *
 * image (buffer) (buffer caps should specify the content type and preferably
 * also set "image-type" field as #GstTagImageType)
 *
 * Since: 0.10.6
 */
#define GST_TAG_IMAGE                  "image"
/**
 * GST_TAG_PREVIEW_IMAGE:
 *
 * image that is meant for preview purposes, e.g. small icon-sized version
 * (buffer) (buffer caps should specify the content type)
 *
 * Since: 0.10.7
 */
#define GST_TAG_PREVIEW_IMAGE          "preview-image"

/**
 * GST_TAG_ATTACHMENT:
 *
 * generic file attachment (buffer) (buffer caps should specify the content
 * type and if possible set "filename" to the file name of the
 * attachment)
 *
 * Since: 0.10.21
 */
#define GST_TAG_ATTACHMENT             "attachment"

/**
 * GST_TAG_BEATS_PER_MINUTE:
 *
 * number of beats per minute in audio (double)
 *
 * Since: 0.10.12
 */
#define GST_TAG_BEATS_PER_MINUTE       "beats-per-minute"

/**
 * GST_TAG_KEYWORDS:
 *
 * comma separated keywords describing the content (string).
 *
 * Since: 0.10.21
 */
#define GST_TAG_KEYWORDS               "keywords"

/**
 * GST_TAG_GEO_LOCATION_NAME:
 *
 * human readable descriptive location of where the media has been recorded or
 * produced. (string).
 *
 * Since: 0.10.21
 */
#define GST_TAG_GEO_LOCATION_NAME               "geo-location-name"

/**
 * GST_TAG_GEO_LOCATION_LATITUDE:
 *
 * geo latitude location of where the media has been recorded or produced in
 * degrees according to WGS84 (zero at the equator, negative values for southern
 * latitudes) (double).
 *
 * Since: 0.10.21
 */
#define GST_TAG_GEO_LOCATION_LATITUDE               "geo-location-latitude"

/**
 * GST_TAG_GEO_LOCATION_LONGITUDE:
 *
 * geo longitude location of where the media has been recorded or produced in
 * degrees according to WGS84 (zero at the prime meridian in Greenwich/UK,
 * negative values for western longitudes). (double).
 *
 * Since: 0.10.21
 */
#define GST_TAG_GEO_LOCATION_LONGITUDE               "geo-location-longitude"

/**
 * GST_TAG_GEO_LOCATION_ELEVATION:
 *
 * geo elevation of where the media has been recorded or produced in meters
 * according to WGS84 (zero is average sea level) (double).
 *
 * Since: 0.10.21
 */
#define GST_TAG_GEO_LOCATION_ELEVATION               "geo-location-elevation"
/**
 * GST_TAG_GEO_LOCATION_COUNTRY:
 *
 * The country (english name) where the media has been produced (string).
 *
 * Since: 0.10.29
 */
#define GST_TAG_GEO_LOCATION_COUNTRY                 "geo-location-country"
/**
 * GST_TAG_GEO_LOCATION_CITY:
 *
 * The city (english name) where the media has been produced (string).
 *
 * Since: 0.10.29
 */
#define GST_TAG_GEO_LOCATION_CITY                    "geo-location-city"
/**
 * GST_TAG_GEO_LOCATION_SUBLOCATION:
 *
 * A location 'smaller' than GST_TAG_GEO_LOCATION_CITY that specifies better
 * where the media has been produced. (e.g. the neighborhood) (string).
 *
 * This tag has been added as this is how it is handled/named in XMP's
 * Iptc4xmpcore schema.
 *
 * Since: 0.10.29
 */
#define GST_TAG_GEO_LOCATION_SUBLOCATION             "geo-location-sublocation"
/**
 * GST_TAG_GEO_LOCATION_HORIZONTAL_ERROR:
 *
 * Represents the expected error on the horizontal positioning in
 * meters (double).
 *
 * Since: 0.10.31
 */
#define GST_TAG_GEO_LOCATION_HORIZONTAL_ERROR   "geo-location-horizontal-error"
/**
 * GST_TAG_GEO_LOCATION_MOVEMENT_SPEED:
 *
 * Speed of the capturing device when performing the capture.
 * Represented in m/s. (double)
 *
 * See also #GST_TAG_GEO_LOCATION_MOVEMENT_DIRECTION
 *
 * Since 0.10.30
 */
#define GST_TAG_GEO_LOCATION_MOVEMENT_SPEED       "geo-location-movement-speed"
/**
 * GST_TAG_GEO_LOCATION_MOVEMENT_DIRECTION:
 *
 * Indicates the movement direction of the device performing the capture
 * of a media. It is represented as degrees in floating point representation,
 * 0 means the geographic north, and increases clockwise (double from 0 to 360)
 *
 * See also #GST_TAG_GEO_LOCATION_CAPTURE_DIRECTION
 *
 * Since: 0.10.30
 */
#define GST_TAG_GEO_LOCATION_MOVEMENT_DIRECTION "geo-location-movement-direction"
/**
 * GST_TAG_GEO_LOCATION_CAPTURE_DIRECTION:
 *
 * Indicates the direction the device is pointing to when capturing
 * a media. It is represented as degrees in floating point representation,
 * 0 means the geographic north, and increases clockwise (double from 0 to 360)
 *
 * See also #GST_TAG_GEO_LOCATION_MOVEMENT_DIRECTION
 *
 * Since: 0.10.30
 */
#define GST_TAG_GEO_LOCATION_CAPTURE_DIRECTION  "geo-location-capture-direction"
/**
 * GST_TAG_SHOW_NAME:
 *
 * Name of the show, used for displaying (string)
 *
 * Since: 0.10.26
 */
#define GST_TAG_SHOW_NAME                         "show-name"
/**
 * GST_TAG_SHOW_SORTNAME:
 *
 * Name of the show, used for sorting (string)
 *
 * Since: 0.10.26
 */
#define GST_TAG_SHOW_SORTNAME                     "show-sortname"
/**
 * GST_TAG_SHOW_EPISODE_NUMBER:
 *
 * Number of the episode within a season/show (unsigned integer)
 *
 * Since: 0.10.26
 */
#define GST_TAG_SHOW_EPISODE_NUMBER               "show-episode-number"
/**
 * GST_TAG_SHOW_SEASON_NUMBER:
 *
 * Number of the season of a show/series (unsigned integer)
 *
 * Since: 0.10.26
 */
#define GST_TAG_SHOW_SEASON_NUMBER                "show-season-number"
/**
 * GST_TAG_LYRICS:
 *
 * The lyrics of the media (string)
 *
 * Since: 0.10.26
 */
#define GST_TAG_LYRICS                            "lyrics"
/**
 * GST_TAG_COMPOSER_SORTNAME:
 *
 * The composer's name, used for sorting (string)
 *
 * Since: 0.10.26
 */
#define GST_TAG_COMPOSER_SORTNAME                 "composer-sortname"
/**
 * GST_TAG_GROUPING:
 *
 * Groups together media that are related and spans multiple tracks. An
 * example are multiple pieces of a concerto. (string)
 *
 * Since: 0.10.26
 */
#define GST_TAG_GROUPING                          "grouping"
/**
 * GST_TAG_USER_RATING:
 *
 * Rating attributed by a person (likely the application user).
 * The higher the value, the more the user likes this media
 * (unsigned int from 0 to 100)
 *
 * Since: 0.10.29
 */
#define GST_TAG_USER_RATING                       "user-rating"
/**
 * GST_TAG_DEVICE_MANUFACTURER:
 *
 * Manufacturer of the device used to create the media (string)
 *
 * Since: 0.10.30
 */
#define GST_TAG_DEVICE_MANUFACTURER               "device-manufacturer"
/**
 * GST_TAG_DEVICE_MODEL:
 *
 * Model of the device used to create the media (string)
 *
 * Since: 0.10.30
 */
#define GST_TAG_DEVICE_MODEL                      "device-model"
/**
 * GST_TAG_APPLICATION_NAME:
 *
 * Name of the application used to create the media (string)
 *
 * Since: 0.10.31
 */
#define GST_TAG_APPLICATION_NAME                  "application-name"
/**
 * GST_TAG_APPLICATION_DATA:
 *
 * Arbitrary application data (buffer)
 *
 * Some formats allow application's to add their own arbitrary data
 * into files. This data is application's dependent.
 *
 * Since: 0.10.31
 */
#define GST_TAG_APPLICATION_DATA          "application-data"
/**
 * GST_TAG_IMAGE_ORIENTATION:
 *
 * Represents the 'Orientation' tag from EXIF. Defines how the image
 * should be rotated and mirrored for display. (string)
 *
 * This tag has a predefined set of allowed values:
 *   "rotate-0"
 *   "rotate-90"
 *   "rotate-180"
 *   "rotate-270"
 *   "flip-rotate-0"
 *   "flip-rotate-90"
 *   "flip-rotate-180"
 *   "flip-rotate-270"
 *
 * The naming is adopted according to a possible transformation to perform
 * on the image to fix its orientation, obviously equivalent operations will
 * yield the same result.
 *
 * Rotations indicated by the values are in clockwise direction and
 * 'flip' means an horizontal mirroring.
 *
 * Since: 0.10.30
 */
#define GST_TAG_IMAGE_ORIENTATION            "image-orientation"

G_END_DECLS

#endif /* __GST_TAGLIST_H__ */
