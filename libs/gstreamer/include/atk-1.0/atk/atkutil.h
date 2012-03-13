/* ATK -  Accessibility Toolkit
 * Copyright 2001 Sun Microsystems Inc.
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

#if defined(ATK_DISABLE_SINGLE_INCLUDES) && !defined (__ATK_H_INSIDE__) && !defined (ATK_COMPILATION)
#error "Only <atk/atk.h> can be included directly."
#endif

#ifndef __ATK_UTIL_H__
#define __ATK_UTIL_H__

#include <atk/atkobject.h>

G_BEGIN_DECLS

#define ATK_TYPE_UTIL                   (atk_util_get_type ())
#define ATK_IS_UTIL(obj)                G_TYPE_CHECK_INSTANCE_TYPE ((obj), ATK_TYPE_UTIL)
#define ATK_UTIL(obj)                   G_TYPE_CHECK_INSTANCE_CAST ((obj), ATK_TYPE_UTIL, AtkUtil)
#define ATK_UTIL_CLASS(klass)                   (G_TYPE_CHECK_CLASS_CAST ((klass), ATK_TYPE_UTIL, AtkUtilClass))
#define ATK_IS_UTIL_CLASS(klass)                (G_TYPE_CHECK_CLASS_TYPE ((klass), ATK_TYPE_UTIL))
#define ATK_UTIL_GET_CLASS(obj)                 (G_TYPE_INSTANCE_GET_CLASS ((obj), ATK_TYPE_UTIL, AtkUtilClass))


#ifndef _TYPEDEF_ATK_UTIL_
#define _TYPEDEF_ATK_UTIL_
typedef struct _AtkUtil      AtkUtil;
typedef struct _AtkUtilClass AtkUtilClass;
typedef struct _AtkKeyEventStruct AtkKeyEventStruct;
#endif

/**
 * AtkEventListener: 
 * @obj: An #AtkObject instance for whom the callback will be called when
 * the specified event (e.g. 'focus:') takes place.
 *
 * A function which is called when an object emits a matching event,
 * as used in #atk_add_focus_tracker.
 * Currently the only events for which object-specific handlers are
 * supported are events of type "focus:".  Most clients of ATK will prefer to 
 * attach signal handlers for the various ATK signals instead.
 *
 * @see: atk_add_focus_tracker.
 **/
typedef void  (*AtkEventListener) (AtkObject* obj);
/**
 * AtkEventListenerInit:
 *
 * An #AtkEventListenerInit function is a special function that is
 * called in order to initialize the per-object event registration system
 * used by #AtkEventListener, if any preparation is required.  
 *
 * @see: atk_focus_tracker_init.
 **/
typedef void  (*AtkEventListenerInit) (void);
/**
 * AtkKeySnoopFunc:
 * @event: an AtkKeyEventStruct containing information about the key event for which
 * notification is being given.
 * @func_data: a block of data which will be passed to the event listener, on notification.
 *
 * An #AtkKeySnoopFunc is a type of callback which is called whenever a key event occurs, 
 * if registered via atk_add_key_event_listener.  It allows for pre-emptive 
 * interception of key events via the return code as described below.
 *
 * Returns: TRUE (nonzero) if the event emission should be stopped and the event 
 * discarded without being passed to the normal GUI recipient; FALSE (zero) if the 
 * event dispatch to the client application should proceed as normal.
 *
 * @see: atk_add_key_event_listener.
 **/
typedef gint  (*AtkKeySnoopFunc)  (AtkKeyEventStruct *event,
				   gpointer func_data);

/**
 * AtkKeyEventStruct:
 * @type: An AtkKeyEventType, generally one of ATK_KEY_EVENT_PRESS or ATK_KEY_EVENT_RELEASE
 * @state: A bitmask representing the state of the modifier keys immediately after the event takes place.   
 * The meaning of the bits is currently defined to match the bitmask used by GDK in
 * GdkEventType.state, see 
 * http://developer.gnome.org/doc/API/2.0/gdk/gdk-Event-Structures.html#GdkEventKey
 * @keyval: A guint representing a keysym value corresponding to those used by GDK and X11: see
 * /usr/X11/include/keysymdef.h.
 * @length: The length of member #string.
 * @string: A string containing one of the following: either a string approximating the text that would 
 * result from this keypress, if the key is a control or graphic character, or a symbolic name for this keypress.
 * Alphanumeric and printable keys will have the symbolic key name in this string member, for instance "A". "0", 
 * "semicolon", "aacute".  Keypad keys have the prefix "KP".
 * @keycode: The raw hardware code that generated the key event.  This field is raraly useful.
 * @timestamp: A timestamp in milliseconds indicating when the event occurred.  
 * These timestamps are relative to a starting point which should be considered arbitrary, 
 * and only used to compare the dispatch times of events to one another.
 *
 * Encapsulates information about a key event.
 **/
struct _AtkKeyEventStruct {
  gint type;
  guint state;
  guint keyval;
  gint length;
  gchar *string;
  guint16 keycode;
  guint32 timestamp;	
};

/**
 *AtkKeyEventType:
 *@ATK_KEY_EVENT_PRESS: specifies a key press event
 *@ATK_KEY_EVENT_RELEASE: specifies a key release event
 *@ATK_KEY_EVENT_LAST_DEFINED: Not a valid value; specifies end of enumeration
 *
 *Specifies the type of a keyboard evemt.
 **/
typedef enum
{
  ATK_KEY_EVENT_PRESS,
  ATK_KEY_EVENT_RELEASE,
  ATK_KEY_EVENT_LAST_DEFINED
} AtkKeyEventType;

struct _AtkUtil
{
  GObject parent;
};

struct _AtkUtilClass
{
   GObjectClass parent;
   guint        (* add_global_event_listener)    (GSignalEmissionHook listener,
						  const gchar        *event_type);
   void         (* remove_global_event_listener) (guint               listener_id);
   guint	(* add_key_event_listener) 	 (AtkKeySnoopFunc     listener,
						  gpointer data);
   void         (* remove_key_event_listener)    (guint               listener_id);
   AtkObject*   (* get_root)                     (void);
   G_CONST_RETURN gchar* (* get_toolkit_name)    (void);
   G_CONST_RETURN gchar* (* get_toolkit_version) (void);
};
GType atk_util_get_type (void);

/**
 *AtkCoordType:
 *@ATK_XY_SCREEN: specifies xy coordinates relative to the screen
 *@ATK_XY_WINDOW: specifies xy coordinates relative to the widget's 
 * top-level window
 *
 *Specifies how xy coordinates are to be interpreted. Used by functions such
 *as atk_component_get_position() and atk_text_get_character_extents() 
 **/
typedef enum {
  ATK_XY_SCREEN,
  ATK_XY_WINDOW
}AtkCoordType;

/*
 * Adds the specified function to the list of functions to be called
 * when an object receives focus.
 */
guint    atk_add_focus_tracker     (AtkEventListener      focus_tracker);

/*
 * Removes the specified focus tracker from the list of function
 * to be called when any object receives focus
 */
void     atk_remove_focus_tracker  (guint                tracker_id);

/*
 * atk_focus_tracker_init:
 * @init: An #AtkEventListenerInit function to be called
 * prior to any focus-tracking requests.
 *
 * Specifies the function to be called for focus tracker initialization.
 * removal. This function should be called by an implementation of the
 * ATK interface if any specific work needs to be done to enable
 * focus tracking.
 */
void     atk_focus_tracker_init    (AtkEventListenerInit  init);

/*
 * Cause the focus tracker functions which have been specified to be
 * executed for the object.
 */
void     atk_focus_tracker_notify  (AtkObject            *object);

/*
 * Adds the specified function to the list of functions to be called
 * when an event of type event_type occurs.
 */
guint	atk_add_global_event_listener (GSignalEmissionHook listener,
				       const gchar        *event_type);

/*
 * Removes the specified event listener
 */
void	atk_remove_global_event_listener (guint listener_id);

/*
 * Adds the specified function to the list of functions to be called
 * when an keyboard event occurs.
 */
guint	atk_add_key_event_listener (AtkKeySnoopFunc listener, gpointer data);

/*
 * Removes the specified event listener
 */
void	atk_remove_key_event_listener (guint listener_id);

/*
 * Returns the root accessible container for the current application.
 */
AtkObject* atk_get_root(void);

AtkObject* atk_get_focus_object (void);

/*
 * Returns name string for the GUI toolkit.
 */
G_CONST_RETURN gchar *atk_get_toolkit_name (void);

/*
 * Returns version string for the GUI toolkit.
 */
G_CONST_RETURN gchar *atk_get_toolkit_version (void);

/*
 * Gets the current version of ATK
 */
G_CONST_RETURN gchar *atk_get_version (void);

/* --- GType boilerplate --- */
/* convenience macros for atk type implementations, which for a type GtkGadgetAccessible will:
 * - prototype: static void     gtk_gadget_accessible_class_init (GtkGadgetClass *klass);
 * - prototype: static void     gtk_gadget_accessible_init       (GtkGadget      *self);
 * - define:    static gpointer gtk_gadget_accessible_parent_class = NULL;
 *   gtk_gadget_accessible_parent_class is initialized prior to calling gtk_gadget_class_init()
 * - implement: GType           gtk_gadget_accessible_get_type (void) { ... }
 * - support custom code in gtk_gadget_accessible_get_type() after the type is registered.
 *
 * macro arguments: TypeName, type_name, TYPE_PARENT, CODE
 * example: ATK_DEFINE_TYPE_WITH_CODE (GtkGadgetAccessible, gtk_gadget_accessible, GTK_TYPE_GADGET,
 *                                     G_IMPLEMENT_INTERFACE (ATK_TYPE_TABLE, gtk_gadget_accessible_table_iface_init))
 */

#define ATK_DEFINE_TYPE(TN, t_n, T_P)			       ATK_DEFINE_TYPE_EXTENDED (TN, t_n, T_P, 0, {})
#define ATK_DEFINE_TYPE_WITH_CODE(TN, t_n, T_P, _C_)	      _ATK_DEFINE_TYPE_EXTENDED_BEGIN (TN, t_n, T_P, 0) {_C_;} _ATK_DEFINE_TYPE_EXTENDED_END()
#define ATK_DEFINE_ABSTRACT_TYPE(TN, t_n, T_P)		       ATK_DEFINE_TYPE_EXTENDED (TN, t_n, T_P, G_TYPE_FLAG_ABSTRACT, {})
#define ATK_DEFINE_ABSTRACT_TYPE_WITH_CODE(TN, t_n, T_P, _C_) _ATK_DEFINE_TYPE_EXTENDED_BEGIN (TN, t_n, T_P, G_TYPE_FLAG_ABSTRACT) {_C_;} _ATK_DEFINE_TYPE_EXTENDED_END()
#define ATK_DEFINE_TYPE_EXTENDED(TN, t_n, T_P, _f_, _C_)      _ATK_DEFINE_TYPE_EXTENDED_BEGIN (TN, t_n, T_P, _f_) {_C_;} _ATK_DEFINE_TYPE_EXTENDED_END()

#define _ATK_DEFINE_TYPE_EXTENDED_BEGIN(TypeName, type_name, TYPE, flags) \
\
static void     type_name##_init              (TypeName        *self); \
static void     type_name##_class_init        (TypeName##Class *klass); \
static gpointer type_name##_parent_class = NULL; \
static void     type_name##_class_intern_init (gpointer klass) \
{ \
  type_name##_parent_class = g_type_class_peek_parent (klass); \
  type_name##_class_init ((TypeName##Class*) klass); \
} \
\
GType \
type_name##_get_type (void) \
{ \
  static volatile gsize g_define_type_id__volatile = 0; \
  if (g_once_init_enter (&g_define_type_id__volatile))  \
    { \
      AtkObjectFactory *factory; \
      GType derived_type; \
      GTypeQuery query; \
      GType derived_atk_type; \
      GType g_define_type_id; \
\
      /* Figure out the size of the class and instance we are deriving from */ \
      derived_type = g_type_parent (TYPE); \
      factory = atk_registry_get_factory (atk_get_default_registry (), \
                                          derived_type); \
      derived_atk_type = atk_object_factory_get_accessible_type (factory); \
      g_type_query (derived_atk_type, &query); \
\
      g_define_type_id = \
        g_type_register_static_simple (derived_atk_type, \
                                       g_intern_static_string (#TypeName), \
                                       query.class_size, \
                                       (GClassInitFunc) type_name##_class_intern_init, \
                                       query.instance_size, \
                                       (GInstanceInitFunc) type_name##_init, \
                                       (GTypeFlags) flags); \
      { /* custom code follows */
#define _ATK_DEFINE_TYPE_EXTENDED_END()	\
        /* following custom code */	\
      }					\
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id); \
    }					\
  return g_define_type_id__volatile;	\
} /* closes type_name##_get_type() */

G_END_DECLS

#endif /* __ATK_UTIL_H__ */
