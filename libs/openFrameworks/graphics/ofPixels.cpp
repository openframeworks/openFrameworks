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

template class ofPixels_<unsigned char>;
template class ofPixels_<float>;
template class ofPixels_<unsigned short>;
