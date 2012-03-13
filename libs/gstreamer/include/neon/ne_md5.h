/* Declaration of functions and data types used for MD5 sum computing
   library functions.
   Copyright (C) 2006, Joe Orton <joe@manyfish.co.uk>
   Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef NE_MD5_H
#define NE_MD5_H 1

#include <stdio.h>

#include "ne_defs.h"

NE_BEGIN_DECLS

/*
 * The following three functions are build up the low level used in
 * the functions `md5_stream' and `md5_buffer'.
 */
struct ne_md5_ctx;

/* Create structure containing state of computation. */
extern struct ne_md5_ctx *ne_md5_create_ctx(void);

/* Starting with the result of former calls of this function (or the
   initialization function update the context for the next LEN bytes
   starting at BUFFER.
   It is necessary that LEN is a multiple of 64!!! */
extern void ne_md5_process_block(const void *buffer, size_t len,
                                 struct ne_md5_ctx *ctx);

/* Starting with the result of former calls of this function (or the
   initialization function update the context for the next LEN bytes
   starting at BUFFER.
   It is NOT required that LEN is a multiple of 64.  */
extern void ne_md5_process_bytes(const void *buffer, size_t len,
                                 struct ne_md5_ctx *ctx);

/* Process the remaining bytes in the buffer and put result from CTX
   in first 16 bytes following RESBUF.  The result is always in little
   endian byte order, so that a byte-wise output yields to the wanted
   ASCII representation of the message digest.

   IMPORTANT: On some systems it is required that RESBUF is correctly
   aligned for a 32 bits value.  */
extern void *ne_md5_finish_ctx(struct ne_md5_ctx *ctx, void *resbuf);


/* Put result from CTX in first 16 bytes following RESBUF.  The result is
   always in little endian byte order, so that a byte-wise output yields
   to the wanted ASCII representation of the message digest.

   IMPORTANT: On some systems it is required that RESBUF is correctly
   aligned for a 32 bits value.  */
extern void *ne_md5_read_ctx(const struct ne_md5_ctx *ctx, void *resbuf);

/* Take a copy of the state structure. */
extern struct ne_md5_ctx *ne_md5_dup_ctx(struct ne_md5_ctx *ctx);

/* Re-initialize the context structure. */
extern void ne_md5_reset_ctx(struct ne_md5_ctx *ctx);

/* Destroy the context structure. */
extern void ne_md5_destroy_ctx(struct ne_md5_ctx *ctx);

/* Compute MD5 message digest for bytes read from STREAM.  The
   resulting message digest number will be written into the 16 bytes
   beginning at RESBLOCK.  */
extern int ne_md5_stream(FILE *stream, void *resblock);

/* Process the remaining bytes in the buffer and put ASCII
   representation of the resulting message digest from CTX in the
   first 33 bytes of BUFFER, including a trailing NUL terminator
   byte.  Returns pointer to buffer. */
char *ne_md5_finish_ascii(struct ne_md5_ctx *ctx, char buffer[33]);

/* MD5 ascii->binary conversion */
void ne_md5_to_ascii(const unsigned char md5_buf[16], char *buffer);
void ne_ascii_to_md5(const char *buffer, unsigned char md5_buf[16]);

NE_END_DECLS

#endif /* NE_MD5_H */
