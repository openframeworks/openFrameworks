

#pragma once


#include "ofMain.h"
#include "pugixml.hpp"
#include "ofAddon.h"

class baseProject {
    
    public: 
    
    baseProject(){
        bLoaded = false;
    };
    virtual ~baseProject(){};
    
    virtual void setup() = 0;
    
    virtual bool create(string path) = 0;
    virtual bool load(string path) = 0;
    virtual bool save(string path) = 0;

    virtual void addSrc(string srcFile, string folder) = 0;
    virtual void addInclude(string includeName) = 0;
    virtual void addLibrary(string libraryName) = 0;

	virtual void addAddon(ofAddon & addon) =  0;


	virtual string getName() = 0;
	virtual string getPath() = 0;
    //private: 

    pugi::xml_document doc;
    bool bLoaded;
    
    string templatePath;
    
    
};


