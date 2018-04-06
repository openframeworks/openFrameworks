//
//  ofxiOSConstants.h
//  iOS+OFLib
//
//  Created by Lukasz Karluk on 25/09/2014.
//  http://julapy.com
//

#pragma once

// iOS Device Types.
//-------------------------------------------------------------------------------
enum ofxiOSDeviceType {
    OFXIOS_DEVICE_IPHONE = 0,
    OFXIOS_DEVICE_IPODTOUCH,
    OFXIOS_DEVICE_IPAD,
    OFXIOS_DEVICE_APPLETV,
    OFXIOS_DEVICE_UNKNOWN
};


// iOS Renderer Types.
//-------------------------------------------------------------------------------
enum ofxiOSRendererType {
    OFXIOS_RENDERER_ES1 = 1,
    OFXIOS_RENDERER_ES2 = 2,
    OFXIOS_RENDERER_ES3 = 3,
    OFXIOS_RENDERER_METAL = 4
};

enum ofxiOSWindowControllerType{
    CORE_ANIMATION = 1,
    GL_KIT = 2,
    METAL_KIT = 3
};

enum ofxiOSRendererColorFormat{
    RGBA8888 = 0,
    RGB565,
    SRGBA8888,
};

enum ofxiOSRendererDepthFormat{
    DEPTH_NONE = 0,
    DEPTH_16,
    DEPTH_24,
};

enum ofxiOSRendererStencilFormat{
    STENCIL_NONE = 0,
    STENCIL_8,
};

// Orientation.
//-------------------------------------------------------------------------------
#define OFXIOS_ORIENTATION_PORTRAIT          OF_ORIENTATION_DEFAULT  // UIDeviceOrientationPortrait
#define OFXIOS_ORIENTATION_UPSIDEDOWN        OF_ORIENTATION_180      // UIDeviceOrientationPortraitUpsideDown
#define OFXIOS_ORIENTATION_LANDSCAPE_RIGHT   OF_ORIENTATION_90_RIGHT // UIDeviceOrientationLandscapeRight
#define OFXIOS_ORIENTATION_LANDSCAPE_LEFT    OF_ORIENTATION_90_LEFT  // UIDeviceOrientationLandscapeLeft


// Possible return values for ofxiOSGetDeviceRevision
// NOTE that these #defines are deprecated in favor of ofxiOSGetDeviceInfo()
//-------------------------------------------------------------------------------
#define OFXIOS_DEVICE_IPHONE_2G         "iPhone1,1"     // iPhone (Original/1st Gen/EDGE) - January 2007
#define OFXIOS_DEVICE_IPHONE_3G         "iPhone1,2"     // iPhone 3G - June 2008
#define OFXIOS_DEVICE_IPHONE_3GS        "iPhone2,1"     // iPhone 3GS - June 2009
#define OFXIOS_DEVICE_IPHONE_4          "iPhone3,1"     // iPhone 4 (GSM) - June 2010

#define OFXIOS_DEVICE_IPOD_1STGEN       "iPod1,1"       // iPod touch (Original/1st Gen) - September 2007
#define OFXIOS_DEVICE_IPOD_2NDGEN       "iPod2,1"       // iPod touch (2nd Gen) - September 2008
#define OFXIOS_DEVICE_IPOD_3RDGEN       "iPod3,1"       // iPod touch (3rd Gen/32 & 64 GB) - September 2009
#define OFXIOS_DEVICE_IPOD_4THGEN       "iPod4,1"       // iPod touch (4th Gen/FaceTime) - September 2010

#define OFXIOS_DEVICE_IPAD_1STGEN       "iPad1,1"       // iPad Wi-Fi/3G/GPS (Original/1st Gen) - January 2010


// backwards compatibility < 0.8.0
//-------------------------------------------------------------------------------
#define ofxiPhone_DEVICE_IPHONE                 OFXIOS_DEVICE_IPHONE
#define ofxiPhone_DEVICE_IPODTOUCH              OFXIOS_DEVICE_IPODTOUCH
#define ofxiPhone_DEVICE_IPAD                   OFXIOS_DEVICE_IPAD
#define ofxiPhone_UNKNOWN_DEVICE                OFXIOS_DEVICE_UNKNOWN
#define ofxiPhone_DEVICE_IPHONE_2G              OFXIOS_DEVICE_IPHONE_2G
#define ofxiPhone_DEVICE_IPHONE_3G              OFXIOS_DEVICE_IPHONE_3G
#define ofxiPhone_DEVICE_IPHONE_3GS             OFXIOS_DEVICE_IPHONE_3GS
#define ofxiPhone_DEVICE_IPHONE_4               OFXIOS_DEVICE_IPHONE_4
#define ofxiPhone_DEVICE_IPOD_1STGEN            OFXIOS_DEVICE_IPOD_1STGEN
#define ofxiPhone_DEVICE_IPOD_2NDGEN            OFXIOS_DEVICE_IPOD_2NDGEN
#define ofxiPhone_DEVICE_IPOD_3RDGEN            OFXIOS_DEVICE_IPOD_3RDGEN
#define ofxiPhone_DEVICE_IPOD_4THGEN            OFXIOS_DEVICE_IPOD_4THGEN
#define ofxiPhone_DEVICE_IPAD_1STGEN            OFXIOS_DEVICE_IPAD_1STGEN

#define ofxiPhone_ORIENTATION_PORTRAIT          OFXIOS_ORIENTATION_PORTRAIT
#define ofxiPhone_ORIENTATION_UPSIDEDOWN        OFXIOS_ORIENTATION_UPSIDEDOWN
#define ofxiPhone_ORIENTATION_LANDSCAPE_RIGHT   OFXIOS_ORIENTATION_LANDSCAPE_RIGHT
#define ofxiPhone_ORIENTATION_LANDSCAPE_LEFT    OFXIOS_ORIENTATION_LANDSCAPE_LEFT


// backwards compatibility == 0.8.1
//-------------------------------------------------------------------------------
#define ofxiOS_DEVICE_IPHONE                    OFXIOS_DEVICE_IPHONE
#define ofxiOS_DEVICE_IPODTOUCH                 OFXIOS_DEVICE_IPODTOUCH
#define ofxiOS_DEVICE_IPAD                      OFXIOS_DEVICE_IPAD
#define ofxiOS_UNKNOWN_DEVICE                   OFXIOS_DEVICE_UNKNOWN
#define ofxiOS_DEVICE_IPHONE_2G                 OFXIOS_DEVICE_IPHONE_2G
#define ofxiOS_DEVICE_IPHONE_3G                 OFXIOS_DEVICE_IPHONE_3G
#define ofxiOS_DEVICE_IPHONE_3GS                OFXIOS_DEVICE_IPHONE_3GS
#define ofxiOS_DEVICE_IPHONE_4                  OFXIOS_DEVICE_IPHONE_4
#define ofxiOS_DEVICE_IPOD_1STGEN               OFXIOS_DEVICE_IPOD_1STGEN
#define ofxiOS_DEVICE_IPOD_2NDGEN               OFXIOS_DEVICE_IPOD_2NDGEN
#define ofxiOS_DEVICE_IPOD_3RDGEN               OFXIOS_DEVICE_IPOD_3RDGEN
#define ofxiOS_DEVICE_IPOD_4THGEN               OFXIOS_DEVICE_IPOD_4THGEN
#define ofxiOS_DEVICE_IPAD_1STGEN               OFXIOS_DEVICE_IPAD_1STGEN

#define ofxiOS_ORIENTATION_PORTRAIT             OFXIOS_ORIENTATION_PORTRAIT
#define ofxiOS_ORIENTATION_UPSIDEDOWN           OFXIOS_ORIENTATION_UPSIDEDOWN
#define ofxiOS_ORIENTATION_LANDSCAPE_RIGHT      OFXIOS_ORIENTATION_LANDSCAPE_RIGHT
#define ofxiOS_ORIENTATION_LANDSCAPE_LEFT       OFXIOS_ORIENTATION_LANDSCAPE_LEFT
