#include "ofFileUtils.h"
#include "ofUtils.h"


//--------------------------------------------------
ofBuffer::ofBuffer(){
	nextLinePos = 0;
}

ofBuffer::ofBuffer(const char * buffer,int size){
	set(buffer,size);
}

ofBuffer::ofBuffer(istream & stream){
	set(stream);
}

ofBuffer::ofBuffer(const ofBuffer & buffer_){
	buffer = buffer_.buffer;
	nextLinePos = buffer_.nextLinePos;
}

ofBuffer::~ofBuffer(){
	clear();
}

bool ofBuffer::set(istream & stream){
	clear();
	if(stream.bad()) return false;

	char aux_buffer[1024];
	std::streamsize size = 0;
	stream.read(aux_buffer, 1024);
	std::streamsize n = stream.gcount();
	while (n > 0){
		buffer.resize(size+n);
		memcpy(&(buffer[0])+size,aux_buffer,n);
		size += n;
		if (stream){
			stream.read(aux_buffer, 1024);
			n = stream.gcount();
		}
		else n = 0;
	}
	if(size){
		return true;
	}else{
		return false;
	}
}

bool ofBuffer::writeTo(ostream & stream){
	if(stream.bad()) return false;
	stream.write(&(buffer[0]),buffer.size());
	return true;
}

void ofBuffer::set(const char * _buffer, int _size){
	clear();
	buffer.resize(_size);
	memcpy(getBuffer(), _buffer, _size);
}

void ofBuffer::clear(){
	buffer.clear();
	nextLinePos = 0;
}

void ofBuffer::allocate(long _size){
	clear();
	buffer.resize(_size);
}

char * ofBuffer::getBuffer(){
	return &buffer[0];
}

const char * ofBuffer::getBuffer() const{
	return &buffer[0];
}

long ofBuffer::size() const{
	return buffer.size();
}

string ofBuffer::getNextLine(){
	if( buffer.empty() ) return "";
	long currentLinePos = nextLinePos;
	while(nextLinePos<buffer.size() && buffer[nextLinePos]!='\n') nextLinePos++;
	string line(getBuffer() + currentLinePos,nextLinePos-currentLinePos);
	if(nextLinePos<buffer.size()-1) nextLinePos++;
	return line;
}

string ofBuffer::getFirstLine(){
	nextLinePos = 0;
	return getNextLine();
}

//--------------------------------------------------
ofBuffer ofBufferFromFile(const string & path, bool binary){
	ios_base::openmode mode = binary? ifstream::binary : ios_base::in;
	ifstream istr(ofToDataPath(path,true).c_str(), mode);
	ofBuffer buffer(istr);
	istr.close();
	return buffer;
}

//--------------------------------------------------
bool ofBufferToFile(const string & path, ofBuffer & file, bool binary){
	ios_base::openmode mode = binary? ofstream::binary : ios_base::out;
	ofstream ostr(ofToDataPath(path,true).c_str(), mode);
	bool ret = file.writeTo(ostr);
	ostr.close();
	return ret;
}


//------------------------------------------------------------------------------------------------------------

#include "Poco/Util/FilesystemConfiguration.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/StringTokenizer.h"
#include "Poco/Exception.h"
#include "Poco/FileStream.h"

using Poco::Path;
using Poco::File;
using Poco::DirectoryIterator;
using Poco::StringTokenizer;
using Poco::NotFoundException;

 bool ofFileUtils::copyFromTo(string pathSrc, string pathDst, bool bRelativeToData,  bool overwrite){
	if( bRelativeToData ) pathSrc = ofToDataPath(pathSrc);
	if( bRelativeToData ) pathDst = ofToDataPath(pathDst);

	if( !ofFileUtils::doesFileExist(pathSrc) ){
		return false;
	}

	if( ofFileUtils::doesFileExist(pathDst) ){
		if( overwrite ){
			ofFileUtils::deleteFolder(pathDst);
		}else{
			return false;
		}
	}

	File fileSrc(pathSrc);
	try{
		fileSrc.copyTo(pathDst);
	}catch (Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "copyFromTo - unable to copy to, file must be busy");
		return false;
	}
	return true;
}

//be careful with slashes here - appending a slash when moving a folder will causes mad headaches
//------------------------------------------------------------------------------------------------------------
 bool ofFileUtils::moveFromTo(string pathSrc, string pathDst, bool bRelativeToData, bool overwrite){
	if( bRelativeToData ) pathSrc = ofToDataPath(pathSrc);
	if( bRelativeToData ) pathDst = ofToDataPath(pathDst);

	if( !ofFileUtils::doesFileExist(pathSrc) ){
		return false;
	}

	if( ofFileUtils::doesFileExist(pathDst) ){
		if( overwrite ){
			ofFileUtils::deleteFolder(pathDst);
		}else{
			return false;
		}
	}

	File fileSrc(pathSrc);

	try{
		fileSrc.moveTo(pathDst);
	}catch (Poco::Exception & except){
		ofLog(OF_LOG_ERROR, "moveFromTo - unable to move to, file must be busy");
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------------------------------------
 string ofFileUtils::getFileExt(string filename){
	std::string::size_type idx;
	idx = filename.rfind('.');

	if(idx != std::string::npos){
		return filename.substr(idx+1);
	}
	else{
		return "";
	}
}

//------------------------------------------------------------------------------------------------------------
 string ofFileUtils::removeExt(string filename){
	std::string::size_type idx;
	idx = filename.rfind('.');

	if(idx != std::string::npos){
		return filename.substr(0, idx);
	}
	else{
		return filename;
	}
}

//------------------------------------------------------------------------------------------------------------
 string ofFileUtils::addLeadingSlash(string path){
	if(path.length() > 0){
		if( path[0] != '/'){
			path = "/"+path;
		}
	}
	return path;
}

//------------------------------------------------------------------------------------------------------------
 string ofFileUtils::addTrailingSlash(string path){
	if(path.length() > 0){
		if( path[path.length()-1] != '/'){
			path += "/";
		}
	}
	return path;
}

//------------------------------------------------------------------------------------------------------------
 string ofFileUtils::removeTrailingSlash(string path){
	if( path.length() > 0 && path[path.length()-1] == '/' ){
		path = path.substr(0, path.length()-1);
	}
	return path;
}

//------------------------------------------------------------------------------------------------------------
 bool ofFileUtils::doesDirectoryExist(string dirPath, bool bRelativeToData){
	if( bRelativeToData ) dirPath = ofToDataPath(dirPath);
	File myFile( dirPath );
	return myFile.exists();
}

//------------------------------------------------------------------------------------------------------------
 bool ofFileUtils::isDirectoryEmpty(string dirPath, bool bRelativeToData){
	if( bRelativeToData )dirPath = ofToDataPath(dirPath);
	File myFile(dirPath);
	if( myFile.exists() && myFile.isDirectory() ){
		vector <string> contents;
		myFile.list(contents);
		if( contents.size() == 0 ){
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------
 string ofFileUtils::getFilenameFromPath(string filePath, bool bRelativeToData){
	if( bRelativeToData ) filePath = ofToDataPath(filePath);

	string fileName;
	
	Path myPath(filePath);
	try{
		fileName = myPath.getFileName();
	}catch( Poco::Exception &except ){
		return "";
	}

	return fileName;
}

//------------------------------------------------------------------------------------------------------------
 string ofFileUtils::getEnclosingDirectoryFromPath(string filePath, bool bRelativeToData){
	if( bRelativeToData ) filePath = ofToDataPath(filePath);
	
	Path myPath(filePath);

	return myPath.parent().toString();
}

//------------------------------------------------------------------------------------------------------------
 string ofFileUtils::getCurrentWorkingDirectory(){
	return Path::current();
}


//------------------------------------------------------------------------------------------------------------
 bool ofFileUtils::doesFileExist(string fPath,  bool bRelativeToData){
	if( bRelativeToData ) fPath = ofToDataPath(fPath);
	File myFile( fPath );
	return myFile.exists();
}

//------------------------------------------------------------------------------------------------------------
 bool ofFileUtils::makeDirectory(string dirPath, bool bRelativeToData){
	if( bRelativeToData ) dirPath = ofToDataPath(dirPath);

	File myFile(dirPath);
	bool success = false;
	try{
		success = myFile.createDirectory();
	}catch( Poco::Exception &except ){
		ofLog(OF_LOG_ERROR, "makeDirectory - directory could not be created");
		return false;
	}

	if(!success)ofLog(OF_LOG_WARNING, "makeDirectory - directory already exists");

	return success;
}

//------------------------------------------------------------------------------------------------------------
 bool ofFileUtils::deleteFile(string path, bool bRelativeToData){
	if( bRelativeToData ) path = ofToDataPath(path);
	File myFile(path);
	try{
		myFile.remove();
	}catch( Poco::Exception &except){
		ofLog(OF_LOG_ERROR, "deleteFile - file could not be deleted");
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------------------------------------
 bool ofFileUtils::deleteFolder(string path, bool bRelativeToData){
	if( bRelativeToData ) path = ofToDataPath(path);
	File myFile(path);
	try{
		myFile.remove(true);
	}catch(Poco::Exception &except){
		ofLog(OF_LOG_ERROR, "deleteFile - folder could not be deleted");
		return false;
	}
	return true;
}

