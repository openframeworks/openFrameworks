#include "ofColor.h"


#define RGBMAX	255
#define HLSMAX	255 

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

void ofColor::setHex (int hexColor, float _a){
	r = (hexColor >> 16) & 0xff;
	g = (hexColor >> 8) & 0xff;
	b = (hexColor >> 0) & 0xff;
	a = _a;
}

ofColor ofColor::clamp (){
	r = CLAMP(r, 0.0f, 255.f);
	g = CLAMP(g, 0.0f, 255.f);
	b = CLAMP(b, 0.0f, 255.f);
	a = CLAMP(a, 0.0f, 255.f);
	return *this;
}

float ofColor::getBrightness (){
	return (((r+g+b)/3.0f)/255.0f);
}

ofColor ofColor::getInverted (){
	return ofColor(255.0f-r, 255.0f-g, 255.0f-b);
}

int ofColor::getHex (){
	return ( ((int)r) << (16 + ((int)g)) << (8 + ((int)b)) );
}

int ofColor::hueMagic (int n1, int n2, int hue){
	/* range check: note values passed add/subtract thirds of range */ 
	if (hue < 0)
		hue += HLSMAX;
	if (hue > HLSMAX)
		hue -= HLSMAX;
	/* return r,g, or b value from this tridrant */ 
	if (hue < (HLSMAX/6))
		return ( n1 + (((n2-n1)*hue+(HLSMAX/12))/(HLSMAX/6)) );
	if (hue < (HLSMAX/2))
		return ( n2 );
	if (hue < ((HLSMAX*2)/3))
		return ( n1 + (((n2-n1)*(((HLSMAX*2)/3)-hue)+(HLSMAX/12))/(HLSMAX/6)));
	else
		return ( n1 );
}

ofColor ofColor::colorFromHLS (int hue, int lum, int sat){
	int rr,gg,bb;
	int magic1, magic2;
	// achromatic case
	if (sat == 0)
		rr  = gg = bb = (lum*RGBMAX)/HLSMAX;
	// chromatic
	else
	{
		/* set up magic numbers */ 
		if (lum <= (HLSMAX/2))
			magic2 = (lum*(HLSMAX + sat) + (HLSMAX/2))/HLSMAX;
		else
			magic2 = lum + sat - ((lum*sat) + (HLSMAX/2))/HLSMAX;
		magic1 = 2*lum-magic2;
		/* get RGB, change units from HLSMAX to RGBMAX */ 
		rr = (hueMagic(magic1,magic2,hue+(HLSMAX/3))*RGBMAX + (HLSMAX/2))/HLSMAX;
		gg = (hueMagic(magic1,magic2,hue)*RGBMAX + (HLSMAX/2)) / HLSMAX;
		bb = (hueMagic(magic1,magic2,hue-(HLSMAX/3))*RGBMAX + (HLSMAX/2))/HLSMAX;
	}
	return ofColor(rr, gg, bb);
}

ofColor ofColor::colorFromHue (int hue){
	return colorFromHLS(hue,128,255);
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
	return ofColor( r*(color.r/255.0f), g*(color.g/255.0f), b*(color.b/255.0f) );
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
