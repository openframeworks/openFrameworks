/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LIBOIL_FUNCTION_H_
#define _LIBOIL_FUNCTION_H_

#include <liboil/liboilutils.h>
#include <liboil/liboiltypes.h>

/**
 * OIL_CHECK_PROTOTYPE:
 * @a:
 *
 * Macro used internally to implement the --enable-prototype-checking
 * configure option.
 */
#ifdef LIBOIL_STRICT_PROTOTYPES
#include <liboil/liboilfuncs.h>
#define OIL_CHECK_PROTOTYPE(a) a
#else
#define OIL_CHECK_PROTOTYPE(a)
#endif

OIL_BEGIN_DECLS

/**
 * OilImplFlag:
 *
 * Implementation flags.
 *
 * @OIL_IMPL_FLAG_REF: is the reference implementation for the class.
 *
 * @OIL_IMPL_FLAG_OPT: was compiled with alternate CFLAGS as specified
 * by --enable-alternate-optimization.
 *
 * @OIL_IMPL_FLAG_ASM: is written in assembly code.
 *
 * @OIL_IMPL_FLAG_DISABLED: is disabled.  This can be set either in the
 * source code or during library initialization.
 *
 * @OIL_IMPL_FLAG_CMOV: uses the i386 instruction cmov or its variants.
 *
 * @OIL_IMPL_FLAG_MMX: uses MMX instructions.
 *
 * @OIL_IMPL_FLAG_SSE: uses SSE instructions.
 *
 * @OIL_IMPL_FLAG_MMXEXT: uses AMD's extended MMX instructions.  These
 * are a subset of what Intel calls SSE2.  If an implementation uses
 * only AMD's extended MMX instructions, it should set this flag, and
 * not @OIL_IMPL_FLAG_SSE2.
 *
 * @OIL_IMPL_FLAG_SSE2: uses SSE2 instructions.  This flag implies
 * @OIL_IMPL_FLAG_SSE and @OIL_IMPL_FLAG_MMXEXT.
 *
 * @OIL_IMPL_FLAG_3DNOW: uses 3DNow! instructions.
 *
 * @OIL_IMPL_FLAG_3DNOWEXT: uses extended 3DNow! instructions.
 *
 * @OIL_IMPL_FLAG_SSE3: uses SSE3 instructions.  This flag implies
 * @OIL_IMPL_FLAG_SSE2.
 *
 * @OIL_IMPL_FLAG_SSSE3: uses SSSE3 instructions.  This flag implies
 * @OIL_IMPL_FLAG_SSE3.
 *
 * @OIL_IMPL_FLAG_ALTIVEC: uses Altivec instructions.
 *
 */
typedef enum {
  OIL_IMPL_FLAG_REF = (1<<0),
  OIL_IMPL_FLAG_OPT = (1<<1),
  OIL_IMPL_FLAG_ASM = (1<<2),
  OIL_IMPL_FLAG_DISABLED = (1<<3),
  OIL_IMPL_FLAG_CMOV = (1<<16),
  OIL_IMPL_FLAG_MMX = (1<<17),
  OIL_IMPL_FLAG_SSE = (1<<18),
  OIL_IMPL_FLAG_MMXEXT = (1<<19),
  OIL_IMPL_FLAG_SSE2 = (1<<20),
  OIL_IMPL_FLAG_3DNOW = (1<<21),
  OIL_IMPL_FLAG_3DNOWEXT = (1<<22),
  OIL_IMPL_FLAG_SSE3 = (1<<23),
  OIL_IMPL_FLAG_ALTIVEC = (1<<24),
  OIL_IMPL_FLAG_EDSP = (1<<25),
  OIL_IMPL_FLAG_ARM6 = (1<<26),
  OIL_IMPL_FLAG_VFP = (1<<27),
  OIL_IMPL_FLAG_SSSE3 = (1<<28)
} OilImplFlag;

#ifdef OIL_ENABLE_UNSTABLE_API

/**
 * OIL_OPT_MANGLE:
 *
 * Used internally to implement the --enable-alternate-optimizations
 * configure option.
 */
/**
 * OIL_OPT_FLAG_MANGLE:
 *
 * Used internally to implement the --enable-alternate-optimizations
 * configure option.
 */
/**
 * OIL_NO_CLASSES:
 *
 * Used internally to implement the --enable-alternate-optimizations
 * configure option.
 */
/**
 * OIL_OPT_SUFFIX:
 *
 * Used internally to implement the --enable-alternate-optimizations
 * configure option.
 */
#ifndef OIL_OPT_MANGLE
#define OIL_OPT_MANGLE(a) a
#define OIL_OPT_FLAG_MANGLE(a) a
#else
#define OIL_NO_CLASSES
#define OIL_OPT_FLAG_MANGLE(a) (((a)&(~OIL_IMPL_FLAG_REF)) | OIL_IMPL_FLAG_OPT)
#endif
#ifndef OIL_OPT_SUFFIX
#define OIL_OPT_SUFFIX
#endif

/**
 * OilFunctionClass:
 *
 * An opaque structure representing a function class.
 *
 */
struct _OilFunctionClass {
  /*< private >*/
	void (*func)(void);
	const char *name;
	const char *desc;
	OilTestFunction test_func;

	OilFunctionImpl *first_impl;
	OilFunctionImpl *reference_impl;

	OilFunctionImpl *chosen_impl;

	const char *prototype;
};

/**
 * OilFunctionImpl:
 *
 * An opaque structure representing a function implementation.
 *
 */
struct _OilFunctionImpl {
  /*< private >*/
	void *next;
	OilFunctionClass *klass;
	void (*func)(void);
	unsigned int flags;
	const char *name;
        double profile_ave;
        double profile_std;
};

/**
 * OIL_GET:
 * @ptr:
 * @offset:
 * @type:
 *
 * Offsets @ptr by @offset number of bytes, and dereferences it
 * as type @type.  Note that the offset is in bytes, and not in
 * the size of the pointer type.
 */
#define OIL_GET(ptr, offset, type) (*(type *)((uint8_t *)(ptr) + (offset)) )
/**
 * OIL_OFFSET:
 * @ptr:
 * @offset:
 *
 * Add @offset bytes to the pointer @ptr.
 */
#define OIL_OFFSET(ptr, offset) ((void *)((uint8_t *)(ptr) + (offset)) )
/**
 * OIL_INCREMENT:
 * @ptr:
 * @offset:
 *
 * Increments the pointer @ptr by @offset number of bytes.
 */
#define OIL_INCREMENT(ptr, offset) (ptr = (void *)((uint8_t *)ptr + (offset)) )

/**
 * OIL_CPU_FLAG_MASK:
 *
 * Mask describing which bits in #OilImplFlag depend on the current
 * CPU.
 */
#define OIL_CPU_FLAG_MASK 0xffff0000

/**
 * OIL_DECLARE_CLASS:
 * @klass: the name of a function class (without the oil_ prefix)
 *
 * Declares the Liboil function class @klass.
 */
#define OIL_DECLARE_CLASS(klass) \
	extern OilFunctionClass _oil_function_class_ ## klass

/**
 * SECTION:liboilmacros
 * @title: Macros
 * @short_description: Macros
 */

#ifndef OIL_NO_CLASSES
/**
 * OIL_DEFINE_CLASS_FULL:
 * @klass: name of class to declare (without oil_ prefix)
 * @string: prototype of class
 * @test: test function
 *
 * Defines a #OilFunctionClass structure for @klass.  Classes
 * defined this way will be automatically at Liboil initialization
 * time.
 */
#define OIL_DEFINE_CLASS_FULL(klass, string, test) \
OilFunctionClass _oil_function_class_ ## klass = { \
	NULL, \
	#klass , \
	NULL, \
        test, \
        NULL, \
        NULL, \
        NULL, \
        string \
}; \
OilFunctionClass *oil_function_class_ptr_ ## klass = \
  &_oil_function_class_ ## klass
#else
#define OIL_DEFINE_CLASS_FULL(klass, string, test) \
  OIL_DECLARE_CLASS(klass)
#endif

/**
 * OIL_DEFINE_CLASS:
 * @klass: name of class to declare (without oil_ prefix)
 * @string: prototype of class
 *
 * Defines a #OilFunctionClass structure for @klass.  Classes
 * defined this way will be automatically at Liboil initialization
 * time.
 */
#define OIL_DEFINE_CLASS(klass, string) \
  OIL_DEFINE_CLASS_FULL (klass, string, NULL)

/**
 * OIL_DEFINE_IMPL_FULL:
 * @function: name of function
 * @klass: name of class to declare (without oil_ prefix)
 * @flags: implementation flags and CPU requirements
 *
 * Defines a #OilFunctionImpl structure for the function @function
 * and class @klass.  CPU-dependent flags in @flags will indicate
 * that this implementation requires the given CPU flags.
 */
#define OIL_DEFINE_IMPL_FULL(function,klass,flags) \
OilFunctionImpl OIL_OPT_MANGLE(_oil_function_impl_ ## function) = { \
	NULL, \
	&_oil_function_class_ ## klass , \
	(void (*)(void)) function,	 \
	OIL_OPT_FLAG_MANGLE(flags), \
        #function OIL_OPT_SUFFIX \
} \
OIL_CHECK_PROTOTYPE(;_oil_type_ ## klass _ignore_me_ ## function = function)

/**
 * OIL_DEFINE_IMPL:
 * @function: name of function
 * @klass: name of class to declare (without oil_ prefix)
 *
 * Shorthand for defining a C implementation.  See OIL_DEFINE_IMPL_FULL().
 */
#define OIL_DEFINE_IMPL(function,klass) \
	OIL_DEFINE_IMPL_FULL(function,klass,0)
/**
 * OIL_DEFINE_IMPL_REF:
 * @function: name of function
 * @klass: name of class to declare (without oil_ prefix)
 *
 * Shorthand for defining a reference implementation.  See OIL_DEFINE_IMPL_FULL().
 */
#define OIL_DEFINE_IMPL_REF(function,klass) \
	OIL_DEFINE_IMPL_FULL(function,klass,OIL_IMPL_FLAG_REF)
/**
 * OIL_DEFINE_IMPL_ASM:
 * @function: name of function
 * @klass: name of class to declare (without oil_ prefix)
 *
 * Shorthand for defining an implementation written in inline
 * assembly code.  See OIL_DEFINE_IMPL_FULL().
 */
#define OIL_DEFINE_IMPL_ASM(function,klass) \
	OIL_DEFINE_IMPL_FULL(function,klass,OIL_IMPL_FLAG_ASM)
/**
 * OIL_DEFINE_IMPL_DEPENDS
 * @function: name of function
 * @klass: name of class to declare (without oil_ prefix)
 * @...: other classes this implementation uses
 *
 * Shorthand for defining an implementation that uses another Liboil
 * function class.  This is not currently used.  See
 * OIL_DEFINE_IMPL_FULL().
 */
#define OIL_DEFINE_IMPL_DEPENDS(function,klass,...) \
	OIL_DEFINE_IMPL_FULL(function,klass,0)

void oil_optimize_all (void);
void oil_optimize (const char *class_name);

OilFunctionClass * oil_class_get_by_index (int i);
OilFunctionClass *oil_class_get (const char *class_name);
void oil_class_optimize (OilFunctionClass *klass);
int oil_class_get_n_classes (void);

OilFunctionImpl * oil_impl_get_by_index (int i);
int oil_impl_is_runnable (OilFunctionImpl *impl);
int oil_impl_is_usable (OilFunctionImpl *impl);

void oil_class_choose_by_name (OilFunctionClass * klass, const char *name);

void oil_class_register_impl_full (OilFunctionClass * klass,
    void (*func)(void), const char *name, unsigned int flags);
void oil_class_register_impl (OilFunctionClass * klass, OilFunctionImpl *impl);
void oil_class_register_impl_by_name (const char *klass_name,
    OilFunctionImpl *impl);

void oil_init_no_optimize(void);

#endif

OIL_END_DECLS

#endif
