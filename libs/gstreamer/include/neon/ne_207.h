/* 
   WebDAV 207 multi-status response handling
   Copyright (C) 1999-2006, Joe Orton <joe@manyfish.co.uk>

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

#ifndef NE_207_H
#define NE_207_H

#include "ne_xml.h"
#include "ne_request.h" /* for ne_request */
#include "ne_uri.h" /* for ne_uri */

NE_BEGIN_DECLS

/* The defined state integer for the '{DAV:}prop' element. */
#define NE_207_STATE_PROP (50)
/* This interface reserves the state integers 'x' where 0 < x < 100 */
#define NE_207_STATE_TOP (100)

/* Handling of 207 multistatus XML documents.  A "multistatus"
 * document is made up of a set of responses, each concerned with a
 * particular resource.  Each response may have an associated result
 * status and failure description.  A response is made up of a set of
 * propstats, each of which again may have an associated result status
 * and failure description. */

/* Start and end response callbacks trigger at the start and end of
 * each "response" within the multistatus body. 'uri' gives the URI
 * of the resource which is subject of this response.  The return
 * value of a 'start_response' callback is passed as the 'response'
 * parameter to the corresponding 'end_response' parameter. */
typedef void *ne_207_start_response(void *userdata, const ne_uri *uri);
typedef void ne_207_end_response(void *userdata, void *response,
                                 const ne_status *status,
                                 const char *description);

/* Similarly, start and end callbacks for each propstat within the
 * response.  The return value of the 'start_response' callback for
 * the response in which this propstat is contains is passed as the
 * 'response' parameter.  The return value of each 'start_propstat' is
 * passed as the 'propstat' parameter' to the corresponding
 * 'end_propstat' callback.  If the start_propstat callback returns
 * NULL, parsing is aborted (the XML parser error must be set by the
 * callback). */
typedef void *ne_207_start_propstat(void *userdata, void *response);
typedef void ne_207_end_propstat(void *userdata, void *propstat,
                                 const ne_status *status,
                                 const char *description);

typedef struct ne_207_parser_s ne_207_parser;

/* Create 207 parser an add the handlers the the given parser's
 * handler stack.  URI references in the 207 response will be resolved
 * relative to the base URI 'base'. */
ne_207_parser *ne_207_create(ne_xml_parser *parser, const ne_uri *base, 
                             void *userdata);

/* Register response handling callbacks. */
void ne_207_set_response_handlers(ne_207_parser *p,
                                  ne_207_start_response *start,
                                  ne_207_end_response *end);

/* Register propstat handling callbacks. */
void ne_207_set_propstat_handlers(ne_207_parser *p, 
                                  ne_207_start_propstat *start,
                                  ne_207_end_propstat *end);

/* Destroy the parser */
void ne_207_destroy(ne_207_parser *p);

/* An acceptance function which only accepts 207 responses */
int ne_accept_207(void *userdata, ne_request *req, const ne_status *status);

void *ne_207_get_current_propstat(ne_207_parser *p);
void *ne_207_get_current_response(ne_207_parser *p);

/* Dispatch request 'req', returning:
 *  NE_ERROR: for a dispatch error, or a non-2xx response, or a
 *            207 response which contained a non-2xx propstat
 *  NE_OK: for a 2xx response or a 207 response which contained
 *            only 2xx-class propstats.
 * The request object is destroyed in both cases. */
int ne_simple_request(ne_session *sess, ne_request *req);

NE_END_DECLS

#endif /* NE_207_H */
