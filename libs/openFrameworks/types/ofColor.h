#pragma once


#include <limits>
#include "ofMath.h"

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
	constexpr ofColor_():
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
	constexpr ofColor_(PixelType red, PixelType green, PixelType blue, PixelType alpha = limit()):
		r(red),
		g(green),
		b(blue),
		a(alpha){
	}

    /// \brief Construct a grayscale ofColor_ by specifying a single number.
    /// 
    /// \param gray A grayscale value.
    /// \param alpha The opacity of the color.
	constexpr ofColor_(PixelType gray, PixelType alpha = limit()):
		r(gray),
		g(gray),
		b(gray),
		a(alpha){
	}

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
	constexpr ofColor_(const ofColor_<PixelType>& color, PixelType alpha):
		r(color.r),
		g(color.g),
		b(color.b),
		a(alpha){
	}

    /// \brief Create an ofColor_ from an HSB representation.
    ///
    /// \param hue The hue of the color.
    /// \param saturation The saturation of the color.
    /// \param brightness The brightness of the color.
    /// \param alpha The alpha value of the color.
    static ofColor_<PixelType> fromHsb(float hue,
                                       float saturation,
                                       float brightness,
									   PixelType alpha = limit());

    /// \brief Create an ofColor_ from a hexadecimal value.
    ///
    /// \param hexColor A color in hexadecimal form.
    /// \param alpha The alpha value of the color.
	static ofColor_<PixelType> fromHex(int hexColor, PixelType alpha = limit());

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
	void set(PixelType red, PixelType green, PixelType blue, PixelType alpha = limit());

    /// \brief Set a grayscale ofColor_ by specifying a single number.
    ///
    /// \param gray A grayscale value.
    /// \param alpha The opacity of the color.
	void set(PixelType gray, PixelType alpha = limit());

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
	void setHex(int hexColor, PixelType alpha = limit());

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
				PixelType alpha = limit());

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
	constexpr static float flimit();

	/// \brief Get the maximum value of a color component.
	///
	/// \returns The value associated with a fully saturated color component.
	constexpr static PixelType limit();

	/// \brief Cast a float to this color type PixelType.
	constexpr static PixelType fromFloat(float v){
		return PixelType(float(std::numeric_limits<PixelType>::max()) * v);
	}

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
	ofColor_<PixelType>& operator = (PixelType value);

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

	/// \brief Get the maximum value of a color component.
	///
	/// \returns The value associated with a fully saturated color component.
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
	static constexpr ofColor_<PixelType> gray{fromFloat(0.5f), fromFloat(0.5f), fromFloat(0.5f)};
	static constexpr ofColor_<PixelType> white{limit(), limit(), limit()};
	static constexpr ofColor_<PixelType> black{0,0,0};
	static constexpr ofColor_<PixelType> red{limit(),0,0};
	static constexpr ofColor_<PixelType> green{0,limit(),0};
	static constexpr ofColor_<PixelType> blue{0,0,limit()};
	static constexpr ofColor_<PixelType> cyan{0, limit(), limit()};
	static constexpr ofColor_<PixelType> magenta{limit(), 0, limit()};
	static constexpr ofColor_<PixelType> yellow{limit(), limit(), 0};
	static constexpr ofColor_<PixelType> aliceBlue{fromFloat(0.941176f),fromFloat(0.972549f),limit()};
	static constexpr ofColor_<PixelType> antiqueWhite{fromFloat(0.980392f),fromFloat(0.921569f),fromFloat(0.843137f)};
	static constexpr ofColor_<PixelType> aqua{0,limit(),limit()};
	static constexpr ofColor_<PixelType> aquamarine{fromFloat(0.498039f),limit(),fromFloat(0.831373f)};
	static constexpr ofColor_<PixelType> azure{fromFloat(0.941176f),limit(),limit()};
	static constexpr ofColor_<PixelType> beige{fromFloat(0.960784f),fromFloat(0.960784f),fromFloat(0.862745f)};
	static constexpr ofColor_<PixelType> bisque{limit(),fromFloat(0.894118f),fromFloat(0.768627f)};
	static constexpr ofColor_<PixelType> blanchedAlmond{limit(),fromFloat(0.921569f),fromFloat(0.803922f)};
	static constexpr ofColor_<PixelType> blueViolet{fromFloat(0.541176f),fromFloat(0.168627f),fromFloat(0.886275f)};
	static constexpr ofColor_<PixelType> brown{fromFloat(0.647059f),fromFloat(0.164706f),fromFloat(0.164706f)};
	static constexpr ofColor_<PixelType> burlyWood{fromFloat(0.870588f),fromFloat(0.721569f),fromFloat(0.529412f)};
	static constexpr ofColor_<PixelType> cadetBlue{fromFloat(0.372549f),fromFloat(0.619608f),fromFloat(0.627451f)};
	static constexpr ofColor_<PixelType> chartreuse{fromFloat(0.498039f),limit(),0};
	static constexpr ofColor_<PixelType> chocolate{fromFloat(0.823529f),fromFloat(0.411765f),fromFloat(0.117647f)};
	static constexpr ofColor_<PixelType> coral{limit(),fromFloat(0.498039f),fromFloat(0.313726f)};
	static constexpr ofColor_<PixelType> cornflowerBlue{fromFloat(0.392157f),fromFloat(0.584314f),fromFloat(0.929412f)};
	static constexpr ofColor_<PixelType> cornsilk{limit(),fromFloat(0.972549f),fromFloat(0.862745f)};
	static constexpr ofColor_<PixelType> crimson{fromFloat(0.862745f),fromFloat(0.0784314f),fromFloat(0.235294f)};
	static constexpr ofColor_<PixelType> darkBlue{0,0,fromFloat(0.545098f)};
	static constexpr ofColor_<PixelType> darkCyan{0,fromFloat(0.545098f),fromFloat(0.545098f)};
	static constexpr ofColor_<PixelType> darkGoldenRod{fromFloat(0.721569f),fromFloat(0.52549f),fromFloat(0.0431373f)};
	static constexpr ofColor_<PixelType> darkGray{fromFloat(0.662745f),fromFloat(0.662745f),fromFloat(0.662745f)};
	static constexpr ofColor_<PixelType> darkGrey{fromFloat(0.662745f),fromFloat(0.662745f),fromFloat(0.662745f)};
	static constexpr ofColor_<PixelType> darkGreen{0,fromFloat(0.392157f),0};
	static constexpr ofColor_<PixelType> darkKhaki{fromFloat(0.741176f),fromFloat(0.717647f),fromFloat(0.419608f)};
	static constexpr ofColor_<PixelType> darkMagenta{fromFloat(0.545098f),0,fromFloat(0.545098f)};
	static constexpr ofColor_<PixelType> darkOliveGreen{fromFloat(0.333333f),fromFloat(0.419608f),fromFloat(0.184314f)};
	static constexpr ofColor_<PixelType> darkorange{limit(),fromFloat(0.54902f),0};
	static constexpr ofColor_<PixelType> darkOrchid{fromFloat(0.6f),fromFloat(0.196078f),fromFloat(0.8f)};
	static constexpr ofColor_<PixelType> darkRed{fromFloat(0.545098f),0,0};
	static constexpr ofColor_<PixelType> darkSalmon{fromFloat(0.913725f),fromFloat(0.588235f),fromFloat(0.478431f)};
	static constexpr ofColor_<PixelType> darkSeaGreen{fromFloat(0.560784f),fromFloat(0.737255f),fromFloat(0.560784f)};
	static constexpr ofColor_<PixelType> darkSlateBlue{fromFloat(0.282353f),fromFloat(0.239216f),fromFloat(0.545098f)};
	static constexpr ofColor_<PixelType> darkSlateGray{fromFloat(0.184314f),fromFloat(0.309804f),fromFloat(0.309804f)};
	static constexpr ofColor_<PixelType> darkSlateGrey{fromFloat(0.184314f),fromFloat(0.309804f),fromFloat(0.309804f)};
	static constexpr ofColor_<PixelType> darkTurquoise{0,fromFloat(0.807843f),fromFloat(0.819608f)};
	static constexpr ofColor_<PixelType> darkViolet{fromFloat(0.580392f),0,fromFloat(0.827451f)};
	static constexpr ofColor_<PixelType> deepPink{limit(),fromFloat(0.0784314f),fromFloat(0.576471f)};
	static constexpr ofColor_<PixelType> deepSkyBlue{0,fromFloat(0.74902f),limit()};
	static constexpr ofColor_<PixelType> dimGray{fromFloat(0.411765f),fromFloat(0.411765f),fromFloat(0.411765f)};
	static constexpr ofColor_<PixelType> dimGrey{fromFloat(0.411765f),fromFloat(0.411765f),fromFloat(0.411765f)};
	static constexpr ofColor_<PixelType> dodgerBlue{fromFloat(0.117647f),fromFloat(0.564706f),limit()};
	static constexpr ofColor_<PixelType> fireBrick{fromFloat(0.698039f),fromFloat(0.133333f),fromFloat(0.133333f)};
	static constexpr ofColor_<PixelType> floralWhite{limit(),fromFloat(0.980392f),fromFloat(0.941176f)};
	static constexpr ofColor_<PixelType> forestGreen{fromFloat(0.133333f),fromFloat(0.545098f),fromFloat(0.133333f)};
	static constexpr ofColor_<PixelType> fuchsia{limit(),0,limit()};
	static constexpr ofColor_<PixelType> gainsboro{fromFloat(0.862745f),fromFloat(0.862745f),fromFloat(0.862745f)};
	static constexpr ofColor_<PixelType> ghostWhite{fromFloat(0.972549f),fromFloat(0.972549f),limit()};
	static constexpr ofColor_<PixelType> gold{limit(),fromFloat(0.843137f),0};
	static constexpr ofColor_<PixelType> goldenRod{fromFloat(0.854902f),fromFloat(0.647059f),fromFloat(0.12549f)};
	static constexpr ofColor_<PixelType> grey{fromFloat(0.501961f),fromFloat(0.501961f),fromFloat(0.501961f)};
	static constexpr ofColor_<PixelType> greenYellow{fromFloat(0.678431f),limit(),fromFloat(0.184314f)};
	static constexpr ofColor_<PixelType> honeyDew{fromFloat(0.941176f),limit(),fromFloat(0.941176f)};
	static constexpr ofColor_<PixelType> hotPink{limit(),fromFloat(0.411765f),fromFloat(0.705882f)};
	static constexpr ofColor_<PixelType> indianRed {fromFloat(0.803922f),fromFloat(0.360784f),fromFloat(0.360784f)};
	static constexpr ofColor_<PixelType> indigo {fromFloat(0.294118f),0,fromFloat(0.509804f)};
	static constexpr ofColor_<PixelType> ivory{limit(),limit(),fromFloat(0.941176f)};
	static constexpr ofColor_<PixelType> khaki{fromFloat(0.941176f),fromFloat(0.901961f),fromFloat(0.54902f)};
	static constexpr ofColor_<PixelType> lavender{fromFloat(0.901961f),fromFloat(0.901961f),fromFloat(0.980392f)};
	static constexpr ofColor_<PixelType> lavenderBlush{limit(),fromFloat(0.941176f),fromFloat(0.960784f)};
	static constexpr ofColor_<PixelType> lawnGreen{fromFloat(0.486275f),fromFloat(0.988235f),0};
	static constexpr ofColor_<PixelType> lemonChiffon{limit(),fromFloat(0.980392f),fromFloat(0.803922f)};
	static constexpr ofColor_<PixelType> lightBlue{fromFloat(0.678431f),fromFloat(0.847059f),fromFloat(0.901961f)};
	static constexpr ofColor_<PixelType> lightCoral{fromFloat(0.941176f),fromFloat(0.501961f),fromFloat(0.501961f)};
	static constexpr ofColor_<PixelType> lightCyan{fromFloat(0.878431f),limit(),limit()};
	static constexpr ofColor_<PixelType> lightGoldenRodYellow{fromFloat(0.980392f),fromFloat(0.980392f),fromFloat(0.823529f)};
	static constexpr ofColor_<PixelType> lightGray{fromFloat(0.827451f),fromFloat(0.827451f),fromFloat(0.827451f)};
	static constexpr ofColor_<PixelType> lightGrey{fromFloat(0.827451f),fromFloat(0.827451f),fromFloat(0.827451f)};
	static constexpr ofColor_<PixelType> lightGreen{fromFloat(0.564706f),fromFloat(0.933333f),fromFloat(0.564706f)};
	static constexpr ofColor_<PixelType> lightPink{limit(),fromFloat(0.713726f),fromFloat(0.756863f)};
	static constexpr ofColor_<PixelType> lightSalmon{limit(),fromFloat(0.627451f),fromFloat(0.478431f)};
	static constexpr ofColor_<PixelType> lightSeaGreen{fromFloat(0.12549f),fromFloat(0.698039f),fromFloat(0.666667f)};
	static constexpr ofColor_<PixelType> lightSkyBlue{fromFloat(0.529412f),fromFloat(0.807843f),fromFloat(0.980392f)};
	static constexpr ofColor_<PixelType> lightSlateGray{fromFloat(0.466667f),fromFloat(0.533333f),fromFloat(0.6f)};
	static constexpr ofColor_<PixelType> lightSlateGrey{fromFloat(0.466667f),fromFloat(0.533333f),fromFloat(0.6f)};
	static constexpr ofColor_<PixelType> lightSteelBlue{fromFloat(0.690196f),fromFloat(0.768627f),fromFloat(0.870588f)};
	static constexpr ofColor_<PixelType> lightYellow{limit(),limit(),fromFloat(0.878431f)};
	static constexpr ofColor_<PixelType> lime{0,limit(),0};
	static constexpr ofColor_<PixelType> limeGreen{fromFloat(0.196078f),fromFloat(0.803922f),fromFloat(0.196078f)};
	static constexpr ofColor_<PixelType> linen{fromFloat(0.980392f),fromFloat(0.941176f),fromFloat(0.901961f)};
	static constexpr ofColor_<PixelType> maroon{fromFloat(0.501961f),0,0};
	static constexpr ofColor_<PixelType> mediumAquaMarine{fromFloat(0.4f),fromFloat(0.803922f),fromFloat(0.666667f)};
	static constexpr ofColor_<PixelType> mediumBlue{0,0,fromFloat(0.803922f)};
	static constexpr ofColor_<PixelType> mediumOrchid{fromFloat(0.729412f),fromFloat(0.333333f),fromFloat(0.827451f)};
	static constexpr ofColor_<PixelType> mediumPurple{fromFloat(0.576471f),fromFloat(0.439216f),fromFloat(0.858824f)};
	static constexpr ofColor_<PixelType> mediumSeaGreen{fromFloat(0.235294f),fromFloat(0.701961f),fromFloat(0.443137f)};
	static constexpr ofColor_<PixelType> mediumSlateBlue{fromFloat(0.482353f),fromFloat(0.407843f),fromFloat(0.933333f)};
	static constexpr ofColor_<PixelType> mediumSpringGreen{0,fromFloat(0.980392f),fromFloat(0.603922f)};
	static constexpr ofColor_<PixelType> mediumTurquoise{fromFloat(0.282353f),fromFloat(0.819608f),fromFloat(0.8f)};
	static constexpr ofColor_<PixelType> mediumVioletRed{fromFloat(0.780392f),fromFloat(0.0823529f),fromFloat(0.521569f)};
	static constexpr ofColor_<PixelType> midnightBlue{fromFloat(0.0980392f),fromFloat(0.0980392f),fromFloat(0.439216f)};
	static constexpr ofColor_<PixelType> mintCream{fromFloat(0.960784f),limit(),fromFloat(0.980392f)};
	static constexpr ofColor_<PixelType> mistyRose{limit(),fromFloat(0.894118f),fromFloat(0.882353f)};
	static constexpr ofColor_<PixelType> moccasin{limit(),fromFloat(0.894118f),fromFloat(0.709804f)};
	static constexpr ofColor_<PixelType> navajoWhite{limit(),fromFloat(0.870588f),fromFloat(0.678431f)};
	static constexpr ofColor_<PixelType> navy{0,0,fromFloat(0.501961f)};
	static constexpr ofColor_<PixelType> oldLace{fromFloat(0.992157f),fromFloat(0.960784f),fromFloat(0.901961f)};
	static constexpr ofColor_<PixelType> olive{fromFloat(0.501961f),fromFloat(0.501961f),0};
	static constexpr ofColor_<PixelType> oliveDrab{fromFloat(0.419608f),fromFloat(0.556863f),fromFloat(0.137255f)};
	static constexpr ofColor_<PixelType> orange{limit(),fromFloat(0.647059f),0};
	static constexpr ofColor_<PixelType> orangeRed{limit(),fromFloat(0.270588f),0};
	static constexpr ofColor_<PixelType> orchid{fromFloat(0.854902f),fromFloat(0.439216f),fromFloat(0.839216f)};
	static constexpr ofColor_<PixelType> paleGoldenRod{fromFloat(0.933333f),fromFloat(0.909804f),fromFloat(0.666667f)};
	static constexpr ofColor_<PixelType> paleGreen{fromFloat(0.596078f),fromFloat(0.984314f),fromFloat(0.596078f)};
	static constexpr ofColor_<PixelType> paleTurquoise{fromFloat(0.686275f),fromFloat(0.933333f),fromFloat(0.933333f)};
	static constexpr ofColor_<PixelType> paleVioletRed{fromFloat(0.858824f),fromFloat(0.439216f),fromFloat(0.576471f)};
	static constexpr ofColor_<PixelType> papayaWhip{limit(),fromFloat(0.937255f),fromFloat(0.835294f)};
	static constexpr ofColor_<PixelType> peachPuff{limit(),fromFloat(0.854902f),fromFloat(0.72549f)};
	static constexpr ofColor_<PixelType> peru{fromFloat(0.803922f),fromFloat(0.521569f),fromFloat(0.247059f)};
	static constexpr ofColor_<PixelType> pink{limit(),fromFloat(0.752941f),fromFloat(0.796078f)};
	static constexpr ofColor_<PixelType> plum{fromFloat(0.866667f),fromFloat(0.627451f),fromFloat(0.866667f)};
	static constexpr ofColor_<PixelType> powderBlue{fromFloat(0.690196f),fromFloat(0.878431f),fromFloat(0.901961f)};
	static constexpr ofColor_<PixelType> purple{fromFloat(0.501961f),0,fromFloat(0.501961f)};
	static constexpr ofColor_<PixelType> rosyBrown{fromFloat(0.737255f),fromFloat(0.560784f),fromFloat(0.560784f)};
	static constexpr ofColor_<PixelType> royalBlue{fromFloat(0.254902f),fromFloat(0.411765f),fromFloat(0.882353f)};
	static constexpr ofColor_<PixelType> saddleBrown{fromFloat(0.545098f),fromFloat(0.270588f),fromFloat(0.0745098f)};
	static constexpr ofColor_<PixelType> salmon{fromFloat(0.980392f),fromFloat(0.501961f),fromFloat(0.447059f)};
	static constexpr ofColor_<PixelType> sandyBrown{fromFloat(0.956863f),fromFloat(0.643137f),fromFloat(0.376471f)};
	static constexpr ofColor_<PixelType> seaGreen{fromFloat(0.180392f),fromFloat(0.545098f),fromFloat(0.341176f)};
	static constexpr ofColor_<PixelType> seaShell{limit(),fromFloat(0.960784f),fromFloat(0.933333f)};
	static constexpr ofColor_<PixelType> sienna{fromFloat(0.627451f),fromFloat(0.321569f),fromFloat(0.176471f)};
	static constexpr ofColor_<PixelType> silver{fromFloat(0.752941f),fromFloat(0.752941f),fromFloat(0.752941f)};
	static constexpr ofColor_<PixelType> skyBlue{fromFloat(0.529412f),fromFloat(0.807843f),fromFloat(0.921569f)};
	static constexpr ofColor_<PixelType> slateBlue{fromFloat(0.415686f),fromFloat(0.352941f),fromFloat(0.803922f)};
	static constexpr ofColor_<PixelType> slateGray{fromFloat(0.439216f),fromFloat(0.501961f),fromFloat(0.564706f)};
	static constexpr ofColor_<PixelType> slateGrey{fromFloat(0.439216f),fromFloat(0.501961f),fromFloat(0.564706f)};
	static constexpr ofColor_<PixelType> snow{limit(),fromFloat(0.980392f),fromFloat(0.980392f)};
	static constexpr ofColor_<PixelType> springGreen{0,limit(),fromFloat(0.498039f)};
	static constexpr ofColor_<PixelType> steelBlue{fromFloat(0.27451f),fromFloat(0.509804f),fromFloat(0.705882f)};
	static constexpr ofColor_<PixelType> blueSteel{fromFloat(0.27451f),fromFloat(0.509804f),fromFloat(0.705882f)};
	static constexpr ofColor_<PixelType> tan{fromFloat(0.823529f),fromFloat(0.705882f),fromFloat(0.54902f)};
	static constexpr ofColor_<PixelType> teal{0,fromFloat(0.501961f),fromFloat(0.501961f)};
	static constexpr ofColor_<PixelType> thistle{fromFloat(0.847059f),fromFloat(0.74902f),fromFloat(0.847059f)};
	static constexpr ofColor_<PixelType> tomato{limit(),fromFloat(0.388235f),fromFloat(0.278431f)};
	static constexpr ofColor_<PixelType> turquoise{fromFloat(0.25098f),fromFloat(0.878431f),fromFloat(0.815686f)};
	static constexpr ofColor_<PixelType> violet{fromFloat(0.933333f),fromFloat(0.509804f),fromFloat(0.933333f)};
	static constexpr ofColor_<PixelType> wheat{fromFloat(0.960784f),fromFloat(0.870588f),fromFloat(0.701961f)};
	static constexpr ofColor_<PixelType> whiteSmoke{fromFloat(0.960784f),fromFloat(0.960784f),fromFloat(0.960784f)};
	static constexpr ofColor_<PixelType> yellowGreen{fromFloat(0.603922f),fromFloat(0.803922f),fromFloat(0.196078f)};
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

template <typename PixelType>
ofColor_<PixelType> operator*(float val, const ofColor_<PixelType> &color) {
	return color * val;
}

template<typename PixelType>
constexpr float ofColor_<PixelType>::flimit() {
	return std::numeric_limits<PixelType>::max();
}

template<>
constexpr float ofColor_<float>::flimit() {
	return 1.f;
}

template<>
constexpr float ofColor_<double>::flimit() {
	return 1.;
}

template<typename PixelType>
constexpr PixelType ofColor_<PixelType>::limit() {
	return std::numeric_limits<PixelType>::max();
}

template<>
constexpr float ofColor_<float>::limit() {
	return 1.f;
}

template<>
constexpr double ofColor_<double>::limit() {
	return 1.;
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
inline void ofColor_<unsigned char>::setHex(int hexColor, unsigned char alpha){
	r = (hexColor >> 16) & 0xff;
	g = (hexColor >> 8) & 0xff;
	b = (hexColor >> 0) & 0xff;
	a = alpha;
}

template<typename PixelType>
inline void ofColor_<PixelType>::setHex (int hexColor, PixelType alpha){
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
