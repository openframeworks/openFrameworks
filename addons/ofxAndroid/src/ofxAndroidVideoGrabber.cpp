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
#include "ofVideoGrabber.h"

static int cameraId;
static bool newPixels;
static unsigned char * buffer = 0;


static map<int,ofVideoGrabber*> instances;
static bool paused=true;



static jclass getJavaClass(){
	JNIEnv *env = ofGetJNIEnv();

	jclass javaClass = env->FindClass("cc/openframeworks/OFAndroidVideoGrabber");

	if(javaClass==NULL){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidVideoGrabber java class");
	}

	return javaClass;
}

bool ofxAndroidInitGrabber(ofVideoGrabber * grabber){
	if(instances.size()>0){
		ofLog(OF_LOG_ERROR,"ofVideoGrabber: This version is limited to 1 camera at the same time");
		return false;
	}

	JNIEnv *env = ofGetJNIEnv();

	jclass javaClass = getJavaClass();
	if(!javaClass) return false;

	jmethodID videoGrabberConstructor = env->GetMethodID(javaClass,"<init>","()V");
	if(!videoGrabberConstructor){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidVideoGrabber constructor");
		return false;
	}

	jobject javaObject = env->NewObject(javaClass,videoGrabberConstructor);
	jmethodID javaCameraId = env->GetMethodID(javaClass,"getId","()I");
	if(javaObject && javaCameraId){
		cameraId = env->CallIntMethod(javaObject,javaCameraId);
		instances[cameraId]=grabber;
	}else{
		ofLog(OF_LOG_ERROR, "cannot get OFAndroidVideoGrabber camera id");
		return false;
	}
	return true;
}

bool ofxAndroidCloseGrabber(ofVideoGrabber * grabber){
	instances.erase(cameraId);
	return true;
}

void ofPauseVideoGrabbers(){
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
}

static void InitConvertTable();

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

ofxAndroidVideoGrabber::ofxAndroidVideoGrabber(){

	attemptFramerate = -1;
	newPixels = false;
	InitConvertTable();
	bGrabberInited = false;
	internalPixelFormat = OF_PIXELS_RGB;
}

ofxAndroidVideoGrabber::~ofxAndroidVideoGrabber(){
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
	if(instances.find(cameraId)==instances.end()){
		ofLog(OF_LOG_ERROR,"ofVideoGrabber: Cannot initialize more than one instance in this version");
		return false;
	}
	if(bGrabberInited){
		ofLog(OF_LOG_ERROR,"ofVideoGrabber: Camera already initialized");
		return false;
	}

	JNIEnv *env = ofGetJNIEnv();
	if(!env) return false;

	jclass javaClass = env->FindClass("cc/openframeworks/OFAndroidVideoGrabber");

	jobject camera = getCamera(env, javaClass, cameraId);
	jmethodID javaInitGrabber = env->GetMethodID(javaClass,"initGrabber","(III)V");
	if(camera && javaInitGrabber){
		env->CallVoidMethod(camera,javaInitGrabber,w,h,attemptFramerate);
	}else{
		ofLog(OF_LOG_ERROR, "cannot get OFAndroidVideoGrabber init grabber method");
		return false;
	}

	//ofLog(OF_LOG_NOTICE,"new frame callback size: " + ofToString((int)width) + "," + ofToString((int)height));
	pixels.allocate(w,h,getPixelFormat());
	bGrabberInited = true;

	ofLog(OF_LOG_NOTICE,"ofVideoGrabber: Camera initialized correctly");
	return true;
}

void ofxAndroidVideoGrabber::videoSettings(){

}

unsigned char * ofxAndroidVideoGrabber::getPixels(){
	return pixels.getPixels();
}

ofPixelsRef	ofxAndroidVideoGrabber::getPixelsRef(){
	return pixels;
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

void ofxAndroidVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat){
	internalPixelFormat = pixelFormat;
}

ofPixelFormat ofxAndroidVideoGrabber::getPixelFormat(){
	return internalPixelFormat;
}

ofPixelsRef ofxAndroidVideoGrabber::getAuxBuffer(){
	return auxBuffer;
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
/**
 * Converts semi-planar YUV420 as generated for camera preview into RGB565
 * format for use as an OpenGL ES texture. It assumes that both the input
 * and output data are contiguous and start at zero.
 *
 * @param yuvs the array of YUV420 semi-planar data
 * @param rgbs an array into which the RGB565 data will be written
 * @param width the number of pixels horizontally
 * @param height the number of pixels vertically
 */

//we tackle the conversion two pixels at a time for greater speed
void ConvertYUV2toRGB565(unsigned char* yuvs, unsigned char* rgbs, int width, int height) {
    //the end of the luminance data
    int lumEnd = width * height;
    //points to the next luminance value pair
    int lumPtr = 0;
    //points to the next chromiance value pair
    int chrPtr = lumEnd;
    //points to the next byte output pair of RGB565 value
    int outPtr = 0;
    //the end of the current luminance scanline
    int lineEnd = width;
    register int R, G, B;
    register int Y1;
    register int Y2;
    register int Cr;
    register int Cb;

    while (true) {

        //skip back to the start of the chromiance values when necessary
        if (lumPtr == lineEnd) {
            if (lumPtr == lumEnd) break; //we've reached the end
            //division here is a bit expensive, but's only done once per scanline
            chrPtr = lumEnd + ((lumPtr  >> 1) / width) * width;
            lineEnd += width;
        }

        //read the luminance and chromiance values
        Y1 = yuvs[lumPtr++] & 0xff;
        Y2 = yuvs[lumPtr++] & 0xff;
        Cr = (yuvs[chrPtr++] & 0xff) - 128;
        Cb = (yuvs[chrPtr++] & 0xff) - 128;

        //generate first RGB components
        B = Y1 + ((454 * Cb) >> 8);
        if(B < 0) B = 0; else if(B > 255) B = 255;
        G = Y1 - ((88 * Cb + 183 * Cr) >> 8);
        if(G < 0) G = 0; else if(G > 255) G = 255;
        R = Y1 + ((359 * Cr) >> 8);
        if(R < 0) R = 0; else if(R > 255) R = 255;
        //NOTE: this assume little-endian encoding
        rgbs[outPtr++]  = (unsigned char) (((G & 0x3c) << 3) | (B >> 3));
        rgbs[outPtr++]  = (unsigned char) ((R & 0xf8) | (G >> 5));

        //generate second RGB components
        B = Y2 + ((454 * Cb) >> 8);
        if(B < 0) B = 0; else if(B > 255) B = 255;
        G = Y2 - ((88 * Cb + 183 * Cr) >> 8);
        if(G < 0) G = 0; else if(G > 255) G = 255;
        R = Y2 + ((359 * Cr) >> 8);
        if(R < 0) R = 0; else if(R > 255) R = 255;
        //NOTE: this assume little-endian encoding
        rgbs[outPtr++]  = (unsigned char) (((G & 0x3c) << 3) | (B >> 3));
        rgbs[outPtr++]  = (unsigned char) ((R & 0xf8) | (G >> 5));
    }
}
void ConvertYUV2toRGB565_2(unsigned char *src0,unsigned char *src1,unsigned char *dst_ori,
                                 int width,int height)
{
    register int y1,y2,u,v;
    register unsigned char *py1,*py2;
    register int i,j, c1, c2, c3, c4;
    register unsigned char *d1, *d2;
    register int R,G,B;
    int width2 = 2*width;
    py1=src0;
    py2=py1+width;
    d1=dst_ori;
    d2=d1+width2;
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
            R = clp[384+((y1 + c1)>>16)];
            G = clp[384+((y1 - c2 - c3)>>16)];
            B = clp[384+((y1 + c4)>>16)];
            *d1++ = (unsigned char) (((G & 0x3c) << 3) | (B >> 3));
            *d1++ = (unsigned char) ((R & 0xf8) | (G >> 5));

            //down-left
            y2 = tab_76309[*py2++];
            B = clp[384+((y2 + c1)>>16)];
            G = clp[384+((y2 - c2 - c3)>>16)];
            R = clp[384+((y2 + c4)>>16)];
            *d2++ = (unsigned char) (((G & 0x3c) << 3) | (B >> 3));
            *d2++ = (unsigned char) ((R & 0xf8) | (G >> 5));

            //up-right
            y1 = tab_76309[*py1++];
            R = clp[384+((y1 + c1)>>16)];
            G = clp[384+((y1 - c2 - c3)>>16)];
            B = clp[384+((y1 + c4)>>16)];
            *d1++ = (unsigned char) (((G & 0x3c) << 3) | (B >> 3));
            *d1++ = (unsigned char) ((R & 0xf8) | (G >> 5));

            //down-right
            y2 = tab_76309[*py2++];
            B = clp[384+((y2 + c1)>>16)];
            G = clp[384+((y2 - c2 - c3)>>16)];
            R = clp[384+((y2 + c4)>>16)];
            *d2++ = (unsigned char) (((G & 0x3c) << 3) | (B >> 3));
            *d2++ = (unsigned char) ((R & 0xf8) | (G >> 5));
        }
        d1 += width2;
        d2 += width2;
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

		//static ofPixels aux_buffer;
		ofxAndroidVideoGrabber* grabber = (ofxAndroidVideoGrabber*)instances[cameraId]->getGrabber().get();

		unsigned char * dst = instances[cameraId]->getPixels();
		if(int(instances[cameraId]->getWidth())!=width || int(instances[cameraId]->getHeight())!=height){
			if(instances[cameraId]->getPixelFormat()!=OF_PIXELS_MONO){
				grabber->getAuxBuffer().allocate(width,height,instances[cameraId]->getPixelFormat());
				dst = grabber->getAuxBuffer().getPixels();
			}else{
				grabber->getAuxBuffer().setFromExternalPixels(buffer,width,height,OF_IMAGE_GRAYSCALE);
			}
		}



		//time_one_frame = ofGetSystemTime();
		if(instances[cameraId]->getPixelFormat()==OF_PIXELS_RGB){
			ConvertYUV2RGB(buffer, 					// y component
					   buffer+(width*height),		// uv components
				       dst,width,height);
		}else if(instances[cameraId]->getPixelFormat()==OF_PIXELS_RGB565){
			ConvertYUV2toRGB565(buffer,dst,width,height);
		}else if(instances[cameraId]->getPixelFormat()==OF_PIXELS_MONO && int(instances[cameraId]->getWidth())==width && int(instances[cameraId]->getHeight())==height){
			memcpy(dst,buffer,(width*height));
		}

		if(int(instances[cameraId]->getWidth())!=width || int(instances[cameraId]->getHeight())!=height){
			grabber->getAuxBuffer().resizeTo(instances[cameraId]->getPixelsRef());
		}
		/*acc_time += ofGetSystemTime() - time_one_frame;
		num_frames ++;
		if(ofGetSystemTime() - time_prev_out > 5000){
			time_prev_out = ofGetSystemTime();
			ofLog(OF_LOG_NOTICE,"avg time: %f" , float(acc_time)/float(num_frames));
		}*/

		env->ReleasePrimitiveArrayCritical(array,buffer,0);
		newPixels = true;
		ofNotifyEvent(grabber->newFrameE,instances[cameraId]->getPixelsRef());
		//((ofxAndroidApp*)ofGetAppPtr())->imageReceived(pixels,width,height);
		return 0;
	}
	return 1;
}
}
