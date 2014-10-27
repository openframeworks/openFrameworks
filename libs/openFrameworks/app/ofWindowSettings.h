#pragma once

class ofWindowSettings{
public:
	ofWindowSettings()
	:width(1024)
	,height(768)
	,position(0,0)
	,windowMode(OF_WINDOW){}

	int width;
	int height;
	ofVec2f position;
	ofWindowMode windowMode;
};

class ofGLWindowSettings: public ofWindowSettings{
public:
	ofGLWindowSettings()
	:glVersionMajor(2)
	,glVersionMinor(1){}

	void setGLVersion(int major, int minor){
		glVersionMajor = major;
		glVersionMinor = minor;
	}

	int glVersionMajor;
	int glVersionMinor;
};

class ofGLESWindowSettings: public ofWindowSettings{
public:
	ofGLESWindowSettings()
	:glesVersion(1){}
	int glesVersion;

	void setGLESVersion(int version){
		glesVersion = version;
	}
};
