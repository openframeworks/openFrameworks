/* 
   Compressed HTTP response handling
   Copyright (C) 2001-2004, Joe Orton <joe@manyfish.co.uk>

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

#ifndef NE_COMPRESS_H
#define NE_COMPRESS_H

#include "ne_request.h"

NE_BEGIN_DECLS

typedef struct ne_decompress_s ne_decompress;

/* Call this to register a 'reader' callback which will be passed
 * blocks of response body (if the 'acceptance' callback is
 * successful).  If the response body is returned compressed by the
 * server, this reader will receive UNCOMPRESSED blocks.
 *
 * Returns pointer to context object which must be passed to
 * ne_decompress_destroy after the request has been dispatched, to
 * free any internal state.  If an error occurs during decompression,
 * the request will be aborted and session error string set. */
ne_decompress *ne_decompress_reader(ne_request *req, ne_accept_response accpt,
				    ne_block_reader rdr, void *userdata);

/* Destroys decompression state. */
void ne_decompress_destroy(ne_decompress *ctx);

NE_END_DECLS

#endif /* NE_COMPRESS_H */
