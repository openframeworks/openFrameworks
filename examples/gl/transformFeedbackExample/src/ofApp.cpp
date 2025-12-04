#include "ofApp.h"


constexpr int numVertices = 256;

//--------------------------------------------------------------
void ofApp::setup(){
	// allocate numVertices without setting them, the vertex
	// shader will fill the positions and colors
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.getVertices().resize(numVertices);
	ofSetBackgroundColor(0);

	// We only need a vertex shader since we are only using the
	// shader to store the modified vertices in a buffer
	ofShader::TransformFeedbackSettings settings;
	settings.shaderFiles[GL_VERTEX_SHADER] = "vert.glsl";
	settings.bindDefaults = false;
	settings.varyingsToCapture = { "v_position", "v_color" };

	ofShader shader;
	shader.setup(settings);

	// allocate enough space for all the vertices in a gpu buffer
	buffer.allocate(sizeof(glm::vec4) * 2 * numVertices, GL_STATIC_DRAW);

	
	// bind the full buffer using glBindBaseBuffer to default index 0
	// and draw the mesh which will end up stored in our buffer
	shader.beginTransformFeedback(GL_POINTS, buffer);
	shader.setUniform1i("numVertices", numVertices);
	mesh.draw();
	shader.endTransformFeedback(buffer);

	// more fine grained control with range bindings:
	// ofShader::TransformFeedbackRangeBinding binding(buffer);
	// binding.index = 0;
	// binding.offset = 0;
	// binding.size = numVertices * sizeof(glm::vec4) * 2;
	// shader.beginTransformFeedback(GL_POINTS, binding);

	// now set the buffer as vertices and colors in the vbo
	// by specifying the stride and offsets
	// since we have position and color both as 4 coordinates:
	// 4 coordinates position at offset 0
	vbo.setVertexBuffer(buffer, 4, sizeof(glm::vec4) * 2, 0);
	// color at offset sizeof(glm::vec4)
	vbo.setColorBuffer(buffer, sizeof(glm::vec4) * 2, sizeof(glm::vec4));
	
	// use a render shader so we can control the size of the points 
	renderShader.load("pointSprite.vert", "pointSprite.frag" );
}

//--------------------------------------------------------------
void ofApp::update(){
	cam.orbitDeg(ofGetElapsedTimef() * 100., 0, 1800, {0,0,0});
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableDepthTest();
	cam.begin();
	ofEnablePointSprites();
	renderShader.begin();
	vbo.draw(GL_POINTS, 0, numVertices);
	renderShader.end();
	ofDisablePointSprites();
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
