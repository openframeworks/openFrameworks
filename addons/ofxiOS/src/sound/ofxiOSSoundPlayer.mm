//
//  ofxiOSSoundPlayer.cpp
//  Created by Lukasz Karluk on 14/11/12.
//  http://julapy.com/blog
//

#include "ofxiOSSoundPlayer.h"
#include "ofUtils.h"
#include "AVSoundPlayer.h"
AVSoundPlayer* soundPlayer;

ofxiOSSoundPlayer::ofxiOSSoundPlayer() {
    soundPlayer = NULL;
}

ofxiOSSoundPlayer::~ofxiOSSoundPlayer() {
    unload();
}

bool ofxiOSSoundPlayer::load(std::filesystem::path fileName, bool stream) {
    if(soundPlayer != NULL) {
        unload();
    }

    string filePath = ofToDataPath(fileName);
    soundPlayer = [[AVSoundPlayer alloc] init];
    BOOL bOk = [soundPlayer loadWithPath:[NSString stringWithUTF8String:filePath.c_str()]];
    
    return bOk;
}

void ofxiOSSoundPlayer::unload() {
    if(soundPlayer != NULL) {
        [soundPlayer unloadSound];
        soundPlayer = NULL;
    }
}

void ofxiOSSoundPlayer::play() {
    if(soundPlayer == NULL) {
        return;
    }
    [soundPlayer play];
}

void ofxiOSSoundPlayer::stop() {
    if(soundPlayer == NULL) {
        return;
    }
    [soundPlayer stop];
}

void ofxiOSSoundPlayer::setVolume(float value) {
    if(soundPlayer == NULL) {
        return;
    }
    [soundPlayer volume:value];
}

void ofxiOSSoundPlayer::setPan(float value) {
    if(soundPlayer == NULL) {
        return;
    }
    [soundPlayer pan:value];
}

void ofxiOSSoundPlayer::setSpeed(float value) {
    if(soundPlayer == NULL) {
        return;
    }
    [soundPlayer speed:value];
}

void ofxiOSSoundPlayer::setPaused(bool bPause) {
    if(soundPlayer == NULL) {
        return;
    }
    if(bPause) {
        [soundPlayer pause];
    } else {
        [soundPlayer play];
    }
}

void ofxiOSSoundPlayer::setLoop(bool bLoop) {
    if(soundPlayer == NULL) {
        return;
    }
    [soundPlayer loop:bLoop];
}

void ofxiOSSoundPlayer::setMultiPlay(bool bMultiPlay) {
    ofLogNotice("ofxiOSSoundPlayer") << "setMultiPlay(): sorry, no support for multiplay streams";
    if(soundPlayer == NULL) {
        return;
    }
    [soundPlayer multiPlay:bMultiPlay];
}

void ofxiOSSoundPlayer::setPosition(float position) {
    if(soundPlayer == NULL) {
        return;
    }
    [soundPlayer position:position];
}

void ofxiOSSoundPlayer::setPositionMS(int positionMS) {
    if(soundPlayer == NULL) {
        return;
    }
    [soundPlayer positionMs:positionMS];
}

float ofxiOSSoundPlayer::getPosition()  const{
    if(soundPlayer == NULL) {
        return 0;
    }
    return [soundPlayer position];
}

int ofxiOSSoundPlayer::getPositionMS() const {
    if(soundPlayer == NULL) {
        return 0;
    }
    return [soundPlayer positionMs];
}

bool ofxiOSSoundPlayer::isPlaying()  const{
    if(soundPlayer == NULL) {
        return false;
    }
    return [soundPlayer isPlaying];
}

float ofxiOSSoundPlayer::getSpeed()  const{
    if(soundPlayer == NULL) {
        return 0;
    }
    return [soundPlayer speed];
}

float ofxiOSSoundPlayer::getPan()  const{
    if(soundPlayer == NULL) {
        return 0;
    }
    return [soundPlayer pan];
}

bool ofxiOSSoundPlayer::isLoaded()  const{
    if(soundPlayer == NULL) {
        return false;
    }
    return [soundPlayer isLoaded];
}

float ofxiOSSoundPlayer::getVolume()  const{
    if(soundPlayer == NULL) {
        return false;
    }
    return [soundPlayer volume];
}

