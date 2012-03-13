/* vim: set sw=8: -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * gsf-opendoc-utils.h:  Handle the application neutral portions of OpenDocument
 *
 * Author:  Luciano Wolf (luciano.wolf@indt.org.br)
 *
 * Copyright (C) 2005-2006 INdT - Instituto Nokia de Tecnologia
 * http://www.indt.org.br
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#ifndef GSF_OPENDOC_UTILS_H
#define GSF_OPENDOC_UTILS_H

#include <gsf/gsf.h>
#include <gsf/gsf-libxml.h>
#include <gmodule.h>

G_BEGIN_DECLS

enum {
	OO_NS_OFFICE,
	OO_NS_STYLE,
	OO_NS_TEXT,
	OO_NS_TABLE,
	OO_NS_DRAW,
	OO_NS_NUMBER,
	OO_NS_CHART,
	OO_NS_DR3D,
	OO_NS_FORM,
	OO_NS_SCRIPT,
	OO_NS_CONFIG,
	OO_NS_MATH,
	OO_NS_FO,
	OO_NS_DC,
	OO_NS_META,
	OO_NS_XLINK,
	OO_NS_SVG,

	/* new in 2.0 */
	OO_NS_OOO,
	OO_NS_OOOW,
	OO_NS_OOOC,
	OO_NS_DOM,
	OO_NS_XFORMS,
	OO_NS_XSD,
	OO_NS_XSI,

	OO_NS_PRESENT,	/* added in gsf-1.14.8 */

	/* new in 3.0 */
	OO_NS_RPT,
	OO_NS_OF,
	OO_NS_RDFA,
	OO_NS_FIELD,
	OO_NS_FORMX,

	/* Other OpenDocument 1.1 */
	OO_NS_ANIM,
	OO_NS_DATASTYLE,
	OO_NS_MANIFEST,
	OO_NS_SMIL,

	/* Symphony 1.3 */
	OO_LOTUS_NS_PRODTOOLS,

	/* KOffice 1.6.3 */
	OO_KDE_NS_KOFFICE,

	/*CleverAge ODF Add-in for Microsoft Office 3.0.5224.0 (11.0.8302)*/
	OO_CLEVERAGE_NS_DC,

	/* Microsoft Excel Formulas */
	OO_MS_NS_MSOXL,

	/* Gnumeric ODF extensions */
	OO_GNUM_NS_EXT,

	/* New in ODF 3.2 */
	OO_NS_GRDDL,
	OO_NS_XHTML,
	OO_NS_TABLE_OOO,

	/* New in ODF 3.3 */
	OO_NS_CHART_OOO
};

G_MODULE_EXPORT GsfXMLInNS *get_gsf_ooo_ns (void);
G_MODULE_EXPORT char const *get_gsf_odf_version_string (void);
G_MODULE_EXPORT short get_gsf_odf_version (void);

extern GsfXMLInNS gsf_ooo_ns[]; /* use get_gsf_ooo_ns instead */

/* For 1.15.x s/opendoc/odf/ and s/ooo/odf/ */
GError	*gsf_opendoc_metadata_read    (GsfInput *input,  GsfDocMetaData *md);
void	 gsf_opendoc_metadata_subtree (GsfXMLIn *doc,    GsfDocMetaData *md);
gboolean gsf_opendoc_metadata_write   (GsfXMLOut *output, GsfDocMetaData const *md);

G_END_DECLS

#endif /* GSF_OPENDOC_UTILS_H */
