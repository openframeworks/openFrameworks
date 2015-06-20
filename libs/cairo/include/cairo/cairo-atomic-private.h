/* cairo - a vector graphics library with display and print output
 *
 * Copyright © 2007 Chris Wilson
 * Copyright © 2010 Andrea Canciani
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335, USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 * The Original Code is the cairo graphics library.
 *
 * The Initial Developer of the Original Code is University of Southern
 * California.
 *
 * Contributor(s):
 *	Chris Wilson <chris@chris-wilson.co.uk>
 *	Andrea Canciani <ranma42@gmail.com>
 */

#ifndef CAIRO_ATOMIC_PRIVATE_H
#define CAIRO_ATOMIC_PRIVATE_H

# include "cairo-compiler-private.h"

#if HAVE_CONFIG_H
#include "config.h"
#endif

/* The autoconf on OpenBSD 4.5 produces the malformed constant name
 * SIZEOF_VOID__ rather than SIZEOF_VOID_P.  Work around that here. */
#if !defined(SIZEOF_VOID_P) && defined(SIZEOF_VOID__)
# define SIZEOF_VOID_P SIZEOF_VOID__
#endif

CAIRO_BEGIN_DECLS

#if HAVE_INTEL_ATOMIC_PRIMITIVES

#define HAS_ATOMIC_OPS 1

typedef int cairo_atomic_int_t;

#ifdef ATOMIC_OP_NEEDS_MEMORY_BARRIER
static cairo_always_inline cairo_atomic_int_t
_cairo_atomic_int_get (cairo_atomic_int_t *x)
{
    __sync_synchronize ();
    return *x;
}

static cairo_always_inline void *
_cairo_atomic_ptr_get (void **x)
{
    __sync_synchronize ();
    return *x;
}
#else
# define _cairo_atomic_int_get(x) (*x)
# define _cairo_atomic_ptr_get(x) (*x)
#endif

# define _cairo_atomic_int_inc(x) ((void) __sync_fetch_and_add(x, 1))
# define _cairo_atomic_int_dec(x) ((void) __sync_fetch_and_add(x, -1))
# define _cairo_atomic_int_dec_and_test(x) (__sync_fetch_and_add(x, -1) == 1)
# define _cairo_atomic_int_cmpxchg(x, oldv, newv) __sync_bool_compare_and_swap (x, oldv, newv)
# define _cairo_atomic_int_cmpxchg_return_old(x, oldv, newv) __sync_val_compare_and_swap (x, oldv, newv)

#if SIZEOF_VOID_P==SIZEOF_INT
typedef int cairo_atomic_intptr_t;
#elif SIZEOF_VOID_P==SIZEOF_LONG
typedef long cairo_atomic_intptr_t;
#elif SIZEOF_VOID_P==SIZEOF_LONG_LONG
typedef long long cairo_atomic_intptr_t;
#else
#error No matching integer pointer type
#endif

# define _cairo_atomic_ptr_cmpxchg(x, oldv, newv) \
    __sync_bool_compare_and_swap ((cairo_atomic_intptr_t*)x, (cairo_atomic_intptr_t)oldv, (cairo_atomic_intptr_t)newv)

# define _cairo_atomic_ptr_cmpxchg_return_old(x, oldv, newv) \
    _cairo_atomic_intptr_to_voidptr (__sync_val_compare_and_swap ((cairo_atomic_intptr_t*)x, (cairo_atomic_intptr_t)oldv, (cairo_atomic_intptr_t)newv))

#endif

#if HAVE_LIB_ATOMIC_OPS
#include <atomic_ops.h>

#define HAS_ATOMIC_OPS 1

typedef  AO_t cairo_atomic_int_t;

# define _cairo_atomic_int_get(x) (AO_load_full (x))

# define _cairo_atomic_int_inc(x) ((void) AO_fetch_and_add1_full(x))
# define _cairo_atomic_int_dec(x) ((void) AO_fetch_and_sub1_full(x))
# define _cairo_atomic_int_dec_and_test(x) (AO_fetch_and_sub1_full(x) == 1)
# define _cairo_atomic_int_cmpxchg(x, oldv, newv) AO_compare_and_swap_full(x, oldv, newv)

#if SIZEOF_VOID_P==SIZEOF_INT
typedef unsigned int cairo_atomic_intptr_t;
#elif SIZEOF_VOID_P==SIZEOF_LONG
typedef unsigned long cairo_atomic_intptr_t;
#elif SIZEOF_VOID_P==SIZEOF_LONG_LONG
typedef unsigned long long cairo_atomic_intptr_t;
#else
#error No matching integer pointer type
#endif

# define _cairo_atomic_ptr_get(x) _cairo_atomic_intptr_to_voidptr (AO_load_full (x))
# define _cairo_atomic_ptr_cmpxchg(x, oldv, newv) \
    _cairo_atomic_int_cmpxchg ((cairo_atomic_intptr_t*)(x), (cairo_atomic_intptr_t)oldv, (cairo_atomic_intptr_t)newv)

#endif

#if HAVE_OS_ATOMIC_OPS
#include <libkern/OSAtomic.h>

#define HAS_ATOMIC_OPS 1

typedef int32_t cairo_atomic_int_t;

# define _cairo_atomic_int_get(x) (OSMemoryBarrier(), *(x))

# define _cairo_atomic_int_inc(x) ((void) OSAtomicIncrement32Barrier (x))
# define _cairo_atomic_int_dec(x) ((void) OSAtomicDecrement32Barrier (x))
# define _cairo_atomic_int_dec_and_test(x) (OSAtomicDecrement32Barrier (x) == 0)
# define _cairo_atomic_int_cmpxchg(x, oldv, newv) OSAtomicCompareAndSwap32Barrier(oldv, newv, x)

#if SIZEOF_VOID_P==4
typedef int32_t cairo_atomic_intptr_t;
# define _cairo_atomic_ptr_cmpxchg(x, oldv, newv) \
    OSAtomicCompareAndSwap32Barrier((cairo_atomic_intptr_t)oldv, (cairo_atomic_intptr_t)newv, (cairo_atomic_intptr_t *)x)

#elif SIZEOF_VOID_P==8
typedef int64_t cairo_atomic_intptr_t;
# define _cairo_atomic_ptr_cmpxchg(x, oldv, newv) \
    OSAtomicCompareAndSwap64Barrier((cairo_atomic_intptr_t)oldv, (cairo_atomic_intptr_t)newv, (cairo_atomic_intptr_t *)x)

#else
#error No matching integer pointer type
#endif

# define _cairo_atomic_ptr_get(x) (OSMemoryBarrier(), *(x))

#endif

#ifndef HAS_ATOMIC_OPS

#if SIZEOF_VOID_P==SIZEOF_INT
typedef unsigned int cairo_atomic_intptr_t;
#elif SIZEOF_VOID_P==SIZEOF_LONG
typedef unsigned long cairo_atomic_intptr_t;
#elif SIZEOF_VOID_P==SIZEOF_LONG_LONG
typedef unsigned long long cairo_atomic_intptr_t;
#else
#error No matching integer pointer type
#endif

typedef cairo_atomic_intptr_t cairo_atomic_int_t;

cairo_private void
_cairo_atomic_int_inc (cairo_atomic_int_t *x);

#define _cairo_atomic_int_dec(x) _cairo_atomic_int_dec_and_test(x)

cairo_private cairo_bool_t
_cairo_atomic_int_dec_and_test (cairo_atomic_int_t *x);

cairo_private cairo_atomic_int_t
_cairo_atomic_int_cmpxchg_return_old_impl (cairo_atomic_int_t *x, cairo_atomic_int_t oldv, cairo_atomic_int_t newv);

cairo_private void *
_cairo_atomic_ptr_cmpxchg_return_old_impl (void **x, void *oldv, void *newv);

#define _cairo_atomic_int_cmpxchg_return_old(x, oldv, newv) _cairo_atomic_int_cmpxchg_return_old_impl (x, oldv, newv)
#define _cairo_atomic_ptr_cmpxchg_return_old(x, oldv, newv) _cairo_atomic_ptr_cmpxchg_return_old_impl (x, oldv, newv)

#ifdef ATOMIC_OP_NEEDS_MEMORY_BARRIER
cairo_private cairo_atomic_int_t
_cairo_atomic_int_get (cairo_atomic_int_t *x);
# define _cairo_atomic_ptr_get(x) (void *) _cairo_atomic_int_get((cairo_atomic_int_t *) x)
#else
# define _cairo_atomic_int_get(x) (*x)
# define _cairo_atomic_ptr_get(x) (*x)
#endif

#else

/* Workaround GCC complaining about casts */
static cairo_always_inline void *
_cairo_atomic_intptr_to_voidptr (cairo_atomic_intptr_t x)
{
  return (void *) x;
}

static cairo_always_inline cairo_atomic_int_t
_cairo_atomic_int_cmpxchg_return_old_fallback(cairo_atomic_int_t *x, cairo_atomic_int_t oldv, cairo_atomic_int_t newv)
{
    cairo_atomic_int_t curr;

    do {
        curr = _cairo_atomic_int_get (x);
    } while (curr == oldv && !_cairo_atomic_int_cmpxchg (x, oldv, newv));

    return curr;
}

static cairo_always_inline void *
_cairo_atomic_ptr_cmpxchg_return_old_fallback(void **x, void *oldv, void *newv)
{
    void *curr;

    do {
        curr = _cairo_atomic_ptr_get (x);
    } while (curr == oldv && !_cairo_atomic_ptr_cmpxchg (x, oldv, newv));

    return curr;
}
#endif

#ifndef _cairo_atomic_int_cmpxchg_return_old
#define _cairo_atomic_int_cmpxchg_return_old(x, oldv, newv) _cairo_atomic_int_cmpxchg_return_old_fallback (x, oldv, newv)
#endif

#ifndef _cairo_atomic_ptr_cmpxchg_return_old
#define _cairo_atomic_ptr_cmpxchg_return_old(x, oldv, newv) _cairo_atomic_ptr_cmpxchg_return_old_fallback (x, oldv, newv)
#endif

#ifndef _cairo_atomic_int_cmpxchg
#define _cairo_atomic_int_cmpxchg(x, oldv, newv) (_cairo_atomic_int_cmpxchg_return_old (x, oldv, newv) == oldv)
#endif

#ifndef _cairo_atomic_ptr_cmpxchg
#define _cairo_atomic_ptr_cmpxchg(x, oldv, newv) (_cairo_atomic_ptr_cmpxchg_return_old (x, oldv, newv) == oldv)
#endif

#define _cairo_atomic_uint_get(x) _cairo_atomic_int_get(x)
#define _cairo_atomic_uint_cmpxchg(x, oldv, newv) \
    _cairo_atomic_int_cmpxchg((cairo_atomic_int_t *)x, oldv, newv)

#define _cairo_status_set_error(status, err) do { \
    int ret__; \
    assert (err < CAIRO_STATUS_LAST_STATUS); \
    /* hide compiler warnings about cairo_status_t != int (gcc treats its as \
     * an unsigned integer instead, and about ignoring the return value. */  \
    ret__ = _cairo_atomic_int_cmpxchg ((cairo_atomic_int_t *) status, CAIRO_STATUS_SUCCESS, err); \
    (void) ret__; \
} while (0)

CAIRO_END_DECLS

#endif
