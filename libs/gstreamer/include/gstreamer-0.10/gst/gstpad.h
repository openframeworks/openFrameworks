/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wim.taymans@chello.be>
 *
 * gstpad.h: Header for GstPad object
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef __GST_PAD_H__
#define __GST_PAD_H__

#include <gst/gstconfig.h>

#include <gst/gstobject.h>
#include <gst/gstbuffer.h>
#include <gst/gstbufferlist.h>
#include <gst/gstcaps.h>
#include <gst/gstevent.h>
#include <gst/gstquery.h>
#include <gst/gsttask.h>

G_BEGIN_DECLS

/*
 * Pad base class
 */
#define GST_TYPE_PAD			(gst_pad_get_type ())
#define GST_IS_PAD(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_PAD))
#define GST_IS_PAD_CLASS(klass)		(G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_PAD))
#define GST_PAD(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_PAD, GstPad))
#define GST_PAD_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_PAD, GstPadClass))
#define GST_PAD_CAST(obj)		((GstPad*)(obj))


typedef struct _GstPad GstPad;
typedef struct _GstPadPrivate GstPadPrivate;
typedef struct _GstPadClass GstPadClass;

/**
 * GstPadLinkReturn:
 * @GST_PAD_LINK_OK		: link succeeded
 * @GST_PAD_LINK_WRONG_HIERARCHY: pads have no common grandparent
 * @GST_PAD_LINK_WAS_LINKED	: pad was already linked
 * @GST_PAD_LINK_WRONG_DIRECTION: pads have wrong direction
 * @GST_PAD_LINK_NOFORMAT	: pads do not have common format
 * @GST_PAD_LINK_NOSCHED	: pads cannot cooperate in scheduling
 * @GST_PAD_LINK_REFUSED	: refused for some reason
 *
 * Result values from gst_pad_link and friends.
 */
typedef enum {
  GST_PAD_LINK_OK               =  0,
  GST_PAD_LINK_WRONG_HIERARCHY  = -1,
  GST_PAD_LINK_WAS_LINKED       = -2,
  GST_PAD_LINK_WRONG_DIRECTION  = -3,
  GST_PAD_LINK_NOFORMAT         = -4,
  GST_PAD_LINK_NOSCHED          = -5,
  GST_PAD_LINK_REFUSED          = -6
} GstPadLinkReturn;

/**
 * GST_PAD_LINK_FAILED:
 * @ret: the #GstPadLinkReturn value
 *
 * Macro to test if the given #GstPadLinkReturn value indicates a failed
 * link step.
 */
#define GST_PAD_LINK_FAILED(ret) ((ret) < GST_PAD_LINK_OK)

/**
 * GST_PAD_LINK_SUCCESSFUL:
 * @ret: the #GstPadLinkReturn value
 *
 * Macro to test if the given #GstPadLinkReturn value indicates a successful
 * link step.
 */
#define GST_PAD_LINK_SUCCESSFUL(ret) ((ret) >= GST_PAD_LINK_OK)

/**
 * GstFlowReturn:
 * @GST_FLOW_RESEND:		 Resend buffer, possibly with new caps (not
 *                                 sent yet) (unused/unimplemented).
 * @GST_FLOW_OK:		 Data passing was ok.
 * @GST_FLOW_NOT_LINKED:	 Pad is not linked.
 * @GST_FLOW_WRONG_STATE:	 Pad is in wrong state.
 * @GST_FLOW_UNEXPECTED:	 Did not expect anything, like after EOS.
 * @GST_FLOW_NOT_NEGOTIATED:	 Pad is not negotiated.
 * @GST_FLOW_ERROR:		 Some (fatal) error occured. Element generating
 *                               this error should post an error message with more
 *                               details.
 * @GST_FLOW_NOT_SUPPORTED:	 This operation is not supported.
 * @GST_FLOW_CUSTOM_SUCCESS:	 Elements can use values starting from
 *                               this (and higher) to define custom success
 *                               codes. Since 0.10.7.
 * @GST_FLOW_CUSTOM_SUCCESS_1:	 Pre-defined custom success code (define your
 *                               custom success code to this to avoid compiler
 *                               warnings). Since 0.10.29.
 * @GST_FLOW_CUSTOM_SUCCESS_2:	 Pre-defined custom success code. Since 0.10.29.
 * @GST_FLOW_CUSTOM_ERROR:	 Elements can use values starting from
 *                               this (and lower) to define custom error codes.
 *                               Since 0.10.7.
 * @GST_FLOW_CUSTOM_ERROR_1:	 Pre-defined custom error code (define your
 *                               custom error code to this to avoid compiler
 *                               warnings). Since 0.10.29.
 * @GST_FLOW_CUSTOM_ERROR_2:	 Pre-defined custom error code. Since 0.10.29.
 *
 * The result of passing data to a pad.
 *
 * Note that the custom return values should not be exposed outside of the
 * element scope and are available since 0.10.7.
 */
/* FIXME 0.11: remove custom flow returns */
typedef enum {
  /* custom success starts here */
  GST_FLOW_CUSTOM_SUCCESS_2 = 102,
  GST_FLOW_CUSTOM_SUCCESS_1 = 101,
  GST_FLOW_CUSTOM_SUCCESS = 100,

  /* core predefined */
  GST_FLOW_RESEND	  =  1,
  GST_FLOW_OK		  =  0,
  /* expected failures */
  GST_FLOW_NOT_LINKED     = -1,
  GST_FLOW_WRONG_STATE    = -2,
  /* error cases */
  GST_FLOW_UNEXPECTED     = -3,
  GST_FLOW_NOT_NEGOTIATED = -4,
  GST_FLOW_ERROR	  = -5,
  GST_FLOW_NOT_SUPPORTED  = -6,

  /* custom error starts here */
  GST_FLOW_CUSTOM_ERROR   = -100,
  GST_FLOW_CUSTOM_ERROR_1 = -101,
  GST_FLOW_CUSTOM_ERROR_2 = -102
} GstFlowReturn;

/**
 * GST_FLOW_IS_FATAL:
 * @ret: a #GstFlowReturn value
 *
 * Macro to test if the given #GstFlowReturn value indicates a fatal
 * error. This macro is mainly used in elements driving the pipeline to decide
 * whether an error message should be posted on the bus. Note that such
 * elements may also need to post an error message in the #GST_FLOW_NOT_LINKED
 * case which is not caught by this macro.
 *
 * Deprecated: This macro is badly named and can't be used in any real
 * scenarios without additional checks.
 */
#ifndef GST_DISABLE_DEPRECATED
#define GST_FLOW_IS_FATAL(ret) ((ret) <= GST_FLOW_UNEXPECTED)
#endif

/**
 * GST_FLOW_IS_SUCCESS:
 * @ret: a #GstFlowReturn value
 *
 * Macro to test if the given #GstFlowReturn value indicates a
 * successfull result
 * This macro is mainly used in elements to decide if the processing
 * of a buffer was successfull.
 *
 * Since: 0.10.7
 *
 * Deprecated: This macro is badly named and can't be used in any real
 * scenarios without additional checks.
 */
#ifndef GST_DISABLE_DEPRECATED
#define GST_FLOW_IS_SUCCESS(ret) ((ret) >= GST_FLOW_OK)
#endif

G_CONST_RETURN gchar*	gst_flow_get_name	(GstFlowReturn ret);
GQuark			gst_flow_to_quark	(GstFlowReturn ret);

/**
 * GstPadLinkCheck:
 * @GST_PAD_LINK_CHECK_NOTHING: Don't check hierarchy or caps compatibility.
 * @GST_PAD_LINK_CHECK_HIERARCHY: Check the pads have same parents/grandparents.
 *   Could be omitted if it is already known that the two elements that own the
 *   pads are in the same bin. 
 * @GST_PAD_LINK_CHECK_TEMPLATE_CAPS: Check if the pads are compatible by using
 *   their template caps. This is much faster than @GST_PAD_LINK_CHECK_CAPS, but
 *   would be unsafe e.g. if one pad has %GST_CAPS_ANY.
 * @GST_PAD_LINK_CHECK_CAPS: Check if the pads are compatible by comparing the
 *   caps returned by gst_pad_get_caps().
 *
 * The amount of checking to be done when linking pads. @GST_PAD_LINK_CHECK_CAPS
 * and @GST_PAD_LINK_CHECK_TEMPLATE_CAPS are mutually exclusive. If both are
 * specified, expensive but safe @GST_PAD_LINK_CHECK_CAPS are performed.
 *
 * <warning><para>
 * Only disable some of the checks if you are 100% certain you know the link
 * will not fail because of hierarchy/caps compatibility failures. If uncertain,
 * use the default checks (%GST_PAD_LINK_CHECK_DEFAULT) or the regular methods
 * for linking the pads.
 * </para></warning>
 *
 * Since: 0.10.30
 */

typedef enum {
  GST_PAD_LINK_CHECK_NOTHING       = 0,
  GST_PAD_LINK_CHECK_HIERARCHY     = 1 << 0,
  GST_PAD_LINK_CHECK_TEMPLATE_CAPS = 1 << 1,
  GST_PAD_LINK_CHECK_CAPS          = 1 << 2
} GstPadLinkCheck;

/**
 * GST_PAD_LINK_CHECK_DEFAULT:
 *
 * The default checks done when linking pads (i.e. the ones used by 
 * gst_pad_link()).
 *
 * Since: 0.10.30
 */
#define GST_PAD_LINK_CHECK_DEFAULT (GST_PAD_LINK_CHECK_HIERARCHY | GST_PAD_LINK_CHECK_CAPS)

/**
 * GstActivateMode:
 * @GST_ACTIVATE_NONE:	  	 Pad will not handle dataflow
 * @GST_ACTIVATE_PUSH:		 Pad handles dataflow in downstream push mode
 * @GST_ACTIVATE_PULL:     	 Pad handles dataflow in upstream pull mode
 *
 * The status of a GstPad. After activating a pad, which usually happens when the
 * parent element goes from READY to PAUSED, the GstActivateMode defines if the
 * pad operates in push or pull mode.
 */
typedef enum {
  GST_ACTIVATE_NONE,
  GST_ACTIVATE_PUSH,
  GST_ACTIVATE_PULL
} GstActivateMode;

/**
 * GST_PAD_MODE_ACTIVATE:
 * @mode: a #GstActivateMode
 *
 * Macro to test if the given #GstActivateMode value indicates that datapassing
 * is possible or not.
 */
#define GST_PAD_MODE_ACTIVATE(mode) ((mode) != GST_ACTIVATE_NONE)

/* pad states */
/**
 * GstPadActivateFunction:
 * @pad: a #GstPad
 *
 * This function is called when the pad is activated during the element
 * READY to PAUSED state change. By default this function will call the
 * activate function that puts the pad in push mode but elements can
 * override this function to activate the pad in pull mode if they wish.
 *
 * Returns: TRUE if the pad could be activated.
 */
typedef gboolean		(*GstPadActivateFunction)	(GstPad *pad);
/**
 * GstPadActivateModeFunction:
 * @pad: a #GstPad
 * @active: activate or deactivate the pad.
 *
 * The prototype of the push and pull activate functions.
 *
 * Returns: TRUE if the pad could be activated or deactivated.
 */
typedef gboolean		(*GstPadActivateModeFunction)	(GstPad *pad, gboolean active);


/* data passing */
/**
 * GstPadChainFunction:
 * @pad: the sink #GstPad that performed the chain.
 * @buffer: the #GstBuffer that is chained, not %NULL.
 *
 * A function that will be called on sinkpads when chaining buffers.
 * The function typically processes the data contained in the buffer and
 * either consumes the data or passes it on to the internally linked pad(s).
 *
 * The implementer of this function receives a refcount to @buffer and should
 * gst_buffer_unref() when the buffer is no longer needed.
 *
 * When a chain function detects an error in the data stream, it must post an
 * error on the bus and return an appropriate #GstFlowReturn value.
 *
 * Returns: #GST_FLOW_OK for success
 */
typedef GstFlowReturn		(*GstPadChainFunction)		(GstPad *pad, GstBuffer *buffer);

/**
 * GstPadChainListFunction:
 * @pad: the sink #GstPad that performed the chain.
 * @list: the #GstBufferList that is chained, not %NULL.
 *
 * A function that will be called on sinkpads when chaining buffer lists.
 * The function typically processes the data contained in the buffer list and
 * either consumes the data or passes it on to the internally linked pad(s).
 *
 * The implementer of this function receives a refcount to @list and
 * should gst_buffer_list_unref() when the list is no longer needed.
 *
 * When a chainlist function detects an error in the data stream, it must
 * post an error on the bus and return an appropriate #GstFlowReturn value.
 *
 * Returns: #GST_FLOW_OK for success
 */
typedef GstFlowReturn		(*GstPadChainListFunction)	(GstPad *pad, GstBufferList *list);

/**
 * GstPadGetRangeFunction:
 * @pad: the src #GstPad to perform the getrange on.
 * @offset: the offset of the range
 * @length: the length of the range
 * @buffer: a memory location to hold the result buffer, cannot be NULL.
 *
 * This function will be called on source pads when a peer element
 * request a buffer at the specified @offset and @length. If this function
 * returns #GST_FLOW_OK, the result buffer will be stored in @buffer. The
 * contents of @buffer is invalid for any other return value.
 *
 * This function is installed on a source pad with
 * gst_pad_set_getrange_function() and can only be called on source pads after
 * they are successfully activated with gst_pad_activate_pull().
 *
 * @offset and @length are always given in byte units. @offset must normally be a value
 * between 0 and the length in bytes of the data available on @pad. The
 * length (duration in bytes) can be retrieved with a #GST_QUERY_DURATION or with a
 * #GST_QUERY_SEEKING.
 *
 * Any @offset larger or equal than the length will make the function return
 * #GST_FLOW_UNEXPECTED, which corresponds to EOS. In this case @buffer does not
 * contain a valid buffer.
 *
 * The buffer size of @buffer will only be smaller than @length when @offset is
 * near the end of the stream. In all other cases, the size of @buffer must be
 * exactly the requested size.
 *
 * It is allowed to call this function with a 0 @length and valid @offset, in
 * which case @buffer will contain a 0-sized buffer and the function returns
 * #GST_FLOW_OK.
 *
 * When this function is called with a -1 @offset, the sequentially next buffer
 * of length @length in the stream is returned.
 *
 * When this function is called with a -1 @length, a buffer with a default
 * optimal length is returned in @buffer. The length might depend on the value
 * of @offset.
 *
 * Returns: #GST_FLOW_OK for success and a valid buffer in @buffer. Any other
 * return value leaves @buffer undefined.
 */
typedef GstFlowReturn		(*GstPadGetRangeFunction)	(GstPad *pad, guint64 offset,
		                                                 guint length, GstBuffer **buffer);

/**
 * GstPadEventFunction:
 * @pad: the #GstPad to handle the event.
 * @event: the #GstEvent to handle.
 *
 * Function signature to handle an event for the pad.
 *
 * Returns: TRUE if the pad could handle the event.
 */
typedef gboolean		(*GstPadEventFunction)		(GstPad *pad, GstEvent *event);


/* FIXME: 0.11: deprecate me, check range should use seeking query */
/**
 * GstPadCheckGetRangeFunction:
 * @pad: a #GstPad
 *
 * Check if @pad can be activated in pull mode.
 *
 * This function will be deprecated after 0.10; use the seeking query to check
 * if a pad can support random access.
 *
 * Returns: TRUE if the pad can operate in pull mode.
 */
typedef gboolean		(*GstPadCheckGetRangeFunction)	(GstPad *pad);

/* internal links */
/**
 * GstPadIntLinkFunction:
 * @pad: The #GstPad to query.
 *
 * The signature of the internal pad link function.
 *
 * Returns: (element-type Gst.Pad) (transfer container): a newly allocated #GList of pads that are linked to the given pad on
 * the inside of the parent element.
 *
 * The caller must call g_list_free() on it after use.
 *
 * Deprecated: use the threadsafe #GstPadIterIntLinkFunction instead.
 */
typedef GList*			(*GstPadIntLinkFunction)	(GstPad *pad);

/**
 * GstPadIterIntLinkFunction:
 * @pad: The #GstPad to query.
 *
 * The signature of the internal pad link iterator function.
 *
 * Returns: a new #GstIterator that will iterate over all pads that are
 * linked to the given pad on the inside of the parent element.
 *
 * the caller must call gst_iterator_free() after usage.
 *
 * Since 0.10.21
 */
typedef GstIterator*           (*GstPadIterIntLinkFunction)    (GstPad *pad);

/* generic query function */
/**
 * GstPadQueryTypeFunction:
 * @pad: a #GstPad to query
 *
 * The signature of the query types function.
 *
 * Returns: a constant array of query types
 */
typedef const GstQueryType*	(*GstPadQueryTypeFunction)	(GstPad *pad);

/**
 * GstPadQueryFunction:
 * @pad: the #GstPad to query.
 * @query: the #GstQuery object to execute
 *
 * The signature of the query function.
 *
 * Returns: TRUE if the query could be performed.
 */
typedef gboolean		(*GstPadQueryFunction)		(GstPad *pad, GstQuery *query);


/* linking */
/**
 * GstPadLinkFunction
 * @pad: the #GstPad that is linked.
 * @peer: the peer #GstPad of the link
 *
 * Function signature to handle a new link on the pad.
 *
 * Returns: the result of the link with the specified peer.
 */
typedef GstPadLinkReturn	(*GstPadLinkFunction)		(GstPad *pad, GstPad *peer);
/**
 * GstPadUnlinkFunction
 * @pad: the #GstPad that is linked.
 *
 * Function signature to handle a unlinking the pad prom its peer.
 */
typedef void			(*GstPadUnlinkFunction)		(GstPad *pad);


/* caps nego */
/**
 * GstPadGetCapsFunction:
 * @pad: the #GstPad to get the capabilities of.
 *
 * Returns a copy of the capabilities of the specified pad. By default this
 * function will return the pad template capabilities, but can optionally
 * be overridden by elements.
 *
 * Returns: a newly allocated copy #GstCaps of the pad.
 */
typedef GstCaps*		(*GstPadGetCapsFunction)	(GstPad *pad);

/**
 * GstPadSetCapsFunction:
 * @pad: the #GstPad to set the capabilities of.
 * @caps: the #GstCaps to set
 *
 * Set @caps on @pad. By default this function updates the caps of the
 * pad but the function can be overriden by elements to perform extra
 * actions or verifications.
 *
 * Returns: TRUE if the caps could be set on the pad.
 */
typedef gboolean		(*GstPadSetCapsFunction)	(GstPad *pad, GstCaps *caps);
/**
 * GstPadAcceptCapsFunction:
 * @pad: the #GstPad to check
 * @caps: the #GstCaps to check
 *
 * Check if @pad can accept @caps. By default this function will see if @caps
 * intersect with the result from gst_pad_get_caps() by can be overridden to
 * perform extra checks.
 *
 * Returns: TRUE if the caps can be accepted by the pad.
 */
typedef gboolean		(*GstPadAcceptCapsFunction)	(GstPad *pad, GstCaps *caps);
/**
 * GstPadFixateCapsFunction:
 * @pad: a #GstPad
 * @caps: the #GstCaps to fixate
 *
 * Given possibly unfixed caps @caps, let @pad use its default prefered
 * format to make a fixed caps. @caps should be writable. By default this
 * function will pick the first value of any ranges or lists in the caps but
 * elements can override this function to perform other behaviour.
 */
typedef void			(*GstPadFixateCapsFunction)	(GstPad *pad, GstCaps *caps);
/**
 * GstPadBufferAllocFunction:
 * @pad: a sink #GstPad
 * @offset: the desired offset of the buffer
 * @size: the desired size of the buffer
 * @caps: the desired caps of the buffer
 * @buf: pointer to hold the allocated buffer.
 *
 * Ask the sinkpad @pad to allocate a buffer with @offset, @size and @caps.
 * The result will be stored in @buf.
 *
 * The purpose of this function is to allocate a buffer that is optimal to
 * be processed by @pad. The function is mostly overridden by elements that can
 * provide a hardware buffer in order to avoid additional memcpy operations.
 *
 * The function can return a buffer that has caps different from the requested
 * @caps, in which case the upstream element requests a format change to this
 * new caps.
 * If a format change was requested, the returned buffer will be one to hold
 * the data of said new caps, so its size might be different from the requested
 * @size.
 *
 * When this function returns anything else than #GST_FLOW_OK, the buffer allocation
 * failed and @buf does not contain valid data. If the function returns #GST_FLOW_OK and
 * the @buf is NULL, a #GstBuffer will be created with @caps, @offset and @size.
 *
 * By default this function returns a new buffer of @size and with @caps containing
 * purely malloced data. The buffer should be freed with gst_buffer_unref()
 * after usage.
 *
 * Returns: #GST_FLOW_OK if @buf contains a valid buffer, any other return
 *  value means @buf does not hold a valid buffer.
 */
typedef GstFlowReturn		(*GstPadBufferAllocFunction)	(GstPad *pad, guint64 offset, guint size,
								 GstCaps *caps, GstBuffer **buf);

/* misc */
/**
 * GstPadDispatcherFunction:
 * @pad: the #GstPad that is dispatched.
 * @data: the gpointer to optional user data.
 *
 * A dispatcher function is called for all internally linked pads, see
 * gst_pad_dispatcher().
 *
 * Returns: TRUE if the dispatching procedure has to be stopped.
 */
typedef gboolean		(*GstPadDispatcherFunction)	(GstPad *pad, gpointer data);

/**
 * GstPadBlockCallback:
 * @pad: the #GstPad that is blockend or unblocked.
 * @blocked: blocking state for the pad
 * @user_data: the gpointer to optional user data.
 *
 * Callback used by gst_pad_set_blocked_async(). Gets called when the blocking
 * operation succeeds.
 */
typedef void			(*GstPadBlockCallback)		(GstPad *pad, gboolean blocked, gpointer user_data);

/**
 * GstPadDirection:
 * @GST_PAD_UNKNOWN: direction is unknown.
 * @GST_PAD_SRC: the pad is a source pad.
 * @GST_PAD_SINK: the pad is a sink pad.
 *
 * The direction of a pad.
 */
typedef enum {
  GST_PAD_UNKNOWN,
  GST_PAD_SRC,
  GST_PAD_SINK
} GstPadDirection;

/**
 * GstPadFlags:
 * @GST_PAD_BLOCKED: is dataflow on a pad blocked
 * @GST_PAD_FLUSHING: is pad refusing buffers
 * @GST_PAD_IN_GETCAPS: GstPadGetCapsFunction() is running now
 * @GST_PAD_IN_SETCAPS: GstPadSetCapsFunction() is running now
 * @GST_PAD_BLOCKING: is pad currently blocking on a buffer or event
 * @GST_PAD_FLAG_LAST: offset to define more flags
 *
 * Pad state flags
 */
typedef enum {
  GST_PAD_BLOCKED       = (GST_OBJECT_FLAG_LAST << 0),
  GST_PAD_FLUSHING      = (GST_OBJECT_FLAG_LAST << 1),
  GST_PAD_IN_GETCAPS    = (GST_OBJECT_FLAG_LAST << 2),
  GST_PAD_IN_SETCAPS    = (GST_OBJECT_FLAG_LAST << 3),
  GST_PAD_BLOCKING	= (GST_OBJECT_FLAG_LAST << 4),
  /* padding */
  GST_PAD_FLAG_LAST     = (GST_OBJECT_FLAG_LAST << 8)
} GstPadFlags;

/* FIXME: this awful circular dependency need to be resolved properly (see padtemplate.h) */
typedef struct _GstPadTemplate GstPadTemplate;

/**
 * GstPad:
 * @element_private: private data owned by the parent element
 * @padtemplate: padtemplate for this pad
 * @direction: the direction of the pad, cannot change after creating
 *             the pad.
 * @stream_rec_lock: recursive stream lock of the pad, used to protect
 *                   the data used in streaming.
 * @task: task for this pad if the pad is actively driving dataflow.
 * @preroll_lock: lock used when prerolling
 * @preroll_cond: conf to signal preroll
 * @block_cond: conditional to signal pad block
 * @block_callback: callback for the pad block if any
 * @block_data: user data for @block_callback
 * @caps: the current caps of the pad
 * @getcapsfunc: function to get caps of the pad
 * @setcapsfunc: function to set caps on the pad
 * @acceptcapsfunc: function to check if pad can accept caps
 * @fixatecapsfunc: function to fixate caps
 * @activatefunc: pad activation function
 * @activatepushfunc: function to activate/deactivate pad in push mode
 * @activatepullfunc: function to activate/deactivate pad in pull mode
 * @linkfunc: function called when pad is linked
 * @unlinkfunc: function called when pad is unlinked
 * @peer: the pad this pad is linked to
 * @sched_private: private storage for the scheduler
 * @chainfunc: function to chain buffer to pad
 * @checkgetrangefunc: function to check if pad can operate in pull mode
 * @getrangefunc: function to get a range of data from a pad
 * @eventfunc: function to send an event to a pad
 * @mode: current activation mode of the pad
 * @querytypefunc: get list of supported queries
 * @queryfunc: perform a query on the pad
 * @intlinkfunc: get the internal links of this pad
 * @bufferallocfunc: function to allocate a buffer for this pad
 * @do_buffer_signals: counter counting installed buffer signals
 * @do_event_signals: counter counting installed event signals
 * @iterintlinkfunc: get the internal links iterator of this pad
 * @block_destroy_data: notify function for gst_pad_set_blocked_async_full()
 *
 * The #GstPad structure. Use the functions to update the variables.
 */
struct _GstPad {
  GstObject			object;

  /*< public >*/
  gpointer			element_private;

  GstPadTemplate		*padtemplate;

  GstPadDirection		 direction;

  /*< public >*/ /* with STREAM_LOCK */
  /* streaming rec_lock */
  GStaticRecMutex		*stream_rec_lock;
  GstTask			*task;
  /*< public >*/ /* with PREROLL_LOCK */
  GMutex			*preroll_lock;
  GCond				*preroll_cond;

  /*< public >*/ /* with LOCK */
  /* block cond, mutex is from the object */
  GCond				*block_cond;
  GstPadBlockCallback		 block_callback;
  gpointer			 block_data;

  /* the pad capabilities */
  GstCaps			*caps;
  GstPadGetCapsFunction		getcapsfunc;
  GstPadSetCapsFunction		setcapsfunc;
  GstPadAcceptCapsFunction	 acceptcapsfunc;
  GstPadFixateCapsFunction	 fixatecapsfunc;

  GstPadActivateFunction	 activatefunc;
  GstPadActivateModeFunction	 activatepushfunc;
  GstPadActivateModeFunction	 activatepullfunc;

  /* pad link */
  GstPadLinkFunction		 linkfunc;
  GstPadUnlinkFunction		 unlinkfunc;
  GstPad			*peer;

  gpointer			 sched_private;

  /* data transport functions */
  GstPadChainFunction		 chainfunc;
  GstPadCheckGetRangeFunction	 checkgetrangefunc;
  GstPadGetRangeFunction	 getrangefunc;
  GstPadEventFunction		 eventfunc;

  GstActivateMode		 mode;

  /* generic query method */
  GstPadQueryTypeFunction	 querytypefunc;
  GstPadQueryFunction		 queryfunc;

  /* internal links */
  GstPadIntLinkFunction		 intlinkfunc;

  GstPadBufferAllocFunction      bufferallocfunc;

  /* whether to emit signals for have-data. counts number
   * of handlers attached. */
  gint				 do_buffer_signals;
  gint				 do_event_signals;

  /* ABI added */
  /* iterate internal links */
  GstPadIterIntLinkFunction     iterintlinkfunc;

  /* free block_data */
  GDestroyNotify block_destroy_data;

  /*< private >*/
  union {
    struct {
      gboolean                      block_callback_called;
      GstPadPrivate                *priv;
    } ABI;
    gpointer _gst_reserved[GST_PADDING - 2];
  } abidata;
};

struct _GstPadClass {
  GstObjectClass	parent_class;

  /* signal callbacks */
  void		(*linked)		(GstPad *pad, GstPad *peer);
  void		(*unlinked)		(GstPad *pad, GstPad *peer);
  void		(*request_link)		(GstPad *pad);
  gboolean	(*have_data)		(GstPad *pad, GstMiniObject *data);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};


/***** helper macros *****/
/* GstPad */
#define GST_PAD_NAME(pad)		(GST_OBJECT_NAME(pad))
#define GST_PAD_PARENT(pad)		(GST_ELEMENT_CAST(GST_OBJECT_PARENT(pad)))
#define GST_PAD_ELEMENT_PRIVATE(pad)	(GST_PAD_CAST(pad)->element_private)
#define GST_PAD_PAD_TEMPLATE(pad)	(GST_PAD_CAST(pad)->padtemplate)
#define GST_PAD_DIRECTION(pad)		(GST_PAD_CAST(pad)->direction)
#define GST_PAD_TASK(pad)		(GST_PAD_CAST(pad)->task)
#define GST_PAD_ACTIVATE_MODE(pad)	(GST_PAD_CAST(pad)->mode)

#define GST_PAD_ACTIVATEFUNC(pad)	(GST_PAD_CAST(pad)->activatefunc)
#define GST_PAD_ACTIVATEPUSHFUNC(pad)	(GST_PAD_CAST(pad)->activatepushfunc)
#define GST_PAD_ACTIVATEPULLFUNC(pad)	(GST_PAD_CAST(pad)->activatepullfunc)
#define GST_PAD_CHAINFUNC(pad)		(GST_PAD_CAST(pad)->chainfunc)
#define GST_PAD_CHECKGETRANGEFUNC(pad)	(GST_PAD_CAST(pad)->checkgetrangefunc)
#define GST_PAD_GETRANGEFUNC(pad)	(GST_PAD_CAST(pad)->getrangefunc)
#define GST_PAD_EVENTFUNC(pad)		(GST_PAD_CAST(pad)->eventfunc)
#define GST_PAD_QUERYTYPEFUNC(pad)	(GST_PAD_CAST(pad)->querytypefunc)
#define GST_PAD_QUERYFUNC(pad)		(GST_PAD_CAST(pad)->queryfunc)
#ifndef GST_DISABLE_DEPRECATED
#define GST_PAD_INTLINKFUNC(pad)	(GST_PAD_CAST(pad)->intlinkfunc)
#endif
#define GST_PAD_ITERINTLINKFUNC(pad)    (GST_PAD_CAST(pad)->iterintlinkfunc)

#define GST_PAD_PEER(pad)		(GST_PAD_CAST(pad)->peer)
#define GST_PAD_LINKFUNC(pad)		(GST_PAD_CAST(pad)->linkfunc)
#define GST_PAD_UNLINKFUNC(pad)		(GST_PAD_CAST(pad)->unlinkfunc)

/**
 * GST_PAD_CAPS:
 * @pad: a #GstPad.
 *
 * The caps for this pad.
 */
#define GST_PAD_CAPS(pad)		(GST_PAD_CAST(pad)->caps)
#define GST_PAD_GETCAPSFUNC(pad)	(GST_PAD_CAST(pad)->getcapsfunc)
#define GST_PAD_SETCAPSFUNC(pad)	(GST_PAD_CAST(pad)->setcapsfunc)
#define GST_PAD_ACCEPTCAPSFUNC(pad)	(GST_PAD_CAST(pad)->acceptcapsfunc)
#define GST_PAD_FIXATECAPSFUNC(pad)	(GST_PAD_CAST(pad)->fixatecapsfunc)

#define GST_PAD_BUFFERALLOCFUNC(pad)	(GST_PAD_CAST(pad)->bufferallocfunc)

#define GST_PAD_DO_BUFFER_SIGNALS(pad) 	(GST_PAD_CAST(pad)->do_buffer_signals)
#define GST_PAD_DO_EVENT_SIGNALS(pad) 	(GST_PAD_CAST(pad)->do_event_signals)

#define GST_PAD_IS_LINKED(pad)		(GST_PAD_PEER(pad) != NULL)
#define GST_PAD_IS_BLOCKED(pad)		(GST_OBJECT_FLAG_IS_SET (pad, GST_PAD_BLOCKED))
#define GST_PAD_IS_BLOCKING(pad)	(GST_OBJECT_FLAG_IS_SET (pad, GST_PAD_BLOCKING))
#define GST_PAD_IS_FLUSHING(pad)	(GST_OBJECT_FLAG_IS_SET (pad, GST_PAD_FLUSHING))
#define GST_PAD_IS_IN_GETCAPS(pad)	(GST_OBJECT_FLAG_IS_SET (pad, GST_PAD_IN_GETCAPS))
#define GST_PAD_IS_IN_SETCAPS(pad)	(GST_OBJECT_FLAG_IS_SET (pad, GST_PAD_IN_SETCAPS))
#define GST_PAD_IS_SRC(pad)		(GST_PAD_DIRECTION(pad) == GST_PAD_SRC)
#define GST_PAD_IS_SINK(pad)		(GST_PAD_DIRECTION(pad) == GST_PAD_SINK)

#define GST_PAD_SET_FLUSHING(pad)	(GST_OBJECT_FLAG_SET (pad, GST_PAD_FLUSHING))
#define GST_PAD_UNSET_FLUSHING(pad)	(GST_OBJECT_FLAG_UNSET (pad, GST_PAD_FLUSHING))

/**
 * GST_PAD_GET_STREAM_LOCK:
 * @pad: a #GstPad
 *
 * Get the stream lock of @pad. The stream lock is protecting the
 * resources used in the data processing functions of @pad.
 */
#define GST_PAD_GET_STREAM_LOCK(pad)    (GST_PAD_CAST(pad)->stream_rec_lock)
/**
 * GST_PAD_STREAM_LOCK:
 * @pad: a #GstPad
 *
 * Lock the stream lock of @pad.
 */
#define GST_PAD_STREAM_LOCK(pad)        (g_static_rec_mutex_lock(GST_PAD_GET_STREAM_LOCK(pad)))
/**
 * GST_PAD_STREAM_LOCK_FULL:
 * @pad: a #GstPad
 * @t: the number of times to recursively lock
 *
 * Lock the stream lock of @pad @t times.
 */
#define GST_PAD_STREAM_LOCK_FULL(pad,t) (g_static_rec_mutex_lock_full(GST_PAD_GET_STREAM_LOCK(pad), t))
/**
 * GST_PAD_STREAM_TRYLOCK:
 * @pad: a #GstPad
 *
 * Try to Lock the stream lock of the pad, return TRUE if the lock could be
 * taken.
 */
#define GST_PAD_STREAM_TRYLOCK(pad)     (g_static_rec_mutex_trylock(GST_PAD_GET_STREAM_LOCK(pad)))
/**
 * GST_PAD_STREAM_UNLOCK:
 * @pad: a #GstPad
 *
 * Unlock the stream lock of @pad.
 */
#define GST_PAD_STREAM_UNLOCK(pad)      (g_static_rec_mutex_unlock(GST_PAD_GET_STREAM_LOCK(pad)))
/**
 * GST_PAD_STREAM_UNLOCK_FULL:
 * @pad: a #GstPad
 *
 * Fully unlock the recursive stream lock of @pad, return the number of times
 * @pad was locked.
 */
#define GST_PAD_STREAM_UNLOCK_FULL(pad) (g_static_rec_mutex_unlock_full(GST_PAD_GET_STREAM_LOCK(pad)))

#define GST_PAD_GET_PREROLL_LOCK(pad)   (GST_PAD_CAST(pad)->preroll_lock)
#define GST_PAD_PREROLL_LOCK(pad)       (g_mutex_lock(GST_PAD_GET_PREROLL_LOCK(pad)))
#define GST_PAD_PREROLL_TRYLOCK(pad)    (g_mutex_trylock(GST_PAD_GET_PREROLL_LOCK(pad)))
#define GST_PAD_PREROLL_UNLOCK(pad)     (g_mutex_unlock(GST_PAD_GET_PREROLL_LOCK(pad)))

#define GST_PAD_GET_PREROLL_COND(pad)   (GST_PAD_CAST(pad)->preroll_cond)
#define GST_PAD_PREROLL_WAIT(pad)       \
    g_cond_wait (GST_PAD_GET_PREROLL_COND (pad), GST_PAD_GET_PREROLL_LOCK (pad))
#define GST_PAD_PREROLL_TIMED_WAIT(pad, timeval) \
    g_cond_timed_wait (GST_PAD_GET_PREROLL_COND (pad), GST_PAD_GET_PREROLL_LOCK (pad), timeval)
#define GST_PAD_PREROLL_SIGNAL(pad)     g_cond_signal (GST_PAD_GET_PREROLL_COND (pad));
#define GST_PAD_PREROLL_BROADCAST(pad)  g_cond_broadcast (GST_PAD_GET_PREROLL_COND (pad));

#define GST_PAD_BLOCK_GET_COND(pad)     (GST_PAD_CAST(pad)->block_cond)
#define GST_PAD_BLOCK_WAIT(pad)         (g_cond_wait(GST_PAD_BLOCK_GET_COND (pad), GST_OBJECT_GET_LOCK (pad)))
#define GST_PAD_BLOCK_SIGNAL(pad)       (g_cond_signal(GST_PAD_BLOCK_GET_COND (pad)))
#define GST_PAD_BLOCK_BROADCAST(pad)    (g_cond_broadcast(GST_PAD_BLOCK_GET_COND (pad)))

/* FIXME: this awful circular dependency need to be resolved properly (see padtemplate.h) */
#include <gst/gstpadtemplate.h>

GType			gst_pad_get_type			(void);

/* creating pads */
GstPad*			gst_pad_new				(const gchar *name, GstPadDirection direction);
GstPad*			gst_pad_new_from_template		(GstPadTemplate *templ, const gchar *name);
GstPad*			gst_pad_new_from_static_template	(GstStaticPadTemplate *templ, const gchar *name);


/**
 * gst_pad_get_name:
 * @pad: the pad to get the name from
 *
 * Get a copy of the name of the pad. g_free() after usage.
 *
 * MT safe.
 */
#define gst_pad_get_name(pad) gst_object_get_name (GST_OBJECT_CAST (pad))
/**
 * gst_pad_get_parent:
 * @pad: the pad to get the parent of
 *
 * Get the parent of @pad. This function increases the refcount
 * of the parent object so you should gst_object_unref() it after usage.
 * Can return NULL if the pad did not have a parent.
 *
 * MT safe.
 */
#define gst_pad_get_parent(pad) gst_object_get_parent (GST_OBJECT_CAST (pad))

GstPadDirection		gst_pad_get_direction			(GstPad *pad);

gboolean		gst_pad_set_active			(GstPad *pad, gboolean active);
gboolean		gst_pad_is_active			(GstPad *pad);
gboolean		gst_pad_activate_pull			(GstPad *pad, gboolean active);
gboolean		gst_pad_activate_push			(GstPad *pad, gboolean active);

gboolean		gst_pad_set_blocked			(GstPad *pad, gboolean blocked);
gboolean		gst_pad_set_blocked_async		(GstPad *pad, gboolean blocked,
								 GstPadBlockCallback callback, gpointer user_data);
gboolean		gst_pad_set_blocked_async_full		(GstPad *pad, gboolean blocked,
								 GstPadBlockCallback callback, gpointer user_data,
                                                                 GDestroyNotify destroy_data);
gboolean		gst_pad_is_blocked			(GstPad *pad);
gboolean		gst_pad_is_blocking			(GstPad *pad);

void			gst_pad_set_element_private		(GstPad *pad, gpointer priv);
gpointer		gst_pad_get_element_private		(GstPad *pad);

GstPadTemplate*		gst_pad_get_pad_template		(GstPad *pad);

void			gst_pad_set_bufferalloc_function	(GstPad *pad, GstPadBufferAllocFunction bufalloc);
GstFlowReturn		gst_pad_alloc_buffer			(GstPad *pad, guint64 offset, gint size,
								 GstCaps *caps, GstBuffer **buf);
GstFlowReturn		gst_pad_alloc_buffer_and_set_caps	(GstPad *pad, guint64 offset, gint size,
								 GstCaps *caps, GstBuffer **buf);

/* data passing setup functions */
void			gst_pad_set_activate_function		(GstPad *pad, GstPadActivateFunction activate);
void			gst_pad_set_activatepull_function	(GstPad *pad, GstPadActivateModeFunction activatepull);
void			gst_pad_set_activatepush_function	(GstPad *pad, GstPadActivateModeFunction activatepush);
void			gst_pad_set_chain_function		(GstPad *pad, GstPadChainFunction chain);
void			gst_pad_set_chain_list_function	(GstPad *pad, GstPadChainListFunction chainlist);
void			gst_pad_set_getrange_function		(GstPad *pad, GstPadGetRangeFunction get);
void			gst_pad_set_checkgetrange_function	(GstPad *pad, GstPadCheckGetRangeFunction check);
void			gst_pad_set_event_function		(GstPad *pad, GstPadEventFunction event);

/* pad links */
void			gst_pad_set_link_function		(GstPad *pad, GstPadLinkFunction link);
void			gst_pad_set_unlink_function		(GstPad *pad, GstPadUnlinkFunction unlink);

gboolean                gst_pad_can_link                        (GstPad *srcpad, GstPad *sinkpad);
GstPadLinkReturn        gst_pad_link				(GstPad *srcpad, GstPad *sinkpad);
GstPadLinkReturn        gst_pad_link_full			(GstPad *srcpad, GstPad *sinkpad, GstPadLinkCheck flags);
gboolean		gst_pad_unlink				(GstPad *srcpad, GstPad *sinkpad);
gboolean		gst_pad_is_linked			(GstPad *pad);

GstPad*			gst_pad_get_peer			(GstPad *pad);

/* capsnego functions */
void			gst_pad_set_getcaps_function		(GstPad *pad, GstPadGetCapsFunction getcaps);
void			gst_pad_set_acceptcaps_function		(GstPad *pad, GstPadAcceptCapsFunction acceptcaps);
void			gst_pad_set_fixatecaps_function		(GstPad *pad, GstPadFixateCapsFunction fixatecaps);
void			gst_pad_set_setcaps_function		(GstPad *pad, GstPadSetCapsFunction setcaps);

G_CONST_RETURN GstCaps*	gst_pad_get_pad_template_caps		(GstPad *pad);

/* capsnego function for linked/unlinked pads */
GstCaps *		gst_pad_get_caps_reffed                 (GstPad * pad);
GstCaps *		gst_pad_get_caps			(GstPad * pad);
void			gst_pad_fixate_caps			(GstPad * pad, GstCaps *caps);
gboolean		gst_pad_accept_caps			(GstPad * pad, GstCaps *caps);
gboolean		gst_pad_set_caps			(GstPad * pad, GstCaps *caps);

GstCaps *		gst_pad_peer_get_caps_reffed		(GstPad * pad);
GstCaps *		gst_pad_peer_get_caps			(GstPad * pad);
gboolean		gst_pad_peer_accept_caps		(GstPad * pad, GstCaps *caps);

/* capsnego for linked pads */
GstCaps *		gst_pad_get_allowed_caps		(GstPad * pad);
GstCaps *		gst_pad_get_negotiated_caps		(GstPad * pad);

/* data passing functions to peer */
GstFlowReturn		gst_pad_push				(GstPad *pad, GstBuffer *buffer);
GstFlowReturn		gst_pad_push_list			(GstPad *pad, GstBufferList *list);
gboolean		gst_pad_check_pull_range		(GstPad *pad);
GstFlowReturn		gst_pad_pull_range			(GstPad *pad, guint64 offset, guint size,
								 GstBuffer **buffer);
gboolean		gst_pad_push_event			(GstPad *pad, GstEvent *event);
gboolean		gst_pad_event_default			(GstPad *pad, GstEvent *event);

/* data passing functions on pad */
GstFlowReturn		gst_pad_chain				(GstPad *pad, GstBuffer *buffer);
GstFlowReturn		gst_pad_chain_list                      (GstPad *pad, GstBufferList *list);
GstFlowReturn		gst_pad_get_range			(GstPad *pad, guint64 offset, guint size,
								 GstBuffer **buffer);
gboolean		gst_pad_send_event			(GstPad *pad, GstEvent *event);

/* pad tasks */
gboolean		gst_pad_start_task			(GstPad *pad, GstTaskFunction func,
								 gpointer data);
gboolean		gst_pad_pause_task			(GstPad *pad);
gboolean		gst_pad_stop_task			(GstPad *pad);

/* internal links */
#ifndef GST_DISABLE_DEPRECATED
void			gst_pad_set_internal_link_function	(GstPad *pad, GstPadIntLinkFunction intlink);
GList*			gst_pad_get_internal_links		(GstPad *pad);
GList*			gst_pad_get_internal_links_default	(GstPad *pad);
#endif

void                    gst_pad_set_iterate_internal_links_function (GstPad * pad,
                                                                 GstPadIterIntLinkFunction iterintlink);
GstIterator *           gst_pad_iterate_internal_links          (GstPad * pad);
GstIterator *           gst_pad_iterate_internal_links_default  (GstPad * pad);


/* generic query function */
void			gst_pad_set_query_type_function		(GstPad *pad, GstPadQueryTypeFunction type_func);
G_CONST_RETURN GstQueryType*
			gst_pad_get_query_types			(GstPad *pad);
G_CONST_RETURN GstQueryType*
			gst_pad_get_query_types_default		(GstPad *pad);

gboolean		gst_pad_query				(GstPad *pad, GstQuery *query);
gboolean		gst_pad_peer_query			(GstPad *pad, GstQuery *query);
void			gst_pad_set_query_function		(GstPad *pad, GstPadQueryFunction query);
gboolean		gst_pad_query_default			(GstPad *pad, GstQuery *query);

/* misc helper functions */
gboolean		gst_pad_dispatcher			(GstPad *pad, GstPadDispatcherFunction dispatch,
								 gpointer data);

#if !defined(GST_DISABLE_LOADSAVE) && !defined(GST_DISABLE_DEPRECATED)
void			gst_pad_load_and_link			(xmlNodePtr self, GstObject *parent);
#endif

G_END_DECLS

#endif /* __GST_PAD_H__ */
