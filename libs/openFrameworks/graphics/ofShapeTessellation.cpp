#include "ofShapeTessellation.h"
#include "ofTessellator.h"
#include "ofShape.h"
#include "ofGraphics.h"


ofShapeTessellation::ofShapeTessellation(){
	bFilled = ofGetStyle().bFill;
	bNeedsTessellation = true;
	polyWindingMode = ofGetStyle().polyMode;
	lineColor = ofGetStyle().color;
	fillColor = ofGetStyle().color;
	strokeWidth = 1;
	renderer = 0;
	bIs3D = true;
	clear();
}


ofShapeTessellation::ofShapeTessellation(const ofShape & path, int curveResolution, bool tesselate){
	setFrom(path,curveResolution,tesselate);
}

ofShapeTessellation::~ofShapeTessellation(){
	if(renderer)delete renderer;
}


void ofShapeTessellation::clear() {
	bNeedsTessellation = true;
	polyline.clear();
	cachedTessellation.clear();
	subShapes.clear();
	curveVertices.clear();
}

void ofShapeTessellation::close() {
	getCurrentSubShape().polyline.setClosed(true);
	newSubShape();
}

void ofShapeTessellation::setPolyWindingMode( int newMode ) {
	if(polyWindingMode != newMode){
		polyWindingMode = newMode;
		bNeedsTessellation = true;
	}
}

void ofShapeTessellation::tessellate(){
	bool bIs2D = !bIs3D;
	cachedTessellation.clear();
	cachedOutline.clear();
	if(bFilled){
		cachedTessellation = ofTessellator::tessellateToMesh( getSubPolylines(), polyWindingMode, bIs2D );
	}
	if ( hasOutline() ){
		if( polyWindingMode != OF_POLY_WINDING_ODD ) {
			cachedOutline = ofTessellator::tessellateToOutline( getSubPolylines(), polyWindingMode, bIs2D );
		}else{
			getSubPolylines();
		}
	}
	bNeedsTessellation = false;
}


void ofShapeTessellation::simplify(float tolerance){
	polyline.simplify(tolerance);
	for(int i=0;i<(int)subShapes.size();i++){
		subShapes[i].simplify(tolerance);
	}
	bNeedsTessellation=true;
}

void ofShapeTessellation::draw(float x, float y){
	ofPushMatrix();
	ofTranslate(x,y);
	if(!renderer){
		ofGetDefaultRenderer()->draw(*this);
	}else{
		renderer->draw(*this);
	}
	ofPopMatrix();
}


void ofShapeTessellation::addVertex(ofPoint p){
	getCurrentSubShape().curveVertices.clear();
	getCurrentSubShape().polyline.addVertex(p);
	bNeedsTessellation = true;
}

void ofShapeTessellation::setPolyline(const ofPolyline & polyline_){
	polyline = polyline_;
	bNeedsTessellation = true;
}

vector<ofPrimitive> & ofShapeTessellation::getTessellation(){
	if ( bNeedsTessellation ){
		tessellate();
	}
	return cachedTessellation;
}

vector<ofPolyline> & ofShapeTessellation::getOutline() {
	if ( bNeedsTessellation ){
			tessellate();
		}
	return cachedOutline;
}

vector<ofPolyline> & ofShapeTessellation::getSubPolylines(){
	if ( bNeedsTessellation ){
		cachedOutline.clear();
		cachedOutline.push_back(polyline);
		for(int i=0;i<(int)subShapes.size();i++){
			vector<ofPolyline> & subPolylines = subShapes[i].getSubPolylines();
			cachedOutline.insert(cachedOutline.end(),subPolylines.begin(),subPolylines.end());
		}
	}
	return cachedOutline;
}

void ofShapeTessellation::setFilled( bool bFill ) {
	if(bFill!=bFilled){
		bFilled = bFill;
		// if vertex info changes we already set the flag to tessellate
		// so in this case is only necesary if there wasn't a previous
		// tesselation. tesselate also clears the caches so there's no
		// invalid info
		if(bFilled && cachedTessellation.empty()){
			bNeedsTessellation = true;
		}
		if(!bFilled && cachedOutline.empty()){
			bNeedsTessellation = true;
		}
	}
}

//-------------------------------------------------------------------------
// primitive generation

ofShapeTessellation &  ofShapeTessellation::getCurrentSubShape(){
	if(subShapes.empty()){
		return *this;
	}else{
		return subShapes.back();
	}
}

ofShapeTessellation & ofShapeTessellation::newSubShape(){
	subShapes.push_back(ofShapeTessellation());
	getCurrentSubShape().polyWindingMode = polyWindingMode;
	getCurrentSubShape().bFilled = bFilled;
	getCurrentSubShape().lineColor = lineColor;
	getCurrentSubShape().strokeWidth = strokeWidth;
	getCurrentSubShape().fillColor = fillColor;
	bNeedsTessellation = true;
	return subShapes.back();
}

void ofShapeTessellation::addSubShape(const ofShapeTessellation & shape){
	subShapes.push_back(shape);
	bNeedsTessellation = true;
}

void ofShapeTessellation::setFrom(const ofShape & path,  int curveResolution, bool bTesselate){
	// TODO: 3D commands
	clear();
	/*const vector<ofShape::Command> & commands = path.getCommands();
	for(int i=0; i<(int)commands.size();i++){
		switch(commands[i].type){

		case ofShape::Command::lineTo:
			getCurrentSubShape().polyline.addVertex(commands[i].to);
			polyline.setIs3D(true);
			bIs3D = true;
			break;
		case ofShape::Command::curveTo:
			getCurrentSubShape().polyline.curveTo(commands[i].to, curveResolution);
			polyline.setIs3D(true);
			bIs3D = true;
			break;
		case ofShape::Command::bezierTo:
			getCurrentSubShape().polyline.bezierTo(commands[i].cp1,commands[i].cp2,commands[i].to, curveResolution);
			polyline.setIs3D(true);
			bIs3D = true;
			break;
		case ofShape::Command::quadBezierTo:
			getCurrentSubShape().polyline.quadBezierTo(commands[i].cp1,commands[i].cp2,commands[i].to, curveResolution);
			bIs3D = true;
			break;
		case ofShape::Command::arc:
			getCurrentSubShape().polyline.arc(commands[i].to,commands[i].radiusX,commands[i].radiusY,commands[i].angleBegin,commands[i].angleEnd, curveResolution);
			polyline.setIs3D(true);
			bIs3D = true;
			break;
		}
	}
	polyline.setClosed(path.isClosed());
	setFillColor(path.getFillColor());
	setStrokeColor(path.getStrokeColor());
	setPolyWindingMode(path.getWindingMode());
	setFilled(path.isFilled());
	setStrokeWidth(path.getStrokeWidth());


	for(int i=0; i<(int)path.getSubShapes().size(); i++){
		//TODO: cast to change the constness, nasty but ofCommand is internally only changing the cached shape
		addSubShape(((ofShape&)path).getSubShapes()[i].getTessellation(curveResolution));
	}
	if(bTesselate) tessellate();*/
}

void ofShapeTessellation::bezierTo( const ofPoint & cp1, const ofPoint & cp2, const ofPoint & to, int curveResolution ){
	// if, and only if poly vertices has points, we can make a bezier
	// from the last point
	curveVertices.clear();
	ofPolyline & polyline = getCurrentSubShape().polyline;

	// the resolultion with which we computer this bezier
	// is arbitrary, can we possibly make it dynamic?

	if (polyline.size() > 0){
		float x0 = polyline[polyline.size()-1].x;
		float y0 = polyline[polyline.size()-1].y;
		float z0 = polyline[polyline.size()-1].z;

		float   ax, bx, cx;
		float   ay, by, cy;
		float   az, bz, cz;
		float   t, t2, t3;
		float   x, y, z;

		// polynomial coefficients
		cx = 3.0f * (cp1.x - x0);
		bx = 3.0f * (cp2.x - cp1.x) - cx;
		ax = to.x - x0 - cx - bx;

		cy = 3.0f * (cp1.y - y0);
		by = 3.0f * (cp2.y - cp1.y) - cy;
		ay = to.y - y0 - cy - by;

		cz = 3.0f * (cp1.z - z0);
		bz = 3.0f * (cp2.z - cp1.z) - cz;
		az = to.z - z0 - cz - bz;

		for (int i = 0; i < curveResolution; i++){
			t 	=  (float)i / (float)(curveResolution-1);
			t2 = t * t;
			t3 = t2 * t;
			x = (ax * t3) + (bx * t2) + (cx * t) + x0;
			y = (ay * t3) + (by * t2) + (cy * t) + y0;
			z = (az * t3) + (bz * t2) + (cz * t) + z0;
			polyline.addVertex(x,y,z);
		}
	}
	bNeedsTessellation = true;
}

void ofShapeTessellation::quadBezierTo(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int curveResolution){
	curveVertices.clear();
	ofPolyline & polyline = getCurrentSubShape().polyline;
	for(int i=0; i <= curveResolution; i++){
		double t = (double)i / (double)(curveResolution);
		double a = (1.0 - t)*(1.0 - t);
		double b = 2.0 * t * (1.0 - t);
		double c = t*t;
		double x = a * x1 + b * x2 + c * x3;
		double y = a * y1 + b * y2 + c * y3;
		double z = a * z1 + b * z2 + c * z3;
		polyline.addVertex(x, y, z);
	}
}

void ofShapeTessellation::curveTo( const ofPoint & to, int curveResolution ){

	curveVertices.push_back(to);
	ofPolyline & polyline = getCurrentSubShape().polyline;

	if (curveVertices.size() == 4){

		int startPos = (int)curveVertices.size() - 4;

		float x0 = curveVertices[startPos + 0].x;
		float y0 = curveVertices[startPos + 0].y;
		float z0 = curveVertices[startPos + 0].z;
		float x1 = curveVertices[startPos + 1].x;
		float y1 = curveVertices[startPos + 1].y;
		float z1 = curveVertices[startPos + 1].z;
		float x2 = curveVertices[startPos + 2].x;
		float y2 = curveVertices[startPos + 2].y;
		float z2 = curveVertices[startPos + 2].z;
		float x3 = curveVertices[startPos + 3].x;
		float y3 = curveVertices[startPos + 3].y;
		float z3 = curveVertices[startPos + 3].z;

		float t,t2,t3;
		float x,y,z;

		for (int i = 0; i < curveResolution; i++){

			t 	=  (float)i / (float)(curveResolution-1);
			t2 	= t * t;
			t3 	= t2 * t;

			x = 0.5f * ( ( 2.0f * x1 ) +
			( -x0 + x2 ) * t +
			( 2.0f * x0 - 5.0f * x1 + 4 * x2 - x3 ) * t2 +
			( -x0 + 3.0f * x1 - 3.0f * x2 + x3 ) * t3 );

			y = 0.5f * ( ( 2.0f * y1 ) +
			( -y0 + y2 ) * t +
			( 2.0f * y0 - 5.0f * y1 + 4 * y2 - y3 ) * t2 +
			( -y0 + 3.0f * y1 - 3.0f * y2 + y3 ) * t3 );

			z = 0.5f * ( ( 2.0f * z1 ) +
			( -z0 + z2 ) * t +
			( 2.0f * z0 - 5.0f * z1 + 4 * z2 - z3 ) * t2 +
			( -z0 + 3.0f * z1 - 3.0f * z2 + z3 ) * t3 );

			polyline.addVertex(ofPoint(x,y,z));
		}
		curveVertices.pop_front();
	}
	bNeedsTessellation = true;
}

void ofShapeTessellation::arc( const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int curveResolution){
	curveVertices.clear();
	setCircleResolution(curveResolution);
	ofPolyline & polyline = getCurrentSubShape().polyline;
	float size = (angleEnd - angleBegin)/360.0f;
	float begin = angleBegin/360.0f;

	if(size<1){
		const int segments = curveResolution*size;
		float angle,sinus,cosinus;
		float segment_size = M_PI*2.0*size/(float)segments;
		angle=-(M_PI*2.0*begin);
		for( int i=0; i<segments; i++){
			polyline.addVertex(radiusX*circlePoints[i].x+center.x,radiusY*circlePoints[i].y+center.y);
			angle-=segment_size ;
		}
		angle=-(M_PI*2.0*begin);
		sinus = sin(angle);
		cosinus = cos(angle);
		polyline.addVertex(radiusX*sinus+center.x,radiusY*cosinus+center.y);
	}else{
		for(int i=0;i<(int)circlePoints.size();i++){
			polyline.addVertex(radiusX*circlePoints[i].x+center.x,radiusY*circlePoints[i].y+center.y,center.z);
		}
		polyline.addVertex(radiusX*circlePoints[0].x+center.x,radiusY*circlePoints[0].y+center.y,center.z);
	}
	bNeedsTessellation = true;
}
