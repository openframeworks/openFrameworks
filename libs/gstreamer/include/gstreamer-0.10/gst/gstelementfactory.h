/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *               2000,2004 Wim Taymans <wim@fluendo.com>
 *
 * gstelement.h: Header for GstElement
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


#ifndef __GST_ELEMENT_FACTORY_H__
#define __GST_ELEMENT_FACTORY_H__

typedef struct _GstElementFactory GstElementFactory;
typedef struct _GstElementFactoryClass GstElementFactoryClass;

#include <gst/gstconfig.h>
#include <gst/gstelement.h>
#include <gst/gstobject.h>
#include <gst/gstplugin.h>
#include <gst/gstpluginfeature.h>
#include <gst/gstiterator.h>

G_BEGIN_DECLS

/* FIXME 0.11: Move GstElementDetails into a private header and use it internally
 * in GstElementFactory, GstElementClass and the registry
 */

typedef struct _GstElementDetails GstElementDetails;

/**
 * GstElementDetails:
 * @longname: long, english name
 * @klass: string describing the type of element, as an unordered list
 * separated with slashes ('/'). See draft-klass.txt of the design docs
 * for more details and common types
 * @description: what the element is about
 * @author: who wrote this thing?
 *
 * This struct defines the public information about a #GstElement. It contains
 * meta-data about the element that is mostly for the benefit of editors.
 *
 * The @klass member can be used by applications to filter elements based
 * on functionality.
 */
/* FIXME: need translatable stuff in here (how handle in registry)?
 * can't we use _N("long name") in element implementations and use _(longname)
 * in gst_element_factory_get_longname()
 */
struct _GstElementDetails
{
  /*< public > */
  gchar *longname;
  gchar *klass;
  gchar *description;
  gchar *author;

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

/**
 * GST_ELEMENT_DETAILS:
 * @longname: long, english name
 * @klass: string describing the type of element, as an unordered list
 * separated with slashes ('/'). See draft-klass.txt of the design docs
 * for more details and common types
 * @description: what the element is about
 * @author: who wrote this element
 *
 * Macro to initialize #GstElementDetails.
 */
#define GST_ELEMENT_DETAILS(longname,klass,description,author)          \
  { longname, klass, description, author, {NULL} }

/**
 * GST_IS_ELEMENT_DETAILS:
 * @details: the #GstElementDetails to check
 *
 * Tests if element details are initialized.
 */
/* FIXME: what about adding '&& (*__gst_reserved==NULL)' */
#define GST_IS_ELEMENT_DETAILS(details) (                                       \
  (details) && ((details)->longname != NULL) && ((details)->klass != NULL)      \
  && ((details)->description != NULL) && ((details)->author != NULL))

#define GST_TYPE_ELEMENT_FACTORY                (gst_element_factory_get_type())
#define GST_ELEMENT_FACTORY(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_ELEMENT_FACTORY,\
                                                 GstElementFactory))
#define GST_ELEMENT_FACTORY_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_ELEMENT_FACTORY,\
                                                 GstElementFactoryClass))
#define GST_IS_ELEMENT_FACTORY(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_ELEMENT_FACTORY))
#define GST_IS_ELEMENT_FACTORY_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_ELEMENT_FACTORY))
#define GST_ELEMENT_FACTORY_CAST(obj)           ((GstElementFactory *)(obj))

/**
 * GstElementFactory:
 *
 * The opaque #GstElementFactory data structure.
 */
struct _GstElementFactory {
  GstPluginFeature      parent;

  GType                 type;                   /* unique GType of element or 0 if not loaded */

  /* FIXME-0.11: deprecate this in favour of meta_data */
  GstElementDetails     details;

  GList *               staticpadtemplates;
  guint                 numpadtemplates;

  /* URI interface stuff */
  guint                 uri_type;
  gchar **              uri_protocols;

  GList *               interfaces;             /* interfaces this element implements */

  /*< private >*/
  /* FIXME-0.11: move up and replace details */
  gpointer		meta_data;
  gpointer _gst_reserved[GST_PADDING - 1];
};

struct _GstElementFactoryClass {
  GstPluginFeatureClass parent_class;

  gpointer _gst_reserved[GST_PADDING];
};

GType                   gst_element_factory_get_type            (void);

GstElementFactory *     gst_element_factory_find                (const gchar *name);

GType                   gst_element_factory_get_element_type    (GstElementFactory *factory);
G_CONST_RETURN gchar *  gst_element_factory_get_longname        (GstElementFactory *factory);
G_CONST_RETURN gchar *  gst_element_factory_get_klass           (GstElementFactory *factory);
G_CONST_RETURN gchar *  gst_element_factory_get_description     (GstElementFactory *factory);
G_CONST_RETURN gchar *  gst_element_factory_get_author          (GstElementFactory *factory);
G_CONST_RETURN gchar *  gst_element_factory_get_documentation_uri (GstElementFactory *factory);
G_CONST_RETURN gchar *  gst_element_factory_get_icon_name       (GstElementFactory *factory);
guint                   gst_element_factory_get_num_pad_templates (GstElementFactory *factory);
G_CONST_RETURN GList *  gst_element_factory_get_static_pad_templates (GstElementFactory *factory);
gint                    gst_element_factory_get_uri_type        (GstElementFactory *factory);
gchar **                gst_element_factory_get_uri_protocols   (GstElementFactory *factory);
gboolean                gst_element_factory_has_interface       (GstElementFactory *factory,
                                                                 const gchar *interfacename);

GstElement*             gst_element_factory_create              (GstElementFactory *factory,
                                                                 const gchar *name);
GstElement*             gst_element_factory_make                (const gchar *factoryname, const gchar *name);

/* FIXME 0.11: move these two into our private headers */
void                    __gst_element_factory_add_static_pad_template (GstElementFactory *elementfactory,
                                                                 GstStaticPadTemplate *templ);
void                    __gst_element_factory_add_interface     (GstElementFactory *elementfactory,
                                                                 const gchar *interfacename);
gboolean                gst_element_register                    (GstPlugin *plugin, const gchar *name,
                                                                 guint rank, GType type);

/* Factory list functions */

/**
 * GstFactoryListType:
 * @GST_ELEMENT_FACTORY_TYPE_DECODER: Decoder elements
 * @GST_ELEMENT_FACTORY_TYPE_ENCODER: Encoder elements
 * @GST_ELEMENT_FACTORY_TYPE_SINK: Sink elements
 * @GST_ELEMENT_FACTORY_TYPE_SRC: Source elements
 * @GST_ELEMENT_FACTORY_TYPE_MUXER: Muxer elements
 * @GST_ELEMENT_FACTORY_TYPE_DEMUXER: Demuxer elements
 * @GST_ELEMENT_FACTORY_TYPE_PARSER: Parser elements
 * @GST_ELEMENT_FACTORY_TYPE_PAYLOADER: Payloader elements
 * @GST_ELEMENT_FACTORY_TYPE_DEPAYLOADER: Depayloader elements
 * @GST_ELEMENT_FACTORY_TYPE_MAX_ELEMENTS: Private, do not use
 * @GST_ELEMENT_FACTORY_TYPE_MEDIA_VIDEO: Elements handling video media types
 * @GST_ELEMENT_FACTORY_TYPE_MEDIA_AUDIO: Elements handling audio media types
 * @GST_ELEMENT_FACTORY_TYPE_MEDIA_IMAGE: Elements handling image media types
 * @GST_ELEMENT_FACTORY_TYPE_MEDIA_SUBTITLE: Elements handling subtitle media types
 * @GST_ELEMENT_FACTORY_TYPE_MEDIA_METADATA: Elements handling metadata media types
 *
 * The type of #GstElementFactory to filter.
 *
 * All @GstFactoryListType up to @GST_ELEMENT_FACTORY_TYPE_MAX_ELEMENTS are exclusive.
 *
 * If one or more of the MEDIA types are specified, then only elements
 * matching the specified media types will be selected.
 *
 * Since: 0.10.31
 */

typedef guint64 GstElementFactoryListType;

#define  GST_ELEMENT_FACTORY_TYPE_DECODER        (G_GUINT64_CONSTANT (1) << 0)
#define  GST_ELEMENT_FACTORY_TYPE_ENCODER        (G_GUINT64_CONSTANT (1) << 1)
#define  GST_ELEMENT_FACTORY_TYPE_SINK           (G_GUINT64_CONSTANT (1) << 2)
#define  GST_ELEMENT_FACTORY_TYPE_SRC            (G_GUINT64_CONSTANT (1) << 3)
#define  GST_ELEMENT_FACTORY_TYPE_MUXER          (G_GUINT64_CONSTANT (1) << 4)
#define  GST_ELEMENT_FACTORY_TYPE_DEMUXER        (G_GUINT64_CONSTANT (1) << 5)
#define  GST_ELEMENT_FACTORY_TYPE_PARSER         (G_GUINT64_CONSTANT (1) << 6)
#define  GST_ELEMENT_FACTORY_TYPE_PAYLOADER      (G_GUINT64_CONSTANT (1) << 7)
#define  GST_ELEMENT_FACTORY_TYPE_DEPAYLOADER    (G_GUINT64_CONSTANT (1) << 8)
#define  GST_ELEMENT_FACTORY_TYPE_FORMATTER      (G_GUINT64_CONSTANT (1) << 9)

#define  GST_ELEMENT_FACTORY_TYPE_MAX_ELEMENTS   (G_GUINT64_CONSTANT (1) << 48)

#define  GST_ELEMENT_FACTORY_TYPE_MEDIA_VIDEO    (G_GUINT64_CONSTANT (1) << 49)
#define  GST_ELEMENT_FACTORY_TYPE_MEDIA_AUDIO    (G_GUINT64_CONSTANT (1) << 50)
#define  GST_ELEMENT_FACTORY_TYPE_MEDIA_IMAGE    (G_GUINT64_CONSTANT (1) << 51)
#define  GST_ELEMENT_FACTORY_TYPE_MEDIA_SUBTITLE (G_GUINT64_CONSTANT (1) << 52)
#define  GST_ELEMENT_FACTORY_TYPE_MEDIA_METADATA (G_GUINT64_CONSTANT (1) << 53)

/**
 * GST_ELEMENT_FACTORY_TYPE_ANY:
 *
 * Elements of any of the defined GST_ELEMENT_FACTORY_LIST types
 *
 * Since: 0.10.31
 */
#define  GST_ELEMENT_FACTORY_TYPE_ANY ((G_GUINT64_CONSTANT (1) << 49) - 1)

/**
 * GST_ELEMENT_FACTORY_TYPE_MEDIA_ANY:
 *
 * Elements matching any of the defined GST_ELEMENT_FACTORY_TYPE_MEDIA types
 *
 * Note: Do not use this if you wish to not filter against any of the defined
 * media types. If you wish to do this, simply don't specify any 
 * GST_ELEMENT_FACTORY_TYPE_MEDIA flag.
 *
 * Since: 0.10.31
 */
#define GST_ELEMENT_FACTORY_TYPE_MEDIA_ANY (~G_GUINT64_CONSTANT (0) << 48)

/**
 * GST_ELEMENT_FACTORY_TYPE_VIDEO_ENCODER:
 *
 * All encoders handling video or image media types
 *
 * Since: 0.10.31
 */
#define GST_ELEMENT_FACTORY_TYPE_VIDEO_ENCODER (GST_ELEMENT_FACTORY_TYPE_ENCODER | GST_ELEMENT_FACTORY_TYPE_MEDIA_VIDEO | GST_ELEMENT_FACTORY_TYPE_MEDIA_IMAGE)

/**
 * GST_ELEMENT_FACTORY_TYPE_AUDIO_ENCODER:
 *
 * All encoders handling audio media types
 *
 * Since: 0.10.31
 */
#define GST_ELEMENT_FACTORY_TYPE_AUDIO_ENCODER (GST_ELEMENT_FACTORY_TYPE_ENCODER | GST_ELEMENT_FACTORY_TYPE_MEDIA_AUDIO)

/**
 * GST_ELEMENT_FACTORY_TYPE_AUDIOVIDEO_SINKS:
 *
 * All sinks handling audio, video or image media types
 *
 * Since: 0.10.31
 */
#define GST_ELEMENT_FACTORY_TYPE_AUDIOVIDEO_SINKS (GST_ELEMENT_FACTORY_TYPE_SINK | GST_ELEMENT_FACTORY_TYPE_MEDIA_AUDIO | GST_ELEMENT_FACTORY_TYPE_MEDIA_VIDEO | GST_ELEMENT_FACTORY_TYPE_MEDIA_IMAGE)

/**
 * GST_ELEMENT_FACTORY_TYPE_DECODABLE:
 *
 * All elements used to 'decode' streams (decoders, demuxers, parsers, depayloaders)
 *
 * Since: 0.10.31
 */
#define GST_ELEMENT_FACTORY_TYPE_DECODABLE \
  (GST_ELEMENT_FACTORY_TYPE_DECODER | GST_ELEMENT_FACTORY_TYPE_DEMUXER | GST_ELEMENT_FACTORY_TYPE_DEPAYLOADER | GST_ELEMENT_FACTORY_TYPE_PARSER)

/* Element klass defines */
#define GST_ELEMENT_FACTORY_KLASS_DECODER		"Decoder"
#define GST_ELEMENT_FACTORY_KLASS_ENCODER		"Encoder"
#define GST_ELEMENT_FACTORY_KLASS_SINK			"Sink"
#define GST_ELEMENT_FACTORY_KLASS_SRC			"Source"
#define GST_ELEMENT_FACTORY_KLASS_MUXER			"Muxer"
#define GST_ELEMENT_FACTORY_KLASS_DEMUXER		"Demuxer"
#define GST_ELEMENT_FACTORY_KLASS_PARSER		"Parser"
#define GST_ELEMENT_FACTORY_KLASS_PAYLOADER		"Payloader"
#define GST_ELEMENT_FACTORY_KLASS_DEPAYLOADER		"Depayloader"
#define GST_ELEMENT_FACTORY_KLASS_FORMATTER		"Formatter"

#define GST_ELEMENT_FACTORY_KLASS_MEDIA_VIDEO		"Video"
#define GST_ELEMENT_FACTORY_KLASS_MEDIA_AUDIO		"Audio"
#define GST_ELEMENT_FACTORY_KLASS_MEDIA_IMAGE		"Image"
#define GST_ELEMENT_FACTORY_KLASS_MEDIA_SUBTITLE	"Subtitle"
#define GST_ELEMENT_FACTORY_KLASS_MEDIA_METADATA	"Metadata"

gboolean      gst_element_factory_list_is_type      (GstElementFactory *factory,
						     GstElementFactoryListType type);

GList *       gst_element_factory_list_get_elements (GstElementFactoryListType type,
						     GstRank minrank);


GList *       gst_element_factory_list_filter       (GList *list, const GstCaps *caps,
						     GstPadDirection direction,
						     gboolean subsetonly);
G_END_DECLS

#endif /* __GST_ELEMENT_FACTORY_H__ */
