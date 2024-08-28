#pragma once

#include "ofColor.h"
#include "ofRectangle.h"
#include "ofVboMesh.h"

/*
 * Internal helper to generate and cache rectangle meshes for ofxGui.
 */
class ofxGuiRectMesh {
	ofColor     mColorFill = {};
	ofRectangle mRect      = {};
	ofVboMesh   mMesh      = {};
	bool        isDirty    = true;
	bool        mHasMesh   = false;

  public:
	void clear() {
		mColorFill = {};
		mRect      = {};
		mMesh.clear();
		mHasMesh = false;
		isDirty  = false;
	}

	void draw() {

		if ( isDirty && mHasMesh ) {
			mMesh.clear();
			mHasMesh = false;
			isDirty  = false;
		}

		if ( mRect.width < 1.f || mRect.height < 1.f ) {
			// We will not draw a mesh for rectangles
			// which are smaller than one pixel for
			// either w, or height.
			return;
		}

		if ( mHasMesh == false ) {

			mMesh.addVertex( mRect.getBottomLeft() );
			mMesh.addVertex( mRect.getBottomRight() );
			mMesh.addVertex( mRect.getTopLeft() );
			mMesh.addVertex( mRect.getTopRight() );

			mMesh.addColor( mColorFill );
			mMesh.addColor( mColorFill );
			mMesh.addColor( mColorFill );
			mMesh.addColor( mColorFill );

			mMesh.addIndex( 0 );
			mMesh.addIndex( 1 );
			mMesh.addIndex( 2 );
			mMesh.addIndex( 2 );
			mMesh.addIndex( 1 );
			mMesh.addIndex( 3 );

			mMesh.setMode( ofPrimitiveMode::OF_PRIMITIVE_TRIANGLES );
			mHasMesh = true;
		}

		mMesh.draw();
	}

	void setFillColor( ofColor const &color ) {
		isDirty |= ( color != mColorFill );
		mColorFill = color;
	}

	void setExtents( ofRectangle const &rect ) {
		isDirty |= ( rect != mRect );
		mRect = rect;
	}

	void setExtents( float x, float y, float w, float h ) {
		setExtents( {x, y, w, h} );
	}
};
