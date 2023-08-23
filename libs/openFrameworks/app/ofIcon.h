#pragma once
#include "utils/ofConstants.h"

#if defined(TARGET_WIN32) || defined(TARGET_LINUX)
    #define TARGET_HAS_WINDOW_ICON
#endif


#include "graphics/ofPixels.h"

/**
 * @brief Get the application icon. Default to OF icon if not set/found
 *
 * 
 * @return const ofPixels& 
 */
const ofPixels getIcon();

/**
 * @brief Get the OF icon.
 *
 * @note if openFrameworks is compiled with DEBUG, it will be the OF-debug icon.
 * 
 * @return const ofPixels& 
 */
const ofPixels getOfIcon();
