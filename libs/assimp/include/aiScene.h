/*
---------------------------------------------------------------------------
Open Asset Import Library (ASSIMP)
---------------------------------------------------------------------------

Copyright (c) 2006-2010, ASSIMP Development Team

All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the following 
conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the ASSIMP team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the ASSIMP Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/

/** @file aiScene.h
 *  @brief Defines the data structures in which the imported scene is returned.
 */
#ifndef __AI_SCENE_H_INC__
#define __AI_SCENE_H_INC__

#include "aiTypes.h"
#include "aiTexture.h"
#include "aiMesh.h"
#include "aiLight.h"
#include "aiCamera.h"
#include "aiMaterial.h"
#include "aiAnim.h"

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------------------
/** A node in the imported hierarchy. 
 *
 * Each node has name, a parent node (except for the root node), 
 * a transformation relative to its parent and possibly several child nodes.
 * Simple file formats don't support hierarchical structures - for these formats 
 * the imported scene does consist of only a single root node without children.
 */
// -------------------------------------------------------------------------------
struct aiNode
{
	/** The name of the node. 
	 *
	 * The name might be empty (length of zero) but all nodes which 
	 * need to be accessed afterwards by bones or anims are usually named.
	 * Multiple nodes may have the same name, but nodes which are accessed
	 * by bones (see #aiBone and #aiMesh::mBones) *must* be unique.
	 * 
	 * Cameras and lights are assigned to a specific node name - if there
	 * are multiple nodes with this name, they're assigned to each of them.
	 * <br>
	 * There are no limitations regarding the characters contained in
	 * this text. You should be able to handle stuff like whitespace, tabs,
	 * linefeeds, quotation marks, ampersands, ... .
	 */
	C_STRUCT aiString mName;

	/** The transformation relative to the node's parent. */
	C_STRUCT aiMatrix4x4 mTransformation;

	/** Parent node. NULL if this node is the root node. */
	C_STRUCT aiNode* mParent;

	/** The number of child nodes of this node. */
	unsigned int mNumChildren;

	/** The child nodes of this node. NULL if mNumChildren is 0. */
	C_STRUCT aiNode** mChildren;

	/** The number of meshes of this node. */
	unsigned int mNumMeshes;

	/** The meshes of this node. Each entry is an index into the mesh */
	unsigned int* mMeshes;

#ifdef __cplusplus
	/** Constructor */
	aiNode() 
	{ 
		// set all members to zero by default
		mParent = NULL; 
		mNumChildren = 0; mChildren = NULL;
		mNumMeshes = 0; mMeshes = NULL;
	}

	/** Construction from a specific name */
	aiNode(const std::string& name) 
	{ 
		// set all members to zero by default
		mParent = NULL; 
		mNumChildren = 0; mChildren = NULL;
		mNumMeshes = 0; mMeshes = NULL;
		mName = name;
	}

	/** Destructor */
	~aiNode()
	{
		// delete all children recursively
		// to make sure we won't crash if the data is invalid ...
		if (mChildren && mNumChildren)  
		{
			for( unsigned int a = 0; a < mNumChildren; a++)
				delete mChildren[a];
		}
		delete [] mChildren;
		delete [] mMeshes;
	}

	/** Searches for a node with a specific name, beginning at this
	 *  nodes. Normally you will call this method on the root node
	 *  of the scene.
	 * 
	 *  @param name Name to search for
	 *  @return NULL or a valid Node if the search was successful.
	 */
	inline aiNode* FindNode(const aiString& name)
	{
		return FindNode(name.data);
	}

	/** @override
	 */
	inline aiNode* FindNode(const char* name)
	{
		if (!::strcmp( mName.data,name))return this;
		for (unsigned int i = 0; i < mNumChildren;++i)
		{
			aiNode* p = mChildren[i]->FindNode(name);
			if (p)return p;
		}
		// there is definitely no sub node with this name
		return NULL;
	}

#endif // __cplusplus
};


// -------------------------------------------------------------------------------
/** @def AI_SCENE_FLAGS_INCOMPLETE
 * Specifies that the scene data structure that was imported is not complete.
 * This flag bypasses some internal validations and allows the import 
 * of animation skeletons, material libraries or camera animation paths 
 * using Assimp. Most applications won't support such data. 
 */
#define AI_SCENE_FLAGS_INCOMPLETE	0x1

/** @def AI_SCENE_FLAGS_VALIDATED
 * This flag is set by the validation postprocess-step (aiPostProcess_ValidateDS)
 * if the validation is successful. In a validated scene you can be sure that
 * any cross references in the data structure (e.g. vertex indices) are valid.
 */
#define AI_SCENE_FLAGS_VALIDATED	0x2

/** @def AI_SCENE_FLAGS_VALIDATION_WARNING
 * This flag is set by the validation postprocess-step (aiPostProcess_ValidateDS)
 * if the validation is successful but some issues have been found.
 * This can for example mean that a texture that does not exist is referenced 
 * by a material or that the bone weights for a vertex don't sum to 1.0 ... .
 * In most cases you should still be able to use the import. This flag could
 * be useful for applications which don't capture Assimp's log output.
 */
#define AI_SCENE_FLAGS_VALIDATION_WARNING  	0x4

/** @def AI_SCENE_FLAGS_NON_VERBOSE_FORMAT
 * This flag is currently only set by the aiProcess_JoinIdenticalVertices step.
 * It indicates that the vertices of the output meshes aren't in the internal
 * verbose format anymore. In the verbose format all vertices are unique,
 * no vertex is ever referenced by more than one face.
 */
#define AI_SCENE_FLAGS_NON_VERBOSE_FORMAT  	0x8

 /** @def AI_SCENE_FLAGS_TERRAIN
 * Denotes pure height-map terrain data. Pure terrains usually consist of quads, 
 * sometimes triangles, in a regular grid. The x,y coordinates of all vertex 
 * positions refer to the x,y coordinates on the terrain height map, the z-axis
 * stores the elevation at a specific point.
 *
 * TER (Terragen) and HMP (3D Game Studio) are height map formats.
 * @note Assimp is probably not the best choice for loading *huge* terrains -
 * fully triangulated data takes extremely much free store and should be avoided
 * as long as possible (typically you'll do the triangulation when you actually
 * need to render it).
 */
#define AI_SCENE_FLAGS_TERRAIN 0x10


// -------------------------------------------------------------------------------
/** The root structure of the imported data. 
 * 
 *  Everything that was imported from the given file can be accessed from here.
 *  Objects of this class are generally maintained and owned by Assimp, not
 *  by the caller. You shouldn't want to instance it, nor should you ever try to
 *  delete a given scene on your own.
 */
// -------------------------------------------------------------------------------
struct aiScene
{

	/** Any combination of the AI_SCENE_FLAGS_XXX flags. By default 
	* this value is 0, no flags are set. Most applications will
	* want to reject all scenes with the AI_SCENE_FLAGS_INCOMPLETE 
	* bit set.
	*/
	unsigned int mFlags;


	/** The root node of the hierarchy. 
	* 
	* There will always be at least the root node if the import
	* was successful (and no special flags have been set). 
	* Presence of further nodes depends on the format and content 
	* of the imported file.
	*/
	C_STRUCT aiNode* mRootNode;



	/** The number of meshes in the scene. */
	unsigned int mNumMeshes;

	/** The array of meshes. 
	*
	* Use the indices given in the aiNode structure to access 
	* this array. The array is mNumMeshes in size. If the
	* AI_SCENE_FLAGS_INCOMPLETE flag is not set there will always 
	* be at least ONE material.
	*/
	C_STRUCT aiMesh** mMeshes;



	/** The number of materials in the scene. */
	unsigned int mNumMaterials;

	/** The array of materials. 
	* 
	* Use the index given in each aiMesh structure to access this
	* array. The array is mNumMaterials in size. If the
	* AI_SCENE_FLAGS_INCOMPLETE flag is not set there will always 
	* be at least ONE material.
	*/
	C_STRUCT aiMaterial** mMaterials;



	/** The number of animations in the scene. */
	unsigned int mNumAnimations; 

	/** The array of animations. 
	*
	* All animations imported from the given file are listed here.
	* The array is mNumAnimations in size.
	*/
	C_STRUCT aiAnimation** mAnimations;



	/** The number of textures embedded into the file */
	unsigned int mNumTextures;

	/** The array of embedded textures.
	* 
	* Not many file formats embed their textures into the file.
	* An example is Quake's MDL format (which is also used by
	* some GameStudio versions)
	*/
	C_STRUCT aiTexture** mTextures;


	/** The number of light sources in the scene. Light sources
	* are fully optional, in most cases this attribute will be 0 
        */
	unsigned int mNumLights;

	/** The array of light sources.
	* 
	* All light sources imported from the given file are
	* listed here. The array is mNumLights in size.
	*/
	C_STRUCT aiLight** mLights;


	/** The number of cameras in the scene. Cameras
	* are fully optional, in most cases this attribute will be 0 
        */
	unsigned int mNumCameras;

	/** The array of cameras.
	* 
	* All cameras imported from the given file are listed here.
	* The array is mNumCameras in size. The first camera in the
	* array (if existing) is the default camera view into
	* the scene.
	*/
	C_STRUCT aiCamera** mCameras;

#ifdef __cplusplus

	//! Default constructor
	aiScene()
	{
		// set all members to zero by default
		mRootNode = NULL;
		mNumMeshes = 0; mMeshes = NULL;
		mNumMaterials = 0; mMaterials = NULL;
		mNumAnimations = 0; mAnimations = NULL;
		mNumTextures = 0; mTextures = NULL;
		mNumCameras = 0; mCameras = NULL;
		mNumLights = 0; mLights = NULL;
		mFlags = 0;
	}

	//! Destructor
	~aiScene()
	{
		// delete all sub-objects recursively
		delete mRootNode;

		// To make sure we won't crash if the data is invalid it's
		// much better to check whether both mNumXXX and mXXX are
		// valid instead of relying on just one of them.
		if (mNumMeshes && mMeshes) 
			for( unsigned int a = 0; a < mNumMeshes; a++)
				delete mMeshes[a];
		delete [] mMeshes;

		if (mNumMaterials && mMaterials) 
			for( unsigned int a = 0; a < mNumMaterials; a++)
				delete mMaterials[a];
		delete [] mMaterials;

		if (mNumAnimations && mAnimations) 
			for( unsigned int a = 0; a < mNumAnimations; a++)
				delete mAnimations[a];
		delete [] mAnimations;

		if (mNumTextures && mTextures) 
			for( unsigned int a = 0; a < mNumTextures; a++)
				delete mTextures[a];
		delete [] mTextures;

		if (mNumLights && mLights) 
			for( unsigned int a = 0; a < mNumLights; a++)
				delete mLights[a];
		delete [] mLights;

		if (mNumCameras && mCameras) 
			for( unsigned int a = 0; a < mNumCameras; a++)
				delete mCameras[a];
		delete [] mCameras;
	}

	//! Check whether the scene contains meshes
	//! Unless no special scene flags are set this will always be true.
	inline bool HasMeshes() const 
		{ return mMeshes != NULL && mNumMeshes > 0; }

	//! Check whether the scene contains materials
	//! Unless no special scene flags are set this will always be true.
	inline bool HasMaterials() const 
		{ return mMaterials != NULL && mNumMaterials > 0; }

	//! Check whether the scene contains lights
	inline bool HasLights() const 
		{ return mLights != NULL && mNumLights > 0; }

	//! Check whether the scene contains textures
	inline bool HasTextures() const 
		{ return mTextures != NULL && mNumTextures > 0; }

	//! Check whether the scene contains cameras
	inline bool HasCameras() const 
		{ return mCameras != NULL && mNumCameras > 0; }

	//! Check whether the scene contains animations
	inline bool HasAnimations() const 
		{ return mAnimations != NULL && mNumAnimations > 0; }

#endif // __cplusplus
};

#ifdef __cplusplus
} //! namespace Assimp
#endif

#endif // __AI_SCENE_H_INC__
