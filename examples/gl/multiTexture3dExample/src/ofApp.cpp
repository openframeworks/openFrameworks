#include "ofApp.h"
#include "ofGLProgrammableRenderer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	// lets make two lights
	for( int i = 0; i < 2; i++ ){
		auto light = make_shared<ofLight>();
		light->setup();
		if( i == 0 ) {
			light->setPosition(400, -100, 400);
		}
		light->setDiffuseColor(ofFloatColor(0.7,1.0));
		light->setAmbientColor(ofFloatColor(0.3,0.1));
		lights.push_back( light );
	}
	
	// store a variable of whether arb textures are enabled
	bool isArbTexEnabled = ofGetUsingArbTex();
	// disable arb textures so the texture coordinates are from 0 -> 1
	// and we have have the texture sample repeat using setTextureWrap(GL_REPEAT, GL_REPEAT);
	// (see TexturePack::configureTexture in ofApp.h)
	ofDisableArbTex();
	
	// texture images are from https://polyhaven.com
	// Use a class called TexturePack that has been defined in ofApp.h
	// this will help us load and store all the textures from a folder
	// see TexturePack class for other functions and variables in ofApp.h
	#ifdef USE_MATERIAL
	string matPath = "textures/Snow_03/";
	auto snowMat = make_shared<ofMaterial>();
	snowMat->setEmissiveColor(ofFloatColor(211/255.,25/255.,150/255., 0.8));
	snowMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC, matPath+"snow_03_arm.jpg");
	snowMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_DIFFUSE, matPath+"snow_03_diff.jpg");
	snowMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_DISPLACEMENT, matPath+"snow_03_disp.png");
	snowMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_NORMAL, matPath+"snow_03_nor_gl.jpg");
	materials.push_back(snowMat);
	
	matPath = "textures/Mud_cracked_dry_03/";
	auto mudMat = make_shared<ofMaterial>();
	mudMat->setEmissiveColor(ofFloatColor(25/255.,221/255.,137/255., 0.8));
	mudMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC, matPath+"mud_cracked_dry_03_arm.jpg");
	mudMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_DIFFUSE, matPath+"mud_cracked_dry_03_diff.jpg");
	mudMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_DISPLACEMENT, matPath+"mud_cracked_dry_03_disp.png");
	mudMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_NORMAL, matPath+"mud_cracked_dry_03_nor_gl.jpg");
	materials.push_back(mudMat);
	
	matPath = "textures/Square_floor/";
	auto floorMat = make_shared<ofMaterial>();
	floorMat->setEmissiveColor(ofColor(237,222,69., 200));
	floorMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC, matPath+"square_floor_arm.jpg");
	floorMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_DIFFUSE, matPath+"square_floor_diff.jpg");
	floorMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_DISPLACEMENT, matPath+"square_floor_disp.png");
	floorMat->loadTexture(ofMaterialTextureType::OF_MATERIAL_TEXTURE_NORMAL, matPath+"square_floor_nor_gl.jpg");
	materials.push_back(floorMat);
	#else
	
	texturePacks.push_back( make_shared<TexturePack>("Snow_03"));
	texturePacks.back()->material.setEmissiveColor(ofFloatColor(211/255.,25/255.,150/255., 0.8));
	texturePacks.back()->material.setSpecularColor(ofFloatColor(1,1,1,0.8));
	texturePacks.back()->material.setShininess(40);
	texturePacks.back()->material.setDiffuseColor(ofFloatColor(1,1,1,1.0));
	
	texturePacks.push_back( make_shared<TexturePack>("Mud_cracked_dry_03"));
	texturePacks.back()->material.setEmissiveColor(ofFloatColor(25/255.,221/255.,137/255., 0.8));
	texturePacks.back()->material.setSpecularColor(ofFloatColor(1,1,1,0.05));
	texturePacks.back()->material.setShininess(1);
	texturePacks.back()->material.setDiffuseColor(ofFloatColor(1,1,1,0.7));
	
	texturePacks.push_back( make_shared<TexturePack>("Square_floor"));
	texturePacks.back()->material.setEmissiveColor(ofColor(237,222,69., 200));
	texturePacks.back()->material.setSpecularColor(ofFloatColor(1,1,1,0.6));
	texturePacks.back()->material.setShininess(100);
	texturePacks.back()->material.setDiffuseColor(ofFloatColor(1,1,1,1.0));
	
	// load a shader that will be used for mesh manipulation and lighting
	shader = make_shared<ofShader>();
	shader->load("shaders/shader");
	
	for( int i = 0; i < texturePacks.size(); i++ ){
		// set the material to use our custom shader
		// so that on material.begin() it will call shader->begin()
		// and we can provide our custom vert and frag shaders
		texturePacks[i]->material.setCustomShader(shader);
		
		texturePacks[i]->material.setAmbientColor(ofFloatColor(1,1,1,0.6));
		// set our textures on the material so that they will be passed to our custom shader
		texturePacks[i]->material.setCustomUniformTexture("mapDiffuse", texturePacks[i]->textureDiffuseMap, 0 );
		texturePacks[i]->material.setCustomUniformTexture("mapNormal", texturePacks[i]->textureNormalMap, 1 );
		texturePacks[i]->material.setCustomUniformTexture("mapDisplacement", texturePacks[i]->textureDisplacementMap, 2 );
		texturePacks[i]->material.setCustomUniformTexture("mapAORoughMetal", texturePacks[i]->textureAORoughMetal, 3 );
	}
	#endif
	// We need a higher resolution sphere because we are going to use the displacement maps
	// to push out the vertices using their normal as a direction ( see shader.vert )
	meshSphere = ofMesh::sphere(1.0, 128);
	
	// allocate an fbo that will be used to draw into to determine how much the textures
	// should influence the sphere
	// we use a value of GL_RGB32F for more precision
	fboInfluence.allocate(512, 512, GL_RGBA32F );
	fboInfluence.begin();
	ofClear(0,0,0,1);
	fboInfluence.end();
	fboInfluence.updateTexture(0);
#ifdef USE_MATERIAL
	for( auto& mat : materials ) {
		mat->setCustomUniformTexture("mapInfluence", fboInfluence.getTexture() );
	}
	keyPressed('r');
#endif
	
	// check the boolean we set before
	// if arb textures were enabled then, lets go ahead and activate again
	if( isArbTexEnabled ){
		ofEnableArbTex();
	}
	
	// load a brush for drawing into the influence fbo
	ofLoadImage( textureBrush, "textures/dot.png");
	// set the anchor percent to 0.5,0.5 so that it will draw from the center of the image
	textureBrush.setAnchorPercent(0.5, 0.5);
}

//--------------------------------------------------------------
void ofApp::update(){
	// it can be handy to reload a shader without recompiling when debugging or working on shader functionality
	// change shader, save and then press 'r' with this app in foreground and the shader should reload
	// check the console to see if there are any errors
#ifndef USE_MATERIAL
	if( ofGetKeyPressed('r') || (!shader->isLoaded() && ofGetFrameNum() % 30 == 0) ){
		cout << "LOADING SHADER | " << ofGetFrameNum() << endl;
		shader->load("shaders/shader");
	}
#endif
	if( lights.size() > 0 ){
		// one of the lights moves around the scene
		lights.back()->orbitDeg( ofWrapDegrees(ofGetElapsedTimef()* 30.0), -40, 500 );
	}
	
	// scale the fbo rect to the window rect so that we can maximize area
	ofRectangle windowRect(0,0,ofGetWidth(), ofGetHeight());
	fboRect = ofRectangle( 0, 0, fboInfluence.getWidth(), fboInfluence.getHeight());
	fboRect.scaleTo(windowRect);
}

//--------------------------------------------------------------
void ofApp::draw(){
	// draw a gradient in the background
	ofBackgroundGradient(ofColor(0), ofColor(50));
	
	camera.begin();
	ofEnableDepthTest();
#ifdef USE_MATERIAL
	materials[materialIndex]->begin();
	ofPushMatrix();
	ofScale(200, 200, 200 );
	meshSphere.draw();
	ofPopMatrix();
	materials[materialIndex]->end();
#else
	if( shader->isLoaded() ){
		texturePacks[texturePackIndex]->material.begin();
		// since we set shader to be the custom shader on material
		// when texturePacks[texturePackIndex]->material.begin(); is called
		// it calls begin on our shader, so now it is active
		shader->setUniformTexture("mapInfluence", fboInfluence.getTexture(), 4 );
		
		ofPushMatrix();
		ofScale(200, 200, 200 );
		meshSphere.draw();
		ofPopMatrix();
		texturePacks[texturePackIndex]->material.end();
	}
#endif
	
	// draw a grid for reference
	ofSetColor( 80 );
	ofPushMatrix();
	ofTranslate(0, -300, 0 );
	ofRotateYDeg(90);
	ofRotateZDeg(90);
	ofDrawGridPlane(100, 20);
	ofPopMatrix();
	
	if( bDebug ){
		ofSetColor( 255 );
		for( auto& light : lights ) {
			ofDrawSphere(light->getPosition(), 20.0);
			ofDrawLine( glm::vec3(0,0,0), light->getPosition() );
		}
		ofDrawAxis(500);
	}
	ofDisableDepthTest();
	camera.end();
	
	
	if( bDrawIntoFbo ){
		// if the mouse is inside the fbo rect and the mouse is pressed, try to draw into the fbo
		if( fboRect.inside(ofGetMouseX(), ofGetMouseY() ) && ofGetMousePressed() ) {
			// map the mouse from screen coordinates to the fbo coordinates
			float fmousex = ofMap( ofGetMouseX(), fboRect.x, fboRect.getRight(), 0.0, fboInfluence.getWidth() );
			float fmousey = ofMap( ofGetMouseY(), fboRect.y, fboRect.getBottom(), 0.0, fboInfluence.getHeight() );
			fboInfluence.begin();
			ofSetColor( 255, 20 );
			float tsize = 150.f;
			// draw based on mouse position
			textureBrush.draw(fmousex,fmousey,tsize,tsize);
			// we want the fbo texture to 'wrap' around the sphere, so we also draw using offsets
			// that way if the brush is at 0,0, it will also draw at width,height of the fbo
			// enable debug to see the fbo influence drawn to the screen
			textureBrush.draw(fmousex+fboInfluence.getWidth(),fmousey, tsize,tsize);
			textureBrush.draw(fmousex-fboInfluence.getWidth(),fmousey, tsize,tsize);
			textureBrush.draw(fmousex,fmousey+fboInfluence.getHeight(), tsize,tsize);
			textureBrush.draw(fmousex,fmousey-fboInfluence.getHeight(), tsize,tsize);
			fboInfluence.end();
		}
		ofNoFill();
		ofSetColor( 180 );
		ofDrawRectangle( fboRect );
		ofFill();
		camera.disableMouseInput();
	}else{
		camera.enableMouseInput();
	}
	
	if( ofGetKeyPressed('e') ){
		fboInfluence.begin();
		// slowly draw a transparent black rectangle over the whole fbo to erase the content over time
		ofSetColor( 0, 10 );
		ofDrawRectangle(0, 0, fboInfluence.getWidth(), fboInfluence.getHeight() );
		fboInfluence.end();
	}
	
	if( ofGetKeyPressed('c')){
		float siny = sinf( ofGetElapsedTimef() * 2.5f ) * 0.5 + 0.5;
		fboInfluence.begin();
		ofSetColor( 0, 150 );
		ofDrawRectangle(0, siny * fboInfluence.getHeight(), fboInfluence.getWidth(), 100.0);
		fboInfluence.end();
	}
	
	if(bDebug){
		ofRectangle fitRect( 20,128,fboInfluence.getWidth() * 0.5, fboInfluence.getHeight() * 0.5 );
		ofSetColor( 255 );
		fboInfluence.draw(fitRect);
	}
	
	ofSetColor( 220 );
	stringstream ss;
	ss << "Draw into influence with mouse pressed and (spacebar): " << bDrawIntoFbo << endl;
	ss << "Linear clear influence(c): " << ofGetKeyPressed('c') << endl;
	ss << "Clear influence(e): " << ofGetKeyPressed('e') << endl;
#ifdef USE_MATERIAL
	ss << "Material index (up / down): " << (materialIndex+1) << " / " << materials.size() << endl;
#else
	ss << "Texture pack index (up / down): " << (texturePackIndex+1) << " / " << texturePacks.size() << endl;
#endif
	ss << "Debug (d): " << bDebug;
	ofDrawBitmapString(ss.str(), 20, 32);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == OF_KEY_UP ){
		#ifdef USE_MATERIAL
		materialIndex--;
		if(materialIndex < 0 ) {
			materialIndex = 0;
		}
		#else
		texturePackIndex--;
		if( texturePackIndex < 0 ) {
			texturePackIndex = 0;
		}
		#endif
	}
	if( key == OF_KEY_DOWN ){
		#ifdef USE_MATERIAL
		materialIndex++;
		if( materialIndex >= materials.size() ) {
			materialIndex = materials.size()-1;
		}
		#else
		texturePackIndex++;
		if( texturePackIndex >= texturePacks.size() ) {
			texturePackIndex = texturePacks.size()-1;
		}
		#endif
	}
	if( key == ' ' ){
		bDrawIntoFbo = true;
	}
	if( key == 'd' ){
		bDebug = !bDebug;
	}
	if( key == 'r' ) {
		#ifdef USE_MATERIAL
		string vname = "shaders/material.vert";
		ofBuffer vbuffer = ofBufferFromFile(vname);
		string fname = "shaders/material.frag";
		ofBuffer fbuffer = ofBufferFromFile(fname);
		if( vbuffer.size() ) {
			for( auto& mat : materials ) {
				ofLogNotice("SETTING the shader vertex main!") << " | " << ofGetFrameNum();
				mat->setShaderMain(vbuffer.getText(), GL_VERTEX_SHADER, "main.vert");
			}
		}
		if( fbuffer.size() ) {
			for( auto& mat : materials ) {
				mat->setShaderMain(fbuffer.getText(), GL_FRAGMENT_SHADER, "main.frag");
			}
		}
		#endif
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if( key == ' ' ){
		bDrawIntoFbo = false;
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
