#include "ofxDropDownList.h"
#include "ofGraphics.h"


ofxDropDownList::ofxDropDownList(string dorpDownListName, vector<string> itemName, int index, float width, float height){
	setup(dorpDownListName,itemName,index,width,height);
}

ofxDropDownList::~ofxDropDownList(){
	for (int i = 0; i < itemValue.size(); i++){
		itemValue[i].removeListener(this,&ofxDropDownList::valueChanged);
	}
}

ofxGuiGroup * ofxDropDownList::setup(string dorpDownListName, vector<string> itemName, int index, float width, float height){
    b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height*itemName.size();
	bGuiActive = false;
	ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
	for (int i = 0; i < itemName.size(); i++){
		ofParameter<string> item;
		string name = "item";
		name = name + ofToString(i);
		item.set(name,itemName[i]);
		items.push_back(item);

		ofParameter<bool> value;
		string _name = "itemValue";
		_name = _name + ofToString(i);
		if (i == index){
			value.set(_name,true);
		}else{
			value.set(_name,false);
		}
		itemValue.push_back(value);
	}

	for (int i = 0; i < itemValue.size(); i++){
		itemValue[i].addListener(this,&ofxDropDownList::valueChanged);
	}
	generateDraw();

	dropDownList.setup(dorpDownListName);
	dropDownList.add(this);
	return &dropDownList;
}


bool ofxDropDownList::mouseMoved(ofMouseEventArgs & args){
	if(isGuiDrawing() && b.inside(ofPoint(args.x,args.y))){
		return true;
	}else{
		return false;
	}
}

bool ofxDropDownList::mousePressed(ofMouseEventArgs & args){
	if(setValue(args.x, args.y, true)){
		return true;
	}else{
		return false;
	}
}

bool ofxDropDownList::mouseDragged(ofMouseEventArgs & args){
	if(bGuiActive && b.inside(ofPoint(args.x,args.y))){
		return true;
	}else{
		return false;
	}
}

bool ofxDropDownList::mouseReleased(ofMouseEventArgs & args){
	bool wasGuiActive = bGuiActive;
	bGuiActive = false;
	if(wasGuiActive && b.inside(ofPoint(args.x,args.y))){
		return true;
	}else{
		return false;
	}
}

void ofxDropDownList::generateDraw(){
	background.clear();
	foreground.clear();
	textMH.clear();

	for(int i=0;i<items.size();i++){
		ofPath bg;
		bg.clear();
		bg.setFillColor(thisBackgroundColor);
		bg.rectangle(b.x,b.y+(i*defaultHeight),b.width,((b.height)/items.size()));
		background.push_back(bg);

		ofPath fg;
		fg.clear();
		if(itemValue[i]){
			if (value != i){
				value = i;
			}
			fg.setFilled(true);
			fg.setFillColor(thisFillColor);
		}else{
			fg.setFilled(false);
			fg.setStrokeWidth(1);
			fg.setStrokeColor(thisFillColor);
		}
		fg.rectangle(b.x,b.y+(i*defaultHeight),b.width,((b.height)/items.size()));
		foreground.push_back(fg);

		ofVboMesh textMesh;
		textMesh = getTextMesh(getItemName(i), b.x+textPadding , b.y+(b.height /items.size())/2 + 4 + (i*defaultHeight));
		textMH.push_back(textMesh);
	}
}

void ofxDropDownList::render(){
	for(int i=0;i<items.size();i++){
		background[i].draw();
		foreground[i].draw();

		ofColor c = ofGetStyle().color;
		ofBlendMode blendMode = ofGetStyle().blendingMode;
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableAlphaBlending();
		}
		ofSetColor(thisTextColor);

		bindFontTexture();
#ifdef SUPPORT_FONTSTASH
		unicodeFont.draw(getItemName(i),fontSize, b.x+textPadding , b.y+(b.height /items.size())/2 + fontSize/2 - (2 * (fontSize/12)) + (i*defaultHeight));
#else
		textMH[i].draw();
#endif
		unbindFontTexture();

		ofSetColor(c);
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableBlendMode(blendMode);
		}
	}
}

bool ofxDropDownList::setValue(float mx, float my, bool bCheck){
	for (int i = 0; i < items.size(); i++){
		if( !isGuiDrawing() ){
			bGuiActive = false;
			return false;
		}
		if( bCheck ){
			ofRectangle check(b.x,b.y+(i*defaultHeight),b.width,((b.height)/items.size()));
			if(check.inside(mx,my)){
				bGuiActive = true;
			}else{
				bGuiActive = false;
			}
		}
		if( bGuiActive ){
			for (int i = 0; i < items.size(); i++){
				itemValue[i] = false;
			}
			itemValue[i] = !itemValue[i];
			if (value != i){
				value = i;
			}
			return true;
		}
	}
	return false;
}

ofAbstractParameter & ofxDropDownList::getParameter(){
	parameters.setName("dropDownList");
	for (int i = 0; i < itemValue.size(); i++){
		parameters.add(items[i]);
		parameters.add(itemValue[i]);
	}
	return parameters;
}

void ofxDropDownList::valueChanged(bool & value){
	generateDraw();
}

void ofxDropDownList::addData(ofAbstractParameter & parameter){
	parameters.add(parameter);
}

string ofxDropDownList::getItemName(int index){
	return items[index];
}
int ofxDropDownList::getCurItemId(){
	return value;
}

int ofxDropDownList::getItemNum(){
	return items.size();
}

void ofxDropDownList::setCurItemId(int id){
	for(int i=0;i<itemValue.size();i++){
		itemValue[i] = false;
	}
	itemValue[id] = true;
	value = id;
}
void ofxDropDownList::minimize(){
	dropDownList.minimize();
}
void ofxDropDownList::maximize(){
	dropDownList.maximize();
}

void ofxDropDownList::minimizeAll(){
	dropDownList.minimizeAll();
}

void ofxDropDownList::maximizeAll(){
	dropDownList.maximizeAll();
}