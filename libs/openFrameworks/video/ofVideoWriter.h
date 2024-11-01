#pragma once

#include "ofTexture.h"
#include "ofVideoBaseTypes.h"
#include "ofFbo.h"
#include "ofShader.h"
#include "ofConstants.h"
#ifdef __OBJC__
	#include "ofAVFoundationVideoWriter.h"
#endif

//---------------------------------------------
class ofVideoWriter {
public:
	ofVideoWriter();
	ofVideoWriter(const of::filesystem::path & _fileName);
//	void setOutputFile(const of::filesystem::path & _fileName);
	void begin(const of::filesystem::path & _fileName);
	void addFrame();
	void end();
	void setFbo(ofFbo * _f);
	
	ofFbo * _fbo = nullptr;
	ofFbo fboShader;
	ofShader shader;
	
	of::filesystem::path fileName;
	bool isBegin = false;
	
#ifdef __OBJC__
	ofAVFoundationVideoWriter * videoWriter;
#else
	void * videoWriter;
#endif
};
