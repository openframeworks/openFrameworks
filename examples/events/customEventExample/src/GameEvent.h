#pragma once
#include "ofMain.h"
#include "Bug.h"
#include "Bullet.h"

class GameEvent : public ofEventArgs {
    
public:

    Bullet * bullet;
    Bug    * bug;
    string   message;
    
    GameEvent() {
        bullet = NULL;
        bug    = NULL;
    }
    
    static ofEvent <GameEvent> events;
};

