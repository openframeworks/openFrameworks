/*
 * This file is part of the Nice GLib ICE library.
 *
 * (C) 2008 Collabora Ltd.
 *  Contact: Youness Alaoui
 * (C) 2008 Nokia Corporation. All rights reserved.
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

#ifndef _DEBUG_H
#define _DEBUG_H


/**
 * SECTION:debug
 * @short_description: Debug messages utility functions
 * @stability: Unstable
 *
 * <para>Libnice can output a lot of information when debug messages are enabled.
 * This can significantly help track down problems and/or understand what
 * it's doing.</para>
 *
 * <para>You can enable/disable the debug messages by calling nice_debug_enable()
 * or nice_debug_disable() and choosing whether you want only ICE debug messages
 * or also stun debug messages.</para>
 *
 * <para>By default, the debug messages are disabled, unless the environment
 * variable NICE_DEBUG is set, in which case, it must contain a comma separated
 * list of flags specifying which debug to enable.</para>
 * <para> The currently available flags are "nice", "stun", "pseudotcp",
 * "pseudotcp-verbose" or "all" to enable all debug messages.</para>
 * <para> If the 'pseudotcp' flag is enabled, then 'pseudotcp-verbose' gets
 * automatically disabled. This is to allow the use of the 'all' flag without
 * having verbose messages from pseudotcp. You can enable verbose debug messages
 * from the pseudotcp layer by specifying 'pseudotcp-verbose' without the
 * 'pseudotcp' flag.</para>
 *
 *
 * <para>This API is unstable and is subject to change at any time...
 * More flags are to come and a better API to enable/disable each flag
 * should be added.</para>
 */


#include <glib.h>

G_BEGIN_DECLS

/**
 * nice_debug_init:
 *
 * Initialize the debugging system. Uses the NICE_DEBUG environment variable
 * to set the appropriate debugging flags
 */
void nice_debug_init (void);

/**
 * nice_debug_enable:
 * @with_stun: Also enable stun debugging messages
 *
 * Enables libnice debug output to the terminal
 */
void nice_debug_enable (gboolean with_stun);

/**
 * nice_debug_disable:
 * @with_stun: Also disable stun debugging messages
 *
 * Disables libnice debug output to the terminal
 */
void nice_debug_disable (gboolean with_stun);

void nice_debug (const char *fmt, ...);

G_END_DECLS

#endif /* _DEBUG_H */

