//
//  ofxAssimpBone.cpp
//  ofxAssimpExample
//
//  Created by Nick Hardeman on 10/24/23.
//

#include "ofxAssimpBone.h"
#include "ofGraphics.h"
#include "of3dGraphics.h"
#include "of3dUtils.h"
#include "ofxAssimpUtils.h"
#include "ofVboMesh.h"

using namespace ofx::assimp;
using std::shared_ptr;
using std::make_shared;

//std::shared_ptr<ofVboMesh> Bone::sRenderMesh;
std::unordered_map< int, std::shared_ptr<ofVboMesh> > Bone::sRenderMeshes;

//--------------------------------------------------------------
void Bone::setSrcBone( std::shared_ptr<ofx::assimp::SrcBone> aSrcBone ) {
	mSrcBone = aSrcBone;
	mOffsetMatrix = mSrcBone->getAiOffsetMatrix();
	
	mGlmOffsetMat = aiMatrix4x4ToGlmMatrix(mOffsetMatrix);
	
	setOfNodeFromAiMatrix( mSrcBone->getAiMatrix(), this );
}

////--------------------------------------------------------------
//void Bone::updateFromSrcBone() {
//	if( mSrcBone ) {
//
//		aiVector3t<float> tAiScale;
//		aiQuaterniont<float> tAiRotation;
//		aiVector3t<float> tAiPosition;
//
//		// TODO: These will get update via the animation keyframes
//		// SO there won't be a decompose every frame
////		mAiMatrixGlobal = mSrcBone->getAiMatrixGlobal();
//		mAiMatrix = mSrcBone->getAiMatrix();
//		// this is the local matrix
//		mSrcBone->getAiMatrix().Decompose( tAiScale, tAiRotation, tAiPosition );
//
//		glm::vec3 tpos = glm::vec3( tAiPosition.x, tAiPosition.y, tAiPosition.z );
//		glm::quat tquat = glm::quat(tAiRotation.w, tAiRotation.x, tAiRotation.y, tAiRotation.z);
//	//	glm::quat tquat = glm::quat(tAiRotation.x, tAiRotation.y, tAiRotation.z, tAiRotation.w);
//		glm::vec3 tscale = glm::vec3( tAiScale.x, tAiScale.y, tAiScale.z );
//
//		setPositionOrientationScale( tpos, tquat, tscale );
//
////		mOffsetMatrix.Decompose(tAiScale, tAiRotation, tAiPosition);
//
////		mOffsetOrientation = glm::quat(tAiRotation.w, tAiRotation.x, tAiRotation.y, tAiRotation.z);
//
////		setPosition( mSrcBone->getPosition() );
////		setOrientation( mSrcBone->getOrientationQuat() );
////		setScale( mSrcBone->getScale() );
//	}
//}

//--------------------------------------------------------------
void Bone::cacheGlobalBoneMat(glm::mat4& aInvMat) {
//	glmMat4ToAiMatrix4x4(gMat);
//	aiMatrix4x4 posTrafo = gBoneMat * sbone->getAiOffsetMatrix();
	mCachedGlobalBoneMat = aInvMat * getGlobalTransformMatrix();
	mCachedGlobalBoneMat = mCachedGlobalBoneMat * mGlmOffsetMat;
	mAiCachedGlobalBoneMat = glmMat4ToAiMatrix4x4(mCachedGlobalBoneMat);
}

//--------------------------------------------------------------
void Bone::draw(float aAxisSize) {
	_initRenderMesh();
	
	// TODO: Cache this length and position
	// IE: get cached global position
	// TODO: figure out bone axis to align drawing to
	// should help avoid spinning when aligned to an axis to create rotation
	auto gpos = getGlobalPosition();
	auto gquat = getGlobalOrientation();
	
	auto localTransformMatrix = glm::translate(glm::mat4(1.0f), gpos );
	//		localTransformMatrix = localTransformMatrix * glm::toMat4((const glm::quat&)gquat);
	
//	auto gOrient = getGlobalOrientation();
	for( auto& kid : mKids ) {
//		auto kgpos = kid->getGlobalPositionCached();
		auto kgpos = kid->getGlobalPosition();
		float tlength = glm::distance( kgpos, gpos );
		auto diffn = glm::normalize(kgpos-gpos);
		mAlignAxis = 2;
		
		ofNode tnode;
		glm::quat lquat;
		// taken from ofNode::lookAt, copied here to save some length and normalize calculations
		auto zaxis = diffn;//glm::normalize(getGlobalPosition() - lookAtPosition);
		if (tlength > 0) {
			auto xaxis = glm::normalize(glm::cross(gquat * glm::vec3(0.f, 0.f, 1.0f), zaxis));
			auto yaxis = glm::cross(zaxis, xaxis);
			glm::mat3 m;
			m[0] = xaxis;
			m[1] = yaxis;
			m[2] = zaxis;
			
			lquat = glm::toQuat(m);
		} else {
			lquat = glm::angleAxis( 0.0f, glm::vec3(0.0f, 0.0f, 1.f) );
		}
		
		tnode.setPositionOrientationScale(gpos, lquat, tlength );
		ofPushMatrix(); {
			ofMultMatrix(tnode.getLocalTransformMatrix());
			if( sRenderMeshes.count(mAlignAxis) > 0) {
				sRenderMeshes[mAlignAxis]->draw();
			}
		} ofPopMatrix();
	}
	
	localTransformMatrix = localTransformMatrix * glm::toMat4((const glm::quat&)gquat);
	ofPushMatrix(); {
		ofMultMatrix(localTransformMatrix);
		ofDrawAxis(aAxisSize);
	} ofPopMatrix();
}

//--------------------------------------------------------------
void Bone::_initRenderMesh() {
	
	if( sRenderMeshes.size() < 1 ) {
		ofMesh tempMesh;
		tempMesh.setMode( OF_PRIMITIVE_LINES );
		
		float cheight = 0.85f;
		float cradius = 0.15f;
		
		float sx = 0.15f;
		float sy = cradius;
		float sz = cradius;
		
		tempMesh.addVertex( {sx,-sy, -sz} );
		tempMesh.addVertex( {sx,-sy, sz} );
		tempMesh.addVertex( {sx, sy, sz} );
		tempMesh.addVertex( {sx,sy, -sz} );
		tempMesh.addVertex( {1.0f,0.f,0.f} );
		tempMesh.addVertex( {0.0f,0.f,0.f} );
		
		tempMesh.addIndices({
			0, 4,
			1, 4,
			2, 4,
			3, 4,
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			0, 5,
			1, 5,
			2, 5,
			3, 5
		});
		
		ofMesh tsphere = ofMesh::icosphere( 0.15, 1 );
		tsphere.clearTexCoords();
		tsphere.clearNormals();
		tsphere.clearColors();
//		sRenderMesh->append(tsphere);
		
		tempMesh.disableTextures();
		tempMesh.disableColors();
		tempMesh.disableNormals();
		
		auto overts = tempMesh.getVertices();
		int numToAdd = 6;
		for( int i = 0; i < numToAdd; i++ ) {
			auto tVboMesh = make_shared<ofVboMesh>();
			auto nverts = overts;
			glm::mat4 trot = glm::mat4(1.0f);// = glm::mat4_cast(glm::rotation(glm::vec3(1.f,0.f,0.f), glm::vec3(0.0f,1.f, 0.0f)));
			if( i == 1 ) {
				// rotate to align to y axis
				trot = glm::mat4_cast(glm::rotation(glm::vec3(1.f,0.f,0.f), glm::vec3(0.0f,1.f, 0.0f)));
			} else if(i == 2) {
				trot = glm::mat4_cast(glm::rotation(glm::vec3(1.f,0.f,0.f), glm::vec3(0.0f,0.f, 1.0f)));
			} else if(i == 3 ) {
				trot = glm::mat4_cast(glm::rotation(glm::vec3(1.f,0.f,0.f), glm::vec3(-1.f,0.f, 0.0f)));
			} else if(i == 4 ) {
				trot = glm::mat4_cast(glm::rotation(glm::vec3(1.f,0.f,0.f), glm::vec3(0.f,-1.f, 0.0f)));
			} else if(i == 5 ) {
				trot = glm::mat4_cast(glm::rotation(glm::vec3(1.f,0.f,0.f), glm::vec3(0.f,0.f, -1.0f)));
			}
			
			if( i > 0 ) {
				for( auto& nv : nverts ) {
					nv = trot * glm::vec4(nv, 1.0f);
				}
			}
			
			tempMesh.getVertices() = nverts;
			tVboMesh->setMode( OF_PRIMITIVE_LINES );
			tVboMesh->append(tempMesh);
			
			sRenderMeshes[i] = tVboMesh;
		}
	}
}
