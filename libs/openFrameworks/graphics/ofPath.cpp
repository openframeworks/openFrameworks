#include "ofPath.h"
#include "ofGraphics.h"
#include "ofTessellator.h"

ofTessellator ofPath::tessellator;

ofPath::Command::Command(Type type)
:type(type){

}

//----------------------------------------------------------
ofPath::Command::Command(Type type , const ofPoint & p)
:type(type)
,to(p)
{}

//----------------------------------------------------------
ofPath::Command::Command(Type type , const ofPoint & p, const ofPoint & cp1, const ofPoint & cp2)
:type(type)
,to(p)
,cp1(cp1)
,cp2(cp2)
{
}

//----------------------------------------------------------
ofPath::Command::Command(Type type , const ofPoint & centre, float radiusX, float radiusY, float angleBegin, float angleEnd)
:type(type)
,to(centre)
,radiusX(radiusX)
,radiusY(radiusY)
,angleBegin(angleBegin)
,angleEnd(angleEnd)
{
}

//----------------------------------------------------------
ofPath::ofPath(){
	strokeWidth = 0;
	bFill = true;
	windingMode = OF_POLY_WINDING_ODD;
	prevCurveRes = 20;
	curveResolution = 20;
	circleResolution = 20;
	mode = COMMANDS;
	bNeedsTessellation = false;
	bHasChanged = false;
	bUseShapeColor = true;
	bNeedsPolylinesGeneration = false;
	clear();
}

//----------------------------------------------------------
void ofPath::clear(){
	commands.clear();
	// for performance, instead of clearing the whole vector
	// let one polyline and clear it: avoids instantiation
	polylines.resize(1);
	polylines[0].clear();
	cachedTessellation.clear();
	flagShapeChanged();
}

//----------------------------------------------------------
void ofPath::newSubPath(){
	if(mode==COMMANDS){
	}else{
		polylines.push_back(ofPolyline());
	}
}

//----------------------------------------------------------
void ofPath::lineTo(const ofPoint & p){
	if(mode==COMMANDS){
		addCommand(Command(Command::lineTo,p));
	}else{
		lastPolyline().lineTo(p);
	}
	flagShapeChanged();
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
	if(mode==COMMANDS){
		addCommand(Command(Command::moveTo,p));
	}else{
		if(lastPolyline().size()>0) newSubPath();
		lastPolyline().addVertex(p);
	}
	flagShapeChanged();
}

//----------------------------------------------------------
void ofPath::moveTo(float x, float y, float z){
	moveTo(ofPoint(x,y,z));
}

//----------------------------------------------------------
void ofPath::curveTo(const ofPoint & p){
	if(mode==COMMANDS){
		addCommand(Command(Command::curveTo,p));
	}else{
		lastPolyline().curveTo(p,curveResolution);
	}
	flagShapeChanged();
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
	if(mode==COMMANDS){
		addCommand(Command(Command::bezierTo,p,cp1,cp2));
	}else{
		lastPolyline().bezierTo(cp1,cp2,p,curveResolution);
	}
	flagShapeChanged();
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
	if(mode==COMMANDS){
		addCommand(Command(Command::quadBezierTo,p,cp1,cp2));
	}else{
		lastPolyline().quadBezierTo(cp1,cp2,p,curveResolution);
	}
	flagShapeChanged();
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
	if(mode==COMMANDS){
		addCommand(Command(Command::arc,centre,radiusX,radiusY,angleBegin,angleEnd));
	}else{
		lastPolyline().arc(centre,radiusX,radiusY,angleBegin,angleEnd,circleResolution);
	}
	flagShapeChanged();
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
	if(mode==COMMANDS){
		addCommand(Command(Command::arcNegative,centre,radiusX,radiusY,angleBegin,angleEnd));
	}else{
		lastPolyline().arcNegative(centre,radiusX,radiusY,angleBegin,angleEnd,circleResolution);
	}
	flagShapeChanged();
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
void ofPath::triangle(float x1,float y1,float x2,float y2,float x3, float y3){
	triangle(x1,y1,0.0f,x2,y2,0.0f,x3,y3,0.0f);
}

//----------------------------------------------------------
void ofPath::triangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3, float y3,float z3){
	moveTo(x1,y1,z1);
	lineTo(x2,y2,z2);
	lineTo(x3,y3,z3);
	close();
}

//----------------------------------------------------------
void ofPath::triangle(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3){
	triangle(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,p3.x,p3.y,p3.z);
}


//----------------------------------------------------------
void ofPath::circle(float x, float y, float radius){
	circle(x,y,0.0f,radius);
}

//----------------------------------------------------------
void ofPath::circle(float x, float y, float z, float radius){
	arc(x,y,z,radius,radius,0,360);
}

//----------------------------------------------------------
void ofPath::circle(const ofPoint & p, float radius){
	circle(p.x,p.y,p.z,radius);
}


//----------------------------------------------------------
void ofPath::ellipse(float x, float y, float width, float height){
	ellipse(x,y,0.0f,width,height);
}

//----------------------------------------------------------
void ofPath::ellipse(float x, float y, float z, float width, float height){
	arc(x,y,z,width*.5,height*.5,0,360);
}

//----------------------------------------------------------
void ofPath::ellipse(const ofPoint & p, float width, float height){
	ellipse(p.x,p.y,p.z,width,height);
}

//----------------------------------------------------------
void ofPath::rectangle(const ofRectangle & r){
	moveTo(r.getTopLeft());
	lineTo(r.getTopRight());
	lineTo(r.getBottomRight());
	lineTo(r.getBottomLeft());
	close();
}

//----------------------------------------------------------
void ofPath::rectangle(const ofPoint & p,float w,float h){
	moveTo(p);
	lineTo(p.x+w,p.y,p.z);
	lineTo(p.x+w,p.y+h,p.z);
	lineTo(p.x,p.y+h,p.z);
	close();
}

//----------------------------------------------------------
void ofPath::rectangle(float x,float y,float w,float h){
	moveTo(x,y);
	lineTo(x+w,y);
	lineTo(x+w,y+h);
	lineTo(x,y+h);
	close();
}

//----------------------------------------------------------
void ofPath::rectangle(float x,float y,float z,float w,float h){
	moveTo(x,y,z);
	lineTo(x+w,y,z);
	lineTo(x+w,y+h,z);
	lineTo(x,y+h,z);
	close();
}

//----------------------------------------------------------
void ofPath::rectRounded(const ofRectangle & b, float r){
	rectRounded(b.x,b.y,0,b.width,b.height,r,r,r,r);
}

//----------------------------------------------------------
void ofPath::rectRounded(const ofPoint & p, float w, float h, float r){
	rectRounded(p.x,p.y,p.z,w,h,r,r,r,r);
}

//----------------------------------------------------------
void ofPath::rectRounded(float x, float y, float w, float h, float r){
	rectRounded(x,y,0.0f,w,h,r,r,r,r);
}

//----------------------------------------------------------
void ofPath::rectRounded(const ofPoint & p, float w, float h, float topLeftRadius,
														float topRightRadius,
														float bottomRightRadius,
														float bottomLeftRadius){

	rectRounded(p.x,p.y,p.z,w,h,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
}

//----------------------------------------------------------
void ofPath::rectRounded(const ofRectangle & b, float topLeftRadius,
										  float topRightRadius,
										  float bottomRightRadius,
										  float bottomLeftRadius){
	rectRounded(b.x,b.y,0,b.width,b.height,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
}

//----------------------------------------------------------
void ofPath::rectRounded(float x, float y, float z, float w, float h, float topLeftRadius,
												  float topRightRadius,
												  float bottomRightRadius,
												  float bottomLeftRadius){
	// since we support w / h < 0, canonicalize the rectangle for easier drawing
	if(w < 0.0f) {
		x += w;
		w *= -1.0f;
	}

	if(h < 0.0f) {
		y += h;
		h *= -1.0f;
	}

	// keep radii in check
	float maxRadius = MIN(w / 2.0f, h / 2.0f);
	topLeftRadius        = MIN(topLeftRadius,     maxRadius);
	topRightRadius       = MIN(topRightRadius,    maxRadius);
	bottomRightRadius    = MIN(bottomRightRadius, maxRadius);
	bottomLeftRadius     = MIN(bottomLeftRadius,  maxRadius);

	// if all radii are ~= 0.0f, then render as a normal rectangle
	if((fabs(topLeftRadius)     < FLT_EPSILON) &&
	   (fabs(topRightRadius)    < FLT_EPSILON) &&
	   (fabs(bottomRightRadius) < FLT_EPSILON) &&
	   (fabs(bottomLeftRadius)  < FLT_EPSILON)) {

		// rect mode respect happens in ofRect
		rectangle(x, y, z, w, h);
	} else {
		float left   = x;
		float right  = x + w;
		float top    = y;
		float bottom = y + h;


		moveTo(left + topLeftRadius, top, z);

		// top right
		if(fabs(topRightRadius) >= FLT_EPSILON) {
			arc(right - topRightRadius, top + topRightRadius, z, topRightRadius, topRightRadius, 270, 360);
		} else {
			lineTo(right, top, z);
		}

		lineTo(right, bottom - bottomRightRadius);
		// bottom right
		if(fabs(bottomRightRadius) >= FLT_EPSILON) {
			arc(right - bottomRightRadius, bottom - bottomRightRadius, z, bottomRightRadius, bottomRightRadius, 0, 90);
		}

		lineTo(left + bottomLeftRadius, bottom, z);

		// bottom left
		if(fabs(bottomLeftRadius) >= FLT_EPSILON) {
			arc(left + bottomLeftRadius, bottom - bottomLeftRadius, z, bottomLeftRadius, bottomLeftRadius, 90, 180);
		}

		lineTo(left, top + topLeftRadius, z);

		// top left
		if(fabs(topLeftRadius) >= FLT_EPSILON) {
			arc(left + topLeftRadius, top + topLeftRadius, z, topLeftRadius, topLeftRadius, 180, 270);
		}
		close();

	}
}

//----------------------------------------------------------
void ofPath::close(){
	if(mode==COMMANDS){
		addCommand(Command(Command::close));
	}else{
		lastPolyline().setClosed(true);
	}
	flagShapeChanged();
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
ofPolyline & ofPath::lastPolyline(){
	if(polylines.empty() || polylines.back().isClosed()){
		polylines.push_back(ofPolyline());
	}
	return polylines.back();
}

//----------------------------------------------------------
vector<ofPath::Command> & ofPath::getCommands(){
	if(mode==POLYLINES){
		ofLogWarning("ofPath") << "getCommands(): trying to get path commands from shape with polylines only";
	}
	return commands;
}

//----------------------------------------------------------
const vector<ofPath::Command> & ofPath::getCommands() const{
	if(mode==POLYLINES){
		ofLogWarning("ofPath") << "getCommands(): trying to get path commands from shape with polylines only";
	}
	return commands;
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
void ofPath::generatePolylinesFromCommands(){
	if(mode==POLYLINES || commands.empty()) return;
	if(bNeedsPolylinesGeneration || curveResolution!=prevCurveRes){
		prevCurveRes = curveResolution;

		polylines.clear();
		int j=-1;

		for(int i=0; i<(int)commands.size();i++){
			switch(commands[i].type){
			case Command::moveTo:
				polylines.push_back(ofPolyline());
				j++;
				polylines[j].addVertex(commands[i].to);
				break;
			case Command::lineTo:
				polylines[j].addVertex(commands[i].to);
				break;
			case Command::curveTo:
				polylines[j].curveTo(commands[i].to, curveResolution);
				break;
			case Command::bezierTo:
				polylines[j].bezierTo(commands[i].cp1,commands[i].cp2,commands[i].to, curveResolution);
				break;
			case Command::quadBezierTo:
				polylines[j].quadBezierTo(commands[i].cp1,commands[i].cp2,commands[i].to, curveResolution);
				break;
			case Command::arc:
				polylines[j].arc(commands[i].to,commands[i].radiusX,commands[i].radiusY,commands[i].angleBegin,commands[i].angleEnd, circleResolution);
				break;
			case Command::arcNegative:
				polylines[j].arcNegative(commands[i].to,commands[i].radiusX,commands[i].radiusY,commands[i].angleBegin,commands[i].angleEnd, circleResolution);
				break;
			case Command::close:
				polylines[j].setClosed(true);
				break;
			}
		}

		bNeedsPolylinesGeneration = false;
		bNeedsTessellation = true;
		cachedTessellationValid=false;
	}
}

//----------------------------------------------------------
void ofPath::tessellate(){
	generatePolylinesFromCommands();
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
		generatePolylinesFromCommands();
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
	if(mode == ofPath::COMMANDS && ofGetCurrentRenderer()->rendersPathPrimitives()){
		ofGetCurrentRenderer()->draw(*this);
	}else{
		tessellate();


		ofColor prevColor;
		if(bUseShapeColor){
			prevColor = ofGetStyle().color;
		}

		if(bFill && !cachedTessellation.getVertices().empty()){
			if(bUseShapeColor){
				ofSetColor(fillColor);
			}
			cachedTessellation.draw();
			//ofGetCurrentRenderer()->draw(cachedTessellation,bUseShapeColor,false,false);

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
	if(mode==COMMANDS){
		bHasChanged = true;
		bNeedsPolylinesGeneration = true;
	}else{
		bNeedsTessellation = true;
	}
}

bool ofPath::hasChanged(){
	if(mode==COMMANDS){
		bool changed = bHasChanged;
		bHasChanged = false;
		return changed;
	}else{
		return bNeedsTessellation;
	}
}

//----------------------------------------------------------
void ofPath::setMode(Mode _mode){
	mode = _mode;
}

//----------------------------------------------------------
ofPath::Mode ofPath::getMode(){
	return mode;
}

//----------------------------------------------------------
void ofPath::setCurveResolution(int _curveResolution){
	curveResolution = _curveResolution;
}

//----------------------------------------------------------
void ofPath::setCircleResolution(int res){
	circleResolution = res;
}

//----------------------------------------------------------
int ofPath::getCircleResolution() const {
	return circleResolution;
}

//----------------------------------------------------------
void ofPath::setArcResolution(int res){
	circleResolution = res;
}

//----------------------------------------------------------
int ofPath::getArcResolution() const {
	return circleResolution;
}

//----------------------------------------------------------
void ofPath::setUseShapeColor(bool useColor){
	bUseShapeColor = useColor;
}

//----------------------------------------------------------
bool ofPath::getUseShapeColor() const {
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
	if(mode==COMMANDS) generatePolylinesFromCommands();
	for(int i=0;i<(int)polylines.size();i++){
		polylines[i].simplify(tolerance);
	}
}

//----------------------------------------------------------
void ofPath::translate(const ofPoint & p){
	if(mode==COMMANDS){
		for(int j=0;j<(int)commands.size();j++){
			commands[j].to += p;
			if(commands[j].type==Command::bezierTo || commands[j].type==Command::quadBezierTo){
				commands[j].cp1 += p;
				commands[j].cp2 += p;
			}
		}
	}else{
		for(int i=0;i<(int)polylines.size();i++){
			for(int j=0;j<(int)polylines[i].size();j++){
				polylines[i][j] += p;
			}
		}
	}
	flagShapeChanged();
}

//----------------------------------------------------------
void ofPath::rotate(float az, const ofVec3f& axis ){
	if(mode==COMMANDS){
		for(int j=0;j<(int)commands.size();j++){
			commands[j].to.rotate(az,axis);
			if(commands[j].type==Command::bezierTo || commands[j].type==Command::quadBezierTo){
				commands[j].cp1.rotate(az,axis);
				commands[j].cp2.rotate(az,axis);
			}
			if(commands[j].type==Command::arc || commands[j].type==Command::arcNegative){
				commands[j].angleBegin += az;
				commands[j].angleEnd += az;
			}
		}
	}else{
		for(int i=0;i<(int)polylines.size();i++){
			for(int j=0;j<(int)polylines[i].size();j++){
				polylines[i][j].rotate(az,axis);
			}
		}
	}
	flagShapeChanged();
}


//----------------------------------------------------------
void ofPath::scale(float x, float y){
	if(mode==COMMANDS){
		for(int j=0;j<(int)commands.size();j++){
			commands[j].to.x*=x;
			commands[j].to.y*=y;
			if(commands[j].type==Command::bezierTo || commands[j].type==Command::quadBezierTo){
				commands[j].cp1.x*=x;
				commands[j].cp1.y*=y;
				commands[j].cp2.x*=x;
				commands[j].cp2.y*=y;
			}
			if(commands[j].type==Command::arc || commands[j].type==Command::arcNegative){
				commands[j].radiusX *= x;
				commands[j].radiusY *= y;
			}
		}
	}else{
		for(int i=0;i<(int)polylines.size();i++){
			for(int j=0;j<(int)polylines[i].size();j++){
				polylines[i][j].x*=x;
				polylines[i][j].y*=y;
			}
		}
	}
	flagShapeChanged();
}

void ofPath::addCommand(const ofPath::Command & command){
	if((commands.empty() || commands.back().type==Command::close) && command.type!=Command::moveTo){
		commands.push_back(Command(Command::moveTo,command.to));
	}
	commands.push_back(command);
}
