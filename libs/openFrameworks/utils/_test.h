#include "ofConstants.h"


#define STRING2(x) #x
#define STRING(x) STRING2(x)
#pragma message("__cplusplus version = " STRING(__cplusplus) )


#if defined(OF_HAS_CPP17)
#pragma message (">> OF_HAS_CPP17")
#endif

#if defined(OF_USING_STD_FS)
#pragma message (">> OF_USING_STD_FS")
#endif

#if defined(OF_USE_EXPERIMENTAL_FS)
#pragma message (">> OF_USE_EXPERIMENTAL_FS")
#endif
