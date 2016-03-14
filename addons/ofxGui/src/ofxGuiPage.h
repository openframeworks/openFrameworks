#pragma once
#include "ofxPanel.h"

class ofxGuiPage : public ofxPanel {

	public:

		ofxGuiPage(const ofJson& config = ofJson());
		virtual ~ofxGuiPage();

//		using ofxPanel::add;

//		template <typename GuiType>
//		GuiType* add(std::unique_ptr<GuiType> element);

		void clear();

		// TODO
//		virtual bool mouseDragged(ofMouseEventArgs & args);

	protected:

};

// TODO (what was this good for?)
//template <typename GuiType>
//GuiType* ofxGuiPage::add(std::unique_ptr<GuiType> element){

//	Element::add(element);

//	if(exclusiveToggles) {
//		setOneToggleActive();
//	}
//}
