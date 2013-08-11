//
//  ofSoundMixer.h
//  openFrameworksLib
//
//  Created by Roy Macdonald on 8/11/13.
//  Of dev con '13 YCAM

#ifndef openFrameworksLib_ofSoundMixer_h
#define openFrameworksLib_ofSoundMixer_h

#include "ofMain.h"

class ofSoundMixerChannelStrip{//if you want to have a fancier mixer you can extend this class
public:
    ofSoundMixerChannelStrip(){}
    ofSoundMixerChannelStrip(string lbl ="",float vl =1.0f, float pn = 0.5f):vol(vl), pan(pn), label(lbl){}
    ~ofSoundMixerChannelStrip(){}
    
    float vol, pan;
    ofSoundObject * source;
    string label;
};

class ofSoundMixer: public ofSoundObject{
    ofSoundMixer();
    ~ofSoundMixer();
    ofSoundMixerChannelStrip& add(ofSoundObject &newSoundSource, string label ="");
    ofSoundMixerChannelStrip& newChannel(string label);
    //volume range 0.0 to 1.0
    float getChannelVolume(int channelNumber);
    float getChannelVolume(string label);
    
    void setChannelVolume(int channelNumber, float volume);
    void setChannelVolume(string label, float volume);

    ofBaseSoundOutput& getChannelStripSource(int channelNumber, bool createNewUnfound = true);
    ofBaseSoundOutput& getChannelStripSource(string label, bool createNewUnfound = true);
    
    ofSoundMixerChannelStrip& getChannelStrip(int channelNumber);
    ofSoundMixerChannelStrip& getChannelStrip(string label); 
    
    int getNumChannels();
    
    float getChannelPan(int channelNumber);
    void  setChannelPan(int channelNumber, float pan);
    
    float getChannelPan(string label);
    void  setChannelPan(string label, float pan);
    
    void setMasterVolume(float vol);
    float getMasterVolume();
    
    float getMasterPan();
    void setMasterPan(float pan);
    
    //void setChannelStripClass(ofSoundMixerChannelStrip *chnlStrip);
    int getChannelNumByLabel(string label);
    string getChannelLabel(int channelNumber);
    bool channelExists(int channelNumber);
    bool channelExists(string label);
    
     void audioOut(ofSoundBuffer &output);
    
protected:
    vector<ofSoundMixerChannelStrip>channels;
    float masterPan;
    float masterVolume;
    //void process();

};



#endif
