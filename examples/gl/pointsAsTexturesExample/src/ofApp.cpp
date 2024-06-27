#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofBackgroundHex(0x000000);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	// load the texure
	ofDisableArbTex();
	ofLoadImage(texture, "dot.png");
	ofLoadImage(logoTexture, "ofLogo.png");

	// set the camera distance
	camDist = 1605;
	camera.setDistance(camDist);
	
	// randomly add a point on a sphere
	int   num = 500;
	float radius = 1000;
	for(int i = 0; i<num; i++ ) {
		
		float theta1 = ofRandom(0, glm::two_pi<float>());
		float theta2 = ofRandom(0, glm::two_pi<float>());
		
		glm::vec3 p;
		p.x = std::cos( theta1 ) * std::cos( theta2 );
		p.y = std::sin( theta1 );
		p.z = std::cos( theta1 ) * std::sin( theta2 );

		p *= radius;
		
		addPoint(p.x, p.y, p.z);
	}
	
	// upload the data to the vbo
	int total = (int)points.size();
	vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
	vbo.setColorData(&colors[0], total, GL_STATIC_DRAW);
	
	// we will be drawing point sprites, so set mode on mesh to POINTS
	mesh.setMode(OF_PRIMITIVE_POINTS);

	if( ofIsGLProgrammableRenderer() ) {
		shader.load("shaders_gl3/shader");
	} else {
		shader.load("shaders/shader");
	}

}

//--------------------------------------------------------------
void ofApp::update() {
	pointSize = ofMap(ofGetMouseX(), 100.f, ofGetWidth() - 50.0f, 1.0f, 60.f, true);
	ofSetPointSize(pointSize);

	if( bUseShader ) {
		mesh.enableColors();
		vbo.enableColors();
	} else {
		if( bUseColors ) {
			mesh.enableColors();
			vbo.enableColors();
		} else {
			mesh.disableColors();
			vbo.disableColors();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	glDepthMask(GL_FALSE);
	
	ofSetColor(255, 100, 90);

	ofEnableAlphaBlending();
	
	// this makes everything look glowy :)
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites();

	if (bSmoothing) {
		ofEnableSmoothing();
	} else {
		ofDisableSmoothing();
	}
	
	// determine if we should bind a texture for our points to draw
	ofTexture* tex = nullptr;
	if (bUseShader) {
		// we are using a shader and it expects a texture 
		tex = &texture;
	} else {
		if( texIndex == 1 ) {
			tex = &texture;
		} else if( texIndex == 2 ) {
			tex = &logoTexture;
		}
	}
	
	// bind the shader and camera
	// everything inside this function
	// will be effected by the shader/camera
	camera.begin();
	if (bUseShader) {
		shader.begin();
		// pass the point size to the shader
		shader.setUniform1f("pointSize", pointSize);
	}
	
	if( tex != nullptr) {
		// bind the texture so that when all the points
		// are drawn they are replace with our dot image
		tex->bind();
	}
	
	
	if (bUseShader) {
		// draw via the vbo for the custom shader
		vbo.draw(GL_POINTS, 0, (int)points.size());
	} else {
		// lets draw the mesh
		mesh.drawVertices();
	}
	
	if (tex != nullptr) {
		tex->unbind();
	}
	
	if (bUseShader){
		shader.end();
	} else {
		
	}

	ofDisablePointSprites();

	camera.end();
	
	ofDisableBlendMode();
	
	// check to see if the points are
	// sizing to the right size
	ofEnableAlphaBlending();
	camera.begin();
	for (unsigned int i=0; i<points.size(); i++) {
		ofSetColor(255, 80);
		glm::vec3 mid = points[i];
		mid = glm::normalize(mid);
		mid *= 300;
		ofDrawLine(points[i], mid);
	}
	camera.end();

	glDepthMask(GL_TRUE);

	ofSetColor(255, 100);
	ofDrawRectangle(0, 0, 250, bUseShader ? 90 : 120);
	ofSetColor(0);
	string info = "FPS "+ofToString(ofGetFrameRate(), 0) + "\n";
	info += "Total Points "+ofToString((int)points.size())+"\n";
	info += "Press 'a' to add more\n";
	info += "Press 'c' to remove all\n";
	info += "Press 's' for shader: " + string(bUseShader ? "yes":"no")+"\n";
	if( !bUseShader) {
		info += "Press 'x' for colors: " + string(bUseColors ? "yes" : "no") + "\n";
		info += "Press 't' for texture: " + string(texIndex == 0 ? "no" : "yes") + "\n";
		info += "Press 'z' for smoothing: " + string(bSmoothing ? "yes" : "no") + "\n";
	}

	ofDrawBitmapString(info, 20, 20);
}

//--------------------------------------------------------------
void ofApp::addPoint(float x, float y, float z) {
	glm::vec3 p(x, y, z);
	points.push_back(p);
	
	// we are passing the size varation per point via the normal x
	float size = ofRandom(1.f, 2.f);
	sizes.push_back(glm::vec3(size));

	ofFloatColor tc;
	tc.setHsb(ofRandom(0.5f, 1.0f), 0.7f, 0.8f, 1.0f);

	colors.push_back(tc);
	mesh.addColor(tc);
	mesh.addVertex(p);
	mesh.addNormal(glm::vec3(size));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	if(key == OF_KEY_UP) {
		camDist -= 10;
	}
	if(key == OF_KEY_DOWN) {
		camDist += 10;
	}
	camera.setDistance(camDist);
	
	
	// clear all the points
	if(key == 'c') {
		points.clear();
		sizes.clear();
		colors.clear();
		mesh.clear();
	}
	
	// add crazy amount
	if(key == 'a') {
		float theta1 = ofRandom(0, glm::two_pi<float>());
		float theta2 = ofRandom(0, glm::two_pi<float>());
		glm::vec3 p;
		p.x = std::cos( theta1 ) * std::cos( theta2 );
		p.y = std::sin( theta1 );
		p.z = std::cos( theta1 ) * std::sin( theta2 );
		p  *= 800;
		addPoint(p.x, p.y, p.z);
		int total = (int)points.size();
		vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
		vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
		vbo.setColorData(&colors[0], total, GL_STATIC_DRAW);
	}
	
	if( key == 's' ) {
		bUseShader = !bUseShader;
	}
	if( key == 't') {
		texIndex ++;
		if( texIndex > 2 ) {
			texIndex = 0;
		}
	}
	if( key == 'z' ) {
		bSmoothing = !bSmoothing;
	}
	if( key == 'x') {
		bUseColors = !bUseColors;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
