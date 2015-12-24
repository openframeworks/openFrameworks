#pragma once

#include "ofConstants.h"
#if !_MSC_VER
#define BOOST_NO_CXX11_SCOPED_ENUMS
#define BOOST_NO_SCOPED_ENUMS
#endif
#include <boost/filesystem.hpp>
namespace std {
	namespace filesystem = boost::filesystem;
}

//----------------------------------------------------------
// ofBuffer
//----------------------------------------------------------

class ofBuffer{
	
public:
	ofBuffer();
	ofBuffer(const char * buffer, std::size_t size);
	ofBuffer(const string & text);
	ofBuffer(istream & stream, size_t ioBlockSize = 1024);

	void set(const char * _buffer, std::size_t _size);
	void set(const string & text);
	bool set(istream & stream, size_t ioBlockSize = 1024);
	void append(const string& _buffer);
	void append(const char * _buffer, std::size_t _size);

	bool writeTo(ostream & stream) const;

	void clear();

	void allocate(std::size_t _size);

	char * getData();
	const char * getData() const;
	OF_DEPRECATED_MSG("Use getData instead",char * getBinaryBuffer());
	OF_DEPRECATED_MSG("Use getData instead",const char * getBinaryBuffer() const);

	string getText() const;
	operator string() const;  // cast to string, to use a buffer as a string
	ofBuffer & operator=(const string & text);

	long size() const;

	OF_DEPRECATED_MSG("use a lines iterator instead",string getNextLine());
	OF_DEPRECATED_MSG("use a lines iterator instead",string getFirstLine());
	OF_DEPRECATED_MSG("use a lines iterator instead",bool isLastLine());
	OF_DEPRECATED_MSG("use a lines iterator instead",void resetLineReader());
    
	friend ostream & operator<<(ostream & ostr, const ofBuffer & buf);
	friend istream & operator>>(istream & istr, ofBuffer & buf);

	vector<char>::iterator begin();
	vector<char>::iterator end();
	vector<char>::const_iterator begin() const;
	vector<char>::const_iterator end() const;
	vector<char>::reverse_iterator rbegin();
	vector<char>::reverse_iterator rend();
	vector<char>::const_reverse_iterator rbegin() const;
	vector<char>::const_reverse_iterator rend() const;

	struct Line: public std::iterator<std::forward_iterator_tag,Line>{
		Line(vector<char>::iterator _begin, vector<char>::iterator _end);
        const string & operator*() const;
        const string * operator->() const;
        const string & asString() const;
        Line& operator++();
        Line operator++(int);
        bool operator!=(Line const& rhs) const;
        bool operator==(Line const& rhs) const;
        bool empty() const;

	private:
        string line;
        vector<char>::iterator _current, _begin, _end;
	};

	struct Lines{
		Lines(vector<char> & buffer);
        Line begin();
        Line end();

	private:
        vector<char>::iterator _begin, _end;
	};

	Lines getLines();

private:
	vector<char> 	buffer;
	Line			currentLine;
};

//--------------------------------------------------
ofBuffer ofBufferFromFile(const string & path, bool binary=false);

//--------------------------------------------------
bool ofBufferToFile(const string & path, ofBuffer & buffer, bool binary=false);


//--------------------------------------------------
class ofFilePath{
public:
		
	static string getFileExt(const std::string& filename);
	static string removeExt(const std::string& filename);
	static string addLeadingSlash(const std::string& path);
	static string addTrailingSlash(const std::string& path);
	static string removeTrailingSlash(const std::string& path);
	static string getPathForDirectory(const std::string& path);
	static string getAbsolutePath(const std::string& path, bool bRelativeToData = true);

	static bool isAbsolute(const std::string& path);
	
	static string getFileName(const std::string& filePath, bool bRelativeToData = true);
	static string getBaseName(const std::string& filePath); // filename without extension

	static string getEnclosingDirectory(const std::string& filePath, bool bRelativeToData = true);
	static bool createEnclosingDirectory(const std::string& filePath, bool bRelativeToData = true, bool bRecursive = true);
	static string getCurrentWorkingDirectory();
	static string join(const std::string& path1, const std::string& path2);
	
	static string getCurrentExePath();
	static string getCurrentExeDir();

	static string getUserHomeDir();

	static string makeRelative(const std::string & from, const std::string & to);
};

class ofFile: public fstream{

public:
	
	enum Mode{
		Reference,
		ReadOnly,
		WriteOnly,
		ReadWrite,
		Append
	};

	ofFile();
	ofFile(const std::filesystem::path & path, Mode mode=ReadOnly, bool binary=true);
	ofFile(const ofFile & mom);
	ofFile & operator= (const ofFile & mom);
	~ofFile();

	bool open(const std::filesystem::path & path, Mode mode=ReadOnly, bool binary=false);
	bool changeMode(Mode mode, bool binary=false); // reopens a file to the same path with a different mode;
	void close();
	bool create();
	
	bool exists() const;
	string path() const;
	
	string getExtension() const;
	string getFileName() const;
	string getBaseName() const; // filename without extension
	string getEnclosingDirectory() const;
	string getAbsolutePath() const;

	bool canRead() const;
	bool canWrite() const;
	bool canExecute() const;

	bool isFile() const;
	bool isLink() const;
	bool isDirectory() const;
	bool isDevice() const;
	bool isHidden() const;

	void setWriteable(bool writeable=true);
	void setReadOnly(bool readable=true);
	void setExecutable(bool executable=true);
	
	//these all work for files and directories
	bool copyTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false) const;
	bool moveTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false);
	bool renameTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false);
	
	
	//be careful! this deletes a file or folder :) 
	bool remove(bool recursive=false);

	uint64_t getSize() const;

	//this allows to compare files by their paths, also provides sorting and use as key in stl containers
	bool operator==(const ofFile & file) const;
	bool operator!=(const ofFile & file) const;
	bool operator<(const ofFile & file) const;
	bool operator<=(const ofFile & file) const;
	bool operator>(const ofFile & file) const;
	bool operator>=(const ofFile & file) const;


	//------------------
	// stream operations
	//------------------

	// since this class inherits from fstream it can be used as a r/w stream:
	// http://www.cplusplus.com/reference/iostream/fstream/


	//helper functions to read/write a whole file to/from an ofBuffer
	ofBuffer readToBuffer();
	bool writeFromBuffer(const ofBuffer & buffer);

	
	// this can be used to read the whole stream into an output stream. ie:
	// it's equivalent to rdbuf() just here to make it easier to use
	// ofLogNotice() << file.getFileBuffer();
	// write_file << file.getFileBuffer();
	filebuf * getFileBuffer() const;
	
	operator std::filesystem::path(){
		return myFile;
	}

    operator const std::filesystem::path() const{
        return myFile;
    }
	

	//-------
	//static helpers
	//-------

	static bool copyFromTo(const std::string& pathSrc, const std::string& pathDst, bool bRelativeToData = true,  bool overwrite = false);

	//be careful with slashes here - appending a slash when moving a folder will causes mad headaches in osx
	static bool moveFromTo(const std::string& pathSrc, const std::string& pathDst, bool bRelativeToData = true, bool overwrite = false);
	static bool doesFileExist(const std::string& fPath,  bool bRelativeToData = true);
	static bool removeFile(const std::string& path, bool bRelativeToData = true);

private:
	bool isWriteMode();
	bool openStream(Mode _mode, bool binary);
	void copyFrom(const ofFile & mom);
	std::filesystem::path myFile;
	Mode mode;
	bool binary;
};

class ofDirectory{

public:
	ofDirectory();
	ofDirectory(const std::filesystem::path & path);

	void open(const std::filesystem::path & path);
	void close();
	bool create(bool recursive = false);

	bool exists() const;
	string path() const;
	string getAbsolutePath() const;

	bool canRead() const;
	bool canWrite() const;
	bool canExecute() const;
	
	bool isDirectory() const;
	bool isHidden() const;

	void setWriteable(bool writeable=true);
	void setReadOnly(bool readable=true);
	void setExecutable(bool executable=true);
	void setShowHidden(bool showHidden);

	bool copyTo(const string& path, bool bRelativeToData = true, bool overwrite = false);
	bool moveTo(const string& path, bool bRelativeToData = true, bool overwrite = false);
	bool renameTo(const string& path, bool bRelativeToData = true, bool overwrite = false);

	//be careful! this deletes a file or folder :)
	bool remove(bool recursive);

	//-------------------
	// dirList operations
	//-------------------
	void allowExt(const string& extension);
	std::size_t listDir(const string& path);
	std::size_t listDir();

	string getOriginalDirectory() const;
	string getName(std::size_t position) const; // e.g., "image.png"
	string getPath(std::size_t position) const;
	ofFile getFile(std::size_t position, ofFile::Mode mode=ofFile::Reference, bool binary=false) const;
	const vector<ofFile> & getFiles() const;

	ofFile operator[](std::size_t position) const;

	bool getShowHidden() const;

	void reset(); //equivalent to close, just here for bw compatibility with ofxDirList
	void sort();
    ofDirectory getSorted();

	std::size_t size() const;

	OF_DEPRECATED_MSG("Use size() instead.", int numFiles());

	//this allows to compare dirs by their paths, also provides sorting and use as key in stl containers
	bool operator==(const ofDirectory & dir) const;
	bool operator!=(const ofDirectory & dir) const;
	bool operator<(const ofDirectory & dir) const;
	bool operator<=(const ofDirectory & dir) const;
	bool operator>(const ofDirectory & dir) const;
	bool operator>=(const ofDirectory & dir) const;

	operator std::filesystem::path(){
		return myDir;
	}

    operator const std::filesystem::path() const{
        return myDir;
    }

	//-------
	//static helpers
	//-------

	static bool createDirectory(const std::string& dirPath, bool bRelativeToData = true, bool recursive = false);
	static bool isDirectoryEmpty(const std::string& dirPath, bool bRelativeToData = true );
	static bool doesDirectoryExist(const std::string& dirPath, bool bRelativeToData = true);
	static bool removeDirectory(const std::string& path, bool deleteIfNotEmpty,  bool bRelativeToData = true);

	vector<ofFile>::const_iterator begin() const;
	vector<ofFile>::const_iterator end() const;
	vector<ofFile>::const_reverse_iterator rbegin() const;
	vector<ofFile>::const_reverse_iterator rend() const;

private:
	std::filesystem::path myDir;
	string originalDirectory;
	vector <string> extensions;
	vector <ofFile> files;
	bool showHidden;

};

