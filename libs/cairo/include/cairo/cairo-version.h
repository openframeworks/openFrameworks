/* This is a dummy file.
 * The actual version info is in toplevel cairo-version.h.
 * The purpose of this file is to make most of the source files NOT depend
 * on the real cairo-version.h, and as a result, changing library version
 * would not cause a complete rebuild of all object files (just a relink).
 * This is useful when bisecting. */
#ifndef CAIRO_VERSION_H
#define CAIRO_VERSION_H

#define CAIRO_VERSION_MAJOR USE_cairo_version_OR_cairo_version_string_INSTEAD
#define CAIRO_VERSION_MINOR USE_cairo_version_OR_cairo_version_string_INSTEAD
#define CAIRO_VERSION_MICRO USE_cairo_version_OR_cairo_version_string_INSTEAD

#endif
