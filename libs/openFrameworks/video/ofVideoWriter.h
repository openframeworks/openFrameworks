#pragma once

#include "ofTexture.h"
#include "ofVideoBaseTypes.h"
#include "ofFbo.h"
#include "ofShader.h"
#include "ofConstants.h"
#ifdef __OBJC__
	#include "ofVideoWriterAVFoundation.h"
#endif

//---------------------------------------------
class ofVideoWriter {
public:
	ofVideoWriter();
	void setOutputFilename(const of::filesystem::path & _fileName);
	void setFps(int _fps);
	void begin();
	void addFrame();
	void end();
	void toggleRecording();
	void setFbo(ofFbo * _f);
	
//	bool autoOpen = false;
	bool autoOpen = true;
	
private:
	ofFbo * _fbo = nullptr;
	ofFbo fboShader;
	ofShader shader;
	
	of::filesystem::path fileName;
	bool isRecording = false;
	bool useCustomName = false;
	int32_t fps = 30;
	
#ifdef __OBJC__
	ofVideoWriterAVFoundation * videoWriter;
#else
	void * videoWriter;
#endif
};
