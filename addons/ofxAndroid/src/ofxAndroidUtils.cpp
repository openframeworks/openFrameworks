
#include "ofxAndroidUtils.h"

bool ofxAndroidIsOnline(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return false;
	}

	jmethodID isOnline = ofGetJNIEnv()->GetStaticMethodID(javaClass,"isOnline","()Z");
	if(!isOnline){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid isOnline method");
		return false;
	}
	return ofGetJNIEnv()->CallStaticBooleanMethod(javaClass,isOnline);
}

bool ofxAndroidIsWifiOnline(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return false;
	}

	jmethodID isWifiOnline = ofGetJNIEnv()->GetStaticMethodID(javaClass,"isWifiOnline","()Z");
	if(!isWifiOnline){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid isWifiOnline method");
		return false;
	}
	return ofGetJNIEnv()->CallStaticBooleanMethod(javaClass,isWifiOnline);

}

bool ofxAndroidIsMobileOnline(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return false;
	}

	jmethodID isMobileOnline = ofGetJNIEnv()->GetStaticMethodID(javaClass,"isMobileOnline","()Z");
	if(!isMobileOnline){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid isMobileOnline method");
		return false;
	}
	return ofGetJNIEnv()->CallStaticBooleanMethod(javaClass,isMobileOnline);
}

string ofxAndroidGetStringRes(string id){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return "";
	}

	jmethodID getStringRes = ofGetJNIEnv()->GetStaticMethodID(javaClass,"getStringRes","(Ljava/lang/String;)Ljava/lang/String;");
	if(!getStringRes){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid getStringRes method");
		return "";
	}
	jstring jId = ofGetJNIEnv()->NewStringUTF(id.c_str());
	jstring str = (jstring)	ofGetJNIEnv()->CallStaticObjectMethod(javaClass,getStringRes,jId);

	jboolean isCopy;
	return ofGetJNIEnv()->GetStringUTFChars(str,&isCopy);

}

void ofxAndroidPauseApp(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID pauseApp = ofGetJNIEnv()->GetStaticMethodID(javaClass,"pauseApp","()V");
	if(!pauseApp){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid pauseApp method");
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,pauseApp);
}

void ofxAndroidAlertBox(string msg){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID alertBox = ofGetJNIEnv()->GetStaticMethodID(javaClass,"alertBox","(Ljava/lang/String;)V");
	if(!alertBox){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid alertBox method");
		return;
	}
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(msg.c_str());
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,alertBox,jMsg);
}


int ofxAndroidProgressBox(string msg){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return -1;
	}

	jmethodID progressBox = ofGetJNIEnv()->GetStaticMethodID(javaClass,"progressBox","(Ljava/lang/String;)I");
	if(!progressBox){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid alertBox method");
		return -1;
	}
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(msg.c_str());
	return ofGetJNIEnv()->CallStaticIntMethod(javaClass,progressBox,jMsg);
}

void ofxAndroidDismissProgressBox(int id){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID dismissProgressBox = ofGetJNIEnv()->GetStaticMethodID(javaClass,"dismissProgressBox","(I)V");
	if(!dismissProgressBox){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid dismissProgressBox method");
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,dismissProgressBox,id);
}



void ofxAndroidOkCancelBox(string msg){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID okCancelBox = ofGetJNIEnv()->GetStaticMethodID(javaClass,"okCancelBox","(Ljava/lang/String;)V");
	if(!okCancelBox){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid okCancelBox method");
		return;
	}
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(msg.c_str());
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,okCancelBox,jMsg);
}

void ofxAndroidToast(string msg){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID toast = ofGetJNIEnv()->GetStaticMethodID(javaClass,"toast","(Ljava/lang/String;)V");
	if(!toast){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid toast method");
		return;
	}
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(msg.c_str());
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,toast,jMsg);
}

void ofxAndroidLockScreenSleep(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID lockScreenSleep = ofGetJNIEnv()->GetStaticMethodID(javaClass,"lockScreenSleep","()V");
	if(!lockScreenSleep){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid lockScreenSleep method");
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,lockScreenSleep);
}

void ofxAndroidUnlockScreenSleep(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return;
	}

	jmethodID unlockScreenSleep = ofGetJNIEnv()->GetStaticMethodID(javaClass,"unlockScreenSleep","()V");
	if(!unlockScreenSleep){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid unlockScreenSleep method");
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,unlockScreenSleep);
}

bool ofxAndroidCheckSDCardMounted(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid java class");
		return false;
	}

	jmethodID unlockScreenSleep = ofGetJNIEnv()->GetStaticMethodID(javaClass,"checkSDCardMounted","()Z");
	if(!unlockScreenSleep){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid checkSDCardMounted method");
		return false;
	}
	return ofGetJNIEnv()->CallStaticBooleanMethod(javaClass,unlockScreenSleep);

}

