#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetLogLevel(OF_LOG_VERBOSE);
    model.loadMeshes("astroBoy_walk.dae",meshes);
	tex.loadImage("boy_10.tga");

	for(int i =0; i < meshes.size();i++){
		vboMeshes.push_back(ofVboMesh());
		vboMeshes.back().meshElement = &meshes[i];
	}
	
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    glEnable(GL_DEPTH_TEST);
	
	lightsOn = true;
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
	for (int i =0; i < vboMeshes.size();i++){
		for (int j=0; j<vboMeshes[i].meshElement->getNumVertices();j++){
			ofVec3f curVert = vboMeshes[i].meshElement->getVertex(j);
			float phi = .006*ofGetFrameNum() + .05*PI*curVert.x;
			float theta = .009*ofGetFrameNum() + .15*PI*curVert.x;
//			ofVec3f modVert = .003*ofVec3f(cos(theta)*sin(phi),sin(theta)*sin(phi),cos(phi));
			ofVec3f modVert = .01*ofVec3f(0,sin(theta)*sin(phi),0);
			vboMeshes[i].meshElement->setVertex(j,curVert+modVert);
		}
		vboMeshes[i].vbo.updateVertexData(vboMeshes[i].meshElement->getVerticesPointer(),vboMeshes[i].meshElement->getNumVertices());
	}
	
	for (int i =0; i < vboMeshes.size();i++){
		for (int j=0; j<vboMeshes[i].meshElement->getNumTexCoords();j++){
			ofVec3f curVert = vboMeshes[i].meshElement->getTexCoord(j);
			float theta = .001*ofGetFrameNum();
			ofVec2f modVert = .1*ofVec3f(cos(theta),sin(theta));
			vboMeshes[i].meshElement->setTexCoord(j,curVert+modVert);
		}
		vboMeshes[i].vbo.updateTexCoordData(vboMeshes[i].meshElement->getTexCoordsPointer(),vboMeshes[i].meshElement->getNumTexCoords());
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	lightsOn ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	ofBackground(50, 50, 50, 0);
	
    ofSetColor(255, 255, 255, 255);
    
    glPushMatrix();

	glScalef(30,30,30);
	glTranslatef(17,20,0);
	glRotatef(180,0,0,1);
	//glScalef(.3,.3,.3);
	glRotatef(ofGetWidth()*.5 - mouseX,0,1,0);		

	tex.getTextureReference().bind();
	for (int i =0; i < vboMeshes.size(); i++){
//		ofSetColor(i*255.0/(vboMeshes.size()-1),0,255 - i*255.0/(vboMeshes.size()-1));
		vboMeshes[i].drawFaces();

//		ofSetColor(120,i*255.0/(vboMeshes.size()-1),i*255.0/(vboMeshes.size()-1));
//		vboMeshes[i].drawVertices();

//		ofSetColor(i*255.0/(vboMeshes.size()-1),i*255.0/(vboMeshes.size()-1),0);
//		vboMeshes[i].drawWireframe();
	}
	tex.getTextureReference().unbind();

    glPopMatrix();
    
	glDisable(GL_LIGHTING);
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key= ' ') {
		lightsOn = !lightsOn;
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

