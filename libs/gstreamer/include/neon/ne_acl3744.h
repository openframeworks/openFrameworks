/*
   Access control
   Copyright (C) 2001-2007, Joe Orton <joe@manyfish.co.uk>
   Copyright (C) 2001, Arun Garg <arung@pspl.co.in>
   Copyright (C) 2007 Henrik Holst <henrik.holst2@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA

*/

#ifndef NE_ACL3744_H
#define NE_ACL3744_H

#include "ne_session.h"

NE_BEGIN_DECLS

#define NE_ACL_READ             (0x0001)
#define NE_ACL_WRITE            (0x0002)
#define NE_ACL_WRITE_PROPERTIES (0x0004)
#define NE_ACL_WRITE_CONTENT    (0x0008)
#define NE_ACL_UNLOCK           (0x0010)
#define NE_ACL_READ_ACL         (0x0020)
#define NE_ACL_READ_CUPRIVSET   (0x0040)
#define NE_ACL_WRITE_ACL        (0x0080)
#define NE_ACL_BIND             (0x0100)
#define NE_ACL_UNBIND           (0x0200)
#define NE_ACL_ALL              (0x0400)

enum ne_acl_target {
    ne_acl_href,
    ne_acl_property,
    ne_acl_all,
    ne_acl_authenticated,
    ne_acl_unauthenticated,
    ne_acl_self
};

enum ne_acl_optype {
    ne_acl_grant,
    ne_acl_deny
};

/* A simplified representation of an Access Control Element (ACE): */
typedef struct {
    /* Identify the principal(s) to which this ACE applies: */
    enum ne_acl_target target;

    /* Whether to grant or deny access: */
    enum ne_acl_optype type;

    /* If target == ne_acl_href, tname must be non-NULL and give the
     * principal URL.  If target == ne_acl_property, tname must be
     * non-NULL and be a property name, including an XML namespace
     * definition, if appropriate (the default namespace being
     * "DAV:").  This restricts access as described at:
     * http://tools.ietf.org/html/rfc3744#section-5.5.1 */
    char *tname;

    /* The set of privileges to be restricted; a bit mask of one or
     * more of the NE_ACL_* constants defined above: */
    unsigned int privileges;

} ne_acl_entry;

/* Set the ACL for the given resource to the list of ACL entries. */
int ne_acl3744_set(ne_session *sess, const char *path,
                   const ne_acl_entry entries[], int numentries);

NE_END_DECLS

#endif /* NE_ACL3744_H */
