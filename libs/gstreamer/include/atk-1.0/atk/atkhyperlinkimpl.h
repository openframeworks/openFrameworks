/* ATK -  Accessibility Toolkit
 * Copyright 2001 Sun Microsystems Inc.
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

#if defined(ATK_DISABLE_SINGLE_INCLUDES) && !defined (__ATK_H_INSIDE__) && !defined (ATK_COMPILATION)
#error "Only <atk/atk.h> can be included directly."
#endif

#ifndef __ATK_HYPERLINK_IMPL_H__
#define __ATK_HYPERLINK_IMPL_H__

#include <atk/atkobject.h>
#include <atk/atkhyperlink.h>

G_BEGIN_DECLS

/*
 * The AtkHyperlinkImpl interface should be supported by objects
 * exposed within the hierarchy as children of an AtkHypertext container
 * which correspond to "links" or embedded content within the text.
 * HTML anchors are not, for instance, normally exposed this way,
 * but embedded images and components which appear inline in the
 * content of a text object are. The AtkHyperlinkIface interface
 * allows a means of determining which children are hyperlinks in this
 * sense of the word, and for obtaining their corresponding AtkHyperlink 
 * object, from which the embedding range, URI, etc. can be obtained.
 *
 * To some extent this interface exists because, for historical 
 * reasons, AtkHyperlink was defined as an object type, not an interface.
 * Thus, in order to interact with AtkObjects via AtkHyperlink semantics,
 * a new interface was required.
 */

#define ATK_TYPE_HYPERLINK_IMPL          (atk_hyperlink_impl_get_type ())
#define ATK_IS_HYPERLINK_IMPL(obj)       G_TYPE_CHECK_INSTANCE_TYPE ((obj), ATK_TYPE_HYPERLINK_IMPL)
#define ATK_HYPERLINK_IMPL(obj)             G_TYPE_CHECK_INSTANCE_CAST ((obj), ATK_TYPE_HYPERLINK_IMPL, AtkHyperlinkImpl)
#define ATK_HYPERLINK_IMPL_GET_IFACE(obj)   G_TYPE_INSTANCE_GET_INTERFACE ((obj), ATK_TYPE_HYPERLINK_IMPL, AtkHyperlinkImplIface)

#ifndef _TYPEDEF_ATK_HYPERLINK_IMPL_
#define _TYPEDEF_ATK_HYPERLINK_IMPL__
typedef struct _AtkHyperlinkImpl AtkHyperlinkImpl;
#endif
typedef struct _AtkHyperlinkImplIface AtkHyperlinkImplIface;

struct _AtkHyperlinkImplIface
{
  GTypeInterface parent;
    
  AtkHyperlink*  (* get_hyperlink) (AtkHyperlinkImpl *impl);

  AtkFunction pad1;
};

GType            atk_hyperlink_impl_get_type (void);

AtkHyperlink    *atk_hyperlink_impl_get_hyperlink (AtkHyperlinkImpl *obj);

G_END_DECLS

#endif /* __ATK_HYPERLINK_IMPL_H__ */
