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
		
        static const ofColor_<PixelType> white, gray, black, red, green, blue, cyan, magenta, yellow, aliceblue,antiquewhite,antiquewhite1,antiquewhite2,antiquewhite3,antiquewhite4,aquamarine,aquamarine1,aquamarine2,aquamarine3,aquamarine4,azure,azure1,azure2,azure3,azure4,beige,bisque,bisque1,bisque2,bisque3,bisque4,blanchedalmond,blue1,blue2,blue3,blue4,blueviolet,brown,brown1,brown2,brown3,brown4,burlywood,burlywood1,burlywood2,burlywood3,burlywood4,cadetblue,cadetblue1,cadetblue2,cadetblue3,cadetblue4,chartreuse,chartreuse1,chartreuse2,chartreuse3,chartreuse4,chocolate,chocolate1,chocolate2,chocolate3,chocolate4,coral,coral1,coral2,coral3,coral4,cornflowerblue,cornsilk,cornsilk1,cornsilk2,cornsilk3,cornsilk4,cyan1,cyan2,cyan3,cyan4,darkblue,darkcyan,darkgoldenrod,darkgoldenrod1,darkgoldenrod2,darkgoldenrod3,darkgoldenrod4,darkgray,darkgreen,darkgrey,darkkhaki,darkmagenta,darkolivegreen,darkolivegreen1,darkolivegreen2,darkolivegreen3,darkolivegreen4,darkorange,darkorange1,darkorange2,darkorange3,darkorange4,darkorchid,darkorchid1,darkorchid2,darkorchid3,darkorchid4,darkred,darksalmon,darkseagreen,darkseagreen1,darkseagreen2,darkseagreen3,darkseagreen4,darkslateblue,darkslategray,darkslategray1,darkslategray2,darkslategray3,darkslategray4,darkslategrey,darkturquoise,darkviolet,deeppink,deeppink1,deeppink2,deeppink3,deeppink4,deepskyblue,deepskyblue1,deepskyblue2,deepskyblue3,deepskyblue4,dimgray,dimgrey,dodgerblue,dodgerblue1,dodgerblue2,dodgerblue3,dodgerblue4,firebrick,firebrick1,firebrick2,firebrick3,firebrick4,floralwhite,forestgreen,gainsboro,ghostwhite,gold,gold1,gold2,gold3,gold4,goldenrod,goldenrod1,goldenrod2,goldenrod3,goldenrod4,green1,green2,green3,green4,greenyellow,grey,honeydew,honeydew1,honeydew2,honeydew3,honeydew4,hotpink,hotpink1,hotpink2,hotpink3,hotpink4,indianred,indianred1,indianred2,indianred3,indianred4,ivory,ivory1,ivory2,ivory3,ivory4,khaki,khaki1,khaki2,khaki3,khaki4,lavender,lavenderblush,lavenderblush1,lavenderblush2,lavenderblush3,lavenderblush4,lawngreen,lemonchiffon,lemonchiffon1,lemonchiffon2,lemonchiffon3,lemonchiffon4,lightblue,lightblue1,lightblue2,lightblue3,lightblue4,lightcoral,lightcyan,lightcyan1,lightcyan2,lightcyan3,lightcyan4,lightgoldenrod,lightgoldenrod1,lightgoldenrod2,lightgoldenrod3,lightgoldenrod4,lightgoldenrodyellow,lightgray,lightgreen,lightgrey,lightpink,lightpink1,lightpink2,lightpink3,lightpink4,lightsalmon,lightsalmon1,lightsalmon2,lightsalmon3,lightsalmon4,lightseagreen,lightskyblue,lightskyblue1,lightskyblue2,lightskyblue3,lightskyblue4,lightslateblue,lightslategray,lightslategrey,lightsteelblue,lightsteelblue1,lightsteelblue2,lightsteelblue3,lightsteelblue4,lightyellow,lightyellow1,lightyellow2,lightyellow3,lightyellow4,limegreen,linen,magenta1,magenta2,magenta3,magenta4,maroon,maroon1,maroon2,maroon3,maroon4,mediumaquamarine,mediumblue,mediumorchid,mediumorchid1,mediumorchid2,mediumorchid3,mediumorchid4,mediumpurple,mediumpurple1,mediumpurple2,mediumpurple3,mediumpurple4,mediumseagreen,mediumslateblue,mediumspringgreen,mediumturquoise,mediumvioletred,midnightblue,mintcream,mistyrose,mistyrose1,mistyrose2,mistyrose3,mistyrose4,moccasin,navajowhite,navajowhite1,navajowhite2,navajowhite3,navajowhite4,navy,navyblue,oldlace,olivedrab,olivedrab1,olivedrab2,olivedrab3,olivedrab4,orange,orange1,orange2,orange3,orange4,orangered,orangered1,orangered2,orangered3,orangered4,orchid,orchid1,orchid2,orchid3,orchid4,palegoldenrod,palegreen,palegreen1,palegreen2,palegreen3,palegreen4,paleturquoise,paleturquoise1,paleturquoise2,paleturquoise3,paleturquoise4,palevioletred,palevioletred1,palevioletred2,palevioletred3,palevioletred4,papayawhip,peachpuff,peachpuff1,peachpuff2,peachpuff3,peachpuff4,peru,pink,pink1,pink2,pink3,pink4,plum,plum1,plum2,plum3,plum4,powderblue,purple,purple1,purple2,purple3,purple4,red1,red2,red3,red4,rosybrown,rosybrown1,rosybrown2,rosybrown3,rosybrown4,royalblue,royalblue1,royalblue2,royalblue3,royalblue4,saddlebrown,salmon,salmon1,salmon2,salmon3,salmon4,sandybrown,seagreen,seagreen1,seagreen2,seagreen3,seagreen4,seashell,seashell1,seashell2,seashell3,seashell4,sienna,sienna1,sienna2,sienna3,sienna4,skyblue,skyblue1,skyblue2,skyblue3,skyblue4,slateblue,slateblue1,slateblue2,slateblue3,slateblue4,slategray,slategray1,slategray2,slategray3,slategray4,slategrey,snow,snow1,snow2,snow3,snow4,springgreen,springgreen1,springgreen2,springgreen3,springgreen4,steelblue,steelblue1,steelblue2,steelblue3,steelblue4,tan,tan1,tan2,tan3,tan4,thistle,thistle1,thistle2,thistle3,thistle4,tomato,tomato1,tomato2,tomato3,tomato4,turquoise,turquoise1,turquoise2,turquoise3,turquoise4,violet,violetred,violetred1,violetred2,violetred3,violetred4,wheat,wheat1,wheat2,wheat3,wheat4,whitesmoke,yellow1,yellow2,yellow3,yellow4,yellowgreen;
	
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
		void setSaturation (float saturation);
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

