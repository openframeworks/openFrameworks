#include "ofxAndroidSoundPlayer.h"
#include "ofxAndroidUtils.h"
#include "ofUtils.h"
#include "ofLog.h"

//------------------------------------------------------------
ofxAndroidSoundPlayer::ofxAndroidSoundPlayer(){
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "constructor: failed to get environment using GetEnv()";
		return;
	}
	jclass localClass = env->FindClass("cc/openframeworks/OFAndroidSoundPlayer");
	javaClass = (jclass) env->NewGlobalRef(localClass);

	if(!javaClass){
		ofLogError("ofxAndroidSoundPlayer") << "constructor: couldn't get java class for SoundPlayer";
		return;
	}


	jmethodID constructor = env->GetMethodID(javaClass,"<init>","()V");
	if(!constructor){
		ofLogError("ofxAndroidSoundPlayer") << "constructor: couldn't get java constructor for SoundPlayer";
		return;
	}

	javaSoundPlayer = env->NewObject(javaClass,constructor);
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "constructor: couldn't create java SoundPlayer";
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
bool ofxAndroidSoundPlayer::load(const of::filesystem::path& fileName, bool stream){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "loadSound(): java SoundPlayer not loaded";
		return false;
	}

	JNIEnv *env = ofGetJNIEnv();
	jmethodID javaLoadMethod = env->GetMethodID(javaClass,"loadSound","(Ljava/lang/String;Z)V");
	if(!javaLoadMethod){
		ofLogError("ofxAndroidSoundPlayer") << "loadSound(): couldn't get java loadSound for SoundPlayer";
		return false;
	}

	jstring javaFileName = ofGetJNIEnv()->NewStringUTF(ofToDataPath(fileName,true).c_str());
	env->CallVoidMethod(javaSoundPlayer,javaLoadMethod,javaFileName,stream?1:0);
	env->DeleteLocalRef((jobject)javaFileName);
	return true;
}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::unload(){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "unloadSound(): java SoundPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "unloadSound(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaUnloadMethod = env->GetMethodID(javaClass,"unloadSound","()V");
	if(!javaUnloadMethod){
		ofLogError("ofxAndroidSoundPlayer") << "unloadSound(): couldn't get java unloadSound for SoundPlayer";
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaUnloadMethod);
}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::play(){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "play(): java SoundPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "play(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaPlayMethod = env->GetMethodID(javaClass,"play","()V");
	if(!javaPlayMethod){
		ofLogError("ofxAndroidSoundPlayer") << "play(): couldn't get java play for SoundPlayer";
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaPlayMethod);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::stop(){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "stop(): java SoundPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "stop(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaStopMethod = env->GetMethodID(javaClass,"stop","()V");
	if(!javaStopMethod){
		ofLogError("ofxAndroidSoundPlayer") << "stop(): couldn't get java play for SoundPlayer";
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaStopMethod);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setVolume(float vol){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "setVolume(): java SoundPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "setVolume(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaVolumeMethod = env->GetMethodID(javaClass,"setVolume","(F)V");
	if(!javaVolumeMethod){
		ofLogError("ofxAndroidSoundPlayer") << "setVolume(): couldn't get java setVolume for SoundPlayer";
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaVolumeMethod,vol);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setPan(float vol){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "setPan(): java SoundPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "setPan(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaPanMethod = env->GetMethodID(javaClass,"setPan","(F)V");
	if(!javaPanMethod){
		ofLogError("ofxAndroidSoundPlayer") << "setPan(): couldn't get java setPan for SoundPlayer";
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaPanMethod,vol);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setSpeed(float spd){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "setSpeed(): java SoundPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "setSpeed(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaSpeedMethod = env->GetMethodID(javaClass,"setSpeed","(F)V");
	if(!javaSpeedMethod){
		ofLogError("ofxAndroidSoundPlayer") << "setSpeed(): couldn't get java setSpeed for SoundPlayer";
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaSpeedMethod,spd);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setPaused(bool bP){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "setPaused(): java SoundPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "setPaused(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaPausedMethod = env->GetMethodID(javaClass,"setPaused","(Z)V");
	if(!javaPausedMethod){
		ofLogError("ofxAndroidSoundPlayer") << "setPaused(): couldn't get java setPaused for SoundPlayer";
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaPausedMethod,bP?1:0);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setLoop(bool bLp){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "setLoop(): java SoundPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "setLoop(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaLoopMethod = env->GetMethodID(javaClass,"setLoop","(Z)V");
	if(!javaLoopMethod){
		ofLogError("ofxAndroidSoundPlayer") << "setLoop(): couldn't get java setLoop for SoundPlayer";
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaLoopMethod,bLp?1:0);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setMultiPlay(bool bMp){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "setMultiPlay(): java SoundPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "setMultiPlay(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaMultiplayMethod = env->GetMethodID(javaClass,"setMultiPlay","(Z)V");
	if(!javaMultiplayMethod){
		ofLogError("ofxAndroidSoundPlayer") << "setMultiPlay(): couldn't get java setMultiPlay for SoundPlayer";
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaMultiplayMethod,bMp?1:0);

}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setPosition(float pct){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "setPosition(): java SoundPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "setPosition(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaPositionMethod = env->GetMethodID(javaClass,"setPosition","(F)V");
	if(!javaPositionMethod){
		ofLogError("ofxAndroidSoundPlayer") << "setPosition(): couldn't get java setPosition for SoundPlayer";
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaPositionMethod,pct);
}

//------------------------------------------------------------
void ofxAndroidSoundPlayer::setPositionMS(int ms){
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "setPositionMS(): java SoundPlayer not loaded";
		return;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "setPositionMS(): couldn't get environment using GetEnv()";
		return;
	}

	jmethodID javaPositionMethod = env->GetMethodID(javaClass,"setPositionMS","(I)V");
	if(!javaPositionMethod){
		ofLogError("ofxAndroidSoundPlayer") << "setPositionMS(): couldn't get java setPositionMS for SoundPlayer";
		return;
	}

	env->CallVoidMethod(javaSoundPlayer,javaPositionMethod,ms);

}

//------------------------------------------------------------
float ofxAndroidSoundPlayer::getPosition() const{
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "getPosition(): java SoundPlayer not loaded";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "getPosition(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaPositionMethod = env->GetMethodID(javaClass,"getPosition","()F");
	if(!javaPositionMethod){
		ofLogError("ofxAndroidSoundPlayer") << "getPosition(): couldn't get java getPosition for SoundPlayer";
		return 0;
	}

	return env->CallFloatMethod(javaSoundPlayer,javaPositionMethod);
}

//------------------------------------------------------------
int ofxAndroidSoundPlayer::getPositionMS() const{
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "getPositionMS(): java SoundPlayer not loaded";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "getPositionMS(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaPositionMethod = env->GetMethodID(javaClass,"getPositionMS","()I");
	if(!javaPositionMethod){
		ofLogError("ofxAndroidSoundPlayer") << "getPositionMS(): couldn't get java getPositionMS for SoundPlayer";
		return 0;
	}

	return env->CallIntMethod(javaSoundPlayer,javaPositionMethod);

}

//------------------------------------------------------------
bool ofxAndroidSoundPlayer::isPlaying() const{
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "getIsPlaying(): java SoundPlayer not loaded";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "getIsPlaying(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaIsPlayingMethod = env->GetMethodID(javaClass,"getIsPlaying","()Z");
	if(!javaIsPlayingMethod){
		ofLogError("ofxAndroidSoundPlayer") << "getIsPlaying(): couldn't get java getIsPlaying for SoundPlayer";
		return 0;
	}

	return env->CallBooleanMethod(javaSoundPlayer,javaIsPlayingMethod);

}

//------------------------------------------------------------
float ofxAndroidSoundPlayer::getSpeed() const{
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "getSpeed(): java SoundPlayer not loaded";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "getSpeed(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaSpeedMethod = env->GetMethodID(javaClass,"getSpeed","()F");
	if(!javaSpeedMethod){
		ofLogError("ofxAndroidSoundPlayer") << "getSpeed(): couldn't get java getSpeed for SoundPlayer";
		return 0;
	}

	return env->CallFloatMethod(javaSoundPlayer,javaSpeedMethod);

}

//------------------------------------------------------------
float ofxAndroidSoundPlayer::getPan() const{
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "getPan(): java SoundPlayer not loaded";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "getPan(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaPanMethod = env->GetMethodID(javaClass,"getPan","()F");
	if(!javaPanMethod){
		ofLogError("ofxAndroidSoundPlayer") << "getPan(): couldn't get java getPan for SoundPlayer";
		return 0;
	}

	return env->CallFloatMethod(javaSoundPlayer,javaPanMethod);

}

//------------------------------------------------------------
bool ofxAndroidSoundPlayer::isPaused() const{
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "getIsPaused(): java SoundPlayer not loaded";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "getIsPaused(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaPausedMethod = env->GetMethodID(javaClass,"getIsPaused","()Z");
	if(!javaPausedMethod){
		ofLogError("ofxAndroidSoundPlayer") << "getIsPaused(): couldn't get java getIsPaused for SoundPlayer";
		return 0;
	}

	return env->CallBooleanMethod(javaSoundPlayer,javaPausedMethod);

}

float ofxAndroidSoundPlayer::getVolume() const{
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "getVolume(): java SoundPlayer not loaded";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "getVolume(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaVolumeMethod = env->GetMethodID(javaClass,"getVolume","()F");
	if(!javaVolumeMethod){
		ofLogError("ofxAndroidSoundPlayer") << "getVolume(): couldn't get java getVolume for SoundPlayer";
		return 0;
	}

	return env->CallFloatMethod(javaSoundPlayer,javaVolumeMethod);
}


bool ofxAndroidSoundPlayer::isLoaded() const{
	if(!javaSoundPlayer){
		ofLogError("ofxAndroidSoundPlayer") << "isLoaded(): java SoundPlayer not loaded";
		return 0;
	}
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError("ofxAndroidSoundPlayer") << "isLoaded(): couldn't get environment using GetEnv()";
		return 0;
	}

	jmethodID javaIsLoadedMethod = env->GetMethodID(javaClass,"isLoaded","()Z");
	if(!javaIsLoadedMethod){
		ofLogError("ofxAndroidSoundPlayer") << "isLoaded(): couldn't get java isLoaded for SoundPlayer";
		return 0;
	}

	return env->CallBooleanMethod(javaSoundPlayer,javaIsLoadedMethod);
}
