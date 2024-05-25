#include "ofApp.h"
#if defined(TARGET_EMSCRIPTEN)
#include "ofxAppEmscriptenWindow.h"
#endif

//--------------------------------------------------------------
void ofApp::setup(){
	
	gui.setup("panel");
	gui.add( bDebug.set("Debug", false ));
	gui.add( tiltDeg.set( "Tilt", 0., -45, 45 ));
	gui.add( panDeg.set( "Pan", 0., -90, 90 ));
	gui.add( cameraOffsetZ.set("CameraOffsetZ", 16.3, 0.0, 20));
	gui.add( bottomOffsetY.set( "BottomOffsetY", -2.6, -20, 20 ));
	gui.add( bottomOffsetZ.set( "BottomOffsetZ", -15.25, -20, 20 ));
	gui.add( shadowStrength.set("ShadowStrength", 1.3, 0.0, 10.0));
	
	gui.add( shadowBias.set("ShadowBias", 0.025, 0.001, 0.1 ));
	gui.add( shadowNormalBias.set("ShadowNormalBias", 0.5, -3, 3));
	
//    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(50);
	// https://polyhaven.com/a/security_camera_02
	loadModel("securityCamera.fbx");
	light.enable();
	light.setScale( 1.f );
//	light.setSpotlight( 30, 20 );
	light.setDirectional();
	light.getShadow().setDirectionalBounds( 1500, 1500 );
	light.getShadow().setNearClip(200);
	light.getShadow().setFarClip(2000);
	glm::quat xq = glm::angleAxis(glm::radians(-50.0f), glm::vec3(1,0,0));
	glm::quat yq = glm::angleAxis(glm::radians(-20.0f), glm::vec3(0,1,0));
	light.setOrientation(yq*xq);
	light.setPosition(glm::vec3(-200, 500, 600));
	light.getShadow().setStrength(0.6f);
	
	ofShadow::enableAllShadows();
	// shadow bias is the margin of error the shadow depth
	// increasing the bias helps reduce shadow acne, but can cause light leaking
	// try to find a good balance that fits your needs
	// bias default is 0.005
	ofShadow::setAllShadowBias(0.07);
	// normal bias default is 0
	// moves the bias along the normal of the mesh, helps reduce shadow acne
	ofShadow::setAllShadowNormalBias(-2.f);
	
	ofShadow::setAllShadowTypes(OF_SHADOW_TYPE_PCF_LOW);
	
	windowResized(ofGetWidth(), ofGetHeight());
	cam.setFarClip(5000);
	cam.setPosition( 0, 0, 1200 );
	
	// box(float width, float height, float depth, int resX=2, int resY=2, int resZ=2)
	wallMesh = ofMesh::box( 2500, 2500, 40, 24, 24, 4 );
	
	// https://polyhaven.com/a/painted_concrete
	wallMaterial.loadTexture( OF_MATERIAL_TEXTURE_DIFFUSE, "wall/painted_concrete_diff_1k.jpg", true, false );
	wallMaterial.loadTexture( OF_MATERIAL_TEXTURE_DISPLACEMENT, "wall/painted_concrete_disp_1k.png", true, false );
	wallMaterial.loadTexture( OF_MATERIAL_TEXTURE_NORMAL, "wall/painted_concrete_nor_gl_1k.png", true, false );
	wallMaterial.loadTexture( OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC, "wall/painted_concrete_arm_1k.jpg", true, false );
	wallMaterial.setDisplacementStrength( 2.0 );
	
	// https://polyhaven.com/a/abandoned_construction
	cubeMap.load("abandoned_construction_2k.hdr", 512);
}

//--------------------------------------------------------------
void ofApp::loadModel(string filename){
	
	ofx::assimp::ImportSettings tsettings;
	tsettings.filePath = filename;
	// we don't have any animations in this model
	tsettings.importAnimations = false;
	// when converted to left hand coordinate system,
	// the z-axis is flipped and the text on the camera body appears backwards
	tsettings.convertToLeftHanded = false;
    
	if( model.load(tsettings)) {
		model.centerAndScaleToWindow();
		cout << endl << endl << endl << endl;
		stringstream ss;
		ss << "Num Meshes: " << model.getNumMeshes() << endl;
		ss << "Num Vertices: " << model.getNumVertices() << endl;
		ss << endl;
		mSceneString = ss.str();
		mSceneString += model.getHierarchyAsString();
		cout << mSceneString << endl;
		
    } else {
        ofLogError() << " can't load model: " << filename << endl;
    }
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	float deltaTime = ofClamp( ofGetLastFrameTime(), 1.f / 1000.0f, 1.f / 5.f);
	
	if( panDirection != 0.f ) {
		panDeg += deltaTime * 60.0f * panDirection;
		panDeg = ofClamp( panDeg, panDeg.getMin(), panDeg.getMax() );
	}
	
	if( tiltDirection != 0.f ) {
		tiltDeg += deltaTime * 60.0f * tiltDirection;
		tiltDeg = ofClamp( tiltDeg, tiltDeg.getMin(), tiltDeg.getMax() );
	}
	
	// move around the meshes
	light.getShadow().setStrength(shadowStrength);
	
	ofShadow::setAllShadowBias(shadowBias);
	ofShadow::setAllShadowNormalBias(shadowNormalBias);
	
	
	model.calculateDimensions();

	auto meshMountNode = model.getNode("security_camera_02_mount", true);
	if( meshMountNode ) {
		auto tpos = meshMountNode->getPosition();
		meshMountNode->setPosition( tpos.x, bottomOffsetY, bottomOffsetZ+cameraOffsetZ );
	}
	// Notice that we pass true for the second argument which strictly searches for the string
	// because the string security_camera_02 is also contained in security_camera_02_mount
	auto meshCameraNode = model.getNode("security_camera_02", true);
	if( meshCameraNode ) {
		auto tpos = meshCameraNode->getPosition();
		meshCameraNode->setPosition( tpos.x, tpos.y, cameraOffsetZ );
		meshCameraNode->setOrientation({0.0f, 0.f, 0.f});
		meshCameraNode->panDeg( panDeg );
		meshCameraNode->tiltDeg( tiltDeg );
	}
	
	if( model.getNumMeshes() > 1 ) {
		auto camMesh = model.getMesh(1);
		auto camBounds = camMesh->getLocalBounds();
		// local bounds do not have transforms applied
		// this will provide us with a more accurate size of the mesh
		// so we need to scale it to the global size
		camBounds *= camMesh->getGlobalScale();
		auto nCamPos = camMesh->getYAxis() * camBounds.getHeight() * 0.52f + camMesh->getZAxis() * (camBounds.getDepth() * 0.45f);
		
		auto camMeshQ = camMesh->getGlobalOrientation();
		nCamPos = camMeshQ * nCamPos;
		camOnSecurityCam.setPosition( nCamPos + camMesh->getGlobalPosition() );
		camOnSecurityCam.setOrientation( camMesh->getGlobalOrientation() );
		
		// we need to rotate the ofCamera in the opposite direction //
		camOnSecurityCam.panDeg(180);
	}
		
	 
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	
	ofEnableDepthTest();
	

	// query the light to see if it has a depth pass
	if( light.shouldRenderShadowDepthPass() ) {
//		// Get the number of passes required.
//		// By default the number of passes is 1. And we could just call beginShadowDepthPass() or beginShadowDepthPass(0);
//		// It will be more than one pass if it is a pointlight with setSingleOmniPass set to false
//		// or a platform that does not support geometry shaders.
//		// Most likely it will be a single pass, but we get the number of passes to be safe.
		int numShadowPasses = light.getNumShadowDepthPasses();
		for( int j = 0; j < numShadowPasses; j++ ) {
			light.beginShadowDepthPass(j);
//			// Shadows have the following gl culling enabled by default
//			// this helps reduce z fighting by only rendering the rear facing triangles to the depth map
//			// the culling can be disabled by calling
//			light.getShadow().setGlCullingEnabled(false);
//			// or the culling winding order can be changed by calling
//			light.getShadow().setFrontFaceWindingOrder(GL_CCW); // default is GL_CW
			renderScene();
			light.endShadowDepthPass(j);
		}
	}
	
	
	
	cam.begin(mViewRect);
	
	cubeMap.draw();
	
	renderScene();
	
	if( bDebug && bDrawBounds ) {
		for( auto& mesh : model.getMeshes() ) {
			mesh->getGlobalBounds().draw();
		}
	}
	
	if( bDebug ) {
		light.getShadow().drawFrustum();
		light.draw();
		camOnSecurityCam.draw();
	}
	
	cam.end();
	ofDisableDepthTest();
	
	ofSetColor( 30 );
	ofDrawRectangle( mTextRect );
	
	ofSetColor(255);
	
	float texPadding = 10.f;
	float texWidth = (mTextRect.getWidth() - texPadding*3.0f) * 0.5;
	// now lets grab the textures from the meshes //
	ofRectangle meshTexRect( mTextRect.x + texPadding, 150.0f, texWidth, texWidth );
	if( model.getNumMeshes() > 0 ) {
		auto mesh = model.getMesh(0);
		if( mesh->getNumTextures() > 0 ) {
			for( unsigned int i = 0; i < mesh->getNumTextures(); i++ ) {
				auto texture = mesh->getTexture(i);
				if( texture ) {
					texture->getTextureRef().draw(meshTexRect);
					ofDrawBitmapString(texture->getOfTextureTypeAsString(), meshTexRect.x, meshTexRect.y-4);
					meshTexRect.x += texPadding + meshTexRect.getWidth();
					if( i % 2 == 1 ) {
						meshTexRect.x = mTextRect.x + texPadding;
						meshTexRect.y += meshTexRect.height + texPadding * 3.0;
					}
				}
			}
		}
	}
	
	ofSetColor( 225 );
	ofDrawBitmapString(mSceneString, mTextRect.x + 12, mTextRect.y + 24 );
	
	float camRectW = (mTextRect.getWidth() - texPadding*2.0f);
	float ratio = 480.f / 640.f;
	auto camRect = ofRectangle( mTextRect.x+texPadding, meshTexRect.y+texPadding, camRectW, camRectW * ratio );
	ofSetColor(255);
	ofDrawBitmapString("Camera feed, control with arrow keys.", camRect.x, camRect.y-texPadding * 0.5 );
	camOnSecurityCam.begin(camRect);
	cubeMap.draw();
	camOnSecurityCam.end();
	
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::renderScene() {
	wallMaterial.begin();
	wallMesh.draw();
	wallMaterial.end();
	if( bDrawMeshes) {
		if( bDrawWireframe ) {
			model.drawWireframe();
		} else {
			model.drawFaces();
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':
			
            break;
        case 'h':
			
            break;
        case OF_KEY_DOWN:
			tiltDirection = 1.f;
            break;
		case OF_KEY_UP:
			tiltDirection = -1.f;
			break;
		case OF_KEY_LEFT:
			panDirection = -1.f;
			break;
		case OF_KEY_RIGHT:
			panDirection = 1.f;
			break;
		case 'f':
			ofToggleFullscreen();
			break;
		case 'w':
			bDrawWireframe=!bDrawWireframe;
			break;
		case 'm':
			bDrawMeshes=!bDrawMeshes;
			break;
		case 'b':
			bDrawBounds=!bDrawBounds;
			break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if( key == OF_KEY_RIGHT || key == OF_KEY_LEFT ) {
		panDirection = 0.f;
	}
	if( key == OF_KEY_DOWN || key == OF_KEY_UP ) {
		tiltDirection = 0.f;
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
//	ofLogNotice("ofApp :: windowResized : ") << w << " x " << h;
	mViewRect = ofRectangle( 0, 0, std::min(ofGetWidth()-320, 900), ofGetHeight());
	mTextRect = ofRectangle( mViewRect.getRight(), 0, ofGetWidth() - mViewRect.getWidth(), ofGetHeight() );
	
	cam.setControlArea(mViewRect);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

