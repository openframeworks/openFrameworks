/* opensslconf.h */

#ifdef  __cplusplus
extern "C" {
#endif

// Conditionals from ofContants.h
//-------------------------------------------
#if defined( __WIN32__ ) || defined( _WIN32 )
  #define OPENSSL_WIN32
#elif defined( __APPLE_CC__)
    #define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0
    #include <TargetConditionals.h>
  #if TARGET_OS_IPHONE_SIMULATOR || TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE || TARGET_IPHONE
    #define OPENSSL_IOS
  #else
    #define OPENSSL_OSX
  #endif
#elif defined (__ANDROID__)
  #define OPENSSL_ANDROID
#elif defined(__ARMEL__)
  // none
#elif defined(__EMSCRIPTEN__)
  // none
#else
  // none
#endif
//-------------------------------------------
// -------  Include correct opensslconf.h for platform
//-------------------------------------------
#if defined(OPENSSL_IOS)
  #include <openssl/opensslconf_ios.h>
#elif defined(OPENSSL_OSX)
  #include <openssl/opensslconf_osx.h>
#elif defined(OPENSSL_ANDROID)
  #include <openssl/opensslconf_android.h>
#elif defined(OPENSSL_WIN32)
  #include <openssl/opensslconf_win32.h>
  //@todo
#else 
  //@todo
  #include "nonononone.h"
#endif
//-------------------------------------------

#ifdef  __cplusplus
}
#endif
