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

    backgroundImage.loadImage("A.jpg");
    foregroundImage.loadImage("B.jpg");
    brushImage.loadImage("brush.png");
    
    int width = backgroundImage.getWidth();
    int height = backgroundImage.getHeight();
    
    maskFbo.allocate(width, height);
    fbo.allocate(width, height);
    
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

    ofSetColor(255);

    //----------------------------------------------------------
    // this is our alpha mask which we draw into.
    if(bBrushDown) {
        maskFbo.begin();
        
        int brushImageSize = 50;
        int brushImageX = mouseX - brushImageSize * 0.5;
        int brushImageY = mouseY - brushImageSize * 0.5;
        brushImage.draw(brushImageX, brushImageY, brushImageSize, brushImageSize);
        
        maskFbo.end();
    }
    
    //----------------------------------------------------------
    // HERE the shader-masking happends
    fbo.begin();
    // Cleaning everthing with alpha mask on 0 in order to make it transparent by default
    ofClear(0, 0, 0, 0);
    
    shader.begin();
    // here is where the fbo is passed to the shader
    shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
    
    backgroundImage.draw(0, 0);
    
    shader.end();
    fbo.end();
    
    //----------------------------------------------------------
    // FIRST draw the background image
    foregroundImage.draw(0,0);
    
    // THEN draw the masked fbo on top
    fbo.draw(0,0);
    
    //----------------------------------------------------------
    ofDrawBitmapString("Drag the Mouse to draw", 15,15);
    ofDrawBitmapString("Press spacebar to clear", 15, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == ' ') {
        maskFbo.begin();
        ofClear(0,0,0,255);
        maskFbo.end();
    }
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