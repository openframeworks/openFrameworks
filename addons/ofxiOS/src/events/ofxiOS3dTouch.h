/*
 *  ofxiOS3dTouch.h
 *  iOS+OFLib
 *
 *  Created by Andreas Borg on 2/9/16
 *  Copyright 2016 Local Projects LLC. All rights reserved.
 *
 */

#ifndef _ofxiOS3dTouch
#define _ofxiOS3dTouch

#include "ofMain.h"
#import <UIKit/UIKit.h>

class ofxiOS3dTouch:public ofNativeTouchData {
	
  public:
	
	ofxiOS3dTouch(){};
	UITouch * data;
};

#endif
