#include "ofShape.h"
#include "ofGraphics.h"
#include "ofTessellator.h"

//----------------------------------------------------------
ofSubPath::ofSubPath(){
	bClosed = false;
	bHasChanged = true;
}

//----------------------------------------------------------
const vector<ofSubPath::Command> & ofSubPath::getCommands() const{
	return commands;
}

//----------------------------------------------------------
vector<ofSubPath::Command> & ofSubPath::getCommands(){
	return commands;
}

//----------------------------------------------------------
void ofSubPath::addCommand(const ofSubPath::Command & command){
	commands.push_back(command);
	bHasChanged=true;
}

//----------------------------------------------------------
void ofSubPath::close(){
	bClosed = true;
}

//----------------------------------------------------------
bool ofSubPath::isClosed(){
	return bClosed;
}

//----------------------------------------------------------
int ofSubPath::size(){
	return commands.size();
}

//----------------------------------------------------------
ofSubPath::Command::Command(Type type , const ofPoint & p)
:type(type)
,to(p)
{}

//----------------------------------------------------------
ofSubPath::Command::Command(Type type , const ofPoint & p, const ofPoint & cp1, const ofPoint & cp2)
:type(type)
,to(p)
,cp1(cp1)
,cp2(cp2)
{
}

//----------------------------------------------------------
ofSubPath::Command::Command(Type type , const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd)
:type(type)
,to(centre)
,radiusX(radiusX)
,radiusY(radiusY)
,angleBegin(angleBegin)
,angleEnd(angleEnd)
{
}



// ofShape
//----------------------------------------------------------
ofShape::ofShape(){
	strokeWidth = 0;
	bFill = true;
	windingMode = OF_POLY_WINDING_ODD;
	prevCurveRes = 16;
	curveResolution = 16;
	arcResolution = 20;
	mode = PATHS;
	bNeedsTessellation = false;
	hasChanged = false;
	cachedTessellation.changed = false;
	bUseShapeColor = true;
	clear();
}

//----------------------------------------------------------
void ofShape::clear(){
	if(mode==PATHS){
		paths.clear();
		hasChanged = true;
	}else{
		// for performance, instead of clearing the whole vector
		// let one polyline and clear it: avoids instantiation
		polylines.resize(1);
		polylines[0].clear();
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofShape::newSubPath(){
	if(mode==PATHS){
		paths.push_back(ofSubPath());
	}else{
		polylines.push_back(ofPolyline());
	}
}

//----------------------------------------------------------
void ofShape::lineTo(const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::lineTo,p));
		hasChanged = true;
	}else{
		lastPolyline().lineTo(p);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofShape::lineTo(float x, float y, float z){
	lineTo(ofPoint(x,y,z));
}

//----------------------------------------------------------
void ofShape::lineTo(float x, float y){
	lineTo(ofPoint(x,y,0));
}

//----------------------------------------------------------
void ofShape::moveTo(const ofPoint & p){
	if(mode==PATHS){
		if(lastPath().size()>0) newSubPath();
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::lineTo,p));
		hasChanged = true;
	}else{
		if(lastPolyline().size()>0) newSubPath();
		lastPolyline().addVertex(p);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofShape::moveTo(float x, float y, float z){
	moveTo(ofPoint(x,y,z));
}

//----------------------------------------------------------
void ofShape::curveTo(const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::curveTo,p));
		hasChanged = true;
	}else{
		lastPolyline().curveTo(p);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofShape::curveTo(float x, float y, float z){
	curveTo(ofPoint(x,y,z));
}

//----------------------------------------------------------
void ofShape::curveTo(float x, float y){
	curveTo(ofPoint(x,y,0));
}

//----------------------------------------------------------
void ofShape::bezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::bezierTo,p,cp1,cp2));
		hasChanged = true;
	}else{
		lastPolyline().bezierTo(cp1,cp2,p);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofShape::bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	bezierTo(ofPoint(cx1,cy1,0),ofPoint(cx2,cy2,0),ofPoint(x,y,0));
}

//----------------------------------------------------------
void ofShape::bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	bezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

//----------------------------------------------------------
void ofShape::quadBezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::quadBezierTo,p,cp1,cp2));
		hasChanged = true;
	}else{
		lastPolyline().quadBezierTo(cp1,cp2,p);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofShape::quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	quadBezierTo(ofPoint(cx1,cy1,0),ofPoint(cx2,cy2,0),ofPoint(x,y,0));
}

//----------------------------------------------------------
void ofShape::quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	quadBezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

//----------------------------------------------------------
void ofShape::arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	if(mode==PATHS){
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::arc,centre,radiusX,radiusY,angleBegin,angleEnd));
		hasChanged = true;
	}else{
		lastPolyline().arc(centre,radiusX,radiusY,angleBegin,angleEnd,arcResolution);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofShape::arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(ofPoint(x,y,0),radiusX,radiusY,angleBegin,angleEnd);
}

//----------------------------------------------------------
void ofShape::arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(ofPoint(x,y,z),radiusX,radiusY,angleBegin,angleEnd);
}

//----------------------------------------------------------
void ofShape::close(){
	if(mode==PATHS){
		lastPath().close();
		hasChanged = true;
	}else{
		lastPolyline().setClosed(true);
		bNeedsTessellation = true;
	}
	//newPath();
}

//----------------------------------------------------------
void ofShape::setPolyWindingMode(ofPolyWindingMode newMode){
	if(windingMode != newMode){
		windingMode = newMode;
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofShape::setFilled(bool hasFill){
	if(bFill != hasFill){
		bFill = hasFill;
		if(bFill) strokeWidth = 0;
		else if(strokeWidth==0) strokeWidth = 1;
		if(cachedTessellation.changed) bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofShape::setStrokeWidth(float width){
	strokeWidth = width;
}

//----------------------------------------------------------
ofSubPath & ofShape::lastPath(){
	if(paths.empty() || paths.back().isClosed()){
		paths.push_back(ofSubPath());
	}
	return paths.back();
}

//----------------------------------------------------------
ofPolyline & ofShape::lastPolyline(){
	if(polylines.empty() || polylines.back().isClosed()){
		polylines.push_back(ofPolyline());
	}
	return polylines.back();
}

//----------------------------------------------------------
vector<ofSubPath> & ofShape::getSubPaths(){
	if(mode==POLYLINES){
		ofLog(OF_LOG_WARNING,"trying to get paths from shape with polylines only");
	}
	return paths;
}

//----------------------------------------------------------
const vector<ofSubPath> & ofShape::getSubPaths() const{
	if(mode==POLYLINES){
		ofLog(OF_LOG_WARNING,"trying to get paths from shape with polylines only");
	}
	return paths;
}

//----------------------------------------------------------
ofPolyWindingMode ofShape::getWindingMode() const{
	return windingMode;
}

//----------------------------------------------------------
bool ofShape::isFilled() const{
	return bFill;
}

//----------------------------------------------------------
ofColor ofShape::getFillColor() const{
	return fillColor;
}

//----------------------------------------------------------
ofColor ofShape::getStrokeColor() const{
	return strokeColor;
}

//----------------------------------------------------------
float ofShape::getStrokeWidth() const{
	return strokeWidth;
}

//----------------------------------------------------------
void ofShape::generatePolylinesFromPaths(){
	if(mode==POLYLINES) return;
	if(hasChanged || curveResolution!=prevCurveRes){
		prevCurveRes = curveResolution;

		polylines.clear();
		polylines.resize(paths.size());
		for(int j=0;j<(int)paths.size();j++){
			const vector<ofSubPath::Command> & commands = paths[j].getCommands();

			for(int i=0; i<(int)commands.size();i++){
				switch(commands[i].type){

				case ofSubPath::Command::lineTo:
					polylines[j].addVertex(commands[i].to);
					break;
				case ofSubPath::Command::curveTo:
					polylines[j].curveTo(commands[i].to, curveResolution);
					break;
				case ofSubPath::Command::bezierTo:
					polylines[j].bezierTo(commands[i].cp1,commands[i].cp2,commands[i].to, curveResolution);
					break;
				case ofSubPath::Command::quadBezierTo:
					polylines[j].quadBezierTo(commands[i].cp1,commands[i].cp2,commands[i].to, curveResolution);
					break;
				case ofSubPath::Command::arc:
					polylines[j].arc(commands[i].to,commands[i].radiusX,commands[i].radiusY,commands[i].angleBegin,commands[i].angleEnd, arcResolution);
					break;
				}
			}
			polylines[j].setClosed(paths[j].isClosed());
		}
		hasChanged = false;
		bNeedsTessellation = true;
		cachedTessellation.changed=true;
	}
}

//----------------------------------------------------------
void ofShape::tessellate(){
	generatePolylinesFromPaths();
	if(!bNeedsTessellation) return;
	if(bFill){
		ofTessellator::tessellateToCache( polylines, windingMode, cachedTessellation);
		cachedTessellation.changed=false;
	}
	if ( hasOutline() ){
		if( windingMode != OF_POLY_WINDING_ODD ) {
			ofTessellator::tessellateToOutline( polylines, windingMode, tessellatedPolylines);
		}
	}
	bNeedsTessellation = false;
}

//----------------------------------------------------------
vector<ofPolyline> & ofShape::getOutline() {
	tessellate();
	if( windingMode != OF_POLY_WINDING_ODD ) {
		return tessellatedPolylines;
	}else{
		return polylines;
	}
}

//----------------------------------------------------------
vector<ofPrimitive> & ofShape::getTessellation(){
	tessellate();
	cachedTessellation.meshes.resize(cachedTessellation.numElements);
	return cachedTessellation.meshes;
}

//----------------------------------------------------------
void ofShape::draw(float x, float y){
	ofPushMatrix();
	ofTranslate(x,y);
	draw();
	ofPopMatrix();
}

//----------------------------------------------------------
void ofShape::draw(){
	if(ofGetDefaultRenderer()->rendersPathPrimitives()){
		ofGetDefaultRenderer()->draw(*this);
	}else{
		tessellate();


		ofColor prevColor;
		if(bUseShapeColor){
			prevColor = ofGetStyle().color;
		}

		if(bFill){
			if(bUseShapeColor){
				ofSetColor(fillColor);
			}
			for(int i=0;i<cachedTessellation.numElements && i<(int)cachedTessellation.meshes.size();i++){
				ofGetDefaultRenderer()->draw(cachedTessellation.meshes[i]);
			}
		}

		if(hasOutline()){
			float lineWidth = ofGetStyle().lineWidth;
			if(bUseShapeColor){
				ofSetColor(strokeColor);
			}
			ofSetLineWidth( strokeWidth );
			vector<ofPolyline> & polys = getOutline();
			for(int i=0;i<(int)polys.size();i++){
				ofGetDefaultRenderer()->draw(polys[i]);
			}
			ofSetLineWidth(lineWidth);
		}

		if(bUseShapeColor){
			ofSetColor(prevColor);
		}
	}
}

//----------------------------------------------------------
void ofShape::flagShapeChanged(){
	if(mode==PATHS){
		hasChanged = true;
	}else{
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofShape::setMode(Mode _mode){
	mode = _mode;
}

//----------------------------------------------------------
void ofShape::setCurveResolution(int _curveResolution){
	curveResolution = _curveResolution;
}

//----------------------------------------------------------
int ofShape::getCurveResolution(){
	return curveResolution;
}

//----------------------------------------------------------
void ofShape::setArcResolution(int res){
	arcResolution = res;
}

//----------------------------------------------------------
int ofShape::getArcResolution(){
	return arcResolution;
}

//----------------------------------------------------------
void ofShape::setUseShapeColor(bool useColor){
	bUseShapeColor = useColor;
}

//----------------------------------------------------------
bool ofShape::getUseShapeColor(){
	return bUseShapeColor;
}

//----------------------------------------------------------
void ofShape::setColor( const ofColor& color ) {
	setFillColor( color );
	setStrokeColor( color );
}

//----------------------------------------------------------
void ofShape::setHexColor( int hex ) {
	setColor( ofColor().fromHex( hex ) );
}

//----------------------------------------------------------
void ofShape::setFillColor(const ofColor & color){
	setUseShapeColor(true);
	fillColor = color;
}

//----------------------------------------------------------
void ofShape::setFillHexColor( int hex ) {
	setFillColor( ofColor().fromHex( hex ) );
}

//----------------------------------------------------------
void ofShape::setStrokeColor(const ofColor & color){
	setUseShapeColor(true);
	strokeColor = color;
}

//----------------------------------------------------------
void ofShape::setStrokeHexColor( int hex ) {
	setStrokeColor( ofColor().fromHex( hex ) );
};

//----------------------------------------------------------
void ofShape::simplify(float tolerance){
	if(mode==PATHS) generatePolylinesFromPaths();
	for(int i=0;i<(int)polylines.size();i++){
		polylines[i].simplify(tolerance);
	}
}
