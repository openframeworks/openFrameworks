#ifndef _OF_APPEVENTS
#define _OF_APPEVENTS

#include "Poco/FIFOEvent.h"
#include "Poco/Delegate.h"
#include "ofTypes.h"




class ofAppListener {

protected:
  
	virtual void setup(){};
	virtual void update(){};
    virtual void draw(){};
	virtual void exit(){};
    
public:
	
	void setup(const void* sender, ofEventArgs& eventArgs){
		setup();
	}
    
	void update(const void* sender, ofEventArgs& eventArgs){
		update();
	}
    
	void draw(const void* sender, ofEventArgs& eventArgs){
		draw();
	}
	
	void exit( const void* sender, ofEventArgs& eventArgs){
		exit();
	}
	
};



class ofAppEventManager {
	
  public:
  
    ofEventArgs  eventArgs;
  
  
	void addListener(ofAppListener* listener){
		addSetupListener(listener);
		addDrawListener(listener);
		addUpdateListener(listener);
		addExitListener(listener);
	}
	
	void removeListener(ofAppListener* listener){
		removeSetupListener(listener);
		removeUpdateListener(listener);
		removeDrawListener(listener);
		removeExitListener(listener);
    }  
  

	void addSetupListener(ofAppListener* listener){
		setupEvent += Poco::Delegate<ofAppListener, ofEventArgs>(listener, &ofAppListener::setup);
	}

	void addUpdateListener(ofAppListener* listener){
		updateEvent += Poco::Delegate<ofAppListener, ofEventArgs>(listener, &ofAppListener::update);
	}
    
	void addDrawListener(ofAppListener* listener){
		drawEvent += Poco::Delegate<ofAppListener, ofEventArgs>(listener, &ofAppListener::draw);
	}
     
	void addExitListener(ofAppListener* listener){
		exitEvent += Poco::Delegate<ofAppListener, ofEventArgs>(listener, &ofAppListener::exit);
	}
	

	void removeSetupListener(ofAppListener* listener){
		setupEvent -= Poco::Delegate<ofAppListener, ofEventArgs>(listener, &ofAppListener::setup);
	}
    
	void removeUpdateListener(ofAppListener* listener){
		updateEvent -= Poco::Delegate<ofAppListener, ofEventArgs>(listener, &ofAppListener::update);
	}
     
	void removeDrawListener(ofAppListener* listener){
		drawEvent -= Poco::Delegate<ofAppListener, ofEventArgs>(listener, &ofAppListener::draw);
	}
	
	void removeExitListener(ofAppListener* listener){
		exitEvent -= Poco::Delegate<ofAppListener, ofEventArgs>(listener, &ofAppListener::exit);
	}
    	

	void notifySetup(void* sender){
		setupEvent.notify(sender, eventArgs);
	}
    
	void notifyUpdate(void* sender){
		updateEvent.notify(sender, eventArgs);
	}
                
	void notifyDraw(void* sender){
		drawEvent.notify(sender, eventArgs);
	}
	
	void notifyExit(void* sender){
		exitEvent.notify(sender, eventArgs);
	}
  
      
  private:
  
	Poco::FIFOEvent<ofEventArgs> setupEvent;
	Poco::FIFOEvent<ofEventArgs> updateEvent;
	Poco::FIFOEvent<ofEventArgs> drawEvent;
	Poco::FIFOEvent<ofEventArgs> exitEvent;
        
};



extern ofAppEventManager ofAppEvents;



#endif