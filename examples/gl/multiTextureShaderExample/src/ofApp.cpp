#include "ofApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    int camWidth 		= 320;	// try to grab at this size. 
	int camHeight 		= 240;
    
    vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth,camHeight);
    
    fingerMovie.loadMovie("fingers.mov");
	fingerMovie.play();
    
    logoImg.loadImage("colors.jpg");
    multimaskImg.loadImage("mask.jpg");
    
    fbo.allocate(camWidth,camHeight);
    maskFbo.allocate(camWidth,camHeight);
    
    //ofSetWindowShape(camWidth, camHeight*2);
    
    // There are 3 of ways of loading a shader:
    //
    //  1 - Using just the name of the shader and ledding ofShader look for .frag and .vert: 
    //      Ex.: shader.load( "myShader");
    //
    //  2 - Giving the right file names for each one: 
    //      Ex.: shader.load( "myShader.vert","myShader.frag");
    //
    //  3 - And the third one is passing the shader programa on a single string;
    //      In this particular example we are using STRINGIFY which is a handy macro
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
                                         
                                         vec4 color = vec4(0,0,0,0);
                                         color = mix(color, rTxt, mask.r );
                                         color = mix(color, gTxt, mask.g );
                                         color = mix(color, bTxt, mask.b );
                                         
                                         gl_FragColor = color;
                                     }
                                     );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram(); 
    
    // Let's clear the FBOs
    // otherwise it will bring some junk with it from the memory    
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    fingerMovie.update();
        
    // This just 
    maskFbo.begin();
    ofClear(255, 0, 0,255);
    multimaskImg.draw( mouseX-multimaskImg.getWidth()*0.5, 0 );
    maskFbo.end();
    
    // MULTITEXTURE MIXING FBO
    //
    fbo.begin();
    ofClear(0, 0, 0,255);
    shader.begin();
    // Pass the video texture
    shader.setUniformTexture("tex0", vidGrabber.getTextureReference() , 1 );
    // Pass the image texture
    shader.setUniformTexture("tex1", logoImg, 2 );
    // Pass the movie texture
    shader.setUniformTexture("tex2", fingerMovie.getTextureReference() , 3 );
    // Pass the mask texture
    shader.setUniformTexture("maskTex", maskFbo.getTextureReference() , 4 );
    
    // We are using this image just as a frame where the pixels can be arrange
    // this could be a mesh also. 
    // Comment "shader.setUniformTexture("maskTex", maskFbo.getTextureReference() , 4 );" to se how there is two ways
    // of passing a texture to the shader
    // 
    maskFbo.draw(0,0);
    
    shader.end();
    fbo.end();

    ofSetWindowTitle( ofToString( ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::gray);
    
    // draw everything
    ofSetColor(255);
    vidGrabber.draw(5,5,320,240);
    ofSetColor(ofColor::red);
    ofDrawBitmapString("RED", 5+30, 5+30);
    
    ofSetColor(255);
    logoImg.draw(320+10,5,320,240);
    ofSetColor(ofColor::green);
    ofDrawBitmapString("GREEN", 320+10+30,5+30);

    
    ofSetColor(255);
    fingerMovie.draw(320*2+15,5,320,240);
    ofSetColor(ofColor::blue);
    ofDrawBitmapString("BLUE", 320*2+5+30,5+30);
    
    
    ofSetColor(255);
    maskFbo.draw(320+10,240+10,320,240);
    ofDrawBitmapString("RGB MASK", 320+10+30,240+10+30);
    
    fbo.draw(320+10,240*2+15,320,240);
    ofDrawBitmapString("Final FBO", 320+10+30,240*2+15+30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
