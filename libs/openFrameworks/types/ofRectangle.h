#pragma once


#include "ofConstants.h"
#include "ofPoint.h"
#include "ofLog.h"


/// \brief A class representing a 2D rectangle.
///
/// ofRectangle is a simple container for describing the position and size of a
/// 2D rectangle.  Since the `width` and `height` variables can be negative, the
/// x/y position is not garanteed to represent the upper right hand corner.  For
/// example, two visually identical rectangles can be represented in the
/// following ways:
///
/// ~~~~~{.cpp}
///     ofRectangle myRect(0, 0, 100, 100);
/// ~~~~~
///
/// or
///
/// ~~~~~{.cpp}
///     ofRectangle myRect(100, 100, -100, -100);
/// ~~~~~
///
/// While both representations will yield the same visual results in all
/// openFrameworks renderers, the results of some method operations that modify
/// x, y, width and height (such as scaling) produce mathematically correct, but
/// visually different results for each of the above representations.
///
/// To avoid this ambiguity, users planning to should prefer "standardized"
/// rectangles.  "Standardized" rectangles are rectangles whose width >= 0 and
/// height >= 0.  The ofRectangle::standardize() method can be used to ensure
/// that the is in it the "standard" form.
class ofRectangle{
public:

    /// \name Constructor
    /// \{

    /// \brief Construct a rectangle with zero width and zero height at 0, 0.
    ofRectangle();

    /// \brief Construct a rectangle using location and size.
    ///
    /// To produce consistent results, users are encouraged to initialize
    /// rectangles in the standardized form with width >=0 and height >= 0.
    ///
    /// \param px The x location of the rectangle.
    /// \param py The y location of the rectangle.
    /// \param w The width of the rectangle.
    /// \param h The height of the rectangle.
    ofRectangle(float px, float py, float w, float h);

    /// \brief Construct a rectangle from a point and dimensions.
    ///
    /// To produce consistent results, users are encouraged to initialize
    /// rectangles in the standardized form with width >=0 and height >= 0.
    ///
    /// \param p The ofPoint representing the location of the rectangle.
    /// \param w The width of the rectangle.
    /// \param h The height of the rectangle.
    ofRectangle(const ofPoint& p, float w, float h);

    /// \brief Construct a rectangle by copying another rectangle.
    ///
    /// To produce consistent results, users are encouraged to initialize
    /// rectangles in the standardized form with width >=0 and height >= 0.
    ///
    /// \param rect The rectangle to copy.
    ofRectangle(const ofRectangle& rect);

    /// \brief Construct a rectangle by defining two corners.
    /// \param p0 The ofPoint representing the upper left hand corner.
    /// \param p1 The ofPoint representing the lower right hand corner.
    ofRectangle(const ofPoint& p0, const ofPoint& p1);

    /// \brief Destroy the rectangle.
    virtual ~ofRectangle();

    /// \}

    /// \name Setters
    /// \{

    /// \brief Set the position and size of a rectangle.
    ///
    /// To produce consistent results, users are encouraged to initialize
    /// rectangles in the standardized form with width >=0 and height >= 0.
    ///
    /// \param px The x location of the rectangle.
    /// \param py The y location of the rectangle.
    /// \param w The width of the rectangle.
    /// \param h The height of the rectangle.
    void set(float px, float py, float w, float h);

    /// \brief Set the position and size of a rectangle.
    ///
    /// To produce consistent results, users are encouraged to initialize
    /// rectangles in the standardized form with width >=0 and height >= 0.
    ///
    /// \param p The ofPoint representing the location of the rectangle.
    /// \param w The width of the rectangle.
    /// \param h The height of the rectangle.
    void set(const ofPoint& p, float w, float h);

    /// \brief Set the position and size of a rectangle by defining two corners.
    ///
    /// \param rect The rectangle to copy.
    void set(const ofRectangle& rect);

    /// \brief Set the position and size of a rectangle using corners.
    /// \param p0 The ofPoint representing the upper left hand corner.
    /// \param p1 The ofPoint representing the lower right hand corner.
    void set(const ofPoint& p0, const ofPoint& p1);

    /// \brief Set the x position of a rectangle.
    /// \param px The x position.
    void setX(float px);

    /// \brief Set the y position of a rectangle.
    /// \param py The y position.
    void setY(float py);
    
    /// \brief Set the width of a rectangle.
    /// \param w The width.
    void setWidth(float w);

    /// \brief Set the height of a rectangle.
    /// \param h The height.
    void setHeight(float h);

    /// \brief Set the position of a rectangle.
    /// \param px The x position.
    /// \param py The y position.
    void setPosition(float px, float py);

    /// \brief Set the position of a rectangle.
    /// \param p The position as an ofPoint.
    void setPosition(const ofPoint& p);

    /// \brief Set position and size of a rectangle from the center.
    ///
    /// The center of the rectangle is defined and the width and height grow
    /// out around the center.
    ///
    /// \param px The x position of the rectangle's center.
    /// \param py The y position of the rectangle's center.
    /// \param w The width of the rectangle.
    /// \param h The height of the rectangle.
    void setFromCenter(float px, float py, float w, float h);

    /// \brief Set position and size of a rectangle from the center.
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
    /// \param dp The amount to translate as an ofPoint.
    void translate(const ofPoint& dp);
    
    /// \brief Translate the x position of the rectangle.
    /// \param dx the amount to translate on the x-axis.
    void translateX(float dx);

    /// \brief Translate the y position of the rectangle.
    /// \param dy the amount to translate on the y-axis.
    void translateY(float dy);

    /// \brief Scale the rectangle.
    ///
    /// Scaling will scale the width and the height, but will not change the
    /// position.
    ///
    /// \param s scaling factor.
    void scale(float s);

    /// \brief Scale the rectangle.
    ///
    /// Scaling will scale the width and the height, but will not change the
    /// position.
    ///
    /// \param sX the x scaling factor.  The new width = sX * width.
    /// \param sY the y scaling factor.  The new height = sY * height.
    void scale(float sX, float sY);

    /// \brief Scale the rectangle.
    ///
    /// Scaling will scale the width and the height, but will not change the position.
    ///
    /// \param s Scales x and y.  The new width = s.x * width and the new height
    ///        = s.y * height.
    void scale(const ofPoint& s);
    
    /// \brief Scale the width of the rectangle.
    ///
    /// Scaling will scale the width, but will not change the
    /// position and height.
    ///
    /// \param sX scaling factor.
    void scaleWidth(float  sX);
    
    /// \brief Scale the height of the rectangle.
    ///
    /// Scaling will scale the height, but will not change the
    /// position and widtg.
    ///
    /// \param sY scaling factor.
    void scaleHeight(float sY);
    
    /// \brief Scales both the width and height the ofRectangle from its center
    /// point.
    /// 
    /// The center point of the rectangle will remain fixed and the width,
    /// height, x, and y will be adjusted.
    ///
    /// \param s scaling factor.    
    void scaleFromCenter(float s);    

    /// \brief Scales both the width and height the ofRectangle from its center
    /// point.
    /// 
    /// The center point of the rectangle will remain fixed and the width,
    /// height, x, and y will be adjusted.
    ///
    /// \param sX the x scaling factor.  The new width = sX * width.
    /// \param sY the y scaling factor.  The new height = sY * height.
    void scaleFromCenter(float sX, float sY);

    /// \brief Scales both the width and height the ofRectangle from its center
    /// point.
    /// 
    /// The center point of the rectangle will remain fixed and the width,
    /// height, x, and y will be adjusted.
    ///
    /// \param s Scales x and y.  The new width = s.x * width and the new height
    ///        = s.y * height.
    void scaleFromCenter(const ofPoint& s);

    /// \brief Adjusts the ofRectangle to match the passed-in ofRectangle using the
    /// passed-in ofScaleMode.
    /// 
    /// Assuming the ofRectangle as the Subject and the passed-in ofRectangle as 
    /// the Target:
    /// 
    /// __OF_SCALEMODE_FIT__:
    ///
    /// This centers the Subject rectangle within the Target rectangle, and resizes
    /// the Subject rectangle to completely fit within the Target Rectangle.
    /// 
    /// - It fits the Subject rectangle inside the target rectangle.
    /// - It preserves Subject's aspect ratio.
    /// - The Subject's final area <= the Target's area.
    /// - The Subject's center == the Target's center.
    /// 
    /// __OF_SCALEMODE_FILL__:
    ///
    /// This centers the Subject rectangle within the Target rectangle, and resizes
    /// the Subject rectangle to completely encompass the Target Rectangle.
    /// 
    /// - It fills the Target rectangle with the Subject rectangle.
    /// - It preserves the Subject's aspect ratio.
    /// - The Subject's Area >= the Target's area.
    /// - The Subject's center == the Target's center.
    /// 
    /// __OF_SCALEMODE_CENTER__:
    ///
    /// This centers the Subject rectangle within the Target rectangle, and does not
    /// modify the Subject's scale.
    /// 
    /// - It preserves the Subject's aspect ratio.
    /// - The Subject's area is unchanged.
    /// - The Subject's center == Target's center.
    /// 
    /// __OF_SCALEMODE_STRETCH_TO_FILL__:
    ///
    /// This simply modifies the Subject rectangle to match the Target rectangle's
    /// dimensions.
    /// 
    /// - It can change the Subject's aspect ratio.
    /// - The Subject's area == the Target's area.
    /// - The Subject's center == the Target's center.
    /// 
    /// For a working example of the various ofScaleModes, see the
    /// *graphics/rectangleAlignmentAndScaling/* example within the examples
    /// directory.
    void scaleTo(const ofRectangle& targetRect,
                 ofScaleMode scaleMode = OF_SCALEMODE_FIT);

    /// \brief Adjusts the ofRectangle to match the passed-in ofRectangle.  It will use the
    /// ofAspectRatioMode to scale the ofRectangle, and will use the use the alignment
    /// anchor parameters to position the rectangle.
    /// 
    /// This is a convenience method when both the target and subject rectangles are
    /// using the same alignment anchors.  For a more complete explanation, see below.
    /// For a working example of how to use ofAspectRatioMode, ofAlignVert, and
    /// ofAlignHorz, see the *graphics/rectangleAlignmentAndScaling/* example within
    /// the examples directory.

    void scaleTo(const ofRectangle& targetRect,
                 ofAspectRatioMode subjectAspectRatioMode,
                 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
                 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);
    
    /// Adjusts the ofRectangle to match the passed-in ofRectangle.  It will
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
    /// For a working example of how to use ofAspectRatioMode, ofAlignVert,
    /// and ofAlignHorz, see the *graphics/rectangleAlignmentAndScaling/*
    /// example within the examples directory.
    void scaleTo(const ofRectangle& targetRect,
                 ofAspectRatioMode subjectAspectRatioMode,
                 ofAlignHorz modelHorzAnchor,
                 ofAlignVert modelVertAnchor,
                 ofAlignHorz subjectHorzAnchor,
                 ofAlignVert subjectVertAnchor);

    /// \}

    /// \name Alignment
    /// \{


    /// \brief Aligns the horizontal position of the ofRectangle to the given x position
    /// using an ofAlignHorz constant. If a constant is not passed in, this will
    /// align the center of the rectangle.
    /// 
    /// __ofAlignHorz Constants:__
    /// 
    /// - `OF_ALIGN_HORZ_IGNORE` : Does not perform any horizontal alignment.
    /// - `OF_ALIGN_HORZ_LEFT`   : Uses the left edge of the rectangle to horizontally anchor the alignment.
    /// - `OF_ALIGN_HORZ_RIGHT`  : Uses the right edge of the rectangle to horizontally anchor the alignment.
    /// - `OF_ALIGN_HORZ_CENTER` : Uses the center of the rectangle to horizontally anchor the alignment.
    /// 
    /// For a working example of how to use ofAlignHorz, see the
    /// *graphics/rectangleAlignmentAndScaling/* example within the examples
    /// directory.
    void alignToHorz(const float& targetX,
                     ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER);

    /// \brief Aligns the horizontal position of the ofRectangle to that of the passed-in
    /// ofRectangle.  Can take an ofAlignHorz constant.
    /// 
    /// If a constant is not passed in, this will align the horizontal centers of
    /// the rectangles.
    /// 
    /// If a constant is passed in, it will use that constant for both rectangles.
    void alignToHorz(const ofRectangle& targetRect,
                     ofAlignHorz sharedAnchor = OF_ALIGN_HORZ_CENTER);

    
    /// \brief Aligns the horizontal position of the ofRectangle to that of the the
    /// passed-in ofRectangle.  Takes two ofAlignHorz constants.
    /// 
    /// Will align the chosen anchor in the ofRectangle with the chosen anchor in
    /// the passed-in ofRectangle.
    void alignToHorz(const ofRectangle& targetRect,
                     ofAlignHorz targetHorzAnchor,
                     ofAlignHorz thisHorzAnchor);

    /// \brief Aligns the vertical position of the ofRectangle to the given x
    /// position using an ofAlignVert constant.
    ///    
    /// If a constant is not passed in, this will align the center of the
    /// rectangle.
    ///    
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
    /// directory.
    void alignToVert(const float& targetY,
                     ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);

    /// \brief Aligns the vertical position of the ofRectangle to that of the passed-
    /// in ofRectangle. Can take an ofAlignVert constant.
    /// 
    /// If a constant is not passed in, this will align the vertical centers
    /// of the rectangles.
    /// 
    /// If a constant is passed in, it will use that constant for both rectangles.
    void alignToVert(const ofRectangle& targetRect,
                     ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);


    /// \brief Aligns the vertical position of the ofRectangle to that of the the
    /// passed-in ofRectangle.  Takes two ofAlignVert constants.
    ///    
    /// Will align the chosen anchor in the ofRectangle with the chosen anchor
    /// in the passed-in ofRectangle.    
    void alignToVert(const ofRectangle& targetRect,
                     ofAlignVert targetVertAnchor,
                     ofAlignVert thisVertAnchor);

    /// \brief Aligns the position of the ofRectangle to the given point using an
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
    void alignTo(const ofPoint& targetPoint,
                ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER,
                ofAlignVert thisVertAnchor = OF_ALIGN_VERT_CENTER);

    /// \brief Aligns the position of the ofRectangle to that of the passed-in
    /// ofRectangle.  Can take an ofAlignHorz constant and an ofAlignVert
    /// constant.    
    /// If neither constant is passed in, this will align the centers of the rectangles.
    /// If constants are passed in, it will use those constants for both rectangles.
    void alignTo(const ofRectangle& targetRect,
                 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
                 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);


    /// Aligns the position of the ofRectangle to that of the passed-in
    /// ofRectangle.  Takes two ofAlignHorz constants and two ofAlignVert
    /// constants.    
    ///
    /// Will align the chosen anchors in the ofRectangle with the chosen
    /// anchors in the passed-in ofRectangle.    
    void alignTo(const ofRectangle& targetRect,
                 ofAlignHorz targetHorzAnchor,
                 ofAlignVert targetVertAnchor,
                 ofAlignHorz thisHorzAnchor,
                 ofAlignVert thisVertAnchor);

    /// \}

    /// \name Intersection
    /// \{

    /// \brief Determines if the passed (x,y) coordinates are within the ofRectangle
    /// or not.
    ///
    /// Note that coordinates on the edge of the ofRectangle are not
    /// considered within the rectangle and will return false.
    ///
    /// \returns `true` if the coordinates are inside the ofRectangle, `false` otherwise.
    bool inside(float px, float py) const;
    
    /// \brief Determines if the passed ofPoint is within the ofRectangle or not.
    ///
    /// Note that points on the edge of the ofRectangle are not
    /// considered within the rectangle and will return false.
    ///
    /// \returns `true` if the point is inside the ofRectangle, `false` otherwise.
    bool inside(const ofPoint& p) const;
    
    /// \brief Determines if a passed ofRectangle is completely 
    /// within the ofRectangle or not.
    ///
    /// Note that rectangles that share an edge with the ofRectangle are not
    /// considered within the rectangle and will return false.
    ///
    /// \returns `true` if the passed rectangle is contained within the ofRectangle, 
    /// `false` otherwise.
    bool inside(const ofRectangle& rect) const;
    
    /// \brief Determines if both of the passed ofPoints are within the ofRectangle or not.
    /// Typically used for determining if a line segment is inside the rectangle.
    /// 
    /// Note that points on the edge of the ofRectangle are not considered
    /// within the rectangle and will return false.
    ///
    /// \returns `true` if both points are inside the rectangle, `false` otherwise.
    bool inside(const ofPoint& p0, const ofPoint& p1) const;

    /// \brief Determines if a passed ofRectangle is intersecting with the ofRectangle
    /// 
    /// Rectangles that only share an edge and do not intersect otherwise are
    /// not considered to intersect and will return false.
    ///
    /// \returns `true` if the area contained within the ofRectangle overlaps
    /// with the area contained within the passed ofRectangle,    
    /// returns `false` otherwise.
    bool intersects(const ofRectangle& rect) const;

    /// \brief Determines if a line defined by two points is intersecting with
    /// the ofRectangle
    /// 
    /// \returns `true` if the line segment defined by the two passed ofPoints
    /// either crosses the perimeter of the ofRectangle or is completely
    /// contained within.
    /// returns `false` otherwise.
    bool intersects(const ofPoint& p0, const ofPoint& p1) const;
    
    /// \brief Modify the ofRectangle so that the `x` and `y` coordinates given are both
    /// enclosed within the rectangle.
    ///
    /// This will potentially change the width, height, x position, and y
    /// position of the ofRectangle.
    void growToInclude(float px, float py);
    
    /// \brief Modify the ofRectangle so that the ofPoint given is enclosed within
    /// the rectangle.
    ///    
    /// This will potentially change the width, height, x position, and y
    /// position of the ofRectangle.
    void growToInclude(const ofPoint& p);
    
    /// Expand the ofRectangle so that the ofRectangle passed into the method
    /// is entirely enclosed by the current ofRectangle.    
    /// 
    /// This will potentially change the width, height, x position, and y
    /// position of the current ofRectangle, but will not modify the passed
    /// ofRectangle.
    void growToInclude(const ofRectangle& rect);
    
    /// \brief Modify the ofRectangle so that both the ofPoints given 
    /// are enclosed within the rectangle.
    /// 
    /// This will potentially change the width, height, x position, and y position
    /// of the ofRectangle.
    void growToInclude(const ofPoint& p0, const ofPoint& p1);

    /// \brief Return a new ofRectangle containing the area shared between the
    /// ofRectangle and the passed ofRectangle.
    /// 
    /// If the two rectangles do not overlap, return a rectangle located at
    /// (0,0) with 0 height and 0 width.
    ///    
    /// If the two rectangles only share an edge this will return a rectangle
    /// positioned on that edge:
    ///    
    /// If the shared edge is vertical, the rectangle will have zero width,
    /// otherwise it will have zero height.
    ofRectangle getIntersection(const ofRectangle& rect) const;

    /// \brief Returns a new ofRectangle whose area contains both the area of the
    /// ofRectangle and that of the passed ofRectangle.
    ofRectangle getUnion(const ofRectangle& rect) const;
    
    /// \}

    /// \name Standardization 
    /// \{

    /// \brief Standardize the rectangle
    /// 
    /// ofRectangle is a simple container for describing the position
    /// and size of 2D rectangles. Like many rectangle data structures
    /// found in other frameworks and graphics libraries, member
    /// width and height variables can take negative values.
    /// Additionally, x / y position and width / height are publicly
    /// accessible, resulting in great programming freedom.
    /// Consequently, two visually identical rectangles can be
    /// represented in the following ways:
    /// 
    /// ~~~~{.cpp}
    /// ofRectangle myRect(0,0,100,100);
    /// // OR
    /// ofRectangle myRect(100,100,-100,-100);
    /// ~~~~
    /// 
    /// While both representations will yield the same visual
    /// results in all openFrameworks renderers, the results of
    /// some method operations that modify x / y / width / height
    /// (such as scaling) produce mathematically correct, but
    /// visually different results for each of the above
    /// representations.
    /// 
    /// "Standardized" rectangles are rectangles whose width >= 0 and height >= 0.
    /// This method can be used to ensure that the rectangle is "standardized".
    /// If the rectangle is non-standard, it will modify the x / width and y / height
    /// values into their respective standardized versions.
    void standardize();
    
    /// \brief If the ofRectangle is standardized, it returns itself.  
    /// Otherwise it returns a standardized copy of itself.
    /// 
    /// \sa For more information about standardized rectangles, see the discussion at
    /// standardize().
    ofRectangle getStandardized() const;

    /// \brief Returns true if this ofRectangle is in the standardized form; 
    /// that its width and height are both positive.
    /// 
    /// \sa For a discussion of the standardized form, see standardize()
    bool isStandardized() const;

    /// \}

    /// \name Getters
    /// \{
    
    /// \brief Gets the area of the ofRectangle as a float.
    /// This is the product of the width and height.
    /// 
    /// ~~~~{.cpp}
    /// // Create a rectangle that is 100 units wide and 200 units tall
    /// ofRectangle myRect(0,0,100,200);
    /// 
    /// // Get the area of that rectangle
    /// float areaValue = myRect.getArea();
    /// 
    /// // areaValue will be 20000.0.
    /// ~~~~
    /// 
    /// \returns The area of the rectangle
    float getArea() const;
    

    /// \brief Gets the perimeter of the ofRectangle as a float.
    /// This is the sum of the lengths of the sides.
    /// 
    /// ~~~~{.cpp}
    /// // Create a rectangle that is 100 units wide and 200 units tall
    /// ofRectangle myRect(0,0,100,200);
    /// 
    /// // Get the perimeter of that rectangle
    /// float perimeterValue = myRect.getPerimeter();
    /// 
    /// // perimeterValue will be 600.0.
    /// ~~~~
    float getPerimeter() const;

    /// \brief Gets the ratio of width to height of the ofRectangle as a
    /// float.    
    /// 
    /// ~~~~{.cpp}
    /// // Create a rectangle that is 100 units wide and 200 units tall
    /// ofRectangle myRect(0,0,100,200);
    /// 
    /// // Get the aspect ratio of that rectangle
    /// float aspectRatioValue = myRect.getAspectRatio();
    /// 
    /// // aspectRatioValue will be 0.5.
    /// ~~~~    
    /// \returns The aspect ratio of the rectangle
    float getAspectRatio() const;
    
    /// \brief Determines if the ofRectangle's width and height are 0
    /// 
    /// \returns true if both the rectangle's width and height are 0, false if
    /// either is non-zero.    
    bool isEmpty() const;  
    
    /// \brief Gets the (x,y) coordinates of the corner of the rectangle with the smallest
    /// x and y position as an ofPoint.
    /// 
    /// \sa Equivalent to getTopLeft().
    ofPoint getMin() const;
    
    /// \brief Gets the (x,y) coordinates of the corner of the rectangle with the
    /// largest x and largest y position as an ofPoint.
    /// 
    /// \sa Equivalent to getBottomRight().
    ofPoint getMax() const;
    
    /// \brief Gets the position of the smaller of the two sides of the rectangle
    /// perpendicular to the x axis as a float.
    /// 
    /// \sa Equivalent to getLeft().    
    float getMinX() const;

    /// \brief Gets the position of the larger of the two sides of the
    /// rectangle perpendicular to the x axis as a float.    
    /// 
    /// \sa Equivalent to getRight().
    float getMaxX() const;
    
    /// \brief Returns the position of the smaller of the two sides of the rectangle
    /// perpendicular to the y axis as a float.
    /// 
    /// \sa Equivalent to getTop().
    float getMinY() const;
    
    /// \brief Gets the position of the larger of the two sides of the rectangle
    /// perpendicular to the y axis as a float.    
    /// 
    /// Equivalent to getBottom().
    float getMaxY() const;
    
    /// \brief Gets the position of the left of the ofRectangle as a float.
    ///
    /// \sa Equivalent to getMinX().
    float getLeft()   const;
    
    /// \brief Gets the position of the right of the ofRectangle as a float.
    /// 
    /// \sa Equivalent to getMaxX().
    float getRight()  const;
    
    /// \brief Returns the position of the top of the ofRectangle as a float.
    /// 
    /// \sa Equivalent to getMinY().
    float getTop()    const;
    
    /// \brief Gets the position of the bottom of the ofRectangle as a float.
    /// 
    /// \sa Equivalent to getMaxY()
    float getBottom() const;
    
    /// \brief Gets the (x,y) coordinates of the top left corner of the ofRectangle 
    /// as an ofPoint.
    /// 
    /// \sa Equivalent to getMin().    
    ofPoint getTopLeft() const;
    
    /// \brief Gets the (x,y) coordinates of the top right corner of the rectangle
    /// as an ofPoint.
    ofPoint getTopRight() const;
    
    /// \brief Gets the (x,y) coordinates of the bottom left corner of the
    /// rectangle as an ofPoint.    
    ofPoint getBottomLeft() const;

    /// \brief Gets the (x,y) coordinates of the bottom right corner of the
    /// rectangle as an ofPoint.    
    /// 
    /// \sa Equivalent to getMax().
    ofPoint getBottomRight() const;
    
    /// \brief A convenience method that returns the value of one of
    /// the horizontal edges of the ofRectangle using the `ofAlignHorz` enum.    
    /// 
    /// Possible anchor values are:
    /// 
    /// - `OF_ALIGN_HORZ_IGNORE`  : returns 0.0
    /// - `OF_ALIGN_HORZ_LEFT`    : returns the position of the left edge, equivalent to ofRectangle::geLeft().
    /// - `OF_ALIGN_HORZ_RIGHT`   : returns the position of the right edge, equivalent to ofRectangle::getRight().
    /// - `OF_ALIGN_HORZ_CENTER`  : returns the x position of the center of the ofRectangle.
    /// 
    /// Any other anchor value will return `0.0`.
    float getHorzAnchor(ofAlignHorz anchor) const;

    /// \brief A convenience method that returns the value of one of the
    /// vertical edges of the ofRectangle using the ofAlignVert enum.
    /// 
    /// Possible anchor values are:
    /// 
    /// - `OF_ALIGN_VERT_IGNORE`  : returns 0.0
    /// - `OF_ALIGN_VERT_TOP`     : returns the position of the upper edge, equivalent to ofRectangle::getTop().
    /// - `OF_ALIGN_VERT_BOTTOM`  : returns the position of the bottom edge, equivalent to ofRectangle::getBottom().
    /// - `OF_ALIGN_VERT_CENTER`  : returns the y position of the center of the ofRectangle.
    /// 
    /// Any other anchor value will return `0.0`.
    float getVertAnchor(ofAlignVert anchor) const;
    
    /// \brief Gets the (x,y) position of the ofRectangle as an ofPoint.
    /// 
    /// \sa Equivalent to getPositionRef().    
    ofPoint  getPosition() const;
    
    /// \brief Gets the current (x,y) position of the ofRectangle as an ofPoint.
    /// 
    /// \sa Equivalent to getPosition().
    ofPoint& getPositionRef();

    /// \brief Gets the (x,y) coordinates of the center of the rectangle as an
    /// ofPoint.
    ofPoint getCenter() const;
    
    /// \brief Gets the x position of the ofRectangle as a float.
    float getX() const;
    
    /// \brief Gets the y position of the ofRectangle as a float.
    float getY() const;
    
    /// \brief Gets the width of the ofRectangle as a float.
    float getWidth() const;

    /// \brief Gets the height of the ofRectangle as a float.
    float getHeight() const;

    /// \}

    /// \name Operators
    /// \{

    ofRectangle& operator = (const ofRectangle& rect);

    /// \brief Returns a new ofRectangle where the x and y positions of the
    /// rectangle are offset by the (x,y) coordinates of the ofPoint.    
    ofRectangle operator + (const ofPoint& p);
    
    /// \brief If both ofRectangles have the same x, y, width, and height,
    /// they are considered equal.    
    bool operator == (const ofRectangle& rect) const;

    /// \brief If the two ofRectangles differ in x, y, width, or height, they
    /// are considered unequal.
    bool operator != (const ofRectangle& rect) const;

    /// \}

    /// \name Properties
    /// \{

    
    /// \brief The (x,y) position of the ofRectangle as an ofPoint.
    ofPoint position;

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
ostream& operator<<(ostream& os, const ofRectangle& rect);
istream& operator>>(istream& is, ofRectangle& rect);
/// \endcond

