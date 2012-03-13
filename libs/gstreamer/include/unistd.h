#ifndef _UNISTD_H
/*
 * This file is part of the Mingw32 package.
 *
 * Modified slightly for MSVC compatibility.
 *
 * unistd.h maps (roughly) to io.h
 * Other headers included by unistd.h may be selectively processed;
 * __UNISTD_H_SOURCED__ enables such selective processing.
 */
#define _UNISTD_H
#define __UNISTD_H_SOURCED__ 1

#include <io.h>
#include <process.h>

/* These are also defined in stdio.h. */
#ifndef	SEEK_SET
#define SEEK_SET 0
#endif

#ifndef	SEEK_CUR
#define SEEK_CUR 1
#endif

#ifndef SEEK_END
#define SEEK_END 2
#endif

#undef __UNISTD_H_SOURCED__
#endif /* _UNISTD_H */
