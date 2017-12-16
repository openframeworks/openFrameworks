#pragma once
#include "ofConstants.h"

template<class Content>
class ofxAndroidCircBuffer {
	ofxAndroidCircBuffer(const ofxAndroidCircBuffer&){};
	ofxAndroidCircBuffer & operator=(const ofxAndroidCircBuffer&){return *this;}
public:
	ofxAndroidCircBuffer(int _size=0, const Content & init_val = Content());

	void setup(int _size, const Content & init_val = Content());

	void write(const Content & element);
	Content read(const Content & default_c = Content());

	int size(){ return buffer_size; }

	bool is_empty(){ return empty; }
	bool is_full(){ return full; }

	int num_elements(){
		if(writeIndex>=readIndex) return writeIndex - readIndex;
		else return buffer_size - readIndex + writeIndex;
	}

private:
	std::vector<Content> buffer;
	int readIndex;
	int writeIndex;
	int buffer_size;

	bool full, empty;
};

template <class Content>
ofxAndroidCircBuffer<Content>::ofxAndroidCircBuffer(int size, const Content & init_val){
	readIndex = 0;
	writeIndex = 0;
	buffer_size = size;
	full = false;
	empty = true;
	buffer.resize(size,init_val);
}

template <class Content>
void ofxAndroidCircBuffer<Content>::setup(int _size, const Content & init_val){
	buffer.resize(_size,init_val);
	buffer_size = _size;
}

template <class Content>
void ofxAndroidCircBuffer<Content>::write(const Content & element){
	/*if(full){
		return;
	}*/

	//empty = false;
	buffer[writeIndex] = element;
	writeIndex = (writeIndex + 1) % buffer_size;
	//full = (writeIndex == readIndex); // true if write catches up to read
}

template <class Content>
Content ofxAndroidCircBuffer<Content>::read(const Content & default_c){
	/*if (empty){
		return default_c;
	}*/

	//full = false;
	const Content & element = buffer[readIndex];
	readIndex = (readIndex + 1) % buffer_size;
	//empty = (readIndex == writeIndex);
	return element;
}
