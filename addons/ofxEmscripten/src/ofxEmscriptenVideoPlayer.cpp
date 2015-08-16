/*
 * ofEmscriptenVideoPlayer.cpp
 *
 *  Created on: May 10, 2014
 *      Author: arturo
 */

#include "ofxEmscriptenVideoPlayer.h"
#include "html5video.h"

enum ReadyState{
	HAVE_NOTHING = 0,
	HAVE_METADATA,
	HAVE_CURRENT_DATA,
	HAVE_FUTURE_DATA,
	HAVE_ENOUGH_DATA
};

ofxEmscriptenVideoPlayer::ofxEmscriptenVideoPlayer()
:id(html5video_player_create())
,gotFirstFrame(false)
,usePixels(true){


}

ofxEmscriptenVideoPlayer::~ofxEmscriptenVideoPlayer() {
	html5video_player_delete(id);
}

bool ofxEmscriptenVideoPlayer::load(string name){
	html5video_player_load(id,ofToDataPath(name).c_str());
	return true;
}

void ofxEmscriptenVideoPlayer::close(){
	html5video_player_delete(id);
	id = html5video_player_create();
	gotFirstFrame = false;
}


void ofxEmscriptenVideoPlayer::update(){
	gotFirstFrame = pixels.isAllocated();
	if(html5video_player_update(id,pixels.isAllocated() && usePixels,pixels.getData())){
		if(texture.texData.width!=html5video_player_width(id) || texture.texData.height!=html5video_player_height(id)){
			texture.texData.width = html5video_player_width(id);
			texture.texData.height = html5video_player_height(id);
			texture.texData.tex_w = texture.texData.width;
			texture.texData.tex_h = texture.texData.height;
			switch(getPixelFormat()){
			case OF_PIXELS_RGBA:
				pixels.allocate(texture.texData.width,texture.texData.height,4);
				break;
			case OF_PIXELS_RGB:
				pixels.allocate(texture.texData.width,texture.texData.height,3);
				break;
			case OF_PIXELS_MONO:
				pixels.allocate(texture.texData.width,texture.texData.height,1);
				break;
			default:
				ofLogError() << "unknown pixel format, can't allocating texture";
				break;
			}
		}
		if(texture.texData.textureID!=html5video_player_texture_id(id)){
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
			texture.setUseExternalTextureID(html5video_player_texture_id(id));
		}
	}
}



void ofxEmscriptenVideoPlayer::play(){
	html5video_player_play(id);
}


void ofxEmscriptenVideoPlayer::stop(){
	html5video_player_stop(id);
}

bool ofxEmscriptenVideoPlayer::isFrameNew() const{
	return gotFirstFrame;
}

ofPixels & ofxEmscriptenVideoPlayer::getPixels(){
	return pixels;
}

const ofPixels & ofxEmscriptenVideoPlayer::getPixels() const{
	return pixels;
}

ofTexture * ofxEmscriptenVideoPlayer::getTexture(){
	return &texture;
}

float ofxEmscriptenVideoPlayer::getWidth() const{
	return texture.getWidth();
}

float ofxEmscriptenVideoPlayer::getHeight() const{
	return texture.getHeight();
}

bool ofxEmscriptenVideoPlayer::isPaused() const{
	return html5video_player_is_paused(id);
}

bool ofxEmscriptenVideoPlayer::isLoaded() const{
	return html5video_player_ready_state(id) == HAVE_ENOUGH_DATA;
}

bool ofxEmscriptenVideoPlayer::isPlaying() const{
	return !isPaused();
}


bool ofxEmscriptenVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat){
	switch(pixelFormat){
	case OF_PIXELS_RGBA:
		html5video_player_set_pixel_format(id,"RGBA");
		break;
	case OF_PIXELS_RGB:
		html5video_player_set_pixel_format(id,"RGB");
		break;
	case OF_PIXELS_MONO:
		html5video_player_set_pixel_format(id,"GRAY");
		break;
	default:
		ofLogError() << "can't set pixel format";
		return false;
	}
	return true;
}

ofPixelFormat ofxEmscriptenVideoPlayer::getPixelFormat() const{
	string format = html5video_player_pixel_format(id);
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

float ofxEmscriptenVideoPlayer::getPosition() const{
	return html5video_player_current_time(id) / html5video_player_duration(id);
}

float ofxEmscriptenVideoPlayer::getSpeed() const{
	return html5video_player_playback_rate(id);
}

float ofxEmscriptenVideoPlayer::getDuration() const{
	return html5video_player_duration(id);
}

bool ofxEmscriptenVideoPlayer::getIsMovieDone() const{
	return html5video_player_ended(id);
}


void ofxEmscriptenVideoPlayer::setPaused(bool bPause){
	if(bPause){
		html5video_player_pause(id);
	}else{
		html5video_player_play(id);
	}
}

void ofxEmscriptenVideoPlayer::setPosition(float pct){
	html5video_player_set_current_time(id,pct*html5video_player_duration(id));
}

void ofxEmscriptenVideoPlayer::setVolume(float volume){
	html5video_player_set_volume(id,volume);
}

void ofxEmscriptenVideoPlayer::setLoopState(ofLoopType state){
	if(state!=OF_LOOP_NONE){
		html5video_player_set_loop(id,1);
	}else{
		html5video_player_set_loop(id,0);
	}
}

void ofxEmscriptenVideoPlayer::setSpeed(float speed){
	html5video_player_set_playback_rate(id,speed);
}

void ofxEmscriptenVideoPlayer::setFrame(int frame){

}


int	ofxEmscriptenVideoPlayer::getCurrentFrame() const{
	return 0;
}

int	ofxEmscriptenVideoPlayer::getTotalNumFrames() const{
	return 1;
}

ofLoopType ofxEmscriptenVideoPlayer::getLoopState() const{
	return html5video_player_loop(id)?OF_LOOP_NORMAL:OF_LOOP_NONE;
}


void ofxEmscriptenVideoPlayer::firstFrame(){
	html5video_player_set_current_time(id,0);
}

void ofxEmscriptenVideoPlayer::nextFrame(){
	html5video_player_set_current_time(id,html5video_player_duration(id));
}

void ofxEmscriptenVideoPlayer::previousFrame(){

}

void ofxEmscriptenVideoPlayer::setUsePixels(bool usePixels){
	this->usePixels = usePixels;
}
