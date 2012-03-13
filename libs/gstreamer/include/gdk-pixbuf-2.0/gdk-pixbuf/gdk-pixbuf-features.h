#if defined(GDK_PIXBUF_DISABLE_SINGLE_INCLUDES) && !defined (GDK_PIXBUF_H_INSIDE) && !defined (GDK_PIXBUF_COMPILATION)
#error "Only <gdk-pixbuf/gdk-pixbuf.h> can be included directly."
#endif

#ifndef GDK_PIXBUF_FEATURES_H
#define GDK_PIXBUF_FEATURES_H 1

#define GDK_PIXBUF_MAJOR (2)
#define GDK_PIXBUF_MINOR (22)
#define GDK_PIXBUF_MICRO (0)
#define GDK_PIXBUF_VERSION "2.22.0"

/* We prefix variable declarations so they can
 * properly get exported/imported from Windows DLLs.
 */
#ifdef G_PLATFORM_WIN32
#  ifdef GDK_PIXBUF_STATIC_COMPILATION
#    define GDK_PIXBUF_VAR extern
#  else /* !GDK_PIXBUF_STATIC_COMPILATION */
#    ifdef GDK_PIXBUF_C_COMPILATION
#      ifdef DLL_EXPORT
#        define GDK_PIXBUF_VAR __declspec(dllexport)
#      else /* !DLL_EXPORT */
#        define GDK_PIXBUF_VAR extern
#      endif /* !DLL_EXPORT */
#    else /* !GDK_PIXBUF_C_COMPILATION */
#      define GDK_PIXBUF_VAR extern __declspec(dllimport)
#    endif /* !GDK_PIXBUF_C_COMPILATION */
#  endif /* !GDK_PIXBUF_STATIC_COMPILATION */
#else /* !G_PLATFORM_WIN32 */
#  define GDK_PIXBUF_VAR extern
#endif /* !G_PLATFORM_WIN32 */

GDK_PIXBUF_VAR const guint gdk_pixbuf_major_version;
GDK_PIXBUF_VAR const guint gdk_pixbuf_minor_version;
GDK_PIXBUF_VAR const guint gdk_pixbuf_micro_version;
GDK_PIXBUF_VAR const char *gdk_pixbuf_version;

#endif /* GDK_PIXBUF_FEATURES_H */
