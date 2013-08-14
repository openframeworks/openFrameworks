#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
#ifdef TARGET_OPENGLES
    shaderBlurX.load("shadersES2/shaderBlurX");
    shaderBlurY.load("shadersES2/shaderBlurY");
#else
	if(ofIsGLProgrammableRenderer()){
		shaderBlurX.load("shadersGL3/shaderBlurX");
        shaderBlurY.load("shadersGL3/shaderBlurY");
	}else{
		shaderBlurX.load("shadersGL2/shaderBlurX");
        shaderBlurY.load("shadersGL2/shaderBlurY");
	}
#endif

    image.loadImage("img.jpg");
    
    fboBlurOnePass.allocate(image.getWidth(), image.getHeight());
    fboBlurTwoPass.allocate(image.getWidth(), image.getHeight());
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    
    float blur = ofMap(mouseX, 0, ofGetWidth(), 0, 10, true);
    
    //----------------------------------------------------------
    fboBlurOnePass.begin();
    
    shaderBlurX.begin();
    shaderBlurX.setUniform1f("blurAmnt", blur);

    image.draw(0, 0);
    
    shaderBlurX.end();
    
    fboBlurOnePass.end();
    
    //----------------------------------------------------------
    fboBlurTwoPass.begin();
    
    shaderBlurY.begin();
    shaderBlurY.setUniform1f("blurAmnt", blur);
    
    fboBlurOnePass.draw(0, 0);
    
    shaderBlurY.end();
    
    fboBlurTwoPass.end();
    
    //----------------------------------------------------------
    ofSetColor(ofColor::white);
    fboBlurTwoPass.draw(0, 0);
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