#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetBackgroundColor(250);
	
    ofSetFrameRate( 60 );
	svg.load("ofLogoDesserts.svg");
	// Print the svg structure to the console.
    ofLogNotice("Svg Structure") << std::endl << svg.toString();
	
	// get all the paths in the Donut -> Sprinkles group
	auto sprinklePaths = svg.getElementsForType<ofxSvgPath>("Donut:Sprinkles");
	// A wildcard (*) is also acceptable. Allowing traversal of groups until the group with the name after the colon is found.
	// In this case "Sprinkles"
//	auto sprinklePaths = svg.getElementsForType<ofxSvgPath>("*:Sprinkles");
	
	for( auto& sprinklePath : sprinklePaths ) {
		// Grab and store the first polyline in the path so we can set it on the svg element later.
		auto spoly = sprinklePath->getFirstPolyline();
		if( spoly.size() > 2 ) {
			// get the center point from the bounding box
			auto centerPos = spoly.getBoundingBox().getCenter();
			sprinklePath->setPosition(centerPos);
			
			sprinklePath->getPath().clear();
			int counter = 0;
			
			// now lets convert the polyline to be local around the center point.
			for( auto& vert : spoly.getVertices() ) {
				vert -= centerPos;
				if( counter < 1 ) {
					sprinklePath->getPath().moveTo(vert);
				} else {
					sprinklePath->getPath().lineTo(vert);
				}
				counter++;
			}
			
			if( spoly.isClosed() ) {
				sprinklePath->getPath().close();
			}
			// Store in a vector so that we can manipulate these later.
			mSprinkles.push_back(sprinklePath);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
    
	// store the elapsed time in a variable since it can be an expensive operation in a loop.
	float etimef = ofGetElapsedTimef();
	int counter = 0;
	for( auto& sprinkle : mSprinkles ) {
		if( ofGetMousePressed() ) {
			// Store the difference from the sprinkle position to the mouse position.
			auto diff = glm::normalize(glm::vec2( ofGetMouseX(), ofGetMouseY() ) - glm::vec2( sprinkle->getPosition().x, sprinkle->getPosition().y ));
			// Convert to a angle rotation in degrees.
			float targetRotation = glm::degrees(atan2f( diff.y, diff.x ));
			// Lerp to the target rotation.
			// Calling ofLerpDegrees handles wrapping and edge cases when using degrees.
			sprinkle->setRotationDeg(ofLerpDegrees(sprinkle->getRotationDeg(), targetRotation, 0.1f ));
			sprinkle->setFillColor(ofColor(255));
		} else {
			// Store the current rotation.
			float sprinkleRotation = sprinkle->getRotationDeg();
			// Rotate a small amount based on cos and the sprinkle x position.
			float rotationAmount = 2.f * ofClamp( cosf( sprinkle->getPosition().x * 0.1f + etimef ), -0.1f, 1.f);
			sprinkle->setRotationDeg(sprinkleRotation+rotationAmount);
			ofColor tcolor( 101,163,253 );
			// Change the hue a bit for a color changing effect.
			tcolor.setHue( tcolor.getHue() + (sin(etimef*0.5f + counter * 2.f)) * 50.f );
			sprinkle->setFillColor(tcolor);
		}
		counter++;
	}
    
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	auto wrect = ofRectangle(0,0,ofGetWidth(), ofGetHeight());
	auto srect = svg.getBounds();
	// scale the bounds of the svg to fit within the window rectangle
	srect.scaleTo(wrect, OF_SCALEMODE_FIT);
	
	ofPushMatrix(); {
		ofTranslate(srect.x, srect.y);
		ofScale( srect.getWidth() / svg.getBounds().getWidth() );
		svg.draw();
		
		ofSetColor( 120 );
		ofNoFill();
		ofDrawRectangle( svg.getBounds() );
		ofFill();
	} ofPopMatrix();
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
