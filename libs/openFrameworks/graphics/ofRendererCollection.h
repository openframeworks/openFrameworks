#pragma once

#include "ofBaseTypes.h"
#include "ofGLRenderer.h"
#include "ofGLProgrammableRenderer.h"
#include "of3dGraphics.h"
#include "ofPath.h"

class ofRendererCollection: public ofBaseRenderer{
public:
	ofRendererCollection():graphics3d(this){}
	 ~ofRendererCollection(){}

	 static const string TYPE;
	 const string & getType(){ return TYPE; }

	 shared_ptr<ofBaseGLRenderer> getGLRenderer(){
		 for(int i=0;i<(int)renderers.size();i++){
			 if(renderers[i]->getType()=="GL" || renderers[i]->getType()=="ProgrammableGL"){
				 return (shared_ptr<ofBaseGLRenderer>&)renderers[i];
			 }
		 }
		#ifndef TARGET_PROGRAMMABLE_GL
		 	 return shared_ptr<ofGLRenderer>();
		#else
		 	 return shared_ptr<ofGLProgrammableRenderer>();
		#endif
	 }

	 bool rendersPathPrimitives(){return true;}

	 void startRender(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->startRender();
		 }
	 }

	 void finishRender(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->finishRender();
		 }
	 }

	 void update(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->update();
		 }
	 }


	 using ofBaseRenderer::draw;

	 void draw(const ofPolyline & poly) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(poly);
		 }
	 }
	 void draw(const ofPath & shape) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(shape);
		 }
	 }

	 void draw(const ofMesh & vertexData, bool useColors, bool useTextures, bool useNormals) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(vertexData,useColors, useTextures, useNormals);
		 }
	 }

	 void draw(const ofMesh & vertexData, ofPolyRenderMode mode, bool useColors, bool useTextures, bool useNormals) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(vertexData,mode,useColors,useTextures,useNormals);
		 }
	 }

    void draw(const  of3dPrimitive& model, ofPolyRenderMode renderType ) const {
        for(int i=0;i<(int)renderers.size();i++) {
            renderers[i]->draw( model, renderType );
        }
    }

    void draw(const  ofNode& node) const {
        for(int i=0;i<(int)renderers.size();i++) {
            renderers[i]->draw( node );
        }
    }

	void draw(const ofImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(img,x,y,z,w,h,sx,sy,sw,sh);
		 }
	}

	void draw(const ofFloatImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->draw(img,x,y,z,w,h,sx,sy,sw,sh);
		}
	}

	void draw(const ofShortImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->draw(img,x,y,z,w,h,sx,sy,sw,sh);
		}
	}

	void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const{
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->draw(video,x,y,w,h);
		}
	}

	/*void bind(const ofBaseVideoDraws & video) const{
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->bind(video);
		}
	}

	void unbind(const ofBaseVideoDraws & video) const{
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->unbind(video);
		}
	}*/


	ofMatrix4x4 getCurrentMatrix(ofMatrixMode matrixMode_) const{
		static ofMatrix4x4 identityMatrix;
		if (!renderers.empty()) {
			return renderers.front()->getCurrentMatrix(matrixMode_);
		} else {
			ofLogWarning() << "No renderer in renderer collection, but current matrix requested. Returning identity matrix.";
			return identityMatrix;
		}
	};


	ofMatrix4x4 getCurrentOrientationMatrix() const{
		static ofMatrix4x4 identityMatrix;
		if (!renderers.empty()) {
			return renderers.front()->getCurrentOrientationMatrix();
		} else {
			ofLogWarning() << "No renderer in renderer collection, but current matrix requested. Returning identity matrix.";
			return identityMatrix;
		}
	};


	ofMatrix4x4 getCurrentNormalMatrix() const{
		static ofMatrix4x4 identityMatrix;
		if (!renderers.empty()) {
			return renderers.front()->getCurrentNormalMatrix();
		} else {
			ofLogWarning() << "No renderer in renderer collection, but current matrix requested. Returning identity matrix.";
			return identityMatrix;
		}
	};

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
	 void viewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=true){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->viewport(x,y,width,height);
		 }
	 }
	 void setupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setupScreenPerspective(width,height,fov,nearDist,farDist);
		 }
	 }
	 void setupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setupScreenOrtho(width,height,nearDist,farDist);
		 }
	 }
	 ofRectangle getCurrentViewport() const{
		 if(renderers.size()){
			 return renderers[0]->getCurrentViewport();
		 }else{
			 return ofRectangle();
		 }
	 }

	 ofRectangle getNativeViewport() const{
		 if(renderers.size()){
			 return renderers[0]->getNativeViewport();
		 }else{
			 return ofRectangle();
		 }
	 }

	 int getViewportWidth() const{
		 if(renderers.size()){
			 return renderers[0]->getViewportWidth();
		 }else{
			 return 0;
		 }
	 }
	 int getViewportHeight() const{
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
	 ofHandednessType getCoordHandedness() const{
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

	void loadIdentityMatrix (void){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->loadIdentityMatrix();
		}
	}

	void loadMatrix (const ofMatrix4x4 & m){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->loadMatrix( m );
		}
	}

	void loadMatrix (const float * m){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->loadMatrix( m );
		}
	}

	void multMatrix (const ofMatrix4x4 & m){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->multMatrix( m );
		}
	}

	void multMatrix (const float * m){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->multMatrix( m );
		}
	}

	void setOrientation(ofOrientation orientation, bool vflip){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->setOrientation( orientation, vflip );
		}
	}

	bool isVFlipped() const{
		if(!renderers.empty()){
			return renderers.front()->isVFlipped();
		}else{
			ofLogWarning() << "No renderer in renderer collection, but vflipped requested returning true.";
			return true;
		}
	}

	void matrixMode(ofMatrixMode mode){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->matrixMode( mode );
		}
	}

	void loadViewMatrix(const ofMatrix4x4& m){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->loadViewMatrix( m );
		}
	}

	void multViewMatrix(const ofMatrix4x4& m){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->multViewMatrix( m );
		}
	}

	ofMatrix4x4 getCurrentViewMatrix() const{
		if(!renderers.empty()){
			return renderers.front()->getCurrentViewMatrix();
		}else{
			ofLogWarning() << "No renderer in renderer collection, but current view matrix requested. Returning identity matrix.";
			return ofMatrix4x4::newIdentityMatrix();
		}
	}


	// screen coordinate things / default gl values
	 void setupGraphicDefaults(){
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->setupGraphicDefaults();
		}
		path.setMode(ofPath::COMMANDS);
		path.setUseShapeColor(false);
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
	ofColor getBackgroundColor(){
		 if(renderers.size()){
			 return renderers[0]->getBackgroundColor();
		 }else{
			 return ofColor(200);
		 }
	}

	void setBackgroundColor(const ofColor & color){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setBackgroundColor(color);
		 }
	}

	bool getBackgroundAuto(){
		 if(renderers.size()){
			 return renderers[0]->getBackgroundAuto();
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

	void clear(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->clear();
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
		if(fill==OF_FILLED){
			path.setFilled(true);
			path.setStrokeWidth(0);
		}else{
			path.setFilled(false);
			path.setStrokeWidth(getStyle().lineWidth);
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
		if(!getStyle().bFill){
			path.setStrokeWidth(lineWidth);
		}
	}

	void setDepthTest(bool depthTest) {
		for(int i=0;i<(int)renderers.size();i++){
			renderers[i]->setDepthTest(depthTest);
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
			 if(renderers[i]->getType()=="GL" || renderers[i]->getType()=="ProgrammableGL"){
				 ((shared_ptr<ofBaseGLRenderer>&)renderers[i])->enablePointSprites();
			 }
		 }
	}
	void disablePointSprites(){
		 for(int i=0;i<(int)renderers.size();i++){
			 if(renderers[i]->getType()=="GL" || renderers[i]->getType()=="ProgrammableGL"){
				 ((shared_ptr<ofBaseGLRenderer>&)renderers[i])->disablePointSprites();
			 }
		 }
	}

	void enableAntiAliasing(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->enableAntiAliasing();
		 }
	}

	void disableAntiAliasing(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->disableAntiAliasing();
		 }
	}

	void setBitmapTextMode(ofDrawBitmapMode mode){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setBitmapTextMode(mode);
		 }
	}

	ofStyle getStyle() const{
		if(renderers.empty()){
			return ofStyle();
		}else{
			return renderers[0]->getStyle();
		}
	}

	void pushStyle(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->pushStyle();
		 }
	}

	void popStyle(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->popStyle();
		 }
	}

	void setStyle(const ofStyle & style){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setStyle(style);
		 }
	}

	void setCurveResolution(int res){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setCurveResolution(res);
		 }
		 path.setCurveResolution(res);
	}

	void setPolyMode(ofPolyWindingMode mode){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setPolyMode(mode);
		 }
		 path.setPolyWindingMode(mode);
	}

	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawLine(x1,y1,z1,x2,y2,z2);
		 }
	}

	void drawRectangle(float x, float y, float z, float w, float h) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawRectangle(x,y,z,w,h);
		 }
	}

	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawTriangle(x1,y1,z1,x2,y2,z2,x3,y3,z3);
		 }
	}

	void drawCircle(float x, float y, float z, float radius) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawCircle(x,y,z,radius);
		 }
	}

	void drawEllipse(float x, float y, float z, float width, float height) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawEllipse(x,y,z,width,height);
		 }
	}

	void drawString(string text, float x, float y, float z) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawString(text, x,y,z);
		 }
	}

	void drawString(const ofTrueTypeFont & font, string text, float x, float y) const{
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->drawString(font, text, x,y);
		 }
	}

	virtual void bind(const ofCamera & camera, const ofRectangle & viewport){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->bind(camera, viewport);
		 }
	}
	virtual void unbind(const ofCamera & camera){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->unbind(camera);
		 }
	}

	const of3dGraphics & get3dGraphics() const{
		return graphics3d;
	}

	of3dGraphics & get3dGraphics(){
		return graphics3d;
	}

	ofPath & getPath(){
		return path;
	}

	vector<shared_ptr<ofBaseRenderer> > renderers;
	of3dGraphics graphics3d;
	ofPath path;
};
