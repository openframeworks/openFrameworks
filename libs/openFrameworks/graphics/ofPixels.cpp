#include "ofPixels.h"
#include "ofMath.h"


template<typename T>
ofPixels_<T>::ofPixels_(){
	bAllocated = false;
	pixelsOwner = false;
	channels = 0;
	pixels = NULL;
	clear();
}


template<typename T>
ofPixels_<T>::~ofPixels_(){
	clear();
}

template<typename T>
ofPixels_<T>::ofPixels_(const ofPixels_<T> & mom){
	bAllocated = false;
	pixelsOwner = false;
	channels = 0;
	pixels = NULL;
	(*this).template copyFrom( mom );
}

template<typename T>
ofPixels_<T>& ofPixels_<T>::operator=(const ofPixels_<T> & mom){
	if(this==&mom) {
		return * this;
	}
	(*this).template copyFrom( mom );
	return *this;
}

template<typename T>
void ofPixels_<T>::copyFrom(const ofPixels_<T> & mom){
	if(mom.isAllocated()) {
		allocate(mom.getWidth(), mom.getHeight(), mom.getNumChannels());
		memcpy(pixels, mom.getPixels(), mom.getWidth() * mom.getHeight() * mom.getBytesPerPixel());
	}
}

template<typename T>
void ofPixels_<T>::set(T val){
	int size = width * height * channels;
	for(int i = 0; i < size; i++){
		pixels[i] = val;
	}
}

template<typename T>
void ofPixels_<T>::set(int channel,T val){
	int size = width*height*channels;
	for(int i=channel;i<size;i+=channels){
		pixels[i] = val;
	}
}

template<typename T>
void ofPixels_<T>::setFromPixels(const T * newPixels,int w, int h, int channels){
	allocate(w, h, channels);
	memcpy(pixels, newPixels, w * h * getBytesPerPixel());
}

template<typename T>
void ofPixels_<T>::setFromPixels(const T * newPixels,int w, int h, ofImageType type){
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

template<typename T>
void ofPixels_<T>::setFromExternalPixels(T * newPixels,int w, int h, int _channels){
	clear();
	channels = _channels;
	width= w;
	height = h;

	pixels = newPixels;
	pixelsOwner = false;
	bAllocated = true;
}

template<typename T>
void ofPixels_<T>::setFromAlignedPixels(const T * newPixels, int width, int height, int channels, int stride) {
	allocate(width, height, channels);
	int dstStride = width * getBytesPerPixel();
	cout << "copying: " << width << " " << height << " " << channels << " " << getBytesPerPixel() << endl;
	const T * src = newPixels;
	T * dst = pixels;
	for(int i = 0; i < height; i++) {
		memcpy(dst, src, dstStride);
		src += stride;
		dst += dstStride;
	}
}

template<typename T>
T * ofPixels_<T>::getPixels(){
	return &pixels[0];
}

template<typename T>
const T * ofPixels_<T>::getPixels() const{
	return &pixels[0];
}


/*template<typename T>
void ofPixels_<T>::allocate(int w, int h, int bitsPerPixel){
	ofImageType type = getImageTypeFromBits(bitsPerPixel);
	allocate(w,h,type);
}*/

template<typename T>
void ofPixels_<T>::allocate(int w, int h, int _channels){
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

	pixels = new T[w * h * channels];
	bAllocated = true;
	pixelsOwner = true;
}


template<typename T>
void ofPixels_<T>::allocate(int w, int h, ofPixelFormat format){

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

template<typename T>
void ofPixels_<T>::allocate(int w, int h, ofImageType type){
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

template<typename T>
void ofPixels_<T>::swapRgb(){
	if (channels >= 3){
		int sizePixels		= width*height;
		int cnt				= 0;
		T * pixels_ptr = pixels;

		while (cnt < sizePixels){
			std::swap(pixels_ptr[0],pixels_ptr[2]);
			cnt++;
			pixels_ptr+=channels;
		}
	}
}

template<typename T>
void ofPixels_<T>::clear(){
	if(pixels){
		if(pixelsOwner) delete[] pixels;
		pixels = NULL;
	}

	width			= 0;
	height			= 0;
	channels		= 0;
	bAllocated		= false;
}

template<typename T>
int ofPixels_<T>::getPixelIndex(int x, int y) const {
	if( !bAllocated ){
		return 0;
	}else{
		return ( x + y * width ) * channels;
	}
}

template<typename T>
ofColor ofPixels_<T>::getColor(int x, int y) const {
	ofColor c;
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

template<typename T>
void ofPixels_<T>::setColor(int x, int y, ofColor color) {
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

template<typename T>
T & ofPixels_<T>::operator[](int pos){
	return pixels[pos];
}

template<typename T>
const T & ofPixels_<T>::operator[](int pos) const{
	return pixels[pos];
}

template<typename T>
bool ofPixels_<T>::isAllocated() const{
	return bAllocated;
}

template<typename T>
int ofPixels_<T>::getWidth() const{
	return width;
}

template<typename T>
int ofPixels_<T>::getHeight() const{
	return height;
}

template<typename T>
int ofPixels_<T>::getBytesPerPixel() const{
	return getBytesPerChannel() * channels;
}

template<typename T>
int ofPixels_<T>::getBitsPerPixel() const{
	return getBitsPerChannel() * channels;
}

template<typename T>
int ofPixels_<T>::getBytesPerChannel() const{
	return sizeof(T);
}

template<typename T>
int ofPixels_<T>::getBitsPerChannel() const{
	return getBytesPerChannel() * 8;
}

template<typename T>
int ofPixels_<T>::getNumChannels() const{
	return channels;
}

template<typename T>
ofImageType ofPixels_<T>::getImageType() const{
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

template<typename T>
int ofPixels_<T>::size() const{
	return width*height*channels;
}

template<typename T>
ofPixels_<T> ofPixels_<T>::getChannel(int channel) const{
	ofPixels_<T> channelPixels;
	channelPixels.allocate(width,height,1);
	channel = ofClamp(channel,0,channels-1);
	int j=0;
	for(int i=channel;i<size();i+=channels, ++j){
		channelPixels[j]=pixels[i];
	}
	return channelPixels;
}

template<typename T>
void ofPixels_<T>::setChannel(int channel, const ofPixels_<T> channelPixels){
	channel = ofClamp(channel,0,channels-1);
	int j=0;
	for(int i=channel;i<size();i+=channels, ++j){
		pixels[i]=channelPixels[j];
	}

}

template class ofPixels_<unsigned char>;
template class ofPixels_<float>;
template class ofPixels_<unsigned short>;
