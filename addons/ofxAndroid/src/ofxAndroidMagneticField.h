/*
 * ofxAndroidMagneticField.h
 *
 *  Created on: 15/05/2013
 *      Author: p
 */

#pragma once
#include <jni.h>

class ofxAndroidMagneticField
{

	public:

		ofxAndroidMagneticField();
		~ofxAndroidMagneticField();

		bool startHeading();
		//void stopHeading();

		//bool startLocation();
		//void stopLocation();
        //bool startMonitoringSignificantLocationChanges();
        //void stopMonitoringSignificantLocationChanges();
		//double getLatitude();
		//double getLongitude();
		//double getLocationAccuracy();
		//double getAltitude();
		//double getAltitudeAccuracy();
		//double getDistMoved();
		//double getCompassX();
		//double getCompassY();
		//double getCompassZ();
		float getMagneticHeading();
		float getTrueHeading();
		//double getHeadingAccuracy();

	private:

		jclass javaClass;
		jclass javaMagneticFieldClass;
		jobject javaMagneticField;

};
