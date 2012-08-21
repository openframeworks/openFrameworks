
#include "ofQTKitGrabber.h"
#import <QTKit/QTKit.h>
#import <QuickTime/QuickTime.h>

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
    ofPixels pixels;	

    BOOL hasNewFrame;
    BOOL isRunning;
    BOOL isFrameNew;
    BOOL isRecording;
    BOOL isRecordReady;
    BOOL verbose;
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
@property(readonly) ofPixelsRef pixels;
@property(readonly) BOOL isFrameNew;
@property(readonly) BOOL isRecording;
@property(readonly) BOOL isRecordReady;
@property(nonatomic, readwrite) BOOL verbose;
@property(nonatomic, readwrite) BOOL useAudio;

+ (NSArray*) listDevices;
+ (NSArray*) listAudioDevices;

- (id) initWithWidth:(NSInteger)width 
			  height:(NSInteger)height 
		 videodevice:(NSInteger)videoDeviceID
		 audiodevice:(NSInteger)audioDeviceID
		  usingAudio:(BOOL)_useAudio;

- (id) initWithoutPreview:(NSInteger)_videoDeviceID 
			  audiodevice:(NSInteger)_audioDeviceID 
			   usingAudio:(BOOL)_useAudio;

- (void) outputVideoFrame:(CVImageBufferRef)videoFrame 
		 withSampleBuffer:(QTSampleBuffer *)sampleBuffer 
		   fromConnection:(QTCaptureConnection *)connection;

- (bool) setSelectedVideoDevice:(QTCaptureDevice *)selectedVideoDevice;
- (bool) setSelectedAudioDevice:(QTCaptureDevice *)selectedAudioDevice;

- (void) setVideoDeviceID:(NSInteger)_videoDeviceID;
- (void) setAudioDeviceID:(NSInteger)_audioDeviceID;

- (void) initRecording:(NSString*)_selectedVideoCodec audioCodec:(NSString*)_selectedAudioCodec;
- (void) setVideoCodec:(NSString*)_selectedVideoCodec;
- (void) setAudioCodec:(NSString*)_selectedAudioCodec;
- (void) startRecording:(NSString*)filePath;
- (void) stopRecording;

+ (NSArray*) listVideoCodecs;
+ (NSArray*) listAudioCodecs;

+ (void) enumerateArray:(NSArray*)someArray;
+ (int)	 getIndexofStringInArray:(NSArray*)someArray stringToFind:(NSString*)someStringDescription;

- (void) startSession;
- (void) update;
- (void) stop;

@end

@implementation QTKitVideoGrabber
@synthesize width, height;
@synthesize session;
@synthesize videoDeviceID;
@synthesize audioDeviceID;
@synthesize videoDeviceInput;
@synthesize audioDeviceInput;
@synthesize captureMovieFileOutput;
@synthesize pixels;
@synthesize isFrameNew;
@synthesize isRecording;
@synthesize isRecordReady;
@synthesize verbose;
@synthesize useAudio;

+ (void) enumerateArray:(NSArray*)someArray
{
	NSInteger count = 0;
	for (id object in someArray) 
	{
		NSLog(@"%d - %@", count, [object description]);
		count++;
	}
	NSLog(@"\n");
}

+ (int) getIndexofStringInArray:(NSArray*)someArray stringToFind:(NSString*)someStringDescription
{
	NSInteger count = 0;
	NSInteger index = -1;
	
	for (id object in someArray) 
	{
		if ([[object description] isEqualToString:someStringDescription]) {
			index = count;
			break;
		} else count++;
	}
	
	return index;
}

+ (NSArray*) listDevices
{
	NSArray* videoDevices = [[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo] 
							 arrayByAddingObjectsFromArray:[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeMuxed]];
	
	NSLog(@"ofQTKitGrabber listing video devices");
	[self enumerateArray:videoDevices];
	
	return videoDevices;
	
}

+ (NSArray*) listAudioDevices
{
	NSArray* audioDevices = [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeSound];
	
	NSLog(@"ofQTKitGrabber listing audio devices");
	[self enumerateArray:audioDevices];
	
	return audioDevices;
}

- (id) initWithWidth:(NSInteger)_width height:(NSInteger)_height videodevice:(NSInteger)_videoDeviceID audiodevice:(NSInteger)_audioDeviceID usingAudio:(BOOL)_useAudio
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
		
		[self setPixelBufferAttributes: [NSDictionary dictionaryWithObjectsAndKeys:
										 [NSNumber numberWithInt: kCVPixelFormatType_24RGB], kCVPixelBufferPixelFormatTypeKey,
										 [NSNumber numberWithInt:width], kCVPixelBufferWidthKey, 
										 [NSNumber numberWithInt:height], kCVPixelBufferHeightKey, 
										 [NSNumber numberWithBool:YES], kCVPixelBufferOpenGLCompatibilityKey,
										 nil]];	
        
        pixels.allocate(_width, _height, OF_IMAGE_COLOR);
		
		//init the session
		self.session = [[[QTCaptureSession alloc] init] autorelease];
		
		NSError* error;
		bool success = [self.session addOutput:self error:&error];
		if( !success ){
			ofLog(OF_LOG_ERROR, "ofQTKitGrabber - ERROR - Error creating capture session");
			return nil;
		}
		
		videoDeviceID = -1;		
		[self setVideoDeviceID:_videoDeviceID];
		
		// if we're using audio add an audio device
		if (self.useAudio) {
			audioDeviceID = -1;
			[self setAudioDeviceID:_audioDeviceID];
		}
        
		// give us some info about the 'native format' of our device/s
		NSEnumerator *videoConnectionEnumerator = [[videoDeviceInput connections] objectEnumerator];
		QTCaptureConnection *videoConnection;
		
		while ((videoConnection = [videoConnectionEnumerator nextObject])) {
			if(verbose) ofLogVerbose("Video Input Format: %s\n", [[[videoConnection formatDescription] localizedFormatSummary] cStringUsingEncoding:NSUTF8StringEncoding]);
		}
        
		NSEnumerator *audioConnectionEnumerator = [[audioDeviceInput connections] objectEnumerator];
		QTCaptureConnection *audioConnection;
		while ((audioConnection = [audioConnectionEnumerator nextObject])) {
			if(verbose) ofLogVerbose("Audio Input Format: %s\n", [[[audioConnection formatDescription] localizedFormatSummary] cStringUsingEncoding:NSUTF8StringEncoding]);
		}   

		[self startSession];
	}
	return self;
}

- (id) initWithoutPreview:(NSInteger)_videoDeviceID audiodevice:(NSInteger)_audioDeviceID usingAudio:(BOOL)_useAudio
{
	if((self = [super init])){
		//configure self
		width = 0;
		height = 0;
		
		//instance variables
		cvFrame = NULL;
		hasNewFrame = NO;
		self.useAudio = _useAudio;
		isRecordReady = NO;
		isRecording = NO;
		
		//init the session
		self.session = [[[QTCaptureSession alloc] init] autorelease];
		
		NSError* error;
		bool success = [self.session addOutput:self error:&error];
		if( !success ){
			ofLog(OF_LOG_ERROR, "ofQTKitGrabber - ERROR - Error creating capture session");
			return nil;
		}
		
		videoDeviceID = -1;		
		[self setVideoDeviceID:_videoDeviceID];
		
		// if we're using audio add an audio device									
		if (self.useAudio) {
			audioDeviceID = -1;
			[self setAudioDeviceID:_audioDeviceID];
		}
		
		// give us some info about the 'native format' of our device/s
		NSEnumerator *videoConnectionEnumerator = [[videoDeviceInput connections] objectEnumerator];
		QTCaptureConnection *videoConnection;
		
		while ((videoConnection = [videoConnectionEnumerator nextObject])) {
			NSLog(@"Video Input Format: %@\n", [[videoConnection formatDescription] localizedFormatSummary]);
		}
		
		NSEnumerator *audioConnectionEnumerator = [[audioDeviceInput connections] objectEnumerator];
		QTCaptureConnection *audioConnection;
		while ((audioConnection = [audioConnectionEnumerator nextObject])) {
			NSLog(@"Audio Input Format: %@\n", [[audioConnection formatDescription] localizedFormatSummary]);
		}   
		
		[self startSession];
	}
	return self;
}

- (void) startSession
{
	//start the session
	if(verbose) ofLogVerbose() << "starting video session";
	[session startRunning];
	
}

- (void) setVideoDeviceID:(NSInteger)_videoDeviceID
{	
	if(videoDeviceID != _videoDeviceID){
		
		//get video device
		NSArray* videoDevices = [[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo] 
								 arrayByAddingObjectsFromArray:[QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeMuxed]];
		
		ofLog(OF_LOG_VERBOSE, "ofQTKitGrabber -- Video Device List:  %s", [[videoDevices description] cString]);
		
		// Try to open the new device
		if(_videoDeviceID >= videoDevices.count){
			ofLog(OF_LOG_ERROR, "ofQTKitGrabber - ERROR - Error video device ID out of range");
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
		
		ofLog(OF_LOG_VERBOSE, "ofQTKitGrabber -- Audio Device List:  %s", [[audioDevices description] cString]);
		
		// Try to open the new device
		if(_audioDeviceID >= audioDevices.count){
			ofLog(OF_LOG_ERROR, "ofQTKitGrabber - ERROR - Error audio device ID out of range");
			return;
		}		
		selectedAudioDevice = [audioDevices objectAtIndex:_audioDeviceID];
		if([self setSelectedAudioDevice:selectedAudioDevice]){
			audioDeviceID = _audioDeviceID;
		}
	}
}

- (bool) setSelectedVideoDevice:(QTCaptureDevice *)_selectedVideoDevice
{
	BOOL success = YES;	
	if (self.videoDeviceInput) {
		// Remove the old device input from the session and close the device
		[self.session removeInput:videoDeviceInput];
		[[self.videoDeviceInput device] close];
		[videoDeviceInput release];
		videoDeviceInput = nil;
	}
	
	if (_selectedVideoDevice) {
		NSError *error = nil;
		
		// Try to open the new device
		success = [_selectedVideoDevice open:&error];
		if(success){
			// Create a device input for the device and add it to the session
			self.videoDeviceInput = [[QTCaptureDeviceInput alloc] initWithDevice:_selectedVideoDevice];
			
			success = [self.session addInput:self.videoDeviceInput error:&error];
			if(verbose) ofLog(OF_LOG_VERBOSE, "ofQTKitGrabber -- Attached camera %s", [[_selectedVideoDevice description] cString]);
		}
	}
	
	if(!success) ofLog(OF_LOG_ERROR, "ofQTKitGrabber - ERROR - Error adding device to session");	
    
	return success;
}

- (bool) setSelectedAudioDevice:(QTCaptureDevice *)_selectedAudioDevice
{
	BOOL success = YES;	
	if (self.audioDeviceInput) {
		// Remove the old device input from the session and close the device
		[self.session removeInput:audioDeviceInput];
		[[self.audioDeviceInput device] close];
		[audioDeviceInput release];
		audioDeviceInput = nil;
	}
	
	if (_selectedAudioDevice) {
		NSError *error = nil;
		
		// Try to open the new device
		success = [_selectedAudioDevice open:&error];
		if(success){
			// Create a device input for the device and add it to the session
			self.audioDeviceInput = [[QTCaptureDeviceInput alloc] initWithDevice:_selectedAudioDevice];
			
			success = [self.session addInput:self.audioDeviceInput error:&error];
			ofLog(OF_LOG_VERBOSE, "ofQTKitGrabber -- Attached audio %s", [[_selectedAudioDevice description] cString]);
		}
	}
	
	if(!success) ofLog(OF_LOG_ERROR, "ofQTKitGrabber - ERROR - Error adding audio device to session");	
	
	return success;
}

- (void) initRecording:(NSString*)_selectedVideoCodec audioCodec:(NSString*)_selectedAudioCodec
{
	BOOL success = YES;	
	NSError *error = nil;
	
	// Create the movie file output and add it to the session		[added by gameover]
	self.captureMovieFileOutput = [[QTCaptureMovieFileOutput alloc] init];
	success = [self.session addOutput:captureMovieFileOutput error:&error];
	if (!success) {
		ofLog(OF_LOG_ERROR, "ofQTKitGrabber - ERROR - Error adding capture output to session");
	} 
    else {		
		[self setVideoCodec:_selectedVideoCodec];
		[self setAudioCodec:_selectedAudioCodec];
		isRecordReady = YES;
	}
}

+ (NSArray*) listVideoCodecs
{
	NSArray* videoCodecs = [QTCompressionOptions compressionOptionsIdentifiersForMediaType:QTMediaTypeVideo];
	
	ofLogVerbose("ofQTKitGrabber listing video compression options"); 
	[self enumerateArray:videoCodecs];
	
	return videoCodecs;
}

+ (NSArray*) listAudioCodecs
{
	NSArray* audioCodecs = [QTCompressionOptions compressionOptionsIdentifiersForMediaType:QTMediaTypeSound];
	
	ofLogVerbose("ofQTKitGrabber listing audio compression options"); 
	[self enumerateArray:audioCodecs];
	
	return audioCodecs;
}

- (void) setVideoCodec:(NSString*)_selectedVideoCodec
{
	// set codec on connection for type Video
	NSArray *outputConnections = [captureMovieFileOutput connections];
	QTCaptureConnection *connection;
	for (connection in outputConnections)
	{
		if ([[connection mediaType] isEqualToString:QTMediaTypeVideo])
			[captureMovieFileOutput setCompressionOptions:[QTCompressionOptions compressionOptionsWithIdentifier:_selectedVideoCodec] forConnection:connection];
	}
}

- (void) setAudioCodec:(NSString*)_selectedAudioCodec
{
	// set codec on connection for type Sound
	NSArray *outputConnections = [captureMovieFileOutput connections];
	QTCaptureConnection *connection;
	for (connection in outputConnections)
	{
		if ([[connection mediaType] isEqualToString:QTMediaTypeSound])
			[captureMovieFileOutput setCompressionOptions:[QTCompressionOptions compressionOptionsWithIdentifier:_selectedAudioCodec] forConnection:connection];
	}
}

- (void) startRecording:(NSString*)filePath
{
	if (isRecordReady) {
		
		BOOL success = YES;
		NSError *error = nil;
		
		if (isRecording) [self stopRecording]; // make sure last movie has stopped
		
		// set url for recording
		[captureMovieFileOutput recordToOutputFileURL:[NSURL fileURLWithPath:filePath]];
		
		ofLog(OF_LOG_VERBOSE, "Started recording movie to: %s", [filePath cString]);
		
		isRecording = YES;
		
	} else {
		ofLog(OF_LOG_ERROR, "ofQTKitGrabber - ERROR - Not set up to record - call initRecording first");
	}
}

- (void) stopRecording
{
	if (isRecordReady) {
		
		// set url to nil to stop recording
		[self.captureMovieFileOutput recordToOutputFileURL:nil];
		
		ofLog(OF_LOG_VERBOSE, "Stopped recording movie");
		
		isRecording = NO;
		
	} else {
		ofLog(OF_LOG_ERROR, "ofQTKitGrabber - ERROR - Not set up to record - call initRecording first");
	}
}

//Frame from the camera
//this tends to be fired on a different thread, so keep the work really minimal
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
            size_t dstBytesPerRow = pixels.getWidth() * 3;
            CVPixelBufferLockBaseAddress(cvFrame, kCVPixelBufferLock_ReadOnly);
            if (CVPixelBufferGetBytesPerRow(cvFrame) == dstBytesPerRow) {
                memcpy(pixels.getPixels(), CVPixelBufferGetBaseAddress(cvFrame), pixels.getHeight() * dstBytesPerRow);
            }
            else {
                unsigned char *dst = pixels.getPixels();
                unsigned char *src = (unsigned char*)CVPixelBufferGetBaseAddress(cvFrame);
                size_t srcBytesPerRow = CVPixelBufferGetBytesPerRow(cvFrame);
                size_t copyBytesPerRow = MIN(dstBytesPerRow, srcBytesPerRow); // should always be dstBytesPerRow but be safe
                for (int y = 0; y < pixels.getHeight(); y++){
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
    if ([[videoDeviceInput device] isOpen]) [[videoDeviceInput device] close];
    if ([[audioDeviceInput device] isOpen]) [[audioDeviceInput device] close];
    
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
	
	if(isInited)[grabber setVideoDeviceID:videoDeviceID];
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
	
	if(isInited) [grabber setAudioDeviceID:audioDeviceID];
	[pool release];	
}

bool ofQTKitGrabber::initGrabber(int w, int h){
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	grabber = [[QTKitVideoGrabber alloc] initWithWidth:w height:h videodevice:videoDeviceID audiodevice:audioDeviceID usingAudio:bUseAudio];
	isInited = (grabber != nil);
	[pool release];
    return isInited;
}

// used to init with no texture or preview etc ie., recording only
bool ofQTKitGrabber::initGrabberWithoutPreview(){
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	grabber = [[QTKitVideoGrabber alloc] initWithoutPreview:videoDeviceID audiodevice:audioDeviceID usingAudio:bUseAudio];
	isInited = (grabber != nil);
	[pool release];	
    return isInited;
}

void ofQTKitGrabber::initRecording(){
	if(confirmInit()){
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		NSString * NSvideoCodec = [NSString stringWithUTF8String: videoCodecIDString.c_str()];
		NSString * NSaudioCodec = [NSString stringWithUTF8String: audioCodecIDString.c_str()];
		[grabber initRecording:NSvideoCodec audioCodec:NSaudioCodec];
		[pool release];	
	}
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
	if(isInited){
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];	
		NSString * NSvideoCodec = [NSString stringWithUTF8String: videoCodecIDString.c_str()];
		[grabber setVideoCodec:NSvideoCodec];
		[pool release];
	}
	
}

void ofQTKitGrabber::setAudioCodec(string _audioCodec){
	audioCodecIDString = _audioCodec;	
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];	
	NSString * NSaudioCodec = [NSString stringWithUTF8String: audioCodecIDString.c_str()];
	[grabber setVideoCodec:NSaudioCodec];
	[pool release];
}

void ofQTKitGrabber::startRecording(string filePath){
	NSString * NSfilePath = [NSString stringWithUTF8String: ofToDataPath(filePath).c_str()];
	[grabber startRecording:NSfilePath];
}

void ofQTKitGrabber::stopRecording(){
	[grabber stopRecording];
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

bool ofQTKitGrabber::isRecording(){
	return isReady() && [grabber isRecording];
}

// would be better if listDevices returned a vector of devices too, 
// but that requires updating the base class...perhaps we could
// then have a ofBaseDevice class to be used for enumerating any 
// type of device for video, sound, serial devices etc etc???
void ofQTKitGrabber::listDevices(){
    listVideoDevices();
}

void ofQTKitGrabber::setPixelFormat(ofPixelFormat pixelFormat){
	if(pixelFormat != OF_PIXELS_RGB){
        ofLogWarning() << "ofQTKitGrabber::setPixelFormat  -- only supports OF_PIXELS_RGB" << endl;
    }
}

ofPixelFormat ofQTKitGrabber::getPixelFormat(){
    return OF_PIXELS_RGB;
}

vector<string>& ofQTKitGrabber::listVideoDevices(){
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSArray* videoDevices = [QTKitVideoGrabber listDevices];
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
	[grabber stop];
	[grabber release];
	isInited = false;	
}

unsigned char* ofQTKitGrabber::getPixels(){
	if(confirmInit()){
		return [grabber pixels].getPixels();
	}
	return NULL;
}

ofPixelsRef ofQTKitGrabber::getPixelsRef(){
	if(confirmInit()){
		return [grabber pixels];
	}
}

void ofQTKitGrabber::setUseAudio(bool _bUseAudio){
	if(_bUseAudio != bUseAudio){
		if(isInited){
			ofLog(OF_LOG_ERROR, "ofQTKitGrabber -- Requesting to use audio after grabber is already initialized. Try doing this first!");
		}
		bUseAudio = _bUseAudio;
	}
}


void ofQTKitGrabber::setVerbose(bool bTalkToMe){
	if(confirmInit()){
		grabber.verbose = bTalkToMe;
	}
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
		ofLog(OF_LOG_ERROR, "ofQTKitGrabber -- ERROR -- Calling method on non intialized video grabber");
	}
	return isInited;
}

