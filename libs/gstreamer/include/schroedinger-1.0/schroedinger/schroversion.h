
#ifndef __SCHRO_VERSION_H__
#define __SCHRO_VERSION_H__

#define SCHRO_VERSION_MAJOR (1)
#define SCHRO_VERSION_MINOR (0)
#define SCHRO_VERSION_MICRO (10)

#define SCHRO_CHECK_VERSION(major,minor,micro) \
(SCHRO_VERSION_MAJOR > (major) || \
(SCHRO_VERSION_MAJOR == (major) && SCHRO_VERSION_MINOR > (minor)) || \
(SCHRO_VERSION_MAJOR == (major) && SCHRO_VERSION_MINOR == (minor) && \
SCHRO_VERSION_MICRO >= (micro)))


#endif

