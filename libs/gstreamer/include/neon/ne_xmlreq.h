/* 
   XML/HTTP response handling
   Copyright (C) 2004-2006, Joe Orton <joe@manyfish.co.uk>

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

#ifndef NE_XMLREQ_H
#define NE_XMLREQ_H

#include "ne_request.h"
#include "ne_xml.h"

NE_BEGIN_DECLS

/* Read the HTTP response body using calls to ne_read_response_block
 * (so must be enclosed by ne_begin_request/ne_end_request calls), and
 * parse it as an XML document, using the given parser.  Returns NE_*
 * error codes.  If an XML parse error occurs, the session error
 * string is set to the XML parser's error string, and NE_ERROR is
 * returned. */
int ne_xml_parse_response(ne_request *req, ne_xml_parser *parser);

/* Dispatch the HTTP request, parsing the response body as an XML
 * document using the given parser, if the response status class is
 * 2xx and an XML media type is specified for the response entity.  If
 * a non-2xx response code is given, or a non-XML media type is
 * specified, then the response body will be silently discarded.
 *
 * Returns NE_* error codes.  If an XML parse error occurs, the
 * session error string is set to the XML parser's error string, and
 * NE_ERROR is returned. */
int ne_xml_dispatch_request(ne_request *req, ne_xml_parser *parser);

NE_END_DECLS

#endif /* NE_XMLREQ_H */
