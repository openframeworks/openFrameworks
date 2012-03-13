/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *                    2003 Benjamin Otte <in7y118@public.uni-hamburg.de>
 *
 * gstinfo.h: debugging functions
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

#ifndef __GSTINFO_H__
#define __GSTINFO_H__

#include <glib.h>
#include <glib-object.h>
#include <gst/gstconfig.h>

G_BEGIN_DECLS

/**
 * GstDebugLevel:
 * @GST_LEVEL_NONE: No debugging level specified or desired. Used to deactivate
 *  debugging output.
 * @GST_LEVEL_ERROR: Error messages are to be used only when an error occured
 *  that stops the application from keeping working correctly.
 *  An examples is gst_element_error, which outputs a message with this priority.
 *  It does not mean that the application is terminating as with g_errror.
 * @GST_LEVEL_WARNING: Warning messages are to inform about abnormal behaviour
 *  that could lead to problems or weird behaviour later on. An example of this
 *  would be clocking issues ("your computer is pretty slow") or broken input
 *  data ("Can't synchronize to stream.")
 * @GST_LEVEL_INFO: Informational messages should be used to keep the developer
 *  updated about what is happening.
 *  Examples where this should be used are when a typefind function has
 *  successfully determined the type of the stream or when an mp3 plugin detects
 *  the format to be used. ("This file has mono sound.")
 * @GST_LEVEL_DEBUG: Debugging messages should be used when something common
 *  happens that is not the expected default behavior, or something that's
 *  useful to know but doesn't happen all the time (ie. per loop iteration or
 *  buffer processed or event handled).
 *  An example would be notifications about state changes or receiving/sending
 *  of events.
 * @GST_LEVEL_LOG: Log messages are messages that are very common but might be
 *  useful to know. As a rule of thumb a pipeline that is iterating as expected
 *  should never output anything else but LOG messages. Use this log level to
 *  log recurring information in chain functions and loop functions, for
 *  example.
 * @GST_LEVEL_FIXME: Fixme messages are messages that indicate that something
 *  in the executed code path is not fully implemented or handled yet. Note
 *  that this does not replace proper error handling in any way, the purpose
 *  of this message is to make it easier to spot incomplete/unfinished pieces
 *  of code when reading the debug log. (Since: 0.10.23)
 * @GST_LEVEL_TRACE: Tracing-related messages (Since: 0.10.30)
 *  Examples for this are referencing/dereferencing of objects.
 * @GST_LEVEL_MEMDUMP: memory dump messages are used to log (small) chunks of
 *  data as memory dumps in the log. They will be displayed as hexdump with
 *  ASCII characters. (Since: 0.10.23)
 * @GST_LEVEL_COUNT: The number of defined debugging levels.
 *
 * The level defines the importance of a debugging message. The more important a
 * message is, the greater the probability that the debugging system outputs it.
 */
typedef enum {
  GST_LEVEL_NONE = 0,
  GST_LEVEL_ERROR,
  GST_LEVEL_WARNING,
  GST_LEVEL_INFO,
  GST_LEVEL_DEBUG,
  GST_LEVEL_LOG,
  GST_LEVEL_FIXME = 6,
  GST_LEVEL_TRACE = 7,
  /* add more */
  GST_LEVEL_MEMDUMP = 9,
  /* add more */
  GST_LEVEL_COUNT
} GstDebugLevel;

/**
 * GST_LEVEL_DEFAULT:
 *
 * Defines the default debugging level to be used with GStreamer. It is normally
 * set to #GST_LEVEL_NONE so nothing get printed.
 * As it can be configured at compile time, developer builds may chose to
 * override that though.
 * You can use this as an argument to gst_debug_set_default_threshold() to
 * reset the debugging output to default behaviour.
 */
#ifndef GST_LEVEL_DEFAULT
#define GST_LEVEL_DEFAULT GST_LEVEL_NONE
#endif

/* defines for format (colors etc)
 * don't change them around, it uses terminal layout
 * Terminal color strings:
 * 00=none 01=bold 04=underscore 05=blink 07=reverse 08=concealed
 * Text color codes:
 * 30=black 31=red 32=green 33=yellow 34=blue 35=magenta 36=cyan 37=white
 * Background color codes:
 * 40=black 41=red 42=green 43=yellow 44=blue 45=magenta 46=cyan 47=white
 */
/**
 * GstDebugColorFlags:
 * @GST_DEBUG_FG_BLACK: Use black as foreground color.
 * @GST_DEBUG_FG_RED: Use red as foreground color.
 * @GST_DEBUG_FG_GREEN: Use green as foreground color.
 * @GST_DEBUG_FG_YELLOW: Use yellow as foreground color.
 * @GST_DEBUG_FG_BLUE: Use blue as foreground color.
 * @GST_DEBUG_FG_MAGENTA: Use magenta as foreground color.
 * @GST_DEBUG_FG_CYAN: Use cyan as foreground color.
 * @GST_DEBUG_FG_WHITE: Use white as foreground color.
 * @GST_DEBUG_BG_BLACK: Use black as background color.
 * @GST_DEBUG_BG_RED: Use red as background color.
 * @GST_DEBUG_BG_GREEN: Use green as background color.
 * @GST_DEBUG_BG_YELLOW: Use yellow as background color.
 * @GST_DEBUG_BG_BLUE: Use blue as background color.
 * @GST_DEBUG_BG_MAGENTA: Use magenta as background color.
 * @GST_DEBUG_BG_CYAN: Use cyan as background color.
 * @GST_DEBUG_BG_WHITE: Use white as background color.
 * @GST_DEBUG_BOLD: Make the output bold.
 * @GST_DEBUG_UNDERLINE: Underline the output.
 *
 * These are some terminal style flags you can use when creating your
 * debugging categories to make them stand out in debugging output.
 */
typedef enum {
  /* colors */
  GST_DEBUG_FG_BLACK		= 0x0000,
  GST_DEBUG_FG_RED		= 0x0001,
  GST_DEBUG_FG_GREEN		= 0x0002,
  GST_DEBUG_FG_YELLOW		= 0x0003,
  GST_DEBUG_FG_BLUE		= 0x0004,
  GST_DEBUG_FG_MAGENTA		= 0x0005,
  GST_DEBUG_FG_CYAN		= 0x0006,
  GST_DEBUG_FG_WHITE		= 0x0007,
  /* background colors */
  GST_DEBUG_BG_BLACK		= 0x0000,
  GST_DEBUG_BG_RED		= 0x0010,
  GST_DEBUG_BG_GREEN		= 0x0020,
  GST_DEBUG_BG_YELLOW		= 0x0030,
  GST_DEBUG_BG_BLUE		= 0x0040,
  GST_DEBUG_BG_MAGENTA		= 0x0050,
  GST_DEBUG_BG_CYAN		= 0x0060,
  GST_DEBUG_BG_WHITE		= 0x0070,
  /* other formats */
  GST_DEBUG_BOLD		= 0x0100,
  GST_DEBUG_UNDERLINE		= 0x0200
} GstDebugColorFlags;

#define GST_DEBUG_FG_MASK	(0x000F)
#define GST_DEBUG_BG_MASK	(0x00F0)
#define GST_DEBUG_FORMAT_MASK	(0xFF00)

typedef struct _GstDebugCategory GstDebugCategory;
/**
 * GstDebugCategory:
 *
 * This is the struct that describes the categories. Once initialized with
 * #GST_DEBUG_CATEGORY_INIT, its values can't be changed anymore.
 */
struct _GstDebugCategory {
  /*< private >*/
  gint                  threshold;
  guint			color;		/* see defines above */

  const gchar *		name;
  const gchar *		description;
};

/********** some convenience macros for debugging **********/

/**
 * GST_STR_NULL:
 * @str: The string to check.
 *
 * Macro to use when a string must not be NULL, but may be NULL. If the string
 * is NULL, "(NULL)" is printed instead.
 * In GStreamer printf string arguments may not be NULL, because on some
 * platforms (ie Solaris) the libc crashes in that case. This includes debugging
 * strings.
 */
#define GST_STR_NULL(str) ((str) ? (str) : "(NULL)")

/* FIXME, not MT safe */
/**
 * GST_DEBUG_PAD_NAME:
 * @pad: The pad to debug.
 *
 * Evaluates to 2 strings, that describe the pad. Often used in debugging
 * statements.
 */
#define GST_DEBUG_PAD_NAME(pad) \
  (pad != NULL) ?  \
  ((GST_OBJECT_PARENT(pad) != NULL) ? \
  GST_STR_NULL (GST_OBJECT_NAME (GST_OBJECT_PARENT(pad))) : \
  "''" ) : "''", \
  (pad != NULL) ? GST_STR_NULL (GST_OBJECT_NAME (pad)) : "''"

/**
 * GST_FUNCTION:
 *
 * This macro should evaluate to the name of the current function and be should
 * be defined when configuring your project, as it is compiler dependant. If it
 * is not defined, some default value is used. It is used to provide debugging
 * output with the function name of the message.
 *
 * Note that this is different from G_STRFUNC as we do not want the full
 * function signature in C++ code.
 */
#ifndef GST_FUNCTION
#if defined (__GNUC__) || (defined (_MSC_VER) && _MSC_VER >= 1300)
#  define GST_FUNCTION     ((const char*) (__FUNCTION__))
#elif defined (__STDC__) && defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  define GST_FUNCTION     ((const char*) (__func__))
#else
#  define GST_FUNCTION     ((const char*) ("???"))
#endif
#endif /* ifndef GST_FUNCTION */


typedef struct _GstDebugMessage GstDebugMessage;

/**
 * GstLogFunction:
 * @category: a #GstDebugCategory
 * @level: a #GstDebugLevel
 * @file: file name
 * @function: function name
 * @line: line number
 * @object: a #GObject
 * @message: the message
 * @data: user data for the log function
 *
 * Function prototype for a logging function that can be registered with
 * gst_debug_add_log_function().
 * Use G_GNUC_NO_INSTRUMENT on that function.
 */
typedef void (*GstLogFunction)  (GstDebugCategory * category,
                                 GstDebugLevel      level,
                                 const gchar      * file,
                                 const gchar      * function,
                                 gint               line,
                                 GObject          * object,
                                 GstDebugMessage  * message,
                                 gpointer           data);


/* FIXME 0.11: move this into private headers */
void            _gst_debug_init (void);


#ifdef GST_USING_PRINTF_EXTENSION

/* not using G_GNUC_PRINTF, since gcc will choke on GST_PTR_FORMAT being %P */
void		    gst_debug_log            (GstDebugCategory * category,
                                          GstDebugLevel      level,
                                          const gchar      * file,
                                          const gchar      * function,
                                          gint               line,
                                          GObject          * object,
                                          const gchar      * format,
                                          ...) G_GNUC_NO_INSTRUMENT;

#else /* GST_USING_PRINTF_EXTENSION */

void		    gst_debug_log            (GstDebugCategory * category,
                                          GstDebugLevel      level,
                                          const gchar      * file,
                                          const gchar      * function,
                                          gint               line,
                                          GObject          * object,
                                          const gchar      * format,
                                          ...) G_GNUC_PRINTF (7, 8) G_GNUC_NO_INSTRUMENT;

#endif /* GST_USING_PRINTF_EXTENSION */

void            gst_debug_log_valist     (GstDebugCategory * category,
                                          GstDebugLevel      level,
                                          const gchar      * file,
                                          const gchar      * function,
                                          gint	              line,
                                          GObject          * object,
                                          const gchar      * format,
                                          va_list            args) G_GNUC_NO_INSTRUMENT;

/* do not use this function, use the GST_DEBUG_CATEGORY_INIT macro */
GstDebugCategory *_gst_debug_category_new (const gchar * name,
                                           guint         color,
                                           const gchar * description);
/* do not use this function, use the GST_DEBUG_CATEGORY_GET macro */
GstDebugCategory *_gst_debug_get_category (const gchar *name);


/* do not use this function, use the GST_CAT_MEMDUMP_* macros */
void _gst_debug_dump_mem (GstDebugCategory * cat, const gchar * file,
    const gchar * func, gint line, GObject * obj, const gchar * msg,
    const guint8 * data, guint length);

/* we define this to avoid a compiler warning regarding a cast from a function
 * pointer to a void pointer
 * (see https://bugzilla.gnome.org/show_bug.cgi?id=309253)
 */
typedef	void (* GstDebugFuncPtr)	(void);

/* do no use these functions, use the GST_DEBUG*_FUNCPTR macros */
void	_gst_debug_register_funcptr	(GstDebugFuncPtr	func,
					 const gchar *		ptrname);
G_CONST_RETURN gchar *
	_gst_debug_nameof_funcptr	(GstDebugFuncPtr	func) G_GNUC_NO_INSTRUMENT;


const gchar   * gst_debug_message_get    (GstDebugMessage  * message);

void            gst_debug_log_default    (GstDebugCategory * category,
                                          GstDebugLevel      level,
                                          const gchar      * file,
                                          const gchar      * function,
                                          gint               line,
                                          GObject          * object,
                                          GstDebugMessage  * message,
                                          gpointer           unused) G_GNUC_NO_INSTRUMENT;

G_CONST_RETURN gchar *
                gst_debug_level_get_name (GstDebugLevel level);

void            gst_debug_add_log_function            (GstLogFunction func,
                                                       gpointer       data);
guint           gst_debug_remove_log_function         (GstLogFunction func);
guint           gst_debug_remove_log_function_by_data (gpointer       data);

void            gst_debug_set_active  (gboolean active);
gboolean        gst_debug_is_active   (void);

void            gst_debug_set_colored (gboolean colored);
gboolean        gst_debug_is_colored  (void);

void            gst_debug_set_default_threshold      (GstDebugLevel level);
GstDebugLevel   gst_debug_get_default_threshold      (void);
void            gst_debug_set_threshold_for_name     (const gchar * name,
                                                      GstDebugLevel level);
void            gst_debug_unset_threshold_for_name   (const gchar * name);


void            gst_debug_category_free              (GstDebugCategory *	category);
void	            gst_debug_category_set_threshold     (GstDebugCategory *	category,
                                                      GstDebugLevel		level);
void            gst_debug_category_reset_threshold   (GstDebugCategory *	category);
GstDebugLevel   gst_debug_category_get_threshold     (GstDebugCategory *	category);
G_CONST_RETURN gchar *
	            gst_debug_category_get_name          (GstDebugCategory *	category);
guint           gst_debug_category_get_color         (GstDebugCategory *	category);
G_CONST_RETURN gchar *
                gst_debug_category_get_description   (GstDebugCategory *	category);
GSList *        gst_debug_get_all_categories	(void);


gchar * gst_debug_construct_term_color (guint colorinfo);
gint    gst_debug_construct_win_color  (guint colorinfo);


#ifndef GST_DISABLE_GST_DEBUG

/**
 * GST_DEBUG_CATEGORY:
 * @cat: the category
 *
 * Defines a GstDebugCategory variable.
 * This macro expands to nothing if debugging is disabled.
 */
#define GST_DEBUG_CATEGORY(cat) GstDebugCategory *cat = NULL
/**
 * GST_DEBUG_CATEGORY_EXTERN:
 * @cat: the category
 *
 * Declares a GstDebugCategory variable as extern. Use in header files.
 * This macro expands to nothing if debugging is disabled.
 */
#define GST_DEBUG_CATEGORY_EXTERN(cat) extern GstDebugCategory *cat

/**
 * GST_DEBUG_CATEGORY_STATIC:
 * @cat: the category
 *
 * Defines a static GstDebugCategory variable.
 * This macro expands to nothing if debugging is disabled.
 */
#define GST_DEBUG_CATEGORY_STATIC(cat) static GstDebugCategory *cat = NULL

/**
 * GST_DEBUG_CATEGORY_INIT:
 * @cat: the category to initialize.
 * @name: the name of the category.
 * @color: the colors to use for a color representation or 0 for no color.
 * @description: optional description of the category.
 *
 * Initializes a new #GstDebugCategory with the given properties and set to
 * the default threshold.
 *
 * <note>
 * <para>
 * This macro expands to nothing if debugging is disabled.
 * </para>
 * <para>
 * When naming your category, please follow the following conventions to ensure
 * that the pattern matching for categories works as expected. It is not
 * earth-shattering if you don't follow these conventions, but it would be nice
 * for everyone.
 * </para>
 * <para>
 * If you define a category for a plugin or a feature of it, name the category
 * like the feature. So if you wanted to write a "filesrc" element, you would
 * name the category "filesrc". Use lowercase letters only.
 * If you define more than one category for the same element, append an
 * underscore and an identifier to your categories, like this: "filesrc_cache"
 * </para>
 * <para>
 * If you create a library or an application using debugging categories, use a
 * common prefix followed by an underscore for all your categories. GStreamer
 * uses the GST prefix so GStreamer categories look like "GST_STATES". Be sure
 * to include uppercase letters.
 * </para>
 * </note>
 */
#define GST_DEBUG_CATEGORY_INIT(cat,name,color,description) G_STMT_START{\
  if (cat == NULL)							\
    cat = _gst_debug_category_new (name,color,description);		\
}G_STMT_END

/**
 * GST_DEBUG_CATEGORY_GET:
 * @cat: the category to initialize.
 * @name: log category name
 *
 * Looks up an existing #GstDebugCategory by its @name and sets @cat. If the
 * category is not found, but GST_CAT_DEFAULT is defined, that is assigned to
 * @cat. Otherwise @cat will be NULL.
 *
 * |[
 * GST_DEBUG_CATEGORY_STATIC (gst_myplugin_debug);
 * #define GST_CAT_DEFAULT gst_myplugin_debug
 * GST_DEBUG_CATEGORY_STATIC (GST_CAT_PERFORMANCE);
 * ...
 * GST_DEBUG_CATEGORY_INIT (gst_myplugin_debug, "myplugin", 0, "nice element");
 * GST_DEBUG_CATEGORY_GET (GST_CAT_PERFORMANCE, "GST_PERFORMANCE");
 * ]|
 *
 * Since: 0.10.24
 */
#ifdef GST_CAT_DEFAULT
#define GST_DEBUG_CATEGORY_GET(cat,name)  G_STMT_START{\
  cat = _gst_debug_get_category (name);			\
  if (!cat) {						\
    cat = GST_CAT_DEFAULT;				\
  }							\
}G_STMT_END
#else
#define GST_DEBUG_CATEGORY_GET(cat,name)  G_STMT_START{\
  cat = _gst_debug_get_category (name);			\
}G_STMT_END
#endif

/**
 * GST_CAT_DEFAULT:
 *
 * Default gstreamer core debug log category. Please define your own.
 */
GST_EXPORT GstDebugCategory *	GST_CAT_DEFAULT;
/* this symbol may not be used */
extern gboolean			__gst_debug_enabled;

/* since 0.10.7, the min debug level, used for quickly discarding debug
 * messages that fall under the threshold. */
GST_EXPORT GstDebugLevel            __gst_debug_min; 

/**
 * GST_CAT_LEVEL_LOG:
 * @cat: category to use
 * @level: the severity of the message
 * @object: the #GObject the message belongs to or NULL if none
 * @...: A printf-style message to output
 *
 * Outputs a debugging message. This is the most general macro for outputting
 * debugging messages. You will probably want to use one of the ones described
 * below.
 */
#ifdef G_HAVE_ISO_VARARGS
#define GST_CAT_LEVEL_LOG(cat,level,object,...) G_STMT_START{		\
  if (G_UNLIKELY (level <= __gst_debug_min)) {						\
    gst_debug_log ((cat), (level), __FILE__, GST_FUNCTION, __LINE__,	\
        (GObject *) (object), __VA_ARGS__);				\
  }									\
}G_STMT_END
#else /* G_HAVE_GNUC_VARARGS */
#ifdef G_HAVE_GNUC_VARARGS
#define GST_CAT_LEVEL_LOG(cat,level,object,args...) G_STMT_START{	\
  if (G_UNLIKELY (level <= __gst_debug_min)) {						\
    gst_debug_log ((cat), (level), __FILE__, GST_FUNCTION, __LINE__,	\
        (GObject *) (object), ##args );					\
  }									\
}G_STMT_END
#else /* no variadic macros, use inline */
static inline void
GST_CAT_LEVEL_LOG_valist (GstDebugCategory * cat,
    GstDebugLevel level, gpointer object, const char *format, va_list varargs)
{
  if (G_UNLIKELY (level <= __gst_debug_min)) {
    gst_debug_log_valist (cat, level, "", "", 0, (GObject *) object, format,
        varargs);
  }
}

static inline void
GST_CAT_LEVEL_LOG (GstDebugCategory * cat, GstDebugLevel level,
    gpointer object, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, level, object, format, varargs);
  va_end (varargs);
}
#endif
#endif /* G_HAVE_ISO_VARARGS */

/* This one doesn't have varargs in the macro, so it's different than all the
 * other macros and hence in a separate block right here. Docs chunks are
 * with the other doc chunks below though. */
#define __GST_CAT_MEMDUMP_LOG(cat,object,msg,data,length) G_STMT_START{       \
  if (G_UNLIKELY (GST_LEVEL_MEMDUMP <= __gst_debug_min)) {                    \
    _gst_debug_dump_mem ((cat), __FILE__, GST_FUNCTION, __LINE__,             \
        (GObject *) (object), (msg), (data), (length));                       \
  }                                                                           \
}G_STMT_END

#define GST_CAT_MEMDUMP_OBJECT(cat,obj,msg,data,length)  \
    __GST_CAT_MEMDUMP_LOG(cat,obj,msg,data,length)
#define GST_CAT_MEMDUMP(cat,msg,data,length)             \
    __GST_CAT_MEMDUMP_LOG(cat,NULL,msg,data,length)
#define GST_MEMDUMP_OBJECT(obj,msg,data,length)          \
    __GST_CAT_MEMDUMP_LOG(GST_CAT_DEFAULT,obj,msg,data,length)
#define GST_MEMDUMP(msg,data,length)                     \
    __GST_CAT_MEMDUMP_LOG(GST_CAT_DEFAULT,NULL,msg,data,length)

/**
 * GST_CAT_ERROR_OBJECT:
 * @cat: category to use
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output an error message belonging to the given object in the given category.
 */
/**
 * GST_CAT_WARNING_OBJECT:
 * @cat: category to use
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output a warning message belonging to the given object in the given category.
 */
/**
 * GST_CAT_INFO_OBJECT:
 * @cat: category to use
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output an informational message belonging to the given object in the given
 * category.
 */
/**
 * GST_CAT_DEBUG_OBJECT:
 * @cat: category to use
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output an debugging message belonging to the given object in the given category.
 */
/**
 * GST_CAT_LOG_OBJECT:
 * @cat: category to use
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output an logging message belonging to the given object in the given category.
 */
/**
 * GST_CAT_FIXME_OBJECT:
 * @cat: category to use
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output a fixme message belonging to the given object in the given category.
 *
 * Since: 0.10.23
 */
/**
 * GST_CAT_TRACE_OBJECT:
 * @cat: category to use
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output a tracing message belonging to the given object in the given
 * category.
 *
 * Since: 0.10.30
 */
/**
 * GST_CAT_MEMDUMP_OBJECT:
 * @cat: category to use
 * @obj: the #GObject the message belongs to
 * @msg: message string to log with the data
 * @data: pointer to the data to output
 * @length: length of the data to output
 *
 * Output a hexdump of @data relating to the given object in the given
 * category.
 *
 * Since: 0.10.23
 */


/**
 * GST_CAT_ERROR:
 * @cat: category to use
 * @...: printf-style message to output
 *
 * Output an error message in the given category.
 */
/**
 * GST_CAT_WARNING:
 * @cat: category to use
 * @...: printf-style message to output
 *
 * Output an warning message in the given category.
 */
/**
 * GST_CAT_INFO:
 * @cat: category to use
 * @...: printf-style message to output
 *
 * Output an informational message in the given category.
 */
/**
 * GST_CAT_DEBUG:
 * @cat: category to use
 * @...: printf-style message to output
 *
 * Output an debugging message in the given category.
 */
/**
 * GST_CAT_LOG:
 * @cat: category to use
 * @...: printf-style message to output
 *
 * Output an logging message in the given category.
 */
/**
 * GST_CAT_FIXME:
 * @cat: category to use
 * @...: printf-style message to output
 *
 * Output an fixme message in the given category.
 *
 * Since: 0.10.23
 */
/**
 * GST_CAT_TRACE:
 * @cat: category to use
 * @...: printf-style message to output
 *
 * Output a tracing message in the given category.
 *
 * Since: 0.10.30
 */
/**
 * GST_CAT_MEMDUMP:
 * @cat: category to use
 * @msg: message string to log with the data
 * @data: pointer to the data to output
 * @length: length of the data to output
 *
 * Output a hexdump of @data in the given category.
 *
 * Since: 0.10.23
 */


/**
 * GST_ERROR_OBJECT:
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output an error message belonging to the given object in the default category.
 */
/**
 * GST_WARNING_OBJECT:
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output a warning message belonging to the given object in the default category.
 */
/**
 * GST_INFO_OBJECT:
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output an informational message belonging to the given object in the default
 * category.
 */
/**
 * GST_DEBUG_OBJECT:
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output a debugging message belonging to the given object in the default
 * category.
 */
/**
 * GST_LOG_OBJECT:
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output a logging message belonging to the given object in the default category.
 */
/**
 * GST_FIXME_OBJECT:
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output a fixme message belonging to the given object in the default category.
 *
 * Since: 0.10.23
 */
/**
 * GST_TRACE_OBJECT:
 * @obj: the #GObject the message belongs to
 * @...: printf-style message to output
 *
 * Output a tracing message belonging to the given object in the default category.
 *
 * Since: 0.10.30
 */
/**
 * GST_MEMDUMP_OBJECT:
 * @obj: the #GObject the message belongs to
 * @msg: message string to log with the data
 * @data: pointer to the data to output
 * @length: length of the data to output
 *
 * Output a logging message belonging to the given object in the default category.
 *
 * Since: 0.10.23
 */


/**
 * GST_ERROR:
 * @...: printf-style message to output
 *
 * Output an error message in the default category.
 */
/**
 * GST_WARNING:
 * @...: printf-style message to output
 *
 * Output a warning message in the default category.
 */
/**
 * GST_INFO:
 * @...: printf-style message to output
 *
 * Output an informational message in the default category.
 */
/**
 * GST_DEBUG:
 * @...: printf-style message to output
 *
 * Output a debugging message in the default category.
 */
/**
 * GST_LOG:
 * @...: printf-style message to output
 *
 * Output a logging message in the default category.
 */
/**
 * GST_FIXME:
 * @...: printf-style message to output
 *
 * Output a fixme message in the default category.
 *
 * Since: 0.10.23
 */
/**
 * GST_TRACE:
 * @...: printf-style message to output
 *
 * Output a tracing message in the default category.
 *
 * Since: 0.10.30
 */
/**
 * GST_MEMDUMP:
 * @msg: message string to log with the data
 * @data: pointer to the data to output
 * @length: length of the data to output
 *
 * Output a hexdump of @data.
 *
 * Since: 0.10.23
 */

#ifdef G_HAVE_ISO_VARARGS

#define GST_CAT_ERROR_OBJECT(cat,obj,...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_ERROR,   obj,  __VA_ARGS__)
#define GST_CAT_WARNING_OBJECT(cat,obj,...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_WARNING, obj,  __VA_ARGS__)
#define GST_CAT_INFO_OBJECT(cat,obj,...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_INFO,    obj,  __VA_ARGS__)
#define GST_CAT_DEBUG_OBJECT(cat,obj,...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_DEBUG,   obj,  __VA_ARGS__)
#define GST_CAT_LOG_OBJECT(cat,obj,...)		GST_CAT_LEVEL_LOG (cat, GST_LEVEL_LOG,     obj,  __VA_ARGS__)
#define GST_CAT_FIXME_OBJECT(cat,obj,...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_FIXME,   obj,  __VA_ARGS__)
#define GST_CAT_TRACE_OBJECT(cat,obj,...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_TRACE,   obj,  __VA_ARGS__)

#define GST_CAT_ERROR(cat,...)			GST_CAT_LEVEL_LOG (cat, GST_LEVEL_ERROR,   NULL, __VA_ARGS__)
#define GST_CAT_WARNING(cat,...)		GST_CAT_LEVEL_LOG (cat, GST_LEVEL_WARNING, NULL, __VA_ARGS__)
#define GST_CAT_INFO(cat,...)			GST_CAT_LEVEL_LOG (cat, GST_LEVEL_INFO,    NULL, __VA_ARGS__)
#define GST_CAT_DEBUG(cat,...)			GST_CAT_LEVEL_LOG (cat, GST_LEVEL_DEBUG,   NULL, __VA_ARGS__)
#define GST_CAT_LOG(cat,...)			GST_CAT_LEVEL_LOG (cat, GST_LEVEL_LOG,     NULL, __VA_ARGS__)
#define GST_CAT_FIXME(cat,...)			GST_CAT_LEVEL_LOG (cat, GST_LEVEL_FIXME,   NULL, __VA_ARGS__)
#define GST_CAT_TRACE(cat,...)		GST_CAT_LEVEL_LOG (cat, GST_LEVEL_TRACE,   NULL, __VA_ARGS__)

#define GST_ERROR_OBJECT(obj,...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_ERROR,   obj,  __VA_ARGS__)
#define GST_WARNING_OBJECT(obj,...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_WARNING, obj,  __VA_ARGS__)
#define GST_INFO_OBJECT(obj,...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_INFO,    obj,  __VA_ARGS__)
#define GST_DEBUG_OBJECT(obj,...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_DEBUG,   obj,  __VA_ARGS__)
#define GST_LOG_OBJECT(obj,...)		GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_LOG,     obj,  __VA_ARGS__)
#define GST_FIXME_OBJECT(obj,...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_FIXME,   obj,  __VA_ARGS__)
#define GST_TRACE_OBJECT(obj,...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_TRACE,   obj,  __VA_ARGS__)

#define GST_ERROR(...)			GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_ERROR,   NULL, __VA_ARGS__)
#define GST_WARNING(...)		GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_WARNING, NULL, __VA_ARGS__)
#define GST_INFO(...)			GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_INFO,    NULL, __VA_ARGS__)
#define GST_DEBUG(...)			GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_DEBUG,   NULL, __VA_ARGS__)
#define GST_LOG(...)			GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_LOG,     NULL, __VA_ARGS__)
#define GST_FIXME(...)			GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_FIXME,   NULL, __VA_ARGS__)
#define GST_TRACE(...)		GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_TRACE,   NULL, __VA_ARGS__)

#else
#ifdef G_HAVE_GNUC_VARARGS

#define GST_CAT_ERROR_OBJECT(cat,obj,args...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_ERROR,   obj,  ##args )
#define GST_CAT_WARNING_OBJECT(cat,obj,args...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_WARNING, obj,  ##args )
#define GST_CAT_INFO_OBJECT(cat,obj,args...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_INFO,    obj,  ##args )
#define GST_CAT_DEBUG_OBJECT(cat,obj,args...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_DEBUG,   obj,  ##args )
#define GST_CAT_LOG_OBJECT(cat,obj,args...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_LOG,     obj,  ##args )
#define GST_CAT_FIXME_OBJECT(cat,obj,args...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_FIXME,   obj,  ##args )
#define GST_CAT_TRACE_OBJECT(cat,obj,args...)	GST_CAT_LEVEL_LOG (cat, GST_LEVEL_TRACE,   obj,  ##args )

#define GST_CAT_ERROR(cat,args...)		GST_CAT_LEVEL_LOG (cat, GST_LEVEL_ERROR,   NULL, ##args )
#define GST_CAT_WARNING(cat,args...)		GST_CAT_LEVEL_LOG (cat, GST_LEVEL_WARNING, NULL, ##args )
#define GST_CAT_INFO(cat,args...)		GST_CAT_LEVEL_LOG (cat, GST_LEVEL_INFO,    NULL, ##args )
#define GST_CAT_DEBUG(cat,args...)		GST_CAT_LEVEL_LOG (cat, GST_LEVEL_DEBUG,   NULL, ##args )
#define GST_CAT_LOG(cat,args...)		GST_CAT_LEVEL_LOG (cat, GST_LEVEL_LOG,     NULL, ##args )
#define GST_CAT_FIXME(cat,args...)		GST_CAT_LEVEL_LOG (cat, GST_LEVEL_FIXME,   NULL, ##args )
#define GST_CAT_TRACE(cat,args...)		GST_CAT_LEVEL_LOG (cat, GST_LEVEL_TRACE,   NULL, ##args )

#define GST_ERROR_OBJECT(obj,args...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_ERROR,   obj,  ##args )
#define GST_WARNING_OBJECT(obj,args...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_WARNING, obj,  ##args )
#define GST_INFO_OBJECT(obj,args...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_INFO,    obj,  ##args )
#define GST_DEBUG_OBJECT(obj,args...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_DEBUG,   obj,  ##args )
#define GST_LOG_OBJECT(obj,args...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_LOG,     obj,  ##args )
#define GST_FIXME_OBJECT(obj,args...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_FIXME,   obj,  ##args )
#define GST_TRACE_OBJECT(obj,args...)	GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_TRACE,   obj,  ##args )

#define GST_ERROR(args...)		GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_ERROR,   NULL, ##args )
#define GST_WARNING(args...)		GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_WARNING, NULL, ##args )
#define GST_INFO(args...)		GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_INFO,    NULL, ##args )
#define GST_DEBUG(args...)		GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_DEBUG,   NULL, ##args )
#define GST_LOG(args...)		GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_LOG,     NULL, ##args )
#define GST_FIXME(args...)		GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_FIXME,   NULL, ##args )
#define GST_TRACE(args...)		GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_TRACE,   NULL, ##args )

#else
/* no variadic macros, use inline */
static inline void
GST_CAT_ERROR_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_ERROR, obj, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_WARNING_OBJECT (GstDebugCategory * cat, gpointer obj,
    const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_WARNING, obj, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_INFO_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_INFO, obj, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_DEBUG_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_DEBUG, obj, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_LOG_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_LOG, obj, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_FIXME_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_FIXME, obj, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_TRACE_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_TRACE, obj, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_ERROR (GstDebugCategory * cat, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_ERROR, NULL, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_WARNING (GstDebugCategory * cat, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_WARNING, NULL, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_INFO (GstDebugCategory * cat, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_INFO, NULL, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_DEBUG (GstDebugCategory * cat, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_DEBUG, NULL, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_LOG (GstDebugCategory * cat, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_LOG, NULL, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_FIXME (GstDebugCategory * cat, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_FIXME, NULL, format, varargs);
  va_end (varargs);
}

static inline void
GST_CAT_TRACE (GstDebugCategory * cat, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (cat, GST_LEVEL_TRACE, NULL, format, varargs);
  va_end (varargs);
}

static inline void
GST_ERROR_OBJECT (gpointer obj, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_ERROR, obj, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_WARNING_OBJECT (gpointer obj, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_WARNING, obj, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_INFO_OBJECT (gpointer obj, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_INFO, obj, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_DEBUG_OBJECT (gpointer obj, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_DEBUG, obj, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_LOG_OBJECT (gpointer obj, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_LOG, obj, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_FIXME_OBJECT (gpointer obj, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_FIXME, obj, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_TRACE_OBJECT (gpointer obj, const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_TRACE, obj, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_ERROR (const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_ERROR, NULL, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_WARNING (const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_WARNING, NULL, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_INFO (const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_INFO, NULL, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_DEBUG (const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_DEBUG, NULL, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_LOG (const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_LOG, NULL,
      format, varargs);
  va_end (varargs);
}

static inline void
GST_FIXME (const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_FIXME, NULL, format,
      varargs);
  va_end (varargs);
}

static inline void
GST_TRACE (const char *format, ...)
{
  va_list varargs;

  va_start (varargs, format);
  GST_CAT_LEVEL_LOG_valist (GST_CAT_DEFAULT, GST_LEVEL_TRACE, NULL, format,
      varargs);
  va_end (varargs);
}
#endif
#endif


/********** function pointer stuff **********/

/**
 * GST_DEBUG_REGISTER_FUNCPTR:
 * @ptr: pointer to the function to register
 *
 * Register a pointer to a function with its name, so it can later be used by
 * GST_DEBUG_FUNCPTR_NAME().
 *
 * Use this variant of #GST_DEBUG_FUNCPTR if you do not need to use @ptr.
 *
 * Since: 0.10.26
 */
#define GST_DEBUG_REGISTER_FUNCPTR(ptr) \
  _gst_debug_register_funcptr((GstDebugFuncPtr)(ptr), #ptr)
/**
 * GST_DEBUG_FUNCPTR:
 * @ptr: pointer to the function to register
 *
 * Register a pointer to a function with its name, so it can later be used by
 * GST_DEBUG_FUNCPTR_NAME().
 *
 * Returns: the value passed to @ptr.
 */
#define GST_DEBUG_FUNCPTR(ptr) \
  (_gst_debug_register_funcptr((GstDebugFuncPtr)(ptr), #ptr) , ptr)

/**
 * GST_DEBUG_FUNCPTR_NAME:
 * @ptr: address of the function of which to look up the name
 *
 * Retrieves the name of the function, if it was previously registered with
 * GST_DEBUG_FUNCPTR(). If not, it returns a description of the pointer.
 *
 * This macro returns a constant string which must not be modified or
 * freed by the caller.
 */
#define GST_DEBUG_FUNCPTR_NAME(ptr) \
  _gst_debug_nameof_funcptr((GstDebugFuncPtr)ptr)


#else /* GST_DISABLE_GST_DEBUG */


#ifndef GST_INFO_C

#if defined(__GNUC__) && __GNUC__ >= 3
#  pragma GCC poison gst_debug_log
#  pragma GCC poison gst_debug_log_valist
#  pragma GCC poison _gst_debug_category_new
#endif

#define __gst_debug_min GST_LEVEL_NONE

#define _gst_debug_init()				G_STMT_START{ }G_STMT_END

#define gst_debug_set_default_threshold(level)		G_STMT_START{ }G_STMT_END
#define gst_debug_get_default_threshold()		(GST_LEVEL_NONE)

#define gst_debug_level_get_name(level)				("NONE")
#define gst_debug_message_get(message)  			("")
#define gst_debug_add_log_function(func,data)		G_STMT_START{ }G_STMT_END
#define gst_debug_set_active(active)			G_STMT_START{ }G_STMT_END
#define gst_debug_is_active()				(FALSE)
#define gst_debug_set_colored(colored)			G_STMT_START{ }G_STMT_END
#define gst_debug_is_colored()				(FALSE)
#define gst_debug_set_default_threshold(level)		G_STMT_START{ }G_STMT_END
#define gst_debug_get_default_threshold()		(GST_LEVEL_NONE)
#define gst_debug_set_threshold_for_name(name,level)	G_STMT_START{ }G_STMT_END
#define gst_debug_unset_threshold_for_name(name)	G_STMT_START{ }G_STMT_END

/* we are using dummy function prototypes here to eat ';' as these macros are
 * used outside of functions */
#define GST_DEBUG_CATEGORY(var)				void _gst_debug_dummy_##var (void)
#define GST_DEBUG_CATEGORY_EXTERN(var)			void _gst_debug_dummy_extern_##var (void)
#define GST_DEBUG_CATEGORY_STATIC(var)			void _gst_debug_dummy_static_##var (void)

#define GST_DEBUG_CATEGORY_INIT(var,name,color,desc)	G_STMT_START{ }G_STMT_END
#define GST_DEBUG_CATEGORY_GET(var,name)		G_STMT_START{ }G_STMT_END
#define gst_debug_category_free(category)		G_STMT_START{ }G_STMT_END
#define gst_debug_category_set_threshold(category,level) G_STMT_START{ }G_STMT_END
#define gst_debug_category_reset_threshold(category)	G_STMT_START{ }G_STMT_END
#define gst_debug_category_get_threshold(category)	(GST_LEVEL_NONE)
#define gst_debug_category_get_name(cat)		("")
#define gst_debug_category_get_color(cat)		(0)
#define gst_debug_category_get_description(cat)		("")
#define gst_debug_get_all_categories()			(NULL)
#define gst_debug_construct_term_color(colorinfo)	(g_strdup ("00"))
#define gst_debug_construct_win_color(colorinfo)	(0)

#endif /* !GST_INFO_C */

#ifdef G_HAVE_ISO_VARARGS

#define GST_CAT_LEVEL_LOG(cat,level,...)		G_STMT_START{ }G_STMT_END

#define GST_CAT_ERROR_OBJECT(...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_WARNING_OBJECT(...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_INFO_OBJECT(...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_DEBUG_OBJECT(...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_LOG_OBJECT(...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_FIXME_OBJECT(...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_TRACE_OBJECT(...)			G_STMT_START{ }G_STMT_END

#define GST_CAT_ERROR(...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_WARNING(...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_INFO(...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_DEBUG(...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_LOG(...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_FIXME(...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_TRACE(...)				G_STMT_START{ }G_STMT_END

#define GST_ERROR_OBJECT(...)				G_STMT_START{ }G_STMT_END
#define GST_WARNING_OBJECT(...)				G_STMT_START{ }G_STMT_END
#define GST_INFO_OBJECT(...)				G_STMT_START{ }G_STMT_END
#define GST_DEBUG_OBJECT(...)				G_STMT_START{ }G_STMT_END
#define GST_LOG_OBJECT(...)				G_STMT_START{ }G_STMT_END
#define GST_FIXME_OBJECT(...)				G_STMT_START{ }G_STMT_END
#define GST_TRACE_OBJECT(...)				G_STMT_START{ }G_STMT_END

#define GST_ERROR(...)					G_STMT_START{ }G_STMT_END
#define GST_WARNING(...)				G_STMT_START{ }G_STMT_END
#define GST_INFO(...)					G_STMT_START{ }G_STMT_END
#define GST_DEBUG(...)					G_STMT_START{ }G_STMT_END
#define GST_LOG(...)					G_STMT_START{ }G_STMT_END
#define GST_FIXME(...)					G_STMT_START{ }G_STMT_END
#define GST_TRACE(...)					G_STMT_START{ }G_STMT_END

#else /* !G_HAVE_ISO_VARARGS */
#ifdef G_HAVE_GNUC_VARARGS

#define GST_CAT_LEVEL_LOG(cat,level,args...)		G_STMT_START{ }G_STMT_END

#define GST_CAT_ERROR_OBJECT(args...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_WARNING_OBJECT(args...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_INFO_OBJECT(args...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_DEBUG_OBJECT(args...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_LOG_OBJECT(args...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_FIXME_OBJECT(args...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_TRACE_OBJECT(args...)			G_STMT_START{ }G_STMT_END

#define GST_CAT_ERROR(args...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_WARNING(args...)			G_STMT_START{ }G_STMT_END
#define GST_CAT_INFO(args...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_DEBUG(args...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_LOG(args...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_FIXME(args...)				G_STMT_START{ }G_STMT_END
#define GST_CAT_TRACE(args...)				G_STMT_START{ }G_STMT_END

#define GST_ERROR_OBJECT(args...)			G_STMT_START{ }G_STMT_END
#define GST_WARNING_OBJECT(args...)			G_STMT_START{ }G_STMT_END
#define GST_INFO_OBJECT(args...)			G_STMT_START{ }G_STMT_END
#define GST_DEBUG_OBJECT(args...)			G_STMT_START{ }G_STMT_END
#define GST_LOG_OBJECT(args...)				G_STMT_START{ }G_STMT_END
#define GST_FIXME_OBJECT(args...)			G_STMT_START{ }G_STMT_END
#define GST_TRACE_OBJECT(args...)			G_STMT_START{ }G_STMT_END

#define GST_ERROR(args...)				G_STMT_START{ }G_STMT_END
#define GST_WARNING(args...)				G_STMT_START{ }G_STMT_END
#define GST_INFO(args...)				G_STMT_START{ }G_STMT_END
#define GST_DEBUG(args...)				G_STMT_START{ }G_STMT_END
#define GST_LOG(args...)				G_STMT_START{ }G_STMT_END
#define GST_FIXME(args...)				G_STMT_START{ }G_STMT_END
#define GST_TRACE(args...)				G_STMT_START{ }G_STMT_END

#else /* !G_HAVE_GNUC_VARARGS */
static inline void
GST_CAT_LEVEL_LOG_valist (GstDebugCategory * cat,
    GstDebugLevel level, gpointer object, const char *format, va_list varargs)
{
}

static inline void
GST_CAT_ERROR_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
}

static inline void
GST_CAT_WARNING_OBJECT (GstDebugCategory * cat, gpointer obj,
    const char *format, ...)
{
}

static inline void
GST_CAT_INFO_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
}

static inline void
GST_CAT_DEBUG_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
}

static inline void
GST_CAT_LOG_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
}

static inline void
GST_CAT_FIXME_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
}

static inline void
GST_CAT_TRACE_OBJECT (GstDebugCategory * cat, gpointer obj, const char *format,
    ...)
{
}

static inline void
GST_CAT_ERROR (GstDebugCategory * cat, const char *format, ...)
{
}

static inline void
GST_CAT_WARNING (GstDebugCategory * cat, const char *format, ...)
{
}

static inline void
GST_CAT_INFO (GstDebugCategory * cat, const char *format, ...)
{
}

static inline void
GST_CAT_DEBUG (GstDebugCategory * cat, const char *format, ...)
{
}

static inline void
GST_CAT_LOG (GstDebugCategory * cat, const char *format, ...)
{
}

static inline void
GST_CAT_FIXME (GstDebugCategory * cat, const char *format, ...)
{
}

static inline void
GST_CAT_TRACE (GstDebugCategory * cat, const char *format, ...)
{
}

static inline void
GST_ERROR_OBJECT (gpointer obj, const char *format, ...)
{
}

static inline void
GST_WARNING_OBJECT (gpointer obj, const char *format, ...)
{
}

static inline void
GST_INFO_OBJECT (gpointer obj, const char *format, ...)
{
}

static inline void
GST_DEBUG_OBJECT (gpointer obj, const char *format, ...)
{
}

static inline void
GST_LOG_OBJECT (gpointer obj, const char *format, ...)
{
}

static inline void
GST_FIXME_OBJECT (gpointer obj, const char *format, ...)
{
}

static inline void
GST_TRACE_OBJECT (gpointer obj, const char *format, ...)
{
}

static inline void
GST_ERROR (const char *format, ...)
{
}

static inline void
GST_WARNING (const char *format, ...)
{
}

static inline void
GST_INFO (const char *format, ...)
{
}

static inline void
GST_DEBUG (const char *format, ...)
{
}

static inline void
GST_LOG (const char *format, ...)
{
}

static inline void
GST_FIXME (const char *format, ...)
{
}

static inline void
GST_TRACE (const char *format, ...)
{
}

#endif /* G_HAVE_GNUC_VARARGS */
#endif /* G_HAVE_ISO_VARARGS */

#define GST_DEBUG_REGISTER_FUNCPTR(ptr) G_STMT_START{ }G_STMT_END
#define GST_DEBUG_FUNCPTR(ptr) (ptr)
#define GST_DEBUG_FUNCPTR_NAME(ptr) (g_strdup_printf ("%p", ptr))

#define GST_CAT_MEMDUMP_OBJECT(cat,obj,msg,data,length) G_STMT_START{ }G_STMT_END
#define GST_CAT_MEMDUMP(cat,msg,data,length)            G_STMT_START{ }G_STMT_END
#define GST_MEMDUMP_OBJECT(obj,msg,data,length)         G_STMT_START{ }G_STMT_END
#define GST_MEMDUMP(msg,data,length)                    G_STMT_START{ }G_STMT_END

#endif /* GST_DISABLE_GST_DEBUG */


void gst_debug_print_stack_trace (void);

G_END_DECLS

#endif /* __GSTINFO_H__ */
