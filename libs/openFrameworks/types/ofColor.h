#pragma once


#include <limits>
#include "ofMath.h"


/// \class ofColor_
/// \brief A class template to represent a color with variable precision.
///
/// ofColor_ is a class template for representing a color using the
/// [RGB model][1].  Consequently, ofColor_ stores three numbers representing
/// representing the R (red), G (green) and B (blue) color components.  In
/// addition to the R, G and B components, ofColor_ represent's a color's
/// opacity with a fourth alpha component.  An alpha value of zero represents a
/// completely transparent color.  A maximal alpha value (which depends upon
/// the range and precision of the underlying data type) represents an opaque
/// color.
///
/// Since each of the R, G, B and A components is represented by any underlying
/// numerical PixelType, the choice of PixelType affects both the range of
/// values and resulting precision of the RGB color representation.
///
/// Typically users will not use this class template directly, but will use the
/// convenient pre-defined typedefs.
///
/// |   Typedef     |    PixelType     | Bit Depth | Min. Value | Max. Value  |
/// | ------------- | ---------------- | --------- | ---------- | ----------- |
/// | `ofColor`     | `unsigned char`  | 8         | 0          | 255         |
/// | `ofShortColor`| `unsigned short` | 16        | 0          | 65535       |
/// | `ofFloatColor`| `float`          | _varies_  | 0.0        | 1.0         |
///
/// For integral PixelTypes (e.g. `unsigned char` and `unsigned short`, the
/// minimum is zero and the maximum is given by:
///
///     std::numeric_limits<PixelType>::max();
///
/// For floating point PixelsTypes, the maximum is 1.0.
///
/// The ofColor typedef represents an 8-bit RGBA color.  Thus, each component
/// has a maximum value of 255 and a minimum value of 0.  The ofColor typedef
/// is the default color type in openFrameworks and will work for most
/// user applications.
///
/// The ofFloatColor and ofShortColor represent higher precision representations
/// of a given color and are used in special cases where precision color math
/// is required.  ofFloatColor might be used for specialized image processing,
/// such as image averaging where rounding errors might otherwise introduce
/// unaccaptable visual artifacts.  ofShortColor might be used with depth camera
/// images that can represent the "depth" of each pixel with greater than 256
/// unique levels.
///
/// [1]: http://en.wikipedia.org/wiki/RGB_color_model "RGB Model"
///
template<typename PixelType>
class ofColor_{
public:
    /// \brief Construct an ofColor_ instance.
    ofColor_<PixelType>();

    /// \brief Destroy an ofColor_ instance.
    ~ofColor_<PixelType>();

    /// \brief Construct an ofColor_ by using channel values.
    ///
    /// When creating an instance of ofColor_ the channel values must fall
    /// within the range represented by the PixelType.  By default, the alpha
    /// component will take the PixelType's maximum, producing an opaque color.
    ///
    /// \param red The red component.
    /// \param green The green component.
    /// \param blue The blue component.
    /// \param alpha The alpha component.
    ofColor_<PixelType>(float red,
                        float green,
                        float blue,
                        float alpha = limit());

    /// \brief Construct an ofColor_ from an existing ofColor_.
    /// \param color The ofColor_ to copy.
    ofColor_<PixelType>(const ofColor_<PixelType>& color);

    /// \brief Construct an ofColor_ from an existing ofColor_.
    ///
    /// This constructor will use the r, g, and b components from the passed
    /// color ignoring its alpha channel.  The provided alpha channel will be
    /// used instead.
    ///
    /// \param color The ofColor_ to copy.
    /// \param alpha The new alpha value to replace the alpha value in color.
    ofColor_<PixelType>(const ofColor_<PixelType>& color, float alpha);

    /// \brief Construct a grayscale ofColor_ by specifiying a single number.
    ///
    /// This constructor will use a single value to describe a grayscale color.
    /// This single value will be taken by each of the r, g, and b components.
    /// The alpha channel is specified separately and will and will default to
    /// the PixelType's maximum resulting in an opaque color.
    ///
    /// \param gray A grayscale value.
    /// \param alpha The opacity of the color.
    ofColor_<PixelType>(float gray, float alpha = limit());

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
    ofColor_<PixelType>(const ofColor_<SrcType>& color);

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
    static ofColor_<PixelType> fromHsb(float hue,
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
    static ofColor_<PixelType> fromHex(int hexColor, float alpha = limit());
    
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
    /// \param red The red component.
    /// \param green The green component.
    /// \param blue The blue component.
    /// \param alpha The alpha component.
    void set(float red, float green, float blue, float alpha = limit());

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
    void set(float gray, float alpha = limit());

    /// \brief Set an ofColor_ from an existing ofColor_.
    ///
    /// This will use the r, g, b and a components from the passed color.
    ///
    /// \param color The ofColor_ to copy.
    /// \param _a The new alpha value to replace the alpha value in color.
    void set(const ofColor_<PixelType>& color);

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
    void setHex(int hexColor, float alpha = limit());

    /// \returns A hexadecimal representation of the ofColor_.
    int getHex() const;

    /// \brief Clamp values between 0 and the limit().
    ///
    /// Clamps the value of each component, r, g, b and a to a maximum of
    /// limit() and a minimum of zero.
    ///
    /// \returns A reference to itself.
    ofColor_<PixelType>& clamp();

    /// \brief Invert the r, g and b components.
    ///
    /// Performs an inversion operation on the color by replacing the red, green
    /// and blue components with their original value subtracted from the
    /// limit().  For example, an ofColor value of:
    ///
    ///     unsigned char maxValue = limit(); // would return 255
    ///
    ///     unsigned char r = 255;
    ///     unsigned char g = 255;
    ///     unsigned char b = 0;
    ///
    ///     unsigned char rNew = maxValue - r; // 255 - 255 = 0
    ///     unsigned char gNew = maxValue - g; // 255 - 255 = 0
    ///     unsigned char bNew = maxValue - b; // 255 - 0   = 255;
    ///
    /// Thus the inversion of `ofColor(255, 255, 0)` is `ofColor(0, 0, 255);`
    ///
    /// \returns A reference to itself.
    ofColor_<PixelType>& invert();

    /// \brief Normalize the r, g and b components.
    ///
    /// Performs a normalization operation on the red, green and blue components
    /// by scaling them by brightness of the original color divided by the
    /// limit().  The brightness is calculated by finding the maximum of
    /// original red, green and blue components.  This operation is equivalent
    /// to the following:
    ///
    ///     ofColor myColor(127, 0, 0, 127);
    ///
    ///     float brightness = myColor.getBrightness(); // returns 127.
    ///     float scaleFactor = brightness / limit();   // returns 127 / 255.
    ///
    ///     ofColor myNormalizedColor = myColor / scaleFactor;
    ///                                 // Divides the red, green and blue
    ///                                 // components by the scale factor.
    ///
    /// \returns A reference to itself.
    ofColor_<PixelType>& normalize();

    ofColor_<PixelType>& lerp(const ofColor_<PixelType>& target, float amount);
    
    ofColor_<PixelType> getClamped() const;
    ofColor_<PixelType> getInverted() const;
    ofColor_<PixelType> getNormalized() const;
    ofColor_<PixelType> getLerped(const ofColor_<PixelType>& target,
                                  float amount) const;
    
    float getHue() const; // 0-255
    float getHueAngle() const; // 0-360
    float getSaturation() const;

    // brightest component
    /*
     Brightness is simply the maximum of the three color components.
     This is used by Photoshop (HSB) and Processing (HSB).
     Brightness is also called "value".
     */
    

    float getBrightness() const;

    /*
     Lightness is the average of the three color components.
     This is used by the Lab and HSL color spaces.
     */
    
    float getLightness() const;
    void getHsb(float& hue, float& saturation, float& brightness) const;

    // 0-255
    void setHue(float hue);

    // 0-360
    void setHueAngle(float angle);

    // uses hsb, not hsl. so red desaturated is white, not gray http://en.wikipedia.org/wiki/HSL_and_HSV
    void setSaturation(float saturation);
    void setBrightness(float brightness);
    void setHsb(float hue, float saturation, float brightness, float alpha = limit());
    
    ofColor_<PixelType>& operator = (const ofColor_<PixelType>& color);

    template<typename SrcType>
    ofColor_<PixelType>& operator = (const ofColor_<SrcType>& color);

    ofColor_<PixelType>& operator = (const float& val);
    bool operator == (const ofColor_<PixelType>& color);
    bool operator != (const ofColor_<PixelType>& color);
    ofColor_<PixelType>  operator +  (const ofColor_<PixelType>& color) const;
    ofColor_<PixelType>  operator +  (const float& val) const;
    ofColor_<PixelType>& operator += (const ofColor_<PixelType>& color);
    ofColor_<PixelType>& operator += (const float& val);
    ofColor_<PixelType>  operator -  (const ofColor_<PixelType>& color) const;
    ofColor_<PixelType>  operator -  (const float& val) const;
    ofColor_<PixelType>& operator -= (const ofColor_<PixelType>& color);
    ofColor_<PixelType>& operator -= (const float& val);
    ofColor_<PixelType>  operator *  (const ofColor_<PixelType>& color) const;
    ofColor_<PixelType>  operator *  (const float& val) const;
    ofColor_<PixelType>& operator *= (const ofColor_<PixelType>& color);
    ofColor_<PixelType>& operator *= (const float& val);
    ofColor_<PixelType>  operator /  (const ofColor_<PixelType>& color) const;
    ofColor_<PixelType>  operator /  (const float& val) const;
    ofColor_<PixelType>& operator /= (const ofColor_<PixelType>& color);
    ofColor_<PixelType>& operator /= (const float& val);

    const PixelType& operator [] (int n) const;
    PixelType& operator [] (int n);

    /// \brief An output stream operator.
    ///
    /// Colors can be serialized into comma separated values (CSV) using the
    /// output stream operator.  Input stream values are written in the
    /// following form
    ///
    ///     RED_VALUE, GREEN_VALUE, BLUE_VALUE, ALPHA_VALUE
    ///
    /// For example, to write output into a std::stringstream for later
    /// retrieval using the ofColor_ input stream operator one might use the
    /// following:
    ///
    ///     std::stringstream ss;     // Create an empty std::string stream.
    ///                               // Note: std::stringstream is just one
    ///                               // option for an output stream.  Another
    ///                               // popular output stream std::cout.
    ///
    ///     ofColor myColor(255, 255, 0, 255);
    ///                               // Create an ofColor to output.
    ///
    ///     ss << myColor;            // Write the color to the output stream.
    ///
    /// \param os An output stream reference.
    /// \param color The color to write to the output stream.
    /// \returns The passed output stream reference, useful for method chaining.
    friend ostream& operator << (ostream& os, const ofColor_<PixelType>& color) {
        if(sizeof(PixelType) == 1) {
            os << (int) color.r << ", " << (int) color.g << ", " << (int) color.b << ", " << (int) color.a;
        } else {
            os << color.r << ", " << color.g << ", " << color.b << ", " << color.a;
        }
        return os;
    }

    /// \brief An input stream operator.
    ///
    /// Colors can be deserialized from comma separated values (CSV) input
    /// stream using the input stream operator.  Input stream values are read
    /// in the following form
    ///
    ///     RED_VALUE, GREEN_VALUE, BLUE_VALUE, ALPHA_VALUE
    ///
    /// For example, to read input from a std::stringstream into an ofColor
    /// one might use the following:
    ///
    ///     std::stringstream ss;     // Create an empty std::string stream.
    ///                               // Note: std::stringstream is just one
    ///                               // option for an input stream.
    ///     ss << "255, 255, 0, 255"; // Write the color to the stringstream.
    ///     ofColor myColor;          // Create an ofColor instance to fill.
    ///     ss >> myColor;            // myColor is now filled with a yellow.
    ///
    /// \param is An input stream reference.
    /// \param color The color to fill with the input stream.
    /// \returns The passed input stream reference, useful for method chaining.
    friend istream& operator >> (istream& is, ofColor_<PixelType>& color) {
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

    /// \brief Return the maximum value of a color.
    /// \return The value associated with a fully saturated color component.
    static float limit();

    /// \union A union.
    union  {
        /// \struct A struct
        struct {
            PixelType r; //< \brief The red color component.
            PixelType g; //< \brief The green color component.
            PixelType b; //< \brief The blue color component.
            PixelType a; //< \brief The alpha color component.
        };
        PixelType v[4]; ///< \brief The pixel values as an array.
    };
    
private:
    template<typename SrcType>
    void copyFrom(const ofColor_<SrcType>& mom);
};

typedef ofColor_<unsigned char> ofColor;
    ///< \brief A typedef representing an 8-bit RGBA color.

typedef ofColor_<unsigned short> ofShortColor;
    ///< \brief A typedef representing a 16-bit RGBA color.

typedef ofColor_<float> ofFloatColor;
    ///< \brief A typedef representing a floating point RGBA color.

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

