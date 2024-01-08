//
//  ofxAssimpBounds.h
//  AssimpModelLoaderGlow
//
//  Created by Nick Hardeman on 10/18/23.
//

#pragma once
#include "ofMesh.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ofNode.h"

namespace ofx::assimp {

class Bounds {
protected:
	static ofMesh mLinesMesh;
	
public:
	
	Bounds();
	Bounds( const glm::vec2& amin, const glm::vec2& amax );
	Bounds( const glm::vec3& amin, const glm::vec3& amax );
	~Bounds();
	
	void clear();
	
	void include( glm::mat4& amat, const std::vector<aiVector3D>& averts);
	void include( const std::vector<aiVector3D>& averts);
	
	void include( const glm::mat4& amat, const std::vector<glm::vec3>& averts);
	void include( const std::vector<glm::vec3>& averts);
		
	void update();
	void draw();
	
	std::vector<glm::vec3>& getBoundingVerts() {return mBoundingVerts;}
	
	float getWidth();
	float getHeight();
	float getDepth();
	glm::vec3 getDimensions();
	
	bool operator==( const Bounds& aob ) const;
	bool operator!=( const Bounds& aob ) const;
	
	Bounds  operator+( const glm::vec2& apnt ) const;
	Bounds  operator+( const glm::vec3& apnt ) const;
	Bounds& operator+=( const glm::vec2& aPos2 );
	Bounds& operator+=( const glm::vec3& aPos3 );
	
	Bounds  operator-( const glm::vec2& apnt ) const;
	Bounds  operator-( const glm::vec3& apnt ) const;
	Bounds& operator-=( const glm::vec2& aPos2 );
	Bounds& operator-=( const glm::vec3& aPos3 );
	
	Bounds  operator*( const float& ascale ) const;
	Bounds& operator*=( const float& ascale );
	Bounds  operator*( const glm::vec3& ascale ) const;
	Bounds& operator*=( const glm::vec3& ascale );
	
	glm::vec3 min = {0.f, 0.f, 0.f };
	glm::vec3 max = {0.f,0.f,0.f};
	glm::vec3 center = {0.f, 0.f, 0.f };
	float radius = 0.f;
	
protected:
	void _resetMinMax();
	void _calcMin( const glm::vec3& apt );
	void _calcMax( const glm::vec3& apt );
	void _calcCenter();
	void _calcRadius();
	
	std::vector<glm::vec3> mBoundingVerts;
	
	bool bCalced = false;
};
}

std::ostream& operator<<(std::ostream& os, const ofx::assimp::Bounds& ab);
