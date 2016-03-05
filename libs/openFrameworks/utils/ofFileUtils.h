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

/// \class ofBuffer
/// \brief a buffer of raw byte data which can be accessed as simple bytes or as text
class ofBuffer{
	
public:
	ofBuffer();
	
	/// \brief create a buffer and set it's contents from a raw byte pointer
	/// \param buffer pointer to the raw byte buffer to copy data from
	/// \param _size the number of bytes to read
	/// \warning buffer *must* not be NULL
	/// \warning _size *must* be <= the number of bytes allocated in _buffer
	ofBuffer(const char * buffer, std::size_t size);
	
	/// \brief create a buffer and set it's contents from a string
	/// \param text string to copy data from
	ofBuffer(const string & text);
	
	/// \brief create a buffer and set it's contents from an input stream
	/// \param ioBlockSize the number of bytes to read from the stream in chunks
	ofBuffer(istream & stream, size_t ioBlockSize = 1024);

	/// \brief set contents of the buffer from a raw byte pointer
	/// \param buffer pointer to the raw byte buffer to copy data from
	/// \param _size the number of bytes to read
	/// \warning buffer *must* not be NULL
	/// \warning _size *must* be <= the number of bytes allocated in _buffer
	void set(const char * _buffer, std::size_t _size);
	
	/// \brief set contents of the buffer from a string
	/// \param text string to copy data from
	void set(const string & text);
	
	/// \brief set contents of the buffer from an input stream
	/// \param stream input stream to copy data from
	/// \param ioBlockSize the number of bytes to read from the stream in chunks
	bool set(istream & stream, size_t ioBlockSize = 1024);
	
	/// \brief set all bytes in the buffer to a given value
	/// \param mem byte value to set
	void setall(char mem);
	
	/// \brief append bytes to the end of buffer from a string
	/// \param _buffer string to copy bytes from
	void append(const string& _buffer);
	
	/// \brief append bytes to the end of the buffer from a raw byte pointer
	/// \param buffer pointer to the raw byte buffer to copy data from
	/// \param _size the number of bytes to read
	/// \warning buffer *must* not be NULL
	/// \warning _size *must* be <= the number of bytes allocated in _buffer
	void append(const char * _buffer, std::size_t _size);
	
	/// \brief request that the buffer capacity be at least enough to contain a specified number of bytes
	/// \param size number of bytes to reserve space for
	void reserve(size_t size);

	/// \brief write contents of the buffer to an output stream
	bool writeTo(ostream & stream) const;

	/// \brief removes all bytes from the buffer leaving a size of 0
	void clear();

	/// \brief request that the buffer capacity be at least enough to contain a specified number of bytes
	/// \param _size number of bytes to reserve space for
	void allocate(std::size_t _size);
	
	/// \brief resize the buffer so that it contains a specified number of bytes
	/// \param _size number of bytes to resize the buffer to
	/// if _size is < the current buffer size, the contents are reduced to _size bytes & remaining bytes are removed
	/// if _size is > the current buffer size, the buffer's size is increased to _size_ bytes
    void resize(std::size_t _size);

	/// \return pointer to internal raw bytes
	/// \warning do not access bytes at indices beyond size()!
	char * getData();
	
	/// \return const pointer to internal raw bytes
	/// \warning do not access bytes at indices beyond size()!
	const char * getData() const;
	OF_DEPRECATED_MSG("Use getData instead",char * getBinaryBuffer());
	OF_DEPRECATED_MSG("Use getData instead",const char * getBinaryBuffer() const);

	string getText() const; //< /brief get the contents of the buffer as a string
	operator string() const; //< /brief allows use a buffer as a string via cast
	ofBuffer & operator=(const string & text); //< /brief set contents of the buffer from a string

	/// \return the size of the buffer's content in bytes
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

	/// \class Line
	/// \brief a line of text in the buffer
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

	/// \class Lines
	/// \brief a series of text lines in the buffer
	struct Lines{
		Lines(vector<char>::iterator begin, vector<char>::iterator end);
        Line begin(); //< \brief get the first line in the buffer
        Line end(); //< \brief get the last line in the buffer

	private:
        vector<char>::iterator _begin, _end;
	};

	/// \return the contents of the buffer as a series of text lines
	/// ie. if the buffer loads a text file with lines separated by an endline
	/// char '\n', you can access each line individually using Line structs
	Lines getLines();

private:
	vector<char> 	buffer;
	Line			currentLine;
};

//--------------------------------------------------
/// \brief read the contents of a file at path into a buffer
/// opens as a text file by default
/// \param path file to open
/// \param binary set to true if you are reading binary data aka an image, not text
ofBuffer ofBufferFromFile(const string & path, bool binary=false);

//--------------------------------------------------
/// \brief write the contents of a buffer to a file at path
/// saves as a text file by default
/// \param binary set to true if you are writing binary data aka an image, not text
bool ofBufferToFile(const string & path, ofBuffer & buffer, bool binary=false);

//--------------------------------------------------
/// \class ofFilePath
/// \brief static class for working with file path strings
class ofFilePath{
public:
	
	/// \return the extension of a filename, ie. "duck.jpg" -> "jpg"
	/// \param filename file path
	static string getFileExt(const std::string& filename);
	
	/// \return the filename without it's extension, ie. "duck.jpg" ->"duck"
	/// \param filename file path
	static string removeExt(const std::string& filename);
	
	/// \return a path prepended with a slash, ie. "images" -> "/images"
	/// \param path file or directory path
	static string addLeadingSlash(const std::string& path);
	
	/// \return a path appended with a slash, ie. "images" -> "images/"
	/// \param path directory path
	static string addTrailingSlash(const std::string& path);
	
	/// \return a path with the trailing slash removed (if found), ie. "images/" -> "images"
	/// \param path directory path
	static string removeTrailingSlash(const std::string& path);
	
	/// \return a cleaned up a directory path by adding a trailing slash if needed
	/// \param path directory path
	/// if it's a windows-style path string using "\", it will add a "\"
	/// if it's a unix-style path string using "/", it will add a "/"
	static string getPathForDirectory(const std::string& path);
	
	/// \return the absolute, full path for a given path
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images"
	/// \param path file or directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not*
	/// in the data folder and want the direct path without relative ../../
	static string getAbsolutePath(const std::string& path, bool bRelativeToData = true);

	/// \return true if the path is an absolute path
	/// \param path file or directory path
	/// ie. "images" -> false, "/Users/mickey/of/apps/myApps/Donald/bin/data/images" -> true
	static bool isAbsolute(const std::string& path);
	
	/// \return the filename of a given path by stripping the parent directories
	/// ie. "images/duck.jpg" -> "duck.jpg", assumes the path is in the data folder
	/// \param filePath file path
	/// \param bRelativeToData set to false if you are working with paths that are *not*
	/// in the data folder and want the direct path without relative ../../
	static string getFileName(const std::string& filePath, bool bRelativeToData = true);
	
	/// \return a file path without it's last component
	/// ie. "images/duck.jpg" -> "images" and "images/some/folder" -> "images/some"
	/// \param filePath file path
	static string getBaseName(const std::string& filePath);

	/// \return the enclosing parent directory of a path
	/// ie. "images/duck.jpg" -> "images", assumes the path is in the data folder
	/// \param filePath file path
	/// \param bRelativeToData set to false if you are working with paths that are *not*
	/// in the data folder and want the direct path without relative ../../
	static string getEnclosingDirectory(const std::string& filePath, bool bRelativeToData = true);
	
	/// \brief create the enclosing parent directory of a path
	/// ie. "images" is the enclosing dir of "duck.jpg" = "images/duck.jpg"
	/// assumes the path is in the data folder & automatically creates nested dirs as required
	/// \param bRecursive set = false to override automatically nest dir creation
	/// \param bRelativeToData set to false if you are working with paths that are *not*
	/// in the data folder and want the direct path without relative ../../
	static bool createEnclosingDirectory(const std::string& filePath, bool bRelativeToData = true, bool bRecursive = true);
	
	/// \return the full path to the app's current working directory
	/// this may be the app's parent directory or the location the app was launched from (aka on the commandline)
	/// \warning this location *may* change if you or a library calls the cd() std C function
	static string getCurrentWorkingDirectory();
	
	/// \return a single path by joining path1 & path2 using a slash
	/// ie. "/hello/world" + "foo/bar" -> "/hello/world/foo/bar"
	/// \param path1 left half of the path to join
	/// \param path2 right half of the path to join
	static string join(const std::string& path1, const std::string& path2);
	
	/// \return the full path to the application's executable file
	/// Mac: the binary within the application's .app bundle Contents/MacOS dir
	/// Windows: the .exe
	/// Linux: the binary file itself
	static string getCurrentExePath();
	
	/// \returns the full path to the application's parent directory
	/// Windows & Linux: the application's parent directory
	/// Mac: the Contents/MacOS folder within the application's .app bundle
	static string getCurrentExeDir();

	/// \returns absolute path to the user's home directory,
	/// Mac OSX: /Users/<username>
	/// Windows: <root>\Users\<username>
	/// Linux: /home/<username>
	static string getUserHomeDir();

	/// \returns one path relative to another
	/// ie. the relative path of "images/felines/lions" to "images/felines/tigers" is "../tigers"
	/// \param from starting path
	/// \param to destination path
	static string makeRelative(const std::string & from, const std::string & to);
};

/// \class ofFile
/// \brief path to a file or directory
/// inherits from an fstream so you can read/write using the stream operators
/// once a file path has been opened
class ofFile: public fstream{

public:
	
	/// file access mode
	enum Mode{
		Reference,  //<
		ReadOnly,  //< read only from the file, do not write
		WriteOnly, //< write only to the file, do not read
		ReadWrite, //< read from and write to the file
		Append     //< append data to the end of the file, do not overwrite
	};

	/// \brief create an ofFile instance
	/// does not refer to a specific file until you either open a file or create a file or directory path
	ofFile();
	
	/// \brief create a new ofFile instance and attempt to open the path as a file
	/// opens as a binary file with read only access by default
	/// \param path file path
	/// \param mode file access mode depending on how you plan to use the file (read only, read write, etc)
	/// \param binary set to false if you are explicitly creating a text file
	ofFile(const std::filesystem::path & path, Mode mode=ReadOnly, bool binary=true);
	
	/// \brief create a new file path using the same path & settings of another file
	/// \param mom ofFile instance source
	ofFile(const ofFile & mom);
	
	/// \brief copy the path and settings of an ofFile into this instance
	/// \param mom ofFile instance source
	ofFile & operator= (const ofFile & mom);
	
	~ofFile();

	/// \brief open the path as a file
	/// opens as a text file with read only access by default
	/// \param path file path
	/// \param mode file access mode depending on how you plan to use the file (read only, read write, etc)
	/// \param binary set to true if you are working with binary data (aka image, not text)
	bool open(const std::filesystem::path & path, Mode mode=ReadOnly, bool binary=false);
	
	/// \brief reopen the current file path with a different access mode (read only, read write, etc)
	/// \param mode file access mode depending on how you plan to use the file (read only, read write, etc)
	/// \param binary set to true if you are working with binary data (aka image, not text)
	bool changeMode(Mode mode, bool binary=false);
	
	/// \brief close a currently open file
	void close();
	
	/// \brief create a file at the current path
	bool create();
	
	/// \return true if a file exists at the current path
	bool exists() const;
	
	/// \return the current path
	string path() const;
	
	/// \return the current path without it's extension, ie. "duck.jpg" ->"duck"
	string getExtension() const;
	
	/// \return the filename of the current path by stripping the parent directories
	/// ie. "images/duck.jpg"  -> "duck.jpg"
	string getFileName() const;
	
	/// \return the current path without it's last component
	/// ie. "images/duck.jpg" -> "images" and "images/some/folder" -> "images/some"
	string getBaseName() const;
	
	/// \return the enclosing parent directory of a path
	/// ie. "images/duck.jpg" -> "images", assumes the path is in the data folder
	string getEnclosingDirectory() const;
	
	/// \return the absolute, full path of the file
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images"
	string getAbsolutePath() const;

	/// \return true if the current path is readable
	bool canRead() const;
	
	/// \return true if the current path is writable
	bool canWrite() const;
	
	/// \return true if the current path is executable
	bool canExecute() const;

	/// \return true if the current path is a file and not a directory
	bool isFile() const;
	
	/// \return true if the current path is a system link to another file or directory
	bool isLink() const;
	
	/// \return true if the current path is a directory and not a file
	bool isDirectory() const;
	
	/// \return true if the current path is a device file
	/// works on Mac & Linux which can represent devices as files,
	/// always returns false on Windows
	bool isDevice() const;
	
	/// \return true if the current path is hidden
	/// works on Mac & Linux which denote hidden files by prepending a period
	/// to the filename -> ".hello", always returns false on Windows
	bool isHidden() const;

	/// \brief set the writable flag of the current path
	void setWriteable(bool writeable=true);
	
	/// \brief set the readable flag of the current path
	void setReadOnly(bool readable=true);
	
	/// \brief set the executable flag of the current path
	void setExecutable(bool executable=true);
	
	/// \brief copy the current file or directory path to a new path
	/// copies relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data folder
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or directory at the new path
	/// \return true if the copy was successful
	bool copyTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false) const;
	
	/// \brief move the current file or directory path to a new path
	/// moves relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data folder
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or directory at the new path
	/// \return true if the copy was successful
	bool moveTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief rename the current file or directory path to a new path
	/// renames relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data folder
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or directory at the new path
	/// \return true if the copy was successful
	bool renameTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief removes the file or directory at the current path
	/// does not remove non-empty directories by default
	/// \param recursive set to true to remove a non-empty directory and it's contents
	/// \warning be careful! this deletes a file or folder :)
	/// \return true if the path was removed successfully
	bool remove(bool recursive=false);

	/// \return the size of the file at the current file path
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

	/// \brief copy source path to destination path
	/// copies relative to the data path & does *not* overwrite by default
	/// assumes the source & destination path is in the data folder
	/// \param pathSrc source file or directory path
	/// \param pathDst destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or directory at the new path
	/// \return true if the copy was successful
	static bool copyFromTo(const std::string& pathSrc, const std::string& pathDst, bool bRelativeToData = true,  bool overwrite = false);

	/// \brief move source path to destination path
	/// moves relative to the data path & does *not* overwrite by default
	/// assumes the source & destination path is in the data folder
	/// \param pathSrc source file or directory path
	/// \param pathDst destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or directory at the new path
	/// \warning be careful with slashes here, appending a slash when moving a folder may cause mad headaches in OSX
	/// \return true if the move was successful
	static bool moveFromTo(const std::string& pathSrc, const std::string& pathDst, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief check if a file or directory exists at a given path
	/// \param fPath file path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder and want the direct path without relative ../../
	/// \return true if a file or directory exists
	static bool doesFileExist(const std::string& fPath,  bool bRelativeToData = true);
	
	/// \brief remove a file or directory at a given path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder and want the direct path without relative ../../
	/// \returns true if the path was removed successfully
	static bool removeFile(const std::string& path, bool bRelativeToData = true);

private:
	bool isWriteMode();
	bool openStream(Mode _mode, bool binary);
	void copyFrom(const ofFile & mom);
	std::filesystem::path myFile;
	Mode mode;
	bool binary;
};

/// \class ofDirectory
/// \brief path to a directory, can be used to query file and directory contents
class ofDirectory{

public:

	/// \brief create an ofDirectory instance
	/// does not refer to a specific directory until you either open or create a directory path
	ofDirectory();
	
	/// \brief create an ofDirectory instance and attempt to open the path
	/// \param path directory path
	ofDirectory(const std::filesystem::path & path);

	/// \brief open a directory path
	/// clears the current file list
	/// \param path directory path
	void open(const std::filesystem::path & path);
	
	/// \brief close the currently open path
	void close();
	
	/// \brief create a directory at the current path
	/// \param bRecursive set to true to automatically create nested dirs as required
	bool create(bool recursive = false);

	/// \return true if a directory exists at the current path
	bool exists() const;
	
	/// \return the current path
	string path() const;
	
	/// \return the absolute, full path of the directory,
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images"
	string getAbsolutePath() const;

	/// \return true if the current path is readable
	bool canRead() const;
	
	/// \return true if the current path is writeable
	bool canWrite() const;
	
	/// \return true if the current path is executable
	bool canExecute() const;
	
	/// \return true if the current path is indeed a directory and not a file
	bool isDirectory() const;
	
	/// \return true if the current path is hidden
	/// works on Mac & Linux which denote hidden directories by prepending
	/// a period -> ".hello", always returns false on Windows
	bool isHidden() const;

	/// \brief set the writable flag of the current path
	void setWriteable(bool writeable=true);
	
	/// \brief set the readable flag of the current path
	void setReadOnly(bool readable=true);
	
	/// \brief set the executable flag of the current path
	void setExecutable(bool executable=true);
	
	/// \brief show hidden files & directories when listing files?
	/// Mac & Linux denote hidden directories by prepending a period -> ".hello"
	void setShowHidden(bool showHidden);

	/// \brief copy the current file or directory path to a new path
	/// copies relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data folder
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or directory at the new path
	/// \return true if the copy was successful
	bool copyTo(const string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief move the current file or directory path to a new path
	/// moves relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data folder
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or directory at the new path
	/// \return true if the copy was successful
	bool moveTo(const string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief rename the current file or directory path to a new path
	/// renames relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data folder
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or directory at the new path
	/// \return true if the copy was successful
	bool renameTo(const string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief removes the file or directory at the current path
	/// does not remove non-empty directories by default
	/// \param recursive set to true to remove a non-empty directory and it's contents
	/// \warning be careful! this deletes a file or folder :)
	/// \return true if the path was removed successfully
	bool remove(bool recursive);

	//-------------------
	// dirList operations
	//-------------------
	
	/// \brief allow a file extension when listing the contents the current directory path
	/// setting an allowed extension enables a whitelist mode which only lists extensions which have been
	/// explicitly allowed
	/// \param extension file type extension ie. "jpg", "png", "txt", etc
	void allowExt(const string& extension);
	
	/// \brief open and read the contents of a directory
	/// uses allowed extension whitelist to ignore unwanted file types if allowExt() has been called
	/// \param path directory path
	/// \return number of paths found
	std::size_t listDir(const string& path);
	
	/// \brief open and read the contents of the current directory
	/// uses allowed extension whitelist to ignore unwanted file types if allowExt() has been called
	/// \return number of paths found
	std::size_t listDir();

	/// \return the current path
	string getOriginalDirectory() const;
	
	/// \brief get the filename at a given position in the dir contents list ie. "duck.jpg"
	/// \param position array index in the dir contents list
	/// \warning call listDir() before using this function or the dir contents list will be empty
	/// \throw throws an out of bounds exception if position >= the number of listed dir contents
	/// \return file or directory name
	string getName(std::size_t position) const;
	
	/// \brief get the full path of the file or directory at a given position in the dir contents list
	/// \param position array index in the dir contents list
	/// \warning call listDir() before using this function or the dir contents list will be empty
	/// \throw throws an out of bounds exception if position >= the number of listed dir contents
	/// \return file or directory name including the current path
	string getPath(std::size_t position) const;
	
	/// \brief open an ofFile instance using the path a given position in the dir contents list
	/// opens as a text file with read only access by default
	/// \param position array index in the dir contents list
	/// \param mode file access mode depending on how you plan to use the file (read only, read write, etc)
	/// \param binary set to true if you are working with binary data (aka image, not text)
	/// \warning call listDir() before using this function or the dir contents list will be empty
	/// \throw throws an out of bounds exception if position >= the number of listed dir contents
	/// \return ofFile instance
	ofFile getFile(std::size_t position, ofFile::Mode mode=ofFile::Reference, bool binary=false) const;
	
	/// \return files and directories in the dir contents list
	/// directory contents are automatically listed
	const vector<ofFile> & getFiles() const;

	/// \brief dir contents array operator access
	/// \param position array index in the dir contents list
	/// \warning call listDir() before using this function or the dir contents list will be empty
	/// \throw throws an out of bounds exception if position >= the number of listed dir contents
	/// \return opened ofFile instance
	ofFile operator[](std::size_t position) const;

	/// \return true when hidden files & directories are included when listing files
	/// Mac & Linux denote hidden directories by prepending a period -> ".hello"
	bool getShowHidden() const;

	/// \brief closes the directory, backwards compatibility with ofxDirList
	void reset();
	
	/// \brief sort the dir contents list alphabetically
	/// \warning call listDir() before using this function or their will be nothing to sort
	void sort();
	
	/// \return a sorted ofDirectory instance using the current path
    ofDirectory getSorted();

	/// \return the number of paths in the current dir list
	/// \warning call listDir() before using this function or it will return 0
	std::size_t size() const;

	OF_DEPRECATED_MSG("Use size() instead.", int numFiles());

	// this allows to compare dirs by their paths, also provides sorting and use as key in stl containers
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
	// static helpers
	//-------

	/// \brief create a directory at a given path
	/// creates relative to the data path by default
	/// \param dirPath directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \param bRecursive set to true to automatically create nested dirs as required
	/// \return true if directory was created successfully
	static bool createDirectory(const std::string& dirPath, bool bRelativeToData = true, bool recursive = false);
	
	/// \brief check if a directory at a given path is empty
	/// assumes directory path is relative to the data path by default
	/// \param dirPath directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \return true if the directory is empty aka contains no files or directories
	static bool isDirectoryEmpty(const std::string& dirPath, bool bRelativeToData = true );
	
	/// \brief check if a directory exists at a given path
	/// assumes directory path is relative to the data path by default
	/// \param dirPath directory path
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \return true if the directory exists
	static bool doesDirectoryExist(const std::string& dirPath, bool bRelativeToData = true);
	
	
	/// \brief remove a directory at a given path
	/// \param deleteIfNotEmpty set to true if you want to recursively delete the directory *and* it's contents
	/// \param bRelativeToData set to false if you are working with paths that are *not* in the data folder
	/// \return true if the path was removed successfully
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
