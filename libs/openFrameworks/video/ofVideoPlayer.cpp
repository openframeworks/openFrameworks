#include "ofVideoPlayer.h"
#include "ofUtils.h"
#include "ofGraphics.h"



//---------------------------------------------------------------------------
ofVideoPlayer::ofVideoPlayer (){
	bUseTexture			= true;
	playerTex			= NULL;
	internalPixelFormat = OF_PIXELS_RGB;
	height 				= 0;
	width 				= 0;
	tex.resize(1);
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPlayer(shared_ptr<ofBaseVideoPlayer> newPlayer){
	player = newPlayer;
	setPixelFormat(internalPixelFormat);	//this means that it will try to set the pixel format you have been using before. 
											//if the format is not supported ofVideoPlayer's internalPixelFormat will be updated to that of the player's
}

//---------------------------------------------------------------------------
shared_ptr<ofBaseVideoPlayer> ofVideoPlayer::getPlayer(){
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
ofPixelFormat ofVideoPlayer::getPixelFormat(){
	if( player ){
		internalPixelFormat = player->getPixelFormat();
	}
	return internalPixelFormat;
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::loadMovie(string name){
	if( !player ){
		setPlayer( shared_ptr<OF_VID_PLAYER_TYPE>(new OF_VID_PLAYER_TYPE) );
		player->setPixelFormat(internalPixelFormat);
	}
	
	bool bOk = player->loadMovie(name);
	width	 = player->getWidth();
	height	 = player->getHeight();

	if( bOk){
        moviePath = name;
        if(bUseTexture){
        	if(player->getTexture()==NULL){
				if(width!=0 && height!=0) {
					tex.resize(player->getPixelsRef().getNumPlanes());
					for(int i=0;i<player->getPixelsRef().getNumPlanes();i++){
						ofPixels plane = player->getPixelsRef().getPlane(i);
						tex[i].allocate(plane);
						if(ofGetGLProgrammableRenderer() && plane.getPixelFormat() == OF_PIXELS_GRAY){
							tex[i].setRGToRGBASwizzles(true);
						}
					}
				}
        	}else{
        		playerTex = player->getTexture();
        	}
        }
    }
	
	return bOk;
}

//---------------------------------------------------------------------------
string ofVideoPlayer::getMoviePath(){
    return moviePath;	
}

//---------------------------------------------------------------------------
unsigned char * ofVideoPlayer::getPixels(){
	if( player ){
		return player->getPixels();
	}
	return NULL;	
}

//---------------------------------------------------------------------------
ofPixelsRef ofVideoPlayer::getPixelsRef(){
	return player->getPixelsRef();
}

//---------------------------------------------------------------------------
//for getting a reference to the texture
ofTexture & ofVideoPlayer::getTextureReference(){
	if(playerTex == NULL){
		return tex[0];
	}
	else{
		return *playerTex;
	}
}

vector<ofTexture> & ofVideoPlayer::getTexturePlanes(){
	if(playerTex != NULL){
		tex.clear();
		tex.push_back(*playerTex);
	}
	return tex;
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::isFrameNew(){
	if( player ){
		return player->isFrameNew();
	}
	return false;
}

//--------------------------------------------------------------------
void ofVideoPlayer::update(){
	if( player ){

		player->update();
		width = player->getWidth();
		height = player->getHeight();
		
		if( bUseTexture && player->isFrameNew() ) {
			
			playerTex = player->getTexture();
			
			if(playerTex == NULL){
				if(int(tex.size())!=player->getPixelsRef().getNumPlanes()){
					tex.resize(player->getPixelsRef().getNumPlanes());
				}
				if(player->getWidth() != 0 && player->getHeight() != 0) {
					for(int i=0;i<player->getPixelsRef().getNumPlanes();i++){
						ofPixels plane = player->getPixelsRef().getPlane(i);
						bool bDiffPixFormat = ( tex[i].isAllocated() && tex[i].texData.glTypeInternal != ofGetGLInternalFormatFromPixelFormat(plane.getPixelFormat()) );
						if(width==0 || height==0 || bDiffPixFormat || !tex[i].isAllocated() ){
							tex[i].allocate(plane);
							if(ofGetGLProgrammableRenderer() && plane.getPixelFormat() == OF_PIXELS_GRAY){
								tex[i].setRGToRGBASwizzles(true);
							}
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

ofLoopType ofVideoPlayer::getLoopState(){
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
float ofVideoPlayer::getDuration(){
	if( player ){
		return player->getDuration();
	}
	
	return 0.0;
}

//---------------------------------------------------------------------------
float ofVideoPlayer::getPosition(){
	if( player ){
		return player->getPosition();
	}
	return 0.0;
}

//---------------------------------------------------------------------------
int ofVideoPlayer::getCurrentFrame(){
	if( player ){
		return player->getCurrentFrame();
	}
	return 0;
}


//---------------------------------------------------------------------------
bool ofVideoPlayer::getIsMovieDone(){
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
float ofVideoPlayer::getSpeed(){
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
	if(bUse && player && !player->getTexture() && getWidth()!=0 && getHeight()!=0){
		for(int i=0;i<player->getPixelsRef().getNumPlanes();i++){
			ofPixels plane = player->getPixelsRef().getPlane(i);
			bool bDiffPixFormat = ( tex[i].bAllocated() && tex[i].texData.glTypeInternal != ofGetGLInternalFormatFromPixelFormat(plane.getPixelFormat()) );
			if(!tex[i].isAllocated() || bDiffPixFormat){
				tex[i].allocate(plane);
			}
			if(ofGetGLProgrammableRenderer() && plane.getPixelFormat() == OF_PIXELS_GRAY){
				tex[i].setRGToRGBASwizzles(true);
			}
		}
	}
}

//------------------------------------
bool ofVideoPlayer::isUsingTexture(){
	return bUseTexture;
}

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPercent(float xPct, float yPct){
	getTextureReference().setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPoint(float x, float y){
	getTextureReference().setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofVideoPlayer::resetAnchor(){
	getTextureReference().resetAnchor();
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y, float _w, float _h){
	ofGetCurrentRenderer()->draw(*this,_x,_y,_w,_h);
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y){
	draw(_x, _y, width, height);
}


//------------------------------------
void ofVideoPlayer::bind(){
	ofGetCurrentRenderer()->bind(*this);
}

//------------------------------------
void ofVideoPlayer::unbind(){
	ofGetCurrentRenderer()->unbind(*this);
}

//------------------------------------
int ofVideoPlayer::getTotalNumFrames(){
	if( player ){
		return player->getTotalNumFrames();
	}
	return 0;
}

//----------------------------------------------------------
float ofVideoPlayer::getWidth(){
	if( player ){
		width = player->getWidth();
	}
	return (float)width;
}

//----------------------------------------------------------
float ofVideoPlayer::getHeight(){
	if( player ){
		height = player->getHeight();
	}
	return (float)height;
}

//----------------------------------------------------------
bool ofVideoPlayer::isPaused(){
	if( player ){
		return player->isPaused();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isLoaded(){
	if( player ){
		return player->isLoaded();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isPlaying(){
	if( player ){
		return player->isPlaying();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isInitialized(){
	return player->isInitialized() && (!bUseTexture || tex[0].isAllocated());
}


