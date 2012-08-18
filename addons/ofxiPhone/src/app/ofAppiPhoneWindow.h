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

class ofAppiPhoneWindow : public ofAppBaseWindow{
public:
	
	ofAppiPhoneWindow();
	virtual ~ofAppiPhoneWindow(){};
	
	virtual void		setupOpenGL(int w, int h, int screenMode);
	virtual void		initializeWindow();
	virtual void		runAppViaInfiniteLoop(ofBaseApp * appPtr);
    virtual void        startAppWithDelegate(string appDelegateClassName);
	
	virtual void		hideCursor() {};
	virtual void		showCursor() {};
    
	virtual void		setWindowPosition(int x, int y) {};
	virtual void		setWindowShape(int w, int h) {};
	
	virtual int			getFrameNum();
	virtual float		getFrameRate();
	virtual double		getLastFrameTime();
	
	virtual ofPoint		getWindowPosition();
	virtual ofPoint		getWindowSize();
	virtual ofPoint		getScreenSize();
	
	virtual int			getWidth();
	virtual int			getHeight();
	
	virtual void		setFrameRate(float targetRate);
	virtual void		setWindowTitle(string title);
	
	virtual int			getWindowMode();
	
	virtual void		setFullscreen(bool fullscreen);
	virtual void		toggleFullscreen();
	
	virtual void		enableSetupScreen();
	virtual void		disableSetupScreen();
    virtual bool        isSetupScreenEnabled();
	
	void				setOrientation(ofOrientation orientation);
	ofOrientation		getOrientation();
	void				rotateXY(float &x, float &y);		// updates
	
    bool                enableRetina();
    bool                disableRetina();
    bool                isRetinaEnabled();
    bool                isRetinaSupportedOnDevice();
    
    bool                enableDepthBuffer();
    bool                disableDepthBuffer();
    bool                isDepthBufferEnabled();
    
    bool                enableAntiAliasing(int samples);
    bool                disableAntiAliasing();
    bool                isAntiAliasingEnabled();
    int					getAntiAliasingSampleCount();
    
    //---------------------------------------------- deprecation messages. to be removed in OF 0073.
    OF_DEPRECATED_MSG("Use enableRetina() instead", void enableRetinaSupport());
    OF_DEPRECATED_MSG("Use isRetinaSupportedOnDevice() instead", bool isRetinaSupported());
    OF_DEPRECATED_MSG("Use isDepthBufferEnabled() instead", bool isDepthEnabled());
	
	void timerLoop();
	int					windowMode;
	
	static ofAppiPhoneWindow* getInstance();
	
protected:

	bool bEnableSetupScreen;
	ofOrientation orientation;
	
    bool bRetinaEnabled;
    bool bRetinaSupportedOnDevice;
    bool bRetinaSupportedOnDeviceChecked;
	bool bDepthEnabled;
	bool bAntiAliasingEnabled;
	int antiAliasingSamples;
};



