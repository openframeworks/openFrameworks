#if TARGET_API_MAC_CARBON

#ifdef __APPLE_CC__
	#include <Carbon/Carbon.h>
#else
	#include <Carbon.h>
#endif
#else
	#include <Types.h>
	#include <OSA.h>
	#include <AppleScript.h>
#endif

#include <string.h>

OSStatus SimpleRunAppleScript(const char* theScript);

