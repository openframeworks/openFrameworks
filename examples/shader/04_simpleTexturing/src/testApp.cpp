#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
#ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
#else
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}else{
		shader.load("shadersGL2/shader");
	}
#endif

    img.loadImage("img.jpg");

    plane.set(800, 600, 10, 10);
    plane.mapTexCoords(0, 0, img.getWidth(), img.getHeight());
    ofVec4f tcoords = plane.getTexCoords();
    plane.mapTexCoords(tcoords.x, tcoords.y, tcoords.z, tcoords.w);
}


//--------------------------------------------------------------
void testApp::update() {
    
}

//--------------------------------------------------------------
void testApp::draw() {
    
    // bind our texture. in our shader this will now be tex0 by default
    // so we can just go ahead and access it there.
    img.getTextureReference().bind();
    
    // start our shader, in our OpenGL3 shader this will automagically set
    // up a lot of matrices that we want for figuring out the texture matrix
    // and the modelView matrix
    shader.begin();
    
    // get mouse position relative to center of screen
    float mousePosition = ofMap(mouseX, 0, ofGetWidth(), 1.0, -1.0, true);
#ifndef TARGET_OPENGLES
    // when texture coordinates are normalised, they are always between 0 and 1.
    // in GL2 and GL3 the texture coordinates are not normalised,
    // so we have to multiply the normalised mouse position by the plane width.
    mousePosition *= plane.getWidth();
#endif

    shader.setUniform1f("mouseX", mousePosition);

    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    plane.draw();

    ofPopMatrix();
    
    shader.end();

    img.getTextureReference().unbind();
    
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
