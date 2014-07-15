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
///     ofRectangle myRect(0, 0, 100, 100);
///
/// or
///
///     ofRectangle myRect(100, 100, -100, -100);
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
    /// \brief Construct a rectangle with zero width and zero height at 0, 0.
    ofRectangle();

    /// \brief Destroy the rectangle.
    virtual ~ofRectangle();

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
    /// \param p The ofPoint representing the upper left hand corner.
    /// \param p The ofPoint representing the lower right hand corner.
    ofRectangle(const ofPoint& p0, const ofPoint& p1);

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
    /// \param p The ofPoint representing the upper left hand corner.
    /// \param p The ofPoint representing the lower right hand corner.
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
    /// \param s sclaing factor.
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
    /// \param s Scales x and y.  The new width = sX * width and the new height
    ///        = sY * height.
    void scale(const ofPoint& s);
    
    void scaleWidth(float  sX);
    void scaleHeight(float sY);
    
    void scaleFromCenter(float s);
    void scaleFromCenter(float sX, float sY);
    void scaleFromCenter(const ofPoint& s);
    
    void scaleTo(const ofRectangle& targetRect,
                 ofScaleMode scaleMode = OF_SCALEMODE_FIT);

    void scaleTo(const ofRectangle& targetRect,
                 ofAspectRatioMode subjectAspectRatioMode,
                 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
                 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);

    void scaleTo(const ofRectangle& targetRect,
                 ofAspectRatioMode subjectAspectRatioMode,
                 ofAlignHorz modelHorzAnchor,
                 ofAlignVert modelVertAnchor,
                 ofAlignHorz subjectHorzAnchor,
                 ofAlignVert subjectVertAnchor);

    void alignToHorz(const float& targetX,
                     ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER);

    void alignToHorz(const ofRectangle& targetRect,
                     ofAlignHorz sharedAnchor = OF_ALIGN_HORZ_CENTER);

    void alignToHorz(const ofRectangle& targetRect,
                     ofAlignHorz targetHorzAnchor,
                     ofAlignHorz thisHorzAnchor);

    void alignToVert(const float& targetY,
                     ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);

    void alignToVert(const ofRectangle& targetRect,
                     ofAlignVert sharedAnchor = OF_ALIGN_VERT_CENTER);

    void alignToVert(const ofRectangle& targetRect,
                     ofAlignVert targetVertAnchor,
                     ofAlignVert thisVertAnchor);

    void alignTo(const ofPoint& targetPoint,
                ofAlignHorz thisHorzAnchor = OF_ALIGN_HORZ_CENTER,
                ofAlignVert thisVertAnchor = OF_ALIGN_VERT_CENTER);
    
    void alignTo(const ofRectangle& targetRect,
                 ofAlignHorz sharedHorzAnchor = OF_ALIGN_HORZ_CENTER,
                 ofAlignVert sharedVertAnchor = OF_ALIGN_VERT_CENTER);

    void alignTo(const ofRectangle& targetRect,
                 ofAlignHorz targetHorzAnchor,
                 ofAlignVert targetVertAnchor,
                 ofAlignHorz thisHorzAnchor,
                 ofAlignVert thisVertAnchor);

    bool inside(float px, float py) const;
    bool inside(const ofPoint& p) const;
    bool inside(const ofRectangle& rect) const;
    bool inside(const ofPoint& p0, const ofPoint& p1) const;

    bool intersects(const ofRectangle& rect) const;
    bool intersects(const ofPoint& p0, const ofPoint& p1) const;
    
    void growToInclude(float px, float py);
    void growToInclude(const ofPoint& p);
    void growToInclude(const ofRectangle& rect);
    void growToInclude(const ofPoint& p0, const ofPoint& p1);

    ofRectangle getIntersection(const ofRectangle& rect) const;

    ofRectangle getUnion(const ofRectangle& rect) const;
    
    void standardize();
    ofRectangle getStandardized() const;
    bool isStandardized() const;  // are width/height >= 0.0f
    
    float getArea() const;
    float getPerimeter() const;
    float getAspectRatio() const;
    
    bool isEmpty() const;     // are width/height == 0.0f
    
    ofPoint getMin() const;
    ofPoint getMax() const;
    
    float getMinX() const;
    float getMaxX() const;
    float getMinY() const;
    float getMaxY() const;
    
    float getLeft()   const;
    float getRight()  const;
    float getTop()    const;
    float getBottom() const;
    
    ofPoint getTopLeft() const;
    ofPoint getTopRight() const;
    ofPoint getBottomLeft() const;
    ofPoint getBottomRight() const;
    
    float getHorzAnchor(ofAlignHorz anchor) const;
    float getVertAnchor(ofAlignVert anchor) const;
    
    ofPoint  getPosition() const;
    ofPoint& getPositionRef();

    ofPoint getCenter() const;
    
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    ofRectangle& operator = (const ofRectangle& rect);
    ofRectangle operator + (const ofPoint& p);
    
    bool operator == (const ofRectangle& rect) const;
    bool operator != (const ofRectangle& rect) const;
    
    ofPoint position;
    float& x;
    float& y;
    float width;
    float height;
};

ostream& operator<<(ostream& os, const ofRectangle& rect);
istream& operator>>(istream& is, ofRectangle& rect);
