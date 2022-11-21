//
//  ofxInputField.cpp
//  ofxInputField
//
//  Based on ofxInputField by Felix Lange
//
//

#include "ofxInputField.h"

#include "ofGraphics.h"

using namespace std;

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
	  return s.find_first_not_of("0123456789.-e", 0) == std::string::npos
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

//-----------------------------------------------------------
template<typename Type>
ofxInputField<Type>::ofxInputField(){
}

//-----------------------------------------------------------
template<typename Type>
ofxInputField<Type>::ofxInputField(ofParameter<Type> _val, float width, float height){
	setup(_val,width,height);
}

//-----------------------------------------------------------
template<typename Type>
ofxInputField<Type>* ofxInputField<Type>::setup(ofParameter<Type> _val, float width, float height){
	value.makeReferenceTo(_val);
	visibleInput = input = toString(value.get());
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	setNeedsRedraw();

	if(!insideSlider){
		registerMouseEvents();
	}
	listeners.push(value.newListener(this,&ofxInputField::valueChanged));
	listeners.push(ofEvents().charEvent.newListener(this, &ofxInputField<Type>::charPressed, OF_EVENT_ORDER_BEFORE_APP));
	listeners.push(ofEvents().keyPressed.newListener(this, &ofxInputField<Type>::keyPressed, OF_EVENT_ORDER_BEFORE_APP));
	return this;
}

//-----------------------------------------------------------
template<typename Type>
ofxInputField<Type>* ofxInputField<Type>::setup(const std::string& _name, Type _val, Type _min, Type _max, float width, float height){
	value.set(_name,_val,_min,_max);
	return setup(value,width,height);
}

//-----------------------------------------------------------
template<typename Type>
ofxInputField<Type>* ofxInputField<Type>::setup(const std::string& _name, Type _val){
	value.set(_name,_val);
	return setup(value);
}

//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::setMin(Type min){
    value.setMin(min);
}

//-----------------------------------------------------------
template<typename Type>
Type ofxInputField<Type>::getMin(){
    return value.getMin();
}

//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::setMax(Type max){
    value.setMax(max);
}

//-----------------------------------------------------------
template<typename Type>
Type ofxInputField<Type>::getMax(){
    return value.getMax();
}

//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::calculateSelectionArea(int selectIdx1, int selectIdx2){
	selectStartPos = selectIdx1;
	selectEndPos = selectIdx2;

	if(selectEndPos>visibleInputEnd){
		visibleInputEnd = selectEndPos;
	}else if(std::max(selectEndPos-1,0)<visibleInputStart){
		visibleInputStart = std::max(selectEndPos-1,0);
	}

	visibleInput = ofUTF8Substring(input, visibleInputStart, visibleInputEnd - visibleInputStart);
	auto substrWidth = getTextBoundingBox(visibleInput,0,0).width;
	auto totalTextWidth = b.width - textPadding * 2;

	if(substrWidth > totalTextWidth){
		substrWidth = 0;
		visibleInput = "";
		auto substrLen = 0;
		while(substrWidth < totalTextWidth){
			substrLen += 1;
			visibleInput = ofUTF8Substring(input, visibleInputStart, substrLen);
			substrWidth = getTextBoundingBox(ofUTF8Substring(input, visibleInputStart, substrLen),0,0).width;
		}
		substrLen -= 1;
		visibleInput = ofUTF8Substring(input, visibleInputStart, substrLen);
		substrWidth = getTextBoundingBox(ofUTF8Substring(input, visibleInputStart, substrLen),0,0).width;
		visibleInputEnd = visibleInputStart + substrLen;

		if(selectEndPos > visibleInputEnd){
			substrWidth = 0;
			visibleInput = "";
			visibleInputEnd = selectEndPos;
			visibleInputStart = selectEndPos;
			while(substrWidth < totalTextWidth){
				visibleInputStart -= 1;
				visibleInput = ofUTF8Substring(input, visibleInputStart, visibleInputEnd - visibleInputStart);
				substrWidth = getTextBoundingBox(ofUTF8Substring(input, visibleInputStart, visibleInputEnd - visibleInputStart),0,0).width;
			}
			visibleInputStart += 1;
			visibleInput = ofUTF8Substring(input, visibleInputStart, visibleInputEnd - visibleInputStart);
			substrWidth = getTextBoundingBox(ofUTF8Substring(input, visibleInputStart, visibleInputEnd - visibleInputStart),0,0).width;
		}
	}

	auto first = std::min(selectStartPos, selectEndPos);
	auto last = std::max(selectStartPos, selectEndPos);
	auto substrLength = ofUTF8Length(visibleInput);
	float preSelectWidth = 0;
	auto substrFirst = ofClamp(first-visibleInputStart, 0, substrLength);
	auto substrLast = ofClamp(last-visibleInputStart, 0, substrLength);
	if(first > visibleInputStart){
		std::string preSelectStr = ofUTF8Substring(visibleInput, 0, substrFirst);
		preSelectWidth = getTextBoundingBox(preSelectStr,0,0).width;
	}

	if(showLabelWhileEditing){
		selectStartX = b.getMaxX() - textPadding - substrWidth + preSelectWidth;
	}else{
		selectStartX = b.x + textPadding + preSelectWidth;
	}

	if(hasSelectedText()){
		std::string selectStr = ofUTF8Substring(visibleInput, substrFirst, substrLast - substrFirst);
		selectionWidth = getTextBoundingBox(selectStr,0,0).width;
	}

	setNeedsRedraw();
}

//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::moveCursor(int cursorPos){
	
	if ( cursorPos < 0 ){
		return;
	}

	lastCursorMoveTime = ofGetElapsedTimeMillis();

	selectStartPos = selectEndPos = cursorPos;
	selectionWidth = 0;
	
	if ( visibleInputEnd < ofUTF8Length( input ) ){
		// Attempt to extend visible input range. If a character was being entered 
		// somewhere inside the text, there might be enough space to the right to 
		// show the newly extended input.
		visibleInputEnd += 1;
	}

	if(cursorPos>visibleInputEnd){
		visibleInputEnd = cursorPos;
	}else if(cursorPos<visibleInputStart){
		visibleInputStart = cursorPos;
	}

	visibleInput = ofUTF8Substring(input, visibleInputStart, visibleInputEnd - visibleInputStart);
	auto substrWidth = getTextBoundingBox(visibleInput,0,0).width;
	auto totalTextWidth = b.width - textPadding * 2;

	if(substrWidth > totalTextWidth){
		substrWidth = 0;
		visibleInput = "";
		auto substrLen = 0;
		while(substrWidth < totalTextWidth){
			substrLen += 1;
			visibleInput = ofUTF8Substring(input, visibleInputStart, substrLen);
			substrWidth = getTextBoundingBox(ofUTF8Substring(input, visibleInputStart, substrLen),0,0).width;
		}
		substrLen -= 1;
		visibleInput = ofUTF8Substring(input, visibleInputStart, substrLen);
		substrWidth = getTextBoundingBox(ofUTF8Substring(input, visibleInputStart, substrLen),0,0).width;
		visibleInputEnd = visibleInputStart + substrLen;

		if(cursorPos > visibleInputEnd){
			substrWidth = 0;
			visibleInput = "";
			visibleInputEnd = cursorPos;
			visibleInputStart = cursorPos;
			while(substrWidth < totalTextWidth){
				visibleInputStart -= 1;
				visibleInput = ofUTF8Substring(input, visibleInputStart, visibleInputEnd - visibleInputStart);
				substrWidth = getTextBoundingBox(ofUTF8Substring(input, visibleInputStart, visibleInputEnd - visibleInputStart),0,0).width;
			}
			visibleInputStart += 1;
			visibleInput = ofUTF8Substring(input, visibleInputStart, visibleInputEnd - visibleInputStart);
			substrWidth = getTextBoundingBox(ofUTF8Substring(input, visibleInputStart, visibleInputEnd - visibleInputStart),0,0).width;
		}
	}


	float beforeCursorWidth = 0;
	if(selectStartPos > visibleInputStart){
		auto beforeCursorStr = ofUTF8Substring(visibleInput, 0, selectStartPos - visibleInputStart);
		beforeCursorWidth = getTextBoundingBox(beforeCursorStr,0,0).width;
	}

	if(showLabelWhileEditing){
		selectStartX = b.getMaxX() - textPadding - substrWidth + beforeCursorWidth;
	}else{
		selectStartX = b.x + textPadding + beforeCursorWidth;
	}

	setNeedsRedraw();
}

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::mouseMoved(ofMouseEventArgs & mouse){
	bool mouseOver = b.inside(mouse);
	if(mouseOver != bMouseOver && !bGuiActive && overlappingLabel){
		setNeedsRedraw();
	}
	bMouseOver = mouseOver;
	return (isGuiDrawing() || insideSlider) && bMouseOver;
}

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::mousePressed(ofMouseEventArgs & mouse){
	if(!isGuiDrawing() && !insideSlider){
		return false;
	}
	if(b.inside(mouse)){
		if(!insideSlider || mouse.button == OF_MOUSE_BUTTON_LEFT){
			bMousePressed = true;
			if(bGuiActive){
				auto inputWidth = getTextBoundingBox(visibleInput,0,0).width;
				float cursorX;
				if(showLabelWhileEditing){
					cursorX = mouse.x - (b.x + b.width - textPadding - inputWidth);
				}else{
					cursorX = mouse.x - (b.x + textPadding);
				}
				mousePressedPos =  round(visibleInputStart + ofMap(cursorX, 0, inputWidth, 0, ofUTF8Length(visibleInput), true));
				moveCursor(mousePressedPos);
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

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::mouseDragged(ofMouseEventArgs & mouse){
	if(!isGuiDrawing() && !insideSlider){
		return false;
	}
	if(!bGuiActive){
		return false;
	}

	if(!insideSlider || mouse.button == OF_MOUSE_BUTTON_LEFT){
		auto inputWidth = getTextBoundingBox(visibleInput,0,0).width;
		float cursorX;
		if(showLabelWhileEditing){
			cursorX = mouse.x - (b.x + b.width - textPadding - inputWidth);
		}else{
			cursorX = mouse.x - (b.x + textPadding);
		}
		auto cursorPos = round(visibleInputStart + ofMap(cursorX, 0, inputWidth, 0, ofUTF8Length(visibleInput)));
		cursorPos = ofClamp(cursorPos, 0, ofUTF8Length(input));
		calculateSelectionArea(mousePressedPos, cursorPos);
		setNeedsRedraw();
	}
	return true;
}

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::mouseReleased(ofMouseEventArgs &){
	bMousePressed = false;
	return bGuiActive;
}

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::mouseScrolled(ofMouseEventArgs & mouse){
	if(!isGuiDrawing() || insideSlider){
		//when insideSlider it is the slider object who is in charge of handling the scrolling
		return false;
	}
	if(b.inside(mouse)){
		if(!bGuiActive){
			if(mouse.scrollY>0 || mouse.scrollY<0){
				double range = getRange(value.getMin(), value.getMax(), b.width);
				Type newValue = value + ofMap(mouse.scrollY,-1,1,-range, range);
				newValue = ofClamp(newValue,value.getMin(),value.getMax());
				value = newValue;
			}
		}
		return true;
	}else{
		return false;
	}
}

//-----------------------------------------------------------
template<>
bool ofxInputField<string>::mouseScrolled(ofMouseEventArgs & mouse){
	if(b.inside(mouse)){
		return true;
	}else{
		return false;
	}
}

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::charPressed(uint32_t & key){
	if(!isGuiDrawing() && !insideSlider){
		return false;
	}
	if(bGuiActive && !bMousePressed){
		if(key >= '0' && key <= '9'){
			moveCursor(insertKeystroke(key));
		}else if(key == '.' || key == '-' || key == '+'){
			moveCursor(insertKeystroke(key));
		}else{
			moveCursor(insertAlphabetic(key));
		}
		return true;
	}
	return false;
}

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::keyPressed(ofKeyEventArgs & args){
	if(!isGuiDrawing()){
		return false;
	}
	if(bGuiActive && !bMousePressed){
		auto key = args.key;
		auto first = std::min(selectStartPos, selectEndPos);
		auto last = std::max(selectStartPos, selectEndPos);
		auto selectLen = last - first;
		if(key == OF_KEY_BACKSPACE || key == OF_KEY_DEL){
			if(hasSelectedText()){
				ofUTF8Erase(input, first, selectLen);
				moveCursor(selectStartPos);
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
					moveCursor(deleteIdx);
				}
			}
		}else if(key == OF_KEY_LEFT){
			if(args.hasModifier(OF_KEY_SHIFT)){
				selectEndPos -= 1;
				selectEndPos = std::max(0, selectEndPos);
				calculateSelectionArea(selectStartPos, selectEndPos);
			}else{
				if(hasSelectedText()){
					moveCursor(first);
				}else{
					moveCursor(std::max(0, selectEndPos - 1));
				}
			}
		}else if(key == OF_KEY_RIGHT){
			if(args.hasModifier(OF_KEY_SHIFT)){
				auto inputSize = ofUTF8Length(input);
				selectEndPos += 1;
				selectEndPos = std::min(selectEndPos, int(inputSize));
				calculateSelectionArea(selectStartPos, selectEndPos);
			}else{
				if(hasSelectedText()){
					moveCursor(last);
				}else{
					auto inputSize = ofUTF8Length(input);
					moveCursor(std::min(selectEndPos + 1, int(inputSize)));
				}
			}
		}else if(key == OF_KEY_RETURN){
			leaveFocus();
		}else if(key == OF_KEY_ESC){
			input = toString(value.get());
			leaveFocus();
		}else if(key == 'a' && args.hasModifier(OF_KEY_CONTROL)){
			calculateSelectionArea(0, ofUTF8Length(input));
		}else if(key == 'c' && args.hasModifier(OF_KEY_CONTROL)){
			if(selectLen>0){
				auto selection = ofUTF8Substring(input, first, selectLen);
				ofSetClipboardString(selection);
			}
		}else if(key == 'v' && args.hasModifier(OF_KEY_CONTROL)){
			auto cursorPos = first;
			if(selectLen>0){
				ofUTF8Erase(input, cursorPos, selectLen);
			}
			auto clipboard = ofGetClipboardString();
			for(auto c: ofUTF8Iterator(clipboard)){
				ofUTF8Insert(input, cursorPos, c);
				cursorPos+=1;
			}
			moveCursor(cursorPos);
		}else if(key == 'x' && args.hasModifier(OF_KEY_CONTROL)){
			if(selectLen>0){
				auto selection = ofUTF8Substring(input, first, selectLen);
				ofSetClipboardString(selection);
				ofUTF8Erase(input, first, selectLen);
				moveCursor(first);
			}
		}else if(key == OF_KEY_END){
			auto inputLength = ofUTF8Length(input);
			if(args.hasModifier(OF_KEY_SHIFT)){
				calculateSelectionArea(selectStartPos, inputLength);
			}else{
				moveCursor(inputLength);
			}
		}else if(key == OF_KEY_HOME){
			if(args.hasModifier(OF_KEY_SHIFT)){
				calculateSelectionArea(selectStartPos, 0);
			}else{
				moveCursor(0);
			}
		}
		return true;
	}
	return false;
}


//-----------------------------------------------------------
template<typename Type>
int ofxInputField<Type>::insertKeystroke(uint32_t character){
	auto first = std::min(selectStartPos, selectEndPos);
	auto last = std::max(selectStartPos, selectEndPos);
	auto selectLen = last - first;
	if(hasSelectedText()){
		ofUTF8Erase(input, first, selectLen);
	}
	ofUTF8Insert(input, first, character);
	auto cursorPos = first + 1;

	setNeedsRedraw();
	return cursorPos;
}

//-----------------------------------------------------------
template<typename Type>
int ofxInputField<Type>::insertAlphabetic(uint32_t character){
	if(character == 'x' || character == 'a' || character == 'b' || character=='c' || character=='d' || character=='e' || character=='f'){
	   return insertKeystroke(character);
	}else{
		return -1; //cursor or selection area stay the same
	}
}

//-----------------------------------------------------------
template<>
int ofxInputField<string>::insertAlphabetic(uint32_t character){
	return insertKeystroke(character);
}

//-----------------------------------------------------------
template<typename Type>
Type ofxInputField<Type>::operator=(Type v){
	value = v;
	return v;
}

//-----------------------------------------------------------
template<typename Type>
ofxInputField<Type>::operator const Type & (){
	return value;
}

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::hasSelectedText(){
	return selectStartPos != selectEndPos;
}

//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::generateDraw(){
	bg.clear();

	bg.append(rectangle(b, thisBackgroundColor));

	if(bGuiActive){
		if(hasSelectedText()){
			ofRectangle selection(selectStartX, b.y+1, selectionWidth, b.height-2);
			bg.append(rectangle(selection, thisFillColor));
		}else if(!blinkingCursor){
			ofRectangle cursor(selectStartX, b.y, 1, b.height);
			bg.append(rectangle(cursor, thisTextColor));
		}
	}

	auto input = visibleInput;
	if(!bGuiActive && !containsValidValue()){
		input = toString(value);
	}

	auto inputWidth = getTextBoundingBox(input,0,0).width;
	auto yPos = getTextVCenteredInRect(b);
	auto label = getTextBoundingBox(getName(), b.x + textPadding, yPos);
	auto value = getTextBoundingBox(input, b.x + b.width - textPadding - inputWidth, yPos);
	overlappingLabel = label.getMaxX() > value.x;

	textMesh.clear();
	if(!bGuiActive || showLabelWhileEditing){
		if(!overlappingLabel || (!bMouseOver && !bGuiActive)){
			textMesh.append(getTextMesh(getName(), b.x + textPadding, yPos) );
		}
		if((!bGuiActive && (bMouseOver || !overlappingLabel)) || bGuiActive){
			textMesh.append(getTextMesh(input, b.x + b.width - textPadding - inputWidth, yPos));
		}
	}else{
		textMesh.append(getTextMesh(input, b.x + textPadding, yPos));
	}
	textMesh.getColors().assign(textMesh.getVertices().size(), thisTextColor);
}

//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::render(){
	bg.draw();

	if(!insideSlider && errorTime > 0 && !containsValidValue()){
		auto now = ofGetElapsedTimeMillis();
		auto pct = (now - errorTime) * 0.5f;
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

//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::drawCursor(){
	auto now = ofGetElapsedTimeMillis();
	auto timeSinceLastCursorMove = now - lastCursorMoveTime;

	if(!blinkingCursor || ((now % 2000) >= 1000) || (timeSinceLastCursorMove < 500)){
		ofPushStyle();
		ofSetColor(thisTextColor);
		ofDrawLine( selectStartX, b.y, selectStartX, b.y+b.height );
		ofPopStyle();
	}
}

//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::setBlinkingCursor(bool blink){
	blinkingCursor = blink;
	setNeedsRedraw();
}

//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::setShowLabelWhileEditing(bool show){
	showLabelWhileEditing = show;
	setNeedsRedraw();
}

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::setValue(float mx, float my, bool bCheck){
	return false;
}

//-----------------------------------------------------------
template<typename Type>
ofAbstractParameter & ofxInputField<Type>::getParameter(){
	return value;
}

//-----------------------------------------------------------
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
			errorTime = ofGetElapsedTimeMillis();
		}
		validValue = false;
	}
}

//-----------------------------------------------------------
template<>
void ofxInputField<string>::parseInput(){
	value = input;
}

//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::valueChanged(Type & value){
	visibleInput = input = toString(value);
	if(bGuiActive){
		moveCursor(ofUTF8Length(input));
	}
    setNeedsRedraw();
}

//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::leaveFocus(){
	bGuiActive = false;
	parseInput();
	setNeedsRedraw();
	leftFocus.notify(this);
}

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::containsValidValue() const{
	return validValue;
}

//-----------------------------------------------------------
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

//for some reason osx errors if this isn't defined
#if defined TARGET_OSX || defined TARGET_EMSCRIPTEN
	template class ofxInputField<typename std::conditional<std::is_same<uint32_t, size_t>::value || std::is_same<uint64_t, size_t>::value, bool, size_t>::type>;
#endif
