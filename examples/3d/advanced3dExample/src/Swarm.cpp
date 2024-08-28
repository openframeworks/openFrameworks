#include "Swarm.h"

// This 'swarm' object demonstrates a simple particle system
//  with 'simple harmonic motion'

swarm::swarm(){
	light.setAmbientColor(ofColor(0, 0, 0));
}

void swarm::init(int nParticles, float positionDispersion, float velocityDispersion){

	// Check if we've already initialised
	if(particles.size() != 0){
		// clear out old data
		ofLogWarning("swarm") <<  "Swarm: Already initialised";

		particles.clear();
	}

	glm::vec3 position, velocity;
	ofColor color;
	for(int i = 0; i < nParticles; i++){
		position.x = (ofRandom(1.0f) - 0.5f)  * positionDispersion;
		position.y = (ofRandom(1.0f) - 0.5f)  * positionDispersion;
		position.z = (ofRandom(1.0f) - 0.5f)  * positionDispersion;

		velocity.x = (ofRandom(1.0f) - 0.5f)  * velocityDispersion;
		velocity.y = (ofRandom(1.0f) - 0.5f)  * velocityDispersion;
		velocity.z = (ofRandom(1.0f) - 0.5f)  * velocityDispersion;

		color.r = ofRandom(255.0f);
		color.g = ofRandom(255.0f);
		color.b = 150.0f;
		color.a = 255.0f;

		particle newParticle;
		newParticle.position = position;
		newParticle.velocity = velocity;
		newParticle.color = color;

		// add our new particle to the vector
		particles.push_back(newParticle);
	}

}

void swarm::customDraw(){
	// We run the update ourselves manually. ofNode does
	//  not do this for us.
	update();



	//--
	// Draw particles

	// We use the position of the first
	//  particle as the position of the
	//  light.
	ofPushStyle();
	light.enable();
	light.setPosition(particles[0].position);

	for(unsigned int i = 0; i < particles.size(); i++){
		//ofPushStyle();
		//ofSetColor(particles[i].color);
		material.setDiffuseColor(particles[i].color);

		material.begin();
		ofDrawSphere(particles[i].position, 1.0);

		//ofPopStyle();
	}
	material.end();

	light.disable();
	ofDisableLighting();

	//
	//--



	// Render light as white sphere
	ofSetColor(255, 255, 255);
	ofDrawSphere(light.getPosition(), 2.0);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
	ofDrawBitmapString(" light", particles[0].position);
	ofPopStyle();
}

void swarm::update(){

	// Calculate time past per frame
	float dt = ofGetLastFrameTime();

	// Update positions, velocities
	for(unsigned int i = 0; i < particles.size(); i++){

		// -----------
		//
		//	MOTION MATHS
		//
		//		'Simple Harmonic Motion' + a little extra
		// ----
		//

		// [1] apply velocity to postion
		//  (i.e. integrate velocity)
		//
		//  v = dx / dt (*)
		//  x = x + dx [every frame]
		//
		// therefore
		//  x = x + v * dt (*)
		//

		// (velcotity is taken from previous frame)
		particles[i].position += particles[i].velocity * dt;



		// [2] apply spring force to velocity
		//  (i.e. integrate acceleration)
		//
		//  a = -k * x (this is the shm restoring force, aka spring force)
		//  a = dv / dt
		//
		// therefore from (*)s above
		//  (v = v + dv)
		//
		//  v = v + (dt * a)
		//  v = v + (dt * -k * x)
		//
		particles[i].velocity += -SPRING_CONSTANT * particles[i].position * dt;


		// [3] to get a super simple kind of 'flocking' behaviour
		//  we add a second spring force to velocity relative
		//  to the position of the light
		// NOTICE: THIS ISN'T REAL FLOCKING!
		particles[i].velocity += -SPRING_CONSTANT * (particles[i].position - light.getPosition()) * dt;


		// [4] Force a maximum velocity
		if(glm::length(particles[i].velocity) > MAX_VELOCITY){
			particles[i].velocity /= glm::length(particles[i].velocity) * MAX_VELOCITY;
		}

		//
		// -----------

	}
}
