#include "ofApp.h"
#if defined(TARGET_EMSCRIPTEN)
#include "ofxAppEmscriptenWindow.h"
#endif

//--------------------------------------------------------------
void ofApp::setup(){
//    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(50);
    loadModel("Fox/Fox_05.fbx");
	
	windowResized(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::loadModel(string filename){
	
	ofx::assimp::ImportSettings tsettings;
	tsettings.filePath = filename;
//	tsettings.importTextures = false;
//	tsettings.importAnimations = false;
	tsettings.excludeNodesContainingStrings = {"_Goal"};
    
	// aiProcess_FixInfacingNormals
	if( model.load(tsettings)) {
        if( model.hasAnimations() ){
			model.getCurrentAnimation().setLoopType( OF_LOOP_NORMAL );
			model.getCurrentAnimation().play();
			
        }
		model.centerAndScaleToWindow();
		mesh.clear();
		cout << endl << endl << endl << endl;
		stringstream ss;
		ss << "Num Meshes: " << model.getNumMeshes() << endl;
		ss << "Num Skeletons: " << model.getNumSkeletons() << endl;
		ss << "Num Bones: " << model.getNumBones() << endl;
		ss << "Num Vertices: " << model.getNumVertices() << endl;
		ss << endl;
		mSceneString = ss.str();
		mSceneString += model.getHierarchyAsString();
		cout << mSceneString << endl;
		
		auto neckBone = model.getNodeAsType<ofx::assimp::Bone>("*:neck_control");
		if( neckBone ) {
			mLocalQuat = neckBone->getOrientationQuat();
		}
		
		model.addAnimation( 0, "idle", 2, 300, OF_LOOP_NORMAL );
		model.addAnimation( 0, "idleBark", 310, 350 );
		model.addAnimation( 0, "walk", 390, 410, OF_LOOP_NORMAL );
		model.addAnimation( 0, "scratch", 740, 820 );
		model.addAnimation( 0, "eat", 822, 850 );
		model.addAnimation( 0, "jump", 852, 890 );
		
		model.getAnimation("eat").setLoopType( OF_LOOP_NORMAL );
		
		auto tailBone = model.getNodeAsType<ofx::assimp::Bone>("*:tail_top");
		if( tailBone ) {
//			tailBone->setAnimationsEnabled(false);
		}
		
//		cout << "NUMBER OF SKELETONS: " << model.getNumSkeletons() << endl;
//		if( model.hasSkeletons() ) {
//			auto skel = model.getSkeleton(0);
//			cout << skel->getAsString() << endl;
//			cout << endl << endl;
//		}
    } else {
        ofLogError() << " can't load model: " << filename << endl;
    }
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	light.setPosition(model.getPosition() + glm::vec3(-300, 300, 1200));
	
	
	model.earlyUpdate();
	
	
	float deltaTime = ofClamp(ofGetLastFrameTime(), 1.f/1000.f, 1.f/5.f);
	
	if(bAutoRotate) {
		model.panDeg( deltaTime * 30.0f );
	}
	
	if( bFollowTarget ) {
		lookStrength += deltaTime * 2.0f;
	} else {
		lookStrength -= deltaTime;
	}
	lookStrength = ofClamp( lookStrength, 0, 1);
	
	mLookAtPos = model.getLookAtDir();
	mLookAtPos.y = 0.f;
	
	mLookAtPos *= 400.0f;
	mLookAtPos += model.getPosition();
//	mLookAtPos += model.getSideDir() * sinf(ofGetElapsedTimef()) * 400.0f;
	mLookAtPos += model.getSideDir() * ofMap( ofGetMouseX(), mViewRect.x, mViewRect.getRight(), -1.f, 1.f, true) * 400.0f;
	mLookAtPos.y +=  ofMap( ofGetMouseY(), mViewRect.y, mViewRect.getBottom(), 1.0f, 0.0, true ) * 400.0f;
	
	if( lookStrength > 0.0f ) {
		// try to grab the neck bone
		auto neckBone = model.getNodeAsType<ofx::assimp::Bone>("*:neck_control");
		if( neckBone ) {
			
			glm::vec3 relPosition = neckBone->getGlobalPosition() - mLookAtPos;
			auto radius = glm::length(relPosition);
			// pulled from ofNode so we can slerp it up
			float latitude = acos(relPosition.y / radius) - glm::half_pi<float>();
			float longitude = atan2(relPosition.x , relPosition.z);
			glm::quat dq = glm::angleAxis(0.f, glm::vec3(0,0,1)) * glm::angleAxis(longitude, glm::vec3(0,1,0)) * glm::angleAxis(latitude, glm::vec3(1,0,0));
			auto animQ = neckBone->getGlobalOrientation();
			
			neckBone->setGlobalOrientation( glm::slerp(animQ, dq, powf(lookStrength, 1.5f)) );
		}
		
		
	}
	// if the scene does not load animations, but we move bones, tell the scene something has been updated
	model.flagSceneDirty();
	
	
	model.lateUpdate();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	
	ofEnableDepthTest();
	
	cam.begin(mViewRect);
	
	ofEnableLighting();
	light.enable();
	ofEnableSeparateSpecularLight();
	
	if( bDrawBones ) {
		model.drawBones();
	}
	
	if( bDrawMeshes) {
		if( bDrawWireframe ) {
			model.drawWireframe();
		} else {
			model.drawFaces();
		}
	}
	
	if( bFollowTarget) {
		ofSetColor( ofColor::red );
		ofDrawSphere(mLookAtPos, 10);
	}
	
	
	ofPushMatrix(); {
		ofTranslate( 300, 0, 0);
		mesh.draw();
	} ofPopMatrix();
	
	light.disable();
	ofDisableLighting();
	ofDisableSeparateSpecularLight();
	
	
	ofDrawSphere(light.getPosition(), 10);
	
	
	ofPushMatrix(); {
		ofTranslate(model.getPosition());
		ofRotateYDeg(90);
		ofRotateZDeg(90);
		ofSetColor(100);
		ofDrawGridPlane(24, 24);
	} ofPopMatrix();
	
	cam.end();
	ofDisableDepthTest();
	
	ofSetColor( 30 );
	ofDrawRectangle( mTextRect );
	ofSetColor( 225 );
	ofDrawBitmapString(mSceneString, mTextRect.x + 12, mTextRect.y + 24 );
	
	if(bHelpText){
		ofSetColor(255, 255, 255 );
		string str;
		str += "(h): help.\n\n";
		str += "FPS: " + ofToString(ofGetFrameRate(),0) + "\n\n";
		str +="(Spacebar): play/pause.\n";
		str += "(m): bDrawMeshes.\n";
		str += "(w): wireframe.\n";
		str += "(b): draw bones - "+ofToString(model.getNumBones(),0)+".\n";
		str += "(t): follow target.\n";
		str += "(f): toggle fullscreen: " + ofToString(ofGetWidth(),0)+" x "+ofToString(ofGetHeight(),0)+"\n";
		
		if( model.hasAnimations() ) {
			
			stringstream ss;
			ss << endl << endl;
			ss << "(up/down): Animations "<< endl << "------------------" << endl;
			for( int i = 0; i < model.getNumAnimations(); i++ ) {
				if( i > 0 ) {
					ss << endl;
				}
				if( i == model.getCurrentAnimationIndex() ) {
					ss << "-->";
				} else {
					ss << "   ";
				}
				ss <<i<<". ";
				ss << model.getAnimation(i).getName() << " " << ofToString(model.getAnimation(i).getDuration(), 2);
			}
			str += ss.str();
		}
		ofDrawBitmapString(str, 20, 20);
	} else {
		ofDrawBitmapString("(h): help.", 20, 20);
	}
	
	if( model.hasAnimations() ) {
		stringstream ss;
		ofSetColor( 30 );
		ofDrawRectangle( mTimelineRect );
		ofSetColor(200);
		
		
		auto posSecs = ofToString(model.getCurrentAnimation().getPositionInSeconds(), 2 );
		
		float timeWidth = ((float)mTimelineRect.getWidth()-40);
		float timePadding = 20;
		float tx = timePadding;
		
		ss.str("");
		ss << (model.getCurrentAnimationIndex()+1) << "/" << model.getNumAnimations() << " - " << model.getCurrentAnimation().getName() << " - " << (model.getCurrentAnimation().isPlaying() ? "playing" : "paused") << ". Cycle with (up/down).";
		float swidth = ss.str().length() * 8;
		ofDrawBitmapString(ss.str(), timePadding, mTimelineRect.y + 16 );
		
		ss.str("");
		ss << posSecs << " / " << ofToString(model.getCurrentAnimation().getDuration(), 2) << " seconds.";
		swidth = ss.str().length() * 8;
		ofDrawBitmapString(ss.str(),  mTimelineRect.getRight() - swidth - 16, mTimelineRect.y + 16 );
		
		tx = timePadding + timeWidth;
		ofSetColor( 100 );
		ofDrawRectangle(timePadding, mTimelineRect.getBottom()-10, tx-timePadding, 4);
		
		ofDrawRectangle( tx, mTimelineRect.getBottom()-24, 1, 24 );
		string sframe = ofToString(model.getCurrentAnimation().getTotalNumFrames(),0);
		swidth = sframe.length() * 8;
		ofDrawBitmapString( sframe, tx-swidth-4, mTimelineRect.getBottom()-14 );
		
		
		
		ofDrawRectangle( timePadding, mTimelineRect.getBottom()-24, 1, 24 );
		ofDrawBitmapString( "0", 24, mTimelineRect.getBottom()-14 );
		
		
		
		ofSetColor(220);
		tx = timeWidth * model.getCurrentAnimation().getPosition();
		ofDrawRectangle(timePadding, mTimelineRect.getBottom()-10, tx, 4);
		
		ofDrawRectangle( timePadding + tx, mTimelineRect.getBottom()-24, 1, 24 );
		sframe = ofToString(model.getCurrentAnimation().getCurrentFrame(), 0);
		swidth = sframe.length() * 8;
		ofDrawBitmapString( sframe, tx-swidth+16, mTimelineRect.getBottom()-24 );
	}
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':
            if( model.hasAnimations() ){
				if( model.getCurrentAnimation().isPlaying() ) {
					model.getCurrentAnimation().stop();
				} else {
					model.getCurrentAnimation().play();
				}
            }
			
            break;
        case 'h':
            bHelpText = !bHelpText;
            break;
        case OF_KEY_DOWN:
            if( model.hasAnimations() ){
				auto cindex = model.getCurrentAnimationIndex();
				cindex++;
				cindex %= model.getNumAnimations();
				if( model.getNumAnimations() > 1 ) {
					model.transitionCurrentAnimation((int)cindex, 0.5f);
				} else {
					model.setCurrentAnimation(cindex);
				}
				model.getCurrentAnimation().play();
            }
            break;
		case OF_KEY_UP:
			if( model.hasAnimations() ){
				auto cindex = (int)model.getCurrentAnimationIndex();
				cindex--;
				if( cindex < 0 ) {
					cindex = model.getNumAnimations()-1;
				}
				if( model.getNumAnimations() > 1 ) {
					model.transitionCurrentAnimation( cindex, 0.5f);
				} else {
					model.setCurrentAnimation(cindex);
				}
				model.getCurrentAnimation().play();
			}
			break;
		case 'f':
			ofToggleFullscreen();
			break;
		case 'b':
			bDrawBones=!bDrawBones;
			break;
		case 'w':
			bDrawWireframe=!bDrawWireframe;
			break;
		case 'm':
			bDrawMeshes=!bDrawMeshes;
			break;
		case 't':
			bFollowTarget=!bFollowTarget;
			break;
		case 'r':
			bAutoRotate=!bAutoRotate;
        default:
            break;
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
//	ofLogNotice("ofApp :: windowResized : ") << w << " x " << h;
	mTimelineRect.height = 64;
	mTimelineRect.y = ofGetHeight()-mTimelineRect.height;
	mViewRect = ofRectangle( 0, 0, std::min(ofGetWidth()-320, 900), ofGetHeight()-mTimelineRect.height );
	mTimelineRect.width = mViewRect.width;
	mTextRect = ofRectangle( mViewRect.getRight(), 0, ofGetWidth() - mViewRect.getWidth(), ofGetHeight() );
	
	cam.setControlArea(mViewRect);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

