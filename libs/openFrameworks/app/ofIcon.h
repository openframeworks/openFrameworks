#pragma once
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
