    #pragma once


#include "ofConstants.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>



/// \brief Used to represent the available rectangle aspect ratio scaling modes.
///
/// \sa ofRectangle
enum ofAspectRatioMode {
	/// \brief Set the rectangle's width and height to match the target.
	OF_ASPECT_RATIO_IGNORE            = 0,
	/// \brief Resizes the rectangle to completely fit within the target.
	OF_ASPECT_RATIO_KEEP              = 1,
	/// \brief Resizes the rectangle to completely enclose the target.
	OF_ASPECT_RATIO_KEEP_BY_EXPANDING = 2,
};

/// \brief Used to represent the available vertical rectangle alignment modes.
///
/// \sa ofRectangle
enum ofAlignVert {
	/// \brief Do not perform any vertical alignment.
	OF_ALIGN_VERT_IGNORE   = 0x0000,
	/// \brief Use the upper edge of the rectangle to vertically anchor the alignment.
	OF_ALIGN_VERT_TOP      = 0x0010,
	/// \brief Use the bottom edge of the rectangle to vertically anchor the alignment.
	OF_ALIGN_VERT_BOTTOM   = 0x0020,
	/// \brief Use the center of the rectangle to vertically anchor the alignment.
	OF_ALIGN_VERT_CENTER   = 0x0040,
};


/// \brief Used to represent the available horizontal rectangle alignment modes.
///
/// \sa ofRectangle
enum ofAlignHorz {
	/// \brief Do not perform any horizontal alignment.
	OF_ALIGN_HORZ_IGNORE   = 0x0000,
	/// \brief Use the left edge of the rectangle to horizontally anchor the alignment.
	OF_ALIGN_HORZ_LEFT     = 0x0001,
	/// \brief Use the right edge of the rectangle to horizontally anchor the alignment.
	OF_ALIGN_HORZ_RIGHT    = 0x0002,
	/// \brief Use the center of the rectangle to horizontally anchor the alignment.
	OF_ALIGN_HORZ_CENTER   = 0x0004,
};

/// \brief Used to represent the available rectangle scaling modes.
///
/// ofScaleMode can usually be interpreted as a concise combination of
/// an ::ofAspectRatioMode, an ::ofAlignVert and an ::ofAlignHorz.
enum ofScaleMode{

	/// \brief Center and scale the rectangle to fit inside the target.
	///
	/// This centers the subject rectangle within the target rectangle and
	/// resizes the subject rectangle to completely fit within the target
	/// rectangle.
	OF_SCALEMODE_FIT     = 0,

	/// \brief Move and scale the rectangle to completely enclose the target.
	///
	/// This centers the subject rectangle within the target rectangle and
	/// resizes the subject rectangle to completely encompass the target
	/// rectangle.
	OF_SCALEMODE_FILL    = 1,

	/// \brief Move the rectangle to be centered on the target.
	///
	/// This centers the subject rectangle within the target rectangle and
	/// does not modify the Subject's size or aspect ratio.
	OF_SCALEMODE_CENTER  = 2, // centers the subject

	/// \brief Match the target rectangle's position and dimensions.
	OF_SCALEMODE_STRETCH_TO_FILL = 3
};


/// \brief A class representing a 2D rectangle.
///
/// ofRectangle is a simple container for describing the position and size of a
/// 2D rectangle. The ofRectangle::standardize() method can be used to ensure
/// that the origin is in the "standard" form.
///
/// \warning While ofRectangle takes glm::vec3
/// all ofRectangle operations are 2D only, ignoring the z-component.
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
	/// \warning The z-component of the passed glm::vec3 is ignored.
    ///
	/// \param p The glm::vec3 representing the position of the rectangle.
    /// \param w The width of the rectangle.
    /// \param h The height of the rectangle.
	ofRectangle(const glm::vec3& p, float w, float h);
	ofRectangle(const glm::vec2& p, float w, float h);

    /// \brief Construct a rectangle by copying another rectangle.
    /// \param rect The rectangle to copy.
    ofRectangle(const ofRectangle& rect);

    /// \brief Construct a rectangle by defining two corners.
    ///
	/// \warning The z-components of the passed glm::vec3s are ignored.
    ///
	/// \param p0 An glm::vec3 representing the upper left hand corner.
	/// \param p1 An glm::vec3 representing the lower right hand corner.
	ofRectangle(const glm::vec3& p0, const glm::vec3& p1);
	ofRectangle(const glm::vec2& p0, const glm::vec2& p1);

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
	/// \warning The z-component of the passed glm::vec3 is ignored.
    ///
    /// \param p The new position.
    /// \param w The new width.
    /// \param h The new height.
	void set(const glm::vec3& p, float w, float h);
	void set(const glm::vec2& p, float w, float h);

    /// \brief Set the position and size by copying them from anohter rectangle.
    /// \param rect The rectangle to copy.
    void set(const ofRectangle& rect);

    /// \brief Set the position and size of the rectangle using corners.
    ///
	/// \warning The z-components of the passed glm::vec3s are ignored.
    ///
	/// \param p0 The glm::vec3 representing the upper left hand corner.
	/// \param p1 The glm::vec3 representing the lower right hand corner.
	void set(const glm::vec3& p0, const glm::vec3& p1);
	void set(const glm::vec2& p0, const glm::vec2& p1);

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
	/// \warning The z-component of the passed glm::vec3 is ignored.
    ///
	/// \param p The position as an glm::vec3.
	void setPosition(const glm::vec3& p);

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
	/// \param p The position of the rectangle's center as an glm::vec3.
    /// \param w The width of the rectangle.
    /// \param h The height of the rectangle.
	void setFromCenter(const glm::vec3& p, float w, float h);
	void setFromCenter(const glm::vec2& p, float w, float h);

    /// \}

    /// \name Transformation
    /// \{

    /// \brief Translate the rectangle's position by an x and y amount.
    /// \param dx The amount to translate in the x direction.
    /// \param dy The amount to translate in the y direction.
    void translate(float dx, float dy);

    /// \brief Translate the rectangle's position by an x and y amount.
    ///
	/// \warning The z-component of the passed glm::vec3 is ignored.
    ///
	/// \param dp The amount to translate as an glm::vec3.
	void translate(const glm::vec3& dp);
	void translate(const glm::vec2& dp);

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
    /// \param sX The width-scaling factor.
    /// \param sY The height-scaling factor.
    void scale(float sX, float sY);

    /// \brief Scale the rectangle.
    ///
    /// Scaling will scale the width and the height, but will not change the
    /// position.
    ///
	/// \warning The z-component of the passed glm::vec3 is ignored.
    ///
    /// \param s The scaling factor.
	void scale(const glm::vec3& s);
	void scale(const glm::vec2& s);

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
	/// \warning The z-component of the passed glm::vec3 is ignored.
    ///
    /// \param s The scaling factor.
	void scaleFromCenter(const glm::vec3& s);
	void scaleFromCenter(const glm::vec2& s);

    /// \brief Scale the rectanle using a target ofRectangle and ofScaleMode.
    ///
    /// Adjusts the ofRectangle to match the provided ofRectangle using the
    /// provided ofScaleMode.
    ///
    /// \param targetRect The Target Rectangle to scale to.
    /// \param scaleMode The scale mode to use when scaling.
    void scaleTo(const ofRectangle& targetRect,
                 ofScaleMode scaleMode = OF_SCALEMODE_FIT);


    /// \brief Scale the rectangle using a target ofRectangle and parameters.
    ///
    /// Scales the ofRectangle to match the target ofRectangle. It will use the
    /// ofAspectRatioMode to scale the ofRectangle and will use the
    /// alignment anchor parameters to position the rectangle.
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
    /// position using an ofAlignHorz constant as anchor. If a constant is not passed in
    /// this will align the center of the rectangle.
    ///
    /// \param targetX The x-position to align this rectangle to.
    /// \param thisHorzAnchor The edge of this rectangle to align.
    void alignToHorz(const float& targetX,
                     ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER);

    /// \brief Horizontally align two rectangles to another using one anchor for both.
    ///
    /// Aligns the horizontal position of the ofRectangle to that of the
    /// passed-in ofRectangle using an ofAlignHorz constant as anchor.
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

    /// \brief Horizontally align two rectangles to another using two anchors.
    ///
    /// Aligns the horizontal position of the ofRectangle to that of the
    /// passed-in ofRectangle. Takes two ofAlignHorz constants as anchors.
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

    /// \brief Vertically align a rectangle using a position and anchor edge.
    ///
    /// Aligns the vertical position of the ofRectangle to the given y-
    /// position using an ofAlignVert constant as anchor. If a constant is not passed in
    /// this will align the center of the rectangle.
    ///
    /// \param targetY The target y-position to align this rectangle to.
    /// \param sharedAnchor The alignment position of this ofRectangle to use.
    void alignToVert(const float& targetY,
                     ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);

    /// \brief Vertically align two rectangles to another using one anchor for both.
    ///
    /// Aligns the vertical position of the ofRectangle to that of the
    /// passed-in ofRectangle using an ofAlignVert constant as anchor.
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


    /// \brief Vertically align two rectangles to another using two anchors.
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

	/// \brief Align this ofRectangle to an glm::vec3 in both x- and y dimentions.
    ///
    /// Aligns the position of the ofRectangle to the given point using an
    /// ofAlignHorz constant and an ofAlignVert constant. If neither constant is
    /// passed in, this will align the center of the rectangle.
    ///
	/// \warning The z-component of the passed glm::vec3 is ignored.
    ///
	/// \param targetPoint The target glm::vec3 to align to.
    /// \param thisHorzAnchor The horizontal alignment anchor.
    /// \param thisVertAnchor The vertical alignment anchor.
	void alignTo(const glm::vec3& targetPoint,
                ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER,
                ofAlignVert thisVertAnchor = OF_ALIGN_VERT_CENTER);
	void alignTo(const glm::vec2& targetPoint,
				ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER,
				ofAlignVert thisVertAnchor = OF_ALIGN_VERT_CENTER);

	/// \brief Align this ofRectangle to an glm::vec3 in both x- and y dimentions using a shared anchor.
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
    /// \param px The x-coordinate to test.
    /// \param py The y-coordinate to test.
    /// \returns true if px and py are inside this ofRectangle.
    bool inside(float px, float py) const;

	/// \brief Determines if the glm::vec3 is within the ofRectangle.
    ///
    /// Note that points on the edge of the ofRectangle are not
    /// considered within the rectangle and will return false.
    ///
	/// \warning The z-component of the passed glm::vec3 is ignored.
    ///
    /// \param p The point to test.
    /// \returns true if the point p is inside this ofRectangle.
	bool inside(const glm::vec3& p) const;
	bool inside(const glm::vec2& p) const;

    /// \brief Determines if another ofRectangle is completely within the ofRectangle.
    ///
    /// Note that rectangles that share an edge with the ofRectangle are not
    /// considered within the rectangle and will return false.
    ///
    /// \param rect The ofRectangle to test.
    /// \returns true if all four corners of the rect are within this rectangle.
    bool inside(const ofRectangle& rect) const;

	/// \brief Determines if both of the passed glm::vec3s are within the ofRectangle or not.
    ///
    /// Note that points on the edge of the ofRectangle are not considered
    /// within the rectangle and will return false.
    ///
	/// \warning The z-components of the passed glm::vec3s are ignored.
    ///
    /// \param p0 The first point to test.
    /// \param p1 The second point to test.
    /// \returns true if both points are inside the rectangle.
	bool inside(const glm::vec3& p0, const glm::vec3& p1) const;
	bool inside(const glm::vec2& p0, const glm::vec2& p1) const;

    /// \brief Determines if another rectangle intersects with this rectangle.
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
	/// \warning The z-components of the passed glm::vec3s are ignored.
    ///
    /// \param p0 The first point to test.
    /// \param p1 The second point to test.
	/// \returns `true` if the line segment defined by the two passed glm::vec3s
    ///     either crosses the perimeter of the ofRectangle or is completely
    ///     contained within.
	bool intersects(const glm::vec3& p0, const glm::vec3& p1) const;
	bool intersects(const glm::vec2& p0, const glm::vec2& p1) const;

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
    /// This will potentially change the width, height, x-position, and y-
    /// position of the ofRectangle.  If the point is already within the
    /// rectangle, this rectangle will remain unchanged.
    ///
	/// \warning The z-component of the passed glm::vec3 is ignored.
    ///
    /// \param p The position to include.
	void growToInclude(const glm::vec3& p);
	void growToInclude(const glm::vec2& p);

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
    /// This will potentially change the width, height, x-position, and
    /// y-position of the ofRectangle.  If the points are already within the
    /// rectangle, this rectangle will remain unchanged.
    ///
	/// \warning The z-components of the passed glm::vec3s are ignored.
    ///
    /// \param p0 The first point to include.
    /// \param p1 The second point to include.
	void growToInclude(const glm::vec3& p0, const glm::vec3& p1);
	void growToInclude(const glm::vec2& p0, const glm::vec2& p1);

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
    /// \sa getIntersection(const ofRectangle& rect) const
    /// \param rect The rectangle to unite with.
    /// \returns A new ofRectangle whose area contains both the area of the
    ///          this rectangle and the passed rectangle..
    ofRectangle getUnion(const ofRectangle& rect) const;

    /// \}

    /// \name Standardization
    /// \{

    /// \brief Standardize the rectangle
    ///
    /// "Standardized" rectangles are rectangles whose width and height are positive: width >= 0 and height
    /// >= 0. This method can be used to ensure that the rectangle is
    /// "standardized". If the rectangle is non-standard, it will modify the x /
    /// width and y / height values into their respective standardized versions.
    void standardize();

    /// \brief Get the standardized representation of this rectangle.
    ///
    /// "Standardized" rectangles are rectangles whose width and height are positive. 
    /// For more information about standardized rectangles, see the discussion of the standardize() method.
    ///
    /// \returns the Standardized version of this ofRectangle.
    ofRectangle getStandardized() const;

    /// \brief Returns true if this ofRectangle is standardized.
    ///
    /// "Standardized" rectangles are rectangles whose width and height are positive. 
    /// For more information about standardized rectangles, see the discussion of the standardize() method.
    ///
    /// \returns true if both width >= 0 and height >= 0.
    bool isStandardized() const;

    /// \}

    /// \name Getters
    /// \{

    /// \brief Get the area of the ofRectangle.
    ///
    /// This is the product of the width and height of the recatngle.
    ///
    /// \returns The area of the rectangle as float.
    float getArea() const;


    /// \brief Gets the perimeter of the ofRectangle.
    ///
    /// This is the sum of the lengths of the sides.
    ///
    /// \returns The perimeter of the rectangle as float.
    float getPerimeter() const;

    /// \brief Gets the ratio of width to height of the ofRectangle.
    ///
    /// \returns The aspect ratio of the rectangle.
    float getAspectRatio() const;

    /// \brief Determines if the ofRectangle's area is zero.
    /// \sa getArea().
    /// \returns true if both the width == 0 and height == 0, false if either is non-zero.
    bool isEmpty() const;

	/// \brief Get the minimum x and y coordinates of the ofRectangle as glm::vec3.
    /// \returns The minimum x and y coordinates of the rectangle.
	glm::vec3 getMin() const;

	/// \brief Get the maximum x and y coordinates of the ofRectangle as glm::vec3.
    /// \returns The maximum x and y coordinates of the rectangle.
	glm::vec3 getMax() const;

    /// \brief Get the smallest x position of the ofRectangle as float.
    /// \returns The smallest xposition of the rectangle.
    float getMinX() const;

    /// \brief Get the largest x position of the ofRectangle as float.
    /// \returns The largest x position of the rectangle.
    float getMaxX() const;

    /// \brief Get the smallest y position of the ofRectangle as float.
    /// \returns The smallest y position of the rectangle.
    float getMinY() const;

    /// \brief Get the largest y position of the ofRectangle as float.
    /// \returns The largest y position of the rectangle.
    float getMaxY() const;

    /// \brief Gets the x position of the left edge of the ofRectangle as float.
    /// \sa getMinX()
    /// \returns The x position of the left edge of the rectangle.
    float getLeft() const;

    /// \brief Gets the x position of the right edge of the ofRectangle as float.
    /// \sa getMaxX()
    /// \returns The y position of the right edge of the rectangle.
    float getRight() const;

    /// \brief Gets the y position of the top edge of the ofRectangle as float.
    /// \sa getMinY()
    /// \returns The y position of the top edge of the rectangle.
    float getTop() const;

    /// \brief Gets the y position of the bottom edge of the ofRectangle as float.
    /// \sa getMaxY()
    /// \returns The y position of the bottom edge of the rectangle.
    float getBottom() const;

	/// \brief Get the top-left coordinates of the ofRectangle as glm::vec3.
    /// \sa getMin()
    /// \returns The top-left coordinates of the rectangle.
	glm::vec3 getTopLeft() const;

	/// \brief Get the top-right coordinates of the ofRectangle as glm::vec3.
    /// \returns The top-right coordinates of the rectangle.
	glm::vec3 getTopRight() const;

	/// \brief Get the bottom-left coordinates of the ofRectangle as glm::vec3.
    /// \returns The bottom-left coordinates of the rectangle.
	glm::vec3 getBottomLeft() const;

	/// \brief Get the bottom-right coordinates of the ofRectangle as glm::vec3.
    /// \sa getMax()
    /// \returns The bottom-right coordinates of the rectangle.
	glm::vec3 getBottomRight() const;

    /// \brief A convenience method that returns the value of one of
    /// the horizontal edges of the ofRectangle using the `ofAlignHorz` enum.
    ///
    /// \param anchor The anchor position to query.
    /// \returns the value of the referenced anchor position.
    float getHorzAnchor(ofAlignHorz anchor) const;

    /// \brief A convenience method that returns the value of one of the
    /// vertical edges of the ofRectangle using the ofAlignVert enum.
    ///
    /// \param anchor The anchor position to query.
    /// \returns the value of the referenced anchor position.
    float getVertAnchor(ofAlignVert anchor) const;

    /// \brief Get the ofRectangle's position.
    ///
	/// \warning The z component of the returned glm::vec3 is undefined.
    ///
    /// \returns The rectangle's position.
	const glm::vec3& getPosition() const;

	OF_DEPRECATED_MSG("Use getPosition() instead.", glm::vec3& getPositionRef() );

	/// \brief Get the coordiantes of the ofRectangle's center as glm::vec3.
    ///
	/// \warning The z component of the returned glm::vec3 will always be 0.
    ///
    /// \returns The x and y coordinates of the center of the rectangle (z = 0).
	glm::vec3 getCenter() const;

    /// \brief Gets the x position of the ofRectangle as float.
    /// \returns The x position of the rectangle.
    float getX() const;

    /// \brief Gets the y position of the ofRectangle as float.
    /// \returns the y position of the rectangle.
    float getY() const;

    /// \brief Gets the width of the ofRectangle as float.
    /// \returns The width of the rectangle.
    float getWidth() const;

    /// \brief Gets the height of the ofRectangle as float.
    /// \returns The height of the rectangle.
    float getHeight() const;

    
    /// \brief Maps a normalized coordinate into this rectangle
    /// 
    /// Normalized coordinates range from [0,1] inclusive. It is used to define a ratio
    /// between the coordinates and a unitary rectangle. This ratio is mapped into
    /// this rectangle to scale to real values. If normalized coordinates are out of bounds,
    /// output will be scaled accordingly.
    ///
    /// \param coeff Normalized coordinate to map to this rectangle
    /// \returns The mapped coordinate
    glm::vec2 map(const glm::vec2 & coeff) const;
    
    /// \brief Maps a normalized coordinate into this rectangle
    /// 
    /// Normalized rectangles' position, width and height range from [0,1] inclusive.
    /// It is used to define a ratio between the rectangle and a unitary rectangle.
    /// This ratio is mapped into this rectangle to scale to real values. 
    /// If normalized rectangle is out of bounds, output will be scaled accordingly.
    /// 
    /// \param coeff Normalized rectangle to map to this rectangle
    /// \returns The mapped coordinate
    ofRectangle map(const ofRectangle & coeff) const;
    
    /// \brief Maps a normalized coordinate into this rectangle, clamping if out of bounds
    /// 
    /// Normalized coordinates range from [0,1] inclusive. It is used to define a ratio
    /// between the coordinates and a unitary rectangle. This ratio is mapped into
    /// this rectangle to scale to real values. If normalized coordinates are out of bounds,
    /// output will be clamped to this rectangle's position, width and height.
    /// 
    /// \param coeff Normalized coordinate to map to this rectangle
    /// \returns The mapped coordinate, clamped
    glm::vec2 mapClamp(const glm::vec2 & coeff) const;
    
    /// \brief Maps a normalized rectangle to this rectangle, clamping if out of bounds
    /// 
    /// Normalized rectangles' position, width and height range from [0,1] inclusive.
    /// It is used to define a ratio between the rectangle and a unitary rectangle.
    /// This ratio is mapped into this rectangle to scale to real values. 
    /// If normalized rectangle is out of bounds, output will be clamped to this 
    /// rectangle's position, width and height.
    /// 
    /// \param coeff Normalized rectangle to map to this rectangle
    /// \returns The mapped rectangle
    ofRectangle mapClamp(const ofRectangle & coeff) const;
    
    /// \}

    /// \name Operators
    /// \{

    /// \brief Assignment operator.
    /// \param rect The rectangle to assign.
    /// \returns A reference to this rectangle.
    ofRectangle& operator = (const ofRectangle& rect);

    /// \brief Returns a new ofRectangle where the x and y positions of the
	/// rectangle are offset by the (x, y) coordinates of the glm::vec3.
    /// \param p The point to translate.
    /// \returns The translated ofRectangle.
	ofRectangle operator + (const glm::vec3& p);
	ofRectangle operator + (const glm::vec2& p);

	/// \brief Returns a new ofRectangle where the x and y-positions of the
	/// rectangle are offset by the (x, y) coordinates of the glm::vec3.
	/// \param p The point to translate.
	/// \returns The translated ofRectangle.
	ofRectangle operator - (const glm::vec3& p);
	ofRectangle operator - (const glm::vec2& p);

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


	/// \brief The (x,y) position of the ofRectangle as an glm::vec3.
    ///
    /// \warning The z-component of this position is preserved and can be used
    /// but all ofRectangle operations will ignore the z-component.
	glm::vec3 position{};

    /// \brief The x position of the ofRectangle.
    float& x;

    /// \brief The y position of the ofRectangle.
    float& y;

    /// \brief The width of the ofRectangle.
    float width;

    /// \brief The height of the ofRectangle.
    float height;

    /// \}
};

/// \cond INTERNAL
/// \warning The internal z component of the glm::vec3 is preserved even though it
/// is not used.
std::ostream& operator<<(std::ostream& os, const ofRectangle& rect);
/// \warning The internal z component of the glm::vec3 is preserved even though it
/// is not used.
std::istream& operator>>(std::istream& is, ofRectangle& rect);
/// \endcond
