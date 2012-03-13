/* 
   Date manipulation routines
   Copyright (C) 1999-2002, 2005, Joe Orton <joe@manyfish.co.uk>

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

#ifndef NE_DATES_H
#define NE_DATES_H

#include <sys/types.h>

#include "ne_defs.h"

NE_BEGIN_DECLS

/* Date manipulation routines as per RFC1123 and RFC1036 */

/* Return current date/time in RFC1123 format */
char *ne_rfc1123_date(time_t anytime);

/* Returns time from date/time using the subset of the ISO8601 format
 * referenced in RFC2518 (e.g as used in the creationdate property in
 * the DAV: namespace). */
time_t ne_iso8601_parse(const char *date);

/* Returns time from date/time in RFC1123 format */
time_t ne_rfc1123_parse(const char *date);

time_t ne_rfc1036_parse(const char *date);

/* Parses asctime date string */
time_t ne_asctime_parse(const char *date);

/* Parse an HTTP-date as per RFC2616 */
time_t ne_httpdate_parse(const char *date);

NE_END_DECLS

#endif /* NE_DATES_H */
