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

/// a buffer of raw byte data which can be accessed as simple bytes or as text
class ofBuffer{
	
public:
	ofBuffer();
	
	/// create a buffer and set it's contents from a raw byte pointer,
	/// _size is the number of bytes to read and *must* be <= the number
	/// of bytes allocated in _buffer
	ofBuffer(const char * buffer, std::size_t size);
	
	/// create a buffer and set it's contents from a string
	ofBuffer(const string & text);
	
	/// create a buffer and set it's contents from an input stream,
	/// ioBlockSize is the number of bytes to read from the stream in chunks
	ofBuffer(istream & stream, size_t ioBlockSize = 1024);

	/// set contents of the buffer from a raw byte pointer,
	/// _size is the number of bytes to read and *must* be <= the number
	/// of bytes allocated in _buffer
	void set(const char * _buffer, std::size_t _size);
	
	/// set contents of the buffer from a string
	void set(const string & text);
	
	/// set contents of the buffer from an input stream,
	/// ioBlockSize is the number of bytes to read from the stream in chunks
	bool set(istream & stream, size_t ioBlockSize = 1024);
	
	/// set all bytes in the buffer to a given value
	void setall(char mem);
	
	/// append bytes to the end of buffer from a string
	void append(const string& _buffer);
	
	/// append bytes to the end of the buffer from a raw byte pointer,
	/// _size is the number of bytes to read and *must* be <= the number
	/// of bytes allocated in _buffer
	void append(const char * _buffer, std::size_t _size);
	
	/// request that the buffer capacity be at least enough to contain a specifed number of bytes
	void reserve(size_t size);

	/// write contents of the buffer to an output stream
	bool writeTo(ostream & stream) const;

	/// removes all bytes from the buffer leaving a size of 0
	void clear();

	/// request that the buffer capacity be at least enough to contain a specifed number of bytes
	void allocate(std::size_t _size);
	
	/// resize the buffer so that it contains a specified number of bytes,
	/// if _size is < the current buffer size, the contents are reduced to _size bytes & remaining bytes are removed
	/// if _size is > the current buffer size, the buffer's size is increased to _size_ bytes
    void resize(std::size_t _size);

	/// \returns pointer to internal raw bytes, do not access bytes at indices beyond size()!
	char * getData();
	
	/// \returns const pointer to internal raw bytes, do not access bytes at indices beyond size()!
	const char * getData() const;
	OF_DEPRECATED_MSG("Use getData instead",char * getBinaryBuffer());
	OF_DEPRECATED_MSG("Use getData instead",const char * getBinaryBuffer() const);

	string getText() const; //< get the contents of the buffer as a string
	operator string() const; //< allows use a buffer as a string via cast
	ofBuffer & operator=(const string & text); //< set contents of the buffer from a string

	/// \returns the size of the buffer's content in bytes
	std::size_t size() const;

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

	/// a line of text in the buffer
	struct Line: public std::iterator<std::forward_iterator_tag,Line>{
		Line(vector<char>::iterator _begin, vector<char>::iterator _end);
        const string & operator*() const;
        const string * operator->() const;
        const string & asString() const;
        Line& operator++(); //< increment to the next line
        Line operator++(int); //< increment to a number of lines
        bool operator!=(Line const& rhs) const;
        bool operator==(Line const& rhs) const;
        bool empty() const; //< is this line empty? (aka empty string "")

	private:
        string line;
        vector<char>::iterator _current, _begin, _end;
	};

	/// a series of text lines in the buffer
	struct Lines{
		Lines(vector<char>::iterator begin, vector<char>::iterator end);
        Line begin(); //< get the first line in the buffer
        Line end(); //< get the last line in the buffer

	private:
        vector<char>::iterator _begin, _end;
	};

	/// \returns the contents of the buffer as a series of text lines
	/// ie. if the buffer loads a text file with lines separated by an endline
	/// char '\n', you can access each line individually using Line structs
	Lines getLines();

private:
	vector<char> 	buffer;
	Line			currentLine;
};

//--------------------------------------------------
/// reads the contents of a file at path into a buffer,
/// set binary to true if you are reading binary data aka an image, not text
ofBuffer ofBufferFromFile(const string & path, bool binary=false);

//--------------------------------------------------
/// write the contents of a buffer to a file at path,
/// set binary to true if you are writing binary data aka an image, not text
bool ofBufferToFile(const string & path, ofBuffer & buffer, bool binary=false);


//--------------------------------------------------
/// static class for working with file path strings
class ofFilePath{
public:
	
	/// \returns the extension of a filename, ie. "duck.jpg" -> "jpg"
	static string getFileExt(const std::string& filename);
	
	/// \returns the filename without it's extension, ie. "duck.jpg" ->"duck"
	static string removeExt(const std::string& filename);
	
	/// \returns a path prepended with a slash, ie. "images" -> "/images"
	static string addLeadingSlash(const std::string& path);
	
	/// \returns a path appended with a slash, ie. "images" -> "images/"
	static string addTrailingSlash(const std::string& path);
	
	/// \returns a path with the trailing slash removed (if found), ie. "images/" -> "images"
	static string removeTrailingSlash(const std::string& path);
	
	/// \returns a cleaned up a directory path by adding a trailing slash if needed
	///
	/// if it's a windows-style path string using "\", it will add a "\"
	/// if it's a unix-style path string using "/", it will add a "/"
	static string getPathForDirectory(const std::string& path);
	
	/// \returns the absolute, full path for a given path,
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images"
	///
	/// set bRelativeToData to true if you are working with paths that are *not*
	/// in the data folder and want the direct path without relative ../../
	static string getAbsolutePath(const std::string& path, bool bRelativeToData = true);

	/// \returns true if the path is an absolute path,
	/// ie. "images" -> false, "/Users/mickey/of/apps/myApps/Donald/bin/data/images" -> true
	static bool isAbsolute(const std::string& path);
	
	/// \returns the filename of a given path by stripping the parent directories,
	/// ie. "images/duck.jpg"  -> "duck.jpg", assumes the path is in the data folder
	/// set bRelativeToData to false if you are working with paths that are *not*
	/// in the data folder
	static string getFileName(const std::string& filePath, bool bRelativeToData = true);
	
	/// \returns the path without it's last component
	/// ie. "images/duck.jpg" -> "images" and "images/some/folder" -> "images/some"
	static string getBaseName(const std::string& filePath);

	/// \returns the enclosing parent directory of a path,
	/// ie. "images/duck.jpg" -> "images", assumes the path is in the data folder
	/// set bRelativeToData to false if you are working with paths that are *not*
	/// in the data folder
	static string getEnclosingDirectory(const std::string& filePath, bool bRelativeToData = true);
	
	/// creates the enclosing parent directory of a path,
	/// ie. "images" is the enclosing dir of "duck.jpg" = "images/duck.jpg"
	///
	/// automatically creates nested dirs as required, set bRecursive = false to override
	///
	/// assumes the path is in the data folder, set bRelativeToData to false if
	/// you are working with paths that are *not* in the data folder
	static bool createEnclosingDirectory(const std::string& filePath, bool bRelativeToData = true, bool bRecursive = true);
	
	/// \returns the full path to the app's current working directory,
	/// this may be the app's parent directory or the location the app was launched from (aka on the commandline)
	/// note: this location may change after the cd() std C function
	static string getCurrentWorkingDirectory();
	
	/// \returns a single path by joining path1 & path2 using a slash
	static string join(const std::string& path1, const std::string& path2);
	
	/// \returns the full path to the application's executable file,
	/// Mac: the binary within the application's .app bundle Contents/MacOS dir
	/// Windows: the .exe
	/// Linux: the binary file itself
	static string getCurrentExePath();
	
	/// \returns the full path to the application's parent directory,
	/// Windows & Linux: the application's parent directory
	/// Mac: the Contents/MacOS folder within the application's .app bundle
	static string getCurrentExeDir();

	/// \returns absolute path to the user's home directory,
	/// Mac OSX: /Users/<username>
	/// Windows: <root>\Users\<username>
	/// Linux: /home/<username>
	static string getUserHomeDir();

	/// \returns one path relative to another,
	/// ie. the relative path of "images/felines/lions" to "images/felines/tigers"
	/// is "../tigers"
	static string makeRelative(const std::string & from, const std::string & to);
};

/// path to a file or directory,
/// inherits from an fstream so you can read/write using the stream operators
/// once a file path has been opened
class ofFile: public fstream{

public:
	
	/// file access mode
	enum Mode{
		Reference,
		ReadOnly,
		WriteOnly,
		ReadWrite,
		Append
	};

	/// create an ofFile instance, does not refer to a specific file until you
	/// either open a file or create a file or directory path
	ofFile();
	
	/// create a new ofFile instance and attempt to open the path as a file
	/// opens as a binary file with read only access by default
	/// set the file access mode depending on how you plan to use the file (read only, read write, etc)
	/// set binary to false if you are explicitly creating a text file
	ofFile(const std::filesystem::path & path, Mode mode=ReadOnly, bool binary=true);
	
	/// create a new file path using the same path & settings of another file
	ofFile(const ofFile & mom);
	
	/// copy the path and settings of a file into this file
	ofFile & operator= (const ofFile & mom);
	
	~ofFile();

	/// open the path as a file,
	/// opens as a text file with read only access by default
	/// set the file access mode depending on how you plan to use the file (read only, read write, etc)
	/// set binary to true if you are working with binary data (aka image, not text)
	bool open(const std::filesystem::path & path, Mode mode=ReadOnly, bool binary=false);
	
	/// reopens the current file path with a different access mode (read only, read write, etc)
	/// set binary to true if you are working with binary data (aka image, not text)
	bool changeMode(Mode mode, bool binary=false);
	
	/// close a currently open file
	void close();
	
	/// creates a file at the current path
	bool create();
	
	/// \returns true if a file exists at the current path
	bool exists() const;
	
	/// \returns the current path
	string path() const;
	
	/// \returns the current path without it's extension, ie. "duck.jpg" ->"duck"
	string getExtension() const;
	
	/// \returns the filename of the current path by stripping the parent directories,
	/// ie. "images/duck.jpg"  -> "duck.jpg", assumes the path is in the data folder
	/// set bRelativeToData to false if you are working with paths that are *not* in the data folder
	string getFileName() const;
	
	/// \returns the current path without it's last component
	/// ie. "images/duck.jpg" -> "images" and "images/some/folder" -> "images/some"
	string getBaseName() const;
	
	/// creates the enclosing parent directory of the file,
	/// ie. "images" is the enclosing dir of "duck.jpg" = "images/duck.jpg"
	///
	/// automatically creates nested dirs as require, set bRecursive = false to override
	///
	/// assumes the path is in the data folder, set bRelativeToData to false if
	/// you are working with paths that are *not* in the data folder
	string getEnclosingDirectory() const;
	
	/// \returns the absolute, full path of the file,
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images"
	string getAbsolutePath() const;

	/// \returns true if the current path is readable
	bool canRead() const;
	
	/// \returns true if the current path is writable
	bool canWrite() const;
	
	/// \returns true if the current path is executable
	bool canExecute() const;

	/// \returns true if the current path is a file and not a directory
	bool isFile() const;
	
	/// \returns true if the current path is a system link to another file or directory
	bool isLink() const;
	
	/// \returns true if the current path is a directory and not a file
	bool isDirectory() const;
	
	/// \returns true if the current path is a device file,
	/// works on Mac & Linux which can represent devices as files,
	/// always returns false on Windows
	bool isDevice() const;
	
	/// \returns true if the current path is hidden,
	/// works on Mac & Linux which denote hidden files by prepending a period
	/// to the filename -> ".hello", always returns false on Windows
	bool isHidden() const;

	/// set the writable flag of the current path
	void setWriteable(bool writeable=true);
	
	/// set the readable flag of the current path
	void setReadOnly(bool readable=true);
	
	/// set the executable flag of the current path
	void setExecutable(bool executable=true);
	
	/// copy the current file or directory path to a new path,
	/// copies relative to the data path & does *not* overwrite by default
	/// does not change the current path
	/// \returns true if the copy was successful
	///
	/// set overwrite = true if you want to overwrite the file or directory at the new path
	///
	/// assumes the new path is in the data folder, set bRelativeToData to false if
	/// you are working with paths that are *not* in the data folder
	bool copyTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false) const;
	
	/// move the current file or directory path to a new path,
	/// moves relative to the data path & does *not* overwrite by default
	/// does not change the current path
	/// \returns true if the move was successful
	///
	/// set overwrite = true if you want to overwrite the file or directory at the new path
	///
	/// assumes the new path is in the data folder, set bRelativeToData to false if
	/// you are working with paths that are *not* in the data folder
	bool moveTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// rename the current file or directory path to a new path,
	/// renames relative to the data path & does *not* overwrite by default
	/// does not change the current path
	/// \returns true if the rename was successful
	///
	/// set overwrite = true if you want to overwrite the file or directory at the new path
	///
	/// assumes the new path is in the data folder, set bRelativeToData to false if
	/// you are working with paths that are *not* in the data folder
	bool renameTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// removes the file or directory at the current path,
	/// does not remove non-empty directories by default
	/// \returns true if the path was removed successfully
	///
	/// be careful! this deletes a file or folder :)
	///
	/// set recursive to true to remove a non-empty directory and it's contents
	bool remove(bool recursive=false);

	/// \returns the size of the file at the current file path
	uint64_t getSize() const;

	// this allows to compare files by their paths, also provides sorting and use as key in stl containers
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
	
	/// read the contents of a file at the current path into a buffer
	ofBuffer readToBuffer();
	
	/// write the contents of a buffer into a file at the current path
	bool writeFromBuffer(const ofBuffer & buffer);
	
	/// read the entire contents of the currently opened file into an output stream,
	/// basically an easy to use equivalent to rdbuf()
	/// ie. ofLogNotice() << file.getFileBuffer();
	///     write_file << file.getFileBuffer();
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

	/// copy source path to destination path,
	/// copies relative to the data path & does *not* overwrite by default
	/// \returns true if the copy was successful
	///
	/// set overwrite = true if you want to overwrite the file or directory at the destination path
	///
	/// assumes the destination path is in the data folder, set bRelativeToData to false if
	/// you are working with paths that are *not* in the data folder
	static bool copyFromTo(const std::string& pathSrc, const std::string& pathDst, bool bRelativeToData = true,  bool overwrite = false);

	//be careful with slashes here - appending a slash when moving a folder will causes mad headaches in osx
	
	/// move source path to destination path,
	/// moves relative to the data path & does *not* overwrite by default
	/// \returns true if the move was successful
	///
	/// set overwrite = true if you want to overwrite the file or directory at the destination path
	///
	/// assumes the destination path is in the data folder, set bRelativeToData to false if
	/// you are working with paths that are *not* in the data folder
	static bool moveFromTo(const std::string& pathSrc, const std::string& pathDst, bool bRelativeToData = true, bool overwrite = false);
	
	/// \returns true if a file or directory exists at a given path
	///
	/// set bRelativeToData to true if you are working with paths that are *not*
	/// in the data folder and want the direct path without relative ../../
	static bool doesFileExist(const std::string& fPath,  bool bRelativeToData = true);
	
	/// remove a file or directory at a given path
	/// \returns true if the path was removed successfully
	///
	/// set bRelativeToData to true if you are working with paths that are *not*
	/// in the data folder and want the direct path without relative ../../
	static bool removeFile(const std::string& path, bool bRelativeToData = true);

private:
	bool isWriteMode();
	bool openStream(Mode _mode, bool binary);
	void copyFrom(const ofFile & mom);
	std::filesystem::path myFile;
	Mode mode;
	bool binary;
};

/// path to a directory, can be used to query file and directory contents
class ofDirectory{

public:

	/// create an ofDirectory instance, does not refer to a specific directory until you
	/// either open or create a directory path
	ofDirectory();
	
	/// create a new ofDirectory instance and attempt to open the path
	ofDirectory(const std::filesystem::path & path);

	/// open a directory path, clears the current file list
	void open(const std::filesystem::path & path);
	
	/// close the current path
	void close();
	
	/// create a directory at the current path
	/// set bRecursive to true to automatically creates nested dirs as required
	bool create(bool recursive = false);

	/// \returns true if a directory exists at the current path
	bool exists() const;
	
	/// \returns the current path
	string path() const;
	
	/// \returns the absolute, full path of the directory,
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images"
	string getAbsolutePath() const;

	/// \returns true if the current path is readable
	bool canRead() const;
	
	/// \returns true if the current path is writeable
	bool canWrite() const;
	
	/// \returns true if the current path is executable
	bool canExecute() const;
	
	/// \returns true if the current path is indeed a directory and not a file
	bool isDirectory() const;
	
	/// \returns true if the current path is hidden,
	/// works on Mac & Linux which denote hidden directories by prepending
	/// a period -> ".hello", always returns false on Windows
	bool isHidden() const;

	/// set the writable flag of the current path
	void setWriteable(bool writeable=true);
	
	/// set the readable flag of the current path
	void setReadOnly(bool readable=true);
	
	/// set the executable flag of the current path
	void setExecutable(bool executable=true);
	
	/// show hidden files & directories when listing files?
	/// Mac & Linux denote hidden directories by prepending a period -> ".hello"
	void setShowHidden(bool showHidden);

	/// copy the current file or directory path to a new path,
	/// copies relative to the data path & does *not* overwrite by default
	/// does not change the current path
	/// \returns true if the copy was successful
	///
	/// set overwrite = true if you want to overwrite the file or directory at the new path
	///
	/// assumes the new path is in the data folder, set bRelativeToData to false if
	/// you are working with paths that are *not* in the data folder
	bool copyTo(const string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// move the current file or directory path to a new path,
	/// moves relative to the data path & does *not* overwrite by default
	/// does not change the current path
	/// \returns true if the move was successful
	///
	/// set overwrite = true if you want to overwrite the file or directory at the new path
	///
	/// assumes the new path is in the data folder, set bRelativeToData to false if
	/// you are working with paths that are *not* in the data folder
	bool moveTo(const string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// rename the current file or directory path to a new path,
	/// renames relative to the data path & does *not* overwrite by default
	/// does not change the current path
	/// \returns true if the rename was successful
	///
	/// set overwrite = true if you want to overwrite the file or directory at the new path
	///
	/// assumes the new path is in the data folder, set bRelativeToData to false if
	/// you are working with paths that are *not* in the data folder
	bool renameTo(const string& path, bool bRelativeToData = true, bool overwrite = false);

	/// removes the file or directory at the current path,
	/// does not remove non-empty directories by default
	/// \returns true if the path was removed successfully
	///
	/// be careful! this deletes a file or folder :)
	///
	/// set recursive to true to remove a non-empty directory and it's contents
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

	/// \returns whether hidden files & directories are shown when listing files
	/// Mac & Linux denote hidden directories by prepending a period -> ".hello"
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

