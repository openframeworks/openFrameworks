#pragma once
#include "ofNode.h"
#include "ofImage.h"
#include "ofPath.h"
#include <map>
#include "ofTrueTypeFont.h"
#include "ofxSvgCss.h"
#include "ofxSvgFontBook.h"

enum ofxSvgType {
	OFXSVG_TYPE_ELEMENT = 0,
	OFXSVG_TYPE_GROUP,
	OFXSVG_TYPE_RECTANGLE,
	OFXSVG_TYPE_IMAGE,
	OFXSVG_TYPE_ELLIPSE,
	OFXSVG_TYPE_CIRCLE,
	OFXSVG_TYPE_PATH,
	OFXSVG_TYPE_TEXT,
	OFXSVG_TYPE_DOCUMENT,
	OFXSVG_TYPE_TOTAL
};

class ofxSvgElement : public ofNode {
	// adding some friend classes so we can set protected properties from other classes.
	friend class ofxSvg;
	friend class ofxSvgGroup;
public:
	
	/// \brief Get the ofxSvgType as a string.
	/// \param ofxSvgType atype.
	/// \return std::string as readable type.
	static std::string sGetTypeAsString(ofxSvgType atype);
	/// \brief Get the xml name from the ofxSvgType.
	/// \param ofxSvgType atype.
	/// \return std::string as readable type.
	static std::string sGetSvgXmlName(ofxSvgType atype);
	/// \brief Get the ofxSvgType for this element.
	/// \return ofxSvgType.
	virtual ofxSvgType getType() {return OFXSVG_TYPE_ELEMENT;}
	/// \brief Get the ofxSvgType as a string.
	/// \return std::string as readable type.
	std::string getTypeAsString();
	/// \brief Get the name of the element; or the id attribute from the xml node.
	/// \return std::string name of element.
	std::string getName() { return name; }
	/// \brief Set the name of the element; or the id attribute of the xml node.
	/// \param aname string; name to be used for the element
	void setName( const std::string& aname ) {
		name = aname;
	}
	/// \brief Get name with escaped characters and attempts to remove added naming patterns.
	/// Removes the numbers added to the name by illustrator
	/// ie. lelbow_00000070086365269320197030000010368508730034196876_ becomes lelbow
	std::string getCleanName();
	/// \return bool if this element is a group or not.
	bool isGroup() {
		return (getType() == OFXSVG_TYPE_GROUP);
	}
	
	/// \brief Set the rotation around the z-axis.
	/// \param adegrees rotation in degrees.
	void setRotationDeg( float adegrees ) {
		setRotationRad(glm::radians(adegrees));
	}
	
	/// \brief Set the rotation around the z-axis.
	/// \param aradians rotation in radians.
	void setRotationRad( float aradians ) {
		setOrientation(glm::angleAxis(aradians, glm::vec3(0.f, 0.f, 1.f) ));
	}
	
	/// \brief Convenience function that wraps getRollDeg() from ofNode.
	/// \return Rotation around the z-axis in degrees.
	float getRotationDeg() {
		return getRollDeg();
	}
	
	/// \brief Convenience function that wraps getRollRad() from ofNode.
	/// \return Rotation around the z-axis in radians.
	float getRotationRad() {
		return getRollRad();
	}
	
	/// \brief Set the visibility of the element, will not draw if not visible.
	/// \param bool aBVisible set to true for visible.
	virtual void setVisible( bool ab ) { bVisible = ab; }
	/// \return bool if this element is visible.
	bool isVisible() const { return bVisible; }
	/// \brief Set the drawing alpha value of the element.
	/// \param float aAlpha from 0-1; transparent to full opacity.
	virtual void setAlpha( float aAlpha ) {
		alpha = aAlpha;
	}
	/// \return float alpha value of the element.
	float getAlpha() {
		return alpha;
	}
	/// \brief The layer order of the element in the document hierarchy.
	/// \return float layer of the element.
	float getLayer() {
		return layer;
	}
	/// \brief Enable or disable using colors from paths or text spans.
	/// Set to false so ofSetColor() outside of this class will have effect/
	/// \param bool Enable or disable the shape colors.
	virtual void setUseColors( bool ab ) {
		bUseShapeColor = ab;
	}

	
	/// \brief Output a string description
	/// \param nlevel (optional) is the indentation amount.
	/// \return string with type and name.
	virtual std::string toString(int nlevel = 0);
	
	/// \brief Override the function in ofNode meant for drawing
	virtual void customDraw() override {};
	/// \brief Apply css class to the element. Meant to be overridden in subsequent classes.
	virtual void applyStyle(ofxSvgCssClass& aclass) {};
	
	/// \brief Get the local axis aligned bounding box of the element without transforms applied.
	virtual ofRectangle getBoundingBox() { return ofRectangle( 0.0f, 0.0f, 1.f, 1.f ); };
	
	/// \brief Get the axis aligned bounding box of the element, taking into account
	/// global position, rotation and scale to determine the extents of the element aligned with x and y axes.
	virtual ofRectangle getGlobalBoundingBox() {
		auto localRect = getBoundingBox();
		std::vector<glm::vec3> rverts = { localRect.getTopLeft(), localRect.getTopRight(), localRect.getBottomRight(), localRect.getBottomLeft()};
		auto gtrans = getGlobalTransformMatrix();
		
		for( auto& rv : rverts ) {
			rv = glm::vec3(gtrans * glm::vec4(rv, 1.f));
		}
		return _calculateExtents( rverts );
	};
	
	virtual ofPolyline getFirstPolyline() {
		ofLogWarning("ofxSvgElement") << __FUNCTION__ << " : Element " << getTypeAsString() << " does not have a path.";
		return ofPolyline();
	}
protected:
	ofRectangle _calculateExtents( const std::vector<glm::vec3>& averts ) {
		glm::vec2 min = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
		glm::vec2 max = {std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};
		
		for( auto& rv : averts ) {
			min.x = std::min( min.x, rv.x );
			min.y = std::min( min.y, rv.y );
			
			max.x = std::max( max.x, rv.x );
			max.y = std::max( max.y, rv.y );
		}
		
		return ofRectangle( min.x, min.y, max.x-min.x, max.y-min.y );
	}
	
	bool bUseShapeColor = true;
	
	float alpha = 1.f;
	std::string name = "";
	float layer = -1.f;
	bool bVisible=true;
	
	// used for storing the offset model rotation point //
	glm::vec2 mModelRotationPoint = glm::vec2(0.f, 0.f);
	
	virtual std::shared_ptr<ofxSvgElement> clone() {
		return std::make_shared<ofxSvgElement>(*this);
	};
};

class ofxSvgPath : public ofxSvgElement {
	friend class ofxSvg;
public:
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_PATH;}
	
	virtual void setUseColors( bool ab ) override {
		ofxSvgElement::setUseColors(ab);
		path.setUseShapeColor(ab);
	}
	
	virtual void applyStyle(ofxSvgCssClass& aclass) override;
	
	virtual void customDraw() override;
	
	bool isFilled() { return path.isFilled(); }
	ofColor getFillColor() { return path.getFillColor(); }
	void setFillColor( const ofColor& acolor ) {
		path.setFillColor(acolor);
	}
	
	bool hasStroke() { return path.hasOutline(); }
	float getStrokeWidth() { return path.getStrokeWidth(); }
	void setStrokeWidth( const float& awidth ) {
		path.setStrokeWidth(awidth);
	}
	ofColor getStrokeColor() { return path.getStrokeColor(); }
	void setStrokeColor( const ofColor& acolor ) {
		path.setStrokeColor( acolor );
	}
	
	/// \brief Get the first polyline if available.
	/// \return ofPolyline returned from path.getOutline()[0];
	ofPolyline getFirstPolyline() override {
		if( path.getOutline().size() > 0 ) {
			return path.getOutline()[0];
		}
		ofLogWarning("ofxSvgPath") << __FUNCTION__ << " : path does not have an outline.";
		return ofPolyline();
	}
	
	/// \brief Get the local axis aligned bounding box of the path, no transforms applied.
	virtual ofRectangle getBoundingBox() override {
		if(mBBoxNeedsRecalc) {
			mBBoxNeedsRecalc = false;
//			ofRectangle brect;
			const auto& outlines = path.getOutline();
			if( outlines.size() > 0 ) {
				bool bFirst = true;
				for( auto& outline : outlines ) {
					auto bbox = outline.getBoundingBox();
					bbox.x += mOffsetPos.x;
					bbox.y += mOffsetPos.y;
					
					if( bFirst ) {
						bFirst = false;
						mBounds = bbox;
//						mBounds = outline.getBoundingBox();
					} else {
						mBounds.growToInclude( bbox );
//						mBounds.growToInclude( outline.getBoundingBox() );
					}
				}
			}
		}
		return mBounds;
	};
	
	/// \brief Offset the local shape position. Helpful with offset rotations.
	void setOffsetPathPosition( float ax, float ay ) {
		if( ax != mOffsetPos.x || ay != mOffsetPos.y ) {
			mBBoxNeedsRecalc=true;
		}
		mOffsetPos = {ax, ay, 0.f};
	}
	glm::vec3& getOffsetPathPosition() {
		return mOffsetPos;
	}
	
	ofPath& getPath() {
		mBBoxNeedsRecalc = true;
		return path;
	};
	
protected:
	ofPath path;
	bool mBBoxNeedsRecalc = true;
	ofRectangle mBounds;
	
	glm::vec3 mOffsetPos = {0.f, 0.f, 0.f};
	
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		return std::make_shared<ofxSvgPath>(*this);
	};
};

class ofxSvgRectangle : public ofxSvgPath {
	friend class ofxSvg;
public:
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_RECTANGLE;}
	
	float getWidth() { return width;}
	float getHeight() { return height;}
	
	float getGlobalWidth() { return width * getGlobalScale().x;}
	float getGlobalHeight() { return height * getGlobalScale().y;}
	
	/// \brief Get the local bounding box of the rectangle, no transforms applied.
	virtual ofRectangle getBoundingBox() override {
		return ofRectangle(mOffsetPos.x, mOffsetPos.y, getWidth(), getHeight());
	};
	
	/// \brief Set the radius for rounded corners.
	/// \param aRoundAmount the curve radius
	void setRoundRadius( float aRoundAmount ) {
		if( aRoundAmount != roundRadius ) {
			roundRadius = aRoundAmount;
			path.clear();
			if( roundRadius > 0.0f ) {
				path.rectRounded(0.f, 0.f, width, height, roundRadius);
			} else {
				path.rectangle(0.f, 0.f, width, height );
			}
		}
	}
	
	float getRoundRadius() {
		return roundRadius;
	}
	
protected:
	float roundRadius = 0.f;
	float width = 0.f;
	float height = 0.f;
	
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		return std::make_shared<ofxSvgRectangle>(*this);
	};
};


class ofxSvgCircle : public ofxSvgPath {
	friend class ofxSvg;
public:
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_CIRCLE;}
	float getRadius() {return radius;}
	float getGlobalRadius() {return radius * getGlobalScale().x;}
	
	/// \brief Get the local bounding box of the circle, no transforms applied.
	virtual ofRectangle getBoundingBox() override {
		return ofRectangle(-getRadius()+mOffsetPos.x, -getRadius()+mOffsetPos.y, getRadius()*2.f, getRadius()*2.f );
	};
	
	/// \brief Set the radius of the circle. Only updates if the stored radius differs from the radius argument.
	/// \param float desired radius of the circle.
	void setRadius( float aradius ) {
		if( aradius != radius ) {
			radius = aradius;
			path.clear();
			path.circle(0.f, 0.f, radius);
		}
	}
	
protected:
	float radius = 0.0;
	
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		return std::make_shared<ofxSvgCircle>(*this);
	};
};

class ofxSvgEllipse : public ofxSvgPath {
	friend class ofxSvg;
public:
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_ELLIPSE;}
	float getRadiusX() {return radiusX;}
	float getRadiusY() {return radiusY;}
	
	float getGlobalRadiusX() {return radiusX * getGlobalScale().x;}
	float getGlobalRadiusY() {return radiusY * getGlobalScale().x;}
	
	/// \brief Get the local bounding box of the circle, no transforms applied.
	virtual ofRectangle getBoundingBox() override {
		return ofRectangle(-getRadiusX()+mOffsetPos.x, -getRadiusY()+mOffsetPos.y, getRadiusX()*2.f, getRadiusY()*2.f );
	};
	
	/// \brief Set the radius X and radius Y of the ellipse. Only updates if the stored radius X or radius Y differs from the radius arguments.
	/// \param float aRadiusX desired radius X of the ellipse.
	/// \param float aRadiusY desired radius Y of the ellipse.
	void setRadius( float aRadiusX, float aRadiusY ) {
		if( aRadiusX != radiusX ||  aRadiusY != radiusY ) {
			path.clear();
			radiusX = aRadiusX;
			radiusY = aRadiusY;
			path.ellipse({0.f,0.f}, radiusX * 2.0f, radiusY * 2.0f );
		}
	}
	
protected:
	float radiusX, radiusY = 10.0f;
	
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		return std::make_shared<ofxSvgEllipse>(*this);
	};
};


class ofxSvgImage : public ofxSvgElement {
	friend class ofxSvg;
public:
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_IMAGE;}
	
	float getWidth() const { return width;}
	float getHeight() const { return height;}
	
	float getGlobalWidth() { return width * getGlobalScale().x;}
	float getGlobalHeight() { return height * getGlobalScale().y;}
	
	/// \brief Get the local bounding box of the image, no transforms applied.
	virtual ofRectangle getBoundingBox() override {
		return ofRectangle(0.f, 0.f, getWidth(), getHeight());
	};
	
	void load();
	bool isLoaded() {
		return (img.isAllocated() && img.getWidth() > 0 && img.getHeight() > 0);
	}
	
	virtual void customDraw() override;
	//	glm::vec2 getAnchorPointForPercent( float ax, float ay );
	
	const std::filesystem::path& getFilePath() { return filepath; }
	ofImage& getImage() { return img; }
	
	void setColor( ofColor aColor ) {
		color = aColor;
	}
	ofColor getColor() {
		return ofColor( color.r, color.g, color.b, alpha * (float)color.a );
	}
	
protected:
	of::filesystem::path filepath;
	
	ofColor color;
	ofImage img;
	
	float width = 0.f;
	float height = 0.f;
	bool bTryLoad = false;
	
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		return std::make_shared<ofxSvgImage>(*this);
	};
};


class ofxSvgText : public ofxSvgElement {
	friend class ofxSvg;
public:
	class TextSpan {
		friend class ofxSvgText;
	public:
		TextSpan() {
			text = "";
			lineHeight = 0;
		}

		std::string getFontKey() {
			return ofxSvgFontBook::getFontKey(getFontFamily(), isBold(), isItalic());
		}
		
		std::string getFontFamily() {
			return mSvgCssClass.getFontFamily("Arial");
		}
		void setFontFamily(std::string aFontFamily) {
			mSvgCssClass.setFontFamily(aFontFamily);
		}
		
		int getFontSize() {
			return mSvgCssClass.getFontSize(12);
		}
		void setFontSize( int asize ) {
			mSvgCssClass.setFontSize(asize);
		}
		
		ofColor getColor() {
			auto tcolor = mSvgCssClass.getColor("color");
			tcolor.a *= alpha;
			return tcolor;
		}
		
		void setColor( const ofColor& acolor ) {
			mSvgCssClass.setColor(acolor);
		}
		
		bool isBold() {
			return mSvgCssClass.isFontBold();
		}
		void setBold( bool ab ) {
			mSvgCssClass.setFontBold(ab);
		}
		
		bool isItalic() {
			return mSvgCssClass.isFontItalic();
		}
		void setItalic( bool ab ) {
			mSvgCssClass.setFontItalic(ab);
		}
		
		std::string getText() {
			return text;
		}
		
		void setText( const std::string& atext ) {
			mBTextDirty = true;
			text = atext;
		}
		
		ofRectangle rect;
		float lineHeight = 0;
		float alpha = 1.f;

		void applyStyle(ofxSvgCssClass& aclass);
		ofxSvgCssClass& getCssClass() { return mSvgCssClass; }

		ofTrueTypeFont& getFont();
		void draw( const std::string& astring, bool abCentered );
		void draw(const std::string &astring, const ofColor& acolor, bool abCentered );
		
	protected:
		ofRectangle fontRect;
		std::string text;
		ofxSvgCssClass mSvgCssClass;
		bool mBTextDirty = true;
	};
	
	
	ofxSvgText() = default;
	
	// Deep-copy constructor
	ofxSvgText(const ofxSvgText& other) {
		setPosition(other.getPosition());
		setOrientation(other.getOrientationQuat());
		setScale(other.getScale());
		
		layer = other.layer;
		bVisible = other.bVisible;
		alpha = other.alpha;
		name = other.name;
		bUseShapeColor = other.bUseShapeColor;
		
		fdirectory = other.fdirectory;
		bCentered = other.bCentered;
		mSvgCssClass = other.mSvgCssClass;
		
		textSpans.reserve(other.textSpans.size());
		for (const auto& ptr : other.textSpans) {
			// Create a new shared_ptr to a new Item copy or nullptr
			textSpans.push_back(ptr ? std::make_shared<TextSpan>(*ptr) : nullptr);
		}
		
		create();
	}
	
	// Deep-copy assignment
	ofxSvgText& operator=(const ofxSvgText& other) {
		if (this != &other) {
			setPosition(other.getPosition());
			setOrientation(other.getOrientationQuat());
			setScale(other.getScale());
			
			layer = other.layer;
			bVisible = other.bVisible;
			alpha = other.alpha;
			name = other.name;
			bUseShapeColor = other.bUseShapeColor;
			
			fdirectory = other.fdirectory;
			bCentered = other.bCentered;
			mSvgCssClass = other.mSvgCssClass;
			
			textSpans.clear();
			textSpans.reserve(other.textSpans.size());
			for (const auto& ptr : other.textSpans) {
				// Create a new shared_ptr to a new Item copy or nullptr
				textSpans.push_back(ptr ? std::make_shared<TextSpan>(*ptr) : nullptr);
			}
			create();
		}
		return *this;
	}
	
	
	
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_TEXT;}
	
	ofTrueTypeFont& getFont();
	
	void setText( const std::string& astring, std::string aFontFamily, int aFontSize, float aMaxWidth );
	void setText( const std::string& astring, const ofxSvgCssClass& aSvgCssClass, float aMaxWidth );
	void create();
	void customDraw() override;
	// going to override
//	void draw(const std::string &astring, bool abCentered );
//	void draw(const std::string &astring, const ofColor& acolor, bool abCentered );
	
	void setFontDirectory( const of::filesystem::path& aPath ) {
		fdirectory = aPath;
	}
	of::filesystem::path getFontDirectory() {
		return fdirectory;
	}
	
	std::string getText() {
		std::string ttext;
		for( auto& tspan : textSpans ) {
			ttext += tspan->getText();
		}
		return ttext;
	}
	
	void setColor( ofColor acolor ) {
		mSvgCssClass.setColor(acolor);
	}
	
	/// \brief Apply css class to the element. Text spans will use this style unless overridden by their css.
	virtual void applyStyle(ofxSvgCssClass& aclass) override {
		mSvgCssClass = aclass;
	}
	
	ofColor getColor() {
		auto tcolor = mSvgCssClass.getColor("color", ofColor(0));
		tcolor.a *= alpha;
		return tcolor;
	}
	
	void setCentered(bool ab) {
		if( ab != bCentered ) {
			bCentered = ab;
			create();
		}
	}
	
/// \brief Get the local bounding box of all of the text spans.
	virtual ofRectangle getBoundingBox() override;
	
	std::map< std::string, std::map<int, ofMesh> > meshes;
	std::vector< std::shared_ptr<TextSpan> > textSpans;
	
	bool areTextSpansDirty();
	
protected:
	of::filesystem::path fdirectory;
	
	bool bCentered = false;
	
	ofxSvgCssClass mSvgCssClass;
	
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		auto newEle = std::make_shared<ofxSvgText>(*this);
		newEle->textSpans.clear();
		newEle->textSpans.reserve(textSpans.size());
		for (const auto& ptr : textSpans) {
			// Create a new shared_ptr to a new Item copy or nullptr
			// lets not copy over the nullptrs
			if( ptr ) {
				newEle->textSpans.push_back(std::make_shared<TextSpan>(*ptr));
			}
		}
		return newEle;
	};
	
	struct SpanData {
		std::string style;
		std::string content;
	};
	
	std::vector<std::string>splitBySpanTags(const std::string& input);
	SpanData extractSpanData(const std::string& spanTag);
	bool endsWithLineEnding(const std::string& astr);
	std::vector<std::string> splitWordsAndLineEndings(const std::string& input);
	
};













