#pragma once

#include "ofConstants.h"

//----------------------------------------------------------
// ofBuffer
//----------------------------------------------------------

class ofBuffer{
	vector<char> 	buffer;
	long 			nextLinePos;
public:

	ofBuffer();
	ofBuffer(const char * buffer, int size);
	ofBuffer(istream & stream);
	ofBuffer(const ofBuffer & buffer_);

	~ofBuffer();

	void set(const char * _buffer, int _size);
	bool set(istream & stream);

	bool writeTo(ostream & stream);

	void clear();

	void allocate(long _size);

	char * getBuffer();
	const char * getBuffer() const;

	long size() const;

	string getNextLine();
	string getFirstLine();
};

//--------------------------------------------------
ofBuffer ofBufferFromFile(const string & path, bool binary=false);

//--------------------------------------------------
bool ofBufferToFile(const string & path, ofBuffer & file, bool binary=false);


class ofFileUtils{
	public:

	//------------------------------------------------------------------------------------------------------------
	static bool copyFromTo(string pathSrc, string pathDst, bool bRelativeToData = true,  bool overwrite = false);

	//be careful with slashes here - appending a slash when moving a folder will causes mad headaches
	static bool moveFromTo(string pathSrc, string pathDst, bool bRelativeToData = true, bool overwrite = false);
	
	//------------------------------------------------------------------------------------------------------------
	static string getFileExt(string filename);
	static string removeExt(string filename);
	
	static string addLeadingSlash(string path);
	static string addTrailingSlash(string path);
	static string removeTrailingSlash(string path);

	//------------------------------------------------------------------------------------------------------------	
	static string getFilenameFromPath(string filePath, bool bRelativeToData = true);
	static string getEnclosingDirectoryFromPath(string filePath, bool bRelativeToData = true);

	//------------------------------------------------------------------------------------------------------------
	static string getCurrentWorkingDirectory();
	
	//------------------------------------------------------------------------------------------------------------
	static bool doesDirectoryExist(string dirPath, bool bRelativeToData = true);
	static bool isDirectoryEmpty(string dirPath, bool bRelativeToData = true );

	//------------------------------------------------------------------------------------------------------------
	static bool doesFileExist(string fPath,  bool bRelativeToData = true);

	//------------------------------------------------------------------------------------------------------------
	static bool makeDirectory(string dirPath, bool bRelativeToData = true);
	
	//------------------------------------------------------------------------------------------------------------
	static bool deleteFile(string path, bool bRelativeToData = true);
	static bool deleteFolder(string path, bool bRelativeToData = true);

};
