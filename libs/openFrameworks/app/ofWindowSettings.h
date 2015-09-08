#pragma once

class ofWindowSettings{
public:
	ofWindowSettings()
	:width(1024)
	,height(768)
	,windowMode(OF_WINDOW)
	,position(0,0)
	,positionSet(false){}

	virtual ~ofWindowSettings(){};

	int width;
	int height;
	ofWindowMode windowMode;

	void setPosition(const ofVec2f & position) {
		this->position = position;
		this->positionSet = true;
	}

	const ofVec2f & getPosition() const {
		return position;
	}

	bool isPositionSet() const {
		return positionSet;
	}

protected:
	ofVec2f position;
	bool positionSet;
};

class ofGLWindowSettings: public ofWindowSettings{
public:
	ofGLWindowSettings()
	:glVersionMajor(2)
	,glVersionMinor(1){}

	ofGLWindowSettings(const ofWindowSettings & settings)
	:ofWindowSettings(settings)
	,glVersionMajor(2)
	,glVersionMinor(1){
        const ofGLWindowSettings * glSettings = dynamic_cast<const ofGLWindowSettings*>(&settings);
        if(glSettings){
            glVersionMajor = glSettings->glVersionMajor;
            glVersionMinor = glSettings->glVersionMinor;
        }
    }

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
        :mGlesVersionMajor(1), mGlesVersionMinor(0){}
    
    ofGLESWindowSettings(const ofWindowSettings & settings)
	:ofWindowSettings(settings), mGlesVersionMajor(1), mGlesVersionMinor(0) {
        const ofGLESWindowSettings * glesSettings = dynamic_cast<const ofGLESWindowSettings*>(&settings);
        if(glesSettings){
            mGlesVersionMajor = glesSettings->mGlesVersionMajor;
            mGlesVersionMinor = glesSettings->mGlesVersionMinor;
        }
    }
    
    virtual ~ofGLESWindowSettings(){};
    
    void setGLESVersion(int versionMajor, int versionMinor = 0){
        mGlesVersionMajor = versionMajor;
        mGlesVersionMinor = versionMinor;
    }

    int glesVersionMajor() const { return mGlesVersionMajor; }
    int glesVersionMinor() const { return mGlesVersionMinor ; }

private:
    int mGlesVersionMajor;
    int mGlesVersionMinor;
};
