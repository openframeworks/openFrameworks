/*
 *  ofxiPhoneVideoGrabber.mm
 */

#include "ofxiPhoneVideoGrabber.h"

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>


#if __IPHONE_OS_VERSION_MIN_REQUIRED > __IPHONE_3_2

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
	return self;
}

- (void)initCapture:(int)framerate capWidth:(int)w capHeight:(int)h{
	/*We setup the input*/
	captureInput						= [AVCaptureDeviceInput 
										  deviceInputWithDevice:[AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo] 
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

	[self.captureSession setSessionPreset:preset]; 
	
	/*We add input and output*/
	[self.captureSession addInput:captureInput];
	[self.captureSession addOutput:captureOutput];
	
	[self.captureSession commitConfiguration];		
	[self.captureSession startRunning];

	bInitCalled = true;
	
}

-(void) startCapture{

	if( !bInitCalled ){
		[self initCapture:15 capWidth:480 capHeight:320];
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

#pragma mark -
#pragma mark AVCaptureSession delegate
- (void)captureOutput:(AVCaptureOutput *)captureOutput 
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer 
	   fromConnection:(AVCaptureConnection *)connection 
{ 
	/*We create an autorelease pool because as we are not in the main_queue our code is
	 not executed in the main thread. So we have to create an autorelease pool for the thread we are in*/
	
	//printf("capturing!\n");	
	
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer); 
    /*Lock the image buffer*/
    CVPixelBufferLockBaseAddress(imageBuffer,0); 

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
	
	grabberPtr->updatePixelsCB(currentFrame);
	
	[pool drain];
} 

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
    [super dealloc];
}


@end


ofxiPhoneVideoGrabber::ofxiPhoneVideoGrabber(){
	fps		= 15;
	grabber = [iPhoneVideoGrabber alloc];
	pixels	= NULL;
}

ofxiPhoneVideoGrabber::~ofxiPhoneVideoGrabber(){
	[grabber stopCapture];
	clear();
}
		
void ofxiPhoneVideoGrabber::clear(){
	if( pixels != NULL ){
		delete[] pixels;
		pixels = NULL;
	}
	tex.clear();
}

void ofxiPhoneVideoGrabber::setCaptureRate(int capRate){
	fps = capRate;
}

void ofxiPhoneVideoGrabber::initGrabber(int w, int h){

	[grabber initCapture:fps capWidth:w capHeight:h];
	grabber->grabberPtr = this;
	
	width	= grabber->width;
	height	= grabber->height;
	
	clear();
	
	pixels = new unsigned char[width * width * 3];
	memset(pixels, 0, width*height*3);
	
	tex.allocate(width, height, GL_RGB);
		
	[grabber startCapture];
	
	bUpdateTex = true;
}

void ofxiPhoneVideoGrabber::updatePixelsCB( CGImageRef & ref ){
	bUpdateTex = convertCGImageToPixels(ref, pixels);
}
		
void ofxiPhoneVideoGrabber::draw(float x, float y){
	draw(x, y, width, height);
}

void ofxiPhoneVideoGrabber::draw(float x, float y, float w, float h){
	if( bUpdateTex ){
		tex.loadData(pixels, w, h, GL_RGB);
		bUpdateTex = false;
	}
	tex.draw(x, y, w, h);
}

bool ofxiPhoneVideoGrabber::convertCGImageToPixels(CGImageRef & ref, unsigned char * pixels){
	CGContextRef spriteContext;

	int bytesPerPixel	= CGImageGetBitsPerPixel(ref)/8;
	if(bytesPerPixel == 3) bytesPerPixel = 4;
	
	int w			= CGImageGetWidth(ref);
	int h			= CGImageGetHeight(ref);
	
	// Allocated memory needed for the bitmap context
	GLubyte *pixelsTmp	= (GLubyte *) malloc(w * h * bytesPerPixel);
	
	// Uses the bitmatp creation function provided by the Core Graphics framework. 
	spriteContext = CGBitmapContextCreate(pixelsTmp, w, h, CGImageGetBitsPerComponent(ref), w * bytesPerPixel, CGImageGetColorSpace(ref), bytesPerPixel == 4 ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNone);
	
	if(spriteContext == NULL) {
		ofLog(OF_LOG_ERROR, "convertCGImageToPixels - CGBitmapContextCreate returned NULL");
		free(pixelsTmp);
		return false;
	}

	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)w, (CGFloat)h), ref);
	CGContextRelease(spriteContext);
	
	int totalSrcBytes = w*h*bytesPerPixel;  
	int j = 0;
	for(int k = 0; k < totalSrcBytes; k+= bytesPerPixel ){
		pixels[j  ] = pixelsTmp[k  ];
		pixels[j+1] = pixelsTmp[k+1];
		pixels[j+2] = pixelsTmp[k+2];
		
		j+=3;
	}
					
	free(pixelsTmp);
	return true;
}

#endif	// (__arm__) compile only for ARM

#else   // (> 3.2 SDK) compile for 4.0+

#warning "skipping ofxIphoneVideoGrabber compilation because you need > 3.2 iOS SDK"

#endif
