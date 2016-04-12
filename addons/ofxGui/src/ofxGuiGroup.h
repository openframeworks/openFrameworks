#pragma once
#include "ofxBaseGui.h"

#include "ofxSlider.h"
#include "ofxButton.h"
#include "ofxLabel.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "FloatingBoxLayout.h"

template<class VecType>
class ofxVecSlider_;
template<class VecType>
class ofxColorSlider_;

typedef ofxVecSlider_<ofVec3f> ofxVec3Slider;
typedef ofxVecSlider_<ofVec2f> ofxVec2Slider;
typedef ofxVecSlider_<ofVec4f> ofxVec4Slider;
typedef ofxVecSlider_<ofVec3f> ofxPointSlider;

typedef ofxColorSlider_<unsigned char> ofxColorSlider;
typedef ofxColorSlider_<unsigned short> ofxShortColorSlider;
typedef ofxColorSlider_<float> ofxFloatColorSlider;

class ofxPanel;
class ofxGuiTabs;
class ofxFpsPlotter;

class ofxGuiGroupHeader : public ofxBaseGui {
	public:
		ofxGuiGroupHeader(const ofJson &config = ofJson());

		~ofxGuiGroupHeader();

		virtual bool mousePressed(ofMouseEventArgs & args) override;

	protected:
		virtual void generateDraw();
		virtual void render();
		ofVboMesh textMesh;

};


class ofxGuiGroup : public ofxBaseGui {
	public:

		ofxGuiGroup();
		ofxGuiGroup(const std::string& collectionName);
		ofxGuiGroup(const std::string& collectionName, const ofJson & config);
		ofxGuiGroup(const ofParameterGroup & parameters, const ofJson &config = ofJson());
		ofxGuiGroup(const ofParameterGroup & parameters, const std::string& _filename, float x = 10, float y = 10);
		ofxGuiGroup(const std::string& collectionName, const std::string& _filename, float x = 10, float y = 10);

		virtual ~ofxGuiGroup();

		void setup();

		using Element::add;

		template <typename GuiType, typename... Args>
		GuiType* add(Args&&... args);

		template<typename T>
		typename std::enable_if<std::is_arithmetic<T>::value, ofxSlider<T>*>::type add(ofParameter<T> & p, const ofJson & config = ofJson());

		ofxButton* add(ofParameter <void> & parameter, const ofJson & config = ofJson());
		ofxToggle* add(ofParameter <bool> & parameter, const ofJson & config = ofJson());
		ofxLabel* add(ofParameter <std::string> & parameter, const ofJson & config = ofJson());
		ofxVec2Slider *add(ofParameter <ofVec2f> & parameter, const ofJson & config = ofJson());
		ofxVecSlider_<ofVec3f> *add(ofParameter <ofVec3f> & parameter, const ofJson & config = ofJson());
		ofxVec4Slider* add(ofParameter <ofVec4f> & parameter, const ofJson & config = ofJson());
		ofxColorSlider* add(ofParameter <ofColor> & parameter, const ofJson & config = ofJson());
		ofxShortColorSlider* add(ofParameter <ofShortColor> & parameter, const ofJson & config = ofJson());
		ofxFloatColorSlider* add(ofParameter <ofFloatColor> & parameter, const ofJson & config = ofJson());

		void add(const ofParameterGroup& parameters);

		ofxBaseGui* addSpacer(float width, float height);
		ofxBaseGui* addSpacer(const ofJson & config);
		ofxFpsPlotter* addFpsPlotter(const ofJson & config="");

		ofxGuiGroup* addGroup(const std::string& name="", const ofJson& config = ofJson());
		ofxGuiGroup* addGroup(const ofParameterGroup & parameters, const ofJson& config = ofJson());
		ofxPanel* addPanel(const std::string& name="", const ofJson& config = ofJson());
		ofxPanel* addPanel(const ofParameterGroup & parameters, const ofJson& config = ofJson());
		ofxGuiTabs* addTabs(const std::string& name="", const ofJson& config = ofJson());

		virtual void setHeaderBackgroundColor(const ofColor & color) override;

		virtual void minimize();
		virtual void maximize();
		virtual void minimizeAll();
		virtual void maximizeAll();
		bool getMinimized();
		void toggleMinimize();

		virtual void clear() override;

		/// \brief Get a list of pointers to the child elements.
		///
		/// The parent Element retains ownership.
		///
		/// \returns a list of pointers to child elements.
		std::vector<ofxBaseGui*> getControls();

//		std::vector <std::string> getControlNames() const;
		std::size_t getNumControls();

		ofxIntSlider* getIntSlider(const std::string& name);
		ofxFloatSlider* getFloatSlider(const std::string& name);
		ofxToggle* getToggle(const std::string& name);
		ofxButton* getButton(const std::string& name);
		ofxGuiGroup* getGroup(const std::string& name);

		ofxBaseGui* getControl(const std::string& name);
		ofxBaseGui* getControl(std::size_t num);
		int getControlIndex(const std::string& name);
		int getControlIndex(ofxBaseGui* element);

		template <class ControlType>
		ControlType* getControlType(const std::string& name);

		template <class ControlType>
		ControlType* getControlType(const int& index);

		virtual ofAbstractParameter & getParameter() override;
		virtual std::string getName() override;
		virtual void setName(const std::string& name) override;

		void setShowHeader(bool show);
		ofxBaseGui* getHeader();

		bool getTogglesExclusive() const;
		void setExclusiveToggles(bool exclusive);
		bool setActiveToggle(int index);
		bool setActiveToggle(ofxToggle* toggle);
		ofParameter<int>& getActiveToggleIndex();
		void deactivateAllOtherToggles(ofxToggle* toggle);

	protected:

		ofParameterGroup parameters;

		virtual void _setConfig(const ofJson & config);

		void addParametersFrom(const ofParameterGroup & parameters);

		ofParameter<int> active_toggle_index;
		bool processToggles();
		void setOneToggleActive();

		ofParameter<std::string> filename;
		ofParameter<bool> exclusiveToggles;
		ofParameter<bool> minimized;

		ofParameter<float> headerHeight;
		ofParameter<bool> showHeader;
		ofxBaseGui* header;

		virtual void onHeaderVisibility(bool& showing);
		virtual void onHeaderHeight(float& height);
		virtual void onResize(ResizeEventArgs&);

};

template <class ControlType>
ControlType* ofxGuiGroup::getControlType(const std::string& name){
	ControlType* control = dynamic_cast <ControlType *>(getControl(name));
	if(control){
		return control;
	}else{
		ofLogWarning() << "getControlType " << name << " not found, creating new";
		control = this->add<ControlType>();
		control->setName(name);
		return control;
	}
}

template <class ControlType>
ControlType* ofxGuiGroup::getControlType(const int& index){

	int counter = -1;
	for(auto &child : children()){
		if(ControlType* e = dynamic_cast<ControlType*>(child)){
			counter++;
			if(counter == index){
				return e;
			}
		}
	}

	return nullptr;

}

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, ofxSlider<T>*>::type ofxGuiGroup::add(ofParameter<T> & p, const ofJson & config){
	return add<ofxSlider<T>>(p,config);
}


template <typename GuiType, typename... Args>
GuiType* ofxGuiGroup::add(Args&&... args){

	GuiType* e = Element::add(std::make_unique<GuiType>(std::forward<Args>(args)...));

	if(exclusiveToggles) {
		setOneToggleActive();
	}

	return e;

}

