#include "ofColor.h"
#include "ofConstants.h"

const ofColor ofColor::gray = ofColor( 128,128,128 );
const ofColor ofColor::white = ofColor( 255,255,255 );
const ofColor ofColor::red = ofColor( 255,0,0 );
const ofColor ofColor::green = ofColor( 0,255,0 );
const ofColor ofColor::blue = ofColor( 0,0,255 );
const ofColor ofColor::cyan = ofColor( 0,255,255 );
const ofColor ofColor::magenta = ofColor( 255,0,255 );
const ofColor ofColor::yellow = ofColor( 255,255,0 );
const ofColor ofColor::black = ofColor( 0,0,0 );

ofColor::ofColor (){
	r = 255.0f;
	g = 255.0f;
	b = 255.0f;
	a = 255.0f;
}

ofColor::~ ofColor (){}

ofColor::ofColor (float _r, float _g, float _b, float _a){
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

ofColor::ofColor (ofColor const & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

ofColor::ofColor (ofColor const & color, float _a){
	r = color.r;
	g = color.g;
	b = color.b;
	a = _a;
}

ofColor::ofColor (float gray, float _a){
	r = g = b = gray;
	a = _a;
}

ofColor ofColor::fromHsb (float hue, float saturation, float brightness, float alpha) {
	ofColor cur;
	cur.setHsb(hue, saturation, brightness, alpha);
	return cur;
}

ofColor ofColor::fromHex(int hexColor, float alpha) {
	ofColor cur;
	cur.setHex(hexColor, alpha);
	return cur;
}

void ofColor::set (float _r, float _g, float _b, float _a){
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

void ofColor::set (float _gray, float _a){
	r = g = b = _gray;
	a = _a;
}

void ofColor::set (ofColor const & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

void ofColor::setHex (int hexColor, float alpha){
	r = (hexColor >> 16) & 0xff;
	g = (hexColor >> 8) & 0xff;
	b = (hexColor >> 0) & 0xff;
	a = alpha;
}

ofColor& ofColor::clamp (){
	r = CLAMP(r, 0.0f, 255.f);
	g = CLAMP(g, 0.0f, 255.f);
	b = CLAMP(b, 0.0f, 255.f);
	a = CLAMP(a, 0.0f, 255.f);
	return *this;
}

ofColor& ofColor::invert() {
	r = 255.f - r;
	g = 255.f - g;
	b = 255.f - b;
	return *this;
}

ofColor& ofColor::normalize() {
	float brightness = getBrightness();
	r = 255.f * (r / brightness);
	g = 255.f * (g / brightness);
	b = 255.f * (b / brightness);
	return *this;
}

ofColor& ofColor::lerp(const ofColor& target, float amount) {
	float invAmount = 1.f - amount;
	r = invAmount * r + amount * target.r;
	g = invAmount * g + amount * target.g;
	b = invAmount * b + amount * target.b;
	a = invAmount * a + amount * target.a;
	return *this;
}

ofColor ofColor::getClamped () const {
	ofColor cur(*this);
	cur.clamp();
	return cur;
}

ofColor ofColor::getInverted () const {
	ofColor cur(*this);
	cur.invert();
	return cur;
}

ofColor ofColor::getNormalized () const {
	ofColor cur(*this);
	cur.normalize();
	return cur;
}

ofColor ofColor::getLerped(const ofColor& target, float amount) const {
	ofColor cur(*this);
	cur.lerp(target, amount);
	return cur;
}


float ofColor::getHue() const {
	float hue, saturation, brightness;
	getHsb(hue, saturation, brightness);
	return hue;
}

float ofColor::getSaturation() const {
	float hue, saturation, brightness;
	getHsb(hue, saturation, brightness);
	return saturation;
}

/*
	Brightness is simply the maximum of the three color components.
	This is used by Photoshop (HSB) and Processing (HSB).
	Brightness is also called "value".
*/
float ofColor::getBrightness() const {
	float max = r;
	if(g > max) {
		max = g;
	}
	if(b > max) {
		max = b;
	}
	return max;
}

/*
	Lightness is the average of the three color components.
	This is used by the Lab and HSL color spaces.
*/
float ofColor::getLightness() const {
	return (r + g + b) / 3.f;
}


void ofColor::getHsb(float& hue, float& saturation, float& brightness) const {	
	float max = getBrightness();
	
	float min = r;
	if(g < min) {
		min = g;
	}
	if(b < min) {
		min = b;
	}
	
	if(max == min) { // grays
		hue = 0.f;
		saturation = 0.f;
		brightness = 255.f * max;
		return;
	}
	
	float hueSixth;
	if(r == max) {
		hueSixth = (g - b) / (max - min);
		if(hueSixth < 0.f)
			hueSixth += 6.f;
	} else if (g == max) {
		hueSixth = 2.f + (b - r) / (max - min);
	} else {
		hueSixth = 4.f + (r - g) / (max - min);
	}
	hue = 255.f * hueSixth / 6.f;
	saturation = 255.f * (max - min) / max;
	brightness = max;
}

void ofColor::setHue (float hue) {
	float oldHue, saturation, brightness;
	getHsb(oldHue, saturation, brightness);
	setHsb(hue, saturation, brightness);
}

void ofColor::setSaturation (float saturation) {
	float hue, oldSaturation, brightness;
	getHsb(hue, oldSaturation, brightness);
	setHsb(hue, saturation, brightness);
}

void ofColor::setBrightness (float brightness) {
	float hue, saturation, oldBrightness;
	getHsb(hue, saturation, oldBrightness);
	setHsb(hue, saturation, brightness);
}

void ofColor::setHsb(float hue, float saturation, float brightness, float alpha) {
	a = alpha;
	setHsb(hue, saturation, brightness);
}

/*
	setHsb() breaks the "always override alpha" model because it's needed by setHue(),
	setSaturation(), and setBrightness() -- which shouldn't modify the alpha.
*/
void ofColor::setHsb(float hue, float saturation, float brightness) {
	if(brightness == 0) { // black
		set(0);
	} else if(saturation == 0) { // grays
		set(brightness);
	} else {
		float hueSix = hue * 6. / 255.;
		float saturationNorm = saturation / 255.;
		int hueSixCategory = (int) floorf(hueSix);
		float hueSixRemainder = hueSix - hueSixCategory;
		unsigned char pv = (unsigned char) ((1.f - saturationNorm) * brightness);
		unsigned char qv = (unsigned char) ((1.f - saturationNorm * hueSixRemainder) * brightness);
		unsigned char tv = (unsigned char) ((1.f - saturationNorm * (1.f - hueSixRemainder)) * brightness);
		switch(hueSixCategory) {
			case 0: // r
				r = brightness;
				g = tv;
				b = pv;
				break;
			case 1: // g
				r = qv;
				g = brightness;
				b = pv;
				break;
			case 2:
				r = pv;
				g = brightness;
				b = tv;
				break;
			case 3: // b
				r = pv;
				g = qv;
				b = brightness;
				break;
			case 4:
				r = tv;
				g = pv;
				b = brightness;
				break;
			case 5: // back to r
				r = brightness;
				g = pv;
				b = qv;
				break;
		}
	}
}

int ofColor::getHex () const {
	return
		((0xff & (unsigned char) r) << 16) |
		((0xff & (unsigned char) g) << 8) |
		((0xff & (unsigned char) b));
}

ofColor & ofColor::operator = (ofColor const & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
	return *this;
}

ofColor & ofColor::operator = (float const & val){
	r = val;
	g = val;
	b = val;
	a = 255.0f;
	return *this;
}

bool ofColor::operator == (ofColor const & color){
	return (r == color.r) && (g == color.g) && (b == color.b);
}

bool ofColor::operator != (ofColor const & color){
	return (r != color.r) || (g != color.g) || (b != color.b);
}

ofColor ofColor::operator + (ofColor const & color) const{
	return ofColor( r+color.r, g+color.g, b+color.b ).clamp();
}

ofColor ofColor::operator + (float const & val) const{
	return ofColor( r+val, g+val, b+val ).clamp();
}

ofColor & ofColor::operator += (ofColor const & color){
	r += color.r;
	g += color.g;
	b += color.b;
	this->clamp();
	return *this;
}

ofColor & ofColor::operator += (float const & val){
	r += val;
	g += val;
	b += val;
	this->clamp();
	return *this;
}

ofColor ofColor::operator - (ofColor const & color) const{
	return ofColor( r-color.r, g-color.g, b-color.b ).clamp();
}

ofColor ofColor::operator - (float const & val) const{
	return ofColor( r-val, g-val, b-val).clamp();
}

ofColor & ofColor::operator -= (ofColor const & color){
	r -= color.r;
	g -= color.g;
	b -= color.b;
	this->clamp();
	return *this;
}

ofColor & ofColor::operator -= (float const & val){
	r -= val;
	g -= val;
	b -= val;
	this->clamp();
	return *this;
}

ofColor ofColor::operator * (ofColor const & color) const{
	return ofColor( r*(color.r/255.0f), g*(color.g/255.0f), b*(color.b/255.0f));
}

ofColor ofColor::operator * (float const & val) const{
	float v = CLAMP(val,0.0,1.0);
	return ofColor( r*v, g*v, b*v).clamp();
}

ofColor & ofColor::operator *= (ofColor const & color){
	r *= (color.r/255.0f);
	g *= (color.g/255.0f);
	b *= (color.b/255.0f);
	return *this;
}

ofColor & ofColor::operator *= (float const & val){
	float v = CLAMP(val,0.0,1.0);
	r *= v;
	g *= v;
	b *= v;
	return *this;
}

ofColor ofColor::operator / (ofColor const & color) const{
	return ofColor( color.r!=0.0f ? r/(color.r/255.0f) : r , color.g!=0.0f ? g/(color.g/255.0f) : g, color.b!=0.0f ? b/(color.b/255.0f) : b );
}

ofColor ofColor::operator / (float const & val) const{
	if( val != 0.0f)
		return ofColor( r/val, g/val, b/val ).clamp();
	else
		return *this;
}

ofColor & ofColor::operator /= (ofColor const & color){
	if (color.r!=0.0f) r /= (color.r/255.0f);
	if (color.g!=0.0f) g /= (color.g/255.0f);
	if (color.b!=0.0f) b /= (color.b/255.0f);
	return *this;
}

ofColor & ofColor::operator /= (float const & val){
	if( val != 0.0f ){
		r /= val;
		g /= val;
		b /= val;
		clamp();
		return *this;
	}
	else
		return *this;
}

float & ofColor::operator [] (int n){
	switch( n ){
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3: 
			return a;
		default:
			return r;
			break;
	}
}
