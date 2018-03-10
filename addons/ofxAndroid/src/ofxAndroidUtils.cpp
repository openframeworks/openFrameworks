

#include "ofxAndroidUtils.h"

#ifndef __clang__
// fix for undefined symbols from ndk r8c
extern "C" {
  extern void *__dso_handle __attribute__((__visibility__ ("hidden")));
  void *__dso_handle;
}

extern int atexit (void (*func)(void)) noexcept;
int atexit (void (*func)(void)){
	return 0;
}
#endif
#include "ofLog.h"

using namespace std;

bool ofxAndroidIsOnline(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidIsOnline(): couldn't find OFAndroid java class";
		return false;
	}

	jmethodID isOnline = ofGetJNIEnv()->GetStaticMethodID(javaClass,"isOnline","()Z");
	if(!isOnline){
		ofLogError("ofxAndroidUtils") << "ofxAndroidIsOnline(): couldn't find OFAndroid isOnline method";
		return false;
	}
	return ofGetJNIEnv()->CallStaticBooleanMethod(javaClass,isOnline);
}

bool ofxAndroidIsWifiOnline(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidIsWifiOnline(): couldn't find OFAndroid java class";
		return false;
	}

	jmethodID isWifiOnline = ofGetJNIEnv()->GetStaticMethodID(javaClass,"isWifiOnline","()Z");
	if(!isWifiOnline){
		ofLogError("ofxAndroidUtils") << "ofxAndroidIsWifiOnline(): couldn't find OFAndroid isWifiOnline method";
		return false;
	}
	return ofGetJNIEnv()->CallStaticBooleanMethod(javaClass,isWifiOnline);

}

bool ofxAndroidIsMobileOnline(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidIsMobileOnline(): couldn't find OFAndroid java class";
		return false;
	}

	jmethodID isMobileOnline = ofGetJNIEnv()->GetStaticMethodID(javaClass,"isMobileOnline","()Z");
	if(!isMobileOnline){
		ofLogError("ofxAndroidUtils") << "ofxAndroidIsMobileOnline(): couldn't find OFAndroid isMobileOnline method";
		return false;
	}
	return ofGetJNIEnv()->CallStaticBooleanMethod(javaClass,isMobileOnline);
}

string ofxAndroidGetStringRes(string id){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidGetStringRes(): couldn't find OFAndroid java class";
		return "";
	}

	jmethodID getStringRes = ofGetJNIEnv()->GetStaticMethodID(javaClass,"getStringRes","(Ljava/lang/String;)Ljava/lang/String;");
	if(!getStringRes){
		ofLogError("ofxAndroidUtils") << "ofxAndroidGetStringRes(): couldn't find OFAndroid getStringRes method";
		return "";
	}
	jstring jId = ofGetJNIEnv()->NewStringUTF(id.c_str());
	jstring str = (jstring)	ofGetJNIEnv()->CallStaticObjectMethod(javaClass,getStringRes,jId);

	ofGetJNIEnv()->DeleteLocalRef((jobject)jId);

	jboolean isCopy;
	return ofGetJNIEnv()->GetStringUTFChars(str,&isCopy);

}

void ofxAndroidPauseApp(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidPauseApp(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID pauseApp = ofGetJNIEnv()->GetStaticMethodID(javaClass,"pauseApp","()V");
	if(!pauseApp){
		ofLogError("ofxAndroidUtils") << "ofxAndroidPauseApp(): couldn't find OFAndroid pauseApp method";
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,pauseApp);
}

void ofxAndroidAlertBox(string msg){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidAlertBox(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID alertBox = ofGetJNIEnv()->GetStaticMethodID(javaClass,"alertBox","(Ljava/lang/String;)V");
	if(!alertBox){
		ofLogError("ofxAndroidUtils") << "ofxAndroidAlertBox(): couldn't find OFAndroid alertBox method";
		return;
	}
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(msg.c_str());
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,alertBox,jMsg);
	ofGetJNIEnv()->DeleteLocalRef((jobject)jMsg);
}


int ofxAndroidProgressBox(string msg){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidProgressBox(): couldn't find OFAndroid java class";
		return -1;
	}

	jmethodID progressBox = ofGetJNIEnv()->GetStaticMethodID(javaClass,"progressBox","(Ljava/lang/String;)I");
	if(!progressBox){
		ofLogError("ofxAndroidUtils") << "ofxAndroidProgressBox(): couldn't find OFAndroid alertBox method";
		return -1;
	}
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(msg.c_str());
	int ret = ofGetJNIEnv()->CallStaticIntMethod(javaClass,progressBox,jMsg);
	ofGetJNIEnv()->DeleteLocalRef((jobject)jMsg);
	return ret;
}

void ofxAndroidDismissProgressBox(int id){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidDismissProgressBox(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID dismissProgressBox = ofGetJNIEnv()->GetStaticMethodID(javaClass,"dismissProgressBox","(I)V");
	if(!dismissProgressBox){
		ofLogError("ofxAndroidUtils") << "ofxAndroidDismissProgressBox(): couldn't find OFAndroid dismissProgressBox method";
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,dismissProgressBox,id);
}

string ofxAndroidGetPermissioString(ofxAndroidPermission permission){
    switch(permission){
        case OFX_ANDROID_PERMISSION_READ_CALENDAR:
            return "android.permission.READ_CALENDAR";
        case OFX_ANDROID_PERMISSION_WRITE_CALENDAR:
            return "android.permission.WRITE_CALENDAR";
        case OFX_ANDROID_PERMISSION_CAMERA:
            return "android.permission.CAMERA";
        case OFX_ANDROID_PERMISSION_READ_CONTACTS:
            return "android.permission.READ_CONTACTS";
        case OFX_ANDROID_PERMISSION_WRITE_CONTACTS:
            return "android.permission.WRITE_CONTACTS";
        case OFX_ANDROID_PERMISSION_GET_ACCOUNTS:
            return "android.permission.GET_ACCOUNTS";
        case OFX_ANDROID_PERMISSION_ACCESS_FINE_LOCATION:
            return "android.permission.ACCESS_FINE_LOCATION";
        case OFX_ANDROID_PERMISSION_ACCESS_COARSE_LOCATION:
            return "android.permission.ACCESS_COARSE_LOCATION";
        case OFX_ANDROID_PERMISSION_RECORD_AUDIO:
            return "android.permission.RECORD_AUDIO";
        case OFX_ANDROID_PERMISSION_READ_PHONE_STATE:
            return "android.permission.READ_PHONE_STATE";
        case OFX_ANDROID_PERMISSION_CALL_PHONE:
            return "android.permission.CALL_PHONE";
        case OFX_ANDROID_PERMISSION_READ_CALL_LOG:
            return "android.permission.READ_CALL_LOG";
        case OFX_ANDROID_PERMISSION_WRITE_CALL_LOG:
            return "android.permission.WRITE_CALL_LOG";
        case OFX_ANDROID_PERMISSION_ADD_VOICEMAIL:
            return "android.permission.ADD_VOICEMAIL";
        case OFX_ANDROID_PERMISSION_USE_SIP:
            return "android.permission.USE_SIP";
        case OFX_ANDROID_PERMISSION_PROCESS_OUTGOING_CALLS:
            return "android.permission.PROCESS_OUTGOING_CALLS";
        case OFX_ANDROID_PERMISSION_BODY_SENSORS:
            return "android.permission.BODY_SENSORS";
        case OFX_ANDROID_PERMISSION_SEND_SMS:
            return "android.permission.SEND_SMS";
        case OFX_ANDROID_PERMISSION_RECEIVE_SMS:
            return "android.permission.RECEIVE_SMS";
        case OFX_ANDROID_PERMISSION_READ_SMS:
            return "android.permission.READ_SMS";
        case OFX_ANDROID_PERMISSION_RECEIVE_WAP_PUSH:
            return "android.permission.RECEIVE_WAP_PUSH";
        case OFX_ANDROID_PERMISSION_RECEIVE_MMS:
            return "android.permission.RECEIVE_MMS";
        case OFX_ANDROID_PERMISSION_READ_EXTERNAL_STORAGE:
            return "android.permission.READ_EXTERNAL_STORAGE";
        case OFX_ANDROID_PERMISSION_WRITE_EXTERNAL_STORAGE:
            return "android.permission.WRITE_EXTERNAL_STORAGE";
    }
}

void ofxAndroidRequestPermission(ofxAndroidPermission permission){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidRequestPermission(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID requestPermission = ofGetJNIEnv()->GetStaticMethodID(javaClass,"requestPermission","(Ljava/lang/String;)V");
	if(!requestPermission){
		ofLogError("ofxAndroidUtils") << "ofxAndroidRequestPermission(): couldn't find OFAndroid requestPermission method";
		return;
	}
    string stringPermission = ofxAndroidGetPermissioString(permission);
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(stringPermission.c_str());

	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,requestPermission,jMsg);
}


bool ofxAndroidCheckPermission(ofxAndroidPermission permission){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidCheckPermission(): couldn't find OFAndroid java class";
		return false;
	}

	jmethodID checkPermission = ofGetJNIEnv()->GetStaticMethodID(javaClass,"checkPermission","(Ljava/lang/String;)Z");
	if(!checkPermission){
		ofLogError("ofxAndroidUtils") << "ofxAndroidCheckPermission(): couldn't find OFAndroid requestPermission method";
		return false;
	}
	string stringPermission = ofxAndroidGetPermissioString(permission);
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(stringPermission.c_str());

    return ofGetJNIEnv()->CallStaticBooleanMethod(javaClass,checkPermission,jMsg);
}



void ofxAndroidOkCancelBox(string msg){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidOkCancelBox(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID okCancelBox = ofGetJNIEnv()->GetStaticMethodID(javaClass,"okCancelBox","(Ljava/lang/String;)V");
	if(!okCancelBox){
		ofLogError("ofxAndroidUtils") << "ofxAndroidOkCancelBox(): couldn't find OFAndroid okCancelBox method";
		return;
	}
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(msg.c_str());
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,okCancelBox,jMsg);
	ofGetJNIEnv()->DeleteLocalRef((jobject)jMsg);
}

void ofxAndroidYesNoBox(string msg){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidYesNoBox(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID method = ofGetJNIEnv()->GetStaticMethodID(javaClass,"yesNoBox","(Ljava/lang/String;)V");
	if(!method){
		ofLogError("ofxAndroidUtils") << "ofxAndroidYesNoBox(): couldn't find OFAndroid okCancelBox method";
		return;
	}
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(msg.c_str());
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,method,jMsg);
	ofGetJNIEnv()->DeleteLocalRef((jobject)jMsg);
}


void ofxAndroidAlertTextBox(string question, string text){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidAlertTextBox(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID alertTextBox = ofGetJNIEnv()->GetStaticMethodID(javaClass,"alertTextBox","(Ljava/lang/String;Ljava/lang/String;)V");
	if(!alertTextBox){
		ofLogError("ofxAndroidUtils") << "ofxAndroidAlertTextBox(): couldn't find OFAndroid alertTextBox method";
		return;
	}
	jstring jQuestion = ofGetJNIEnv()->NewStringUTF(question.c_str());
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(text.c_str());
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,alertTextBox,jQuestion,jMsg);
	ofGetJNIEnv()->DeleteLocalRef((jobject)jMsg);
	ofGetJNIEnv()->DeleteLocalRef((jobject)jQuestion);
}

bool ofxAndroidAlertListBox(string title, const vector<string> & list){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidAlertListBox(): couldn't find OFAndroid java class";
		return "";
	}

	jmethodID alertListBox = ofGetJNIEnv()->GetStaticMethodID(javaClass,"alertListBox","(Ljava/lang/String;[Ljava/lang/String;)Z");
	if(!alertListBox){
		ofLogError("ofxAndroidUtils") << "ofxAndroidAlertListBox(): couldn't find OFAndroid alertListBox method";
		return "";
	}
	jstring jTitle = ofGetJNIEnv()->NewStringUTF(title.c_str());

	jclass jStringClass = ofGetJNIEnv()->FindClass("java/lang/String");
	jobjectArray jList = ofGetJNIEnv()->NewObjectArray(list.size(), jStringClass, NULL);
	for(int i=0;i<(int)list.size(); i++){
		jstring element = ofGetJNIEnv()->NewStringUTF(list[i].c_str());
		ofGetJNIEnv()->SetObjectArrayElement(jList,i,(jobject)element);
		ofGetJNIEnv()->DeleteLocalRef((jobject)element);
	}

	jboolean res = ofGetJNIEnv()->CallStaticBooleanMethod(javaClass,alertListBox,jTitle,jList);
	ofGetJNIEnv()->DeleteLocalRef((jobject)jTitle);
	ofGetJNIEnv()->DeleteLocalRef((jobject)jList);
	return res;
}

void ofxAndroidToast(string msg){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidToast(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID toast = ofGetJNIEnv()->GetStaticMethodID(javaClass,"toast","(Ljava/lang/String;)V");
	if(!toast){
		ofLogError("ofxAndroidUtils") << "ofxAndroidToast(): couldn't find OFAndroid toast method";
		return;
	}
	jstring jMsg = ofGetJNIEnv()->NewStringUTF(msg.c_str());
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,toast,jMsg);
	ofGetJNIEnv()->DeleteLocalRef((jobject)jMsg);
}

void ofxAndroidLockScreenSleep(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidLockScreenSleep(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID lockScreenSleep = ofGetJNIEnv()->GetStaticMethodID(javaClass,"lockScreenSleep","()V");
	if(!lockScreenSleep){
		ofLogError("ofxAndroidUtils") << "ofxAndroidLockScreenSleep(): couldn't find OFAndroid lockScreenSleep method";
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,lockScreenSleep);
}

void ofxAndroidUnlockScreenSleep(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidUnlockScreenSleep(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID unlockScreenSleep = ofGetJNIEnv()->GetStaticMethodID(javaClass,"unlockScreenSleep","()V");
	if(!unlockScreenSleep){
		ofLogError("ofxAndroidUtils") << "ofxAndroidUnlockScreenSleep(): couldn't find OFAndroid unlockScreenSleep method";
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,unlockScreenSleep);
}

bool ofxAndroidCheckSDCardMounted(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidCheckSDCardMounted(): couldn't find OFAndroid java class";
		return false;
	}

	jmethodID unlockScreenSleep = ofGetJNIEnv()->GetStaticMethodID(javaClass,"checkSDCardMounted","()Z");
	if(!unlockScreenSleep){
		ofLogError("ofxAndroidUtils") << "ofxAndroidCheckSDCardMounted(): couldn't find OFAndroid checkSDCardMounted method";
		return false;
	}
	return ofGetJNIEnv()->CallStaticBooleanMethod(javaClass,unlockScreenSleep);

}

void ofxAndroidEnableMulticast(){

	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidEnableMulticast(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID method = ofGetJNIEnv()->GetStaticMethodID(javaClass,"enableMulticast","()V");
	if(!method){
		ofLogError("ofxAndroidUtils") << "ofxAndroidEnableMulticast(): couldn't find OFAndroid enableMulticast method";
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,method);
}

void ofxAndroidDisableMulticast(){

	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidDisableMulticast(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID method = ofGetJNIEnv()->GetStaticMethodID(javaClass,"disableMulticast","()V");
	if(!method){
		ofLogError("ofxAndroidUtils") << "ofxAndroidDisableMulticast(): couldn't find OFAndroid disableMulticast method";
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,method);
}

void ofxAndroidSetViewItemChecked(string item_name, bool checked){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"ofxAndroidSetViewItemChecked: cannot find OFAndroid java class");
		return;
	}

	jmethodID setViewItemChecked = ofGetJNIEnv()->GetStaticMethodID(javaClass,"setViewItemChecked","(Ljava/lang/String;Z)V");
	if(!setViewItemChecked){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroid setViewItemChecked method");
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,setViewItemChecked,ofGetJNIEnv()->NewStringUTF(item_name.c_str()),checked);
}

string ofxAndroidRandomUUID(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidRandomUUID(): couldn't find OFAndroid java class";
		return "";
	}


	jmethodID randomUUID = ofGetJNIEnv()->GetStaticMethodID(javaClass,"getRandomUUID","()Ljava/lang/String;");
	if(!randomUUID){
		ofLogError("ofxAndroidUtils") << "ofxAndroidRandomUUID(): couldn't find OFAndroid randomUUID method";
		return "";
	}
	jstring str = (jstring)	ofGetJNIEnv()->CallStaticObjectMethod(javaClass,randomUUID);

	jboolean isCopy;
	return ofGetJNIEnv()->GetStringUTFChars(str,&isCopy);
}

void ofxAndroidMonitorNetworkState(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidMonitorNetworkState(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID method = ofGetJNIEnv()->GetStaticMethodID(javaClass,"monitorNetworkState","()V");
	if(!method){
		ofLogError("ofxAndroidUtils") << "ofxAndroidMonitorNetworkState(): couldn't find OFAndroid monitorNetworkState method";
		return;
	}
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,method);
}

string ofxAndroidGetTextBoxResult(){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidGetTextBoxResult(): could't find OFAndroid java class";
		return "";
	}


	jmethodID getLastTextBoxResult = ofGetJNIEnv()->GetStaticMethodID(javaClass,"getLastTextBoxResult","()Ljava/lang/String;");
	if(!getLastTextBoxResult){
		ofLogError("ofxAndroidUtils") << "ofxAndroidGetTextBoxResult(): couldn't find OFAndroid getLastTextBoxResult method";
		return "";
	}
	jstring str = (jstring)	ofGetJNIEnv()->CallStaticObjectMethod(javaClass,getLastTextBoxResult);

	jboolean isCopy;
	return ofGetJNIEnv()->GetStringUTFChars(str,&isCopy);
}

void ofxAndroidLaunchBrowser(string url){
	jclass javaClass = ofGetJavaOFAndroid();

	if(javaClass==0){
		ofLogError("ofxAndroidUtils") << "ofxAndroidLaunchBrowser(): couldn't find OFAndroid java class";
		return;
	}

	jmethodID method = ofGetJNIEnv()->GetStaticMethodID(javaClass,"launchBrowser","(Ljava/lang/String;)V");
	if(!method){
		ofLogError("ofxAndroidUtils") << "ofxAndroidLaunchBrowser(): couldn't find OFAndroid launchBrowser method";
		return;
	}
	jstring jUrl = ofGetJNIEnv()->NewStringUTF(url.c_str());
	ofGetJNIEnv()->CallStaticVoidMethod(javaClass,method,jUrl);
	ofGetJNIEnv()->DeleteLocalRef((jobject)jUrl);
}

void ofxAndroidNotifyLoadPercent(float percent){
	jobject activity = ofGetOFActivityObject();
	jclass activityClass = ofGetJNIEnv()->FindClass("cc/openframeworks/OFActivity");
	jmethodID onLoadPercent = ofGetJNIEnv()->GetMethodID(activityClass,"onLoadPercent","(F)V");
	ofGetJNIEnv()->CallVoidMethod(activity,onLoadPercent,(jfloat)percent);
}

ofxAndroidEventsClass & ofxAndroidEvents(){
	static ofxAndroidEventsClass * events = new ofxAndroidEventsClass;
	return *events;
}

jmethodID ofxJavaGetMethodID(jclass classID, std::string methodName, std::string methodSignature) {

	jmethodID result = ofGetJNIEnv()->GetMethodID(classID, methodName.c_str(), methodSignature.c_str());

	if(!result){

		ofLogError("ofxJavaGetMethodID") << "couldn't find instance method '"
		<< methodName << "' with signature '"
		<< methodSignature << "' in class '"
		<< ofxJavaGetClassName(classID) << "'";
		return NULL;
	}

	return result;
}

jmethodID ofxJavaGetStaticMethodID(jclass classID, std::string methodName, std::string methodSignature) {

	jmethodID result = ofGetJNIEnv()->GetStaticMethodID(classID, methodName.c_str(), methodSignature.c_str());

	if(!result){

		ofLogError("ofxJavaGetStaticMethodID") << "couldn't find static method '"
		<< methodName << "' with signature '"
		<< methodSignature << "' in class '"
		<< ofxJavaGetClassName(classID) << "'";
		return NULL;
	}

	return result;
}

std::string ofxJavaGetClassName(jclass classID)
{
	return "[UNKNOWN]"; //TODO
}

jclass ofxJavaGetClassID(std::string className)
{
	jclass result = ofGetJNIEnv()->FindClass(className.c_str());

	if(!result){

		ofLogError("ofxJavaGetClassID") << "couldn't find class '"
        << className << "'";
		return NULL;
	}

	return result;
}

jfieldID ofxJavaGetStaticFieldID(jclass classID, std::string fieldName, std::string fieldType) {
	jfieldID result = ofGetJNIEnv()->GetStaticFieldID(classID, fieldName.c_str(), fieldType.c_str());

	if(!result){

		ofLogError("ofxJavaGetStaticFieldID") << "couldn't find static field '" <<
        fieldName << "' of type '" <<
        fieldType << "' in class '" <<
        ofxJavaGetClassName(classID) << "'";
		return NULL;
	}

	return result;
}

jobject ofxJavaGetStaticObjectField(jclass classID, std::string fieldName, std::string fieldType) {

	jfieldID fieldID = ofxJavaGetStaticFieldID(classID, fieldName, fieldType);

	if (!fieldID)
		return NULL;

	return ofGetJNIEnv()->GetStaticObjectField(classID, fieldID);
}

jobject ofxJavaGetStaticObjectField(std::string className, std::string fieldName, std::string fieldType) {

	jclass classID = ofxJavaGetClassID(className);

	if (!classID)
		return NULL;

	jobject result = ofxJavaGetStaticObjectField(classID, fieldName, fieldType);

	ofGetJNIEnv()->DeleteLocalRef(classID);

	return result;
}

void ofxJavaCallVoidMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args){
	jmethodID methodID = ofxJavaGetMethodID(classID, methodName, methodSignature);

	if (!methodID)
		return;

	ofGetJNIEnv()->CallVoidMethodV(object, methodID, args);
}

void ofxJavaCallVoidMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...) {

	va_list args;

	va_start(args, methodSignature);

	ofxJavaCallVoidMethod(object, classID, methodName, methodSignature, args);

	va_end(args);
}

void ofxJavaCallVoidMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...)
{
	jclass classID = ofxJavaGetClassID(className);

	if (!classID)
		return;

	va_list args;

	va_start(args, methodSignature);

	ofxJavaCallVoidMethod(object, classID, methodName, methodSignature, args);

	va_end(args);

	ofGetJNIEnv()->DeleteLocalRef(classID);
}

jobject ofxJavaCallStaticObjectMethod(jclass classID, std::string methodName, std::string methodSignature, va_list args)
{
	jmethodID methodID = ofxJavaGetStaticMethodID(classID, methodName, methodSignature);

	if (!methodID)
		return NULL;

	return ofGetJNIEnv()->CallStaticObjectMethodV(classID, methodID, args);
}

jobject ofxJavaCallStaticObjectMethod(jclass classID, std::string methodName, std::string methodSignature, ...)
{
	va_list args;

	va_start(args, methodSignature);

	jobject result = ofxJavaCallStaticObjectMethod(classID, methodName, methodSignature, args);

	va_end(args);

	return result;
}

jobject ofxJavaCallStaticObjectMethod(std::string className, std::string methodName, std::string methodSignature, ...)
{
	jclass classID = ofxJavaGetClassID(className);

	if (!classID)
		return NULL;

	va_list args;

	va_start(args, methodSignature);

	jobject result = ofxJavaCallStaticObjectMethod(classID, methodName, methodSignature, args);

	va_end(args);

	ofGetJNIEnv()->DeleteLocalRef(classID);

	return result;
}

jobject ofxJavaCallObjectMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args)
{
	jmethodID methodID = ofxJavaGetMethodID(classID, methodName, methodSignature);

	if (!methodID)
		return NULL;

	return ofGetJNIEnv()->CallObjectMethodV(object, methodID, args);
}

jobject ofxJavaCallObjectMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...)
{
	va_list args;

	va_start(args, methodSignature);

	jobject result = ofxJavaCallObjectMethod(object, classID, methodName, methodSignature, args);

	va_end(args);

	return result;
}

jobject ofxJavaCallObjectMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...)
{
	jclass classID = ofxJavaGetClassID(className);

	if (!classID)
		return NULL;

	va_list args;

	va_start(args, methodSignature);

	jobject result = ofxJavaCallObjectMethod(object, classID, methodName, methodSignature, args);

	va_end(args);

	ofGetJNIEnv()->DeleteLocalRef(classID);

	return result;
}


void ofxJavaCallStaticVoidMethod(jclass classID, std::string methodName, std::string methodSignature, va_list args)
{
	jmethodID methodID = ofxJavaGetStaticMethodID(classID, methodName, methodSignature);

	if (!methodID)
		return;

	ofGetJNIEnv()->CallStaticVoidMethodV(classID, methodID, args);
}

void ofxJavaCallStaticVoidMethod(jclass classID, std::string methodName, std::string methodSignature, ...)
{
	va_list args;

	va_start(args, methodSignature);

	ofxJavaCallStaticVoidMethod(classID, methodName, methodSignature, args);

	va_end(args);
}

void ofxJavaCallStaticVoidMethod(std::string className, std::string methodName, std::string methodSignature, ...)
{
	jclass classID = ofxJavaGetClassID(className);

	if (!classID)
		return;

	va_list args;

	va_start(args, methodSignature);

	ofxJavaCallStaticVoidMethod(classID, methodName, methodSignature, args);

	va_end(args);

	ofGetJNIEnv()->DeleteLocalRef(classID);
}

float ofxJavaCallFloatMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args){
	jmethodID methodID = ofxJavaGetMethodID(classID, methodName, methodSignature);

	if (!methodID){
		ofLogError() << "Couldn't find " << methodName << " for float call";
		return 0;
	}

	return ofGetJNIEnv()->CallFloatMethodV(object, methodID, args);
}

float ofxJavaCallFloatMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...){

	va_list args;

	va_start(args, methodSignature);

	auto result = ofxJavaCallFloatMethod(object, classID, methodName, methodSignature, args);

	va_end(args);

	return result;
}

float ofxJavaCallFloatMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...){
	jclass classID = ofxJavaGetClassID(className);

	if (!classID){
		ofLogError() << "Couldn't find " << className << " for float call";
		return 0;
	}

	va_list args;

	va_start(args, methodSignature);

	auto result = ofxJavaCallFloatMethod(object, classID, methodName, methodSignature, args);

	va_end(args);

	ofGetJNIEnv()->DeleteLocalRef(classID);

	return result;
}

int ofxJavaCallIntMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args){
	jmethodID methodID = ofxJavaGetMethodID(classID, methodName, methodSignature);

	if (!methodID){
		ofLogError() << "Couldn't find " << methodName << " for int call";
		return 0;
	}

	return ofGetJNIEnv()->CallIntMethodV(object, methodID, args);
}

int ofxJavaCallIntMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...){

	va_list args;

	va_start(args, methodSignature);

	auto result = ofxJavaCallIntMethod(object, classID, methodName, methodSignature, args);

	va_end(args);

	return result;

}

int ofxJavaCallIntMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...){
	jclass classID = ofxJavaGetClassID(className);

	if (!classID){
		ofLogError() << "Couldn't find " << className << " for int call";
		return 0;
	}

	va_list args;

	va_start(args, methodSignature);

	auto result = ofxJavaCallIntMethod(object, classID, methodName, methodSignature, args);

	va_end(args);

	ofGetJNIEnv()->DeleteLocalRef(classID);

	return result;
}

int64_t ofxJavaCallLongMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args){
	jmethodID methodID = ofxJavaGetMethodID(classID, methodName, methodSignature);

	if (!methodID){
		ofLogError() << "Couldn't find " << methodName << " for int64_t call";
		return 0;
	}

	return ofGetJNIEnv()->CallLongMethodV(object, methodID, args);
}

int64_t ofxJavaCallLongMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...){

	va_list args;

	va_start(args, methodSignature);

	auto result = ofxJavaCallLongMethod(object, classID, methodName, methodSignature, args);

	va_end(args);

	return result;

}

int64_t ofxJavaCallLongMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...){
	jclass classID = ofxJavaGetClassID(className);

	if (!classID){
		ofLogError() << "Couldn't find " << className << " for int64_t call";
		return 0;
	}

	va_list args;

	va_start(args, methodSignature);

	auto result = ofxJavaCallLongMethod(object, classID, methodName, methodSignature, args);

	va_end(args);

	ofGetJNIEnv()->DeleteLocalRef(classID);

	return result;
}

bool ofxJavaCallBoolMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, va_list args){
	jmethodID methodID = ofxJavaGetMethodID(classID, methodName, methodSignature);

	if (!methodID){
		ofLogError() << "Couldn't find " << methodName << " for bool call";
		return false;
	}

	return ofGetJNIEnv()->CallBooleanMethodV(object, methodID, args);
}

bool ofxJavaCallBoolMethod(jobject object, jclass classID, std::string methodName, std::string methodSignature, ...){

	va_list args;

	va_start(args, methodSignature);

	auto result = ofxJavaCallBoolMethod(object, classID, methodName, methodSignature, args);

	va_end(args);

	return result;

}

bool ofxJavaCallBoolMethod(jobject object, std::string className, std::string methodName, std::string methodSignature, ...){
	jclass classID = ofxJavaGetClassID(className);

	if (!classID){
		ofLogError() << "Couldn't find " << className << " for bool call";
		return false;
	}

	va_list args;

	va_start(args, methodSignature);

	auto result = ofxJavaCallBoolMethod(object, classID, methodName, methodSignature, args);

	va_end(args);

	ofGetJNIEnv()->DeleteLocalRef(classID);

	return result;
}

void ofxJavaGetJString(const std::string& str, jstring &jstr)
{
	jstr = ofGetJNIEnv()->NewStringUTF(str.c_str());
}

void ofxJstringToString(JNIEnv * env, jstring jstr, std::string &str)
{
    jboolean isCopy;
    const char *arrayChar = env->GetStringUTFChars(jstr, &isCopy);
    if(isCopy == JNI_FALSE)
        return;
    str = arrayChar;
}

ofxAndroidScaleEventArgs::ofxAndroidScaleEventArgs(jobject detector){
	this->detector = detector;
}

float ofxAndroidScaleEventArgs::getCurrentSpan(){
	return ofxJavaCallFloatMethod(detector,"android/view/ScaleGestureDetector","getCurrentSpan","()F");
}

float ofxAndroidScaleEventArgs::getCurrentSpanX(){
	return ofxJavaCallFloatMethod(detector,"android/view/ScaleGestureDetector","getCurrentSpanX","()F");
}

float ofxAndroidScaleEventArgs::getCurrentSpanY(){
	return ofxJavaCallFloatMethod(detector,"android/view/ScaleGestureDetector","getCurrentSpanY","()F");
}

int64_t ofxAndroidScaleEventArgs::getEventTime(){
	return ofxJavaCallLongMethod(detector,"android/view/ScaleGestureDetector","getEventTime","()L");
}

float ofxAndroidScaleEventArgs::getFocusX(){
	return ofxJavaCallFloatMethod(detector,"android/view/ScaleGestureDetector","getFocusX","()F");
}

float ofxAndroidScaleEventArgs::getFocusY(){
	return ofxJavaCallFloatMethod(detector,"android/view/ScaleGestureDetector","getFocusY","()F");
}

float ofxAndroidScaleEventArgs::getPreviousSpan(){
	return ofxJavaCallFloatMethod(detector,"android/view/ScaleGestureDetector","getPreviousSpan","()F");
}

float ofxAndroidScaleEventArgs::getPreviousSpanX(){
	return ofxJavaCallFloatMethod(detector,"android/view/ScaleGestureDetector","getPreviousSpanX","()F");
}

float ofxAndroidScaleEventArgs::getPreviousSpanY(){
	return ofxJavaCallFloatMethod(detector,"android/view/ScaleGestureDetector","getPreviousSpanY","()F");
}

float ofxAndroidScaleEventArgs::getScaleFactor(){
	return ofxJavaCallFloatMethod(detector,"android/view/ScaleGestureDetector","getScaleFactor","()F");
}

int64_t ofxAndroidScaleEventArgs::getTimeDelta(){
	return ofxJavaCallLongMethod(detector,"android/view/ScaleGestureDetector","getTimeDelta","()L");
}
