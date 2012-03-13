/* 
   socket handling interface
   Copyright (C) 1999-2010, Joe Orton <joe@manyfish.co.uk>

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

#ifndef NE_SOCKET_H
#define NE_SOCKET_H

#include <sys/types.h>

#ifdef WIN32
#include <stdlib.h> /* for size_t */
#endif

#include "ne_defs.h"
#include "ne_ssl.h" /* for ne_ssl_context */

NE_BEGIN_DECLS

#define NE_SOCK_ERROR (-1)
/* Read/Write timed out */
#define NE_SOCK_TIMEOUT (-2)
/* Socket was closed */
#define NE_SOCK_CLOSED (-3)
/* Connection was reset (e.g. server crashed) */
#define NE_SOCK_RESET (-4)
/* Secure connection was closed without proper SSL shutdown. */
#define NE_SOCK_TRUNC (-5)

/* ne_socket represents a TCP socket. */
typedef struct ne_socket_s ne_socket;

/* ne_sock_addr represents an address object. */
typedef struct ne_sock_addr_s ne_sock_addr;

#ifndef NE_INET_ADDR_DEFINED
typedef struct ne_inet_addr_s ne_inet_addr;
#endif

/* Perform process-global initialization of any libraries in use.
 * Returns non-zero on error. */
int ne_sock_init(void);

/* Perform process-global shutdown of any libraries in use.  This
 * function only has effect when it has been called an equal number of
 * times to ne_sock_init() for the process. */
void ne_sock_exit(void);

/* Resolve the given hostname.  'flags' must be zero.  Hex
 * string IPv6 addresses (e.g. `::1') may be enclosed in brackets
 * (e.g. `[::1]'). */
ne_sock_addr *ne_addr_resolve(const char *hostname, int flags);

/* Returns zero if name resolution was successful, non-zero on
 * error. */
int ne_addr_result(const ne_sock_addr *addr);

/* Returns the first network address associated with the 'addr'
 * object.  Undefined behaviour if ne_addr_result returns non-zero for
 * 'addr'; otherwise, never returns NULL.  */
const ne_inet_addr *ne_addr_first(ne_sock_addr *addr);

/* Returns the next network address associated with the 'addr' object,
 * or NULL if there are no more. */
const ne_inet_addr *ne_addr_next(ne_sock_addr *addr);

/* NB: the pointers returned by ne_addr_first and ne_addr_next are
 * valid until ne_addr_destroy is called for the corresponding
 * ne_sock_addr object.  They must not be passed to ne_iaddr_free. */

/* If name resolution fails, copies the error string into 'buffer',
 * which is of size 'bufsiz'.  'buffer' is returned. */
char *ne_addr_error(const ne_sock_addr *addr, char *buffer, size_t bufsiz);

/* Destroys an address object created by ne_addr_resolve. */
void ne_addr_destroy(ne_sock_addr *addr);

/* Network address type; IPv4 or IPv6 */
typedef enum {
    ne_iaddr_ipv4 = 0,
    ne_iaddr_ipv6
} ne_iaddr_type;

/* Create a network address object from raw byte representation (in
 * network byte order) of given type.  'raw' must be four bytes for an
 * IPv4 address, 16 bytes for an IPv6 address.  May return NULL if
 * address type is not supported. */
ne_inet_addr *ne_iaddr_make(ne_iaddr_type type, const unsigned char *raw);

/* Compare two network address objects i1 and i2; returns zero if they
 * are equivalent or non-zero otherwise.  */
int ne_iaddr_cmp(const ne_inet_addr *i1, const ne_inet_addr *i2);

/* Return the type of the given network address object. */
ne_iaddr_type ne_iaddr_typeof(const ne_inet_addr *ia);

/* Print the string representation of network address 'ia' into the
 * buffer 'buffer', which is of length 'bufsiz'.  Returns 'buffer'. */
char *ne_iaddr_print(const ne_inet_addr *ia, char *buffer, size_t bufsiz);

/* Dump the raw byte representation (in network byte order) of address
 * 'ia' into the buffer 'buffer', which must be of a suitable length
 * (4 bytes for an IPv4 address, 16 bytes for an IPv6 address).
 * Returns 'buffer'. */
unsigned char *ne_iaddr_raw(const ne_inet_addr *ia, unsigned char *buffer);

/* Perform the reverse name lookup on network address 'ia', placing
 * the returned name in the 'buf' buffer (of length 'bufsiz') if
 * successful.  Returns zero on success, or non-zero on error. */
int ne_iaddr_reverse(const ne_inet_addr *ia, char *buf, size_t bufsiz);

/* Convert network address string 'addr' (for example, "127.0.0.1")
 * into a network address object.  Returns NULL on parse error.  If
 * non-NULL, return value must be freed using ne_iaddr_free. */
ne_inet_addr *ne_iaddr_parse(const char *addr, ne_iaddr_type type);

/* Destroy a network address object created using ne_iaddr_make or
 * ne_iaddr_parse. */
void ne_iaddr_free(ne_inet_addr *addr);

/* Create a socket object; returns NULL on error. */
ne_socket *ne_sock_create(void);

/* Specify an address to which the local end of the socket will be
 * bound during a subsequent ne_sock_connect() call.  If the address
 * passed to ne_sock_connect() is of a different type (family) to
 * 'addr', 'addr' is ignored.  Either 'addr' may be NULL, to use the
 * given port with unspecified address, or 'port' may be 0, to use the
 * given address with an unspecified port.
 *
 * (Note: This function is not equivalent to a BSD socket bind(), it
 * only takes effect during the _connect() call). */
void ne_sock_prebind(ne_socket *sock, const ne_inet_addr *addr,
                     unsigned int port);

/* Connect the socket to server at address 'addr' on port 'port'.
 * Returns zero on success, NE_SOCK_TIMEOUT if a timeout occurs when a
 * non-zero connect timeout is configured (and is supported), or
 * NE_SOCK_ERROR on failure.  */
int ne_sock_connect(ne_socket *sock, const ne_inet_addr *addr, 
                    unsigned int port);

/* Read up to 'count' bytes from socket into 'buffer'.  Returns:
 *   NE_SOCK_* on error,
 *   >0 length of data read into buffer (may be less than 'count')
 */
ssize_t ne_sock_read(ne_socket *sock, char *buffer, size_t count);

/* Read up to 'count' bytes into 'buffer', leaving the data available
 * in the socket buffer to be returned by a subsequent call to
 * ne_sock_read or ne_sock_peek. Returns:
 *   NE_SOCK_* on error,
 *   >0 length of data read into buffer.
 */
ssize_t ne_sock_peek(ne_socket *sock, char *buffer, size_t count);

/* Block for up to 'n' seconds until data becomes available for reading
 * from the socket. Returns:
 *  NE_SOCK_* on error,
 *  NE_SOCK_TIMEOUT if no data arrives in 'n' seconds,
 *  0 if data arrived on the socket.
 */
int ne_sock_block(ne_socket *sock, int n);

/* Write 'count' bytes of 'data' to the socket.  Guarantees to either
 * write all the bytes or to fail.  Returns 0 on success, or NE_SOCK_*
 * on error. */
int ne_sock_fullwrite(ne_socket *sock, const char *data, size_t count); 

/* I/O vector. */
struct ne_iovec {
    void *base;
    size_t len;
};

/* Writes 'count' blocks described by 'vector' to the socket.
 * Guarantees to either write all the bytes or to fail.  Count must be
 * greater than zero and smaller than the system-defined maximum
 * vector limit.  Returns 0 on success, or NE_SOCK_* on error. */
int ne_sock_fullwritev(ne_socket *sock, const struct ne_iovec *vector,
                       int count); 

/* Read an LF-terminated line into 'buffer', and NUL-terminate it.
 * At most 'len' bytes are read (including the NUL terminator).
 * Returns:
 * NE_SOCK_* on error,
 * >0 number of bytes read (including NUL terminator)
 */
ssize_t ne_sock_readline(ne_socket *sock, char *buffer, size_t len);

/* Read exactly 'len' bytes into buffer, or fail; returns 0 on
 * success, NE_SOCK_* on error. */
ssize_t ne_sock_fullread(ne_socket *sock, char *buffer, size_t len);

/* Accepts a connection from listening socket 'fd' and places the
 * socket in 'sock'.  Returns zero on success or -1 on failure. */
int ne_sock_accept(ne_socket *sock, int fd);

/* Returns the file descriptor used for socket 'sock'. */
int ne_sock_fd(const ne_socket *sock);

/* Return address of peer, or NULL on error.  The returned address
 * must be destroyed by caller using ne_iaddr_free. */
ne_inet_addr *ne_sock_peer(ne_socket *sock, unsigned int *port);

/* Close the socket and destroy the socket object.  If SSL is in use
 * for the socket, a closure alert is sent to initiate a clean
 * shutdown, but this function does not wait for the peer's response.
 * Returns zero on success, or non-zero on failure. */
int ne_sock_close(ne_socket *sock);

/* Return current error string for socket. */
const char *ne_sock_error(const ne_socket *sock);

/* Set the error string for the socket; takes printf-like format
 * string. */
void ne_sock_set_error(ne_socket *sock, const char *format, ...)
    ne_attribute((format (printf, 2, 3)));

/* Set read timeout for socket, in seconds; must be a non-zero
 * positive integer. */
void ne_sock_read_timeout(ne_socket *sock, int timeout);

/* Set connect timeout for socket, in seconds; must be a positive
 * integer.  If a timeout of 'zero' is used then then no explicit
 * timeout handling will be used for ne_sock_connect(), and the
 * connect call will only timeout as dictated by the TCP stack. */
void ne_sock_connect_timeout(ne_socket *sock, int timeout);

/* Negotiate an SSL connection on socket as an SSL server, using given
 * SSL context. */
int ne_sock_accept_ssl(ne_socket *sock, ne_ssl_context *ctx);

/* Negotiate an SSL connection on socket as an SSL client, using given
 * SSL context.  The 'userdata' parameter is associated with the
 * underlying SSL library's socket structure for use in callbacks.
 * Returns zero on success, or non-zero on error. */
int ne_sock_connect_ssl(ne_socket *sock, ne_ssl_context *ctx,
                        void *userdata);

/* Retrieve the session ID of the current SSL session.  If 'buf' is
 * non-NULL, on success, copies at most *buflen bytes to 'buf' and
 * sets *buflen to the exact number of bytes copied.  If 'buf' is
 * NULL, on success, sets *buflen to the length of the session ID.
 * Returns zero on success, non-zero on error. */
int ne_sock_sessid(ne_socket *sock, unsigned char *buf, size_t *buflen);

/* Return human-readable name of SSL/TLS cipher used for connection,
 * or NULL if none.  The format of this string is not intended to be
 * fixed or parseable, but is informational only.  Return value is
 * NUL-terminated malloc-allocated string if not NULL, which must be
 * freed by the caller. */
char *ne_sock_cipher(ne_socket *sock);

/* SOCKS proxy protocol version: */
enum ne_sock_sversion {
    NE_SOCK_SOCKSV4 = 0,
    NE_SOCK_SOCKSV4A,
    NE_SOCK_SOCKSV5
};

/* Given a socket 'sock' which is connected to a SOCKS proxy, initiate
 * a connection to a destination server using that proxy, specified
 * either by network address or hostname, at given port 'port'.
 *
 * If 'vers' is NE_SOCKS_V4, addr must be an IPv4 address; hostname
 * and password are ignored; username must be non-NULL.
 *
 * If 'vers' is NE_SOCKS_V4A, hostname must be non-NULL; addr is
 * ignored; password is ignored; username must be non-NULL.
 *
 * If 'vers' is NE_SOCKS_V5, addr may be NULL, in which case hostname
 * must be non-NULL.  addr if non-NULL may be an IPv4 or IPv6 address;
 * username may be NULL, in which case password is ignored.  If
 * username is non-NULL password must also be non-NULL.
 *
 * Returns 0 on success, or NE_SOCK_* on failure - in which case, the
 * socket error string is set.  On failure, the socket must be closed
 * by the caller.
 */
int ne_sock_proxy(ne_socket *sock, enum ne_sock_sversion vers,
                  const ne_inet_addr *addr, const char *hostname, 
                  unsigned int port,
                  const char *username, const char *password);

NE_END_DECLS

#endif /* NE_SOCKET_H */
