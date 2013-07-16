/*
 * ofxAndroidVideoPlayer.cpp
 *
 *  Created on: 25/04/2013
 *      Author: p
 */

#include "ofxAndroidVideoPlayer.h"
#include "ofxAndroidUtils.h"
#include "ofLog.h"
#include <set>

//---------------------------------------------------------------------------
static set<ofxAndroidVideoPlayer*> & all_videoplayers(){
	static set<ofxAndroidVideoPlayer*> *all_videoplayers = new set<ofxAndroidVideoPlayer*>;
	return *all_videoplayers;
}

void ofPauseVideoPlayers(){
	ofLog(OF_LOG_NOTICE,"ofVideoPlayer: releasing textures");
	set<ofxAndroidVideoPlayer*>::iterator it;
	for(it=all_videoplayers().begin();it!=all_videoplayers().end();it++){
		(*it)->unloadTexture();
	}
}

void ofResumeVideoPlayers(){
	ofLog(OF_LOG_NOTICE,"ofVideoPlayer: trying to allocate textures");
	set<ofxAndroidVideoPlayer*>::iterator it;
	for(it=all_videoplayers().begin();it!=all_videoplayers().end();it++){
		(*it)->reloadTexture();
	}
	ofLog(OF_LOG_NOTICE,"ofVideoPlayer: textures allocated");
}

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::reloadTexture(){
	if(!texture.texData.bAllocated) return;

	glGenTextures(1, (GLuint *)&texture.texData.textureID);

	glEnable(texture.texData.textureTarget);

	glBindTexture(texture.texData.textureTarget, (GLuint)texture.texData.textureID);

	glTexParameterf(texture.texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(texture.texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(texture.texData.textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(texture.texData.textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glDisable(texture.texData.textureTarget);

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}
	jmethodID javasetTexture = env->GetMethodID(javaClass,"setTexture","(I)V");
	if(!javasetTexture){
		ofLog(OF_LOG_ERROR,"Failed to get the java setTexture for VideoPlayer");
		return;
	}
	env->CallVoidMethod(javaVideoPlayer,javasetTexture,texture.getTextureData().textureID);
}

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::unloadTexture(){
	texture.texData.textureID=0;
}


//---------------------------------------------------------------------------
ofxAndroidVideoPlayer::ofxAndroidVideoPlayer(){

	all_videoplayers().insert(this);

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}
	jclass localClass = env->FindClass("cc/openframeworks/OFAndroidVideoPlayer");
	javaClass = (jclass) env->NewGlobalRef(localClass);

	if(!javaClass){
		ofLog(OF_LOG_ERROR,"Failed to get the java class for VideoPlayer");
		return;
	}

	jmethodID constructor = env->GetMethodID(javaClass,"<init>","()V");
	if(!constructor){
		ofLog(OF_LOG_ERROR,"Failed to get the java constructor for VideoPlayer");
		return;
	}

	javaVideoPlayer = env->NewObject(javaClass,constructor);
	if(!javaVideoPlayer){
		ofLog(OF_LOG_ERROR,"Failed to create java VideoPlayer");
		return;
	}

	javaVideoPlayer = (jobject)env->NewGlobalRef(javaVideoPlayer);

	bIsFrameNew = false;

	jfloatArray localMatrixJava = env->NewFloatArray(16);
	matrixJava = (jfloatArray) env->NewGlobalRef(localMatrixJava);

}

//---------------------------------------------------------------------------
ofxAndroidVideoPlayer::~ofxAndroidVideoPlayer(){

	all_videoplayers().erase(this);

	JNIEnv *env = ofGetJNIEnv();
	if(javaVideoPlayer) env->DeleteGlobalRef(javaVideoPlayer);
	if(javaClass) env->DeleteGlobalRef(javaClass);
	if(matrixJava) env->DeleteGlobalRef(matrixJava);
}

//---------------------------------------------------------------------------
bool ofxAndroidVideoPlayer::loadMovie(string fileName){

	if(!javaVideoPlayer){
		ofLogError() << "cannot load sound, java soundVideoPlayer object not created";
		return false;
	}

	JNIEnv *env = ofGetJNIEnv();
	jmethodID javaLoadMethod = env->GetMethodID(javaClass,"loadMovie","(Ljava/lang/String;)V");
	if(!javaLoadMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java loadVideo for VideoPlayer");
		return false;
	}

	jstring javaFileName = ofGetJNIEnv()->NewStringUTF(ofToDataPath(fileName,true).c_str());
	env->CallVoidMethod(javaVideoPlayer,javaLoadMethod,javaFileName);
	env->DeleteLocalRef((jobject)javaFileName);

	jmethodID javaGetWidthMethod = env->GetMethodID(javaClass,"getWidth","()I");
	if(!javaGetWidthMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java getWidth for VideoPlayer");
		return false;
	}
	width = env->CallIntMethod(javaVideoPlayer, javaGetWidthMethod);

	jmethodID javaGetHeightMethod = env->GetMethodID(javaClass,"getHeight","()I");
	if(!javaGetHeightMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java getHeight for VideoPlayer");
		return false;
	}
	height = env->CallIntMethod(javaVideoPlayer, javaGetHeightMethod);

	ofTextureData td;
	td.width = width;
	td.height = height;
	td.tex_w = td.width;
	td.tex_h = td.height;
	td.tex_t = 1; // Hack!
	td.tex_u = 1;
	td.textureTarget = GL_TEXTURE_EXTERNAL_OES;
	td.glTypeInternal = GL_RGBA;
	td.bFlipTexture = false;
	td.useTextureMatrix = true;

	// hack to initialize gl resources from outside ofTexture
	texture.texData = td;
	texture.texData.bAllocated = true;
	reloadTexture();

	ofLog() << "ofxAndroidVideoPlayer :: Movie size: "<< width <<"," << height;

	return true;

}

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::close(){

}

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::update(){

	JNIEnv *env = ofGetJNIEnv();
	jmethodID javaUpdate = env->GetMethodID(javaClass,"update","()Z");
	if(!javaUpdate){
		ofLog(OF_LOG_ERROR,"Failed to get the java update for VideoPlayer");
		return;
	}
	bIsFrameNew = env->CallBooleanMethod(javaVideoPlayer,javaUpdate);

	jmethodID javaGetTextureMatrix = env->GetMethodID(javaClass,"getTextureMatrix","([F)V");
	if(!javaGetTextureMatrix){
		ofLog(OF_LOG_ERROR,"Failed to get the java javaGetTextureMatrix for VideoPlayer");
		return;
	}
	env->CallVoidMethod(javaVideoPlayer,javaGetTextureMatrix,matrixJava);
	jfloat * m = env->GetFloatArrayElements(matrixJava,0);

	for(int i=0;i<16;i++) {
		texture.texData.textureMatrix.getPtr()[i] = m[i];
	}

	ofMatrix4x4 vFlipTextureMatrix;
	vFlipTextureMatrix.scale(1,-1,1);
	vFlipTextureMatrix.translate(0,1,0);
	texture.texData.textureMatrix = vFlipTextureMatrix * texture.texData.textureMatrix;
	//texture.getTextureData().tex_t = 1.+1-matrix.getPtr()[0]; // Hack!
	//texture.getTextureData().tex_u = 1.;

	env->ReleaseFloatArrayElements(matrixJava,m,0);

}

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::play(){

	JNIEnv *env = ofGetJNIEnv();
	jmethodID javaPlay = env->GetMethodID(javaClass,"play","()V");
	if(!javaPlay){
		ofLog(OF_LOG_ERROR,"Failed to get the java play for VideoPlayer");
		return;
	}
	env->CallVoidMethod(javaVideoPlayer,javaPlay);

};

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::stop(){

	JNIEnv *env = ofGetJNIEnv();
	jmethodID javaStop = env->GetMethodID(javaClass,"stop","()V");
	if(!javaStop){
		ofLog(OF_LOG_ERROR,"Failed to get the java stop for VideoPlayer");
		return;
	}
	env->CallVoidMethod(javaVideoPlayer,javaStop);

};


//---------------------------------------------------------------------------
bool ofxAndroidVideoPlayer::isPaused(){

	if(!javaVideoPlayer){
		ofLogError() << "cannot get paused state on an unloaded video player";
		return 0;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaIsPausedMethod = env->GetMethodID(javaClass,"isPaused","()Z");
	if(!javaIsPausedMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java isPaused for VideoPlayer");
		return 0;
	}

	return env->CallBooleanMethod(javaVideoPlayer,javaIsPausedMethod);

};


//---------------------------------------------------------------------------
bool ofxAndroidVideoPlayer::isLoaded(){

	if(!javaVideoPlayer){
		ofLogError() << "cannot get loaded state on an unloaded video player";
		return 0;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaIsLoadedMethod = env->GetMethodID(javaClass,"isLoaded","()Z");
	if(!javaIsLoadedMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java isLoaded for VideoPlayer");
		return 0;
	}

	return env->CallBooleanMethod(javaVideoPlayer,javaIsLoadedMethod);

};

//---------------------------------------------------------------------------
bool ofxAndroidVideoPlayer::isPlaying(){

	if(!javaVideoPlayer){
		ofLogError() << "cannot get playing state on an unloaded video player";
		return 0;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaIsPlayingMethod = env->GetMethodID(javaClass,"isPlaying","()Z");
	if(!javaIsPlayingMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java isPlaying for VideoPlayer");
		return 0;
	}

	return env->CallBooleanMethod(javaVideoPlayer,javaIsPlayingMethod);

};

//---------------------------------------------------------------------------
ofTexture * ofxAndroidVideoPlayer::getTexture(){
	return & texture;
}

//---------------------------------------------------------------------------
float ofxAndroidVideoPlayer::getWidth(){
	return width;
};

//---------------------------------------------------------------------------
float ofxAndroidVideoPlayer::getHeight(){
	return height;
};

//---------------------------------------------------------------------------
float ofxAndroidVideoPlayer::getPosition(){

	if(!javaVideoPlayer){
		ofLogError() << "cannot get position on an unloaded video player";
		return 0.;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaGetPositionMethod = env->GetMethodID(javaClass,"getPosition","()F");
	if(!javaGetPositionMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java getPosition for VideoPlayer");
		return 0;
	}

	return env->CallFloatMethod(javaVideoPlayer,javaGetPositionMethod);

};

//---------------------------------------------------------------------------
float ofxAndroidVideoPlayer::getDuration(){

	if(!javaVideoPlayer){
		ofLogError() << "cannot get duration on an unloaded video player";
		return 0.;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaGetDurationMethod = env->GetMethodID(javaClass,"getDuration","()F");
	if(!javaGetDurationMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java getDuration for VideoPlayer");
		return 0;
	}

	return env->CallFloatMethod(javaVideoPlayer,javaGetDurationMethod);

};

//---------------------------------------------------------------------------
bool ofxAndroidVideoPlayer::getIsMovieDone(){

	if(!javaVideoPlayer){
		ofLogError() << "cannot get movieDone state on an unloaded video player";
		return 0;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaIsMovieDoneMethod = env->GetMethodID(javaClass,"isMovieDone","()Z");
	if(!javaIsMovieDoneMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java isMovieDone for VideoPlayer");
		return 0;
	}

	return env->CallBooleanMethod(javaVideoPlayer,javaIsMovieDoneMethod);

};

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::setPosition(float pct){

	if(!javaVideoPlayer){
		ofLogError() << "cannot set position on an unloaded video player";
		return;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaSetPositionMethod = env->GetMethodID(javaClass,"setPosition","(F)V");
	if(!javaSetPositionMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java setPosition for VideoPlayer");
		return;
	}

	env->CallVoidMethod(javaVideoPlayer,javaSetPositionMethod,pct);

};

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::setPaused(bool bPause){

	if(!javaVideoPlayer){
		ofLogError() << "cannot set pause on an unloaded video player";
		return;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaPausedMethod = env->GetMethodID(javaClass,"setPaused","(Z)V");
	if(!javaPausedMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java setPaused for VideoPlayer");
		return;
	}

	env->CallVoidMethod(javaVideoPlayer,javaPausedMethod,bPause?1:0);

};

//------------------------------------------------------------
void ofxAndroidVideoPlayer::setVolume(float vol){

	if(!javaVideoPlayer){
		ofLogError() << "cannot set volume on an unloaded video player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaVolumeMethod = env->GetMethodID(javaClass,"setVolume","(F)V");
	if(!javaVolumeMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java setVolume for VideoPlayer");
		return;
	}

	env->CallVoidMethod(javaVideoPlayer,javaVolumeMethod,vol);

};

//------------------------------------------------------------
void ofxAndroidVideoPlayer::setLoopState(ofLoopType state){

	if(!javaVideoPlayer){
		ofLogError() << "cannot set loop state on an unloaded video player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaLoopStateMethod = env->GetMethodID(javaClass,"setLoopState","(Z)V");
	if(!javaLoopStateMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java LoopState for VideoPlayer");
		return;
	}

	switch (state) {
		case OF_LOOP_NORMAL:
			env->CallVoidMethod(javaVideoPlayer,javaLoopStateMethod,1);
			break;
		case OF_LOOP_PALINDROME:
			// TODO Palindrome loop not implemented
			ofLog(OF_LOG_ERROR,"OF_LOOP_PALINDROME not implemented");
			break;
		case OF_LOOP_NONE:
			default:
			env->CallVoidMethod(javaVideoPlayer,javaLoopStateMethod,0);
			break;
	}

};

//------------------------------------------------------------
ofLoopType ofxAndroidVideoPlayer::getLoopState(){

	if(!javaVideoPlayer){
		ofLogError() << "cannot get loop state on an unloaded video player";
		return OF_LOOP_NONE;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return OF_LOOP_NONE;
	}

	jmethodID javaGetLoopStateMethod = env->GetMethodID(javaClass,"getLoopState","()Z");
	if(!javaGetLoopStateMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java GetLoopState for VideoPlayer");
		return OF_LOOP_NONE;
	}

	bool loopState = env->CallBooleanMethod(javaVideoPlayer,javaGetLoopStateMethod);

	if (loopState) {
		return OF_LOOP_NORMAL;
	} else {
		return OF_LOOP_NONE;
	}

};


//------------------------------------------------------------
/* Needs movie frameRate to work
int ofxAndroidVideoPlayer::getCurrentFrame() {

	if(!javaVideoPlayer){
		ofLogError() << "cannot get duration on an unloaded video player";
		return 0.;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaGetCurrentFrame = env->GetMethodID(javaClass,"getCurrentFrame","()I");
	if(!javaGetCurrentFrame){
		ofLog(OF_LOG_ERROR,"Failed to get the java getCurrentFrame for VideoPlayer");
		return 0;
	}

	return env->CallIntMethod(javaVideoPlayer,javaGetCurrentFrame);

};
*/


//------------------------------------------------------------
/* Needs movie frameRate to work
int ofxAndroidVideoPlayer::getTotalNumFrames() {

	if(!javaVideoPlayer){
		ofLogError() << "cannot get duration on an unloaded video player";
		return 0.;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaGetTotalNumFrames = env->GetMethodID(javaClass,"getTotalNumFrames","()I");
	if(!javaGetTotalNumFrames){
		ofLog(OF_LOG_ERROR,"Failed to get the java getTotalNumFrames for VideoPlayer");
		return 0;
	}

	return env->CallIntMethod(javaVideoPlayer,javaGetTotalNumFrames);

};
*/






