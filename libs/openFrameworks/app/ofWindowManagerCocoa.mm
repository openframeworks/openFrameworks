/*
 *  ofWindowManagerCocoa.mm
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 3/8/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */

#include "ofWindowManagerCocoa.h"

#include "ofWindowCocoa.h"

ofWindow* ofWindowManagerCocoa::createWindow(){
	ofWindow* win = new ofWindowCocoa();
	addWindow(win);
	return win;
}