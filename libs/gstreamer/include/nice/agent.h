/*
 * This file is part of the Nice GLib ICE library.
 *
 * (C) 2006-2010 Collabora Ltd.
 *  Contact: Youness Alaoui
 * (C) 2006-2010 Nokia Corporation. All rights reserved.
 *  Contact: Kai Vehmanen
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Nice GLib ICE library.
 *
 * The Initial Developers of the Original Code are Collabora Ltd and Nokia
 * Corporation. All Rights Reserved.
 *
 * Contributors:
 *   Dafydd Harries, Collabora Ltd.
 *   Youness Alaoui, Collabora Ltd.
 *   Kai Vehmanen, Nokia
 *
 * Alternatively, the contents of this file may be used under the terms of the
 * the GNU Lesser General Public License Version 2.1 (the "LGPL"), in which
 * case the provisions of LGPL are applicable instead of those above. If you
 * wish to allow use of your version of this file only under the terms of the
 * LGPL and not to allow others to use your version of this file under the
 * MPL, indicate your decision by deleting the provisions above and replace
 * them with the notice and other provisions required by the LGPL. If you do
 * not delete the provisions above, a recipient may use your version of this
 * file under either the MPL or the LGPL.
 */

#ifndef _AGENT_H
#define _AGENT_H

/**
 * SECTION:agent
 * @short_description:  ICE agent API implementation
 * @see_also: #NiceCandidate
 * @see_also: #NiceAddress
 * @include: agent.h
 * @stability: Stable
 *
 * The #NiceAgent is your main object when using libnice.
 * It is the agent that will take care of everything relating to ICE.
 * It will take care of discovering your local candidates and do
 *  connectivity checks to create a stream of data between you and your peer.
 *
 <example>
   <title>Simple example on how to use libnice</title>
   <programlisting>
   guint stream_id;
   gchar buffer[] = "hello world!";
   GSList *lcands = NULL;

   // Create a nice agent
   NiceAgent *agent = nice_agent_new (NULL, NICE_COMPATIBILITY_RFC5245);

   // Connect the signals
   g_signal_connect (G_OBJECT (agent), "candidate-gathering-done",
                     G_CALLBACK (cb_candidate_gathering_done), NULL);
   g_signal_connect (G_OBJECT (agent), "component-state-changed",
                     G_CALLBACK (cb_component_state_changed), NULL);
   g_signal_connect (G_OBJECT (agent), "new-selected-pair",
                     G_CALLBACK (cb_new_selected_pair), NULL);

   // Create a new stream with one component and start gathering candidates
   stream_id = nice_agent_add_stream (agent, 1);
   nice_agent_gather_candidates (agent, stream_id);

   // Attach to the component to receive the data
   nice_agent_attach_recv (agent, stream_id, 1, NULL,
                          cb_nice_recv, NULL);

   // ... Wait until the signal candidate-gathering-done is fired ...
   lcands = nice_agent_get_local_candidates(agent, stream_id, 1);

   // ... Send local candidates to the peer and set the peer's remote candidates
   nice_agent_set_remote_candidates (agent, stream_id, 1, rcands);

   // ... Wait until the signal new-selected-pair is fired ...
   // Send our message!
   nice_agent_send (agent, stream_id, 1, sizeof(buffer), buffer);

   // Anything received will be received through the cb_nice_recv callback

   // Destroy the object
   g_object_unref(agent);

   </programlisting>
 </example>
 */


#include <glib-object.h>

/**
 * NiceAgent:
 *
 * The #NiceAgent is the main GObject of the libnice library and represents
 * the ICE agent.
 */
typedef struct _NiceAgent NiceAgent;

#include "address.h"
#include "candidate.h"
#include "debug.h"


G_BEGIN_DECLS

#define NICE_TYPE_AGENT nice_agent_get_type()

#define NICE_AGENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  NICE_TYPE_AGENT, NiceAgent))

#define NICE_AGENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  NICE_TYPE_AGENT, NiceAgentClass))

#define NICE_IS_AGENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  NICE_TYPE_AGENT))

#define NICE_IS_AGENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  NICE_TYPE_AGENT))

#define NICE_AGENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  NICE_TYPE_AGENT, NiceAgentClass))

typedef struct _NiceAgentClass NiceAgentClass;

struct _NiceAgentClass
{
  GObjectClass parent_class;
};


GType nice_agent_get_type (void);


/**
 * NICE_AGENT_MAX_REMOTE_CANDIDATES:
 *
 * A hard limit for the number of remote candidates. This
 * limit is enforced to protect against malevolent remote
 * clients.
 */
#define NICE_AGENT_MAX_REMOTE_CANDIDATES    25

/**
 * NiceComponentState:
 * @NICE_COMPONENT_STATE_DISCONNECTED: No activity scheduled
 * @NICE_COMPONENT_STATE_GATHERING: Gathering local candidates
 * @NICE_COMPONENT_STATE_CONNECTING: Establishing connectivity
 * @NICE_COMPONENT_STATE_CONNECTED: At least one working candidate pair
 * @NICE_COMPONENT_STATE_READY: ICE concluded, candidate pair selection
 * is now final
 * @NICE_COMPONENT_STATE_FAILED: Connectivity checks have been completed,
 * but connectivity was not established
 * @NICE_COMPONENT_STATE_LAST: Dummy state
 *
 * An enum representing the state of a component.
 * <para> See also: #NiceAgent::component-state-changed </para>
 */
typedef enum
{
  NICE_COMPONENT_STATE_DISCONNECTED,
  NICE_COMPONENT_STATE_GATHERING,
  NICE_COMPONENT_STATE_CONNECTING,
  NICE_COMPONENT_STATE_CONNECTED,
  NICE_COMPONENT_STATE_READY,
  NICE_COMPONENT_STATE_FAILED,
  NICE_COMPONENT_STATE_LAST
} NiceComponentState;


/**
 * NiceComponentType:
 * @NICE_COMPONENT_TYPE_RTP: RTP Component type
 * @NICE_COMPONENT_TYPE_RTCP: RTCP Component type
 *
 * Convenience enum representing the type of a component for use as the
 * component_id for RTP/RTCP usages.
 <example>
   <title>Example of use.</title>
   <programlisting>
   nice_agent_send (agent, stream_id, NICE_COMPONENT_TYPE_RTP, len, buf);
   </programlisting>
  </example>
 */
typedef enum
{
  NICE_COMPONENT_TYPE_RTP = 1,
  NICE_COMPONENT_TYPE_RTCP = 2
} NiceComponentType;


/**
 * NiceCompatibility:
 * @NICE_COMPATIBILITY_RFC5245: Use compatibility with the RFC5245 ICE specs
 * @NICE_COMPATIBILITY_GOOGLE: Use compatibility for Google Talk specs
 * @NICE_COMPATIBILITY_MSN: Use compatibility for MSN Messenger specs
 * @NICE_COMPATIBILITY_WLM2009: Use compatibility with Windows Live Messenger
 * 2009
 * @NICE_COMPATIBILITY_DRAFT19: Use compatibility for ICE Draft 19 specs
 * @NICE_COMPATIBILITY_LAST: Dummy last compatibility mode
 *
 * An enum to specify which compatible specifications the #NiceAgent should use.
 * Use with nice_agent_new()
 *
 * <warning>@NICE_COMPATIBILITY_DRAFT19 is deprecated and should not be used
 * in newly-written code. It is kept for compatibility reasons and
 * represents the same compatibility as @NICE_COMPATIBILITY_RFC5245 </warning>
 */
typedef enum
{
  NICE_COMPATIBILITY_RFC5245 = 0,
  NICE_COMPATIBILITY_GOOGLE,
  NICE_COMPATIBILITY_MSN,
  NICE_COMPATIBILITY_WLM2009,
  NICE_COMPATIBILITY_DRAFT19 = NICE_COMPATIBILITY_RFC5245,
  NICE_COMPATIBILITY_LAST = NICE_COMPATIBILITY_WLM2009,
} NiceCompatibility;

/**
 * NiceProxyType:
 * @NICE_PROXY_TYPE_NONE: Do not use a proxy
 * @NICE_PROXY_TYPE_SOCKS5: Use a SOCKS5 proxy
 * @NICE_PROXY_TYPE_HTTP: Use an HTTP proxy
 * @NICE_PROXY_TYPE_LAST: Dummy last proxy type
 *
 * An enum to specify which proxy type to use for relaying.
 * Note that the proxies will only be used with TCP TURN relaying.
 * <para> See also: #NiceAgent:proxy-type </para>
 *
 * Since: 0.0.4
 */
typedef enum
{
  NICE_PROXY_TYPE_NONE = 0,
  NICE_PROXY_TYPE_SOCKS5,
  NICE_PROXY_TYPE_HTTP,
  NICE_PROXY_TYPE_LAST = NICE_PROXY_TYPE_HTTP,
} NiceProxyType;


/**
 * NiceAgentRecvFunc:
 * @agent: The #NiceAgent Object
 * @stream_id: The id of the stream
 * @component_id: The id of the component of the stream
 *        which received the data
 * @len: The length of the data
 * @buf: The buffer containing the data received
 * @user_data: The user data set in nice_agent_attach_recv()
 *
 * Callback function when data is received on a component
 *
 */
typedef void (*NiceAgentRecvFunc) (
  NiceAgent *agent, guint stream_id, guint component_id, guint len,
  gchar *buf, gpointer user_data);


/**
 * nice_agent_new:
 * @ctx: The Glib Mainloop Context to use for timers
 * @compat: The compatibility mode of the agent
 *
 * Create a new #NiceAgent.
 * The returned object must be freed with g_object_unref()
 *
 * Returns: The new agent GObject
 */
NiceAgent *
nice_agent_new (GMainContext *ctx, NiceCompatibility compat);


/**
 * nice_agent_new_reliable:
 * @ctx: The Glib Mainloop Context to use for timers
 * @compat: The compatibility mode of the agent
 *
 * Create a new #NiceAgent in reliable mode, which uses #PseudoTcpSocket to
 * assure reliability of the messages.
 * The returned object must be freed with g_object_unref()
 * <para> See also: #NiceAgent::reliable-transport-writable </para>
 *
 * Since: 0.0.11
 *
 * Returns: The new agent GObject
 */
NiceAgent *
nice_agent_new_reliable (GMainContext *ctx, NiceCompatibility compat);

/**
 * nice_agent_add_local_address:
 * @agent: The #NiceAgent Object
 * @addr: The address to listen to
 * If the port is 0, then a random port will be chosen by the system
 *
 * Add a local address from which to derive local host candidates for
 * candidate gathering.
 * <para>
 * Since 0.0.5, if this method is not called, libnice will automatically
 * discover the local addresses available
 * </para>
 *
 * See also: nice_agent_gather_candidates()
 * Returns: %TRUE on success, %FALSE on fatal (memory allocation) errors
 */
gboolean
nice_agent_add_local_address (NiceAgent *agent, NiceAddress *addr);


/**
 * nice_agent_add_stream:
 * @agent: The #NiceAgent Object
 * @n_components: The number of components to add to the stream
 *
 * Adds a data stream to @agent containing @n_components components.
 *
 * Returns: The ID of the new stream, 0 on failure
 **/
guint
nice_agent_add_stream (
  NiceAgent *agent,
  guint n_components);

/**
 * nice_agent_remove_stream:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of the stream to remove
 *
 * Remove and free a previously created data stream from @agent
 *
 **/
void
nice_agent_remove_stream (
  NiceAgent *agent,
  guint stream_id);

/**
 * nice_agent_set_relay_info:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of the stream
 * @component_id: The ID of the component
 * @server_ip: The IP address of the TURN server
 * @server_port: The port of the TURN server
 * @username: The TURN username to use for the allocate
 * @password: The TURN password to use for the allocate
 * @type: The type of relay to use
 *
 * Sets the settings for using a relay server during the candidate discovery.
 *
 * Returns: %TRUE if the TURN settings were accepted.
 * %FALSE if the address was invalid.
 */
gboolean nice_agent_set_relay_info(
    NiceAgent *agent,
    guint stream_id,
    guint component_id,
    const gchar *server_ip,
    guint server_port,
    const gchar *username,
    const gchar *password,
    NiceRelayType type);

/**
 * nice_agent_gather_candidates:
 * @agent: The #NiceAgent Object
 * @stream_id: The id of the stream to start
 *
 * Start the candidate gathering process.
 * Once done, #NiceAgent::candidate-gathering-done is called for the stream
 *
 * Returns: %FALSE if there were no local addresses and they couldn't be discovered. In this case, call nice_agent_add_local_address() first.
 *
 * See also: nice_agent_add_local_address()
 <note>
   <para>
    Local addresses can be previously set with nice_agent_add_local_address()
  </para>
  <para>
    Since 0.0.5, If no local address was previously added, then the nice agent
    will automatically detect the local address using
    nice_interfaces_get_local_ips()
   </para>
 </note>
 */
gboolean
nice_agent_gather_candidates (
  NiceAgent *agent,
  guint stream_id);

/**
 * nice_agent_set_remote_credentials:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of the stream
 * @ufrag: NULL-terminated string containing an ICE username fragment
 *    (length must be between 22 and 256 chars)
 * @pwd: NULL-terminated string containing an ICE password
 *    (length must be between 4 and 256 chars)
 *
 * Sets the remote credentials for stream @stream_id.
 *
 <note>
   <para>
     Stream credentials do not override per-candidate credentials if set
   </para>
 </note>
 *
 * Returns: %TRUE on success, %FALSE on error.
 */
gboolean
nice_agent_set_remote_credentials (
  NiceAgent *agent,
  guint stream_id,
  const gchar *ufrag, const gchar *pwd);



/**
 * nice_agent_get_local_credentials:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of the stream
 * @ufrag: a pointer to a NULL-terminated string containing
 * an ICE username fragment [OUT].
 * This string must be freed with g_free()
 * @pwd: a pointer to a NULL-terminated string containing an ICE
 * password [OUT]
 * This string must be freed with g_free()
 *
 * Gets the local credentials for stream @stream_id.
 *
 * Returns: %TRUE on success, %FALSE on error.
 */
gboolean
nice_agent_get_local_credentials (
  NiceAgent *agent,
  guint stream_id,
  gchar **ufrag, gchar **pwd);

/**
 * nice_agent_set_remote_candidates:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of the stream the candidates are for
 * @component_id: The ID of the component the candidates are for
 * @candidates: a #GList of #NiceCandidate items describing each candidate to add
 *
 * Sets, adds or updates the remote candidates for a component of a stream.
 *
 <note>
   <para>
    NICE_AGENT_MAX_REMOTE_CANDIDATES is the absolute maximum limit
    for remote candidates.
   </para>
   <para>
   You must first call nice_agent_gather_candidates() and wait for the
   #NiceAgent::candidate-gathering-done signale before
   calling nice_agent_set_remote_candidates()
   </para>
 </note>
 *
 * Returns: The number of candidates added, negative on errors (memory allocation
 * or if the local candidates are not done gathering yet)
 **/
int
nice_agent_set_remote_candidates (
  NiceAgent *agent,
  guint stream_id,
  guint component_id,
  const GSList *candidates);


/**
 * nice_agent_send:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of the stream to send to
 * @component_id: The ID of the component to send to
 * @len: The length of the buffer to send
 * @buf: The buffer of data to send
 *
 * Sends a data payload over a stream's component.
 *
 <note>
   <para>
     Component state MUST be NICE_COMPONENT_STATE_READY, or as a special case,
     in any state if component was in READY state before and was then restarted
   </para>
 </note>
 *
 * Returns: The number of bytes sent, or negative error code
 */
gint
nice_agent_send (
  NiceAgent *agent,
  guint stream_id,
  guint component_id,
  guint len,
  const gchar *buf);

/**
 * nice_agent_get_local_candidates:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of the stream
 * @component_id: The ID of the component
 *
 * Retreive from the agent the list of all local candidates
 * for a stream's component
 *
 <note>
   <para>
     The caller owns the returned GSList as well as the candidates contained
     within it.
     To get full results, the client should wait for the
     #NiceAgent::candidate-gathering-done signal.
   </para>
 </note>
 *
 * Returns: a #GSList of #NiceCandidate objects representing
 * the local candidates of @agent
 **/
GSList *
nice_agent_get_local_candidates (
  NiceAgent *agent,
  guint stream_id,
  guint component_id);


/**
 * nice_agent_get_remote_candidates:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of the stream
 * @component_id: The ID of the component
 *
 * Get a list of the remote candidates set on a stream's component
 *
 <note>
   <para>
     The caller owns the returned GSList but not the candidates
     contained within it.
   </para>
   <para>
     The list of remote candidates can change during processing.
     The client should register for the #NiceAgent::new-remote-candidate signal
     to get notified of new remote candidates.
   </para>
 </note>
 *
 * Returns: a #GSList of #NiceCandidates objects representing
 * the remote candidates set on the @agent
 **/
GSList *
nice_agent_get_remote_candidates (
  NiceAgent *agent,
  guint stream_id,
  guint component_id);

/**
 * nice_agent_restart:
 * @agent: The #NiceAgent Object
 *
 * Restarts the session as defined in ICE draft 19. This function
 * needs to be called both when initiating (ICE spec section 9.1.1.1.
 * "ICE Restarts"), as well as when reacting (spec section 9.2.1.1.
 * "Detecting ICE Restart") to a restart.
 *
 * Returns: %TRUE on success %FALSE on error
 **/
gboolean
nice_agent_restart (
  NiceAgent *agent);


/**
 * nice_agent_attach_recv:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of stream
 * @component_id: The ID of the component
 * @ctx: The Glib Mainloop Context to use for listening on the component
 * @func: The callback function to be called when data is received on
 * the stream's component
 * @data: user data associated with the callback
 *
 * Attaches the stream's component's sockets to the Glib Mainloop Context in
 * order to be notified whenever data becomes available for a component.
 *
 * Returns: %TRUE on success, %FALSE if the stream or component IDs are invalid.
 */
gboolean
nice_agent_attach_recv (
  NiceAgent *agent,
  guint stream_id,
  guint component_id,
  GMainContext *ctx,
  NiceAgentRecvFunc func,
  gpointer data);


/**
 * nice_agent_set_selected_pair:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of the stream
 * @component_id: The ID of the component
 * @lfoundation: The local foundation of the candidate to use
 * @rfoundation: The remote foundation of the candidate to use
 *
 * Sets the selected candidate pair for media transmission
 * for a given stream's component. Calling this function will
 * disable all further ICE processing (connection check,
 * state machine updates, etc). Note that keepalives will
 * continue to be sent.
 *
 * Returns: %TRUE on success, %FALSE if the candidate pair cannot be found
 */
gboolean
nice_agent_set_selected_pair (
  NiceAgent *agent,
  guint stream_id,
  guint component_id,
  const gchar *lfoundation,
  const gchar *rfoundation);

/**
 * nice_agent_set_selected_remote_candidate:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of the stream
 * @component_id: The ID of the component
 * @candidate: The #NiceCandidate to select
 *
 * Sets the selected remote candidate for media transmission
 * for a given stream's component. This is used to force the selection of
 * a specific remote candidate even when connectivity checks are failing
 * (e.g. non-ICE compatible candidates).
 * Calling this function will disable all further ICE processing
 * (connection check, state machine updates, etc). Note that keepalives will
 * continue to be sent.
 *
 * Returns: %TRUE on success, %FALSE on failure
 */
gboolean
nice_agent_set_selected_remote_candidate (
  NiceAgent *agent,
  guint stream_id,
  guint component_id,
  NiceCandidate *candidate);


/**
 * nice_agent_set_stream_tos:
 * @agent: The #NiceAgent Object
 * @stream_id: The ID of the stream
 * @tos: The ToS to set
 *
 * Sets the IP_TOS and/or IPV6_TCLASS field on the stream's sockets' options
 *
 * Since: 0.0.9
 */
void nice_agent_set_stream_tos (
  NiceAgent *agent,
  guint stream_id,
  gint tos);



/**
 * nice_agent_set_software:
 * @agent: The #NiceAgent Object
 * @software: The value of the SOFTWARE attribute to add.
 *
 * This function will set the value of the SOFTWARE attribute to be added to
 * STUN requests, responses and error responses sent during connectivity checks.
 * <para>
 * The SOFTWARE attribute will only be added in the #NICE_COMPATIBILITY_RFC5245
 * and #NICE_COMPATIBILITY_WLM2009 compatibility modes.
 *
 * </para>
 * <note>
     <para>
       The @software argument will be appended with the libnice version before
       being sent.
     </para>
     <para>
       The @software argument must be in UTF-8 encoding and only the first
       128 characters will be sent.
     </para>
   </note>
 *
 * Since: 0.0.10
 *
 */
void nice_agent_set_software (NiceAgent *agent, const gchar *software);

G_END_DECLS

#endif /* _AGENT_H */

