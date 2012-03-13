/* 
   HTTP Request Handling
   Copyright (C) 1999-2006, 2008, Joe Orton <joe@manyfish.co.uk>

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

#ifndef NE_REQUEST_H
#define NE_REQUEST_H

#include "ne_utils.h" /* For ne_status */
#include "ne_string.h" /* For ne_buffer */
#include "ne_session.h"

NE_BEGIN_DECLS

#define NE_OK (0) /* Success */
#define NE_ERROR (1) /* Generic error; use ne_get_error(session) for message */
#define NE_LOOKUP (2) /* Server or proxy hostname lookup failed */
#define NE_AUTH (3) /* User authentication failed on server */
#define NE_PROXYAUTH (4) /* User authentication failed on proxy */
#define NE_CONNECT (5) /* Could not connect to server */
#define NE_TIMEOUT (6) /* Connection timed out */
#define NE_FAILED (7) /* The precondition failed */
#define NE_RETRY (8) /* Retry request (ne_end_request ONLY) */
#define NE_REDIRECT (9) /* See ne_redirect.h */

/* Opaque object representing a single HTTP request. */
typedef struct ne_request_s ne_request;

/***** Request Handling *****/

/* Create a request in session 'sess', with given method and path.
 * 'path' must conform to the 'abs_path' grammar in RFC2396, with an
 * optional "? query" part, and MUST be URI-escaped by the caller. */
ne_request *ne_request_create(ne_session *sess,
			      const char *method, const char *path);

/* The request body will be taken from 'size' bytes of 'buffer'. */
void ne_set_request_body_buffer(ne_request *req, const char *buffer,
				size_t size);

/* The request body will be taken from 'length' bytes read from the
 * file descriptor 'fd', starting from file offset 'offset'. */
void ne_set_request_body_fd(ne_request *req, int fd,
                            ne_off_t offset, ne_off_t length);

/* "Pull"-based request body provider: a callback which is invoked to
 * provide blocks of request body on demand.
 *
 * Before each time the body is provided, the callback will be called
 * once with buflen == 0.  The body may have to be provided >1 time
 * per request (for authentication retries etc.).
 *
 * For a call with buflen == 0, the callback must return zero on success
 * or non-zero on error; the session error string must be set on error.
 * For a call with buflen > 0, the callback must return:
 *        <0           : error, abort request; session error string must be set.
 *         0           : ignore 'buffer' contents, end of body.
 *     0 < x <= buflen : buffer contains x bytes of body data.  */
typedef ssize_t (*ne_provide_body)(void *userdata, 
				   char *buffer, size_t buflen);

/* Install a callback which is invoked as needed to provide the
 * request body, a block at a time.  The total size of the request
 * body is 'length'; the callback must ensure that it returns no more
 * than 'length' bytes in total. */
void ne_set_request_body_provider(ne_request *req, ne_off_t length,
				  ne_provide_body provider, void *userdata);

/* Handling response bodies; two callbacks must be provided:
 *
 * 1) 'acceptance' callback: determines whether you want to handle the
 * response body given the response-status information, e.g., if you
 * only want 2xx responses, say so here.
 *
 * 2) 'reader' callback: passed blocks of the response-body as they
 * arrive, if the acceptance callback returned non-zero.  */

/* 'acceptance' callback type. Return non-zero to accept the response,
 * else zero to ignore it. */
typedef int (*ne_accept_response)(void *userdata, ne_request *req, 
                                  const ne_status *st);

/* An 'acceptance' callback which only accepts 2xx-class responses.
 * Ignores userdata. */
int ne_accept_2xx(void *userdata, ne_request *req, const ne_status *st);

/* An acceptance callback which accepts all responses.  Ignores
 * userdata. */
int ne_accept_always(void *userdata, ne_request *req, const ne_status *st);

/* Callback for reading a block of data.  Returns zero on success, or
 * non-zero on error.  If returning an error, the response will be
 * aborted and the callback will not be invoked again.  The request
 * dispatch (or ne_read_response_block call) will fail with NE_ERROR;
 * the session error string should have been set by the callback. */
typedef int (*ne_block_reader)(void *userdata, const char *buf, size_t len);

/* Add a response reader for the given request, with the given
 * acceptance function. userdata is passed as the first argument to
 * the acceptance + reader callbacks. 
 *
 * The acceptance callback is called once each time the request is
 * sent: it may be sent >1 time because of authentication retries etc.
 * For each time the acceptance callback is called, if it returns
 * non-zero, blocks of the response body will be passed to the reader
 * callback as the response is read.  After all the response body has
 * been read, the callback will be called with a 'len' argument of
 * zero.  */
void ne_add_response_body_reader(ne_request *req, ne_accept_response accpt,
				 ne_block_reader reader, void *userdata);

/* Retrieve the value of the response header field with given name;
 * returns NULL if no response header with given name was found.  The
 * return value is valid only until the next call to either
 * ne_request_destroy or ne_begin_request for this request. */
const char *ne_get_response_header(ne_request *req, const char *name);

/* Iterator interface for response headers: if passed a NULL cursor,
 * returns the first header; if passed a non-NULL cursor pointer,
 * returns the next header.  The return value is a cursor pointer: if
 * it is non-NULL, *name and *value are set to the name and value of
 * the header field.  If the return value is NULL, no more headers are
 * found, *name and *value are undefined.
 *
 * The order in which response headers is returned is undefined.  Both
 * the cursor and name/value pointers are valid only until the next
 * call to either ne_request_destroy or ne_begin_request for this
 * request. */
void *ne_response_header_iterate(ne_request *req, void *cursor,
                                 const char **name, const char **value);

/* Adds a header to the request with given name and value. */
void ne_add_request_header(ne_request *req, const char *name, 
			   const char *value);
/* Adds a header to the request with given name, using printf-like
 * format arguments for the value. */
void ne_print_request_header(ne_request *req, const char *name,
			     const char *format, ...) 
    ne_attribute((format(printf, 3, 4)));

/* ne_request_dispatch: Sends the given request, and reads the
 * response.  Returns:
 *  - NE_OK if the request was sent and response read successfully
 *  - NE_AUTH, NE_PROXYAUTH for a server or proxy server authentication error
 *  - NE_CONNECT if connection could not be established
 *  - NE_TIMEOUT if an timeout occurred sending or reading from the server
 *  - NE_ERROR for other fatal dispatch errors
 * On any error, the session error string is set.  On success or
 * authentication error, the actual response-status can be retrieved using
 * ne_get_status(). */
int ne_request_dispatch(ne_request *req);

/* Returns a pointer to the response status information for the given
 * request; pointer is valid until request object is destroyed. */
const ne_status *ne_get_status(const ne_request *req) ne_attribute((const));

/* Returns pointer to session associated with request. */
ne_session *ne_get_session(const ne_request *req) ne_attribute((const));

/* Destroy memory associated with request pointer */
void ne_request_destroy(ne_request *req);

/* "Caller-pulls" request interface.  This is an ALTERNATIVE interface
 * to ne_request_dispatch: either use that, or do all this yourself:
 *
 * caller must call:
 *  1. ne_begin_request (fail if returns non-NE_OK)
 *  2. while(ne_read_response_block(...) > 0) ... loop ...;
 *     (fail if ne_read_response_block returns <0)
 *  3. ne_end_request
 *
 * ne_end_request and ne_begin_request both return an NE_* code; if
 * ne_end_request returns NE_RETRY, you must restart the loop from (1)
 * above. */
int ne_begin_request(ne_request *req);
int ne_end_request(ne_request *req);

/* Read a block of the response into the passed buffer of size 'buflen'.
 *
 * Returns:
 *  <0 - error, stop reading.
 *   0 - end of response
 *  >0 - number of bytes read into buffer.
 */
ssize_t ne_read_response_block(ne_request *req, char *buffer, size_t buflen);

/* Read response blocks until end of response; exactly equivalent to
 * calling ne_read_response_block() until it returns 0.  Returns
 * non-zero on error. */
int ne_discard_response(ne_request *req);

/* Read response blocks until end of response, writing content to the
 * given file descriptor.  Returns NE_ERROR on error. */
int ne_read_response_to_fd(ne_request *req, int fd);

/* Defined request flags: */
typedef enum ne_request_flag_e {
    NE_REQFLAG_EXPECT100 = 0, /* enable this flag to enable use of the
                               * "Expect: 100-continue" for the
                               * request. */

    NE_REQFLAG_IDEMPOTENT, /* disable this flag if the request uses a
                            * non-idempotent method such as POST. */

    NE_REQFLAG_LAST /* enum sentinel value */
} ne_request_flag;

/* Set a new value for a particular request flag. */
void ne_set_request_flag(ne_request *req, ne_request_flag flag, int value);

/* Return 0 if the given flag is not set, >0 it is set, or -1 if the
 * flag is not supported. */
int ne_get_request_flag(ne_request *req, ne_request_flag flag);

/**** Request hooks handling *****/

typedef void (*ne_free_hooks)(void *cookie);

/* Hook called when a request is created; passed the request method,
 * and the string used as the Request-URI (note that this may be a
 * absolute URI if a proxy is in use, an absolute path, a "*", etc).
 * A create_request hook is called exactly once per request. */
typedef void (*ne_create_request_fn)(ne_request *req, void *userdata,
				     const char *method, const char *requri);
void ne_hook_create_request(ne_session *sess, 
			    ne_create_request_fn fn, void *userdata);

/* Hook called before the request is sent.  'header' is the raw HTTP
 * header before the trailing CRLF is added; more headers can be added
 * here.  A pre_send hook may be called >1 time per request if the
 * request is retried due to a post_send hook returning NE_RETRY. */
typedef void (*ne_pre_send_fn)(ne_request *req, void *userdata, 
			       ne_buffer *header);
void ne_hook_pre_send(ne_session *sess, ne_pre_send_fn fn, void *userdata);

/* Hook called directly after the response headers have been read, but
 * before the resposnse body has been read.  'status' is the response
 * status-code.  A post_header hook may be called >1 time per request
 * if the request is retried due to a post_send hook returning
 * NE_RETRY. */
typedef void (*ne_post_headers_fn)(ne_request *req, void *userdata,
                                   const ne_status *status);
void ne_hook_post_headers(ne_session *sess, 
                          ne_post_headers_fn fn, void *userdata);

/* Hook called after the request is dispatched (request sent, and
 * the entire response read).  If an error occurred reading the response,
 * this hook will not run.  May return:
 *  NE_OK     everything is okay
 *  NE_RETRY  try sending the request again.
 * anything else signifies an error, and the request is failed. The return
 * code is passed back the _dispatch caller, so the session error must
 * also be set appropriately (ne_set_error).
 */
typedef int (*ne_post_send_fn)(ne_request *req, void *userdata,
			       const ne_status *status);
void ne_hook_post_send(ne_session *sess, ne_post_send_fn fn, void *userdata);

/* Hook called when the function is destroyed. */
typedef void (*ne_destroy_req_fn)(ne_request *req, void *userdata);
void ne_hook_destroy_request(ne_session *sess,
			     ne_destroy_req_fn fn, void *userdata);

typedef void (*ne_destroy_sess_fn)(void *userdata);
/* Hook called when the session is about to be destroyed. */
void ne_hook_destroy_session(ne_session *sess,
			     ne_destroy_sess_fn fn, void *userdata);

typedef void (*ne_close_conn_fn)(void *userdata);
/* Hook called when the connection is closed; note that this hook
 * may be called *AFTER* the destroy_session hook. */
void ne_hook_close_conn(ne_session *sess, ne_close_conn_fn fn, void *userdata);

/* The ne_unhook_* functions remove a hook registered with the given
 * session.  If a hook is found which was registered with a given
 * function 'fn', and userdata pointer 'userdata', then it will be
 * removed from the hooks list.
 *
 * It is unsafe to use any of these functions from a hook function to
 * unregister itself, except for ne_unhook_destroy_request. */
void ne_unhook_create_request(ne_session *sess, 
                              ne_create_request_fn fn, void *userdata);
void ne_unhook_pre_send(ne_session *sess, ne_pre_send_fn fn, void *userdata);
void ne_unhook_post_headers(ne_session *sess, ne_post_headers_fn fn, void *userdata);
void ne_unhook_post_send(ne_session *sess, ne_post_send_fn fn, void *userdata);
void ne_unhook_destroy_request(ne_session *sess,
                               ne_destroy_req_fn fn, void *userdata);
void ne_unhook_destroy_session(ne_session *sess,
                               ne_destroy_sess_fn fn, void *userdata);
void ne_unhook_close_conn(ne_session *sess, 
                          ne_close_conn_fn fn, void *userdata);

/* Store an opaque context for the request, 'priv' is returned by a
 * call to ne_request_get_private with the same ID. */
void ne_set_request_private(ne_request *req, const char *id, void *priv);
void *ne_get_request_private(ne_request *req, const char *id);

NE_END_DECLS

#endif /* NE_REQUEST_H */
