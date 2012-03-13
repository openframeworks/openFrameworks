/* 
   HTTP-redirect support
   Copyright (C) 1999-2002, Joe Orton <joe@manyfish.co.uk>

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

#ifndef NE_REDIRECT_H
#define NE_REDIRECT_H

#include "ne_request.h"

NE_BEGIN_DECLS

/* Register redirect handling for the session: if a valid redirect
 * (30x) response is given for any request in this session, the
 * request will fail with the NE_REDIRECT code, and the destination of
 * the redirect can be retrieved using ne_redirect_location(). */
void ne_redirect_register(ne_session *sess);

/* Returns location of last redirect for the session.  Returns NULL if
 * no redirect has been encountered for given session, or the last
 * redirect encountered could not be parsed. */
const ne_uri *ne_redirect_location(ne_session *sess);

NE_END_DECLS

#endif /* NE_REDIRECT_H */
