#pragma once

#include "ofTexture.h"
#include "ofVideoBaseTypes.h"
#include "ofFbo.h"
#include "ofConstants.h"

//---------------------------------------------
class ofVideoWriter {
public:
	ofVideoWriter();
	ofVideoWriter(const of::filesystem::path & _fileName);
	void begin(const of::filesystem::path & _fileName);
	void addFrame();
	void end();
	void setFbo(ofFbo & _f);
	ofFbo * _fbo = nullptr;
	of::filesystem::path fileName;
};
