#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

class ofxLabel: public ofxBaseGui {
public:
	ofxLabel(){label.setSerializable(false);}
    ofxLabel(ofParameter<std::string> _label, float width = defaultWidth, float height = defaultHeight);

	template<typename F>
	ofxLabel(ofReadOnlyParameter<std::string,F> _label, float width = defaultWidth, float height = defaultHeight){
		setup(_label, width, height);
	}

    virtual ~ofxLabel();

    ofxLabel * setup(ofParameter<std::string> _label, float width = defaultWidth, float height = defaultHeight);

	template<typename F>
	ofxLabel* setup(ofReadOnlyParameter<std::string,F> _label, float width, float height) {
		label.makeReferenceTo(_label);
		b.width  = width;
		b.height = height;
		setNeedsRedraw();
		_label.addListener(this,&ofxLabel::valueChanged);
		return this;
	}
    ofxLabel * setup(const std::string& labelName, std::string label, float width = defaultWidth, float height = defaultHeight);

    // Abstract methods we must implement, but have no need for!
    virtual bool mouseMoved(ofMouseEventArgs & args){return false;}
    virtual bool mousePressed(ofMouseEventArgs & args){return false;}
    virtual bool mouseDragged(ofMouseEventArgs & args){return false;}
    virtual bool mouseReleased(ofMouseEventArgs & args){return false;}
    virtual bool mouseScrolled(ofMouseEventArgs & args){return false;}

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		label.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		label.removeListener(listener,method);
	}


	std::string operator=(std::string v) { label = v; return v; }
    operator const std::string & ()       { return label; }

    ofAbstractParameter & getParameter();

protected:
    void render();
	ofReadOnlyParameter<std::string, ofxLabel> label;
    void generateDraw();
    void valueChanged(std::string & value);
    bool setValue(float mx, float my, bool bCheckBounds){return false;}
    ofPath bg;
    ofVboMesh textMesh;
};
