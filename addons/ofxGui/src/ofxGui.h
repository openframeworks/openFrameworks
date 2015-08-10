#pragma once

#include "ofxToggle.h"
#include "ofxSlider.h"
#include "ofxSliderGroup.h"
#include "ofxPanel.h"
#include "ofxButton.h"
#include "ofxLabel.h"
#include "ofxMinimalButton.h"
#include "ofxMinimalToggle.h"
#include "ofxRotarySlider.h"
#include "ofxGuiSpacer.h"
#include "ofxGuiMatrix.h"
#include "ofxGuiPage.h"
#include "ofxTabbedPages.h"
#include "ofxValuePlotter.h"
#include "ofxFpsPlotter.h"

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
