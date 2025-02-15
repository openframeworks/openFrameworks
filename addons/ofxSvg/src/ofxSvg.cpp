//
//  ofxSvgParser.cpp
//
//  Created by Nick Hardeman on 8/31/24.
//

#include "ofxSvg.h"
#include "ofUtils.h"
#include <regex>
#include "ofGraphics.h"

using namespace ofx::svg;
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

//--------------------------------------------------------------
bool ofxSvg::load( const of::filesystem::path& fileName ) {
	ofFile mainXmlFile( fileName, ofFile::ReadOnly );
	ofBuffer tMainXmlBuffer( mainXmlFile );
	
	svgPath     = fileName;
	folderPath  = ofFilePath::getEnclosingDirectory( fileName, false );
	
	ofXml xml;
	if( !xml.load(tMainXmlBuffer )) {
		ofLogWarning(moduleName()) << " unable to load svg from " << fileName;
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
			ofLogVerbose(moduleName()) << svgNode.findFirst("style").toString() << "  bounds: " << mBounds;
		} else {
			ofLogVerbose( moduleName() ) << __FUNCTION__ << " : NO svgNode: ";
		}
		
		
		ofXml styleXmlNode = svgNode.findFirst("//style");
		if( styleXmlNode ) {
			ofLogVerbose(moduleName()) << __FUNCTION__ << " : STYLE NODE" << styleXmlNode.getAttribute("type").getValue() << " string: " << styleXmlNode.getValue();
			
			mSvgCss.parse(styleXmlNode.getValue());
			
			ofLogVerbose(moduleName()) << "-----------------------------";
			ofLogVerbose() << mSvgCss.toString();
			ofLogVerbose(moduleName()) << "-----------------------------";
 		} else {
			ofLogVerbose(moduleName()) << __FUNCTION__ << " : NO STYLE NODE";
		}
        
		
		// the defs are added in the _parseXmlNode function //
		_parseXmlNode( svgNode, mChildren );
		
		ofLogVerbose(moduleName()) << " number of defs elements: " << mDefElements.size();
    }
    
    return true;
}

//--------------------------------------------------------------
bool ofxSvg::reload() {
    if( svgPath.empty() ) {
        ofLogError(moduleName()) << __FUNCTION__ << " : svg path is empty, please call load with file path before calling reload";
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
	mCurrentSvgCss.reset();
	mSvgCss.clear();
	mCPoints.clear();
	mCenterPoints.clear();
	
	mCurrentCss.clear();
	
	mGroupStack.clear();
	mModelMatrix = glm::mat4(1.f);
	mModelMatrixStack = std::stack<glm::mat4>();
	
	mPaths.clear();
}

//--------------------------------------------------------------
const int ofxSvg::getTotalLayers(){
	return mCurrentLayer;
}

//--------------------------------------------------------------
void ofxSvg::recalculateLayers() {
	mCurrentLayer = 0;
	auto allKids = getAllChildren(true);
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
		ofLogWarning(moduleName()) << "getPathAt: " << n << " out of bounds for number of paths: " << mPaths.size();
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
			mPaths[i] = spaths[i]->path;
		}
	}
	return mPaths;
}

//--------------------------------------------------------------
void ofxSvg::setFontsDirectory( string aDir ) {
    fontsDirectory = aDir;
    if( fontsDirectory.back() != '/' ) {
        fontsDirectory += '/';
    }
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
void ofxSvg::_parseXmlNode( ofXml& aParentNode, vector< shared_ptr<Element> >& aElements ) {
    
    auto kids = aParentNode.getChildren();
    for( auto& kid : kids ) {
		if( kid.getName() == "g" ) {
			auto fkid = kid.getFirstChild();
			if( fkid ) {
				mCurrentSvgCss.reset();
				auto tgroup = std::make_shared<Group>();
				tgroup->layer = mCurrentLayer += 1.0;
				auto idattr = kid.getAttribute("id");
				if( idattr ) {
					tgroup->name = idattr.getValue();
				}
				
				mCurrentSvgCss = std::make_shared<ofx::svg::CssClass>( _parseStyle(kid) );
				
				aElements.push_back( tgroup );
				_parseXmlNode( kid, tgroup->getChildren() );
			}
		} else if( kid.getName() == "defs") {
			ofLogVerbose(moduleName()) << __FUNCTION__ << " found a defs node.";
			_parseXmlNode(kid, mDefElements );
        } else {
            
            bool bAddOk = _addElementFromXmlNode( kid, aElements );
//            cout << "----------------------------------" << endl;
//            cout << kid.getName() << " kid: " << kid.getAttribute("id").getValue() << " out xml: " << txml.toString() << endl;
        }
    }
}

//--------------------------------------------------------------
bool ofxSvg::_addElementFromXmlNode( ofXml& tnode, vector< shared_ptr<Element> >& aElements ) {
    shared_ptr<Element> telement;
	
	if( tnode.getName() == "use") {
		if( auto hrefAtt = tnode.getAttribute("xlink:href")) {
			ofLogVerbose(moduleName()) << "found a use node with href " << hrefAtt.getValue();
			std::string href = hrefAtt.getValue();
			if( href.size() > 1 && href[0] == '#' ) {
				// try to find by id
				href = href.substr(1, std::string::npos);
				ofLogVerbose(moduleName()) << "going to look for href " << href;
				for( auto & def : mDefElements ) {
					if( def->name == href ) {
						if( def->getType() == ofx::svg::TYPE_RECTANGLE ) {
							auto drect = std::dynamic_pointer_cast<ofx::svg::Rectangle>(def);
							auto nrect = std::make_shared<ofx::svg::Rectangle>( *drect );
							telement = nrect;
						} else if( def->getType() == ofx::svg::TYPE_IMAGE ) {
							auto dimg = std::dynamic_pointer_cast<ofx::svg::Image>(def);
							auto nimg = std::make_shared<ofx::svg::Image>( *dimg );
							ofLogVerbose(moduleName()) << "created an image node with filepath: " << nimg->getFilePath();
							telement = nimg;
						} else if( def->getType() == ofx::svg::TYPE_ELLIPSE ) {
							auto dell= std::dynamic_pointer_cast<ofx::svg::Ellipse>(def);
							auto nell = std::make_shared<ofx::svg::Ellipse>( *dell );
							telement = nell;
						} else if( def->getType() == ofx::svg::TYPE_CIRCLE ) {
							auto dcir= std::dynamic_pointer_cast<ofx::svg::Circle>(def);
							auto ncir = std::make_shared<ofx::svg::Circle>( *dcir );
							telement = ncir;
						} else if( def->getType() == ofx::svg::TYPE_PATH ) {
							auto dpat= std::dynamic_pointer_cast<ofx::svg::Path>(def);
							auto npat = std::make_shared<ofx::svg::Path>( *dpat );
							telement = npat;
						} else if( def->getType() == ofx::svg::TYPE_TEXT ) {
							auto dtex = std::dynamic_pointer_cast<ofx::svg::Text>(def);
							auto ntex = std::make_shared<ofx::svg::Text>( *dtex );
							telement = ntex;
						} else {
							ofLogWarning("Parser") << "could not find type for def : " << def->name;
						}
						break;
					}
				}
			} else {
				ofLogWarning(moduleName()) << "could not parse use node with href : " << href;
			}
		} else {
			ofLogWarning(moduleName()) << "found a use node but no href!";
		}
	} else if( tnode.getName() == "image" ) {
        auto image = std::make_shared<Image>();
        auto wattr = tnode.getAttribute("width");
        if(wattr) image->width  = wattr.getFloatValue();
        auto hattr = tnode.getAttribute("height");
        if(hattr) image->height = hattr.getFloatValue();
        auto xlinkAttr = tnode.getAttribute("xlink:href");
        if( xlinkAttr ) {
            image->filepath = folderPath+xlinkAttr.getValue();
        }
        telement = image;
        
    } else if( tnode.getName() == "ellipse" ) {
        auto ellipse = std::make_shared<Ellipse>();
        auto cxAttr = tnode.getAttribute("cx");
        if(cxAttr) ellipse->pos.x = cxAttr.getFloatValue();
        auto cyAttr = tnode.getAttribute("cy");
        if(cyAttr) ellipse->pos.y = cyAttr.getFloatValue();
        
        auto rxAttr = tnode.getAttribute( "rx" );
        if(rxAttr) ellipse->radiusX = rxAttr.getFloatValue();
        auto ryAttr = tnode.getAttribute( "ry" );
        if(ryAttr) ellipse->radiusY = ryAttr.getFloatValue();
		
		ellipse->path.setCircleResolution(mCircleResolution);
		ellipse->path.setCurveResolution(mCurveResolution);
		// make local so we can apply transform later in the function
		ellipse->path.ellipse({0.f,0.f}, ellipse->radiusX * 2.0f, ellipse->radiusY * 2.0f );
		
		_applyStyleToPath( tnode, ellipse );
        
        telement = ellipse;
	} else if( tnode.getName() == "circle" ) {
		auto circle = std::make_shared<Circle>();
		auto cxAttr = tnode.getAttribute("cx");
		if(cxAttr) circle->pos.x = cxAttr.getFloatValue();
		auto cyAttr = tnode.getAttribute("cy");
		if(cyAttr) circle->pos.y = cyAttr.getFloatValue();
		
		auto rAttr = tnode.getAttribute( "r" );
		if(rAttr) circle->radius = rAttr.getFloatValue();
		
		// make local so we can apply transform later in the function
		// position is from the top left
		circle->path.setCircleResolution(mCircleResolution);
		circle->path.setCurveResolution(mCurveResolution);
		circle->path.circle({0.f,0.f}, circle->radius );
		
		_applyStyleToPath( tnode, circle );
		
		telement = circle;
		
	} else if( tnode.getName() == "line" ) {
		auto telePath = std::make_shared<Path>();
		
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
		auto tpath = std::make_shared<Path>();
		_parsePolylinePolygon(tnode, tpath);
		_applyStyleToPath( tnode, tpath );
		telement = tpath;
	} else if( tnode.getName() == "path" ) {
		auto tpath = std::make_shared<Path>();
		_parsePath( tnode, tpath );
		_applyStyleToPath( tnode, tpath );
		telement = tpath;
    } else if( tnode.getName() == "rect" ) {
        auto rect = std::make_shared<Rectangle>();
        auto xattr = tnode.getAttribute("x");
        if(xattr) rect->rectangle.x       = xattr.getFloatValue();
        auto yattr = tnode.getAttribute("y");
        if(yattr) rect->rectangle.y       = yattr.getFloatValue();
        auto wattr = tnode.getAttribute("width");
        if(wattr) rect->rectangle.width   = wattr.getFloatValue();
        auto hattr = tnode.getAttribute("height");
        if(hattr) rect->rectangle.height  = hattr.getFloatValue();
        rect->pos.x = rect->rectangle.x;
        rect->pos.y = rect->rectangle.y;
		
		auto rxAttr = tnode.getAttribute("rx");
		auto ryAttr = tnode.getAttribute("ry");
		
		rect->path.setCircleResolution(mCircleResolution);
		rect->path.setCurveResolution(mCurveResolution);
		
		// make local so we can apply transform later in the function
		if( !CssClass::sIsNone(rxAttr.getValue()) || !CssClass::sIsNone(ryAttr.getValue())) {
			
			rect->round = std::max(CssClass::sGetFloat(rxAttr.getValue()),
								   CssClass::sGetFloat(ryAttr.getValue()));
			
			rect->path.rectRounded(0.f, 0.f, rect->rectangle.getWidth(), rect->rectangle.getHeight(),
								   rect->round
								   );
			
		} else {
			rect->path.rectangle(0.f, 0.f, rect->getWidth(), rect->getHeight());
		}
        
        telement = rect;
        		
		_applyStyleToPath( tnode, rect );
        
        // this shouldn't be drawn at all, may be a rect that for some reason is generated
        // by text blocks //
        if( !rect->isFilled() && !rect->hasStroke() ) {
			telement->setVisible(false);
        }
        
    } else if( tnode.getName() == "text" ) {
        auto text = std::make_shared<Text>();
        telement = text;
//		std::cout << "has kids: " << tnode.getFirstChild() << " node value: " << tnode.getValue() << std::endl;
        if( tnode.getFirstChild() ) {
            
            auto kids = tnode.getChildren();
            for( auto& kid : kids ) {
                if(kid) {
                    if( kid.getName() == "tspan" ) {
						text->textSpans.push_back( _getTextSpanFromXmlNode( kid ) );
                    }
                }
            }
            
            // this may not be a text block or it may have no text //
            if( text->textSpans.size() == 0 ) {
				text->textSpans.push_back( _getTextSpanFromXmlNode( tnode ) );
            }
        }
        
        string tempFolderPath = folderPath;
        if( tempFolderPath.back() != '/' ) {
            tempFolderPath += '/';
        }
        if( ofDirectory::doesDirectoryExist( tempFolderPath+"fonts/" )) {
            text->setFontDirectory( tempFolderPath+"fonts/" );
        }
        if( fontsDirectory != "" ) {
            if( ofDirectory::doesDirectoryExist(fontsDirectory)) {
                text->setFontDirectory( fontsDirectory );
            }
        }
        
    } else if( tnode.getName() == "g" ) {
		
    }
    
    if( !telement ) {
        return false;
    }
    
    auto idAttr = tnode.getAttribute("id");
    if( idAttr ) {
        telement->name = idAttr.getValue();
    }
    
    if( telement->getType() == TYPE_RECTANGLE || telement->getType() == TYPE_IMAGE || telement->getType() == TYPE_TEXT || telement->getType() == TYPE_CIRCLE || telement->getType() == TYPE_ELLIPSE ) {
        auto transAttr = tnode.getAttribute("transform");
        if( transAttr ) {
//            getTransformFromSvgMatrix( transAttr.getValue(), telement->pos, telement->scale.x, telement->scale.y, telement->rotation );
			setTransformFromSvgMatrixString( transAttr.getValue(), telement );
        }
		
		std::vector<SvgType> typesToApplyTransformToPath = {
			TYPE_RECTANGLE,
			TYPE_CIRCLE,
			TYPE_ELLIPSE
		};
		
		bool bApplyTransformToPath = false;
		for( auto & etype : typesToApplyTransformToPath ) {
			if( etype == telement->getType() ) {
				bApplyTransformToPath = true;
				break;
			}
		}
		
		if( bApplyTransformToPath ) {
			auto epath = std::dynamic_pointer_cast<Path>( telement );
			auto outlines = epath->path.getOutline();
			auto transform = epath->getTransformMatrix();
			for( auto& outline : outlines ) {
				for( auto& v : outline ) {
					v = transform * glm::vec4(v, 1.0f);
				}
			}
			// now we have new outlines, what do we do?
			epath->path.clear();
			bool bFirstOne = true;
			for( auto& outline : outlines ) {
				for( auto& v : outline ) {
					if(bFirstOne) {
						bFirstOne = false;
						epath->path.moveTo(v);
					} else {
						epath->path.lineTo(v);
					}
				}
				if( outline.isClosed() ) {
					epath->path.close();
				}
			}
		}
    }
    
    if( telement->getType() == TYPE_TEXT ) {
        auto text = std::dynamic_pointer_cast<Text>( telement );
        text->ogPos = text->pos;
        text->create();
    }
	
	_applyStyleToElement(tnode, telement);
        
	telement->layer = mCurrentLayer += 1.0;
    aElements.push_back( telement );
    return true;
}

std::vector<glm::vec3> parsePoints(const std::string& input) {
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
	
	// Create vec2 pairs from the values
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
void ofxSvg::_parsePolylinePolygon( ofXml& tnode, std::shared_ptr<Path> aSvgPath ) {
	auto pointsAttr = tnode.getAttribute("points");
	if( !pointsAttr ) {
		ofLogWarning(moduleName()) << __FUNCTION__ << " polyline or polygon does not have a points attriubute.";
		return;
	}
	
	if( pointsAttr.getValue().empty() ) {
		ofLogWarning(moduleName()) << __FUNCTION__ << " polyline or polygon does not have points.";
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
void ofxSvg::_parsePath( ofXml& tnode, std::shared_ptr<Path> aSvgPath ) {
	aSvgPath->path.clear();
	
	auto dattr = tnode.getAttribute("d");
	if( !dattr ) {
		ofLogWarning(moduleName()) << __FUNCTION__ << " path node does not have d attriubute.";
		return;
	}
	
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
//	ofLogNotice(moduleName()) << __FUNCTION__ << " dattr: " << ostring;
	
	if( ostring.empty() ) {
		ofLogError(moduleName()) << __FUNCTION__ << " there is no data in the d string.";
		return;
	}
	
	std::size_t index = 0;
	if( ostring[index] != 'm' && ostring[index] != 'M' ) {
		ofLogWarning(moduleName()) << __FUNCTION__ << " first char is not a m or M, ostring[index]: " << ostring[index];
		return;
	}
	
	glm::vec3 currentPos = {0.f, 0.f, 0.f};
	glm::vec3 secondControlPoint = currentPos;
	glm::vec3 qControlPoint = currentPos;
	
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
		
//		ofLogNotice(moduleName()) << " o : ["<< ostring[index] <<"]";
		
		// up to next valid character //
		std::string currentString;
		bool bFoundValidNextChar = false;
		auto pos = index+1;
		if( pos >= ostring.size() ) {
			ofLogVerbose("svgParser") << "pos is greater than string size: " << pos << " / " << ostring.size();
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
		
		index += currentString.size()+1;
		
		
		if( currentString.empty() ) {
			ofLogVerbose("svgParser") << "currentString is empty: " << cchar;
//			break;
		}
		
		
		ofLogVerbose(moduleName()) << "["<<cchar<<"]: " << currentString;
		
		bool bRelative = false;
		std::vector<glm::vec3> npositions= {glm::vec3(0.f, 0.f, 0.f)};
		std::optional<ofPath::Command::Type> ctype;
		
		// check if we are looking for a position
		if( cchar == 'm' || cchar == 'M' ) {
			if( cchar == 'm' ) {
				bRelative = true;
			}
			npositions = parsePoints(currentString);
			ctype = ofPath::Command::moveTo;
		} else if( cchar == 'v' || cchar == 'V' ) {
			if( cchar == 'v' ) {
				bRelative = true;
				npositions[0].x = 0.f;
			} else {
				npositions[0].x = currentPos.x;
			}
			
			npositions[0].y = ofToFloat(currentString);
			ctype = ofPath::Command::lineTo;
		} else if( cchar == 'H' || cchar == 'h' ) {
			if( cchar == 'h' ) {
				bRelative = true;
				npositions[0].y = 0.f;
			} else {
				npositions[0].y = currentPos.y;
			}
			npositions[0].x = ofToFloat(currentString);
			
			ctype = ofPath::Command::lineTo;
		} else if( cchar == 'L' || cchar == 'l' ) {
			if( cchar == 'l' ) {
				bRelative = true;
			}
			npositions = parsePoints(currentString);
//			for( auto& np : npositions ) {
//				ofLogNotice(moduleName()) << cchar << " line to: " << np;
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
//				ofLogNotice(moduleName()) << cchar << " bezier to: " << np;
//			}
		} else if( cchar == 'Q' || cchar == 'q' || cchar == 'T' || cchar == 't' ) {
			if( cchar == 'q' ) {
				bRelative = true;
			}
			
			ctype = ofPath::Command::quadBezierTo;
			npositions = parsePoints(currentString);
			
//			for( auto& np : npositions ) {
//				ofLogNotice(moduleName()) << " Quad bezier to: " << np;
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
				ofLogError(moduleName()) << "unable to parse arc command, incorrect number of parameters detected: " << arcValues.size();
				ofLogError(moduleName()) << "-- Arc values ---------------------- ";
				for( std::size_t n = 0; n < arcValues.size(); n++ ) {
					ofLogError(moduleName()) << n << ": " << arcValues[n];
				}
				
			}
//			for( auto& np : npositions ) {
//				ofLogNotice(moduleName()) << " arc parsed positions: " << np;
//			}
		}
		
		if( ctype.has_value() ) {
			
//			for( auto& np : npositions ) {
//				ofLogNotice(moduleName()) << cchar << " position: " << np;
//			}
			
			auto prevPos = currentPos;
			
			auto commandT = ctype.value();
			
			if( commandT == ofPath::Command::arc ) {
				if( npositions.size() == 4 ) {
					std::vector<glm::vec3> tpositions = {npositions[3]};
					currentPos = convertToAbsolute(bRelative, currentPos, tpositions );
					npositions[3] = tpositions[0];
				} else {
					ofLogWarning("ofx::svg::Parser") << "invalid number of arc commands.";
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
//				if( commandT == ofPath::Command::moveTo ) {
//					ofLogNotice("ofxSvgParser") << "before current pos is altered: move to: " << npositions[0] << " current Pos: " << currentPos << " relative: " << bRelative;
//				}
				if( npositions.size() > 0 && commandT != ofPath::Command::close ) {
					currentPos = convertToAbsolute(bRelative, currentPos, npositions );
				}
			}
			
//			if( npositions.size() > 0 ) {
//				ofLogNotice("ofxSvgParser") << "before current pos is altered: move to: " << npositions[0] << " current Pos: " << currentPos << " relative: " << bRelative;
//			}
			
			if( commandT != ofPath::Command::bezierTo ) {
				secondControlPoint = currentPos;
			}
			if( commandT != ofPath::Command::quadBezierTo ) {
				qControlPoint = currentPos;
			}
			
			if( commandT == ofPath::Command::moveTo ) {
				aSvgPath->path.moveTo(currentPos);
			} else if( commandT == ofPath::Command::lineTo ) {
				aSvgPath->path.lineTo(currentPos);
			} else if( commandT == ofPath::Command::close ) {
				aSvgPath->path.close();
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
					
//					if( npositions.size() == 2 ) {
//						auto cp2 = (secondControlPoint - prevPos) * -1.f;
//						cp2 += prevPos;
//						npositions.insert(npositions.begin(), cp2 );
//					}
				}
				
				auto tcpos = prevPos;
				
				for( std::size_t k = 0; k < npositions.size(); k +=3 ) {
					aSvgPath->path.bezierTo(npositions[k+0], npositions[k+1], npositions[k+2]);
					secondControlPoint = npositions[k+1];
					
					mCPoints.push_back(prevPos);
					mCPoints.push_back(npositions[k+0]);
					mCPoints.push_back(npositions[k+1]);
					tcpos = npositions[k+2];
					
//					mCPoints.push_back(npositions[k+0]);
//					mCPoints.push_back(npositions[k+1]);
//					mCenterPoints.push_back(npositions[k+2]);
				}
				
//				mCPoints.insert( mCPoints.end(), npositions.begin(), npositions.end() );
				
//				if( npositions.size() == 3 ) {
//					aSvgPath->path.bezierTo(npositions[0], npositions[1], npositions[2]);
//				}
//				
//				secondControlPoint = npositions[1];
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
					
					
//					glm::vec3 cpt(spt.x, ept.y, 0.0f);
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
					
					ofLogVerbose("Arc winding order is ") << worderS << " order: " << windingOrder << " startDiff: " << startDiff << " endDiff: " << endDiff << " xAxisRotation: " << xAxisRotation;
					
					ofPolyline tline;
										
					if( windingOrder < 0 ) {
//						aSvgPath->path.arcNegative(cpt, radii.x, radii.y, glm::degrees(startAngle), glm::degrees(endAngle) );
						tline.arcNegative(cpt, radii.x, radii.y, glm::degrees(startAngle-xrotRad), glm::degrees(endAngle-xrotRad), mCircleResolution );
//						tline.arcNegative(cpt, radii.x, radii.y, glm::degrees(startAngle), glm::degrees(endAngle) );
//						aSvgPath->path.arcNegative(cpt, radii.x, radii.y, glm::degrees(startAngle-xrotRad), glm::degrees(endAngle-xrotRad) );
					} else {
						tline.arc(cpt, radii.x, radii.y, glm::degrees(startAngle-xrotRad), glm::degrees(endAngle-xrotRad), mCircleResolution );
//						tline.arc(cpt, radii.x, radii.y, glm::degrees(startAngle), glm::degrees(endAngle) );
//						aSvgPath->path.arc(cpt, radii.x, radii.y, glm::degrees(startAngle-xrotRad), glm::degrees(endAngle-xrotRad) );
//						aSvgPath->path.arc(cpt, radii.x, radii.y, glm::degrees(startAngle), glm::degrees(endAngle) );
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
					
//					auto centers = findEllipseCenter( spt, ept, radii.x, radii.y );
					
//					ofLogNotice("centers: ") << std::get<0>(centers) << " and " << std::get<1>(centers) << " spt: " << spt << " ept: " << ept << " radii: " << radii;
					
					
					mCenterPoints.push_back(cpt);
//					mCenterPoints.push_back(cpt);
					npositions.clear();
					npositions.push_back(ept);
				} else {
					ofLogWarning("ofx::svg::Parser") << "unable to parse arc segment.";
				}
			}
			
//			mCenterPoints.push_back(currentPos);
//			mCPoints.insert( mCPoints.end(), npositions.begin(), npositions.end() );
		}
		
//		ofLogNotice(moduleName()) << "["<<cchar<<"]: " << currentString;
		
		
		justInCase++;
	}
}

//--------------------------------------------------------------
CssClass ofxSvg::_parseStyle( ofXml& anode ) {
	CssClass css;
	
	if( mCurrentSvgCss ) {
		// apply first if we have a global style //
		for( auto& tprop : mCurrentSvgCss->properties ) {
			css.addProperty(tprop.first, tprop.second);
		}
	}
	
	// now apply all of the other via css classes //
	// now lets figure out if there is any css applied //
	if( auto classAttr = anode.getAttribute("class") ) {
		// get a list of classes, is this separated by commas?
		auto classList = ofSplitString(classAttr.getValue(), ",");
//		ofLogNotice("ofx::svg::Parser") << " going to try and parse style classes string: " << classAttr.getValue();
		for( auto& className : classList ) {
			if( mSvgCss.hasClass(className) ) {
//				ofLogNotice("ofx::svg::Parser") << " has class " << className;
				// now lets try to apply it to the path
				auto& tCss = mSvgCss.getClass(className);
				for( auto& tprop : tCss.properties ) {
					css.addProperty(tprop.first, tprop.second);
				}
			}
		}
	}
	
	// locally set on node overrides the class listing
	// are there any properties on the node?
	if( auto fillAttr = anode.getAttribute("fill")) {
		css.addProperty("fill", fillAttr.getValue());
	}
	if( auto strokeAttr = anode.getAttribute("stroke")) {
		css.addProperty("stroke", strokeAttr.getValue());
	}
	
	if( auto strokeWidthAttr = anode.getAttribute("stroke-width")) {
		css.addProperty("stroke-width", strokeWidthAttr.getValue());
	}
	
	if( auto ffattr = anode.getAttribute("font-family") ) {
		std::string tFontFam = ffattr.getValue();
		ofStringReplace( tFontFam, "'", "" );
		css.addProperty("font-family", tFontFam);
	}
	
	if( auto fsattr = anode.getAttribute("font-size") ) {
		css.addProperty("font-size", fsattr.getValue() );
	}
	
	// and lastly style
	if( auto styleAttr = anode.getAttribute("style") ) {
		css.addProperties(styleAttr.getValue());
	}
	
	// override anything else if set directly on the node
	if( auto disAttr = anode.getAttribute("display") ) {
		css.addProperties(disAttr.getValue());
	}
	
	return css;
}

//--------------------------------------------------------------
void ofxSvg::_applyStyleToElement( ofXml& tnode, std::shared_ptr<Element> aEle ) {
	auto css = _parseStyle(tnode);
//	ofLogNotice("_applyStyleToElement" ) << " " << aEle->name << " -----";
	if( css.hasAndIsNone("display")) {
//		ofLogNotice("parser") << "setting element to invisible: " << aEle->name;
		aEle->setVisible(false);
	}
}

//--------------------------------------------------------------
void ofxSvg::_applyStyleToPath( ofXml& tnode, std::shared_ptr<Path> aSvgPath ) {
	auto css = _parseStyle(tnode);
	_applyStyleToPath(css, aSvgPath);
}

//--------------------------------------------------------------
void ofxSvg::_applyStyleToPath( CssClass& aclass, std::shared_ptr<Path> aSvgPath ) {
	// now lets figure out if there is any css applied //
	
	if( aclass.hasProperty("fill")) {
		if( !aclass.isNone("fill")) {
			aSvgPath->path.setFillColor(aclass.getColor("fill"));
		} else {
			aSvgPath->path.setFilled(false);
		}
	} else {
//		aSvgPath->path.setFilled(false);
		aSvgPath->path.setFillColor(ofColor(0));
	}
	
	if( !aclass.isNone("stroke") ) {
		aSvgPath->path.setStrokeColor(aclass.getColor("stroke"));
	}
	
	if( aclass.hasProperty("stroke-width")) {
		if( aclass.isNone("stroke-width")) {
			aSvgPath->path.setStrokeWidth(0.f);
		} else {
			aSvgPath->path.setStrokeWidth( aclass.getFloatValue("stroke-width", 0.f));
		}
	} else {
		// default with no value is 1.f
//		aSvgPath->path.setStrokeWidth(1.f);
	}
	
	// if the color is not set and the width is not set, then it should be 0
	if( !aclass.isNone("stroke") ) {
		if( !aclass.hasProperty("stroke-width")) {
			aSvgPath->path.setStrokeWidth(1.f);
		}
	}
}

//--------------------------------------------------------------
void ofxSvg::_applyStyleToText( ofXml& anode, std::shared_ptr<Text::TextSpan> aTextSpan ) {
	auto css = _parseStyle(anode);
	_applyStyleToText(css, aTextSpan);
}

//--------------------------------------------------------------
void ofxSvg::_applyStyleToText( CssClass& aclass, std::shared_ptr<Text::TextSpan> aTextSpan ) {
	// default font family
	aTextSpan->fontFamily    = aclass.getValue("font-family", "Arial");
	aTextSpan->fontSize      = aclass.getIntValue("font-size", 18 );
	aTextSpan->color 		= aclass.getColor("fill");
}
	


//--------------------------------------------------------------
glm::vec3 ofxSvg::_parseMatrixString(const std::string& input, const std::string& aprefix, bool abDefaultZero ) {
//	std::string prefix = aprefix+"(";
//	std::string suffix = ")";
	ofLogVerbose(moduleName()) << __FUNCTION__ << " input: " << input;;
	std::string searchStr = aprefix + "(";
	size_t startPos = input.find(searchStr);
	
	if (startPos != std::string::npos) {
		startPos += searchStr.size();
		size_t endPos = input.find(")", startPos);
		
		if (endPos != std::string::npos) {
			// Extract the part inside the parentheses
			std::string inside = input.substr(startPos, endPos - startPos);
			
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
//bool Parser::getTransformFromSvgMatrix( string aStr, glm::vec2& apos, float& scaleX, float& scaleY, float& arotation ) {
bool ofxSvg::setTransformFromSvgMatrixString( string aStr, std::shared_ptr<Element> aele ) {
    
	aele->scale = glm::vec2(1.0f, 1.0f);
	aele->rotation = 0.0;
	aele->mModelRotationPoint = glm::vec2(0.0f, 0.0f);
	//TODO: implement matrix push and pop structure, similar to renderers
	ofLogVerbose(moduleName()) << __FUNCTION__ << " going to parse string: " << aStr << " pos: " << aele->pos;
	
	glm::mat4 mat = glm::mat4(1.f);
	
	if( ofIsStringInString(aStr, "translate")) {
		auto transStr = aStr;
		auto tp = _parseMatrixString(transStr, "translate", false );
		ofLogVerbose(moduleName()) << __FUNCTION__ << " translate: " << tp;
//		apos += tp;
		mat = glm::translate(glm::mat4(1.0f), glm::vec3(tp.x, tp.y, 0.0f));
	} else {
		mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.0f));
	}
	
	if( ofIsStringInString(aStr, "rotate")) {
		auto transStr = aStr;
		auto tr = _parseMatrixString(transStr, "rotate", true );
		aele->rotation = tr.x;
		if( aele->rotation != 0.f ) {
			glm::vec2 rcenter(0.f, 0.f);
			if( tr.y != 0.0f || tr.z != 0.0f ) {
				rcenter.x = tr.y;
				rcenter.y = tr.z;
				
				aele->mModelRotationPoint = rcenter;
				
				glm::vec3 pivot(rcenter.x, rcenter.y, 0.f);
				// Step 1: Translate to pivot (move pivot to origin)
				glm::mat4 toOrigin = glm::translate(glm::mat4(1.0f), -pivot );
				
				// Step 2: Apply rotation
				glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(aele->rotation), glm::vec3(0.f, 0.f, 1.f) );
				
				// Step 3: Translate back to original position
				glm::mat4 backToPivot = glm::translate(glm::mat4(1.0f), pivot);
				
				// Apply transformations in the correct order: T_back * R * T_origin * Original_Transform
				mat = backToPivot * rotation * toOrigin * mat;
			} else {
				mat = mat * glm::toMat4((const glm::quat&)glm::angleAxis(glm::radians(aele->rotation), glm::vec3(0.f, 0.f, 1.f)));
			}
//			ofLogNotice("svg parser") << "rcenter: " << rcenter.x << ", " << rcenter.y;
		}
		ofLogVerbose(moduleName()) << __FUNCTION__ << " arotation: " << aele->rotation;
	}
	
	if( ofIsStringInString(aStr, "scale")) {
		auto transStr = aStr;
		auto ts = _parseMatrixString(transStr, "scale", false );
		aele->scale.x = ts.x;
		aele->scale.y = ts.y;
		ofLogVerbose(moduleName()) << __FUNCTION__ << " scale: " << ts;
		
		mat = glm::scale(mat, glm::vec3(aele->scale.x, aele->scale.y, 1.f));
	}
	
	glm::vec3 pos3 = mat * glm::vec4( aele->pos.x, aele->pos.y, 0.0f, 1.f );
	aele->pos.x = pos3.x;
	aele->pos.y = pos3.y;
	
	
	if( ofIsStringInString(aStr, "matrix")) {
		auto matrix = aStr;
		ofStringReplace(matrix, "matrix(", "");
		ofStringReplace(matrix, ")", "");
		vector<string> matrixNum = ofSplitString(matrix, " ", false, true);
		vector<float> matrixF;
		for(std::size_t i = 0; i < matrixNum.size(); i++){
			matrixF.push_back(ofToFloat(matrixNum[i]));
//			std::cout << " matrix[" << i << "] = " << matrixF[i] << " string version is " << matrixNum[i] << std::endl;
		}
		
		if( matrixNum.size() == 6 ) {
			
			mat = glm::translate(glm::mat4(1.0f), glm::vec3(matrixF[4], matrixF[5], 0.0f));
			
			aele->rotation = glm::degrees( atan2f(matrixF[1],matrixF[0]) );
			if( aele->rotation != 0.f ) {
				mat = mat * glm::toMat4((const glm::quat&)glm::angleAxis(glm::radians(aele->rotation), glm::vec3(0.f, 0.f, 1.f)));
			}
			
			aele->scale.x = glm::sqrt(matrixF[0] * matrixF[0] + matrixF[1] * matrixF[1]);
			aele->scale.y = glm::sqrt(matrixF[2] * matrixF[2] + matrixF[3] * matrixF[3]);
			
			mat = glm::scale(mat, glm::vec3(aele->scale.x, aele->scale.y, 1.f));
			
			pos3 = mat * glm::vec4( aele->pos.x, aele->pos.y, 0.0f, 1.f );
			
			aele->pos.x = pos3.x;
			aele->pos.y = pos3.y;
			
//			apos.x = matrixF[4];
//			apos.y = matrixF[5];
//			
//			scaleX = std::sqrtf(matrixF[0] * matrixF[0] + matrixF[1] * matrixF[1]) * (float)ofSign(matrixF[0]);
//			scaleY = std::sqrtf(matrixF[2] * matrixF[2] + matrixF[3] * matrixF[3]) * (float)ofSign(matrixF[3]);
//			
//			arotation = glm::degrees( std::atan2f(matrixF[2],matrixF[3]) );
//			if( scaleX < 0 && scaleY < 0 ){
//				
//			}else{
//				arotation *= -1.0f;
//			}
			//        cout << " rotation is " << arotation << endl;
//			std::cout << "matrix rotation is " << arotation << " ScaleX: " << scaleX << " scaleY: " << scaleY << " apos: " << apos << std::endl;
			
//			return true;
		}
	}
    return false;
}

//--------------------------------------------------------------
std::string ofxSvg::getSvgMatrixStringFromElement( std::shared_ptr<Element> aele ) {
	// matrix(1 0 0 1 352.4516 349.0799)">

	// there's probably a better way to determine if this should be rotated in a certain way
	if( aele->mModelRotationPoint.x != 0.0f || aele->mModelRotationPoint.y != 0.0f ) {
		glm::vec2 rcenter(0.f, 0.f);
		rcenter.x = aele->mModelRotationPoint.x;
		rcenter.y = aele->mModelRotationPoint.y;
		
		std::ostringstream matrixStream;
		matrixStream << std::fixed << std::setprecision(6) << "rotate(" << aele->rotation << " " << rcenter.x << " " << rcenter.y <<")";
		if( aele->scale.x != 1.f || aele->scale.y != 1.f ) {
			matrixStream << " scale(" << aele->scale.x << " " << aele->scale.y <<")";
		}
		return matrixStream.str();
		
	} else {
		
		// Create the transformation matrix
		glm::mat4 transform = glm::mat4(1.0f); // Identity matrix
											   //	transform = glm::translate(transform, glm::vec3(aele->pos, 0.0f) );
		transform = glm::translate(transform, glm::vec3(aele->mModelPos, 0.0f) );
		
		transform = glm::rotate(transform, glm::radians(aele->rotation), glm::vec3( 0.0f, 0.0f, 1.f));
		
		
		transform = glm::scale(transform, glm::vec3(aele->scale, 1.0f) );
		
		// Extract the 2D matrix values (first two rows and three columns)
		float a = transform[0][0]; // m00
		float b = transform[0][1]; // m01
		float c = transform[1][0]; // m10
		float d = transform[1][1]; // m11
		float e = transform[3][0]; // m20 (translation x)
		float f = transform[3][1]; // m21 (translation y)
		
		// Create the SVG transform matrix string
		std::ostringstream matrixStream;
		matrixStream << std::fixed << std::setprecision(6)
		<< "matrix(" << a << " " << b << " " << c << " " << d << " " << e << " " << f << ")";
		return matrixStream.str();
	}
	return "";
	
}

//--------------------------------------------------------------
std::shared_ptr<Text::TextSpan> ofxSvg::_getTextSpanFromXmlNode( ofXml& anode ) {
	auto tspan = std::make_shared<Text::TextSpan>();;
    
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
    
    tspan->text          = tText;
    tspan->rect.x        = tx;
    tspan->rect.y        = ty;
	
	_applyStyleToText(anode, tspan);
    
    return tspan;
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
	std::shared_ptr<ofx::svg::Group> cgroup;
	if( mGroupStack.size() > 0 ) {
		mGroupStack.back()->get<ofx::svg::Group>( apath );
	} else {
		cgroup = get<ofx::svg::Group>( apath );
	}
	
	if( cgroup ) {
		pushGroup(cgroup);
	} else {
		ofLogWarning("ofx::svg::Parser") << "could not find group with path " << apath;
	}
}

//--------------------------------------------------------------
void ofxSvg::pushGroup( const std::shared_ptr<ofx::svg::Group>& agroup ) {
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
	mCurrentCss.setFillColor(acolor);
}

//--------------------------------------------------------------
void ofxSvg::setFilled(bool abFilled) {
	if( abFilled ) {
		mCurrentCss.setFillColor(mFillColor);
	} else {
		mCurrentCss.setNoFill();
	}
}

//--------------------------------------------------------------
void ofxSvg::setStrokeColor(ofColor acolor) {
	mStrokeColor = acolor;
	mCurrentCss.setStrokeColor(acolor);
}

//--------------------------------------------------------------
void ofxSvg::setStrokeWidth(float aLineWidth) {
	mCurrentCss.setStrokeWidth(aLineWidth);
}

//--------------------------------------------------------------
void ofxSvg::setHasStroke(bool abStroke) {
	if( abStroke ) {
		mCurrentCss.setStrokeColor(mStrokeColor);
	} else {
		mCurrentCss.setNoStroke();
	}
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Group> ofxSvg::addGroup(std::string aname) {
	auto tgroup = std::make_shared<Group>();
	tgroup->name = aname;
	_getPushedGroup()->add(tgroup);
	recalculateLayers();
	return tgroup;
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Path> ofxSvg::add( const ofPath& apath ) {
	auto path = std::make_shared<Path>();
	path->path = apath;
//	_config(path);
	_applyModelMatrixToElement( path, glm::vec2(0.f, 0.f) );
	_applyStyleToPath( mCurrentCss, path );
	_getPushedGroup()->add(path);
	recalculateLayers();
	mPaths.clear();
	return path;
}

//--------------------------------------------------------------
std::vector< std::shared_ptr<ofx::svg::Path> > ofxSvg::add( const std::vector<ofPath>& apaths ) {
	std::vector< std::shared_ptr<Path> > rpaths;
	for( auto& path : apaths ) {
		rpaths.push_back( add(path) );
	}
	return rpaths;
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Path> ofxSvg::add( const ofPolyline& apoly ) {
	if( apoly.size() < 2 ) {
		return std::shared_ptr<Path>();
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
std::vector< std::shared_ptr<ofx::svg::Path> > ofxSvg::add( const std::vector<ofPolyline>& apolys ) {
	std::vector< std::shared_ptr<Path> > rpaths;
	for( auto& poly : apolys ) {
		rpaths.push_back( add(poly) );
	}
	return rpaths;
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Rectangle> ofxSvg::add( const ofRectangle& arect ) {
	return add( arect, 0.0f);
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Rectangle> ofxSvg::add( const ofRectangle& arect, float aRoundRadius ) {
	auto rect = std::make_shared<Rectangle>();
	rect->rectangle = arect;
//	rect->pos = arect.getPosition();
	_applyModelMatrixToElement( rect, arect.getPosition() );
	rect->round = aRoundRadius;
	rect->path.rectangle(arect);
//	_config(rect);
	_applyStyleToPath( mCurrentCss, rect );
	_getPushedGroup()->add(rect);
	recalculateLayers();
	mPaths.clear();
	return rect;
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Circle> ofxSvg::addCircle( float aradius ) {
	return addCircle(glm::vec2(0.f, 0.f), aradius );
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Circle> ofxSvg::addCircle( const glm::vec2& apos, float aradius ) {
	auto circle = std::make_shared<Circle>();
//	circle->pos = apos;
	_applyModelMatrixToElement( circle, apos );
	circle->radius = aradius;
	circle->path.setCircleResolution(mCircleResolution);
	circle->path.circle(apos, aradius);
//	_config(circle);
	_applyStyleToPath( mCurrentCss, circle );
	_getPushedGroup()->add(circle);
	recalculateLayers();
	mPaths.clear();
	return circle;
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Circle> ofxSvg::addCircle( const glm::vec3& apos, float aradius ) {
	return addCircle( glm::vec2(apos.x, apos.y), aradius );
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Circle> ofxSvg::addCircle( const float& ax, const float& ay, float aradius ) {
	return addCircle( glm::vec2(ax, ay), aradius );
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Ellipse> ofxSvg::addEllipse( float aradiusX, float aradiusY ) {
	return addEllipse( glm::vec2(0.f, 0.f), aradiusX, aradiusY );
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Ellipse> ofxSvg::addEllipse( const glm::vec2& apos, float aradiusX, float aradiusY ) {
	auto ellipse = std::make_shared<Ellipse>();
	_applyModelMatrixToElement( ellipse, apos );
	
	ellipse->radiusX = aradiusX;
	ellipse->radiusY = aradiusY;
	ellipse->path.setCircleResolution(mCircleResolution);
	ellipse->path.ellipse(apos, aradiusX, aradiusY);
	
	_applyStyleToPath( mCurrentCss, ellipse );
	_getPushedGroup()->add(ellipse);
	recalculateLayers();
	mPaths.clear();
	return ellipse;
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Ellipse> ofxSvg::addEllipse( const glm::vec3& apos, float aradiusX, float aradiusY ) {
	return addEllipse( glm::vec2(apos.x, apos.y), aradiusX, aradiusY );
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Ellipse> ofxSvg::addEllipse( const float& ax, const float& ay, float aradiusX, float aradiusY ) {
	return addEllipse( glm::vec2(ax, ay), aradiusX, aradiusY );
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Image> ofxSvg::addImage( const of::filesystem::path& apath, const ofTexture& atex ) {
	return addImage(glm::vec2(0.f, 0.f), apath, atex );
}

//--------------------------------------------------------------
std::shared_ptr<ofx::svg::Image> ofxSvg::addImage( const glm::vec2& apos, const of::filesystem::path& apath, const ofTexture& atex ) {
	auto img = std::make_shared<Image>();
	img->filepath = apath;
	img->width = atex.getWidth();
	img->height = atex.getHeight();
	_applyModelMatrixToElement( img, apos );
	_getPushedGroup()->add(img);
	recalculateLayers();
	return img;
}

//----------------------------------------------------------
void ofxSvg::pushMatrix() {
	mModelMatrixStack.push(mModelMatrix);
}

//----------------------------------------------------------
bool ofxSvg::popMatrix() {
	if( !mModelMatrixStack.empty() ) {
		mModelMatrix = mModelMatrixStack.top();
		mModelMatrixStack.pop();
		return true;
	} else {
		loadIdentityMatrix();
	}
	return false;
}

//----------------------------------------------------------
void ofxSvg::translate(const glm::vec2 & p) {
	translate(p.x, p.y);
}

//----------------------------------------------------------
void ofxSvg::translate(float x, float y) {
	mModelMatrix = glm::translate(mModelMatrix, glm::vec3(x, y, 0.0f));
}

//----------------------------------------------------------
void ofxSvg::scale(float xAmnt, float yAmnt) {
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(xAmnt, yAmnt, 1.f));
}

//----------------------------------------------------------
void ofxSvg::rotateRadians(float aradians) {
	mModelMatrix = glm::rotate(mModelMatrix, aradians, glm::vec3(0.f, 0.f, 1.f));
}

//----------------------------------------------------------
void ofxSvg::rotateDegrees(float adegrees) {
	rotateRadians( ofDegToRad(adegrees));
}

//----------------------------------------------------------
void ofxSvg::multMatrix(const glm::mat4 & m) {
	mModelMatrix = mModelMatrix * m;
}

//----------------------------------------------------------
void ofxSvg::loadMatrix(const glm::mat4 & m) {
	mModelMatrix = m;
}

//----------------------------------------------------------
void ofxSvg::loadIdentityMatrix() {
	mModelMatrix = glm::mat4(1.f);
}


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
	
	ofSetColor( ofColor::orange );
	for( auto& cp : mCenterPoints ) {
		ofDrawCircle(cp, 4.f);
	}
	
}

//--------------------------------------------------------------
ofx::svg::Group* ofxSvg::_getPushedGroup() {
	if( mGroupStack.size() > 0 ) {
		return mGroupStack.back().get();
	}
	return this;
}

//--------------------------------------------------------------
bool ofxSvg::_hasPushedMatrix() {
	return mModelMatrix != glm::mat4(1.0f);
}

//--------------------------------------------------------------
void ofxSvg::_applyModelMatrixToElement( std::shared_ptr<Element> aele, glm::vec2 aDefaultPos ) {
	if(_hasPushedMatrix() ) {
		aele->pos = aDefaultPos;
		aele->mModelPos = _getPos2d(mModelMatrix);
		aele->rotation = glm::degrees(_getZRotationRadians(mModelMatrix));
		aele->scale = _getScale2d(mModelMatrix);
		
	} else {
		aele->mModelPos = glm::vec2(0.f, 0.f);
		aele->pos = aDefaultPos;
	}
}

//--------------------------------------------------------------
glm::vec2 ofxSvg::_getPos2d(const glm::mat4& amat) {
	// Extract translation (position)
	return glm::vec2(amat[3][0], amat[3][1]);
}

//--------------------------------------------------------------
glm::vec2 ofxSvg::_getScale2d(const glm::mat4& amat) {
	// Extract scale (length of column vectors)
	return glm::vec2(glm::length(glm::vec2(amat[0][0], amat[0][1])), // Length of first column
					 glm::length(glm::vec2(amat[1][0], amat[1][1])) // Length of second column
					 );
}

// Function to extract Z-axis rotation (in degrees) from a glm::mat4
//--------------------------------------------------------------
float ofxSvg::_getZRotationRadians(const glm::mat4& amat) {
	// Normalize the first column (remove scale effect)
	glm::vec2 xAxis = glm::vec2(amat[0][0], amat[0][1]);
	if( glm::length2(xAxis) > 0.0f ) {
		xAxis = glm::normalize(xAxis);
	} else {
		return 0.0f;
	}
	
	// Compute rotation angle using atan2
	float angleRadians = atan2f(xAxis.y, xAxis.x);
	return angleRadians;
}

//--------------------------------------------------------------
CssClass& ofxSvg::_addCssClassFromPath( std::shared_ptr<Path> aSvgPath ) {
	CssClass tcss;
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
void ofxSvg::_addCssClassFromPath( std::shared_ptr<Path> aSvgPath, ofXml& anode ) {
	
//	ofLogNotice("Parser::_addCssClassFromPath") << mSvgCss.toString();
	
	auto& css = _addCssClassFromPath(aSvgPath);
	
//	ofLogNotice("Parser::_addCssClassFromPath") << "adding: " << css.name << " fill: " << aSvgPath->getFillColor() << std::endl << mSvgCss.toString();
	
	if( auto xattr = anode.appendAttribute("class") ) {
		xattr.set(css.name);
	}
}

//--------------------------------------------------------------
void ofxSvg::_addCssClassFromImage( std::shared_ptr<Image> aSvgImage, ofXml& anode ) {
	
	if( !aSvgImage->isVisible() ) {
		CssClass tcss;
		tcss.name = "st";
		tcss.addProperty("display", "none" );
		
		auto& addedClass = mSvgCss.getAddClass(tcss);
		
		if( auto xattr = anode.appendAttribute("class") ) {
			xattr.set(addedClass.name);
		}
	}
}

//--------------------------------------------------------------
bool ofxSvg::_toXml( ofXml& aParentNode, std::shared_ptr<ofx::svg::Element> aele ) {
	ofXml txml = aParentNode.appendChild( ofx::svg::Element::sGetSvgXmlName(aele->getType()));
	if( !aele->getName().empty() ) {
		if( auto iattr = txml.appendAttribute("id")) {
			iattr.set(aele->getName());
		}
	}
	if( aele->getType() == ofx::svg::TYPE_GROUP ) {
		auto tgroup = std::dynamic_pointer_cast<ofx::svg::Group>(aele);
		if( tgroup ) {
			if( tgroup->getNumChildren() > 0 ) {
				for( auto& kid : tgroup->getChildren() ) {
					_toXml( txml, kid );
				}
			}
		}
	} else if( aele->getType() == ofx::svg::TYPE_RECTANGLE ) {
		auto trect = std::dynamic_pointer_cast<ofx::svg::Rectangle>(aele);
		_addCssClassFromPath( trect, txml );
		
		if( auto xattr = txml.appendAttribute("x")) {
			xattr.set(trect->pos.x);
		}
		if( auto xattr = txml.appendAttribute("y")) {
			xattr.set(trect->pos.y);
		}
		if( auto xattr = txml.appendAttribute("width")) {
			xattr.set(trect->rectangle.getWidth());
		}
		if( auto xattr = txml.appendAttribute("height")) {
			xattr.set(trect->rectangle.getHeight());
		}
		if( trect->round > 0.0f ) {
			if( auto xattr = txml.appendAttribute("rx")) {
				xattr.set(trect->round);
			}
			if( auto xattr = txml.appendAttribute("ry")) {
				xattr.set(trect->round);
			}
		}
		
	} else if( aele->getType() == ofx::svg::TYPE_IMAGE ) {
		auto timage = std::dynamic_pointer_cast<ofx::svg::Image>(aele);
		
		_addCssClassFromImage( timage, txml );
		
		if( auto xattr = txml.appendAttribute("width")) {
			xattr.set(timage->width);
		}
		if( auto xattr = txml.appendAttribute("height")) {
			xattr.set(timage->height);
		}
		if( !timage->getFilePath().empty() ) {
			if( auto xattr = txml.appendAttribute("xlink:href")) {
				xattr.set(timage->getFilePath());
			}
		}
		
		
	} else if( aele->getType() == ofx::svg::TYPE_ELLIPSE ) {
		auto tellipse = std::dynamic_pointer_cast<ofx::svg::Ellipse>(aele);
		_addCssClassFromPath( tellipse, txml );
		
		if( auto xattr = txml.appendAttribute("cx")) {
			xattr.set(tellipse->pos.x);
		}
		if( auto xattr = txml.appendAttribute("cy")) {
			xattr.set(tellipse->pos.y);
		}
		if( auto xattr = txml.appendAttribute("rx")) {
			xattr.set(tellipse->radiusX);
		}
		if( auto xattr = txml.appendAttribute("ry")) {
			xattr.set(tellipse->radiusY);
		}
		
	} else if( aele->getType() == ofx::svg::TYPE_CIRCLE ) {
		auto tcircle = std::dynamic_pointer_cast<ofx::svg::Circle>(aele);
		_addCssClassFromPath( tcircle, txml );
		
		if( auto xattr = txml.appendAttribute("cx")) {
			xattr.set(tcircle->pos.x);
		}
		if( auto xattr = txml.appendAttribute("cy")) {
			xattr.set(tcircle->pos.y);
		}
		if( auto xattr = txml.appendAttribute("r")) {
			xattr.set(tcircle->getRadius());
		}
		
	} else if( aele->getType() == ofx::svg::TYPE_PATH ) {
		auto tpath = std::dynamic_pointer_cast<ofx::svg::Path>(aele);
		
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
		
		
		
	} else if( aele->getType() == ofx::svg::TYPE_TEXT ) {
		// TODO: Maybe at some point ;/
	}
	
	// figure out if we need a transform attribute
	if( aele->getType() == TYPE_IMAGE || aele->rotation != 0.0f || aele->scale.x != 1.0f || aele->scale.y != 1.0f ) {
		if( auto xattr = txml.appendAttribute("transform")) {
			xattr.set( getSvgMatrixStringFromElement(aele) );
		}
	}
	
	
	
	return txml;
}


