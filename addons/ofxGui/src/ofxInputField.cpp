//
//  ofxInputField.cpp
//  ofxInputField
//
//  Based on ofxInputField by Felix Lange
//
//

#include "ofxInputField.h"

#include "ofGraphics.h"

namespace{
	template<typename Type>
	typename std::enable_if<std::is_integral<Type>::value, Type>::type
	getRange(Type min, Type max, float width){
		double range = max - min;
		range /= width*4;
		return std::max(range,1.0);
	}

	template<typename Type>
	typename std::enable_if<std::is_floating_point<Type>::value, Type>::type
	getRange(Type min, Type max, float width){
		double range = max - min;
		range /= width*4;
		return range;
	}

	template<typename Type>
	std::string toString(Type t){
		return ofToString(t);
	}

	template<>
	std::string toString(uint8_t t){
		return ofToString((int) t);
	}

	template<>
	std::string toString(int8_t t){
		return ofToString((int) t);
	}

	template<>
	std::string toString(std::string t){
		return t;
	}

	bool isHexNotation(std::string const& s){
	  return s.compare(0, 2, "0x") == 0
		  && s.size() > 2
		  && s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
	}

	bool isANumber(std::string const& s){
	  return s.find_first_not_of("0123456789.", 0) == std::string::npos
			  && std::count(s.begin(), s.end(), '.') <= 1;
	}

	template<typename Type>
	Type fromString(const std::string & str){
		if(isHexNotation(str)){
			return ofHexToInt(str);
		}else if(isANumber(str)){
			return ofFromString<Type>(str);
		}else{
			throw std::exception();
		}
	}

	template<>
	uint8_t fromString(const std::string & str){
		auto ret = 0;
		if(isHexNotation(str)){
			ret = ofHexToInt(str);
		}else if(isANumber(str)){
			ret = ofFromString<int>(str);
		}else{
			throw std::exception();
		}
		return std::max(std::min(ret, 255), 0);
	}

	template<>
	int8_t fromString(const std::string & str){
		auto ret = 0;
		if(isHexNotation(str)){
			ret = ofHexToInt(str);
		}else if(isANumber(str)){
			ret = ofFromString<int>(str);
		}else{
			throw std::exception();
		}
		return std::max(std::min(ret, -127), 127);
	}

	ofMesh rectangle(const ofRectangle & r, const ofFloatColor & c){
		ofMesh mesh;
		mesh.addVertex(r.position);
		mesh.addVertex(glm::vec3(r.x + r.width, r.y, 0));
		mesh.addVertex(glm::vec3(r.x + r.width, r.y + r.height, 0));

		mesh.addVertex(glm::vec3(r.x + r.width, r.y + r.height, 0));
		mesh.addVertex(glm::vec3(r.x, r.y + r.height, 0));
		mesh.addVertex(glm::vec3(r.x, r.y, 0));

		mesh.addColor(c);
		mesh.addColor(c);
		mesh.addColor(c);

		mesh.addColor(c);
		mesh.addColor(c);
		mesh.addColor(c);

		return mesh;
	}
}

template<typename Type>
ofxInputField<Type> ofxInputField<Type>::createInsideSlider(){
	ofxInputField<Type> input;
	input.insideSlider = true;
	return input;
}

template<typename Type>
ofxInputField<Type>::ofxInputField(){
	bGuiActive = false;
	bMousePressed = false;
	mousePressedPos = -1;
	selectStartX = -1;
	selectStartPos = -1;
	selectEndPos = -1;
	selectionWidth = 0;
}

template<typename Type>
ofxInputField<Type>::ofxInputField(ofParameter<Type> _val, float width, float height){
	setup(_val,width,height);
}

template<typename Type>
ofxInputField<Type>* ofxInputField<Type>::setup(ofParameter<Type> _val, float width, float height){
	value.makeReferenceTo(_val);
	input = toString(value.get());
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	setNeedsRedraw();

	if(!insideSlider){
		registerMouseEvents();
	}
	listeners.push_back(value.newListener(this,&ofxInputField::valueChanged));
	listeners.push_back(ofEvents().charEvent.newListener(this, &ofxInputField<Type>::charPressed, OF_EVENT_ORDER_BEFORE_APP));
	listeners.push_back(ofEvents().keyPressed.newListener(this, &ofxInputField<Type>::keyPressed, OF_EVENT_ORDER_BEFORE_APP));
	return this;
}

template<typename Type>
ofxInputField<Type>* ofxInputField<Type>::setup(const std::string& _name, Type _val, Type _min, Type _max, float width, float height){
	value.set(_name,_val,_min,_max);
	return setup(value,width,height);
}

template<typename Type>
ofxInputField<Type>* ofxInputField<Type>::setup(const std::string& _name, Type _val){
	value.set(_name,_val);
	return setup(value);
}

template<typename Type>
void ofxInputField<Type>::setMin(Type min){
    value.setMin(min);
}

template<typename Type>
Type ofxInputField<Type>::getMin(){
    return value.getMin();
}

template<typename Type>
void ofxInputField<Type>::setMax(Type max){
    value.setMax(max);
}

template<typename Type>
Type ofxInputField<Type>::getMax(){
    return value.getMax();
}

template<typename Type>
void ofxInputField<Type>::calculateSelectionArea(int selectIdx1, int selectIdx2){
	std::string preSelectStr, selectStr;

	if(selectIdx1 <= selectIdx2){
		selectStartPos = selectIdx1;
		selectEndPos = selectIdx2;
	}else{
		selectStartPos = selectIdx2;
		selectEndPos = selectIdx1;
	}

	float preSelectWidth = 0;
	if(selectStartPos > 0){
		preSelectStr = ofUTF8Substring(input, 0, selectStartPos);
		preSelectWidth = getTextBoundingBox(preSelectStr,0,0).width;
	}

	auto inputWidth = getTextBoundingBox(input,0,0).width;
	inputWidth = getTextBoundingBox(input,0,0).width;
	selectStartX = b.width - textPadding - inputWidth + preSelectWidth;

	if(hasSelectedText()){
		selectStr = ofUTF8Substring(input, selectStartPos, selectEndPos - selectStartPos);
		selectionWidth = getTextBoundingBox(selectStr,0,0).width;
	}
	setNeedsRedraw();
}

template<typename Type>
bool ofxInputField<Type>::mouseMoved(ofMouseEventArgs & mouse){
	return (isGuiDrawing() || insideSlider) && b.inside(mouse);
}

template<typename Type>
bool ofxInputField<Type>::mousePressed(ofMouseEventArgs & mouse){
	if(!isGuiDrawing() && !insideSlider){
		return false;
	}
	if(b.inside(mouse)){
		if(!insideSlider || mouse.button == OF_MOUSE_BUTTON_LEFT){
			bMousePressed = true;
			if(bGuiActive){
				auto inputWidth = getTextBoundingBox(input,0,0).width;
				float cursorX = mouse.x - (b.x + b.width - textPadding - inputWidth);
				int cursorPos = round(ofMap(cursorX, 0, inputWidth, 0, ofUTF8Length(input), true));
				mousePressedPos = cursorPos;

				calculateSelectionArea(cursorPos, cursorPos);
			}else{
				calculateSelectionArea(0, ofUTF8Length(input));
				bGuiActive = true;
			}

			setNeedsRedraw();
		}
		return true;

	}else{
		if(bGuiActive){
			leaveFocus();
		}
	}
	return false;
}

template<typename Type>
bool ofxInputField<Type>::mouseDragged(ofMouseEventArgs & mouse){
	if(!isGuiDrawing() && !insideSlider){
		return false;
	}
	if(!bGuiActive){
		return false;
	}

	if(!insideSlider || mouse.button == OF_MOUSE_BUTTON_LEFT){
		auto inputWidth = getTextBoundingBox(input,0,0).width;
		float cursorX = mouse.x - (b.x + b.width - textPadding - inputWidth);
		int cursorPos = round(ofMap(cursorX, 0, inputWidth, 0, ofUTF8Length(input), true));
		calculateSelectionArea(mousePressedPos,cursorPos);
		setNeedsRedraw();
	}
	return true;
}

template<typename Type>
bool ofxInputField<Type>::mouseReleased(ofMouseEventArgs &){
	bMousePressed = false;
	return bGuiActive;
}

template<typename Type>
bool ofxInputField<Type>::mouseScrolled(ofMouseEventArgs & mouse){
	if(b.inside(mouse)){
		if(!bGuiActive){
			if(mouse.y>0 || mouse.y<0){
				double range = getRange(value.getMin(), value.getMax(), b.width);
				Type newValue = value + ofMap(mouse.y,-1,1,-range, range);
				newValue = ofClamp(newValue,value.getMin(),value.getMax());
				value = newValue;
			}
		}
		return true;
	}else{
		return false;
	}
}

template<>
bool ofxInputField<string>::mouseScrolled(ofMouseEventArgs & mouse){
	if(b.inside(mouse)){
		return true;
	}else{
		return false;
	}
}

template<typename Type>
bool ofxInputField<Type>::charPressed(uint32_t & key){
	if(!isGuiDrawing() && !insideSlider){
		return false;
	}
	if(bGuiActive && !bMousePressed){
		int newCursorIdx = -1;
		if(key >= '0' && key <= '9'){
			newCursorIdx = insertKeystroke(key);
		}else if(key == '.' ){
			newCursorIdx = insertKeystroke(key);
		}else{
			newCursorIdx = insertAlphabetic(key);
		}

		if(newCursorIdx != -1){
			//set cursor
			calculateSelectionArea(newCursorIdx,newCursorIdx);
		}
		setNeedsRedraw();
		return true;
	}
	return false;
}

template<typename Type>
bool ofxInputField<Type>::keyPressed(ofKeyEventArgs & args){
	if(!isGuiDrawing()){
		return false;
	}
	if(bGuiActive && !bMousePressed){
		auto key = args.key;
		int newCursorIdx = -1;
		if(key == OF_KEY_BACKSPACE || key == OF_KEY_DEL){
			if(hasSelectedText()){
				ofUTF8Erase(input, selectStartPos, selectEndPos - selectStartPos);
				newCursorIdx = selectStartPos;
			}else{
				int deleteIdx = -1;
				if(key == OF_KEY_BACKSPACE){
					deleteIdx = selectStartPos-1;
				}else if(key == OF_KEY_DEL){
					deleteIdx = selectStartPos;
				}

				//erase char if valid deleteIdx
				if(deleteIdx >= 0 && deleteIdx < (int)ofUTF8Length(input)){
					ofUTF8Erase(input, deleteIdx, 1);
					newCursorIdx = deleteIdx;
				}
			}
		}else if(key == OF_KEY_LEFT){
			if(hasSelectedText()){
				newCursorIdx = selectStartPos;
			}else{
				newCursorIdx = selectStartPos == 0 ? 0 : selectStartPos-1;
			}
		}else if(key == OF_KEY_RIGHT){
			if(hasSelectedText()){
				newCursorIdx = selectEndPos;
			}else{
				auto inputSize = ofUTF8Length(input);
				newCursorIdx = selectStartPos == (int)inputSize ? (int)inputSize : selectStartPos+1;
			}
		}else if(key == OF_KEY_RETURN){
			leaveFocus();
		}else if(key == OF_KEY_ESC){
			input = toString(value.get());
			leaveFocus();
		}else if(key == 'a' && args.hasModifier(OF_KEY_CONTROL)){
			calculateSelectionArea(0, ofUTF8Length(input));
		}else if(key == 'c' && args.hasModifier(OF_KEY_CONTROL)){
			auto selectLen = selectEndPos - selectStartPos;
			if(selectLen>0){
				auto selection = ofUTF8Substring(input, selectStartPos, selectLen);
				ofSetClipboardString(selection);
			}
		}else if(key == 'v' && args.hasModifier(OF_KEY_CONTROL)){
			auto selectLen = selectEndPos - selectStartPos;
			newCursorIdx = selectStartPos;
			if(selectLen>0){
				ofUTF8Erase(input, newCursorIdx, selectLen);
			}
			auto clipboard = ofGetClipboardString();
			for(auto c: ofUTF8Iterator(clipboard)){
				ofUTF8Insert(input, newCursorIdx, c);
				newCursorIdx+=1;
			}
		}else if(key == 'x' && args.hasModifier(OF_KEY_CONTROL)){
			auto selectLen = selectEndPos - selectStartPos;
			if(selectLen>0){
				auto selection = ofUTF8Substring(input, selectStartPos, selectLen);
				ofSetClipboardString(selection);
				newCursorIdx = selectStartPos;
				ofUTF8Erase(input, newCursorIdx, selectLen);
			}
		}else if(key == OF_KEY_END){
			newCursorIdx = ofUTF8Length(input);
		}else if(key == OF_KEY_HOME){
			newCursorIdx = 0;
		}

		if(newCursorIdx != -1){
			//set cursor
			calculateSelectionArea(newCursorIdx,newCursorIdx);
		}
		return true;
	}
	return false;
}


template<typename Type>
int ofxInputField<Type>::insertKeystroke(uint32_t character){
	if(hasSelectedText()){
		ofUTF8Erase(input, selectStartPos, selectEndPos - selectStartPos);
	}
	ofUTF8Insert(input, selectStartPos, character);
	setNeedsRedraw();
	return selectStartPos + 1;
}

template<typename Type>
int ofxInputField<Type>::insertAlphabetic(uint32_t character){
	if(character == 'x' || character == 'a' || character == 'b' || character=='c' || character=='d' || character=='e' || character=='f'){
	   return insertKeystroke(character);
	}else{
		return -1; //cursor or selection area stay the same
	}
}

template<>
int ofxInputField<string>::insertAlphabetic(uint32_t character){
	return insertKeystroke(character);
}

template<typename Type>
Type ofxInputField<Type>::operator=(Type v){
	value = v;
	return v;
}

template<typename Type>
ofxInputField<Type>::operator const Type & (){
	return value;
}

template<typename Type>
bool ofxInputField<Type>::hasSelectedText(){
	return selectStartPos != selectEndPos;
}

template<typename Type>
void ofxInputField<Type>::generateDraw(){
	bg.clear();

	bg.append(rectangle(b, thisBackgroundColor));

	if(bGuiActive){
		if(hasSelectedText()){
			ofRectangle selection(selectStartX+b.x, b.y+1, selectionWidth, b.height-2);
			bg.append(rectangle(selection, thisFillColor));
		}else if(!blinkingCursor){
			ofRectangle cursor(selectStartX+b.x, b.y, 1, b.height);
			bg.append(rectangle(cursor, thisTextColor));
		}
	}

	auto input = this->input;
	if(!bGuiActive && !containsValidValue()){
		input = toString(value);
	}
	textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	auto inputWidth = getTextBoundingBox(input,0,0).width;
	textMesh.append(getTextMesh(input, b.x + b.width - textPadding - inputWidth, b.y + b.height / 2 + 4));
	for(auto & c: textMesh.getVertices()){
		textMesh.addColor(thisTextColor);
	}
}

template<typename Type>
void ofxInputField<Type>::render(){
	bg.draw();

	if(!insideSlider && errorTime>0 && !containsValidValue()){
		auto now = ofGetElapsedTimef();
		auto pct = (now - errorTime) / 0.5;
		if(pct<1){
			for(size_t i=0;i<originalColors.size();i++){
				bg.getColors()[i] = ofFloatColor::darkRed.getLerped(originalColors[i], pct);
			}
		}else{
			bg.getColors() = originalColors;
			errorTime = 0;
		}
	}

	if(bGuiActive && !hasSelectedText() && blinkingCursor){
		drawCursor();
	}

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

template<typename Type>
void ofxInputField<Type>::drawCursor(){
	if(!blinkingCursor || (int(ofGetElapsedTimef()*2) % 2) == 0){
		ofPushStyle();
		ofSetColor(thisTextColor);
		ofDrawLine( selectStartX+b.x, b.y, selectStartX+b.x, b.y+b.height );
		ofPopStyle();
	}
}

template<typename Type>
void ofxInputField<Type>::setBlinkingCursor(bool blink){
	blinkingCursor = blink;
	setNeedsRedraw();
}

template<typename Type>
bool ofxInputField<Type>::setValue(float mx, float my, bool bCheck){
	return false;
}

template<typename Type>
ofAbstractParameter & ofxInputField<Type>::getParameter(){
	return value;
}

template<typename Type>
void ofxInputField<Type>::parseInput(){
	try{
		Type tmpVal = fromString<Type>(input);
		if(tmpVal < getMin()){
			tmpVal = getMin();
		}else if(tmpVal > getMax()){
			tmpVal = getMax();
		}
		value = tmpVal;
		validValue = true;
	}catch(...){
		if(!insideSlider){
			originalColors = bg.getColors();
			errorTime = ofGetElapsedTimef();
		}
		validValue = false;
	}
}

template<>
void ofxInputField<string>::parseInput(){
	value = input;
}

template<typename Type>
void ofxInputField<Type>::valueChanged(Type & value){
	input = toString(value);
	if(bGuiActive){
		int cursorPos = ofUTF8Length(input);
		calculateSelectionArea(cursorPos,cursorPos);
	}
    setNeedsRedraw();
}

template<typename Type>
void ofxInputField<Type>::leaveFocus(){
	bGuiActive = false;
	parseInput();
	setNeedsRedraw();
	leftFocus.notify(this);
}

template<typename Type>
bool ofxInputField<Type>::containsValidValue() const{
	return validValue;
}

template class ofxInputField<int8_t>;
template class ofxInputField<uint8_t>;
template class ofxInputField<int16_t>;
template class ofxInputField<uint16_t>;
template class ofxInputField<int32_t>;
template class ofxInputField<uint32_t>;
template class ofxInputField<int64_t>;
template class ofxInputField<uint64_t>;
template class ofxInputField<float>;
template class ofxInputField<double>;
template class ofxInputField<std::string>;
