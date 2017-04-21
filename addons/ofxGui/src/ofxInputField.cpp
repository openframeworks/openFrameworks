#include "ofxInputField.h"

#include "ofGraphics.h"

template<typename Type>
ofxInputField<Type>::ofxInputField(){
	bChangedInternally = false;
	bGuiActive = false;
	bMousePressed = false;
	mouseInside = false;
	bRegisteredForKeyEvents = false;
	mousePressedPos = -1;
	selectStartX = -1;
	selectStartPos = -1;
	selectEndPos = -1;
	pressCounter = 0;
	inputWidth = 0;
	selectionWidth = 0;
}

template<typename Type>
ofxInputField<Type>::~ofxInputField(){
	value.removeListener(this,&ofxInputField::valueChanged);
}

template<typename Type>
ofxInputField<Type>::ofxInputField(ofParameter<Type> _val, float width, float height){
	setup(_val,width,height);
}

template<typename Type>

ofxInputField<Type>* ofxInputField<Type>::setup(ofParameter<Type> _val, float width, float height){
	value.makeReferenceTo(_val);
	input = ofToString(value);
	inputWidth = getTextBoundingBox(input,0,0).width;
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	setNeedsRedraw();

	value.addListener(this,&ofxInputField::valueChanged);
	registerMouseEvents();
	registerKeyEvents();
	pressCounter = 0;
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
		preSelectStr.assign(input,0,selectStartPos);
		preSelectWidth = getTextBoundingBox(preSelectStr,0,0).width;
	}
	selectStartX = b.width - textPadding - inputWidth + preSelectWidth;

	if(hasSelectedText()){
		selectStr.assign(input,selectStartPos,selectEndPos-selectStartPos);
		selectionWidth = getTextBoundingBox(selectStr,0,0).width;
	}
}

template<typename Type>
bool ofxInputField<Type>::mouseMoved(ofMouseEventArgs & args){
	mouseInside = isGuiDrawing() && b.inside(ofPoint(args.x,args.y));
	return mouseInside;
}

template<typename Type>
bool ofxInputField<Type>::mousePressed(ofMouseEventArgs & args){
	if(b.inside(args.x,args.y)){
		bMousePressed = true;
		if(!bGuiActive){
			bGuiActive = true;
		}

		float cursorX = args.x - (b.x + b.width - textPadding - inputWidth);
		int cursorPos = ofMap(cursorX,0,inputWidth,0,input.size(),true);
		mousePressedPos = cursorPos;

		calculateSelectionArea(cursorPos, cursorPos);

		pressCounter++;

	}else{
		if(bGuiActive){
			leaveFocus();
		}
	}
	return false;
}

template<typename Type>
bool ofxInputField<Type>::mouseDragged(ofMouseEventArgs & args){
	if(!bGuiActive || !bMousePressed)
		return false;

	float cursorX = args.x - (b.x + b.width - textPadding - inputWidth);
	int cursorPos = ofMap(cursorX,0,inputWidth,0,input.size(),true);
	calculateSelectionArea(mousePressedPos,cursorPos);
	return false;
}

template<typename Type>
bool ofxInputField<Type>::mouseReleased(ofMouseEventArgs & args){
//	if(bUpdateOnEnterOnly){
//		value.enableEvents();
//	}
	if(bGuiActive){
		if(pressCounter == 1 && !hasSelectedText()){
			//activated panel without selecting an area => select all
			calculateSelectionArea(0, input.size());
		}
	}

	bMousePressed = false;
	return false;
}

template<typename Type>
void ofxInputField<Type>::registerKeyEvents(){
	if(bRegisteredForKeyEvents == true){
		return; // already registered.
	}
	bRegisteredForKeyEvents = true;
	ofRegisterKeyEvents(this, OF_EVENT_ORDER_BEFORE_APP);
}

template<typename Type>
void ofxInputField<Type>::unregisterKeyEvents(){
	if(bRegisteredForKeyEvents == false){
		return; // not registered.
	}
	ofUnregisterKeyEvents(this, OF_EVENT_ORDER_BEFORE_APP);
	bRegisteredForKeyEvents = false;
}

template<typename Type>
bool ofxInputField<Type>::keyPressed(ofKeyEventArgs & args){
	if(bGuiActive && !bMousePressed){
		ofLogNotice("ofxInputField::keyPressed") << args.key;

		int newCursorIdx = -1;
		if(args.key >= '0' && args.key <= '9'){
			int digit = args.key - '0';
			newCursorIdx = insertKeystroke(ofToString(digit));
		}else if(args.key == '.' ){
			newCursorIdx = insertKeystroke(".");
		}else if(args.key == OF_KEY_BACKSPACE || args.key == OF_KEY_DEL){
			if(hasSelectedText()){
				input.erase(selectStartPos,selectEndPos-selectStartPos);
				newCursorIdx = selectStartPos;
				parseInput();
			}else{
				int deleteIdx = -1;
				if(args.key == OF_KEY_BACKSPACE){
					deleteIdx = selectStartPos-1;
				}else if(args.key == OF_KEY_DEL){
					deleteIdx = selectStartPos;
				}

				//erase char if valid deleteIdx
				if(deleteIdx >= 0 && deleteIdx < input.size()){
					input.erase(deleteIdx,1);
					newCursorIdx = deleteIdx;
					parseInput();
				}
			}
		}else if(args.key == OF_KEY_LEFT){
			if(hasSelectedText()){
				newCursorIdx = selectStartPos;
			}else{
				newCursorIdx = selectStartPos == 0 ? 0 : selectStartPos-1;
			}
		}else if(args.key == OF_KEY_RIGHT){
			if(hasSelectedText()){
				newCursorIdx = selectEndPos;
			}else{
				newCursorIdx = selectStartPos == input.size() ? input.size() : selectStartPos+1;
			}
		}else if(args.key == OF_KEY_RETURN){
			leaveFocus();
		}else if(args.key >= '!' && args.key <= '~'){
			newCursorIdx = insertAlphabetic(ofToString((char)args.key));
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
bool ofxInputField<Type>::keyReleased(ofKeyEventArgs & args){
	return bGuiActive && !bMousePressed;
}


template<typename Type>
int ofxInputField<Type>::insertKeystroke(const std::string & character){
	if(hasSelectedText()){
		input.erase(selectStartPos,selectEndPos-selectStartPos);
	}
	input.insert(selectStartPos,character);
	parseInput();
	return selectStartPos + 1;
}

template<typename Type>
int ofxInputField<Type>::insertAlphabetic(const std::string & character){
	return -1; //cursor or selection area stay the same
}

template<>
int ofxInputField<string>::insertAlphabetic(const std::string & character){
	return insertKeystroke(character);
}


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
bool ofxInputField<Type>::mouseScrolled(ofMouseEventArgs & args){
	if(mouseInside || bGuiActive){
		if(args.y>0 || args.y<0){
			double range = getRange(value.getMin(),value.getMax(),b.width);
			Type newValue = value + ofMap(args.y,-1,1,-range, range);
			newValue = ofClamp(newValue,value.getMin(),value.getMax());
			value = newValue;
		}
		return true;
	}else{
		return false;
	}
}

template<>
bool ofxInputField<string>::mouseScrolled(ofMouseEventArgs & args){
	if(mouseInside || bGuiActive){
		return true;
	}else{
		return false;
	}
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
void ofxInputField<Type>::generateDraw(){
	bg.clear();

	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(b);

	generateText();
}


template<typename Type>
void ofxInputField<Type>::generateText(){
	string valStr = input;
	textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	textMesh.append(getTextMesh(valStr, b.x + b.width - textPadding - getTextBoundingBox(valStr,0,0).width, b.y + b.height / 2 + 4));
}

template<typename Type>
void ofxInputField<Type>::render(){
	bg.draw();

	if(bGuiActive){
		drawFocusedBB();

		if(hasSelectedText()){
			drawSelectedArea();
		}else{
			drawCursor();
		}
	}

	drawMesh();
}

template<typename Type>
bool ofxInputField<Type>::hasSelectedText(){
	return selectStartPos != selectEndPos;
}

template<typename Type>
void ofxInputField<Type>::drawMesh(){
	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(thisTextColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	ofColor c = ofGetStyle().color;
	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

template<typename Type>
void ofxInputField<Type>::drawSelectedArea(){
	ofPushStyle();
	ofSetColor(thisFillColor);
	ofFill();
	ofDrawRectangle( selectStartX+b.x, b.y+1, selectionWidth, b.height-2 );
	ofPopStyle();
}

template<typename Type>
void ofxInputField<Type>::drawCursor(){
	ofPushStyle();
	ofSetColor(thisTextColor);
	ofDrawLine( selectStartX+b.x, b.y, selectStartX+b.x, b.y+b.height );
	ofPopStyle();
}

template<typename Type>
void ofxInputField<Type>::drawFocusedBB(){
	ofPushStyle();
	ofSetColor(thisTextColor);
	ofDrawLine( selectStartX+b.x, b.y, selectStartX+b.x, b.y+b.height );
	ofPopStyle();
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
	bChangedInternally = true;
	Type tmpVal = ofToFloat(input);
	if(tmpVal < getMin()){
		tmpVal = getMin();
	}else if(tmpVal > getMax()){
		tmpVal = getMax();
	}
	value = tmpVal;
}

template<>
void ofxInputField<string>::parseInput(){
	bChangedInternally = true;
	value = input;
}

template<typename Type>
void ofxInputField<Type>::valueChanged(Type & value){
	if(bChangedInternally){
		bChangedInternally = false;
		inputWidth = getTextBoundingBox(input,0,0).width;
	}else{
		input = ofToString(value);
		inputWidth = getTextBoundingBox(input,0,0).width;
		if(bGuiActive){
			int cursorPos = input.size();
			calculateSelectionArea(cursorPos,cursorPos);
		}
	}
    setNeedsRedraw();
}

template<typename Type>
void ofxInputField<Type>::leaveFocus(){
	bGuiActive = false;
	pressCounter = 0;
	input = ofToString(value);
	inputWidth = getTextBoundingBox(input,0,0).width;
	setNeedsRedraw();
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
template class ofxInputField<string>;
