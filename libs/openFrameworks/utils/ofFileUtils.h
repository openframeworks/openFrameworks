#pragma once

#include "ofConstants.h"
#include "Poco/File.h"

//----------------------------------------------------------
// ofBuffer
//----------------------------------------------------------

class ofBuffer{
	
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

	char * getBinaryBuffer();
	const char * getBinaryBuffer() const;

	string getText() const;

	long size() const;

	string getNextLine();
	string getFirstLine();

	friend ostream & operator<<(ostream & ostr,ofBuffer & buf);
	friend istream & operator>>(istream & istr,ofBuffer & buf);

private:
	vector<char> 	buffer;
	long 			nextLinePos;
};

//--------------------------------------------------
ofBuffer ofBufferFromFile(const string & path, bool binary=false);

//--------------------------------------------------
bool ofBufferToFile(const string & path, ofBuffer & buffer, bool binary=false);


//--------------------------------------------------
class ofFilePath{
public:
		
	static string getFileExt(string filename);
	static string removeExt(string filename);
	static string addLeadingSlash(string path);
	static string addTrailingSlash(string path);
	static string removeTrailingSlash(string path);
	static string getPathForDirectory(string path);
	static bool isAbsolute(string path);
	
	static string getFilename(string filePath, bool bRelativeToData = true);	

	static string getEnclosingDirectory(string filePath, bool bRelativeToData = true);
	static string getCurrentWorkingDirectory();
	
};

class ofFile: public fstream{

public:
	
	enum Mode{
		Closed,
		ReadOnly,
		WriteOnly,
		ReadWrite,
		Append
	};

	ofFile();
	ofFile(string filePath, Mode mode=ReadOnly, bool binary=false);
	~ofFile();

	void open(string path, Mode mode=ReadOnly, bool binary=false);
	void close();
	bool create();
	
	bool exists();
	string path();
	
	bool canRead();
	bool canWrite();
	bool canExecute();

	bool isFile();
	bool isLink();
	bool isDirectory();		
	bool isDevice();
	bool isHidden();

	void setWriteable(bool writeable);
	void setReadOnly(bool readable);
	void setExecutable(bool executable);
	
	//these all work for files and directories
	bool copyTo(string path, bool bRelativeToData = true, bool overwrite = false);
	bool moveTo(string path, bool bRelativeToData = true, bool overwrite = false);		
	bool renameTo(string path, bool bRelativeToData = true, bool overwrite = false);
	
	
	//be careful! this deletes a file or folder :) 
	bool remove(bool recursive);	

	uint64_t getSize();

	//if you want access to a few other things
	Poco::File & getPocoFile();
	


	//------------------
	// stream operations
	//------------------

	// since this class inherits from fstream it can be used as a r/w stream:
	// http://www.cplusplus.com/reference/iostream/fstream/


	//helper functions to read/write a whole file to/from an ofBuffer
	ofBuffer readToBuffer();
	bool writeFromBuffer(ofBuffer & buffer);

	
	// this can be used to read the whole stream into an output stream. ie:
	// it's equivalent to rdbuf() just here to make it easier to use
	// cout << file.getFileBuffer() << endl;
	// write_file << file.getFileBuffer();
	filebuf * getFileBuffer() const;


	//-------
	//static helpers
	//-------
	
	//------------------------------------------------------------------------------------------------------------
	static bool copyFromTo(string pathSrc, string pathDst, bool bRelativeToData = true,  bool overwrite = false);

	//be careful with slashes here - appending a slash when moving a folder will causes mad headaches in osx
	static bool moveFromTo(string pathSrc, string pathDst, bool bRelativeToData = true, bool overwrite = false);	
	static bool doesFileExist(string fPath,  bool bRelativeToData = true);
	static bool removeFile(string path, bool bRelativeToData = true);
	
private:
	bool isWriteMode();
	Poco::File::File myFile;
	//fstream fstr;
	Mode mode;
};

class ofDirectory{

public:
	void open(string path);
	void close();
	bool create();

	bool exists();
	string path();

	bool canRead();
	bool canWrite();
	bool canExecute();
	
	bool isDirectory();
	bool isHidden();

	void setWriteable(bool writeable);
	void setReadOnly(bool readable);
	void setExecutable(bool executable);

	bool copyTo(string path, bool bRelativeToData = true, bool overwrite = false);
	bool moveTo(string path, bool bRelativeToData = true, bool overwrite = false);
	bool renameTo(string path, bool bRelativeToData = true, bool overwrite = false);

	//be careful! this deletes a file or folder :)
	bool remove(bool recursive);

//		string getName(unsigned int position); // e.g., "image.png"
//		string getPath(unsigned int position);
//		ofFile getFile(unsigned int position);

	static bool createDirectory(string dirPath, bool bRelativeToData = true, bool recursive = false);
	static bool isDirectoryEmpty(string dirPath, bool bRelativeToData = true );
	static bool doesDirectoryExist(string dirPath, bool bRelativeToData = true);
	static bool removeDirectory(string path, bool deleteIfNotEmpty,  bool bRelativeToData = true);

	//if you want access to a few other things
	Poco::File & getPocoFile();
private:
	Poco::File::File myDir;

};

