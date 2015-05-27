/*
    3DS model loader
    � Keith O'Conor 2005
    keith.oconor @ {cs.tcd.ie, gmail.com}
*/

#include "model3DS.h"

model3DS::model3DS(){
	hasTexture = false;
}

void model3DS::loadModel(string filename, float scale){
	filename = ofToDataPath(filename);
	loadModel((const char *)filename.c_str(), scale);
}

void model3DS::loadModel(const char* filename, float scale){

	m_filename	= filename;
	m_scale		= scale;

	std::ifstream *modelFile = new std::ifstream(filename,std::ios::in | std::ios::binary | std::ios::ate);

    if(!modelFile->is_open()){
        std::cout<<"[3DS] ERROR: Could not open '"<<filename<<"'"<<std::endl;
        return;
    }
    if(int(modelFile->tellg()) == 0){
        std::cout<<"[3DS] ERROR: Model '"<<filename<<"' is empty"<<std::endl;
        modelFile->close();
        return;
    }

	// Extract path from filename
	int lastSlashPosition=-1, lastForwardSlash=-1, lastBackslash=-1;
	lastForwardSlash = (int)m_filename.find_last_of('/');
	lastBackslash = (int)m_filename.find_last_of('\\');
	if(lastForwardSlash > lastSlashPosition) lastSlashPosition = lastForwardSlash;
	if(lastBackslash > lastSlashPosition) lastSlashPosition = lastBackslash;
	m_filepath = m_filename.substr(0,lastSlashPosition+1);
	m_filename = m_filename.substr(lastSlashPosition+1);

	// Check to make sure file is valid 3DS format (begins with 0x4D4D)
    ushort chunkHeader;
	unsigned int chunkLength;

    modelFile->seekg(0, std::ios::beg);
	modelFile->read((char*)&chunkHeader,2);
	modelFile->read((char*)&chunkLength,4);

	if(chunkHeader != CHUNK_MAIN){
		std::cout<<"[3DS] ERROR: Model '"<<filename<<"' is not a valid 3DS file"<<std::endl;
        modelFile->close();
        return;
	}

	// Detect VBO support
	std::stringstream extStream((const char*)glGetString(GL_EXTENSIONS));
	std::string nextToken;
	bool isVBOSupported=false;
	while(!extStream.eof()){
		extStream >> nextToken;
		if(nextToken == "GL_ARB_vertex_buffer_object"){
			isVBOSupported=true;
			break;
		}
	}
	m_drawMode = DRAW_VERTEX_ARRAY;


	// Initialise bounding box to min & max 4-byte float values
	m_boundingBox.minX = m_boundingBox.minY = m_boundingBox.minZ = 3.4e+38f;
	m_boundingBox.maxX = m_boundingBox.maxY = m_boundingBox.maxZ = 3.4e-38f;

	// Read all 3DS chunks recursively
	
	long end;
	modelFile->seekg (0, ios::end);
	end = modelFile->tellg();
	modelFile->seekg (0, ios::beg);


	while(!modelFile->eof() &&  (end != modelFile->tellg())){
		readChunk(modelFile, modelFile->tellg(), chunkLength);
	}

	m_centerX = (m_boundingBox.minX + m_boundingBox.maxX) / 2.f;
	m_centerY = (m_boundingBox.minY + m_boundingBox.maxY) / 2.f;
	m_centerZ = (m_boundingBox.minZ + m_boundingBox.maxZ) / 2.f;

	// Model loaded, clean up
	modelFile->close();
	delete modelFile;
    std::cout<<"[3DS] Model '"<<filename<<"' loaded"<<std::endl;
}

void model3DS::readChunk(std::ifstream *modelFile, const int objectStart, const int objectLength){
	//std::cout<<std::hex<<"readChunk("<<objectStart<<"-"<<(objectStart+objectLength)<<")"<<std::dec<<std::endl;

	ushort chunkHeader;
	unsigned int chunkLength;

	unsigned long offset;
    ushort numVertices;
	ushort usTemp;
	ushort usTemp1, usTemp2, usTemp3;
	unsigned int uiTemp;
	float vertexX,vertexY,vertexZ;
    int v;
	std::string name;
	char currentLetter;
	unsigned char rgbByte;

    while((modelFile->tellg() < (objectStart + objectLength)) && !modelFile->eof()){

        offset = modelFile->tellg();

		modelFile->read((char*)&chunkHeader, 2);
		modelFile->read((char*)&chunkLength, 4);

        if(DEBUG_OUTPUT) std::cout<<std::hex<<"["<<offset<<"] chunk: 0x"<<chunkHeader<<" ("<<offset<<"-"<<(offset+chunkLength)<<")"<<std::dec<<std::endl;
		switch(chunkHeader){

			  //////////////////
			 // Main chunks
			/////////////////

			case CHUNK_MAIN: continue;

			case CHUNK_3D_EDITOR: continue;

			case CHUNK_OBJECT_BLOCK:
                if(DEBUG_OUTPUT) std::cout<<std::endl<<"[Object block]"<<std::endl;

				m_currentMesh = new mesh3DS(this);
				m_currentMesh->setDrawMode(m_drawMode);

				// Read object name
				do{
					modelFile->read(&currentLetter,1);
					name += currentLetter;
				}while(currentLetter!='\0' && name.length()<20);
				m_currentMesh->setName(name);
				if(DEBUG_OUTPUT) std::cout<<"  Object: "<<name<<std::endl;
				name.erase();

				// Read object sub-chunks
				readChunk(modelFile, modelFile->tellg(), chunkLength - (long(modelFile->tellg()) - offset));

				if(m_currentMesh->getNumFaces() != 0){
					m_currentMesh->buildMesh();
					m_meshes.push_back(*m_currentMesh);
				}
				delete m_currentMesh;
				break;

			  /////////////////////
			 // Geometry chunks
			////////////////////

			case CHUNK_MESH:continue;

            case CHUNK_VERTICES:
                modelFile->read((char*)&numVertices,2);

                for(v=0; v < numVertices*3; v+=3){
                    modelFile->read((char*)&vertexX,4);
					modelFile->read((char*)&vertexY,4);
					modelFile->read((char*)&vertexZ,4);

					// 3DS Max has different axes to OpenGL
					vertexX *= m_scale;
					vertexY *= m_scale;
					vertexZ *= m_scale;

                    m_currentMesh->addVertex(vertexX);// x
                    m_currentMesh->addVertex(vertexY);// y
                    m_currentMesh->addVertex(vertexZ);// z
					
					// Update bounding box
					if(vertexX < m_boundingBox.minX)
						m_boundingBox.minX = vertexX;
					if(vertexY < m_boundingBox.minY)
						m_boundingBox.minY = vertexY;
					if(vertexZ < m_boundingBox.minZ)
						m_boundingBox.minZ = vertexZ;

					if(vertexX > m_boundingBox.maxX)
						m_boundingBox.maxX = vertexX;
					if(vertexY > m_boundingBox.maxY)
						m_boundingBox.maxY = vertexY;
					if(vertexZ > m_boundingBox.maxZ)
						m_boundingBox.maxZ = vertexZ;
                }
                break;

            case CHUNK_TEXCOORDS: // texcoords list
                modelFile->read((char*)&numVertices,2);
                for(v=0; v < numVertices*2; v+=2){
                    modelFile->read((char*)&vertexX,4);
                    modelFile->read((char*)&vertexY,4);
                    m_currentMesh->addTexcoord(vertexX);
					m_currentMesh->addTexcoord(vertexY);
                }
                break;

			case CHUNK_FACES:
                modelFile->read((char*)&m_tempUshort,2);

                for(v=0; v < m_tempUshort*3; v+=3){
                    modelFile->read((char*)&usTemp1,2);
					modelFile->read((char*)&usTemp2,2);
					modelFile->read((char*)&usTemp3,2);
					m_currentMesh->addFaceIndex(usTemp1);
					m_currentMesh->addFaceIndex(usTemp2);
					m_currentMesh->addFaceIndex(usTemp3);

					modelFile->read((char*)&usTemp,2); //face flags
                }

				// Read face sub-chunks
				readChunk(modelFile, modelFile->tellg(), chunkLength - (long(modelFile->tellg()) - offset));
                break;

			case CHUNK_SMOOTHING_GROUP:
				for(v=0; v < m_tempUshort; v++){
					modelFile->read((char*)&uiTemp,4);
					m_currentMesh->addFaceSmoothing(uiTemp);
					//if(DEBUG_OUTPUT) std::cout<<"Smoothing: "<<uiTemp<<std::endl;
				}
				break;

			  /////////////////////
			 // Material chunks
			////////////////////

            case CHUNK_FACE_MATERIAL:
				// Read material name
				do{
					modelFile->read(&currentLetter,1);
					name += currentLetter;
				}while(currentLetter!='\0' && name.length()<20);

				modelFile->read((char*)&m_tempUshort,2);

				for(v=0; v < m_tempUshort; v++){
					modelFile->read((char*)&usTemp,2);
					m_currentMesh->addMaterialFace(name, usTemp);
				}

				name.erase();

				break;

			case CHUNK_MATERIAL_BLOCK:
				if(DEBUG_OUTPUT) std::cout<<std::endl<<"[Material block]"<<std::endl;

				m_currentMaterial = new material3DS();

				// Read material sub-chunks
                readChunk(modelFile, modelFile->tellg(), chunkLength - (long(modelFile->tellg()) - offset));

				m_materials[m_currentMaterial->getName()] = *m_currentMaterial;
				delete m_currentMaterial;
                break;

			case CHUNK_MATERIAL_NAME:
				// Read material name and add to current material
				do{
					modelFile->read(&currentLetter,1);
					name += currentLetter;
				}while(currentLetter!='\0' && name.length()<20);
				m_currentMaterial->setName(name);
				if(DEBUG_OUTPUT) std::cout<<"  Material: "<<m_currentMaterial->getName()<<"("<<m_currentMaterial->getName().size()<<")"<<std::endl;
				name.erase();
				break;

			case CHUNK_TEXTURE_MAP:
			case CHUNK_BUMP_MAP:
				//Read texture name and add to current material
				readChunk(modelFile, modelFile->tellg(), chunkLength - (long(modelFile->tellg()) - offset));
				m_currentMaterial->loadTexture(m_filepath + m_tempString, chunkHeader);
				hasTexture = true;

				break;

			case CHUNK_MAP_FILENAME:
				// Read texture map filename
				m_tempString.erase();
				do{
					modelFile->read(&currentLetter,1);
					m_tempString += currentLetter;
				}while(currentLetter!='\0' && m_tempString.length()<20);
				break;

			case CHUNK_MATERIAL_TWO_SIDED:
				m_currentMaterial->setTwoSided(true);
				break;

			case CHUNK_DIFFUSE_COLOR:
				// Read color sub-chunks
                readChunk(modelFile, modelFile->tellg(), chunkLength - (long(modelFile->tellg()) - offset));
				m_currentMaterial->setDiffuseColor(m_currentColor);

				break;

			case CHUNK_AMBIENT_COLOR:
				// Read color sub-chunks
                readChunk(modelFile, modelFile->tellg(), chunkLength - (long(modelFile->tellg()) - offset));
				m_currentMaterial->setAmbientColor(m_currentColor);

				break;

			case CHUNK_SPECULAR_COLOR:
				// Read color sub-chunks
                readChunk(modelFile, modelFile->tellg(), chunkLength - (long(modelFile->tellg()) - offset));
				m_currentMaterial->setSpecularColor(m_currentColor);

				break;

			case CHUNK_SPECULAR_EXPONENT:
				// Read percent sub-chunk
				readChunk(modelFile, modelFile->tellg(), chunkLength - (long(modelFile->tellg()) - offset));
                m_currentMaterial->setSpecularExponent(m_tempFloat);
				break;

			case CHUNK_SHININESS:
				// Read percent sub-chunk
				readChunk(modelFile, modelFile->tellg(), chunkLength - (long(modelFile->tellg()) - offset));
                m_currentMaterial->setShininess(m_tempFloat);
				break;

			case CHUNK_TRANSPARENCY:
				// Read percent sub-chunk
				readChunk(modelFile, modelFile->tellg(), chunkLength - (long(modelFile->tellg()) - offset));
                m_currentMaterial->setOpacity(1.0f - m_tempFloat);
				break;

			  /////////////////////
			 // Global chunks
			////////////////////

			case CHUNK_RGB_FLOAT:
			case CHUNK_RGB_FLOAT_GAMMA:
				modelFile->read((char*)&m_currentColor[0],4);
				modelFile->read((char*)&m_currentColor[1],4);
				modelFile->read((char*)&m_currentColor[2],4);
				break;

			case CHUNK_RGB_BYTE:
			case CHUNK_RGB_BYTE_GAMMA:
				modelFile->read((char*)&rgbByte,1); m_currentColor[0]=float(rgbByte)/255.f;
				modelFile->read((char*)&rgbByte,1); m_currentColor[1]=float(rgbByte)/255.f;
				modelFile->read((char*)&rgbByte,1); m_currentColor[2]=float(rgbByte)/255.f;
				break;

			case CHUNK_PERCENT_INT:
				modelFile->read((char*)&usTemp,2);
				m_tempFloat = usTemp / 100.f;
				break;

			case CHUNK_PERCENT_FLOAT:
				modelFile->read((char*)&m_tempFloat,4);
				m_tempFloat /= 100.f;
				break;

			default:break; // any other chunk

		}

		// Go to the next chunk's header (if any left in object)
		modelFile->seekg(offset + chunkLength, std::ios::beg);
	}

}

void material3DS::loadTexture(std::string filename, int chunkType){

	string lowerCaseStr = filename;
	std::transform(lowerCaseStr.begin(),lowerCaseStr.end(),lowerCaseStr.begin(), ::tolower);

	if((lowerCaseStr.find(".jpg") == std::string::npos) && (lowerCaseStr.find(".png") == std::string::npos) && (lowerCaseStr.find(".tga") == std::string::npos) && (lowerCaseStr.find(".bmp") == std::string::npos)){
		std::cout<<"[3DS] WARNING: Could not load map '"<<filename<<"'\n[3DS] WARNING: (texture must be TGA, PNG, JPG or BMP)"<<std::endl;
		return;
	}

	GLuint newTextureId;
	glGenTextures(1, &newTextureId);

    texture3DS newTexture(filename, newTextureId);

	switch(chunkType){
		case CHUNK_TEXTURE_MAP:
			m_textureMapId = newTextureId;
			m_hasTextureMap = true;
			break;
		case CHUNK_BUMP_MAP:
			m_bumpMapId = newTextureId;
			m_hasBumpMap = true;
			break;
	}

}

void mesh3DS::buildMesh(){
	calculateNormals();
	sortFacesByMaterial();

}

void mesh3DS::calculateNormals(){

	// Doesn't take smoothing groups into account yet

	if(DEBUG_OUTPUT) std::cout<<"Calculating normals... ";
	m_normals.assign(m_vertices.size(), 0.0f);

	Vertex vtx1, vtx2, vtx3;
	Vector3DS v1, v2, faceNormal;

	for(int face=0; face < int(m_faces.size()); face+=3){
		// Calculate face normal
		vtx1.set(m_vertices[m_faces[face]*3], m_vertices[(m_faces[face]*3)+1], m_vertices[(m_faces[face]*3)+2]);
		vtx2.set(m_vertices[m_faces[face+1]*3], m_vertices[(m_faces[face+1]*3)+1], m_vertices[(m_faces[face+1]*3)+2]);
		vtx3.set(m_vertices[m_faces[face+2]*3], m_vertices[(m_faces[face+2]*3)+1], m_vertices[(m_faces[face+2]*3)+2]);

		v1 = vtx2 - vtx1;
		v2 = vtx3 - vtx1;

		faceNormal = v1.crossProduct(v2);

		// Add normal to all three vertex normals
		m_normals[m_faces[face]*3] += faceNormal.x;
		m_normals[(m_faces[face]*3)+1] += faceNormal.y;
		m_normals[(m_faces[face]*3)+2] += faceNormal.z;

		m_normals[m_faces[face+1]*3] += faceNormal.x;
		m_normals[(m_faces[face+1]*3)+1] += faceNormal.y;
		m_normals[(m_faces[face+1]*3)+2] += faceNormal.z;

		m_normals[m_faces[face+2]*3] += faceNormal.x;
		m_normals[(m_faces[face+2]*3)+1] += faceNormal.y;
		m_normals[(m_faces[face+2]*3)+2] += faceNormal.z;

	}

	//normalize all normals
	for(int n=0; n < int(m_normals.size()); n+=3){
		faceNormal.set(m_normals[n], m_normals[n+1], m_normals[n+2]);
		faceNormal.normalize();
		m_normals[n] = faceNormal.x;
		m_normals[n+1] = faceNormal.y;
		m_normals[n+2] = faceNormal.z;
	}

	if(DEBUG_OUTPUT) std::cout<<"done"<<std::endl;
}

void mesh3DS::sortFacesByMaterial(){

	assert(getNumFaces()!=0);
	assert(m_parentModel!=NULL);

	std::vector<ushort> newMatFaces;

	// mark each face off as assigned to a material so
	// we can figure out which faces have no material
	std::vector<bool> assignedFaces;
	std::vector<bool>::iterator assignedFacesIter;
	assignedFaces.assign(m_faces.size() / 3, false);

	// loop over each material
	std::map<std::string, std::vector<ushort> >::iterator matFacesIter;
	for(matFacesIter=m_materialFaces.begin(); matFacesIter!=m_materialFaces.end(); ++matFacesIter){
		//std::cout<<"    Faces in material '"<<matFacesIter->first<<"': "<<matFacesIter->second.size()<<std::endl;

		// loop over all the faces with that material
		std::vector<ushort>::iterator facesIter;
		for(facesIter=matFacesIter->second.begin(); facesIter!=matFacesIter->second.end(); ++facesIter){
			newMatFaces.push_back(m_faces[((*facesIter)*3)]);
			newMatFaces.push_back(m_faces[((*facesIter)*3)+1]);
			newMatFaces.push_back(m_faces[((*facesIter)*3)+2]);
			assignedFaces[*facesIter]=true;
		}

		//replace the material's face indices with the actual face vertex indices
		m_materialFaces[matFacesIter->first].assign(newMatFaces.begin(),newMatFaces.end());
		newMatFaces.clear();
	}

	// Make a default material and assign any unused faces to it
	int numUnassignedFaces=0;
	for(assignedFacesIter=assignedFaces.begin(); assignedFacesIter!=assignedFaces.end(); ++assignedFacesIter){
		if(*assignedFacesIter == false){
			numUnassignedFaces++;
			//assign face to default material
		}
	}
	//std::cout<<"numUnassignedFaces: "<<numUnassignedFaces<<std::endl;
}

void mesh3DS::draw(){

	assert(getNumFaces()!=0);

    int face, numFaces, vertexIndex, texcoordIndex;
	GLuint materialFaces; //GL_FRONT or GL_FRONT_AND_BACK

	std::map<std::string, std::vector<ushort> >::iterator materialsIter;
	for(materialsIter=m_materialFaces.begin(); materialsIter!=m_materialFaces.end(); ++materialsIter){
		const material3DS& currentMaterial = m_parentModel->getMaterial(materialsIter->first);

		// Bind texture map (if any)
		bool hasTextureMap = currentMaterial.hasTextureMap();
		if(hasTextureMap) {
			glBindTexture(GL_TEXTURE_2D, currentMaterial.getTextureMapId());
		}
		else 
			glBindTexture(GL_TEXTURE_2D, 0);

		const GLfloat *specular = currentMaterial.getSpecularColor();
		float shininess = currentMaterial.getShininess();
		float adjustedSpecular[4] = {specular[0]*shininess, specular[1]*shininess, specular[2]*shininess, 1};

#ifndef TARGET_OPENGLES
		glPushAttrib(GL_LIGHTING_BIT);
#endif

#ifdef TARGET_OPENGLES
		/* In OpenGL ES, the GL_FRONT_AND_BACK flag is the only flag that can be passed */
		materialFaces = GL_FRONT_AND_BACK;
#else
		if(currentMaterial.isTwoSided()){
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
			materialFaces = GL_FRONT_AND_BACK;
		}
		else{
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
			materialFaces = GL_FRONT;
		}
#endif

		// Apply material colors
		if(glIsEnabled(GL_LIGHTING)){
			//const GLfloat matZero[4]={0,0,0,0};
			const GLfloat matOne[4]={1,1,1,1};
			if(hasTextureMap){ //replace color with texture, but keep lighting contribution
				glMaterialfv(materialFaces, GL_DIFFUSE, matOne);
			}
			else glMaterialfv(materialFaces, GL_DIFFUSE, currentMaterial.getDiffuseColor());
			glMaterialfv(materialFaces, GL_AMBIENT, currentMaterial.getAmbientColor());
			glMaterialfv(materialFaces, GL_SPECULAR, adjustedSpecular);
			glMaterialf(materialFaces, GL_SHININESS, 128.f * currentMaterial.getSpecularExponent());
		}
		else
		{
			const float* color = currentMaterial.getDiffuseColor();
			glColor4f(color[0], color[1], color[2], 1.0f);
		}

		const std::vector<ushort> *currentMatFaces = &(materialsIter->second);
		numFaces = (int)currentMatFaces->size(); //number of faces in this material

		switch(m_drawMode){
			case DRAW_IMMEDIATE_MODE:
				// This is like DRAW_VERTEX_ARRAY, but the array is not already constructed, so we need to do it here
				for(face=0; face<numFaces; face+=3){
					int vertexIndices[] = {
						(*currentMatFaces)[face]*3,
						(*currentMatFaces)[face+1]*3,
						(*currentMatFaces)[face+2]*3
					};
					float vertex_data[] = {
						m_normals[vertexIndices[0]], m_normals[vertexIndices[0]+1], m_normals[vertexIndices[0]+2],
						m_normals[vertexIndices[1]], m_normals[vertexIndices[1]+1], m_normals[vertexIndices[1]+2],
						m_normals[vertexIndices[2]], m_normals[vertexIndices[2]+1], m_normals[vertexIndices[2]+2]
						
					};
					float normal_data[] = {
						m_vertices[vertexIndices[0]], m_vertices[vertexIndices[0]+1], m_vertices[vertexIndices[0]+2],
						m_vertices[vertexIndices[1]], m_vertices[vertexIndices[1]+1], m_vertices[vertexIndices[1]+2],
						m_vertices[vertexIndices[2]], m_vertices[vertexIndices[2]+1], m_vertices[vertexIndices[2]+2]
						
					};
					
					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_NORMAL_ARRAY);
					if (hasTextureMap) {
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					}

					glVertexPointer(3, GL_FLOAT, 0, vertex_data);
					glNormalPointer(GL_FLOAT, 0, normal_data);

					if (hasTextureMap) {
						int texCoordIndices[] = {
							(*currentMatFaces)[face]*2,
							(*currentMatFaces)[face+1]*2,
							(*currentMatFaces)[face+2]*2
						};
						float texcoord_data[] = {
							m_texcoords[texCoordIndices[0]], m_texcoords[texCoordIndices[0]+1],
							m_texcoords[texCoordIndices[1]], m_texcoords[texCoordIndices[1]+1],
							m_texcoords[texCoordIndices[2]], m_texcoords[texCoordIndices[2]+1]
						};

						glTexCoordPointer(2, GL_FLOAT, 0, texcoord_data);
						// don't move this statement out of the block since texcoord_data will be out of scope otherwise
						glDrawArrays(GL_TRIANGLES, 0, 3);
					}
					else
					{
						glDrawArrays(GL_TRIANGLES, 0, 3);
					}

					glDisableClientState( GL_VERTEX_ARRAY );
					glDisableClientState( GL_NORMAL_ARRAY );
					if(hasTextureMap){
						glDisableClientState( GL_TEXTURE_COORD_ARRAY );
					}
				}
				break;
			case DRAW_VERTEX_ARRAY:

				glEnableClientState( GL_VERTEX_ARRAY );
				glEnableClientState( GL_NORMAL_ARRAY );

				if(hasTextureMap){
					glTexCoordPointer( 2, GL_FLOAT, 0, &m_texcoords[0] );
					glEnableClientState( GL_TEXTURE_COORD_ARRAY );
				}

				glVertexPointer( 3, GL_FLOAT, 0, &m_vertices[0] );
				glNormalPointer(GL_FLOAT, 0, &m_normals[0] );
				glDrawElements(GL_TRIANGLES, numFaces, GL_UNSIGNED_SHORT, &(materialsIter->second[0]));

				glDisableClientState( GL_VERTEX_ARRAY );
				glDisableClientState( GL_NORMAL_ARRAY );
				if(hasTextureMap){
					glDisableClientState( GL_TEXTURE_COORD_ARRAY );
				}

				break;


			case DRAW_VBO:

				glGenBuffers( 1 , &m_verticesArrayId );
				glBindBuffer( GL_ARRAY_BUFFER , m_verticesArrayId );
				glBufferData( GL_ARRAY_BUFFER , m_vertices.size() * sizeof( float ) , &m_vertices[0] , GL_STATIC_DRAW );

				glGenBuffers( 1 , &m_normalsArrayId );
				glBindBuffer( GL_ARRAY_BUFFER , m_normalsArrayId );
				glBufferData( GL_ARRAY_BUFFER , m_normals.size() * sizeof( float ) , &m_normals[0] , GL_STATIC_DRAW );

				if(hasTextureMap){
					glGenBuffers( 1 , &m_texcoordsArrayId );
					glBindBuffer( GL_ARRAY_BUFFER , m_texcoordsArrayId );
					glBufferData( GL_ARRAY_BUFFER , m_texcoords.size() * sizeof( float ) , &m_texcoords[0] , GL_STATIC_DRAW );
				}

				//vPos = getAttribLocation( 

				break;

			default:
				std::cout<<"[3DS] ERROR: Invalid mesh draw mode specified"<<std::endl;
				break;
		}
#ifndef TARGET_OPENGLES
		glPopAttrib(); // GL_LIGHTING_BIT
#endif
	}
}


void model3DS::draw(){
	std::vector<mesh3DS>::iterator meshIter;
	for(meshIter = m_meshes.begin(); meshIter != m_meshes.end(); meshIter++)
		meshIter->draw( );
}

