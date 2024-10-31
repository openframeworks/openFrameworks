#pragma once

#include "ofRectangle.h"
#include "ofConstants.h"
#if defined(OF_CAIRO)

/// \}
/// \name Screen Saving
/// \{

/// \brief Begin rendering to a PDF file.
///
/// openFrameworks allows rendering of 2D graphics to pdf via the
/// ofCairoRenderer. ofBeginSaveScreenAsPDF() is called before drawing. When
/// done drawing call ofEndSaveScreenAsPDF() to output the file.
///
/// ~~~~{.cpp}
/// void ofApp::setup(){
///     ofBeginSaveScreenAsPDF("screenshot.pdf", false);
///     ofSetColor(54,54,54);
///     ofDrawEllipse(100,100,200,200);
///     ofEndSaveScreenAsPDF();
/// }
/// ~~~~
/// \sa End drawing with ofEndSaveScreenAsPDF()
///
void ofBeginSaveScreenAsPDF(const of::filesystem::path & fileName, bool bMultipage = false, bool b3D = false, ofRectangle outputsize = ofRectangle(0,0,0,0));

/// \brief Terminates draw to PDF through ofCairoRenderer and outputs the file.
/// \sa ofBeginSaveScreenAsPDF()
void ofEndSaveScreenAsPDF();

/// \brief Begin rendering to a SVG file.
/// \sa ofEndSaveScreenAsSVG(), ofBeginSaveScreenAsPDF()
void ofBeginSaveScreenAsSVG(const of::filesystem::path & fileName, bool bMultipage = false, bool b3D = false, ofRectangle outputsize = ofRectangle(0,0,0,0));

/// \brief Terminates draw to SVG and outputs the file.
/// \sa ofBeginSaveScreenAsSVG()
void ofEndSaveScreenAsSVG();

#endif
