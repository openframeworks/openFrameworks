#pragma once
#include "ofxSvgGroup.h"
#include "ofXml.h"
#include "ofxSvgCss.h"
#include <stack>

/// \file
/// ofxSvg is used for loading, manipulating, rendering and saving of SVG files.
/// Based on this spec: https://www.w3.org/TR/SVG/Overview.html

class ofxSvg : public ofxSvgGroup {
protected:
	std::vector<std::shared_ptr<ofxSvgElement>> deepCopyVector(const std::vector<std::shared_ptr<ofxSvgElement>>& original);
	void deepCopyFrom( const ofxSvg & mom );
	void moveFrom( ofxSvg&& mom );
	
public:
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_DOCUMENT;}
	
	// Default constructor
	ofxSvg() = default;
	// Copy constructor (deep copy)
	ofxSvg(const ofxSvg & mom);
	// Copy assignment operator (deep copy)
	ofxSvg& operator=(const ofxSvg& mom);
	// Move constructor
	ofxSvg(ofxSvg&& mom);
	// Move assignment operator
	ofxSvg& operator=(ofxSvg&& mom);
	
	~ofxSvg() = default;
	
	ofxSvg(const of::filesystem::path & fileName);
	
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
	glm::mat4 setTransformFromSvgMatrixString( std::string aStr, std::shared_ptr<ofxSvgElement> aele );
	/// \brief Return a string used to represent matrix transforms in svg
	/// The matrix can be represented as an array of values like :
	/// Or using individual components like tranform(translateX translateY), scale(scaleX scaleY) and rotate(degrees <optional>ptx <optional>pty )
	/// Skew is currently not supported.
	/// Reference: https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/transform
	/// \param aele the element to extract the svg matrix from using its pos, scale and rotation properties.
	/// \return string that represents the svg matrix for using in a svg file.
	std::string getSvgMatrixStringFromElement( std::shared_ptr<ofxSvgElement> aele );
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
	
	/// \brief Push a svg group to be active for adding elements to.
	/// \param aname The path to the svg group in the document, if a group is already pushed, search in that group.
	void pushGroup( const std::string& aname );
	/// \brief Push a svg group to be active for adding elements to.
	/// \param aname The group to push and make active.
	void pushGroup( const std::shared_ptr<ofxSvgGroup>& agroup );
	/// \brief Remove the most recent pushed group if any.
	void popGroup();
	/// \brief Set the current fill color. Any subsequent items using a fill color will adopt this color.
	/// \param acolor is the color to set.
	void setFillColor(ofColor acolor);
	/// \brief Set if items should be filled or not. Any subsequent added items will use this value.
	/// \param abFilled should the items be filled or not.
	void setFilled(bool abFilled);
	/// \brief Set the current stroke color. Any subsequent items using a stroke color will adopt this color.
	/// \param acolor is the color to set.
	void setStrokeColor(ofColor acolor);
	/// \brief Set the current stroke width. Any subsequent items using a stroke width will adopt this value.
	/// \param aLineWidth is the width of the lines.
	void setStrokeWidth(float aLineWidth);
	/// \brief Set if items should have a stroke or not. Any subsequent items using a stroke will adopt this value.
	/// \param abStroke activates or deactivates strokes.
	void setHasStroke(bool abStroke);
	/// \brief Set the circle resolution for rendering.
	/// Set this value before calling load.
	/// \param ac (int) the resolution to use.
	void setCircleResolution( int ac ) { mCircleResolution = ac; }
	/// \brief Set the curve resolution for rendering.
	/// Set this value before calling load.
	/// \param ac (int) the resolution to use.
	void setCurveResolution( int ac ) { mCurveResolution = ac; }
	/// \brief Get the circle resolution for rendering.
	/// \return int of the circle resolution.
	int getCircleResolution() { return mCircleResolution; }
	/// \brief Get the curve resolution for rendering.
	/// \return int of the circle resolution.
	int getCurveResolution() { return mCurveResolution; };
	/// \brief Get the current css used for items.
	/// \return ofxSvgCssClass.
	ofxSvgCssClass& getCurrentCss() { return mCurrentCss;}
	/// \brief Set the default winding mode of imported paths with all sub paths that are closed. The default is OF_POLY_WINDING_NONZERO.
	/// If not all of the sub paths are closed, the winding mode is not set.
	/// Must be called before load() to apply to newly imported paths.
	/// \param ofPolyWindingMode to be used as default.
	void setDefaultClosedPathWindingMode( ofPolyWindingMode aWindingMode );
	/// \brief Get the default winding mode of imported paths.
	/// \return ofPolyWindingMode.
	ofPolyWindingMode getDefaultClosedPathWindingMode();
	
	ofxSvgCssStyleSheet& getCssStyleSheet() {return mSvgCss; }
	/// \brief Add a ofxSvgGroup to the document. This will also push back the group as current.
	/// \param aname is the name of the group.
	/// \return std::shared_ptr<ofxSvgGroup> as the group that was created.
	std::shared_ptr<ofxSvgGroup> addGroup(std::string aname);
	
	/// \brief Add a ofxSvgPath to the document. The ofxSvgPath will be added to the current group.
	/// \param ofPath apath is the path to be added.
	/// \return std::shared_ptr<ofxSvgPath> as the path that was created and added to the document.
	std::shared_ptr<ofxSvgPath> add( const ofPath& apath );
	/// \brief Add multiple ofxSvgPaths to the document. The ofxSvgPaths will be added to the current group.
	/// \param std::vector<ofPath> apaths are the paths to be added.
	/// \return std::vector< std::shared_ptr<ofxSvgPath> > the paths that were created and added to the document.
	std::vector< std::shared_ptr<ofxSvgPath> > add( const std::vector<ofPath>& apaths );
	/// \brief Add a ofxSvgPath to the document. The ofxSvgPath will be added to the current group.
	/// \param ofPolyline apoly is the polyline that will create an ofxSvgPath and added.
	/// \return std::shared_ptr<ofxSvgPath> as the path that was created and added to the document.
	std::shared_ptr<ofxSvgPath> add( const ofPolyline& apoly );
	/// \brief Add multiple ofxSvgPaths to the document. The ofxSvgPaths will be added to the current group.
	/// \param std::vector<ofPolyline> apolys are the polylines used to create and add ofxSvgPaths to the document.
	/// \return std::vector< std::shared_ptr<ofxSvgPath> > the paths that were created and added to the document.
	std::vector< std::shared_ptr<ofxSvgPath> > add( const std::vector<ofPolyline>& apolys );
	
	/// \brief Add a ofxSvgRectangle to the document. The ofxSvgRectangle will be added to the current group.
	/// \param ofRectangle arect is the rectangle that will create an ofxSvgRectangle.
	/// \return std::shared_ptr<ofxSvgRectangle> as the rectangle that was created and added to the document.
	std::shared_ptr<ofxSvgRectangle> add( const ofRectangle& arect );
	/// \brief Add a ofxSvgRectangle to the document. The ofxSvgRectangle will be added to the current group.
	/// \param ofRectangle arect is the rectangle that will create an ofxSvgRectangle.
	/// \param float aRoundRadius is the corner radius used to round the corners.
	/// \return std::shared_ptr<ofxSvgRectangle> as the rectangle that was created and added to the document.
	std::shared_ptr<ofxSvgRectangle> add( const ofRectangle& arect, float aRoundRadius );
	
	/// \brief Add a ofxSvgCircle to the document. The ofxSvgCircle will be added to the current group.
	/// The default position is 0,0. Use the returned ofxSvgCircle to set properties directly.
	/// \param float aradius is the radius used to create an ofxSvgCircle.
	/// \return std::shared_ptr<ofxSvgCircle> as the circle that was created and added to the document.
	std::shared_ptr<ofxSvgCircle> addCircle( float aradius );
	/// \brief Add a ofxSvgCircle to the document. The ofxSvgCircle will be added to the current group.
	/// \param glm::vec2 apos is the center position of the circle.
	/// \param float aradius is the radius used to create an ofxSvgCircle.
	/// \return std::shared_ptr<ofxSvgCircle> as the circle that was created and added to the document.
	std::shared_ptr<ofxSvgCircle> addCircle( const glm::vec2& apos, float aradius );
	/// \brief Add a ofxSvgCircle to the document. The ofxSvgCircle will be added to the current group.
	/// \param glm::vec3 apos is the center position of the circle. Note: z is not used.
	/// \param float aradius is the radius used to create an ofxSvgCircle.
	/// \return std::shared_ptr<ofxSvgCircle> as the circle that was created and added to the document.
	std::shared_ptr<ofxSvgCircle> addCircle( const glm::vec3& apos, float aradius );
	/// \brief Add a ofxSvgCircle to the document. The ofxSvgCircle will be added to the current group.
	/// \param float ax is the x center position of the circle.
	/// \param float ay is the y center position of the circle.
	/// \param float aradius is the radius used to create an ofxSvgCircle.
	/// \return std::shared_ptr<ofxSvgCircle> as the circle that was created and added to the document.
	std::shared_ptr<ofxSvgCircle> addCircle( const float& ax, const float& ay, float aradius );
	
	/// \brief Add a ofxSvgEllipse to the document. The ofxSvgEllipse will be added to the current group.
	/// The default position is 0,0. Use the returned ofxSvgEllipse to set properties directly.
	/// \param float aradiusX is the horizontal radius used to create an ofxSvgEllipse.
	/// \param float aradiusY is the vertical radius used to create an ofxSvgEllipse.
	/// \return std::shared_ptr<ofxSvgEllipse> as the ellipse that was created and added to the document.
	std::shared_ptr<ofxSvgEllipse> addEllipse( float aradiusX, float aradiusY );
	/// \brief Add a ofxSvgEllipse to the document. The ofxSvgEllipse will be added to the current group.
	/// The default position is 0,0. Use the returned ofxSvgEllipse to set properties directly.
	/// \param glm::vec2 apos is the center position of the ellipse.
	/// \param float aradiusX is the horizontal radius used to create an ofxSvgEllipse.
	/// \param float aradiusY is the vertical radius used to create an ofxSvgEllipse.
	/// \return std::shared_ptr<ofxSvgEllipse> as the ellipse that was created and added to the document.
	std::shared_ptr<ofxSvgEllipse> addEllipse( const glm::vec2& apos, float aradiusX, float aradiusY );
	/// \brief Add a ofxSvgEllipse to the document. The ofxSvgEllipse will be added to the current group.
	/// The default position is 0,0. Use the returned ofxSvgEllipse to set properties directly.
	/// \param glm::vec3 apos is the center position of the ellipse. Note: z is not used.
	/// \param float aradiusX is the horizontal radius used to create an ofxSvgEllipse.
	/// \param float aradiusY is the vertical radius used to create an ofxSvgEllipse.
	/// \return std::shared_ptr<ofxSvgEllipse> as the ellipse that was created and added to the document.
	std::shared_ptr<ofxSvgEllipse> addEllipse( const glm::vec3& apos, float aradiusX, float aradiusY );
	/// \brief Add a ofxSvgEllipse to the document. The ofxSvgEllipse will be added to the current group.
	/// The default position is 0,0. Use the returned ofxSvgEllipse to set properties directly.
	/// \param float ax is the x center position of the ellipse.
	/// \param float ay is the y center position of the ellipse.
	/// \param float aradiusX is the horizontal radius used to create an ofxSvgEllipse.
	/// \param float aradiusY is the vertical radius used to create an ofxSvgEllipse.
	/// \return std::shared_ptr<ofxSvgEllipse> as the ellipse that was created and added to the document.
	std::shared_ptr<ofxSvgEllipse> addEllipse( const float& ax, const float& ay, float aradiusX, float aradiusY );
	
	/// \brief Add a ofxSvgImage to the document. The ofxSvgImage will be added to the current group.
	/// The default position is 0,0. Use the returned ofxSvgImage to set properties directly.
	/// \param of::filesystem::path apath is the path to the linked image file.
	/// \param ofTexture atex provides the width and height of the image.
	/// \return std::shared_ptr<ofxSvgImage> as the image that was created and added to the document.
	std::shared_ptr<ofxSvgImage> addImage( const of::filesystem::path& apath, const ofTexture& atex );
	/// \brief Add a ofxSvgImage to the document. The ofxSvgImage will be added to the current group.
	/// \param glm::vec2 apos is the position; anchored to the top left.
	/// \param of::filesystem::path apath is the path to the linked image file.
	/// \param ofTexture atex provides the width and height of the image.
	/// \return std::shared_ptr<ofxSvgImage> as the image that was created and added to the document.
	std::shared_ptr<ofxSvgImage> addImage( const glm::vec2& apos, const of::filesystem::path& apath, const ofTexture& atex );
	/// \brief Add a ofxSvgImage to the document. The ofxSvgImage will be added to the current group.
	/// \param of::filesystem::path apath is the path to the linked image file.
	/// \param float awidth provides the width of the image.
	/// \param float aheight provides the height of the image.
	/// \return std::shared_ptr<ofxSvgImage> as the image that was created and added to the document.
	std::shared_ptr<ofxSvgImage> addImage( const of::filesystem::path& apath, const float& awidth, const float& aheight );
	/// \brief Add a ofxSvgImage to the document. The ofxSvgImage will be added to the current group.
	/// \param glm::vec2 apos is the position; anchored to the top left.
	/// \param of::filesystem::path apath is the path to the linked image file.
	/// \param float awidth provides the width of the image.
	/// \param float aheight provides the height of the image.
	/// \return std::shared_ptr<ofxSvgImage> as the image that was created and added to the document.
	std::shared_ptr<ofxSvgImage> addImage( const glm::vec2& apos, const of::filesystem::path& apath, const float& awidth, const float& aheight );
	/// \brief Add a ofxSvgImage to the document. The ofxSvgImage will be added to the current group.
	/// \param glm::vec2 apos is the position; anchored to the top left.
	/// \param ofPixels apixels is the pixels to embed in the svg document. Note: This can increase file size dramatically.
	/// \return std::shared_ptr<ofxSvgImage> as the image that was created and added to the document.
	std::shared_ptr<ofxSvgImage> addEmbeddedImage( const glm::vec2& apos, const ofPixels& apixels );
	
	/// \brief Remove an element from this document or child groups.
	/// \param shared_ptr<ofxSvgElement> aelement to be removed.
	/// \return bool true if element was found and removed.
	bool remove( std::shared_ptr<ofxSvgElement> aelement ) override;
	
	/// \brief Used for development to provide insight into anchor point / control point placements.
	virtual void drawDebug();
	
protected:
	std::string fontsDirectory = "";
	of::filesystem::path folderPath, svgPath;
	ofRectangle mViewbox;
	ofRectangle mBounds;
	void validateXmlSvgRoot( ofXml& aRootSvgNode );
	std::string cleanString( std::string aStr, std::string aReplace );
	void _parseXmlNode( ofXml& aParentNode, std::vector< std::shared_ptr<ofxSvgElement> >& aElements );
	std::shared_ptr<ofxSvgElement> _addElementFromXmlNode( ofXml& tnode, std::vector< std::shared_ptr<ofxSvgElement> >& aElements );
	
	void _parsePolylinePolygon( ofXml& tnode, std::shared_ptr<ofxSvgPath> aSvgPath );
	// reference: https://www.w3.org/TR/SVG/paths.html
	void _parsePath( ofXml& tnode, std::shared_ptr<ofxSvgPath> aSvgPath );
	
	ofxSvgCssClass _parseStyle( ofXml& tnode );
	void _applyStyleToElement( ofXml& tnode, std::shared_ptr<ofxSvgElement> aEle );
	void _applyStyleToPath( ofXml& tnode, std::shared_ptr<ofxSvgPath> aSvgPath );
	void _applyStyleToText( ofXml& tnode, std::shared_ptr<ofxSvgText::TextSpan> aTextSpan );
	
	glm::vec3 _parseMatrixString(const std::string& input, const std::string& aprefix, bool abDefaultZero );
	
	void _getTextSpanFromXmlNode( ofXml& anode, std::vector< std::shared_ptr<ofxSvgText::TextSpan> >& aspans );
	
	void _setNodeParentGroupStack( std::shared_ptr<ofxSvgElement> aele );
	
	ofxSvgGroup* _getPushedGroup();
	
	ofxSvgCssClass& _addCssClassFromPath( std::shared_ptr<ofxSvgPath> aSvgPath );
	void _addCssClassFromPath( std::shared_ptr<ofxSvgPath> aSvgPath, ofXml& anode );
	void _addCssClassFromImage( std::shared_ptr<ofxSvgImage> aSvgImage, ofXml& anode );
	void _addCssClassFromTextSpan( std::shared_ptr<ofxSvgText::TextSpan> aSvgTextSpan, ofXml& anode );
	bool _toXml( ofXml& aParentNode, std::shared_ptr<ofxSvgElement> aele );
	
	void _pushCssClass( const ofxSvgCssClass& acss );
	void _popCssClass();
	void _buildCurrentSvgCssFromStack();
	
	unsigned int mCurrentLayer = 0;
	
	std::string mUnitStr = "px";
	
	ofxSvgCssStyleSheet mSvgCss;
	ofxSvgCssClass mCurrentCss, mDocumentCss;
	std::vector<ofxSvgCssClass> mCssClassStack;
	
	
	ofColor mFillColor, mStrokeColor;
	
	std::vector< std::shared_ptr<ofxSvgGroup> > mGroupStack;
	std::vector< std::shared_ptr<ofxSvgElement> > mDefElements;
	
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
	
	ofPolyWindingMode mDefaultClosedPathWindingMode = OF_POLY_WINDING_NONZERO;
	
};








