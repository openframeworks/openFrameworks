#include "ofPath.h"

using namespace std;

#if defined(TARGET_EMSCRIPTEN)
	ofTessellator ofPath::tessellator;
#elif HAS_TLS
    thread_local ofTessellator ofPath::tessellator;
#endif

ofPath::Command::Command(Type type)
:type(type){

}

//----------------------------------------------------------
ofPath::Command::Command(Type type , const glm::vec3 & p)
:type(type)
,to(p)
,cp1(glm::vec3(0))
,cp2(glm::vec3(0))
,radiusX(0)
,radiusY(0)
,angleBegin(0)
,angleEnd(0)
{}

//----------------------------------------------------------
ofPath::Command::Command(Type type , const glm::vec3 & p, const glm::vec3 & cp1, const glm::vec3 & cp2)
:type(type)
,to(p)
,cp1(cp1)
,cp2(cp2)
,radiusX(0)
,radiusY(0)
,angleBegin(0)
,angleEnd(0)
{
}

//----------------------------------------------------------
ofPath::Command::Command(Type type , const glm::vec3 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd)
:type(type)
,to(centre)
,cp1(glm::vec3(0))
,cp2(glm::vec3(0))
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
void ofPath::lineTo(const glm::vec3 & p){
	if(mode==COMMANDS){
		addCommand(Command(Command::lineTo,p));
	}else{
		lastPolyline().lineTo(p);
	}
	flagShapeChanged();
}

//----------------------------------------------------------
void ofPath::lineTo(const glm::vec2 & p){
	lineTo(glm::vec3(p,0.0));
}

//----------------------------------------------------------
void ofPath::lineTo(float x, float y, float z){
	lineTo(glm::vec3(x,y,z));
}

//----------------------------------------------------------
void ofPath::lineTo(float x, float y){
	lineTo(glm::vec3(x,y,0));
}

//----------------------------------------------------------
void ofPath::moveTo(const glm::vec3 & p){
	if(mode==COMMANDS){
		addCommand(Command(Command::moveTo,p));
	}else{
		if(lastPolyline().size()>0) newSubPath();
		lastPolyline().addVertex(p);
	}
	flagShapeChanged();
}

//----------------------------------------------------------
void ofPath::moveTo(const glm::vec2 & p){
	moveTo(glm::vec3(p, 0.0));
}

//----------------------------------------------------------
void ofPath::moveTo(float x, float y, float z){
	moveTo(glm::vec3(x,y,z));
}

//----------------------------------------------------------
void ofPath::curveTo(const glm::vec3 & p){
	if(mode==COMMANDS){
		addCommand(Command(Command::curveTo,p));
	}else{
		lastPolyline().curveTo(p,curveResolution);
	}
	flagShapeChanged();
}

//----------------------------------------------------------
void ofPath::curveTo(const glm::vec2 & p){
	curveTo(glm::vec3(p, 0.0));
}

//----------------------------------------------------------
void ofPath::curveTo(float x, float y, float z){
	curveTo(glm::vec3(x,y,z));
}

//----------------------------------------------------------
void ofPath::curveTo(float x, float y){
	curveTo(glm::vec3(x,y,0));
}

//----------------------------------------------------------
void ofPath::bezierTo(const glm::vec3 & cp1, const glm::vec3 & cp2, const glm::vec3 & p){
	if(mode==COMMANDS){
		addCommand(Command(Command::bezierTo,p,cp1,cp2));
	}else{
		lastPolyline().bezierTo(cp1,cp2,p,curveResolution);
	}
	flagShapeChanged();
}

//----------------------------------------------------------
void ofPath::bezierTo(const glm::vec2 & cp1, const glm::vec2 & cp2, const glm::vec2 & p){
	bezierTo(glm::vec3(cp1,0.0), glm::vec3(cp2,0.0), glm::vec3(p,0.0));
}

//----------------------------------------------------------
void ofPath::bezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	bezierTo(glm::vec3(cx1,cy1,0),glm::vec3(cx2,cy2,0),glm::vec3(x,y,0));
}

//----------------------------------------------------------
void ofPath::bezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	bezierTo(glm::vec3(cx1,cy1,cz1),glm::vec3(cx2,cy2,cz2),glm::vec3(x,y,z));
}

//----------------------------------------------------------
void ofPath::quadBezierTo(const glm::vec3 & cp1, const glm::vec3 & cp2, const glm::vec3 & p){
	if(mode==COMMANDS){
		addCommand(Command(Command::quadBezierTo,p,cp1,cp2));
	}else{
		lastPolyline().quadBezierTo(cp1,cp2,p,curveResolution);
	}
	flagShapeChanged();
}

//----------------------------------------------------------
void ofPath::quadBezierTo(const glm::vec2 & cp1, const glm::vec2 & cp2, const glm::vec2 & p){
	quadBezierTo(glm::vec3(cp1, 0.0), glm::vec3(cp2, 0.0), glm::vec3(p, 0.0));
}

//----------------------------------------------------------
void ofPath::quadBezierTo(float cx1, float cy1, float cx2, float cy2, float x, float y){
	quadBezierTo(glm::vec3(cx1,cy1,0),glm::vec3(cx2,cy2,0),glm::vec3(x,y,0));
}

//----------------------------------------------------------
void ofPath::quadBezierTo(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z){
	quadBezierTo(glm::vec3(cx1,cy1,cz1),glm::vec3(cx2,cy2,cz2),glm::vec3(x,y,z));
}

//----------------------------------------------------------
void ofPath::arc(const glm::vec3 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise){
    if(clockwise) {
        arc(centre,radiusX,radiusY,angleBegin,angleEnd);
    } else {
        arcNegative(centre,radiusX,radiusY,angleBegin,angleEnd);
    }
}

//----------------------------------------------------------
void ofPath::arc(const glm::vec2 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise){
	arc(glm::vec3(centre, 0.0), radiusX, radiusY, angleBegin, angleEnd, clockwise);
}

//----------------------------------------------------------
void ofPath::arc(const glm::vec3 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	if(mode==COMMANDS){
		//addCommand adds a moveTo if one hasn't been set, but in this case it is adding a moveTo to the center of the arc and not the beginning of the arc
		if(commands.empty() || commands.back().type==Command::close){
			glm::vec3 start = centre + glm::vec3( glm::cos( glm::radians(angleBegin) ) * radiusX, glm::sin( glm::radians(angleBegin) ) * radiusY, 0.0f );
			commands.push_back(Command(Command::moveTo,start));
		}
		addCommand(Command(Command::arc,centre,radiusX,radiusY,angleBegin,angleEnd));
	}else{
		lastPolyline().arc(centre,radiusX,radiusY,angleBegin,angleEnd,circleResolution);
	}
	flagShapeChanged();
}

//----------------------------------------------------------
void ofPath::arc(const glm::vec2 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(glm::vec3(centre, 0.0), radiusX, radiusY, angleBegin, angleEnd);
}

//----------------------------------------------------------
void ofPath::arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(glm::vec3(x,y,0),radiusX,radiusY,angleBegin,angleEnd);
}

//----------------------------------------------------------
void ofPath::arc(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arc(glm::vec3(x,y,z),radiusX,radiusY,angleBegin,angleEnd);
}

//----------------------------------------------------------
void ofPath::arcNegative(const glm::vec3 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	if(mode==COMMANDS){
		if(commands.empty() || commands.back().type==Command::close){
			glm::vec3 start = centre + glm::vec3( glm::cos( glm::radians(angleBegin) ) * radiusX, glm::sin( glm::radians(angleBegin) ) * radiusY, 0.0f );
			commands.push_back(Command(Command::moveTo,start));
		}
		addCommand(Command(Command::arcNegative,centre,radiusX,radiusY,angleBegin,angleEnd));
	}else{
		lastPolyline().arcNegative(centre,radiusX,radiusY,angleBegin,angleEnd,circleResolution);
	}
	flagShapeChanged();
}

//----------------------------------------------------------
void ofPath::arcNegative(const glm::vec2 & centre, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arcNegative(glm::vec3(centre,0),radiusX,radiusY,angleBegin,angleEnd);
}

//----------------------------------------------------------
void ofPath::arcNegative(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arcNegative(glm::vec3(x,y,0),radiusX,radiusY,angleBegin,angleEnd);
}

//----------------------------------------------------------
void ofPath::arcNegative(float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd){
	arcNegative(glm::vec3(x,y,z),radiusX,radiusY,angleBegin,angleEnd);
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
void ofPath::triangle(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3){
	triangle(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,p3.x,p3.y,p3.z);
}

//----------------------------------------------------------
void ofPath::triangle(const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3){
	triangle(p1.x,p1.y,0.0,p2.x,p2.y,0.0,p3.x,p3.y,0.0);
}


//----------------------------------------------------------
void ofPath::circle(float x, float y, float radius){
	circle(x,y,0.0f,radius);
}

//----------------------------------------------------------
void ofPath::circle(float x, float y, float z, float radius){
	moveTo(x + radius, y, z);
	arc(x,y,z,radius,radius,0,360);
}

//----------------------------------------------------------
void ofPath::circle(const glm::vec3 & p, float radius){
	circle(p.x,p.y,p.z,radius);
}

//----------------------------------------------------------
void ofPath::circle(const glm::vec2 & p, float radius){
	circle(p.x,p.y,0.0,radius);
}


//----------------------------------------------------------
void ofPath::ellipse(float x, float y, float width, float height){
	ellipse(x,y,0.0f,width,height);
}

//----------------------------------------------------------
void ofPath::ellipse(float x, float y, float z, float width, float height){
	arc(x,y,z,width*.5f,height*.5f,0,360);
}

//----------------------------------------------------------
void ofPath::ellipse(const glm::vec3 & p, float width, float height){
	ellipse(p.x,p.y,p.z,width,height);
}

//----------------------------------------------------------
void ofPath::ellipse(const glm::vec2 & p, float width, float height){
	ellipse(p.x,p.y,0.0,width,height);
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
void ofPath::rectangle(const glm::vec3 & p,float w,float h){
	moveTo(p);
	lineTo(p.x+w,p.y,p.z);
	lineTo(p.x+w,p.y+h,p.z);
	lineTo(p.x,p.y+h,p.z);
	close();
}

//----------------------------------------------------------
void ofPath::rectangle(const glm::vec2 & p,float w,float h){
	rectangle(glm::vec3(p,0.0), w, h);
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
void ofPath::rectRounded(const glm::vec3 & p, float w, float h, float r){
	rectRounded(p.x,p.y,p.z,w,h,r,r,r,r);
}

//----------------------------------------------------------
void ofPath::rectRounded(const glm::vec2 & p, float w, float h, float r){
	rectRounded(p.x,p.y,0.0,w,h,r,r,r,r);
}

//----------------------------------------------------------
void ofPath::rectRounded(float x, float y, float w, float h, float r){
	rectRounded(x,y,0.0f,w,h,r,r,r,r);
}

//----------------------------------------------------------
void ofPath::rectRounded(const glm::vec3 & p, float w, float h, float topLeftRadius,
														float topRightRadius,
														float bottomRightRadius,
														float bottomLeftRadius){

	rectRounded(p.x,p.y,p.z,w,h,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
}

//----------------------------------------------------------
void ofPath::rectRounded(const glm::vec2 & p, float w, float h, float topLeftRadius,
														float topRightRadius,
														float bottomRightRadius,
														float bottomLeftRadius){

	rectRounded(p.x,p.y,0.0,w,h,topLeftRadius,topRightRadius,bottomRightRadius,bottomLeftRadius);
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
		bNeedsTessellation = true;
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
	}else{
		flagShapeChanged();
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
	}
}

//----------------------------------------------------------
void ofPath::tessellate(){
	generatePolylinesFromCommands();
	if(!bNeedsTessellation || polylines.empty() || std::all_of(polylines.begin(), polylines.end(), [](const ofPolyline & p) {return p.getVertices().empty();})) return;
	if(bFill){
		tessellator.tessellateToMesh( polylines, windingMode, cachedTessellation);
	}
	if(hasOutline() && windingMode!=OF_POLY_WINDING_ODD){
		tessellator.tessellateToPolylines( polylines, windingMode, tessellatedContour);
	}
	bNeedsTessellation = false;
}

//----------------------------------------------------------
const vector<ofPolyline> & ofPath::getOutline() const{
	if(windingMode!=OF_POLY_WINDING_ODD){
		const_cast<ofPath*>(this)->tessellate();
		return tessellatedContour;
	}else{
		const_cast<ofPath*>(this)->generatePolylinesFromCommands();
		return polylines;
	}
}

//----------------------------------------------------------
const ofMesh & ofPath::getTessellation() const{
	const_cast<ofPath*>(this)->tessellate();
	return cachedTessellation;
}

//----------------------------------------------------------
void ofPath::draw(float x, float y) const{
	ofGetCurrentRenderer()->draw(*this,x,y);
}

//----------------------------------------------------------
void ofPath::draw() const{
	ofGetCurrentRenderer()->draw(*this);
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
ofPath::Mode ofPath::getMode() const {
	return mode;
}

//----------------------------------------------------------
void ofPath::setCurveResolution(int _curveResolution){
	curveResolution = _curveResolution;
}

//----------------------------------------------------------
int ofPath::getCurveResolution() const {
	return curveResolution;
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
void ofPath::translate(const glm::vec3 & p){
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
void ofPath::translate(const glm::vec2 & p){
	translate(glm::vec3(p, 0.0));
}

//----------------------------------------------------------

void ofPath::rotateDeg(float degrees, const glm::vec3& axis ){
    auto radians = ofDegToRad(degrees);
    if(mode==COMMANDS){
        for(int j=0;j<(int)commands.size();j++){
            commands[j].to = glm::rotate(commands[j].to, radians, axis);
            if(commands[j].type==Command::bezierTo || commands[j].type==Command::quadBezierTo){
                commands[j].cp1 = glm::rotate(commands[j].cp1, radians, axis);
                commands[j].cp2 = glm::rotate(commands[j].cp2, radians, axis);
            }
            if(commands[j].type==Command::arc || commands[j].type==Command::arcNegative){
                commands[j].angleBegin += degrees;
                commands[j].angleEnd += degrees;
            }
        }
    }else{
        for(int i=0;i<(int)polylines.size();i++){
            for(int j=0;j<(int)polylines[i].size();j++){
                polylines[i][j] = glm::rotate(toGlm(polylines[i][j]), radians, axis);
            }
        }
    }
    flagShapeChanged();
}

//----------------------------------------------------------
void ofPath::rotateRad(float radians, const glm::vec3& axis ){
    rotateDeg(ofRadToDeg(radians), axis);
}

//----------------------------------------------------------
void ofPath::rotate(float degrees, const glm::vec3& axis ){
    rotateDeg(degrees, axis);
}

//----------------------------------------------------------
void ofPath::rotate(float degrees, const glm::vec2& axis ){
    rotateDeg(degrees, glm::vec3(axis, 0.0));
}

//----------------------------------------------------------
void ofPath::rotateDeg(float degrees, const glm::vec2& axis){
    rotateDeg(degrees, glm::vec3(axis, 0.0));
}

//----------------------------------------------------------
void ofPath::rotateRad(float radians, const glm::vec2& axis){
    rotateRad(radians, glm::vec3(axis, 0.0));
}

//----------------------------------------------------------
void ofPath::scale(float x, float y){
	if(mode==COMMANDS){
        for(std::size_t j=0;j<commands.size();j++){
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
		for(std::size_t i=0;i<polylines.size();i++){
			for(std::size_t j=0;j<polylines[i].size();j++){
				polylines[i][j].x*=x;
				polylines[i][j].y*=y;
			}
		}
	}
	flagShapeChanged();
}

void ofPath::append(const ofPath & path){
	if(mode==COMMANDS){
		for(auto & command: path.getCommands()){
			addCommand(command);
		}
	}else{
		for(auto & poly: path.getOutline()){
			polylines.push_back(poly);
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
