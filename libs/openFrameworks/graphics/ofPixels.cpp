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

static float pixelBytesFromPixelFormat(ofPixelFormat format){
	switch(format){
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
			return 3;
			break;
		case OF_PIXELS_RGB565:
			return 2;
			break;
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
			return 4;
			break;
		case OF_PIXELS_MONO:
			return 1;
			break;
		case OF_PIXELS_NV12:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
			return 1.5;
		case OF_PIXELS_YUY2:
			return 2;
			break;
		default:
			return 0;
	}

}

static int bytesFromPixelFormat(int w, int h, ofPixelFormat format){
	return w*h*pixelBytesFromPixelFormat(format);
}

static int channelsFromPixelFormat(ofPixelFormat format){
	switch(format){
	case OF_PIXELS_RGB:
	case OF_PIXELS_BGR:
		return 3;
		break;
	case OF_PIXELS_RGBA:
	case OF_PIXELS_BGRA:
		return 4;
		break;
	case OF_PIXELS_MONO:
		return 1;
		break;
	default:
		ofLog(OF_LOG_ERROR,"ofPixels: format doesn't support channels");
		return 0;
	}
}

static ofPixelFormat ofPixelFormatFromImageType(ofImageType type){
	switch(type){
	case OF_IMAGE_GRAYSCALE:
		return OF_PIXELS_MONO;
		break;
	case OF_IMAGE_COLOR:
		return OF_PIXELS_RGB;
		break;
	case OF_IMAGE_COLOR_ALPHA:
		return OF_PIXELS_RGBA;
		break;
	default:
		ofLog(OF_LOG_ERROR,"ofPixels: image type not supported");
		return OF_PIXELS_UNKOWN;
	}
}

string ofToString(ofPixelFormat pixelFormat){
	switch(pixelFormat){
		case OF_PIXELS_RGB:
			return "RGB";
		case OF_PIXELS_BGR:
			return "BGR";
		case OF_PIXELS_RGBA:
			return "RGBA";
		case OF_PIXELS_BGRA:
			return "BGRA";
		case OF_PIXELS_MONO:
			return "MONO";
		case OF_PIXELS_RGB565:
			return "RGB565";
		case OF_PIXELS_NV12:
			return "NV12";
		case OF_PIXELS_YV12:
			return "YV12";
		case OF_PIXELS_I420:
			return "I420";
		case OF_PIXELS_YUY2:
			return "YUY2";
		default:
			return "UNKOWN";
	}
}

static ofPixelFormat pixelFormatFromNumChannels(int channels){
	switch(channels){
	case 1: return OF_PIXELS_MONO;
	case 3: return OF_PIXELS_RGB;
	case 4: return OF_PIXELS_RGBA;
	default: return OF_PIXELS_UNKOWN;
	}
}

template<typename PixelType>
ofPixels_<PixelType>::ofPixels_(){
	bAllocated = false;
	pixelsOwner = false;
	pixels = NULL;
	pixelsSize = 0;
	width = 0;
	height = 0;
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
	pixelsSize = 0;
	pixels = NULL;
	width = 0;
	height = 0;
	pixelFormat = OF_PIXELS_UNKOWN;
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
		allocate(mom.getWidth(), mom.getHeight(), mom.getPixelFormat());
		memcpy(pixels, mom.getPixels(), mom.size() * sizeof(PixelType));
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::set(PixelType val){
	int _size = size();
	for(int i = 0; i < _size; i++){
		pixels[i] = val;
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::set(int channel,PixelType val){
	int pixelStride;
	switch(pixelFormat){
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
			pixelStride = 3;
			for(int i=channel;i<size();i+=pixelStride){
				pixels[i] = val;
			}
			break;
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
			pixelStride = 4;
			for(int i=channel;i<size();i+=pixelStride){
				pixels[i] = val;
			}
			break;
		case OF_PIXELS_MONO:
			pixelStride = 1;
			for(int i=channel;i<size();i+=pixelStride){
				pixels[i] = val;
			}
			break;
		case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UNKOWN:
			ofLogWarning() << "setting channels not supported for " << ofToString(pixelFormat) << " format";
			break;
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromPixels(const PixelType * newPixels,int w, int h, ofPixelFormat _pixelFormat){
	allocate(w, h, _pixelFormat);
	memcpy(pixels, newPixels, size() * sizeof(PixelType));
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromPixels(const PixelType * newPixels, int w, int h, ofImageType type){
	allocate(w,h,type);
	setFromPixels(newPixels,w,h,ofPixelFormatFromImageType(type));
}


template<typename PixelType>
void ofPixels_<PixelType>::setFromExternalPixels(PixelType * newPixels,int w, int h, int channels){
	setFromExternalPixels(newPixels,w,h,pixelFormatFromNumChannels(channels));
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromExternalPixels(PixelType * newPixels,int w, int h, ofPixelFormat _pixelFormat){
	clear();
	pixelFormat = _pixelFormat;
	width= w;
	height = h;

	pixelsSize = bytesFromPixelFormat(w,h,_pixelFormat);

	pixels = newPixels;
	pixelsOwner = false;
	bAllocated = true;
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromAlignedPixels(const PixelType * newPixels, int width, int height, int channels, int stride){
	setFromAlignedPixels(newPixels,width,height,pixelFormatFromNumChannels(channels),stride);
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromAlignedPixels(const PixelType * newPixels, int width, int height, ofPixelFormat _pixelFormat, int stride) {
	int channels = channelsFromPixelFormat(_pixelFormat);
	if(channels==0) return;

	if(width*channels==stride){
		setFromPixels(newPixels,width,height,_pixelFormat);
		return;
	}
	allocate(width, height, _pixelFormat);
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
	std::swap(pixelFormat,pix.pixelFormat);
	std::swap(pixelsSize,pix.pixelsSize);
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
	allocate(w,h,pixelFormatFromNumChannels(_channels));
}

template<typename PixelType>
void ofPixels_<PixelType>::allocate(int w, int h, ofPixelFormat format){
	if (w <= 0 || h <= 0 || format==OF_PIXELS_UNKOWN) {
		return;
	}

	int newSize = bytesFromPixelFormat(w,h,format);
	//we check if we are already allocated at the right size
	if(bAllocated && newSize==size()){
		pixelFormat = format;
		return; //we don't need to allocate
	}

	//we do need to allocate, clear the data
	clear();

	pixelFormat	= format;
	width 		= w;
	height 		= h;

	pixelsSize = newSize;

	pixels = new PixelType[newSize];
	bAllocated = true;
	pixelsOwner = true;
}

template<typename PixelType>
void ofPixels_<PixelType>::allocate(int w, int h, ofImageType type){
	allocate(w,h,ofPixelFormatFromImageType(type));
}

template<typename PixelType>
void ofPixels_<PixelType>::swapRgb(){
	switch(pixelFormat){
	case OF_PIXELS_RGB:
	case OF_PIXELS_BGR:{
		int sizePixels = width*height*3;
		for (int i=0; i< sizePixels; i+=3){
			std::swap(pixels[i],pixels[i+2]);
		}
	}
	break;
	case OF_PIXELS_RGBA:
	case OF_PIXELS_BGRA:{
		int sizePixels = width*height*4;
		for (int i=0; i< sizePixels; i+=4){
			std::swap(pixels[i],pixels[i+2]);
		}
	}
	break;
	default:
		ofLogWarning("ofPixels") << "rgb swap not supported for this pixel format";
		break;
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
	pixelFormat		= OF_PIXELS_UNKOWN;
	pixelsSize		= 0;
	bAllocated		= false;
}

template<typename PixelType>
int ofPixels_<PixelType>::getPixelIndex(int x, int y) const {
	if( !bAllocated ){
		return 0;
	}else{
		int pixelStride;
		switch(pixelFormat){
			case OF_PIXELS_RGB:
			case OF_PIXELS_BGR:
				pixelStride = 3;
				return ( x + y * width ) * pixelStride;
				break;
			case OF_PIXELS_RGBA:
			case OF_PIXELS_BGRA:
				pixelStride = 4;
				return ( x + y * width ) * pixelStride;
				break;
			case OF_PIXELS_MONO:
				pixelStride = 1;
				return ( x + y * width ) * pixelStride;
				break;
			case OF_PIXELS_RGB565:
				pixelStride = 2;
				return ( x + y * width ) * pixelStride;
				break;
			case OF_PIXELS_NV12:
			case OF_PIXELS_YV12:
			case OF_PIXELS_I420:
			case OF_PIXELS_YUY2:
			case OF_PIXELS_UNKOWN:
			default:
				ofLogWarning() << "setting channels not supported for " << ofToString(pixelFormat) << " format";
				return 0;
				break;
		}
	}
}

template<typename PixelType>
ofColor_<PixelType> ofPixels_<PixelType>::getColor(int x, int y) const {
	ofColor_<PixelType> c;
	int index = getPixelIndex(x, y);

	switch(pixelFormat){
		case OF_PIXELS_RGB:
			c.set( pixels[index], pixels[index+1], pixels[index+2] );
			break;
		case OF_PIXELS_BGR:
			c.set( pixels[index+2], pixels[index+1], pixels[index] );
			break;
		case OF_PIXELS_RGBA:
			c.set( pixels[index], pixels[index+1], pixels[index+2], pixels[index+3] );
			break;
		case OF_PIXELS_BGRA:
			c.set( pixels[index+2], pixels[index+1], pixels[index], pixels[index+3] );
			break;
		case OF_PIXELS_MONO:
			c.set( pixels[index] );
			break;
		case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UNKOWN:
			ofLogWarning() << "returning color not supported yet for " << ofToString(pixelFormat) << " format";
			return 0;
			break;
	}

	return c;
}

template<typename PixelType>
void ofPixels_<PixelType>::setColor(int index, const ofColor_<PixelType>& color) {

	switch(pixelFormat){
		case OF_PIXELS_RGB:
			pixels[index] = color.r;
			pixels[index+1] = color.g;
			pixels[index+2] = color.b;
			break;
		case OF_PIXELS_BGR:
			pixels[index] = color.b;
			pixels[index+1] = color.g;
			pixels[index+2] = color.r;
			break;
		case OF_PIXELS_RGBA:
			pixels[index] = color.r;
			pixels[index+1] = color.g;
			pixels[index+2] = color.b;
			pixels[index+3] = color.a;
			break;
		case OF_PIXELS_BGRA:
			pixels[index] = color.b;
			pixels[index+1] = color.g;
			pixels[index+2] = color.r;
			pixels[index+3] = color.a;
			break;
		case OF_PIXELS_MONO:
			pixels[index] = color.getBrightness();
			break;
		case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UNKOWN:
			ofLogWarning("ofPixels") << "setting color not supported yet for " << ofToString(pixelFormat) << " format";
			break;
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::setColor(int x, int y, const ofColor_<PixelType>& color) {
	setColor(getPixelIndex(x, y), color);
}

template<typename PixelType>
void ofPixels_<PixelType>::setColor(const ofColor_<PixelType>& color) {
	int i = 0;
	switch(pixelFormat){
		case OF_PIXELS_RGB:
			while(i < size()) {
				pixels[i++] = color.r;
				pixels[i++] = color.g;
				pixels[i++] = color.b;
			}
			break;
		case OF_PIXELS_BGR:
			while(i < size()) {
				pixels[i++] = color.b;
				pixels[i++] = color.g;
				pixels[i++] = color.r;
			}
			break;
		case OF_PIXELS_RGBA:
			while(i < size()) {
				pixels[i++] = color.r;
				pixels[i++] = color.g;
				pixels[i++] = color.b;
				pixels[i++] = color.a;
			}
			break;
		case OF_PIXELS_BGRA:
			while(i < size()) {
				pixels[i++] = color.b;
				pixels[i++] = color.g;
				pixels[i++] = color.r;
				pixels[i++] = color.a;
			}
			break;
		case OF_PIXELS_MONO:{
			PixelType b = color.getBrightness();
			while(i < size()) {
				pixels[i++] = b;
			}
		}
			break;
		case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UNKOWN:
			ofLogWarning("ofPixels") << "setting color not supported yet for " << ofToString(pixelFormat) << " format";
			break;
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
	return pixelBytesFromPixelFormat(pixelFormat);
}

template<typename PixelType>
int ofPixels_<PixelType>::getBitsPerPixel() const{
	return pixelBytesFromPixelFormat(pixelFormat)*8;
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
	return channelsFromPixelFormat(pixelFormat);
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
ofPixelFormat ofPixels_<PixelType>::getPixelFormat() const{
	return pixelFormat;
}

template<typename PixelType>
void ofPixels_<PixelType>::setNumChannels(int numChannels){
	if(!isAllocated() || numChannels==getNumChannels()) return;
	setImageType(getImageTypeFromChannels(numChannels));
}

template<typename PixelType>
int ofPixels_<PixelType>::size() const{
	return pixelsSize;
}

template<typename PixelType>
ofPixels_<PixelType> ofPixels_<PixelType>::getChannel(int channel) const{
	ofPixels_<PixelType> channelPixels;
	int channels = channelsFromPixelFormat(pixelFormat);
	if(channels==0) return channelPixels;

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
	int channels = channelsFromPixelFormat(pixelFormat);
	if(channels==0) return;

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
	int channels = channelsFromPixelFormat(pixelFormat);
	if (bAllocated == true && channels>0){

		_width = ofClamp(_width,1,getWidth());
		_height = ofClamp(_height,1,getHeight());

		int bytesPerPixel = channelsFromPixelFormat(pixelFormat);
		if(bytesPerPixel==0) return;

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

		int bytesPerPixel = channelsFromPixelFormat(pixelFormat);
		if(bytesPerPixel==0) return;

		if ((toPix.width != _width) || (toPix.height != _height) || (toPix.pixelFormat != pixelFormat)){
			toPix.allocate(_width, _height, pixelFormat);
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
	int channels = channelsFromPixelFormat(pixelFormat);

	if (bAllocated == false || channels==0){
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
	int channels = channelsFromPixelFormat(pixelFormat);

	if (bAllocated == false || channels==0){
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
	int channels = channelsFromPixelFormat(pixelFormat);

	if ((!vertically && !horizontal) || channels==0){
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
	int channels = channelsFromPixelFormat(pixelFormat);
	if(channels==0) return;

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
