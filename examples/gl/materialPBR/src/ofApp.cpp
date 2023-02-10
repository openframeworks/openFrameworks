#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(false);
	
	camera.setNearClip(200);
	camera.setFarClip(15000.0);
	
	// loading a cube map will enable image based lighting on PBR materials.
	// cube map will create a prefiltered light cube map and an irradiance cube map
	// cube map texture from https://polyhaven.com
	#ifdef USE_CUBE_MAP
	// comment out the loading of the cube map image to see added cube map lighting without image
	// fake environment lighting is added in the pbr shader
	cubeMap.load("modern_buildings_2_1k.exr", 512, true );
	#endif
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofEnableDepthTest();
	camera.begin();
	float tradius = 50.0;
	int numCols = 5;
	int numRows = 3;
	glm::vec3 pos = {0,0,0};
	float spacing = 10.0;
	material.begin();

	for( int y = 0; y < numRows; y++ ) {
		pos.y = ((float)(numRows-1.f)/2.0f) * ((tradius*2.0)+spacing) - (float)y * ((tradius*2.0)+spacing);
//
		for( int x = 0; x < numCols; x++ ) {
			float xpct = (float)x / ((float)numCols-1);

			pos.x = -((float)(numCols-1.f)/2.0f) * ((tradius*2.0)+spacing) + (float)x * ((tradius*2.0)+spacing);
//			pos.x *= -1.f;
			// most properties (excluding textures) of the PBR material can be set inside material.begin()
			// setting properties that are related to PBR convert the material to a PBR material
			// a material can also be set to PBR explicitly by calling material.setPBR(true).
			if( y == 0 ) {
				material.setRoughness(0.05f);
				material.setReflectance(0.0f);
				material.setMetallic(xpct);
				material.setDiffuseColor(ofFloatColor(0.97, 0.96, 0.91, 1.0) );
				material.setDiffuseColor(ofFloatColor(1.0, 1.0) );
			} else if( y == 1 ) {
				material.setReflectance(0.01);
				material.setRoughness(ofClamp(xpct+0.05, 0., 1.));
				material.setMetallic(1.0);
				// copper metal: 0.97, 0.74, 0.62
				material.setDiffuseColor(ofFloatColor(0.97, 0.74, 0.62, 1.0) );
			} else if( y == 2 ) {
				material.setRoughness(0.1);
				material.setMetallic(0.0);
				material.setReflectance(xpct);
				material.setDiffuseColor(ofFloatColor(1.0, 1.0) );
			}
			ofDrawSphere( pos, tradius );
		}
	}
	material.end();
	
	#ifdef USE_CUBE_MAP
	// drawing of the cube map renders at max depth, so it can be drawn last
	// this will allow for the benefit of depth clipping
	if( cubeMapMode == 2 ) {
		if( cubeMap.hasPrefilteredMap() ) {
			cubeMap.drawPrefilteredCube(0.25f);
		} else {
			ofLogNotice("DOES NOT HAVE PRE FILTERED CUBE MAP") << " " << ofGetFrameNum();
		}
	} else if( cubeMapMode == 3 ) {
		if( cubeMap.hasIrradianceMap() ) {
			cubeMap.drawIrradiance();
		}
	} else {
		if( cubeMap.hasCubeMap() ) {
			cubeMap.drawCubeMap();
		}
	}
	#endif
	
	camera.end();
	ofDisableDepthTest();
	
	stringstream ss;
	string cmMode = "Cube Map";
	if( cubeMapMode == 2 ) {
		cmMode = "Prefiltered";
	} else if(cubeMapMode == 3 ) {
		cmMode = "Irradiance";
	}
	#ifdef USE_CUBE_MAP
	if(cubeMap.hasCubeMap()) {
		ss << "Cube Map mode(1,2,3): " << cmMode;
	} else {
		ss << "Cube map deteced with no images, so adding fake cube lighting.";
	}
	#else
	ss << "No light or cube map detected, adding fake light from camera position";
	#endif
	ss << endl << "Click and drag to move camera.";
	if( !ofIsGLProgrammableRenderer() ) {
		ss << endl << "PBR only works with programmable renderer.";
	}
	ss << endl << "Row 1: 0 -> 1 metallic of smooth surface";
	ss << endl << "Row 2: 0 -> 1 roughness of metallic surface.";
	ss << endl << "Row 3: 0 -> 1 reflectivity of non-metal surface.";
		
	ofDrawBitmapStringHighlight(ss.str(), 30, 30);
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == '1' ) {
		cubeMapMode = 1;
	} else if( key == '2' ) {
		cubeMapMode = 2;
	} else if( key == '3' ) {
		cubeMapMode = 3;
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
