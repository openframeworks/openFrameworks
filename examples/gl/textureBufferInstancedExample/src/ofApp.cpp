#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(0);
	ofSetFrameRate(60);
	matrices.resize(600);
	nodes.resize(600);
	buffer.allocate();
	buffer.bind(GL_TEXTURE_BUFFER);
	buffer.setData(matrices,GL_STREAM_DRAW);

	ofTextureData texData;
	texData.glTypeInternal = GL_RGBA32F;
	texData.textureTarget = GL_TEXTURE_BUFFER;
	texData.width = matrices.size();
	texData.height = 1;
	tex.allocate(texData);
	tex.bind();
	glTexBuffer(GL_TEXTURE_BUFFER,GL_RGBA32F,buffer.getId());
	tex.unbind();

	shader.load("vert.glsl","frag.glsl");
	shader.begin();
	shader.setUniformTexture("tex",tex,0);
	shader.setUniform1i("numInstances",matrices.size());
	shader.end();

	mesh = ofMesh::box(50,50,50,1,1,1);
}

//--------------------------------------------------------------
void ofApp::update(){
	float movementSpeed = .1;
	float spacing = 1;
	float now = ofGetElapsedTimef();
	for(int i=0;i<matrices.size();i++){
		ofNode & node = nodes[i];

		float t = (now + i * spacing) * movementSpeed;
		ofVec3f pos(
			ofSignedNoise(t, 0, 0),
			ofSignedNoise(0, t, 0),
			ofSignedNoise(0, 0, t));

		pos *= ofGetWidth()/2;
		node.setPosition(pos);
		node.setOrientation(pos);
		matrices[i] = node.getLocalTransformMatrix();
	}
	buffer.updateData(0,matrices);
}

//--------------------------------------------------------------
void ofApp::draw(){
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
