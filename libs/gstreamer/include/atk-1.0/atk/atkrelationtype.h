/* ATK -  Accessibility Toolkit
 * Copyright 2002 Sun Microsystems Inc.
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

#ifndef __ATK_RELATION_TYPE_H__
#define __ATK_RELATION_TYPE_H__

#include <glib.h>

G_BEGIN_DECLS

/**
 *AtkRelationType:
 *@ATK_RELATION_NULL: Not used, represens "no relationship" or an error condition.
 *@ATK_RELATION_CONTROLLED_BY: Indicates an object controlled by one or more target objects.
 *@ATK_RELATION_CONTROLLER_FOR: Indicates an object is an controller for one or more target objects.
 *@ATK_RELATION_LABEL_FOR: Indicates an object is a label for one or more target objects.
 *@ATK_RELATION_LABELLED_BY: Indicates an object is labelled by one or more target objects.
 *@ATK_RELATION_MEMBER_OF: Indicates an object is a member of a group of one or more target objects.
 *@ATK_RELATION_NODE_CHILD_OF: Indicates an object is a cell in a treetable which is displayed because a cell in the same column is expanded and identifies that cell.
 *@ATK_RELATION_FLOWS_TO: Indicates that the object has content that flows logically to another
 *  AtkObject in a sequential way, (for instance text-flow).
 *@ATK_RELATION_FLOWS_FROM: Indicates that the object has content that flows logically from
 *  another AtkObject in a sequential way, (for instance text-flow).
 *@ATK_RELATION_SUBWINDOW_OF: Indicates a subwindow attached to a component but otherwise has no connection in  the UI heirarchy to that component.
 *@ATK_RELATION_EMBEDS: Indicates that the object visually embeds 
 *  another object's content, i.e. this object's content flows around 
 *  another's content.
 *@ATK_RELATION_EMBEDDED_BY: Inverse of %ATK_RELATION_EMBEDS, indicates that
 *  this object's content is visualy embedded in another object.
 *@ATK_RELATION_POPUP_FOR: Indicates that an object is a popup for another object.
 *@ATK_RELATION_PARENT_WINDOW_OF: Indicates that an object is a parent window of another object.
 *@ATK_RELATION_DESCRIBED_BY: Indicates that another object provides descriptive information about this object; more verbose than ATK_RELATION_LABELLED_BY.
 *@ATK_RELATION_DESCRIPTION_FOR: Indicates that an object provides descriptive information about another object; more verbose than ATK_RELATION_LABEL_FOR.
 *@ATK_RELATION_NODE_PARENT_OF: Indicates an object is a cell in a treetable and is expanded to display other cells in the same column.
 *@ATK_RELATION_LAST_DEFINED: Not used, this value indicates the end of the enumeration.
 * 
 *Describes the type of the relation
 **/
typedef enum
{
  ATK_RELATION_NULL = 0,
  ATK_RELATION_CONTROLLED_BY,
  ATK_RELATION_CONTROLLER_FOR,
  ATK_RELATION_LABEL_FOR,
  ATK_RELATION_LABELLED_BY,
  ATK_RELATION_MEMBER_OF,
  ATK_RELATION_NODE_CHILD_OF,
  ATK_RELATION_FLOWS_TO,
  ATK_RELATION_FLOWS_FROM,
  ATK_RELATION_SUBWINDOW_OF, 
  ATK_RELATION_EMBEDS, 
  ATK_RELATION_EMBEDDED_BY, 
  ATK_RELATION_POPUP_FOR, 
  ATK_RELATION_PARENT_WINDOW_OF, 
  ATK_RELATION_DESCRIBED_BY,
  ATK_RELATION_DESCRIPTION_FOR,
  ATK_RELATION_NODE_PARENT_OF,
  ATK_RELATION_LAST_DEFINED
} AtkRelationType;

G_END_DECLS

#endif /* __ATK_RELATION_TYPE_H__ */
