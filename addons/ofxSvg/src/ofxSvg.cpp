#include "ofxSvg.h"
#include "ofConstants.h"

using namespace std;

extern "C"{
	#include "svgtiny.h"
}
ofxSVG::~ofxSVG(){
	paths.clear();
}

float ofxSVG::getWidth() const {
	return width;
}

float ofxSVG::getHeight() const {
	return height;
}

int ofxSVG::getNumPath(){
	return paths.size();
}
ofPath & ofxSVG::getPathAt(int n){
	return paths[n];
}

void ofxSVG::load(std::string path){
	path = ofToDataPath(path);

	if(path.compare("") == 0){
		ofLogError("ofxSVG") << "load(): path does not exist: \"" << path << "\"";
		return;
	}

	ofBuffer buffer = ofBufferFromFile(path);
	
	loadFromString(buffer.getText(), path);
	
}

void ofxSVG::loadFromString(std::string stringdata, std::string urlstring){
	
	// goes some way to improving SVG compatibility
	fixSvgString(stringdata);

	const char* data = stringdata.c_str();
	int size = stringdata.size();
	const char* url = urlstring.c_str();

	struct svgtiny_diagram * diagram = svgtiny_create();
	svgtiny_code code = svgtiny_parse(diagram, data, size, url, 0, 0);

	if(code != svgtiny_OK){
		string msg;
		switch(code){
		 case svgtiny_OUT_OF_MEMORY:
			 msg = "svgtiny_OUT_OF_MEMORY";
			 break;

		 /*case svgtiny_LIBXML_ERROR:
			 msg = "svgtiny_LIBXML_ERROR";
			 break;*/

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
		ofLogError("ofxSVG") << "load(): couldn't parse \"" << urlstring << "\": " << msg;
	}

	setupDiagram(diagram);

	svgtiny_free(diagram);
}

void ofxSVG::fixSvgString(std::string& xmlstring) {
	
	ofXml xml;
	
	xml.parse(xmlstring);
	
	// so it turns out that if the stroke width is <1 it rounds it down to 0,
	// and makes it disappear because svgtiny stores strokewidth as an integer!
	ofXml::Search strokeWidthElements = xml.find("//*[@stroke-width]");
	if(!strokeWidthElements.empty()) {
		
		for(ofXml & element: strokeWidthElements){
			//cout << element.toString() << endl;
			float strokewidth = element.getAttribute("stroke-width").getFloatValue();
			strokewidth = MAX(1,round(strokewidth));
			element.getAttribute("stroke-width").set(strokewidth);
			
		}
	}
	
	//lib svgtiny doesn't remove elements with display = none, so this code fixes that
	
	bool finished = false;
	while(!finished) {
		
		ofXml::Search invisibleElements  = xml.find("//*[@display=\"none\"]");
		
		if(invisibleElements.empty()) {
			finished = true;
		} else {
			const ofXml& element = invisibleElements[0];
			ofXml parent = element.getParent();
			if(parent && element) parent.removeChild(element);
		}
		
	}
	
	// implement the SVG "use" element by expanding out those elements into
	// XML that svgtiny will parse correctly.
	ofXml::Search useElements = xml.find("//use");
	if(!useElements.empty()) {
		
		for(ofXml & element: useElements){
			
			// get the id attribute
			string id = element.getAttribute("xlink:href").getValue();
			// remove the leading "#" from the id
			id.erase(id.begin());
			
			// find the original definition of that element - TODO add defs into path?
			string searchstring ="//*[@id='"+id+"']";
			ofXml idelement = xml.findFirst(searchstring);
			
			// if we found one then use it! (find first returns an empty xml on failure)
			if(idelement.getAttribute("id").getValue()!="") {
				
				// make a copy of that element
				element.appendChild(idelement);
				
				// then turn the use element into a g element
				element.setName("g");
				
			}
		}
	}
	
	xmlstring = xml.toString();
	
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
