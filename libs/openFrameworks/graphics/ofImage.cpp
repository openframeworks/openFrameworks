#include "ofImage.h"


//----------------------------------------------------------
// static variable for freeImage initialization:
static bool		bFreeImageInited = false;
//----------------------------------------------------------



//----------------------------------------------------------
ofImage::ofImage(){

	myPixels.width				= 0;
	myPixels.height				= 0;
	myPixels.bitsPerPixel		= 0;
	myPixels.bytesPerPixel		= 0;
	myPixels.glDataType			= GL_LUMINANCE;
	myPixels.ofImageType		= OF_IMAGE_UNDEFINED;
	myPixels.bAllocated			= false;

	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	//----------------------- init free image if necessary
	if (!bFreeImageInited){
		FreeImage_Initialise();
		bFreeImageInited = true;
	}
}

//----------------------------------------------------------
ofImage& ofImage::operator=(const ofImage& mom) {
	clone(mom);
	update();
	return *this;
}

//----------------------------------------------------------
ofImage::ofImage(const ofImage& mom) {
	myPixels.bAllocated			= false;

	if (!bFreeImageInited){
		FreeImage_Initialise();
		bFreeImageInited = true;
	}

	clear();
	clone(mom);
	update();
};

//----------------------------------------------------------
ofImage::~ofImage(){
	clear();
}

//----------------------------------------------------------
bool ofImage::loadImage(string fileName){
	bool bLoadedOk = false;
	bLoadedOk = loadImageIntoPixels(fileName, myPixels);

	if (bLoadedOk == true){
	if (myPixels.bAllocated == true && bUseTexture == true){
		tex.allocate(myPixels.width, myPixels.height, myPixels.glDataType);
	}
	update();
}

	return bLoadedOk;
}

//----------------------------------------------------------
void ofImage::saveImage(string fileName){
	saveImageFromPixels(fileName, myPixels);
}

//we could cap these values - but it might be more useful
//to be able to set anchor points outside the image

//----------------------------------------------------------
void ofImage::setAnchorPercent(float xPct, float yPct){
    if (bUseTexture)tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofImage::setAnchorPoint(float x, float y){
    if (bUseTexture)tex.setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofImage::resetAnchor(){
   	if (bUseTexture)tex.resetAnchor();
}

//------------------------------------
void ofImage::draw(float _x, float _y, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofImage::draw(float x, float y){
	draw(x,y,myPixels.width,myPixels.height);
}

//------------------------------------
void ofImage::allocate(int w, int h, int type){


	int newBpp = 0;

	switch (type){
		case OF_IMAGE_GRAYSCALE:
			newBpp = 8;
			break;
		case OF_IMAGE_COLOR:
			newBpp = 24;
			break;
		case OF_IMAGE_COLOR_ALPHA:
			newBpp = 32;
			break;
		default:
			ofLog(OF_LOG_ERROR,"error = bad imageType in ofImage::allocate");
			return;
	}

	allocatePixels(myPixels, w, h, newBpp);

	// take care of texture allocation --
	if (myPixels.bAllocated == true && bUseTexture == true){
		tex.allocate(myPixels.width, myPixels.height, myPixels.glDataType);
	}

	update();
}


//------------------------------------
void ofImage::clear(){

	if (myPixels.bAllocated == true){
		delete[] myPixels.pixels;
	}
	if(bUseTexture)	tex.clear();

	myPixels.width			= 0;
	myPixels.height			= 0;
	myPixels.bitsPerPixel	= 0;
	myPixels.bytesPerPixel	= 0;
	myPixels.glDataType		= GL_LUMINANCE;
	myPixels.ofImageType	= OF_IMAGE_UNDEFINED;
	myPixels.bAllocated		= false;

	width					= 0;
	height					= 0;
	bpp						= 0;
	type 					= OF_IMAGE_UNDEFINED;
	bUseTexture 			= true;		// the default is, yes, use a texture
}

//------------------------------------
unsigned char * ofImage::getPixels(){
	return myPixels.pixels;
}

//------------------------------------
//for getting a reference to the texture
ofTexture & ofImage::getTextureReference(){
	if(!tex.bAllocated() ){
		ofLog(OF_LOG_WARNING, "ofImage - getTextureReference - texture is not allocated");
	}
	return tex;
}

//------------------------------------
void  ofImage::setFromPixels(unsigned char * newPixels, int w, int h, int newType, bool bOrderIsRGB){

	if (!myPixels.bAllocated){
		allocate(w, h, newType);
	}

	if (!((width == w) && (height == h) && (type == newType))){
		clear();
		allocate(w,h, newType);
	}


	int newBpp = 0;
	switch (type){
		case OF_IMAGE_GRAYSCALE:
			newBpp = 8;
			break;
		case OF_IMAGE_COLOR:
			newBpp = 24;
			break;
		case OF_IMAGE_COLOR_ALPHA:
			newBpp = 32;
			break;
		default:
			ofLog(OF_LOG_ERROR,"error = bad imageType in ofImage::setFromPixels");
			return;
	}

	allocatePixels(myPixels, w, h, newBpp);
	int bytesPerPixel = myPixels.bitsPerPixel / 8;
	memcpy(myPixels.pixels, newPixels, w*h*bytesPerPixel);

	if (myPixels.bytesPerPixel > 1){
		if (!bOrderIsRGB){
			swapRgb(myPixels);
		}
	}

	update();
}

//------------------------------------
void ofImage::update(){

	if (myPixels.bAllocated == true && bUseTexture == true){
		tex.loadData(myPixels.pixels, myPixels.width, myPixels.height, myPixels.glDataType);
	}

	width	= myPixels.width;
	height	= myPixels.height;
	bpp		= myPixels.bitsPerPixel;
	type	= myPixels.ofImageType;
}

//------------------------------------
void ofImage::setUseTexture(bool bUse){
	bUseTexture = bUse;
}


//------------------------------------
void ofImage::grabScreen(int _x, int _y, int _w, int _h){

	if (!myPixels.bAllocated){
		allocate(_w, _h, OF_IMAGE_COLOR);
	}

	int screenHeight = ofGetHeight();
	_y = screenHeight - _y;
	_y -= _h; // top, bottom issues

	if (!((width == _w) && (height == _h))){
		resize(_w, _h);
	}

	#ifndef TARGET_OF_IPHONE
		glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );											// be nice to anyone else who might use pixelStore
	#endif
		glPixelStorei(GL_PACK_ALIGNMENT, 1);														// set read non block aligned...
		glReadPixels(_x, _y, _w, _h, myPixels.glDataType,GL_UNSIGNED_BYTE, myPixels.pixels);		// read the memory....
	#ifndef TARGET_OF_IPHONE
		glPopClientAttrib();
	#endif

	int sizeOfOneLineOfPixels = myPixels.width * myPixels.bytesPerPixel;
	unsigned char * tempLineOfPix = new unsigned char[sizeOfOneLineOfPixels];
	unsigned char * linea;
	unsigned char * lineb;
	for (int i = 0; i < myPixels.height/2; i++){
		linea = myPixels.pixels + i * sizeOfOneLineOfPixels;
		lineb = myPixels.pixels + (myPixels.height-i-1) * sizeOfOneLineOfPixels;
		memcpy(tempLineOfPix, linea, sizeOfOneLineOfPixels);
		memcpy(linea, lineb, sizeOfOneLineOfPixels);
		memcpy(lineb, tempLineOfPix, sizeOfOneLineOfPixels);
	}
	delete [] tempLineOfPix;
	update();
}


//------------------------------------
void ofImage::clone(const ofImage &mom){

	allocatePixels(myPixels, mom.width, mom.height, mom.bpp);
	memcpy(myPixels.pixels, mom.myPixels.pixels, myPixels.width*myPixels.height*myPixels.bytesPerPixel);

	tex.clear();
	bUseTexture = mom.bUseTexture;
	if (bUseTexture == true){
		tex.allocate(myPixels.width, myPixels.height, myPixels.glDataType);
	}

	update();
}

//------------------------------------
void ofImage::setImageType(int newType){
	changeTypeOfPixels(myPixels, newType);
	update();
}

//------------------------------------
void ofImage::resize(int newWidth, int newHeight){
	resizePixels(myPixels, newWidth, newHeight);

	if (bUseTexture == true){
		tex.clear();
		tex.allocate(myPixels.width, myPixels.height, myPixels.glDataType);
	}

	update();
}


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
// freeImage based code & utilities:

//----------------------------------------------------
inline void ofImage::swapRgb(ofPixels &pix){
	if (pix.bitsPerPixel != 8){
		int sizePixels		= pix.width*pix.height;
		int cnt				= 0;
		unsigned char temp;
		int byteCount		= pix.bitsPerPixel/8;

		while (cnt < sizePixels){
			temp					= pix.pixels[cnt*byteCount];
			pix.pixels[cnt*byteCount]		= pix.pixels[cnt*byteCount+2];
			pix.pixels[cnt*byteCount+2]		= temp;
			cnt++;
		}
	}
}


//----------------------------------------------------
inline void  ofImage::allocatePixels(ofPixels &pix, int width, int height, int bpp){

	bool bNeedToAllocate = false;
	if (pix.bAllocated == true){
		if ( (pix.width == width) && (pix.height == height) && (pix.bitsPerPixel == bpp)){
			//ofLog(OF_LOG_NOTICE,"we are good, no reallocation needed");
			bNeedToAllocate = false;
		 } else {
			delete[] pix.pixels;
			bNeedToAllocate = true;
		 }
	} else {
		bNeedToAllocate = true;
	}

	int byteCount = bpp / 8;

	if (bNeedToAllocate == true){
		pix.width			= width;
		pix.height			= height;
		pix.bitsPerPixel	= bpp;
		pix.bytesPerPixel	= bpp / 8;
		switch (pix.bitsPerPixel){
			case 8:
				pix.glDataType		= GL_LUMINANCE;
				pix.ofImageType		= OF_IMAGE_GRAYSCALE;
				break;
			case 24:
				pix.glDataType		= GL_RGB;
				pix.ofImageType		= OF_IMAGE_COLOR;
				break;
			case 32:
				pix.glDataType		= GL_RGBA;
				pix.ofImageType		= OF_IMAGE_COLOR_ALPHA;
				break;
		}

		pix.pixels			= new unsigned char[pix.width*pix.height*byteCount];
		pix.bAllocated		= true;
	}
}

//----------------------------------------------------
FIBITMAP *  ofImage::getBmpFromPixels(ofPixels &pix){

	FIBITMAP * bmp = NULL;

	int w						= pix.width;
	int h						= pix.height;
	unsigned char * pixels		= pix.pixels;
	int bpp						= pix.bitsPerPixel;
	int bytesPerPixel			= pix.bitsPerPixel / 8;

	bmp							= FreeImage_ConvertFromRawBits(pixels, w,h, w*bytesPerPixel, bpp, 0,0,0, true);

	//this is for grayscale images they need to be paletted from: http://sourceforge.net/forum/message.php?msg_id=2856879
	if( pix.ofImageType == OF_IMAGE_GRAYSCALE ){
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
void ofImage::putBmpIntoPixels(FIBITMAP * bmp, ofPixels &pix){
	int width			= FreeImage_GetWidth(bmp);
	int height			= FreeImage_GetHeight(bmp);
	int bpp				= FreeImage_GetBPP(bmp);
	int bytesPerPixel	= bpp / 8;
	//------------------------------------------
	// call the allocation routine (which checks if really need to allocate) here:
	allocatePixels(pix, width, height, bpp);
	FreeImage_ConvertToRawBits(pix.pixels, bmp, width*bytesPerPixel, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, true);  // get bits
}

//----------------------------------------------------
void ofImage::resizePixels(ofPixels &pix, int newWidth, int newHeight){

	FIBITMAP * bmp					= getBmpFromPixels(pix);
	FIBITMAP * convertedBmp			= NULL;

	convertedBmp = FreeImage_Rescale(bmp, newWidth, newHeight, FILTER_BICUBIC);
	putBmpIntoPixels(convertedBmp, pix);

	if (bmp != NULL)				FreeImage_Unload(bmp);
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);

}

//----------------------------------------------------
void ofImage::changeTypeOfPixels(ofPixels &pix, int newType){

	
		
	if (pix.ofImageType == newType) return;

	FIBITMAP * bmp					= getBmpFromPixels(pix);
	FIBITMAP * convertedBmp			= NULL;

	// check if we need to reallocate the texture.
	bool bNeedNewTexture = false;
	int oldType = pix.ofImageType;
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
				tex.allocate(myPixels.width, myPixels.height, GL_RGB);
			}
			break;
		
		//------------------------------------
		case OF_IMAGE_COLOR_ALPHA:
			convertedBmp = FreeImage_ConvertTo32Bits(bmp);
			if (bNeedNewTexture){
				tex.clear();
				tex.allocate(myPixels.width, myPixels.height, GL_RGBA);
			}
			break;
	}

	putBmpIntoPixels(convertedBmp, pix);

	if (bmp != NULL)				FreeImage_Unload(bmp);
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);

}

//----------------------------------------------------
// freeImage based stuff:
void ofCloseFreeImage(){
	if (bFreeImageInited){
		FreeImage_DeInitialise();
		bFreeImageInited = false;
	}
}

//----------------------------------------------------
bool ofImage::loadImageIntoPixels(string fileName, ofPixels &pix){

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
		bLoaded = true;
		if (bmp == NULL){
			bLoaded = false;
		}
	}
	//-----------------------------

	if (bLoaded ){

		width 		= FreeImage_GetWidth(bmp);
		height 		= FreeImage_GetHeight(bmp);
		bpp 		= FreeImage_GetBPP(bmp);

		bool bPallette = (FreeImage_GetColorType(bmp) == FIC_PALETTE);

		switch (bpp){
			case 8:
				if (bPallette) {
					FIBITMAP 	* bmpTemp =		FreeImage_ConvertTo24Bits(bmp);
					if (bmp != NULL)			FreeImage_Unload(bmp);
					bmp							= bmpTemp;
					bpp							= FreeImage_GetBPP(bmp);
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
				FIBITMAP 	* bmpTemp =		FreeImage_ConvertTo24Bits(bmp);
				if (bmp != NULL)			FreeImage_Unload(bmp);
				bmp							= bmpTemp;
				bpp							= FreeImage_GetBPP(bmp);
		}


		int byteCount = bpp / 8;

		//------------------------------------------
		// call the allocation routine (which checks if really need to allocate) here:
		allocatePixels(pix, width, height, bpp);



		FreeImage_ConvertToRawBits(pix.pixels, bmp, width*byteCount, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, true);  // get bits

		//------------------------------------------
		// RGB or RGBA swap
		// this can be done with some ill pointer math.
		// anyone game?
		//

		#ifdef TARGET_LITTLE_ENDIAN
			if (byteCount != 1) swapRgb(pix);
		#endif
		//------------------------------------------


	} else {
		width = height = bpp = 0;
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}

	return bLoaded;
}

//----------------------------------------------------------------
void  ofImage::saveImageFromPixels(string fileName, ofPixels &pix){

	if (pix.bAllocated == false){
		ofLog(OF_LOG_ERROR,"error saving image - pixels aren't allocated");
		return;
	}

	#ifdef TARGET_LITTLE_ENDIAN
		if (pix.bytesPerPixel != 1) swapRgb(pix);
	#endif

	FIBITMAP * bmp	= getBmpFromPixels(pix);

	#ifdef TARGET_LITTLE_ENDIAN
		if (pix.bytesPerPixel != 1) swapRgb(pix);
	#endif

	fileName = ofToDataPath(fileName);
	if (pix.bAllocated == true){
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		fif = FreeImage_GetFileType(fileName.c_str(), 0);
		if(fif == FIF_UNKNOWN) {
			// or guess via filename
			fif = FreeImage_GetFIFFromFilename(fileName.c_str());
		}
		if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
			FreeImage_Save(fif, bmp, fileName.c_str(), 0);
		}
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}
}

//----------------------------------------------------------
float ofImage::getHeight(){
	return height;
}

//----------------------------------------------------------
float ofImage::getWidth(){
	return width;
}

//----------------------------------------------------------
// Sosolimited: texture compression
// call this function before you call loadImage()
void ofImage::setCompression(ofTexCompression compression)
{
	tex.setCompression( compression );
}



