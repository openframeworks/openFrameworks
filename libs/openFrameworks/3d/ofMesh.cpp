#include "ofMesh.h"
#include "ofGraphics.h"

//--------------------------------------------------------------
ofMesh::ofMesh(){
	mode = OF_PRIMITIVE_TRIANGLES;
	bVertsChanged = false;
	bColorsChanged = false;
	bNormalsChanged = false;
	bTexCoordsChanged = false;
	bIndicesChanged = false;
    useColors = true;
    useTextures = true;
    useNormals = true;
    useIndices = true;

}

//--------------------------------------------------------------
ofMesh::ofMesh(ofPrimitiveMode mode, const vector<ofVec3f>& verts){
	setMode(mode);
	addVertices(verts);
}

//--------------------------------------------------------------
ofMesh::~ofMesh(){
}

//--------------------------------------------------------------
void ofMesh::clear(){
	if(!vertices.empty()){
		bVertsChanged = true;
		vertices.clear();
	}
	if(!colors.empty()){
		bColorsChanged = true;
		colors.clear();
	}
	if(!normals.empty()){
		bNormalsChanged = true;
		normals.clear();
	}
	if(!texCoords.empty()){
		bTexCoordsChanged = true;
		texCoords.clear();
	}
	if(!indices.empty()){
		bIndicesChanged = true;
		indices.clear();
	}

}

//--------------------------------------------------------------
bool ofMesh::haveVertsChanged(){
	if(bVertsChanged){
		bVertsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::haveColorsChanged(){
	if(bColorsChanged){
		bColorsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::haveNormalsChanged(){
	if(bNormalsChanged){
		bNormalsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::haveTexCoordsChanged(){
	if(bTexCoordsChanged){
		bTexCoordsChanged = false;
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------
bool ofMesh::haveIndicesChanged(){
	if(bIndicesChanged){
		bIndicesChanged = false;
		return true;
	}else{
		return false;
	}
}


//--------------------------------------------------------------
bool ofMesh::hasVertices(){
	return !vertices.empty();
}

//--------------------------------------------------------------
bool ofMesh::hasColors(){
	return !colors.empty();
}

//--------------------------------------------------------------
bool ofMesh::hasNormals(){
	return !normals.empty();
}

//--------------------------------------------------------------
bool ofMesh::hasTexCoords(){
	return !texCoords.empty();
}

//--------------------------------------------------------------
bool ofMesh::hasIndices(){
	return !indices.empty();
}

//ADDERS

//--------------------------------------------------------------
void ofMesh::addVertex(const ofVec3f& v){
	vertices.push_back(v);
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addVertices(const vector<ofVec3f>& verts){
	vertices.insert(vertices.end(),verts.begin(),verts.end());
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addVertices(const ofVec3f* verts, int amt){
	vertices.insert(vertices.end(),verts,verts+amt);
	bVertsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addColor(const ofFloatColor& c){
	colors.push_back(c);
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addColors(const vector<ofFloatColor>& cols){
	colors.insert(colors.end(),cols.begin(),cols.end());
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addColors(const ofFloatColor* cols, int amt){
	colors.insert(colors.end(),cols,cols+amt);
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addNormal(const ofVec3f& n){
	normals.push_back(n);
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addNormals(const vector<ofVec3f>& norms){
	normals.insert(normals.end(),norms.begin(),norms.end());
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addNormals(const ofVec3f* norms, int amt){
	normals.insert(normals.end(),norms,norms+amt);
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addTexCoord(const ofVec2f& t){
	//TODO: figure out if we add to all other arrays to match
	texCoords.push_back(t);
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addTexCoords(const vector<ofVec2f>& tCoords){
	texCoords.insert(texCoords.end(),tCoords.begin(),tCoords.end());
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addTexCoords(const ofVec2f* tCoords, int amt){
	texCoords.insert(texCoords.end(),tCoords,tCoords+amt);
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
ofIndexType ofMesh::getIndex(ofIndexType i) const{
	return indices[i];
}

//--------------------------------------------------------------
void ofMesh::addIndex(ofIndexType i){
	indices.push_back(i);
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addIndices(const vector<ofIndexType>& inds){
	indices.insert(indices.end(),inds.begin(),inds.end());
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addIndices(const ofIndexType* inds, int amt){
	indices.insert(indices.end(),inds,inds+amt);
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::addTriangle(ofIndexType index1, ofIndexType index2, ofIndexType index3) {
    addIndex(index1);
    addIndex(index2);
    addIndex(index3);
}

//REMOVERS
//--------------------------------------------------------------
void ofMesh::removeVertex(ofIndexType index){
  if(index >= vertices.size()){
    ofLog(OF_LOG_ERROR,"Trying to remove vertex out of range of this mesh. Taking no action.");
  }else{
    vertices.erase(vertices.begin() + index);
    bVertsChanged = true;
  }
}

//--------------------------------------------------------------
void ofMesh::removeNormal(ofIndexType index){
  if(index >= vertices.size()){
    ofLog(OF_LOG_ERROR,"Trying to remove normal out of range of this mesh. Taking no action.");
  }else{
    normals.erase(normals.begin() + index);
    bNormalsChanged = true;
  }
}

//--------------------------------------------------------------
void ofMesh::removeColor(ofIndexType index){
  if(index >= vertices.size()){
    ofLog(OF_LOG_ERROR,"Trying to remove color out of range of this mesh. Taking no action.");
  }else{
    colors.erase(colors.begin() + index);
    bColorsChanged = true;
  }
}

//--------------------------------------------------------------
void ofMesh::removeTexCoord(ofIndexType index){
  if(index >= vertices.size()){
    ofLog(OF_LOG_ERROR,"Trying to remove texCoord out of range of this mesh. Taking no action.");
  }else{
    texCoords.erase(texCoords.begin() + index);
    bTexCoordsChanged = true;
  }
}

//--------------------------------------------------------------
void ofMesh::removeIndex(ofIndexType index){
  if(index >= vertices.size()){
    ofLog(OF_LOG_ERROR,"Trying to remove index out of range of this mesh. Taking no action.");
  }else{
    indices.erase(indices.begin() + index);
    bIndicesChanged = true;
  }
}


//GETTERS
//--------------------------------------------------------------
ofPrimitiveMode ofMesh::getMode() const{
	return mode;
}

//--------------------------------------------------------------
ofVec3f ofMesh::getVertex(ofIndexType i) const{
	return vertices[i];
}

//--------------------------------------------------------------
ofVec3f ofMesh::getNormal(ofIndexType i) const{
	return normals[i];
}

//--------------------------------------------------------------
ofFloatColor ofMesh::getColor(ofIndexType i) const{
	return colors[i];
}

//--------------------------------------------------------------
ofVec2f ofMesh::getTexCoord(ofIndexType i) const{
	return texCoords[i];
}

//--------------------------------------------------------------
int ofMesh::getNumVertices() const{
	return (int)vertices.size();
}

//--------------------------------------------------------------
int ofMesh::getNumColors() const{
	return (int)colors.size();
}

//--------------------------------------------------------------
int ofMesh::getNumNormals() const{
	return (int)normals.size();
}

//--------------------------------------------------------------
int ofMesh::getNumTexCoords() const{
	return (int)texCoords.size();
}

//--------------------------------------------------------------
int ofMesh::getNumIndices() const{
	return (int)indices.size();
}

/*
//--------------------------------------------------------------
int ofPrimitive::getNumIndicesSolid(){
	return indicesSolid.size();
}

//--------------------------------------------------------------
int ofPrimitive::getNumIndicesWire(){
	return indicesWire.size();
}
 */

//--------------------------------------------------------------
ofVec3f* ofMesh::getVerticesPointer(){
#ifdef TARGET_OSX
	return &vertices[0];
#else
	return vertices.data();
#endif
}

//--------------------------------------------------------------
ofFloatColor* ofMesh::getColorsPointer(){
#ifdef TARGET_OSX
	return &colors[0];
#else
	return colors.data();
#endif
}

//--------------------------------------------------------------
ofVec3f* ofMesh::getNormalsPointer(){
#ifdef TARGET_OSX
		return &normals[0];
#else
	return normals.data();
#endif
}

//--------------------------------------------------------------
ofVec2f* ofMesh::getTexCoordsPointer(){
#ifdef TARGET_OSX
		return &texCoords[0];
#else
	return texCoords.data();
#endif
}

//--------------------------------------------------------------
ofIndexType* ofMesh::getIndexPointer(){
#ifdef TARGET_OSX
		return &indices[0];
#else
	return indices.data();
#endif
}


//--------------------------------------------------------------
const ofVec3f* ofMesh::getVerticesPointer() const{
#ifdef TARGET_OSX
	return &vertices[0];
#else
	return vertices.data();
#endif
}

//--------------------------------------------------------------
const ofFloatColor* ofMesh::getColorsPointer() const{
#ifdef TARGET_OSX
	return &colors[0];
#else
	return colors.data();
#endif
}

//--------------------------------------------------------------
const ofVec3f* ofMesh::getNormalsPointer() const{
#ifdef TARGET_OSX
	return &normals[0];
#else
	return normals.data();
#endif
}

//--------------------------------------------------------------
const ofVec2f* ofMesh::getTexCoordsPointer() const{
#ifdef TARGET_OSX
	return &texCoords[0];
#else
	return texCoords.data();
#endif
}

//--------------------------------------------------------------
const ofIndexType * ofMesh::getIndexPointer() const{
#ifdef TARGET_OSX
	return &indices[0];
#else
	return indices.data();
#endif
}

vector<ofVec3f> & ofMesh::getVertices(){
	bVertsChanged = true;
	return vertices;
}

vector<ofFloatColor> & ofMesh::getColors(){
	bColorsChanged = true;
	return colors;
}

vector<ofVec3f> & ofMesh::getNormals(){
	bNormalsChanged = true;
	return normals;
}

vector<ofVec2f> & ofMesh::getTexCoords(){
	bTexCoordsChanged = true;
	return texCoords;
}

vector<ofIndexType> & ofMesh::getIndices(){
	bIndicesChanged = true;
	return indices;
}

const vector<ofVec3f> & ofMesh::getVertices() const{
	return vertices;
}

const vector<ofFloatColor> & ofMesh::getColors() const{
	return colors;
}

const vector<ofVec3f> & ofMesh::getNormals() const{
	return normals;
}

const vector<ofVec2f> & ofMesh::getTexCoords() const{
	return texCoords;
}

const vector<ofIndexType> & ofMesh::getIndices() const{
	return indices;
}

/*
//--------------------------------------------------------------
GLuint* ofPrimitive::getSolidIndexPointer(){
	return &indicesSolid[0];
}

//--------------------------------------------------------------
GLuint* ofPrimitive::getWireIndexPointer(){
	return &indicesWire[0];
}
 */

/*
//--------------------------------------------------------------
vector<int>& ofPrimitive::getFace(int faceNum){
	switch(mode){
		//GL_QUADS
		indices[faceNum*4+0];
		indices[faceNum*4+1];
		indices[faceNum*4+2];
		indices[faceNum*4+3];

		//GL_TRIANGLES
		indices[faceNum*3+0];
		indices[faceNum*3+1];
		indices[faceNum*3+2];

		//GL_TRIANGLE_FAN
		// 1 element per fan
		indices[0];
		indices[faceNum+1];
		indices[faceNum+2];

		//GL_TRIANGLE_STRIP
		// 1 element per strip
		indices[faceNum+0];
		indices[faceNum+1];
		indices[faceNum+2];
		default:break;
	}
}
 */


//--------------------------------------------------------------
ofVec3f ofMesh::getCentroid() const {
	if(vertices.size() == 0) {
		ofLogWarning() << "Called ofMesh::getCentroid() on mesh with zero vertices, returned ofPoint(0, 0, 0)";
		return ofPoint(0, 0, 0);
	}

	ofVec3f sum;
	for(unsigned int i = 0; i < vertices.size(); i++) {
		sum += vertices[i];
	}
	sum /= vertices.size();
	return sum;
}

//SETTERS
//--------------------------------------------------------------
void ofMesh::setMode(ofPrimitiveMode m){
	bIndicesChanged = true;
	mode = m;
}

//--------------------------------------------------------------
void ofMesh::setVertex(ofIndexType index, const ofVec3f& v){
	vertices[index] = v;
	bVertsChanged = true;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::setNormal(ofIndexType index, const ofVec3f& n){
	normals[index] = n;
	bNormalsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::setColor(ofIndexType index, const ofFloatColor& c){
	colors[index] = c;
	bColorsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::setTexCoord(ofIndexType index, const ofVec2f& t){
	texCoords[index] = t;
	bTexCoordsChanged = true;
}

//--------------------------------------------------------------
void ofMesh::setIndex(ofIndexType index, ofIndexType  val){
	indices[index] = val;
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::setName(string name_){
	name = name_;
}

//--------------------------------------------------------------
void ofMesh::setupIndicesAuto(){
	bIndicesChanged = true;
	indices.clear();
	indices.resize(vertices.size());
	for(int i = 0; i < (int)vertices.size();i++){
		indices[i]=(ofIndexType)i;
	}
}



//--------------------------------------------------------------
void ofMesh::clearVertices(){
	vertices.clear();
	bVertsChanged=true;
}

//--------------------------------------------------------------
void ofMesh::clearNormals(){
	normals.clear();
	bNormalsChanged=true;
}

//--------------------------------------------------------------
void ofMesh::clearColors(){
	colors.clear();
	bColorsChanged=true;
}

//--------------------------------------------------------------
void ofMesh::clearTexCoords(){
	texCoords.clear();
	bTexCoordsChanged=true;
}

//--------------------------------------------------------------
void ofMesh::clearIndices(){
	indices.clear();
	bIndicesChanged = true;
}

//--------------------------------------------------------------
void ofMesh::drawVertices(){
	draw(OF_MESH_POINTS);
}

//--------------------------------------------------------------
void ofMesh::drawWireframe(){
	draw(OF_MESH_WIREFRAME);
}

//--------------------------------------------------------------
void ofMesh::drawFaces(){
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void ofMesh::draw(){
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void ofMesh::draw(ofPolyRenderMode renderType){
	ofGetCurrentRenderer()->draw(*this,renderType,useColors,useTextures,useNormals);
}

//--------------------------------------------------------------
void ofMesh::enableColors(){
    useColors = true;
}

//--------------------------------------------------------------
void ofMesh::enableTextures(){
    useTextures = true;
}

//--------------------------------------------------------------
void ofMesh::enableNormals(){
    useNormals = true;
}

//--------------------------------------------------------------
void ofMesh::enableIndices(){
	useIndices = true;
}

//--------------------------------------------------------------
void ofMesh::disableColors(){
    useColors = false;
}

//--------------------------------------------------------------
void ofMesh::disableTextures(){
    useTextures = false;
}

//--------------------------------------------------------------
void ofMesh::disableNormals(){
    useNormals = false;
}

//--------------------------------------------------------------
void ofMesh::disableIndices(){
	useIndices = false;
}

//--------------------------------------------------------------
bool ofMesh::usingColors(){
    return useColors;
}

//--------------------------------------------------------------
bool ofMesh::usingTextures(){
    return useTextures;
}

//--------------------------------------------------------------
bool ofMesh::usingNormals(){
    return useNormals;
}

//--------------------------------------------------------------
bool ofMesh::usingIndices(){
    return useIndices;
}


//--------------------------------------------------------------
void ofMesh::load(string path){
	ofFile is(path, ofFile::ReadOnly);
	ofMesh& data = *this;

	string line;
	string error;
	ofBuffer buffer(is);
	ofMesh backup = data;

	int orderVertices=-1;
	int orderIndices=-1;

	int vertexCoordsFound=0;
	int colorCompsFound=0;
	int texCoordsFound=0;
	int normalsCoordsFound=0;

	int currentVertex = 0;
	int currentNormal = 0;
	int currentFace = 0;
	
	bool floatColor = false;

	enum State{
		Header,
		VertexDef,
		FaceDef,
		Vertices,
		Normals,
		Faces
	};

	data.clear();
	State state = Header;

	int lineNum = 0;

	line = buffer.getFirstLine();
	lineNum++;
	if(line!="ply"){
		error = "wrong format, expecting 'ply'";
		goto clean;
	}

	line = buffer.getNextLine();
	lineNum++;
	if(line!="format ascii 1.0"){
		error = "wrong format, expecting 'format ascii 1.0'";
		goto clean;
	}

	while(!buffer.isLastLine()){
		line = buffer.getNextLine();
		lineNum++;
		if(line.find("comment")==0){
			continue;
		}

		if((state==Header || state==FaceDef) && line.find("element vertex")==0){
			state = VertexDef;
			orderVertices = MAX(orderIndices, 0)+1;
			data.getVertices().resize(ofToInt(line.substr(15)));
			continue;
		}

		if((state==Header || state==VertexDef) && line.find("element face")==0){
			state = FaceDef;
			orderIndices = MAX(orderVertices, 0)+1;
			data.getIndices().resize(ofToInt(line.substr(13))*3);
			continue;
		}

		if(state==VertexDef && (line.find("property float x")==0 || line.find("property float y")==0 || line.find("property float z")==0)){
			vertexCoordsFound++;
			continue;
		}

		if(state==VertexDef && (line.find("property float r")==0 || line.find("property float g")==0 || line.find("property float b")==0 || line.find("property float a")==0)){
			colorCompsFound++;
			data.getColors().resize(data.getVertices().size());
			floatColor = true;
			continue;
		}

		if(state==VertexDef && (line.find("property uchar red")==0 || line.find("property uchar green")==0 || line.find("property uchar blue")==0 || line.find("property uchar alpha")==0)){
			colorCompsFound++;
			data.getColors().resize(data.getVertices().size());
			floatColor = false;
			continue;
		}

		if(state==VertexDef && (line.find("property float u")==0 || line.find("property float v")==0)){
			texCoordsFound++;
			data.getTexCoords().resize(data.getVertices().size());
			continue;
		}

		if(state==VertexDef && (line.find("property float nx")==0 || line.find("property float ny")==0 || line.find("property float nz")==0)){
			normalsCoordsFound++;
			if (normalsCoordsFound==3) data.getNormals().resize(data.getVertices().size());
			continue;
		}

		if(state==FaceDef && line.find("property list")!=0 && line!="end_header"){
			error = "wrong face definition";
			goto clean;
		}

		if(line=="end_header"){
			if(data.hasColors() && colorCompsFound!=3 && colorCompsFound!=4){
				error =  "data has color coordiantes but not correct number of components. Found " + ofToString(colorCompsFound) + " expecting 3 or 4";
				goto clean;
			}
			if(data.hasNormals() && normalsCoordsFound!=3){
				error = "data has normal coordiantes but not correct number of components. Found " + ofToString(normalsCoordsFound) + " expecting 3";
				goto clean;
			}
			if(!data.hasVertices()){
				ofLogWarning() << "mesh without vertices";
			}
			if(orderVertices==-1) orderVertices=9999;
			if(orderIndices==-1) orderIndices=9999;

			if(orderVertices < orderIndices){
				state = Vertices;
			}else {
				state = Faces;
			}
			continue;
		}

		if(state==Vertices){
			stringstream sline;
			sline.str(line);
			ofVec3f v;
			sline >> v.x;
			sline >> v.y;
			if(vertexCoordsFound>2) sline >> v.z;
			data.getVertices()[currentVertex] = v;

			if(colorCompsFound>0){
				if (floatColor){
					ofFloatColor c;
					sline >> c.r;
					sline >> c.g;
					sline >> c.b;
					if(colorCompsFound>3) sline >> c.a;
					data.getColors()[currentVertex] = c;
				}else{
					int r, g, b, a = 255;
					sline >> r;
					sline >> g;
					sline >> b;
					if(colorCompsFound>3) sline >> a;
					data.getColors()[currentVertex] = ofColor(r, g, b, a);
				}
			}

			if(texCoordsFound>0){
				ofVec2f uv;
				sline >> uv.x;
				sline >> uv.y;
				data.getTexCoords()[currentVertex] = uv;
			}
			
			if (normalsCoordsFound>0){
				ofVec3f n;
				sline >> n.x;
				sline >> n.y;
				sline >> n.z;
				data.getNormals()[currentVertex] = n;
			}
			
			currentVertex++;
			if(currentVertex==data.getNumVertices()){
				if(orderVertices<orderIndices){
					state = Faces;
				}else{
					state = Vertices;
				}
			}
			continue;
		}

		if(state==Faces){
			stringstream sline;
			sline.str(line);
			int numV;
			sline >> numV;
			if(numV!=3){
				error = "face not a triangle";
				goto clean;
			}
			int i;
			sline >> i;
			data.getIndices()[currentFace*3] = i;
			sline >> i;
			data.getIndices()[currentFace*3+1] = i;
			sline >> i;
			data.getIndices()[currentFace*3+2] = i;

			currentFace++;
			if(currentFace==data.getNumIndices()/3){
				if(orderVertices<orderIndices){
					state = Vertices;
				}else{
					state = Faces;
				}
			}
			continue;
		}
	}


	return;
	clean:
	ofLogError() << lineNum << ":" << error;
	ofLogError() << "\"" << line << "\"";
	data = backup;
}

void ofMesh::save(string path, bool useBinary){
	ofFile os(path, ofFile::WriteOnly);
	ofMesh& data = *this;

	os << "ply" << endl;
	if(useBinary) {
		os << "format binary_little_endian 1.0" << endl;
	} else {
		os << "format ascii 1.0" << endl;
	}

	if(data.getNumVertices()){
		os << "element vertex " << data.getNumVertices() << endl;
		os << "property float x" << endl;
		os << "property float y" << endl;
		os << "property float z" << endl;
		if(data.getNumColors()){
			os << "property uchar red" << endl;
			os << "property uchar green" << endl;
			os << "property uchar blue" << endl;
			os << "property uchar alpha" << endl;
		}
		if(data.getNumTexCoords()){
			os << "property float u" << endl;
			os << "property float v" << endl;
		}
		if(data.getNumNormals()){
			os << "property float nx" << endl;
			os << "property float ny" << endl;
			os << "property float nz" << endl;
		}
	}

	unsigned char faceSize = 3;
	if(data.getNumIndices()){
		os << "element face " << data.getNumIndices() / faceSize << endl;
		os << "property list uchar int vertex_indices" << endl;
	} else if(data.getMode() == OF_PRIMITIVE_TRIANGLES) {
		os << "element face " << data.getNumVertices() / faceSize << endl;
		os << "property list uchar int vertex_indices" << endl;
	}

	os << "end_header" << endl;

	for(int i = 0; i < data.getNumVertices(); i++){
		if(useBinary) {
			os.write((char*) &data.getVertices()[i], sizeof(ofVec3f));
		} else {
			os << data.getVertex(i).x << " " << data.getVertex(i).y << " " << data.getVertex(i).z;
		}
		if(data.getNumColors()){
			// VCG lib / MeshLab don't support float colors, so we have to cast
			ofColor cur = data.getColors()[i];
			if(useBinary) {
				os.write((char*) &cur, sizeof(ofColor));
			} else {
				os << " " << (int) cur.r << " " << (int) cur.g << " " << (int) cur.b << " " << (int) cur.a;
			}
		}
		if(data.getNumTexCoords()){
			if(useBinary) {
				os.write((char*) &data.getTexCoords()[i], sizeof(ofVec2f));
			} else {
				os << " " << data.getTexCoord(i).x << " " << data.getTexCoord(i).y;
			}
		}
		if(data.getNumNormals()){
			if(useBinary) {
				os.write((char*) &data.getNormals()[i], sizeof(ofVec3f));
			} else {
				os << " " << data.getNormal(i).x << " " << data.getNormal(i).y << " " << data.getNormal(i).z;
			}
		}
		if(!useBinary) {
			os << endl;
		}
	}

	if(data.getNumIndices()) {
		for(int i = 0; i < data.getNumIndices(); i += faceSize) {
			if(useBinary) {
				os.write((char*) &faceSize, sizeof(unsigned char));
				for(int j = 0; j < faceSize; j++) {
					int curIndex = data.getIndex(i + j);
					os.write((char*) &curIndex, sizeof(int));
				}
			} else {
				os << (int) faceSize << " " << data.getIndex(i) << " " << data.getIndex(i+1) << " " << data.getIndex(i+2) << endl;
			}
		}
	} else if(data.getMode() == OF_PRIMITIVE_TRIANGLES) {
		for(int i = 0; i < data.getNumVertices(); i += faceSize) {
			int indices[] = {i, i + 1, i + 2};
			if(useBinary) {
				os.write((char*) &faceSize, sizeof(unsigned char));
				for(int j = 0; j < faceSize; j++) {
					os.write((char*) &indices[j], sizeof(int));
				}
			} else {
				os << (int) faceSize << " " << indices[0] << " " << indices[1] << " " << indices[2] << endl;
			}
		}
	}

	//TODO: add index generation for other OF_PRIMITIVE cases
}
