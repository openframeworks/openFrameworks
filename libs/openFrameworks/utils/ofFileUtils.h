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

class ofFile{

public:
	
	enum Mode{
		Reference, // default, for non-stream operations
		ReadOnly,  // the file will switch automatically to read only if theres any read operation
		WriteOnly, // write modes need to be set explicitly
		ReadWrite,
		Append
	};

	ofFile();
	ofFile(string filePath, Mode mode=Reference, bool binary=false);
	~ofFile();

	void open(string path, Mode mode=Reference, bool binary=false);
	void close();
	bool create();
	
	ofBuffer readToBuffer(bool binary=false);
	bool writeFromBuffer(ofBuffer & buffer, bool binary=false);
	
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

	// writes anything to the file
	// use spaces to separate different elements in the same file
	// ie: file.write(5);
	//     file.write(" ");
	//     file.write(ofVec3f(0,0,1))
	template<typename T>
	bool write(T & t);

	// reads anything from the file
	// ie: file.read(num);
	//     file.read(vec3)
	template<typename T>
	bool read(T & t);

	// allows for cout << file
	friend ostream & operator<<(ostream & ostr,ofFile & file);
	friend istream & operator>>(istream & istr,ofFile & file);

	// used to convert the ofFile to istream or ostream
	// they are used by the operators at the end of the file to allow things like
	// file << "hello" << 5 << ofVec3f(4,5,6)
	// to write to a file or
	// file >> str >> number >> vec3
	// to read from the file to variables of type string int and ofVec3f
	ostream & getWriteStream();
	istream & getReadStream();

	// cast operator, allows to use an ofFile as an ostream or istream
	// ie ofBuffer(ofFile("file.txt"));
	operator ostream&();
	operator istream&();
	


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
	fstream fstr;
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

template<typename T>
inline ostream & operator<<(ofFile & f, const T&t){
	f.getWriteStream() << t;
	return f.getWriteStream();
}

template<typename T>
inline istream & operator>>(ofFile & f, T&t){
	f.getReadStream() >> t;
	return f.getReadStream();
}

template<typename T>
bool ofFile::write(T & t){
	if(path() == "") return false;
	getWriteStream() << t;
	return true;
}

template<typename T>
bool ofFile::read(T & t){
	if(path() == "" || !exists()) return false;
	getReadStream() >> t;
	return true;
}
