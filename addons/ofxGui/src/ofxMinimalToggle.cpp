#include "ofxMinimalToggle.h"
#include "ofGraphics.h"
using namespace std;

ofxMinimalToggle::ofxMinimalToggle() :
	ofxToggle(){
	thisBorderColor = thisFillColor;
}

ofxMinimalToggle::ofxMinimalToggle(ofParameter <bool> val, const Config & config)
:ofxToggle(val,config){
	thisBorderColor = thisFillColor;
    if(b.width == 0) {
        b. width = getTextWidth(val.getName(), config.shape.height);
    }
}

ofxMinimalToggle::~ofxMinimalToggle(){
}

ofxMinimalToggle & ofxMinimalToggle::setup(ofParameter <bool> val, float width, float height){
    ofxToggle::setup(val,width,height);
    if(b.width == 0){
        b.width = getTextWidth(val.getName(), height);
    }

	return *this;

}

ofxMinimalToggle & ofxMinimalToggle::setup(const string & toggleName, bool val, float width, float height){
    value.set(toggleName, val);
	return setup(value, width, height);
}

void ofxMinimalToggle::generateDraw(){
	checkboxRect.set(1, 1, b.width - 2, b.height - 2);

	bg.clear();
	bg.setFillColor(thisBackgroundColor);
	bg.rectangle(b.getPosition() + checkboxRect.getTopLeft(), checkboxRect.width, checkboxRect.height);

	fg.clear();
	fg.setFilled(true);
	fg.setFillColor(thisFillColor);
	fg.rectangle(b.getPosition() + checkboxRect.getTopLeft(), checkboxRect.width, checkboxRect.height);

	border.clear();
	border.setFilled(false);
	border.setStrokeWidth(1);
	border.setStrokeColor(thisBorderColor);
	border.rectangle(b.getPosition() + checkboxRect.getTopLeft(), checkboxRect.width, checkboxRect.height);

    if(bShowName){
        textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
    }
}

void ofxMinimalToggle::render(){

	bg.draw();
	if(value){
		fg.draw();
	}
	border.draw();

    if(bShowName){
        ofColor c = ofGetStyle().color;
        ofBlendMode blendMode = ofGetStyle().blendingMode;
        if(blendMode != OF_BLENDMODE_ALPHA){
            ofEnableAlphaBlending();
        }
        ofSetColor(thisTextColor);

        bindFontTexture();
        textMesh.draw();
        unbindFontTexture();

        ofSetColor(c);
        if(blendMode != OF_BLENDMODE_ALPHA){
            ofEnableBlendMode(blendMode);
        }
    }
}

float ofxMinimalToggle::getTextWidth(std::string text, float _height){
	float _width = 0;
	ofVboMesh mesh = getTextMesh(text, 0, _height / 2 + 4);
	for(unsigned int i = 0; i < mesh.getVertices().size(); i++){
		if(mesh.getVertex(i).x > _width){
			_width = mesh.getVertex(i).x;
		}
	}
	_width += textPadding * 2;
	return _width;
}

