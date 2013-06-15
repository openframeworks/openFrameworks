#pragma once
#include <limits>
#include "ofMath.h"

//----------------------------------------------------------
// ofColor
//----------------------------------------------------------

template<typename PixelType>
class ofColor_{
	public:
	
		ofColor_<PixelType> ();
		~ofColor_<PixelType> ();
		
		ofColor_<PixelType> (float _r, float _g, float _b, float _a = limit());
		ofColor_<PixelType> (const ofColor_<PixelType> & color);
		ofColor_<PixelType> (const ofColor_<PixelType> & color, float _a);
		ofColor_<PixelType> (float gray, float _a = limit());
		
		template<typename SrcType>
		ofColor_<PixelType> (const ofColor_<SrcType> & color);

		static ofColor_<PixelType> fromHsb (float hue, float saturation, float brightness, float alpha = limit());
		static ofColor_<PixelType> fromHex (int hexColor, float alpha = limit());
		
        // these are based on CSS named colors
        // http://www.w3schools.com/cssref/css_colornames.asp
    
        static const ofColor_<PixelType> white, gray, black, red, green, blue, cyan, magenta,
        yellow,aliceBlue,antiqueWhite,aqua,aquamarine,azure,beige,bisque,blanchedAlmond,
        blueViolet,brown,burlyWood,cadetBlue,chartreuse,chocolate,coral,cornflowerBlue,cornsilk,
        crimson,darkBlue,darkCyan,darkGoldenRod,darkGray,darkGrey,darkGreen,darkKhaki,
        darkMagenta,darkOliveGreen,darkorange,darkOrchid,darkRed,darkSalmon,darkSeaGreen,
        darkSlateBlue,darkSlateGray,darkSlateGrey,darkTurquoise,darkViolet,deepPink,
        deepSkyBlue,dimGray,dimGrey,dodgerBlue,fireBrick,floralWhite,forestGreen,fuchsia,
        gainsboro,ghostWhite,gold,goldenRod,grey,greenYellow,honeyDew,hotPink,indianRed,indigo,
        ivory,khaki,lavender,lavenderBlush,lawnGreen,lemonChiffon,lightBlue,lightCoral,
        lightCyan,lightGoldenRodYellow,lightGray,lightGrey,lightGreen,lightPink,lightSalmon,
        lightSeaGreen,lightSkyBlue,lightSlateGray,lightSlateGrey,lightSteelBlue,lightYellow,
        lime,limeGreen,linen,maroon,mediumAquaMarine,mediumBlue,mediumOrchid,mediumPurple,
        mediumSeaGreen,mediumSlateBlue,mediumSpringGreen,mediumTurquoise,mediumVioletRed,
        midnightBlue,mintCream,mistyRose,moccasin,navajoWhite,navy,oldLace,olive,oliveDrab,
        orange,orangeRed,orchid,paleGoldenRod,paleGreen,paleTurquoise,paleVioletRed,papayaWhip,
        peachPuff,peru,pink,plum,powderBlue,purple,rosyBrown,royalBlue,saddleBrown,salmon,
        sandyBrown,seaGreen,seaShell,sienna,silver,skyBlue,slateBlue,slateGray,slateGrey,snow,
        springGreen,steelBlue,blueSteel,tan,teal,thistle,tomato,turquoise,violet,wheat,whiteSmoke,
        yellowGreen;
    
		void set (float _r, float _g, float _b, float _a = limit());
		void set (float _gray, float _a = limit());
		void set (ofColor_<PixelType> const & color);

		void setHex (int hexColor, float alpha = limit());
		int getHex () const;
		
		ofColor_<PixelType>& clamp ();
		ofColor_<PixelType>& invert ();
		ofColor_<PixelType>& normalize ();
		ofColor_<PixelType>& lerp(const ofColor_<PixelType>& target, float amount);
		
		ofColor_<PixelType> getClamped () const;
		ofColor_<PixelType> getInverted () const;
		ofColor_<PixelType> getNormalized () const;
		ofColor_<PixelType> getLerped(const ofColor_<PixelType>& target, float amount) const;
		
		float getHue () const;
		float getSaturation () const;
		float getBrightness () const; // brightest component
		float getLightness () const; // average of the components
		void getHsb(float& hue, float& saturation, float& brightness) const;
		
		void setHue (float hue);
		void setSaturation (float saturation); // uses hsb, not hsl. so red desaturated is white, not gray http://en.wikipedia.org/wiki/HSL_and_HSV
		void setBrightness (float brightness);
		void setHsb(float hue, float saturation, float brightness, float alpha = limit() );
		
		ofColor_<PixelType> & operator = (ofColor_<PixelType> const & color);

		template<typename SrcType>
		ofColor_<PixelType> & operator = (ofColor_<SrcType> const & color);

		ofColor_<PixelType> & operator = (float const & val);
		bool operator == (ofColor_<PixelType> const & color);
		bool operator != (ofColor_<PixelType> const & color);
		ofColor_<PixelType> operator + (ofColor_<PixelType> const & color) const;
		ofColor_<PixelType> operator + (float const & val) const;
		ofColor_<PixelType> & operator += (ofColor_<PixelType> const & color);
		ofColor_<PixelType> & operator += (float const & val);
		ofColor_<PixelType> operator - (ofColor_<PixelType> const & color) const;
		ofColor_<PixelType> operator - (float const & val) const;
		ofColor_<PixelType> & operator -= (ofColor_<PixelType> const & color);
		ofColor_<PixelType> & operator -= (float const & val);
		ofColor_<PixelType> operator * (ofColor_<PixelType> const & color) const;
		ofColor_<PixelType> operator * (float const & val) const;
		ofColor_<PixelType> & operator *= (ofColor_<PixelType> const & color);
		ofColor_<PixelType> & operator *= (float const & val);
		ofColor_<PixelType> operator / (ofColor_<PixelType> const & color) const;
		ofColor_<PixelType> operator / (float const & val) const;
		ofColor_<PixelType> & operator /= (ofColor_<PixelType> const & color);
		ofColor_<PixelType> & operator /= (float const & val);
		const PixelType & operator [] (int n) const;
		PixelType & operator [] (int n);
		
		friend ostream& operator<<(ostream& os, const ofColor_<PixelType>& color) {
			if(sizeof(PixelType) == 1) {
				os << (int) color.r << ", " << (int) color.g << ", " << (int) color.b << ", " << (int) color.a;
			} else {
				os << color.r << ", " << color.g << ", " << color.b << ", " << color.a;
			}
			return os;
		}

		friend istream& operator>>(istream& is, ofColor_<PixelType>& color) {
			if(sizeof(PixelType) == 1) {
				int component;
				is >> component;
				color.r = component;
				is.ignore(2);
				is >> component;
				color.g = component;
				is.ignore(2);
				is >> component;
				color.b = component;
				is.ignore(2);
				is >> component;
				color.a = component;
			}else{
				is >> color.r;
				is.ignore(2);
				is >> color.g;
				is.ignore(2);
				is >> color.b;
				is.ignore(2);
				is >> color.a;
			}
			return is;
		}
				
		static float limit();
		
		union  {
			struct {
				PixelType r, g, b, a;
			};
			PixelType v[4];
		};
		
	private:
		template<typename SrcType>
		void copyFrom(const ofColor_<SrcType> & mom);
};


typedef ofColor_<unsigned char> ofColor;
typedef ofColor_<float> ofFloatColor;
typedef ofColor_<unsigned short> ofShortColor;

template<typename PixelType>
template<typename SrcType>
ofColor_<PixelType>::ofColor_(const ofColor_<SrcType> & mom){
	copyFrom(mom);
}

template<typename PixelType>
template<typename SrcType>
ofColor_<PixelType> & ofColor_<PixelType>::operator=(const ofColor_<SrcType> & mom){
	copyFrom(mom);
	return *this;
}

template<typename PixelType>
template<typename SrcType>
void ofColor_<PixelType>::copyFrom(const ofColor_<SrcType> & mom){
	const float srcMax = mom.limit();
	const float dstMax = limit();
	const float factor = dstMax / srcMax;

	if(sizeof(SrcType) == sizeof(float)) {
		// coming from float we need a special case to clamp the values
		for(int i = 0; i < 4; i++){
			v[i] = ofClamp(mom[i], 0, 1) * factor;
		}
	} else{
		// everything else is a straight scaling
		for(int i = 0; i < 4; i++){
			v[i] = mom[i] * factor;
		}
	}
}

