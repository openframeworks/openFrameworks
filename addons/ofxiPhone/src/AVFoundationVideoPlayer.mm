
#import "AVFoundationVideoPlayer.h"

#import <AVFoundation/AVVideoSettings.h>
#import <AVFoundation/AVAudioSettings.h>
#import <CoreAudio/CoreAudioTypes.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@implementation AVFoundationVideoPlayer

static const int BUFFER_BYTE_SIZE = 8192;

//#define DECODE_AUDIO_REMOTEIO

void audioCallback( void* aqData, AudioQueueRef queue, AudioQueueBufferRef buffer);

- (id)init
{
	paused = true;
	finished = false;
	initialised = false;
	asset_reader = nil;
	audio_queue = nil;
	audio_finished = false;
	told_midpoint = false;
	bDoAudio	  = false;
	
	vol = 1;
	
	current_video_time = 0;
	presentation_timestamp_s = current_video_time-1.0f;
	last_returned_frame_presentation_timestamp_s = presentation_timestamp_s;
	
	return self;
}

- (id)initWithURL:(NSURL*)url
{
	//--------
	AudioSessionInitialize ( NULL, NULL,NULL,NULL);
	UInt32 category = kAudioSessionCategory_AmbientSound;
	OSStatus result = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(category), &category);
	if ( result != noErr ) 
		NSLog(@"!!couldn't AudioSessionSetProperty( category ): OSStatus %i", result);
	
	UInt32 one = 1;
	result = AudioSessionSetProperty( kAudioSessionProperty_OverrideCategoryMixWithOthers, sizeof(one), &one );
	if ( result != noErr )
		NSLog(@"!!couldn't AudioSessionSetProperty( override mix with others ): OSStatus %i", result );
	AudioSessionSetActive(true);
	//--------
	
	[self init];
	NSLog(@"opening asset %@", url);
	
	// construct and initialise
	asset = [[AVURLAsset alloc] initWithURL:url options:nil];
	
	[asset loadValuesAsynchronouslyForKeys:[NSArray arrayWithObject:@"tracks"] completionHandler:^{
		dispatch_async(dispatch_get_main_queue(), ^{
			[self continueInit];
		});
	}];
	
	playOnLoad=false;
	
	return self;
}

- (id)initWithPath:(NSString*)path
{
	return [self initWithURL:[NSURL fileURLWithPath:path]];
}

- (void)continueInit
{	
	NSLog(@"continueInit: paused %c", paused?'y':'n' );
	NSError *error = nil;
	if([asset statusOfValueForKey:@"tracks" error:&error] == AVKeyValueStatusFailed)
	{
		NSLog(@"error loading: %@", [error description]);
		finished = true;
		return;
	}

	
	NSArray* video_tracks = [asset tracksWithMediaType:AVMediaTypeVideo];
	NSArray* audio_tracks = [asset tracksWithMediaType:AVMediaTypeAudio];

	NSLog(@"asset opened, creating reader");
	asset_reader = [[AVAssetReader alloc] initWithAsset:asset error:&error];
	if ( error != nil )
	{
		NSLog(@"asset_reader: error during initialisation: %@", error );
		finished = true;
	}
	else
	{
		NSLog(@"asset reader is [%x] %@ %i", asset_reader, asset_reader, [asset_reader retainCount] );
		
		if ( [video_tracks count] == 0 )
		{
			NSLog(@"asset has no video tracks");
			finished = true;
		}
		else
		{
			// add track reader for video track
			AVAssetTrack* video_track = [video_tracks objectAtIndex:0];
			video_duration = CMTimeGetSeconds( video_track.timeRange.duration);
			NSLog(@"reader created, creating reader track output for video track (%f fps, duration %f)", video_track.nominalFrameRate, 
				  video_duration );

			NSMutableDictionary* dictionary = [[[NSMutableDictionary alloc] init] autorelease];
			[dictionary setObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
			asset_reader_video_output = [[AVAssetReaderTrackOutput alloc] initWithTrack:video_track outputSettings:dictionary];
			/*asset_reader_video_output = [[AVAssetReaderVideoCompositionOutput alloc] 
										 initWithVideoTracks:[NSArray arrayWithObject:video_track] 
										 videoSettings:dictionary];*/
			//asset_reader_video_output = [[AVAssetReaderTrackOutput alloc] initWithTrack:video_track outputSettings:nil];
			if ( asset_reader_video_output == nil )
			{
				NSLog(@"asset_reader_output initWithTrack failed for video track");
			}
			else
			{
				if ( ![ asset_reader canAddOutput:asset_reader_video_output ] )
				{
					NSLog(@"cannot add video output");
				}
				else
					[asset_reader addOutput:asset_reader_video_output];
			}
		}
		
		
		if ( [audio_tracks count] == 0 )
		{
			NSLog(@"asset has no audio tracks");
			bDoAudio = false;
		}
		else
		{
			// add track reader for audio track
			AVAssetTrack* audio_track = [audio_tracks objectAtIndex:0];
			NSLog(@"reader created, creating reader track output for audio track");
			
			NSMutableDictionary* dictionary = [[[NSMutableDictionary alloc] init] autorelease];
			// set up linear pcm, 44100 Hz, 16 bit, 2 channels
			[dictionary setObject:[NSNumber numberWithInt:kAudioFormatLinearPCM] forKey:AVFormatIDKey];
			[dictionary setObject:[NSNumber numberWithInt:16] forKey:AVLinearPCMBitDepthKey];
			[dictionary setObject:[NSNumber numberWithBool:FALSE] forKey:AVLinearPCMIsFloatKey];
			
			asset_reader_audio_output = [[AVAssetReaderTrackOutput alloc] initWithTrack:audio_track outputSettings:dictionary];
			
			if ( asset_reader_audio_output == nil )
			{
				NSLog(@"asset_reader_output initWithTrack failed for audio track");
			}
			else
			{
				bDoAudio = true;
			
				[asset_reader addOutput:asset_reader_audio_output];

				// get track data formats
				NSArray* format_descriptions = [audio_track formatDescriptions];
				//NSLog(@"first format description: %@", [format_descriptions objectAtIndex:0]);
				CMFormatDescriptionRef format_description = (CMFormatDescriptionRef)[format_descriptions objectAtIndex:0];
				const AudioStreamBasicDescription* audio_track_description = CMAudioFormatDescriptionGetStreamBasicDescription( format_description );
				audio_n_channels = audio_track_description->mChannelsPerFrame;
				//NSLog(@"number of channels: %i", audio_n_channels );
				
				
				AudioStreamBasicDescription input_format;
				input_format.mFormatID = kAudioFormatLinearPCM;
				input_format.mSampleRate = 44100;
				input_format.mFormatFlags = kAudioFormatFlagIsSignedInteger;
				input_format.mChannelsPerFrame = audio_n_channels;
				input_format.mFramesPerPacket = 1;
				input_format.mBytesPerFrame = 2*input_format.mChannelsPerFrame;
				input_format.mBytesPerPacket = input_format.mBytesPerFrame*input_format.mFramesPerPacket;
				input_format.mBitsPerChannel = 16;
				NSLog(@"calling AudioQueueNewOutput with self %x", self );
				OSStatus error = AudioQueueNewOutput (
										&input_format,
										audioCallback,
										self,
										NULL,
										NULL,
										0,
										&audio_queue
									);
				if ( error != noErr ){
					NSLog(@"error opening audio queue: OSStatus %lu\n", error );
					bDoAudio = false;
				}
				
				for ( int i=0; i<NUM_AUDIO_BUFFERS; i++ )
				{
					// create the buffer
					error = AudioQueueAllocateBuffer(audio_queue, BUFFER_BYTE_SIZE, &audio_buffer[i]);
					if ( error != noErr )
						NSLog(@"error allocating audio buffer %i: OSStatus %lu\n", i, error );
				}
				
			}
		}
		 
		
		buffer = nil;
		audio_from_avasset_buffer = nil;

		

		// start reading
		NSLog(@"starting reader");
		BOOL started = [asset_reader startReading];
		if ( !started )
			NSLog(@"asset_reader couldn't startReading: %@", [asset_reader error]);
		// calculate audio lag
		audio_lag = float(NUM_AUDIO_BUFFERS*BUFFER_BYTE_SIZE/(2*2))/44100.0f;
		presentation_timestamp_s = current_video_time-1.0f;


		if( bDoAudio ){
			// start audio queue
			//NSLog(@"priming audio queue");
			// prime buffers with first bit of data
			for ( int i=0; i<NUM_AUDIO_BUFFERS; i++ )
			{
				audioCallback( (void*)self, audio_queue, audio_buffer[i] );
			}
			UInt32 num_prepared;
			OSStatus status = AudioQueuePrime( audio_queue, 0, &num_prepared );
			if ( status != noErr )
				NSLog(@"error calling AudioQueuePrime: %lu", status );
			//NSLog(@"AudioQueuePrime prepared %lu samples", num_prepared );
		}
		
		#ifdef DECODE_AUDIO_REMOTEIO
		NSLog(@"starting remote io");
		[self setupAudio];
		#endif
		
		current_video_time = 0;
		initialised = true;
	}					   
	if(playOnLoad)
	{
		playOnLoad = false;
		[self play];
	}
}


- (void)addStatusListener:(UIViewController <AVFoundationVideoPlayerStatusListener>*)listener
{
	status_listeners.push_back( listener );
}

- (void)tellListenersStatusChanged:(NSString*)message withArgument:(float)arg
{
	for ( int i=0; i<status_listeners.size(); i++ )
	{
		[status_listeners[i] AVFoundationVideoPlayerStatusChanged:message withArgument:arg];
	}
}



- (void)updateWithElapsedTime:(float)elapsed
{
	//NSLog(@"AVFoundationVideoPlayer: updateWithElapsedTime");

	if(! finished)
	{
		if ( !paused )
		{
			current_video_time += elapsed;
			float percent = current_video_time/video_duration;
			percent = min(1.0f,max(0.0f,percent));
			[self tellListenersStatusChanged:VIDEO_FRAME_EXTRACTOR_PROGRESS withArgument:percent];
			if ( percent > 0.5f && !told_midpoint )
			{
				[self tellListenersStatusChanged:VIDEO_FRAME_EXTRACTOR_MIDPOINT withArgument:nil];
				told_midpoint = true;
			}
		}
		
		// bail out if we're not already reading
		if ( asset_reader == nil || [asset_reader status]!=AVAssetReaderStatusReading )
		{
			//NSLog(@"AVAssetReaderStatus is not playing");
			if ( buffer != nil )
				CFRelease( buffer );
			buffer = nil;

			if ( [asset_reader status] == AVAssetReaderStatusCompleted )
			{
				finished = true;
				NSLog(@"updateWithElapsedTime: set finished to true");
			}
			
			return;
		}

		// loop through frames until we are synchronised
		while ( [asset_reader status]==AVAssetReaderStatusReading && presentation_timestamp_s < current_video_time )
		{		
			if ( buffer != nil )
				CFRelease( buffer );
			//NSLog(@"copyNextSampleBuffer");
			
			buffer = [asset_reader_video_output copyNextSampleBuffer];
			CMTime presentation_timestamp = CMSampleBufferGetPresentationTimeStamp( buffer );
			presentation_timestamp_s = CMTimeGetSeconds( presentation_timestamp );
			/*NSLog(@"video: copyNextSampleBuffer at time %f: buf  %x, pres_time %f, status: %i, error: %@", current_video_time, buffer, presentation_timestamp_s, 
				  [asset_reader status], [asset_reader error] );*/
		}
	}
}


void audioCallback( void* aqData, AudioQueueRef queue, AudioQueueBufferRef output_buffer)
{
	AVFoundationVideoPlayer* extractor = (AVFoundationVideoPlayer*)aqData;
	[extractor audioCallbackWithQueue:queue output:output_buffer];

}


- (void)audioCallbackWithQueue:(AudioQueueRef)queue output:(AudioQueueBufferRef)output_buffer
{
	audio_mutex.lock();
	
	AudioQueueSetParameter(audio_queue, kAudioQueueParam_Volume, vol);
	
	assert( queue == audio_queue );
	
	output_buffer->mAudioDataByteSize = BUFFER_BYTE_SIZE;
	uint32_t bytes_remaining = BUFFER_BYTE_SIZE;
	
	while ( bytes_remaining > 0 )
	{
		// check if we should get a new one
		if ( audio_from_avasset_buffer == nil || 
			audio_from_avasset_samples_given_out >= CMSampleBufferGetNumSamples(audio_from_avasset_buffer) )
		{
			if ( audio_from_avasset_buffer != nil )
			{
				CFRelease( audio_from_avasset_buffer );
				audio_from_avasset_buffer = nil;
			}

			if ( asset_reader && [asset_reader status] != AVAssetReaderStatusReading )
			{
				// pool for the error message
				NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
				//NSLog(@"audio: asset_reader is not ready; in fact is %i", [asset_reader status]);
				if ( [asset_reader status] == AVAssetReaderStatusFailed )
				{
					NSLog(@"audio: asset_reader failed, error %@", [asset_reader error] );
				}
				[self pause];
				finished = true;
				[pool release];
				bytes_remaining = 0;
				break;
			}
			else if ( presentation_timestamp_s == 0 )
			{
				NSLog(@"audio: asset_reader or video playback is not ready, bailing out");
				bytes_remaining = 0;
				break;
				
			}
			
			if ( !audio_finished )
			{
				// pool for copyNextSampleBuffer
				NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
				// actually copy the buffer
				audio_from_avasset_buffer = [asset_reader_audio_output copyNextSampleBuffer];
				if ( audio_from_avasset_buffer == nil )
				{
					NSLog(@"audio: no more data\n" );
					audio_finished = true;
				}
				else
				{
					audio_from_avasset_samples_given_out = 0;
					// what's the timestamp?
					CMTime presentation_timestamp_audio = CMSampleBufferGetPresentationTimeStamp( audio_from_avasset_buffer );
					float presentation_timestamp_audio_s = CMTimeGetSeconds( presentation_timestamp_audio );
					current_video_time = max(0.0f,presentation_timestamp_audio_s - audio_lag);
					//NSLog(@"audio: video time %f (timestamp %f), audio: %f diff: %f\n", current_video_time, presentation_timestamp_s, presentation_timestamp_audio_s, presentation_timestamp_s-presentation_timestamp_audio_s );
				}
				[pool release];
			}
		}
		
		if ( audio_from_avasset_buffer == nil )
		{
			NSLog(@"audio: buffer is nil");
			memset( &((unsigned char*)output_buffer->mAudioData)[BUFFER_BYTE_SIZE-bytes_remaining], 0, bytes_remaining );
			bytes_remaining = 0;
		}
		else
		{

			// copy from audio_from_avasset_buffer to output_buffer
			Boolean is_ready = CMSampleBufferDataIsReady(audio_from_avasset_buffer); 
			if ( !is_ready )
			{
				NSLog(@"in audioCallback, sample wasn't ready, bailing out");
				bytes_remaining = 0;
				break;
			}
			
			
			CMBlockBufferRef blockBuffer;
			AudioBufferList audioBufferList;
			
			//allocates new buffer memory
			CMSampleBufferGetAudioBufferListWithRetainedBlockBuffer(audio_from_avasset_buffer, NULL, 
																	&audioBufferList, sizeof(audioBufferList),NULL, NULL, 0, &blockBuffer);
			
			int16_t* the_audio_buffer = (int16_t*)audioBufferList.mBuffers[0].mData;
			//NSLog(@"audio: %i source buffers; 0 has %i channels", audioBufferList.mNumberBuffers, audioBufferList.mBuffers[0].mNumberChannels );
			
			// get number of samples in current block
			uint32_t sample_count = CMSampleBufferGetNumSamples(audio_from_avasset_buffer); 
			/*if ( audio_n_channels == 1 )
				sample_count /= 2;*/
			
			uint32_t samples_available = sample_count - audio_from_avasset_samples_given_out;
			
			uint32_t num_samples_to_copy = min( samples_available, bytes_remaining/(2*audio_n_channels) );
			uint32_t num_bytes_to_copy = num_samples_to_copy*2*audio_n_channels;
			if ( num_bytes_to_copy == 0 )
			{
				NSLog(@"no bytes to copy: something else went wrong, bailing");
				bytes_remaining = 0;
				break;
			}
//			NSLog(@"in audioCallback, wants %i bytes, buffer available %i, actually copying %u, ", output_buffer->mAudioDataByteSize, 
//				  samples_available*2, num_bytes_to_copy );
			memcpy( &((unsigned char*)output_buffer->mAudioData)[BUFFER_BYTE_SIZE-bytes_remaining],  
				   /* note: int16_t[] */&the_audio_buffer[audio_from_avasset_samples_given_out*audio_n_channels], 
				   num_bytes_to_copy );
			
			bytes_remaining -= num_bytes_to_copy;

			CFRelease(blockBuffer);
			
			audio_from_avasset_samples_given_out += num_samples_to_copy;
		}				   
	}
	
	
	//NSLog(@"enqueing buffer on queue %x (%i bytes)", queue, output_buffer->mAudioDataByteSize );
	AudioQueueEnqueueBuffer( queue, output_buffer, 0, NULL );

	audio_mutex.unlock();
	
}

- (bool)isPaused
{
	return paused;
}

- (bool)canPlay
{
	return paused && !finished && initialised;
}

- (void)setVolume:(float)volume
{
	vol = volume;
}

- (void)play
{
	
	if([self canPlay]) {
		#ifdef DECODE_AUDIO_REMOTEIO
			[self playAudio];
		#else
		
		if( bDoAudio ){
		//	NSLog(@"playing, paused %c, audio_queue %x", paused?'y':'n', audio_queue );
			
			if ( initialised && paused && audio_queue != NULL )
			{
				UInt32 is_running;
				UInt32 is_running_size = sizeof(is_running);
				AudioQueueGetProperty( audio_queue, kAudioQueueProperty_IsRunning, &is_running, &is_running_size );
				//NSLog(@"calling AudioQueueStart (%x) (isRunning %c)", audio_queue, is_running?'y':'n' );
				OSStatus status = AudioQueueStart( audio_queue, NULL );
				if ( status != noErr )
				{
					NSLog(@"error calling AudioQueueStart: %i", status );
				}
				
			}
		}
		
		#endif
			[self tellListenersStatusChanged:VIDEO_FRAME_EXTRACTOR_PAUSED withArgument:0.0f];
			paused = false;
	}
	else
		playOnLoad=true;
}

- (void)pause
{
#ifdef DECODE_AUDIO_REMOTEIO
	[self pauseAudio];
#else
	
	if( bDoAudio ){
	//	NSLog(@"pausing, paused %c, audio_queue %x", paused?'y':'n', audio_queue );
		if ( !paused && audio_queue != nil )
		{
			OSStatus status = AudioQueuePause( audio_queue );
			if ( status != noErr )
			{
				NSLog(@"error calling AudioQueuePause: %i", status );
			}
		}
	}
#endif
	[self tellListenersStatusChanged:VIDEO_FRAME_EXTRACTOR_PAUSED withArgument:1.0f];
	paused = true;
}


- (bool)hasNewFrame
{
	return presentation_timestamp_s > last_returned_frame_presentation_timestamp_s;
}

- (bool)isFinished
{
	return finished;
}

- (bool)isInErrorState
{
	return [asset_reader status] == AVAssetReaderStatusFailed;
}

- (CVImageBufferRef)getCurrentFrame
{
	// try to get video
	CVImageBufferRef image_buffer = CMSampleBufferGetImageBuffer(buffer);
	last_returned_frame_presentation_timestamp_s = presentation_timestamp_s;
	return image_buffer;
}

- (pair<UIImage*,CGImageRef>)getCopyOfCurrentFrame
{	
	// try to get video
	CVImageBufferRef image_buffer = CMSampleBufferGetImageBuffer(buffer);
	// lock the buffer
	CVPixelBufferLockBaseAddress(image_buffer,0); 
	// get pixels base address
	uint8_t *source_pixels = (uint8_t *)CVPixelBufferGetBaseAddress(image_buffer); 
	// get pixel buffer width/height/stride
	size_t stride = CVPixelBufferGetBytesPerRow(image_buffer); 
	size_t width = CVPixelBufferGetWidth(image_buffer); 
	size_t height = CVPixelBufferGetHeight(image_buffer);
	// unlock the image buffer
	CVPixelBufferUnlockBaseAddress(image_buffer,0);
	
	//NSLog(@"sample buffer for time %f is %ix%i (stride %i)", presentation_timestamp_s, width, height, stride );
	
	// now convert the image buffer to a UIImage
	// first create a CGImageRef
	CGColorSpaceRef color_space = CGColorSpaceCreateDeviceRGB(); 
	CGContextRef cg_context = CGBitmapContextCreate(source_pixels, width, height, 8, stride, color_space, kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Big ); 
	
	CGImageRef cg_image = CGBitmapContextCreateImage(cg_context); 
	
	// create the actual UIImage
	UIImage *ui_image = [UIImage imageWithCGImage:cg_image scale:1.0 orientation:UIImageOrientationRight];
	
	// cleanup
	CGContextRelease(cg_context); 
	CGColorSpaceRelease(color_space);

	last_returned_frame_presentation_timestamp_s = presentation_timestamp_s;

	return make_pair(ui_image,cg_image);
}
	





- (void)dealloc
{
	NSLog(@"in AVFoundationVideoPlayer dealloc");
	if ( audio_queue )
	{
		NSLog(@"about to AudioQueueStop");
		OSStatus status;
		status = AudioQueueStop( audio_queue, true );
		status = AudioQueueDispose( audio_queue, true );
		if (status != noErr )
			NSLog(@"error calling AudioQueueStop: %i");
	}
	
	[asset release];
	[asset_reader release];
	if ( asset_reader_audio_output != nil )
		[asset_reader_audio_output release];
	if ( asset_reader_video_output != nil )
		[asset_reader_video_output release];
	if ( buffer != nil )
		CFRelease( buffer );
	if ( audio_from_avasset_buffer != nil )
		CFRelease( audio_from_avasset_buffer );
	
	[super dealloc];
}

- (float)getVideoTime
{
	return current_video_time;
}

- (float)getVideoPosition
{
	return current_video_time/video_duration;
}

- (Float64)getDuration
{
	return video_duration;
}

@end
