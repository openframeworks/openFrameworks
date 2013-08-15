//
//  ofSoundMixer.h
//  openFrameworksLib
//
//  Created by Roy Macdonald on 8/11/13.
//  Of dev con '13 YCAM

#pragma once
#include "ofConstants.h"
#include "ofSoundObject.h"
class ofSoundMixer;
ofSoundMixer &ofGetSystemSoundMixer();

class ofSoundMixer: public ofSoundObject {
public:
    ofSoundMixer(){
        masterVolume = 1.0f;
        masterPan = 0.5f;
    }
    ~ofSoundMixer(){}
    
    ofBaseSoundOutput& getChannelSource(int channelNumber);
    
    int getNumChannels();
    
    void setMasterVolume(float vol);
    float getMasterVolume();
    
    float getMasterPan();
    void  setMasterPan(float pan);
    
    void audioOut(ofSoundBuffer &output);
    bool isConnectedTo(ofSoundObject& obj);
    
protected:
    void disconnectInput(ofSoundObject * input);
    vector<ofSoundObject*>channels;
    float masterPan;
    float masterVolume;
    void setInput(ofSoundObject *obj);
};
