#pragma once
#include "Bug.h"
#include "Bullet.h"
#include "ofMain.h"

class GameEvent : public ofEventArgs {

public:
	Bullet * bullet;
	Bug * bug;
	string message;

	GameEvent() {
		bullet = NULL;
		bug = NULL;
	}

	static ofEvent<GameEvent> events;
};
