#include "ofxSvgElements.h"
#include "ofGraphics.h"
#include <regex>

using std::vector;
using std::string;

//--------------------------------------------------------------
std::string ofxSvgElement::sGetTypeAsString(ofxSvgType atype) {
	switch (atype) {
		case OFXSVG_TYPE_GROUP:
			return "Group";
			break;
		case OFXSVG_TYPE_RECTANGLE:
			return "Rectangle";
			break;
		case OFXSVG_TYPE_IMAGE:
			return "Image";
			break;
		case OFXSVG_TYPE_ELLIPSE:
			return "Ellipse";
			break;
		case OFXSVG_TYPE_CIRCLE:
			return "Circle";
			break;
		case OFXSVG_TYPE_PATH:
			return "Path";
			break;
		case OFXSVG_TYPE_TEXT:
			return "Text";
			break;
		case OFXSVG_TYPE_DOCUMENT:
			return "Document";
			break;
		case OFXSVG_TYPE_ELEMENT:
			return "Element";
			break;
		default:
			break;
	}
	return "Unknown";
}

//--------------------------------------------------------------
std::string ofxSvgElement::sGetSvgXmlName(ofxSvgType atype) {
	switch (atype) {
		case OFXSVG_TYPE_GROUP:
			return "g";
			break;
		case OFXSVG_TYPE_RECTANGLE:
			return "rect";
			break;
		case OFXSVG_TYPE_IMAGE:
			return "image";
			break;
		case OFXSVG_TYPE_ELLIPSE:
			return "ellipse";
			break;
		case OFXSVG_TYPE_CIRCLE:
			return "circle";
			break;
		case OFXSVG_TYPE_PATH:
			return "path";
			break;
		case OFXSVG_TYPE_TEXT:
			return "text";
			break;
		case OFXSVG_TYPE_DOCUMENT:
			return "svg";
			break;
		case OFXSVG_TYPE_ELEMENT:
			return "element";
			break;
		default:
			break;
	}
	return "unknown";
}

//--------------------------------------------------------------
string ofxSvgElement::getTypeAsString() {
	return sGetTypeAsString(getType());
}

//--------------------------------------------------------------
string ofxSvgElement::getCleanName() {
	if( name.empty() ) {
		return name;
	}
	
	string nstr = name;
	
	// Step 2: Replace known patterns from Illustrator
	const std::vector<std::pair<std::regex, std::string>> tregs = {
		{std::regex("_x5F_"), "_"},
		{std::regex("_x28_"), "("},
		{std::regex("_x29_"), ")"},
		{std::regex("_x3B_"), ";"},
		{std::regex("_x2C_"), ","}
	};
	
	for (const auto& [regexPattern, replacement] : tregs) {
		nstr = std::regex_replace(nstr, regexPattern, replacement);
	}
	
	// this regex pattern removes the added numbers added by illustrator
	// ie. lelbow_00000070086365269320197030000010368508730034196876_ becomes lelbow
	// Define a regular expression pattern to match '_<numbers>_'
	std::regex pattern("_\\d+_");
	// Use regex_replace to remove the pattern from the input string
	nstr = std::regex_replace(nstr, pattern, "");
	
	return nstr;
}

//--------------------------------------------------------------
string ofxSvgElement::toString( int nlevel ) {
    
    string tstr = "";
    for( int k = 0; k < nlevel; k++ ) {
        tstr += "   ";
    }
    tstr += getTypeAsString() + " - " + getName() + "\n";
    
    return tstr;
}

//--------------------------------------------------------------
glm::mat4 ofxSvgElement::getTransformMatrix() {
    glm::mat4 rmat = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));
    if( rotation != 0.0f ) {
//        glm::quat rq = glm::angleAxis(ofDegToRad(rotation), glm::vec3(0.f, 0.f, 1.0f ));
//        rmat = rmat * glm::toMat4((const glm::quat&)rq);
		rmat = glm::rotate(rmat, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
    }
    if( scale.x != 1.0f || scale.y != 1.0f ) {
        rmat = glm::scale(rmat, glm::vec3(scale.x, scale.y, 1.0f));
    }
    return rmat;
};

//--------------------------------------------------------------
ofNode ofxSvgElement::getNodeTransform() {
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

//--------------------------------------------------------------
void ofxSvgPath::applyStyle(ofxSvgCssClass& aclass) {
	if( aclass.hasProperty("fill")) {
		if( !aclass.isNone("fill")) {
			path.setFillColor(aclass.getColor("fill"));
		} else {
			path.setFilled(false);
		}
	} else {
		//		aSvgPath->path.setFilled(false);
		path.setFillColor(ofColor(0));
	}
	
	if( aclass.hasProperty("fill-opacity")) {
		if( aclass.isNone("fill-opacity")) {
			path.setFilled(false);
		} else {
			float val = aclass.getFloatValue("fill-opacity", 1.0f);
			if( val <= 0.0001f ) {
				path.setFilled(false);
			} else {
				auto pcolor = path.getFillColor();
				pcolor.a = val;
				path.setFillColor(pcolor);
			}
		}
	}
	
	if( !aclass.isNone("stroke") ) {
		path.setStrokeColor(aclass.getColor("stroke"));
	}
	
	if( aclass.hasProperty("stroke-width")) {
		if( aclass.isNone("stroke-width")) {
			path.setStrokeWidth(0.f);
		} else {
			path.setStrokeWidth( aclass.getFloatValue("stroke-width", 0.f));
		}
	} else {
		// default with no value is 1.f
		//		aSvgPath->path.setStrokeWidth(1.f);
	}
	
	// if the color is not set and the width is not set, then it should be 0
	if( !aclass.isNone("stroke") ) {
		if( !aclass.hasProperty("stroke-width")) {
			path.setStrokeWidth(1.f);
		}
	}
	
	if( aclass.hasProperty("opacity")) {
		if( path.isFilled() ) {
			auto fcolor = path.getFillColor();
			fcolor.a *= aclass.getFloatValue("opacity", 1.f);
			path.setFillColor( fcolor );
		}
		if( path.hasOutline() ) {
			auto scolor = path.getStrokeColor();
			scolor.a *= aclass.getFloatValue("opacity", 1.f);
			path.setStrokeColor( scolor );
		}
	}
	
}

//--------------------------------------------------------------
void ofxSvgPath::draw() {
	ofPushMatrix(); {
		ofSetColor( ofColor::orange );
//		ofDrawCircle(0, 0, 15);
		
		ofTranslate(pos.x, pos.y);
		
		ofSetColor( ofColor::green );
//		ofDrawCircle(0, 0, 10);
		
		if( rotation != 0.0 ) ofRotateZDeg( rotation );
		ofScale( scale.x, scale.y );
		if(isVisible()) {
			ofColor fillColor = getFillColor();
			ofColor strokeColor = getStrokeColor();
			if( alpha != 1.f ) {
				if( isFilled() ) {
					path.setFillColor(ofColor(fillColor.r, fillColor.g, fillColor.b, alpha * (float)fillColor.a ));
				}
				if( hasStroke() ) {
					path.setStrokeColor(ofColor(strokeColor.r, strokeColor.g, strokeColor.b, alpha * (float)strokeColor.a ));
				}
			}
			path.draw();
			
			if( alpha != 1.f ) {
				if( isFilled() ) {
					path.setFillColor(fillColor);
				}
				if( hasStroke() ) {
					path.setStrokeColor(strokeColor);
				}
			}
		}
	} ofPopMatrix();
}

#pragma mark - Image
//--------------------------------------------------------------
//ofRectangle ofxSvgImage::getRectangle() {
//	return ofRectangle(pos.x, pos.y, getWidth(), getHeight());
//}

//--------------------------------------------------------------
void ofxSvgImage::draw() {
	if( !bTryLoad ) {
		if( getFilePath().empty() ) {
			img.load( getFilePath() );
			bTryLoad = true;
		}
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
glm::vec2 ofxSvgImage::getAnchorPointForPercent( float ax, float ay ) {
	glm::vec2 ap = glm::vec2( width * ax * scale.x, height * ay * scale.y );
	ap = glm::rotate(ap, glm::radians(rotation));
	return ap;
}


#pragma mark - Text
std::vector<std::string> ofxSvgText::splitBySpanTags(const std::string& input) {
    std::vector<std::string> result;
    std::regex span_regex(R"(<[^>]+>[\s\S]*?</[^>]+>)"); // Match tags with content including newlines
    std::sregex_iterator begin(input.begin(), input.end(), span_regex);
    std::sregex_iterator end;

    size_t last_pos = 0;
    for (auto it = begin; it != end; ++it) {
        std::smatch match = *it;
        size_t match_start = match.position();
        size_t match_end = match_start + match.length();

        // Add text before match
        if (match_start > last_pos) {
            result.push_back(input.substr(last_pos, match_start - last_pos));
        }

        // Add the matched tag
        result.push_back(match.str());

        last_pos = match_end;
    }

    // Add any remaining text after the last match
    if (last_pos < input.length()) {
        result.push_back(input.substr(last_pos));
    }

    return result;
}

//struct SpanData {
//    std::string style;
//    std::string content;
//};

ofxSvgText::SpanData ofxSvgText::extractSpanData(const std::string& spanTag) {
    SpanData data;

    // Extract style
    std::regex style_regex(R"(style\s*=\s*["']([^"']*)["'])");
    std::smatch style_match;
    if (std::regex_search(spanTag, style_match, style_regex)) {
        data.style = style_match[1].str();
    }

    // Extract inner content (even across newlines)
    // std::regex inner_text_regex(R"(<span[^>]*>([\s\S]*?)</span>)");
	std::regex inner_text_regex(R"(<span[^>]*>([\s\S]*?)<\/span>)");
    std::smatch content_match;
    if (std::regex_search(spanTag, content_match, inner_text_regex)) {
        data.content = content_match[1].str();
    }

    return data;
}

bool ofxSvgText::endsWithLineEnding(const std::string& astr) {
	if (astr.size() >= 2 && astr.substr(astr.size() - 2) == "\r\n") {
		// Windows line ending
		return true;
	} else if (!astr.empty() && astr.back() == '\n') {
		// Unix line ending
		return true;
	}
	return false;
}

std::vector<std::string> ofxSvgText::splitWordsAndLineEndings(const std::string& input) {
    std::vector<std::string> result;

    // Match a single \r or \n or \r\n, or a word
    std::regex token_regex(R"(\r\n|[\r\n]|[^\s\r\n]+)");
    std::sregex_iterator begin(input.begin(), input.end(), token_regex);
    std::sregex_iterator end;

    for (auto it = begin; it != end; ++it) {
        result.push_back(it->str());
    }

    return result;
}

// build the text spans from a string and not from xml //
//--------------------------------------------------------------
void ofxSvgText::setText( const std::string& astring, std::string aFontFamily, int aFontSize, float aMaxWidth ) {
	meshes.clear();
	textSpans.clear();

	if( astring.empty() ) {
		ofLogWarning("ofxSvgText::setText") << "string argument is empty. ";
		return;
	}

	auto spanStrings = splitBySpanTags(astring);
	// ofLogNotice("ofxSvgText") << "number of strings: " << spanStrings.size();
	float ex = 0.f;
	float ey = 0.f;
	int spanCounter = 0;
	for( auto& spanString : spanStrings ) {
		std::vector<std::string> twords;// = ofSplitString(spanString, " ", true, true);
		bool bLastCharIsSpace = false;
		
		// ofLogNotice("ofxSvgText") << "spanString: |" <<spanString<<"|";
		ofxSvgCssClass css;
		css.addProperty("font-family", aFontFamily);
		css.addProperty("font-size", aFontSize);
		css.addProperty("color", getFillColor() );
		if (spanString.find("<span") != std::string::npos) {
            SpanData data = extractSpanData(spanString);
//            std::cout << "Found <span> tag.\n";
//            std::cout << "  Style: [" << data.style << "]\n";
//            std::cout << "  Content: [" << data.content << "]\n";
			css.addProperties(data.style);
			if (!data.content.empty() && data.content.back() == ' ') {
				bLastCharIsSpace = true;
				data.content.pop_back();
			}
			// twords = ofSplitString(data.content, " " , false, false );
			twords = splitWordsAndLineEndings(data.content);
        } else {
//            std::cout << "Regular text: [" << spanString << "]\n";
			
			if (!spanString.empty() && spanString.back() == ' ') {
				bLastCharIsSpace = true;
				spanString.pop_back();
			}
			
			// twords = ofSplitString(spanString, " " , false, false );
			twords = splitWordsAndLineEndings(spanString);
        }
		// cspan->applyStyle(css);

		auto cspan = std::make_shared<ofxSvgText::TextSpan>();
		cspan->applyStyle(css);

		ofLogVerbose("ofxSvgText" ) << "going to try and load: " << spanString << " bold: " << cspan->isBold() << " italic: " << cspan->isItalic();
		if(!ofxSvgFontBook::loadFont(fdirectory, cspan->getFontFamily(), cspan->getFontSize(), cspan->isBold(), cspan->isItalic() )) {
			continue;
		}

		auto& font = cspan->getFont();

		// if( spanString=="\n") {
		// 	ex = 0.f;
		// 	ey += font.getLineHeight();
		// 	continue;
		// }

//		if( spanCounter == 0 ) {
//			// shift everything down on the first line so it's within the desired bounds 
//			ey = font.stringHeight("M");
//		}

		cspan->rect.x = ex;
		cspan->rect.y = ey;

		
		std::stringstream ss;
    	std::string tCurrentString = "";

		// ofRectangle addedBounds(ex, ey,1, 1);

		bool bFirstBreak = true;

		bool bWasAStringLineBreak = false;

		// std::cout << "_____________________________________________________" << std::endl;
		// for( std::size_t i = 0; i < twords.size(); i++ ) {
		// 	auto& token = twords[i];
		// 	if (token == "\n") {
		// 		std::cout << "[\\n]" << std::endl;
		// 	} else if (token == "\r\n") {
		// 		std::cout << "[\\r\\n]" << std::endl;
		// 	} else if (token == "\r") {
		// 		std::cout << "[\\r]" << std::endl;
		// 	} else {
		// 		std::cout << "[" << token << "]" << std::endl;
		// 	}
		// }
		// std::cout << "_____________________________________________________" << std::endl;

		for( std::size_t i = 0; i < twords.size(); i++ ) {

			bool bFinalWord = (i == twords.size()-1);
			
			bool bAddedBreak = false;
			tCurrentString += twords[i];//+ " ";
			if( i < twords.size()-1 ) { //|| (bFinalWord && bLastCharIsSpace )) {
				tCurrentString += " ";
			} else {
				if(bFinalWord && bLastCharIsSpace) {
					tCurrentString += "f";
				}
			}

			bool bIsAStringLineBreak = false;
			if( twords[i] == "\r\n" || twords[i] == "\n" ) {
				bIsAStringLineBreak = true;
			}

			// ofLogNotice("textSpan word string") << "|"<<tCurrentString << "|";;
			// ex = font.stringWidth(tCurrentString);
			ex = font.getStringBoundingBox(tCurrentString,0,0,true).getRight();
			if(ex + cspan->rect.x > aMaxWidth || bIsAStringLineBreak > 0 ) {
			// if( ex > aMaxWidth ) {
				
				tCurrentString = twords[i];
				if( bLastCharIsSpace ) {
					tCurrentString += "f";
				}
				// ofLogNotice("ofxSvgText") << "we have a break, taking ex from string: |" << tCurrentString << "|";
				ex = font.getStringBoundingBox(tCurrentString,0,0,true).getRight();
				// if( bLastCharIsSpace ) {
					// ex += 60;//font.getCharWidth(' ');
				// }

				if(bIsAStringLineBreak) {
					ex = 0.f;
					tCurrentString = "";
				}

				if( bFirstBreak && spanCounter > 0 ) {
					bFirstBreak = false;
					// if( spanCounter > 0 ) {
						cspan->text = ss.str();
						// cspan->rect.height = font.stringHeight(cspan->text+"M");
						//ey += cspan->rect.height;
						ey += font.getLineHeight();

						if( ss.str().size() > 0 ) {
							// ofLogNotice("ofxSvgText") << "LINE Break: adding cspan: " << textSpans.size() << " x: " << cspan->rect.x << " text: |" << cspan->text << "|";
							textSpans.push_back(cspan);
						}

						cspan = std::make_shared<ofxSvgText::TextSpan>();
						cspan->applyStyle(css);
						ss.str("");
						ss.clear();
						// ofLogNotice("Adding a new line I think") << "ss: " <<ss.str();
						// cspan->rect.x = ex;
						cspan->rect.y = ey;
						bAddedBreak = true;
						// }
					// }
				} else {
					//ey += font.stringHeight("M");
					bAddedBreak = true;
					if( spanCounter == 0 && i == 0 ) {

					} else {
						ey += font.getLineHeight();
						ss << std::endl;
					}
				}

				bFirstBreak = false;

				// addedBounds.growToInclude(font.getStringBoundingBox(cspan->text+"M", 0, 0));
				// ey += addedBounds.height;
				// ey += font.getLineHeight();
				
				// if(!bFirstBreak) {
				// 	cspan->rect.x = ex;
				// }
			}

			if( !bAddedBreak && i > 0 && !bWasAStringLineBreak) {
				// if( !bFinalWord ) {
					ss << " ";
				// }
			}
			if( !bIsAStringLineBreak ) {
				ss << twords[i];
			}
			
			bWasAStringLineBreak = bIsAStringLineBreak;
		}

		// ey += font.stringHeight(ss.str());
		// if( !bFirstBreak ) {
			// ey += font.stringHeight(ss.str()+"M");
		// }
		
		ex += cspan->rect.x;

		if( ss.str().size() > 0 ) {
			if( endsWithLineEnding(ss.str())) {
				ex = 0.f;
			}
			// if(!bLastCharIsSpace && bFirstBreak) {
				// ss << " ";
				//ex += 30.f;//font.getCharWidth(' ') * 4.0f;//font.getStringBoundingBox(" ",0.f, 0.f).getRight();
				// ex += font.getCharWidth('f');
			// }
			cspan->text = ss.str();
			// ofLogNotice("ofxSvgText") << "adding cspan: " << textSpans.size() << " x: " << cspan->rect.x << " text: |" << cspan->text << "|";
			cspan->rect.height = font.getStringBoundingBox(cspan->text, 0.f, 0.f ).getHeight();//font.stringHeight(cspan->text);
			if( bFirstBreak ) {
				cspan->rect.height = font.getLineHeight();
				// ey += font.getLineHeight();
			} else {
				// cspan->rect.height += font.getLineHeight();
			}
			// ey = cspan->rect.y + cspan->rect.height;
			//ey += cspan->rect.height;
			textSpans.push_back(cspan);
		}

		spanCounter++;
	}

	// for( std::size_t i = 0; i < textSpans.size(); i++ ) {
	// 	ofLogNotice("TextSpan") << i << " - " << "|"<<textSpans[i]->text<<"|";
	// }

	create();
}

//--------------------------------------------------------------
void ofxSvgText::create() {
    meshes.clear();
    
    // now lets sort the text based on meshes that we need to create //
    vector< std::shared_ptr<TextSpan> > tspans = textSpans;

	for( auto& tspan : textSpans ) {
//		auto tkey = ofxSvgFontBook::getFontKey(tspan->getFontFamily(), tspan->isBold(), tspan->isItalic() );
		if( !ofxSvgFontBook::hasFont(tspan->getFontFamily(), tspan->getFontSize(), tspan->isBold(), tspan->isItalic() )) {
			ofLogVerbose("ofxSvgText") << "Trying to load font " << tspan->getFontFamily() << " bold: " << tspan->isBold() << " italic: " << tspan->isItalic() << " | " << ofGetFrameNum();
			// loadFont( const std::string& aDirectory, const std::string& aFontFamily, int aFontSize, bool aBBold, bool aBItalic )
			ofxSvgFontBook::loadFont(fdirectory, tspan->getFontFamily(), tspan->getFontSize(), tspan->isBold(), tspan->isItalic() );
		}
	}
    
	std::map< string, std::map< int, vector<std::shared_ptr<TextSpan> > > > tspanFonts;
    for( std::size_t i = 0; i < tspans.size(); i++ ) {
        if( tspanFonts.count( tspans[i]->getFontKey() ) == 0 ) {
			std::map< int, vector< std::shared_ptr<TextSpan>> > tmapap;
            tspanFonts[ tspans[i]->getFontKey() ] = tmapap;
        }
		std::map< int, vector< std::shared_ptr<TextSpan>> >& spanMap = tspanFonts[ tspans[i]->getFontKey() ];
        if( spanMap.count(tspans[i]->getFontSize()) == 0 ) {
            vector< std::shared_ptr<TextSpan> > tvec;
            spanMap[ tspans[i]->getFontSize() ] = tvec;
        }
        spanMap[ tspans[i]->getFontSize() ].push_back( tspans[i] );
    }
    
	std::map< string, std::map< int, vector< std::shared_ptr<TextSpan>> > >::iterator mainIt;
    for( mainIt = tspanFonts.begin(); mainIt != tspanFonts.end(); ++mainIt ) {
        
        // now create a mesh for the family //
        // map< string, map<int, ofMesh> > meshes;
        if( meshes.count(mainIt->first) == 0 ) {
			std::map< int, ofMesh > tempMeshMap;
            meshes[ mainIt->first ] = tempMeshMap;
        }
        
        // Font& tfont = fonts[ mainIt->first ];
		std::map< int, ofMesh >& meshMap = meshes[ mainIt->first ];
        
		std::map< int, vector<std::shared_ptr<TextSpan>> >::iterator vIt;
        for( vIt = mainIt->second.begin(); vIt != mainIt->second.end(); ++vIt ) {
            vector<std::shared_ptr<TextSpan>>& spanSpans = vIt->second;

			if( !ofxSvgFontBook::hasFontForKey(mainIt->first, vIt->first )) {
				ofLogError("ofxSvgText") << __FUNCTION__ << " : Could not find that font size in the map: " << vIt->first;
				continue;
			}

			if( meshMap.count(vIt->first) == 0 ) {
                meshMap[ vIt->first ] = ofMesh();
            }
            ofMesh& tmesh = meshMap[ vIt->first ];
            
            // ofTrueTypeFont& ttfont = tfont.sizes[ vIt->first ];
			auto& ttfont = ofxSvgFontBook::getFontForKey(mainIt->first, vIt->first);
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
//            tempSpan->lineHeight     = tfont.getStringBoundingBox("M", 0, 0).height;
			tempSpan->lineHeight 	= tfont.getLineHeight();
//            tempSpan.rect.x         = tempSpan.rect.x - ogPos.x;
//            tempSpan.rect.y         = tempSpan.rect.x - ogPos.x;
            //tempSpan.rect.y         -= tempSpan.lineHeight;
        }
    }
}

//--------------------------------------------------------------
void ofxSvgText::draw() {
    if( !isVisible() ) return;
//    map< string, map<int, ofMesh> > meshes;
    if(bUseShapeColor) {
        ofSetColor( 255, 255, 255, 255.f * alpha );
    }
	std::map< string, std::map<int, ofMesh> >::iterator mainIt;
    
    ofPushMatrix(); {
        
        ofTranslate( pos.x, pos.y );
        
//        ofSetColor( 255, 255, 255, 255.f * alpha );
		if( rotation != 0 ) {ofRotateZDeg( rotation );}
        ofTexture* tex = NULL;
        for( mainIt = meshes.begin(); mainIt != meshes.end(); ++mainIt ) {
            string fontKey = mainIt->first;
			std::map< int, ofMesh >::iterator mIt;
            for( mIt = meshes[ fontKey ].begin(); mIt != meshes[ fontKey ].end(); ++mIt ) {
                int fontSize = mIt->first;
                // let's check to make sure that the texture is there, so that we can bind it //
                bool bHasTexture = false;
                // static map< string, Font > fonts;
                // if( fonts.count( fontKey ) ) {
				if( ofxSvgFontBook::hasBookFont(fontKey)) {
					auto& fbook = ofxSvgFontBook::getBookFont(fontKey);
                    if( fbook.textures.count( fontSize ) ) {
                        bHasTexture = true;
                        tex = &fbook.textures[ fontSize ];
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
void ofxSvgText::draw(const std::string &astring, bool abCentered ) {
	if( textSpans.size() > 0 ) {
		ofPushMatrix(); {
			ofTranslate( pos.x, pos.y );
			if( rotation > 0 ) ofRotateZDeg( rotation );
			textSpans[0]->draw(astring, abCentered );
		} ofPopMatrix();
	} else {
		ofLogVerbose("ofxSvgText") << __FUNCTION__ << " : no text spans to draw with.";
	}
}

//--------------------------------------------------------------
void ofxSvgText::draw(const std::string& astring, const ofColor& acolor, bool abCentered ) {
	if( textSpans.size() > 0 ) {
		ofPushMatrix(); {
			ofTranslate( pos.x, pos.y );
			if( rotation > 0 ) ofRotateZDeg( rotation );
			textSpans[0]->draw(astring, acolor, abCentered );
		} ofPopMatrix();
	} else {
		ofLogVerbose("ofxSvgText") << __FUNCTION__ << " : no text spans to draw with.";
	}
}

//--------------------------------------------------------------
void ofxSvgText::TextSpan::applyStyle(ofxSvgCssClass& aclass) {
	mSvgCssClass = aclass;
	
//	fontFamily   	= aclass.getValue("font-family", "Arial");
//	fontFamily = mSvgCssClass.getFontFamily("Arial");
//	fontSize      	= aclass.getIntValue("font-size", 18 );
//	fontSize = mSvgCssClass.getFontSize(18);
	color 			= ofColor(0);
	if( aclass.hasProperty("color")) {
		color = aclass.getColor("color");
	} else if( aclass.hasProperty("fill")) {
		color = aclass.getColor("fill");
	}
	
	if( aclass.hasProperty("opacity")) {
		color.a *= aclass.getFloatValue("opacity", 1.f);
	}
	

//	bold = false;
//	if( ofIsStringInString(aclass.getValue("font-weight", "" ), "bold")) {
//		bold = true;
//	}
	
//	bold = mSvgCssClass.isFontBold();
	
//	italic = false;
//	if( ofIsStringInString(aclass.getValue("font-style", "" ), "italic")) {
//		italic = true;
//	}
	
//	italic = mSvgCssClass.isFontItalic();
	
	
	ofLogVerbose("ofxSvgText::TextSpan::applyStyle") << "text: " << text;
	ofLogVerbose("ofxSvgText::TextSpan::applyStyle") << " css class: " << aclass.toString() << std::endl << " color: " << color;
	
}

// must return a reference for some reason here //
//--------------------------------------------------------------
ofTrueTypeFont& ofxSvgText::TextSpan::getFont() {
	return ofxSvgFontBook::getFontForKey( getFontKey(), getFontSize() );
}

//--------------------------------------------------------------
void ofxSvgText::TextSpan::draw(const std::string &astring, bool abCentered ) {
	draw( astring, color, abCentered );
}

//--------------------------------------------------------------
void ofxSvgText::TextSpan::draw(const std::string &astring, const ofColor& acolor, bool abCentered ) {
	ofSetColor( acolor );
	auto& cfont = getFont();
	ofRectangle tempBounds = cfont.getStringBoundingBox( astring, 0, 0 );
	float tffontx = abCentered ? rect.getCenter().x - tempBounds.width/2 : rect.x;
	//	const ofMesh& stringMesh  = cfont.getStringMesh( astring, tffontx, rect.y );
	//	cfont.drawString(astring, tffontx, rect.y + (tempBounds.getHeight() - lineHeight) );
	cfont.drawString(astring, tffontx, rect.y );
}

//--------------------------------------------------------------
ofTrueTypeFont& ofxSvgText::getFont() {
	if( textSpans.size() > 0 ) {
		return textSpans[0]->getFont();
	}
	ofLogWarning("ofxSvgText") << __FUNCTION__ << " : no font detected from text spans, returning default font.";
	return ofxSvgFontBook::defaultFont;
}

//--------------------------------------------------------------
ofColor ofxSvgText::getColor() {
	if( textSpans.size() > 0 ) {
		return textSpans[0]->color;
	}
	ofLogWarning("ofxSvgText") << __FUNCTION__ << " : no font detected from text spans, returning path fill color.";
	return path.getFillColor();
}

// get the bounding rect for all of the text spans in this svg'ness
// should be called after create //
//--------------------------------------------------------------
ofRectangle ofxSvgText::getBoundingBox() {
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










