#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);
	
	ofDisableArbTex();
	
	light.setDirectional();
	light.enable();
	light.setPosition(100.1, 400, 600 );
	light.lookAt(glm::vec3(0,0,0));
	light.getShadow().setEnabled(true);
	light.getShadow().setGlCullingEnabled(true);
	light.getShadow().setDirectionalBounds(2000, 1000);
	light.getShadow().setNearClip(200);
	light.getShadow().setFarClip(2000);
	light.getShadow().setStrength(1.0);
	// increase alpha value to increase strength of light
	light.setDiffuseColor(ofFloatColor(1.0, 2.0));
	light.getShadow().setShadowType(OF_SHADOW_TYPE_PCF_LOW);
	
	meshLogoHollow.load("ofLogoHollow.ply");
	meshLogoHollow.mergeDuplicateVertices();
	// we need to flip the normals for this mesh //
	for( size_t i = 0; i < meshLogoHollow.getNumNormals(); i++ ) {
		meshLogoHollow.getNormals()[i] *= -1.f;
	}

	// going to add a custom uniform, passing elapsed time to use for animation
	matLogo.setCustomUniform1f("iElapsedTime", 1.0);
//	// set the material to PBR, default if phong
	matLogo.setPBR(true);
	matFloor.setPBR(true);
	
	// try commenting this out to see the effect that cube maps have on lighting
	// https://polyhaven.com/a/kloppenheim_06_puresky
	cubeMap.load( "kloppenheim_06_puresky_1k.exr", 512 );
	
	matPlywood.setPBR(true);
	matPlywood.loadTexture(OF_MATERIAL_TEXTURE_DIFFUSE, "plywood/plywood_diff_2k.jpg" );
	matPlywood.loadTexture(OF_MATERIAL_TEXTURE_NORMAL, "plywood/plywood_nor_gl_2k.png" );
	matPlywood.loadTexture(OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC, "plywood/plywood_arm_2k.jpg" );
	matPlywood.setTexCoordScale(1.0, 0.3);
	matPlywood.setClearCoatEnabled(true);
	matPlywood.setClearCoatStrength(1.0);
	matPlywood.setClearCoatRoughness(0.0);
	
	// higher resolution sphere
	meshPlySphere = ofMesh::icosphere(1.0, 5);
	
	matSphere.setPBR(true);
	matSphere.setDiffuseColor(ofFloatColor(0.25,1.0));
	matSphere.setMetallic(1.0);
	matSphere.setRoughness(0.05);
	
	reloadShader();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableDepthTest();
	
	
	if( light.shouldRenderShadowDepthPass() ) {
		int numShadowPasses = light.getNumShadowDepthPasses();
		for( int j = 0; j < numShadowPasses; j++ ) {
			light.beginShadowDepthPass(j);
			renderScene();
			light.endShadowDepthPass(j);
		}
	}
	
	
	camera.begin(); {
		
		renderScene();
		
		 if( cubeMap.hasPrefilteredMap() ) {
		 	cubeMap.drawPrefilteredCube(0.2f);
		 }
		
		if(bDebug) {
			light.draw();
			
			if( light.getIsEnabled() && light.getShadow().getIsEnabled() ) {
				light.getShadow().drawFrustum();
			}
		}
		
	} camera.end();
	
	ofDisableDepthTest();
	
	ofSetColor(255);
	ofEnableAlphaBlending();
#ifdef USE_LIGHT
	for( auto& lp : lights ) {
		lp->gui.draw();
	}
#endif
	
	stringstream ss;
	ss << "Reload shader(r): make changes to shader in data/shaders/main.frag and then press 'r' to see changes.";
	ofDrawBitmapStringHighlight(ss.str(), 40, 40);
}

//--------------------------------------------------------------
void ofApp::renderScene() {
	
	matFloor.setMetallic(0.0);
	matFloor.setReflectance(0.01);
	matFloor.setRoughness(0.8);
	matFloor.setDiffuseColor(ofFloatColor(0.97, 0.96, 0.91, 1.0) );
	
	matFloor.begin();
	ofDrawBox( 0, -275, 0 ,3200, 50, 2200 );
	matFloor.end();
	
	matPlywood.begin();
	ofPushMatrix();
	float angle = ofGetElapsedTimef();
	ofTranslate( 700.0*cos(angle), -20, sin(angle)*300.0-100 );
	ofScale(120);
	meshPlySphere.draw();
	ofPopMatrix();
	matPlywood.end();
	
	matSphere.begin();
	ofPushMatrix();
	ofTranslate( 700.0*cos(angle-PI), -20, sin(angle-PI)*300.0-100 );
	ofScale(120);
	meshPlySphere.draw();
	ofPopMatrix();
	matSphere.end();
	
	matLogo.setCustomUniform1f("iElapsedTime", ofGetElapsedTimef());
	matLogo.begin();
	ofPushMatrix();
	ofTranslate( -70, -250, 0 );
	ofRotateXDeg(-90);
	ofScale(100);
	meshLogoHollow.draw();
	ofPopMatrix();
	matLogo.end();
}

//--------------------------------------------------------------
bool ofApp::reloadShader() {
	// load the shader main functions //
	string vname = "shaders/main.vert";
	ofBuffer vbuffer = ofBufferFromFile(vname);
	string fname = "shaders/main.frag";
	ofBuffer fbuffer = ofBufferFromFile(fname);
	if( vbuffer.size() && fbuffer.size() ) {
		matLogo.setShaderMain(vbuffer.getText(), GL_VERTEX_SHADER, "main.vert");
		matLogo.setShaderMain(fbuffer.getText(), GL_FRAGMENT_SHADER, "main.frag");
		return true;
	}
	return false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == 'r' ) {
		reloadShader();
	}
	if( key == 'd' ) {
		bDebug = !bDebug;
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
