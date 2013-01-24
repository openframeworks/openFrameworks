#include "ofxAndroidSoundPlayer.h"
#include "ofxAndroidUtils.h"
#include "ofLog.h"

//------------------------------------------------------------
ofxAndroidSoundPlayer::ofxAndroidSoundPlayer(){
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}
	jclass localClass = env->FindClass("cc/openframeworks/OFAndroidSoundPlayer");
	javaClass = (jclass) env->NewGlobalRef(localClass);

	if(!javaClass){
		ofLog(OF_LOG_ERROR,"Failed to get the java class for SoundPlayer");
		return;
	}


	jmethodID constructor = env->GetMethodID(javaClass,"<init>","()V");
	if(!constructor){
		ofLog(OF_LOG_ERROR,"Failed to get the java constructor for SoundPlayer");
		return;
	}

	javaSoundPlayer = env->NewObject(javaClass,constructor);
	if(!javaSoundPlayer){
		ofLog(OF_LOG_ERROR,"Failed to create java SoundPlayer");
		return;
	}

	javaSoundPlayer = (jobject)env->NewGlobalRef(javaSoundPlayer);
}

//------------------------------------------------------------
ofxAndroidSoundPlayer::~ofxAndroidSoundPlayer(){
	JNIEnv *env = ofGetJNIEnv();
	if(javaSoundPlayer) env->DeleteGlobalRef(javaSoundPlayer);
	if(javaClass) env->DeleteGlobalRef(javaClass);
}


//------------------------------------------------------------
bool ofxAndroidSoundPlayer::loadSound(string fileName, bool stream){
	if(!javaSoundPlayer){
		ofLogError() << "cannot load sound, java soundPlayer object not created";
		return false;
	}

	JNIEnv *env = ofGetJNIEnv();
	jmethodID javaLoadMethod = env->GetMethodID(javaClass,"loadSound","(Ljava/lang/String;Z)V");
	if(!javaLoadMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java loadSound for SoundPlayer");
		return false;
	}

	jstring javaFileName = ofGetJNIEnv()->NewStringUTF(ofToDataPath(fileName,true).c_str());
	env->CallVoidMethod(javaSoundPlayer,javaLoadMethod,javaFileName,stream?1:0);
	env->DeleteLocalRef((jobject)javaFileName);
	return true;
}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::unloadSound(){
	if(!javaSoundPlayer){
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaUnloadMethod = env->GetMethodID(javaClass,"unloadSound","()V");
	if(!javaUnloadMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java unloadSound for SoundPlayer");
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaUnloadMethod);
}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::play(){
	if(!javaSoundPlayer){
		ofLogError() << "cannot play on an unloaded sound player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaPlayMethod = env->GetMethodID(javaClass,"play","()V");
	if(!javaPlayMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java play for SoundPlayer");
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaPlayMethod);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::stop(){
	if(!javaSoundPlayer){
		ofLogError() << "cannot stop on an unloaded sound player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaStopMethod = env->GetMethodID(javaClass,"stop","()V");
	if(!javaStopMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java play for SoundPlayer");
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaStopMethod);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setVolume(float vol){
	if(!javaSoundPlayer){
		ofLogError() << "cannot set volume on an unloaded sound player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaVolumeMethod = env->GetMethodID(javaClass,"setVolume","(F)V");
	if(!javaVolumeMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java setVolume for SoundPlayer");
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaVolumeMethod,vol);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setPan(float vol){
	if(!javaSoundPlayer){
		ofLogError() << "cannot set pan on an unloaded sound player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaPanMethod = env->GetMethodID(javaClass,"setPan","(F)V");
	if(!javaPanMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java setPan for SoundPlayer");
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaPanMethod,vol);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setSpeed(float spd){
	if(!javaSoundPlayer){
		ofLogError() << "cannot set spped on an unloaded sound player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaSpeedMethod = env->GetMethodID(javaClass,"setSpeed","(F)V");
	if(!javaSpeedMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java setSpeed for SoundPlayer");
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaSpeedMethod,spd);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setPaused(bool bP){
	if(!javaSoundPlayer){
		ofLogError() << "cannot set pause on an unloaded sound player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaPausedMethod = env->GetMethodID(javaClass,"setPaused","(Z)V");
	if(!javaPausedMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java setPaused for SoundPlayer");
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaPausedMethod,bP?1:0);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setLoop(bool bLp){
	if(!javaSoundPlayer){
		ofLogError() << "cannot set loop on an unloaded sound player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaLoopMethod = env->GetMethodID(javaClass,"setLoop","(Z)V");
	if(!javaLoopMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java setLoop for SoundPlayer");
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaLoopMethod,bLp?1:0);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setMultiPlay(bool bMp){
	if(!javaSoundPlayer){
		ofLogError() << "cannot set multiplay on an unloaded sound player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaMultiplayMethod = env->GetMethodID(javaClass,"setMultiPlay","(Z)V");
	if(!javaMultiplayMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java setMultiPlay for SoundPlayer");
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaMultiplayMethod,bMp?1:0);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setPosition(float pct){
	if(!javaSoundPlayer){
		ofLogError() << "cannot set position on an unloaded sound player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaPositionMethod = env->GetMethodID(javaClass,"setPosition","(F)V");
	if(!javaPositionMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java setPosition for SoundPlayer");
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaPositionMethod,pct);
}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setPositionMS(int ms){
	if(!javaSoundPlayer){
		ofLogError() << "cannot set positionMS on an unloaded sound player";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}

	jmethodID javaPositionMethod = env->GetMethodID(javaClass,"setPositionMS","(I)V");
	if(!javaPositionMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java setPositionMS for SoundPlayer");
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaPositionMethod,ms);

}

//------------------------------------------------------------
float ofxAndroidSoundPlayer::getPosition(){
	if(!javaSoundPlayer){
		ofLogError() << "cannot query position on an unloaded sound player";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaPositionMethod = env->GetMethodID(javaClass,"getPosition","()F");
	if(!javaPositionMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java getPosition for SoundPlayer");
		return 0;
	}

	return env->CallFloatMethod(javaSoundPlayer,javaPositionMethod);
}

//------------------------------------------------------------
int ofxAndroidSoundPlayer::getPositionMS(){
	if(!javaSoundPlayer){
		ofLogError() << "cannot query positionMS on an unloaded sound player";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaPositionMethod = env->GetMethodID(javaClass,"getPositionMS","()I");
	if(!javaPositionMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java getPositionMS for SoundPlayer");
		return 0;
	}

	return env->CallIntMethod(javaSoundPlayer,javaPositionMethod);

}

//------------------------------------------------------------
bool ofxAndroidSoundPlayer::getIsPlaying(){
	if(!javaSoundPlayer){
		ofLogError() << "cannot query playing on an unloaded sound player";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaIsPlayingMethod = env->GetMethodID(javaClass,"getIsPlaying","()Z");
	if(!javaIsPlayingMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java getIsPlaying for SoundPlayer");
		return 0;
	}

	return env->CallBooleanMethod(javaSoundPlayer,javaIsPlayingMethod);

}

//------------------------------------------------------------
float ofxAndroidSoundPlayer::getSpeed(){
	if(!javaSoundPlayer){
		ofLogError() << "cannot query speed on an unloaded sound player";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaSpeedMethod = env->GetMethodID(javaClass,"getSpeed","()F");
	if(!javaSpeedMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java getSpeed for SoundPlayer");
		return 0;
	}

	return env->CallFloatMethod(javaSoundPlayer,javaSpeedMethod);

}

//------------------------------------------------------------
float ofxAndroidSoundPlayer::getPan(){
	if(!javaSoundPlayer){
		ofLogError() << "cannot query pan on an unloaded sound player";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaPanMethod = env->GetMethodID(javaClass,"getPan","()F");
	if(!javaPanMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java getPan for SoundPlayer");
		return 0;
	}

	return env->CallFloatMethod(javaSoundPlayer,javaPanMethod);

}

//------------------------------------------------------------
bool ofxAndroidSoundPlayer::getIsPaused(){
	if(!javaSoundPlayer){
		ofLogError() << "cannot query paused on an unloaded sound player";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaPausedMethod = env->GetMethodID(javaClass,"getIsPaused","()Z");
	if(!javaPausedMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java getIsPaused for SoundPlayer");
		return 0;
	}

	return env->CallBooleanMethod(javaSoundPlayer,javaPausedMethod);

}

float ofxAndroidSoundPlayer::getVolume(){
	if(!javaSoundPlayer){
		ofLogError() << "cannot get volume on an unloaded sound player";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaVolumeMethod = env->GetMethodID(javaClass,"getVolume","(V)F");
	if(!javaVolumeMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java getVolume for SoundPlayer");
		return 0;
	}

	return env->CallFloatMethod(javaSoundPlayer,javaVolumeMethod);
}


bool ofxAndroidSoundPlayer::isLoaded(){
	if(!javaSoundPlayer){
		ofLogError() << "cannot get loaded state on an unloaded sound player";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return 0;
	}

	jmethodID javaIsLoadedMethod = env->GetMethodID(javaClass,"isLoaded","(V)Z");
	if(!javaIsLoadedMethod){
		ofLog(OF_LOG_ERROR,"Failed to get the java isLoaded for SoundPlayer");
		return 0;
	}

	return env->CallBooleanMethod(javaSoundPlayer,javaIsLoadedMethod);
}
