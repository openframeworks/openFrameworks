#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
#ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
#else
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}else{
		shader.load("shadersGL2/shader");
	}
#endif

    srcImg.loadImage("A.jpg");
    dstImg.loadImage("B.jpg");
    brushImg.loadImage("brush.png");
    
    int width = srcImg.getWidth();
    int height = srcImg.getHeight();
    
    maskFbo.allocate(width,height);
    fbo.allocate(width,height);
    
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
    
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
    bBrushDown = false;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255);

    // MASK (frame buffer object)
    //
    maskFbo.begin();
    if (bBrushDown){
        brushImg.draw(mouseX-25,mouseY-25,50,50);
    }
    maskFbo.end();
    
    // HERE the shader-masking happends
    //
    fbo.begin();
    // Cleaning everthing with alpha mask on 0 in order to make it transparent by default
    ofClear(0, 0, 0, 0);
    
    shader.begin();
    // here is where the fbo is passed to the shader
    shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
    
    srcImg.draw(0,0);
    
    shader.end();
    fbo.end();
    
    
    // FIRST draw the background image
    dstImg.draw(0,0);
    
    // THEN draw the masked fbo on top
    fbo.draw(0,0);
    
    ofDrawBitmapString("Drag the Mouse to draw", 15,15);
    ofDrawBitmapString("Press spacebar to clear", 15, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
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
    bBrushDown = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    bBrushDown = false;
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