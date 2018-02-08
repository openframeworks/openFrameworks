/*
 * ofxAndroidVideoPlayer.cpp
 *
 *  Created on: 25/04/2013
 *      Author: p
 */

#include "ofxAndroidVideoPlayer.h"
#include "ofxAndroidUtils.h"
#include "ofLog.h"
#include "ofMatrix4x4.h"

using namespace std;

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
		ofLogError("ofxAndroidVideoPlayer") << "reloadTexture(): couldn't get environment using GetEnv()";
		return;
	}
	jmethodID javasetTexture = env->GetMethodID(javaClass,"setTexture","(I)V");
	if(!javasetTexture){
		ofLogError("ofxAndroidVideoPlayer") << "reloadTexture(): couldn't get java setTexture for VideoPlayer";
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

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "constructor: couldn't get environment using GetEnv()";
		return;
	}
	jclass localClass = env->FindClass("cc/openframeworks/OFAndroidVideoPlayer");
	javaClass = (jclass) env->NewGlobalRef(localClass);

	if(!javaClass){
		ofLogError("ofxAndroidVideoPlayer") << "constructor: couldn't get java class for VideoPlayer";
		return;
	}

	jmethodID constructor = env->GetMethodID(javaClass,"<init>","()V");
	if(!constructor){
		ofLogError("ofxAndroidVideoPlayer") << "constructor: couldn't get java constructor for VideoPlayer";
		return;
	}

	javaVideoPlayer = env->NewObject(javaClass,constructor);
	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "constructor: couldn't create java VideoPlayer";
		return;
	}

	javaVideoPlayer = (jobject)env->NewGlobalRef(javaVideoPlayer);

	bIsFrameNew = false;

	jfloatArray localMatrixJava = env->NewFloatArray(16);
	matrixJava = (jfloatArray) env->NewGlobalRef(localMatrixJava);

	ofAddListener(ofxAndroidEvents().unloadGL,this,&ofxAndroidVideoPlayer::unloadTexture);
	ofAddListener(ofxAndroidEvents().reloadGL,this,&ofxAndroidVideoPlayer::reloadTexture);

}

//---------------------------------------------------------------------------
ofxAndroidVideoPlayer::~ofxAndroidVideoPlayer(){
	JNIEnv *env = ofGetJNIEnv();
	if(javaVideoPlayer) env->DeleteGlobalRef(javaVideoPlayer);
	if(javaClass) env->DeleteGlobalRef(javaClass);
	if(matrixJava) env->DeleteGlobalRef(matrixJava);
}


//---------------------------------------------------------------------------
bool ofxAndroidVideoPlayer::load(string fileName){

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "load(): java soundVideoPlayer not loaded";
		return false;
	}

	JNIEnv *env = ofGetJNIEnv();
	jmethodID javaLoadMethod = env->GetMethodID(javaClass,"loadMovie","(Ljava/lang/String;)V");
	if(!javaLoadMethod){
		ofLogError("ofxAndroidVideoPlayer") << "load(): couldn't get java loadVideo for VideoPlayer";
		return false;
	}

	jstring javaFileName = ofGetJNIEnv()->NewStringUTF(ofToDataPath(fileName,true).c_str());
	env->CallVoidMethod(javaVideoPlayer,javaLoadMethod,javaFileName);
	env->DeleteLocalRef((jobject)javaFileName);

	jmethodID javaGetWidthMethod = env->GetMethodID(javaClass,"getWidth","()I");
	if(!javaGetWidthMethod){
		ofLogError("ofxAndroidVideoPlayer") << "loadMovie(): couldn't get java getWidth for VideoPlayer";
		return false;
	}
	width = env->CallIntMethod(javaVideoPlayer, javaGetWidthMethod);

	jmethodID javaGetHeightMethod = env->GetMethodID(javaClass,"getHeight","()I");
	if(!javaGetHeightMethod){
		ofLogError("ofxAndroidVideoPlayer") << "loadMovie(): couldn't get java getHeight for VideoPlayer";
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
	td.glInternalFormat = GL_RGBA;
	td.bFlipTexture = false;

	// hack to initialize gl resources from outside ofTexture
	texture.texData = td;
	texture.texData.bAllocated = true;
	reloadTexture();

	ofLogVerbose("ofxAndroidVideoPlayer") << "loadMovie(): movie size: "<< width << "x" << height;

	return true;

}

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::close(){
	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "unloadMovie(): java VideoPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "unloadMovie(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaUnloadMethod = env->GetMethodID(javaClass,"unloadMovie","()V");
	if(!javaUnloadMethod){
		ofLogError("ofxAndroidVideoPlayer") << "unloadMovie(): couldn't get java unloadMovie for VideoPlayer";
		return;
	}

	unloadTexture();
	env->CallVoidMethod(javaVideoPlayer,javaUnloadMethod);
}

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::update(){

	JNIEnv *env = ofGetJNIEnv();
	jmethodID javaUpdate = env->GetMethodID(javaClass,"update","()Z");
	if(!javaUpdate){
		ofLogError("ofxAndroidVideoPlayer") << "update(): couldn't get java update for VideoPlayer";
		return;
	}
	bIsFrameNew = env->CallBooleanMethod(javaVideoPlayer,javaUpdate);

	jmethodID javaGetTextureMatrix = env->GetMethodID(javaClass,"getTextureMatrix","([F)V");
	if(!javaGetTextureMatrix){
		ofLogError("ofxAndroidVideoPlayer") << "update(): couldn't get java javaGetTextureMatrix for VideoPlayer";
		return;
	}
	env->CallVoidMethod(javaVideoPlayer,javaGetTextureMatrix,matrixJava);
	jfloat * m = env->GetFloatArrayElements(matrixJava,0);

	ofMatrix4x4 textureMatrix(m);

	ofMatrix4x4 vFlipTextureMatrix;
	vFlipTextureMatrix.scale(1,-1,1);
	vFlipTextureMatrix.translate(0,1,0);
	texture.setTextureMatrix(vFlipTextureMatrix * textureMatrix);
	//texture.getTextureData().tex_t = 1.+1-matrix.getPtr()[0]; // Hack!
	//texture.getTextureData().tex_u = 1.;

	env->ReleaseFloatArrayElements(matrixJava,m,0);

}

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::play(){

	JNIEnv *env = ofGetJNIEnv();
	jmethodID javaPlay = env->GetMethodID(javaClass,"play","()V");
	if(!javaPlay){
		ofLogError("ofxAndroidVideoPlayer") << "play(): couldn't get java play for VideoPlayer";
		return;
	}
	env->CallVoidMethod(javaVideoPlayer,javaPlay);

};

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::stop(){

	JNIEnv *env = ofGetJNIEnv();
	jmethodID javaStop = env->GetMethodID(javaClass,"stop","()V");
	if(!javaStop){
		ofLogError("ofxAndroidVideoPlayer") << "stop(): couldn't get java stop for VideoPlayer";
		return;
	}
	env->CallVoidMethod(javaVideoPlayer,javaStop);

};


//---------------------------------------------------------------------------
bool ofxAndroidVideoPlayer::isPaused() const {

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "isPaused(): java VideoPlayer not loaded";
		return 0;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "isPaused(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaIsPausedMethod = env->GetMethodID(javaClass,"isPaused","()Z");
	if(!javaIsPausedMethod){
		ofLogError("ofxAndroidVideoPlayer") << "isPaused(): couldn't get java isPaused for VideoPlayer";
		return 0;
	}

	return env->CallBooleanMethod(javaVideoPlayer,javaIsPausedMethod);

};


//---------------------------------------------------------------------------
bool ofxAndroidVideoPlayer::isLoaded() const {

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "isLoaded(): java VideoPlayer not loaded";
		return 0;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "isLoaded(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaIsLoadedMethod = env->GetMethodID(javaClass,"isLoaded","()Z");
	if(!javaIsLoadedMethod){
		ofLogError("ofxAndroidVideoPlayer") << "isLoaded(): couldn't get java isLoaded for VideoPlayer";
		return 0;
	}

	return env->CallBooleanMethod(javaVideoPlayer,javaIsLoadedMethod);

};

//---------------------------------------------------------------------------
bool ofxAndroidVideoPlayer::isPlaying() const {

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "isPlaying(): java VideoPlayer not loaded";
		return 0;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "isPlaying(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaIsPlayingMethod = env->GetMethodID(javaClass,"isPlaying","()Z");
	if(!javaIsPlayingMethod){
		ofLogError("ofxAndroidVideoPlayer") << "isPlaying(): couldn't get the java isPlaying for VideoPlayer";
		return 0;
	}

	return env->CallBooleanMethod(javaVideoPlayer,javaIsPlayingMethod);

};

//---------------------------------------------------------------------------
ofTexture * ofxAndroidVideoPlayer::getTexturePtr(){
	return & texture;
}

//---------------------------------------------------------------------------
float ofxAndroidVideoPlayer::getWidth() const {
	return width;
};

//---------------------------------------------------------------------------
float ofxAndroidVideoPlayer::getHeight() const {
	return height;
};

//---------------------------------------------------------------------------
float ofxAndroidVideoPlayer::getPosition() const {

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "getPosition(): java VideoPlayer not loaded";
		return 0.;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "getPosition(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaGetPositionMethod = env->GetMethodID(javaClass,"getPosition","()F");
	if(!javaGetPositionMethod){
		ofLogError("ofxAndroidVideoPlayer") << "getPosition(): couldn't get java getPosition for VideoPlayer";
		return 0;
	}

	return env->CallFloatMethod(javaVideoPlayer,javaGetPositionMethod);

};

//---------------------------------------------------------------------------
float ofxAndroidVideoPlayer::getDuration() const {

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "getDuration(): java VideoPlayer not loaded";
		return 0.;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "getDuration(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaGetDurationMethod = env->GetMethodID(javaClass,"getDuration","()F");
	if(!javaGetDurationMethod){
		ofLogError("ofxAndroidVideoPlayer") << "getDuration(): couldn't get java getDuration for VideoPlayer";
		return 0;
	}

	return env->CallFloatMethod(javaVideoPlayer,javaGetDurationMethod);

};

//---------------------------------------------------------------------------
bool ofxAndroidVideoPlayer::getIsMovieDone() const {

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "getIsMovieDone(): java VideoPlayer not loaded";
		return 0;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "getIsMovieDone(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaIsMovieDoneMethod = env->GetMethodID(javaClass,"isMovieDone","()Z");
	if(!javaIsMovieDoneMethod){
		ofLogError("ofxAndroidVideoPlayer") << "getIsMovieDone(): couldn't get java isMovieDone for VideoPlayer";
		return 0;
	}

	return env->CallBooleanMethod(javaVideoPlayer,javaIsMovieDoneMethod);

};

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::setPosition(float pct){

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "setPosition(): java VideoPlayer not loaded";
		return;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "setPosition(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaSetPositionMethod = env->GetMethodID(javaClass,"setPosition","(F)V");
	if(!javaSetPositionMethod){
		ofLogError("ofxAndroidVideoPlayer") << "setPosition(): couldn't get java setPosition for VideoPlayer";
		return;
	}

	env->CallVoidMethod(javaVideoPlayer,javaSetPositionMethod,pct);

};

//---------------------------------------------------------------------------
void ofxAndroidVideoPlayer::setPaused(bool bPause){

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "setPaused(): java VideoPlayer not loaded";
		return;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "setPaused(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaPausedMethod = env->GetMethodID(javaClass,"setPaused","(Z)V");
	if(!javaPausedMethod){
		ofLogError("ofxAndroidVideoPlayer") << "setPaused(): coulnd't get java setPaused for VideoPlayer";
		return;
	}

	env->CallVoidMethod(javaVideoPlayer,javaPausedMethod,bPause?1:0);

};

//------------------------------------------------------------
void ofxAndroidVideoPlayer::setVolume(float vol){

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "setVolume(): java VideoPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "setVolume(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaVolumeMethod = env->GetMethodID(javaClass,"setVolume","(F)V");
	if(!javaVolumeMethod){
		ofLogError("ofxAndroidVideoPlayer") << "setVolume(): couldn't get java setVolume for VideoPlayer";
		return;
	}

	env->CallVoidMethod(javaVideoPlayer,javaVolumeMethod,vol);

};

//------------------------------------------------------------
void ofxAndroidVideoPlayer::setLoopState(ofLoopType state){

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "setLoopState(): java VideoPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "setLoopState(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaLoopStateMethod = env->GetMethodID(javaClass,"setLoopState","(Z)V");
	if(!javaLoopStateMethod){
		ofLogError("ofxAndroidVideoPlayer") << "setLoopState(): couldn't get java LoopState for VideoPlayer";
		return;
	}

	switch (state) {
		case OF_LOOP_NORMAL:
			env->CallVoidMethod(javaVideoPlayer,javaLoopStateMethod,1);
			break;
		case OF_LOOP_PALINDROME:
			// TODO Palindrome loop not implemented
			ofLogError("ofxAndroidVideoPlayer") << "setLoopState(): OF_LOOP_PALINDROME not implemented";
			break;
		case OF_LOOP_NONE:
			default:
			env->CallVoidMethod(javaVideoPlayer,javaLoopStateMethod,0);
			break;
	}

};

//------------------------------------------------------------
ofLoopType ofxAndroidVideoPlayer::getLoopState() const {

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "getLoopState(): java VideoPlayer not loaded";
		return OF_LOOP_NONE;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "getLoopState(): couldn't get environment using GetEnv()";
		return OF_LOOP_NONE;
	}

	jmethodID javaGetLoopStateMethod = env->GetMethodID(javaClass,"getLoopState","()Z");
	if(!javaGetLoopStateMethod){
		ofLogError("ofxAndroidVideoPlayer") << "getLoopState(): couldn't get java GetLoopState for VideoPlayer";
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
		ofLogError("ofxAndroidVideoPlayer") << "getCurrentFrame(): java VideoPlayer not loaded";
		return 0.;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "getCurrentFrame(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaGetCurrentFrame = env->GetMethodID(javaClass,"getCurrentFrame","()I");
	if(!javaGetCurrentFrame){
		ofLogError("ofxAndroidVideoPlayer") << "getCurrentFrame(): couldn't get java getCurrentFrame for VideoPlayer";
		return 0;
	}

	return env->CallIntMethod(javaVideoPlayer,javaGetCurrentFrame);

};
*/


//------------------------------------------------------------
/* Needs movie frameRate to work
int ofxAndroidVideoPlayer::getTotalNumFrames() {

	if(!javaVideoPlayer){
		ofLogError("ofxAndroidVideoPlayer") << "getTotalNumFrames(): java VideoPlayer not loaded";
		return 0.;
	}

	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidVideoPlayer") << "getTotalNumFrames(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaGetTotalNumFrames = env->GetMethodID(javaClass,"getTotalNumFrames","()I");
	if(!javaGetTotalNumFrames){
		ofLogError("ofxAndroidVideoPlayer") << "getTotalNumFrames(): couldn't get java getTotalNumFrames for VideoPlayer";
		return 0;
	}

	return env->CallIntMethod(javaVideoPlayer,javaGetTotalNumFrames);

};
*/






