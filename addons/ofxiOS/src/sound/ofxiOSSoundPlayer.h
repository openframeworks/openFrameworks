//
//  ofxiOSSoundPlayer.h
//  Created by Lukasz Karluk on 14/11/12.
//  http://julapy.com/blog
//

#pragma once

#include "ofMain.h"

class ofxiOSSoundPlayer : public ofBaseSoundPlayer {
    
public:

    ofxiOSSoundPlayer();
    ~ofxiOSSoundPlayer();
    
    bool loadSound(string fileName, bool stream = false);
    void unloadSound();
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
    
    float getPosition();
    int getPositionMS();
    bool getIsPlaying();
    float getSpeed();
    float getPan();
    bool isLoaded();
    float getVolume();
    
protected:
    
    void * soundPlayer;
    
};

#define ofxiPhoneSoundPlayer ofxiOSSoundPlayer