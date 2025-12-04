#include "ofApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup(){
	// Disabling arb textures loads all of the textures with tex coords from 0-1
	// If ARB is left enabled, then it would be 0 -> tex width and 0 -> tex height
	// opengl es only supports non arb textures where tex coords are 0 - 1
	ofDisableArbTex();
	ofEnableAlphaBlending();
	int camWidth 		= 320;	// try to grab at this size.
	int camHeight 		= 240;

	vidGrabber.setVerbose(true);
	vidGrabber.setup(camWidth,camHeight);

	fingerMovie.load("fingers.mov");
	fingerMovie.play();

	logoImg.load("colors.jpg");
	multimaskImg.load("mask.jpg");

	fbo.allocate(camWidth,camHeight);
	maskFbo.allocate(camWidth,camHeight);

	//ofSetWindowShape(camWidth, camHeight*2);

	// There are 3 of ways of loading a shader:
	//
	//  1 - Using just the name of the shader and letting ofShader look for .frag and .vert:
	//      Ex.: shader.load( "myShader");
	//
	//  2 - Giving the right file names for each one:
	//      Ex.: shader.load( "myShader.vert","myShader.frag");
	//
	//  3 - And the third one is passing the shader programa on a single string;
	//      In this particular example we are using STRINGIFY which is a handy macro
	
	
	if( ofIsGLProgrammableRenderer() ) {
		string shaderProgramVert = STRINGIFY(
											 // OF_GLSL_SHADER_HEADER is replaced by OF with the appropriate shader header
											 OF_GLSL_SHADER_HEADER
											 uniform mat4 modelViewProjectionMatrix;
											 in vec4 position;

											 void main() {
												 gl_Position = modelViewProjectionMatrix * position;
											 }

											 );
		
		string shaderProgramFrag = STRINGIFY(
										// OF_GLSL_SHADER_HEADER is replaced by OF with the appropriate shader header
										OF_GLSL_SHADER_HEADER
									   uniform float texCoordWidthScale;
									   uniform float texCoordHeightScale;
											 
									   uniform sampler2D tex0;
									   uniform sampler2D tex1;
									   uniform sampler2D tex2;
									   uniform sampler2D maskTex;
											 
										out vec4 oFragColor;
										 
										 void main(){
											 vec2 pos = gl_FragCoord.xy;
											 pos.x /= texCoordWidthScale;
											 pos.y /= texCoordHeightScale;
											 
											 vec4 rTxt = texture(tex0, pos);
											 vec4 gTxt = texture(tex1, pos);
											 vec4 bTxt = texture(tex2, pos);
											 vec4 mask = texture(maskTex, pos);
											 
											 vec4 color = vec4(0,0,0,0);
											 color = mix(color, rTxt, mask.r );
											 color = mix(color, gTxt, mask.g );
											 color = mix(color, bTxt, mask.b );
											 
											 oFragColor = color;
											 
										 }
										 );
		
		shader.setupShaderFromSource(GL_VERTEX_SHADER, shaderProgramVert);
		shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgramFrag);
		shader.linkProgram();
	
	} else {
		
		string shaderProgram = STRINGIFY(
										 OF_GLSL_SHADER_HEADER
										 uniform float texCoordWidthScale;
										 uniform float texCoordHeightScale;
										 uniform sampler2D tex0;
										 uniform sampler2D tex1;
										 uniform sampler2D tex2;
										 uniform sampler2D maskTex;
										 
										 void main (void){
//											 vec2 pos = gl_TexCoord[0].st;
											 vec2 pos = gl_FragCoord.xy;
											 pos.x /= texCoordWidthScale;
											 pos.y /= texCoordHeightScale;
											 
											 vec4 rTxt = texture2D(tex0, pos);
											 vec4 gTxt = texture2D(tex1, pos);
											 vec4 bTxt = texture2D(tex2, pos);
											 vec4 mask = texture2D(maskTex, pos);
											 
											 vec4 color = vec4(0,0,0,0);
											 color = mix(color, rTxt, mask.r );
											 color = mix(color, gTxt, mask.g );
											 color = mix(color, bTxt, mask.b );
											 
											 gl_FragColor = color;
										 }
										 );
		
		shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
		shader.linkProgram();
	}

	

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
	
	if( vidGrabber.getTexture().isAllocated() && fingerMovie.getTexture().isAllocated()) {
		fbo.begin();
		ofClear(0, 0, 0,255);
		shader.begin();
		
		// sampler2D expects 0-1 for tex coordinates
		// we pass in the width and height of the fbo so that we can normalize it in the shader
		shader.setUniform1f("texCoordWidthScale", fbo.getWidth());
		shader.setUniform1f("texCoordHeightScale", fbo.getHeight());
		
		// Pass the video texture
		shader.setUniformTexture("tex0", vidGrabber.getTexture() , 1 );
		// Pass the image texture
		shader.setUniformTexture("tex1", logoImg, 2 );
		// Pass the movie texture
		shader.setUniformTexture("tex2", fingerMovie.getTexture() , 3 );
		// Pass the mask texture
		shader.setUniformTexture("maskTex", maskFbo.getTexture() , 4 );
		
		// We are using this image just as a frame where the pixels can be arrange
		// this could be a mesh also.
		// Comment "shader.setUniformTexture("maskTex", maskFbo.getTexture() , 4 );" to se how there is two ways
		// of passing a texture to the shader
		//
		maskFbo.draw(0,0);
		
		shader.end();
		fbo.end();
	}

	ofSetWindowTitle( ofToString( ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor::gray);

	// draw everything
	ofSetColor(255);
	if( vidGrabber.getTexture().isAllocated() ) {
		vidGrabber.draw(5,5,320,240);
	}
	ofSetColor(ofColor::red);
	ofDrawBitmapString("RED", 5+30, 5+30);

	ofSetColor(255);
	logoImg.draw(320+10,5,320,240);
	ofSetColor(ofColor::green);
	ofDrawBitmapString("GREEN", 320+10+30,5+30);


	ofSetColor(255);
	if(fingerMovie.getTexture().isAllocated() ) {
		fingerMovie.draw(320*2+15,5,320,240);
	}
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
