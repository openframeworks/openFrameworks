#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);

    ofSetFrameRate(120);

	panel1.setup("extended gui");


    /*
     *  label without showing the label name
     */
	ofxBaseGui::Config labelconfig;
    labelconfig.showName = false;
    panel1.add<ofxLabel>(label_param.set("LabelName", "label without labelname"), labelconfig);

    /*
     * FPS plotter
     */
    panel1.add<ofxFpsPlotter>();

	/*
	 * minimal button and toggle
	 */
    panel1.add<ofxMinimalToggle>(toggle_param.set("show header", true));
	toggle_param.addListener(this, &ofApp::toggleGroupHeader);
	panel1.add(button.setup("simple button"));

	/*
	 * rotary slider
	 */
    rotary.setup("rotary");
    rotary.add<ofxFloatRotarySlider>(slider_param.set("slider", 0.5, 0, 1));
    panel1.add(rotary);

    /*
     * labels can not only show string values but any ofParameter with a toString()-function
     */
    panel1.add(matrix_active_name.set("element name", ""));
    panel1.add<ofxIntLabel>(matrix_active_index.set("element index", -1));

	/*
     * matrix with only one allowed active toggle, listener to use labels above to show current index
	 */
	matrix_params.push_back(ofParameter <bool>("only", false));
	matrix_params.push_back(ofParameter <bool>("one", false));
	matrix_params.push_back(ofParameter <bool>("toggle", false));
	matrix_params.push_back(ofParameter <bool>("can", false));
	matrix_params.push_back(ofParameter <bool>("be", false));
	matrix_params.push_back(ofParameter <bool>("active", false));

    matrix.setup("matrix", 3);
    matrix.setExclusiveToggles(true);
	matrix.getActiveToggleIndex().addListener(this, &ofApp::updateMatrixIndex);
	for(unsigned int i = 0; i < matrix_params.size(); i++){
        matrix.add<ofxMinimalToggle>(matrix_params.at(i));
    }
    panel1.add(matrix);

    /*
     *  input fields
     */
    panel1.add<ofxFloatInputField>(floatfield_param.set("float input",3.5,0,500));
    panel1.add<ofxTextField>(textfield_param.set("text input","type in here"));

	/*
     * horizontal panel
	 */
	panel2.setup("horizontal", "", 260, 10);
    panel2.setLayout(ofxBaseGui::Horizontal);
	panel2.setBorderColor(ofColor::black);

	ofxToggle::Config toggle_config;
	toggle_config.shape.width = 90;
	panel2.add <ofxToggle>(toggle1_param.set("toggle1", false), toggle_config);
	panel2.add <ofxMinimalToggle>(toggle2_param.set("toggle2", false), toggle_config);
    panel2.add <ofxMinimalToggle>(toggle3_param.set("toggle3", false), toggle_config);
	panel2.add <ofxMinimalToggle>(toggle4_param.set("toggle4", false), toggle_config);

    /*
     * horizontal panel with vertical sliders and spacer
     */
    panel3.setup("vertical sliders, spacer", "", 260, 80);
    panel3.setLayout(ofxBaseGui::Horizontal);

    ofxBaseGui::Config slider_config;
    slider_config.layout = ofxBaseGui::Vertical;

    panel3.add <ofxFloatSlider>(slider1_param.set("slider1", 1. / 7., 0, 1), slider_config);
    panel3.addSpacer(42);
    panel3.add <ofxFloatSlider>(slider2_param.set("slider2", 5. / 7., 0, 1), slider_config);
    panel3.add <ofxFloatSlider>(slider3_param.set("slider3", 4. / 7., 0, 1), slider_config);
    panel3.add <ofxFloatSlider>(slider4_param.set("slider4", 6. / 7., 0, 1), slider_config);

    panel3.getControl("slider1")->setSize(40, 130);
    panel3.getControl("slider2")->setSize(50, 130);
    panel3.getControl("slider3")->setSize(60, 130);
    panel3.getControl("slider4")->setSize(70, 130);

    /*
     * set precision of sliders
     */
    panel4.setup("slider precision","", 260, 240);
    ofxFloatSlider::Config slider_config2;
    slider_config2.precision = 1;
    panel4.add <ofxFloatSlider>(slider1_param, slider_config2);
    panel4.add <ofxFloatSlider>(slider2_param, slider_config2);
    panel4.add <ofxFloatSlider>(slider3_param, slider_config2);
    panel4.add <ofxFloatSlider>(slider4_param, slider_config2);


    cameraMatrixParameters.setName("cameras");
    cameraMatrixParameters.add(cam0.set("cam0",false));
    cameraMatrixParameters.add(cam1.set("cam1",false));
    cameraMatrixParameters.add(cam2.set("cam2",false));
    cameraMatrixParameters.add(cam3.set("cam3",false));

    panel5.setup("ofParameterGroup", "", 260, 440);
    matrixCam.setup(cameraMatrixParameters, 4);
    matrixCam.setExclusiveToggles(true);
    panel5.add(matrixCam);

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

void ofApp::updateMatrixIndex(int &index){
    matrix_active_index = index;
    matrix_active_name = matrix.getControl(index)->getName();
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
