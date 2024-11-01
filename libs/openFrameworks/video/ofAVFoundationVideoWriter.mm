#import "ofAVFoundationVideoWriter.h"

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface ofAVFoundationVideoWriter () {
	BOOL _isWaitingForInputReady;
	dispatch_semaphore_t _writeSemaphore;
	CMTime frameTime;
	AVAssetWriter *_writer;
	
//	ofPixels_<float> pixels;
	ofPixels pixels;
//	ofFbo * fbo;
}
//@property (nonatomic, strong) AVAssetWriterInput *writerInput;
//@property (nonatomic, strong) AVAssetWriterInputPixelBufferAdaptor *adaptor;
@property (strong) AVAssetWriterInput *writerInput;
@property (strong) AVAssetWriterInputPixelBufferAdaptor *adaptor;

@end

@implementation ofAVFoundationVideoWriter

//@synthesize writer;
@synthesize path;
@synthesize dimensions;
@synthesize _fbo;
//@synthesize pixels;

//- (void)dealloc {
//	[_adaptor release];
//	[_writerInput release];
//	[_writer release];
//	[super dealloc];
//}


- (void) initPath:(NSString*)path
{
	// FIXME: remove
//	self = [super init];
	self.path = path;
	
	[[NSFileManager defaultManager] removeItemAtPath:self.path error:nil];
	NSURL* url = [NSURL fileURLWithPath:self.path];
	
	NSLog(@"url %@", url);

	_writer = [AVAssetWriter assetWriterWithURL:url
			fileType:AVFileTypeQuickTimeMovie //AVFileTypeAppleM4V
			error:nil];
	

//	NSLog(@"dimensions x %d, y %d", dimensions.x, dimensions.y);

	NSDictionary* settings = @{
//		AVVideoCodecKey: AVVideoCodecTypeH264,
		AVVideoCodecKey: AVVideoCodecTypeAppleProRes422HQ,
		AVVideoWidthKey: @(dimensions.x),
		AVVideoHeightKey: @(dimensions.y),
//		AVVideoCompressionPropertiesKey: @{
//			AVVideoAverageBitRateKey: @(1200000),
//			AVVideoMaxKeyFrameIntervalKey: @(150),
//			AVVideoProfileLevelKey: AVVideoProfileLevelH264BaselineAutoLevel,
//			AVVideoAllowFrameReorderingKey: @NO,
//			AVVideoExpectedSourceFrameRateKey: @(24),
//			//AVVideoH264EntropyModeKey: AVVideoH264EntropyModeCAVLC,
//			//AVVideoAverageNonDroppableFrameRateKey: @(30)
//		}
	};
	_writerInput = [AVAssetWriterInput assetWriterInputWithMediaType:AVMediaTypeVideo outputSettings:settings];

	_writerInput.expectsMediaDataInRealTime = YES;
	
	//int local_bytesPerRow = self.frameSize.width * 4;
	int local_bytesPerRow = dimensions.x * 4;

	NSDictionary *pixelBufferOptions = [[NSDictionary alloc] initWithObjectsAndKeys:
		[NSNumber numberWithInt:kCVPixelFormatType_32BGRA],  kCVPixelBufferPixelFormatTypeKey, //BGRA is SERIOUSLY BLOODY QUICK!
		 //kCFBooleanTrue, kCVPixelBufferCGImageCompatibilityKey,
		kCFBooleanTrue, kCVPixelBufferCGBitmapContextCompatibilityKey,
		[NSNumber numberWithInt:local_bytesPerRow], kCVPixelBufferBytesPerRowAlignmentKey,
		[NSNumber numberWithInt:dimensions.x], kCVPixelBufferWidthKey,
		[NSNumber numberWithInt:dimensions.y], kCVPixelBufferHeightKey,
		 nil];
	
	_adaptor = [[AVAssetWriterInputPixelBufferAdaptor alloc] initWithAssetWriterInput:_writerInput sourcePixelBufferAttributes:pixelBufferOptions];
	
	if ([_writer canAddInput:_writerInput]) {
		[_writer addInput:_writerInput];
//		NSLog(@"Writer addinput");
	}

	[_writer startWriting];
	frameTime = CMTimeMake(0, 24);
	[_writer startSessionAtSourceTime:frameTime];
}

- (BOOL) addFrame {
	_fbo->getTexture().readToPixels(pixels);
	
//		NSMutableData nsData = (NSMutableData*)(pixels.getData());
	CVPixelBufferRef pixelBuffer;
	
//	CFDataRef bufferData = CFDataCreate(NULL, pixels.getData(), pixels.getTotalBytes());

	CVReturn result = CVPixelBufferCreateWithBytes(NULL,
							 pixels.getWidth(),
							 pixels.getHeight(),
							 kCVPixelFormatType_32BGRA,
//									 k32ARGBPixelFormat,
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
//	CVPixelBufferLockBaseAddress( pixelBuffer, 0 );

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
