#pragma once


#include <limits>
#include "ofMath.h"


/// \class ofColor_
/// \brief A class template representing color at various levels of precision.
///
/// ofColor_ is a class template for representing a single color composed of 4
/// channels (red, green, blue and alpha) at various levels of precision.
/// A color's alpha value represents the color's opacity.  A high alpha value
/// means that the color is more opaque.  Lower values represent transparency.
/// The class template provides a corresponding set of templated utilities for
/// manipulating and converting the color.  Typically users will not use this
/// class template directly, but will use the convenient pre-defined typedefs
/// discussed below.
///
/// The ofColor typedef represents 8 bit color values, resulting in color
/// channel values in the range of 0 - 255 per channel.  This level of precision
/// is the default level of precision and is used extensively throughout
/// openFrameworks.
///
/// The ofFloatColor and ofShortColor represent higher precision representations
/// of a given color and are used in special cases where higher precision color
/// manipulation is required.  Examples cases include complex image compositing
/// where rounding errors due to low precision color representations might
/// introduce unaccaptable visual artifacts or when representing depth camera
/// images that can represent the "depth" at each pixel with greater than 256
/// unique levels.
template<typename PixelType>
class ofColor_{
public:
    /// \brief Construct an ofColor_ instance.
    ofColor_<PixelType> ();

    /// \brief Destroy an ofColor_ instance.
    ~ofColor_<PixelType> ();

    /// \brief Construct an ofColor_ by using channel values.
    ///
    /// When creating an instance of ofColor_ the channel values must fall
    /// within the range represented by the PixelType.  By default, the alpha
    /// component will take the PixelType's maximum, producing an opaque color.
    ///
    /// \param _r The red component.
    /// \param _g The green component.
    /// \param _b The blue component.
    /// \param _a The alpha component.
    ofColor_<PixelType> (float _r, float _g, float _b, float _a = limit());

    /// \brief Construct an ofColor_ from an existing ofColor_.
    /// \param color The ofColor_ to copy.
    ofColor_<PixelType> (const ofColor_<PixelType> & color);

    /// \brief Construct an ofColor_ from an existing ofColor_.
    ///
    /// This constructor will use the r, g, and b components from the passed
    /// color ignoring its alpha channel.  The provided alpha channel will be
    /// used instead.
    ///
    /// \param color The ofColor_ to copy.
    /// \param _a The new alpha value to replace the alpha value in color.
    ofColor_<PixelType> (const ofColor_<PixelType> & color, float _a);

    /// \brief Construct a grayscale ofColor_ by specifiying a single number.
    ///
    /// This constructor will use a single value to describe a grayscale color.
    /// This single value will be taken by each of the r, g, and b components.
    /// The alpha channel is specified separately and will and will default to
    /// the PixelType's maximum resulting in an opaque color.
    ///
    /// \param gray A grayscale value.
    /// \param alpha The opacity of the color.
    ofColor_<PixelType> (float gray, float _a = limit());

    /// \brief Construct an ofColor_ from an existing ofColor_.
    ///
    /// This constructor will attempt to convert between color instances of
    /// differing precision.
    ///
    /// \warning Depending on the PixelType and SrcType used, color precision
    ///     may be lost when converting a higher precision to a lower precision
    ///     representation.
    ///
    /// \param color The ofColor_ to copy.
    template<typename SrcType>
    ofColor_<PixelType> (const ofColor_<SrcType> & color);

    /// \brief Create an ofColor_ from an HSB representation.
    ///
    /// While ofColor_ represents color using R, G, B, and A components, other
    /// representations are available.  This method allows the user to construct
    /// an ofColor_ from a Hue (H), Saturation (S), and Brightness (B) and an
    /// optional alpha value.
    ///
    /// The alpha channel is specified separately and will default to the
    /// PixelType's maximum, resulting in an opaque color.
    ///
    /// For more information about HSB, see:
    ///     http://en.wikipedia.org/wiki/HSV_color_space
    ///
    /// \param hue The hue of the color.
    /// \param saturation The saturation of the color.
    /// \param brightness The brightness of the color.
    /// \param alpha The alpha value of the color.
    static ofColor_<PixelType> fromHsb (float hue,
                                        float saturation,
                                        float brightness,
                                        float alpha = limit());

    /// \brief Create an ofColor_ from a hexadecimal value.
    ///
    /// In some cases, it is convenient to represent colors using a hexadecimal
    /// number.  In this case, red, green and blue values are packed into a
    /// single integer.
    ///
    /// The alpha channel is specified separately and will default to the
    /// PixelType's maximum, resulting in an opaque color.
    ///
    /// \param hexColor A color in hexadecimal form.
    /// \param alpha The alpha value of the color.
    static ofColor_<PixelType> fromHex (int hexColor, float alpha = limit());
    
    /// \brief A collection of static colors defined by name.
    ///
    /// These constants allow the user to access colors by name.  For example,
    /// if using ofColor, one might set a color in the following way:
    ///
    ///     ofColor myColor = ofColor::white;
    ///
    /// This will result in myColor being set to a fully opaque white.  The list of
    /// colors is based upon the CSS names colors and can be viewed here:
    /// http://www.w3schools.com/cssref/css_colornames.asp
    ///
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

    /// \brief Set an ofColor_ by using channel values.
    ///
    /// When modifying an instance of ofColor_ the channel values must fall
    /// within the range represented by the PixelType.  By default, the alpha
    /// component will take the PixelType's maximum, producing an opaque color.
    ///
    /// \param _r The red component.
    /// \param _g The green component.
    /// \param _b The blue component.
    /// \param _a The alpha component.
    void set (float _r, float _g, float _b, float _a = limit());

    /// \brief Set a grayscale ofColor_ by specifiying a single number.
    ///
    /// When modifying an instance of ofColor_ you can use a single value to
    /// describe a grayscale color. This single value will be taken by each of
    /// the r, g, and b components. The alpha channel is specified separately
    /// and will and will default to the PixelType's maximum resulting in an
    /// opaque color.
    ///
    /// \param gray A grayscale value.
    /// \param alpha The opacity of the color.
    void set (float _gray, float _a = limit());

    /// \brief Set an ofColor_ from an existing ofColor_.
    ///
    /// This constructor will use the r, g, b and a components from the passed
    /// color.
    ///
    /// \param color The ofColor_ to copy.
    /// \param _a The new alpha value to replace the alpha value in color.
    void set (ofColor_<PixelType> const & color);

    /// \brief Set an ofColor_ from a hexadecimal representation.
    ///
    /// In some cases, it is convenient to represent colors using a hexadecimal
    /// number.  In this case, red, green and blue values are packed into a
    /// integer.
    ///
    /// The alpha channel is specified separately and will default to the
    /// PixelType's maximum, resulting in an opaque color.
    ///
    /// \param hexColor A color in hexadecimal form.
    /// \param alpha The alpha value of the color.
    void setHex (int hexColor, float alpha = limit());

    /// \returns A hexadecimal representation of the ofColor_.
    int getHex () const;
    
    ofColor_<PixelType>& clamp ();
    ofColor_<PixelType>& invert ();
    ofColor_<PixelType>& normalize ();
    ofColor_<PixelType>& lerp(const ofColor_<PixelType>& target, float amount);
    
    ofColor_<PixelType> getClamped () const;
    ofColor_<PixelType> getInverted () const;
    ofColor_<PixelType> getNormalized () const;
    ofColor_<PixelType> getLerped(const ofColor_<PixelType>& target, float amount) const;
    
    float getHue () const; // 0-255
    float getHueAngle () const; // 0-360
    float getSaturation () const;
    float getBrightness () const; // brightest component
    float getLightness () const; // average of the components
    void getHsb(float& hue, float& saturation, float& brightness) const;
    
    void setHue (float hue); // 0-255
    void setHueAngle (float angle); // 0-360
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

