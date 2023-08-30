#pragma once

#include "ofNode.h"

class ofRectangle;

// \todo Use the public API of ofNode for all transformations
// \todo add set projection matrix
// \todo support for left handed or right handed?

/// \brief A basic camera object for interacting with objects in 3D space.
/// \author Memo Akten, MSA Visuals Ltd. 2011
class ofCamera : public ofNode {
public:
	/// \name Constructor and Destructor
	/// \{
    
    /// \brief Construct a default camera.
	ofCamera();

    /// \brief Destroy the camera.
    virtual ~ofCamera();
	
	/// \}
    /// \name Camera Settings
    /// \{

	/// \brief Set the field of view for a perspective camera.
	///
	/// This sets the vertical field of view for the camera, in degrees.
	/// This only operates with perspective cameras, and will have no effect 
	/// with cameras in orthographic mode. 
	///
	/// \param f The desired field of view for the camera, in degrees.
	void setFov(float f);
	
	void setNearClip(float f);

    void setFarClip(float f);
	
	/// \brief Set the "lens offset" applied to this camera.
	/// 
	/// Ordinarily, the camera is pointed straight down the center of its view
    /// frustum. However, it is possible to orient the camera towards a location
    /// offset from the center of its frustum. This is called an "assymetric
    /// frustum" and is used (for example) in stereo views.  It is acheived by
    /// applying an offset to the center of projection.  This function sets this
    /// offset from an glm::vec2 argument.  For more information see
    /// <http://www.orthostereo.com/geometryopengl.html>.
	///
	/// \param lensOffset The "lens offset" to apply to this camera, encoded in
    ///     an glm::vec2.
	void setLensOffset(const glm::vec2 & lensOffset);
	
	/// \brief Set the recommended aspect ratio for a perspective camera.
	///
	/// Sets the aspect ratio of the camera to the desired float, and forces the
    /// use of aspect ratio calculations.  Currently only used with perspective
    /// cameras. The default value (and the value used with orthographic
    /// cameras) is the ratio of the viewport's width to the viewport's height.
	/// 
	/// \param aspectRatio The desired aspect ratio, e.g. 1.3333, 1.6, etc.
	void setAspectRatio(float aspectRatio);
	
	/// \brief Set whether or not the aspect ratio of this camera is forced to a non-default setting.
	/// 
	/// The camera's aspect ratio, by default, is the aspect ratio of your
    /// viewport.  If you have set a non-default value (with
    /// ofCamera::setAspectRatio()), you can toggle whether or not this value is
    /// applied.
	/// 
	/// \param forceAspectRatio Whether or not this camera should use an aspect ratio you have set yourself.  
	void setForceAspectRatio(bool forceAspectRatio);

	/// \brief Get the camera's field of view, in degrees.
	///
	/// Get the camera's vertical field of view, in degrees.  This is only
    /// meaningful for perspective cameras.
	///
	/// \returns The camera's field of view, in degrees.
	float getFov() const { return fov; };
	
    float getNearClip() const { return nearClip; };

    float getFarClip() const { return farClip; };
	
	/// \brief Get the "lens offset" applied to this camera, encoded as an glm::vec2.
	/// 
	/// Ordinarily, the camera is pointed straight down the center of its view
    /// frustum.  However, it is possible to orient the camera towards a
    /// location offset from the center of its frustum.  This is called an
    /// "asymetric frustum" and is used (for example) in stereo views.  It is
    /// acheived by applying an offset to the center of projection.  This
    /// function returns the offset that has been applied, as an glm::vec2.  For
    /// more information see http://www.orthostereo.com/geometryopengl.html.
	///
	/// \returns The "lens offset" applied to this camera, encoded in an glm::vec2.
	glm::vec2 getLensOffset() const { return lensOffset; };
	
	/// \brief Get the boolean state which indicates whether the aspect ratio of this camera is forced to a non-default setting.
	/// 
	/// \returns A boolean: whether or not this camera's aspect ratio is set to a non-default value. 
	bool getForceAspectRatio() const {return forceAspectRatio;};
	
	/// \brief Get the aspect ratio of this camera's viewport.
	///
	/// Returns the aspect ratio of this camera's viewport. Usually this will be
    /// the ratio of the width to height of your display. Intended for
    /// perspective cameras.
	/// 
	/// \returns The aspect ratio of this camera's viewport.
	float getAspectRatio() const {return aspectRatio; };

	/// \}
	/// \name OpenGL Setup
	/// \{
	
	void setupPerspective(bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0, const glm::vec2 & lensOffset = glm::vec2(0.0f, 0.0f));

	void setupOffAxisViewPortal(const glm::vec3 & topLeft, const glm::vec3 & bottomLeft, const glm::vec3 & bottomRight);
	
	void setVFlip(bool vflip);

    bool isVFlipped() const;

	void enableOrtho();

    void disableOrtho();

    bool getOrtho() const;
	
	float getImagePlaneDistance(const ofRectangle & viewport = ofRectangle()) const;

	/// \}
	/// \name Rendering
	/// \{

	/// \brief Begins rendering with the camera.
    ///
    /// ~~~~{.cpp}
    /// void draw() {
    ///     // Begin rendering from the camera's perspective.
    ///     camera.begin();
    ///
    ///     ofLine(0, 0, ofGetWidth(), ofGetHeight());
    ///     // Additional rendering ...
    ///
    ///     // End rendering form the camera's perspective.
    ///     camera.end();
    /// }
    /// ~~~~
    /// \param viewport The camera's rendering viewport.
	virtual void begin(){
		begin(getViewport());
	}
	virtual void begin(const ofRectangle & viewport);

    /// \brief Ends rendering with the camera.
	virtual void end();
	
	/// \}
	/// \name OpenGL Matrix
	/// \{

	/// \brief Access the projection matrix.
    /// \returns the current 4x4 projection matrix.
	glm::mat4 getProjectionMatrix() const{
		return getProjectionMatrix(getViewport());
	}
	glm::mat4 getProjectionMatrix(const ofRectangle & viewport) const;

    /// \brief Access the model view matrix.
    /// \returns the current 4x4 model view matrix.
	glm::mat4 getModelViewMatrix() const;

    /// \todo getModelViewProjectionMatrix()
	glm::mat4 getModelViewProjectionMatrix(const ofRectangle & viewport) const;
	glm::mat4 getModelViewProjectionMatrix() const{
		return getModelViewProjectionMatrix(getViewport());
	}

    /// \}
    /// \name Coordinate Conversion
    /// \{
	
    /// \brief Obtain the screen coordinates of a point in the 3D world.
	///
	/// Takes an (X,Y,Z) point in your 3D world, encoded as an glm::vec3,
	/// and returns the location (also as an glm::vec3) where this point would
    /// appear on your (two-dimensional) display. The screen position's "Z
    /// coordinate" is set to be the same as your camera's.
	///
	/// \param WorldXYZ A 3D point in the world, whose screen coordinates you wish to know. 
	/// \param viewport (Optional) A viewport. The default is ofGetCurrentViewport(). 
	/// \returns An glm::vec3 containing the screen coordinates of your 3D point of interest.
	glm::vec3 worldToScreen(glm::vec3 WorldXYZ, const ofRectangle & viewport) const;
	glm::vec3 worldToScreen(glm::vec3 WorldXYZ) const{
		return worldToScreen(WorldXYZ, getViewport());
	}
	
	/// \brief Obtain the coordinates, in the 3D world, of a 2D point presumed to be on your screen.
	///
	/// Takes a pixel location on your screen, encoded in an glm::vec3,
	/// and returns (also as an glm::vec3) the 3D world coordinates of that point.
	/// You'll also need to specify a Z value when providing your screen point. 
	/// This Z value is interpreted as a distance into or away from the screen. 
	///
	/// \param ScreenXYZ A point on your screen, whose 3D world coordinates you wish to know.
	glm::vec3 screenToWorld(glm::vec3 ScreenXYZ, const ofRectangle & viewport) const;
	glm::vec3 screenToWorld(glm::vec3 ScreenXYZ) const{
		return screenToWorld(ScreenXYZ, getViewport());
	}
	
	/// \todo worldToCamera()
	glm::vec3 worldToCamera(glm::vec3 WorldXYZ, const ofRectangle & viewport) const;
	glm::vec3 worldToCamera(glm::vec3 WorldXYZ) const{
		return worldToCamera(WorldXYZ, getViewport());
	}

	/// \todo cameraToWorld()
	glm::vec3 cameraToWorld(glm::vec3 CameraXYZ, const ofRectangle & viewport) const;
	glm::vec3 cameraToWorld(glm::vec3 CameraXYZ) const{
		return cameraToWorld(CameraXYZ, getViewport());
	}

	/// \}
	/// \name Renderer
	/// \{
    
    void setRenderer(std::shared_ptr<ofBaseRenderer> renderer);
	
	/// \}

	/// \brief Draw a visual representation of the camera's frustum
	/// \note  This will only be visible when the camera drawing its 
	///        frustum is viewed through another camera.
	void drawFrustum(const ofRectangle & viewport) const;
	void drawFrustum() const{
		drawFrustum(getViewport());
	}

protected:
	ofRectangle getViewport() const;
	std::shared_ptr<ofBaseRenderer> getRenderer() const;
	void calcClipPlanes(const ofRectangle & viewport);
	
private:
	bool isOrtho;
	float fov;
	float nearClip;
	float farClip;
	glm::vec2 lensOffset;
	bool forceAspectRatio;
	float aspectRatio; // only used when forceAspect=true, = w / h
	bool vFlip;
	std::shared_ptr<ofBaseRenderer> renderer;
};

