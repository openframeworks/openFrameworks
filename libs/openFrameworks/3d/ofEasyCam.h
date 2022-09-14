#pragma once

#include "ofCamera.h"
#include "ofEvents.h"
#include "ofRectangle.h"
#include "glm/gtc/quaternion.hpp"

/// \brief A super simple camera for interacting with objects in 3D space.
class ofEasyCam : public ofCamera {
public:
    /// \name Constructor and Destructor
	/// \{
    
    /// \brief Create a default camera.
	ofEasyCam();

	/// \}
	/// \name Rendering
	/// \{

	virtual void begin(const ofRectangle & viewport);
	virtual void begin(){
		begin(getViewport());
	}

    /// \brief Reset the camera position and orientation.
	void reset();
	
	/// \}
	/// \name Camera Target
	/// \{

    /// \brief Set the camera's target.
    /// \param target The position of the target.
	void setTarget(const glm::vec3& target);

    /// \brief Set the camera's target.
    /// \param target The position of the target.
	void setTarget(ofNode& target);

    /// \brief Get the camera's target node reference.
    /// \returns a reference the the camera's target node.
	const ofNode& getTarget() const;

	/// \}
	/// \name Getters and Setters
	/// \{

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

	/// \brief Enable or disable camera autodistance.
    ///
    /// Allow the camera to attempt to set the distance based on the camera's
    /// current viewpoirt.
    ///
    /// \param bAutoDistance true to enable auto distance.
    void setAutoDistance(bool bAutoDistance);

    void setEvents(ofCoreEvents & events);
	
	/// \brief Set the input sensitivity of the rotation.
	/// X and Y axes - when the value is 1.0, moving the mouse from one side to
	/// the other of the arcball (min(viewport.width, viewport.height)) will
	/// rotate 180 degrees. When the value is 0.5, the rotation will be 90
	/// degrees.
	/// \param value Scales the xyz axes rotation factor by these values.
	void setRotationSensitivity(float x, float y, float z);
	void setRotationSensitivity(const glm::vec3& sensitivity);
	
    /// \brief Set the input sensitivity of the translation.
    /// \param value Scales the xyz axes translation factor by these values.
    void setTranslationSensitivity(float x, float y, float z);
	void setTranslationSensitivity(const glm::vec3& sensitivity);
    /// \brief Set the key used to switch between camera rotation and translation.
    ///
    /// Translation will only happen when the translation key is pressed.
    ///
    /// \param key The key code for the translation key.
    /// \todo char is not the right data type for this. Should be int.
	void setTranslationKey(char key);

    /// \brief Get the current translation key code.
    /// \returns the current translation key code.
    char getTranslationKey() const;

    /// \}
    /// \name Mouse Input
    /// \{

    /// \brief Enable mouse camera control.
	void enableMouseInput();

    /// \brief Disable mouse camera control.
	void disableMouseInput();

    /// \brief Determine if mouse camera control is enabled.
    /// \todo Rename to isMouseInputEnabled().
    /// \returns true iff mouse camera control is enabled.
	bool getMouseInputEnabled() const;

    /// \brief Enable the mouse's middle button for camera control.
	void enableMouseMiddleButton();

    /// \brief Disable the mouse's middle button for camera control.
	void disableMouseMiddleButton();

    /// \brief Determine if the middle mouse button is enabled.
    /// \todo Rename to isMouseMiddleButtonEnabled().
    /// \returns true iff the mouse's middle button is enabled.
	bool getMouseMiddleButtonEnabled() const;

	/// \}

	/// Uses Y axis relative to the camera orientation
	///
	/// By default the Y axis used for interactive rotation
	/// is vec3(0,1,0) or whatever is set as up axis using
	/// setUpAxis
	void setRelativeYAxis(bool relative=true);

	/// Determine if the Y axis is set to be relative to the 
	/// camera orientation
	bool getRelativeYAxis() const;

	/// Set the camera fixed up axis for interactive
	/// manipulation.
	void setUpAxis(const glm::vec3 & up);

	/// Get the up axis.
	const glm::vec3 & getUpAxis() const;

	void enableInertia();
	void disableInertia();

	/// Determine if intertia is enabled.
	bool getInertiaEnabled() const;

	/// Set the area bounds for mouse control.
	/// Uses the full viewport by default.
	void setControlArea(const ofRectangle & controlArea);

	/// Clears the area bounds for mouse control so that 
	/// the full viewport is used.
	void clearControlArea();

	/// Returns the area bounds used for mouse control.
	ofRectangle getControlArea() const;
	
	/// Transformation types available for mouse interaction.
	enum TransformType{
		TRANSFORM_NONE,
		TRANSFORM_ROTATE,
		TRANSFORM_TRANSLATE_XY,
		TRANSFORM_TRANSLATE_Z,
		TRANSFORM_SCALE
	};

	void addInteraction(TransformType type, int mouseButton, int key = -1);
	void removeInteraction(TransformType type, int mouseButton, int key = -1);
	bool hasInteraction(TransformType type, int mouseButton, int key = -1);
	bool hasInteraction(int mouseButton, int key);
	void removeAllInteractions();
protected:
	virtual void onPositionChanged() ;
private:
	void setDistance(float distance, bool save);

	ofNode target;

	bool bEnableMouseMiddleButton = true;
	bool bApplyInertia = false;
	
	bool bInsideArcball = false;
	bool bMouseInputEnabled = true;
	bool bDistanceSet = false;
	bool bAutoDistance = true;
	bool bEventsSet = false;
	bool bIsScrolling = false;
	float lastDistance = 0.f;

	
	float drag = 0.9f;
	
	/// rot and translated are used as a temporary values shared between the mouse callbacks and the update method.
	/// How much the camera needs to be rotated.
	glm::vec3 rot;
	/// How much the camera needs to be translated.
	glm::vec3 translate;
	
	/// \brief Sensitivity
	/// These varibles determine how sensitive is the interaction.
	/// High values mean faster and bigger movements/rotations.
	/// Low Values mean more presicion.
	glm::vec3 sensitivityTranslate;
	glm::vec3 sensitivityRot;
	float     sensitivityScroll = 1.0f;

	/// \brief The previous mouse position.
	glm::vec2 prevMouse;
	/// \brief The mouse velocity (mouse position delta).
	glm::vec2 mouseVel;
	
	void updateRotation();
	void updateTranslation();
	void update(ofEventArgs & args);
	void mousePressed(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);
	void mouseScrolled(ofMouseEventArgs & mouse);
	void updateMouse(const glm::vec2 & mouse);
	/// \brief Returns the up axis vector;
	glm::vec3 up() const;

    /// \brief The key used to differentiate between translation and rotation.
	char doTranslationKey = 'm';

    /// \brief The time of the last pointer down event.
	unsigned long lastTap = 0;

    /// \brief The current rotation quaternion.
	glm::quat curRot;
	
	/// \name On Press cache
	/// \{
	/// \brief camera properties when the mouse is pressed.

	glm::vec3 lastPressAxisX;
	glm::vec3 lastPressAxisY;
	glm::vec3 lastPressAxisZ;
	glm::vec3 lastPressPosition;
	glm::quat lastPressOrientation;
	glm::vec2 lastPressMouse;
	
	/// \}
	
	/// \brief the current viewport.
	ofRectangle viewport;

	/// \brief If set, the area mouse control is bound to.
	ofRectangle controlArea;

	ofEventListeners listeners;
	ofCoreEvents * events = nullptr;

	bool bRelativeYAxis = false;
	bool doInertia = false;
	glm::vec3 upAxis{0,1,0};
	
	glm::vec2 mouseAtScroll;
	
	TransformType currentTransformType;
	/// \brief This struct holds the combination of mouse button and key press that will trigger a specific interaction.
	struct interaction{
		interaction():mouseButton(0), key(-1), transformType(TRANSFORM_NONE){}
		interaction(TransformType type, int _mouseButton, int _key = -1):mouseButton(_mouseButton), key(_key), transformType(type){}
		int mouseButton;
		int key;
		TransformType transformType;
	};
	std::vector<interaction> interactions;
};
