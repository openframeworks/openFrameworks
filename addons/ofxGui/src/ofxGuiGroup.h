#pragma once
#include "ofxBaseGui.h"

#include "ofxSlider.h"
#include "ofxButton.h"
#include "ofxLabel.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"

class ofxGuiGroup : public ofxBaseGui {
	public:
		ofxGuiGroup();
		ofxGuiGroup(const ofParameterGroup & parameters, const std::string& _filename = "settings.xml", float x = 10, float y = 10);
		virtual ~ofxGuiGroup(){
		}
		ofxGuiGroup * setup(const std::string& collectionName = "", const std::string& filename = "settings.xml", float x = 10, float y = 10);
		ofxGuiGroup * setup(const ofParameterGroup & parameters, const std::string& filename = "settings.xml", float x = 10, float y = 10);

		void add(ofxBaseGui * element);
		void add(const ofParameterGroup & parameters);

		template<typename T>
		typename std::enable_if<std::is_arithmetic<T>::value, void>::type add(ofParameter<T> & p){
			add(createGuiElement< ofxSlider<T> >(p));
		}
		void add(ofParameter <void> & parameter);
		void add(ofParameter <bool> & parameter);
		void add(ofParameter <std::string> & parameter);

		template<typename F>
		void add(ofReadOnlyParameter <std::string, F> & parameter){
			ownedCollection.emplace_back(std::make_unique<ofxLabel>(parameter));
			add(ownedCollection.back().get());
		}
		void add(ofParameter <ofVec2f> & parameter);
		void add(ofParameter <ofVec3f> & parameter);
		void add(ofParameter <ofVec4f> & parameter);
		void add(ofParameter <glm::vec2> & parameter);
		void add(ofParameter <glm::vec3> & parameter);
		void add(ofParameter <glm::vec4> & parameter);
		void add(ofParameter <ofColor> & parameter);
		void add(ofParameter <ofShortColor> & parameter);
		void add(ofParameter <ofFloatColor> & parameter);
		void add(ofParameter <ofRectangle> & parameter);

		void minimize();
		void maximize();
		void minimizeAll();
		void maximizeAll();
		bool isMinimized() const;

		void setWidthElements(float w);

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

		virtual void setPosition(const glm::vec3& p);
		virtual void setPosition(float x, float y);
		
		void enableHeader();
		void disableHeader();
		bool isHeaderEnabled();
		
		static float elementSpacing;
		static float groupSpacing;
		static float childrenLeftIndent;
		static float childrenRightIndent;
	protected:
	
		void updateChildrenPositions(bool bUpdateWidth = false);
		void updateChild(ofxBaseGui* child, const float& x, const float& y, const float& width, bool bUpdateWidth = false);
		
		bool bHeaderEnabled = true;
		virtual void render();
		virtual bool setValue(float mx, float my, bool bCheck);
		virtual void onMinimize();
		virtual void onMaximize();

		
	
		ofRectangle headerRect;
		ofRectangle minimizeRect;
		
		template <class ControlType>
		ControlType & getControlType(const std::string& name);

		virtual void generateDraw();

		std::vector <ofxBaseGui *> collection;
		ofParameterGroup parameters;

		std::string filename;
		bool minimized;
		bool bGuiActive;

		ofPath border, headerBg;
		ofVboMesh textMesh;
	
	template<typename T, typename P>
	ofxBaseGui* createGuiElement(ofParameter<P>&param, float width = 0, float height = defaultHeight){
		ownedCollection.emplace_back(std::make_unique<T>(param, (ofIsFloatEqual(width, 0.f)? b.width: width), height));
		return ownedCollection.back().get();
	}
	ofxBaseGui* createGuiGroup(const ofParameterGroup & parameters){
		ownedCollection.emplace_back(std::make_unique<ofxGuiGroup>(parameters));
		return ownedCollection.back().get();
	}
	
	private:
	// This array stores the unique pointers for the elements that this gui group creates, thus owns.
	// This allowes for correct memory management and no leaks
	std::vector<std::unique_ptr<ofxBaseGui> > ownedCollection;
	
};

template <class ControlType>
ControlType & ofxGuiGroup::getControlType(const std::string& name){
	ControlType * control = dynamic_cast <ControlType *>(getControl(name));
	if(control){
		return *control;
	}else{
		ofLogWarning() << "getControlType " << name << " not found, creating new";
		ownedCollection.emplace_back(std::make_unique<ControlType>());
		control =  static_cast<ControlType*> (ownedCollection.back().get());
		control->setName(name);
		add(control);
		return *control;
	}
}
