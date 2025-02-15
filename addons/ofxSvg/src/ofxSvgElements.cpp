//
//  ofxSvgElements.cpp
//
//  Created by Nick Hardeman on 7/31/15.
//

#include "ofxSvgElements.h"
#include "ofGraphics.h"

using std::vector;
using std::string;
using namespace ofx::svg;

std::map< string, Text::Font > Text::fonts;
ofTrueTypeFont Text::defaultFont;

//--------------------------------------------------------------
std::string Element::sGetTypeAsString(SvgType atype) {
	switch (atype) {
		case TYPE_GROUP:
			return "Group";
			break;
		case TYPE_RECTANGLE:
			return "Rectangle";
			break;
		case TYPE_IMAGE:
			return "Image";
			break;
		case TYPE_ELLIPSE:
			return "Ellipse";
			break;
		case TYPE_CIRCLE:
			return "Circle";
			break;
		case TYPE_PATH:
			return "Path";
			break;
		case TYPE_TEXT:
			return "Text";
			break;
		case TYPE_DOCUMENT:
			return "Document";
			break;
		case TYPE_ELEMENT:
			return "Element";
			break;
		default:
			break;
	}
	return "Unknown";
}

//--------------------------------------------------------------
std::string Element::sGetSvgXmlName(SvgType atype) {
	switch (atype) {
		case TYPE_GROUP:
			return "g";
			break;
		case TYPE_RECTANGLE:
			return "rect";
			break;
		case TYPE_IMAGE:
			return "image";
			break;
		case TYPE_ELLIPSE:
			return "ellipse";
			break;
		case TYPE_CIRCLE:
			return "circle";
			break;
		case TYPE_PATH:
			return "path";
			break;
		case TYPE_TEXT:
			return "text";
			break;
		case TYPE_DOCUMENT:
			return "svg";
			break;
		case TYPE_ELEMENT:
			return "Element";
			break;
		default:
			break;
	}
	return "Unknown";
}

//--------------------------------------------------------------
string Element::getTypeAsString() {
	return sGetTypeAsString(getType());
}

//--------------------------------------------------------------
string Element::toString( int nlevel ) {
    
    string tstr = "";
    for( int k = 0; k < nlevel; k++ ) {
        tstr += "   ";
    }
    tstr += getTypeAsString() + " - " + getName() + "\n";
    
    return tstr;
}

////--------------------------------------------------------------
//glm::mat4 ofxSvgBase::getTransformMatrix() {
//    glm::mat4 rmat = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));
//    return rmat;
//}
//
////--------------------------------------------------------------
//ofNode ofxSvgBase::getNodeTransform() {
//    ofNode tnode;
//    tnode.setPosition( pos.x, pos.y, 0.0f );
//    return tnode;
//}

//--------------------------------------------------------------
glm::mat4 Element::getTransformMatrix() {
    glm::mat4 rmat = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));
    if( rotation != 0.0f ) {
        glm::quat rq = glm::angleAxis(ofDegToRad(rotation), glm::vec3(0.f, 0.f, 1.0f ));
        rmat = rmat * glm::toMat4((const glm::quat&)rq);
    }
    if( scale.x != 1.0f || scale.y != 1.0f ) {
        rmat = glm::scale(rmat, glm::vec3(scale.x, scale.y, 1.0f));
    }
    return rmat;
};

//--------------------------------------------------------------
ofNode Element::getNodeTransform() {
	ofNode tnode;// = ofxSvgBase::getNodeTransform();
	tnode.setPosition(pos.x, pos.y, 0.0f);
    if( rotation != 0.0f ) {
        glm::quat rq = glm::angleAxis(ofDegToRad(rotation), glm::vec3(0.f, 0.f, 1.0f ));
        tnode.setOrientation(rq);
    }
    tnode.setScale(1.0f);
    if( scale.x != 1.0f || scale.y != 1.0f ) {
        tnode.setScale(scale.x, scale.y, 1.f );
    }
    return tnode;
}

#pragma mark - Image
//--------------------------------------------------------------
ofRectangle Image::getRectangle() {
	return ofRectangle(pos.x, pos.y, getWidth(), getHeight());
}

//--------------------------------------------------------------
void Image::draw() {
	if( !bTryLoad ) {
		img.load( getFilePath() );
		bTryLoad = true;
	}
	
	if( isVisible() ) {
		if( img.isAllocated() ) {
			ofPushMatrix(); {
				ofTranslate( pos.x, pos.y );
				if( rotation != 0.0 ) ofRotateZDeg( rotation );
				ofScale( scale.x, scale.y );
				if(bUseShapeColor) ofSetColor( getColor() );
				img.draw( 0, 0 );
			} ofPopMatrix();
		}
	}
}

//--------------------------------------------------------------
glm::vec2 Image::getAnchorPointForPercent( float ax, float ay ) {
	glm::vec2 ap = glm::vec2( width * ax * scale.x, height * ay * scale.y );
	ap = glm::rotate(ap, glm::radians(rotation));
	return ap;
}

#pragma mark - Text

//--------------------------------------------------------------
void Text::create() {
    meshes.clear();
    
    // now lets sort the text based on meshes that we need to create //
    vector< std::shared_ptr<TextSpan> > tspans = textSpans;
    
	std::map< string, std::map< int, vector<std::shared_ptr<TextSpan> > > > tspanFonts;
    for( std::size_t i = 0; i < tspans.size(); i++ ) {
        if( tspanFonts.count( tspans[i]->fontFamily ) == 0 ) {
			std::map< int, vector< std::shared_ptr<TextSpan>> > tmapap;
            tspanFonts[ tspans[i]->fontFamily ] = tmapap;
        }
		std::map< int, vector< std::shared_ptr<TextSpan>> >& spanMap = tspanFonts[ tspans[i]->fontFamily ];
        if( spanMap.count(tspans[i]->fontSize) == 0 ) {
            vector< std::shared_ptr<TextSpan> > tvec;
            spanMap[ tspans[i]->fontSize ] = tvec;
        }
        spanMap[ tspans[i]->fontSize ].push_back( tspans[i] );
    }
    
    
    bool bHasFontDirectory = false;
//    cout << "checking directory: " << fdirectory+"/fonts/" << endl;
    string fontsDirectory = ofToDataPath("", true);
    if( fdirectory != "" ) {
        fontsDirectory = fdirectory;//+"/fonts/";
    }
    if( ofFile::doesFileExist( fontsDirectory )) {
        bHasFontDirectory = true;
    }
    
	std::map< string, std::map< int, vector< std::shared_ptr<TextSpan>> > >::iterator mainIt;
    for( mainIt = tspanFonts.begin(); mainIt != tspanFonts.end(); ++mainIt ) {
        if( fonts.count(mainIt->first) == 0 ) {
            Font tafont;
            tafont.fontFamily = mainIt->first;
            fonts[ mainIt->first ] = tafont;
        }
        
        // now create a mesh for the family //
        // map< string, map<int, ofMesh> > meshes;
        if( meshes.count(mainIt->first) == 0 ) {
			std::map< int, ofMesh > tempMeshMap;
            meshes[ mainIt->first ] = tempMeshMap;
        }
        
        Font& tfont = fonts[ mainIt->first ];
		std::map< int, ofMesh >& meshMap = meshes[ mainIt->first ];
        
		std::map< int, vector<std::shared_ptr<TextSpan>> >::iterator vIt;
        for( vIt = mainIt->second.begin(); vIt != mainIt->second.end(); ++vIt ) {
            vector<std::shared_ptr<TextSpan>>& spanSpans = vIt->second;
            bool bFontLoadOk = true;
			if (tfont.sizes.count(vIt->first) == 0) {
//                string _filename, int _fontSize, bool _bAntiAliased, bool _bFullCharacterSet, bool _makeContours, float _simplifyAmt, int _dpi
                // first let's see if the fonts are provided. Some system fonts are .dfont that have several of the faces
                // in them, but OF isn't setup to parse them, so we need each bold, regular, italic, etc to be a .ttf font //
				string tfontPath = tfont.fontFamily;
				if (bHasFontDirectory) {

					ofLogNotice(moduleName()) << __FUNCTION__ << " : " << tfont.fontFamily << " : starting off searching directory : " << fontsDirectory;
					string tNewFontPath = "";
					bool bFoundTheFont = _recursiveFontDirSearch(fontsDirectory, tfont.fontFamily, tNewFontPath);
					if (bFoundTheFont) {
						tfontPath = tNewFontPath;
					}

					/*ofDirectory tfDir;
					tfDir.listDir( fontsDirectory );
					for( int ff = 0; ff < tfDir.size(); ff++ ) {
						ofFile tfFile = tfDir.getFile(ff);
						if( tfFile.getExtension() == "ttf" || tfFile.getExtension() == "otf" ) {
							cout << ff << " - font family: " << tfont.fontFamily << " file name: " << tfFile.getBaseName() << endl;
							if( ofToLower(tfFile.getBaseName()) == ofToLower(tfont.fontFamily) ) {
								ofLogNotice(" >> ofxSvgText found font file for " ) << tfont.fontFamily;
								tfontPath = tfFile.getAbsolutePath();
								break;
							}
						}
					}*/
				}

				ofLogNotice(moduleName()) << __FUNCTION__ << " : Trying to load font from: " << tfontPath;

				if (tfontPath == "") {
					bFontLoadOk = false;
				}
				else {
                    // load(const std::string& _filename, int _fontSize, bool _bAntiAliased, bool _bFullCharacterSet, bool _makeContours, float _simplifyAmt, int _dpi)
					bFontLoadOk = tfont.sizes[vIt->first].load(tfontPath, vIt->first, true, true, false, 0.5, 72);
				}
                if(bFontLoadOk) {
//                    tfont.sizes[ vIt->first ].setSpaceSize( 0.57 );
//                    tfont.sizes[ vIt->first ]       = datFontTho;
                    tfont.textures[ vIt->first ]    = tfont.sizes[ vIt->first ].getFontTexture();
                } else {
                    ofLogError(moduleName()) << __FUNCTION__ << " : error loading font family: " << tfont.fontFamily << " size: " << vIt->first;
					tfont.sizes.erase(vIt->first);
                }
            }
            if( !bFontLoadOk ) continue;
            
            if( meshMap.count(vIt->first) == 0 ) {
                meshMap[ vIt->first ] = ofMesh();
            }
            ofMesh& tmesh = meshMap[ vIt->first ];
            
            if( !tfont.sizes.count( vIt->first ) ) {
				ofLogError(moduleName()) << __FUNCTION__ << " : Could not find that font size in the map: " << vIt->first;
                continue;
            }
            
            ofTrueTypeFont& ttfont = tfont.sizes[ vIt->first ];
            for( std::size_t i = 0; i < spanSpans.size(); i++ ) {
                // create a mesh here //
				std::shared_ptr<TextSpan>& cspan = spanSpans[i];
                if( cspan->text == "" ) continue;
//                cout << "font family: " << cspan.fontFamily << " size: " << cspan.fontSize << " text: " << cspan.text << endl;
                
//                const ofMesh& stringMesh  = ttfont.getStringMesh( "please work", 20, 20 );
                
                ofRectangle tempBounds = ttfont.getStringBoundingBox( cspan->text, 0, 0 );
                float tffontx = bCentered ? cspan->rect.x - tempBounds.width/2 : cspan->rect.x;
//                const ofMesh& stringMesh  = ttfont.getStringMesh( cspan.text, tffontx-ogPos.x, cspan.rect.y-ogPos.y );
                const ofMesh& stringMesh  = ttfont.getStringMesh( cspan->text, tffontx, cspan->rect.y );
                int offsetIndex     = tmesh.getNumVertices();
                
                vector<ofIndexType> tsIndices = stringMesh.getIndices();
                for( std::size_t k = 0; k < tsIndices.size(); k++ ) {
                    tsIndices[k] = tsIndices[k] + offsetIndex;
                }
                
                ofFloatColor tcolor = cspan->color;
                vector< ofFloatColor > tcolors;
                tcolors.assign( stringMesh.getVertices().size(), tcolor );
                
                tmesh.addIndices( tsIndices );
                tmesh.addVertices( stringMesh.getVertices() );
                tmesh.addTexCoords( stringMesh.getTexCoords() );
                tmesh.addColors( tcolors );
            }
        }
    }
    
    // now loop through and set the width and height of the text spans //
    for( std::size_t i = 0; i < textSpans.size(); i++ ) {
        auto& tempSpan = textSpans[i];
        ofTrueTypeFont& tfont = tempSpan->getFont();
        if( tfont.isLoaded() ) {
            ofRectangle tempBounds  = tfont.getStringBoundingBox( tempSpan->text, 0, 0 );
            tempSpan->rect.width     = tempBounds.width;
            tempSpan->rect.height    = tempBounds.height;
            tempSpan->lineHeight     = tfont.getStringBoundingBox("M", 0, 0).height;
//            tempSpan.rect.x         = tempSpan.rect.x - ogPos.x;
//            tempSpan.rect.y         = tempSpan.rect.x - ogPos.x;
            //tempSpan.rect.y         -= tempSpan.lineHeight;
        }
    }
}

//--------------------------------------------------------------
void Text::draw() {
    if( !isVisible() ) return;
//    map< string, map<int, ofMesh> > meshes;
    if(bUseShapeColor) {
        ofSetColor( 255, 255, 255, 255.f * alpha );
    }
	std::map< string, std::map<int, ofMesh> >::iterator mainIt;
    
    ofPushMatrix(); {
        ofSetColor( 255, 0, 0 );
        ofDrawCircle(pos, 6);
        ofNoFill();
//        ofSetColor( 0, 0, 224 );
//        ofDrawCircle( ogPos, 10);
        ofDrawRectangle(getRectangle());
        ofFill();
		
//		ofLogNotice("ofx::svg2") << "Text: num text spans: " << textSpans.size() << " meshes size: " << meshes.size();
        
        ofTranslate( pos.x, pos.y );
        
//        ofSetColor( 255, 255, 255, 255.f * alpha );
        if( rotation > 0 ) ofRotateZDeg( rotation );
        ofTexture* tex = NULL;
        for( mainIt = meshes.begin(); mainIt != meshes.end(); ++mainIt ) {
            string fontFam = mainIt->first;
			std::map< int, ofMesh >::iterator mIt;
            for( mIt = meshes[ fontFam ].begin(); mIt != meshes[ fontFam ].end(); ++mIt ) {
                int fontSize = mIt->first;
                // let's check to make sure that the texture is there, so that we can bind it //
                bool bHasTexture = false;
                // static map< string, Font > fonts;
                if( fonts.count( fontFam ) ) {
                    if( fonts[ fontFam ].textures.count( fontSize ) ) {
                        bHasTexture = true;
                        tex = &fonts[ fontFam ].textures[ fontSize ];
                    }
                }
                
                if( bHasTexture ) tex->bind();
                ofMesh& tMeshMesh = mIt->second;
                if( bUseShapeColor ) {
                    vector< ofFloatColor >& tcolors = tMeshMesh.getColors();
                    for( auto& tc : tcolors ) {
                        if( bOverrideColor ) {
                            tc = _overrideColor;
                        } else {
                            tc.a = alpha;
                        }
                    }
                } else {
                    tMeshMesh.disableColors();
                }
                tMeshMesh.draw();
                if( bHasTexture ) tex->unbind();
                tMeshMesh.enableColors();
            }
        }
    } ofPopMatrix();
    
}

//--------------------------------------------------------------
void Text::draw(const std::string &astring, bool abCentered ) {
	if( textSpans.size() > 0 ) {
		ofPushMatrix(); {
			ofTranslate( pos.x, pos.y );
			if( rotation > 0 ) ofRotateZDeg( rotation );
			textSpans[0]->draw(astring, abCentered );
		} ofPopMatrix();
	} else {
		ofLogVerbose(moduleName()) << __FUNCTION__ << " : no text spans to draw with.";
	}
}

//--------------------------------------------------------------
void Text::draw(const std::string& astring, const ofColor& acolor, bool abCentered ) {
	if( textSpans.size() > 0 ) {
		ofPushMatrix(); {
			ofTranslate( pos.x, pos.y );
			if( rotation > 0 ) ofRotateZDeg( rotation );
			textSpans[0]->draw(astring, acolor, abCentered );
		} ofPopMatrix();
	} else {
		ofLogVerbose(moduleName()) << __FUNCTION__ << " : no text spans to draw with.";
	}
}

//--------------------------------------------------------------
bool Text::_recursiveFontDirSearch(const string& afile, const string& aFontFamToLookFor, string& fontpath) {
	if (fontpath != "") {
		return true;
	}
	ofFile tfFile( afile, ofFile::Reference );
	if (tfFile.isDirectory()) {
		ofLogVerbose(moduleName()) << __FUNCTION__ << " : searching in directory : " << afile << " | " << ofGetFrameNum();
		ofDirectory tdir;
		tdir.listDir(afile);
		tdir.sort();
		for (std::size_t i = 0; i < tdir.size(); i++) {
			bool youGoodOrWhat = _recursiveFontDirSearch(tdir.getPath(i), aFontFamToLookFor, fontpath);
            if( youGoodOrWhat ) {
                return true;
            }
		}
		tdir.close();
	} else {
		if ( tfFile.getExtension() == "ttf" || tfFile.getExtension() == "otf") {
			if (ofToLower( tfFile.getBaseName() ) == ofToLower(aFontFamToLookFor)) {
				ofLogNotice(moduleName()) << __FUNCTION__ << " : found font file for " << aFontFamToLookFor;
				fontpath = tfFile.getAbsolutePath();
				return true;
			}
			string tAltFileName = ofToLower(tfFile.getBaseName());
			ofStringReplace(tAltFileName, " ", "-");
			if (tAltFileName == ofToLower(aFontFamToLookFor)) {
				ofLogNotice(moduleName()) << __FUNCTION__ << " : found font file for " << aFontFamToLookFor;
				fontpath = tfFile.getAbsolutePath();
				return true;
			}
		}
	}
    return false;
}

// must return a reference for some reason here //
//--------------------------------------------------------------
ofTrueTypeFont& Text::TextSpan::getFont() {
    if( Text::fonts.count( fontFamily ) > 0 ) {
        Font& tfont = fonts[ fontFamily ];
        if( tfont.sizes.count(fontSize) > 0 ) {
            return tfont.sizes[ fontSize ];
        }
    }
    return defaultFont;
}

//--------------------------------------------------------------
void Text::TextSpan::draw(const std::string &astring, bool abCentered ) {
	draw( astring, color, abCentered );
}

//--------------------------------------------------------------
void Text::TextSpan::draw(const std::string &astring, const ofColor& acolor, bool abCentered ) {
	ofSetColor( acolor );
	auto& cfont = getFont();
	ofRectangle tempBounds = cfont.getStringBoundingBox( astring, 0, 0 );
	float tffontx = abCentered ? rect.getCenter().x - tempBounds.width/2 : rect.x;
	//	const ofMesh& stringMesh  = cfont.getStringMesh( astring, tffontx, rect.y );
	//	cfont.drawString(astring, tffontx, rect.y + (tempBounds.getHeight() - lineHeight) );
	cfont.drawString(astring, tffontx, rect.y );
}

//--------------------------------------------------------------
ofTrueTypeFont& Text::getFont() {
	if( textSpans.size() > 0 ) {
		return textSpans[0]->getFont();
	}
	ofLogWarning(moduleName()) << __FUNCTION__ << " : no font detected from text spans, returning default font.";
	return defaultFont;
}

//--------------------------------------------------------------
ofColor Text::getColor() {
	if( textSpans.size() > 0 ) {
		return textSpans[0]->color;
	}
	ofLogWarning(moduleName()) << __FUNCTION__ << " : no font detected from text spans, returning path fill color.";
	return path.getFillColor();
}

// get the bounding rect for all of the text spans in this svg'ness
// should be called after create //
//--------------------------------------------------------------
ofRectangle Text::getRectangle() {
    ofRectangle temp( 0, 0, 1, 1 );
    for( std::size_t i = 0; i < textSpans.size(); i++ ) {
        ofRectangle trect = textSpans[i]->rect;
        trect.x = trect.x;// - ogPos.x;
        trect.y = trect.y;// - ogPos.y;
        trect.y -= textSpans[i]->lineHeight;
        if( i == 0 ) {
            temp = trect;
        } else {
            temp.growToInclude( trect );
        }
    }
    
    
    temp.x += pos.x;
    temp.y += pos.y;
    return temp;
}










