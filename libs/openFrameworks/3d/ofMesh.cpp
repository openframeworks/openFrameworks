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

// PLANE MESH //
//--------------------------------------------------------------
ofMesh ofMesh::plane(float width, float height, int columns, int rows, ofPrimitiveMode mode ) {
    ofMesh mesh;
    
    if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
        ofLog( OF_LOG_WARNING ) << "ofGetPlaneMesh :: mode not supported, setting to OF_PRIMITIVE_TRIANGLES";
        mode = OF_PRIMITIVE_TRIANGLES;
    }
    
    mesh.setMode(mode);
    
    ofVec3f vert;
    ofVec3f normal(0, 0, 1); // always facing forward //
    ofVec2f texcoord;
    
    // the origin of the plane is the center //
    float halfW = width/2.f;
    float halfH = height/2.f;
    // add the vertexes //
    for(int iy = 0; iy < rows; iy++) {
        for(int ix = 0; ix < columns; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)columns-1.f));
            texcoord.y = ((float)iy/((float)rows-1.f));
            
            vert.x = texcoord.x * width - halfW;
            vert.y = texcoord.y * height - halfH;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    if(mode == OF_PRIMITIVE_TRIANGLE_STRIP) {
        for(int y = 0; y < rows-1; y++) {
            // even rows //
            if((y&1)==0) {
                for(int x = 0; x < columns; x++) {
                    mesh.addIndex( (y) * columns + x );
                    mesh.addIndex( (y+1) * columns + x);
                }
            } else {
                for(int x = columns-1; x >0; x--) {
                    mesh.addIndex( (y+1) * columns + x );
                    mesh.addIndex( y * columns + x-1 );
                }
            }
        }
        
        if(rows%2!=0) mesh.addIndex(mesh.getNumVertices()-columns);
    } else {
        // Triangles //
        for(int y = 0; y < rows-1; y++) {
            for(int x = 0; x < columns-1; x++) {
                // first triangle //
                mesh.addIndex((y)*columns + x);
                mesh.addIndex((y)*columns + x+1);
                mesh.addIndex((y+1)*columns + x);
                
                // second triangle //
                mesh.addIndex((y)*columns + x+1);
                mesh.addIndex((y+1)*columns + x+1);
                mesh.addIndex((y+1)*columns + x);
            }
        }
    }
    
    return mesh;
}


//----------------------------------------------------------
ofMesh ofMesh::sphere( float radius, int res, ofPrimitiveMode mode ) {
    
    ofMesh mesh;
    
    float doubleRes = res*2.f;
    float polarInc = PI/(res); // ringAngle
    float azimInc = TWO_PI/(doubleRes); // segAngle //
    
    if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
        mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    }
    mesh.setMode(mode);
    
    ofVec3f vert;
    ofVec2f tcoord;
    
    for(float i = 0; i < res+1; i++) {
        
        float tr = sin( PI-i * polarInc );
        float ny = cos( PI-i * polarInc );
        
        tcoord.y = i / res;
        
        for(float j = 0; j <= doubleRes; j++) {
            
            float nx = tr * sin(j * azimInc);
            float nz = tr * cos(j * azimInc);
            
            tcoord.x = j / (doubleRes);
            
            vert.set(nx, ny, nz);
            mesh.addNormal(vert);
            vert *= radius;
            mesh.addVertex(vert);
            mesh.addTexCoord(tcoord);
        }
    }
    
    int nr = doubleRes+1;
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        
        int index1, index2, index3;
        
        for(float iy = 0; iy < res; iy++) {
            for(float ix = 0; ix < doubleRes; ix++) {
                
                // first tri //
                if(iy > 0) {
                    index1 = (iy+0) * (nr) + (ix+0);
                    index2 = (iy+0) * (nr) + (ix+1);
                    index3 = (iy+1) * (nr) + (ix+0);
                    
                    mesh.addIndex(index1);
                    mesh.addIndex(index2);
                    mesh.addIndex(index3);
                }
                
                if(iy < res-1 ) {
                    // second tri //
                    index1 = (iy+0) * (nr) + (ix+1);
                    index2 = (iy+1) * (nr) + (ix+1);
                    index3 = (iy+1) * (nr) + (ix+0);
                    
                    mesh.addIndex(index1);
                    mesh.addIndex(index2);
                    mesh.addIndex(index3);
                    
                }
            }
        }
        
    } else {
        for(int y = 0; y < res; y++) {
            for(int x = 0; x <= doubleRes; x++) {
                mesh.addIndex( (y)*nr + x );
                mesh.addIndex( (y+1)*nr + x );
            }
        }
    }
    
    
    return mesh;
}

/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural
 
 For the latest info, see http://code.google.com/p/ogre-procedural/
 
 Copyright (c) 2010 Michael Broutin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
// http://code.google.com/p/ogre-procedural/source/browse/library/src/ProceduralIcoSphereGenerator.cpp

// NO texture coords or normals
// use ofGetIcoSphere(radius, 0) // 0 iterations will return Icosahedron //
//----------------------------------------------------------
ofMesh ofMesh::icosahedron(float radius) {
    ofMesh mesh;
    
    const float sqrt5 = sqrt(5.0f);
    const float phi = (1.0f + sqrt5) * 0.5f;
    
    /// Step 1 : Generate icosahedron
    float invnorm = 1/sqrt(phi*phi+1);
    
    mesh.addVertex(invnorm*ofVec3f(-1,  phi, 0));//0
    mesh.addVertex(invnorm*ofVec3f( 1,  phi, 0));//1
    mesh.addVertex(invnorm*ofVec3f(0,   1,  -phi));//2
    mesh.addVertex(invnorm*ofVec3f(0,   1,   phi));//3
    mesh.addVertex(invnorm*ofVec3f(-phi,0,  -1));//4
    mesh.addVertex(invnorm*ofVec3f(-phi,0,   1));//5
    mesh.addVertex(invnorm*ofVec3f( phi,0,  -1));//6
    mesh.addVertex(invnorm*ofVec3f( phi,0,   1));//7
    mesh.addVertex(invnorm*ofVec3f(0,   -1, -phi));//8
    mesh.addVertex(invnorm*ofVec3f(0,   -1,  phi));//9
    mesh.addVertex(invnorm*ofVec3f(-1,  -phi,0));//10
    mesh.addVertex(invnorm*ofVec3f( 1,  -phi,0));//11
    
    int firstFaces[] = {
        0,1,2,
        0,3,1,
        0,4,5,
        1,7,6,
        1,6,2,
        1,3,7,
        0,2,4,
        0,5,3,
        2,6,8,
        2,8,4,
        3,5,9,
        3,9,7,
        11,6,7,
        10,5,4,
        10,4,8,
        10,9,5,
        11,8,6,
        11,7,9,
        10,8,11,
        10,11,9
    };
    
    for(int i = 0; i < mesh.getNumVertices(); i++) {
        mesh.setVertex(i, mesh.getVertex(i) * radius);
    }
    
    for(int i = 0; i < 60; i+=3) {
        mesh.addTriangle(firstFaces[i], firstFaces[i+1], firstFaces[i+2]);
    }
    
    return mesh;
}



// based on code by Michael Broutin for the ogre-procedural library //
// http://code.google.com/p/ogre-procedural/source/browse/library/src/ProceduralIcoSphereGenerator.cpp
// For the latest info, see http://code.google.com/p/ogre-procedural/ //
//----------------------------------------------------------
ofMesh ofMesh::icosphere(float radius, int iterations) {
    
    //ofMesh icosahedron = ofGetIcosahedronMesh( 1.f );
    ofMesh icosahedron = ofMesh::icosahedron( 1.f );
    vector<ofVec3f> vertices = icosahedron.getVertices();
    vector<ofIndexType> faces = icosahedron.getIndices();
    
    int size = faces.size();
    
    /// Step 2 : tessellate
    for (unsigned short iteration = 0; iteration < iterations; iteration++)
    {
        size*=4;
        vector<ofIndexType> newFaces;
        newFaces.clear();
        //newFaces.resize(size);
        for (int i=0; i<size/12; i++)
        {
            int i1 = faces[i*3];
            int i2 = faces[i*3+1];
            int i3 = faces[i*3+2];
            int i12 = vertices.size();
            int i23 = i12+1;
            int i13 = i12+2;
            ofVec3f v1 = vertices[i1];
            ofVec3f v2 = vertices[i2];
            ofVec3f v3 = vertices[i3];
            //make 1 vertice at the center of each edge and project it onto the sphere
            vertices.push_back((v1+v2).normalized());
            vertices.push_back((v2+v3).normalized());
            vertices.push_back((v1+v3).normalized());
            //now recreate indices
            newFaces.push_back(i1);
            newFaces.push_back(i12);
            newFaces.push_back(i13);
            newFaces.push_back(i2);
            newFaces.push_back(i23);
            newFaces.push_back(i12);
            newFaces.push_back(i3);
            newFaces.push_back(i13);
            newFaces.push_back(i23);
            newFaces.push_back(i12);
            newFaces.push_back(i23);
            newFaces.push_back(i13);
        }
        faces.swap(newFaces);
    }
    
    /// Step 3 : generate texcoords
    vector<ofVec2f> texCoords;
    for (unsigned short i=0;i<vertices.size();i++)
    {
        const ofVec3f& vec = vertices[i];
        float u, v;
        float r0 = sqrtf(vec.x*vec.x+vec.z*vec.z);
        float alpha;
        alpha = atan2f(vec.z,vec.x);
        u = alpha/TWO_PI+.5f;
        v = atan2f(vec.y, r0)/PI + .5f;
        // reverse the u coord, so the default is texture mapped left to
        // right on the outside of a sphere //
        texCoords.push_back(ofVec2f(1.0-u,v));
    }
    
    /// Step 4 : fix texcoords
    // find vertices to split
    std::vector<int> indexToSplit;
    
    for (unsigned int i=0;i<faces.size()/3;i++)
    {
        ofVec2f& t0 = texCoords[faces[i*3+0]];
        ofVec2f& t1 = texCoords[faces[i*3+1]];
        ofVec2f& t2 = texCoords[faces[i*3+2]];
        
        if (abs(t2.x-t0.x)>0.5)
        {
            if (t0.x<0.5)
                indexToSplit.push_back(faces[i*3]);
            else
                indexToSplit.push_back(faces[i*3+2]);
        }
        if (abs(t1.x-t0.x)>0.5)
        {
            if (t0.x<0.5)
                indexToSplit.push_back(faces[i*3]);
            else
                indexToSplit.push_back(faces[i*3+1]);
        }
        if (abs(t2.x-t1.x)>0.5)
        {
            if (t1.x<0.5)
                indexToSplit.push_back(faces[i*3+1]);
            else
                indexToSplit.push_back(faces[i*3+2]);
        }
    }
    
    //split vertices
    for (unsigned short i=0;i<indexToSplit.size();i++)
    {
        int index = indexToSplit[i];
        //duplicate vertex
        ofVec3f v = vertices[index];
        ofVec2f t = texCoords[index] + ofVec2f(1.f, 0.f);
        vertices.push_back(v);
        texCoords.push_back(t);
        int newIndex = vertices.size()-1;
        //reassign indices
        for (unsigned short j=0;j<faces.size();j++)
        {
            if (faces[j]==index)
            {
                int index1 = faces[(j+1)%3+(j/3)*3];
                int index2 = faces[(j+2)%3+(j/3)*3];
                if ((texCoords[index1].x>0.5) || (texCoords[index2].x>0.5))
                {
                    faces[j] = newIndex;
                }
            }
        }
    }
    
    
    ofMesh sphere;
    
    sphere.addIndices( faces );
    sphere.addNormals( vertices );
    sphere.addTexCoords( texCoords );
    
    for(int i = 0; i < vertices.size(); i++ ) {
        vertices[i] *= radius;
    }
    sphere.addVertices( vertices );
    
    return  sphere;
}
/*
 -----------------------------------------------------------------------------
 // END OGRE
 -----------------------------------------------------------------------------
 */



// Cylinder Mesh
//----------------------------------------------------------
ofMesh ofMesh::cylinder( float radius, float height, int radiusSegments, int heightSegments, int numCapSegments, bool bCapped, ofPrimitiveMode mode ) {
    ofMesh mesh;
    if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
        mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    }
    mesh.setMode(mode);
    
    int capSegs = numCapSegments;
    if(heightSegments < 2) heightSegments = 2;
    if(capSegs < 2) capSegs = 2;
    if(!bCapped) capSegs=1;
    
    float angleIncRadius = -1 * (TWO_PI/((float)radiusSegments-1.f));
    float heightInc = height/((float)heightSegments-1.f);
    float halfH = height*.5f;
    
    float newRad;
    ofVec3f vert;
    ofVec2f tcoord;
    ofVec3f normal;
    ofVec3f up(0,1,0);
    
    int vertOffset = 0;
    
    float maxTexY           = heightSegments-1.f + (capSegs*2)-2.f;
    float maxTexYNormalized = (capSegs-1.f) / maxTexY;
    
    // add the top cap //
    if(bCapped) {
        normal.set(0,-1,0);
        for(int iy = 0; iy < capSegs; iy++) {
            for(int ix = 0; ix < radiusSegments; ix++) {
                newRad = ofMap((float)iy, 0, capSegs-1, 0.0, radius);
                vert.x = cos((float)ix*angleIncRadius) * newRad;
                vert.z = sin((float)ix*angleIncRadius) * newRad;
                vert.y = -halfH;
                
                tcoord.x = (float)ix/((float)radiusSegments-1.f);
                tcoord.y = ofMap(iy, 0, capSegs-1, 0, maxTexYNormalized);
                
                mesh.addTexCoord( tcoord );
                mesh.addVertex( vert );
                mesh.addNormal( normal );
            }
        }
        
        if(mode == OF_PRIMITIVE_TRIANGLES) {
            for(int y = 0; y < capSegs-1; y++) {
                for(int x = 0; x < radiusSegments-1; x++) {
                    if(y > 0) {
                        // first triangle //
                        mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                        mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                        mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                    }
                    
                    // second triangle //
                    mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                }
            }
        } else {
            for(int y = 0; y < capSegs-1; y++) {
                for(int x = 0; x < radiusSegments; x++) {
                    mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                }
            }
        }
        
        vertOffset = mesh.getNumVertices();
        
    }
    
    //maxTexY             = heightSegments-1.f + capSegs-1.f;
    float minTexYNormalized = 0;
    if(bCapped) minTexYNormalized = maxTexYNormalized;
    maxTexYNormalized   = 1.f;
    if(bCapped) maxTexYNormalized = (heightSegments) / maxTexY;
    
    // cylinder vertices //
    for(int iy = 0; iy < heightSegments; iy++) {
        normal.set(1,0,0);
        for(int ix = 0; ix < radiusSegments; ix++) {
            
            //newRad = ofMap((float)iy, 0, heightSegments-1, 0.0, radius);
            vert.x = cos((float)ix*angleIncRadius) * radius;
            vert.y = heightInc*((float)iy) - halfH;
            vert.z = sin((float)ix*angleIncRadius) * radius;
            
            tcoord.x = (float)ix/((float)radiusSegments-1.f);
            tcoord.y = ofMap(iy, 0, heightSegments-1, minTexYNormalized, maxTexYNormalized );
            
            mesh.addTexCoord( tcoord );
            mesh.addVertex( vert );
            mesh.addNormal( normal );
            
            normal.rotateRad(-angleIncRadius, up);
            
        }
    }
    
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        for(int y = 0; y < heightSegments-1; y++) {
            for(int x = 0; x < radiusSegments-1; x++) {
                // first triangle //
                mesh.addIndex( (y)*radiusSegments + x + vertOffset);
                mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset );
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset );
                
                // second triangle //
                mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset );
                mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset );
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset );
            }
        }
    } else {
        for(int y = 0; y < heightSegments-1; y++) {
            for(int x = 0; x < radiusSegments; x++) {
                mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset );
            }
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    // add the bottom cap
    if(bCapped) {
        minTexYNormalized = maxTexYNormalized;
        maxTexYNormalized   = 1.f;
        
        normal.set(0, 1,0);
        for(int iy = 0; iy < capSegs; iy++) {
            for(int ix = 0; ix < radiusSegments; ix++) {
                newRad = ofMap((float)iy, 0, capSegs-1, radius, 0.0);
                vert.x = cos((float)ix*angleIncRadius) * newRad;
                vert.z = sin((float)ix*angleIncRadius) * newRad;
                vert.y = halfH;
                
                tcoord.x = (float)ix/((float)radiusSegments-1.f);
                tcoord.y = ofMap(iy, 0, capSegs-1, minTexYNormalized, maxTexYNormalized);
                
                mesh.addTexCoord( tcoord );
                mesh.addVertex( vert );
                mesh.addNormal( normal );
            }
        }
        
        if(mode == OF_PRIMITIVE_TRIANGLES) {
            for(int y = 0; y < capSegs-1; y++) {
                for(int x = 0; x < radiusSegments-1; x++) {
                    // first triangle //
                    mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                    mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                    
                    if(y < capSegs -1 && capSegs > 2) {
                        // second triangle //
                        mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                        mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset);
                        mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                    }
                }
            }
        } else {
            for(int y = 0; y < capSegs-1; y++) {
                for(int x = 0; x < radiusSegments; x++) {
                    mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                }
            }
        }
        
        vertOffset = mesh.getNumVertices();
        
    }
    
    return mesh;
}

// Cone Mesh //
//--------------------------------------------------------------
ofMesh ofMesh::cone( float radius, float height, int radiusSegments, int heightSegments, int capSegments, ofPrimitiveMode mode ) {
    ofMesh mesh;
    if(mode != OF_PRIMITIVE_TRIANGLE_STRIP && mode != OF_PRIMITIVE_TRIANGLES) {
        mode = OF_PRIMITIVE_TRIANGLE_STRIP;
    }
    mesh.setMode(mode);
    
    if(heightSegments < 2) heightSegments = 2;
    int capSegs = capSegments;
    if(capSegs < 2) capSegs = 2;
    
    
    float angleIncRadius    = -1.f * ((TWO_PI/((float)radiusSegments-1.f)));
    float heightInc         = height/((float)heightSegments-1);
    float halfH             = height*.5f;
    
    float newRad;
    ofVec3f vert;
    ofVec3f normal;
    ofVec2f tcoord;
    ofVec3f up(0,1,0);
    
    
    int vertOffset = 0;
    
    float maxTexY = heightSegments-1.f + capSegs-1.f;
    
    float crossAngle = ofVec3f(radius, 1, 0).angle( ofVec3f(0,1,0) );
    ofVec3f startVec(0, -halfH-1.f, 0);
    
    // cone vertices //
    for(int iy = 0; iy < heightSegments; iy++) {
        normal.set(1,0,0);
        for(int ix = 0; ix < radiusSegments; ix++) {
            
            newRad = ofMap((float)iy, 0, heightSegments-1, 0.0, radius);
            vert.x = cos((float)ix*angleIncRadius) * newRad;
            vert.y = heightInc*((float)iy) - halfH;
            vert.z = sin((float)ix*angleIncRadius) * newRad;
            
            tcoord.x = (float)ix/((float)radiusSegments-1.f);
            tcoord.y = (float)iy/((float)maxTexY);
            
            mesh.addTexCoord( tcoord );
            mesh.addVertex( vert );
            
            if(iy == 0) {
                newRad = 1.f;
                vert.x = cos((float)ix*angleIncRadius) * newRad;
                vert.y = heightInc*((float)iy) - halfH;
                vert.z = sin((float)ix*angleIncRadius) * newRad;
            }
            
            ofVec3f diff = vert-startVec;
            ofVec3f crossed = up.crossed(vert);
            normal = crossed.normalized();
            normal = crossed.getPerpendicular(diff);
            
            normal.normalize();
            
            mesh.addNormal( normal );
            //}
            
        }
    }
    
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        for(int y = 0; y < heightSegments-1; y++) {
            for(int x = 0; x < radiusSegments-1; x++) {
                if(y > 0){
                    // first triangle //
                    mesh.addIndex( (y)*radiusSegments + x );
                    mesh.addIndex( (y)*radiusSegments + x+1 );
                    mesh.addIndex( (y+1)*radiusSegments + x );
                }
                
                // second triangle //
                mesh.addIndex( (y)*radiusSegments + x+1 );
                mesh.addIndex( (y+1)*radiusSegments + x+1 );
                mesh.addIndex( (y+1)*radiusSegments + x );
            }
        }
    } else {
        for(int y = 0; y < heightSegments-1; y++) {
            for(int x = 0; x < radiusSegments; x++) {
                mesh.addIndex( (y)*radiusSegments + x );
                mesh.addIndex( (y+1)*radiusSegments + x );
            }
        }
    }
    
    vertOffset = mesh.getNumVertices();
    float maxTexYNormalized = (heightSegments-1.f) / maxTexY;
    
    // add the cap //
    normal.set(0,1,0);
    for(int iy = 0; iy < capSegs; iy++) {
        for(int ix = 0; ix < radiusSegments; ix++) {
            newRad = ofMap((float)iy, 0, capSegs-1, radius, 0.0);
            vert.x = cos((float)ix*angleIncRadius) * newRad;
            vert.z = sin((float)ix*angleIncRadius) * newRad;
            vert.y = halfH;
            
            tcoord.x = (float)ix/((float)radiusSegments-1.f);
            tcoord.y = ofMap(iy, 0, capSegs-1, maxTexYNormalized, 1.f);
            
            mesh.addTexCoord( tcoord );
            mesh.addVertex( vert );
            mesh.addNormal( normal );
        }
    }
    
    if(mode == OF_PRIMITIVE_TRIANGLES) {
        for(int y = 0; y < capSegs-1; y++) {
            for(int x = 0; x < radiusSegments-1; x++) {
                //if(y > 0) {
                // first triangle //
                mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                //}
                
                if(y < capSegs-1) {
                    // second triangle //
                    mesh.addIndex( (y)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
                }
            }
        }
    } else {
        for(int y = 0; y < capSegs-1; y++) {
            for(int x = 0; x < radiusSegments; x++) {
                mesh.addIndex( (y)*radiusSegments + x + vertOffset );
                mesh.addIndex( (y+1)*radiusSegments + x + vertOffset);
            }
        }
    }
    
    return mesh;
}



// Box Mesh //
//--------------------------------------------------------------
ofMesh ofMesh::box( float width, float height, float depth, int resX, int resY, int resZ ) {
    // mesh only available as triangles //
    ofMesh mesh;
    mesh.setMode( OF_PRIMITIVE_TRIANGLES );
    
    // halves //
    float halfW = width * .5f;
    float halfH = height * .5f;
    float halfD = depth * .5f;
    
    ofVec3f vert;
    ofVec2f texcoord;
    ofVec3f normal;
    int vertOffset = 0;
    
    // TRIANGLES //
    
    // Front Face //
    normal.set(0, 0, 1);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            vert.x = texcoord.x * width - halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    // Right Side Face //
    normal.set(1, 0, 0);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resZ; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resZ-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            //vert.x = texcoord.x * width - halfW;
            vert.x = halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = texcoord.x * -depth + halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resZ-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resZ + x + vertOffset);
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    // Left Side Face //
    normal.set(-1, 0, 0);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resZ; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resZ-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            //vert.x = texcoord.x * width - halfW;
            vert.x = -halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = texcoord.x * depth - halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resZ-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resZ + x + vertOffset);
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    // Back Face //
    normal.set(0, 0, -1);
    // add the vertexes //
    for(int iy = 0; iy < resY; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resY-1.f));
            
            vert.x = texcoord.x * -width + halfW;
            vert.y = texcoord.y * height - halfH;
            vert.z = -halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resY-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    
    // Top Face //
    normal.set(0, -1, 0);
    // add the vertexes //
    for(int iy = 0; iy < resZ; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resZ-1.f));
            
            vert.x = texcoord.x * width - halfW;
            //vert.y = texcoord.y * height - halfH;
            vert.y = -halfH;
            vert.z = texcoord.y * depth - halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resZ-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    vertOffset = mesh.getNumVertices();
    
    
    // Bottom Face //
    normal.set(0, 1, 0);
    // add the vertexes //
    for(int iy = 0; iy < resZ; iy++) {
        for(int ix = 0; ix < resX; ix++) {
            
            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = ((float)iy/((float)resZ-1.f));
            
            vert.x = texcoord.x * width - halfW;
            //vert.y = texcoord.y * height - halfH;
            vert.y = halfH;
            vert.z = texcoord.y * -depth + halfD;
            
            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }
    
    for(int y = 0; y < resZ-1; y++) {
        for(int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            
            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }
    
    
    
    return mesh;
}








