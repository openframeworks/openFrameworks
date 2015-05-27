#include "ofxSvg.h"
#include "ofConstants.h"

ofxSVG::~ofxSVG(){
	paths.clear();
}

void ofxSVG::load(string path){
	path = ofToDataPath(path);

	if(path.compare("") == 0){
		ofLogError("ofxSVG") << "load(): path does not exist: \"" << path << "\"";
		return;
	}

	ofBuffer buffer = ofBufferFromFile(path);
	size_t size = buffer.size();

	struct svgtiny_diagram * diagram = svgtiny_create();
	svgtiny_code code = svgtiny_parse(diagram, buffer.getText().c_str(), size, path.c_str(), 0, 0);

	if(code != svgtiny_OK){
		string msg;
		switch(code){
		 case svgtiny_OUT_OF_MEMORY:
			 msg = "svgtiny_OUT_OF_MEMORY";
			 break;

		 case svgtiny_LIBXML_ERROR:
			 msg = "svgtiny_LIBXML_ERROR";
			 break;

		 case svgtiny_NOT_SVG:
			 msg = "svgtiny_NOT_SVG";
			 break;

		 case svgtiny_SVG_ERROR:
			 msg = "svgtiny_SVG_ERROR: line " + ofToString(diagram->error_line) + ": " + diagram->error_message;
			 break;

		 default:
			 msg = "unknown svgtiny_code " + ofToString(code);
			 break;
		}
		ofLogError("ofxSVG") << "load(): couldn't parse \"" << path << "\": " << msg;
	}

	setupDiagram(diagram);

	svgtiny_free(diagram);
}

void ofxSVG::draw(){
	for(int i = 0; i < (int)paths.size(); i++){
		paths[i].draw();
	}
}


void ofxSVG::setupDiagram(struct svgtiny_diagram * diagram){

	width = diagram->width;
	height = diagram->height;

	paths.clear();

	for(int i = 0; i < (int)diagram->shape_count; i++){
		if(diagram->shape[i].path){
			paths.push_back(ofPath());
			setupShape(&diagram->shape[i],paths.back());
		}else if(diagram->shape[i].text){
			ofLogWarning("ofxSVG") << "setupDiagram(): text: not implemented yet";
		}
	}
}

void ofxSVG::setupShape(struct svgtiny_shape * shape, ofPath & path){
	float * p = shape->path;

	path.setFilled(false);

	if(shape->fill != svgtiny_TRANSPARENT){
		path.setFilled(true);
		path.setFillHexColor(shape->fill);
		path.setPolyWindingMode(OF_POLY_WINDING_NONZERO);
    }

	if(shape->stroke != svgtiny_TRANSPARENT){
		path.setStrokeWidth(shape->stroke_width);
		path.setStrokeHexColor(shape->stroke);
	}

	for(int i = 0; i < (int)shape->path_length;){
		if(p[i] == svgtiny_PATH_MOVE){
			path.moveTo(p[i + 1], p[i + 2]);
			i += 3;
		}
		else if(p[i] == svgtiny_PATH_CLOSE){
			path.close();

			i += 1;
		}
		else if(p[i] == svgtiny_PATH_LINE){
			path.lineTo(p[i + 1], p[i + 2]);
			i += 3;
		}
		else if(p[i] == svgtiny_PATH_BEZIER){
			path.bezierTo(p[i + 1], p[i + 2],
						   p[i + 3], p[i + 4],
						   p[i + 5], p[i + 6]);
			i += 7;
		}
		else{
			ofLogError("ofxSVG") << "setupShape(): SVG parse error";
			i += 1;
		}
	}
}

const vector <ofPath> & ofxSVG::getPaths() const{
    return paths;
}
