/*
   Access control
   Copyright (C) 2001-2006, Joe Orton <joe@manyfish.co.uk>

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

/*
 * DEPRECATED ACL Interface.  See ne_acl3744.h for replacement API.
 */

#ifndef NE_ACL_H
#define NE_ACL_H

#include "ne_session.h"

NE_BEGIN_DECLS

typedef struct
{
    enum {
	ne_acl_href,
	ne_acl_property,
	ne_acl_all
    } apply;
    
    enum {
	ne_acl_grant,
	ne_acl_deny
    } type;

    char *principal;
    int read;
    int read_acl;
    int write;
    int write_acl;
    int read_cuprivset;
} ne_acl_entry;

/* Set the ACL for the given resource to the list of ACL entries. */
int ne_acl_set(ne_session *sess, const char *uri,
	       const ne_acl_entry entries[], int numentries);

NE_END_DECLS

#endif /* NE_ACL_H */
