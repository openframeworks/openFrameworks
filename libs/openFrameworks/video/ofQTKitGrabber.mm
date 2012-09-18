
#include "ofQTKitGrabber.h"
#import <QTKit/QTKit.h>
#import <QuickTime/QuickTime.h>


//This Objective-C class contains all the OS specific
//Stuff we need for pulling images from a video camera feed
//the QTCaptureVideoPreviewOutput is a non-blocking frame receiver delegate
//for a QTCaptureSession
//All you wanted to know and more about QTKit Capture can be found here:
// http://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/QTKitApplicationProgrammingGuide/UsingQTKit/UsingQTKit.html

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
		NSLog(@"%d - %@", count, [object description]);
		count++;
	}
	NSLog(@"\n");
}

+ (NSInteger) getIndexofStringInArray:(NSArray*)someArray stringToFind:(NSString*)someStringDescription
{
	NSInteger count = 0;
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
	NSArray* videoDevices = [[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo] 
							 arrayByAddingObjectsFromArray:[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeMuxed]];
	
	[self enumerateArray:videoDevices];
	
	return videoDevices;
	
}

+ (NSArray*) listAudioDevices
{
	NSArray* audioDevices = [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeSound];
	
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
            [pixelBufferAttributes setValue:[NSNumber numberWithInt: kCVPixelFormatType_24RGB]
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
				ofLogError("ofQTKitGrabber") << "Failed to add delegate to capture session: error was " << [[error description] UTF8String];
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
	ofLogVerbose("QTKitVideoGrabber") << "starting video session";
	[session startRunning];
	
}

- (void) setVideoDeviceID:(NSInteger)_videoDeviceID
{	
	if(videoDeviceID != _videoDeviceID){
		
		//get video device
		NSArray* videoDevices = [[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo] 
								 arrayByAddingObjectsFromArray:[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeMuxed]];
		
        ofLogVerbose("QTKitVideoGrabber") << "Video Device List: " << [[videoDevices description] UTF8String];
        
		// Try to open the new device
		if(_videoDeviceID < 0 || _videoDeviceID >= videoDevices.count){
			ofLogError("ofQTKitGrabber") << " Selected video device ID (" << _videoDeviceID << ") out of range (" << videoDevices.count << ")";
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
		
		ofLogVerbose("ofQTKitGrabber") << "Audio Device List: " <<  [[audioDevices description] UTF8String];
		// Try to open the new device
		if(_audioDeviceID < 0 || _audioDeviceID >= audioDevices.count){
			ofLogError("ofQTKitGrabber") << " Selected video device ID (" << _audioDeviceID << ") out of range (" << audioDevices.count << ")";
			return;
		}
        
		selectedAudioDevice = [audioDevices objectAtIndex:_audioDeviceID];
        if(selectedAudioDevice == nil){
            ofLogError("ofQTKitGrabber") << "Audio Device is nil for id " << _audioDeviceID << endl;
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
        ofLogVerbose("ofQTKitGrabber") << " Removing existing video device " << [self.videoDeviceInput.description UTF8String];
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
			self.videoDeviceInput = [[QTCaptureDeviceInput alloc] initWithDevice:_selectedVideoDevice];
			
			success = [self.session addInput:self.videoDeviceInput error:&error];
			if(!success || error != nil){
				ofLogError("ofQTKitGrabber") << "Failed to add the video device input. Error: " << [[error localizedDescription] UTF8String];
			}
			else{
				ofLogVerbose("ofQTKitGrabber") << "Attached video device" << [_selectedVideoDevice.description UTF8String];
			}
		}
		else {
			ofLogError("ofQTKitGrabber") << "Failed to open the video device input. Error: " << [[error localizedDescription] UTF8String];
		}
	}
	
	return success;
}

- (BOOL) setSelectedAudioDevice:(QTCaptureDevice *)_selectedAudioDevice
{
	BOOL success = YES;	
	if (self.audioDeviceInput != nil) {
        ofLogVerbose("ofQTKitGrabber") << " Removing existing audio device " << [self.audioDeviceInput.description UTF8String];
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
			self.audioDeviceInput = [[QTCaptureDeviceInput alloc] initWithDevice:_selectedAudioDevice];
			
			success = [self.session addInput:self.audioDeviceInput error:&error];
			if(!success && error != nil){
				ofLogError("ofQTKitGrabber") << "Failed to add the audtio device input. Error: " << [[error localizedDescription] UTF8String];
			}
			else{
				ofLogVerbose("ofQTKitGrabber") << "Attached audio device: " << [_selectedAudioDevice.description UTF8String];
			}
		}
		else {
			ofLogError("ofQTKitGrabber") << "Failed to open the audio device. Error: " << [[error localizedDescription] UTF8String];
		}
	}
	
	return success;
}

- (BOOL) initRecording:(NSString*)_selectedVideoCodec audioCodec:(NSString*)_selectedAudioCodec
{
	BOOL success = YES;	
	NSError *error = nil;
	
	// Create the movie file output and add it to the session
	self.captureMovieFileOutput = [[QTCaptureMovieFileOutput alloc] init];
    [self.captureMovieFileOutput setDelegate:self];
    
	success = [self.session addOutput:captureMovieFileOutput error:&error];
	if (!success) {
		ofLogError("ofQTKitGrabber") << " Failed to initialize recording " << [[error localizedDescription] UTF8String];
        isRecordReady = NO;
	} 
    else {
		[self setVideoCodec:_selectedVideoCodec];
		[self setAudioCodec:_selectedAudioCodec];
		NSLog(@"audio codec is %@", _selectedAudioCodec);
		isRecordReady = YES;
	}
    return success;
}

+ (NSArray*) listVideoCodecs
{
	NSArray* videoCodecs = [QTCompressionOptions compressionOptionsIdentifiersForMediaType:QTMediaTypeVideo];
	
	ofLogVerbose("ofQTKitGrabber") << "listing video compression options";
	[self enumerateArray:videoCodecs];
	
	return videoCodecs;
}

+ (NSArray*) listAudioCodecs
{
	NSArray* audioCodecs = [QTCompressionOptions compressionOptionsIdentifiersForMediaType:QTMediaTypeSound];
	
	ofLogVerbose("ofQTKitGrabber") << "listing audio compression options";
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
		
		BOOL success = YES;
		
        // make sure last movie has stopped
		if (isRecording){
            [self stopRecording];
        }
		
		// set url for recording
		[self.captureMovieFileOutput recordToOutputFileURL:[NSURL fileURLWithPath:filePath]];
		
		ofLogVerbose("ofQTKitGrabber") << "Started recording movie to: " << [filePath UTF8String];
		
		isRecording = YES;
		
	}
    else {
		ofLogError("ofQTKitGrabber") << " Not set up to record - call initRecording first";
	}
}

- (void) stopRecording
{
	if (isRecordReady && isRecording){
		
		// set url to nil to stop recording
		[self.captureMovieFileOutput recordToOutputFileURL:nil];
		
		ofLogVerbose("ofQTKitGrabber") << "Stopped recording movie";
		
		isRecording = NO;
		
	}
    else {
        ofLogError("ofQTKitGrabber") << " Cannot stop recording - call initRecording then record first";
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
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    @synchronized(self){
        if(cvFrame != NULL){
			CVBufferRelease(cvFrame);
		}
		cvFrame = videoFrame;
		CVBufferRetain(cvFrame);
		hasNewFrame = YES;
	}
    
    [pool release];
}

- (void) update
{
	@synchronized(self){
        if(hasNewFrame){
			if(pixels == NULL){
                ofLogError("ofQTKitGrabber") << "Pixels not set on Obj-C Class from C++ wrapper";
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
            
            size_t dstBytesPerRow = pixels->getWidth() * 3;
            CVPixelBufferLockBaseAddress(cvFrame, kCVPixelBufferLock_ReadOnly);
            if (CVPixelBufferGetBytesPerRow(cvFrame) == dstBytesPerRow) {
                memcpy(pixels->getPixels(), CVPixelBufferGetBaseAddress(cvFrame), pixels->getHeight() * dstBytesPerRow);
            }
            else {
                unsigned char *dst = pixels->getPixels();
                unsigned char *src = (unsigned char*)CVPixelBufferGetBaseAddress(cvFrame);
                size_t srcBytesPerRow = CVPixelBufferGetBytesPerRow(cvFrame);
                size_t copyBytesPerRow = MIN(dstBytesPerRow, srcBytesPerRow); // should always be dstBytesPerRow but be safe
                for (int y = 0; y < pixels->getHeight(); y++){
                    memcpy(dst, src, copyBytesPerRow);
                    dst += dstBytesPerRow;
                    src += srcBytesPerRow;
                }
            }
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
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		[grabber setVideoDeviceID:videoDeviceID];
		[pool release];	
	}
}

void ofQTKitGrabber::setAudioDeviceID(int _audioDeviceID){
	audioDeviceID = _audioDeviceID;
	if(isInited){
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		[grabber setAudioDeviceID:audioDeviceID];
		[pool release];	
	}		
}

void ofQTKitGrabber::setDeviceID(string _videoDeviceIDString){
    setVideoDeviceID(_videoDeviceIDString);
}

void ofQTKitGrabber::setVideoDeviceID(string _videoDeviceIDString){
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
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
	[pool release];	
}

void ofQTKitGrabber::setAudioDeviceID(string _audioDeviceIDString){
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
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
	[pool release];	
}

bool ofQTKitGrabber::initGrabberWithoutPreview(){
    bPreview = false;
    return initGrabber(-1,-1); //supplying -1 allows CoreVideo to choose the dimensions
}

bool ofQTKitGrabber::initGrabber(int w, int h){
    
    if(grabber != NULL){
        close();
    }
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
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
	[pool release];
    return isInited;
}

bool ofQTKitGrabber::initRecording(){
    bool success = false;
	if(confirmInit()){
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		NSString * videoCodec = [NSString stringWithUTF8String: videoCodecIDString.c_str()];
		NSString * audioCodec = [NSString stringWithUTF8String: audioCodecIDString.c_str()];
        
		success = [grabber initRecording:videoCodec audioCodec:audioCodec];

		[pool release];
	}
    return success;
}

vector<string>& ofQTKitGrabber::listVideoCodecs(){
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSArray* videoCodecs = [QTKitVideoGrabber listVideoCodecs];
	videoCodecsVec.clear();
	for (id object in videoCodecs){
		string str = [[object description] UTF8String];
		videoCodecsVec.push_back(str);
	}
	[pool release];	
	return videoCodecsVec;
}

vector<string>& ofQTKitGrabber::listAudioCodecs(){
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSArray* audioCodecs = [QTKitVideoGrabber listAudioCodecs];
	audioCodecsVec.clear();
	for (id object in audioCodecs){
		string str = [[object description] UTF8String];
		audioCodecsVec.push_back(str);
	}
	[pool release];	
	return audioCodecsVec;
}

void ofQTKitGrabber::setVideoCodec(string _videoCodec){
	videoCodecIDString = _videoCodec;	
	if(confirmInit()){
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];	
		NSString * NSvideoCodec = [NSString stringWithUTF8String: videoCodecIDString.c_str()];
		[grabber setVideoCodec:NSvideoCodec];
		[pool release];
	}
}

void ofQTKitGrabber::setAudioCodec(string _audioCodec){
    if(confirmInit()){
        audioCodecIDString = _audioCodec;
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];	
        NSString * NSaudioCodec = [NSString stringWithUTF8String: audioCodecIDString.c_str()];
        [grabber setAudioCodec:NSaudioCodec];
        [pool release];
    }
}

void ofQTKitGrabber::startRecording(string filePath){
    if(confirmInit()){
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		NSString * NSfilePath = [NSString stringWithUTF8String: ofToDataPath(filePath).c_str()];
		[grabber startRecording:NSfilePath];
		[pool release];
    }
}

void ofQTKitGrabber::stopRecording(){
    if(confirmInit()){
	    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        [grabber stopRecording];
        [pool release];
    }
}

void ofQTKitGrabber::update(){
    if(confirmInit()){
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		[grabber update];
        [pool release];
	} 
}

bool ofQTKitGrabber::isFrameNew(){
	return isInited && [grabber isFrameNew];
}

bool ofQTKitGrabber::isReady(){
	return isInited;
}

bool ofQTKitGrabber::isRecordingReady(){
	return isInited && [grabber isRecordReady];
}

bool ofQTKitGrabber::isRecording(){
	return isReady() && [grabber isRecording];
}

bool ofQTKitGrabber::hasPreview(){
    return bPreview;
}

// would be better if listDevices returned a vector of devices too, 
// but that requires updating the base class...perhaps we could
// then have a ofBaseDevice class to be used for enumerating any 
// type of device for video, sound, serial devices etc etc???
void ofQTKitGrabber::listDevices(){
    listVideoDevices();
}

//---------------------------------------------------------------------------
bool ofQTKitGrabber::setPixelFormat(ofPixelFormat pixelFormat){
	//note as we only support RGB we are just confirming that this pixel format is supported
	if( pixelFormat == OF_PIXELS_RGB ){
		return true;
	}
	ofLogWarning("ofQTKitGrabber") << "requested pixel format not supported." << endl;
	return false;
}

//---------------------------------------------------------------------------
ofPixelFormat ofQTKitGrabber::getPixelFormat(){
	//note if you support more than one pixel format you will need to return a ofPixelFormat variable. 
	return OF_PIXELS_RGB;
}

//---------------------------------------------------------------------------
vector<string>& ofQTKitGrabber::listVideoDevices(){
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
	NSArray* videoDevices = [QTKitVideoGrabber listVideoDevices];
	videoDeviceVec.clear();
	for (id object in videoDevices){
		string str = [[object description] UTF8String];
		videoDeviceVec.push_back(str);
	}
	[pool release];
    
	return videoDeviceVec;
}

vector<string>& ofQTKitGrabber::listAudioDevices(){
    
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
	NSArray* audioDevices = [QTKitVideoGrabber listAudioDevices];
	audioDeviceVec.clear();
	for (id object in audioDevices){
		string str = [[object description] UTF8String];
		audioDeviceVec.push_back(str);
	}
	
    [pool release];
    
	return audioDeviceVec;
}

void ofQTKitGrabber::close(){

    if(grabber != NULL){
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        
        [grabber stop];
        [grabber release];
        grabber = NULL;
        isInited = false;
        
        [pool release];
    }
}

//will return empty if pixels haven't been allocated
unsigned char* ofQTKitGrabber::getPixels(){
	return getPixelsRef().getPixels();
}

ofPixelsRef ofQTKitGrabber::getPixelsRef(){
	if(!confirmInit() || !pixels.isAllocated()){
	    ofLogError("ofQTKitGrabber") << "Error asking for pixels on unitialized grabber.";
	}
	return pixels;
}

void ofQTKitGrabber::setUseAudio(bool _bUseAudio){
	if(_bUseAudio != bUseAudio){
		if(isInited){
			ofLogError("ofQTKitGrabber") << "Requesting to use audio after grabber is already initialized. Try doing this first!";
		}
		bUseAudio = _bUseAudio;
	}
}

void ofQTKitGrabber::setVerbose(bool bTalkToMe){
	//Now handled by ofLogVerbose()
}

void ofQTKitGrabber::videoSettings(){
	ofSystemAlertDialog("Video Settings is not supported in 10.7+. Please compile against the 10.6 SDK for this feature.");
}

int ofQTKitGrabber::getDeviceID(){
    return getVideoDeviceID();
}

int ofQTKitGrabber::getVideoDeviceID(){
    if(confirmInit()){
		return grabber.videoDeviceID;
	}
	return -1;
}

int ofQTKitGrabber::getAudioDeviceID(){
	if(confirmInit()){
		return grabber.audioDeviceID;
	}
	return -1;
}

float ofQTKitGrabber::getHeight(){
	if(confirmInit()){
		return float(grabber.height);
	}
	return 0;
}

float ofQTKitGrabber::getWidth(){
	if(confirmInit()){
		return float(grabber.width);
	}
	return 0;
}
		  
bool ofQTKitGrabber::confirmInit(){
	if(!isInited){
		ofLogError("ofQTKitGrabber") << "Calling method on unintialized video grabber.";
	}
	return isInited;
}
