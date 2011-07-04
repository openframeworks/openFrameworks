#include "ofPixels.h"
#include "ofMath.h"


template<typename PixelType>
ofPixels_<PixelType>::ofPixels_(){
	bAllocated = false;
	pixelsOwner = false;
	channels = 0;
	pixels = NULL;
	clear();
}


template<typename PixelType>
ofPixels_<PixelType>::~ofPixels_(){
	clear();
}

template<typename PixelType>
ofPixels_<PixelType>::ofPixels_(const ofPixels_<PixelType> & mom){
	bAllocated = false;
	pixelsOwner = false;
	channels = 0;
	pixels = NULL;
	copyFrom( mom );
}

template<typename PixelType>
ofPixels_<PixelType>& ofPixels_<PixelType>::operator=(const ofPixels_<PixelType> & mom){
	if(this==&mom) {
		return * this;
	}
	copyFrom( mom );
	return *this;
}

template<typename PixelType>
void ofPixels_<PixelType>::copyFrom(const ofPixels_<PixelType> & mom){
	if(mom.isAllocated()) {
		allocate(mom.getWidth(), mom.getHeight(), mom.getNumChannels());
		memcpy(pixels, mom.getPixels(), mom.getWidth() * mom.getHeight() * mom.getBytesPerPixel());
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::set(PixelType val){
	int size = width * height * channels;
	for(int i = 0; i < size; i++){
		pixels[i] = val;
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::set(int channel,PixelType val){
	int size = width*height*channels;
	for(int i=channel;i<size;i+=channels){
		pixels[i] = val;
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromPixels(const PixelType * newPixels,int w, int h, int channels){
	allocate(w, h, channels);
	memcpy(pixels, newPixels, w * h * getBytesPerPixel());
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromPixels(const PixelType * newPixels,int w, int h, ofImageType type){
	allocate(w,h,type);
	switch(type){
	case OF_IMAGE_GRAYSCALE:
		setFromPixels(newPixels,w,h,1);
		break;
	case OF_IMAGE_COLOR:
		setFromPixels(newPixels,w,h,3);
		break;
	case OF_IMAGE_COLOR_ALPHA:
		setFromPixels(newPixels,w,h,4);
		break;
	default:
		ofLog(OF_LOG_ERROR,"ofPixels: image type not supported");
		break;
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromExternalPixels(PixelType * newPixels,int w, int h, int _channels){
	clear();
	channels = _channels;
	width= w;
	height = h;

	pixels = newPixels;
	pixelsOwner = false;
	bAllocated = true;
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromAlignedPixels(const PixelType * newPixels, int width, int height, int channels, int stride) {
	allocate(width, height, channels);
	int dstStride = width * getBytesPerPixel();
	const unsigned char* src = (unsigned char*) newPixels;
	unsigned char* dst =  (unsigned char*) pixels;
	for(int i = 0; i < height; i++) {
		memcpy(dst, src, dstStride);
		src += stride;
		dst += dstStride;
	}
}

template<typename PixelType>
PixelType * ofPixels_<PixelType>::getPixels(){
	return &pixels[0];
}

template<typename PixelType>
const PixelType * ofPixels_<PixelType>::getPixels() const{
	return &pixels[0];
}


/*template<typename PixelType>
void ofPixels_<PixelType>::allocate(int w, int h, int bitsPerPixel){
	ofImageType type = getImageTypeFromBits(bitsPerPixel);
	allocate(w,h,type);
}*/

template<typename PixelType>
void ofPixels_<PixelType>::allocate(int w, int h, int _channels){
	if (w < 0 || h < 0) {
		return;
	}

	//we check if we are already allocated at the right size
	if(bAllocated && w == width && h == height && channels ==_channels){
		return; //we don't need to allocate
	}

	//we do need to allocate, clear the data
	clear();

	channels = _channels;
	width= w;
	height = h;

	pixels = new PixelType[w * h * channels];
	bAllocated = true;
	pixelsOwner = true;
}


template<typename PixelType>
void ofPixels_<PixelType>::allocate(int w, int h, ofPixelFormat format){

	if (w < 0 || h < 0) return;

	ofImageType imgType;
	switch(format){
		case OF_PIXELS_RGB:
			imgType = OF_IMAGE_COLOR;
			break;
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
			imgType = OF_IMAGE_COLOR_ALPHA;
			break;
		case OF_PIXELS_MONO:
			imgType = OF_IMAGE_GRAYSCALE;
			break;
		default:
			ofLog(OF_LOG_ERROR,"ofPixels: format not supported, not allocating");
			return;
			break;

	}
	allocate(w,h,imgType);
}

template<typename PixelType>
void ofPixels_<PixelType>::allocate(int w, int h, ofImageType type){
	switch(type){
	case OF_IMAGE_GRAYSCALE:
		allocate(w,h,1);
		break;
	case OF_IMAGE_COLOR:
		allocate(w,h,3);
		break;
	case OF_IMAGE_COLOR_ALPHA:
		allocate(w,h,4);
		break;
	default:
		ofLog(OF_LOG_ERROR,"ofPixels: image type not supported");
		break;

	}
}

template<typename PixelType>
void ofPixels_<PixelType>::swapRgb(){
	if (channels >= 3){
		int sizePixels		= width*height;
		int cnt				= 0;
		PixelType * pixels_ptr = pixels;

		while (cnt < sizePixels){
			std::swap(pixels_ptr[0],pixels_ptr[2]);
			cnt++;
			pixels_ptr+=channels;
		}
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::clear(){
	if(pixels){
		if(pixelsOwner) delete[] pixels;
		pixels = NULL;
	}

	width			= 0;
	height			= 0;
	channels		= 0;
	bAllocated		= false;
}

template<typename PixelType>
int ofPixels_<PixelType>::getPixelIndex(int x, int y) const {
	if( !bAllocated ){
		return 0;
	}else{
		return ( x + y * width ) * channels;
	}
}

template<typename PixelType>
ofColor_<PixelType> ofPixels_<PixelType>::getColor(int x, int y) const {
	ofColor_<PixelType> c;
	int index = getPixelIndex(x, y);

	if( channels == 1 ){
		c.set( pixels[index] );
	}else if( channels == 3 ){
		c.set( pixels[index], pixels[index+1], pixels[index+2] );
	}else if( channels == 4 ){
		c.set( pixels[index], pixels[index+1], pixels[index+2], pixels[index+3] );
	}

	return c;
}

template<typename PixelType>
void ofPixels_<PixelType>::setColor(int x, int y, ofColor_<PixelType> color) {
	int index = getPixelIndex(x, y);

	if( channels == 1 ){
		pixels[index] = color.getBrightness();
	}else if( channels == 3 ){
		pixels[index] = color.r;
		pixels[index+1] = color.g;
		pixels[index+2] = color.b;
	}else if( channels == 4 ){
		pixels[index] = color.r;
		pixels[index+1] = color.g;
		pixels[index+2] = color.b;
		pixels[index+3] = color.a;
	}
}

template<typename PixelType>
PixelType & ofPixels_<PixelType>::operator[](int pos){
	return pixels[pos];
}

template<typename PixelType>
const PixelType & ofPixels_<PixelType>::operator[](int pos) const{
	return pixels[pos];
}

template<typename PixelType>
bool ofPixels_<PixelType>::isAllocated() const{
	return bAllocated;
}

template<typename PixelType>
int ofPixels_<PixelType>::getWidth() const{
	return width;
}

template<typename PixelType>
int ofPixels_<PixelType>::getHeight() const{
	return height;
}

template<typename PixelType>
int ofPixels_<PixelType>::getBytesPerPixel() const{
	return getBytesPerChannel() * channels;
}

template<typename PixelType>
int ofPixels_<PixelType>::getBitsPerPixel() const{
	return getBitsPerChannel() * channels;
}

template<typename PixelType>
int ofPixels_<PixelType>::getBytesPerChannel() const{
	return sizeof(PixelType);
}

template<typename PixelType>
int ofPixels_<PixelType>::getBitsPerChannel() const{
	return getBytesPerChannel() * 8;
}

template<typename PixelType>
int ofPixels_<PixelType>::getNumChannels() const{
	return channels;
}

template<typename PixelType>
ofImageType ofPixels_<PixelType>::getImageType() const{
	switch(getNumChannels()){
	case 1:
		return OF_IMAGE_GRAYSCALE;
	case 3:
		return OF_IMAGE_COLOR;
	case 4:
		return OF_IMAGE_COLOR_ALPHA;
	default:
		return OF_IMAGE_UNDEFINED;
	}
}

template<typename PixelType>
int ofPixels_<PixelType>::size() const{
	return width*height*channels;
}

template<typename PixelType>
ofPixels_<PixelType> ofPixels_<PixelType>::getChannel(int channel) const{
	ofPixels_<PixelType> channelPixels;
	channelPixels.allocate(width,height,1);
	channel = ofClamp(channel,0,channels-1);
	int j=0;
	for(int i=channel;i<size();i+=channels, ++j){
		channelPixels[j]=pixels[i];
	}
	return channelPixels;
}

template<typename PixelType>
void ofPixels_<PixelType>::setChannel(int channel, const ofPixels_<PixelType> channelPixels){
	channel = ofClamp(channel,0,channels-1);
	int j=0;
	for(int i=channel;i<size();i+=channels, ++j){
		pixels[i]=channelPixels[j];
	}

}

//From ofPixelsUtils
//----------------------------------------------------------------------
template<typename PixelType>
void ofPixels_<PixelType>::crop(int x, int y, int _width, int _height){

	if (_width < 0 || _height < 0){
		return;
	}

	if (bAllocated == true){

		int bytesPerPixel = channels;

		int newWidth = _width;
		int newHeight = _height;
		
		PixelType * newPixels = new PixelType[newWidth*newHeight*bytesPerPixel];
		memset(newPixels, 0, newWidth*newHeight*channels*sizeof(PixelType));

		// this prevents having to do a check for bounds in the for loop;
		int minX = MAX(x, 0);
		int maxX = MIN(x+_width, width);
		int minY = MAX(y, 0);
		int maxY = MIN(y+_height, height);

		// TODO: point math can help speed this up:
		for (int i = minX; i < maxX; i++){
			for (int j = minY; j < maxY; j++){

				int newPixel = (j-y) * newWidth + (i-x);
				int oldPixel = (j) * width + (i);

				for (int k = 0; k < bytesPerPixel; k++){
					newPixels[newPixel*bytesPerPixel + k] = pixels[oldPixel*bytesPerPixel + k];
				}
			}
		}

		delete [] pixels;
		pixels = newPixels;
		width = newWidth;
		height = newHeight;
	}
}

//----------------------------------------------------------------------
template<typename PixelType>
void ofPixels_<PixelType>::cropTo(ofPixels_<PixelType> &toPix, int x, int y, int _width, int _height){

	if (_width < 0 || _height < 0){
		return;
	}

	if (bAllocated == true){

		int bytesPerPixel = channels;

		if ((toPix.width != _width) || (toPix.height != _height) || (toPix.channels != channels)){
			toPix.allocate(_width, _height, channels);
		}

		int newWidth = _width;
		PixelType * newPixels = toPix.pixels;

		// this prevents having to do a check for bounds in the for loop;
		int minX = MAX(x, 0);
		int maxX = MIN(x+_width, width);
		int minY = MAX(y, 0);
		int maxY = MIN(y+_height, height);

		// TODO: point math can help speed this up:
		for (int i = minX; i < maxX; i++){
			for (int j = minY; j < maxY; j++){

				int newPixel = (j-y) * newWidth + (i-x);
				int oldPixel = (j) * width + (i);

				for (int k = 0; k < bytesPerPixel; k++){
					newPixels[newPixel*bytesPerPixel + k] = pixels[oldPixel*bytesPerPixel + k];
				}
			}
		}


	}

}

//----------------------------------------------------------------------
template<typename PixelType>
void ofPixels_<PixelType>::rotate90(int nClockwiseRotations){


	if (bAllocated == false){
		return;
	}

	// first, figure out which type of rotation we have
	int rotation = nClockwiseRotations;
	while (rotation < 0){
		rotation+=4;
	}
	rotation %= 4;

	// if it's 0, do nothing.  if it's 2, do it by a mirror operation.
	if (rotation == 0) {
		return;
		// do nothing!
	} else if (rotation == 2) {
		mirror(true, true);
		return;
	}

	// otherwise, we will need to do some new allocaiton.
	int bytesPerPixel = channels;
	PixelType * oldPixels = pixels;
	int newWidth = height;
	int newHeight = width;
	PixelType * newPixels = new PixelType[newWidth*newHeight*bytesPerPixel];

	if(rotation == 1){
		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){

				int pixela = (j*width + i);
				int pixelb = ((i) * newWidth + (height - j - 1));
				for (int k = 0; k < bytesPerPixel; k++){
					newPixels[pixelb*bytesPerPixel + k] = oldPixels[pixela*bytesPerPixel + k];
				}

			}
		}
	} else if(rotation == 3){
		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){

				int pixela = (j*width + i);
				int pixelb = ((width-i-1) * newWidth + j);
				for (int k = 0; k < bytesPerPixel; k++){
					newPixels[pixelb*bytesPerPixel + k] = oldPixels[pixela*bytesPerPixel + k];
				}
			}
		}
	}

	delete [] pixels;
	pixels = newPixels;
	width = newWidth;
	height = newHeight;

}

//----------------------------------------------------------------------
template<typename PixelType>
void ofPixels_<PixelType>::mirror(bool vertically, bool horizontal){

	if (!vertically && !horizontal){
		return;
	}

	int bytesPerPixel = channels;
	PixelType * oldPixels = pixels;
	PixelType tempVal;

	if (! (vertically && horizontal)){
		int wToDo = horizontal ? width/2 : width;
		int hToDo = vertically ? height/2 : height;

		for (int i = 0; i < wToDo; i++){
			for (int j = 0; j < hToDo; j++){

				int pixelb = (vertically ? (height - j - 1) : j) * width + (horizontal ? (width - i - 1) : i);
				int pixela = j*width + i;
				for (int k = 0; k < bytesPerPixel; k++){

					tempVal = oldPixels[pixela*bytesPerPixel + k];
					oldPixels[pixela*bytesPerPixel + k] = oldPixels[pixelb*bytesPerPixel + k];
					oldPixels[pixelb*bytesPerPixel + k] = tempVal;

				}
			}
		}
	} else {
		// I couldn't think of a good way to do this in place.  I'm sure there is.
		mirror(true, false);
		mirror(false, true);
	}

}

//----------------------------------------------------------------------
template<typename PixelType>
bool ofPixels_<PixelType>::resize(int dstWidth, int dstHeight, ofInterpolationMethod interpMethod){

	if ((dstWidth<=0) || (dstHeight<=0) || !(isAllocated())) return false;

	ofPixels_<PixelType> dstPixels;
	dstPixels.allocate(dstWidth, dstHeight,getImageType());

	if(!resizeTo(dstPixels,interpMethod)) return false;

	delete [] pixels;
	pixels = dstPixels.getPixels();
	width  = dstWidth;
	height = dstHeight;
	dstPixels.pixelsOwner = false;
	return true;
}

//----------------------------------------------------------------------
template<typename PixelType>
float ofPixels_<PixelType>::bicubicInterpolate (const PixelType *patch, float x,float y, float x2,float y2, float x3,float y3) {
	// adapted from http://www.paulinternet.nl/?page=bicubic
	// Note that this code can produce values outside of 0...255, due to cubic overshoot.
	// The ofClamp() prevents this from happening.

	PixelType p00 = patch[ 0];
	PixelType p10 = patch[ 4];
	PixelType p20 = patch[ 8];
	PixelType p30 = patch[12];

	PixelType p01 = patch[ 1];
	PixelType p11 = patch[ 5];
	PixelType p21 = patch[ 9];
	PixelType p31 = patch[13];

	PixelType p02 = patch[ 2];
	PixelType p12 = patch[ 6];
	PixelType p22 = patch[10];
	PixelType p32 = patch[14];

	PixelType p03 = patch[ 3];
	PixelType p13 = patch[ 7];
	PixelType p23 = patch[11];
	PixelType p33 = patch[15];

	PixelType a00 =    p11;
	PixelType a01 =   -p10 +   p12;
	PixelType a02 =  2*p10 - 2*p11 +   p12 -   p13;
	PixelType a03 =   -p10 +   p11 -   p12 +   p13;
	PixelType a10 =   -p01 +   p21;
	PixelType a11 =    p00 -   p02 -   p20 +   p22;
	PixelType a12 = -2*p00 + 2*p01 -   p02 +   p03 + 2*p20 - 2*p21 +   p22 -   p23;
	PixelType a13 =    p00 -   p01 +   p02 -   p03 -   p20 +   p21 -   p22 +   p23;
	PixelType a20 =  2*p01 - 2*p11 +   p21 -   p31;
	PixelType a21 = -2*p00 + 2*p02 + 2*p10 - 2*p12 -   p20 +   p22 +   p30 -   p32;
	PixelType a22 =  4*p00 - 4*p01 + 2*p02 - 2*p03 - 4*p10 + 4*p11 - 2*p12 + 2*p13 + 2*p20 - 2*p21 + p22 - p23 - 2*p30 + 2*p31 - p32 + p33;
	PixelType a23 = -2*p00 + 2*p01 - 2*p02 + 2*p03 + 2*p10 - 2*p11 + 2*p12 - 2*p13 -   p20 +   p21 - p22 + p23 +   p30 -   p31 + p32 - p33;
	PixelType a30 =   -p01 +   p11 -   p21 +   p31;
	PixelType a31 =    p00 -   p02 -   p10 +   p12 +   p20 -   p22 -   p30 +   p32;
	PixelType a32 = -2*p00 + 2*p01 -   p02 +   p03 + 2*p10 - 2*p11 +   p12 -   p13 - 2*p20 + 2*p21 - p22 + p23 + 2*p30 - 2*p31 + p32 - p33;
	PixelType a33 =    p00 -   p01 +   p02 -   p03 -   p10 +   p11 -   p12 +   p13 +   p20 -   p21 + p22 - p23 -   p30 +   p31 - p32 + p33;

	float out =
    a00      + a01 * y      + a02 * y2      + a03 * y3 +
    a10 * x  + a11 * x  * y + a12 * x  * y2 + a13 * x  * y3 +
    a20 * x2 + a21 * x2 * y + a22 * x2 * y2 + a23 * x2 * y3 +
    a30 * x3 + a31 * x3 * y + a32 * x3 * y2 + a33 * x3 * y3;

	return MIN(255, MAX(out, 0));
}

//----------------------------------------------------------------------
template<typename PixelType>
bool ofPixels_<PixelType>::resizeTo(ofPixels_<PixelType>& dst, ofInterpolationMethod interpMethod){

	if (!(isAllocated()) || !(dst.isAllocated()) || getBytesPerPixel() != dst.getBytesPerPixel()) return false;

	int srcWidth      = getWidth();
	int srcHeight     = getHeight();
	int dstWidth	  = dst.getWidth();
	int dstHeight	  = dst.getHeight();
	int bytesPerPixel = getBytesPerPixel();


	PixelType * dstPixels = dst.getPixels();

	switch (interpMethod){

			//----------------------------------------
		case OF_INTERPOLATE_NEAREST_NEIGHBOR:{
			int dstIndex = 0;
			float srcxFactor = (float)srcWidth/dstWidth;
			float srcyFactor = (float)srcHeight/dstHeight;
			float srcy = 0.5;
			for (int dsty=0; dsty<dstHeight; dsty++){
				float srcx = 0.5;
				int srcIndex = int(srcy)*srcWidth;
				for (int dstx=0; dstx<dstWidth; dstx++){
					int pixelIndex = int(srcIndex + srcx) * bytesPerPixel;
					for (int k=0; k<bytesPerPixel; k++){
						dstPixels[dstIndex] = pixels[pixelIndex];
						dstIndex++;
						pixelIndex++;
					}
					srcx+=srcxFactor;
				}
				srcy+=srcyFactor;
			}
		}break;

			//----------------------------------------
		case OF_INTERPOLATE_BILINEAR:
			// not implemented yet
			ofLogError(" Bilinear resize not implemented ");
			break;

			//----------------------------------------
		case OF_INTERPOLATE_BICUBIC:
			float px1, py1;
			float px2, py2;
			float px3, py3;

			PixelType srcColor;
			PixelType interpCol;
			int patchRow;
			int patchIndex;
			PixelType patch[16];

			int srcRowBytes = srcWidth*bytesPerPixel;
			int loIndex = (srcRowBytes)+1;
			int hiIndex = (srcWidth*srcHeight*bytesPerPixel)-(srcRowBytes)-1;

			for (int dsty=0; dsty<dstHeight; dsty++){
				for (int dstx=0; dstx<dstWidth; dstx++){

					int   dstIndex0 = (dsty*dstWidth + dstx) * bytesPerPixel;
					float srcxf = srcWidth  * (float)dstx/(float)dstWidth;
					float srcyf = srcHeight * (float)dsty/(float)dstHeight;
					int   srcx = (int) MIN(srcWidth-1,   srcxf);
					int   srcy = (int) MIN(srcHeight-1,  srcyf);
					int   srcIndex0 = (srcy*srcWidth + srcx) * bytesPerPixel;

					px1 = srcxf - srcx;
					py1 = srcyf - srcy;
					px2 = px1 * px1;
					px3 = px2 * px1;
					py2 = py1 * py1;
					py3 = py2 * py1;

					for (int k=0; k<bytesPerPixel; k++){
						int   dstIndex = dstIndex0+k;
						int   srcIndex = srcIndex0+k;

						for (int dy=0; dy<4; dy++) {
							patchRow = srcIndex + ((dy-1)*srcRowBytes);
							for (int dx=0; dx<4; dx++) {
								patchIndex = patchRow + (dx-1)*bytesPerPixel;
								if ((patchIndex >= loIndex) && (patchIndex < hiIndex)) {
									srcColor = pixels[patchIndex];
								}
								patch[dx*4 + dy] = srcColor;
							}
						}

						interpCol = (PixelType)bicubicInterpolate(patch, px1,py1, px2,py2, px3,py3);
						dstPixels[dstIndex] = interpCol;
					}

				}
			}
			break;
	}

	return true;
}

//----------------------------------------------------------------------
template<typename PixelType>
bool ofPixels_<PixelType>::pasteInto(ofPixels_<PixelType> &dst, int xTo, int yTo){
	if (!(isAllocated()) || !(dst.isAllocated()) || getBytesPerPixel() != dst.getBytesPerPixel() || xTo>=dst.getWidth() || yTo>=dst.getHeight()) return false;

	int bytesToCopyPerRow = (xTo + getWidth()<=dst.getWidth() ? getWidth() : dst.getWidth()-xTo) * getBytesPerPixel();
	int columnsToCopy = yTo + getHeight() <= dst.getHeight() ? getHeight() : dst.getHeight()-yTo;
	PixelType * dstPix = dst.getPixels() + ((xTo + yTo*dst.getWidth())*dst.getBytesPerPixel());
	PixelType * srcPix = getPixels();
	int srcStride = getWidth()*getBytesPerPixel();
	int dstStride = dst.getWidth()*dst.getBytesPerPixel();

	for(int y=0;y<columnsToCopy; y++){
		memcpy(dstPix,srcPix,bytesToCopyPerRow);
		dstPix += dstStride;
		srcPix += srcStride;
	}

	return true;
}

template class ofPixels_<unsigned char>;
template class ofPixels_<float>;
template class ofPixels_<unsigned short>;
