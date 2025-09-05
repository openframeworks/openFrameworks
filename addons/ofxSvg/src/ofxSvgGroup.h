#pragma once
#include "ofxSvgElements.h"

class ofxSvgGroup : public ofxSvgElement {
public:
	
	virtual ofxSvgType getType() override {return OFXSVG_TYPE_GROUP;}
	
	ofxSvgGroup() = default;
	
	/// \brief Deep-copy constructor. Clone the children so that the new group does not have references to the children in this class.
	ofxSvgGroup(const ofxSvgGroup& other) {
		ofLogVerbose("ofxSvgGroup") << "ofxSvgGroup(const ofxSvgGroup& other)";
		mChildren.reserve(other.mChildren.size());
		
		setPosition(other.getPosition());
		setOrientation(other.getOrientationQuat());
		setScale(other.getScale());
		
		bVisible = other.bVisible;
		alpha = other.alpha;
		layer = other.layer;
		name = other.name;
		for (const auto& ptr : other.mChildren) {
			// Create a new shared_ptr to a new item copy.
			if( ptr ) {
				auto newKid = ptr->clone();
				newKid->setParent(*this);
				mChildren.push_back(newKid);
			}
		}
	}
	
	/// \brief Deep-copy assignment. Clone / create copies of the children from other.
	ofxSvgGroup& operator=(const ofxSvgGroup& other) {
		if (this != &other) {
			ofLogVerbose("ofxSvgGroup") << "operator=(const ofxSvgGroup& other)";
			
			setPosition(other.getPosition());
			setOrientation(other.getOrientationQuat());
			setScale(other.getScale());
			
			bVisible = other.bVisible;
			alpha = other.alpha;
			layer = other.layer;
			name = other.name;
			mChildren.clear();
			mChildren.reserve(other.mChildren.size());
			for (const auto& ptr : other.mChildren) {
				// Create a new shared_ptr to a new item copy.
				if( ptr ) {
					auto newKid = ptr->clone();
					newKid->setParent(*this);
					mChildren.push_back(newKid);
				}
			}
		}
		return *this;
	}
	
	// we need to override this so that we can draw the children correctly
	// without this transform getting applied to the children
	virtual void draw() const override;
	
	/// \brief Set the visibility of the group. Does not set visibility of each child. The group only draws its children if the group is visible.
	/// \param bool aBVisible set to true for visible.
//	virtual void setVisible( bool aBVisible ) override {
//		ofxSvgElement::setVisible(aBVisible);
//	}
	
	/// \brief Set the alpha of the group and call setAlpha(aAlpha) on its children.
	/// \param float aAlpha in range from 0-1 where 0 is transparent and 1 is full opacity.
	virtual void setAlpha( float aAlpha ) override {
		alpha = aAlpha;
		for( auto& kid : mChildren ) {
			kid->setAlpha( aAlpha );
		}
	}
	
	/// \brief Get the bounding box of all of the elements in this group.
	/// \return ofRectangle encapsulating all of the elements in the group.
	virtual ofRectangle getBoundingBox() override {
		ofRectangle rrect;
		bool bFirstRect = true;
		auto allEs = getAllElements(false);
		for( auto& ele : allEs ) {
			auto erect = ele->getBoundingBox();
			if( erect.width > 0 && erect.height > 0 ) {
				if(bFirstRect) {
					rrect = erect;
				} else {
					rrect.growToInclude(erect);
				}
				bFirstRect = false;
			}
		}
		return rrect;
	};
	
	/// \brief Get the number of immediate children in this group.
	/// \return std::size_t number of chilren.
	std::size_t getNumChildren();
	/// \brief Get immediate children in this group.
	/// \return std::vector< std::shared_ptr<ofxSvgElement> > all of the children in this group as a reference.
	std::vector< std::shared_ptr<ofxSvgElement> >& getChildren();
	/// \brief Get all of the elements under this group recursively creating a flattened vector. If groups are included,
	/// the returned groups will point to shared_ptrs in their children but also in the returned vector.
	/// \param bool aBIncludeGroups true if groups should be included in the returning vector.
	/// \return std::vector< std::shared_ptr<ofxSvgElement> > as a flat vector.
	std::vector< std::shared_ptr<ofxSvgElement> > getAllElements(bool aBIncludeGroups);
	/// \brief Get all of the groups under this group recursively creating a flattened vector.
	/// \return std::vector< std::shared_ptr<ofxSvgGroup> > of all the groups.
	std::vector< std::shared_ptr<ofxSvgGroup> > getAllGroups();
	
	
	
	/*
	/// -- Search Functions with aPath argument for searching and retrieving elements. --
	 
	/// \param std::string aPath to the element separated by semi colons and ending with the name of the desired element.
	/// If empty, will search children.
	/// For example: "MyGroup:MyElement" will attempt to retrieve an element with name "MyElement" from inside the group "MyGroup"
	 
	/// Wildcards are also acceptable and will recursively search until the element with the name is found.
	/// For example: "*:MyElement" will recursively search for the element "MyElement".
	 
	/// \param bool bStrict if true, will include elements with a name that matches the final component of the apath.
	/// If bStrict is false, will include elements that contain the final component of the aPath.
	/// -- For example: Argument aPath = "MyElement" --
	/// Argument bStrict = true; Will NOT return an element with name "MyElement2".
	/// Argument bStrict = false; Will return an element with name "MyElement2".
	*/
	
	
	
	
	/// \brief Retrieve an element at a path, casted to a type. See notes above about search functions.
	/// Example call auto myEle = group->get<ofxSvgRectangle>("MyRectangle", false);
	/// \param std::string aPath to the element separated by semi colons and ending with the name of the desired element. If empty, will search children.
	/// \param bool bStrict if true, search only elements with a name that matches the final element of the aPath component.
	/// \return std::shared_ptr< ofxSvg_T > and invalid if not found.
	template<typename ofxSvg_T>
	std::shared_ptr< ofxSvg_T > get( std::string aPath, bool bStrict = false ) {
		auto stemp = getElement( aPath, bStrict );
		if( !stemp ) {
			return std::shared_ptr<ofxSvg_T>();
		}
		return std::dynamic_pointer_cast<ofxSvg_T>( stemp );
	}
	
	/// \brief Retrieve an element from the vector of children.
	/// \param int aIndex the index to the element in the vector of children.
	/// \return std::shared_ptr< ofxSvg_T > and invalid if not found.
	template<typename ofxSvg_T>
	std::shared_ptr< ofxSvg_T > get( int aIndex ) {
		if( aIndex > -1 && aIndex < mChildren.size() ) {
			return std::dynamic_pointer_cast<ofxSvg_T>( mChildren[ aIndex ] );
		}
		return std::shared_ptr< ofxSvg_T >();
	}
	
	/// \brief Retrieve elements in a group of a specific type. See notes above about search functions.
	/// \param std::string aPathToGroup path to the group to be searched separated by semi colons and ending with the name of the desired group.
	/// If empty, will search children.
	/// \param bool bStrict if true, search only elements with a name that matches the final element of the aPath component.
	/// \return std::vector< std::shared_ptr<ofxSvg_T> > casted to type.
	template<typename ofxSvg_T>
	std::vector< std::shared_ptr<ofxSvg_T> > getElementsForType( std::string aPathToGroup, bool bStrict= false ) {
		auto temp = std::make_shared<ofxSvg_T>();
		auto sType = temp->getType();
		
		std::vector< std::shared_ptr<ofxSvg_T> > telements;
		std::vector< std::shared_ptr<ofxSvgElement> > elementsToSearch;
		if( aPathToGroup == "" ) {
			elementsToSearch = mChildren;
		} else {
			auto temp = getElement( aPathToGroup, bStrict );
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
	
	/// \brief Retrieve the first element in a group of a specific type. See notes above about search functions.
	/// \param std::string aPathToGroup path to group to be searched separated by semi colons and ending with the name of the desired group.
	/// If empty, will search children.
	/// \param bool bStrict if true, search only elements with a name that matches the final element of the aPath component.
	/// \return std::shared_ptr<ofxSvg_T> casted to type and invalid if not found.
	template<typename ofxSvg_T>
	std::shared_ptr<ofxSvg_T> getFirstElementForType( std::string aPathToGroup, bool bStrict= false ) {
		auto eles = getElementsForType<ofxSvg_T>(aPathToGroup, bStrict );
		if( eles.size() > 0 ) {
			return eles[0];
		}
		return std::shared_ptr<ofxSvg_T>();
	}
	
	
	/// \brief Retrieve all of the elements recursively in the group of a specific type.
	/// \return std::vector< std::shared_ptr<ofxSvg_T> > casted to type.
	template<typename ofxSvg_T>
	std::vector< std::shared_ptr<ofxSvg_T> > getAllElementsForType() {
		
		auto temp = std::make_shared<ofxSvg_T>();
		auto sType = temp->getType();
		
		std::vector< std::shared_ptr<ofxSvg_T> > telements;
		auto elementsToSearch = getAllElements(true);
		
		for( std::size_t i = 0; i < elementsToSearch.size(); i++ ) {
			if( elementsToSearch[i]->getType() == sType ) {
				telements.push_back( std::dynamic_pointer_cast<ofxSvg_T>(elementsToSearch[i]) );
			}
		}
		return telements;
	}
	
	/// \brief Retrieve all of the elements recursively in the group of a specific type filtered by the name provided.
	/// \param std::string aname Name to be matched against the elements name.
	/// \param bool bStrict if true, the element name mush match aname, if false, the element name must be contained in aname.
	/// \return std::vector< std::shared_ptr<ofxSvg_T> > casted to type.
	template<typename ofxSvg_T>
	std::vector< std::shared_ptr<ofxSvg_T> > getAllElementsForTypeForName(std::string aname, bool bStrict = false) {
		std::vector< std::shared_ptr<ofxSvg_T> > relements;
		auto elementsToSearch = getAllElementsForType<ofxSvg_T>();
		for( std::size_t i = 0; i < elementsToSearch.size(); i++ ) {
			if( bStrict ) {
				if( elementsToSearch[i]->getCleanName() == aname ) {
					relements.push_back( std::dynamic_pointer_cast<ofxSvg_T>(elementsToSearch[i]) );
				}
			} else {
				if( ofIsStringInString(elementsToSearch[i]->getCleanName(), aname) ) {
					relements.push_back( std::dynamic_pointer_cast<ofxSvg_T>(elementsToSearch[i]) );
				}
			}
		}
		
		return relements;
	}
	
	/// \brief Retrieve all of the elements recursively in the group that have a path.
	/// Including ofxSvgPath, ofxSvgRectangle, ofxSvgCircle, ofxSvgEllipse
	/// \return std::vector< std::shared_ptr<ofxSvgPath> >.
	std::vector< std::shared_ptr<ofxSvgPath> > getAllElementsWithPath();
	
	/// \brief Retrieve an element in this group for a path. See notes above about search functions.
	/// \param aPath to the element separated by semi colons and ending with the name of the desired element.
	/// \param bool bStrict if true, search only elements with a name that matches the final element of the aPath component.
	/// \return std::shared_ptr<ofxSvgElement>. Valid if found.
	std::shared_ptr<ofxSvgElement> getElement( std::string aPath, bool bStrict = false );
	
	/// \brief Retrieve all of the elements recursively in the group filtered by the name provided.
	/// \param std::string aname Name to be matched against the elements name.
	/// \param bool bStrict if true, the element name mush match aname, if false, the element name must be contained in aname.
	/// \return std::vector< std::shared_ptr<ofxSvgElement> >.
	std::vector< std::shared_ptr<ofxSvgElement> > getAllElementsForName( const std::string& aname, bool bStrict = false );
	
	/// \brief Retrieve child elements in the group filtered by the name provided.
	/// \param std::string aname Name to be matched against the elements name.
	/// \param bool bStrict if true, the element name mush match aname, if false, the element name must be contained in aname.
	/// \return std::vector< std::shared_ptr<ofxSvgElement> >.
	std::vector< std::shared_ptr<ofxSvgElement> > getChildrenForName( const std::string& aname, bool bStrict = false );
	
	/// \brief Retrieve child elements in the group of a specific type filtered by the name provided.
	/// \param std::string aname Name to be matched against the elements name.
	/// \param bool bStrict if true, the element name mush match aname, if false, the element name must be contained in aname.
	/// \return std::vector< std::shared_ptr<ofxSvg_T> > casted to type.
	template<typename ofxSvg_T>
	std::vector< std::shared_ptr<ofxSvg_T> > getChildrenForTypeForName( const std::string& aname, bool bStrict = false ) {
		auto temp = std::make_shared<ofxSvg_T>();
		auto sType = temp->getType();
		
		std::vector< std::shared_ptr<ofxSvg_T> > relements;
		auto namedKids = getChildrenForName(aname);
		for( auto& namedKid : namedKids ) {
			if( namedKid->getType() != sType ) continue;
			relements.push_back(std::dynamic_pointer_cast<ofxSvg_T>(namedKid));
		}
		return relements;
	}
	
	/// \brief Retrieve child elements in the group of a specific type.
	/// \return std::vector< std::shared_ptr<ofxSvg_T> > casted to type.
	template<typename ofxSvg_T>
	std::vector< std::shared_ptr<ofxSvg_T> > getChildrenForType() {
		return getElementsForType<ofxSvg_T>("");
	}
	
	/// \brief Retrieve the first child element of a specific type.
	/// \return std::shared_ptr<ofxSvg_T> casted to type; Invalid if not found.
	template<typename ofxSvg_T>
	std::shared_ptr<ofxSvg_T> getFirstChildForType() {
		return getFirstElementForType<ofxSvg_T>("");
	}
	
	/// \brief Replace an item with a new item.
	/// \param shared_ptr<ofxSvgElement> aOriginal Element to be replaced.
	/// \param shared_ptr<ofxSvgElement> aNew Element used to replace aOriginal.
	/// \return bool true if aOriginal was replaced.
	bool replace( std::shared_ptr<ofxSvgElement> aOriginal, std::shared_ptr<ofxSvgElement> aNew );
	
	/// \brief Remove an element from this group or child groups.
	/// \param shared_ptr<ofxSvgElement> aelement to be removed.
	/// \return bool true if element was found and removed.
	virtual bool remove( std::shared_ptr<ofxSvgElement> aelement );
	/// \brief Remove elements in a vector from this group or child groups. Example: svg.remove<ofxSvgCircle>();
	/// \param vector<shared_ptr<ofxSvgElement> > aelements to be removed.
	/// \return bool true if all of the elements were found and removed.
	template<typename ofxSvg_T>
	bool removeElements( std::vector<std::shared_ptr<ofxSvg_T> > aelements ) {
		if( aelements.size() < 1 ) {
			return false;
		}
		
		bool bAllRemoved = true;
		for( auto& aele : aelements ) {
			if( !aele ) {
				ofLogWarning("ofxSvgGroup::remove") << "element is invalid.";
				bAllRemoved = false;
				continue;
			}
			bool bEleRemoved = remove( aele );
			if( !bEleRemoved ) {
				bAllRemoved = false;
			}
		}
		return bAllRemoved;
	}
	
	/// \brief Add a child element of provided type.
	/// \param std::string aname Name to give the created element.
	/// \return std::shared_ptr<ofxSvg_T>.
	template<typename ofxSvg_T>
	std::shared_ptr<ofxSvg_T> add(std::string aname) {
		auto element = std::make_shared<ofxSvg_T>();
		element->name = aname;
		mChildren.push_back(element);
		return element;
	};
	
	/// \brief Add a child element.
	/// \param std::shared_ptr<ofxSvgElement> aele Element to add as a child of this group.
	void add( std::shared_ptr<ofxSvgElement> aele ) { mChildren.push_back(aele); }
	
	/// \brief Create a string representation of the group hierarchy.
	/// \param int nlevel (optional) used for indentation.
	virtual std::string toString(int nlevel = 0) override;
	
	/// \brief Disable shape colors on children.
	void disableColors();
	
	/// \brief Disable shape colors on children.
	void enableColors();
	
protected:
	void _getElementForNameRecursive(std::vector< std::string >& aNamesToFind,
									 std::shared_ptr<ofxSvgElement>& aTarget,
									 std::vector< std::shared_ptr<ofxSvgElement> >& aElements,
									 bool bStrict
									 );
	void _getAllElementsRecursive(std::vector< std::shared_ptr<ofxSvgElement> >& aElesToReturn,
								  std::shared_ptr<ofxSvgElement> aele,
								  bool aBIncludeGroups
								  );
	
	void _getAllGroupsRecursive( std::vector< std::shared_ptr<ofxSvgGroup> >& aGroupsToReturn, std::shared_ptr<ofxSvgElement> aele );
	
	void _replaceElementRecursive(std::shared_ptr<ofxSvgElement> aTarget,
								  std::shared_ptr<ofxSvgElement> aNew,
								  std::vector< std::shared_ptr<ofxSvgElement> >& aElements,
								  bool& aBSuccessful
								  );
	
	void _removeElementRecursive(std::shared_ptr<ofxSvgElement> aTarget,
								 std::vector<std::shared_ptr<ofxSvgElement> >& aElements,
								 bool& aBSuccessful
								 );
	
	std::vector< std::shared_ptr<ofxSvgElement> > mChildren;
	
	
	virtual std::shared_ptr<ofxSvgElement> clone() override {
		//ofLogVerbose("ofxSvgGroup") << "std::shared_ptr<ofxSvgElement> clone():";
		auto newEle = std::make_shared<ofxSvgGroup>(*this);
		newEle->mChildren.clear();
		newEle->mChildren.reserve(mChildren.size());
		for( const auto& ptr : mChildren ) {
			if( ptr ) {
				auto newKid = ptr->clone();
				newKid->setParent(*this);
				newEle->mChildren.push_back(newKid);
			}
		}
		return newEle;
	};
};
















