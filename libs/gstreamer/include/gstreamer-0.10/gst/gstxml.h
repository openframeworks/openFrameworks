/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wim.taymans@chello.be>
 *
 * gstxml.h: Header for XML save/restore operations of pipelines
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

#ifndef __GST_XML_H__
#define __GST_XML_H__

#include <gst/gstconfig.h>

#ifndef GST_DISABLE_DEPRECATED
#ifndef GST_DISABLE_LOADSAVE

#include <gst/gstelement.h>

G_BEGIN_DECLS

#define GST_TYPE_XML 		(gst_xml_get_type ())
#define GST_XML(obj) 		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_XML, GstXML))
#define GST_IS_XML(obj) 	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_XML))
#define GST_XML_CLASS(klass) 	(G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_XML, GstXMLClass))
#define GST_IS_XML_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_XML))
#define GST_XML_GET_CLASS(obj) 	(G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_XML, GstXMLClass))

typedef struct _GstXML GstXML;
typedef struct _GstXMLClass GstXMLClass;

/**
 * GstXML:
 * @topelements: list of element nodes
 * @ns: name space
 *
 * XML parser object
 */
struct _GstXML {
  GstObject object;

  /*< public >*/
  GList      *topelements;

  xmlNsPtr ns;

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

struct _GstXMLClass {
  GstObjectClass parent_class;

  /* signal callbacks */
  void (*object_loaded)         (GstXML *xml, GstObject *object, xmlNodePtr self);
  void (*object_saved)          (GstXML *xml, GstObject *object, xmlNodePtr self);

  gpointer _gst_reserved[GST_PADDING];
};

GType		gst_xml_get_type	(void);


/* create an XML document out of a pipeline */
xmlDocPtr	gst_xml_write		(GstElement *element);

/* write a formatted representation of a pipeline to an open file */
gint		gst_xml_write_file	(GstElement *element, FILE *out);

GstXML*		gst_xml_new		(void);

gboolean	gst_xml_parse_doc	(GstXML *xml, xmlDocPtr doc, const guchar *root);
gboolean	gst_xml_parse_file	(GstXML *xml, const guchar *fname, const guchar *root);
gboolean	gst_xml_parse_memory	(GstXML *xml, guchar *buffer, guint size, const gchar *root);


GstElement*	gst_xml_get_element	(GstXML *xml, const guchar *name);
GList*		gst_xml_get_topelements (GstXML *xml);

GstElement*	gst_xml_make_element	(xmlNodePtr cur, GstObject *parent);

G_END_DECLS

#else /* GST_DISABLE_LOADSAVE */

#if defined __GNUC__ && __GNUC__ >= 3
#pragma GCC poison gst_xml_write
#pragma GCC poison gst_xml_new
#pragma GCC poison gst_xml_parse_doc
#pragma GCC poison gst_xml_parse_file
#pragma GCC poison gst_xml_parse_memory
#pragma GCC poison gst_xml_get_element
#pragma GCC poison gst_xml_get_topelements
#endif

#endif /* GST_DISABLE_LOADSAVE */

#endif

#endif /* __GST_XML_H__ */
