#include "ofImage.h"
#include "ofAppRunner.h"
#include "FreeImage.h"

#include "ofURLFileLoader.h"
#include "uriparser/Uri.h"

#if defined(TARGET_ANDROID)
#include "ofxAndroidUtils.h"
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
		*bFreeImageInited = false;
	}
}

template <typename T>
FREE_IMAGE_TYPE getFreeImageType(const ofPixels_<T>& pix);

template <>
FREE_IMAGE_TYPE getFreeImageType(const ofPixels& pix) {
	return FIT_BITMAP;
}

template <>
FREE_IMAGE_TYPE getFreeImageType(const ofShortPixels& pix) {
	switch(pix.getNumChannels()) {
		case 1: return FIT_UINT16;
		case 3: return FIT_RGB16;
		case 4: return FIT_RGBA16;
		default:
			ofLogError("ofImage") << "getFreeImageType(): unknown FreeImage type for number of channels: " << pix.getNumChannels();
			return FIT_UNKNOWN;
	}
}
template <>
FREE_IMAGE_TYPE getFreeImageType(const ofFloatPixels& pix) {
	switch(pix.getNumChannels()) {
		case 1: return FIT_FLOAT;
		case 3: return FIT_RGBF;
		case 4: return FIT_RGBAF;
		default:
			ofLogError("ofImage") << "getFreeImageType(): unknown FreeImage type for number of channels: " << pix.getNumChannels();
			return FIT_UNKNOWN;
	}
}

//----------------------------------------------------
template<typename PixelType>
FIBITMAP* getBmpFromPixels(const ofPixels_<PixelType> &pix){
	const PixelType* pixels = pix.getData();
	unsigned int width = pix.getWidth();
	unsigned int height = pix.getHeight();
    unsigned int bpp = pix.getBitsPerPixel();

	FREE_IMAGE_TYPE freeImageType = getFreeImageType(pix);
	FIBITMAP* bmp = FreeImage_AllocateT(freeImageType, width, height, bpp);
	unsigned char* bmpBits = FreeImage_GetBits(bmp);
	if(bmpBits != nullptr) {
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
void putBmpIntoPixels(FIBITMAP * bmp, ofPixels_<PixelType>& pix, bool swapOnLittleEndian = true, bool bUsePassedPixelFormat = false) {

	// convert to correct type depending on type of input bmp and PixelType
	FIBITMAP* bmpConverted = nullptr;
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
#ifdef TARGET_LITTLE_ENDIAN
    bool swapRG = channels && swapOnLittleEndian && (bpp/channels == 8);
#else
    bool swapRG = false;
#endif


	ofPixelFormat pixFormat;
    if( bUsePassedPixelFormat ){
        pixFormat = pix.getPixelFormat();
    }else{
        if(channels==1) pixFormat=OF_PIXELS_GRAY;
        if(swapRG){
            if(channels==3) pixFormat=OF_PIXELS_BGR;
            if(channels==4) pixFormat=OF_PIXELS_BGRA;
        }else{
            if(channels==3) pixFormat=OF_PIXELS_RGB;
            if(channels==4) pixFormat=OF_PIXELS_RGBA;
        }
    }
    
	// ofPixels are top left, FIBITMAP is bottom left
	FreeImage_FlipVertical(bmp);

	unsigned char* bmpBits = FreeImage_GetBits(bmp);
	if(bmpBits != nullptr) {
		pix.setFromAlignedPixels((PixelType*) bmpBits, width, height, pixFormat, pitch);
	} else {
		ofLogError("ofImage") << "putBmpIntoPixels(): unable to set ofPixels from FIBITMAP";
	}

	if(bmpConverted != nullptr) {
		FreeImage_Unload(bmpConverted);
	}

    if(swapRG && channels >=3 ) {
		pix.swapRgb();
    }
}

/// internal
static int getJpegOptionFromImageLoadSetting(const ofImageLoadSettings &settings) {
	int option = 0;
	if(settings.accurate)     option |= JPEG_ACCURATE;
	if(settings.exifRotate)   option |= JPEG_EXIFROTATE;
	if(settings.grayscale)    option |= JPEG_GREYSCALE;
	if(settings.separateCMYK) option |= JPEG_CMYK;
	return option;
}

template<typename PixelType>
static bool loadImage(ofPixels_<PixelType> & pix, const std::filesystem::path& _fileName, const ofImageLoadSettings& settings){
	ofInitFreeImage();

	auto uriStr = _fileName.string();
	UriUriA uri;
	UriParserStateA state;
	state.uri = &uri;

	if(uriParseUriA(&state, uriStr.c_str())!=URI_SUCCESS){
		const int bytesNeeded = 8 + 3 * strlen(uriStr.c_str()) + 1;
		std::vector<char> absUri(bytesNeeded);
	#ifdef TARGET_WIN32
		uriWindowsFilenameToUriStringA(uriStr.c_str(), absUri.data());
	#else
		uriUnixFilenameToUriStringA(uriStr.c_str(), absUri.data());
	#endif
		if(uriParseUriA(&state, absUri.data())!=URI_SUCCESS){
			ofLogError("ofImage") << "loadImage(): malformed uri when loading image from uri " << _fileName;
			uriFreeUriMembersA(&uri);
			return false;
		}
	}
	std::string scheme(uri.scheme.first, uri.scheme.afterLast);
	uriFreeUriMembersA(&uri);

	if(scheme == "http" || scheme == "https"){
		return ofLoadImage(pix, ofLoadURL(_fileName.string()).data);
	}

	std::string fileName = ofToDataPath(_fileName, true);
	bool bLoaded = false;
	FIBITMAP * bmp = nullptr;

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if(fif == FIF_UNKNOWN) {
		// or guess via filename
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	}
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		if(fif == FIF_JPEG) {
			int option = getJpegOptionFromImageLoadSetting(settings);
			bmp = FreeImage_Load(fif, fileName.c_str(), option | settings.freeImageFlags);
		} else {
			bmp = FreeImage_Load(fif, fileName.c_str(), 0 | settings.freeImageFlags);
		}

		if (bmp != nullptr){
			bLoaded = true;
		}
	}

	//-----------------------------

	if ( bLoaded ){
		putBmpIntoPixels(bmp,pix);
	}

	if (bmp != nullptr){
		FreeImage_Unload(bmp);
	}

	return bLoaded;
}

template<typename PixelType>
static bool loadImage(ofPixels_<PixelType> & pix, const ofBuffer & buffer, const ofImageLoadSettings &settings){
	ofInitFreeImage();
	bool bLoaded = false;
	FIBITMAP* bmp = nullptr;
	FIMEMORY* hmem = nullptr;

	hmem = FreeImage_OpenMemory((unsigned char*) buffer.getData(), buffer.size());
	if (hmem == nullptr){
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
	if(fif == FIF_JPEG) {
		int option = getJpegOptionFromImageLoadSetting(settings);
		bmp = FreeImage_LoadFromMemory(fif, hmem, option | settings.freeImageFlags);
	} else {
		bmp = FreeImage_LoadFromMemory(fif, hmem, settings.freeImageFlags);
	}

	if( bmp != nullptr ){
		bLoaded = true;
	}

	//-----------------------------

	if (bLoaded){
		putBmpIntoPixels(bmp,pix);
	}

	if (bmp != nullptr){
		FreeImage_Unload(bmp);
	}

	if( hmem != nullptr ){
		FreeImage_CloseMemory(hmem);
	}

	return bLoaded;
}

//----------------------------------------------------------------
bool ofLoadImage(ofPixels & pix, const std::filesystem::path& path, const ofImageLoadSettings &settings) {
	return loadImage(pix, path, settings);
}

//----------------------------------------------------------------
bool ofLoadImage(ofPixels & pix, const ofBuffer & buffer, const ofImageLoadSettings &settings) {
	return loadImage(pix, buffer, settings);
}

//----------------------------------------------------------------
bool ofLoadImage(ofShortPixels & pix, const std::filesystem::path& path, const ofImageLoadSettings &settings) {
	return loadImage(pix, path, settings);
}

//----------------------------------------------------------------
bool ofLoadImage(ofShortPixels & pix, const ofBuffer & buffer, const ofImageLoadSettings &settings) {
	return loadImage(pix, buffer, settings);
}

//----------------------------------------------------------------
bool ofLoadImage(ofFloatPixels & pix, const std::filesystem::path& path, const ofImageLoadSettings &settings) {
	return loadImage(pix, path, settings);
}

//----------------------------------------------------------------
bool ofLoadImage(ofFloatPixels & pix, const ofBuffer & buffer, const ofImageLoadSettings &settings) {
	return loadImage(pix, buffer, settings);
}

//----------------------------------------------------------------
bool ofLoadImage(ofTexture & tex, const std::filesystem::path& path, const ofImageLoadSettings &settings){
	ofPixels pixels;
	bool loaded = ofLoadImage(pixels, path, settings);
	if(loaded){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGLInternalFormat(pixels));
		tex.loadData(pixels);
	}
	return loaded;
}

//----------------------------------------------------------------
bool ofLoadImage(ofTexture & tex, const ofBuffer & buffer, const ofImageLoadSettings &settings){
	ofPixels pixels;
	bool loaded = ofLoadImage(pixels, buffer, settings);
	if(loaded){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), ofGetGLInternalFormat(pixels));
		tex.loadData(pixels);
	}
	return loaded;
}

//----------------------------------------------------------------
template<typename PixelType>
static bool saveImage(const ofPixels_<PixelType> & _pix, const std::filesystem::path& _fileName, ofImageQualityType qualityLevel) {
	ofInitFreeImage();
	if (_pix.isAllocated() == false){
		ofLogError("ofImage") << "saveImage(): couldn't save \"" << _fileName << "\", pixels are not allocated";
		return false;
	}

	ofFilePath::createEnclosingDirectory(_fileName);
	std::string fileName = ofToDataPath(_fileName);
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if(fif == FIF_UNKNOWN) {
		// or guess via filename
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	}
	if(fif==FIF_JPEG && (_pix.getNumChannels()==4 || _pix.getBitsPerChannel() > 8)){
		ofPixels pix3 = _pix;
        if( pix3.getPixelFormat() == OF_PIXELS_BGRA ){
            pix3.swapRgb();
        }
		pix3.setNumChannels(3);
		return saveImage(pix3,_fileName,qualityLevel);
	}

	FIBITMAP * bmp = nullptr;
	#ifdef TARGET_LITTLE_ENDIAN
	if(sizeof(PixelType) == 1 && (_pix.getPixelFormat()==OF_PIXELS_RGB || _pix.getPixelFormat()==OF_PIXELS_RGBA)) {	// Make a local copy.
		ofPixels_<PixelType> pix = _pix;
		pix.swapRgb();
		bmp	= getBmpFromPixels(pix);
	}else{
	#endif

		bmp	= getBmpFromPixels(_pix);


	#ifdef TARGET_LITTLE_ENDIAN
	}
	#endif

	bool retValue = false;
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
			retValue = FreeImage_Save(fif, bmp, fileName.c_str(), quality);
		} else {
			if(qualityLevel != OF_IMAGE_QUALITY_BEST) {
				ofLogWarning("ofImage") << "saveImage(): ofImageCompressionType only applies to JPEGs,"
					<< " ignoring value for \" "<< fileName << "\"";
			}

			if (fif == FIF_GIF) {
				FIBITMAP* convertedBmp;
				if(_pix.getImageType() == OF_IMAGE_COLOR_ALPHA) {
					// this just converts the image to grayscale so it can save something
					convertedBmp = FreeImage_ConvertTo8Bits(bmp);
				} else {
					// this will create a 256-color palette from the image
					convertedBmp = FreeImage_ColorQuantize(bmp, FIQ_NNQUANT);
				}
				retValue = FreeImage_Save(fif, convertedBmp, fileName.c_str());
				if (convertedBmp != nullptr){
					FreeImage_Unload(convertedBmp);
				}
			} else {
				retValue = FreeImage_Save(fif, bmp, fileName.c_str());
			}
		}
	}

	if (bmp != nullptr){
		FreeImage_Unload(bmp);
	}

	return retValue;
}

//----------------------------------------------------------------
bool ofSaveImage(const ofPixels & pix, const std::filesystem::path& fileName, ofImageQualityType qualityLevel){
	return saveImage(pix,fileName,qualityLevel);
}

//----------------------------------------------------------------
bool ofSaveImage(const ofFloatPixels & pix, const std::filesystem::path& fileName, ofImageQualityType qualityLevel) {
	return saveImage(pix,fileName,qualityLevel);
}

//----------------------------------------------------------------
bool ofSaveImage(const ofShortPixels & pix, const std::filesystem::path& fileName, ofImageQualityType qualityLevel) {
	return saveImage(pix,fileName,qualityLevel);
}

//----------------------------------------------------------------
template<typename PixelType>
static bool saveImage(const ofPixels_<PixelType> & _pix, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	// thanks to alvaro casinelli for the implementation

	ofInitFreeImage();

	if (_pix.isAllocated() == false){
		ofLogError("ofImage","saveImage(): couldn't save to ofBuffer, pixels are not allocated");
		return false;
	}

	if(format==OF_IMAGE_FORMAT_JPEG && (_pix.getNumChannels()==4 || _pix.getBitsPerChannel() > 8)){
		ofPixels pix3 = _pix;
        if( pix3.getPixelFormat() == OF_PIXELS_BGRA ){
            pix3.swapRgb();
        }
		pix3.setNumChannels(3);
		return saveImage(pix3,buffer,format,qualityLevel);
	}


	FIBITMAP * bmp = nullptr;
	#ifdef TARGET_LITTLE_ENDIAN
	if(sizeof(PixelType) == 1 && (_pix.getPixelFormat()==OF_PIXELS_RGB || _pix.getPixelFormat()==OF_PIXELS_RGBA)) {	// Make a local copy.
		ofPixels_<PixelType> pix = _pix;
		pix.swapRgb();
		bmp	= getBmpFromPixels(pix);
	}else{
	#endif

		bmp	= getBmpFromPixels(_pix);


	#ifdef TARGET_LITTLE_ENDIAN
	}
	#endif

	if (bmp)  // bitmap successfully created
	{
		bool returnValue;
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
			returnValue = FreeImage_SaveToMemory(FIF_JPEG, bmp, hmem, quality);
		}else{
			returnValue = FreeImage_SaveToMemory((FREE_IMAGE_FORMAT)format, bmp, hmem);
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
		   std::uint32_t size_in_bytes = 0;
		#endif
		// Save compressed data on mem_buffer
		// note: FreeImage_AquireMemory allocates space for aux_mem_buffer):
		//
		unsigned char *mem_buffer = nullptr;
		if (!FreeImage_AcquireMemory(hmem, &mem_buffer, &size_in_bytes)){
			ofLogError("ofImage") << "saveImage(): couldn't save to ofBuffer, aquiring compressed image from memory failed";
			return false;
		}

		/*
		  Now, before closing the memory stream, copy the content of mem_buffer
		  to an auxiliary buffer
		*/

		buffer.set((char*)mem_buffer,size_in_bytes);

		// Finally, close the FIBITMAP object, or we will get a memory leak:
		FreeImage_Unload(bmp);

		// Close the memory stream (otherwise we may get a memory leak).
		FreeImage_CloseMemory(hmem);
		return returnValue;
	}else{
		return false;
	}
}

//----------------------------------------------------------------
bool ofSaveImage(const ofPixels & pix, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	return saveImage(pix,buffer,format,qualityLevel);
}

bool ofSaveImage(const ofFloatPixels & pix, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	return saveImage(pix,buffer,format,qualityLevel);
}

bool ofSaveImage(const ofShortPixels & pix, ofBuffer & buffer, ofImageFormat format, ofImageQualityType qualityLevel) {
	return saveImage(pix,buffer,format,qualityLevel);
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
ofImage_<PixelType>::ofImage_(const std::filesystem::path & fileName, const ofImageLoadSettings &settings){
	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	//----------------------- init free image if necessary
	ofInitFreeImage();


	load(fileName, settings);
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>& ofImage_<PixelType>::operator=(const ofImage_<PixelType>& mom) {
	if(&mom==this) return *this;
	clone(mom);

	#if defined(TARGET_ANDROID)
	ofAddListener(ofxAndroidEvents().unloadGL,this,&ofImage_<PixelType>::unloadTexture);
	ofAddListener(ofxAndroidEvents().reloadGL,this,&ofImage_<PixelType>::update);
	#endif
	return *this;
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::ofImage_(const ofImage_<PixelType>& mom) {
	clear();
	clone(mom);

	#if defined(TARGET_ANDROID)
	ofAddListener(ofxAndroidEvents().unloadGL,this,&ofImage_<PixelType>::unloadTexture);
	ofAddListener(ofxAndroidEvents().reloadGL,this,&ofImage_<PixelType>::update);
	#endif
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::ofImage_(ofImage_<PixelType>&& mom){
    pixels      = std::move(mom.pixels);
    tex         = std::move(mom.tex);

    bUseTexture = mom.bUseTexture;
    width       = mom.width;
    height      = mom.height;
    bpp         = mom.bpp;
    type        = mom.type;

    mom.clear(); //clear remaining flags and sizes from the mom

    #if defined(TARGET_ANDROID)
    ofAddListener(ofxAndroidEvents().unloadGL,this,&ofImage_<PixelType>::unloadTexture);
    ofAddListener(ofxAndroidEvents().reloadGL,this,&ofImage_<PixelType>::update);
    #endif
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>& ofImage_<PixelType>::operator=(ofImage_<PixelType>&& mom){
    if(&mom==this) return *this;

    pixels      = std::move(mom.pixels);
    tex         = std::move(mom.tex);

    bUseTexture = mom.bUseTexture;
    width       = mom.width;
    height      = mom.height;
    bpp         = mom.bpp;
    type        = mom.type;

    mom.clear(); //clear remaining flags and sizes from the mom

    #if defined(TARGET_ANDROID)
    ofAddListener(ofxAndroidEvents().unloadGL,this,&ofImage_<PixelType>::unloadTexture);
    ofAddListener(ofxAndroidEvents().reloadGL,this,&ofImage_<PixelType>::update);
    #endif

    return *this;
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::~ofImage_(){
	clear();
}

//----------------------------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::loadImage(const ofFile & file){
	return load(file);
}

//----------------------------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::load(const std::filesystem::path& fileName, const ofImageLoadSettings &settings){
	#if defined(TARGET_ANDROID)
	ofAddListener(ofxAndroidEvents().unloadGL,this,&ofImage_<PixelType>::unloadTexture);
	ofAddListener(ofxAndroidEvents().reloadGL,this,&ofImage_<PixelType>::update);
	#endif
	bool bLoadedOk = ofLoadImage(pixels, fileName, settings);
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
bool ofImage_<PixelType>::loadImage(const std::string& fileName){
	return load(fileName);
}

//----------------------------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::load(const ofBuffer & buffer, const ofImageLoadSettings &settings){
	#if defined(TARGET_ANDROID)
	ofAddListener(ofxAndroidEvents().unloadGL,this,&ofImage_<PixelType>::unloadTexture);
	ofAddListener(ofxAndroidEvents().reloadGL,this,&ofImage_<PixelType>::update);
	#endif
	bool bLoadedOk = ofLoadImage(pixels, buffer, settings);
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
bool ofImage_<PixelType>::save(const std::filesystem::path& fileName, ofImageQualityType qualityLevel) const {
	return ofSaveImage(pixels, fileName, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::save(ofBuffer & buffer, ofImageFormat imageFormat, ofImageQualityType qualityLevel) const {
	return ofSaveImage(pixels, buffer, imageFormat, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::saveImage(const std::string& fileName, ofImageQualityType qualityLevel) const {
	save(fileName, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::saveImage(ofBuffer & buffer, ofImageQualityType qualityLevel) const {
    save(buffer, OF_IMAGE_FORMAT_PNG, qualityLevel);
}

//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::saveImage(const ofFile & file, ofImageQualityType compressionLevel) const {
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
void ofImage_<PixelType>::draw(const glm::vec3 & pos) const{
	draw(pos.x,pos.y,pos.z,getWidth(),getHeight());
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
void ofImage_<PixelType>::draw(const glm::vec3 & pos, float w, float h) const{
	draw(pos.x,pos.y,pos.z,w,h);
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
	ofAddListener(ofxAndroidEvents().unloadGL,this,&ofImage_<PixelType>::unloadTexture);
	ofAddListener(ofxAndroidEvents().reloadGL,this,&ofImage_<PixelType>::update);
#endif
	pixels.allocate(w, h, newType);

	// take care of texture allocation --
	if (pixels.isAllocated() && bUseTexture){
		tex.allocate(pixels);
	}

	width	= pixels.getWidth();
	height	= pixels.getHeight();
	bpp		= pixels.getBitsPerPixel();
	type	= pixels.getImageType();
}


//------------------------------------
template<typename PixelType>
bool ofImage_<PixelType>::bAllocated(){
    return pixels.isAllocated();
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::clear(){
#if defined(TARGET_ANDROID)
	ofRemoveListener(ofxAndroidEvents().unloadGL,this,&ofImage_<PixelType>::unloadTexture);
	ofRemoveListener(ofxAndroidEvents().reloadGL,this,&ofImage_<PixelType>::update);
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
ofColor_<PixelType> ofImage_<PixelType>::getColor(int index) const {
	return pixels.getColor(index);
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
		int glInternalFormat = ofGetGLInternalFormat(pixels);
		if(!tex.isAllocated() || tex.getWidth() != width || tex.getHeight() != height || tex.getTextureData().glInternalFormat != glInternalFormat){
			tex.allocate(pixels);
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
	std::shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->saveScreen(x,y,w,h,pixels);
		update();
	}
}

//------------------------------------
template<typename PixelType>
void ofGrabScreen(ofPixels_<PixelType> & pixels, int x, int y, int w, int h){
	ofPixels p;
	std::shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->saveScreen(x,y,w,h,p);
		pixels = p;
	}
}

//------------------------------------
template<>
void ofGrabScreen(ofPixels & p, int x, int y, int w, int h){
	std::shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->saveScreen(x,y,w,h,p);
	}
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::grabScreen(int x, int y, int w, int h){
	ofGrabScreen(pixels,x,y,w,h);
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
ofImageType ofImage_<PixelType>::getImageType() const{
	return type;
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
	w = glm::clamp(w,1,int(getWidth()));
	h = glm::clamp(h,1,int(getHeight()));

	pixels.crop(x,y,w,h);
	update();
}

//------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::cropFrom(const ofImage_<PixelType> & otherImage, int x, int y, int w, int h){
	w = glm::clamp(w,1,int(otherImage.getWidth()));
	h = glm::clamp(h,1,int(otherImage.getHeight()));

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
	FIBITMAP * convertedBmp			= nullptr;

	convertedBmp = FreeImage_Rescale(bmp, newWidth, newHeight, FILTER_BICUBIC);
    putBmpIntoPixels(convertedBmp, pix, false, true);

	if (bmp != nullptr)				FreeImage_Unload(bmp);
	if (convertedBmp != nullptr)		FreeImage_Unload(convertedBmp);
}

//----------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::changeTypeOfPixels(ofPixels_<PixelType> &pix, ofImageType newType){
	int oldType = pix.getImageType();

	if (oldType == newType) {
		return; // no need to reallocate
	}

	FIBITMAP * bmp = getBmpFromPixels(pix);
	FIBITMAP * convertedBmp = nullptr;

    ofPixelFormat oldPixFormat = pix.getPixelFormat();

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
			ofLogError("ofImage") << "changeTypeOfPixels(): unknown image type: "
				<< ofToString(newType);
			break;
	}

    putBmpIntoPixels(convertedBmp, pix, false);
    
    // if we started with BGRA or BGR pixels make sure we end up with similar
    if( pix.getNumChannels() >= 3 && ( oldPixFormat == OF_PIXELS_BGR || oldPixFormat == OF_PIXELS_BGRA ) ){
        ofPixelFormat fixedFormat = pix.getPixelFormat();
        if( fixedFormat == OF_PIXELS_RGBA ){
            fixedFormat = OF_PIXELS_BGRA;
        }else if( fixedFormat == OF_PIXELS_RGB ){
            fixedFormat = OF_PIXELS_BGR;
        }
        pix.setFromPixels(pix.getData(),pix.getWidth(),pix.getHeight(), fixedFormat);
    }

	if (bmp != nullptr) {
		FreeImage_Unload(bmp);
	}
	if (convertedBmp != nullptr) {
		FreeImage_Unload(convertedBmp);
	}
}


//----------------------------------------------------------
template<typename PixelType>
void ofImage_<PixelType>::unloadTexture(){
	tex.clear();
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

template<>
std::string ofToString(const ofImageType & imgType){
	switch(imgType) {
		case OF_IMAGE_GRAYSCALE:
			return "OF_IMAGE_GRAYSCALE";
		break;
		case OF_IMAGE_COLOR:
			return "OF_IMAGE_COLOR";
		break;
		case  OF_IMAGE_COLOR_ALPHA:
			return "OF_IMAGE_COLOR_ALPHA";
		break;
		case OF_IMAGE_UNDEFINED:
			return "OF_IMAGE_UNDEFINED";
		break;
	}
	return "OF_IMAGE_UNDEFINED";
}
