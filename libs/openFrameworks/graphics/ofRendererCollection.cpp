/*
 * ofRendererCollection.cpp
 *
 *  Created on: Mar 30, 2013
 *      Author: arturo
 */

#include "ofRendererCollection.h"
#include "ofGLRenderer.h"
#include "ofGLProgrammableRenderer.h"

const std::string ofRendererCollection::TYPE="collection";

std::shared_ptr<ofBaseGLRenderer> ofRendererCollection::getGLRenderer(){
   for(auto renderer: renderers){
		 if(renderer->getType()=="GL" || renderer->getType()=="ProgrammableGL"){
			 return std::dynamic_pointer_cast<ofBaseGLRenderer>(renderer);
		 }
   }
   #ifndef TARGET_PROGRAMMABLE_GL
		return std::shared_ptr<ofGLRenderer>();
   #else
		return std::shared_ptr<ofGLProgrammableRenderer>();
   #endif
}

bool ofRendererCollection::rendersPathPrimitives(){return true;}

void ofRendererCollection::startRender(){
	for(auto renderer: renderers){
		renderer->startRender();
	}
}

void ofRendererCollection::finishRender(){
	for(auto renderer: renderers){
		renderer->finishRender();
	}
}

void ofRendererCollection::draw(const ofPolyline & poly) const{
	for(auto renderer: renderers){
		renderer->draw(poly);
	}
}
void ofRendererCollection::draw(const ofPath & shape) const{
	for(auto renderer: renderers){
		renderer->draw(shape);
	}
}

void ofRendererCollection::draw(const ofMesh & vertexData, ofPolyRenderMode mode, bool useColors, bool useTextures, bool useNormals) const{
	for(auto renderer: renderers){
		renderer->draw(vertexData,mode,useColors,useTextures,useNormals);
	}
}

void ofRendererCollection::draw(const  of3dPrimitive& model, ofPolyRenderMode renderType ) const {
   for(auto renderer: renderers){
	   renderer->draw( model, renderType );
   }
}

void ofRendererCollection::draw(const  ofNode& node) const {
   for(auto renderer: renderers){
	   renderer->draw( node );
   }
}

void ofRendererCollection::draw(const ofImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
   for(auto renderer: renderers){
	   renderer->draw(img,x,y,z,w,h,sx,sy,sw,sh);
	}
}

void ofRendererCollection::draw(const ofFloatImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
   for(auto renderer: renderers){
	   renderer->draw(img,x,y,z,w,h,sx,sy,sw,sh);
   }
}

void ofRendererCollection::draw(const ofShortImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
   for(auto renderer: renderers){
	   renderer->draw(img,x,y,z,w,h,sx,sy,sw,sh);
   }
}

void ofRendererCollection::draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const{
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


glm::mat4 ofRendererCollection::getCurrentMatrix(ofMatrixMode matrixMode_) const{
   if (!renderers.empty()) {
	   return renderers.front()->getCurrentMatrix(matrixMode_);
   } else {
	   ofLogWarning() << "No renderer in renderer collection, but current matrix requested. Returning identity matrix.";
	   return glm::mat4(1.0f);
   }
}


glm::mat4 ofRendererCollection::getCurrentOrientationMatrix() const{
   if (!renderers.empty()) {
	   return renderers.front()->getCurrentOrientationMatrix();
   } else {
	   ofLogWarning() << "No renderer in renderer collection, but current matrix requested. Returning identity matrix.";
	   return glm::mat4(1.0f);
   }
}


glm::mat4 ofRendererCollection::getCurrentNormalMatrix() const{
   if (!renderers.empty()) {
	   return renderers.front()->getCurrentNormalMatrix();
   } else {
	   ofLogWarning() << "No renderer in renderer collection, but current matrix requested. Returning identity matrix.";
	   return glm::mat4(1.0f);
   }
}

//--------------------------------------------
// transformations
void ofRendererCollection::pushView(){
	for(auto renderer: renderers){
		renderer->pushView();
	}
}

void ofRendererCollection::popView(){
	for(auto renderer: renderers){
		renderer->popView();
	}
}

// setup matrices and viewport (upto you to push and pop view before and after)
// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
void ofRendererCollection::viewport(ofRectangle viewport){
   for(auto renderer: renderers){
	   renderer->viewport(viewport);
	}
}

void ofRendererCollection::viewport(float x, float y, float width, float height, bool vflip){
	for(auto renderer: renderers){
		renderer->viewport(x,y,width,height,vflip);
	}
}

void ofRendererCollection::setupScreenPerspective(float width, float height, float fov, float nearDist, float farDist){
	for(auto renderer: renderers){
		renderer->setupScreenPerspective(width,height,fov,nearDist,farDist);
	}
}

void ofRendererCollection::setupScreenOrtho(float width, float height, float nearDist, float farDist){
	for(auto renderer: renderers){
		renderer->setupScreenOrtho(width,height,nearDist,farDist);
	}
}

ofRectangle ofRendererCollection::getCurrentViewport() const{
	if(renderers.size()){
		return renderers[0]->getCurrentViewport();
	}else{
		return ofRectangle();
	}
}

ofRectangle ofRendererCollection::getNativeViewport() const{
	if(renderers.size()){
		return renderers[0]->getNativeViewport();
	}else{
		return ofRectangle();
	}
}

int ofRendererCollection::getViewportWidth() const{
	if(renderers.size()){
		return renderers[0]->getViewportWidth();
	}else{
		return 0;
	}
}
int ofRendererCollection::getViewportHeight() const{
	if(renderers.size()){
		return renderers[0]->getViewportHeight();
	}else{
		return 0;
	}
}

void ofRendererCollection::setCoordHandedness(ofHandednessType handedness){
	for(auto renderer: renderers){
		renderer->setCoordHandedness(handedness);
	}
}
ofHandednessType ofRendererCollection::getCoordHandedness() const{
	if(renderers.size()){
		return renderers[0]->getCoordHandedness();
	}else{
		return OF_LEFT_HANDED;
	}
}

//our openGL wrappers
void ofRendererCollection::pushMatrix(){
	for(auto renderer: renderers){
		renderer->pushMatrix();
	}
}
void ofRendererCollection::popMatrix(){
	for(auto renderer: renderers){
		renderer->popMatrix();
	}
}
void ofRendererCollection::translate(float x, float y, float z){
	for(auto renderer: renderers){
		renderer->translate(x,y,z);
	}
}
void ofRendererCollection::translate(const glm::vec3 & p){
	for(auto renderer: renderers){
		renderer->translate(p);
	}
}
void ofRendererCollection::scale(float xAmnt, float yAmnt, float zAmnt){
	for(auto renderer: renderers){
		renderer->scale(xAmnt,yAmnt,zAmnt);
	}
}

void ofRendererCollection::rotateDeg(float degrees, float vecX, float vecY, float vecZ){
	for(auto renderer: renderers){
		renderer->rotateDeg(degrees,vecX,vecY,vecZ);
	}
}
void ofRendererCollection::rotateXDeg(float degrees){
	for(auto renderer: renderers){
		renderer->rotateXDeg(degrees);
	}
}
void ofRendererCollection::rotateYDeg(float degrees){
	for(auto renderer: renderers){
		renderer->rotateYDeg(degrees);
	}
}
void ofRendererCollection::rotateZDeg(float degrees){
	for(auto renderer: renderers){
		renderer->rotateZDeg(degrees);
	}
}
void ofRendererCollection::rotateDeg(float degrees){
	for(auto renderer: renderers){
		renderer->rotateDeg(degrees);
	}
}

void ofRendererCollection::rotateRad(float radians, float vecX, float vecY, float vecZ){
	for(auto renderer: renderers){
		renderer->rotateRad(radians,vecX,vecY,vecZ);
	}
}
void ofRendererCollection::rotateXRad(float radians){
	for(auto renderer: renderers){
		renderer->rotateXRad(radians);
	}
}
void ofRendererCollection::rotateYRad(float radians){
	for(auto renderer: renderers){
		renderer->rotateYRad(radians);
	}
}
void ofRendererCollection::rotateZRad(float radians){
	for(auto renderer: renderers){
		renderer->rotateZRad(radians);
	}
}
void ofRendererCollection::rotateRad(float radians){
	for(auto renderer: renderers){
		renderer->rotateRad(radians);
	}
}

void ofRendererCollection::loadIdentityMatrix (void){
   for(auto renderer: renderers){
	   renderer->loadIdentityMatrix();
   }
}

void ofRendererCollection::loadMatrix (const glm::mat4 & m){
   for(auto renderer: renderers){
	   renderer->loadMatrix( m );
   }
}

void ofRendererCollection::loadMatrix (const float * m){
   for(auto renderer: renderers){
	   renderer->loadMatrix( m );
   }
}

void ofRendererCollection::multMatrix (const glm::mat4 & m){
   for(auto renderer: renderers){
	   renderer->multMatrix( m );
   }
}

void ofRendererCollection::multMatrix (const float * m){
   for(auto renderer: renderers){
	   renderer->multMatrix( m );
   }
}

void ofRendererCollection::setOrientation(ofOrientation orientation, bool vflip){
   for(auto renderer: renderers){
	   renderer->setOrientation( orientation, vflip );
   }
}

bool ofRendererCollection::isVFlipped() const{
   if(!renderers.empty()){
	   return renderers.front()->isVFlipped();
   }else{
	   ofLogWarning() << "No renderer in renderer collection, but vflipped requested returning true.";
	   return true;
   }
}

void ofRendererCollection::matrixMode(ofMatrixMode mode){
   for(auto renderer: renderers){
	   renderer->matrixMode( mode );
   }
}

void ofRendererCollection::loadViewMatrix(const glm::mat4& m){
   for(auto renderer: renderers){
	   renderer->loadViewMatrix( m );
   }
}

void ofRendererCollection::multViewMatrix(const glm::mat4& m){
   for(auto renderer: renderers){
	   renderer->multViewMatrix( m );
   }
}

glm::mat4 ofRendererCollection::getCurrentViewMatrix() const{
   if(!renderers.empty()){
	   return renderers.front()->getCurrentViewMatrix();
   }else{
	   ofLogWarning() << "No renderer in renderer collection, but current view matrix requested. Returning identity matrix.";
	   return glm::mat4(1.0);
   }
}


// screen coordinate things / default gl values
void ofRendererCollection::setupGraphicDefaults(){
	for(auto renderer: renderers){
		renderer->setupGraphicDefaults();
   }
   path.setMode(ofPath::COMMANDS);
   path.setUseShapeColor(false);
}

void ofRendererCollection::setupScreen(){
	for(auto renderer: renderers){
		renderer->setupScreen();
	}
}

// color options
void ofRendererCollection::setColor(int r, int g, int b){
   for(auto renderer: renderers){
	   renderer->setColor(r,g,b);
	}
}

void ofRendererCollection::setColor(int r, int g, int b, int a){
   for(auto renderer: renderers){
	   renderer->setColor(r,g,b,a);
	}
}

void ofRendererCollection::setColor(const ofColor & color){
   for(auto renderer: renderers){
	   renderer->setColor(color);
	}
}

void ofRendererCollection::setColor(const ofColor & color, int _a){
   for(auto renderer: renderers){
	   renderer->setColor(color,_a);
	}
}

void ofRendererCollection::setColor(int gray){
   for(auto renderer: renderers){
	   renderer->setColor(gray);
	}
}

void ofRendererCollection::ofRendererCollection::setHexColor( int hexColor ){
   for(auto renderer: renderers){
	   renderer->setHexColor(hexColor);
	}
} // hex, like web 0xFF0033;

// bg color
ofColor ofRendererCollection::getBackgroundColor(){
	if(renderers.size()){
		return renderers[0]->getBackgroundColor();
	}else{
		return ofColor(200);
	}
}

void ofRendererCollection::setBackgroundColor(const ofColor & color){
   for(auto renderer: renderers){
	   renderer->setBackgroundColor(color);
	}
}

bool ofRendererCollection::getBackgroundAuto(){
	if(renderers.size()){
		return renderers[0]->getBackgroundAuto();
	}else{
		return true;
	}
}

void ofRendererCollection::background(const ofColor & c){
   for(auto renderer: renderers){
	   renderer->background(c);
	}
}

void ofRendererCollection::background(float brightness){
   for(auto renderer: renderers){
	   renderer->background(brightness);
	}
}

void ofRendererCollection::background(int hexColor, float _a){
   for(auto renderer: renderers){
	   renderer->background(hexColor,_a);
	}
}

void ofRendererCollection::background(int r, int g, int b, int a){
   for(auto renderer: renderers){
	   renderer->background(r,g,b,a);
	}
}

void ofRendererCollection::setBackgroundAuto(bool bManual){
   for(auto renderer: renderers){
	   renderer->setBackgroundAuto(bManual);
	}
}

void ofRendererCollection::clear(){
   for(auto renderer: renderers){
	   renderer->clear();
	}
}

void ofRendererCollection::clear(float r, float g, float b, float a){
   for(auto renderer: renderers){
	   renderer->clear(r,g,b,a);
	}
}

void ofRendererCollection::clear(float brightness, float a){
   for(auto renderer: renderers){
	   renderer->clear(brightness,a);
	}
}

void ofRendererCollection::clearAlpha(){
   for(auto renderer: renderers){
	   renderer->clearAlpha();
	}
}

// drawing modes
void ofRendererCollection::setRectMode(ofRectMode mode){
   for(auto renderer: renderers){
	   renderer->setRectMode(mode);
	}
}

ofRectMode ofRendererCollection::getRectMode(){
   if(renderers.empty()){
	   return OF_RECTMODE_CORNER;
   }else{
	   return renderers[0]->getRectMode();
   }
}

void ofRendererCollection::setFillMode(ofFillFlag fill){
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

ofFillFlag ofRendererCollection::getFillMode(){
   if(renderers.empty()){
	   return OF_FILLED;
   }else{
	   return renderers[0]->getFillMode();
   }
}

void ofRendererCollection::setLineWidth(float lineWidth){
   for(auto renderer: renderers){
	   renderer->setLineWidth(lineWidth);
   }
   if(!getStyle().bFill){
	   path.setStrokeWidth(lineWidth);
   }
}

void ofRendererCollection::setDepthTest(bool depthTest) {
   for(auto renderer: renderers){
	   renderer->setDepthTest(depthTest);
   }
}

void ofRendererCollection::setBlendMode(ofBlendMode blendMode){
   for(auto renderer: renderers){
	   renderer->setBlendMode(blendMode);
	}
}
void ofRendererCollection::setLineSmoothing(bool smooth){
   for(auto renderer: renderers){
	   renderer->setLineSmoothing(smooth);
	}
}
void ofRendererCollection::setCircleResolution(int res){
   for(auto renderer: renderers){
	   renderer->setCircleResolution(res);
	}
}
void ofRendererCollection::enablePointSprites(){
   for(auto renderer: renderers){
		if(renderer->getType()=="GL" || renderer->getType()=="ProgrammableGL"){
			std::dynamic_pointer_cast<ofBaseGLRenderer>(renderer)->enablePointSprites();
		}
	}
}
void ofRendererCollection::disablePointSprites(){
   for(auto renderer: renderers){
		if(renderer->getType()=="GL" || renderer->getType()=="ProgrammableGL"){
			std::dynamic_pointer_cast<ofBaseGLRenderer>(renderer)->disablePointSprites();
		}
	}
}

void ofRendererCollection::enableAntiAliasing(){
   for(auto renderer: renderers){
	   renderer->enableAntiAliasing();
	}
}

void ofRendererCollection::disableAntiAliasing(){
   for(auto renderer: renderers){
	   renderer->disableAntiAliasing();
	}
}

void ofRendererCollection::setBitmapTextMode(ofDrawBitmapMode mode){
   for(auto renderer: renderers){
	   renderer->setBitmapTextMode(mode);
	}
}

ofStyle ofRendererCollection::getStyle() const{
   if(renderers.empty()){
	   return ofStyle();
   }else{
	   return renderers[0]->getStyle();
   }
}

void ofRendererCollection::pushStyle(){
   for(auto renderer: renderers){
	   renderer->pushStyle();
	}
}

void ofRendererCollection::popStyle(){
   for(auto renderer: renderers){
	   renderer->popStyle();
	}
}

void ofRendererCollection::setStyle(const ofStyle & style){
   for(auto renderer: renderers){
	   renderer->setStyle(style);
	}
}

void ofRendererCollection::setCurveResolution(int res){
   for(auto renderer: renderers){
	   renderer->setCurveResolution(res);
	}
	path.setCurveResolution(res);
}

void ofRendererCollection::setPolyMode(ofPolyWindingMode mode){
   for(auto renderer: renderers){
	   renderer->setPolyMode(mode);
	}
	path.setPolyWindingMode(mode);
}

// drawing
void ofRendererCollection::drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const{
   for(auto renderer: renderers){
	   renderer->drawLine(x1,y1,z1,x2,y2,z2);
	}
}

void ofRendererCollection::drawRectangle(float x, float y, float z, float w, float h) const{
   for(auto renderer: renderers){
	   renderer->drawRectangle(x,y,z,w,h);
	}
}

void ofRendererCollection::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const{
   for(auto renderer: renderers){
	   renderer->drawTriangle(x1,y1,z1,x2,y2,z2,x3,y3,z3);
	}
}

void ofRendererCollection::drawCircle(float x, float y, float z, float radius) const{
   for(auto renderer: renderers){
	   renderer->drawCircle(x,y,z,radius);
	}
}

void ofRendererCollection::drawEllipse(float x, float y, float z, float width, float height) const{
   for(auto renderer: renderers){
	   renderer->drawEllipse(x,y,z,width,height);
	}
}

void ofRendererCollection::drawString(std::string text, float x, float y, float z) const{
   for(auto renderer: renderers){
	   renderer->drawString(text, x,y,z);
	}
}

void ofRendererCollection::drawString(const ofTrueTypeFont & font, std::string text, float x, float y) const{
   for(auto renderer: renderers){
	   renderer->drawString(font, text, x,y);
	}
}

void ofRendererCollection::bind(const ofCamera & camera, const ofRectangle & viewport){
   for(auto renderer: renderers){
	   renderer->bind(camera, viewport);
	}
}
void ofRendererCollection::unbind(const ofCamera & camera){
	for(auto renderer: renderers){
		renderer->unbind(camera);
	}
}

const of3dGraphics & ofRendererCollection::get3dGraphics() const{
   return graphics3d;
}

of3dGraphics & ofRendererCollection::get3dGraphics(){
   return graphics3d;
}

ofPath & ofRendererCollection::getPath(){
   return path;
}
