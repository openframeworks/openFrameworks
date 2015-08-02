#pragma once

#include "ofParameter.h"
#include "ofxToggle.h"

class ofxMinimalToggle : public ofxToggle {

	public:
		struct Config : public ofxToggle::Config {
			Config(){
			}
			Config(const ofxToggle::Config & c) : ofxToggle::Config(c){
			}
			Config(const ofxBaseGui::Config & c) : ofxToggle::Config(c){
			}
		};
		ofxMinimalToggle();
		~ofxMinimalToggle();
		ofxMinimalToggle(ofParameter <bool> _bVal, const Config & config = Config());

		ofxMinimalToggle & setup(ofParameter <bool> _bVal, float width = defaultWidth, float height = defaultHeight);
		ofxMinimalToggle & setup(const std::string & toggleName, bool _bVal, float width = defaultWidth, float height = defaultHeight);

		void resize(float w, float h);

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

	protected:
		virtual void render();
		void generateDraw();

		ofEvent <void> triggerEvent;
		void valueChanged(bool & v);

		bool setValue(float mx, float my, bool bCheck);
		float getTextWidth(string text, float height);

		ofPath border;
};

