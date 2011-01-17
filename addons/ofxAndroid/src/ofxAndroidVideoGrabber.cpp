/*
 * ofxAndroidVideoGrabber.cpp
 *
 *  Created on: 09/07/2010
 *      Author: arturo
 */

#include "ofxAndroidVideoGrabber.h"
#include "ofxAndroidUtils.h"
#include <map>
#include "ofAppRunner.h"
#include "ofUtils.h"

static map<int,ofxAndroidVideoGrabber*> instances;
static int cameraId;
static bool newPixels;
static unsigned char * buffer = 0;

static bool paused=true;


static void InitConvertTable();

static jclass getJavaClass(){
	JNIEnv *env = ofGetJNIEnv();

	jclass javaClass = env->FindClass("cc.openframeworks.OFAndroidVideoGrabber");

	if(javaClass==NULL){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidVideoGrabber java class");
	}

	return javaClass;
}

jobject getCamera(JNIEnv *env, jclass javaClass, jint id){
	jmethodID getInstanceMethod = env->GetStaticMethodID(javaClass,"getInstance","(I)Lcc/openframeworks/OFAndroidVideoGrabber;");
	if(!getInstanceMethod){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidVideoGrabber getInstance method");
		return NULL;
	}
	return env->CallStaticObjectMethod(javaClass,getInstanceMethod,id);
}

static void releaseJavaObject(){
	JNIEnv *env = ofGetJNIEnv();
	jclass javaClass = getJavaClass();
	if(!javaClass) return;
	jobject javaObject = getCamera(env,javaClass,cameraId);
	jmethodID javaOFDestructor = env->GetMethodID(javaClass,"release","()V");
	if(javaObject && javaOFDestructor){
		env->CallVoidMethod(javaObject,javaOFDestructor);
	}
}

/*void ofPauseVideoGrabbers(){
	paused = true;
	ofLog(OF_LOG_NOTICE,"ofVideoGrabber: releasing textures");
	map<int,ofVideoGrabber*>::iterator it;
	for(it=instances.begin(); it!=instances.end(); it++){
		it->second->getTextureReference().texData.textureID=0;
	}
}

void ofResumeVideoGrabbers(){
	ofLog(OF_LOG_NOTICE,"ofVideoGrabber: trying to allocate textures");
	map<int,ofVideoGrabber*>::iterator it;
	for(it=instances.begin(); it!=instances.end(); it++){
		it->second->getTextureReference().allocate(it->second->getWidth(),it->second->getHeight(),GL_RGB);
	}
	ofLog(OF_LOG_NOTICE,"ofVideoGrabber: textures allocated");
	paused = false;
}*/


ofxAndroidVideoGrabber::ofxAndroidVideoGrabber(){
	if(instances.size()>0){
		ofLog(OF_LOG_ERROR,"ofVideoGrabber: This version is limited to 1 camera at the same time");
		return;
	}

	JNIEnv *env = ofGetJNIEnv();

	jclass javaClass = getJavaClass();
	if(!javaClass) return;

	jmethodID videoGrabberConstructor = env->GetMethodID(javaClass,"<init>","()V");
	if(!videoGrabberConstructor){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidVideoGrabber constructor");
		return;
	}

	jobject javaObject = env->NewObject(javaClass,videoGrabberConstructor);
	jmethodID javaCameraId = env->GetMethodID(javaClass,"getId","()I");
	if(javaObject && javaCameraId){
		cameraId = env->CallIntMethod(javaObject,javaCameraId);
		instances[cameraId]=this;
	}else{
		ofLog(OF_LOG_ERROR, "cannot get OFAndroidVideoGrabber camera id");
		return;
	}
	attemptFramerate = 30;
	newPixels = false;
	InitConvertTable();
	bGrabberInited = false;
}

ofxAndroidVideoGrabber::~ofxAndroidVideoGrabber(){
	instances.erase(cameraId);
	releaseJavaObject();
}

void ofxAndroidVideoGrabber::listDevices(){

}

bool ofxAndroidVideoGrabber::isFrameNew(){
	return bIsFrameNew;
}

void ofxAndroidVideoGrabber::update(){
	if(paused){
		//ofLog(OF_LOG_WARNING,"ofVideoGrabber paused cannot grab frame");
		return;
	}
	//ofLog(OF_LOG_NOTICE,"updating camera");
	if(bGrabberInited && newPixels){
		//ofLog(OF_LOG_NOTICE,"new pixels");
		newPixels = false;
		bIsFrameNew = true;
	}else{
		bIsFrameNew = false;
	}
}

void ofxAndroidVideoGrabber::close(){

}

bool ofxAndroidVideoGrabber::initGrabber(int w, int h){
	if(instances[cameraId]!=this){
		ofLog(OF_LOG_ERROR,"ofVideoGrabber: Cannot initialize more than one instance in this version");
		return false;
	}
	if(bGrabberInited){
		ofLog(OF_LOG_ERROR,"ofVideoGrabber: Camera already initialized");
		return false;
	}

	JNIEnv *env = ofGetJNIEnv();
	if(!env) return false;

	jclass javaClass = env->FindClass("cc.openframeworks.OFAndroidVideoGrabber");

	jobject camera = getCamera(env, javaClass, cameraId);
	jmethodID javaInitGrabber = env->GetMethodID(javaClass,"initGrabber","(III)V");
	if(camera && javaInitGrabber){
		env->CallVoidMethod(camera,javaInitGrabber,w,h,attemptFramerate);
	}else{
		ofLog(OF_LOG_ERROR, "cannot get OFAndroidVideoGrabber init grabber method");
		return false;
	}

	//ofLog(OF_LOG_NOTICE,"new frame callback size: " + ofToString((int)width) + "," + ofToString((int)height));
	pixels.allocate(w,h,OF_IMAGE_COLOR);
	pixels.set(0);
	bGrabberInited = true;

	ofLog(OF_LOG_NOTICE,"ofVideoGrabber: Camera initialized correctly");
	return true;
}

void ofxAndroidVideoGrabber::videoSettings(){

}

unsigned char * ofxAndroidVideoGrabber::getPixels(){
	return pixels.getPixels();
}

void ofxAndroidVideoGrabber::setVerbose(bool bTalkToMe){

}

void ofxAndroidVideoGrabber::setDeviceID(int _deviceID){

}

void ofxAndroidVideoGrabber::setDesiredFrameRate(int framerate){
	attemptFramerate = framerate;
}

float ofxAndroidVideoGrabber::getHeight(){
	return pixels.getHeight();
}

float ofxAndroidVideoGrabber::getWidth(){
	return pixels.getWidth();
}




// Conversion from yuv nv21 to rgb24 adapted from
// videonet conversion from YUV420 to RGB24
// http://www.codeguru.com/cpp/g-m/multimedia/video/article.php/c7621
 static long int crv_tab[256];
 static long int cbu_tab[256];
 static long int cgu_tab[256];
 static long int cgv_tab[256];
 static long int tab_76309[256];
 static unsigned char clp[1024];         //for clip in CCIR601
 //
 //Initialize conversion table for YUV420 to RGB
 //
 void InitConvertTable()
 {
    long int crv,cbu,cgu,cgv;
    int i,ind;

    crv = 104597; cbu = 132201;  /* fra matrise i global.h */
    cgu = 25675;  cgv = 53279;

    for (i = 0; i < 256; i++) {
       crv_tab[i] = (i-128) * crv;
       cbu_tab[i] = (i-128) * cbu;
       cgu_tab[i] = (i-128) * cgu;
       cgv_tab[i] = (i-128) * cgv;
       tab_76309[i] = 76309*(i-16);
    }

    for (i=0; i<384; i++)
       clp[i] =0;
    ind=384;
    for (i=0;i<256; i++)
        clp[ind++]=i;
    ind=640;
    for (i=0;i<384;i++)
        clp[ind++]=255;
 }

 void ConvertYUV2RGB(unsigned char *src0,unsigned char *src1,unsigned char *dst_ori,
                                  int width,int height)
 {
     register int y1,y2,u,v;
     register unsigned char *py1,*py2;
     register int i,j, c1, c2, c3, c4;
     register unsigned char *d1, *d2;

     int width3 = 3*width;
     py1=src0;
     py2=py1+width;
     d1=dst_ori;
     d2=d1+width3;
     for (j = 0; j < height; j += 2) {
         for (i = 0; i < width; i += 2) {

             v = *src1++;
             u = *src1++;

             c1 = crv_tab[v];
             c2 = cgu_tab[u];
             c3 = cgv_tab[v];
             c4 = cbu_tab[u];

             //up-left
             y1 = tab_76309[*py1++];
             *d1++ = clp[384+((y1 + c1)>>16)];
             *d1++ = clp[384+((y1 - c2 - c3)>>16)];
             *d1++ = clp[384+((y1 + c4)>>16)];

             //down-left
             y2 = tab_76309[*py2++];
             *d2++ = clp[384+((y2 + c1)>>16)];
             *d2++ = clp[384+((y2 - c2 - c3)>>16)];
             *d2++ = clp[384+((y2 + c4)>>16)];

             //up-right
             y1 = tab_76309[*py1++];
             *d1++ = clp[384+((y1 + c1)>>16)];
             *d1++ = clp[384+((y1 - c2 - c3)>>16)];
             *d1++ = clp[384+((y1 + c4)>>16)];

             //down-right
             y2 = tab_76309[*py2++];
             *d2++ = clp[384+((y2 + c1)>>16)];
             *d2++ = clp[384+((y2 - c2 - c3)>>16)];
             *d2++ = clp[384+((y2 + c4)>>16)];
         }
         d1 += width3;
         d2 += width3;
         py1+=   width;
         py2+=   width;
     }


}


 /*static int time_one_frame = 0;
 static int acc_time = 0;
 static int num_frames = 0;
 static int time_prev_out = 0;*/

extern "C"{
jint
Java_cc_openframeworks_OFAndroidVideoGrabber_newFrame(JNIEnv*  env, jobject  thiz, jbyteArray array, jint width, jint height){
	//ofLog(OF_LOG_NOTICE,"new pixels callback");
	if(ofGetAppPtr()!=NULL){
		//ofLog(OF_LOG_NOTICE,"gettings pixels app not null");
		buffer = (unsigned char*)env->GetPrimitiveArrayCritical(array, NULL);
		if(!buffer) return 1;

		//time_one_frame = ofGetSystemTime();
		ConvertYUV2RGB(buffer, 																 // y component
					   buffer+int(instances[cameraId]->getWidth()*instances[cameraId]->getHeight()),		 // uv components
				       instances[cameraId]->getPixels(),instances[cameraId]->getWidth(),instances[cameraId]->getHeight());

		/*acc_time += ofGetSystemTime() - time_one_frame;
		num_frames ++;
		if(ofGetSystemTime() - time_prev_out > 5000){
			time_prev_out = ofGetSystemTime();
			ofLog(OF_LOG_NOTICE,"avg time: %f" , float(acc_time)/float(num_frames));
		}*/

		//dc1394_YUV411_to_RGB8(buffer, rgb_buffer, width, height);
		/*android_color rgb24;
		for(int i=0;i<width*height;i++){
			//ofLog(OF_LOG_NOTICE,"reading pixel: " + ofToString(i));
			rgb16_to_rgb24(((short*)buffer)[i],rgb24);
			rgb_buffer[i*3]   = rgb24.r;
			rgb_buffer[i*3+1] = rgb24.g;
			rgb_buffer[i*3+2] = rgb24.b;
		}*/
		//memcpy(rgb_buffer,buffer,width*height*3);
		env->ReleasePrimitiveArrayCritical(array,buffer,0);
		newPixels = true;
		//((ofxAndroidApp*)ofGetAppPtr())->imageReceived(pixels,width,height);
		return 0;
	}
	return 1;
}
}
