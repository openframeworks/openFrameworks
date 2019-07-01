#pragma once


#include "ofConstants.h"
#include <limits>
#include "glm/common.hpp"

/// \class ofColor_
///
/// \brief ofColor represents a color in openFrameworks.
///
/// The default ofColor uses unsigned char values [0,255], but use
/// ofFloatColor for floating point values [0.0,1.0], or ofShortColor for
/// unsigned short ints [0,[65,535]]. ofColor can be represented by an RGB
/// value, an HSB value or a hex value.
///
/// \tparam PixelType The data type used to represent a single pixel value.
template<typename PixelType>
class ofColor_{
public:
    /// \name Create an ofColor
    /// \{

    /// \brief Construct a default white color.
    ofColor_():
        r(limit()),
        g(limit()),
        b(limit()),
        a(limit()) {};

    /// \brief Construct an ofColor_ by using channel values.
    ///
    /// \param red The red component.
    /// \param green The green component.
    /// \param blue The blue component.
    /// \param alpha The alpha component.
    ofColor_(float red, float green, float blue, float alpha = limit());

    /// \brief Construct a grayscale ofColor_ by specifying a single number.
    ///
    /// \param gray A grayscale value.
    /// \param alpha The opacity of the color.
    ofColor_(float gray, float alpha = limit());

    /// \brief Construct an ofColor_ from an existing ofColor_.
    ///
    /// \warning Depending on the PixelType and SrcType used, color precision
    ///          may be lost when converting a higher precision to a lower
    ///          precision representation.
    ///
    /// \param color The ofColor_ to copy.
    template<typename SrcType>
    ofColor_(const ofColor_<SrcType>& color);

    /// \brief Construct an ofColor_ from an existing ofColor_.
    ///
    /// \param color The ofColor_ to copy.
    /// \param alpha The new alpha value to replace the alpha value in color.
    ofColor_(const ofColor_<PixelType>& color, float alpha);

    /// \brief Create an ofColor_ from an HSB representation.
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
    /// \param hexColor A color in hexadecimal form.
    /// \param alpha The alpha value of the color.
    static ofColor_<PixelType> fromHex(int hexColor, float alpha = limit());

    /// \}

    /// \name Data Accessors
    /// \{

    /// \brief A union representing the internal data.
	///
    /// \sa r, g, b and a
    union {
        struct {
            PixelType r; ///< \brief The red color component.
            PixelType g; ///< \brief The green color component.
            PixelType b; ///< \brief The blue color component.
            PixelType a; ///< \brief The alpha color component.
        };
        PixelType v[4]; ///< \brief The pixel values as an array.
    };

    /// \}

    /// \name Setters
    /// \{

    /// \brief Set an ofColor_ by using RGB values.
    ///
    /// \param red The red component.
    /// \param green The green component.
    /// \param blue The blue component.
    /// \param alpha The alpha component.
    void set(float red, float green, float blue, float alpha = limit());

    /// \brief Set a grayscale ofColor_ by specifying a single number.
    ///
    /// \param gray A grayscale value.
    /// \param alpha The opacity of the color.
    void set(float gray, float alpha = limit());

    /// \brief Set an ofColor_ from an existing ofColor_.
    ///
    /// This will use the R, G, B and A components from the passed color.
    ///
    /// \param color The ofColor_ to copy.
    void set(const ofColor_<PixelType>& color);

    /// \brief Set an ofColor_ from a hexadecimal representation.
    ///
    /// \warning The alpha value _should not_ be packed in the hexColor and
    ///     must be specified separately.
    ///
    /// \param hexColor An RGB color in hexadecimal form.
    /// \param alpha The alpha value of the color.
    void setHex(int hexColor, float alpha = limit());

    /// \brief Set the hue of this color.
    ///
    /// \param hue A hue value to set in the range of 0 - limit().
    void setHue(float hue);

    /// \brief Set the hue angle of this color.
	///
    /// \param angle A hue angle value to set in the range of 0 - 360 degrees.
    void setHueAngle(float angle);

    /// \brief Set the saturation of this color.
    ///
    /// This method uses HSB not HSL. So red desaturated is white, not gray
    ///
    /// \param saturation A saturation value value in the range of 0 - limit().
    /// \sa http://en.wikipedia.org/wiki/HSL_and_HSV
    void setSaturation(float saturation);

    /// \brief Set the brightness of this color.
    ///
    /// \param brightness A brightness value to set in the range of 0 - limit().
    void setBrightness(float brightness);

    /// \brief Set the color using HSB components.
	///
    /// \param hue A hue value to set in the range of 0 - limit().
    /// \param saturation A saturation value to set in the range of 0 - limit().
    /// \param brightness A brightness value to set in the range of 0 - limit().
    /// \param alpha An alpha value to set in the range of 0 - limit().
    void setHsb(float hue,
                float saturation,
                float brightness,
                float alpha = limit());

    /// \}

    /// \name Modifiers
    /// \{

    /// \brief Clamp values between 0 and the limit().
    ///
    /// Clamps the value of each component, R, G, B and A to a maximum of
    /// limit() and a minimum of zero.
    ///
    /// \returns A reference to itself.
    ofColor_<PixelType>& clamp();

    /// \brief Invert the R, G and B components.
    ///
    /// Performs an inversion operation on the color by replacing the red, green
    /// and blue components with their original value subtracted from the
    /// limit().
    ///
    /// \returns A reference to itself.
    ofColor_<PixelType>& invert();

    /// \brief Normalize the R, G and B components.
    ///
    /// Performs a normalization operation on the red, green and blue components
    /// by scaling them by brightness of the original color divided by the
    /// limit().  The brightness is calculated by finding the maximum of
    /// original red, green and blue components.
    /// In short: ofColor / (brightness / limit())
    ///
    /// \returns A reference to itself.
    ofColor_<PixelType>& normalize();

    /// \brief A linear interpolation between all components of two colors.
    ///
    /// The linear interpolation is carried out
    /// on _all_ components, R, G, B _and_ A.
    ///
    /// \param target The target color corresponding to an amount of 1.0.
    /// \param amount The amount (between 0.0 and 1.0) to interpolate.
    ///     A value of 0.0 will yield an unchanged color.  A value of 1.0
    ///     will yield the target color.
    /// \returns A reference to itself.
    ofColor_<PixelType>& lerp(const ofColor_<PixelType>& target, float amount);

    /// \}

    /// \name Getters
    /// \{

    /// \brief A non-destructive version of clamp().
	///
    /// \sa clamp()
    /// \returns A copy of this color, clamped.
    ofColor_<PixelType> getClamped() const;

    /// \brief A non-destructive version of invert().
	///
    /// \sa invert()
    /// \returns A copy of this color, inverted.
    ofColor_<PixelType> getInverted() const;

    /// \brief A non-destructive version of normalize().
	///
	/// \sa normalize()
    /// \returns A copy of this color, normalized.
    ofColor_<PixelType> getNormalized() const;


    /// \brief A non-destructive version of lerp().
	///
    /// \param target The target color corresponding to an amount of 1.0.
    /// \param amount The amount (between 0.0 and 1.0) to interpolate.
    ///     A value of 0.0 will yield an unchanged color.  A value of 1.0
    ///     will yield the target color.
    /// \returns A copy of this color, lerped.
    /// \sa lerp()
    ofColor_<PixelType> getLerped(const ofColor_<PixelType>& target,
                                  float amount) const;

    /// \brief Get a 24-bit hexadecimal representation of the RGB color.
    ///
    /// \warning This method does _not_ encode the alpha component.
    ///
    /// \returns An integer representing an RGB color.
    int getHex() const;

    /// \brief Get the Hue of this color.
    ///
    /// The color is converted from the default RGB to an HSB color space and
    /// the resulting Hue value is returned.  The resulting hue value will
    /// always be returned in the range 0 - limit().
    ///
    /// \returns The Hue value in the range 0 - limit().
    float getHue() const;

    /// \brief Get the Hue angle of this color.
    ///
    /// The color is converted from the default RGB to an HSB color space and
    /// the resulting Hue angle is returned.  The resulting hue value will
    /// always be returned in degrees in the range 0 - 360.
    ///
    /// \returns The Hue angle in the range 0 - 360.
    float getHueAngle() const;

    /// \brief Get the Saturation of this color.
    ///
    /// The color is converted from the default RGB to an HSB color space and
    /// the resulting saturation is returned.  The resulting saturation value
    /// will always be returned in the range 0 - limit().
    ///
    /// \returns The saturation in the range 0 - limit().
    float getSaturation() const;

    /// \brief Calculate the brightness of of the R, G and B components.
    ///
    /// Brightness is simply the maximum of the three color components. This
    /// method of calculating brightness is used by Photoshop (HSB) and
    /// Processing (HSB).  Note that brightness is also called "Value".
    ///
    /// \returns the brightness in the range 0 - limit().
    float getBrightness() const;

    /// \brief Calculate the lightness of the R, G and B components.
    ///
    /// Lightness is simply the average of the three color components. This
    /// method of calculating lightness is used by the Lab and HSL color spaces.
    ///
    /// \returns the lightness in the range 0 - limit().
    float getLightness() const;

    /// \brief Extract the hue, saturation and brightness (HSB) from this color.
    ///
    /// \param hue A reference to the hue to fill. Will be in the range of
    ///     0 - limit().
    /// \param saturation A reference to the saturation to fill. Will be in the
	///		range of 0 - limit().
    /// \param brightness A reference to the brightness to fill. Will be in the
    ///     range of 0 - limit().
    void getHsb(float& hue, float& saturation, float& brightness) const;

    /// \brief Get the maximum value of a color component.
    ///
    /// \returns The value associated with a fully saturated color component.
    static float limit();

    /// \}

    /// \name Operators
    /// \{

    /// \brief Assign a color using an existing color.
    ///
    /// R, G, B and A components are set to the the values of the assigned
    /// color.
    ///
    /// \warning Depending on the PixelType and SrcType used, color precision
    ///     may be lost when converting a higher precision to a lower precision
    ///     representation.
    ///
    /// \param color The color to assign.
    /// \returns A reference to itself.
    template<typename SrcType>
    ofColor_<PixelType>& operator = (const ofColor_<SrcType>& color);

    /// \brief Assign a color using a grayscale value.
    ///
    /// \param value The grayscale value.
    /// \returns A reference to itself.
    ofColor_<PixelType>& operator = (float value);

    /// \brief Test two colors for equality.
	///
    /// \returns true iff the R, G, B and A components are all equal.
    bool operator == (const ofColor_<PixelType>& color) const;

    /// \brief Test two colors for inequality.
    /// \returns true iff any of the R, G, B or A components are not equal.
    bool operator != (const ofColor_<PixelType>& color) const;

    /// \brief Clamped addition operator.
    ///
    /// Add two colors by summing and clamping their R, G and B components.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param color The color to add.
    /// \returns The new clamped color.
    ofColor_<PixelType>  operator +  (const ofColor_<PixelType>& color) const;

    /// \brief Clamped addition operator.
    ///
    /// Add a value to each of the R, G and B components and clamp each.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param color The value to add.
    /// \returns The new clamped color.
    ofColor_<PixelType>  operator +  (float color) const;

    /// \brief Clamped addition operator.
    ///
    /// Add two colors by summing and clamping their R, G and B components.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param color The color to add.
    /// \returns A reference to itself, the new clamped color.
    ofColor_<PixelType>& operator += (const ofColor_<PixelType>& color);

    /// \brief Clamped addition operator.
    ///
    /// Add a value to each of the R, G and B components and clamp each.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param color The value to add.
    /// \returns A reference to itself, the new clamped color.
    ofColor_<PixelType>& operator += (float color);

    /// \brief Clamped subtraction operator.
    ///
    /// Subtract two colors by subtracting and clamping their R, G and B
    /// components.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param color The color to subtract.
    /// \returns The new clamped color.
    ofColor_<PixelType>  operator -  (const ofColor_<PixelType>& color) const;

    /// \brief Clamped subtraction operator.
    ///
    /// Subtract a value from each of the R, G and B components and clamp each.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param value The value to subtract.
    /// \returns The new clamped color.
    ofColor_<PixelType>  operator -  (float value) const;

    /// \brief Clamped subtraction operator.
    ///
    /// Add two colors by subtracting and clamping their R, G and B components.
    ///
    /// \param color The color to subtract.
    /// \returns A reference to itself, the new clamped color.
    ofColor_<PixelType>& operator -= (const ofColor_<PixelType>& color);

    /// \brief Clamped subtraction operator.
    ///
    /// Subtract a value to each of the R, G and B components and clamp each.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param color The value to subtract.
    /// \returns A reference to itself, the new clamped color.
    ofColor_<PixelType>& operator -= (float color);

    /// \brief Clamped multiplication operator.
    ///
    /// Multiply two colors by multiplying and clamping their R, G and B
    /// components.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param color The color to multiply.
    /// \returns The new clamped color.
    ofColor_<PixelType>  operator *  (const ofColor_<PixelType>& color) const;

    /// \brief Clamped scalar multiplication operator.
    ///
    /// Multiply the R, G and B components by a scaler and clamp each.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param value The value to multiply.
    /// \returns The new clamped color.
    ofColor_<PixelType>  operator *  (float value) const;

    /// \brief Clamped multiplication operator.
    ///
    /// Multiply two colors by multiplying and clamping their R, G and B
    /// components.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param color The color to multiply.
    /// \returns A reference to itself, the new clamped color.
    ofColor_<PixelType>& operator *= (const ofColor_<PixelType>& color);

    /// \brief Clamped scalar multiplication operator.
    ///
    /// Multiply the R, G and B components by a scaler and clamp each.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param value The scaler value.
    /// \returns A reference to itself, the new clamped color.
    ofColor_<PixelType>& operator *= (float value);

    /// \brief Clamped division operator.
    ///
    /// Divide two colors by treating the passed color components as divisors.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param color The divisor color.
    /// \returns The new clamped color.
    ofColor_<PixelType>  operator /  (const ofColor_<PixelType>& color) const;

    /// \brief Clamped scalar division operator.
    ///
    /// Divide each of the R, G and B components by a scalar and clamp each.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param value The divisor value.
    /// \returns The new clamped color.
    ofColor_<PixelType>  operator /  (float value) const;

    /// \brief Clamped division operator.
    ///
    /// Divide two colors by treating the passed color components as divisors.
    ///
    /// \param color The divisor color.
    /// \returns A reference to itself, the new clamped color.
    ofColor_<PixelType>& operator /= (const ofColor_<PixelType>& color);

    /// \brief Clamped scalar division operator.
    ///
    /// Divide each of the R, G and B components by a scalar and clamp each.
    ///
    /// \warning The alpha component is ignored.
    ///
    /// \param value The divisor value.
    /// \returns A reference to itself, the new clamped color.
    ofColor_<PixelType>& operator /= (float value);

    /// \brief Array subscript operator.
    ///
    /// If n is 0 returns .r, if 1 returns .g, if 2 returns .b, if 3 returns
    /// alpha.
    ///
    /// \param n An index 0-3 of the component to get.
    /// \returns The value of the requested component.
    const PixelType& operator [] (std::size_t n) const;

    /// \brief Array subscript operator.
    ///
    /// If n is 0 returns .r, if 1 returns .g, if 2 returns .b, if 3 returns
    /// alpha.
    ///
    /// \param n An index 0-3 of the component to get.
    /// \returns The value of the requested component.
	PixelType& operator [] (std::size_t n);

    /// \brief An output stream operator.
    ///
    /// Colors can be serialized into comma separated values (CSV) using the
    /// output stream operator.
    ///
    /// \param os An output stream reference.
    /// \param color The color to write to the output stream.
    /// \returns The passed output stream reference, useful for method chaining.
	friend std::ostream& operator << (std::ostream& os, const ofColor_<PixelType>& color) {
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
    /// stream using the input stream operator.
    ///
    /// \param is An input stream reference.
    /// \param color The color to fill with the input stream.
    /// \returns The passed input stream reference, useful for method chaining.
	friend std::istream& operator >> (std::istream& is, ofColor_<PixelType>& color) {
		if(sizeof(PixelType) == 1) {
			int component;
			is >> std::skipws >> component;
			color.r = component;
			is.ignore(1);
			is >> std::skipws >> component;
			color.g = component;
			is.ignore(1);
			is >> std::skipws >> component;
			color.b = component;
			is.ignore(1);
			is >> std::skipws >> component;
			color.a = component;
		}else{
			is >> std::skipws >> color.r;
			is.ignore(1);
			is >> std::skipws >> color.g;
			is.ignore(1);
			is >> std::skipws >> color.b;
			is.ignore(1);
			is >> std::skipws >> color.a;
		}
		return is;
	}


    /// \}

    /// \name Predefined Colors
	///
	/// \brief A collection of static colors defined by name.
    ///
    /// These constants allow the user to access colors by name.  For example,
    /// if using ofColor, one might set a color in the following way:
    ///
    /// ~~~~{.cpp}
    ///     ofColor myColor = ofColor::white;
    ///     // myColor is now full white (255,255,255,255).
    /// ~~~~
    ///
    /// The list of colors is based upon the CSS names colors and can be viewed
    /// [here](http://www.w3schools.com/cssref/css_colornames.asp).
    ///
    /// \{

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


    /// \}


private:
    template<typename SrcType>
    void copyFrom(const ofColor_<SrcType>& mom);

};

/// \name Variants
/// \{

/// \typedef ofColor
/// \brief A typedef representing an 8-bit (`unsigned char`) RGBA color.
typedef ofColor_<unsigned char> ofColor;

/// \typedef ofShortColor
/// \brief A typedef representing a 16-bit (`unsigned short`) RGBA color.
typedef ofColor_<unsigned short> ofShortColor;

/// \typedef ofFloatColor
/// \brief A typedef representing a floating (`float`) point RGBA color.
typedef ofColor_<float> ofFloatColor;

/// \}


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

	if(typeid(SrcType) == typeid(float) || typeid(SrcType) == typeid(double)) {
		// coming from float we need a special case to clamp the values
		for(int i = 0; i < 4; i++){
			v[i] = glm::clamp(float(mom[i]), 0.f, 1.f) * factor;
		}
	} else{
		// everything else is a straight scaling
		for(int i = 0; i < 4; i++){
			v[i] = mom[i] * factor;
		}
	}
}

template <typename PixelType>
ofColor_<PixelType> operator*(float val, const ofColor_<PixelType> &color) {
	return color * val;
}

template<typename PixelType>
inline float ofColor_<PixelType>::limit() {
	return std::numeric_limits<PixelType>::max();
}

template<>
inline float ofColor_<float>::limit() {
	return 1.f;
}

template<>
inline int ofColor_<unsigned char>::getHex() const {
	return
		((0xff & (unsigned char) r) << 16) |
		((0xff & (unsigned char) g) << 8) |
		((0xff & (unsigned char) b));
}

template<typename PixelType>
inline int ofColor_<PixelType>::getHex() const {
	return ((ofColor) *this).getHex();
}

template<>
inline void ofColor_<unsigned char>::setHex(int hexColor, float alpha){
	r = (hexColor >> 16) & 0xff;
	g = (hexColor >> 8) & 0xff;
	b = (hexColor >> 0) & 0xff;
	a = alpha;
}

template<typename PixelType>
inline void ofColor_<PixelType>::setHex (int hexColor, float alpha){
	ofColor c = ofColor::fromHex(hexColor);
	*this = c;
	a = alpha;
}


extern template class ofColor_<char>;
extern template class ofColor_<unsigned char>;
extern template class ofColor_<short>;
extern template class ofColor_<unsigned short>;
extern template class ofColor_<int>;
extern template class ofColor_<unsigned int>;
extern template class ofColor_<long>;
extern template class ofColor_<unsigned long>;
extern template class ofColor_<float>;
extern template class ofColor_<double>;
