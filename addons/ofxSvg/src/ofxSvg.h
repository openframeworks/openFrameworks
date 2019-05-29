#pragma once

//#include "ofMain.h"
#include "ofPath.h"
#include "ofTypes.h"
#include "ofXml.h"

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
		/// As the underlying SVG parsing library requires a url, this method
		/// gives you the option of providing one.
		///
		/// ~~~~
		void loadFromString(std::string data, std::string url="local");
	
		/// \brief Enabled or disable a mode that improves compatibility.
		///
		/// Set to true by default, this mode adds a number of improved
		/// SVG parsing features. Turning it off will increase performance
		/// but fewer SVG files will work.
		///
		/// ~~~~
		void setImprovedCompatibilityMode(bool enabled);
	
		void draw();

		int getNumPath();
		ofPath & getPathAt(int n);
	
		const std::vector <ofPath> & getPaths() const;

	private:

		void fixSvgText(std::string& xmlstring);
	
		bool increaseCompatibility = true;
	
		float width, height;

		std::vector <ofPath> paths;

		void setupDiagram(struct svgtiny_diagram * diagram);
		void setupShape(struct svgtiny_shape * shape, ofPath & path);

};
