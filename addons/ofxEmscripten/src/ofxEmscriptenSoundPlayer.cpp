/*
 * ofxEmscriptenSoundPlayer.cpp
 *
 *  Created on: May 15, 2014
 *      Author: arturo
 */

#include "ofxEmscriptenSoundPlayer.h"
#include "html5audio.h"

using namespace std;

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
,volume(0.5)
,playing(false){

}

ofxEmscriptenSoundPlayer::~ofxEmscriptenSoundPlayer(){
	sound = -1;
	html5audio_sound_free(sound);
}

bool ofxEmscriptenSoundPlayer::load(const std::filesystem::path& fileName, bool stream){
	sound = html5audio_sound_load(ofToDataPath(fileName).c_str());
	return sound != -1;
}

bool ofxEmscriptenSoundPlayer::load(const std::string& fileName, bool stream){
	sound = html5audio_sound_load(fileName.c_str());
	return sound != -1;
}

void ofxEmscriptenSoundPlayer::unload(){
	if(sound != -1){
		html5audio_sound_free(sound);
		sound = -1;
	}
}

void ofxEmscriptenSoundPlayer::play(){
	if(sound != -1){
		if(playing && !multiplay && !html5audio_sound_done(sound)){
			html5audio_sound_stop(sound);
		}
		html5audio_sound_play(sound, 0);
		html5audio_sound_set_rate(sound,speed);
		html5audio_sound_set_volume(sound,volume);
		playing = true;
	}
}

void ofxEmscriptenSoundPlayer::stop(){
	if(sound != -1){
		html5audio_sound_stop(sound);
	}
	playing = false;
}


void ofxEmscriptenSoundPlayer::setVolume(float vol){
	volume = vol;
	if(sound != -1){
		html5audio_sound_set_volume(sound,vol);
	}
}

void ofxEmscriptenSoundPlayer::setPan(float pan){
	if(sound != -1){
		html5audio_sound_set_pan(sound, pan);
	}
}

void ofxEmscriptenSoundPlayer::setSpeed(float spd){
	if(sound != -1){
		html5audio_sound_set_rate(sound,spd);
	}
}

void ofxEmscriptenSoundPlayer::setPaused(bool bP){
	if(sound != -1){
		if(bP) html5audio_sound_pause(sound);
		else html5audio_sound_play(sound, 0);
	}
}

void ofxEmscriptenSoundPlayer::setLoop(bool bLp){
	if(sound != -1){
		html5audio_sound_set_loop(sound, bLp);
	}
}

void ofxEmscriptenSoundPlayer::setMultiPlay(bool bMp){
	multiplay = bMp;
}

void ofxEmscriptenSoundPlayer::setPosition(float pct){
	if(sound != -1){
		html5audio_sound_set_position(sound, pct);
	}
}

void ofxEmscriptenSoundPlayer::setPositionMS(int ms){
	if(sound != -1){
		html5audio_sound_set_position(sound, ms / html5audio_sound_duration(sound) / 1000);
	}
}

float ofxEmscriptenSoundPlayer::getPosition() const{
	if(sound != -1){
		return html5audio_sound_position(sound);
	}else{
		return 0;
	}
}

int ofxEmscriptenSoundPlayer::getPositionMS() const{
	if(sound != -1){
		return html5audio_sound_position(sound) * html5audio_sound_duration(sound) * 1000;
	}else{
		return 0;
	}
}

bool ofxEmscriptenSoundPlayer::isPlaying() const{
	return playing && !html5audio_sound_done(sound);
}

float ofxEmscriptenSoundPlayer::getSpeed() const{
	if(sound != -1){
		return html5audio_sound_rate(sound);
	}else{
		return 0;
	}
}

float ofxEmscriptenSoundPlayer::getPan() const{
	if(sound != -1){
		return html5audio_sound_pan(sound);
	}else{
		return 0;
	}
}

bool ofxEmscriptenSoundPlayer::isLoaded() const{
	return html5audio_sound_is_loaded(sound);
}

float ofxEmscriptenSoundPlayer::getVolume() const{
	if(sound != -1){
		return html5audio_sound_volume(sound);
	}else{
		return 0;
	}
}

int ofxEmscriptenSoundPlayer::getDurationMS() const{
	if(sound != -1){
		return html5audio_sound_duration(sound) * 1000;
	}else{
		return 0;
	}
}

double ofxEmscriptenSoundPlayer::getDurationSecs() const{
	if(sound != -1){
		return html5audio_sound_duration(sound);
	}else{
		return 0;
	}
}

float * ofxEmscriptenSoundPlayer::getSystemSpectrum(int bands){
	systemSpectrum.resize(bands);
	html5audio_context_spectrum(bands, &systemSpectrum[0]);
	for(size_t i = 0; i < systemSpectrum.size(); i++){
		systemSpectrum[i] = (systemSpectrum[i]+100) * 0.01;
	}
	return &systemSpectrum[0];
}
