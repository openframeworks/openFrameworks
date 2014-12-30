#pragma once


#include "ofRectangle.h"
#include "ofGraphics.h"
#include "ofNode.h"

/// \todo Use the public API of ofNode for all transformations
/// \todo add set projection matrix
/// \todo support for left handed or right handed?

/// \brief A basic camera object for interacting with objects in 3D space.
/// \author Memo Akten, MSA Visuals Ltd. 2011
class ofCamera : public ofNode {
public:
    /// \brief Construct a default camera.
	ofCamera();

    /// \brief Destroy the camera.
    virtual ~ofCamera();
	
	/// \brief Set the field of view for a perspective camera.
	///
	/// This sets the horizontal field of view for the camera, in degrees.
	/// This only operates with perspective cameras, and will have no effect 
	/// with cameras in orthographic mode. 
	///
	/// \param f The desired field of view for the camera, in degrees.
	void setFov(float f);
	
	/// \todo setNearClip()
	void setNearClip(float f);

    /// \todo setFarClip()
	void setFarClip(float f);
	
	/// \brief Set the "lens offset" applied to this camera.
	/// 
	/// Ordinarily, the camera is pointed straight down the center of its view
    /// frustum. However, it is possible to orient the camera towards a location
    /// offset from the center of its frustum. This is called an "assymetric
    /// frustum" and is used (for example) in stereo views.  It is acheived by
    /// applying an offset to the center of projection.  This function sets this
    /// offset from an ofVec2f argument.  For more information see
    /// <http://www.orthostereo.com/geometryopengl.html>.
	///
	/// \param lensOffset The "lens offset" to apply to this camera, encoded in
    ///     an ofVec2f.
	void setLensOffset(const ofVec2f & lensOffset);
	
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
	/// Get the horizontal camera's field of view, in degrees.  This is only
    /// meaningful for perspective cameras.
	///
	/// \returns The camera's field of view, in degrees.
	float getFov() const { return fov; };
	
    /// \todo getNearClip()
	float getNearClip() const { return nearClip; };

    /// \todo getFarClip()
	float getFarClip() const { return farClip; };
	
	/// \brief Get the "lens offset" applied to this camera, encoded as an ofVec2f.
	/// 
	/// Ordinarily, the camera is pointed straight down the center of its view
    /// frustum.  However, it is possible to orient the camera towards a
    /// location offset from the center of its frustum.  This is called an
    /// "asymetric frustum" and is used (for example) in stereo views.  It is
    /// acheived by applying an offset to the center of projection.  This
    /// function returns the offset that has been applied, as an ofVec2f.  For
    /// more information see http://www.orthostereo.com/geometryopengl.html.
	///
	/// \returns The "lens offset" applied to this camera, encoded in an ofVec2f.
	ofVec2f getLensOffset() const { return lensOffset; };
	
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
	
	/// \todo setupPerspective()
	void setupPerspective(bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0, const ofVec2f & lensOffset = ofVec2f(0.0f, 0.0f));

    /// \todo setupOffAxisViewPortal()
    void setupOffAxisViewPortal(const ofVec3f & topLeft, const ofVec3f & bottomLeft, const ofVec3f & bottomRight);
	
	/// \todo setVFlip()
	void setVFlip(bool vflip);

    /// \todo isVFlipped()
    bool isVFlipped() const;

	/// \todo enableOrtho()
	void enableOrtho();

    /// \todo disableOrtho()
    void disableOrtho();

    /// \todo getOrtho()
    bool getOrtho() const;
	
	/// \todo getImagePlaneDistance
	float getImagePlaneDistance(ofRectangle viewport = ofRectangle()) const;
	
	/// \brief Begins rendering with the camera.
    ///
    /// Example:
    /// \code{.cpp}
    ///
    /// void draw() {
    ///
    ///     // Begin rendering from the camera's perspective.
    ///     camera.begin();
    ///
    ///     ofLine(0, 0, ofGetWidth(), ofGetHeight());
    ///     // Additional rendering ...
    ///
    ///     // End rendering form the camera's perspective.
    ///     camera.end();
    ///
    /// }
    ///
    /// \endcode
    ///
    /// \param viewport The camera's rendering viewport.
	virtual void begin(ofRectangle viewport = ofRectangle());

    /// \brief Ends rendering with the camera.
	virtual void end();
	
	/// \brief Access the projection matrix.
    /// \returns the current 4x4 projection matrix.
	ofMatrix4x4 getProjectionMatrix(ofRectangle viewport = ofRectangle()) const;

    /// \brief Access the model view matrix.
    /// \returns the current 4x4 model view matrix.
    ofMatrix4x4 getModelViewMatrix() const;

    /// \todo getModelViewProjectionMatrix()
    ofMatrix4x4 getModelViewProjectionMatrix(ofRectangle viewport = ofRectangle()) const;
	
    /// \brief Obtain the screen coordinates of a point in the 3D world.
	///
	/// Takes an (X,Y,Z) point in your 3D world, encoded as an ofVec3f, 
	/// and returns the location (also as an ofVec3f) where this point would
    /// appear on your (two-dimensional) display. The screen position's "Z
    /// coordinate" is set to be the same as your camera's.
	///
	/// \param WorldXYZ A 3D point in the world, whose screen coordinates you wish to know. 
	/// \param viewport (Optional) A viewport. The default is ofGetCurrentViewport(). 
	/// \returns An ofVec3f containing the screen coordinates of your 3D point of interest. 
	ofVec3f worldToScreen(ofVec3f WorldXYZ, ofRectangle viewport = ofRectangle()) const;
	
	/// \brief Obtain the coordinates, in the 3D world, of a 2D point presumed to be on your screen.
	///
	/// Takes a pixel location on your screen, encoded in an ofVec3f, 
	/// and returns (also as an ofVec3f) the 3D world coordinates of that point.
	/// You'll also need to specify a Z value when providing your screen point. 
	/// This Z value is interpreted as a distance into or away from the screen. 
	///
	/// \param ScreenXYZ A point on your screen, whose 3D world coordinates you wish to know.
	ofVec3f screenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport = ofRectangle()) const;
	
	/// \todo worldToCamera()
	ofVec3f worldToCamera(ofVec3f WorldXYZ, ofRectangle viewport = ofRectangle()) const;

	/// \todo cameraToWorld()
	ofVec3f cameraToWorld(ofVec3f CameraXYZ, ofRectangle viewport = ofRectangle()) const;
	

    void setRenderer(shared_ptr<ofBaseRenderer> renderer);

protected:
	ofRectangle getViewport(const ofRectangle & _viewport) const;
	shared_ptr<ofBaseRenderer> getRenderer() const;
	void calcClipPlanes(const ofRectangle & viewport);
	
private:
	bool isOrtho;
	float fov;
	float nearClip;
	float farClip;
	ofVec2f lensOffset;
	bool forceAspectRatio;
	float aspectRatio; // only used when forceAspect=true, = w / h
	bool isActive;
	bool vFlip;
	shared_ptr<ofBaseRenderer> renderer;
};

