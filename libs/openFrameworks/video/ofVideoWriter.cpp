#include "ofVideoWriter.h"
#include "ofAppRunner.h"
#include "ofGLUtils.h"
#include "ofPixels.h"
#include <algorithm>

#ifdef OF_VIDEO_PLAYER_AVFOUNDATION
	#include "ofAVFoundationWriter.h"
	#define OF_VID_WRITER_TYPE ofAVFoundationWriter
#endif

//---------------------------------------------------------------------------
ofVideoWriter::ofVideoWriter() {
}

//---------------------------------------------------------------------------
ofVideoWriter::ofVideoWriter(const of::filesystem::path & fileName) {
	setup(fileName);
}

//---------------------------------------------------------------------------
void ofVideoWriter::setup(const of::filesystem::path & _fileName) {
	fileName = _fileName;
	// open video file here
}

//---------------------------------------------------------------------------
void ofVideoWriter::addFrame() {
}

//---------------------------------------------------------------------------
void ofVideoWriter::finish() {
	// save video file here
}
