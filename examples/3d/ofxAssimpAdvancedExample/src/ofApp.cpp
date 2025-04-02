#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(false);
	ofBackground(0);
	
		
	float maxWizardPos = 1000.f;
	gui.setup("panel");
	gui.add(mBDebug.set("Debug", false));
	gui.add( wizardPos.set("WizardPos", glm::vec3(-215.f, 102.f, 165.f),
						   glm::vec3(-maxWizardPos,-maxWizardPos,-maxWizardPos),
						   glm::vec3(maxWizardPos,maxWizardPos,maxWizardPos) ) );
	gui.add(wizardHandOffset.set("WizardHandOffset", glm::vec3(10.0, 12, 38), glm::vec3(-200), glm::vec3(200)));
	gui.add(mBEnableCubeMap.set("EnableCubeMap", true));
	gui.add(cubeMapExposure.set("CubeMapExposure", 0.25, 0.0, 1.0));
	gui.add(mBDrawCubeMap.set("DrawCubeMap", false ));
	
	ofxAssimp::ImportSettings tsettings;
	tsettings.filePath = "ofLogoHollow.fbx";
	// we don't have any animations in this model
	tsettings.importAnimations = false;
	
	// lets make a logo material to replace the loaded material from assimp
	logoMaterial = make_shared<ofMaterial>();
	logoMaterial->setPBR(true);
	logoMaterial->setDiffuseColor( ofFloatColor(0.85, 0.16, 0.43, 1.0));
	
	if(logoModel.load(tsettings)) {
		for( unsigned int i = 0; i < logoModel.getNumMeshes(); i++ ) {
			auto mesh = logoModel.getMesh(i);
			if( mesh->material ) {
				mesh->material = logoMaterial;
			}
		}
	}
	
	tsettings.filePath = "Druid/druid.gltf";
	tsettings.importAnimations = true;
	tsettings.convertToLeftHanded = false;
	wizardModel.load(tsettings);
	wizardModel.setScale( 200.0 );
//	wizardModel.panDeg(180.f);
	cout << "Wizard --------------- " << endl;
	cout << wizardModel.getHierarchyAsString() << endl;
	for( unsigned int i = 0; i < wizardModel.getNumAnimations(); i++ ) {
		cout << i << " - " << wizardModel.getAnimation(i).getName() << endl;
	}
	wizardModel.setCurrentAnimation("Waiting");
	wizardModel.getCurrentAnimation().setLoopType(OF_LOOP_NORMAL);
	wizardModel.getCurrentAnimation().play();
	
	tsettings.filePath = "Fox/Fox_05.fbx";
	tsettings.importAnimations = true;
	tsettings.excludeNodesContainingStrings = {"_Goal"};
	tsettings.convertToLeftHanded = false;
	srcFoxScene = make_shared<ofxAssimp::Scene>();
	if( srcFoxScene->load(tsettings) ) {
		int numFoxes = 16;
		for( int i = 0; i < numFoxes; i++ ) {
			auto fox = make_shared<ofxAssimp::Scene>();
			if( fox->setup(srcFoxScene->getSrcScene())) {
				fox->addAnimation( 0, "idle", 2, 300, OF_LOOP_NORMAL );
				fox->addAnimation( 0, "idleBark", 310, 350 );
				fox->addAnimation( 0, "jump", 852, 890 );
				fox->setCurrentAnimation("idle");
				fox->getCurrentAnimation().play();
				fox->getCurrentAnimation().setPosition( (float)i / (float)numFoxes);
				// slightly different scales
				fox->setScale( ofRandom(0.9, 1.1) );
				// slightly different rotations around the y axis
				fox->panDeg( ofRandom(-10, 10));
				foxes.push_back(fox);
			}
		}
	}
	
	groundMesh = ofMesh::box( 1500, 40, 1500, 24, 2, 24 );
	for( auto& tc : groundMesh.getTexCoords() ) {
		tc.x *= 4.f;
		tc.y *= 4.f;
	}
	groundMaterial.setPBR(true);
	
	light.enable();
	light.setPointLight();
	light.setPosition(glm::vec3(-200, 500, 600));
	light.getShadow().setNearClip(20);
	light.getShadow().setFarClip(800);
	
	mShadowParams.setName("Shadow");
	mShadowParams.add( mShadowStrength.set("Strength", 1.0, 0.0, 1.0));
	gui.add(mShadowParams);
	
	ofShadow::enableAllShadows();
	// shadow bias is the margin of error the shadow depth
	// increasing the bias helps reduce shadow acne, but can cause light leaking
	// try to find a good balance that fits your needs
	// bias default is 0.005
	ofShadow::setAllShadowBias(0.05f);
	// normal bias default is 0
	// moves the bias along the normal of the mesh, helps reduce shadow acne
//	ofShadow::setAllShadowNormalBias(-2.f);
	
	ofShadow::setAllShadowTypes(OF_SHADOW_TYPE_PCF_LOW);
	
	cam.setNearClip(10);
	cam.setFarClip(5000);
	cam.setPosition( 0, 400, 1000 );
	cam.lookAt( glm::vec3(0,150,0) );
	
	// cube maps still have an effect on the final output even if an image is not loaded.
	// https://polyhaven.com/a/kloppenheim_06_puresky
	cubeMap.load( "kloppenheim_06_puresky_1k.hdr", 512 );
}

//--------------------------------------------------------------
void ofApp::update(){
	float deltaTime = ofClamp( ofGetLastFrameTime(), 1.f / 1000.0f, 1.f / 5.f );
	mTimeSinceStaffHitGround += deltaTime;
	
	wizardModel.setPosition(wizardPos);
	wizardModel.update();
		
	if( wizardModel.getCurrentAnimation().getName() == "PortalOpen" ) {
		if( wizardModel.getCurrentAnimation().getPosition() > 0.5f ) {
			if(!bFiredHitGroundEvent) {
				for( unsigned int i = 0; i < foxes.size(); i++ ) {
					if( foxes[i]->getCurrentAnimation().getName() != "jump" ) {
						foxes[i]->transitionCurrentAnimation("jump", 0.1);
						foxes[i]->getCurrentAnimation().setSpeed(ofRandom(0.8,1.2));
						foxes[i]->getCurrentAnimation().play();
					}
				}
				mTimeSinceStaffHitGround = 0.0f;
			}
			bFiredHitGroundEvent = true;
		}
		
		if(wizardModel.getCurrentAnimation().isDone()) {
			wizardModel.transitionCurrentAnimation("Waiting", 0.5f );
			wizardModel.getCurrentAnimation().play();
		}
	} else {
		bFiredHitGroundEvent = false;
	}
	if( mTimeSinceStaffHitGround < 1.0f ) {
		float logoStr = ofMap(mTimeSinceStaffHitGround, 0.1f, 1.f, 1.0f, 0.0, true );
		light.setDiffuseColor( ofFloatColor(1.f + logoStr*2.0f, 1.f + logoStr*2.0f));
	}
	
	light.getShadow().setStrength(mShadowStrength);
	cubeMap.setEnabled(mBEnableCubeMap);
	if( mBEnableCubeMap ) {
		cubeMap.setExposure(cubeMapExposure);
	}
	
	float spacing = 50.0f;
	float hw = (((float)foxes.size()-1.f) * spacing) / 2.f;
	float numFoxes = foxes.size();
	for( int i = 0; i < foxes.size(); i++ ) {
		auto& fox = foxes[i];
		float fpct = (float)i / (numFoxes-1.f);
		if( fox->getCurrentAnimation().getName() == "jump" && fox->getCurrentAnimation().isDone() ) {
			fox->transitionCurrentAnimation("idle", 0.5f);
			fox->getCurrentAnimation().play();
			fox->getCurrentAnimation().setPosition( ofRandom(0.0, 0.9));
		}
		fox->update();
		float fx = ofMap( (float)i, 0.0f, foxes.size()-1, -hw, hw );
		fox->setPosition( fx-80.0, 20.0, 320.0f + sinf(glm::pi<float>() * fpct) * 100.0f  );
	}
	
	auto wizardLeftHand = wizardModel.getNodeAsType<ofxAssimp::Bone>("*:hand right");
	if(wizardLeftHand) {
		auto lpos = wizardLeftHand->getGlobalOrientation() * wizardHandOffset.get();
		light.setPosition(wizardLeftHand->getGlobalPosition()+lpos);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	ofEnableDepthTest();
	
	// query the light to see if it has a depth pass
	if( light.shouldRenderShadowDepthPass() ) {
		int numShadowPasses = light.getNumShadowDepthPasses();
		for( int j = 0; j < numShadowPasses; j++ ) {
			light.beginShadowDepthPass(j);
			renderScene();
			light.endShadowDepthPass(j);
		}
	}
	
	
	cam.begin();
	if(mBDrawCubeMap) {
		cubeMap.draw();
	}
	renderScene();
	if(mBDebug) {
		light.draw();
		light.getShadow().drawFrustum();
	}
	cam.end();
	
	ofDisableDepthTest();
	
	if(bDrawGui) {
		ofSetColor(255);
		ofDrawBitmapString(ofToString(ofGetFrameRate(), 0), 500, 40 );
		gui.draw();
	} else {
		ofDrawBitmapString("H: Show gui.\nSpacebar/s: staff.", 40, 40 );
	}
}

//--------------------------------------------------------------
void ofApp::renderScene() {
	ofSetColor(255);
	logoModel.draw();
	ofSetColor(255);
	wizardModel.draw();
	
	for( auto& fox : foxes ) {
		fox->draw();
	}
	
	groundMaterial.begin();
	groundMesh.draw();
	groundMaterial.end();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == ' ' || key == 's' ) {
		if( wizardModel.getCurrentAnimation().getName() != "PortalOpen") {
			wizardModel.transitionCurrentAnimation("PortalOpen", 0.2f);
			wizardModel.getCurrentAnimation().play();
		}
	}
	if( key == 'H') {
		bDrawGui = !bDrawGui;
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
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

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
