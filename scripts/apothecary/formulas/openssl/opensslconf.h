#include <TargetConditionals.h>
#if defined(_MSC_VER)
# include <openssl/opensslconf_vs.h>
#elif defined( __WIN32__ ) || defined( _WIN32 )
#   include <openssl/opensslconf_win32.h>
#elif TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
#   include <openssl/opensslconf_ios.h>
#elif TARGET_OS_TV
#   include <openssl/opensslconf_tvos.h>
#elif defined(__APPLE_CC__)
#   include <openssl/opensslconf_osx.h>
#elif defined (__ANDROID__)
#   include <openssl/opensslconf_android.h>
#endif
