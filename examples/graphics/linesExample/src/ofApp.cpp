#include "ofApp.h"
#include "ofGLProgrammableRenderer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(false);
	ofSetFrameRate(-1);
	
	ofSetCircleResolution(100);
	
	font.load("verdana.ttf", 64, true, true, true);
	// store a vector of ofPaths that represent the characters in the string.
	textPaths = font.getStringAsPoints("openFrameworks", false, false);
	
	// disable arb textures so that the tex coords are from 0 -> 1
	ofDisableArbTex();
	ofLoadImage(texture, "tex.png");
	// since we have disabled arb textures, we can set the texture to repeat
	texture.setTextureWrap( GL_REPEAT, GL_CLAMP );
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetLineWidth(lineWidth);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofFloatColor(1.0), ofFloatColor(0.8));
	ofSetColor( 60 );
	std::stringstream ss;
	ss << "Line width (+/-): " << ofToString(lineWidth, 1) ;
	ss << std::endl << "Line colors (c): " << (bUseLineColors ? "yes" : "no");
	ss << std::endl << "Texture line (t): " << (bLineTexture ? "yes" : "no");
	ofDrawBitmapString( ss.str(), 24, 36 );
	
	if( ofIsGLProgrammableRenderer() ) {
		auto rend = std::dynamic_pointer_cast<ofGLProgrammableRenderer>(ofGetGLRenderer());
		// comment out the line below to see how openGL renders lines
		// with the ofGLProgrammableRenderer and line shaders disabled, there is no line width
		//	rend->disableLinesShaders();
		//	rend->enableLinesShaders();
		
		// by default, lines are rendered in screen space, so further lines will have the
		// same thickness as those closer to the camera
		// comment out the below to enable size attenuation on the lines.
		// notice in the box mesh moving up and down, the lines further from the camera appear thinner.
		//	rend->enableLineSizeAttenuation();
	}
	
	// lets store a reference to elapsed time to use later.
	float elapsedTime = ofGetElapsedTimef();
	
	
	ofSetColor( 60 );
	// we can push the style so that line width changes, color changes and other style changes
	// will only remain between the push and pop of the style
	ofPushStyle();
	ofSetLineWidth(std::max(1.f, lineWidth/4.f));
	
	float rectRadius = 10.f + (1.f-std::abs(std::sinf(elapsedTime))) * 80.f;
	ofRectangle outlineRect(200, 150, ofGetWidth()-400, ofGetHeight()-300);
	
	ofPath testRectPath;
	testRectPath.setCurveResolution(16);
	testRectPath.setCircleResolution(120);
	testRectPath.rectRounded(outlineRect, rectRadius);
	testRectPath.setFilled(false);
	testRectPath.setFillColor(ofColor::lightBlue);
	testRectPath.setStrokeWidth(2.);
	testRectPath.setStrokeColor(ofColor( 60 ));
	testRectPath.draw();
	ofPopStyle();
	
	// enable depth test because we will be drawing in 3d space //
	ofEnableDepthTest();
	ofPushMatrix(); {
		ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2);
		int numPts = 150;
		float height = 200.0f;
		ofMesh lineMesh;
		// set the mode to render lines
		lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
		ofFloatColor lineColor;
		for( int i = 0; i < numPts; i++ ) {
			float fi = (float)i;
			float fpct = fi / (float)numPts;
			glm::vec3 pt = glm::vec3(// magic numbers! :) move the vertices around with sin and cos
									 std::cosf(fi * 0.15f + elapsedTime ) * (fpct+0.1f) * height * 0.5,
									 std::sinf(fi * 0.08 + elapsedTime) * height * 0.5 + height * (fpct * 2.0f-1.0f) * 0.5,
									 std::cosf(fi * -0.05f + elapsedTime) * height * 0.25f
									 );
			lineMesh.addVertex(pt);
			if(bUseLineColors) {
				lineColor.setHsb(fpct, 0.8f, 0.85f);
				lineMesh.addColor(lineColor);
			}
			if(bLineTexture) {
				// if we are going to bind a texture to this mesh
				// we need to add coordinates for the texture to map to.
				// out texture is non-arb, so the coordinates for the texture are from 0 -> 1.
				// the texture has been set to repeat in x, so we can go over 1 to repeat.
				// the renderer adds vertices to render lines that are thicker.
				// the y texture coordinate will be set at 0.0f or the height of the texture by the renderer.
				lineMesh.addTexCoord( glm::vec2(fpct * texture.getWidth(), 0.0f ));
			}
		}
		
		if(bLineTexture) {
			ofPushStyle();
			ofSetColor(255);
			texture.bind();
		}
		
		lineMesh.draw();
		
		if(bLineTexture) {
			ofPopStyle();
			texture.unbind();
		}
		
		
		ofMesh boxMesh;
		boxMesh.setMode( OF_PRIMITIVE_LINE_LOOP);
		
		float bsize = height * 0.8f;
		boxMesh.addVertex(glm::vec3(-bsize, -bsize, 0.f));
		boxMesh.addVertex(glm::vec3(bsize, -bsize, 0.f));
		boxMesh.addVertex(glm::vec3(bsize, bsize, 0.f));
		boxMesh.addVertex(glm::vec3(-bsize, bsize, 0.f));
		
		if( !bUseLineColors ) {
			// lets add some colors to our mesh
			// one for each vertex
			boxMesh.addColor(ofColor::magenta);
			boxMesh.addColor(ofColor::yellow);
			boxMesh.addColor(ofColor::green);
			boxMesh.addColor(ofColor::blueSteel);
		}
		
		boxMesh.addIndex(0);
		boxMesh.addIndex(1);
		boxMesh.addIndex(2);
		boxMesh.addIndex(3);
		// OF_PRIMITIVE_LINE_LOOP draws a line between the first and last point
		// so we don't need to add an index for that.
		
		ofPushMatrix(); {
			ofTranslate(0.0, height * 0.5 * std::sinf(elapsedTime) );
			ofRotateXRad( glm::half_pi<float>() );
			boxMesh.draw();
		} ofPopMatrix();
	} ofPopMatrix();
	
	// disable depth test because we will only be drawing in 2d
	ofDisableDepthTest();
	
	
	
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() - 48 );
	
	string textStringToRender = "openFrameworks";
	auto fontRect = font.getStringBoundingBox(textStringToRender, 0.0f, 0.f);
	
	
	
	ofPushMatrix(); {
		ofTranslate(-fontRect.getWidth() * 0.5f, 0.f );
		float fi = 0.f;
		ofFloatColor floatColor;
		floatColor.setHex(0xd3ffd3);
		floatColor = ofColor::lightGray;
		// draw each one of the character paths that we grabbed from the font
		// in the setup function
		for( auto& fp : textPaths ) {
			fp.setFilled(true);
			float cs = std::max(std::cosf(fi*0.35f - elapsedTime), 0.f);
			fp.setFillColor(floatColor);
			fp.setStrokeColor(ofColor( 60 ).getLerped(ofColor::seaGreen, cs));
			// line width does not apply to paths, since paths have a stroke width property
			// so change the width that the paths render by setting it on the path
			fp.setStrokeWidth(2.f + std::max(lineWidth*0.25f*std::cosf(fi*0.25f - elapsedTime), 0.f));
			fi += 1.f;
			fp.draw();
		}
	} ofPopMatrix();
	
//	// we can render the entire string by calling font.drawString which uses textures to render the font
//	ofPushStyle();
//	ofSetHexColor(0xd3ffd3);
//	font.drawString(textStringToRender, -fontRect.getWidth() * 0.5f, 0.f );
//	ofPopStyle();
//	
//	ofPushStyle();
//	// we can render the outlines of the entire string by calling drawStringAsShapes
//	// this will be affected by line width
//	ofSetLineWidth(2);
//	font.drawStringAsShapes(textStringToRender,  -fontRect.getWidth() * 0.5f, 0.f );
//	ofPopStyle();
	
	// now lets draw some circles
	ofPushStyle();
	ofNoFill();
	ofSetLineWidth( std::max(1.f, lineWidth*0.5f));
	ofSetColor( ofColor::seaGreen );
	ofDrawCircle(-fontRect.getWidth() * 0.5f - 80, -25, 25 );
	ofDrawCircle(fontRect.getWidth() * 0.5f + 80, -25, 25 );
	ofPopStyle();
	
	ofPopMatrix();
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == '+' || key == '=') {
		lineWidth += 1.f;
	}
	if( key == '-' || key == '_') {
		lineWidth -= 1.f;
	}
	if( key == 'c' ) {
		bUseLineColors = !bUseLineColors;
	}
	if( key == 't' ) {
		bLineTexture = !bLineTexture;
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
