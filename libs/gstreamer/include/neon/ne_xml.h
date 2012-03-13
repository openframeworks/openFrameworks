/* 
   neon XML parser interface
   Copyright (C) 1999-2007, Joe Orton <joe@manyfish.co.uk>

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

#ifndef NE_XML_H
#define NE_XML_H

#include <sys/types.h> /* for size_t */

#include "ne_defs.h"

NE_BEGIN_DECLS

/* The neon XML interface filters a streamed XML tree through a stack
 * of SAX "handlers".  A handler is made up of three callbacks
 * (start-element, char-data, end-element).  Each start-element event
 * is passed to each handler in the stack in turn until one until one
 * accepts the element.  This handler then receives subsequent
 * char-data and end-element events for the element.
 *
 * For each new start-element event, the search up the handler stack
 * begins with the handler for the parent element (for the root
 * element, at the base of the stack).
 *
 * For each accepted element, a "state" integer is stored, which is
 * passed to the corresponding char-data and end-element callbacks for
 * the element.  This integer is also passed to the start-element
 * callback of child elements so they can determine context.
 *
 * If no handler in the stack accepts a particular element, it (and
 * its children, if any) is ignored. */

#define NE_XML_DECLINE (0)
#define NE_XML_ABORT (-1)

/* A start-element callback for element with given namespace/name.
 * The callback may return:
 *   <0  =>  abort the parse (NE_XML_ABORT)
 *    0  =>  decline this element (NE_XML_DECLINE)
 *   >0  =>  accept this element; value is state for this element.
 *
 * The 'parent' integer is the state returned by the handler of the 
 * parent element.   The attributes array gives name/value pairs
 * in atts[n] and atts[n+1] from n=0 up to atts[n]==NULL. */
typedef int ne_xml_startelm_cb(void *userdata, int parent,
                               const char *nspace, const char *name,
                               const char **atts);

/* state for the root element */
#define NE_XML_STATEROOT (0)

/* Character data callback; may return non-zero to abort the parse. */
typedef int ne_xml_cdata_cb(void *userdata, int state,
                            const char *cdata, size_t len);
/* End element callback; may return non-zero to abort the parse. */
typedef int ne_xml_endelm_cb(void *userdata, int state, 
                             const char *nspace, const char *name);

typedef struct ne_xml_parser_s ne_xml_parser;

/* Create an XML parser. */
ne_xml_parser *ne_xml_create(void);

/* Push a new handler on the stack of parser 'p'. 'cdata' and/or
 * 'endelm' may be NULL; startelm must be non-NULL. */
void ne_xml_push_handler(ne_xml_parser *p,
                         ne_xml_startelm_cb *startelm, 
                         ne_xml_cdata_cb *cdata,
                         ne_xml_endelm_cb *endelm,
                         void *userdata);

/* ne_xml_failed returns non-zero if there was an error during
 * parsing, or zero if the parse completed successfully.  The return
 * value is equal to that of the last ne_xml_parse() call for this
 * parser object. */
int ne_xml_failed(ne_xml_parser *p);

/* Set error string for parser.  (The string may be truncated
 * internally). */
void ne_xml_set_error(ne_xml_parser *p, const char *msg);

/* Return the error string (and never NULL).  After ne_xml_failed
 * returns >0, this will describe the parse error.  Otherwise it will
 * be a default error string. */
const char *ne_xml_get_error(ne_xml_parser *p);

/* Parse the given block of input of length len.  Parser must be
 * called with len=0 to signify the end of the document (for that
 * case, the block argument is ignored).  Returns zero on success, or
 * non-zero on error: for an XML syntax error, a positive number is
 * returned; if parsing is aborted by a caller-supplied callback, that
 * callback's return value is returned. */
int ne_xml_parse(ne_xml_parser *p, const char *block, size_t len);

/* As ne_xml_parse, casting (ne_xml_parser *)userdata internally.
 * (This function can be passed to ne_add_response_body_reader) */
int ne_xml_parse_v(void *userdata, const char *block, size_t len);

/* Return current line of document during parsing or after parsing is
 * complete. */
int ne_xml_currentline(ne_xml_parser *p);

/* From a start_element callback which was passed 'attrs' using given
 * parser, return attribute of given name and namespace.  If nspace is
 * NULL, no namespace resolution is performed.  Note that this call is
 * context-specific; if called outside a start_element callback,
 * behaviour is undefined. */
const char *ne_xml_get_attr(ne_xml_parser *parser,
			    const char **attrs, const char *nspace, 
			    const char *name);

/* From a start_element callback, resolve a given XML Namespace
 * prefix, if defined.  Given a non-NULL prefix, returns the namespace
 * URI which corresponds to the prefix 'prefix' (of length 'length'),
 * or NULL if no such namespace prefix is defined.  Given a NULL
 * prefix, returns the default namespace URI or the empty string if
 * none is defined.  Note that this call is context-specific; if
 * called outside a start_element callback, behaviour is undefined. */
const char *ne_xml_resolve_nspace(ne_xml_parser *parser, 
                                  const char *prefix, size_t length);

/* Return the encoding of the document being parsed.  May return NULL
 * if no encoding is defined or if the XML declaration has not yet
 * been parsed. */
const char *ne_xml_doc_encoding(const ne_xml_parser *p);

/* Destroy the parser object. */
void ne_xml_destroy(ne_xml_parser *p);

/* A utility interface for mapping {nspace, name} onto an integer. */
struct ne_xml_idmap {
    const char *nspace, *name;
    int id;
};

/* Return the size of an idmap array */
#define NE_XML_MAPLEN(map) (sizeof(map) / sizeof(struct ne_xml_idmap))

/* Return the 'id' corresponding to {nspace, name}, or zero. */
int ne_xml_mapid(const struct ne_xml_idmap map[], size_t maplen,
                 const char *nspace, const char *name);

/* media type, appropriate for adding to a Content-Type header */
#define NE_XML_MEDIA_TYPE "application/xml"

NE_END_DECLS

#endif /* NE_XML_H */
