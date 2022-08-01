//
//  ofAVEngineSoundPlayer.hpp
//  soundPlayerExample
//
//  Created by Theo Watson on 3/24/21.
//

#pragma once

#include "ofSoundBaseTypes.h"
#include "ofEvents.h"

class ofAVEngineSoundPlayer : public ofBaseSoundPlayer {
    
public:

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
    void * soundPlayer;
	std::vector <void *> mMultiplayerSoundPlayers;
	static std::vector<float> systemBins;

};
