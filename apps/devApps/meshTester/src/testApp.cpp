#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
//	ofSetLogLevel(OF_LOG_VERBOSE);
    model.loadMeshes("astroBoy_walk.dae",data);
	tex.loadImage("boy_10.tga");
	
	ofEnableNormalizedTexCoords();

	for(int i =0; i < data.size();i++){
		meshes.push_back(ofMesh());
		meshes.back().vertexData = &data[i];
		meshes.back().enableTexCoords();
		meshes.back().enableNormals();
		meshes.back().drawType = GL_STREAM_DRAW_ARB;
		meshes.back().setUseIndices();
		meshes.back().setRenderMethod(OF_MESH_USING_VBO);
	}
	
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


	for (int i =0; i < meshes.size();i++){
		for (int j=0; j<meshes[i].vertexData->getNumVertices();j++){
			ofVec3f curVert = meshes[i].vertexData->getVertex(j);
			float phi = .006*ofGetFrameNum() + .05*PI*curVert.x;
			float theta = .009*ofGetFrameNum() + .15*PI*curVert.x;
			ofVec3f modVert = .01*ofVec3f(0,sin(theta)*sin(phi),0);
			meshes[i].vertexData->setVertex(j,curVert+modVert);
		}
	}
	
	for (int i =0; i < meshes.size();i++){
		for (int j=0; j<meshes[i].vertexData->getNumTexCoords();j++){
			ofVec3f curVert = meshes[i].vertexData->getTexCoord(j);
			float theta = .002*ofGetFrameNum();
			ofVec2f modVert = .001*ofVec3f(cos(theta),sin(theta));
			meshes[i].vertexData->setTexCoord(j,curVert+modVert);
		}
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
	for (int i =0; i < meshes.size(); i++){
//		ofSetColor(i*255.0/(meshes.size()-1),0,255 - i*255.0/(meshes.size()-1));
		meshes[i].drawFaces();

//		ofSetColor(120,i*255.0/(vboMeshes.size()-1),i*255.0/(vboMeshes.size()-1));
//		meshes[i].drawVertices();

//		ofSetColor(i*255.0/(meshes.size()-1),i*255.0/(meshes.size()-1),0);
//		meshes[i].drawWireframe();
	}
	tex.getTextureReference().unbind();

    glPopMatrix();
    
	glDisable(GL_LIGHTING);
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key== ' ') {
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

