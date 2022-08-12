//
//  ofxiOSSoundPlayer.h
//  Created by Lukasz Karluk on 14/11/12.
//  http://julapy.com/blog
//

#pragma once

#include "ofSoundBaseTypes.h"

class ofxiOSSoundPlayer : public ofBaseSoundPlayer {
    
public:

    ofxiOSSoundPlayer();
    ~ofxiOSSoundPlayer();
    
    bool load(const of::filesystem::path& fileName, bool stream = false);
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
    
    void * getAVSoundPlayer();
    
protected:
    
    void * soundPlayer;
    
};

#define ofxiPhoneSoundPlayer ofxiOSSoundPlayer
