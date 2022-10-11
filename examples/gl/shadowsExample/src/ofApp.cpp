#include "ofApp.h"
#include "ofGLProgrammableRenderer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// applies lighting to everything, regardless of shadow or facing lights
	ofSetGlobalAmbientColor(ofColor(140));
	
	// add two lights
	int numLights = 2;
	for( int i = 0; i < numLights; i++ ) {
		auto light = make_shared<ofLight>();
		light->setup();
		if( i == 0 ) {
			light->setPointLight();
		} else {
			// set the light to be a spot light with a cutoff (cone/fov) of 30 degrees
			// and a concentration (softness) of 50 out of a range 0 - 128
			light->setSpotlight(30, 50);
			light->getShadow().setNearClip(100);
			light->getShadow().setFarClip(2000);
		}
		
		if( light->getType() == OF_LIGHT_DIRECTIONAL || light->getType() == OF_LIGHT_SPOT ){
			glm::quat xq = glm::angleAxis(glm::radians(30.0f), glm::vec3(1,0,0));
			glm::quat yq = glm::angleAxis(glm::radians(200.0f), glm::vec3(0,1,0));
			light->setOrientation(yq*xq);
			// unsure why this is reversed
			if(light->getType() == OF_LIGHT_SPOT) {
				light->setOrientation( light->getOrientationQuat() * glm::angleAxis(ofDegToRad(180), glm::vec3(0,1,0) ) );
			}
		}
		
		if( !ofIsGLProgrammableRenderer() ) {
			light->enable();
		}
		
		lights.push_back(light);
	}
	// shadows are disabled by default
	// call this function to enable all of them that are attached to lights
	ofShadow::enableAllShadows();
	// shadow bias is the margin of error the shadow depth
	// increasing the bias helps reduce shadow acne, but can cause light leaking
	// try to find a good balance that fits your needs
	// bias default is 0.005
	//ofShadow::setAllShadowBias(0.006);
	// normal bias default is 0
	// moves the bias along the normal of the mesh, helps reduce shadow acne
	ofShadow::setAllShadowNormalBias(-4.f);
	ofShadow::setAllShadowDepthResolutions( 1024, 1024 );
	shadowType = OF_SHADOW_TYPE_PCF_LOW;
	ofShadow::setAllShadowTypes(shadowType);
	// point light shadow depth maps require OF_PRIMITIVE_TRIANGLES
	// so we grab some meshes here for drawing and make sure they use OF_PRIMITIVE_TRIANGLES
	// we also use a higher resolution for the meshes than the default
	sphereMesh = ofMesh::sphere(1.0, 48, OF_PRIMITIVE_TRIANGLES);
	boxMesh = ofMesh::box(1, 1, 1, 24, 24, 24);
	cylinderMesh = ofMesh::cylinder(0.4, 1.0, 48, 12, 4, true, OF_PRIMITIVE_TRIANGLES );
	
	// shadows and lights work with materials
	// create some different materials so the lighting and shadows can be applied
	boxMaterial.setDiffuseColor( ofFloatColor(0.25) );
	boxMaterial.setShininess(60);
	boxMaterial.setSpecularColor(ofFloatColor(1));
	
	bgMaterial.setDiffuseColor( ofFloatColor(0.5) );
	bgMaterial.setShininess(1);
	bgMaterial.setSpecularColor(ofFloatColor(1));
	
	sphereMaterial.setAmbientColor(ofFloatColor(0.85, 0.16, 0.43, 0.4));
	sphereMaterial.setDiffuseColor(ofFloatColor(0.85, 0.16, 0.43, 1.0));
	sphereMaterial.setSpecularColor(ofFloatColor(1.0,1.0,1.0,1.0));
	sphereMaterial.setShininess(100);
	
}

//--------------------------------------------------------------
void ofApp::update(){
	float deltaTime = ofClamp(ofGetLastFrameTime(), 1.f/5000.f, 1.f/5.f);
	float etimef = ofGetElapsedTimef();
	
	if( lights.size() > 0 ){
		lights[0]->setPosition( cosf(etimef * 0.6) * 220.0, 230.0, sinf(etimef * 0.2) * 100 );
	}
	if( lights.size() > 1 ){
		lights[1]->setPosition( 210, 430.0, 750 );
	}
	
	colorHue += deltaTime * 2.0;
	if(colorHue >= 255){
		colorHue = 0.f;
	}
	lightColor.setHue(colorHue);
	
	for( int i = 0; i < (int)lights.size(); i++ ){
		auto& light = lights[i];
		lightColor.setHsb(ofWrap(colorHue + (float)i * 29.5, 0, 255.f), 200, 100 );
		light->setDiffuseColor(lightColor);
		lightColor.setBrightness(220);
		light->setSpecularColor(lightColor);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofEnableDepthTest();
	
	for( int i = 0; i < lights.size(); i++ ) {
		auto& light = lights[i];
		// query the light to see if it has a depth pass
		if( light->shouldRenderShadowDepthPass() ) {
			// Get the number of passes required.
			// By default the number of passes is 1. And we could just call beginShadowDepthPass() or beginShadowDepthPass(0);
			// It will be more than one pass if it is a pointlight with setSingleOmniPass set to false
			// or a platform that does not support geometry shaders.
			// Most likely it will be a single pass, but we get the number of passes to be safe.
			int numShadowPasses = light->getNumShadowDepthPasses();
			for( int j = 0; j < numShadowPasses; j++ ) {
				light->beginShadowDepthPass(j);
				// By default, shadows have the following gl culling enabled by default
				// this helps reduce z fighting by only rendering the rear facing triangles to the depth map
				// enables face culling
				//glEnable(GL_CULL_FACE);
				// sets the gl triangle winding order, default for ofShadow is GL_CW
				//glFrontFace(mGlFrontFaceWindingOrder);
				// tells OpenGL to cull front faces
				//glCullFace(GL_FRONT);
				
				// the culling can be disabled by calling
				//light->getShadow().setGlCullingEnabled(false);
				// or the culling winding order can be changed by calling
				//light->getShadow().setFrontFaceWindingOrder(GL_CCW); // default is GL_CW
				renderScene();
				light->endShadowDepthPass(j);
			}
		}
	}
	
	camera.begin();
	
	if( !ofIsGLProgrammableRenderer() ) {
		ofEnableLighting();
	}
	
	// CULL the back faces of the geometry for rendering
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	renderScene();
	glDisable(GL_CULL_FACE);
	
	if( !ofIsGLProgrammableRenderer() ) {
		ofDisableLighting();
	}

	
	for( int i = 0; i < lights.size(); i++ ){
		auto& light = lights[i];
		if( !light->getShadow().getIsEnabled() ){
			continue;
		}
		ofSetColor(light->getDiffuseColor());
		light->draw();
		
		if( light->getType() == OF_LIGHT_DIRECTIONAL ){
			ofSetColor( ofColor::orchid );
			light->transformGL(); {
				ofDrawArrow( glm::vec3(0,0,0), glm::vec3(0,0,100), 10 );
			} light->restoreTransformGL();
		}
		
		if( light->getIsEnabled() && light->getShadow().getIsEnabled() && bDrawFrustums ){
			light->getShadow().drawFrustum();
		}
	}

	camera.end();
	ofDisableDepthTest();
	
	
	stringstream ss;
	
	if( !ofIsGLProgrammableRenderer() ) {
		ss << endl << "SHADOWS ONLY WORK WITH PROGRAMMABLE RENDERER!" << endl;
	} else {
		ss << "Shadows enabled (spacebar): " << bEnableShadows;
		ss << endl << "Draw frustums (f): " << bDrawFrustums;
		ss << endl << "Shadow Type (right): " << ofShadow::getShadowTypeAsString(shadowType);
		ss << endl << "Sample Radius (up / down): " << shadowSampleRadius;
		if( ofGetGLRenderer() && ofGetGLRenderer()->getGLVersionMajor() < 4 ) {
			ss << endl << "Point light shadows only work with OpenGL 4+" << endl;
		}
	}
	
	
	ofDrawBitmapStringHighlight(ss.str(), 20, 20 );
	
	ofSetColor(255);
	ofEnableAlphaBlending();
	
}
// create a renderScene() function so the same drawing can happen in both ofApp::draw()
// and inside the ofLight::beginShadowDepthPass()
//--------------------------------------------------------------
void ofApp::renderScene() {
	float etimef = ofGetElapsedTimef();
	ofSetColor(200, 50, 120 );
	sphereMaterial.begin();
	ofPushMatrix();
	ofTranslate(-250, sinf(etimef*0.7)*250, 0);
	ofRotateZDeg(90*sin(etimef));
	ofRotateYDeg(90*cos(etimef*0.7));
	ofScale( 100, 100, 100 );
	sphereMesh.draw();
	sphereMaterial.end();
	ofSetColor( 255 );
	// ofDrawSphere() is triangle strip!!!!
	// it will cause issues with rendering of point light depth maps
	//	ofDrawSphere(0, 0, 0, 100 );
	ofPopMatrix();
	
	boxMaterial.begin();
	ofDrawBox(cosf(etimef) * 200., 0, sinf(etimef*0.452) * 100, 100 );
	ofPushMatrix(); {
		ofTranslate(250, cosf(etimef*0.6)*50-80, 200 );
		ofRotateZDeg(ofWrapDegrees((etimef*0.04) * 360));
		ofRotateXDeg(ofWrapDegrees((etimef*0.06) * 360));
		ofScale(100, 100, 100.0);
		boxMesh.draw();
	} ofPopMatrix();
	
	boxMaterial.end();
	
	ofSetColor( 200 );
	
	bgMaterial.begin();
	ofPushMatrix(); {
		ofTranslate(0, -250, 0);
		ofScale(1000, 50, 1000);
		boxMesh.draw();
	} ofPopMatrix();
	
	ofPushMatrix(); {
		ofTranslate(500, 0, 0);
		ofScale(50, 1000, 1000);
		boxMesh.draw();
	} ofPopMatrix();
	
	
	ofPushMatrix(); {
		ofTranslate(-500, 0, 0);
		ofScale(50, 1000, 1000);
		boxMesh.draw();
	} ofPopMatrix();
	
	ofPushMatrix(); {
		ofTranslate(0, 0, -500);
		ofScale(1000, 1000, 50);
		boxMesh.draw();
	} ofPopMatrix();
	
	ofPushMatrix(); {
		ofTranslate(0, 500, 0);
		ofScale(1000, 50, 1000);
		boxMesh.draw();
	} ofPopMatrix();
	
	bgMaterial.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == ' ' ) {
		bEnableShadows = !bEnableShadows;
		if( bEnableShadows ) {
			ofShadow::enableAllShadows();
		} else {
			ofShadow::disableAllShadows();
		}
	}
	if( key == 'f' ) {
		bDrawFrustums = !bDrawFrustums;
	}
	if( key == OF_KEY_RIGHT ) {
		int stype = (int)shadowType+1;
		if( stype == OF_SHADOW_TYPE_TOTAL ) {
			stype = 0;
		}
		shadowType = (ofShadowType)stype;
		ofShadow::setAllShadowTypes(shadowType);
	}
	if( key == OF_KEY_UP ) {
		shadowSampleRadius += 1.f;
		ofShadow::setAllShadowSampleRadius(shadowSampleRadius);
	}
	if( key == OF_KEY_DOWN ) {
		shadowSampleRadius -= 1.f;
		if( shadowSampleRadius < 0 ) {
			shadowSampleRadius = 0.f;
		}
		ofShadow::setAllShadowSampleRadius(shadowSampleRadius);
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
