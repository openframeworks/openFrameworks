/*
 * ofxEmscriptenSoundPlayer.cpp
 *
 *  Created on: May 15, 2014
 *      Author: arturo
 */

#include "ofFileUtils.h"
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
,multiplay(false)
,volume(0.5)
,speed(1)
,pan(0)
,playing(false)
,player_id(html5audio_player_create()){
}

ofxEmscriptenSoundPlayer::~ofxEmscriptenSoundPlayer(){
	html5audio_sound_free(player_id);
}

bool ofxEmscriptenSoundPlayer::load(const std::filesystem::path& fileName, bool stream){
	html5audio_sound_load(player_id, ofToDataPath(fileName).c_str());
	return true;
}

bool ofxEmscriptenSoundPlayer::load(const std::string& fileName, bool stream){
	html5audio_sound_load(player_id, fileName.c_str());
	return true;
}

void ofxEmscriptenSoundPlayer::unload(){
	html5audio_sound_free(player_id);
}

void ofxEmscriptenSoundPlayer::play(){
	if(playing && !multiplay && !html5audio_sound_done(player_id)){
		html5audio_sound_stop(player_id);
	}
	html5audio_sound_play(player_id, multiplay, volume, speed, pan, 0);
	html5audio_sound_set_rate(player_id, speed);
	html5audio_sound_set_volume(player_id, volume);
	playing = true;
}

void ofxEmscriptenSoundPlayer::stop(){
	html5audio_sound_stop(player_id);
	playing = false;
}


void ofxEmscriptenSoundPlayer::setVolume(float vol){
	volume = vol;
	html5audio_sound_set_volume(player_id, vol);
}

void ofxEmscriptenSoundPlayer::setPan(float panorama){
	pan = panorama;
	html5audio_sound_set_pan(player_id, pan);
}

void ofxEmscriptenSoundPlayer::setSpeed(float spd){
	speed = spd;
	html5audio_sound_set_rate(player_id, spd);
}

void ofxEmscriptenSoundPlayer::setPaused(bool bP){
	if(bP) html5audio_sound_pause(player_id);
	else html5audio_sound_play(player_id, multiplay, volume, speed, pan, 0);
}

void ofxEmscriptenSoundPlayer::setLoop(bool bLp){
	html5audio_sound_set_loop(player_id, bLp);
}

void ofxEmscriptenSoundPlayer::setMultiPlay(bool bMp){
	multiplay = bMp;
}

void ofxEmscriptenSoundPlayer::setPosition(float pct){
	html5audio_sound_set_position(player_id, pct);
}

void ofxEmscriptenSoundPlayer::setPositionMS(int ms){
	html5audio_sound_set_position(player_id, ms / html5audio_sound_duration(player_id) / 1000);
}

float ofxEmscriptenSoundPlayer::getPosition() const{
	return html5audio_sound_position(player_id);
}

int ofxEmscriptenSoundPlayer::getPositionMS() const{
	return html5audio_sound_position(player_id) * html5audio_sound_duration(player_id) * 1000;
}

bool ofxEmscriptenSoundPlayer::isPlaying() const{
	return playing && !html5audio_sound_done(player_id);
}

float ofxEmscriptenSoundPlayer::getSpeed() const{
	return html5audio_sound_rate(player_id);
}

float ofxEmscriptenSoundPlayer::getPan() const{
	return html5audio_sound_pan(player_id);
}

bool ofxEmscriptenSoundPlayer::isLoaded() const{
	return html5audio_sound_is_loaded(player_id);
}

float ofxEmscriptenSoundPlayer::getVolume() const{
	return html5audio_sound_volume(player_id);
}

int ofxEmscriptenSoundPlayer::getDurationMS() const{
	return html5audio_sound_duration(player_id) * 1000;
}

double ofxEmscriptenSoundPlayer::getDurationSecs() const{
	return html5audio_sound_duration(player_id);
}

float * ofxEmscriptenSoundPlayer::getSystemSpectrum(int bands){
	systemSpectrum.resize(bands);
	html5audio_context_spectrum(bands, &systemSpectrum[0]);
	for(size_t i = 0; i < systemSpectrum.size(); i++){
		systemSpectrum[i] = (systemSpectrum[i]+100) * 0.01;
	}
	return &systemSpectrum[0];
}
