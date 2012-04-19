#pragma once

#include "ofBaseTypes.h"

class ofRendererCollection: public ofBaseRenderer{
public:
	 ~ofRendererCollection(){}

	 string getType(){ return "collection"; }

	 ofPtr<ofGLRenderer> getGLRenderer(){
		 for(int i=0;i<(int)renderers.size();i++){
			 if(renderers[i]->getType()=="GL"){
				 return (ofPtr<ofGLRenderer>&)renderers[i];
			 }
		 }
		 return ofPtr<ofGLRenderer>();
	 }

	 bool rendersPathPrimitives(){return true;}

	 void update(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->update();
		 }
	 }


	 void draw(ofPolyline & poly){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(poly);
		 }
	 }
	 void draw(ofPath & shape){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(shape);
		 }
	 }
	 void draw(ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(vertexData,useColors, useTextures, useNormals);
		 }
	 }

	 void draw(ofMesh & vertexData, ofPolyRenderMode mode, bool useColors, bool useTextures, bool useNormals){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(vertexData,mode,useColors,useTextures,useNormals);
		 }
	 }

	void draw(vector<ofPoint> & vertexData, ofPrimitiveMode drawMode){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(vertexData,drawMode);
		 }
	}

	void draw(ofImage & img, float x, float y, float z, float w, float h){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(img,x,y,z,w,h);
		 }
	}
	
	void draw(ofFloatImage & img, float x, float y, float z, float w, float h){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->draw(img,x,y,z,w,h);
		}
	}
	
	void draw(ofShortImage & img, float x, float y, float z, float w, float h){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->draw(img,x,y,z,w,h);
		}
	}
	


	//--------------------------------------------
	// transformations
	 void pushView(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->pushView();
		 }
	 }
	 void popView(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->popView();
		 }
	 }

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	void viewport(ofRectangle viewport){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->viewport(viewport);
		 }
	}
	 void viewport(float x = 0, float y = 0, float width = 0, float height = 0, bool invertY = true){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->viewport(x,y,width,height,invertY);
		 }
	 }
	 void setupScreenPerspective(float width = 0, float height = 0, ofOrientation orientation=OF_ORIENTATION_UNKNOWN, bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0, const ofVec2f & lensOffset = ofVec2f(0.0f, 0.0f)){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setupScreenPerspective(width,height,orientation,vFlip,fov,nearDist,farDist,lensOffset);
		 }
	 }
	 void setupScreenOrtho(float width = 0, float height = 0, ofOrientation orientation=OF_ORIENTATION_UNKNOWN, bool vFlip = true, float nearDist = -1, float farDist = 1){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setupScreenOrtho(width,height,orientation,vFlip,nearDist,farDist);
		 }
	 }
	 ofRectangle getCurrentViewport(){
		 if(renderers.size()){
			 return renderers[0]->getCurrentViewport();
		 }else{
			 return ofRectangle();
		 }
	 }
	 int getViewportWidth(){
		 if(renderers.size()){
			 return renderers[0]->getViewportWidth();
		 }else{
			 return 0;
		 }
	 }
	 int getViewportHeight(){
		 if(renderers.size()){
			 return renderers[0]->getViewportHeight();
		 }else{
			 return 0;
		 }
	 }

	 void setCoordHandedness(ofHandednessType handedness){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setCoordHandedness(handedness);
		 }
	 }
	 ofHandednessType getCoordHandedness(){
		 if(renderers.size()){
			 return renderers[0]->getCoordHandedness();
		 }else{
			 return OF_LEFT_HANDED;
		 }
	 }

	//our openGL wrappers
	 void pushMatrix(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->pushMatrix();
		 }
	 }
	 void popMatrix(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->popMatrix();
		 }
	 }
	 void translate(float x, float y, float z = 0){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->translate(x,y,z);
		 }
	 }
	 void translate(const ofPoint & p){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->translate(p);
		 }
	 }
	 void scale(float xAmnt, float yAmnt, float zAmnt = 1){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->scale(xAmnt,yAmnt,zAmnt);
		 }
	 }
	 void rotate(float degrees, float vecX, float vecY, float vecZ){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->rotate(degrees,vecX,vecY,vecZ);
		 }
	 }
	 void rotateX(float degrees){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->rotateX(degrees);
		 }
	 }
	 void rotateY(float degrees){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->rotateY(degrees);
		 }
	 }
	 void rotateZ(float degrees){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->rotateZ(degrees);
		 }
	 }
	 void rotate(float degrees){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->rotate(degrees);
		 }
	 }

	// screen coordinate things / default gl values
	 void setupGraphicDefaults(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setupGraphicDefaults();
		 }
	 }
	 void setupScreen(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setupScreen();
		 }
	 }

	// color options
	void setColor(int r, int g, int b){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setColor(r,g,b);
		 }
	}

	void setColor(int r, int g, int b, int a){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setColor(r,g,b,a);
		 }
	}

	void setColor(const ofColor & color){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setColor(color);
		 }
	}

	void setColor(const ofColor & color, int _a){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setColor(color,_a);
		 }
	}

	void setColor(int gray){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setColor(gray);
		 }
	};

	void setHexColor( int hexColor ){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setHexColor(hexColor);
		 }
	 }; // hex, like web 0xFF0033;

	// bg color
	ofFloatColor & getBgColor(){
		 if(renderers.size()){
			 return renderers[0]->getBgColor();
		 }else{
			 static ofFloatColor c;
			 return c;
		 }
	}

	bool bClearBg(){
		 if(renderers.size()){
			 return renderers[0]->bClearBg();
		 }else{
			 return true;
		 }
	}

	void background(const ofColor & c){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->background(c);
		 }
	}

	void background(float brightness){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->background(brightness);
		 }
	}

	void background(int hexColor, float _a=255.0f){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->background(hexColor,_a);
		 }
	}

	void background(int r, int g, int b, int a=255){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->background(r,g,b,a);
		 }
	}

	void setBackgroundAuto(bool bManual){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setBackgroundAuto(bManual);
		 }
	}

	void clear(float r, float g, float b, float a=0){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->clear(r,g,b,a);
		 }
	}
	void clear(float brightness, float a=0){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->clear(brightness,a);
		 }
	}
	void clearAlpha(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->clearAlpha();
		 }
	}

	// drawing modes
	void setRectMode(ofRectMode mode){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setRectMode(mode);
		 }
	}

	ofRectMode getRectMode(){
		if(renderers.empty()){
			return OF_RECTMODE_CORNER;
		}else{
			return renderers[0]->getRectMode();
		}
	}

	void setFillMode(ofFillFlag fill){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setFillMode(fill);
		 }
	}

	ofFillFlag getFillMode(){
		if(renderers.empty()){
			return OF_FILLED;
		}else{
			return renderers[0]->getFillMode();
		}
	}

	void setLineWidth(float lineWidth){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setLineWidth(lineWidth);
		 }
	}

	void setBlendMode(ofBlendMode blendMode){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setBlendMode(blendMode);
		 }
	}
	void setLineSmoothing(bool smooth){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setLineSmoothing(smooth);
		 }
	}
	void setCircleResolution(int res){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setCircleResolution(res);
		 }
	}
	void enablePointSprites(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->enablePointSprites();
		 }
	}
	void disablePointSprites(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->disablePointSprites();
		 }
	}

	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawLine(x1,y1,z1,x2,y2,z2);
		 }
	}

	void drawRectangle(float x, float y, float z, float w, float h){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawRectangle(x,y,z,w,h);
		 }
	}

	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawTriangle(x1,y1,z1,x2,y2,z2,x3,y3,z3);
		 }
	}

	void drawCircle(float x, float y, float z, float radius){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawCircle(x,y,z,radius);
		 }
	}
	
	void drawSphere(float x, float y, float z, float radius) {
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->drawSphere(x,y,z,radius);
		}
	}

	void drawEllipse(float x, float y, float z, float width, float height){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawEllipse(x,y,z,width,height);
		 }
	}

	void drawString(string text, float x, float y, float z, ofDrawBitmapMode mode){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawString(text, x,y,z,mode);
		 }
	}

	vector<ofPtr<ofBaseRenderer> > renderers;
};
