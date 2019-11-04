/*
 *  ofAVFoundationGrabber.mm
 */

#include "ofAVFoundationGrabber.h"
#include "ofVec2f.h"
#include "ofRectangle.h"
#include "ofGLUtils.h"

#ifdef OF_VIDEO_CAPTURE_AVF

#import <Accelerate/Accelerate.h>

@interface OSXVideoGrabber ()
@property (nonatomic,retain) AVCaptureSession *captureSession;
@end

@implementation OSXVideoGrabber
@synthesize captureSession;

#pragma mark -
#pragma mark Initialization
- (id)init {
	self = [super init];
	if (self) {
		captureInput = nil;
		captureOutput = nil;
		device = nil;

		bInitCalled = NO;
		grabberPtr = NULL;
		deviceID = 0;
        width = 0;
        height = 0;
        currentFrame = 0;
	}
	return self;
}

- (BOOL)initCapture:(int)framerate capWidth:(int)w capHeight:(int)h{
	NSArray * devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];

	if([devices count] > 0) {
		if(deviceID>[devices count]-1)
			deviceID = [devices count]-1;


		// We set the device
		device = [devices objectAtIndex:deviceID];

		NSError *error = nil;
		[device lockForConfiguration:&error];

		if(!error) {

			float smallestDist = 99999999.0;
			int bestW, bestH = 0;

			// Set width and height to be passed in dimensions
			// We will then check to see if the dimensions are supported and if not find the closest matching size.
			width = w;
			height = h;

			ofVec2f requestedDimension(width, height);

			AVCaptureDeviceFormat * bestFormat  = nullptr;
			for ( AVCaptureDeviceFormat * format in [device formats] ) {
				CMFormatDescriptionRef desc = format.formatDescription;
				CMVideoDimensions dimensions = CMVideoFormatDescriptionGetDimensions(desc);

				int tw = dimensions.width;
				int th = dimensions.height;
				ofVec2f formatDimension(tw, th);

				if( tw == width && th == height ){
					bestW = tw;
					bestH = th;
					bestFormat = format;
					break;
				}

				float dist = (formatDimension-requestedDimension).length();
				if( dist < smallestDist ){
					smallestDist = dist;
					bestW = tw;
					bestH = th;
					bestFormat = format;
				}

				ofLogVerbose("ofAvFoundationGrabber") << " supported dimensions are: " << dimensions.width << " " << dimensions.height;
			}

			// Set the new dimensions and format
			if( bestFormat != nullptr && bestW != 0 && bestH != 0 ){
				if( bestW != width || bestH != height ){
					ofLogWarning("ofAvFoundationGrabber") << " requested width and height aren't supported. Setting capture size to closest match: " << bestW << " by " << bestH<< std::endl;
				}

				[device setActiveFormat:bestFormat];
				width = bestW;
				height = bestH;
			}

			//only set the framerate if it has been set by the user
			if( framerate > 0 ){

				AVFrameRateRange * desiredRange = nil;
				NSArray * supportedFrameRates = device.activeFormat.videoSupportedFrameRateRanges;

				int numMatch = 0;
				for(AVFrameRateRange * range in supportedFrameRates){

					if( (floor(range.minFrameRate) <= framerate && ceil(range.maxFrameRate) >= framerate) ){
						ofLogVerbose("ofAvFoundationGrabber") << "found good framerate range, min: " << range.minFrameRate << " max: " << range.maxFrameRate << " for requested fps: " << framerate;
						desiredRange = range;
						numMatch++;
					}
				}

				if( numMatch > 0 ){
					//TODO: this crashes on some devices ( Orbecc Astra Pro )
					device.activeVideoMinFrameDuration = desiredRange.minFrameDuration;
					device.activeVideoMaxFrameDuration = desiredRange.maxFrameDuration;
				}else{
					ofLogError("ofAvFoundationGrabber") << " could not set framerate to: " << framerate << ". Device supports: ";
					for(AVFrameRateRange * range in supportedFrameRates){
						ofLogError() << "  framerate range of: " << range.minFrameRate <<
					 " to " << range.maxFrameRate;
					 }
				}

			}

			[device unlockForConfiguration];
		} else {
			NSLog(@"OSXVideoGrabber Init Error: %@", error);
		}

		// We setup the input
		captureInput						= [AVCaptureDeviceInput
											   deviceInputWithDevice:device
											   error:nil];

		// We setup the output
		captureOutput = [[AVCaptureVideoDataOutput alloc] init];
		// While a frame is processes in -captureOutput:didOutputSampleBuffer:fromConnection: delegate methods no other frames are added in the queue.
		// If you don't want this behaviour set the property to NO
		captureOutput.alwaysDiscardsLateVideoFrames = YES;



		// We create a serial queue to handle the processing of our frames
		dispatch_queue_t queue;
		queue = dispatch_queue_create("cameraQueue", NULL);
		[captureOutput setSampleBufferDelegate:self queue:queue];
		dispatch_release(queue);

		NSDictionary* videoSettings =[NSDictionary dictionaryWithObjectsAndKeys:
                              [NSNumber numberWithDouble:width], (id)kCVPixelBufferWidthKey,
                              [NSNumber numberWithDouble:height], (id)kCVPixelBufferHeightKey,
                              [NSNumber numberWithUnsignedInt:kCVPixelFormatType_32BGRA], (id)kCVPixelBufferPixelFormatTypeKey,
                              nil];
		[captureOutput setVideoSettings:videoSettings];

		// And we create a capture session
		if(self.captureSession) {
			self.captureSession = nil;
		}
		self.captureSession = [[[AVCaptureSession alloc] init] autorelease];

		[self.captureSession beginConfiguration];

		// We add input and output
		[self.captureSession addInput:captureInput];
		[self.captureSession addOutput:captureOutput];

		// We specify a minimum duration for each frame (play with this settings to avoid having too many frames waiting
		// in the queue because it can cause memory issues). It is similar to the inverse of the maximum framerate.
		// In this example we set a min frame duration of 1/10 seconds so a maximum framerate of 10fps. We say that
		// we are not able to process more than 10 frames per second.
		// Called after added to captureSession

		AVCaptureConnection *conn = [captureOutput connectionWithMediaType:AVMediaTypeVideo];
		if ([conn isVideoMinFrameDurationSupported] == YES &&
			[conn isVideoMaxFrameDurationSupported] == YES) {
				[conn setVideoMinFrameDuration:CMTimeMake(1, framerate)];
				[conn setVideoMaxFrameDuration:CMTimeMake(1, framerate)];
		}

		// We start the capture Session
		[self.captureSession commitConfiguration];
		[self.captureSession startRunning];

		bInitCalled = YES;
		return YES;
	}
	return NO;
}

-(void) startCapture{

	[self.captureSession startRunning];

	[captureInput.device lockForConfiguration:nil];

	//if( [captureInput.device isExposureModeSupported:AVCaptureExposureModeAutoExpose] ) [captureInput.device setExposureMode:AVCaptureExposureModeAutoExpose ];
	if( [captureInput.device isFocusModeSupported:AVCaptureFocusModeAutoFocus] )	[captureInput.device setFocusMode:AVCaptureFocusModeAutoFocus ];

}

-(void) lockExposureAndFocus{

	[captureInput.device lockForConfiguration:nil];

	//if( [captureInput.device isExposureModeSupported:AVCaptureExposureModeLocked] ) [captureInput.device setExposureMode:AVCaptureExposureModeLocked ];
	if( [captureInput.device isFocusModeSupported:AVCaptureFocusModeLocked] )	[captureInput.device setFocusMode:AVCaptureFocusModeLocked ];


}

-(void)stopCapture{
	if(self.captureSession) {
		if(captureOutput){
			if(captureOutput.sampleBufferDelegate != nil) {
				[captureOutput setSampleBufferDelegate:nil queue:NULL];
			}
		}

		// remove the input and outputs from session
		for(AVCaptureInput *input1 in self.captureSession.inputs) {
		    [self.captureSession removeInput:input1];
		}
		for(AVCaptureOutput *output1 in self.captureSession.outputs) {
		    [self.captureSession removeOutput:output1];
		}

		[self.captureSession stopRunning];
	}
}

-(CGImageRef)getCurrentFrame{
	return currentFrame;
}

-(std::vector <std::string>)listDevices{
    std::vector <std::string> deviceNames;
	NSArray * devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	int i=0;
	for (AVCaptureDevice * captureDevice in devices){
        deviceNames.push_back([captureDevice.localizedName UTF8String]);
		 ofLogNotice() << "Device: " << i << ": " << deviceNames.back();
		i++;
    }
    return deviceNames;
}

-(void)setDevice:(int)_device{
	deviceID = _device;
}

#pragma mark -
#pragma mark AVCaptureSession delegate
- (void)captureOutput:(AVCaptureOutput *)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
	   fromConnection:(AVCaptureConnection *)connection
{
	if(grabberPtr != NULL) {
		@autoreleasepool {
			CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
			// Lock the image buffer
			CVPixelBufferLockBaseAddress(imageBuffer,0);

			if( grabberPtr != NULL && !grabberPtr->bLock ){

				unsigned char *isrc4 = (unsigned char *)CVPixelBufferGetBaseAddress(imageBuffer);
				size_t widthIn  = CVPixelBufferGetWidth(imageBuffer);
				size_t heightIn	= CVPixelBufferGetHeight(imageBuffer);

				if( widthIn != grabberPtr->getWidth() || heightIn != grabberPtr->getHeight() ){
					ofLogError("ofAVFoundationGrabber") << " incoming image dimensions " << widthIn << " by " << heightIn << " don't match. This shouldn't happen! Returning.";
					return;
				}

				if( grabberPtr->pixelFormat == OF_PIXELS_BGRA ){

					if( grabberPtr->capMutex.try_lock() ){
						grabberPtr->pixelsTmp.setFromPixels(isrc4, widthIn, heightIn, 4);
						grabberPtr->updatePixelsCB();
						grabberPtr->capMutex.unlock();
					}

				}else{

					ofPixels rgbConvertPixels;
					rgbConvertPixels.allocate(widthIn, heightIn, 3);

					vImage_Buffer srcImg;
					srcImg.width = widthIn;
					srcImg.height = heightIn;
					srcImg.data = isrc4;
					srcImg.rowBytes = CVPixelBufferGetBytesPerRow(imageBuffer);

					vImage_Buffer dstImg;
					dstImg.width = srcImg.width;
					dstImg.height = srcImg.height;
					dstImg.rowBytes = width*3;
					dstImg.data = rgbConvertPixels.getData();

					vImage_Error err;
					err = vImageConvert_BGRA8888toRGB888(&srcImg, &dstImg, kvImageNoFlags);
					if(err != kvImageNoError){
						ofLogError("ofAVFoundationGrabber") << "Error using accelerate to convert bgra to rgb with vImageConvert_BGRA8888toRGB888 error: " << err;
					}else{

						if( grabberPtr->capMutex.try_lock() ){
							grabberPtr->pixelsTmp = rgbConvertPixels;
							grabberPtr->updatePixelsCB();
							grabberPtr->capMutex.unlock();
						}

					}
				}

			// Unlock the image buffer
			CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);

			}
		}
	}
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	// Stop the CaptureSession
	if(self.captureSession) {
		[self stopCapture];
		self.captureSession = nil;
	}
	if(captureOutput){
		if(captureOutput.sampleBufferDelegate != nil) {
			[captureOutput setSampleBufferDelegate:nil queue:NULL];
		}
		[captureOutput release];
		captureOutput = nil;
	}

	captureInput = nil;
	device = nil;

	if(grabberPtr) {
		[self eraseGrabberPtr];
	}
	grabberPtr = nil;
	if(currentFrame) {
		// release the currentFrame image
		CGImageRelease(currentFrame);
		currentFrame = nil;
	}
    [super dealloc];
}

- (void)eraseGrabberPtr {
	grabberPtr = NULL;
}

@end


ofAVFoundationGrabber::ofAVFoundationGrabber(){
	fps		= -1;
	grabber = [[OSXVideoGrabber alloc] init];
    width = 0;
    height = 0;
	bIsInit = false;
	pixelFormat = OF_PIXELS_RGB;
	newFrame = false;
	bHavePixelsChanged = false;
	bLock = false;
}

ofAVFoundationGrabber::~ofAVFoundationGrabber(){
	ofLog(OF_LOG_VERBOSE, "ofAVFoundationGrabber destructor");
	close();
}

void ofAVFoundationGrabber::clear(){
	if( pixels.size() ){
		pixels.clear();
		pixelsTmp.clear();
	}
}

void ofAVFoundationGrabber::close(){
	bLock = true;
	if(grabber) {
		// Stop and release the the OSXVideoGrabber
		[grabber stopCapture];
		[grabber eraseGrabberPtr];
		[grabber release];
		grabber = nil;
	}
	clear();
	bIsInit = false;
	width = 0;
    height = 0;
	fps		= -1;
	pixelFormat = OF_PIXELS_RGB;
	newFrame = false;
	bHavePixelsChanged = false;
	bLock = false;
}

void ofAVFoundationGrabber::setDesiredFrameRate(int capRate){
	fps = capRate;
}

bool ofAVFoundationGrabber::setup(int w, int h){

	if( grabber == nil ){
		grabber = [[OSXVideoGrabber alloc] init];
	}

	grabber->grabberPtr = this;

	if( [grabber initCapture:fps capWidth:w capHeight:h] ) {

		//update the pixel dimensions based on what the camera supports
		width = grabber->width;
		height = grabber->height;

		clear();

		pixels.allocate(width, height, pixelFormat);
		pixelsTmp.allocate(width, height, pixelFormat);

		[grabber startCapture];

		newFrame=false;
		bIsInit = true;

		return true;
	} else {
		return false;
	}
}


bool ofAVFoundationGrabber::isInitialized() const{
    return bIsInit;
}

void ofAVFoundationGrabber::update(){
	newFrame = false;

	if (bHavePixelsChanged == true){
		capMutex.lock();
			pixels = pixelsTmp;
			bHavePixelsChanged = false;
		capMutex.unlock();
		newFrame = true;
	}
}

ofPixels & ofAVFoundationGrabber::getPixels(){
	return pixels;
}

const ofPixels & ofAVFoundationGrabber::getPixels() const{
	return pixels;
}

bool ofAVFoundationGrabber::isFrameNew() const{
	return newFrame;
}

void ofAVFoundationGrabber::updatePixelsCB(){
	//TODO: does this need a mutex? or some thread protection?
	bHavePixelsChanged = true;
}

std::vector <ofVideoDevice> ofAVFoundationGrabber::listDevices() const{
	std::vector <std::string> devList = [grabber listDevices];

    std::vector <ofVideoDevice> devices;
    for(int i = 0; i < devList.size(); i++){
        ofVideoDevice vd;
        vd.deviceName = devList[i];
        vd.id = i;
        vd.bAvailable = true;
        devices.push_back(vd);
    }

    return devices;
}

void ofAVFoundationGrabber::setDeviceID(int deviceID) {
	if( grabber == nil ){
		grabber = [[OSXVideoGrabber alloc] init];
	}
	[grabber setDevice:deviceID];
	device = deviceID;
}

bool ofAVFoundationGrabber::setPixelFormat(ofPixelFormat PixelFormat) {
	if(PixelFormat == OF_PIXELS_RGB){
		pixelFormat = PixelFormat;
		return true;
	} else if(PixelFormat == OF_PIXELS_RGBA){
		pixelFormat = PixelFormat;
		return true;
	} else if(PixelFormat == OF_PIXELS_BGRA){
		pixelFormat = PixelFormat;
		return true;
	}
	return false;
}

ofPixelFormat ofAVFoundationGrabber::getPixelFormat() const{
	return pixelFormat;
}

#endif
