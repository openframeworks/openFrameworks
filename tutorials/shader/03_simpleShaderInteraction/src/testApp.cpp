#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
#ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
#else
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}else{
		shader.load("shadersGL2/shader");
	}
#endif

    int planeWidth = ofGetWidth();
    int planeHeight = ofGetHeight();
    int planeGridSize = 20;
    int planeColums = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
    
    plane.set(planeWidth, planeHeight, planeColums, planeRows, OF_PRIMITIVE_TRIANGLES);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    
    shader.begin();
    
    // center screen.
    float cx = ofGetWidth() / 2.0;
    float cy = ofGetHeight() / 2.0;
    
    // the plane is being position in the middle of the screen,
    // so we have to apply the same offset to the mouse coordinates before passing into the shader.
    float mx = mouseX - cx;
    float my = mouseY - cy;
    
    // we can pass in a single value into the shader by using the setUniform1 function.
    // if you want to pass in a float value, use setUniform1f.
    // if you want to pass in a integer value, use setUniform1i.
    shader.setUniform1f("mouseRange", 150);
    
    // we can pass in two values into the shader at the same time by using the setUniform2 function.
    // inside the shader these two values are set inside a vec2 object.
    shader.setUniform2f("mousePos", mx, my);
    
    // color changes from magenta to blue when moving the mouse from left to right.
    float percentX = mouseX / (float)ofGetWidth();
    percentX = ofClamp(percentX, 0, 1);
    ofFloatColor colorLeft = ofColor::magenta;
    ofFloatColor colorRight = ofColor::blue;
    ofFloatColor colorMix = colorLeft.getLerped(colorRight, percentX);
    
    // create a float array with the color values.
    float mouseColor[4] = {colorMix.r, colorMix.g, colorMix.b, colorMix.a};
    
    // we can pass in four values into the shader at the same time as a float array.
    // we do this by passing a pointer reference to the first element in the array.
    // inside the shader these four values are set inside a vec4 object.
    shader.setUniform4fv("mouseColor", &mouseColor[0]);
    
    ofTranslate(cx, cy);

    plane.drawWireframe();
    
    shader.end();
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