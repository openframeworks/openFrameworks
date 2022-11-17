#pragma once

#include "ofxToggle.h"
#include "ofxSlider.h"
#include "ofxSliderGroup.h"
#include "ofxPanel.h"
#include "ofxButton.h"
#include "ofxLabel.h"
#include "ofxInputField.h"
#include "ofTrueTypeFont.h"
#include "ofxColorPicker.h"
#include "ofEvents.h"

void ofxGuiSetFont(const std::string & fontPath,int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=true, int dpi=0);
void ofxGuiSetFont(const ofTrueTypeFontSettings & fontSettings);
void ofxGuiSetBitmapFont();

void ofxGuiSetHeaderColor(const ofColor & color);
void ofxGuiSetBackgroundColor(const ofColor & color);
void ofxGuiSetBorderColor(const ofColor & color);
void ofxGuiSetTextColor(const ofColor & color);
void ofxGuiSetFillColor(const ofColor & color);

void ofxGuiSetTextPadding(int padding);
void ofxGuiSetDefaultWidth(int width);
void ofxGuiSetDefaultHeight(int height);
void ofxGuiSetDefaultEventsPriority(ofEventOrder eventsPriority);

void ofxGuiEnableHiResDisplay();//hiDpi display
void ofxGuiDisableHiResDisplay();//hiDpi display
