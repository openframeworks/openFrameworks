#include "ofxGuiMatrix.h"
#include "ofGraphics.h"
using namespace std;

ofxGuiMatrix::ofxGuiMatrix() :
	ofxGuiGroup()
	,
	numCol(Config().columnCount)
	,
	w_matrix(Config().shape.width)
	,
	w_element(0)
	,
	h_element(Config().rowHeight){
	sizeChangedCB();
	setNeedsRedraw();
}

ofxGuiMatrix::ofxGuiMatrix(const ofParameterGroup & parameters, const Config & config) :
	ofxGuiGroup(parameters, config)
	,
	numCol(config.columnCount)
	,
	w_matrix(config.shape.width)
	,
	w_element(0)
	,
	h_element(config.rowHeight){
	sizeChangedCB();
	setNeedsRedraw();
}

ofxGuiMatrix::ofxGuiMatrix(const ofParameterGroup & parameters, int cols, const std::string & filename, float x, float y) :
	ofxGuiGroup(parameters, filename, x, y)
	,
	w_matrix(Config().shape.width)
	,
	w_element(0)
	,
	h_element(Config().rowHeight){
	setColNum(cols);
	sizeChangedCB();
	setNeedsRedraw();
}

ofxGuiMatrix & ofxGuiMatrix::setup(const ofParameterGroup & parameters, const Config & config){
	ofxGuiGroup::setup(parameters, config);
	setColNum(config.columnCount);
	setElementHeight(config.rowHeight);
	w_matrix = b.width;
	return *this;
}

ofxGuiMatrix & ofxGuiMatrix::setup(const std::string & collectionName, int cols, const std::string & filename, float x, float y){
	setColNum(cols);
	ofxGuiGroup::setup(collectionName, filename, x, y);
	w_matrix = b.width;
	return *this;
}

ofxGuiMatrix & ofxGuiMatrix::setup(const ofParameterGroup & _parameters, int cols, const std::string & filename, float x, float y){
	setColNum(cols);
	ofxGuiGroup::setup(_parameters, filename, x, y);
	w_matrix = b.width;
	return *this;
}

void ofxGuiMatrix::add(ofxBaseGui * element){
	collection.push_back(element);

	element->unregisterMouseEvents();

	parameters.add(element->getParameter());
	sizeChangedCB();
}

void ofxGuiMatrix::setWidthElements(float w){
	if((int)b.width != (int)w){
		scaleWidthElements(w / b.width);
	}
}

void ofxGuiMatrix::scaleWidthElements(float factor){

	w_matrix = this->b.getWidth() * factor;

	sizeChangedCB();
	setNeedsRedraw();

}

void ofxGuiMatrix::maximize(){
	minimized = false;
	b.height += (h_element + spacing) * getRowNum();
	setNeedsRedraw();
}

void ofxGuiMatrix::sizeChangedCB(){
	updateElementWidth();
	float x, y;
	x = b.x;
	y = b.y + spacing + spacingFirstElement;
	if(bShowHeader){
		y += header;
	}

	x += b.width - w_matrix;

	for(unsigned int i = 0; i < collection.size(); i++){
		int x_e = i;
		if(numCol > 0){
			x_e = x_e % numCol;
		}
		int y_e = 0;
		if(numCol > 0){
			y_e = (int)(i / numCol);
		}

		collection[i]->setSize(w_element, h_element);

		if(i == 0){
			collection[i]->setPosition(x, y);
		}else{
			ofRectangle first_shape = collection[0]->getShape();
			collection[i]->setPosition(first_shape.x + (w_element + spacing) * x_e, first_shape.y + (h_element + spacing) * y_e);
		}

	}

	b.height = y + (h_element + spacing) * getRowNum() + spacing - b.y;

	setNeedsRedraw();
}

void ofxGuiMatrix::updateElementWidth(){
	w_element = w_matrix / numCol - spacing;
}

void ofxGuiMatrix::setColNum(int num){
	numCol = num;
}

int ofxGuiMatrix::getColNum(){
	return numCol;
}

int ofxGuiMatrix::getRowNum(){
	if(collection.size() > 0){
		if(numCol == 0){
			return 1;
		}else{
			return (int)((collection.size() - 1) / numCol + 1);
		}
	}else{
		return 0;
	}

}

void ofxGuiMatrix::setElementHeight(float h){
	h_element = h;
}

