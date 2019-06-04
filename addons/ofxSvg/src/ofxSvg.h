#pragma once

//#include "ofMain.h"
#include "ofPath.h"
#include "ofTypes.h"
#include "ofXml.h"


/// \file
/// ofxSVG is used for loading and rendering SVG files. It's a wrapper
/// for the open source C library [Libsvgtiny](https://www.netsurf-browser.org/projects/libsvgtiny/ "Libsvgtiny website"),
/// and it supports files in the [SVG Tiny format](https://www.w3.org/TR/SVGMobile/ "SVG Tiny 1.2
/// format specification at the W3C").
///
/// Libsvgtiny supports a subset of SVG elements, (for a full list, see the Libsvgtiny readme file)
/// but we have gone some way to improving this by manually implementing some extra features (such as the
/// SVG "use" element).

class ofxSVG {
	public: ~ofxSVG();

		float getWidth() const ;
		float getHeight() const;
	
		/// \brief Loads an SVG file from the provided filename.
		///
		/// ~~~~
		void load(std::string path);
	
		/// \brief Loads an SVG from a text string.
		///
		/// Useful for parsing SVG text from sources other than a file. As the
		/// underlying SVG parsing library requires a url, this method gives
		/// you the option of providing one.
		///
		/// ~~~~
		void loadFromString(std::string data, std::string url="local");
	
		void draw();

		int getNumPath();
		ofPath & getPathAt(int n);
	
		const std::vector <ofPath> & getPaths() const;

	private:

		void fixSvgString(std::string& xmlstring);
	
		float width, height;

		std::vector <ofPath> paths;

		void setupDiagram(struct svgtiny_diagram * diagram);
		void setupShape(struct svgtiny_shape * shape, ofPath & path);

};
