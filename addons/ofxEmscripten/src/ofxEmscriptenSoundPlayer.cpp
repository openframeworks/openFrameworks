/*
 * ofxEmscriptenSoundPlayer.cpp
 *
 *  Created on: May 15, 2014
 *      Author: arturo
 */

#include "ofxEmscriptenSoundPlayer.h"
#include "html5audio.h"
#include "ofUtils.h"

vector<float> ofxEmscriptenSoundPlayer::systemSpectrum;

int ofxEmscriptenAudioContext(){
	static bool initialized=false;
	static int context = -1;
	if(!initialized){
		context = html5audio_context_create();
		initialized = true;
	}
	return context;
}

ofxEmscriptenSoundPlayer::ofxEmscriptenSoundPlayer()
:context(ofxEmscriptenAudioContext())
,sound(-1)
,multiplay(false)
,speed(1)
,volume(0)
,playing(false){

}

ofxEmscriptenSoundPlayer::~ofxEmscriptenSoundPlayer(){
	sound = -1;
	html5audio_sound_free(sound);
}


bool ofxEmscriptenSoundPlayer::load(string fileName, bool stream){
	if(context!=-1){
		sound = html5audio_sound_load(context,ofToDataPath(fileName).c_str());
	}
	return sound!=-1;
}

void ofxEmscriptenSoundPlayer::unload(){
	html5audio_sound_free(sound);
	sound = -1;
}

void ofxEmscriptenSoundPlayer::play(){
	if(sound!=-1){
		if(playing && !multiplay && !html5audio_sound_done(sound)){
			html5audio_sound_stop(sound);
		}
		html5audio_sound_play(context,sound,0);
		html5audio_sound_set_rate(sound,speed);
		html5audio_sound_set_gain(sound,volume);
		playing = true;
	}
}

void ofxEmscriptenSoundPlayer::stop(){
	if(sound!=-1){
		html5audio_sound_stop(sound);
	}
	playing = false;
}


void ofxEmscriptenSoundPlayer::setVolume(float vol){
	volume = vol;
	if(sound!=-1){
		html5audio_sound_set_gain(sound,vol);
	}
}

void ofxEmscriptenSoundPlayer::setPan(float vol){

}

void ofxEmscriptenSoundPlayer::setSpeed(float spd){
	speed = spd;
	if(sound!=-1){
		html5audio_sound_set_rate(sound,spd);
	}
}

void ofxEmscriptenSoundPlayer::setPaused(bool bP){
	if(sound!=-1){
		if(bP) html5audio_sound_pause(sound);
		else html5audio_sound_play(context,sound,0);
	}
}

void ofxEmscriptenSoundPlayer::setLoop(bool bLp){
	if(sound!=-1){
		html5audio_sound_set_loop(sound,bLp);
	}
}

void ofxEmscriptenSoundPlayer::setMultiPlay(bool bMp){
	multiplay = bMp;
}

void ofxEmscriptenSoundPlayer::setPosition(float pct){
	if(sound!=-1){
		double duration = html5audio_sound_duration(sound);
		double position = duration * pct;
		setPositionSecs(position);
	}

}

void ofxEmscriptenSoundPlayer::setPositionMS(int ms){
	setPositionSecs(ms/1000.0);
}

void ofxEmscriptenSoundPlayer::setPositionSecs(double s){
	if(sound!=-1){
		if(playing && !multiplay && !html5audio_sound_done(sound)){
			html5audio_sound_stop(sound);
		}
		html5audio_sound_play(context,sound,s);
		html5audio_sound_set_rate(sound,speed);
		playing = true;
	}

}


float ofxEmscriptenSoundPlayer::getPosition() const{
	double duration = getDurationSecs() / speed;
	if(duration>0){
		return html5audio_sound_position(sound)/duration;
	}else{
		return 0;
	}
}

int ofxEmscriptenSoundPlayer::getPositionMS() const{
	if(sound!=-1){
		return html5audio_sound_position(sound)*1000;
	}else{
		return 0;
	}
}

bool ofxEmscriptenSoundPlayer::isPlaying() const{
	return playing && !html5audio_sound_done(sound);
}

float ofxEmscriptenSoundPlayer::getSpeed() const{
	return speed;
}

float ofxEmscriptenSoundPlayer::getPan() const{
	return 0;
}

bool ofxEmscriptenSoundPlayer::isLoaded() const{
	return sound!=-1;
}

float ofxEmscriptenSoundPlayer::getVolume() const{
	return volume;
}

int ofxEmscriptenSoundPlayer::getDurationMS() const{
	if(sound!=-1){
		return html5audio_sound_duration(sound) * 1000;
	}else{
		return 0;
	}
}

double ofxEmscriptenSoundPlayer::getDurationSecs() const{
	if(sound!=-1){
		return html5audio_sound_duration(sound);
	}else{
		return 0;
	}
}

float * ofxEmscriptenSoundPlayer::getSystemSpectrum(int bands){
	systemSpectrum.resize(bands);
	html5audio_context_spectrum(ofxEmscriptenAudioContext(),bands,&systemSpectrum[0]);
	for(size_t i=0;i<systemSpectrum.size();i++){
		systemSpectrum[i] = (systemSpectrum[i]+100)*0.01;
	}
	return &systemSpectrum[0];
}
