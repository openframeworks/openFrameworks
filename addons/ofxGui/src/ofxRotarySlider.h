#pragma once

#include "ofxSlider.h"
#include "ofParameter.h"

template <typename Type>
class ofxRotarySlider : public ofxSlider <Type> {
	public:

		ofxRotarySlider(const ofJson & config = ofJson());
		ofxRotarySlider(ofParameter <Type> _val, const ofJson & config = ofJson());

		~ofxRotarySlider();

		virtual bool mousePressed(ofMouseEventArgs & args) override;

	protected:
		virtual void render();
		virtual bool setValue(float mx, float my, bool bCheck) override;
		void generateDraw();
		void generateText();
		void valueChanged(Type & value);
		void arcStrip(ofPath & path, ofPoint center, float outer_radius, float inner_radius, float percent);
		float _mouseOffset;
};

typedef ofxRotarySlider <float> ofxFloatRotarySlider;
typedef ofxRotarySlider <int> ofxIntRotarySlider;
