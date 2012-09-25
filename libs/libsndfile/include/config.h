/* src/config.h.  Generated from config.h.in by configure.  */
/* src/config.h.in.  Generated from configure.ac by autoheader.  */

/* Set to 1 if the compile is GNU GCC. */
#define COMPILER_IS_GCC 1

/* Target processor clips on negative float to int conversion. */
#define CPU_CLIPS_NEGATIVE 0

/* Target processor clips on positive float to int conversion. */
#define CPU_CLIPS_POSITIVE 0

/* Target processor is big endian. */
#define CPU_IS_BIG_ENDIAN 0

/* Target processor is little endian. */
#define CPU_IS_LITTLE_ENDIAN 1

/* Set to 1 to enable experimental code. */
#define ENABLE_EXPERIMENTAL_CODE 0

/* Define to 1 if you have the <alsa/asoundlib.h> header file. */
/* #undef HAVE_ALSA_ASOUNDLIB_H */

/* Define to 1 if you have the <byteswap.h> header file. */
#define HAVE_BYTESWAP_H 1

/* Define to 1 if you have the `calloc' function. */
/* #undef HAVE_CALLOC */

/* Define to 1 if you have the `ceil' function. */
#define HAVE_CEIL 1

/* Set to 1 if S_IRGRP is defined. */
#define HAVE_DECL_S_IRGRP 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <endian.h> header file. */
#define HAVE_ENDIAN_H 1

/* Will be set to 1 if flac, ogg and vorbis are available. */
#define HAVE_EXTERNAL_LIBS 1

/* Set to 1 if the compile supports the struct hack. */
#define HAVE_FLEXIBLE_ARRAY 0

/* Define to 1 if you have the `floor' function. */
#define HAVE_FLOOR 1

/* Define to 1 if you have the `fmod' function. */
#define HAVE_FMOD 1

/* Define to 1 if you have the `free' function. */
/* #undef HAVE_FREE */

/* Define to 1 if you have the `fstat' function. */
/* #undef HAVE_FSTAT */

/* Define to 1 if you have the `fsync' function. */
/* #undef HAVE_FSYNC */

/* Define to 1 if you have the `ftruncate' function. */
/* #undef HAVE_FTRUNCATE */

/* Define to 1 if you have the `getpagesize' function. */
/* #undef HAVE_GETPAGESIZE */

/* Define to 1 if you have the `gettimeofday' function. */
/* #undef HAVE_GETTIMEOFDAY */

/* Define to 1 if you have the `gmtime' function. */
/* #undef HAVE_GMTIME */

/* Define to 1 if you have the `gmtime_r' function. */
/* #undef HAVE_GMTIME_R */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the `localtime' function. */
/* #undef HAVE_LOCALTIME */

/* Define to 1 if you have the `localtime_r' function. */
/* #undef HAVE_LOCALTIME_R */

/* Define if you have C99's lrint function. */
#define HAVE_LRINT 1

/* Define if you have C99's lrintf function. */
#define HAVE_LRINTF 1

/* Define to 1 if you have the `lseek' function. */
/* #undef HAVE_LSEEK */

/* Define to 1 if you have the `malloc' function. */
/* #undef HAVE_MALLOC */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mmap' function. */
/* #undef HAVE_MMAP */

/* Define to 1 if you have the `open' function. */
/* #undef HAVE_OPEN */

/* Define to 1 if you have the `pipe' function. */
/* #undef HAVE_PIPE */

/* Define to 1 if you have the `pread' function. */
/* #undef HAVE_PREAD */

/* Define to 1 if you have the `pwrite' function. */
/* #undef HAVE_PWRITE */

/* Define to 1 if you have the `read' function. */
/* #undef HAVE_READ */

/* Define to 1 if you have the `realloc' function. */
/* #undef HAVE_REALLOC */

/* Define to 1 if you have the `setlocale' function. */
/* #undef HAVE_SETLOCALE */

/* Define to 1 if you have the <sndio.h> header file. */
/* #undef HAVE_SNDIO_H */

/* Define to 1 if you have the `snprintf' function. */
/* #undef HAVE_SNPRINTF */

/* Set to 1 if you have libsqlite3. */
#define HAVE_SQLITE3 0

/* Define to 1 if the system has the type `ssize_t'. */
#define HAVE_SSIZE_T 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vsnprintf' function. */
/* #undef HAVE_VSNPRINTF */

/* Define to 1 if you have the `waitpid' function. */
/* #undef HAVE_WAITPID */

/* Define to 1 if you have the `write' function. */
/* #undef HAVE_WRITE */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Set to 1 if compiling for MacOSX */
#define OS_IS_MACOSX 0

/* Set to 1 if compiling for Win32 */
#define OS_IS_WIN32 0

/* Name of package */
#define PACKAGE "libsndfile"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "sndfile@mega-nerd.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "libsndfile"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libsndfile 1.0.25"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libsndfile"

/* Define to the home page for this package. */
#define PACKAGE_URL "http://www.mega-nerd.com/libsndfile/"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.0.25"

/* Set to maximum allowed value of sf_count_t type. */
#define SF_COUNT_MAX 0x7FFFFFFF

/* The size of `double', as computed by sizeof. */
#define SIZEOF_DOUBLE 8

/* The size of `float', as computed by sizeof. */
#define SIZEOF_FLOAT 4

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `int64_t', as computed by sizeof. */
#define SIZEOF_INT64_T 8

/* The size of `loff_t', as computed by sizeof. */
#define SIZEOF_LOFF_T 8

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `off64_t', as computed by sizeof. */
#define SIZEOF_OFF64_T 8

/* The size of `off_t', as computed by sizeof. */
#define SIZEOF_OFF_T 4

/* Set to sizeof (long) if unknown. */
#define SIZEOF_SF_COUNT_T 4

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* The size of `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T 4

/* The size of `ssize_t', as computed by sizeof. */
#define SIZEOF_SSIZE_T 4

/* The size of `void*', as computed by sizeof. */
#define SIZEOF_VOIDP 4

/* The size of `wchar_t', as computed by sizeof. */
#define SIZEOF_WCHAR_T 1

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Set to long if unknown. */
#define TYPEOF_SF_COUNT_T off_t

/* Set to 1 to use the native windows API */
#define USE_WINDOWS_API 0

/* Version number of package */
#define VERSION "1.0.25"

/* Set to 1 if windows DLL is being built. */
#define WIN32_TARGET_DLL 0

/* Target processor is big endian. */
#define WORDS_BIGENDIAN 0

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define to make fseeko etc. visible, on some hosts. */
/* #undef _LARGEFILE_SOURCE */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Set to 1 to use C99 printf/snprintf in MinGW. */
/* #undef __USE_MINGW_ANSI_STDIO */
