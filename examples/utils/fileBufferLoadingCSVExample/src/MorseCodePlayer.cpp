/*
 *  MorseCodePlayer.cpp
 *
 *  Created by Jason Van Cleave on 2/22/12.
 *
 */

#include "MorseCodePlayer.h"


MorseCodePlayer::MorseCodePlayer(){
	currentSoundIndex = 0;
	isReady = false;
}

void MorseCodePlayer::setup(){
	dotPlayer.loadSound("dot.wav", false);
	dashPlayer.loadSound("dash.wav", false);
	//ofLogVerbose("dotPlayer duration: " + ofToString(dotPlayer.length/dotPlayer.internalFreq));
	//ofLogVerbose("dashPlayer duration: " + ofToString(dashPlayer.length/dashPlayer.internalFreq));
	isReady = true;
}

void MorseCodePlayer::update(){
	if (!dotPlayer.getIsPlaying() && !dashPlayer.getIsPlaying()) {
		if (codes.size()>0){
			
			currentCode = codes[0];
			codes.erase(codes.begin());
			
			if (currentCode == "."){
				dotPlayer.play();
			}
			if (currentCode == "-"){
				dashPlayer.play();
			}
		}
	}
	
	
}

void MorseCodePlayer::playCode(string morseCode){
	isReady = false;
	codes.clear();
	for (int i =0; i<morseCode.size(); i++){
		codes.push_back(morseCode[i]);		
	}
	currentSoundIndex = 0;
	isReady = true;
	
}

