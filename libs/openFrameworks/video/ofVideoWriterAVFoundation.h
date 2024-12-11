//
//  ofVideoWriterAVFoundation.m
//  Created by Dimitre Lima / Dmtr.org on 31/10/24.
//	Copyright Dmtr.org 2024
//
#pragma once

#import "ofFbo.h"

#ifdef __OBJC__
#import <Foundation/Foundation.h>
@interface ofVideoWriterAVFoundation : NSObject
@property (nonatomic, assign) int fps;
@property (nonatomic, assign) NSString* path;
@property (nonatomic, assign) ofFbo * _fbo;
- (void)initPath:(NSString*)path;
- (BOOL)addFrame;
- (void)finishWithCompletionHandler:(void (^)(void))handler;
- (void)stopRecording;
@end
#endif

class VW {
public:
	VW();
	void initPath(const std::string & fileName);
	void addFrame();
	void stop();
#ifdef __OBJC__
	ofVideoWriterAVFoundation * videoWriter;
#endif
};
