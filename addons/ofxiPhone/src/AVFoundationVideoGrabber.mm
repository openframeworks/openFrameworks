/*
 *  AVFoundationVideoGrabber.mm
 */

#include "AVFoundationVideoGrabber.h"

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#include "ofxiPhoneExtras.h"

//#if __IPHONE_OS_VERSION_MIN_REQUIRED > __IPHONE_3_2

#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>

#if defined  __arm__

@implementation iPhoneVideoGrabber

@synthesize captureSession	= _captureSession;

#pragma mark -
#pragma mark Initialization
- (id)init {
	self = [super init];
//	if (self) {
//		/*We initialize some variables (they might be not initialized depending on what is commented or not)*/
//		self.imageView = nil;
//		self.prevLayer = nil;
//		self.customLayer = nil;
//	}
	bInitCalled = false;
	grabberPtr=NULL;
	device=0;
	return self;
}

- (bool)initCapture:(int)framerate capWidth:(int)w capHeight:(int)h{
	NSArray * devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	
	if([devices count] > 0)
	{
		if(device>[devices count]-1)
			device = [devices count]-1;
		
		/*We setup the input*/
		/*captureInput						= [AVCaptureDeviceInput 
											  deviceInputWithDevice:[AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo] 
											  error:nil];*/
		
		captureInput						= [AVCaptureDeviceInput 
											   deviceInputWithDevice:[devices objectAtIndex:device]
											   error:nil];
												
		/*We setupt the output*/
		AVCaptureVideoDataOutput *captureOutput = [[AVCaptureVideoDataOutput alloc] init];
		/*While a frame is processes in -captureOutput:didOutputSampleBuffer:fromConnection: delegate methods no other frames are added in the queue.
		 If you don't want this behaviour set the property to NO */
		captureOutput.alwaysDiscardsLateVideoFrames = YES; 
		/*We specify a minimum duration for each frame (play with this settings to avoid having too many frames waiting
		 in the queue because it can cause memory issues). It is similar to the inverse of the maximum framerate.
		 In this example we set a min frame duration of 1/10 seconds so a maximum framerate of 10fps. We say that
		 we are not able to process more than 10 frames per second.*/
		captureOutput.minFrameDuration = CMTimeMake(1, framerate);
		
		/*We create a serial queue to handle the processing of our frames*/
		dispatch_queue_t queue;
		queue = dispatch_queue_create("cameraQueue", NULL);
		[captureOutput setSampleBufferDelegate:self queue:queue];
		dispatch_release(queue);
		
		// Set the video output to store frame in BGRA (It is supposed to be faster)
		NSString* key = (NSString*)kCVPixelBufferPixelFormatTypeKey; 
		NSNumber* value = [NSNumber numberWithUnsignedInt:kCVPixelFormatType_32BGRA]; 

		NSDictionary* videoSettings = [NSDictionary dictionaryWithObject:value forKey:key]; 
		[captureOutput setVideoSettings:videoSettings]; 

		/*And we create a capture session*/	
		self.captureSession = [[AVCaptureSession alloc] init];
		
		[self.captureSession beginConfiguration]; 
		
		
		
		NSString * preset = AVCaptureSessionPresetMedium;
		width	= 480;
		height	= 360;	
		
		if(ofxiPhoneGetDeviceRevision() == OFXIPHONE_DEVICE_IPHONE_3G) {
			width = 400;
			height = 304;
		}
		else {
			if( w == 640 && h == 480 ){
				preset = AVCaptureSessionPreset640x480;
				width	= w;
				height	= h;
			}
			else if( w == 1280 && h == 720 ){
				preset = AVCaptureSessionPreset1280x720;
				width	= w;
				height	= h;		
			}
			else if( w == 192 && h == 144 ){
				preset = AVCaptureSessionPresetLow;
				width	= w;
				height	= h;		
			}
		}
		[self.captureSession setSessionPreset:preset]; 
		
		/*We add input and output*/
		[self.captureSession addInput:captureInput];
		[self.captureSession addOutput:captureOutput];
		
		[self.captureSession commitConfiguration];		
		[self.captureSession startRunning];

		bInitCalled = true;
		return true;
	}
	return false;
}

-(void) startCapture{

	if( !bInitCalled ){
		[self initCapture:60 capWidth:480 capHeight:320];
	}

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
	[self.captureSession stopRunning];
}

-(CGImageRef)getCurrentFrame{
	return currentFrame;
}

-(void)listDevices{
	NSArray * devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	int i=0;
	for (AVCaptureDevice * captureDevice in devices){
		 cout<<"Device "<<i<<": "<<ofxNSStringToString(captureDevice.localizedName)<<endl;
		i++;
    }
}
-(void)setDevice:(int)_device{
	device = _device;
}

#pragma mark -
#pragma mark AVCaptureSession delegate
- (void)captureOutput:(AVCaptureOutput *)captureOutput 
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer 
	   fromConnection:(AVCaptureConnection *)connection 
{ 
	if(grabberPtr != NULL) {
		NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
		
		CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer); 
		/*Lock the image buffer*/
		CVPixelBufferLockBaseAddress(imageBuffer,0); 

		if(grabberPtr->internalGlDataType == GL_BGRA) {	
			
			unsigned int *isrc4 = (unsigned int *)CVPixelBufferGetBaseAddress(imageBuffer); 
			
			unsigned int *idst4 = (unsigned int *)grabberPtr->pixels;
			unsigned int *ilast4 = &isrc4[width*height-1];
			while (isrc4 < ilast4){
				*(idst4++) = *(isrc4++);
			}
			grabberPtr->newFrame=true;
			CVPixelBufferUnlockBaseAddress(imageBuffer,0);
			
		}
		else {
			/*Get information about the image*/
			uint8_t *baseAddress	= (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer); 
			size_t bytesPerRow		= CVPixelBufferGetBytesPerRow(imageBuffer); 
			size_t widthIn			= CVPixelBufferGetWidth(imageBuffer); 
			size_t heightIn			= CVPixelBufferGetHeight(imageBuffer);  
				
			/*Create a CGImageRef from the CVImageBufferRef*/
			
			CGColorSpaceRef colorSpace	= CGColorSpaceCreateDeviceRGB(); 
			
			CGContextRef newContext		= CGBitmapContextCreate(baseAddress, widthIn, heightIn, 8, bytesPerRow, colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
			CGImageRef newImage			= CGBitmapContextCreateImage(newContext); 

			CGImageRelease(currentFrame);	
			currentFrame = CGImageCreateCopy(newImage);		
				
			/*We release some components*/
			CGContextRelease(newContext); 
			CGColorSpaceRelease(colorSpace);

			/*We relase the CGImageRef*/
			CGImageRelease(newImage);
				
			/*We unlock the  image buffer*/
			CVPixelBufferUnlockBaseAddress(imageBuffer,0);
			
			grabberPtr->updatePixelsCB(currentFrame); // this is an issue if the class is deleted before the object is removed on quit etc.
		}
		[pool drain];
	}
} 

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
    [super dealloc];
}

- (void)eraseGrabberPtr {
	grabberPtr = NULL;
}

@end


AVFoundationVideoGrabber::AVFoundationVideoGrabber(){
	fps		= 60;
	grabber = [iPhoneVideoGrabber alloc];
	pixels	= NULL;
	
	internalGlDataType = GL_RGB;
	newFrame=false;
	bHavePixelsChanged = false;
}

AVFoundationVideoGrabber::~AVFoundationVideoGrabber(){
	[grabber eraseGrabberPtr];
	[grabber stopCapture];
	clear();
}
		
void AVFoundationVideoGrabber::clear(){
	if( pixels != NULL ){
		free(pixels);
		pixels = NULL;
		free(pixelsTmp);
	}
	//tex.clear();
}

void AVFoundationVideoGrabber::setCaptureRate(int capRate){
	fps = capRate;
}

bool AVFoundationVideoGrabber::initGrabber(int w, int h){

	if( [grabber initCapture:fps capWidth:w capHeight:h] ) {
		grabber->grabberPtr = this;
		
		if(ofxiPhoneGetOrientation() == UIDeviceOrientationPortrait || ofxiPhoneGetOrientation() == UIDeviceOrientationPortraitUpsideDown) {
			width = grabber->height;
			height = grabber->width;
		}
		else {
			width	= grabber->width;
			height	= grabber->height;
		}
		
		clear();
		
		pixelsTmp	= (GLubyte *) malloc(width * height * 4);

		if(internalGlDataType == GL_RGB)
			pixels = (GLubyte *) malloc(width * height * 3);//new unsigned char[width * width * 3];//memset(pixels, 0, width*height*3);
		else if(internalGlDataType == GL_RGBA)
			pixels = (GLubyte *) malloc(width * height * 4);
		else if(internalGlDataType == GL_BGRA)
			pixels = (GLubyte *) malloc(width * height * 4);
			
		[grabber startCapture];
		
		newFrame=false;
		
		return true;
	}
	else
		return false;
}

void AVFoundationVideoGrabber::update(){
	newFrame = false;
	if (bHavePixelsChanged == true){
		newFrame = true;
		bHavePixelsChanged = false;
	}
}

void AVFoundationVideoGrabber::updatePixelsCB( CGImageRef & ref ){
	
	CGAffineTransform transform = CGAffineTransformIdentity;
	
	CGContextRef spriteContext;
		
	// Uses the bitmap creation function provided by the Core Graphics framework. 
	spriteContext = CGBitmapContextCreate(pixelsTmp, width, height, CGImageGetBitsPerComponent(ref), width * 4, CGImageGetColorSpace(ref), kCGImageAlphaPremultipliedLast);
	
	if(ofxiPhoneGetOrientation() == UIDeviceOrientationPortrait) {
			transform = CGAffineTransformMakeTranslation(0.0, height);
			transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
			
			CGContextConcatCTM(spriteContext, transform);
			CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)height, (CGFloat)width), ref);
	}
	else if(ofxiPhoneGetOrientation() == UIDeviceOrientationPortraitUpsideDown) {
		ofLog(OF_LOG_WARNING, "upsidedown orientation not supported at this time"); // TODO support this
	}
	else if(ofxiPhoneGetOrientation() == UIDeviceOrientationLandscapeLeft) {
		transform = CGAffineTransformMakeTranslation(width, height);
		transform = CGAffineTransformScale(transform, -1.0, -1.0);
		
		CGContextConcatCTM(spriteContext, transform);
		CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), ref);
	}
	else // landscape RIGHT
		CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), ref);
	
	CGContextRelease(spriteContext);
	
	if(internalGlDataType == GL_RGB)
	{
		unsigned int *isrc4 = (unsigned int *)pixelsTmp;
		unsigned int *idst3 = (unsigned int *)pixels;
		unsigned int *ilast4 = &isrc4[width*height-1];
		while (isrc4 < ilast4){
			*(idst3++) = *(isrc4++);
			idst3 = (unsigned int *) (((unsigned char *) idst3) - 1);
		}
	}
	else if(internalGlDataType == GL_RGBA || internalGlDataType == GL_BGRA)
		memcpy(pixels, pixelsTmp, width*height*4);
	
	bHavePixelsChanged=true;
}

bool AVFoundationVideoGrabber::isFrameNew()
{
	return newFrame;
}

void AVFoundationVideoGrabber::listDevices() {
	[grabber listDevices];
}

void AVFoundationVideoGrabber::setDevice(int deviceID) {
	[grabber setDevice:deviceID];
	device = deviceID;
}

void AVFoundationVideoGrabber::setPixelFormat(ofPixelFormat PixelFormat) {
	if(PixelFormat == OF_PIXELS_RGB)
		internalGlDataType = GL_RGB;
	else if(PixelFormat == OF_PIXELS_RGBA)
		internalGlDataType = GL_RGBA;
	else if(PixelFormat == OF_PIXELS_BGRA)
		internalGlDataType = GL_BGRA;
}

ofPixelFormat AVFoundationVideoGrabber::getPixelFormat() {
	if(internalGlDataType == GL_RGB)
        return OF_PIXELS_RGB;
	else if(internalGlDataType == GL_RGBA)
        return OF_PIXELS_RGBA;
	else if(internalGlDataType == GL_BGRA)
        return OF_PIXELS_BGRA;
    else
        return OF_PIXELS_RGB;
}

#endif	// (__arm__) compile only for ARM

//#else   // compile for 4.0+
//
//#warning "skipping AVFoundationVideoGrabber compilation because you need > 3.2 iOS SDK"
//
//#endif
