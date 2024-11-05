//
//  ofVideoWriterAVFoundation.m
//  Created by Dimitre Lima / Dmtr.org on 31/10/24.
//	Copyright Dmtr.org 2024
//

/*
 FIXME: better color depth / ofPixels_<float> pixels / kCVPixelFormatType_64BGRA
 FIXME: mudar stopRecording para finishWithCompletionHandler
 FIXME: Color profiles, ex: AVCaptureColorSpace.HLG_BT2020
 
 Tirar mais ideias daqui: semaphore
 http://codefromabove.com/2015/01/av-foundation-saving-a-sequence-of-raw-rgb-frames-to-a-movie/
 https://github.com/dsward2/macSVG/blob/c9d1b4e30a06769f5109f025e9960fbd1f56ef2c/macSVG/SVG%20Functions/SVGtoVideoConverter.m#L144
*/

#import "ofVideoWriterAVFoundation.h"
#import "ofPixels.h"

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#import "AVFoundation/AVAssetWriter.h"
#import "AVFoundation/AVAssetWriterInput.h"
#import "AVFoundation/AVMediaFormat.h"
#import "AVFoundation/AVVideoSettings.h"


@interface ofVideoWriterAVFoundation () {
	CMTime frameTime;
	AVAssetWriter * _writer;
	ofPixels pixels;
	
	BOOL _isWaitingForInputReady;
	dispatch_semaphore_t _writeSemaphore;
}

@property (strong) AVAssetWriterInput *writerInput;
@property (strong) AVAssetWriterInputPixelBufferAdaptor *adaptor;
@end

@implementation ofVideoWriterAVFoundation

@synthesize path;
@synthesize fps;
@synthesize _fbo;

- (void) initPath:(NSString*)path {
	if (_fbo == nullptr) {
		return;
	}
	//	self = [super init];
	self.path = path;

	[[NSFileManager defaultManager] removeItemAtPath:self.path error:nil];
	
	NSURL* url = [NSURL fileURLWithPath:self.path];

	_writer = [AVAssetWriter assetWriterWithURL:url
			fileType:AVFileTypeQuickTimeMovie //AVFileTypeAppleM4V
			error:nil];
	
	NSDictionary* settings = @{
//		AVVideoCodecKey: AVVideoCodecTypeH264,
		AVVideoCodecKey: AVVideoCodecTypeHEVC,
//		AVVideoCodecKey: AVVideoCodecTypeAppleProRes422HQ,
		AVVideoWidthKey: @(_fbo->getWidth()),
		AVVideoHeightKey: @(_fbo->getHeight()),
		AVVideoCompressionPropertiesKey: @{
			AVVideoColorPrimariesKey: AVVideoColorPrimaries_ITU_R_709_2,
			AVVideoTransferFunctionKey: AVVideoTransferFunction_ITU_R_709_2,
			AVVideoYCbCrMatrixKey: AVVideoYCbCrMatrix_ITU_R_709_2,
			
//			AVVideoExpectedSourceFrameRateKey: @(fps),
//			AVVideoAverageBitRateKey: @(1200000),
//			AVVideoMaxKeyFrameIntervalKey: @(150),
//			AVVideoProfileLevelKey: AVVideoProfileLevelH264BaselineAutoLevel,
//			AVVideoAllowFrameReorderingKey: @NO,
//			//AVVideoH264EntropyModeKey: AVVideoH264EntropyModeCAVLC,
//			//AVVideoAverageNonDroppableFrameRateKey: @(30)
		}
	};
	_writerInput = [AVAssetWriterInput assetWriterInputWithMediaType:AVMediaTypeVideo outputSettings:settings];

	_writerInput.expectsMediaDataInRealTime = YES;
	_fbo->getTexture().readToPixels(pixels);
	int local_bytesPerRow = pixels.getBytesStride();

	NSDictionary *pixelBufferOptions = [[NSDictionary alloc] initWithObjectsAndKeys:
		[NSNumber numberWithInt:kCVPixelFormatType_32BGRA],  kCVPixelBufferPixelFormatTypeKey,
		 //kCFBooleanTrue, kCVPixelBufferCGImageCompatibilityKey,
		kCFBooleanTrue, kCVPixelBufferCGBitmapContextCompatibilityKey,
		[NSNumber numberWithInt:local_bytesPerRow], kCVPixelBufferBytesPerRowAlignmentKey,
		[NSNumber numberWithInt:_fbo->getWidth()], kCVPixelBufferWidthKey,
		[NSNumber numberWithInt:_fbo->getHeight()], kCVPixelBufferHeightKey,
		 nil];
	
	_adaptor = [[AVAssetWriterInputPixelBufferAdaptor alloc] initWithAssetWriterInput:_writerInput sourcePixelBufferAttributes:pixelBufferOptions];
	
	if ([_writer canAddInput:_writerInput]) {
		[_writer addInput:_writerInput];
	}

	[_writer startWriting];
	frameTime = CMTimeMake(0, fps);
	[_writer startSessionAtSourceTime:frameTime];
}


- (BOOL) addFrame {
	@autoreleasepool {
		_fbo->getTexture().readToPixels(pixels);
		CVPixelBufferRef pixelBuffer;
		CVReturn result = CVPixelBufferCreateWithBytes(NULL,
							 pixels.getWidth(),
							 pixels.getHeight(),
							 kCVPixelFormatType_32BGRA,
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
}


- (void)stopRecording {
	if ( _writer.status == AVAssetWriterStatusWriting ) {
		[_writerInput markAsFinished];
	}

	frameTime.value++;
	[_writer endSessionAtSourceTime:frameTime];

	[_writer finishWritingWithCompletionHandler:^{
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
}


@end



VW::VW() {
	videoWriter = [[ofVideoWriterAVFoundation alloc] init];
}

void VW::initPath(const std::string & fileName) {
	[videoWriter initPath:[NSString stringWithUTF8String:fileName.c_str()]];
}

void VW::addFrame() {
	[videoWriter addFrame];
}

void VW::stop() {
	[videoWriter stopRecording];
}
