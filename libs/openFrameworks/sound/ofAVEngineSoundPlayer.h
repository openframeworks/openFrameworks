//
//  ofAVEngineSoundPlayer.hpp
//  soundPlayerExample
//
//  Created by Theo Watson on 3/24/21.
//

#pragma once

#include "ofConstants.h"

#ifdef OF_SOUND_PLAYER_AV_ENGINE

#include "ofSoundBaseTypes.h"
#include "ofEvents.h"

#ifdef __OBJC__
    #import <Foundation/Foundation.h>
    #import <AVFoundation/AVFoundation.h>
    #import <Accelerate/Accelerate.h>
#endif

class ofAVEngineSoundPlayer : public ofBaseSoundPlayer {
    
public:

//thanks to @bangnoise for this trick 
#ifdef __OBJC__
    using ObjectType = id<NSObject>;
#else
    using ObjectType = void *;
#endif

    ofAVEngineSoundPlayer();
    ~ofAVEngineSoundPlayer();
    
    static std::vector <float> getSystemSpectrum(int bands);

    bool load(const std::filesystem::path& fileName, bool stream = false);
    void unload();
    void play();
    void stop();
	
    void setVolume(float vol);
    void setPan(float vol);
    void setSpeed(float spd);
    void setPaused(bool bP);
    void setLoop(bool bLp);
    void setMultiPlay(bool bMp);
    void setPosition(float pct);
    void setPositionMS(int ms);
    
    float getPosition() const;
    int getPositionMS() const;
    bool isPlaying() const;
    float getSpeed() const;
    float getPan() const;
    bool isLoaded() const;
    float getVolume() const;
    
    void * getAVEnginePlayer();
    
protected:
    
	void updateFunction(ofEventArgs & args);
	bool bAddedUpdate = false;
	
        void cleanupMultiplayers();
        static bool removeMultiPlayer(void * aPlayer);
        ObjectType soundPlayer;
	std::vector <ObjectType> mMultiplayerSoundPlayers;
	static std::vector<float> systemBins;

};

#endif
