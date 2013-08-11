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

    drawWires = false;
    
    img.loadImage("img.jpg");
    plane.set(800, 600, 10, 10);
    plane.mapTexCoordsFromTexture(img.getTextureReference());
}


//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofRotate(rotation, 1, 0, 0);
    
    // bind our texture. in our shader this will now be tex0 by default
    // so we can just go ahead and access it there.
    img.getTextureReference().bind();
    
    // start our shader, in our OpenGL3 shader this will automagically set
    // up a lot of matrices that we want for figuring out the texture matrix
    // and the modelView matrix
    shader.begin();
    
    // get mouse position relative to center of screen
    float mousePosition = float(mouseX) - float(ofGetWidth()/2);

    // and pass it to the shader (inverted)
    shader.setUniform1f("mouseX", mousePosition * -1.);
    
    if(drawWires) {
        plane.drawWireframe();
    } else {
        plane.draw();
    }
    
    shader.end();

    img.getTextureReference().unbind();
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    drawWires = !drawWires;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    rotation = 45 + (-60 * ( (float) y / ofGetHeight()));
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
