#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
//	ofSetLogLevel(OF_LOG_VERBOSE);
    ofLoadModel("astroBoy_walk.dae", model);
	
	ofEnableNormalizedTexCoords();
	model.enableTextures();
	model.enableNormals();

	map<string, ofMesh*>::iterator it;
	for(it=model.named_meshes.begin();it!=model.named_meshes.begin();it++){
		cout << it->first << endl;
	}

    glEnable(GL_DEPTH_TEST);
	
	lightsOn = true;
	wiggleModel = false;
	glEnable(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(4);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
}

//--------------------------------------------------------------
void testApp::update(){
	/*
	if(wiggleModel){
		for (int i =0; i < (int)model.meshes.size();i++){
			for (int j=0; j<model.meshes[i].vertexData->getNumVertices();j++){
				ofVec3f curVert = model.meshes[i].vertexData->getVertex(j);
				float phi = .006*ofGetFrameNum() + .05*PI*curVert.x;
				float theta = .009*ofGetFrameNum() + .15*PI*curVert.x;
				ofVec3f modVert = .01*ofVec3f(0,sin(theta)*sin(phi),0);
				model.meshes[i].vertexData->setVertex(j,curVert+modVert);
			}
		}
		
		for (int i =0; i < (int)model.meshes.size();i++){
			for (int j=0; j<model.meshes[i].vertexData->getNumTexCoords();j++){
				ofVec3f curVert = model.meshes[i].vertexData->getTexCoord(j);
				float theta = .002*ofGetFrameNum();
				ofVec2f modVert = .001*ofVec3f(cos(theta),sin(theta));
				model.meshes[i].vertexData->setTexCoord(j,curVert+modVert);
			}
		}
	}
	*/
}

//--------------------------------------------------------------
void testApp::draw(){
	lightsOn ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	ofBackground(50, 50, 50, 0);
    
    glPushMatrix();

	glScalef(30,30,30);
	glTranslatef(17,20,0);
	glRotatef(180,0,0,1);
	glRotatef(ofGetWidth()*.5 - mouseX,0,1,0);		

	ofSetColor(255,255,255);
	model.drawWireframe();

	glPointSize(20);
	glDisable(GL_LIGHTING);
	ofFill();
	
	for (int i =0; i < (int)model.nodes.size();i++){
		model.nodes.at(i).transformGL();
		ofDrawAxis(0.1);
		model.nodes.at(i).restoreTransformGL();
	}

    glPopMatrix();
    
	ofSetColor(255, 255, 255, 255);
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15,0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key== ' ') {
		lightsOn = !lightsOn;
	}
	if (key == 'w'){
		wiggleModel = !wiggleModel;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

