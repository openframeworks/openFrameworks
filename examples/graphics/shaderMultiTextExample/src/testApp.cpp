#include "testApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    int camWidth 		= 320;	// try to grab at this size. 
	int camHeight 		= 240;
    
    vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth,camHeight);
    
    fingerMovie.loadMovie("fingers.mov");
	fingerMovie.play();
    
    logoImg.loadImage("logo.jpg");
    multimaskImg.loadImage("mask.jpg");
    
    fbo.allocate(camWidth,camHeight);
    maskFbo.allocate(camWidth,camHeight);
    
    ofSetWindowShape(camWidth, camHeight*2);
    
    // There are 3 of ways of loading a shader:
    //
    //  1 - Using just the name of the shader and ledding ofShader look for .frag and .vert: 
    //      Ex.: shader.load( "myShader");
    //
    //  2 - Giving the right file names for each one: 
    //      Ex.: shader.load( "myShader.vert","myShader.frag");
    //
    //  3 - And the third one it«s passing the shader programa on a single string;
    //      In this particular explample we are usin STRINGIFY witch it«s a handy macro
    string shaderProgram = STRINGIFY(
                                     uniform sampler2DRect tex0;
                                     uniform sampler2DRect tex1;
                                     uniform sampler2DRect tex2;
                                     uniform sampler2DRect maskTex;

                                     void main (void){
                                         vec2 pos = gl_TexCoord[0].st;
                                         
                                         vec4 rTxt = texture2DRect(tex0, pos);
                                         vec4 gTxt = texture2DRect(tex1, pos);
                                         vec4 bTxt = texture2DRect(tex2, pos);
                                         vec4 mask = texture2DRect(maskTex, pos);
                                         
                                         vec4 color = rTxt;
                                         color = mix(color, gTxt, mask.g );
                                         color = mix(color, bTxt, mask.b );
                                         
                                         gl_FragColor = color;
                                     }
                                     );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram(); 
    
    // Let«s clear the FBO«s
    // otherwise it will bring some junk with it from the memory    
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
}

//--------------------------------------------------------------
void testApp::update(){
    vidGrabber.grabFrame();
    fingerMovie.idleMovie();
        
    maskFbo.begin();
    ofClear(255, 0, 0,255);
    multimaskImg.draw( mouseX-multimaskImg.getWidth()*0.5, 0 );
    maskFbo.end();
    
    // MULTITEXTURE MIXING FBO
    //
    fbo.begin();
    ofClear(0, 0, 0,255);
    shader.begin();
    shader.setUniformTexture("tex0", logoImg, 1 );
    shader.setUniformTexture("tex1", vidGrabber.getTextureReference() , 2 );
    shader.setUniformTexture("tex2", fingerMovie.getTextureReference() , 3 );
    shader.setUniformTexture("maskTex", maskFbo.getTextureReference() , 4 );
    logoImg.draw(0,0);
    
    shader.end();
    fbo.end();

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255);
    
    maskFbo.draw(0,0);
    ofDrawBitmapString("multiTexture mask", 15, 15);
    
    fbo.draw(0,240);
    ofDrawBitmapString("Final mix", 15,255);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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