#pragma once
#include "ofxBaseGui.h"

#include "ofxSlider.h"
#include "ofxButton.h"
#include "ofxLabel.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"

class ofxGuiGroup : public ofxBaseGui {
	public:

		struct Config: public ofxBaseGui::Config{
			Config(){
				this->layout = ofxBaseGui::Vertical;
				this->shape.x = 10;
				this->shape.y = 10;
			}
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
			bool distributeEvenly = false;
		};

		ofxGuiGroup();
		ofxGuiGroup(const Config & config);
		ofxGuiGroup(const ofParameterGroup & parameters, const Config & groupConfig = Config(), const Config &itemConfig = ofxBaseGui::Config());
		virtual ~ofxGuiGroup();

		virtual ofxGuiGroup & setup(const Config & config);
		virtual ofxGuiGroup & setup(const std::string& collectionName = "", const Config & config = ofxGuiGroup::Config());
		virtual ofxGuiGroup & setup(const ofParameterGroup & parameters, const Config & groupConfig = ofxGuiGroup::Config(), const Config &itemConfig = ofxBaseGui::Config());
		virtual ofxGuiGroup & setup(const std::string& collectionName, const std::string& filename, float x = 10, float y = 10);
		virtual ofxGuiGroup & setup(const ofParameterGroup & parameters, const std::string& filename, float x = 10, float y = 10);

		void add(ofxBaseGui & element);
		void add(ofxGuiGroup & element);

		template<typename T>
		typename std::enable_if<std::is_arithmetic<T>::value, void>::type add(ofParameter<T> & p){
			add(new ofxSlider<T>(p, typename ofxSlider<T>::Config()));
		}

		void add(ofParameter <void> & parameter);
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

		template<typename T, class Config>
		typename std::enable_if<std::is_arithmetic<T>::value, void>::type add(ofParameter<T> & p, const Config & config){
			add(new ofxSlider<T>(p, config));
		}

		template<class Config>
		void add(ofParameter <void> & parameter, const Config & config);

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
		void add(ofParameter<Type> &p, const Config & config);

		template<class GuiType=ofxGuiGroup, class CGroup, class CItem=ofxBaseGui::Config>
		void add(ofParameterGroup p, const CGroup & groupConfig, const CItem & itemConfig = ofxBaseGui::Config());

		template<class GuiType, class Config>
		void add(const Config & config);

		template<class GuiType>
		void add();

		void addSpacer(float size);

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
		virtual void setSize(float w, float h);
		virtual void setShape(ofRectangle r);
		virtual void setShape(float x, float y, float w, float h);

		void setShowHeader(bool show);
		void setDistributeEvenly(bool distribute);

		void setExclusiveToggles(bool exclusive);
		bool setActiveToggle(int index);
		bool setActiveToggle(ofxToggle* toggle);
		ofParameter<int>& getActiveToggleIndex();

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
		virtual void setWidthElements(float w);
		void addParametersFrom(const ofParameterGroup & parameters, const Config &config = ofxBaseGui::Config());

		float spacing, spacingNextElement, spacingFirstElement;
		float header;

		std::vector <ofxBaseGui *> collection;
		ofParameterGroup parameters;

		std::string filename;
		bool minimized;
		bool bShowHeader;
		bool bExclusiveToggles;
		bool bGuiActive;
		bool bDistributeEvenly;

		ofParameter<int> active_toggle_index;
		bool processToggles(ofxToggle *toggle, ofMouseEventArgs a);
		void setOneToggleActive();
		void deactivateAllOtherToggles(ofxToggle* toggle);

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
void ofxGuiGroup::add(ofParameter <void> & parameter, const C & config){
	add<ofxButton>(parameter, config);
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
void ofxGuiGroup::add(ofParameter<Type> &p, const C & config){
	auto inContainerConfig = config;
	inContainerConfig.inContainer = true;
//	inContainerConfig.layout = this->layout;
	addOwned(new GuiType(p,inContainerConfig));
}

template<class GuiType, class CGroup, class CItem>
void ofxGuiGroup::add(ofParameterGroup p, const CGroup & groupConfig, const CItem & itemConfig){
	auto inContainerConfig = groupConfig;
	inContainerConfig.inContainer = true;
	addOwned(new GuiType(p,inContainerConfig, itemConfig));
}

template<class GuiType, class C>
void ofxGuiGroup::add(const C & config){
	auto inContainerConfig = config;
	inContainerConfig.inContainer = true;
	addOwned(new GuiType(inContainerConfig));
}

template<class GuiType>
void ofxGuiGroup::add(){
	typename GuiType::Config inContainerConfig;
	inContainerConfig.inContainer = true;
	addOwned(new GuiType(inContainerConfig));
}

template<class GuiType, typename Type>
void ofxGuiGroup::add(ofParameter<Type> p){
	add<GuiType>(p,typename GuiType::Config());
}

template<class GuiType>
void ofxGuiGroup::add(ofParameterGroup p){
	add<GuiType>(p,typename GuiType::Config());
}
