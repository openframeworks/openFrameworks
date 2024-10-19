#include "ofVideoWriter.h"

#ifdef __OBJC__
	#include "ofAVFoundationVideoWriter.h"
#endif

//---------------------------------------------------------------------------
ofVideoWriter::ofVideoWriter() {
}

//---------------------------------------------------------------------------
ofVideoWriter::ofVideoWriter(const of::filesystem::path & fileName) {
	// FIXME: Begin or setup?
	begin(fileName);
}

//---------------------------------------------------------------------------
void ofVideoWriter::setFbo(ofFbo & _f) {
	_fbo = _f;
	// save video file here
}
//---------------------------------------------------------------------------
void ofVideoWriter::begin(const of::filesystem::path & _fileName) {
	fileName = _fileName;
	// open video file here
}

//---------------------------------------------------------------------------
void ofVideoWriter::addFrame() {
}

//---------------------------------------------------------------------------
void ofVideoWriter::end() {
	// save video file here
}
