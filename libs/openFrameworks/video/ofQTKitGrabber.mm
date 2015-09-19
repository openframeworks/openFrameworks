
#include "ofQTKitGrabber.h"
#import <QTKit/QTKit.h>
#import <QuickTime/QuickTime.h>
#import <Accelerate/Accelerate.h>

//This Objective-C class contains all the OS specific
//Stuff we need for pulling images from a video camera feed
//the QTCaptureVideoPreviewOutput is a non-blocking frame receiver delegate
//for a QTCaptureSession
//All you wanted to know and more about QTKit Capture can be found here:
// http://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/QTKitApplicationProgrammingGuide/UsingQTKit/UsingQTKit.html

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#if defined(MAC_OS_X_VERSION_10_9)
	#warning Using QTKit, which is deprecated in OSX 10.9
#endif

@interface QTKitVideoGrabber : QTCaptureVideoPreviewOutput
{
    QTCaptureSession *session;
    QTCaptureDeviceInput *videoDeviceInput;
    QTCaptureDeviceInput *audioDeviceInput;
    QTCaptureDevice* selectedVideoDevice;
    QTCaptureDevice* selectedAudioDevice;
    QTCaptureMovieFileOutput *captureMovieFileOutput;
    NSInteger width, height;
    NSInteger videoDeviceID, audioDeviceID;

    CVImageBufferRef cvFrame;
    ofPixels* pixels;
	ofQTKitGrabber* grabber;
	
    BOOL hasNewFrame;
    BOOL isFrameNew;
    BOOL isRecording;
    BOOL isRecordReady;
    BOOL useAudio;
}

@property(nonatomic, readonly) NSInteger height;
@property(nonatomic, readonly) NSInteger width;
@property(nonatomic, readwrite) NSInteger videoDeviceID;
@property(nonatomic, readwrite) NSInteger audioDeviceID;
@property(retain) QTCaptureSession* session;
@property(nonatomic, retain) QTCaptureDeviceInput* videoDeviceInput;
@property(nonatomic, retain) QTCaptureDeviceInput* audioDeviceInput;
@property(nonatomic, retain) QTCaptureMovieFileOutput *captureMovieFileOutput;
@property(nonatomic, readonly) BOOL isRunning;
@property(readonly) BOOL isFrameNew;
@property(readonly) BOOL isRecording;
@property(readonly) BOOL isRecordReady;
@property(nonatomic, readwrite) BOOL useAudio;
@property(nonatomic, readwrite) ofQTKitGrabber* grabber; //for event reference setting

//Static list helpsers
+ (NSArray*) listVideoDevices;
+ (NSArray*) listAudioDevices;
+ (NSArray*) listVideoCodecs;
+ (NSArray*) listAudioCodecs;

+ (void) enumerateArray:(NSArray*)someArray;
+ (NSInteger) getIndexofStringInArray:(NSArray*)someArray stringToFind:(NSString*)someStringDescription;

//initialization
- (void) setVideoDeviceID:(NSInteger)_videoDeviceID;
- (void) setAudioDeviceID:(NSInteger)_audioDeviceID; //audio device is only used for recording

//Create the recorder, flexible to be previewless and
//
- (id) initWithWidth:(NSInteger)_width
              height:(NSInteger)_height
         videodevice:(NSInteger)_videoDeviceID
         audiodevice:(NSInteger)_audioDeviceID
          usingAudio:(BOOL)_useAudio
	   capturePixels:(BOOL)_shouldCapturePixels
           pixelsRef:(ofPixelsRef)_pixels;


//life cycle
- (void) startSession;
- (void) update;
- (void) stop;

//delegate method for getting pixels
- (void) outputVideoFrame:(CVImageBufferRef)videoFrame 
		 withSampleBuffer:(QTSampleBuffer *)sampleBuffer 
		   fromConnection:(QTCaptureConnection *)connection;

//Recording
- (BOOL) initRecording:(NSString*)_selectedVideoCodec audioCodec:(NSString*)_selectedAudioCodec;
- (void) setVideoCodec:(NSString*)_selectedVideoCodec;
- (void) setAudioCodec:(NSString*)_selectedAudioCodec;
- (void) startRecording:(NSString*)filePath;
- (void) stopRecording;
- (void) captureOutput:(QTCaptureFileOutput *)captureOutput didFinishRecordingToOutputFileAtURL:(NSURL *)outputFileURL forConnections:(NSArray *)connections dueToError:(NSError *)error;

//internal methods 
- (BOOL) setSelectedVideoDevice:(QTCaptureDevice *)selectedVideoDevice;
- (BOOL) setSelectedAudioDevice:(QTCaptureDevice *)selectedAudioDevice;


@end

@implementation QTKitVideoGrabber
@synthesize width, height;
@synthesize session;
@synthesize videoDeviceID;
@synthesize audioDeviceID;
@synthesize videoDeviceInput;
@synthesize audioDeviceInput;
@synthesize captureMovieFileOutput;
@synthesize isFrameNew;
@synthesize isRecording;
@synthesize isRecordReady;
@synthesize useAudio;
@synthesize grabber;

+ (void) enumerateArray:(NSArray*)someArray
{
	NSInteger count = 0;
	for (id object in someArray) {
        ofLogVerbose("ofQtKitGrabber") << count << " - " << [[object description] UTF8String];
		count++;
	}
}

+ (NSInteger) getIndexofStringInArray:(NSArray*)someArray stringToFind:(NSString*)someStringDescription
{
	NSInteger index = -1;
	
	for (id object in someArray) {
		if ([[object description] isEqualToString:someStringDescription]) {
			return index;
		}
	}
	return -1;
}

+ (NSArray*) listVideoDevices
{
    //create a session for enumerating devices
	NSArray* videoDevices = [[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo]
							 arrayByAddingObjectsFromArray:[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeMuxed]];
	
    ofLogVerbose("ofQTKitGrabber") << "listing video devices:";
	[self enumerateArray:videoDevices];
	
	return videoDevices;
	
}

+ (NSArray*) listAudioDevices
{
    //create a session for enumerating devices
	NSArray* audioDevices = [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeSound];
	
    ofLogVerbose("ofQTKitGrabber") << "listing audio devices:";
	[self enumerateArray:audioDevices];
	
	return audioDevices;
}

- (id) initWithWidth:(NSInteger)_width
              height:(NSInteger)_height
         videodevice:(NSInteger)_videoDeviceID
         audiodevice:(NSInteger)_audioDeviceID
          usingAudio:(BOOL)_useAudio
	   capturePixels:(BOOL)_shouldCapturePixels
           pixelsRef:(ofPixelsRef)_pixels
{
	if((self = [super init])){
		//configure self
		width = _width;
		height = _height;
		
		//instance variables
		cvFrame = NULL;
		hasNewFrame = NO;
		self.useAudio = _useAudio;
		isRecordReady = NO;
		isRecording = NO;
		
        NSMutableDictionary* pixelBufferAttributes = [NSMutableDictionary dictionary];
        //we can turn capture pixels off if we just want to use this object for recording
        if(_shouldCapturePixels){
            [pixelBufferAttributes setValue:[NSNumber numberWithInt: kCVPixelFormatType_32BGRA]
                                     forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
            [pixelBufferAttributes setValue:[NSNumber numberWithBool:YES]
                                     forKey:(NSString*)kCVPixelBufferOpenGLCompatibilityKey];
        }
        if(width != -1 && height != -1){
            [pixelBufferAttributes setValue:[NSNumber numberWithInt:width]
                                     forKey:(NSString*)kCVPixelBufferWidthKey];
            [pixelBufferAttributes setValue:[NSNumber numberWithInt:height]
                                     forKey:(NSString*)kCVPixelBufferHeightKey];
        }
        
        if(pixelBufferAttributes.count > 0){
			[self setPixelBufferAttributes: pixelBufferAttributes ];
        }
		//init the session
		self.session = [[[QTCaptureSession alloc] init] autorelease];
        
		if(_shouldCapturePixels){
            pixels = &_pixels;
            if(width != -1 && height != -1){
                pixels->allocate(width, height, OF_IMAGE_COLOR);
            }
            
            NSError* error;
            bool success = [self.session addOutput:self error:&error];
            if( !success ){
				ofLogError("ofQTKitGrabber") << "failed to add delegate to capture session: " << [[error description] UTF8String];
                return nil;
            }
    	}
        
		videoDeviceID = -1;		
		[self setVideoDeviceID:_videoDeviceID];
		
		// if we're using audio add an audio device
		if (self.useAudio) {
			audioDeviceID = -1;
			[self setAudioDeviceID:_audioDeviceID];
		}
         
		[self startSession];
	}
	return self;
}


- (void) startSession
{
	//start the session
	ofLogVerbose("ofQTKitGrabber") << "starting video session";
	[session startRunning];
	
}

- (void) setVideoDeviceID:(NSInteger)_videoDeviceID
{	
	if(videoDeviceID != _videoDeviceID){
		
		//get video device
		NSArray* videoDevices = [[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo] 
								 arrayByAddingObjectsFromArray:[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeMuxed]];
		
		// Try to open the new device
		if(_videoDeviceID < 0 || _videoDeviceID >= videoDevices.count){
			ofLogError("ofQTKitGrabber") << "selected video device id " << _videoDeviceID << " out of range for number of devices: " << videoDevices.count;
			return;
		}		
		selectedVideoDevice = [videoDevices objectAtIndex:_videoDeviceID];
		if([self setSelectedVideoDevice:selectedVideoDevice]){
			videoDeviceID = _videoDeviceID;
		}
	}
}

- (void) setAudioDeviceID:(NSInteger)_audioDeviceID
{	
	if(audioDeviceID != _audioDeviceID){
		
		//get audio device
		NSArray* audioDevices = [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeSound];
		
		// Try to open the new device
		if(_audioDeviceID < 0 || _audioDeviceID >= audioDevices.count){
			ofLogError("ofQTKitGrabber") << "selected video device id " << _audioDeviceID << " is out of range for number of devices: " << audioDevices.count;
			return;
		}
        
		selectedAudioDevice = [audioDevices objectAtIndex:_audioDeviceID];
        if(selectedAudioDevice == nil){
            ofLogError("ofQTKitGrabber") << "audio device is NULL for id " << _audioDeviceID << endl;
        }
		if([self setSelectedAudioDevice:selectedAudioDevice]){
			audioDeviceID = _audioDeviceID;
		}
	}
}

- (BOOL) setSelectedVideoDevice:(QTCaptureDevice *)_selectedVideoDevice
{
	BOOL success = YES;	
	if (self.videoDeviceInput != nil) {
        ofLogVerbose("ofQTKitGrabber") << "removing existing video device " << [self.videoDeviceInput.description UTF8String];
		// Remove the old device input from the session and close the device
		[self.session removeInput:videoDeviceInput];
		[[self.videoDeviceInput device] close];
		self.videoDeviceInput = nil;
	}
	
	if (_selectedVideoDevice != nil) {
		NSError *error = nil;
		
		// Try to open the new device
		success = [_selectedVideoDevice open:&error];
		if(success){
			// Create a device input for the device and add it to the session
			self.videoDeviceInput = [QTCaptureDeviceInput deviceInputWithDevice:_selectedVideoDevice];
			
			success = [self.session addInput:self.videoDeviceInput error:&error];
			if(!success || error != nil){
				ofLogError("ofQTKitGrabber") << "failed to add the video device input: " << [[error localizedDescription] UTF8String];
			}
			else{
				ofLogVerbose("ofQTKitGrabber") << "attached video device: " << [_selectedVideoDevice.description UTF8String];
			}
		}
		else {
			ofLogError("ofQTKitGrabber") << "failed to open the video device input: " << [[error localizedDescription] UTF8String];
		}
	}
	
	return success;
}

- (BOOL) setSelectedAudioDevice:(QTCaptureDevice *)_selectedAudioDevice
{
	BOOL success = YES;	
	if (self.audioDeviceInput != nil) {
        ofLogVerbose("ofQTKitGrabber") << "removing existing audio device " << [self.audioDeviceInput.description UTF8String];
		// Remove the old device input from the session and close the device
		[self.session removeInput:audioDeviceInput];
		[[self.audioDeviceInput device] close];
		self.audioDeviceInput = nil;
	}
	
	if (_selectedAudioDevice != nil) {
		NSError *error = nil;
		
		// Try to open the new device
		success = [_selectedAudioDevice open:&error];
		if(success){
			// Create a device input for the device and add it to the session
			self.audioDeviceInput = [QTCaptureDeviceInput deviceInputWithDevice:_selectedAudioDevice];
			
			success = [self.session addInput:self.audioDeviceInput error:&error];
			if(!success && error != nil){
				ofLogError("ofQTKitGrabber") << "failed to add the audio device input: " << [[error localizedDescription] UTF8String];
			}
			else{
				ofLogVerbose("ofQTKitGrabber") << "attached audio device: " << [_selectedAudioDevice.description UTF8String];
			}
		}
		else {
			ofLogError("ofQTKitGrabber") << "failed to open the audio device: " << [[error localizedDescription] UTF8String];
		}
	}
	
	return success;
}

- (BOOL) initRecording:(NSString*)_selectedVideoCodec audioCodec:(NSString*)_selectedAudioCodec
{
	BOOL success = YES;	
	NSError *error = nil;
	
	// Create the movie file output and add it to the session
	self.captureMovieFileOutput = [[[QTCaptureMovieFileOutput alloc] init] autorelease];
    [self.captureMovieFileOutput setDelegate:self];
    
	success = [self.session addOutput:captureMovieFileOutput error:&error];
	if (!success) {
		ofLogError("ofQTKitGrabber") << "failed to initialize recording: " << [[error localizedDescription] UTF8String];
        isRecordReady = NO;
	} 
    else {
		[self setVideoCodec:_selectedVideoCodec];
		[self setAudioCodec:_selectedAudioCodec];
		ofLogVerbose("ofQTKitGrabber") << "video codec is " << [[_selectedVideoCodec description] UTF8String];
		ofLogVerbose("ofQTKitGrabber") << "audio codec is " << [[_selectedAudioCodec description] UTF8String];
		isRecordReady = YES;
	}
    return success;
}

+ (NSArray*) listVideoCodecs
{
	NSArray* videoCodecs = [QTCompressionOptions compressionOptionsIdentifiersForMediaType:QTMediaTypeVideo];
	
	ofLogVerbose("ofQTKitGrabber") << "listing available video codecs:";
	[self enumerateArray:videoCodecs];
	
	return videoCodecs;
}

+ (NSArray*) listAudioCodecs
{
	NSArray* audioCodecs = [QTCompressionOptions compressionOptionsIdentifiersForMediaType:QTMediaTypeSound];
	
	ofLogVerbose("ofQTKitGrabber") << "listing available audio codecs:";
	[self enumerateArray:audioCodecs];
	
	return audioCodecs;
}

- (void) setVideoCodec:(NSString*)_selectedVideoCodec
{
	// set codec on connection for type Video
	NSArray *outputConnections = [captureMovieFileOutput connections];
	QTCaptureConnection *connection;
	for (connection in outputConnections) {
		if ([[connection mediaType] isEqualToString:QTMediaTypeVideo]){
			[captureMovieFileOutput setCompressionOptions:[QTCompressionOptions compressionOptionsWithIdentifier:_selectedVideoCodec]
                                            forConnection:connection];
        }
	}
}

- (void) setAudioCodec:(NSString*)_selectedAudioCodec
{
	// set codec on connection for type Sound
	NSArray *outputConnections = [captureMovieFileOutput connections];
	QTCaptureConnection *connection;
	for (connection in outputConnections) {
		if ([[connection mediaType] isEqualToString:QTMediaTypeSound]){
			[self.captureMovieFileOutput setCompressionOptions:[QTCompressionOptions compressionOptionsWithIdentifier:_selectedAudioCodec]
                                            forConnection:connection];
        }
	}
}

- (void) startRecording:(NSString*)filePath
{
	if (isRecordReady) {
		
        // make sure last movie has stopped
		if (isRecording){
            [self stopRecording];
        }
		
		// set url for recording
		[self.captureMovieFileOutput recordToOutputFileURL:[NSURL fileURLWithPath:filePath]];
		
		ofLogVerbose("ofQTKitGrabber") << "started recording movie to: \"" << [filePath UTF8String] << "\"";
		
		isRecording = YES;
		
	}
    else {
		ofLogError("ofQTKitGrabber") << "not set up to record, call initRecording() first";
	}
}

- (void) stopRecording
{
	if (isRecordReady && isRecording){
		
		// set url to nil to stop recording
		[self.captureMovieFileOutput recordToOutputFileURL:nil];
		
		ofLogVerbose("ofQTKitGrabber") << "stopped recording movie";
		
		isRecording = NO;
		
	}
    else {
        ofLogError("ofQTKitGrabber") << "cannot stop recording, call initRecording() then record() first";
	}
}

- (void) captureOutput:(QTCaptureFileOutput *)captureOutput
didFinishRecordingToOutputFileAtURL:(NSURL *)outputFileURL
         			 forConnections:(NSArray *)connections
              		     dueToError:(NSError *)error
{
	
	ofVideoSavedEventArgs savedEvent;
	savedEvent.grabber = grabber;
	savedEvent.videoPath = [[outputFileURL path] UTF8String];
	if(error != nil){
		savedEvent.error = [[error localizedDescription] UTF8String];
	}
	ofNotifyEvent(grabber->videoSavedEvent, savedEvent, grabber);
}

//Frame from the camera
//this tends to be fired on a different thread, so keep the work really minimal and don't perform copies here
//That way if the frames are really big we will appropriately drop frames rather than lagging behind
- (void) outputVideoFrame:(CVImageBufferRef)videoFrame 
		 withSampleBuffer:(QTSampleBuffer *)sampleBuffer 
		   fromConnection:(QTCaptureConnection *)connection
{
	@autoreleasepool {
		@synchronized(self){
			if(cvFrame != NULL){
				CVBufferRelease(cvFrame);
			}
			cvFrame = videoFrame;
			CVBufferRetain(cvFrame);
			hasNewFrame = YES;
		}
	}
}

- (void) update
{
	@synchronized(self){
        if(hasNewFrame){
			if(pixels == NULL){
                ofLogError("ofQTKitGrabber") << "pixels not set on Obj-C Class from C++ wrapper";
                return;
            }
            
            //pixels won't be allocated if we didn't force the video width and height
            //allocate upon receving the first frame
            if(!pixels->isAllocated()){
                pixels->allocate(CVPixelBufferGetWidth(cvFrame),
                                 CVPixelBufferGetHeight(cvFrame), OF_IMAGE_COLOR);
                width  = CVPixelBufferGetWidth(cvFrame);
                height = CVPixelBufferGetHeight(cvFrame);
            }

            CVPixelBufferLockBaseAddress(cvFrame, kCVPixelBufferLock_ReadOnly);
#if( MAC_OS_X_VERSION_10_8 && MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_8)
            //fast conversion of BGRA -> RGB introduced in 10.8
            vImage_Buffer src;
            src.height = CVPixelBufferGetHeight(cvFrame);
            src.width = CVPixelBufferGetWidth(cvFrame);
            src.data = CVPixelBufferGetBaseAddress(cvFrame);
            src.rowBytes = CVPixelBufferGetBytesPerRow(cvFrame);
            
            vImage_Buffer dest;
            dest.height = height;
            dest.width = width;
            dest.rowBytes = width*3;
            dest.data = pixels->getPixels();

            vImage_Error err = vImageConvert_BGRA8888toRGB888(&src, &dest, kvImageNoFlags);
            if(err != kvImageNoError){
                NSLog(@"[error] ofxQTKitGrabber: update(): pixel copy vImage_error %ld", err);
            }
#else
            //manually convert BGRA -> RGB
            int dstBpp = 3;
            int srcBpp = 4;
            int dstRowBytes = width*dstBpp;
            int srcRowBytes = CVPixelBufferGetBytesPerRow(cvFrame);
            for(int y = 0; y < height; y++){
                unsigned char* src = ((unsigned char*)CVPixelBufferGetBaseAddress(cvFrame)) + y*srcRowBytes;
                unsigned char* dst = pixels->getPixels() + y*dstRowBytes;
                for(int x = 0; x < width; x++){
                    dst[0] = src[2];
                    dst[1] = src[1];
                    dst[2] = src[0];
                    dst+=dstBpp;
                    src+=srcBpp;
                }                    
            }
#endif
            CVPixelBufferUnlockBaseAddress(cvFrame, kCVPixelBufferLock_ReadOnly);

            hasNewFrame = NO;
            isFrameNew = YES;
        }
        else{
            isFrameNew = NO;
        }
    }
}

- (void) stop
{
    if (isRecording){
		[self stopRecording];   
    }
    if(self.isRunning){
	    [self.session stopRunning];
    }
    if ([[self.videoDeviceInput device] isOpen]) [[videoDeviceInput device] close];
    if ([[self.audioDeviceInput device] isOpen]) [[audioDeviceInput device] close];
    
    //setting NIL releases memory because these properties are synthesized as retain
	self.videoDeviceInput = nil;
    self.audioDeviceInput = nil;
    self.captureMovieFileOutput = nil;
	self.session = nil;
}

- (BOOL) isRunning
{
	return self.session && self.session.isRunning;
}

- (void) dealloc
{
	if(self.isRunning){
		[self stop];
	}
	
	if(cvFrame != NULL){
		CVPixelBufferRelease(cvFrame);
		cvFrame = NULL;
	}

	[super dealloc];
}

@end

//C++ Wrapper class:
ofQTKitGrabber::ofQTKitGrabber(){
	videoDeviceID = 0;
	audioDeviceID = 0;
    videoCodecIDString = "QTCompressionOptionsJPEGVideo";					// setting default video codec
	audioCodecIDString = "QTCompressionOptionsHighQualityAACAudio";			// setting audio video codec
	grabber = NULL;
	isInited = false;
    bUseAudio = false;
    bPreview = true;
}

ofQTKitGrabber::~ofQTKitGrabber(){
	if(isInited){
		close();
	}
}

void ofQTKitGrabber::setDeviceID(int _videoDeviceID){
    setVideoDeviceID(_videoDeviceID);
}

void ofQTKitGrabber::setVideoDeviceID(int _videoDeviceID){
	videoDeviceID = _videoDeviceID;
	if(isInited){
		@autoreleasepool {
			[grabber setVideoDeviceID:videoDeviceID];
		}
	}
}

void ofQTKitGrabber::setAudioDeviceID(int _audioDeviceID){
	audioDeviceID = _audioDeviceID;
	if(isInited){
		@autoreleasepool {
			[grabber setAudioDeviceID:audioDeviceID];
		}
	}
}

void ofQTKitGrabber::setDeviceID(string _videoDeviceIDString){
    setVideoDeviceID(_videoDeviceIDString);
}

void ofQTKitGrabber::setVideoDeviceID(string _videoDeviceIDString){
	@autoreleasepool {	
		// set array filled with devices
		NSArray* deviceArray = [[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo] 
								arrayByAddingObjectsFromArray:[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeMuxed]];

		// convert device string to NSString
		NSString* deviceIDString = [NSString stringWithUTF8String: _videoDeviceIDString.c_str()];

		// find the index of the device name in the array of devices
		videoDeviceID = (NSInteger)[QTKitVideoGrabber getIndexofStringInArray:deviceArray
																 stringToFind:deviceIDString];

		if(isInited){
			[grabber setVideoDeviceID:videoDeviceID];
		}
	}
}

void ofQTKitGrabber::setAudioDeviceID(string _audioDeviceIDString){
	@autoreleasepool {
		// set array filled with devices
		NSArray* deviceArray = [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeSound];

		// convert device string to NSString
		NSString* deviceIDString = [NSString stringWithUTF8String: _audioDeviceIDString.c_str()];

		// find the index of the device name in the array of devices
		audioDeviceID = (NSInteger)[QTKitVideoGrabber getIndexofStringInArray:deviceArray
																 stringToFind:deviceIDString];

		if(isInited){
			[grabber setAudioDeviceID:audioDeviceID];
		}
	}
}

bool ofQTKitGrabber::initGrabberWithoutPreview(){
    bPreview = false;
    return setup(-1,-1); //supplying -1 allows CoreVideo to choose the dimensions
}

bool ofQTKitGrabber::setup(int w, int h){

	if(grabber != NULL){
		close();
	}

	@autoreleasepool {
		grabber = [[QTKitVideoGrabber alloc] initWithWidth:w //wxh can be -1 to specify do not force
													height:h
											   videodevice:videoDeviceID
											   audiodevice:audioDeviceID
												usingAudio:bUseAudio
											 capturePixels:bPreview
												 pixelsRef:pixels];

		isInited = (grabber != nil);
		if(isInited){
			grabber.grabber = this; //for events
		}
	}
	return isInited;
}

bool ofQTKitGrabber::initRecording(){
	bool success = false;
	if(confirmInit()){
		@autoreleasepool {
			NSString * videoCodec = [NSString stringWithUTF8String: videoCodecIDString.c_str()];
			NSString * audioCodec = [NSString stringWithUTF8String: audioCodecIDString.c_str()];

			success = [grabber initRecording:videoCodec audioCodec:audioCodec];
		}
	}
	return success;
}

const vector<string>& ofQTKitGrabber::listVideoCodecs() const{
	@autoreleasepool {
		NSArray* videoCodecs = [QTKitVideoGrabber listVideoCodecs];
		videoCodecsVec.clear();
		for (id object in videoCodecs){
			string str = [[object description] UTF8String];
			videoCodecsVec.push_back(str);
		}
	}
	return videoCodecsVec;
}

const vector<string>& ofQTKitGrabber::listAudioCodecs() const{
	@autoreleasepool {
		NSArray* audioCodecs = [QTKitVideoGrabber listAudioCodecs];
		audioCodecsVec.clear();
		for (id object in audioCodecs){
			string str = [[object description] UTF8String];
			audioCodecsVec.push_back(str);
		}
	}
	return audioCodecsVec;
}

void ofQTKitGrabber::setVideoCodec(string _videoCodec){
	videoCodecIDString = _videoCodec;
	if(confirmInit()){
		@autoreleasepool {
			NSString * NSvideoCodec = [NSString stringWithUTF8String: videoCodecIDString.c_str()];
			[grabber setVideoCodec:NSvideoCodec];
		}
	}
}

void ofQTKitGrabber::setAudioCodec(string _audioCodec){
	if(confirmInit()){
		audioCodecIDString = _audioCodec;
		@autoreleasepool {
			NSString * NSaudioCodec = [NSString stringWithUTF8String: audioCodecIDString.c_str()];
			[grabber setAudioCodec:NSaudioCodec];
		}
	}
}

void ofQTKitGrabber::startRecording(string filePath){
	if(confirmInit()){
		@autoreleasepool {
			NSString * NSfilePath = [NSString stringWithUTF8String: ofToDataPath(filePath).c_str()];
			[grabber startRecording:NSfilePath];
		}
	}
}

void ofQTKitGrabber::stopRecording(){
	if(confirmInit()){
		@autoreleasepool {
			[grabber stopRecording];
		}
	}
}

void ofQTKitGrabber::update(){
	if(confirmInit()){
		@autoreleasepool {
			[grabber update];
		}
	} 
}

bool ofQTKitGrabber::isFrameNew() const {
	return isInited && [grabber isFrameNew];
}

bool ofQTKitGrabber::isReady() const {
	return isInited;
}

bool ofQTKitGrabber::isInitialized() const{
    return isInited;
}

bool ofQTKitGrabber::isRecordingReady() const {
	return isInited && [grabber isRecordReady];
}

bool ofQTKitGrabber::isRecording() const {
	return isReady() && [grabber isRecording];
}

bool ofQTKitGrabber::hasPreview() const {
    return bPreview;
}

vector <ofVideoDevice> ofQTKitGrabber::listDevices() const {
    vector <string> devList = listVideoDevices();
    
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

//---------------------------------------------------------------------------
bool ofQTKitGrabber::setPixelFormat(ofPixelFormat pixelFormat){
	//note as we only support RGB we are just confirming that this pixel format is supported
	if( pixelFormat == OF_PIXELS_RGB ){
		return true;
	}
	ofLogWarning("ofQTKitGrabber") << "setPixelFormat(): requested pixel format " << pixelFormat << " not supported";
	return false;
}

//---------------------------------------------------------------------------
ofPixelFormat ofQTKitGrabber::getPixelFormat() const{
	//note if you support more than one pixel format you will need to return a ofPixelFormat variable. 
	return OF_PIXELS_RGB;
}

//---------------------------------------------------------------------------
const vector<string>& ofQTKitGrabber::listVideoDevices() const{
	@autoreleasepool {
		NSArray* videoDevices = [QTKitVideoGrabber listVideoDevices];
		videoDeviceVec.clear();
		for (id object in videoDevices){
			string str = [[object description] UTF8String];
			videoDeviceVec.push_back(str);
		}
	}
	return videoDeviceVec;
}

const vector<string>& ofQTKitGrabber::listAudioDevices() const{
	@autoreleasepool {
		NSArray* audioDevices = [QTKitVideoGrabber listAudioDevices];
		audioDeviceVec.clear();
		for (id object in audioDevices){
			string str = [[object description] UTF8String];
			audioDeviceVec.push_back(str);
		}
	}
	return audioDeviceVec;
}

void ofQTKitGrabber::close(){
    if(grabber != NULL){
		@autoreleasepool {
			[grabber stop];
			[grabber release];
			grabber = NULL;
			isInited = false;
		}
	}
}

ofPixels& ofQTKitGrabber::getPixels(){
	if(!confirmInit() || !pixels.isAllocated()){
	    ofLogError("ofQTKitGrabber") << "getPixels(): asking for pixels on unitialized grabber";
	}
	return pixels;
}

const ofPixels& ofQTKitGrabber::getPixels() const {
	if(!confirmInit() || !pixels.isAllocated()){
	    ofLogError("ofQTKitGrabber") << "getPixels(): asking for pixels on unitialized grabber";
	}
	return pixels;
}

void ofQTKitGrabber::setUseAudio(bool _bUseAudio){
	if(_bUseAudio != bUseAudio){
		if(isInited){
			ofLogError("ofQTKitGrabber") << "setUseAudio(): requesting to use audio after grabber is already initialized, try calling setUseAudio() first";
		}
		bUseAudio = _bUseAudio;
	}
}

void ofQTKitGrabber::setVerbose(bool bTalkToMe){
	//Now handled by ofLogVerbose()
}

void ofQTKitGrabber::videoSettings(){
	ofSystemAlertDialog("ofQTKitGrabber: Video Settings is not supported in 10.7+. Please compile against the 10.6 SDK for this feature.");
}

int ofQTKitGrabber::getDeviceID() const {
    return getVideoDeviceID();
}

int ofQTKitGrabber::getVideoDeviceID() const {
    if(confirmInit()){
		return grabber.videoDeviceID;
	}
	return -1;
}

int ofQTKitGrabber::getAudioDeviceID() const {
	if(confirmInit()){
		return grabber.audioDeviceID;
	}
	return -1;
}

float ofQTKitGrabber::getHeight() const {
	if(confirmInit()){
		return float(grabber.height);
	}
	return 0;
}

float ofQTKitGrabber::getWidth() const {
	if(confirmInit()){
		return float(grabber.width);
	}
	return 0;
}
		  
bool ofQTKitGrabber::confirmInit() const {
	if(!isInited){
		ofLogError("ofQTKitGrabber") << "confirmInit(): calling method on unintialized video grabber";
	}
	return isInited;
}

void ofQTKitGrabber::setDesiredFrameRate(int framerate){
	ofLogWarning("ofQTKitGrabber") << "setDesiredFrameRate(): cannot set framerate for QTKitGrabber";
}

#pragma clang diagnostic pop
