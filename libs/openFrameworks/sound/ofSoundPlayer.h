#pragma once

#include "ofConstants.h"
#include "ofTypes.h"

/// \todo: FIX THIS!!!!!!
/// #warning FIX THIS.

/// \brief Stops all active sound players on FMOD-based systems (windows, osx).
void ofSoundStopAll();

/// \brief Cleans up FMOD (windows, osx).
void ofSoundShutdown();

/// \brief Sets global volume for FMOD-based sound players (windows, osx).
/// \param vol range is 0 to 1.
void ofSoundSetVolume(float vol);

/// \brief Call in your app's update() to update FMOD-based sound players.
void ofSoundUpdate();

/// \brief Gets a frequency spectrum sample, taking all current sound players into account.
///
/// Each band will be represented as a float between 0 and 1.
///
/// \warning This isn't implemented on mobile & embedded platforms.
/// \param nBands number of spectrum bands to return, max 512.
/// \return pointer to an FFT sample, sample size is equal to the nBands parameter.
float * ofSoundGetSpectrum(int nBands);

#include "ofBaseTypes.h"
#include "ofBaseSoundPlayer.h"


#ifdef OF_SOUND_PLAYER_QUICKTIME
#include "ofQuicktimeSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofQuicktimeSoundPlayer
#endif

#ifdef OF_SOUND_PLAYER_FMOD
#include "ofFmodSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofFmodSoundPlayer
#endif

#ifdef OF_SOUND_PLAYER_OPENAL
#include "ofOpenALSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofOpenALSoundPlayer
#endif

#ifdef TARGET_OF_IOS
#include "ofxiOSSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofxiOSSoundPlayer
#endif

#ifdef TARGET_ANDROID
#include "ofxAndroidSoundPlayer.h"
#define OF_SOUND_PLAYER_TYPE ofxAndroidSoundPlayer
inline void ofSoundShutdown(){}
#endif

#ifdef TARGET_LINUX_ARM
inline void ofSoundShutdown(){}
#endif

/// \class ofSoundPlayer
/// \brief Plays sound files.
///
/// ofSoundPlayer handles simple playback of sound files, with controls for
/// volume, pan, speed, seeking and multiplay.  This is a common cross-platform
/// sound player interface which is inherited by each of the platform-specific
/// sound player implementations.
class ofSoundPlayer : public ofBaseSoundPlayer {
public:
    ofSoundPlayer();

    void setPlayer(ofPtr<ofBaseSoundPlayer> newPlayer);
    ofPtr<ofBaseSoundPlayer> getPlayer();

    /// \brief Tells the sound player which file to play.
    ///
    /// Codec support varies by platform but wav, aif, and mp3 are safe.
    ///
    /// \param fileName Path to the sound file, relative to your app's data folder.
    /// \param stream set "true" to enable streaming from disk (for large files).
    bool loadSound(string fileName, bool stream = false);

    /// \brief Stops and unloads the current sound.
    void unloadSound();
    
    /// \brief Starts playback.
    void play();

    /// \brief Stops playback.
    void stop();

    /// \brief Sets playback volume.
    /// \param vol range is 0 to 1.
    void setVolume(float vol);

    /// \brief Sets stereo pan.
    /// \param pan range is -1 to 1 (-1 is full left, 1 is full right).
    void setPan(float pan);

    /// \brief Sets playback speed.
    /// \param speed set > 1 for faster playback, < 1 for slower playback.
    void setSpeed(float speed);

    /// \brief Enables pause / resume.
    /// \param paused "true" to pause, "false" to resume.
    void setPaused(bool paused);

    /// \brief Sets whether to loop once the end of the file is reached.
    /// \param loop "true" to loop, default is false.
    void setLoop(bool loop);
    
    /// \brief Enables playing multiple simultaneous copies of the sound.
    /// \param multiplay "true" to enable, default is false.
    void setMultiPlay(bool multiplay);

    /// \brief Sets position of the playhead within the file (aka "seeking").
    /// \param percent range is 0 (beginning of file) to 1 (end of file).
    void setPosition(float percent);
    
    /// \brief Sets position of the playhead within the file (aka "seeking").
    /// \param ms number of milliseconds from the start of the file.
    void setPositionMS(int ms);

    /// \brief Gets position of the playhead.
    /// \return playhead position in milliseconds.
    int getPositionMS();

    /// \brief Gets position of the playhead.
    /// \return playhead position as a float between 0 and 1.
    float getPosition();

    /// \brief Gets current playback state.
    /// \return true if the player is currently playing a file.
    bool getIsPlaying();

    /// \brief Gets playback speed.
    /// \return playback speed (see ofSoundPlayer::setSpeed()).
    float getSpeed();
    
    /// \brief Gets stereo pan.
    /// \return stereo pan in the range -1 to 1.
    float getPan();

    /// \brief Gets current volume.
    /// \return current volume in the range 0 to 1.
    float getVolume();

    /// \brief Queries the player to see if its file was loaded successfully.
    /// \return whether or not the player is ready to begin playback.
    bool isLoaded(); 		

protected:
    ofPtr<ofBaseSoundPlayer> player;

};
