#pragma once

#include "ofxBaseGui.h"

template<typename Type>
class ofxInputField : public ofxBaseGui{
public:
	ofxInputField(const ofJson &config = ofJson());
	ofxInputField(ofParameter<Type> _val, const ofJson &config = ofJson());
	ofxInputField(ofParameter<Type> _val, float width, float height = defaultHeight);
	ofxInputField(const std::string& _name, Type _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);
	//TODO the setup non-ofParameter setup is a pain for the Type string (because of the forced min and max)

	~ofxInputField();

	void setup(const ofJson &config);

	void setMin(Type min);
	Type getMin();
	void setMax(Type max);
	Type getMax();

	virtual void pointerMoved(PointerUIEventArgs& e);
	virtual void pointerDragged(PointerUIEventArgs& e);
	virtual void pointerPressed(PointerUIEventArgs& e);
	virtual void pointerReleased(PointerUIEventArgs& e);
	virtual void pointerScrolled(PointerUIEventArgs& e);

	void registerKeyEvents();
	void unregisterKeyEvents();

	virtual bool keyPressed(ofKeyEventArgs & args);
	virtual bool keyReleased(ofKeyEventArgs & args);

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
	virtual void render();
	ofParameter<Type> value;
	bool bGuiActive, bMousePressed;
	bool mouseInside;
	bool setValue(float mx, float my);
	virtual void generateDraw();
	virtual void generateText();
	void valueChanged(Type & value);
	ofVboMesh textMesh;

	bool bRegisteredForKeyEvents;

	std::string input;
	float inputWidth;
	bool bChangedInternally;
	void parseInput();
	int insertKeystroke(const std::string & character);
	int insertAlphabetic(const std::string & character);

	int mousePressedPos; //set by mouse interaction
	bool hasSelectedText();

	float selectStartX, selectionWidth; //calculated from select indices
	int selectStartPos, selectEndPos;
	void calculateSelectionArea(int selectIdx1, int selectIdx2);

	virtual void drawSelectedArea();
	virtual void drawCursor();
	virtual void drawFocusedBB();
	virtual void drawMesh();

	int pressCounter;

	void leaveFocus();
};

typedef ofxInputField<float> ofxFloatInputField;
typedef ofxInputField<int> ofxIntInputField;
typedef ofxInputField<string> ofxTextField;
