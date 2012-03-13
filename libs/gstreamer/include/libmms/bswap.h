#ifndef BSWAP_H_INCLUDED
#define BSWAP_H_INCLUDED

/*
 * Copyright (C) 2004 Maciej Katafiasz <mathrick@users.sourceforge.net>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


/* Go cheap now, will rip out glib later. *Sigh* */
#include <glib.h>

/* NOTE:
 * Now, to clear up confusion: LE_XX means "from LE to native, XX bits wide"
 * I know it's not very clear naming (tell me about it, I
 * misinterpreted in first version and caused bad nasty bug, *sigh*),
 * but that's inherited code, will clean up as things go
 * Oh, and one more thing -- they take *pointers*, not actual ints
 */

#if defined(__sun) || defined(__hpux) || defined (__MINGW) || defined(__MINGW32__)

/* Actually these platforms do not support u_int*_t types */
#include <sys/types.h>

#define LE_16(val) (GINT16_FROM_LE (*((uint16_t*)(val))))
#define BE_16(val) (GINT16_FROM_BE (*((uint16_t*)(val))))
#define LE_32(val) (GINT32_FROM_LE (*((uint32_t*)(val))))
#define BE_32(val) (GINT32_FROM_BE (*((uint32_t*)(val))))

#define LE_64(val) (GINT64_FROM_LE (*((uint64_t*)(val))))
#define BE_64(val) (GINT64_FROM_BE (*((uint64_t*)(val))))

#else

#define LE_16(val) (GINT16_FROM_LE (*((u_int16_t*)(val))))
#define BE_16(val) (GINT16_FROM_BE (*((u_int16_t*)(val))))
#define LE_32(val) (GINT32_FROM_LE (*((u_int32_t*)(val))))
#define BE_32(val) (GINT32_FROM_BE (*((u_int32_t*)(val))))

#define LE_64(val) (GINT64_FROM_LE (*((u_int64_t*)(val))))
#define BE_64(val) (GINT64_FROM_BE (*((u_int64_t*)(val))))

#endif

#endif /* BSWAP_H_INCLUDED */
