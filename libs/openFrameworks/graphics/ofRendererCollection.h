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
		for(auto renderer: renderers){
			  if(renderer->getType()=="GL" || renderer->getType()=="ProgrammableGL"){
				  return dynamic_pointer_cast<ofBaseGLRenderer>(renderer);
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
		 for(auto renderer: renderers){
			 renderer->startRender();
		 }
	 }

	 void finishRender(){
		 for(auto renderer: renderers){
			 renderer->finishRender();
		 }
	 }


	 using ofBaseRenderer::draw;

	 void draw(const ofPolyline & poly) const{
		 for(auto renderer: renderers){
			 renderer->draw(poly);
		 }
	 }
	 void draw(const ofPath & shape) const{
		 for(auto renderer: renderers){
			 renderer->draw(shape);
		 }
	 }

	 void draw(const ofMesh & vertexData, ofPolyRenderMode mode, bool useColors, bool useTextures, bool useNormals) const{
		 for(auto renderer: renderers){
			 renderer->draw(vertexData,mode,useColors,useTextures,useNormals);
		 }
	 }

    void draw(const  of3dPrimitive& model, ofPolyRenderMode renderType ) const {
		for(auto renderer: renderers){
			renderer->draw( model, renderType );
        }
    }

    void draw(const  ofNode& node) const {
		for(auto renderer: renderers){
			renderer->draw( node );
        }
    }

	void draw(const ofImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
		for(auto renderer: renderers){
			renderer->draw(img,x,y,z,w,h,sx,sy,sw,sh);
		 }
	}

	void draw(const ofFloatImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
		for(auto renderer: renderers){
			renderer->draw(img,x,y,z,w,h,sx,sy,sw,sh);
		}
	}

	void draw(const ofShortImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
		for(auto renderer: renderers){
			renderer->draw(img,x,y,z,w,h,sx,sy,sw,sh);
		}
	}

	void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const{
		for(auto renderer: renderers){
			renderer->draw(video,x,y,w,h);
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


	glm::mat4 getCurrentMatrix(ofMatrixMode matrixMode_) const{
		if (!renderers.empty()) {
			return renderers.front()->getCurrentMatrix(matrixMode_);
		} else {
			ofLogWarning() << "No renderer in renderer collection, but current matrix requested. Returning identity matrix.";
			return glm::mat4(1.0f);
		}
	}


	glm::mat4 getCurrentOrientationMatrix() const{
		if (!renderers.empty()) {
			return renderers.front()->getCurrentOrientationMatrix();
		} else {
			ofLogWarning() << "No renderer in renderer collection, but current matrix requested. Returning identity matrix.";
			return glm::mat4(1.0f);
		}
	}


	glm::mat4 getCurrentNormalMatrix() const{
		if (!renderers.empty()) {
			return renderers.front()->getCurrentNormalMatrix();
		} else {
			ofLogWarning() << "No renderer in renderer collection, but current matrix requested. Returning identity matrix.";
			return glm::mat4(1.0f);
		}
	}

	//--------------------------------------------
	// transformations
	 void pushView(){
		 for(auto renderer: renderers){
			 renderer->pushView();
		 }
	 }

	 void popView(){
		 for(auto renderer: renderers){
			 renderer->popView();
		 }
	 }

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	void viewport(ofRectangle viewport){
		for(auto renderer: renderers){
			renderer->viewport(viewport);
		 }
	}

	 void viewport(float x = 0, float y = 0, float width = -1, float height = -1, bool vflip=true){
		 for(auto renderer: renderers){
			 renderer->viewport(x,y,width,height,vflip);
		 }
	 }

	 void setupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0){
		 for(auto renderer: renderers){
			 renderer->setupScreenPerspective(width,height,fov,nearDist,farDist);
		 }
	 }

	 void setupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1){
		 for(auto renderer: renderers){
			 renderer->setupScreenOrtho(width,height,nearDist,farDist);
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
		 for(auto renderer: renderers){
			 renderer->setCoordHandedness(handedness);
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
		 for(auto renderer: renderers){
			 renderer->pushMatrix();
		 }
	 }
	 void popMatrix(){
		 for(auto renderer: renderers){
			 renderer->popMatrix();
		 }
	 }
	 void translate(float x, float y, float z = 0){
		 for(auto renderer: renderers){
			 renderer->translate(x,y,z);
		 }
	 }
	 void translate(const glm::vec3 & p){
		 for(auto renderer: renderers){
			 renderer->translate(p);
		 }
	 }
	 void scale(float xAmnt, float yAmnt, float zAmnt = 1){
		 for(auto renderer: renderers){
			 renderer->scale(xAmnt,yAmnt,zAmnt);
		 }
	 }

	 void rotateDeg(float degrees, float vecX, float vecY, float vecZ){
		 for(auto renderer: renderers){
			 renderer->rotateDeg(degrees,vecX,vecY,vecZ);
		 }
	 }
	 void rotateXDeg(float degrees){
		 for(auto renderer: renderers){
			 renderer->rotateXDeg(degrees);
		 }
	 }
	 void rotateYDeg(float degrees){
		 for(auto renderer: renderers){
			 renderer->rotateYDeg(degrees);
		 }
	 }
	 void rotateZDeg(float degrees){
		 for(auto renderer: renderers){
			 renderer->rotateZDeg(degrees);
		 }
	 }
	 void rotateDeg(float degrees){
		 for(auto renderer: renderers){
			 renderer->rotateDeg(degrees);
		 }
	 }

	 void rotateRad(float radians, float vecX, float vecY, float vecZ){
		 for(auto renderer: renderers){
			 renderer->rotateRad(radians,vecX,vecY,vecZ);
		 }
	 }
	 void rotateXRad(float radians){
		 for(auto renderer: renderers){
			 renderer->rotateXRad(radians);
		 }
	 }
	 void rotateYRad(float radians){
		 for(auto renderer: renderers){
			 renderer->rotateYRad(radians);
		 }
	 }
	 void rotateZRad(float radians){
		 for(auto renderer: renderers){
			 renderer->rotateZRad(radians);
		 }
	 }
	 void rotateRad(float radians){
		 for(auto renderer: renderers){
			 renderer->rotateRad(radians);
		 }
	 }

	void loadIdentityMatrix (void){
		for(auto renderer: renderers){
			renderer->loadIdentityMatrix();
		}
	}

	void loadMatrix (const glm::mat4 & m){
		for(auto renderer: renderers){
			renderer->loadMatrix( m );
		}
	}

	void loadMatrix (const float * m){
		for(auto renderer: renderers){
			renderer->loadMatrix( m );
		}
	}

	void multMatrix (const glm::mat4 & m){
		for(auto renderer: renderers){
			renderer->multMatrix( m );
		}
	}

	void multMatrix (const float * m){
		for(auto renderer: renderers){
			renderer->multMatrix( m );
		}
	}

	void setOrientation(ofOrientation orientation, bool vflip){
		for(auto renderer: renderers){
			renderer->setOrientation( orientation, vflip );
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
		for(auto renderer: renderers){
			renderer->matrixMode( mode );
		}
	}

	void loadViewMatrix(const glm::mat4& m){
		for(auto renderer: renderers){
			renderer->loadViewMatrix( m );
		}
	}

	void multViewMatrix(const glm::mat4& m){
		for(auto renderer: renderers){
			renderer->multViewMatrix( m );
		}
	}

	glm::mat4 getCurrentViewMatrix() const{
		if(!renderers.empty()){
			return renderers.front()->getCurrentViewMatrix();
		}else{
			ofLogWarning() << "No renderer in renderer collection, but current view matrix requested. Returning identity matrix.";
			return glm::mat4(1.0);
		}
	}


	// screen coordinate things / default gl values
	 void setupGraphicDefaults(){
		 for(auto renderer: renderers){
			 renderer->setupGraphicDefaults();
		}
		path.setMode(ofPath::COMMANDS);
		path.setUseShapeColor(false);
	 }

	 void setupScreen(){
		 for(auto renderer: renderers){
			 renderer->setupScreen();
		 }
	 }

	// color options
	void setColor(int r, int g, int b){
		for(auto renderer: renderers){
			renderer->setColor(r,g,b);
		 }
	}

	void setColor(int r, int g, int b, int a){
		for(auto renderer: renderers){
			renderer->setColor(r,g,b,a);
		 }
	}

	void setColor(const ofColor & color){
		for(auto renderer: renderers){
			renderer->setColor(color);
		 }
	}

	void setColor(const ofColor & color, int _a){
		for(auto renderer: renderers){
			renderer->setColor(color,_a);
		 }
	}

	void setColor(int gray){
		for(auto renderer: renderers){
			renderer->setColor(gray);
		 }
	}

	void setHexColor( int hexColor ){
		for(auto renderer: renderers){
			renderer->setHexColor(hexColor);
		 }
	 } // hex, like web 0xFF0033;

	// bg color
	ofColor getBackgroundColor(){
		 if(renderers.size()){
			 return renderers[0]->getBackgroundColor();
		 }else{
			 return ofColor(200);
		 }
	}

	void setBackgroundColor(const ofColor & color){
		for(auto renderer: renderers){
			renderer->setBackgroundColor(color);
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
		for(auto renderer: renderers){
			renderer->background(c);
		 }
	}

	void background(float brightness){
		for(auto renderer: renderers){
			renderer->background(brightness);
		 }
	}

	void background(int hexColor, float _a=255.0f){
		for(auto renderer: renderers){
			renderer->background(hexColor,_a);
		 }
	}

	void background(int r, int g, int b, int a=255){
		for(auto renderer: renderers){
			renderer->background(r,g,b,a);
		 }
	}

	void setBackgroundAuto(bool bManual){
		for(auto renderer: renderers){
			renderer->setBackgroundAuto(bManual);
		 }
	}

	void clear(){
		for(auto renderer: renderers){
			renderer->clear();
		 }
	}

	void clear(float r, float g, float b, float a=0){
		for(auto renderer: renderers){
			renderer->clear(r,g,b,a);
		 }
	}

	void clear(float brightness, float a=0){
		for(auto renderer: renderers){
			renderer->clear(brightness,a);
		 }
	}

	void clearAlpha(){
		for(auto renderer: renderers){
			renderer->clearAlpha();
		 }
	}

	// drawing modes
	void setRectMode(ofRectMode mode){
		for(auto renderer: renderers){
			renderer->setRectMode(mode);
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
		for(auto renderer: renderers){
			renderer->setFillMode(fill);
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
		for(auto renderer: renderers){
			renderer->setLineWidth(lineWidth);
		}
		if(!getStyle().bFill){
			path.setStrokeWidth(lineWidth);
		}
	}

	void setDepthTest(bool depthTest) {
		for(auto renderer: renderers){
			renderer->setDepthTest(depthTest);
		}
	}

	void setBlendMode(ofBlendMode blendMode){
		for(auto renderer: renderers){
			renderer->setBlendMode(blendMode);
		 }
	}
	void setLineSmoothing(bool smooth){
		for(auto renderer: renderers){
			renderer->setLineSmoothing(smooth);
		 }
	}
	void setCircleResolution(int res){
		for(auto renderer: renderers){
			renderer->setCircleResolution(res);
		 }
	}
	void enablePointSprites(){
		for(auto renderer: renderers){
			 if(renderer->getType()=="GL" || renderer->getType()=="ProgrammableGL"){
				 dynamic_pointer_cast<ofBaseGLRenderer>(renderer)->enablePointSprites();
			 }
		 }
	}
	void disablePointSprites(){
		for(auto renderer: renderers){
			 if(renderer->getType()=="GL" || renderer->getType()=="ProgrammableGL"){
				 dynamic_pointer_cast<ofBaseGLRenderer>(renderer)->disablePointSprites();
			 }
		 }
	}

	void enableAntiAliasing(){
		for(auto renderer: renderers){
			renderer->enableAntiAliasing();
		 }
	}

	void disableAntiAliasing(){
		for(auto renderer: renderers){
			renderer->disableAntiAliasing();
		 }
	}

	void setBitmapTextMode(ofDrawBitmapMode mode){
		for(auto renderer: renderers){
			renderer->setBitmapTextMode(mode);
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
		for(auto renderer: renderers){
			renderer->pushStyle();
		 }
	}

	void popStyle(){
		for(auto renderer: renderers){
			renderer->popStyle();
		 }
	}

	void setStyle(const ofStyle & style){
		for(auto renderer: renderers){
			renderer->setStyle(style);
		 }
	}

	void setCurveResolution(int res){
		for(auto renderer: renderers){
			renderer->setCurveResolution(res);
		 }
		 path.setCurveResolution(res);
	}

	void setPolyMode(ofPolyWindingMode mode){
		for(auto renderer: renderers){
			renderer->setPolyMode(mode);
		 }
		 path.setPolyWindingMode(mode);
	}

	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const{
		for(auto renderer: renderers){
			renderer->drawLine(x1,y1,z1,x2,y2,z2);
		 }
	}

	void drawRectangle(float x, float y, float z, float w, float h) const{
		for(auto renderer: renderers){
			renderer->drawRectangle(x,y,z,w,h);
		 }
	}

	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const{
		for(auto renderer: renderers){
			renderer->drawTriangle(x1,y1,z1,x2,y2,z2,x3,y3,z3);
		 }
	}

	void drawCircle(float x, float y, float z, float radius) const{
		for(auto renderer: renderers){
			renderer->drawCircle(x,y,z,radius);
		 }
	}

	void drawEllipse(float x, float y, float z, float width, float height) const{
		for(auto renderer: renderers){
			renderer->drawEllipse(x,y,z,width,height);
		 }
	}

	void drawString(string text, float x, float y, float z) const{
		for(auto renderer: renderers){
			renderer->drawString(text, x,y,z);
		 }
	}

	void drawString(const ofTrueTypeFont & font, string text, float x, float y) const{
		for(auto renderer: renderers){
			renderer->drawString(font, text, x,y);
		 }
	}

	virtual void bind(const ofCamera & camera, const ofRectangle & viewport){
		for(auto renderer: renderers){
			renderer->bind(camera, viewport);
		 }
	}
	virtual void unbind(const ofCamera & camera){
		 for(auto renderer: renderers){
			 renderer->unbind(camera);
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
