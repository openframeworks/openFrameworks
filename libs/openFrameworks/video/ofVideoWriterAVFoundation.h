#import <glm/vec2.hpp>
#import "ofPixels.h"
#import "ofFbo.h"

#import <Foundation/Foundation.h>
#import "AVFoundation/AVAssetWriter.h"
#import "AVFoundation/AVAssetWriterInput.h"
#import "AVFoundation/AVMediaFormat.h"
#import "AVFoundation/AVVideoSettings.h"

//@class AVAssetWriter, AVAssetWriterInput, AVAssetWriterInputPixelBufferAdaptor, NSString;

@interface ofVideoWriterAVFoundation : NSObject

//@property (nonatomic, strong) AVAssetWriter *writer;

@property (nonatomic, assign) NSString* path;
//@property (nonatomic, assign) glm::ivec2 dimensions;
//@property (nonatomic, assign) ofPixels_<float> pixels;
@property (nonatomic, assign) int fps;
@property (nonatomic, assign) ofFbo * _fbo;

- (void)initPath:(NSString*)path;
- (BOOL)addFrame;
- (void)finishWithCompletionHandler:(void (^)(void))handler;
- (void)stopRecording;

@end

