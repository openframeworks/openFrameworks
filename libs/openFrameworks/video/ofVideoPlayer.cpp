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
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPlayer(ofPtr<ofBaseVideoPlayer> newPlayer){
	player = newPlayer;
	setPixelFormat(internalPixelFormat);	//this means that it will try to set the pixel format you have been using before. 
											//if the format is not supported ofVideoPlayer's internalPixelFormat will be updated to that of the player's
}

//---------------------------------------------------------------------------
ofPtr<ofBaseVideoPlayer> ofVideoPlayer::getPlayer(){
	return player;
}

//we only set pixel format on the player if it exists. 
//if the movie is already loaded then we can't update the format.
//also if the format is not supported we get the format from the player instead.
//--------------------------------------------------------------------
bool ofVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat) {
	if( player != NULL ){
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
	if( player != NULL ){
		internalPixelFormat = player->getPixelFormat();
	}
	return internalPixelFormat;
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::loadMovie(string name){
	//#ifndef TARGET_ANDROID
		if( player == NULL ){
			setPlayer( ofPtr<OF_VID_PLAYER_TYPE>(new OF_VID_PLAYER_TYPE) );
			player->setPixelFormat(internalPixelFormat);
		}
	//#endif
	
	bool bOk = player->loadMovie(name);
	width	 = player->getWidth();
	height	 = player->getHeight();

	if( bOk){
        moviePath = name;
        if(bUseTexture ){
            if(width!=0 && height!=0) {
                tex.allocate(width, height, ofGetGLInternalFormatFromPixelFormat(internalPixelFormat));
        		if(ofGetGLProgrammableRenderer() && internalPixelFormat == OF_PIXELS_MONO){
        			tex.setRGToRGBASwizzles(true);
        		}
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
	if( player != NULL ){
		return player->getPixels();
	}
	return NULL;	
}

//---------------------------------------------------------------------------
ofPixelsRef ofVideoPlayer::getPixelsRef(){
	return player->getPixelsRef();
}

//
//---------------------------------------------------------------------------
//ofPixels ofVideoPlayer::getOFPixels(){
//	if( player != NULL ){
//		return player->getOFPixels();
//	}
//	return ofPixels();
//}
//
//---------------------------------------------------------------------------
//ofPixels ofVideoPlayer::getOFPixels() const{
//	if( player != NULL ){
//		return player->getOFPixels();
//	}
//	return ofPixels();
//}

//---------------------------------------------------------------------------
//for getting a reference to the texture
ofTexture & ofVideoPlayer::getTextureReference(){
	if(playerTex == NULL){
		return tex;
	}
	else{
		return *playerTex;
	}
}


//---------------------------------------------------------------------------
bool ofVideoPlayer::isFrameNew(){
	if( player != NULL ){
		return player->isFrameNew();
	}
	return false;
}

//--------------------------------------------------------------------
void ofVideoPlayer::update(){
	if(	player != NULL ){

		player->update();
		
		if( bUseTexture && player->isFrameNew() ) {
			
			playerTex = player->getTexture();
			
			if(playerTex == NULL){
				unsigned char *pxls = player->getPixels();
				
				bool bDiffPixFormat = ( tex.bAllocated() && tex.texData.glTypeInternal != ofGetGLInternalFormatFromPixelFormat(internalPixelFormat) );
				
				//TODO: we might be able to do something smarter here for not re-allocating movies of the same size and type. 
				if(width==0 || height==0 || bDiffPixFormat ){ //added a check if the pixel format and the texture don't match
					if(player->getWidth() != 0 && player->getHeight() != 0) {
						
						width = player->getWidth();
						height = player->getHeight();
					
						if(tex.bAllocated())
							tex.clear();

						tex.allocate(width, height, ofGetGLInternalFormatFromPixelFormat(internalPixelFormat));
		        		if(ofGetGLProgrammableRenderer() && internalPixelFormat == OF_PIXELS_MONO){
		        			tex.setRGToRGBASwizzles(true);
		        		}
						tex.loadData(pxls, tex.getWidth(), tex.getHeight(), ofGetGLTypeFromPixelFormat(internalPixelFormat));
					}
				}else{					
					tex.loadData(pxls, width, height, ofGetGLTypeFromPixelFormat(internalPixelFormat));
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
	if( player != NULL ){
		player->close();
	}
	tex.clear();
}

//--------------------------------------------------------
void ofVideoPlayer::play(){
	if( player != NULL ){
		player->play();
	}
}

//--------------------------------------------------------
void ofVideoPlayer::stop(){
	if( player != NULL ){
		player->stop();
	}
}

//--------------------------------------------------------
void ofVideoPlayer::setVolume(float volume){
	if( player != NULL ){
		if ( volume > 1.0f ){
			ofLogWarning("ofVideoPlayer") << "setVolume(): expected range is 0-1, limiting requested volume " << volume << " to 1.0";
			volume = 1.0f;
		}
		player->setVolume(volume);
	}
}


//--------------------------------------------------------
void ofVideoPlayer::setLoopState(ofLoopType state){
	if( player != NULL ){
		player->setLoopState(state);
	}
}

ofLoopType ofVideoPlayer::getLoopState(){
	if( player != NULL ){
		return player->getLoopState();
	}else{
		return OF_LOOP_NONE;
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPosition(float pct){
	if( player != NULL ){
		player->setPosition(pct);
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setFrame(int frame){
	if( player != NULL ){
		player->setFrame(frame);
	}
}


//---------------------------------------------------------------------------
float ofVideoPlayer::getDuration(){
	if( player != NULL ){
		return player->getDuration();
	}
	
	return 0.0;
}

//---------------------------------------------------------------------------
float ofVideoPlayer::getPosition(){
	if( player != NULL ){
		return player->getPosition();
	}
	return 0.0;
}

//---------------------------------------------------------------------------
int ofVideoPlayer::getCurrentFrame(){
	if( player != NULL ){
		return player->getCurrentFrame();
	}
	return 0;
}


//---------------------------------------------------------------------------
bool ofVideoPlayer::getIsMovieDone(){
	if( player != NULL ){
		return player->getIsMovieDone();
	}
	return false;
}

//---------------------------------------------------------------------------
void ofVideoPlayer::firstFrame(){
	if( player != NULL ){
		player->firstFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::nextFrame(){
	if( player != NULL ){
		player->nextFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::previousFrame(){
	if( player != NULL ){
		player->previousFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setSpeed(float _speed){
	if( player != NULL ){
		player->setSpeed(_speed);
	}
}

//---------------------------------------------------------------------------
float ofVideoPlayer::getSpeed(){
	if( player != NULL ){
		return player->getSpeed();
	}
	return 0.0;
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPaused(bool _bPause){
	if( player != NULL ){
		player->setPaused(_bPause);
	}
}

//------------------------------------
void ofVideoPlayer::setUseTexture(bool bUse){
	bUseTexture = bUse;
	if(bUse && width!=0 && height!=0 && !tex.isAllocated()){
		tex.allocate(width, height, ofGetGLTypeFromPixelFormat(internalPixelFormat));
		if(ofGetGLProgrammableRenderer() && internalPixelFormat == OF_PIXELS_MONO){
			tex.setRGToRGBASwizzles(true);
		}
	}
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
	getTextureReference().draw(_x, _y, _w, _h);	
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y){
	draw(_x, _y, width, height);
}

//------------------------------------
int ofVideoPlayer::getTotalNumFrames(){
	if( player != NULL ){
		return player->getTotalNumFrames();
	}
	return 0;
}

//----------------------------------------------------------
float ofVideoPlayer::getWidth(){
	if(	player != NULL ){
		width = player->getWidth();
	}
	return (float)width;
}

//----------------------------------------------------------
float ofVideoPlayer::getHeight(){
	if(	player != NULL ){
		height = player->getHeight();
	}
	return (float)height;
}

//----------------------------------------------------------
bool ofVideoPlayer::isPaused(){
	if(	player != NULL ){
		return player->isPaused();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isLoaded(){
	if(	player != NULL ){
		return player->isLoaded();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isPlaying(){
	if(	player != NULL ){
		return player->isPlaying();
	}
	return false;
}
