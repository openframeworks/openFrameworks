/*
 * Farsight2 - Farsight Candidate
 *
 * Copyright 2007 Collabora Ltd.
 *  @author: Philippe Kalaf <philippe.kalaf@collabora.co.uk>
 * Copyright 2007 Nokia Corp.
 *
 * fs-candidate.h - A Farsight candidate
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef __FS_CANDIDATE_H__
#define __FS_CANDIDATE_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define FS_TYPE_CANDIDATE \
  (fs_candidate_get_type ())


#define FS_TYPE_CANDIDATE_LIST \
  (fs_candidate_list_get_type ())

/**
 * FsCandidateType:
 * @FS_CANDIDATE_TYPE_HOST: A host candidate (local)
 * @FS_CANDIDATE_TYPE_SRFLX: A server reflexive candidate.
 * @FS_CANDIDATE_TYPE_PRFLX: A peer reflexive candidate
 * @FS_CANDIDATE_TYPE_RELAY: An relay candidate
 * @FS_CANDIDATE_TYPE_MULTICAST: A multicast address
 *
 * An enum for the type of candidate used/reported
 */
typedef enum
{
  FS_CANDIDATE_TYPE_HOST,
  FS_CANDIDATE_TYPE_SRFLX,
  FS_CANDIDATE_TYPE_PRFLX,
  FS_CANDIDATE_TYPE_RELAY,    /* An external stream relay */
  FS_CANDIDATE_TYPE_MULTICAST
} FsCandidateType;

/**
 * FsNetworkProtocol:
 * @FS_NETWORK_PROTOCOL_UDP: A UDP based protocol
 * @FS_NETWORK_PROTOCOL_TCP: A TCP based protocol
 *
 * An enum for the base IP protocol
 */
typedef enum
{
  FS_NETWORK_PROTOCOL_UDP,
  FS_NETWORK_PROTOCOL_TCP
} FsNetworkProtocol;

/**
 * FsComponentType:
 * @FS_COMPONENT_NONE: Use this when specifying a component is innapropriate
 * @FS_COMPONENT_RTP: This component is for RTP data
 * @FS_COMPONENT_RTCP: This component is for RTCP control
 *
 * This enum contains the component IDs defined in ICE-19
 */

typedef enum
{
  FS_COMPONENT_NONE = 0,
  FS_COMPONENT_RTP = 1,
  FS_COMPONENT_RTCP = 2
} FsComponentType;


typedef struct _FsCandidate FsCandidate;

/**
 * FsCandidate:
 * @foundation: a string representing the foundation of this candidate (maximum 32 chars)
 * @component_id: value between 1 and 256 indicating which component this candidate represents (1 is RTP, 2 is RTCP, #FsComponentType can be used here)
 * @ip: IP in dotted format
 * @port: Port to use
 * @base_ip: IP of base in dotted format as defined in ICE-19.
 * @base_port: Port of base as defined in ICE-19.
 * @proto: #FsNetworkProtocol for ip protocol to use as candidate
 * @priority: Value between 0 and (2^31 - 1) representing the priority
 * @type: The #FsCandidateType of the candidate
 * @username: Username to use to connect to client if necessary,
 *            NULL otherwise
 * @password: Username to use to connect to client if necessary,
 *            NULL otherwise
 * @ttl: The TTL used when sending Multicast packet (0 = auto)
 *
 * Struct to hold information about ICE-19 compliant candidates
 */
struct _FsCandidate
{
  gchar *foundation;
  guint component_id;
  const gchar *ip;
  guint16 port;
  const gchar *base_ip;
  guint16 base_port;
  FsNetworkProtocol proto;
  guint32 priority;
  FsCandidateType type;
  const gchar *username;
  const gchar *password;
  guint ttl;
  /*< private >*/
  gpointer _padding[4];
};

GType fs_candidate_get_type (void);
GType fs_candidate_list_get_type (void);

void fs_candidate_destroy (FsCandidate *cand);

FsCandidate *fs_candidate_copy (const FsCandidate *cand);

void fs_candidate_list_destroy (GList *candidate_list);

GList *fs_candidate_list_copy (const GList *candidate_list);

FsCandidate * fs_candidate_new (
    const gchar *foundation,
    guint component_id,
    FsCandidateType type,
    FsNetworkProtocol proto,
    const gchar *ip,
    guint port);

G_END_DECLS
#endif /* __FS_CANDIDATE_H__ */
