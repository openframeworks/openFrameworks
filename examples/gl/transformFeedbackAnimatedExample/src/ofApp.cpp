#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofShader::TransformFeedbackSettings settings;
	// we only need a vertex shader since the transform feedback is not used for rendering
	settings.shaderFiles[GL_FRAGMENT_SHADER] = "feedback.frag";
	settings.shaderFiles[GL_VERTEX_SHADER] = "feedback.vert";
	// the default attributes for ofShader are
	// position, normal, color and texcoord
	// the names are defined at the top of ofShader.cpp
	settings.bindDefaults = true;
	// we define 2 outputs of the transform feedback objects
	settings.varyingsToCapture = { "TF_Position", "TF_Color" };
	settings.bufferMode = GL_SEPARATE_ATTRIBS;
	shaderTransformFeedback.setup(settings);
	
	// create some vectors to hold the initial data
	// we are using glm::vec4 because it gives us some more data to use in the shaders
	// we don't use a vector of ofFloatColor because it can clamp the value to its color range 0 -> 1
	vector<glm::vec4> colorData;
	// we are going to use the ofVbo.setVertexData function that expects glm::vec3
	vector<glm::vec3> positionData;
	
	ofPixels logoPixels;
	// lets load an image so that we can use it to layout the particles
	if( ofLoadImage(logoPixels, "ofLogo.png")) {
		float pixWidth = logoPixels.getWidth();
		float pixHeight = logoPixels.getHeight();

		for( int ix = 0; ix < pixWidth; ix += 2 ) {
			for( int iy = 0; iy < pixHeight; iy += 2 ) {
				ofColor pixColor = logoPixels.getColor( ix, iy );
				// if we found a lighter pixel, than go ahead and create some data to store the position
				if( pixColor.r > 200 ) {
					// we are going to store this intial 'home' position in the color data x and y values that
					// we will reference later in both the transform feedback shader and pointSprite render shader
					glm::vec4 newColorData = glm::vec4(ix-pixWidth*0.5,iy-pixHeight*0.5,0,ofRandom(0.5,1.5));
					newColorData.x *= 2.f;
					newColorData.y *= 2.f;
					// in the transform feedback shader (vert.glsl) we will use the newColorData.z to track the life of the particle
					// and newColorData.w as the max life ( see bin/data/vert.glsl for more detail );
					colorData.push_back(newColorData);
					// set the initial position of the particle along the x and z plane
					auto position = glm::vec3(newColorData.x, 0, newColorData.y);
					positionData.push_back(position);
				}
			}
		}
		numVertices = colorData.size();
	} else {
		// if there was a problem loading the image, then just assign default data
		colorData.assign( numVertices, glm::vec4(0,0,0,0.0) );
		positionData.assign( numVertices, glm::vec3(0,0,0) );
	}
	
	// we are going to use two separate vbos to ping pong the data between the two transform feedback objects
	// the output of transform feedback 0 will output to vbo[1]
	// the output of transform feedback 1 will output to vbo[0]
	for (int i = 0; i < 2; i++) {
		auto vbo = make_shared<ofVbo>();
		vbo->setVertexData( positionData.data(), numVertices, GL_DYNAMIC_COPY );
		// since we are using glm::vec4 we can not call the vbo->setColorData function because it is expecting a vector
		// of ofFloatColor as an argument 
		vbo->setAttributeData( ofShader::COLOR_ATTRIBUTE, &colorData[0].x, 4, (int)colorData.size(), GL_DYNAMIC_COPY, sizeof(glm::vec4) );
		vbos.push_back( vbo );
	}
	
	// create 2 sets of transform feedback buffers to capture the ping pong
	for (int i = 0; i < 2; i++) {
		transformFeedbackVertexBindings.push_back(ofShader::TransformFeedbackBaseBinding(vbos[i]->getVertexBuffer()));
		transformFeedbackVertexBindings[i].index = ofShader::POSITION_ATTRIBUTE;
		
		transformFeedbackNormalBindings.push_back(ofShader::TransformFeedbackBaseBinding(vbos[i]->getColorBuffer()));
		transformFeedbackNormalBindings[i].index = ofShader::COLOR_ATTRIBUTE;
	}
	
	// load a shader to work with point sprites
	// shaderRender will read in the attributes from the vbo and use them for rendering
	shaderRender.load("pointSprite.vert", "pointSprite.frag");
	
	// load an image to use for shaderRender
	// we call disableArbTex to be used with the point sprite texcoords that are mapped from 0 -> 1
	ofDisableArbTex();
	ofLoadImage(dotTexture, "dot.png");
	ofEnableArbTex();
	
	gui.setup();
	// GUI //
	paramGroupTransformFeedback.setName("TransformFeedbackShader");
	paramGroupTransformFeedback.add(paramMaxLife.set("uMaxLife", 1.0, 0.2, 4.0 ));
	paramGroupTransformFeedback.add(paramNoiseStrength.set("uNoiseStrength", 60.0, 0, 1000));
	paramGroupTransformFeedback.add(paramUpwardForce.set("uUpForce", 40.f, -500, 500));
	paramGroupTransformFeedback.add(paramNoiseTimeOffsetMultiplier.set("uNoiseTimeOffsetMultiplier", 0.5, 0.0, 10.0));
	gui.add( paramGroupTransformFeedback );
	
	paramGroupRender.setName("RenderShader");
	paramGroupRender.add(paramPointSize.set("uPointSize", 7, 2, 30));
	gui.add( paramGroupRender );
	// we do not add to the shader parameter group because we are going to use those to set the
	// uniforms on the shader and colors from ofParameters are not supported
	gui.add( paramRenderStartColor1.set("uStartColor1", ofFloatColor(211/255.,25/255.,150/255.,1.0)));
	gui.add( paramRenderStartColor2.set("uStartColor2", ofFloatColor(222/255.,1.0)));
	gui.add( paramRenderEndColor.set("uEndColor", ofFloatColor(25/255.,221/255.,137/255.,1.0)));
	
	gui.loadFromFile("settings.xml");
	
	cam.setPosition(0, 600, 200);
	cam.lookAt( glm::vec3(0,0,0) );
}

//--------------------------------------------------------------
void ofApp::update(){
	float deltaTime = ofClamp( ofGetLastFrameTime(), 1.f/5000.f, 1.f/5.f );
	// create a vector of the active transform feedback buffers using 1-bufferIndex
	// we use 1-bufferIndex here so that it can output to the opposing vbos[bufferIndex]
	// these will be passed to shaderTransformFeedback
	vector<ofShader::TransformFeedbackBaseBinding> currentTransformFeedbackBindings = {
		transformFeedbackVertexBindings[1-bufferIndex],
		transformFeedbackNormalBindings[1-bufferIndex]
	};

	shaderTransformFeedback.beginTransformFeedback(GL_POINTS, currentTransformFeedbackBindings );
	shaderTransformFeedback.setUniform1i("uTotalParticles", numVertices );
	shaderTransformFeedback.setUniform1f("uElapsedTime", ofGetElapsedTimef() );
	shaderTransformFeedback.setUniform1f("uDeltaTime", deltaTime );
	shaderTransformFeedback.setUniforms(paramGroupTransformFeedback);
	vbos[bufferIndex]->draw(GL_POINTS, 0, numVertices);
	shaderTransformFeedback.endTransformFeedback(currentTransformFeedbackBindings);
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor(10), ofColor(50));
	
	cam.begin();
	
	ofSetColor( 150 );
	ofPushMatrix();
	ofRotateYDeg(90);
	ofRotateZDeg(90);
	ofDrawGridPlane(100, 10);
	ofPopMatrix();
	
	ofEnablePointSprites();
	shaderRender.begin();
	shaderRender.setUniformTexture("tex", dotTexture, 0);
	shaderRender.setUniform1f("uElapsedTime", ofGetElapsedTimef() );
	shaderRender.setUniforms(paramGroupRender);
	
	shaderRender.setUniform4f( paramRenderStartColor1.getName(), paramRenderStartColor1.get() );
	shaderRender.setUniform4f( paramRenderStartColor2.getName(), paramRenderStartColor2.get() );
	shaderRender.setUniform4f( paramRenderEndColor.getName(), paramRenderEndColor.get() );
	
	vbos[bufferIndex]->draw(GL_POINTS, 0, numVertices);
	shaderRender.end();
	ofDisablePointSprites();
	
	cam.end();
	// swap the buffer index
	bufferIndex = 1 - bufferIndex;
	
	ofSetColor(255);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == 'r' ) {
		// reload the render shader, helpful for testing
		shaderRender.load("pointSprite.vert", "pointSprite.frag");
		ofLogNotice("Loading render shader from pointSprite");
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
