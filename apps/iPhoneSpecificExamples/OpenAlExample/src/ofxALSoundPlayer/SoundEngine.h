/*

File: SoundEngine.h
Abstract: These functions play background music tracks, multiple sound effects,
and support stereo panning with a low-latency response.

Version: 1.8

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

/*==================================================================================================
	SoundEngine.h
==================================================================================================*/
#if !defined(__SoundEngine_h__)
#define __SoundEngine_h__

//==================================================================================================
//	Includes
//==================================================================================================

//	System Includes
#include <CoreAudio/CoreAudioTypes.h>
#include <AudioToolbox/AudioToolbox.h>

#if defined(__cplusplus)
extern "C"
{
#endif

//==================================================================================================
//	Sound Engine
//==================================================================================================
/*!
    @enum SoundEngine error codes
    @abstract   These are the error codes returned from the SoundEngine API.
    @constant   kSoundEngineErrUnitialized 
		The SoundEngine has not been initialized. Use SoundEngine_Initialize().
    @constant   kSoundEngineErrInvalidID 
		The specified EffectID was not found. This can occur if the effect has not been loaded, or
		if an unloaded effect is trying to be accessed.
    @constant   kSoundEngineErrFileNotFound 
		The specified file was not found.
    @constant   kSoundEngineErrInvalidFileFormat 
		The format of the file is invalid. Effect data must be little-endian 8 or 16 bit LPCM.
    @constant   kSoundEngineErrDeviceNotFound 
		The output device was not found.

*/
enum {
		kSoundEngineErrUnitialized			= 1,
		kSoundEngineErrInvalidID			= 2,
		kSoundEngineErrFileNotFound			= 3,
		kSoundEngineErrInvalidFileFormat	= 4,
		kSoundEngineErrDeviceNotFound		= 5
};


/*!
    @function       SoundEngine_Initialize
    @abstract       Initializes and sets up the sound engine. Calling after a previous initialize will
						reset the state of the SoundEngine, with all previous effects and music tracks
						erased. Note: This is not required, loading an effect or background track will 
						initialize as necessary.
    @param          inMixerOutputRate
                        A Float32 that represents the output sample rate of the mixer unit. Setting this to 
						0 will use the default rate (the sample rate of the device)
	@result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_Initialize(Float32 inMixerOutputRate);

/*!
    @function       SoundEngine_Teardown
    @abstract       Tearsdown the sound engine.
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_Teardown();

/*!
    @function       SoundEngine_SetMasterVolume
    @abstract       Sets the overall volume of all sounds coming from the process
    @param          inValue
                        A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetMasterVolume(Float32 inValue);

/*!
    @function       SoundEngine_SetListenerPosition
    @abstract       Sets the position of the listener in the 3D space
    @param          inX
                        A Float32 that represents the listener's position along the X axis.
    @param          inY
                        A Float32 that represents the listener's position along the Y axis.
    @param          inZ
                        A Float32 that represents the listener's position along the Z axis.
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetListenerPosition(Float32 inX, Float32 inY, Float32 inZ);
	
OSStatus  SoundEngine_SetListenerVelocity(Float32 inX, Float32 inY, Float32 inZ);

/*!
    @function       SoundEngine_SetListenerGain
    @abstract       Sets the gain of the listener. Must be >= 0.0
    @param          inValue
                        A Float32 that represents the listener's gain
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetListenerGain(Float32 inValue);

/*!
    @function       SoundEngine_LoadBackgroundMusicTrack
    @abstract       Tells the background music player which file to play
    @param          inPath
                        The absolute path to the file to play.
    @param          inAddToQueue
                        If true, file will be added to the current background music queue. If
						false, queue will be cleared and only loop the specified file.
    @param          inLoadAtOnce
                        If true, file will be loaded completely into memory. If false, data will be 
						streamed from the file as needed. For games without large memory pressure and/or
						small background music files, this can save memory access and improve power efficiency
	@result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_LoadBackgroundMusicTrack(const char* inPath, Boolean inAddToQueue, Boolean inLoadAtOnce);

/*!
    @function       SoundEngine_UnloadBackgroundMusicTrack
    @abstract       Tells the background music player to release all resources and stop playing.
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_UnloadBackgroundMusicTrack();

/*!
    @function       SoundEngine_StartBackgroundMusic
    @abstract       Tells the background music player to start playing.
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StartBackgroundMusic();

/*!
    @function       SoundEngine_StopBackgroundMusic
    @abstract       Tells the background music player to stop playing.
    @param          inAddToQueue
                        If true, playback will stop when all tracks have completed. If false, playback
						will stop immediately.
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StopBackgroundMusic(Boolean inStopAtEnd);

/*!
    @function       SoundEngine_SetBackgroundMusicVolume
    @abstract       Sets the volume for the background music player
    @param          inValue
                        A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetBackgroundMusicVolume(Float32 inValue);

/*!
    @function       SoundEngine_LoadLoopingEffect
    @abstract       Loads a sound effect from a file and return an ID to refer to that effect. Note: The files
						MUST all be in the same data format and sample rate
    @param          inLoopFilePath
                        The absolute path to the file to load. This is the file that will loop continually.
    @param          inAttackFilePath
                        The absolute path to the file to load. This will play once at the start of the effect.
						Set to NULL if no attack is desired, looping file will play immediately.
    @param          inDecayFilePath
                        The absolute path to the file to load. This will play once after looping has been ended. 
						Triggered using SoundEngine_StopEffect with the inDoDecay set to true. Set to NULL if no
						decay is desired, looping file will stop immediately. 
	@param			outEffectID
						A UInt32 ID that refers to the effect.
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_LoadLoopingEffect(const char* inLoopFilePath, const char* inAttackFilePath, const char* inDecayFilePath, UInt32* outEffectID);

/*!
    @function       SoundEngine_LoadEffect
    @abstract       Loads a sound effect from a file and return an ID to refer to that effect.
    @param          inPath
                        The absolute path to the file to load.
	@param			outEffectID
						A UInt32 ID that refers to the effect.
    @result         An OSStatus indicating success or failure.
*/
	

OSStatus  SoundEngine_LoadEffectFromData(int * data, int dataSize, int dataRate, bool bLooping, UInt32* outEffectID); //zach 
	
	
OSStatus  SoundEngine_LoadEffect(const char* inPath, UInt32* outEffectID);

/*!
    @function       SoundEngine_UnloadEffect
    @abstract       Releases all resources associated with the given effect ID
    @param          inEffectID
                        The ID of the effect to unload.
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_UnloadEffect(UInt32 inEffectID);

/*!
    @function       SoundEngine_StartEffect
    @abstract       Starts playback of an effect
    @param          inEffectID
                        The ID of the effect to start.
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StartEffect(UInt32 inEffectID);

/*!
    @function       SoundEngine_StopEffect
    @abstract       Stops playback of an effect
    @param          inEffectID
                        The ID of the effect to stop.
    @param          inDoDecay
                        Whether to play the decay file or stop immmediately (this is ignored
						for non-looping effects)
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StopEffect(UInt32 inEffectID, Boolean inDoDecay);

/*!
    @function       SoundEngine_Vibrate
    @abstract       Tells the device to vibrate
*/
#if TARGET_OS_IPHONE
	#define SoundEngine_Vibrate() AudioServicesPlaySystemSound(kSystemSoundID_Vibrate)
#endif

/*!
    @function       SoundEngine_SetEffectPitch
    @abstract       Applies pitch shifting to an effect
    @param          inEffectID
                        The ID of the effect to adjust
    @param          inValue
                        A Float32 that represents the pitch scalar, with 1.0 being unchanged. Must 
						be greater than 0.
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetEffectPitch(UInt32 inEffectID, Float32 inValue);

/*!
    @function       SoundEngine_SetEffectVolume
    @abstract       Sets the volume for an effect
    @param          inEffectID
                        The ID of the effect to adjust
    @param          inValue
                        A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
    @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetEffectLevel(UInt32 inEffectID, Float32 inValue);

/*!
    @function       SoundEngine_SetEffectPosition
    @abstract       Tells the engine whether a given effect should loop when played or if it should
					play through just once and stop.
    @param          inEffectID
                        The ID of the effect to adjust
    @param          inX
                        A Float32 that represents the effect's position along the X axis. Maximum distance
						is 100000.0 (absolute, not per axis), reference distance (distance from which gain 
						begins to attenuate) is 1.0
    @param          inY
                        A Float32 that represents the effect's position along the Y axis. Maximum distance
						is 100000.0 (absolute, not per axis), reference distance (distance from which gain 
						begins to attenuate) is 1.0
	@param          inZ
                        A Float32 that represents the effect's position along the Z axis. Maximum distance
						is 100000.0 by default (absolute, not per axis), reference distance (distance from 
						which gain begins to attenuate) is 1.0
	@result         An OSStatus indicating success or failure.
*/
OSStatus	SoundEngine_SetEffectPosition(UInt32 inEffectID, Float32 inX, Float32 inY, Float32 inZ);

/*!
   @function       SoundEngine_SetEffectsVolume
   @abstract       Sets the overall volume for the effects
   @param          inValue
                       A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
   @result         An OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetEffectsVolume(Float32 inValue);

/*!
   @function       SoundEngine_SetMaxDistance
   @abstract       Sets the maximum distance for effect attenuation. Gain will attenuate between the
				   reference distance and the maximum distance, after which gain will be 0.0
   @param          inValue
                       A Float32 that represents the level. Must be greater than 0.0.
   @result         An OSStatus indicating success or failure.
*/
OSStatus	SoundEngine_SetMaxDistance(Float32 inValue);

/*!
   @function       SoundEngine_SetReferenceDistance
   @abstract       Sets the distance at which effect gain attenuation begins. It will attenuate between
				   the reference distance and the maximum distance. Sounds closer than the reference
				   distance will have no attenuation applied
   @param          inValue
                       A Float32 that represents the level. Must be greater than 0.0.
   @result         An OSStatus indicating success or failure.
*/
OSStatus	SoundEngine_SetReferenceDistance(Float32 inValue);

#if defined(__cplusplus)
}
#endif

#endif
