/* 
   WebDAV Class 2 locking operations
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

#ifndef NE_LOCKS_H
#define NE_LOCKS_H

#include "ne_request.h" /* for ne_session + ne_request */
#include "ne_uri.h" /* for ne_uri */

NE_BEGIN_DECLS

/* The scope of a lock */
enum ne_lock_scope {
    ne_lockscope_exclusive,
    ne_lockscope_shared
};

/* Lock type. Only write locks are defined in RFC2518. */
enum ne_lock_type {
    ne_locktype_write
};

/* A lock object. */
struct ne_lock {
    ne_uri uri;
    int depth; /* the depth of the lock (NE_DEPTH_*). */
    enum ne_lock_type type;
    enum ne_lock_scope scope;
    char *token; /* the lock token: uniquely identifies this lock. */
    char *owner; /* string describing the owner of the lock. */
    long timeout; /* timeout in seconds. (or NE_TIMEOUT_*) */
};
/* NB: struct ne_lock Would be typedef'ed to ne_lock except lock is
 * a verb and a noun, so we already have ne_lock the function. Damn
 * the English language. */

#define NE_TIMEOUT_INFINITE -1
#define NE_TIMEOUT_INVALID -2

/* Create a depth zero, exclusive write lock, with default timeout
 * (allowing a server to pick a default).  token, owner and uri are
 * unset. */
struct ne_lock *ne_lock_create(void);

/* HINT: to initialize uri host/port/scheme for the lock's URI, use
 * ne_fill_server_uri from ne_session.h. */

/* Deep-copy a lock structure: strdup's any of path, token, owner,
 * hostport which are set. */
struct ne_lock *ne_lock_copy(const struct ne_lock *lock);

/* Free a lock structure; free's any of any of the URI, token and
 * owner which are set, but not the lock object itself. */
void ne_lock_free(struct ne_lock *lock);

/* Like ne_lock_free; but free's the lock object itself too. */
void ne_lock_destroy(struct ne_lock *lock);

/* ne_lock_store: an opaque type which is used to store a set of lock
 * objects. */
typedef struct ne_lock_store_s ne_lock_store;

/* Create a lock store. */
ne_lock_store *ne_lockstore_create(void);

/* Register the lock store 'store' with the HTTP session 'sess': any
 * operations made using 'sess' which operate on a locked resource,
 * can use the locks from 'store' if needed. */
void ne_lockstore_register(ne_lock_store *store, ne_session *sess);

/* Destroy a lock store, free'ing any locks remaining inside. */
void ne_lockstore_destroy(ne_lock_store *store);

/* Add a lock to the store: the store then "owns" the lock object, and
 * you must not free it. The lock MUST have all of:
 *  - a completed URI structure: scheme, host, port, and path all set
 *  - a valid lock token
 *  - a valid depth
 */
void ne_lockstore_add(ne_lock_store *store, struct ne_lock *lock);

/* Remove given lock object from store: 'lock' MUST point to a lock
 * object which is known to be in the store. */
void ne_lockstore_remove(ne_lock_store *store, struct ne_lock *lock);

/* Returns the first lock in the lock store, or NULL if the store is
 * empty. */
struct ne_lock *ne_lockstore_first(ne_lock_store *store);

/* After ne_lockstore_first has been called; returns the next lock in
 * the lock store, or NULL if there are no more locks stored.
 * Behaviour is undefined if ne_lockstore_first has not been called on
 * 'store' since the store was created, or the last time this function
 * returned NULL for the store.. */
struct ne_lock *ne_lockstore_next(ne_lock_store *store);

/* Find a lock in the store for the given server, and with the given
 * path. */
struct ne_lock *ne_lockstore_findbyuri(ne_lock_store *store, 
				       const ne_uri *uri);

/* Issue a LOCK request for the given lock.  Requires that the uri,
 * depth, type, scope, and timeout members of 'lock' are filled in.
 * owner and token must be malloc-allocated if not NULL; and may be
 * free()d by this function.  On successful return, lock->token will
 * contain the lock token. */
int ne_lock(ne_session *sess, struct ne_lock *lock);

/* Issue an UNLOCK request for the given lock */
int ne_unlock(ne_session *sess, const struct ne_lock *lock);

/* Refresh a lock. Updates lock->timeout appropriately. */
int ne_lock_refresh(ne_session *sess, struct ne_lock *lock);

/* Callback for lock discovery.  If 'lock' is NULL, something went
 * wrong performing lockdiscovery for the resource, look at 'status'
 * for the details.
 * 
 * If lock is non-NULL, at least lock->uri and lock->token will be
 * filled in; and status will be NULL. */
typedef void (*ne_lock_result)(void *userdata, const struct ne_lock *lock, 
			       const ne_uri *uri, const ne_status *status);

/* Perform lock discovery on the given path.  'result' is called with
 * the results (possibly >1 times).  */
int ne_lock_discover(ne_session *sess, const char *path,
		     ne_lock_result result, void *userdata);

/* The ne_lock_using_* functions should be used before dispatching a
 * request which modify resources.  If a lock store has been
 * registered with the session associated with the request, and locks
 * are present in the lock store which cover the resources which are
 * being modified by the request, then the appropriate lock tokens are
 * submitted in the request headers. */

/* Indicate that request 'req' will modify the resource at 'path', and
 * is an operation of given 'depth'. */
void ne_lock_using_resource(ne_request *req, const char *path, int depth);

/* Indicate that request 'req' will modify the parent collection of
 * the resource found at 'path' (for instance when removing the
 * resource from the collection). */
void ne_lock_using_parent(ne_request *req, const char *path);

NE_END_DECLS

#endif /* NE_LOCKS_H */
