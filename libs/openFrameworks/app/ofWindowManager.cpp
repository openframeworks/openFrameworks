/*
 *  ofWindowManager.cpp
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 3/8/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */

#include "ofWindowManager.h"
#include "ofConstants.h"

ofWindowManager::ofWindowManager(){

};

ofWindowManager::~ofWindowManager(){

};

void ofWindowManager::addWindow(ofWindow* win){
	windows.push_back(ofWindowPtr(win));
}