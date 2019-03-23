#pragma once

#include "ofConstants.h"
#include "ofEventUtils.h"
#include "ofTimer.h"
#include "ofFpsCounter.h"
#include <set>
#include "glm/vec2.hpp"

//-------------------------- mouse/key query
bool ofGetMousePressed(int button=-1); //by default any button
bool ofGetKeyPressed(int key=-1); //by default any key

int	ofGetMouseX();
int	ofGetMouseY();

int	ofGetPreviousMouseX();
int	ofGetPreviousMouseY();



//--------------------------------------------
//
// 	Keyboard definitions
//
// 	ok -- why this?
// 	glut key commands have some annoying features,
// 	in that some normal keys have the same value as special keys,
// 	but we want ONE key routine, so we need to redefine several,
// 	so that we get some normalacy across keys routines
//
// 	(everything that comes through "glutSpecialKeyFunc" will get 256 added to it,
// 	to avoid conflicts, before, values like "left, right up down" (ie, 104, 105, 106) were conflicting with
// 	letters.. now they will be 256 + 104, 256 + 105....)

enum ofKey{
	OF_KEY_RETURN	=	13,
	OF_KEY_ESC		=	27,
	OF_KEY_TAB      =    9,


	OF_KEY_BACKSPACE =	8,
	OF_KEY_DEL		 =	127,


	// For legacy reasons we are mixing up control keys
	// and unicode codepoints when sending key events,
	// for the modifiers that need to be usable as bitmask
	// we are using some control codes that have nothing to do
	// with the keys being represented and then 0x0ee0.. and 0x0e60...
	// which are free in the unicode table

	OF_KEY_SHIFT	=	 0x1,
	OF_KEY_CONTROL	=	 0x2,
	OF_KEY_ALT		=	 0x4,
	OF_KEY_SUPER	=	 0x10,
	OF_KEY_COMMAND  =    OF_KEY_SUPER,
	OF_KEY_LEFT_SHIFT    =	 0xe60,
	OF_KEY_RIGHT_SHIFT   =	 0xe61,
	OF_KEY_LEFT_CONTROL  =	 0xe62,
	OF_KEY_RIGHT_CONTROL = 0xe63,
	OF_KEY_LEFT_ALT		= 0xe64,
	OF_KEY_RIGHT_ALT	= 0xe65,
	OF_KEY_LEFT_SUPER	= 0xe66,
	OF_KEY_RIGHT_SUPER	= 0xe67,
	OF_KEY_LEFT_COMMAND = OF_KEY_LEFT_SUPER,
	OF_KEY_RIGHT_COMMAND = OF_KEY_RIGHT_SUPER,

	// Use values from the Unicode private use codepoint range E000 - F8FF. 
	// See https://www.unicode.org/faq/private_use.html
	OF_KEY_F1        = 0xe000,
	OF_KEY_F2        = 0xe001,
	OF_KEY_F3        = 0xe002,
	OF_KEY_F4        = 0xe003,
	OF_KEY_F5        = 0xe004,
	OF_KEY_F6        = 0xe005,
	OF_KEY_F7        = 0xe006,
	OF_KEY_F8        = 0xe007,
	OF_KEY_F9        = 0xe008,
	OF_KEY_F10       = 0xe009,
	OF_KEY_F11       = 0xe00A,
	OF_KEY_F12       = 0xe00B,
	OF_KEY_LEFT      = 0xe00C,
	OF_KEY_UP        = 0xe00D,
	OF_KEY_RIGHT     = 0xe00E,
	OF_KEY_DOWN      = 0xe00F,
	OF_KEY_PAGE_UP   = 0xe010,
	OF_KEY_PAGE_DOWN = 0xe011,
	OF_KEY_HOME      = 0xe012,
	OF_KEY_END       = 0xe013,
	OF_KEY_INSERT    = 0xe014,

	OF_MOUSE_BUTTON_1 =    0,
	OF_MOUSE_BUTTON_2 =    1,
	OF_MOUSE_BUTTON_3 =    2,
	OF_MOUSE_BUTTON_4 =    3,
	OF_MOUSE_BUTTON_5 =    4,
	OF_MOUSE_BUTTON_6 =    5,
	OF_MOUSE_BUTTON_7 =    6,
	OF_MOUSE_BUTTON_8 =    7,
	OF_MOUSE_BUTTON_LAST   = OF_MOUSE_BUTTON_8,
	OF_MOUSE_BUTTON_LEFT   = OF_MOUSE_BUTTON_1,
	OF_MOUSE_BUTTON_MIDDLE = OF_MOUSE_BUTTON_2,
	OF_MOUSE_BUTTON_RIGHT  = OF_MOUSE_BUTTON_3,
};

//-----------------------------------------------
class ofDragInfo{
	public:
		std::vector <std::string> files;
		glm::vec2 position;
};


//-----------------------------------------------
// event arguments, this are used in oF to pass
// the data when notifying events

class ofEventArgs{};

class ofKeyEventArgs : public ofEventArgs {
public:
	enum Type{
		Pressed,
		Released,
	};

	ofKeyEventArgs()
  	:type(Pressed)
  	,key(0)
	,keycode(0)
	,scancode(0)
	,codepoint(0)
	,isRepeat(false){}

	ofKeyEventArgs(Type type, int key, int keycode, int scancode, unsigned int codepoint, int modifiers)
	:type(type)
	,key(key)
	,keycode(keycode)
	,scancode(scancode)
	,codepoint(codepoint)
	,isRepeat(false)
	,modifiers(modifiers){

	}

	ofKeyEventArgs(Type type, int key)
	:type(type)
	,key(key)
	,keycode(0)
	,scancode(0)
	,codepoint(0)
	,isRepeat(false){

	}

	Type type;
	/// For special keys, one of OF_KEY_* (@see ofConstants.h). For all other keys, the Unicode code point you'd expect if this key combo (including modifier keys that may be down) was pressed in a text editor (same as codepoint).
	int key;
	/// The keycode returned by the windowing system, independent of any modifier keys or keyboard layout settings. For ofAppGLFWWindow this value is one of GLFW_KEY_* (@see glfw3.h) - typically, ASCII representation of the symbol on the physical key, so A key always returns 0x41 even if shift, alt, ctrl are down.
	int keycode;
	/// The raw scan code returned by the keyboard, OS and hardware specific.
	int scancode;
	/// The Unicode code point you'd expect if this key combo (including modifier keys) was pressed in a text editor, or 0 for non-printable characters.
	uint32_t codepoint;
	/// If this is a repeat event
	bool isRepeat;
	/// Key modifiers
	int modifiers = 0;

	bool hasModifier(int modifier) const {
		return modifiers & modifier;
	}
};

class ofMouseEventArgs : public ofEventArgs, public glm::vec2 {
  public:
	enum Type{
		Pressed,
		Moved,
		Released,
		Dragged,
		Scrolled,
		Entered,
		Exited
	};

	ofMouseEventArgs()
	:type(Pressed)
	,button(OF_MOUSE_BUTTON_LEFT)
	,scrollX(0.f)
	,scrollY(0.f)
	{}

	ofMouseEventArgs(Type type, float x, float y, int button)
	:glm::vec2(x,y)
	,type(type)
	,button(button)
	,scrollX(0.f)
	,scrollY(0.f)
	{}

	ofMouseEventArgs(Type type, float x, float y, int button, int modifiers)
	:glm::vec2(x,y)
	,type(type)
	,button(button)
	,scrollX(0.f)
	,scrollY(0.f)
	,modifiers(modifiers)
	{}

	ofMouseEventArgs(Type type, float x, float y)
	:glm::vec2(x,y)
	,type(type)
	,button(0)
	,scrollX(0.f)
	,scrollY(0.f)
	{}

	Type type;
	int button;
	float scrollX;
	float scrollY;
	/// Key modifiers
	int modifiers = 0;

	bool hasModifier(int modifier){
		return modifiers & modifier;
	}
};

class ofTouchEventArgs : public ofEventArgs, public glm::vec2 {
  public:
	enum Type{
		down,
		up,
		move,
		doubleTap,
		cancel
	};

	ofTouchEventArgs()
	:type(down)
	,id(0)
	,time(0)
	,numTouches(0)
	,width(0)
	,height(0)
	,angle(0)
	,minoraxis(0)
	,majoraxis(0)
	,pressure(0)
	,xspeed(0)
	,yspeed(0)
	,xaccel(0)
	,yaccel(0)
	{

	}

	ofTouchEventArgs(Type type, float x, float y, int id)
	:glm::vec2(x,y)
	,type(type)
	,id(id)
	,time(0)
	,numTouches(0)
	,width(0)
	,height(0)
	,angle(0)
	,minoraxis(0)
	,majoraxis(0)
	,pressure(0)
	,xspeed(0)
	,yspeed(0)
	,xaccel(0)
	,yaccel(0){}

	Type type;
	int id;
	int time;
	int numTouches;
	float width, height;
	float angle;
	float minoraxis, majoraxis;
	float pressure;
	float xspeed, yspeed;
	float xaccel, yaccel;
};

class ofResizeEventArgs : public ofEventArgs {
public:
	ofResizeEventArgs()
  	:width(0)
	,height(0){}

	ofResizeEventArgs(int width, int height)
	:width(width)
	,height(height){}

	int width;
	int height;
};

class ofWindowPosEventArgs : public ofEventArgs, public glm::vec2  {
public:
	ofWindowPosEventArgs(){}

	ofWindowPosEventArgs(int x, int y)
	:glm::vec2(x,y){}
};

class ofMessage : public ofEventArgs{
	public:
		ofMessage( std::string msg ){
			message = msg;
		}
		std::string message;
};

enum ofTimeMode{
	System,
	FixedRate,
	Filtered,
};

class ofCoreEvents {
  public:
	ofCoreEvents();
	ofEvent<ofEventArgs> 		setup;
	ofEvent<ofEventArgs> 		update;
	ofEvent<ofEventArgs> 		draw;
	ofEvent<ofEventArgs> 		exit;

	ofEvent<ofResizeEventArgs> 	windowResized;
	ofEvent<ofWindowPosEventArgs> 	windowMoved;

	ofEvent<ofKeyEventArgs> 	keyPressed;
	ofEvent<ofKeyEventArgs> 	keyReleased;

	ofEvent<ofMouseEventArgs> 	mouseMoved;
	ofEvent<ofMouseEventArgs> 	mouseDragged;
	ofEvent<ofMouseEventArgs> 	mousePressed;
	ofEvent<ofMouseEventArgs> 	mouseReleased;
	ofEvent<ofMouseEventArgs> 	mouseScrolled;
	ofEvent<ofMouseEventArgs> 	mouseEntered;
	ofEvent<ofMouseEventArgs> 	mouseExited;

	ofEvent<ofTouchEventArgs>	touchDown;
	ofEvent<ofTouchEventArgs>	touchUp;
	ofEvent<ofTouchEventArgs>	touchMoved;
	ofEvent<ofTouchEventArgs>	touchDoubleTap;
	ofEvent<ofTouchEventArgs>	touchCancelled;

	ofEvent<ofMessage>			messageEvent;
	ofEvent<ofDragInfo>			fileDragEvent;
	ofEvent<uint32_t>			charEvent;

	void disable();
	void enable();

	void setTimeModeSystem();
	void setTimeModeFixedRate(uint64_t nanosecsPerFrame);
	void setTimeModeFiltered(float alpha);

	void setFrameRate(int _targetRate);
	float getFrameRate() const;
	float getTargetFrameRate() const;
	double getLastFrameTime() const;
	uint64_t getFrameNum() const;

	bool getMousePressed(int button=-1) const;
	bool getKeyPressed(int key=-1) const;
	int getMouseX() const;
	int getMouseY() const;
	int getPreviousMouseX() const;
	int getPreviousMouseY() const;
	int getModifiers() const;

	//  event notification only for internal OF use
	bool notifySetup();
	bool notifyUpdate();
	bool notifyDraw();

	bool notifyKeyPressed(int key, int keycode=-1, int scancode=-1, uint32_t codepoint=0);
	bool notifyKeyReleased(int key, int keycode=-1, int scancode=-1, uint32_t codepoint=0);
	bool notifyKeyEvent(ofKeyEventArgs & keyEvent);

	bool notifyMousePressed(int x, int y, int button);
	bool notifyMouseReleased(int x, int y, int button);
	bool notifyMouseDragged(int x, int y, int button);
	bool notifyMouseMoved(int x, int y);
	bool notifyMouseScrolled(int x, int y, float scrollX, float scrollY);
	bool notifyMouseEntered(int x, int y);
	bool notifyMouseExited(int x, int y);
	bool notifyMouseEvent(ofMouseEventArgs & mouseEvent);

	void notifyTouchDown(int x, int y, int touchID);
	void notifyTouchUp(int x, int y, int touchID);
	void notifyTouchMoved(int x, int y, int touchID);
	void notifyTouchCancelled(int x, int y, int touchID);
	void notifyTouchDoubleTap(int x, int y, int touchID);
	void notifyTouchEvent(ofTouchEventArgs & touchEvent);

	bool notifyExit();
	bool notifyWindowResized(int width, int height);
	bool notifyWindowMoved(int x, int y);

	bool notifyDragEvent(ofDragInfo info);

private:
	float targetRate;
	bool bFrameRateSet;
	ofTimer timer;
	ofFpsCounter fps;

	int	currentMouseX, currentMouseY;
	int	previousMouseX, previousMouseY;
	bool bPreMouseNotSet;
	std::set<int> pressedMouseButtons;
	std::set<int> pressedKeys;
	int modifiers = 0;

	enum TimeMode{
		System,
		FixedRate,
		Filtered,
	} timeMode = System;
	std::chrono::nanoseconds fixedRateTimeNanos;
};

bool ofSendMessage(ofMessage msg);
bool ofSendMessage(std::string messageString);

ofCoreEvents & ofEvents();

template<class ListenerClass>
void ofRegisterMouseEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().mouseDragged,listener,&ListenerClass::mouseDragged,prio);
	ofAddListener(ofEvents().mouseMoved,listener,&ListenerClass::mouseMoved,prio);
	ofAddListener(ofEvents().mousePressed,listener,&ListenerClass::mousePressed,prio);
	ofAddListener(ofEvents().mouseReleased,listener,&ListenerClass::mouseReleased,prio);
	ofAddListener(ofEvents().mouseScrolled,listener,&ListenerClass::mouseScrolled,prio);
	ofAddListener(ofEvents().mouseEntered,listener,&ListenerClass::mouseEntered,prio);
	ofAddListener(ofEvents().mouseExited,listener,&ListenerClass::mouseExited,prio);
}

template<class ListenerClass>
void ofRegisterKeyEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().keyPressed, listener, &ListenerClass::keyPressed,prio);
	ofAddListener(ofEvents().keyReleased, listener, &ListenerClass::keyReleased,prio);
}

template<class ListenerClass>
void ofRegisterTouchEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().touchDoubleTap, listener, &ListenerClass::touchDoubleTap,prio);
	ofAddListener(ofEvents().touchDown, listener, &ListenerClass::touchDown,prio);
	ofAddListener(ofEvents().touchMoved, listener, &ListenerClass::touchMoved,prio);
	ofAddListener(ofEvents().touchUp, listener, &ListenerClass::touchUp,prio);
	ofAddListener(ofEvents().touchCancelled, listener, &ListenerClass::touchCancelled,prio);
}

template<class ListenerClass>
void ofRegisterGetMessages(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().messageEvent, listener, &ListenerClass::gotMessage,prio);
}

template<class ListenerClass>
void ofRegisterDragEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().fileDragEvent, listener, &ListenerClass::dragEvent,prio);
}

template<class ListenerClass>
void ofUnregisterMouseEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().mouseDragged,listener,&ListenerClass::mouseDragged,prio);
	ofRemoveListener(ofEvents().mouseMoved,listener,&ListenerClass::mouseMoved,prio);
	ofRemoveListener(ofEvents().mousePressed,listener,&ListenerClass::mousePressed,prio);
	ofRemoveListener(ofEvents().mouseReleased,listener,&ListenerClass::mouseReleased,prio);
	ofRemoveListener(ofEvents().mouseScrolled,listener,&ListenerClass::mouseScrolled,prio);
	ofRemoveListener(ofEvents().mouseEntered,listener,&ListenerClass::mouseEntered,prio);
	ofRemoveListener(ofEvents().mouseExited,listener,&ListenerClass::mouseExited,prio);
}

template<class ListenerClass>
void ofUnregisterKeyEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().keyPressed, listener, &ListenerClass::keyPressed,prio);
	ofRemoveListener(ofEvents().keyReleased, listener, &ListenerClass::keyReleased,prio);
}

template<class ListenerClass>
void ofUnregisterTouchEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().touchDoubleTap, listener, &ListenerClass::touchDoubleTap,prio);
	ofRemoveListener(ofEvents().touchDown, listener, &ListenerClass::touchDown,prio);
	ofRemoveListener(ofEvents().touchMoved, listener, &ListenerClass::touchMoved,prio);
	ofRemoveListener(ofEvents().touchUp, listener, &ListenerClass::touchUp,prio);
	ofRemoveListener(ofEvents().touchCancelled, listener, &ListenerClass::touchCancelled,prio);
}

template<class ListenerClass>
void ofUnregisterGetMessages(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().messageEvent, listener, &ListenerClass::gotMessage,prio);
}

template<class ListenerClass>
void ofUnregisterDragEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().fileDragEvent, listener, &ListenerClass::dragEvent,prio);
}
