#pragma once
#include "ofMain.h"

#define SPRING_CONSTANT 0.1f
#define MAX_VELOCITY 30.0f

// This 'swarm' object demonstrates a simple particle system
//  with 'simple harmonic motion'
class swarm : public ofNode {
	struct particle {
		glm::vec3 position;
		glm::vec3 velocity;
		ofColor color;
	};

	public:
		swarm();
		void init(int nParticles, float positionDispersion, float velocityDispersion);
		void customDraw();
		ofLight light;
		ofMaterial material;

	protected:
		// we call this update function ourselves
		//  at the beginning of customDraw
		void update();

		// objects
		vector<particle>particles;
};
