
//
//  ofSoundMixer.cpp
//  openFrameworksLib
//
//  Created by Roy Macdonald on 8/11/13.
//  Of dev con '13 YCAM
//

#include "ofSoundMixer.h"
ofSoundMixer::ofSoundMixer(){}
ofSoundMixer::~ofSoundMixer(){}
//void connectTo(ofBaseSoundOutput &output);
//void disconnect();
ofSoundMixerChannelStrip& ofSoundMixer::add(ofBaseSoundOutput &newSoundSource, string label){
    newChannel(label);
    channels.back().source = &newSoundSource;
    return channels.back();
}
ofSoundMixerChannelStrip& ofSoundMixer::newChannel(string label){
    string newLabel = label;
    if (channelExists(label)) {
        int n = 1;
        while (channelExists(label + ofToString(n))) {
            n++;
        }
        newLabel = label + ofToString(n);
    }
    ofLogNotice() << "ofMixer::newChannel " << newLabel;    
    channels.push_back(ofSoundMixerChannelStrip(newLabel));
    return channels.back();
}
ofBaseSoundOutput& ofSoundMixer::getChannelStripSource(int channelNumber, bool createNewUnfound){
    if (channelExists(channelNumber)) {
        return *channels[channelNumber].source;
    }else{
        return NULL;
    }
}

ofBaseSoundOutput& ofSoundMixer::getChannelStripSource(string label, bool createNewUnfound){
    int n = getChannelNumByLabel(label);
    if (n >- 1) {
        return *channels[n].source;
    }else{
        if (createNewUnfound) {
            return *newChannel(label).source;
        }else{
            return NULL;
        }
    }
}

ofSoundMixerChannelStrip& ofSoundMixer::getChannelStrip(int channelNumber){
    if (channelExists(channelNumber)) {
        return channels[channelNumber];
    }else{
        return NULL;
    }
}
ofSoundMixerChannelStrip& ofSoundMixer::getChannelStrip(string label){
    int n = getChannelNumByLabel(label);
    if (n >- 1) {
        return channels[n];
    }else{
        return NULL;
    }
}

int ofSoundMixer::getNumChannels(){
    return channels.size();
}
//volume range 0.0 to 1.0
float ofSoundMixer::getChannelVolume(int channelNumber){
    if (channelExists(channelNumber)) {
        return channels[channelNumber].vol;
    }else{
        return -1;
    }    
}
float ofSoundMixer::getChannelVolume(string label){
    int n = getChannelNumByLabel(label);
    if (n >- 1) {
        return channels[n].vol;
    }else{
        return -1;
    }
}

void ofSoundMixer::setChannelVolume(int channelNumber, float volume){
    if (channelExists(channelNumber)) {
        channels[channelNumber].vol =volume;
    }
}
void ofSoundMixer::setChannelVolume(string label, float volume){
    int n = getChannelNumByLabel(label);
    if (n >- 1) {
        channels[n].vol = volume;
    }
}

float ofSoundMixer::getChannelPan(int channelNumber){
    if (channelExists(channelNumber)) {
       return channels[channelNumber].pan;
    }else{
        return -1;
    }
}
void  ofSoundMixer::setChannelPan(int channelNumber, float pan){
    if (channelExists(channelNumber)) {
        channels[channelNumber].pan =pan;
    }
}

float ofSoundMixer::getChannelPan(string label){
    int n = getChannelNumByLabel(label);
    if (n >- 1) {
        return channels[n].pan;
    }else{
        return -1;
    }
}
void  ofSoundMixer::setChannelPan(string label, float pan){
    int n = getChannelNumByLabel(label);
    if (n >- 1) {
        channels[n].pan = pan;
    }
}

void ofSoundMixer::setMasterVolume(float vol){
    masterVolume = vol;
}
float ofSoundMixer::getMasterVolume(){
    return masterVolume;
}

float ofSoundMixer::getMasterPan(){
    return masterPan;
}
void ofSoundMixer::setMasterPan(float pan){
    masterPan = pan;
}
int ofSoundMixer::getChannelNumByLabel(string label){
    for (int i = 0; i < channels.size(); i++){
        if (channels[i].label == label) {
            return i;
        }
    }
    ofLogWarning() << "ofSoundMixer: The requested channel " + label + " doesn't exist!!";
    return -1;
}
string ofSoundMixer::getChannelLabel(int channelNumber){
    if (channelExists(channelNumber)) {
        return channels[channelNumber].label;
    }else{
        return "";
    }
}
bool ofSoundMixer::channelExists(int channelNumber){
    if (channelNumber < channels.size()) {
        return true;
    }else{
         ofLogWarning() << "ofSoundMixer: The requested channel " + ofToString(channelNumber) + " doesn't exist!!"; 
        return false;
    }
}
bool ofSoundMixer::channelExists(string label){
    for (int i = 0; i < channels.size(); i++){
        if (channels[i].label == label) {
            return true;
        }
    }
    ofLogWarning() << "ofSoundMixer: The requested channel " + label + " doesn't exist!!";    
    return false;
}


