/*
 * This file is part of the Nice GLib ICE library.
 *
 * (C) 2008-2009 Collabora Ltd.
 *  Contact: Youness Alaoui
 * (C) 2007-2009 Nokia Corporation. All rights reserved.
 *  Contact: Rémi Denis-Courmont
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
 *   Youness Alaoui, Collabora Ltd.
 *   Rémi Denis-Courmont, Nokia
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

#ifndef STUN_TIMER_H
# define STUN_TIMER_H 1

/**
 * SECTION:timer
 * @short_description: STUN timer Usage
 * @include: stun/usages/timer.h
 * @stability: Stable
 *
 * The STUN timer usage is a set of helpful utility functions that allows you
 * to easily track when a STUN message should be retransmitted or considered
 * as timed out.
 *
 *
 <example>
   <title>Simple example on how to use the timer usage</title>
   <programlisting>
   StunTimer timer;
   unsigned remainder;
   StunUsageTimerReturn ret;

   // Build the message, etc..
   ...

   // Send the message and start the timer
   send(socket, request, sizeof(request));
   stun_timer_start(&timer);

   // Loop until we get the response
   for (;;) {
     remainder = stun_timer_remainder(&timer);

     // Poll the socket until data is received or the timer expires
     if (poll (&pollfd, 1, delay) <= 0) {
       // Time out and no response was received
       ret = stun_timer_refresh (&timer);
       if (ret == STUN_USAGE_TIMER_RETURN_TIMEOUT) {
         // Transaction timed out
         break;
       } else if (ret == STUN_USAGE_TIMER_RETURN_RETRANSMIT) {
         // A retransmission is necessary
         send(socket, request, sizeof(request));
         continue;
       } else if (ret == STUN_USAGE_TIMER_RETURN_SUCCESS) {
         // The refresh succeeded and nothing has to be done, continue polling
         continue;
       }
     } else {
       // We received a response, read it
       recv(socket, response, sizeof(response));
       break;
     }
   }

   // Check if the transaction timed out or not
   if (ret == STUN_USAGE_TIMER_RETURN_TIMEOUT) {
     // do whatever needs to be done in that case
   } else {
     // Parse the response
   }

   </programlisting>
 </example>
 */

#ifdef _WIN32
#include <winsock2.h>
#else
# include <sys/types.h>
# include <sys/time.h>
# include <time.h>
#endif


/**
 * StunTimer:
 *
 * An opaque structure representing a STUN transaction retransmission timer
 */
typedef struct stun_timer_s StunTimer;

struct stun_timer_s {
  struct timeval deadline;
  unsigned delay;
};


/**
 * StunUsageTimerReturn:
 * @STUN_USAGE_TIMER_RETURN_SUCCESS: The timer was refreshed successfully
 * and there is nothing to be done
 * @STUN_USAGE_TIMER_RETURN_RETRANSMIT: The timer expired and the message
 * should be retransmitted now.
 * @STUN_USAGE_TIMER_RETURN_TIMEOUT: The timer expired as well as all the
 * retransmissions, the transaction timed out
 *
 * Return value of stun_usage_timer_refresh() which provides you with status
 * information on the timer.
 */
typedef enum {
  STUN_USAGE_TIMER_RETURN_SUCCESS,
  STUN_USAGE_TIMER_RETURN_RETRANSMIT,
  STUN_USAGE_TIMER_RETURN_TIMEOUT
} StunUsageTimerReturn;

# ifdef __cplusplus
extern "C" {
# endif

/**
 * stun_timer_start:
 * @timer: The #StunTimer to start
 *
 * Starts a STUN transaction retransmission timer.
 * This should be called as soon as you send the message for the first time on
 * a UDP socket
 */
void stun_timer_start (StunTimer *timer);

/**
 * stun_timer_start_reliable:
 * @timer: The #StunTimer to start
 *
 * Starts a STUN transaction retransmission timer for a reliable transport.
 * This should be called as soon as you send the message for the first time on
 * a TCP socket
 */
void stun_timer_start_reliable (StunTimer *timer);

/**
 * stun_timer_refresh:
 * @timer: The #StunTimer to refresh
 *
 * Updates a STUN transaction retransmission timer.
 * Returns: A #StunUsageTimerReturn telling you what to do next
 */
StunUsageTimerReturn stun_timer_refresh (StunTimer *timer);

/**
 * stun_timer_remainder:
 * @timer: The #StunTimer to query
 *
 * Query the timer on the time left before the next refresh should be done
 * Returns: The time remaining for the timer to expire in milliseconds
 */
unsigned stun_timer_remainder (const StunTimer *timer);

# ifdef __cplusplus
}
# endif

#endif /* !STUN_TIMER_H */
