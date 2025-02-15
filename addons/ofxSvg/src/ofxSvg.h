//
//  ofxSvgParser.h
//
//  Created by Nick Hardeman on 8/31/24.
//

#pragma once
#include "ofxSvgGroup.h"
#include "ofXml.h"
#include "ofxSvgCss.h"
#include <stack>

/// \file
/// ofxSVG is used for loading, manipulating, rendering and saving of SVG files.

//namespace ofx::svg {
class ofxSvg : public ofx::svg::Group {
public:
	
	virtual ofx::svg::SvgType getType() override {return ofx::svg::TYPE_DOCUMENT;}
	
	/// \brief Loads an SVG file from the provided filename.
	/// \return true if the load was successful.
	bool load( const of::filesystem::path & fileName );
	/// \brief provided for legacy support.
	/// \return true if the load was successful.
	bool loadFromString( const std::string& data, std::string url = "local");
	/// \brief Reload from filepath saved from a previouly called load().
	/// \return true if the reload was successful.
	bool reload();
	/// \brief Save the svg to the path.
	/// Use the add functions to provide data to be saved or load data using the load() function.
	/// \return true if the save was successful.
	bool save( of::filesystem::path apath );
	/// \brief Remove all of the data from the document.
	void clear();
	/// \brief Set the directory to search for fonts if using text elements.
	/// Needs to be set before calling load();
	/// \param aDir string representation of the directory.
	void setFontsDirectory( std::string aDir );
	
	/// \brief A string of the element hierarchy.
	/// Helpful for visualizing structure.
	std::string toString(int nlevel = 0) override;
	/// \brief Get the units, ie. px, cm, in, etc.
	/// \return string description of the units used.
	std::string getUnitString() { return mUnitStr; }
	/// \brief Set the units using a string, ie. px, cm, in, etc.
	void setUnitString(std::string astring ) { mUnitStr = astring; }
	/// \brief The total layers in the svg, should also be the number of groups + elements.
	/// \return int that is the total layers.
	const int getTotalLayers();
	/// \brief If an element has been added or removed, the function should be called internally.
	/// The function updates the 'layer' property on each element.
	/// If the layers appear to be incorrect, call this function.
	void recalculateLayers();
	/// \brief Provided for legacy support.
	/// Includes circles, ellipses, rectangles and paths as ofPaths.
	/// \return int as the number of paths in the document.
	int getNumPath();
	/// \brief Provided for legacy support. Use getNumPath() to acquire the total number of paths detected.
	/// \param n the index of the ofPath to return.
	/// \return An ofPath using the provided index.
	ofPath & getPathAt(int n);
	/// \brief Provided for legacy support.
	/// Includes circles, ellipses, rectangles and paths as ofPaths.
	/// \return A vector of ofPaths in the entire document.
	const std::vector<ofPath> & getPaths() const;
	/// \brief Parse the svg transform string into global position, scale and rotation.
	/// The elements pos, scale and rotation is set.
	/// \param aStr svg transform string.
	/// \param aele ofx::svg::Element to be updated.
	bool setTransformFromSvgMatrixString( std::string aStr, std::shared_ptr<Element> aele );
	/// \brief Return a string used to represent matrix transforms in svg
	/// The matrix can be represented as an array of values like :
	/// Or using individual components like tranform(translateX translateY), scale(scaleX scaleY) and rotate(degrees <optional>ptx <optional>pty )
	/// Skew is currently not supported.
	/// Reference: https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/transform
	/// \param aele the element to extract the svg matrix from using its pos, scale and rotation properties.
	/// \return string that represents the svg matrix for using in a svg file.
	std::string getSvgMatrixStringFromElement( std::shared_ptr<Element> aele );
	/// \brief The viewbox width.
	/// \return float width of the viewbox.
	float getWidth() const;
	/// \brief The viewbox height.
	/// \return float height of the viewbox.
	float getHeight() const;
	/// \brief The viewbox rectangle. The viewbox attribute on the svg node.
	/// \return ofRectangle with the dimenstions of the viewbox.
	ofRectangle getViewbox() const;
	/// \brief The bounds width.
	/// \return float width of the bounds.
	float getBoundsWidth() const;
	/// \brief The bounds height.
	/// \return float height of the bounds.
	float getBoundsHeight() const;
	/// \brief The bounds rectangle. The width and height attribute on the svg node.
	/// \return ofRectangle with the dimenstions of the bounds.
	ofRectangle getBounds() const;
	
	/// \brief Set the width of the viewbox.
	/// If the bounds width is less than 1, set the bounds to the same width.
	/// \param aw float width.
	void setWidth( float aw );
	/// \brief Set the height of the viewbox.
	/// If the bounds height is less than 1, set the bounds to the same height.
	/// \param ah float height.
	void setHeight( float ah );
	/// \brief Set the dimensions of the viewbox.
	/// \param arect an ofRectangle with the dimensions of the viewbox.
	void setViewBox( const ofRectangle& arect );
	/// \brief Set the width of the bounds.
	/// \param aw float width.
	void setBoundsWidth( float aw );
	/// \brief Set the height of the viewbox.
	/// \param ah float height.
	void setBoundsHeight( float ah );
	/// \brief Set the dimensions of the bounds.
	/// \param arect an ofRectangle with the dimensions of the bounds.
	void setBounds( const ofRectangle& arect );
	
	
	void pushGroup( const std::string& aname );
	void pushGroup( const std::shared_ptr<ofx::svg::Group>& agroup );
	void popGroup();
	
	void setFillColor(ofColor acolor);
	void setFilled(bool abFilled);

	void setStrokeColor(ofColor acolor);
	void setStrokeWidth(float aLineWidth);
	void setHasStroke(bool abStroke);
	
	void setCircleResolution( int ac ) { mCircleResolution = ac; }
	void setCurveResolution( int ac ) { mCurveResolution = ac; }
	int getCircleResolution() { return mCircleResolution; }
	int getCurveResolution() { return mCurveResolution; };
	
	ofx::svg::CssClass& getCurrentCss() { return mCurrentCss;}
	
	std::shared_ptr<ofx::svg::Group> addGroup(std::string aname);
	
	std::shared_ptr<ofx::svg::Path> add( const ofPath& apath );
	std::vector< std::shared_ptr<ofx::svg::Path> > add( const std::vector<ofPath>& apaths );
	
	std::shared_ptr<ofx::svg::Path> add( const ofPolyline& apoly );
	std::vector< std::shared_ptr<ofx::svg::Path> > add( const std::vector<ofPolyline>& apolys );
	
	std::shared_ptr<ofx::svg::Rectangle> add( const ofRectangle& arect );
	std::shared_ptr<ofx::svg::Rectangle> add( const ofRectangle& arect, float aRoundRadius );
	
	std::shared_ptr<ofx::svg::Circle> addCircle( float aradius );
	std::shared_ptr<ofx::svg::Circle> addCircle( const glm::vec2& apos, float aradius );
	std::shared_ptr<ofx::svg::Circle> addCircle( const glm::vec3& apos, float aradius );
	std::shared_ptr<ofx::svg::Circle> addCircle( const float& ax, const float& ay, float aradius );
	
	std::shared_ptr<ofx::svg::Ellipse> addEllipse( float aradiusX, float aradiusY );
	std::shared_ptr<ofx::svg::Ellipse> addEllipse( const glm::vec2& apos, float aradiusX, float aradiusY );
	std::shared_ptr<ofx::svg::Ellipse> addEllipse( const glm::vec3& apos, float aradiusX, float aradiusY );
	std::shared_ptr<ofx::svg::Ellipse> addEllipse( const float& ax, const float& ay, float aradiusX, float aradiusY );
	
	std::shared_ptr<ofx::svg::Image> addImage( const of::filesystem::path& apath, const ofTexture& atex );
	std::shared_ptr<ofx::svg::Image> addImage( const glm::vec2& apos, const of::filesystem::path& apath, const ofTexture& atex );
	
	// adapted from ofGLProgrammableRenderer for some sort of conformity
	void pushMatrix();
	bool popMatrix();
	void translate(float x, float y);
	void translate(const glm::vec2 & p);
	void scale(float xAmnt, float yAmnt);
	void rotateRadians(float radians);
	void rotateDegrees(float adegrees);
	void multMatrix(const glm::mat4 & m);
	void loadMatrix(const glm::mat4 & m);
	void loadIdentityMatrix();
	
	virtual void drawDebug();
	
protected:
	std::string fontsDirectory = "";
	std::string folderPath, svgPath;
	ofRectangle mViewbox;
	ofRectangle mBounds;
	void validateXmlSvgRoot( ofXml& aRootSvgNode );
	std::string cleanString( std::string aStr, std::string aReplace );
	void _parseXmlNode( ofXml& aParentNode, std::vector< std::shared_ptr<Element> >& aElements );
	bool _addElementFromXmlNode( ofXml& tnode, std::vector< std::shared_ptr<Element> >& aElements );
	
	void _parsePolylinePolygon( ofXml& tnode, std::shared_ptr<ofx::svg::Path> aSvgPath );
	// reference: https://www.w3.org/TR/SVG/paths.html
	void _parsePath( ofXml& tnode, std::shared_ptr<ofx::svg::Path> aSvgPath );
	
	ofx::svg::CssClass _parseStyle( ofXml& tnode );
	void _applyStyleToElement( ofXml& tnode, std::shared_ptr<Element> aEle );
	void _applyStyleToPath( ofXml& tnode, std::shared_ptr<ofx::svg::Path> aSvgPath );
	void _applyStyleToPath( ofx::svg::CssClass& aclass, std::shared_ptr<ofx::svg::Path> aSvgPath );
	void _applyStyleToText( ofXml& tnode, std::shared_ptr<ofx::svg::Text::TextSpan> aTextSpan );
	void _applyStyleToText( ofx::svg::CssClass& aclass, std::shared_ptr<ofx::svg::Text::TextSpan> aTextSpan );
	
	glm::vec3 _parseMatrixString(const std::string& input, const std::string& aprefix, bool abDefaultZero );
	
	std::shared_ptr<ofx::svg::Text::TextSpan> _getTextSpanFromXmlNode( ofXml& anode );
	
	ofx::svg::Group* _getPushedGroup();
	bool _hasPushedMatrix();
	void _applyModelMatrixToElement( std::shared_ptr<Element> aele, glm::vec2 aDefaultPos );
	glm::vec2 _getPos2d(const glm::mat4& amat);
	glm::vec2 _getScale2d(const glm::mat4& amat);
	float _getZRotationRadians( const glm::mat4& amat );
	
	ofx::svg::CssClass& _addCssClassFromPath( std::shared_ptr<ofx::svg::Path> aSvgPath );
	void _addCssClassFromPath( std::shared_ptr<ofx::svg::Path> aSvgPath, ofXml& anode );
	void _addCssClassFromImage( std::shared_ptr<ofx::svg::Image> aSvgImage, ofXml& anode );
	bool _toXml( ofXml& aParentNode, std::shared_ptr<ofx::svg::Element> aele );
	
	unsigned int mCurrentLayer = 0;
	
	std::string mUnitStr = "px";
	
	ofx::svg::CssStyleSheet mSvgCss;
	ofx::svg::CssClass mCurrentCss;
	ofColor mFillColor, mStrokeColor;
	
	std::vector< std::shared_ptr<ofx::svg::Group> > mGroupStack;
	
	std::shared_ptr<ofx::svg::CssClass> mCurrentSvgCss;
	
	std::vector< std::shared_ptr<Element> > mDefElements;
	
	// just used for debugging
	std::vector<glm::vec3> mCPoints;
	std::vector<glm::vec3> mCenterPoints;
	
	glm::mat4 mModelMatrix = glm::mat4(1.f);
	std::stack<glm::mat4> mModelMatrixStack;
	
	int mCircleResolution = 64;
	int mCurveResolution = 24;
	
	// for legacy purposes //
	static ofPath sDummyPath;
	mutable std::vector<ofPath> mPaths;
};

//}







