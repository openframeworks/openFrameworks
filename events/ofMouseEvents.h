#ifndef _OF_MOUSEEVENTS
#define _OF_MOUSEEVENTS

#include "Poco/FIFOEvent.h"
#include "Poco/Delegate.h"
#include "ofTypes.h"


class ofMouseEventArgs : public ofEventArgs {
  public:
    int x;
    int y;
    int button;
};


class ofMouseListener {

  protected:
  
	virtual void mouseMoved(int x, int y){};
	virtual void mouseDragged(int x, int y, int button){};
    virtual void mousePressed(int x, int y, int button){};
	virtual void mouseReleased(){}; //this will be depreciated - need so we don't break current code
	virtual void mouseReleased(int x, int y, int button){};
    
	void mouseMoved(const void* sender, ofMouseEventArgs& eventArgs){
		mouseMoved(eventArgs.x, eventArgs.y);
	}
    
	void mouseDragged(const void* sender, ofMouseEventArgs& eventArgs){
		mouseDragged(eventArgs.x, eventArgs.y, eventArgs.button);
	}
    
	void mousePressed(const void* sender, ofMouseEventArgs& eventArgs){
		mousePressed(eventArgs.x, eventArgs.y, eventArgs.button);
	}
	
	void mouseReleased( const void* sender, ofMouseEventArgs& eventArgs){
		mouseReleased();
		mouseReleased(eventArgs.x, eventArgs.y, eventArgs.button);
	}
	
};



class ofMouseEventManager {
	
  public:
  
    ofMouseEventArgs 	mouseEventArgs;
  
  
	void addListener(ofMouseListener* listener){
		addMouseDraggedListener(listener);
		addMouseMovedListener(listener);
		addMousePressedListener(listener);
		addMouseReleasedListener(listener);
	}
	
	void removeListener(ofMouseListener* listener){
		removeMouseDraggedListener(listener);
		removeMouseMovedListener(listener);
		removeMousePressedListener(listener);
		removeMouseReleasedListener(listener);
    }  
  

	void addMouseMovedListener(ofMouseListener* listener){
		mouseMovedEvent += Poco::Delegate<ofMouseListener, ofMouseEventArgs>(listener, &ofMouseListener::mouseMoved);
	}
    
	void addMouseDraggedListener(ofMouseListener* listener){
		mouseDraggedEvent += Poco::Delegate<ofMouseListener, ofMouseEventArgs>(listener, &ofMouseListener::mouseDragged);
	}
     
	void addMousePressedListener(ofMouseListener* listener){
		mousePressedEvent += Poco::Delegate<ofMouseListener, ofMouseEventArgs>(listener, &ofMouseListener::mousePressed);
	}
	
	void addMouseReleasedListener(ofMouseListener* listener){
		mouseReleasedEvent += Poco::Delegate<ofMouseListener, ofMouseEventArgs>(listener, &ofMouseListener::mouseReleased);
	}


	void removeMouseMovedListener(ofMouseListener* listener){
		mouseMovedEvent -= Poco::Delegate<ofMouseListener, ofMouseEventArgs>(listener, &ofMouseListener::mouseMoved);
	}
    
	void removeMouseDraggedListener(ofMouseListener* listener){
		mouseDraggedEvent -= Poco::Delegate<ofMouseListener, ofMouseEventArgs>(listener, &ofMouseListener::mouseDragged);
	}
     
	void removeMousePressedListener(ofMouseListener* listener){
		mousePressedEvent -= Poco::Delegate<ofMouseListener, ofMouseEventArgs>(listener, &ofMouseListener::mousePressed);
	}
	
	void removeMouseReleasedListener(ofMouseListener* listener){
		mouseReleasedEvent -= Poco::Delegate<ofMouseListener, ofMouseEventArgs>(listener, &ofMouseListener::mouseReleased);
	}
    	

	void notifyMoved(void* sender){
		mouseMovedEvent.notify(sender, mouseEventArgs);
	}
    
	void notifyDragged(void* sender){
		mouseDraggedEvent.notify(sender, mouseEventArgs);
	}
                
	void notifyPressed(void* sender){
		mousePressedEvent.notify(sender, mouseEventArgs);
	}
	
	void notifyReleased(void* sender){
		mouseReleasedEvent.notify(sender, mouseEventArgs);
	}
  
      
  private:
  
	Poco::FIFOEvent<ofMouseEventArgs> mouseMovedEvent;
	Poco::FIFOEvent<ofMouseEventArgs> mouseDraggedEvent;
	Poco::FIFOEvent<ofMouseEventArgs> mousePressedEvent;
	Poco::FIFOEvent<ofMouseEventArgs> mouseReleasedEvent;
        
};



extern ofMouseEventManager ofMouseEvents;



#endif