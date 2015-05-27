/*
 *  AVFoundationVideoGrabber.mm
 */

#include "AVFoundationVideoGrabber.h"

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#include "ofxiOSExtras.h"
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>

#define IS_IOS_7_OR_LATER    ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
#define IS_IOS_6_OR_LATER    ([[[UIDevice currentDevice] systemVersion] floatValue] >= 6.0)

#if TARGET_IPHONE_SIMULATOR
#warning Target = iOS Simulator - The AVFoundationVideoGrabber will not function on the iOS Simulator
#endif

@interface iOSVideoGrabber() <AVCaptureVideoDataOutputSampleBufferDelegate> {
	AVCaptureDeviceInput		*captureInput;	
	AVCaptureVideoDataOutput    *captureOutput;
	AVCaptureDevice				*device;
}
@property (nonatomic, retain) AVCaptureSession *captureSession;
@end

@implementation iOSVideoGrabber

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
		
		// iOS 7+ way of dealing with framerates.
        if(IS_IOS_7_OR_LATER) {
            #ifdef __IPHONE_7_0
			NSError *error = nil;
			[device lockForConfiguration:&error];
			if(!error) {
				NSArray * supportedFrameRates = device.activeFormat.videoSupportedFrameRateRanges;
				float minFrameRate = 1;
				float maxFrameRate = 1;
				for(AVFrameRateRange * range in supportedFrameRates) {
					minFrameRate = range.minFrameRate;
					maxFrameRate = range.maxFrameRate;
					break;
				}
				if(framerate < minFrameRate) {
					NSLog(@"iOSVideoGrabber: Framerate set is less than minimum. Setting to Minimum");
					framerate = minFrameRate;
				}
				if(framerate > maxFrameRate) {
					NSLog(@"iOSVideoGrabber: Framerate set is greater than maximum. Setting to Maximum");
					framerate = maxFrameRate;
				}
				device.activeVideoMinFrameDuration = CMTimeMake(1, framerate);
				device.activeVideoMaxFrameDuration = CMTimeMake(1, framerate);
				[device unlockForConfiguration];
			} else {
				NSLog(@"iOSVideoGrabber Init Error: %@", error);
			}
            #endif
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
		
		// Set the video output to store frame in BGRA (It is supposed to be faster)
		NSString* key = (NSString*)kCVPixelBufferPixelFormatTypeKey; 
		NSNumber* value = [NSNumber numberWithUnsignedInt:kCVPixelFormatType_32BGRA]; 

		NSDictionary* videoSettings = [NSDictionary dictionaryWithObject:value forKey:key]; 
		[captureOutput setVideoSettings:videoSettings]; 

		// And we create a capture session
		if(self.captureSession) {
			self.captureSession = nil;
		}
		self.captureSession = [[[AVCaptureSession alloc] init] autorelease];
		
		[self.captureSession beginConfiguration]; 
		
		
		
		NSString * preset = AVCaptureSessionPresetMedium;
		width	= 480;
		height	= 360;	
		
		if(ofxiOSGetDeviceRevision() == ofxiOS_DEVICE_IPHONE_3G) {
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
			else if( w == 1920 && h == 1080 ){
				preset = AVCaptureSessionPreset1920x1080;
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
		
		// We add input and output
		[self.captureSession addInput:captureInput];
		[self.captureSession addOutput:captureOutput];
		
		// We specify a minimum duration for each frame (play with this settings to avoid having too many frames waiting
		// in the queue because it can cause memory issues). It is similar to the inverse of the maximum framerate.
		// In this example we set a min frame duration of 1/10 seconds so a maximum framerate of 10fps. We say that
		// we are not able to process more than 10 frames per second.
		// Called after added to captureSession
        
        if(IS_IOS_7_OR_LATER == false) {
            if(IS_IOS_6_OR_LATER) {
                #ifdef __IPHONE_6_0
                AVCaptureConnection *conn = [captureOutput connectionWithMediaType:AVMediaTypeVideo];
                if ([conn isVideoMinFrameDurationSupported] == YES &&
                    [conn isVideoMaxFrameDurationSupported] == YES) { // iOS 6+
                        [conn setVideoMinFrameDuration:CMTimeMake(1, framerate)];
                        [conn setVideoMaxFrameDuration:CMTimeMake(1, framerate)];
                }
                #endif
            } else { // iOS 5 or earlier
                [captureOutput setMinFrameDuration:CMTimeMake(1, framerate)];
            }
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

	if( !bInitCalled ){
		[self initCapture:30 capWidth:480 capHeight:320];
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

-(vector <string>)listDevices{
    vector <string> deviceNames;
	NSArray * devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	int i=0;
	for (AVCaptureDevice * captureDevice in devices){
        deviceNames.push_back(ofxNSStringToString(captureDevice.localizedName));
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

			if(grabberPtr != NULL && grabberPtr->internalGlDataType == GL_BGRA) {
				
				unsigned int *isrc4 = (unsigned int *)CVPixelBufferGetBaseAddress(imageBuffer); 
				
				unsigned int *idst4 = (unsigned int *)grabberPtr->pixels;
				unsigned int *ilast4 = &isrc4[width*height-1];
				while (isrc4 < ilast4){
					*(idst4++) = *(isrc4++);
				}
				grabberPtr->newFrame=true;
				CVPixelBufferUnlockBaseAddress(imageBuffer,0);
				
			} else {
				// Get information about the image
				uint8_t *baseAddress	= (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer); 
				size_t bytesPerRow		= CVPixelBufferGetBytesPerRow(imageBuffer); 
				size_t widthIn			= CVPixelBufferGetWidth(imageBuffer); 
				size_t heightIn			= CVPixelBufferGetHeight(imageBuffer);  
					
				// Create a CGImageRef from the CVImageBufferRef
				CGColorSpaceRef colorSpace	= CGColorSpaceCreateDeviceRGB(); 
				
				CGContextRef newContext		= CGBitmapContextCreate(baseAddress, widthIn, heightIn, 8, bytesPerRow, colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
				CGImageRef newImage			= CGBitmapContextCreateImage(newContext); 

				CGImageRelease(currentFrame);	
				currentFrame = CGImageCreateCopy(newImage);		
					
				// We release some components
				CGContextRelease(newContext); 
				CGColorSpaceRelease(colorSpace);

				// We relase the CGImageRef
				CGImageRelease(newImage);
					
				// We unlock the  image buffer
				CVPixelBufferUnlockBaseAddress(imageBuffer,0);
				
				if(grabberPtr != NULL && grabberPtr->bLock != true) {
					grabberPtr->updatePixelsCB(currentFrame);
				}
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


AVFoundationVideoGrabber::AVFoundationVideoGrabber(){
	fps		= 30;
	grabber = [iOSVideoGrabber alloc];
	pixels	= NULL;
    width = 0;
    height = 0;
	
	internalGlDataType = GL_RGB;
	newFrame = false;
	bHavePixelsChanged = false;
	bLock = false;
}

AVFoundationVideoGrabber::~AVFoundationVideoGrabber(){
	ofLog(OF_LOG_VERBOSE, "AVFoundationVideoGrabber dtor");
	bLock = true;
	if(grabber) {
		// Stop and release the the iOSVideoGrabber
		[grabber stopCapture];
		[grabber eraseGrabberPtr];
		[grabber release];
		grabber = nil;
	}
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
		
		if(ofGetOrientation() == OF_ORIENTATION_DEFAULT || ofGetOrientation() == OF_ORIENTATION_180) {
			width = grabber->height;
			height = grabber->width;
		} else {
			width	= grabber->width;
			height	= grabber->height;
		}
		
		clear();
		
		pixelsTmp	= (GLubyte *) malloc(width * height * 4);

		if(internalGlDataType == GL_RGB) {
			pixels = (GLubyte *) malloc(width * height * 3);//new unsigned char[width * width * 3];//memset(pixels, 0, width*height*3);
		} else if(internalGlDataType == GL_RGBA) {
			pixels = (GLubyte *) malloc(width * height * 4);
		} else if(internalGlDataType == GL_BGRA) {
			pixels = (GLubyte *) malloc(width * height * 4);
		}
			
		[grabber startCapture];
		
		newFrame=false;
		bIsInit = true;
        
		return true;
	} else {
		return false;
	}
}


bool AVFoundationVideoGrabber::isInitialized(){
    return bIsInit;
}

void AVFoundationVideoGrabber::update(){
	newFrame = false;
	if (bHavePixelsChanged == true){
		newFrame = true;
		bHavePixelsChanged = false;
	}
}

void AVFoundationVideoGrabber::updatePixelsCB( CGImageRef & ref ){
	
	if(bLock) {
		return;
	}
	
	CGAffineTransform transform = CGAffineTransformIdentity;
	
	CGContextRef spriteContext;
	if(pixelsTmp != NULL) {
		// Uses the bitmap creation function provided by the Core Graphics framework. 
		spriteContext = CGBitmapContextCreate(pixelsTmp, width, height, CGImageGetBitsPerComponent(ref), width * 4, CGImageGetColorSpace(ref), kCGImageAlphaPremultipliedLast);
		
		if(ofGetOrientation() == OF_ORIENTATION_DEFAULT) {
			transform = CGAffineTransformMakeTranslation(0.0, height);
			transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
				
			CGContextConcatCTM(spriteContext, transform);
			CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)height, (CGFloat)width), ref);
		} else if(ofGetOrientation() == OF_ORIENTATION_180) {
			transform = CGAffineTransformMakeTranslation(width, 0.0);
			transform = CGAffineTransformRotate(transform, M_PI / 2.0);
			
			CGContextConcatCTM(spriteContext, transform);
			CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)height, (CGFloat)width), ref);
		} else if(ofGetOrientation() == OF_ORIENTATION_90_LEFT) {
			CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), ref);
		} else { // landscape RIGHT
			transform = CGAffineTransformMakeTranslation(width, height);
			transform = CGAffineTransformScale(transform, -1.0, -1.0);
			
			CGContextConcatCTM(spriteContext, transform);
			CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), ref);
		}
		
		if(bLock) {
			return;
		}
		
		CGContextRelease(spriteContext);
		
		if(internalGlDataType == GL_RGB) {
			unsigned int *isrc4 = (unsigned int *)pixelsTmp;
			unsigned int *idst3 = (unsigned int *)pixels;
			unsigned int *ilast4 = &isrc4[width*height-1];
			while (isrc4 < ilast4){
				if(bLock) {
					return;
				}
				*(idst3++) = *(isrc4++);
				idst3 = (unsigned int *) (((unsigned char *) idst3) - 1);
			}
		}
		else if(internalGlDataType == GL_RGBA || internalGlDataType == GL_BGRA){
			if(bLock) {
				return;
			}
			memcpy(pixels, pixelsTmp, width*height*4);
		}
		
		bHavePixelsChanged=true;
	}
}

bool AVFoundationVideoGrabber::isFrameNew() {
	return newFrame;
}

vector <ofVideoDevice> AVFoundationVideoGrabber::listDevices() {
	vector <string> devList = [grabber listDevices];
    
    vector <ofVideoDevice> devices; 
    for(int i = 0; i < devList.size(); i++){
        ofVideoDevice vd; 
        vd.deviceName = devList[i]; 
        vd.id = i;  
        vd.bAvailable = true; 
        devices.push_back(vd); 
    }
    
    return devices; 
}

void AVFoundationVideoGrabber::setDevice(int deviceID) {
	[grabber setDevice:deviceID];
	device = deviceID;
}

bool AVFoundationVideoGrabber::setPixelFormat(ofPixelFormat PixelFormat) {
	if(PixelFormat == OF_PIXELS_RGB){
		internalGlDataType = GL_RGB;
		return true;
	} else if(PixelFormat == OF_PIXELS_RGBA){
		internalGlDataType = GL_RGBA;
		return true;
	} else if(PixelFormat == OF_PIXELS_BGRA){
		internalGlDataType = GL_BGRA;
		return true;
	}
	return false;
}

ofPixelFormat AVFoundationVideoGrabber::getPixelFormat() {
	if(internalGlDataType == GL_RGB){
        return OF_PIXELS_RGB;
	} else if(internalGlDataType == GL_RGBA){
        return OF_PIXELS_RGBA;
	} else if(internalGlDataType == GL_BGRA){
        return OF_PIXELS_BGRA;
    } else {
        return OF_PIXELS_RGB;
	}
}



