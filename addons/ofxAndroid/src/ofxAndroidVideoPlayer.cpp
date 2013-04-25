/*
 * ofxAndroidVideoPlayer.cpp
 *
 *  Created on: 25/04/2013
 *      Author: p
 */

#include "ofxAndroidVideoPlayer.h"
#include "ofxAndroidUtils.h"
#include "ofLog.h"

//---------------------------------------------------------------------------
ofxAndroidVideoPlayer::ofxAndroidVideoPlayer(){

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
	td.bFlipTexture = true;
	glGenTextures(1, (GLuint *)&td.textureID);

	glEnable(td.textureTarget);

	glBindTexture(td.textureTarget, (GLuint)td.textureID);

	glTexParameterf(td.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(td.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(td.textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(td.textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glDisable(td.textureTarget);
	td.bAllocated = true;
	texture.texData = td;

	jmethodID javaSetTexture = env->GetMethodID(javaClass,"setTexture","(I)V");
	if(!javaSetTexture){
		ofLog(OF_LOG_ERROR,"Failed to get the java setTexture for VideoPlayer");
		return false;
	}
	env->CallVoidMethod(javaVideoPlayer,javaSetTexture,texture.getTextureData().textureID);

	ofLogError() << width <<"," << height;

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
		matrix.getPtr()[i] = m[i];
	}

	texture.getTextureData().tex_t = 1.+1-matrix.getPtr()[0]; // Hack!
	texture.getTextureData().tex_u = 1.;

	env->ReleaseFloatArrayElements(matrixJava,m,0);

}

//---------------------------------------------------------------------------
ofMatrix4x4 & ofxAndroidVideoPlayer::getTextureMatrix() {
	return matrix;
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

