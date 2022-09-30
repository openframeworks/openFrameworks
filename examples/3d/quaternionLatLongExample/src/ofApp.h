#pragma once

#include "ofMain.h"

typedef struct {
	string name;
	float latitude;
	float longitude;
} City;

class FlyingObject : public ofNode {
public:

	void draw() {
		ofFloatColor color1 = ofColor::lightGoldenRodYellow;
		ofFloatColor color2 = ofColor::tomato;
		for( size_t i = 0; i < trailPositions.size(); i++ ) {
			float drawPercent = ofMap( i, 0, trailPositions.size(), 0.0, 1, true );
			ofFloatColor drawColor = color1.getLerped( color2, 1.f-drawPercent );
			ofSetColor( drawColor );
			ofDrawSphere( trailPositions[i], 5.0f * (drawPercent+0.1f) );
		}

		transformGL();
		ofSetColor( 255 );
		// draw a body
		ofDrawBox(0, 0, 0, 8, 4, 24);
		// draw some wings
		ofDrawTriangle(glm::vec3(-4, 0, 12), glm::vec3(-20, 0, -10), glm::vec3(-4, 0, -10) );
		ofDrawTriangle(glm::vec3(4, 0, 12), glm::vec3(20, 0, -10), glm::vec3(4, 0, -10) );
		restoreTransformGL();
	}

	vector<glm::vec3> trailPositions;

	float speed = 1.0;
	float durationToNextCity = 3.0f;
	float percentToNextCity = 0.0;
	float distanceBetweenCities = 100.f;
};

class ofApp : public ofBaseApp{

  public:
	void setup();
	void update();
	void draw();

	void flyToNextCity();
	glm::vec3 getPositionFromLatitudeLongitude( float alatitude, float alongitude, float adistance );

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	vector<City> cities;
	ofEasyCam camera;
	FlyingObject flyingObj;

	float earthRadius = 300.f;
	float earthSpin = 0.0;

	int currentCityIndex = 0;
	int nextCityIndex = 1;

	ofMesh earthMesh;
	ofTexture earthTexture;

	ofMesh starsMesh;
};
