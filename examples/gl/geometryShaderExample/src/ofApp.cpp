#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(50, 50, 50);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();

	shader.setGeometryInputType(GL_LINES);
	shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	shader.setGeometryOutputCount(4);
	if( ofIsGLProgrammableRenderer() ) {
		shader.load("shadersGL3/vert.glsl", "shadersGL3/frag.glsl", "shadersGL3/geom.glsl");
	} else {
		shader.load("shadersGL2/vert.glsl", "shadersGL2/frag.glsl", "shadersGL2/geom.glsl");
	}

	ofLog() << "Maximum number of output vertices support is: " << shader.getGeometryMaxOutputCount();
	
	camera.setFarClip(20000);
	camera.move(0,0,1000);

	// create a bunch of random points
	float r = ofGetHeight()/2;
	for(int i=0; i<100; i++) {
		points.push_back(glm::vec3(ofRandomf() * r, ofRandomf() * r, ofRandomf() * r));
	}
	
	colors.push_back(ofColor(230));
	colors.push_back(ofColor(199,57,194));
	colors.push_back(ofColor(28,219,244));
	colors.push_back(ofColor(40));
	
	mesh.setMode(OF_PRIMITIVE_LINES);
	mesh.addVertices( points );
	// set some placeholder normals that will be updated if smooth == true in update()
	for( int i = 0; i < mesh.getNumVertices(); i++ ) {
		mesh.addNormal(glm::vec3(1,0,0));
		int colorIndex = i % colors.size();
		mesh.addColor(colors[colorIndex]);
	}
	// now lets setup the indices for drawing lines
	for( int i = 0; i < mesh.getNumVertices()-1; i++ ) {
		mesh.addIndex(i);
		mesh.addIndex(i+1);
	}

	doShader = true;
	bUseMesh = false;
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update(){
	
	float deltaTime = ofClamp(ofGetLastFrameTime(), 1.f/5000.0f, 1.f / 5.f );
	
	baseAngle += deltaTime * baseAngleSpeed;
	
	float xAngle = baseAngle;
	float yAngle = baseAngle;
	float zAngle = baseAngle;
	
	float radius = 500.f;
	
	// update the locations of the points
	for( size_t i = 0; i < points.size(); i++ ){
		auto& point = points[i];
		point.x = cosf(xAngle) * sinf(xAngle*0.4) * radius;
		point.y = sinf(yAngle) * cosf(zAngle * 2.4f) * radius;
		point.z = sinf(zAngle) * cosf(zAngle*0.74) * radius;

		xAngle += xAngleIncrement;
		yAngle += yAngleIncrement;
		zAngle += zAngleIncrement;
		
		if( bUseMesh ) {
			// now update the vertex on the mesh
			mesh.setVertex(i, point);
		}
	}
	
	if( bUseColors ){
		mesh.enableColors();
	}else{
		mesh.disableColors();
	}
	
	if( bUseMesh ){
		mesh.enableNormals();
		glm::vec3 normal = glm::vec3(0,0,0);
		glm::vec3 direction = glm::vec3(0,0,0);
		for( size_t i = 0; i < points.size(); i++ ){
			if( i == 0 ){
				direction = points[i+1] - points[i];
			} else if( i == points.size()-1 ) {
				direction = points[i] - points[i-1];
			}else{
				direction = points[i+1] - points[i-1];
			}
			direction = glm::normalize(direction);
			normal = glm::normalize(glm::cross(direction, glm::vec3(0,0,1)));
			// vary the length of the normals that will be read by the shader
			// will allow for varying line widths per vertex
			float normalVariation = (cos((float)i*0.3f) * 0.5+0.5) * 2.5 + 0.3;
			mesh.setNormal(i, normal * normalVariation);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	// draw a gradient in the background
	ofBackgroundGradient(ofColor(10), ofColor(50));
	
	camera.begin();
	
	if(doShader){
		shader.begin();

		// set thickness of ribbons
		shader.setUniform1f("thickness", 20);
		
		if( bUseMesh){
			shader.setUniform1f("uUseNormals", 1.0);
		}else{
			shader.setUniform1f("uUseNormals", 0.0);
		}
		
		// make light direction slowly rotate
		shader.setUniform3f("lightDir", sin(ofGetElapsedTimef()/10), cos(ofGetElapsedTimef()/10), 0);
	}

	ofSetColor(255);
	
	if( bUseMesh ){
		mesh.draw();
	}else{
		for(unsigned int i=1; i<points.size(); i++){
			if( bUseColors ) {
				int colorIndex = i % colors.size();
				ofSetColor( colors[colorIndex] );
			}
			ofDrawLine(points[i-1], points[i]);
		}
	}
	
	if(doShader){
		shader.end();
	}
	
	camera.end();
	
	ofSetColor(255);

	ofDrawBitmapString("fps: " + ofToString((int)ofGetFrameRate()) + "\nPress 's' to toggle shader: " + (doShader ? "ON" : "OFF"), 15, 15);
	ofDrawBitmapString("Use Mesh (spacebar): "+ofToString(bUseMesh, 0), 15, 45 );
	ofDrawBitmapString("X Angle Increment (o/p): "+ofToString(xAngleIncrement, 2), 15, 60 );
	ofDrawBitmapString("Y Angle Increment (k/l): "+ofToString(yAngleIncrement, 2), 15, 75 );
	ofDrawBitmapString("Z Angle Increment (m/,): "+ofToString(zAngleIncrement, 2), 15, 90 );
	ofDrawBitmapString("Angle Speed (up/down): "+ofToString(baseAngleSpeed, 2), 15, 105 );
	ofDrawBitmapString("Use Colors (c): "+ofToString(bUseColors, 0), 15, 120 );
	
	
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	if( key == 's' ){
		doShader = !doShader;
	}
	if( key == OF_KEY_UP ){
		baseAngleSpeed += 0.5;
	}
	if( key == OF_KEY_DOWN ){
		baseAngleSpeed -= 0.5f;
	}
	if( key == 'c'){
		bUseColors = !bUseColors;
	}
	if( key == ' ' ){
		bUseMesh = !bUseMesh;
	}
	if( key == 'o' ){
		xAngleIncrement -= 0.05f;
	}
	if( key == 'p' ){
		xAngleIncrement += 0.05f;
	}
	if( key == 'k' ){
		yAngleIncrement -= 0.05f;
	}
	if( key == 'l' ){
		yAngleIncrement += 0.05f;
	}
	if( key == 'm' ){
		zAngleIncrement -= 0.05f;
	}
	if( key == ',' ){
		zAngleIncrement += 0.05f;
	}
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

