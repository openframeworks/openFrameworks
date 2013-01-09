#include "testApp.h"

ofShader shader;
ofImage img;
//--------------------------------------------------------------
void testApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	img.loadImage("testImage.jpg");

	//shader.load("", "shaders/test.frag");

    string test = "#version 120 \n\
#extension GL_ARB_texture_rectangle : enable \n\
#pragma include \"shaders/ofBlendingUtils.frag\" \n\
#pragma include \"shaders/ofImageProcessingUtils.frag\" \n\
uniform sampler2DRect tex0; \
uniform float blurScale; \
uniform float saturation; \
void main(){ \
	vec2 st	= gl_TexCoord[0].st; \
	vec4 ogColor = texture2DRect(tex0, st); \
	vec4 blurColor	=  blurH(tex0, st, blurScale); \
	blurColor		+= blurV(tex0, st, blurScale); \
	blurColor *= 0.5; \
	blurColor		+= blurV(tex0, st, blurScale*2.2) * 0.7; \
	blurColor		+= blurH(tex0, st, blurScale*2.2) * 0.7; \
	blurColor.rgb = ContrastSaturationBrightness(blurColor.rgb, 1.0, saturation, 1.5); \
	gl_FragColor = blurColor; \
}";

	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, test);
    shader.linkProgram();
	
	ofBackground(40, 40, 40);
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255);
	shader.begin();
		shader.setUniform1i("tex0", 0);
		shader.setUniform1f("blurScale", ofGetMouseX() / 600.0 );  
		shader.setUniform1f("saturation", ofMap(ofGetMouseY(), 0, ofGetHeight(), 0.3, 2.0, true));  
		img.draw(0,0);
	shader.end(); 
	
	ofDrawBitmapString("mouse x - controls blur amount\nmouse y - controls saturation", 10, img.getHeight()+20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
