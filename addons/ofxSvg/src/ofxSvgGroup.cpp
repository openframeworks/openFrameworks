#include "ofxSvgGroup.h"
#include "ofGraphics.h"

using std::vector;
using std::shared_ptr;
using std::string;

//--------------------------------------------------------------
void ofxSvgGroup::draw() const {
	if( !isVisible() ) return;
	
    std::size_t numElements = mChildren.size();
	for( std::size_t i = 0; i < numElements; i++ ) {
		mChildren[i]->draw();
	}
}

//--------------------------------------------------------------
std::size_t ofxSvgGroup::getNumChildren() {
	return mChildren.size();
}

//--------------------------------------------------------------
vector< shared_ptr<ofxSvgElement> >& ofxSvgGroup::getChildren() {
    return mChildren;
}

//--------------------------------------------------------------
vector< shared_ptr<ofxSvgElement> > ofxSvgGroup::getAllElements(bool aBIncludeGroups) {
    vector< shared_ptr<ofxSvgElement> > retElements;
    
    for( auto ele : mChildren ) {
        _getAllElementsRecursive( retElements, ele, aBIncludeGroups );
    }
    
    return retElements;
}

//--------------------------------------------------------------
std::vector< std::shared_ptr<ofxSvgGroup> > ofxSvgGroup::getAllGroups() {
	vector< shared_ptr<ofxSvgGroup> > retGroups;
	for( auto ele : mChildren ) {
		_getAllGroupsRecursive( retGroups, ele );
	}
	return retGroups;
}

// flattens out hierarchy //
//--------------------------------------------------------------
void ofxSvgGroup::_getAllElementsRecursive( vector< shared_ptr<ofxSvgElement> >& aElesToReturn, shared_ptr<ofxSvgElement> aele, bool aBIncludeGroups ) {
    if( aele ) {
        if( aele->isGroup() ) {
            auto tgroup = std::dynamic_pointer_cast<ofxSvgGroup>(aele);
			if(aBIncludeGroups) {aElesToReturn.push_back(tgroup);}
            for( auto ele : tgroup->getChildren() ) {
                _getAllElementsRecursive( aElesToReturn, ele, aBIncludeGroups );
            }
        } else {
            aElesToReturn.push_back( aele );
        }
    }
}

//--------------------------------------------------------------
void ofxSvgGroup::_getAllGroupsRecursive( std::vector< std::shared_ptr<ofxSvgGroup> >& aGroupsToReturn, std::shared_ptr<ofxSvgElement> aele ) {
	if( aele ) {
		if( aele->isGroup() ) {
			auto tgroup = std::dynamic_pointer_cast<ofxSvgGroup>(aele);
			aGroupsToReturn.push_back(tgroup);
			for( auto ele : tgroup->getChildren() ) {
				if( ele->isGroup() ) {
					_getAllGroupsRecursive( aGroupsToReturn, ele );
				}
			}
		}
	}
}

//--------------------------------------------------------------
std::vector< std::shared_ptr<ofxSvgPath> > ofxSvgGroup::getAllElementsWithPath() {
	auto allKids = getAllElements(false);
	std::vector< std::shared_ptr<ofxSvgPath> > rpaths;
	for( auto kid : allKids ) {
		if( kid->getType() == OFXSVG_TYPE_RECTANGLE ) {
			rpaths.push_back(std::dynamic_pointer_cast<ofxSvgRectangle>(kid));
		} else if( kid->getType() == OFXSVG_TYPE_PATH ) {
			rpaths.push_back(std::dynamic_pointer_cast<ofxSvgPath>(kid));
		} else if( kid->getType() == OFXSVG_TYPE_CIRCLE ) {
			rpaths.push_back(std::dynamic_pointer_cast<ofxSvgCircle>(kid));
		} else if( kid->getType() == OFXSVG_TYPE_ELLIPSE ) {
			rpaths.push_back(std::dynamic_pointer_cast<ofxSvgEllipse>(kid));
		}
	}
	
	return rpaths;
}

//--------------------------------------------------------------
shared_ptr<ofxSvgElement> ofxSvgGroup::getElement( std::string aPath, bool bStrict ) {
    
    vector< std::string > tsearches;
    if( ofIsStringInString( aPath, ":" ) ) {
        tsearches = ofSplitString( aPath, ":" );
    } else {
        tsearches.push_back( aPath );
    }
    
    shared_ptr<ofxSvgElement> temp;
	_getElementForNameRecursive( tsearches, temp, mChildren, bStrict );
    return temp;
}

//--------------------------------------------------------------
std::vector< std::shared_ptr<ofxSvgElement> > ofxSvgGroup::getAllElementsForName( const std::string& aname, bool bStrict ) {
	std::vector< std::shared_ptr<ofxSvgElement> > relements;
	auto allElements = getAllElements(true);
	for( auto& ele : allElements ) {
		if( bStrict ) {
			if( ele->getCleanName() == aname ) {
				relements.push_back(ele);
			}
		} else {
			if( ofIsStringInString( ele->getCleanName(), aname )) {
				relements.push_back(ele);
			}
		}
	}
	return relements;
}

//--------------------------------------------------------------
std::vector< std::shared_ptr<ofxSvgElement> > ofxSvgGroup::getChildrenForName( const std::string& aname, bool bStrict ) {
	std::vector< std::shared_ptr<ofxSvgElement> > relements;
	for( auto& kid : mChildren ) {
		if( bStrict ) {
			if( kid->getCleanName() == aname ) {
				relements.push_back(kid);
			}
		} else {
			if( ofIsStringInString( kid->getCleanName(), aname )) {
				relements.push_back(kid);
			}
		}
	}
	return relements;
}

//--------------------------------------------------------------
void ofxSvgGroup::_getElementForNameRecursive( vector<string>& aNamesToFind, shared_ptr<ofxSvgElement>& aTarget, vector< shared_ptr<ofxSvgElement> >& aElements, bool bStrict ) {
	
	if( aNamesToFind.size() < 1 ) {
		return;
	}
	if(aTarget) {
		return;
	}
	
	bool bKeepGoing = false;
	std::string nameToFind = aNamesToFind[0];
	if( aNamesToFind.size() > 1 ) {
		if( aNamesToFind[0] == "*" ) {
			bKeepGoing = true;
			nameToFind = aNamesToFind[1];
		}
	}
	for( std::size_t i = 0; i < aElements.size(); i++ ) {
		bool bFound = false;
		if(bStrict) {
			if( aElements[i]->getName() == nameToFind ) {
				bFound = true;
			}
		} else {
//			std::cout << "Group::_getElementForNameRecursive: ele name: " << aElements[i]->getName() << " nameToFind: " << nameToFind << " keep going: " << bKeepGoing << std::endl;
			if( ofIsStringInString( aElements[i]->getName(), nameToFind )) {
				bFound = true;
			}
			
			if (!bFound && aElements[i]->getType() == OFXSVG_TYPE_TEXT) {
				
				if (aElements[i]->getName() == "No Name") {
					// the ids for text block in illustrator are weird,
					// so try to grab the name from the text contents //
					auto etext = std::dynamic_pointer_cast<ofxSvgText>(aElements[i]);
					if (etext) {
						if (etext->textSpans.size()) {
							if(ofIsStringInString( etext->textSpans.front()->getText(), aNamesToFind[0] )) {
								bFound = true;
							}
						}
					}
				}
			}
		}
		
		if( bFound && !bKeepGoing ) {
			if( !bKeepGoing && aNamesToFind.size() > 0 ) {
				aNamesToFind.erase( aNamesToFind.begin() );
			}
			if(aNamesToFind.size() == 0 ) {
				aTarget = aElements[i];
				break;
			} else {
				if( aElements[i]->getType() == OFXSVG_TYPE_GROUP ) {
					auto tgroup = std::dynamic_pointer_cast<ofxSvgGroup>( aElements[i] );
					_getElementForNameRecursive( aNamesToFind, aTarget, tgroup->getChildren(), bStrict );
					break;
				}
			}
		}
		
		if( bKeepGoing ) {
			if( bFound ) {
//				std::cout << "Group::_getElementForNameRecursive: SETTING TARGET: " << aElements[i]->getName() << " keep going: " << bKeepGoing << std::endl;
				aTarget = aElements[i];
				break;
			} else {
				if( aElements[i]->getType() == OFXSVG_TYPE_GROUP ) {
//					std::cout << "Group::_getElementForNameRecursive: FOUND A GROUP, But still going: " << aElements[i]->getName() << " keep going: " << bKeepGoing << std::endl;
					auto tgroup = std::dynamic_pointer_cast<ofxSvgGroup>( aElements[i] );
					_getElementForNameRecursive( aNamesToFind, aTarget, tgroup->getChildren(), bStrict );
				}
			}
		}
	}
}

//--------------------------------------------------------------
bool ofxSvgGroup::replace( shared_ptr<ofxSvgElement> aOriginal, shared_ptr<ofxSvgElement> aNew ) {
    bool bReplaced = false;
    _replaceElementRecursive( aOriginal, aNew, mChildren, bReplaced );
    return bReplaced;
}

//--------------------------------------------------------------
void ofxSvgGroup::_replaceElementRecursive( shared_ptr<ofxSvgElement> aTarget, shared_ptr<ofxSvgElement> aNew, vector< shared_ptr<ofxSvgElement> >& aElements, bool& aBSuccessful ) {
    for( std::size_t i = 0; i < aElements.size(); i++ ) {
        bool bFound = false;
        if( aTarget == aElements[i] ) {
            bFound = true;
            aBSuccessful = true;
            aElements[i] = aNew;
			aNew->layer = aTarget->layer;
			if( aTarget->getParent() ) {
				aNew->setParent(*aTarget->getParent(), true);
				aTarget->clearParent();
			}
            break;
        }
        if( !bFound ) {
            if( aElements[i]->getType() == OFXSVG_TYPE_GROUP ) {
                auto tgroup = std::dynamic_pointer_cast<ofxSvgGroup>( aElements[i] );
                _replaceElementRecursive(aTarget, aNew, tgroup->mChildren, aBSuccessful );
            }
        }
    }
}

//--------------------------------------------------------------
bool ofxSvgGroup::remove( std::shared_ptr<ofxSvgElement> aelement ) {
	if( !aelement ) {
		ofLogWarning("ofxSvgGroup::remove") << "element is invalid.";
		return false;
	}
	bool bRemoved = false;
	_removeElementRecursive( aelement, mChildren, bRemoved );
	return bRemoved;
}

////--------------------------------------------------------------
//bool ofxSvgGroup::remove( std::vector<std::shared_ptr<ofxSvgElement> > aelements ) {
//	if( aelements.size() < 1 ) {
//		return false;
//	}
//	
//	bool bAllRemoved = true;
//	for( auto& aele : aelements ) {
//		if( !aele ) {
//			ofLogWarning("ofxSvgGroup::remove") << "element is invalid.";
//			bAllRemoved = false;
//			continue;
//		}
//		bool bEleRemoved = remove( aele );
//		if( !bEleRemoved ) {
//			bAllRemoved = false;
//		}
//	}
//	return bAllRemoved;
//}

//--------------------------------------------------------------
void ofxSvgGroup::_removeElementRecursive( shared_ptr<ofxSvgElement> aTarget, vector< shared_ptr<ofxSvgElement> >& aElements, bool& aBSuccessful ) {
	
	for( std::size_t i = 0; i < aElements.size(); i++ ) {
		bool bFound = false;
		if( aTarget == aElements[i] ) {
			bFound = true;
			aBSuccessful = true;
			aElements.erase(aElements.begin() + i);
			// make sure to break here since the size() will change
			break;
		}
		if( !bFound ) {
			if( aElements[i]->getType() == OFXSVG_TYPE_GROUP ) {
				auto tgroup = std::dynamic_pointer_cast<ofxSvgGroup>( aElements[i] );
				_removeElementRecursive(aTarget, tgroup->mChildren, aBSuccessful );
			}
		}
	}
}

//--------------------------------------------------------------
string ofxSvgGroup::toString(int nlevel) {
    
    string tstr = "";
    for( int k = 0; k < nlevel; k++ ) {
        tstr += "   ";
    }
    tstr += getTypeAsString() + " - " + getName() + "\n";
    
    if( mChildren.size() ) {
        for( std::size_t i = 0; i < mChildren.size(); i++ ) {
            tstr += mChildren[i]->toString( nlevel+1);
        }
    }
    
    return tstr;
}


//--------------------------------------------------------------
void ofxSvgGroup::disableColors() {
    auto telements = getAllElements(false);
    for( auto& ele : telements ) {
        ele->setUseColors(false);
    }
}

//--------------------------------------------------------------
void ofxSvgGroup::enableColors() {
    auto telements = getAllElements(false);
    for( auto& ele : telements ) {
        ele->setUseColors(true);
    }
}








