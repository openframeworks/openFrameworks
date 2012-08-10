#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	ofBackground(0);
	ofSetColor(255);
	
	svg.load("tiger.svg");
}

float step;

//--------------------------------------------------------------
void testApp::update()
{
	step += 0.001;
	if (step > 1) {
        
		step -= 1;
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofRotate(mouseX);
	float scale = ofMap(mouseY, 0, ofGetHeight(), .5, 10);
	ofScale(scale, scale, scale);
	ofTranslate(-250, -250);
	if(ofGetMousePressed()) {
		
		for (int i = 0; i < svg.getNumPath(); i++)
		{
			ofPath &p = svg.getPathAt(i);
			
			vector<ofPolyline>& lines = p.getOutline();
			
			for (int k = 0; k < lines.size(); k++)
			{
				ofPolyline line = lines[k].getResampledBySpacing(1);
				
				int num = step * line.size();
				
				glBegin(GL_LINE_STRIP);
				for (int j = 0; j < num; j++)
				{
					ofVec3f &vv = line[j];
					glVertex3f(vv.x, vv.y, vv.z);
				}
				glEnd();
			}
		}
	} else {
		svg.draw();
	}
	
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
	
}
