/* GLIB - Library of useful routines for C programming
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 */

#if defined(G_DISABLE_SINGLE_INCLUDES) && !defined (__GLIB_H_INSIDE__) && !defined (GLIB_COMPILATION)
#error "Only <glib.h> can be included directly."
#endif

#ifndef __G_MEM_H__
#define __G_MEM_H__

#include <glib/gslice.h>
#include <glib/gtypes.h>

G_BEGIN_DECLS

/**
 * GMemVTable:
 * @malloc: function to use for allocating memory.
 * @realloc: function to use for reallocating memory.
 * @free: function to use to free memory.
 * @calloc: function to use for allocating zero-filled memory.
 * @try_malloc: function to use for allocating memory without a default error handler.
 * @try_realloc: function to use for reallocating memory without a default error handler.
 * 
 * A set of functions used to perform memory allocation. The same #GMemVTable must
 * be used for all allocations in the same program; a call to g_mem_set_vtable(),
 * if it exists, should be prior to any use of GLib.
 */
typedef struct _GMemVTable GMemVTable;


#if GLIB_SIZEOF_VOID_P > GLIB_SIZEOF_LONG
/**
 * G_MEM_ALIGN:
 * 
 * Indicates the number of bytes to which memory will be aligned on the
 * current platform.
 */
#  define G_MEM_ALIGN	GLIB_SIZEOF_VOID_P
#else	/* GLIB_SIZEOF_VOID_P <= GLIB_SIZEOF_LONG */
#  define G_MEM_ALIGN	GLIB_SIZEOF_LONG
#endif	/* GLIB_SIZEOF_VOID_P <= GLIB_SIZEOF_LONG */


/* Memory allocation functions
 */

void	 g_free	          (gpointer	 mem);

gpointer g_malloc         (gsize	 n_bytes) G_GNUC_MALLOC G_GNUC_ALLOC_SIZE(1);
gpointer g_malloc0        (gsize	 n_bytes) G_GNUC_MALLOC G_GNUC_ALLOC_SIZE(1);
gpointer g_realloc        (gpointer	 mem,
			   gsize	 n_bytes) G_GNUC_WARN_UNUSED_RESULT;
gpointer g_try_malloc     (gsize	 n_bytes) G_GNUC_MALLOC G_GNUC_ALLOC_SIZE(1);
gpointer g_try_malloc0    (gsize	 n_bytes) G_GNUC_MALLOC G_GNUC_ALLOC_SIZE(1);
gpointer g_try_realloc    (gpointer	 mem,
			   gsize	 n_bytes) G_GNUC_WARN_UNUSED_RESULT;

gpointer g_malloc_n       (gsize	 n_blocks,
			   gsize	 n_block_bytes) G_GNUC_MALLOC G_GNUC_ALLOC_SIZE2(1,2);
gpointer g_malloc0_n      (gsize	 n_blocks,
			   gsize	 n_block_bytes) G_GNUC_MALLOC G_GNUC_ALLOC_SIZE2(1,2);
gpointer g_realloc_n      (gpointer	 mem,
			   gsize	 n_blocks,
			   gsize	 n_block_bytes) G_GNUC_WARN_UNUSED_RESULT;
gpointer g_try_malloc_n   (gsize	 n_blocks,
			   gsize	 n_block_bytes) G_GNUC_MALLOC G_GNUC_ALLOC_SIZE2(1,2);
gpointer g_try_malloc0_n  (gsize	 n_blocks,
			   gsize	 n_block_bytes) G_GNUC_MALLOC G_GNUC_ALLOC_SIZE2(1,2);
gpointer g_try_realloc_n  (gpointer	 mem,
			   gsize	 n_blocks,
			   gsize	 n_block_bytes) G_GNUC_WARN_UNUSED_RESULT;


/* Optimise: avoid the call to the (slower) _n function if we can
 * determine at compile-time that no overflow happens.
 */
#if defined (__GNUC__) && (__GNUC__ >= 2) && defined (__OPTIMIZE__)
#  define _G_NEW(struct_type, n_structs, func) \
	(struct_type *) (__extension__ ({			\
	  gsize __n = (gsize) (n_structs);			\
	  gsize __s = sizeof (struct_type);			\
	  gpointer __p;						\
	  if (__s == 1)						\
	    __p = g_##func (__n);				\
	  else if (__builtin_constant_p (__n) &&		\
	           (__s == 0 || __n <= G_MAXSIZE / __s))	\
	    __p = g_##func (__n * __s);				\
	  else							\
	    __p = g_##func##_n (__n, __s);			\
	  __p;							\
	}))
#  define _G_RENEW(struct_type, mem, n_structs, func) \
	(struct_type *) (__extension__ ({			\
	  gsize __n = (gsize) (n_structs);			\
	  gsize __s = sizeof (struct_type);			\
	  gpointer __p = (gpointer) (mem);			\
	  if (__s == 1)						\
	    __p = g_##func (__p, __n);				\
	  else if (__builtin_constant_p (__n) &&		\
	           (__s == 0 || __n <= G_MAXSIZE / __s))	\
	    __p = g_##func (__p, __n * __s);			\
	  else							\
	    __p = g_##func##_n (__p, __n, __s);			\
	  __p;							\
	}))

#else

/* Unoptimised version: always call the _n() function. */

#define _G_NEW(struct_type, n_structs, func) \
        ((struct_type *) g_##func##_n ((n_structs), sizeof (struct_type)))
#define _G_RENEW(struct_type, mem, n_structs, func) \
        ((struct_type *) g_##func##_n (mem, (n_structs), sizeof (struct_type)))

#endif

/**
 * g_new:
 * @struct_type: the type of the elements to allocate
 * @n_structs: the number of elements to allocate
 * 
 * Allocates @n_structs elements of type @struct_type.
 * The returned pointer is cast to a pointer to the given type.
 * If @n_structs is 0 it returns %NULL.
 * Care is taken to avoid overflow when calculating the size of the allocated block.
 * 
 * Since the returned pointer is already casted to the right type,
 * it is normally unnecessary to cast it explicitly, and doing
 * so might hide memory allocation errors.
 * 
 * Returns: a pointer to the allocated memory, cast to a pointer to @struct_type
 */
#define g_new(struct_type, n_structs)			_G_NEW (struct_type, n_structs, malloc)
/**
 * g_new0:
 * @struct_type: the type of the elements to allocate.
 * @n_structs: the number of elements to allocate.
 * 
 * Allocates @n_structs elements of type @struct_type, initialized to 0's.
 * The returned pointer is cast to a pointer to the given type.
 * If @n_structs is 0 it returns %NULL.
 * Care is taken to avoid overflow when calculating the size of the allocated block.
 * 
 * Since the returned pointer is already casted to the right type,
 * it is normally unnecessary to cast it explicitly, and doing
 * so might hide memory allocation errors.
 * 
 * Returns: a pointer to the allocated memory, cast to a pointer to @struct_type.
 */
#define g_new0(struct_type, n_structs)			_G_NEW (struct_type, n_structs, malloc0)
/**
 * g_renew:
 * @struct_type: the type of the elements to allocate
 * @mem: the currently allocated memory
 * @n_structs: the number of elements to allocate
 * 
 * Reallocates the memory pointed to by @mem, so that it now has space for
 * @n_structs elements of type @struct_type. It returns the new address of
 * the memory, which may have been moved.
 * Care is taken to avoid overflow when calculating the size of the allocated block.
 * 
 * Returns: a pointer to the new allocated memory, cast to a pointer to @struct_type
 */
#define g_renew(struct_type, mem, n_structs)		_G_RENEW (struct_type, mem, n_structs, realloc)
/**
 * g_try_new:
 * @struct_type: the type of the elements to allocate
 * @n_structs: the number of elements to allocate
 * 
 * Attempts to allocate @n_structs elements of type @struct_type, and returns
 * %NULL on failure. Contrast with g_new(), which aborts the program on failure.
 * The returned pointer is cast to a pointer to the given type.
 * The function returns %NULL when @n_structs is 0 of if an overflow occurs.
 * 
 * Since: 2.8
 * Returns: a pointer to the allocated memory, cast to a pointer to @struct_type
 */
#define g_try_new(struct_type, n_structs)		_G_NEW (struct_type, n_structs, try_malloc)
/**
 * g_try_new0:
 * @struct_type: the type of the elements to allocate
 * @n_structs: the number of elements to allocate
 * 
 * Attempts to allocate @n_structs elements of type @struct_type, initialized
 * to 0's, and returns %NULL on failure. Contrast with g_new0(), which aborts
 * the program on failure.
 * The returned pointer is cast to a pointer to the given type.
 * The function returns %NULL when @n_structs is 0 of if an overflow occurs.
 * 
 * Since: 2.8
 * Returns: a pointer to the allocated memory, cast to a pointer to @struct_type
 */
#define g_try_new0(struct_type, n_structs)		_G_NEW (struct_type, n_structs, try_malloc0)
/**
 * g_try_renew:
 * @struct_type: the type of the elements to allocate
 * @mem: the currently allocated memory
 * @n_structs: the number of elements to allocate
 * 
 * Attempts to reallocate the memory pointed to by @mem, so that it now has
 * space for @n_structs elements of type @struct_type, and returns %NULL on
 * failure. Contrast with g_renew(), which aborts the program on failure.
 * It returns the new address of the memory, which may have been moved.
 * The function returns %NULL if an overflow occurs.
 * 
 * Since: 2.8
 * Returns: a pointer to the new allocated memory, cast to a pointer to @struct_type
 */
#define g_try_renew(struct_type, mem, n_structs)	_G_RENEW (struct_type, mem, n_structs, try_realloc)


/* Memory allocation virtualization for debugging purposes
 * g_mem_set_vtable() has to be the very first GLib function called
 * if being used
 */
struct _GMemVTable {
  gpointer (*malloc)      (gsize    n_bytes);
  gpointer (*realloc)     (gpointer mem,
			   gsize    n_bytes);
  void     (*free)        (gpointer mem);
  /* optional; set to NULL if not used ! */
  gpointer (*calloc)      (gsize    n_blocks,
			   gsize    n_block_bytes);
  gpointer (*try_malloc)  (gsize    n_bytes);
  gpointer (*try_realloc) (gpointer mem,
			   gsize    n_bytes);
};
void	 g_mem_set_vtable (GMemVTable	*vtable);
gboolean g_mem_is_system_malloc (void);

GLIB_VAR gboolean g_mem_gc_friendly;

/* Memory profiler and checker, has to be enabled via g_mem_set_vtable()
 */
GLIB_VAR GMemVTable	*glib_mem_profiler_table;
void	g_mem_profile	(void);


/* deprecated memchunks and allocators */
#if !defined (G_DISABLE_DEPRECATED) || defined (GTK_COMPILATION) || defined (GDK_COMPILATION)
typedef struct _GAllocator GAllocator;
typedef struct _GMemChunk  GMemChunk;
#define g_mem_chunk_create(type, pre_alloc, alloc_type)	( \
  g_mem_chunk_new (#type " mem chunks (" #pre_alloc ")", \
		   sizeof (type), \
		   sizeof (type) * (pre_alloc), \
		   (alloc_type)) \
)
#define g_chunk_new(type, chunk)	( \
  (type *) g_mem_chunk_alloc (chunk) \
)
#define g_chunk_new0(type, chunk)	( \
  (type *) g_mem_chunk_alloc0 (chunk) \
)
#define g_chunk_free(mem, mem_chunk)	G_STMT_START { \
  g_mem_chunk_free ((mem_chunk), (mem)); \
} G_STMT_END
#define G_ALLOC_ONLY	  1
#define G_ALLOC_AND_FREE  2
GMemChunk* g_mem_chunk_new     (const gchar *name,
				gint         atom_size,
				gsize        area_size,
				gint         type);
void       g_mem_chunk_destroy (GMemChunk   *mem_chunk);
gpointer   g_mem_chunk_alloc   (GMemChunk   *mem_chunk);
gpointer   g_mem_chunk_alloc0  (GMemChunk   *mem_chunk);
void       g_mem_chunk_free    (GMemChunk   *mem_chunk,
				gpointer     mem);
void       g_mem_chunk_clean   (GMemChunk   *mem_chunk);
void       g_mem_chunk_reset   (GMemChunk   *mem_chunk);
void       g_mem_chunk_print   (GMemChunk   *mem_chunk);
void       g_mem_chunk_info    (void);
void	   g_blow_chunks       (void);
GAllocator*g_allocator_new     (const gchar  *name,
				guint         n_preallocs);
void       g_allocator_free    (GAllocator   *allocator);
#define	G_ALLOCATOR_LIST       (1)
#define	G_ALLOCATOR_SLIST      (2)
#define	G_ALLOCATOR_NODE       (3)
#endif /* G_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __G_MEM_H__ */
