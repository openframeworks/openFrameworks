//
//  ofxInputField.cpp
//  ofxInputField
//
//  Based on ofxInputField by Felix Lange
//
//

#pragma once

#include "ofxBaseGui.h"

template<typename Type>
class ofxInputField : public ofxBaseGui{
	template<typename T>
	friend class ofxSlider;

	
public:	
	ofxInputField();
	ofxInputField(ofParameter<Type> _val, float width = defaultWidth, float height = defaultHeight);
	ofxInputField* setup(ofParameter<Type> _val, float width = defaultWidth, float height = defaultHeight);
	ofxInputField* setup(const std::string& _name, Type _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);
	ofxInputField* setup(const std::string& _name, Type _val);
	
	void setMin(Type min);
	Type getMin();
	void setMax(Type max);
	Type getMax();

	void setBlinkingCursor(bool blink);
	void setShowLabelWhileEditing(bool show);

	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);
	virtual bool mouseReleased(ofMouseEventArgs & args);
	virtual bool mouseScrolled(ofMouseEventArgs & args);
	virtual bool keyPressed(ofKeyEventArgs & args);
	virtual bool charPressed(uint32_t & key);

	bool containsValidValue() const;

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}

	Type operator=(Type v);
	operator const Type & ();

	ofAbstractParameter & getParameter();

protected:
	enum Inside{
		InsideSlider,
	};
	void ofxSlider(Inside){
		insideSlider = true;
	}
	virtual void render();
	ofParameter<Type> value;
	bool bGuiActive=false, bMousePressed=false, bMouseOver=false;
	bool setValue(float mx, float my, bool bCheck);
	void generateDraw();
	void valueChanged(Type & value);
	ofVboMesh bg;
	ofVboMesh textMesh;

	std::string input;        // input text
	std::string visibleInput; // input text currently visible, i.e. not obscured by gui
	int visibleInputStart = 0, visibleInputEnd = 0; // boundaries for visible input text

	void parseInput();
	int insertKeystroke(uint32_t character);
	int insertAlphabetic(uint32_t character);

	bool hasSelectedText();

	int mousePressedPos = -1; //set by mouse interaction
	float selectStartX = -1, selectionWidth = 0; //calculated from select indices
	int selectStartPos = -1, selectEndPos = -1;
	void calculateSelectionArea(int selectIdx1, int selectIdx2);
	void moveCursor(int cursorPos);

	virtual void drawCursor();

	bool insideSlider = false;
	bool blinkingCursor = true;
	bool validValue = true;
	bool showLabelWhileEditing = false;
	bool overlappingLabel = false;
	uint64_t errorTime = 0;  // time last input error occured, used for animations
	uint64_t lastCursorMoveTime = 0; // last time cursor was moved, used to calculate whether to blink

	void leaveFocus();

	std::vector<ofFloatColor> originalColors;

	ofEvent<void> leftFocus;
	ofEventListeners listeners;
};

typedef ofxInputField<float> ofxFloatField;
typedef ofxInputField<int> ofxIntField;
typedef ofxInputField<std::string> ofxTextField;
