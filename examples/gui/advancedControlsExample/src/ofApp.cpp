#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetFrameRate(120);

	panel1.setup("extended gui");

	ofxBaseGui::Config labelconfig;
    labelconfig.showName = false;
	panel1.add <ofxLabel>(label_param.set("LabelName", "extended Label"), labelconfig);

	panel1.add(fps.setup());

	/*
	 * minimal button and toggle
	 */
	panel1.add <ofxMinimalToggle>(toggle_param.set("show header", true));
	toggle_param.addListener(this, &ofApp::toggleGroupHeader);
	panel1.add(button.setup("simple button"));

	/*
	 * rotary slider
	 */
	rotary.setup("rotary");
	rotary.setBorderColor(ofColor::blanchedAlmond - ofColor(50));
	rotary.add <ofxFloatRotarySlider>(slider_param.set("slider", 0.5, 0, 1));
	rotary.getControl("slider")->setFillColor(ofColor::white);
	rotary.getControl("slider")->setBackgroundColor(ofColor::blanchedAlmond - ofColor(130));

	/*
	 * matrix with only one allowed active toggle
	 */
	matrix_params.push_back(ofParameter <bool>("only", false));
	matrix_params.push_back(ofParameter <bool>("one", false));
	matrix_params.push_back(ofParameter <bool>("toggle", false));
	matrix_params.push_back(ofParameter <bool>("can", false));
	matrix_params.push_back(ofParameter <bool>("be", false));
	matrix_params.push_back(ofParameter <bool>("active", false));
	matrix.setup("matrix", 3);
	for(unsigned int i = 0; i < matrix_params.size(); i++){
		matrix.ofxGuiGroup::add <ofxMinimalToggle>(matrix_params.at(i));
	}
	matrix.setBorderColor(ofColor::aquamarine);
	matrix.setElementHeight(26);
	matrix.setExclusiveToggles(true);

	panel1.add(rotary);
	panel1.add(matrix);

	/*
	 * horizontal panel with spacer
	 */
	panel2.setup("horizontal", "", 260, 10);
	panel2.setAlignHorizontal();
	panel2.setBorderColor(ofColor::black);

	ofxToggle::Config toggle_config;
	toggle_config.shape.width = 90;
	panel2.add <ofxToggle>(toggle1_param.set("toggle1", false), toggle_config);
	panel2.add <ofxMinimalToggle>(toggle2_param.set("toggle2", false), toggle_config);

	ofxGuiSpacer::Config spacer_config;
	spacer_config.shape.width = 42;
	panel2.add <ofxGuiSpacer>(spacer_config);

	panel2.add <ofxMinimalToggle>(toggle3_param.set("toggle3", false), toggle_config);
	panel2.add <ofxMinimalToggle>(toggle4_param.set("toggle4", false), toggle_config);

	/*
	 * panel with canvas
	 */
	panel3.setup("canvas", "", 260, 90);
	img.load("images/ente.jpg");
	canvas.setup("some texture", &img.getTexture());
	panel3.add(canvas);

	panel4.setup("zoomable canvas", "", 500, 90);
	zcanvas.setup("same texture", &img.getTexture());
	panel4.add(zcanvas);

	panel5.setup("vertical sliders", "", 260, 280);
	panel5.setAlignHorizontal();
	panel5.add <ofxFloatVerticalSlider>(slider1_param.set("slider1", 1. / 7., 0, 1));
	panel5.add <ofxFloatVerticalSlider>(slider2_param.set("slider2", 5. / 7., 0, 1));
	panel5.add <ofxFloatVerticalSlider>(slider3_param.set("slider3", 4. / 7., 0, 1));
	panel5.add <ofxFloatVerticalSlider>(slider4_param.set("slider4", 6. / 7., 0, 1));
	panel5.getControl("slider1")->setSize(40, 130);
	panel5.getControl("slider2")->setSize(50, 130);
	panel5.getControl("slider3")->setSize(60, 130);
	panel5.getControl("slider4")->setSize(70, 130);

}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
	fps.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	if(toggle1_param.get()){
		ofSetColor(ofColor::royalBlue);
	}else{
		if(toggle2_param.get()){
			ofSetColor(ofColor::azure);
		}else{
			if(matrix_params.at(0).get()){
				ofSetColor(ofColor::burlyWood);
			}else{
				ofSetColor(ofColor::fromHex(0x2da1e3));
			}
		}
	}

	ofDrawRectangle(ofGetWindowRect());

	ofSetColor(255);

	panel1.draw();
	panel2.draw();
	panel3.draw();
	panel4.draw();
	panel5.draw();

}

void ofApp::toggleGroupHeader(bool & val){
	rotary.setShowHeader(val);
	matrix.setShowHeader(val);
	panel1.setShowHeader(val);
	panel2.setShowHeader(val);
	panel3.setShowHeader(val);
	panel4.setShowHeader(val);
	panel5.setShowHeader(val);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key){
	 case 'f': {
		 ofToggleFullscreen();
		 break;
	 }

	 default:
		 break;
	}

}

//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs & args){
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs & args){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs & args){
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
