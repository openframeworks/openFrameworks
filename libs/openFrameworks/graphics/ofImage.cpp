#include "ofImage.h"
#include "ofAppRunner.h"
#include "ofTypes.h"
#include "ofURLFileLoader.h"

//----------------------------------------------------------
// static variable for freeImage initialization:
void ofInitFreeImage(bool deinit){
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

//----------------------------------------------------
FIBITMAP *  getBmpFromPixels(ofPixels &pix){

	FIBITMAP * bmp = NULL;

	int w						= pix.getWidth();
	int h						= pix.getHeight();
	unsigned char * pixels		= pix.getPixels();
	int bpp						= pix.getBitsPerPixel();
	int bytesPerPixel			= pix.getBytesPerPixel();

	bmp							= FreeImage_ConvertFromRawBits(pixels, w,h, w*bytesPerPixel, bpp, 0,0,0, true);

	//this is for grayscale images they need to be paletted from: http://sourceforge.net/forum/message.php?msg_id=2856879
	if( pix.getImageType() == OF_IMAGE_GRAYSCALE ){
		RGBQUAD *pal = FreeImage_GetPalette(bmp);
		for(int i = 0; i < 256; i++) {
			pal[i].rgbRed = i;
			pal[i].rgbGreen = i;
			pal[i].rgbBlue = i;
		}
	}

	return bmp;
}

//----------------------------------------------------
FIBITMAP *  getBmpFromPixels(ofFloatPixels &pix){
/*  TODO:
	FIBITMAP * bmp = NULL;

	int w						= pix.getWidth();
	int h						= pix.getHeight();
	unsigned char * pixels		= pix.getPixels();
	int bpp						= pix.getBitsPerPixel();
	int bytesPerPixel			= pix.getBytesPerPixel();

	bmp							= FreeImage_ConvertFromRawBits(pixels, w,h, w*bytesPerPixel, bpp, 0,0,0, true);

	//this is for grayscale images they need to be paletted from: http://sourceforge.net/forum/message.php?msg_id=2856879
	if( pix.getImageType() == OF_IMAGE_GRAYSCALE ){
		RGBQUAD *pal = FreeImage_GetPalette(bmp);
		for(int i = 0; i < 256; i++) {
			pal[i].rgbRed = i;
			pal[i].rgbGreen = i;
			pal[i].rgbBlue = i;
		}
	}

	return bmp;*/
	return NULL;
}

//----------------------------------------------------
void putBmpIntoPixels(FIBITMAP * bmp, ofPixels &pix, bool swapForLittleEndian = true){
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

//----------------------------------------------------
void putBmpIntoPixels(FIBITMAP * bmp, ofFloatPixels &pix, bool swapForLittleEndian = true){
	//TODO
	/*int width			= FreeImage_GetWidth(bmp);
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
	#endif*/
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

bool ofLoadImage(ofFloatPixels & pix, const ofBuffer & buffer){
	return loadImage(pix,buffer);
}


//----------------------------------------------------------------
bool ofLoadImage(ofTexture & tex, string path){
	ofPixels pixels;
	bool loaded = ofLoadImage(pixels,path);
	if(loaded){
		tex.allocate(pixels.getWidth(),pixels.getHeight(),pixels.getGlDataType());
		tex.loadData(pixels);
	}
	return loaded;
}

//----------------------------------------------------------------
bool ofLoadImage(ofTexture & tex, const ofBuffer & buffer){
	ofPixels pixels;
	bool loaded = ofLoadImage(pixels,buffer);
	if(loaded){
		tex.allocate(pixels.getWidth(),pixels.getHeight(),pixels.getGlDataType());
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

void ofSaveImage(ofPixels & pix, ofBuffer & buffer, ofImageQualityType qualityLevel) {
	ofLog(OF_LOG_ERROR, "ofSaveImage(pix, buffer) is not yet implemented");
}


//----------------------------------------------------
// freeImage based stuff:
void ofCloseFreeImage(){
	ofInitFreeImage(true);
}




