#include "ofApp.h"
#include "ofGLProgrammableRenderer.h"

//--------------------------------------------------------------
void ofApp::setup(){
//	ofSetLogLevel(OF_LOG_VERBOSE);
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
	// to push out the vertices using their normal as a direction ( see shader.vert and material.vert )
#if defined(TARGET_OPENGLES)
	meshSphere = ofMesh::sphere(1.0, 72);
#else
	meshSphere = ofMesh::sphere(1.0, 128);
#endif
	colliderMesh = ofMesh::sphere(1.0, 16, OF_PRIMITIVE_TRIANGLES );
	// allocate an fbo that will be used to draw into to determine how much the textures
	// should influence the sphere
	// we use a value of GL_RGBA32F for more precision
	fboInfluence.allocate(512, 512, GL_RGBA32F );
	fboInfluence.begin();
	ofClear(0,0,0,1);
	fboInfluence.end();
	fboInfluence.updateTexture(0);
#ifdef USE_MATERIAL
	for( auto& mat : materials ) {
		mat->setCustomUniformTexture("mapInfluence", fboInfluence.getTexture() );
		mat->setCustomUniform1f("uIsDrawingInto", 0.0f );
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
	// we delay setting bDrawIntoFbo to true by default because it disables the camera mouse input
	// and if we start with it disabled, then it does not update its position
	// do give it a few frames to update before enabled bDrawIntoFbo
	if( ofGetFrameNum() == 5 ) {
		bDrawIntoFbo = true;
	}
	
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
	
	mInputPos = glm::vec2(ofGetMouseX(), ofGetMouseY() );
	bBrushDrawing = ofGetMousePressed();
	
	auto dpos = camera.screenToWorld( glm::vec3( mInputPos.x, mInputPos.y, 100) );
	auto dir = camera.getPosition() - dpos;
	dir = glm::normalize(dir);
	
	float sphereScale = 200.f;
	// try to find a hit pos
	mBHitATri = false;
	mHitSpherePos = {-1000, -1000, 0};
	const auto& indices = colliderMesh.getIndices();
	const auto& verts = colliderMesh.getVertices();
	const auto& normals = colliderMesh.getNormals();
	auto& colors = colliderMesh.getColors();
	colors.clear();
	if( colors.size() != verts.size() ) {
		colors.assign(verts.size(), ofFloatColor(1,1) );
	}
	
	if( mHitMeshTri.getNumVertices() != 3 ) {
		mHitMeshTri.getVertices().resize(3);
		mHitMeshTri.getColors().assign(mHitMeshTri.getNumVertices(), ofFloatColor(1,0.8));
		mHitMeshTri.setMode( OF_PRIMITIVE_TRIANGLES );
	}
	
	ofFloatColor backColor = ofColor::darkMagenta;
	
	size_t numIndices = indices.size();
	glm::vec3 v1, v2, v3;
	auto camLook = camera.getLookAtDir();
	for( int i = 0; i < numIndices; i+=3 ) {
		
		if( i+2 >= numIndices ) {
			break;
		}
		
		v1 = verts[ indices[i+0] ] * sphereScale;
		v2 = verts[ indices[i+1] ] * sphereScale;
		v3 = verts[ indices[i+2] ] * sphereScale;
		
		auto& faceNormal = normals[indices[i+0]];

		if( glm::dot(faceNormal, camLook ) > 0 ) {
//			 looking in the same direction //
//			 ie, away from camera
			// these must be in the back of the sphere, set debug colors
			// and don't check if a tri is hit
			colors[indices[i+0]] = backColor;
			colors[indices[i+1]] = backColor;
			colors[indices[i+2]] = backColor;
			continue;
		}
		
		// we haven't hit a triangle yet, so perform some ray -> triangle intersection
		if(!mBHitATri && rayIntersectsTriangle( dpos, dir, v1, v2, v3, mHitSpherePos )) {
			// we hit a triangle! store the information
			mBHitATri = true;
			mHitMeshTri.getVertices()[0] = v1;
			mHitMeshTri.getVertices()[1] = v2;
			mHitMeshTri.getVertices()[2] = v3;
			
			mHitNormal = normals[indices[i+0]] + normals[indices[i+1]] + normals[indices[i+2]];
			mHitNormal /= 3.f;
			mHitNormal = glm::normalize(mHitNormal);
			
			float a0=0.f, a1=0.f, a2=0.f;
			// calculate the area of the triangle using baycentric coordinates so that
			// we can use those values to store the texture coord
			calcTriArea( v1, v2, v3, mHitSpherePos, a0, a1, a2 );
			// apply baycentric values to the tex coord so that we can calculate the brush
			// position in the fbo in the draw function 
			mHitCoord = colliderMesh.getTexCoords()[indices[i+0]] * a0;
			mHitCoord += colliderMesh.getTexCoords()[indices[i+1]] * a1;
			mHitCoord += colliderMesh.getTexCoords()[indices[i+2]] * a2;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	// draw a gradient in the background
	ofBackgroundGradient(ofColor(0), ofColor(50));
	
	camera.begin();
	ofEnableDepthTest();
#ifdef USE_MATERIAL
	
	if(!bDebug) {
		materials[materialIndex]->setCustomUniform1f("uIsDrawingInto", (bDrawIntoFbo && mBHitATri) ? 1.0f : 0.0f );		
		materials[materialIndex]->begin();
		ofPushMatrix();
		ofScale(200, 200, 200 );
		meshSphere.draw();
		ofPopMatrix();
		materials[materialIndex]->end();
	}
#else
	
	if(!bDebug) {
		if( shader->isLoaded() ){
			texturePacks[texturePackIndex]->material.begin();
			// since we set shader to be the custom shader on material
			// when texturePacks[texturePackIndex]->material.begin(); is called
			// it calls begin on our shader, so now it is active
			shader->setUniformTexture("mapInfluence", fboInfluence.getTexture(), 4 );
			shader->setUniform1f("uIsDrawingInto", (bDrawIntoFbo && mBHitATri) ? 1.0f : 0.0f );
			
			ofPushMatrix();
			ofScale(200, 200, 200 );
			meshSphere.draw();
			ofPopMatrix();
			texturePacks[texturePackIndex]->material.end();
		}
	}
#endif
	
	if(bDebug) {
		ofPushMatrix();
		ofScale(200, 200, 200 );
		colliderMesh.drawWireframe();
		ofPopMatrix();
		
		if( mBHitATri ) {
			ofSetColor( 255 );
			mHitMeshTri.draw();
		}
		
		ofSetColor( ofColor::yellow );
		if( mBHitATri ) {
			ofDrawLine( mHitSpherePos, mHitSpherePos + mHitNormal * 300.0f);
			ofDrawSphere( mHitSpherePos, 6 );
		}
	}
	
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
		}
		ofDrawAxis(500);
	}
	
	
	ofDisableDepthTest();
	camera.end();
	
	
	if( bDrawIntoFbo ){
		if(mBHitATri ) {
			float deltaTime = ofClamp( ofGetLastFrameTime(), 1.f/1000.f, 1.f/10.f );
			fboInfluence.begin();
			ofSetColor( 255, MAX( (deltaTime * 60.0f) * mBrushStrength, 1) );
			// only enable drawing into the red and alpha channels that will be used for the height
			// offset on the mesh in the shader
			glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);

			float tsize = 0.5f * mBrushSize;//ofMap( fabs(mHitCoord.y-0.5), 0.5, 0.0,1.0, 0.5, true )  *  mBrushSize;
			float tsizey = mBrushSize;
			float fmousex = mHitCoord.x * fboInfluence.getWidth();
			float fmousey = mHitCoord.y * fboInfluence.getHeight();
			
			if(bBrushDrawing) {
				// draw based on mouse position
				textureBrush.draw(fmousex,fmousey,tsize,tsizey);
				// we want the fbo texture to 'wrap' around the sphere horizontally, so we also draw using offsets
				// that way if the brush is at 0,0, it will also draw at width,height of the fbo
				// enable debug to see the fbo influence drawn to the screen
				textureBrush.draw(fmousex+fboInfluence.getWidth(),fmousey, tsize,tsizey);
				textureBrush.draw(fmousex-fboInfluence.getWidth(),fmousey, tsize,tsizey);
			}
			
			// lets store the brush influence in the Green channel, since it's not used to calculate height
			// mask out the other colors to only render green
			glColorMask(GL_FALSE, GL_TRUE, GL_FALSE, GL_TRUE);
			// clear to black
			ofClear(0, 0, 0, 0);
			ofSetColor(255);
			textureBrush.draw(fmousex,fmousey,tsize,tsizey);
			textureBrush.draw(fmousex+fboInfluence.getWidth(),fmousey, tsize,tsizey);
			textureBrush.draw(fmousex-fboInfluence.getWidth(),fmousey, tsize,tsizey);
			// reset drawing all colors
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			fboInfluence.end();
		}
		
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
	
	if( ofGetKeyPressed('w')){
		float siny = sinf( ofGetElapsedTimef() * 2.5f ) * 0.5 + 0.5;
		fboInfluence.begin();
		ofSetColor( 0, 150 );
		ofDrawRectangle(0, siny * fboInfluence.getHeight(), fboInfluence.getWidth(), 100.0);
		fboInfluence.end();
	}
	
	ofSetColor( 220 );
	stringstream ss;
	ss << "Draw brush into influence with mouse pressed." << endl;
	ss << "Move camera, press and hold (c): " << !bDrawIntoFbo << endl;
	ss << "Linear clear influence(w): " << ofGetKeyPressed('w') << endl;
	ss << "Clear influence(e): " << ofGetKeyPressed('e') << endl;
	ss << "Brush size(z,x): " << mBrushSize << endl;
	ss << "Brush strength(a,s): " << mBrushStrength << endl;
#ifdef USE_MATERIAL
	ss << "Material index (up / down): " << (materialIndex+1) << " / " << materials.size() << endl;
#else
	ss << "Texture pack index (up / down): " << (texturePackIndex+1) << " / " << texturePacks.size() << endl;
#endif
	ss << "Debug (d): " << bDebug;
	ofDrawBitmapString(ss.str(), 20, 32);
}
	
// http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/
bool ofApp::rayIntersectsTriangle( glm::vec3& aP, glm::vec3& aDirection,
								  glm::vec3& aV0, glm::vec3& aV1, glm::vec3& aV2, glm::vec3& aIntersectionPt ) {
	
	float a,f,u,v;
	glm::vec3 e1 = aV1 - aV0;
	glm::vec3 e2 = aV2 - aV0;
	glm::vec3 h, s, q;
	h = glm::cross(aDirection, e2);
	a = glm::dot(e1, h);
	
	if (a > -0.00001f && a < 0.00001f) {
		return false;
	}
	
	f = 1.f/a;
	s = aP - aV0;
	u = f * glm::dot(s,h);
	
	if (u < 0.0f || u > 1.0f) {
		return false;
	}
	
	q = glm::cross(s, e1);
	v = f * glm::dot(aDirection, q);
	
	if (v < 0.0 || u + v > 1.0) {
		return false;
	}
	
	// at this stage we can compute t to find out where
	// the intersection point is on the line
	float t = f * glm::dot(e2,q);
	
	if ( t > 0.00001f ) {
		// ray intersection
		aIntersectionPt = aP + t * aDirection;
		return true;
	}
	
	// this means that there is a line intersection
	// but not a ray intersection
	return false;
}

// using baycentric coordinates //
//--------------------------------------------------------------
float ofApp::calcTriArea( glm::vec3& aA, glm::vec3& aB, glm::vec3& aC ) {
	float det = 0.0;
	glm::vec3 ba = aB-aA;
	glm::vec3 ca = aC-aA;
	glm::vec3 tcross = glm::cross(ba, ca);
	det = glm::length(tcross);
	return det / 2.0f;
}

// aA, aB, aC are the points that make up the triangle //
// aP is the point inside the triangle //
// a1, a2 and a3 are the areas of the three triangles //
// use those areas to multiply by the verts to know where the point should be
// i.e. point = aA * a1 + aB * a2 + aC * a3 //
//--------------------------------------------------------------
void ofApp::calcTriArea( glm::vec3& aA, glm::vec3& aB, glm::vec3& aC, glm::vec3& aP, float& a1, float& a2, float& a3 ) {
	float area = calcTriArea( aA, aB, aC );
	if( area == 0 ) {
		ofLogWarning("CalcTriArea :: area == 0 ");
		a1 = a2 = a3 = 0.f;
		// returning to avoid divide by zero
		return;
	}
	
	//calculate barycentric coordinates of aA, aB and aC
	a1 = calcTriArea( aB, aC, aP)/area;
	a2 = calcTriArea( aC, aA, aP)/area;
	a3 = calcTriArea( aA, aB, aP)/area;
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
	if( key == 'c' ){
		bDrawIntoFbo = false;
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
	
	if( key == 'z' ) {
		mBrushSize -= 10.f;
	}
	if( key == 'x') {
		mBrushSize += 10.f;
	}
	if( key == 'a' ) {
		mBrushStrength -= 2;
	}
	if(key == 's') {
		mBrushStrength += 2;
	}
	mBrushStrength = ofClamp(mBrushStrength, 2, 250 );
	
	mBrushSize = ofClamp(mBrushSize, 10, 500);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if( key == 'c' ){
		bDrawIntoFbo = true;
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
