/* GStreamer Navigation
 * Copyright (C) 2003 Ronald Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2003 David A. Schleef <ds@schleef.org>
 *
 * navigation.h: navigation interface design
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

#ifndef __GST_NAVIGATION_H__
#define __GST_NAVIGATION_H__

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_NAVIGATION \
  (gst_navigation_get_type ())
#define GST_NAVIGATION(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_NAVIGATION, GstNavigation))
#define GST_IS_NAVIGATION(obj) \
      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_NAVIGATION))
#define GST_NAVIGATION_GET_IFACE(obj) \
    (G_TYPE_INSTANCE_GET_INTERFACE ((obj), GST_TYPE_NAVIGATION, GstNavigationInterface))

typedef struct _GstNavigation GstNavigation;
typedef struct _GstNavigationInterface GstNavigationInterface;

struct _GstNavigationInterface {
  GTypeInterface g_iface;

  /* virtual functions */
  void (*send_event) (GstNavigation *navigation, GstStructure *structure);
  
  gpointer _gst_reserved[GST_PADDING];
};

GType           gst_navigation_get_type (void);

/* Navigation commands */

/**
 * GstNavigationCommand:
 * @GST_NAVIGATION_COMMAND_INVALID: An invalid command entry
 * @GST_NAVIGATION_COMMAND_MENU1: Execute navigation menu command 1. For DVD,
 * this enters the DVD root menu, or exits back to the title from the menu.
 * @GST_NAVIGATION_COMMAND_MENU2: Execute navigation menu command 2. For DVD,
 * this jumps to the DVD title menu.
 * @GST_NAVIGATION_COMMAND_MENU3: Execute navigation menu command 3. For DVD,
 * this jumps into the DVD root menu.
 * @GST_NAVIGATION_COMMAND_MENU4: Execute navigation menu command 4. For DVD,
 * this jumps to the Subpicture menu.
 * @GST_NAVIGATION_COMMAND_MENU5: Execute navigation menu command 5. For DVD,
 * the jumps to the audio menu.
 * @GST_NAVIGATION_COMMAND_MENU6: Execute navigation menu command 6. For DVD,
 * this jumps to the angles menu.
 * @GST_NAVIGATION_COMMAND_MENU7: Execute navigation menu command 7. For DVD,
 * this jumps to the chapter menu.
 * @GST_NAVIGATION_COMMAND_LEFT: Select the next button to the left in a menu,
 * if such a button exists.
 * @GST_NAVIGATION_COMMAND_RIGHT: Select the next button to the right in a menu,
 * if such a button exists.
 * @GST_NAVIGATION_COMMAND_UP: Select the button above the current one in a
 * menu, if such a button exists.
 * @GST_NAVIGATION_COMMAND_DOWN: Select the button below the current one in a
 * menu, if such a button exists.
 * @GST_NAVIGATION_COMMAND_ACTIVATE: Activate (click) the currently selected
 * button in a menu, if such a button exists.
 * @GST_NAVIGATION_COMMAND_PREV_ANGLE: Switch to the previous angle in a
 * multiangle feature.
 * @GST_NAVIGATION_COMMAND_NEXT_ANGLE: Switch to the next angle in a multiangle
 * feature.
 *
 * A set of commands that may be issued to an element providing the
 * #GstNavigation interface. The available commands can be queried via
 * the gst_navigation_query_new_commands() query.
 *
 * For convenience in handling DVD navigation, the MENU commands are aliased as:
 *    GST_NAVIGATION_COMMAND_DVD_MENU            = @GST_NAVIGATION_COMMAND_MENU1
 *    GST_NAVIGATION_COMMAND_DVD_TITLE_MENU      = @GST_NAVIGATION_COMMAND_MENU2
 *    GST_NAVIGATION_COMMAND_DVD_ROOT_MENU       = @GST_NAVIGATION_COMMAND_MENU3
 *    GST_NAVIGATION_COMMAND_DVD_SUBPICTURE_MENU = @GST_NAVIGATION_COMMAND_MENU4
 *    GST_NAVIGATION_COMMAND_DVD_AUDIO_MENU      = @GST_NAVIGATION_COMMAND_MENU5
 *    GST_NAVIGATION_COMMAND_DVD_ANGLE_MENU      = @GST_NAVIGATION_COMMAND_MENU6
 *    GST_NAVIGATION_COMMAND_DVD_CHAPTER_MENU    = @GST_NAVIGATION_COMMAND_MENU7
 *
 * Since: 0.10.23
 */
typedef enum {
  GST_NAVIGATION_COMMAND_INVALID  = 0,

  GST_NAVIGATION_COMMAND_MENU1    = 1,
  GST_NAVIGATION_COMMAND_MENU2    = 2,
  GST_NAVIGATION_COMMAND_MENU3    = 3,
  GST_NAVIGATION_COMMAND_MENU4    = 4,
  GST_NAVIGATION_COMMAND_MENU5    = 5,
  GST_NAVIGATION_COMMAND_MENU6    = 6,
  GST_NAVIGATION_COMMAND_MENU7    = 7,
  
  GST_NAVIGATION_COMMAND_LEFT     = 20,
  GST_NAVIGATION_COMMAND_RIGHT    = 21,
  GST_NAVIGATION_COMMAND_UP       = 22,
  GST_NAVIGATION_COMMAND_DOWN     = 23,
  GST_NAVIGATION_COMMAND_ACTIVATE = 24,

  GST_NAVIGATION_COMMAND_PREV_ANGLE = 30,
  GST_NAVIGATION_COMMAND_NEXT_ANGLE = 31
} GstNavigationCommand;

/* Some aliases for the menu command types */
#define GST_NAVIGATION_COMMAND_DVD_MENU            GST_NAVIGATION_COMMAND_MENU1
#define GST_NAVIGATION_COMMAND_DVD_TITLE_MENU      GST_NAVIGATION_COMMAND_MENU2
#define GST_NAVIGATION_COMMAND_DVD_ROOT_MENU       GST_NAVIGATION_COMMAND_MENU3
#define GST_NAVIGATION_COMMAND_DVD_SUBPICTURE_MENU GST_NAVIGATION_COMMAND_MENU4
#define GST_NAVIGATION_COMMAND_DVD_AUDIO_MENU      GST_NAVIGATION_COMMAND_MENU5
#define GST_NAVIGATION_COMMAND_DVD_ANGLE_MENU      GST_NAVIGATION_COMMAND_MENU6
#define GST_NAVIGATION_COMMAND_DVD_CHAPTER_MENU    GST_NAVIGATION_COMMAND_MENU7

/* Queries */
typedef enum
{
  GST_NAVIGATION_QUERY_INVALID     = 0,
  GST_NAVIGATION_QUERY_COMMANDS    = 1,
  GST_NAVIGATION_QUERY_ANGLES      = 2
} GstNavigationQueryType;

GstNavigationQueryType gst_navigation_query_get_type (GstQuery *query);

GstQuery *gst_navigation_query_new_commands (void);
void gst_navigation_query_set_commands (GstQuery *query, gint n_cmds, ...);
void gst_navigation_query_set_commandsv (GstQuery *query, gint n_cmds,
    GstNavigationCommand *cmds);
gboolean gst_navigation_query_parse_commands_length (GstQuery *query,
                                                 guint *n_cmds);
gboolean gst_navigation_query_parse_commands_nth (GstQuery *query, guint nth,
                                              GstNavigationCommand *cmd);

GstQuery *gst_navigation_query_new_angles (void);
void gst_navigation_query_set_angles (GstQuery *query, guint cur_angle,
                                      guint n_angles);
gboolean gst_navigation_query_parse_angles (GstQuery *query, guint *cur_angle,
                                            guint *n_angles);

/* Element messages */
/**
 * GstNavigationMessageType:
 * @GST_NAVIGATION_MESSAGE_INVALID: Returned from
 * gst_navigation_message_get_type() when the passed message is not a
 * navigation message.
 * @GST_NAVIGATION_MESSAGE_MOUSE_OVER: Sent when the mouse moves over or leaves a
 * clickable region of the output, such as a DVD menu button.
 * @GST_NAVIGATION_MESSAGE_COMMANDS_CHANGED: Sent when the set of available commands
 * changes and should re-queried by interested applications.
 * @GST_NAVIGATION_MESSAGE_ANGLES_CHANGED: Sent when display angles in a multi-angle
 * feature (such as a multiangle DVD) change - either angles have appeared or
 * disappeared.
 *
 * A set of notifications that may be received on the bus when navigation
 * related status changes.
 *
 * Since: 0.10.23
 */
typedef enum {
  GST_NAVIGATION_MESSAGE_INVALID,
  GST_NAVIGATION_MESSAGE_MOUSE_OVER,
  GST_NAVIGATION_MESSAGE_COMMANDS_CHANGED,
  GST_NAVIGATION_MESSAGE_ANGLES_CHANGED
} GstNavigationMessageType;

GstNavigationMessageType gst_navigation_message_get_type (GstMessage *message);

GstMessage *gst_navigation_message_new_mouse_over (GstObject *src,
                                                   gboolean active);
gboolean gst_navigation_message_parse_mouse_over (GstMessage *message,
                                              gboolean *active);

GstMessage *gst_navigation_message_new_commands_changed (GstObject *src);

GstMessage *gst_navigation_message_new_angles_changed (GstObject *src,
                                                       guint cur_angle,
                                                       guint n_angles);
gboolean gst_navigation_message_parse_angles_changed (GstMessage *message,
                                                      guint *cur_angle,
                                                      guint *n_angles);

/* event parsing functions */
/**
 * GstNavigationEventType:
 * @GST_NAVIGATION_EVENT_INVALID: Returned from
 * gst_navigation_event_get_type() when the passed event is not a navigation event.
 * @GST_NAVIGATION_EVENT_KEY_PRESS: A key press event. Use
 * gst_navigation_event_parse_key_event() to extract the details from the event.
 * @GST_NAVIGATION_EVENT_KEY_RELEASE: A key release event. Use
 * gst_navigation_event_parse_key_event() to extract the details from the event.
 * @GST_NAVIGATION_EVENT_MOUSE_BUTTON_PRESS: A mouse button press event. Use
 * gst_navigation_event_parse_mouse_button_event() to extract the details from the
 * event.
 * @GST_NAVIGATION_EVENT_MOUSE_BUTTON_RELEASE: A mouse button release event. Use
 * gst_navigation_event_parse_mouse_button_event() to extract the details from the
 * event.
 * @GST_NAVIGATION_EVENT_MOUSE_MOVE: A mouse movement event. Use
 * gst_navigation_event_parse_mouse_move_event() to extract the details from the
 * event.
 * @GST_NAVIGATION_EVENT_COMMAND: A navigation command event. Use
 * gst_navigation_event_parse_command() to extract the details from the event.
 *
 * Enum values for the various events that an element implementing the
 * GstNavigation interface might send up the pipeline.
 *
 * Since: 0.10.23
 */
typedef enum {
  GST_NAVIGATION_EVENT_INVALID                    = 0,
  GST_NAVIGATION_EVENT_KEY_PRESS                  = 1,
  GST_NAVIGATION_EVENT_KEY_RELEASE                = 2,
  GST_NAVIGATION_EVENT_MOUSE_BUTTON_PRESS         = 3,
  GST_NAVIGATION_EVENT_MOUSE_BUTTON_RELEASE       = 4,
  GST_NAVIGATION_EVENT_MOUSE_MOVE                 = 5,
  GST_NAVIGATION_EVENT_COMMAND                    = 6
} GstNavigationEventType;

GstNavigationEventType gst_navigation_event_get_type (GstEvent *event);
gboolean gst_navigation_event_parse_key_event (GstEvent *event,
    const gchar **key);
gboolean gst_navigation_event_parse_mouse_button_event (GstEvent *event,
    gint *button, gdouble *x, gdouble *y);
gboolean gst_navigation_event_parse_mouse_move_event (GstEvent *event,
    gdouble *x, gdouble *y);
gboolean gst_navigation_event_parse_command (GstEvent *event,
    GstNavigationCommand *command);

/* interface virtual function wrappers */
void gst_navigation_send_event (GstNavigation *navigation,
        GstStructure *structure);
void gst_navigation_send_key_event (GstNavigation *navigation, 
        const char *event, const char *key);
void gst_navigation_send_mouse_event (GstNavigation *navigation, 
        const char *event, int button, double x, double y);
void gst_navigation_send_command (GstNavigation *navigation,
        GstNavigationCommand command);

G_END_DECLS

#endif /* __GST_NAVIGATION_H__ */
