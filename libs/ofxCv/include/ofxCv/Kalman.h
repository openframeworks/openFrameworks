#pragma once

#include "ofxCv.h"

namespace ofxCv {
	
	using namespace cv;
	
	// Kalman filter for positioning
	template <class T>
	class KalmanPosition_ {
		KalmanFilter KF;
		Mat_<T> measurement, prediction, estimated;
	public:
		// smoothness, rapidness: smaller is more smooth/rapid
		// bUseAccel: set true to smooth out velocity
		void init(T smoothness = 0.1, T rapidness = 0.1, bool bUseAccel = false);
		void update(const ofVec3f&);
		ofVec3f getPrediction();
		ofVec3f getEstimation();
		ofVec3f getVelocity();
	};
	
	typedef KalmanPosition_<float> KalmanPosition;
	
	// Kalman filter for orientation
	template <class T>
	class KalmanEuler_ : public KalmanPosition_<T> {
		ofVec3f eulerPrev; // used for finding appropriate dimension
	public:
		void init(T smoothness = 0.1, T rapidness = 0.1, bool bUseAccel = false);
		void update(const ofQuaternion&);
		ofQuaternion getPrediction();
		ofQuaternion getEstimation();
		//ofQuaternion getVelocity();
	};
	
	typedef KalmanEuler_<float> KalmanEuler;	
}
