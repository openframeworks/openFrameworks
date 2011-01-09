


#pragma once
#include "ofConstants.h"

//----------------------------------------------------------
// ofColor
//----------------------------------------------------------

class ofColor{
public:
	ofColor(){
		r = 255.0f;
		g = 255.0f;
		b = 255.0f;
		a = 255.0f;
	}
	virtual ~ofColor(){}
	
    ofColor( float _r, float _g, float _b , float _a=255.0f ) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
	
	ofColor( const ofColor & color){
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }
	
	ofColor( const ofColor & color, float _a){
        r = color.r;
        g = color.g;
        b = color.b;
        a = _a;
    }
	
	ofColor( float gray, float _a=255.0f){
		r = g = b = gray;
        a = _a;
    }
	
	void set(float _r, float _g, float _b, float _a = 255.0f){
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
	
    void set(float _gray, float _a = 255.0f){
        r = g = b = _gray;
        a = _a;
    }
	
    void set(const ofColor & color){
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }
	
    void setHex(int hexColor, float _a=255.0f){
		r = (hexColor >> 16) & 0xff;
		g = (hexColor >> 8) & 0xff;
		b = (hexColor >> 0) & 0xff;
        a = _a;
    }
	
	// clamp channels between 0.0 and 255.0
    ofColor clamp(){
		r = CLAMP(r, 0.0f, 255.f);
		g = CLAMP(g, 0.0f, 255.f);
		b = CLAMP(b, 0.0f, 255.f);
		a = CLAMP(a, 0.0f, 255.f);
		return *this;
    }
	
	// Return average brightnes (0.0 - 1.0), ignoring alpha
    float getBrightness(){
		return (((r+g+b)/3.0f)/255.0f);
    }
	
	// Return average brightnes (0.0 - 1.0), ignoring alpha
    ofColor getInverted(){
		return ofColor(255.0f-r, 255.0f-g, 255.0f-b);
    }
	
	// Return hex color
	// ROGER: TESTAR ISSO AQUI!!!!
    int getHex(){
		return ( ((int)r) << (16 + ((int)g)) << (8 + ((int)b)) );
    }
	
	
	// make color from hue / brightness / contrast
	// from http://support.microsoft.com/kb/29240
	// this is static so you can go like...
	//	ofColor c = ofColor::colorFromHue(255);
#define RGBMAX	255
#define HLSMAX	255
	// this magic is actually private
	static int hueMagic(int n1, int n2, int hue)
	{
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
	// make color from hue / brightness / saturation 0..225
	static ofColor colorFromHLS(int hue, int lum, int sat){
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
	// make color from hue only 0..225
	static ofColor colorFromHue(int hue){
		return colorFromHLS(hue,128,255);
	}
	
	
	
	//------ Operators:
	
	//Set
	ofColor & operator=( const ofColor & color ){
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
		return *this;
	}
	
	// set grayscale
	ofColor & operator=( const float& val ){
		r = val;
		g = val;
		b = val;
		a = 255.0f;
		return *this;
	}
	
    //equality
    bool operator==( const ofColor & color ) {
        return (r == color.r) && (g == color.g) && (b == color.b);
    }
	
	//inequality
    bool operator!=( const ofColor & color ) {
        return (r != color.r) || (g != color.g) || (b != color.b);
    }
	
	// Add
	// always clamp after to keep channels between 0.0 and 255.0
    ofColor operator+( const ofColor & color ) const {
        return ofColor( r+color.r, g+color.g, b+color.b ).clamp();
    }
	
    ofColor operator+( const float& val ) const {
        return ofColor( r+val, g+val, b+val ).clamp();
    }
	
	ofColor & operator+=( const ofColor & color ) {
        r += color.r;
        g += color.g;
        b += color.b;
		this->clamp();
        return *this;
    }
	
	ofColor & operator+=( const float & val ) {
        r += val;
        g += val;
        b += val;
		this->clamp();
        return *this;
    }
	
	// Subtract
	// always clamp after to keep channels between 0.0 and 255.0
    ofColor operator-(const ofColor & color) const {
        return ofColor( r-color.r, g-color.g, b-color.b ).clamp();
    }
	
    ofColor operator-(const float& val) const {
        return ofColor( r-val, g-val, b-val).clamp();
    }
	
    ofColor & operator-=( const ofColor & color ) {
        r -= color.r;
        g -= color.g;
        b -= color.b;
		this->clamp();
        return *this;
    }
	
    ofColor & operator-=( const float & val ) {
        r -= val;
        g -= val;
        b -= val;
		this->clamp();
        return *this;
    }
	
	//
	// Multiply
	// theory: http://www.digitalartform.com/imageArithmetic.htm
	// color multiplication is only useful when colors are represented as floats 0.0 to 1.0, not as bytes 0 to 255
	// in fact we are talking about percentages here
	// multiplying a color by 0.2 will give us 20% of that color
	// multiplying a color by 0.0 will make it black (0% of it)
	// multiplying a color by 1.0 will return the original color (100% of it)
	// multiplying a color by 2.0 makes no sense (there is no 200%), so we clamp the value to 1.0 (100%)
	// the same apply to each channel, so we must think about each channel separately
	// multiply RED by GREEN will give us black, because the red channel of GREEN will cancel RED, and the green channel of RED will cancel GREEN
    ofColor operator*( const ofColor & color ) const {
        return ofColor( r*(color.r/255.0f), g*(color.g/255.0f), b*(color.b/255.0f) );
    }
	
	// we clamp the value when multiplying by float (faster than clamping each channel)
    ofColor operator*(const float& val) const {
		float v = CLAMP(val,0.0,1.0);
        return ofColor( r*v, g*v, b*v).clamp();
    }
	
    ofColor & operator*=( const ofColor & color ) {
        r *= (color.r/255.0f);
        g *= (color.g/255.0f);
        b *= (color.b/255.0f);
        return *this;
    }
	
    ofColor & operator*=( const float & val ) {
		float v = CLAMP(val,0.0,1.0);
        r *= v;
        g *= v;
        b *= v;
        return *this;
    }
	
	// Divide
	// Same concept of multiplication
	// Dividing anything by WHITE will return the original color
	// Dividing anything by BLACK is impossible (division by zero = core dump!), so will return the original color
	// The same apply for each channel
    ofColor operator/( const ofColor & color ) const {
        return ofColor( color.r!=0.0f ? r/(color.r/255.0f) : r , color.g!=0.0f ? g/(color.g/255.0f) : g, color.b!=0.0f ? b/(color.b/255.0f) : b );
    }
	
	// we clamp the color when divided by float
    ofColor operator/( const float &val ) const {
		if( val != 0.0f)
			return ofColor( r/val, g/val, b/val ).clamp();
		else
			return *this;
    }
	
    ofColor& operator/=( const ofColor & color ) {
        if (color.r!=0.0f) r /= (color.r/255.0f);
        if (color.g!=0.0f) g /= (color.g/255.0f);
        if (color.b!=0.0f) b /= (color.b/255.0f);
        return *this;
    }
	
    ofColor& operator/=( const float &val ) {
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
	
	float & operator[]( int n ){
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
	
	// union allows us to access the coordinates through
    // both an array 'v' and 'r', 'g', 'b', 'a' member varibles
    union  {
        struct {
			float r, g, b, a;
        };
        float v[4];
    };
};