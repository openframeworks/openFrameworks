#pragma once
#include "ofMain.h"

#include "Bug.h"
#include "Bullet.h"
#include "GameEvent.h"

class ofApp : public ofBaseApp{
			
	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		static bool shouldRemoveBullet(Bullet &b);
		static bool shouldRemoveBug(Bug &b);
		
		void gameEvent(GameEvent &e);
		
		vector <Bullet> bullets;
		vector <Bug> bugs;
		vector <ofVec2f> holes;
		
		unsigned int maxBullets;
		int bugsKilled;
		bool bFire;
};
