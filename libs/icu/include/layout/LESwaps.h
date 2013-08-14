/*
 *
 * (C) Copyright IBM Corp. 1998-2011 - All Rights Reserved
 *
 */

#ifndef __LESWAPS_H
#define __LESWAPS_H

#include "LETypes.h"

/**
 * \file 
 * \brief C++ API: Endian independent access to data for LayoutEngine
 */

U_NAMESPACE_BEGIN

/**
 * A convenience macro which invokes the swapWord member function
 * from a concise call.
 *
 * @stable ICU 2.8
 */
#define SWAPW(value) LESwaps::swapWord((le_uint16)(value))

/**
 * A convenience macro which invokes the swapLong member function
 * from a concise call.
 *
 * @stable ICU 2.8
 */
#define SWAPL(value) LESwaps::swapLong((le_uint32)(value))

/**
 * This class is used to access data which stored in big endian order
 * regardless of the conventions of the platform.
 *
 * All methods are static and inline in an attempt to induce the compiler
 * to do most of the calculations at compile time.
 *
 * @stable ICU 2.8
 */
class U_LAYOUT_API LESwaps /* not : public UObject because all methods are static */ {
public:

    /**
     * Reads a big-endian 16-bit word and returns a native-endian value.
     * No-op on a big-endian platform, byte-swaps on a little-endian platform.
     *
     * @param value - the word to be byte swapped
     *
     * @return the byte swapped word
     *
     * @stable ICU 2.8
     */
    static le_uint16 swapWord(le_uint16 value)
    {
#if (defined(U_IS_BIG_ENDIAN) && U_IS_BIG_ENDIAN) || \
    (defined(BYTE_ORDER) && defined(BIG_ENDIAN) && (BYTE_ORDER == BIG_ENDIAN)) || \
    defined(__BIG_ENDIAN__)
        // Fastpath when we know that the platform is big-endian.
        return value;
#else
        // Reads a big-endian value on any platform.
        const le_uint8 *p = reinterpret_cast<const le_uint8 *>(&value);
        return (le_uint16)((p[0] << 8) | p[1]);
#endif
    };

    /**
     * Reads a big-endian 32-bit word and returns a native-endian value.
     * No-op on a big-endian platform, byte-swaps on a little-endian platform.
     *
     * @param value - the long to be byte swapped
     *
     * @return the byte swapped long
     *
     * @stable ICU 2.8
     */
    static le_uint32 swapLong(le_uint32 value)
    {
#if (defined(U_IS_BIG_ENDIAN) && U_IS_BIG_ENDIAN) || \
    (defined(BYTE_ORDER) && defined(BIG_ENDIAN) && (BYTE_ORDER == BIG_ENDIAN)) || \
    defined(__BIG_ENDIAN__)
        // Fastpath when we know that the platform is big-endian.
        return value;
#else
        // Reads a big-endian value on any platform.
        const le_uint8 *p = reinterpret_cast<const le_uint8 *>(&value);
        return (le_uint32)((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
#endif
    };

private:
    LESwaps() {} // private - forbid instantiation
};

U_NAMESPACE_END
#endif
