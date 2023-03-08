//
//  ofxiOSSoundPlayer.cpp
//  Created by Lukasz Karluk on 14/11/12.
//  http://julapy.com/blog
//

#include "ofxiOSSoundPlayer.h"
#include "ofUtils.h"
#include "ofLog.h"
#import "AVSoundPlayer.h"

using std::string;

ofxiOSSoundPlayer::ofxiOSSoundPlayer() {
    soundPlayer = NULL;
}

ofxiOSSoundPlayer::~ofxiOSSoundPlayer() {
    unload();
}

bool ofxiOSSoundPlayer::load(const of::filesystem::path& fileName, bool stream) {
    if(soundPlayer != NULL) {
        unload();
    }

    string filePath = ofToDataPath(fileName);
    __autoreleasing AVSoundPlayer *player = [[AVSoundPlayer alloc] init];
    BOOL bOk = [player loadWithPath:[NSString stringWithUTF8String:filePath.c_str()]];
    if(bOk) {
        soundPlayer = (__bridge_retained void *)player;
    }

    return bOk;
}

void ofxiOSSoundPlayer::unload() {
    if(soundPlayer != NULL) {
        __autoreleasing AVSoundPlayer *player = (__bridge_transfer AVSoundPlayer *)soundPlayer;
        [player unloadSound];
        soundPlayer = NULL;
    }
}

void ofxiOSSoundPlayer::play() {
    if(soundPlayer == NULL) {
        return;
    }
    [(__bridge AVSoundPlayer *)soundPlayer play];
}

void ofxiOSSoundPlayer::stop() {
    if(soundPlayer == NULL) {
        return;
    }
    [(__bridge AVSoundPlayer *)soundPlayer stop];
}

void ofxiOSSoundPlayer::setVolume(float value) {
    if(soundPlayer == NULL) {
        return;
    }
    [(__bridge AVSoundPlayer *)soundPlayer volume:value];
}

void ofxiOSSoundPlayer::setPan(float value) {
    if(soundPlayer == NULL) {
        return;
    }
    [(__bridge AVSoundPlayer *)soundPlayer pan:value];
}

void ofxiOSSoundPlayer::setSpeed(float value) {
    if(soundPlayer == NULL) {
        return;
    }
    [(__bridge AVSoundPlayer *)soundPlayer speed:value];
}

void ofxiOSSoundPlayer::setPaused(bool bPause) {
    if(soundPlayer == NULL) {
        return;
    }
    if(bPause) {
        [(__bridge AVSoundPlayer *)soundPlayer pause];
    } else {
        [(__bridge AVSoundPlayer *)soundPlayer play];
    }
}

void ofxiOSSoundPlayer::setLoop(bool bLoop) {
    if(soundPlayer == NULL) {
        return;
    }
    [(__bridge AVSoundPlayer *)soundPlayer loop:bLoop];
}

void ofxiOSSoundPlayer::setMultiPlay(bool bMultiPlay) {
    ofLogNotice("ofxiOSSoundPlayer") << "setMultiPlay(): sorry, no support for multiplay streams";
    if(soundPlayer == NULL) {
        return;
    }
    [(__bridge AVSoundPlayer *)soundPlayer multiPlay:bMultiPlay];
}

void ofxiOSSoundPlayer::setPosition(float position) {
    if(soundPlayer == NULL) {
        return;
    }
    [(__bridge AVSoundPlayer *)soundPlayer position:position];
}

void ofxiOSSoundPlayer::setPositionMS(int positionMS) {
    if(soundPlayer == NULL) {
        return;
    }
    [(__bridge AVSoundPlayer *)soundPlayer positionMs:positionMS];
}

float ofxiOSSoundPlayer::getPosition()  const{
    if(soundPlayer == NULL) {
        return 0;
    }
    return [(__bridge AVSoundPlayer *)soundPlayer position];
}

int ofxiOSSoundPlayer::getPositionMS() const {
    if(soundPlayer == NULL) {
        return 0;
    }
    return [(__bridge AVSoundPlayer *)soundPlayer positionMs];
}

bool ofxiOSSoundPlayer::isPlaying()  const{
    if(soundPlayer == NULL) {
        return false;
    }
    return [(__bridge AVSoundPlayer *)soundPlayer isPlaying];
}

float ofxiOSSoundPlayer::getSpeed()  const{
    if(soundPlayer == NULL) {
        return 0;
    }
    return [(__bridge AVSoundPlayer *)soundPlayer speed];
}

float ofxiOSSoundPlayer::getPan()  const{
    if(soundPlayer == NULL) {
        return 0;
    }
    return [(__bridge AVSoundPlayer *)soundPlayer pan];
}

bool ofxiOSSoundPlayer::isLoaded()  const{
    if(soundPlayer == NULL) {
        return false;
    }
    return [(__bridge AVSoundPlayer *)soundPlayer isLoaded];
}

float ofxiOSSoundPlayer::getVolume()  const{
    if(soundPlayer == NULL) {
        return false;
    }
    return [(__bridge AVSoundPlayer *)soundPlayer volume];
}

void * ofxiOSSoundPlayer::getAVSoundPlayer() {
    return soundPlayer;
}
