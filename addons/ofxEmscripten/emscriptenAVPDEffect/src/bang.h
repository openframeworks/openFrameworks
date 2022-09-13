#ifndef _BANG
#define _BANG

#include "ofMain.h"

class bang {

    public:
    bang();
    ~bang();

    void setup(float xpos, float ypos, float size);
    void update();
    void draw();
    void clear();
    
    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    void mouseScrolled(ofMouseEventArgs & args);
    void mouseEntered(ofMouseEventArgs & args);
    void mouseExited(ofMouseEventArgs & args);
    
    void touchDown(ofTouchEventArgs & args);
    void touchMoved(ofTouchEventArgs & args);
    void touchUp(ofTouchEventArgs & args);
    void touchDoubleTap(ofTouchEventArgs & args);
    void touchCancelled(ofTouchEventArgs & args);
        
    float xpos;
    float ypos;
    float size;
    float now;
    bool value;
    bool isTouch;
    ofEvent<bool> onMousePressed;
    
    protected:
    bool bRegisteredEvents;
};
#endif
