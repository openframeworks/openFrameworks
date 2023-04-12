/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv, Douglas Edric Stanley, www.abstractmachine.net
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/

#include <TargetConditionals.h>

#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)

#include "ofxiOSMapKitDelegate.h"
#include "ofxiOSMapKit.h"
#include "ofLog.h"

@implementation ofxiOSMapKitDelegate

-(instancetype)initWithMapKit:(ofxiOSMapKit*)mk {
	if(self = [super init]) {
		mapKit = mk;
		ofLogVerbose("ofxiOSMapKitDelegate") << "initWithMapKit";
	}
	return self;
}

-(void)dealloc {
	ofLogVerbose("ofxiOSMapKitDelegate") << "dealloc";
}


- (void)mapView:(MKMapView *)mapView regionDidChangeAnimated:(BOOL)animated {
	mapKit->regionDidChange(animated);
}

- (void)mapView:(MKMapView *)mapView regionWillChangeAnimated:(BOOL)animated {
	mapKit->regionWillChange(animated);
}

- (void)mapViewWillStartLoadingMap:(MKMapView *)mapView {
	mapKit->willStartLoadingMap();
}

- (void)mapViewDidFinishLoadingMap:(MKMapView *)mapView {
	mapKit->didFinishLoadingMap();
}

- (void)mapViewDidFailLoadingMap:(MKMapView *)mapView withError:(NSError *)error {
	ofLogVerbose("ofxiOSMapKitDelegate") << "mapViewDidFailLoadingMap";
	std::string s = error != nil ? [[error localizedDescription] UTF8String] : "unknown error";
	mapKit->errorLoadingMap(s);
}



@end

#endif
