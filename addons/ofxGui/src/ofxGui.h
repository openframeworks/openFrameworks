#pragma once

#include "ofxToggle.h"
#include "ofxSlider.h"
#include "ofxSliderGroup.h"
#include "ofxPanel.h"
#include "ofxButton.h"
#include "ofxLabel.h"

void ofxGuiSetFont(const string & fontPath,int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=false, int dpi=0);
void ofxGuiSetBitmapFont();

void ofxGuiSetHeaderColor(const ofColor & color);
void ofxGuiSetBackgroundColor(const ofColor & color);
void ofxGuiSetBorderColor(const ofColor & color);
void ofxGuiSetTextColor(const ofColor & color);
void ofxGuiSetFillColor(const ofColor & color);

void ofxGuiSetTextPadding(int padding);
void ofxGuiSetDefaultWidth(int width);
void ofxGuiSetDefaultHeight(int height);
