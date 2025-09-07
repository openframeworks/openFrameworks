#include "ofxSvg.h"
#include "ofUtils.h"
#include <regex>
#include "ofGraphics.h"
#include "ofxSvgUtils.h"

using std::string;
using std::vector;
using std::shared_ptr;

ofPath ofxSvg::sDummyPath;

struct Measurement {
	double value;
	std::string unit;
};

//--------------------------------------------------------------
Measurement parseMeasurement(const std::string& input) {
	
	if( input.empty() ) {
		Measurement result;
		result.value = 0.0;
		result.unit = "";
		return result;
	}
	
	size_t i = 0;
	
	// Extract numeric part
	while (i < input.size() && (std::isdigit(input[i]) || input[i] == '.' || input[i] == '-')) {
		i++;
	}
	
	Measurement result;
	result.value = std::stod(input.substr(0, i));  // Convert number part to double
	result.unit = input.substr(i);                 // Extract the unit part
	
	return result;
}

// Function to deep copy a vector of shared_ptrs
std::vector<std::shared_ptr<ofxSvgElement>> ofxSvg::deepCopyVector(const std::vector<std::shared_ptr<ofxSvgElement>>& original) {
	std::vector<std::shared_ptr<ofxSvgElement>> copy;
	copy.reserve(original.size()); // Reserve space for efficiency
	
	for (auto ptr : original) {
		if (ptr) {
			copy.push_back(ptr->clone());
		}
	}
	return copy;
}

void ofxSvg::deepCopyFrom( const ofxSvg & mom ) {
	
	ofLogVerbose("ofxSvg::deepCopyFrom");
	
	if( mom.mChildren.size() > 0 ) {
		mChildren = deepCopyVector(mom.mChildren);
		for( auto& kid : mChildren ) {
			kid->setParent(*this);
		}
	}
	if( mom.mDefElements.size() > 0 ) {
		mDefElements = deepCopyVector(mom.mDefElements);
	}
	
	mViewbox = mom.mViewbox;
	mBounds = mom.mBounds;
	
	fontsDirectory = mom.fontsDirectory;
	folderPath = mom.folderPath;
	svgPath = mom.svgPath;
	
	mCurrentLayer = mom.mCurrentLayer;
	mUnitStr = mom.mUnitStr;
	
//	if(mom.mCurrentSvgCss) {
//		mCurrentSvgCss = std::make_shared<ofxSvgCssClass>(*mom.mCurrentSvgCss);
//	}
	
	mSvgCss = mom.mSvgCss;
	mCurrentCss = mom.mCurrentCss;
	mDocumentCss = mom.mDocumentCss;
	mFillColor = mom.mFillColor;
	mStrokeColor = mom.mStrokeColor;
	
	mCssClassStack = mom.mCssClassStack;
	_buildCurrentSvgCssFromStack();
	
	mModelMatrix = mom.mModelMatrix;
	mModelMatrixStack = mom.mModelMatrixStack;
	
	mCircleResolution = mom.mCircleResolution;
	mCurveResolution = mom.mCurveResolution;
	
	mPaths = mom.mPaths;
}

//--------------------------------------------------------------
void ofxSvg::moveFrom( ofxSvg&& mom ) {
	ofLogVerbose("ofxSvg::moveFrom");
	
	mChildren = std::move(mom.mChildren);
	
	for( auto& kid : mChildren ) {
		kid->setParent(*this);
	}
	
	mDefElements = std::move(mom.mDefElements);
	
	mViewbox = mom.mViewbox;
	mBounds = mom.mBounds;
	
	fontsDirectory = mom.fontsDirectory;
	folderPath = mom.folderPath;
	svgPath = mom.svgPath;
	
	mCurrentLayer = mom.mCurrentLayer;
	mUnitStr = mom.mUnitStr;
	
//	mCurrentSvgCss = mom.mCurrentSvgCss;
	
	mSvgCss = std::move(mom.mSvgCss);
	mCurrentCss = std::move(mom.mCurrentCss);
	mDocumentCss = std::move(mom.mDocumentCss);
	mFillColor = mom.mFillColor;
	mStrokeColor = mom.mStrokeColor;
	
	mCssClassStack = std::move(mom.mCssClassStack);
	_buildCurrentSvgCssFromStack();
	
	mModelMatrix = mom.mModelMatrix;
	mModelMatrixStack = mom.mModelMatrixStack;
	
	mCircleResolution = mom.mCircleResolution;
	mCurveResolution = mom.mCurveResolution;
	
	mPaths = std::move(mom.mPaths);
}


// Copy constructor (deep copy)
//--------------------------------------------------------------
ofxSvg::ofxSvg(const ofxSvg & mom) {
	clear();
	ofLogVerbose("ofxSvg") << "ofxSvg(const ofxSvg & mom)";
	deepCopyFrom(mom);
}

// Copy assignment operator (deep copy)
//--------------------------------------------------------------
ofxSvg& ofxSvg::operator=(const ofxSvg& mom) {
	if (this != &mom) {
		ofLogVerbose("ofxSvg") << "ofxSvg::operator=(const ofxSvg& mom)";
		clear();
		deepCopyFrom(mom);
	}
	return *this;
}

// Move constructor
//--------------------------------------------------------------
ofxSvg::ofxSvg(ofxSvg && mom) {
	ofLogVerbose("ofxSvg") << "ofxSvg(ofxSvg && mom)";
	clear();
	moveFrom(std::move(mom));
}

// Move assignment operator
ofxSvg& ofxSvg::operator=(ofxSvg&& mom) {
	if (this != &mom) {
		ofLogVerbose("ofxSvg") << "ofxSvg::operator=(ofxSvg&& mom)";
		clear();
		moveFrom(std::move(mom));
	}
	return *this;
}

//--------------------------------------------------------------
ofxSvg::ofxSvg(const of::filesystem::path & fileName) {
	load(fileName);
}

//--------------------------------------------------------------
bool ofxSvg::load( const of::filesystem::path& fileName ) {
	ofFile mainXmlFile( fileName, ofFile::ReadOnly );
	ofBuffer tMainXmlBuffer( mainXmlFile );
	
	svgPath     = fileName;
	folderPath  = ofFilePath::getEnclosingDirectory( fileName, false );

	ofXml xml;
	if (!xml.load(tMainXmlBuffer)) {
		ofLogWarning("ofxSvg") << " unable to load svg from " << fileName << " mainXmlFile: " << mainXmlFile.getAbsolutePath();
		return false;
	}

	return loadFromString(tMainXmlBuffer.getText());
}

//--------------------------------------------------------------
bool ofxSvg::loadFromString(const std::string& data, std::string url) {
	clear();
	
	ofXml xml;
	xml.parse(data);
    
    if( xml ) {
        ofXml svgNode = xml.getFirstChild();
        
        validateXmlSvgRoot( svgNode );
        ofXml::Attribute viewBoxAttr = svgNode.getAttribute("viewBox");
        if(svgNode) {
			
			std::vector<Measurement> values = {
				parseMeasurement(svgNode.getAttribute("x").getValue()),
				parseMeasurement(svgNode.getAttribute("y").getValue()),
				parseMeasurement(svgNode.getAttribute("width").getValue()),
				parseMeasurement(svgNode.getAttribute("height").getValue())
			};
						
			for( auto& tv : values ) {
				if( !tv.unit.empty() ) {
					mUnitStr = tv.unit;
				}
			}
			if( mUnitStr.empty() ) {
				mUnitStr = "px";
			}
			
			mBounds.x = values[0].value;
			mBounds.y = values[1].value;
			mBounds.width = values[2].value;
			mBounds.height = values[3].value;
			
//			mBounds.x        = ofToFloat( cleanString( svgNode.getAttribute("x").getValue(), "px") );
//			mBounds.y        = ofToFloat( cleanString( svgNode.getAttribute("y").getValue(), "px" ));
//			mBounds.width    = ofToFloat( cleanString( svgNode.getAttribute("width").getValue(), "px" ));
//			mBounds.height   = ofToFloat( cleanString( svgNode.getAttribute("height").getValue(), "px" ));
            mViewbox = mBounds;
        }
        
        if( viewBoxAttr ) {
            string tboxstr = viewBoxAttr.getValue();
            vector< string > tvals = ofSplitString( tboxstr, " " );
            if( tvals.size() == 4 ) {
                mViewbox.x = ofToFloat(tvals[0] );
                mViewbox.y = ofToFloat( tvals[1] );
                mViewbox.width = ofToFloat( tvals[2] );
                mViewbox.height = ofToFloat( tvals[3] );
            }
        }
		
		if(svgNode) {
			ofLogVerbose("ofxSvg") << svgNode.findFirst("style").toString() << "  bounds: " << mBounds;
		} else {
			ofLogVerbose("ofxSvg") << __FUNCTION__ << " : NO svgNode: ";
		}
		
		
		ofXml styleXmlNode = svgNode.findFirst("//style");
		if( styleXmlNode ) {
			ofLogVerbose("ofxSvg") << __FUNCTION__ << " : STYLE NODE" << styleXmlNode.getAttribute("type").getValue() << " string: " << styleXmlNode.getValue();
			
			mSvgCss.parse(styleXmlNode.getValue());
			
			ofLogVerbose("ofxSvg") << "-----------------------------";
			ofLogVerbose() << mSvgCss.toString();
			ofLogVerbose("ofxSvg") << "-----------------------------";
 		} else {
			ofLogVerbose("ofxSvg") << __FUNCTION__ << " : NO STYLE NODE";
		}
        
		
		// the defs are added in the _parseXmlNode function //
		_parseXmlNode( svgNode, mChildren );
		// then set the parent to be the document
//		for( auto& child : mChildren ) {
//			child->setParent(*this);
//		}
		
		ofLogVerbose("ofxSvg") << " number of defs elements: " << mDefElements.size();
    }
    
    return true;
}

//--------------------------------------------------------------
bool ofxSvg::reload() {
    if( svgPath.empty() ) {
        ofLogError("ofxSvg") << __FUNCTION__ << " : svg path is empty, please call load with file path before calling reload";
        return false;
    }
    return load( svgPath );
}

//--------------------------------------------------------------
bool ofxSvg::save( of::filesystem::path apath ) {
	// https://www.w3.org/TR/SVG2/struct.html#NewDocument
	ofXml svgXml;
	ofXml svgXmlNode = svgXml.appendChild("svg");
	if( auto vattr = svgXmlNode.appendAttribute("version")) {
		vattr.set("1.1");
	}
	if( auto vattr = svgXmlNode.appendAttribute("xmlns")) {
		vattr.set("http://www.w3.org/2000/svg");
	}
	if( auto vattr = svgXmlNode.appendAttribute("xmlns:xlink")) {
		vattr.set("http://www.w3.org/1999/xlink");
	}
	//	<?xml version="1.0" encoding="utf-8"?>
	
	if( auto vattr = svgXmlNode.appendAttribute("x")) {
		vattr.set(ofToString(mBounds.x,2)+mUnitStr);
	}
	if( auto vattr = svgXmlNode.appendAttribute("y")) {
		vattr.set(ofToString(mBounds.y,2)+mUnitStr);
	}
	if( auto vattr = svgXmlNode.appendAttribute("width")) {
		vattr.set(ofToString(mBounds.getWidth(),2)+mUnitStr);
	}
	if( auto vattr = svgXmlNode.appendAttribute("height")) {
		vattr.set(ofToString(mBounds.getHeight(),2)+mUnitStr);
	}
	if( auto vattr = svgXmlNode.appendAttribute("viewBox")) {
		vattr.set(ofToString(mViewbox.x,0)+" "+ofToString(mViewbox.y,0)+" "+ofToString(mViewbox.getWidth(),0)+" "+ofToString(mViewbox.getHeight(),0));
	}
	
	ofXml cssNode = svgXmlNode.appendChild("style");
	
//	if( mCurrentCss.properties.size() > 0 ) {
//		mSvgCss.getAddClass(mCurrentCss);
//	}
	
	// now we need to save out the children //
	for( auto& kid : mChildren ) {
		_toXml( svgXmlNode, kid );
	}
	
	if( mSvgCss.classes.size() > 0 && cssNode) {
		if( auto vattr = cssNode.appendAttribute("type")) {
			vattr.set("text/css");
		}
		cssNode.set(mSvgCss.toString(false));
	} else {
		svgXmlNode.removeChild(cssNode);
	}
	
//	ofLogNotice("Parser::CSS") << mSvgCss.toString();
	
	return svgXml.save(apath);
}

//--------------------------------------------------------------
void ofxSvg::clear() {
	mChildren.clear();
	mDefElements.clear();
	mCurrentLayer = 0;
//	mCurrentSvgCss.reset();
	mSvgCss.clear();
	mCPoints.clear();
	mCenterPoints.clear();
	
	mCurrentCss.clear();
	mCssClassStack.clear();
	
	mGroupStack.clear();
	mModelMatrix = glm::mat4(1.f);
	mModelMatrixStack = std::stack<glm::mat4>();
//	loadIdentityMatrix();
	
	mFillColor = ofColor(0);
	mStrokeColor = ofColor(0);
	
	mPaths.clear();
}

//--------------------------------------------------------------
const int ofxSvg::getTotalLayers(){
	return mCurrentLayer;
}

//--------------------------------------------------------------
void ofxSvg::recalculateLayers() {
	mCurrentLayer = 0;
	auto allKids = getAllElements(true);
	for( auto& kid : allKids ) {
		kid->layer = mCurrentLayer += 1.0;
	}
}

// including these for legacy considerations //
//--------------------------------------------------------------
int ofxSvg::getNumPath() {
	if( mPaths.size() < 1 ) {
		getPaths();
	}
	return mPaths.size();
}

//--------------------------------------------------------------
ofPath& ofxSvg::getPathAt(int n) {
	if( mPaths.size() < 1 ) {
		getPaths();
	}
	if( n < 0 || n >= mPaths.size() ) {
		ofLogWarning("ofxSvg") << "getPathAt: " << n << " out of bounds for number of paths: " << mPaths.size();
		return sDummyPath;
	}
	return mPaths[n];
}

//--------------------------------------------------------------
const std::vector<ofPath>& ofxSvg::getPaths() const {
//	auto spaths = const_cast<Parser *>(this)->getAllElementsForType<ofx::svg::Path>();
//	std::size_t num = spaths.size();
	if( mPaths.size() < 1 ) {
		// previous ofxSvg also included, circles, ellipses, rects, paths, etc.
		auto spaths = const_cast<ofxSvg *>(this)->getAllElementsWithPath();
		std::size_t num = spaths.size();
		mPaths.resize(num);
		for( std::size_t i = 0; i < num; i++ ) {
			mPaths[i] = spaths[i]->getPath();
		}
	}
	return mPaths;
}

//--------------------------------------------------------------
void ofxSvg::setFontsDirectory( string aDir ) {
    auto fontsDir = aDir;
    if( fontsDir.size() > 1 && fontsDir.back() != '/' ) {
        fontsDir += '/';
    }
	fontsDirectory = fontsDir;
	ofxSvgFontBook::setFontDirectory(fontsDir);
}

//--------------------------------------------------------------
string ofxSvg::toString(int nlevel) {
    string tstr = "";
    if( mChildren.size() ) {
        for( std::size_t i = 0; i < mChildren.size(); i++ ) {
            tstr += mChildren[i]->toString( nlevel );
        }
    }
    return tstr;
}

//--------------------------------------------------------------
void ofxSvg::validateXmlSvgRoot( ofXml& aRootSvgNode ) {
    // if there is no width and height set in the svg base node, svg tiny no likey //
    if(aRootSvgNode) {
        // check for x, y, width and height //
        {
            auto xattr = aRootSvgNode.getAttribute("x");
            if( !xattr ) {
                auto nxattr = aRootSvgNode.appendAttribute("x");
                if(nxattr) nxattr.set("0px");
            }
        }
        {
            auto yattr = aRootSvgNode.getAttribute("y");
            if( !yattr ) {
                auto yattr = aRootSvgNode.appendAttribute("y");
                if( yattr ) yattr.set("0px");
            }
        }
		
		ofXml::Attribute viewBoxAttr = aRootSvgNode.getAttribute("viewBox");
		
		ofRectangle vrect;
		if( viewBoxAttr ) {
			string tboxstr = viewBoxAttr.getValue();
			vector< string > tvals = ofSplitString( tboxstr, " " );
			if( tvals.size() == 4 ) {
				vrect.x = ofToFloat( cleanString( tvals[0], "px") );
				vrect.y = ofToFloat( cleanString( tvals[1], "px") );
				vrect.width = ofToFloat( cleanString( tvals[2], "px") );
				vrect.height = ofToFloat( cleanString( tvals[3], "px") );
			}
		}
        
        auto wattr = aRootSvgNode.getAttribute("width");
        auto hattr = aRootSvgNode.getAttribute("height");
        
        if( !wattr || !hattr ) {
//            ofXml::Attribute viewBoxAttr = aRootSvgNode.getAttribute("viewBox");
            if( vrect.getWidth() > 0.0f && vrect.getHeight() > 0.0f ) {
//                string tboxstr = viewBoxAttr.getValue();
//                vector< string > tvals = ofSplitString( tboxstr, " " );
//                if( tvals.size() >= 4 ) {
                    if( !wattr ) {
                        auto nwattr = aRootSvgNode.appendAttribute("width");
                        if(nwattr) nwattr.set( ofToString(vrect.getWidth())+"px" );
                    }
                    
                    if( !hattr ) {
                        auto nhattr = aRootSvgNode.appendAttribute("height");
                        if(nhattr) nhattr.set( ofToString(vrect.getHeight())+"px" );
                    }
//                }
            }
        }
		// from previous version of ofxSvg
		// Affinity Designer does not set width/height as pixels but as a percentage
		// and relies on the "viewBox" to convey the size of things. this applies the
		// viewBox to the width and height.
		if( vrect.getWidth() > 0.0f ) {
			if(wattr) {
				if( ofIsStringInString( wattr.getValue(), "%")) {
					float wpct = ofToFloat( cleanString(wattr.getValue(), "%" )) / 100.0f;
					wattr.set( ofToString(wpct * vrect.getWidth())+"px" );
				}
			}
		}
		if( vrect.getHeight() > 0.0f ) {
			if(hattr) {
				if( ofIsStringInString( hattr.getValue(), "%")) {
					float hpct = ofToFloat( cleanString(hattr.getValue(), "%" )) / 100.0f;
					hattr.set( ofToString(hpct * vrect.getHeight())+"px" );
				}
			}
		}
    }
}

//--------------------------------------------------------------
string ofxSvg::cleanString( string aStr, string aReplace ) {
    ofStringReplace( aStr, aReplace, "");
    return aStr;
}

//--------------------------------------------------------------
void ofxSvg::_parseXmlNode( ofXml& aParentNode, vector< shared_ptr<ofxSvgElement> >& aElements ) {
    
    auto kids = aParentNode.getChildren();
    for( auto& kid : kids ) {
		if( kid.getName() == "g" ) {
			auto fkid = kid.getFirstChild();
			if( fkid ) {
				auto tgroup = std::make_shared<ofxSvgGroup>();
				tgroup->layer = mCurrentLayer += 1.0;
				auto idattr = kid.getAttribute("id");
				if( idattr ) {
					tgroup->name = idattr.getValue();
				}
				
				auto kidStyle = _parseStyle(kid);
				_pushCssClass(kidStyle);
				
				auto transAttr = kid.getAttribute("transform");
				if( transAttr ) {
					setTransformFromSvgMatrixString( transAttr.getValue(), tgroup );
				}
				
//				// set the parent
//				if( mGroupStack.size() > 0 ) {
//					auto pgroup = mGroupStack.back();
//					tgroup->setParent(*pgroup.get());
//				}
				tgroup->setParent(*_getPushedGroup(), false);
				
				pushGroup(tgroup);
				aElements.push_back( tgroup );
				_parseXmlNode( kid, tgroup->getChildren() );
				popGroup();
				
				_popCssClass();
			}
		} else if( kid.getName() == "defs") {
			ofLogVerbose("ofxSvg") << __FUNCTION__ << " found a defs node.";
			_parseXmlNode(kid, mDefElements );
        } else {
            
            auto ele = _addElementFromXmlNode( kid, aElements );
//            cout << "----------------------------------" << endl;
//            cout << kid.getName() << " kid: " << kid.getAttribute("id").getValue() << " out xml: " << txml.toString() << endl;
        }
    }
}

//--------------------------------------------------------------
shared_ptr<ofxSvgElement> ofxSvg::_addElementFromXmlNode( ofXml& tnode, vector< shared_ptr<ofxSvgElement> >& aElements ) {
    shared_ptr<ofxSvgElement> telement;
	
	bool bHasMatOrTrans = false;
	
	if(auto transAttr = tnode.getAttribute("transform") ) {
		bHasMatOrTrans = true;
	}
	
	if( tnode.getName() == "use") {
		if( auto hrefAtt = tnode.getAttribute("xlink:href")) {
			ofLogVerbose("ofxSvg") << "found a use node with href " << hrefAtt.getValue();
			std::string href = hrefAtt.getValue();
			if( href.size() > 1 && href[0] == '#' ) {
				// try to find by id
				href = href.substr(1, std::string::npos);
				ofLogVerbose("ofxSvg") << "going to look for href " << href;
				for( auto & def : mDefElements ) {
					if( def->name == href ) {
//						ofLogNotice("ofxSvg") << "Found a mDefElement with href: " << def->getName();
//						telement = clone(def);
						telement = def->clone();
						if( !telement ) {
							ofLogWarning("ofxSvg") << "could not find type for def : " << def->name;
						}
						break;
					}
				}
			} else {
				ofLogWarning("ofxSvg") << "could not parse use node with href : " << href;
			}
		} else {
			ofLogWarning("ofxSvg") << "found a use node but no href!";
		}
	} else if( tnode.getName() == "image" ) {
        auto image = std::make_shared<ofxSvgImage>();
        auto wattr = tnode.getAttribute("width");
        if(wattr) image->width  = wattr.getFloatValue();
        auto hattr = tnode.getAttribute("height");
        if(hattr) image->height = hattr.getFloatValue();
        auto xlinkAttr = tnode.getAttribute("xlink:href");
        if( xlinkAttr ) {
			// determine if this is an embedded image //
			if( ofIsStringInString(xlinkAttr.getValue(), "image/png;base64")) {
				auto decodedPix = ofxSvgUtils::base64_decode(xlinkAttr.getValue() );
				if(decodedPix.isAllocated() && decodedPix.getWidth() > 0 && decodedPix.getHeight() > 0 ) {
					image->img.setFromPixels(decodedPix);
				}
			} else {
				image->filepath = folderPath;
				image->filepath.append(xlinkAttr.getValue());
				telement = image;
			}
        }
        
        
    } else if( tnode.getName() == "ellipse" ) {
        auto ellipse = std::make_shared<ofxSvgEllipse>();
		
		auto tpos = glm::vec2(0.f, 0.f);
        auto cxAttr = tnode.getAttribute("cx");
		if(cxAttr) {tpos.x = cxAttr.getFloatValue();}
        auto cyAttr = tnode.getAttribute("cy");
		if(cyAttr) {tpos.y = cyAttr.getFloatValue();}
		
		if( bHasMatOrTrans ) {
			ellipse->setOffsetPathPosition(tpos.x,tpos.y);
		} else {
			ellipse->setPosition( tpos.x, tpos.y, 0.0f);
		}
		
		glm::vec2 radii(0.f, 0.f);
        
		if(auto rxAttr = tnode.getAttribute( "rx" )) {
			radii.x = rxAttr.getFloatValue();
		}
		if(auto ryAttr = tnode.getAttribute( "ry" )) {
			radii.y = ryAttr.getFloatValue();
		}
		
		ellipse->path.setCircleResolution(mCircleResolution);
		ellipse->path.setCurveResolution(mCurveResolution);
		// make local so we can apply transform later in the function
//		ellipse->path.ellipse({0.f,0.f}, ellipse->radiusX * 2.0f, ellipse->radiusY * 2.0f );
		ellipse->setRadius(radii.x, radii.y);
		
		_applyStyleToPath( tnode, ellipse );
        
        telement = ellipse;
	} else if( tnode.getName() == "circle" ) {
		auto circle = std::make_shared<ofxSvgCircle>();
		auto tpos = glm::vec2(0.f, 0.f);
		
		if(auto cxAttr = tnode.getAttribute("cx")) {
			tpos.x = cxAttr.getFloatValue();
		}
		if(auto cyAttr = tnode.getAttribute("cy")) {
			tpos.y = cyAttr.getFloatValue();
		}
		
		if( bHasMatOrTrans ) {
			circle->setOffsetPathPosition(tpos.x,tpos.y);
		} else {
			circle->setPosition(tpos.x, tpos.y, 0.f);
		}
		
		// make local so we can apply transform later in the function
		// position is from the top left
		circle->path.setCircleResolution(mCircleResolution);
		circle->path.setCurveResolution(mCurveResolution);
//		circle->path.circle({0.f,0.f}, circle->radius );
		if(auto rAttr = tnode.getAttribute( "r" )) {
			circle->setRadius(rAttr.getFloatValue());
		}
		
		_applyStyleToPath( tnode, circle );
		
		telement = circle;
		
	} else if( tnode.getName() == "line" ) {
		auto telePath = std::make_shared<ofxSvgPath>();
		
		glm::vec3 p1 = {0.f, 0.f, 0.f};
		glm::vec3 p2 = {0.f, 0.f, 0.f};
		auto x1Attr = tnode.getAttribute("x1");
		if(x1Attr) p1.x = x1Attr.getFloatValue();
		auto y1Attr = tnode.getAttribute("y1");
		if(y1Attr) p1.y = y1Attr.getFloatValue();
		
		auto x2Attr = tnode.getAttribute("x2");
		if(x2Attr) p2.x = x2Attr.getFloatValue();
		auto y2Attr = tnode.getAttribute("y2");
		if(y2Attr) p2.y = y2Attr.getFloatValue();
		
		// set the colors and stroke width, etc.
		telePath->path.clear();
		telePath->path.moveTo(p1);
		telePath->path.lineTo(p2);
		
		_applyStyleToPath( tnode, telePath );
		
		telement = telePath;
        
	} else if(tnode.getName() == "polyline" || tnode.getName() == "polygon") {
		auto tpath = std::make_shared<ofxSvgPath>();
		_parsePolylinePolygon(tnode, tpath);
		_applyStyleToPath( tnode, tpath );
		telement = tpath;
	} else if( tnode.getName() == "path" ) {
		auto tpath = std::make_shared<ofxSvgPath>();
		_parsePath( tnode, tpath );
		_applyStyleToPath( tnode, tpath );
		telement = tpath;
    } else if( tnode.getName() == "rect" ) {
        auto rect = std::make_shared<ofxSvgRectangle>();
		
		auto tpos = glm::vec2(0.f, 0.f);
		if(auto xattr = tnode.getAttribute("x")) {
			tpos.x = xattr.getFloatValue();
		}
		if(auto yattr = tnode.getAttribute("y")) {
			tpos.y = yattr.getFloatValue();
		}
		if(auto wattr = tnode.getAttribute("width")) {
			rect->width = wattr.getFloatValue();
		}
		if(auto hattr = tnode.getAttribute("height")) {
			rect->height = hattr.getFloatValue();
		}
		
		if( bHasMatOrTrans ) {
			rect->setOffsetPathPosition(tpos.x,tpos.y);
		} else {
			rect->setPosition(tpos.x, tpos.y, 0.0f);
		}
		
		auto rxAttr = tnode.getAttribute("rx");
		auto ryAttr = tnode.getAttribute("ry");
		
		rect->path.setCircleResolution(mCircleResolution);
		rect->path.setCurveResolution(mCurveResolution);
		
		float rRadius = 0.0f;
		
		if( !ofxSvgCssClass::sIsNone(rxAttr.getValue()) || !ofxSvgCssClass::sIsNone(ryAttr.getValue())) {
			rRadius = std::max(ofxSvgCssClass::sGetFloat(rxAttr.getValue()),
							   ofxSvgCssClass::sGetFloat(ryAttr.getValue()));
		}
		rect->roundRadius = -1.f;
		rect->setRoundRadius(rRadius);
		
//		// make local so we can apply transform later in the function
//		if( !ofxSvgCssClass::sIsNone(rxAttr.getValue()) || !ofxSvgCssClass::sIsNone(ryAttr.getValue())) {
//			rect->roundRadius = -1.f; // force an update in setRoundRadius
//			rect->setRoundRadius(std::max(ofxSvgCssClass::sGetFloat(rxAttr.getValue()),
//										  ofxSvgCssClass::sGetFloat(ryAttr.getValue()))
//								 );
////			rect->roundRadius = std::max(ofxSvgCssClass::sGetFloat(rxAttr.getValue()),
////								   ofxSvgCssClass::sGetFloat(ryAttr.getValue()));
////			
////			
////			rect->path.rectRounded(tpos.x, tpos.y, rect->width, rect->height, rect->roundRadius);
//			
//		} else {
//			rect->path.rectangle(tpos.x, tpos.y, rect->getWidth(), rect->getHeight());
//		}
        
        telement = rect;
        		
		_applyStyleToPath( tnode, rect );
        
        // this shouldn't be drawn at all, may be a rect that for some reason is generated
        // by text blocks //
        if( !rect->isFilled() && !rect->hasStroke() ) {
			telement->setVisible(false);
        }
        
    } else if( tnode.getName() == "text" ) {
        auto text = std::make_shared<ofxSvgText>();
        telement = text;
		
		auto textCss = _parseStyle( tnode );
		_pushCssClass(textCss);
//		auto tempCss = mCurrentCss;
//		textCss.addMissingClassProperties(tempCss);
//		mCurrentCss = textCss;
		
//		ofLogNotice("ofxSvg") << "_addElementFromXmlNode :: text: " << "has kids: " << tnode.getFirstChild() << " node value: " << tnode.getValue() << std::endl;
//		ofLogNotice("ofxSvg") << "_addElementFromXmlNode :: text: " << "name:" << tnode.getName() << " to string: " << tnode.toString() << std::endl;
//		if( tnode.getAttribute("id")) {
//			ofLogNotice("ofxSvg") << "_addElementFromXmlNode :: text: " << tnode.getAttribute("id").getValue();
//		}
		
        if( tnode.getFirstChild() ) {
            
            auto kids = tnode.getChildren();
            for( auto& kid : kids ) {
                if(kid) {
                    if( kid.getName() == "tspan" ) {
//						text->textSpans.push_back( _getTextSpanFromXmlNode( kid ) );
						_getTextSpanFromXmlNode( kid, text->textSpans );
                    }
                }
            }
            
            // this may not be a text block or it may have no text //
            if( text->textSpans.size() == 0 ) {
				// ok lets see if the node has a value / text
				if( !tnode.getValue().empty() ) {
					_getTextSpanFromXmlNode( tnode, text->textSpans );
				}
            }
        }
		_popCssClass();
		
		string tempFolderPath = ofFilePath::addTrailingSlash(folderPath);
        if( ofDirectory::doesDirectoryExist( tempFolderPath+"fonts/" )) {
            text->setFontDirectory( tempFolderPath+"fonts/" );
        }
        if( fontsDirectory != "" ) {
            if( ofDirectory::doesDirectoryExist(fontsDirectory)) {
                text->setFontDirectory(fontsDirectory);
            }
        }
        
    } else if( tnode.getName() == "g" ) {
		
    }
    
    if( !telement ) {
        return shared_ptr<ofxSvgElement>();
    }
    
    auto idAttr = tnode.getAttribute("id");
    if( idAttr ) {
        telement->name = idAttr.getValue();
    }
    
//    if( telement->getType() == OFXSVG_TYPE_RECTANGLE || telement->getType() == OFXSVG_TYPE_IMAGE || telement->getType() == OFXSVG_TYPE_TEXT || telement->getType() == OFXSVG_TYPE_CIRCLE || telement->getType() == OFXSVG_TYPE_ELLIPSE ) {
	if( telement->getType() != OFXSVG_TYPE_DOCUMENT ) {
        auto transAttr = tnode.getAttribute("transform");
        if( transAttr ) {
//            getTransformFromSvgMatrix( transAttr.getValue(), telement->pos, telement->scale.x, telement->scale.y, telement->rotation );
			setTransformFromSvgMatrixString( transAttr.getValue(), telement );
        }
    }
    
    if( telement->getType() == OFXSVG_TYPE_TEXT ) {
        auto text = std::dynamic_pointer_cast<ofxSvgText>( telement );
        text->create();
    }
	
	_applyStyleToElement(tnode, telement);
        
	telement->layer = mCurrentLayer += 1.0;
    aElements.push_back( telement );
	
	if( telement->getType() == OFXSVG_TYPE_RECTANGLE ) {
		auto rect = std::dynamic_pointer_cast<ofxSvgRectangle>( telement );
		ofLogVerbose("ofxSvg::_addElementFromXmlNode") << "rect->pos: " << rect->getGlobalPosition() << " shape: " << rect->getOffsetPathPosition();
	}
	
	if( mGroupStack.size() > 0 ) {
		auto pgroup = mGroupStack.back();
		ofLogVerbose("ofxSvg::_addElementFromXmlNode") << "element: " << telement->getTypeAsString() << " -" << telement->getCleanName() << "- pos: " << telement->getPosition() << "- parent: " << pgroup->getCleanName();
		telement->setParent(*_getPushedGroup(), false);
	}
	
    return telement;
}

std::vector<float> parseToFloats(const std::string& input) {
	std::vector<glm::vec3> points;
	std::regex regex("[-]?\\d*\\.?\\d+");  // Matches positive/negative floats
	std::sregex_iterator begin(input.begin(), input.end(), regex), end;
	
	std::vector<float> values;
	
	// Extract all floating-point values using regex
	for (std::sregex_iterator i = begin; i != end; ++i) {
		try {
			values.push_back(std::stof((*i).str()));
		} catch (const std::invalid_argument&) {
			std::cerr << "Invalid number found: " << (*i).str() << std::endl;
		}
	}
	
	return values;
}

std::vector<glm::vec3> parsePoints(const std::string& input) {
	std::vector<glm::vec3> points;
	auto values = parseToFloats( input );
	
	// Create vec3 pairs from the values
	for (size_t i = 0; i < values.size(); i += 2) {
		if (i + 1 < values.size()) {
			glm::vec3 point(values[i], values[i + 1], 0.f);
			points.push_back(point);
		}
	}
	
	if( values.size() == 1 && points.size() < 1) {
		glm::vec3 point(values[0], values[0], 0.f);
		points.push_back(point);
	}
	
	return points;
}


std::vector<glm::vec3> parsePointsDefaultY(const std::string& input, float aYpos ) {
	std::vector<glm::vec3> points;
	auto values = parseToFloats( input );
	
	// Create vec3 pairs from the values
	for (size_t i = 0; i < values.size(); i++) {
		glm::vec3 point(values[i], aYpos, 0.f);
		points.push_back(point);
	}
	
	if( values.size() == 1 && points.size() < 1) {
		glm::vec3 point(values[0], aYpos, 0.f);
		points.push_back(point);
	}
	
	return points;
}

std::vector<glm::vec3> parsePointsDefaultX(const std::string& input, float aXpos ) {
	std::vector<glm::vec3> points;
	auto values = parseToFloats( input );
	
	// Create vec3 pairs from the values
	for (size_t i = 0; i < values.size(); i++) {
		glm::vec3 point(aXpos, values[i], 0.f);
		points.push_back(point);
	}
	
	if( values.size() == 1 && points.size() < 1) {
		glm::vec3 point(aXpos, values[0], 0.f);
		points.push_back(point);
	}
	
	return points;
}



//----------------------------------------------------
std::vector<double> _parseSvgArc(const std::string& arcStr) {
	std::vector<double> result;
	std::regex numberRegex(R"([-+]?[0-9]*\.?[0-9]+)"); // Improved regex for better compatibility
	std::sregex_iterator iter(arcStr.begin(), arcStr.end(), numberRegex);
	std::sregex_iterator end;
	
	while (iter != end) {
		try {
			result.push_back(std::stod(iter->str()));
		} catch (const std::exception& e) {
			std::cerr << "Error parsing number: " << iter->str() << " - " << e.what() << std::endl;
		}
		++iter;
	}
	
	return result;
}

//----------------------------------------------------
int _getWindingOrderOnArc( glm::vec3& aStartPos, glm::vec3& aCenterPos, glm::vec3& aendPos ) {
	glm::vec3 sdiff = (aStartPos - aCenterPos);
	if( glm::length2(sdiff) > 0.0f ) {
		sdiff = glm::normalize(sdiff);
	}
	glm::vec3 ediff = (aendPos - aCenterPos);
	if( glm::length2(ediff) > 0.0f ) {
		ediff = glm::normalize(ediff);
	}
	float tcross = sdiff.x * ediff.y - sdiff.y * ediff.x;
//	ofLogNotice("_getWindingOrderOnArc") << "tcross is " << tcross;
	if( tcross > 0.0f ) {
		// clockwise
		return 1;
	} else if( tcross < 0.0f ) {
		// counter clockwise
		return -1;
	}
	// co-linear
	return 0;
	
}

//----------------------------------------------------
// Function to find the center of the elliptical arc from SVG arc parameters
glm::vec2 findArcCenter(glm::vec2 start, glm::vec2 end, double rx, double ry, double x_axis_rotation, bool large_arc_flag, bool sweep_flag) {
	// Convert the rotation to radians
	double phi = glm::radians(x_axis_rotation);
	double cos_phi = cos(phi);
	double sin_phi = sin(phi);
	
	// Step 1: Compute (x1', y1') - the coordinates of the start point in the transformed coordinate system
	glm::vec2 diff = (start - end) / 2.0f;
	glm::vec2 p1_prime(cos_phi * diff.x + sin_phi * diff.y, -sin_phi * diff.x + cos_phi * diff.y);
	
	// Step 2: Correct radii if necessary
	double p1_prime_x_sq = p1_prime.x * p1_prime.x;
	double p1_prime_y_sq = p1_prime.y * p1_prime.y;
	double rx_sq = rx * rx;
	double ry_sq = ry * ry;
	double radii_check = p1_prime_x_sq / rx_sq + p1_prime_y_sq / ry_sq;
	if (radii_check > 1) {
		// Scale radii to ensure the arc can fit between the two points
		double scale = std::sqrt(radii_check);
		rx *= scale;
		ry *= scale;
		rx_sq = rx * rx;
		ry_sq = ry * ry;
	}
	
	// Step 3: Compute (cx', cy') - the center point in the transformed coordinate system
	double factor_numerator = rx_sq * ry_sq - rx_sq * p1_prime_y_sq - ry_sq * p1_prime_x_sq;
	double factor_denominator = rx_sq * p1_prime_y_sq + ry_sq * p1_prime_x_sq;
	if (factor_numerator < 0) {
		factor_numerator = 0; // Precision error correction to avoid sqrt of negative numbers
	}
	
	double factor = std::sqrt(factor_numerator / factor_denominator);
	if (large_arc_flag == sweep_flag) {
		factor = -factor;
	}
	
	glm::vec2 center_prime(factor * rx * p1_prime.y / ry, factor * -ry * p1_prime.x / rx);
	
	// Step 4: Compute the center point in the original coordinate system
	glm::vec2 center(
					 cos_phi * center_prime.x - sin_phi * center_prime.y + (start.x + end.x) / 2.0,
					 sin_phi * center_prime.x + cos_phi * center_prime.y + (start.y + end.y) / 2.0
					 );
	
	return center;
}


//--------------------------------------------------------------
void ofxSvg::_parsePolylinePolygon( ofXml& tnode, std::shared_ptr<ofxSvgPath> aSvgPath ) {
	auto pointsAttr = tnode.getAttribute("points");
	if( !pointsAttr ) {
		ofLogWarning("ofxSvg") << __FUNCTION__ << " polyline or polygon does not have a points attriubute.";
		return;
	}
	
	if( pointsAttr.getValue().empty() ) {
		ofLogWarning("ofxSvg") << __FUNCTION__ << " polyline or polygon does not have points.";
		return;
	}
	
	auto points = parsePoints(pointsAttr.getValue());
	std::size_t numPoints = points.size();
	for( std::size_t i = 0; i < numPoints; i++ ) {
		if( i == 0 ) {
			aSvgPath->path.moveTo(points[i]);
		} else {
			aSvgPath->path.lineTo(points[i]);
		}
	}
	if( numPoints > 2 ) {
		if(tnode.getName() == "polygon" ) {
			aSvgPath->path.close();
		}
	}
}
// reference: https://www.w3.org/TR/SVG2/paths.html#PathData
//--------------------------------------------------------------
void ofxSvg::_parsePath( ofXml& tnode, std::shared_ptr<ofxSvgPath> aSvgPath ) {
	
	aSvgPath->path.clear();
	
	auto dattr = tnode.getAttribute("d");
	if( !dattr ) {
		ofLogWarning("ofxSvg") << __FUNCTION__ << " path node does not have d attriubute.";
		return;
	}
	
//	OF_POLY_WINDING_ODD
	///     OF_POLY_WINDING_NONZERO
	///     OF_POLY_WINDING_POSITIVE
	///     OF_POLY_WINDING_NEGATIVE
	///     OF_POLY_WINDING_ABS_GEQ_TWO
//	aSvgPath->path.setPolyWindingMode(OF_POLY_WINDING_POSITIVE);
//	aSvgPath->path.setPolyWindingMode(mDefaultPathWindingMode);
	
	
	aSvgPath->path.setCircleResolution(mCircleResolution);
	aSvgPath->path.setCurveResolution(mCurveResolution);
	
	std::vector<unsigned char> splitChars = {
		'M', 'm', // move to
		'V', 'v', // vertical line
		'H', 'h', // horizontal line
		'L','l', // line
		'z','Z', // close path
		'c','C','s','S', // cubic bézier
		'Q', 'q', 'T', 't', // quadratic bézier
		'A', 'a' // elliptical arc
	};
	std::string ostring = dattr.getValue();
//	ofLogNotice("ofxSvg") << __FUNCTION__ << " dattr: " << ostring;
	
	if( ostring.empty() ) {
		ofLogError("ofxSvg") << __FUNCTION__ << " there is no data in the d string.";
		return;
	}
	
	std::size_t index = 0;
	if( ostring[index] != 'm' && ostring[index] != 'M' ) {
		ofLogWarning("ofxSvg") << __FUNCTION__ << " first char is not a m or M, ostring[index]: " << ostring[index];
		return;
	}
	
	glm::vec3 currentPos = {0.f, 0.f, 0.f};
	glm::vec3 secondControlPoint = currentPos;
	glm::vec3 qControlPoint = currentPos;
	
	int numSubPathsClosed = 0;
	int numSubPaths = 0;
	
	auto convertToAbsolute = [](bool aBRelative, glm::vec3& aCurrentPos, std::vector<glm::vec3>& aposes) -> glm::vec3 {
		for(auto& apos : aposes ) {
			if( aBRelative ) {
				apos += aCurrentPos;
			}
		}
		if( aposes.size() > 0 ) {
			aCurrentPos = aposes.back();
		}
		return aCurrentPos;
	};
	
	auto convertToAbsolute2 = [](bool aBRelative, glm::vec3& aCurrentPos, std::vector<glm::vec3>& aposes) -> glm::vec3 {
		for( std::size_t k = 0; k < aposes.size(); k+= 1 ) {
			if( aBRelative ) {
				aposes[k] += aCurrentPos;
			}
			if( k > 0 && k % 2 == 1 ) {
				aCurrentPos = aposes[k];
			}
		}
		return aCurrentPos;
	};
	
	auto convertToAbsolute3 = [](bool aBRelative, glm::vec3& aCurrentPos, std::vector<glm::vec3>& aposes) -> glm::vec3 {
		for( std::size_t k = 0; k < aposes.size(); k+= 1 ) {
			if( aBRelative ) {
				aposes[k] += aCurrentPos;
			}
			
			if( k > 0 && k % 3 == 2 ) {
				aCurrentPos = aposes[k];
			}
			
		}
		return aCurrentPos;
	};
	
	auto lineToRelativeFromAbsoluteRecursive = [](glm::vec3& aStartPos, glm::vec3& acurrentPos, std::vector<glm::vec3>& aposes, std::shared_ptr<ofxSvgPath> aPath )  {
	//			int ncounter = 0;
		auto cp = aStartPos;
		for( auto& np : aposes ) {
			auto relativePos = np-aStartPos;
//			auto relativePos = np;
			auto newPos = relativePos + cp;
			aPath->path.lineTo(newPos);
			cp = newPos;//relativePos+prevPos;
//				ncounter++;
		}
		acurrentPos = cp;
	};
	
	std::string tname;
	if( auto tattr = tnode.getAttribute("id")) {
		tname = tattr.getValue();
	}
	
	ofLogVerbose("ofxSvg::_parsePath") << " ------ PARSE-" << tname << "-----------------------" ;
	
	aSvgPath->path.clear();
	
	unsigned int justInCase = 0;
//	std::vector<ofPath::Command> commands;
	bool breakMe = false;
	while( index < ostring.size() && !breakMe && justInCase < 999999) {
		// figure out what we have here .
		auto cchar = ostring[index];
		// check for valid character //
		bool bFoundValidChar = false;
		for( auto& sc : splitChars ) {
			if( sc == cchar ) {
				bFoundValidChar = true;
				break;
			}
		}
		if( !bFoundValidChar ) {
			ofLogWarning("svgParser") << "Did not find valid character: " << cchar;
			breakMe = true;
			break;
		}
		
		
		ofLogVerbose("ofxSvg") << tname << "- o : ["<< ostring[index] <<"]";
		
		// up to next valid character //
		std::string currentString;
		bool bFoundValidNextChar = false;
		auto pos = index+1;
		if( pos >= ostring.size() ) {
//			ofLogVerbose("ofxSvg") << "pos is greater than string size: " << pos << " / " << ostring.size();
//			break;
			breakMe = true;
		}
		
		bFoundValidChar = false;
		for( pos = index+1; pos < ostring.size(); pos++ ) {
			for( auto& sc : splitChars ) {
				if( sc == ostring[pos] ) {
					bFoundValidChar = true;
					break;
				}
			}
			if( bFoundValidChar ) {
				break;
			}
			currentString.push_back(ostring[pos]);
		}
		
		
		int cindex = index;
		index += currentString.size()+1;
		
		
		if( currentString.empty() ) {
			ofLogVerbose("ofxSvg") << "currentString is empty: " << cchar;
//			break;
		}
		
		
		ofLogVerbose("ofxSvg") <<"["<<cindex<<"]["<<cchar<<"]: " << currentString;
		
		bool bRelative = false;
		std::vector<glm::vec3> npositions= {glm::vec3(0.f, 0.f, 0.f)};
		/// \note: ofxSvgOptional is declared in ofxSvgUtils
		/// Using a custom class because older versions of OF did not include std::optional.
		ofxSvgOptional<ofPath::Command::Type> ctype;
		
		// check if we are looking for a position
		if( cchar == 'm' || cchar == 'M' ) {
			/* ------------------------------------------------
//			Reference: https://www.w3.org/TR/SVG/paths.html
			"Start a new sub-path at the given (x,y) coordinates. M (uppercase) indicates that absolute coordinates will follow;
			m (lowercase) indicates that relative coordinates will follow.
			If a moveto is followed by multiple pairs of coordinates, the subsequent pairs are treated as implicit lineto commands.
			Hence, implicit lineto commands will be relative if the moveto is relative, and absolute if the moveto is absolute.
			If a relative moveto (m) appears as the first element of the path, then it is treated as a pair of absolute coordinates.
			In this case, subsequent pairs of coordinates are treated as relative even though the initial moveto is interpreted as an absolute moveto."
			 ------------------------------------------------ */
			
			if( cchar == 'm' && cindex > 0 ) {
				bRelative = true;
			}
			npositions = parsePoints(currentString);
			for( int ni = 0; ni < npositions.size(); ni++ ) {
				ofLogVerbose("ofxSvg::_parsePath") << ni << "->" << npositions[ni];
			}
			ctype = ofPath::Command::moveTo;
			numSubPaths++;
		} else if( cchar == 'v' || cchar == 'V' ) {
			
			float xvalue = 0.f;
			if( cchar == 'v' ) {
				bRelative = true;
			} else {
				xvalue = currentPos.x;
			}
			npositions = parsePointsDefaultX(currentString,xvalue);
			//ofLogVerbose("ofxSvg") << cchar << " line to: " << npositions[0] << " current pos: " << currentPos;
			ctype = ofPath::Command::lineTo;
		} else if( cchar == 'H' || cchar == 'h' ) {
			float yvalue = 0.f;
			if( cchar == 'h' ) {
				bRelative = true;
			} else {
				yvalue = currentPos.y;
			}
			npositions = parsePointsDefaultY(currentString,yvalue);
			ctype = ofPath::Command::lineTo;
		} else if( cchar == 'L' || cchar == 'l' ) {
			if( cchar == 'l' ) {
				bRelative = true;
			}
			npositions = parsePoints(currentString);
//			for( auto& np : npositions ) {
//				ofLogVerbose("ofxSvg") << cchar << " line to: " << np;
//			}
			ctype = ofPath::Command::lineTo;
		} else if( cchar == 'z' || cchar == 'Z' ) {
			ctype = ofPath::Command::close;
			npositions.clear();
		} else if( cchar == 'c' || cchar == 'C' || cchar == 'S' || cchar == 's' ) {
			if( cchar == 'c' || cchar == 's') {
				bRelative = true;
			}
			ctype = ofPath::Command::bezierTo;
			npositions = parsePoints(currentString);
//			for( auto& np : npositions ) {
//				ofLogVerbose("ofxSvg") << cchar << " bezier to: " << np;
//			}
		} else if( cchar == 'Q' || cchar == 'q' || cchar == 'T' || cchar == 't' ) {
			if( cchar == 'q' ) {
				bRelative = true;
			}
			
			ctype = ofPath::Command::quadBezierTo;
			npositions = parsePoints(currentString);
			
//			for( auto& np : npositions ) {
//				ofLogNotice("ofxSvg") << " Quad bezier to: " << np;
//			}
		} else if(cchar == 'a' || cchar == 'A' ) {
			if( cchar == 'a' ) {
				bRelative = true;
			}
			ctype = ofPath::Command::arc;
//			npositions = _parseStrCoordsFunc(currentString);
			auto arcValues = _parseSvgArc(currentString);
			if( arcValues.size() == 7 ) {
				npositions = {
					glm::vec3(arcValues[0], arcValues[1], 0.0f),
					glm::vec3(arcValues[2], 0.0f, 0.0f),
					glm::vec3(arcValues[3], arcValues[4], 0.0f),
					glm::vec3(arcValues[5], arcValues[6], 0.0f)
				};
			} else {
				ofLogError("ofxSvg") << "unable to parse arc command, incorrect number of parameters detected: " << arcValues.size();
				ofLogError("ofxSvg") << "-- Arc values ---------------------- ";
				for( std::size_t n = 0; n < arcValues.size(); n++ ) {
					ofLogError("ofxSvg") << n << ": " << arcValues[n];
				}
				
			}
//			for( auto& np : npositions ) {
//				ofLogNotice("ofxSvg") << " arc parsed positions: " << np;
//			}
		}
		
		if( ctype.has_value() ) {
			
			auto prevPos = currentPos;
			auto commandT = ctype.value();
			
			if( commandT == ofPath::Command::arc ) {
				if( npositions.size() == 4 ) {
					std::vector<glm::vec3> tpositions = {npositions[3]};
					currentPos = convertToAbsolute(bRelative, currentPos, tpositions );
					npositions[3] = tpositions[0];
				} else {
					ofLogWarning("ofxSvg") << "invalid number of arc commands.";
				}
			} else if( commandT == ofPath::Command::bezierTo ) {
				if( cchar == 'S' || cchar == 's' ) {
					currentPos = convertToAbsolute2(bRelative, currentPos, npositions );
				} else {
					currentPos = convertToAbsolute3(bRelative, currentPos, npositions );
				}
//			} else if( commandT == ofPath::Command::quadBezierTo ) {
				// TODO: Check quad bezier for poly bezier like cubic bezier
				
			} else {
				if( npositions.size() > 0 && commandT != ofPath::Command::close ) {
					if( commandT == ofPath::Command::moveTo ) {
						// going to handle this below;
						// inside the if( commandT == ofPath::Command::moveTo ) { check.
					} else {
						currentPos = convertToAbsolute(bRelative, currentPos, npositions );
					}
				}
			}
			
			if( commandT != ofPath::Command::bezierTo ) {
				secondControlPoint = currentPos;
			}
			if( commandT != ofPath::Command::quadBezierTo ) {
				qControlPoint = currentPos;
			}
						
			if( commandT == ofPath::Command::moveTo ) {
				
				if( cchar == 'm' ) {
					if(npositions.size() > 0 ) {
						if(cindex == 0 ) {
							// this is the first m, so the moveTo is absolute but the subsequent points are relative
							currentPos = npositions[0];
						} else {
							currentPos += npositions[0];
						}
					}
				} else {
					if(npositions.size() > 0 ) {
						currentPos = npositions[0];
					}
				}
				
				if( npositions.size() > 0 ) {
					ofLogVerbose("ofxSvg::moveTo") << npositions[0] << " currentPos: " << currentPos;// << " path pos: " << aSvgPath->pos;
					aSvgPath->path.moveTo(currentPos);
				}
				
				if(npositions.size() > 1 ) {
					bool bLineToRelative = bRelative;
					// determine if these points started with m and is the first character
					if( cchar == 'm') {
						bLineToRelative = true;
					}
					
					if( bLineToRelative ) {
						for( int ki = 1; ki < npositions.size(); ki++ ) {
							currentPos += npositions[ki];
							aSvgPath->path.lineTo(currentPos);
						}
					} else {
						for( int ki = 1; ki < npositions.size(); ki++ ) {
							aSvgPath->path.lineTo(npositions[ki]);
						}
						if(npositions.size() > 0 ) {
							currentPos = npositions.back();
						}
					}
				}
				
				secondControlPoint = currentPos;
				qControlPoint = currentPos;
				
			} else if( commandT == ofPath::Command::lineTo ) {
				if( npositions.size() > 0 ) {
					// current pos is already set above
					// so just worry about adding paths
					if( bRelative ) {
						lineToRelativeFromAbsoluteRecursive(prevPos, currentPos, npositions, aSvgPath );
					} else {
						int ncounter = 0;
						for( auto& np : npositions ) {
							ofLogVerbose("ofxSvg::lineTo") << ncounter << "--->"<< np << " prevPos:" << prevPos;
							aSvgPath->path.lineTo(np);
							ncounter++;
						}
					}
				}
			} else if( commandT == ofPath::Command::close ) {
//				ofLogNotice("ofxSvg") << "Closing the path";
				// TODO: Not sure if we need to draw a line to the start point here
				aSvgPath->path.close();
				numSubPathsClosed++;
			} else if( commandT == ofPath::Command::bezierTo ) {
				
				if( cchar == 'S' || cchar == 's' ) {
					// these can come in as multiple sets of points //
					std::vector<glm::vec3> ppositions;// = npositions;
					auto tppos = prevPos;
					for( std::size_t i = 0; i < npositions.size(); i += 2 ) {
						auto cp2 = (secondControlPoint - tppos) * -1.f;
						cp2 += tppos;
						ppositions.push_back( cp2 );
						ppositions.push_back(npositions[i+0]);
						ppositions.push_back(npositions[i+1]);
						tppos = npositions[i+1];
						secondControlPoint = npositions[i+0];
					}
					
					npositions = ppositions;
				}
				
				auto tcpos = prevPos;
				
				for( std::size_t k = 0; k < npositions.size(); k +=3 ) {
					aSvgPath->path.bezierTo(npositions[k+0], npositions[k+1], npositions[k+2]);
					secondControlPoint = npositions[k+1];
					
					mCPoints.push_back(prevPos);
					mCPoints.push_back(npositions[k+0]);
					mCPoints.push_back(npositions[k+1]);
					tcpos = npositions[k+2];
				}
			} else if( commandT == ofPath::Command::quadBezierTo ) {
				if( cchar == 'T' || cchar == 't' ) {
					if( npositions.size() == 1 ) {
						auto cp2 = (qControlPoint - prevPos) * -1.f;
						cp2 += prevPos;
						npositions.insert(npositions.begin(), cp2 );
					}
				}
				
				if( npositions.size() == 2 ) {
					aSvgPath->path.quadBezierTo(prevPos, npositions[0], npositions[1] );
				}
				qControlPoint = npositions[0];
			} else if( commandT == ofPath::Command::arc ) {
				if( npositions.size() == 4 ) {
					// first point is rx, ry
					// second point x value is x-axis rotation
					// third point x value is large-arc-flag, y value is sweep-flag
					// fourth point is x and y: When a relative a command is used, the end point of the arc is (cpx + x, cpy + y).
					glm::vec3 radii = npositions[0];
					float xAxisRotation = npositions[1].x;
					float largeArcFlag = std::clamp( npositions[2].x, 0.f, 1.f );
					float sweepFlag = std::clamp( npositions[2].y, 0.f, 1.f );
					
					glm::vec3 spt = prevPos;
					glm::vec3 ept = npositions[3];
										
					auto cpt = glm::vec3(findArcCenter(spt, ept, radii.x, radii.y, xAxisRotation, largeArcFlag, sweepFlag ), 0.f);
					auto windingOrder = _getWindingOrderOnArc( spt, cpt, ept );
					
					if( largeArcFlag < 1 ) {
						if( sweepFlag > 0 ) {
							if( windingOrder < 0 ) {
								windingOrder *= -1.f;
							}
						} else {
							if( windingOrder > 0 ) {
								windingOrder *= -1.f;
							}
						}
					} else {
						if( sweepFlag > 0 ) {
							if( windingOrder < 1 ) {
								windingOrder *= -1.f;
							}
						} else {
							if( windingOrder > -1 ) {
								windingOrder *= -1.f;
							} else {
								
							}
						}
					}
					
					
					auto startDiff = (spt - cpt);
					if( glm::length2(startDiff) > 0.0f ) {
						startDiff = glm::normalize(startDiff);
					} else {
						startDiff = glm::vec3(1.f, 0.f, 0.f );
					}
					auto endDiff = (ept - cpt);
					if( glm::length2(endDiff) > 0.0f ) {
						endDiff = glm::normalize(endDiff);
					} else {
						endDiff = glm::vec3(1.f, 0.f, 0.f );
					}
					
					float startAngle = atan2f( startDiff.y, startDiff.x );// - glm::radians(40.f);
					float endAngle = atan2f( endDiff.y, endDiff.x );
					
					float xrotRad = glm::radians(xAxisRotation);
					
					startAngle = ofWrapRadians(startAngle);
					endAngle = ofWrapRadians(endAngle);
					
					
					std::string worderS = "co linear";
					if( windingOrder > 0 ) {
						worderS = "clockwise";
					} else if( windingOrder < 0 ) {
						worderS = "counter clockwise";
					}
					
					ofLogVerbose("ofxSvg") << "Arc winding order is: " << worderS << " order: " << windingOrder << " startDiff: " << startDiff << " endDiff: " << endDiff << " xAxisRotation: " << xAxisRotation;
					
					ofPolyline tline;
										
					if( windingOrder < 0 ) {
						tline.arcNegative(cpt, radii.x, radii.y, glm::degrees(startAngle-xrotRad), glm::degrees(endAngle-xrotRad), mCircleResolution );
					} else {
						tline.arc(cpt, radii.x, radii.y, glm::degrees(startAngle-xrotRad), glm::degrees(endAngle-xrotRad), mCircleResolution );
					}
					
					// rotate based on x-axis rotation //
//					aSvgPath->path.rotateRad(xrotRad, glm::vec3(0.0f, 0.0f, 1.f));
					
					for( auto& pv : tline.getVertices() ) {
						auto nv = pv - cpt;
						if( glm::length2(nv) > 0.0f ) {
							nv = glm::vec3( glm::rotate(glm::vec2(nv.x, nv.y), xrotRad), 0.f);
						}
						nv += cpt;
						pv.x = nv.x;
						pv.y = nv.y;
					}
////
//					// I guess we have to copy the line via commands
					if( tline.size() > 0 ) {
//						aSvgPath->path.moveTo(spt);
						for( std::size_t i = 0; i < tline.size(); i++ ) {
//							if( i == 0 ) {
//								aSvgPath->path.moveTo(tline[0]);
//							} else {
								aSvgPath->path.lineTo(tline[i]);
//							}
						}
					}
					
					mCenterPoints.push_back(cpt);
//					mCenterPoints.push_back(cpt);
					npositions.clear();
					npositions.push_back(ept);
				} else {
					ofLogWarning("ofxSvg") << "unable to parse arc segment.";
				}
			}
		}
		
//		ofLogNotice("ofxSvg") << "["<<cchar<<"]: " << currentString;
//		ofLogNotice("ofxSvg::_parsepath") << " num closed: " << numSubPathsClosed << " / " << numSubPaths;
		
		// If all of the paths are closed, then set the winding mode.
		// We only set it on closed paths because other winding modes can force close the paths
		// which is incorrect with open paths / polylines.
		if( numSubPaths == numSubPathsClosed ) {
			aSvgPath->path.setPolyWindingMode(mDefaultClosedPathWindingMode);
		}
		
		justInCase++;
	}
}

//--------------------------------------------------------------
ofxSvgCssClass ofxSvg::_parseStyle( ofXml& anode ) {
	ofxSvgCssClass css;
	
	// apply first if we have a global style //
	for( auto& tprop : mCurrentCss.properties ) {
		if( tprop.first.empty() ) {
			ofLogNotice("ofxSvg") << "First prop is empty";
		}
		css.addProperty(tprop.first, tprop.second);
	}
	
	// now apply all of the other via css classes //
	// now lets figure out if there is any css applied //
	if( auto classAttr = anode.getAttribute("class") ) {
		// get a list of classes, is this separated by commas?
		auto classList = ofSplitString(classAttr.getValue(), ",");
//		ofLogNotice("ofxSvg") << " going to try and parse style classes string: " << classAttr.getValue();
		for( auto& className : classList ) {
			if( mSvgCss.hasClass(className) ) {
//				ofLogNotice("ofxSvg") << " has class " << className;
				// now lets try to apply it to the path
				auto& tCss = mSvgCss.getClass(className);
				for( auto& tprop : tCss.properties ) {
//					ofLogNotice("ofxSvg") << " adding property " << tprop.first << " value: " << tprop.second.srcString;
					css.addProperty(tprop.first, tprop.second);
				}
			}
		}
	}
	
	// locally set on node overrides the class listing
	// are there any properties on the node?
	
	// avoid the following
	std::vector<std::string> reservedAtts = {
		"d", "id", "xlink:href", "width", "height", "rx", "ry", "cx", "cy", "r", "style", "font-family",
		"x","y","x1","y1","x2","y2","transform"
	};
	
	// lets try to do this a better way
	for( auto& att : anode.getAttributes() ) {
		auto atName = ofToLower(att.getName());
		bool bFileIt = true;
		for( auto& rattName : reservedAtts ) {
			if( atName == rattName ) {
				bFileIt=false;
				break;
			}
		}
		if( bFileIt ) {
			css.addProperty(att.getName(), att.getValue());
		}
	}
	
	if( auto ffattr = anode.getAttribute("font-family") ) {
		std::string tFontFam = ffattr.getValue();
		ofStringReplace( tFontFam, "'", "" );
		css.addProperty("font-family", tFontFam);
	}
	
	// and lastly style
	if( auto styleAttr = anode.getAttribute("style") ) {
		css.addProperties(styleAttr.getValue());
	}
	
	return css;
}

//--------------------------------------------------------------
void ofxSvg::_applyStyleToElement( ofXml& tnode, std::shared_ptr<ofxSvgElement> aEle ) {
	auto css = _parseStyle(tnode);
	if( css.hasAndIsNone("display")) {
		ofLogVerbose("ofxSvg") << "setting element to invisible: " << aEle->name;
		aEle->setVisible(false);
	}
}

//--------------------------------------------------------------
void ofxSvg::_applyStyleToPath( ofXml& tnode, std::shared_ptr<ofxSvgPath> aSvgPath ) {
	auto css = _parseStyle(tnode);
	aSvgPath->applyStyle(css);
}

//--------------------------------------------------------------
void ofxSvg::_applyStyleToText( ofXml& anode, std::shared_ptr<ofxSvgText::TextSpan> aTextSpan ) {
	auto css = _parseStyle(anode);
	aTextSpan->applyStyle(css);
}


//--------------------------------------------------------------
glm::vec3 ofxSvg::_parseMatrixString(const std::string& input, const std::string& aprefix, bool abDefaultZero ) {
	ofLogVerbose("ofxSvg") << __FUNCTION__ << " input: " << input;;
	std::string searchStr = aprefix + "(";
	size_t startPos = input.find(searchStr);
	
	if (startPos != std::string::npos) {
		startPos += searchStr.size();
		size_t endPos = input.find(")", startPos);
		
		if (endPos != std::string::npos) {
			// Extract the part inside the parentheses
			std::string inside = input.substr(startPos, endPos - startPos);
			
			std::replace(inside.begin(), inside.end(), ',', ' ');
			
			// Ensure numbers like ".5" are correctly handled by adding a leading zero if needed
			if (inside[0] == '.') {
				inside = "0" + inside;
			}
			
			float tx = 0.f, ty = 0.f, tz = 0.f;
			std::stringstream ss(inside);
			if (ss >> tx) {
				if (!(ss >> ty)) {
					if(abDefaultZero) {
						ty = 0.0f;
					} else {
						ty = tx; // If only one value is provided, duplicate it
					}
				}
				if (!(ss >> tz)) {
					if( abDefaultZero) {
						tz = 0.0f;
					} else {
						tz = ty; // If only two values are provided, duplicate the second one
					}
				}
				return glm::vec3(tx, ty, tz);
			}
		}
	}
	return glm::vec3(0.f, 0.f, 0.0f);
}

//--------------------------------------------------------------
glm::mat4 ofxSvg::setTransformFromSvgMatrixString( string aStr, std::shared_ptr<ofxSvgElement> aele ) {
	ofLogVerbose("-----------ofxSvg::setTransformFromSvgMatrixString") << aele->getTypeAsString() << " name: " << aele->getName() +"----------------";
//	aele->rotation = 0.0;
	aele->setScale(1.f);
	aele->mModelRotationPoint = glm::vec2(0.0f, 0.0f);
	// TODO: Should a matrix push and pop structure, similar to renderers, be implemented?
	ofLogVerbose("ofxSvg") << __FUNCTION__ << " name: " << aele->getName() << " going to parse string: " << aStr << " pos: " << aele->getPosition();
	
	float trotation = 0.f;
	glm::mat4 mat = glm::mat4(1.f);
	
	if( ofIsStringInString(aStr, "translate")) {
		auto transStr = aStr;
		auto tp = _parseMatrixString(transStr, "translate", false );
		tp.z = 0.f;
		ofLogVerbose("ofxSvg::setTransformFromSvgMatrixString") << aele->getTypeAsString() << " name: " << aele->getName() << " translate: " << tp;
//		apos += tp;
		mat = glm::translate(glm::mat4(1.0f), glm::vec3(tp.x, tp.y, 0.0f));
//		gmat = glm::translate(gmat, glm::vec3(tp.x, tp.y, 0.0f));
		aele->setPosition(tp.x, tp.y, 0.0f);
	} else {
		mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.0f));
//		gmat = glm::translate(gmat, glm::vec3(0.f, 0.f, 0.0f));
	}
	
	if( ofIsStringInString(aStr, "rotate")) {
		auto transStr = aStr;
		auto tr = _parseMatrixString(transStr, "rotate", true );
		trotation = tr.x;
		if( trotation != 0.f ) {
			glm::vec2 rcenter(0.f, 0.f);
			if( tr.y != 0.0f || tr.z != 0.0f ) {
				rcenter.x = tr.y;
				rcenter.y = tr.z;
				
				aele->mModelRotationPoint = rcenter;
				
				glm::vec3 pivot(rcenter.x, rcenter.y, 0.f);
				// Step 1: Translate to pivot (move pivot to origin)
				glm::mat4 toOrigin = glm::translate(glm::mat4(1.0f), -pivot );
				
				// Step 2: Apply rotation
				glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(trotation), glm::vec3(0.f, 0.f, 1.f) );
				
				// Step 3: Translate back to original position
				glm::mat4 backToPivot = glm::translate(glm::mat4(1.0f), pivot);
				
				// Apply transformations in the correct order: T_back * R * T_origin * Original_Transform
				mat = backToPivot * rotation * toOrigin * mat;
//				gmat = backToPivot * rotation * toOrigin * gmat;
			} else {
//				mat = mat * glm::toMat4((const glm::quat&)glm::angleAxis(glm::radians(aele->rotation), glm::vec3(0.f, 0.f, 1.f)));
				mat = glm::rotate(mat, glm::radians(trotation), glm::vec3(0.f, 0.f, 1.f));
//				gmat = glm::rotate(gmat, glm::radians(trotation), glm::vec3(0.f, 0.f, 1.f));
			}
			
			// now we need to apply the rotation
			aele->setOrientation(glm::angleAxis(glm::radians(trotation), glm::vec3(0.f, 0.f, 1.f) ));
//			ofLogNotice("ofxSvg") << "rcenter: " << rcenter.x << ", " << rcenter.y;
		}
		ofLogVerbose("ofxSvg") << __FUNCTION__ << " name: " << aele->getName() << " arotation: " << trotation << " trot: " << tr;
	}
	
	if( ofIsStringInString(aStr, "scale")) {
		auto transStr = aStr;
		auto ts = _parseMatrixString(transStr, "scale", false );
//		aele->scale.x = ts.x;
//		aele->scale.y = ts.y;
		aele->setScale(ts.x, ts.y, 1.f);
		ofLogVerbose("ofxSvg") << __FUNCTION__ << " name: " << aele->getName() << " scale: " << ts;
		
		mat = glm::scale(mat, glm::vec3(aele->getScale().x, aele->getScale().y, 1.f));
//		gmat = glm::scale(gmat, glm::vec3(aele->getScale().x, aele->getScale().y, 1.f));
	}
	
//	glm::vec3 pos3 = mat * glm::vec4( aele->getPosition().x, aele->getPosition().y, 0.0f, 1.f );
//	pos3 = gmat * glm::vec4( aele->pos.x, aele->pos.y, 0.0f, 1.f );
//	aele->pos.x = pos3.x;
//	aele->pos.y = pos3.y;
//	aele->setPosition( pos3.x, pos3.y, 0.0f);
	
	if( ofIsStringInString(aStr, "matrix")) {
		
		// example transform string for matrix form.
		// transform="matrix(-1,0,0,-1,358.9498,1564.4744)"
		
		auto matrix = aStr;
		ofStringReplace(matrix, "matrix(", "");
		ofStringReplace(matrix, ")", "");
		ofStringReplace(matrix, ",", " ");
		vector<string> matrixNum = ofSplitString(matrix, " ", false, true);
		vector<float> matrixF;
		for(std::size_t i = 0; i < matrixNum.size(); i++){
			matrixF.push_back(ofToFloat(matrixNum[i]));
			ofLogVerbose("ofxSvg::setTransformFromSvgMatrixString") << aele->getCleanName() << " matrix[" << i << "] = " << matrixF[i] << " string version is " << matrixNum[i];
		}
		
		if( matrixNum.size() == 6 ) {
			
			mat = glm::translate(glm::mat4(1.0f), glm::vec3(matrixF[4], matrixF[5], 0.0f));
			
			aele->setPosition(matrixF[4], matrixF[5], 0.f);
			
			float trotation = glm::degrees( atan2f(matrixF[1],matrixF[0]) );
			
//			aele->scale.x = glm::sqrt(matrixF[0] * matrixF[0] + matrixF[1] * matrixF[1]);
//			aele->scale.y = glm::sqrt(matrixF[2] * matrixF[2] + matrixF[3] * matrixF[3]);
			float sx = glm::sqrt(matrixF[0] * matrixF[0] + matrixF[1] * matrixF[1]);
			float sy = glm::sqrt(matrixF[2] * matrixF[2] + matrixF[3] * matrixF[3]);
			
			if (matrixF[0] < 0) sx *= -1.f;
			if (matrixF[3] < 0) sy *= -1.f;
			
			aele->setScale(sx, sy, 1.f);
			
			// Avoid double-rotating when both scale = -1 and rotation = 180
			if (sx < 0 && sy < 0 && glm::abs(trotation - 180.0f) < 0.01f) {
				trotation = 0.f;
			}
			
			if( trotation != 0.f ) {
//				mat = mat * glm::toMat4((const glm::quat&)glm::angleAxis(glm::radians(aele->rotation), glm::vec3(0.f, 0.f, 1.f)));
				mat = glm::rotate(mat, glm::radians(trotation), glm::vec3(0.f, 0.f, 1.f));
//				aele->setOrientation(glm::angleAxis(glm::radians(trotation), glm::vec3(0.f, 0.f, 1.f) ));
				aele->setRotationDeg(trotation);
			}
			
			mat = glm::scale(mat, glm::vec3(aele->getScale().x, aele->getScale().y, 1.f));
			
			ofLogVerbose("ofxSvg::setTransformFromSvgMatrixString") << "pos: " << aele->getPosition() << " rotation: " << trotation << " scale: " << aele->getScale();
		}
	}
	
    return mat;
}

//--------------------------------------------------------------
std::string ofxSvg::getSvgMatrixStringFromElement( std::shared_ptr<ofxSvgElement> aele ) {
	std::ostringstream matrixStream;
	matrixStream << std::fixed << std::setprecision(1);
	bool bFirst = true;
	
	if( aele->getPosition().x != 0.f || aele->getPosition().y != 0.f ) {
		bFirst = false;
		matrixStream << "translate(" << aele->getPosition().x << "," << aele->getPosition().y << ")";
	}
	
	if( aele->getRotationDeg() != 0.f ) {
		if(!bFirst) {
			matrixStream << " ";
		}
		bFirst = false;
		if( aele->mModelRotationPoint.x != 0.0f || aele->mModelRotationPoint.y != 0.0f ) {
			matrixStream << "rotate(" << aele->getRotationDeg() << " " << aele->mModelRotationPoint.x << " " << aele->mModelRotationPoint.y <<")";
		} else {
			matrixStream << "rotate(" << aele->getRotationDeg() <<")";
		}
	}
	
	if( aele->getScale().x != 1.f || aele->getScale().y != 1.f ) {
		if(!bFirst) {
			matrixStream << " ";
		}
		bFirst = false;
		matrixStream << "scale(" << aele->getScale().x << " " << aele->getScale().y <<")";
	}
	
	if( matrixStream.str().size() > 3 ) {
		return matrixStream.str();
	}
	
	return "";
	
}

//--------------------------------------------------------------
void ofxSvg::_getTextSpanFromXmlNode( ofXml& anode, std::vector< std::shared_ptr<ofxSvgText::TextSpan> >& aspans ) {
//	if( anode.getName() != "tspan") {
//		return;
//	}
	auto tspan = std::make_shared<ofxSvgText::TextSpan>();
    
    string tText = anode.getValue();
    float tx = 0;
    auto txattr = anode.getAttribute("x");
    if( txattr) {
        tx = txattr.getFloatValue();
    }
    float ty = 0;
    auto tyattr = anode.getAttribute("y");
    if( tyattr ) {
        ty = tyattr.getFloatValue();
    }
    
	tspan->setText(tText);
    tspan->rect.x        = tx;
    tspan->rect.y        = ty;
	
	_applyStyleToText(anode, tspan);
	
//	ofLogNotice("ofxSvg::_getTextSpanFromXmlNode") << anode.getValue() << " anode string: " << anode.toString();
	
	aspans.push_back(tspan);
	
	_pushCssClass(tspan->getCssClass());
	for( auto& kid : anode.getChildren() ) {
		if( kid ) {
			if( kid.getName() == "tspan") {
//				ofLogNotice("ofxSvg::_getTextSpanFromXmlNode") << anode.getValue() << " anode string: " << anode.toString();
				_getTextSpanFromXmlNode( kid, aspans );
			}
		}
	}
	_popCssClass();
	
}

//--------------------------------------------------------------
float ofxSvg::getWidth() const {
	return mViewbox.getWidth();
}

//--------------------------------------------------------------
float ofxSvg::getHeight() const {
	return mViewbox.getHeight();
}

//--------------------------------------------------------------
ofRectangle ofxSvg::getViewbox() const {
	return mViewbox;
}

//--------------------------------------------------------------
float ofxSvg::getBoundsWidth() const {
	return mBounds.getWidth();
}

//--------------------------------------------------------------
float ofxSvg::getBoundsHeight() const {
	return mBounds.getHeight();
}

//--------------------------------------------------------------
ofRectangle ofxSvg::getBounds() const {
	return mBounds;
}

//--------------------------------------------------------------
void ofxSvg::setWidth(float aw) {
	mViewbox.width = aw;
	if( mBounds.width < 1 ) {
		mBounds.width = aw;
	}
}

//--------------------------------------------------------------
void ofxSvg::setHeight(float ah) {
	mViewbox.height = ah;
	if( mBounds.height < 1 ) {
		mBounds.height = ah;
	}
}

//--------------------------------------------------------------
void ofxSvg::setViewBox( const ofRectangle& arect ) {
	mViewbox = arect;
}

//--------------------------------------------------------------
void ofxSvg::setBoundsWidth( float aw ) {
	mBounds.width = aw;
}

//--------------------------------------------------------------
void ofxSvg::setBoundsHeight( float ah ) {
	mBounds.height = ah;
}

//--------------------------------------------------------------
void ofxSvg::setBounds( const ofRectangle& arect ) {
	mBounds = arect;
}

//--------------------------------------------------------------
void ofxSvg::pushGroup( const std::string& apath ) {
	std::shared_ptr<ofxSvgGroup> cgroup;
	if( mGroupStack.size() > 0 ) {
		mGroupStack.back()->get<ofxSvgGroup>( apath );
	} else {
		cgroup = get<ofxSvgGroup>( apath );
	}
	
	if( cgroup ) {
		pushGroup(cgroup);
	} else {
		ofLogWarning("ofxSvg") << "could not find group with path " << apath;
	}
}

//--------------------------------------------------------------
void ofxSvg::pushGroup( const std::shared_ptr<ofxSvgGroup>& agroup ) {
	if( agroup ) {
		mGroupStack.push_back(agroup);
	}
}

//--------------------------------------------------------------
void ofxSvg::popGroup() {
	if( mGroupStack.size() > 0 ) {
		mGroupStack.pop_back();
	}
}

//--------------------------------------------------------------
void ofxSvg::setFillColor(ofColor acolor) {
	mFillColor = acolor;
	mDocumentCss.setFillColor(acolor);
	mCurrentCss.setFillColor(acolor);
}

//--------------------------------------------------------------
void ofxSvg::setFilled(bool abFilled) {
	if( abFilled ) {
		mDocumentCss.setFillColor(mFillColor);
		mCurrentCss.setFillColor(mFillColor);
	} else {
		mDocumentCss.setNoFill();
		mCurrentCss.setNoFill();
	}
}

//--------------------------------------------------------------
void ofxSvg::setStrokeColor(ofColor acolor) {
	mStrokeColor = acolor;
	mCurrentCss.setStrokeColor(acolor);
	mDocumentCss.setStrokeColor(acolor);
}

//--------------------------------------------------------------
void ofxSvg::setStrokeWidth(float aLineWidth) {
	mCurrentCss.setStrokeWidth(aLineWidth);
	mDocumentCss.setStrokeWidth(aLineWidth);
}

//--------------------------------------------------------------
void ofxSvg::setHasStroke(bool abStroke) {
	if( abStroke ) {
		mCurrentCss.setStrokeColor(mStrokeColor);
		mDocumentCss.setStrokeColor(mStrokeColor);
	} else {
		mCurrentCss.setNoStroke();
		mDocumentCss.setNoStroke();
	}
}

//--------------------------------------------------------------
void ofxSvg::setDefaultClosedPathWindingMode( ofPolyWindingMode aWindingMode ) {
	mDefaultClosedPathWindingMode = aWindingMode;
}

//--------------------------------------------------------------
ofPolyWindingMode ofxSvg::getDefaultClosedPathWindingMode() {
	return mDefaultClosedPathWindingMode;
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgGroup> ofxSvg::addGroup(std::string aname) {
	auto tgroup = std::make_shared<ofxSvgGroup>();
	tgroup->name = aname;
	_getPushedGroup()->add(tgroup);
	recalculateLayers();
	return tgroup;
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgPath> ofxSvg::add( const ofPath& apath ) {
	auto path = std::make_shared<ofxSvgPath>();
	path->path = apath;
	path->applyStyle(mCurrentCss);
	_getPushedGroup()->add(path);
	recalculateLayers();
	mPaths.clear();
	return path;
}

//--------------------------------------------------------------
std::vector< std::shared_ptr<ofxSvgPath> > ofxSvg::add( const std::vector<ofPath>& apaths ) {
	std::vector< std::shared_ptr<ofxSvgPath> > rpaths;
	for( auto& path : apaths ) {
		rpaths.push_back( add(path) );
	}
	return rpaths;
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgPath> ofxSvg::add( const ofPolyline& apoly ) {
	if( apoly.size() < 2 ) {
		ofLogWarning("ofxSvg::add") << "polyline has less than 2 vertices.";
	}
	
	ofPath opath;
	const auto& verts = apoly.getVertices();
	for( std::size_t i = 0; i < verts.size(); i++ ) {
		if( i == 0 ) {
			opath.moveTo(verts[i]);
		} else {
			opath.lineTo(verts[i]);
		}
	}
	if( apoly.isClosed() ) {
		opath.close();
	}
	return add( opath );
}

//--------------------------------------------------------------
std::vector< std::shared_ptr<ofxSvgPath> > ofxSvg::add( const std::vector<ofPolyline>& apolys ) {
	std::vector< std::shared_ptr<ofxSvgPath> > rpaths;
	for( auto& poly : apolys ) {
		rpaths.push_back( add(poly) );
	}
	return rpaths;
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgRectangle> ofxSvg::add( const ofRectangle& arect ) {
	return add( arect, 0.0f);
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgRectangle> ofxSvg::add( const ofRectangle& arect, float aRoundRadius ) {
	auto rect = std::make_shared<ofxSvgRectangle>();
	rect->setPosition(arect.x, arect.y, 0.0f);
	rect->width = arect.getWidth();
	rect->height = arect.getHeight();
	rect->roundRadius = -1; // force setting round
	rect->setRoundRadius(std::max(0.f,aRoundRadius));
	rect->applyStyle(mCurrentCss);
	_getPushedGroup()->add(rect);
	recalculateLayers();
	mPaths.clear();
	return rect;
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgCircle> ofxSvg::addCircle( float aradius ) {
	return addCircle(glm::vec2(0.f, 0.f), aradius );
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgCircle> ofxSvg::addCircle( const glm::vec2& apos, float aradius ) {
	auto circle = std::make_shared<ofxSvgCircle>();
	circle->setPosition( apos.x, apos.y, 0.f);
	circle->path.setCircleResolution(mCircleResolution);
	circle->radius = -1.f;
	circle->setRadius(std::max(0.f,aradius));
	circle->setPosition( apos.x, apos.y, 0.0f);
	circle->applyStyle(mCurrentCss);
	_getPushedGroup()->add(circle);
	recalculateLayers();
	mPaths.clear();
	return circle;
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgCircle> ofxSvg::addCircle( const glm::vec3& apos, float aradius ) {
	return addCircle( glm::vec2(apos.x, apos.y), aradius );
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgCircle> ofxSvg::addCircle( const float& ax, const float& ay, float aradius ) {
	return addCircle( glm::vec2(ax, ay), aradius );
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgEllipse> ofxSvg::addEllipse( float aradiusX, float aradiusY ) {
	return addEllipse( glm::vec2(0.f, 0.f), aradiusX, aradiusY );
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgEllipse> ofxSvg::addEllipse( const glm::vec2& apos, float aradiusX, float aradiusY ) {
	auto ellipse = std::make_shared<ofxSvgEllipse>();
	ellipse->setPosition(apos.x, apos.y, 0.f);
	
	ellipse->radiusX = aradiusX;
	ellipse->radiusY = aradiusY;
	ellipse->path.setCircleResolution(mCircleResolution);
	ellipse->path.ellipse(apos, aradiusX, aradiusY);
	
	ellipse->applyStyle(mCurrentCss);
	_getPushedGroup()->add(ellipse);
	recalculateLayers();
	mPaths.clear();
	return ellipse;
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgEllipse> ofxSvg::addEllipse( const glm::vec3& apos, float aradiusX, float aradiusY ) {
	return addEllipse( glm::vec2(apos.x, apos.y), aradiusX, aradiusY );
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgEllipse> ofxSvg::addEllipse( const float& ax, const float& ay, float aradiusX, float aradiusY ) {
	return addEllipse( glm::vec2(ax, ay), aradiusX, aradiusY );
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgImage> ofxSvg::addImage( const of::filesystem::path& apath, const ofTexture& atex ) {
	return addImage(glm::vec2(0.f, 0.f), apath, atex.getWidth(), atex.getHeight() );
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgImage> ofxSvg::addImage( const glm::vec2& apos, const of::filesystem::path& apath, const ofTexture& atex ) {
	return addImage( apos, apath, atex.getWidth(), atex.getHeight() );
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgImage> ofxSvg::addImage( const of::filesystem::path& apath, const float& awidth, const float& aheight ) {
	return addImage(glm::vec2(0.f, 0.f), apath, awidth, aheight );
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgImage> ofxSvg::addImage( const glm::vec2& apos, const of::filesystem::path& apath, const float& awidth, const float& aheight ) {
	auto img = std::make_shared<ofxSvgImage>();
	img->filepath = apath;
	img->width = awidth;
	img->height = aheight;
	img->setPosition(apos.x, apos.y, 0.0f);
	_getPushedGroup()->add(img);
	recalculateLayers();
	return img;
}

//--------------------------------------------------------------
std::shared_ptr<ofxSvgImage> ofxSvg::addEmbeddedImage(const glm::vec2& apos, const ofPixels& apixels ) {
	auto img = std::make_shared<ofxSvgImage>();
	img->img.setFromPixels(apixels);
	img->width = apixels.getWidth();
	img->height = apixels.getHeight();
//	_applyModelMatrixToElement( img, glm::vec2(0.f, 0.f) );
	img->setPosition(apos.x, apos.y, 0.0f);
	_getPushedGroup()->add(img);
	recalculateLayers();
	return img;
}

//--------------------------------------------------------------
bool ofxSvg::remove( std::shared_ptr<ofxSvgElement> aelement ) {
	bool bRemoved = ofxSvgGroup::remove(aelement);
	if( bRemoved ) {
		recalculateLayers();
	}
	return bRemoved;
}

////--------------------------------------------------------------
//bool ofxSvg::remove( std::vector<std::shared_ptr<ofxSvgElement> > aelements ) {
//	bool bAllRemoved = ofxSvgGroup::remove(aelements);
//	// we should just recalculate the layers if there was more than one element
//	// since the function only returns true if all of the elements were found and removed.
//	if( aelements.size() > 0 ) {
//		recalculateLayers();
//	}
//	return bAllRemoved;
//}

//--------------------------------------------------------------
void ofxSvg::drawDebug() {
//	Group::draw();
	ofSetColor( ofColor::limeGreen );
	ofNoFill();
	
//	int cindex = 0;
//	for( auto& cp : mCPoints ) {
//		ofSetColor( (float)(cindex % 2) * 255, 200, 60 );
////		ofDrawCircle( cp, (cindex+1) * 1.0f );
//		ofDrawCircle( cp, 3. );
//		cindex ++;
//	}
//	ofFill();
		
	for( std::size_t k = 0; k < mCPoints.size(); k += 3 ) {
		ofSetColor( ofColor::orange );
		ofDrawCircle( mCPoints[k+0], 6.f );
		ofSetColor( ofColor::white );
		ofDrawCircle( mCPoints[k+1], 3.f );
		ofDrawCircle( mCPoints[k+2], 3.f );
		ofDrawLine( mCPoints[k+0], mCPoints[k+1] );
		ofDrawLine( mCPoints[k+0], mCPoints[k+2] );
	}
	
	ofFill();
	
	
	int tcounter = 0;
	for( auto& cp : mCenterPoints ) {
		if(tcounter == 0) {
			ofSetColor( ofColor::cyan );
		} else {
			ofSetColor( ofColor::orange );
		}
		ofDrawCircle(cp, 4.f);
		tcounter++;
	}
	
}

//--------------------------------------------------------------
ofxSvgGroup* ofxSvg::_getPushedGroup() {
	if( mGroupStack.size() > 0 ) {
		return mGroupStack.back().get();
	}
	return this;
}

//--------------------------------------------------------------
ofxSvgCssClass& ofxSvg::_addCssClassFromPath( std::shared_ptr<ofxSvgPath> aSvgPath ) {
	ofxSvgCssClass tcss;
//	tcss.name = aSvgPath->name+"st";
	tcss.name = "st";
	if( aSvgPath->path.isFilled() ) {
		tcss.setFillColor(aSvgPath->path.getFillColor());
	} else {
		tcss.setNoFill();
	}
	
	if( aSvgPath->path.hasOutline() ) {
		tcss.setStrokeColor(aSvgPath->path.getStrokeColor());
		tcss.setStrokeWidth(aSvgPath->path.getStrokeWidth());
	} else {
		tcss.setNoStroke();
	}
	if( !aSvgPath->isVisible() ) {
		tcss.addProperty("display", "none" );
	}
	
	return mSvgCss.getAddClass(tcss);
}

//--------------------------------------------------------------
void ofxSvg::_addCssClassFromPath( std::shared_ptr<ofxSvgPath> aSvgPath, ofXml& anode ) {
	auto& css = _addCssClassFromPath(aSvgPath);
	if( auto xattr = anode.appendAttribute("class") ) {
		xattr.set(css.name);
	}
}

//--------------------------------------------------------------
void ofxSvg::_addCssClassFromImage( std::shared_ptr<ofxSvgImage> aSvgImage, ofXml& anode ) {
	
	if( !aSvgImage->isVisible() ) {
		ofxSvgCssClass tcss;
		tcss.name = "st";
		tcss.addProperty("display", "none" );
		
		auto& addedClass = mSvgCss.getAddClass(tcss);
		
		if( auto xattr = anode.appendAttribute("class") ) {
			xattr.set(addedClass.name);
		}
	}
}

//--------------------------------------------------------------
void ofxSvg::_addCssClassFromTextSpan( std::shared_ptr<ofxSvgText::TextSpan> aSvgTextSpan, ofXml& anode ) {
	
	auto textCss = aSvgTextSpan->getCssClass();
	
	if( textCss.name.empty() ) {
		textCss.name = "ts";
	}
	
	auto& tcss = mSvgCss.getAddClass(textCss);
	
	if( auto xattr = anode.appendAttribute("class") ) {
		xattr.set(tcss.name);
	}
	
//	if( !aSvgTextSpan->isVisible() ) {
//		ofxSvgCssClass tcss;
//		tcss.name = "st";
//		tcss.addProperty("display", "none" );
//		
//		auto& addedClass = mSvgCss.getAddClass(tcss);
//		
//		if( auto xattr = anode.appendAttribute("class") ) {
//			xattr.set(addedClass.name);
//		}
//	}
}

//--------------------------------------------------------------
bool ofxSvg::_toXml( ofXml& aParentNode, std::shared_ptr<ofxSvgElement> aele ) {
	ofXml txml = aParentNode.appendChild( ofxSvgElement::sGetSvgXmlName(aele->getType()));
	if( !aele->getName().empty() ) {
		if( auto iattr = txml.appendAttribute("id")) {
			iattr.set(aele->getName());
		}
	}
	if( aele->getType() == OFXSVG_TYPE_GROUP ) {
		auto tgroup = std::dynamic_pointer_cast<ofxSvgGroup>(aele);
		if( tgroup ) {
			if( tgroup->getNumChildren() > 0 ) {
				for( auto& kid : tgroup->getChildren() ) {
					_toXml( txml, kid );
				}
			}
		}
	} else if( aele->getType() == OFXSVG_TYPE_RECTANGLE ) {
		auto trect = std::dynamic_pointer_cast<ofxSvgRectangle>(aele);
		_addCssClassFromPath( trect, txml );
		
		if( auto xattr = txml.appendAttribute("x")) {
//			xattr.set(trect->getPosition().x);
			xattr.set(trect->getOffsetPathPosition().x);
		}
		if( auto xattr = txml.appendAttribute("y")) {
//			xattr.set(trect->getPosition().y);
			xattr.set(trect->getOffsetPathPosition().y);
		}
		if( auto xattr = txml.appendAttribute("width")) {
			xattr.set(trect->getWidth());
		}
		if( auto xattr = txml.appendAttribute("height")) {
			xattr.set(trect->getHeight());
		}
		if( trect->getRoundRadius() > 0.0f ) {
			if( auto xattr = txml.appendAttribute("rx")) {
				xattr.set(trect->getRoundRadius());
			}
			if( auto xattr = txml.appendAttribute("ry")) {
				xattr.set(trect->getRoundRadius());
			}
		}
		
	} else if( aele->getType() == OFXSVG_TYPE_IMAGE ) {
		auto timage = std::dynamic_pointer_cast<ofxSvgImage>(aele);
		
		_addCssClassFromImage( timage, txml );
		
		if( auto xattr = txml.appendAttribute("width")) {
			xattr.set(timage->width);
		}
		if( auto xattr = txml.appendAttribute("height")) {
			xattr.set(timage->height);
		}
		if( !timage->getFilePath().empty() ) {
			if( auto xattr = txml.appendAttribute("xlink:href")) {
				xattr.set(timage->getFilePath().string());
			}
		} else {
			// check for embedded image //
			if( timage->img.getPixels().isAllocated() ) {
				// embed the pixels //
				if( auto xattr = txml.appendAttribute("xlink:href")) {
					auto base64String = ofxSvgUtils::base64_encode( timage->img.getPixels() );
					std::string encString = "data:image/png;base64,"+base64String;
					xattr.set(encString);
				}
			}
		}
		
		
	} else if( aele->getType() == OFXSVG_TYPE_ELLIPSE ) {
		auto tellipse = std::dynamic_pointer_cast<ofxSvgEllipse>(aele);
		_addCssClassFromPath( tellipse, txml );
		
		if( auto xattr = txml.appendAttribute("cx")) {
			xattr.set(tellipse->getOffsetPathPosition().x);
		}
		if( auto xattr = txml.appendAttribute("cy")) {
			xattr.set(tellipse->getOffsetPathPosition().y);
		}
		if( auto xattr = txml.appendAttribute("rx")) {
			xattr.set(tellipse->radiusX);
		}
		if( auto xattr = txml.appendAttribute("ry")) {
			xattr.set(tellipse->radiusY);
		}
		
	} else if( aele->getType() == OFXSVG_TYPE_CIRCLE ) {
		auto tcircle = std::dynamic_pointer_cast<ofxSvgCircle>(aele);
		_addCssClassFromPath( tcircle, txml );
		
		if( auto xattr = txml.appendAttribute("cx")) {
			xattr.set(tcircle->getOffsetPathPosition().x);
		}
		if( auto xattr = txml.appendAttribute("cy")) {
			xattr.set(tcircle->getOffsetPathPosition().y);
		}
		if( auto xattr = txml.appendAttribute("r")) {
			xattr.set(tcircle->getRadius());
		}
		
	} else if( aele->getType() == OFXSVG_TYPE_PATH ) {
		auto tpath = std::dynamic_pointer_cast<ofxSvgPath>(aele);
		
		_addCssClassFromPath( tpath, txml );
		
		std::stringstream vstr;
		
		if( tpath->path.getMode() == ofPath::Mode::POLYLINES ) {
			
			auto outlines = tpath->path.getOutline();
			for( auto& polyline : outlines ) {
				const auto& pverts = polyline.getVertices();
				if( pverts.size() > 1 ) {
					for( std::size_t i = 0; i < pverts.size(); i++ ) {
						if( i == 0 ) {
							vstr << "M"<<pverts[i].x << " " << pverts[i].y << " ";
						} else {
							vstr << "L"<<pverts[i].x << " " << pverts[i].y << " ";
						}
					}
					if( polyline.isClosed() ) {
						vstr << "Z ";
					}
				}
			}
		} else {
			
//			moveTo,
//			lineTo,
//			curveTo,
//			bezierTo,
//			quadBezierTo,
//			arc,
//			arcNegative,
//			close
			
			// https://www.w3.org/TR/SVG2/paths.html#PathElement
			// add based on the ofPath commands //
			auto& commands = tpath->path.getCommands();
			if( commands.size() > 1 ) {
//				std::stringstream vstr;
				for( auto& command : commands ) {
					if( command.type == ofPath::Command::moveTo ) {
						vstr << "M" << command.to.x << "," << command.to.y << " ";
					} else if( command.type == ofPath::Command::lineTo ) {
						vstr << "L" << command.to.x << "," << command.to.y << " ";
					} else if( command.type == ofPath::Command::curveTo ) {
						// hmm, not sure how to handle this at the moment
					} else if( command.type == ofPath::Command::bezierTo ) {
						vstr << "C" << command.cp1.x << "," << command.cp1.y << " " << command.cp2.x << "," << command.cp2.y << " " << command.to.x << "," << command.to.y << " ";
					} else if( command.type == ofPath::Command::quadBezierTo ) {
						vstr << "Q" << command.cp2.x << "," << command.cp2.y << " " << command.to.x << "," << command.to.y << " ";
					} else if( command.type == ofPath::Command::arc ) {
						// TODO: Not so sure about these
						glm::vec2 ept = glm::vec2(command.to.x + cosf( command.radiusX ), command.to.y + sinf(command.radiusY ));
						vstr << "A" << command.radiusX << "," << command.radiusY << " 0 " << "0,1 " << ept.x << "," <<ept.y << " ";
					} else if( command.type == ofPath::Command::arcNegative ) {
						glm::vec2 ept = glm::vec2(command.to.x + cosf( command.radiusX ), command.to.y + sinf(command.radiusY ));
						vstr << "A" << command.radiusX << "," << command.radiusY << " 0 " << "0,0 " << ept.x << "," <<ept.y << " ";
					} else if( command.type == ofPath::Command::close ) {
						vstr << "Z ";
					}
				}
			}
		}
		
		auto vstring = vstr.str();
		if( !vstring.empty() ) {
			if( auto xattr = txml.appendAttribute("d")) {
				xattr.set(vstring);
			}
		}
		
		
		
	} else if( aele->getType() == OFXSVG_TYPE_TEXT ) {
		auto ttext = std::dynamic_pointer_cast<ofxSvgText>(aele);
		for( auto tspan : ttext->textSpans ) {
			if( auto spanXml = txml.appendChild("tspan")) {
				if( auto xattr = spanXml.appendAttribute("x")) {
					xattr.set(tspan->rect.x);
				}
				if( auto yattr = spanXml.appendAttribute("y")) {
					yattr.set(tspan->rect.y);
				}
				spanXml.set(tspan->getText());
				_addCssClassFromTextSpan( tspan, spanXml );
			}
		}
	}
	
	// figure out if we need a transform attribute
	auto matrixString = getSvgMatrixStringFromElement(aele);
	if( !matrixString.empty() ) {
		if( auto xattr = txml.appendAttribute("transform")) {
			xattr.set(matrixString);
		}
	}
	return txml;
}

//--------------------------------------------------------------
void ofxSvg::_pushCssClass( const ofxSvgCssClass& acss ) {
	mCssClassStack.push_back(acss);
	_buildCurrentSvgCssFromStack();
}

//--------------------------------------------------------------
void ofxSvg::_popCssClass() {
	if( mCssClassStack.size() > 0 ) {
		mCssClassStack.pop_back();
		_buildCurrentSvgCssFromStack();
	}
}

//--------------------------------------------------------------
void ofxSvg::_buildCurrentSvgCssFromStack() {
	// maybe not efficient, but should account for removing / adding
	mCurrentCss.clear();
	mCurrentCss.setClassProperties(mDocumentCss);
	for( auto& css : mCssClassStack ) {
		mCurrentCss.setClassProperties(css);
	}
}


