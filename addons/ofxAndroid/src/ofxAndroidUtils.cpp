
#include "ofxAndroidUtils.h"

// fix for undefined symbols from ndk r8c
extern "C" {
  extern void *__dso_handle __attribute__((__visibility__ ("hidden")));
  void *__dso_handle;
}

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
