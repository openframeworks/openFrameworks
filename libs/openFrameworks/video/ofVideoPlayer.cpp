#include "ofVideoPlayer.h"
#include "ofUtils.h"

//---------------------------------------------------------------------------
ofVideoPlayer::ofVideoPlayer (){
	player						= NULL;
	bUseTexture					= true;
	playerTex					= NULL;
	internalPixelFormat = OF_PIXELS_RGB;
}

//---------------------------------------------------------------------------
ofVideoPlayer::~ofVideoPlayer(){
	close();
	
	if( player != NULL ){
		delete player;
		player = NULL;
	}
	
	tex.clear();
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::setPlayer(ofBaseVideoPlayer * newPlayer){
	if( player == NULL ){
		player = newPlayer;
		return true;
	}else{
		//TODO: should we delete newPlayer? This is why we need smart pointers. 
		ofLog(OF_LOG_ERROR, "ofVideoPlayer::setPlayer - player already set!");
	}
	
	return false;
}

//--------------------------------------------------------------------
void ofVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat) {
	internalPixelFormat = pixelFormat;
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::loadMovie(string name){
	if( player == NULL ){
		setPlayer( new OF_VID_PLAYER_TYPE );
		player->setPixelFormat(internalPixelFormat);
	}
	
	bool bOk = player->loadMovie(name);
	width	 = player->getWidth();
	height	 = player->getHeight();
	
	if( bOk && bUseTexture )
		if(width!=0 && height!=0)
			tex.allocate(width, height, GL_RGB);
	
	return bOk;
}

//---------------------------------------------------------------------------
unsigned char * ofVideoPlayer::getPixels(){
	if( player != NULL ){		
		return player->getPixels();
	}
	return NULL;	
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
	return tex;
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
			
			if(playerTex == NULL)
			{
				unsigned char *pxls = player->getPixels();
			
				if(width==0 || height==0) {
					if(player->getWidth() != 0 && player->getHeight() != 0) {
						
						width = player->getWidth();
						height = player->getHeight();
					
						if(tex.bAllocated())
							tex.clear();
					
						tex.allocate(width, height, GL_RGB);
						tex.loadData(pxls, tex.getWidth(), tex.getHeight(), GL_RGB);
					}
				}
				else
					tex.loadData(pxls, tex.getWidth(), tex.getHeight(), GL_RGB);
			}
		}
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::idleMovie(){
	update();
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
void ofVideoPlayer::setVolume(int volume){
	if( player != NULL ){
		player->setVolume(volume);
	}
}


//--------------------------------------------------------
void ofVideoPlayer::setLoopState(ofLoopType state){
	if( player != NULL ){
		player->setLoopState(state);
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
}

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPercent(float xPct, float yPct){
	tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPoint(int x, int y){
	tex.setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofVideoPlayer::resetAnchor(){
	tex.resetAnchor();
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y, float _w, float _h){
	if(playerTex == NULL)
		tex.draw(_x, _y, _w, _h);
	else
		playerTex->draw(_x, _y, _w, _h);
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y){
	if(playerTex == NULL)
		tex.draw(_x, _y);
	else
	{
		playerTex->draw(_x,_y);
	}
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
