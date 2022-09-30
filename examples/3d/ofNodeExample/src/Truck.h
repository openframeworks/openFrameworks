
#pragma once

#include "ofMain.h"

class Truck{
	public:

		void setup();
		void update();
		void draw();

		void steer( float adir );
		void brake(float amnt);
		void accelerate(float amnt);
		const ofNode & getNode() const;

		void resetPosition();

		ofParameterGroup params;

	private:
		ofParameter<float> wheelXAxisOffset;
		ofParameter<float> wheelYAxisOffset;
		ofParameter<float> frontWheelZAxisOffset;
		ofParameter<float> backWheelZAxisOffset;

		ofParameter<float> lightXAxisOffset;
		ofParameter<float> lightYAxisOffset;
		ofParameter<float> lightZAxisOffset;

		ofParameter<float> logoXAxisOffset;
		ofParameter<float> logoYAxisOffset;
		ofParameter<float> logoZAxisOffset;

		float velocity = 0.0;
		float acceleration = 0.0;
		float localTurnAngleAroundYAxis = 0.;

		ofMaterial bodyMaterial;
		ofVboMesh bodyMesh;
		ofMaterial wheelMaterial;
		ofVboMesh wheelMesh;
		ofVboMesh logoMesh;
		ofMaterial logoMaterial;

		// everything is attached to this node
		ofNode node;
		ofNode bodyNode;
		ofNode logoNode;

		vector<ofNode> wheelNodes;
		float wheelRotationAroundXAxis = 0.0f;

		ofLight headlightL;
		ofLight headlightR;

		ofSoundPlayer hornSoundPlayer;
		float timeSinceSpacebarPress = 10.0;
		ofSoundPlayer reverseSoundPlayer;
		float reverseVolume = 0.f;
		ofSoundPlayer engineSoundPlayer;
		float engineVolume = 0.0f;
		ofParameter<bool> bUseSound;
};
