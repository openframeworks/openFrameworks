/**
 *
 * OFDevCon Example Code Sprint
 * Quaternion Example for plotting latitude and longitude onto a sphere
 *
 * Created by James George on 2/23/2012
 * Updated by Nick Hardeman on 9/20/2022 :)
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofEnableAlphaBlending();
//	ofNoFill();

	camera.setFarClip(5000);
	camera.move(0,0,1000);

	// create little objects for each city.
	// A Lat/Lon like this:
	// Lewiston, Idaho 	W 46 24' N 117 2'
	// This translate to angles and degrees (1 degree = 1/60. of an angle)
	// West and South are negative values

	// here is a list of big cities and their positions
	// http://www.infoplease.com/ipa/A0001796.html

	City newyork = { "new york", 40+47/60., -73 + 58/60. };
	City tokyo = { "tokyo", 35 + 40./60, 139 + 45/60. };
	City london = { "london", 51 + 32/60., -5./60. };
	City shanghai = { "shanghai", 31 + 10/60., 121 + 28/60. };
	City buenosaires = { "buenos aires", -34 + 35/60., -58 + 22/60. };
	City melbourne = { "melbourne" , -37 + 47/60., 144 + 58/60. };
	City detroit = { "detroit", 42 + 19/60., -83 + 2 / 60. };

	cities.push_back( newyork );
	cities.push_back( tokyo );
	cities.push_back( london );
	cities.push_back( shanghai );
	cities.push_back( buenosaires );
	cities.push_back( melbourne );
	cities.push_back( detroit );

	earthMesh = ofMesh::icosphere(earthRadius, 3);

	ofDisableArbTex();
	ofLoadImage( earthTexture, "earth.jpg" );
	ofEnableArbTex();

	flyToNextCity();

	starsMesh.setMode(OF_PRIMITIVE_POINTS);
	// now lets add some random points //
	int numberOfStars = 200;
	for( int i = 0; i < numberOfStars; i++ ) {

		float randomLatitude = ofRandom( -360, 360 );
		float randomLongitude = ofRandom( -180, 180 );

		float randomDistance = earthRadius + ofRandom( 100, 1000);

		glm::vec3 starPosition = getPositionFromLatitudeLongitude(randomLatitude, randomLongitude, randomDistance );
		starsMesh.addVertex(starPosition);

	}
}

//--------------------------------------------------------------
void ofApp::update(){
	float deltaTime = ofClamp( ofGetLastFrameTime(), 1.f / 5000.f, 1.f / 5.f );

	earthSpin += deltaTime * 3.0;
	earthSpin = ofWrapDegrees(earthSpin);

	flyingObj.percentToNextCity += deltaTime / flyingObj.durationToNextCity;

	if( flyingObj.percentToNextCity >= 1.0f ) {
		flyToNextCity();
	}


	City currentCity = cities[currentCityIndex];
	// `auto nextCity` is the same as using `City nextCity` when assigning a variable
	auto nextCity = cities[nextCityIndex];

	glm::vec3 currentPosition = getPositionFromLatitudeLongitude(currentCity.latitude, currentCity.longitude, earthRadius );
	auto nextPosition = getPositionFromLatitudeLongitude( nextCity.latitude, nextCity.longitude, earthRadius );

	float flyLatitude = ofLerpDegrees( currentCity.latitude, nextCity.latitude, flyingObj.percentToNextCity );
	float flyLongitude = ofLerpDegrees( currentCity.longitude, nextCity.longitude, flyingObj.percentToNextCity );

	// lets push out the flying object a bit //
	float offsetDistance = sinf(flyingObj.percentToNextCity * glm::pi<float>() ) * flyingObj.distanceBetweenCities * 0.6f;

	flyingObj.setPosition(getPositionFromLatitudeLongitude( flyLatitude, flyLongitude, earthRadius + offsetDistance ));

	if( flyingObj.trailPositions.size() < 1 ){
		flyingObj.trailPositions.push_back(flyingObj.getPosition());
	} else {
		// tell the node to look at the previous position to get orientation
		// we pass in the normalized position of the flyingObj because it is relative to the center of the sphere
		// the second argument is the up vector to orient around
		flyingObj.lookAt(flyingObj.trailPositions.back(), glm::normalize(flyingObj.getPosition()) );
		// lets check the distance to the last point, so we don't add a point every frame
		glm::vec3 lastPoint = flyingObj.trailPositions.back();
		float distanceToLastPoint = glm::distance( flyingObj.getPosition(), lastPoint );
		if( distanceToLastPoint > 20.0 ) {
			flyingObj.trailPositions.push_back( flyingObj.getPosition() );
		}
	}

	// make sure we don't add trail points forever and ever
	// if the vector is larger than 20 points, erase the first one
	if( flyingObj.trailPositions.size() > 20 ){
		flyingObj.trailPositions.erase( flyingObj.trailPositions.begin() );
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor(10), ofColor(50));

	ofEnableDepthTest();

	camera.begin();

	ofSetColor( 220 );
	#ifndef TARGET_EMSCRIPTEN
		glPointSize(3.0f);
	#endif
	starsMesh.draw();

	ofPushMatrix();
	//add the earthSpin
	ofRotateDeg(earthSpin, 0, 1, 0);
	ofSetColor(150);
	earthTexture.bind();
	earthMesh.draw();
	earthTexture.unbind();
	ofPopMatrix();

	ofSetColor(255);
	for(unsigned int i = 0; i < cities.size(); i++){

		float extraRadius = 10.;
		if( i == currentCityIndex ){
			extraRadius = ofMap( flyingObj.percentToNextCity, 0.2f, 0.9f, 60.0f, 10.0, true );
		} else if( i == nextCityIndex ){
			ofSetColor(255);
			extraRadius = ofMap( flyingObj.percentToNextCity, 0.2f, 0.9f, 10.0f, 60.0, true );
		}else{
			ofSetColor(155);
		}

		auto worldPoint = getPositionFromLatitudeLongitude( cities[i].latitude, cities[i].longitude, earthRadius+extraRadius );

		//draw it and label it
		ofDrawLine(glm::vec3(0,0,0), worldPoint);

		//set the bitmap text mode billboard so the points show up correctly in 3d
		ofDrawBitmapString(cities[i].name, worldPoint );
	}

	ofSetColor(255);
	flyingObj.draw();
	// uncomment to draw the axis of the flying object
//	flyingObj.transformGL();
//	ofDrawAxis(30);
//	flyingObj.restoreTransformGL();

	camera.end();

	ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::flyToNextCity() {
	currentCityIndex++;
	if( currentCityIndex >= cities.size() ) {
		currentCityIndex = 0;
	}
	nextCityIndex = currentCityIndex+1;
	if( nextCityIndex >= cities.size() ) {
		nextCityIndex = 0;
	}

	auto& currentCity = cities[currentCityIndex];
	auto& nextCity = cities[nextCityIndex];

	glm::vec3 currentPosition = getPositionFromLatitudeLongitude( currentCity.latitude, currentCity.longitude, earthRadius );
	auto nextPosition = getPositionFromLatitudeLongitude( nextCity.latitude, nextCity.longitude, earthRadius );

	flyingObj.percentToNextCity = 0.f;
	float distanceToFly = glm::distance( currentPosition, nextPosition );
	flyingObj.durationToNextCity = distanceToFly / 200.0;
	ofLogNotice() << "flyingObj.durationToNextCity : " << flyingObj.durationToNextCity << " current: " << currentCityIndex << " next: " << nextCityIndex << " total: " << cities.size() << endl;
	if( flyingObj.durationToNextCity < 1.0 ) {
		flyingObj.durationToNextCity = 1.0;
	}
	flyingObj.distanceBetweenCities = distanceToFly;
}

//--------------------------------------------------------------
glm::vec3 ofApp::getPositionFromLatitudeLongitude( float alatitude, float alongitude, float adistance ) {
	//three rotations
	//two to represent the latitude and lontitude of the city
	//a third so that it spins along with the spinning sphere
	glm::quat latRot, longRot, spinQuat;
	// we pass in negative latitude to align along the y axis,
	latRot = glm::angleAxis(ofDegToRad(-alatitude), glm::vec3(1, 0, 0));
	longRot = glm::angleAxis(ofDegToRad(alongitude), glm::vec3(0, 1, 0));
	// we add 90 to the rotation so that it lines up with the texture
	spinQuat = glm::angleAxis(ofDegToRad(90+earthSpin), glm::vec3(0, 1, 0));

	//our starting point is 0,0, on the surface of our sphere, this is where the meridian and equator meet
	glm::vec3 center = glm::vec3(0.0f,0.f,adistance);
	//multiplying a quat with another quat combines their rotations into one quat
	//multiplying a quat to a vector applies the quat's rotation to that vector
	//so to to generate our point on the sphere, multiply all of our quaternions together then multiple the center by the combined rotation
	glm::vec3 worldPoint = spinQuat * longRot * latRot * center;
	return worldPoint;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
