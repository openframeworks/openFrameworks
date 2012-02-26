#pragma once
#include "ofMain.h"

#include "Bug.h"
#include "Bullet.h"
#include "GameEvent.h"

class testApp : public ofBaseApp{
    
public:

    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mousePressed(int x, int y, int button);
    
    static bool shouldRemoveBullet(Bullet &b);
    static bool shouldRemoveBug(Bug &b);
    
    void gameEvent(GameEvent &e);
    
    vector <Bullet>  bullets;
    vector <Bug>     bugs;
    vector <ofVec2f> holes;
    
    int              maxBullets;
    int              bugsKilled;
    bool             bFire;
};
