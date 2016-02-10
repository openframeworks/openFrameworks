#pragma once


#include <limits>
#include "ofMath.h"

/// \class ofColor_
/// ofColor represents a color in openFrameworks. Colors are usually defined by
/// specifying a red, green, and blue component (RGB), and a transparency
/// (alpha) component. You can also specify colors using hue, saturation, and
/// brightness (HSB).
///
/// For example:
/// ~~~~{.cpp}
///     // Set red, component by component
///     ofColor red;
///     red.r=255;
///     red.g=0;
///     red.b=0;
///     ofSetColor(red);
///     // Draw color is now red.
///
///     // Shorter notation is also possible.
///     ofColor green(0, 255, 0);
///     ofSetColor(green);
///     // Draw color is now green.
///
///     // ... or even shorter.
///     ofSetColor( ofColor(0, 0, ofRandom( 128, 255 ) );
///     // Draw color is now a random blue.
/// ~~~~
///
/// ofColor also enables a lot of extra functionality like using HSB instead of
/// color spectrums, lerping or linearly interpolating between colors, and
/// inverting colors, among other things.
///
/// ofColor is templated, which means that it has several different ways it can
/// be created. These are probably best to leave as they are because there's
/// already a few kinds typedefed for you. The default ofColor uses unsigned
/// char values (0 to 255), but you can make an ofFloatColor if you want to work
/// with floating point numbers between 0 and 1, or ofShortColor if you want to
/// work with integers between 0 and 65,535.
///
/// |   Typedef     |    PixelType     | Bit Depth | Min. Value | Max. Value  |
/// | ------------- | ---------------- | --------- | ---------- | ----------- |
/// | `ofColor`     | `unsigned char`  | 8         | 0          | 255         |
/// | `ofShortColor`| `unsigned short` | 16        | 0          | 65535       |
/// | `ofFloatColor`| `float`          | _varies_  | 0.0        | 1.0         |
///
/// #### HSB
///
/// You're probably familiar with RGB colors already, but HSB is a big part of
/// ofColor. It uses a *hue* value (for the standard ofColor the range for this
/// value is between 0 and 255) to determine what the hue (the 'color' in the
/// sense of a color in the rainbow) will be:
///
/// ![HSB](types/hsb.jpg)
///
/// Approximate hues for some common colors:
///
/// * Red: 0 (wrapped round from 255)
/// * Orange: 25
/// * Yellow: 42
/// * Green: 85
/// * Blue: 170
/// * Purple: 205
/// * Red: 255 (wraps round to 0)
///
/// Once you've selected a hue, you can use the *saturation* and *brightness*
/// values to further refine the color. The saturation determines how much of
/// the hue versus white is present and brightness determines how much hue
/// versus black is present:
///
/// ![SB](types/hsb-cone.jpg)
///
/// In other words, saturation refers to the intensity of the color: high
/// saturation means intense color, low saturation means washed out or black and
/// white. Brightness refers to how light or dark the color is: high brightness
/// means a bright color, low brightness means a dark color. If the brightness
/// is 0 the resulting color will be black, regardless of the values of hue or
/// saturation.
///
/// \tparam PixelType The data type used to represent a single pixel value.
template<typename PixelType>
class ofColor_{
public:
    /// \name Create an ofColor
    /// \{

    /// \brief Construct a default white color.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c;
    ///     ofSetColor(c); // Draw color is now white.
    /// ~~~~
    ofColor_():
        r(limit()),
        g(limit()),
        b(limit()),
        a(limit()) {};

    /// \brief Construct an ofColor_ by using channel values.
    ///
    /// When creating an instance of ofColor_ the channel values must fall
    /// within the range represented by the PixelType.  By default, the alpha
    /// component will take the PixelType's maximum, producing an opaque color.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c1(255, 0, 0); // Red with 100% alpha.
    ///     ofColor c2(255, 0, 0, 128); // Red with 50% alpha.
    /// ~~~~
    /// \param red The red component.
    /// \param green The green component.
    /// \param blue The blue component.
    /// \param alpha The alpha component.
    ofColor_(float red, float green, float blue, float alpha = limit());

    /// \brief Construct a grayscale ofColor_ by specifying a single number.
    ///
    /// This constructor will use a single value to describe a grayscale color.
    /// This single value will be taken by each of the R, G and B components.
    /// The alpha channel is specified separately and will and will default to
    /// the PixelType's maximum resulting in an opaque color.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c(0.5); // c is now gray.
    /// ~~~~
    /// \param gray A grayscale value.
    /// \param alpha The opacity of the color.
    ofColor_(float gray, float alpha = limit());

    /// \brief Construct an ofColor_ from an existing ofColor_.
    ///
    /// This constructor will attempt to convert between color instances of
    /// differing precision.
    ///
    /// ~~~~{.cpp}
    ///     ofColor colorA(255, 0, 0);
    ///     ofColor colorB(colorA); // colorB is now equal to colorA.
    /// ~~~~
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
    /// This constructor will use the R, G, and B components from the passed
    /// color ignoring its alpha channel.  The provided alpha channel will be
    /// used instead.
    ///
    /// \param color The ofColor_ to copy.
    /// \param alpha The new alpha value to replace the alpha value in color.
    ofColor_(const ofColor_<PixelType>& color, float alpha);

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
    /// ~~~~{.cpp}
    ///     ofColor c = ofColor::fromHsb(128, 255, 255);
    ///     // c is bright saturated cyan.
    /// ~~~~
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
    /// number. In this case, red, green, and blue values are packed into a
    /// single integer.
    ///
    /// The alpha channel is specified separately and will default to the
    /// PixelType's maximum, resulting in an opaque color.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c = ofColor::fromHex(0xFFFF00); // c is yellow.
    /// ~~~~
    ///
    /// \param hexColor A color in hexadecimal form.
    /// \param alpha The alpha value of the color.
    static ofColor_<PixelType> fromHex(int hexColor, float alpha = limit());

    /// \}

    /// \name Data Accessors
    /// \{

    /// \brief A union representing the internal data
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

    /// \}

    /// \name Setters
    /// \{

    /// \brief Set an ofColor_ by using channel values.
    ///
    /// When modifying an instance of ofColor_ the channel values must fall
    /// within the range represented by the PixelType.  By default, the alpha
    /// component will take the PixelType's maximum, producing an opaque color.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c(255, 0, 0); // Red ...
    ///     c.set(0, 255, 0); // ... and now green.
    /// ~~~~
    ///
    /// \param red The red component.
    /// \param green The green component.
    /// \param blue The blue component.
    /// \param alpha The alpha component.
    void set(float red, float green, float blue, float alpha = limit());

    /// \brief Set a grayscale ofColor_ by specifying a single number.
    ///
    /// When modifying an instance of ofColor_ you can use a single value to
    /// describe a grayscale color. This single value will be taken by each of
    /// the R, G and B components. The alpha channel is specified separately
    /// and will and will default to the PixelType's maximum resulting in an
    /// opaque color.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c(255, 0, 0); // Red ...
    ///     c.set(128, 128); // ... and now 50% gray with 50% alpha.
    /// ~~~~
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
    /// In some cases, it is convenient to represent colors using a hexadecimal
    /// number.  In this case, red, green and blue values are packed into a
    /// integer.
    ///
    /// The alpha channel is specified separately and will default to the
    /// PixelType's maximum, resulting in an opaque color.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c;
    ///     c.setHex(0xFFFFFF); // White.
    ///     c.setHex(0x00FF00); // Green.
    ///     c.setHex(0xFF8000, 128); // Orange, 50% alpha.
    /// ~~~~
    ///
    /// \warning The alpha value _should not_ be packed in the hexColor and
    ///     must be specified separately.
    ///
    /// \param hexColor An RGB color in hexadecimal form.
    /// \param alpha The alpha value of the color.
    void setHex(int hexColor, float alpha = limit());

    /// \brief Set the hue of this color.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c = ofColor::fromHsb(0, 255, 255); // Bright red ...
    ///     c.setHue(128); // ... now bright cyan.
    /// ~~~~
    ///
    /// \param hue A hue value to set in the range of 0 - limit().
    void setHue(float hue);

    /// \brief Set the hue angle of this color.
    /// \param angle A hue angle value to set in the range of 0 - 360 degrees.
    void setHueAngle(float angle);

    /// \brief Set the saturation of this color.
    ///
    /// This method uses HSB not HSL. So red desaturated is white, not gray
    ///
    /// ~~~~{.cpp}
    ///     ofColor c(0, 0, 255); // Vibrant blue ...
    ///     c.setSaturation( 128 ); // ... pale blue.
    /// ~~~~
    ///
    /// \param saturation A saturation value value in the range of 0 - limit().
    /// \sa http://en.wikipedia.org/wiki/HSL_and_HSV
    void setSaturation(float saturation);

    /// \brief Set the brightness of this color.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c(0, 0, 255); // Bright blue ...
    ///     c.setBrightness(128); // ... dark blue.
    /// ~~~~
    ///
    /// \param brightness A brightness value to set in the range of 0 - limit().
    void setBrightness(float brightness);

    /// \brief Set the color using HSB components.
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
    /// ~~~~{.cpp}
    ///     ofColor c( 300, 0, 0 );
    ///     // r component is above the limit() of 255.
    ///     c.clamp(); // c.r has now been clamped so its value is 255.
    /// ~~~~
    ///
    /// \returns A reference to itself.
    ofColor_<PixelType>& clamp();

    /// \brief Invert the R, G and B components.
    ///
    /// Performs an inversion operation on the color by replacing the red, green
    /// and blue components with their original value subtracted from the
    /// limit().  For example, an ofColor value of:
    ///
    /// ~~~~~{.cpp}
    ///     unsigned char maxValue = limit(); // would return 255
    ///
    ///     unsigned char r = 255;
    ///     unsigned char g = 255;
    ///     unsigned char b = 0;
    ///
    ///     unsigned char rNew = maxValue - r; // 255 - 255 = 0
    ///     unsigned char gNew = maxValue - g; // 255 - 255 = 0
    ///     unsigned char bNew = maxValue - b; // 255 - 0   = 255;
    /// ~~~~~
    ///
    /// Example usage:
    /// ~~~~{.cpp}
    ///     ofColor c(0, 0, 255); // Blue.
    ///     c.invert();
    ///     // c is now (255,255,0), or yellow, which is blue's complement.
    /// ~~~~
    ///
    /// \returns A reference to itself.
    ofColor_<PixelType>& invert();

    /// \brief Normalize the R, G and B components.
    ///
    /// Performs a normalization operation on the red, green and blue components
    /// by scaling them by brightness of the original color divided by the
    /// limit().  The brightness is calculated by finding the maximum of
    /// original red, green and blue components.  This operation is equivalent
    /// to the following:
    ///
    /// ~~~~~{.cpp}
    ///     ofColor myColor(127, 0, 0, 127);
    ///
    ///     float brightness = myColor.getBrightness(); // Returns 127.
    ///     float scaleFactor = brightness / limit();   // Returns 127 / 255.
    ///
    ///     ofColor myNormalizedColor = myColor / scaleFactor;
    ///                                 // Divides the red, green and blue
    ///                                 // components by the scale factor.
    /// ~~~~~
    ///
    /// \returns A reference to itself.
    ofColor_<PixelType>& normalize();

    /// \brief A linear interpolation between all components of two colors.
    ///
    /// The lerp method performs a linear interpolation (or [lerp][1]) between
    /// this color and a target color.  In contrast to many of the mathematical
    /// functions found in ofColor_, The linear interpolation is carried out
    /// on _all_ components, R, G, B _and_ A.  The amount is typically a value
    /// between 0.0 and 1.0, with 0.0 yielding an unchanged color and 1.0
    /// yielding a color identical to the target color.
    ///
    /// ~~~~{.cpp}
    ///     ofColor r = ofColor::red;
    ///     ofColor b = ofColor::blue;
    ///     b.lerp(r, 0.5); // now purple
    /// ~~~~
    ///
    /// [1]: http://en.wikipedia.org/wiki/Lerp_(computing) "Lerp"
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
    /// \sa clamp()
    /// \returns A copy of this color, clamped.
    ofColor_<PixelType> getClamped() const;

    /// \brief A non-destructive version of invert().
    /// \sa invert()
    /// \returns A copy of this color, inverted.
    ofColor_<PixelType> getInverted() const;

    /// \brief A non-destructive version of normalize().
    /// \sa normalize()
    /// \returns A copy of this color, normalized.
    ofColor_<PixelType> getNormalized() const;


    /// \brief A non-destructive version of lerp().
    /// \sa lerp()
    /// \returns A copy of this color, lerped.
    ofColor_<PixelType> getLerped(const ofColor_<PixelType>& target,
                                  float amount) const;

    /// \brief Get a 24-bit hexadecimal representation of the RGB color.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c( 255, 255, 0 ); // Yellow.
    ///     int hex = c.getHex(); // Hex is 0xffff00 (or 16776960 in decimal).
    /// ~~~~
    ///
    /// \warning This method does _not_ encode the alpha component.
    ///
    /// \returns An integer representing an RGB color.
    int getHex() const;

    /// \brief Get the Hue of this color.
    ///
    /// The color is converted from the default RGB to an HSB color space and
    /// the resulting Hue value is returned.  The resulting hue value will
    /// always be returned in the range 0 - limit().
    ///
    /// \returns The Hue value in the range 0 - limit().
    float getHue() const;

    /// \brief Get the Hue angle of this color.
    ///
    /// The color is converted from the default RGB to an HSB color space and
    /// the resulting Hue angle is returned.  The resulting hue value will
    /// always be returned in degrees in the range 0 - 360.
    ///
    /// \returns The Hue angle in the range 0 - 360.
    float getHueAngle() const;

    /// \brief Get the Saturation of this color.
    ///
    /// The color is converted from the default RGB to an HSB color space and
    /// the resulting saturation is returned.  The resulting saturation value
    /// will always be returned in the range 0 - limit().
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
    /// ~~~~{.cpp}
    ///     ofColor red(255,0,0);
    ///     float hue = 0;  // The hue value to set.
    ///     float saturation = 0; // The saturation value to set.
    ///     float brightness = 0; /// The brightness value to set.
    ///     red.getHsb(hue, saturation, brightness);
    ///     // Hue is now 0 (for red), saturation is 255, brightness is 255.
    /// ~~~~
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
    /// Based on the type of PixelType (whether its a ofColor, ofFloatColor or
    /// ofShortColor), the maximum value different. For a ofFloatColor the
    /// maximum is `1.0`, but for a ofColor it's `255`.
    ///
    /// Use this function to get this maximum number.
    ///
    /// \returns The value associated with a fully saturated color component.
    static float limit();

    /// \}

    /// \name Operators
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
    /// R, G and B components are set to the grayscale value and alpha is
    /// set to limit().
    ///
    /// ~~~~~{.cpp}
    ///     ofColor myColor = 127;
    /// ~~~~~
    ///
    /// is equivalent to:
    ///
    /// ~~~~~{.cpp}
    ///     ofColor myColor(127, 127, 127, 255);
    /// ~~~~~
    ///
    /// \param value The grayscale value.
    /// \returns A reference to itself.
    ofColor_<PixelType>& operator = (const float& value);

    /// \brief Test two colors for equality.
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
    ofColor_<PixelType>  operator +  (const float& color) const;

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
    ofColor_<PixelType>& operator += (const float& color);

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
    ofColor_<PixelType>  operator -  (const float& value) const;

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
    ofColor_<PixelType>& operator -= (const float& color);

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
    ofColor_<PixelType>  operator *  (const float& value) const;

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
    ofColor_<PixelType>& operator *= (const float& value);

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
    ofColor_<PixelType>  operator /  (const float& value) const;

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
    ofColor_<PixelType>& operator /= (const float& value);

    /// \brief Array subscript operator.
    ///
    /// If n is 0 returns .r, if 1 returns .g, if 2 returns .b, if 3 returns
    /// alpha.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c(128, 64, 255);
    ///     float red = c[0]; // Red is 128.
    /// ~~~~
    ///
    /// \param n An index 0-3 of the component to get.
    /// \returns The value of the requested component.
    const PixelType& operator [] (std::size_t n) const;

    /// \brief Array subscript operator.
    ///
    /// If n is 0 returns .r, if 1 returns .g, if 2 returns .b, if 3 returns
    /// alpha.
    ///
    /// ~~~~{.cpp}
    ///     ofColor c(128, 64, 255);
    ///     float red = c[0]; // Red is 128.
    /// ~~~~
    ///
    /// \param n An index 0-3 of the component to get.
    /// \returns The value of the requested component.
	PixelType& operator [] (std::size_t n);

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
    /// ~~~~{.cpp}
    ///     std::stringstream ss;     // Create an empty std::string stream.
    ///                               // Note: std::stringstream is just one
    ///                               // option for an output stream.  Another
    ///                               // popular output stream std::cout.
    ///
    ///     ofColor myColor(255, 255, 0, 255);
    ///                               // Create an ofColor to output.
    ///
    ///     ss << myColor;            // Write the color to the output stream.
    /// ~~~~
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
    /// ~~~~{.cpp}
    ///     std::stringstream ss;     // Create an empty std::string stream.
    ///                               // Note: std::stringstream is just one
    ///                               // option for an input stream.
    ///     ss << "255, 255, 0, 255"; // Write the color to the stringstream.
    ///     ofColor myColor;          // Create an ofColor instance to fill.
    ///     ss >> myColor;            // myColor is now filled with a yellow.
    /// ~~~~
    ///
    /// \param is An input stream reference.
    /// \param color The color to fill with the input stream.
    /// \returns The passed input stream reference, useful for method chaining.
    friend istream& operator >> (istream& is, ofColor_<PixelType>& color) {
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

    /// \name Predefined Colors
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
