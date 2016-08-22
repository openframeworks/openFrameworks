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
///
/// \brief A buffer of data which can be accessed as simple bytes or text.
///
class ofBuffer{
	
public:
	ofBuffer();
	
	/// \brief Create a buffer and set its contents from a raw byte pointer.
	///
	/// \param buffer pointer to the raw byte buffer to copy data from
	/// \param _size the number of bytes to read
	/// \warning buffer *must* not be NULL
	/// \warning _size *must* be <= the number of bytes allocated in _buffer
    ofBuffer(const char * buffer, std::size_t size);
	
	/// \brief Create a buffer and set its contents from an input stream.
	///
	/// \param ioBlockSize the number of bytes to read from the stream in chunks
	ofBuffer(istream & stream, size_t ioBlockSize = 1024);

	/// \brief Set the contents of the buffer from a raw byte pointer.
	///
	/// \warning buffer *must* not be NULL
	/// \warning _size *must* be <= the number of bytes allocated in _buffer
	/// \param buffer pointer to the raw byte buffer to copy data from
	/// \param _size the number of bytes to read
	void set(const char * _buffer, std::size_t _size);
	
	/// \brief Set contents of the buffer from a string.
	///
	/// \param text string to copy data from
	void set(const string & text);
	
	/// \brief Set contents of the buffer from an input stream.
	///
	/// \param stream input stream to copy data from
	/// \param ioBlockSize the number of bytes to read from the stream in chunks
	bool set(istream & stream, size_t ioBlockSize = 1024);
	
	/// \brief Set all bytes in the buffer to a given value.
	///
	/// \param mem byte value to set
	void setall(char mem);
	
	/// \brief Append bytes to the end of buffer from a string.
	///
	/// \param _buffer string to copy bytes from
	void append(const string& _buffer);
	
	/// \brief Append bytes to the end of the buffer from a raw byte pointer.
	///
	/// \warning buffer *must* not be NULL
	/// \warning _size *must* be <= the number of bytes allocated in _buffer
	/// \param buffer pointer to the raw byte buffer to copy data from
	/// \param _size the number of bytes to read
	void append(const char * _buffer, std::size_t _size);
	
	/// \brief Request that the buffer capacity be at least enough to contain a
	/// specified number of bytes.
	///
	/// \param size number of bytes to reserve space for
	void reserve(size_t size);

	/// \brief Write contents of the buffer to an output stream.
	bool writeTo(ostream & stream) const;

	/// \brief Remove all bytes from the buffer, leaving a size of 0.
	void clear();

	/// \brief Request that the buffer capacity be at least enough to contain a
	/// specified number of bytes.
	///
	/// \param _size number of bytes to reserve space for
	void allocate(std::size_t _size);
	
	/// \brief Resize the buffer to contain a specified number of bytes.
	///
	/// If _size is < the current buffer size, the contents are reduced to _size
	/// bytes & remaining bytes are removed. If _size is > the current buffer
	/// size, the buffer's size is increased to _size_ bytes.
	///
	/// \param _size number of bytes to resize the buffer to
	void resize(std::size_t _size);

	/// \brief Access the buffer's contents using a raw byte pointer.
	///
	/// \warning Do not access bytes at indices beyond size()!
	/// \returns pointer to internal raw bytes
	char * getData();
	
	/// \brief Access the buffer's contents using a const raw byte pointer.
	///
	/// \warning Do not access bytes at indices beyond size()!
	/// \returns const pointer to internal raw bytes
	const char * getData() const;
	OF_DEPRECATED_MSG("Use getData instead",char * getBinaryBuffer());
	OF_DEPRECATED_MSG("Use getData instead",const char * getBinaryBuffer() const);

	/// \brief Get the contents of the buffer as a string.
	///
	/// \returns buffer contents as a string
	string getText() const;
	
	/// \brief Use buffer as a string via cast.
	///
	/// \returns buffer contents as a string
	operator string() const;
	
	/// \brief set contents of the buffer from a string
	ofBuffer & operator=(const string & text);

	/// \brief Check the buffer's size.
	///
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

	/// \class Line
	///
	/// \brief A line of text in the buffer.
	///
	struct Line: public std::iterator<std::forward_iterator_tag,Line>{
		Line(vector<char>::iterator _begin, vector<char>::iterator _end);
		const string & operator*() const;
		const string * operator->() const;
		const string & asString() const;
		
		/// \brief Increment to the next line.
		Line& operator++();
		
		/// \brief Increment to a number of lines.
		Line operator++(int);
		
		bool operator!=(Line const& rhs) const;
		bool operator==(Line const& rhs) const;
		
		 /// \brief Is this line empty? (aka an empty string "")
		bool empty() const;

	private:
		string line;
		vector<char>::iterator _current, _begin, _end;
	};

	/// \class Lines
	///
	/// \brief A series of text lines in the buffer.
	///
	struct Lines{
		Lines(vector<char>::iterator begin, vector<char>::iterator end);
		
		/// \brief Get the first line in the buffer.
		Line begin();
		
		/// \brief Get the last line in the buffer.
		Line end();

	private:
		vector<char>::iterator _begin, _end;
	};

	/// \brief Access the contents of the buffer as a series of text lines.
	///
	/// If the buffer loads a text file with lines separated by an endline
	/// char '\n', you can access each line individually using Line structs.
	///
	/// \returns buffer text lines
	Lines getLines();

private:
	vector<char> 	buffer;
	Line			currentLine;
};

//--------------------------------------------------
/// \brief Read the contents of a file at path into a buffer.
///
/// Opens as a text file by default.
///
/// \param path file to open
/// \param binary set to false if you are reading a text file & want lines
/// split at endline characters automatically
ofBuffer ofBufferFromFile(const std::filesystem::path & path, bool binary=true);

//--------------------------------------------------
/// \brief Write the contents of a buffer to a file at path.
///
/// Saves as a text file by default.
///
/// \param path file to open
/// \param buffer data source to write from
/// \param binary set to false if you are writing a text file & want lines
/// split at endline characters automatically
bool ofBufferToFile(const std::filesystem::path & path, const ofBuffer& buffer, bool binary=true);

//--------------------------------------------------
/// \class ofFilePath
///
/// \brief Static class for working with file path strings.
///
class ofFilePath{
public:
	
	/// \brief Get the extension of a filename, ie. "duck.jpg" -> "jpg".
	///
	/// \param filename file path
	/// \returns filename extension only
    static string getFileExt(const std::filesystem::path& filename);
	
	/// \brief Remove extension from a filename, ie. "duck.jpg" ->"duck".
	///
	/// \param filename file path
	/// \returns filename without extension
    static string removeExt(const std::filesystem::path& filename);
	
	/// \brief Prepend path with a slash, ie. "images" -> "/images".
	///
	/// \param path file or directory path
	/// \returns slah + path
    static string addLeadingSlash(const std::filesystem::path& path);
	
	/// \brief Append path with a slash, ie. "images" -> "images/".
	///
	/// \param path directory path
	/// \returns path + slash
    static string addTrailingSlash(const std::filesystem::path& path);
	
	/// \brief Remove a path's trailing slash (if found),
	/// ie. "images/" -> "images".
	///
	/// \param path directory path
	/// \returns path minus trailing slash
    static string removeTrailingSlash(const std::filesystem::path& path);
	
	/// \brief Cleaned up a directory path by adding a trailing slash if needed.
	///
	/// For Windows-style path strings using "\", a "\" will be added.
	/// For Unix-style path strings using "/", a "/" will be added.
	///
	/// \param path directory path
	/// \returns cleaned path + trailing slash (if needed)
    static string getPathForDirectory(const std::filesystem::path& path);
	
	/// \brief Get the absolute, full path for a given path,
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images".
	///
	/// \param path file or directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder and want the direct path without relative
	/// "../../"
	/// \returns absolute path
    static string getAbsolutePath(const std::filesystem::path& path, bool bRelativeToData = true);

	/// \brief Check if a path is an absolute (aka a full path),
	/// ie. "images" -> false,
	/// "/Users/mickey/of/apps/myApps/Donald/bin/data/images" -> true.
	///
	/// \param path file or directory path
	/// \returns true if the path is an absolute path
    static bool isAbsolute(const std::filesystem::path& path);
	
	/// \brief Get the filename of a given path by stripping the parent
	/// directories ie. "images/duck.jpg" -> "duck.jpg", assumes the path is in
	/// the data folder.
	///
	/// \param filePath file path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder and want the direct path without relative
	/// "../../"
	/// \returns filename
    static string getFileName(const std::filesystem::path& filePath, bool bRelativeToData = true);
	
	/// \brief Get a file path without its last component,
	/// ie. "images/duck.jpg" -> "images" and
	/// "images/some/folder" -> "images/some"
	///
	/// \param filePath file path
	/// \returns basename
    static string getBaseName(const std::filesystem::path& filePath);

	/// \brief Get the enclosing parent directory of a path,
	/// ie. "images/duck.jpg" -> "images", assumes the path is in the data
	/// directory.
	///
	/// \param filePath file path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder and want the direct path without relative
	/// "../../"
	///\returns enclosing directory
    static string getEnclosingDirectory(const std::filesystem::path& filePath, bool bRelativeToData = true);
	
	/// \brief Create the enclosing parent directory of a path, ie.
	/// "images" is the enclosing directory of "duck.jpg" = "images/duck.jpg".
	///
	/// Assumes the path is in the data folder & automatically creates nested
	/// directories as required.
	///
	/// \param bRecursive set to false to override automatically nested
	/// directory creation
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder and want the direct path without relative
	/// "../../"
	/// \returns true if the enclosing directory was created
    static bool createEnclosingDirectory(const std::filesystem::path& filePath, bool bRelativeToData = true, bool bRecursive = true);
	
	/// \brief Get the full path to the app's current working directory.
	///
	/// This may be the app's parent directory or the location the app was
	/// launched from (aka on the commandline).
	///
	/// \warning This location *may* change if you or a library calls the cd()
	/// std C function.
	/// \returns current working directory
	static string getCurrentWorkingDirectory();
	
	/// \brief Create a single path by joining path1 & path2 using a slash,
	/// ie. "/hello/world" + "foo/bar" -> "/hello/world/foo/bar".
	///
	/// \param path1 left half of the path to join
	/// \param path2 right half of the path to join
	/// \returns joined path
    static string join(const std::filesystem::path& path1, const std::filesystem::path& path2);
	
	/// \brief Get the full path to the application's executable file.
	///
	/// Mac: the binary within the application's .app bundle Contents/MacOS dir
	/// Windows: the .exe
	/// Linux: the binary file itself
	///
	/// \returns current executable path
	static string getCurrentExePath();
	
	/// \brief Get the full path to the application's parent directory.
	///
	/// Windows & Linux: the application's parent directory
	/// Mac: the Contents/MacOS folder within the application's .app bundle
	///
	/// \returns current executable directory
	static string getCurrentExeDir();

	/// \brief Get the absolute path to the user's home directory.
	///
	/// Mac OSX: /Users/<username>
	/// Windows: <root>\Users\<username>
	/// Linux: /home/<username>
	///
	/// \returns home directory path
	static string getUserHomeDir();

	/// \brief Make one path relative to another,
	/// ie. the relative path of "images/felines/lions" to
	/// "images/felines/tigers" is "../tigers".
	///
	/// \param from starting path
	/// \param to destination path
	/// \returns relative path
    static string makeRelative(const std::filesystem::path & from, const std::filesystem::path & to);
};

/// \class ofFile
///
/// \brief path to a file or directory
///
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

	/// \brief Create an ofFile instance.
	///
	/// Does not refer to a specific file until you either open a file or create
	/// a file or directory path.
	ofFile();
	
	/// \brief Create a new ofFile instance and attempt to open the path as a
	/// file.
	///
	/// Opens as a binary file with read only access by default.
	///
	/// \param path file path
	/// \param mode file access mode depending on how you plan to use the file
	/// (read only, read write, etc)
	/// \param binary set to false if you are working with a text file & want
	/// lines split at endline characters automatically
	ofFile(const std::filesystem::path & path, Mode mode=ReadOnly, bool binary=true);
	
	/// \brief Create a new file path using the same path & settings of another
	/// file.
	///
	/// \param mom ofFile instance source
	ofFile(const ofFile & mom);
	
	/// \brief Copy the path and settings of an ofFile into this instance.
	///
	/// \param mom ofFile instance source
	ofFile & operator= (const ofFile & mom);
	
	~ofFile();

	/// \brief Open the path as a file.
	///
	/// Opens as a text file with read only access by default.
	///
	/// \param path file path
	/// \param mode file access mode depending on how you plan to use the file
	/// (read only, read write, etc)
	/// \param binary set to false if you are reading a text file & want lines
	/// split at endline characters automatically
	/// \returns true if the path was opened
	bool open(const std::filesystem::path & path, Mode mode=ReadOnly, bool binary=true);
	
	/// \brief Reopen the current file path with a different access mode.
	///
	/// \param mode file access mode depending on how you plan to use the file
	/// (read only, read write, etc)
	/// \param binary set to false if you are reading a text file & want lines
	/// split at endline characters automatically
	/// \returns true if the file was reopened with the new access mode(s).
	bool changeMode(Mode mode, bool binary=true);
	
	/// \brief Close a currently open file.
	void close();
	
	/// \brief Create a file at the current path.
	///
	/// Creates as a write only binary file by default.
	///
	/// \returns true if the file was created
	bool create();
	
	/// \brief Create a file at a given path.
	///
	/// Creates as a write only binary file by default.
	///
	/// \param path file path
	/// \returns true if the file was created
	bool create(const std::filesystem::path & path);
	
	/// \brief Check if a file exists at the current path.
	///
	/// \returns true if the file exists
	bool exists() const;
	
	/// \brief Get the current path.
	///
	/// \returns current path
	string path() const;
	
	/// \brief Get the current path without its extension,
	/// ie. "duck.jpg" ->"duck".
	///
	/// \returns current path file extension
	string getExtension() const;
	
	/// \brief Get the filename of the current path by stripping the parent
	/// directories, ie. "images/duck.jpg"  -> "duck.jpg".
	///
	/// \returns current path filename
	string getFileName() const;
	
	/// \biref Get the current path without its last component,
	/// ie. "images/duck.jpg" -> "images" and
	/// "images/some/folder" -> "images/some".
	///
	/// \returns current path basename
	string getBaseName() const;
	
	/// \brief Get the enclosing parent directory of a path,
	/// ie. "images/duck.jpg" -> "images", assumes the path is in the data
	/// directory.
	///
	/// \returns current path's enclosing directory
	string getEnclosingDirectory() const;
	
	/// \biref Get the absolute, full path of the file,
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images".
	///
	/// \returns current path as an absolute path
	string getAbsolutePath() const;

	/// \brief Check if the current path is readable.
	///
	/// \returns true if readable
	bool canRead() const;
	
	/// \brief Check if the current path is writable.
	///
	/// \returns true if writable
	bool canWrite() const;
	
	/// \brief Check if the current path is executable.
	///
	/// \returns true if executable
	bool canExecute() const;

	/// \brief Check if the current path is a file and not a directory.
	///
	/// \returns true if a file
	bool isFile() const;
	
	/// \brief Check if the current path is a system link to another file or
	/// directory.
	///
	/// \returns true if a system link
	bool isLink() const;
	
	/// \brief Check if the current path is a directory and not a file.
	///
	/// \returns true if a directory
	bool isDirectory() const;
	
	/// \brief Check if the current path is a device file.
	///
	/// Works on Mac & Linux which can represent devices as files, however
	/// always returns false on Windows.
	///
	/// \returns true if a device file
	bool isDevice() const;
	
	/// \brief Check if the current path is hidden.
	///
	/// Works on Mac & Linux which denote hidden files by prepending a period
	/// to the filename -> ".hello", however always returns false on Windows.
	///
	/// \returns true if hidden
	bool isHidden() const;

	/// \brief Set the writable flag of the current path.
	void setWriteable(bool writeable=true);

	OF_DEPRECATED_MSG("Use ofFile::setWriteable(!flag).", void setReadOnly(bool flag));
	
	/// \brief Set the readable flag of the current path.
	void setReadable(bool readable=true);
	
	/// \brief Set the executable flag of the current path.
	void setExecutable(bool executable=true);
	
	/// \brief Copy the current file or directory path to a new path.
	///
	/// Copies relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data
	/// folder.
	///
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or
	/// directory at the new path
	/// \returns true if the copy was successful
	bool copyTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false) const;
	
	/// \brief Move the current file or directory path to a new path.
	///
	/// Moves relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data
	/// folder.
	///
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or
	/// directory at the new path
	/// \returns true if the copy was successful
	bool moveTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief Rename the current file or directory path to a new path.
	///
	/// Renames relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data
	/// folder.
	///
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or
	/// directory at the new path
	/// \returns true if the copy was successful
	bool renameTo(const std::string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief Removes the file or directory at the current path.
	///
	/// Does not remove non-empty directories by default.
	///
	/// \warning Be careful! This deletes a file or folder. :)
	/// \param recursive set to true to remove a non-empty directory and its
	/// contents
	/// \returns true if the path was removed successfully
	bool remove(bool recursive=false);

	/// \brief get the size of the file at the current file path
	///
	/// \returns size in bytes
	uint64_t getSize() const;

	// this allows to compare files by their paths, also provides sorting
	// and use as key in stl containers
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
	
	/// \brief Read the contents of a file at the current path into a buffer.
	///
	/// \returns buffer with file contents
	ofBuffer readToBuffer();
	
	/// \brief Write the contents of a buffer into a file at the current path.
	///
	/// \param buffer source byte buffer
	/// \returns true if buffer's contents written successfully
	bool writeFromBuffer(const ofBuffer & buffer);
	
	/// \brief Read the entire contents of the currently opened file into an
	/// output stream.
	///
	/// This is basically an easy to use equivalent to rdbuf():
	/// ie. ofLogNotice() << file.getFileBuffer();
	///     write_file << file.getFileBuffer();
	///
	/// \return output stream
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

	/// \brief Copy source path to destination path.
	///
	/// Copies relative to the data path & does *not* overwrite by default
	/// assumes the source & destination path is in the data directory.
	///
	/// \param pathSrc source file or directory path
	/// \param pathDst destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data directory
	/// \param overwrite set to true if you want to overwrite the file or
	/// directory at the new path
	/// \returns true if the copy was successful
	static bool copyFromTo(const std::string& pathSrc, const std::string& pathDst, bool bRelativeToData = true,  bool overwrite = false);

	/// \brief Move source path to destination path.
	///
	/// Moves relative to the data path & does *not* overwrite by default
	/// assumes the source & destination path is in the data directory.
	///
	/// \param pathSrc source file or directory path
	/// \param pathDst destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or
	/// directory at the new path
	/// \warning be careful with slashes here, appending a slash when moving a
	/// folder may cause mad headaches in OSX
	/// \returns true if the move was successful
	static bool moveFromTo(const std::string& pathSrc, const std::string& pathDst, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief Check if a file or directory exists at a given path.
	///
	/// \param fPath file path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder and want the direct path without relative
	/// "../../"
	/// \returns true if a file or directory exists
	static bool doesFileExist(const std::string& fPath,  bool bRelativeToData = true);
	
	/// \brief Remove a file or directory at a given path.
	///
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder and want the direct path without relative
	/// "../../"
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
///
/// \brief Path to a directory. Can be used to query file and directory
/// contents.
///
class ofDirectory{

public:

	/// \brief Create an ofDirectory instance
	///
	/// Does not refer to a specific directory until you either open or create
	/// a directory path.
	ofDirectory();
	
	/// \brief Create an ofDirectory instance and attempt to open the path.
	///
	/// \param path directory path
	ofDirectory(const std::filesystem::path & path);

	/// \brief Open a directory path, clears the current file list.
	///
	/// \param path directory path
	void open(const std::filesystem::path & path);
	
	/// \brief Close the currently open path.
	void close();
	
	/// \brief Create a directory at the current path.
	///
	/// \param bRecursive set to true to automatically create nested directories
	/// as required
	bool create(bool recursive = false);

	/// \brief Check if a directory exists at the current path.
	///
	/// \returns true if exists
	bool exists() const;
	
	/// \brief Get the current path.
	///
	/// \returns current path
	string path() const;
	
	/// \brief Get the absolute, full path of the directory,
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images".
	///
	/// \return current path as an absolute path
	string getAbsolutePath() const;

	/// \brief Check if the current path is readable.
	///
	/// \returns true if readable
	bool canRead() const;
	
	/// \brief Check if the current path is writeable.
	///
	/// \returns true if writable
	bool canWrite() const;
	
	/// \brief Check if the current path is executable.
	///
	/// \returns true if executable
	bool canExecute() const;
	
	/// \brief Check if the current path is indeed a directory and not a file.
	///
	/// \returns true if a directory
	bool isDirectory() const;
	
	/// \brief Check if the current path is hidden.
	///
	/// Works on Mac & Linux which denote hidden directories by prepending
	/// a period -> ".hello", however always returns false on Windows.
	///
	/// \returns true if hidden
	bool isHidden() const;

	/// \brief Set the writable flag of the current path.
	///
	/// \param writable set to true to make path writable
	void setWriteable(bool writeable=true);

	OF_DEPRECATED_MSG("Use ofDirectory::setWriteable(!flag).", void setReadOnly(bool flag));
	
	/// \brief Set the readable flag of the current path.
	///
	/// \param readable set to true to make path readable
	void setReadable(bool readable=true);
	
	/// \brief Set the executable flag of the current path.
	///
	/// \param executable set to true to make path executable
	void setExecutable(bool executable=true);
	
	/// \brief Show hidden files & directories when listing files?
	///
	/// Mac & Linux denote hidden directories by prepending a period
	/// -> ".hello".
	///
	/// \param showHidden set to true to show hidden files
	void setShowHidden(bool showHidden);

	/// \brief Copy the current file or directory path to a new path.
	///
	/// Copies relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data
	/// directory.
	///
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data directory
	/// \param overwrite set to true if you want to overwrite the file or
	/// directory at the new path
	/// \returns true if the copy was successful
	bool copyTo(const string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief Move the current file or directory path to a new path.
	///
	/// Moves relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data
	/// directory.
	///
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data directory
	/// \param overwrite set to true if you want to overwrite the file or
	/// directory at the new path
	/// \returns true if the copy was successful
	bool moveTo(const string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief Rename the current file or directory path to a new path.
	///
	/// Renames relative to the data path & does *not* overwrite by default
	/// does not change the current path & assumes the new path is in the data
	/// directory.
	///
	/// \param path destination file or directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder
	/// \param overwrite set to true if you want to overwrite the file or
	/// directory at the new path
	/// \returns true if the copy was successful
	bool renameTo(const string& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// \brief Removes the file or directory at the current path.
	///
	/// Does not remove non-empty directories by default.
	///
	/// \warning Be careful! This deletes a file or folder. :)
	/// \param recursive set to true to remove a non-empty directory and its
	/// contents
	/// \returns true if the path was removed successfully
	bool remove(bool recursive);

	//-------------------
	// dirList operations
	//-------------------
	
	/// \brief Allow a file extension when listing the contents the current
	/// directory path.
	///
	/// Setting an allowed extension enables a whitelist mode which only lists
	/// extensions which have been explicitly allowed.
	///
	/// \param extension file type extension ie. "jpg", "png", "txt", etc
	void allowExt(const string& extension);
	
	/// \brief Open and read the contents of a directory.
	///
	/// Uses allowed extension whitelist to ignore unwanted file types if
	/// allowExt() has been called.
	///
	/// \param path directory path
	/// \returns number of paths found
	std::size_t listDir(const string& path);
	
	/// \brief Open and read the contents of the current directory.
	///
	/// Uses allowed extension whitelist to ignore unwanted file types if
	/// allowExt() has been called.
	///
	/// \returns number of paths found
	std::size_t listDir();

	/// \returns the current path
	string getOriginalDirectory() const;
	
	/// \brief Get the filename at a given position in the directory contents
	/// list, ie. "duck.jpg".
	///
	/// \warning Call listDir() before using this function or the directory
	/// contents list will be empty.
	/// \throws Throws an out of bounds exception if position >= the number of
	/// listed directory contents.
	/// \param position array index in the directory contents list
	/// \returns file or directory name
	string getName(std::size_t position) const;
	
	/// \brief Get the full path of the file or directory at a given position in
	/// the directory contents list.
	///
	/// \warning Call listDir() before using this function or the directory
	/// contents list will be empty.
	/// \throws Throws an out of bounds exception if position >= the number of
	/// listed directory contents.
	/// \param position array index in the directory contents list
	/// \returns file or directory name including the current path
	string getPath(std::size_t position) const;
	
	/// \brief Open an ofFile instance using the path a given position in the
	/// directory contents list.
	///
	/// Opens as a binary file with readonly access by default.
	///
	/// \warning Call listDir() before using this function or the directory
	/// contents list will be empty.
	/// \throw Throws an out of bounds exception if position >= the number of
	/// listed directory contents.
	/// \param position array index in the directory contents list
	/// \param mode file access mode depending on how you plan to use the file
	/// (read only, read write, etc)
	/// \param binary set to false if you are working with a text file & want
	/// lines split at endline characters automatically
	/// \returns ofFile instance
	ofFile getFile(std::size_t position, ofFile::Mode mode=ofFile::Reference, bool binary=true) const;
	
	/// \brief Get files and directories in the directory contents list.
	///
	/// Directory contents are automatically listed.
	///
	/// \returns vector of files in the directory
	const vector<ofFile> & getFiles() const;

	/// \brief Access directory contents via th array operator.
	///
	/// \warning Call listDir() before using this function or the directory
	/// contents list will be empty.
	/// \throw Throws an out of bounds exception if position >= the number of
	/// listed directory contents.
	/// \param position array index in the directory contents list
	/// \returns opened ofFile instance
	ofFile operator[](std::size_t position) const;

	/// \brief Check whether hidden files & directories are included when
	/// listing files.
	///
	/// Mac & Linux denote hidden directories by prepending a period
	/// -> ".hello".
	///
	/// \returns true if hidden files are shown
	bool getShowHidden() const;

	/// \brief Closes the directory.
	///
	/// This is for backwards compatibility with ofxDirList.
	void reset();
	
	/// \brief Sort the directory contents list alphabetically.
	///
	/// \warning Call listDir() before using this function or there will be
	/// nothing to sort.
	void sort();
	
	/// \brief Get a sorted ofDirectory instance using the current path.
	///
	/// \returns sorted ofDirectory instance
	ofDirectory getSorted();

	/// \brief Get the number of paths in the current directory list.
	///
	/// \warning Call listDir() before using this function or it will return 0
	/// since the directory list will be empty.
	/// \returns number of paths
	std::size_t size() const;

	OF_DEPRECATED_MSG("Use size() instead.", int numFiles());

	// this allows to compare directories by their paths, also provides sorting
	// and use as key in stl containers
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

	/// \brief Create a directory at a given path.
	///
	/// Creates relative to the data path by default.
	///
	/// \param dirPath directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data directory
	/// \param bRecursive set to true to automatically create nested directories
	/// as required
	/// \returns true if directory was created successfully
	static bool createDirectory(const std::string& dirPath, bool bRelativeToData = true, bool recursive = false);
	
	/// \brief Check if a directory at a given path is empty.
	///
	/// Assumes directory path is relative to the data path by default.
	///
	/// \param dirPath directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data directory
	/// \returns true if the directory is empty aka contains no files or
	/// directories
	static bool isDirectoryEmpty(const std::string& dirPath, bool bRelativeToData = true );
	
	/// \brief Check if a directory exists at a given path.
	///
	/// Assumes directory path is relative to the data path by default.
	///
	/// \param dirPath directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data directory
	/// \returns true if the directory exists
	static bool doesDirectoryExist(const std::string& dirPath, bool bRelativeToData = true);
	
	
	/// \brief remove a directory at a given path
	///
	/// \param deleteIfNotEmpty set to true if you want to recursively delete
	/// the directory *and* its contents
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data directory
	/// \returns true if the path was removed successfully
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
