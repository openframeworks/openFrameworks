/*
 * ofAddon.h
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#ifndef OFADDON_H_
#define OFADDON_H_

#include <map>
#include "ofConstants.h"

class ofAddon {

public:
	ofAddon();
	ofAddon(string path, string platform);

	void fromFS(string path, string platform);
	void fromXML(string installXmlName);
	void clear();

    // this is source files:
    map < string, string > filesToFolders;      //the addons has had, for each file,
                                                //sometimes a listing of what folder to put it in, such as "addons/ofxOsc/src"

    vector < string > srcFiles;
    vector < string > libs;
    vector < string > includePaths;
    string name;
};

#endif /* OFADDON_H_ */
