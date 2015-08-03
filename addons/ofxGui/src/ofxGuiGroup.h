#pragma once
#include "ofxBaseGui.h"

#include "ofxSlider.h"
#include "ofxButton.h"
#include "ofxLabel.h"
#include "ofParameterGroup.h"

class ofxGuiGroup : public ofxBaseGui {
	public:
		struct Config: public ofxBaseGui::Config{
			Config(){}
			Config(const ofxBaseGui::Config & c)
			:ofxBaseGui::Config(c){}

			std::string filename = "settings.xml";
			bool minimized = false;
			float spacing = 1;
			float spacingNextElement = 3;
			float spacingFirstElement = 0;
            float header = defaultHeight;
            bool showHeader = true;
            bool exclusiveToggles = false;
		};

        ofxGuiGroup();
        ofxGuiGroup(const ofParameterGroup & parameters, const Config & config = Config());
		ofxGuiGroup(const ofParameterGroup & parameters, const std::string& _filename, float x = 10, float y = 10);
		virtual ~ofxGuiGroup();

		virtual ofxGuiGroup & setup(const ofParameterGroup & parameters, const Config & config);
		virtual ofxGuiGroup & setup(const std::string& collectionName = "", const std::string& filename = "settings.xml", float x = 10, float y = 10);
		virtual ofxGuiGroup & setup(const ofParameterGroup & parameters, const std::string& filename = "settings.xml", float x = 10, float y = 10);

		void add(ofxBaseGui & element);
		void add(ofxGuiGroup & element);

		void add(ofParameter <float> & parameter);
		void add(ofParameter <int> & parameter);
		void add(ofParameter <bool> & parameter);
		void add(ofParameter <std::string> & parameter);
		void add(ofParameter <ofVec2f> & parameter);
		void add(ofParameter <ofVec3f> & parameter);
		void add(ofParameter <ofVec4f> & parameter);
		void add(ofParameter <ofColor> & parameter);
		void add(ofParameter <ofShortColor> & parameter);
		void add(ofParameter <ofFloatColor> & parameter);

		template<class GuiType, typename Type>
		void add(ofParameter<Type> p);

		template<class GuiType=ofxGuiGroup>
		void add(ofParameterGroup p);

		template<class Config>
		void add(ofParameter <float> & parameter, const Config & config);

		template<class Config>
		void add(ofParameter <int> & parameter, const Config & config);

		template<class Config>
		void add(ofParameter <bool> & parameter, const Config & config);

		template<class Config>
		void add(ofParameter <std::string> & parameter, const Config & config);

		template<class Config>
		void add(ofParameter <ofVec2f> & parameter, const Config & config);

		template<class Config>
		void add(ofParameter <ofVec3f> & parameter, const Config & config);

		template<class Config>
		void add(ofParameter <ofVec4f> & parameter, const Config & config);

		template<class Config>
		void add(ofParameter <ofColor> & parameter, const Config & config);

		template<class Config>
		void add(ofParameter <ofShortColor> & parameter, const Config & config);

		template<class Config>
		void add(ofParameter <ofFloatColor> & parameter, const Config & config);

		template<class GuiType, typename Type, class Config>
		void add(ofParameter<Type> p, const Config & config);

		template<class GuiType=ofxGuiGroup, class Config>
		void add(ofParameterGroup p, const Config & config);

		template<class GuiType, class Config>
		void add(const Config & config);

		void minimize();
		void maximize();
		void minimizeAll();
		void maximizeAll();

		void clear();

		virtual void sizeChangedCB();

		virtual bool mouseMoved(ofMouseEventArgs & args);
		virtual bool mousePressed(ofMouseEventArgs & args);
		virtual bool mouseDragged(ofMouseEventArgs & args);
		virtual bool mouseReleased(ofMouseEventArgs & args);
		virtual bool mouseScrolled(ofMouseEventArgs & args);


		std::vector <std::string> getControlNames() const;
		std::size_t getNumControls() const;

		ofxIntSlider & getIntSlider(const std::string& name);
		ofxFloatSlider & getFloatSlider(const std::string& name);
		ofxToggle & getToggle(const std::string& name);
		ofxButton & getButton(const std::string& name);
		ofxGuiGroup & getGroup(const std::string& name);

		ofxBaseGui * getControl(const std::string& name);
		ofxBaseGui * getControl(std::size_t num);

		virtual ofAbstractParameter & getParameter();

		virtual void setPosition(ofPoint p);
		virtual void setPosition(float x, float y);
        virtual void setSize(float w, float h, bool callback = true);
        virtual void setShape(ofRectangle r, bool callback = true);
        virtual void setShape(float x, float y, float w, float h, bool callback = true);

        void setShowHeader(bool show);

        void setExclusiveToggles(bool exclusive);
        bool setActiveToggle(int index);
        bool setActiveToggle(ofxToggle* toggle);
        int getActiveToggleIndex();

	protected:
		virtual void render();
		virtual void generateDraw();
		virtual bool setValue(float mx, float my, bool bCheck);
		virtual void addOwned(ofxBaseGui * element);
		virtual void addOwned(ofxGuiGroup * element);
		virtual void add(ofxBaseGui * element);
		virtual void add(ofxGuiGroup * element);
		template <class ControlType>
		ControlType & getControlType(const std::string& name);
		void setWidthElements(float w);
		void addParametersFrom(const ofParameterGroup & parameters);

        int active_toggle_index;
        bool processToggles(ofxToggle *toggle, ofMouseEventArgs a);
        void setOneToggleActive();
        void deactivateAllOtherToggles(ofxToggle* toggle);

		float spacing, spacingNextElement, spacingFirstElement;
		float header;

		std::vector <ofxBaseGui *> collection;
		ofParameterGroup parameters;

		std::string filename;
		bool minimized;
		bool bGuiActive;
        bool bShowHeader;
        bool bExclusiveToggles;

		ofPath border, headerBg;
		ofVboMesh textMesh;
		Config config;
	private:
		std::vector <std::unique_ptr<ofxBaseGui>> collectionOwned;
};

template <class ControlType>
ControlType & ofxGuiGroup::getControlType(const std::string& name){
	ControlType * control = dynamic_cast <ControlType *>(getControl(name));
	if(control){
		return *control;
	}else{
		ofLogWarning() << "getControlType " << name << " not found, creating new";
		control = new ControlType;
		control->setName(name);
		add(control);
		return *control;
	}
}

template<class C>
void ofxGuiGroup::add(ofParameter <float> & parameter, const C & config){
	add<ofxFloatSlider>(parameter, config);
}

template<class C>
void ofxGuiGroup::add(ofParameter <int> & parameter, const C & config){
	add<ofxIntSlider>(parameter, config);
}

template<class C>
void ofxGuiGroup::add(ofParameter <bool> & parameter, const C & config){
	add<ofxToggle>(parameter, config);
}

template<class C>
void ofxGuiGroup::add(ofParameter <std::string> & parameter, const C & config){
	add<ofxLabel>(parameter, config);
}

template<class GuiType, typename Type, class C>
void ofxGuiGroup::add(ofParameter<Type> p, const C & config){
	addOwned(new GuiType(p,config));
}

template<class GuiType, class C>
void ofxGuiGroup::add(ofParameterGroup p, const C & config){
	addOwned(new GuiType(p,config));
}

template<class GuiType, class C>
void ofxGuiGroup::add(const C & config){
	addOwned(new GuiType(config));
}

template<class GuiType, typename Type>
void ofxGuiGroup::add(ofParameter<Type> p){
	add<GuiType>(p,this->config);
}

template<class GuiType>
void ofxGuiGroup::add(ofParameterGroup p){
	add<GuiType>(p,this->config);
}
