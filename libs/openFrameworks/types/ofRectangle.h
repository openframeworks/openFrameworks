#pragma once


#include "ofConstants.h"
#include "ofPoint.h"
#include "ofLog.h"


/// \brief A class representing a 2D rectangle.
///
/// ofRectangle is a simple container for describing the position and size of a
/// 2D rectangle. Since the `width` and `height` variables can be negative, the
/// x/y-position is not guaranteed to represent the upper right hand corner. For
/// example, two visually identical rectangles can be represented in the
/// following ways:
///
/// ~~~~{.cpp}
///     ofRectangle myRect(0, 0, 100, 100);
/// ~~~~
///
/// or
///
/// ~~~~{.cpp}
///     ofRectangle myRect(100, 100, -100, -100);
/// ~~~~
///
/// While both representations will yield the same visual results in all
/// openFrameworks renderers, the results of some method operations that modify
/// x, y, width and height (such as scaling) produce mathematically correct, but
/// visually different results for each of the above representations.
///
/// To avoid this ambiguity, users should should prefer "standardized"
/// rectangles. "Standardized" rectangles are rectangles whose width >= 0 and
/// height >= 0. The ofRectangle::standardize() method can be used to ensure
/// that the is in it the "standard" form.
///
/// \warning While ofRectangle takes ofPoint (a typedef for ofVec3f, a 3D
/// vector), all ofRectangle operations are 2D only, ignoring the z-component.
class ofRectangle{
public:

    /// \name Constructor
    /// \{

    /// \brief Construct a rectangle with zero width and zero height at 0, 0.
    ofRectangle();

    /// \brief Construct a rectangle using position and size.
    ///
    /// To produce consistent results, users are encouraged to initialize
    /// rectangles in the standardized form with width >=0 and height >= 0.
    ///
    /// \param px The x-position of the rectangle.
    /// \param py The y-position of the rectangle.
    /// \param w The width of the rectangle.
    /// \param h The height of the rectangle.
    ofRectangle(float px, float py, float w, float h);

    /// \brief Construct a rectangle from a point and dimensions.
    ///
    /// To produce consistent results, users are encouraged to initialize
    /// rectangles in the standardized form with width >=0 and height >= 0.
    ///
    /// \warning The z-component of the passed ofPoint is ignored.
    ///
    /// \param p The ofPoint representing the position of the rectangle.
    /// \param w The width of the rectangle.
    /// \param h The height of the rectangle.
    ofRectangle(const ofPoint& p, float w, float h);

    /// \brief Construct a rectangle by copying another rectangle.
    /// \param rect The rectangle to copy.
    ofRectangle(const ofRectangle& rect);

    /// \brief Construct a rectangle by defining two corners.
    ///
    /// \warning The z-components of the passed ofPoints are ignored.
    ///
    /// \param p0 An ofPoint representing the upper left hand corner.
    /// \param p1 An ofPoint representing the lower right hand corner.
    ofRectangle(const ofPoint& p0, const ofPoint& p1);

    /// \brief Destroy the rectangle.
    virtual ~ofRectangle();

    /// \}

    /// \name Setters
    /// \{

    /// \brief Set the position and size of the rectangle.
    ///
    /// To produce consistent results, users are encouraged to initialize
    /// rectangles in the standardized form with width >=0 and height >= 0.
    ///
    /// \param px The new x-position.
    /// \param py The new y-position.
    /// \param w The new width.
    /// \param h The new height.
    void set(float px, float py, float w, float h);

    /// \brief Set the position and size of the rectangle.
    ///
    /// To produce consistent results, users are encouraged to initialize
    /// rectangles in the standardized form with width >=0 and height >= 0.
    ///
    /// \warning The z-component of the passed ofPoint is ignored.
    ///
    /// \param p The new position.
    /// \param w The new width.
    /// \param h The new height.
    void set(const ofPoint& p, float w, float h);

    /// \brief Set the position and size by copying them from anohter rectangle.
    /// \param rect The rectangle to copy.
    void set(const ofRectangle& rect);

    /// \brief Set the position and size of the rectangle using corners.
    ///
    /// \warning The z-components of the passed ofPoints are ignored.
    ///
    /// \param p0 The ofPoint representing the upper left hand corner.
    /// \param p1 The ofPoint representing the lower right hand corner.
    void set(const ofPoint& p0, const ofPoint& p1);

    /// \brief Set the x-position of the rectangle.
    /// \param px The new x-position.
    void setX(float px);

    /// \brief Set the y-position of the rectangle.
    /// \param py The new y-position.
    void setY(float py);

    /// \brief Set the width of the rectangle.
    /// \param w The new width.
    void setWidth(float w);

    /// \brief Set the height of the rectangle.
    /// \param h The new height.
    void setHeight(float h);

    /// \brief Set the position of the rectangle.
    /// \param px The new x-position.
    /// \param py The new y-position.
    void setPosition(float px, float py);

    /// \brief Set the position of the rectangle.
    ///
    /// \warning The z-component of the passed ofPoint is ignored.
    ///
    /// \param p The position as an ofPoint.
    void setPosition(const ofPoint& p);

	/// \brief Set the size of the rectangle.
	/// \param w The new width.
	/// \param h The new height.
	void setSize(float w, float h);

    /// \brief Set position and size of the rectangle from the center.
    ///
    /// The center of the rectangle is defined and the width and height grow
    /// out around the center.
    ///
    /// \param px The x-position of the rectangle's center.
    /// \param py The y-position of the rectangle's center.
    /// \param w The width of the rectangle.
    /// \param h The height of the rectangle.
    void setFromCenter(float px, float py, float w, float h);

    /// \brief Set position and size of the rectangle from the center.
    ///
    /// The center of the rectangle is defined and the width and height grow
    /// out around the center.
    ///
    /// \param p The position of the rectangle's center as an ofPoint.
    /// \param w The width of the rectangle.
    /// \param h The height of the rectangle.
    void setFromCenter(const ofPoint& p, float w, float h);

    /// \}

    /// \name Transformation
    /// \{

    /// \brief Translate the rectangle's position by an x and y amount.
    /// \param dx The amount to translate in the x direction.
    /// \param dy The amount to translate in the y direction.
    void translate(float dx, float dy);

    /// \brief Translate the rectangle's position by an x and y amount.
    ///
    /// \warning The z-component of the passed ofPoint is ignored.
    ///
    /// \param dp The amount to translate as an ofPoint.
    void translate(const ofPoint& dp);

    /// \brief Translate the x-position of the rectangle.
    /// \param dx The amount to translate on the x-axis.
    void translateX(float dx);

    /// \brief Translate the y-position of the rectangle.
    /// \param dy The amount to translate on the y-axis.
    void translateY(float dy);

    /// \brief Scale the rectangle.
    ///
    /// Scaling will scale the width and the height, but will not change the
    /// position.
    ///
    /// \param s The scaling factor.
    void scale(float s);

    /// \brief Scale the rectangle.
    ///
    /// Scaling will scale the width and the height, but will not change the
    /// position.
    ///
    /// This is equivalent to calling:
    ///
    /// ~~~~{.cpp}
    ///
    /// ofRectangle myRect(0, 0, 100, 100);
    ///
    /// float sX = 2;
    /// float sY = 2;
    ///
    /// myRect.scaleWidth(sX);
    /// myRect.scaleHeight(sY);
    ///
    /// ~~~~
    /// \param sX The width-scaling factor.
    /// \param sY The height-scaling factor.
    void scale(float sX, float sY);

    /// \brief Scale the rectangle.
    ///
    /// Scaling will scale the width and the height, but will not change the
    /// position.
    ///
    /// Passing an ofPoint as a scaling factor `s` scales both width and height.
    ///
    /// This is equivalent to calling:
    ///
    /// ~~~~{.cpp}
    ///
    /// ofRectangle myRect(0, 0, 100, 100);
    ///
    /// ofPoint myScaler(2, 2);
    ///
    /// myRect.scaleWidth(myScaler.x);
    /// myRect.scaleHeight(myScaler.y);
    ///
    /// ~~~~
    ///
    /// \warning The z-component of the passed ofPoint is ignored.
    ///
    /// \param s The scaling factor.
    void scale(const ofPoint& s);

    /// \brief Scale the width of the rectangle.
    ///
    /// This will scale the width but will not change the position or height.
    ///
    /// \param sX The width-scaling factor.
    void scaleWidth(float sX);

    /// \brief Scale the height of the rectangle.
    ///
    /// This will scale the height but will not change the position and width.
    ///
    /// \param sY The height-scaling factor.
    void scaleHeight(float sY);

    /// \brief Scales both the width and height the ofRectangle from its center.
    ///
    /// The center point of the rectangle will remain fixed and the width,
    /// height, x, and y will be adjusted.
    ///
    /// \param s The scaling factor.
    void scaleFromCenter(float s);

    /// \brief Scales both the width and height the ofRectangle from its center.
    ///
    /// The center point of the rectangle will remain fixed and the width,
    /// height, x, and y will be adjusted.
    ///
    /// \param sX the width-scaling factor.
    /// \param sY the height-scaling factor.
    void scaleFromCenter(float sX, float sY);

    /// \brief Scales both the width and height the ofRectangle from its center.
    ///
    /// The center point of the rectangle will remain fixed and the width,
    /// height, x, and y will be adjusted.
    ///
    ///
    /// This is equivalent to calling:
    ///
    /// ~~~~{.cpp}
    ///
    /// ofRectangle myRect(0, 0, 100, 100);
    ///
    /// ofPoint myScaler(2, 2);
    ///
    /// myRect.scaleFromCenter(myScaler.x, myScaler.y);
    ///
    /// ~~~~
    ///
    /// \warning The z-component of the passed ofPoint is ignored.
    ///
    /// \param s The scaling factor.
    void scaleFromCenter(const ofPoint& s);

    /// \brief Scale the rectanle using a target ofRectangle and ofScaleMode.
    ///
    /// Adjusts the ofRectangle to match the provided ofRectangle using the
    /// provided ofScaleMode.
    ///
    /// Assuming the ofRectangle as the Subject and the passed-in ofRectangle as
    /// the Target:
    ///
    /// __OF_SCALEMODE_FIT__:
    ///
    /// This centers the Subject rectangle within the Target rectangle and
    /// resizes the Subject rectangle to completely fit within the Target
    /// Rectangle.
    ///
    /// - It fits the Subject rectangle inside the target rectangle.
    /// - It preserves Subject's aspect ratio.
    /// - The Subject's final area <= the Target's area.
    /// - The Subject's center == the Target's center.
    ///
    /// __OF_SCALEMODE_FILL__:
    ///
    /// This centers the Subject rectangle within the Target rectangle and
    /// resizes the Subject rectangle to completely encompass the Target
    /// Rectangle.
    ///
    /// - It fills the Target rectangle with the Subject rectangle.
    /// - It preserves the Subject's aspect ratio.
    /// - The Subject's Area >= the Target's area.
    /// - The Subject's center == the Target's center.
    ///
    /// __OF_SCALEMODE_CENTER__:
    ///
    /// This centers the Subject rectangle within the Target rectangle and does
    /// not modify the Subject's scale.
    ///
    /// - It preserves the Subject's aspect ratio.
    /// - The Subject's area is unchanged.
    /// - The Subject's center == Target's center.
    ///
    /// __OF_SCALEMODE_STRETCH_TO_FILL__:
    ///
    /// This simply modifies the Subject rectangle to match the Target
    /// Rectangle's dimensions.
    ///
    /// - It can change the Subject's aspect ratio.
    /// - The Subject's area == the Target's area.
    /// - The Subject's center == the Target's center.
    ///
    /// For an example of the various ofScaleModes, see the
    /// *graphics/rectangleAlignmentAndScaling/* example within the examples
    /// directory.
    ///
    /// \param targetRect The Target Rectangle to scale to.
    /// \param scaleMode The scale mode to use when scaling.
    void scaleTo(const ofRectangle& targetRect,
                 ofScaleMode scaleMode = OF_SCALEMODE_FIT);


    /// \brief Scale the rectangle using a target ofRectangle and parameters.
    ///
    /// Scales the ofRectangle to match the target ofRectangle. It will use the
    /// ofAspectRatioMode to scale the ofRectangle and will use the use the
    /// alignment anchor parameters to position the rectangle.
    ///
    /// This is a convenience method when both the target and subject rectangles
    /// are using the same alignment anchors. For a more complete explanation,
    /// see void scaleTo(const ofRectangle& targetRect, ofAspectRatioMode
    /// subjectAspectRatioMode, ofAlignHorz modelHorzAnchor, ofAlignVert
    /// modelVertAnchor, ofAlignHorz subjectHorzAnchor, ofAlignVert
    /// subjectVertAnchor);
    ///
    /// For a working example of how to use ofAspectRatioMode, ofAlignVert, and
    /// ofAlignHorz, see the *graphics/rectangleAlignmentAndScaling/* example
    /// within the examples directory.
    ///
    /// \param targetRect The Target Rectangle to scale to.
    /// \param subjectAspectRatioMode Aspect ratio scaling mode.
    /// \param sharedHorzAnchor The horizontal alignment method.
    /// \param sharedVertAnchor The vertical alignment method.
    void scaleTo(const ofRectangle& targetRect,
                 ofAspectRatioMode subjectAspectRatioMode,
                 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
                 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);

    /// \brief Scale the rectangle using a target ofRectangle and parameters.
    ///
    /// Adjusts the ofRectangle to match the passed-in ofRectangle. It will
    /// use the ofAspectRatioMode to scale the ofRectangle, and will use the
    /// use the alignment anchor parameters to position the rectangle.
    ///
    /// Assuming the ofRectangle as the Subject and the passed-in ofRectangle
    /// as the Target:
    ///
    /// __ofAspectRatioMode Options:__
    ///
    /// - `OF_ASPECT_RATIO_IGNORE`  : Sets the Subject rectangle's width and height to match those of the Target.
    /// - `OF_ASPECT_RATIO_KEEP`    : Resizes the Subject rectangle to completely fit within the Target rectangle.
    /// - `OF_ASPECT_RATIO_KEEP_BY_EXPANDING` : Resizes the Subject rectangle to completely enclose the Target rectangle.
    ///
    /// __ofAlignHorz Options:__
    ///
    /// - `OF_ALIGN_HORZ_IGNORE` : Does not perform any horizontal alignment.
    /// - `OF_ALIGN_HORZ_LEFT`   : Uses the left edge of the rectangle to horizontally anchor the alignment.
    /// - `OF_ALIGN_HORZ_RIGHT`  : Uses the right edge of the rectangle to horizontally anchor the alignment.
    /// - `OF_ALIGN_HORZ_CENTER` : Uses the center of the rectangle to horizontally anchor the alignment.
    ///
    /// __ofAlignVert Options:__
    ///
    /// - `OF_ALIGN_VERT_IGNORE` : Does not perform any vertical alignment.
    /// - `OF_ALIGN_VERT_TOP`    : Uses the upper edge of the rectangle to vertically anchor the alignment.
    /// - `OF_ALIGN_VERT_BOTTOM` : Uses the bottom edge of the rectangle to vertically anchor the alignment.
    /// - `OF_ALIGN_VERT_CENTER` : Uses the center of the rectangle to vertically anchor the alignment.
    ///
    /// For an example of how to use ofAspectRatioMode, ofAlignVert,
    /// and ofAlignHorz, see the *graphics/rectangleAlignmentAndScaling/*
    /// example within the examples directory.
    ///
    /// \param targetRect The Target Rectangle to scale to.
    /// \param subjectAspectRatioMode Aspect ratio scaling mode.
    /// \param modelHorzAnchor The target horizontal alignment method.
    /// \param modelVertAnchor The target vertical alignment method.
    /// \param subjectHorzAnchor The subject horizontal alignment method.
    /// \param subjectVertAnchor The subject vertical alignment method.
    void scaleTo(const ofRectangle& targetRect,
                 ofAspectRatioMode subjectAspectRatioMode,
                 ofAlignHorz modelHorzAnchor,
                 ofAlignVert modelVertAnchor,
                 ofAlignHorz subjectHorzAnchor,
                 ofAlignVert subjectVertAnchor);

    /// \}

    /// \name Alignment
    /// \{


    /// \brief Horizontally align a rectangle using a position and anchor edge.
    ///
    /// Aligns the horizontal position of the ofRectangle to the given x-
    /// position using an ofAlignHorz constant. If a constant is not passed in
    /// this will align the center of the rectangle.
    ///
    /// __ofAlignHorz Constants:__
    ///
    /// - `OF_ALIGN_HORZ_IGNORE` : Does not perform any horizontal alignment.
    /// - `OF_ALIGN_HORZ_LEFT`   : Uses the left edge of the rectangle to horizontally anchor the alignment.
    /// - `OF_ALIGN_HORZ_RIGHT`  : Uses the right edge of the rectangle to horizontally anchor the alignment.
    /// - `OF_ALIGN_HORZ_CENTER` : Uses the center of the rectangle to horizontally anchor the alignment.
    ///
    /// For an example of how to use ofAlignHorz, see the
    /// *graphics/rectangleAlignmentAndScaling/* example within the examples
    /// directory.
    ///
    /// \param targetX The x-position to align this rectangle to.
    /// \param thisHorzAnchor The edge of this rectangle to align.
    void alignToHorz(const float& targetX,
                     ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER);

    /// \brief Horizontally align two rectangles.
    ///
    /// Aligns the horizontal position of the ofRectangle to that of the
    /// passed-in ofRectangle. Can take an ofAlignHorz constant.
    ///
    /// If a constant is not passed in, this will align the horizontal centers
    /// of the rectangles.
    ///
    /// If a constant is passed in, it will use that constant for both
    /// rectangles.
    ///
    /// \param targetRect The target rectangle to align this rectangle to.
    /// \param sharedAnchor The common edge of the rectangles to align.
    void alignToHorz(const ofRectangle& targetRect,
                     ofAlignHorz sharedAnchor = OF_ALIGN_HORZ_CENTER);

    /// \brief Horizontally align two rectangles.
    ///
    /// Aligns the horizontal position of the ofRectangle to that of the
    /// passed-in ofRectangle. Takes two ofAlignHorz constants.
    ///
    /// Will align the chosen anchor in the ofRectangle with the chosen anchor
    /// in the passed-in ofRectangle.
    ///
    /// \param targetRect The target rectangle to align this rectangle to.
    /// \param targetHorzAnchor The edge of the other rectangle to align.
    /// \param thisHorzAnchor The edge of this rectangle to align.
    void alignToHorz(const ofRectangle& targetRect,
                     ofAlignHorz targetHorzAnchor,
                     ofAlignHorz thisHorzAnchor);

    /// \brief Vertically align this ofRectangle to a target y-position.
    ///
    /// Aligns the vertical position of the ofRectangle to the given x-position
    /// using an ofAlignVert constant.
    ///
    /// If a constant is not passed in, this will align the center of the
    /// rectangle.
    ///
    /// __ofAlignVert Constants:__
    ///
    /// - `OF_ALIGN_VERT_IGNORE` : Does not perform any vertical alignment.
    /// - `OF_ALIGN_VERT_TOP`    : Uses the upper edge of the rectangle to vertically anchor the alignment.
    /// - `OF_ALIGN_VERT_BOTTOM` : Uses the bottom edge of the rectangle to vertically anchor the alignment.
    /// - `OF_ALIGN_VERT_CENTER` : Uses the center of the rectangle to vertically anchor the alignment.
    ///
    /// For a working example of how to use ofAlignVert, see the
    /// *graphics/rectangleAlignmentAndScaling/* example within the examples
    /// directory
    ///
    /// \param targetY The target y-position to align this rectangle to.
    /// \param sharedAnchor The alignment position of this ofRectangle to use.
    void alignToVert(const float& targetY,
                     ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);

    /// \brief Vertically align this ofRectangle to another target ofRectangle.
    ///
    /// If a constant is not passed in, this will align the vertical centers
    /// of the rectangles.
    ///
    /// If a constant is passed in, it will use that constant for both rectangles.
    ///
    /// \param targetRect The target ofRectangle to align this rectangle to.
    /// \param sharedAnchor The alignment position for both rectangles to use.
    void alignToVert(const ofRectangle& targetRect,
                     ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);


    /// \brief Vertically align this ofRectangle to another target ofRectangle.
    ///
    /// Will align the chosen anchor in the ofRectangle with the chosen anchor
    /// in the passed-in ofRectangle.
    ///
    /// \param targetRect The target ofRectangle to align this rectangle to.
    /// \param targetVertAnchor The alignment anchor for this rectangle.
    /// \param thisVertAnchor The alignment anchor for the other rectangle.
    void alignToVert(const ofRectangle& targetRect,
                     ofAlignVert targetVertAnchor,
                     ofAlignVert thisVertAnchor);

    /// \brief Align this ofRectangle to an ofPoint in both x- and y dimentions.
    ///
    /// Aligns the position of the ofRectangle to the given point using an
    /// ofAlignHorz constant and an ofAlignVert constant. If neither constant is
    /// passed in, this will align the center of the rectangle.
    ///
    /// __ofAlignHorz Constants:__
    ///
    /// - `OF_ALIGN_HORZ_IGNORE` : Does not perform any horizontal alignment.
    /// - `OF_ALIGN_HORZ_LEFT`   : Uses the left edge of the rectangle to horizontally anchor the alignment.
    /// - `OF_ALIGN_HORZ_RIGHT`  : Uses the right edge of the rectangle to horizontally anchor the alignment.
    /// - `OF_ALIGN_HORZ_CENTER` : Uses the center of the rectangle to horizontally anchor the alignment.
    ///
    /// __ofAlignVert Constants:__
    ///
    /// - `OF_ALIGN_VERT_IGNORE` : Does not perform any vertical alignment.
    /// - `OF_ALIGN_VERT_TOP`    : Uses the upper edge of the rectangle to vertically anchor the alignment.
    /// - `OF_ALIGN_VERT_BOTTOM` : Uses the bottom edge of the rectangle to vertically anchor the alignment.
    /// - `OF_ALIGN_VERT_CENTER` : Uses the center of the rectangle to vertically anchor the alignment.
    ///
    /// For a working example of how to use ofAlignVert and ofAlignHorz, see the
    /// *graphics/rectangleAlignmentAndScaling/* example within the examples
    /// directory.
    ///
    /// \warning The z-component of the passed ofPoint is ignored.
    ///
    /// \param targetPoint The target ofPoint to align to.
    /// \param thisHorzAnchor The horizontal alignment anchor.
    /// \param thisVertAnchor The vertical alignment anchor.
    void alignTo(const ofPoint& targetPoint,
                ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER,
                ofAlignVert thisVertAnchor = OF_ALIGN_VERT_CENTER);

    /// \brief Vertically align this ofRectangle to another target ofRectangle.
    ///
    /// Aligns the position of the ofRectangle to that of the passed-in
    /// ofRectangle. Can take an ofAlignHorz constant and an ofAlignVert
    /// constant.
    ///
    /// If neither constant is passed in, this will align the centers of the
    /// rectangles.
    ///
    /// If constants are passed in, it will use those constants for both
    /// rectangles.
    ///
    /// \param targetRect The target ofRectangle to align this rectangle to.
    /// \param sharedHorzAnchor The alignment anchor for both rectangles.
    /// \param sharedVertAnchor The alignment anchor for both rectangles.
    void alignTo(const ofRectangle& targetRect,
                 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
                 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);


    /// \brief Vertically align this ofRectangle to another target ofRectangle.
    ///
    /// Aligns the position of the ofRectangle to that of the passed-in
    /// ofRectangle. Takes two ofAlignHorz constants and two ofAlignVert
    /// constants.
    ///
    /// Will align the chosen anchors in the ofRectangle with the chosen
    /// anchors in the passed-in ofRectangle.
    ///
    /// \param targetRect The target ofRectangle to align this rectangle to.
    /// \param targetHorzAnchor The vertical alignment anchor for the target rectangle.
    /// \param targetVertAnchor The horizontal alignment anchor for the target rectangle.
    /// \param thisHorzAnchor The horizontal alignment anchor for this rectangle.
    /// \param thisVertAnchor The vertical alignment anchor for this rectangle.
    void alignTo(const ofRectangle& targetRect,
                 ofAlignHorz targetHorzAnchor,
                 ofAlignVert targetVertAnchor,
                 ofAlignHorz thisHorzAnchor,
                 ofAlignVert thisVertAnchor);

    /// \}

    /// \name Intersection
    /// \{

    /// \brief Determines if the coordinates (x, y) are within the ofRectangle.
    ///
    /// Note that coordinates on the edge of the ofRectangle are not
    /// considered within the rectangle and will return false.
    ///
    /// Coordinates (x, y) are considered inside the rectangle if:
    ///
    /// `x > rectMinX && x < rectMinX && y > rectMinY && y < rectMaxY`
    ///
    /// \param px The x-coordinate to test.
    /// \param py The y-coordinate to test.
    /// \returns true if px and py are inside this ofRectangle.
    bool inside(float px, float py) const;

    /// \brief Determines if the ofPoint is within the ofRectangle.
    ///
    /// Note that points on the edge of the ofRectangle are not
    /// considered within the rectangle and will return false.
    ///
    /// Coordinates (x, y) are considered inside the rectangle if:
    ///
    /// `x > rectMinX && x < rectMinX && y > rectMinY && y < rectMaxY`
    ///
    /// \warning The z-component of the passed ofPoint is ignored.
    ///
    /// \param p The point to test.
    /// \returns true if the point p is inside this ofRectangle.
    bool inside(const ofPoint& p) const;

    /// \brief Determines if another ofRectangle is within the ofRectangle.
    ///
    /// Note that rectangles that share an edge with the ofRectangle are not
    /// considered within the rectangle and will return false.
    ///
    /// \param rect The ofRectangle to test.
    /// \returns true if all four corners of the rect are within this rectangle.
    bool inside(const ofRectangle& rect) const;

    /// \brief Determines if a line segment is within the ofRectangle.
    ///
    /// This can be used to test if a line segment is inside the rectangle.
    ///
    /// Note that points on the edge of the ofRectangle are not considered
    /// within the rectangle and will return false.
    ///
    /// \warning The z-components of the passed ofPoints are ignored.
    ///
    /// \param p0 The first point to test.
    /// \param p1 The second point to test.
    /// \returns true if both points are inside the rectangle.
    bool inside(const ofPoint& p0, const ofPoint& p1) const;

    /// \brief Determines if another rectangle intersects with the ofRectangle.
    ///
    /// Rectangles that only share an edge and do not intersect otherwise are
    /// not considered to intersect and will return false.
    ///
    /// \param rect The rectangle to test.
    /// \returns `true` if the area contained within the ofRectangle overlaps
    ///     with the area contained within the passed ofRectangle.
    bool intersects(const ofRectangle& rect) const;

    /// \brief Determines if a line segment intersects with the ofRectangle.
    ///
    /// \warning The z-components of the passed ofPoints are ignored.
    ///
    /// \param p0 The first point to test.
    /// \param p1 The second point to test.
    /// \returns `true` if the line segment defined by the two passed ofPoints
    ///     either crosses the perimeter of the ofRectangle or is completely
    ///     contained within.
    bool intersects(const ofPoint& p0, const ofPoint& p1) const;

    /// \brief Grow the ofRectangle to include the given (x, y) coordinates.
    ///
    /// This will potentially change the width, height, x-position, and y-
    /// position of the ofRectangle. If the points are already within the
    /// rectangle, this rectangle will remain unchanged.
    ///
    /// \param px The x-coordinate to include.
    /// \param py The y-coordiante to include.
    void growToInclude(float px, float py);

    /// \brief Grow the ofRectangle to include the given point.
    ///
    /// \warning The z-component of the passed ofPoint is ignored.
    ///
    /// This will potentially change the width, height, x-position, and y-
    /// position of the ofRectangle.  If the point is already within the
    /// rectangle, this rectangle will remain unchanged.
    ///
    /// \param p The position to include.
    void growToInclude(const ofPoint& p);

    /// \brief Grow the ofRectangle to include the given ofRectangle.
    ///
    /// This will potentially change the width, height, x-position, and y-
    /// position of the ofRectangle.  If the rectangle is already within the
    /// rectangle, this rectangle will remain unchanged.
    ///
    /// \param rect The rectangle to include.
    void growToInclude(const ofRectangle& rect);

    /// \brief Grow the ofRectangle to include the given line segment.
    ///
    /// \warning The z-components of the passed ofPoints are ignored.
    ///
    /// This will potentially change the width, height, x-position, and
    /// y-position of the ofRectangle.  If the points are already within the
    /// rectangle, this rectangle will remain unchanged.
    ///
    /// \param p0 The first point to include.
    /// \param p1 The second point to include.
    void growToInclude(const ofPoint& p0, const ofPoint& p1);

    /// \brief Get the intersecting area between this rectangle and another.
    ///
    /// This method will attempt to return the area of overlap between this
    /// rectangle and the passed rectangle.
    ///
    /// If the two rectangles do not overlap, it will return an "empty"
    /// rectangle located (0, 0) with 0 width and 0 height.
    ///
    /// If the two rectangles only share an edge this will return a rectangle
    /// positioned on that edge:
    ///
    /// If the shared edge is vertical, the rectangle will have zero width,
    /// otherwise it will have zero height.
    ///
    /// \param rect The rectangle to intersect.
    /// \returns A new ofRectangle representing the intersecting area or an
    ///          empty rectangle (0, 0, 0, 0) if there is no intersection.
    ofRectangle getIntersection(const ofRectangle& rect) const;

    /// \brief Get the union area between this rectangle and anohter.
    ///
    /// \sa growToInclude(const ofRectangle& rect)
    /// \param rect The rectangle to unite with.
    /// \returns A new ofRectangle whose area contains both the area of the
    ///          this rectangle and the passed rectangle..
    ofRectangle getUnion(const ofRectangle& rect) const;

    /// \}

    /// \name Standardization
    /// \{

    /// \brief Standardize the rectangle
    ///
    /// ofRectangle is a simple container for describing the position and size
    /// of 2D rectangles. Like many rectangle data structures found in other
    /// frameworks and graphics libraries, member width and height variables can
    /// take negative values. Additionally, x / y-position and width / height
    /// are publicly accessible, resulting in great programming freedom.
    /// Consequently, two visually identical rectangles can be represented in
    /// the following ways:
    ///
    /// ~~~~{.cpp}
    ///     ofRectangle myRect(0,0,100,100);
    ///
    ///     // OR
    ///
    ///     ofRectangle myRect(100,100,-100,-100);
    /// ~~~~
    ///
    /// While both representations will yield the same visual results in all
    /// openFrameworks renderers, the results of some method operations that
    /// modify x / y / width / height (such as scaling) produce mathematically
    /// correct, but visually different results for each of the above
    /// representations.
    ///
    /// "Standardized" rectangles are rectangles whose width >= 0 and height
    /// >= 0. This method can be used to ensure that the rectangle is
    /// "standardized". If the rectangle is non-standard, it will modify the x /
    /// width and y / height values into their respective standardized versions.
    void standardize();

    /// \brief Get the standardized representation of this rectangle..
    ///
    /// For more information about standardized rectangles, see the discussion
    /// of the standardize() method.
    ///
    /// \returns the Standardized version of this ofRectangle.
    ofRectangle getStandardized() const;

    /// \brief Returns true if this ofRectangle is standardized.
    ///
    /// For more information about standardized rectangles, see the discussion
    /// of the standardize() method.
    ///
    /// \returns true if both width >= 0 and height >= 0.
    bool isStandardized() const;

    /// \}

    /// \name Getters
    /// \{

    /// \brief Get the area of the ofRectangle.
    ///
    /// This is the product of the width and height.
    ///
    /// ~~~~{.cpp}
    ///     // Create a rectangle that is 100 units wide and 200 units tall
    ///     ofRectangle myRect(0,0,100,200);
    ///
    ///     // Get the area of that rectangle
    ///     float areaValue = myRect.getArea();
    ///
    ///     // areaValue will be 20000.0.
    /// ~~~~
    ///
    /// \returns The area of the rectangle.
    float getArea() const;


    /// \brief Gets the perimeter of the ofRectangle.
    ///
    /// This is the sum of the lengths of the sides.
    ///
    /// ~~~~{.cpp}
    ///     // Create a rectangle that is 100 units wide and 200 units tall
    ///     ofRectangle myRect(0,0,100,200);
    ///
    ///     // Get the perimeter of that rectangle
    ///     float perimeterValue = myRect.getPerimeter();
    ///
    ///     // perimeterValue will be 600.0.
    /// ~~~~
    ///
    /// \returns The perimeter of the rectangle.
    float getPerimeter() const;

    /// \brief Gets the ratio of width to height of the ofRectangle.
    ///
    /// ~~~~{.cpp}
    ///     // Create a rectangle that is 100 units wide and 200 units tall
    ///     ofRectangle myRect(0,0,100,200);
    ///
    ///     // Get the aspect ratio of that rectangle
    ///     float aspectRatioValue = myRect.getAspectRatio();
    ///
    ///     // aspectRatioValue will be 0.5.
    /// ~~~~
    ///
    /// \returns The aspect ratio of the rectangle.
    float getAspectRatio() const;

    /// \brief Determines if the ofRectangle's area is zero.
    /// \sa getArea().
    /// \returns true if both the width == 0 and height == 0.
    bool isEmpty() const;

    /// \brief Get the minimum x- and y- coordinates of the ofRectangle.
    /// \returns The minimum x- and y- coordinates of the rectangle.
    ofPoint getMin() const;

    /// \brief Get the maximum x- and y- coordinates of the ofRectangle.
    /// \returns The maximum x- and y- coordinates of the rectangle.
    ofPoint getMax() const;

    /// \brief Get the smallest x-position of the ofRectangle.
    /// \returns The smallest x-position of the rectangle.
    float getMinX() const;

    /// \brief Get the largest x-position of the ofRectangle.
    /// \returns The largest x-position of the rectangle.
    float getMaxX() const;

    /// \brief Get the smallest y-position of the ofRectangle.
    /// \returns The smallest y-position of the rectangle.
    float getMinY() const;

    /// \brief Get the largest y-position of the ofRectangle.
    /// \returns The largest y-position of the rectangle.
    float getMaxY() const;

    /// \brief Gets the x-position of the left edge of the ofRectangle.
    /// \sa getMinX()
    /// \returns The x-position of the left edge of the rectangle.
    float getLeft() const;

    /// \brief Gets the x-position of the right edge of the ofRectangle.
    /// \sa getMaxX()
    /// \returns The y-position of the right edge of the rectangle.
    float getRight() const;

    /// \brief Gets the y-position of the top edge of the ofRectangle.
    /// \sa getMinY()
    /// \returns The y-position of the top edge of the rectangle.
    float getTop() const;

    /// \brief Gets the y-position of the bottom edge of the ofRectangle.
    /// \sa getMaxY()
    /// \returns The y-position of the bottom edge of the rectangle.
    float getBottom() const;

    /// \brief Get the top-left coordinates of the ofRectangle.
    /// \sa getMin()
    /// \returns The top-left coordinates of the rectangle.
    ofPoint getTopLeft() const;

    /// \brief Get the top-right coordinates of the ofRectangle.
    /// \returns The top-right coordinates of the rectangle.
    ofPoint getTopRight() const;

    /// \brief Get the bottom-left coordinates of the ofRectangle.
    /// \returns The bottom-left coordinates of the rectangle.
    ofPoint getBottomLeft() const;

    /// \brief Get the bottom-right coordinates of the ofRectangle.
    /// \sa getMax()
    /// \returns The bottom-right coordinates of the rectangle.
    ofPoint getBottomRight() const;

    /// \brief A convenience method that returns the value of one of
    /// the horizontal edges of the ofRectangle using the `ofAlignHorz` enum.
    ///
    /// Possible anchor values are:
    ///
    /// - `OF_ALIGN_HORZ_IGNORE`  : returns 0.0
    /// - `OF_ALIGN_HORZ_LEFT`    : returns the position of the left edge, equivalent to ofRectangle::geLeft().
    /// - `OF_ALIGN_HORZ_RIGHT`   : returns the position of the right edge, equivalent to ofRectangle::getRight().
    /// - `OF_ALIGN_HORZ_CENTER`  : returns the x-position of the center of the ofRectangle.
    ///
    /// Any other anchor value will return `0.0`.
    ///
    /// \param anchor The anchor position to query.
    /// \returns the value of the referenced anchor position.
    float getHorzAnchor(ofAlignHorz anchor) const;

    /// \brief A convenience method that returns the value of one of the
    /// vertical edges of the ofRectangle using the ofAlignVert enum.
    ///
    /// Possible anchor values are:
    ///
    /// - `OF_ALIGN_VERT_IGNORE`  : returns 0.0
    /// - `OF_ALIGN_VERT_TOP`     : returns the position of the upper edge, equivalent to ofRectangle::getTop().
    /// - `OF_ALIGN_VERT_BOTTOM`  : returns the position of the bottom edge, equivalent to ofRectangle::getBottom().
    /// - `OF_ALIGN_VERT_CENTER`  : returns the y-position of the center of the ofRectangle.
    ///
    /// Any other anchor value will return `0.0`.
    ///
    /// \param anchor The anchor position to query.
    /// \returns the value of the referenced anchor position.
    float getVertAnchor(ofAlignVert anchor) const;

    /// \brief Get the ofRectangle's position.
    ///
    /// \warning The z-component of the returned ofPoint is undefined.
    ///
    /// \returns The rectangle's position.
    ofPoint getPosition() const;

    /// \brief Get a reference to the ofRectangle's position.
    ///
    /// \warning Changes to the z-component of the referenced ofPoint are not
    /// guaranteed to be preserved and will be ignored during further
    /// ofRectangle operations.
    ///
    /// \returns A reference to the position of the rectangle.
    ofPoint& getPositionRef();

    /// \brief Get the coordiantes of the ofRectangle's center.
    ///
    /// \warning The z-component of the returned ofPoint will always be 0.
    ///
    /// \returns The coordinates of the center of the rectangle (z = 0).
    ofPoint getCenter() const;

    /// \brief Gets the x-position of the ofRectangle.
    /// \returns The x-position of the rectangle.
    float getX() const;

    /// \brief Gets the y-position of the ofRectangle.
    /// \returns the y-position of the rectangle.
    float getY() const;

    /// \brief Gets the width of the ofRectangle.
    /// \returns The width of the rectangle.
    float getWidth() const;

    /// \brief Gets the height of the ofRectangle.
    /// \returns The height of the rectangle.
    float getHeight() const;

    /// \}

    /// \name Operators
    /// \{

    /// \brief Assignment operator.
    /// \param rect The rectangle to assign.
    /// \returns A reference to this rectangle.
    ofRectangle& operator = (const ofRectangle& rect);

    /// \brief Returns a new ofRectangle where the x and y-positions of the
    /// rectangle are offset by the (x, y) coordinates of the ofPoint.
    /// \param p The point to translate.
    /// \returns The translated ofRectangle.
    ofRectangle operator + (const ofPoint& p);

	/// \brief Returns a new ofRectangle where the x and y-positions of the
	/// rectangle are offset by the (x, y) coordinates of the ofPoint.
	/// \param p The point to translate.
	/// \returns The translated ofRectangle.
	ofRectangle operator - (const ofPoint& p);

    /// \brief If both ofRectangles have the same x, y, width, and height,
    /// they are considered equal.
    /// \param rect The rectangle to compare.
    /// \returns True if the rectangles are equal.
    bool operator == (const ofRectangle& rect) const;

    /// \brief If the two ofRectangles differ in x, y, width, or height, they
    /// are considered unequal.
    /// \param rect The rectangle to compare.
    /// \returns True if the rectangles are not equal.
    bool operator != (const ofRectangle& rect) const;
    
    bool isZero() const;


    /// \}

    /// \name Properties
    /// \{


    /// \brief The (x,y) position of the ofRectangle as an ofPoint.
    ///
    /// \warning The z-component of this position is preserved and can be used
    /// but all ofRectangle operations will ignore the z-component.
    ofPoint position;

    /// \brief The x-position of the ofRectangle.
    float& x;

    /// \brief The y-position of the ofRectangle.
    float& y;

    /// \brief The width of the ofRectangle.
    float width;

    /// \brief The height of the ofRectangle.
    float height;

    /// \}
};

/// \cond INTERNAL
/// \warning The internal z-component of the ofPoint is preserved even though it
/// is not used.
ostream& operator<<(ostream& os, const ofRectangle& rect);
/// \warning The internal z-component of the ofPoint is preserved even though it
/// is not used.
istream& operator>>(istream& is, ofRectangle& rect);
/// \endcond
