#include "ofxMinimalToggle.h"
#include "ofGraphics.h"
using namespace std;

ofxMinimalToggle::ofxMinimalToggle() :
	ofxToggle(){
	thisBorderColor = thisFillColor;
}

ofxMinimalToggle::ofxMinimalToggle(ofParameter <bool> _bVal, const Config & config){
	thisBorderColor = thisFillColor;
	setup(_bVal, config.shape.width, config.shape.height);
}

ofxMinimalToggle::~ofxMinimalToggle(){
	value.removeListener(this, &ofxMinimalToggle::valueChanged);
}

ofxMinimalToggle & ofxMinimalToggle::setup(ofParameter <bool> _bVal, float width, float height){
	if(width == 0){
		width = getTextWidth(_bVal.getName(), height);
	}
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	value.makeReferenceTo(_bVal);

	value.addListener(this, &ofxMinimalToggle::valueChanged);
	registerMouseEvents();
	setNeedsRedraw();

	return *this;

}

ofxMinimalToggle & ofxMinimalToggle::setup(const string & toggleName, bool _bVal, float width, float height){
	value.set(toggleName, _bVal);
	return setup(value, width, height);
}


bool ofxMinimalToggle::mouseMoved(ofMouseEventArgs & args){
	return ofxToggle::mouseMoved(args);
}

bool ofxMinimalToggle::mousePressed(ofMouseEventArgs & args){
	return ofxToggle::mousePressed(args);
}

bool ofxMinimalToggle::mouseDragged(ofMouseEventArgs & args){
	return ofxToggle::mouseDragged(args);
}

bool ofxMinimalToggle::mouseReleased(ofMouseEventArgs & args){
	return ofxToggle::mouseReleased(args);
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

bool ofxMinimalToggle::setValue(float mx, float my, bool bCheck){
	return ofxToggle::setValue(mx, my, bCheck);
}

float ofxMinimalToggle::getTextWidth(string text, float _height){
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

void ofxMinimalToggle::valueChanged(bool & value){
	ofxToggle::valueChanged(value);
}
