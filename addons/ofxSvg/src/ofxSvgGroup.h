//
//  ofxSvgGroup.h
//
//  Created by Nick Hardeman on 7/31/15.
//

#pragma once
#include "ofxSvgElements.h"

class ofxSvgGroup : public ofxSvgElement {
public:
	
	virtual ofxSvgType getType() override {return TYPE_GROUP;}
	
	virtual void draw() override;
	
	std::size_t getNumChildren();// override;
	std::vector< std::shared_ptr<ofxSvgElement> >& getChildren();
	std::vector< std::shared_ptr<ofxSvgElement> > getAllChildren(bool aBIncludeGroups);
	std::vector< std::shared_ptr<ofxSvgGroup> > getAllChildGroups();
	
	template<typename ofxSvg_T>
	std::vector< std::shared_ptr<ofxSvg_T> > getElementsForType( std::string aPathToGroup="", bool bStrict= false ) {
		auto temp = std::make_shared<ofxSvg_T>();
		auto sType = temp->getType();
		
		std::vector< std::shared_ptr<ofxSvg_T> > telements;
		std::vector< std::shared_ptr<ofxSvgElement> > elementsToSearch;
		if( aPathToGroup == "" ) {
			elementsToSearch = mChildren;
		} else {
			std::shared_ptr< ofxSvgElement > temp = getElementForName( aPathToGroup, bStrict );
			if( temp ) {
				if( temp->isGroup() ) {
					std::shared_ptr< ofxSvgGroup > tgroup = std::dynamic_pointer_cast<ofxSvgGroup>( temp );
					elementsToSearch = tgroup->mChildren;
				}
			}
		}
		
		if( !elementsToSearch.size() && mChildren.size() ) {
			ofLogNotice("ofx::svg::Group") << __FUNCTION__ << " did not find group with name: " << aPathToGroup;
			elementsToSearch = mChildren;
		}
		
		for( std::size_t i = 0; i < elementsToSearch.size(); i++ ) {
			if( elementsToSearch[i]->getType() == sType ) {
				telements.push_back( std::dynamic_pointer_cast<ofxSvg_T>(elementsToSearch[i]) );
			}
		}
		return telements;
	}
	
	template<typename ofxSvg_T>
	std::shared_ptr<ofxSvg_T> getFirstElementForType( std::string aPathToGroup="", bool bStrict= false ) {
		auto eles = getElementsForType<ofxSvg_T>(aPathToGroup, bStrict );
		if( eles.size() > 0 ) {
			return eles[0];
		}
		return std::shared_ptr<ofxSvg_T>();
	}
	
	template<typename ofxSvg_T>
	std::vector< std::shared_ptr<ofxSvg_T> > getAllElementsForType() {
		
		auto temp = std::make_shared<ofxSvg_T>();
		auto sType = temp->getType();
		
		std::vector< std::shared_ptr<ofxSvg_T> > telements;
		auto elementsToSearch = getAllChildren(true);
		
		for( std::size_t i = 0; i < elementsToSearch.size(); i++ ) {
			if( elementsToSearch[i]->getType() == sType ) {
				telements.push_back( std::dynamic_pointer_cast<ofxSvg_T>(elementsToSearch[i]) );
			}
		}
		return telements;
	}
	
	template<typename ofxSvg_T>
	std::vector< std::shared_ptr<ofxSvg_T> > getAllElementsContainingNameForType(std::string aname) {
		
		auto temp = std::make_shared<ofxSvg_T>();
		auto sType = temp->getType();
		
		std::vector< std::shared_ptr<ofxSvg_T> > telements;
		// get all children does not include groups, since it's meant to flatten hierarchy
		
		if( sType == ofxSvgType::TYPE_GROUP ) {
			auto groupsToSearch = getAllChildGroups();
			
			for( std::size_t i = 0; i < groupsToSearch.size(); i++ ) {
				if( groupsToSearch[i]->getType() == sType ) {
					if( ofIsStringInString(groupsToSearch[i]->getName(), aname) ) {
						telements.push_back( std::dynamic_pointer_cast<ofxSvg_T>(groupsToSearch[i]) );
					}
				}
			}
			
		} else {
			auto elementsToSearch = getAllChildren(false);
			for( std::size_t i = 0; i < elementsToSearch.size(); i++ ) {
				if( elementsToSearch[i]->getType() == sType ) {
					if( ofIsStringInString(elementsToSearch[i]->getName(), aname) ) {
						telements.push_back( std::dynamic_pointer_cast<ofxSvg_T>(elementsToSearch[i]) );
					}
				}
			}
		}
		
		
		return telements;
	}
	
	std::vector< std::shared_ptr<ofxSvgPath> > getAllElementsWithPath();
	
	std::shared_ptr<ofxSvgElement> getElementForName( std::string aPath, bool bStrict = false );
	std::vector< std::shared_ptr<ofxSvgElement> > getChildrenForName( const std::string& aname, bool bStrict = false );
	
	template<typename ofxSvg_T>
	std::vector< std::shared_ptr<ofxSvg_T> > getChildrenForTypeForName( const std::string& aname, bool bStrict = false ) {
		auto temp = std::make_shared<ofxSvg_T>();
		auto sType = temp->getType();
		
		std::vector< std::shared_ptr<ofxSvg_T> > relements;
		for( auto& kid : mChildren ) {
			if( kid->getType() != sType ) {continue;}
			if( bStrict ) {
				if( kid->getName() == aname ) {
					relements.push_back( std::dynamic_pointer_cast<ofxSvg_T>(kid));
				}
			} else {
				if( ofIsStringInString( kid->getName(), aname )) {
					relements.push_back(std::dynamic_pointer_cast<ofxSvg_T>(kid));
				}
			}
		}
		return relements;
	}
	
	template<typename ofxSvg_T>
	std::shared_ptr< ofxSvg_T > get( std::string aPath, bool bStrict = false ) {
		auto stemp = std::dynamic_pointer_cast<ofxSvg_T>( getElementForName( aPath, bStrict ) );
		return stemp;
	}
	
	template<typename ofxSvg_T>
	std::shared_ptr< ofxSvg_T > get( int aIndex ) {
		auto stemp = std::dynamic_pointer_cast<ofxSvg_T>( mChildren[ aIndex ] );
		return stemp;
	}
	
	bool replace( std::shared_ptr<ofxSvgElement> aOriginal, std::shared_ptr<ofxSvgElement> aNew );
	
	// adding 
	template<typename ofxSvg_T>
	std::shared_ptr<ofxSvg_T> add(std::string aname) {
		auto element = std::make_shared<ofxSvg_T>();
		element->name = aname;
		mChildren.push_back(element);
		return element;
	};
	
	void add( std::shared_ptr<ofxSvgElement> aele ) { mChildren.push_back(aele); }
	
	virtual std::string toString(int nlevel = 0) override;
	
	void disableColors();
	void enableColors();
	
protected:
	void _getElementForNameRecursive( std::vector< std::string >& aNamesToFind, std::shared_ptr<ofxSvgElement>& aTarget, std::vector< std::shared_ptr<ofxSvgElement> >& aElements, bool bStrict );
	void _getAllElementsRecursive( std::vector< std::shared_ptr<ofxSvgElement> >& aElesToReturn, std::shared_ptr<ofxSvgElement> aele, bool aBIncludeGroups );
	
	void _getAllGroupsRecursive( std::vector< std::shared_ptr<ofxSvgGroup> >& aGroupsToReturn, std::shared_ptr<ofxSvgElement> aele );
	
	void _replaceElementRecursive( std::shared_ptr<ofxSvgElement> aTarget, std::shared_ptr<ofxSvgElement> aNew, std::vector< std::shared_ptr<ofxSvgElement> >& aElements, bool& aBSuccessful );
	
	std::vector< std::shared_ptr<ofxSvgElement> > mChildren;
};
















