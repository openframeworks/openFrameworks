#pragma once

#include "ofxMinimalToggle.h"
#include "ofParameter.h"

class ofxMinimalButton : public ofxMinimalToggle {
	friend class ofPanel;

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

		ofxMinimalButton(const Config & config = Config());
		~ofxMinimalButton();
		ofxMinimalButton & setup(const std::string & toggleName, float width = defaultWidth, float height = defaultHeight);

		virtual bool mouseReleased(ofMouseEventArgs & args);
		virtual bool mouseMoved(ofMouseEventArgs & args);
		virtual bool mousePressed(ofMouseEventArgs & args);
		virtual bool mouseDragged(ofMouseEventArgs & args);

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
