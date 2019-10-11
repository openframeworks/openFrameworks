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
	camera.setFarClip(ofGetWidth()*12);
}

//--------------------------------------------------------------
void ofApp::update(){
	// update each box transformation
	float movementSpeed = .1;
	float spacing = 0.5;
	float now = ofGetElapsedTimef();
	float cloudSize = ofGetWidth()*4;
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
	// use drawInstanced to draw lots of instances of the same geometry
	camera.begin();
	shader.begin();
	mesh.drawInstanced(OF_MESH_WIREFRAME,matrices.size());
	shader.end();
	camera.end();
	ofDrawBitmapString(ofGetFrameRate(),20,20);
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
