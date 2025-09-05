#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor( 230 );
	
	svgAddTypes = {
		OFXSVG_TYPE_RECTANGLE,
		OFXSVG_TYPE_CIRCLE,
		OFXSVG_TYPE_PATH
	};
	
	svg.load("ofxSvg.svg");
	
}

//--------------------------------------------------------------
void ofApp::update(){
	svg.setBoundsWidth(ofGetWidth());
	svg.setBoundsHeight(ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::draw(){
	svg.draw();
	ofSetColor( ofColor::cyan );
	polyline.draw();
	
	std::stringstream ss;
	ss << "Add Type (left/right): " << (svgTypeIndex+1) << " / " << svgAddTypes.size() << " - " << ofxSvgElement::sGetTypeAsString(svgAddTypes[svgTypeIndex]);
	if( svgAddTypes[svgTypeIndex] == OFXSVG_TYPE_PATH ) {
		ss << std::endl << "Click and drag mouse to add points.";
	}
	ss << std::endl << "Size (up/down): " << size;
	ss << std::endl << "Remove circles (c)";
	ss << std::endl << "Remove rectangles (r)";
	ss << std::endl << "Save (s)";
	ss << std::endl << "Clear (delete)";
	ofDrawBitmapStringHighlight(ss.str(), 40, 40);
}

//--------------------------------------------------------------
void ofApp::exit(){
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == OF_KEY_DEL || key == OF_KEY_BACKSPACE ) {
		svg.clear();
	}
	if( key == 's' ) {
		std::string filename = ofGetTimestampString()+".svg";
		ofLogNotice("ofApp") << "saving svg to file: " << filename;
		svg.save(filename);
	}
	
	if( key == 'c' ) {
		// get all of the circles
		auto circles = svg.getAllElementsForType<ofxSvgCircle>();
		// now lets remove all of them.
		svg.removeElements( circles );
		// another option would be to loop through the circles and call remove on each one.
//		for( auto& circle : circles ) {
//			svg.remove(circle);
//		}
	}
	if( key == 'r' ) {
		// our original document contains rectangles
		// or maybe paths since Illustrator sometimes converts them.
		// lets try grabbing them by name.
		// The second argument of this function determines if the name is strictly matched or not.
		// We pass in false because we name our rectangles with myrect + frame num.
		// So we want to get all of the rects that contain the string "myrect" in their name.
		auto myRects = svg.getAllElementsForTypeForName<ofxSvgRectangle>("myrect", false);
		svg.removeElements(myRects);
	}
	
	if( key == OF_KEY_RIGHT ) {
		svgTypeIndex++;
		svgTypeIndex %= svgAddTypes.size();
	}
	if( key == OF_KEY_LEFT ) {
		svgTypeIndex--;
		if( svgTypeIndex < 0 ) {
			svgTypeIndex = svgAddTypes.size()-1;
		}
	}
	if( key == OF_KEY_UP ) {
		size += 2;
	}
	if( key == OF_KEY_DOWN ) {
		size -= 2;
	}
	
	size = ofClamp( size, 2, 2000 );
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if(polyline.size() > 0 ) {
		polyline.addVertex(glm::vec3(x,y,0.f));
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	polyline.clear();
	svg.setFilled(true);
	svg.setHasStroke(false);
//	svg.setStrokeWidth(0);
	if( svgAddTypes[svgTypeIndex] == OFXSVG_TYPE_RECTANGLE ) {
		ofRectangle rect;
		rect.setFromCenter(x, y, size, size);
		svg.setFillColor(ofColor(255, x%255, (y*10) % 255));
		auto svgRect = svg.add( rect );
		svgRect->setName("myrect-"+ofToString(ofGetFrameNum()));
	} else if( svgAddTypes[svgTypeIndex] == OFXSVG_TYPE_CIRCLE ) {
		svg.setFillColor(ofColor(x%255, 205, (y*10) % 255));
		auto svgCircle = svg.addCircle( glm::vec2(x,y), size );
		svgCircle->setName("mycircle-"+ofToString(ofGetFrameNum()));
	} else if( svgAddTypes[svgTypeIndex] == OFXSVG_TYPE_PATH ) {
		svg.setFilled(false);
		svg.setStrokeColor(ofColor::magenta);
		svg.setStrokeWidth(3);
		polyline.addVertex(glm::vec3(x,y,0.f));
	}
	
	std::cout << "------------------------------------" << std::endl;
	std::cout << svg.toString() << std::endl;
	std::cout << "------------------------------------" << std::endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if( svgAddTypes[svgTypeIndex] == OFXSVG_TYPE_PATH ) {
		if( polyline.size() > 2 ) {
			auto svgPath = svg.add(polyline);
			svgPath->setName("p-"+ofToString(ofGetFrameNum()));
			polyline.clear();
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
