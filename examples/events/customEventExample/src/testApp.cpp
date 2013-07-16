#include "testApp.h"


/*
 
 This example demonstrates a simple game. We have a GameEvent class that is 
 used to store the bullet and bug that just collided. 
 
 
 Topics:
 ofEventArgs
 ofAddListener
 ofRemove
 vector math
 objects & classes
 
 by: Todd Vanderlin
 
 */

//--------------------------------------------------------------
bool testApp::shouldRemoveBullet(Bullet &b) {
    
    if(b.bRemove) return true;
    
    
    bool bRemove = false;
    
    // get the rectangle of the OF world
    ofRectangle rec = ofGetCurrentViewport();
    
    // check if the bullet is inside the world
    if(rec.inside(b.pos) == false) {
        bRemove = true;
    }
    
    
    
    return bRemove;
}

//--------------------------------------------------------------
bool testApp::shouldRemoveBug(Bug &b) {
    return b.bRemove;
}

//--------------------------------------------------------------
void testApp::setup() {
    
    ofBackgroundHex(0xc5c9b2);
    ofSetFrameRate(60);
    
    bFire = false;
    bugsKilled = 0;
    maxBullets = 30;
    
    // add some random holes for the bugs to come out
    int nHoldes = 10;
    for(int i=0; i<nHoldes; i++) {
        ofVec2f p(ofRandomWidth(), ofRandomHeight());
        holes.push_back(p);
    }
    
    // listen to any of the events for the game
    ofAddListener(GameEvent::events, this, &testApp::gameEvent);
    
}

//--------------------------------------------------------------
void testApp::gameEvent(GameEvent &e) {
    
    cout << "Game Event: "+e.message << endl;
    e.bug->timeBugKilled = ofGetElapsedTimef();
    e.bug->bSquashed = true;
    
    e.bullet->bRemove = true;
    
    bugsKilled ++;
}

//--------------------------------------------------------------
void testApp::update() {
    
    if((int)ofRandom(0, 20)==10) {
        
        int randomHole = ofRandom(holes.size());
        
        Bug newBug;
        newBug.pos = holes[randomHole];
        newBug.vel.set(ofRandom(-1, 1), ofRandom(-1, 1));
        bugs.push_back(newBug);
    }
    
    for(unsigned int i=0; i<bugs.size(); i++) {
        
        bugs[i].update();
        
        // bug pos and size
        float   size = bugs[i].size;
        ofVec2f pos  = bugs[i].pos;
        
        // wrap the bugs around the screen
        if(pos.x > ofGetWidth()-size)  bugs[i].pos.x = -size;
        if(pos.x < -size)              bugs[i].pos.x = ofGetWidth()-size;
        if(pos.y > ofGetHeight()+size) bugs[i].pos.y = -size;
        if(pos.y < -size)              bugs[i].pos.y = ofGetHeight()-size;
        
    } 
    
    // check if we should remove any bugs
    ofRemove(bugs, shouldRemoveBug);
    
    // update the bullets
    for(unsigned int i=0; i<bullets.size(); i++) {
        bullets[i].update();
    } 
    
    // check if we want to remove the bullet
    ofRemove(bullets, shouldRemoveBullet);
    
    
    // did we hit a bug loop we are checking to see if a bullet
    // hits a bug. if so we are going to launch an event for the game
    for(unsigned int i=0; i<bullets.size(); i++) {
        for(unsigned int j=0; j<bugs.size(); j++) {
            
            ofVec2f a       = bullets[i].pos;
            ofVec2f b       = bugs[j].pos;
            float   minSize = bugs[j].size;
            
            if(a.distance(b) < minSize) {
                
                static GameEvent newEvent;
                newEvent.message = "BUG HIT";
                newEvent.bug     = &bugs[j];
                newEvent.bullet  = &bullets[i];
                
                ofNotifyEvent(GameEvent::events, newEvent);
            }
            
        }
    }
    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    // draw the bug holes
    for(unsigned int i=0; i<holes.size(); i++) {
        ofSetColor(100);
        ofCircle(holes[i], 10);
        ofSetColor(40);
        ofCircle(holes[i], 7);
    }
    
    for(unsigned int i=0; i<bugs.size(); i++) {
        bugs[i].draw();
    }
    
    // draw the bullets
    for(unsigned int i=0; i<bullets.size(); i++) {
        bullets[i].draw();
    }
    
    
    
    // game stats
    ofSetColor(10);
    ofDrawBitmapString("Bullets "+ofToString(bullets.size())+"\nBugs Killed: "+ofToString(bugsKilled), 20, 20);
    
    
    // gun
    ofVec2f gunPos(ofGetWidth()/2, ofGetHeight()-20);
    ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());
    
    // get the vector from the mouse to gun
    ofVec2f vec = mousePos - gunPos;
    vec.normalize();
    vec *= 100;
    
    // get the angle of the vector for rotating the rect
    float angle = ofRadToDeg(atan2(vec.y, vec.x)) - 90;
    
    ofPushMatrix();
    ofTranslate(gunPos.x, gunPos.y);    
    ofRotateZ(angle);
    
    ofFill();
    ofSetColor(10);
    ofRect(-10, 0, 20, 100);
    
    float bulletPct = ofMap(bullets.size(), 0, maxBullets, 0.0, 100.0);
    ofSetColor(100);
    ofRect(-10, 0, 20, bulletPct);
    
    ofSetColor(100);
    ofRect(-10, 90, 20, 10);
    
    if(bFire) {
        ofSetColor(220, 0, 0);
        ofRect(-10, 97, 20, 3);
    }
    ofPopMatrix();
    
    ofSetColor(255);
    ofCircle(gunPos.x, gunPos.y, 2);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    if(key == ' ') {
        if(bullets.size() < maxBullets) {
            bFire = true;
            
            Bullet b;
            
            // the two points for the mouse and gun
            ofVec2f gunPt(ofGetWidth()/2, ofGetHeight()-20);
            ofVec2f mousePt(ofGetMouseX(), ofGetMouseY());
            
            // the vector between the two
            ofVec2f vec = mousePt - gunPt;
            
            // normalize = 0.0 - 1.0
            vec.normalize();
            
            // bullet position = the start pos of the gun
            // and the vec scaled by 100
            ofVec2f bulletPos = gunPt + (vec * 100);
            
            b.pos     = bulletPos;
            b.vel     = vec * ofRandom(9, 12); // randomly make it faster        
            b.bRemove = false;
            
            // add the bullets to the array
            bullets.push_back(b);
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
    bFire = false;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
