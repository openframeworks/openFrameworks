/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 

#import "ofSoundPlayer.h"
void ofSoundStopAll(){
}

//--------------------
void ofSoundSetVolume(float vol){
}

//--------------------
float * ofSoundGetSpectrum(int nBands){
	return NULL;
}

// ------------------------------------------------------------
// ------------------------------------------------------------


// now, the individual sound player:
//------------------------------------------------------------
ofSoundPlayer::ofSoundPlayer(){
}

//ofSoundPlayer::~ofSoundPlayer(){
//}

#ifdef OF_SOUND_PLAYER_FMOD
//---------------------------------------
// this should only be called once
void ofSoundPlayer::initializeFmod(){
}


//---------------------------------------
void ofSoundPlayer::closeFmod(){
}
#endif

//------------------------------------------------------------
void ofSoundPlayer::loadSound(string fileName, bool stream){
}

//------------------------------------------------------------
void ofSoundPlayer::unloadSound(){
}

//------------------------------------------------------------
bool ofSoundPlayer::getIsPlaying(){
	return false;
}

//------------------------------------------------------------
float ofSoundPlayer::getSpeed(){
	return 0;
}

//------------------------------------------------------------
float ofSoundPlayer::getPan(){
	return 0;
}

//------------------------------------------------------------
float ofSoundPlayer::getVolume(){
}

//------------------------------------------------------------
void ofSoundPlayer::setVolume(float vol){
}

//------------------------------------------------------------
void ofSoundPlayer::setPosition(float pct){
}

//------------------------------------------------------------
float ofSoundPlayer::getPosition(){
	return 0;
}

//------------------------------------------------------------
void ofSoundPlayer::setPan(float p){
}


//------------------------------------------------------------
void ofSoundPlayer::setPaused(bool bP){
}


//------------------------------------------------------------
void ofSoundPlayer::setSpeed(float spd){
}


//------------------------------------------------------------
void ofSoundPlayer::setLoop(bool bLp){
}

// ----------------------------------------------------------------------------
void ofSoundPlayer::setMultiPlay(bool bMp){
}

// ----------------------------------------------------------------------------
void ofSoundPlayer::play(){
}

// ----------------------------------------------------------------------------
void ofSoundPlayer::stop(){
}
