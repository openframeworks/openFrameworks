#pragma once

//#include "ofMain.h"
#include "svgtiny.h"
#include "ofPath.h"
#include "ofTypes.h"

class ofxSVG {
	public: ~ofxSVG();


		float getWidth() const {
			return width;
		}
		float getHeight() const {
			return height;
		}
		void load(string path);
		void draw();

		int getNumPath(){
			return paths.size();
		}
		ofPath & getPathAt(int n){
			return paths[n];
		}

	private:

		float width, height;

		vector <ofPath> paths;

		void setupDiagram(struct svgtiny_diagram * diagram);
		void setupShape(struct svgtiny_shape * shape, ofPath & path);

};
