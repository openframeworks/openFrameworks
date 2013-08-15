

#include "ofSoundMixer.h"
//ofSoundMixer::ofSoundMixer(){}
//ofSoundMixer::~ofSoundMixer(){}
//void connectTo(ofBaseSoundOutput &output);
//void disconnect();
ofSoundMixer systemSoundMixer;
ofSoundMixer & ofGetSystemSoundMixer(){
    return systemSoundMixer;
}
ofBaseSoundOutput& ofSoundMixer::getChannelSource(int channelNumber){
    if (channelNumber <channels.size()) {
        return *channels[channelNumber];
    }else{
        //return NULL;
    }
}
void ofSoundMixer::disconnectInput(ofSoundObject * input){
    for (int i =0; i<channels.size(); i++) {
        if (input == channels[i]) {
            channels.erase(channels.begin() + i);
            break;
        }
    }
}

void ofSoundMixer::setInput(ofSoundObject *obj){
    for (int i =0; i<channels.size(); i++) {
        if (obj == channels[i]) {
            ofLogNotice("ofSoundMixer::setInput") << " already connected" << endl;
            return;
        }
    }
    channels.push_back(obj);
}
int ofSoundMixer::getNumChannels(){
    return channels.size();
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
bool ofSoundMixer::isConnectedTo(ofSoundObject& obj){
    for (int i =0; i<channels.size(); i++) {
        if (&obj == channels[i]) {
            return true;
        }
    }
    return false;
}

// this pulls the audio through from earlier links in the chain
void ofSoundMixer::audioOut(ofSoundBuffer &output) {
	if(channels.size()>0) {
        for(int i = 0; i < channels.size(); i++){
            if (channels[i] != NULL) {
                ofSoundBuffer tempBuffer;
                tempBuffer.resize(output.size());
                tempBuffer.setNumChannels(output.getNumChannels());
                tempBuffer.setSampleRate(output.getSampleRate());
                channels[i]->audioOut(tempBuffer);
                
                for (int i = 0; i < tempBuffer.size(); i++) {
                    output.getBuffer()[i] += tempBuffer.getBuffer()[i];
                } // same as -> tempBuffer.addTo(output);
            }
        }
        output.stereoPan(1-masterPan, masterPan);
        output*=masterVolume;   
	}
}
