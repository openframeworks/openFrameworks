#include "ofImage.h"
#include "ofAppRunner.h"
#include "ofTypes.h"
#include "ofURLFileLoader.h"
#include "ofGraphics.h"
#include "ofPixelUtils.h"
#include "FreeImage.h"

#ifdef TARGET_ANDROID
#include <set>
	// android destroys the opengl context on screen orientation change
	// or when the application runs in the background so we need to reload
	// all the textures when the context is created again.
	// keeping a pointer to all the images we can tell them to reload from a static method
	static set<ofImage *> all_images;
	static set<ofFloatImage *> all_float_images;
	static set<ofShortImage *> all_short_images;

	static void registerImage(ofImage * img){
		all_images.insert(img);
	}

	static void registerImage(ofFloatImage * img){
		all_float_images.insert(img);
	}

	static void registerImage(ofShortImage * img){
		all_short_images.insert(img);
	}

	static void unregisterImage(ofImage * img){
		all_images.erase(img);
	}

	static void unregisterImage(ofFloatImage * img){
		all_float_images.erase(img);
	}

	static void unregisterImage(ofShortImage * img){
		all_short_images.erase(img);
	}

	void ofReloadAllImageTextures(){
		set<ofImage *>::iterator it;
		for(it=all_images.begin(); it!=all_images.end(); it++){
			(*it)->reloadTexture();
		}
		set<ofFloatImage *>::iterator f_it;
		for(f_it=all_float_images.begin(); f_it!=all_float_images.end(); f_it++){
			(*f_it)->reloadTexture();
		}
	}
#endif


//----------------------------------------------------------
// static variable for freeImage initialization:
void ofInitFreeImage(bool deinit=false){
	// need a new bool to avoid c++ "deinitialization order fiasco":
	// http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.15
	static bool	* bFreeImageInited = new bool(false);
	if(!bFreeImageInited && !deinit){
		FreeImage_Initialise();
		*bFreeImageInited = true;
	}
	if(bFreeImageInited && deinit){
		FreeImage_DeInitialise();
	}
}

template <typename T>
FREE_IMAGE_TYPE getFreeImageType(ofPixels_<T>& pix);

template <>
FREE_IMAGE_TYPE getFreeImageType(ofPixels& pix) {
	return FIT_BITMAP;
}

template <>
FREE_IMAGE_TYPE getFreeImageType(ofShortPixels& pix) {
	switch(pix.getNumChannels()) {
		case 1: return FIT_UINT16;
		case 3: return FIT_RGB16;
		case 4: return FIT_RGBA16;
	}
}
template <>
FREE_IMAGE_TYPE getFreeImageType(ofFloatPixels& pix) {
	switch(pix.getNumChannels()) {
		case 1: return FIT_FLOAT;
		case 3: return FIT_RGBF;
		case 4: return FIT_RGBAF;
	}
}

//----------------------------------------------------
template<typename T>
FIBITMAP* getBmpFromPixels(ofPixels_<T> &pix){	
	unsigned char* pixels = (unsigned char*) pix.getPixels();
	int w = pix.getWidth();
	int h = pix.getHeight();
	unsigned int bytesPerPixel = pix.getBytesPerPixel();
	int pitch = w * bytesPerPixel;
	unsigned int bpp = pix.getBitsPerPixel();
	
	FIBITMAP* bmp = FreeImage_ConvertFromRawBits(pixels, w, h, pitch, bpp, 0,0,0, true);

	BITMAPINFOHEADER* header;
	header = FreeImage_GetInfoHeader(bmp);
	//ofLogVerbose() << "getBmpFromPixels(" << bpp << ") is " << header->biSize << ", " << header->biPlanes << ", " << header->biBitCount << ", " << header->biCompression << ", " << header->biSizeImage;
	//header->biBitCount = bpp;
	//header->biPlanes = pix.getNumChannels();
	
	// {afaict, this paletting code isn't necessary anymore - kyle, may 2011}
	// this is for grayscale images they need to be paletted from: http://sourceforge.net/forum/message.php?msg_id=2856879
	if(pix.getImageType() == OF_IMAGE_GRAYSCALE && pix.getBitsPerChannel() <= 8){
		RGBQUAD *pal = FreeImage_GetPalette(bmp);
		for(unsigned int i = 0; i < 256; i++) {
			pal[i].rgbRed = i;
			pal[i].rgbGreen = i;
			pal[i].rgbBlue = i;
		}
	}
	
	return bmp;
}

//----------------------------------------------------
template<typename T>
void putBmpIntoPixels(FIBITMAP * bmp, ofPixels_<T> &pix, bool swapForLittleEndian = true){
	unsigned int width = FreeImage_GetWidth(bmp);
	unsigned int height = FreeImage_GetHeight(bmp);
	unsigned int bpp = FreeImage_GetBPP(bmp);
	unsigned int channels = (bpp / sizeof(T)) / 8;
	unsigned int pitch = (width * bpp) / 8;
	
	//BITMAPINFOHEADER* header = FreeImage_GetInfoHeader(bmp);
	//ofLogVerbose() << "putBmpIntoPixels(" << bpp << " / " << channels << ") is " << header->biSize << ", " << header->biPlanes << ", " << header->biBitCount << ", " << header->biCompression << ", " << header->biSizeImage;

	pix.allocate(width, height, channels);
	FreeImage_ConvertToRawBits((uint8_t*) pix.getPixels(), bmp, pitch, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, true);

#ifdef TARGET_LITTLE_ENDIAN
	if(swapForLittleEndian) {
		pix.swapRgb();
	}
#endif
}

//----------------------------------------------------
template<>
void putBmpIntoPixels(FIBITMAP * bmp, ofPixels &pix, bool swapForLittleEndian){
	int width			= FreeImage_GetWidth(bmp);
	int height			= FreeImage_GetHeight(bmp);
	int bpp				= FreeImage_GetBPP(bmp);

	FIBITMAP * bmpTemp = NULL;

	switch (bpp){
		case 8:
			if (FreeImage_GetColorType(bmp) == FIC_PALETTE) {
				bmpTemp = FreeImage_ConvertTo24Bits(bmp);
				bmp = bmpTemp;
				bpp = FreeImage_GetBPP(bmp);
			} else {
			// do nothing we are grayscale
			}
		break;
		case 24:
			// do nothing we are color
		break;
		case 32:
			// do nothing we are colorAlpha
		break;
		default:
			bmpTemp = FreeImage_ConvertTo24Bits(bmp);
			bmp = bmpTemp;
			bpp = FreeImage_GetBPP(bmp);
		break;
	}

	int channels	= bpp / 8;
	pix.allocate(width, height, channels);
	FreeImage_ConvertToRawBits(pix.getPixels(), bmp, width*channels, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, true);  // get bits

	if (bmpTemp != NULL) FreeImage_Unload(bmpTemp);

	#ifdef TARGET_LITTLE_ENDIAN
		if(swapForLittleEndian)
			pix.swapRgb();
	#endif
}

template<typename T>
static bool loadImage(ofPixels_<T> & pix, string fileName){
	ofInitFreeImage();
	if(fileName.substr(0, 7) == "http://") {
		return ofLoadImage(pix, ofLoadURL(fileName).data);
	}
	
	int					width, height, bpp;
	fileName			= ofToDataPath(fileName);
	bool bLoaded		= false;
	FIBITMAP 			* bmp = NULL;


	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if(fif == FIF_UNKNOWN) {
		// or guess via filename
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	}
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		bmp					= FreeImage_Load(fif, fileName.c_str(), 0);

		if (bmp){
			bLoaded = true;
		}
	}
	//-----------------------------

	if ( bLoaded ){
		putBmpIntoPixels(bmp,pix);
	} else {
		width = height = bpp = 0;
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}

	return bLoaded;
}

template<typename T>
static bool loadImage(ofPixels_<T> & pix, const ofBuffer & buffer){
	ofInitFreeImage();
	int					width, height, bpp;
	bool bLoaded		= false;
	FIBITMAP * bmp		= NULL;
	FIMEMORY *hmem		= NULL;
	
	hmem = FreeImage_OpenMemory((unsigned char*)buffer.getBinaryBuffer(), buffer.size());
	if (hmem == NULL){
		ofLog(OF_LOG_ERROR, "couldn't create memory handle!");
		return false;
	}

	//get the file type!
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem);
	if( fif == -1 ){
		ofLog(OF_LOG_ERROR, "unable to guess format", fif);
		return false;
		FreeImage_CloseMemory(hmem);
	}


	//make the image!!
	bmp = FreeImage_LoadFromMemory(fif, hmem, 0);
	
	if( bmp != NULL ){
		bLoaded = true;
		ofLog(OF_LOG_VERBOSE, "FreeImage_LoadFromMemory worked!");
	}
	
	//-----------------------------

	if (bLoaded){
		putBmpIntoPixels(bmp,pix);
	} else {
		width = height = bpp = 0;
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}
	
	if( hmem != NULL ){
		FreeImage_CloseMemory(hmem);
	}

	return bLoaded;
}


//----------------------------------------------------
bool ofLoadImage(ofPixels & pix, string fileName) {
	return loadImage(pix,fileName);
}

//----------------------------------------------------
bool ofLoadImage(ofPixels & pix, const ofBuffer & buffer) {
	return loadImage(pix,buffer);
}

//----------------------------------------------------
bool ofLoadImage(ofFloatPixels & pix, string path){
	return loadImage(pix,path);
}

//----------------------------------------------------
bool ofLoadImage(ofFloatPixels & pix, const ofBuffer & buffer){
	return loadImage(pix,buffer);
}

//----------------------------------------------------
bool ofLoadImage(ofShortPixels & pix, string path){
	return loadImage(pix,path);
}

//----------------------------------------------------
bool ofLoadImage(ofShortPixels & pix, const ofBuffer & buffer){
	return loadImage(pix,buffer);
}


//----------------------------------------------------------------
bool ofLoadImage(ofTexture & tex, string path){
	ofPixels pixels;
	bool loaded = ofLoadImage(pixels,path);
	if(loaded){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
		tex.loadData(pixels);
	}
	return loaded;
}

//----------------------------------------------------------------
bool ofLoadImage(ofTexture & tex, const ofBuffer & buffer){
	ofPixels pixels;
	bool loaded = ofLoadImage(pixels,buffer);
	if(loaded){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
		tex.loadData(pixels);
	}
	return loaded;
}

//----------------------------------------------------------------
template<typename T>
static void saveImage(ofPixels_<T> & pix, string fileName, ofImageQualityType qualityLevel) {
	ofInitFreeImage();
	if (pix.isAllocated() == false){
		ofLog(OF_LOG_ERROR,"error saving image - pixels aren't allocated");
		return;
	}

	#ifdef TARGET_LITTLE_ENDIAN
		pix.swapRgb();
	#endif

	FIBITMAP * bmp	= getBmpFromPixels(pix);

	#ifdef TARGET_LITTLE_ENDIAN
		pix.swapRgb();
	#endif
	
	fileName = ofToDataPath(fileName);
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if(fif == FIF_UNKNOWN) {
		// or guess via filename
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	}
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		if(fif == FIF_JPEG) {
			int quality = JPEG_QUALITYSUPERB;
			switch(qualityLevel) {
				case OF_IMAGE_QUALITY_WORST: quality = JPEG_QUALITYBAD; break;
				case OF_IMAGE_QUALITY_LOW: quality = JPEG_QUALITYAVERAGE; break;
				case OF_IMAGE_QUALITY_MEDIUM: quality = JPEG_QUALITYNORMAL; break;
				case OF_IMAGE_QUALITY_HIGH: quality = JPEG_QUALITYGOOD; break;
				case OF_IMAGE_QUALITY_BEST: quality = JPEG_QUALITYSUPERB; break;
			}
			FreeImage_Save(fif, bmp, fileName.c_str(), quality);
		} else {
			if(qualityLevel != OF_IMAGE_QUALITY_BEST) {
				ofLog(OF_LOG_WARNING, "ofImageCompressionType only applies to JPEG images, ignoring value.");
			}
			if (fif==FIF_GIF) bmp = FreeImage_ConvertTo8Bits(bmp);
			FreeImage_Save(fif, bmp, fileName.c_str());
		}
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}
}

void ofSaveImage(ofPixels & pix, string fileName, ofImageQualityType qualityLevel){
	saveImage(pix,fileName,qualityLevel);
}

void ofSaveImage(ofFloatPixels & pix, string fileName, ofImageQualityType qualityLevel) {
	saveImage(pix,fileName,qualityLevel);
}

void ofSaveImage(ofShortPixels & pix, string fileName, ofImageQualityType qualityLevel) {
	saveImage(pix,fileName,qualityLevel);
}

void ofSaveImage(ofPixels & pix, ofBuffer & buffer, ofImageQualityType qualityLevel) {
	ofLog(OF_LOG_ERROR, "ofSaveImage(pix, buffer) is not yet implemented");
}

void ofSaveImage(ofFloatPixels & pix, ofBuffer & buffer, ofImageQualityType qualityLevel) {
	ofLog(OF_LOG_ERROR, "ofSaveImage(pix, buffer) is not yet implemented");
}

void ofSaveImage(ofShortPixels & pix, ofBuffer & buffer, ofImageQualityType qualityLevel) {
	ofLog(OF_LOG_ERROR, "ofSaveImage(pix, buffer) is not yet implemented");
}


//----------------------------------------------------
// freeImage based stuff:
void ofCloseFreeImage(){
	ofInitFreeImage(true);
}

//-------------------------------------------------------------
//  implementation

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
	registerImage(this);
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
	registerImage(this);
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
	registerImage(this);
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
	registerImage(this);
#endif

	loadImage(filename);
}

//----------------------------------------------------------
template<typename T>
ofImage_<T>& ofImage_<T>::operator=(const ofImage_<T>& mom) {
	if(&mom==this) return *this;
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
	unregisterImage(this);
#endif
}


//----------------------------------------------------------
template<typename T>
void ofImage_<T>::reloadTexture(){
	if (pixels.isAllocated() == true && bUseTexture == true){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
		tex.loadData(pixels.getPixels(), pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
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
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
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
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
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
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
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
T * ofImage_<T>::getPixels(){
	return pixels.getPixels();
}

//----------------------------------------------------------
template<typename T>
ofPixels_<T> & ofImage_<T>::getPixelsRef(){
	return pixels;
}

//----------------------------------------------------------
template<typename T>
ofImage_<T>::operator ofPixels_<T>&(){
	return pixels;
}

//------------------------------------
//for getting a reference to the texture
template<typename T>
ofTexture & ofImage_<T>::getTextureReference(){
/*
	// it should be the responsibility of anything using getTextureReference()
	// to check that it's allocated
	if(!tex.bAllocated() ){
		ofLog(OF_LOG_WARNING, "ofImage - getTextureReference - texture is not allocated");
	}
	*/
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
		tex.loadData(pixels.getPixels(), pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
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
		glReadPixels(_x, _y, _w, _h, ofGetGlInternalFormat(pixels), GL_UNSIGNED_BYTE, pixels.getPixels()); // read the memory....
	#ifndef TARGET_OPENGLES
		glPopClientAttrib();
	#endif

	int sizeOfOneLineOfPixels = pixels.getWidth() * pixels.getBytesPerPixel();
	T * tempLineOfPix = new T[sizeOfOneLineOfPixels];
	T * linea;
	T * lineb;
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
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
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
			tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
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
			tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
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

	FIBITMAP * bmp					= getBmpFromPixels(pix);
	FIBITMAP * convertedBmp			= NULL;

	convertedBmp = FreeImage_Rescale(bmp, newWidth, newHeight, FILTER_BICUBIC);
	putBmpIntoPixels(convertedBmp, pix, false);

	if (bmp != NULL)				FreeImage_Unload(bmp);
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);
}

//----------------------------------------------------
template<typename T>
void ofImage_<T>::changeTypeOfPixels(ofPixels_<T> &pix, ofImageType newType){
	if (pix.getImageType() == newType) return;

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
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);

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

template class ofImage_<unsigned char>;
template class ofImage_<float>;
template class ofImage_<unsigned short>;


