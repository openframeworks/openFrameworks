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
ofxInputField<Type> ofxInputField<Type>::createInsideSlider(){
	ofxInputField<Type> input;
	input.insideSlider = true;
	return input;
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
	if(!std::is_same<Type, std::string>::value && insideSlider){
		//the following are the hard coded images with the minimal size font
		inputStateButtons[INPUT_STATE_MIN].setup("MIN", false, {0xa2,0x4b,0x6c,0xb2,0xa8,0xa4,0x11,0x49,0x22,0xba,0x4,0x0});
		inputStateButtons[INPUT_STATE_MAX].setup("MAX", false, {0x22,0x93,0x6c,0x29,0xa9,0x9e,0x11,0xa5,0x24,0x4a,0x9,0x0});
		inputStateButtons[INPUT_STATE_VAL].setup("VAL", true, {0xca,0x4,0x54,0xa,0xa8,0x17,0x50,0x29,0x40,0xd2,0x3,0x0});
		setInputStateButtonsShape();
	}
	setNeedsRedraw();

	if(!insideSlider){
		registerMouseEvents();
	}
	listeners.push_back(value.newListener(this,&ofxInputField::valueChanged));
	listeners.push_back(ofEvents().charEvent.newListener(this, &ofxInputField<Type>::charPressed, OF_EVENT_ORDER_BEFORE_APP));
	listeners.push_back(ofEvents().keyPressed.newListener(this, &ofxInputField<Type>::keyPressed, OF_EVENT_ORDER_BEFORE_APP));
	if(!std::is_same<Type, std::string>::value && insideSlider){
		listeners.push_back(inputStateButtons[INPUT_STATE_MIN].clickedEvent.newListener([this](){
			setInputState(INPUT_STATE_MIN);
		}) );
		listeners.push_back(inputStateButtons[INPUT_STATE_VAL].clickedEvent.newListener([this](){
			setInputState(INPUT_STATE_VAL);
		}) );
		listeners.push_back(inputStateButtons[INPUT_STATE_MAX].clickedEvent.newListener([this](){
			setInputState(INPUT_STATE_MAX);
		}) );
	}
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
void ofxInputField<Type>::setInputStateButtonsShape(){
	if(!std::is_same<Type, std::string>::value && insideSlider){
		float h = b.height/3;
		float w = 0;
		bool bUseFont = (useTTF && h >= font.getAscenderHeight());
		
		for(int i =0; i < INPUT_STATE_TOTAL; i++){
			if(bUseFont){
				inputStateButtons[i].textRect = font.getStringBoundingBox(inputStateButtons[i].name, 0, 0);
			}else if(h >= 14.f){//Use ofBitmapFont
				inputStateButtons[i].textRect = bitmapFont.getBoundingBox(inputStateButtons[i].name, 0, 0);
			}else{// Use bitmap buttons
				inputStateButtons[i].textRect.set(0,0,17, 6);
			}
		}
		if(bUseFont){
			//font might not be mono width so we find out the max width of the three buttons so these align nicely.
			for(int i =0; i < INPUT_STATE_TOTAL; i++){
				w = std::max(w, inputStateButtons[i].textRect.width);
			}
		}else{
			//the other options all have the same width so lets just take it from the first one..
			//This is just to avoid the for loop above
			w = inputStateButtons[0].textRect.width;
		}
		w += 2; //just add a 1 pixel margin on each side of the text.
		for(int i =0; i < INPUT_STATE_TOTAL; i++){
			inputStateButtons[i].rect.set(b.getMaxX() - w, b.y + h*i, w, h);
			inputStateButtons[i].textRect.alignTo(inputStateButtons[i].rect);
		}
	}
}
//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::setInputState(InputState i){
	if(!std::is_same<Type, std::string>::value && insideSlider){
		inputState = i;
		switch(inputState){
			case INPUT_STATE_MIN:
				input = toString(value.getMin());
				break;
			case INPUT_STATE_MAX:
				input = toString(value.getMax());
				break;
			case INPUT_STATE_VAL:
				input = toString(value.get());
				break;
			case INPUT_STATE_TOTAL://Just so the complainer doesnt complain with a warning. :)
				break;
		}
		for(int j = 0; j < INPUT_STATE_TOTAL; j++){
			inputStateButtons[j].bActive = (j == (int)inputState);
		}
		visibleInput = input;
		moveCursor(ofUTF8Length(input));
		setNeedsRedraw();
	}
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
	if(!isGuiDrawing() && !insideSlider){
		return false;
	}
	bool mouseOver = b.inside(mouse);
	if(!std::is_same<Type, std::string>::value && insideSlider){
		for(int i =0; i < INPUT_STATE_TOTAL; i++){
			inputStateButtons[i].mouseMoved(mouse);
		}
	}
	if(mouseOver != bMouseOver && !bGuiActive && overlappingLabel){
		setNeedsRedraw();
	}
	bMouseOver = mouseOver;
	return bMouseOver;
}

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::mousePressed(ofMouseEventArgs & mouse){
	if(!isGuiDrawing() && !insideSlider){
		return false;
	}
	if(b.inside(mouse)){
		if(isGuiDrawing() && !(std::is_same<Type, std::string>::value) && insideSlider){
			for(int i =0; i < INPUT_STATE_TOTAL; i++){
				if(inputStateButtons[i].mousePressed(mouse)){
					return true;
				}
			}
		}
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
	if(!std::is_same<Type, std::string>::value && insideSlider){
		for(int i =0; i < INPUT_STATE_TOTAL; i++){
			if(inputStateButtons[i].mouseDragged(mouse)){
				return true;
			}
		}
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
bool ofxInputField<Type>::mouseReleased(ofMouseEventArgs & mouse){
	bMousePressed = false;
	if(bGuiActive && !(std::is_same<Type, std::string>::value)&& insideSlider){
		for(int i =0; i < INPUT_STATE_TOTAL; i++){
			if(inputStateButtons[i].mouseReleased(mouse)){
				return true;
			}
		}
	}
	return bGuiActive;
}

//-----------------------------------------------------------
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

//-----------------------------------------------------------
template<>
bool ofxInputField<string>::mouseScrolled(ofMouseEventArgs & mouse){
	if(b.inside(mouse)){
		return true;
	}else{
		return false;
	}
}

template<typename Type>
void ofxInputField<Type>::setPosition(const ofPoint & p){
	ofxBaseGui::setPosition(p);
	setInputStateButtonsShape();
}
template<typename Type>
void ofxInputField<Type>::setPosition(float x, float y){
	ofxBaseGui::setPosition(x,y);
	setInputStateButtonsShape();
}
template<typename Type>
void ofxInputField<Type>::setSize(float w, float h){
	ofxBaseGui::setSize(w, h);
	setInputStateButtonsShape();
}
template<typename Type>
void ofxInputField<Type>::setShape(ofRectangle r){
	ofxBaseGui::setShape(r);
	setInputStateButtonsShape();
}
template<typename Type>
void ofxInputField<Type>::setShape(float x, float y, float w, float h){
	ofxBaseGui::setShape(x, y, w, h);
	setInputStateButtonsShape();
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
	auto label = getTextBoundingBox(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	auto value = getTextBoundingBox(input, b.x + b.width - textPadding - inputWidth, b.y + b.height / 2 + 4);
	overlappingLabel = label.getMaxX() > value.x;

	textMesh.clear();
	if(!bGuiActive || showLabelWhileEditing){
		if(!overlappingLabel || (!bMouseOver && !bGuiActive)){
			textMesh.append(getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4) );
		}
		if((!bGuiActive && (bMouseOver || !overlappingLabel)) || bGuiActive){
			textMesh.append(getTextMesh(input, b.x + b.width - textPadding - inputWidth, b.y + b.height / 2 + 4));
		}
	}else{
		textMesh.append(getTextMesh(input, b.x + textPadding, b.y + b.height / 2 + 4));
	}
	textMesh.getColors().assign(textMesh.getVertices().size(), thisTextColor);
	
	
	setInputStateButtonsShape();
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
	if(!std::is_same<Type, std::string>::value && insideSlider){
		for(int i =0; i < INPUT_STATE_TOTAL; i++){
			inputStateButtons[i].draw();
		}
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
		if(inputState == INPUT_STATE_VAL){
			if(tmpVal < getMin()){
				tmpVal = getMin();
			}else if(tmpVal > getMax()){
				tmpVal = getMax();
			}
		value = tmpVal;
		}else if(inputState == INPUT_STATE_MIN){
			setMin(tmpVal);
		}else if(inputState == INPUT_STATE_MAX){
			setMax(tmpVal);
		}
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
	if(!std::is_same<Type, std::string>::value && insideSlider){
		setInputState(INPUT_STATE_VAL);
		value = value.get();//hack to trigger the value change event so the gui redraws
	}
	setNeedsRedraw();
	leftFocus.notify(this);
}

//-----------------------------------------------------------
template<typename Type>
bool ofxInputField<Type>::containsValidValue() const{
	return validValue;
}

//-----------------------------------------------------------
//   Simple button implementation
//-----------------------------------------------------------
template<typename Type>
void ofxInputField<Type>::SimpleButton::setup(std::string name, bool bSetActive, const std::vector<unsigned char>& imgData){
    this->name = name;
    this->bActive = bSetActive;
    if(imgData.size()){
        this->img.allocate(17, 6, OF_IMAGE_COLOR_ALPHA);
        ofxBaseGui::loadStencilFromHex(this->img, const_cast<unsigned char*>(&imgData[0]));
        this->img.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
    }
}
template<typename Type>
void ofxInputField<Type>::SimpleButton::draw(){
    ofPushStyle();
    ofFill();
    ofSetColor((bActive?ofColor::red:(bInside?ofxBaseGui::fillColor:ofxBaseGui::backgroundColor)));
    ofDrawRectangle(rect);
    ofSetColor(bActive?ofxBaseGui::textColor:ofxBaseGui::headerBackgroundColor);
    if(ofxBaseGui::useTTF && rect.height >= font.getAscenderHeight()){
        ofxBaseGui::font.drawString(name, textRect.x, textRect.getMaxY());
    }else
        if(rect.height >= 14.f){ //height equal or greater than the bitmap font size.
            ofDrawBitmapString(name, textRect.x, textRect.getMaxY() -3);
        }else{
            img.draw(textRect);
        }
    ofPopStyle();
}
template<typename Type>
bool ofxInputField<Type>::SimpleButton::mouseMoved(ofMouseEventArgs & args){
    bInside = rect.inside(args);
    return false;
}
template<typename Type>
bool ofxInputField<Type>::SimpleButton::mousePressed(ofMouseEventArgs & args){
    bInside = rect.inside(args);
    bPressed = bInside;
    return bInside;
}
template<typename Type>
bool ofxInputField<Type>::SimpleButton::mouseDragged(ofMouseEventArgs & args){
    bInside = rect.inside(args);
    if(bPressed && !bInside){bPressed = false;}
    return bPressed && bInside;
}
template<typename Type>
bool ofxInputField<Type>::SimpleButton::mouseReleased(ofMouseEventArgs & args){
    bInside = rect.inside(args);
    if(bInside && bPressed){
        ofNotifyEvent(clickedEvent);
    }
    bPressed = false;
    return bInside;
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
template class ofxInputField<typename std::conditional<std::is_same<uint32_t, size_t>::value || std::is_same<uint64_t, size_t>::value, bool, size_t>::type>;
