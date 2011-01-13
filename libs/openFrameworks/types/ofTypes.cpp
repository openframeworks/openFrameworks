/*
 * ofTypes.cpp
 *
 *  Created on: 11/01/2011
 *      Author: arturo
 */

#include "ofTypes.h"

ofBuffer::ofBuffer(){
	nextLinePos = 0;
}

ofBuffer::ofBuffer(int size, char * buffer){
	set(size,buffer);
}

ofBuffer::ofBuffer(istream & stream){
	set(stream);
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
	while (n > 0)
	{
		buffer.resize(size+n);
		memcpy(&(buffer[0])+size,aux_buffer,n);
		size += n;
		if (stream)
		{
			stream.read(aux_buffer, 1024);
			n = stream.gcount();
		}
		else n = 0;
	}
	if(size)
		return true;
	else
		return false;
}

void ofBuffer::set(int _size, char * _buffer){
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


long ofBuffer::getSize() const{
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
