#pragma once

#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofParameter.h"
#include "ofTrueTypeFont.h"
#include "ofBitmapFont.h"

#include "ofx/DOM/Element.h"

using namespace ofx;
using namespace ofx::DOM;

enum TextAlignment{
	Left,
	Right,
	Centered
};

class ofxBaseGui : public ofx::DOM::Element {
	public:

		ofxBaseGui(const ofJson & config = ofJson());

		void setup(const ofJson & config = ofJson());

		virtual ~ofxBaseGui();
		ofxBaseGui(const ofxBaseGui &) = delete;
		ofxBaseGui & operator=(const ofxBaseGui &) = delete;

		void saveToFile(const std::string& filename);
		void loadFromFile(const std::string& filename);

		void setDefaultSerializer(std::shared_ptr <ofBaseFileSerializer> serializer);

		virtual void saveTo(ofBaseSerializer & serializer);
		virtual void loadFrom(ofBaseSerializer & serializer);

		std::string getName();
		void setName(const std::string& name);

		virtual void setTextAlignment(TextAlignment textLayout=TextAlignment::Left);
		TextAlignment getTextAlignment() const;

		ofColor getHeaderBackgroundColor() const;
		ofColor getBackgroundColor() const;
		ofColor getBorderColor() const;
		ofColor getTextColor() const;
		ofColor getFillColor() const;
		bool getShowName() const;

		void setHeaderBackgroundColor(const ofColor & color);
		void setBackgroundColor(const ofColor & color);
		void setBorderColor(const ofColor & color);
		void setTextColor(const ofColor & color);
		void setFillColor(const ofColor & color);
		void setBorderWidth(const float &width);

		static void setDefaultHeaderBackgroundColor(const ofColor & color);
		static void setDefaultBackgroundColor(const ofColor & color);
		static void setDefaultBorderColor(const ofColor & color);
		static void setDefaultTextColor(const ofColor & color);
		static void setDefaultFillColor(const ofColor & color);

		static void setDefaultTextPadding(int padding);
		static void setDefaultWidth(int width);
		static void setDefaultHeight(int height);

		void setShowName(bool show);

		virtual ofAbstractParameter & getParameter(){return parameter;}
		static void loadFont(const std::string& filename, int fontsize, bool _bAntiAliased = true, bool _bFullCharacterSet = false, int dpi = 0);
		static void setUseTTF(bool bUseTTF);

		/// \returns true if the pointer is over this Widget.
		bool isPointerOver() const;

		/// \returns true if the pointer is down on this Widget.
		bool isPointerDown() const;

		/// \brief Enable or disable the Widget as a drop target.
		/// \param dropTarget True iff this Widget is a drop target.
		void setDropTarget(bool dropTarget);

		/// \brief Determine if this Widget is a drop target.
		/// \returns true iff this Widget is a drop target.
		bool isDropTarget() const;

		/// \brief Set draggability for this Widget.
		/// \param draggable True iff draggability is enabled.
		void setDraggable(bool draggable);

		/// \brief Determine if draggability is enabled for this Widget.
		/// \returns true iff the draggability is enabled.
		bool isDraggable() const;

		/// \brief Determine if this Widget is being dragged.
		/// \returns true if this Widget is being dragged.
		bool isDragging() const;

	protected:

		virtual void generateDraw();
		virtual void render();

		void registerPointerEvents();
		void unregisterPointerEvents();

		template <class ListenerClass>
		void _registerPointerEvents(ListenerClass* obj, bool useCapture = false, int priority = OF_EVENT_ORDER_AFTER_APP);
		template <class ListenerClass>
		void _unregisterPointerEvents(ListenerClass* obj, bool useCapture = false, int priority = OF_EVENT_ORDER_AFTER_APP);

		virtual void pointerMoved(PointerUIEventArgs& e){}
		virtual void pointerDragged(PointerUIEventArgs& e){}
		virtual void pointerPressed(PointerUIEventArgs& e){}
		virtual void pointerReleased(PointerUIEventArgs& e){}
		virtual void pointerEntered(PointerUIEventArgs& e){}
		virtual void pointerExited(PointerUIEventArgs& e){}
		virtual void pointerScrolled(PointerUIEventArgs& e){}

		virtual void onPointerEvent(PointerUIEventArgs &e);
		/// \brief Default callback for built-in events, including dragging.
		virtual void onPointerCaptureEvent(PointerCaptureUIEventArgs& e);

		virtual void processConfig(const ofJson & config);
		virtual bool setValue(float mx, float my){return false;}
		void bindFontTexture();
		void unbindFontTexture();
		ofMesh getTextMesh(const std::string & text, ofPoint p);
		ofMesh getTextMesh(const std::string & text, float x, float y);
		ofRectangle getTextBoundingBox(const std::string & text, float x, float y);
		float getTextWidth(const std::string & text, float height);

		static ofTrueTypeFont font;
		static bool fontLoaded;
		static bool useTTF;
		static ofBitmapFont bitmapFont;
		std::shared_ptr <ofBaseFileSerializer> serializer;

		// TODO should be in different class -->
		static ofColor defaultHeaderBackgroundColor;
		static ofColor defaultBackgroundColor;
		static ofColor defaultBorderColor;
		static ofColor defaultTextColor;
		static ofColor defaultFillColor;
		static float defaultBorderWidth;

		// TODO make textPadding ofxGuiAttribute
		static int textPadding;

		static int defaultWidth;
		static int defaultHeight;
		// TODO should be in different class <--

		static std::string saveStencilToHex(const ofImage & img);
		static void loadStencilFromHex(ofImage & img, unsigned char * data);


		/// \brief True if the Widget is a target for dragged Widgets.
		bool _isDropTarget = false;

		/// \brief True if the Widget is configured to be dragged.
		bool _isDraggable = false;

		/// \brief True if the widget is currently being dragged.
		bool _isDragging = false;

		/// \brief True if the pointer is over the widget.
		bool _isPointerOver = false;

		ofPath bg, border;

		ofParameter<ofColor> headerBackgroundColor;
		ofParameter<ofColor> backgroundColor;
		ofParameter<ofColor> borderColor;
		ofParameter<ofColor> textColor;
		ofParameter<ofColor> fillColor;
		ofParameter<float> borderWidth;
		ofParameter<TextAlignment> textAlignment;
		ofParameter<bool> showName;

		bool registeredForPointerEvents;

		ofParameter<void> parameter;

};

template <class ListenerClass>
void ofxBaseGui::_registerPointerEvents(ListenerClass* obj, bool useCapture, int priority){
	ofAddListener(pointerOver.event(useCapture), obj, &ListenerClass::pointerMoved, priority);
	ofAddListener(pointerMove.event(useCapture), obj, &ListenerClass::pointerDragged, priority);
	ofAddListener(pointerScroll.event(useCapture), obj, &ListenerClass::pointerScrolled, priority);
	ofAddListener(pointerEnter.event(useCapture), obj, &ListenerClass::pointerEntered, priority);
	ofAddListener(pointerOut.event(useCapture), obj, &ListenerClass::pointerExited, priority);
	ofAddListener(pointerLeave.event(useCapture), obj, &ListenerClass::pointerExited, priority);
	ofAddListener(pointerDown.event(useCapture), obj, &ListenerClass::pointerPressed, priority);
	ofAddListener(pointerUp.event(useCapture), obj, &ListenerClass::pointerReleased, priority);

	ofAddListener(pointerOver.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	ofAddListener(pointerMove.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	ofAddListener(pointerScroll.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	ofAddListener(pointerEnter.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	ofAddListener(pointerOut.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	ofAddListener(pointerLeave.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	ofAddListener(pointerDown.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	ofAddListener(pointerUp.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);

	ofAddListener(gotPointerCapture.event(useCapture), obj, &ListenerClass::onPointerCaptureEvent, priority);
	ofAddListener(lostPointerCapture.event(useCapture), obj, &ListenerClass::onPointerCaptureEvent, priority);

}

template <class ListenerClass>
void ofxBaseGui::_unregisterPointerEvents(ListenerClass* obj, bool useCapture, int priority){
	 ofRemoveListener(pointerOver.event(useCapture), obj, &ListenerClass::pointerMoved, priority);
	 ofRemoveListener(pointerMove.event(useCapture), obj, &ListenerClass::pointerDragged, priority);
	 ofRemoveListener(pointerScroll.event(useCapture), obj, &ListenerClass::pointerScrolled, priority);
	 ofRemoveListener(pointerEnter.event(useCapture), obj, &ListenerClass::pointerEntered, priority);
	 ofRemoveListener(pointerOut.event(useCapture), obj, &ListenerClass::pointerExited, priority);
	 ofRemoveListener(pointerLeave.event(useCapture), obj, &ListenerClass::pointerExited, priority);
	 ofRemoveListener(pointerDown.event(useCapture), obj, &ListenerClass::pointerPressed, priority);
	 ofRemoveListener(pointerUp.event(useCapture), obj, &ListenerClass::pointerReleased, priority);

	 ofRemoveListener(pointerOver.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	 ofRemoveListener(pointerMove.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	 ofRemoveListener(pointerScroll.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	 ofRemoveListener(pointerEnter.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	 ofRemoveListener(pointerOut.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	 ofRemoveListener(pointerLeave.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	 ofRemoveListener(pointerDown.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);
	 ofRemoveListener(pointerUp.event(useCapture), obj, &ListenerClass::onPointerEvent, priority);

	 ofRemoveListener(gotPointerCapture.event(useCapture), obj, &ListenerClass::onPointerCaptureEvent, priority);
	 ofRemoveListener(lostPointerCapture.event(useCapture), obj, &ListenerClass::onPointerCaptureEvent, priority);

}
