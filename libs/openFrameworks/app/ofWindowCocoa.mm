/*
 *  ofWindowCocoa.cpp
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 3/8/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */

#include "ofWindowCocoa.h"

ofWindowCocoa::ofWindowCocoa(){

};

ofWindowCocoa::~ofWindowCocoa(){

};

bool ofWindowCocoa::open(){
	windowPtr = [[NSWindow alloc]
							  //initWithContentRect:[[m_window screen] frame]
							  styleMask:(NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask)
							  backing:NSBackingStoreBuffered
							  defer:YES];
}