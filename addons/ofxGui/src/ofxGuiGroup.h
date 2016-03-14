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

class ofxGuiSpacer;

class ofxGuiGroupHeader : public ofxBaseGui {
	public:
		ofxGuiGroupHeader(const ofJson &config = ofJson());

		~ofxGuiGroupHeader();

		virtual void pointerPressed(PointerUIEventArgs & e);

	protected:
		virtual void generateDraw();
		virtual void render();
		ofVboMesh textMesh;

};


class ofxGuiGroup : public ofxBaseGui {
	public:

		ofxGuiGroup(const std::string& collectionName="", const ofJson & config = ofJson());
		ofxGuiGroup(const ofParameterGroup & parameters, const ofJson &config = ofJson());
		ofxGuiGroup(const ofParameterGroup & parameters, const std::string& _filename, float x = 10, float y = 10);
		ofxGuiGroup(const std::string& collectionName, const std::string& _filename, float x = 10, float y = 10);

		virtual ~ofxGuiGroup();

		void setup(const ofJson &config);

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

		void minimize();
		void maximize();
		void minimizeAll();
		void maximizeAll();
		bool getMinimized();
		void toggleMinimize();

		virtual void clear() override;

//		std::vector <std::string> getControlNames() const;
		std::size_t getNumControls();

		ofxIntSlider & getIntSlider(const std::string& name);
		ofxFloatSlider & getFloatSlider(const std::string& name);
		ofxToggle & getToggle(const std::string& name);
		ofxButton & getButton(const std::string& name);
		ofxGuiGroup & getGroup(const std::string& name);

		ofxBaseGui * getControl(const std::string& name);
		ofxBaseGui * getControl(std::size_t num);
		int getControlIndex(const std::string& name);

		virtual ofAbstractParameter & getParameter();

		void setShowHeader(bool show);
		void setDistributeEvenly(bool distribute);

		void setExclusiveToggles(bool exclusive);
		bool setActiveToggle(int index);
		bool setActiveToggle(ofxToggle* toggle);
		ofParameter<int>& getActiveToggleIndex();
		void deactivateAllOtherToggles(ofxToggle* toggle);

	protected:

		ofParameterGroup parameters;

		virtual void processConfig(const ofJson & config);

		template <class ControlType>
		ControlType & getControlType(const std::string& name);

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


	private:
		void onHeaderVisibility(bool& showing);
		void onHeaderHeight(float& height);
		void onResize(ResizeEventArgs&);

};

template <class ControlType>
ControlType & ofxGuiGroup::getControlType(const std::string& name){
	// TODO why reference not pointer?
//	ControlType * control = dynamic_cast <ControlType *>(getControl(name));
//	if(control){
//		return *control;
//	}else{
//		ofLogWarning() << "getControlType " << name << " not found, creating new";
//		control = new ControlType;
//		control->setName(name);
//		add(control);
//		return *control;
//	}
	ofLogError("ofxGuiGroup::getControlType") << "function not implemented! Do not use it!";
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

