#include "Kalman.h"

namespace ofxCv {
	
	// based on code from:
	// http://www.morethantechnical.com/2011/06/17/simple-kalman-filter-for-tracking-using-opencv-2-2-w-code/
	
	using namespace cv;
	
	template <class T>
	void KalmanPosition_<T>::init(T smoothness, T rapidness) {
		KF.init(6, 3, 0); // 6 variables (position+velocity) and 3 measurements (position)
		
		KF.transitionMatrix = *(Mat_<T>(6, 6) <<
								1,0,0,1,0,0,
								0,1,0,0,1,0,
								0,0,1,0,0,1,
								0,0,0,1,0,0,
								0,0,0,0,1,0,
								0,0,0,0,0,1);
		
		measurement = Mat_<T>::zeros(3, 1);
		
		KF.statePre = Mat_<T>::zeros(6, 1);
		
		setIdentity(KF.measurementMatrix);
		setIdentity(KF.processNoiseCov, Scalar::all(smoothness));
		setIdentity(KF.measurementNoiseCov, Scalar::all(rapidness));
		setIdentity(KF.errorCovPost, Scalar::all(.1));
	}
	
	template <class T>
	void KalmanPosition_<T>::update(const ofVec3f& p) {
		// First predict, to update the internal statePre variable
		prediction = KF.predict();
		
		// The "correct" phase that is going to use the predicted value and our measurement
		measurement(0) = p.x;
		measurement(1) = p.y;
		measurement(2) = p.z;
		estimated = KF.correct(measurement);
	}
	
	template <class T>
	ofVec3f KalmanPosition_<T>::getPrediction()
	{
		return ofVec3f(prediction(0), prediction(1), prediction(2));
	}
	
	template <class T>
	ofVec3f KalmanPosition_<T>::getEstimation()
	{
		return ofVec3f(estimated(0), estimated(1), estimated(2));
	}
	
	template <class T>
	ofVec3f KalmanPosition_<T>::getVelocity()
	{
		return ofVec3f(estimated(3), estimated(4), estimated(5));
	}
	
	template class KalmanPosition_<float>;
	
}
