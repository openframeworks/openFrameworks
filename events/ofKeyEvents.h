#ifndef _OF_KEYEVENTS
#define _OF_KEYEVENTS

#include "Poco/FIFOEvent.h"
#include "Poco/Delegate.h"
#include "ofTypes.h"


class ofKeyEventArgs : public ofEventArgs {
  public:
    int key;
};


class ofKeyListener {

  protected:
  
	virtual void keyPressed(int key){};
	virtual void keyReleased(int key){};
  
  
  public:   
	void keyPressed(const void* sender, ofKeyEventArgs& eventArgs){
		keyPressed(eventArgs.key);
	}
	
	void keyReleased( const void* sender, ofKeyEventArgs& eventArgs){
		keyReleased(eventArgs.key);
	}
	
};



class ofKeyEventManager {
	
  public:
  
    ofKeyEventArgs 	keyEventArgs;
  
  
	void addListener(ofKeyListener* listener){
		addKeyPressedListener(listener);
		addKeyReleasedListener(listener);
	}
	
	void removeListener(ofKeyListener* listener){
		removeKeyPressedListener(listener);
		removeKeyReleasedListener(listener);
    }  
  
  
	void addKeyPressedListener(ofKeyListener* listener){
		keyPressedEvent += Poco::Delegate<ofKeyListener, ofKeyEventArgs>(listener, &ofKeyListener::keyPressed);
	}
	
	void addKeyReleasedListener(ofKeyListener* listener){
		keyReleasedEvent += Poco::Delegate<ofKeyListener, ofKeyEventArgs>(listener, &ofKeyListener::keyReleased);
	}
	
    
	void removeKeyPressedListener(ofKeyListener* listener){
		keyPressedEvent -= Poco::Delegate<ofKeyListener, ofKeyEventArgs>(listener, &ofKeyListener::keyPressed);
	}
		
	void removeKeyReleasedListener(ofKeyListener* listener){
		keyReleasedEvent -= Poco::Delegate<ofKeyListener, ofKeyEventArgs>(listener, &ofKeyListener::keyReleased);
	}
	
    
	void notifyPressed(void* sender){
		keyPressedEvent.notify(sender, keyEventArgs);
	}
	
	void notifyReleased(void* sender){
		keyReleasedEvent.notify(sender, keyEventArgs);
	}
  
      
  private:
  
	Poco::FIFOEvent<ofKeyEventArgs> keyPressedEvent;
	Poco::FIFOEvent<ofKeyEventArgs> keyReleasedEvent;
    
};



extern ofKeyEventManager ofKeyEvents;



#endif