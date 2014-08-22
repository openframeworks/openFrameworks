#include "ofPixels.h"
#include "ofMath.h"


static ofImageType getImageTypeFromChannels(int channels){
	switch(channels){
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
	width = 0;
	height = 0;
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
void ofPixels_<PixelType>::setFromPixels(const PixelType * newPixels, int w, int h, int channels){
	allocate(w, h, channels);
	memcpy(pixels, newPixels, w * h * getBytesPerPixel());
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromPixels(const PixelType * newPixels, int w, int h, ofImageType type){
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
		ofLogError("ofPixels") << "setFromPixels(): image type " << type << " not supported, not copying";
		break;
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromExternalPixels(PixelType * newPixels, int w, int h, int _channels){
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
	if(width*channels==stride){
		setFromPixels(newPixels,width,height,channels);
		return;
	}
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
void ofPixels_<PixelType>::swap(ofPixels_<PixelType> & pix){
	std::swap(pixels,pix.pixels);
	std::swap(width, pix.width);
	std::swap(height,pix.height);
	std::swap(channels,pix.channels);
	std::swap(pixelsOwner, pix.pixelsOwner);
	std::swap(bAllocated, pix.bAllocated);
}

template<typename PixelType>
PixelType * ofPixels_<PixelType>::getPixels(){
	return &pixels[0];
}

template<typename PixelType>
const PixelType * ofPixels_<PixelType>::getPixels() const{
	return &pixels[0];
}

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
			ofLogError("ofPixels") << "allocate(): unknown pixel format, not allocating";
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
		ofLogError("ofPixels") << "allocate(): unknown image type, not allocating";
		break;

	}
}

template<typename PixelType>
void ofPixels_<PixelType>::swapRgb(){
	if (channels >= 3){
		int sizePixels = width*height*channels;
		for (int i=0; i< sizePixels; i+=channels){
			std::swap(pixels[i],pixels[i+2]);
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
void ofPixels_<PixelType>::setColor(int index, const ofColor_<PixelType>& color) {
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
void ofPixels_<PixelType>::setColor(int x, int y, const ofColor_<PixelType>& color) {
	setColor(getPixelIndex(x, y), color);
}

template<typename PixelType>
void ofPixels_<PixelType>::setColor(const ofColor_<PixelType>& color) {
	int i = 0;
	while(i < size()) {
		for(int j = 0; j < channels; j++) {
			pixels[i++] = color[j];
		}
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
	return getImageTypeFromChannels(getNumChannels());
}

template<typename PixelType>
void ofPixels_<PixelType>::setImageType(ofImageType imageType){
	if(!isAllocated() || imageType==getImageType()) return;
	ofPixels_<PixelType> dst;
	dst.allocate(width,height,imageType);
	PixelType * dstPtr = &dst[0];
	PixelType * srcPtr = &pixels[0];
	int diffNumChannels = 0;
	if(dst.getNumChannels()<getNumChannels()){
		diffNumChannels = getNumChannels()-dst.getNumChannels();
	}
	for(int i=0;i<width*height;i++){
		const PixelType & gray = *srcPtr;
		for(int j=0;j<dst.getNumChannels();j++){
			if(j<getNumChannels()){
				*dstPtr++ =  *srcPtr++;
			}else if(j<3){
				*dstPtr++ = gray;
			}else{
				*dstPtr++ = ofColor_<PixelType>::limit();
			}
		}
		srcPtr+=diffNumChannels;
	}
	swap(dst);
}

template<typename PixelType>
void ofPixels_<PixelType>::setNumChannels(int numChannels){
	if(!isAllocated() || numChannels==getNumChannels()) return;
	setImageType(getImageTypeFromChannels(numChannels));
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

	if (bAllocated == true){

		_width = ofClamp(_width,1,getWidth());
		_height = ofClamp(_height,1,getHeight());

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


	if (bAllocated == true){

		_width = ofClamp(_width,1,getWidth());
		_height = ofClamp(_height,1,getHeight());

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
void ofPixels_<PixelType>::rotate90To(ofPixels_<PixelType> & dst, int nClockwiseRotations){
	if (bAllocated == false){
		return;
	}

	if(&dst == this){
		rotate90(nClockwiseRotations);
		return;
	}

	// first, figure out which type of rotation we have
	int rotation = nClockwiseRotations;
	while (rotation < 0){
		rotation+=4;
	}
	rotation %= 4;

	// if it's 0, just make a copy.  if it's 2, do it by a mirror operation.
	if (rotation == 0) {
		dst = *this;
		return;
		// do nothing!
	} else if (rotation == 2) {
		mirrorTo(dst, true, true);
		return;
	}

	// otherwise, we will need to do some new allocaiton.
	dst.allocate(height,width,getImageType());
    
	int strideSrc = width * channels;
	int strideDst = dst.width * channels;

	if(rotation == 1){
		PixelType * srcPixels = pixels;
		for (int i = 0; i < height; i++){
			PixelType * dstPixels = dst.getPixels() + (strideDst - channels*(i+1));
			for (int j = 0; j < width; j++){
				for (int k = 0; k < channels; k++){
					dstPixels[k] = srcPixels[k];
				}
				srcPixels += channels;
				dstPixels += strideDst;
			}
		}
	} else if(rotation == 3){
		PixelType * dstPixels = dst.pixels;
		for (int i = 0; i < dst.height; i++){
			PixelType * srcPixels = pixels + (strideSrc - channels*(i+1));
			for (int j = 0; j < dst.width; j++){
				for (int k = 0; k < channels; k++){
					dstPixels[k] = srcPixels[k];
				}
				srcPixels += strideSrc;
				dstPixels += channels;
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

	ofPixels_<PixelType> newPixels;
	rotate90To(newPixels,nClockwiseRotations);
	delete [] pixels;
	pixels = newPixels.pixels;
	width = newPixels.width;
	height = newPixels.height;
	newPixels.pixelsOwner = false;

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
void ofPixels_<PixelType>::mirrorTo(ofPixels_<PixelType> & dst, bool vertically, bool horizontal){
	if(&dst == this){
		mirror(vertically,horizontal);
		return;
	}

	if (!vertically && !horizontal){
		dst = *this;
		return;
	}

	int bytesPerPixel = channels;

	if (! (vertically && horizontal)){
		int wToDo = horizontal ? width/2 : width;
		int hToDo = vertically ? height/2 : height;

		for (int i = 0; i < wToDo; i++){
			for (int j = 0; j < hToDo; j++){

				int pixelb = (vertically ? (height - j - 1) : j) * width + (horizontal ? (width - i - 1) : i);
				int pixela = j*width + i;
				for (int k = 0; k < bytesPerPixel; k++){
					dst[pixela*bytesPerPixel + k] = pixels[pixelb*bytesPerPixel + k];
					dst[pixelb*bytesPerPixel + k] = pixels[pixela*bytesPerPixel + k];

				}
			}
		}
	} else {
		// I couldn't think of a good way to do this in place.  I'm sure there is.
		mirrorTo(dst,true, false);
		dst.mirror(false, true);
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
float ofPixels_<PixelType>::bicubicInterpolate (const float *patch, float x,float y, float x2,float y2, float x3,float y3) {
	// adapted from http://www.paulinternet.nl/?page=bicubic
	// Note that this code can produce values outside of 0...255, due to cubic overshoot.
	// The ofClamp() prevents this from happening.

	float p00 = patch[ 0];
	float p10 = patch[ 4];
	float p20 = patch[ 8];
	float p30 = patch[12];

	float p01 = patch[ 1];
	float p11 = patch[ 5];
	float p21 = patch[ 9];
	float p31 = patch[13];

	float p02 = patch[ 2];
	float p12 = patch[ 6];
	float p22 = patch[10];
	float p32 = patch[14];

	float p03 = patch[ 3];
	float p13 = patch[ 7];
	float p23 = patch[11];
	float p33 = patch[15];

	float a00 =    p11;
	float a01 =   -p10 +   p12;
	float a02 =  2.0f*p10 - 2.0f*p11 +   p12 -   p13;
	float a03 =   -p10 +   p11 -   p12 +   p13;
	float a10 =   -p01 +   p21;
	float a11 =    p00 -   p02 -   p20 +   p22;
	float a12 = -2.0f*p00 + 2.0f*p01 -   p02 +   p03 + 2.0f*p20 - 2.0f*p21 +   p22 -   p23;
	float a13 =    p00 -   p01 +   p02 -   p03 -   p20 +   p21 -   p22 +   p23;
	float a20 =  2.0f*p01 - 2.0f*p11 +   p21 -   p31;
	float a21 = -2.0f*p00 + 2.0f*p02 + 2.0f*p10 - 2.0f*p12 -   p20 +   p22 +   p30 -   p32;
	float a22 =  4*p00 - 4*p01 + 2.0f*p02 - 2.0f*p03 - 4*p10 + 4*p11 - 2.0f*p12 + 2.0f*p13 + 2.0f*p20 - 2.0f*p21 + p22 - p23 - 2.0f*p30 + 2.0f*p31 - p32 + p33;
	float a23 = -2.0f*p00 + 2.0f*p01 - 2.0f*p02 + 2.0f*p03 + 2.0f*p10 - 2.0f*p11 + 2.0f*p12 - 2.0f*p13 -   p20 +   p21 - p22 + p23 +   p30 -   p31 + p32 - p33;
	float a30 =   -p01 +   p11 -   p21 +   p31;
	float a31 =    p00 -   p02 -   p10 +   p12 +   p20 -   p22 -   p30 +   p32;
	float a32 = -2.0f*p00 + 2.0f*p01 -   p02 +   p03 + 2.0f*p10 - 2.0f*p11 +   p12 -   p13 - 2.0f*p20 + 2.0f*p21 - p22 + p23 + 2.0f*p30 - 2.0f*p31 + p32 - p33;
	float a33 =    p00 -   p01 +   p02 -   p03 -   p10 +   p11 -   p12 +   p13 +   p20 -   p21 + p22 - p23 -   p30 +   p31 - p32 + p33;

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
	if(&dst == this){
		return true;
	}

	if (!(isAllocated()) || !(dst.isAllocated()) || getBytesPerPixel() != dst.getBytesPerPixel() || getNumChannels()!=dst.getNumChannels()) return false;

	int srcWidth      = getWidth();
	int srcHeight     = getHeight();
	int dstWidth	  = dst.getWidth();
	int dstHeight	  = dst.getHeight();
	int channels 	  = getNumChannels();


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
					int pixelIndex = int(srcIndex + srcx) * channels;
					for (int k=0; k<channels; k++){
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
			ofLogError("ofPixels") << "resizeTo(): bilinear resize not implemented, not resizing";
			break;

			//----------------------------------------
		case OF_INTERPOLATE_BICUBIC:
			float px1, py1;
			float px2, py2;
			float px3, py3;

			float srcColor = 0;
			float interpCol;
			int patchRow;
			int patchIndex;
			float patch[16];

			int srcRowBytes = srcWidth*channels;
			int loIndex = (srcRowBytes)+1;
			int hiIndex = (srcWidth*srcHeight*channels)-(srcRowBytes)-1;

			for (int dsty=0; dsty<dstHeight; dsty++){
				for (int dstx=0; dstx<dstWidth; dstx++){

					int   dstIndex0 = (dsty*dstWidth + dstx) * channels;
					float srcxf = srcWidth  * (float)dstx/(float)dstWidth;
					float srcyf = srcHeight * (float)dsty/(float)dstHeight;
					int   srcx = (int) MIN(srcWidth-1,   srcxf);
					int   srcy = (int) MIN(srcHeight-1,  srcyf);
					int   srcIndex0 = (srcy*srcWidth + srcx) * channels;

					px1 = srcxf - srcx;
					py1 = srcyf - srcy;
					px2 = px1 * px1;
					px3 = px2 * px1;
					py2 = py1 * py1;
					py3 = py2 * py1;

					for (int k=0; k<channels; k++){
						int   dstIndex = dstIndex0+k;
						int   srcIndex = srcIndex0+k;

						for (int dy=0; dy<4; dy++) {
							patchRow = srcIndex + ((dy-1)*srcRowBytes);
							for (int dx=0; dx<4; dx++) {
								patchIndex = patchRow + (dx-1)*channels;
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

template class ofPixels_<char>;
template class ofPixels_<unsigned char>;
template class ofPixels_<short>;
template class ofPixels_<unsigned short>;
template class ofPixels_<int>;
template class ofPixels_<unsigned int>;
template class ofPixels_<long>;
template class ofPixels_<unsigned long>;
template class ofPixels_<float>;
template class ofPixels_<double>;
