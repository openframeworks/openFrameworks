#include "ofBaseTypes.h"
#include "ofUtils.h"
#include "ofGLUtils.h"
#include "ofGLProgrammableRenderer.h"
#include "of3dGraphics.h"


//---------------------------------------------------------------------------
ofBaseVideoGrabber::~ofBaseVideoGrabber(){
	
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setVerbose(bool bTalkToMe){
	ofLogWarning("ofBaseVideoGrabber") << "setVerbose() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setDeviceID(int _deviceID){
	ofLogWarning("ofBaseVideoGrabber") << "setDeviceID() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::setDesiredFrameRate(int framerate){
	ofLogWarning("ofBaseVideoGrabber") << "setDesiredFrameRate() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoGrabber::videoSettings(){
	ofLogWarning("ofBaseVideoGrabber") << "videoSettings() not implemented";
}

//---------------------------------------------------------------------------
//void ofBaseVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat){
//	ofLogWarning("ofBaseVideoGrabber") << "setPixelFormat() not implemented";
//}
//
//ofPixelFormat ofBaseVideoGrabber::getPixelFormat(){
//	ofLogWarning("ofBaseVideoGrabber") << "getPixelFormat() not implemented";
//	return OF_PIXELS_RGB;
//}

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
ofBaseVideoPlayer::~ofBaseVideoPlayer(){

}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getPosition() const {
	ofLogWarning("ofBaseVideoPlayer") << "getPosition() not implemented";
	return 0.0;
}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getSpeed() const {
	ofLogWarning("ofBaseVideoPlayer") << "getSpeed() not implemented";
	return 0.0;
}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getDuration() const {
	ofLogWarning("ofBaseVideoPlayer") << "getDuration() not implemented";
	return 0.0;
}

//---------------------------------------------------------------------------
bool ofBaseVideoPlayer::getIsMovieDone() const {
	ofLogWarning("ofBaseVideoPlayer") << "getIsMovieDone() not implemented";
	return false;
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setPaused(bool bPause){
	ofLogWarning("ofBaseVideoPlayer") << "setPaused() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setPosition(float pct){
	ofLogWarning("ofBaseVideoPlayer") << "setPosition() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setVolume(float volume){
	ofLogWarning("ofBaseVideoPlayer") << "setVolume() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setLoopState(ofLoopType state){
	ofLogWarning("ofBaseVideoPlayer") << "setLoopState() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setSpeed(float speed){
	ofLogWarning("ofBaseVideoPlayer") << "setSpeed() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setFrame(int frame){
	ofLogWarning("ofBaseVideoPlayer") << "setFrame() not implemented";
}

//---------------------------------------------------------------------------
int	ofBaseVideoPlayer::getCurrentFrame() const {
	ofLogWarning("ofBaseVideoPlayer") << "getCurrentFrame() not implemented";
	return 0;
}

//---------------------------------------------------------------------------
int	ofBaseVideoPlayer::getTotalNumFrames() const {
	ofLogWarning("ofBaseVideoPlayer") << "getTotalNumFrames() not implemented";
	return 0;
}

//---------------------------------------------------------------------------
ofLoopType ofBaseVideoPlayer::getLoopState() const {
	ofLogWarning("ofBaseVideoPlayer") << "getLoopState() not implemented";
	return OF_LOOP_NONE;
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::firstFrame(){
	ofLogWarning("ofBaseVideoPlayer") << "firstFrame() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::nextFrame(){
	ofLogWarning("ofBaseVideoPlayer") << "nextFrame() not implemented";
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::previousFrame(){
	ofLogWarning("ofBaseVideoPlayer") << "previousFrame() not implemented";
}

//---------------------------------------------------------------------------
ofMatrix4x4 ofBaseRenderer::getCurrentOrientationMatrix() const {
	ofLogWarning() << "getCurrentOrientationMatrix() Not implemented for this renderer. Returning Identity matrix.";
	return ofMatrix4x4();
}

void ofBaseRenderer::setPlaneResolution( int columns, int rows ){
	get3dGraphics().setPlaneResolution(columns,rows);
}

ofVec2f ofBaseRenderer::getPlaneResolution() const{
	return get3dGraphics().getPlaneResolution();
}

void ofBaseRenderer::drawPlane(float x, float y, float width, float height) const{
	get3dGraphics().drawPlane(x,y,width,height);
}

void ofBaseRenderer::drawPlane(float x, float y, float z, float width, float height) const{
	get3dGraphics().drawPlane(x,y,z,width,height);
}

void ofBaseRenderer::drawPlane(ofPoint& position, float width, float height) const{
	get3dGraphics().drawPlane(position,width,height);
}

void ofBaseRenderer::drawPlane( float width, float height ) const{
	get3dGraphics().drawPlane(width,height);
}

void ofBaseRenderer::setSphereResolution(int res){
	get3dGraphics().setSphereResolution(res);
}

int ofBaseRenderer::getSphereResolution() const{
	return get3dGraphics().getSphereResolution();
}

void ofBaseRenderer::drawSphere(float x, float y, float radius) const{
	get3dGraphics().drawSphere(x,y,radius);
}

void ofBaseRenderer::drawSphere(float x, float y, float z, float radius) const{
	get3dGraphics().drawSphere(x,y,z,radius);
}

void ofBaseRenderer::drawSphere(const ofPoint& position, float radius) const{
	get3dGraphics().drawSphere(position,radius);
}

void ofBaseRenderer::drawSphere(float radius) const{
	get3dGraphics().drawSphere(radius);
}

void ofBaseRenderer::setIcoSphereResolution( int res ){
	get3dGraphics().setIcoSphereResolution(res);
}

int ofBaseRenderer::getIcoSphereResolution() const{
	return get3dGraphics().getIcoSphereResolution();
}

void ofBaseRenderer::drawIcoSphere(float x, float y, float z, float radius) const{
	get3dGraphics().drawIcoSphere(x,y,z,radius);
}

void ofBaseRenderer::drawIcoSphere(float x, float y, float radius) const{
	get3dGraphics().drawIcoSphere(x,y,radius);
}

void ofBaseRenderer::drawIcoSphere(const ofPoint& position, float radius) const{
	get3dGraphics().drawIcoSphere(position,radius);
}

void ofBaseRenderer::drawIcoSphere(float radius) const{
	get3dGraphics().drawIcoSphere(radius);
}

void ofBaseRenderer::setCylinderResolution( int radiusSegments, int heightSegments, int capSegments ){
	get3dGraphics().setCylinderResolution(radiusSegments,heightSegments,capSegments);
}

ofVec3f ofBaseRenderer::getCylinderResolution() const{
	return get3dGraphics().getCylinderResolution();
}

void ofBaseRenderer::drawCylinder(float x, float y, float radius, float height) const{
	get3dGraphics().drawCylinder(x,y,radius,height);
}

void ofBaseRenderer::drawCylinder(float x, float y, float z, float radius, float height) const{
	get3dGraphics().drawCylinder(x,y,z,radius,height);
}

void ofBaseRenderer::drawCylinder(const ofPoint& position, float radius, float height) const{
	get3dGraphics().drawCylinder(position,radius,height);
}

void ofBaseRenderer::drawCylinder(float radius, float height) const{
	get3dGraphics().drawCylinder(radius,height);
}

void ofBaseRenderer::setConeResolution( int radiusSegments, int heightSegments, int capSegments){
	get3dGraphics().setConeResolution(radiusSegments,heightSegments,capSegments);
}

ofVec3f ofBaseRenderer::getConeResolution() const{
	return get3dGraphics().getConeResolution();
}

void ofBaseRenderer::drawCone(float x, float y, float z, float radius, float height) const{
	get3dGraphics().drawCone(x,y,z,radius,height);
}

void ofBaseRenderer::drawCone(float x, float y, float radius, float height) const{
	get3dGraphics().drawCone(x,y,radius,height);
}

void ofBaseRenderer::drawCone(const ofPoint& position, float radius, float height) const{
	get3dGraphics().drawCone(position,radius,height);
}

void ofBaseRenderer::drawCone(float radius, float height) const{
	get3dGraphics().drawCone(radius,height);
}

void ofBaseRenderer::setBoxResolution( int res ){
	get3dGraphics().setBoxResolution(res);
}

void ofBaseRenderer::setBoxResolution( int resWidth, int resHeight, int resDepth ){
	get3dGraphics().setBoxResolution(resWidth,resHeight,resDepth);
}

ofVec3f ofBaseRenderer::getBoxResolution() const{
	return get3dGraphics().getBoxResolution();
}

void ofBaseRenderer::drawBox( float x, float y, float z, float width, float height, float depth) const{
	get3dGraphics().drawBox(x,y,z,width,height,depth);
}

void ofBaseRenderer::drawBox(float x, float y, float z, float size) const{
	get3dGraphics().drawBox(x,y,z,size);
}

void ofBaseRenderer::drawBox(const ofPoint& position, float width, float height, float depth) const{
	get3dGraphics().drawBox(position,width,height,depth);
}

void ofBaseRenderer::drawBox(const ofPoint& position, float size) const{
	get3dGraphics().drawBox(position,size);
}

void ofBaseRenderer::drawBox(float size) const{
	get3dGraphics().drawBox(size);
}

void ofBaseRenderer::drawBox( float width, float height, float depth ) const{
	get3dGraphics().drawBox(width,height,depth);
}

void ofBaseRenderer::drawAxis(float size) const{
	get3dGraphics().drawAxis(size);
}

void ofBaseRenderer::drawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z) const{
	get3dGraphics().drawGrid(stepSize,numberOfSteps,labels,x,y,z);
}

void ofBaseRenderer::drawGridPlane(float stepSize, size_t numberOfSteps, bool labels) const{
	get3dGraphics().drawGridPlane(stepSize,numberOfSteps,labels);
}

void ofBaseRenderer::drawArrow(const ofVec3f& start, const ofVec3f& end, float headSize) const{
	get3dGraphics().drawArrow(start,end,headSize);
}

void ofBaseRenderer::drawRotationAxes(float radius, float stripWidth, int circleRes) const{
	get3dGraphics().drawRotationAxes(radius,stripWidth,circleRes);
}

void ofBaseMaterial::uploadMatrices(const ofShader & shader,ofGLProgrammableRenderer & renderer) const{
	const ofMatrix4x4 & normalMatrix = renderer.getCurrentNormalMatrix();
	shader.setUniformMatrix4f("normalMatrix",normalMatrix);
}
