#include "ofxQuadWarp.h"
#include "ofGraphics.h"

ofxQuadWarp::ofxQuadWarp(){
	bGuiActive = false;
	bCircle.push_back(false);
	bCircle.push_back(false);
	bCircle.push_back(false);
	bCircle.push_back(false);
}

ofxQuadWarp::ofxQuadWarp(string quadWarpName, ofBaseDraws &content, float width, float height){
	setup(quadWarpName,content,width,height);
}

ofxQuadWarp::~ofxQuadWarp(){
   
}

ofxGuiGroup* ofxQuadWarp::setup(string quadWarpName, ofBaseDraws &content, float width, float height) {
	b.x = 0;
	b.y = 0;
	b.width  = defaultWidth - defaultWidth * 0.02;
	b.height = height * (defaultWidth/width);
	name = quadWarpName;
	ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
	this->content = &content;

	this->width = b.width/this->content->getWidth();
	this->height = b.height/this->content->getHeight();

	InitQuadPos();
	quadWarpGroup.setup(quadWarpName);
	quadWarpGroup.add(this);
	return &quadWarpGroup;
}

void ofxQuadWarp::render() {
	ofColor c = ofGetStyle().color;
	content->draw(b.x , b.y,b.width,b.height);
	bg.draw();
	circumscribe.clear();
	circumscribe.setStrokeWidth(3);
	circumscribe.setColor(ofColor::darkRed);
	circumscribe.moveTo(dstQuadPos[0]->x * width + b.x,dstQuadPos[0]->y * height + b.y);
	circumscribe.lineTo(dstQuadPos[1]->x * width + b.x,dstQuadPos[1]->y * height + b.y);
	circumscribe.moveTo(dstQuadPos[1]->x * width + b.x,dstQuadPos[1]->y * height + b.y);
	circumscribe.lineTo(dstQuadPos[2]->x * width + b.x,dstQuadPos[2]->y * height + b.y);
	circumscribe.moveTo(dstQuadPos[2]->x * width + b.x,dstQuadPos[2]->y * height + b.y);
	circumscribe.lineTo(dstQuadPos[3]->x * width + b.x,dstQuadPos[3]->y * height + b.y);
	circumscribe.moveTo(dstQuadPos[3]->x * width + b.x,dstQuadPos[3]->y * height + b.y);
	circumscribe.lineTo(dstQuadPos[0]->x * width + b.x,dstQuadPos[0]->y * height + b.y);
	circumscribe.draw();

	for (int i = 0; i < 4; i++){
		if (bCircle[i]){
			circle.clear();
			circle.setFillColor(ofColor(255,0,0));
			circle.setFilled(true);
			circle.circle(0,0,4);
			circle.draw(dstQuadPos[i]->x * width + b.x, dstQuadPos[i]->y * height + b.y);
		}else {
			circle.clear();
			circle.setFillColor(ofColor(0,255,0));
			circle.setFilled(true);
			circle.circle(0,0,3);
			circle.draw(dstQuadPos[i]->x * width + b.x, dstQuadPos[i]->y * height + b.y);
		}
	}

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
    ofSetColor(textColor);

    ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

ofAbstractParameter & ofxQuadWarp::getParameter(){
	dstQuadPosGroup.setName(name);
	dstQuadPosGroup.add(dstQuadPos[0]);
	dstQuadPosGroup.add(dstQuadPos[1]);
	dstQuadPosGroup.add(dstQuadPos[2]);
	dstQuadPosGroup.add(dstQuadPos[3]);
	return dstQuadPosGroup;
}

void ofxQuadWarp::valueChanged(string & value){
	generateDraw();
}

bool ofxQuadWarp::mouseMoved(ofMouseEventArgs & args){
	return false;
}

bool ofxQuadWarp::mousePressed(ofMouseEventArgs & args){
	if(setValue(args.x, args.y, true)){
		return true;
	}else{
		return false;
	}
}

bool ofxQuadWarp::mouseDragged(ofMouseEventArgs & args){
	for (int i = 0; i < 4; i++){
		if (bCircle[i]){
			ofRectangle check = ofRectangle(b.x,b.y,b.width,b.height);
			if (check.inside(args.x,args.y)){
				dstQuadPos[i]= ofVec3f((args.x - b.x)/width, (args.y - b.y)/height);
			}
		}
	}
	return false;
}

bool ofxQuadWarp::mouseReleased(ofMouseEventArgs & args){
	for (int i = 0; i < 4; i++){
		if (ofDistSquared(args.x, args.y,dstQuadPos[i]->x * width +b.x,dstQuadPos[i]->y * height + b.y)<MOUSE_DISTANCE * MOUSE_DISTANCE && (ofGetElapsedTimef() -lastMousePressTime)<1.0f ){
				switch (i){
				case 0:  dstQuadPos[0] = ofVec3f(0,0);                                        break;
				case 1:  dstQuadPos[1] = ofVec3f(content->getWidth(),0);                      break;
				case 2:  dstQuadPos[2] = ofVec3f(content->getWidth(),content->getHeight());   break;
				case 3:  dstQuadPos[3] = ofVec3f(0,content->getHeight());                     break;
				}
		}
	}
	lastMousePressTime = ofGetElapsedTimef();

	return false;
}

bool ofxQuadWarp::setValue(float mx, float my, bool bCheck){
	if( !isGuiDrawing() ){
		bGuiActive = false;
		return false;
	}
	for (int i = 0; i < 4; i++){
		bCircle[i] = false;
	}
	for (int i = 0; i < 4; i++)
	{
		if( bCheck ){
			if(ofDistSquared(mx,my,dstQuadPos[i]->x * width + b.x,dstQuadPos[i]->y * height + b.y)<MOUSE_DISTANCE * MOUSE_DISTANCE){
				bGuiActive = true;
			}else{
				bGuiActive = false;
			}
		}
		if( bGuiActive ){
			bCircle[i] = !bCircle[i];
			return true;
		}
	}
	return false;
}

void ofxQuadWarp::InitQuadPos(){
	dstQuadPos = new ofParameter<ofVec3f>[4];
	dstQuadPos[0].set("dstQuadPos0",ofVec3f(0,0));
	dstQuadPos[1].set("dstQuadPos1",ofVec3f(content->getWidth(),0));
	dstQuadPos[2].set("dstQuadPos2",ofVec3f(content->getWidth(),content->getHeight()));
	dstQuadPos[3].set("dstQuadPos3",ofVec3f(0,content->getHeight()));
	
	srcQuadPos = new ofPoint[4];
	srcQuadPos[0] = ofVec3f(0,0);
	srcQuadPos[1] = ofVec3f(content->getWidth(),0);
	srcQuadPos[2] = ofVec3f(content->getWidth(),content->getHeight());
	srcQuadPos[3] = ofVec3f(0,content->getHeight());
}

ofVec3f * ofxQuadWarp::getDstQuadPos(){ 
	_dstQuadPos = new ofVec3f[4];
	_dstQuadPos[0] = dstQuadPos[0];
	_dstQuadPos[1] = dstQuadPos[1];
	_dstQuadPos[2] = dstQuadPos[2];
	_dstQuadPos[3] = dstQuadPos[3];
	return _dstQuadPos; 
}

ofVec3f * ofxQuadWarp::getSrcQuadPos(){
	return srcQuadPos; 
}

void ofxQuadWarp::minimize(){
	quadWarpGroup.minimize();
}
void ofxQuadWarp::maximize(){
	quadWarpGroup.maximize();
}

void ofxQuadWarp::minimizeAll(){
	quadWarpGroup.minimizeAll();
}
void ofxQuadWarp::maximizeAll(){
	quadWarpGroup.maximizeAll();
}