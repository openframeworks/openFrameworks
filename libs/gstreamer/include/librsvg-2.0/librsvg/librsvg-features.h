#ifndef LIBRSVG_FEATURES_H
#define LIBRSVG_FEATURES_H 1

#define LIBRSVG_MAJOR_VERSION (2)
#define LIBRSVG_MINOR_VERSION (32)
#define LIBRSVG_MICRO_VERSION (0)
#define LIBRSVG_VERSION "2.32.0"

#define LIBRSVG_CHECK_VERSION(major,minor,micro) \
  (LIBRSVG_MAJOR_VERSION > (major) || \
   (LIBRSVG_MAJOR_VERSION == (major) && LIBRSVG_MINOR_VERSION > (minor)) || \
   (LIBRSVG_MAJOR_VERSION == (major) && LIBRSVG_MINOR_VERSION == (minor) && LIBRSVG_MICRO_VERSION >= (micro)))

#define LIBRSVG_HAVE_SVGZ  (1)
#define LIBRSVG_HAVE_CSS   (1)

#define LIBRSVG_CHECK_FEATURE(FEATURE) (defined(LIBRSVG_HAVE_##FEATURE) && LIBRSVG_HAVE_##FEATURE)

extern const unsigned int librsvg_major_version, librsvg_minor_version, librsvg_micro_version;
extern const char *librsvg_version;

void librsvg_preinit(void *app, void *modinfo);
void librsvg_postinit(void *app, void *modinfo);

#endif
