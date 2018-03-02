#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// load the file and check if it was loaded ok. Also check it's
	// format is correct, it needs to have:
	//
	// - a root called drawing which
	// - contains a background element which itself contains nodes
	//   red, green and blue
	auto isLoaded = xml.load_file(ofToDataPath("points.xml").c_str()) != pugi::status_ok;
	isLoaded &= bool(xml.child("drawing"));
	isLoaded &= bool(xml.select_node("//drawing/background[red and green and blue]"));

	// Last check could have also been done like:
	// isLoaded &= bool(xml.select_node("//drawing/background/red"));
	// isLoaded &= bool(xml.select_node("//drawing/background/green"));
	// isLoaded &= bool(xml.select_node("//drawing/background/blue"));

	if(!isLoaded){
		ofLogNotice() << "File not found or incorrect format, creating";
		auto bg = xml.append_child("drawing")
				  .append_child("background");
		bg.append_child("red")
				.append_child(pugi::node_pcdata)
				.set_value("255");
		bg.append_child("green")
				.append_child(pugi::node_pcdata)
				.set_value("255");
		bg.append_child("blue")
				.append_child(pugi::node_pcdata)
				.set_value("255");
	}

	// select background and read it's values
	auto background = xml.select_node("//background").node();
	if(background){
		bgColor.r = background.child("red").text().as_int();
		bgColor.g = background.child("green").text().as_int();
		bgColor.b = background.child("blue").text().as_int();
	}

	// select all strokes and iterate through them
	// for each stroke, create a new mesh
	auto strokesXml = xml.select_nodes("//strokes");
	for(auto & stroke: strokesXml){
		strokes.emplace_back();
		strokes.back().setMode(OF_PRIMITIVE_LINE_STRIP);

		// for each pt in the stroke insert a new
		// vertex in the mesh
		auto pts = stroke.node().children("pt");
		for(auto & pt: pts){
			auto x = pt.attribute("x").as_int();
			auto y = pt.attribute("y").as_int();
			strokes.back().addVertex({x,y,0});
		}
	}

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	for(auto & stroke: strokes){
		stroke.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 's'){
		xml.save_file(ofToDataPath("points.xml").c_str());
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	auto pt = currentStroke.append_child("pt");
	pt.append_attribute("x").set_value(to_string(x).c_str());
	pt.append_attribute("y").set_value(to_string(y).c_str());
	strokes.back().addVertex({x,y,0});
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	currentStroke = xml.child("drawing").append_child("stroke");
	strokes.emplace_back();
	strokes.back().setMode(OF_PRIMITIVE_LINE_STRIP);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
