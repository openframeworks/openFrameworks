/***********************************************************************
 
 Copyright (C) 2011 by Zach Gage
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
This is based on code from:
 
http://stormyprods.com/SoundEngine

 
and apple:
 
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
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

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
		if an unloaded is trying to be accessed.
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
		kSoundEngineErrDeviceNotFound		= 5,
		kSoundEngineErrNoSourcesAvailable   = 6,
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
	@result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_Initialize(Float32 inMixerOutputRate);

/*!
    @function       SoundEngine_Teardown
    @abstract       Tearsdown the sound engine.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_Teardown();

/*!
    @function       SoundEngine_SetMasterVolume
    @abstract       Sets the overall volume of all sounds coming from the process
    @param          inValue
                        A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
    @result         A OSStatus indicating success or failure.
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
    @result         A OSStatus indicating success or failure.
*/
	OSStatus  SoundEngine_SetListenerVelocity(Float32 inX, Float32 inY, Float32 inZ);

	OSStatus  SoundEngine_SetListenerPosition(Float32 inX, Float32 inY, Float32 inZ);

/*!
    @function       SoundEngine_SetListenerGain
    @abstract       Sets the gain of the listener. Must be >= 0.0
    @param          inValue
                        A Float32 that represents the listener's gain
    @result         A OSStatus indicating success or failure.
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
	@result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_LoadBackgroundMusicTrack(const char* inPath, Boolean inAddToQueue, Boolean inLoadAtOnce);

/*!
    @function       SoundEngine_UnloadBackgroundMusicTrack
    @abstract       Tells the background music player to release all resources and stop playing.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_UnloadBackgroundMusicTrack();

/*!
    @function       SoundEngine_StartBackgroundMusic
    @abstract       Tells the background music player to start playing.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StartBackgroundMusic();

/*!
    @function       SoundEngine_StopBackgroundMusic
    @abstract       Tells the background music player to stop playing.
    @param          inAddToQueue
                        If true, playback will stop when all tracks have completed. If false, playback
						will stop immediately.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StopBackgroundMusic(Boolean inStopAtEnd);
	
void  SoundEngine_SetBackgroundMusicLooping(Boolean loop);
/*!
    @function       SoundEngine_SetBackgroundMusicVolume
    @abstract       Sets the volume for the background music player
    @param          inValue
                        A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetBackgroundMusicVolume(Float32 inValue);
	
void	  SoundEngine_ClearSources();


/*!
    @function       SoundEngine_LoadEffect
    @abstract       Loads a sound effect from a file and return an ID to refer to that effect.
    @param          inPath
                        The absolute path to the file to load.
	@param			outEffectID
						A UInt32 ID that refers to the effect.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_LoadEffect(const char* inPath, UInt32* outEffectID);

/*!
    @function       SoundEngine_UnloadEffect
    @abstract       Releases all resources associated with the given effect ID
    @param          inEffectID
                        The ID of the effect to unload.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_UnloadEffect(UInt32 inEffectID);

	
/*!
 @function       SoundEngine_StartEffect
 @abstract       Binds the sound effect buffer to a source
 @param          inEffectID
					The ID of the effect to prime.
 @param			outSourceID
					A ALuint ID that refers to the OpenAL source.
 @result         The index of the primed effect in the map for unpriming later
 */
int  SoundEngine_PrimeEffect(UInt32 inEffectID, ALuint *outSourceID); 

void SoundEngine_SetEffectPosition(ALuint sourceID, float position);

float SoundEngine_GetEffectPosition(ALuint sourceID);
	
unsigned long SoundEngine_GetEffectLength(ALint _id);

/*!
 @function       SoundEngine_Update
 @abstract       Checks to see if the sound as finished playing. if it has, releases the source
 @param          sourceID
 The ID of the effect to check.
 @param			mSource
the id of the buffer that has been used.
 @result         weather the sound is finished and the buffer has been released
 */
bool  SoundEngine_Update(ALuint sourceID, int mSource); 
	
	
/*!
    @function       SoundEngine_StartEffect
    @abstract       Starts playback of a source
    @param          sourceID
                        The ID of the source to start.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StartEffect(ALuint sourceID);
	
OSStatus  SoundEngine_PauseEffect(ALuint sourceID);

/*!
    @function       SoundEngine_StopEffect
    @abstract       Stops playback of a source
	@param          sourceID
						The ID of the source to stop.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_StopEffect(ALuint sourceID);

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
	@param          sourceID
						The ID of the source to adjust.
    @param          inValue
                        A Float32 that represents the pitch scalar, with 1.0 being unchanged. Must 
						be greater than 0.
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetEffectPitch(ALuint sourceID, Float32 inValue);

/*!
    @function       SoundEngine_SetEffectVolume
    @abstract       Sets the volume for an effect
	@param          sourceID
						The ID of the source to adjust.
    @param          inValue
                        A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
    @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetEffectLevel(ALuint sourceID, Float32 inValue);

/*!
    @function       SoundEngine_SetEffectLocation
    @abstract       Tells the engine whether a given effect should loop when played or if it should
					play through just once and stop.
	@param          sourceID
						The ID of the source to adjust.
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
	@result         A OSStatus indicating success or failure.
*/
OSStatus	SoundEngine_SetEffectLocation(ALuint sourceID, Float32 inX, Float32 inY, Float32 inZ);

/*!
   @function       SoundEngine_SetEffectsVolume
   @abstract       Sets the overall volume for the effects
   @param          inValue
                       A Float32 that represents the level. The range is between 0.0 and 1.0 (inclusive).
   @result         A OSStatus indicating success or failure.
*/
OSStatus  SoundEngine_SetEffectsVolume(Float32 inValue);

/*!
   @function       SoundEngine_SetMaxDistance
   @abstract       Sets the maximum distance for effect attenuation. Gain will attenuate between the
				   reference distance and the maximum distance, after which gain will be 0.0
   @param          inValue
                       A Float32 that represents the level. Must be greater than 0.0.
   @result         A OSStatus indicating success or failure.
*/
OSStatus	SoundEngine_SetMaxDistance(Float32 inValue);
	
UInt64  SoundEngine_getBackgroundMusicLength();
	
bool  SoundEngine_getBackgroundMusicStopped();
	
void SoundEngine_setBackgroundMusicPosition(UInt64 pos);


OSStatus	SoundEngine_SetLooping(bool looping, ALint _id);

/*!
   @function       SoundEngine_SetReferenceDistance
   @abstract       Sets the distance at which effect gain attenuation begins. It will attenuate between
				   the reference distance and the maximum distance. Sounds closer than the reference
				   distance will have no attenuation applied
   @param          inValue
                       A Float32 that represents the level. Must be greater than 0.0.
   @result         A OSStatus indicating success or failure.
*/
OSStatus	SoundEngine_SetReferenceDistance(Float32 inValue);
	

#if defined(__cplusplus)
}
#endif

#endif
