/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/

#pragma once
#include <TargetConditionals.h>
#include "ofAppBaseWindow.h"
#include "ofxiOSConstants.h"
#include "ofEvents.h"

class ofiOSWindowSettings: public ofGLESWindowSettings{
public:
    ofiOSWindowSettings()
    :enableRetina(true)
    ,retinaScale(0)
    ,enableDepth(false)
    ,enableAntiAliasing(false)
    ,numOfAntiAliasingSamples(0)
    ,enableHardwareOrientation(false)
    ,enableHardwareOrientationAnimation(false)
    ,enableSetupScreen(true)
    ,windowControllerType(ofxiOSWindowControllerType::CORE_ANIMATION)
    ,colorType(ofxiOSRendererColorFormat::RGBA8888)
    ,depthType(ofxiOSRendererDepthFormat::DEPTH_NONE)
    ,stencilType(ofxiOSRendererStencilFormat::STENCIL_NONE)
    ,enableMultiTouch(false) {
        windowMode = OF_FULLSCREEN;
        setupOrientation = OF_ORIENTATION_DEFAULT;
        glesVersion = 2;
    }
    
    ofiOSWindowSettings(const ofWindowSettings & settings)
    :ofGLESWindowSettings(settings)
    ,enableRetina(true)
    ,retinaScale(0)
    ,enableDepth(false)
    ,enableAntiAliasing(false)
    ,numOfAntiAliasingSamples(0)
    ,enableHardwareOrientation(false)
    ,enableHardwareOrientationAnimation(false)
    ,enableSetupScreen(true)
    ,windowControllerType(ofxiOSWindowControllerType::CORE_ANIMATION)
    ,colorType(ofxiOSRendererColorFormat::RGBA8888)
    ,depthType(ofxiOSRendererDepthFormat::DEPTH_NONE)
    ,stencilType(ofxiOSRendererStencilFormat::STENCIL_NONE)
    ,enableMultiTouch(false) {
        const ofGLESWindowSettings * glesSettings = dynamic_cast<const ofGLESWindowSettings*>(&settings);
        if(glesSettings){
            glesVersion = glesSettings->glesVersion;
        } else {
            glesVersion = 2;
        }
        const ofiOSWindowSettings * iosSettings = dynamic_cast<const ofiOSWindowSettings*>(&settings);
        if(iosSettings){
            enableRetina = iosSettings->enableRetina;
            retinaScale = iosSettings->retinaScale;
            enableDepth = iosSettings->enableDepth;
            enableAntiAliasing = iosSettings->enableAntiAliasing;
            numOfAntiAliasingSamples = iosSettings->numOfAntiAliasingSamples;
            enableHardwareOrientation = iosSettings->enableHardwareOrientation;
            enableHardwareOrientationAnimation = iosSettings->enableHardwareOrientationAnimation;
            enableSetupScreen = iosSettings->enableSetupScreen;
            setupOrientation = iosSettings->setupOrientation;
            windowControllerType = iosSettings->windowControllerType;
            colorType = iosSettings->colorType;
            depthType = iosSettings->depthType;
            stencilType = iosSettings->stencilType;
            enableMultiTouch = iosSettings->enableMultiTouch;
        } else {
            enableRetina = true;
            retinaScale = 0;
            enableDepth = false;
            enableAntiAliasing = false;
            numOfAntiAliasingSamples = 0;
            enableHardwareOrientation = false;
            enableHardwareOrientationAnimation = false;
            enableSetupScreen = true;
            setupOrientation = OF_ORIENTATION_DEFAULT;
            colorType = ofxiOSRendererColorFormat::RGBA8888;
            depthType = ofxiOSRendererDepthFormat::DEPTH_NONE;
            stencilType = ofxiOSRendererStencilFormat::STENCIL_NONE;
            enableMultiTouch = false;
        }
    }

    ofiOSWindowSettings(const ofGLESWindowSettings & settings)
    :ofGLESWindowSettings(settings)
    ,enableRetina(true)
    ,retinaScale(0)
    ,enableDepth(false)
    ,enableAntiAliasing(false)
    ,numOfAntiAliasingSamples(0)
    ,enableHardwareOrientation(false)
    ,enableHardwareOrientationAnimation(false)
    ,enableSetupScreen(true)
    ,windowControllerType(ofxiOSWindowControllerType::CORE_ANIMATION)
    ,colorType(ofxiOSRendererColorFormat::RGBA8888)
    ,depthType(ofxiOSRendererDepthFormat::DEPTH_NONE)
    ,stencilType(ofxiOSRendererStencilFormat::STENCIL_NONE)
    ,enableMultiTouch(false){
        const ofiOSWindowSettings * iosSettings = dynamic_cast<const ofiOSWindowSettings*>(&settings);
        if(iosSettings){
            enableRetina = iosSettings->enableRetina;
            retinaScale = iosSettings->retinaScale;
            enableDepth = iosSettings->enableDepth;
            enableAntiAliasing = iosSettings->enableAntiAliasing;
            numOfAntiAliasingSamples = iosSettings->numOfAntiAliasingSamples;
            enableHardwareOrientation = iosSettings->enableHardwareOrientation;
            enableHardwareOrientationAnimation = iosSettings->enableHardwareOrientationAnimation;
            enableSetupScreen = iosSettings->enableSetupScreen;
            setupOrientation = iosSettings->setupOrientation;
            windowControllerType = iosSettings->windowControllerType;
            colorType = iosSettings->colorType;
            depthType = iosSettings->depthType;
            stencilType = iosSettings->stencilType;
            enableMultiTouch = iosSettings->enableMultiTouch;
        }
    }

    virtual ~ofiOSWindowSettings(){};
    
    bool enableRetina;
    float retinaScale;
    bool enableDepth;
    bool enableAntiAliasing;
    int numOfAntiAliasingSamples;
    bool enableHardwareOrientation;
    bool enableHardwareOrientationAnimation;
    bool enableSetupScreen;
    bool enableMultiTouch;
    ofxiOSWindowControllerType windowControllerType;
    ofxiOSRendererColorFormat colorType;
    ofxiOSRendererDepthFormat depthType;
    ofxiOSRendererStencilFormat stencilType;
    ofOrientation setupOrientation;
    
};


class ofAppiOSWindow : public ofAppBaseGLESWindow {
public:

    static ofAppiOSWindow * getInstance();
    
    ofAppiOSWindow();
    ~ofAppiOSWindow();
    
    static void loop();
    static bool doesLoop(){ return true; }
    static bool allowsMultiWindow(){ return false; }
    static bool needsPolling(){ return false; }
    static void pollEvents(){ }
    
    void setup(const ofWindowSettings & _settings);
    void setup(const ofGLESWindowSettings & _settings);
    void setup(const ofiOSWindowSettings & _settings);
    void setup();
    
    void run(ofBaseApp * appPtr);
    OF_DEPRECATED_MSG("Use setup(const ofiOSWindowSettings & settings); instead.", virtual void setupOpenGL(int w, int h, ofWindowMode screenMode) );
    static void startAppWithDelegate(std::string appDelegateClassName);
    void update();
    void draw();
   
    void close();
    
    virtual void hideCursor();
    virtual void showCursor();
    
    virtual void setWindowPosition(int x, int y);
    virtual void setWindowShape(int w, int h);
        
    virtual glm::vec2 getWindowPosition();
    virtual glm::vec2 getWindowSize();
    virtual glm::vec2 getScreenSize();
    
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
    virtual void setOrientation(ofOrientation orientation);
    virtual ofOrientation getOrientation();
    virtual bool doesHWOrientation();
    //-------------------------------------------- ios config.
    bool enableHardwareOrientation();
    bool disableHardwareOrientation();
    
    bool enableOrientationAnimation();
    bool disableOrientationAnimation();
#endif
    
    virtual int getWidth();
    virtual int getHeight();
    
    ofiOSWindowSettings & getSettings();
    ofCoreEvents & events();
    std::shared_ptr<ofBaseRenderer> & renderer();
    
    virtual void setWindowTitle(std::string title);
    
    virtual ofWindowMode getWindowMode();
    
    virtual void setFullscreen(bool fullscreen);
    virtual void toggleFullscreen();
    
    virtual void enableSetupScreen();
    virtual void disableSetupScreen();
    virtual bool isSetupScreenEnabled();
    
    virtual void setVerticalSync(bool enabled);
        
    bool isProgrammableRenderer();
    ofxiOSRendererType getGLESVersion();
    OF_DEPRECATED_MSG("Use ofiOSWindowSettings to setup programmable renderer by selecting glesVerison to >=2", bool enableRendererES2());
    OF_DEPRECATED_MSG("Use ofiOSWindowSettings to setup  non-programmable renderer by selecting glesVersion Version to 1", bool enableRendererES1());
    OF_DEPRECATED_MSG("Use isProgrammableRenderer() or getGLESVersion()", bool isRendererES2());
    OF_DEPRECATED_MSG("Use isProgrammableRenderer() or getGLESVersion()", bool isRendererES1());
    
    bool enableRetina(float retinaScale=0);
    bool disableRetina();
    bool isRetinaEnabled();
    bool isRetinaSupportedOnDevice();
    float getRetinaScale();
    
    bool enableDepthBuffer();
    bool disableDepthBuffer();
    bool isDepthBufferEnabled();
    
    bool enableAntiAliasing(int samples);
    bool disableAntiAliasing();
    bool isAntiAliasingEnabled();
    int getAntiAliasingSampleCount();
    
    void enableMultiTouch(bool isOn);
    bool isMultiTouch();
    ofxiOSWindowControllerType getWindowControllerType();
    ofxiOSRendererColorFormat getRendererColorType();
    ofxiOSRendererDepthFormat getRendererDepthType();
    ofxiOSRendererStencilFormat getRendererStencilType();
    
protected:
    
    ofCoreEvents coreEvents;
    std::shared_ptr<ofBaseRenderer> currentRenderer;
    ofiOSWindowSettings settings;

    ofOrientation orientation;
    
    bool bRetinaSupportedOnDevice;
    bool bRetinaSupportedOnDeviceChecked;
    
    bool hasExited;
};

OF_DEPRECATED_MSG("ofAppiPhoneWindow is deprecated, use ofAppiOSWindow instead.", typedef ofAppiOSWindow ofAppiPhoneWindow);



