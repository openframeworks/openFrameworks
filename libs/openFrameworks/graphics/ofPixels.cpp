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
static float pixelBytesFromPixelFormat(ofPixelFormat format){
	switch(format){
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
			return 3 * sizeof(PixelType);

		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
			return 4 * sizeof(PixelType);

		case OF_PIXELS_GRAY:
		case OF_PIXELS_Y:
		case OF_PIXELS_U:
		case OF_PIXELS_V:
			return 1 * sizeof(PixelType);

		case OF_PIXELS_NV12:
		case OF_PIXELS_NV21:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
			return 1.5;

		case OF_PIXELS_UV:
		case OF_PIXELS_VU:
		case OF_PIXELS_GRAY_ALPHA:
			return 2 * sizeof(PixelType);

		case OF_PIXELS_YUY2:
		case OF_PIXELS_UYVY:
		case OF_PIXELS_RGB565:
			return 2;
			break;
		default:
			return 0;
	}

}

template<typename PixelType>
static int bytesFromPixelFormat(int w, int h, ofPixelFormat format){
	return w*h*pixelBytesFromPixelFormat<PixelType>(format);
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
	case OF_PIXELS_GRAY:
		return 1;
		break;
	case OF_PIXELS_GRAY_ALPHA:
	case OF_PIXELS_RGB565:
		return 2;
		break;
	case OF_PIXELS_NV12:
	case OF_PIXELS_NV21:
	case OF_PIXELS_YV12:
	case OF_PIXELS_I420:
		return 1;
		break;
	case OF_PIXELS_YUY2:
	case OF_PIXELS_UYVY:
		return 2;
		break;
	case OF_PIXELS_Y:
	case OF_PIXELS_U:
	case OF_PIXELS_V:
		return 1;
		break;
	case OF_PIXELS_UV:
	case OF_PIXELS_VU:
		return 2;
		break;
	default:
		ofLog(OF_LOG_ERROR,"ofPixels: format doesn't support channels");
		return 1;
	}
}

static ofPixelFormat ofPixelFormatFromImageType(ofImageType type){
	switch(type){
	case OF_IMAGE_GRAYSCALE:
		return OF_PIXELS_GRAY;
		break;
	case OF_IMAGE_COLOR:
		return OF_PIXELS_RGB;
		break;
	case OF_IMAGE_COLOR_ALPHA:
		return OF_PIXELS_RGBA;
		break;
	default:
		ofLog(OF_LOG_ERROR,"ofPixels: image type not supported");
		return OF_PIXELS_UNKNOWN;
	}
}

static ofImageType ofImageTypeFromPixelFormat(ofPixelFormat pixelFormat){
	switch(pixelFormat){
	case OF_PIXELS_GRAY:
		return OF_IMAGE_GRAYSCALE;
		break;
	case OF_PIXELS_RGB:
		return OF_IMAGE_COLOR;
		break;
	case OF_PIXELS_RGBA:
		return OF_IMAGE_COLOR_ALPHA;
		break;
	case OF_PIXELS_UNKNOWN:
		return OF_IMAGE_UNDEFINED;
	default:
		ofLog(OF_LOG_ERROR,"ofPixels: image type not supported");
		return OF_IMAGE_UNDEFINED;
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
		case OF_PIXELS_GRAY:
			return "GRAY";
		case OF_PIXELS_RGB565:
			return "RGB565";
		case OF_PIXELS_NV12:
			return "NV12";
		case OF_PIXELS_NV21:
			return "NV21";
		case OF_PIXELS_YV12:
			return "YV12";
		case OF_PIXELS_I420:
			return "I420";
		case OF_PIXELS_YUY2:
			return "YUY2";
		case OF_PIXELS_UYVY:
			return "UYVY";
		default:
			return "UNKOWN";
	}
}

static ofPixelFormat pixelFormatFromNumChannels(int channels){
	switch(channels){
	case 1: return OF_PIXELS_GRAY;
	case 2: return OF_PIXELS_GRAY_ALPHA;
	case 3: return OF_PIXELS_RGB;
	case 4: return OF_PIXELS_RGBA;
	default: return OF_PIXELS_UNKNOWN;
	}
}

template<typename PixelType>
ofPixels_<PixelType>::ofPixels_(){
	bAllocated = false;
	pixelsOwner = false;
	pixelFormat = OF_PIXELS_UNKNOWN;
	pixels = NULL;
	pixelsSize = 0;
	width = 0;
	height = 0;
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
	pixelFormat = OF_PIXELS_UNKNOWN;
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
		memcpy(pixels, mom.getData(), mom.size() * sizeof(PixelType));
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::set(PixelType val){
	iterator _end = end();
	for(iterator i=begin();i<_end;++i){
		*i = val;
	}
}

template<typename PixelType>
void ofPixels_<PixelType>::set(int channel,PixelType val){
	switch(pixelFormat){
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
		case OF_PIXELS_GRAY:
		case OF_PIXELS_GRAY_ALPHA:{
			Pixels pixels = getPixelsIter();
			Pixel _end = pixels.end();
			for(Pixel pixel=pixels.begin();pixel!=_end;pixel++){
				pixel[channel] = val;
			}
		}
		break;
		case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12:
		case OF_PIXELS_NV21:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UYVY:
		case OF_PIXELS_Y:
		case OF_PIXELS_U:
		case OF_PIXELS_V:
		case OF_PIXELS_UV:
		case OF_PIXELS_VU:
		case OF_PIXELS_UNKNOWN:
		default:
			ofLogWarning() << "setting channels not supported for " << ofToString(pixelFormat) << " format";
			break;
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
	setFromPixels(newPixels,w,h,ofPixelFormatFromImageType(type));
}

template<typename PixelType>
void ofPixels_<PixelType>::setFromPixels(const PixelType * newPixels, int w, int h, ofPixelFormat format){
	allocate(w,h,format);
	memcpy(pixels, newPixels, w * h * getBytesPerPixel());
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

	pixelsSize = bytesFromPixelFormat<PixelType>(w,h,_pixelFormat);

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
ofPixels_<PixelType>::operator PixelType*(){
	return pixels;
}

template<typename PixelType>
ofPixels_<PixelType>::operator const PixelType*(){
	return pixels;
}

template<typename PixelType>
PixelType * ofPixels_<PixelType>::getPixels(){
	return pixels;
}

template<typename PixelType>
const PixelType * ofPixels_<PixelType>::getPixels() const{
	return pixels;
}

template<typename PixelType>
PixelType * ofPixels_<PixelType>::getData(){
	return pixels;
}

template<typename PixelType>
const PixelType * ofPixels_<PixelType>::getData() const{
	return pixels;
}

template<typename PixelType>
void ofPixels_<PixelType>::allocate(int w, int h, int _channels){
	allocate(w,h,pixelFormatFromNumChannels(_channels));
}

template<typename PixelType>
void ofPixels_<PixelType>::allocate(int w, int h, ofPixelFormat format){
	if (w <= 0 || h <= 0 || format==OF_PIXELS_UNKNOWN) {
		return;
	}

	int newSize = bytesFromPixelFormat<PixelType>(w,h,format);
	//we check if we are already allocated at the right size
	if(bAllocated && newSize==size()){
        pixelFormat = format;
        width = w;
        height = h;
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
	case OF_PIXELS_BGR:
	case OF_PIXELS_RGBA:
	case OF_PIXELS_BGRA:{
		Pixels pixels = getPixelsIter();
		Pixel _end = pixels.end();
		for(Pixel pixel=pixels.begin();pixel!=_end;pixel++){
			std::swap(pixel[0],pixel[2]);
		}
	}
	break;
	default:
		ofLogWarning("ofPixels") << "rgb swap not supported for this pixel format";
		break;
	}
	switch(pixelFormat){
	case OF_PIXELS_RGB:
		pixelFormat = OF_PIXELS_BGR;
		break;
	case OF_PIXELS_BGR:
		pixelFormat = OF_PIXELS_RGB;
		break;
	case OF_PIXELS_RGBA:
		pixelFormat = OF_PIXELS_BGRA;
		break;
	case OF_PIXELS_BGRA:
		pixelFormat = OF_PIXELS_RGBA;
		break;
	default:
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
	pixelFormat		= OF_PIXELS_UNKNOWN;
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
			case OF_PIXELS_GRAY:
				pixelStride = 1;
				return ( x + y * width ) * pixelStride;
				break;
			case OF_PIXELS_GRAY_ALPHA:
				pixelStride = 2;
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
			case OF_PIXELS_UYVY:
			case OF_PIXELS_Y:
			case OF_PIXELS_U:
			case OF_PIXELS_V:
			case OF_PIXELS_UV:
			case OF_PIXELS_VU:
			case OF_PIXELS_UNKNOWN:
			default:
				ofLogWarning() << "getting pixel index not supported for " << ofToString(pixelFormat) << " format";
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
		case OF_PIXELS_GRAY:
			c.set( pixels[index] );
			break;
		case OF_PIXELS_GRAY_ALPHA:
			c.set( pixels[index], pixels[index], pixels[index], pixels[index+1] );
			break;
		case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12:
		case OF_PIXELS_NV21:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UYVY:
		case OF_PIXELS_Y:
		case OF_PIXELS_U:
		case OF_PIXELS_V:
		case OF_PIXELS_UV:
		case OF_PIXELS_VU:
		case OF_PIXELS_UNKNOWN:
		default:
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
		case OF_PIXELS_GRAY:
			pixels[index] = color.getBrightness();
			break;
		case OF_PIXELS_GRAY_ALPHA:
			pixels[index] = color.getBrightness();
			pixels[index+1] = color.a;
			break;
		case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12:
		case OF_PIXELS_NV21:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UYVY:
		case OF_PIXELS_Y:
		case OF_PIXELS_U:
		case OF_PIXELS_V:
		case OF_PIXELS_UV:
		case OF_PIXELS_VU:
		case OF_PIXELS_UNKNOWN:
		default:
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
	switch(pixelFormat){
		case OF_PIXELS_RGB:{
			Pixels pixels = getPixelsIter();
			Pixel _end = pixels.end();
			for(Pixel pixel=pixels.begin();pixel!=_end;pixel++){
				pixel[0] = color.r;
				pixel[1] = color.g;
				pixel[2] = color.b;
			}
		}
		break;
		case OF_PIXELS_BGR:{
			Pixels pixels = getPixelsIter();
			Pixel _end = pixels.end();
			for(Pixel pixel=pixels.begin();pixel!=_end;pixel++){
				pixel[0] = color.b;
				pixel[1] = color.g;
				pixel[2] = color.r;
			}
		}
		break;
		case OF_PIXELS_RGBA:{
			Pixels pixels = getPixelsIter();
			Pixel _end = pixels.end();
			for(Pixel pixel=pixels.begin();pixel!=_end;pixel++){
				pixel[0] = color.r;
				pixel[1] = color.g;
				pixel[2] = color.b;
				pixel[3] = color.a;
			}
		}
		break;
		case OF_PIXELS_BGRA:{
			Pixels pixels = getPixelsIter();
			Pixel _end = pixels.end();
			for(Pixel pixel=pixels.begin();pixel!=_end;pixel++){
				pixel[0] = color.b;
				pixel[1] = color.g;
				pixel[2] = color.r;
				pixel[3] = color.a;
			}
		}
		break;
		case OF_PIXELS_GRAY:{
			PixelType b = color.getBrightness();
			for(iterator i=begin();i!=end();++i){
				*i = b;
			}
		}
		break;
		case OF_PIXELS_GRAY_ALPHA:{
			PixelType b = color.getBrightness();
			Pixels pixels = getPixelsIter();
			Pixel _end = pixels.end();
			for(Pixel pixel=pixels.begin();pixel!=_end;pixel++){
				pixel[0] = b;
				pixel[1] = color.a;
			}
		}
		break;
		case OF_PIXELS_RGB565:
		case OF_PIXELS_NV12:
		case OF_PIXELS_NV21:
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UYVY:
		case OF_PIXELS_Y:
		case OF_PIXELS_U:
		case OF_PIXELS_V:
		case OF_PIXELS_UV:
		case OF_PIXELS_VU:
		case OF_PIXELS_UNKNOWN:
		default:
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
	return pixelBytesFromPixelFormat<PixelType>(pixelFormat);
}

template<typename PixelType>
int ofPixels_<PixelType>::getBitsPerPixel() const{
	return pixelBytesFromPixelFormat<PixelType>(pixelFormat)*8;
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
int ofPixels_<PixelType>::getBytesStride() const{
	return pixelBytesFromPixelFormat<PixelType>(pixelFormat) * width;
}

template<typename PixelType>
int ofPixels_<PixelType>::getNumChannels() const{
	return channelsFromPixelFormat(pixelFormat);
}

template<typename PixelType>
int ofPixels_<PixelType>::getTotalBytes() const{
	return bytesFromPixelFormat<PixelType>(width,height,pixelFormat);
}

template<typename PixelType>
int ofPixels_<PixelType>::getNumPlanes() const{
	switch(pixelFormat){
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
		case OF_PIXELS_RGB565:
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
		case OF_PIXELS_GRAY:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UYVY:
		case OF_PIXELS_Y:
		case OF_PIXELS_U:
		case OF_PIXELS_V:
		case OF_PIXELS_UV:
		case OF_PIXELS_VU:
			return 1;
			break;
		case OF_PIXELS_NV12:
		case OF_PIXELS_NV21:
			return 2;
			break;
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
			return 3;
			break;
		default:
			return 0;
	}
}

template<typename PixelType>
ofPixels_<PixelType> ofPixels_<PixelType>::getPlane(int planeIdx){
	planeIdx = ofClamp(planeIdx,0,getNumPlanes());
	ofPixels_<PixelType> plane;
	switch(pixelFormat){
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
		case OF_PIXELS_RGB565:
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
		case OF_PIXELS_GRAY:
		case OF_PIXELS_YUY2:
		case OF_PIXELS_UYVY:
			plane.setFromExternalPixels(pixels,width,height,pixelFormat);
			break;
		case OF_PIXELS_NV12:
			switch(planeIdx){
			case 0:
				plane.setFromExternalPixels(pixels,width,height,OF_PIXELS_Y);
				break;
			case 1:
				plane.setFromExternalPixels(pixels+width*height,width/2,height/2,OF_PIXELS_UV);
				break;
			}
			break;
		case OF_PIXELS_NV21:
			switch(planeIdx){
			case 0:
				plane.setFromExternalPixels(pixels,width,height,OF_PIXELS_Y);
				break;
			case 1:
				plane.setFromExternalPixels(pixels+width*height,width/2,height/2,OF_PIXELS_VU);
				break;
			}
			break;
		case OF_PIXELS_YV12:
			switch(planeIdx){
			case 0:
				plane.setFromExternalPixels(pixels,width,height,OF_PIXELS_Y);
				break;
			case 1:
				plane.setFromExternalPixels(pixels+width*height,width/2,height/2,OF_PIXELS_V);
				break;
			case 2:
				plane.setFromExternalPixels(pixels+int(width*height+width/2*height/2),width/2,height/2,OF_PIXELS_U);
				break;
			}
			break;
		case OF_PIXELS_I420:
			switch(planeIdx){
			case 0:
				plane.setFromExternalPixels(pixels,width,height,OF_PIXELS_Y);
				break;
			case 1:
				plane.setFromExternalPixels(pixels+width*height,width/2,height/2,OF_PIXELS_U);
				break;
			case 2:
				plane.setFromExternalPixels(pixels+int(width*height+width/2*height/2),width/2,height/2,OF_PIXELS_V);
				break;
			}
			break;
		default:
			break;
	}
	return plane;
}

template<typename PixelType>
ofImageType ofPixels_<PixelType>::getImageType() const{
	return ofImageTypeFromPixelFormat(pixelFormat);
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
	iterator channelPixel = channelPixels.begin();
	const_iterator _end = end();
	for(const_iterator i=begin()+channel;i<_end;i+=channels,++channelPixel){
		*channelPixel = *i;
	}
	return channelPixels;
}

template<typename PixelType>
void ofPixels_<PixelType>::setChannel(int channel, const ofPixels_<PixelType> channelPixels){
	int channels = channelsFromPixelFormat(pixelFormat);
	if(channels==0) return;

	channel = ofClamp(channel,0,channels-1);
	const_iterator channelPixel = channelPixels.begin();
	iterator _end = end();
	for(iterator i=begin()+channel;i<_end;i+=channels,++channelPixel){
		*i = *channelPixel;
	}

}

//From ofPixelsUtils
//----------------------------------------------------------------------
template<typename PixelType>
void ofPixels_<PixelType>::crop(int x, int y, int _width, int _height){
	if (bAllocated){
		ofPixels_<PixelType> crop;
		cropTo(crop,x,y,_width,_height);
		swap(crop);
	}
}

//----------------------------------------------------------------------
template<typename PixelType>
void ofPixels_<PixelType>::cropTo(ofPixels_<PixelType> &toPix, int x, int y, int _width, int _height) const{
	if (bAllocated){

		if(&toPix == this){
			toPix.crop(x,y,_width,_height);
			return;
		}

		_width = ofClamp(_width,1,getWidth());
		_height = ofClamp(_height,1,getHeight());

		if ((toPix.width != _width) || (toPix.height != _height) || (toPix.pixelFormat != pixelFormat)){
			toPix.allocate(_width, _height, pixelFormat);
		}

		// this prevents having to do a check for bounds in the for loop;
		int minX = MAX(x, 0) * getNumChannels();
		int maxX = MIN(x+_width, width) * getNumChannels();
		int minY = MAX(y, 0);
		int maxY = MIN(y+_height, height);


		iterator newPixel = toPix.begin();
		for(ConstLine line = getConstLines().begin()+minY; line!=getConstLines().begin()+maxY; ++line ){
			for(const_iterator pixel = line.begin()+minX; pixel<line.begin()+maxX; ++pixel){
				*newPixel++ = *pixel;
			}
		}
	}
}

//----------------------------------------------------------------------
template<typename PixelType>
void ofPixels_<PixelType>::rotate90To(ofPixels_<PixelType> & dst, int nClockwiseRotations) const{
	int channels = channelsFromPixelFormat(pixelFormat);

	if (bAllocated == false || channels==0){
		return;
	}

	if(&dst == this){
		dst.rotate90(nClockwiseRotations);
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
		PixelType * startPixels = dst.getData() + (strideDst - channels);
		for (int i = 0; i < height; ++i, --startPixels){
			PixelType * dstPixels = startPixels;
			for (int j = 0; j < width; ++j){
				for (int k = 0; k < channels; ++k){
					dstPixels[k] = srcPixels[k];
				}
				srcPixels += channels;
				dstPixels += strideDst;
			}
		}
	} else if(rotation == 3){
		PixelType * dstPixels = dst.pixels;
		PixelType * startPixels = pixels + (strideSrc - channels);
		for (int i = 0; i < dst.height; ++i, --startPixels){
			PixelType * srcPixels = startPixels;
			for (int j = 0; j < dst.width; ++j){
				for (int k = 0; k < channels; ++k){
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
	std::swap(newPixels.pixels,pixels);
	width = newPixels.width;
	height = newPixels.height;

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
void ofPixels_<PixelType>::mirrorTo(ofPixels_<PixelType> & dst, bool vertically, bool horizontal) const{
	if(&dst == this){
		dst.mirror(vertically,horizontal);
		return;
	}

	if (!vertically && !horizontal){
		dst = *this;
		return;
	}

	int bytesPerPixel = getNumChannels();

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
	pixels = dstPixels.getData();
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
bool ofPixels_<PixelType>::resizeTo(ofPixels_<PixelType>& dst, ofInterpolationMethod interpMethod) const{
	if(&dst == this){
		return true;
	}

	if (!(isAllocated()) || !(dst.isAllocated()) || getBytesPerPixel() != dst.getBytesPerPixel()) return false;

	int srcWidth      = getWidth();
	int srcHeight     = getHeight();
	int dstWidth	  = dst.getWidth();
	int dstHeight	  = dst.getHeight();
	int bytesPerPixel = getBytesPerPixel();


	PixelType * dstPixels = dst.getData();

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
bool ofPixels_<PixelType>::pasteInto(ofPixels_<PixelType> &dst, int xTo, int yTo) const{
	if (!(isAllocated()) || !(dst.isAllocated()) || getBytesPerPixel() != dst.getBytesPerPixel() || xTo>=dst.getWidth() || yTo>=dst.getHeight()) return false;

	int bytesToCopyPerRow = (xTo + getWidth()<=dst.getWidth() ? getWidth() : dst.getWidth()-xTo) * getBytesPerPixel();
	int columnsToCopy = yTo + getHeight() <= dst.getHeight() ? getHeight() : dst.getHeight()-yTo;
	PixelType * dstPix = dst.getData() + ((xTo + yTo*dst.getWidth())*dst.getBytesPerPixel());
	const PixelType * srcPix = getData();
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
