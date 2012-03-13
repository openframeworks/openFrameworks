/* 
   HTTP utility functions
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

#ifndef NE_UTILS_H
#define NE_UTILS_H

#include <sys/types.h>

#include <stdarg.h>
#include <stdio.h>

#include "ne_defs.h"

#ifdef NEON_TRIO
#include <trio.h>
#endif

NE_BEGIN_DECLS

/* Returns a human-readable library version string describing the
 * version and build information; for example: 
 *    "neon 0.2.0: Library build, OpenSSL support" */
const char *ne_version_string(void);

/* Returns non-zero if library version is not of major version
 * 'major', or if minor version is not greater than or equal to
 * 'minor'.  For neon versions with major == 0, all minor versions are
 * presumed to be incompatible.  */
int ne_version_match(int major, int minor);

/* Feature codes: */
#define NE_FEATURE_SSL (1) /* SSL/TLS support */
#define NE_FEATURE_ZLIB (2) /* zlib compression in compress interface */
#define NE_FEATURE_IPV6 (3) /* IPv6 is supported in resolver */
#define NE_FEATURE_LFS (4) /* large file support */
#define NE_FEATURE_SOCKS (5) /* SOCKSv5 support */
#define NE_FEATURE_TS_SSL (6) /* Thread-safe SSL/TLS support */
#define NE_FEATURE_I18N (7) /* i18n error message support */

/* Returns non-zero if library is built with support for the given
 * NE_FEATURE_* feature code 'code'. */
int ne_has_support(int feature);

/* Debugging macro to allow code to be optimized out if debugging is
 * disabled at build time. */
#ifndef NE_DEBUGGING
#define NE_DEBUG if (0) ne_debug
#else /* DEBUGGING */
#define NE_DEBUG ne_debug
#endif /* DEBUGGING */

/* Debugging masks. */
#define NE_DBG_SOCKET (1<<0) /* raw socket */
#define NE_DBG_HTTP (1<<1) /* HTTP request/response handling */
#define NE_DBG_XML (1<<2) /* XML parser */
#define NE_DBG_HTTPAUTH (1<<3) /* HTTP authentication (hiding credentials) */
#define NE_DBG_HTTPPLAIN (1<<4) /* plaintext HTTP authentication */
#define NE_DBG_LOCKS (1<<5) /* WebDAV locking */
#define NE_DBG_XMLPARSE (1<<6) /* low-level XML parser */
#define NE_DBG_HTTPBODY (1<<7) /* HTTP response body blocks */
#define NE_DBG_SSL (1<<8) /* SSL/TLS */
#define NE_DBG_FLUSH (1<<30) /* always flush debugging */

/* Send debugging output to 'stream', for all of the given debug
 * channels.  To disable debugging, pass 'stream' as NULL and 'mask'
 * as 0. */
void ne_debug_init(FILE *stream, int mask);

/* The current debug mask and stream set by the last call to
 * ne_debug_init. */
extern int ne_debug_mask;
extern FILE *ne_debug_stream;

/* Produce debug output if any of channels 'ch' is enabled for
 * debugging. */
void ne_debug(int ch, const char *, ...) ne_attribute((format(printf, 2, 3)));

/* Storing an HTTP status result */
typedef struct {
    int major_version;
    int minor_version;
    int code; /* Status-Code value */
    int klass; /* Class of Status-Code (1-5) */
    char *reason_phrase;
} ne_status;

/* NB: couldn't use 'class' in ne_status because it would clash with
 * the C++ reserved word. */

/* Parse 'status_line' using the the RFC2616 Status-Line grammar.
 * s->reason_phrase is malloc-allocated if non-NULL, and must be
 * free'd by the caller.  Returns 0 on success, in which case all
 * fields of '*s' will be set; or -1 on parse error, in which case
 * '*s' is unmodified. */
int ne_parse_statusline(const char *status_line, ne_status *s);

NE_END_DECLS

#endif /* NE_UTILS_H */
