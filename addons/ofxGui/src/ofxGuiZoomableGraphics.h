#pragma once

#include "ofxGuiGraphics.h"
#include "ofFbo.h"

class ofxGuiZoomableGraphics : public ofxGuiGraphics {
	public:
        ofxGuiZoomableGraphics();
        ofxGuiZoomableGraphics(string graphicsName, ofBaseDraws * graphics = 0, float w = 0, float h = 0);
        ofxGuiZoomableGraphics(ofBaseDraws * tex, float w = defaultWidth, float h = 0);
        virtual ~ofxGuiZoomableGraphics();

        ofxGuiZoomableGraphics * setup(string graphicsName = "", ofBaseDraws * graphics = 0, float w = 0, float h = 0);

		virtual bool mouseMoved(ofMouseEventArgs & args){
			return false;
		}
		virtual bool mousePressed(ofMouseEventArgs & args);
		virtual bool mouseDragged(ofMouseEventArgs & args);
		virtual bool mouseReleased(ofMouseEventArgs & args);
		virtual bool mouseScrolled(ofMouseEventArgs & args);

		virtual void setSize(float w, float h, bool callBack = true);
		virtual void setShape(float x, float y, float w, float h, bool callBack = true);
		virtual void setShape(ofRectangle r, bool callBack = true);

		virtual ofAbstractParameter & getParameter();

	protected:

		bool setValue(float mx, float my, bool bCheckBounds){
			return false;
		}
		virtual void render();
		virtual void generateDraw();

		void setZoomFactor(int factor);
		ofPoint addZoom(ofPoint p);
		float addZoom(float p);
		ofPoint removeZoom(ofPoint p);

		int zoom_factor;
		float zoom_speed;
		ofPoint zoom_point, zoom_point_scaled, zoom_point_offset;
		ofPoint zoom_translation;

		bool dragging_dst;
		ofPoint last_mouse;

		ofFbo contentFbo;

};
