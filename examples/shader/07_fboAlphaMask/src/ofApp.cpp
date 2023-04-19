#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}else{
		shader.load("shadersGL2/shader");
	}
	
	brushImage.load("brush.png");
	// change the image to draw from the center
	// the default is from top left
	brushImage.setAnchorPercent(0.5f, 0.5f);
	
	textures.resize(5);
	for( int i = 0; i < textures.size(); i++ ){
		ofLoadImage(textures[i], "tex"+ofToString(i,0)+".jpg");
	}
	
	bBrushDown = false;
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	// check to see if we need a new fbo size
	// if the window size changes or the fbo is not allocated, we can catch it here and reallocate
	if( !maskFbo.isAllocated() || maskFbo.getWidth() != ofGetWidth() || maskFbo.getHeight() != ofGetHeight() ){
		ofLogNotice("Allocating mask fbo");
		maskFbo.allocate( ofGetWidth(), ofGetHeight());
		// Clear the FBO
		// otherwise it will bring some junk with it from the memory
		maskFbo.begin();
		ofClear(0,0,0,255);
		maskFbo.end();
		
		drawMesh = ofMesh::plane(maskFbo.getWidth(), maskFbo.getHeight() );
		// the plane has tex coords from 0 - 1, but we need tex coords from 0 -> maskFbo.getWidth()
		auto& texCoords = drawMesh.getTexCoords();
		// loop through the tex coords and remap to the size of the mask fbo
                auto& tex = maskFbo.getTexture();
		for( auto& tc : texCoords ){
                    tc = maskFbo.getTexture().getCoordFromPercent(tc.x, (1.0-tc.y));
		}
		
		// lets reload the font at a size in relation to width or height
		ofTrueTypeFont tf;
		tf.load("Batang.ttf", MIN(maskFbo.getWidth(), maskFbo.getHeight()) / 2, true, true, true);
		
		oLine.clear();
		// pull out the polylines of the O character for later use
		// getCharacterAsPoints returns an ofPath
		auto opath = tf.getCharacterAsPoints('O', true, false);
		// opath.getOutline returns a vector of ofPolyline
		auto opolys = opath.getOutline();
		// loop through the vector of ofPolyline and add the vertices to the oLine polyline
		for( auto op : opolys ){
			oLine.addVertices( op.getVertices() );
		}
		
		fLine.clear();
		// pull out the polylines of the F character for later use
		opath = tf.getCharacterAsPoints('F', true, false);
		opolys = opath.getOutline();
		for( auto op : opolys ){
			fLine.addVertices( op.getVertices() );
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(255);
	
	//----------------------------------------------------------
	// draw the brush into the fbo with the position based on the polylines derived from the font
	maskFbo.begin();
		ofSetColor(255, brushAlpha);
		if( bEraser ){
			// set the color to black //
			ofSetColor( 0, brushAlpha );
		}
		// store the elapsed time in a variable for later use
		float elapsedTime = ofGetElapsedTimef();
		float deltaTime = ofClamp( ofGetLastFrameTime(), 1.f / 1000.0, 1.f / 5.0f );
		
		float mouseXPercent = ofMap( ofGetMouseX(), 0, ofGetWidth(), 0.05, 1.0, true );
		eTimeCounter += deltaTime * mouseXPercent;
		
		float offset = ofMap( ofGetMouseY(), 10, ofGetHeight()-10, 0, 100, true);
		
		float polylinePercent = ofWrap( eTimeCounter, 0, 1);
		float findex = oLine.getIndexAtPercent(polylinePercent);
		
		auto position = oLine.getPointAtPercent( polylinePercent );
		auto normal = oLine.getNormalAtIndexInterpolated(findex);
		normal *= sin(  elapsedTime * 13.4f ) * cos(elapsedTime * 2.4f);
		normal.z = 0.0;
		position += normal * offset;
		
		ofPushMatrix();
			// translate to the center of the fbo //
			ofTranslate( maskFbo.getWidth()/2., maskFbo.getHeight() / 2. );
			// shift the letter over
			ofTranslate( -oLine.getCentroid2D().x, -oLine.getCentroid2D().y );
			ofTranslate( -oLine.getBoundingBox().getWidth()*0.75f, 0.0f );
			brushImage.draw(position.x, position.y, brushSize, brushSize);
		ofPopMatrix();
		
		
		findex = fLine.getIndexAtPercent(polylinePercent);
		position = fLine.getPointAtPercent( polylinePercent );
		normal = fLine.getNormalAtIndexInterpolated(findex);
		normal *= sin(  elapsedTime * 9.4f ) * cos(elapsedTime * 5.4f);
		normal.z = 0.0;
		position += normal * offset;
		
		ofPushMatrix();
			// translate to the center of the fbo //
			ofTranslate( maskFbo.getWidth()/2., maskFbo.getHeight() / 2. );
			// shift the letter over
			ofTranslate( -fLine.getCentroid2D().x, -fLine.getCentroid2D().y );
			ofTranslate( fLine.getBoundingBox().getWidth()*0.75f, 0.0f );
			brushImage.draw(position.x, position.y, brushSize, brushSize);
		ofPopMatrix();
		
	maskFbo.end();

	//----------------------------------------------------------
	// this is our alpha mask which we draw into.
	if(bBrushDown){
		maskFbo.begin();
		ofSetColor( 255, brushAlpha );
		if( bEraser ){
			// set the color to black //
			ofSetColor( 0, brushAlpha );
		}
		int brushImageX = ofGetMouseX();
		int brushImageY = ofGetMouseY();
		brushImage.draw(brushImageX, brushImageY, brushSize, brushSize);
		maskFbo.end();
	}
	
	//----------------------------------------------------------
	// HERE the shader-masking happens
	shader.begin();
	// tell the shader to use textures or not based on the boolean
	shader.setUniform1f("uUseTextures", bUseTextures ? 1.0f : 0.0f );
	// here is where the fbo is passed to the shader
	shader.setUniformTexture("maskTex", maskFbo.getTexture(), 0 );
	// pass the size of the mask texture to the shader
	shader.setUniform2f("uMaskSize", glm::vec2(maskFbo.getWidth(), maskFbo.getHeight()) );
	// pass the size of the sample textures to the shader
	// in this case they are all the same size, so we just pass the first one
	shader.setUniform2f("uTexSize", glm::vec2(textures[0].getWidth(), textures[0].getHeight()) );
	// set the textures
	for( int i = 0; i < textures.size(); i++ ){
		if( textures[i].isAllocated() ){
			// we add 1 here because the mask tex is at textureLocation 0
			shader.setUniformTexture("tex"+ofToString(i,0), textures[i], i+1);
		}
	}
	ofPushMatrix();
		// translate to center of window because ofMesh::plane origin is in the center //
		ofTranslate(ofGetWidth()/2, ofGetHeight()/2 );
		drawMesh.draw();
	ofPopMatrix();
	
	shader.end();
	
	//----------------------------------------------------------
	float drawStringY = 0;
	ofDrawBitmapStringHighlight("Drag the Mouse to draw. Mode (e): "+string(bEraser?"eraser":"brush"), 15,drawStringY+=15);
	ofDrawBitmapStringHighlight("Brush size ( [ / ] ): "+ofToString(brushSize,0), 15, drawStringY+=15);
	ofDrawBitmapStringHighlight("Brush alpha ( , / . ): "+ofToString(brushAlpha,0), 15, drawStringY+=15);
	ofDrawBitmapStringHighlight("Use Textures ( t ): "+string(bUseTextures?"yes":"no"), 15, drawStringY+=15);
	ofDrawBitmapStringHighlight("Press spacebar to clear", 15, drawStringY+=15);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == ' '){
		maskFbo.begin();
		ofClear(0,0,0,255);
		maskFbo.end();
	}
	if( key == '[' ){
		brushSize -= 10;
	}
	if( key == ']'){
		brushSize += 10;
	}
	if( key == ',' ){
		brushAlpha -= 5;
	}
	if( key == '.'){
		brushAlpha += 5;
	}
	if( key == 't' ){
		bUseTextures =!bUseTextures;
	}
	if( key == 'e' ){
		bEraser = !bEraser;
	}
	brushAlpha = ofClamp(brushAlpha, 0, 255);
	brushSize = ofClamp(brushSize, 10, 9999);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	bBrushDown = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bBrushDown = false;
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
