#include "ofApp.h"


constexpr int numVertices = 256;

//--------------------------------------------------------------
void ofApp::setup(){
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.getVertices().resize(numVertices);
	ofSetBackgroundColor(0);

	ofShader::TransformFeedbackSettings settings;
	settings.shaderFiles[GL_VERTEX_SHADER] = "vert.glsl";
	settings.bindDefaults = false;
	settings.varyingsToCapture = { "v_position", "v_color" };
	shader.setup(settings);


	buffer.allocate(sizeof(glm::vec4) * 2 * numVertices, GL_STATIC_DRAW);

	ofShader::TransformFeedbackBinding binding(buffer);
	binding.index = 0;
	binding.offset = 0;
	binding.size = numVertices * sizeof(glm::vec4) * 2;
	
	shader.beginTransformFeedback(GL_POINTS, binding);
	mesh.draw();
	shader.endTransformFeedback(binding);

	vbo.setVertexBuffer(buffer, 4, sizeof(glm::vec4) * 2, 0);
	vbo.setColorBuffer(buffer, sizeof(glm::vec4) * 2, sizeof(glm::vec4));

	ofEnablePointSprites();
	glPointSize(4);
	cam.setDistance(2400);
	cam.orbit(80, 80, 2400, { 0.f, 0.f, 0.f });
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	vbo.draw(GL_POINTS, 0, numVertices);
	cam.end();
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
