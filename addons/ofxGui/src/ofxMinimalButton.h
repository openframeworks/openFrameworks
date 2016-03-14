#pragma once

#include "ofxMinimalToggle.h"
#include "ofParameter.h"

class ofxMinimalButton : public ofxMinimalToggle {

	public:

		ofxMinimalButton(const std::string& buttonName="", const ofJson & config = ofJson());
		ofxMinimalButton(const std::string& buttonName, float width, float height);

		~ofxMinimalButton();

		void setup(const ofJson& config = ofJson());

		virtual void pointerReleased(PointerUIEventArgs& e);

		template <class ListenerClass, typename ListenerMethod>
		void addListener(ListenerClass * listener, ListenerMethod method){
			ofAddListener(triggerEvent, listener, method);
		}

		template <class ListenerClass, typename ListenerMethod>
		void removeListener(ListenerClass * listener, ListenerMethod method){
			ofRemoveListener(triggerEvent, listener, method);
		}

	private:

		ofEvent <void> triggerEvent;
		void valueChanged(bool & v);

};
