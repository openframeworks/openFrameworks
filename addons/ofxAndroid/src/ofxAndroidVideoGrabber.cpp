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
#include "ofGLUtils.h"
#include "ofMatrix4x4.h"

using namespace std;

struct ofxAndroidVideoGrabber::Data{
	bool bIsFrameNew;
	bool bGrabberInited;
	bool bUsePixels;
	int width;
	int height;
	ofPixelFormat internalPixelFormat;
	bool bNewBackFrame;
	ofPixels frontBuffer, backBuffer, resizeBuffer;
	ofTexture texture;
	jfloatArray matrixJava;
	int cameraId;
	bool appPaused;
	bool newPixels;
	int attemptFramerate;
	jobject javaVideoGrabber;

	Data();
	~Data();
	void onAppPause();
	void onAppResume();
	void loadTexture();
	void update();
};

map<int,weak_ptr<ofxAndroidVideoGrabber::Data>> & instances(){
	static auto * instances = new map<int,weak_ptr<ofxAndroidVideoGrabber::Data>>;
	return *instances;
}

static jclass getJavaClass(){
	JNIEnv *env = ofGetJNIEnv();

	jclass javaClass = env->FindClass("cc/openframeworks/OFAndroidVideoGrabber");

	if(javaClass==NULL){
		ofLogError("ofxAndroidVideoGrabber") << "couldn't find OFAndroidVideoGrabber java class";
	}

	return javaClass;
}

static void InitConvertTable();
static void ConvertYUV2RGB(unsigned char *src0,unsigned char *src1,unsigned char *dst_ori,
					int width,int height);
static void ConvertYUV2toRGB565(unsigned char* yuvs, unsigned char* rgbs, int width, int height);

ofxAndroidVideoGrabber::Data::Data()
:bIsFrameNew(false)
,bGrabberInited(false)
,cameraId(-1)
,appPaused(false)
,newPixels(false)
,attemptFramerate(-1)
,bUsePixels(true)
,javaVideoGrabber(nullptr){
	JNIEnv *env = ofGetJNIEnv();

	jclass javaClass = getJavaClass();
	if(!javaClass) return;

	jmethodID videoGrabberConstructor = env->GetMethodID(javaClass,"<init>","()V");
	if(!videoGrabberConstructor){
		ofLogError("ofxAndroidVideoGrabber") << "initGrabber(): couldn't find OFAndroidVideoGrabber constructor";
		return;
	}

	javaVideoGrabber = env->NewObject(javaClass,videoGrabberConstructor);
	javaVideoGrabber = (jobject)env->NewGlobalRef(javaVideoGrabber);
	jmethodID javaCameraId = env->GetMethodID(javaClass,"getId","()I");
	if(javaVideoGrabber && javaCameraId){
		cameraId = env->CallIntMethod(javaVideoGrabber,javaCameraId);
	}else{
		ofLogError("ofxAndroidVideoGrabber") << "initGrabber(): couldn't get OFAndroidVideoGrabber camera id";
	}
	InitConvertTable();
	internalPixelFormat = OF_PIXELS_RGB;

	jfloatArray localMatrixJava = ofGetJNIEnv()->NewFloatArray(16);
	matrixJava = (jfloatArray) ofGetJNIEnv()->NewGlobalRef(localMatrixJava);
	ofAddListener(ofxAndroidEvents().unloadGL,this,&ofxAndroidVideoGrabber::Data::onAppPause);
	ofAddListener(ofxAndroidEvents().reloadGL,this,&ofxAndroidVideoGrabber::Data::onAppResume);
}

void ofxAndroidVideoGrabber::Data::update(){
	JNIEnv *env = ofGetJNIEnv();
	jmethodID getTextureMatrix = env->GetMethodID(getJavaClass(), "getTextureMatrix", "([F)V");
	env->CallVoidMethod(javaVideoGrabber, getTextureMatrix, matrixJava);
	jfloat* cfloats = env->GetFloatArrayElements(matrixJava, 0);
	ofMatrix4x4 mat(cfloats);
	if(mat(0,0) == -1){
		mat.scale(-1,1,1);
		mat.translate(1,0,0);
	}
	if(mat(1,1) == -1){
		mat.scale(1,-1,1);
		mat.translate(0,1,0);
	}
	texture.setTextureMatrix(mat);
}

ofxAndroidVideoGrabber::Data::~Data(){
	JNIEnv *env = ofGetJNIEnv();
	jclass javaClass = getJavaClass();
	if(!javaClass) return;
	jmethodID javaOFDestructor = env->GetMethodID(javaClass,"release","()V");
	if(javaVideoGrabber && javaOFDestructor){
		env->CallVoidMethod(javaVideoGrabber,javaOFDestructor);
		env->DeleteGlobalRef(javaVideoGrabber);
	}
	if(matrixJava) ofGetJNIEnv()->DeleteGlobalRef(matrixJava);
	ofRemoveListener(ofxAndroidEvents().unloadGL,this,&ofxAndroidVideoGrabber::Data::onAppPause);
	ofRemoveListener(ofxAndroidEvents().reloadGL,this,&ofxAndroidVideoGrabber::Data::onAppResume);
}

void ofxAndroidVideoGrabber::Data::loadTexture(){
	ofTextureData td;
	GLuint texId[1];
	glGenTextures(1, texId);

	glEnable(GL_TEXTURE_EXTERNAL_OES);
	glBindTexture(GL_TEXTURE_EXTERNAL_OES, texId[0]);

	glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (!ofIsGLProgrammableRenderer()) {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

	glDisable(GL_TEXTURE_EXTERNAL_OES);

	// Set the externally created texture reference
	texture.setUseExternalTextureID(texId[0]);
	texture.texData.width = width;
	texture.texData.height = height;
	texture.texData.tex_w = width;
	texture.texData.tex_h = height;
	texture.texData.tex_t = 1; // Hack!
	texture.texData.tex_u = 1;
	texture.texData.textureTarget = GL_TEXTURE_EXTERNAL_OES;
	texture.texData.glInternalFormat = GL_RGBA;


}


ofxAndroidVideoGrabber::ofxAndroidVideoGrabber()
:data(new Data){
	if(data->cameraId!=-1){
		instances().insert(std::pair<int,weak_ptr<ofxAndroidVideoGrabber::Data>>(data->cameraId,data));
	}
}

ofxAndroidVideoGrabber::~ofxAndroidVideoGrabber(){
}

void ofxAndroidVideoGrabber::Data::onAppPause(){
	appPaused = true;
	glDeleteTextures(1, &texture.texData.textureID);
	texture.texData.textureID = 0;
	ofLogVerbose("ofxAndroidVideoGrabber") << "ofPauseVideoGrabbers(): releasing textures";
}

void ofxAndroidVideoGrabber::Data::onAppResume(){
    if(!ofxAndroidCheckPermission(OFX_ANDROID_PERMISSION_CAMERA)) return;

    ofLogVerbose("ofxAndroidVideoGrabber") << "ofResumeVideoGrabbers(): trying to allocate textures";
	JNIEnv *env = ofGetJNIEnv();
	if(!env){
		ofLogError("ofxAndroidVideoGrabber") << "init grabber failed : couldn't get environment using GetEnv()";
		return;
	}
	jclass javaClass = getJavaClass();
	jmethodID javaInitGrabber = env->GetMethodID(javaClass,"initGrabber","(IIII)V");
	loadTexture();

	int texID= texture.texData.textureID;
	int w=width;
	int h=height;
	env->CallVoidMethod(javaVideoGrabber,javaInitGrabber,w,h,attemptFramerate,texID);
	ofLogVerbose("ofxAndroidVideoGrabber") << "ofResumeVideoGrabbers(): textures allocated";
	bGrabberInited = true;
	appPaused = false;
}
vector<ofVideoDevice> ofxAndroidVideoGrabber::listDevices() const{

	vector<ofVideoDevice> devices;

	int numDevices = getNumCameras();
	for(int i = 0; i < numDevices; i++){
		int facing = getFacingOfCamera(i);
		ofVideoDevice vd;
		vd.deviceName = facing == 0? "Back" : "Front";
		vd.id = i;
		vd.bAvailable = true;
		devices.push_back(vd);
	}

	return devices;
}

bool ofxAndroidVideoGrabber::isFrameNew() const{
	return data->bIsFrameNew;
}

void ofxAndroidVideoGrabber::update(){
	if(data->appPaused){
		//ofLogWarning("ofxAndroidVideoGrabber") << "update(): couldn't grab frame, movie is paused";
		return;
	}
	//ofLogVerbose("ofxAndroidVideoGrabber") << "update(): updating camera";
	if(data->bGrabberInited && data->newPixels){
		//ofLogVerbose("ofxAndroidVideoGrabber") << "update(): new pixels";
		data->newPixels = false;
		data->bIsFrameNew = true;

		if (data->bNewBackFrame && data->bUsePixels) {
			//std::unique_lock <std::mutex> lck(data->mtx);
			std::swap(data->backBuffer, data->frontBuffer);
			data->bNewBackFrame = false;
		}

		// Call update in the java code
		// This will tell the camera api that we are ready for a new frame
		jmethodID update = ofGetJNIEnv()->GetMethodID(getJavaClass(), "update", "()V");
		ofGetJNIEnv()->CallVoidMethod(data->javaVideoGrabber, update);
		data->update();
	} else {
		data->bIsFrameNew = false;
	}
}

void ofxAndroidVideoGrabber::close(){
	// Release texture
	glDeleteTextures(1, &data->texture.texData.textureID);

    JNIEnv *env = ofGetJNIEnv();
    jclass javaClass = getJavaClass();
    jmethodID javaCloseGrabber = env->GetMethodID(javaClass,"close","()V");
    if(data->javaVideoGrabber && javaCloseGrabber){
        env->CallVoidMethod(data->javaVideoGrabber,javaCloseGrabber);
    } else {
        ofLogError("ofxAndroidVideoGrabber") << "close(): couldn't get OFAndroidVideoGrabber close grabber method";
    }
    
    data->bGrabberInited = false;
}


ofTexture *	ofxAndroidVideoGrabber::getTexturePtr(){
	if(supportsTextureRendering()) return &data->texture;
	else return nullptr;
}

bool ofxAndroidVideoGrabber::supportsTextureRendering(){
	static bool supportsTexture = false;
	static bool supportChecked = false;
	if(!supportChecked){
		JNIEnv *env = ofGetJNIEnv();
		if(!env){
			ofLogError("ofxAndroidVideoGrabber") << "init grabber failed :  couldn't get environment using GetEnv()";
			return false;
		}

		jmethodID supportsTextureMethod = env->GetStaticMethodID(getJavaClass(),"supportsTextureRendering","()Z");
		supportsTexture = env->CallStaticBooleanMethod(getJavaClass(),supportsTextureMethod);
		supportChecked = true;
	}
	return supportsTexture;
}

bool ofxAndroidVideoGrabber::setup(int w, int h){
	if(data->bGrabberInited){
		ofLogError("ofxAndroidVideoGrabber") << "initGrabber(): camera already initialized";
		return false;
	}

    ofxAndroidRequestPermission(OFX_ANDROID_PERMISSION_CAMERA);
    if(!ofxAndroidCheckPermission(OFX_ANDROID_PERMISSION_CAMERA)) return false;

    data->width = w;
    data->height = h;
    data->frontBuffer.allocate(w, h, data->internalPixelFormat);
    data->backBuffer.allocate(w, h, data->internalPixelFormat);

	ofLogNotice() << "initializing camera with external texture";

	data->loadTexture();

	bool bInit = initCamera();
	if(!bInit) return false;

	ofLogVerbose("ofxAndroidVideoGrabber") << "initGrabber(): camera initialized correctly";
	data->appPaused = false;
	return true;
}

bool ofxAndroidVideoGrabber::initCamera(){
    if(!ofxAndroidCheckPermission(OFX_ANDROID_PERMISSION_CAMERA)) return false;

    JNIEnv *env = ofGetJNIEnv();
	if(!env) return false;

	jclass javaClass = getJavaClass();

	jmethodID javaInitGrabber = env->GetMethodID(javaClass,"initGrabber","(IIII)V");
	if(data->javaVideoGrabber && javaInitGrabber){
		env->CallVoidMethod(data->javaVideoGrabber,javaInitGrabber,data->width,data->height,data->attemptFramerate,data->texture.texData.textureID);
	} else {
		ofLogError("ofxAndroidVideoGrabber") << "initGrabber(): couldn't get OFAndroidVideoGrabber init grabber method";
		return false;
	}

	//ofLogVerbose("ofxAndroidVideoGrabber") << "initGrabber(): new frame callback size: " << (int) width << "x" << (int) height;
	data->bGrabberInited = true;

	return true;
};

bool ofxAndroidVideoGrabber::isInitialized() const{
	return data->bGrabberInited;
}

void ofxAndroidVideoGrabber::videoSettings(){
}

void ofxAndroidVideoGrabber::setUsePixels(bool usePixels){
	data->bUsePixels = usePixels;
}

ofPixels& ofxAndroidVideoGrabber::getPixels(){
	if(!data->bUsePixels){
		ofLogNotice()<<"Calling getPixels will not return frame data when setUsePixels(false) has been set";
	}

	return data->frontBuffer;
}

const ofPixels& ofxAndroidVideoGrabber::getPixels() const {
	return const_cast<ofxAndroidVideoGrabber*>(this)->getPixels();
}

void ofxAndroidVideoGrabber::setVerbose(bool bTalkToMe){

}

int ofxAndroidVideoGrabber::getCameraFacing(int facing)const{
	JNIEnv *env = ofGetJNIEnv();
	if(!env) return -1;

	jclass javaClass = getJavaClass();

	jmethodID javagetCameraFacing = env->GetMethodID(javaClass,"getCameraFacing","(I)I");
	if(data->javaVideoGrabber && javagetCameraFacing){
		return env->CallIntMethod(data->javaVideoGrabber,javagetCameraFacing,facing);
	} else {
		ofLogError("ofxAndroidVideoGrabber") << "getCameraFacing(): couldn't get OFAndroidVideoGrabber getCameraFacing method";
		return -1;
	}
}

int ofxAndroidVideoGrabber::getBackCamera()const{
	return getCameraFacing(0);
}

int ofxAndroidVideoGrabber::getFrontCamera()const{
	return getCameraFacing(1);
}

int ofxAndroidVideoGrabber::getNumCameras() const{
	JNIEnv *env = ofGetJNIEnv();
	if(!env) return 0;

	jclass javaClass = getJavaClass();

	jmethodID javagetNumCameras= env->GetMethodID(javaClass,"getNumCameras","()I");
	if(data->javaVideoGrabber && javagetNumCameras){
		return env->CallIntMethod(data->javaVideoGrabber,javagetNumCameras);
	} else {
		ofLogError("ofxAndroidVideoGrabber") << "getNumCameras(): couldn't get OFAndroidVideoGrabber getNumCameras method";
		return 0;
	}
}

int ofxAndroidVideoGrabber::getCameraOrientation(int device)const{
	JNIEnv *env = ofGetJNIEnv();
	if(!env) return 0;

	jclass javaClass = getJavaClass();

	jmethodID javagetCameraOrientation= env->GetMethodID(javaClass,"getCameraOrientation","(I)I");
	if(data->javaVideoGrabber && javagetCameraOrientation){
		return env->CallIntMethod(data->javaVideoGrabber,javagetCameraOrientation, device);
	} else {
		ofLogError("ofxAndroidVideoGrabber") << "getCameraOrientation(): couldn't get OFAndroidVideoGrabber getCameraOrientation method";
		return 0;
	}
}

int ofxAndroidVideoGrabber::getFacingOfCamera(int device)const{
	JNIEnv *env = ofGetJNIEnv();
	if(!env) return 0;

	jclass javaClass = getJavaClass();

	jmethodID javagetFacingOfCamera= env->GetMethodID(javaClass,"getFacingOfCamera","(I)I");
	if(data->javaVideoGrabber && javagetFacingOfCamera){
		return env->CallIntMethod(data->javaVideoGrabber,javagetFacingOfCamera, device);
	} else {
		ofLogError("ofxAndroidVideoGrabber") << "getFacingOfCamera(): couldn't get OFAndroidVideoGrabber getFacingOfCamera method";
		return 0;
	}
}

void ofxAndroidVideoGrabber::setDeviceID(int _deviceID){
	bool wasInited = data->bGrabberInited;
	int w = this->getWidth();
	int h = this->getHeight();
	if(data->bGrabberInited){
		close();
	}

	JNIEnv *env = ofGetJNIEnv();
	if(!env) return;

	jclass javaClass = getJavaClass();

	jmethodID javasetDeviceID = env->GetMethodID(javaClass,"setDeviceID","(I)V");
	if(data->javaVideoGrabber && javasetDeviceID){
		env->CallVoidMethod(data->javaVideoGrabber,javasetDeviceID,_deviceID);
	} else {
		ofLogError("ofxAndroidVideoGrabber") << "setDeviceID(): couldn't get OFAndroidVideoGrabber setDeviceID method";
		return;
	}

	if(wasInited){
		setup(w, h);
	}
}

bool ofxAndroidVideoGrabber::setAutoFocus(bool autofocus){
	JNIEnv *env = ofGetJNIEnv();
	if(!env) return false;

	jclass javaClass = getJavaClass();
	jmethodID javasetAutoFocus = env->GetMethodID(javaClass,"setAutoFocus","(Z)Z");
	if(data->javaVideoGrabber && javasetAutoFocus){
		return env->CallBooleanMethod(data->javaVideoGrabber,javasetAutoFocus,autofocus);
	}else{
		ofLogError("ofxAndroidVideoGrabber") << "setAutoFocus(): couldn't get OFAndroidVideoGrabber setAutoFocus method";
		return false;
	}
}

void ofxAndroidVideoGrabber::setDesiredFrameRate(int framerate){
	data->attemptFramerate = framerate;
}

float ofxAndroidVideoGrabber::getHeight() const{
	return data->height;
}

float ofxAndroidVideoGrabber::getWidth() const{
	return data->width;
}

bool ofxAndroidVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat){
	data->internalPixelFormat = pixelFormat;
	return true;
}

ofPixelFormat ofxAndroidVideoGrabber::getPixelFormat() const{
	return data->internalPixelFormat;
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
	static bool inited = false;
	if(inited) return;
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

	inited = true;
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


extern "C"{
jint
Java_cc_openframeworks_OFAndroidVideoGrabber_newFrame(JNIEnv*  env, jobject  thiz, jbyteArray array, jint width, jint height, jint cameraId){
	auto data = instances()[cameraId].lock();
	if(!data) return 1;

	if(data->bUsePixels) {
		jboolean isCopy;
		auto currentFrame = (unsigned char *) env->GetByteArrayElements(array, &isCopy);
		//ofLog()<<"Is copy: "<<(isCopy?true:false);

		if (!currentFrame) return 1;

		ofPixels &pixels = data->backBuffer;
		bool needsResize = false;
		if (pixels.getWidth() != width || pixels.getHeight() != height) {
			needsResize = true;
            pixels = data->resizeBuffer;
            if(!pixels.isAllocated() || pixels.getWidth() != width || pixels.getHeight() != height) {
                pixels.allocate(width, height, data->internalPixelFormat);
            }
		}

		if (data->internalPixelFormat == OF_PIXELS_RGB) {
			ConvertYUV2RGB(currentFrame,                    // y component
						   currentFrame + (width * height),            // uv components
						   pixels.getData(), width, height);
		} else if (data->internalPixelFormat == OF_PIXELS_RGB565) {
			ConvertYUV2toRGB565(currentFrame, pixels.getData(), width, height);
		} else if (data->internalPixelFormat == OF_PIXELS_GRAY) {
			pixels.setFromPixels(currentFrame, width, height, OF_IMAGE_GRAYSCALE);
		} else if (data->internalPixelFormat == OF_PIXELS_NV21) {
            pixels.setFromPixels(currentFrame, width, height, OF_PIXELS_NV21);
        }

		env->ReleaseByteArrayElements(array, (jbyte*)currentFrame, 0);

		if (needsResize) {
			pixels.resizeTo(data->backBuffer, OF_INTERPOLATE_NEAREST_NEIGHBOR);
		}

		data->bNewBackFrame=true;
	}

	data->newPixels = true;

	return 0;
}
}
