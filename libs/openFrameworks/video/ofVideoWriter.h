#pragma once

#include "ofTexture.h"
#include "ofVideoBaseTypes.h"

//---------------------------------------------
class ofVideoWriter {
public:
	ofVideoWriter();
	ofVideoWriter(const of::filesystem::path & _fileName);
	void setup(const of::filesystem::path & _fileName);
	void addFrame();
	void finish();
	of::filesystem::path fileName;
};
