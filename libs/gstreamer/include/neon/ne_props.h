/* 
   WebDAV Properties manipulation
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

#ifndef NE_PROPS_H
#define NE_PROPS_H

#include "ne_request.h"
#include "ne_207.h"

NE_BEGIN_DECLS

/* There are two interfaces for fetching properties. The first is
 * 'ne_simple_propfind', which is relatively simple, and easy to use,
 * but only lets you fetch FLAT properties, i.e. properties which are
 * just a string of bytes.  The complex interface is 'ne_propfind_*',
 * which is complicated, and hard to use, but lets you parse
 * structured properties, i.e.  properties which have XML content.  */

/* The 'ne_simple_propfind' interface. ***
 *
 * ne_simple_propfind allows you to fetch a set of properties for a
 * single resource, or a tree of resources.  You set the operation
 * going by passing these arguments:
 *
 *  - the session which should be used.
 *  - the URI and the depth of the operation (0, 1, infinite)
 *  - the names of the properties which you want to fetch
 *  - a results callback, and the userdata for the callback.
 *
 * For each resource found, the results callback is called, passing
 * you two things along with the userdata you passed in originally:
 *
 *   - the URI of the resource (const ne_uri *uri)
 *   - the properties results set (const ne_prop_result_set *results)
 * */

/* The name of a WebDAV property. 'nspace' may be NULL. */
typedef struct {
    const char *nspace, *name;
} ne_propname;

typedef struct ne_prop_result_set_s ne_prop_result_set;

/* Get the value of a given property. Will return NULL if there was an
 * error fetching this property on this resource.  Call
 * ne_propset_result to get the response-status if so.  */
const char *ne_propset_value(const ne_prop_result_set *set,
			      const ne_propname *propname);

/* Returns the status structure for fetching the given property on
 * this resource. This function will return NULL if the server did not
 * return the property (which is a server error). */
const ne_status *ne_propset_status(const ne_prop_result_set *set,
				      const ne_propname *propname);

/* Returns the private pointer for the given propset. */
void *ne_propset_private(const ne_prop_result_set *set);

/* Return language string of property (may be NULL). */
const char *ne_propset_lang(const ne_prop_result_set *set,
			     const ne_propname *pname);

/* ne_propset_iterate iterates over a properties result set,
 * calling the callback for each property in the set. userdata is
 * passed as the first argument to the callback. value may be NULL,
 * indicating an error occurred fetching this property: look at 
 * status for the error in that case.
 *
 * If the iterator returns non-zero, ne_propset_iterate will return
 * immediately with that value.
 */
typedef int (*ne_propset_iterator)(void *userdata,
				    const ne_propname *pname,
				    const char *value,
				    const ne_status *status);

/* Iterate over all the properties in 'set', calling 'iterator'
 * for each, passing 'userdata' as the first argument to callback.
 * 
 * Returns:
 *   whatever value iterator returns.
 */
int ne_propset_iterate(const ne_prop_result_set *set,
			ne_propset_iterator iterator, void *userdata);

/* Callback for handling the results of fetching properties for a
 * single resource (identified by URI 'uri').  The results are stored
 * in the result set 'results': use ne_propset_* to examine this
 * object.  */
typedef void (*ne_props_result)(void *userdata, const ne_uri *uri,
                                const ne_prop_result_set *results);

/* Fetch properties for a resource (if depth == NE_DEPTH_ZERO),
 * or a tree of resources (if depth == NE_DEPTH_ONE or _INFINITE).
 *
 * Names of the properties required must be given in 'props',
 * or if props is NULL, *all* properties are fetched.
 *
 * 'results' is called for each resource in the response, userdata is
 * passed as the first argument to the callback. It is important to
 * note that the callback is called as the response is read off the
 * socket, so don't do anything silly in it (e.g. sleep(100), or call
 * any functions which use this session).
 *
 * Note that if 'depth' is NE_DEPTH_INFINITY, some servers may refuse
 * the request.
 *
 * Returns NE_*.  */
int ne_simple_propfind(ne_session *sess, const char *path, int depth,
			const ne_propname *props,
			ne_props_result results, void *userdata);

/* The properties of a resource can be manipulated using ne_proppatch.
 * A single proppatch request may include any number of individual
 * "set" and "remove" operations, and is defined to have
 * "all-or-nothing" semantics, so either all the operations succeed,
 * or none do. */

/* A proppatch operation may either set a property to have a new
 * value, in which case 'type' must be ne_propset, and 'value' must be
 * non-NULL; or it can remove a property; in which case 'type' must be
 * ne_propremove, and 'value' is ignored.  In both cases, 'name' must
 * be set to the name of the property to alter. */
enum ne_proppatch_optype {
    ne_propset,
    ne_propremove
};
typedef struct {
    const ne_propname *name;
    enum ne_proppatch_optype type;
    const char *value;
} ne_proppatch_operation;

/* Execute a set of property operations 'ops' on 'path'. 'ops' is an
 * array terminated by an operation with a NULL 'name' field. Returns
 * NE_*. */
int ne_proppatch(ne_session *sess, const char *path,
		 const ne_proppatch_operation *ops);

/* Retrieve property names for the resources at 'path'.  'results'
 * callback is called for each resource.  Use 'ne_propset_iterate' on
 * the passed results object to retrieve the list of property names.
 * */
int ne_propnames(ne_session *sess, const char *path, int depth,
		 ne_props_result results, void *userdata);

/* The complex, you-do-all-the-work, property fetch interface:
 */

struct ne_propfind_handler_s;
typedef struct ne_propfind_handler_s ne_propfind_handler;

/* Retrieve the 'private' pointer for the current propset for the
 * given handler, as returned by the ne_props_create_complex callback
 * installed using 'ne_propfind_set_private'.  If this callback was
 * not registered, this function will return NULL.  */
void *ne_propfind_current_private(ne_propfind_handler *handler);

/* Create a PROPFIND handler, for the given resource or set of 
 * resources.
 *
 * Depth must be one of NE_DEPTH_*. */
ne_propfind_handler *
ne_propfind_create(ne_session *sess, const char *path, int depth);

/* Return the XML parser for the given handler (only need if you want
 * to handle complex properties). */
ne_xml_parser *ne_propfind_get_parser(ne_propfind_handler *handler);

/* This interface reserves the state integer range 'x' where 0 < x
 * and x < NE_PROPS_STATE_TOP. */
#define NE_PROPS_STATE_TOP (NE_207_STATE_TOP + 100)

/* Return the request object for the given handler.  You MUST NOT use
 * ne_set_request_body_* on this request object.  (this call is only
 * needed if for instance, you want to add extra headers to the
 * PROPFIND request).  The result of using the request pointer after
 * ne_propfind_destroy(handler) has been called is undefined. */
ne_request *ne_propfind_get_request(ne_propfind_handler *handler);

/* A "complex property" has a value which is structured XML. To handle
 * complex properties, you must set up and register an XML handler
 * which will understand the elements which make up such properties.
 * The handler must be registered with the parser returned by
 * 'ne_propfind_get_parser'.
 *
 * To store the parsed value of the property, a 'private' structure is
 * allocated in each propset (i.e. one per resource). When parsing the
 * property value elements, for each new resource encountered in the
 * response, the 'creator' callback is called to retrieve a 'private'
 * structure for this resource.  When the private structure is no longer
 * needed, the 'destructor' callback is called to deallocate any 
 * memory, if necessary.
 *
 * Whilst in XML element callbacks you will have registered to handle
 * complex properties, you can use the 'ne_propfind_current_private'
 * call to retrieve the pointer to this private structure.
 *
 * To retrieve this 'private' structure from the propset in the
 * results callback, simply call 'ne_propset_private'.
 * */
typedef void *(*ne_props_create_complex)(void *userdata, const ne_uri *uri);
typedef void (*ne_props_destroy_complex)(void *userdata, void *complex);

void ne_propfind_set_private(ne_propfind_handler *handler,
			     ne_props_create_complex creator,
			     ne_props_destroy_complex destructor,
			     void *userdata);

/* Fetch all properties.
 *
 * Returns NE_*. */
int ne_propfind_allprop(ne_propfind_handler *handler, 
			ne_props_result result, void *userdata);

/* Fetch all properties with names listed in array 'names', which is
 * terminated by a property with a NULL name field.  For each resource
 * encountered, the result callback will be invoked, passing in
 * 'userdata' as the first argument.
 *
 * Returns NE_*. */
int ne_propfind_named(ne_propfind_handler *handler, 
		      const ne_propname *names,
		      ne_props_result result, void *userdata);

/* Destroy a propfind handler after use. */
void ne_propfind_destroy(ne_propfind_handler *handler);

NE_END_DECLS

#endif /* NE_PROPS_H */
