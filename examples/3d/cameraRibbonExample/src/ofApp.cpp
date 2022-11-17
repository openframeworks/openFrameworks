/**
 *
 * OFDevCon Example Code Sprint
 * Camera Ribbon example
 * This example generates ribbons along the mouse trail that descend in space
 * When you click space bar, you can
 *
 * Created by James George for openFrameworks workshop at Waves Festival Vienna sponsored by Lichterloh and Pratersauna
 * Adapted during ofDevCon on 2/23/2012
 */


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//just set up the openFrameworks stuff
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(255);

	//initialize the variable so it's off at the beginning
	usecamera = false;
}

//--------------------------------------------------------------
void ofApp::update(){
	//don't move the points if we are using the camera
	if(!usecamera){
		glm::vec3 sumOfAllPoints(0,0,0);
		for(unsigned int i = 0; i < points.size(); i++){
			points[i].z -= 4;
			sumOfAllPoints += points[i];
		}
		center = sumOfAllPoints / points.size();
	}
	if( points.size() > 250 ) {
		points.erase(points.begin());
	}
	
	polyline.clear();
	polyline.addVertices( points );
	if( polyline.getPerimeter() > 5 ) {
		// this will provide an even distance between points on the line
//		polyline = polyline.getResampledBySpacing(5);
		
		polyline = polyline.getSmoothed(smoothingSize);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){


	//if we're using the camera, start it.
	//everything that you draw between begin()/end() shows up from the view of the camera
	if(usecamera){
		camera.begin();
	}
	
	auto verts = points;
	if(usePolyline) {
		verts = polyline.getVertices();
	}
	
	ofSetColor(0);
	//do the same thing from the first example...
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	for(unsigned int i = 1; i < (int)verts.size(); i++){

		//find this point and the next point
		glm::vec3 thisPoint = verts[i-1];
		glm::vec3 nextPoint = verts[i];
		if( i < verts.size()-1 ) {
			nextPoint = verts[i+1];
		}

		//get the direction from one to the next.
		//the ribbon should fan out from this direction
		glm::vec3 direction = (nextPoint - thisPoint);

		//get the distance from one point to the next
		float distance = glm::length(direction);

		//get the normalized direction. normalized vectors always have a length of one
		//and are really useful for representing directions as opposed to something with length
		glm::vec3 unitDirection = glm::normalize(direction);

		//find both directions to the left and to the right
		glm::vec3 toTheLeft =  glm::rotate(unitDirection, -90.f, glm::vec3(0,0,1));
		glm::vec3 toTheRight = glm::rotate(unitDirection, 90.f, glm::vec3(0,0,1));

		//use the map function to determine the distance.
		//the longer the distance, the narrower the line.
		//this makes it look a bit like brush strokes
		float thickness = ofMap(distance, 0, 60, 20, 2, true);
		
		// calculate a taper based on the index
		float indexPct = 1.0f;
		if( i < 25 ) {
			indexPct = ofMap( i, 0, 25, 0.0, 1.0, true );
		}
		thickness *= indexPct;

		//calculate the points to the left and to the right
		//by extending the current point in the direction of left/right by the length
		glm::vec3 leftPoint = thisPoint+toTheLeft*thickness;
		glm::vec3 rightPoint = thisPoint+toTheRight*thickness;

		//add these points to the triangle strip
		mesh.addVertex(glm::vec3(leftPoint.x, leftPoint.y, leftPoint.z));
		mesh.addVertex(glm::vec3(rightPoint.x, rightPoint.y, rightPoint.z));
	}

	//end the shape
	mesh.draw();


	//if we're using the camera, take it away
	if(usecamera){
		camera.end();
	}
	
	ofSetColor( 40 );
	string outString = "Move the mouse to add points to the line";
	outString += "\nUse camera view(spacebar): "+ofToString(usecamera);
	outString += "\nUse polyline for smoothing(p): "+ofToString(usePolyline);
	outString += "\nPolyline smoothing size(up/down): "+ofToString(smoothingSize);
	ofDrawBitmapString( outString, 20, 20 );

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == ' ' ) {
		//hitting spacebar swaps the camera view
		usecamera = !usecamera;
	}
	if( key == 'p' ) {
		usePolyline = !usePolyline;
	}
	
	if( key == OF_KEY_UP ) {
		smoothingSize++;
	}
	if( key == OF_KEY_DOWN ){
		smoothingSize--;
		if( smoothingSize < 1 ){
			smoothingSize=1;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	//if we are using the camera, the mouse moving should rotate it around the whole sculpture
	if(usecamera){
		float rotateAmount = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 360);
		glm::vec3 furthestPoint;
		if (points.size() > 0) {
			furthestPoint = points[0];
		}
		else
		{
			furthestPoint = glm::vec3(x, y, 0);
		}

		glm::vec3 directionToFurthestPoint = (furthestPoint - center);
		glm::vec3 directionToFurthestPointRotated = glm::rotate(directionToFurthestPoint, ofDegToRad(rotateAmount), glm::vec3(0,1,0));
		camera.setPosition(center + directionToFurthestPointRotated);
		camera.lookAt(center);
	}
	//otherwise add points like before
	else{
		glm::vec3 mousePoint(x,y,0);
		points.push_back(mousePoint);
		
	}
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
