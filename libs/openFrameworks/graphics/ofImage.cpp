#include "ofImage.h"
#include "ofAppRunner.h"
#include "ofTypes.h"
#include "ofURLFileLoader.h"
#include "ofGraphics.h"
#include "FreeImage.h"

#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
#include <set>
	// android destroys the opengl context on screen orientation change
	// or when the application runs in the background so we need to reload
	// all the textures when the context is created again.
	// keeping a pointer to all the images we can tell them to reload from a static method
	static set<ofImage *> & all_images(){
		static set<ofImage *> * images = new set<ofImage *>;
		return *images;
	}
	static set<ofFloatImage *> & all_float_images(){
		static set<ofFloatImage *> * images = new set<ofFloatImage *>;
		return *images;
	}

	static set<ofShortImage *> & all_short_images(){
		static set<ofShortImage *> * images = new set<ofShortImage *>;
		return *images;
	}

	static void registerImage(ofImage * img){
		all_images().insert(img);
	}

	static void registerImage(ofFloatImage * img){
		all_float_images().insert(img);
	}

	static void registerImage(ofShortImage * img){
		all_short_images().insert(img);
	}

	static void unregisterImage(ofImage * img){
		all_images().erase(img);
	}

	static void unregisterImage(ofFloatImage * img){
		all_float_images().erase(img);
	}

	static void unregisterImage(ofShortImage * img){
		all_short_images().erase(img);
	}

	void ofReloadAllImageTextures(){
		set<ofImage *>::iterator it;
		for(it=all_images().begin(); it!=all_images().end(); it++){
			(*it)->reloadTexture();
		}
		set<ofFloatImage *>::iterator f_it;
		for(f_it=all_float_images().begin(); f_it!=all_float_images().end(); f_it++){
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
	if(!*bFreeImageInited && !deinit){
		FreeImage_Initialise();
		*bFreeImageInited = true;
	}
	if(*bFreeImageInited && deinit){
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
		default:
			ofLogError("ofImage") << "getFreeImageType(): unknown FreeImage type for number of channels:" << pix.getNumChannels();
			return FIT_UNKNOWN;
	}
}
template <>
FREE_IMAGE_TYPE getFreeImageType(ofFloatPixels& pix) {
	switch(pix.getNumChannels()) {
		case 1: return FIT_FLOAT;
		case 3: return FIT_RGBF;
		case 4: return FIT_RGBAF;
		default:
			ofLogError("ofImage") << "getFreeImageType(): unknown FreeImage type for number of channels:" << pix.getNumChannels();
			return FIT_UNKNOWN;
	}
}

//----------------------------------------------------
template<typename PixelType>
FIBITMAP* getBmpFromPixels(ofPixels_<PixelType> &pix){
	PixelType* pixels = pix.getPixels();
	unsigned int width = pix.getWidth();
	unsigned int height = pix.getHeight();
	unsigned int bpp = pix.getBitsPerPixel();
	
	FREE_IMAGE_TYPE freeImageType = getFreeImageType(pix);
	FIBITMAP* bmp = FreeImage_AllocateT(freeImageType, width, height, bpp);
	unsigned char* bmpBits = FreeImage_GetBits(bmp);
	if(bmpBits != NULL) {
		int srcStride = width * pix.getBytesPerPixel();
		int dstStride = FreeImage_GetPitch(bmp);
		unsigned char* src = (unsigned char*) pixels;
		unsigned char* dst = bmpBits;
		for(int i = 0; i < (int)height; i++) {
			memcpy(dst, src, srcStride);
			src += srcStride;
			dst += dstStride;
		}
	} else {
		ofLogError("ofImage") << "getBmpFromPixels(): unable to get FIBITMAP from ofPixels";
	}
	
	// ofPixels are top left, FIBITMAP is bottom left
	FreeImage_FlipVertical(bmp);
	
	return bmp;
}

//----------------------------------------------------
template<typename PixelType>
void putBmpIntoPixels(FIBITMAP * bmp, ofPixels_<PixelType> &pix, bool swapForLittleEndian = true) {
	// some images use a palette, or <8 bpp, so convert them to raster 8-bit channels
	FIBITMAP* bmpConverted = NULL;
	if(FreeImage_GetColorType(bmp) == FIC_PALETTE || FreeImage_GetBPP(bmp) < 8) {
		if(FreeImage_IsTransparent(bmp)) {
			bmpConverted = FreeImage_ConvertTo32Bits(bmp);
		} else {
			bmpConverted = FreeImage_ConvertTo24Bits(bmp);
		}
		bmp = bmpConverted;
	}

	unsigned int width = FreeImage_GetWidth(bmp);
	unsigned int height = FreeImage_GetHeight(bmp);
	unsigned int bpp = FreeImage_GetBPP(bmp);
	unsigned int channels = (bpp / sizeof(PixelType)) / 8;
	unsigned int pitch = FreeImage_GetPitch(bmp);

	// ofPixels are top left, FIBITMAP is bottom left
	FreeImage_FlipVertical(bmp);
	
	unsigned char* bmpBits = FreeImage_GetBits(bmp);
	if(bmpBits != NULL) {
		pix.setFromAlignedPixels((PixelType*) bmpBits, width, height, channels, pitch);
	} else {
		ofLogError("ofImage") << "putBmpIntoPixels(): unable to set ofPixels from FIBITMAP";
	}
	
	if(bmpConverted != NULL) {
		FreeImage_Unload(bmpConverted);
	}

#ifdef TARGET_LITTLE_ENDIAN
	if(swapForLittleEndian && sizeof(PixelType) == 1) {
		pix.swapRgb();
	}
#endif
}

template<typename PixelType>
static bool loadImage(ofPixels_<PixelType> & pix, string fileName){
	ofInitFreeImage();
	if(fileName.substr(0, 7) == "http://") {
		return ofLoadImage(pix, ofLoadURL(fileName).data);
	}
	
	fileName = ofToDataPath(fileName);
	bool bLoaded = false;
	FIBITMAP * bmp = NULL;

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if(fif == FIF_UNKNOWN) {
		// or guess via filename
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	}
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		bmp = FreeImage_Load(fif, fileName.c_str(), 0);

		if (bmp != NULL){
			bLoaded = true;
		}
	}
	
	//-----------------------------

	if ( bLoaded ){
		putBmpIntoPixels(bmp,pix);
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}

	return bLoaded;
}

template<typename PixelType>
static bool loadImage(ofPixels_<PixelType> & pix, const ofBuffer & buffer){
	ofInitFreeImage();
	bool bLoaded = false;
	FIBITMAP* bmp = NULL;
	FIMEMORY* hmem = NULL;
	
	hmem = FreeImage_OpenMemory((unsigned char*) buffer.getBinaryBuffer(), buffer.size());
	if (hmem == NULL){
		ofLogError("ofImage") << "loadImage(): couldn't laod image from ofBuffer, opening FreeImage memory failed";
		return false;
	}

	//get the file type!
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem);
	if( fif == -1 ){
		ofLogError("ofImage") << "loadImage(): couldn't load image from ofBuffer, unable to guess image format from memory";
		return false;
		FreeImage_CloseMemory(hmem);
	}


	//make the image!!
	bmp = FreeImage_LoadFromMemory(fif, hmem, 0);
	
	if( bmp != NULL ){
		bLoaded = true;
	}
	
	//-----------------------------
	
	if (bLoaded){
		putBmpIntoPixels(bmp,pix);
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
template<typename PixelType>
static void saveImage(ofPixels_<PixelType> & pix, string fileName, ofImageQualityType qualityLevel) {
	ofInitFreeImage();
	if (pix.isAllocated() == false){
		ofLogError("ofImage") << "saveImage(): couldn't save \"" << fileName << "\", pixels are not allocated";
		return;
	}

	#ifdef TARGET_LITTLE_ENDIAN
	if(sizeof(PixelType) == 1) {
		pix.swapRgb();
	}
	#endif

	FIBITMAP * bmp	= getBmpFromPixels(pix);

	#ifdef TARGET_LITTLE_ENDIAN
	if(sizeof(PixelType) == 1) {
		pix.swapRgb();
	}
	#endif
	
	ofFilePath::createEnclosingDirectory(fileName);
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
				ofLogWarning("ofImage") << "saveImage(): ofImageCompressionType only applies to JPEGs,"
					<< " ignoring value for \" "<< fileName << "\"";
			}
			
			if (fif == FIF_GIF) {
				FIBITMAP* convertedBmp;
				if(pix.getImageType() == OF_IMAGE_COLOR_ALPHA) {
					// this just converts the image to grayscale so it can save something
					convertedBmp = FreeImage_ConvertTo8Bits(bmp);
				} else {
					// this will create a 256-color palette from the image
					convertedBmp = FreeImage_ColorQuantize(bmp, FIQ_NNQUANT);
				}
				FreeImage_Save(fif, convertedBmp, fileName.c_str());
				if (convertedBmp != NULL){
					FreeImage_Unload(convertedBmp);
				}
			} else {
				FreeImage_Save(fif, bmp, fileName.c_str());
			}
		}
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}
}

//----------------------------------------------------------------
void ofSaveImage(ofPixels & pix, string fileName, ofImageQualityType qualityLevel){
	saveImage(pix,fileName,qualityLevel);
}

//----------------------------------------------------------------
void ofSaveImage(ofFloatPixels & pix, string fileName, ofImageQualityType qualityLevel) {
	saveImage(pix,fileName,qualityLevel);
}

//----------------------------------------------------------------
void ofSaveImage(ofShortPixels & pix, string fileName, ofImageQualityType qualityLevel) {
	saveImage(pix,fileName,qualityLevel);
}

//----------------------------------------------------------------
template<typename PixelType>
static void saveImage(ofPixels_<PixelType> & pix, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	// thanks to alvaro casinelli for the implementation

	ofInitFreeImage();

	if (pix.isAllocated() == false){
		ofLogError("ofImage","saveImage(): couldn't save to ofBuffer, pixels are not allocated");
		return;
	}

	if(format==OF_IMAGE_FORMAT_JPEG && pix.getNumChannels()==4){
		ofPixels pix3 = pix;
		pix3.setNumChannels(3);
		saveImage(pix3,buffer,format,qualityLevel);
		return;
	}

	#ifdef TARGET_LITTLE_ENDIAN
	if(sizeof(PixelType) == 1) {
		pix.swapRgb();
	}
	#endif

	FIBITMAP * bmp	= getBmpFromPixels(pix);

	#ifdef TARGET_LITTLE_ENDIAN
	if(sizeof(PixelType) == 1) {
		pix.swapRgb();
	}
	#endif

	if (bmp)  // bitmap successfully created
	{
		   // (b) open a memory stream to compress the image onto mem_buffer:
		   //
		   FIMEMORY *hmem = FreeImage_OpenMemory();
		   // (c) encode and save the image to the memory (on dib FIBITMAP image):
		   //
		   if(FREE_IMAGE_FORMAT(format) == FIF_JPEG) {
				int quality = JPEG_QUALITYSUPERB;
				switch(qualityLevel) {
					case OF_IMAGE_QUALITY_WORST: quality = JPEG_QUALITYBAD; break;
					case OF_IMAGE_QUALITY_LOW: quality = JPEG_QUALITYAVERAGE; break;
					case OF_IMAGE_QUALITY_MEDIUM: quality = JPEG_QUALITYNORMAL; break;
					case OF_IMAGE_QUALITY_HIGH: quality = JPEG_QUALITYGOOD; break;
					case OF_IMAGE_QUALITY_BEST: quality = JPEG_QUALITYSUPERB; break;
				}
				FreeImage_SaveToMemory(FIF_JPEG, bmp, hmem, quality);
		   }else{
				FreeImage_SaveToMemory((FREE_IMAGE_FORMAT)format, bmp, hmem);
		   }

		   /*

		  NOTE: at this point, hmem contains the entire data in memory stored in fif format. the
		  amount of space used by the memory is equal to file_size:
		  long file_size = FreeImage_TellMemory(hmem);
		  but can also be retrieved by FreeImage_AcquireMemory that retrieves both the
		  length of the buffer, and the buffer memory address.
		  */
			#ifdef TARGET_WIN32
		   	   DWORD size_in_bytes = 0;
			#else
		   	   uint32_t size_in_bytes = 0;
			#endif
		   // Save compressed data on mem_buffer
		   // note: FreeImage_AquireMemory allocates space for aux_mem_buffer):
		   //
		   unsigned char *mem_buffer = NULL;
		   if (!FreeImage_AcquireMemory(hmem, &mem_buffer, &size_in_bytes))
				   ofLogError("ofImage") << "saveImage(): couldn't save to ofBuffer, aquiring compressed image from memory failed";

		   /*
			  Now, before closing the memory stream, copy the content of mem_buffer
			  to an auxiliary buffer
		    */

		   buffer.set((char*)mem_buffer,size_in_bytes);

		   // Finally, close the FIBITMAP object, or we will get a memory leak:
		   FreeImage_Unload(bmp);

		   // Close the memory stream (otherwise we may get a memory leak).
		   FreeImage_CloseMemory(hmem);
	}
}

//----------------------------------------------------------------
void ofSaveImage(ofPixels & pix, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	saveImage(pix,buffer,format,qualityLevel);
}

void ofSaveImage(ofFloatPixels & pix, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	saveImage(pix,buffer,format,qualityLevel);
}

void ofSaveImage(ofShortPixels & pix, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	saveImage(pix,buffer,format,qualityLevel);
}


//----------------------------------------------------
// freeImage based stuff:
void ofCloseFreeImage(){
	ofInitFreeImage(true);
}

//-------------------------------------------------------------
//  implementation

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::ofImage_(){

	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	//----------------------- init free image if necessary
	ofInitFreeImage();

}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::ofImage_(const ofPixels_<PixelType> & pix){
	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	//----------------------- init free image if necessary
	ofInitFreeImage();


	setFromPixels(pix);
}

template<typename PixelType>
ofImage_<PixelType>::ofImage_(const ofFile & file){
	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	//----------------------- init free image if necessary
	ofInitFreeImage();


	loadImage(file);
}

template<typename PixelType>
ofImage_<PixelType>::ofImage_(const string & filename){
	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	//----------------------- init free image if necessary
	ofInitFreeImage();


	loadImage(filename);
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>& ofImage_<PixelType>::operator=(const ofImage_<PixelType>& mom) {
	if(&mom==this) return *this;
	clone(mom);
	update();
	return *this;
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::ofImage_(const ofImage_<PixelType>& mom) {
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	registerImage(this);
#endif
	clear();
	clone(mom);
	update();
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::~ofImage_(){
	clear();
}


//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::reloadTexture(){
	if (pixels.isAllocated() && bUseTexture){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
		tex.loadData(pixels);
	}
}

//----------------------------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::loadImage(const ofFile & file){
	return loadImage(file.getAbsolutePath());
}

//----------------------------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::loadImage(string fileName){
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	registerImage(this);
#endif
	bool bLoadedOk = ofLoadImage(pixels, fileName);
	if (!bLoadedOk) {
		ofLogError("ofImage") << "loadImage(): couldn't load image from \"" << fileName << "\"";
		clear();
		return false;
	}
	if (bLoadedOk && pixels.isAllocated() && bUseTexture){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
		if(ofGetGLProgrammableRenderer() && (pixels.getNumChannels()==1 || pixels.getNumChannels()==2)){
			tex.setRGToRGBASwizzles(true);
		}
	}
	update();
	return bLoadedOk;
}

template<typename PixelType>
bool ofImage_<PixelType>::loadImage(const ofBuffer & buffer){
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	registerImage(this);
#endif
	bool bLoadedOk = ofLoadImage(pixels, buffer);
	if (!bLoadedOk) {
		ofLogError("ofImage") << "loadImage(): couldn't load image from ofBuffer";
		clear();
		return false;
	}
	if (bLoadedOk && pixels.isAllocated() && bUseTexture){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
		if(ofGetGLProgrammableRenderer() && (pixels.getNumChannels()==1 || pixels.getNumChannels()==2)){
			tex.setRGToRGBASwizzles(true);
		}
	}
	update();
	return bLoadedOk;
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::saveImage(string fileName, ofImageQualityType qualityLevel){
	ofSaveImage(pixels, fileName, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::saveImage(ofBuffer & buffer, ofImageQualityType qualityLevel){
	ofSaveImage(pixels, buffer, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::saveImage(const ofFile & file, ofImageQualityType compressionLevel){
	ofSaveImage(pixels,file.getAbsolutePath(),compressionLevel);
}

// we could cap these values - but it might be more useful
// to be able to set anchor points outside the image

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::setAnchorPercent(float xPct, float yPct){
    if (bUseTexture)tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::setAnchorPoint(float x, float y){
    if (bUseTexture)tex.setAnchorPoint(x, y);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::resetAnchor(){
   	if (bUseTexture)tex.resetAnchor();
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::draw(float x, float y){
	draw(x,y,0,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::draw(float x, float y, float z){
	draw(x,y,z,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::draw(float x, float y, float w, float h){
	draw(x,y,0,w,h);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::draw(float x, float y, float z, float w, float h){
	drawSubsection(x,y,z,w,h,0,0,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::drawSubsection(float x, float y, float w, float h, float sx, float sy){
	drawSubsection(x,y,0,w,h,sx,sy,w,h);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::drawSubsection(float x, float y, float w, float h, float sx, float sy, float _sw, float _sh){
	drawSubsection(x,y,0,w,h,sx,sy,_sw,_sh);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy){
	drawSubsection(x,y,z,w,h,sx,sy,w,h);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	ofGetCurrentRenderer()->draw(*this,x,y,z,w,h,sx,sy,sw,sh);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::allocate(int w, int h, ofImageType newType){
	
	if (width == w && height == h && newType == type){
		return;
	}
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	registerImage(this);
#endif
	pixels.allocate(w, h, newType);

	// take care of texture allocation --
	if (pixels.isAllocated() && bUseTexture){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
		if(ofGetGLProgrammableRenderer() && (pixels.getNumChannels()==1 || pixels.getNumChannels()==2)){
			tex.setRGToRGBASwizzles(true);
		}
	}
	
	width	= pixels.getWidth();
	height	= pixels.getHeight();
	bpp		= pixels.getBitsPerPixel();
	type	= pixels.getImageType();
}


//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::clear(){
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	unregisterImage(this);
#endif
	pixels.clear();
	if(bUseTexture)	tex.clear();

	width					= 0;
	height					= 0;
	bpp						= 0;
	type 					= OF_IMAGE_UNDEFINED;
	bUseTexture 			= true;		// the default is, yes, use a texture
}

//------------------------------------
template<typename PixelType>
PixelType * ofImage_<PixelType>::getPixels(){
	return pixels.getPixels();
}

//----------------------------------------------------------
template<typename PixelType>
ofPixels_<PixelType> & ofImage_<PixelType>::getPixelsRef(){
	return pixels;
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::operator ofPixels_<PixelType>&(){
	return pixels;
}

//------------------------------------
// for getting a reference to the texture
template<typename PixelType>
ofTexture & ofImage_<PixelType>::getTextureReference(){
/*
	// it should be the responsibility of anything using getTextureReference()
	// to check that it's allocated
	if(!tex.bAllocated() ){
		ofLogWarning("ofImage") << "getTextureReference(): texture is not allocated";
	}
	*/
	return tex;
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::bind(){
	if (bUseTexture && tex.bAllocated())
		tex.bind();
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::unbind(){
	if (bUseTexture && tex.bAllocated())
		tex.unbind();
}

//------------------------------------
template<typename PixelType>
ofColor_<PixelType> ofImage_<PixelType>::getColor(int x, int y) const {
	return pixels.getColor(x, y);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::setColor(int x, int y, const ofColor_<PixelType>& color) {
	pixels.setColor(x, y, color);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::setColor(int index, const ofColor_<PixelType>& color) {
	pixels.setColor(index, color);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::setColor(const ofColor_<PixelType>& color) {
	pixels.setColor(color);
}

//------------------------------------
template<typename PixelType>
void  ofImage_<PixelType>::setFromPixels(const PixelType * newPixels, int w, int h, ofImageType newType, bool bOrderIsRGB){

	allocate(w, h, newType);
	pixels.setFromPixels(newPixels,w,h,newType);

	if (!bOrderIsRGB && sizeof(PixelType) == 1){
		pixels.swapRgb();
	}

	update();
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::setFromPixels(const ofPixels_<PixelType> & pixels){
	setFromPixels(pixels.getPixels(),pixels.getWidth(),pixels.getHeight(),pixels.getImageType());
}

//------------------------------------
template<typename PixelType>
ofImage_<PixelType> & ofImage_<PixelType>::operator=(ofPixels_<PixelType> & pixels){
	setFromPixels(pixels);
	return *this;
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::update(){
	width = pixels.getWidth();
	height = pixels.getHeight();
	bpp = pixels.getBitsPerPixel();
	type = pixels.getImageType();
	if (pixels.isAllocated() && bUseTexture){
		int glTypeInternal = ofGetGlInternalFormat(pixels);
		if(!tex.isAllocated() || tex.getWidth() != width || tex.getHeight() != height || tex.getTextureData().glTypeInternal != glTypeInternal){
			tex.allocate(pixels.getWidth(), pixels.getHeight(), glTypeInternal);
			if(ofGetGLProgrammableRenderer() && (pixels.getNumChannels()==1 || pixels.getNumChannels()==2)){
				tex.setRGToRGBASwizzles(true);
			}
		}
		tex.loadData(pixels);
	}
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::isUsingTexture(){
	return bUseTexture;
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::grabScreen(int _x, int _y, int _w, int _h){

	allocate(_w, _h, OF_IMAGE_COLOR);

    int sh = ofGetViewportHeight();     // if we are in a FBO or other viewport, this fails: ofGetHeight();


	#ifndef TARGET_OPENGLES
    
    if(ofIsVFlipped()){
		_y = sh - _y;
		_y -= _h; // top, bottom issues
    }
    
    glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );											// be nice to anyone else who might use pixelStore
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(_x, _y, _w, _h, ofGetGlFormat(pixels), GL_UNSIGNED_BYTE, pixels.getPixels()); // read the memory....
    glPopClientAttrib();
    
	int sizeOfOneLineOfPixels = pixels.getWidth() * pixels.getBytesPerPixel();
	PixelType * tempLineOfPix = new PixelType[sizeOfOneLineOfPixels];
	PixelType * linea;
	PixelType * lineb;
	for (int i = 0; i < pixels.getHeight()/2; i++){
		linea = pixels.getPixels() + i * sizeOfOneLineOfPixels;
		lineb = pixels.getPixels() + (pixels.getHeight()-i-1) * sizeOfOneLineOfPixels;
		memcpy(tempLineOfPix, linea, sizeOfOneLineOfPixels);
		memcpy(linea, lineb, sizeOfOneLineOfPixels);
		memcpy(lineb, tempLineOfPix, sizeOfOneLineOfPixels);
	}
	delete [] tempLineOfPix;
	
    #else
    
    int sw = ofGetViewportWidth();
    int numPixels   = width*height;
    if( numPixels == 0 ){
        ofLogError("ofImage") << "grabScreen(): unable to grab screen, image width and/or height are 0: " << width << "x" << height;
        return;
    }
    
    int numRGBA         = numPixels*4;
    GLubyte *bufferRGBA = (GLubyte *) malloc(numRGBA);

    if(ofGetOrientation() == OF_ORIENTATION_DEFAULT || ofDoesHWOrientation()) {

        if(ofIsVFlipped()){
			_y = sh - _y;   // screen is flipped vertically.
			_y -= _h;
        }
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(_x, _y, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, bufferRGBA);
        
        for(int y = 0; y < _h; y++){  
            for(int x = 0; x < _w; x++){
                
                int i = y * _w * 3 + x * 3;
                int j = (_h-1-y) * _w * 4 + x * 4;  // rotate 90.
                
                pixels.getPixels()[i]   = bufferRGBA[j];
                pixels.getPixels()[i+1] = bufferRGBA[j+1];
                pixels.getPixels()[i+2] = bufferRGBA[j+2];
            }
        }
    }
    else if(ofGetOrientation() == OF_ORIENTATION_180) {

        if(ofIsVFlipped()){
			_x = sw - _x;   // screen is flipped horizontally.
			_x -= _w;
        }
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(_x, _y, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, bufferRGBA);
        
        for(int y = 0; y < _h; y++){  
            for(int x = 0; x < _w; x++){
                
                int i = y * _w * 3 + x * 3;
                int j = y * _w * 4 + (_w-1-x) * 4;  // rotate 90.
                
                pixels.getPixels()[i]   = bufferRGBA[j];
                pixels.getPixels()[i+1] = bufferRGBA[j+1];
                pixels.getPixels()[i+2] = bufferRGBA[j+2];
            }
        }
    }
    else if(ofGetOrientation() == OF_ORIENTATION_90_RIGHT) {
        
        swap(_w,_h);
        swap(_x,_y);


        if(!ofIsVFlipped()){
			_x = sw - _x;   // screen is flipped horizontally.
			_x -= _w;

			_y = sh - _y;   // screen is flipped vertically.
			_y -= _h;
        }
        
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(_x, _y, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, bufferRGBA);
        
        for(int y = 0; y < _h; y++){  
            for(int x = 0; x < _w; x++){
                
                int i = x * _h * 3 + y * 3;
                int j = y * _w * 4 + x * 4;
                
                pixels.getPixels()[i]   = bufferRGBA[j];
                pixels.getPixels()[i+1] = bufferRGBA[j+1];
                pixels.getPixels()[i+2] = bufferRGBA[j+2];
            }
        }
    }
    else if(ofGetOrientation() == OF_ORIENTATION_90_LEFT) {
        
        swap(_w, _h);
        
        swap(_x, _y);

        if(ofIsVFlipped()){
			_x = sw - _x;   // screen is flipped horizontally.
			_x -= _w;

			_y = sh - _y;   // screen is flipped vertically.
			_y -= _h;
        }
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(_x, _y, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, bufferRGBA);
        
        for(int y = 0; y < _h; y++){  
            for(int x = 0; x < _w; x++){
                
                int i = x * _h * 3 + y * 3;
                int j = (_h-1-y) * _w * 4 + (_w-1-x) * 4;
                
                pixels.getPixels()[i]   = bufferRGBA[j];
                pixels.getPixels()[i+1] = bufferRGBA[j+1];
                pixels.getPixels()[i+2] = bufferRGBA[j+2];
            }
        }
    }
    
    free(bufferRGBA);

    #endif

	update();
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::setImageType(ofImageType newType){
	changeTypeOfPixels(pixels, newType);
	update();
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::resize(int newWidth, int newHeight){
	if(newWidth == width && newHeight == height) return;

	resizePixels(pixels, newWidth, newHeight);

	if (bUseTexture){
		tex.clear();
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
	}

	update();
}


//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::crop(int x, int y, int w, int h){
	w = ofClamp(w,1,getWidth());
	h = ofClamp(h,1,getHeight());

	pixels.crop(x,y,w,h);
	update();
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::cropFrom(ofImage_<PixelType> & otherImage, int x, int y, int w, int h){
	w = ofClamp(w,1,otherImage.getWidth());
	h = ofClamp(h,1,otherImage.getHeight());

	int myOldWidth = pixels.getWidth();
	int myOldHeight = pixels.getHeight();

	otherImage.pixels.cropTo(pixels, x, y, w, h);

	if (myOldWidth != pixels.getWidth() || myOldHeight != pixels.getHeight()){
		if (bUseTexture){
			tex.clear();
			tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
			if(ofGetGLProgrammableRenderer() && (pixels.getNumChannels()==1 || pixels.getNumChannels()==2)){
				tex.setRGToRGBASwizzles(true);
			}
		}
	}

	update();
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::rotate90(int nRotations){
	int myOldWidth = pixels.getWidth();
	int myOldHeight = pixels.getHeight();
	pixels.rotate90(nRotations);
	if (myOldWidth != pixels.getWidth() || myOldHeight != pixels.getHeight()){
		if (bUseTexture){
			tex.clear();
			tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
			if(ofGetGLProgrammableRenderer() && (pixels.getNumChannels()==1 || pixels.getNumChannels()==2)){
				tex.setRGToRGBASwizzles(true);
			}
		}
	}
	update();
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::mirror(bool vertical, bool horizontal){
	pixels.mirror(vertical, horizontal);
	update();
}






//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
// freeImage based code & utilities:


//----------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::resizePixels(ofPixels_<PixelType> &pix, int newWidth, int newHeight){

	FIBITMAP * bmp					= getBmpFromPixels(pix);
	FIBITMAP * convertedBmp			= NULL;

	convertedBmp = FreeImage_Rescale(bmp, newWidth, newHeight, FILTER_BICUBIC);
	putBmpIntoPixels(convertedBmp, pix, false);

	if (bmp != NULL)				FreeImage_Unload(bmp);
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);
}

//----------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::changeTypeOfPixels(ofPixels_<PixelType> &pix, ofImageType newType){
	int oldType = pix.getImageType();
		
	if (oldType == newType) {
		return; // no need to reallocate
	}

	FIBITMAP * bmp = getBmpFromPixels(pix);
	FIBITMAP * convertedBmp = NULL;

	switch (newType){
		case OF_IMAGE_GRAYSCALE:
			convertedBmp = FreeImage_ConvertToGreyscale(bmp);
			break;
		case OF_IMAGE_COLOR:
			convertedBmp = FreeImage_ConvertTo24Bits(bmp);
			break;
		case OF_IMAGE_COLOR_ALPHA:
			convertedBmp = FreeImage_ConvertTo32Bits(bmp);
			break;
		default:
			ofLogError("ofImage") << "changeTypeOfPixels(): unknown image type: " << newType;
			break;
	}
	
	putBmpIntoPixels(convertedBmp, pix, false);

	if (bmp != NULL) {
		FreeImage_Unload(bmp);
	}
	if (convertedBmp != NULL) {
		FreeImage_Unload(convertedBmp);
	}

	if(bUseTexture){
		// always reallocate the texture. if ofTexture doesn't need reallocation,
		// it doesn't have to. but it needs to change the internal format.
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
		if(ofGetGLProgrammableRenderer() && (pixels.getNumChannels()==1 || pixels.getNumChannels()==2)){
			tex.setRGToRGBASwizzles(true);
		}
	}
}

//----------------------------------------------------------
template<typename PixelType>
float ofImage_<PixelType>::getHeight() {
	return height;
}

//----------------------------------------------------------
template<typename PixelType>
float ofImage_<PixelType>::getWidth() {
	return width;
}

//----------------------------------------------------------
// Sosolimited: texture compression
// call this function before you call loadImage()
template<typename PixelType>
void ofImage_<PixelType>::setCompression(ofTexCompression compression)
{
	if(bUseTexture){
		tex.setCompression( compression );
	}
}

template class ofImage_<unsigned char>;
template class ofImage_<float>;
template class ofImage_<unsigned short>;


