#include "ofPath.h"
#include "ofGraphics.h"
#include "ofTessellator.h"

ofTessellator ofPath::tessellator;

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
ofPath::ofPath(){
	strokeWidth = 0;
	bFill = true;
	windingMode = OF_POLY_WINDING_ODD;
	prevCurveRes = 16;
	curveResolution = 16;
	arcResolution = 20;
	mode = PATHS;
	bNeedsTessellation = false;
	hasChanged = false;
	bUseShapeColor = true;
	clear();
}

//----------------------------------------------------------
void ofPath::clear(){
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
void ofPath::newSubPath(){
	if(mode==PATHS){
		paths.push_back(ofSubPath());
	}else{
		polylines.push_back(ofPolyline());
	}
}

//----------------------------------------------------------
void ofPath::lineTo(const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::lineTo,p));
		hasChanged = true;
	}else{
		lastPolyline().lineTo(p);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofPath::lineTo(float x, float y, float z){
	lineTo(ofPoint(x,y,z));
}

//----------------------------------------------------------
void ofPath::lineTo(float x, float y){
	lineTo(ofPoint(x,y,0));
}

//----------------------------------------------------------
void ofPath::moveTo(const ofPoint & p){
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
void ofPath::moveTo(float x, float y, float z){
	moveTo(ofPoint(x,y,z));
}

//----------------------------------------------------------
void ofPath::curveTo(const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::curveTo,p));
		hasChanged = true;
	}else{
		lastPolyline().curveTo(p);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofPath::curveTo(float x, float y, float z){
	curveTo(ofPoint(x,y,z));
}

//----------------------------------------------------------
void ofPath::curveTo(float x, float y){
	curveTo(ofPoint(x,y,0));
}

//----------------------------------------------------------
void ofPath::bezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::bezierTo,p,cp1,cp2));
		hasChanged = true;
	}else{
		lastPolyline().bezierTo(cp1,cp2,p);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofPath::bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	bezierTo(ofPoint(cx1,cy1,0),ofPoint(cx2,cy2,0),ofPoint(x,y,0));
}

//----------------------------------------------------------
void ofPath::bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	bezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

//----------------------------------------------------------
void ofPath::quadBezierTo(const ofPoint & cp1, const ofPoint & cp2, const ofPoint & p){
	if(mode==PATHS){
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::quadBezierTo,p,cp1,cp2));
		hasChanged = true;
	}else{
		lastPolyline().quadBezierTo(cp1,cp2,p);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofPath::quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	quadBezierTo(ofPoint(cx1,cy1,0),ofPoint(cx2,cy2,0),ofPoint(x,y,0));
}

//----------------------------------------------------------
void ofPath::quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	quadBezierTo(ofPoint(cx1,cy1,cz1),ofPoint(cx2,cy2,cz2),ofPoint(x,y,z));
}

//----------------------------------------------------------
void ofPath::arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise){
    if(clockwise) {
        arc(centre,radiusX,radiusY,angleBegin,angleEnd);
    } else {
        arcNegative(centre,radiusX,radiusY,angleBegin,angleEnd);
    }
}

//----------------------------------------------------------
void ofPath::arc(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	if(mode==PATHS){
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::arc,centre,radiusX,radiusY,angleBegin,angleEnd));
		hasChanged = true;
	}else{
		lastPolyline().arc(centre,radiusX,radiusY,angleBegin,angleEnd,arcResolution);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofPath::arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(ofPoint(x,y,0),radiusX,radiusY,angleBegin,angleEnd);
}

//----------------------------------------------------------
void ofPath::arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(ofPoint(x,y,z),radiusX,radiusY,angleBegin,angleEnd);
}

//----------------------------------------------------------
void ofPath::arcNegative(const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	if(mode==PATHS){
		lastPath().addCommand(ofSubPath::Command(ofSubPath::Command::arcNegative,centre,radiusX,radiusY,angleBegin,angleEnd));
		hasChanged = true;
	}else{
		lastPolyline().arcNegative(centre,radiusX,radiusY,angleBegin,angleEnd,arcResolution);
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofPath::arcNegative(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arcNegative(ofPoint(x,y,0),radiusX,radiusY,angleBegin,angleEnd);
}

//----------------------------------------------------------
void ofPath::arcNegative(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arcNegative(ofPoint(x,y,z),radiusX,radiusY,angleBegin,angleEnd);
}

//----------------------------------------------------------
void ofPath::close(){
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
void ofPath::setPolyWindingMode(ofPolyWindingMode newMode){
	if(windingMode != newMode){
		windingMode = newMode;
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofPath::setFilled(bool hasFill){
	if(bFill != hasFill){
		bFill = hasFill;
		if(bFill) strokeWidth = 0;
		else if(strokeWidth==0) strokeWidth = 1;
		if(!cachedTessellationValid) bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofPath::setStrokeWidth(float width){
	strokeWidth = width;
}

//----------------------------------------------------------
ofSubPath & ofPath::lastPath(){
	if(paths.empty() || paths.back().isClosed()){
		paths.push_back(ofSubPath());
	}
	return paths.back();
}

//----------------------------------------------------------
ofPolyline & ofPath::lastPolyline(){
	if(polylines.empty() || polylines.back().isClosed()){
		polylines.push_back(ofPolyline());
	}
	return polylines.back();
}

//----------------------------------------------------------
vector<ofSubPath> & ofPath::getSubPaths(){
	if(mode==POLYLINES){
		ofLog(OF_LOG_WARNING,"trying to get paths from shape with polylines only");
	}
	return paths;
}

//----------------------------------------------------------
const vector<ofSubPath> & ofPath::getSubPaths() const{
	if(mode==POLYLINES){
		ofLog(OF_LOG_WARNING,"trying to get paths from shape with polylines only");
	}
	return paths;
}

//----------------------------------------------------------
ofPolyWindingMode ofPath::getWindingMode() const{
	return windingMode;
}

//----------------------------------------------------------
bool ofPath::isFilled() const{
	return bFill;
}

//----------------------------------------------------------
ofColor ofPath::getFillColor() const{
	return fillColor;
}

//----------------------------------------------------------
ofColor ofPath::getStrokeColor() const{
	return strokeColor;
}

//----------------------------------------------------------
float ofPath::getStrokeWidth() const{
	return strokeWidth;
}

//----------------------------------------------------------
void ofPath::generatePolylinesFromPaths(){
	if(mode==POLYLINES || paths.empty()) return;
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
                case ofSubPath::Command::arcNegative:
                    polylines[j].arcNegative(commands[i].to,commands[i].radiusX,commands[i].radiusY,commands[i].angleBegin,commands[i].angleEnd, arcResolution);
                    break;
				}
			}
			polylines[j].setClosed(paths[j].isClosed());
		}
		hasChanged = false;
		bNeedsTessellation = true;
		cachedTessellationValid=false;
	}
}

//----------------------------------------------------------
void ofPath::tessellate(){
	generatePolylinesFromPaths();
	if(!bNeedsTessellation) return;
	if(bFill){
		tessellator.tessellateToMesh( polylines, windingMode, cachedTessellation);
		cachedTessellationValid=true;
	}
	if(hasOutline() && windingMode!=OF_POLY_WINDING_ODD){
		tessellator.tessellateToPolylines( polylines, windingMode, tessellatedContour);
	}
	bNeedsTessellation = false;
}

//----------------------------------------------------------
vector<ofPolyline> & ofPath::getOutline() {
	if(windingMode!=OF_POLY_WINDING_ODD){
		tessellate();
		return tessellatedContour;
	}else{
		generatePolylinesFromPaths();
		return polylines;
	}
}

//----------------------------------------------------------
ofMesh & ofPath::getTessellation(){
	tessellate();
	return cachedTessellation;
}

//----------------------------------------------------------
void ofPath::draw(float x, float y){
	ofPushMatrix();
	ofTranslate(x,y);
	draw();
	ofPopMatrix();
}

//----------------------------------------------------------
void ofPath::draw(){
	if(ofGetCurrentRenderer()->rendersPathPrimitives()){
		ofGetCurrentRenderer()->draw(*this);
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

			ofGetCurrentRenderer()->draw(cachedTessellation);

		}

		if(hasOutline()){
			float lineWidth = ofGetStyle().lineWidth;
			if(bUseShapeColor){
				ofSetColor(strokeColor);
			}
			ofSetLineWidth( strokeWidth );
			vector<ofPolyline> & polys = getOutline();
			for(int i=0;i<(int)polys.size();i++){
				ofGetCurrentRenderer()->draw(polys[i]);
			}
			ofSetLineWidth(lineWidth);
		}

		if(bUseShapeColor){
			ofSetColor(prevColor);
		}
	}
}

//----------------------------------------------------------
void ofPath::flagShapeChanged(){
	if(mode==PATHS){
		hasChanged = true;
	}else{
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofPath::setMode(Mode _mode){
	mode = _mode;
}

//----------------------------------------------------------
void ofPath::setCurveResolution(int _curveResolution){
	curveResolution = _curveResolution;
}

//----------------------------------------------------------
int ofPath::getCurveResolution(){
	return curveResolution;
}

//----------------------------------------------------------
void ofPath::setArcResolution(int res){
	arcResolution = res;
}

//----------------------------------------------------------
int ofPath::getArcResolution(){
	return arcResolution;
}

//----------------------------------------------------------
void ofPath::setUseShapeColor(bool useColor){
	bUseShapeColor = useColor;
}

//----------------------------------------------------------
bool ofPath::getUseShapeColor(){
	return bUseShapeColor;
}

//----------------------------------------------------------
void ofPath::setColor( const ofColor& color ) {
	setFillColor( color );
	setStrokeColor( color );
}

//----------------------------------------------------------
void ofPath::setHexColor( int hex ) {
	setColor( ofColor().fromHex( hex ) );
}

//----------------------------------------------------------
void ofPath::setFillColor(const ofColor & color){
	setUseShapeColor(true);
	fillColor = color;
}

//----------------------------------------------------------
void ofPath::setFillHexColor( int hex ) {
	setFillColor( ofColor().fromHex( hex ) );
}

//----------------------------------------------------------
void ofPath::setStrokeColor(const ofColor & color){
	setUseShapeColor(true);
	strokeColor = color;
}

//----------------------------------------------------------
void ofPath::setStrokeHexColor( int hex ) {
	setStrokeColor( ofColor().fromHex( hex ) );
};

//----------------------------------------------------------
void ofPath::simplify(float tolerance){
	if(mode==PATHS) generatePolylinesFromPaths();
	for(int i=0;i<(int)polylines.size();i++){
		polylines[i].simplify(tolerance);
	}
}

//----------------------------------------------------------
void ofPath::translate(const ofPoint & p){
	if(mode==PATHS){
		for(int i=0;i<(int)paths.size();i++){
			for(int j=0;j<(int)paths[i].getCommands().size();j++){
				paths[i].getCommands()[j].to += p;
				if(paths[i].getCommands()[j].type==ofSubPath::Command::bezierTo || paths[i].getCommands()[j].type==ofSubPath::Command::quadBezierTo){
					paths[i].getCommands()[j].cp1 += p;
					paths[i].getCommands()[j].cp2 += p;
				}
			}
		}
		hasChanged = true;
	}else{
		for(int i=0;i<(int)polylines.size();i++){
			for(int j=0;j<(int)polylines[i].size();j++){
				polylines[i][j] += p;
			}
		}
		bNeedsTessellation = true;
	}
}

//----------------------------------------------------------
void ofPath::rotate(float az, const ofVec3f& axis ){
	if(mode==PATHS){
		for(int i=0;i<(int)paths.size();i++){
			for(int j=0;j<(int)paths[i].getCommands().size();j++){
				paths[i].getCommands()[j].to.rotate(az,axis);
				if(paths[i].getCommands()[j].type==ofSubPath::Command::bezierTo || paths[i].getCommands()[j].type==ofSubPath::Command::quadBezierTo){
					paths[i].getCommands()[j].cp1.rotate(az,axis);
					paths[i].getCommands()[j].cp2.rotate(az,axis);
				}
				if(paths[i].getCommands()[j].type==ofSubPath::Command::arc || paths[i].getCommands()[j].type==ofSubPath::Command::arcNegative){
					paths[i].getCommands()[j].angleBegin += az;
					paths[i].getCommands()[j].angleEnd += az;
				}
			}
		}
		hasChanged = true;
	}else{
		for(int i=0;i<(int)polylines.size();i++){
			for(int j=0;j<(int)polylines[i].size();j++){
				polylines[i][j].rotate(az,axis);
			}
		}
		bNeedsTessellation = true;
	}
}


//----------------------------------------------------------
void ofPath::scale(float x, float y){
	if(mode==PATHS){
		for(int i=0;i<(int)paths.size();i++){
			for(int j=0;j<(int)paths[i].getCommands().size();j++){
				paths[i].getCommands()[j].to.x*=x;
				paths[i].getCommands()[j].to.y*=y;
				if(paths[i].getCommands()[j].type==ofSubPath::Command::bezierTo || paths[i].getCommands()[j].type==ofSubPath::Command::quadBezierTo){
					paths[i].getCommands()[j].cp1.x*=x;
					paths[i].getCommands()[j].cp1.y*=y;
					paths[i].getCommands()[j].cp2.x*=x;
					paths[i].getCommands()[j].cp2.y*=y;
				}
				if(paths[i].getCommands()[j].type==ofSubPath::Command::arc || paths[i].getCommands()[j].type==ofSubPath::Command::arcNegative){
					paths[i].getCommands()[j].radiusX *= x;
					paths[i].getCommands()[j].radiusY *= y;
				}
			}
		}
		hasChanged = true;
	}else{
		for(int i=0;i<(int)polylines.size();i++){
			for(int j=0;j<(int)polylines[i].size();j++){
				polylines[i][j].x*=x;
				polylines[i][j].y*=y;
			}
		}
		bNeedsTessellation = true;
	}
}

