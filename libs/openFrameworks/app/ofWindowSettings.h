#pragma once

#define GLM_FORCE_CTOR_INIT
#include "glm/vec2.hpp"
#include <string>

/// \brief Used to represent the available windowing modes for the application.
enum ofWindowMode{
	/// \brief A floating application window.
	OF_WINDOW 		= 0,
	/// \brief A fullscreen application window.
	OF_FULLSCREEN 	= 1,
	/// \brief A fullscreen application window with a custom width and height.
	OF_GAME_MODE	= 2
};


/// \brief Used to represent the available screen orientations.
///
/// These don't use "landscape" or "portrait", because phones typically default
/// to portrait while desktop screens are typically landscape by default.
///
/// \sa ::ofSetOrientation
/// \sa ::ofGetOrientation
enum ofOrientation: short{
	/// \brief Represents the default screen orientation.
	OF_ORIENTATION_DEFAULT = 1,
	/// \brief Represents a screen rotated 180 degrees, also known as upside-down.
	OF_ORIENTATION_180 = 2,
	/// \brief Represents a screen rotated 90 degrees clockwise.
	OF_ORIENTATION_90_LEFT = 3,
	/// \brief Represents a screen rotated 90 degrees counter-clockwise.
	OF_ORIENTATION_90_RIGHT = 4,
	/// \brief Represents an unknown orientation.
	OF_ORIENTATION_UNKNOWN = 5
};

class ofWindowSettings{
public:
	ofWindowSettings()
	:windowMode(OF_WINDOW)
	,width(1024)
	,height(768)
	,sizeSet(false)
	,position(0,0)
	,positionSet(false){}

	virtual ~ofWindowSettings(){};

	std::string title;
	ofWindowMode windowMode;

	void setPosition(const glm::vec2 & position) {
		this->position = position;
		this->positionSet = true;
	}

	void setSize(int width, int height) {
		this->width = width;
		this->height = height;
		this->sizeSet = true;
	}

	bool isSizeSet() const {
		return sizeSet;
	}

	int getWidth() const {
		return width;
	}

	int getHeight() const {
		return height;
	}

	const glm::vec2 & getPosition() const {
		return position;
	}

	bool isPositionSet() const {
		return positionSet;
	}

protected:
	int width;
	int height;
	bool sizeSet;
	glm::vec2 position;
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
	:glesVersion(1){}

	ofGLESWindowSettings(const ofWindowSettings & settings)
	:ofWindowSettings(settings), glesVersion(1) {
        const ofGLESWindowSettings * glesSettings = dynamic_cast<const ofGLESWindowSettings*>(&settings);
        if(glesSettings){
            glesVersion = glesSettings->glesVersion;
        }
    }

	virtual ~ofGLESWindowSettings(){};

	void setGLESVersion(int version){
		glesVersion = version;
	}

	int glesVersion;
};
