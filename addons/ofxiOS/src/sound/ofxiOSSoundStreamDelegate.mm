//
//  ofxiOSSoundStreamDelegate.m
//  Created by Lukasz Karluk on 14/06/13.
//  http://julapy.com/blog
//

#import "ofxiOSSoundStreamDelegate.h"
#import "ofBaseTypes.h"
#import "ofSoundBuffer.h"

@interface ofxiOSSoundStreamDelegate() {
	ofBaseSoundInput * soundInputApp;
	ofBaseSoundOutput * soundOutputApp;
	std::shared_ptr<ofSoundBuffer> inputBuffer;
	std::shared_ptr<ofSoundBuffer> outputBuffer;
}

@end

@implementation ofxiOSSoundStreamDelegate

- (id)init {
    self = [super init];
    if(self) {
        soundInputApp = NULL;
        soundOutputApp = NULL;
        inputBuffer = std::shared_ptr<ofSoundBuffer>(new ofSoundBuffer);
        outputBuffer = std::shared_ptr<ofSoundBuffer>(new ofSoundBuffer);
    }
    return self;
}

- (void)dealloc {
    soundInputApp = NULL;
    soundOutputApp = NULL;
    [super dealloc];
}

- (id)initWithSoundInputApp:(ofBaseSoundInput *)app {
    self = [self init];
    if(self) {
        soundInputApp = app;
    }
    return self;
}

- (id)initWithSoundOutputApp:(ofBaseSoundOutput *)app {
    self = [self init];
    if(self) {
        soundOutputApp = app;
    }
    return self;
}

- (void)setInput:(ofBaseSoundInput *)input{
	soundInputApp = input;
}
- (void)setOutput:(ofBaseSoundOutput *)output{
	soundOutputApp = output;
}

- (void)soundStreamRequested:(id)sender
                      output:(float *)output
                  bufferSize:(NSInteger)bufferSize
               numOfChannels:(NSInteger)numOfChannels {
    if(soundOutputApp) {
		outputBuffer->setNumChannels(numOfChannels);
		outputBuffer->resize(bufferSize*numOfChannels);
		soundOutputApp->audioOut(*outputBuffer);
		outputBuffer->copyTo(output, bufferSize, numOfChannels, 0);
    }
}

- (void)soundStreamReceived:(id)sender
                      input:(float *)input
                 bufferSize:(NSInteger)bufferSize
              numOfChannels:(NSInteger)numOfChannels {
    if(soundInputApp) {
		inputBuffer->copyFrom(input, bufferSize, numOfChannels, inputBuffer->getSampleRate());
		soundInputApp->audioIn(*inputBuffer);
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
