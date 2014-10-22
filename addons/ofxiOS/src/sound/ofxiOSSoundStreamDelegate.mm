//
//  ofxiOSSoundStreamDelegate.m
//  Created by Lukasz Karluk on 14/06/13.
//  http://julapy.com/blog
//

#import "ofxiOSSoundStreamDelegate.h"
#import "ofBaseTypes.h"

@implementation ofxiOSSoundStreamDelegate

- (id)init {
    self = [super init];
    if(self) {
        soundInputApp = NULL;
        soundOutputApp = NULL;
		outputBuffer = new ofSoundBuffer();
		inputBuffer = new ofSoundBuffer();
    }
    return self;
}

- (void)dealloc {
    soundInputApp = NULL;
    soundOutputApp = NULL;
	outputBuffer = NULL;
	inputBuffer = NULL;
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
    if(soundOutputApp == NULL) {
        return;
    }
    //soundOutputApp->audioOut(output, bufferSize, numOfChannels);
	outputBuffer->resize(bufferSize*numOfChannels);
	soundOutputApp->audioOut(*outputBuffer);
	
	outputBuffer->setNumChannels(numOfChannels);
	outputBuffer->copyTo(output, bufferSize, numOfChannels,0);

}

- (void)soundStreamReceived:(id)sender
                      input:(float *)input
                 bufferSize:(NSInteger)bufferSize
              numOfChannels:(NSInteger)numOfChannels {
    if(soundInputApp == NULL) {
        return;
    }
    soundInputApp->audioIn(input, bufferSize, numOfChannels);
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