#include "ofColor.h"
#include "ofConstants.h"


template<typename A, typename B>
A clampedSubtract(const A& a, const B& b) {
	return CLAMP((float) a - (float) b, 0, ofColor_<A>::plimit());
}
template<typename A, typename B>
A clampedAdd(const A& a, const B& b) {
	return CLAMP((float) a + (float) b, 0, ofColor_<A>::plimit());
}
template<typename A, typename B>
A clampedDivide(const A& a, const B& b) {
	return CLAMP((float) a / (float) b, 0, ofColor_<A>::plimit());
}
template<typename A, typename B>
A clampedMultiply(const A& a, const B& b) {
	return CLAMP((float) a * (float) b, 0, ofColor_<A>::plimit());
}

template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::fromHsb(float hue,
												 float saturation,
												 float brightness,
												 PixelType alpha){
	ofColor_<PixelType> cur;
	cur.setHsb(hue, saturation, brightness, alpha);
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::fromHex(int hexColor, PixelType alpha){
	ofColor_<PixelType> cur;
	cur.setHex(hexColor, alpha);
	return cur;
}


template<typename PixelType>
void ofColor_<PixelType>::set(PixelType _r, PixelType _g, PixelType _b, PixelType _a){
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}


template<typename PixelType>
void ofColor_<PixelType>::set(PixelType _gray, PixelType _a){
	r = _gray;
    g = _gray;
    b = _gray;
	a = _a;
}


template<typename PixelType>
void ofColor_<PixelType>::set(const ofColor_<PixelType>& color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::clamp(){
	r = CLAMP(r, 0.0f, plimit());
	g = CLAMP(g, 0.0f, plimit());
	b = CLAMP(b, 0.0f, plimit());
	a = CLAMP(a, 0.0f, plimit());
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
	float scaleFactor = getBrightness() / limit();
	*this /= scaleFactor;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::lerp(const ofColor_<PixelType>& target,
                                               float amount) {
	float invAmount = 1.f - amount;
	r = invAmount * r + amount * target.r;
	g = invAmount * g + amount * target.g;
	b = invAmount * b + amount * target.b;
	a = invAmount * a + amount * target.a;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getClamped() const {
	ofColor_<PixelType> cur(*this);
	cur.clamp();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getInverted() const {
	ofColor_<PixelType> cur(*this);
	cur.invert();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getNormalized() const {
	ofColor_<PixelType> cur(*this);
	cur.normalize();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getLerped(const ofColor_<PixelType>& target,
                                                   float amount) const
{
	ofColor_<PixelType> cur(*this);
	cur.lerp(target, amount);
	return cur;
}

template<typename PixelType>
float ofColor_<PixelType>::getHue() const {
	float hue = 0;
    float saturation = 0;
    float brightness = 0;
	getHsb(hue, saturation, brightness);
	return hue;
}

template<typename PixelType>
float ofColor_<PixelType>::getHueAngle() const {
	return getHue() * 360. / limit();
}

template<typename PixelType>
float ofColor_<PixelType>::getSaturation() const {
	float hue = 0;
    float saturation = 0;
    float brightness = 0;
	getHsb(hue, saturation, brightness);
	return saturation;
}

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

template<typename PixelType>
float ofColor_<PixelType>::getLightness() const {
	return (r + g + b) / 3.f;
}

template<typename PixelType>
void ofColor_<PixelType>::getHsb(float& hue,
                                 float& saturation,
                                 float& brightness) const {
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
		brightness = max;
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
void ofColor_<PixelType>::setHue(float hue) {
	float oldHue = 0;
    float saturation = 0;
    float brightness = 0;
	getHsb(oldHue, saturation, brightness);
	setHsb(hue, saturation, brightness, a);
}

template<typename PixelType>
void ofColor_<PixelType>::setHueAngle(float angle) {
	setHue(angle / 360. * limit());
}

template<typename PixelType>
void ofColor_<PixelType>::setSaturation(float saturation) {
	float hue, oldSaturation, brightness;
	getHsb(hue, oldSaturation, brightness);
	setHsb(hue, saturation, brightness, a );
}


template<typename PixelType>
void ofColor_<PixelType>::setBrightness(float brightness) {
	float hue, saturation, oldBrightness;
	getHsb(hue, saturation, oldBrightness);
	setHsb(hue, saturation, brightness, a );
}


template<typename PixelType>
void ofColor_<PixelType>::setHsb(float hue, float saturation, float brightness, PixelType alpha) {
	saturation = ofClamp(saturation, 0.f, limit());
	brightness = ofClamp(brightness, 0.f, limit());
	if(brightness == 0) { // black
		set(0);
	} else if(saturation == 0) { // grays
		set(brightness);
	} else {
		float hueSix = hue * 6.f / limit();
		float saturationNorm = saturation / limit();
		int hueSixCategory = (int) floorf(hueSix);
		float hueSixRemainder = hueSix - hueSixCategory;
		PixelType pv = (PixelType) ((1.f - saturationNorm) * brightness);
		PixelType qv = (PixelType) ((1.f - saturationNorm * hueSixRemainder) * brightness);
		PixelType tv = (PixelType) ((1.f - saturationNorm * (1.f - hueSixRemainder)) * brightness);
		switch(hueSixCategory) {
			case 0: case 6: // r
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
    
    // finally assign the alpha
    a = alpha;
}

template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator = (PixelType val){
	r = val;
	g = val;
	b = val;
	a = plimit();
	return *this;
}


template<typename PixelType>
bool ofColor_<PixelType>::operator == (const ofColor_<PixelType>& color) const {
	return (r == color.r) && (g == color.g) && (b == color.b) && (a == color.a);
}


template<typename PixelType>
bool ofColor_<PixelType>::operator != (const ofColor_<PixelType>& color) const {
	return (r != color.r) || (g != color.g) || (b != color.b) || (a != color.a);
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator + (const ofColor_<PixelType>& color) const{
	ofColor_<PixelType> result(*this);
	result += color;
	return result;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator + (float val) const{
	ofColor_<PixelType> result(*this);
	result += val;
	return result;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator += (const ofColor_<PixelType>& color){
	r = clampedAdd(r, color.r);
	g = clampedAdd(g, color.g);
	b = clampedAdd(b, color.b);
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator += (float val){
	r = clampedAdd(r, val);
	g = clampedAdd(g, val);
	b = clampedAdd(b, val);
	return *this;
}

template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator - (const ofColor_<PixelType>& color) const{
	ofColor_<PixelType> result(*this);
	result -= color;
	return result;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator - (float val) const{
	ofColor_<PixelType> result(*this);
	result -= val;
	return result;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator -= (const ofColor_<PixelType>& color){
	r = clampedSubtract(r, color.r);
	g = clampedSubtract(g, color.g);
	b = clampedSubtract(b, color.b);
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator -= (float val){
	r = clampedSubtract(r, val);
	g = clampedSubtract(g, val);
	b = clampedSubtract(b, val);
	return *this;
}

template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator * (const ofColor_<PixelType>& color) const{
	ofColor_<PixelType> result(*this);
	result *= color;
	return result;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator * (float val) const{
	ofColor_<PixelType> result(*this);
	result *= val;
	return result;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator *= (const ofColor_<PixelType>& color){
	r = clampedMultiply(r, (float) color.r / limit());
	g = clampedMultiply(g, (float) color.g / limit());
	b = clampedMultiply(b, (float) color.b / limit());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator *= (float val){
	r = clampedMultiply(r, val);
	g = clampedMultiply(g, val);
	b = clampedMultiply(b, val);
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator / (const ofColor_<PixelType>& color) const{
	ofColor_<PixelType> result(*this);
	result /= color;
	return result;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator / (float val) const{
	ofColor_<PixelType> result(*this);
	result /= val;
	return result;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator /= (const ofColor_<PixelType>& color){
	r = clampedDivide(r, (float) color.r / limit());
	g = clampedDivide(g, (float) color.g / limit());
	b = clampedDivide(b, (float) color.b / limit());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator /= (float val){
	r = clampedDivide(r, val);
	g = clampedDivide(g, val);
	b = clampedDivide(b, val);
	return *this;
}


template<typename PixelType>
const PixelType & ofColor_<PixelType>::operator [] (std::size_t n) const{
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
PixelType & ofColor_<PixelType>::operator [] (std::size_t n){
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

template class ofColor_<char>;
template class ofColor_<unsigned char>;
template class ofColor_<short>;
template class ofColor_<unsigned short>;
template class ofColor_<int>;
template class ofColor_<unsigned int>;
template class ofColor_<long>;
template class ofColor_<unsigned long>;
template class ofColor_<float>;
template class ofColor_<double>;
