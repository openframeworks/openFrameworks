//
//  ofVideoWriterAVFoundation.m
//  Created by Dimitre Lima / Dmtr.org on 31/10/24.
//	Copyright Dmtr.org 2024
//

#import <glm/vec2.hpp>
#import "ofFbo.h"

#import <Foundation/Foundation.h>
#import "AVFoundation/AVAssetWriter.h"
#import "AVFoundation/AVAssetWriterInput.h"
#import "AVFoundation/AVMediaFormat.h"
#import "AVFoundation/AVVideoSettings.h"

@interface ofVideoWriterAVFoundation : NSObject

@property (nonatomic, assign) NSString* path;
@property (nonatomic, assign) int fps;
@property (nonatomic, assign) ofFbo * _fbo;

- (void)initPath:(NSString*)path;
- (BOOL)addFrame;
- (void)finishWithCompletionHandler:(void (^)(void))handler;
- (void)stopRecording;

@end

