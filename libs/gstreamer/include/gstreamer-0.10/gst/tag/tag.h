/* GStreamer
 * Copyright (C) 2003 Benjamin Otte <in7y118@public.uni-hamburg.de>
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


#ifndef __GST_TAG_TAG_H__
#define __GST_TAG_TAG_H__

#include <gst/gst.h>

G_BEGIN_DECLS

/* Tag names */

/**
 * GST_TAG_MUSICBRAINZ_TRACKID
 *
 * MusicBrainz track ID
 */
#define GST_TAG_MUSICBRAINZ_TRACKID	"musicbrainz-trackid"
/**
 * GST_TAG_MUSICBRAINZ_ARTISTID
 *
 * MusicBrainz artist ID
 */
#define GST_TAG_MUSICBRAINZ_ARTISTID	"musicbrainz-artistid"
/**
 * GST_TAG_MUSICBRAINZ_ALBUMID
 *
 * MusicBrainz album ID
 */
#define GST_TAG_MUSICBRAINZ_ALBUMID	"musicbrainz-albumid"
/**
 * GST_TAG_MUSICBRAINZ_ALBUMARTISTID
 *
 * MusicBrainz album artist ID
 */
#define GST_TAG_MUSICBRAINZ_ALBUMARTISTID	"musicbrainz-albumartistid"
/**
 * GST_TAG_MUSICBRAINZ_TRMID
 *
 * MusicBrainz track TRM ID
 */
#define GST_TAG_MUSICBRAINZ_TRMID	"musicbrainz-trmid"

/* FIXME 0.11: remove GST_TAG_MUSICBRAINZ_SORTNAME */
#ifndef GST_DISABLE_DEPRECATED
/**
 * GST_TAG_MUSICBRAINZ_SORTNAME
 *
 * MusicBrainz artist sort name
 *
 * Deprecated.  Use GST_TAG_ARTIST_SORTNAME instead.
 */
#define GST_TAG_MUSICBRAINZ_SORTNAME	GST_TAG_ARTIST_SORTNAME
#endif

/**
 * GST_TAG_CMML_STREAM
 *
 * Annodex CMML stream element tag
 */
#define GST_TAG_CMML_STREAM "cmml-stream"
/**
 * GST_TAG_CMML_HEAD
 *
 * Annodex CMML head element tag
 */

#define GST_TAG_CMML_HEAD "cmml-head"
/**
 * GST_TAG_CMML_CLIP
 *
 * Annodex CMML clip element tag
 */
#define GST_TAG_CMML_CLIP "cmml-clip"

/* CDDA tags */

/**
 * GST_TAG_CDDA_CDDB_DISCID:
 *
 * CDDB disc id in its short form (e.g. 'aa063d0f')
 */
#define GST_TAG_CDDA_CDDB_DISCID              "discid"

/**
 * GST_TAG_CDDA_CDDB_DISCID_FULL:
 *
 * CDDB disc id including all details
 */
#define GST_TAG_CDDA_CDDB_DISCID_FULL         "discid-full"

/**
 * GST_TAG_CDDA_MUSICBRAINZ_DISCID:
 *
 * Musicbrainz disc id (e.g. 'ahg7JUcfR3vCYBphSDIogOOWrr0-')
 */
#define GST_TAG_CDDA_MUSICBRAINZ_DISCID       "musicbrainz-discid"

/**
 * GST_TAG_CDDA_MUSICBRAINZ_DISCID_FULL:
 *
 * Musicbrainz disc id details
 */
#define GST_TAG_CDDA_MUSICBRAINZ_DISCID_FULL  "musicbrainz-discid-full"

/**
 * GST_TAG_CAPTURING_SHUTTER_SPEED:
 *
 * Shutter speed used when capturing an image, in seconds. (fraction)
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_SHUTTER_SPEED        "capturing-shutter-speed"

/**
 * GST_TAG_CAPTURING_FOCAL_RATIO:
 *
 * Focal ratio (f-number) used when capturing an image. (double)
 *
 * The value stored is the denominator of the focal ratio (f-number).
 * For example, if this tag value is 2, the focal ratio is f/2.
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_FOCAL_RATIO          "capturing-focal-ratio"

/**
 * GST_TAG_CAPTURING_FOCAL_LENGTH:
 *
 * Focal length used when capturing an image, in mm. (double)
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_FOCAL_LENGTH         "capturing-focal-length"

/**
 * GST_TAG_CAPTURING_DIGITAL_ZOOM_RATIO:
 *
 * Digital zoom ratio used when capturing an image. (double)
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_DIGITAL_ZOOM_RATIO   "capturing-digital-zoom-ratio"

/**
 * GST_TAG_CAPTURING_ISO_SPEED:
 *
 * ISO speed used when capturing an image. (integer)
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_ISO_SPEED           "capturing-iso-speed"

/**
 * GST_TAG_CAPTURING_EXPOSURE_PROGRAM:
 *
 * Type of exposure control used when capturing an image. (string)
 *
 * The allowed values are:
 *   "undefined"
 *   "manual"
 *   "normal" - automatically controlled
 *   "aperture-priority" - user selects aperture value
 *   "shutter-priority" - user selects shutter speed
 *   "creative" - biased towards depth of field
 *   "action" - biased towards fast shutter speed
 *   "portrait" - closeup, leaving background out of focus
 *   "landscape" - landscape photos, background in focus
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_EXPOSURE_PROGRAM     "capturing-exposure-program"

/**
 * GST_TAG_CAPTURING_EXPOSURE_MODE:
 *
 * Exposure mode used when capturing an image. (string)
 *
 * The allowed values are:
 *   "auto-exposure"
 *   "manual-exposure"
 *   "auto-bracket"
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_EXPOSURE_MODE       "capturing-exposure-mode"

/**
 * GST_TAG_CAPTURING_SCENE_CAPTURE_TYPE:
 *
 * Scene mode used when capturing an image. (string)
 *
 * The allowed values are:
 *   "standard"
 *   "landscape"
 *   "portrait"
 *   "night-scene"
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_SCENE_CAPTURE_TYPE  "capturing-scene-capture-type"

/**
 * GST_TAG_CAPTURING_GAIN_ADJUSTMENT:
 *
 * Gain adjustment applied to an image. (string)
 *
 * The allowed values are:
 *   "none"
 *   "low-gain-up"
 *   "high-gain-up"
 *   "low-gain-down"
 *   "high-gain-down"
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_GAIN_ADJUSTMENT     "capturing-gain-adjustment"

/**
 * GST_TAG_CAPTURING_WHITE_BALANCE:
 *
 * White balance mode used when capturing an image. (string)
 *
 * The allowed values are:
 *   "auto"
 *   "manual"
 *   "daylight"
 *   "cloudy"
 *   "tungsten"
 *   "fluorescent"
 *   "fluorescent h" (newer daylight-calibrated fluorescents)
 *   "flash"
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_WHITE_BALANCE       "capturing-white-balance"

/**
 * GST_TAG_CAPTURING_CONTRAST:
 *
 * Direction of contrast processing applied when capturing an image. (string)
 *
 * The allowed values are:
 *  "normal"
 *  "soft"
 *  "hard"
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_CONTRAST            "capturing-contrast"

/**
 * GST_TAG_CAPTURING_SATURATION:
 *
 * Direction of saturation processing applied when capturing an image. (string)
 *
 * The allowed values are:
 *  "normal"
 *  "low-saturation"
 *  "high-saturation"
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_SATURATION          "capturing-saturation"

/**
 * GST_TAG_CAPTURING_SHARPNESS:
 *
 * Direction of sharpness processing applied when capturing an image. (string)
 *
 * The allowed values are:
 *  "normal"
 *  "soft"
 *  "hard"
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_SHARPNESS          "capturing-sharpness"

/**
 * GST_TAG_CAPTURING_FLASH_FIRED:
 *
 * If flash was fired during the capture of an image. (boolean)
 *
 * Note that if this tag isn't present, it should not be assumed that
 * the flash did not fire. It should be treated as unknown.
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_FLASH_FIRED         "capturing-flash-fired"

/**
 * GST_TAG_CAPTURING_FLASH_MODE:
 *
 * The flash mode selected during the capture of an image. (string)
 *
 * The allowed values are:
 *  "auto"
 *  "always"
 *  "never"
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_FLASH_MODE         "capturing-flash-mode"

/**
 * GST_TAG_CAPTURING_METERING_MODE:
 *
 * Defines the way a camera determines the exposure. (string)
 *
 * The allowed values are:
 *   "unknown"
 *   "average"
 *   "center-weighted-average"
 *   "spot"
 *   "multi-spot"
 *   "pattern"
 *   "partial"
 *   "other"
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_METERING_MODE      "capturing-metering-mode"

/**
 * GST_TAG_CAPTURING_SOURCE:
 *
 * Indicates the source of capture. The device/medium used to do the
 * capture. (string)
 *
 * Allowed values are:
 *   "dsc" (= digital still camera)
 *   "transparent-scanner"
 *   "reflex-scanner"
 *   "other"
 *
 * Since: 0.10.31
 */
#define GST_TAG_CAPTURING_SOURCE             "capturing-source"

/**
 * GST_TAG_IMAGE_HORIZONTAL_PPI:
 *
 * Media (image/video) intended horizontal pixel density in ppi. (double)
 *
 * Since: 0.10.31
 */
#define GST_TAG_IMAGE_HORIZONTAL_PPI         "image-horizontal-ppi"
/**
 * GST_TAG_IMAGE_VERTICAL_PPI:
 *
 * Media (image/video) intended vertical pixel density in ppi. (double)
 *
 * Since: 0.10.31
 */
#define GST_TAG_IMAGE_VERTICAL_PPI           "image-vertical-ppi"


/* additional information for image tags */

/**
 * GstTagImageType:
 * @GST_TAG_IMAGE_TYPE_NONE                  : No image type. Can be used to
 *      tell functions such as gst_tag_image_data_to_image_buffer() that no
 *      image type should be set. (Since: 0.10.20)
 * @GST_TAG_IMAGE_TYPE_UNDEFINED             : Undefined/other image type
 * @GST_TAG_IMAGE_TYPE_FRONT_COVER           : Cover (front)
 * @GST_TAG_IMAGE_TYPE_BACK_COVER            : Cover (back)
 * @GST_TAG_IMAGE_TYPE_LEAFLET_PAGE          : Leaflet page
 * @GST_TAG_IMAGE_TYPE_MEDIUM                : Medium (e.g. label side of CD)
 * @GST_TAG_IMAGE_TYPE_LEAD_ARTIST           : Lead artist/lead performer/soloist
 * @GST_TAG_IMAGE_TYPE_ARTIST                : Artist/performer
 * @GST_TAG_IMAGE_TYPE_CONDUCTOR             : Conductor
 * @GST_TAG_IMAGE_TYPE_BAND_ORCHESTRA        : Band/orchestra
 * @GST_TAG_IMAGE_TYPE_COMPOSER              : Composer
 * @GST_TAG_IMAGE_TYPE_LYRICIST              : Lyricist/text writer
 * @GST_TAG_IMAGE_TYPE_RECORDING_LOCATION    : Recording location
 * @GST_TAG_IMAGE_TYPE_DURING_RECORDING      : During recording
 * @GST_TAG_IMAGE_TYPE_DURING_PERFORMANCE    : During performance
 * @GST_TAG_IMAGE_TYPE_VIDEO_CAPTURE         : Movie/video screen capture
 * @GST_TAG_IMAGE_TYPE_FISH                  : A fish as funny as the ID3v2 spec
 * @GST_TAG_IMAGE_TYPE_ILLUSTRATION          : Illustration
 * @GST_TAG_IMAGE_TYPE_BAND_ARTIST_LOGO      : Band/artist logotype
 * @GST_TAG_IMAGE_TYPE_PUBLISHER_STUDIO_LOGO : Publisher/studio logotype
 *
 * Type of image contained in an image tag (specified as field in
 * the image buffer's caps structure)
 *
 * Since: 0.10.9
 */
/* Note: keep in sync with register_tag_image_type_enum() */
typedef enum {
  GST_TAG_IMAGE_TYPE_NONE = -1,
  GST_TAG_IMAGE_TYPE_UNDEFINED = 0,
  GST_TAG_IMAGE_TYPE_FRONT_COVER,
  GST_TAG_IMAGE_TYPE_BACK_COVER,
  GST_TAG_IMAGE_TYPE_LEAFLET_PAGE,
  GST_TAG_IMAGE_TYPE_MEDIUM,
  GST_TAG_IMAGE_TYPE_LEAD_ARTIST,
  GST_TAG_IMAGE_TYPE_ARTIST,
  GST_TAG_IMAGE_TYPE_CONDUCTOR,
  GST_TAG_IMAGE_TYPE_BAND_ORCHESTRA,
  GST_TAG_IMAGE_TYPE_COMPOSER,
  GST_TAG_IMAGE_TYPE_LYRICIST,
  GST_TAG_IMAGE_TYPE_RECORDING_LOCATION,
  GST_TAG_IMAGE_TYPE_DURING_RECORDING,
  GST_TAG_IMAGE_TYPE_DURING_PERFORMANCE,
  GST_TAG_IMAGE_TYPE_VIDEO_CAPTURE,
  GST_TAG_IMAGE_TYPE_FISH,
  GST_TAG_IMAGE_TYPE_ILLUSTRATION,
  GST_TAG_IMAGE_TYPE_BAND_ARTIST_LOGO,
  GST_TAG_IMAGE_TYPE_PUBLISHER_STUDIO_LOGO
} GstTagImageType;

#define GST_TYPE_TAG_IMAGE_TYPE  (gst_tag_image_type_get_type ())
GType   gst_tag_image_type_get_type (void);


/* functions for vorbis comment manipulation */

G_CONST_RETURN gchar *  gst_tag_from_vorbis_tag                 (const gchar *          vorbis_tag);
G_CONST_RETURN gchar *  gst_tag_to_vorbis_tag                   (const gchar *          gst_tag);
void                    gst_vorbis_tag_add                      (GstTagList *           list, 
                                                                 const gchar *          tag, 
                                                                 const gchar *          value);

GList *                 gst_tag_to_vorbis_comments              (const GstTagList *     list, 
                                                                 const gchar *          tag);

/* functions to convert GstBuffers with vorbiscomment contents to GstTagLists and back */
GstTagList *            gst_tag_list_from_vorbiscomment_buffer  (const GstBuffer *      buffer,
                                                                 const guint8 *         id_data,
                                                                 const guint            id_data_length,
                                                                 gchar **               vendor_string);
GstBuffer *             gst_tag_list_to_vorbiscomment_buffer    (const GstTagList *     list,
                                                                 const guint8 *         id_data,
                                                                 const guint            id_data_length,
                                                                 const gchar *          vendor_string);

/* functions for ID3 tag manipulation */

guint                   gst_tag_id3_genre_count                 (void);
G_CONST_RETURN gchar *  gst_tag_id3_genre_get                   (const guint            id);
GstTagList *            gst_tag_list_new_from_id3v1             (const guint8 *         data);

G_CONST_RETURN gchar *  gst_tag_from_id3_tag                    (const gchar *          id3_tag);
G_CONST_RETURN gchar *  gst_tag_from_id3_user_tag               (const gchar *          type,
                                                                 const gchar *          id3_user_tag);
G_CONST_RETURN gchar *  gst_tag_to_id3_tag                      (const gchar *          gst_tag);

gboolean                gst_tag_list_add_id3_image (GstTagList   * tag_list,
                                                    const guint8 * image_data,
                                                    guint          image_data_len,
                                                    guint          id3_picture_type);

/* functions to  convert GstBuffers with xmp packets contents to GstTagLists and back */
GstTagList *            gst_tag_list_from_xmp_buffer  (const GstBuffer *  buffer);
GstBuffer *             gst_tag_list_to_xmp_buffer    (const GstTagList * list,
                                                       gboolean           read_only);

/* functions related to exif */
GstBuffer *             gst_tag_list_to_exif_buffer (const GstTagList * taglist,
                                                     gint byte_order,
                                                     guint32 base_offset);

GstBuffer *             gst_tag_list_to_exif_buffer_with_tiff_header (const GstTagList * taglist);

GstTagList *            gst_tag_list_from_exif_buffer (const GstBuffer * buffer,
                                                       gint byte_order,
                                                       guint32 base_offset);

GstTagList *            gst_tag_list_from_exif_buffer_with_tiff_header (
                                                      const GstBuffer * buffer);

/* other tag-related functions */

gboolean                gst_tag_parse_extended_comment (const gchar  * ext_comment,
                                                        gchar       ** key,
                                                        gchar       ** lang,
                                                        gchar       ** value,
                                                        gboolean       fail_if_no_key);

gchar                 * gst_tag_freeform_string_to_utf8 (const gchar  * data,
                                                         gint           size,
                                                         const gchar ** env_vars);

GstBuffer             * gst_tag_image_data_to_image_buffer (const guint8   * image_data,
                                                            guint            image_data_len,
                                                            GstTagImageType  image_type);

/* FIXME 0.11: replace with a more general gst_tag_library_init() */
void                    gst_tag_register_musicbrainz_tags (void);


/* language tag related functions */

gchar **       gst_tag_get_language_codes (void);

const gchar *  gst_tag_get_language_name (const gchar * language_code);

const gchar *  gst_tag_get_language_code_iso_639_1 (const gchar * lang_code);

const gchar *  gst_tag_get_language_code_iso_639_2B (const gchar * lang_code);

const gchar *  gst_tag_get_language_code_iso_639_2T (const gchar * lang_code);

/**
 * gst_tag_get_language_code:
 * @lang_code: ISO-639 language code (e.g. "deu" or "ger" or "de")
 *
 * Convenience macro wrapping gst_tag_get_language_code_iso_639_1().
 *
 * Since: 0.10.26
 */
#define gst_tag_get_language_code(lang_code) \
    gst_tag_get_language_code_iso_639_1(lang_code)

G_END_DECLS

#endif /* __GST_TAG_TAG_H__ */
