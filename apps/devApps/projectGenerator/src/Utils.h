/*
 * Utils.h
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "ofConstants.h"

#include "pugixml.hpp"

#include "ofMain.h"



string getOFRoot();
void setOFRoot(string path);

void findandreplace( std::string& tInput, std::string tFind, std::string tReplace );

bool doesTagAndAttributeExist(pugi::xml_document & doc, string tag, string attribute, string newValue);
pugi::xml_node appendValue(pugi::xml_document & doc, string tag, string attribute, string newValue);



void getFoldersRecursively(const string & path, vector < string > & folderNames);
void getFilesRecursively(const string & path, vector < string > & fileNames);
void getLibsRecursively(const string & path, vector < string > & libFiles, vector < string > & libLibs, string platform="" );



void splitFromLast(string toSplit, string deliminator, string & first, string & second);
void splitFromFirst(string toSplit, string deliminator, string & first, string & second);

void parseAddonsDotMake(string path, vector < string > & addons);

void fixSlashOrder(string & toFix);
string unsplitString (vector < string > strings, string deliminator );

#endif /* UTILS_H_ */
