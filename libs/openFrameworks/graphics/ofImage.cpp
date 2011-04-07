#include "ofImage.h"
#include "ofAppRunner.h"
#include "ofTypes.h"
#include "ofURLFileLoader.h"
#include "ofGraphics.h"
#include "FreeImage.h"

//----------------------------------------------------------
// static variable for freeImage initialization:
static void initFreeImage(bool deinit=false){
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
//----------------------------------------------------------

#ifdef TARGET_ANDROID
	// android destroys the opengl context on screen orientation change
	// or when the application runs in the background so we need to reload
	// all the textures when the context is created again.
	// keeping a pointer to all the images we can tell them to reload from a static method
	#include <set>
	static set<ofImage*> all_images;

	void ofReloadAllImageTextures(){
		set<ofImage*>::iterator it;
		for(it=all_images.begin(); it!=all_images.end(); it++){
			(*it)->reloadTexture();
		}
	}
#endif
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
	initFreeImage();
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
	initFreeImage();
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
void ofSaveImage(ofPixels & pix, string fileName, ofImageQualityType qualityLevel) {
	initFreeImage();
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
	initFreeImage();

#ifdef TARGET_ANDROID
	all_images.insert(this);
#endif
}

//----------------------------------------------------------
ofImage& ofImage::operator=(const ofImage& mom) {
	clone(mom);
	update();
	return *this;
}

//----------------------------------------------------------
ofImage::ofImage(const ofImage& mom) {
	clear();
	clone(mom);
	update();
}

//----------------------------------------------------------
ofImage::~ofImage(){
	clear();

#ifdef TARGET_ANDROID
	all_images.erase(this);
#endif
}


//----------------------------------------------------------
void ofImage::reloadTexture(){
	if (pixels.isAllocated() == true && bUseTexture == true){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}
}

//----------------------------------------------------------
bool ofImage::loadImage(string fileName){
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

bool ofImage::loadImage(const ofBuffer & buffer){
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
void ofImage::saveImage(string fileName, ofImageQualityType qualityLevel){
	ofSaveImage(pixels, fileName, qualityLevel);
}

//----------------------------------------------------------
void ofImage::saveImage(ofBuffer & buffer, ofImageQualityType qualityLevel){
	ofSaveImage(pixels, buffer, qualityLevel);
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
	ofGetDefaultRenderer()->draw(*this,_r.x,_r.y,0,_r.width,_r.height);
}

//------------------------------------
void ofImage::draw(const ofPoint & _p, float _w, float _h){
	ofGetDefaultRenderer()->draw(*this,_p.x,_p.y,_p.z,_w,_h);
}

//------------------------------------
void ofImage::draw(float _x, float _y, float _w, float _h){
	ofGetDefaultRenderer()->draw(*this,_x,_y,0,_w,_h);
}

//------------------------------------
void ofImage::draw(float _x, float _y, float _z, float _w, float _h){
	ofGetDefaultRenderer()->draw(*this,_x,_y,_z,_w,_h);
}

//------------------------------------
void ofImage::draw(const ofPoint & p){
	draw(p.x,p.y,p.z,pixels.getWidth(),pixels.getHeight());
}

//------------------------------------
void ofImage::draw(float x, float y){
	draw(x,y,0.0f,pixels.getWidth(),pixels.getHeight());
}

//------------------------------------
void ofImage::draw(float x, float y, float z){
	draw(x,y,z,pixels.getWidth(),pixels.getHeight());
}

//------------------------------------
void ofImage::allocate(int w, int h, ofImageType type){

	pixels.allocate(w, h, type);

	// take care of texture allocation --
	if (pixels.isAllocated() && bUseTexture){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}

	update();
}


//------------------------------------
void ofImage::clear(){

	pixels.clear();
	if(bUseTexture)	tex.clear();

	width					= 0;
	height					= 0;
	bpp						= 0;
	type 					= OF_IMAGE_UNDEFINED;
	bUseTexture 			= true;		// the default is, yes, use a texture
}

//------------------------------------
unsigned char * ofImage::getPixels(){
	return pixels.getPixels();
}

//----------------------------------------------------------
ofPixelsRef ofImage::getPixelsRef(){
	return pixels;
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
ofColor ofImage::getColor(int x, int y) const {
	return pixels.getColor(x, y);
}

//------------------------------------
void ofImage::setColor(int x, int y, ofColor color) {
	pixels.setColor(x, y, color);
}

//------------------------------------
void  ofImage::setFromPixels(unsigned char * newPixels, int w, int h, ofImageType newType, bool bOrderIsRGB){

	allocate(w, h, newType);
	pixels.setFromPixels(newPixels,w,h,newType);

	if (!bOrderIsRGB){
		pixels.swapRgb();
	}

	update();
}

//------------------------------------
void ofImage::update(){

	if (pixels.isAllocated() && bUseTexture){
		tex.loadData(pixels.getPixels(), pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}

	width	= pixels.getWidth();
	height	= pixels.getHeight();
	bpp		= pixels.getBitsPerPixel();
	type	= pixels.getImageType();
}

//------------------------------------
void ofImage::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//------------------------------------
bool ofImage::isUsingTexture(){
	return bUseTexture;
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
void ofImage::clone(const ofImage &mom){

	pixels = mom.pixels;

	tex.clear();
	bUseTexture = mom.bUseTexture;
	if (bUseTexture == true){
		tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}

	update();
}

//------------------------------------
void ofImage::setImageType(ofImageType newType){
	changeTypeOfPixels(pixels, newType);
	update();
}

//------------------------------------
void ofImage::resize(int newWidth, int newHeight){

	resizePixels(pixels, newWidth, newHeight);

	if (bUseTexture == true){
		tex.clear();
		tex.allocate(pixels.getWidth(), pixels.getHeight(), pixels.getGlDataType());
	}

	update();
}


//------------------------------------
void ofImage::crop(int x, int y, int w, int h){
	ofPixelUtils::crop(pixels, x,y,w,h);
	update();
}

//------------------------------------
void ofImage::cropFrom(ofImage & otherImage, int x, int y, int w, int h){
	
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
void ofImage::rotate90(int nRotations){
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
void ofImage::mirror(bool vertical, bool horizontal){
	ofPixelUtils::mirror(pixels, vertical, horizontal);
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

//----------------------------------------------------
// freeImage based stuff:
void ofCloseFreeImage(){
	initFreeImage(true);
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



