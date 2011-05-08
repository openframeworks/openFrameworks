#pragma once

#include <set>

#include "ofFileUtils.h"
#include "ofTexture.h"
#include "ofPixels.h"
#include "ofBaseTypes.h"
#include "ofConstants.h"
#include "ofGraphics.h"
#include "ofPixelUtils.h"
#include "FreeImage.h"

enum ofImageQualityType {
	OF_IMAGE_QUALITY_BEST,
	OF_IMAGE_QUALITY_HIGH,
	OF_IMAGE_QUALITY_MEDIUM,
	OF_IMAGE_QUALITY_LOW,
	OF_IMAGE_QUALITY_WORST
};

//----------------------------------------------------
// FreeImage based stuff:
bool ofLoadImage(ofPixels & pix, string path);
bool ofLoadImage(ofPixels & pix, const ofBuffer & buffer);

bool ofLoadImage(ofFloatPixels & pix, string path);
bool ofLoadImage(ofFloatPixels & pix, const ofBuffer & buffer);

bool ofLoadImage(ofTexture & tex, string path);
bool ofLoadImage(ofTexture & tex, const ofBuffer & buffer);

void ofSaveImage(ofPixels & pix, string path, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);
void ofSaveImage(ofPixels & pix, ofBuffer & buffer, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

void ofSaveImage(ofFloatPixels & pix, string path, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);
void ofSaveImage(ofFloatPixels & pix, ofBuffer & buffer, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

void 	ofCloseFreeImage();		// when we exit, we shut down ofImage

//----------------------------------------------------
template<typename T>
class ofImage_ : public ofBaseImage{

	public :

		ofImage_();
		virtual ~ofImage_();

		ofImage_(const ofPixels_<T> & pix);
		ofImage_(const ofFile & file);
		ofImage_(const string & filename);

		// alloation / deallocation routines
		void 				allocate(int w, int h, ofImageType type);
		void 				clear();

		// default copy overwriting (for = or std::vector)
		ofImage_(const ofImage_<T>& mom);
		ofImage_<T>& operator= (const ofImage_<T>& mom);

		// copying:
		void 				clone(const ofImage_<T> &mom);

	
		
	
		
		// enable or disable using the texture of this image
		void 				setUseTexture(bool bUse);
		bool				isUsingTexture();

		//for getting a reference to the texture
		ofTexture & getTextureReference();

		// quick texture binding shortcut
		void bind();
		void unbind();

		// file loading / saving
		bool 				loadImage(string fileName);
		bool				loadImage(const ofBuffer & buffer);
		bool 				loadImage(const ofFile & file);
		void 				saveImage(string fileName, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST);
		void 				saveImage(ofBuffer & buffer, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST);
		void 				saveImage(const ofFile & file, ofImageQualityType compressionLevel = OF_IMAGE_QUALITY_BEST);

		//Sosolimited: texture compression and mipmaps
		void				setCompression(ofTexCompression compression);

		// getting the data
		unsigned char * 	getPixels();			// up to you to get this right
		ofPixelsRef		 	getPixelsRef();

		operator ofPixels&();

		ofColor getColor(int x, int y) const;

		// alter the image
		void setColor(int x, int y, ofColor color);
		void 				setFromPixels(const T * pixels, int w, int h, ofImageType type, bool bOrderIsRGB = true);
		void				setFromPixels(const ofPixels_<T> & pixels);
		void 				setImageType(ofImageType type);
		void 				resize(int newWidth, int newHeight);
		void 				grabScreen(int x, int y, int w, int h);		// grab pixels from opengl, using glreadpixels
		// this does an inplace crop. 
		// NOTE: this reallocates memory.
		void				crop(int x, int y, int w, int h); 
		// this does a crop from another image.
		// NOTE: this will reallocate memory if the image types are different, or if the w & h do not
		// equal this images w & h
		void				cropFrom(ofImage_<T> & otherImage, int x, int y, int w, int h);
		// perform rotation of 90 degress clockwise rotation amont times. 
		void				rotate90(int rotation);
		void				mirror(bool vertical, bool horizontal); 
	
	
	
		// if you've altered the pixels (e.g., from getPixels())
		// call update() to see a change (move the pixels to the texture)
		void update();

		//the anchor is the point the image is drawn around.
		//this can be useful if you want to rotate an image around a particular point.
        void				setAnchorPercent(float xPct, float yPct);	//set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        void				setAnchorPoint(float x, float y);				//set the anchor point in pixels
        void				resetAnchor();								//resets the anchor to (0, 0)

		// draw:
		void 				draw(const ofRectangle & r);
		void 				draw(const ofPoint & p, float w, float h);
		void 				draw(float x, float y, float w, float h);
		void 				draw(float x, float y, float z, float w, float h);
		void 				draw(const ofPoint & p);
		void 				draw(float x, float y);
		void 				draw(float x, float y, float z);

		float 				getHeight();
		float 				getWidth();
		bool 				bAllocated() {return pixels.isAllocated();};

		void				reloadTexture();

		int 				width, height, bpp;		// w,h, bits per pixel
		int					type;					// OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA

		ofImage_<T> & operator=(ofPixels_<T> & pixels);
	protected:
	
		void				changeTypeOfPixels(ofPixels_<T> &pix, ofImageType type);
		void				resizePixels(ofPixels_<T> &pix, int newWidth, int newHeight);

		ofPixels_<T>		pixels;
		bool				bUseTexture;
		ofTexture			tex;

#ifdef TARGET_ANDROID
	// android destroys the opengl context on screen orientation change
	// or when the application runs in the background so we need to reload
	// all the textures when the context is created again.
	// keeping a pointer to all the images we can tell them to reload from a static method
	static set<ofImage_<T> *> all_images;

	static void reloadAllImageTextures(){
		set<ofImage_<T> *>::iterator it;
		for(it=all_images.begin(); it!=all_images.end(); it++){
			(*it)->reloadTexture();
		}
	}
#endif
};

typedef ofImage_<unsigned char> ofImage;
typedef ofImage_<float> ofFloatImage;



//-------------------------------------------------------------
//  implementation


//----------------------------------------------------------
// static variable for freeImage initialization:
void ofInitFreeImage(bool deinit=false);

//----------------------------------------------------------
template<typename T>
ofImage_<T>::ofImage_(){

	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	//----------------------- init free image if necessary
	ofInitFreeImage();

#ifdef TARGET_ANDROID
	all_images.insert(this);
#endif
}

//----------------------------------------------------------
template<typename T>
ofImage_<T>::ofImage_(const ofPixels_<T> & pix){
	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	//----------------------- init free image if necessary
	ofInitFreeImage();

#ifdef TARGET_ANDROID
	all_images.insert(this);
#endif

	setFromPixels(pix);
}

template<typename T>
ofImage_<T>::ofImage_(const ofFile & file){
	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	//----------------------- init free image if necessary
	ofInitFreeImage();

#ifdef TARGET_ANDROID
	all_images.insert(this);
#endif

	loadImage(file);
}

template<typename T>
ofImage_<T>::ofImage_(const string & filename){
	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	//----------------------- init free image if necessary
	ofInitFreeImage();

#ifdef TARGET_ANDROID
	all_images.insert(this);
#endif

	loadImage(filename);
}

//----------------------------------------------------------
template<typename T>
ofImage_<T>& ofImage_<T>::operator=(const ofImage_<T>& mom) {
	clone(mom);
	update();
	return *this;
}

//----------------------------------------------------------
template<typename T>
ofImage_<T>::ofImage_(const ofImage_<T>& mom) {
	clear();
	clone(mom);
	update();
}

//----------------------------------------------------------
template<typename T>
ofImage_<T>::~ofImage_(){
	clear();

#ifdef TARGET_ANDROID
	all_images.erase(this);
#endif
}


//----------------------------------------------------------
template<typename T>
void ofImage_<T>::reloadTexture(){
	if (pixels.isAllocated() == true && bUseTexture == true){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
		tex.loadData(pixels.getPixels(), pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}
}

//----------------------------------------------------------
template<typename T>
bool ofImage_<T>::loadImage(const ofFile & file){
	return loadImage(file.getAbsolutePath());
}

//----------------------------------------------------------
template<typename T>
bool ofImage_<T>::loadImage(string fileName){
	bool bLoadedOk = false;
	bLoadedOk = ofLoadImage(pixels, fileName);
	if (bLoadedOk && pixels.isAllocated() && bUseTexture){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}
	if (!bLoadedOk) {
		ofLog(OF_LOG_ERROR, "Couldn't load image from " + fileName);
	}
	update();
	return bLoadedOk;
}

template<typename T>
bool ofImage_<T>::loadImage(const ofBuffer & buffer){
	bool bLoadedOk = false;
	bLoadedOk = ofLoadImage(pixels, buffer);
	if (bLoadedOk && pixels.isAllocated() && bUseTexture){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}
	if (!bLoadedOk) {
		ofLog(OF_LOG_ERROR, "Couldn't load image from buffer.");
	}
	update();
	return bLoadedOk;
}

//----------------------------------------------------------
template<typename T>
void ofImage_<T>::saveImage(string fileName, ofImageQualityType qualityLevel){
	ofSaveImage(pixels, fileName, qualityLevel);
}

//----------------------------------------------------------
template<typename T>
void ofImage_<T>::saveImage(ofBuffer & buffer, ofImageQualityType qualityLevel){
	ofSaveImage(pixels, buffer, qualityLevel);
}

//----------------------------------------------------------
template<typename T>
void ofImage_<T>::saveImage(const ofFile & file, ofImageQualityType compressionLevel){
	ofSaveImage(pixels,file.getAbsolutePath(),compressionLevel);
}

//we could cap these values - but it might be more useful
//to be able to set anchor points outside the image

//----------------------------------------------------------
template<typename T>
void ofImage_<T>::setAnchorPercent(float xPct, float yPct){
    if (bUseTexture)tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
template<typename T>
void ofImage_<T>::setAnchorPoint(float x, float y){
    if (bUseTexture)tex.setAnchorPoint(x, y);
}

//----------------------------------------------------------
template<typename T>
void ofImage_<T>::resetAnchor(){
   	if (bUseTexture)tex.resetAnchor();
}

//------------------------------------
template<typename T>
void ofImage_<T>::draw(const ofRectangle & _r){
	ofGetDefaultRenderer()->draw(*this,_r.x,_r.y,0,_r.width,_r.height);
}

//------------------------------------
template<typename T>
void ofImage_<T>::draw(const ofPoint & _p, float _w, float _h){
	ofGetDefaultRenderer()->draw(*this,_p.x,_p.y,_p.z,_w,_h);
}

//------------------------------------
template<typename T>
void ofImage_<T>::draw(float _x, float _y, float _w, float _h){
	ofGetDefaultRenderer()->draw(*this,_x,_y,0,_w,_h);
}

//------------------------------------
template<typename T>
void ofImage_<T>::draw(float _x, float _y, float _z, float _w, float _h){
	ofGetDefaultRenderer()->draw(*this,_x,_y,_z,_w,_h);
}

//------------------------------------
template<typename T>
void ofImage_<T>::draw(const ofPoint & p){
	draw(p.x,p.y,p.z,pixels.getWidth(),pixels.getHeight());
}

//------------------------------------
template<typename T>
void ofImage_<T>::draw(float x, float y){
	draw(x,y,0.0f,pixels.getWidth(),pixels.getHeight());
}

//------------------------------------
template<typename T>
void ofImage_<T>::draw(float x, float y, float z){
	draw(x,y,z,pixels.getWidth(),pixels.getHeight());
}

//------------------------------------
template<typename T>
void ofImage_<T>::allocate(int w, int h, ofImageType type){

	pixels.allocate(w, h, type);

	// take care of texture allocation --
	if (pixels.isAllocated() && bUseTexture){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}

	update();
}


//------------------------------------
template<typename T>
void ofImage_<T>::clear(){

	pixels.clear();
	if(bUseTexture)	tex.clear();

	width					= 0;
	height					= 0;
	bpp						= 0;
	type 					= OF_IMAGE_UNDEFINED;
	bUseTexture 			= true;		// the default is, yes, use a texture
}

//------------------------------------
template<typename T>
unsigned char * ofImage_<T>::getPixels(){
	return pixels.getPixels();
}

//----------------------------------------------------------
template<typename T>
ofPixelsRef ofImage_<T>::getPixelsRef(){
	return pixels;
}

//----------------------------------------------------------
template<typename T>
ofImage_<T>::operator ofPixels&(){
	return pixels;
}

//------------------------------------
//for getting a reference to the texture
template<typename T>
ofTexture & ofImage_<T>::getTextureReference(){
	if(!tex.bAllocated() ){
		ofLog(OF_LOG_WARNING, "ofImage - getTextureReference - texture is not allocated");
	}
	return tex;
}

//----------------------------------------------------------
template<typename T>
void ofImage_<T>::bind(){
	if (bUseTexture && tex.bAllocated())
		tex.bind();
}

//----------------------------------------------------------
template<typename T>
void ofImage_<T>::unbind(){
	if (bUseTexture && tex.bAllocated())
		tex.unbind();
}

//------------------------------------
template<typename T>
ofColor ofImage_<T>::getColor(int x, int y) const {
	return pixels.getColor(x, y);
}

//------------------------------------
template<typename T>
void ofImage_<T>::setColor(int x, int y, ofColor color) {
	pixels.setColor(x, y, color);
}

//------------------------------------
template<typename T>
void  ofImage_<T>::setFromPixels(const T * newPixels, int w, int h, ofImageType newType, bool bOrderIsRGB){

	allocate(w, h, newType);
	pixels.setFromPixels(newPixels,w,h,newType);

	if (!bOrderIsRGB){
		pixels.swapRgb();
	}

	update();
}

//------------------------------------
template<typename T>
void ofImage_<T>::setFromPixels(const ofPixels_<T> & pixels){
	setFromPixels(pixels.getPixels(),pixels.getWidth(),pixels.getHeight(),pixels.getImageType());
}

//------------------------------------
template<typename T>
ofImage_<T> & ofImage_<T>::operator=(ofPixels_<T> & pixels){
	setFromPixels(pixels);
	return *this;
}

//------------------------------------
template<typename T>
void ofImage_<T>::update(){

	if (pixels.isAllocated() == true && bUseTexture == true){
		tex.loadData(pixels.getPixels(), pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}

	width	= pixels.getWidth();
	height	= pixels.getHeight();
	bpp		= pixels.getBitsPerPixel();
	type	= pixels.getImageType();
}

//------------------------------------
template<typename T>
void ofImage_<T>::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//------------------------------------
template<typename T>
bool ofImage_<T>::isUsingTexture(){
	return bUseTexture;
}

//------------------------------------
template<typename T>
void ofImage_<T>::grabScreen(int _x, int _y, int _w, int _h){

	allocate(_w, _h, OF_IMAGE_COLOR);

	int screenHeight =	ofGetViewportHeight(); // if we are in a FBO or other viewport, this fails: ofGetHeight();
	_y = screenHeight - _y;
	_y -= _h; // top, bottom issues

	if (!((width == _w) && (height == _h))){
		resize(_w, _h);
	}

	#ifndef TARGET_OPENGLES
		glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );											// be nice to anyone else who might use pixelStore
	#endif
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(_x, _y, _w, _h, pixels.getGlDataType(),GL_UNSIGNED_BYTE, pixels.getPixels());		// read the memory....
	#ifndef TARGET_OPENGLES
		glPopClientAttrib();
	#endif

	int sizeOfOneLineOfPixels = pixels.getWidth() * pixels.getBytesPerPixel();
	unsigned char * tempLineOfPix = new unsigned char[sizeOfOneLineOfPixels];
	unsigned char * linea;
	unsigned char * lineb;
	for (int i = 0; i < pixels.getHeight()/2; i++){
		linea = pixels.getPixels() + i * sizeOfOneLineOfPixels;
		lineb = pixels.getPixels() + (pixels.getHeight()-i-1) * sizeOfOneLineOfPixels;
		memcpy(tempLineOfPix, linea, sizeOfOneLineOfPixels);
		memcpy(linea, lineb, sizeOfOneLineOfPixels);
		memcpy(lineb, tempLineOfPix, sizeOfOneLineOfPixels);
	}
	delete [] tempLineOfPix;
	update();
}


//------------------------------------
template<typename T>
void ofImage_<T>::clone(const ofImage_<T> &mom){

	pixels = mom.pixels;

	tex.clear();
	bUseTexture = mom.bUseTexture;
	if (bUseTexture == true){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}

	update();
}

//------------------------------------
template<typename T>
void ofImage_<T>::setImageType(ofImageType newType){
	changeTypeOfPixels(pixels, newType);
	update();
}

//------------------------------------
template<typename T>
void ofImage_<T>::resize(int newWidth, int newHeight){
	if(newWidth == width && newHeight == height) return;

	resizePixels(pixels, newWidth, newHeight);

	if (bUseTexture == true){
		tex.clear();
		tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}

	update();
}


//------------------------------------
template<typename T>
void ofImage_<T>::crop(int x, int y, int w, int h){
	ofPixelUtils::crop(pixels, x,y,w,h);
	update();
}

//------------------------------------
template<typename T>
void ofImage_<T>::cropFrom(ofImage_<T> & otherImage, int x, int y, int w, int h){

	int myOldWidth = pixels.getWidth();
	int myOldHeight = pixels.getHeight();

	ofPixelUtils::cropFromTo(otherImage.pixels,pixels, x,y,w,h);

	if (myOldWidth != pixels.getWidth() || myOldHeight != pixels.getHeight()){
		if (bUseTexture == true){
			tex.clear();
			tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
		}
	}

	update();
}

//------------------------------------
template<typename T>
void ofImage_<T>::rotate90(int nRotations){
	int myOldWidth = pixels.getWidth();
	int myOldHeight = pixels.getHeight();
	ofPixelUtils::rotate90(pixels, nRotations);
	if (myOldWidth != pixels.getWidth() || myOldHeight != pixels.getHeight()){
		if (bUseTexture == true){
			tex.clear();
			tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
		}
	}
	update();
}

//------------------------------------
template<typename T>
void ofImage_<T>::mirror(bool vertical, bool horizontal){
	ofPixelUtils::mirror(pixels, vertical, horizontal);
	update();
}






//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
// freeImage based code & utilities:


//----------------------------------------------------
template<typename T>
void ofImage_<T>::resizePixels(ofPixels_<T> &pix, int newWidth, int newHeight){

	/* TODO:
	 *
	FIBITMAP * bmp					= getBmpFromPixels(pix);
	FIBITMAP * convertedBmp			= NULL;

	convertedBmp = FreeImage_Rescale(bmp, newWidth, newHeight, FILTER_BICUBIC);
	putBmpIntoPixels(convertedBmp, pix, false);

	if (bmp != NULL)				FreeImage_Unload(bmp);
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);
	*/
}

//----------------------------------------------------
template<typename T>
void ofImage_<T>::changeTypeOfPixels(ofPixels_<T> &pix, ofImageType newType){



	/*if (pix.getImageType() == newType) return;

	FIBITMAP * bmp					= getBmpFromPixels(pix);
	FIBITMAP * convertedBmp			= NULL;

	// check if we need to reallocate the texture.
	bool bNeedNewTexture = false;
	int oldType = pix.getImageType();
	if (newType > oldType){
		bNeedNewTexture = true;
	}

	// new type !
	switch (newType){

		//------------------------------------
		case OF_IMAGE_GRAYSCALE:
			convertedBmp = FreeImage_ConvertToGreyscale(bmp);
			break;

		//------------------------------------
		case OF_IMAGE_COLOR:
			convertedBmp = FreeImage_ConvertTo24Bits(bmp);
			if (bNeedNewTexture){
				tex.clear();
				tex.allocate(pixels.getWidth(), pixels.getHeight(), GL_RGB);
			}
			break;

		//------------------------------------
		case OF_IMAGE_COLOR_ALPHA:
			convertedBmp = FreeImage_ConvertTo32Bits(bmp);
			if (bNeedNewTexture){
				tex.clear();
				tex.allocate(pixels.getWidth(), pixels.getHeight(), GL_RGBA);
			}
			break;
		default:
			ofLog(OF_LOG_ERROR,"ofImage: format not supported");
	}

	putBmpIntoPixels(convertedBmp, pix, false);

	if (bmp != NULL)				FreeImage_Unload(bmp);
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);*/

}

//----------------------------------------------------------
template<typename T>
float ofImage_<T>::getHeight(){
	return height;
}

//----------------------------------------------------------
template<typename T>
float ofImage_<T>::getWidth(){
	return width;
}

//----------------------------------------------------------
// Sosolimited: texture compression
// call this function before you call loadImage()
template<typename T>
void ofImage_<T>::setCompression(ofTexCompression compression)
{
	tex.setCompression( compression );
}
