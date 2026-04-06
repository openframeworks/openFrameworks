#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(0);
	matrices.resize(3000);

	// upload the transformation for each box using a
	// texture buffer.
	// for that we need to upload the matrices to the buffer
	// and allocate the texture using it
	buffer.allocate();
	buffer.bind(GL_TEXTURE_BUFFER);
	buffer.setData(matrices,GL_STREAM_DRAW);

	// using GL_RGBA32F allows to read each row of each matrix
	// as a float vec4 from the shader.
	// Note that we're allocating the texture as a Buffer Texture:
	// https://www.opengl.org/wiki/Buffer_Texture
	tex.allocateAsBufferTexture(buffer,GL_RGBA32F);

	// now we bind the texture to the shader as a uniform
	// so we can read the texture buffer from it
	shader.load("vert.glsl","frag.glsl");
	shader.begin();
	shader.setUniformTexture("tex",tex,0);
	shader.end();
	
	// load the shader main functions //
	string vname = "vert.glsl";
	ofBuffer vbuffer = ofBufferFromFile(vname);
	if( vbuffer.size()) {
		// configure the shader to include shadow functions for passing depth
		// #define OF_SHADOW_DEPTH_PASS gets added to the shader file in the setupShadowDepthShader function
		light.getShadow().setupShadowDepthShader(mDepthShader, vbuffer.getText());
	}
	
	mDepthShader.begin();
	mDepthShader.setUniformTexture("tex",tex,0);
	mDepthShader.end();

	// we are going to use instanced drawing so we
	// only need one geometry
	mesh = ofMesh::box(50,50,50,1,1,1);
	mesh.setUsage(GL_STATIC_DRAW);

	// we want each box to have a different color so let's add
	// as many colors as boxes
	mesh.getColors().resize(matrices.size());
	for(size_t i=0;i<mesh.getColors().size();i++){
		mesh.getColors()[i] = ofColor::fromHsb(i % 255, 255, 255);
	}

	// then we tell the vbo that colors should be used per instance by using
	// ofVbo::setAttributeDivisor
	mesh.getVbo().setAttributeDivisor(ofShader::COLOR_ATTRIBUTE,1);


	// lots of boxes, let's move the camera backwards
	camera.setDistance(ofGetWidth()*6);
	camera.setFarClip(ofGetWidth()*15);
	
	light.setDirectional();
	light.enable();
	light.setPosition(0.0, 3600, 600 );
	light.lookAt( glm::vec3(0,0,0) );
	light.getShadow().setEnabled(true);
	light.getShadow().setCullingEnabled(true);
	light.getShadow().setStrength(0.2);
	light.getShadow().setNearClip(200);
	light.getShadow().setFarClip(7500);
	light.getShadow().setDirectionalBounds(7000, 7000);
}

//--------------------------------------------------------------
void ofApp::update(){
	if( light.shouldRenderShadowDepthPass() ) {
		int numShadowPasses = light.getNumShadowDepthPasses();
		for( int j = 0; j < numShadowPasses; j++ ) {
			light.beginShadowDepthPass(j);
			renderScene(true);
			light.endShadowDepthPass(j);
		}
	}
	
	// update each box transformation
	float movementSpeed = .1;
	float spacing = 0.25;
	float now = ofGetElapsedTimef();
	float cloudSize = ofGetWidth()*3;
	for(size_t i=0;i<matrices.size();i++){
		ofNode node;

		float t = (now + i * spacing) * movementSpeed;
		glm::vec3 pos(
			ofSignedNoise(t, 0, 0),
			ofSignedNoise(0, t, 0),
			ofSignedNoise(0, 0, t));

		pos *= cloudSize;
		node.setPosition(pos);
		node.setOrientation(pos);
		matrices[i] = node.getLocalTransformMatrix();
	}

	// and upload them to the texture buffer
	buffer.updateData(0,matrices);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableDepthTest();
	if( light.shouldRenderShadowDepthPass() ) {
		int numShadowPasses = light.getNumShadowDepthPasses();
		for( int j = 0; j < numShadowPasses; j++ ) {
			light.beginShadowDepthPass(j);
			renderScene(true);
			light.endShadowDepthPass(j);
		}
	}
	
	// use drawInstanced to draw lots of instances of the same geometry
	camera.begin();
//	shader.begin();
	//mesh.drawInstanced(OF_MESH_WIREFRAME,matrices.size());
//	mesh.drawInstanced(OF_MESH_FILL,matrices.size());
	renderScene(false);
	
	light.draw();
	
	if( light.getIsEnabled() && light.getShadow().getIsEnabled() ) {
		light.getShadow().drawFrustum();
	}
//	shader.end();
	camera.end();
	ofDisableDepthTest();
	ofDrawBitmapString(ofGetFrameRate(),20,20);
}

//--------------------------------------------------------------
void ofApp::renderScene(bool bShadowPass) {
	if( bShadowPass ) {
		mDepthShader.begin();
	} else {
		shader.begin();
	}
	
	mesh.drawInstanced(OF_MESH_FILL,matrices.size());
	
	if(bShadowPass) {
		mDepthShader.end();
	} else {
		shader.end();
	}
	ofSetColor( 180 );
	matFloor.begin();
	ofDrawBox( 0, -(float)ofGetWidth()*3.f, 0, 7000, 50, 7000 );
	matFloor.end();
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
