#ifndef _HSLIDER
#define _HSLIDER

#include "ofMain.h"

class hSlider {

    public:
    hSlider();
    ~hSlider();

    void setup(float xpos, float ypos, float width, float height, float minVal, float maxVal);
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
    float width;
    float height;
    float minVal;
    float maxVal;
    float value;
    float slider;
    bool mouseState;
    bool isTouch;
    ofEvent<float> onMousePressed;
    
    protected:
    bool bRegisteredEvents;
};
#endif
