//
//  ofVideoWriterAVFoundation.m
//  Created by Dimitre Lima / Dmtr.org on 31/10/24.
//

#pragma once

#include "ofTexture.h"
#include "ofVideoBaseTypes.h"
#include "ofFbo.h"
#include "ofShader.h"
#include "ofConstants.h"
//#ifdef __OBJC__
	#include "ofVideoWriterAVFoundation.h"
//#endif

//---------------------------------------------
class ofVideoWriter {
public:
	ofVideoWriter();
	void setOutputFilename(const of::filesystem::path & _fileName);
	void setFps(int32_t _fps);
	void begin();
	void addFrame();
	void end();
	void toggleRecording();
	void setFbo(ofFbo * _f);
	void setScale(float s);
	
	bool autoOpen = true;
	
private:
	float scale = 1.0f;
	ofFbo * _fbo = nullptr;
	ofFbo fboShader;
	ofShader shader;
	
	of::filesystem::path fileName;
	bool isRecording = false;
	bool useCustomName = false;
	int32_t fps = 30;
	
	VW writer;
//#ifdef __OBJC__
//	ofVideoWriterAVFoundation * videoWriter;
//#else
//	void * videoWriter;
//#endif
};
