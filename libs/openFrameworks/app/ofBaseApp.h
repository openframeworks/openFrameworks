#pragma once

#include "ofSoundBaseTypes.h"

class ofResizeEventArgs;
class ofEventArgs;
class ofMouseEventArgs;
class ofKeyEventArgs;
class ofDragInfo;
class ofMessage;
class ofTouchEventArgs;


class ofBaseApp : public ofBaseSoundInput, public ofBaseSoundOutput{

	public:
		ofBaseApp();

		virtual ~ofBaseApp();

		virtual void setup();
		virtual void update();
		virtual void draw();
		virtual void exit();

		virtual void windowResized(int w, int h);

		virtual void keyPressed( int key );
		virtual void keyReleased( int key );

		/// \brief Called on the active window when the mouse is moved
		virtual void mouseMoved( int x, int y );

		/// \brief Called on the active window when the mouse is dragged, i.e.
		/// moved with a button pressed
		virtual void mouseDragged( int x, int y, int button );

		/// \brief Called on the active window when a mouse button is pressed
		virtual void mousePressed( int x, int y, int button );

		/// \brief Called on the active window when a mouse button is released
		virtual void mouseReleased(int x, int y, int button );

		/// \brief Called on the active window when the mouse wheel is scrolled
		virtual void mouseScrolled(int x, int y, float scrollX, float scrollY );

		/// \brief Called on the active window when the mouse cursor enters the
		/// window area
		///
		/// Note that the mouse coordinates are the last known x/y before the
		/// event occurred, i.e. from the previous frame
		virtual void mouseEntered( int x, int y );

		/// \brief Called on the active window when the mouse cursor leaves the
		/// window area
		///
		/// Note that the mouse coordinates are the last known x/y before the
		/// event occurred, i.e. from the previous frame
		virtual void mouseExited( int x, int y);
		
		virtual void dragEvent(ofDragInfo dragInfo);
		virtual void gotMessage(ofMessage msg);
		
		int mouseX, mouseY;			// for processing heads

		virtual void setup(ofEventArgs & args);
		virtual void update(ofEventArgs & args);
		virtual void draw(ofEventArgs & args);
		virtual void exit(ofEventArgs & args);

		virtual void windowResized(ofResizeEventArgs & resize);

		virtual void keyPressed( ofKeyEventArgs & key );
		virtual void keyReleased( ofKeyEventArgs & key );

		virtual void mouseMoved( ofMouseEventArgs & mouse );
		virtual void mouseDragged( ofMouseEventArgs & mouse );
		virtual void mousePressed( ofMouseEventArgs & mouse );
		virtual void mouseReleased(ofMouseEventArgs & mouse);
		virtual void mouseScrolled( ofMouseEventArgs & mouse );
		virtual void mouseEntered( ofMouseEventArgs & mouse );
		virtual void mouseExited( ofMouseEventArgs & mouse );
		virtual void dragged(ofDragInfo & drag);
		virtual void messageReceived(ofMessage & message);

		virtual void touchDown(int x, int y, int id);
		virtual void touchMoved(int x, int y, int id);
		virtual void touchUp(int x, int y, int id);
		virtual void touchDoubleTap(int x, int y, int id);
		virtual void touchCancelled(int x, int y, int id);

		virtual void touchDown(ofTouchEventArgs & touch);
		virtual void touchMoved(ofTouchEventArgs & touch);
		virtual void touchUp(ofTouchEventArgs & touch);
		virtual void touchDoubleTap(ofTouchEventArgs & touch);
		virtual void touchCancelled(ofTouchEventArgs & touch);
};


