#include "ofxGuiPage.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiPage::ofxGuiPage() : ofxPanel(){
}

ofxGuiPage::~ofxGuiPage(){
	for(auto e: collection){
		ofRemoveListener(e->sizeChangedE,this,&ofxGuiPage::sizeChangedCB);
	}
}

void ofxGuiPage::add(ofxBaseGui * element){
	collection.push_back(element);

	ofPoint newpos = element->getPosition() + this->getPosition();
	if(bShowHeader){
		newpos.y += header + spacing;
	}
	element->setPosition(newpos);
	b.width = max(element->getShape().getRight() - b.x + 1, b.width);
	b.height = max(element->getShape().getBottom() - b.y + 1, b.height);
	element->unregisterMouseEvents();
	ofAddListener(element->sizeChangedE, this, &ofxGuiPage::sizeChangedCB);

	parameters.add(element->getParameter());
	setNeedsRedraw();
}

bool ofxGuiPage::mouseDragged(ofMouseEventArgs & args){
	if(setValue(args.x, args.y, false)){
		return true;
	}
	if(bGuiActive){
		ofMouseEventArgs a = args;
		for(auto & e: collection){
			if(e->mouseDragged(a)){
				//collection is only allowed to be moved within page boundaries
				float tmp_header = 0;
				if(bShowHeader){
					tmp_header += header + spacing;
				}
				ofPoint pos = e->getPosition();
				if(e->getShape().getLeft() < b.getLeft()){
					pos.x = b.getLeft();
				}else {
					if(e->getShape().getRight() > b.getRight()-1){
						pos.x = b.getRight()-1 - e->getWidth();
					}
				}
				if(e->getShape().getTop() < b.getTop()+tmp_header){
					pos.y = b.getTop()+tmp_header;
				}else {
					if(e->getShape().getBottom() > b.getBottom()-1){
						pos.y = b.getBottom()-1 - e->getHeight();
					}
				}
				e->setPosition(pos);
				return true;
			}
		}
	}
	return false;
}

void ofxGuiPage::setWidthElements(float w){
}

void ofxGuiPage::scaleWidthElements(float factor){
}

void ofxGuiPage::clear(){
	collection.clear();
	parameters.clear();
	b.height = spacing + spacingNextElement;
	if(bShowHeader){
		b.height += header;
	}
	sizeChangedCB();
}

void ofxGuiPage::sizeChangedCB(){
	for(auto & e : collection){
		b.width = max(e->getShape().getRight() - b.x + 1, b.width);
		b.height = max(e->getShape().getBottom() - b.y + 1, b.height);
	}
	sizeChangedE.notify(this);
	setNeedsRedraw();
}
