#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetFrameRate(120);

	active_name.set("element name", "");
	active_index.set("element index", -1);


	/*
	 *  panel without header and a button that toggles the visibility of all the other headers
	 */
	panel1 = gui.addPanel();
	panel1->setPosition(20,20);
	panel1->setShowHeader(false);
	panel1->setBackgroundColor(ofColor(0,0,0,0));

	/*
	 * FPS plotter
	 */
	panel1->addFpsPlotter();
	panel1->addSpacer(panel1->getWidth(), 20);

	/*
	 * toggles
	 */
	toggles = panel1->addGroup("toggles");
	toggles->setShowHeader(false);

	toggle_param.set("show/hide header", true);
	toggles->add(toggle_param, ofJson({{"type", "fullsize"}, {"text-align", "center"}}));

	group1 = toggles->addGroup();
	group1->setShowHeader(false);
	ofJson toggle_config = {{"width", "50%"}, {"float", "left"}};
	group1->add(toggle1_param.set("multiple", false), toggle_config);
	group1->add(toggle2_param.set("choice", false), toggle_config);

	/*
	 * matrix with only one allowed active toggle, listener to use labels above to show current index
	 */
	matrix_params.push_back(ofParameter<bool>("only", false));
	matrix_params.push_back(ofParameter<bool>("one", false));
	matrix_params.push_back(ofParameter<bool>("toggle", false));
	matrix_params.push_back(ofParameter<bool>("can", false));
	matrix_params.push_back(ofParameter<bool>("be", false));
	matrix_params.push_back(ofParameter<bool>("active", false));

	group3 = toggles->addGroup();
	group3->getActiveToggleIndex().addListener(this, &ofApp::updateMatrixIndex);
	group3->setShowHeader(false);
	group3->setExclusiveToggles(true);
	for(unsigned int i = 0; i < matrix_params.size(); i++){
		group3->add(matrix_params.at(i), ofJson({{"float", "left"},	{"width", "50%"}, {"type", "fullsize"}}));
	}

	panel1->addSpacer(panel1->getWidth(), 20);

	/*
	 *  labels
	 */
	labels = panel1->addGroup("labels");
	labels->add<ofxLabel>("blabla");
	labels->add(active_name);
	labels->add<ofxIntLabel>(active_index);

	panel1->addSpacer(panel1->getWidth(), 20);

	/*
	 * buttons
	 */
	buttons = panel1->addGroup("buttons");
	buttons->add<ofxButton>("fullsize button", ofJson({{"type", "fullsize"}, {"text-align", "center"}}));
	buttons->add<ofxButton>("checkbox button", ofJson({{"type", "checkbox"}}));
	buttons->add<ofxButton>("radio button", ofJson({{"type", "radio"}}));

	panel1->addSpacer(panel1->getWidth(), 20);

	/*
	 *  input fields
	 */
	panel1->add<ofxFloatInputField>(floatfield_param.set("float input",3.5,0,500));
	panel1->add<ofxTextField>(textfield_param.set("text input","type in here"));


	/*
	 *  sliders
	 */
	sliders = gui.addPanel("vertical sliders");
	sliders->setPosition(260,20);
	sliders->setWidth(260);

	sliders->add(slider1_param.set("slider1", 1. / 7., 0, 1), ofJson({{"float", "left"}, {"width", 40}, {"height", 130}}));
	sliders->add(slider2_param.set("slider2", 5. / 7., 0, 1), ofJson({{"float", "left"}, {"width", 50}, {"height", 130}}));
	sliders->add(slider3_param.set("slider3", 4. / 7., 0, 1), ofJson({{"float", "left"}, {"width", 60}, {"height", 130}}));
	sliders->add(slider4_param.set("slider4", 6. / 7., 0, 1), ofJson({{"float", "left"}, {"width", 70}, {"height", 130}}));

	sliders->addSpacer(sliders->getWidth(), 20);

	ofJson slider_config2 = {{"precision", 1}};
	sliders->add(slider1_param, slider_config2);
	sliders->add(slider2_param, slider_config2);
	sliders->add(slider3_param, slider_config2);
	sliders->add(slider4_param, slider_config2);

	sliders->addSpacer(sliders->getWidth(), 20);

	sliders->add(slider_param.set("slider", 0.5, 0, 1), ofJson({{"type", "circular"}, {"height", 60}}));


	/*
	 * ofParameterGroup example with radio toggles
	 */

	colorParameters.setName("colors");
	colorParameters.add(color0.set("rosyBrown",false));
	colorParameters.add(color1.set("mediumVioletRed",false));
	colorParameters.add(color2.set("mediumAquaMarine",false));
	colorParameters.add(color3.set("steelBlue",false));

	panel3 = gui.addPanel("ofParameterGroup");
	panel3->setPosition(260, 460);
	color_toggles = panel3->addGroup(colorParameters);
	color_toggles->setExclusiveToggles(true);
	color_toggles->setConfig(ofJson({{"type", "radio"}}));

	/*
	 * adding listeners
	 */

	toggle_param.addListener(this, &ofApp::toggleGroupHeader);
	color_toggles->getActiveToggleIndex().addListener(this, &ofApp::setHeaderColors);
	color_toggles->setActiveToggle(3);

}

//--------------------------------------------------------------
void ofApp::exit(){
	toggle_param.removeListener(this, &ofApp::toggleGroupHeader);
	group3->getActiveToggleIndex().removeListener(this, &ofApp::updateMatrixIndex);
	color_toggles->getActiveToggleIndex().removeListener(this, &ofApp::setHeaderColors);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackgroundGradient(ofColor::white, ofColor::gray);

}

void ofApp::setHeaderColors(int& index){

	ofColor c;
	switch(index){
		case 0: c = ofColor::rosyBrown; break;
		case 1: c = ofColor::mediumVioletRed; break;
		case 2: c = ofColor::mediumAquaMarine; break;
		default: case 3: c = ofColor::steelBlue; break;

	}

	labels->setHeaderBackgroundColor(c);
	toggles->setHeaderBackgroundColor(c);
	buttons->setHeaderBackgroundColor(c);
	group1->setHeaderBackgroundColor(c);
	sliders->setHeaderBackgroundColor(c);
	group3->setHeaderBackgroundColor(c);
	panel3->setHeaderBackgroundColor(c);
	color_toggles->setHeaderBackgroundColor(c);

	color = c;
}

void ofApp::toggleGroupHeader(bool & val){
	labels->setShowHeader(val);
	sliders->setShowHeader(val);
	buttons->setShowHeader(val);
	panel3->setShowHeader(val);
	color_toggles->setShowHeader(val);
}

void ofApp::updateMatrixIndex(int &index){
	active_index = index;
	active_name = matrix_params.at(index).getName();
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
