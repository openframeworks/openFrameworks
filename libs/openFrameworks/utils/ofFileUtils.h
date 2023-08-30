#pragma once

// FIXME: constants deprecated only
#include "ofConstants.h"
#include <fstream>

//----------------------------------------------------------
// ofBuffer
//----------------------------------------------------------

/// \class ofBuffer
///
/// A buffer of data which can be accessed as simple bytes or text.
///
class ofBuffer{
	
public:
	ofBuffer();
	
	/// Create a buffer and set its contents from a raw byte pointer.
	///
	/// \param buffer pointer to the raw byte buffer to copy data from
	/// \param size the number of bytes to read
	/// \warning buffer *must* not be NULL
	/// \warning size *must* be <= the number of bytes allocated in buffer
    ofBuffer(const char * buffer, std::size_t size);
	
	/// Create a buffer and set its contents from an input stream.
	///
	/// \param ioBlockSize the number of bytes to read from the stream in chunks
	ofBuffer(std::istream & stream, std::size_t ioBlockSize = 1024);

	/// Set the contents of the buffer from a raw byte pointer.
	///
	/// \warning buffer *must* not be NULL
	/// \warning size *must* be <= the number of bytes allocated in buffer
	/// \param buffer pointer to the raw byte buffer to copy data from
	/// \param size the number of bytes to read
	void set(const char * buffer, std::size_t size);
	
	/// Set contents of the buffer from a string.
	///
	/// \param text string to copy data from
	void set(const std::string & text);
	
	/// Set contents of the buffer from an input stream.
	///
	/// \param stream input stream to copy data from
	/// \param ioBlockSize the number of bytes to read from the stream in chunks
	bool set(std::istream & stream, std::size_t ioBlockSize = 1024);
	
	/// Set all bytes in the buffer to a given value.
	///
	/// \param mem byte value to set
	void setall(char mem);
	
	/// Append bytes to the end of buffer from a string.
	///
	/// \param buffer string to copy bytes from
	void append(const std::string& buffer);
	
	/// Append bytes to the end of the buffer from a raw byte pointer.
	///
	/// \warning buffer *must* not be NULL
	/// \warning size *must* be <= the number of bytes allocated in buffer
	/// \param buffer pointer to the raw byte buffer to copy data from
	/// \param size the number of bytes to read
	void append(const char * buffer, std::size_t size);
	
	/// Request that the buffer capacity be at least enough to contain a
	/// specified number of bytes.
	///
	/// \param size number of bytes to reserve space for
	void reserve(std::size_t size);

	/// Write contents of the buffer to an output stream.
	bool writeTo(std::ostream & stream) const;

	/// Remove all bytes from the buffer, leaving a size of 0.
	void clear();

	/// Request that the buffer capacity be at least enough to contain a
	/// specified number of bytes.
	///
	/// \param size number of bytes to reserve space for
	void allocate(std::size_t size);
	
	/// Resize the buffer to contain a specified number of bytes.
	///
	/// If size is < the current buffer size, the contents are reduced to size
	/// bytes & remaining bytes are removed. If size is > the current buffer
	/// size, the buffer's size is increased to size_ bytes.
	///
	/// \param size number of bytes to resize the buffer to
	void resize(std::size_t size);

	/// Access the buffer's contents using a raw byte pointer.
	///
	/// \warning Do not access bytes at indices beyond size()!
	/// \returns pointer to internal raw bytes
	char * getData();
	
	/// access the buffer's contents using a const raw byte pointer.
	///
	/// \warning Do not access bytes at indices beyond size()!
	/// \returns const pointer to internal raw bytes
	const char * getData() const;
	OF_DEPRECATED_MSG("Use getData instead",char * getBinaryBuffer());
	OF_DEPRECATED_MSG("Use getData instead",const char * getBinaryBuffer() const);

	/// get the contents of the buffer as a string.
	///
	/// \returns buffer contents as a string
	std::string getText() const;
	
	/// Use buffer as a string via cast.
	///
	/// \returns buffer contents as a string
	operator std::string() const;
	
	/// set contents of the buffer from a string
	ofBuffer & operator=(const std::string & text);

	/// Check the buffer's size.
	///
	/// \returns the size of the buffer's content in bytes
	std::size_t size() const;

	OF_DEPRECATED_MSG("use a lines iterator instead",std::string getNextLine());
	OF_DEPRECATED_MSG("use a lines iterator instead",std::string getFirstLine());
	OF_DEPRECATED_MSG("use a lines iterator instead",bool isLastLine());
	OF_DEPRECATED_MSG("use a lines iterator instead",void resetLineReader());
	
	friend std::ostream & operator<<(std::ostream & ostr, const ofBuffer & buf);
	friend std::istream & operator>>(std::istream & istr, ofBuffer & buf);

	std::vector<char>::iterator begin();
	std::vector<char>::iterator end();
	std::vector<char>::const_iterator begin() const;
	std::vector<char>::const_iterator end() const;
	std::vector<char>::reverse_iterator rbegin();
	std::vector<char>::reverse_iterator rend();
	std::vector<char>::const_reverse_iterator rbegin() const;
	std::vector<char>::const_reverse_iterator rend() const;

	/// A line of text in the buffer.
	///

	struct Line {
		Line(std::vector<char>::iterator _begin, std::vector<char>::iterator _end);
		const std::string & operator*() const;
		const std::string * operator->() const;
		const std::string & asString() const;
		
		using value_type = std::string;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using pointer = const value_type*;
		using reference = const value_type&;
		
		/// Increment to the next line.
		Line& operator++();
		
		/// Increment to a number of lines.
		Line operator++(int);
		
		bool operator!=(Line const& rhs) const;
		bool operator==(Line const& rhs) const;
		
		 /// Is this line empty? (aka an empty string "")
		bool empty() const;

	private:
		std::string line;
		std::vector<char>::iterator _current, _begin, _end;
	};

	/// A line of text in the buffer.
	///
	struct RLine {
		RLine(std::vector<char>::reverse_iterator _begin, std::vector<char>::reverse_iterator _end);
		const std::string & operator*() const;
		const std::string * operator->() const;
		const std::string & asString() const;
		
		using value_type = std::string;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using pointer = const value_type*;
		using reference = const value_type&;

		/// Increment to the next line.
		RLine& operator++();

		/// Increment to a number of lines.
		RLine operator++(int);

		bool operator!=(RLine const& rhs) const;
		bool operator==(RLine const& rhs) const;

		 /// Is this line empty? (aka an empty string "")
		bool empty() const;

	private:
		std::string line;
		std::vector<char>::reverse_iterator _current, _rbegin, _rend;
	};

	/// A series of text lines in the buffer.
	///
	struct Lines{
		Lines(std::vector<char>::iterator begin, std::vector<char>::iterator end);
		
		/// Get the first line in the buffer.
		Line begin();
		
		/// Get the last line in the buffer.
		Line end();

		RLine rbegin();
		RLine rend();

	private:
		std::vector<char>::iterator _begin, _end;
	};


	/// A series of text lines in the buffer.
	///
	struct RLines{
		RLines(std::vector<char>::reverse_iterator rbegin, std::vector<char>::reverse_iterator rend);

		/// Get the first line in the buffer.
		RLine begin();

		/// Get the last line in the buffer.
		RLine end();

	private:
		std::vector<char>::reverse_iterator _rbegin, _rend;
	};

	/// Access the contents of the buffer as a series of text lines.
	///
	/// If the buffer loads a text file with lines separated by an endline
	/// char '\n', you can access each line individually using Line structs.
	///
	/// \returns buffer text lines
	Lines getLines();

	/// Access the contents of the buffer as a series of text lines in reverse
	/// order
	///
	/// If the buffer loads a text file with lines separated by an endline
	/// char '\n' or '\r\n', you can access each line individually using Line structs.
	///
	/// \returns buffer text lines
	RLines getReverseLines();

private:
	std::vector<char> 	buffer;
	Line			currentLine;
};

//--------------------------------------------------
/// Read the contents of a file at path into a buffer.
///
/// Opens as a text file by default.
///
/// \param path file to open
/// \param binary set to false if you are reading a text file & want lines
/// split at endline characters automatically
ofBuffer ofBufferFromFile(const of::filesystem::path & path, bool binary=true);

//--------------------------------------------------
/// Write the contents of a buffer to a file at path.
///
/// Saves as a text file by default.
///
/// \param path file to open
/// \param buffer data source to write from
/// \param binary set to false if you are writing a text file & want lines
/// split at endline characters automatically
bool ofBufferToFile(const of::filesystem::path & path, const ofBuffer& buffer, bool binary=true);

//--------------------------------------------------
/// \class ofFilePath
///
/// Static class for working with file path strings.
///
class ofFilePath{
public:
	
	/// Get the extension of a filename, ie. "duck.jpg" -> "jpg".
	///
	/// \param filename file path
	/// \returns filename extension only
    static std::string getFileExt(const of::filesystem::path& filename);
	
	/// Remove extension from a filename, ie. "duck.jpg" ->"duck".
	///
	/// \param filename file path
	/// \returns filename without extension
//	MARK: - near future
//	static of::filesystem::path removeExt(const of::filesystem::path& filename);
	static std::string removeExt(const of::filesystem::path& filename);

	/// Prepend path with a slash, ie. "images" -> "/images".
	///
	/// \param path file or directory path
	/// \returns slah + path
    static std::string addLeadingSlash(const of::filesystem::path& path);
	
	/// Append path with a slash, ie. "images" -> "images/".
	///
	/// \param path directory path
	/// \returns path + slash
//	MARK: - near future
//	static of::filesystem::path addTrailingSlash(const of::filesystem::path& path);
	static std::string addTrailingSlash(const of::filesystem::path& path);

	/// Remove a path's trailing slash (if found),
	/// ie. "images/" -> "images".
	///
	/// \param path directory path
	/// \returns path minus trailing slash
    static std::string removeTrailingSlash(const of::filesystem::path& path);
	
	/// Cleaned up a directory path by adding a trailing slash if needed.
	///
	/// For Windows-style path strings using "\", a "\" will be added.
	/// For Unix-style path strings using "/", a "/" will be added.
	///
	/// \param path directory path
	/// \returns cleaned path + trailing slash (if needed)
    static std::string getPathForDirectory(const of::filesystem::path& path);
	
	/// Get the absolute, full path for a given path,
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images".
	///
	/// \param path file or directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder and want the direct path without relative
	/// "../../"
	/// \returns absolute path
	// MARK - near future
	// static of::filesystem::path getAbsolutePath(const of::filesystem::path& path, bool bRelativeToData = true);
	static std::string getAbsolutePath(const of::filesystem::path& path, bool bRelativeToData = true);

	/// Check if a path is an absolute (aka a full path),
	/// ie. "images" -> false,
	/// "/Users/mickey/of/apps/myApps/Donald/bin/data/images" -> true.
	///
	/// \param path file or directory path
	/// \returns true if the path is an absolute path
    static bool isAbsolute(const of::filesystem::path& path);
	
	/// Get the filename of a given path by stripping the parent
	/// directories ie. "images/duck.jpg" -> "duck.jpg", assumes the path is in
	/// the data folder.
	///
	/// \param filePath file path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder and want the direct path without relative
	/// "../../"
	/// \returns filename
	///
	// FIXME: Deprecate / Remove this.
    static std::string getFileName(const of::filesystem::path& filePath, bool bRelativeToData = true);
	
	/// Get a file name without its extension,
	/// ie. "images/duck.jpg" -> "duck" and
	/// "images/some/folder" -> "folder"
	///
	/// \param filePath file path
	/// \returns basename
    static std::string getBaseName(const of::filesystem::path& filePath);

	/// Get the enclosing parent directory of a path,
	/// ie. "images/duck.jpg" -> "images", assumes the path is in the data
	/// directory.
	///
	/// \param filePath file path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder and want the direct path without relative
	/// "../../"
	///\returns enclosing directory
//	MARK: - near future
//	static of::filesystem::path getEnclosingDirectory(const of::filesystem::path& filePath, bool bRelativeToData = true);
	static std::string getEnclosingDirectory(const of::filesystem::path& filePath, bool bRelativeToData = true);
	
	/// Create the enclosing parent directory of a path, ie.
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
    static bool createEnclosingDirectory(const of::filesystem::path& filePath, bool bRelativeToData = true, bool bRecursive = true);
	
	/// Get the full path to the app's current working directory.
	///
	/// This may be the app's parent directory or the location the app was
	/// launched from (aka on the commandline).
	///
	/// \warning This location *may* change if you or a library calls the cd()
	/// std C function.
	/// \returns current working directory
	static std::string getCurrentWorkingDirectory();
	
	/// Create a single path by joining path1 & path2 using a slash,
	/// ie. "/hello/world" + "foo/bar" -> "/hello/world/foo/bar".
	///
	/// \param path1 left half of the path to join
	/// \param path2 right half of the path to join
	/// \returns joined path
//	MARK: - near future
//	static of::filesystem::path join(const of::filesystem::path& path1, const of::filesystem::path& path2);
	static std::string join(const of::filesystem::path& path1, const of::filesystem::path& path2);

	/// Get the full path to the application's executable file.
	///
	/// Mac: the binary within the application's .app bundle Contents/MacOS dir
	/// Windows: the .exe
	/// Linux: the binary file itself
	///
	/// \returns current executable path
	static std::string getCurrentExePath();
	
	/// Get the full path to the application's parent directory.
	///
	/// Windows & Linux: the application's parent directory
	/// Mac: the Contents/MacOS folder within the application's .app bundle
	///
	/// \returns current executable directory
//	MARK: - near future
//	static of::filesystem::path getCurrentExeDir();
	static std::string getCurrentExeDir();

	/// Get the absolute path to the user's home directory.
	///
	/// Mac OSX: /Users/<username>
	/// Windows: <root>\Users\<username>
	/// Linux: /home/<username>
	///
	/// \returns home directory path
	static std::string getUserHomeDir();

	/// Make one path relative to another,
	/// ie. the relative path of "images/felines/lions" to
	/// "images/felines/tigers" is "../tigers".
	///
	/// \param from starting path
	/// \param to destination path
	/// \returns relative path
	//	MARK: - near future
//    static of::filesystem::path makeRelative(const of::filesystem::path & from, const of::filesystem::path & to);
	static std::string makeRelative(const of::filesystem::path & from, const of::filesystem::path & to);
};

/// \class ofFile
///
/// path to a file or directory
///
/// inherits from an fstream so you can read/write using the stream operators
/// once a file path has been opened
class ofFile: public std::fstream{

public:
	
	/// file access mode
	enum Mode{
		Reference,  //<
		ReadOnly,  //< read only from the file, do not write
		WriteOnly, //< write only to the file, do not read
		ReadWrite, //< read from and write to the file
		Append     //< append data to the end of the file, do not overwrite
	};

	/// Create an ofFile instance.
	///
	/// Does not refer to a specific file until you either open a file or create
	/// a file or directory path.
	ofFile();
	
	/// Create a new ofFile instance and attempt to open the path as a
	/// file.
	///
	/// Opens as a binary file with read only access by default.
	///
	/// \param path file path
	/// \param mode file access mode depending on how you plan to use the file
	/// (read only, read write, etc)
	/// \param binary set to false if you are working with a text file & want
	/// lines split at endline characters automatically
	ofFile(const of::filesystem::path & path, Mode mode=ReadOnly, bool binary=true);
	
	/// Create a new file path using the same path & settings of another
	/// file.
	///
	/// \param mom ofFile instance source
	ofFile(const ofFile & mom);
	
	/// Copy the path and settings of an ofFile into this instance.
	///
	/// \param mom ofFile instance source
	ofFile & operator= (const ofFile & mom);
	
	~ofFile();

	/// Open the path as a file.
	///
	/// Opens as a text file with read only access by default.
	///
	/// \param path file path
	/// \param mode file access mode depending on how you plan to use the file
	/// (read only, read write, etc)
	/// \param binary set to false if you are reading a text file & want lines
	/// split at endline characters automatically
	/// \returns true if the path was opened
//	bool open(of::filesystem::path & path, Mode mode=ReadOnly, bool binary=true);
	bool open(const of::filesystem::path & path, Mode mode=ReadOnly, bool binary=true);

	/// Open the path as a file.
	///
	/// Opens as a text file with read only access by default from the current working directory without internally calling ofToDataPath.
	///
	/// \param path file path
	/// \param mode file access mode depending on how you plan to use the file
	/// (read only, read write, etc)
	/// \param binary set to false if you are reading a text file & want lines
	/// split at endline characters automatically
	/// \returns true if the path was opened
	bool openFromCWD(const of::filesystem::path & path, Mode mode=ReadOnly, bool binary=true);
	
	/// Reopen the current file path with a different access mode.
	///
	/// \param mode file access mode depending on how you plan to use the file
	/// (read only, read write, etc)
	/// \param binary set to false if you are reading a text file & want lines
	/// split at endline characters automatically
	/// \returns true if the file was reopened with the new access mode(s).
	bool changeMode(Mode mode, bool binary=true);
	
	/// Close a currently open file.
	void close();
	
	/// Create a file at the current path.
	///
	/// Creates as a write only binary file by default.
	///
	/// \returns true if the file was created
	bool create();
	
	/// Create a file at a given path.
	///
	/// Creates as a write only binary file by default.
	///
	/// \param path file path
	/// \returns true if the file was created
	bool create(const of::filesystem::path & path);
	
	/// Check if a file exists at the current path.
	///
	/// \returns true if the file exists
	bool exists() const;
	
	/// Get the current path.
	///
	/// \returns current path
//	MARK: - near future
//	of::filesystem::path path() const;
	std::string path() const;

	/// Get the current path without its extension,
	/// ie. "duck.jpg" ->"duck".
	///
	/// \returns current path file extension
	std::string getExtension() const;
	
	/// Get the filename of the current path by stripping the parent
	/// directories, ie. "images/duck.jpg"  -> "duck.jpg".
	///
	/// \returns current path filename
	std::string getFileName() const;
	
	/// \biref Get the current path without its last component,
	/// ie. "images/duck.jpg" -> "images" and
	/// "images/some/folder" -> "images/some".
	///
	/// \returns current path basename
	std::string getBaseName() const;
	
	/// Get the enclosing parent directory of a path,
	/// ie. "images/duck.jpg" -> "images", assumes the path is in the data
	/// directory.
	///
	/// \returns current path's enclosing directory
//	MARK: - near future
//	of::filesystem::path getEnclosingDirectory() const;
	std::string getEnclosingDirectory() const;
	
	/// \biref Get the absolute, full path of the file,
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images".
	///
	/// \returns current path as an absolute path
//	MARK: - near future
//	of::filesystem::path getAbsolutePath() const;
	std::string getAbsolutePath() const;

	/// Check if the current path is readable.
	///
	/// \returns true if readable
	bool canRead() const;
	
	/// Check if the current path is writable.
	///
	/// \returns true if writable
	bool canWrite() const;
	
	/// Check if the current path is executable.
	///
	/// \returns true if executable
	bool canExecute() const;

	/// Check if the current path is a file and not a directory.
	///
	/// \returns true if a file
	bool isFile() const;
	
	/// Check if the current path is a system link to another file or
	/// directory.
	///
	/// \returns true if a system link
	bool isLink() const;
	
	/// Check if the current path is a directory and not a file.
	///
	/// \returns true if a directory
	bool isDirectory() const;
	
	/// Check if the current path is a device file.
	///
	/// Works on Mac & Linux which can represent devices as files, however
	/// always returns false on Windows.
	///
	/// \returns true if a device file
	bool isDevice() const;
	
	/// Check if the current path is hidden.
	///
	/// Works on Mac & Linux which denote hidden files by prepending a period
	/// to the filename -> ".hello", however always returns false on Windows.
	///
	/// \returns true if hidden
	bool isHidden() const;

	/// Set the writable flag of the current path.
	void setWriteable(bool writeable=true);

	OF_DEPRECATED_MSG("Use ofFile::setWriteable(!flag).", void setReadOnly(bool flag));
	
	/// Set the readable flag of the current path.
	void setReadable(bool readable=true);
	
	/// Set the executable flag of the current path.
	void setExecutable(bool executable=true);
	
	/// Copy the current file or directory path to a new path.
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
	bool copyTo(const of::filesystem::path& path, bool bRelativeToData = true, bool overwrite = false) const;
	
	/// Move the current file or directory path to a new path.
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
	bool moveTo(const of::filesystem::path& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// Rename the current file or directory path to a new path.
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
	bool renameTo(const of::filesystem::path& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// Removes the file or directory at the current path.
	///
	/// Does not remove non-empty directories by default.
	///
	/// \warning Be careful! This deletes a file or folder. :)
	/// \param recursive set to true to remove a non-empty directory and its
	/// contents
	/// \returns true if the path was removed successfully
	bool remove(bool recursive=false);

	/// get the size of the file at the current file path
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
	
	/// Read the contents of a file at the current path into a buffer.
	///
	/// \returns buffer with file contents
	ofBuffer readToBuffer();
	
	/// Write the contents of a buffer into a file at the current path.
	///
	/// \param buffer source byte buffer
	/// \returns true if buffer's contents written successfully
	bool writeFromBuffer(const ofBuffer & buffer);
	
	/// Read the entire contents of the currently opened file into an
	/// output stream.
	///
	/// This is basically an easy to use equivalent to rdbuf():
	/// ie. ofLogNotice() << file.getFileBuffer();
	///     write_file << file.getFileBuffer();
	///
	/// \return output stream
	std::filebuf * getFileBuffer() const;
	
	operator of::filesystem::path(){
		return myFile;
	}
//
	operator of::filesystem::path() const{
		return myFile;
	}

	//-------
	//static helpers
	//-------

	/// Copy source path to destination path.
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
	static bool copyFromTo(const of::filesystem::path& pathSrc, const of::filesystem::path& pathDst, bool bRelativeToData = true,  bool overwrite = false);

	/// Move source path to destination path.
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
	static bool moveFromTo(const of::filesystem::path& pathSrc, const of::filesystem::path& pathDst, bool bRelativeToData = true, bool overwrite = false);
	
	/// Check if a file or directory exists at a given path.
	///
	/// \param fPath file path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder and want the direct path without relative
	/// "../../"
	/// \returns true if a file or directory exists
	static bool doesFileExist(const of::filesystem::path& fPath,  bool bRelativeToData = true);
	
	/// Remove a file or directory at a given path.
	///
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data folder and want the direct path without relative
	/// "../../"
	/// \returns true if the path was removed successfully
	static bool removeFile(const of::filesystem::path& path, bool bRelativeToData = true);

private:
	bool isWriteMode();
	bool openStream(Mode _mode, bool binary);
	void copyFrom(const ofFile & mom);
	
	of::filesystem::path myFile;
	Mode mode;
	bool binary;
};

/// \class ofDirectory
///
/// Path to a directory. Can be used to query file and directory
/// contents.
///
class ofDirectory{

public:

	/// Create an ofDirectory instance
	///
	/// Does not refer to a specific directory until you either open or create
	/// a directory path.
	ofDirectory();
	
	/// Create an ofDirectory instance and attempt to open the path.
	///
	/// \param path directory path
	ofDirectory(const of::filesystem::path & path);

	/// Open a directory path, clears the current file list.
	///
	/// \param path directory path
	void open(const of::filesystem::path & path);
	
	/// Open a directory path relative to the current working directory without calling ofToDataPath internally, clears the current file list.
	///
	/// \param path directory path
	void openFromCWD(const of::filesystem::path & path);
	
	/// Close the currently open path.
	void close();
	
	/// Create a directory at the current path.
	///
	/// \param bRecursive set to true to automatically create nested directories
	/// as required
	bool create(bool recursive = false);

	/// Check if a directory exists at the current path.
	///
	/// \returns true if exists
	bool exists() const;
	
	/// Get the current path.
	///
	/// \returns current path
//	MARK: - near future
//	of::filesystem::path path() const;
	std::string path() const;

	/// Get the absolute, full path of the directory,
	/// ie. "images" -> "/Users/mickey/of/apps/myApps/Donald/bin/data/images".
	///
	/// \return current path as an absolute path
//	MARK: - near future
//	of::filesystem::path getAbsolutePath() const;
	std::string getAbsolutePath() const;

	/// Check if the current path is readable.
	///
	/// \returns true if readable
	bool canRead() const;
	
	/// Check if the current path is writeable.
	///
	/// \returns true if writable
	bool canWrite() const;
	
	/// Check if the current path is executable.
	///
	/// \returns true if executable
	bool canExecute() const;
	
	/// Check if the current path is indeed a directory and not a file.
	///
	/// \returns true if a directory
	bool isDirectory() const;
	
	/// Check if the current path is hidden.
	///
	/// Works on Mac & Linux which denote hidden directories by prepending
	/// a period -> ".hello", however always returns false on Windows.
	///
	/// \returns true if hidden
	bool isHidden() const;

	/// Set the writable flag of the current path.
	///
	/// \param writable set to true to make path writable
	void setWriteable(bool writeable=true);

	OF_DEPRECATED_MSG("Use ofDirectory::setWriteable(!flag).", void setReadOnly(bool flag));
	
	/// Set the readable flag of the current path.
	///
	/// \param readable set to true to make path readable
	void setReadable(bool readable=true);
	
	/// Set the executable flag of the current path.
	///
	/// \param executable set to true to make path executable
	void setExecutable(bool executable=true);
	
	/// Show hidden files & directories when listing files?
	///
	/// Mac & Linux denote hidden directories by prepending a period
	/// -> ".hello".
	///
	/// \param showHidden set to true to show hidden files
	void setShowHidden(bool showHidden);

	/// Copy the current file or directory path to a new path.
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
	bool copyTo(const of::filesystem::path& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// Move the current file or directory path to a new path.
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
	bool moveTo(const of::filesystem::path& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// Rename the current file or directory path to a new path.
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
	bool renameTo(const of::filesystem::path& path, bool bRelativeToData = true, bool overwrite = false);
	
	/// Removes the file or directory at the current path.
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
	
	/// Allow a file extension when listing the contents the current
	/// directory path.
	///
	/// Setting an allowed extension enables a whitelist mode which only lists
	/// extensions which have been explicitly allowed.
	///
	/// \param extension file type extension ie. "jpg", "png", "txt", etc
	void allowExt(const std::string& extension);
	
	/// Open and read the contents of a directory.
	///
	/// Uses allowed extension whitelist to ignore unwanted file types if
	/// allowExt() has been called.
	///
	/// \param path directory path
	/// \returns number of paths found
	std::size_t listDir(const of::filesystem::path & path);
	
	/// Open and read the contents of the current directory.
	///
	/// Uses allowed extension whitelist to ignore unwanted file types if
	/// allowExt() has been called.
	///
	/// \returns number of paths found
	std::size_t listDir();

	/// \returns the current path
	std::string getOriginalDirectory() const;
	
	/// Get the filename at a given position in the directory contents
	/// list, ie. "duck.jpg".
	///
	/// \warning Call listDir() before using this function or the directory
	/// contents list will be empty.
	/// \throws Throws an out of bounds exception if position >= the number of
	/// listed directory contents.
	/// \param position array index in the directory contents list
	/// \returns file or directory name
	std::string getName(std::size_t position) const;
	
	/// Get the full path of the file or directory at a given position in
	/// the directory contents list.
	///
	/// \warning Call listDir() before using this function or the directory
	/// contents list will be empty.
	/// \throws Throws an out of bounds exception if position >= the number of
	/// listed directory contents.
	/// \param position array index in the directory contents list
	/// \returns file or directory name including the current path
	std::string getPath(std::size_t position) const;
	
	/// Open an ofFile instance using the path a given position in the
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
	
	/// Get files and directories in the directory contents list.
	///
	/// Directory contents are automatically listed.
	///
	/// \returns vector of files in the directory
	const std::vector<ofFile> & getFiles() const;

	/// Access directory contents via th array operator.
	///
	/// \warning Call listDir() before using this function or the directory
	/// contents list will be empty.
	/// \throw Throws an out of bounds exception if position >= the number of
	/// listed directory contents.
	/// \param position array index in the directory contents list
	/// \returns opened ofFile instance
	ofFile operator[](std::size_t position) const;

	/// Check whether hidden files & directories are included when
	/// listing files.
	///
	/// Mac & Linux denote hidden directories by prepending a period
	/// -> ".hello".
	///
	/// \returns true if hidden files are shown
	bool getShowHidden() const;

	/// Closes the directory.
	///
	/// This is for backwards compatibility with ofxDirList.
	void reset();
	
	typedef enum{
            SORT_FAST,
            SORT_NATURAL,
            SORT_BY_DATE
	}SortMode;

	/// Sort the directory contents list alphabetically.
	///
	/// \warning Call listDir() before using this function or there will be
	/// nothing to sort.
	/// \param SortMode options are SORT_FAST, SORT_NATURAL (default) or SORT_BY_DATE
	void sort(const SortMode & mode = SORT_NATURAL);
	
	/// Sort the directory contents list by date.
	///
	/// \warning Call listDir() before using this function or there will be
	/// nothing to sort.
	void sortByDate();

	/// Get a sorted ofDirectory instance using the current path.
	///
	/// \returns sorted ofDirectory instance
	ofDirectory getSorted();

	/// Get the number of paths in the current directory list.
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

	operator of::filesystem::path(){
		return myDir;
	}

	operator of::filesystem::path() const{
		return myDir;
	}

	//-------
	// static helpers
	//-------

	/// Create a directory at a given path.
	///
	/// Creates relative to the data path by default.
	///
	/// \param dirPath directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data directory
	/// \param bRecursive set to true to automatically create nested directories
	/// as required
	/// \returns true if directory was created successfully
	static bool createDirectory(const of::filesystem::path& dirPath, bool bRelativeToData = true, bool recursive = false);
	
	/// Check if a directory at a given path is empty.
	///
	/// Assumes directory path is relative to the data path by default.
	///
	/// \param dirPath directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data directory
	/// \returns true if the directory is empty aka contains no files or
	/// directories
	static bool isDirectoryEmpty(const of::filesystem::path& dirPath, bool bRelativeToData = true );
	
	/// Check if a directory exists at a given path.
	///
	/// Assumes directory path is relative to the data path by default.
	///
	/// \param dirPath directory path
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data directory
	/// \returns true if the directory exists
	static bool doesDirectoryExist(const of::filesystem::path& dirPath, bool bRelativeToData = true);
	
	
	/// remove a directory at a given path
	///
	/// \param deleteIfNotEmpty set to true if you want to recursively delete
	/// the directory *and* its contents
	/// \param bRelativeToData set to false if you are working with paths that
	/// are *not* in the data directory
	/// \returns true if the path was removed successfully
	static bool removeDirectory(const of::filesystem::path& path, bool deleteIfNotEmpty,  bool bRelativeToData = true);

	std::vector<ofFile>::const_iterator begin() const;
	std::vector<ofFile>::const_iterator end() const;
	std::vector<ofFile>::const_reverse_iterator rbegin() const;
	std::vector<ofFile>::const_reverse_iterator rend() const;

	of::filesystem::path myDir;
	std::string originalDirectory;
	std::vector <std::string> extensions;
	std::vector <ofFile> files;
	bool showHidden;

};

/// \section Data Path
/// \brief Enable the use of the data path.
///
/// This function causes ofToDataPath() to respect the relative path set
/// with ofSetDataPathRoot().  This is enabled by default.
void ofEnableDataPath();

/// \brief Disable the use of the data path.
///
/// This function causes ofToDataPath() to ignore the relative path set
/// with ofSetDataPathRoot().
void ofDisableDataPath();

/// \brief Make a path relative to the location of the data/ folder.
///
/// This funtion returns path unchanged if ofDisableDataPath() was called first.
///
/// By default, a relative path is returned. Users requiring absolute paths for
/// (e.g. for non-openFrameworks functions), can specify that an absolute path
/// be returned.
///
/// \param path The path to make relative to the data/ folder.
/// \param absolute Set to true to return an absolute path.
/// \returns the new path, unless paths were disabled with ofDisableDataPath().
//	MARK: - near future
//of::filesystem::path ofToDataPath(const of::filesystem::path & path, bool absolute=false);
std::string ofToDataPath(const of::filesystem::path & path, bool absolute=false);

/// \brief Reset the working directory to the platform default.
///
/// The default working directory is where the application was started from
/// or the exe directory in case of osx bundles. GLUT might change the default
/// working directory to the resources directory in the bundle in osx. This
/// will restore it to the exe dir or whatever was the current dir when the
/// application was started
bool ofRestoreWorkingDirectoryToDefault();

/// \brief Set the relative path to the data/ folder from the executable.
///
/// This method can be useful when users want to embed the data as a resource
/// folder within an *.app bundle on OSX or perhaps work from a shared data
/// folder in the user's Documents directory.
///
/// \warning The provided path must have a trailing slash (/).
/// \param root The path to the data/ folder relative to the app executable.
void ofSetDataPathRoot(const of::filesystem::path& root);

/*! \cond PRIVATE */
namespace of{
namespace priv{
    void initfileutils();
}
}
/*! \endcond */
