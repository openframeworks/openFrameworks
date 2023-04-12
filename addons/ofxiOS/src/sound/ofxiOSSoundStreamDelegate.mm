//
//  ofxiOSSoundStreamDelegate.m
//  Created by Lukasz Karluk on 14/06/13.
//  http://julapy.com/blog
//

#include "ofxiOSSoundStreamDelegate.h"
#include "ofLog.h"
#include "ofSoundBuffer.h"

@interface ofxiOSSoundStreamDelegate() {
	std::function<void(ofSoundBuffer &)> inCallback;
	std::function<void(ofSoundBuffer &)> outCallback;
	std::shared_ptr<ofSoundBuffer> inputBuffer;
	std::shared_ptr<ofSoundBuffer> outputBuffer;
    unsigned long long tickCount;
}

@end

@implementation ofxiOSSoundStreamDelegate

- (instancetype)init {
    self = [super init];
    if(self) {
        inputBuffer = std::shared_ptr<ofSoundBuffer>(new ofSoundBuffer);
        outputBuffer = std::shared_ptr<ofSoundBuffer>(new ofSoundBuffer);
        tickCount = 0;
    }
    return self;
}

- (void)dealloc {
    inCallback = nullptr;
    outCallback = nullptr;
}

- (instancetype)initWithSoundInputFn:(std::function<void(ofSoundBuffer &)>)fn {
    self = [self init];
    if(self) {
        inCallback = fn;
    }
    return self;
}

- (instancetype)initWithSoundOutputFn:(std::function<void(ofSoundBuffer &)>)fn {
    self = [self init];
    if(self) {
        outCallback = fn;
    }
    return self;
}

- (void)setInput:(std::function<void(ofSoundBuffer &)>)input{
	inCallback = input;
}

- (void)setOutput:(std::function<void(ofSoundBuffer &)>)output{
	outCallback = output;
}

- (void)soundStreamRequested:(id)sender
                      output:(float *)output
                  bufferSize:(NSInteger)bufferSize
               numOfChannels:(NSInteger)numOfChannels {
    if(outCallback) {
		outputBuffer->setNumChannels(numOfChannels);
		outputBuffer->resize(bufferSize*numOfChannels);
		outputBuffer->setTickCount(tickCount);
		outCallback(*outputBuffer);
		outputBuffer->copyTo(output, bufferSize, numOfChannels, 0);
		tickCount++;
    }
}

- (void)soundStreamReceived:(id)sender
                      input:(float *)input
                 bufferSize:(NSInteger)bufferSize
              numOfChannels:(NSInteger)numOfChannels {
    if(inCallback) {
		inputBuffer->copyFrom(input, bufferSize, numOfChannels, inputBuffer->getSampleRate());
		inputBuffer->setTickCount(tickCount);
		inCallback(*inputBuffer);
    }
}

- (void)soundStreamBeginInterruption:(id)sender {
    NSString * streamType = [[sender class] description];
    NSString * errorMessage = [NSString stringWithFormat:@"%@ :: Begin Interruption", streamType];
    ofLogVerbose("ofxiOSSoundStreamDelegate") << [errorMessage UTF8String];
}

- (void)soundStreamEndInterruption:(id)sender {
    NSString * streamType = [[sender class] description];
    NSString * errorMessage = [NSString stringWithFormat:@"%@ :: End Interruption", streamType];
    ofLogVerbose("ofxiOSSoundStreamDelegate") << [errorMessage UTF8String];
}

- (void)soundStreamError:(id)sender
                   error:(NSString *)error {
    NSString * streamType = [[sender class] description];
    NSString * errorMessage = [NSString stringWithFormat:@"%@ :: %@", streamType, error];
    ofLogVerbose("ofxiOSSoundStreamDelegate") << [errorMessage UTF8String];
}

@end
