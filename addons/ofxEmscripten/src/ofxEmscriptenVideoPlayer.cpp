/*
 * ofEmscriptenVideoPlayer.cpp
 *
 *  Created on: May 10, 2014
 *      Author: arturo
 */

#include "ofxEmscriptenVideoPlayer.h"
#include "html5video.h"

using namespace std;

enum ReadyState{
	HAVE_NOTHING = 0,
	HAVE_METADATA,
	HAVE_CURRENT_DATA,
	HAVE_FUTURE_DATA,
	HAVE_ENOUGH_DATA
};

ofxEmscriptenVideoPlayer::ofxEmscriptenVideoPlayer()
:player_id(html5video_player_create())
,usePixels(true){

}

ofxEmscriptenVideoPlayer::~ofxEmscriptenVideoPlayer() {
	html5video_player_delete(player_id);
}

bool ofxEmscriptenVideoPlayer::load(string name){
	if (name.substr(0, 12) == "blob:http://" || name.substr(0, 13) == "blob:https://"){
		html5video_player_load(player_id, name.c_str());
	} else{
		html5video_player_load(player_id, ofToDataPath(name).c_str());
	}
	return true;
}

void ofxEmscriptenVideoPlayer::close(){
	html5video_player_delete(player_id);
	player_id = html5video_player_create();
}

void ofxEmscriptenVideoPlayer::update(){
	if(html5video_player_update(player_id, pixels.isAllocated() && usePixels, pixels.getData())){
		if(texture.texData.width!=html5video_player_width(player_id) || texture.texData.height!=html5video_player_height(player_id)){
			texture.texData.width = html5video_player_width(player_id);
			texture.texData.height = html5video_player_height(player_id);
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
		if(texture.texData.textureID!=html5video_player_texture_id(player_id)){
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
			texture.setUseExternalTextureID(html5video_player_texture_id(player_id));
		}
	}
}

void ofxEmscriptenVideoPlayer::play(){
	html5video_player_play(player_id);
}

void ofxEmscriptenVideoPlayer::stop(){
	html5video_player_stop(player_id);
}

bool ofxEmscriptenVideoPlayer::isFrameNew() const{
	// does not work with Emscripten
	if (pixels.isAllocated() || texture.isAllocated()){
		return true;
	} else{
		return false;
	}
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
	return html5video_player_is_paused(player_id);
}

bool ofxEmscriptenVideoPlayer::isLoaded() const{
	return html5video_player_ready_state(player_id) == HAVE_ENOUGH_DATA;
}

bool ofxEmscriptenVideoPlayer::isPlaying() const{
	return !isPaused();
}

bool ofxEmscriptenVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat){
	switch(pixelFormat){
	case OF_PIXELS_RGBA:
		html5video_player_set_pixel_format(player_id, "RGBA");
		break;
	case OF_PIXELS_RGB:
		html5video_player_set_pixel_format(player_id, "RGB");
		break;
	case OF_PIXELS_MONO:
		html5video_player_set_pixel_format(player_id, "GRAY");
		break;
	default:
		ofLogError() << "can't set pixel format";
		return false;
	}
	return true;
}

ofPixelFormat ofxEmscriptenVideoPlayer::getPixelFormat() const{
	string format = html5video_player_pixel_format(player_id);
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
	if (html5video_player_duration(player_id) != 0) {
	    return html5video_player_current_time(player_id) / html5video_player_duration(player_id);
	} else {
	    return 0;
	}
}

float ofxEmscriptenVideoPlayer::getSpeed() const{
	return html5video_player_playback_rate(player_id);
}

float ofxEmscriptenVideoPlayer::getDuration() const{
	return html5video_player_duration(player_id);
}

bool ofxEmscriptenVideoPlayer::getIsMovieDone() const{
	return html5video_player_ended(player_id);
}

void ofxEmscriptenVideoPlayer::setPaused(bool bPause){
	if(bPause){
		html5video_player_pause(player_id);
	}else{
		html5video_player_play(player_id);
	}
}

void ofxEmscriptenVideoPlayer::setPosition(float pct){
	html5video_player_set_current_time(player_id, pct * html5video_player_duration(player_id));
}

void ofxEmscriptenVideoPlayer::setVolume(float volume){
	html5video_player_set_volume(player_id, volume);
}

void ofxEmscriptenVideoPlayer::setLoopState(ofLoopType state){
	if(state != OF_LOOP_NONE){
		html5video_player_set_loop(player_id, 1);
	}else{
		html5video_player_set_loop(player_id, 0);
	}
}

void ofxEmscriptenVideoPlayer::setSpeed(float speed){
	html5video_player_set_playback_rate(player_id, speed);
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
	return html5video_player_loop(player_id)?OF_LOOP_NORMAL:OF_LOOP_NONE;
}

void ofxEmscriptenVideoPlayer::firstFrame(){
	html5video_player_set_current_time(player_id, 0);
}

void ofxEmscriptenVideoPlayer::nextFrame(){
	html5video_player_set_current_time(player_id, html5video_player_duration(player_id));
}

void ofxEmscriptenVideoPlayer::previousFrame(){

}

void ofxEmscriptenVideoPlayer::setUsePixels(bool usePixels){
	this->usePixels = usePixels;
}

void ofxEmscriptenVideoPlayer::setPan(float pan){
	html5video_player_set_pan(player_id, pan);
}

float ofxEmscriptenVideoPlayer::getPan() const{
	return html5video_player_pan(player_id);
}
