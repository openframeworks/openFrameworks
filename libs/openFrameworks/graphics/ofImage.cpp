#include "ofImage.h"
#include "ofAppRunner.h"
#include "ofTypes.h"
#include "ofURLFileLoader.h"
#include "ofGraphics.h"
#include "FreeImage.h"

//----------------------------------------------------------
// static variable for freeImage initialization:
static bool		bFreeImageInited = false;
//----------------------------------------------------------

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

	int bytesPerPixel	= bpp / 8;
	pix.allocate(width, height, bpp);
	FreeImage_ConvertToRawBits(pix.getPixels(), bmp, width*bytesPerPixel, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, true);  // get bits

	if (bmpTemp != NULL) FreeImage_Unload(bmpTemp);

	#ifdef TARGET_LITTLE_ENDIAN
		if(swapForLittleEndian)
			pix.swapRgb();
	#endif
}

//----------------------------------------------------
bool ofLoadImage(ofPixels & pix, string fileName) {
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

//----------------------------------------------------
bool ofLoadImage(ofPixels & pix, const ofBuffer & buffer) {

	int					width, height, bpp;
	bool bLoaded		= false;
	FIBITMAP * bmp		= NULL;
	FIMEMORY *hmem		= NULL;
	
	hmem = FreeImage_OpenMemory((unsigned char*)buffer.getBuffer(), buffer.size());
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

//----------------------------------------------------------------
void ofSaveImage(ofPixels & pix, string fileName, ofImageQualityType qualityLevel) {

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
			FreeImage_Save(fif, bmp, fileName.c_str());
		}
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}
}

void ofSaveImage(ofPixels & pix, ofBuffer & buffer, ofImageQualityType qualityLevel) {
	ofLog(OF_LOG_ERROR, "ofSaveImage(pix, buffer) is not yet implemented");
}


//----------------------------------------------------------
ofImage::ofImage(){

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

	if (!bFreeImageInited){
		FreeImage_Initialise();
		bFreeImageInited = true;
	}

	clear();
	clone(mom);
	update();
}

//----------------------------------------------------------
ofImage::~ofImage(){
	clear();
}

//----------------------------------------------------------
bool ofImage::loadImage(string fileName){
	bool bLoadedOk = false;
	bLoadedOk = ofLoadImage(myPixels, fileName);
	if (bLoadedOk && myPixels.isAllocated() && bUseTexture){
		tex.allocate(myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
	} 
	if (!bLoadedOk) {
		ofLog(OF_LOG_ERROR, "Couldn't load image from " + fileName);
	}
	update();
	return bLoadedOk;
}

bool ofImage::loadImage(const ofBuffer & buffer){
	bool bLoadedOk = false;
	bLoadedOk = ofLoadImage(myPixels, buffer);
	if (bLoadedOk && myPixels.isAllocated() && bUseTexture){
		tex.allocate(myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
	} 
	if (!bLoadedOk) {
		ofLog(OF_LOG_ERROR, "Couldn't load image from buffer.");
	}
	update();
	return bLoadedOk;
}

//----------------------------------------------------------
void ofImage::saveImage(string fileName, ofImageQualityType qualityLevel){
	ofSaveImage(myPixels, fileName, qualityLevel);
}

//----------------------------------------------------------
void ofImage::saveImage(ofBuffer & buffer, ofImageQualityType qualityLevel){
	ofSaveImage(myPixels, buffer, qualityLevel);
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
void ofImage::draw(const ofRectangle & _r){
	if (bUseTexture){
		tex.draw(_r);
	}
}

//------------------------------------
void ofImage::draw(const ofPoint & _p, float _w, float _h){
	if (bUseTexture){
		tex.draw(_p, _w, _h);
	}
}

//------------------------------------
void ofImage::draw(float _x, float _y, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofImage::draw(float _x, float _y, float _z, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _z, _w, _h);
	}
}

//------------------------------------
void ofImage::draw(const ofPoint & p){
	draw(p.x,p.y,p.z,myPixels.getWidth(),myPixels.getHeight());
}

//------------------------------------
void ofImage::draw(float x, float y){
	draw(x,y,0.0f,myPixels.getWidth(),myPixels.getHeight());
}

//------------------------------------
void ofImage::draw(float x, float y, float z){
	draw(x,y,z,myPixels.getWidth(),myPixels.getHeight());
}

//------------------------------------
void ofImage::allocate(int w, int h, ofImageType type){

	myPixels.allocate(w, h, type);

	// take care of texture allocation --
	if (myPixels.isAllocated() && bUseTexture){
		tex.allocate(myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
	}

	update();
}


//------------------------------------
void ofImage::clear(){

	myPixels.clear();
	if(bUseTexture)	tex.clear();

	width					= 0;
	height					= 0;
	bpp						= 0;
	type 					= OF_IMAGE_UNDEFINED;
	bUseTexture 			= true;		// the default is, yes, use a texture
}

//------------------------------------
unsigned char * ofImage::getPixels(){
	return myPixels.getPixels();
}

ofPixels ofImage::getOFPixels(){
	return myPixels;
}

ofPixels ofImage::getOFPixels() const{
	return myPixels;
}

//------------------------------------
//for getting a reference to the texture
ofTexture & ofImage::getTextureReference(){
	if(!tex.bAllocated() ){
		ofLog(OF_LOG_WARNING, "ofImage - getTextureReference - texture is not allocated");
	}
	return tex;
}

//----------------------------------------------------------
void ofImage::bind(){
	if (bUseTexture && tex.bAllocated())
		tex.bind();
}

//----------------------------------------------------------
void ofImage::unbind(){
	if (bUseTexture && tex.bAllocated())
		tex.unbind();
}



//------------------------------------
void  ofImage::setFromPixels(unsigned char * newPixels, int w, int h, ofImageType newType, bool bOrderIsRGB){

	allocate(w, h, newType);
	myPixels.setFromPixels(newPixels,w,h,newType);

	if (!bOrderIsRGB){
		myPixels.swapRgb();
	}

	update();
}

//------------------------------------
void ofImage::update(){

	if (myPixels.isAllocated() && bUseTexture){
		tex.loadData(myPixels.getPixels(), myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
	}

	width	= myPixels.getWidth();
	height	= myPixels.getHeight();
	bpp		= myPixels.getBitsPerPixel();
	type	= myPixels.getImageType();
}

//------------------------------------
void ofImage::setUseTexture(bool bUse){
	bUseTexture = bUse;
}


//------------------------------------
void ofImage::grabScreen(int _x, int _y, int _w, int _h){

	allocate(_w, _h, OF_IMAGE_COLOR);

	int screenHeight =	ofGetViewportHeight(); // if we are in a FBO or other viewport, this fails: ofGetHeight();
	_y = screenHeight - _y;
	_y -= _h; // top, bottom issues

	if (!((width == _w) && (height == _h))){
		resize(_w, _h);
	}

	#ifndef TARGET_OF_IPHONE
		glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );											// be nice to anyone else who might use pixelStore
	#endif
		glPixelStorei(GL_PACK_ALIGNMENT, 1);														// set read non block aligned...
		glReadPixels(_x, _y, _w, _h, myPixels.getGlDataType(),GL_UNSIGNED_BYTE, myPixels.getPixels());		// read the memory....
	#ifndef TARGET_OF_IPHONE
		glPopClientAttrib();
	#endif

	int sizeOfOneLineOfPixels = myPixels.getWidth() * myPixels.getBytesPerPixel();
	unsigned char * tempLineOfPix = new unsigned char[sizeOfOneLineOfPixels];
	unsigned char * linea;
	unsigned char * lineb;
	for (int i = 0; i < myPixels.getHeight()/2; i++){
		linea = myPixels.getPixels() + i * sizeOfOneLineOfPixels;
		lineb = myPixels.getPixels() + (myPixels.getHeight()-i-1) * sizeOfOneLineOfPixels;
		memcpy(tempLineOfPix, linea, sizeOfOneLineOfPixels);
		memcpy(linea, lineb, sizeOfOneLineOfPixels);
		memcpy(lineb, tempLineOfPix, sizeOfOneLineOfPixels);
	}
	delete [] tempLineOfPix;
	update();
}


//------------------------------------
void ofImage::clone(const ofImage &mom){

	myPixels = mom.getOFPixels();

	tex.clear();
	bUseTexture = mom.bUseTexture;
	if (bUseTexture == true){
		tex.allocate(myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
	}

	update();
}

//------------------------------------
void ofImage::setImageType(ofImageType newType){
	changeTypeOfPixels(myPixels, newType);
	update();
}

//------------------------------------
void ofImage::resize(int newWidth, int newHeight){

	resizePixels(myPixels, newWidth, newHeight);

	if (bUseTexture == true){
		tex.clear();
		tex.allocate(myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
	}

	update();
}


//------------------------------------
void ofImage::crop(int x, int y, int w, int h){
	ofPixelUtils::crop(myPixels, x,y,w,h);
	update();
}

//------------------------------------
void ofImage::cropFrom(ofImage & otherImage, int x, int y, int w, int h){
	
	int myOldWidth = myPixels.getWidth();
	int myOldHeight = myPixels.getHeight();
	
	ofPixelUtils::cropFromTo(otherImage.myPixels,myPixels, x,y,w,h);
	
	if (myOldWidth != myPixels.getWidth() || myOldHeight != myPixels.getHeight()){
		if (bUseTexture == true){
			tex.clear();
			tex.allocate(myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
		}
	} 
	
	update();
}

//------------------------------------
void ofImage::rotate90(int nRotations){
	int myOldWidth = myPixels.getWidth();
	int myOldHeight = myPixels.getHeight();
	ofPixelUtils::rotate90(myPixels, nRotations);
	if (myOldWidth != myPixels.getWidth() || myOldHeight != myPixels.getHeight()){
		if (bUseTexture == true){
			tex.clear();
			tex.allocate(myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
		}
	}
	update();
}

//------------------------------------
void ofImage::mirror(bool vertical, bool horizontal){
	ofPixelUtils::mirror(myPixels, vertical, horizontal);
	update();
}






//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
// freeImage based code & utilities:


//----------------------------------------------------
void ofImage::resizePixels(ofPixels &pix, int newWidth, int newHeight){

	FIBITMAP * bmp					= getBmpFromPixels(pix);
	FIBITMAP * convertedBmp			= NULL;

	convertedBmp = FreeImage_Rescale(bmp, newWidth, newHeight, FILTER_BICUBIC);
	putBmpIntoPixels(convertedBmp, pix, false);

	if (bmp != NULL)				FreeImage_Unload(bmp);
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);

}

//----------------------------------------------------
void ofImage::changeTypeOfPixels(ofPixels &pix, ofImageType newType){

	
		
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
				tex.allocate(myPixels.getWidth(), myPixels.getHeight(), GL_RGB);
			}
			break;
		
		//------------------------------------
		case OF_IMAGE_COLOR_ALPHA:
			convertedBmp = FreeImage_ConvertTo32Bits(bmp);
			if (bNeedNewTexture){
				tex.clear();
				tex.allocate(myPixels.getWidth(), myPixels.getHeight(), GL_RGBA);
			}
			break;
	}

	putBmpIntoPixels(convertedBmp, pix, false);

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



