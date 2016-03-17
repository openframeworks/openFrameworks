#include "ofxSlider.h"
#include "JsonConfigParser.h"
#include "ofGraphics.h"
using namespace std;
using namespace ofx;
using namespace ofx::DOM;

template<typename DataType>
ofxSlider<DataType>::ofxSlider()
	:ofxBaseGui(){

	setup();

}

template<typename DataType>
ofxSlider<DataType>::ofxSlider(const ofJson &config)
	:ofxSlider(){

	_setConfig(config);

}

template<typename DataType>
ofxSlider<DataType>::ofxSlider(ofParameter<DataType> _val, const ofJson &config)
:ofxBaseGui(){

	value.makeReferenceTo(_val);
	value.addListener(this,&ofxSlider::valueChanged);
	setup();
	_setConfig(config);

}

template<typename DataType>
ofxSlider<DataType>::ofxSlider(ofParameter<DataType> _val, float width, float height)
	:ofxBaseGui(){

	value.makeReferenceTo(_val);
	setSize(width, height);
	value.addListener(this,&ofxSlider::valueChanged);
	setup();

}

template<typename DataType>
ofxSlider<DataType>::ofxSlider(const std::string& sliderName, DataType _val, DataType _min, DataType _max, float width, float height)
	:ofxSlider(){

	value.set(sliderName,_val,_min,_max);
	value.addListener(this,&ofxSlider::valueChanged);
	setSize(width,height);

}

template<typename DataType>
ofxSlider<DataType>::~ofxSlider(){

	value.removeListener(this,&ofxSlider::valueChanged);
	ofRemoveListener(resize, this, &ofxSlider<DataType>::resized);

}

template<typename DataType>
void ofxSlider<DataType>::setup(){

	hasFocus = false;
	updateOnReleaseOnly.set("update-on-release-only", false);
	precision.set("precision", 6);
	type.set("type", ofxSliderType::STRAIGHT);
	horizontal = getWidth() > getHeight();
	ofAddListener(resize, this, &ofxSlider<DataType>::resized);
	registerMouseEvents();

}

template<typename DataType>
void ofxSlider<DataType>::_setConfig(const ofJson &config){

	ofxBaseGui::_setConfig(config);

	JsonConfigParser::parse(config, updateOnReleaseOnly);
	JsonConfigParser::parse(config, precision);

	if (config.find(type.getName()) != config.end()) {
		std::string val = config[type.getName()];
		setType(val);
	}


}

template<typename DataType>
void ofxSlider<DataType>::setMin(DataType min){
	value.setMin(min);
}

template<typename DataType>
DataType ofxSlider<DataType>::getMin(){
	return value.getMin();
}

template<typename DataType>
void ofxSlider<DataType>::setMax(DataType max){
	value.setMax(max);
}

template<typename DataType>
DataType ofxSlider<DataType>::getMax(){
	return value.getMax();
}

template<typename DataType>
void ofxSlider<DataType>::setType(const std::string& type){
	if(type == "circular"){
		setType(ofxSliderType::CIRCULAR);
	}
	else{
		setType(ofxSliderType::STRAIGHT);
	}
}

template<typename DataType>
void ofxSlider<DataType>::setType(const ofxSliderType::Type& type){
	this->type.set(type);
	setNeedsRedraw();
}

template<typename DataType>
ofxSliderType::Type ofxSlider<DataType>::getType(){
	return type;
}


template<typename DataType>
void ofxSlider<DataType>::resized(ResizeEventArgs &){
	horizontal = getWidth() > getHeight();
}

template<typename DataType>
void ofxSlider<DataType>::setPrecision(int precision){
	this->precision = precision;
}

template<typename DataType>
bool ofxSlider<DataType>::mousePressed(ofMouseEventArgs & args){

	ofxBaseGui::mousePressed(args);

	if((type == ofxSliderType::CIRCULAR) && isMouseOver()){

		ofPoint pos = screenToLocal(ofPoint(args.x, args.y));

		DataType firstClickVal = ofMap(pos.y, getShape().getHeight(), 0, 0, 1, true);
		DataType lastVal = ofMap(value, value.getMin(), value.getMax(), 0, 1, true);
		_mouseOffset = (firstClickVal - lastVal) * getShape().height;

	}

	if(updateOnReleaseOnly){
		value.disableEvents();
	}
	return setValue(args.x, args.y, true);

}

template<typename DataType>
bool ofxSlider<DataType>::mouseDragged(ofMouseEventArgs & args){

	ofxBaseGui::mouseDragged(args);

	return setValue(args.x, args.y, false);

}

template<typename DataType>
bool ofxSlider<DataType>::mouseReleased(ofMouseEventArgs & args){

	ofxBaseGui::mouseReleased(args);

	if(updateOnReleaseOnly){
		value.enableEvents();
	}
	bool attended = setValue(args.x, args.y, false);
	hasFocus = false;
	return attended;

}

template<typename DataType>
bool ofxSlider<DataType>::mouseScrolled(ofMouseEventArgs & args){

	ofxBaseGui::mouseScrolled(args);

	if(isMouseOver()){
		if(args.scrollY>0 || args.scrollY<0){
			// TODO
//			double range = getRange(value.getMin(),value.getMax(), getWidth());
//			DataType newValue = value + ofMap(args.scrollY,-1,1,-range, range);
//			newValue = ofClamp(newValue,value.getMin(),value.getMax());
//			value = newValue;
		}
		return true;
	}else{
		return false;
	}
}

template<typename DataType>
typename std::enable_if<std::is_integral<DataType>::value, DataType>::type
getRange(DataType min, DataType max, float width){
	double range = max - min;
	range /= width*4;
	return std::max(range,1.0);
}

template<typename DataType>
typename std::enable_if<std::is_floating_point<DataType>::value, DataType>::type
getRange(DataType min, DataType max, float width){
	double range = max - min;
	range /= width*4;
	return range;
}

template<typename DataType>
double ofxSlider<DataType>::operator=(DataType v){
	value = v;
	return v;
}

template<typename DataType>
ofxSlider<DataType>::operator const DataType & (){
	return value;
}

template<typename DataType>
void ofxSlider<DataType>::generateDraw(){

	if(type == ofxSliderType::STRAIGHT){

		ofxBaseGui::generateDraw();

		bar.clear();

		float valAsPct;
		if(horizontal){
			valAsPct = ofMap(value, value.getMin(), value.getMax(), 0, getWidth(), true);
		}else{
			valAsPct = ofMap(value, value.getMin(), value.getMax(), 0, getHeight(), true);
		}
		bar.setFillColor(fillColor);
		bar.setFilled(true);
		if(horizontal){
			bar.rectangle(0,0, valAsPct, getHeight());
		}else{
			bar.rectangle(0, getHeight() - valAsPct, getWidth(), valAsPct);
		}

	}
	if(type == ofxSliderType::CIRCULAR){

		float inner_r = min(getShape().width, getShape().height) / 6;
		float outer_r = min(getShape().width, getShape().height) / 2-1;

		bg.clear();
		bar.clear();

		bg.setStrokeColor(borderColor);
		bg.setStrokeWidth(1);
		bg.setFillColor(backgroundColor);
		bg.setFilled(true);
		arcStrip(bg, ofPoint(getWidth()/2, getHeight()/2), outer_r-1, inner_r+1, 1);

		float val = ofMap(value, value.getMin(), value.getMax(), 0, 1);
		bar.setFillColor(fillColor);
		bar.setFilled(true);
		arcStrip(bar, ofPoint(getWidth()/2, getHeight()/2), outer_r - 1, inner_r + 1, val);

	}

	generateText();
}


template<typename DataType>
void ofxSlider<DataType>::generateText(){

	string valStr = ofToString(value.get(), precision);
	_generateText(valStr);
}

template<>
void ofxSlider<unsigned char>::generateText(){

	string valStr = ofToString((int)value, precision);
	_generateText(valStr);
}

template<typename DataType>
void ofxSlider<DataType>::_generateText(std::string valStr){

	if(type == ofxSliderType::STRAIGHT){

		if(horizontal){
			textMesh.clear();
			if(showName){
				textMesh.append(getTextMesh(getName(), ofPoint(textPadding, getHeight() / 2 + 4)));
			}
			textMesh.append(getTextMesh(valStr, getShape().getWidth() - textPadding - getTextBoundingBox(valStr,0,0).width, getHeight() / 2 + 4));
		}else{
			textMesh.clear();
			if(showName){
				string nameStr = getName();
				while(getTextBoundingBox(nameStr, 0, 0).getWidth() + textPadding * 2 > getWidth() && nameStr.length() > 1){
					nameStr = nameStr.substr(0, nameStr.size() - 1);
				}
				textMesh.append(getTextMesh(nameStr, textPadding, textPadding + getTextBoundingBox(nameStr, 0, 0).height));
			}
			while(getTextBoundingBox(valStr, 0, 0).getWidth() + textPadding * 2 > getWidth() && valStr.length() > 1){
				valStr = valStr.substr(0, valStr.size() - 1);
			}
			textMesh.append(getTextMesh(valStr, textPadding, getHeight() - textPadding));
		}
	}
	if(type == ofxSliderType::CIRCULAR){

		textMesh.clear();
		if(showName){
			textMesh.append(getTextMesh(getName(), textPadding, getShape().height - textPadding));
		}
		textMesh.append(getTextMesh(valStr, getShape().width - textPadding - getTextBoundingBox(valStr, 0, 0).width, getShape().height - textPadding));

	}
}

template<typename DataType>
void ofxSlider<DataType>::render(){
	ofColor c = ofGetStyle().color;

	ofxBaseGui::render();

	bar.draw();

	if(showName){
		ofBlendMode blendMode = ofGetStyle().blendingMode;
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableAlphaBlending();
		}
		ofSetColor(textColor);

		bindFontTexture();
		textMesh.draw();
		unbindFontTexture();

		ofSetColor(c);
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableBlendMode(blendMode);
		}
	}
}


template<typename DataType>
bool ofxSlider<DataType>::setValue(float mx, float my, bool bCheck){

	if(isHidden()){
		hasFocus = false;
		return false;
	}

	if(bCheck){
		hasFocus = isMouseOver();
	}

	if(hasFocus){

		if(type == ofxSliderType::STRAIGHT){

			ofPoint topleft = localToScreen(ofPoint(0, 0));
			ofPoint bottomright = localToScreen(ofPoint(getWidth(), getHeight()));
			if(horizontal){
				value = ofMap(mx, topleft.x, bottomright.x, value.getMin(), value.getMax(), true);
			}else{
				value = ofMap(my, bottomright.y, topleft.y, value.getMin(), value.getMax(), true);
			}
			return true;

		}
		if(type == ofxSliderType::CIRCULAR){

			ofPoint pos = screenToLocal(ofPoint(mx,my));

			DataType res = ofMap(pos.y,
							 getHeight() - _mouseOffset,
							 - _mouseOffset,
							 value.getMin(),
							 value.getMax(),
							 true);
			value.set(res);
			return true;

		}

	}

	return false;
}

template<typename DataType>
ofAbstractParameter & ofxSlider<DataType>::getParameter(){
	return value;
}

template<typename DataType>
void ofxSlider<DataType>::valueChanged(DataType & value){
	setNeedsRedraw();
}

/*
 * adapted from ofxUI by Reza Ali (www.syedrezaali.com || syed.reza.ali@gmail.com || @rezaali)
 *
 */
template <typename DataType>
void ofxSlider <DataType>::arcStrip(ofPath & path, ofPoint center, float outer_radius, float inner_radius, float percent){
	float theta = ofMap(percent, 0, 1, 0, 360.0, true);

	{
		float x = sin(-ofDegToRad(0));
		float y = cos(-ofDegToRad(0));
		path.moveTo(center.x + outer_radius * x, center.y + outer_radius * y);
	}

	for(int i = 0; i <= theta; i += 10){
		float x = sin(-ofDegToRad(i));
		float y = cos(-ofDegToRad(i));

		path.lineTo(center.x + outer_radius * x, center.y + outer_radius * y);
	}

	{
		float x = sin(-ofDegToRad(theta));
		float y = cos(-ofDegToRad(theta));
		path.lineTo(center.x + outer_radius * x, center.y + outer_radius * y);
		path.lineTo(center.x + inner_radius * x, center.y + inner_radius * y);
	}

	for(int i = theta; i >= 0; i -= 10){
		float x = sin(-ofDegToRad(i));
		float y = cos(-ofDegToRad(i));

		path.lineTo(center.x + inner_radius * x, center.y + inner_radius * y);
	}

	{
		float x = sin(-ofDegToRad(0));
		float y = cos(-ofDegToRad(0));
		path.lineTo(center.x + inner_radius * x, center.y + inner_radius * y);
	}

	path.close();
}

template class ofxSlider<int8_t>;
template class ofxSlider<uint8_t>;
template class ofxSlider<int16_t>;
template class ofxSlider<uint16_t>;
template class ofxSlider<int32_t>;
template class ofxSlider<uint32_t>;
template class ofxSlider<int64_t>;
template class ofxSlider<uint64_t>;
template class ofxSlider<float>;
template class ofxSlider<double>;
