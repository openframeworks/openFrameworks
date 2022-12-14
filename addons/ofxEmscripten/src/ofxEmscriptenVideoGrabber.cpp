/*
 * ofEmscriptenGrabber.cpp
 *
 *  Created on: May 12, 2014
 *      Author: arturo
 */

#include "ofxEmscriptenVideoGrabber.h"
#include "html5video.h"

using namespace std;

enum ReadyState{
	HAVE_NOTHING = 0,
	HAVE_METADATA,
	HAVE_CURRENT_DATA,
	HAVE_FUTURE_DATA,
	HAVE_ENOUGH_DATA
};

ofxEmscriptenVideoGrabber::ofxEmscriptenVideoGrabber()
:id(html5video_grabber_create())
,desiredFramerate(-1)
,usePixels(true){
	// TODO Auto-generated constructor stub

}

ofxEmscriptenVideoGrabber::~ofxEmscriptenVideoGrabber() {
	// TODO Auto-generated destructor stub
}

vector<ofVideoDevice> ofxEmscriptenVideoGrabber::listDevices() const{
	html5video_list_devices();
	return vector<ofVideoDevice>();
}

bool ofxEmscriptenVideoGrabber::setup(int w, int h){
	if(id!=-1){
		html5video_grabber_init(id,w,h,desiredFramerate);
		switch(getPixelFormat()){
		case OF_PIXELS_RGBA:
			pixels.allocate(w,h,4);
			break;
		case OF_PIXELS_RGB:
			pixels.allocate(w,h,3);
			break;
		case OF_PIXELS_MONO:
			pixels.allocate(w,h,1);
			break;
		default:
			ofLogError() << "unknown pixel format, can't allocating texture";
			break;
		}
		pixels.set(0);
		return true;
	}else{
		return false;
	}
}

bool ofxEmscriptenVideoGrabber::isInitialized() const{
	return html5video_grabber_ready_state(id)>=HAVE_ENOUGH_DATA;
}

void ofxEmscriptenVideoGrabber::update(){
	if(html5video_grabber_update(id,usePixels,pixels.getData())){
		texture.texData.width = html5video_grabber_width(id);
		texture.texData.height = html5video_grabber_height(id);
		texture.texData.tex_w = texture.texData.width;
		texture.texData.tex_h = texture.texData.height;
		if(texture.texData.textureID!=html5video_grabber_texture_id(id)){
			texture.texData.bFlipTexture = false;
			switch(getPixelFormat()){
			case OF_PIXELS_RGBA:
				texture.texData.glInternalFormat = GL_RGBA;
				break;
			case OF_PIXELS_RGB:
				texture.texData.glInternalFormat = GL_RGB;
				break;
			case OF_PIXELS_MONO:
				texture.texData.glInternalFormat = GL_LUMINANCE;
				break;
			default:
				ofLogError() << "unknown pixel format, can't allocating texture";
				break;
			}
			texture.texData.tex_u = 1;
			texture.texData.tex_t = 1;
			texture.texData.textureTarget = GL_TEXTURE_2D;
			texture.texData.bAllocated = true;
			texture.setUseExternalTextureID(html5video_grabber_texture_id(id));
		}
	}
}

bool ofxEmscriptenVideoGrabber::isFrameNew() const{
	// does not work with Emscripten
	if (pixels.isAllocated() || texture.isAllocated()){
		return true;
	} else{
		return false;
	}
}

ofPixels & ofxEmscriptenVideoGrabber::getPixels(){
	return pixels;
}

const ofPixels & ofxEmscriptenVideoGrabber::getPixels() const{
	return pixels;
}

void ofxEmscriptenVideoGrabber::close(){

}

float ofxEmscriptenVideoGrabber::getHeight() const{
	return texture.getHeight();
}

float ofxEmscriptenVideoGrabber::getWidth() const{
	return texture.getWidth();
}

bool ofxEmscriptenVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat){
	switch(pixelFormat){
	case OF_PIXELS_RGBA:
		html5video_grabber_set_pixel_format(id,"RGBA");
		break;
	case OF_PIXELS_RGB:
		html5video_grabber_set_pixel_format(id,"RGB");
		break;
	case OF_PIXELS_MONO:
		html5video_grabber_set_pixel_format(id,"GRAY");
		break;
	default:
		ofLogError() << "can't set pixel format";
		return false;
	}
	return true;
}

ofPixelFormat ofxEmscriptenVideoGrabber::getPixelFormat() const{
	string format = html5video_grabber_pixel_format(id);
	if(format == "RGB"){
		return OF_PIXELS_RGB;
	}else if(format == "RGBA"){
		return OF_PIXELS_RGBA;
	}else if(format == "GRAY"){
		return OF_PIXELS_MONO;
	}else{
		return OF_PIXELS_UNKNOWN;
	}
}

ofTexture * ofxEmscriptenVideoGrabber::getTexture(){
	return &texture;
}

//should implement!
void ofxEmscriptenVideoGrabber::setDeviceID(int _deviceID){

}

void ofxEmscriptenVideoGrabber::setDesiredFrameRate(int framerate){
	desiredFramerate = framerate;
}

void ofxEmscriptenVideoGrabber::videoSettings(){

}

void ofxEmscriptenVideoGrabber::setUsePixels(bool usePixels){
	this->usePixels = usePixels;
}
