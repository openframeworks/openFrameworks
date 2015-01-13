#include "ofImage.h"
#include "ofAppRunner.h"
#include "ofTypes.h"
#include "ofGraphics.h"
#include "FreeImage.h"

#ifndef TARGET_EMSCRIPTEN
#include "ofURLFileLoader.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#endif

#if defined(TARGET_ANDROID)
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
		for(auto img: all_images()){
			img->update();
		}
		for(auto img: all_short_images()){
			img->update();
		}
		for(auto img: all_float_images()){
			img->update();
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
	PixelType* pixels = pix.getData();
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
		if(srcStride != dstStride){
			for(int i = 0; i < (int)height; i++) {
				memcpy(dst, src, srcStride);
				src += srcStride;
				dst += dstStride;
			}
		}else{
			memcpy(dst,src,dstStride*height);
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
	// convert to correct type depending on type of input bmp and PixelType
	FIBITMAP* bmpConverted = NULL;
	FREE_IMAGE_TYPE imgType = FreeImage_GetImageType(bmp);
	if(sizeof(PixelType)==1 &&
		(FreeImage_GetColorType(bmp) == FIC_PALETTE || FreeImage_GetBPP(bmp) < 8
		||  imgType!=FIT_BITMAP)) {
		if(FreeImage_IsTransparent(bmp)) {
			bmpConverted = FreeImage_ConvertTo32Bits(bmp);
		} else {
			bmpConverted = FreeImage_ConvertTo24Bits(bmp);
		}
		bmp = bmpConverted;
	}else if(sizeof(PixelType)==2 && imgType!=FIT_UINT16 && imgType!=FIT_RGB16 && imgType!=FIT_RGBA16){
		if(FreeImage_IsTransparent(bmp)) {
			bmpConverted = FreeImage_ConvertToType(bmp,FIT_RGBA16);
		} else {
			bmpConverted = FreeImage_ConvertToType(bmp,FIT_RGB16);
		}
		bmp = bmpConverted;
	}else if(sizeof(PixelType)==4 && imgType!=FIT_FLOAT && imgType!=FIT_RGBF && imgType!=FIT_RGBAF){
		if(FreeImage_IsTransparent(bmp)) {
			bmpConverted = FreeImage_ConvertToType(bmp,FIT_RGBAF);
		} else {
			bmpConverted = FreeImage_ConvertToType(bmp,FIT_RGBF);
		}
		bmp = bmpConverted;
	}

	unsigned int width = FreeImage_GetWidth(bmp);
	unsigned int height = FreeImage_GetHeight(bmp);
	unsigned int bpp = FreeImage_GetBPP(bmp);
	unsigned int channels = (bpp / sizeof(PixelType)) / 8;
	unsigned int pitch = FreeImage_GetPitch(bmp);

	ofPixelFormat pixFormat;
	if(channels==1) pixFormat=OF_PIXELS_GRAY;
#ifdef TARGET_LITTLE_ENDIAN
	if(swapForLittleEndian){
		if(channels==3) pixFormat=OF_PIXELS_BGR;
		if(channels==4) pixFormat=OF_PIXELS_BGRA;
	}else{
		if(channels==3) pixFormat=OF_PIXELS_RGB;
		if(channels==4) pixFormat=OF_PIXELS_RGBA;
	}
#else
	if(channels==3) pixFormat=OF_PIXELS_RGB;
	if(channels==4) pixFormat=OF_PIXELS_RGBA;
#endif

	// ofPixels are top left, FIBITMAP is bottom left
	FreeImage_FlipVertical(bmp);
	
	unsigned char* bmpBits = FreeImage_GetBits(bmp);
	if(bmpBits != NULL) {
		pix.setFromAlignedPixels((PixelType*) bmpBits, width, height, pixFormat, pitch);
	} else {
		ofLogError("ofImage") << "putBmpIntoPixels(): unable to set ofPixels from FIBITMAP";
	}
	
	if(bmpConverted != NULL) {
		FreeImage_Unload(bmpConverted);
	}

#ifdef TARGET_LITTLE_ENDIAN
	if(swapForLittleEndian && sizeof(PixelType) == 1 && channels >=3 ) {
		pix.swapRgb();
	}
#endif
}

template<typename PixelType>
static bool loadImage(ofPixels_<PixelType> & pix, string fileName){
	ofInitFreeImage();

#ifndef TARGET_EMSCRIPTEN
	// Attempt to parse the fileName as a url - specifically it must be a full address starting with http/https
	// Poco::URI normalizes to lowercase
	Poco::URI uri;
    try {
        uri = Poco::URI(fileName);
    } catch(const Poco::SyntaxException& exc){
        ofLogError("ofImage") << "loadImage(): malformed url when loading image from url \"" << fileName << "\": " << exc.displayText();
		return false;
    }
	if(uri.getScheme() == "http" || uri.getScheme() == "https"){
		return ofLoadImage(pix, ofLoadURL(fileName).data);
	}
#endif
	
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
	
	hmem = FreeImage_OpenMemory((unsigned char*) buffer.getData(), buffer.size());
	if (hmem == NULL){
		ofLogError("ofImage") << "loadImage(): couldn't load image from ofBuffer, opening FreeImage memory failed";
		return false;
	}

	//get the file type!
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem);
	if( fif == -1 ){
		ofLogError("ofImage") << "loadImage(): couldn't load image from ofBuffer, unable to guess image format from memory";
		FreeImage_CloseMemory(hmem);
		return false;
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
	if(sizeof(PixelType) == 1 && (pix.getPixelFormat()==OF_PIXELS_RGB || pix.getPixelFormat()==OF_PIXELS_RGBA)) {
		pix.swapRgb();
	}
	#endif

	FIBITMAP * bmp	= getBmpFromPixels(pix);

	#ifdef TARGET_LITTLE_ENDIAN
	if(sizeof(PixelType) == 1 && (pix.getPixelFormat()==OF_PIXELS_BGR || pix.getPixelFormat()==OF_PIXELS_BGRA)) {
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


	load(file);
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


	load(filename);
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
#if defined(TARGET_ANDROID)
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
bool ofImage_<PixelType>::load(const ofFile & file){
	return load(file.getAbsolutePath());
}

//----------------------------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::loadImage(const ofFile & file){
	return load(file);
}

//----------------------------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::load(string fileName){
#if defined(TARGET_ANDROID)
	registerImage(this);
#endif
	bool bLoadedOk = ofLoadImage(pixels, fileName);
	if (!bLoadedOk) {
		ofLogError("ofImage") << "loadImage(): couldn't load image from \"" << fileName << "\"";
		clear();
		return false;
	}
	update();
	return bLoadedOk;
}

//----------------------------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::loadImage(string fileName){
	return load(fileName);
}

//----------------------------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::load(const ofBuffer & buffer){
#if defined(TARGET_ANDROID)
	registerImage(this);
#endif
	bool bLoadedOk = ofLoadImage(pixels, buffer);
	if (!bLoadedOk) {
		ofLogError("ofImage") << "loadImage(): couldn't load image from ofBuffer";
		clear();
		return false;
	}
	update();
	return bLoadedOk;
}

//----------------------------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::loadImage(const ofBuffer & buffer){
	return load(buffer);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::save(string fileName, ofImageQualityType qualityLevel){
	ofSaveImage(pixels, fileName, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::save(ofBuffer & buffer, ofImageQualityType qualityLevel){
	ofSaveImage(pixels, buffer, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::save(const ofFile & file, ofImageQualityType compressionLevel){
	ofSaveImage(pixels,file.getAbsolutePath(),compressionLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::saveImage(string fileName, ofImageQualityType qualityLevel){
	save(fileName, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::saveImage(ofBuffer & buffer, ofImageQualityType qualityLevel){
	save(buffer, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::saveImage(const ofFile & file, ofImageQualityType compressionLevel){
	save(file,compressionLevel);
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
void ofImage_<PixelType>::draw(float x, float y) const{
	draw(x,y,0,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::draw(float x, float y, float z) const{
	draw(x,y,z,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::draw(float x, float y, float w, float h) const{
	draw(x,y,0,w,h);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::draw(float x, float y, float z, float w, float h) const{
	drawSubsection(x,y,z,w,h,0,0,getWidth(),getHeight());
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::drawSubsection(float x, float y, float w, float h, float sx, float sy) const{
	drawSubsection(x,y,0,w,h,sx,sy,w,h);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::drawSubsection(float x, float y, float w, float h, float sx, float sy, float _sw, float _sh) const{
	drawSubsection(x,y,0,w,h,sx,sy,_sw,_sh);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy) const{
	drawSubsection(x,y,z,w,h,sx,sy,w,h);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	ofGetCurrentRenderer()->draw(*this,x,y,z,w,h,sx,sy,sw,sh);
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::allocate(int w, int h, ofImageType newType){
	
	if (width == w && height == h && newType == type){
		return;
	}
#if defined(TARGET_ANDROID)
	registerImage(this);
#endif
	pixels.allocate(w, h, newType);

	// take care of texture allocation --
	if (pixels.isAllocated() && bUseTexture){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
		if(ofIsGLProgrammableRenderer() && (pixels.getPixelFormat()==OF_PIXELS_GRAY || pixels.getPixelFormat()==OF_PIXELS_GRAY_ALPHA)){
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
#if defined(TARGET_ANDROID)
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
ofPixels_<PixelType> &  ofImage_<PixelType>::getPixels(){
	return pixels;
}

//------------------------------------
template<typename PixelType>
const ofPixels_<PixelType> & ofImage_<PixelType>::getPixels() const{
	return pixels;
}

//----------------------------------------------------------
template<typename PixelType>
ofPixels_<PixelType> & ofImage_<PixelType>::getPixelsRef(){
	return pixels;
}

//----------------------------------------------------------
template<typename PixelType>
const ofPixels_<PixelType> & ofImage_<PixelType>::getPixelsRef() const {
	return pixels;
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::operator ofPixels_<PixelType>&(){
	return pixels;
}

//------------------------------------
template<typename PixelType>
ofTexture & ofImage_<PixelType>::getTexture(){
	return tex;
}

//------------------------------------
template<typename PixelType>
const ofTexture & ofImage_<PixelType>::getTexture() const{
	return tex;
}

//------------------------------------
// for getting a reference to the texture
template<typename PixelType>
ofTexture & ofImage_<PixelType>::getTextureReference(){
	return getTexture();
}

//------------------------------------
template<typename PixelType>
const ofTexture & ofImage_<PixelType>::getTextureReference() const{
	return getTexture();
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::bind(int textureLocation) const{
	if (bUseTexture && tex.isAllocated())
		tex.bind(textureLocation);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::unbind(int textureLocation) const{
	if (bUseTexture && tex.isAllocated())
		tex.unbind(textureLocation);
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
	setFromPixels(pixels.getData(),pixels.getWidth(),pixels.getHeight(),pixels.getImageType());
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
			tex.allocate(pixels);
			if(ofIsGLProgrammableRenderer() && (pixels.getPixelFormat()==OF_PIXELS_GRAY || pixels.getPixelFormat()==OF_PIXELS_GRAY_ALPHA)){
				tex.setRGToRGBASwizzles(true);
			}
		}else{
			tex.loadData(pixels);
		}
	}
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::isUsingTexture() const{
	return bUseTexture;
}

//------------------------------------
template<>
void ofImage_<unsigned char>::grabScreen(int x, int y, int w, int h){
	shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->saveScreen(x,y,w,h,pixels);
		update();
	}
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::grabScreen(int x, int y, int w, int h){
	ofPixels p;
	shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->saveScreen(x,y,w,h,p);
		pixels = p;
		update();
	}
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

	otherImage.pixels.cropTo(pixels, x, y, w, h);
	update();
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::rotate90(int nRotations){
	pixels.rotate90(nRotations);
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
}

//----------------------------------------------------------
template<typename PixelType>
float ofImage_<PixelType>::getHeight() const{
	return height;
}

//----------------------------------------------------------
template<typename PixelType>
float ofImage_<PixelType>::getWidth() const{
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


