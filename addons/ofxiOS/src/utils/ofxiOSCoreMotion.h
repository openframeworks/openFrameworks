/*
 ofxiOSCoreMotion created by Trent Brooks.
    - Original DeviceMotion/attitude example by Nardove: http://forum.openframeworks.cc/index.php/topic,11517.0.html - Thanks!
    - Added Gyroscope, accelerometer, & magnetometer.
    - Attitude also has quaternion (used to spin the box in the example)
 */

#pragma once

#import <CoreMotion/CoreMotion.h>
#include "ofVectorMath.h"


class ofxiOSCoreMotion {

public:
    
    ofxiOSCoreMotion();
    virtual ~ofxiOSCoreMotion();

    void setup(bool enableAttitude = true, bool enableAccelerometer = false, bool enableGyro = false, bool enableMagnetometer = false);
    
    void setupGyroscope();
    void setupAccelerometer();
    void setupMagnetometer();
    void setupAttitude(CMAttitudeReferenceFrame type = CMAttitudeReferenceFrameXArbitraryZVertical);
    
    void disableGyroscope();
    void disableAccelerometer();
    void disableMagnetometer();
    void disableAttitude();
    
    void resetAttitude(bool toCurrentReferenceFrame = true);

    void setUpdateFrequency(float updateFrequency);
    void update();

    glm::vec3 getAccelerometerData();
    glm::vec3 getGyroscopeData();
    glm::vec3 getMagnetometerData();
    float getRoll();
    float getPitch();
    float getYaw();
    glm::quat getQuaternion();
    glm::mat4 getRotationMatrix();
    glm::vec3 getGravity();
    glm::vec3 getUserAcceleration();
    glm::vec3 getMagneticField();

protected:
    
    // core motion
    CMMotionManager* motionManager;
	CMAttitude* referenceAttitude;
    CMAttitudeReferenceFrame referenceFrameType; // 1,2,4,8
    
    float updateFrequency;
    bool enableAttitude, enableGyro, enableAccelerometer, enableMagnetometer;

    glm::vec3 accelerometerData;
    glm::vec3 gyroscopeData;
    glm::vec3 magnetometerData;
    float roll, pitch, yaw;
    glm::quat attitudeQuat;
    glm::mat4 rotMatrix;
    glm::vec3 gravity;
    glm::vec3 userAcceleration;
    glm::vec3 magneticField;

};

