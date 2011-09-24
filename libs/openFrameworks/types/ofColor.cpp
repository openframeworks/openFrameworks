#include "ofColor.h"
#include "ofConstants.h"

template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gray = ofColor(128, 128, 128);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::white = ofColor(255, 255, 255);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::red = ofColor(255, 0, 0);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::green = ofColor(0, 255, 0);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blue = ofColor(0, 0, 255);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cyan = ofColor(0, 255, 255);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::magenta = ofColor(255, 0, 255);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::yellow = ofColor(255, 255, 0);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::black = ofColor(0, 0, 0);

template<typename PixelType>
float ofColor_<PixelType>::limit() {
	return numeric_limits<PixelType>::max();
}

template<>
float ofColor_<float>::limit() {
	return 1.f;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (){
	r = limit();
	g = limit();
	b = limit();
	a = limit();
}

template<typename PixelType>
ofColor_<PixelType>::~ofColor_ (){}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (float _r, float _g, float _b, float _a){
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (const ofColor_<PixelType> & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (const ofColor_<PixelType> & color, float _a){
	r = color.r;
	g = color.g;
	b = color.b;
	a = _a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (float gray, float _a){
	r = g = b = gray;
	a = _a;
}

template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::fromHsb (float hue, float saturation, float brightness, float alpha) {
	ofColor_<PixelType> cur;
	cur.setHsb(hue, saturation, brightness, alpha);
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::fromHex(int hexColor, float alpha) {
	ofColor_<PixelType> cur;
	cur.setHex(hexColor, alpha);
	return cur;
}


template<typename PixelType>
void ofColor_<PixelType>::set (float _r, float _g, float _b, float _a){
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}


template<typename PixelType>
void ofColor_<PixelType>::set (float _gray, float _a){
	r = g = b = _gray;
	a = _a;
}


template<typename PixelType>
void ofColor_<PixelType>::set (ofColor_<PixelType> const & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}


template<typename PixelType>
void ofColor_<PixelType>::setHex (int hexColor, float alpha){
	r = (hexColor >> 16) & 0xff;
	g = (hexColor >> 8) & 0xff;
	b = (hexColor >> 0) & 0xff;
	a = alpha;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::clamp (){
	r = CLAMP(r, 0.0f, limit());
	g = CLAMP(g, 0.0f, limit());
	b = CLAMP(b, 0.0f, limit());
	a = CLAMP(a, 0.0f, limit());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::invert() {
	r = limit() - r;
	g = limit() - g;
	b = limit() - b;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::normalize() {
	float brightness = getBrightness();
	r = limit() * (r / brightness);
	g = limit() * (g / brightness);
	b = limit() * (b / brightness);
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::lerp(const ofColor_<PixelType>& target, float amount) {
	float invAmount = 1.f - amount;
	r = invAmount * r + amount * target.r;
	g = invAmount * g + amount * target.g;
	b = invAmount * b + amount * target.b;
	a = invAmount * a + amount * target.a;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getClamped () const {
	ofColor_<PixelType> cur(*this);
	cur.clamp();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getInverted () const {
	ofColor_<PixelType> cur(*this);
	cur.invert();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getNormalized () const {
	ofColor_<PixelType> cur(*this);
	cur.normalize();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getLerped(const ofColor_<PixelType>& target, float amount) const {
	ofColor_<PixelType> cur(*this);
	cur.lerp(target, amount);
	return cur;
}



template<typename PixelType>
float ofColor_<PixelType>::getHue() const {
	float hue, saturation, brightness;
	getHsb(hue, saturation, brightness);
	return hue;
}


template<typename PixelType>
float ofColor_<PixelType>::getSaturation() const {
	float hue, saturation, brightness;
	getHsb(hue, saturation, brightness);
	return saturation;
}

/*
	Brightness is simply the maximum of the three color components.
	This is used by Photoshop (HSB) and Processing (HSB).
	Brightness is also called "value".
*/

template<typename PixelType>
float ofColor_<PixelType>::getBrightness() const {
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

template<typename PixelType>
float ofColor_<PixelType>::getLightness() const {
	return (r + g + b) / 3.f;
}



template<typename PixelType>
void ofColor_<PixelType>::getHsb(float& hue, float& saturation, float& brightness) const {
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
		brightness = limit() * max;
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
	hue = limit() * hueSixth / 6.f;
	saturation = limit() * (max - min) / max;
	brightness = max;
}


template<typename PixelType>
void ofColor_<PixelType>::setHue (float hue) {
	float oldHue, saturation, brightness;
	getHsb(oldHue, saturation, brightness);
	setHsb(hue, saturation, brightness);
}


template<typename PixelType>
void ofColor_<PixelType>::setSaturation (float saturation) {
	float hue, oldSaturation, brightness;
	getHsb(hue, oldSaturation, brightness);
	setHsb(hue, saturation, brightness);
}


template<typename PixelType>
void ofColor_<PixelType>::setBrightness (float brightness) {
	float hue, saturation, oldBrightness;
	getHsb(hue, saturation, oldBrightness);
	setHsb(hue, saturation, brightness);
}


template<typename PixelType>
void ofColor_<PixelType>::setHsb(float hue, float saturation, float brightness, float alpha) {
	a = alpha;
	setHsb(hue, saturation, brightness);
}

/*
	setHsb() breaks the "always override alpha" model because it's needed by setHue(),
	setSaturation(), and setBrightness() -- which shouldn't modify the alpha.
*/

template<typename PixelType>
void ofColor_<PixelType>::setHsb(float hue, float saturation, float brightness) {
	if(brightness == 0) { // black
		set(0);
	} else if(saturation == 0) { // grays
		set(brightness);
	} else {
		float hueSix = hue * 6. / limit();
		float saturationNorm = saturation / limit();
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


template<typename PixelType>
int ofColor_<PixelType>::getHex () const {
	return
		((0xff & (unsigned char) r) << 16) |
		((0xff & (unsigned char) g) << 8) |
		((0xff & (unsigned char) b));
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator = (ofColor_<PixelType> const & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator = (float const & val){
	r = val;
	g = val;
	b = val;
	a = limit();
	return *this;
}


template<typename PixelType>
bool ofColor_<PixelType>::operator == (ofColor_<PixelType> const & color){
	return (r == color.r) && (g == color.g) && (b == color.b);
}


template<typename PixelType>
bool ofColor_<PixelType>::operator != (ofColor_<PixelType> const & color){
	return (r != color.r) || (g != color.g) || (b != color.b);
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator + (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( r+color.r, g+color.g, b+color.b ).clamp();
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator + (float const & val) const{
	return ofColor_<PixelType>( r+val, g+val, b+val ).clamp();
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator += (ofColor_<PixelType> const & color){
	r += color.r;
	g += color.g;
	b += color.b;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator += (float const & val){
	r += val;
	g += val;
	b += val;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator - (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( r-color.r, g-color.g, b-color.b ).clamp();
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator - (float const & val) const{
	return ofColor_<PixelType>( r-val, g-val, b-val).clamp();
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator -= (ofColor_<PixelType> const & color){
	r -= color.r;
	g -= color.g;
	b -= color.b;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator -= (float const & val){
	r -= val;
	g -= val;
	b -= val;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator * (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( r*(color.r/limit()), g*(color.g/limit()), b*(color.b/limit()));
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator * (float const & val) const{
	float v = CLAMP(val,0.0,1.0);
	return ofColor_<PixelType>( r*v, g*v, b*v).clamp();
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator *= (ofColor_<PixelType> const & color){
	r *= (color.r/limit());
	g *= (color.g/limit());
	b *= (color.b/limit());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator *= (float const & val){
	float v = CLAMP(val,0.0,1.0);
	r *= v;
	g *= v;
	b *= v;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator / (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( color.r!=0.0f ? r/(color.r/limit()) : r , color.g!=0.0f ? g/(color.g/limit()) : g, color.b!=0.0f ? b/(color.b/limit()) : b );
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator / (float const & val) const{
	if( val != 0.0f)
		return ofColor_<PixelType>( r/val, g/val, b/val ).clamp();
	else
		return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator /= (ofColor_<PixelType> const & color){
	if (color.r!=0.0f) r /= (color.r/limit());
	if (color.g!=0.0f) g /= (color.g/limit());
	if (color.b!=0.0f) b /= (color.b/limit());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator /= (float const & val){
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


template<typename PixelType>
const PixelType & ofColor_<PixelType>::operator [] (int n) const{
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

template<typename PixelType>
PixelType & ofColor_<PixelType>::operator [] (int n){
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

template class ofColor_<unsigned char>;
template class ofColor_<float>;
template class ofColor_<unsigned short>;
