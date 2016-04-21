#pragma once
#include "ofxGuiGroup.h"

class ofxGuiMatrix : public ofxGuiGroup {
	public:
		struct Config : public ofxGuiGroup::Config {
			Config(){
			}
			Config(const ofxGuiGroup::Config & c) : ofxGuiGroup::Config(c){
			}
			Config(const ofxBaseGui::Config & c) : ofxGuiGroup::Config(c){
			}

			int columnCount = 0;
			float rowHeight = 20;
		};

		ofxGuiMatrix();
		ofxGuiMatrix(const ofParameterGroup & parameters, const Config & groupConfig = ofxGuiMatrix::Config(), const Config &itemConfig = ofxBaseGui::Config());
		virtual ~ofxGuiMatrix(){
		}
		virtual ofxGuiMatrix & setup(const ofParameterGroup & parameters, const Config & groupConfig = ofxGuiMatrix::Config(), const Config &itemConfig = ofxBaseGui::Config());
		virtual ofxGuiMatrix & setup(const std::string & collectionName, const Config & config = ofxGuiMatrix::Config());
		virtual ofxGuiMatrix & setup(const std::string & collectionName = "", int cols = 0, const std::string & filename = "settings.xml", float x = 10, float y = 10);
		virtual ofxGuiMatrix & setup(const ofParameterGroup & parameters, int cols = 0, const std::string & filename = "settings.xml", float x = 10, float y = 10);

		using ofxGuiGroup::add;

		void maximize();

		virtual void setWidthElements(float w);
		virtual void scaleWidthElements(float factor);

		void setColNum(int num);
		int getColNum();
		int getRowNum();

		float getElementWidth();
		float getElementHeight();

		void setElementHeight(float h);

	protected:
		virtual void add(ofxBaseGui * element);
		void updateElementWidth();
		virtual void sizeChangedCB();

		int numCol;
		float w_matrix, w_element, h_element;

};
