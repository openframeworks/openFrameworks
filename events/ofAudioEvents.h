#ifndef _OF_AUDIOEVENTS
#define _OF_AUDIOEVENTS

#include "Poco/FIFOEvent.h"
#include "Poco/Delegate.h"
#include "ofTypes.h"


class ofAudioEventArgs : public ofEventArgs {
  public:
    float* buffer;
    int bufferSize;
    int nChannels;
};


class ofAudioListener {

  protected:
  
	virtual void audioReceived(float* input, int bufferSize, int nChannels){};
	virtual void audioRequested(float* output, int bufferSize, int nChannels){};
            
	void audioReceived(const void* sender, ofAudioEventArgs& eventArgs){
		audioReceived(eventArgs.buffer, eventArgs.bufferSize, eventArgs.nChannels);
	}
	
	void audioRequested( const void* sender, ofAudioEventArgs& eventArgs){
		audioRequested(eventArgs.buffer, eventArgs.bufferSize, eventArgs.nChannels);
	}
	
};



class ofAudioEventManager {
	
  public:
  
    ofAudioEventArgs 	audioEventArgs;
  
  
	void addListener(ofAudioListener* listener){
		addAudioReceivedListener(listener);
		addAudioRequestedListener(listener);
	}
	
	void removeListener(ofAudioListener* listener){
		removeAudioReceivedListener(listener);
		removeAudioRequestedListener(listener);
    }  
  
  
	void addAudioReceivedListener(ofAudioListener* listener){
		audioReceivedEvent += Poco::Delegate<ofAudioListener, ofAudioEventArgs>(listener, &ofAudioListener::audioReceived);
	}
	
	void addAudioRequestedListener(ofAudioListener* listener){
		audioRequestedEvent += Poco::Delegate<ofAudioListener, ofAudioEventArgs>(listener, &ofAudioListener::audioRequested);
	}
	
    
	void removeAudioReceivedListener(ofAudioListener* listener){
		audioReceivedEvent -= Poco::Delegate<ofAudioListener, ofAudioEventArgs>(listener, &ofAudioListener::audioReceived);
	}
		
	void removeAudioRequestedListener(ofAudioListener* listener){
		audioRequestedEvent -= Poco::Delegate<ofAudioListener, ofAudioEventArgs>(listener, &ofAudioListener::audioRequested);
	}
	
    
	void notifyReceived(void* sender){
		audioReceivedEvent.notify(sender, audioEventArgs);
	}
	
	void notifyRequested(void* sender){
		audioRequestedEvent.notify(sender, audioEventArgs);
	}
  
      
  private:
  
	Poco::FIFOEvent<ofAudioEventArgs> audioReceivedEvent;
	Poco::FIFOEvent<ofAudioEventArgs> audioRequestedEvent;
    
};



extern ofAudioEventManager ofAudioEvents;



#endif