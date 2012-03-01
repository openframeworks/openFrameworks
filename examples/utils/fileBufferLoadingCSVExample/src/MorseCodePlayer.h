/*
 *  MorseCodePlayer.h
 *
 *  Created by Jason Van Cleave on 2/22/12.
 *
 */

#include "ofMain.h"

struct MorseCodeSymbol {
	string character;
	string code;
};


class MorseCodePlayer {
public:
	MorseCodePlayer();
	void setup();
	void update();
	void playCode(string morseCode);
	
	
	ofFmodSoundPlayer dotPlayer;
	ofFmodSoundPlayer dashPlayer;
	
	int currentSoundIndex;
	
	vector<char> codes;
	string currentCode;
	
	bool isReady;
};
