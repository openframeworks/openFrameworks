#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetFrameRate(120);

	gui_doc =  std::make_unique<ofx::DOM::Document>();

	panel1 = gui_doc->add<ofxPanel>("extended gui");


	/*
	 *  label without showing the label name
	 */
	panel1->add<ofxLabel>(label_param.set("LabelName", "label without labelname"), ofJson({{"show-name", false}}));

	/*
	 * FPS plotter
	 */
	panel1->add<ofxFpsPlotter>();

	/*
	 * minimal button and toggle
	 */
	panel1->add<ofxToggle>(toggle_param.set("show header", true), ofJson({{"type", "fullsize"}}));
	toggle_param.addListener(this, &ofApp::toggleGroupHeader);
	button = panel1->add<ofxButton>("simple button", ofJson({{"type", "fullsize"}}));

	/*
	 * rotary slider
	 */
	rotary = panel1->add<ofxGuiGroup>("rotary");
	rotary->add<ofxFloatRotarySlider>(slider_param.set("slider", 0.5, 0, 1), ofJson({{"height", 60}}));

	/*
	 * labels can not only show string values but any ofParameter with a toString()-function
	 */
	panel1->add(matrix_active_name.set("element name", ""));
	panel1->add<ofxIntLabel>(matrix_active_index.set("element index", -1));

	/*
	 * matrix with only one allowed active toggle, listener to use labels above to show current index
	 */
	matrix_params.push_back(ofParameter <bool>("only", false));
	matrix_params.push_back(ofParameter <bool>("one", false));
	matrix_params.push_back(ofParameter <bool>("toggle", false));
	matrix_params.push_back(ofParameter <bool>("can", false));
	matrix_params.push_back(ofParameter <bool>("be", false));
	matrix_params.push_back(ofParameter <bool>("active", false));

	matrix = panel1->add<ofxGuiGroup>("matrix");
	matrix->setExclusiveToggles(true);
	matrix->getActiveToggleIndex().addListener(this, &ofApp::updateMatrixIndex);
	for(unsigned int i = 0; i < matrix_params.size(); i++){
		matrix->add<ofxToggle>(matrix_params.at(i), ofJson({
															{"float", "left"},
															{"width", (matrix->getWidth()-1)/3},
															{"type", "fullsize"}}));
	}

	/*
	 *  input fields
	 */
	panel1->add<ofxFloatInputField>(floatfield_param.set("float input",3.5,0,500));
	panel1->add<ofxTextField>(textfield_param.set("text input","type in here"));

	/*
	 * horizontal panel
	 */
	panel2 = gui_doc->add<ofxPanel>("horizontal", "", 260, 10);
	panel2->setWidth(400);
	panel2->setBackgroundColor(ofColor::black);

	ofJson toggle_config = {{"width", 90}, {"float", "left"}};
	panel2->add <ofxToggle>(toggle1_param.set("toggle1", false), toggle_config);
	panel2->add <ofxToggle>(toggle2_param.set("toggle2", false), toggle_config);
	panel2->add <ofxToggle>(toggle3_param.set("toggle3", false), toggle_config);
	panel2->add <ofxToggle>(toggle4_param.set("toggle4", false), toggle_config);

	/*
	 * horizontal panel with vertical sliders and spacer
	 */
	panel3 = gui_doc->add<ofxPanel>("vertical sliders, spacer", "", 260, 80);
	panel3->setSize(260, panel3->getHeight());

	panel3->add <ofxFloatSlider>(slider1_param.set("slider1", 1. / 7., 0, 1), ofJson({{"float", "left"}, {"width", 40}, {"height", 130}}));
	panel3->add<ofxGuiSpacer>(ofJson({{"width", 10}, {"height", 100}, {"float", "left"}}));
	panel3->add <ofxFloatSlider>(slider2_param.set("slider2", 5. / 7., 0, 1), ofJson({{"float", "left"}, {"width", 50}, {"height", 130}}));
	panel3->add <ofxFloatSlider>(slider3_param.set("slider3", 4. / 7., 0, 1), ofJson({{"float", "left"}, {"width", 60}, {"height", 130}}));
	panel3->add <ofxFloatSlider>(slider4_param.set("slider4", 6. / 7., 0, 1), ofJson({{"float", "left"}, {"width", 70}, {"height", 130}}));

	/*
	 * set precision of sliders
	 */
	panel4 = gui_doc->add<ofxPanel>("slider precision","", 260, 240);
	ofJson slider_config2 = {{"precision", 1}};
	panel4->add <ofxFloatSlider>(slider1_param, slider_config2);
	panel4->add <ofxFloatSlider>(slider2_param, slider_config2);
	panel4->add <ofxFloatSlider>(slider3_param, slider_config2);
	panel4->add <ofxFloatSlider>(slider4_param, slider_config2);


	cameraMatrixParameters.setName("cameras");
	cameraMatrixParameters.add(cam0.set("cam0",false));
	cameraMatrixParameters.add(cam1.set("cam1",false));
	cameraMatrixParameters.add(cam2.set("cam2",false));
	cameraMatrixParameters.add(cam3.set("cam3",false));

	panel5 = gui_doc->add<ofxPanel>("ofParameterGroup", "", 260, 440);
	matrixCam = panel5->add<ofxGuiGroup>(cameraMatrixParameters);
	matrixCam->setExclusiveToggles(true);
	matrixCam->setConfig(ofJson({{"type", "radio"}}));

}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
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

}

void ofApp::toggleGroupHeader(bool & val){
	rotary->setShowHeader(val);
	matrix->setShowHeader(val);
	panel1->setShowHeader(val);
	panel2->setShowHeader(val);
	panel3->setShowHeader(val);
	panel4->setShowHeader(val);
	panel5->setShowHeader(val);
}

void ofApp::updateMatrixIndex(int &index){
	matrix_active_index = index;
	matrix_active_name = matrix_params.at(index).getName();
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
