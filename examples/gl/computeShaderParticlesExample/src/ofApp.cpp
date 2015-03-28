#include "ofApp.h"
#include "ofConstants.h"

//--------------------------------------------------------------
void ofApp::setup(){
	compute.setupShaderFromFile(GL_COMPUTE_SHADER,"compute1.glsl");
	compute.linkProgram();
	camera.setFarClip(ofGetWidth()*10);
	particles.resize(1024*8);
	int i=0;
	for(auto & p: particles){
		p.pos.x = ofRandom(-ofGetWidth()*0.5,ofGetWidth()*0.5);
		p.pos.y = ofRandom(-ofGetHeight()*0.5,ofGetHeight()*0.5);
		p.pos.z = ofRandom(-ofGetHeight()*0.5,ofGetHeight()*0.5);
		p.pos.w = 1;
		p.vel.set(0,0,0,0);
		i++;
	}
	particlesBuffer.allocate(particles,GL_DYNAMIC_DRAW);
	particlesBuffer2.allocate(particles,GL_DYNAMIC_DRAW);

	vbo.setVertexBuffer(particlesBuffer,4,sizeof(Particle));
	vbo.setColorBuffer(particlesBuffer,sizeof(Particle),sizeof(ofVec4f)*2);
	vbo.disableColors();
	dirAsColor = false;

	ofBackground(0);
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	gui.setup();
	shaderUniforms.setName("shader params");
	shaderUniforms.add(attractionCoeff.set("attraction",0.18,0,1));
	shaderUniforms.add(cohesionCoeff.set("cohesion",0.05,0,1));
	shaderUniforms.add(repulsionCoeff.set("repulsion",0.7,0,1));
	shaderUniforms.add(maxSpeed.set("max_speed",2500,0,5000));
	shaderUniforms.add(attractor1Force.set("attr1_force",800,0,5000));
	shaderUniforms.add(attractor2Force.set("attr2_force",800,0,5000));
	shaderUniforms.add(attractor3Force.set("attr3_force",1200,0,5000));
	gui.add(shaderUniforms);
	gui.add(fps.set("fps",60,0,60));
	gui.add(dirAsColor.set("dir as color",false));
	dirAsColor.addListener(this,&ofApp::dirAsColorChanged);

	particlesBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	particlesBuffer2.bindBase(GL_SHADER_STORAGE_BUFFER, 1);
}

//--------------------------------------------------------------
void ofApp::update(){
	fps = ofGetFrameRate();

	compute.begin();
	compute.setUniforms(shaderUniforms);
	compute.setUniform1f("timeLastFrame",ofGetLastFrameTime());
	compute.setUniform1f("elapsedTime",ofGetElapsedTimef());
	float size = 4;
	atractor1.set(ofMap(ofNoise(ofGetElapsedTimef()*0.3),0,1,-ofGetWidth()*size,ofGetWidth()*size),
			ofMap(ofNoise(ofGetElapsedTimef()*0.3+0.2),0,1,-ofGetHeight()*size,ofGetHeight()*size),
			ofMap(ofNoise(ofGetElapsedTimef()*0.3+0.5),0,1,0,-ofGetHeight()*size));
	atractor2.set(ofMap(ofNoise(ofGetElapsedTimef()*0.5+0.3),0,1,-ofGetWidth()*size,ofGetWidth()*size),
			ofMap(ofNoise(ofGetElapsedTimef()*0.5+0.2),0,1,-ofGetHeight()*size,ofGetHeight()*size),
			ofMap(ofNoise(ofGetElapsedTimef()*0.5+0.1),0,1,0,-ofGetHeight()*size));
	atractor3.set(ofMap(ofNoise(ofGetElapsedTimef()*0.9+0.1),0,1,-ofGetWidth()*size,ofGetWidth()*size),
			ofMap(ofNoise(ofGetElapsedTimef()*0.9+0.5),0,1,-ofGetHeight()*size,ofGetHeight()*size),
			ofMap(ofNoise(ofGetElapsedTimef()*0.9+0.7),0,1,0,-ofGetHeight()*size));

	compute.setUniform3f("attractor1",atractor1.x,atractor1.y,atractor1.z);
	compute.setUniform3f("attractor2",atractor2.x,atractor2.y,atractor2.z);
	compute.setUniform3f("attractor3",atractor3.x,atractor3.y,atractor3.z);
	
	// since each work group has a local_size of 1024 (this is defined in the shader)
	// we only have to issue 1 / 1024 workgroups to cover the full workload.
	// note how we add 1024 and subtract one, this is a fast way to do the equivalent
	// of std::ceil() in the float domain, i.e. to round up, so that we're also issueing
	// a work group should the total size of particles be < 1024
	compute.dispatchCompute((particles.size() + 1024 -1 )/1024, 1, 1);
	
	compute.end();

	particlesBuffer.copyTo(particlesBuffer2);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	camera.begin();
	ofSetColor(ofColor::red);
	ofDrawRectangle(atractor1,10,10);
	ofDrawRectangle(atractor2,10,10);
	ofDrawRectangle(atractor3,10,10);

	ofSetColor(255,70);
	glPointSize(5);
	vbo.draw(GL_POINTS,0,particles.size());
	ofSetColor(255);
	glPointSize(2);
	vbo.draw(GL_POINTS,0,particles.size());

	ofNoFill();
	ofDrawBox(0,0,-ofGetHeight()*2,ofGetWidth()*4,ofGetHeight()*4,ofGetHeight()*4);

	camera.end();

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetColor(255);
	gui.draw();
}

void ofApp::dirAsColorChanged(bool & dirAsColor){
	if(dirAsColor){
		vbo.enableColors();
	}else{
		vbo.disableColors();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'f'){
		ofToggleFullscreen();
	}

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
