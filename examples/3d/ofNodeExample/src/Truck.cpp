
#include "Truck.h"

//--------------------------------------------------------------
void Truck::setup(){
	ofFloatColor teal = ofFloatColor::teal;
	teal.a = 0.4;
	bodyMaterial.setAmbientColor(teal);
	teal.a = 1.0;
	bodyMaterial.setDiffuseColor(teal);
	bodyMaterial.setShininess(10);

	wheelMaterial.setDiffuseColor(ofFloatColor(0.1));

	logoMaterial.setAmbientColor(ofFloatColor(0.85, 0.16, 0.43, 0.4));
	logoMaterial.setDiffuseColor(ofFloatColor(0.85, 0.16, 0.43, 1.0));
	logoMaterial.setSpecularColor(ofFloatColor(1.0,1.0,1.0,1.0));
	logoMaterial.setShininess(10);

	bodyMesh.load("truck.ply");
	wheelMesh.load("wheel.ply");
	logoMesh.load("oflogo.ply");

	node.setScale(100.0);

	// append the bodyNode to the main node that we will move around
	bodyNode.setParent( node );
	// the bodyNode is now a child of node.

	// assign 4 nodes to control the wheels in the vector
	wheelNodes.assign(4, ofNode());
	// wheel 0 and 1 are the front two wheels
	// wheel 2 and 3 are the rear wheels

	// parent all of the nodes to the body node
	for( int i = 0; i < wheelNodes.size(); i++ ){
		wheelNodes[i].setParent( node );
	}

	params.setName("Truck");
	// create some parameters to control the placement of the wheels
	// since the wheels are parented to the primary node, all of the positions
	// are local, relative to the main node.
	params.add(wheelXAxisOffset.set("WheelXAxisOffset", 0.92, 0, 2));
	params.add(wheelYAxisOffset.set("WheelYAxisOffset", 0, -1, 1));
	params.add(frontWheelZAxisOffset.set("FrontWheelZAxisOffset", -0.78, -2, 4));
	params.add(backWheelZAxisOffset.set("BackWheelZAxisOffset", 2.5, 0, 4));


	// set the headlights to be children of the bodyNode.
	// the hierarchy is node > bodyNode > headLightL
	// the hierarchy is node > bodyNode > headLightR

	headlightL.setParent(bodyNode);
	headlightR.setParent(bodyNode);

	headlightL.tiltDeg(-20);
	headlightR.tiltDeg(-20);
	headlightL.setSpotlight();
	headlightR.setSpotlight();
	headlightL.setDiffuseColor(ofFloatColor::white);
	headlightR.setDiffuseColor(ofFloatColor::white);
	headlightL.setup();
	headlightR.setup();

	// add some parameters to control the position of the headlights
	// the position is relative to the x,y,z of the bodyNode
	params.add(lightXAxisOffset.set("LightXAxisOffset", 0.8, 0.0, 2.0));
	params.add(lightYAxisOffset.set("LightYAxisOffset", 1, 0, 2));
	params.add(lightZAxisOffset.set("LightZAxisOffset", -2, -3, 0));

	// add some parameters to control the position of the logo node
	// the position is relative to the x,y,z of the bodyNode
	logoNode.setParent(bodyNode);
	// scale down the logo quite a bit
	logoNode.setScale(0.005);
	params.add(logoXAxisOffset.set("LogoXAxisOffset", -0.57, -1, 1));
	params.add(logoYAxisOffset.set("LogoYAxisOffset", 0.45, -5, 5));
	params.add(logoZAxisOffset.set("LogoZAxisOffset", 2.25, 0, 3));
	// pan and tilt the node so it appears to be in the bed of the truck
	logoNode.panDeg(-90);
	logoNode.tiltDeg(-90);

	// add a toggle to use sound, disabled by default
	params.add(bUseSound.set("UseSound", false ));
	// lets add some sounds
	hornSoundPlayer.load("horn.mp3");
	hornSoundPlayer.setVolume(0.5);
	reverseSoundPlayer.load("reverse.mp3");
	reverseSoundPlayer.setVolume(0.0);
	#ifndef TARGET_EMSCRIPTEN
		reverseSoundPlayer.setLoop(true);
	#endif

	engineSoundPlayer.load("engine.wav");
	#ifndef TARGET_EMSCRIPTEN
		engineSoundPlayer.setLoop(true);
	#endif
	engineSoundPlayer.setVolume(0.0);

}

//--------------------------------------------------------------
void Truck::update(){
	// deltaTime is how much time has passed since update() was last called
	float deltaTime = ofClamp(ofGetLastFrameTime(), 1.f/5000.f, 1.f/5.f);


	if( ofGetKeyPressed(OF_KEY_UP) ){
		accelerate(deltaTime * 150.0f);
	}
	if( ofGetKeyPressed(OF_KEY_DOWN)){
		brake(deltaTime * 150.0f);
	}

	bool bSteering = false;
	if( ofGetKeyPressed(OF_KEY_RIGHT)){
		steer(-30*deltaTime);
		bSteering = true;
	}
	if(ofGetKeyPressed(OF_KEY_LEFT)){
		steer(30*deltaTime);
		bSteering = true;
	}

	if( !bSteering ){
		// straighten out the wheels if we are not trying to turn
		localTurnAngleAroundYAxis /= 1.f + deltaTime* 2.0f;
	}

	// turn the node around its local y axis for steering behavior
	node.panDeg(deltaTime * localTurnAngleAroundYAxis*0.4f * (velocity*0.7f));

	velocity += acceleration * deltaTime;
	auto zaxis = -node.getZAxis();
	node.move( zaxis * velocity );

	// add some damping to the velocity and acceleration to slow down the truck
	velocity /= 1.f + deltaTime * 1.8f;
	acceleration /= 1.f + deltaTime * 3.f;

	// check if the spacebar is pressed, this will play the horn sound
	if( ofGetKeyPressed(' ') ) {
		// we don't want the horn sound to play too frequently, so we use timeSinceSpacebarPress as a time counter
		// and if that time is greater than 0.15, we reset it and play the horn sound
		if(timeSinceSpacebarPress > 0.15 && bUseSound) {
			timeSinceSpacebarPress = 0.0;
			hornSoundPlayer.play();
		}
	} else {
		// we only increased the time counter for the horn when spacebar is not pressed
		// so you can not hold down spacebar and play the horn sound over and over
		timeSinceSpacebarPress += deltaTime;
	}


	if( !bUseSound ){
		reverseVolume=0.0;
		reverseSoundPlayer.setVolume( reverseVolume );
		if( reverseSoundPlayer.isPlaying() ) {
			reverseSoundPlayer.stop();
		}
	}else{
		if( !reverseSoundPlayer.isPlaying() ) {
			reverseSoundPlayer.play();
		}
		// are we going in reverse?
		if( velocity < -0.1 ) {
			// if so, increase the reverse sound volume
			reverseVolume += deltaTime * 2.0;
			if( reverseVolume > 0.5 ) {
				reverseVolume = 0.5;
			}
		} else {
			// otherwise, decrease the reverse sound volume
			reverseVolume -= deltaTime * 4.0;
			if( reverseVolume < 0.0 ) {
				reverseVolume = 0.0;
			}
		}
		reverseSoundPlayer.setVolume( reverseVolume );
	}

	if( bUseSound ){
		if( !engineSoundPlayer.isPlaying()){
			engineSoundPlayer.play();
		}
		// map the velocity to engine sound volume and speed
		engineVolume = ofMap(fabs(velocity), 1, 25, 0.05, 0.4, true );
		float pitch = ofMap(fabs(velocity), 5, 30, 1.0, 1.35, true );
		engineSoundPlayer.setSpeed(pitch);
		engineSoundPlayer.setVolume(engineVolume);
	}else{
		engineVolume=0.0f;
		engineSoundPlayer.setSpeed(1.0);
		engineSoundPlayer.setVolume(engineVolume);
		if( engineSoundPlayer.isPlaying()){
			engineSoundPlayer.stop();
		}
	}

	// position the wheels based on the slider values
	// the positions are local, based on the node axis system
	wheelNodes[0].setPosition(-wheelXAxisOffset, wheelYAxisOffset, frontWheelZAxisOffset );
	wheelNodes[1].setPosition(wheelXAxisOffset, wheelYAxisOffset, frontWheelZAxisOffset );

	wheelNodes[2].setPosition(-wheelXAxisOffset, wheelYAxisOffset, backWheelZAxisOffset );
	wheelNodes[3].setPosition(wheelXAxisOffset, wheelYAxisOffset, backWheelZAxisOffset );

	// lets add some rotation based on velocity so it looks like the wheels are rotating on the ground
	wheelRotationAroundXAxis += ofWrapDegrees(-velocity * 0.5f);

	glm::quat zrotation = glm::angleAxis( ofDegToRad(0.0), glm::vec3(0,0,1) );
	glm::quat yrotation = glm::angleAxis( ofDegToRad(localTurnAngleAroundYAxis), glm::vec3(0,1,0) );
	glm::quat xrotation = glm::angleAxis( ofDegToRad(wheelRotationAroundXAxis), glm::vec3(1,0,0) );

	// we are treating the first two wheels in the vector as the two front wheels
	// so we use localTurnAngleAroundYAxis to set their rotation around their local y axis
	// and we rotate them around their local x axis (wheelRotationAroundXAxis) so they appear to be rolling
	wheelNodes[0].setOrientation( zrotation * yrotation * xrotation );
	wheelNodes[1].setOrientation( zrotation * yrotation * xrotation );

	// the back two tires do not turn around the y axis, not used for steering
	yrotation = glm::angleAxis( ofDegToRad(0.0), glm::vec3(0,1,0) );

	wheelNodes[2].setOrientation( zrotation * yrotation * xrotation );
	wheelNodes[3].setOrientation( zrotation * yrotation * xrotation );

	headlightL.enable(); // force update
	headlightL.setPosition( -lightXAxisOffset, lightYAxisOffset, lightZAxisOffset );
	headlightR.enable(); // force update
	headlightR.setPosition( lightXAxisOffset, lightYAxisOffset, lightZAxisOffset );

	// lets make the body move a bit in relation to velocity and turn amount
	float rollDegAmount = fabs(velocity) * localTurnAngleAroundYAxis * -0.01f;
	// reset the orientation
	bodyNode.setOrientation( glm::vec3(0.f, 0.f, 0.f) );
	// roll the body around the local z axis
	bodyNode.rollDeg(rollDegAmount);
	// map the velocity to the rotation around the local x axis
	bodyNode.tiltDeg(velocity*0.2);

	// position the logo node //
	logoNode.setPosition(logoXAxisOffset, logoYAxisOffset, logoZAxisOffset);

}

//--------------------------------------------------------------
void Truck::draw(){

	bodyNode.transformGL();
	// the body by default is pointing straight up along the y axis
	// rotate the body around the x axis to be pointing along the local z axis
	ofRotateXDeg(-90);
	bodyMaterial.begin();
	bodyMesh.draw();
	bodyMaterial.end();
	bodyNode.restoreTransformGL();

	logoNode.transformGL();
	logoMaterial.begin();
	logoMesh.draw();
	logoMaterial.end();
	logoNode.restoreTransformGL();

	headlightL.transformGL();
	ofSetColor( headlightL.getDiffuseColor() );
	ofDrawSphere(0, 0, 0.1 );
	headlightL.restoreTransformGL();

	headlightR.transformGL();
	ofSetColor( headlightR.getDiffuseColor() );
	ofDrawSphere(0, 0, 0.1 );
	headlightR.restoreTransformGL();

	wheelMaterial.begin();
	for( int i = 0; i < wheelNodes.size(); i++ ){
		wheelNodes[i].transformGL();
		wheelMesh.draw();
		wheelNodes[i].restoreTransformGL();
	}
	wheelMaterial.end();
}

//--------------------------------------------------------------
void Truck::steer( float adir ){
	localTurnAngleAroundYAxis += adir;
	if( localTurnAngleAroundYAxis < -45 ) {
		localTurnAngleAroundYAxis = -45;
	}
	if( localTurnAngleAroundYAxis > 45 ) {
		localTurnAngleAroundYAxis = 45;
	}
}

//--------------------------------------------------------------
void Truck::brake(float amnt){
	acceleration -= amnt;
}

//--------------------------------------------------------------
void Truck::accelerate(float amnt){
	acceleration += amnt;
}

//--------------------------------------------------------------
const ofNode & Truck::getNode() const {
	return node;
}

//--------------------------------------------------------------
void Truck::resetPosition() {
	node.setPosition(0, 0, 0);
	node.lookAt( node.getPosition()+glm::vec3(1,0,-0.5));
	velocity = 0.;
	acceleration = 0;
	localTurnAngleAroundYAxis = 0;
	reverseVolume = 0.0;
}
