//
//  ofxSvgGroup.cpp
//
//  Created by Nick Hardeman on 7/31/15.
//

#include "ofxSvgGroup.h"
#include "ofGraphics.h"

using namespace ofx::svg;
using std::vector;
using std::shared_ptr;
using std::string;

//--------------------------------------------------------------
void Group::draw() {
    std::size_t numElements = mChildren.size();
    bool bTrans = pos.x != 0 || pos.y != 0.0;
    if( bTrans ) {
        ofPushMatrix();
        ofTranslate(pos.x, pos.y);
    }
    for( std::size_t i = 0; i < numElements; i++ ) {
		mChildren[i]->draw();
    }
    if( bTrans ) {
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
std::size_t Group::getNumChildren() {
	return mChildren.size();
}

//--------------------------------------------------------------
vector< shared_ptr<Element> >& Group::getChildren() {
    return mChildren;
}

//--------------------------------------------------------------
vector< shared_ptr<Element> > Group::getAllChildren(bool aBIncludeGroups) {
    vector< shared_ptr<Element> > retElements;
    
    for( auto ele : mChildren ) {
        _getAllElementsRecursive( retElements, ele, aBIncludeGroups );
    }
    
    return retElements;
}

//--------------------------------------------------------------
std::vector< std::shared_ptr<ofx::svg::Group> > Group::getAllChildGroups() {
	vector< shared_ptr<ofx::svg::Group> > retGroups;
	for( auto ele : mChildren ) {
		_getAllGroupsRecursive( retGroups, ele );
	}
	return retGroups;
}

// flattens out hierarchy //
//--------------------------------------------------------------
void Group::_getAllElementsRecursive( vector< shared_ptr<Element> >& aElesToReturn, shared_ptr<Element> aele, bool aBIncludeGroups ) {
    if( aele ) {
        if( aele->isGroup() ) {
            shared_ptr<Group> tgroup = std::dynamic_pointer_cast<Group>(aele);
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
void Group::_getAllGroupsRecursive( std::vector< std::shared_ptr<ofx::svg::Group> >& aGroupsToReturn, std::shared_ptr<Element> aele ) {
	if( aele ) {
		if( aele->isGroup() ) {
			shared_ptr<Group> tgroup = std::dynamic_pointer_cast<Group>(aele);
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
std::vector< std::shared_ptr<ofx::svg::Path> > Group::getAllElementsWithPath() {
	auto allKids = getAllChildren(false);
	std::vector< std::shared_ptr<Path> > rpaths;
	for( auto kid : allKids ) {
		if( kid->getType() == TYPE_RECTANGLE ) {
			rpaths.push_back(std::dynamic_pointer_cast<ofx::svg::Rectangle>(kid));
		} else if( kid->getType() == TYPE_PATH ) {
			rpaths.push_back(std::dynamic_pointer_cast<ofx::svg::Path>(kid));
		} else if( kid->getType() == TYPE_CIRCLE ) {
			rpaths.push_back(std::dynamic_pointer_cast<ofx::svg::Circle>(kid));
		} else if( kid->getType() == TYPE_ELLIPSE ) {
			rpaths.push_back(std::dynamic_pointer_cast<ofx::svg::Ellipse>(kid));
		}
	}
	
	return rpaths;
}

//--------------------------------------------------------------
shared_ptr<Element> Group::getElementForName( std::string aPath, bool bStrict ) {
    
    vector< std::string > tsearches;
    if( ofIsStringInString( aPath, ":" ) ) {
        tsearches = ofSplitString( aPath, ":" );
    } else {
        tsearches.push_back( aPath );
    }
    
    shared_ptr<Element> temp;
	_getElementForNameRecursive( tsearches, temp, mChildren, bStrict );
    return temp;
}

//--------------------------------------------------------------
std::vector< std::shared_ptr<Element> > Group::getChildrenForName( const std::string& aname, bool bStrict ) {
	std::vector< std::shared_ptr<Element> > relements;
	for( auto& kid : mChildren ) {
		if( bStrict ) {
			if( kid->getName() == aname ) {
				relements.push_back(kid);
			}
		} else {
			if( ofIsStringInString( kid->getName(), aname )) {
				relements.push_back(kid);
			}
		}
	}
	return relements;
}

//--------------------------------------------------------------
void Group::_getElementForNameRecursive( vector<string>& aNamesToFind, shared_ptr<Element>& aTarget, vector< shared_ptr<Element> >& aElements, bool bStrict ) {
	
	if( aNamesToFind.size() < 1 ) {
		return;
	}
	if(aTarget) {
		return;
	}
	
	bool bKeepGoing = false;
	std::string nameToFind = aNamesToFind[0];
	if( aNamesToFind.size() > 1 ) {
		bKeepGoing = (aNamesToFind[0] == "*");
		nameToFind = aNamesToFind[1];
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
			
			if (!bFound && aElements[i]->getType() == TYPE_TEXT) {
				
				if (aElements[i]->getName() == "No Name") {
					// the ids for text block in illustrator are weird,
					// so try to grab the name from the text contents //
					auto etext = std::dynamic_pointer_cast<Text>(aElements[i]);
					if (etext) {
						if (etext->textSpans.size()) {
//                            cout << "Searching for " << aNamesToFind[0] << " in " << etext->textSpans.front().text << endl;
							if(ofIsStringInString( etext->textSpans.front()->text, aNamesToFind[0] )) {
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
				if( aElements[i]->getType() == TYPE_GROUP ) {
					auto tgroup = std::dynamic_pointer_cast<Group>( aElements[i] );
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
				if( aElements[i]->getType() == TYPE_GROUP ) {
//					std::cout << "Group::_getElementForNameRecursive: FOUND A GROUP, But still going: " << aElements[i]->getName() << " keep going: " << bKeepGoing << std::endl;
					auto tgroup = std::dynamic_pointer_cast<Group>( aElements[i] );
					_getElementForNameRecursive( aNamesToFind, aTarget, tgroup->getChildren(), bStrict );
				}
			}
		}
	}
}

//--------------------------------------------------------------
bool Group::replace( shared_ptr<Element> aOriginal, shared_ptr<Element> aNew ) {
    bool bReplaced = false;
    _replaceElementRecursive( aOriginal, aNew, mChildren, bReplaced );
    return bReplaced;
}

//--------------------------------------------------------------
void Group::_replaceElementRecursive( shared_ptr<Element> aTarget, shared_ptr<Element> aNew, vector< shared_ptr<Element> >& aElements, bool& aBSuccessful ) {
    for( std::size_t i = 0; i < aElements.size(); i++ ) {
        bool bFound = false;
        if( aTarget == aElements[i] ) {
            bFound = true;
            aBSuccessful = true;
            aElements[i] = aNew;
			aNew->layer = aTarget->layer;
            break;
        }
        if( !bFound ) {
            if( aElements[i]->getType() == TYPE_GROUP ) {
                auto tgroup = std::dynamic_pointer_cast<Group>( aElements[i] );
                _replaceElementRecursive(aTarget, aNew, tgroup->mChildren, aBSuccessful );
            }
        }
    }
}

//--------------------------------------------------------------
string Group::toString(int nlevel) {
    
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
void Group::disableColors() {
    auto telements = getAllChildren(false);
    for( auto& ele : telements ) {
        ele->setUseShapeColor(false);
    }
}

//--------------------------------------------------------------
void Group::enableColors() {
    auto telements = getAllChildren(false);
    for( auto& ele : telements ) {
        ele->setUseShapeColor(true);
    }
}








