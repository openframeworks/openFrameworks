#import "ofVideoWriterAVFoundation.h"
//#import "ofEvents.h"

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface ofVideoWriterAVFoundation () {
	BOOL _isWaitingForInputReady;
	dispatch_semaphore_t _writeSemaphore;
	CMTime frameTime;
	AVAssetWriter *_writer;
	
	// FIXME: mais profundidade de cores
//	ofPixels_<float> pixels;
	ofPixels pixels;
//	ofFbo * fbo;
}
//@property (nonatomic, strong) AVAssetWriterInput *writerInput;
//@property (nonatomic, strong) AVAssetWriterInputPixelBufferAdaptor *adaptor;
@property (strong) AVAssetWriterInput *writerInput;
@property (strong) AVAssetWriterInputPixelBufferAdaptor *adaptor;

@end

@implementation ofVideoWriterAVFoundation

//@synthesize dimensions;
@synthesize path;
@synthesize fps;
@synthesize _fbo;

// MARK: initPath
- (void) initPath:(NSString*)path {
	if (_fbo == nullptr) {
		return;
	}
//	NSLog(@"initPath %@", path);

	//	self = [super init];
	self.path = path;

	[[NSFileManager defaultManager] removeItemAtPath:self.path error:nil];
	
	NSURL* url = [NSURL fileURLWithPath:self.path];

	_writer = [AVAssetWriter assetWriterWithURL:url
			fileType:AVFileTypeQuickTimeMovie //AVFileTypeAppleM4V
			error:nil];
	
//	NSLog(@"dimensions x %d, y %d", dimensions.x, dimensions.y);

	NSDictionary* settings = @{
//		AVVideoCodecKey: AVVideoCodecTypeH264,
		AVVideoCodecKey: AVVideoCodecTypeHEVC,
//		AVVideoCodecKey: AVVideoCodecTypeAppleProRes422HQ,
		AVVideoWidthKey: @(_fbo->getWidth()),
		AVVideoHeightKey: @(_fbo->getHeight()),
//		AVVideoCompressionPropertiesKey: @{
//			AVVideoExpectedSourceFrameRateKey: @(fps),
//			AVVideoAverageBitRateKey: @(1200000),
//			AVVideoMaxKeyFrameIntervalKey: @(150),
//			AVVideoProfileLevelKey: AVVideoProfileLevelH264BaselineAutoLevel,
//			AVVideoAllowFrameReorderingKey: @NO,
//			//AVVideoH264EntropyModeKey: AVVideoH264EntropyModeCAVLC,
//			//AVVideoAverageNonDroppableFrameRateKey: @(30)
//		}
	};
	_writerInput = [AVAssetWriterInput assetWriterInputWithMediaType:AVMediaTypeVideo outputSettings:settings];

	_writerInput.expectsMediaDataInRealTime = YES;
	
	//int local_bytesPerRow = self.frameSize.width * 4;
	
//	int local_bytesPerRow = _fbo->getWidth()* 4;
	_fbo->getTexture().readToPixels(pixels);
	int local_bytesPerRow = pixels.getBytesStride();

	NSDictionary *pixelBufferOptions = [[NSDictionary alloc] initWithObjectsAndKeys:
		[NSNumber numberWithInt:kCVPixelFormatType_32BGRA],  kCVPixelBufferPixelFormatTypeKey, //BGRA is SERIOUSLY BLOODY QUICK!
		 //kCFBooleanTrue, kCVPixelBufferCGImageCompatibilityKey,
		kCFBooleanTrue, kCVPixelBufferCGBitmapContextCompatibilityKey,
		[NSNumber numberWithInt:local_bytesPerRow], kCVPixelBufferBytesPerRowAlignmentKey,
		[NSNumber numberWithInt:_fbo->getWidth()], kCVPixelBufferWidthKey,
		[NSNumber numberWithInt:_fbo->getHeight()], kCVPixelBufferHeightKey,
		 nil];
	
	_adaptor = [[AVAssetWriterInputPixelBufferAdaptor alloc] initWithAssetWriterInput:_writerInput sourcePixelBufferAttributes:pixelBufferOptions];
	
	if ([_writer canAddInput:_writerInput]) {
		[_writer addInput:_writerInput];
//		NSLog(@"Writer addinput");
	}

	[_writer startWriting];
	frameTime = CMTimeMake(0, fps);
	
//	NSLog(@"FPS: %d", fps);
//	NSLog(@"frameTime: %@", frameTime);
	[_writer startSessionAtSourceTime:frameTime];
}

- (BOOL) addFrame {
	
	@autoreleasepool {
		_fbo->getTexture().readToPixels(pixels);
		//NSMutableData nsData = (NSMutableData*)(pixels.getData());
		CVPixelBufferRef pixelBuffer;
		
		//FDataRef bufferData = CFDataCreate(NULL, pixels.getData(), pixels.getTotalBytes());
		CVReturn result = CVPixelBufferCreateWithBytes(NULL,
							 pixels.getWidth(),
							 pixels.getHeight(),
							 kCVPixelFormatType_32BGRA, //k32ARGBPixelFormat
							 (void*)(pixels.getData()),
							 pixels.getBytesStride(),
							 NULL,
							 0,
							 NULL,
							 &pixelBuffer);
		
		if (result == kCVReturnSuccess) {
		} else {
			return NO;
		}

		if ( !_writerInput.isReadyForMoreMediaData ) {
			return NO;
	//		_isWaitingForInputReady = YES;
	//		dispatch_semaphore_wait(_writeSemaphore, DISPATCH_TIME_FOREVER);
		}

		
		BOOL ok = [_adaptor appendPixelBuffer:pixelBuffer withPresentationTime:frameTime];
		if (ok) {
			frameTime.value++;
		}
		else {
			NSLog(@"Not OK");
		}

		return ok;
	}

//		CVBufferRelease(pixelBuffer);
//		CVPixelBufferRelease(pixelBuffer);
//		CFRelease(pixelBuffer);
	
//		NSLog(@"z %zu", pixels.getWidth() * 4);
//		NSLog(@"z %zu", pixels.getBytesStride());
//				NSLog(@"w %zu", CVPixelBufferGetWidth( pixelBuffer ));
//				NSLog(@"h %zu", CVPixelBufferGetHeight( pixelBuffer ));
//				NSLog(@"br %zu", CVPixelBufferGetBytesPerRow( pixelBuffer ));
//				NSLog(@"bd %zu", CVPixelBufferGetDataSize( pixelBuffer ));
//				NSLog(@"xxx %zu", pixels.getTotalBytes());
}


- (void)stopRecording {
	if ( _writer.status == AVAssetWriterStatusWriting ) {
		[_writerInput markAsFinished];
	}

	frameTime.value++;
	[_writer endSessionAtSourceTime:frameTime];

	[_writer finishWritingWithCompletionHandler:^{
//		NSLog(@"Write Ended");
	}];

}

- (void) finishWithCompletionHandler:(void (^)(void))handler {
	NSLog(@"finishWithCompletionHandler");

	if( _writer.status == AVAssetWriterStatusWriting) {
		[_writerInput markAsFinished];
	}

	if (_writer.status == AVAssetWriterStatusCompleted || _writer.status == AVAssetWriterStatusCancelled || _writer.status == AVAssetWriterStatusUnknown)
	{
		if (handler) {
			handler();
		}
		return;
	}
	
	[_writer finishWritingWithCompletionHandler: handler];
//	NSLog(@"Write Ended");
}


@end
