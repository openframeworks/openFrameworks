/* 
   String utility functions
   Copyright (C) 1999-2009, Joe Orton <joe@manyfish.co.uk>

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

#ifndef NE_STRING_H
#define NE_STRING_H

#include "ne_defs.h"
#include "ne_alloc.h"

#include <stdarg.h>

NE_BEGIN_DECLS

/* ne_token and ne_qtoken return the next token in *str before either
 * the next separator character 'sep' or the NUL terminator.
 * ne_qtoken skips over any parts quoted using a pair of any one of
 * the characters given in 'quotes'.  After returning, *str will point
 * to the next character after the separator, or NULL if no separator
 * character was found.
 * 
 * ne_qtoken will return NULL if unterminated quotes are found. */
char *ne_token(char **str, char sep);
char *ne_qtoken(char **str, char sep, const char *quotes);

/* Return portion of 'str' with any characters in 'whitespace' shaved
 * off the beginning and end.  Modifies str in-place. */
char *ne_shave(char *str, const char *whitespace);

/* Cleanse 'str' of non-printable (e.g. control) characters.  'str' is
 * modified in-place, and returned. */
char *ne_strclean(char *str);

/* Encode 'len' bytes of 'text' to base64.  Returns malloc-allocated
 * NUL-terminated buffer which the caller must free(). */
char *ne_base64(const unsigned char *text, size_t len);

/* Decode NUL-terminated base64-encoded string 'data', placing
 * malloc-allocated raw decoder output in '*out'.  Returns length, or
 * zero on decode error (in which case the content of *out is
 * undefined). */
size_t ne_unbase64(const char *data, unsigned char **out);

/* Dynamically-allocated string buffer.  A string buffer which grows
 * dynamically . (Strings are zero-terminated still).  A
 * string buffer ne_buffer which grows dynamically with the string. */
typedef struct {
    char *data; /* contents: NUL-terminated string */
    size_t used; /* strlen(data) + 1 */
    size_t length; /* number of bytes allocated */
} ne_buffer;

/* Create a new string buffer object. */
ne_buffer *ne_buffer_create(void);

/* Create a new string buffer object with at least 'size' bytes of
 * allocated space. */
ne_buffer *ne_buffer_ncreate(size_t size);

/* Returns size of data in buffer, equiv to strlen(ne_buffer_data(buf)) */
#define ne_buffer_size(buf) ((buf)->used - 1)

/* Concatenate all given strings onto the end of the buffer.  The
 * strings must all be NUL-terminated, and MUST be followed by a NULL
 * argument marking the end of the list.  */
void ne_buffer_concat(ne_buffer *buf, ...)
    ne_attribute_sentinel;

/* Append a NUL-terminated string 'str' to buf. */
void ne_buffer_zappend(ne_buffer *buf, const char *str);

/* Append 'len' bytes of 'data' to buf, where 'data' does not contain
 * a NUL terminator.  (A NUL terminator is appended to buf) */
void ne_buffer_append(ne_buffer *buf, const char *data, size_t len);

/* Append 'len' bytes of 'data' to buf.  All non-ASCII bytes, and
 * ASCII control characters, are escaped.  (Note that this includes
 * the NUL byte). */
void ne_buffer_qappend(ne_buffer *buf, const unsigned char *data, size_t len);

/* Print a string to the end of the buffer using printf-style format
 * string 'format' and subsqeuent arguments.  At most 'max' characters
 * are appended; the number of characters appended (excluding the NUL
 * terminator) is returned.  Behaviour is undefined if 'max' is passed
 * as zero. */
size_t ne_buffer_snprintf(ne_buffer *buf, size_t max, 
                          const char *format, ...)
    ne_attribute((format(printf, 3, 4)));

/* Append a literal, NUL-terminated constant string 'str' to buffer
 * 'buf'. */
#define ne_buffer_czappend(buf, str) \
ne_buffer_append((buf), (str), sizeof((str)) - 1)

/* Clear the string buffer 'buf', making it equivalent to the empty
 * string. */
void ne_buffer_clear(ne_buffer *buf);

/* Grow the allocated size of string buffer 'buf' to at least 'size'
 * bytes. */
void ne_buffer_grow(ne_buffer *buf, size_t size);

/* Re-establish the 'used' invariant if the string buffer data field is
 * altered directly. */
void ne_buffer_altered(ne_buffer *buf);

/* Destroy the string buffer object 'buf' without deallocating the
 * data string.  The data string must subsequently be freed using
 * ne_free(). */
char *ne_buffer_finish(ne_buffer *buf);

/* Destroy a string buffer object. */
void ne_buffer_destroy(ne_buffer *buf);

/* Thread-safe strerror() wrapper; place system error for errno value
 * 'errnum' in 'buffer', which is of length 'buflen'.  Returns
 * 'buffer'. */
char *ne_strerror(int errnum, char *buffer, size_t buflen);

/* ne_strnzcpy copies at most 'n'-1 bytes of 'src' to 'dest', and
 * ensures that 'dest' is subsequently NUL-terminated. */
#define ne_strnzcpy(dest, src, n) do { size_t ne__nm1 = (n) - 1;      \
strncpy(dest, src, ne__nm1); dest[ne__nm1] = '\0'; } while (0)

/* Return a malloc-allocated copy of 'data', of length 'len', with all
 * non-ASCII bytes, and ASCII control characters escaped.  (Note that
 * the escaping includes the NUL byte). */
char *ne_strnqdup(const unsigned char *data, size_t len);

/* Return malloc-allocated concatenation of all NUL-terminated string
 * arguments, up to a terminating NULL pointer. */
char *ne_concat(const char *str, ...)
    ne_attribute_sentinel;

/* Wrapper for snprintf: always NUL-terminates returned buffer, and
 * returns strlen(str). */
size_t ne_snprintf(char *str, size_t size, const char *fmt, ...)
    ne_attribute((format(printf, 3, 4)));

/* Wrapper for vsnprintf. */
size_t ne_vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
    ne_attribute((format(printf, 3, 0)));

/* Implementations of strcasecmp and strncasecmp which behave as
 * defined by the ANSI C strcasecmp() and strncasecmp() when in the
 * POSIX locale; i.e. ignoring the process locale. */

/* Compares 's1' and 's2', ignoring differences in case. */
int ne_strcasecmp(const char *s1, const char *s2);
/* Compares up to 'n' characters of 's1' and 's2', ignoring
 * differences in case. */
int ne_strncasecmp(const char *s1, const char *s2, size_t n);

/* Return lowercase 'c' as in POSIX locale; note difference from ANSI
 * C semantics as both the argument and return value are unsigned
 * char. */
#define ne_tolower(c) (ne_tolower_array()[(unsigned char)c])

const unsigned char *ne_tolower_array(void);

/* Convert an ASCII hexadecimal character in the ranges '0'..'9'
 * 'a'..'f' 'A'..'F' to its numeric equivalent. */
#define NE_ASC2HEX(x) (((x) <= '9') ? ((x) - '0') : \
                       (ne_tolower((x)) + 10 - 'a'))

/* Convert an integer in the range 0..15 to the equivalent (lowercase)
 * ASCII hexadecimal equivalent character, in the range '0..9,'a..f' */
#define NE_HEX2ASC(x) ((char) ((x) > 9 ? ((x) - 10 + 'a') : ((x) + '0')))

NE_END_DECLS

#endif /* NE_STRING_H */
