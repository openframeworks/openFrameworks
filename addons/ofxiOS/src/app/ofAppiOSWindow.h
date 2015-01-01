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

#import "ofAppBaseWindow.h"
#include "ofxiOSConstants.h"
#include "ofWindowSettings.h"

class ofiOSWindowSettings: public ofGLESWindowSettings{
public:
    ofiOSWindowSettings()
    :enableRetina(false)
    ,retinaScale(0)
    ,enableDepth(false)
    ,enableAntiAliasing(false)
    ,numOfAntiAliasingSamples(0)
    ,enableHardwareOrientation(false)
    ,enableHardwareOrientationAnimation(false)
    ,enableSetupScreen(true) {
        windowMode = OF_FULLSCREEN;
        glesVersion = 1;
    }
    
    ofiOSWindowSettings(const ofWindowSettings & settings)
    :ofGLESWindowSettings(settings)
    ,enableRetina(false)
    ,retinaScale(0)
    ,enableDepth(false)
    ,enableAntiAliasing(false)
    ,numOfAntiAliasingSamples(0)
    ,enableHardwareOrientation(false)
    ,enableHardwareOrientationAnimation(false)
    ,enableSetupScreen(true) {
        const ofGLESWindowSettings * glesSettings = dynamic_cast<const ofGLESWindowSettings*>(&settings);
        if(glesSettings){
            glesVersion = glesSettings->glesVersion;
        } else {
            glesVersion = 1;
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
        } else {
            enableRetina = false;
            retinaScale = 0;
            enableDepth = false;
            enableAntiAliasing = false;
            numOfAntiAliasingSamples = 0;
            enableHardwareOrientation = false;
            enableHardwareOrientationAnimation = false;
            enableSetupScreen = true;
        }
    }

    ofiOSWindowSettings(const ofGLESWindowSettings & settings)
    :ofGLESWindowSettings(settings)
    ,enableRetina(false)
    ,retinaScale(0)
    ,enableDepth(false)
    ,enableAntiAliasing(false)
    ,numOfAntiAliasingSamples(0)
    ,enableHardwareOrientation(false)
    ,enableHardwareOrientationAnimation(false)
    ,enableSetupScreen(true) {
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
    
    void run(ofBaseApp * appPtr);
    OF_DEPRECATED_MSG("Use setup(const ofiOSWindowSettings & settings); instead.", virtual void setupOpenGL(int w, int h, ofWindowMode screenMode) );
    static void startAppWithDelegate(string appDelegateClassName);
    void update();
    void draw();
   
    void close();
    
	virtual void hideCursor();
	virtual void showCursor();
    
	virtual void setWindowPosition(int x, int y);
	virtual void setWindowShape(int w, int h);
		
	virtual ofPoint getWindowPosition();
	virtual ofPoint getWindowSize();
	virtual ofPoint getScreenSize();
    
	virtual void setOrientation(ofOrientation orientation);
	virtual ofOrientation getOrientation();
    virtual bool doesHWOrientation();
	
	virtual int getWidth();
	virtual int getHeight();
    
    ofiOSWindowSettings & getSettings();
    ofCoreEvents & events();
    shared_ptr<ofBaseRenderer> & renderer();
	
	virtual void setWindowTitle(string title);
	
	virtual ofWindowMode getWindowMode();
	
	virtual void setFullscreen(bool fullscreen);
	virtual void toggleFullscreen();
	
	virtual void enableSetupScreen();
	virtual void disableSetupScreen();
    virtual bool isSetupScreenEnabled();
    
    virtual void setVerticalSync(bool enabled);
		
    //-------------------------------------------- ios config.
    bool enableHardwareOrientation();
    bool disableHardwareOrientation();

    bool enableOrientationAnimation();
    bool disableOrientationAnimation();
    
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
    
protected:
    
    ofCoreEvents coreEvents;
    shared_ptr<ofBaseRenderer> currentRenderer;
    ofiOSWindowSettings settings;

	ofOrientation orientation;
	
    bool bRetinaSupportedOnDevice;
    bool bRetinaSupportedOnDeviceChecked;
    
    bool hasExited;
};

OF_DEPRECATED_MSG("ofAppiPhoneWindow is deprecated, use ofAppiOSWindow instead.", typedef ofAppiOSWindow ofAppiPhoneWindow);



