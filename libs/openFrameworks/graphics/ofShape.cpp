/*
 *  ofShape.cpp
 *  openFrameworks
 *
 *  Created by theo on 28/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofShape.h"
#include "ofTessellator.h"
#include "ofPath.h"
#include "ofGraphics.h"


float ofPolyline::getPerimeter() const {
	float perimeter = 0;
	int lastPosition = points.size() - 1;
	for(int i = 0; i < lastPosition; i++) {
		perimeter += points[i].distance(points[i + 1]);
	}
	if(bClosed && points.size() > 1) {
		perimeter += points[points.size() - 1].distance(points[0]);
	}
	return perimeter;
}


//This is for polygon/contour simplification - we use it to reduce the number of points needed in
//representing the letters as openGL shapes - will soon be moved to ofGraphics.cpp

// From: http://softsurfer.com/Archive/algorithm_0205/algorithm_0205.htm
// Copyright 2002, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

typedef struct{
	ofPoint P0;
	ofPoint P1;
}Segment;

// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define norm2(v)   dot(v,v)        // norm2 = squared length of vector
#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define d2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
#define d(u,v)     norm(u-v)       // distance = norm of difference

static void simplifyDP(float tol, ofPoint* v, int j, int k, int* mk ){
    if (k <= j+1) // there is nothing to simplify
        return;

    // check for adequate approximation by segment S from v[j] to v[k]
    int     maxi	= j;          // index of vertex farthest from S
    float   maxd2	= 0;         // distance squared of farthest vertex
    float   tol2	= tol * tol;  // tolerance squared
    Segment S		= {v[j], v[k]};  // segment from v[j] to v[k]
    ofPoint u;
	u				= S.P1 - S.P0;   // segment direction vector
    double  cu		= dot(u,u);     // segment length squared

    // test each vertex v[i] for max distance from S
    // compute using the Feb 2001 Algorithm's dist_ofPoint_to_Segment()
    // Note: this works in any dimension (2D, 3D, ...)
    ofPoint  w;
    ofPoint   Pb;                // base of perpendicular from v[i] to S
    float  b, cw, dv2;        // dv2 = distance v[i] to S squared

    for (int i=j+1; i<k; i++){
        // compute distance squared
        w = v[i] - S.P0;
        cw = dot(w,u);
        if ( cw <= 0 ) dv2 = d2(v[i], S.P0);
        else if ( cu <= cw ) dv2 = d2(v[i], S.P1);
        else {
            b = (float)(cw / cu);
            Pb = S.P0 + u*b;
            dv2 = d2(v[i], Pb);
        }
        // test with current max distance squared
        if (dv2 <= maxd2) continue;

        // v[i] is a new max vertex
        maxi = i;
        maxd2 = dv2;
    }
    if (maxd2 > tol2)        // error is worse than the tolerance
    {
        // split the polyline at the farthest vertex from S
        mk[maxi] = 1;      // mark v[maxi] for the simplified polyline
        // recursively simplify the two subpolylines at v[maxi]
        simplifyDP( tol, v, j, maxi, mk );  // polyline v[j] to v[maxi]
        simplifyDP( tol, v, maxi, k, mk );  // polyline v[maxi] to v[k]
    }
    // else the approximation is OK, so ignore intermediate vertices
    return;
}


void ofPolyline::simplify(float tol){

	int n = size();

	vector <ofPoint> sV;
	sV.resize(n);

    int    i, k, m, pv;            // misc counters
    float  tol2 = tol * tol;       // tolerance squared
    ofPoint * vt = new ofPoint[n];
	int * mk = new int[n];

	memset(mk, 0, sizeof(int) * n );

    // STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
    vt[0] = points[0];              // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
        if (d2(points[i], points[pv]) < tol2) continue;

        vt[k++] = points[i];
        pv = i;
    }
    if (pv < n-1) vt[k++] = points[n-1];      // finish at the end

    // STAGE 2.  Douglas-Peucker polyline simplification
    mk[0] = mk[k-1] = 1;       // mark the first and last vertices
    simplifyDP( tol, vt, 0, k-1, mk );

    // copy marked vertices to the output simplified polyline
    for (i=m=0; i<k; i++) {
        if (mk[i]) sV[m++] = vt[i];
    }

	//get rid of the unused points
	if( m < (int)sV.size() ) sV.erase( sV.begin()+m, sV.end() );

	delete [] vt;
	delete [] mk;

	points = sV;
}


ofShape::ofShape(){
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


ofShape::ofShape(const ofPath & path, int curveResolution, bool tesselate){
	setFrom(path,curveResolution,tesselate);
}

ofShape::~ofShape(){
	if(renderer)delete renderer;
}


void ofShape::clear() {
	bNeedsTessellation = true;
	polyline.clear();
	cachedTessellation.clear();
	subShapes.clear();
	bNeedsOutlineDraw = false;
}

void ofShape::close() {
	getCurrentSubShape().polyline.setClosed(true);
	newSubShape();
}

void ofShape::setPolyWindingMode( int newMode ) {
	if(polyWindingMode != newMode){
		polyWindingMode = newMode;
		bNeedsTessellation = true;
	}
}

void ofShape::tessellate(){
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


void ofShape::simplify(float tolerance){
	polyline.simplify(tolerance);
	for(int i=0;i<subShapes.size();i++){
		subShapes[i].simplify(tolerance);
	}
	bNeedsTessellation=true;
}

void ofShape::draw(float x, float y){
	ofPushMatrix();
	ofTranslate(x,y);
	if(!renderer){
		ofGetDefaultRenderer()->draw(*this);
	}else{
		renderer->draw(*this);
	}
	ofPopMatrix();
}


void ofShape::addVertex(ofPoint p){
	polyline.addVertex(p);
	bNeedsTessellation = true;
}

void ofShape::setPolyline(const ofPolyline & polyline_){
	polyline = polyline_;
	bNeedsTessellation = true;
}

vector<ofVertexData> & ofShape::getTessellation(){
	if ( bNeedsTessellation ){
		tessellate();
	}
	return cachedTessellation;
}

vector<ofPolyline> & ofShape::getOutline() {
	if ( bNeedsTessellation ){
			tessellate();
		}
	return cachedOutline;
}

vector<ofPolyline> & ofShape::getSubPolylines(){
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

void ofShape::setFilled( bool bFill ) {
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

ofShape &  ofShape::getCurrentSubShape(){
	if(subShapes.empty()){
		return *this;
	}else{
		return subShapes.back();
	}
}

ofShape & ofShape::newSubShape(){
	subShapes.push_back(ofShape());
	getCurrentSubShape().polyWindingMode = polyWindingMode;
	getCurrentSubShape().bFilled = bFilled;
	getCurrentSubShape().lineColor = lineColor;
	getCurrentSubShape().strokeWidth = strokeWidth;
	getCurrentSubShape().fillColor = fillColor;
	bNeedsTessellation = true;
	return subShapes.back();
}

void ofShape::addSubShape(const ofShape & shape){
	subShapes.push_back(shape);
	bNeedsTessellation = true;
}

void ofShape::setCircleResolution(int res){
	if (res > 1 && res != (int)circlePoints.size()){
		circlePoints.resize(res);

		float angle = 0.0f;
		float angleAdder = M_TWO_PI / (float)res;
		for (int i = 0; i < res; i++){
			circlePoints[i].x = cos(angle);
			circlePoints[i].y = sin(angle);
			circlePoints[i].z = 0.0f;
			angle += angleAdder;
		}
	}
}

void ofShape::setFrom(const ofPath & path,  int curveResolution, bool bTesselate){
	// TODO: 3D commands
	clear();
	const vector<ofPath::Command> & commands = path.getCommands();
	for(int i=0; i<(int)commands.size();i++){
		switch(commands[i].type){
		case ofPath::Command::line2DTo:
			curveVertices.clear();
			polyline.addVertex(commands[i].to);
			break;
		case ofPath::Command::curve2DTo:
			curveTo(commands[i].to, curveResolution);
			break;
		case ofPath::Command::bezier2DTo:
			curveVertices.clear();
			bezierTo(commands[i].cp1,commands[i].cp2,commands[i].to, curveResolution);
			break;
		case ofPath::Command::quadBezier2DTo:
			curveVertices.clear();
			quadBezierTo(commands[i].cp1,commands[i].cp2,commands[i].to, curveResolution);
			break;
		case ofPath::Command::arc2D:
			curveVertices.clear();
			arc(commands[i].to,commands[i].radiusX,commands[i].radiusY,commands[i].angleBegin,commands[i].angleEnd, curveResolution);
			break;

		case ofPath::Command::line3DTo:
			curveVertices.clear();
			polyline.addVertex(commands[i].to);
			polyline.setIs3D(true);
			bIs3D = true;
			break;
		case ofPath::Command::curve3DTo:
			curveTo(commands[i].to, curveResolution);
			polyline.setIs3D(true);
			bIs3D = true;
			break;
		case ofPath::Command::bezier3DTo:
			curveVertices.clear();
			bezierTo(commands[i].cp1,commands[i].cp2,commands[i].to, curveResolution);
			polyline.setIs3D(true);
			bIs3D = true;
			break;
		case ofPath::Command::arc3D:
			curveVertices.clear();
			arc(commands[i].to,commands[i].radiusX,commands[i].radiusY,commands[i].angleBegin,commands[i].angleEnd, curveResolution);
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


	for(int i=0; i<(int)path.getSubPaths().size(); i++){
		//TODO: cast to change the constness, nasty but ofPath is internally only changing the cached shape
		addSubShape(((ofPath&)path).getSubPaths()[i].getShape(curveResolution));
	}
	if(bTesselate) tessellate();
}

void ofShape::bezierTo( const ofPoint & cp1, const ofPoint & cp2, const ofPoint & to, int curveResolution ){
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

void ofShape::quadBezierTo(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int curveResolution){
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

void ofShape::curveTo( const ofPoint & to, int curveResolution ){

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

void ofShape::arc( const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int curveResolution){
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
		for(int i=0;i<circlePoints.size();i++){
			polyline.addVertex(radiusX*circlePoints[i].x+center.x,radiusY*circlePoints[i].y+center.y,center.z);
		}
		polyline.addVertex(radiusX*circlePoints[0].x+center.x,radiusY*circlePoints[0].y+center.y,center.z);
	}
	bNeedsTessellation = true;
}
