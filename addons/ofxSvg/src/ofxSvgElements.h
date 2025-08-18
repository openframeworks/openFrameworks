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

class ofxSvgElement {
	friend class ofxSvg;
	friend class ofxSvgGroup;
public:
	
	static std::string sGetTypeAsString(ofxSvgType atype);
	static std::string sGetSvgXmlName(ofxSvgType atype);
	
	virtual ofxSvgType getType() {return OFXSVG_TYPE_ELEMENT;}
	std::string getTypeAsString();
	
	std::string getName() { return name; }
	std::string getCleanName();
	
	bool isGroup() {
		return (getType() == OFXSVG_TYPE_GROUP);
	}
	
	// override this in ofxSvgGroup 
	virtual void setVisible( bool ab ) { bVisible = ab; }
	bool isVisible() { return bVisible; }
	
	virtual void setAlpha( float aAlpha ) {
		alpha = aAlpha;
	}
	float getAlpha() {
		return alpha;
	}
	
	virtual std::string toString(int nlevel = 0);
	
	float layer = -1.f;
	bool bVisible=true;
	bool bUseShapeColor = true;
		
	glm::vec2 pos = glm::vec2(0.f, 0.f);
	glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	float rotation = 0.0f;
	
	virtual glm::mat4 getTransformMatrix();
	virtual ofNode getNodeTransform();
	
	virtual void draw() {}
	
	virtual void applyStyle(ofxSvgCssClass& aclass) {};
	
	virtual void setUseShapeColor( bool ab ) {
		bUseShapeColor = ab;
	}
	
	/// \brief Get the bounding box of the element , taking into account
	/// all the points and rotation to determine the extents of the element aligned with x and y axes.
	virtual ofRectangle getBoundingBox() { return ofRectangle( pos.x, pos.y, 1, 1 ); };
	
	virtual ofPolyline getFirstPolyline() {
		ofLogWarning("ofxSvgElement") << __FUNCTION__ << " : Element " << getTypeAsString() << " does not have a path.";
		return ofPolyline();
	}
protected:
	float alpha = 1.f;
	std::string name = "";
	// used for saving to set the model position of the current mat4 //
	glm::vec2 mModelPos = glm::vec2(0.f, 0.f);
	glm::vec2 mModelRotationPoint = glm::vec2(0.f, 0.f);
	
	virtual std::shared_ptr<ofxSvgElement> clone() {
		return std::make_shared<ofxSvgElement>(*this);
	};
};

class ofxSvgPath : public ofxSvgElement {
public:
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_PATH;}
	
	virtual void setUseShapeColor( bool ab ) override {
		ofxSvgElement::setUseShapeColor(ab);
		path.setUseShapeColor(ab);
	}
	
	virtual void applyStyle(ofxSvgCssClass& aclass) override;
	
	virtual void draw() override;
	
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
	
	ofPolyline getFirstPolyline() override {
		if( path.getOutline().size() > 0 ) {
			return path.getOutline()[0];
		}
		ofLogWarning("ofxSvgPath") << __FUNCTION__ << " : path does not have an outline.";
		return ofPolyline();
	}
	
	/// \brief Get the bounding box of the path, taking into account
	/// all the points; the points are global, not relative to pos and don't have rotation.
	virtual ofRectangle getBoundingBox() override {
		ofRectangle brect;
		if( path.getOutline().size() > 0 ) {
			bool bFirst = true;
			const auto& outlines = path.getOutline();
			for( auto& outline : outlines ) {
				if( bFirst ) {
					brect = outline.getBoundingBox();
				} else {
					brect.growToInclude( outline.getBoundingBox() );
				}
			}
		}
		return brect;
	};
	
	ofPath path;
	
protected:
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		return std::make_shared<ofxSvgPath>(*this);
	};
};

class ofxSvgRectangle : public ofxSvgPath {
public:
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_RECTANGLE;}
	ofRectangle rectangle;
	
	float getWidth() { return rectangle.getWidth() * scale.x;}
	float getHeight() { return rectangle.getHeight() * scale.y;}
	
	float round = 0.f;
	
protected:
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		return std::make_shared<ofxSvgRectangle>(*this);
	};
};

class ofxSvgImage : public ofxSvgElement {
public:
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_IMAGE;}
	
	float getWidth() const { return width * scale.x;}
	float getHeight() const { return height * scale.y;}
	
	/// \brief Get the bounding box of the image.
	virtual ofRectangle getBoundingBox() override {
		return ofRectangle(pos.x, pos.y, getWidth(), getHeight());
	};
	
	virtual void draw() override;
	glm::vec2 getAnchorPointForPercent( float ax, float ay );
	
	std::filesystem::path getFilePath() { return filepath; }
	
	void setColor( ofColor aColor ) {
		color = aColor;
	}
	ofColor getColor() {
		return ofColor( color.r, color.g, color.b, alpha * (float)color.a );
	}
	
	ofColor color;
	ofImage img;
	
	of::filesystem::path filepath;
	float width = 0.f;
	float height = 0.f;
	
protected:
	bool bTryLoad = false;
	
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		return std::make_shared<ofxSvgImage>(*this);
	};
	
};

class ofxSvgCircle : public ofxSvgPath {
public:
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_CIRCLE;}
	float getRadius() {return radius;}
	float radius = 10.0;
	
protected:
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		return std::make_shared<ofxSvgCircle>(*this);
	};
};

class ofxSvgEllipse : public ofxSvgPath {
public:
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_ELLIPSE;}
	float radiusX, radiusY = 10.0f;
	
protected:
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		return std::make_shared<ofxSvgEllipse>(*this);
	};
};


class ofxSvgText : public ofxSvgRectangle {
public:
	
	class TextSpan {
	public:
		TextSpan() {
			text = "";
//			fontSize = 12;
			lineHeight = 0;
//			mSvgCssClass.setFontSize(fontSize);
//			fontFamily = "Arial";
//			mSvgCssClass.setFontFamily(fontFamily);
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
		
		bool isBold() {
			return mSvgCssClass.isFontBold();
		}
		void setBold( bool ab ) {
			if( ab ) {
				mSvgCssClass.addProperty("font-weight", "bold");
			} else {
				mSvgCssClass.addProperty("font-weight", "regular");
			}
		}
		
		bool isItalic() {
			return mSvgCssClass.isFontItalic();
		}
		void setItalic( bool ab ) {
			if( ab ) {
				mSvgCssClass.addProperty("font-style", "italic");
			} else {
				mSvgCssClass.addProperty("font-style", "regular");
			}
		}
		
		std::string text;
//		int fontSize = 12;
//		std::string fontFamily;
		ofRectangle rect;
		ofColor color;
		float lineHeight = 0;

		void applyStyle(ofxSvgCssClass& aclass);
		ofxSvgCssClass& getCss() { return mSvgCssClass; }

		ofTrueTypeFont& getFont();
		void draw( const std::string& astring, bool abCentered );
		void draw(const std::string &astring, const ofColor& acolor, bool abCentered );
		
	protected:
		ofxSvgCssClass mSvgCssClass;
	};
	
	
	ofxSvgText() = default;
	
	// Deep-copy constructor
	ofxSvgText(const ofxSvgText& other) {
		pos = other.pos;
		scale = other.scale;
		rotation = other.rotation;
		
		layer = other.layer;
		bVisible = other.bVisible;
		
		fdirectory = other.fdirectory;
		bCentered = other.bCentered;
		_overrideColor = other._overrideColor;
		bOverrideColor = other.bOverrideColor;
		
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
			pos = other.pos;
			scale = other.scale;
			rotation = other.rotation;
			
			layer = other.layer;
			bVisible = other.bVisible;
			
			fdirectory = other.fdirectory;
			bCentered = other.bCentered;
			_overrideColor = other._overrideColor;
			bOverrideColor = other.bOverrideColor;
			
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
	ofColor getColor();

	void setText( const std::string& astring, std::string aFontFamily, int aFontSize, float aMaxWidth );
	void create();
	void draw() override;
	void draw(const std::string &astring, bool abCentered );
	void draw(const std::string &astring, const ofColor& acolor, bool abCentered );
	
	void setFontDirectory( std::string aPath ) {
		fdirectory = aPath;
	}
	
	std::string getText() {
		std::string ttext;
		for( auto& tspan : textSpans ) {
			ttext += tspan->text;
		}
		return ttext;
	}
	
	void layoutTextSpans();
	
	void overrideColor( ofColor aColor ) {
		bOverrideColor = true;
		_overrideColor = aColor;
	}
	
/// \brief Get the bounding box of all of the text spans.
	virtual ofRectangle getBoundingBox() override;
	
	std::map< std::string, std::map<int, ofMesh> > meshes;
	std::vector< std::shared_ptr<TextSpan> > textSpans;
	
	std::string fdirectory;
	bool bCentered = false;
	
protected:
	
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
	
	ofFloatColor _overrideColor;
	bool bOverrideColor = false;
};













