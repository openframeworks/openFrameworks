#include "Car.h"

Car::Car(){
}

void Car::setup(){
    material.setDiffuseColor(ofFloatColor::green);
    // This is how we append the 2 headlamps to the car.
    // Calling setParent, we are telling to the lights that they are
    // a child of the object car, and that teir movement are relative to
    // the movement of the car

    lightL.setParent(geometry);
    lightR.setParent(geometry);
    
    lightL.move(-45,20,-51);
    lightR.move(45,20,-51);
    lightL.tilt(-20);
    lightR.tilt(-20);
	lightL.setSpotlight();
	lightR.setSpotlight();
	lightL.setDiffuseColor(ofFloatColor::yellow);
	lightR.setDiffuseColor(ofFloatColor::yellow);
    lightL.setup();
    lightR.setup();
}

void Car::draw(){
    material.begin();
    geometry.draw();
    material.end();
    
    lightR.draw();
    lightL.draw();
}

void Car::update(){
    vel += acceleration;
    vel *= 0.9;
	acceleration *= 0.99;
    // here we are defining the vector that contains the direction
    // in which the car should move, that is defined by the z-axis of the car and the velocity
    auto velVector = geometry.getZAxis() * -vel;
	geometry.move(velVector);
}

void Car::brake(){
    acceleration -= 0.1;
}

void Car::accellerate(){
    acceleration += 0.1;
}

void Car::steer(float dir){
    geometry.rotate(dir, 0, 1, 0); // the rotation happens on the y axis
}
