#pragma once

#include "ofxMinimalToggle.h"
#include "ofParameter.h"

class ofxMinimalButton : public ofxMinimalToggle {

	public:
		struct Config : public ofxMinimalToggle::Config {
			Config(){
			}
			Config(const ofxMinimalToggle::Config & c) : ofxMinimalToggle::Config(c){
			}
			Config(const ofxBaseGui::Config & c) : ofxMinimalToggle::Config(c){
			}
			std::string name;
		};

		ofxMinimalButton();
		ofxMinimalButton(const Config & config);
		~ofxMinimalButton();
		ofxMinimalButton & setup(const std::string& buttonName, const Config & config);
		ofxMinimalButton & setup(const std::string& buttonName, float width = defaultWidth, float height = defaultHeight);

		virtual bool mouseReleased(ofMouseEventArgs & args);

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
