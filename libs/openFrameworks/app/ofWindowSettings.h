#pragma once

class ofWindowSettings{
public:
	ofWindowSettings()
	:width(1024)
	,height(768)
	,position(0,0)
	,windowMode(OF_WINDOW){}

	virtual ~ofWindowSettings(){};

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

	ofGLWindowSettings(const ofWindowSettings & settings)
	:ofWindowSettings(settings)
	,glVersionMajor(2)
	,glVersionMinor(1){}

	virtual ~ofGLWindowSettings(){};

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

	ofGLESWindowSettings(const ofWindowSettings & settings)
	:ofWindowSettings(settings)
	,glesVersion(2){}

	virtual ~ofGLESWindowSettings(){};

	void setGLESVersion(int version){
		glesVersion = version;
	}

	int glesVersion;
};
