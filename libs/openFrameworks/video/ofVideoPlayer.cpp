#include "ofVideoPlayer.h"
#include "ofAppRunner.h"
#include "ofGLUtils.h"
#include "ofPixels.h"
#include <algorithm>

using std::shared_ptr;
using std::vector;
using std::string;


#ifdef OF_VIDEO_PLAYER_GSTREAMER
	#include "ofGstVideoPlayer.h"
	#define OF_VID_PLAYER_TYPE ofGstVideoPlayer
#endif

#ifdef OF_VIDEO_PLAYER_QUICKTIME
	#include "ofQuickTimePlayer.h"
	#define OF_VID_PLAYER_TYPE ofQuickTimePlayer
#endif

#ifdef OF_VIDEO_PLAYER_QTKIT
	#include "ofQTKitPlayer.h"
	#define OF_VID_PLAYER_TYPE ofQTKitPlayer
#endif

#ifdef OF_VIDEO_PLAYER_AVFOUNDATION
	#include "ofAVFoundationPlayer.h"
	#define OF_VID_PLAYER_TYPE ofAVFoundationPlayer
#endif

#ifdef OF_VIDEO_PLAYER_DIRECTSHOW
	#include "ofDirectShowPlayer.h"
	#define OF_VID_PLAYER_TYPE ofDirectShowPlayer
#endif

#if defined(OF_VIDEO_PLAYER_MEDIA_FOUNDATION)
	#include "ofMediaFoundationPlayer.h"
	#define OF_VID_PLAYER_TYPE ofMediaFoundationPlayer
#endif

#ifdef OF_VIDEO_PLAYER_IOS
	#include "ofxiOSVideoPlayer.h"
	#define OF_VID_PLAYER_TYPE ofxiOSVideoPlayer
#endif

#ifdef OF_VIDEO_PLAYER_ANDROID
	#include "ofxAndroidVideoPlayer.h"
	#define OF_VID_PLAYER_TYPE ofxAndroidVideoPlayer
#endif

#ifdef OF_VIDEO_PLAYER_EMSCRIPTEN
	#include "ofxEmscriptenVideoPlayer.h"
	#define OF_VID_PLAYER_TYPE ofxEmscriptenVideoPlayer
#endif


//---------------------------------------------------------------------------
ofVideoPlayer::ofVideoPlayer (){
	bUseTexture			= true;
	playerTex			= nullptr;
	internalPixelFormat = OF_PIXELS_RGB;
	tex.resize(1);
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPlayer(shared_ptr<ofBaseVideoPlayer> newPlayer){
	player = std::move(newPlayer);
	setPixelFormat(internalPixelFormat);	//this means that it will try to set the pixel format you have been using before. 
											//if the format is not supported ofVideoPlayer's internalPixelFormat will be updated to that of the player's
}

//---------------------------------------------------------------------------
shared_ptr<ofBaseVideoPlayer> ofVideoPlayer::getPlayer(){
	if( !player ){
		setPlayer(std::make_shared<OF_VID_PLAYER_TYPE>());
	}
	return player;
}

const shared_ptr<ofBaseVideoPlayer>	ofVideoPlayer::getPlayer() const{
	return player;
}

//we only set pixel format on the player if it exists. 
//if the movie is already loaded then we can't update the format.
//also if the format is not supported we get the format from the player instead.
//--------------------------------------------------------------------
bool ofVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat) {
	if( player ){
		if( player->isLoaded() ){
			ofLogWarning("ofVideoPlayer") << "setPixelFormat(): can't set pixel format of a loaded movie";
			internalPixelFormat = player->getPixelFormat(); 
			return false;
		}else{
			if( player->setPixelFormat(pixelFormat) ){		
				internalPixelFormat = player->getPixelFormat();  //we do this as either way we want the players format
			}else{
				internalPixelFormat = player->getPixelFormat();  //we do this as either way we want the players format
				return false; 					
			}
		}
	}else{
		internalPixelFormat = pixelFormat;	
	}
	return true;
}

//---------------------------------------------------------------------------
ofPixelFormat ofVideoPlayer::getPixelFormat() const{
	if( player ){
		internalPixelFormat = player->getPixelFormat();
	}
	return internalPixelFormat;
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::load(string name){
	if( !player ){
		setPlayer(std::make_shared<OF_VID_PLAYER_TYPE>());
		player->setPixelFormat(internalPixelFormat);
	}
	
	bool bOk = player->load(name);

	if( bOk){
        moviePath = name;
        if(bUseTexture){
        	if(player->getTexturePtr()==nullptr){
				if(tex.empty()) {
					tex.resize(std::max(player->getPixels().getNumPlanes(),static_cast<size_t>(1)));
					for(std::size_t i=0;i<player->getPixels().getNumPlanes();i++){
						ofPixels plane = player->getPixels().getPlane(i);
						tex[i].allocate(plane);
						if(ofIsGLProgrammableRenderer() && plane.getPixelFormat() == OF_PIXELS_GRAY){
							tex[i].setRGToRGBASwizzles(true);
						}
					}
				}
        	}else{
        		playerTex = player->getTexturePtr();
        	}
        }
		setLoopState(OF_LOOP_NORMAL);
    }
	
	return bOk;
}

//---------------------------------------------------------------------------
void ofVideoPlayer::loadAsync(string name){
	if( !player ){
		setPlayer(std::make_shared<OF_VID_PLAYER_TYPE>());
		player->setPixelFormat(internalPixelFormat);
	}
	
	player->loadAsync(name);
	moviePath = name;
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::loadMovie(string name){
	return load(name);
}

//---------------------------------------------------------------------------
string ofVideoPlayer::getMoviePath() const{
    return moviePath;	
}

//---------------------------------------------------------------------------
ofPixels & ofVideoPlayer::getPixels(){
	return getPlayer()->getPixels();
}

//---------------------------------------------------------------------------
const ofPixels&	ofVideoPlayer::getPixels() const{
	return getPlayer()->getPixels();
}

//---------------------------------------------------------------------------
ofPixels& ofVideoPlayer::getPixelsRef(){
	return getPlayer()->getPixels();
}

//---------------------------------------------------------------------------
const ofPixels& ofVideoPlayer::getPixelsRef() const{
	return getPlayer()->getPixels();
}

//---------------------------------------------------------------------------
ofTexture & ofVideoPlayer::getTexture(){
	if(playerTex == nullptr){
		return tex[0];
	}else{
		return *playerTex;
	}
}

//---------------------------------------------------------------------------
const ofTexture & ofVideoPlayer::getTexture() const{
	if(playerTex == nullptr){
		return tex[0];
	}else{
		return *playerTex;
	}
}

//---------------------------------------------------------------------------
ofTexture & ofVideoPlayer::getTextureReference(){
	return getTexture();
}

//---------------------------------------------------------------------------
const ofTexture & ofVideoPlayer::getTextureReference() const{
	return getTexture();
}

//---------------------------------------------------------------------------
vector<ofTexture> & ofVideoPlayer::getTexturePlanes(){
	if(playerTex != nullptr){
		tex.clear();
		tex.push_back(*playerTex);
	}
	return tex;
}

//---------------------------------------------------------------------------
const vector<ofTexture> & ofVideoPlayer::getTexturePlanes() const{
	if(playerTex != nullptr){
		ofVideoPlayer * mutThis = const_cast<ofVideoPlayer*>(this);
		mutThis->tex.clear();
		mutThis->tex.push_back(*playerTex);
	}
	return tex;
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::isFrameNew() const{
	if( player ){
		return player->isFrameNew();
	}
	return false;
}

//--------------------------------------------------------------------
void ofVideoPlayer::update(){
	if( player ){

		player->update();
		
		if( bUseTexture && player->isFrameNew() ) {
			
			playerTex = player->getTexturePtr();
			
			if(playerTex == nullptr){
				if(tex.size()!=player->getPixels().getNumPlanes()){
					tex.resize(std::max(player->getPixels().getNumPlanes(),static_cast<std::size_t>(1)));
				}
				if(std::size_t(player->getWidth()) != 0 && std::size_t(player->getHeight()) != 0) {
					for(std::size_t i=0;i<player->getPixels().getNumPlanes();i++){
						ofPixels plane = player->getPixels().getPlane(i);
						bool bDiffPixFormat = ( tex[i].isAllocated() && tex[i].texData.glInternalFormat != ofGetGLInternalFormatFromPixelFormat(plane.getPixelFormat()) );
						if(bDiffPixFormat || !tex[i].isAllocated() || std::size_t(tex[i].getWidth()) != plane.getWidth() || std::size_t(tex[i].getHeight()) != plane.getHeight())
						{
							tex[i].allocate(plane);
						}
						tex[i].loadData(plane);
					}
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::closeMovie(){
	close();
}

//---------------------------------------------------------------------------
void ofVideoPlayer::close(){
	if( player ){
		player->close();
	}
	tex.clear();
}

//--------------------------------------------------------
void ofVideoPlayer::play(){
	if( player ){
		player->play();
	}
}

//--------------------------------------------------------
void ofVideoPlayer::stop(){
	if( player ){
		player->stop();
	}
}

//--------------------------------------------------------
void ofVideoPlayer::setVolume(float volume){
	if( player ){
		if ( volume > 1.0f ){
			ofLogWarning("ofVideoPlayer") << "setVolume(): expected range is 0-1, limiting requested volume " << volume << " to 1.0";
			volume = 1.0f;
		}
		player->setVolume(volume);
	}
}


//--------------------------------------------------------
void ofVideoPlayer::setLoopState(ofLoopType state){
	if( player ){
		player->setLoopState(state);
	}
}

ofLoopType ofVideoPlayer::getLoopState() const{
	if( player ){
		return player->getLoopState();
	}else{
		return OF_LOOP_NONE;
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPosition(float pct){
	if( player ){
		player->setPosition(pct);
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setFrame(int frame){
	if( player ){
		player->setFrame(frame);
	}
}


//---------------------------------------------------------------------------
float ofVideoPlayer::getDuration() const{
	if( player ){
		return player->getDuration();
	}
	
	return 0.0;
}

//---------------------------------------------------------------------------
float ofVideoPlayer::getPosition() const{
	if( player ){
		return player->getPosition();
	}
	return 0.0;
}

//---------------------------------------------------------------------------
int ofVideoPlayer::getCurrentFrame() const{
	if( player ){
		return player->getCurrentFrame();
	}
	return 0;
}


//---------------------------------------------------------------------------
bool ofVideoPlayer::getIsMovieDone() const{
	if( player ){
		return player->getIsMovieDone();
	}
	return false;
}

//---------------------------------------------------------------------------
void ofVideoPlayer::firstFrame(){
	if( player ){
		player->firstFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::nextFrame(){
	if( player ){
		player->nextFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::previousFrame(){
	if( player ){
		player->previousFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setSpeed(float _speed){
	if( player ){
		player->setSpeed(_speed);
	}
}

//---------------------------------------------------------------------------
float ofVideoPlayer::getSpeed() const{
	if( player ){
		return player->getSpeed();
	}
	return 0.0;
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPaused(bool _bPause){
	if( player ){
		player->setPaused(_bPause);
	}
}

//------------------------------------
void ofVideoPlayer::setUseTexture(bool bUse){
	bUseTexture = bUse;
	if(bUse && player && !player->getTexturePtr() && std::size_t(getWidth()) != 0 && std::size_t(getHeight()) != 0){
		for(std::size_t i=0;i<player->getPixels().getNumPlanes();i++){
			ofPixels plane = player->getPixels().getPlane(i);
			bool bDiffPixFormat = ( tex[i].isAllocated() && tex[i].texData.glInternalFormat != ofGetGLInternalFormatFromPixelFormat(plane.getPixelFormat()) );
			if(!tex[i].isAllocated() || bDiffPixFormat){
				tex[i].allocate(plane);
			}
		}
	}
}

//------------------------------------
bool ofVideoPlayer::isUsingTexture() const{
	return bUseTexture;
}

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPercent(float xPct, float yPct){
	getTexture().setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPoint(float x, float y){
	getTexture().setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofVideoPlayer::resetAnchor(){
	getTexture().resetAnchor();
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y, float _w, float _h) const{
	ofGetCurrentRenderer()->draw(*this,_x,_y,_w,_h);
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y) const{
	draw(_x, _y, getWidth(), getHeight());
}


//------------------------------------
void ofVideoPlayer::bind() const{
	shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->bind(*this);
	}
}

//------------------------------------
void ofVideoPlayer::unbind() const{
	shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->unbind(*this);
	}
}

//------------------------------------
int ofVideoPlayer::getTotalNumFrames() const{
	if( player ){
		return player->getTotalNumFrames();
	}
	return 0;
}

//----------------------------------------------------------
float ofVideoPlayer::getWidth() const{
	if( player ){
		return player->getWidth();
	}else{
		return 0;
	}
}

//----------------------------------------------------------
float ofVideoPlayer::getHeight() const{
	if( player ){
		return player->getHeight();
	}else{
		return 0;
	}
}

//----------------------------------------------------------
bool ofVideoPlayer::isPaused() const{
	if( player ){
		return player->isPaused();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isLoaded() const{
	if( player ){
		return player->isLoaded();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isPlaying() const{
	if( player ){
		return player->isPlaying();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isInitialized() const{
	if( player ){
		return player->isInitialized() && (!bUseTexture || tex[0].isAllocated() || (player->getTexturePtr() && player->getTexturePtr()->isAllocated()) );
	}
	return false;
}


