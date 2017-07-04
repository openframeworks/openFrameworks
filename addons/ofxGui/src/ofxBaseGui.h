#pragma once

#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofParameter.h"
#include "ofTrueTypeFont.h"
#include "ofBitmapFont.h"

class ofxBaseGui {
	public:
		ofxBaseGui();

		virtual ~ofxBaseGui();
		void draw();

		void saveToFile(const std::string& filename);
		void loadFromFile(const std::string& filename);

		template<class T>
		void saveTo(T & serializer){
			ofSerialize(serializer, getParameter());
		}

		template<class T>
		void loadFrom(T & serializer){
			ofDeserialize(serializer, getParameter());
		}

		std::string getName();
		void setName(const std::string& name);

		virtual void setPosition(const ofPoint & p);
		virtual void setPosition(float x, float y);
		virtual void setSize(float w, float h);
		virtual void setShape(ofRectangle r);
		virtual void setShape(float x, float y, float w, float h);

		ofPoint getPosition() const;
		ofRectangle getShape() const;
		float getWidth() const;
		float getHeight() const;

		ofColor getHeaderBackgroundColor() const;
		ofColor getBackgroundColor() const;
		ofColor getBorderColor() const;
		ofColor getTextColor() const;
		ofColor getFillColor() const;

		void setHeaderBackgroundColor(const ofColor & color);
		void setBackgroundColor(const ofColor & color);
		void setBorderColor(const ofColor & color);
		void setTextColor(const ofColor & color);
		void setFillColor(const ofColor & color);

		static void setDefaultHeaderBackgroundColor(const ofColor & color);
		static void setDefaultBackgroundColor(const ofColor & color);
		static void setDefaultBorderColor(const ofColor & color);
		static void setDefaultTextColor(const ofColor & color);
		static void setDefaultFillColor(const ofColor & color);

		static void setDefaultTextPadding(int padding);
		static void setDefaultWidth(int width);
		static void setDefaultHeight(int height);

		static void setDefaultEventsPriority(ofEventOrder eventsPriority);

		static void loadFont(const std::string& filename, int fontsize, bool _bAntiAliased = true, bool _bFullCharacterSet = false, int dpi = 0);
		static void loadFont(const ofTrueTypeFont::Settings & fontSettings);
		static void setUseTTF(bool bUseTTF);

		void registerMouseEvents();
		void unregisterMouseEvents();

		virtual void sizeChangedCB();
		virtual void setParent(ofxBaseGui * parent);
		ofxBaseGui * getParent();

		virtual ofAbstractParameter & getParameter() = 0;
		virtual bool mouseMoved(ofMouseEventArgs & args) = 0;
		virtual bool mousePressed(ofMouseEventArgs & args) = 0;
		virtual bool mouseDragged(ofMouseEventArgs & args) = 0;
		virtual bool mouseReleased(ofMouseEventArgs & args) = 0;
		virtual bool mouseScrolled(ofMouseEventArgs & args) = 0;
		virtual void mouseEntered(ofMouseEventArgs &){
		}
		virtual void mouseExited(ofMouseEventArgs &){
		}

	protected:
		virtual void render() = 0;
		virtual bool setValue(float mx, float my, bool bCheckBounds) = 0;
		virtual void generateDraw() = 0;

		bool isGuiDrawing();
		void bindFontTexture();
		void unbindFontTexture();
		ofMesh getTextMesh(const std::string & text, float x, float y);
		ofRectangle getTextBoundingBox(const std::string & text, float x, float y);

		ofxBaseGui * parent;

		ofRectangle b;
		static ofTrueTypeFont font;
		static bool fontLoaded;
		static bool useTTF;
		static ofBitmapFont bitmapFont;

		static ofColor headerBackgroundColor;
		static ofColor backgroundColor;
		static ofColor borderColor;
		static ofColor textColor;
		static ofColor fillColor;

		ofColor thisHeaderBackgroundColor;
		ofColor thisBackgroundColor;
		ofColor thisBorderColor;
		ofColor thisTextColor;
		ofColor thisFillColor;

		static int textPadding;
		static int defaultWidth;
		static int defaultHeight;
		static ofEventOrder defaultEventsPriority;

		static std::string saveStencilToHex(const ofImage & img);
		static void loadStencilFromHex(ofImage & img, unsigned char * data);

		void setNeedsRedraw();

	private:
		bool needsRedraw;
		unsigned long currentFrame;
		bool bRegisteredForMouseEvents;
		//std::vector<ofEventListener> coreListeners;
};
