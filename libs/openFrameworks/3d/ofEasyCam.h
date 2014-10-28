#pragma once

#include "ofCamera.h"
#include "ofEvents.h"


/// \brief A super simple camera for interacting with objects in 3D space.
class ofEasyCam : public ofCamera {
public:
    /// \brief Create a default camera.
	ofEasyCam();

    /// \brief Destroy the camera.
	~ofEasyCam();

	virtual void begin(ofRectangle viewport = ofRectangle());

    /// \brief Reset the camera position and orientation.
	void reset();

    /// \brief Set the camera's target.
    /// \param target The position of the target.
	void setTarget(const ofVec3f& target);

    /// \brief Set the camera's target.
    /// \param target The position of the target.
	void setTarget(ofNode& target);

    /// \brief Get the camera's target node reference.
    /// \returns a reference the the camera's target node.
	ofNode& getTarget();

    /// \brief Set the camera's distance to the target.
    /// \param distance The distance to the target.
	void setDistance(float distance);

    /// \brief Get the distance to the target.
    /// \returns the distance to the target.
	float getDistance() const;

    /// \brief Set the camera's drag coefficient.
    ///
    /// Drag is affects how quickly the camera starts up and slows down when
    /// a user interacts with its position using a pointer.
    ///
    /// \param drag The normalized coefficient value between 0 and 1.
	void setDrag(float drag);

    /// \brief Get the camera's drag coefficient.
    /// \returns the camera's drag coefficient.
	float getDrag() const;

    /// \brief Set the key used to switch between camera rotation and translation.
    ///
    /// Translation will only happen when the translation key is pressed.
    ///
    /// \param key The key code for the translation key.
    /// \todo char is not the right data type for this. Should be int.
	void setTranslationKey(char key);

    /// \brief Get the current translation key code.
    /// \returns the current translation key code.
    char getTranslationKey();

    /// \brief Enable mouse camera control.
	void enableMouseInput();

    /// \brief Disable mouse camera control.
	void disableMouseInput();

    /// \brief Determine if mouse camera control is enabled.
    /// \todo Rename to isMouseInputEnabled().
    /// \returns true iff mouse camera control is enabled.
	bool getMouseInputEnabled();

    /// \brief Enable the mouse's middle button for camera control.
	void enableMouseMiddleButton();

    /// \brief Disable the mouse's middle button for camera control.
	void disableMouseMiddleButton();

    /// \brief Determine if the middle mouse button is enabled.
    /// \todo Rename to isMouseMiddleButtonEnabled().
    /// \returns true iff the mouse's middle button is enabled.
	bool getMouseMiddleButtonEnabled();

    /// \brief Enable or disable camera autodistance.
    ///
    /// Allow the camera to attempt to set the distance based on the camera's
    /// current viewpoirt.
    ///
    /// \param bAutoDistance true to enable auto distance.
    void setAutoDistance(bool bAutoDistance);

    void setEvents(ofCoreEvents & events);
	
private:
	void setDistance(float distance, bool save);

	ofNode target;

	bool bEnableMouseMiddleButton;
	bool bApplyInertia;
	bool bDoTranslate;
	bool bDoRotate;
	bool bDoScrollZoom;
	bool bInsideArcball;
	bool bMouseInputEnabled;
	bool bDistanceSet;
    bool bAutoDistance;
    bool bEventsSet;
	float lastDistance;

	float drag;
	
	float xRot;
	float yRot;
	float zRot;
	
	float moveX;
	float moveY;
	float moveZ;
	
	float sensitivityXY;
	float sensitivityZ;
	float sensitivityRot;
	
	float rotationFactor;

	ofVec2f lastMouse, prevMouse;
	ofVec2f mouseVel;
	
	void updateRotation();
	void updateTranslation();
	void update(ofEventArgs & args);
	void mousePressed(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);
	void mouseDragged(ofMouseEventArgs & mouse);
	void mouseScrolled(ofMouseEventArgs & mouse);
	void updateMouse(const ofMouseEventArgs & mouse);

    /// \brief The key used to differentiate between translation and rotation.
	char doTranslationKey;

    /// \brief The time of the last pointer down event.
	unsigned long lastTap;

    /// \brief The current rotation quaternion.
	ofQuaternion curRot;

    /// \brief The previous X axis.
    ofVec3f prevAxisX;

    /// \brief The previous Y axis.
    ofVec3f prevAxisY;

    /// \brief The previous Z axis.
    ofVec3f prevAxisZ;

    /// \brief the previous camera position.
	ofVec3f prevPosition;

    /// \brief The previous camera orientation.
	ofQuaternion prevOrientation;

	ofRectangle viewport;

	ofCoreEvents * events;
};
