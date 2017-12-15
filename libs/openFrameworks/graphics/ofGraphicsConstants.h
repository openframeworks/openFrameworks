#pragma once

#include "ofMathConstants.h"

template<typename T>
class ofColor_;
typedef ofColor_<float> ofFloatColor;
using ofDefaultVertexType = ofDefaultVec3;
using ofDefaultNormalType = ofDefaultVec3;
using ofDefaultColorType = ofFloatColor;
using ofDefaultTexCoordType = ofDefaultVec2;

enum ofPolyRenderMode{
	OF_MESH_POINTS,
	OF_MESH_WIREFRAME,
	OF_MESH_FILL
};


enum ofPrimitiveMode{
	OF_PRIMITIVE_TRIANGLES,
	OF_PRIMITIVE_TRIANGLE_STRIP,
	OF_PRIMITIVE_TRIANGLE_FAN,
	OF_PRIMITIVE_LINES,
	OF_PRIMITIVE_LINE_STRIP,
	OF_PRIMITIVE_LINE_LOOP,
	OF_PRIMITIVE_POINTS,
#ifndef TARGET_OPENGLES
	OF_PRIMITIVE_LINES_ADJACENCY,
	OF_PRIMITIVE_LINE_STRIP_ADJACENCY,
	OF_PRIMITIVE_TRIANGLES_ADJACENCY,
	OF_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY,
	OF_PRIMITIVE_PATCHES
#endif
};


/// \brief Used to represent the available fill modes.
///
/// \sa ofBaseRenderer
enum ofFillFlag{
	/// \brief Draw shapes as outlines, unfilled.
	OF_OUTLINE=	0,
	/// \brief Draw shapes filled with the current draw color.
	OF_FILLED = 1,
};

#define		OF_MAX_STYLE_HISTORY	32

/// \deprecated Not currently used in the OF codebase.
#define		OF_MAX_VIEWPORT_HISTORY	32

/// \deprecated Not currently used in the OF codebase.
#define		OF_MAX_CIRCLE_PTS 1024

/// \brief Used to represent the available blending modes for drawing.
enum ofBlendMode{
	/// \brief Blend mode is disabled.
	OF_BLENDMODE_DISABLED = 0,
	/// \brief Blend mode used for alpha blending.
	OF_BLENDMODE_ALPHA 	  = 1,
	/// \brief Blend mode used for additive blending.
	OF_BLENDMODE_ADD 	  = 2,
	/// \brief Blend mode used for subtractive blending.
	OF_BLENDMODE_SUBTRACT = 3,
	/// \brief Blend mode used for multiplicative blending.
	OF_BLENDMODE_MULTIPLY = 4,
	/// \brief Blend mode used for screen blending.
	OF_BLENDMODE_SCREEN   = 5
};


/// \brief Represents the gradient types available to ofBackgroundGradient().
enum ofGradientMode {
	/// \brief Represents a top-to-bottom linear gradient.
	OF_GRADIENT_LINEAR = 0,
	/// \brief Represents a circular gradient beginning at the screen's center.
	OF_GRADIENT_CIRCULAR,
	/// \brief Represents a horizontal bar gradient.
	///
	///  This is a horizontal gradient starting across the screen's center,
	///  and extending both to the top and bottom of the screen.
	OF_GRADIENT_BAR
};

/// \brief represents the available polygon winding modes.
///
/// These are straight out of glu, but renamed and included here
/// for convenience.  We don't mean to wrap the whole glu library
/// (or any other library for that matter), but these defines are useful
/// to give people flexibility over the polygonizer.
///
/// \sa ofPath::tessellate()
/// \sa ofTessellator::performTessellation()
/// \sa http://glprogramming.com/red/images/Image128.gif
/// \sa http://glprogramming.com/red/chapter11.html
enum ofPolyWindingMode{
	/// \brief Fill odd winding numbers.
	OF_POLY_WINDING_ODD 	        ,
	/// \brief Fill all non-zero winding numbers.
	OF_POLY_WINDING_NONZERO         ,
	/// \brief Fill all winding numbers greater than zero.
	OF_POLY_WINDING_POSITIVE        ,
	/// \brief Fill all winding numbers less than zero.
	OF_POLY_WINDING_NEGATIVE        ,
	/// \brief Fill all winding numbers greater than 1 or less than -1.
	///
	/// This stands for "Fill ABSolute values Greater than EQual to TWO".
	OF_POLY_WINDING_ABS_GEQ_TWO
};

/// \brief represents the available matrix coordinate system handednesses.
///
/// \sa ofMatrixStack
/// \sa http://seanmiddleditch.com/matrices-handedness-pre-and-post-multiplication-row-vs-column-major-and-notations/
enum ofHandednessType {OF_LEFT_HANDED, OF_RIGHT_HANDED};

/// \brief represents the available matrix types used internally in ::ofMatrixStack.
enum ofMatrixMode {OF_MATRIX_MODELVIEW=0, OF_MATRIX_PROJECTION, OF_MATRIX_TEXTURE};



/// \brief Sets the bitmap drawing mode for text.
/// \sa ofSetDrawBitmapMode()
enum ofDrawBitmapMode{
	OF_BITMAPMODE_SIMPLE = 0,
	OF_BITMAPMODE_SCREEN,
	OF_BITMAPMODE_VIEWPORT,
	OF_BITMAPMODE_MODEL,
	OF_BITMAPMODE_MODEL_BILLBOARD
};


/// \brief Used to represent the available rectangle drawing modes.
///
/// \sa ofRectangle
/// \sa ofTexture
/// \sa ofImage
enum ofRectMode{
	/// \brief Represents the mode where rectangles draw from the top left.
	OF_RECTMODE_CORNER=0,
	/// \brief Represents the mode where rectangles draw from the center.
	OF_RECTMODE_CENTER=1
};


/// \brief Used to represent the available channel types in ofImage.
///
/// These represent an abstraction of both CPU pixels (ofPixels) and GPU pixels
/// (ofTexture). In most cases, developers should prefer ::ofPixelFormat over
/// ::ofImageType for a more precise description of channel types.
///
/// \sa ofImage
enum ofImageType: short{
	/// \brief A single channel (or monochrome) image.
	///
	/// \sa OF_PIXELS_GRAY
	OF_IMAGE_GRAYSCALE		= 0x00,
	/// \brief A three channel (or RGB) image.
	///
	/// \sa OF_PIXELS_RGB
	OF_IMAGE_COLOR			= 0x01,
	/// \brief A four channel (or RGBA) image.
	///
	/// \sa OF_PIXELS_RGBA
	OF_IMAGE_COLOR_ALPHA	= 0x02,
	/// \brief An unknown and unsupported image type.
	///
	/// \sa OF_PIXELS_UNKNOWN
	OF_IMAGE_UNDEFINED		= 0x03
};
