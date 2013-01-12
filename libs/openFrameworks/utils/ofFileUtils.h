#pragma once

#include "ofConstants.h"
#include "Poco/File.h"

//----------------------------------------------------------
// ofBuffer
//----------------------------------------------------------

class ofBuffer{
	
public:
	ofBuffer();
	ofBuffer(const char * buffer, unsigned int size);
	ofBuffer(const string & text);
	ofBuffer(istream & stream);
	ofBuffer(const ofBuffer & buffer_);

	~ofBuffer();

	void set(const char * _buffer, unsigned int _size);
	void set(const string & text);
	bool set(istream & stream);
	void append(const string& _buffer);
	void append(const char * _buffer, unsigned int _size);

	bool writeTo(ostream & stream) const;

	void clear();

	void allocate(long _size);

	char * getBinaryBuffer();
	const char * getBinaryBuffer() const;

	string getText() const;
	operator string() const;  // cast to string, to use a buffer as a string
	ofBuffer & operator=(const string & text);

	long size() const;
	string getNextLine();
    string getFirstLine();
	bool isLastLine();
    void resetLineReader();
    
	friend ostream & operator<<(ostream & ostr, const ofBuffer & buf);
	friend istream & operator>>(istream & istr, ofBuffer & buf);

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
	ofFilePath();
	ofFilePath(string filePath);

	bool open(string path);
	void close();
	bool createFile();
	bool createDirectory(bool recursive = false);
	bool exists() const;
	string path() const;

	string getExtension() const;
	string getFileName() const;
	string getBaseName() const; // filename without extension
	ofFilePath getEnclosingDirectory() const;
	string getAbsolutePath() const;

	bool canRead() const;
	bool canWrite() const;
	bool canExecute() const;

	bool isFile() const;
	bool isLink() const;
	bool isDirectory() const;
	bool isDevice() const;
	bool isHidden() const;

	void setWriteable(bool writeable);
	void setReadOnly(bool readable);
	void setExecutable(bool executable);

	//these all work for files and directories
	bool copyTo(string path, bool bRelativeToData = true, bool overwrite = false);
	bool moveTo(string path, bool bRelativeToData = true, bool overwrite = false);
	bool renameTo(string path, bool bRelativeToData = true, bool overwrite = false);


	//be careful! this deletes a file or folder :)
	bool remove(bool recursive=false);

	uint64_t getSize() const;

	//if you want access to a few other things
	Poco::File & getPocoFile();

	// allows to use ofFilePath as a string
	operator string() const;

	//this allows to compare files by their paths, also provides sorting and use as key in stl containers
	bool operator==(const ofFilePath & path) const;
	bool operator!=(const ofFilePath & path) const;
	bool operator<(const ofFilePath & path) const;
	bool operator<=(const ofFilePath & path) const;
	bool operator>(const ofFilePath & path) const;
	bool operator>=(const ofFilePath & path) const;

	ofFilePath operator+(const ofFilePath & path) const;
	ofFilePath & operator+=(const ofFilePath & path);


	static string getFileExt(string filename);
	static string removeExt(string filename);
	static string addLeadingSlash(string path);
	static string addTrailingSlash(string path);
	static string removeTrailingSlash(string path);
	static string removeLeadingSlash(string path);
	static string getPathForDirectory(string path);
	static string getAbsolutePath(string path, bool bRelativeToData = true);

	static bool isAbsolute(string path);
	
	static string getFileName(string filePath, bool bRelativeToData = true);	
	static string getBaseName(string filePath); // filename without extension

	static ofFilePath getEnclosingDirectory(string filePath, bool bRelativeToData = true);
	static ofFilePath getCurrentWorkingDirectory();
	static string join(string path1,string path2);
	
	static ofFilePath getCurrentExePath();
	static ofFilePath getCurrentExeDir();

	static ofFilePath getUserHomeDir();

private:
	Poco::File myFile;
};

class ofFile: public fstream, public ofFilePath{

public:
	
	enum Mode{
		Reference,
		ReadOnly,
		WriteOnly,
		ReadWrite,
		Append
	};

	ofFile();
	ofFile(string filePath, Mode mode=ReadOnly, bool binary=true);
	ofFile(const ofFile & mom);
	ofFile & operator= (const ofFile & mom);
	~ofFile();

	bool open(string path, Mode mode=ReadOnly, bool binary=true);
	bool changeMode(Mode mode, bool binary=false); // reopens a file to the same path with a different mode;
	void close();
	bool create();


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

	static bool copyFromTo(string pathSrc, string pathDst, bool bRelativeToData = true,  bool overwrite = false);

	//be careful with slashes here - appending a slash when moving a folder will causes mad headaches in osx
	static bool moveFromTo(string pathSrc, string pathDst, bool bRelativeToData = true, bool overwrite = false);
	static bool doesFileExist(string fPath,  bool bRelativeToData = true);
	static bool removeFile(string path, bool bRelativeToData = true);

private:
	bool isWriteMode();
	bool openStream(Mode _mode, bool binary);
	void copyFrom(const ofFile & mom);
	Mode mode;
};

class ofDirectory: public ofFilePath{

public:
	ofDirectory();
	ofDirectory(string path);

	void open(string path);
	void close();
	bool create(bool recursive = false);

	//-------------------
	// dirList operations
	//-------------------
	void allowExt(string extension);
	int listDir(string path);
	int listDir();

	string getOriginalDirectory();
	string getName(unsigned int position); // e.g., "image.png"
	string getPath(unsigned int position);
	ofFile getFile(unsigned int position, ofFile::Mode mode=ofFile::Reference, bool binary=false);
	vector<ofFile> getFiles();

	ofFile operator[](unsigned int position);

	bool getShowHidden();

	void reset(); //equivalent to close, just here for bw compatibility with ofxDirList
	void sort();

	unsigned int size();
	int numFiles(); // numFiles is deprecated, use size()

	//-------
	//static helpers
	//-------

	static bool createDirectory(string dirPath, bool bRelativeToData = true, bool recursive = false);
	static bool isDirectoryEmpty(string dirPath, bool bRelativeToData = true );
	static bool doesDirectoryExist(string dirPath, bool bRelativeToData = true);
	static bool removeDirectory(string path, bool deleteIfNotEmpty,  bool bRelativeToData = true);


private:
	string originalDirectory;
	vector <string> extensions;
	vector <ofFile> files;
	bool showHidden;

};

